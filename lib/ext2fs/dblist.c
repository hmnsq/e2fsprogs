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
#အကယ်၍ HAVE_UNISTD_H
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
			     sa_nracanaa ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = NULL;
	errcode_t	retval;
	ext2_ino_t	num_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	အကယ်၍ ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		ပြန်ပေး 0;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_struct_dblist), &dblist);
	အကယ်၍ (retval)
		goto cleanup;
	memset(dblist, 0, parimaa_na(sa_nracanaa ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	အကယ်၍ (size)
		dblist->size = size;
	မဟုတ်ပါက {
		retval = ext2fs_get_num_dirs(fs, &num_dirs);
		အကယ်၍ (retval)
			goto cleanup;
		dblist->size = (num_dirs * 2) + 12;
	}
	len = (size_t) parimaa_na(sa_nracanaa ext2_db_entry2) * dblist->size;
	dblist->count = count;
	retval = ext2fs_get_array(dblist->size, parimaa_na(sa_nracanaa ext2_db_entry2),
		&dblist->list);
	အကယ်၍ (retval)
		goto cleanup;

	အကယ်၍ (list)
		memcpy(dblist->list, list, len);
	မဟုတ်ပါက
		memset(dblist->list, 0, len);
	အကယ်၍ (ret_dblist)
		*ret_dblist = dblist;
	မဟုတ်ပါက
		fs->dblist = dblist;
	ပြန်ပေး 0;
cleanup:
	အကယ်၍ (dblist)
		ext2fs_free_mem(&dblist);
	ပြန်ပေး retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	အကယ်၍ (retval)
		ပြန်ပေး retval;

	dblist->sorted = 1;
	အကယ်၍ (ret_dblist)
		*ret_dblist = dblist;
	မဟုတ်ပါက
		fs->dblist = dblist;

	ပြန်ပေး 0;
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
	အကယ်၍ (retval)
		ပြန်ပေး retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	ပြန်ပေး 0;
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
	sa_nracanaa ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	အကယ်၍ (dblist->count >= dblist->size) {
		old_size = dblist->size * parimaa_na(sa_nracanaa ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   parimaa_na(sa_nracanaa ext2_db_entry2),
					   &dblist->list);
		အကယ်၍ (retval) {
			dblist->size = old_size / parimaa_na(sa_nracanaa ext2_db_entry2);
			ပြန်ပေး retval;
		}
	}
	new_entry = dblist->list + ( dblist->count++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	ပြန်ပေး 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	အတွက် (i=0; i < dblist->count; i++) {
		အကယ်၍ ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			continue;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		ပြန်ပေး 0;
	}
	ပြန်ပေး EXT2_ET_DB_NOT_FOUND;
}

void ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(const void *,
						     const void *))
{
	အကယ်၍ (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->count,
	      parimaa_na(sa_nracanaa ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 ကိန်းပြည့် (*func)(ext2_filsys fs,
					     sa_nracanaa ext2_db_entry2 *db_info,
					     void	*priv_data),
				 unsigned long long start,
				 unsigned long long count,
				 void *priv_data)
{
	unsigned long long	i, end;
	ကိန်းပြည့်		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + count;
	အကယ်၍ (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	အကယ်၍ (end > dblist->count)
		end = dblist->count;
	အတွက် (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		အကယ်၍ (ret & DBLIST_ABORT)
			ပြန်ပေး 0;
	}
	ပြန်ပေး 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 ကိန်းပြည့် (*func)(ext2_filsys fs,
					     sa_nracanaa ext2_db_entry2 *db_info,
					     void	*priv_data),
				 void *priv_data)
{
	ပြန်ပေး ext2fs_dblist_iterate3(dblist, func, 0, dblist->count,
				      priv_data);
}

static EXT2_QSORT_TYPE dir_block_cmp2(const void *a, const void *b)
{
	const sa_nracanaa ext2_db_entry2 *db_a =
		(const sa_nracanaa ext2_db_entry2 *) a;
	const sa_nracanaa ext2_db_entry2 *db_b =
		(const sa_nracanaa ext2_db_entry2 *) b;

	အကယ်၍ (db_a->blk != db_b->blk)
		ပြန်ပေး (ကိန်းပြည့်) (db_a->blk - db_b->blk);

	အကယ်၍ (db_a->ino != db_b->ino)
		ပြန်ပေး (ကိန်းပြည့်) (db_a->ino - db_b->ino);

	ပြန်ပေး (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	ပြန်ပေး dblist->count;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  sa_nracanaa ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	အကယ်၍ (dblist->count == 0)
		ပြန်ပေး EXT2_ET_DBLIST_EMPTY;

	အကယ်၍ (entry)
		*entry = dblist->list + ( dblist->count-1);
	ပြန်ပေး 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	အကယ်၍ (dblist->count == 0)
		ပြန်ပေး EXT2_ET_DBLIST_EMPTY;

	dblist->count--;
	ပြန်ပေး 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       ကိန်းပြည့် blockcnt)
{
	ပြန်ပေး ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       ကိန်းပြည့် blockcnt)
{
	ပြန်ပေး ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

void ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(const void *,
						    const void *))
{
	အကယ်၍ (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} မဟုတ်ပါက
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->count,
	      parimaa_na(sa_nracanaa ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
sa_nracanaa iterate_passthrough {
	ကိန်းပြည့် (*func)(ext2_filsys fs,
		    sa_nracanaa ext2_db_entry *db_info,
		    void	*priv_data);
	void *priv_data;
};

static ကိန်းပြည့် passthrough_func(ext2_filsys fs,
			    sa_nracanaa ext2_db_entry2 *db_info,
			    void	*priv_data)
{
	sa_nracanaa iterate_passthrough *p = priv_data;
	sa_nracanaa ext2_db_entry db;
	ကိန်းပြည့် ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (ကိန်းပြည့်) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	ပြန်ပေး ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				ကိန်းပြည့် (*func)(ext2_filsys fs,
					    sa_nracanaa ext2_db_entry *db_info,
					    void	*priv_data),
				void *priv_data)
{
	sa_nracanaa iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	ပြန်ပေး ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

static EXT2_QSORT_TYPE dir_block_cmp(const void *a, const void *b)
{
	const sa_nracanaa ext2_db_entry2 *db_a =
		(const sa_nracanaa ext2_db_entry2 *) a;
	const sa_nracanaa ext2_db_entry2 *db_b =
		(const sa_nracanaa ext2_db_entry2 *) b;

	sa_nracanaa ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	ပြန်ပေး sortfunc32(&a32, &b32);
}

ကိန်းပြည့် ext2fs_dblist_count(ext2_dblist dblist)
{
	ပြန်ပေး dblist->count;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 sa_nracanaa ext2_db_entry **entry)
{
	static sa_nracanaa ext2_db_entry ret_entry;
	sa_nracanaa ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	အကယ်၍ (dblist->count == 0)
		ပြန်ပေး EXT2_ET_DBLIST_EMPTY;

	အကယ်၍ (!entry)
		ပြန်ပေး 0;

	last = dblist->list + dblist->count -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	ပြန်ပေး 0;
}

