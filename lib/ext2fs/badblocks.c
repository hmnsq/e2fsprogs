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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
स्थिर errcode_t make_u32_list(पूर्णांक size, पूर्णांक अंक, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(आकार(संरचना ext2_struct_u32_list), &bb);
	जर (retval)
		परत retval;
	स_रखो(bb, 0, आकार(संरचना ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->अंक = अंक;
	retval = ext2fs_get_array(bb->size, आकार(blk_t), &bb->list);
	जर (retval) {
		ext2fs_free_mem(&bb);
		परत retval;
	}
	जर (list)
		स_नकल(bb->list, list, bb->size * आकार(blk_t));
	अन्यथा
		स_रखो(bb->list, 0, bb->size * आकार(blk_t));
	*ret = bb;
	परत 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, पूर्णांक size)
{
	परत make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, पूर्णांक size)
{
	परत make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->अंक, src->list, dest);
	जर (retval)
		परत retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	परत 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	परत ext2fs_u32_copy((ext2_u32_list) src,
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
	अचिन्हित दीर्घ	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	जर (bb->अंक >= bb->size) {
		old_size = bb->size * आकार(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * आकार(__u32),
					   &bb->list);
		जर (retval) {
			bb->size -= 100;
			परत retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->अंक-1;
	जर ((bb->अंक != 0) && (bb->list[i] == blk))
		परत 0;
	जर ((bb->अंक == 0) || (bb->list[i] < blk)) {
		bb->list[bb->अंक++] = blk;
		परत 0;
	}

	j = bb->अंक;
	साठी (i=0; i < bb->अंक; i++) {
		जर (bb->list[i] == blk)
			परत 0;
		जर (bb->list[i] > blk) {
			j = i;
			थांबा;
		}
	}
	साठी (i=bb->अंक; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->अंक++;
	परत 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	परत ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
पूर्णांक ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	पूर्णांक	खालचा, वरचा, mid;

	जर (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		परत -1;

	जर (bb->अंक == 0)
		परत -1;

	खालचा = 0;
	वरचा = bb->अंक-1;
	जर (blk == bb->list[खालचा])
		परत खालचा;
	जर (blk == bb->list[वरचा])
		परत वरचा;

	पर्यंत (खालचा < वरचा) {
		mid = ((अचिन्हित)खालचा + (अचिन्हित)वरचा)/2;
		जर (mid == खालचा || mid == वरचा)
			थांबा;
		जर (blk == bb->list[mid])
			परत mid;
		जर (blk < bb->list[mid])
			वरचा = mid;
		अन्यथा
			खालचा = mid;
	}
	परत -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
पूर्णांक ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	जर (ext2fs_u32_list_find(bb, blk) < 0)
		परत 0;
	अन्यथा
		परत 1;
}

पूर्णांक ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	परत ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
पूर्णांक ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	पूर्णांक	remloc, i;

	जर (bb->अंक == 0)
		परत -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	जर (remloc < 0)
		परत -1;

	साठी (i = remloc ; i < bb->अंक-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->अंक--;
	परत 0;
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

	retval = ext2fs_get_mem(आकार(संरचना ext2_struct_u32_iterate), &iter);
	जर (retval)
		परत retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	परत 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	परत ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


पूर्णांक ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	जर (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		परत 0;

	bb = iter->bb;

	जर (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		परत 0;

	जर (iter->ptr < bb->अंक) {
		*blk = bb->list[iter->ptr++];
		परत 1;
	}
	*blk = 0;
	परत 0;
}

पूर्णांक ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	परत ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


शून्य ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	जर (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		परत;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

शून्य ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


पूर्णांक ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	जर (bb1->अंक != bb2->अंक)
		परत 0;

	जर (स_भेद(bb1->list, bb2->list, bb1->अंक * आकार(blk_t)) != 0)
		परत 0;
	परत 1;
}

पूर्णांक ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	परत ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

पूर्णांक ext2fs_u32_list_मोजणी(ext2_u32_list bb)
{
	परत bb->अंक;
}
