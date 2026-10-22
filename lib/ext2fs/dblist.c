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

#include "config.h"
#include <stdio.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

static EXT2_QSORT_TYPE dir_block_cmp(const void *a, const void *b);
static EXT2_QSORT_TYPE dir_block_cmp2(const void *a, const void *b);
static EXT2_QSORT_TYPE (*sortfunc32)(const void *a, const void *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
static errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t count,
			     struct ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = NULL;
	errcode_t	retval;
	ext2_ino_t	num_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	nếu ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		trả_về 0;

	retval = ext2fs_get_mem(sizeof(struct ext2_struct_dblist), &dblist);
	nếu (retval)
		goto cleanup;
	memset(dblist, 0, sizeof(struct ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	nếu (size)
		dblist->size = size;
	không_thì {
		retval = ext2fs_get_num_dirs(fs, &num_dirs);
		nếu (retval)
			goto cleanup;
		dblist->size = (num_dirs * 2) + 12;
	}
	len = (size_t) sizeof(struct ext2_db_entry2) * dblist->size;
	dblist->count = count;
	retval = ext2fs_get_array(dblist->size, sizeof(struct ext2_db_entry2),
		&dblist->list);
	nếu (retval)
		goto cleanup;

	nếu (list)
		memcpy(dblist->list, list, len);
	không_thì
		memset(dblist->list, 0, len);
	nếu (ret_dblist)
		*ret_dblist = dblist;
	không_thì
		fs->dblist = dblist;
	trả_về 0;
cleanup:
	nếu (dblist)
		ext2fs_free_mem(&dblist);
	trả_về retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	nếu (retval)
		trả_về retval;

	dblist->sorted = 1;
	nếu (ret_dblist)
		*ret_dblist = dblist;
	không_thì
		fs->dblist = dblist;

	trả_về 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->size, src->count, src->list,
			     &dblist);
	nếu (retval)
		trả_về retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	trả_về 0;
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
	struct ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	nếu (dblist->count >= dblist->size) {
		old_size = dblist->size * sizeof(struct ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   sizeof(struct ext2_db_entry2),
					   &dblist->list);
		nếu (retval) {
			dblist->size = old_size / sizeof(struct ext2_db_entry2);
			trả_về retval;
		}
	}
	new_entry = dblist->list + ( dblist->count++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	trả_về 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	cho (i=0; i < dblist->count; i++) {
		nếu ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			continue;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		trả_về 0;
	}
	trả_về EXT2_ET_DB_NOT_FOUND;
}

void ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(const void *,
						     const void *))
{
	nếu (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->count,
	      sizeof(struct ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 số_nguyên (*func)(ext2_filsys fs,
					     struct ext2_db_entry2 *db_info,
					     void	*priv_data),
				 unsigned long long start,
				 unsigned long long count,
				 void *priv_data)
{
	unsigned long long	i, end;
	số_nguyên		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + count;
	nếu (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	nếu (end > dblist->count)
		end = dblist->count;
	cho (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		nếu (ret & DBLIST_ABORT)
			trả_về 0;
	}
	trả_về 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 số_nguyên (*func)(ext2_filsys fs,
					     struct ext2_db_entry2 *db_info,
					     void	*priv_data),
				 void *priv_data)
{
	trả_về ext2fs_dblist_iterate3(dblist, func, 0, dblist->count,
				      priv_data);
}

static EXT2_QSORT_TYPE dir_block_cmp2(const void *a, const void *b)
{
	const struct ext2_db_entry2 *db_a =
		(const struct ext2_db_entry2 *) a;
	const struct ext2_db_entry2 *db_b =
		(const struct ext2_db_entry2 *) b;

	nếu (db_a->blk != db_b->blk)
		trả_về (số_nguyên) (db_a->blk - db_b->blk);

	nếu (db_a->ino != db_b->ino)
		trả_về (số_nguyên) (db_a->ino - db_b->ino);

	trả_về (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	trả_về dblist->count;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  struct ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	nếu (dblist->count == 0)
		trả_về EXT2_ET_DBLIST_EMPTY;

	nếu (entry)
		*entry = dblist->list + ( dblist->count-1);
	trả_về 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	nếu (dblist->count == 0)
		trả_về EXT2_ET_DBLIST_EMPTY;

	dblist->count--;
	trả_về 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       số_nguyên blockcnt)
{
	trả_về ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       số_nguyên blockcnt)
{
	trả_về ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

void ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(const void *,
						    const void *))
{
	nếu (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} không_thì
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->count,
	      sizeof(struct ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
struct iterate_passthrough {
	số_nguyên (*func)(ext2_filsys fs,
		    struct ext2_db_entry *db_info,
		    void	*priv_data);
	void *priv_data;
};

static số_nguyên passthrough_func(ext2_filsys fs,
			    struct ext2_db_entry2 *db_info,
			    void	*priv_data)
{
	struct iterate_passthrough *p = priv_data;
	struct ext2_db_entry db;
	số_nguyên ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (số_nguyên) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	trả_về ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				số_nguyên (*func)(ext2_filsys fs,
					    struct ext2_db_entry *db_info,
					    void	*priv_data),
				void *priv_data)
{
	struct iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	trả_về ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

static EXT2_QSORT_TYPE dir_block_cmp(const void *a, const void *b)
{
	const struct ext2_db_entry2 *db_a =
		(const struct ext2_db_entry2 *) a;
	const struct ext2_db_entry2 *db_b =
		(const struct ext2_db_entry2 *) b;

	struct ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	trả_về sortfunc32(&a32, &b32);
}

số_nguyên ext2fs_dblist_count(ext2_dblist dblist)
{
	trả_về dblist->count;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 struct ext2_db_entry **entry)
{
	static struct ext2_db_entry ret_entry;
	struct ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	nếu (dblist->count == 0)
		trả_về EXT2_ET_DBLIST_EMPTY;

	nếu (!entry)
		trả_về 0;

	last = dblist->list + dblist->count -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	trả_về 0;
}

