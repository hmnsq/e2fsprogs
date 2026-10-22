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
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "brel.h"

ساكن errcode_t bma_put(ext2_brel brel, blk64_t old,
			بنية ext2_بلوک_relocate_مدخل *ent);
ساكن errcode_t bma_get(ext2_brel brel, blk64_t old,
			بنية ext2_بلوک_relocate_مدخل *ent);
ساكن errcode_t bma_شروع_iter(ext2_brel brel);
ساكن errcode_t bma_بعدی(ext2_brel brel, blk64_t *old,
			 بنية ext2_بلوک_relocate_مدخل *ent);
ساكن errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
ساكن errcode_t bma_delete(ext2_brel brel, blk64_t old);
ساكن errcode_t bma_free(ext2_brel brel);

بنية brel_ma {
	__u32 magic;
	blk64_t max_بلوک;
	بنية ext2_بلوک_relocate_مدخل *entries;
};

errcode_t ext2fs_brel_memarray_create(حرف *name, blk64_t max_بلوک,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	بنية brel_ma 	*ma = 0;
	size_t		اندازه;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(حجم(بنية ext2_بلوک_relocation_جدول),
				&brel);
	إذا (retval)
		اذهب errout;
	memset(brel, 0, حجم(بنية ext2_بلوک_relocation_جدول));

	retval = ext2fs_get_mem(طول_رشته(name)+1, &brel->name);
	إذا (retval)
		اذهب errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(حجم(بنية brel_ma), &ma);
	إذا (retval)
		اذهب errout;
	memset(ma, 0, حجم(بنية brel_ma));
	brel->priv_داده = ma;

	اندازه = (size_t) (حجم(بنية ext2_بلوک_relocate_مدخل) *
			 (max_بلوک+1));
	retval = ext2fs_get_array(max_بلوک+1,
		حجم(بنية ext2_بلوک_relocate_مدخل), &ma->entries);
	إذا (retval)
		اذهب errout;
	memset(ma->entries, 0, اندازه);
	ma->max_بلوک = max_بلوک;

	/*
	 * Fill in the brel data structure
	 */
	brel->put = bma_put;
	brel->get = bma_get;
	brel->شروع_iter = bma_شروع_iter;
	brel->بعدی = bma_بعدی;
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
			بنية ext2_بلوک_relocate_مدخل *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_داده;
	إذا (old > ma->max_بلوک)
		أرجع EXT2_ET_INVALID_آرگومان;
	ma->entries[(بیعلامت)old] = *ent;
	أرجع 0;
}

ساكن errcode_t bma_get(ext2_brel brel, blk64_t old,
			بنية ext2_بلوک_relocate_مدخل *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_داده;
	إذا (old > ma->max_بلوک)
		أرجع EXT2_ET_INVALID_آرگومان;
	إذا (ma->entries[(بیعلامت)old].new == 0)
		أرجع ENOENT;
	*ent = ma->entries[old];
	أرجع 0;
}

ساكن errcode_t bma_شروع_iter(ext2_brel brel)
{
	brel->current = 0;
	أرجع 0;
}

ساكن errcode_t bma_بعدی(ext2_brel brel, blk64_t *old,
			  بنية ext2_بلوک_relocate_مدخل *ent)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_داده;
	طالما (++brel->current < ma->max_بلوک) {
		إذا (ma->entries[(بیعلامت)brel->current].new == 0)
			استمر;
		*old = brel->current;
		*ent = ma->entries[(بیعلامت)brel->current];
		أرجع 0;
	}
	*old = 0;
	أرجع 0;
}

ساكن errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_داده;
	إذا ((old > ma->max_بلوک) || (new > ma->max_بلوک))
		أرجع EXT2_ET_INVALID_آرگومان;
	إذا (ma->entries[(بیعلامت)old].new == 0)
		أرجع ENOENT;
	ma->entries[(بیعلامت)new] = ma->entries[old];
	ma->entries[(بیعلامت)old].new = 0;
	أرجع 0;
}

ساكن errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	بنية brel_ma 	*ma;

	ma = brel->priv_داده;
	إذا (old > ma->max_بلوک)
		أرجع EXT2_ET_INVALID_آرگومان;
	إذا (ma->entries[(بیعلامت)old].new == 0)
		أرجع ENOENT;
	ma->entries[(بیعلامت)old].new = 0;
	أرجع 0;
}

ساكن errcode_t bma_free(ext2_brel brel)
{
	بنية brel_ma 	*ma;

	إذا (!brel)
		أرجع 0;

	ma = brel->priv_داده;

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
