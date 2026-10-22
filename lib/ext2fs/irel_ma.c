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

#include "config.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#nếu HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "irel.h"

static errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 struct ext2_inode_relocate_entry *ent);
static errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 struct ext2_inode_relocate_entry *ent);
static errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 struct ext2_inode_relocate_entry *ent);
static errcode_t ima_start_iter(ext2_irel irel);
static errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			  struct ext2_inode_relocate_entry *ent);
static errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     struct ext2_inode_reference *ref);
static errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
static errcode_t ima_next_ref(ext2_irel irel, struct ext2_inode_reference *ref);
static errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
static errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
static errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
struct inode_reference_entry {
	__u16 num;
	struct ext2_inode_reference *refs;
};

struct irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	số_nguyên   ref_iter;
	ext2_ino_t	*orig_map;
	struct ext2_inode_relocate_entry *entries;
	struct inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(char *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	struct irel_ma 	*ma = 0;
	size_t		size;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(sizeof(struct ext2_inode_relocation_table),
				&irel);
	nếu (retval)
		goto errout;
	memset(irel, 0, sizeof(struct ext2_inode_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &irel->name);
	nếu (retval)
		goto errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(sizeof(struct irel_ma), &ma);
	nếu (retval)
		goto errout;
	memset(ma, 0, sizeof(struct irel_ma));
	irel->priv_data = ma;

	size = (size_t) (sizeof(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, sizeof(ext2_ino_t),
		&ma->orig_map);
	nếu (retval)
		goto errout;
	memset(ma->orig_map, 0, size);

	size = (size_t) (sizeof(struct ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		sizeof(struct ext2_inode_relocate_entry), &ma->entries);
	nếu (retval)
		goto errout;
	memset(ma->entries, 0, size);

	size = (size_t) (sizeof(struct inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		sizeof(struct inode_reference_entry), &ma->ref_entries);
	nếu (retval)
		goto errout;
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
	irel->free = ima_free;

	*new_irel = irel;
	trả_về 0;

errout:
	ima_free(irel);
	trả_về retval;
}

static errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			struct ext2_inode_relocate_entry *ent)
{
	struct inode_reference_entry	*ref_ent;
	struct irel_ma 			*ma;
	errcode_t			retval;
	size_t				size, old_size;

	ma = irel->priv_data;
	nếu (old > ma->max_inode)
		trả_về EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	nếu (ma->entries[(unsigned) old].new == 0)
		ent->orig = old;
	không_thì
		ent->orig = ma->entries[(unsigned) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (unsigned) old;
	nếu (ref_ent->refs && ent->max_refs !=
	    ma->entries[(unsigned) old].max_refs) {
		size = (sizeof(struct ext2_inode_reference) * ent->max_refs);
		old_size = (sizeof(struct ext2_inode_reference) *
			    ma->entries[(unsigned) old].max_refs);
		retval = ext2fs_resize_mem(old_size, size, &ref_ent->refs);
		nếu (retval)
			trả_về retval;
	}

	ma->entries[(unsigned) old] = *ent;
	ma->orig_map[(unsigned) ent->orig] = old;
	trả_về 0;
}

static errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			struct ext2_inode_relocate_entry *ent)
{
	struct irel_ma 	*ma;

	ma = irel->priv_data;
	nếu (old > ma->max_inode)
		trả_về EXT2_ET_INVALID_ARGUMENT;
	nếu (ma->entries[(unsigned) old].new == 0)
		trả_về ENOENT;
	*ent = ma->entries[(unsigned) old];
	trả_về 0;
}

static errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			struct ext2_inode_relocate_entry *ent)
{
	struct irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_data;
	nếu (orig > ma->max_inode)
		trả_về EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(unsigned) orig];
	nếu (ino == 0)
		trả_về ENOENT;
	*old = ino;
	*ent = ma->entries[(unsigned) ino];
	trả_về 0;
}

static errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	trả_về 0;
}

static errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			 struct ext2_inode_relocate_entry *ent)
{
	struct irel_ma 	*ma;

	ma = irel->priv_data;
	trong_khi (++irel->current < ma->max_inode) {
		nếu (ma->entries[(unsigned) irel->current].new == 0)
			continue;
		*old = irel->current;
		*ent = ma->entries[(unsigned) irel->current];
		trả_về 0;
	}
	*old = 0;
	trả_về 0;
}

static errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     struct ext2_inode_reference *ref)
{
	struct irel_ma 	*ma;
	size_t		size;
	struct inode_reference_entry *ref_ent;
	struct ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_data;
	nếu (ino > ma->max_inode)
		trả_về EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (unsigned) ino;
	ent = ma->entries + (unsigned) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	nếu (ref_ent->refs == 0) {
		size = (size_t) ((sizeof(struct ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			sizeof(struct ext2_inode_reference), &ref_ent->refs);
		nếu (retval)
			trả_về retval;
		memset(ref_ent->refs, 0, size);
		ref_ent->num = 0;
	}

	nếu (ref_ent->num >= ent->max_refs)
		trả_về EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(unsigned) ref_ent->num++] = *ref;
	trả_về 0;
}

static errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	struct irel_ma 	*ma;

	ma = irel->priv_data;
	nếu (ino > ma->max_inode)
		trả_về EXT2_ET_INVALID_ARGUMENT;
	nếu (ma->entries[(unsigned) ino].new == 0)
		trả_về ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	trả_về 0;
}

static errcode_t ima_next_ref(ext2_irel irel,
			      struct ext2_inode_reference *ref)
{
	struct irel_ma 	*ma;
	struct inode_reference_entry *ref_ent;

	ma = irel->priv_data;

	ref_ent = ma->ref_entries + ma->ref_current;

	nếu ((ref_ent->refs == NULL) ||
	    (ma->ref_iter >= ref_ent->num)) {
		ref->block = 0;
		ref->offset = 0;
		trả_về 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	trả_về 0;
}


static errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	struct irel_ma 	*ma;

	ma = irel->priv_data;
	nếu ((old > ma->max_inode) || (new > ma->max_inode))
		trả_về EXT2_ET_INVALID_ARGUMENT;
	nếu (ma->entries[(unsigned) old].new == 0)
		trả_về ENOENT;

	ma->entries[(unsigned) new] = ma->entries[(unsigned) old];
	nếu (ma->ref_entries[(unsigned) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) new].refs);
	ma->ref_entries[(unsigned) new] = ma->ref_entries[(unsigned) old];

	ma->entries[(unsigned) old].new = 0;
	ma->ref_entries[(unsigned) old].num = 0;
	ma->ref_entries[(unsigned) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	trả_về 0;
}

static errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	struct irel_ma 	*ma;

	ma = irel->priv_data;
	nếu (old > ma->max_inode)
		trả_về EXT2_ET_INVALID_ARGUMENT;
	nếu (ma->entries[(unsigned) old].new == 0)
		trả_về ENOENT;

	ma->entries[old].new = 0;
	nếu (ma->ref_entries[(unsigned) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) old].refs);
	ma->orig_map[ma->entries[(unsigned) old].orig] = 0;

	ma->ref_entries[(unsigned) old].num = 0;
	ma->ref_entries[(unsigned) old].refs = 0;
	trả_về 0;
}

static errcode_t ima_free(ext2_irel irel)
{
	struct irel_ma 	*ma;
	ext2_ino_t	ino;

	nếu (!irel)
		trả_về 0;

	ma = irel->priv_data;

	nếu (ma) {
		nếu (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		nếu (ma->entries)
			ext2fs_free_mem(&ma->entries);
		nếu (ma->ref_entries) {
			cho (ino = 0; ino <= ma->max_inode; ino++) {
				nếu (ma->ref_entries[(unsigned) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(unsigned) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	nếu (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	trả_về 0;
}
