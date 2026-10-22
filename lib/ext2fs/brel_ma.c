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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "brel.h"

ସ୍ଥିର errcode_t bma_put(ext2_brel brel, blk64_t old,
			ସଂରଚନା ext2_block_relocate_entry *ent);
ସ୍ଥିର errcode_t bma_get(ext2_brel brel, blk64_t old,
			ସଂରଚନା ext2_block_relocate_entry *ent);
ସ୍ଥିର errcode_t bma_start_iter(ext2_brel brel);
ସ୍ଥିର errcode_t bma_next(ext2_brel brel, blk64_t *old,
			 ସଂରଚନା ext2_block_relocate_entry *ent);
ସ୍ଥିର errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
ସ୍ଥିର errcode_t bma_delete(ext2_brel brel, blk64_t old);
ସ୍ଥିର errcode_t bma_free(ext2_brel brel);

ସଂରଚନା brel_ma {
	__u32 magic;
	blk64_t max_block;
	ସଂରଚନା ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(ଅକ୍ଷର *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	ସଂରଚନା brel_ma 	*ma = 0;
	size_t		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_block_relocation_table),
				&brel);
	ଯଦି (retval)
		ଯାଅ errout;
	memset(brel, 0, ଆକାର(ସଂରଚନା ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	ଯଦି (retval)
		ଯାଅ errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା brel_ma), &ma);
	ଯଦି (retval)
		ଯାଅ errout;
	memset(ma, 0, ଆକାର(ସଂରଚନା brel_ma));
	brel->priv_data = ma;

	size = (size_t) (ଆକାର(ସଂରଚନା ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		ଆକାର(ସଂରଚନା ext2_block_relocate_entry), &ma->entries);
	ଯଦି (retval)
		ଯାଅ errout;
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
	brel->ମୁକ୍ତ = bma_free;

	*new_brel = brel;
	ଫେରନ୍ତୁ 0;

errout:
	bma_free(brel);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t bma_put(ext2_brel brel, blk64_t old,
			ସଂରଚନା ext2_block_relocate_entry *ent)
{
	ସଂରଚନା brel_ma 	*ma;

	ma = brel->priv_data;
	ଯଦି (old > ma->max_block)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t bma_get(ext2_brel brel, blk64_t old,
			ସଂରଚନା ext2_block_relocate_entry *ent)
{
	ସଂରଚନା brel_ma 	*ma;

	ma = brel->priv_data;
	ଯଦି (old > ma->max_block)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	ଯଦି (ma->entries[(unsigned)old].new == 0)
		ଫେରନ୍ତୁ ENOENT;
	*ent = ma->entries[old];
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  ସଂରଚନା ext2_block_relocate_entry *ent)
{
	ସଂରଚନା brel_ma 	*ma;

	ma = brel->priv_data;
	ଯେତେବେଳେ (++brel->current < ma->max_block) {
		ଯଦି (ma->entries[(unsigned)brel->current].new == 0)
			ଜାରି;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		ଫେରନ୍ତୁ 0;
	}
	*old = 0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	ସଂରଚନା brel_ma 	*ma;

	ma = brel->priv_data;
	ଯଦି ((old > ma->max_block) || (new > ma->max_block))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	ଯଦି (ma->entries[(unsigned)old].new == 0)
		ଫେରନ୍ତୁ ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	ସଂରଚନା brel_ma 	*ma;

	ma = brel->priv_data;
	ଯଦି (old > ma->max_block)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	ଯଦି (ma->entries[(unsigned)old].new == 0)
		ଫେରନ୍ତୁ ENOENT;
	ma->entries[(unsigned)old].new = 0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t bma_free(ext2_brel brel)
{
	ସଂରଚନା brel_ma 	*ma;

	ଯଦି (!brel)
		ଫେରନ୍ତୁ 0;

	ma = brel->priv_data;

	ଯଦି (ma) {
		ଯଦି (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	ଯଦି (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	ଫେରନ୍ତୁ 0;
}
