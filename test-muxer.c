#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <babeltrace/plugin/plugin.h>
#include <babeltrace/graph/graph.h>
#include <babeltrace/graph/port.h>
#include <babeltrace/graph/component-class.h>
#include <babeltrace/graph/component.h>
#include <babeltrace/graph/component-source.h>
#include <babeltrace/graph/component-filter.h>
#include <babeltrace/graph/component-sink.h>
#include <babeltrace/values.h>

static
void connect_port_to_muxer(struct bt_graph *graph,
		struct bt_port *source_port,
		struct bt_component *muxer_comp)
{
	const char *muxer_port_name;
	const char *source_port_name;
	struct bt_port *free_muxer_port = NULL;
	void *conn;
	int64_t i;
	int64_t count;

	assert(muxer_comp);
	count = bt_component_filter_get_input_port_count(muxer_comp);
	assert(count >= 0);

	for (i = 0; i < count; i++) {
		struct bt_port *muxer_port =
			bt_component_filter_get_input_port_by_index(
				muxer_comp, i);

		assert(muxer_port);

		if (!bt_port_is_connected(muxer_port)) {
			BT_MOVE(free_muxer_port, muxer_port);
			break;
		} else {
			bt_put(muxer_port);
		}
	}

	assert(free_muxer_port);
	conn = bt_graph_connect_ports(graph, source_port, free_muxer_port);
	assert(conn);
	source_port_name = bt_port_get_name(source_port);
	muxer_port_name = bt_port_get_name(free_muxer_port);
	fprintf(stderr, ":: connected source port `%s` to muxer port `%s`\n",
		source_port_name, muxer_port_name);
	bt_put(conn);
	bt_put(free_muxer_port);
}

static
void port_added_listener(struct bt_port *upstream_port, void *data)
{
	struct bt_component *upstream_comp =
		bt_port_get_component(upstream_port);
	struct bt_component *muxer_comp = data;
	struct bt_graph *graph = NULL;
	const char *upstream_comp_name;

	assert(upstream_comp);
	upstream_comp_name = bt_component_get_name(upstream_comp);

	if (strcmp(upstream_comp_name, "live-comp") != 0) {
		goto end;
	}

	graph = bt_component_get_graph(upstream_comp);
	assert(graph);
	connect_port_to_muxer(graph, upstream_port, muxer_comp);

end:
	bt_put(upstream_comp);
	bt_put(graph);
	return;
}

int main(int argc, char *argv[])
{
	struct bt_graph *graph = bt_graph_create();
	struct bt_component_class *ctf_fs_cc;
	struct bt_component_class *utils_muxer_cc;
	struct bt_component_class *text_pretty_cc;
	struct bt_component *muxer_comp;
	struct bt_component *fs_comp;
	struct bt_component *pretty_comp;
	struct bt_value *params;
	struct bt_port *pretty_in_port;
	struct bt_port *muxer_out_port;
	void *conn;
	enum bt_graph_status graph_status = BT_GRAPH_STATUS_OK;
	int ret;
	size_t i;
	uint64_t count;

	assert(graph);
	ctf_fs_cc = bt_plugin_find_component_class("ctf", "fs", BT_COMPONENT_CLASS_TYPE_SOURCE);
	assert(ctf_fs_cc);
	utils_muxer_cc = bt_plugin_find_component_class("utils", "muxer", BT_COMPONENT_CLASS_TYPE_FILTER);
	assert(utils_muxer_cc);
	text_pretty_cc = bt_plugin_find_component_class("text", "pretty", BT_COMPONENT_CLASS_TYPE_SINK);
	assert(text_pretty_cc);
	muxer_comp = bt_component_create(utils_muxer_cc, "muxer-comp", NULL);
	assert(muxer_comp);
	pretty_comp = bt_component_create(text_pretty_cc, "pretty-comp", NULL);
	assert(muxer_comp);
	params = bt_value_map_create();
	assert(params);
	ret = bt_value_map_insert_string(params, "path", argv[1]);
	assert(ret == 0);
	fs_comp = bt_component_create(ctf_fs_cc, "live-comp", params);
	assert(fs_comp);
	count = bt_component_source_get_output_port_count(fs_comp);
	assert(count >= 0);
	ret = bt_graph_add_port_added_listener(graph, port_added_listener,
		muxer_comp);
	assert(ret == 0);

	for (i = 0; i < count; i++) {
		struct bt_port *port =
			bt_component_source_get_output_port_by_index(fs_comp,
				i);

		connect_port_to_muxer(graph, port, muxer_comp);
		bt_put(port);
	}

	muxer_out_port = bt_component_filter_get_output_port_by_index(muxer_comp, 0);
	assert(muxer_out_port);
	pretty_in_port = bt_component_sink_get_input_port_by_index(pretty_comp, 0);
	assert(pretty_in_port);
	conn = bt_graph_connect_ports(graph, muxer_out_port, pretty_in_port);
	assert(conn);

	while (graph_status == BT_GRAPH_STATUS_OK ||
			graph_status == BT_GRAPH_STATUS_AGAIN) {
		graph_status = bt_graph_run(graph);

		if (graph_status == BT_GRAPH_STATUS_AGAIN) {
			/* Wait 100 ms... */
			usleep(100 * 1000);
		}
	}

	assert(graph_status == BT_GRAPH_STATUS_END);
	bt_put(conn);
	bt_put(muxer_out_port);
	bt_put(pretty_in_port);
	bt_put(params);
	bt_put(pretty_comp);
	bt_put(fs_comp);
	bt_put(muxer_comp);
	bt_put(ctf_fs_cc);
	bt_put(utils_muxer_cc);
	bt_put(text_pretty_cc);
	bt_put(graph);
	return 0;
}
