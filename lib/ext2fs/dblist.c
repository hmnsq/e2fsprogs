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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

స్థిర EXT2_QSORT_TYPE dir_ఖండం_cmp(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b);
స్థిర EXT2_QSORT_TYPE dir_ఖండం_cmp2(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b);
స్థిర EXT2_QSORT_TYPE (*sortfunc32)(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
స్థిర errcode_t make_dblist(ext2_filsys fs, ext2_ino_t కొలత,
			     ext2_ino_t లెక్క,
			     నిర్మాణం ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = ఖాళీ;
	errcode_t	retval;
	ext2_ino_t	అంకె_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		తిరిగి 0;

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_struct_dblist), &dblist);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	memset(dblist, 0, పరిమాణం(నిర్మాణం ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	ఒకవేళ (కొలత)
		dblist->కొలత = కొలత;
	లేకపోతే {
		retval = ext2fs_get_అంకె_dirs(fs, &అంకె_dirs);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
		dblist->కొలత = (అంకె_dirs * 2) + 12;
	}
	len = (size_t) పరిమాణం(నిర్మాణం ext2_db_entry2) * dblist->కొలత;
	dblist->లెక్క = లెక్క;
	retval = ext2fs_get_array(dblist->కొలత, పరిమాణం(నిర్మాణం ext2_db_entry2),
		&dblist->list);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	ఒకవేళ (list)
		జ్ఞాపకం_నకలు(dblist->list, list, len);
	లేకపోతే
		memset(dblist->list, 0, len);
	ఒకవేళ (ret_dblist)
		*ret_dblist = dblist;
	లేకపోతే
		fs->dblist = dblist;
	తిరిగి 0;
cleanup:
	ఒకవేళ (dblist)
		ext2fs_free_mem(&dblist);
	తిరిగి retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	ఒకవేళ (retval)
		తిరిగి retval;

	dblist->sorted = 1;
	ఒకవేళ (ret_dblist)
		*ret_dblist = dblist;
	లేకపోతే
		fs->dblist = dblist;

	తిరిగి 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->కొలత, src->లెక్క, src->list,
			     &dblist);
	ఒకవేళ (retval)
		తిరిగి retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	తిరిగి 0;
}

/*
 * Close a directory block list
 *
 * (moved to closefs.c)
 */


/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_కూడు_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	నిర్మాణం ext2_db_entry2 	*new_నమోదు;
	errcode_t		retval;
	గుర్తులేని దీర్ఘ		old_కొలత;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ఒకవేళ (dblist->లెక్క >= dblist->కొలత) {
		old_కొలత = dblist->కొలత * పరిమాణం(నిర్మాణం ext2_db_entry2);
		dblist->కొలత += dblist->కొలత > 200 ? dblist->కొలత / 2 : 100;
		retval = ext2fs_resize_mem(old_కొలత, (size_t) dblist->కొలత *
					   పరిమాణం(నిర్మాణం ext2_db_entry2),
					   &dblist->list);
		ఒకవేళ (retval) {
			dblist->కొలత = old_కొలత / పరిమాణం(నిర్మాణం ext2_db_entry2);
			తిరిగి retval;
		}
	}
	new_నమోదు = dblist->list + ( dblist->లెక్క++);
	new_నమోదు->blk = blk;
	new_నమోదు->ino = ino;
	new_నమోదు->blockcnt = blockcnt;

	dblist->sorted = 0;

	తిరిగి 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_సమితి_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	కొరకు (i=0; i < dblist->లెక్క; i++) {
		ఒకవేళ ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			కొనసాగు;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		తిరిగి 0;
	}
	తిరిగి EXT2_ET_DB_NOT_దొరికింది;
}

