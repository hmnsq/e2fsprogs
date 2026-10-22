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
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ຖ້າ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ຖ້າ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
static errcode_t make_u32_list(ຈຳນວນເຕັມ size, ຈຳນວນເຕັມ num, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_struct_u32_list), &bb);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;
	memset(bb, 0, parimaa_na(sa_nracanaa ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->num = num;
	retval = ext2fs_get_array(bb->size, parimaa_na(blk_t), &bb->list);
	ຖ້າ (retval) {
		ext2fs_free_mem(&bb);
		ສົ່ງຄືນ retval;
	}
	ຖ້າ (list)
		memcpy(bb->list, list, bb->size * parimaa_na(blk_t));
	ບໍ່ດັ່ງນັ້ນ
		memset(bb->list, 0, bb->size * parimaa_na(blk_t));
	*ret = bb;
	ສົ່ງຄືນ 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, ຈຳນວນເຕັມ size)
{
	ສົ່ງຄືນ make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, ຈຳນວນເຕັມ size)
{
	ສົ່ງຄືນ make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->num, src->list, dest);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	ສົ່ງຄືນ ext2fs_u32_copy((ext2_u32_list) src,
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
	ຈຳນວນເຕັມ		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ຖ້າ (bb->num >= bb->size) {
		old_size = bb->size * parimaa_na(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * parimaa_na(__u32),
					   &bb->list);
		ຖ້າ (retval) {
			bb->size -= 100;
			ສົ່ງຄືນ retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->num-1;
	ຖ້າ ((bb->num != 0) && (bb->list[i] == blk))
		ສົ່ງຄືນ 0;
	ຖ້າ ((bb->num == 0) || (bb->list[i] < blk)) {
		bb->list[bb->num++] = blk;
		ສົ່ງຄືນ 0;
	}

	j = bb->num;
	ສຳລັບ (i=0; i < bb->num; i++) {
		ຖ້າ (bb->list[i] == blk)
			ສົ່ງຄືນ 0;
		ຖ້າ (bb->list[i] > blk) {
			j = i;
			bha_nga;
		}
	}
	ສຳລັບ (i=bb->num; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->num++;
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	ສົ່ງຄືນ ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
ຈຳນວນເຕັມ ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	ຈຳນວນເຕັມ	low, high, mid;

	ຖ້າ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ສົ່ງຄືນ -1;

	ຖ້າ (bb->num == 0)
		ສົ່ງຄືນ -1;

	low = 0;
	high = bb->num-1;
	ຖ້າ (blk == bb->list[low])
		ສົ່ງຄືນ low;
	ຖ້າ (blk == bb->list[high])
		ສົ່ງຄືນ high;

	ໃນຂະນະທີ່ (low < high) {
		mid = ((unsigned)low + (unsigned)high)/2;
		ຖ້າ (mid == low || mid == high)
			bha_nga;
		ຖ້າ (blk == bb->list[mid])
			ສົ່ງຄືນ mid;
		ຖ້າ (blk < bb->list[mid])
			high = mid;
		ບໍ່ດັ່ງນັ້ນ
			low = mid;
	}
	ສົ່ງຄືນ -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
ຈຳນວນເຕັມ ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ຖ້າ (ext2fs_u32_list_find(bb, blk) < 0)
		ສົ່ງຄືນ 0;
	ບໍ່ດັ່ງນັ້ນ
		ສົ່ງຄືນ 1;
}

ຈຳນວນເຕັມ ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	ສົ່ງຄືນ ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
ຈຳນວນເຕັມ ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	ຈຳນວນເຕັມ	remloc, i;

	ຖ້າ (bb->num == 0)
		ສົ່ງຄືນ -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	ຖ້າ (remloc < 0)
		ສົ່ງຄືນ -1;

	ສຳລັບ (i = remloc ; i < bb->num-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->num--;
	ສົ່ງຄືນ 0;
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
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	ສົ່ງຄືນ ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


ຈຳນວນເຕັມ ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ຖ້າ (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		ສົ່ງຄືນ 0;

	bb = iter->bb;

	ຖ້າ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ສົ່ງຄືນ 0;

	ຖ້າ (iter->ptr < bb->num) {
		*blk = bb->list[iter->ptr++];
		ສົ່ງຄືນ 1;
	}
	*blk = 0;
	ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	ສົ່ງຄືນ ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


void ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	ຖ້າ (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		ສົ່ງຄືນ;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

void ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


ຈຳນວນເຕັມ ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ຖ້າ (bb1->num != bb2->num)
		ສົ່ງຄືນ 0;

	ຖ້າ (memcmp(bb1->list, bb2->list, bb1->num * parimaa_na(blk_t)) != 0)
		ສົ່ງຄືນ 0;
	ສົ່ງຄືນ 1;
}

ຈຳນວນເຕັມ ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	ສົ່ງຄືນ ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

ຈຳນວນເຕັມ ext2fs_u32_list_count(ext2_u32_list bb)
{
	ສົ່ງຄືນ bb->num;
}
