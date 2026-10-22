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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
ਸਥਿਰ errcode_t make_u32_list(ਪੂਰਨ ਮਾਪ, ਪੂਰਨ ਅੰਕ, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_struct_u32_list), &bb);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	memset(bb, 0, ਆਕਾਰ(ਬਣਤਰ ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->ਮਾਪ = ਮਾਪ ? ਮਾਪ : 10;
	bb->ਅੰਕ = ਅੰਕ;
	retval = ext2fs_get_array(bb->ਮਾਪ, ਆਕਾਰ(blk_t), &bb->list);
	ਜੇ (retval) {
		ext2fs_free_mem(&bb);
		ਵਾਪਸ retval;
	}
	ਜੇ (list)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(bb->list, list, bb->ਮਾਪ * ਆਕਾਰ(blk_t));
	ਨਹੀਂਤਾਂ
		memset(bb->list, 0, bb->ਮਾਪ * ਆਕਾਰ(blk_t));
	*ret = bb;
	ਵਾਪਸ 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, ਪੂਰਨ ਮਾਪ)
{
	ਵਾਪਸ make_u32_list(ਮਾਪ, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, ਪੂਰਨ ਮਾਪ)
{
	ਵਾਪਸ make_u32_list(ਮਾਪ, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->ਮਾਪ, src->ਅੰਕ, src->list, dest);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	ਵਾਪਸ ext2fs_u32_copy((ext2_u32_list) src,
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
errcode_t ext2fs_u32_list_ਜੋੜੋ(ext2_u32_list bb, __u32 blk)
{
	errcode_t	retval;
	ਪੂਰਨ		i, j;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	old_ਮਾਪ;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ਜੇ (bb->ਅੰਕ >= bb->ਮਾਪ) {
		old_ਮਾਪ = bb->ਮਾਪ * ਆਕਾਰ(__u32);
		bb->ਮਾਪ += 100;
		retval = ext2fs_resize_mem(old_ਮਾਪ, bb->ਮਾਪ * ਆਕਾਰ(__u32),
					   &bb->list);
		ਜੇ (retval) {
			bb->ਮਾਪ -= 100;
			ਵਾਪਸ retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->ਅੰਕ-1;
	ਜੇ ((bb->ਅੰਕ != 0) && (bb->list[i] == blk))
		ਵਾਪਸ 0;
	ਜੇ ((bb->ਅੰਕ == 0) || (bb->list[i] < blk)) {
		bb->list[bb->ਅੰਕ++] = blk;
		ਵਾਪਸ 0;
	}

	j = bb->ਅੰਕ;
	ਲਈ (i=0; i < bb->ਅੰਕ; i++) {
		ਜੇ (bb->list[i] == blk)
			ਵਾਪਸ 0;
		ਜੇ (bb->list[i] > blk) {
			j = i;
			ਰੋਕੋ;
		}
	}
	ਲਈ (i=bb->ਅੰਕ; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->ਅੰਕ++;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_badblocks_list_ਜੋੜੋ(ext2_badblocks_list bb, blk_t blk)
{
	ਵਾਪਸ ext2fs_u32_list_ਜੋੜੋ((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
ਪੂਰਨ ext2fs_u32_list_ਲੱਭੋ(ext2_u32_list bb, __u32 blk)
{
	ਪੂਰਨ	ਨੀਵਾਂ, ਉੱਚਾ, mid;

	ਜੇ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ਵਾਪਸ -1;

	ਜੇ (bb->ਅੰਕ == 0)
		ਵਾਪਸ -1;

	ਨੀਵਾਂ = 0;
	ਉੱਚਾ = bb->ਅੰਕ-1;
	ਜੇ (blk == bb->list[ਨੀਵਾਂ])
		ਵਾਪਸ ਨੀਵਾਂ;
	ਜੇ (blk == bb->list[ਉੱਚਾ])
		ਵਾਪਸ ਉੱਚਾ;

	ਜਦੋਂ (ਨੀਵਾਂ < ਉੱਚਾ) {
		mid = ((ਬਿਨਾਨਿਸ਼ਾਨੀ)ਨੀਵਾਂ + (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਉੱਚਾ)/2;
		ਜੇ (mid == ਨੀਵਾਂ || mid == ਉੱਚਾ)
			ਰੋਕੋ;
		ਜੇ (blk == bb->list[mid])
			ਵਾਪਸ mid;
		ਜੇ (blk < bb->list[mid])
			ਉੱਚਾ = mid;
		ਨਹੀਂਤਾਂ
			ਨੀਵਾਂ = mid;
	}
	ਵਾਪਸ -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
ਪੂਰਨ ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ਜੇ (ext2fs_u32_list_ਲੱਭੋ(bb, blk) < 0)
		ਵਾਪਸ 0;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ 1;
}

ਪੂਰਨ ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	ਵਾਪਸ ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
ਪੂਰਨ ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	ਪੂਰਨ	remloc, i;

	ਜੇ (bb->ਅੰਕ == 0)
		ਵਾਪਸ -1;

	remloc = ext2fs_u32_list_ਲੱਭੋ(bb, blk);
	ਜੇ (remloc < 0)
		ਵਾਪਸ -1;

	ਲਈ (i = remloc ; i < bb->ਅੰਕ-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->ਅੰਕ--;
	ਵਾਪਸ 0;
}

ਖਾਲੀ ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_struct_u32_iterate), &iter);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	ਵਾਪਸ ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


ਪੂਰਨ ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ਜੇ (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		ਵਾਪਸ 0;

	bb = iter->bb;

	ਜੇ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ਵਾਪਸ 0;

	ਜੇ (iter->ptr < bb->ਅੰਕ) {
		*blk = bb->list[iter->ptr++];
		ਵਾਪਸ 1;
	}
	*blk = 0;
	ਵਾਪਸ 0;
}

ਪੂਰਨ ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	ਵਾਪਸ ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


ਖਾਲੀ ext2fs_u32_list_iterate_ਸਿਰਾ(ext2_u32_iterate iter)
{
	ਜੇ (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		ਵਾਪਸ;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

ਖਾਲੀ ext2fs_badblocks_list_iterate_ਸਿਰਾ(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_ਸਿਰਾ((ext2_u32_iterate) iter);
}


ਪੂਰਨ ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ਜੇ (bb1->ਅੰਕ != bb2->ਅੰਕ)
		ਵਾਪਸ 0;

	ਜੇ (memcmp(bb1->list, bb2->list, bb1->ਅੰਕ * ਆਕਾਰ(blk_t)) != 0)
		ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}

ਪੂਰਨ ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	ਵਾਪਸ ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

ਪੂਰਨ ext2fs_u32_list_ਗਿਣਤੀ(ext2_u32_list bb)
{
	ਵਾਪਸ bb->ਅੰਕ;
}
