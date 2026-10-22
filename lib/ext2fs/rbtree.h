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

#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ "compiler.h"

#എങ്കിൽ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic തള്ളുക
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

ഘടന rb_കണ്ണി
{
	uintptr_t  rb_parent_color;
#define	RB_RED		0
#define	RB_BLACK	1
	ഘടന rb_കണ്ണി *rb_വലത്;
	ഘടന rb_കണ്ണി *rb_ഇടത്;
} __attribute__((aligned(വലുപ്പം(long))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

ഘടന rb_വേര്
{
	ഘടന rb_കണ്ണി *rb_കണ്ണി;
};


#define ext2fs_rb_parent(r)   ((ഘടന rb_കണ്ണി *)((r)->rb_parent_color & ~3))
#define ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#define ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#define ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#define ext2fs_rb_set_red(r)  ചെയ്യുക { (r)->rb_parent_color &= ~1; } ആകുമ്പോൾ (0)
#define ext2fs_rb_set_black(r)  ചെയ്യുക { (r)->rb_parent_color |= 1; } ആകുമ്പോൾ (0)

സ്ഥിതം വരിയിൽ ശൂന്യം ext2fs_rb_set_parent(ഘടന rb_കണ്ണി *rb, ഘടന rb_കണ്ണി *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
സ്ഥിതം വരിയിൽ ശൂന്യം ext2fs_rb_set_color(ഘടന rb_കണ്ണി *rb, പൂർണ്ണസംഖ്യ color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_വേര്	(ഘടന rb_വേര്) { ശൂന്യ, }
#define	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ ext2fs_rb_empty_വേര്(ഘടന rb_വേര് *വേര്)
{
	തിരികെ വേര്->rb_കണ്ണി == ശൂന്യ;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ ext2fs_rb_empty_കണ്ണി(ഘടന rb_കണ്ണി *കണ്ണി)
{
	തിരികെ ext2fs_rb_parent(കണ്ണി) == കണ്ണി;
}

സ്ഥിതം വരിയിൽ ശൂന്യം ext2fs_rb_clear_കണ്ണി(ഘടന rb_കണ്ണി *കണ്ണി)
{
	ext2fs_rb_set_parent(കണ്ണി, കണ്ണി);
}

ബാഹ്യ ശൂന്യം ext2fs_rb_തിരുകുക_color(ഘടന rb_കണ്ണി *, ഘടന rb_വേര് *);
ബാഹ്യ ശൂന്യം ext2fs_rb_erase(ഘടന rb_കണ്ണി *, ഘടന rb_വേര് *);

/* Find logical next and previous nodes in a tree */
ബാഹ്യ ഘടന rb_കണ്ണി *ext2fs_rb_next(ഘടന rb_കണ്ണി *);
ബാഹ്യ ഘടന rb_കണ്ണി *ext2fs_rb_prev(ഘടന rb_കണ്ണി *);
ബാഹ്യ ഘടന rb_കണ്ണി *ext2fs_rb_first(സ്ഥിരം ഘടന rb_വേര് *);
ബാഹ്യ ഘടന rb_കണ്ണി *ext2fs_rb_last(സ്ഥിരം ഘടന rb_വേര് *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
ബാഹ്യ ശൂന്യം ext2fs_rb_replace_കണ്ണി(ഘടന rb_കണ്ണി *victim, ഘടന rb_കണ്ണി *new_,
				 ഘടന rb_വേര് *വേര്);

സ്ഥിതം വരിയിൽ ശൂന്യം ext2fs_rb_link_കണ്ണി(ഘടന rb_കണ്ണി * കണ്ണി,
				     ഘടന rb_കണ്ണി * parent,
				     ഘടന rb_കണ്ണി ** rb_link)
{
	കണ്ണി->rb_parent_color = (uintptr_t)parent;
	കണ്ണി->rb_ഇടത് = കണ്ണി->rb_വലത് = ശൂന്യ;

	*rb_link = കണ്ണി;
}

#എങ്കിൽ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic എടുക്കുക
#endif

#endif	/* _LINUX_RBTREE_H */
