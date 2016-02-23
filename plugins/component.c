/*
 * component.c
 *
 * Babeltrace Plugin Component
 *
 * Copyright 2015 Jérémie Galarneau <jeremie.galarneau@efficios.com>
 *
 * Author: Jérémie Galarneau <jeremie.galarneau@efficios.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <babeltrace/plugin/component.h>
#include <babeltrace/plugin/component-internal.h>
#include <babeltrace/plugin/source-internal.h>
#include <babeltrace/plugin/sink-internal.h>
#include <babeltrace/babeltrace-internal.h>
#include <babeltrace/compiler.h>
#include <babeltrace/ref.h>

static
void bt_component_destroy(struct bt_object *obj)
{
	struct bt_component *component = NULL;
	struct bt_component_class *component_class = NULL;

	if (!obj) {
		return;
	}

	component = container_of(obj, struct bt_component, base);

	/**
	 * User data is destroyed first, followed by the concrete component
	 * instance.
	 */
	assert(!component->user_data || component->user_destroy);
	component->user_destroy(component->user_data);

	g_string_free(component->name, TRUE);

	assert(component->destroy);
	component_class = component->class;

	/* Frees the component, which becomes invalid */
	component->destroy(component);
	component = NULL;

	bt_put(component_class);
}

BT_HIDDEN
enum bt_component_status bt_component_init(struct bt_component *component,
		struct bt_component_class *class, const char *name,
		bt_component_destroy_cb destroy)
{
	enum bt_component_status ret = BT_COMPONENT_STATUS_OK;

	bt_object_init(component, bt_component_destroy);
	if (!component || !class || !name || name[0] == '\0' || !destroy) {
		ret = BT_COMPONENT_STATUS_INVAL;
		goto end;
	}

	component->class = bt_get(class);
	component->name = g_string_new(name);
	if (!component->name) {
		ret = BT_COMPONENT_STATUS_NOMEM;
		goto end;
	}
	component->destroy = destroy;
end:
	return ret;
}

BT_HIDDEN
enum bt_component_type bt_component_get_type(struct bt_component *component)
{
	return component ? component->class->type : BT_COMPONENT_TYPE_UNKNOWN;
}

struct bt_component *bt_component_create(
		struct bt_component_class *component_class, const char *name)
{
	struct bt_component *component = NULL;

	if (!component_class) {
		goto end;
	}

	switch (bt_component_class_get_type(component_class))
	{
	case BT_COMPONENT_TYPE_SOURCE:
		component = bt_component_source_create(component_class, name);
		break;
	case BT_COMPONENT_TYPE_SINK:
		component = bt_component_sink_create(component_class, name);
		break;
	default:
		goto end;
	}
end:
	return component;
}

const char *bt_component_get_name(struct bt_component *component)
{
	const char *ret = NULL;

	if (!component) {
		goto end;
	}

	ret = component->name->str;
end:
	return ret;
}

enum bt_component_status bt_component_set_name(struct bt_component *component,
		const char *name)
{
	enum bt_component_status ret = BT_COMPONENT_STATUS_OK;

	if (!component || !name || name[0] == '\0') {
		ret = BT_COMPONENT_STATUS_INVAL;
		goto end;
	}

	g_string_assign(component->name, name);
end:
	return ret;
}

struct bt_component_class *bt_component_get_class(
		struct bt_component *component)
{
	return component ? bt_get(component->class) : NULL;
}

enum bt_component_status bt_component_set_error_stream(
		struct bt_component *component, FILE *stream)
{
	enum bt_component_status ret = BT_COMPONENT_STATUS_OK;

	if (!component) {
		ret = BT_COMPONENT_STATUS_INVAL;
		goto end;
	}

	component->error_stream = stream;
end:
	return ret;
}

void *bt_component_get_private_data(struct bt_component *component)
{
	return component ? component->user_data : NULL;
}

enum bt_component_status
bt_component_set_private_data(struct bt_component *component,
		void *data)
{
	enum bt_component_status ret = BT_COMPONENT_STATUS_OK;

	if (!component) {
		ret = BT_COMPONENT_STATUS_INVAL;
		goto end;
	}

	component->user_data = data;
end:
	return ret;
}
