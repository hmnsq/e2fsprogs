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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
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

errcode_t ext2fs_brel_memarray_create(अक्षर *name, blk64_t max_block,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	संरचना brel_ma 	*ma = 0;
	माप_प्रकार		size;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(आकार(संरचना ext2_block_relocation_table),
				&brel);
	जर (retval)
		जा errout;
	स_रखो(brel, 0, आकार(संरचना ext2_block_relocation_table));

	retval = ext2fs_get_mem(म_माप(name)+1, &brel->name);
	जर (retval)
		जा errout;
	म_नकल(brel->name, name);

	retval = ext2fs_get_mem(आकार(संरचना brel_ma), &ma);
	जर (retval)
		जा errout;
	स_रखो(ma, 0, आकार(संरचना brel_ma));
	brel->priv_data = ma;

	size = (माप_प्रकार) (आकार(संरचना ext2_block_relocate_entry) *
			 (max_block+1));
	retval = ext2fs_get_array(max_block+1,
		आकार(संरचना ext2_block_relocate_entry), &ma->entries);
	जर (retval)
		जा errout;
	स_रखो(ma->entries, 0, size);
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
	परत 0;

errout:
	bma_free(brel);
	परत retval;
}

स्थिर errcode_t bma_put(ext2_brel brel, blk64_t old,
			संरचना ext2_block_relocate_entry *ent)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_data;
	जर (old > ma->max_block)
		परत EXT2_ET_INVALID_ARGUMENT;
	ma->entries[(अचिन्हित)old] = *ent;
	परत 0;
}

स्थिर errcode_t bma_get(ext2_brel brel, blk64_t old,
			संरचना ext2_block_relocate_entry *ent)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_data;
	जर (old > ma->max_block)
		परत EXT2_ET_INVALID_ARGUMENT;
	जर (ma->entries[(अचिन्हित)old].new == 0)
		परत ENOENT;
	*ent = ma->entries[old];
	परत 0;
}

स्थिर errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	परत 0;
}

स्थिर errcode_t bma_next(ext2_brel brel, blk64_t *old,
			  संरचना ext2_block_relocate_entry *ent)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_data;
	पर्यंत (++brel->current < ma->max_block) {
		जर (ma->entries[(अचिन्हित)brel->current].new == 0)
			सुरूठेवा;
		*old = brel->current;
		*ent = ma->entries[(अचिन्हित)brel->current];
		परत 0;
	}
	*old = 0;
	परत 0;
}

स्थिर errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_data;
	जर ((old > ma->max_block) || (new > ma->max_block))
		परत EXT2_ET_INVALID_ARGUMENT;
	जर (ma->entries[(अचिन्हित)old].new == 0)
		परत ENOENT;
	ma->entries[(अचिन्हित)new] = ma->entries[old];
	ma->entries[(अचिन्हित)old].new = 0;
	परत 0;
}

स्थिर errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_data;
	जर (old > ma->max_block)
		परत EXT2_ET_INVALID_ARGUMENT;
	जर (ma->entries[(अचिन्हित)old].new == 0)
		परत ENOENT;
	ma->entries[(अचिन्हित)old].new = 0;
	परत 0;
}

स्थिर errcode_t bma_free(ext2_brel brel)
{
	संरचना brel_ma 	*ma;

	जर (!brel)
		परत 0;

	ma = brel->priv_data;

	जर (ma) {
		जर (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	जर (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	परत 0;
}
