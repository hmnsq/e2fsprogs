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

#यदि_अपरिभाषितम्	_LINUX_RBTREE_H
#परिभाषय	_LINUX_RBTREE_H

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव "compiler.h"

#यदि __GNUC_PREREQ (4, 6)
#निर्देशः GCC diagnostic प्रक्षिप
#निर्देशः GCC diagnostic ignored "-Wunused-function"
#यदि_अन्तः

संरचना rb_ग्रन्थिः
{
	uintptr_t  rb_parent_color;
#परिभाषय	RB_RED		0
#परिभाषय	RB_BLACK	1
	संरचना rb_ग्रन्थिः *rb_दक्षिणः;
	संरचना rb_ग्रन्थिः *rb_वामः;
} __attribute__((aligned(परिमाण(दीर्घ))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

संरचना rb_मूलम्
{
	संरचना rb_ग्रन्थिः *rb_ग्रन्थिः;
};


#परिभाषय ext2fs_rb_parent(r)   ((संरचना rb_ग्रन्थिः *)((r)->rb_parent_color & ~3))
#परिभाषय ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#परिभाषय ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#परिभाषय ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#परिभाषय ext2fs_rb_समुच्चयः_red(r)  कुरु { (r)->rb_parent_color &= ~1; } यावत् (0)
#परिभाषय ext2fs_rb_समुच्चयः_black(r)  कुरु { (r)->rb_parent_color |= 1; } यावत् (0)

स्थिर अन्तःस्थ शून्य ext2fs_rb_समुच्चयः_parent(संरचना rb_ग्रन्थिः *rb, संरचना rb_ग्रन्थिः *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
स्थिर अन्तःस्थ शून्य ext2fs_rb_समुच्चयः_color(संरचना rb_ग्रन्थिः *rb, पूर्णाङ्क color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#परिभाषय RB_मूलम्	(संरचना rb_मूलम्) { रिक्त, }
#परिभाषय	ext2fs_rb_प्रविष्टिः(ptr, type, member) container_सम्बन्धि(ptr, type, member)

स्थिर अन्तःस्थ पूर्णाङ्क ext2fs_rb_रिक्तानि_मूलम्(संरचना rb_मूलम् *मूलम्)
{
	प्रत्यागम मूलम्->rb_ग्रन्थिः == रिक्त;
}

स्थिर अन्तःस्थ पूर्णाङ्क ext2fs_rb_रिक्तानि_ग्रन्थिः(संरचना rb_ग्रन्थिः *ग्रन्थिः)
{
	प्रत्यागम ext2fs_rb_parent(ग्रन्थिः) == ग्रन्थिः;
}

स्थिर अन्तःस्थ शून्य ext2fs_rb_clear_ग्रन्थिः(संरचना rb_ग्रन्थिः *ग्रन्थिः)
{
	ext2fs_rb_समुच्चयः_parent(ग्रन्थिः, ग्रन्थिः);
}

बाह्य शून्य ext2fs_rb_निवेशय_color(संरचना rb_ग्रन्थिः *, संरचना rb_मूलम् *);
बाह्य शून्य ext2fs_rb_erase(संरचना rb_ग्रन्थिः *, संरचना rb_मूलम् *);

/* Find logical next and previous nodes in a tree */
बाह्य संरचना rb_ग्रन्थिः *ext2fs_rb_अग्र(संरचना rb_ग्रन्थिः *);
बाह्य संरचना rb_ग्रन्थिः *ext2fs_rb_पूर्व(संरचना rb_ग्रन्थिः *);
बाह्य संरचना rb_ग्रन्थिः *ext2fs_rb_first(ध्रुव संरचना rb_मूलम् *);
बाह्य संरचना rb_ग्रन्थिः *ext2fs_rb_चरमः(ध्रुव संरचना rb_मूलम् *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
बाह्य शून्य ext2fs_rb_replace_ग्रन्थिः(संरचना rb_ग्रन्थिः *victim, संरचना rb_ग्रन्थिः *new_,
				 संरचना rb_मूलम् *मूलम्);

स्थिर अन्तःस्थ शून्य ext2fs_rb_link_ग्रन्थिः(संरचना rb_ग्रन्थिः * ग्रन्थिः,
				     संरचना rb_ग्रन्थिः * parent,
				     संरचना rb_ग्रन्थिः ** rb_link)
{
	ग्रन्थिः->rb_parent_color = (uintptr_t)parent;
	ग्रन्थिः->rb_वामः = ग्रन्थिः->rb_दक्षिणः = रिक्त;

	*rb_link = ग्रन्थिः;
}

#यदि __GNUC_PREREQ (4, 6)
#निर्देशः GCC diagnostic निष्कासय
#यदि_अन्तः

#यदि_अन्तः	/* _LINUX_RBTREE_H */
