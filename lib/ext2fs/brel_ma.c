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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "brel.h"

ಸ್ಥಿರ errcode_t bma_put(ext2_brel brel, blk64_t old,
			ಸಂರಚನಾ ext2_block_relocate_entry *ent);
ಸ್ಥಿರ errcode_t bma_get(ext2_brel brel, blk64_t old,
			ಸಂರಚನಾ ext2_block_relocate_entry *ent);
ಸ್ಥಿರ errcode_t bma_start_iter(ext2_brel brel);
ಸ್ಥಿರ errcode_t bma_next(ext2_brel brel, blk64_t *old,
			 ಸಂರಚನಾ ext2_block_relocate_entry *ent);
ಸ್ಥಿರ errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
ಸ್ಥಿರ errcode_t bma_delete(ext2_brel brel, blk64_t old);
ಸ್ಥಿರ errcode_t bma_free(ext2_brel brel);

ಸಂರಚನಾ brel_ma {
	__u32 magic;
	blk64_t max_block;
	ಸಂರಚನಾ ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(ವರ್ಣ *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	ಸಂರಚನಾ brel_ma 	*ma = 0;
	size_t		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_block_relocation_table),
				&brel);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	memset(brel, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ brel_ma), &ma);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	memset(ma, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ brel_ma));
	brel->priv_data = ma;

	size = (size_t) (ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_block_relocate_entry), &ma->entries);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
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
	brel->ಮುಕ್ತ = bma_free;

	*new_brel = brel;
	ಪ್ರತ್ಯಾಗಮ 0;

errout:
	bma_free(brel);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t bma_put(ext2_brel brel, blk64_t old,
			ಸಂರಚನಾ ext2_block_relocate_entry *ent)
{
	ಸಂರಚನಾ brel_ma 	*ma;

	ma = brel->priv_data;
	ಯದಿ (old > ma->max_block)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t bma_get(ext2_brel brel, blk64_t old,
			ಸಂರಚನಾ ext2_block_relocate_entry *ent)
{
	ಸಂರಚನಾ brel_ma 	*ma;

	ma = brel->priv_data;
	ಯದಿ (old > ma->max_block)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	ಯದಿ (ma->entries[(unsigned)old].new == 0)
		ಪ್ರತ್ಯಾಗಮ ENOENT;
	*ent = ma->entries[old];
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  ಸಂರಚನಾ ext2_block_relocate_entry *ent)
{
	ಸಂರಚನಾ brel_ma 	*ma;

	ma = brel->priv_data;
	ಯಾವತ್ (++brel->current < ma->max_block) {
		ಯದಿ (ma->entries[(unsigned)brel->current].new == 0)
			ಅನುವರ್ತ;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	*old = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	ಸಂರಚನಾ brel_ma 	*ma;

	ma = brel->priv_data;
	ಯದಿ ((old > ma->max_block) || (new > ma->max_block))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	ಯದಿ (ma->entries[(unsigned)old].new == 0)
		ಪ್ರತ್ಯಾಗಮ ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	ಸಂರಚನಾ brel_ma 	*ma;

	ma = brel->priv_data;
	ಯದಿ (old > ma->max_block)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	ಯದಿ (ma->entries[(unsigned)old].new == 0)
		ಪ್ರತ್ಯಾಗಮ ENOENT;
	ma->entries[(unsigned)old].new = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t bma_free(ext2_brel brel)
{
	ಸಂರಚನಾ brel_ma 	*ma;

	ಯದಿ (!brel)
		ಪ್ರತ್ಯಾಗಮ 0;

	ma = brel->priv_data;

	ಯದಿ (ma) {
		ಯದಿ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	ಯದಿ (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	ಪ್ರತ್ಯಾಗಮ 0;
}
