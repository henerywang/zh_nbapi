// SPDX-License-Identifier: GPL-2.0-or-later
/* Hash routine.
 * Copyright (C) 1998 Kunihiro Ishiguro
 */

#include <math.h>

#include "hash.h"
#include "memory.h"
#include "linklist.h"
#include <pthread.h>


DEFINE_MTYPE_STATIC(LIB, HASH, "Hash");
DEFINE_MTYPE_STATIC(LIB, HASH_BUCKET, "Hash Bucket");
DEFINE_MTYPE_STATIC(LIB, HASH_INDEX, "Hash Index");

static pthread_mutex_t _hashes_mtx = PTHREAD_MUTEX_INITIALIZER;
static struct list *_hashes;

struct hash *hash_create_size(unsigned int size,
			      unsigned int (*hash_key)(const void *),
			      bool (*hash_cmp)(const void *, const void *),
			      const char *name)
{
	struct hash *hash;

	assert((size & (size - 1)) == 0);
	hash = XCALLOC(MTYPE_HASH, sizeof(struct hash));
	hash->index =
		XCALLOC(MTYPE_HASH_INDEX, sizeof(struct hash_bucket *) * size);
	hash->size = size;
	hash->hash_key = hash_key;
	hash->hash_cmp = hash_cmp;
	hash->count = 0;
	hash->name = name ? XSTRDUP(MTYPE_HASH, name) : NULL;
	hash->stats.empty = hash->size;

	pthread_mutex_lock(&_hashes_mtx);
    {
		if (!_hashes)
			_hashes = list_new();

		listnode_add(_hashes, hash);
	}
    pthread_mutex_unlock(&_hashes_mtx);

	return hash;
}

struct hash *hash_create(unsigned int (*hash_key)(const void *),
			 bool (*hash_cmp)(const void *, const void *),
			 const char *name)
{
	return hash_create_size(HASH_INITIAL_SIZE, hash_key, hash_cmp, name);
}

void *hash_alloc_intern(void *arg)
{
	return arg;
}

/*
 * ssq = ssq + (new^2 - old^2)
 *     = ssq + ((new + old) * (new - old))
 */
#define hash_update_ssq(hz, old, new)                                          \
	do {                                                                   \
		int _adjust = (new + old) * (new - old);                       \
		if (_adjust < 0)                                               \
			atomic_fetch_sub_explicit(&hz->stats.ssq, -_adjust,    \
						  memory_order_relaxed);       \
		else                                                           \
			atomic_fetch_add_explicit(&hz->stats.ssq, _adjust,     \
						  memory_order_relaxed);       \
	} while (0)

/* Expand hash if the chain length exceeds the threshold. */
static void hash_expand(struct hash *hash)
{
	unsigned int i, new_size;
	struct hash_bucket *hb, *hbnext, **new_index;

	new_size = hash->size * 2;

	if (hash->max_size && new_size > hash->max_size)
		return;

	new_index = XCALLOC(MTYPE_HASH_INDEX,
			    sizeof(struct hash_bucket *) * new_size);

	hash->stats.empty = new_size;

	for (i = 0; i < hash->size; i++)
		for (hb = hash->index[i]; hb; hb = hbnext) {
			unsigned int h = hb->key & (new_size - 1);

			hbnext = hb->next;
			hb->next = new_index[h];

			int oldlen = hb->next ? hb->next->len : 0;
			int newlen = oldlen + 1;

			if (newlen == 1)
				hash->stats.empty--;
			else
				hb->next->len = 0;

			hb->len = newlen;

			hash_update_ssq(hash, oldlen, newlen);

			new_index[h] = hb;
		}

	/* Switch to new table */
	XFREE(MTYPE_HASH_INDEX, hash->index);
	hash->size = new_size;
	hash->index = new_index;
}

void *hash_get(struct hash *hash, void *data, void *(*alloc_func)(void *))
{
	//frrtrace(2, frr_libfrr, hash_get, hash, data);

	unsigned int key;
	unsigned int index;
	void *newdata;
	struct hash_bucket *bucket;

	if (!alloc_func && !hash->count)
		return NULL;

	key = (*hash->hash_key)(data);
	index = key & (hash->size - 1);

	for (bucket = hash->index[index]; bucket != NULL;
	     bucket = bucket->next) {
		if (bucket->key == key && (*hash->hash_cmp)(bucket->data, data))
			return bucket->data;
	}

	if (alloc_func) {
		newdata = (*alloc_func)(data);
		if (newdata == NULL)
			return NULL;

		if (HASH_THRESHOLD(hash->count + 1, hash->size)) {
			hash_expand(hash);
			index = key & (hash->size - 1);
		}

		bucket = XCALLOC(MTYPE_HASH_BUCKET, sizeof(struct hash_bucket));
		bucket->data = newdata;
		bucket->key = key;
		bucket->next = hash->index[index];
		hash->index[index] = bucket;
		hash->count++;

		//frrtrace(3, frr_libfrr, hash_insert, hash, data, key);

		int oldlen = bucket->next ? bucket->next->len : 0;
		int newlen = oldlen + 1;

		if (newlen == 1)
			hash->stats.empty--;
		else
			bucket->next->len = 0;

		bucket->len = newlen;

		hash_update_ssq(hash, oldlen, newlen);

		return bucket->data;
	}
	return NULL;
}

