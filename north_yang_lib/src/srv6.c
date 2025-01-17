// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * SRv6 definitions
 * Copyright (C) 2020  Hiroki Shirokura, LINE Corporation
 */


#include "srv6.h"
#include "linklist.h"
#include "yang.h"

DEFINE_MTYPE_STATIC(LIB, SRV6_LOCATOR, "SRV6 locator");
DEFINE_MTYPE_STATIC(LIB, SRV6_LOCATOR_CHUNK, "SRV6 locator chunk");

const char *seg6local_action2str(uint32_t action)
{
	switch (action) {
	case ZEBRA_SEG6_LOCAL_ACTION_END:
		return "End";
	case ZEBRA_SEG6_LOCAL_ACTION_END_X:
		return "End.X";
	case ZEBRA_SEG6_LOCAL_ACTION_END_T:
		return "End.T";
	case ZEBRA_SEG6_LOCAL_ACTION_END_DX2:
		return "End.DX2";
	case ZEBRA_SEG6_LOCAL_ACTION_END_DX6:
		return "End.DX6";
	case ZEBRA_SEG6_LOCAL_ACTION_END_DX4:
		return "End.DX4";
	case ZEBRA_SEG6_LOCAL_ACTION_END_DT6:
		return "End.DT6";
	case ZEBRA_SEG6_LOCAL_ACTION_END_DT4:
		return "End.DT4";
	case ZEBRA_SEG6_LOCAL_ACTION_END_B6:
		return "End.B6";
	case ZEBRA_SEG6_LOCAL_ACTION_END_B6_ENCAP:
		return "End.B6.Encap";
	case ZEBRA_SEG6_LOCAL_ACTION_END_BM:
		return "End.BM";
	case ZEBRA_SEG6_LOCAL_ACTION_END_S:
		return "End.S";
	case ZEBRA_SEG6_LOCAL_ACTION_END_AS:
		return "End.AS";
	case ZEBRA_SEG6_LOCAL_ACTION_END_AM:
		return "End.AM";
	case ZEBRA_SEG6_LOCAL_ACTION_END_DT46:
		return "End.DT46";
	case ZEBRA_SEG6_LOCAL_ACTION_UNSPEC:
		return "unspec";
	default:
		return "unknown";
	}
}

int snprintf_seg6_segs(char *str,
		size_t size, const struct seg6_segs *segs)
{
	str[0] = '\0';
	for (size_t i = 0; i < segs->num_segs; i++) {
		char addr[INET6_ADDRSTRLEN];
		bool not_last = (i + 1) < segs->num_segs;

		inet_ntop(AF_INET6, &segs->segs[i], addr, sizeof(addr));
		strlcat(str, addr, size);
		strlcat(str, not_last ? "," : "", size);
	}
	return strlen(str);
}

const char *seg6local_context2str(char *str, size_t size,
				  const struct seg6local_context *ctx,
				  uint32_t action)
{
	switch (action) {

	case ZEBRA_SEG6_LOCAL_ACTION_END:
		snprintf(str, size, "USP");
		return str;

	case ZEBRA_SEG6_LOCAL_ACTION_END_X:
	case ZEBRA_SEG6_LOCAL_ACTION_END_DX6:
		snprintf(str, size, "nh6 %pI6", &ctx->nh6);
		return str;

	case ZEBRA_SEG6_LOCAL_ACTION_END_DX4:
		snprintf(str, size, "nh4 %pI4", &ctx->nh4);
		return str;

	case ZEBRA_SEG6_LOCAL_ACTION_END_T:
	case ZEBRA_SEG6_LOCAL_ACTION_END_DT6:
	case ZEBRA_SEG6_LOCAL_ACTION_END_DT4:
	case ZEBRA_SEG6_LOCAL_ACTION_END_DT46:
		snprintf(str, size, "table %u", ctx->table);
		return str;

	case ZEBRA_SEG6_LOCAL_ACTION_END_B6:
	case ZEBRA_SEG6_LOCAL_ACTION_END_B6_ENCAP:
		snprintf(str, size, "nh6 %pI6", &ctx->nh6);
		return str;
	case ZEBRA_SEG6_LOCAL_ACTION_END_DX2:
	case ZEBRA_SEG6_LOCAL_ACTION_END_BM:
	case ZEBRA_SEG6_LOCAL_ACTION_END_S:
	case ZEBRA_SEG6_LOCAL_ACTION_END_AS:
	case ZEBRA_SEG6_LOCAL_ACTION_END_AM:
	case ZEBRA_SEG6_LOCAL_ACTION_UNSPEC:
	default:
		snprintf(str, size, "unknown(%s)", __func__);
		return str;
	}
}


