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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "irel.h"

ସ୍ଥିର errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 ସଂରଚନା ext2_inode_relocate_entry *ent);
ସ୍ଥିର errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 ସଂରଚନା ext2_inode_relocate_entry *ent);
ସ୍ଥିର errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 ସଂରଚନା ext2_inode_relocate_entry *ent);
ସ୍ଥିର errcode_t ima_start_iter(ext2_irel irel);
ସ୍ଥିର errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			  ସଂରଚନା ext2_inode_relocate_entry *ent);
ସ୍ଥିର errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     ସଂରଚନା ext2_inode_reference *ref);
ସ୍ଥିର errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
ସ୍ଥିର errcode_t ima_next_ref(ext2_irel irel, ସଂରଚନା ext2_inode_reference *ref);
ସ୍ଥିର errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
ସ୍ଥିର errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
ସ୍ଥିର errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
ସଂରଚନା inode_reference_entry {
	__u16 ଅଙ୍କ;
	ସଂରଚନା ext2_inode_reference *refs;
};

ସଂରଚନା irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା   ref_iter;
	ext2_ino_t	*orig_map;
	ସଂରଚନା ext2_inode_relocate_entry *entries;
	ସଂରଚନା inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(ଅକ୍ଷର *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	ସଂରଚନା irel_ma 	*ma = 0;
	size_t		size;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_inode_relocation_table),
				&irel);
	ଯଦି (retval)
		ଯାଅ errout;
	memset(irel, 0, ଆକାର(ସଂରଚନା ext2_inode_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &irel->name);
	ଯଦି (retval)
		ଯାଅ errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା irel_ma), &ma);
	ଯଦି (retval)
		ଯାଅ errout;
	memset(ma, 0, ଆକାର(ସଂରଚନା irel_ma));
	irel->priv_data = ma;

	size = (size_t) (ଆକାର(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, ଆକାର(ext2_ino_t),
		&ma->orig_map);
	ଯଦି (retval)
		ଯାଅ errout;
	memset(ma->orig_map, 0, size);

	size = (size_t) (ଆକାର(ସଂରଚନା ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		ଆକାର(ସଂରଚନା ext2_inode_relocate_entry), &ma->entries);
	ଯଦି (retval)
		ଯାଅ errout;
	memset(ma->entries, 0, size);

	size = (size_t) (ଆକାର(ସଂରଚନା inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		ଆକାର(ସଂରଚନା inode_reference_entry), &ma->ref_entries);
	ଯଦି (retval)
		ଯାଅ errout;
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
	irel->ମୁକ୍ତ = ima_free;

	*new_irel = irel;
	ଫେରନ୍ତୁ 0;

errout:
	ima_free(irel);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			ସଂରଚନା ext2_inode_relocate_entry *ent)
{
	ସଂରଚନା inode_reference_entry	*ref_ent;
	ସଂରଚନା irel_ma 			*ma;
	errcode_t			retval;
	size_t				size, old_size;

	ma = irel->priv_data;
	ଯଦି (old > ma->max_inode)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	ଯଦି (ma->entries[(unsigned) old].new == 0)
		ent->orig = old;
	ଅନ୍ୟଥା
		ent->orig = ma->entries[(unsigned) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (unsigned) old;
	ଯଦି (ref_ent->refs && ent->max_refs !=
	    ma->entries[(unsigned) old].max_refs) {
		size = (ଆକାର(ସଂରଚନା ext2_inode_reference) * ent->max_refs);
		old_size = (ଆକାର(ସଂରଚନା ext2_inode_reference) *
			    ma->entries[(unsigned) old].max_refs);
		retval = ext2fs_resize_mem(old_size, size, &ref_ent->refs);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}

	ma->entries[(unsigned) old] = *ent;
	ma->orig_map[(unsigned) ent->orig] = old;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			ସଂରଚନା ext2_inode_relocate_entry *ent)
{
	ସଂରଚନା irel_ma 	*ma;

	ma = irel->priv_data;
	ଯଦି (old > ma->max_inode)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	ଯଦି (ma->entries[(unsigned) old].new == 0)
		ଫେରନ୍ତୁ ENOENT;
	*ent = ma->entries[(unsigned) old];
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			ସଂରଚନା ext2_inode_relocate_entry *ent)
{
	ସଂରଚନା irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_data;
	ଯଦି (orig > ma->max_inode)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(unsigned) orig];
	ଯଦି (ino == 0)
		ଫେରନ୍ତୁ ENOENT;
	*old = ino;
	*ent = ma->entries[(unsigned) ino];
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			 ସଂରଚନା ext2_inode_relocate_entry *ent)
{
	ସଂରଚନା irel_ma 	*ma;

	ma = irel->priv_data;
	ଯେତେବେଳେ (++irel->current < ma->max_inode) {
		ଯଦି (ma->entries[(unsigned) irel->current].new == 0)
			ଜାରି;
		*old = irel->current;
		*ent = ma->entries[(unsigned) irel->current];
		ଫେରନ୍ତୁ 0;
	}
	*old = 0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     ସଂରଚନା ext2_inode_reference *ref)
{
	ସଂରଚନା irel_ma 	*ma;
	size_t		size;
	ସଂରଚନା inode_reference_entry *ref_ent;
	ସଂରଚନା ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_data;
	ଯଦି (ino > ma->max_inode)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (unsigned) ino;
	ent = ma->entries + (unsigned) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	ଯଦି (ref_ent->refs == 0) {
		size = (size_t) ((ଆକାର(ସଂରଚନା ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			ଆକାର(ସଂରଚନା ext2_inode_reference), &ref_ent->refs);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		memset(ref_ent->refs, 0, size);
		ref_ent->ଅଙ୍କ = 0;
	}

	ଯଦି (ref_ent->ଅଙ୍କ >= ent->max_refs)
		ଫେରନ୍ତୁ EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(unsigned) ref_ent->ଅଙ୍କ++] = *ref;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	ସଂରଚନା irel_ma 	*ma;

	ma = irel->priv_data;
	ଯଦି (ino > ma->max_inode)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	ଯଦି (ma->entries[(unsigned) ino].new == 0)
		ଫେରନ୍ତୁ ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ima_next_ref(ext2_irel irel,
			      ସଂରଚନା ext2_inode_reference *ref)
{
	ସଂରଚନା irel_ma 	*ma;
	ସଂରଚନା inode_reference_entry *ref_ent;

	ma = irel->priv_data;

	ref_ent = ma->ref_entries + ma->ref_current;

	ଯଦି ((ref_ent->refs == ଖାଲି) ||
	    (ma->ref_iter >= ref_ent->ଅଙ୍କ)) {
		ref->block = 0;
		ref->offset = 0;
		ଫେରନ୍ତୁ 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	ଫେରନ୍ତୁ 0;
}


ସ୍ଥିର errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	ସଂରଚନା irel_ma 	*ma;

	ma = irel->priv_data;
	ଯଦି ((old > ma->max_inode) || (new > ma->max_inode))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	ଯଦି (ma->entries[(unsigned) old].new == 0)
		ଫେରନ୍ତୁ ENOENT;

	ma->entries[(unsigned) new] = ma->entries[(unsigned) old];
	ଯଦି (ma->ref_entries[(unsigned) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) new].refs);
	ma->ref_entries[(unsigned) new] = ma->ref_entries[(unsigned) old];

	ma->entries[(unsigned) old].new = 0;
	ma->ref_entries[(unsigned) old].ଅଙ୍କ = 0;
	ma->ref_entries[(unsigned) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	ସଂରଚନା irel_ma 	*ma;

	ma = irel->priv_data;
	ଯଦି (old > ma->max_inode)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;
	ଯଦି (ma->entries[(unsigned) old].new == 0)
		ଫେରନ୍ତୁ ENOENT;

	ma->entries[old].new = 0;
	ଯଦି (ma->ref_entries[(unsigned) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) old].refs);
	ma->orig_map[ma->entries[(unsigned) old].orig] = 0;

	ma->ref_entries[(unsigned) old].ଅଙ୍କ = 0;
	ma->ref_entries[(unsigned) old].refs = 0;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ima_free(ext2_irel irel)
{
	ସଂରଚନା irel_ma 	*ma;
	ext2_ino_t	ino;

	ଯଦି (!irel)
		ଫେରନ୍ତୁ 0;

	ma = irel->priv_data;

	ଯଦି (ma) {
		ଯଦି (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		ଯଦି (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ଯଦି (ma->ref_entries) {
			ପାଇଁ (ino = 0; ino <= ma->max_inode; ino++) {
				ଯଦି (ma->ref_entries[(unsigned) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(unsigned) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	ଯଦି (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	ଫେରନ୍ତୁ 0;
}
