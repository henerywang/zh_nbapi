// SPDX-License-Identifier: ISC AND BSD-2-Clause
/*	$OpenBSD: subr_tree.c,v 1.9 2017/06/08 03:30:52 dlg Exp $ */

/*
 * Copyright 2002 Niels Provos <provos@citi.umich.edu>
 */

/*
 * Copyright (c) 2016 David Gwynne <dlg@openbsd.org>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>

#include "openbsd-tree.h"

static inline struct rb_entry *rb_n2e(const struct rb_type *t, void *node)
{
	unsigned long addr = (unsigned long)node;

	return ((struct rb_entry *)(addr + t->t_offset));
}

static inline void *rb_e2n(const struct rb_type *t, struct rb_entry *rbe)
{
	unsigned long addr = (unsigned long)rbe;

	return ((void *)(addr - t->t_offset));
}

#define RBE_LEFT(_rbe)		(_rbe)->rbt_left
#define RBE_RIGHT(_rbe)		(_rbe)->rbt_right
#define RBE_PARENT(_rbe)	(_rbe)->rbt_parent
#define RBE_COLOR(_rbe)		(_rbe)->rbt_color

#define RBH_ROOT(_rbt)		(_rbt)->rbt_root

static inline void rbe_set(struct rb_entry *rbe, struct rb_entry *parent)
{
	RBE_PARENT(rbe) = parent;
	RBE_LEFT(rbe) = RBE_RIGHT(rbe) = NULL;
	RBE_COLOR(rbe) = RB_RED;
}

static inline void rbe_set_blackred(struct rb_entry *black,
				    struct rb_entry *red)
{
	RBE_COLOR(black) = RB_BLACK;
	RBE_COLOR(red) = RB_RED;
}

static inline void rbe_augment(const struct rb_type *t, struct rb_entry *rbe)
{
	(*t->t_augment)(rb_e2n(t, rbe));
}

static inline void rbe_if_augment(const struct rb_type *t, struct rb_entry *rbe)
{
	if (t->t_augment != NULL)
		rbe_augment(t, rbe);
}

static inline void rbe_rotate_left(const struct rb_type *t,
				   struct rbt_tree *rbt, struct rb_entry *rbe)
{
	struct rb_entry *parent;
	struct rb_entry *tmp;

	tmp = RBE_RIGHT(rbe);
	RBE_RIGHT(rbe) = RBE_LEFT(tmp);
	if (RBE_RIGHT(rbe) != NULL)
		RBE_PARENT(RBE_LEFT(tmp)) = rbe;

	parent = RBE_PARENT(rbe);
	RBE_PARENT(tmp) = parent;
	if (parent != NULL) {
		if (rbe == RBE_LEFT(parent))
			RBE_LEFT(parent) = tmp;
		else
			RBE_RIGHT(parent) = tmp;
	} else
		RBH_ROOT(rbt) = tmp;

	RBE_LEFT(tmp) = rbe;
	RBE_PARENT(rbe) = tmp;

	if (t->t_augment != NULL) {
		rbe_augment(t, rbe);
		rbe_augment(t, tmp);
		parent = RBE_PARENT(tmp);
		if (parent != NULL)
			rbe_augment(t, parent);
	}
}

static inline void rbe_rotate_right(const struct rb_type *t,
				    struct rbt_tree *rbt, struct rb_entry *rbe)
{
	struct rb_entry *parent;
	struct rb_entry *tmp;

	tmp = RBE_LEFT(rbe);
	RBE_LEFT(rbe) = RBE_RIGHT(tmp);
	if (RBE_LEFT(rbe) != NULL)
		RBE_PARENT(RBE_RIGHT(tmp)) = rbe;

	parent = RBE_PARENT(rbe);
	RBE_PARENT(tmp) = parent;
	if (parent != NULL) {
		if (rbe == RBE_LEFT(parent))
			RBE_LEFT(parent) = tmp;
		else
			RBE_RIGHT(parent) = tmp;
	} else
		RBH_ROOT(rbt) = tmp;

	RBE_RIGHT(tmp) = rbe;
	RBE_PARENT(rbe) = tmp;

	if (t->t_augment != NULL) {
		rbe_augment(t, rbe);
		rbe_augment(t, tmp);
		parent = RBE_PARENT(tmp);
		if (parent != NULL)
			rbe_augment(t, parent);
	}
}

static inline void rbe_insert_color(const struct rb_type *t,
				    struct rbt_tree *rbt, struct rb_entry *rbe)
{
	struct rb_entry *parent, *gparent, *tmp;

	while ((parent = RBE_PARENT(rbe)) != NULL
	       && RBE_COLOR(parent) == RB_RED) {
		gparent = RBE_PARENT(parent);

		if (parent == RBE_LEFT(gparent)) {
			tmp = RBE_RIGHT(gparent);
			if (tmp != NULL && RBE_COLOR(tmp) == RB_RED) {
				RBE_COLOR(tmp) = RB_BLACK;
				rbe_set_blackred(parent, gparent);
				rbe = gparent;
				continue;
			}

			if (RBE_RIGHT(parent) == rbe) {
				rbe_rotate_left(t, rbt, parent);
				tmp = parent;
				parent = rbe;
				rbe = tmp;
			}

			rbe_set_blackred(parent, gparent);
			rbe_rotate_right(t, rbt, gparent);
		} else {
			tmp = RBE_LEFT(gparent);
			if (tmp != NULL && RBE_COLOR(tmp) == RB_RED) {
				RBE_COLOR(tmp) = RB_BLACK;
				rbe_set_blackred(parent, gparent);
				rbe = gparent;
				continue;
			}

			if (RBE_LEFT(parent) == rbe) {
				rbe_rotate_right(t, rbt, parent);
				tmp = parent;
				parent = rbe;
				rbe = tmp;
			}

			rbe_set_blackred(parent, gparent);
			rbe_rotate_left(t, rbt, gparent);
		}
	}

	RBE_COLOR(RBH_ROOT(rbt)) = RB_BLACK;
}

static inline void rbe_remove_color(const struct rb_type *t,
				    struct rbt_tree *rbt,
				    struct rb_entry *parent,
				    struct rb_entry *rbe)
{
	struct rb_entry *tmp;

	while ((rbe == NULL || RBE_COLOR(rbe) == RB_BLACK)
	       && rbe != RBH_ROOT(rbt) && parent) {
		if (RBE_LEFT(parent) == rbe) {
			tmp = RBE_RIGHT(parent);
			if (RBE_COLOR(tmp) == RB_RED) {
				rbe_set_blackred(tmp, parent);
				rbe_rotate_left(t, rbt, parent);
				tmp = RBE_RIGHT(parent);
			}
			if ((RBE_LEFT(tmp) == NULL
			     || RBE_COLOR(RBE_LEFT(tmp)) == RB_BLACK)
			    && (RBE_RIGHT(tmp) == NULL
				|| RBE_COLOR(RBE_RIGHT(tmp)) == RB_BLACK)) {
				RBE_COLOR(tmp) = RB_RED;
				rbe = parent;
				parent = RBE_PARENT(rbe);
			} else {
				if (RBE_RIGHT(tmp) == NULL
				    || RBE_COLOR(RBE_RIGHT(tmp)) == RB_BLACK) {
					struct rb_entry *oleft;

					oleft = RBE_LEFT(tmp);
					if (oleft != NULL)
						RBE_COLOR(oleft) = RB_BLACK;

					RBE_COLOR(tmp) = RB_RED;
					rbe_rotate_right(t, rbt, tmp);
					tmp = RBE_RIGHT(parent);
				}

				RBE_COLOR(tmp) = RBE_COLOR(parent);
				RBE_COLOR(parent) = RB_BLACK;
				if (RBE_RIGHT(tmp))
					RBE_COLOR(RBE_RIGHT(tmp)) = RB_BLACK;

				rbe_rotate_left(t, rbt, parent);
				rbe = RBH_ROOT(rbt);
				break;
			}
		} else {
			tmp = RBE_LEFT(parent);
			if (RBE_COLOR(tmp) == RB_RED) {
				rbe_set_blackred(tmp, parent);
				rbe_rotate_right(t, rbt, parent);
				tmp = RBE_LEFT(parent);
			}

			if ((RBE_LEFT(tmp) == NULL
			     || RBE_COLOR(RBE_LEFT(tmp)) == RB_BLACK)
			    && (RBE_RIGHT(tmp) == NULL
				|| RBE_COLOR(RBE_RIGHT(tmp)) == RB_BLACK)) {
				RBE_COLOR(tmp) = RB_RED;
				rbe = parent;
				parent = RBE_PARENT(rbe);
			} else {
				if (RBE_LEFT(tmp) == NULL
				    || RBE_COLOR(RBE_LEFT(tmp)) == RB_BLACK) {
					struct rb_entry *oright;

					oright = RBE_RIGHT(tmp);
					if (oright != NULL)
						RBE_COLOR(oright) = RB_BLACK;

					RBE_COLOR(tmp) = RB_RED;
					rbe_rotate_left(t, rbt, tmp);
					tmp = RBE_LEFT(parent);
				}

				RBE_COLOR(tmp) = RBE_COLOR(parent);
				RBE_COLOR(parent) = RB_BLACK;
				if (RBE_LEFT(tmp) != NULL)
					RBE_COLOR(RBE_LEFT(tmp)) = RB_BLACK;

				rbe_rotate_right(t, rbt, parent);
				rbe = RBH_ROOT(rbt);
				break;
			}
		}
	}

	if (rbe != NULL)
		RBE_COLOR(rbe) = RB_BLACK;
}

static inline struct rb_entry *
rbe_remove(const struct rb_type *t, struct rbt_tree *rbt, struct rb_entry *rbe)
{
	struct rb_entry *child, *parent, *old = rbe;
	unsigned int color;

	if (RBE_LEFT(rbe) == NULL)
		child = RBE_RIGHT(rbe);
	else if (RBE_RIGHT(rbe) == NULL)
		child = RBE_LEFT(rbe);
	else {
		struct rb_entry *tmp;

		rbe = RBE_RIGHT(rbe);
		while ((tmp = RBE_LEFT(rbe)) != NULL)
			rbe = tmp;

		child = RBE_RIGHT(rbe);
		parent = RBE_PARENT(rbe);
		color = RBE_COLOR(rbe);
		if (child != NULL)
			RBE_PARENT(child) = parent;
		if (parent != NULL) {
			if (RBE_LEFT(parent) == rbe)
				RBE_LEFT(parent) = child;
			else
				RBE_RIGHT(parent) = child;

			rbe_if_augment(t, parent);
		} else
			RBH_ROOT(rbt) = child;
		if (RBE_PARENT(rbe) == old)
			parent = rbe;
		*rbe = *old;

		tmp = RBE_PARENT(old);
		if (tmp != NULL) {
			if (RBE_LEFT(tmp) == old)
				RBE_LEFT(tmp) = rbe;
			else
				RBE_RIGHT(tmp) = rbe;

			rbe_if_augment(t, tmp);
		} else
			RBH_ROOT(rbt) = rbe;

		RBE_PARENT(RBE_LEFT(old)) = rbe;
		if (RBE_RIGHT(old))
			RBE_PARENT(RBE_RIGHT(old)) = rbe;

		if (t->t_augment != NULL && parent != NULL) {
			tmp = parent;
			do {
				rbe_augment(t, tmp);
				tmp = RBE_PARENT(tmp);
			} while (tmp != NULL);
		}

		goto color;
	}

	parent = RBE_PARENT(rbe);
	color = RBE_COLOR(rbe);

	if (child != NULL)
		RBE_PARENT(child) = parent;
	if (parent != NULL) {
		if (RBE_LEFT(parent) == rbe)
			RBE_LEFT(parent) = child;
		else
			RBE_RIGHT(parent) = child;

		rbe_if_augment(t, parent);
	} else
		RBH_ROOT(rbt) = child;
color:
	if (color == RB_BLACK)
		rbe_remove_color(t, rbt, parent, child);

	return (old);
}

void *_rb_remove(const struct rb_type *t, struct rbt_tree *rbt, void *elm)
{
	struct rb_entry *rbe = rb_n2e(t, elm);
	struct rb_entry *old;

	old = rbe_remove(t, rbt, rbe);

	return (old == NULL ? NULL : rb_e2n(t, old));
}

void *_rb_insert(const struct rb_type *t, struct rbt_tree *rbt, void *elm)
{
	struct rb_entry *rbe = rb_n2e(t, elm);
	struct rb_entry *tmp;
	struct rb_entry *parent = NULL;
	void *node;
	int comp = 0;

	tmp = RBH_ROOT(rbt);
	while (tmp != NULL) {
		parent = tmp;

		node = rb_e2n(t, tmp);
		comp = (*t->t_compare)(elm, node);
		if (comp < 0)
			tmp = RBE_LEFT(tmp);
		else if (comp > 0)
			tmp = RBE_RIGHT(tmp);
		else
			return (node);
	}

	rbe_set(rbe, parent);

	if (parent != NULL) {
		if (comp < 0)
			RBE_LEFT(parent) = rbe;
		else
			RBE_RIGHT(parent) = rbe;

		rbe_if_augment(t, parent);
	} else
		RBH_ROOT(rbt) = rbe;

	rbe_insert_color(t, rbt, rbe);

	return NULL;
}

/* Finds the node with the same key as elm */
void *_rb_find(const struct rb_type *t, const struct rbt_tree *rbt,
	       const void *key)
{
	struct rb_entry *tmp = RBH_ROOT(rbt);
	void *node;
	int comp;

	while (tmp != NULL) {
		node = rb_e2n(t, tmp);
		comp = (*t->t_compare)(key, node);
		if (comp < 0)
			tmp = RBE_LEFT(tmp);
		else if (comp > 0)
			tmp = RBE_RIGHT(tmp);
		else
			return (node);
	}

	return NULL;
}

