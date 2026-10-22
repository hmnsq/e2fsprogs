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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸ್ಥಿರ EXT2_QSORT_TYPE dir_block_cmp(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b);
ಸ್ಥಿರ EXT2_QSORT_TYPE dir_block_cmp2(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b);
ಸ್ಥಿರ EXT2_QSORT_TYPE (*sortfunc32)(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
ಸ್ಥಿರ errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t ಎಣಿಕೆ,
			     ಸಂರಚನಾ ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = ರಿಕ್ತ;
	errcode_t	retval;
	ext2_ino_t	ಅಂಕೆ_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		ಪ್ರತ್ಯಾಗಮ 0;

	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_struct_dblist), &dblist);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	memset(dblist, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	ಯದಿ (size)
		dblist->size = size;
	ಅನ್ಯಥಾ {
		retval = ext2fs_get_ಅಂಕೆ_dirs(fs, &ಅಂಕೆ_dirs);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
		dblist->size = (ಅಂಕೆ_dirs * 2) + 12;
	}
	len = (size_t) ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_db_entry2) * dblist->size;
	dblist->ಎಣಿಕೆ = ಎಣಿಕೆ;
	retval = ext2fs_get_array(dblist->size, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_db_entry2),
		&dblist->list);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	ಯದಿ (list)
		memcpy(dblist->list, list, len);
	ಅನ್ಯಥಾ
		memset(dblist->list, 0, len);
	ಯದಿ (ret_dblist)
		*ret_dblist = dblist;
	ಅನ್ಯಥಾ
		fs->dblist = dblist;
	ಪ್ರತ್ಯಾಗಮ 0;
cleanup:
	ಯದಿ (dblist)
		ext2fs_free_mem(&dblist);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	dblist->sorted = 1;
	ಯದಿ (ret_dblist)
		*ret_dblist = dblist;
	ಅನ್ಯಥಾ
		fs->dblist = dblist;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->size, src->ಎಣಿಕೆ, src->list,
			     &dblist);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	ಪ್ರತ್ಯಾಗಮ 0;
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
	ಸಂರಚನಾ ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಯದಿ (dblist->ಎಣಿಕೆ >= dblist->size) {
		old_size = dblist->size * ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_db_entry2),
					   &dblist->list);
		ಯದಿ (retval) {
			dblist->size = old_size / ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_db_entry2);
			ಪ್ರತ್ಯಾಗಮ retval;
		}
	}
	new_entry = dblist->list + ( dblist->ಎಣಿಕೆ++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಪ್ರತಿ (i=0; i < dblist->ಎಣಿಕೆ; i++) {
		ಯದಿ ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			ಅನುವರ್ತ;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_DB_NOT_FOUND;
}

ಶೂನ್ಯ ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ಧ್ರುವ ಶೂನ್ಯ *,
						     ಧ್ರುವ ಶೂನ್ಯ *))
{
	ಯದಿ (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->ಎಣಿಕೆ,
	      ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
					     ಸಂರಚನಾ ext2_db_entry2 *db_info,
					     ಶೂನ್ಯ	*priv_data),
				 unsigned long long start,
				 unsigned long long ಎಣಿಕೆ,
				 ಶೂನ್ಯ *priv_data)
{
	unsigned long long	i, end;
	ಪೂರ್ಣಾಙ್ಕ		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + ಎಣಿಕೆ;
	ಯದಿ (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	ಯದಿ (end > dblist->ಎಣಿಕೆ)
		end = dblist->ಎಣಿಕೆ;
	ಪ್ರತಿ (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		ಯದಿ (ret & DBLIST_ABORT)
			ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
					     ಸಂರಚನಾ ext2_db_entry2 *db_info,
					     ಶೂನ್ಯ	*priv_data),
				 ಶೂನ್ಯ *priv_data)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_dblist_iterate3(dblist, func, 0, dblist->ಎಣಿಕೆ,
				      priv_data);
}