void *hash_lookup(struct hash *hash, void *data)
{
	return hash_get(hash, data, NULL);
}

unsigned int string_hash_make(const char *str)
{
	unsigned int hash = 0;

	while (*str)
		hash = (hash * 33) ^ (unsigned int)*str++;

	return hash;
}

void *hash_release(struct hash *hash, void *data)
{
	void *ret = NULL;
	unsigned int key;
	unsigned int index;
	struct hash_bucket *bucket;
	struct hash_bucket *pp;

	key = (*hash->hash_key)(data);
	index = key & (hash->size - 1);

	for (bucket = pp = hash->index[index]; bucket; bucket = bucket->next) {
		if (bucket->key == key
		    && (*hash->hash_cmp)(bucket->data, data)) {
			int oldlen = hash->index[index]->len;
			int newlen = oldlen - 1;

			if (bucket == pp)
				hash->index[index] = bucket->next;
			else
				pp->next = bucket->next;

			if (hash->index[index])
				hash->index[index]->len = newlen;
			else
				hash->stats.empty++;

			hash_update_ssq(hash, oldlen, newlen);

			ret = bucket->data;
			XFREE(MTYPE_HASH_BUCKET, bucket);
			hash->count--;
			break;
		}
		pp = bucket;
	}

	//frrtrace(3, frr_libfrr, hash_release, hash, data, ret);

	return ret;
}

void hash_iterate(struct hash *hash, void (*func)(struct hash_bucket *, void *),
		  void *arg)
{
	unsigned int i;
	struct hash_bucket *hb;
	struct hash_bucket *hbnext;

	for (i = 0; i < hash->size; i++)
		for (hb = hash->index[i]; hb; hb = hbnext) {
			/* get pointer to next hash bucket here, in case (*func)
			 * decides to delete hb by calling hash_release
			 */
			hbnext = hb->next;
			(*func)(hb, arg);
		}
}

void hash_walk(struct hash *hash, int (*func)(struct hash_bucket *, void *),
	       void *arg)
{
	unsigned int i;
	struct hash_bucket *hb;
	struct hash_bucket *hbnext;
	int ret = HASHWALK_CONTINUE;

	for (i = 0; i < hash->size; i++) {
		for (hb = hash->index[i]; hb; hb = hbnext) {
			/* get pointer to next hash bucket here, in case (*func)
			 * decides to delete hb by calling hash_release
			 */
			hbnext = hb->next;
			ret = (*func)(hb, arg);
			if (ret == HASHWALK_ABORT)
				return;
		}
	}
}

void hash_clean(struct hash *hash, void (*free_func)(void *))
{
	unsigned int i;
	struct hash_bucket *hb;
	struct hash_bucket *next;

	for (i = 0; i < hash->size; i++) {
		for (hb = hash->index[i]; hb; hb = next) {
			next = hb->next;

			if (free_func)
				(*free_func)(hb->data);

			XFREE(MTYPE_HASH_BUCKET, hb);
			hash->count--;
		}
		hash->index[i] = NULL;
	}

	hash->stats.ssq = 0;
	hash->stats.empty = hash->size;
}

void hash_clean_and_free(struct hash **hash, void (*free_func)(void *))
{
	if (!*hash)
		return;

	hash_clean(*hash, free_func);
	hash_free(*hash);
	*hash = NULL;
}

static void hash_to_list_iter(struct hash_bucket *hb, void *arg)
{
	struct list *list = arg;

	listnode_add(list, hb->data);
}

struct list *hash_to_list(struct hash *hash)
{
	struct list *list = list_new();

	hash_iterate(hash, hash_to_list_iter, list);
	return list;
}

void hash_free(struct hash *hash)
{
	pthread_mutex_lock(&_hashes_mtx);
    {
		if (_hashes) {
			listnode_delete(_hashes, hash);
			if (_hashes->count == 0) {
				list_delete(&_hashes);
			}
		}
	}
    pthread_mutex_unlock(&_hashes_mtx);
	XFREE(MTYPE_HASH, hash->name);

	XFREE(MTYPE_HASH_INDEX, hash->index);
	XFREE(MTYPE_HASH, hash);
}

