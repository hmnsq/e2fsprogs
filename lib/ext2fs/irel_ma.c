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

#ضمن "config.h"
#ضمن <fcntl.h>
#ضمن <stdio.h>
#ضمن <string.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ܐܢ HAVE_ERRNO_H
#ضمن <errno.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "irel.h"

ساكن errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 بنية ext2_inode_relocate_entry *ent);
ساكن errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 بنية ext2_inode_relocate_entry *ent);
ساكن errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 بنية ext2_inode_relocate_entry *ent);
ساكن errcode_t ima_start_iter(ext2_irel irel);
ساكن errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			  بنية ext2_inode_relocate_entry *ent);
ساكن errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     بنية ext2_inode_reference *ref);
ساكن errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
ساكن errcode_t ima_next_ref(ext2_irel irel, بنية ext2_inode_reference *ref);
ساكن errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
ساكن errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
ساكن errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
بنية inode_reference_entry {
	__u16 num;
	بنية ext2_inode_reference *refs;
};

بنية irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	صحيح   ref_iter;
	ext2_ino_t	*orig_map;
	بنية ext2_inode_relocate_entry *entries;
	بنية inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(حرف *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	بنية irel_ma 	*ma = 0;
	size_t		size;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(حجم(بنية ext2_inode_relocation_table),
				&irel);
	ܐܢ (retval)
		اذهب errout;
	memset(irel, 0, حجم(بنية ext2_inode_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &irel->name);
	ܐܢ (retval)
		اذهب errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(حجم(بنية irel_ma), &ma);
	ܐܢ (retval)
		اذهب errout;
	memset(ma, 0, حجم(بنية irel_ma));
	irel->priv_data = ma;

	size = (size_t) (حجم(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, حجم(ext2_ino_t),
		&ma->orig_map);
	ܐܢ (retval)
		اذهب errout;
	memset(ma->orig_map, 0, size);

	size = (size_t) (حجم(بنية ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		حجم(بنية ext2_inode_relocate_entry), &ma->entries);
	ܐܢ (retval)
		اذهب errout;
	memset(ma->entries, 0, size);

	size = (size_t) (حجم(بنية inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		حجم(بنية inode_reference_entry), &ma->ref_entries);
	ܐܢ (retval)
		اذهب errout;
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
	irel->حرر = ima_free;

	*new_irel = irel;
	ܦܢܐ 0;

errout:
	ima_free(irel);
	ܦܢܐ retval;
}

ساكن errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			بنية ext2_inode_relocate_entry *ent)
{
	بنية inode_reference_entry	*ref_ent;
	بنية irel_ma 			*ma;
	errcode_t			retval;
	size_t				size, old_size;

	ma = irel->priv_data;
	ܐܢ (old > ma->max_inode)
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	ܐܢ (ma->entries[(unsigned) old].new == 0)
		ent->orig = old;
	ܐܚܪܢܐ
		ent->orig = ma->entries[(unsigned) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (unsigned) old;
	ܐܢ (ref_ent->refs && ent->max_refs !=
	    ma->entries[(unsigned) old].max_refs) {
		size = (حجم(بنية ext2_inode_reference) * ent->max_refs);
		old_size = (حجم(بنية ext2_inode_reference) *
			    ma->entries[(unsigned) old].max_refs);
		retval = ext2fs_resize_mem(old_size, size, &ref_ent->refs);
		ܐܢ (retval)
			ܦܢܐ retval;
	}

	ma->entries[(unsigned) old] = *ent;
	ma->orig_map[(unsigned) ent->orig] = old;
	ܦܢܐ 0;
}

ساكن errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			بنية ext2_inode_relocate_entry *ent)
{
	بنية irel_ma 	*ma;

	ma = irel->priv_data;
	ܐܢ (old > ma->max_inode)
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;
	ܐܢ (ma->entries[(unsigned) old].new == 0)
		ܦܢܐ ENOENT;
	*ent = ma->entries[(unsigned) old];
	ܦܢܐ 0;
}

ساكن errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			بنية ext2_inode_relocate_entry *ent)
{
	بنية irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_data;
	ܐܢ (orig > ma->max_inode)
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(unsigned) orig];
	ܐܢ (ino == 0)
		ܦܢܐ ENOENT;
	*old = ino;
	*ent = ma->entries[(unsigned) ino];
	ܦܢܐ 0;
}

ساكن errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	ܦܢܐ 0;
}

