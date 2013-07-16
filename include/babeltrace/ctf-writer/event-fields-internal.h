#ifndef _BABELTRACE_CTF_WRITER_EVENT_FIELDS_INTERNAL_H
#define _BABELTRACE_CTF_WRITER_EVENT_FIELDS_INTERNAL_H

/*
 * BabelTrace - CTF Writer
 *
 * CTF Writer Event Fields
 *
 * Copyright 2013 EfficiOS Inc. and Linux Foundation
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

#include <babeltrace/ctf-writer/ref-internal.h>
#include <babeltrace/ctf-writer/event-fields.h>
#include <glib.h>

struct bt_ctf_field {
	struct bt_ctf_ref ref_count;
	struct bt_ctf_field_type *type;
};

struct bt_ctf_field_integer {
	struct bt_ctf_field parent;
	union {
		int64_t _signed, _unsigned;
	} payload;
};

struct bt_ctf_field_enumeration {
	struct bt_ctf_field parent;
	int64_t payload;
};

struct bt_ctf_field_floating_point {
	struct bt_ctf_field parent;
	long double payload;
};

struct bt_ctf_field_structure {
	struct bt_ctf_field parent;
	GHashTable *field_name_to_index;
	GPtrArray *fields; /* Array of pointers to struct bt_ctf_field */
};

struct bt_ctf_field_variant {
	struct bt_ctf_field parent;
	struct bt_ctf_field *tag;
	struct bt_ctf_field *payload;
};

struct bt_ctf_field_array {
	struct bt_ctf_field parent;
	GPtrArray *elements; /* Array of pointers to struct bt_ctf_field */
};

struct bt_ctf_field_sequence {
	struct bt_ctf_field parent;
	struct bt_ctf_field *length;
	GPtrArray *elements; /* Array of pointers to struct bt_ctf_field */
};

struct bt_ctf_field_string {
	struct bt_ctf_field parent;
	GString *payload;
};

#endif /* _BABELTRACE_CTF_WRITER_EVENT_FIELDS_INTERNAL_H */
