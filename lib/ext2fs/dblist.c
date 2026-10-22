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
#جيڪڏهن_پورو
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

ساكن EXT2_QSORT_TYPE dir_بلاڪ_cmp(ثابت فراغ *a, ثابت فراغ *b);
ساكن EXT2_QSORT_TYPE dir_بلاڪ_cmp2(ثابت فراغ *a, ثابت فراغ *b);
ساكن EXT2_QSORT_TYPE (*sortfunc32)(ثابت فراغ *a, ثابت فراغ *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
ساكن errcode_t make_dblist(ext2_filsys fs, ext2_ino_t سائيز,
			     ext2_ino_t ڳڻپ,
			     بنية ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = عدم;
	errcode_t	retval;
	ext2_ino_t	انگ_dirs;
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
	إذا (سائيز)
		dblist->سائيز = سائيز;
	وإلا {
		retval = ext2fs_get_انگ_dirs(fs, &انگ_dirs);
		إذا (retval)
			اذهب cleanup;
		dblist->سائيز = (انگ_dirs * 2) + 12;
	}
	len = (size_t) حجم(بنية ext2_db_entry2) * dblist->سائيز;
	dblist->ڳڻپ = ڳڻپ;
	retval = ext2fs_get_array(dblist->سائيز, حجم(بنية ext2_db_entry2),
		&dblist->list);
	إذا (retval)
		اذهب cleanup;

	إذا (list)
		يادگيري_نقل(dblist->list, list, len);
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

	retval = make_dblist(src->fs, src->سائيز, src->ڳڻپ, src->list,
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
errcode_t ext2fs_جوڙيو_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	بنية ext2_db_entry2 	*new_داخلا;
	errcode_t		retval;
	بنانشان ڊگهو		old_سائيز;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->ڳڻپ >= dblist->سائيز) {
		old_سائيز = dblist->سائيز * حجم(بنية ext2_db_entry2);
		dblist->سائيز += dblist->سائيز > 200 ? dblist->سائيز / 2 : 100;
		retval = ext2fs_resize_mem(old_سائيز, (size_t) dblist->سائيز *
					   حجم(بنية ext2_db_entry2),
					   &dblist->list);
		إذا (retval) {
			dblist->سائيز = old_سائيز / حجم(بنية ext2_db_entry2);
			أرجع retval;
		}
	}
	new_داخلا = dblist->list + ( dblist->ڳڻپ++);
	new_داخلا->blk = blk;
	new_داخلا->ino = ino;
	new_داخلا->blockcnt = blockcnt;

	dblist->sorted = 0;

	أرجع 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_سيٽ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	لكل (i=0; i < dblist->ڳڻپ; i++) {
		إذا ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			استمر;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		أرجع 0;
	}
	أرجع EXT2_ET_DB_NOT_مليو;
}

فراغ ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
						     ثابت فراغ *))
{
	إذا (!sortfunc)
		sortfunc = dir_بلاڪ_cmp2;
	qsort(dblist->list, (size_t) dblist->ڳڻپ,
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
				 بنانشان ڊگهو ڊگهو شروعات,
				 بنانشان ڊگهو ڊگهو ڳڻپ,
				 فراغ *priv_معلومات)
{
	بنانشان ڊگهو ڊگهو	i, پڇاڙي;
	صحيح		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	پڇاڙي = شروعات + ڳڻپ;
	إذا (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	إذا (پڇاڙي > dblist->ڳڻپ)
		پڇاڙي = dblist->ڳڻپ;
	لكل (i = شروعات; i < پڇاڙي; i++) {
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
	أرجع ext2fs_dblist_iterate3(dblist, func, 0, dblist->ڳڻپ,
				      priv_معلومات);
}

ساكن EXT2_QSORT_TYPE dir_بلاڪ_cmp2(ثابت فراغ *a, ثابت فراغ *b)
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
	أرجع dblist->ڳڻپ;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  بنية ext2_db_entry2 **داخلا)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->ڳڻپ == 0)
		أرجع EXT2_ET_DBLIST_خالي_جايون;

	إذا (داخلا)
		*داخلا = dblist->list + ( dblist->ڳڻپ-1);
	أرجع 0;
}

errcode_t ext2fs_dblist_drop_آخري(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->ڳڻپ == 0)
		أرجع EXT2_ET_DBLIST_خالي_جايون;

	dblist->ڳڻپ--;
	أرجع 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_جوڙيو_dir_بلاڪ(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       صحيح blockcnt)
{
	أرجع ext2fs_جوڙيو_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_سيٽ_dir_بلاڪ(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       صحيح blockcnt)
{
	أرجع ext2fs_سيٽ_dir_block2(dblist, ino, blk, blockcnt);
}

فراغ ext2fs_dblist_ترتيب_ڏيو(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
						    ثابت فراغ *))
{
	إذا (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_بلاڪ_cmp;
	} وإلا
		sortfunc = dir_بلاڪ_cmp2;
	qsort(dblist->list, (size_t) dblist->ڳڻپ,
	      حجم(بنية ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
بنية iterate_passthrough {
	صحيح (*func)(ext2_filsys fs,
		    بنية ext2_db_داخلا *db_info,
		    فراغ	*priv_معلومات);
	فراغ *priv_معلومات;
};

ساكن صحيح passthrough_func(ext2_filsys fs,
			    بنية ext2_db_entry2 *db_info,
			    فراغ	*priv_معلومات)
{
	بنية iterate_passthrough *p = priv_معلومات;
	بنية ext2_db_داخلا db;
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
					    بنية ext2_db_داخلا *db_info,
					    فراغ	*priv_معلومات),
				فراغ *priv_معلومات)
{
	بنية iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_معلومات = priv_معلومات;

	أرجع ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

ساكن EXT2_QSORT_TYPE dir_بلاڪ_cmp(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ext2_db_entry2 *db_a =
		(ثابت بنية ext2_db_entry2 *) a;
	ثابت بنية ext2_db_entry2 *db_b =
		(ثابت بنية ext2_db_entry2 *) b;

	بنية ext2_db_داخلا a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	أرجع sortfunc32(&a32, &b32);
}

صحيح ext2fs_dblist_ڳڻپ(ext2_dblist dblist)
{
	أرجع dblist->ڳڻپ;
}

errcode_t ext2fs_dblist_get_آخري(ext2_dblist dblist,
				 بنية ext2_db_داخلا **داخلا)
{
	ساكن بنية ext2_db_داخلا ret_داخلا;
	بنية ext2_db_entry2 *آخري;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->ڳڻپ == 0)
		أرجع EXT2_ET_DBLIST_خالي_جايون;

	إذا (!داخلا)
		أرجع 0;

	آخري = dblist->list + dblist->ڳڻپ -1;

	ret_داخلا.ino = آخري->ino;
	ret_داخلا.blk = آخري->blk;
	ret_داخلا.blockcnt = آخري->blockcnt;
	*داخلا = &ret_داخلا;

	أرجع 0;
}

