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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
ساكن errcode_t make_u32_list(صحيح ناپ, صحيح عدد, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_u32_list), &bb);
	إذا (retval)
		أرجع retval;
	memset(bb, 0, حجم(بنية ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->ناپ = ناپ ? ناپ : 10;
	bb->عدد = عدد;
	retval = ext2fs_get_array(bb->ناپ, حجم(blk_t), &bb->list);
	إذا (retval) {
		ext2fs_free_mem(&bb);
		أرجع retval;
	}
	إذا (list)
		حافظہ_نقل(bb->list, list, bb->ناپ * حجم(blk_t));
	وإلا
		memset(bb->list, 0, bb->ناپ * حجم(blk_t));
	*ret = bb;
	أرجع 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, صحيح ناپ)
{
	أرجع make_u32_list(ناپ, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, صحيح ناپ)
{
	أرجع make_u32_list(ناپ, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->ناپ, src->عدد, src->list, dest);
	إذا (retval)
		أرجع retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	أرجع 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	أرجع ext2fs_u32_copy((ext2_u32_list) src,
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
errcode_t ext2fs_u32_list_جوڑو(ext2_u32_list bb, __u32 blk)
{
	errcode_t	retval;
	صحيح		i, j;
	بنانشانی لما	old_ناپ;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	إذا (bb->عدد >= bb->ناپ) {
		old_ناپ = bb->ناپ * حجم(__u32);
		bb->ناپ += 100;
		retval = ext2fs_resize_mem(old_ناپ, bb->ناپ * حجم(__u32),
					   &bb->list);
		إذا (retval) {
			bb->ناپ -= 100;
			أرجع retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->عدد-1;
	إذا ((bb->عدد != 0) && (bb->list[i] == blk))
		أرجع 0;
	إذا ((bb->عدد == 0) || (bb->list[i] < blk)) {
		bb->list[bb->عدد++] = blk;
		أرجع 0;
	}

	j = bb->عدد;
	لكل (i=0; i < bb->عدد; i++) {
		إذا (bb->list[i] == blk)
			أرجع 0;
		إذا (bb->list[i] > blk) {
			j = i;
			اكسر;
		}
	}
	لكل (i=bb->عدد; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->عدد++;
	أرجع 0;
}

errcode_t ext2fs_badblocks_list_جوڑو(ext2_badblocks_list bb, blk_t blk)
{
	أرجع ext2fs_u32_list_جوڑو((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
صحيح ext2fs_u32_list_لبھو(ext2_u32_list bb, __u32 blk)
{
	صحيح	کم, بلند, mid;

	إذا (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		أرجع -1;

	إذا (bb->عدد == 0)
		أرجع -1;

	کم = 0;
	بلند = bb->عدد-1;
	إذا (blk == bb->list[کم])
		أرجع کم;
	إذا (blk == bb->list[بلند])
		أرجع بلند;

	طالما (کم < بلند) {
		mid = ((بنانشانی)کم + (بنانشانی)بلند)/2;
		إذا (mid == کم || mid == بلند)
			اكسر;
		إذا (blk == bb->list[mid])
			أرجع mid;
		إذا (blk < bb->list[mid])
			بلند = mid;
		وإلا
			کم = mid;
	}
	أرجع -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
صحيح ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	إذا (ext2fs_u32_list_لبھو(bb, blk) < 0)
		أرجع 0;
	وإلا
		أرجع 1;
}

صحيح ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	أرجع ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
صحيح ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	صحيح	remloc, i;

	إذا (bb->عدد == 0)
		أرجع -1;

	remloc = ext2fs_u32_list_لبھو(bb, blk);
	إذا (remloc < 0)
		أرجع -1;

	لكل (i = remloc ; i < bb->عدد-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->عدد--;
	أرجع 0;
}

فراغ ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_u32_iterate), &iter);
	إذا (retval)
		أرجع retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	أرجع 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	أرجع ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


صحيح ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	إذا (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		أرجع 0;

	bb = iter->bb;

	إذا (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		أرجع 0;

	إذا (iter->ptr < bb->عدد) {
		*blk = bb->list[iter->ptr++];
		أرجع 1;
	}
	*blk = 0;
	أرجع 0;
}

صحيح ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	أرجع ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


فراغ ext2fs_u32_list_iterate_سرا(ext2_u32_iterate iter)
{
	إذا (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		أرجع;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

فراغ ext2fs_badblocks_list_iterate_سرا(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_سرا((ext2_u32_iterate) iter);
}


صحيح ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	إذا (bb1->عدد != bb2->عدد)
		أرجع 0;

	إذا (memcmp(bb1->list, bb2->list, bb1->عدد * حجم(blk_t)) != 0)
		أرجع 0;
	أرجع 1;
}

صحيح ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	أرجع ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

صحيح ext2fs_u32_list_گنتی(ext2_u32_list bb)
{
	أرجع bb->عدد;
}
