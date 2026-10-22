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

#అన్తర్భావ "config.h"
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "brel.h"

స్థిర errcode_t bma_put(ext2_brel brel, blk64_t old,
			నిర్మాణం ext2_ఖండం_relocate_నమోదు *ent);
స్థిర errcode_t bma_get(ext2_brel brel, blk64_t old,
			నిర్మాణం ext2_ఖండం_relocate_నమోదు *ent);
స్థిర errcode_t bma_ప్రారంభం_iter(ext2_brel brel);
స్థిర errcode_t bma_తదుపరి(ext2_brel brel, blk64_t *old,
			 నిర్మాణం ext2_ఖండం_relocate_నమోదు *ent);
స్థిర errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
స్థిర errcode_t bma_delete(ext2_brel brel, blk64_t old);
స్థిర errcode_t bma_free(ext2_brel brel);

నిర్మాణం brel_ma {
	__u32 magic;
	blk64_t max_ఖండం;
	నిర్మాణం ext2_ఖండం_relocate_నమోదు *entries;
};

errcode_t ext2fs_brel_memarray_create(అక్షరం *name, blk64_t max_ఖండం,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	నిర్మాణం brel_ma 	*ma = 0;
	size_t		కొలత;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_ఖండం_relocation_పట్టిక),
				&brel);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	memset(brel, 0, పరిమాణం(నిర్మాణం ext2_ఖండం_relocation_పట్టిక));

	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(name)+1, &brel->name);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం brel_ma), &ma);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	memset(ma, 0, పరిమాణం(నిర్మాణం brel_ma));
	brel->priv_దత్తాంశం = ma;

	కొలత = (size_t) (పరిమాణం(నిర్మాణం ext2_ఖండం_relocate_నమోదు) *
			 (max_ఖండం+1));
	retval = ext2fs_get_array(max_ఖండం+1,
		పరిమాణం(నిర్మాణం ext2_ఖండం_relocate_నమోదు), &ma->entries);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	memset(ma->entries, 0, కొలత);
	ma->max_ఖండం = max_ఖండం;

	/*
	 * Fill in the brel data structure
	 */
	brel->put = bma_put;
	brel->get = bma_get;
	brel->ప్రారంభం_iter = bma_ప్రారంభం_iter;
	brel->తదుపరి = bma_తదుపరి;
	brel->move = bma_move;
	brel->delete = bma_delete;
	brel->విడుదల = bma_free;

	*new_brel = brel;
	తిరిగి 0;

errout:
	bma_free(brel);
	తిరిగి retval;
}

స్థిర errcode_t bma_put(ext2_brel brel, blk64_t old,
			నిర్మాణం ext2_ఖండం_relocate_నమోదు *ent)
{
	నిర్మాణం brel_ma 	*ma;

	ma = brel->priv_దత్తాంశం;
	ఒకవేళ (old > ma->max_ఖండం)
		తిరిగి EXT2_ET_INVALID_వాదం;
	ma->entries[(గుర్తులేని)old] = *ent;
	తిరిగి 0;
}

స్థిర errcode_t bma_get(ext2_brel brel, blk64_t old,
			నిర్మాణం ext2_ఖండం_relocate_నమోదు *ent)
{
	నిర్మాణం brel_ma 	*ma;

	ma = brel->priv_దత్తాంశం;
	ఒకవేళ (old > ma->max_ఖండం)
		తిరిగి EXT2_ET_INVALID_వాదం;
	ఒకవేళ (ma->entries[(గుర్తులేని)old].new == 0)
		తిరిగి ENOENT;
	*ent = ma->entries[old];
	తిరిగి 0;
}

స్థిర errcode_t bma_ప్రారంభం_iter(ext2_brel brel)
{
	brel->current = 0;
	తిరిగి 0;
}

స్థిర errcode_t bma_తదుపరి(ext2_brel brel, blk64_t *old,
			  నిర్మాణం ext2_ఖండం_relocate_నమోదు *ent)
{
	నిర్మాణం brel_ma 	*ma;

	ma = brel->priv_దత్తాంశం;
	అయితే (++brel->current < ma->max_ఖండం) {
		ఒకవేళ (ma->entries[(గుర్తులేని)brel->current].new == 0)
			కొనసాగు;
		*old = brel->current;
		*ent = ma->entries[(గుర్తులేని)brel->current];
		తిరిగి 0;
	}
	*old = 0;
	తిరిగి 0;
}

స్థిర errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	నిర్మాణం brel_ma 	*ma;

	ma = brel->priv_దత్తాంశం;
	ఒకవేళ ((old > ma->max_ఖండం) || (new > ma->max_ఖండం))
		తిరిగి EXT2_ET_INVALID_వాదం;
	ఒకవేళ (ma->entries[(గుర్తులేని)old].new == 0)
		తిరిగి ENOENT;
	ma->entries[(గుర్తులేని)new] = ma->entries[old];
	ma->entries[(గుర్తులేని)old].new = 0;
	తిరిగి 0;
}

స్థిర errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	నిర్మాణం brel_ma 	*ma;

	ma = brel->priv_దత్తాంశం;
	ఒకవేళ (old > ma->max_ఖండం)
		తిరిగి EXT2_ET_INVALID_వాదం;
	ఒకవేళ (ma->entries[(గుర్తులేని)old].new == 0)
		తిరిగి ENOENT;
	ma->entries[(గుర్తులేని)old].new = 0;
	తిరిగి 0;
}

స్థిర errcode_t bma_free(ext2_brel brel)
{
	నిర్మాణం brel_ma 	*ma;

	ఒకవేళ (!brel)
		తిరిగి 0;

	ma = brel->priv_దత్తాంశం;

	ఒకవేళ (ma) {
		ఒకవేళ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	ఒకవేళ (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	తిరిగి 0;
}
