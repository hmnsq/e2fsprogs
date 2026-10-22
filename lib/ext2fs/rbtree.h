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
#pragma GCC diagnostic ڈالو
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

بنية rb_گرہ
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	بنية rb_گرہ *rb_دایاں;
	بنية rb_گرہ *rb_بایاں;
} __attribute__((aligned(حجم(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

بنية rb_جڑ
{
	بنية rb_گرہ *rb_گرہ;
};


#define ext2fs_rb_parent(r)   ((بنية rb_گرہ *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  افعل { (r)->rb_parent_color &= ~1; } طالما (0)
#define ext2fs_rb_set_black(r)  افعل { (r)->rb_parent_color |= 1; } طالما (0)

ساكن مضمن فراغ ext2fs_rb_set_parent(بنية rb_گرہ *rb, بنية rb_گرہ *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
ساكن مضمن فراغ ext2fs_rb_set_color(بنية rb_گرہ *rb, صحيح color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_جڑ	(بنية rb_جڑ) { عدم, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

ساكن مضمن صحيح ext2fs_rb_empty_جڑ(بنية rb_جڑ *جڑ)
{
	أرجع جڑ->rb_گرہ == عدم;
}

ساكن مضمن صحيح ext2fs_rb_empty_گرہ(بنية rb_گرہ *گرہ)
{
	أرجع ext2fs_rb_parent(گرہ) == گرہ;
}

ساكن مضمن فراغ ext2fs_rb_clear_گرہ(بنية rb_گرہ *گرہ)
{
	ext2fs_rb_set_parent(گرہ, گرہ);
}

خارجي فراغ ext2fs_rb_داخل_کرو_color(بنية rb_گرہ *, بنية rb_جڑ *);
خارجي فراغ ext2fs_rb_erase(بنية rb_گرہ *, بنية rb_جڑ *);

/* Find logical next and previous nodes in a tree */
خارجي بنية rb_گرہ *ext2fs_rb_next(بنية rb_گرہ *);
خارجي بنية rb_گرہ *ext2fs_rb_prev(بنية rb_گرہ *);
خارجي بنية rb_گرہ *ext2fs_rb_first(ثابت بنية rb_جڑ *);
خارجي بنية rb_گرہ *ext2fs_rb_last(ثابت بنية rb_جڑ *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
خارجي فراغ ext2fs_rb_replace_گرہ(بنية rb_گرہ *victim, بنية rb_گرہ *new_,
				 بنية rb_جڑ *جڑ);

ساكن مضمن فراغ ext2fs_rb_link_گرہ(بنية rb_گرہ * گرہ,
				     بنية rb_گرہ * parent,
				     بنية rb_گرہ ** rb_link)
{
	گرہ->rb_parent_color = (uintptr_t)parent;
	گرہ->rb_بایاں = گرہ->rb_دایاں = عدم;

	*rb_link = گرہ;
}

#إذا __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic نکالو
#endif

#endif	/* _LINUX_RBTREE_H */
