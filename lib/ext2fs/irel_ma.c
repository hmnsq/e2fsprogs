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
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "irel.h"

ಸ್ಥಿರ errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 ಸಂರಚನಾ ext2_inode_relocate_entry *ent);
ಸ್ಥಿರ errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 ಸಂರಚನಾ ext2_inode_relocate_entry *ent);
ಸ್ಥಿರ errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 ಸಂರಚನಾ ext2_inode_relocate_entry *ent);
ಸ್ಥಿರ errcode_t ima_start_iter(ext2_irel irel);
ಸ್ಥಿರ errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			  ಸಂರಚನಾ ext2_inode_relocate_entry *ent);
ಸ್ಥಿರ errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     ಸಂರಚನಾ ext2_inode_reference *ref);
ಸ್ಥಿರ errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
ಸ್ಥಿರ errcode_t ima_next_ref(ext2_irel irel, ಸಂರಚನಾ ext2_inode_reference *ref);
ಸ್ಥಿರ errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
ಸ್ಥಿರ errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
ಸ್ಥಿರ errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
ಸಂರಚನಾ inode_reference_entry {
	__u16 ಅಂಕೆ;
	ಸಂರಚನಾ ext2_inode_reference *refs;
};

ಸಂರಚನಾ irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	ಪೂರ್ಣಾಙ್ಕ   ref_iter;
	ext2_ino_t	*orig_map;
	ಸಂರಚನಾ ext2_inode_relocate_entry *entries;
	ಸಂರಚನಾ inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(ವರ್ಣ *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	ಸಂರಚನಾ irel_ma 	*ma = 0;
	size_t		size;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_relocation_table),
				&irel);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	memset(irel, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &irel->name);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ irel_ma), &ma);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	memset(ma, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ irel_ma));
	irel->priv_data = ma;

	size = (size_t) (ಪರಿಮಾಣ(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, ಪರಿಮಾಣ(ext2_ino_t),
		&ma->orig_map);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	memset(ma->orig_map, 0, size);

	size = (size_t) (ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_relocate_entry), &ma->entries);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	memset(ma->entries, 0, size);

	size = (size_t) (ಪರಿಮಾಣ(ಸಂರಚನಾ inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		ಪರಿಮಾಣ(ಸಂರಚನಾ inode_reference_entry), &ma->ref_entries);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
	memset(ma->ref_entries, 0, size);
	ma->max_inode = max_inode;

	/*
	 * Fill in the irel data structure
	 */
	irel->put = ima_put;
	irel->get = ima_get;
	irel->get_by_orig = ima_get_by_orig;
	irel->start_iter = ima_start_iter;
	irel->next = ima_next;
	irel->add_ref = ima_add_ref;
	irel->start_iter_ref = ima_start_iter_ref;
	irel->next_ref = ima_next_ref;
	irel->move = ima_move;
	irel->delete = ima_delete;
	irel->ಮುಕ್ತ = ima_free;

	*new_irel = irel;
	ಪ್ರತ್ಯಾಗಮ 0;

errout:
	ima_free(irel);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			ಸಂರಚನಾ ext2_inode_relocate_entry *ent)
{
	ಸಂರಚನಾ inode_reference_entry	*ref_ent;
	ಸಂರಚನಾ irel_ma 			*ma;
	errcode_t			retval;
	size_t				size, old_size;

	ma = irel->priv_data;
	ಯದಿ (old > ma->max_inode)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	ಯದಿ (ma->entries[(unsigned) old].new == 0)
		ent->orig = old;
	ಅನ್ಯಥಾ
		ent->orig = ma->entries[(unsigned) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (unsigned) old;
	ಯದಿ (ref_ent->refs && ent->max_refs !=
	    ma->entries[(unsigned) old].max_refs) {
		size = (ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_reference) * ent->max_refs);
		old_size = (ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_reference) *
			    ma->entries[(unsigned) old].max_refs);
		retval = ext2fs_resize_mem(old_size, size, &ref_ent->refs);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	ma->entries[(unsigned) old] = *ent;
	ma->orig_map[(unsigned) ent->orig] = old;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			ಸಂರಚನಾ ext2_inode_relocate_entry *ent)
{
	ಸಂರಚನಾ irel_ma 	*ma;

	ma = irel->priv_data;
	ಯದಿ (old > ma->max_inode)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	ಯದಿ (ma->entries[(unsigned) old].new == 0)
		ಪ್ರತ್ಯಾಗಮ ENOENT;
	*ent = ma->entries[(unsigned) old];
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			ಸಂರಚನಾ ext2_inode_relocate_entry *ent)
{
	ಸಂರಚನಾ irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_data;
	ಯದಿ (orig > ma->max_inode)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(unsigned) orig];
	ಯದಿ (ino == 0)
		ಪ್ರತ್ಯಾಗಮ ENOENT;
	*old = ino;
	*ent = ma->entries[(unsigned) ino];
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			 ಸಂರಚನಾ ext2_inode_relocate_entry *ent)
{
	ಸಂರಚನಾ irel_ma 	*ma;

	ma = irel->priv_data;
	ಯಾವತ್ (++irel->current < ma->max_inode) {
		ಯದಿ (ma->entries[(unsigned) irel->current].new == 0)
			ಅನುವರ್ತ;
		*old = irel->current;
		*ent = ma->entries[(unsigned) irel->current];
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	*old = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     ಸಂರಚನಾ ext2_inode_reference *ref)
{
	ಸಂರಚನಾ irel_ma 	*ma;
	size_t		size;
	ಸಂರಚನಾ inode_reference_entry *ref_ent;
	ಸಂರಚನಾ ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_data;
	ಯದಿ (ino > ma->max_inode)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (unsigned) ino;
	ent = ma->entries + (unsigned) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	ಯದಿ (ref_ent->refs == 0) {
		size = (size_t) ((ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_reference), &ref_ent->refs);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		memset(ref_ent->refs, 0, size);
		ref_ent->ಅಂಕೆ = 0;
	}

	ಯದಿ (ref_ent->ಅಂಕೆ >= ent->max_refs)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(unsigned) ref_ent->ಅಂಕೆ++] = *ref;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	ಸಂರಚನಾ irel_ma 	*ma;

	ma = irel->priv_data;
	ಯದಿ (ino > ma->max_inode)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	ಯದಿ (ma->entries[(unsigned) ino].new == 0)
		ಪ್ರತ್ಯಾಗಮ ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ima_next_ref(ext2_irel irel,
			      ಸಂರಚನಾ ext2_inode_reference *ref)
{
	ಸಂರಚನಾ irel_ma 	*ma;
	ಸಂರಚನಾ inode_reference_entry *ref_ent;

	ma = irel->priv_data;

	ref_ent = ma->ref_entries + ma->ref_current;

	ಯದಿ ((ref_ent->refs == ರಿಕ್ತ) ||
	    (ma->ref_iter >= ref_ent->ಅಂಕೆ)) {
		ref->block = 0;
		ref->offset = 0;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	ಪ್ರತ್ಯಾಗಮ 0;
}


ಸ್ಥಿರ errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	ಸಂರಚನಾ irel_ma 	*ma;

	ma = irel->priv_data;
	ಯದಿ ((old > ma->max_inode) || (new > ma->max_inode))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	ಯದಿ (ma->entries[(unsigned) old].new == 0)
		ಪ್ರತ್ಯಾಗಮ ENOENT;

	ma->entries[(unsigned) new] = ma->entries[(unsigned) old];
	ಯದಿ (ma->ref_entries[(unsigned) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) new].refs);
	ma->ref_entries[(unsigned) new] = ma->ref_entries[(unsigned) old];

	ma->entries[(unsigned) old].new = 0;
	ma->ref_entries[(unsigned) old].ಅಂಕೆ = 0;
	ma->ref_entries[(unsigned) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	ಸಂರಚನಾ irel_ma 	*ma;

	ma = irel->priv_data;
	ಯದಿ (old > ma->max_inode)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;
	ಯದಿ (ma->entries[(unsigned) old].new == 0)
		ಪ್ರತ್ಯಾಗಮ ENOENT;

	ma->entries[old].new = 0;
	ಯದಿ (ma->ref_entries[(unsigned) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) old].refs);
	ma->orig_map[ma->entries[(unsigned) old].orig] = 0;

	ma->ref_entries[(unsigned) old].ಅಂಕೆ = 0;
	ma->ref_entries[(unsigned) old].refs = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ima_free(ext2_irel irel)
{
	ಸಂರಚನಾ irel_ma 	*ma;
	ext2_ino_t	ino;

	ಯದಿ (!irel)
		ಪ್ರತ್ಯಾಗಮ 0;

	ma = irel->priv_data;

	ಯದಿ (ma) {
		ಯದಿ (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		ಯದಿ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ಯದಿ (ma->ref_entries) {
			ಪ್ರತಿ (ino = 0; ino <= ma->max_inode; ino++) {
				ಯದಿ (ma->ref_entries[(unsigned) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(unsigned) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	ಯದಿ (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	ಪ್ರತ್ಯಾಗಮ 0;
}
