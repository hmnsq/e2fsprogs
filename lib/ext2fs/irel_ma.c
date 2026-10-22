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

#شامل "config.h"
#شامل <fcntl.h>
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "irel.h"

ایستا errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 ساختار ext2_inode_relocate_entry *ent);
ایستا errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 ساختار ext2_inode_relocate_entry *ent);
ایستا errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 ساختار ext2_inode_relocate_entry *ent);
ایستا errcode_t ima_start_iter(ext2_irel irel);
ایستا errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			  ساختار ext2_inode_relocate_entry *ent);
ایستا errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     ساختار ext2_inode_reference *ref);
ایستا errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
ایستا errcode_t ima_next_ref(ext2_irel irel, ساختار ext2_inode_reference *ref);
ایستا errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
ایستا errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
ایستا errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
ساختار inode_reference_entry {
	__u16 رقم;
	ساختار ext2_inode_reference *refs;
};

ساختار irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	صحیح   ref_iter;
	ext2_ino_t	*orig_map;
	ساختار ext2_inode_relocate_entry *entries;
	ساختار inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(نویسه *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	ساختار irel_ma 	*ma = 0;
	size_t		size;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(اندازه(ساختار ext2_inode_relocation_table),
				&irel);
	اگر (retval)
		برو errout;
	memset(irel, 0, اندازه(ساختار ext2_inode_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &irel->name);
	اگر (retval)
		برو errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(اندازه(ساختار irel_ma), &ma);
	اگر (retval)
		برو errout;
	memset(ma, 0, اندازه(ساختار irel_ma));
	irel->priv_data = ma;

	size = (size_t) (اندازه(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, اندازه(ext2_ino_t),
		&ma->orig_map);
	اگر (retval)
		برو errout;
	memset(ma->orig_map, 0, size);

	size = (size_t) (اندازه(ساختار ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		اندازه(ساختار ext2_inode_relocate_entry), &ma->entries);
	اگر (retval)
		برو errout;
	memset(ma->entries, 0, size);

	size = (size_t) (اندازه(ساختار inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		اندازه(ساختار inode_reference_entry), &ma->ref_entries);
	اگر (retval)
		برو errout;
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
	irel->آزاد = ima_free;

	*new_irel = irel;
	بازگشت 0;

errout:
	ima_free(irel);
	بازگشت retval;
}

ایستا errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			ساختار ext2_inode_relocate_entry *ent)
{
	ساختار inode_reference_entry	*ref_ent;
	ساختار irel_ma 			*ma;
	errcode_t			retval;
	size_t				size, old_size;

	ma = irel->priv_data;
	اگر (old > ma->max_inode)
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	اگر (ma->entries[(unsigned) old].new == 0)
		ent->orig = old;
	وگرنه
		ent->orig = ma->entries[(unsigned) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (unsigned) old;
	اگر (ref_ent->refs && ent->max_refs !=
	    ma->entries[(unsigned) old].max_refs) {
		size = (اندازه(ساختار ext2_inode_reference) * ent->max_refs);
		old_size = (اندازه(ساختار ext2_inode_reference) *
			    ma->entries[(unsigned) old].max_refs);
		retval = ext2fs_resize_mem(old_size, size, &ref_ent->refs);
		اگر (retval)
			بازگشت retval;
	}

	ma->entries[(unsigned) old] = *ent;
	ma->orig_map[(unsigned) ent->orig] = old;
	بازگشت 0;
}

ایستا errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			ساختار ext2_inode_relocate_entry *ent)
{
	ساختار irel_ma 	*ma;

	ma = irel->priv_data;
	اگر (old > ma->max_inode)
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	اگر (ma->entries[(unsigned) old].new == 0)
		بازگشت ENOENT;
	*ent = ma->entries[(unsigned) old];
	بازگشت 0;
}

ایستا errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			ساختار ext2_inode_relocate_entry *ent)
{
	ساختار irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_data;
	اگر (orig > ma->max_inode)
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(unsigned) orig];
	اگر (ino == 0)
		بازگشت ENOENT;
	*old = ino;
	*ent = ma->entries[(unsigned) ino];
	بازگشت 0;
}

ایستا errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	بازگشت 0;
}

