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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
ಸ್ಥಿರ errcode_t make_u32_list(ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_struct_u32_list), &bb);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	memset(bb, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->ಅಂಕೆ = ಅಂಕೆ;
	retval = ext2fs_get_array(bb->size, ಪರಿಮಾಣ(blk_t), &bb->list);
	ಯದಿ (retval) {
		ext2fs_free_mem(&bb);
		ಪ್ರತ್ಯಾಗಮ retval;
	}
	ಯದಿ (list)
		memcpy(bb->list, list, bb->size * ಪರಿಮಾಣ(blk_t));
	ಅನ್ಯಥಾ
		memset(bb->list, 0, bb->size * ಪರಿಮಾಣ(blk_t));
	*ret = bb;
	ಪ್ರತ್ಯಾಗಮ 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, ಪೂರ್ಣಾಙ್ಕ size)
{
	ಪ್ರತ್ಯಾಗಮ make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, ಪೂರ್ಣಾಙ್ಕ size)
{
	ಪ್ರತ್ಯಾಗಮ make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->ಅಂಕೆ, src->list, dest);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_u32_copy((ext2_u32_list) src,
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
	ಪೂರ್ಣಾಙ್ಕ		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ಯದಿ (bb->ಅಂಕೆ >= bb->size) {
		old_size = bb->size * ಪರಿಮಾಣ(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * ಪರಿಮಾಣ(__u32),
					   &bb->list);
		ಯದಿ (retval) {
			bb->size -= 100;
			ಪ್ರತ್ಯಾಗಮ retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->ಅಂಕೆ-1;
	ಯದಿ ((bb->ಅಂಕೆ != 0) && (bb->list[i] == blk))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ ((bb->ಅಂಕೆ == 0) || (bb->list[i] < blk)) {
		bb->list[bb->ಅಂಕೆ++] = blk;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	j = bb->ಅಂಕೆ;
	ಪ್ರತಿ (i=0; i < bb->ಅಂಕೆ; i++) {
		ಯದಿ (bb->list[i] == blk)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಯದಿ (bb->list[i] > blk) {
			j = i;
			ಭಙ್ಗ;
		}
	}
	ಪ್ರತಿ (i=bb->ಅಂಕೆ; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->ಅಂಕೆ++;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	ಪೂರ್ಣಾಙ್ಕ	ಕೆಳಗಿನ, ಮೇಲಿನ, mid;

	ಯದಿ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (bb->ಅಂಕೆ == 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಕೆಳಗಿನ = 0;
	ಮೇಲಿನ = bb->ಅಂಕೆ-1;
	ಯದಿ (blk == bb->list[ಕೆಳಗಿನ])
		ಪ್ರತ್ಯಾಗಮ ಕೆಳಗಿನ;
	ಯದಿ (blk == bb->list[ಮೇಲಿನ])
		ಪ್ರತ್ಯಾಗಮ ಮೇಲಿನ;

	ಯಾವತ್ (ಕೆಳಗಿನ < ಮೇಲಿನ) {
		mid = ((unsigned)ಕೆಳಗಿನ + (unsigned)ಮೇಲಿನ)/2;
		ಯದಿ (mid == ಕೆಳಗಿನ || mid == ಮೇಲಿನ)
			ಭಙ್ಗ;
		ಯದಿ (blk == bb->list[mid])
			ಪ್ರತ್ಯಾಗಮ mid;
		ಯದಿ (blk < bb->list[mid])
			ಮೇಲಿನ = mid;
		ಅನ್ಯಥಾ
			ಕೆಳಗಿನ = mid;
	}
	ಪ್ರತ್ಯಾಗಮ -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ಯದಿ (ext2fs_u32_list_find(bb, blk) < 0)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ 1;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	ಪೂರ್ಣಾಙ್ಕ	remloc, i;

	ಯದಿ (bb->ಅಂಕೆ == 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	ಯದಿ (remloc < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತಿ (i = remloc ; i < bb->ಅಂಕೆ-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->ಅಂಕೆ--;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_struct_u32_iterate), &iter);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ಯದಿ (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		ಪ್ರತ್ಯಾಗಮ 0;

	bb = iter->bb;

	ಯದಿ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (iter->ptr < bb->ಅಂಕೆ) {
		*blk = bb->list[iter->ptr++];
		ಪ್ರತ್ಯಾಗಮ 1;
	}
	*blk = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


ಶೂನ್ಯ ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	ಯದಿ (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		ಪ್ರತ್ಯಾಗಮ;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

ಶೂನ್ಯ ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ಯದಿ (bb1->ಅಂಕೆ != bb2->ಅಂಕೆ)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (memcmp(bb1->list, bb2->list, bb1->ಅಂಕೆ * ಪರಿಮಾಣ(blk_t)) != 0)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_u32_list_ಎಣಿಕೆ(ext2_u32_list bb)
{
	ಪ್ರತ್ಯಾಗಮ bb->ಅಂಕೆ;
}
