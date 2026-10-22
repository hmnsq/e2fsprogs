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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
স্থির errcode_t make_u32_list(পূর্ণাঙ্ক size, পূর্ণাঙ্ক অঙ্ক, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা ext2_struct_u32_list), &bb);
	যদি (retval)
		প্রত্যাগম retval;
	memset(bb, 0, পরিমাণ(সংরচনা ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->অঙ্ক = অঙ্ক;
	retval = ext2fs_get_array(bb->size, পরিমাণ(blk_t), &bb->list);
	যদি (retval) {
		ext2fs_free_mem(&bb);
		প্রত্যাগম retval;
	}
	যদি (list)
		memcpy(bb->list, list, bb->size * পরিমাণ(blk_t));
	অন্যথা
		memset(bb->list, 0, bb->size * পরিমাণ(blk_t));
	*ret = bb;
	প্রত্যাগম 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, পূর্ণাঙ্ক size)
{
	প্রত্যাগম make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, পূর্ণাঙ্ক size)
{
	প্রত্যাগম make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->অঙ্ক, src->list, dest);
	যদি (retval)
		প্রত্যাগম retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	প্রত্যাগম 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	প্রত্যাগম ext2fs_u32_copy((ext2_u32_list) src,
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
	পূর্ণাঙ্ক		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	যদি (bb->অঙ্ক >= bb->size) {
		old_size = bb->size * পরিমাণ(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * পরিমাণ(__u32),
					   &bb->list);
		যদি (retval) {
			bb->size -= 100;
			প্রত্যাগম retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->অঙ্ক-1;
	যদি ((bb->অঙ্ক != 0) && (bb->list[i] == blk))
		প্রত্যাগম 0;
	যদি ((bb->অঙ্ক == 0) || (bb->list[i] < blk)) {
		bb->list[bb->অঙ্ক++] = blk;
		প্রত্যাগম 0;
	}

	j = bb->অঙ্ক;
	প্রতি (i=0; i < bb->অঙ্ক; i++) {
		যদি (bb->list[i] == blk)
			প্রত্যাগম 0;
		যদি (bb->list[i] > blk) {
			j = i;
			ভঙ্গ;
		}
	}
	প্রতি (i=bb->অঙ্ক; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->অঙ্ক++;
	প্রত্যাগম 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	প্রত্যাগম ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
পূর্ণাঙ্ক ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	পূর্ণাঙ্ক	নিচের, উপরের, mid;

	যদি (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		প্রত্যাগম -1;

	যদি (bb->অঙ্ক == 0)
		প্রত্যাগম -1;

	নিচের = 0;
	উপরের = bb->অঙ্ক-1;
	যদি (blk == bb->list[নিচের])
		প্রত্যাগম নিচের;
	যদি (blk == bb->list[উপরের])
		প্রত্যাগম উপরের;

	যাবত্ (নিচের < উপরের) {
		mid = ((unsigned)নিচের + (unsigned)উপরের)/2;
		যদি (mid == নিচের || mid == উপরের)
			ভঙ্গ;
		যদি (blk == bb->list[mid])
			প্রত্যাগম mid;
		যদি (blk < bb->list[mid])
			উপরের = mid;
		অন্যথা
			নিচের = mid;
	}
	প্রত্যাগম -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
পূর্ণাঙ্ক ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	যদি (ext2fs_u32_list_find(bb, blk) < 0)
		প্রত্যাগম 0;
	অন্যথা
		প্রত্যাগম 1;
}

পূর্ণাঙ্ক ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	প্রত্যাগম ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
পূর্ণাঙ্ক ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	পূর্ণাঙ্ক	remloc, i;

	যদি (bb->অঙ্ক == 0)
		প্রত্যাগম -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	যদি (remloc < 0)
		প্রত্যাগম -1;

	প্রতি (i = remloc ; i < bb->অঙ্ক-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->অঙ্ক--;
	প্রত্যাগম 0;
}

শূন্য ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা ext2_struct_u32_iterate), &iter);
	যদি (retval)
		প্রত্যাগম retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	প্রত্যাগম 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	প্রত্যাগম ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


পূর্ণাঙ্ক ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	যদি (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		প্রত্যাগম 0;

	bb = iter->bb;

	যদি (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		প্রত্যাগম 0;

	যদি (iter->ptr < bb->অঙ্ক) {
		*blk = bb->list[iter->ptr++];
		প্রত্যাগম 1;
	}
	*blk = 0;
	প্রত্যাগম 0;
}

পূর্ণাঙ্ক ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	প্রত্যাগম ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


শূন্য ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	যদি (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		প্রত্যাগম;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

শূন্য ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


পূর্ণাঙ্ক ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	যদি (bb1->অঙ্ক != bb2->অঙ্ক)
		প্রত্যাগম 0;

	যদি (memcmp(bb1->list, bb2->list, bb1->অঙ্ক * পরিমাণ(blk_t)) != 0)
		প্রত্যাগম 0;
	প্রত্যাগম 1;
}

পূর্ণাঙ্ক ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	প্রত্যাগম ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

পূর্ণাঙ্ক ext2fs_u32_list_গুনতি(ext2_u32_list bb)
{
	প্রত্যাগম bb->অঙ্ক;
}