/* Finds the first node greater than or equal to the search key */
void *_rb_nfind(const struct rb_type *t, const struct rbt_tree *rbt,
		const void *key)
{
	struct rb_entry *tmp = RBH_ROOT(rbt);
	void *node;
	void *res = NULL;
	int comp;

	while (tmp != NULL) {
		node = rb_e2n(t, tmp);
		comp = (*t->t_compare)(key, node);
		if (comp < 0) {
			res = node;
			tmp = RBE_LEFT(tmp);
		} else if (comp > 0)
			tmp = RBE_RIGHT(tmp);
		else
			return (node);
	}

	return (res);
}

void *_rb_next(const struct rb_type *t, void *elm)
{
	struct rb_entry *rbe = rb_n2e(t, elm);

	if (RBE_RIGHT(rbe) != NULL) {
		rbe = RBE_RIGHT(rbe);
		while (RBE_LEFT(rbe) != NULL)
			rbe = RBE_LEFT(rbe);
	} else {
		if (RBE_PARENT(rbe) && (rbe == RBE_LEFT(RBE_PARENT(rbe))))
			rbe = RBE_PARENT(rbe);
		else {
			while (RBE_PARENT(rbe)
			       && (rbe == RBE_RIGHT(RBE_PARENT(rbe))))
				rbe = RBE_PARENT(rbe);
			rbe = RBE_PARENT(rbe);
		}
	}

	return (rbe == NULL ? NULL : rb_e2n(t, rbe));
}

