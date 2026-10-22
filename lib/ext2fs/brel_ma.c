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

#अन्तर्भाव "config.h"
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जइ HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "brel.h"

स्थिर errcode_t bma_put(ext2_brel brel, blk64_t old,
			संरचना ext2_block_relocate_entry *ent);
स्थिर errcode_t bma_get(ext2_brel brel, blk64_t old,
			संरचना ext2_block_relocate_entry *ent);
स्थिर errcode_t bma_start_iter(ext2_brel brel);
स्थिर errcode_t bma_next(ext2_brel brel, blk64_t *old,
			 संरचना ext2_block_relocate_entry *ent);
स्थिर errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
स्थिर errcode_t bma_delete(ext2_brel brel, blk64_t old);
स्थिर errcode_t bma_free(ext2_brel brel);

संरचना brel_ma {
	__u32 magic;
	blk64_t max_block;
	संरचना ext2_block_relocate_entry *entries;
};

errcode_t ext2fs_brel_memarray_create(वर्ण *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	संरचना brel_ma 	*ma = 0;
	size_t		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(परिमाण(संरचना ext2_block_relocation_table),
				&brel);
	जइ (retval)
		गच्छ errout;
	memset(brel, 0, परिमाण(संरचना ext2_block_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &brel->name);
	जइ (retval)
		गच्छ errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(परिमाण(संरचना brel_ma), &ma);
	जइ (retval)
		गच्छ errout;
	memset(ma, 0, परिमाण(संरचना brel_ma));
	brel->priv_data = ma;

	size = (size_t) (परिमाण(संरचना ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		परिमाण(संरचना ext2_block_relocate_entry), &ma->entries);
	जइ (retval)
		गच्छ errout;
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
	brel->मुक्त = bma_free;

	*new_brel = brel;
	प्रत्यागम 0;

errout:
	bma_free(brel);
	प्रत्यागम retval;
}

स्थिर errcode_t bma_put(ext2_brel brel, blk64_t old,
			संरचना ext2_block_relocate_entry *ent)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_data;
	जइ (old > ma->max_block)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(unsigned)old] = *ent;
	प्रत्यागम 0;
}

स्थिर errcode_t bma_get(ext2_brel brel, blk64_t old,
			संरचना ext2_block_relocate_entry *ent)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_data;
	जइ (old > ma->max_block)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
	जइ (ma->entries[(unsigned)old].new == 0)
		प्रत्यागम ENOENT;
	*ent = ma->entries[old];
	प्रत्यागम 0;
}

स्थिर errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  संरचना ext2_block_relocate_entry *ent)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_data;
	जाव (++brel->current < ma->max_block) {
		जइ (ma->entries[(unsigned)brel->current].new == 0)
			अनुवर्त;
		*old = brel->current;
		*ent = ma->entries[(unsigned)brel->current];
		प्रत्यागम 0;
	}
	*old = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_data;
	जइ ((old > ma->max_block) || (new > ma->max_block))
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
	जइ (ma->entries[(unsigned)old].new == 0)
		प्रत्यागम ENOENT;
	ma->entries[(unsigned)new] = ma->entries[old];
	ma->entries[(unsigned)old].new = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_data;
	जइ (old > ma->max_block)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;
	जइ (ma->entries[(unsigned)old].new == 0)
		प्रत्यागम ENOENT;
	ma->entries[(unsigned)old].new = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t bma_free(ext2_brel brel)
{
	संरचना brel_ma 	*ma;

	जइ (!brel)
		प्रत्यागम 0;

	ma = brel->priv_data;

	जइ (ma) {
		जइ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	जइ (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	प्रत्यागम 0;
}
