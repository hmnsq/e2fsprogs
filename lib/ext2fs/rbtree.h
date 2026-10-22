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

#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ "compiler.h"

#ଯଦି __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ଠେଲ
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

ସଂରଚନା rb_ଗଣ୍ଠି
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	ସଂରଚନା rb_ଗଣ୍ଠି *rb_ଡାହାଣ;
	ସଂରଚନା rb_ଗଣ୍ଠି *rb_ବାମ;
} __attribute__((aligned(ଆକାର(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

ସଂରଚନା rb_ମୂଳ
{
	ସଂରଚନା rb_ଗଣ୍ଠି *rb_ଗଣ୍ଠି;
};


#define ext2fs_rb_parent(r)   ((ସଂରଚନା rb_ଗଣ୍ଠି *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  କର { (r)->rb_parent_color &= ~1; } ଯେତେବେଳେ (0)
#define ext2fs_rb_set_black(r)  କର { (r)->rb_parent_color |= 1; } ଯେତେବେଳେ (0)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ ext2fs_rb_set_parent(ସଂରଚନା rb_ଗଣ୍ଠି *rb, ସଂରଚନା rb_ଗଣ୍ଠି *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ ext2fs_rb_set_color(ସଂରଚନା rb_ଗଣ୍ଠି *rb, ପୂର୍ଣ୍ଣସଂଖ୍ୟା color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_ମୂଳ	(ସଂରଚନା rb_ମୂଳ) { ଖାଲି, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_rb_empty_ମୂଳ(ସଂରଚନା rb_ମୂଳ *ମୂଳ)
{
	ଫେରନ୍ତୁ ମୂଳ->rb_ଗଣ୍ଠି == ଖାଲି;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_rb_empty_ଗଣ୍ଠି(ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
	ଫେରନ୍ତୁ ext2fs_rb_parent(ଗଣ୍ଠି) == ଗଣ୍ଠି;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ ext2fs_rb_clear_ଗଣ୍ଠି(ସଂରଚନା rb_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
	ext2fs_rb_set_parent(ଗଣ୍ଠି, ଗଣ୍ଠି);
}

ବାହ୍ୟ ଶୂନ୍ୟ ext2fs_rb_ଭର୍ତ୍ତି_color(ସଂରଚନା rb_ଗଣ୍ଠି *, ସଂରଚନା rb_ମୂଳ *);
ବାହ୍ୟ ଶୂନ୍ୟ ext2fs_rb_erase(ସଂରଚନା rb_ଗଣ୍ଠି *, ସଂରଚନା rb_ମୂଳ *);

/* Find logical next and previous nodes in a tree */
ବାହ୍ୟ ସଂରଚନା rb_ଗଣ୍ଠି *ext2fs_rb_next(ସଂରଚନା rb_ଗଣ୍ଠି *);
ବାହ୍ୟ ସଂରଚନା rb_ଗଣ୍ଠି *ext2fs_rb_prev(ସଂରଚନା rb_ଗଣ୍ଠି *);
ବାହ୍ୟ ସଂରଚନା rb_ଗଣ୍ଠି *ext2fs_rb_first(ଅଚଳ ସଂରଚନା rb_ମୂଳ *);
ବାହ୍ୟ ସଂରଚନା rb_ଗଣ୍ଠି *ext2fs_rb_last(ଅଚଳ ସଂରଚନା rb_ମୂଳ *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
ବାହ୍ୟ ଶୂନ୍ୟ ext2fs_rb_replace_ଗଣ୍ଠି(ସଂରଚନା rb_ଗଣ୍ଠି *victim, ସଂରଚନା rb_ଗଣ୍ଠି *new_,
				 ସଂରଚନା rb_ମୂଳ *ମୂଳ);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ ext2fs_rb_link_ଗଣ୍ଠି(ସଂରଚନା rb_ଗଣ୍ଠି * ଗଣ୍ଠି,
				     ସଂରଚନା rb_ଗଣ୍ଠି * parent,
				     ସଂରଚନା rb_ଗଣ୍ଠି ** rb_link)
{
	ଗଣ୍ଠି->rb_parent_color = (uintptr_t)parent;
	ଗଣ୍ଠି->rb_ବାମ = ଗଣ୍ଠି->rb_ଡାହାଣ = ଖାଲି;

	*rb_link = ଗଣ୍ଠି;
}

#ଯଦି __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic କାଢ଼
#endif

#endif	/* _LINUX_RBTREE_H */
