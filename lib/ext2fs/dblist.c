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

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <time.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

अटल EXT2_QSORT_TYPE dir_block_cmp(स्थिर व्योम *a, स्थिर व्योम *b);
अटल EXT2_QSORT_TYPE dir_block_cmp2(स्थिर व्योम *a, स्थिर व्योम *b);
अटल EXT2_QSORT_TYPE (*sortfunc32)(स्थिर व्योम *a, स्थिर व्योम *b);

/*
 * helper function for making a new directory block list (for
 * initialize and copy).
 */
अटल errcode_t make_dblist(ext2_filsys fs, ext2_ino_t आकार,
			     ext2_ino_t गिनती,
			     काष्ठा ext2_db_entry2 *list,
			     ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist = शून्य;
	errcode_t	retval;
	ext2_ino_t	अंक_dirs;
	माप_प्रकार		len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर ((ret_dblist == 0) && fs->dblist &&
	    (fs->dblist->magic == EXT2_ET_MAGIC_DBLIST))
		वापस 0;

	retval = ext2fs_get_mem(माप(काष्ठा ext2_struct_dblist), &dblist);
	अगर (retval)
		जाओ cleanup;
	स_रखो(dblist, 0, माप(काष्ठा ext2_struct_dblist));

	dblist->magic = EXT2_ET_MAGIC_DBLIST;
	dblist->fs = fs;
	अगर (आकार)
		dblist->आकार = आकार;
	अन्यथा {
		retval = ext2fs_get_अंक_dirs(fs, &अंक_dirs);
		अगर (retval)
			जाओ cleanup;
		dblist->आकार = (अंक_dirs * 2) + 12;
	}
	len = (माप_प्रकार) माप(काष्ठा ext2_db_entry2) * dblist->आकार;
	dblist->गिनती = गिनती;
	retval = ext2fs_get_array(dblist->आकार, माप(काष्ठा ext2_db_entry2),
		&dblist->list);
	अगर (retval)
		जाओ cleanup;

	अगर (list)
		स_नकल(dblist->list, list, len);
	अन्यथा
		स_रखो(dblist->list, 0, len);
	अगर (ret_dblist)
		*ret_dblist = dblist;
	अन्यथा
		fs->dblist = dblist;
	वापस 0;
cleanup:
	अगर (dblist)
		ext2fs_free_mem(&dblist);
	वापस retval;
}

/*
 * Initialize a directory block list
 */
errcode_t ext2fs_init_dblist(ext2_filsys fs, ext2_dblist *ret_dblist)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(fs, 0, 0, 0, &dblist);
	अगर (retval)
		वापस retval;

	dblist->sorted = 1;
	अगर (ret_dblist)
		*ret_dblist = dblist;
	अन्यथा
		fs->dblist = dblist;

	वापस 0;
}

/*
 * Copy a directory block list
 */
