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
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
ساكن errcode_t make_u32_list(صحيح size, صحيح ژمارە, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_u32_list), &bb);
	إذا (retval)
		أرجع retval;
	memset(bb, 0, حجم(بنية ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->ژمارە = ژمارە;
	retval = ext2fs_get_array(bb->size, حجم(blk_t), &bb->list);
	إذا (retval) {
		ext2fs_free_mem(&bb);
		أرجع retval;
	}
	إذا (list)
		memcpy(bb->list, list, bb->size * حجم(blk_t));
	وإلا
		memset(bb->list, 0, bb->size * حجم(blk_t));
	*ret = bb;
	أرجع 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, صحيح size)
{
	أرجع make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, صحيح size)
{
	أرجع make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->ژمارە, src->list, dest);
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
errcode_t ext2fs_u32_list_add(ext2_u32_list bb, __u32 blk)
{
	errcode_t	retval;
	صحيح		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	إذا (bb->ژمارە >= bb->size) {
		old_size = bb->size * حجم(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * حجم(__u32),
					   &bb->list);
		إذا (retval) {
			bb->size -= 100;
			أرجع retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->ژمارە-1;
	إذا ((bb->ژمارە != 0) && (bb->list[i] == blk))
		أرجع 0;
	إذا ((bb->ژمارە == 0) || (bb->list[i] < blk)) {
		bb->list[bb->ژمارە++] = blk;
		أرجع 0;
	}

	j = bb->ژمارە;
	لكل (i=0; i < bb->ژمارە; i++) {
		إذا (bb->list[i] == blk)
			أرجع 0;
		إذا (bb->list[i] > blk) {
			j = i;
			اكسر;
		}
	}
	لكل (i=bb->ژمارە; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->ژمارە++;
	أرجع 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	أرجع ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
صحيح ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	صحيح	نزم, بەرز, mid;

	إذا (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		أرجع -1;

	إذا (bb->ژمارە == 0)
		أرجع -1;

	نزم = 0;
	بەرز = bb->ژمارە-1;
	إذا (blk == bb->list[نزم])
		أرجع نزم;
	إذا (blk == bb->list[بەرز])
		أرجع بەرز;

	طالما (نزم < بەرز) {
		mid = ((unsigned)نزم + (unsigned)بەرز)/2;
		إذا (mid == نزم || mid == بەرز)
			اكسر;
		إذا (blk == bb->list[mid])
			أرجع mid;
		إذا (blk < bb->list[mid])
			بەرز = mid;
		وإلا
			نزم = mid;
	}
	أرجع -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
صحيح ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	إذا (ext2fs_u32_list_find(bb, blk) < 0)
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

	إذا (bb->ژمارە == 0)
		أرجع -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	إذا (remloc < 0)
		أرجع -1;

	لكل (i = remloc ; i < bb->ژمارە-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->ژمارە--;
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

	إذا (iter->ptr < bb->ژمارە) {
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


فراغ ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	إذا (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		أرجع;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

فراغ ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


صحيح ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	إذا (bb1->ژمارە != bb2->ژمارە)
		أرجع 0;

	إذا (memcmp(bb1->list, bb2->list, bb1->ژمارە * حجم(blk_t)) != 0)
		أرجع 0;
	أرجع 1;
}

صحيح ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	أرجع ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

صحيح ext2fs_u32_list_ژماردن(ext2_u32_list bb)
{
	أرجع bb->ژمارە;
}
