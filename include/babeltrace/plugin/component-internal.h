#ifndef BABELTRACE_PLUGIN_COMPONENT_INTERNAL_H
#define BABELTRACE_PLUGIN_COMPONENT_INTERNAL_H

/*
 * BabelTrace - Component internal
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

#include <babeltrace/babeltrace-internal.h>
#include <babeltrace/plugin/plugin-system.h>
#include <babeltrace/plugin/component.h>
#include <babeltrace/ctf-writer/ref-internal.h>
#include <glib.h>
#include <stdio.h>

struct bt_component {
	struct bt_ctf_ref ref_count;
	GString *name;
	enum bt_component_type type;
	/** No ownership taken */
	FILE *error_stream;

	void *user_data;
	bt_component_destroy_cb user_data_destroy;
	bt_component_destroy_cb destroy;
};

BT_HIDDEN
enum bt_component_status bt_component_init(struct bt_component *component,
		const char *name, void *user_data,
		bt_component_destroy_cb destroy_func,
		enum bt_component_type component_type,
		bt_component_destroy_cb component_destroy);

#endif /* BABELTRACE_PLUGIN_COMPONENT_INTERNAL_H */
