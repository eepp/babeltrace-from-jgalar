#ifndef BABELTRACE_PLUGIN_SOURCE_H
#define BABELTRACE_PLUGIN_SOURCE_H

/*
 * BabelTrace - Source Plug-in
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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bt_plugin;
struct bt_notification_iterator;
typedef struct bt_iterator *(*bt_plugin_source_iterator_create_func)(struct bt_plugin *);

struct bt_notification_iterator *bt_plugin_source_get_iterator(
		struct bt_plugin *plugin);

/* Plug-in initialization functions */
struct bt_plugin *bt_plugin_source_create(void *data);
int bt_plugin_source_set_iterator_create_function(struct bt_plugin *plugin,
		bt_plugin_source_iterator_create_func func);

#ifdef __cplusplus
}
#endif

#endif /* BABELTRACE_PLUGIN_SOURCE_H */