శూన్య ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(స్థిరరాశి శూన్య *,
						     స్థిరరాశి శూన్య *))
{
	ఒకవేళ (!sortfunc)
		sortfunc = dir_ఖండం_cmp2;
	qsort(dblist->list, (size_t) dblist->లెక్క,
	      పరిమాణం(నిర్మాణం ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 పూర్ణాంకం (*func)(ext2_filsys fs,
					     నిర్మాణం ext2_db_entry2 *db_info,
					     శూన్య	*priv_దత్తాంశం),
				 గుర్తులేని దీర్ఘ దీర్ఘ ప్రారంభం,
				 గుర్తులేని దీర్ఘ దీర్ఘ లెక్క,
				 శూన్య *priv_దత్తాంశం)
{
	గుర్తులేని దీర్ఘ దీర్ఘ	i, చివర;
	పూర్ణాంకం		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	చివర = ప్రారంభం + లెక్క;
	ఒకవేళ (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	ఒకవేళ (చివర > dblist->లెక్క)
		చివర = dblist->లెక్క;
	కొరకు (i = ప్రారంభం; i < చివర; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_దత్తాంశం);
		ఒకవేళ (ret & DBLIST_ABORT)
			తిరిగి 0;
	}
	తిరిగి 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 పూర్ణాంకం (*func)(ext2_filsys fs,
					     నిర్మాణం ext2_db_entry2 *db_info,
					     శూన్య	*priv_దత్తాంశం),
				 శూన్య *priv_దత్తాంశం)
{
	తిరిగి ext2fs_dblist_iterate3(dblist, func, 0, dblist->లెక్క,
				      priv_దత్తాంశం);
}

స్థిర EXT2_QSORT_TYPE dir_ఖండం_cmp2(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
	స్థిరరాశి నిర్మాణం ext2_db_entry2 *db_a =
		(స్థిరరాశి నిర్మాణం ext2_db_entry2 *) a;
	స్థిరరాశి నిర్మాణం ext2_db_entry2 *db_b =
		(స్థిరరాశి నిర్మాణం ext2_db_entry2 *) b;

	ఒకవేళ (db_a->blk != db_b->blk)
		తిరిగి (పూర్ణాంకం) (db_a->blk - db_b->blk);

	ఒకవేళ (db_a->ino != db_b->ino)
		తిరిగి (పూర్ణాంకం) (db_a->ino - db_b->ino);

	తిరిగి (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	తిరిగి dblist->లెక్క;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  నిర్మాణం ext2_db_entry2 **నమోదు_2)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ఒకవేళ (dblist->లెక్క == 0)
		తిరిగి EXT2_ET_DBLIST_ఖాళీలు;

	ఒకవేళ (నమోదు_2)
		*నమోదు_2 = dblist->list + ( dblist->లెక్క-1);
	తిరిగి 0;
}

errcode_t ext2fs_dblist_drop_చివరి(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ఒకవేళ (dblist->లెక్క == 0)
		తిరిగి EXT2_ET_DBLIST_ఖాళీలు;

	dblist->లెక్క--;
	తిరిగి 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_కూడు_dir_ఖండం(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       పూర్ణాంకం blockcnt)
{
	తిరిగి ext2fs_కూడు_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_సమితి_dir_ఖండం(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       పూర్ణాంకం blockcnt)
{
	తిరిగి ext2fs_సమితి_dir_block2(dblist, ino, blk, blockcnt);
}

శూన్య ext2fs_dblist_క్రమీకరించు(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(స్థిరరాశి శూన్య *,
						    స్థిరరాశి శూన్య *))
{
	ఒకవేళ (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_ఖండం_cmp;
	} లేకపోతే
		sortfunc = dir_ఖండం_cmp2;
	qsort(dblist->list, (size_t) dblist->లెక్క,
	      పరిమాణం(నిర్మాణం ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
నిర్మాణం iterate_passthrough {
	పూర్ణాంకం (*func)(ext2_filsys fs,
		    నిర్మాణం ext2_db_నమోదు *db_info,
		    శూన్య	*priv_దత్తాంశం);
	శూన్య *priv_దత్తాంశం;
};

స్థిర పూర్ణాంకం passthrough_func(ext2_filsys fs,
			    నిర్మాణం ext2_db_entry2 *db_info,
			    శూన్య	*priv_దత్తాంశం)
{
	నిర్మాణం iterate_passthrough *p = priv_దత్తాంశం;
	నిర్మాణం ext2_db_నమోదు db;
	పూర్ణాంకం ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (పూర్ణాంకం) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_దత్తాంశం);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	తిరిగి ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				పూర్ణాంకం (*func)(ext2_filsys fs,
					    నిర్మాణం ext2_db_నమోదు *db_info,
					    శూన్య	*priv_దత్తాంశం),
				శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_దత్తాంశం = priv_దత్తాంశం;

	తిరిగి ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

స్థిర EXT2_QSORT_TYPE dir_ఖండం_cmp(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
	స్థిరరాశి నిర్మాణం ext2_db_entry2 *db_a =
		(స్థిరరాశి నిర్మాణం ext2_db_entry2 *) a;
	స్థిరరాశి నిర్మాణం ext2_db_entry2 *db_b =
		(స్థిరరాశి నిర్మాణం ext2_db_entry2 *) b;

	నిర్మాణం ext2_db_నమోదు a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	తిరిగి sortfunc32(&a32, &b32);
}

పూర్ణాంకం ext2fs_dblist_లెక్క(ext2_dblist dblist)
{
	తిరిగి dblist->లెక్క;
}

errcode_t ext2fs_dblist_get_చివరి(ext2_dblist dblist,
				 నిర్మాణం ext2_db_నమోదు **నమోదు_2)
{
	స్థిర నిర్మాణం ext2_db_నమోదు ret_నమోదు;
	నిర్మాణం ext2_db_entry2 *చివరి;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ఒకవేళ (dblist->లెక్క == 0)
		తిరిగి EXT2_ET_DBLIST_ఖాళీలు;

	ఒకవేళ (!నమోదు_2)
		తిరిగి 0;

	చివరి = dblist->list + dblist->లెక్క -1;

	ret_నమోదు.ino = చివరి->ino;
	ret_నమోదు.blk = చివరి->blk;
	ret_నమోదు.blockcnt = చివరి->blockcnt;
	*నమోదు_2 = &ret_నమోదు;

	తిరిగి 0;
}

