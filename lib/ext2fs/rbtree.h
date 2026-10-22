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

#اگر_غیر_تعریف_شدہ	_LINUX_RBTREE_H
#تعریف_کرو	_LINUX_RBTREE_H

#شامل <stdlib.h>
#شامل <stdint.h>
#شامل "compiler.h"

#اگر __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic ڈالو
#ہدایت GCC diagnostic ignored "-Wunused-function"
#اگر_ختم

ساخت rb_گرہ
{
	uintptr_t  rb_والد_color;
#تعریف_کرو	RB_RED		0
#تعریف_کرو	RB_BLACK	1
	ساخت rb_گرہ *rb_دایاں;
	ساخت rb_گرہ *rb_بایاں;
} __attribute__((aligned(حجم(طویل))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

ساخت rb_جڑ
{
	ساخت rb_گرہ *rb_گرہ;
};


#تعریف_کرو ext2fs_rb_والد(r)   ((ساخت rb_گرہ *)((r)->rb_والد_color & ~3))
#تعریف_کرو ext2fs_rb_color(r)   ((r)->rb_والد_color & 1)
#تعریف_کرو ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#تعریف_کرو ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#تعریف_کرو ext2fs_rb_سیٹ_red(r)  کرو { (r)->rb_والد_color &= ~1; } جب (0)
#تعریف_کرو ext2fs_rb_سیٹ_black(r)  کرو { (r)->rb_والد_color |= 1; } جب (0)

ساکن اندرونی خالی ext2fs_rb_سیٹ_والد(ساخت rb_گرہ *rb, ساخت rb_گرہ *p)
{
	rb->rb_والد_color = (rb->rb_والد_color & 3) | (uintptr_t)p;
}
ساکن اندرونی خالی ext2fs_rb_سیٹ_color(ساخت rb_گرہ *rb, صحیح color)
{
	rb->rb_والد_color = (rb->rb_والد_color & ~1) | color;
}

#تعریف_کرو RB_جڑ	(ساخت rb_جڑ) { تہی, }
#تعریف_کرو	ext2fs_rb_اندراج(ptr, type, member) container_کا(ptr, type, member)

ساکن اندرونی صحیح ext2fs_rb_خالی_خانے_جڑ(ساخت rb_جڑ *جڑ)
{
	واپس جڑ->rb_گرہ == تہی;
}

ساکن اندرونی صحیح ext2fs_rb_خالی_خانے_گرہ(ساخت rb_گرہ *گرہ)
{
	واپس ext2fs_rb_والد(گرہ) == گرہ;
}

ساکن اندرونی خالی ext2fs_rb_clear_گرہ(ساخت rb_گرہ *گرہ)
{
	ext2fs_rb_سیٹ_والد(گرہ, گرہ);
}

خارجی خالی ext2fs_rb_داخل_کرو_color(ساخت rb_گرہ *, ساخت rb_جڑ *);
خارجی خالی ext2fs_rb_erase(ساخت rb_گرہ *, ساخت rb_جڑ *);

/* Find logical next and previous nodes in a tree */
خارجی ساخت rb_گرہ *ext2fs_rb_اگلا(ساخت rb_گرہ *);
خارجی ساخت rb_گرہ *ext2fs_rb_پچھلا(ساخت rb_گرہ *);
خارجی ساخت rb_گرہ *ext2fs_rb_پہلا(ثابت ساخت rb_جڑ *);
خارجی ساخت rb_گرہ *ext2fs_rb_آخری(ثابت ساخت rb_جڑ *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
خارجی خالی ext2fs_rb_replace_گرہ(ساخت rb_گرہ *victim, ساخت rb_گرہ *new_,
				 ساخت rb_جڑ *جڑ);

ساکن اندرونی خالی ext2fs_rb_link_گرہ(ساخت rb_گرہ * گرہ,
				     ساخت rb_گرہ * والد,
				     ساخت rb_گرہ ** rb_link)
{
	گرہ->rb_والد_color = (uintptr_t)والد;
	گرہ->rb_بایاں = گرہ->rb_دایاں = تہی;

	*rb_link = گرہ;
}

#اگر __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic نکالو
#اگر_ختم

#اگر_ختم	/* _LINUX_RBTREE_H */
