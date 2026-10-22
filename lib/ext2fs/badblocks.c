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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
સ્થિર errcode_t make_u32_list(પૂર્ણાંક size, પૂર્ણાંક અંક, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(કદ(સંરચના ext2_struct_u32_list), &bb);
	જો (retval)
		પાછા retval;
	memset(bb, 0, કદ(સંરચના ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->અંક = અંક;
	retval = ext2fs_get_array(bb->size, કદ(blk_t), &bb->list);
	જો (retval) {
		ext2fs_free_mem(&bb);
		પાછા retval;
	}
	જો (list)
		memcpy(bb->list, list, bb->size * કદ(blk_t));
	નહીંતર
		memset(bb->list, 0, bb->size * કદ(blk_t));
	*ret = bb;
	પાછા 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, પૂર્ણાંક size)
{
	પાછા make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, પૂર્ણાંક size)
{
	પાછા make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->અંક, src->list, dest);
	જો (retval)
		પાછા retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	પાછા 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	પાછા ext2fs_u32_copy((ext2_u32_list) src,
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
	પૂર્ણાંક		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	જો (bb->અંક >= bb->size) {
		old_size = bb->size * કદ(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * કદ(__u32),
					   &bb->list);
		જો (retval) {
			bb->size -= 100;
			પાછા retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->અંક-1;
	જો ((bb->અંક != 0) && (bb->list[i] == blk))
		પાછા 0;
	જો ((bb->અંક == 0) || (bb->list[i] < blk)) {
		bb->list[bb->અંક++] = blk;
		પાછા 0;
	}

	j = bb->અંક;
	માટે (i=0; i < bb->અંક; i++) {
		જો (bb->list[i] == blk)
			પાછા 0;
		જો (bb->list[i] > blk) {
			j = i;
			તોડો;
		}
	}
	માટે (i=bb->અંક; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->અંક++;
	પાછા 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	પાછા ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
પૂર્ણાંક ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	પૂર્ણાંક	નીચલું, ઉપલું, mid;

	જો (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		પાછા -1;

	જો (bb->અંક == 0)
		પાછા -1;

	નીચલું = 0;
	ઉપલું = bb->અંક-1;
	જો (blk == bb->list[નીચલું])
		પાછા નીચલું;
	જો (blk == bb->list[ઉપલું])
		પાછા ઉપલું;

	જ્યારે (નીચલું < ઉપલું) {
		mid = ((unsigned)નીચલું + (unsigned)ઉપલું)/2;
		જો (mid == નીચલું || mid == ઉપલું)
			તોડો;
		જો (blk == bb->list[mid])
			પાછા mid;
		જો (blk < bb->list[mid])
			ઉપલું = mid;
		નહીંતર
			નીચલું = mid;
	}
	પાછા -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
પૂર્ણાંક ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	જો (ext2fs_u32_list_find(bb, blk) < 0)
		પાછા 0;
	નહીંતર
		પાછા 1;
}

પૂર્ણાંક ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	પાછા ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
પૂર્ણાંક ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	પૂર્ણાંક	remloc, i;

	જો (bb->અંક == 0)
		પાછા -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	જો (remloc < 0)
		પાછા -1;

	માટે (i = remloc ; i < bb->અંક-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->અંક--;
	પાછા 0;
}

શૂન્ય ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(કદ(સંરચના ext2_struct_u32_iterate), &iter);
	જો (retval)
		પાછા retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	પાછા 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	પાછા ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


પૂર્ણાંક ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	જો (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		પાછા 0;

	bb = iter->bb;

	જો (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		પાછા 0;

	જો (iter->ptr < bb->અંક) {
		*blk = bb->list[iter->ptr++];
		પાછા 1;
	}
	*blk = 0;
	પાછા 0;
}

પૂર્ણાંક ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	પાછા ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


શૂન્ય ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	જો (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		પાછા;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

શૂન્ય ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


પૂર્ણાંક ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	જો (bb1->અંક != bb2->અંક)
		પાછા 0;

	જો (memcmp(bb1->list, bb2->list, bb1->અંક * કદ(blk_t)) != 0)
		પાછા 0;
	પાછા 1;
}

પૂર્ણાંક ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	પાછા ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

પૂર્ણાંક ext2fs_u32_list_ગણતરી(ext2_u32_list bb)
{
	પાછા bb->અંક;
}
