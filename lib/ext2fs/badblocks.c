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
#အကယ်၍ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#အကယ်၍ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#အကယ်၍ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
static errcode_t make_u32_list(ကိန်းပြည့် size, ကိန်းပြည့် num, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_struct_u32_list), &bb);
	အကယ်၍ (retval)
		ပြန်ပေး retval;
	memset(bb, 0, parimaa_na(sa_nracanaa ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->num = num;
	retval = ext2fs_get_array(bb->size, parimaa_na(blk_t), &bb->list);
	အကယ်၍ (retval) {
		ext2fs_free_mem(&bb);
		ပြန်ပေး retval;
	}
	အကယ်၍ (list)
		memcpy(bb->list, list, bb->size * parimaa_na(blk_t));
	မဟုတ်ပါက
		memset(bb->list, 0, bb->size * parimaa_na(blk_t));
	*ret = bb;
	ပြန်ပေး 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, ကိန်းပြည့် size)
{
	ပြန်ပေး make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, ကိန်းပြည့် size)
{
	ပြန်ပေး make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->num, src->list, dest);
	အကယ်၍ (retval)
		ပြန်ပေး retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	ပြန်ပေး 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	ပြန်ပေး ext2fs_u32_copy((ext2_u32_list) src,
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
	ကိန်းပြည့်		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	အကယ်၍ (bb->num >= bb->size) {
		old_size = bb->size * parimaa_na(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * parimaa_na(__u32),
					   &bb->list);
		အကယ်၍ (retval) {
			bb->size -= 100;
			ပြန်ပေး retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->num-1;
	အကယ်၍ ((bb->num != 0) && (bb->list[i] == blk))
		ပြန်ပေး 0;
	အကယ်၍ ((bb->num == 0) || (bb->list[i] < blk)) {
		bb->list[bb->num++] = blk;
		ပြန်ပေး 0;
	}

	j = bb->num;
	အတွက် (i=0; i < bb->num; i++) {
		အကယ်၍ (bb->list[i] == blk)
			ပြန်ပေး 0;
		အကယ်၍ (bb->list[i] > blk) {
			j = i;
			bha_nga;
		}
	}
	အတွက် (i=bb->num; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->num++;
	ပြန်ပေး 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	ပြန်ပေး ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
ကိန်းပြည့် ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	ကိန်းပြည့်	low, high, mid;

	အကယ်၍ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ပြန်ပေး -1;

	အကယ်၍ (bb->num == 0)
		ပြန်ပေး -1;

	low = 0;
	high = bb->num-1;
	အကယ်၍ (blk == bb->list[low])
		ပြန်ပေး low;
	အကယ်၍ (blk == bb->list[high])
		ပြန်ပေး high;

	နေစဉ် (low < high) {
		mid = ((unsigned)low + (unsigned)high)/2;
		အကယ်၍ (mid == low || mid == high)
			bha_nga;
		အကယ်၍ (blk == bb->list[mid])
			ပြန်ပေး mid;
		အကယ်၍ (blk < bb->list[mid])
			high = mid;
		မဟုတ်ပါက
			low = mid;
	}
	ပြန်ပေး -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
ကိန်းပြည့် ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	အကယ်၍ (ext2fs_u32_list_find(bb, blk) < 0)
		ပြန်ပေး 0;
	မဟုတ်ပါက
		ပြန်ပေး 1;
}

ကိန်းပြည့် ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	ပြန်ပေး ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
ကိန်းပြည့် ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	ကိန်းပြည့်	remloc, i;

	အကယ်၍ (bb->num == 0)
		ပြန်ပေး -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	အကယ်၍ (remloc < 0)
		ပြန်ပေး -1;

	အတွက် (i = remloc ; i < bb->num-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->num--;
	ပြန်ပေး 0;
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
	အကယ်၍ (retval)
		ပြန်ပေး retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	ပြန်ပေး 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	ပြန်ပေး ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


ကိန်းပြည့် ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	အကယ်၍ (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		ပြန်ပေး 0;

	bb = iter->bb;

	အကယ်၍ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ပြန်ပေး 0;

	အကယ်၍ (iter->ptr < bb->num) {
		*blk = bb->list[iter->ptr++];
		ပြန်ပေး 1;
	}
	*blk = 0;
	ပြန်ပေး 0;
}

ကိန်းပြည့် ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	ပြန်ပေး ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


void ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	အကယ်၍ (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		ပြန်ပေး;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

void ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


ကိန်းပြည့် ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	အကယ်၍ (bb1->num != bb2->num)
		ပြန်ပေး 0;

	အကယ်၍ (memcmp(bb1->list, bb2->list, bb1->num * parimaa_na(blk_t)) != 0)
		ပြန်ပေး 0;
	ပြန်ပေး 1;
}

ကိန်းပြည့် ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	ပြန်ပေး ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

ကိန်းပြည့် ext2fs_u32_list_count(ext2_u32_list bb)
{
	ပြန်ပေး bb->num;
}