void *_rb_prev(const struct rb_type *t, void *elm)
{
	struct rb_entry *rbe = rb_n2e(t, elm);

	if (RBE_LEFT(rbe)) {
		rbe = RBE_LEFT(rbe);
		while (RBE_RIGHT(rbe))
			rbe = RBE_RIGHT(rbe);
	} else {
		if (RBE_PARENT(rbe) && (rbe == RBE_RIGHT(RBE_PARENT(rbe))))
			rbe = RBE_PARENT(rbe);
		else {
			while (RBE_PARENT(rbe)
			       && (rbe == RBE_LEFT(RBE_PARENT(rbe))))
				rbe = RBE_PARENT(rbe);
			rbe = RBE_PARENT(rbe);
		}
	}

	return (rbe == NULL ? NULL : rb_e2n(t, rbe));
}

void *_rb_root(const struct rb_type *t, const struct rbt_tree *rbt)
{
	struct rb_entry *rbe = RBH_ROOT(rbt);

	return (rbe == NULL ? rbe : rb_e2n(t, rbe));
}

void *_rb_min(const struct rb_type *t, const struct rbt_tree *rbt)
{
	struct rb_entry *rbe = RBH_ROOT(rbt);
	struct rb_entry *parent = NULL;

	while (rbe != NULL) {
		parent = rbe;
		rbe = RBE_LEFT(rbe);
	}

	return (parent == NULL ? NULL : rb_e2n(t, parent));
}

