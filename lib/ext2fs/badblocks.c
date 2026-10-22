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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
ساکن errcode_t make_u32_list(صحیح سائز, صحیح عدد, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم(ساخت ext2_struct_u32_list), &bb);
	اگر (retval)
		واپس retval;
	memset(bb, 0, حجم(ساخت ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->سائز = سائز ? سائز : 10;
	bb->عدد = عدد;
	retval = ext2fs_get_array(bb->سائز, حجم(blk_t), &bb->list);
	اگر (retval) {
		ext2fs_free_mem(&bb);
		واپس retval;
	}
	اگر (list)
		حافظہ_نقل(bb->list, list, bb->سائز * حجم(blk_t));
	ورنہ
		memset(bb->list, 0, bb->سائز * حجم(blk_t));
	*ret = bb;
	واپس 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, صحیح سائز)
{
	واپس make_u32_list(سائز, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, صحیح سائز)
{
	واپس make_u32_list(سائز, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->سائز, src->عدد, src->list, dest);
	اگر (retval)
		واپس retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	واپس 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	واپس ext2fs_u32_copy((ext2_u32_list) src,
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
	صحیح		i, j;
	غیرعلامتی طویل	old_سائز;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	اگر (bb->عدد >= bb->سائز) {
		old_سائز = bb->سائز * حجم(__u32);
		bb->سائز += 100;
		retval = ext2fs_resize_mem(old_سائز, bb->سائز * حجم(__u32),
					   &bb->list);
		اگر (retval) {
			bb->سائز -= 100;
			واپس retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->عدد-1;
	اگر ((bb->عدد != 0) && (bb->list[i] == blk))
		واپس 0;
	اگر ((bb->عدد == 0) || (bb->list[i] < blk)) {
		bb->list[bb->عدد++] = blk;
		واپس 0;
	}

	j = bb->عدد;
	برائے (i=0; i < bb->عدد; i++) {
		اگر (bb->list[i] == blk)
			واپس 0;
		اگر (bb->list[i] > blk) {
			j = i;
			توڑو;
		}
	}
	برائے (i=bb->عدد; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->عدد++;
	واپس 0;
}

errcode_t ext2fs_badblocks_list_جوڑو(ext2_badblocks_list bb, blk_t blk)
{
	واپس ext2fs_u32_list_جوڑو((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
صحیح ext2fs_u32_list_ڈھونڈو(ext2_u32_list bb, __u32 blk)
{
	صحیح	کم, بلند, mid;

	اگر (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		واپس -1;

	اگر (bb->عدد == 0)
		واپس -1;

	کم = 0;
	بلند = bb->عدد-1;
	اگر (blk == bb->list[کم])
		واپس کم;
	اگر (blk == bb->list[بلند])
		واپس بلند;

	جب (کم < بلند) {
		mid = ((غیرعلامتی)کم + (غیرعلامتی)بلند)/2;
		اگر (mid == کم || mid == بلند)
			توڑو;
		اگر (blk == bb->list[mid])
			واپس mid;
		اگر (blk < bb->list[mid])
			بلند = mid;
		ورنہ
			کم = mid;
	}
	واپس -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
صحیح ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	اگر (ext2fs_u32_list_ڈھونڈو(bb, blk) < 0)
		واپس 0;
	ورنہ
		واپس 1;
}

صحیح ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	واپس ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
صحیح ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	صحیح	remloc, i;

	اگر (bb->عدد == 0)
		واپس -1;

	remloc = ext2fs_u32_list_ڈھونڈو(bb, blk);
	اگر (remloc < 0)
		واپس -1;

	برائے (i = remloc ; i < bb->عدد-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->عدد--;
	واپس 0;
}

خالی ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(حجم(ساخت ext2_struct_u32_iterate), &iter);
	اگر (retval)
		واپس retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	واپس 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	واپس ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


صحیح ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	اگر (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		واپس 0;

	bb = iter->bb;

	اگر (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		واپس 0;

	اگر (iter->ptr < bb->عدد) {
		*blk = bb->list[iter->ptr++];
		واپس 1;
	}
	*blk = 0;
	واپس 0;
}

صحیح ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	واپس ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


خالی ext2fs_u32_list_iterate_انتہا(ext2_u32_iterate iter)
{
	اگر (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		واپس;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

خالی ext2fs_badblocks_list_iterate_انتہا(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_انتہا((ext2_u32_iterate) iter);
}


صحیح ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	اگر (bb1->عدد != bb2->عدد)
		واپس 0;

	اگر (memcmp(bb1->list, bb2->list, bb1->عدد * حجم(blk_t)) != 0)
		واپس 0;
	واپس 1;
}

صحیح ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	واپس ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

صحیح ext2fs_u32_list_گنتی(ext2_u32_list bb)
{
	واپس bb->عدد;
}
