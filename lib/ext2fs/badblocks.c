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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
സ്ഥിതം errcode_t make_u32_list(പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ അക്കം, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_struct_u32_list), &bb);
	എങ്കിൽ (retval)
		തിരികെ retval;
	memset(bb, 0, വലുപ്പം(ഘടന ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->അക്കം = അക്കം;
	retval = ext2fs_get_array(bb->size, വലുപ്പം(blk_t), &bb->list);
	എങ്കിൽ (retval) {
		ext2fs_free_mem(&bb);
		തിരികെ retval;
	}
	എങ്കിൽ (list)
		memcpy(bb->list, list, bb->size * വലുപ്പം(blk_t));
	അല്ലെങ്കിൽ
		memset(bb->list, 0, bb->size * വലുപ്പം(blk_t));
	*ret = bb;
	തിരികെ 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, പൂർണ്ണസംഖ്യ size)
{
	തിരികെ make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, പൂർണ്ണസംഖ്യ size)
{
	തിരികെ make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->അക്കം, src->list, dest);
	എങ്കിൽ (retval)
		തിരികെ retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	തിരികെ 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	തിരികെ ext2fs_u32_copy((ext2_u32_list) src,
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
	പൂർണ്ണസംഖ്യ		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	എങ്കിൽ (bb->അക്കം >= bb->size) {
		old_size = bb->size * വലുപ്പം(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * വലുപ്പം(__u32),
					   &bb->list);
		എങ്കിൽ (retval) {
			bb->size -= 100;
			തിരികെ retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->അക്കം-1;
	എങ്കിൽ ((bb->അക്കം != 0) && (bb->list[i] == blk))
		തിരികെ 0;
	എങ്കിൽ ((bb->അക്കം == 0) || (bb->list[i] < blk)) {
		bb->list[bb->അക്കം++] = blk;
		തിരികെ 0;
	}

	j = bb->അക്കം;
	വേണ്ടി (i=0; i < bb->അക്കം; i++) {
		എങ്കിൽ (bb->list[i] == blk)
			തിരികെ 0;
		എങ്കിൽ (bb->list[i] > blk) {
			j = i;
			നിർത്തുക;
		}
	}
	വേണ്ടി (i=bb->അക്കം; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->അക്കം++;
	തിരികെ 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	തിരികെ ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
പൂർണ്ണസംഖ്യ ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	പൂർണ്ണസംഖ്യ	താഴെയുള്ള, മുകളിലെ, mid;

	എങ്കിൽ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		തിരികെ -1;

	എങ്കിൽ (bb->അക്കം == 0)
		തിരികെ -1;

	താഴെയുള്ള = 0;
	മുകളിലെ = bb->അക്കം-1;
	എങ്കിൽ (blk == bb->list[താഴെയുള്ള])
		തിരികെ താഴെയുള്ള;
	എങ്കിൽ (blk == bb->list[മുകളിലെ])
		തിരികെ മുകളിലെ;

	ആകുമ്പോൾ (താഴെയുള്ള < മുകളിലെ) {
		mid = ((unsigned)താഴെയുള്ള + (unsigned)മുകളിലെ)/2;
		എങ്കിൽ (mid == താഴെയുള്ള || mid == മുകളിലെ)
			നിർത്തുക;
		എങ്കിൽ (blk == bb->list[mid])
			തിരികെ mid;
		എങ്കിൽ (blk < bb->list[mid])
			മുകളിലെ = mid;
		അല്ലെങ്കിൽ
			താഴെയുള്ള = mid;
	}
	തിരികെ -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
പൂർണ്ണസംഖ്യ ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	എങ്കിൽ (ext2fs_u32_list_find(bb, blk) < 0)
		തിരികെ 0;
	അല്ലെങ്കിൽ
		തിരികെ 1;
}

പൂർണ്ണസംഖ്യ ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	തിരികെ ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
പൂർണ്ണസംഖ്യ ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	പൂർണ്ണസംഖ്യ	remloc, i;

	എങ്കിൽ (bb->അക്കം == 0)
		തിരികെ -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	എങ്കിൽ (remloc < 0)
		തിരികെ -1;

	വേണ്ടി (i = remloc ; i < bb->അക്കം-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->അക്കം--;
	തിരികെ 0;
}

ശൂന്യം ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_struct_u32_iterate), &iter);
	എങ്കിൽ (retval)
		തിരികെ retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	തിരികെ 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	തിരികെ ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


പൂർണ്ണസംഖ്യ ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	എങ്കിൽ (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		തിരികെ 0;

	bb = iter->bb;

	എങ്കിൽ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		തിരികെ 0;

	എങ്കിൽ (iter->ptr < bb->അക്കം) {
		*blk = bb->list[iter->ptr++];
		തിരികെ 1;
	}
	*blk = 0;
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	തിരികെ ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


ശൂന്യം ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	എങ്കിൽ (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		തിരികെ;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

ശൂന്യം ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


പൂർണ്ണസംഖ്യ ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	എങ്കിൽ (bb1->അക്കം != bb2->അക്കം)
		തിരികെ 0;

	എങ്കിൽ (memcmp(bb1->list, bb2->list, bb1->അക്കം * വലുപ്പം(blk_t)) != 0)
		തിരികെ 0;
	തിരികെ 1;
}

പൂർണ്ണസംഖ്യ ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	തിരികെ ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

പൂർണ്ണസംഖ്യ ext2fs_u32_list_എണ്ണം(ext2_u32_list bb)
{
	തിരികെ bb->അക്കം;
}
