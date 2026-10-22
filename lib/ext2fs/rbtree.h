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

#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ "compiler.h"

#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ঠেলো
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

সংরচনা rb_গ্ৰন্থি
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	সংরচনা rb_গ্ৰন্থি *rb_ডান;
	সংরচনা rb_গ্ৰন্থি *rb_বাম;
} __attribute__((aligned(পরিমাণ(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

সংরচনা rb_মূল
{
	সংরচনা rb_গ্ৰন্থি *rb_গ্ৰন্থি;
};


#define ext2fs_rb_parent(r)   ((সংরচনা rb_গ্ৰন্থি *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  কৰা { (r)->rb_parent_color &= ~1; } যেতিয়ালৈ (0)
#define ext2fs_rb_set_black(r)  কৰা { (r)->rb_parent_color |= 1; } যেতিয়ালৈ (0)

স্থির অন্তঃস্থ শূন্য ext2fs_rb_set_parent(সংরচনা rb_গ্ৰন্থি *rb, সংরচনা rb_গ্ৰন্থি *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
স্থির অন্তঃস্থ শূন্য ext2fs_rb_set_color(সংরচনা rb_গ্ৰন্থি *rb, পূর্ণাঙ্ক color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_মূল	(সংরচনা rb_মূল) { রিক্ত, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

স্থির অন্তঃস্থ পূর্ণাঙ্ক ext2fs_rb_empty_মূল(সংরচনা rb_মূল *মূল)
{
	ঘূৰাই মূল->rb_গ্ৰন্থি == রিক্ত;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক ext2fs_rb_empty_গ্ৰন্থি(সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি)
{
	ঘূৰাই ext2fs_rb_parent(গ্ৰন্থি) == গ্ৰন্থি;
}

স্থির অন্তঃস্থ শূন্য ext2fs_rb_clear_গ্ৰন্থি(সংরচনা rb_গ্ৰন্থি *গ্ৰন্থি)
{
	ext2fs_rb_set_parent(গ্ৰন্থি, গ্ৰন্থি);
}

বাহ্য শূন্য ext2fs_rb_ঢোকাও_color(সংরচনা rb_গ্ৰন্থি *, সংরচনা rb_মূল *);
বাহ্য শূন্য ext2fs_rb_erase(সংরচনা rb_গ্ৰন্থি *, সংরচনা rb_মূল *);

/* Find logical next and previous nodes in a tree */
বাহ্য সংরচনা rb_গ্ৰন্থি *ext2fs_rb_next(সংরচনা rb_গ্ৰন্থি *);
বাহ্য সংরচনা rb_গ্ৰন্থি *ext2fs_rb_prev(সংরচনা rb_গ্ৰন্থি *);
বাহ্য সংরচনা rb_গ্ৰন্থি *ext2fs_rb_first(ধ্রুৱ সংরচনা rb_মূল *);
বাহ্য সংরচনা rb_গ্ৰন্থি *ext2fs_rb_last(ধ্রুৱ সংরচনা rb_মূল *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
বাহ্য শূন্য ext2fs_rb_replace_গ্ৰন্থি(সংরচনা rb_গ্ৰন্থি *victim, সংরচনা rb_গ্ৰন্থি *new_,
				 সংরচনা rb_মূল *মূল);

স্থির অন্তঃস্থ শূন্য ext2fs_rb_link_গ্ৰন্থি(সংরচনা rb_গ্ৰন্থি * গ্ৰন্থি,
				     সংরচনা rb_গ্ৰন্থি * parent,
				     সংরচনা rb_গ্ৰন্থি ** rb_link)
{
	গ্ৰন্থি->rb_parent_color = (uintptr_t)parent;
	গ্ৰন্থি->rb_বাম = গ্ৰন্থি->rb_ডান = রিক্ত;

	*rb_link = গ্ৰন্থি;
}

#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic বেৰ_কৰো
#endif

#endif	/* _LINUX_RBTREE_H */
