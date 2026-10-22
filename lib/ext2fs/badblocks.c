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
#ᨀᨘ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᨀᨘ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᨀᨘ HAVE_SYS_TYPES_H
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
	ᨀᨘ (retval)
		ᨀᨘᨒ retval;
	memset(bb, 0, parimaa_na(sa_nracanaa ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->num = num;
	retval = ext2fs_get_array(bb->size, parimaa_na(blk_t), &bb->list);
	ᨀᨘ (retval) {
		ext2fs_free_mem(&bb);
		ᨀᨘᨒ retval;
	}
	ᨀᨘ (list)
		memcpy(bb->list, list, bb->size * parimaa_na(blk_t));
	ᨀᨘᨁ
		memset(bb->list, 0, bb->size * parimaa_na(blk_t));
	*ret = bb;
	ᨀᨘᨒ 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, puur_naa_nka size)
{
	ᨀᨘᨒ make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, puur_naa_nka size)
{
	ᨀᨘᨒ make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->num, src->list, dest);
	ᨀᨘ (retval)
		ᨀᨘᨒ retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	ᨀᨘᨒ 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	ᨀᨘᨒ ext2fs_u32_copy((ext2_u32_list) src,
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

	ᨀᨘ (bb->num >= bb->size) {
		old_size = bb->size * parimaa_na(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * parimaa_na(__u32),
					   &bb->list);
		ᨀᨘ (retval) {
			bb->size -= 100;
			ᨀᨘᨒ retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->num-1;
	ᨀᨘ ((bb->num != 0) && (bb->list[i] == blk))
		ᨀᨘᨒ 0;
	ᨀᨘ ((bb->num == 0) || (bb->list[i] < blk)) {
		bb->list[bb->num++] = blk;
		ᨀᨘᨒ 0;
	}

	j = bb->num;
	for (i=0; i < bb->num; i++) {
		ᨀᨘ (bb->list[i] == blk)
			ᨀᨘᨒ 0;
		ᨀᨘ (bb->list[i] > blk) {
			j = i;
			bha_nga;
		}
	}
	for (i=bb->num; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->num++;
	ᨀᨘᨒ 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	ᨀᨘᨒ ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
puur_naa_nka ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	puur_naa_nka	low, high, mid;

	ᨀᨘ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ᨀᨘᨒ -1;

	ᨀᨘ (bb->num == 0)
		ᨀᨘᨒ -1;

	low = 0;
	high = bb->num-1;
	ᨀᨘ (blk == bb->list[low])
		ᨀᨘᨒ low;
	ᨀᨘ (blk == bb->list[high])
		ᨀᨘᨒ high;

	ᨀᨛ (low < high) {
		mid = ((unsigned)low + (unsigned)high)/2;
		ᨀᨘ (mid == low || mid == high)
			bha_nga;
		ᨀᨘ (blk == bb->list[mid])
			ᨀᨘᨒ mid;
		ᨀᨘ (blk < bb->list[mid])
			high = mid;
		ᨀᨘᨁ
			low = mid;
	}
	ᨀᨘᨒ -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
puur_naa_nka ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ᨀᨘ (ext2fs_u32_list_find(bb, blk) < 0)
		ᨀᨘᨒ 0;
	ᨀᨘᨁ
		ᨀᨘᨒ 1;
}

puur_naa_nka ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	ᨀᨘᨒ ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
puur_naa_nka ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	puur_naa_nka	remloc, i;

	ᨀᨘ (bb->num == 0)
		ᨀᨘᨒ -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	ᨀᨘ (remloc < 0)
		ᨀᨘᨒ -1;

	for (i = remloc ; i < bb->num-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->num--;
	ᨀᨘᨒ 0;
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
	ᨀᨘ (retval)
		ᨀᨘᨒ retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	ᨀᨘᨒ 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	ᨀᨘᨒ ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


puur_naa_nka ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ᨀᨘ (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		ᨀᨘᨒ 0;

	bb = iter->bb;

	ᨀᨘ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ᨀᨘᨒ 0;

	ᨀᨘ (iter->ptr < bb->num) {
		*blk = bb->list[iter->ptr++];
		ᨀᨘᨒ 1;
	}
	*blk = 0;
	ᨀᨘᨒ 0;
}

puur_naa_nka ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	ᨀᨘᨒ ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


void ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	ᨀᨘ (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		ᨀᨘᨒ;

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

	ᨀᨘ (bb1->num != bb2->num)
		ᨀᨘᨒ 0;

	ᨀᨘ (memcmp(bb1->list, bb2->list, bb1->num * parimaa_na(blk_t)) != 0)
		ᨀᨘᨒ 0;
	ᨀᨘᨒ 1;
}

puur_naa_nka ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	ᨀᨘᨒ ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

puur_naa_nka ext2fs_u32_list_count(ext2_u32_list bb)
{
	ᨀᨘᨒ bb->num;
}
