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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <time.h>

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

স্থির EXT2_QSORT_TYPE dir_block_cmp(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b);
স্থির EXT2_QSORT_TYPE dir_block_cmp2(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b);
স্থির EXT2_QSORT_TYPE (*sortfunc32)(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
স্থির errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t গুনতি,
			     সংরচনা ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = রিক্ত;
	errcode_t	retval;
	ext2_ino_t	অঙ্ক_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		ঘূৰাই 0;

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা ext2_struct_dblist), &dblist);
	যদি (retval)
		গচ্ছ cleanup;
	memset(dblist, 0, পরিমাণ(সংরচনা ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	যদি (size)
		dblist->size = size;
	অন্যথা {
		retval = ext2fs_get_অঙ্ক_dirs(fs, &অঙ্ক_dirs);
		যদি (retval)
			গচ্ছ cleanup;
		dblist->size = (অঙ্ক_dirs * 2) + 12;
	}
	len = (size_t) পরিমাণ(সংরচনা ext2_db_entry2) * dblist->size;
	dblist->গুনতি = গুনতি;
	retval = ext2fs_get_array(dblist->size, পরিমাণ(সংরচনা ext2_db_entry2),
		&dblist->list);
	যদি (retval)
		গচ্ছ cleanup;

	যদি (list)
		memcpy(dblist->list, list, len);
	অন্যথা
		memset(dblist->list, 0, len);
	যদি (ret_dblist)
		*ret_dblist = dblist;
	অন্যথা
		fs->dblist = dblist;
	ঘূৰাই 0;
cleanup:
	যদি (dblist)
		ext2fs_free_mem(&dblist);
	ঘূৰাই retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	যদি (retval)
		ঘূৰাই retval;

	dblist->sorted = 1;
	যদি (ret_dblist)
		*ret_dblist = dblist;
	অন্যথা
		fs->dblist = dblist;

	ঘূৰাই 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->size, src->গুনতি, src->list,
			     &dblist);
	যদি (retval)
		ঘূৰাই retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	ঘূৰাই 0;
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
	সংরচনা ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	যদি (dblist->গুনতি >= dblist->size) {
		old_size = dblist->size * পরিমাণ(সংরচনা ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   পরিমাণ(সংরচনা ext2_db_entry2),
					   &dblist->list);
		যদি (retval) {
			dblist->size = old_size / পরিমাণ(সংরচনা ext2_db_entry2);
			ঘূৰাই retval;
		}
	}
	new_entry = dblist->list + ( dblist->গুনতি++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	ঘূৰাই 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	বাবে (i=0; i < dblist->গুনতি; i++) {
		যদি ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			অনুৱর্ত;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		ঘূৰাই 0;
	}
	ঘূৰাই EXT2_ET_DB_NOT_FOUND;
}

শূন্য ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ধ্রুৱ শূন্য *,
						     ধ্রুৱ শূন্য *))
{
	যদি (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->গুনতি,
	      পরিমাণ(সংরচনা ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 পূর্ণাঙ্ক (*func)(ext2_filsys fs,
					     সংরচনা ext2_db_entry2 *db_info,
					     শূন্য	*priv_data),
				 unsigned long long start,
				 unsigned long long গুনতি,
				 শূন্য *priv_data)
{
	unsigned long long	i, end;
	পূর্ণাঙ্ক		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + গুনতি;
	যদি (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	যদি (end > dblist->গুনতি)
		end = dblist->গুনতি;
	বাবে (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		যদি (ret & DBLIST_ABORT)
			ঘূৰাই 0;
	}
	ঘূৰাই 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 পূর্ণাঙ্ক (*func)(ext2_filsys fs,
					     সংরচনা ext2_db_entry2 *db_info,
					     শূন্য	*priv_data),
				 শূন্য *priv_data)
{
	ঘূৰাই ext2fs_dblist_iterate3(dblist, func, 0, dblist->গুনতি,
				      priv_data);
}

স্থির EXT2_QSORT_TYPE dir_block_cmp2(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
	ধ্রুৱ সংরচনা ext2_db_entry2 *db_a =
		(ধ্রুৱ সংরচনা ext2_db_entry2 *) a;
	ধ্রুৱ সংরচনা ext2_db_entry2 *db_b =
		(ধ্রুৱ সংরচনা ext2_db_entry2 *) b;

	যদি (db_a->blk != db_b->blk)
		ঘূৰাই (পূর্ণাঙ্ক) (db_a->blk - db_b->blk);

	যদি (db_a->ino != db_b->ino)
		ঘূৰাই (পূর্ণাঙ্ক) (db_a->ino - db_b->ino);

	ঘূৰাই (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	ঘূৰাই dblist->গুনতি;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  সংরচনা ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	যদি (dblist->গুনতি == 0)
		ঘূৰাই EXT2_ET_DBLIST_EMPTY;

	যদি (entry)
		*entry = dblist->list + ( dblist->গুনতি-1);
	ঘূৰাই 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	যদি (dblist->গুনতি == 0)
		ঘূৰাই EXT2_ET_DBLIST_EMPTY;

	dblist->গুনতি--;
	ঘূৰাই 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       পূর্ণাঙ্ক blockcnt)
{
	ঘূৰাই ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       পূর্ণাঙ্ক blockcnt)
{
	ঘূৰাই ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

শূন্য ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ধ্রুৱ শূন্য *,
						    ধ্রুৱ শূন্য *))
{
	যদি (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} অন্যথা
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->গুনতি,
	      পরিমাণ(সংরচনা ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
সংরচনা iterate_passthrough {
	পূর্ণাঙ্ক (*func)(ext2_filsys fs,
		    সংরচনা ext2_db_entry *db_info,
		    শূন্য	*priv_data);
	শূন্য *priv_data;
};

স্থির পূর্ণাঙ্ক passthrough_func(ext2_filsys fs,
			    সংরচনা ext2_db_entry2 *db_info,
			    শূন্য	*priv_data)
{
	সংরচনা iterate_passthrough *p = priv_data;
	সংরচনা ext2_db_entry db;
	পূর্ণাঙ্ক ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (পূর্ণাঙ্ক) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	ঘূৰাই ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				পূর্ণাঙ্ক (*func)(ext2_filsys fs,
					    সংরচনা ext2_db_entry *db_info,
					    শূন্য	*priv_data),
				শূন্য *priv_data)
{
	সংরচনা iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	ঘূৰাই ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

স্থির EXT2_QSORT_TYPE dir_block_cmp(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
	ধ্রুৱ সংরচনা ext2_db_entry2 *db_a =
		(ধ্রুৱ সংরচনা ext2_db_entry2 *) a;
	ধ্রুৱ সংরচনা ext2_db_entry2 *db_b =
		(ধ্রুৱ সংরচনা ext2_db_entry2 *) b;

	সংরচনা ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	ঘূৰাই sortfunc32(&a32, &b32);
}

পূর্ণাঙ্ক ext2fs_dblist_গুনতি(ext2_dblist dblist)
{
	ঘূৰাই dblist->গুনতি;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 সংরচনা ext2_db_entry **entry)
{
	স্থির সংরচনা ext2_db_entry ret_entry;
	সংরচনা ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	যদি (dblist->গুনতি == 0)
		ঘূৰাই EXT2_ET_DBLIST_EMPTY;

	যদি (!entry)
		ঘূৰাই 0;

	last = dblist->list + dblist->গুনতি -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	ঘূৰাই 0;
}

