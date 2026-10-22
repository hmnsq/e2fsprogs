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

#جيڪڏهن_وصف_نه_ٿيل	_LINUX_RBTREE_H
#وصف_ڪريو	_LINUX_RBTREE_H

#ضمن <stdlib.h>
#ضمن <stdint.h>
#ضمن "compiler.h"

#إذا __GNUC_PREREQ (4, 6)
#هدايت GCC diagnostic ڌڪو
#هدايت GCC diagnostic ignored "-Wunused-function"
#جيڪڏهن_پورو

بنية rb_ڳنڍ
{
	uintptr_t  rb_والد_color;
#وصف_ڪريو	RB_RED		0
#وصف_ڪريو	RB_BLACK	1
	بنية rb_ڳنڍ *rb_ساڄو;
	بنية rb_ڳنڍ *rb_کاٻو;
} __attribute__((aligned(حجم(ڊگهو))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

بنية rb_پاڙ
{
	بنية rb_ڳنڍ *rb_ڳنڍ;
};


#وصف_ڪريو ext2fs_rb_والد(r)   ((بنية rb_ڳنڍ *)((r)->rb_والد_color & ~3))
#وصف_ڪريو ext2fs_rb_color(r)   ((r)->rb_والد_color & 1)
#وصف_ڪريو ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#وصف_ڪريو ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#وصف_ڪريو ext2fs_rb_سيٽ_red(r)  افعل { (r)->rb_والد_color &= ~1; } طالما (0)
#وصف_ڪريو ext2fs_rb_سيٽ_black(r)  افعل { (r)->rb_والد_color |= 1; } طالما (0)

ساكن مضمن فراغ ext2fs_rb_سيٽ_والد(بنية rb_ڳنڍ *rb, بنية rb_ڳنڍ *p)
{
	rb->rb_والد_color = (rb->rb_والد_color & 3) | (uintptr_t)p;
}
ساكن مضمن فراغ ext2fs_rb_سيٽ_color(بنية rb_ڳنڍ *rb, صحيح color)
{
	rb->rb_والد_color = (rb->rb_والد_color & ~1) | color;
}

#وصف_ڪريو RB_پاڙ	(بنية rb_پاڙ) { عدم, }
#وصف_ڪريو	ext2fs_rb_داخلا(ptr, type, member) container_جو(ptr, type, member)

ساكن مضمن صحيح ext2fs_rb_خالي_جايون_پاڙ(بنية rb_پاڙ *پاڙ)
{
	أرجع پاڙ->rb_ڳنڍ == عدم;
}

ساكن مضمن صحيح ext2fs_rb_خالي_جايون_ڳنڍ(بنية rb_ڳنڍ *ڳنڍ)
{
	أرجع ext2fs_rb_والد(ڳنڍ) == ڳنڍ;
}

ساكن مضمن فراغ ext2fs_rb_clear_ڳنڍ(بنية rb_ڳنڍ *ڳنڍ)
{
	ext2fs_rb_سيٽ_والد(ڳنڍ, ڳنڍ);
}

خارجي فراغ ext2fs_rb_وجهو_color(بنية rb_ڳنڍ *, بنية rb_پاڙ *);
خارجي فراغ ext2fs_rb_erase(بنية rb_ڳنڍ *, بنية rb_پاڙ *);

/* Find logical next and previous nodes in a tree */
خارجي بنية rb_ڳنڍ *ext2fs_rb_ايندڙ(بنية rb_ڳنڍ *);
خارجي بنية rb_ڳنڍ *ext2fs_rb_prev(بنية rb_ڳنڍ *);
خارجي بنية rb_ڳنڍ *ext2fs_rb_پهريون(ثابت بنية rb_پاڙ *);
خارجي بنية rb_ڳنڍ *ext2fs_rb_آخري(ثابت بنية rb_پاڙ *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
خارجي فراغ ext2fs_rb_replace_ڳنڍ(بنية rb_ڳنڍ *victim, بنية rb_ڳنڍ *new_,
				 بنية rb_پاڙ *پاڙ);

ساكن مضمن فراغ ext2fs_rb_link_ڳنڍ(بنية rb_ڳنڍ * ڳنڍ,
				     بنية rb_ڳنڍ * والد,
				     بنية rb_ڳنڍ ** rb_link)
{
	ڳنڍ->rb_والد_color = (uintptr_t)والد;
	ڳنڍ->rb_کاٻو = ڳنڍ->rb_ساڄو = عدم;

	*rb_link = ڳنڍ;
}

#إذا __GNUC_PREREQ (4, 6)
#هدايت GCC diagnostic ڪڍو
#جيڪڏهن_پورو

#جيڪڏهن_پورو	/* _LINUX_RBTREE_H */
