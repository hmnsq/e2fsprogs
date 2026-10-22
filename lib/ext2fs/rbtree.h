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

#నిర్వచితం_కాకపోతే	_LINUX_RBTREE_H
#నిర్వచించు	_LINUX_RBTREE_H

#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ "compiler.h"

#ఒకవేళ __GNUC_PREREQ (4, 6)
#సూచన GCC diagnostic నెట్టు
#సూచన GCC diagnostic ignored "-Wunused-function"
#అయితే_ముగింపు

నిర్మాణం rb_కణుపు
{
	uintptr_t  rb_మాతృ_color;
#నిర్వచించు	RB_RED		0
#నిర్వచించు	RB_BLACK	1
	నిర్మాణం rb_కణుపు *rb_కుడి;
	నిర్మాణం rb_కణుపు *rb_ఎడమ;
} __attribute__((aligned(పరిమాణం(దీర్ఘ))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

నిర్మాణం rb_మూలం
{
	నిర్మాణం rb_కణుపు *rb_కణుపు;
};


#నిర్వచించు ext2fs_rb_మాతృ(r)   ((నిర్మాణం rb_కణుపు *)((r)->rb_మాతృ_color & ~3))
#నిర్వచించు ext2fs_rb_color(r)   ((r)->rb_మాతృ_color & 1)
#నిర్వచించు ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#నిర్వచించు ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#నిర్వచించు ext2fs_rb_సమితి_red(r)  చేయి { (r)->rb_మాతృ_color &= ~1; } అయితే (0)
#నిర్వచించు ext2fs_rb_సమితి_black(r)  చేయి { (r)->rb_మాతృ_color |= 1; } అయితే (0)

స్థిర అంతఃవరుస శూన్య ext2fs_rb_సమితి_మాతృ(నిర్మాణం rb_కణుపు *rb, నిర్మాణం rb_కణుపు *p)
{
	rb->rb_మాతృ_color = (rb->rb_మాతృ_color & 3) | (uintptr_t)p;
}
స్థిర అంతఃవరుస శూన్య ext2fs_rb_సమితి_color(నిర్మాణం rb_కణుపు *rb, పూర్ణాంకం color)
{
	rb->rb_మాతృ_color = (rb->rb_మాతృ_color & ~1) | color;
}

#నిర్వచించు RB_మూలం	(నిర్మాణం rb_మూలం) { ఖాళీ, }
#నిర్వచించు	ext2fs_rb_నమోదు(ptr, type, member) container_యొక్క(ptr, type, member)

స్థిర అంతఃవరుస పూర్ణాంకం ext2fs_rb_ఖాళీలు_మూలం(నిర్మాణం rb_మూలం *మూలం)
{
	తిరిగి మూలం->rb_కణుపు == ఖాళీ;
}

స్థిర అంతఃవరుస పూర్ణాంకం ext2fs_rb_ఖాళీలు_కణుపు(నిర్మాణం rb_కణుపు *కణుపు)
{
	తిరిగి ext2fs_rb_మాతృ(కణుపు) == కణుపు;
}

స్థిర అంతఃవరుస శూన్య ext2fs_rb_clear_కణుపు(నిర్మాణం rb_కణుపు *కణుపు)
{
	ext2fs_rb_సమితి_మాతృ(కణుపు, కణుపు);
}

బాహ్య శూన్య ext2fs_rb_చొప్పించు_color(నిర్మాణం rb_కణుపు *, నిర్మాణం rb_మూలం *);
బాహ్య శూన్య ext2fs_rb_erase(నిర్మాణం rb_కణుపు *, నిర్మాణం rb_మూలం *);

/* Find logical next and previous nodes in a tree */
బాహ్య నిర్మాణం rb_కణుపు *ext2fs_rb_తదుపరి(నిర్మాణం rb_కణుపు *);
బాహ్య నిర్మాణం rb_కణుపు *ext2fs_rb_prev(నిర్మాణం rb_కణుపు *);
బాహ్య నిర్మాణం rb_కణుపు *ext2fs_rb_మొదటి(స్థిరరాశి నిర్మాణం rb_మూలం *);
బాహ్య నిర్మాణం rb_కణుపు *ext2fs_rb_చివరి(స్థిరరాశి నిర్మాణం rb_మూలం *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
బాహ్య శూన్య ext2fs_rb_replace_కణుపు(నిర్మాణం rb_కణుపు *victim, నిర్మాణం rb_కణుపు *new_,
				 నిర్మాణం rb_మూలం *మూలం);

స్థిర అంతఃవరుస శూన్య ext2fs_rb_link_కణుపు(నిర్మాణం rb_కణుపు * కణుపు,
				     నిర్మాణం rb_కణుపు * మాతృ,
				     నిర్మాణం rb_కణుపు ** rb_link)
{
	కణుపు->rb_మాతృ_color = (uintptr_t)మాతృ;
	కణుపు->rb_ఎడమ = కణుపు->rb_కుడి = ఖాళీ;

	*rb_link = కణుపు;
}

#ఒకవేళ __GNUC_PREREQ (4, 6)
#సూచన GCC diagnostic తీయి
#అయితే_ముగింపు

#అయితే_ముగింపు	/* _LINUX_RBTREE_H */
