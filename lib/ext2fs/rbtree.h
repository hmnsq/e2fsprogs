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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ	_LINUX_RBTREE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ	_LINUX_RBTREE_H

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "compiler.h"

#ਜੇ __GNUC_PREREQ (4, 6)
#ਹਦਾਇਤ GCC diagnostic ਧੱਕੋ
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-function"
#ਜੇ_ਅੰਤ

ਬਣਤਰ rb_ਗੰਢ
{
	uintptr_t  rb_ਮਾਪਾ_color;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ	RB_RED		0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ	RB_BLACK	1
	ਬਣਤਰ rb_ਗੰਢ *rb_ਸੱਜਾ;
	ਬਣਤਰ rb_ਗੰਢ *rb_ਖੱਬਾ;
} __attribute__((aligned(ਆਕਾਰ(ਲੰਮਾ))));
    /* The alignment might seem pointless, but allegedly CRIS needs it */

ਬਣਤਰ rb_ਜੜ੍ਹ
{
	ਬਣਤਰ rb_ਗੰਢ *rb_ਗੰਢ;
};


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_rb_ਮਾਪਾ(r)   ((ਬਣਤਰ rb_ਗੰਢ *)((r)->rb_ਮਾਪਾ_color & ~3))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_rb_color(r)   ((r)->rb_ਮਾਪਾ_color & 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_rb_is_red(r)   (!ext2fs_rb_color(r))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_rb_is_black(r) ext2fs_rb_color(r)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_rb_ਸੈੱਟ_red(r)  ਕਰੋ { (r)->rb_ਮਾਪਾ_color &= ~1; } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_rb_ਸੈੱਟ_black(r)  ਕਰੋ { (r)->rb_ਮਾਪਾ_color |= 1; } ਜਦੋਂ (0)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(ਬਣਤਰ rb_ਗੰਢ *rb, ਬਣਤਰ rb_ਗੰਢ *p)
{
	rb->rb_ਮਾਪਾ_color = (rb->rb_ਮਾਪਾ_color & 3) | (uintptr_t)p;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_rb_ਸੈੱਟ_color(ਬਣਤਰ rb_ਗੰਢ *rb, ਪੂਰਨ color)
{
	rb->rb_ਮਾਪਾ_color = (rb->rb_ਮਾਪਾ_color & ~1) | color;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RB_ਜੜ੍ਹ	(ਬਣਤਰ rb_ਜੜ੍ਹ) { ਨਿਰਾ, }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ	ext2fs_rb_ਇੰਦਰਾਜ(ptr, type, member) container_ਦਾ(ptr, type, member)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ext2fs_rb_ਖਾਲੀ_ਥਾਵਾਂ_ਜੜ੍ਹ(ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਵਾਪਸ ਜੜ੍ਹ->rb_ਗੰਢ == ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ext2fs_rb_ਖਾਲੀ_ਥਾਵਾਂ_ਗੰਢ(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ)
{
	ਵਾਪਸ ext2fs_rb_ਮਾਪਾ(ਗੰਢ) == ਗੰਢ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_rb_clear_ਗੰਢ(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ)
{
	ext2fs_rb_ਸੈੱਟ_ਮਾਪਾ(ਗੰਢ, ਗੰਢ);
}

ਬਾਹਰੀ ਖਾਲੀ ext2fs_rb_ਪਾਓ_color(ਬਣਤਰ rb_ਗੰਢ *, ਬਣਤਰ rb_ਜੜ੍ਹ *);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_rb_erase(ਬਣਤਰ rb_ਗੰਢ *, ਬਣਤਰ rb_ਜੜ੍ਹ *);

/* Find logical next and previous nodes in a tree */
ਬਾਹਰੀ ਬਣਤਰ rb_ਗੰਢ *ext2fs_rb_ਅਗਲਾ(ਬਣਤਰ rb_ਗੰਢ *);
ਬਾਹਰੀ ਬਣਤਰ rb_ਗੰਢ *ext2fs_rb_prev(ਬਣਤਰ rb_ਗੰਢ *);
ਬਾਹਰੀ ਬਣਤਰ rb_ਗੰਢ *ext2fs_rb_ਪਹਿਲਾ(ਅਚਲ ਬਣਤਰ rb_ਜੜ੍ਹ *);
ਬਾਹਰੀ ਬਣਤਰ rb_ਗੰਢ *ext2fs_rb_ਆਖਰੀ(ਅਚਲ ਬਣਤਰ rb_ਜੜ੍ਹ *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
ਬਾਹਰੀ ਖਾਲੀ ext2fs_rb_replace_ਗੰਢ(ਬਣਤਰ rb_ਗੰਢ *victim, ਬਣਤਰ rb_ਗੰਢ *new_,
				 ਬਣਤਰ rb_ਜੜ੍ਹ *ਜੜ੍ਹ);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_rb_link_ਗੰਢ(ਬਣਤਰ rb_ਗੰਢ * ਗੰਢ,
				     ਬਣਤਰ rb_ਗੰਢ * ਮਾਪਾ,
				     ਬਣਤਰ rb_ਗੰਢ ** rb_link)
{
	ਗੰਢ->rb_ਮਾਪਾ_color = (uintptr_t)ਮਾਪਾ;
	ਗੰਢ->rb_ਖੱਬਾ = ਗੰਢ->rb_ਸੱਜਾ = ਨਿਰਾ;

	*rb_link = ਗੰਢ;
}

#ਜੇ __GNUC_PREREQ (4, 6)
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ	/* _LINUX_RBTREE_H */
