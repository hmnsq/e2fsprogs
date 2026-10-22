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
#ಅನ್ತರ್ಭಾವ "irel.h"

ಸ್ಥಿರ errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent);
ಸ್ಥಿರ errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent);
ಸ್ಥಿರ errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent);
ಸ್ಥಿರ errcode_t ima_ಆರಂಭ_iter(ext2_irel irel);
ಸ್ಥಿರ errcode_t ima_ಮುಂದಿನ(ext2_irel irel, ext2_ino_t *old,
			  ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent);
ಸ್ಥಿರ errcode_t ima_ಕೂಡಿಸು_ref(ext2_irel irel, ext2_ino_t ino,
			     ರಚನೆ ext2_inode_reference *ref);
ಸ್ಥಿರ errcode_t ima_ಆರಂಭ_iter_ref(ext2_irel irel, ext2_ino_t ino);
ಸ್ಥಿರ errcode_t ima_ಮುಂದಿನ_ref(ext2_irel irel, ರಚನೆ ext2_inode_reference *ref);
ಸ್ಥಿರ errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
ಸ್ಥಿರ errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
ಸ್ಥಿರ errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
ರಚನೆ inode_reference_ನಮೂದು {
	__u16 ಅಂಕೆ;
	ರಚನೆ ext2_inode_reference *refs;
};

