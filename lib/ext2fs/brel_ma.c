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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "brel.h"

நிலையான errcode_t bma_put(ext2_brel brel, blk64_t old,
			கட்டமைப்பு ext2_block_relocate_entry *ent);
நிலையான errcode_t bma_get(ext2_brel brel, blk64_t old,
			கட்டமைப்பு ext2_block_relocate_entry *ent);
நிலையான errcode_t bma_start_iter(ext2_brel brel);
நிலையான errcode_t bma_next(ext2_brel brel, blk64_t *old,
			 கட்டமைப்பு ext2_block_relocate_entry *ent);
நிலையான errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
நிலையான errcode_t bma_delete(ext2_brel brel, blk64_t old);
நிலையான errcode_t bma_free(ext2_brel brel);

கட்டமைப்பு brel_ma {
	__u32 magic;
	blk64_t max_block;
	கட்டமைப்பு ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(எழுத்து *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	கட்டமைப்பு brel_ma 	*ma = 0;
	size_t		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_block_relocation_table),
				&brel);
	எனில் (retval)
		செல் errout;
	memset(brel, 0, அளவு(கட்டமைப்பு ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	எனில் (retval)
		செல் errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு brel_ma), &ma);
	எனில் (retval)
		செல் errout;
	memset(ma, 0, அளவு(கட்டமைப்பு brel_ma));
	brel->priv_data = ma;

	size = (size_t) (அளவு(கட்டமைப்பு ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		அளவு(கட்டமைப்பு ext2_block_relocate_entry), &ma->entries);
	எனில் (retval)
		செல் errout;
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
	brel->விடுவி = bma_free;

	*new_brel = brel;
	திரும்பு 0;

errout:
	bma_free(brel);
	திரும்பு retval;
}

நிலையான errcode_t bma_put(ext2_brel brel, blk64_t old,
			கட்டமைப்பு ext2_block_relocate_entry *ent)
{
	கட்டமைப்பு brel_ma 	*ma;

	ma = brel->priv_data;
	எனில் (old > ma->max_block)
		திரும்பு EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	திரும்பு 0;
}

நிலையான errcode_t bma_get(ext2_brel brel, blk64_t old,
			கட்டமைப்பு ext2_block_relocate_entry *ent)
{
	கட்டமைப்பு brel_ma 	*ma;

	ma = brel->priv_data;
	எனில் (old > ma->max_block)
		திரும்பு EXT2_ET_INVALID_ARGUMENT;
	எனில் (ma->entries[(unsigned)old].new == 0)
		திரும்பு ENOENT;
	*ent = ma->entries[old];
	திரும்பு 0;
}

நிலையான errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	திரும்பு 0;
}

நிலையான errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  கட்டமைப்பு ext2_block_relocate_entry *ent)
{
	கட்டமைப்பு brel_ma 	*ma;

	ma = brel->priv_data;
	போது (++brel->current < ma->max_block) {
		எனில் (ma->entries[(unsigned)brel->current].new == 0)
			தொடர்;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		திரும்பு 0;
	}
	*old = 0;
	திரும்பு 0;
}

நிலையான errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	கட்டமைப்பு brel_ma 	*ma;

	ma = brel->priv_data;
	எனில் ((old > ma->max_block) || (new > ma->max_block))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;
	எனில் (ma->entries[(unsigned)old].new == 0)
		திரும்பு ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	திரும்பு 0;
}

நிலையான errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	கட்டமைப்பு brel_ma 	*ma;

	ma = brel->priv_data;
	எனில் (old > ma->max_block)
		திரும்பு EXT2_ET_INVALID_ARGUMENT;
	எனில் (ma->entries[(unsigned)old].new == 0)
		திரும்பு ENOENT;
	ma->entries[(unsigned)old].new = 0;
	திரும்பு 0;
}

நிலையான errcode_t bma_free(ext2_brel brel)
{
	கட்டமைப்பு brel_ma 	*ma;

	எனில் (!brel)
		திரும்பு 0;

	ma = brel->priv_data;

	எனில் (ma) {
		எனில் (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	எனில் (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	திரும்பு 0;
}
