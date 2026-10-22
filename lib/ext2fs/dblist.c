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

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

ساكن EXT2_QSORT_TYPE dir_بلاک_cmp(ثابت فراغ *a, ثابت فراغ *b);
ساكن EXT2_QSORT_TYPE dir_بلاک_cmp2(ثابت فراغ *a, ثابت فراغ *b);
ساكن EXT2_QSORT_TYPE (*sortfunc32)(ثابت فراغ *a, ثابت فراغ *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
ساكن errcode_t make_dblist(ext2_filsys fs, ext2_ino_t اندازه,
			     ext2_ino_t شمېر,
			     بنية ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = عدم;
	errcode_t	retval;
	ext2_ino_t	عدد_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		أرجع 0;

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_dblist), &dblist);
	إذا (retval)
		اذهب cleanup;
	memset(dblist, 0, حجم(بنية ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	إذا (اندازه)
		dblist->اندازه = اندازه;
	وإلا {
		retval = ext2fs_get_عدد_dirs(fs, &عدد_dirs);
		إذا (retval)
			اذهب cleanup;
		dblist->اندازه = (عدد_dirs * 2) + 12;
	}
	len = (size_t) حجم(بنية ext2_db_entry2) * dblist->اندازه;
	dblist->شمېر = شمېر;
	retval = ext2fs_get_array(dblist->اندازه, حجم(بنية ext2_db_entry2),
		&dblist->list);
	إذا (retval)
		اذهب cleanup;

	إذا (list)
		حافظه_نقل(dblist->list, list, len);
	وإلا
		memset(dblist->list, 0, len);
	إذا (ret_dblist)
		*ret_dblist = dblist;
	وإلا
		fs->dblist = dblist;
	أرجع 0;
cleanup:
	إذا (dblist)
		ext2fs_free_mem(&dblist);
	أرجع retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	إذا (retval)
		أرجع retval;

	dblist->sorted = 1;
	إذا (ret_dblist)
		*ret_dblist = dblist;
	وإلا
		fs->dblist = dblist;

	أرجع 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->اندازه, src->شمېر, src->list,
			     &dblist);
	إذا (retval)
		أرجع retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	أرجع 0;
}

/*
 * Close a directory block list
 *
 * (moved to closefs.c)
 */


/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_جمع_کړه_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	بنية ext2_db_entry2 	*new_ثبت;
	errcode_t		retval;
	بینښه اوږد		old_اندازه;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->شمېر >= dblist->اندازه) {
		old_اندازه = dblist->اندازه * حجم(بنية ext2_db_entry2);
		dblist->اندازه += dblist->اندازه > 200 ? dblist->اندازه / 2 : 100;
		retval = ext2fs_resize_mem(old_اندازه, (size_t) dblist->اندازه *
					   حجم(بنية ext2_db_entry2),
					   &dblist->list);
		إذا (retval) {
			dblist->اندازه = old_اندازه / حجم(بنية ext2_db_entry2);
			أرجع retval;
		}
	}
	new_ثبت = dblist->list + ( dblist->شمېر++);
	new_ثبت->blk = blk;
	new_ثبت->ino = ino;
	new_ثبت->blockcnt = blockcnt;

	dblist->sorted = 0;

	أرجع 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_سېټ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	لكل (i=0; i < dblist->شمېر; i++) {
		إذا ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			استمر;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		أرجع 0;
	}
	أرجع EXT2_ET_DB_NOT_وموندل;
}

