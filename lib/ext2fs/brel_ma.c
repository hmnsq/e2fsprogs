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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "brel.h"

સ્થિર errcode_t bma_put(ext2_brel brel, blk64_t old,
			સંરચના ext2_block_relocate_entry *ent);
સ્થિર errcode_t bma_get(ext2_brel brel, blk64_t old,
			સંરચના ext2_block_relocate_entry *ent);
સ્થિર errcode_t bma_start_iter(ext2_brel brel);
સ્થિર errcode_t bma_next(ext2_brel brel, blk64_t *old,
			 સંરચના ext2_block_relocate_entry *ent);
સ્થિર errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
સ્થિર errcode_t bma_delete(ext2_brel brel, blk64_t old);
સ્થિર errcode_t bma_free(ext2_brel brel);

સંરચના brel_ma {
	__u32 magic;
	blk64_t max_block;
	સંરચના ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(અક્ષર *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	સંરચના brel_ma 	*ma = 0;
	size_t		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(કદ(સંરચના ext2_block_relocation_table),
				&brel);
	જો (retval)
		જાઓ errout;
	memset(brel, 0, કદ(સંરચના ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	જો (retval)
		જાઓ errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(કદ(સંરચના brel_ma), &ma);
	જો (retval)
		જાઓ errout;
	memset(ma, 0, કદ(સંરચના brel_ma));
	brel->priv_data = ma;

	size = (size_t) (કદ(સંરચના ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		કદ(સંરચના ext2_block_relocate_entry), &ma->entries);
	જો (retval)
		જાઓ errout;
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
	brel->મુક્ત = bma_free;

	*new_brel = brel;
	પાછા 0;

errout:
	bma_free(brel);
	પાછા retval;
}

સ્થિર errcode_t bma_put(ext2_brel brel, blk64_t old,
			સંરચના ext2_block_relocate_entry *ent)
{
	સંરચના brel_ma 	*ma;

	ma = brel->priv_data;
	જો (old > ma->max_block)
		પાછા EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	પાછા 0;
}

સ્થિર errcode_t bma_get(ext2_brel brel, blk64_t old,
			સંરચના ext2_block_relocate_entry *ent)
{
	સંરચના brel_ma 	*ma;

	ma = brel->priv_data;
	જો (old > ma->max_block)
		પાછા EXT2_ET_INVALID_ARGUMENT;
	જો (ma->entries[(unsigned)old].new == 0)
		પાછા ENOENT;
	*ent = ma->entries[old];
	પાછા 0;
}

સ્થિર errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	પાછા 0;
}

સ્થિર errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  સંરચના ext2_block_relocate_entry *ent)
{
	સંરચના brel_ma 	*ma;

	ma = brel->priv_data;
	જ્યારે (++brel->current < ma->max_block) {
		જો (ma->entries[(unsigned)brel->current].new == 0)
			ચાલુ;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		પાછા 0;
	}
	*old = 0;
	પાછા 0;
}

સ્થિર errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	સંરચના brel_ma 	*ma;

	ma = brel->priv_data;
	જો ((old > ma->max_block) || (new > ma->max_block))
		પાછા EXT2_ET_INVALID_ARGUMENT;
	જો (ma->entries[(unsigned)old].new == 0)
		પાછા ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	પાછા 0;
}

સ્થિર errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	સંરચના brel_ma 	*ma;

	ma = brel->priv_data;
	જો (old > ma->max_block)
		પાછા EXT2_ET_INVALID_ARGUMENT;
	જો (ma->entries[(unsigned)old].new == 0)
		પાછા ENOENT;
	ma->entries[(unsigned)old].new = 0;
	પાછા 0;
}

સ્થિર errcode_t bma_free(ext2_brel brel)
{
	સંરચના brel_ma 	*ma;

	જો (!brel)
		પાછા 0;

	ma = brel->priv_data;

	જો (ma) {
		જો (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	જો (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	પાછા 0;
}
