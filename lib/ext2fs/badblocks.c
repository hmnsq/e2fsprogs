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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
ସ୍ଥିର errcode_t make_u32_list(ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_struct_u32_list), &bb);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	memset(bb, 0, ଆକାର(ସଂରଚନା ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->ଅଙ୍କ = ଅଙ୍କ;
	retval = ext2fs_get_array(bb->size, ଆକାର(blk_t), &bb->list);
	ଯଦି (retval) {
		ext2fs_free_mem(&bb);
		ଫେରନ୍ତୁ retval;
	}
	ଯଦି (list)
		memcpy(bb->list, list, bb->size * ଆକାର(blk_t));
	ଅନ୍ୟଥା
		memset(bb->list, 0, bb->size * ଆକାର(blk_t));
	*ret = bb;
	ଫେରନ୍ତୁ 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	ଫେରନ୍ତୁ make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	ଫେରନ୍ତୁ make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->ଅଙ୍କ, src->list, dest);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	ଫେରନ୍ତୁ ext2fs_u32_copy((ext2_u32_list) src,
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
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ଯଦି (bb->ଅଙ୍କ >= bb->size) {
		old_size = bb->size * ଆକାର(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * ଆକାର(__u32),
					   &bb->list);
		ଯଦି (retval) {
			bb->size -= 100;
			ଫେରନ୍ତୁ retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->ଅଙ୍କ-1;
	ଯଦି ((bb->ଅଙ୍କ != 0) && (bb->list[i] == blk))
		ଫେରନ୍ତୁ 0;
	ଯଦି ((bb->ଅଙ୍କ == 0) || (bb->list[i] < blk)) {
		bb->list[bb->ଅଙ୍କ++] = blk;
		ଫେରନ୍ତୁ 0;
	}

	j = bb->ଅଙ୍କ;
	ପାଇଁ (i=0; i < bb->ଅଙ୍କ; i++) {
		ଯଦି (bb->list[i] == blk)
			ଫେରନ୍ତୁ 0;
		ଯଦି (bb->list[i] > blk) {
			j = i;
			ଭାଙ୍ଗ;
		}
	}
	ପାଇଁ (i=bb->ଅଙ୍କ; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->ଅଙ୍କ++;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	ଫେରନ୍ତୁ ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ତଳ, ଉପର, mid;

	ଯଦି (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ଫେରନ୍ତୁ -1;

	ଯଦି (bb->ଅଙ୍କ == 0)
		ଫେରନ୍ତୁ -1;

	ତଳ = 0;
	ଉପର = bb->ଅଙ୍କ-1;
	ଯଦି (blk == bb->list[ତଳ])
		ଫେରନ୍ତୁ ତଳ;
	ଯଦି (blk == bb->list[ଉପର])
		ଫେରନ୍ତୁ ଉପର;

	ଯେତେବେଳେ (ତଳ < ଉପର) {
		mid = ((unsigned)ତଳ + (unsigned)ଉପର)/2;
		ଯଦି (mid == ତଳ || mid == ଉପର)
			ଭାଙ୍ଗ;
		ଯଦି (blk == bb->list[mid])
			ଫେରନ୍ତୁ mid;
		ଯଦି (blk < bb->list[mid])
			ଉପର = mid;
		ଅନ୍ୟଥା
			ତଳ = mid;
	}
	ଫେରନ୍ତୁ -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ଯଦି (ext2fs_u32_list_find(bb, blk) < 0)
		ଫେରନ୍ତୁ 0;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	ଫେରନ୍ତୁ ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	remloc, i;

	ଯଦି (bb->ଅଙ୍କ == 0)
		ଫେରନ୍ତୁ -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	ଯଦି (remloc < 0)
		ଫେରନ୍ତୁ -1;

	ପାଇଁ (i = remloc ; i < bb->ଅଙ୍କ-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->ଅଙ୍କ--;
	ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_struct_u32_iterate), &iter);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	ଫେରନ୍ତୁ ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ଯଦି (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		ଫେରନ୍ତୁ 0;

	bb = iter->bb;

	ଯଦି (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ଫେରନ୍ତୁ 0;

	ଯଦି (iter->ptr < bb->ଅଙ୍କ) {
		*blk = bb->list[iter->ptr++];
		ଫେରନ୍ତୁ 1;
	}
	*blk = 0;
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	ଫେରନ୍ତୁ ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


ଶୂନ୍ୟ ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	ଯଦି (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		ଫେରନ୍ତୁ;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

ଶୂନ୍ୟ ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ଯଦି (bb1->ଅଙ୍କ != bb2->ଅଙ୍କ)
		ଫେରନ୍ତୁ 0;

	ଯଦି (memcmp(bb1->list, bb2->list, bb1->ଅଙ୍କ * ଆକାର(blk_t)) != 0)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	ଫେରନ୍ତୁ ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_u32_list_ଗଣନା(ext2_u32_list bb)
{
	ଫେରନ୍ତୁ bb->ଅଙ୍କ;
}
