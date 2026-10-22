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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "brel.h"

স্থির errcode_t bma_put(ext2_brel brel, blk64_t old,
			সংরচনা ext2_block_relocate_entry *ent);
স্থির errcode_t bma_get(ext2_brel brel, blk64_t old,
			সংরচনা ext2_block_relocate_entry *ent);
স্থির errcode_t bma_start_iter(ext2_brel brel);
স্থির errcode_t bma_next(ext2_brel brel, blk64_t *old,
			 সংরচনা ext2_block_relocate_entry *ent);
স্থির errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
স্থির errcode_t bma_delete(ext2_brel brel, blk64_t old);
স্থির errcode_t bma_free(ext2_brel brel);

সংরচনা brel_ma {
	__u32 magic;
	blk64_t max_block;
	সংরচনা ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(ৱর্ণ *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	সংরচনা brel_ma 	*ma = 0;
	size_t		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(পরিমাণ(সংরচনা ext2_block_relocation_table),
				&brel);
	যদি (retval)
		গচ্ছ errout;
	memset(brel, 0, পরিমাণ(সংরচনা ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	যদি (retval)
		গচ্ছ errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা brel_ma), &ma);
	যদি (retval)
		গচ্ছ errout;
	memset(ma, 0, পরিমাণ(সংরচনা brel_ma));
	brel->priv_data = ma;

	size = (size_t) (পরিমাণ(সংরচনা ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		পরিমাণ(সংরচনা ext2_block_relocate_entry), &ma->entries);
	যদি (retval)
		গচ্ছ errout;
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
	brel->মুক্ত = bma_free;

	*new_brel = brel;
	ঘূৰাই 0;

errout:
	bma_free(brel);
	ঘূৰাই retval;
}

স্থির errcode_t bma_put(ext2_brel brel, blk64_t old,
			সংরচনা ext2_block_relocate_entry *ent)
{
	সংরচনা brel_ma 	*ma;

	ma = brel->priv_data;
	যদি (old > ma->max_block)
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	ঘূৰাই 0;
}

স্থির errcode_t bma_get(ext2_brel brel, blk64_t old,
			সংরচনা ext2_block_relocate_entry *ent)
{
	সংরচনা brel_ma 	*ma;

	ma = brel->priv_data;
	যদি (old > ma->max_block)
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
	যদি (ma->entries[(unsigned)old].new == 0)
		ঘূৰাই ENOENT;
	*ent = ma->entries[old];
	ঘূৰাই 0;
}

স্থির errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	ঘূৰাই 0;
}

স্থির errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  সংরচনা ext2_block_relocate_entry *ent)
{
	সংরচনা brel_ma 	*ma;

	ma = brel->priv_data;
	যেতিয়ালৈ (++brel->current < ma->max_block) {
		যদি (ma->entries[(unsigned)brel->current].new == 0)
			অনুৱর্ত;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		ঘূৰাই 0;
	}
	*old = 0;
	ঘূৰাই 0;
}

স্থির errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	সংরচনা brel_ma 	*ma;

	ma = brel->priv_data;
	যদি ((old > ma->max_block) || (new > ma->max_block))
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
	যদি (ma->entries[(unsigned)old].new == 0)
		ঘূৰাই ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	ঘূৰাই 0;
}

স্থির errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	সংরচনা brel_ma 	*ma;

	ma = brel->priv_data;
	যদি (old > ma->max_block)
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;
	যদি (ma->entries[(unsigned)old].new == 0)
		ঘূৰাই ENOENT;
	ma->entries[(unsigned)old].new = 0;
	ঘূৰাই 0;
}

স্থির errcode_t bma_free(ext2_brel brel)
{
	সংরচনা brel_ma 	*ma;

	যদি (!brel)
		ঘূৰাই 0;

	ma = brel->priv_data;

	যদি (ma) {
		যদি (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	যদি (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	ঘূৰাই 0;
}