void *_rb_max(const struct rb_type *t, const struct rbt_tree *rbt)
{
	struct rb_entry *rbe = RBH_ROOT(rbt);
	struct rb_entry *parent = NULL;

	while (rbe != NULL) {
		parent = rbe;
		rbe = RBE_RIGHT(rbe);
	}

	return (parent == NULL ? NULL : rb_e2n(t, parent));
}

void *_rb_left(const struct rb_type *t, void *node)
{
	struct rb_entry *rbe = rb_n2e(t, node);
	rbe = RBE_LEFT(rbe);
	return (rbe == NULL ? NULL : rb_e2n(t, rbe));
}

void *_rb_right(const struct rb_type *t, void *node)
{
	struct rb_entry *rbe = rb_n2e(t, node);
	rbe = RBE_RIGHT(rbe);
	return (rbe == NULL ? NULL : rb_e2n(t, rbe));
}

void *_rb_parent(const struct rb_type *t, void *node)
{
	struct rb_entry *rbe = rb_n2e(t, node);
	rbe = RBE_PARENT(rbe);
	return (rbe == NULL ? NULL : rb_e2n(t, rbe));
}

void _rb_set_left(const struct rb_type *t, void *node, void *left)
{
	struct rb_entry *rbe = rb_n2e(t, node);
	struct rb_entry *rbl = (left == NULL) ? NULL : rb_n2e(t, left);

	RBE_LEFT(rbe) = rbl;
}

void _rb_set_right(const struct rb_type *t, void *node, void *right)
{
	struct rb_entry *rbe = rb_n2e(t, node);
	struct rb_entry *rbr = (right == NULL) ? NULL : rb_n2e(t, right);

	RBE_RIGHT(rbe) = rbr;
}

void _rb_set_parent(const struct rb_type *t, void *node, void *parent)
{
	struct rb_entry *rbe = rb_n2e(t, node);
	struct rb_entry *rbp = (parent == NULL) ? NULL : rb_n2e(t, parent);

	RBE_PARENT(rbe) = rbp;
}

void _rb_poison(const struct rb_type *t, void *node, unsigned long poison)
{
	struct rb_entry *rbe = rb_n2e(t, node);

	RBE_PARENT(rbe) = RBE_LEFT(rbe) = RBE_RIGHT(rbe) =
		(struct rb_entry *)poison;
}

int _rb_check(const struct rb_type *t, void *node, unsigned long poison)
{
	struct rb_entry *rbe = rb_n2e(t, node);

	return ((unsigned long)RBE_PARENT(rbe) == poison
		&& (unsigned long)RBE_LEFT(rbe) == poison
		&& (unsigned long)RBE_RIGHT(rbe) == poison);
}
