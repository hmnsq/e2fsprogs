/*
 * brel_ma.c
 *
 * Copyright (C) 1996, 1997 Theodore Ts'o.
 *
 * TODO: rewrite to not use a direct array!!!  (Fortunately this
 * module isn't really used yet.)
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#समावेश "config.h"
#समावेश <fcntl.h>
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "brel.h"

अटल errcode_t bma_put(ext2_brel brel, blk64_t old,
			काष्ठा ext2_block_relocate_entry *ent);
अटल errcode_t bma_get(ext2_brel brel, blk64_t old,
			काष्ठा ext2_block_relocate_entry *ent);
अटल errcode_t bma_start_iter(ext2_brel brel);
अटल errcode_t bma_अग्र(ext2_brel brel, blk64_t *old,
			 काष्ठा ext2_block_relocate_entry *ent);
अटल errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
अटल errcode_t bma_delete(ext2_brel brel, blk64_t old);
अटल errcode_t bma_free(ext2_brel brel);

काष्ठा brel_ma {
	__u32 magic;
	blk64_t max_block;
	काष्ठा ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(अक्षर *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	काष्ठा brel_ma 	*ma = 0;
	माप_प्रकार		आकार;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(माप(काष्ठा ext2_block_relocation_table),
				&brel);
	अगर (retval)
		जाओ errout;
	स_रखो(brel, 0, माप(काष्ठा ext2_block_relocation_table));

	retval = ext2fs_get_mem(म_माप(name)+1, &brel->name);
	अगर (retval)
		जाओ errout;
	म_नकल(brel->name, name);

	retval = ext2fs_get_mem(माप(काष्ठा brel_ma), &ma);
	अगर (retval)
		जाओ errout;
	स_रखो(ma, 0, माप(काष्ठा brel_ma));
	brel->priv_तथ्य = ma;

	आकार = (माप_प्रकार) (माप(काष्ठा ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		माप(काष्ठा ext2_block_relocate_entry), &ma->entries);
	अगर (retval)
		जाओ errout;
	स_रखो(ma->entries, 0, आकार);
	ma->max_block = max_block;

	/*
	 * Fill in the brel data structure
	 */
	brel->put = bma_put;
	brel->get = bma_get;
	brel->start_iter = bma_start_iter;
	brel->अग्र = bma_अग्र;
	brel->move = bma_move;
	brel->delete = bma_delete;
	brel->मुक्त = bma_free;

	*new_brel = brel;
	वापस 0;

errout:
	bma_free(brel);
	वापस retval;
}

अटल errcode_t bma_put(ext2_brel brel, blk64_t old,
			काष्ठा ext2_block_relocate_entry *ent)
{
	काष्ठा brel_ma 	*ma;

	ma = brel->priv_तथ्य;
	अगर (old > ma->max_block)
		वापस EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(अचिन्हित)old] = *ent;
	वापस 0;
}

अटल errcode_t bma_get(ext2_brel brel, blk64_t old,
			काष्ठा ext2_block_relocate_entry *ent)
{
	काष्ठा brel_ma 	*ma;

	ma = brel->priv_तथ्य;
	अगर (old > ma->max_block)
		वापस EXT2_ET_INVALID_ARGUMENT;
	अगर (ma->entries[(अचिन्हित)old].new == 0)
		वापस ENOENT;
	*ent = ma->entries[old];
	वापस 0;
}

अटल errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	वापस 0;
}

अटल errcode_t bma_अग्र(ext2_brel brel, blk64_t *old,
			  काष्ठा ext2_block_relocate_entry *ent)
{
	काष्ठा brel_ma 	*ma;

	ma = brel->priv_तथ्य;
	जबतक (++brel->current < ma->max_block) {
		अगर (ma->entries[(अचिन्हित)brel->current].new == 0)
			जारी;
		*old = brel->current;
		*ent = ma->entries[(अचिन्हित)brel->current];
		वापस 0;
	}
	*old = 0;
	वापस 0;
}

अटल errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	काष्ठा brel_ma 	*ma;

	ma = brel->priv_तथ्य;
	अगर ((old > ma->max_block) || (new > ma->max_block))
		वापस EXT2_ET_INVALID_ARGUMENT;
	अगर (ma->entries[(अचिन्हित)old].new == 0)
		वापस ENOENT;
	ma->entries[(अचिन्हित)new] = ma->entries[old];
	ma->entries[(अचिन्हित)old].new = 0;
	वापस 0;
}

अटल errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	काष्ठा brel_ma 	*ma;

	ma = brel->priv_तथ्य;
	अगर (old > ma->max_block)
		वापस EXT2_ET_INVALID_ARGUMENT;
	अगर (ma->entries[(अचिन्हित)old].new == 0)
		वापस ENOENT;
	ma->entries[(अचिन्हित)old].new = 0;
	वापस 0;
}

अटल errcode_t bma_free(ext2_brel brel)
{
	काष्ठा brel_ma 	*ma;

	अगर (!brel)
		वापस 0;

	ma = brel->priv_तथ्य;

	अगर (ma) {
		अगर (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	अगर (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	वापस 0;
}
