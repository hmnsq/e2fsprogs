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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "irel.h"

സ്ഥിതം errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 ഘടന ext2_inode_relocate_entry *ent);
സ്ഥിതം errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 ഘടന ext2_inode_relocate_entry *ent);
സ്ഥിതം errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 ഘടന ext2_inode_relocate_entry *ent);
സ്ഥിതം errcode_t ima_start_iter(ext2_irel irel);
സ്ഥിതം errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			  ഘടന ext2_inode_relocate_entry *ent);
സ്ഥിതം errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     ഘടന ext2_inode_reference *ref);
സ്ഥിതം errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
സ്ഥിതം errcode_t ima_next_ref(ext2_irel irel, ഘടന ext2_inode_reference *ref);
സ്ഥിതം errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
സ്ഥിതം errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
സ്ഥിതം errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
ഘടന inode_reference_entry {
	__u16 അക്കം;
	ഘടന ext2_inode_reference *refs;
};

ഘടന irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	പൂർണ്ണസംഖ്യ   ref_iter;
	ext2_ino_t	*orig_map;
	ഘടന ext2_inode_relocate_entry *entries;
	ഘടന inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(അക്ഷരം *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	ഘടന irel_ma 	*ma = 0;
	size_t		size;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_inode_relocation_table),
				&irel);
	എങ്കിൽ (retval)
		പോകുക errout;
	memset(irel, 0, വലുപ്പം(ഘടന ext2_inode_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &irel->name);
	എങ്കിൽ (retval)
		പോകുക errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന irel_ma), &ma);
	എങ്കിൽ (retval)
		പോകുക errout;
	memset(ma, 0, വലുപ്പം(ഘടന irel_ma));
	irel->priv_data = ma;

	size = (size_t) (വലുപ്പം(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, വലുപ്പം(ext2_ino_t),
		&ma->orig_map);
	എങ്കിൽ (retval)
		പോകുക errout;
	memset(ma->orig_map, 0, size);

	size = (size_t) (വലുപ്പം(ഘടന ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		വലുപ്പം(ഘടന ext2_inode_relocate_entry), &ma->entries);
	എങ്കിൽ (retval)
		പോകുക errout;
	memset(ma->entries, 0, size);

	size = (size_t) (വലുപ്പം(ഘടന inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		വലുപ്പം(ഘടന inode_reference_entry), &ma->ref_entries);
	എങ്കിൽ (retval)
		പോകുക errout;
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
	irel->സ്വതന്ത്രം = ima_free;

	*new_irel = irel;
	തിരികെ 0;

errout:
	ima_free(irel);
	തിരികെ retval;
}

സ്ഥിതം errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			ഘടന ext2_inode_relocate_entry *ent)
{
	ഘടന inode_reference_entry	*ref_ent;
	ഘടന irel_ma 			*ma;
	errcode_t			retval;
	size_t				size, old_size;

	ma = irel->priv_data;
	എങ്കിൽ (old > ma->max_inode)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	എങ്കിൽ (ma->entries[(unsigned) old].new == 0)
		ent->orig = old;
	അല്ലെങ്കിൽ
		ent->orig = ma->entries[(unsigned) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (unsigned) old;
	എങ്കിൽ (ref_ent->refs && ent->max_refs !=
	    ma->entries[(unsigned) old].max_refs) {
		size = (വലുപ്പം(ഘടന ext2_inode_reference) * ent->max_refs);
		old_size = (വലുപ്പം(ഘടന ext2_inode_reference) *
			    ma->entries[(unsigned) old].max_refs);
		retval = ext2fs_resize_mem(old_size, size, &ref_ent->refs);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}

	ma->entries[(unsigned) old] = *ent;
	ma->orig_map[(unsigned) ent->orig] = old;
	തിരികെ 0;
}

സ്ഥിതം errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			ഘടന ext2_inode_relocate_entry *ent)
{
	ഘടന irel_ma 	*ma;

	ma = irel->priv_data;
	എങ്കിൽ (old > ma->max_inode)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	എങ്കിൽ (ma->entries[(unsigned) old].new == 0)
		തിരികെ ENOENT;
	*ent = ma->entries[(unsigned) old];
	തിരികെ 0;
}

സ്ഥിതം errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			ഘടന ext2_inode_relocate_entry *ent)
{
	ഘടന irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_data;
	എങ്കിൽ (orig > ma->max_inode)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(unsigned) orig];
	എങ്കിൽ (ino == 0)
		തിരികെ ENOENT;
	*old = ino;
	*ent = ma->entries[(unsigned) ino];
	തിരികെ 0;
}

