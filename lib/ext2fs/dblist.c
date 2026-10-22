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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਸਥਿਰ EXT2_QSORT_TYPE dir_ਖੰਡ_cmp(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b);
ਸਥਿਰ EXT2_QSORT_TYPE dir_ਖੰਡ_cmp2(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b);
ਸਥਿਰ EXT2_QSORT_TYPE (*sortfunc32)(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
ਸਥਿਰ errcode_t make_dblist(ext2_filsys fs, ext2_ino_t ਮਾਪ,
			     ext2_ino_t ਗਿਣਤੀ_2,
			     ਬਣਤਰ ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = ਨਿਰਾ;
	errcode_t	retval;
	ext2_ino_t	ਅੰਕ_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		ਵਾਪਸ 0;

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_struct_dblist), &dblist);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	memset(dblist, 0, ਆਕਾਰ(ਬਣਤਰ ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	ਜੇ (ਮਾਪ)
		dblist->ਮਾਪ = ਮਾਪ;
	ਨਹੀਂਤਾਂ {
		retval = ext2fs_get_ਅੰਕ_dirs(fs, &ਅੰਕ_dirs);
		ਜੇ (retval)
			ਜਾਓ cleanup;
		dblist->ਮਾਪ = (ਅੰਕ_dirs * 2) + 12;
	}
	len = (size_t) ਆਕਾਰ(ਬਣਤਰ ext2_db_entry2) * dblist->ਮਾਪ;
	dblist->ਗਿਣਤੀ_2 = ਗਿਣਤੀ_2;
	retval = ext2fs_get_array(dblist->ਮਾਪ, ਆਕਾਰ(ਬਣਤਰ ext2_db_entry2),
		&dblist->list);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	ਜੇ (list)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(dblist->list, list, len);
	ਨਹੀਂਤਾਂ
		memset(dblist->list, 0, len);
	ਜੇ (ret_dblist)
		*ret_dblist = dblist;
	ਨਹੀਂਤਾਂ
		fs->dblist = dblist;
	ਵਾਪਸ 0;
cleanup:
	ਜੇ (dblist)
		ext2fs_free_mem(&dblist);
	ਵਾਪਸ retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	dblist->sorted = 1;
	ਜੇ (ret_dblist)
		*ret_dblist = dblist;
	ਨਹੀਂਤਾਂ
		fs->dblist = dblist;

	ਵਾਪਸ 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->ਮਾਪ, src->ਗਿਣਤੀ_2, src->list,
			     &dblist);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	ਵਾਪਸ 0;
}

/*
 * Close a directory block list
 *
 * (moved to closefs.c)
 */


/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_ਜੋੜੋ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	ਬਣਤਰ ext2_db_entry2 	*new_ਇੰਦਰਾਜ;
	errcode_t		retval;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		old_ਮਾਪ;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ਜੇ (dblist->ਗਿਣਤੀ_2 >= dblist->ਮਾਪ) {
		old_ਮਾਪ = dblist->ਮਾਪ * ਆਕਾਰ(ਬਣਤਰ ext2_db_entry2);
		dblist->ਮਾਪ += dblist->ਮਾਪ > 200 ? dblist->ਮਾਪ / 2 : 100;
		retval = ext2fs_resize_mem(old_ਮਾਪ, (size_t) dblist->ਮਾਪ *
					   ਆਕਾਰ(ਬਣਤਰ ext2_db_entry2),
					   &dblist->list);
		ਜੇ (retval) {
			dblist->ਮਾਪ = old_ਮਾਪ / ਆਕਾਰ(ਬਣਤਰ ext2_db_entry2);
			ਵਾਪਸ retval;
		}
	}
	new_ਇੰਦਰਾਜ = dblist->list + ( dblist->ਗਿਣਤੀ_2++);
	new_ਇੰਦਰਾਜ->blk = blk;
	new_ਇੰਦਰਾਜ->ino = ino;
	new_ਇੰਦਰਾਜ->blockcnt = blockcnt;

	dblist->sorted = 0;

	ਵਾਪਸ 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_ਸੈੱਟ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ਲਈ (i=0; i < dblist->ਗਿਣਤੀ_2; i++) {
		ਜੇ ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			ਜਾਰੀ;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ EXT2_ET_DB_NOT_ਮਿਲਿਆ;
}

