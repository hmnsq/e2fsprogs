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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

நிலையான EXT2_QSORT_TYPE dir_block_cmp(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b);
நிலையான EXT2_QSORT_TYPE dir_block_cmp2(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b);
நிலையான EXT2_QSORT_TYPE (*sortfunc32)(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
நிலையான errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t மொத்தம்,
			     கட்டமைப்பு ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = வெற்று;
	errcode_t	retval;
	ext2_ino_t	இலக்கம்_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		திரும்பு 0;

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_struct_dblist), &dblist);
	எனில் (retval)
		செல் cleanup;
	memset(dblist, 0, அளவு(கட்டமைப்பு ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	எனில் (size)
		dblist->size = size;
	இல்லையெனில் {
		retval = ext2fs_get_இலக்கம்_dirs(fs, &இலக்கம்_dirs);
		எனில் (retval)
			செல் cleanup;
		dblist->size = (இலக்கம்_dirs * 2) + 12;
	}
	len = (size_t) அளவு(கட்டமைப்பு ext2_db_entry2) * dblist->size;
	dblist->மொத்தம் = மொத்தம்;
	retval = ext2fs_get_array(dblist->size, அளவு(கட்டமைப்பு ext2_db_entry2),
		&dblist->list);
	எனில் (retval)
		செல் cleanup;

	எனில் (list)
		memcpy(dblist->list, list, len);
	இல்லையெனில்
		memset(dblist->list, 0, len);
	எனில் (ret_dblist)
		*ret_dblist = dblist;
	இல்லையெனில்
		fs->dblist = dblist;
	திரும்பு 0;
cleanup:
	எனில் (dblist)
		ext2fs_free_mem(&dblist);
	திரும்பு retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	எனில் (retval)
		திரும்பு retval;

	dblist->sorted = 1;
	எனில் (ret_dblist)
		*ret_dblist = dblist;
	இல்லையெனில்
		fs->dblist = dblist;

	திரும்பு 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->size, src->மொத்தம், src->list,
			     &dblist);
	எனில் (retval)
		திரும்பு retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	திரும்பு 0;
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
	கட்டமைப்பு ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	எனில் (dblist->மொத்தம் >= dblist->size) {
		old_size = dblist->size * அளவு(கட்டமைப்பு ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   அளவு(கட்டமைப்பு ext2_db_entry2),
					   &dblist->list);
		எனில் (retval) {
			dblist->size = old_size / அளவு(கட்டமைப்பு ext2_db_entry2);
			திரும்பு retval;
		}
	}
	new_entry = dblist->list + ( dblist->மொத்தம்++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	திரும்பு 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	க்கு (i=0; i < dblist->மொத்தம்; i++) {
		எனில் ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			தொடர்;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		திரும்பு 0;
	}
	திரும்பு EXT2_ET_DB_NOT_FOUND;
}

வெறுமை ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(மாறிலி வெறுமை *,
						     மாறிலி வெறுமை *))
{
	எனில் (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->மொத்தம்,
	      அளவு(கட்டமைப்பு ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 முழு (*func)(ext2_filsys fs,
					     கட்டமைப்பு ext2_db_entry2 *db_info,
					     வெறுமை	*priv_data),
				 unsigned long long start,
				 unsigned long long மொத்தம்,
				 வெறுமை *priv_data)
{
	unsigned long long	i, end;
	முழு		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + மொத்தம்;
	எனில் (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	எனில் (end > dblist->மொத்தம்)
		end = dblist->மொத்தம்;
	க்கு (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		எனில் (ret & DBLIST_ABORT)
			திரும்பு 0;
	}
	திரும்பு 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 முழு (*func)(ext2_filsys fs,
					     கட்டமைப்பு ext2_db_entry2 *db_info,
					     வெறுமை	*priv_data),
				 வெறுமை *priv_data)
{
	திரும்பு ext2fs_dblist_iterate3(dblist, func, 0, dblist->மொத்தம்,
				      priv_data);
}

நிலையான EXT2_QSORT_TYPE dir_block_cmp2(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
	மாறிலி கட்டமைப்பு ext2_db_entry2 *db_a =
		(மாறிலி கட்டமைப்பு ext2_db_entry2 *) a;
	மாறிலி கட்டமைப்பு ext2_db_entry2 *db_b =
		(மாறிலி கட்டமைப்பு ext2_db_entry2 *) b;

	எனில் (db_a->blk != db_b->blk)
		திரும்பு (முழு) (db_a->blk - db_b->blk);

	எனில் (db_a->ino != db_b->ino)
		திரும்பு (முழு) (db_a->ino - db_b->ino);

	திரும்பு (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	திரும்பு dblist->மொத்தம்;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  கட்டமைப்பு ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	எனில் (dblist->மொத்தம் == 0)
		திரும்பு EXT2_ET_DBLIST_EMPTY;

	எனில் (entry)
		*entry = dblist->list + ( dblist->மொத்தம்-1);
	திரும்பு 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	எனில் (dblist->மொத்தம் == 0)
		திரும்பு EXT2_ET_DBLIST_EMPTY;

	dblist->மொத்தம்--;
	திரும்பு 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       முழு blockcnt)
{
	திரும்பு ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       முழு blockcnt)
{
	திரும்பு ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

வெறுமை ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(மாறிலி வெறுமை *,
						    மாறிலி வெறுமை *))
{
	எனில் (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} இல்லையெனில்
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->மொத்தம்,
	      அளவு(கட்டமைப்பு ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
கட்டமைப்பு iterate_passthrough {
	முழு (*func)(ext2_filsys fs,
		    கட்டமைப்பு ext2_db_entry *db_info,
		    வெறுமை	*priv_data);
	வெறுமை *priv_data;
};

நிலையான முழு passthrough_func(ext2_filsys fs,
			    கட்டமைப்பு ext2_db_entry2 *db_info,
			    வெறுமை	*priv_data)
{
	கட்டமைப்பு iterate_passthrough *p = priv_data;
	கட்டமைப்பு ext2_db_entry db;
	முழு ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (முழு) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	திரும்பு ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				முழு (*func)(ext2_filsys fs,
					    கட்டமைப்பு ext2_db_entry *db_info,
					    வெறுமை	*priv_data),
				வெறுமை *priv_data)
{
	கட்டமைப்பு iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	திரும்பு ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

நிலையான EXT2_QSORT_TYPE dir_block_cmp(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
	மாறிலி கட்டமைப்பு ext2_db_entry2 *db_a =
		(மாறிலி கட்டமைப்பு ext2_db_entry2 *) a;
	மாறிலி கட்டமைப்பு ext2_db_entry2 *db_b =
		(மாறிலி கட்டமைப்பு ext2_db_entry2 *) b;

	கட்டமைப்பு ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	திரும்பு sortfunc32(&a32, &b32);
}

முழு ext2fs_dblist_மொத்தம்(ext2_dblist dblist)
{
	திரும்பு dblist->மொத்தம்;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 கட்டமைப்பு ext2_db_entry **entry)
{
	நிலையான கட்டமைப்பு ext2_db_entry ret_entry;
	கட்டமைப்பு ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	எனில் (dblist->மொத்தம் == 0)
		திரும்பு EXT2_ET_DBLIST_EMPTY;

	எனில் (!entry)
		திரும்பு 0;

	last = dblist->list + dblist->மொத்தம் -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	திரும்பு 0;
}

