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

#شامل "config.h"
#شامل <fcntl.h>
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "brel.h"

ایستا errcode_t bma_put(ext2_brel brel, blk64_t old,
			ساختار ext2_block_relocate_entry *ent);
ایستا errcode_t bma_get(ext2_brel brel, blk64_t old,
			ساختار ext2_block_relocate_entry *ent);
ایستا errcode_t bma_start_iter(ext2_brel brel);
ایستا errcode_t bma_next(ext2_brel brel, blk64_t *old,
			 ساختار ext2_block_relocate_entry *ent);
ایستا errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
ایستا errcode_t bma_delete(ext2_brel brel, blk64_t old);
ایستا errcode_t bma_free(ext2_brel brel);

ساختار brel_ma {
	__u32 magic;
	blk64_t max_block;
	ساختار ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(نویسه *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	ساختار brel_ma 	*ma = 0;
	size_t		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(اندازه(ساختار ext2_block_relocation_table),
				&brel);
	اگر (retval)
		برو errout;
	memset(brel, 0, اندازه(ساختار ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	اگر (retval)
		برو errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(اندازه(ساختار brel_ma), &ma);
	اگر (retval)
		برو errout;
	memset(ma, 0, اندازه(ساختار brel_ma));
	brel->priv_data = ma;

	size = (size_t) (اندازه(ساختار ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		اندازه(ساختار ext2_block_relocate_entry), &ma->entries);
	اگر (retval)
		برو errout;
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
	brel->آزاد = bma_free;

	*new_brel = brel;
	بازگشت 0;

errout:
	bma_free(brel);
	بازگشت retval;
}

ایستا errcode_t bma_put(ext2_brel brel, blk64_t old,
			ساختار ext2_block_relocate_entry *ent)
{
	ساختار brel_ma 	*ma;

	ma = brel->priv_data;
	اگر (old > ma->max_block)
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	بازگشت 0;
}

ایستا errcode_t bma_get(ext2_brel brel, blk64_t old,
			ساختار ext2_block_relocate_entry *ent)
{
	ساختار brel_ma 	*ma;

	ma = brel->priv_data;
	اگر (old > ma->max_block)
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	اگر (ma->entries[(unsigned)old].new == 0)
		بازگشت ENOENT;
	*ent = ma->entries[old];
	بازگشت 0;
}

ایستا errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	بازگشت 0;
}

ایستا errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  ساختار ext2_block_relocate_entry *ent)
{
	ساختار brel_ma 	*ma;

	ma = brel->priv_data;
	وقتی (++brel->current < ma->max_block) {
		اگر (ma->entries[(unsigned)brel->current].new == 0)
			ادامه;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		بازگشت 0;
	}
	*old = 0;
	بازگشت 0;
}

ایستا errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	ساختار brel_ma 	*ma;

	ma = brel->priv_data;
	اگر ((old > ma->max_block) || (new > ma->max_block))
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	اگر (ma->entries[(unsigned)old].new == 0)
		بازگشت ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	بازگشت 0;
}

ایستا errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	ساختار brel_ma 	*ma;

	ma = brel->priv_data;
	اگر (old > ma->max_block)
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	اگر (ma->entries[(unsigned)old].new == 0)
		بازگشت ENOENT;
	ma->entries[(unsigned)old].new = 0;
	بازگشت 0;
}

ایستا errcode_t bma_free(ext2_brel brel)
{
	ساختار brel_ma 	*ma;

	اگر (!brel)
		بازگشت 0;

	ma = brel->priv_data;

	اگر (ma) {
		اگر (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	اگر (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	بازگشت 0;
}
