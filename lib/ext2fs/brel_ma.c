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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "brel.h"

ಸ್ಥಿರ errcode_t bma_put(ext2_brel brel, blk64_t old,
			ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು *ent);
ಸ್ಥಿರ errcode_t bma_get(ext2_brel brel, blk64_t old,
			ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು *ent);
ಸ್ಥಿರ errcode_t bma_ಆರಂಭ_iter(ext2_brel brel);
ಸ್ಥಿರ errcode_t bma_ಮುಂದಿನ(ext2_brel brel, blk64_t *old,
			 ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು *ent);
ಸ್ಥಿರ errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
ಸ್ಥಿರ errcode_t bma_delete(ext2_brel brel, blk64_t old);
ಸ್ಥಿರ errcode_t bma_free(ext2_brel brel);

ರಚನೆ brel_ma {
	__u32 magic;
	blk64_t max_ಖಂಡ;
	ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು *entries;
};

errcode_t ext2fs_brel_memarray_create(ಅಕ್ಷರ *name, blk64_t max_ಖಂಡ,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	ರಚನೆ brel_ma 	*ma = 0;
	size_t		ಗಾತ್ರ_2;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_ಖಂಡ_relocation_ಕೋಷ್ಟಕ),
				&brel);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	memset(brel, 0, ಗಾತ್ರ(ರಚನೆ ext2_ಖಂಡ_relocation_ಕೋಷ್ಟಕ));

	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)+1, &brel->name);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ brel_ma), &ma);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	memset(ma, 0, ಗಾತ್ರ(ರಚನೆ brel_ma));
	brel->priv_ದತ್ತಾಂಶ = ma;

	ಗಾತ್ರ_2 = (size_t) (ಗಾತ್ರ(ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು) *
			 (max_ಖಂಡ+1));
	retval = ext2fs_get_array(max_ಖಂಡ+1,
		ಗಾತ್ರ(ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು), &ma->entries);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	memset(ma->entries, 0, ಗಾತ್ರ_2);
	ma->max_ಖಂಡ = max_ಖಂಡ;

	/*
	 * Fill in the brel data structure
	 */
	brel->put = bma_put;
	brel->get = bma_get;
	brel->ಆರಂಭ_iter = bma_ಆರಂಭ_iter;
	brel->ಮುಂದಿನ = bma_ಮುಂದಿನ;
	brel->move = bma_move;
	brel->delete = bma_delete;
	brel->ಬಿಡುಗಡೆ = bma_free;

	*new_brel = brel;
	ಹಿಂತಿರುಗಿ 0;

errout:
	bma_free(brel);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t bma_put(ext2_brel brel, blk64_t old,
			ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು *ent)
{
	ರಚನೆ brel_ma 	*ma;

	ma = brel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ (old > ma->max_ಖಂಡ)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	ma->entries[(ಚಿಹ್ನೆರಹಿತ)old] = *ent;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t bma_get(ext2_brel brel, blk64_t old,
			ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು *ent)
{
	ರಚನೆ brel_ma 	*ma;

	ma = brel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ (old > ma->max_ಖಂಡ)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	ಆದರೆ (ma->entries[(ಚಿಹ್ನೆರಹಿತ)old].new == 0)
		ಹಿಂತಿರುಗಿ ENOENT;
	*ent = ma->entries[old];
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t bma_ಆರಂಭ_iter(ext2_brel brel)
{
	brel->current = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t bma_ಮುಂದಿನ(ext2_brel brel, blk64_t *old,
			  ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು *ent)
{
	ರಚನೆ brel_ma 	*ma;

	ma = brel->priv_ದತ್ತಾಂಶ;
	ಆಗ (++brel->current < ma->max_ಖಂಡ) {
		ಆದರೆ (ma->entries[(ಚಿಹ್ನೆರಹಿತ)brel->current].new == 0)
			ಮುಂದುವರಿ;
		*old = brel->current;
		*ent = ma->entries[(ಚಿಹ್ನೆರಹಿತ)brel->current];
		ಹಿಂತಿರುಗಿ 0;
	}
	*old = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	ರಚನೆ brel_ma 	*ma;

	ma = brel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ ((old > ma->max_ಖಂಡ) || (new > ma->max_ಖಂಡ))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	ಆದರೆ (ma->entries[(ಚಿಹ್ನೆರಹಿತ)old].new == 0)
		ಹಿಂತಿರುಗಿ ENOENT;
	ma->entries[(ಚಿಹ್ನೆರಹಿತ)new] = ma->entries[old];
	ma->entries[(ಚಿಹ್ನೆರಹಿತ)old].new = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	ರಚನೆ brel_ma 	*ma;

	ma = brel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ (old > ma->max_ಖಂಡ)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	ಆದರೆ (ma->entries[(ಚಿಹ್ನೆರಹಿತ)old].new == 0)
		ಹಿಂತಿರುಗಿ ENOENT;
	ma->entries[(ಚಿಹ್ನೆರಹಿತ)old].new = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t bma_free(ext2_brel brel)
{
	ರಚನೆ brel_ma 	*ma;

	ಆದರೆ (!brel)
		ಹಿಂತಿರುಗಿ 0;

	ma = brel->priv_ದತ್ತಾಂಶ;

	ಆದರೆ (ma) {
		ಆದರೆ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	ಆದರೆ (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	ಹಿಂತಿರುಗಿ 0;
}
