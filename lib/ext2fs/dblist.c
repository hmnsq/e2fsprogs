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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"

સ્થિર EXT2_QSORT_TYPE dir_block_cmp(અચલ શૂન્ય *a, અચલ શૂન્ય *b);
સ્થિર EXT2_QSORT_TYPE dir_block_cmp2(અચલ શૂન્ય *a, અચલ શૂન્ય *b);
સ્થિર EXT2_QSORT_TYPE (*sortfunc32)(અચલ શૂન્ય *a, અચલ શૂન્ય *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
સ્થિર errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t ગણતરી,
			     સંરચના ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = ખાલી;
	errcode_t	retval;
	ext2_ino_t	અંક_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		પાછા 0;

	retval = ext2fs_get_mem(કદ(સંરચના ext2_struct_dblist), &dblist);
	જો (retval)
		જાઓ cleanup;
	memset(dblist, 0, કદ(સંરચના ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	જો (size)
		dblist->size = size;
	નહીંતર {
		retval = ext2fs_get_અંક_dirs(fs, &અંક_dirs);
		જો (retval)
			જાઓ cleanup;
		dblist->size = (અંક_dirs * 2) + 12;
	}
	len = (size_t) કદ(સંરચના ext2_db_entry2) * dblist->size;
	dblist->ગણતરી = ગણતરી;
	retval = ext2fs_get_array(dblist->size, કદ(સંરચના ext2_db_entry2),
		&dblist->list);
	જો (retval)
		જાઓ cleanup;

	જો (list)
		memcpy(dblist->list, list, len);
	નહીંતર
		memset(dblist->list, 0, len);
	જો (ret_dblist)
		*ret_dblist = dblist;
	નહીંતર
		fs->dblist = dblist;
	પાછા 0;
cleanup:
	જો (dblist)
		ext2fs_free_mem(&dblist);
	પાછા retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	જો (retval)
		પાછા retval;

	dblist->sorted = 1;
	જો (ret_dblist)
		*ret_dblist = dblist;
	નહીંતર
		fs->dblist = dblist;

	પાછા 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->size, src->ગણતરી, src->list,
			     &dblist);
	જો (retval)
		પાછા retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	પાછા 0;
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
	સંરચના ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	જો (dblist->ગણતરી >= dblist->size) {
		old_size = dblist->size * કદ(સંરચના ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   કદ(સંરચના ext2_db_entry2),
					   &dblist->list);
		જો (retval) {
			dblist->size = old_size / કદ(સંરચના ext2_db_entry2);
			પાછા retval;
		}
	}
	new_entry = dblist->list + ( dblist->ગણતરી++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	પાછા 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	માટે (i=0; i < dblist->ગણતરી; i++) {
		જો ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			ચાલુ;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		પાછા 0;
	}
	પાછા EXT2_ET_DB_NOT_FOUND;
}

શૂન્ય ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(અચલ શૂન્ય *,
						     અચલ શૂન્ય *))
{
	જો (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->ગણતરી,
	      કદ(સંરચના ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 પૂર્ણાંક (*func)(ext2_filsys fs,
					     સંરચના ext2_db_entry2 *db_info,
					     શૂન્ય	*priv_data),
				 unsigned long long start,
				 unsigned long long ગણતરી,
				 શૂન્ય *priv_data)
{
	unsigned long long	i, end;
	પૂર્ણાંક		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + ગણતરી;
	જો (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	જો (end > dblist->ગણતરી)
		end = dblist->ગણતરી;
	માટે (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		જો (ret & DBLIST_ABORT)
			પાછા 0;
	}
	પાછા 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 પૂર્ણાંક (*func)(ext2_filsys fs,
					     સંરચના ext2_db_entry2 *db_info,
					     શૂન્ય	*priv_data),
				 શૂન્ય *priv_data)
{
	પાછા ext2fs_dblist_iterate3(dblist, func, 0, dblist->ગણતરી,
				      priv_data);
}

સ્થિર EXT2_QSORT_TYPE dir_block_cmp2(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
	અચલ સંરચના ext2_db_entry2 *db_a =
		(અચલ સંરચના ext2_db_entry2 *) a;
	અચલ સંરચના ext2_db_entry2 *db_b =
		(અચલ સંરચના ext2_db_entry2 *) b;

	જો (db_a->blk != db_b->blk)
		પાછા (પૂર્ણાંક) (db_a->blk - db_b->blk);

	જો (db_a->ino != db_b->ino)
		પાછા (પૂર્ણાંક) (db_a->ino - db_b->ino);

	પાછા (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	પાછા dblist->ગણતરી;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  સંરચના ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	જો (dblist->ગણતરી == 0)
		પાછા EXT2_ET_DBLIST_EMPTY;

	જો (entry)
		*entry = dblist->list + ( dblist->ગણતરી-1);
	પાછા 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	જો (dblist->ગણતરી == 0)
		પાછા EXT2_ET_DBLIST_EMPTY;

	dblist->ગણતરી--;
	પાછા 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       પૂર્ણાંક blockcnt)
{
	પાછા ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       પૂર્ણાંક blockcnt)
{
	પાછા ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

શૂન્ય ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(અચલ શૂન્ય *,
						    અચલ શૂન્ય *))
{
	જો (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} નહીંતર
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->ગણતરી,
	      કદ(સંરચના ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
સંરચના iterate_passthrough {
	પૂર્ણાંક (*func)(ext2_filsys fs,
		    સંરચના ext2_db_entry *db_info,
		    શૂન્ય	*priv_data);
	શૂન્ય *priv_data;
};

સ્થિર પૂર્ણાંક passthrough_func(ext2_filsys fs,
			    સંરચના ext2_db_entry2 *db_info,
			    શૂન્ય	*priv_data)
{
	સંરચના iterate_passthrough *p = priv_data;
	સંરચના ext2_db_entry db;
	પૂર્ણાંક ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (પૂર્ણાંક) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	પાછા ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				પૂર્ણાંક (*func)(ext2_filsys fs,
					    સંરચના ext2_db_entry *db_info,
					    શૂન્ય	*priv_data),
				શૂન્ય *priv_data)
{
	સંરચના iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	પાછા ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

સ્થિર EXT2_QSORT_TYPE dir_block_cmp(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
	અચલ સંરચના ext2_db_entry2 *db_a =
		(અચલ સંરચના ext2_db_entry2 *) a;
	અચલ સંરચના ext2_db_entry2 *db_b =
		(અચલ સંરચના ext2_db_entry2 *) b;

	સંરચના ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	પાછા sortfunc32(&a32, &b32);
}

પૂર્ણાંક ext2fs_dblist_ગણતરી(ext2_dblist dblist)
{
	પાછા dblist->ગણતરી;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 સંરચના ext2_db_entry **entry)
{
	સ્થિર સંરચના ext2_db_entry ret_entry;
	સંરચના ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	જો (dblist->ગણતરી == 0)
		પાછા EXT2_ET_DBLIST_EMPTY;

	જો (!entry)
		પાછા 0;

	last = dblist->list + dblist->ગણતરી -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	પાછા 0;
}

