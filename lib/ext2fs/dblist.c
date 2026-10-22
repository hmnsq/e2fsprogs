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
#если HAVE_UNISTD_H
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

	если ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		вернуть 0;

	retval = ext2fs_get_mem(sizeof(struct ext2_struct_dblist), &dblist);
	если (retval)
		goto cleanup;
	memset(dblist, 0, sizeof(struct ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	если (size)
		dblist->size = size;
	иначе {
		retval = ext2fs_get_num_dirs(fs, &num_dirs);
		если (retval)
			goto cleanup;
		dblist->size = (num_dirs * 2) + 12;
	}
	len = (size_t) sizeof(struct ext2_db_entry2) * dblist->size;
	dblist->count = count;
	retval = ext2fs_get_array(dblist->size, sizeof(struct ext2_db_entry2),
		&dblist->list);
	если (retval)
		goto cleanup;

	если (list)
		memcpy(dblist->list, list, len);
	иначе
		memset(dblist->list, 0, len);
	если (ret_dblist)
		*ret_dblist = dblist;
	иначе
		fs->dblist = dblist;
	вернуть 0;
cleanup:
	если (dblist)
		ext2fs_free_mem(&dblist);
	вернуть retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	если (retval)
		вернуть retval;

	dblist->sorted = 1;
	если (ret_dblist)
		*ret_dblist = dblist;
	иначе
		fs->dblist = dblist;

	вернуть 0;
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
	если (retval)
		вернуть retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	вернуть 0;
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

	если (dblist->count >= dblist->size) {
		old_size = dblist->size * sizeof(struct ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   sizeof(struct ext2_db_entry2),
					   &dblist->list);
		если (retval) {
			dblist->size = old_size / sizeof(struct ext2_db_entry2);
			вернуть retval;
		}
	}
	new_entry = dblist->list + ( dblist->count++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	вернуть 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	для (i=0; i < dblist->count; i++) {
		если ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			continue;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		вернуть 0;
	}
	вернуть EXT2_ET_DB_NOT_FOUND;
}

void ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(const void *,
						     const void *))
{
	если (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->count,
	      sizeof(struct ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 целое (*func)(ext2_filsys fs,
					     struct ext2_db_entry2 *db_info,
					     void	*priv_data),
				 unsigned long long start,
				 unsigned long long count,
				 void *priv_data)
{
	unsigned long long	i, end;
	целое		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + count;
	если (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	если (end > dblist->count)
		end = dblist->count;
	для (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		если (ret & DBLIST_ABORT)
			вернуть 0;
	}
	вернуть 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 целое (*func)(ext2_filsys fs,
					     struct ext2_db_entry2 *db_info,
					     void	*priv_data),
				 void *priv_data)
{
	вернуть ext2fs_dblist_iterate3(dblist, func, 0, dblist->count,
				      priv_data);
}

static EXT2_QSORT_TYPE dir_block_cmp2(const void *a, const void *b)
{
	const struct ext2_db_entry2 *db_a =
		(const struct ext2_db_entry2 *) a;
	const struct ext2_db_entry2 *db_b =
		(const struct ext2_db_entry2 *) b;

	если (db_a->blk != db_b->blk)
		вернуть (целое) (db_a->blk - db_b->blk);

	если (db_a->ino != db_b->ino)
		вернуть (целое) (db_a->ino - db_b->ino);

	вернуть (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	вернуть dblist->count;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  struct ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	если (dblist->count == 0)
		вернуть EXT2_ET_DBLIST_EMPTY;

	если (entry)
		*entry = dblist->list + ( dblist->count-1);
	вернуть 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	если (dblist->count == 0)
		вернуть EXT2_ET_DBLIST_EMPTY;

	dblist->count--;
	вернуть 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       целое blockcnt)
{
	вернуть ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       целое blockcnt)
{
	вернуть ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

void ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(const void *,
						    const void *))
{
	если (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} иначе
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->count,
	      sizeof(struct ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
struct iterate_passthrough {
	целое (*func)(ext2_filsys fs,
		    struct ext2_db_entry *db_info,
		    void	*priv_data);
	void *priv_data;
};

static целое passthrough_func(ext2_filsys fs,
			    struct ext2_db_entry2 *db_info,
			    void	*priv_data)
{
	struct iterate_passthrough *p = priv_data;
	struct ext2_db_entry db;
	целое ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (целое) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	вернуть ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				целое (*func)(ext2_filsys fs,
					    struct ext2_db_entry *db_info,
					    void	*priv_data),
				void *priv_data)
{
	struct iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	вернуть ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
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

	вернуть sortfunc32(&a32, &b32);
}

целое ext2fs_dblist_count(ext2_dblist dblist)
{
	вернуть dblist->count;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 struct ext2_db_entry **entry)
{
	static struct ext2_db_entry ret_entry;
	struct ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	если (dblist->count == 0)
		вернуть EXT2_ET_DBLIST_EMPTY;

	если (!entry)
		вернуть 0;

	last = dblist->list + dblist->count -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	вернуть 0;
}

