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

#ضمن "config.h"
#ضمن <fcntl.h>
#ضمن <stdio.h>
#ضمن <string.h>
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#אם HAVE_ERRNO_H
#ضمن <errno.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "brel.h"

ساكن errcode_t bma_put(ext2_brel brel, blk64_t old,
			بنية ext2_block_relocate_entry *ent);
ساكن errcode_t bma_get(ext2_brel brel, blk64_t old,
			بنية ext2_block_relocate_entry *ent);
ساكن errcode_t bma_start_iter(ext2_brel brel);
ساكن errcode_t bma_next(ext2_brel brel, blk64_t *old,
			 بنية ext2_block_relocate_entry *ent);
ساكن errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
ساكن errcode_t bma_delete(ext2_brel brel, blk64_t old);
ساكن errcode_t bma_free(ext2_brel brel);

بنية brel_ma {
	__u32 magic;
	blk64_t max_block;
	بنية ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(حرف *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	بنية brel_ma 	*ma = 0;
	size_t		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(حجم(بنية ext2_block_relocation_table),
				&brel);
	אם (retval)
		اذهب errout;
	memset(brel, 0, حجم(بنية ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	אם (retval)
		اذهب errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(حجم(بنية brel_ma), &ma);
	אם (retval)
		اذهب errout;
	memset(ma, 0, حجم(بنية brel_ma));
	brel->priv_data = ma;

	size = (size_t) (حجم(بنية ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		حجم(بنية ext2_block_relocate_entry), &ma->entries);
	אם (retval)
		اذهب errout;
	memset(ma->entries, 0, size);
	ma->max_block = max_block;

	/*
	 * Fill in the brel data structure
	 */
	brel->put = bma_put;
	brel->get = bma_get;
	brel->start_iter = bma_start_iter;
	brel->next = bma_next;
	brel->move = bma_move;
	brel->delete = bma_delete;
	brel->حرر = bma_free;

	*new_brel = brel;
	החזר 0;

errout:
	bma_free(brel);
	החזר retval;
}

ساكن errcode_t bma_put(ext2_brel brel, blk64_t old,
			بنية ext2_block_relocate_entry *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_data;
	אם (old > ma->max_block)
		החזר EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	החזר 0;
}

ساكن errcode_t bma_get(ext2_brel brel, blk64_t old,
			بنية ext2_block_relocate_entry *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_data;
	אם (old > ma->max_block)
		החזר EXT2_ET_INVALID_ARGUMENT;
	אם (ma->entries[(unsigned)old].new == 0)
		החזר ENOENT;
	*ent = ma->entries[old];
	החזר 0;
}

ساكن errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	החזר 0;
}

ساكن errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  بنية ext2_block_relocate_entry *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_data;
	כלעוד (++brel->current < ma->max_block) {
		אם (ma->entries[(unsigned)brel->current].new == 0)
			استمر;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		החזר 0;
	}
	*old = 0;
	החזר 0;
}

ساكن errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_data;
	אם ((old > ma->max_block) || (new > ma->max_block))
		החזר EXT2_ET_INVALID_ARGUMENT;
	אם (ma->entries[(unsigned)old].new == 0)
		החזר ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	החזר 0;
}

ساكن errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_data;
	אם (old > ma->max_block)
		החזר EXT2_ET_INVALID_ARGUMENT;
	אם (ma->entries[(unsigned)old].new == 0)
		החזר ENOENT;
	ma->entries[(unsigned)old].new = 0;
	החזר 0;
}

ساكن errcode_t bma_free(ext2_brel brel)
{
	بنية brel_ma 	*ma;

	אם (!brel)
		החזר 0;

	ma = brel->priv_data;

	אם (ma) {
		אם (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	אם (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	החזר 0;
}
