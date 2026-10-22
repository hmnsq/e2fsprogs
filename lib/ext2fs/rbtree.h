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

#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব "compiler.h"

#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ঠেলো
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

কাঠামো rb_গ্রন্থি
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	কাঠামো rb_গ্রন্থি *rb_ডান;
	কাঠামো rb_গ্রন্থি *rb_বাম;
} __attribute__((aligned(আকার(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

কাঠামো rb_মূল
{
	কাঠামো rb_গ্রন্থি *rb_গ্রন্থি;
};


#define ext2fs_rb_parent(r)   ((কাঠামো rb_গ্রন্থি *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  করো { (r)->rb_parent_color &= ~1; } যখন (0)
#define ext2fs_rb_set_black(r)  করো { (r)->rb_parent_color |= 1; } যখন (0)

স্থির অন্তঃরেখ শূন্য ext2fs_rb_set_parent(কাঠামো rb_গ্রন্থি *rb, কাঠামো rb_গ্রন্থি *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
স্থির অন্তঃরেখ শূন্য ext2fs_rb_set_color(কাঠামো rb_গ্রন্থি *rb, পূর্ণসংখ্যা color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_মূল	(কাঠামো rb_মূল) { ফাঁকা, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

স্থির অন্তঃরেখ পূর্ণসংখ্যা ext2fs_rb_empty_মূল(কাঠামো rb_মূল *মূল)
{
	ফেরত মূল->rb_গ্রন্থি == ফাঁকা;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা ext2fs_rb_empty_গ্রন্থি(কাঠামো rb_গ্রন্থি *গ্রন্থি)
{
	ফেরত ext2fs_rb_parent(গ্রন্থি) == গ্রন্থি;
}

স্থির অন্তঃরেখ শূন্য ext2fs_rb_clear_গ্রন্থি(কাঠামো rb_গ্রন্থি *গ্রন্থি)
{
	ext2fs_rb_set_parent(গ্রন্থি, গ্রন্থি);
}

বাহ্যিক শূন্য ext2fs_rb_ঢোকাও_color(কাঠামো rb_গ্রন্থি *, কাঠামো rb_মূল *);
বাহ্যিক শূন্য ext2fs_rb_erase(কাঠামো rb_গ্রন্থি *, কাঠামো rb_মূল *);

/* Find logical next and previous nodes in a tree */
বাহ্যিক কাঠামো rb_গ্রন্থি *ext2fs_rb_পরবর্তী(কাঠামো rb_গ্রন্থি *);
বাহ্যিক কাঠামো rb_গ্রন্থি *ext2fs_rb_পূর্ব(কাঠামো rb_গ্রন্থি *);
বাহ্যিক কাঠামো rb_গ্রন্থি *ext2fs_rb_first(ধ্রুবক কাঠামো rb_মূল *);
বাহ্যিক কাঠামো rb_গ্রন্থি *ext2fs_rb_last(ধ্রুবক কাঠামো rb_মূল *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
বাহ্যিক শূন্য ext2fs_rb_replace_গ্রন্থি(কাঠামো rb_গ্রন্থি *victim, কাঠামো rb_গ্রন্থি *new_,
				 কাঠামো rb_মূল *মূল);

স্থির অন্তঃরেখ শূন্য ext2fs_rb_link_গ্রন্থি(কাঠামো rb_গ্রন্থি * গ্রন্থি,
				     কাঠামো rb_গ্রন্থি * parent,
				     কাঠামো rb_গ্রন্থি ** rb_link)
{
	গ্রন্থি->rb_parent_color = (uintptr_t)parent;
	গ্রন্থি->rb_বাম = গ্রন্থি->rb_ডান = ফাঁকা;

	*rb_link = গ্রন্থি;
}

#যদি __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic বের_করো
#endif

#endif	/* _LINUX_RBTREE_H */
