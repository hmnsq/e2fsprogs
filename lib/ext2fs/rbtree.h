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

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "compiler.h"

#ಯದಿ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ತಳ್ಳು
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

ಸಂರಚನಾ rb_ಗಂಟು
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	ಸಂರಚನಾ rb_ಗಂಟು *rb_ಬಲ;
	ಸಂರಚನಾ rb_ಗಂಟು *rb_ಎಡ;
} __attribute__((aligned(ಪರಿಮಾಣ(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

ಸಂರಚನಾ rb_ಬೇರು
{
	ಸಂರಚನಾ rb_ಗಂಟು *rb_ಗಂಟು;
};


#define ext2fs_rb_parent(r)   ((ಸಂರಚನಾ rb_ಗಂಟು *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  ಕುರು { (r)->rb_parent_color &= ~1; } ಯಾವತ್ (0)
#define ext2fs_rb_set_black(r)  ಕುರು { (r)->rb_parent_color |= 1; } ಯಾವತ್ (0)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ ext2fs_rb_set_parent(ಸಂರಚನಾ rb_ಗಂಟು *rb, ಸಂರಚನಾ rb_ಗಂಟು *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ ext2fs_rb_set_color(ಸಂರಚನಾ rb_ಗಂಟು *rb, ಪೂರ್ಣಾಙ್ಕ color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_ಬೇರು	(ಸಂರಚನಾ rb_ಬೇರು) { ರಿಕ್ತ, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ ext2fs_rb_empty_ಬೇರು(ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು)
{
	ಪ್ರತ್ಯಾಗಮ ಬೇರು->rb_ಗಂಟು == ರಿಕ್ತ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ ext2fs_rb_empty_ಗಂಟು(ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_rb_parent(ಗಂಟು) == ಗಂಟು;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ ext2fs_rb_clear_ಗಂಟು(ಸಂರಚನಾ rb_ಗಂಟು *ಗಂಟು)
{
	ext2fs_rb_set_parent(ಗಂಟು, ಗಂಟು);
}

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_rb_ಸೇರಿಸು_color(ಸಂರಚನಾ rb_ಗಂಟು *, ಸಂರಚನಾ rb_ಬೇರು *);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_rb_erase(ಸಂರಚನಾ rb_ಗಂಟು *, ಸಂರಚನಾ rb_ಬೇರು *);

/* Find logical next and previous nodes in a tree */
ಬಾಹ್ಯ ಸಂರಚನಾ rb_ಗಂಟು *ext2fs_rb_next(ಸಂರಚನಾ rb_ಗಂಟು *);
ಬಾಹ್ಯ ಸಂರಚನಾ rb_ಗಂಟು *ext2fs_rb_prev(ಸಂರಚನಾ rb_ಗಂಟು *);
ಬಾಹ್ಯ ಸಂರಚನಾ rb_ಗಂಟು *ext2fs_rb_first(ಧ್ರುವ ಸಂರಚನಾ rb_ಬೇರು *);
ಬಾಹ್ಯ ಸಂರಚನಾ rb_ಗಂಟು *ext2fs_rb_last(ಧ್ರುವ ಸಂರಚನಾ rb_ಬೇರು *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_rb_replace_ಗಂಟು(ಸಂರಚನಾ rb_ಗಂಟು *victim, ಸಂರಚನಾ rb_ಗಂಟು *new_,
				 ಸಂರಚನಾ rb_ಬೇರು *ಬೇರು);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ ext2fs_rb_link_ಗಂಟು(ಸಂರಚನಾ rb_ಗಂಟು * ಗಂಟು,
				     ಸಂರಚನಾ rb_ಗಂಟು * parent,
				     ಸಂರಚನಾ rb_ಗಂಟು ** rb_link)
{
	ಗಂಟು->rb_parent_color = (uintptr_t)parent;
	ಗಂಟು->rb_ಎಡ = ಗಂಟು->rb_ಬಲ = ರಿಕ್ತ;

	*rb_link = ಗಂಟು;
}

#ಯದಿ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ತೆಗೆ
#endif

#endif	/* _LINUX_RBTREE_H */