void srv6_locator_chunk_free(struct srv6_locator_chunk **chunk)
{
  XFREE(MTYPE_SRV6_LOCATOR_CHUNK, *chunk);
}


void srv6_locator_chunk_list_free(void *data)
{
	struct srv6_locator_chunk *chunk = data;

	srv6_locator_chunk_free(&chunk);
}

struct srv6_locator *srv6_locator_alloc(const char *name)
{
	struct srv6_locator *locator = NULL;

	locator = XCALLOC(MTYPE_SRV6_LOCATOR, sizeof(struct srv6_locator));
	strlcpy(locator->name, name, sizeof(locator->name));
	locator->chunks = list_new();
	locator->chunks->del = srv6_locator_chunk_list_free;
	return locator;
}

struct srv6_locator_chunk *srv6_locator_chunk_alloc(void)
{
	struct srv6_locator_chunk *chunk = NULL;

	chunk = XCALLOC(MTYPE_SRV6_LOCATOR_CHUNK,
			sizeof(struct srv6_locator_chunk));
	return chunk;
}

void srv6_locator_free(struct srv6_locator *locator)
{
	if (locator) {
//		QOBJ_UNREG(locator);
		list_delete(&locator->chunks);

		XFREE(MTYPE_SRV6_LOCATOR, locator);
	}
}

json_object *srv6_locator_json(const struct srv6_locator *loc)
{
	struct listnode *node;
	json_object *jo_root = NULL;


	jo_root = json_object_new_object();

	/* set name */
	json_object_string_add(jo_root, "name", loc->name);

	/* set prefix */

	/* set block_bits_length */
	json_object_int_add(jo_root, "blockBitsLength", loc->block_bits_length);

	/* set node_bits_length */
	json_object_int_add(jo_root, "nodeBitsLength", loc->node_bits_length);

	/* set function_bits_length */
	json_object_int_add(jo_root, "functionBitsLength",
			    loc->function_bits_length);

	/* set argument_bits_length */
	json_object_int_add(jo_root, "argumentBitsLength",
			    loc->argument_bits_length);

	/* set true if the locator is a Micro-segment (uSID) locator */
	if (CHECK_FLAG(loc->flags, SRV6_LOCATOR_USID))
		json_object_string_add(jo_root, "behavior", "usid");

	/* set status_up */
	json_object_boolean_add(jo_root, "statusUp",
				loc->status_up);

	return jo_root;
}

json_object *srv6_locator_detailed_json(const struct srv6_locator *loc)
{
	struct listnode *node;
	json_object *jo_root = NULL;


	jo_root = json_object_new_object();

	/* set name */
	json_object_string_add(jo_root, "name", loc->name);

	/* set prefix */

	/* set block_bits_length */
	json_object_int_add(jo_root, "blockBitsLength", loc->block_bits_length);

	/* set node_bits_length */
	json_object_int_add(jo_root, "nodeBitsLength", loc->node_bits_length);

	/* set function_bits_length */
	json_object_int_add(jo_root, "functionBitsLength",
			    loc->function_bits_length);

	/* set argument_bits_length */
	json_object_int_add(jo_root, "argumentBitsLength",
			    loc->argument_bits_length);

	/* set true if the locator is a Micro-segment (uSID) locator */
	if (CHECK_FLAG(loc->flags, SRV6_LOCATOR_USID))
		json_object_string_add(jo_root, "behavior", "usid");

	/* set algonum */
	json_object_int_add(jo_root, "algoNum", loc->algonum);

	/* set status_up */
	json_object_boolean_add(jo_root, "statusUp", loc->status_up);

	return jo_root;
}
