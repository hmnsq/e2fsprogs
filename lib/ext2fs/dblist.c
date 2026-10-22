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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସ୍ଥିର EXT2_QSORT_TYPE dir_block_cmp(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b);
ସ୍ଥିର EXT2_QSORT_TYPE dir_block_cmp2(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b);
ସ୍ଥିର EXT2_QSORT_TYPE (*sortfunc32)(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
ସ୍ଥିର errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t ଗଣନା_2,
			     ସଂରଚନା ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = ଖାଲି;
	errcode_t	retval;
	ext2_ino_t	ଅଙ୍କ_dirs;
	size_t		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		ଫେରନ୍ତୁ 0;

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_struct_dblist), &dblist);
	ଯଦି (retval)
		ଯାଅ cleanup;
	memset(dblist, 0, ଆକାର(ସଂରଚନା ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	ଯଦି (size)
		dblist->size = size;
	ଅନ୍ୟଥା {
		retval = ext2fs_get_ଅଙ୍କ_dirs(fs, &ଅଙ୍କ_dirs);
		ଯଦି (retval)
			ଯାଅ cleanup;
		dblist->size = (ଅଙ୍କ_dirs * 2) + 12;
	}
	len = (size_t) ଆକାର(ସଂରଚନା ext2_db_entry2) * dblist->size;
	dblist->ଗଣନା_2 = ଗଣନା_2;
	retval = ext2fs_get_array(dblist->size, ଆକାର(ସଂରଚନା ext2_db_entry2),
		&dblist->list);
	ଯଦି (retval)
		ଯାଅ cleanup;

	ଯଦି (list)
		memcpy(dblist->list, list, len);
	ଅନ୍ୟଥା
		memset(dblist->list, 0, len);
	ଯଦି (ret_dblist)
		*ret_dblist = dblist;
	ଅନ୍ୟଥା
		fs->dblist = dblist;
	ଫେରନ୍ତୁ 0;
cleanup:
	ଯଦି (dblist)
		ext2fs_free_mem(&dblist);
	ଫେରନ୍ତୁ retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	dblist->sorted = 1;
	ଯଦି (ret_dblist)
		*ret_dblist = dblist;
	ଅନ୍ୟଥା
		fs->dblist = dblist;

	ଫେରନ୍ତୁ 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->size, src->ଗଣନା_2, src->list,
			     &dblist);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	ଫେରନ୍ତୁ 0;
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
	ସଂରଚନା ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	unsigned long		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ଯଦି (dblist->ଗଣନା_2 >= dblist->size) {
		old_size = dblist->size * ଆକାର(ସଂରଚନା ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (size_t) dblist->size *
					   ଆକାର(ସଂରଚନା ext2_db_entry2),
					   &dblist->list);
		ଯଦି (retval) {
			dblist->size = old_size / ଆକାର(ସଂରଚନା ext2_db_entry2);
			ଫେରନ୍ତୁ retval;
		}
	}
	new_entry = dblist->list + ( dblist->ଗଣନା_2++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	ଫେରନ୍ତୁ 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ପାଇଁ (i=0; i < dblist->ଗଣନା_2; i++) {
		ଯଦି ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			ଜାରି;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ EXT2_ET_DB_NOT_FOUND;
}

ଶୂନ୍ୟ ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ଅଚଳ ଶୂନ୍ୟ *,
						     ଅଚଳ ଶୂନ୍ୟ *))
{
	ଯଦି (!sortfunc)
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->ଗଣନା_2,
	      ଆକାର(ସଂରଚନା ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_filsys fs,
					     ସଂରଚନା ext2_db_entry2 *db_info,
					     ଶୂନ୍ୟ	*priv_data),
				 unsigned long long start,
				 unsigned long long ଗଣନା_2,
				 ଶୂନ୍ୟ *priv_data)
{
	unsigned long long	i, end;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + ଗଣନା_2;
	ଯଦି (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	ଯଦି (end > dblist->ଗଣନା_2)
		end = dblist->ଗଣନା_2;
	ପାଇଁ (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		ଯଦି (ret & DBLIST_ABORT)
			ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_filsys fs,
					     ସଂରଚନା ext2_db_entry2 *db_info,
					     ଶୂନ୍ୟ	*priv_data),
				 ଶୂନ୍ୟ *priv_data)
{
	ଫେରନ୍ତୁ ext2fs_dblist_iterate3(dblist, func, 0, dblist->ଗଣନା_2,
				      priv_data);
}

ସ୍ଥିର EXT2_QSORT_TYPE dir_block_cmp2(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
	ଅଚଳ ସଂରଚନା ext2_db_entry2 *db_a =
		(ଅଚଳ ସଂରଚନା ext2_db_entry2 *) a;
	ଅଚଳ ସଂରଚନା ext2_db_entry2 *db_b =
		(ଅଚଳ ସଂରଚନା ext2_db_entry2 *) b;

	ଯଦି (db_a->blk != db_b->blk)
		ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (db_a->blk - db_b->blk);

	ଯଦି (db_a->ino != db_b->ino)
		ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (db_a->ino - db_b->ino);

	ଫେରନ୍ତୁ (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	ଫେରନ୍ତୁ dblist->ଗଣନା_2;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  ସଂରଚନା ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ଯଦି (dblist->ଗଣନା_2 == 0)
		ଫେରନ୍ତୁ EXT2_ET_DBLIST_EMPTY;

	ଯଦି (entry)
		*entry = dblist->list + ( dblist->ଗଣନା_2-1);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ଯଦି (dblist->ଗଣନା_2 == 0)
		ଫେରନ୍ତୁ EXT2_ET_DBLIST_EMPTY;

	dblist->ଗଣନା_2--;
	ଫେରନ୍ତୁ 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା blockcnt)
{
	ଫେରନ୍ତୁ ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା blockcnt)
{
	ଫେରନ୍ତୁ ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

ଶୂନ୍ୟ ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ଅଚଳ ଶୂନ୍ୟ *,
						    ଅଚଳ ଶୂନ୍ୟ *))
{
	ଯଦି (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} ଅନ୍ୟଥା
		sortfunc = dir_block_cmp2;
	qsort(dblist->list, (size_t) dblist->ଗଣନା_2,
	      ଆକାର(ସଂରଚନା ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
ସଂରଚନା iterate_passthrough {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_filsys fs,
		    ସଂରଚନା ext2_db_entry *db_info,
		    ଶୂନ୍ୟ	*priv_data);
	ଶୂନ୍ୟ *priv_data;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା passthrough_func(ext2_filsys fs,
			    ସଂରଚନା ext2_db_entry2 *db_info,
			    ଶୂନ୍ୟ	*priv_data)
{
	ସଂରଚନା iterate_passthrough *p = priv_data;
	ସଂରଚନା ext2_db_entry db;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	ଫେରନ୍ତୁ ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_filsys fs,
					    ସଂରଚନା ext2_db_entry *db_info,
					    ଶୂନ୍ୟ	*priv_data),
				ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	ଫେରନ୍ତୁ ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

ସ୍ଥିର EXT2_QSORT_TYPE dir_block_cmp(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
	ଅଚଳ ସଂରଚନା ext2_db_entry2 *db_a =
		(ଅଚଳ ସଂରଚନା ext2_db_entry2 *) a;
	ଅଚଳ ସଂରଚନା ext2_db_entry2 *db_b =
		(ଅଚଳ ସଂରଚନା ext2_db_entry2 *) b;

	ସଂରଚନା ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	ଫେରନ୍ତୁ sortfunc32(&a32, &b32);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_dblist_ଗଣନା(ext2_dblist dblist)
{
	ଫେରନ୍ତୁ dblist->ଗଣନା_2;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 ସଂରଚନା ext2_db_entry **entry)
{
	ସ୍ଥିର ସଂରଚନା ext2_db_entry ret_entry;
	ସଂରଚନା ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ଯଦି (dblist->ଗଣନା_2 == 0)
		ଫେରନ୍ତୁ EXT2_ET_DBLIST_EMPTY;

	ଯଦି (!entry)
		ଫେରନ୍ତୁ 0;

	last = dblist->list + dblist->ଗଣନା_2 -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	ଫେରନ୍ତୁ 0;
}

