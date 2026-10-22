/*
 * badblocks.c --- routines to manipulate the bad block structure
 *
 * Copyright (C) 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᯀᯮᯝ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᯀᯮᯝ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᯀᯮᯝ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
static errcode_t make_u32_list(puur_naa_nka size, puur_naa_nka num, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_struct_u32_list), &bb);
	ᯀᯮᯝ (retval)
		ᯀᯮᯞ retval;
	memset(bb, 0, parimaa_na(sa_nracanaa ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->num = num;
	retval = ext2fs_get_array(bb->size, parimaa_na(blk_t), &bb->list);
	ᯀᯮᯝ (retval) {
		ext2fs_free_mem(&bb);
		ᯀᯮᯞ retval;
	}
	ᯀᯮᯝ (list)
		memcpy(bb->list, list, bb->size * parimaa_na(blk_t));
	ᯀᯮᯝᯂᯪ
		memset(bb->list, 0, bb->size * parimaa_na(blk_t));
	*ret = bb;
	ᯀᯮᯞ 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, puur_naa_nka size)
{
	ᯀᯮᯞ make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, puur_naa_nka size)
{
	ᯀᯮᯞ make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->num, src->list, dest);
	ᯀᯮᯝ (retval)
		ᯀᯮᯞ retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	ᯀᯮᯞ 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	ᯀᯮᯞ ext2fs_u32_copy((ext2_u32_list) src,
			       (ext2_u32_list *) dest);
}

/*
 * This procedure frees a badblocks list.
 *
 * (note: moved to closefs.c)
 */


/*
 * This procedure adds an item to a tracking list (e.g. badblocks or casefold).
 */
errcode_t ext2fs_u32_list_add(ext2_u32_list bb, __u32 blk)
{
	errcode_t	retval;
	puur_naa_nka		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ᯀᯮᯝ (bb->num >= bb->size) {
		old_size = bb->size * parimaa_na(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * parimaa_na(__u32),
					   &bb->list);
		ᯀᯮᯝ (retval) {
			bb->size -= 100;
			ᯀᯮᯞ retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->num-1;
	ᯀᯮᯝ ((bb->num != 0) && (bb->list[i] == blk))
		ᯀᯮᯞ 0;
	ᯀᯮᯝ ((bb->num == 0) || (bb->list[i] < blk)) {
		bb->list[bb->num++] = blk;
		ᯀᯮᯞ 0;
	}

	j = bb->num;
	for (i=0; i < bb->num; i++) {
		ᯀᯮᯝ (bb->list[i] == blk)
			ᯀᯮᯞ 0;
		ᯀᯮᯝ (bb->list[i] > blk) {
			j = i;
			bha_nga;
		}
	}
	for (i=bb->num; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->num++;
	ᯀᯮᯞ 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	ᯀᯮᯞ ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
puur_naa_nka ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	puur_naa_nka	low, high, mid;

	ᯀᯮᯝ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ᯀᯮᯞ -1;

	ᯀᯮᯝ (bb->num == 0)
		ᯀᯮᯞ -1;

	low = 0;
	high = bb->num-1;
	ᯀᯮᯝ (blk == bb->list[low])
		ᯀᯮᯞ low;
	ᯀᯮᯝ (blk == bb->list[high])
		ᯀᯮᯞ high;

	ᯀᯬᯝ (low < high) {
		mid = ((unsigned)low + (unsigned)high)/2;
		ᯀᯮᯝ (mid == low || mid == high)
			bha_nga;
		ᯀᯮᯝ (blk == bb->list[mid])
			ᯀᯮᯞ mid;
		ᯀᯮᯝ (blk < bb->list[mid])
			high = mid;
		ᯀᯮᯝᯂᯪ
			low = mid;
	}
	ᯀᯮᯞ -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
puur_naa_nka ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ᯀᯮᯝ (ext2fs_u32_list_find(bb, blk) < 0)
		ᯀᯮᯞ 0;
	ᯀᯮᯝᯂᯪ
		ᯀᯮᯞ 1;
}

puur_naa_nka ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	ᯀᯮᯞ ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
puur_naa_nka ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	puur_naa_nka	remloc, i;

	ᯀᯮᯝ (bb->num == 0)
		ᯀᯮᯞ -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	ᯀᯮᯝ (remloc < 0)
		ᯀᯮᯞ -1;

	for (i = remloc ; i < bb->num-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->num--;
	ᯀᯮᯞ 0;
}

void ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_struct_u32_iterate), &iter);
	ᯀᯮᯝ (retval)
		ᯀᯮᯞ retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	ᯀᯮᯞ 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	ᯀᯮᯞ ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


puur_naa_nka ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ᯀᯮᯝ (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		ᯀᯮᯞ 0;

	bb = iter->bb;

	ᯀᯮᯝ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ᯀᯮᯞ 0;

	ᯀᯮᯝ (iter->ptr < bb->num) {
		*blk = bb->list[iter->ptr++];
		ᯀᯮᯞ 1;
	}
	*blk = 0;
	ᯀᯮᯞ 0;
}

puur_naa_nka ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	ᯀᯮᯞ ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


void ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	ᯀᯮᯝ (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		ᯀᯮᯞ;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

void ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


puur_naa_nka ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ᯀᯮᯝ (bb1->num != bb2->num)
		ᯀᯮᯞ 0;

	ᯀᯮᯝ (memcmp(bb1->list, bb2->list, bb1->num * parimaa_na(blk_t)) != 0)
		ᯀᯮᯞ 0;
	ᯀᯮᯞ 1;
}

puur_naa_nka ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	ᯀᯮᯞ ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

puur_naa_nka ext2fs_u32_list_count(ext2_u32_list bb)
{
	ᯀᯮᯞ bb->num;
}
