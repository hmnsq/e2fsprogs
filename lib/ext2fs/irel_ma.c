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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "irel.h"

સ્થિર errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 સંરચના ext2_inode_relocate_entry *ent);
સ્થિર errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 સંરચના ext2_inode_relocate_entry *ent);
સ્થિર errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 સંરચના ext2_inode_relocate_entry *ent);
સ્થિર errcode_t ima_start_iter(ext2_irel irel);
સ્થિર errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			  સંરચના ext2_inode_relocate_entry *ent);
સ્થિર errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     સંરચના ext2_inode_reference *ref);
સ્થિર errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
સ્થિર errcode_t ima_next_ref(ext2_irel irel, સંરચના ext2_inode_reference *ref);
સ્થિર errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
સ્થિર errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
સ્થિર errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
સંરચના inode_reference_entry {
	__u16 અંક;
	સંરચના ext2_inode_reference *refs;
};

સંરચના irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	પૂર્ણાંક   ref_iter;
	ext2_ino_t	*orig_map;
	સંરચના ext2_inode_relocate_entry *entries;
	સંરચના inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(અક્ષર *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	સંરચના irel_ma 	*ma = 0;
	size_t		size;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(કદ(સંરચના ext2_inode_relocation_table),
				&irel);
	જો (retval)
		જાઓ errout;
	memset(irel, 0, કદ(સંરચના ext2_inode_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &irel->name);
	જો (retval)
		જાઓ errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(કદ(સંરચના irel_ma), &ma);
	જો (retval)
		જાઓ errout;
	memset(ma, 0, કદ(સંરચના irel_ma));
	irel->priv_data = ma;

	size = (size_t) (કદ(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, કદ(ext2_ino_t),
		&ma->orig_map);
	જો (retval)
		જાઓ errout;
	memset(ma->orig_map, 0, size);

	size = (size_t) (કદ(સંરચના ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		કદ(સંરચના ext2_inode_relocate_entry), &ma->entries);
	જો (retval)
		જાઓ errout;
	memset(ma->entries, 0, size);

	size = (size_t) (કદ(સંરચના inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		કદ(સંરચના inode_reference_entry), &ma->ref_entries);
	જો (retval)
		જાઓ errout;
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
	irel->મુક્ત = ima_free;

	*new_irel = irel;
	પાછા 0;

errout:
	ima_free(irel);
	પાછા retval;
}

સ્થિર errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			સંરચના ext2_inode_relocate_entry *ent)
{
	સંરચના inode_reference_entry	*ref_ent;
	સંરચના irel_ma 			*ma;
	errcode_t			retval;
	size_t				size, old_size;

	ma = irel->priv_data;
	જો (old > ma->max_inode)
		પાછા EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	જો (ma->entries[(unsigned) old].new == 0)
		ent->orig = old;
	નહીંતર
		ent->orig = ma->entries[(unsigned) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (unsigned) old;
	જો (ref_ent->refs && ent->max_refs !=
	    ma->entries[(unsigned) old].max_refs) {
		size = (કદ(સંરચના ext2_inode_reference) * ent->max_refs);
		old_size = (કદ(સંરચના ext2_inode_reference) *
			    ma->entries[(unsigned) old].max_refs);
		retval = ext2fs_resize_mem(old_size, size, &ref_ent->refs);
		જો (retval)
			પાછા retval;
	}

	ma->entries[(unsigned) old] = *ent;
	ma->orig_map[(unsigned) ent->orig] = old;
	પાછા 0;
}

સ્થિર errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			સંરચના ext2_inode_relocate_entry *ent)
{
	સંરચના irel_ma 	*ma;

	ma = irel->priv_data;
	જો (old > ma->max_inode)
		પાછા EXT2_ET_INVALID_ARGUMENT;
	જો (ma->entries[(unsigned) old].new == 0)
		પાછા ENOENT;
	*ent = ma->entries[(unsigned) old];
	પાછા 0;
}

સ્થિર errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			સંરચના ext2_inode_relocate_entry *ent)
{
	સંરચના irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_data;
	જો (orig > ma->max_inode)
		પાછા EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(unsigned) orig];
	જો (ino == 0)
		પાછા ENOENT;
	*old = ino;
	*ent = ma->entries[(unsigned) ino];
	પાછા 0;
}

સ્થિર errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	પાછા 0;
}

