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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर EXT2_QSORT_TYPE dir_block_cmp(ध्रुव शून्य *a, ध्रुव शून्य *b);
स्थिर EXT2_QSORT_TYPE dir_block_cmp2(ध्रुव शून्य *a, ध्रुव शून्य *b);
स्थिर EXT2_QSORT_TYPE (*sortfunc32)(ध्रुव शून्य *a, ध्रुव शून्य *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
स्थिर errcode_t make_dblist(ext2_filsys fs, ext2_ino_t size,
			     ext2_ino_t गिनती,
			     संरचना ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = रिक्त;
	errcode_t	retval;
	ext2_ino_t	अंक_dirs;
	माप_प्रकार		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		प्रत्यागम 0;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_struct_dblist), &dblist);
	यदि (retval)
		गच्छ cleanup;
	स_रखो(dblist, 0, परिमाण(संरचना ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	यदि (size)
		dblist->size = size;
	अन्यथा {
		retval = ext2fs_get_अंक_dirs(fs, &अंक_dirs);
		यदि (retval)
			गच्छ cleanup;
		dblist->size = (अंक_dirs * 2) + 12;
	}
	len = (माप_प्रकार) परिमाण(संरचना ext2_db_entry2) * dblist->size;
	dblist->गिनती = गिनती;
	retval = ext2fs_get_array(dblist->size, परिमाण(संरचना ext2_db_entry2),
		&dblist->list);
	यदि (retval)
		गच्छ cleanup;

	यदि (list)
		स_नकल(dblist->list, list, len);
	अन्यथा
		स_रखो(dblist->list, 0, len);
	यदि (ret_dblist)
		*ret_dblist = dblist;
	अन्यथा
		fs->dblist = dblist;
	प्रत्यागम 0;
cleanup:
	यदि (dblist)
		ext2fs_free_mem(&dblist);
	प्रत्यागम retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	यदि (retval)
		प्रत्यागम retval;

	dblist->sorted = 1;
	यदि (ret_dblist)
		*ret_dblist = dblist;
	अन्यथा
		fs->dblist = dblist;

	प्रत्यागम 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->size, src->गिनती, src->list,
			     &dblist);
	यदि (retval)
		प्रत्यागम retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	प्रत्यागम 0;
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
	संरचना ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	अचिन्हित दीर्घ		old_size;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	यदि (dblist->गिनती >= dblist->size) {
		old_size = dblist->size * परिमाण(संरचना ext2_db_entry2);
		dblist->size += dblist->size > 200 ? dblist->size / 2 : 100;
		retval = ext2fs_resize_mem(old_size, (माप_प्रकार) dblist->size *
					   परिमाण(संरचना ext2_db_entry2),
					   &dblist->list);
		यदि (retval) {
			dblist->size = old_size / परिमाण(संरचना ext2_db_entry2);
			प्रत्यागम retval;
		}
	}
	new_entry = dblist->list + ( dblist->गिनती++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	प्रत्यागम 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	प्रति (i=0; i < dblist->गिनती; i++) {
		यदि ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			अनुवर्त;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		प्रत्यागम 0;
	}
	प्रत्यागम EXT2_ET_DB_NOT_FOUND;
}

शून्य ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(ध्रुव शून्य *,
						     ध्रुव शून्य *))
{
	यदि (!sortfunc)
		sortfunc = dir_block_cmp2;
	क्विक(dblist->list, (माप_प्रकार) dblist->गिनती,
	      परिमाण(संरचना ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 पूर्णाङ्क (*func)(ext2_filsys fs,
					     संरचना ext2_db_entry2 *db_info,
					     शून्य	*priv_data),
				 अचिन्हित दीर्घ दीर्घ start,
				 अचिन्हित दीर्घ दीर्घ गिनती,
				 शून्य *priv_data)
{
	अचिन्हित दीर्घ दीर्घ	i, end;
	पूर्णाङ्क		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + गिनती;
	यदि (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	यदि (end > dblist->गिनती)
		end = dblist->गिनती;
	प्रति (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_data);
		यदि (ret & DBLIST_ABORT)
			प्रत्यागम 0;
	}
	प्रत्यागम 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 पूर्णाङ्क (*func)(ext2_filsys fs,
					     संरचना ext2_db_entry2 *db_info,
					     शून्य	*priv_data),
				 शून्य *priv_data)
{
	प्रत्यागम ext2fs_dblist_iterate3(dblist, func, 0, dblist->गिनती,
				      priv_data);
}

स्थिर EXT2_QSORT_TYPE dir_block_cmp2(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	ध्रुव संरचना ext2_db_entry2 *db_a =
		(ध्रुव संरचना ext2_db_entry2 *) a;
	ध्रुव संरचना ext2_db_entry2 *db_b =
		(ध्रुव संरचना ext2_db_entry2 *) b;

	यदि (db_a->blk != db_b->blk)
		प्रत्यागम (पूर्णाङ्क) (db_a->blk - db_b->blk);

	यदि (db_a->ino != db_b->ino)
		प्रत्यागम (पूर्णाङ्क) (db_a->ino - db_b->ino);

	प्रत्यागम (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	प्रत्यागम dblist->गिनती;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  संरचना ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	यदि (dblist->गिनती == 0)
		प्रत्यागम EXT2_ET_DBLIST_EMPTY;

	यदि (entry)
		*entry = dblist->list + ( dblist->गिनती-1);
	प्रत्यागम 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	यदि (dblist->गिनती == 0)
		प्रत्यागम EXT2_ET_DBLIST_EMPTY;

	dblist->गिनती--;
	प्रत्यागम 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       पूर्णाङ्क blockcnt)
{
	प्रत्यागम ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       पूर्णाङ्क blockcnt)
{
	प्रत्यागम ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

शून्य ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(ध्रुव शून्य *,
						    ध्रुव शून्य *))
{
	यदि (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} अन्यथा
		sortfunc = dir_block_cmp2;
	क्विक(dblist->list, (माप_प्रकार) dblist->गिनती,
	      परिमाण(संरचना ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
संरचना iterate_passthrough {
	पूर्णाङ्क (*func)(ext2_filsys fs,
		    संरचना ext2_db_entry *db_info,
		    शून्य	*priv_data);
	शून्य *priv_data;
};

स्थिर पूर्णाङ्क passthrough_func(ext2_filsys fs,
			    संरचना ext2_db_entry2 *db_info,
			    शून्य	*priv_data)
{
	संरचना iterate_passthrough *p = priv_data;
	संरचना ext2_db_entry db;
	पूर्णाङ्क ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (पूर्णाङ्क) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_data);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	प्रत्यागम ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				पूर्णाङ्क (*func)(ext2_filsys fs,
					    संरचना ext2_db_entry *db_info,
					    शून्य	*priv_data),
				शून्य *priv_data)
{
	संरचना iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_data = priv_data;

	प्रत्यागम ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

स्थिर EXT2_QSORT_TYPE dir_block_cmp(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	ध्रुव संरचना ext2_db_entry2 *db_a =
		(ध्रुव संरचना ext2_db_entry2 *) a;
	ध्रुव संरचना ext2_db_entry2 *db_b =
		(ध्रुव संरचना ext2_db_entry2 *) b;

	संरचना ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	प्रत्यागम sortfunc32(&a32, &b32);
}

पूर्णाङ्क ext2fs_dblist_गिनती(ext2_dblist dblist)
{
	प्रत्यागम dblist->गिनती;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 संरचना ext2_db_entry **entry)
{
	स्थिर संरचना ext2_db_entry ret_entry;
	संरचना ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	यदि (dblist->गिनती == 0)
		प्रत्यागम EXT2_ET_DBLIST_EMPTY;

	यदि (!entry)
		प्रत्यागम 0;

	last = dblist->list + dblist->गिनती -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	प्रत्यागम 0;
}

