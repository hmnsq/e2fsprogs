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

#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ "compiler.h"

#எனில் __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic தள்ளு
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

கட்டமைப்பு rb_முனை
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	கட்டமைப்பு rb_முனை *rb_வலது;
	கட்டமைப்பு rb_முனை *rb_இடது;
} __attribute__((aligned(அளவு(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

கட்டமைப்பு rb_வேர்
{
	கட்டமைப்பு rb_முனை *rb_முனை;
};


#define ext2fs_rb_parent(r)   ((கட்டமைப்பு rb_முனை *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  செய் { (r)->rb_parent_color &= ~1; } போது (0)
#define ext2fs_rb_set_black(r)  செய் { (r)->rb_parent_color |= 1; } போது (0)

நிலையான உள்வரி வெறுமை ext2fs_rb_set_parent(கட்டமைப்பு rb_முனை *rb, கட்டமைப்பு rb_முனை *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
நிலையான உள்வரி வெறுமை ext2fs_rb_set_color(கட்டமைப்பு rb_முனை *rb, முழு color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_வேர்	(கட்டமைப்பு rb_வேர்) { வெற்று, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

நிலையான உள்வரி முழு ext2fs_rb_empty_வேர்(கட்டமைப்பு rb_வேர் *வேர்)
{
	திரும்பு வேர்->rb_முனை == வெற்று;
}

நிலையான உள்வரி முழு ext2fs_rb_empty_முனை(கட்டமைப்பு rb_முனை *முனை)
{
	திரும்பு ext2fs_rb_parent(முனை) == முனை;
}

நிலையான உள்வரி வெறுமை ext2fs_rb_clear_முனை(கட்டமைப்பு rb_முனை *முனை)
{
	ext2fs_rb_set_parent(முனை, முனை);
}

வெளி வெறுமை ext2fs_rb_செருகு_color(கட்டமைப்பு rb_முனை *, கட்டமைப்பு rb_வேர் *);
வெளி வெறுமை ext2fs_rb_erase(கட்டமைப்பு rb_முனை *, கட்டமைப்பு rb_வேர் *);

/* Find logical next and previous nodes in a tree */
வெளி கட்டமைப்பு rb_முனை *ext2fs_rb_next(கட்டமைப்பு rb_முனை *);
வெளி கட்டமைப்பு rb_முனை *ext2fs_rb_prev(கட்டமைப்பு rb_முனை *);
வெளி கட்டமைப்பு rb_முனை *ext2fs_rb_first(மாறிலி கட்டமைப்பு rb_வேர் *);
வெளி கட்டமைப்பு rb_முனை *ext2fs_rb_last(மாறிலி கட்டமைப்பு rb_வேர் *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
வெளி வெறுமை ext2fs_rb_replace_முனை(கட்டமைப்பு rb_முனை *victim, கட்டமைப்பு rb_முனை *new_,
				 கட்டமைப்பு rb_வேர் *வேர்);

நிலையான உள்வரி வெறுமை ext2fs_rb_link_முனை(கட்டமைப்பு rb_முனை * முனை,
				     கட்டமைப்பு rb_முனை * parent,
				     கட்டமைப்பு rb_முனை ** rb_link)
{
	முனை->rb_parent_color = (uintptr_t)parent;
	முனை->rb_இடது = முனை->rb_வலது = வெற்று;

	*rb_link = முனை;
}

#எனில் __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic எடு
#endif

#endif	/* _LINUX_RBTREE_H */
