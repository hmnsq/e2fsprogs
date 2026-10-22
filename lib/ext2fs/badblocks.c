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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
स्थिर errcode_t make_u32_list(पूर्णाङ्क आकार, पूर्णाङ्क अङ्कः, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_struct_u32_list), &bb);
	यदि (retval)
		प्रत्यागम retval;
	स्मृतिं_पूरय(bb, 0, परिमाण(संरचना ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->आकार = आकार ? आकार : 10;
	bb->अङ्कः = अङ्कः;
	retval = ext2fs_get_array(bb->आकार, परिमाण(blk_t), &bb->list);
	यदि (retval) {
		ext2fs_free_mem(&bb);
		प्रत्यागम retval;
	}
	यदि (list)
		स्मृतिं_प्रतिलिख(bb->list, list, bb->आकार * परिमाण(blk_t));
	अन्यथा
		स्मृतिं_पूरय(bb->list, 0, bb->आकार * परिमाण(blk_t));
	*ret = bb;
	प्रत्यागम 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, पूर्णाङ्क आकार)
{
	प्रत्यागम make_u32_list(आकार, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, पूर्णाङ्क आकार)
{
	प्रत्यागम make_u32_list(आकार, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->आकार, src->अङ्कः, src->list, dest);
	यदि (retval)
		प्रत्यागम retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	प्रत्यागम 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	प्रत्यागम ext2fs_u32_copy((ext2_u32_list) src,
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
errcode_t ext2fs_u32_list_सङ्कलय(ext2_u32_list bb, __u32 blk)
{
	errcode_t	retval;
	पूर्णाङ्क		i, j;
	अचिह्नित दीर्घ	old_आकार;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	यदि (bb->अङ्कः >= bb->आकार) {
		old_आकार = bb->आकार * परिमाण(__u32);
		bb->आकार += 100;
		retval = ext2fs_resize_mem(old_आकार, bb->आकार * परिमाण(__u32),
					   &bb->list);
		यदि (retval) {
			bb->आकार -= 100;
			प्रत्यागम retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->अङ्कः-1;
	यदि ((bb->अङ्कः != 0) && (bb->list[i] == blk))
		प्रत्यागम 0;
	यदि ((bb->अङ्कः == 0) || (bb->list[i] < blk)) {
		bb->list[bb->अङ्कः++] = blk;
		प्रत्यागम 0;
	}

	j = bb->अङ्कः;
	प्रति (i=0; i < bb->अङ्कः; i++) {
		यदि (bb->list[i] == blk)
			प्रत्यागम 0;
		यदि (bb->list[i] > blk) {
			j = i;
			भङ्ग;
		}
	}
	प्रति (i=bb->अङ्कः; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->अङ्कः++;
	प्रत्यागम 0;
}

errcode_t ext2fs_badblocks_list_सङ्कलय(ext2_badblocks_list bb, blk_t blk)
{
	प्रत्यागम ext2fs_u32_list_सङ्कलय((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
पूर्णाङ्क ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	पूर्णाङ्क	नीचः, उच्चः, mid;

	यदि (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		प्रत्यागम -1;

	यदि (bb->अङ्कः == 0)
		प्रत्यागम -1;

	नीचः = 0;
	उच्चः = bb->अङ्कः-1;
	यदि (blk == bb->list[नीचः])
		प्रत्यागम नीचः;
	यदि (blk == bb->list[उच्चः])
		प्रत्यागम उच्चः;

	यावत् (नीचः < उच्चः) {
		mid = ((अचिह्नित)नीचः + (अचिह्नित)उच्चः)/2;
		यदि (mid == नीचः || mid == उच्चः)
			भङ्ग;
		यदि (blk == bb->list[mid])
			प्रत्यागम mid;
		यदि (blk < bb->list[mid])
			उच्चः = mid;
		अन्यथा
			नीचः = mid;
	}
	प्रत्यागम -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
पूर्णाङ्क ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	यदि (ext2fs_u32_list_find(bb, blk) < 0)
		प्रत्यागम 0;
	अन्यथा
		प्रत्यागम 1;
}

पूर्णाङ्क ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	प्रत्यागम ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
पूर्णाङ्क ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	पूर्णाङ्क	remloc, i;

	यदि (bb->अङ्कः == 0)
		प्रत्यागम -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	यदि (remloc < 0)
		प्रत्यागम -1;

	प्रति (i = remloc ; i < bb->अङ्कः-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->अङ्कः--;
	प्रत्यागम 0;
}

शून्य ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_struct_u32_iterate), &iter);
	यदि (retval)
		प्रत्यागम retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	प्रत्यागम 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	प्रत्यागम ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


पूर्णाङ्क ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	यदि (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		प्रत्यागम 0;

	bb = iter->bb;

	यदि (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		प्रत्यागम 0;

	यदि (iter->ptr < bb->अङ्कः) {
		*blk = bb->list[iter->ptr++];
		प्रत्यागम 1;
	}
	*blk = 0;
	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	प्रत्यागम ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


शून्य ext2fs_u32_list_iterate_अन्तः(ext2_u32_iterate iter)
{
	यदि (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		प्रत्यागम;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

शून्य ext2fs_badblocks_list_iterate_अन्तः(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_अन्तः((ext2_u32_iterate) iter);
}


पूर्णाङ्क ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	यदि (bb1->अङ्कः != bb2->अङ्कः)
		प्रत्यागम 0;

	यदि (memcmp(bb1->list, bb2->list, bb1->अङ्कः * परिमाण(blk_t)) != 0)
		प्रत्यागम 0;
	प्रत्यागम 1;
}

पूर्णाङ्क ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	प्रत्यागम ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

पूर्णाङ्क ext2fs_u32_list_गणनम्(ext2_u32_list bb)
{
	प्रत्यागम bb->अङ्कः;
}
