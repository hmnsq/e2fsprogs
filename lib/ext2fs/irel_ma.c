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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "irel.h"

স্থির errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 কাঠামো ext2_inode_relocate_entry *ent);
স্থির errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 কাঠামো ext2_inode_relocate_entry *ent);
স্থির errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 কাঠামো ext2_inode_relocate_entry *ent);
স্থির errcode_t ima_start_iter(ext2_irel irel);
স্থির errcode_t ima_পরবর্তী(ext2_irel irel, ext2_ino_t *old,
			  কাঠামো ext2_inode_relocate_entry *ent);
স্থির errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     কাঠামো ext2_inode_reference *ref);
স্থির errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
স্থির errcode_t ima_পরবর্তী_ref(ext2_irel irel, কাঠামো ext2_inode_reference *ref);
স্থির errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
স্থির errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
স্থির errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
কাঠামো inode_reference_entry {
	__u16 অঙ্ক;
	কাঠামো ext2_inode_reference *refs;
};

কাঠামো irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	পূর্ণসংখ্যা   ref_iter;
	ext2_ino_t	*orig_map;
	কাঠামো ext2_inode_relocate_entry *entries;
	কাঠামো inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(অক্ষর *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	কাঠামো irel_ma 	*ma = 0;
	size_t		আকার_2;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(আকার(কাঠামো ext2_inode_relocation_table),
				&irel);
	যদি (retval)
		যাও errout;
	memset(irel, 0, আকার(কাঠামো ext2_inode_relocation_table));

	retval = ext2fs_get_mem(strlen(name)+1, &irel->name);
	যদি (retval)
		যাও errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(আকার(কাঠামো irel_ma), &ma);
	যদি (retval)
		যাও errout;
	memset(ma, 0, আকার(কাঠামো irel_ma));
	irel->priv_তথ্য = ma;

	আকার_2 = (size_t) (আকার(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, আকার(ext2_ino_t),
		&ma->orig_map);
	যদি (retval)
		যাও errout;
	memset(ma->orig_map, 0, আকার_2);

	আকার_2 = (size_t) (আকার(কাঠামো ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		আকার(কাঠামো ext2_inode_relocate_entry), &ma->entries);
	যদি (retval)
		যাও errout;
	memset(ma->entries, 0, আকার_2);

	আকার_2 = (size_t) (আকার(কাঠামো inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		আকার(কাঠামো inode_reference_entry), &ma->ref_entries);
	যদি (retval)
		যাও errout;
	memset(ma->ref_entries, 0, আকার_2);
	ma->max_inode = max_inode;

	/*
	 * Fill in the irel data structure
	 */
	irel->put = ima_put;
	irel->get = ima_get;
	irel->get_by_orig = ima_get_by_orig;
	irel->start_iter = ima_start_iter;
	irel->পরবর্তী = ima_পরবর্তী;
	irel->add_ref = ima_add_ref;
	irel->start_iter_ref = ima_start_iter_ref;
	irel->পরবর্তী_ref = ima_পরবর্তী_ref;
	irel->move = ima_move;
	irel->delete = ima_delete;
	irel->মুক্ত = ima_free;

	*new_irel = irel;
	ফেরত 0;

errout:
	ima_free(irel);
	ফেরত retval;
}

স্থির errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			কাঠামো ext2_inode_relocate_entry *ent)
{
	কাঠামো inode_reference_entry	*ref_ent;
	কাঠামো irel_ma 			*ma;
	errcode_t			retval;
	size_t				আকার_2, old_আকার;

	ma = irel->priv_তথ্য;
	যদি (old > ma->max_inode)
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	যদি (ma->entries[(unsigned) old].new == 0)
		ent->orig = old;
	অন্যথা
		ent->orig = ma->entries[(unsigned) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (unsigned) old;
	যদি (ref_ent->refs && ent->max_refs !=
	    ma->entries[(unsigned) old].max_refs) {
		আকার_2 = (আকার(কাঠামো ext2_inode_reference) * ent->max_refs);
		old_আকার = (আকার(কাঠামো ext2_inode_reference) *
			    ma->entries[(unsigned) old].max_refs);
		retval = ext2fs_resize_mem(old_আকার, আকার_2, &ref_ent->refs);
		যদি (retval)
			ফেরত retval;
	}

	ma->entries[(unsigned) old] = *ent;
	ma->orig_map[(unsigned) ent->orig] = old;
	ফেরত 0;
}

স্থির errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			কাঠামো ext2_inode_relocate_entry *ent)
{
	কাঠামো irel_ma 	*ma;

	ma = irel->priv_তথ্য;
	যদি (old > ma->max_inode)
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	যদি (ma->entries[(unsigned) old].new == 0)
		ফেরত ENOENT;
	*ent = ma->entries[(unsigned) old];
	ফেরত 0;
}

স্থির errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			কাঠামো ext2_inode_relocate_entry *ent)
{
	কাঠামো irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_তথ্য;
	যদি (orig > ma->max_inode)
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(unsigned) orig];
	যদি (ino == 0)
		ফেরত ENOENT;
	*old = ino;
	*ent = ma->entries[(unsigned) ino];
	ফেরত 0;
}

