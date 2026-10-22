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

#شامل <stdlib.h>
#شامل <stdint.h>
#شامل "compiler.h"

#اگر __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic افزودن
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

ساختار rb_گره
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	ساختار rb_گره *rb_راست;
	ساختار rb_گره *rb_چپ;
} __attribute__((aligned(اندازه(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

ساختار rb_ریشه
{
	ساختار rb_گره *rb_گره;
};


#define ext2fs_rb_parent(r)   ((ساختار rb_گره *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  انجامبده { (r)->rb_parent_color &= ~1; } وقتی (0)
#define ext2fs_rb_set_black(r)  انجامبده { (r)->rb_parent_color |= 1; } وقتی (0)

ایستا درونخطی تهی ext2fs_rb_set_parent(ساختار rb_گره *rb, ساختار rb_گره *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
ایستا درونخطی تهی ext2fs_rb_set_color(ساختار rb_گره *rb, صحیح color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_ریشه	(ساختار rb_ریشه) { پوچ, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

ایستا درونخطی صحیح ext2fs_rb_empty_ریشه(ساختار rb_ریشه *ریشه)
{
	بازگشت ریشه->rb_گره == پوچ;
}

ایستا درونخطی صحیح ext2fs_rb_empty_گره(ساختار rb_گره *گره)
{
	بازگشت ext2fs_rb_parent(گره) == گره;
}

ایستا درونخطی تهی ext2fs_rb_clear_گره(ساختار rb_گره *گره)
{
	ext2fs_rb_set_parent(گره, گره);
}

بیرونی تهی ext2fs_rb_درج_color(ساختار rb_گره *, ساختار rb_ریشه *);
بیرونی تهی ext2fs_rb_erase(ساختار rb_گره *, ساختار rb_ریشه *);

/* Find logical next and previous nodes in a tree */
بیرونی ساختار rb_گره *ext2fs_rb_next(ساختار rb_گره *);
بیرونی ساختار rb_گره *ext2fs_rb_prev(ساختار rb_گره *);
بیرونی ساختار rb_گره *ext2fs_rb_first(ثابت ساختار rb_ریشه *);
بیرونی ساختار rb_گره *ext2fs_rb_last(ثابت ساختار rb_ریشه *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
بیرونی تهی ext2fs_rb_replace_گره(ساختار rb_گره *victim, ساختار rb_گره *new_,
				 ساختار rb_ریشه *ریشه);

ایستا درونخطی تهی ext2fs_rb_link_گره(ساختار rb_گره * گره,
				     ساختار rb_گره * parent,
				     ساختار rb_گره ** rb_link)
{
	گره->rb_parent_color = (uintptr_t)parent;
	گره->rb_چپ = گره->rb_راست = پوچ;

	*rb_link = گره;
}

#اگر __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic برداشت
#endif

#endif	/* _LINUX_RBTREE_H */
