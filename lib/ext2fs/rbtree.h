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

#که_نه_تعریف_شوی	_LINUX_RBTREE_H
#تعریف_کړه	_LINUX_RBTREE_H

#ضمن <stdlib.h>
#ضمن <stdint.h>
#ضمن "compiler.h"

#إذا __GNUC_PREREQ (4, 6)
#لارښوونه GCC diagnostic ټېل
#لارښوونه GCC diagnostic ignored "-Wunused-function"
#د_که_پای

بنية rb_غوټه
{
	uintptr_t  rb_پلار_color;
#تعریف_کړه	RB_RED		0
#تعریف_کړه	RB_BLACK	1
	بنية rb_غوټه *rb_ښی;
	بنية rb_غوټه *rb_کیڼ;
} __attribute__((aligned(حجم(اوږد))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

بنية rb_ریښه
{
	بنية rb_غوټه *rb_غوټه;
};


#تعریف_کړه ext2fs_rb_پلار(r)   ((بنية rb_غوټه *)((r)->rb_پلار_color & ~3))
#تعریف_کړه ext2fs_rb_color(r)   ((r)->rb_پلار_color & 1)
#تعریف_کړه ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#تعریف_کړه ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#تعریف_کړه ext2fs_rb_سېټ_red(r)  افعل { (r)->rb_پلار_color &= ~1; } طالما (0)
#تعریف_کړه ext2fs_rb_سېټ_black(r)  افعل { (r)->rb_پلار_color |= 1; } طالما (0)

ساكن مضمن فراغ ext2fs_rb_سېټ_پلار(بنية rb_غوټه *rb, بنية rb_غوټه *p)
{
	rb->rb_پلار_color = (rb->rb_پلار_color & 3) | (uintptr_t)p;
}
ساكن مضمن فراغ ext2fs_rb_سېټ_color(بنية rb_غوټه *rb, صحيح color)
{
	rb->rb_پلار_color = (rb->rb_پلار_color & ~1) | color;
}

#تعریف_کړه RB_ریښه	(بنية rb_ریښه) { عدم, }
#تعریف_کړه	ext2fs_rb_ثبت(ptr, type, member) container_د(ptr, type, member)

ساكن مضمن صحيح ext2fs_rb_تشه_ریښه(بنية rb_ریښه *ریښه)
{
	أرجع ریښه->rb_غوټه == عدم;
}

ساكن مضمن صحيح ext2fs_rb_تشه_غوټه(بنية rb_غوټه *غوټه)
{
	أرجع ext2fs_rb_پلار(غوټه) == غوټه;
}

ساكن مضمن فراغ ext2fs_rb_clear_غوټه(بنية rb_غوټه *غوټه)
{
	ext2fs_rb_سېټ_پلار(غوټه, غوټه);
}

خارجي فراغ ext2fs_rb_ورننباسه_color(بنية rb_غوټه *, بنية rb_ریښه *);
خارجي فراغ ext2fs_rb_erase(بنية rb_غوټه *, بنية rb_ریښه *);

/* Find logical next and previous nodes in a tree */
خارجي بنية rb_غوټه *ext2fs_rb_بل(بنية rb_غوټه *);
خارجي بنية rb_غوټه *ext2fs_rb_prev(بنية rb_غوټه *);
خارجي بنية rb_غوټه *ext2fs_rb_لومړی(ثابت بنية rb_ریښه *);
خارجي بنية rb_غوټه *ext2fs_rb_وروستی(ثابت بنية rb_ریښه *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
خارجي فراغ ext2fs_rb_replace_غوټه(بنية rb_غوټه *victim, بنية rb_غوټه *new_,
				 بنية rb_ریښه *ریښه);

ساكن مضمن فراغ ext2fs_rb_link_غوټه(بنية rb_غوټه * غوټه,
				     بنية rb_غوټه * پلار,
				     بنية rb_غوټه ** rb_link)
{
	غوټه->rb_پلار_color = (uintptr_t)پلار;
	غوټه->rb_کیڼ = غوټه->rb_ښی = عدم;

	*rb_link = غوټه;
}

#إذا __GNUC_PREREQ (4, 6)
#لارښوونه GCC diagnostic راوباسه
#د_که_پای

#د_که_پای	/* _LINUX_RBTREE_H */
