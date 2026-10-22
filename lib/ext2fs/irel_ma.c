/*
 * irel_ma.c
 *
 * Copyright (C) 1996, 1997 Theodore Ts'o.
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
#ਅਨ੍ਤਰ੍ਭਾਵ "irel.h"

ਸਥਿਰ errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ *ent);
ਸਥਿਰ errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ *ent);
ਸਥਿਰ errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ *ent);
ਸਥਿਰ errcode_t ima_ਸ਼ੁਰੂ_iter(ext2_irel irel);
ਸਥਿਰ errcode_t ima_ਅਗਲਾ(ext2_irel irel, ext2_ino_t *old,
			  ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ *ent);
ਸਥਿਰ errcode_t ima_ਜੋੜੋ_ref(ext2_irel irel, ext2_ino_t ino,
			     ਬਣਤਰ ext2_inode_reference *ref);
ਸਥਿਰ errcode_t ima_ਸ਼ੁਰੂ_iter_ref(ext2_irel irel, ext2_ino_t ino);
ਸਥਿਰ errcode_t ima_ਅਗਲਾ_ref(ext2_irel irel, ਬਣਤਰ ext2_inode_reference *ref);
ਸਥਿਰ errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
ਸਥਿਰ errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
ਸਥਿਰ errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
ਬਣਤਰ inode_reference_ਇੰਦਰਾਜ {
	__u16 ਅੰਕ;
	ਬਣਤਰ ext2_inode_reference *refs;
};

ਬਣਤਰ irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	ਪੂਰਨ   ref_iter;
	ext2_ino_t	*orig_map;
	ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ *entries;
	ਬਣਤਰ inode_reference_ਇੰਦਰਾਜ *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(ਅੱਖਰ *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	ਬਣਤਰ irel_ma 	*ma = 0;
	size_t		ਮਾਪ;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_inode_relocation_ਸਾਰਣੀ),
				&irel);
	ਜੇ (retval)
		ਜਾਓ errout;
	memset(irel, 0, ਆਕਾਰ(ਬਣਤਰ ext2_inode_relocation_ਸਾਰਣੀ));

	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(name)+1, &irel->name);
	ਜੇ (retval)
		ਜਾਓ errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ irel_ma), &ma);
	ਜੇ (retval)
		ਜਾਓ errout;
	memset(ma, 0, ਆਕਾਰ(ਬਣਤਰ irel_ma));
	irel->priv_ਅੰਕੜੇ = ma;

	ਮਾਪ = (size_t) (ਆਕਾਰ(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, ਆਕਾਰ(ext2_ino_t),
		&ma->orig_map);
	ਜੇ (retval)
		ਜਾਓ errout;
	memset(ma->orig_map, 0, ਮਾਪ);

	ਮਾਪ = (size_t) (ਆਕਾਰ(ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		ਆਕਾਰ(ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ), &ma->entries);
	ਜੇ (retval)
		ਜਾਓ errout;
	memset(ma->entries, 0, ਮਾਪ);

	ਮਾਪ = (size_t) (ਆਕਾਰ(ਬਣਤਰ inode_reference_ਇੰਦਰਾਜ) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		ਆਕਾਰ(ਬਣਤਰ inode_reference_ਇੰਦਰਾਜ), &ma->ref_entries);
	ਜੇ (retval)
		ਜਾਓ errout;
	memset(ma->ref_entries, 0, ਮਾਪ);
	ma->max_inode = max_inode;

	/*
	 * Fill in the irel data structure
	 */
	irel->put = ima_put;
	irel->get = ima_get;
	irel->get_by_orig = ima_get_by_orig;
	irel->ਸ਼ੁਰੂ_iter = ima_ਸ਼ੁਰੂ_iter;
	irel->ਅਗਲਾ = ima_ਅਗਲਾ;
	irel->ਜੋੜੋ_ref = ima_ਜੋੜੋ_ref;
	irel->ਸ਼ੁਰੂ_iter_ref = ima_ਸ਼ੁਰੂ_iter_ref;
	irel->ਅਗਲਾ_ref = ima_ਅਗਲਾ_ref;
	irel->move = ima_move;
	irel->delete = ima_delete;
	irel->ਮੁਕਤ = ima_free;

	*new_irel = irel;
	ਵਾਪਸ 0;

