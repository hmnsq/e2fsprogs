/*
  Red Black Trees
  (C) 1999  Andrea Arcangeli <andrea@suse.de>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

  linux/include/linux/rbtree.h

  To use rbtrees you'll have to implement your own insert and search cores.
  This will avoid us to use callbacks and to drop dramatically performances.
  I know it's not the cleaner way,  but in C (not in C++) to get
  performances and genericity...

  Some example of insert and search follows here. The search is a plain
  normal search over an ordered tree. The insert instead must be implemented
  in two steps: First, the code must insert the element in order as a red leaf
  in the tree, and then the support library function rb_insert_color() must
  be called. Such function will do the not trivial work to rebalance the
  rbtree, if necessary.

-----------------------------------------------------------------------
static inline struct page * rb_search_page_cache(struct inode * inode,
						 unsigned long offset)
{
	struct rb_node * n = inode->i_rb_page_cache.rb_node;
	struct page * page;

	while (n)
	{
		page = rb_entry(n, struct page, rb_page_cache);

		if (offset < page->offset)
			n = n->rb_left;
		else if (offset > page->offset)
			n = n->rb_right;
		else
			return page;
	}
	return NULL;
}

static inline struct page * __rb_insert_page_cache(struct inode * inode,
						   unsigned long offset,
						   struct rb_node * node)
{
	struct rb_node ** p = &inode->i_rb_page_cache.rb_node;
	struct rb_node * parent = NULL;
	struct page * page;

	while (*p)
	{
		parent = *p;
		page = rb_entry(parent, struct page, rb_page_cache);

		if (offset < page->offset)
			p = &(*p)->rb_left;
		else if (offset > page->offset)
			p = &(*p)->rb_right;
		else
			return page;
	}

	rb_link_node(node, parent, p);

	return NULL;
}

static inline struct page * rb_insert_page_cache(struct inode * inode,
						 unsigned long offset,
						 struct rb_node * node)
{
	struct page * ret;
	if ((ret = __rb_insert_page_cache(inode, offset, node)))
		goto out;
	rb_insert_color(node, &inode->i_rb_page_cache);
 out:
	return ret;
}
-----------------------------------------------------------------------
*/

#ifndef	_LINUX_RBTREE_H
#define	_LINUX_RBTREE_H

#include <stdlib.h>
#include <stdint.h>
#include "compiler.h"

#ប្រសិនបើ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

sa_nracanaa rb_node
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	sa_nracanaa rb_node *rb_right;
	sa_nracanaa rb_node *rb_left;
} __attribute__((aligned(parimaa_na(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

sa_nracanaa rb_root
{
	sa_nracanaa rb_node *rb_node;
};


#define ext2fs_rb_parent(r)   ((sa_nracanaa rb_node *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  do { (r)->rb_parent_color &= ~1; } ខណៈពេល (0)
#define ext2fs_rb_set_black(r)  do { (r)->rb_parent_color |= 1; } ខណៈពេល (0)

static anta_hstha void ext2fs_rb_set_parent(sa_nracanaa rb_node *rb, sa_nracanaa rb_node *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
static anta_hstha void ext2fs_rb_set_color(sa_nracanaa rb_node *rb, ចំនួនគត់ color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_ROOT	(sa_nracanaa rb_root) { NULL, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

static anta_hstha ចំនួនគត់ ext2fs_rb_empty_root(sa_nracanaa rb_root *root)
{
	ត្រឡប់ root->rb_node == NULL;
}

static anta_hstha ចំនួនគត់ ext2fs_rb_empty_node(sa_nracanaa rb_node *node)
{
	ត្រឡប់ ext2fs_rb_parent(node) == node;
}

static anta_hstha void ext2fs_rb_clear_node(sa_nracanaa rb_node *node)
{
	ext2fs_rb_set_parent(node, node);
}

extern void ext2fs_rb_insert_color(sa_nracanaa rb_node *, sa_nracanaa rb_root *);
extern void ext2fs_rb_erase(sa_nracanaa rb_node *, sa_nracanaa rb_root *);

/* Find logical next and previous nodes in a tree */
extern sa_nracanaa rb_node *ext2fs_rb_next(sa_nracanaa rb_node *);
extern sa_nracanaa rb_node *ext2fs_rb_prev(sa_nracanaa rb_node *);
extern sa_nracanaa rb_node *ext2fs_rb_first(const sa_nracanaa rb_root *);
extern sa_nracanaa rb_node *ext2fs_rb_last(const sa_nracanaa rb_root *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
extern void ext2fs_rb_replace_node(sa_nracanaa rb_node *victim, sa_nracanaa rb_node *new_,
				 sa_nracanaa rb_root *root);

static anta_hstha void ext2fs_rb_link_node(sa_nracanaa rb_node * node,
				     sa_nracanaa rb_node * parent,
				     sa_nracanaa rb_node ** rb_link)
{
	node->rb_parent_color = (uintptr_t)parent;
	node->rb_left = node->rb_right = NULL;

	*rb_link = node;
}

#ប្រសិនបើ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic pop
#endif

#endif	/* _LINUX_RBTREE_H */
