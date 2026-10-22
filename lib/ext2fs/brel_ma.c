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

#include "config.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#ᤀᤡ᤺ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ᤀᤡ᤺ HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "brel.h"

static errcode_t bma_put(ext2_brel brel, blk64_t old,
			sa_nracanaa ext2_block_relocate_entry *ent);
static errcode_t bma_get(ext2_brel brel, blk64_t old,
			sa_nracanaa ext2_block_relocate_entry *ent);
static errcode_t bma_start_iter(ext2_brel brel);
static errcode_t bma_next(ext2_brel brel, blk64_t *old,
			 sa_nracanaa ext2_block_relocate_entry *ent);
static errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
static errcode_t bma_delete(ext2_brel brel, blk64_t old);
static errcode_t bma_free(ext2_brel brel);

sa_nracanaa brel_ma {
	__u32 magic;
	blk64_t max_block;
	sa_nracanaa ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(var_na *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	sa_nracanaa brel_ma 	*ma = 0;
	size_t		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_block_relocation_table),
				&brel);
	ᤀᤡ᤺ (retval)
		goto errout;
	memset(brel, 0, parimaa_na(sa_nracanaa ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	ᤀᤡ᤺ (retval)
		goto errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa brel_ma), &ma);
	ᤀᤡ᤺ (retval)
		goto errout;
	memset(ma, 0, parimaa_na(sa_nracanaa brel_ma));
	brel->priv_data = ma;

	size = (size_t) (parimaa_na(sa_nracanaa ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		parimaa_na(sa_nracanaa ext2_block_relocate_entry), &ma->entries);
	ᤀᤡ᤺ (retval)
		goto errout;
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
	brel->free = bma_free;

	*new_brel = brel;
	ᤀᤡᤴ 0;

errout:
	bma_free(brel);
	ᤀᤡᤴ retval;
}

static errcode_t bma_put(ext2_brel brel, blk64_t old,
			sa_nracanaa ext2_block_relocate_entry *ent)
{
	sa_nracanaa brel_ma 	*ma;

	ma = brel->priv_data;
	ᤀᤡ᤺ (old > ma->max_block)
		ᤀᤡᤴ EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	ᤀᤡᤴ 0;
}

static errcode_t bma_get(ext2_brel brel, blk64_t old,
			sa_nracanaa ext2_block_relocate_entry *ent)
{
	sa_nracanaa brel_ma 	*ma;

	ma = brel->priv_data;
	ᤀᤡ᤺ (old > ma->max_block)
		ᤀᤡᤴ EXT2_ET_INVALID_ARGUMENT;
	ᤀᤡ᤺ (ma->entries[(unsigned)old].new == 0)
		ᤀᤡᤴ ENOENT;
	*ent = ma->entries[old];
	ᤀᤡᤴ 0;
}

static errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	ᤀᤡᤴ 0;
}

static errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  sa_nracanaa ext2_block_relocate_entry *ent)
{
	sa_nracanaa brel_ma 	*ma;

	ma = brel->priv_data;
	ᤀᤠ᤺ (++brel->current < ma->max_block) {
		ᤀᤡ᤺ (ma->entries[(unsigned)brel->current].new == 0)
			continue;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		ᤀᤡᤴ 0;
	}
	*old = 0;
	ᤀᤡᤴ 0;
}

static errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	sa_nracanaa brel_ma 	*ma;

	ma = brel->priv_data;
	ᤀᤡ᤺ ((old > ma->max_block) || (new > ma->max_block))
		ᤀᤡᤴ EXT2_ET_INVALID_ARGUMENT;
	ᤀᤡ᤺ (ma->entries[(unsigned)old].new == 0)
		ᤀᤡᤴ ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	ᤀᤡᤴ 0;
}

static errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	sa_nracanaa brel_ma 	*ma;

	ma = brel->priv_data;
	ᤀᤡ᤺ (old > ma->max_block)
		ᤀᤡᤴ EXT2_ET_INVALID_ARGUMENT;
	ᤀᤡ᤺ (ma->entries[(unsigned)old].new == 0)
		ᤀᤡᤴ ENOENT;
	ma->entries[(unsigned)old].new = 0;
	ᤀᤡᤴ 0;
}

static errcode_t bma_free(ext2_brel brel)
{
	sa_nracanaa brel_ma 	*ma;

	ᤀᤡ᤺ (!brel)
		ᤀᤡᤴ 0;

	ma = brel->priv_data;

	ᤀᤡ᤺ (ma) {
		ᤀᤡ᤺ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	ᤀᤡ᤺ (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	ᤀᤡᤴ 0;
}
