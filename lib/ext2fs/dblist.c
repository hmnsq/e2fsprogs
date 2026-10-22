/*
 * dblist.c -- directory block list functions
 *
 * Copyright 1997 by Theodore Ts'o
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर EXT2_QSORT_TYPE dir_block_cmp(ध्रुव शून्य *a, ध्रुव शून्य *b);
स्थिर EXT2_QSORT_TYPE dir_block_cmp2(ध्रुव शून्य *a, ध्रुव शून्य *b);
स्थिर EXT2_QSORT_TYPE (*sortfunc32)(ध्रुव शून्य *a, ध्रुव शून्य *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
स्थिर errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t गणनम्,
			     संरचना ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = रिक्त;
	errcode_t	retval;
	ext2_ino_t	अङ्कः_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	सचे ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		पटिनिवत्त 0;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_struct_dblist), &dblist);
	सचे (retval)
		गच्छ cleanup;
	memset(dblist, 0, परिमाण(संरचना ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	सचे (size)
		dblist->size = size;
	अञ्ञथा {
		retval = ext2fs_get_अङ्कः_dirs(fs, &अङ्कः_dirs);
		सचे (retval)
			गच्छ cleanup;
		dblist->size = (अङ्कः_dirs * 2) + 12;
	}
	len = (size_t) परिमाण(संरचना ext2_db_entry2) * dblist->size;
	dblist->गणनम् = गणनम्;
	retval = ext2fs_get_array(dblist->size, परिमाण(संरचना ext2_db_entry2),
		&dblist->list);
	सचे (retval)
		गच्छ cleanup;

	सचे (list)
		memcpy(dblist->list, list, len);
	अञ्ञथा
		memset(dblist->list, 0, len);
	सचे (ret_dblist)
		*ret_dblist = dblist;
	अञ्ञथा
		fs->dblist = dblist;
	पटिनिवत्त 0;
cleanup:
	सचे (dblist)
		ext2fs_free_mem(&dblist);
	पटिनिवत्त retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	सचे (retval)
		पटिनिवत्त retval;

	dblist->sorted = 1;
	सचे (ret_dblist)
		*ret_dblist = dblist;
	अञ्ञथा
		fs->dblist = dblist;

	पटिनिवत्त 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->size, src->गणनम्, src->list,
			     &dblist);
	सचे (retval)
		पटिनिवत्त retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	पटिनिवत्त 0;
}

/*
 * Close a directory block list
 *
 * (moved to closefs.c)
 */


/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	संरचना ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	सचे (dblist->गणनम् >= dblist->size) {
		old_size = dblist->size * परिमाण(संरचना ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   परिमाण(संरचना ext2_db_entry2),
					   &dblist->list);
		सचे (retval) {
			dblist->size = old_size / परिमाण(संरचना ext2_db_entry2);
			पटिनिवत्त retval;
		}
	}
	new_entry = dblist->list + ( dblist->गणनम्++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	पटिनिवत्त 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	अत्थाय (i=0; i < dblist->गणनम्; i++) {
		सचे ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			अनुवर्त;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		पटिनिवत्त 0;
	}
	पटिनिवत्त EXT2_ET_DB_NOT_FOUND;
}

शून्य ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ध्रुव शून्य *,
						     ध्रुव शून्य *))
{
	सचे (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->गणनम्,
	      परिमाण(संरचना ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 पूर्णाङ्क (*func)(ext2_filsys fs,
					     संरचना ext2_db_entry2 *db_info,
					     शून्य	*priv_data),
				 unsigned long long start,
				 unsigned long long गणनम्,
				 शून्य *priv_data)
{
	unsigned long long	i, end;
	पूर्णाङ्क		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + गणनम्;
	सचे (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	सचे (end > dblist->गणनम्)
		end = dblist->गणनम्;
	अत्थाय (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		सचे (ret & DBLIST_ABORT)
			पटिनिवत्त 0;
	}
	पटिनिवत्त 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 पूर्णाङ्क (*func)(ext2_filsys fs,
					     संरचना ext2_db_entry2 *db_info,
					     शून्य	*priv_data),
				 शून्य *priv_data)
{
	पटिनिवत्त ext2fs_dblist_iterate3(dblist, func, 0, dblist->गणनम्,
				      priv_data);
}

स्थिर EXT2_QSORT_TYPE dir_block_cmp2(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	ध्रुव संरचना ext2_db_entry2 *db_a =
		(ध्रुव संरचना ext2_db_entry2 *) a;
	ध्रुव संरचना ext2_db_entry2 *db_b =
		(ध्रुव संरचना ext2_db_entry2 *) b;

	सचे (db_a->blk != db_b->blk)
		पटिनिवत्त (पूर्णाङ्क) (db_a->blk - db_b->blk);

	सचे (db_a->ino != db_b->ino)
		पटिनिवत्त (पूर्णाङ्क) (db_a->ino - db_b->ino);

	पटिनिवत्त (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	पटिनिवत्त dblist->गणनम्;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  संरचना ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	सचे (dblist->गणनम् == 0)
		पटिनिवत्त EXT2_ET_DBLIST_EMPTY;

	सचे (entry)
		*entry = dblist->list + ( dblist->गणनम्-1);
	पटिनिवत्त 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	सचे (dblist->गणनम् == 0)
		पटिनिवत्त EXT2_ET_DBLIST_EMPTY;

	dblist->गणनम्--;
	पटिनिवत्त 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       पूर्णाङ्क blockcnt)
{
	पटिनिवत्त ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       पूर्णाङ्क blockcnt)
{
	पटिनिवत्त ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

शून्य ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ध्रुव शून्य *,
						    ध्रुव शून्य *))
{
	सचे (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} अञ्ञथा
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->गणनम्,
	      परिमाण(संरचना ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
संरचना iterate_passthrough {
	पूर्णाङ्क (*func)(ext2_filsys fs,
		    संरचना ext2_db_entry *db_info,
		    शून्य	*priv_data);
	शून्य *priv_data;
};

स्थिर पूर्णाङ्क passthrough_func(ext2_filsys fs,
			    संरचना ext2_db_entry2 *db_info,
			    शून्य	*priv_data)
{
	संरचना iterate_passthrough *p = priv_data;
	संरचना ext2_db_entry db;
	पूर्णाङ्क ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (पूर्णाङ्क) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	पटिनिवत्त ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				पूर्णाङ्क (*func)(ext2_filsys fs,
					    संरचना ext2_db_entry *db_info,
					    शून्य	*priv_data),
				शून्य *priv_data)
{
	संरचना iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	पटिनिवत्त ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

स्थिर EXT2_QSORT_TYPE dir_block_cmp(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	ध्रुव संरचना ext2_db_entry2 *db_a =
		(ध्रुव संरचना ext2_db_entry2 *) a;
	ध्रुव संरचना ext2_db_entry2 *db_b =
		(ध्रुव संरचना ext2_db_entry2 *) b;

	संरचना ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	पटिनिवत्त sortfunc32(&a32, &b32);
}

पूर्णाङ्क ext2fs_dblist_गणनम्(ext2_dblist dblist)
{
	पटिनिवत्त dblist->गणनम्;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 संरचना ext2_db_entry **entry)
{
	स्थिर संरचना ext2_db_entry ret_entry;
	संरचना ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	सचे (dblist->गणनम् == 0)
		पटिनिवत्त EXT2_ET_DBLIST_EMPTY;

	सचे (!entry)
		पटिनिवत्त 0;

	last = dblist->list + dblist->गणनम् -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	पटिनिवत्त 0;
}

