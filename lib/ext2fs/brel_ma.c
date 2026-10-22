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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "brel.h"

স্থির errcode_t bma_put(ext2_brel brel, blk64_t old,
			কাঠামো ext2_block_relocate_entry *ent);
স্থির errcode_t bma_get(ext2_brel brel, blk64_t old,
			কাঠামো ext2_block_relocate_entry *ent);
স্থির errcode_t bma_start_iter(ext2_brel brel);
স্থির errcode_t bma_পরবর্তী(ext2_brel brel, blk64_t *old,
			 কাঠামো ext2_block_relocate_entry *ent);
স্থির errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
স্থির errcode_t bma_delete(ext2_brel brel, blk64_t old);
স্থির errcode_t bma_free(ext2_brel brel);

কাঠামো brel_ma {
	__u32 magic;
	blk64_t max_block;
	কাঠামো ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(অক্ষর *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	কাঠামো brel_ma 	*ma = 0;
	size_t		আকার_2;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(আকার(কাঠামো ext2_block_relocation_table),
				&brel);
	যদি (retval)
		যাও errout;
	memset(brel, 0, আকার(কাঠামো ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	যদি (retval)
		যাও errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(আকার(কাঠামো brel_ma), &ma);
	যদি (retval)
		যাও errout;
	memset(ma, 0, আকার(কাঠামো brel_ma));
	brel->priv_তথ্য = ma;

	আকার_2 = (size_t) (আকার(কাঠামো ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		আকার(কাঠামো ext2_block_relocate_entry), &ma->entries);
	যদি (retval)
		যাও errout;
	memset(ma->entries, 0, আকার_2);
	ma->max_block = max_block;

	/*
	 * Fill in the brel data structure
	 */
	brel->put = bma_put;
	brel->get = bma_get;
	brel->start_iter = bma_start_iter;
	brel->পরবর্তী = bma_পরবর্তী;
	brel->move = bma_move;
	brel->delete = bma_delete;
	brel->মুক্ত = bma_free;

	*new_brel = brel;
	ফেরত 0;

errout:
	bma_free(brel);
	ফেরত retval;
}

স্থির errcode_t bma_put(ext2_brel brel, blk64_t old,
			কাঠামো ext2_block_relocate_entry *ent)
{
	কাঠামো brel_ma 	*ma;

	ma = brel->priv_তথ্য;
	যদি (old > ma->max_block)
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	ফেরত 0;
}

স্থির errcode_t bma_get(ext2_brel brel, blk64_t old,
			কাঠামো ext2_block_relocate_entry *ent)
{
	কাঠামো brel_ma 	*ma;

	ma = brel->priv_তথ্য;
	যদি (old > ma->max_block)
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	যদি (ma->entries[(unsigned)old].new == 0)
		ফেরত ENOENT;
	*ent = ma->entries[old];
	ফেরত 0;
}

স্থির errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	ফেরত 0;
}

স্থির errcode_t bma_পরবর্তী(ext2_brel brel, blk64_t *old,
			  কাঠামো ext2_block_relocate_entry *ent)
{
	কাঠামো brel_ma 	*ma;

	ma = brel->priv_তথ্য;
	যখন (++brel->current < ma->max_block) {
		যদি (ma->entries[(unsigned)brel->current].new == 0)
			চালু;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		ফেরত 0;
	}
	*old = 0;
	ফেরত 0;
}

স্থির errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	কাঠামো brel_ma 	*ma;

	ma = brel->priv_তথ্য;
	যদি ((old > ma->max_block) || (new > ma->max_block))
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	যদি (ma->entries[(unsigned)old].new == 0)
		ফেরত ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	ফেরত 0;
}

স্থির errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	কাঠামো brel_ma 	*ma;

	ma = brel->priv_তথ্য;
	যদি (old > ma->max_block)
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	যদি (ma->entries[(unsigned)old].new == 0)
		ফেরত ENOENT;
	ma->entries[(unsigned)old].new = 0;
	ফেরত 0;
}

স্থির errcode_t bma_free(ext2_brel brel)
{
	কাঠামো brel_ma 	*ma;

	যদি (!brel)
		ফেরত 0;

	ma = brel->priv_তথ্য;

	যদি (ma) {
		যদি (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	যদি (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	ফেরত 0;
}