errcode_t ext2fs_copy_dblist(ext2_dblist src, ext2_dblist *dest)
{
	ext2_dblist	dblist;
	errcode_t	retval;

	retval = make_dblist(src->fs, src->आकार, src->गिनती, src->list,
			     &dblist);
	अगर (retval)
		वापस retval;
	dblist->sorted = src->sorted;
	*dest = dblist;
	वापस 0;
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
	काष्ठा ext2_db_entry2 	*new_entry;
	errcode_t		retval;
	अचिन्हित दीर्घ		old_आकार;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	अगर (dblist->गिनती >= dblist->आकार) {
		old_आकार = dblist->आकार * माप(काष्ठा ext2_db_entry2);
		dblist->आकार += dblist->आकार > 200 ? dblist->आकार / 2 : 100;
		retval = ext2fs_resize_mem(old_आकार, (माप_प्रकार) dblist->आकार *
					   माप(काष्ठा ext2_db_entry2),
					   &dblist->list);
		अगर (retval) {
			dblist->आकार = old_आकार / माप(काष्ठा ext2_db_entry2);
			वापस retval;
		}
	}
	new_entry = dblist->list + ( dblist->गिनती++);
	new_entry->blk = blk;
	new_entry->ino = ino;
	new_entry->blockcnt = blockcnt;

	dblist->sorted = 0;

	वापस 0;
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block2(ext2_dblist dblist, ext2_ino_t ino,
				blk64_t blk, e2_blkcnt_t blockcnt)
{
	dgrp_t			i;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	क्रम (i=0; i < dblist->गिनती; i++) {
		अगर ((dblist->list[i].ino != ino) ||
		    (dblist->list[i].blockcnt != blockcnt))
			जारी;
		dblist->list[i].blk = blk;
		dblist->sorted = 0;
		वापस 0;
	}
	वापस EXT2_ET_DB_NOT_FOUND;
}

व्योम ext2fs_dblist_sort2(ext2_dblist dblist,
			 EXT2_QSORT_TYPE (*sortfunc)(स्थिर व्योम *,
						     स्थिर व्योम *))
{
	अगर (!sortfunc)
		sortfunc = dir_block_cmp2;
	क्विक(dblist->list, (माप_प्रकार) dblist->गिनती,
	      माप(काष्ठा ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
errcode_t ext2fs_dblist_iterate3(ext2_dblist dblist,
				 पूर्णांक (*func)(ext2_filsys fs,
					     काष्ठा ext2_db_entry2 *db_info,
					     व्योम	*priv_तथ्य),
				 अचिन्हित दीर्घ दीर्घ start,
				 अचिन्हित दीर्घ दीर्घ गिनती,
				 व्योम *priv_तथ्य)
{
	अचिन्हित दीर्घ दीर्घ	i, end;
	पूर्णांक		ret;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	end = start + गिनती;
	अगर (!dblist->sorted)
		ext2fs_dblist_sort2(dblist, 0);
	अगर (end > dblist->गिनती)
		end = dblist->गिनती;
	क्रम (i = start; i < end; i++) {
		ret = (*func)(dblist->fs, &dblist->list[i], priv_तथ्य);
		अगर (ret & DBLIST_ABORT)
			वापस 0;
	}
	वापस 0;
}

errcode_t ext2fs_dblist_iterate2(ext2_dblist dblist,
				 पूर्णांक (*func)(ext2_filsys fs,
					     काष्ठा ext2_db_entry2 *db_info,
					     व्योम	*priv_तथ्य),
				 व्योम *priv_तथ्य)
{
	वापस ext2fs_dblist_iterate3(dblist, func, 0, dblist->गिनती,
				      priv_तथ्य);
}

अटल EXT2_QSORT_TYPE dir_block_cmp2(स्थिर व्योम *a, स्थिर व्योम *b)
{
	स्थिर काष्ठा ext2_db_entry2 *db_a =
		(स्थिर काष्ठा ext2_db_entry2 *) a;
	स्थिर काष्ठा ext2_db_entry2 *db_b =
		(स्थिर काष्ठा ext2_db_entry2 *) b;

	अगर (db_a->blk != db_b->blk)
		वापस (पूर्णांक) (db_a->blk - db_b->blk);

	अगर (db_a->ino != db_b->ino)
		वापस (पूर्णांक) (db_a->ino - db_b->ino);

	वापस (db_a->blockcnt - db_b->blockcnt);
}

blk64_t ext2fs_dblist_count2(ext2_dblist dblist)
{
	वापस dblist->गिनती;
}

errcode_t ext2fs_dblist_get_last2(ext2_dblist dblist,
				  काष्ठा ext2_db_entry2 **entry)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	अगर (dblist->गिनती == 0)
		वापस EXT2_ET_DBLIST_EMPTY;

	अगर (entry)
		*entry = dblist->list + ( dblist->गिनती-1);
	वापस 0;
}

errcode_t ext2fs_dblist_drop_last(ext2_dblist dblist)
{
	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	अगर (dblist->गिनती == 0)
		वापस EXT2_ET_DBLIST_EMPTY;

	dblist->गिनती--;
	वापस 0;
}

/*
 * Legacy 32-bit versions
 */

/*
 * Add a directory block to the directory block list
 */
errcode_t ext2fs_add_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       पूर्णांक blockcnt)
{
	वापस ext2fs_add_dir_block2(dblist, ino, blk, blockcnt);
}

/*
 * Change the directory block to the directory block list
 */
errcode_t ext2fs_set_dir_block(ext2_dblist dblist, ext2_ino_t ino, blk_t blk,
			       पूर्णांक blockcnt)
{
	वापस ext2fs_set_dir_block2(dblist, ino, blk, blockcnt);
}

व्योम ext2fs_dblist_sort(ext2_dblist dblist,
			EXT2_QSORT_TYPE (*sortfunc)(स्थिर व्योम *,
						    स्थिर व्योम *))
{
	अगर (sortfunc) {
		sortfunc32 = sortfunc;
		sortfunc = dir_block_cmp;
	} अन्यथा
		sortfunc = dir_block_cmp2;
	क्विक(dblist->list, (माप_प्रकार) dblist->गिनती,
	      माप(काष्ठा ext2_db_entry2), sortfunc);
	dblist->sorted = 1;
}

/*
 * This function iterates over the directory block list
 */
काष्ठा iterate_passthrough {
	पूर्णांक (*func)(ext2_filsys fs,
		    काष्ठा ext2_db_entry *db_info,
		    व्योम	*priv_तथ्य);
	व्योम *priv_तथ्य;
};

अटल पूर्णांक passthrough_func(ext2_filsys fs,
			    काष्ठा ext2_db_entry2 *db_info,
			    व्योम	*priv_तथ्य)
{
	काष्ठा iterate_passthrough *p = priv_तथ्य;
	काष्ठा ext2_db_entry db;
	पूर्णांक ret;

	db.ino = db_info->ino;
	db.blk = (blk_t) db_info->blk;
	db.blockcnt = (पूर्णांक) db_info->blockcnt;
	ret = (p->func)(fs, &db, p->priv_तथ्य);
	db_info->ino = db.ino;
	db_info->blk = db.blk;
	db_info->blockcnt = db.blockcnt;
	वापस ret;
}

errcode_t ext2fs_dblist_iterate(ext2_dblist dblist,
				पूर्णांक (*func)(ext2_filsys fs,
					    काष्ठा ext2_db_entry *db_info,
					    व्योम	*priv_तथ्य),
				व्योम *priv_तथ्य)
{
	काष्ठा iterate_passthrough pass;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);
	pass.func = func;
	pass.priv_तथ्य = priv_तथ्य;

	वापस ext2fs_dblist_iterate2(dblist, passthrough_func, &pass);
}

अटल EXT2_QSORT_TYPE dir_block_cmp(स्थिर व्योम *a, स्थिर व्योम *b)
{
	स्थिर काष्ठा ext2_db_entry2 *db_a =
		(स्थिर काष्ठा ext2_db_entry2 *) a;
	स्थिर काष्ठा ext2_db_entry2 *db_b =
		(स्थिर काष्ठा ext2_db_entry2 *) b;

	काष्ठा ext2_db_entry a32, b32;

	a32.ino = db_a->ino;  a32.blk = db_a->blk;
	a32.blockcnt = db_a->blockcnt;

	b32.ino = db_b->ino;  b32.blk = db_b->blk;
	b32.blockcnt = db_b->blockcnt;

	वापस sortfunc32(&a32, &b32);
}

पूर्णांक ext2fs_dblist_गिनती(ext2_dblist dblist)
{
	वापस dblist->गिनती;
}

errcode_t ext2fs_dblist_get_last(ext2_dblist dblist,
				 काष्ठा ext2_db_entry **entry)
{
	अटल काष्ठा ext2_db_entry ret_entry;
	काष्ठा ext2_db_entry2 *last;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	अगर (dblist->गिनती == 0)
		वापस EXT2_ET_DBLIST_EMPTY;

	अगर (!entry)
		वापस 0;

	last = dblist->list + dblist->गिनती -1;

	ret_entry.ino = last->ino;
	ret_entry.blk = last->blk;
	ret_entry.blockcnt = last->blockcnt;
	*entry = &ret_entry;

	वापस 0;
}

