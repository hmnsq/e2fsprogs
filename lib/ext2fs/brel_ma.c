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
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#إذا HAVE_ERRNO_H
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
	إذا (retval)
		اذهب errout;
	memset(brel, 0, حجم(بنية ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	إذا (retval)
		اذهب errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(حجم(بنية brel_ma), &ma);
	إذا (retval)
		اذهب errout;
	memset(ma, 0, حجم(بنية brel_ma));
	brel->priv_data = ma;

	size = (size_t) (حجم(بنية ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		حجم(بنية ext2_block_relocate_entry), &ma->entries);
	إذا (retval)
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
	إرجاع 0;

errout:
	bma_free(brel);
	إرجاع retval;
}

ساكن errcode_t bma_put(ext2_brel brel, blk64_t old,
			بنية ext2_block_relocate_entry *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_data;
	إذا (old > ma->max_block)
		إرجاع EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	إرجاع 0;
}

ساكن errcode_t bma_get(ext2_brel brel, blk64_t old,
			بنية ext2_block_relocate_entry *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_data;
	إذا (old > ma->max_block)
		إرجاع EXT2_ET_INVALID_ARGUMENT;
	إذا (ma->entries[(unsigned)old].new == 0)
		إرجاع ENOENT;
	*ent = ma->entries[old];
	إرجاع 0;
}

ساكن errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	إرجاع 0;
}

ساكن errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  بنية ext2_block_relocate_entry *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_data;
	بينما (++brel->current < ma->max_block) {
		إذا (ma->entries[(unsigned)brel->current].new == 0)
			استمر;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		إرجاع 0;
	}
	*old = 0;
	إرجاع 0;
}

ساكن errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_data;
	إذا ((old > ma->max_block) || (new > ma->max_block))
		إرجاع EXT2_ET_INVALID_ARGUMENT;
	إذا (ma->entries[(unsigned)old].new == 0)
		إرجاع ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	إرجاع 0;
}

ساكن errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_data;
	إذا (old > ma->max_block)
		إرجاع EXT2_ET_INVALID_ARGUMENT;
	إذا (ma->entries[(unsigned)old].new == 0)
		إرجاع ENOENT;
	ma->entries[(unsigned)old].new = 0;
	إرجاع 0;
}

ساكن errcode_t bma_free(ext2_brel brel)
{
	بنية brel_ma 	*ma;

	إذا (!brel)
		إرجاع 0;

	ma = brel->priv_data;

	إذا (ma) {
		إذا (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	إذا (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	إرجاع 0;
}