ساكن errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			 بنية ext2_inode_relocate_entry *ent)
{
	بنية irel_ma 	*ma;

	ma = irel->priv_data;
	ܟܡܐ (++irel->current < ma->max_inode) {
		ܐܢ (ma->entries[(unsigned) irel->current].new == 0)
			استمر;
		*old = irel->current;
		*ent = ma->entries[(unsigned) irel->current];
		ܦܢܐ 0;
	}
	*old = 0;
	ܦܢܐ 0;
}

ساكن errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     بنية ext2_inode_reference *ref)
{
	بنية irel_ma 	*ma;
	size_t		size;
	بنية inode_reference_entry *ref_ent;
	بنية ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_data;
	ܐܢ (ino > ma->max_inode)
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (unsigned) ino;
	ent = ma->entries + (unsigned) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	ܐܢ (ref_ent->refs == 0) {
		size = (size_t) ((حجم(بنية ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			حجم(بنية ext2_inode_reference), &ref_ent->refs);
		ܐܢ (retval)
			ܦܢܐ retval;
		memset(ref_ent->refs, 0, size);
		ref_ent->num = 0;
	}

	ܐܢ (ref_ent->num >= ent->max_refs)
		ܦܢܐ EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(unsigned) ref_ent->num++] = *ref;
	ܦܢܐ 0;
}

ساكن errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	بنية irel_ma 	*ma;

	ma = irel->priv_data;
	ܐܢ (ino > ma->max_inode)
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;
	ܐܢ (ma->entries[(unsigned) ino].new == 0)
		ܦܢܐ ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	ܦܢܐ 0;
}

ساكن errcode_t ima_next_ref(ext2_irel irel,
			      بنية ext2_inode_reference *ref)
{
	بنية irel_ma 	*ma;
	بنية inode_reference_entry *ref_ent;

	ma = irel->priv_data;

	ref_ent = ma->ref_entries + ma->ref_current;

	ܐܢ ((ref_ent->refs == عدم) ||
	    (ma->ref_iter >= ref_ent->num)) {
		ref->block = 0;
		ref->offset = 0;
		ܦܢܐ 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	ܦܢܐ 0;
}


ساكن errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	بنية irel_ma 	*ma;

	ma = irel->priv_data;
	ܐܢ ((old > ma->max_inode) || (new > ma->max_inode))
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;
	ܐܢ (ma->entries[(unsigned) old].new == 0)
		ܦܢܐ ENOENT;

	ma->entries[(unsigned) new] = ma->entries[(unsigned) old];
	ܐܢ (ma->ref_entries[(unsigned) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) new].refs);
	ma->ref_entries[(unsigned) new] = ma->ref_entries[(unsigned) old];

	ma->entries[(unsigned) old].new = 0;
	ma->ref_entries[(unsigned) old].num = 0;
	ma->ref_entries[(unsigned) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	ܦܢܐ 0;
}

ساكن errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	بنية irel_ma 	*ma;

	ma = irel->priv_data;
	ܐܢ (old > ma->max_inode)
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;
	ܐܢ (ma->entries[(unsigned) old].new == 0)
		ܦܢܐ ENOENT;

	ma->entries[old].new = 0;
	ܐܢ (ma->ref_entries[(unsigned) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) old].refs);
	ma->orig_map[ma->entries[(unsigned) old].orig] = 0;

	ma->ref_entries[(unsigned) old].num = 0;
	ma->ref_entries[(unsigned) old].refs = 0;
	ܦܢܐ 0;
}

ساكن errcode_t ima_free(ext2_irel irel)
{
	بنية irel_ma 	*ma;
	ext2_ino_t	ino;

	ܐܢ (!irel)
		ܦܢܐ 0;

	ma = irel->priv_data;

	ܐܢ (ma) {
		ܐܢ (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		ܐܢ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ܐܢ (ma->ref_entries) {
			لكل (ino = 0; ino <= ma->max_inode; ino++) {
				ܐܢ (ma->ref_entries[(unsigned) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(unsigned) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	ܐܢ (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	ܦܢܐ 0;
}
