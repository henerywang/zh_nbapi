// SPDX-License-Identifier: GPL-2.0-or-later
/* json-c wrapper
 * Copyright (C) 2015 Cumulus Networks, Inc.
 */


#include "json.h"

/*
 * This function assumes that the json keyword
 * is the *last* keyword on the line no matter
 * what.
 */

void json_array_string_add(json_object *json, const char *str)
{
	json_object_array_add(json, json_object_new_string(str));
}

void json_object_string_add(struct json_object *obj, const char *key,
			    const char *s)
{
	json_object_object_add(obj, key, json_object_new_string(s));
}

void json_object_int_add(struct json_object *obj, const char *key, int64_t i)
{
	json_object_object_add(obj, key, json_object_new_int64(i));
}

void json_object_double_add(struct json_object *obj, const char *key, double i)
{
	json_object_object_add(obj, key, json_object_new_double(i));
}

void json_object_boolean_false_add(struct json_object *obj, const char *key)
{
	json_object_object_add(obj, key, json_object_new_boolean(0));
}

void json_object_boolean_true_add(struct json_object *obj, const char *key)
{
	json_object_object_add(obj, key, json_object_new_boolean(1));
}

void json_object_boolean_add(struct json_object *obj, const char *key, bool val)
{
	json_object_object_add(obj, key, json_object_new_boolean(val));
}

struct json_object *json_object_lock(struct json_object *obj)
{
	return json_object_get(obj);
}

void json_object_free(struct json_object *obj)
{
	json_object_put(obj);
}
