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

#अगर_अघोषित	_LINUX_RBTREE_H
#घोषणा	_LINUX_RBTREE_H

#समावेश <stdlib.h>
#समावेश <stdint.h>
#समावेश "compiler.h"

#अगर __GNUC_PREREQ (4, 6)
#आशय GCC diagnostic धकेलो
#आशय GCC diagnostic ignored "-Wunused-function"
#पूर्ण_अगर

काष्ठा rb_गाँठ
{
	uintptr_t  rb_parent_color;
#घोषणा	RB_RED		0
#घोषणा	RB_BLACK	1
	काष्ठा rb_गाँठ *rb_दायाँ;
	काष्ठा rb_गाँठ *rb_बायाँ;
} __attribute__((aligned(माप(दीर्घ))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

काष्ठा rb_मूल
{
	काष्ठा rb_गाँठ *rb_गाँठ;
};


#घोषणा ext2fs_rb_parent(r)   ((काष्ठा rb_गाँठ *)((r)->rb_parent_color & ~3))
#घोषणा ext2fs_rb_color(r)   ((r)->rb_parent_color & 1)
#घोषणा ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#घोषणा ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#घोषणा ext2fs_rb_set_red(r)  करो { (r)->rb_parent_color &= ~1; } जबतक (0)
#घोषणा ext2fs_rb_set_black(r)  करो { (r)->rb_parent_color |= 1; } जबतक (0)

अटल अंतरभूत व्योम ext2fs_rb_set_parent(काष्ठा rb_गाँठ *rb, काष्ठा rb_गाँठ *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uintptr_t)p;
}
अटल अंतरभूत व्योम ext2fs_rb_set_color(काष्ठा rb_गाँठ *rb, पूर्णांक color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#घोषणा RB_मूल	(काष्ठा rb_मूल) { शून्य, }
#घोषणा	ext2fs_rb_entry(ptr, type, member) container_of(ptr, type, member)

अटल अंतरभूत पूर्णांक ext2fs_rb_empty_मूल(काष्ठा rb_मूल *मूल)
{
	वापस मूल->rb_गाँठ == शून्य;
}

अटल अंतरभूत पूर्णांक ext2fs_rb_empty_गाँठ(काष्ठा rb_गाँठ *गाँठ)
{
	वापस ext2fs_rb_parent(गाँठ) == गाँठ;
}

अटल अंतरभूत व्योम ext2fs_rb_clear_गाँठ(काष्ठा rb_गाँठ *गाँठ)
{
	ext2fs_rb_set_parent(गाँठ, गाँठ);
}

बाह्य व्योम ext2fs_rb_जोड़ो_color(काष्ठा rb_गाँठ *, काष्ठा rb_मूल *);
बाह्य व्योम ext2fs_rb_erase(काष्ठा rb_गाँठ *, काष्ठा rb_मूल *);

/* Find logical next and previous nodes in a tree */
बाह्य काष्ठा rb_गाँठ *ext2fs_rb_अग्र(काष्ठा rb_गाँठ *);
बाह्य काष्ठा rb_गाँठ *ext2fs_rb_पूर्व(काष्ठा rb_गाँठ *);
बाह्य काष्ठा rb_गाँठ *ext2fs_rb_first(स्थिर काष्ठा rb_मूल *);
बाह्य काष्ठा rb_गाँठ *ext2fs_rb_last(स्थिर काष्ठा rb_मूल *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
बाह्य व्योम ext2fs_rb_replace_गाँठ(काष्ठा rb_गाँठ *victim, काष्ठा rb_गाँठ *new_,
				 काष्ठा rb_मूल *मूल);

अटल अंतरभूत व्योम ext2fs_rb_link_गाँठ(काष्ठा rb_गाँठ * गाँठ,
				     काष्ठा rb_गाँठ * parent,
				     काष्ठा rb_गाँठ ** rb_link)
{
	गाँठ->rb_parent_color = (uintptr_t)parent;
	गाँठ->rb_बायाँ = गाँठ->rb_दायाँ = शून्य;

	*rb_link = गाँठ;
}

#अगर __GNUC_PREREQ (4, 6)
#आशय GCC diagnostic निकालो
#पूर्ण_अगर

#पूर्ण_अगर	/* _LINUX_RBTREE_H */
