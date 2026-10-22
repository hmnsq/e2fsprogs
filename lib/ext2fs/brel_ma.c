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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "brel.h"

ਸਥਿਰ errcode_t bma_put(ext2_brel brel, blk64_t old,
			ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ *ent);
ਸਥਿਰ errcode_t bma_get(ext2_brel brel, blk64_t old,
			ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ *ent);
ਸਥਿਰ errcode_t bma_ਸ਼ੁਰੂ_iter(ext2_brel brel);
ਸਥਿਰ errcode_t bma_ਅਗਲਾ(ext2_brel brel, blk64_t *old,
			 ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ *ent);
ਸਥਿਰ errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new);
ਸਥਿਰ errcode_t bma_delete(ext2_brel brel, blk64_t old);
ਸਥਿਰ errcode_t bma_free(ext2_brel brel);

ਬਣਤਰ brel_ma {
	__u32 magic;
	blk64_t max_ਖੰਡ;
	ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ *entries;
};

errcode_t ext2fs_brel_memarray_create(ਅੱਖਰ *name, blk64_t max_ਖੰਡ,
				      ext2_brel *new_brel)
{
	ext2_brel		brel = 0;
	errcode_t	retval;
	ਬਣਤਰ brel_ma 	*ma = 0;
	size_t		ਮਾਪ;

	*new_brel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_ਖੰਡ_relocation_ਸਾਰਣੀ),
				&brel);
	ਜੇ (retval)
		ਜਾਓ errout;
	memset(brel, 0, ਆਕਾਰ(ਬਣਤਰ ext2_ਖੰਡ_relocation_ਸਾਰਣੀ));

	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(name)+1, &brel->name);
	ਜੇ (retval)
		ਜਾਓ errout;
	strcpy(brel->name, name);

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ brel_ma), &ma);
	ਜੇ (retval)
		ਜਾਓ errout;
	memset(ma, 0, ਆਕਾਰ(ਬਣਤਰ brel_ma));
	brel->priv_ਅੰਕੜੇ = ma;

	ਮਾਪ = (size_t) (ਆਕਾਰ(ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ) *
			 (max_ਖੰਡ+1));
	retval = ext2fs_get_array(max_ਖੰਡ+1,
		ਆਕਾਰ(ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ), &ma->entries);
	ਜੇ (retval)
		ਜਾਓ errout;
	memset(ma->entries, 0, ਮਾਪ);
	ma->max_ਖੰਡ = max_ਖੰਡ;

	/*
	 * Fill in the brel data structure
	 */
	brel->put = bma_put;
	brel->get = bma_get;
	brel->ਸ਼ੁਰੂ_iter = bma_ਸ਼ੁਰੂ_iter;
	brel->ਅਗਲਾ = bma_ਅਗਲਾ;
	brel->move = bma_move;
	brel->delete = bma_delete;
	brel->ਮੁਕਤ = bma_free;

	*new_brel = brel;
	ਵਾਪਸ 0;

errout:
	bma_free(brel);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t bma_put(ext2_brel brel, blk64_t old,
			ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ *ent)
{
	ਬਣਤਰ brel_ma 	*ma;

	ma = brel->priv_ਅੰਕੜੇ;
	ਜੇ (old > ma->max_ਖੰਡ)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ)old] = *ent;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t bma_get(ext2_brel brel, blk64_t old,
			ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ *ent)
{
	ਬਣਤਰ brel_ma 	*ma;

	ma = brel->priv_ਅੰਕੜੇ;
	ਜੇ (old > ma->max_ਖੰਡ)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	ਜੇ (ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ)old].new == 0)
		ਵਾਪਸ ENOENT;
	*ent = ma->entries[old];
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t bma_ਸ਼ੁਰੂ_iter(ext2_brel brel)
{
	brel->current = 0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t bma_ਅਗਲਾ(ext2_brel brel, blk64_t *old,
			  ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ *ent)
{
	ਬਣਤਰ brel_ma 	*ma;

	ma = brel->priv_ਅੰਕੜੇ;
	ਜਦੋਂ (++brel->current < ma->max_ਖੰਡ) {
		ਜੇ (ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ)brel->current].new == 0)
			ਜਾਰੀ;
		*old = brel->current;
		*ent = ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ)brel->current];
		ਵਾਪਸ 0;
	}
	*old = 0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t bma_move(ext2_brel brel, blk64_t old, blk64_t new)
{
	ਬਣਤਰ brel_ma 	*ma;

	ma = brel->priv_ਅੰਕੜੇ;
	ਜੇ ((old > ma->max_ਖੰਡ) || (new > ma->max_ਖੰਡ))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	ਜੇ (ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ)old].new == 0)
		ਵਾਪਸ ENOENT;
	ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ)new] = ma->entries[old];
	ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ)old].new = 0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t bma_delete(ext2_brel brel, blk64_t old)
{
	ਬਣਤਰ brel_ma 	*ma;

	ma = brel->priv_ਅੰਕੜੇ;
	ਜੇ (old > ma->max_ਖੰਡ)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	ਜੇ (ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ)old].new == 0)
		ਵਾਪਸ ENOENT;
	ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ)old].new = 0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t bma_free(ext2_brel brel)
{
	ਬਣਤਰ brel_ma 	*ma;

	ਜੇ (!brel)
		ਵਾਪਸ 0;

	ma = brel->priv_ਅੰਕੜੇ;

	ਜੇ (ma) {
		ਜੇ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ext2fs_free_mem(&ma);
	}
	ਜੇ (brel->name)
		ext2fs_free_mem(&brel->name);
	ext2fs_free_mem(&brel);
	ਵਾਪਸ 0;
}
