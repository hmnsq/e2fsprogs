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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
స్థిర errcode_t make_u32_list(పూర్ణాంకం కొలత, పూర్ణాంకం అంకె, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_struct_u32_list), &bb);
	ఒకవేళ (retval)
		తిరిగి retval;
	memset(bb, 0, పరిమాణం(నిర్మాణం ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->కొలత = కొలత ? కొలత : 10;
	bb->అంకె = అంకె;
	retval = ext2fs_get_array(bb->కొలత, పరిమాణం(blk_t), &bb->list);
	ఒకవేళ (retval) {
		ext2fs_free_mem(&bb);
		తిరిగి retval;
	}
	ఒకవేళ (list)
		జ్ఞాపకం_నకలు(bb->list, list, bb->కొలత * పరిమాణం(blk_t));
	లేకపోతే
		memset(bb->list, 0, bb->కొలత * పరిమాణం(blk_t));
	*ret = bb;
	తిరిగి 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, పూర్ణాంకం కొలత)
{
	తిరిగి make_u32_list(కొలత, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, పూర్ణాంకం కొలత)
{
	తిరిగి make_u32_list(కొలత, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->కొలత, src->అంకె, src->list, dest);
	ఒకవేళ (retval)
		తిరిగి retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	తిరిగి 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	తిరిగి ext2fs_u32_copy((ext2_u32_list) src,
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
errcode_t ext2fs_u32_list_కూడు(ext2_u32_list bb, __u32 blk)
{
	errcode_t	retval;
	పూర్ణాంకం		i, j;
	గుర్తులేని దీర్ఘ	old_కొలత;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ఒకవేళ (bb->అంకె >= bb->కొలత) {
		old_కొలత = bb->కొలత * పరిమాణం(__u32);
		bb->కొలత += 100;
		retval = ext2fs_resize_mem(old_కొలత, bb->కొలత * పరిమాణం(__u32),
					   &bb->list);
		ఒకవేళ (retval) {
			bb->కొలత -= 100;
			తిరిగి retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->అంకె-1;
	ఒకవేళ ((bb->అంకె != 0) && (bb->list[i] == blk))
		తిరిగి 0;
	ఒకవేళ ((bb->అంకె == 0) || (bb->list[i] < blk)) {
		bb->list[bb->అంకె++] = blk;
		తిరిగి 0;
	}

	j = bb->అంకె;
	కొరకు (i=0; i < bb->అంకె; i++) {
		ఒకవేళ (bb->list[i] == blk)
			తిరిగి 0;
		ఒకవేళ (bb->list[i] > blk) {
			j = i;
			ఆపు;
		}
	}
	కొరకు (i=bb->అంకె; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->అంకె++;
	తిరిగి 0;
}

errcode_t ext2fs_badblocks_list_కూడు(ext2_badblocks_list bb, blk_t blk)
{
	తిరిగి ext2fs_u32_list_కూడు((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
పూర్ణాంకం ext2fs_u32_list_కనుగొను(ext2_u32_list bb, __u32 blk)
{
	పూర్ణాంకం	దిగువ, ఎగువ, mid;

	ఒకవేళ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		తిరిగి -1;

	ఒకవేళ (bb->అంకె == 0)
		తిరిగి -1;

	దిగువ = 0;
	ఎగువ = bb->అంకె-1;
	ఒకవేళ (blk == bb->list[దిగువ])
		తిరిగి దిగువ;
	ఒకవేళ (blk == bb->list[ఎగువ])
		తిరిగి ఎగువ;

	అయితే (దిగువ < ఎగువ) {
		mid = ((గుర్తులేని)దిగువ + (గుర్తులేని)ఎగువ)/2;
		ఒకవేళ (mid == దిగువ || mid == ఎగువ)
			ఆపు;
		ఒకవేళ (blk == bb->list[mid])
			తిరిగి mid;
		ఒకవేళ (blk < bb->list[mid])
			ఎగువ = mid;
		లేకపోతే
			దిగువ = mid;
	}
	తిరిగి -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
పూర్ణాంకం ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ఒకవేళ (ext2fs_u32_list_కనుగొను(bb, blk) < 0)
		తిరిగి 0;
	లేకపోతే
		తిరిగి 1;
}

పూర్ణాంకం ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	తిరిగి ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
పూర్ణాంకం ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	పూర్ణాంకం	remloc, i;

	ఒకవేళ (bb->అంకె == 0)
		తిరిగి -1;

	remloc = ext2fs_u32_list_కనుగొను(bb, blk);
	ఒకవేళ (remloc < 0)
		తిరిగి -1;

	కొరకు (i = remloc ; i < bb->అంకె-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->అంకె--;
	తిరిగి 0;
}

శూన్య ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_struct_u32_iterate), &iter);
	ఒకవేళ (retval)
		తిరిగి retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	తిరిగి 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	తిరిగి ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


పూర్ణాంకం ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ఒకవేళ (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		తిరిగి 0;

	bb = iter->bb;

	ఒకవేళ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		తిరిగి 0;

	ఒకవేళ (iter->ptr < bb->అంకె) {
		*blk = bb->list[iter->ptr++];
		తిరిగి 1;
	}
	*blk = 0;
	తిరిగి 0;
}

పూర్ణాంకం ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	తిరిగి ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


శూన్య ext2fs_u32_list_iterate_చివర(ext2_u32_iterate iter)
{
	ఒకవేళ (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		తిరిగి;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

శూన్య ext2fs_badblocks_list_iterate_చివర(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_చివర((ext2_u32_iterate) iter);
}


పూర్ణాంకం ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ఒకవేళ (bb1->అంకె != bb2->అంకె)
		తిరిగి 0;

	ఒకవేళ (memcmp(bb1->list, bb2->list, bb1->అంకె * పరిమాణం(blk_t)) != 0)
		తిరిగి 0;
	తిరిగి 1;
}

పూర్ణాంకం ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	తిరిగి ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

పూర్ణాంకం ext2fs_u32_list_లెక్క(ext2_u32_list bb)
{
	తిరిగి bb->అంకె;
}
