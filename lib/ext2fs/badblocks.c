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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
अटल errcode_t make_u32_list(पूर्णांक आकार, पूर्णांक अंक, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(माप(काष्ठा ext2_struct_u32_list), &bb);
	अगर (retval)
		वापस retval;
	स_रखो(bb, 0, माप(काष्ठा ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->आकार = आकार ? आकार : 10;
	bb->अंक = अंक;
	retval = ext2fs_get_array(bb->आकार, माप(blk_t), &bb->list);
	अगर (retval) {
		ext2fs_free_mem(&bb);
		वापस retval;
	}
	अगर (list)
		स_नकल(bb->list, list, bb->आकार * माप(blk_t));
	अन्यथा
		स_रखो(bb->list, 0, bb->आकार * माप(blk_t));
	*ret = bb;
	वापस 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, पूर्णांक आकार)
{
	वापस make_u32_list(आकार, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, पूर्णांक आकार)
{
	वापस make_u32_list(आकार, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->आकार, src->अंक, src->list, dest);
	अगर (retval)
		वापस retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	वापस 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	वापस ext2fs_u32_copy((ext2_u32_list) src,
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
	पूर्णांक		i, j;
	अचिन्हित दीर्घ	old_आकार;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	अगर (bb->अंक >= bb->आकार) {
		old_आकार = bb->आकार * माप(__u32);
		bb->आकार += 100;
		retval = ext2fs_resize_mem(old_आकार, bb->आकार * माप(__u32),
					   &bb->list);
		अगर (retval) {
			bb->आकार -= 100;
			वापस retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->अंक-1;
	अगर ((bb->अंक != 0) && (bb->list[i] == blk))
		वापस 0;
	अगर ((bb->अंक == 0) || (bb->list[i] < blk)) {
		bb->list[bb->अंक++] = blk;
		वापस 0;
	}

	j = bb->अंक;
	क्रम (i=0; i < bb->अंक; i++) {
		अगर (bb->list[i] == blk)
			वापस 0;
		अगर (bb->list[i] > blk) {
			j = i;
			अवरोध;
		}
	}
	क्रम (i=bb->अंक; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->अंक++;
	वापस 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	वापस ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
पूर्णांक ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	पूर्णांक	निचला, ऊपरी, mid;

	अगर (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		वापस -1;

	अगर (bb->अंक == 0)
		वापस -1;

	निचला = 0;
	ऊपरी = bb->अंक-1;
	अगर (blk == bb->list[निचला])
		वापस निचला;
	अगर (blk == bb->list[ऊपरी])
		वापस ऊपरी;

	जबतक (निचला < ऊपरी) {
		mid = ((अचिन्हित)निचला + (अचिन्हित)ऊपरी)/2;
		अगर (mid == निचला || mid == ऊपरी)
			अवरोध;
		अगर (blk == bb->list[mid])
			वापस mid;
		अगर (blk < bb->list[mid])
			ऊपरी = mid;
		अन्यथा
			निचला = mid;
	}
	वापस -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
पूर्णांक ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	अगर (ext2fs_u32_list_find(bb, blk) < 0)
		वापस 0;
	अन्यथा
		वापस 1;
}

पूर्णांक ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	वापस ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
पूर्णांक ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	पूर्णांक	remloc, i;

	अगर (bb->अंक == 0)
		वापस -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	अगर (remloc < 0)
		वापस -1;

	क्रम (i = remloc ; i < bb->अंक-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->अंक--;
	वापस 0;
}

व्योम ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(माप(काष्ठा ext2_struct_u32_iterate), &iter);
	अगर (retval)
		वापस retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	वापस 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	वापस ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


पूर्णांक ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	अगर (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		वापस 0;

	bb = iter->bb;

	अगर (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		वापस 0;

	अगर (iter->ptr < bb->अंक) {
		*blk = bb->list[iter->ptr++];
		वापस 1;
	}
	*blk = 0;
	वापस 0;
}

पूर्णांक ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	वापस ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


व्योम ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	अगर (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		वापस;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

व्योम ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


पूर्णांक ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	अगर (bb1->अंक != bb2->अंक)
		वापस 0;

	अगर (स_भेद(bb1->list, bb2->list, bb1->अंक * माप(blk_t)) != 0)
		वापस 0;
	वापस 1;
}

पूर्णांक ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	वापस ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

पूर्णांक ext2fs_u32_list_गिनती(ext2_u32_list bb)
{
	वापस bb->अंक;
}
