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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <time.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"

স্থির EXT2_QSORT_TYPE dir_block_cmp(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b);
স্থির EXT2_QSORT_TYPE dir_block_cmp2(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b);
স্থির EXT2_QSORT_TYPE (*sortfunc32)(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
স্থির errcode_t make_dblist(ext2_filsys fs, ext2_ino_t আকার_2,
			     ext2_ino_t গুনতি,
			     কাঠামো ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = ফাঁকা;
	errcode_t	retval;
	ext2_ino_t	অঙ্ক_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		ফেরত 0;

	retval = ext2fs_get_mem(আকার(কাঠামো ext2_struct_dblist), &dblist);
	যদি (retval)
		যাও cleanup;
	memset(dblist, 0, আকার(কাঠামো ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	যদি (আকার_2)
		dblist->আকার_2 = আকার_2;
	অন্যথা {
		retval = ext2fs_get_অঙ্ক_dirs(fs, &অঙ্ক_dirs);
		যদি (retval)
			যাও cleanup;
		dblist->আকার_2 = (অঙ্ক_dirs * 2) + 12;
	}
	len = (size_t) আকার(কাঠামো ext2_db_entry2) * dblist->আকার_2;
	dblist->গুনতি = গুনতি;
	retval = ext2fs_get_array(dblist->আকার_2, আকার(কাঠামো ext2_db_entry2),
		&dblist->list);
	যদি (retval)
		যাও cleanup;

	যদি (list)
		memcpy(dblist->list, list, len);
	অন্যথা
		memset(dblist->list, 0, len);
	যদি (ret_dblist)
		*ret_dblist = dblist;
	অন্যথা
		fs->dblist = dblist;
	ফেরত 0;
cleanup:
	যদি (dblist)
		ext2fs_free_mem(&dblist);
	ফেরত retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	যদি (retval)
		ফেরত retval;

	dblist->sorted = 1;
	যদি (ret_dblist)
		*ret_dblist = dblist;
	অন্যথা
		fs->dblist = dblist;

	ফেরত 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->আকার_2, src->গুনতি, src->list,
			     &dblist);
	যদি (retval)
		ফেরত retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	ফেরত 0;
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
	কাঠামো ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_আকার;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	যদি (dblist->গুনতি >= dblist->আকার_2) {
		old_আকার = dblist->আকার_2 * আকার(কাঠামো ext2_db_entry2);
		dblist->আকার_2 += dblist->আকার_2 > 200 ? dblist->আকার_2 / 2 : 100;
		retval = ext2fs_resize_mem(old_আকার, (size_t) dblist->আকার_2 *
					   আকার(কাঠামো ext2_db_entry2),
					   &dblist->list);
		যদি (retval) {
			dblist->আকার_2 = old_আকার / আকার(কাঠামো ext2_db_entry2);
			ফেরত retval;
		}
	}
	new_entry = dblist->list + ( dblist->গুনতি++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	ফেরত 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	জন্য (i=0; i < dblist->গুনতি; i++) {
		যদি ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			চালু;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		ফেরত 0;
	}
	ফেরত EXT2_ET_DB_NOT_FOUND;
}

শূন্য ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ধ্রুবক শূন্য *,
						     ধ্রুবক শূন্য *))
{
	যদি (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->গুনতি,
	      আকার(কাঠামো ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 পূর্ণসংখ্যা (*func)(ext2_filsys fs,
					     কাঠামো ext2_db_entry2 *db_info,
					     শূন্য	*priv_তথ্য),
				 unsigned long long start,
				 unsigned long long গুনতি,
				 শূন্য *priv_তথ্য)
{
	unsigned long long	i, end;
	পূর্ণসংখ্যা		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + গুনতি;
	যদি (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	যদি (end > dblist->গুনতি)
		end = dblist->গুনতি;
	জন্য (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_তথ্য);
		যদি (ret & DBLIST_ABORT)
			ফেরত 0;
	}
	ফেরত 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 পূর্ণসংখ্যা (*func)(ext2_filsys fs,
					     কাঠামো ext2_db_entry2 *db_info,
					     শূন্য	*priv_তথ্য),
				 শূন্য *priv_তথ্য)
{
	ফেরত ext2fs_dblist_iterate3(dblist, func, 0, dblist->গুনতি,
				      priv_তথ্য);
}

স্থির EXT2_QSORT_TYPE dir_block_cmp2(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
	ধ্রুবক কাঠামো ext2_db_entry2 *db_a =
		(ধ্রুবক কাঠামো ext2_db_entry2 *) a;
	ধ্রুবক কাঠামো ext2_db_entry2 *db_b =
		(ধ্রুবক কাঠামো ext2_db_entry2 *) b;

	যদি (db_a->blk != db_b->blk)
		ফেরত (পূর্ণসংখ্যা) (db_a->blk - db_b->blk);

	যদি (db_a->ino != db_b->ino)
		ফেরত (পূর্ণসংখ্যা) (db_a->ino - db_b->ino);

	ফেরত (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	ফেরত dblist->গুনতি;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  কাঠামো ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	যদি (dblist->গুনতি == 0)
		ফেরত EXT2_ET_DBLIST_EMPTY;

	যদি (entry)
		*entry = dblist->list + ( dblist->গুনতি-1);
	ফেরত 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	যদি (dblist->গুনতি == 0)
		ফেরত EXT2_ET_DBLIST_EMPTY;

	dblist->গুনতি--;
	ফেরত 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       পূর্ণসংখ্যা blockcnt)
{
	ফেরত ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       পূর্ণসংখ্যা blockcnt)
{
	ফেরত ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

শূন্য ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ধ্রুবক শূন্য *,
						    ধ্রুবক শূন্য *))
{
	যদি (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} অন্যথা
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->গুনতি,
	      আকার(কাঠামো ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
কাঠামো iterate_passthrough {
	পূর্ণসংখ্যা (*func)(ext2_filsys fs,
		    কাঠামো ext2_db_entry *db_info,
		    শূন্য	*priv_তথ্য);
	শূন্য *priv_তথ্য;
};

স্থির পূর্ণসংখ্যা passthrough_func(ext2_filsys fs,
			    কাঠামো ext2_db_entry2 *db_info,
			    শূন্য	*priv_তথ্য)
{
	কাঠামো iterate_passthrough *p = priv_তথ্য;
	কাঠামো ext2_db_entry db;
	পূর্ণসংখ্যা ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (পূর্ণসংখ্যা) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_তথ্য);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	ফেরত ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				পূর্ণসংখ্যা (*func)(ext2_filsys fs,
					    কাঠামো ext2_db_entry *db_info,
					    শূন্য	*priv_তথ্য),
				শূন্য *priv_তথ্য)
{
	কাঠামো iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_তথ্য = priv_তথ্য;

	ফেরত ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

স্থির EXT2_QSORT_TYPE dir_block_cmp(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
	ধ্রুবক কাঠামো ext2_db_entry2 *db_a =
		(ধ্রুবক কাঠামো ext2_db_entry2 *) a;
	ধ্রুবক কাঠামো ext2_db_entry2 *db_b =
		(ধ্রুবক কাঠামো ext2_db_entry2 *) b;

	কাঠামো ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	ফেরত sortfunc32(&a32, &b32);
}

পূর্ণসংখ্যা ext2fs_dblist_গুনতি(ext2_dblist dblist)
{
	ফেরত dblist->গুনতি;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 কাঠামো ext2_db_entry **entry)
{
	স্থির কাঠামো ext2_db_entry ret_entry;
	কাঠামো ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	যদি (dblist->গুনতি == 0)
		ফেরত EXT2_ET_DBLIST_EMPTY;

	যদি (!entry)
		ফেরত 0;

	last = dblist->list + dblist->গুনতি -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	ফেরত 0;
}