ಸ್ಥಿರ EXT2_QSORT_TYPE dir_block_cmp2(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
	ಧ್ರುವ ಸಂರಚನಾ ext2_db_entry2 *db_a =
		(ಧ್ರುವ ಸಂರಚನಾ ext2_db_entry2 *) a;
	ಧ್ರುವ ಸಂರಚನಾ ext2_db_entry2 *db_b =
		(ಧ್ರುವ ಸಂರಚನಾ ext2_db_entry2 *) b;

	ಯದಿ (db_a->blk != db_b->blk)
		ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) (db_a->blk - db_b->blk);

	ಯದಿ (db_a->ino != db_b->ino)
		ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) (db_a->ino - db_b->ino);

	ಪ್ರತ್ಯಾಗಮ (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	ಪ್ರತ್ಯಾಗಮ dblist->ಎಣಿಕೆ;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  ಸಂರಚನಾ ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಯದಿ (dblist->ಎಣಿಕೆ == 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DBLIST_EMPTY;

	ಯದಿ (entry)
		*entry = dblist->list + ( dblist->ಎಣಿಕೆ-1);
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಯದಿ (dblist->ಎಣಿಕೆ == 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DBLIST_EMPTY;

	dblist->ಎಣಿಕೆ--;
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       ಪೂರ್ಣಾಙ್ಕ blockcnt)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       ಪೂರ್ಣಾಙ್ಕ blockcnt)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

ಶೂನ್ಯ ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ಧ್ರುವ ಶೂನ್ಯ *,
						    ಧ್ರುವ ಶೂನ್ಯ *))
{
	ಯದಿ (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} ಅನ್ಯಥಾ
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->ಎಣಿಕೆ,
	      ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
ಸಂರಚನಾ iterate_passthrough {
	ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
		    ಸಂರಚನಾ ext2_db_entry *db_info,
		    ಶೂನ್ಯ	*priv_data);
	ಶೂನ್ಯ *priv_data;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ passthrough_func(ext2_filsys fs,
			    ಸಂರಚನಾ ext2_db_entry2 *db_info,
			    ಶೂನ್ಯ	*priv_data)
{
	ಸಂರಚನಾ iterate_passthrough *p = priv_data;
	ಸಂರಚನಾ ext2_db_entry db;
	ಪೂರ್ಣಾಙ್ಕ ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (ಪೂರ್ಣಾಙ್ಕ) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	ಪ್ರತ್ಯಾಗಮ ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
					    ಸಂರಚನಾ ext2_db_entry *db_info,
					    ಶೂನ್ಯ	*priv_data),
				ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	ಪ್ರತ್ಯಾಗಮ ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

ಸ್ಥಿರ EXT2_QSORT_TYPE dir_block_cmp(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
	ಧ್ರುವ ಸಂರಚನಾ ext2_db_entry2 *db_a =
		(ಧ್ರುವ ಸಂರಚನಾ ext2_db_entry2 *) a;
	ಧ್ರುವ ಸಂರಚನಾ ext2_db_entry2 *db_b =
		(ಧ್ರುವ ಸಂರಚನಾ ext2_db_entry2 *) b;

	ಸಂರಚನಾ ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	ಪ್ರತ್ಯಾಗಮ sortfunc32(&a32, &b32);
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_dblist_ಎಣಿಕೆ(ext2_dblist dblist)
{
	ಪ್ರತ್ಯಾಗಮ dblist->ಎಣಿಕೆ;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 ಸಂರಚನಾ ext2_db_entry **entry)
{
	ಸ್ಥಿರ ಸಂರಚನಾ ext2_db_entry ret_entry;
	ಸಂರಚನಾ ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಯದಿ (dblist->ಎಣಿಕೆ == 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DBLIST_EMPTY;

	ಯದಿ (!entry)
		ಪ್ರತ್ಯಾಗಮ 0;

	last = dblist->list + dblist->ಎಣಿಕೆ -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	ಪ್ರತ್ಯಾಗಮ 0;
}

