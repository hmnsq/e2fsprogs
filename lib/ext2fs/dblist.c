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
#endif
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

ساكن EXT2_QSORT_TYPE dir_block_cmp(ثابت فراغ *a, ثابت فراغ *b);
ساكن EXT2_QSORT_TYPE dir_block_cmp2(ثابت فراغ *a, ثابت فراغ *b);
ساكن EXT2_QSORT_TYPE (*sortfunc32)(ثابت فراغ *a, ثابت فراغ *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
ساكن errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t کيراءن,
			     بنية ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = عدم;
	errcode_t	retval;
	ext2_ino_t	اڠک_dirs;
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
	إذا (size)
		dblist->size = size;
	وإلا {
		retval = ext2fs_get_اڠک_dirs(fs, &اڠک_dirs);
		إذا (retval)
			اذهب cleanup;
		dblist->size = (اڠک_dirs * 2) + 12;
	}
	len = (size_t) حجم(بنية ext2_db_entry2) * dblist->size;
	dblist->کيراءن = کيراءن;
	retval = ext2fs_get_array(dblist->size, حجم(بنية ext2_db_entry2),
		&dblist->list);
	إذا (retval)
		اذهب cleanup;

	إذا (list)
		memcpy(dblist->list, list, len);
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

	retval = make_dblist(src->fs, src->size, src->کيراءن, src->list,
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
errcode_t ext2fs_add_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	بنية ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->کيراءن >= dblist->size) {
		old_size = dblist->size * حجم(بنية ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   حجم(بنية ext2_db_entry2),
					   &dblist->list);
		إذا (retval) {
			dblist->size = old_size / حجم(بنية ext2_db_entry2);
			أرجع retval;
		}
	}
	new_entry = dblist->list + ( dblist->کيراءن++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	أرجع 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	لكل (i=0; i < dblist->کيراءن; i++) {
		إذا ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			استمر;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		أرجع 0;
	}
	أرجع EXT2_ET_DB_NOT_FOUND;
}

فراغ ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
						     ثابت فراغ *))
{
	إذا (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->کيراءن,
	      حجم(بنية ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 صحيح (*func)(ext2_filsys fs,
					     بنية ext2_db_entry2 *db_info,
					     فراغ	*priv_data),
				 unsigned long long start,
				 unsigned long long کيراءن,
				 فراغ *priv_data)
{
	unsigned long long	i, end;
	صحيح		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + کيراءن;
	إذا (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	إذا (end > dblist->کيراءن)
		end = dblist->کيراءن;
	لكل (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		إذا (ret & DBLIST_ABORT)
			أرجع 0;
	}
	أرجع 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 صحيح (*func)(ext2_filsys fs,
					     بنية ext2_db_entry2 *db_info,
					     فراغ	*priv_data),
				 فراغ *priv_data)
{
	أرجع ext2fs_dblist_iterate3(dblist, func, 0, dblist->کيراءن,
				      priv_data);
}

ساكن EXT2_QSORT_TYPE dir_block_cmp2(ثابت فراغ *a, ثابت فراغ *b)
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
	أرجع dblist->کيراءن;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  بنية ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->کيراءن == 0)
		أرجع EXT2_ET_DBLIST_EMPTY;

	إذا (entry)
		*entry = dblist->list + ( dblist->کيراءن-1);
	أرجع 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->کيراءن == 0)
		أرجع EXT2_ET_DBLIST_EMPTY;

	dblist->کيراءن--;
	أرجع 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       صحيح blockcnt)
{
	أرجع ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       صحيح blockcnt)
{
	أرجع ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

فراغ ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ثابت فراغ *,
						    ثابت فراغ *))
{
	إذا (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} وإلا
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->کيراءن,
	      حجم(بنية ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
بنية iterate_passthrough {
	صحيح (*func)(ext2_filsys fs,
		    بنية ext2_db_entry *db_info,
		    فراغ	*priv_data);
	فراغ *priv_data;
};

ساكن صحيح passthrough_func(ext2_filsys fs,
			    بنية ext2_db_entry2 *db_info,
			    فراغ	*priv_data)
{
	بنية iterate_passthrough *p = priv_data;
	بنية ext2_db_entry db;
	صحيح ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (صحيح) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	أرجع ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				صحيح (*func)(ext2_filsys fs,
					    بنية ext2_db_entry *db_info,
					    فراغ	*priv_data),
				فراغ *priv_data)
{
	بنية iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	أرجع ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

ساكن EXT2_QSORT_TYPE dir_block_cmp(ثابت فراغ *a, ثابت فراغ *b)
{
	ثابت بنية ext2_db_entry2 *db_a =
		(ثابت بنية ext2_db_entry2 *) a;
	ثابت بنية ext2_db_entry2 *db_b =
		(ثابت بنية ext2_db_entry2 *) b;

	بنية ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	أرجع sortfunc32(&a32, &b32);
}

صحيح ext2fs_dblist_کيراءن(ext2_dblist dblist)
{
	أرجع dblist->کيراءن;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 بنية ext2_db_entry **entry)
{
	ساكن بنية ext2_db_entry ret_entry;
	بنية ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	إذا (dblist->کيراءن == 0)
		أرجع EXT2_ET_DBLIST_EMPTY;

	إذا (!entry)
		أرجع 0;

	last = dblist->list + dblist->کيراءن -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	أرجع 0;
}

