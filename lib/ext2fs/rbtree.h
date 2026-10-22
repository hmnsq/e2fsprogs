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

#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ "compiler.h"

#જો __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ધકેલો
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

સંરચના rb_ગાંઠ
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	સંરચના rb_ગાંઠ *rb_જમણું;
	સંરચના rb_ગાંઠ *rb_ડાબું;
} __attribute__((aligned(કદ(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

સંરચના rb_મૂળ
{
	સંરચના rb_ગાંઠ *rb_ગાંઠ;
};


#define ext2fs_rb_parent(r)   ((સંરચના rb_ગાંઠ *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  કરો { (r)->rb_parent_color &= ~1; } જ્યારે (0)
#define ext2fs_rb_set_black(r)  કરો { (r)->rb_parent_color |= 1; } જ્યારે (0)

સ્થિર અંતઃરેખ શૂન્ય ext2fs_rb_set_parent(સંરચના rb_ગાંઠ *rb, સંરચના rb_ગાંઠ *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
સ્થિર અંતઃરેખ શૂન્ય ext2fs_rb_set_color(સંરચના rb_ગાંઠ *rb, પૂર્ણાંક color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_મૂળ	(સંરચના rb_મૂળ) { ખાલી, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

સ્થિર અંતઃરેખ પૂર્ણાંક ext2fs_rb_empty_મૂળ(સંરચના rb_મૂળ *મૂળ)
{
	પાછા મૂળ->rb_ગાંઠ == ખાલી;
}

સ્થિર અંતઃરેખ પૂર્ણાંક ext2fs_rb_empty_ગાંઠ(સંરચના rb_ગાંઠ *ગાંઠ)
{
	પાછા ext2fs_rb_parent(ગાંઠ) == ગાંઠ;
}

સ્થિર અંતઃરેખ શૂન્ય ext2fs_rb_clear_ગાંઠ(સંરચના rb_ગાંઠ *ગાંઠ)
{
	ext2fs_rb_set_parent(ગાંઠ, ગાંઠ);
}

બાહ્ય શૂન્ય ext2fs_rb_ઉમેરો_color(સંરચના rb_ગાંઠ *, સંરચના rb_મૂળ *);
બાહ્ય શૂન્ય ext2fs_rb_erase(સંરચના rb_ગાંઠ *, સંરચના rb_મૂળ *);

/* Find logical next and previous nodes in a tree */
બાહ્ય સંરચના rb_ગાંઠ *ext2fs_rb_next(સંરચના rb_ગાંઠ *);
બાહ્ય સંરચના rb_ગાંઠ *ext2fs_rb_prev(સંરચના rb_ગાંઠ *);
બાહ્ય સંરચના rb_ગાંઠ *ext2fs_rb_first(અચલ સંરચના rb_મૂળ *);
બાહ્ય સંરચના rb_ગાંઠ *ext2fs_rb_last(અચલ સંરચના rb_મૂળ *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
બાહ્ય શૂન્ય ext2fs_rb_replace_ગાંઠ(સંરચના rb_ગાંઠ *victim, સંરચના rb_ગાંઠ *new_,
				 સંરચના rb_મૂળ *મૂળ);

સ્થિર અંતઃરેખ શૂન્ય ext2fs_rb_link_ગાંઠ(સંરચના rb_ગાંઠ * ગાંઠ,
				     સંરચના rb_ગાંઠ * parent,
				     સંરચના rb_ગાંઠ ** rb_link)
{
	ગાંઠ->rb_parent_color = (uintptr_t)parent;
	ગાંઠ->rb_ડાબું = ગાંઠ->rb_જમણું = ખાલી;

	*rb_link = ગાંઠ;
}

#જો __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic કાઢો
#endif

#endif	/* _LINUX_RBTREE_H */