ਖਾਲੀ ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ਅਚਲ ਖਾਲੀ *,
						     ਅਚਲ ਖਾਲੀ *))
{
	ਜੇ (!sortfunc)
		sortfunc = dir_ਖੰਡ_cmp2;
	qsort(dblist->list, (size_t) dblist->ਗਿਣਤੀ_2,
	      ਆਕਾਰ(ਬਣਤਰ ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 ਪੂਰਨ (*func)(ext2_filsys fs,
					     ਬਣਤਰ ext2_db_entry2 *db_info,
					     ਖਾਲੀ	*priv_ਅੰਕੜੇ),
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਸ਼ੁਰੂ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2,
				 ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	i, ਸਿਰਾ;
	ਪੂਰਨ		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ਸਿਰਾ = ਸ਼ੁਰੂ + ਗਿਣਤੀ_2;
	ਜੇ (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	ਜੇ (ਸਿਰਾ > dblist->ਗਿਣਤੀ_2)
		ਸਿਰਾ = dblist->ਗਿਣਤੀ_2;
	ਲਈ (i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_ਅੰਕੜੇ);
		ਜੇ (ret & DBLIST_ABORT)
			ਵਾਪਸ 0;
	}
	ਵਾਪਸ 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 ਪੂਰਨ (*func)(ext2_filsys fs,
					     ਬਣਤਰ ext2_db_entry2 *db_info,
					     ਖਾਲੀ	*priv_ਅੰਕੜੇ),
				 ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਵਾਪਸ ext2fs_dblist_iterate3(dblist, func, 0, dblist->ਗਿਣਤੀ_2,
				      priv_ਅੰਕੜੇ);
}

ਸਥਿਰ EXT2_QSORT_TYPE dir_ਖੰਡ_cmp2(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
	ਅਚਲ ਬਣਤਰ ext2_db_entry2 *db_a =
		(ਅਚਲ ਬਣਤਰ ext2_db_entry2 *) a;
	ਅਚਲ ਬਣਤਰ ext2_db_entry2 *db_b =
		(ਅਚਲ ਬਣਤਰ ext2_db_entry2 *) b;

	ਜੇ (db_a->blk != db_b->blk)
		ਵਾਪਸ (ਪੂਰਨ) (db_a->blk - db_b->blk);

	ਜੇ (db_a->ino != db_b->ino)
		ਵਾਪਸ (ਪੂਰਨ) (db_a->ino - db_b->ino);

	ਵਾਪਸ (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	ਵਾਪਸ dblist->ਗਿਣਤੀ_2;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  ਬਣਤਰ ext2_db_entry2 **ਇੰਦਰਾਜ)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ਜੇ (dblist->ਗਿਣਤੀ_2 == 0)
		ਵਾਪਸ EXT2_ET_DBLIST_ਖਾਲੀ_ਥਾਵਾਂ;

	ਜੇ (ਇੰਦਰਾਜ)
		*ਇੰਦਰਾਜ = dblist->list + ( dblist->ਗਿਣਤੀ_2-1);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_dblist_drop_ਆਖਰੀ(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ਜੇ (dblist->ਗਿਣਤੀ_2 == 0)
		ਵਾਪਸ EXT2_ET_DBLIST_ਖਾਲੀ_ਥਾਵਾਂ;

	dblist->ਗਿਣਤੀ_2--;
	ਵਾਪਸ 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_ਜੋੜੋ_dir_ਖੰਡ(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       ਪੂਰਨ blockcnt)
{
	ਵਾਪਸ ext2fs_ਜੋੜੋ_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_ਸੈੱਟ_dir_ਖੰਡ(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       ਪੂਰਨ blockcnt)
{
	ਵਾਪਸ ext2fs_ਸੈੱਟ_dir_block2(dblist, ino, blk, blockcnt);
}

ਖਾਲੀ ext2fs_dblist_ਛਾਂਟੋ(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ਅਚਲ ਖਾਲੀ *,
						    ਅਚਲ ਖਾਲੀ *))
{
	ਜੇ (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_ਖੰਡ_cmp;
	} ਨਹੀਂਤਾਂ
		sortfunc = dir_ਖੰਡ_cmp2;
	qsort(dblist->list, (size_t) dblist->ਗਿਣਤੀ_2,
	      ਆਕਾਰ(ਬਣਤਰ ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
ਬਣਤਰ iterate_passthrough {
	ਪੂਰਨ (*func)(ext2_filsys fs,
		    ਬਣਤਰ ext2_db_ਇੰਦਰਾਜ *db_info,
		    ਖਾਲੀ	*priv_ਅੰਕੜੇ);
	ਖਾਲੀ *priv_ਅੰਕੜੇ;
};

ਸਥਿਰ ਪੂਰਨ passthrough_func(ext2_filsys fs,
			    ਬਣਤਰ ext2_db_entry2 *db_info,
			    ਖਾਲੀ	*priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ iterate_passthrough *p = priv_ਅੰਕੜੇ;
	ਬਣਤਰ ext2_db_ਇੰਦਰਾਜ db;
	ਪੂਰਨ ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (ਪੂਰਨ) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_ਅੰਕੜੇ);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	ਵਾਪਸ ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				ਪੂਰਨ (*func)(ext2_filsys fs,
					    ਬਣਤਰ ext2_db_ਇੰਦਰਾਜ *db_info,
					    ਖਾਲੀ	*priv_ਅੰਕੜੇ),
				ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_ਅੰਕੜੇ = priv_ਅੰਕੜੇ;

	ਵਾਪਸ ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

ਸਥਿਰ EXT2_QSORT_TYPE dir_ਖੰਡ_cmp(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
	ਅਚਲ ਬਣਤਰ ext2_db_entry2 *db_a =
		(ਅਚਲ ਬਣਤਰ ext2_db_entry2 *) a;
	ਅਚਲ ਬਣਤਰ ext2_db_entry2 *db_b =
		(ਅਚਲ ਬਣਤਰ ext2_db_entry2 *) b;

	ਬਣਤਰ ext2_db_ਇੰਦਰਾਜ a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	ਵਾਪਸ sortfunc32(&a32, &b32);
}

ਪੂਰਨ ext2fs_dblist_ਗਿਣਤੀ(ext2_dblist dblist)
{
	ਵਾਪਸ dblist->ਗਿਣਤੀ_2;
}

errcode_t ext2fs_dblist_get_ਆਖਰੀ(ext2_dblist dblist,
				 ਬਣਤਰ ext2_db_ਇੰਦਰਾਜ **ਇੰਦਰਾਜ)
{
	ਸਥਿਰ ਬਣਤਰ ext2_db_ਇੰਦਰਾਜ ret_ਇੰਦਰਾਜ;
	ਬਣਤਰ ext2_db_entry2 *ਆਖਰੀ;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ਜੇ (dblist->ਗਿਣਤੀ_2 == 0)
		ਵਾਪਸ EXT2_ET_DBLIST_ਖਾਲੀ_ਥਾਵਾਂ;

	ਜੇ (!ਇੰਦਰਾਜ)
		ਵਾਪਸ 0;

	ਆਖਰੀ = dblist->list + dblist->ਗਿਣਤੀ_2 -1;

	ret_ਇੰਦਰਾਜ.ino = ਆਖਰੀ->ino;
	ret_ਇੰਦਰਾਜ.blk = ਆਖਰੀ->blk;
	ret_ਇੰਦਰਾਜ.blockcnt = ਆਖਰੀ->blockcnt;
	*ਇੰਦਰਾਜ = &ret_ਇੰਦਰਾਜ;

	ਵਾਪਸ 0;
}

