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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ	_LINUX_RBTREE_H
#ವ್ಯಾಖ್ಯಾನಿಸು	_LINUX_RBTREE_H

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "compiler.h"

#ಆದರೆ __GNUC_PREREQ (4, 6)
#ಸೂಚನೆ GCC diagnostic ತಳ್ಳು
#ಸೂಚನೆ GCC diagnostic ignored "-Wunused-function"
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ rb_ಗಂಟು
{
	uintptr_t  rb_ಪೋಷಕ_color;
#ವ್ಯಾಖ್ಯಾನಿಸು	RB_RED		0
#ವ್ಯಾಖ್ಯಾನಿಸು	RB_BLACK	1
	ರಚನೆ rb_ಗಂಟು *rb_ಬಲ;
	ರಚನೆ rb_ಗಂಟು *rb_ಎಡ;
} __attribute__((aligned(ಗಾತ್ರ(ದೀರ್ಘ))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

ರಚನೆ rb_ಬೇರು
{
	ರಚನೆ rb_ಗಂಟು *rb_ಗಂಟು;
};


#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_rb_ಪೋಷಕ(r)   ((ರಚನೆ rb_ಗಂಟು *)((r)->rb_ಪೋಷಕ_color & ~3))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_rb_color(r)   ((r)->rb_ಪೋಷಕ_color & 1)
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_rb_ಗಣ_red(r)  ಮಾಡು { (r)->rb_ಪೋಷಕ_color &= ~1; } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_rb_ಗಣ_black(r)  ಮಾಡು { (r)->rb_ಪೋಷಕ_color |= 1; } ಆಗ (0)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_rb_ಗಣ_ಪೋಷಕ(ರಚನೆ rb_ಗಂಟು *rb, ರಚನೆ rb_ಗಂಟು *p)
{
	rb->rb_ಪೋಷಕ_color = (rb->rb_ಪೋಷಕ_color & 3) | (uintptr_t)p;
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_rb_ಗಣ_color(ರಚನೆ rb_ಗಂಟು *rb, ಪೂರ್ಣಾಂಕ color)
{
	rb->rb_ಪೋಷಕ_color = (rb->rb_ಪೋಷಕ_color & ~1) | color;
}

#ವ್ಯಾಖ್ಯಾನಿಸು RB_ಬೇರು	(ರಚನೆ rb_ಬೇರು) { ಖಾಲಿ, }
#ವ್ಯಾಖ್ಯಾನಿಸು	ext2fs_rb_ನಮೂದು(ptr, type, member) container_ದ(ptr, type, member)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ext2fs_rb_ಖಾಲಿಗಳು_ಬೇರು(ರಚನೆ rb_ಬೇರು *ಬೇರು)
{
	ಹಿಂತಿರುಗಿ ಬೇರು->rb_ಗಂಟು == ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ext2fs_rb_ಖಾಲಿಗಳು_ಗಂಟು(ರಚನೆ rb_ಗಂಟು *ಗಂಟು)
{
	ಹಿಂತಿರುಗಿ ext2fs_rb_ಪೋಷಕ(ಗಂಟು) == ಗಂಟು;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_rb_clear_ಗಂಟು(ರಚನೆ rb_ಗಂಟು *ಗಂಟು)
{
	ext2fs_rb_ಗಣ_ಪೋಷಕ(ಗಂಟು, ಗಂಟು);
}

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_rb_ಸೇರಿಸು_color(ರಚನೆ rb_ಗಂಟು *, ರಚನೆ rb_ಬೇರು *);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_rb_erase(ರಚನೆ rb_ಗಂಟು *, ರಚನೆ rb_ಬೇರು *);

/* Find logical next and previous nodes in a tree */
ಬಾಹ್ಯ ರಚನೆ rb_ಗಂಟು *ext2fs_rb_ಮುಂದಿನ(ರಚನೆ rb_ಗಂಟು *);
ಬಾಹ್ಯ ರಚನೆ rb_ಗಂಟು *ext2fs_rb_prev(ರಚನೆ rb_ಗಂಟು *);
ಬಾಹ್ಯ ರಚನೆ rb_ಗಂಟು *ext2fs_rb_ಮೊದಲ(ಸ್ಥಿರಾಂಕ ರಚನೆ rb_ಬೇರು *);
ಬಾಹ್ಯ ರಚನೆ rb_ಗಂಟು *ext2fs_rb_ಕೊನೆಯ(ಸ್ಥಿರಾಂಕ ರಚನೆ rb_ಬೇರು *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_rb_replace_ಗಂಟು(ರಚನೆ rb_ಗಂಟು *victim, ರಚನೆ rb_ಗಂಟು *new_,
				 ರಚನೆ rb_ಬೇರು *ಬೇರು);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_rb_link_ಗಂಟು(ರಚನೆ rb_ಗಂಟು * ಗಂಟು,
				     ರಚನೆ rb_ಗಂಟು * ಪೋಷಕ,
				     ರಚನೆ rb_ಗಂಟು ** rb_link)
{
	ಗಂಟು->rb_ಪೋಷಕ_color = (uintptr_t)ಪೋಷಕ;
	ಗಂಟು->rb_ಎಡ = ಗಂಟು->rb_ಬಲ = ಖಾಲಿ;

	*rb_link = ಗಂಟು;
}

#ಆದರೆ __GNUC_PREREQ (4, 6)
#ಸೂಚನೆ GCC diagnostic ತೆಗೆ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ	/* _LINUX_RBTREE_H */