errout:
	ima_free(irel);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ *ent)
{
	ਬਣਤਰ inode_reference_ਇੰਦਰਾਜ	*ref_ent;
	ਬਣਤਰ irel_ma 			*ma;
	errcode_t			retval;
	size_t				ਮਾਪ, old_ਮਾਪ;

	ma = irel->priv_ਅੰਕੜੇ;
	ਜੇ (old > ma->max_inode)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	ਜੇ (ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].new == 0)
		ent->orig = old;
	ਨਹੀਂਤਾਂ
		ent->orig = ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (ਬਿਨਾਨਿਸ਼ਾਨੀ) old;
	ਜੇ (ref_ent->refs && ent->max_refs !=
	    ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].max_refs) {
		ਮਾਪ = (ਆਕਾਰ(ਬਣਤਰ ext2_inode_reference) * ent->max_refs);
		old_ਮਾਪ = (ਆਕਾਰ(ਬਣਤਰ ext2_inode_reference) *
			    ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].max_refs);
		retval = ext2fs_resize_mem(old_ਮਾਪ, ਮਾਪ, &ref_ent->refs);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old] = *ent;
	ma->orig_map[(ਬਿਨਾਨਿਸ਼ਾਨੀ) ent->orig] = old;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ *ent)
{
	ਬਣਤਰ irel_ma 	*ma;

	ma = irel->priv_ਅੰਕੜੇ;
	ਜੇ (old > ma->max_inode)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	ਜੇ (ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].new == 0)
		ਵਾਪਸ ENOENT;
	*ent = ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old];
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ *ent)
{
	ਬਣਤਰ irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_ਅੰਕੜੇ;
	ਜੇ (orig > ma->max_inode)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	ino = ma->orig_map[(ਬਿਨਾਨਿਸ਼ਾਨੀ) orig];
	ਜੇ (ino == 0)
		ਵਾਪਸ ENOENT;
	*old = ino;
	*ent = ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) ino];
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ima_ਸ਼ੁਰੂ_iter(ext2_irel irel)
{
	irel->current = 0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ima_ਅਗਲਾ(ext2_irel irel, ext2_ino_t *old,
			 ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ *ent)
{
	ਬਣਤਰ irel_ma 	*ma;

	ma = irel->priv_ਅੰਕੜੇ;
	ਜਦੋਂ (++irel->current < ma->max_inode) {
		ਜੇ (ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) irel->current].new == 0)
			ਜਾਰੀ;
		*old = irel->current;
		*ent = ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) irel->current];
		ਵਾਪਸ 0;
	}
	*old = 0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ima_ਜੋੜੋ_ref(ext2_irel irel, ext2_ino_t ino,
			     ਬਣਤਰ ext2_inode_reference *ref)
{
	ਬਣਤਰ irel_ma 	*ma;
	size_t		ਮਾਪ;
	ਬਣਤਰ inode_reference_ਇੰਦਰਾਜ *ref_ent;
	ਬਣਤਰ ext2_inode_relocate_ਇੰਦਰਾਜ *ent;
	errcode_t		retval;

	ma = irel->priv_ਅੰਕੜੇ;
	ਜੇ (ino > ma->max_inode)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ref_ent = ma->ref_entries + (ਬਿਨਾਨਿਸ਼ਾਨੀ) ino;
	ent = ma->entries + (ਬਿਨਾਨਿਸ਼ਾਨੀ) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	ਜੇ (ref_ent->refs == 0) {
		ਮਾਪ = (size_t) ((ਆਕਾਰ(ਬਣਤਰ ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			ਆਕਾਰ(ਬਣਤਰ ext2_inode_reference), &ref_ent->refs);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		memset(ref_ent->refs, 0, ਮਾਪ);
		ref_ent->ਅੰਕ = 0;
	}

	ਜੇ (ref_ent->ਅੰਕ >= ent->max_refs)
		ਵਾਪਸ EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(ਬਿਨਾਨਿਸ਼ਾਨੀ) ref_ent->ਅੰਕ++] = *ref;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ima_ਸ਼ੁਰੂ_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	ਬਣਤਰ irel_ma 	*ma;

	ma = irel->priv_ਅੰਕੜੇ;
	ਜੇ (ino > ma->max_inode)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	ਜੇ (ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) ino].new == 0)
		ਵਾਪਸ ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ima_ਅਗਲਾ_ref(ext2_irel irel,
			      ਬਣਤਰ ext2_inode_reference *ref)
{
	ਬਣਤਰ irel_ma 	*ma;
	ਬਣਤਰ inode_reference_ਇੰਦਰਾਜ *ref_ent;

	ma = irel->priv_ਅੰਕੜੇ;

	ref_ent = ma->ref_entries + ma->ref_current;

	ਜੇ ((ref_ent->refs == ਨਿਰਾ) ||
	    (ma->ref_iter >= ref_ent->ਅੰਕ)) {
		ref->ਖੰਡ = 0;
		ref->ਵਿੱਥ = 0;
		ਵਾਪਸ 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	ਵਾਪਸ 0;
}


ਸਥਿਰ errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	ਬਣਤਰ irel_ma 	*ma;

	ma = irel->priv_ਅੰਕੜੇ;
	ਜੇ ((old > ma->max_inode) || (new > ma->max_inode))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	ਜੇ (ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].new == 0)
		ਵਾਪਸ ENOENT;

	ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) new] = ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old];
	ਜੇ (ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) new].refs);
	ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) new] = ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old];

	ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].new = 0;
	ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].ਅੰਕ = 0;
	ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	ਬਣਤਰ irel_ma 	*ma;

	ma = irel->priv_ਅੰਕੜੇ;
	ਜੇ (old > ma->max_inode)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
	ਜੇ (ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].new == 0)
		ਵਾਪਸ ENOENT;

	ma->entries[old].new = 0;
	ਜੇ (ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].refs);
	ma->orig_map[ma->entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].orig] = 0;

	ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].ਅੰਕ = 0;
	ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) old].refs = 0;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ima_free(ext2_irel irel)
{
	ਬਣਤਰ irel_ma 	*ma;
	ext2_ino_t	ino;

	ਜੇ (!irel)
		ਵਾਪਸ 0;

	ma = irel->priv_ਅੰਕੜੇ;

	ਜੇ (ma) {
		ਜੇ (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		ਜੇ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ਜੇ (ma->ref_entries) {
			ਲਈ (ino = 0; ino <= ma->max_inode; ino++) {
				ਜੇ (ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(ਬਿਨਾਨਿਸ਼ਾਨੀ) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	ਜੇ (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	ਵਾਪਸ 0;
}
