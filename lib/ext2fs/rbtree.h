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

#اگر_تعریف_نشده	_LINUX_RBTREE_H
#تعریف_کن	_LINUX_RBTREE_H

#ضمن <stdlib.h>
#ضمن <stdint.h>
#ضمن "compiler.h"

#إذا __GNUC_PREREQ (4, 6)
#رهنمود GCC diagnostic افزودن
#رهنمود GCC diagnostic ignored "-Wunused-function"
#پایان_اگر

بنية rb_گره
{
	uintptr_t  rb_والد_color;
#تعریف_کن	RB_RED		0
#تعریف_کن	RB_BLACK	1
	بنية rb_گره *rb_راست;
	بنية rb_گره *rb_چپ;
} __attribute__((aligned(حجم(دراز))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

بنية rb_ریشه
{
	بنية rb_گره *rb_گره;
};


#تعریف_کن ext2fs_rb_والد(r)   ((بنية rb_گره *)((r)->rb_والد_color & ~3))
#تعریف_کن ext2fs_rb_color(r)   ((r)->rb_والد_color & 1)
#تعریف_کن ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#تعریف_کن ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#تعریف_کن ext2fs_rb_مجموعه_red(r)  افعل { (r)->rb_والد_color &= ~1; } طالما (0)
#تعریف_کن ext2fs_rb_مجموعه_black(r)  افعل { (r)->rb_والد_color |= 1; } طالما (0)

ساكن مضمن فراغ ext2fs_rb_مجموعه_والد(بنية rb_گره *rb, بنية rb_گره *p)
{
	rb->rb_والد_color = (rb->rb_والد_color & 3) | (uintptr_t)p;
}
ساكن مضمن فراغ ext2fs_rb_مجموعه_color(بنية rb_گره *rb, صحيح color)
{
	rb->rb_والد_color = (rb->rb_والد_color & ~1) | color;
}

#تعریف_کن RB_ریشه	(بنية rb_ریشه) { عدم, }
#تعریف_کن	ext2fs_rb_مدخل(ptr, type, member) container_از(ptr, type, member)

ساكن مضمن صحيح ext2fs_rb_خالی_ریشه(بنية rb_ریشه *ریشه)
{
	أرجع ریشه->rb_گره == عدم;
}

ساكن مضمن صحيح ext2fs_rb_خالی_گره(بنية rb_گره *گره)
{
	أرجع ext2fs_rb_والد(گره) == گره;
}

ساكن مضمن فراغ ext2fs_rb_clear_گره(بنية rb_گره *گره)
{
	ext2fs_rb_مجموعه_والد(گره, گره);
}

خارجي فراغ ext2fs_rb_درج_color(بنية rb_گره *, بنية rb_ریشه *);
خارجي فراغ ext2fs_rb_erase(بنية rb_گره *, بنية rb_ریشه *);

/* Find logical next and previous nodes in a tree */
خارجي بنية rb_گره *ext2fs_rb_بعدی(بنية rb_گره *);
خارجي بنية rb_گره *ext2fs_rb_prev(بنية rb_گره *);
خارجي بنية rb_گره *ext2fs_rb_اولی(ثابت بنية rb_ریشه *);
خارجي بنية rb_گره *ext2fs_rb_آخرین(ثابت بنية rb_ریشه *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
خارجي فراغ ext2fs_rb_replace_گره(بنية rb_گره *victim, بنية rb_گره *new_,
				 بنية rb_ریشه *ریشه);

ساكن مضمن فراغ ext2fs_rb_link_گره(بنية rb_گره * گره,
				     بنية rb_گره * والد,
				     بنية rb_گره ** rb_link)
{
	گره->rb_والد_color = (uintptr_t)والد;
	گره->rb_چپ = گره->rb_راست = عدم;

	*rb_link = گره;
}

#إذا __GNUC_PREREQ (4, 6)
#رهنمود GCC diagnostic برداشت
#پایان_اگر

#پایان_اگر	/* _LINUX_RBTREE_H */
