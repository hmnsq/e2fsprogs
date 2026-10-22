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

#अन्तर्भाव "config.h"
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#सचे HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "irel.h"

स्थिर errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 संरचना ext2_inode_relocate_entry *ent);
स्थिर errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 संरचना ext2_inode_relocate_entry *ent);
स्थिर errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 संरचना ext2_inode_relocate_entry *ent);
स्थिर errcode_t ima_start_iter(ext2_irel irel);
स्थिर errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			  संरचना ext2_inode_relocate_entry *ent);
स्थिर errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     संरचना ext2_inode_reference *ref);
स्थिर errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
स्थिर errcode_t ima_next_ref(ext2_irel irel, संरचना ext2_inode_reference *ref);
स्थिर errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
स्थिर errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
स्थिर errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
संरचना inode_reference_entry {
	__u16 अङ्कः;
	संरचना ext2_inode_reference *refs;
};

संरचना irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	पूर्णाङ्क   ref_iter;
	ext2_ino_t	*orig_map;
	संरचना ext2_inode_relocate_entry *entries;
	संरचना inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(वर्ण *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	संरचना irel_ma 	*ma = 0;
	size_t		size;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(परिमाण(संरचना ext2_inode_relocation_table),
				&irel);
	सचे (retval)
		गच्छ errout;
	memset(irel, 0, परिमाण(संरचना ext2_inode_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &irel->name);
	सचे (retval)
		गच्छ errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(परिमाण(संरचना irel_ma), &ma);
	सचे (retval)
		गच्छ errout;
	memset(ma, 0, परिमाण(संरचना irel_ma));
	irel->priv_data = ma;

	size = (size_t) (परिमाण(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, परिमाण(ext2_ino_t),
		&ma->orig_map);
	सचे (retval)
		गच्छ errout;
	memset(ma->orig_map, 0, size);

	size = (size_t) (परिमाण(संरचना ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		परिमाण(संरचना ext2_inode_relocate_entry), &ma->entries);
	सचे (retval)
		गच्छ errout;
	memset(ma->entries, 0, size);

	size = (size_t) (परिमाण(संरचना inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		परिमाण(संरचना inode_reference_entry), &ma->ref_entries);
	सचे (retval)
		गच्छ errout;
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
	irel->मुक्त = ima_free;

	*new_irel = irel;
	पटिनिवत्त 0;

errout:
	ima_free(irel);
	पटिनिवत्त retval;
}

स्थिर errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			संरचना ext2_inode_relocate_entry *ent)
{
	संरचना inode_reference_entry	*ref_ent;
	संरचना irel_ma 			*ma;
	errcode_t			retval;
	size_t				size, old_size;

	ma = irel->priv_data;
	सचे (old > ma->max_inode)
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	सचे (ma->entries[(unsigned) old].new == 0)
		ent->orig = old;
	अञ्ञथा
		ent->orig = ma->entries[(unsigned) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (unsigned) old;
	सचे (ref_ent->refs && ent->max_refs !=
	    ma->entries[(unsigned) old].max_refs) {
		size = (परिमाण(संरचना ext2_inode_reference) * ent->max_refs);
		old_size = (परिमाण(संरचना ext2_inode_reference) *
			    ma->entries[(unsigned) old].max_refs);
		retval = ext2fs_resize_mem(old_size, size, &ref_ent->refs);
		सचे (retval)
			पटिनिवत्त retval;
	}

	ma->entries[(unsigned) old] = *ent;
	ma->orig_map[(unsigned) ent->orig] = old;
	पटिनिवत्त 0;
}

स्थिर errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			संरचना ext2_inode_relocate_entry *ent)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_data;
	सचे (old > ma->max_inode)
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;
	सचे (ma->entries[(unsigned) old].new == 0)
		पटिनिवत्त ENOENT;
	*ent = ma->entries[(unsigned) old];
	पटिनिवत्त 0;
}

स्थिर errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			संरचना ext2_inode_relocate_entry *ent)
{
	संरचना irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_data;
	सचे (orig > ma->max_inode)
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(unsigned) orig];
	सचे (ino == 0)
		पटिनिवत्त ENOENT;
	*old = ino;
	*ent = ma->entries[(unsigned) ino];
	पटिनिवत्त 0;
}

