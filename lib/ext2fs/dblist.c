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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸ್ಥಿರ EXT2_QSORT_TYPE dir_ಖಂಡ_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b);
ಸ್ಥಿರ EXT2_QSORT_TYPE dir_ಖಂಡ_cmp2(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b);
ಸ್ಥಿರ EXT2_QSORT_TYPE (*sortfunc32)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
ಸ್ಥಿರ errcode_t make_dblist(ext2_filsys fs, ext2_ino_t ಗಾತ್ರ_2,
			     ext2_ino_t ಎಣಿಕೆ_2,
			     ರಚನೆ ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = ಖಾಲಿ;
	errcode_t	retval;
	ext2_ino_t	ಅಂಕೆ_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		ಹಿಂತಿರುಗಿ 0;

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_struct_dblist), &dblist);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	memset(dblist, 0, ಗಾತ್ರ(ರಚನೆ ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	ಆದರೆ (ಗಾತ್ರ_2)
		dblist->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
	ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_get_ಅಂಕೆ_dirs(fs, &ಅಂಕೆ_dirs);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
		dblist->ಗಾತ್ರ_2 = (ಅಂಕೆ_dirs * 2) + 12;
	}
	len = (size_t) ಗಾತ್ರ(ರಚನೆ ext2_db_entry2) * dblist->ಗಾತ್ರ_2;
	dblist->ಎಣಿಕೆ_2 = ಎಣಿಕೆ_2;
	retval = ext2fs_get_array(dblist->ಗಾತ್ರ_2, ಗಾತ್ರ(ರಚನೆ ext2_db_entry2),
		&dblist->list);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	ಆದರೆ (list)
		ಸ್ಮರಣೆ_ನಕಲು(dblist->list, list, len);
	ಇಲ್ಲದಿದ್ದರೆ
		memset(dblist->list, 0, len);
	ಆದರೆ (ret_dblist)
		*ret_dblist = dblist;
	ಇಲ್ಲದಿದ್ದರೆ
		fs->dblist = dblist;
	ಹಿಂತಿರುಗಿ 0;
cleanup:
	ಆದರೆ (dblist)
		ext2fs_free_mem(&dblist);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	dblist->sorted = 1;
	ಆದರೆ (ret_dblist)
		*ret_dblist = dblist;
	ಇಲ್ಲದಿದ್ದರೆ
		fs->dblist = dblist;

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->ಗಾತ್ರ_2, src->ಎಣಿಕೆ_2, src->list,
			     &dblist);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Close a directory block list
 *
 * (moved to closefs.c)
 */


/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_ಕೂಡಿಸು_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	ರಚನೆ ext2_db_entry2 	*new_ನಮೂದು;
	errcode_t		retval;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		old_ಗಾತ್ರ;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಆದರೆ (dblist->ಎಣಿಕೆ_2 >= dblist->ಗಾತ್ರ_2) {
		old_ಗಾತ್ರ = dblist->ಗಾತ್ರ_2 * ಗಾತ್ರ(ರಚನೆ ext2_db_entry2);
		dblist->ಗಾತ್ರ_2 += dblist->ಗಾತ್ರ_2 > 200 ? dblist->ಗಾತ್ರ_2 / 2 : 100;
		retval = ext2fs_resize_mem(old_ಗಾತ್ರ, (size_t) dblist->ಗಾತ್ರ_2 *
					   ಗಾತ್ರ(ರಚನೆ ext2_db_entry2),
					   &dblist->list);
		ಆದರೆ (retval) {
			dblist->ಗಾತ್ರ_2 = old_ಗಾತ್ರ / ಗಾತ್ರ(ರಚನೆ ext2_db_entry2);
			ಹಿಂತಿರುಗಿ retval;
		}
	}
	new_ನಮೂದು = dblist->list + ( dblist->ಎಣಿಕೆ_2++);
	new_ನಮೂದು->blk = blk;
	new_ನಮೂದು->ino = ino;
	new_ನಮೂದು->blockcnt = blockcnt;

	dblist->sorted = 0;

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_ಗಣ_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಗಾಗಿ (i=0; i < dblist->ಎಣಿಕೆ_2; i++) {
		ಆದರೆ ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			ಮುಂದುವರಿ;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ EXT2_ET_DB_NOT_ಸಿಕ್ಕಿತು;
}

