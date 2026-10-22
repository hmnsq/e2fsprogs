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
#اگر_ختم
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

ساکن EXT2_QSORT_TYPE dir_بلاک_cmp(ثابت خالی *a, ثابت خالی *b);
ساکن EXT2_QSORT_TYPE dir_بلاک_cmp2(ثابت خالی *a, ثابت خالی *b);
ساکن EXT2_QSORT_TYPE (*sortfunc32)(ثابت خالی *a, ثابت خالی *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
ساکن errcode_t make_dblist(ext2_filsys fs, ext2_ino_t سائز,
			     ext2_ino_t گنتی,
			     ساخت ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = تہی;
	errcode_t	retval;
	ext2_ino_t	عدد_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		واپس 0;

	retval = ext2fs_get_mem(حجم(ساخت ext2_struct_dblist), &dblist);
	اگر (retval)
		جاؤ cleanup;
	memset(dblist, 0, حجم(ساخت ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	اگر (سائز)
		dblist->سائز = سائز;
	ورنہ {
		retval = ext2fs_get_عدد_dirs(fs, &عدد_dirs);
		اگر (retval)
			جاؤ cleanup;
		dblist->سائز = (عدد_dirs * 2) + 12;
	}
	len = (size_t) حجم(ساخت ext2_db_entry2) * dblist->سائز;
	dblist->گنتی = گنتی;
	retval = ext2fs_get_array(dblist->سائز, حجم(ساخت ext2_db_entry2),
		&dblist->list);
	اگر (retval)
		جاؤ cleanup;

	اگر (list)
		حافظہ_نقل(dblist->list, list, len);
	ورنہ
		memset(dblist->list, 0, len);
	اگر (ret_dblist)
		*ret_dblist = dblist;
	ورنہ
		fs->dblist = dblist;
	واپس 0;
cleanup:
	اگر (dblist)
		ext2fs_free_mem(&dblist);
	واپس retval;
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
		واپس retval;

	dblist->sorted = 1;
	اگر (ret_dblist)
		*ret_dblist = dblist;
	ورنہ
		fs->dblist = dblist;

	واپس 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->سائز, src->گنتی, src->list,
			     &dblist);
	اگر (retval)
		واپس retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	واپس 0;
}

/*
 * Close a directory block list
 *
 * (moved to closefs.c)
 */


/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_جوڑو_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	ساخت ext2_db_entry2 	*new_اندراج;
	errcode_t		retval;
	غیرعلامتی طویل		old_سائز;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	اگر (dblist->گنتی >= dblist->سائز) {
		old_سائز = dblist->سائز * حجم(ساخت ext2_db_entry2);
		dblist->سائز += dblist->سائز > 200 ? dblist->سائز / 2 : 100;
		retval = ext2fs_resize_mem(old_سائز, (size_t) dblist->سائز *
					   حجم(ساخت ext2_db_entry2),
					   &dblist->list);
		اگر (retval) {
			dblist->سائز = old_سائز / حجم(ساخت ext2_db_entry2);
			واپس retval;
		}
	}
	new_اندراج = dblist->list + ( dblist->گنتی++);
	new_اندراج->blk = blk;
	new_اندراج->ino = ino;
	new_اندراج->blockcnt = blockcnt;

	dblist->sorted = 0;

	واپس 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_سیٹ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	برائے (i=0; i < dblist->گنتی; i++) {
		اگر ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			جاری;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		واپس 0;
	}
	واپس EXT2_ET_DB_NOT_ملا;
}

خالی ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ثابت خالی *,
						     ثابت خالی *))
{
	اگر (!sortfunc)
		sortfunc = dir_بلاک_cmp2;
	qsort(dblist->list, (size_t) dblist->گنتی,
	      حجم(ساخت ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 صحیح (*func)(ext2_filsys fs,
					     ساخت ext2_db_entry2 *db_info,
					     خالی	*priv_ڈیٹا),
				 غیرعلامتی طویل طویل آغاز,
				 غیرعلامتی طویل طویل گنتی,
				 خالی *priv_ڈیٹا)
{
	غیرعلامتی طویل طویل	i, انتہا;
	صحیح		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	انتہا = آغاز + گنتی;
	اگر (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	اگر (انتہا > dblist->گنتی)
		انتہا = dblist->گنتی;
	برائے (i = آغاز; i < انتہا; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_ڈیٹا);
		اگر (ret & DBLIST_ABORT)
			واپس 0;
	}
	واپس 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 صحیح (*func)(ext2_filsys fs,
					     ساخت ext2_db_entry2 *db_info,
					     خالی	*priv_ڈیٹا),
				 خالی *priv_ڈیٹا)
{
	واپس ext2fs_dblist_iterate3(dblist, func, 0, dblist->گنتی,
				      priv_ڈیٹا);
}

ساکن EXT2_QSORT_TYPE dir_بلاک_cmp2(ثابت خالی *a, ثابت خالی *b)
{
	ثابت ساخت ext2_db_entry2 *db_a =
		(ثابت ساخت ext2_db_entry2 *) a;
	ثابت ساخت ext2_db_entry2 *db_b =
		(ثابت ساخت ext2_db_entry2 *) b;

	اگر (db_a->blk != db_b->blk)
		واپس (صحیح) (db_a->blk - db_b->blk);

	اگر (db_a->ino != db_b->ino)
		واپس (صحیح) (db_a->ino - db_b->ino);

	واپس (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	واپس dblist->گنتی;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  ساخت ext2_db_entry2 **اندراج_2)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	اگر (dblist->گنتی == 0)
		واپس EXT2_ET_DBLIST_خالی_خانے;

	اگر (اندراج_2)
		*اندراج_2 = dblist->list + ( dblist->گنتی-1);
	واپس 0;
}

errcode_t ext2fs_dblist_drop_آخری(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	اگر (dblist->گنتی == 0)
		واپس EXT2_ET_DBLIST_خالی_خانے;

	dblist->گنتی--;
	واپس 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_جوڑو_dir_بلاک(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       صحیح blockcnt)
{
	واپس ext2fs_جوڑو_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_سیٹ_dir_بلاک(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       صحیح blockcnt)
{
	واپس ext2fs_سیٹ_dir_block2(dblist, ino, blk, blockcnt);
}

خالی ext2fs_dblist_ترتیب_دو(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ثابت خالی *,
						    ثابت خالی *))
{
	اگر (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_بلاک_cmp;
	} ورنہ
		sortfunc = dir_بلاک_cmp2;
	qsort(dblist->list, (size_t) dblist->گنتی,
	      حجم(ساخت ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
ساخت iterate_passthrough {
	صحیح (*func)(ext2_filsys fs,
		    ساخت ext2_db_اندراج *db_info,
		    خالی	*priv_ڈیٹا);
	خالی *priv_ڈیٹا;
};

ساکن صحیح passthrough_func(ext2_filsys fs,
			    ساخت ext2_db_entry2 *db_info,
			    خالی	*priv_ڈیٹا)
{
	ساخت iterate_passthrough *p = priv_ڈیٹا;
	ساخت ext2_db_اندراج db;
	صحیح ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (صحیح) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_ڈیٹا);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	واپس ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				صحیح (*func)(ext2_filsys fs,
					    ساخت ext2_db_اندراج *db_info,
					    خالی	*priv_ڈیٹا),
				خالی *priv_ڈیٹا)
{
	ساخت iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_ڈیٹا = priv_ڈیٹا;

	واپس ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

ساکن EXT2_QSORT_TYPE dir_بلاک_cmp(ثابت خالی *a, ثابت خالی *b)
{
	ثابت ساخت ext2_db_entry2 *db_a =
		(ثابت ساخت ext2_db_entry2 *) a;
	ثابت ساخت ext2_db_entry2 *db_b =
		(ثابت ساخت ext2_db_entry2 *) b;

	ساخت ext2_db_اندراج a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	واپس sortfunc32(&a32, &b32);
}

صحیح ext2fs_dblist_گنتی(ext2_dblist dblist)
{
	واپس dblist->گنتی;
}

errcode_t ext2fs_dblist_get_آخری(ext2_dblist dblist,
				 ساخت ext2_db_اندراج **اندراج_2)
{
	ساکن ساخت ext2_db_اندراج ret_اندراج;
	ساخت ext2_db_entry2 *آخری;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	اگر (dblist->گنتی == 0)
		واپس EXT2_ET_DBLIST_خالی_خانے;

	اگر (!اندراج_2)
		واپس 0;

	آخری = dblist->list + dblist->گنتی -1;

	ret_اندراج.ino = آخری->ino;
	ret_اندراج.blk = آخری->blk;
	ret_اندراج.blockcnt = آخری->blockcnt;
	*اندراج_2 = &ret_اندراج;

	واپس 0;
}