स्थिर errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	पटिनिवत्त 0;
}

स्थिर errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			 संरचना ext2_inode_relocate_entry *ent)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_data;
	याव (++irel->current < ma->max_inode) {
		सचे (ma->entries[(unsigned) irel->current].new == 0)
			अनुवर्त;
		*old = irel->current;
		*ent = ma->entries[(unsigned) irel->current];
		पटिनिवत्त 0;
	}
	*old = 0;
	पटिनिवत्त 0;
}

स्थिर errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     संरचना ext2_inode_reference *ref)
{
	संरचना irel_ma 	*ma;
	size_t		size;
	संरचना inode_reference_entry *ref_ent;
	संरचना ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_data;
	सचे (ino > ma->max_inode)
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (unsigned) ino;
	ent = ma->entries + (unsigned) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	सचे (ref_ent->refs == 0) {
		size = (size_t) ((परिमाण(संरचना ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			परिमाण(संरचना ext2_inode_reference), &ref_ent->refs);
		सचे (retval)
			पटिनिवत्त retval;
		memset(ref_ent->refs, 0, size);
		ref_ent->अङ्कः = 0;
	}

	सचे (ref_ent->अङ्कः >= ent->max_refs)
		पटिनिवत्त EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(unsigned) ref_ent->अङ्कः++] = *ref;
	पटिनिवत्त 0;
}

स्थिर errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_data;
	सचे (ino > ma->max_inode)
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;
	सचे (ma->entries[(unsigned) ino].new == 0)
		पटिनिवत्त ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	पटिनिवत्त 0;
}

स्थिर errcode_t ima_next_ref(ext2_irel irel,
			      संरचना ext2_inode_reference *ref)
{
	संरचना irel_ma 	*ma;
	संरचना inode_reference_entry *ref_ent;

	ma = irel->priv_data;

	ref_ent = ma->ref_entries + ma->ref_current;

	सचे ((ref_ent->refs == रिक्त) ||
	    (ma->ref_iter >= ref_ent->अङ्कः)) {
		ref->block = 0;
		ref->offset = 0;
		पटिनिवत्त 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	पटिनिवत्त 0;
}


स्थिर errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_data;
	सचे ((old > ma->max_inode) || (new > ma->max_inode))
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;
	सचे (ma->entries[(unsigned) old].new == 0)
		पटिनिवत्त ENOENT;

	ma->entries[(unsigned) new] = ma->entries[(unsigned) old];
	सचे (ma->ref_entries[(unsigned) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) new].refs);
	ma->ref_entries[(unsigned) new] = ma->ref_entries[(unsigned) old];

	ma->entries[(unsigned) old].new = 0;
	ma->ref_entries[(unsigned) old].अङ्कः = 0;
	ma->ref_entries[(unsigned) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	पटिनिवत्त 0;
}

स्थिर errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_data;
	सचे (old > ma->max_inode)
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;
	सचे (ma->entries[(unsigned) old].new == 0)
		पटिनिवत्त ENOENT;

	ma->entries[old].new = 0;
	सचे (ma->ref_entries[(unsigned) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) old].refs);
	ma->orig_map[ma->entries[(unsigned) old].orig] = 0;

	ma->ref_entries[(unsigned) old].अङ्कः = 0;
	ma->ref_entries[(unsigned) old].refs = 0;
	पटिनिवत्त 0;
}

स्थिर errcode_t ima_free(ext2_irel irel)
{
	संरचना irel_ma 	*ma;
	ext2_ino_t	ino;

	सचे (!irel)
		पटिनिवत्त 0;

	ma = irel->priv_data;

	सचे (ma) {
		सचे (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		सचे (ma->entries)
			ext2fs_free_mem(&ma->entries);
		सचे (ma->ref_entries) {
			अत्थाय (ino = 0; ino <= ma->max_inode; ino++) {
				सचे (ma->ref_entries[(unsigned) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(unsigned) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	सचे (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	पटिनिवत्त 0;
}