સ્થિર errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			 સંરચના ext2_inode_relocate_entry *ent)
{
	સંરચના irel_ma 	*ma;

	ma = irel->priv_data;
	જ્યારે (++irel->current < ma->max_inode) {
		જો (ma->entries[(unsigned) irel->current].new == 0)
			ચાલુ;
		*old = irel->current;
		*ent = ma->entries[(unsigned) irel->current];
		પાછા 0;
	}
	*old = 0;
	પાછા 0;
}

સ્થિર errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     સંરચના ext2_inode_reference *ref)
{
	સંરચના irel_ma 	*ma;
	size_t		size;
	સંરચના inode_reference_entry *ref_ent;
	સંરચના ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_data;
	જો (ino > ma->max_inode)
		પાછા EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (unsigned) ino;
	ent = ma->entries + (unsigned) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	જો (ref_ent->refs == 0) {
		size = (size_t) ((કદ(સંરચના ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			કદ(સંરચના ext2_inode_reference), &ref_ent->refs);
		જો (retval)
			પાછા retval;
		memset(ref_ent->refs, 0, size);
		ref_ent->અંક = 0;
	}

	જો (ref_ent->અંક >= ent->max_refs)
		પાછા EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(unsigned) ref_ent->અંક++] = *ref;
	પાછા 0;
}

સ્થિર errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	સંરચના irel_ma 	*ma;

	ma = irel->priv_data;
	જો (ino > ma->max_inode)
		પાછા EXT2_ET_INVALID_ARGUMENT;
	જો (ma->entries[(unsigned) ino].new == 0)
		પાછા ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	પાછા 0;
}

સ્થિર errcode_t ima_next_ref(ext2_irel irel,
			      સંરચના ext2_inode_reference *ref)
{
	સંરચના irel_ma 	*ma;
	સંરચના inode_reference_entry *ref_ent;

	ma = irel->priv_data;

	ref_ent = ma->ref_entries + ma->ref_current;

	જો ((ref_ent->refs == ખાલી) ||
	    (ma->ref_iter >= ref_ent->અંક)) {
		ref->block = 0;
		ref->offset = 0;
		પાછા 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	પાછા 0;
}


સ્થિર errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	સંરચના irel_ma 	*ma;

	ma = irel->priv_data;
	જો ((old > ma->max_inode) || (new > ma->max_inode))
		પાછા EXT2_ET_INVALID_ARGUMENT;
	જો (ma->entries[(unsigned) old].new == 0)
		પાછા ENOENT;

	ma->entries[(unsigned) new] = ma->entries[(unsigned) old];
	જો (ma->ref_entries[(unsigned) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) new].refs);
	ma->ref_entries[(unsigned) new] = ma->ref_entries[(unsigned) old];

	ma->entries[(unsigned) old].new = 0;
	ma->ref_entries[(unsigned) old].અંક = 0;
	ma->ref_entries[(unsigned) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	પાછા 0;
}

સ્થિર errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	સંરચના irel_ma 	*ma;

	ma = irel->priv_data;
	જો (old > ma->max_inode)
		પાછા EXT2_ET_INVALID_ARGUMENT;
	જો (ma->entries[(unsigned) old].new == 0)
		પાછા ENOENT;

	ma->entries[old].new = 0;
	જો (ma->ref_entries[(unsigned) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) old].refs);
	ma->orig_map[ma->entries[(unsigned) old].orig] = 0;

	ma->ref_entries[(unsigned) old].અંક = 0;
	ma->ref_entries[(unsigned) old].refs = 0;
	પાછા 0;
}

સ્થિર errcode_t ima_free(ext2_irel irel)
{
	સંરચના irel_ma 	*ma;
	ext2_ino_t	ino;

	જો (!irel)
		પાછા 0;

	ma = irel->priv_data;

	જો (ma) {
		જો (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		જો (ma->entries)
			ext2fs_free_mem(&ma->entries);
		જો (ma->ref_entries) {
			માટે (ino = 0; ino <= ma->max_inode; ino++) {
				જો (ma->ref_entries[(unsigned) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(unsigned) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	જો (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	પાછા 0;
}
