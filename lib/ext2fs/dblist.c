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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

ایستا EXT2_QSORT_TYPE dir_block_cmp(ثابت تهی *a, ثابت تهی *b);
ایستا EXT2_QSORT_TYPE dir_block_cmp2(ثابت تهی *a, ثابت تهی *b);
ایستا EXT2_QSORT_TYPE (*sortfunc32)(ثابت تهی *a, ثابت تهی *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
ایستا errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t شمار,
			     ساختار ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = پوچ;
	errcode_t	retval;
	ext2_ino_t	رقم_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		بازگشت 0;

	retval = ext2fs_get_mem(اندازه(ساختار ext2_struct_dblist), &dblist);
	اگر (retval)
		برو cleanup;
	memset(dblist, 0, اندازه(ساختار ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	اگر (size)
		dblist->size = size;
	وگرنه {
		retval = ext2fs_get_رقم_dirs(fs, &رقم_dirs);
		اگر (retval)
			برو cleanup;
		dblist->size = (رقم_dirs * 2) + 12;
	}
	len = (size_t) اندازه(ساختار ext2_db_entry2) * dblist->size;
	dblist->شمار = شمار;
	retval = ext2fs_get_array(dblist->size, اندازه(ساختار ext2_db_entry2),
		&dblist->list);
	اگر (retval)
		برو cleanup;

	اگر (list)
		memcpy(dblist->list, list, len);
	وگرنه
		memset(dblist->list, 0, len);
	اگر (ret_dblist)
		*ret_dblist = dblist;
	وگرنه
		fs->dblist = dblist;
	بازگشت 0;
cleanup:
	اگر (dblist)
		ext2fs_free_mem(&dblist);
	بازگشت retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	اگر (retval)
		بازگشت retval;

	dblist->sorted = 1;
	اگر (ret_dblist)
		*ret_dblist = dblist;
	وگرنه
		fs->dblist = dblist;

	بازگشت 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->size, src->شمار, src->list,
			     &dblist);
	اگر (retval)
		بازگشت retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	بازگشت 0;
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
	ساختار ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	اگر (dblist->شمار >= dblist->size) {
		old_size = dblist->size * اندازه(ساختار ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   اندازه(ساختار ext2_db_entry2),
					   &dblist->list);
		اگر (retval) {
			dblist->size = old_size / اندازه(ساختار ext2_db_entry2);
			بازگشت retval;
		}
	}
	new_entry = dblist->list + ( dblist->شمار++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	بازگشت 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	برای (i=0; i < dblist->شمار; i++) {
		اگر ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			ادامه;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		بازگشت 0;
	}
	بازگشت EXT2_ET_DB_NOT_FOUND;
}

تهی ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ثابت تهی *,
						     ثابت تهی *))
{
	اگر (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->شمار,
	      اندازه(ساختار ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 صحیح (*func)(ext2_filsys fs,
					     ساختار ext2_db_entry2 *db_info,
					     تهی	*priv_data),
				 unsigned long long start,
				 unsigned long long شمار,
				 تهی *priv_data)
{
	unsigned long long	i, end;
	صحیح		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + شمار;
	اگر (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	اگر (end > dblist->شمار)
		end = dblist->شمار;
	برای (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		اگر (ret & DBLIST_ABORT)
			بازگشت 0;
	}
	بازگشت 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 صحیح (*func)(ext2_filsys fs,
					     ساختار ext2_db_entry2 *db_info,
					     تهی	*priv_data),
				 تهی *priv_data)
{
	بازگشت ext2fs_dblist_iterate3(dblist, func, 0, dblist->شمار,
				      priv_data);
}

ایستا EXT2_QSORT_TYPE dir_block_cmp2(ثابت تهی *a, ثابت تهی *b)
{
	ثابت ساختار ext2_db_entry2 *db_a =
		(ثابت ساختار ext2_db_entry2 *) a;
	ثابت ساختار ext2_db_entry2 *db_b =
		(ثابت ساختار ext2_db_entry2 *) b;

	اگر (db_a->blk != db_b->blk)
		بازگشت (صحیح) (db_a->blk - db_b->blk);

	اگر (db_a->ino != db_b->ino)
		بازگشت (صحیح) (db_a->ino - db_b->ino);

	بازگشت (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	بازگشت dblist->شمار;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  ساختار ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	اگر (dblist->شمار == 0)
		بازگشت EXT2_ET_DBLIST_EMPTY;

	اگر (entry)
		*entry = dblist->list + ( dblist->شمار-1);
	بازگشت 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	اگر (dblist->شمار == 0)
		بازگشت EXT2_ET_DBLIST_EMPTY;

	dblist->شمار--;
	بازگشت 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       صحیح blockcnt)
{
	بازگشت ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       صحیح blockcnt)
{
	بازگشت ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

تهی ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ثابت تهی *,
						    ثابت تهی *))
{
	اگر (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} وگرنه
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->شمار,
	      اندازه(ساختار ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
ساختار iterate_passthrough {
	صحیح (*func)(ext2_filsys fs,
		    ساختار ext2_db_entry *db_info,
		    تهی	*priv_data);
	تهی *priv_data;
};

ایستا صحیح passthrough_func(ext2_filsys fs,
			    ساختار ext2_db_entry2 *db_info,
			    تهی	*priv_data)
{
	ساختار iterate_passthrough *p = priv_data;
	ساختار ext2_db_entry db;
	صحیح ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (صحیح) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	بازگشت ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				صحیح (*func)(ext2_filsys fs,
					    ساختار ext2_db_entry *db_info,
					    تهی	*priv_data),
				تهی *priv_data)
{
	ساختار iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	بازگشت ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

ایستا EXT2_QSORT_TYPE dir_block_cmp(ثابت تهی *a, ثابت تهی *b)
{
	ثابت ساختار ext2_db_entry2 *db_a =
		(ثابت ساختار ext2_db_entry2 *) a;
	ثابت ساختار ext2_db_entry2 *db_b =
		(ثابت ساختار ext2_db_entry2 *) b;

	ساختار ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	بازگشت sortfunc32(&a32, &b32);
}

صحیح ext2fs_dblist_شمار(ext2_dblist dblist)
{
	بازگشت dblist->شمار;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 ساختار ext2_db_entry **entry)
{
	ایستا ساختار ext2_db_entry ret_entry;
	ساختار ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	اگر (dblist->شمار == 0)
		بازگشت EXT2_ET_DBLIST_EMPTY;

	اگر (!entry)
		بازگشت 0;

	last = dblist->list + dblist->شمار -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	بازگشت 0;
}