ಶೂನ್ಯ ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *,
						     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *))
{
	ಆದರೆ (!sortfunc)
		sortfunc = dir_ಖಂಡ_cmp2;
	qsort(dblist->list, (size_t) dblist->ಎಣಿಕೆ_2,
	      ಗಾತ್ರ(ರಚನೆ ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
					     ರಚನೆ ext2_db_entry2 *db_info,
					     ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಆರಂಭ,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2,
				 ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	i, ಕೊನೆ;
	ಪೂರ್ಣಾಂಕ		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಕೊನೆ = ಆರಂಭ + ಎಣಿಕೆ_2;
	ಆದರೆ (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	ಆದರೆ (ಕೊನೆ > dblist->ಎಣಿಕೆ_2)
		ಕೊನೆ = dblist->ಎಣಿಕೆ_2;
	ಗಾಗಿ (i = ಆರಂಭ; i < ಕೊನೆ; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_ದತ್ತಾಂಶ);
		ಆದರೆ (ret & DBLIST_ABORT)
			ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
					     ರಚನೆ ext2_db_entry2 *db_info,
					     ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
				 ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ಹಿಂತಿರುಗಿ ext2fs_dblist_iterate3(dblist, func, 0, dblist->ಎಣಿಕೆ_2,
				      priv_ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ EXT2_QSORT_TYPE dir_ಖಂಡ_cmp2(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_db_entry2 *db_a =
		(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_db_entry2 *) a;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_db_entry2 *db_b =
		(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_db_entry2 *) b;

	ಆದರೆ (db_a->blk != db_b->blk)
		ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) (db_a->blk - db_b->blk);

	ಆದರೆ (db_a->ino != db_b->ino)
		ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) (db_a->ino - db_b->ino);

	ಹಿಂತಿರುಗಿ (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	ಹಿಂತಿರುಗಿ dblist->ಎಣಿಕೆ_2;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  ರಚನೆ ext2_db_entry2 **ನಮೂದು)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಆದರೆ (dblist->ಎಣಿಕೆ_2 == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_DBLIST_ಖಾಲಿಗಳು;

	ಆದರೆ (ನಮೂದು)
		*ನಮೂದು = dblist->list + ( dblist->ಎಣಿಕೆ_2-1);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_dblist_drop_ಕೊನೆಯ(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಆದರೆ (dblist->ಎಣಿಕೆ_2 == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_DBLIST_ಖಾಲಿಗಳು;

	dblist->ಎಣಿಕೆ_2--;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_ಕೂಡಿಸು_dir_ಖಂಡ(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       ಪೂರ್ಣಾಂಕ blockcnt)
{
	ಹಿಂತಿರುಗಿ ext2fs_ಕೂಡಿಸು_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_ಗಣ_dir_ಖಂಡ(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       ಪೂರ್ಣಾಂಕ blockcnt)
{
	ಹಿಂತಿರುಗಿ ext2fs_ಗಣ_dir_block2(dblist, ino, blk, blockcnt);
}

ಶೂನ್ಯ ext2fs_dblist_ವಿಂಗಡಿಸು(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *,
						    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *))
{
	ಆದರೆ (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_ಖಂಡ_cmp;
	} ಇಲ್ಲದಿದ್ದರೆ
		sortfunc = dir_ಖಂಡ_cmp2;
	qsort(dblist->list, (size_t) dblist->ಎಣಿಕೆ_2,
	      ಗಾತ್ರ(ರಚನೆ ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
ರಚನೆ iterate_passthrough {
	ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
		    ರಚನೆ ext2_db_ನಮೂದು *db_info,
		    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ);
	ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ passthrough_func(ext2_filsys fs,
			    ರಚನೆ ext2_db_entry2 *db_info,
			    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ iterate_passthrough *p = priv_ದತ್ತಾಂಶ;
	ರಚನೆ ext2_db_ನಮೂದು db;
	ಪೂರ್ಣಾಂಕ ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (ಪೂರ್ಣಾಂಕ) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_ದತ್ತಾಂಶ);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	ಹಿಂತಿರುಗಿ ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
					    ರಚನೆ ext2_db_ನಮೂದು *db_info,
					    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
				ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_ದತ್ತಾಂಶ = priv_ದತ್ತಾಂಶ;

	ಹಿಂತಿರುಗಿ ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

ಸ್ಥಿರ EXT2_QSORT_TYPE dir_ಖಂಡ_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_db_entry2 *db_a =
		(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_db_entry2 *) a;
	ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_db_entry2 *db_b =
		(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_db_entry2 *) b;

	ರಚನೆ ext2_db_ನಮೂದು a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	ಹಿಂತಿರುಗಿ sortfunc32(&a32, &b32);
}

ಪೂರ್ಣಾಂಕ ext2fs_dblist_ಎಣಿಕೆ(ext2_dblist dblist)
{
	ಹಿಂತಿರುಗಿ dblist->ಎಣಿಕೆ_2;
}

errcode_t ext2fs_dblist_get_ಕೊನೆಯ(ext2_dblist dblist,
				 ರಚನೆ ext2_db_ನಮೂದು **ನಮೂದು)
{
	ಸ್ಥಿರ ರಚನೆ ext2_db_ನಮೂದು ret_ನಮೂದು;
	ರಚನೆ ext2_db_entry2 *ಕೊನೆಯ;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ಆದರೆ (dblist->ಎಣಿಕೆ_2 == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_DBLIST_ಖಾಲಿಗಳು;

	ಆದರೆ (!ನಮೂದು)
		ಹಿಂತಿರುಗಿ 0;

	ಕೊನೆಯ = dblist->list + dblist->ಎಣಿಕೆ_2 -1;

	ret_ನಮೂದು.ino = ಕೊನೆಯ->ino;
	ret_ನಮೂದು.blk = ಕೊನೆಯ->blk;
	ret_ನಮೂದು.blockcnt = ಕೊನೆಯ->blockcnt;
	*ನಮೂದು = &ret_ನಮೂದು;

	ಹಿಂತಿರುಗಿ 0;
}

