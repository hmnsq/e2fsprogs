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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

സ്ഥിതം EXT2_QSORT_TYPE dir_block_cmp(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b);
സ്ഥിതം EXT2_QSORT_TYPE dir_block_cmp2(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b);
സ്ഥിതം EXT2_QSORT_TYPE (*sortfunc32)(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
സ്ഥിതം errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t എണ്ണം,
			     ഘടന ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = ശൂന്യ;
	errcode_t	retval;
	ext2_ino_t	അക്കം_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		തിരികെ 0;

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_struct_dblist), &dblist);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	memset(dblist, 0, വലുപ്പം(ഘടന ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	എങ്കിൽ (size)
		dblist->size = size;
	അല്ലെങ്കിൽ {
		retval = ext2fs_get_അക്കം_dirs(fs, &അക്കം_dirs);
		എങ്കിൽ (retval)
			പോകുക cleanup;
		dblist->size = (അക്കം_dirs * 2) + 12;
	}
	len = (size_t) വലുപ്പം(ഘടന ext2_db_entry2) * dblist->size;
	dblist->എണ്ണം = എണ്ണം;
	retval = ext2fs_get_array(dblist->size, വലുപ്പം(ഘടന ext2_db_entry2),
		&dblist->list);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	എങ്കിൽ (list)
		memcpy(dblist->list, list, len);
	അല്ലെങ്കിൽ
		memset(dblist->list, 0, len);
	എങ്കിൽ (ret_dblist)
		*ret_dblist = dblist;
	അല്ലെങ്കിൽ
		fs->dblist = dblist;
	തിരികെ 0;
cleanup:
	എങ്കിൽ (dblist)
		ext2fs_free_mem(&dblist);
	തിരികെ retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	എങ്കിൽ (retval)
		തിരികെ retval;

	dblist->sorted = 1;
	എങ്കിൽ (ret_dblist)
		*ret_dblist = dblist;
	അല്ലെങ്കിൽ
		fs->dblist = dblist;

	തിരികെ 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->size, src->എണ്ണം, src->list,
			     &dblist);
	എങ്കിൽ (retval)
		തിരികെ retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	തിരികെ 0;
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
	ഘടന ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	എങ്കിൽ (dblist->എണ്ണം >= dblist->size) {
		old_size = dblist->size * വലുപ്പം(ഘടന ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   വലുപ്പം(ഘടന ext2_db_entry2),
					   &dblist->list);
		എങ്കിൽ (retval) {
			dblist->size = old_size / വലുപ്പം(ഘടന ext2_db_entry2);
			തിരികെ retval;
		}
	}
	new_entry = dblist->list + ( dblist->എണ്ണം++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	തിരികെ 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	വേണ്ടി (i=0; i < dblist->എണ്ണം; i++) {
		എങ്കിൽ ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			തുടരുക;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		തിരികെ 0;
	}
	തിരികെ EXT2_ET_DB_NOT_FOUND;
}

ശൂന്യം ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(സ്ഥിരം ശൂന്യം *,
						     സ്ഥിരം ശൂന്യം *))
{
	എങ്കിൽ (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->എണ്ണം,
	      വലുപ്പം(ഘടന ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
					     ഘടന ext2_db_entry2 *db_info,
					     ശൂന്യം	*priv_data),
				 unsigned long long start,
				 unsigned long long എണ്ണം,
				 ശൂന്യം *priv_data)
{
	unsigned long long	i, end;
	പൂർണ്ണസംഖ്യ		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + എണ്ണം;
	എങ്കിൽ (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	എങ്കിൽ (end > dblist->എണ്ണം)
		end = dblist->എണ്ണം;
	വേണ്ടി (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		എങ്കിൽ (ret & DBLIST_ABORT)
			തിരികെ 0;
	}
	തിരികെ 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
					     ഘടന ext2_db_entry2 *db_info,
					     ശൂന്യം	*priv_data),
				 ശൂന്യം *priv_data)
{
	തിരികെ ext2fs_dblist_iterate3(dblist, func, 0, dblist->എണ്ണം,
				      priv_data);
}

സ്ഥിതം EXT2_QSORT_TYPE dir_block_cmp2(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
	സ്ഥിരം ഘടന ext2_db_entry2 *db_a =
		(സ്ഥിരം ഘടന ext2_db_entry2 *) a;
	സ്ഥിരം ഘടന ext2_db_entry2 *db_b =
		(സ്ഥിരം ഘടന ext2_db_entry2 *) b;

	എങ്കിൽ (db_a->blk != db_b->blk)
		തിരികെ (പൂർണ്ണസംഖ്യ) (db_a->blk - db_b->blk);

	എങ്കിൽ (db_a->ino != db_b->ino)
		തിരികെ (പൂർണ്ണസംഖ്യ) (db_a->ino - db_b->ino);

	തിരികെ (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	തിരികെ dblist->എണ്ണം;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  ഘടന ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	എങ്കിൽ (dblist->എണ്ണം == 0)
		തിരികെ EXT2_ET_DBLIST_EMPTY;

	എങ്കിൽ (entry)
		*entry = dblist->list + ( dblist->എണ്ണം-1);
	തിരികെ 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	എങ്കിൽ (dblist->എണ്ണം == 0)
		തിരികെ EXT2_ET_DBLIST_EMPTY;

	dblist->എണ്ണം--;
	തിരികെ 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       പൂർണ്ണസംഖ്യ blockcnt)
{
	തിരികെ ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       പൂർണ്ണസംഖ്യ blockcnt)
{
	തിരികെ ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

ശൂന്യം ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(സ്ഥിരം ശൂന്യം *,
						    സ്ഥിരം ശൂന്യം *))
{
	എങ്കിൽ (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} അല്ലെങ്കിൽ
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->എണ്ണം,
	      വലുപ്പം(ഘടന ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
ഘടന iterate_passthrough {
	പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
		    ഘടന ext2_db_entry *db_info,
		    ശൂന്യം	*priv_data);
	ശൂന്യം *priv_data;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ passthrough_func(ext2_filsys fs,
			    ഘടന ext2_db_entry2 *db_info,
			    ശൂന്യം	*priv_data)
{
	ഘടന iterate_passthrough *p = priv_data;
	ഘടന ext2_db_entry db;
	പൂർണ്ണസംഖ്യ ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (പൂർണ്ണസംഖ്യ) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	തിരികെ ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
					    ഘടന ext2_db_entry *db_info,
					    ശൂന്യം	*priv_data),
				ശൂന്യം *priv_data)
{
	ഘടന iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	തിരികെ ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

സ്ഥിതം EXT2_QSORT_TYPE dir_block_cmp(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
	സ്ഥിരം ഘടന ext2_db_entry2 *db_a =
		(സ്ഥിരം ഘടന ext2_db_entry2 *) a;
	സ്ഥിരം ഘടന ext2_db_entry2 *db_b =
		(സ്ഥിരം ഘടന ext2_db_entry2 *) b;

	ഘടന ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	തിരികെ sortfunc32(&a32, &b32);
}

പൂർണ്ണസംഖ്യ ext2fs_dblist_എണ്ണം(ext2_dblist dblist)
{
	തിരികെ dblist->എണ്ണം;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 ഘടന ext2_db_entry **entry)
{
	സ്ഥിതം ഘടന ext2_db_entry ret_entry;
	ഘടന ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	എങ്കിൽ (dblist->എണ്ണം == 0)
		തിരികെ EXT2_ET_DBLIST_EMPTY;

	എങ്കിൽ (!entry)
		തിരികെ 0;

	last = dblist->list + dblist->എണ്ണം -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	തിരികെ 0;
}

