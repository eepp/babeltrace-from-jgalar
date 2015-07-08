/*
 * source.c
 *
 * Babeltrace Source Component
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

#include <babeltrace/compiler.h>
#include <babeltrace/plugin/source-internal.h>
#include <babeltrace/plugin/component-internal.h>

static
void bt_component_source_destroy(struct bt_component *component)
{
	struct bt_component_source *source;

	if (!component) {
		return;
	}

	source = container_of(component, struct bt_component_source, parent);
	g_free(source);
}

struct bt_component *bt_component_source_create(const char *name,
		void *private_data, bt_component_destroy_cb destroy_func,
		bt_component_source_iterator_create_cb iterator_create_cb)
{
	struct bt_component_source *source = NULL;
	enum bt_component_status ret;

	if (!iterator_create_cb) {
		goto end;
	}

	source = g_new0(struct bt_component_source, 1);
	if (!source) {
		goto end;
	}

	ret = bt_component_init(&source->parent, name, private_data,
		destroy_func, BT_COMPONENT_TYPE_SOURCE,
		bt_component_source_destroy);
	if (ret != BT_COMPONENT_STATUS_OK) {
		g_free(source);
		source = NULL;
		goto end;
	}

	source->create_iterator = iterator_create_cb;
end:
	return source ? &source->parent : NULL;
}
