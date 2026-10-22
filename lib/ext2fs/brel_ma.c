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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "brel.h"

സ്ഥിതം errcode_t bma_put(ext2_brel brel, blk64_t old,
			ഘടന ext2_block_relocate_entry *ent);
സ്ഥിതം errcode_t bma_get(ext2_brel brel, blk64_t old,
			ഘടന ext2_block_relocate_entry *ent);
സ്ഥിതം errcode_t bma_start_iter(ext2_brel brel);
സ്ഥിതം errcode_t bma_next(ext2_brel brel, blk64_t *old,
			 ഘടന ext2_block_relocate_entry *ent);
സ്ഥിതം errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
സ്ഥിതം errcode_t bma_delete(ext2_brel brel, blk64_t old);
സ്ഥിതം errcode_t bma_free(ext2_brel brel);

ഘടന brel_ma {
	__u32 magic;
	blk64_t max_block;
	ഘടന ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(അക്ഷരം *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	ഘടന brel_ma 	*ma = 0;
	size_t		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_block_relocation_table),
				&brel);
	എങ്കിൽ (retval)
		പോകുക errout;
	memset(brel, 0, വലുപ്പം(ഘടന ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	എങ്കിൽ (retval)
		പോകുക errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന brel_ma), &ma);
	എങ്കിൽ (retval)
		പോകുക errout;
	memset(ma, 0, വലുപ്പം(ഘടന brel_ma));
	brel->priv_data = ma;

	size = (size_t) (വലുപ്പം(ഘടന ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		വലുപ്പം(ഘടന ext2_block_relocate_entry), &ma->entries);
	എങ്കിൽ (retval)
		പോകുക errout;
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
	brel->സ്വതന്ത്രം = bma_free;

	*new_brel = brel;
	തിരികെ 0;

errout:
	bma_free(brel);
	തിരികെ retval;
}

സ്ഥിതം errcode_t bma_put(ext2_brel brel, blk64_t old,
			ഘടന ext2_block_relocate_entry *ent)
{
	ഘടന brel_ma 	*ma;

	ma = brel->priv_data;
	എങ്കിൽ (old > ma->max_block)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	തിരികെ 0;
}

സ്ഥിതം errcode_t bma_get(ext2_brel brel, blk64_t old,
			ഘടന ext2_block_relocate_entry *ent)
{
	ഘടന brel_ma 	*ma;

	ma = brel->priv_data;
	എങ്കിൽ (old > ma->max_block)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	എങ്കിൽ (ma->entries[(unsigned)old].new == 0)
		തിരികെ ENOENT;
	*ent = ma->entries[old];
	തിരികെ 0;
}

സ്ഥിതം errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	തിരികെ 0;
}

സ്ഥിതം errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  ഘടന ext2_block_relocate_entry *ent)
{
	ഘടന brel_ma 	*ma;

	ma = brel->priv_data;
	ആകുമ്പോൾ (++brel->current < ma->max_block) {
		എങ്കിൽ (ma->entries[(unsigned)brel->current].new == 0)
			തുടരുക;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		തിരികെ 0;
	}
	*old = 0;
	തിരികെ 0;
}

സ്ഥിതം errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	ഘടന brel_ma 	*ma;

	ma = brel->priv_data;
	എങ്കിൽ ((old > ma->max_block) || (new > ma->max_block))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	എങ്കിൽ (ma->entries[(unsigned)old].new == 0)
		തിരികെ ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	തിരികെ 0;
}

സ്ഥിതം errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	ഘടന brel_ma 	*ma;

	ma = brel->priv_data;
	എങ്കിൽ (old > ma->max_block)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	എങ്കിൽ (ma->entries[(unsigned)old].new == 0)
		തിരികെ ENOENT;
	ma->entries[(unsigned)old].new = 0;
	തിരികെ 0;
}

സ്ഥിതം errcode_t bma_free(ext2_brel brel)
{
	ഘടന brel_ma 	*ma;

	എങ്കിൽ (!brel)
		തിരികെ 0;

	ma = brel->priv_data;

	എങ്കിൽ (ma) {
		എങ്കിൽ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	എങ്കിൽ (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	തിരികെ 0;
}