സ്ഥിതം errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	തിരികെ 0;
}

സ്ഥിതം errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			 ഘടന ext2_inode_relocate_entry *ent)
{
	ഘടന irel_ma 	*ma;

	ma = irel->priv_data;
	ആകുമ്പോൾ (++irel->current < ma->max_inode) {
		എങ്കിൽ (ma->entries[(unsigned) irel->current].new == 0)
			തുടരുക;
		*old = irel->current;
		*ent = ma->entries[(unsigned) irel->current];
		തിരികെ 0;
	}
	*old = 0;
	തിരികെ 0;
}

സ്ഥിതം errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     ഘടന ext2_inode_reference *ref)
{
	ഘടന irel_ma 	*ma;
	size_t		size;
	ഘടന inode_reference_entry *ref_ent;
	ഘടന ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_data;
	എങ്കിൽ (ino > ma->max_inode)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (unsigned) ino;
	ent = ma->entries + (unsigned) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	എങ്കിൽ (ref_ent->refs == 0) {
		size = (size_t) ((വലുപ്പം(ഘടന ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			വലുപ്പം(ഘടന ext2_inode_reference), &ref_ent->refs);
		എങ്കിൽ (retval)
			തിരികെ retval;
		memset(ref_ent->refs, 0, size);
		ref_ent->അക്കം = 0;
	}

	എങ്കിൽ (ref_ent->അക്കം >= ent->max_refs)
		തിരികെ EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(unsigned) ref_ent->അക്കം++] = *ref;
	തിരികെ 0;
}

സ്ഥിതം errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	ഘടന irel_ma 	*ma;

	ma = irel->priv_data;
	എങ്കിൽ (ino > ma->max_inode)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	എങ്കിൽ (ma->entries[(unsigned) ino].new == 0)
		തിരികെ ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	തിരികെ 0;
}

സ്ഥിതം errcode_t ima_next_ref(ext2_irel irel,
			      ഘടന ext2_inode_reference *ref)
{
	ഘടന irel_ma 	*ma;
	ഘടന inode_reference_entry *ref_ent;

	ma = irel->priv_data;

	ref_ent = ma->ref_entries + ma->ref_current;

	എങ്കിൽ ((ref_ent->refs == ശൂന്യ) ||
	    (ma->ref_iter >= ref_ent->അക്കം)) {
		ref->block = 0;
		ref->offset = 0;
		തിരികെ 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	തിരികെ 0;
}


സ്ഥിതം errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	ഘടന irel_ma 	*ma;

	ma = irel->priv_data;
	എങ്കിൽ ((old > ma->max_inode) || (new > ma->max_inode))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	എങ്കിൽ (ma->entries[(unsigned) old].new == 0)
		തിരികെ ENOENT;

	ma->entries[(unsigned) new] = ma->entries[(unsigned) old];
	എങ്കിൽ (ma->ref_entries[(unsigned) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) new].refs);
	ma->ref_entries[(unsigned) new] = ma->ref_entries[(unsigned) old];

	ma->entries[(unsigned) old].new = 0;
	ma->ref_entries[(unsigned) old].അക്കം = 0;
	ma->ref_entries[(unsigned) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	തിരികെ 0;
}

സ്ഥിതം errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	ഘടന irel_ma 	*ma;

	ma = irel->priv_data;
	എങ്കിൽ (old > ma->max_inode)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;
	എങ്കിൽ (ma->entries[(unsigned) old].new == 0)
		തിരികെ ENOENT;

	ma->entries[old].new = 0;
	എങ്കിൽ (ma->ref_entries[(unsigned) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) old].refs);
	ma->orig_map[ma->entries[(unsigned) old].orig] = 0;

	ma->ref_entries[(unsigned) old].അക്കം = 0;
	ma->ref_entries[(unsigned) old].refs = 0;
	തിരികെ 0;
}

സ്ഥിതം errcode_t ima_free(ext2_irel irel)
{
	ഘടന irel_ma 	*ma;
	ext2_ino_t	ino;

	എങ്കിൽ (!irel)
		തിരികെ 0;

	ma = irel->priv_data;

	എങ്കിൽ (ma) {
		എങ്കിൽ (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		എങ്കിൽ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		എങ്കിൽ (ma->ref_entries) {
			വേണ്ടി (ino = 0; ino <= ma->max_inode; ino++) {
				എങ്കിൽ (ma->ref_entries[(unsigned) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(unsigned) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	എങ്കിൽ (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	തിരികെ 0;
}
