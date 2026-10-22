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

#ضمن "config.h"
#ضمن <fcntl.h>
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "brel.h"

ساكن errcode_t bma_put(ext2_brel brel, blk64_t old,
			بنية ext2_بلاک_relocate_اندراج *ent);
ساكن errcode_t bma_get(ext2_brel brel, blk64_t old,
			بنية ext2_بلاک_relocate_اندراج *ent);
ساكن errcode_t bma_شروع_iter(ext2_brel brel);
ساكن errcode_t bma_اگلا(ext2_brel brel, blk64_t *old,
			 بنية ext2_بلاک_relocate_اندراج *ent);
ساكن errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
ساكن errcode_t bma_delete(ext2_brel brel, blk64_t old);
ساكن errcode_t bma_free(ext2_brel brel);

بنية brel_ma {
	__u32 magic;
	blk64_t max_بلاک;
	بنية ext2_بلاک_relocate_اندراج *entries;
};

errcode_t ext2fs_brel_memarray_create(حرف *name, blk64_t max_بلاک,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	بنية brel_ma 	*ma = 0;
	size_t		ناپ;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(حجم(بنية ext2_بلاک_relocation_جدول),
				&brel);
	إذا (retval)
		اذهب errout;
	memset(brel, 0, حجم(بنية ext2_بلاک_relocation_جدول));

	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &brel->name);
	إذا (retval)
		اذهب errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(حجم(بنية brel_ma), &ma);
	إذا (retval)
		اذهب errout;
	memset(ma, 0, حجم(بنية brel_ma));
	brel->priv_انکڑے = ma;

	ناپ = (size_t) (حجم(بنية ext2_بلاک_relocate_اندراج) *
			 (max_بلاک+1));
	retval = ext2fs_get_array(max_بلاک+1,
		حجم(بنية ext2_بلاک_relocate_اندراج), &ma->entries);
	إذا (retval)
		اذهب errout;
	memset(ma->entries, 0, ناپ);
	ma->max_بلاک = max_بلاک;

	/*
	 * Fill in the brel data structure
	 */
	brel->put = bma_put;
	brel->get = bma_get;
	brel->شروع_iter = bma_شروع_iter;
	brel->اگلا = bma_اگلا;
	brel->move = bma_move;
	brel->delete = bma_delete;
	brel->حرر = bma_free;

	*new_brel = brel;
	أرجع 0;

errout:
	bma_free(brel);
	أرجع retval;
}

ساكن errcode_t bma_put(ext2_brel brel, blk64_t old,
			بنية ext2_بلاک_relocate_اندراج *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_انکڑے;
	إذا (old > ma->max_بلاک)
		أرجع EXT2_ET_INVALID_دلیل;
	ma->entries[(بنانشانی)old] = *ent;
	أرجع 0;
}

ساكن errcode_t bma_get(ext2_brel brel, blk64_t old,
			بنية ext2_بلاک_relocate_اندراج *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_انکڑے;
	إذا (old > ma->max_بلاک)
		أرجع EXT2_ET_INVALID_دلیل;
	إذا (ma->entries[(بنانشانی)old].new == 0)
		أرجع ENOENT;
	*ent = ma->entries[old];
	أرجع 0;
}

ساكن errcode_t bma_شروع_iter(ext2_brel brel)
{
	brel->current = 0;
	أرجع 0;
}

ساكن errcode_t bma_اگلا(ext2_brel brel, blk64_t *old,
			  بنية ext2_بلاک_relocate_اندراج *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_انکڑے;
	طالما (++brel->current < ma->max_بلاک) {
		إذا (ma->entries[(بنانشانی)brel->current].new == 0)
			استمر;
		*old = brel->current;
		*ent = ma->entries[(بنانشانی)brel->current];
		أرجع 0;
	}
	*old = 0;
	أرجع 0;
}

ساكن errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_انکڑے;
	إذا ((old > ma->max_بلاک) || (new > ma->max_بلاک))
		أرجع EXT2_ET_INVALID_دلیل;
	إذا (ma->entries[(بنانشانی)old].new == 0)
		أرجع ENOENT;
	ma->entries[(بنانشانی)new] = ma->entries[old];
	ma->entries[(بنانشانی)old].new = 0;
	أرجع 0;
}

ساكن errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_انکڑے;
	إذا (old > ma->max_بلاک)
		أرجع EXT2_ET_INVALID_دلیل;
	إذا (ma->entries[(بنانشانی)old].new == 0)
		أرجع ENOENT;
	ma->entries[(بنانشانی)old].new = 0;
	أرجع 0;
}

ساكن errcode_t bma_free(ext2_brel brel)
{
	بنية brel_ma 	*ma;

	إذا (!brel)
		أرجع 0;

	ma = brel->priv_انکڑے;

	إذا (ma) {
		إذا (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	إذا (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	أرجع 0;
}