ರಚನೆ irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	ಪೂರ್ಣಾಂಕ   ref_iter;
	ext2_ino_t	*orig_map;
	ರಚನೆ ext2_inode_relocate_ನಮೂದು *entries;
	ರಚನೆ inode_reference_ನಮೂದು *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(ಅಕ್ಷರ *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	ರಚನೆ irel_ma 	*ma = 0;
	size_t		ಗಾತ್ರ_2;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_inode_relocation_ಕೋಷ್ಟಕ),
				&irel);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	memset(irel, 0, ಗಾತ್ರ(ರಚನೆ ext2_inode_relocation_ಕೋಷ್ಟಕ));

	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)+1, &irel->name);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ irel_ma), &ma);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	memset(ma, 0, ಗಾತ್ರ(ರಚನೆ irel_ma));
	irel->priv_ದತ್ತಾಂಶ = ma;

	ಗಾತ್ರ_2 = (size_t) (ಗಾತ್ರ(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, ಗಾತ್ರ(ext2_ino_t),
		&ma->orig_map);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	memset(ma->orig_map, 0, ಗಾತ್ರ_2);

	ಗಾತ್ರ_2 = (size_t) (ಗಾತ್ರ(ರಚನೆ ext2_inode_relocate_ನಮೂದು) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		ಗಾತ್ರ(ರಚನೆ ext2_inode_relocate_ನಮೂದು), &ma->entries);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	memset(ma->entries, 0, ಗಾತ್ರ_2);

	ಗಾತ್ರ_2 = (size_t) (ಗಾತ್ರ(ರಚನೆ inode_reference_ನಮೂದು) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		ಗಾತ್ರ(ರಚನೆ inode_reference_ನಮೂದು), &ma->ref_entries);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	memset(ma->ref_entries, 0, ಗಾತ್ರ_2);
	ma->max_inode = max_inode;

	/*
	 * Fill in the irel data structure
	 */
	irel->put = ima_put;
	irel->get = ima_get;
	irel->get_by_orig = ima_get_by_orig;
	irel->ಆರಂಭ_iter = ima_ಆರಂಭ_iter;
	irel->ಮುಂದಿನ = ima_ಮುಂದಿನ;
	irel->ಕೂಡಿಸು_ref = ima_ಕೂಡಿಸು_ref;
	irel->ಆರಂಭ_iter_ref = ima_ಆರಂಭ_iter_ref;
	irel->ಮುಂದಿನ_ref = ima_ಮುಂದಿನ_ref;
	irel->move = ima_move;
	irel->delete = ima_delete;
	irel->ಬಿಡುಗಡೆ = ima_free;

	*new_irel = irel;
	ಹಿಂತಿರುಗಿ 0;

errout:
	ima_free(irel);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent)
{
	ರಚನೆ inode_reference_ನಮೂದು	*ref_ent;
	ರಚನೆ irel_ma 			*ma;
	errcode_t			retval;
	size_t				ಗಾತ್ರ_2, old_ಗಾತ್ರ;

	ma = irel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ (old > ma->max_inode)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	ಆದರೆ (ma->entries[(ಚಿಹ್ನೆರಹಿತ) old].new == 0)
		ent->orig = old;
	ಇಲ್ಲದಿದ್ದರೆ
		ent->orig = ma->entries[(ಚಿಹ್ನೆರಹಿತ) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (ಚಿಹ್ನೆರಹಿತ) old;
	ಆದರೆ (ref_ent->refs && ent->max_refs !=
	    ma->entries[(ಚಿಹ್ನೆರಹಿತ) old].max_refs) {
		ಗಾತ್ರ_2 = (ಗಾತ್ರ(ರಚನೆ ext2_inode_reference) * ent->max_refs);
		old_ಗಾತ್ರ = (ಗಾತ್ರ(ರಚನೆ ext2_inode_reference) *
			    ma->entries[(ಚಿಹ್ನೆರಹಿತ) old].max_refs);
		retval = ext2fs_resize_mem(old_ಗಾತ್ರ, ಗಾತ್ರ_2, &ref_ent->refs);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	ma->entries[(ಚಿಹ್ನೆರಹಿತ) old] = *ent;
	ma->orig_map[(ಚಿಹ್ನೆರಹಿತ) ent->orig] = old;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent)
{
	ರಚನೆ irel_ma 	*ma;

	ma = irel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ (old > ma->max_inode)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	ಆದರೆ (ma->entries[(ಚಿಹ್ನೆರಹಿತ) old].new == 0)
		ಹಿಂತಿರುಗಿ ENOENT;
	*ent = ma->entries[(ಚಿಹ್ನೆರಹಿತ) old];
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent)
{
	ರಚನೆ irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ (orig > ma->max_inode)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	ino = ma->orig_map[(ಚಿಹ್ನೆರಹಿತ) orig];
	ಆದರೆ (ino == 0)
		ಹಿಂತಿರುಗಿ ENOENT;
	*old = ino;
	*ent = ma->entries[(ಚಿಹ್ನೆರಹಿತ) ino];
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ima_ಆರಂಭ_iter(ext2_irel irel)
{
	irel->current = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ima_ಮುಂದಿನ(ext2_irel irel, ext2_ino_t *old,
			 ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent)
{
	ರಚನೆ irel_ma 	*ma;

	ma = irel->priv_ದತ್ತಾಂಶ;
	ಆಗ (++irel->current < ma->max_inode) {
		ಆದರೆ (ma->entries[(ಚಿಹ್ನೆರಹಿತ) irel->current].new == 0)
			ಮುಂದುವರಿ;
		*old = irel->current;
		*ent = ma->entries[(ಚಿಹ್ನೆರಹಿತ) irel->current];
		ಹಿಂತಿರುಗಿ 0;
	}
	*old = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ima_ಕೂಡಿಸು_ref(ext2_irel irel, ext2_ino_t ino,
			     ರಚನೆ ext2_inode_reference *ref)
{
	ರಚನೆ irel_ma 	*ma;
	size_t		ಗಾತ್ರ_2;
	ರಚನೆ inode_reference_ನಮೂದು *ref_ent;
	ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent;
	errcode_t		retval;

	ma = irel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ (ino > ma->max_inode)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ref_ent = ma->ref_entries + (ಚಿಹ್ನೆರಹಿತ) ino;
	ent = ma->entries + (ಚಿಹ್ನೆರಹಿತ) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	ಆದರೆ (ref_ent->refs == 0) {
		ಗಾತ್ರ_2 = (size_t) ((ಗಾತ್ರ(ರಚನೆ ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			ಗಾತ್ರ(ರಚನೆ ext2_inode_reference), &ref_ent->refs);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		memset(ref_ent->refs, 0, ಗಾತ್ರ_2);
		ref_ent->ಅಂಕೆ = 0;
	}

	ಆದರೆ (ref_ent->ಅಂಕೆ >= ent->max_refs)
		ಹಿಂತಿರುಗಿ EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(ಚಿಹ್ನೆರಹಿತ) ref_ent->ಅಂಕೆ++] = *ref;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ima_ಆರಂಭ_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	ರಚನೆ irel_ma 	*ma;

	ma = irel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ (ino > ma->max_inode)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	ಆದರೆ (ma->entries[(ಚಿಹ್ನೆರಹಿತ) ino].new == 0)
		ಹಿಂತಿರುಗಿ ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ima_ಮುಂದಿನ_ref(ext2_irel irel,
			      ರಚನೆ ext2_inode_reference *ref)
{
	ರಚನೆ irel_ma 	*ma;
	ರಚನೆ inode_reference_ನಮೂದು *ref_ent;

	ma = irel->priv_ದತ್ತಾಂಶ;

	ref_ent = ma->ref_entries + ma->ref_current;

	ಆದರೆ ((ref_ent->refs == ಖಾಲಿ) ||
	    (ma->ref_iter >= ref_ent->ಅಂಕೆ)) {
		ref->ಖಂಡ = 0;
		ref->ಸ್ಥಾನಾಂತರ = 0;
		ಹಿಂತಿರುಗಿ 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	ಹಿಂತಿರುಗಿ 0;
}


ಸ್ಥಿರ errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	ರಚನೆ irel_ma 	*ma;

	ma = irel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ ((old > ma->max_inode) || (new > ma->max_inode))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	ಆದರೆ (ma->entries[(ಚಿಹ್ನೆರಹಿತ) old].new == 0)
		ಹಿಂತಿರುಗಿ ENOENT;

	ma->entries[(ಚಿಹ್ನೆರಹಿತ) new] = ma->entries[(ಚಿಹ್ನೆರಹಿತ) old];
	ಆದರೆ (ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) new].refs);
	ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) new] = ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) old];

	ma->entries[(ಚಿಹ್ನೆರಹಿತ) old].new = 0;
	ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) old].ಅಂಕೆ = 0;
	ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	ರಚನೆ irel_ma 	*ma;

	ma = irel->priv_ದತ್ತಾಂಶ;
	ಆದರೆ (old > ma->max_inode)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;
	ಆದರೆ (ma->entries[(ಚಿಹ್ನೆರಹಿತ) old].new == 0)
		ಹಿಂತಿರುಗಿ ENOENT;

	ma->entries[old].new = 0;
	ಆದರೆ (ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) old].refs);
	ma->orig_map[ma->entries[(ಚಿಹ್ನೆರಹಿತ) old].orig] = 0;

	ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) old].ಅಂಕೆ = 0;
	ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) old].refs = 0;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ima_free(ext2_irel irel)
{
	ರಚನೆ irel_ma 	*ma;
	ext2_ino_t	ino;

	ಆದರೆ (!irel)
		ಹಿಂತಿರುಗಿ 0;

	ma = irel->priv_ದತ್ತಾಂಶ;

	ಆದರೆ (ma) {
		ಆದರೆ (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		ಆದರೆ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ಆದರೆ (ma->ref_entries) {
			ಗಾಗಿ (ino = 0; ino <= ma->max_inode; ino++) {
				ಆದರೆ (ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(ಚಿಹ್ನೆರಹಿತ) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	ಆದರೆ (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	ಹಿಂತಿರುಗಿ 0;
}