স্থির errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	ফেরত 0;
}

স্থির errcode_t ima_পরবর্তী(ext2_irel irel, ext2_ino_t *old,
			 কাঠামো ext2_inode_relocate_entry *ent)
{
	কাঠামো irel_ma 	*ma;

	ma = irel->priv_তথ্য;
	যখন (++irel->current < ma->max_inode) {
		যদি (ma->entries[(unsigned) irel->current].new == 0)
			চালু;
		*old = irel->current;
		*ent = ma->entries[(unsigned) irel->current];
		ফেরত 0;
	}
	*old = 0;
	ফেরত 0;
}

স্থির errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     কাঠামো ext2_inode_reference *ref)
{
	কাঠামো irel_ma 	*ma;
	size_t		আকার_2;
	কাঠামো inode_reference_entry *ref_ent;
	কাঠামো ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_তথ্য;
	যদি (ino > ma->max_inode)
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (unsigned) ino;
	ent = ma->entries + (unsigned) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	যদি (ref_ent->refs == 0) {
		আকার_2 = (size_t) ((আকার(কাঠামো ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			আকার(কাঠামো ext2_inode_reference), &ref_ent->refs);
		যদি (retval)
			ফেরত retval;
		memset(ref_ent->refs, 0, আকার_2);
		ref_ent->অঙ্ক = 0;
	}

	যদি (ref_ent->অঙ্ক >= ent->max_refs)
		ফেরত EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(unsigned) ref_ent->অঙ্ক++] = *ref;
	ফেরত 0;
}

স্থির errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	কাঠামো irel_ma 	*ma;

	ma = irel->priv_তথ্য;
	যদি (ino > ma->max_inode)
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	যদি (ma->entries[(unsigned) ino].new == 0)
		ফেরত ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	ফেরত 0;
}

স্থির errcode_t ima_পরবর্তী_ref(ext2_irel irel,
			      কাঠামো ext2_inode_reference *ref)
{
	কাঠামো irel_ma 	*ma;
	কাঠামো inode_reference_entry *ref_ent;

	ma = irel->priv_তথ্য;

	ref_ent = ma->ref_entries + ma->ref_current;

	যদি ((ref_ent->refs == ফাঁকা) ||
	    (ma->ref_iter >= ref_ent->অঙ্ক)) {
		ref->block = 0;
		ref->offset = 0;
		ফেরত 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	ফেরত 0;
}


স্থির errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	কাঠামো irel_ma 	*ma;

	ma = irel->priv_তথ্য;
	যদি ((old > ma->max_inode) || (new > ma->max_inode))
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	যদি (ma->entries[(unsigned) old].new == 0)
		ফেরত ENOENT;

	ma->entries[(unsigned) new] = ma->entries[(unsigned) old];
	যদি (ma->ref_entries[(unsigned) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) new].refs);
	ma->ref_entries[(unsigned) new] = ma->ref_entries[(unsigned) old];

	ma->entries[(unsigned) old].new = 0;
	ma->ref_entries[(unsigned) old].অঙ্ক = 0;
	ma->ref_entries[(unsigned) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	ফেরত 0;
}

স্থির errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	কাঠামো irel_ma 	*ma;

	ma = irel->priv_তথ্য;
	যদি (old > ma->max_inode)
		ফেরত EXT2_ET_INVALID_ARGUMENT;
	যদি (ma->entries[(unsigned) old].new == 0)
		ফেরত ENOENT;

	ma->entries[old].new = 0;
	যদি (ma->ref_entries[(unsigned) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(unsigned) old].refs);
	ma->orig_map[ma->entries[(unsigned) old].orig] = 0;

	ma->ref_entries[(unsigned) old].অঙ্ক = 0;
	ma->ref_entries[(unsigned) old].refs = 0;
	ফেরত 0;
}

স্থির errcode_t ima_free(ext2_irel irel)
{
	কাঠামো irel_ma 	*ma;
	ext2_ino_t	ino;

	যদি (!irel)
		ফেরত 0;

	ma = irel->priv_তথ্য;

	যদি (ma) {
		যদি (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		যদি (ma->entries)
			ext2fs_free_mem(&ma->entries);
		যদি (ma->ref_entries) {
			জন্য (ino = 0; ino <= ma->max_inode; ino++) {
				যদি (ma->ref_entries[(unsigned) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(unsigned) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	যদি (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	ফেরত 0;
}