فراغ ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
						     ثابت فراغ *))
{
	إذا (!sortfunc)
		sortfunc = dir_بلاک_cmp2;
	qsort(dblist->list, (size_t) dblist->شمېر,
	      حجم(بنية ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 صحيح (*func)(ext2_filsys fs,
					     بنية ext2_db_entry2 *db_info,
					     فراغ	*priv_معلومات),
				 بینښه اوږد اوږد پیل,
				 بینښه اوږد اوږد شمېر,
				 فراغ *priv_معلومات)
{
	بینښه اوږد اوږد	i, پای_ټکی;
	صحيح		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	پای_ټکی = پیل + شمېر;
	إذا (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	إذا (پای_ټکی > dblist->شمېر)
		پای_ټکی = dblist->شمېر;
	لكل (i = پیل; i < پای_ټکی; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_معلومات);
		إذا (ret & DBLIST_ABORT)
			أرجع 0;
	}
	أرجع 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 صحيح (*func)(ext2_filsys fs,
					     بنية ext2_db_entry2 *db_info,
					     فراغ	*priv_معلومات),
				 فراغ *priv_معلومات)
{
	أرجع ext2fs_dblist_iterate3(dblist, func, 0, dblist->شمېر,
				      priv_معلومات);
}

ساكن EXT2_QSORT_TYPE dir_بلاک_cmp2(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ext2_db_entry2 *db_a =
		(ثابت بنية ext2_db_entry2 *) a;
	ثابت بنية ext2_db_entry2 *db_b =
		(ثابت بنية ext2_db_entry2 *) b;

	إذا (db_a->blk != db_b->blk)
		أرجع (صحيح) (db_a->blk - db_b->blk);

	إذا (db_a->ino != db_b->ino)
		أرجع (صحيح) (db_a->ino - db_b->ino);

	أرجع (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	أرجع dblist->شمېر;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  بنية ext2_db_entry2 **ثبت)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->شمېر == 0)
		أرجع EXT2_ET_DBLIST_تشه;

	إذا (ثبت)
		*ثبت = dblist->list + ( dblist->شمېر-1);
	أرجع 0;
}

errcode_t ext2fs_dblist_drop_وروستی(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->شمېر == 0)
		أرجع EXT2_ET_DBLIST_تشه;

	dblist->شمېر--;
	أرجع 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_جمع_کړه_dir_بلاک(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       صحيح blockcnt)
{
	أرجع ext2fs_جمع_کړه_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_سېټ_dir_بلاک(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       صحيح blockcnt)
{
	أرجع ext2fs_سېټ_dir_block2(dblist, ino, blk, blockcnt);
}

فراغ ext2fs_dblist_ترتیبول(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
						    ثابت فراغ *))
{
	إذا (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_بلاک_cmp;
	} وإلا
		sortfunc = dir_بلاک_cmp2;
	qsort(dblist->list, (size_t) dblist->شمېر,
	      حجم(بنية ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
بنية iterate_passthrough {
	صحيح (*func)(ext2_filsys fs,
		    بنية ext2_db_ثبت *db_info,
		    فراغ	*priv_معلومات);
	فراغ *priv_معلومات;
};

ساكن صحيح passthrough_func(ext2_filsys fs,
			    بنية ext2_db_entry2 *db_info,
			    فراغ	*priv_معلومات)
{
	بنية iterate_passthrough *p = priv_معلومات;
	بنية ext2_db_ثبت db;
	صحيح ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (صحيح) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_معلومات);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	أرجع ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				صحيح (*func)(ext2_filsys fs,
					    بنية ext2_db_ثبت *db_info,
					    فراغ	*priv_معلومات),
				فراغ *priv_معلومات)
{
	بنية iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_معلومات = priv_معلومات;

	أرجع ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

ساكن EXT2_QSORT_TYPE dir_بلاک_cmp(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ext2_db_entry2 *db_a =
		(ثابت بنية ext2_db_entry2 *) a;
	ثابت بنية ext2_db_entry2 *db_b =
		(ثابت بنية ext2_db_entry2 *) b;

	بنية ext2_db_ثبت a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	أرجع sortfunc32(&a32, &b32);
}

صحيح ext2fs_dblist_شمېر(ext2_dblist dblist)
{
	أرجع dblist->شمېر;
}

errcode_t ext2fs_dblist_get_وروستی(ext2_dblist dblist,
				 بنية ext2_db_ثبت **ثبت)
{
	ساكن بنية ext2_db_ثبت ret_ثبت;
	بنية ext2_db_entry2 *وروستی;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->شمېر == 0)
		أرجع EXT2_ET_DBLIST_تشه;

	إذا (!ثبت)
		أرجع 0;

	وروستی = dblist->list + dblist->شمېر -1;

	ret_ثبت.ino = وروستی->ino;
	ret_ثبت.blk = وروستی->blk;
	ret_ثبت.blockcnt = وروستی->blockcnt;
	*ثبت = &ret_ثبت;

	أرجع 0;
}