ایستا errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			 ساختار ext2_inode_relocate_entry *ent)
{
	ساختار irel_ma 	*ma;

	ma = irel->priv_data;
	وقتی (++irel->current < ma->max_inode) {
		اگر (ma->entries[(unsigned) irel->current].new == 0)
			ادامه;
		*old = irel->current;
		*ent = ma->entries[(unsigned) irel->current];
		بازگشت 0;
	}
	*old = 0;
	بازگشت 0;
}

ایستا errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     ساختار ext2_inode_reference *ref)
{
	ساختار irel_ma 	*ma;
	size_t		size;
	ساختار inode_reference_entry *ref_ent;
	ساختار ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_data;
	اگر (ino > ma->max_inode)
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (unsigned) ino;
	ent = ma->entries + (unsigned) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	اگر (ref_ent->refs == 0) {
		size = (size_t) ((اندازه(ساختار ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			اندازه(ساختار ext2_inode_reference), &ref_ent->refs);
		اگر (retval)
			بازگشت retval;
		memset(ref_ent->refs, 0, size);
		ref_ent->رقم = 0;
	}

	اگر (ref_ent->رقم >= ent->max_refs)
		بازگشت EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(unsigned) ref_ent->رقم++] = *ref;
	بازگشت 0;
}

ایستا errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	ساختار irel_ma 	*ma;

	ma = irel->priv_data;
	اگر (ino > ma->max_inode)
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	اگر (ma->entries[(unsigned) ino].new == 0)
		بازگشت ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	بازگشت 0;
}

ایستا errcode_t ima_next_ref(ext2_irel irel,
			      ساختار ext2_inode_reference *ref)
{
	ساختار irel_ma 	*ma;
	ساختار inode_reference_entry *ref_ent;

	ma = irel->priv_data;

	ref_ent = ma->ref_entries + ma->ref_current;

	اگر ((ref_ent->refs == پوچ) ||
	    (ma->ref_iter >= ref_ent->رقم)) {
		ref->block = 0;
		ref->offset = 0;
		بازگشت 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	بازگشت 0;
}


ایستا errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	ساختار irel_ma 	*ma;

	ma = irel->priv_data;
	اگر ((old > ma->max_inode) || (new > ma->max_inode))
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	اگر (ma->entries[(unsigned) old].new == 0)
		بازگشت ENOENT;

	ma->entries[(unsigned) new] = ma->entries[(unsigned) old];
	اگر (ma->ref_entries[(unsigned) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) new].refs);
	ma->ref_entries[(unsigned) new] = ma->ref_entries[(unsigned) old];

	ma->entries[(unsigned) old].new = 0;
	ma->ref_entries[(unsigned) old].رقم = 0;
	ma->ref_entries[(unsigned) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	بازگشت 0;
}

ایستا errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	ساختار irel_ma 	*ma;

	ma = irel->priv_data;
	اگر (old > ma->max_inode)
		بازگشت EXT2_ET_INVALID_ARGUMENT;
	اگر (ma->entries[(unsigned) old].new == 0)
		بازگشت ENOENT;

	ma->entries[old].new = 0;
	اگر (ma->ref_entries[(unsigned) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) old].refs);
	ma->orig_map[ma->entries[(unsigned) old].orig] = 0;

	ma->ref_entries[(unsigned) old].رقم = 0;
	ma->ref_entries[(unsigned) old].refs = 0;
	بازگشت 0;
}

ایستا errcode_t ima_free(ext2_irel irel)
{
	ساختار irel_ma 	*ma;
	ext2_ino_t	ino;

	اگر (!irel)
		بازگشت 0;

	ma = irel->priv_data;

	اگر (ma) {
		اگر (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		اگر (ma->entries)
			ext2fs_free_mem(&ma->entries);
		اگر (ma->ref_entries) {
			برای (ino = 0; ino <= ma->max_inode; ino++) {
				اگر (ma->ref_entries[(unsigned) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(unsigned) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	اگر (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	بازگشت 0;
}
