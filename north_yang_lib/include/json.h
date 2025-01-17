// SPDX-License-Identifier: GPL-2.0-or-later
/* json-c wrapper
 * Copyright (C) 2015 Cumulus Networks, Inc.
 */

#ifndef _QUAGGA_JSON_H
#define _QUAGGA_JSON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdint.h>
#include <json-c/json.h>
#include "compiler.h"

/*
 * FRR style JSON iteration.
 * Usage: JSON_FOREACH(...) { ... }
 */
#define JSON_FOREACH(jo, joi, join)                                            \
	/* struct json_object *jo; */                                          \
	/* struct json_object_iterator joi; */                                 \
	/* struct json_object_iterator join; */                                \
	for ((joi) = json_object_iter_begin((jo)),                             \
	    (join) = json_object_iter_end((jo));                               \
	     json_object_iter_equal(&(joi), &(join)) == 0;                     \
	     json_object_iter_next(&(joi)))

#define JSON_OBJECT_NEW_ARRAY(json_func, fields, n)                            \
	({                                                                     \
		struct json_object *_json_array = json_object_new_array();     \
		for (int _i = 0; _i < (n); _i++)                               \
			json_object_array_add(_json_array,                     \
					      (json_func)((fields)[_i]));      \
		(_json_array);                                                 \
	})

extern void json_object_string_add(struct json_object *obj, const char *key,
				   const char *s);
extern void json_object_int_add(struct json_object *obj, const char *key,
				int64_t i);
void json_object_boolean_add(struct json_object *obj, const char *key,
			     bool val);

extern void json_object_double_add(struct json_object *obj, const char *key,
				   double i);
extern void json_object_boolean_false_add(struct json_object *obj,
					  const char *key);
extern void json_object_boolean_true_add(struct json_object *obj,
					 const char *key);
extern struct json_object *json_object_lock(struct json_object *obj);
extern void json_object_free(struct json_object *obj);
extern void json_array_string_add(json_object *json, const char *str);

/* printfrr => json helpers */


#define JSON_STR "JavaScript Object Notation\n"

/* NOTE: json-c lib has following commit 316da85 which
 * handles escape of forward slash.
 * This allows prefix  "20.0.14.0\/24":{
 * to  "20.0.14.0/24":{ some platforms do not have
 * latest copy of json-c where defining below macro.
 */

#ifndef JSON_C_TO_STRING_NOSLASHESCAPE

/**
  * Don't escape forward slashes.
  */
#define JSON_C_TO_STRING_NOSLASHESCAPE (1<<4)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _QUAGGA_JSON_H */
