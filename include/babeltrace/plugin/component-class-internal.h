#ifndef BABELTRACE_PLUGIN_COMPONENT_CLASS_INTERNAL_H
#define BABELTRACE_PLUGIN_COMPONENT_CLASS_INTERNAL_H

/*
 * BabelTrace - Component Class Internal
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
#include <babeltrace/ref-internal.h>
#include <babeltrace/plugin/component-factory-internal.h>
#include <babeltrace/plugin/plugin-internal.h>

struct bt_component_class {
	struct bt_ref ref;
	enum bt_component_type type;
	GString *name;
	struct bt_plugin *plugin;
};

BT_HIDDEN
struct bt_component_class *bt_component_class_create(
		enum bt_component_type type, const char *name,
		struct bt_plugin *plugin);

BT_HIDDEN
void bt_component_class_get(struct bt_component_class *class);

BT_HIDDEN
void bt_component_class_put(struct bt_component_class *class);

#endif /* BABELTRACE_PLUGIN_COMPONENT_CLASS_INTERNAL_H */
