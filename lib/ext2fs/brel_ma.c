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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "brel.h"

स्थिर errcode_t bma_put(ext2_brel brel, blk64_t old,
			संरचना ext2_खण्डम्_relocate_प्रविष्टिः *ent);
स्थिर errcode_t bma_get(ext2_brel brel, blk64_t old,
			संरचना ext2_खण्डम्_relocate_प्रविष्टिः *ent);
स्थिर errcode_t bma_start_iter(ext2_brel brel);
स्थिर errcode_t bma_अग्र(ext2_brel brel, blk64_t *old,
			 संरचना ext2_खण्डम्_relocate_प्रविष्टिः *ent);
स्थिर errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
स्थिर errcode_t bma_delete(ext2_brel brel, blk64_t old);
स्थिर errcode_t bma_free(ext2_brel brel);

संरचना brel_ma {
	__u32 magic;
	blk64_t max_खण्डम्;
	संरचना ext2_खण्डम्_relocate_प्रविष्टिः *entries;
};

errcode_t ext2fs_brel_memarray_create(वर्ण *name, blk64_t max_खण्डम्,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	संरचना brel_ma 	*ma = 0;
	size_t		आकार;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(परिमाण(संरचना ext2_खण्डम्_relocation_सारणी),
				&brel);
	यदि (retval)
		गच्छ errout;
	स्मृतिं_पूरय(brel, 0, परिमाण(संरचना ext2_खण्डम्_relocation_सारणी));

	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(name)+1, &brel->name);
	यदि (retval)
		गच्छ errout;
	वाक्यं_प्रतिलिख(brel->name, name);

	retval = ext2fs_get_mem(परिमाण(संरचना brel_ma), &ma);
	यदि (retval)
		गच्छ errout;
	स्मृतिं_पूरय(ma, 0, परिमाण(संरचना brel_ma));
	brel->priv_दत्त = ma;

	आकार = (size_t) (परिमाण(संरचना ext2_खण्डम्_relocate_प्रविष्टिः) *
			 (max_खण्डम्+1));
	retval = ext2fs_get_array(max_खण्डम्+1,
		परिमाण(संरचना ext2_खण्डम्_relocate_प्रविष्टिः), &ma->entries);
	यदि (retval)
		गच्छ errout;
	स्मृतिं_पूरय(ma->entries, 0, आकार);
	ma->max_खण्डम् = max_खण्डम्;

	/*
	 * Fill in the brel data structure
	 */
	brel->put = bma_put;
	brel->get = bma_get;
	brel->start_iter = bma_start_iter;
	brel->अग्र = bma_अग्र;
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
			संरचना ext2_खण्डम्_relocate_प्रविष्टिः *ent)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_दत्त;
	यदि (old > ma->max_खण्डम्)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	ma->entries[(अचिह्नित)old] = *ent;
	प्रत्यागम 0;
}

स्थिर errcode_t bma_get(ext2_brel brel, blk64_t old,
			संरचना ext2_खण्डम्_relocate_प्रविष्टिः *ent)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_दत्त;
	यदि (old > ma->max_खण्डम्)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	यदि (ma->entries[(अचिह्नित)old].new == 0)
		प्रत्यागम ENOENT;
	*ent = ma->entries[old];
	प्रत्यागम 0;
}

स्थिर errcode_t bma_start_iter(ext2_brel brel)
{
	brel->current = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t bma_अग्र(ext2_brel brel, blk64_t *old,
			  संरचना ext2_खण्डम्_relocate_प्रविष्टिः *ent)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_दत्त;
	यावत् (++brel->current < ma->max_खण्डम्) {
		यदि (ma->entries[(अचिह्नित)brel->current].new == 0)
			अनुवर्त;
		*old = brel->current;
		*ent = ma->entries[(अचिह्नित)brel->current];
		प्रत्यागम 0;
	}
	*old = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_दत्त;
	यदि ((old > ma->max_खण्डम्) || (new > ma->max_खण्डम्))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	यदि (ma->entries[(अचिह्नित)old].new == 0)
		प्रत्यागम ENOENT;
	ma->entries[(अचिह्नित)new] = ma->entries[old];
	ma->entries[(अचिह्नित)old].new = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	संरचना brel_ma 	*ma;

	ma = brel->priv_दत्त;
	यदि (old > ma->max_खण्डम्)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	यदि (ma->entries[(अचिह्नित)old].new == 0)
		प्रत्यागम ENOENT;
	ma->entries[(अचिह्नित)old].new = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t bma_free(ext2_brel brel)
{
	संरचना brel_ma 	*ma;

	यदि (!brel)
		प्रत्यागम 0;

	ma = brel->priv_दत्त;

	यदि (ma) {
		यदि (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	यदि (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	प्रत्यागम 0;
}
