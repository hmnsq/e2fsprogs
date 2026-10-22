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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
ಸ್ಥಿರ errcode_t make_u32_list(ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ ಅಂಕೆ, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_struct_u32_list), &bb);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	memset(bb, 0, ಗಾತ್ರ(ರಚನೆ ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->ಗಾತ್ರ_2 = ಗಾತ್ರ_2 ? ಗಾತ್ರ_2 : 10;
	bb->ಅಂಕೆ = ಅಂಕೆ;
	retval = ext2fs_get_array(bb->ಗಾತ್ರ_2, ಗಾತ್ರ(blk_t), &bb->list);
	ಆದರೆ (retval) {
		ext2fs_free_mem(&bb);
		ಹಿಂತಿರುಗಿ retval;
	}
	ಆದರೆ (list)
		ಸ್ಮರಣೆ_ನಕಲು(bb->list, list, bb->ಗಾತ್ರ_2 * ಗಾತ್ರ(blk_t));
	ಇಲ್ಲದಿದ್ದರೆ
		memset(bb->list, 0, bb->ಗಾತ್ರ_2 * ಗಾತ್ರ(blk_t));
	*ret = bb;
	ಹಿಂತಿರುಗಿ 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	ಹಿಂತಿರುಗಿ make_u32_list(ಗಾತ್ರ_2, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	ಹಿಂತಿರುಗಿ make_u32_list(ಗಾತ್ರ_2, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->ಗಾತ್ರ_2, src->ಅಂಕೆ, src->list, dest);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	ಹಿಂತಿರುಗಿ ext2fs_u32_copy((ext2_u32_list) src,
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
errcode_t ext2fs_u32_list_ಕೂಡಿಸು(ext2_u32_list bb, __u32 blk)
{
	errcode_t	retval;
	ಪೂರ್ಣಾಂಕ		i, j;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	old_ಗಾತ್ರ;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ಆದರೆ (bb->ಅಂಕೆ >= bb->ಗಾತ್ರ_2) {
		old_ಗಾತ್ರ = bb->ಗಾತ್ರ_2 * ಗಾತ್ರ(__u32);
		bb->ಗಾತ್ರ_2 += 100;
		retval = ext2fs_resize_mem(old_ಗಾತ್ರ, bb->ಗಾತ್ರ_2 * ಗಾತ್ರ(__u32),
					   &bb->list);
		ಆದರೆ (retval) {
			bb->ಗಾತ್ರ_2 -= 100;
			ಹಿಂತಿರುಗಿ retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->ಅಂಕೆ-1;
	ಆದರೆ ((bb->ಅಂಕೆ != 0) && (bb->list[i] == blk))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((bb->ಅಂಕೆ == 0) || (bb->list[i] < blk)) {
		bb->list[bb->ಅಂಕೆ++] = blk;
		ಹಿಂತಿರುಗಿ 0;
	}

	j = bb->ಅಂಕೆ;
	ಗಾಗಿ (i=0; i < bb->ಅಂಕೆ; i++) {
		ಆದರೆ (bb->list[i] == blk)
			ಹಿಂತಿರುಗಿ 0;
		ಆದರೆ (bb->list[i] > blk) {
			j = i;
			ನಿಲ್ಲಿಸು;
		}
	}
	ಗಾಗಿ (i=bb->ಅಂಕೆ; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->ಅಂಕೆ++;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_badblocks_list_ಕೂಡಿಸು(ext2_badblocks_list bb, blk_t blk)
{
	ಹಿಂತಿರುಗಿ ext2fs_u32_list_ಕೂಡಿಸು((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
ಪೂರ್ಣಾಂಕ ext2fs_u32_list_ಕಂಡುಹಿಡಿ(ext2_u32_list bb, __u32 blk)
{
	ಪೂರ್ಣಾಂಕ	ಕೆಳಗಿನ, ಮೇಲಿನ, mid;

	ಆದರೆ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (bb->ಅಂಕೆ == 0)
		ಹಿಂತಿರುಗಿ -1;

	ಕೆಳಗಿನ = 0;
	ಮೇಲಿನ = bb->ಅಂಕೆ-1;
	ಆದರೆ (blk == bb->list[ಕೆಳಗಿನ])
		ಹಿಂತಿರುಗಿ ಕೆಳಗಿನ;
	ಆದರೆ (blk == bb->list[ಮೇಲಿನ])
		ಹಿಂತಿರುಗಿ ಮೇಲಿನ;

	ಆಗ (ಕೆಳಗಿನ < ಮೇಲಿನ) {
		mid = ((ಚಿಹ್ನೆರಹಿತ)ಕೆಳಗಿನ + (ಚಿಹ್ನೆರಹಿತ)ಮೇಲಿನ)/2;
		ಆದರೆ (mid == ಕೆಳಗಿನ || mid == ಮೇಲಿನ)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (blk == bb->list[mid])
			ಹಿಂತಿರುಗಿ mid;
		ಆದರೆ (blk < bb->list[mid])
			ಮೇಲಿನ = mid;
		ಇಲ್ಲದಿದ್ದರೆ
			ಕೆಳಗಿನ = mid;
	}
	ಹಿಂತಿರುಗಿ -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
ಪೂರ್ಣಾಂಕ ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ಆದರೆ (ext2fs_u32_list_ಕಂಡುಹಿಡಿ(bb, blk) < 0)
		ಹಿಂತಿರುಗಿ 0;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	ಹಿಂತಿರುಗಿ ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
ಪೂರ್ಣಾಂಕ ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	ಪೂರ್ಣಾಂಕ	remloc, i;

	ಆದರೆ (bb->ಅಂಕೆ == 0)
		ಹಿಂತಿರುಗಿ -1;

	remloc = ext2fs_u32_list_ಕಂಡುಹಿಡಿ(bb, blk);
	ಆದರೆ (remloc < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಗಾಗಿ (i = remloc ; i < bb->ಅಂಕೆ-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->ಅಂಕೆ--;
	ಹಿಂತಿರುಗಿ 0;
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

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_struct_u32_iterate), &iter);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	ಹಿಂತಿರುಗಿ ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


ಪೂರ್ಣಾಂಕ ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ಆದರೆ (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		ಹಿಂತಿರುಗಿ 0;

	bb = iter->bb;

	ಆದರೆ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (iter->ptr < bb->ಅಂಕೆ) {
		*blk = bb->list[iter->ptr++];
		ಹಿಂತಿರುಗಿ 1;
	}
	*blk = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	ಹಿಂತಿರುಗಿ ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


ಶೂನ್ಯ ext2fs_u32_list_iterate_ಕೊನೆ(ext2_u32_iterate iter)
{
	ಆದರೆ (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		ಹಿಂತಿರುಗಿ;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

ಶೂನ್ಯ ext2fs_badblocks_list_iterate_ಕೊನೆ(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_ಕೊನೆ((ext2_u32_iterate) iter);
}


ಪೂರ್ಣಾಂಕ ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ಆದರೆ (bb1->ಅಂಕೆ != bb2->ಅಂಕೆ)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (memcmp(bb1->list, bb2->list, bb1->ಅಂಕೆ * ಗಾತ್ರ(blk_t)) != 0)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	ಹಿಂತಿರುಗಿ ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

ಪೂರ್ಣಾಂಕ ext2fs_u32_list_ಎಣಿಕೆ(ext2_u32_list bb)
{
	ಹಿಂತಿರುಗಿ bb->ಅಂಕೆ;
}
