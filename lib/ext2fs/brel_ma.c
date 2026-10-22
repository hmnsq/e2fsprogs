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

#شامل "config.h"
#شامل <fcntl.h>
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "brel.h"

ساکن errcode_t bma_put(ext2_brel brel, blk64_t old,
			ساخت ext2_بلاک_relocate_اندراج *ent);
ساکن errcode_t bma_get(ext2_brel brel, blk64_t old,
			ساخت ext2_بلاک_relocate_اندراج *ent);
ساکن errcode_t bma_آغاز_iter(ext2_brel brel);
ساکن errcode_t bma_اگلا(ext2_brel brel, blk64_t *old,
			 ساخت ext2_بلاک_relocate_اندراج *ent);
ساکن errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
ساکن errcode_t bma_delete(ext2_brel brel, blk64_t old);
ساکن errcode_t bma_free(ext2_brel brel);

ساخت brel_ma {
	__u32 magic;
	blk64_t max_بلاک;
	ساخت ext2_بلاک_relocate_اندراج *entries;
};

errcode_t ext2fs_brel_memarray_create(حرف *name, blk64_t max_بلاک,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	ساخت brel_ma 	*ma = 0;
	size_t		سائز;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(حجم(ساخت ext2_بلاک_relocation_جدول),
				&brel);
	اگر (retval)
		جاؤ errout;
	memset(brel, 0, حجم(ساخت ext2_بلاک_relocation_جدول));

	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &brel->name);
	اگر (retval)
		جاؤ errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(حجم(ساخت brel_ma), &ma);
	اگر (retval)
		جاؤ errout;
	memset(ma, 0, حجم(ساخت brel_ma));
	brel->priv_ڈیٹا = ma;

	سائز = (size_t) (حجم(ساخت ext2_بلاک_relocate_اندراج) *
			 (max_بلاک+1));
	retval = ext2fs_get_array(max_بلاک+1,
		حجم(ساخت ext2_بلاک_relocate_اندراج), &ma->entries);
	اگر (retval)
		جاؤ errout;
	memset(ma->entries, 0, سائز);
	ma->max_بلاک = max_بلاک;

	/*
	 * Fill in the brel data structure
	 */
	brel->put = bma_put;
	brel->get = bma_get;
	brel->آغاز_iter = bma_آغاز_iter;
	brel->اگلا = bma_اگلا;
	brel->move = bma_move;
	brel->delete = bma_delete;
	brel->آزاد = bma_free;

	*new_brel = brel;
	واپس 0;

errout:
	bma_free(brel);
	واپس retval;
}

ساکن errcode_t bma_put(ext2_brel brel, blk64_t old,
			ساخت ext2_بلاک_relocate_اندراج *ent)
{
	ساخت brel_ma 	*ma;

	ma = brel->priv_ڈیٹا;
	اگر (old > ma->max_بلاک)
		واپس EXT2_ET_INVALID_دلیل;
	ma->entries[(غیرعلامتی)old] = *ent;
	واپس 0;
}

ساکن errcode_t bma_get(ext2_brel brel, blk64_t old,
			ساخت ext2_بلاک_relocate_اندراج *ent)
{
	ساخت brel_ma 	*ma;

	ma = brel->priv_ڈیٹا;
	اگر (old > ma->max_بلاک)
		واپس EXT2_ET_INVALID_دلیل;
	اگر (ma->entries[(غیرعلامتی)old].new == 0)
		واپس ENOENT;
	*ent = ma->entries[old];
	واپس 0;
}

ساکن errcode_t bma_آغاز_iter(ext2_brel brel)
{
	brel->current = 0;
	واپس 0;
}

ساکن errcode_t bma_اگلا(ext2_brel brel, blk64_t *old,
			  ساخت ext2_بلاک_relocate_اندراج *ent)
{
	ساخت brel_ma 	*ma;

	ma = brel->priv_ڈیٹا;
	جب (++brel->current < ma->max_بلاک) {
		اگر (ma->entries[(غیرعلامتی)brel->current].new == 0)
			جاری;
		*old = brel->current;
		*ent = ma->entries[(غیرعلامتی)brel->current];
		واپس 0;
	}
	*old = 0;
	واپس 0;
}

ساکن errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	ساخت brel_ma 	*ma;

	ma = brel->priv_ڈیٹا;
	اگر ((old > ma->max_بلاک) || (new > ma->max_بلاک))
		واپس EXT2_ET_INVALID_دلیل;
	اگر (ma->entries[(غیرعلامتی)old].new == 0)
		واپس ENOENT;
	ma->entries[(غیرعلامتی)new] = ma->entries[old];
	ma->entries[(غیرعلامتی)old].new = 0;
	واپس 0;
}

ساکن errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	ساخت brel_ma 	*ma;

	ma = brel->priv_ڈیٹا;
	اگر (old > ma->max_بلاک)
		واپس EXT2_ET_INVALID_دلیل;
	اگر (ma->entries[(غیرعلامتی)old].new == 0)
		واپس ENOENT;
	ma->entries[(غیرعلامتی)old].new = 0;
	واپس 0;
}

ساکن errcode_t bma_free(ext2_brel brel)
{
	ساخت brel_ma 	*ma;

	اگر (!brel)
		واپس 0;

	ma = brel->priv_ڈیٹا;

	اگر (ma) {
		اگر (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	اگر (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	واپس 0;
}
