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

#ضمن <stdlib.h>
#ضمن <stdint.h>
#ضمن "compiler.h"

#إذا __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ادفع
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

بنية rb_عقدة
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	بنية rb_عقدة *rb_يمين;
	بنية rb_عقدة *rb_يسار;
} __attribute__((aligned(حجم(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

بنية rb_جذر
{
	بنية rb_عقدة *rb_عقدة;
};


#define ext2fs_rb_parent(r)   ((بنية rb_عقدة *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  افعل { (r)->rb_parent_color &= ~1; } بينما (0)
#define ext2fs_rb_set_black(r)  افعل { (r)->rb_parent_color |= 1; } بينما (0)

ساكن مضمن فراغ ext2fs_rb_set_parent(بنية rb_عقدة *rb, بنية rb_عقدة *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
ساكن مضمن فراغ ext2fs_rb_set_color(بنية rb_عقدة *rb, صحيح color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_جذر	(بنية rb_جذر) { عدم, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

ساكن مضمن صحيح ext2fs_rb_empty_جذر(بنية rb_جذر *جذر)
{
	إرجاع جذر->rb_عقدة == عدم;
}

ساكن مضمن صحيح ext2fs_rb_empty_عقدة(بنية rb_عقدة *عقدة)
{
	إرجاع ext2fs_rb_parent(عقدة) == عقدة;
}

ساكن مضمن فراغ ext2fs_rb_clear_عقدة(بنية rb_عقدة *عقدة)
{
	ext2fs_rb_set_parent(عقدة, عقدة);
}

خارجي فراغ ext2fs_rb_أدرج_color(بنية rb_عقدة *, بنية rb_جذر *);
خارجي فراغ ext2fs_rb_erase(بنية rb_عقدة *, بنية rb_جذر *);

/* Find logical next and previous nodes in a tree */
خارجي بنية rb_عقدة *ext2fs_rb_next(بنية rb_عقدة *);
خارجي بنية rb_عقدة *ext2fs_rb_prev(بنية rb_عقدة *);
خارجي بنية rb_عقدة *ext2fs_rb_first(ثابت بنية rb_جذر *);
خارجي بنية rb_عقدة *ext2fs_rb_last(ثابت بنية rb_جذر *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
خارجي فراغ ext2fs_rb_replace_عقدة(بنية rb_عقدة *victim, بنية rb_عقدة *new_,
				 بنية rb_جذر *جذر);

ساكن مضمن فراغ ext2fs_rb_link_عقدة(بنية rb_عقدة * عقدة,
				     بنية rb_عقدة * parent,
				     بنية rb_عقدة ** rb_link)
{
	عقدة->rb_parent_color = (uintptr_t)parent;
	عقدة->rb_يسار = عقدة->rb_يمين = عدم;

	*rb_link = عقدة;
}

#إذا __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic اسحب
#endif

#endif	/* _LINUX_RBTREE_H */
