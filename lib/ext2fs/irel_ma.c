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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "irel.h"

स्थिर errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 संरचना ext2_inode_relocate_प्रविष्टिः *ent);
स्थिर errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 संरचना ext2_inode_relocate_प्रविष्टिः *ent);
स्थिर errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 संरचना ext2_inode_relocate_प्रविष्टिः *ent);
स्थिर errcode_t ima_start_iter(ext2_irel irel);
स्थिर errcode_t ima_अग्र(ext2_irel irel, ext2_ino_t *old,
			  संरचना ext2_inode_relocate_प्रविष्टिः *ent);
स्थिर errcode_t ima_सङ्कलय_ref(ext2_irel irel, ext2_ino_t ino,
			     संरचना ext2_inode_reference *ref);
स्थिर errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
स्थिर errcode_t ima_अग्र_ref(ext2_irel irel, संरचना ext2_inode_reference *ref);
स्थिर errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
स्थिर errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
स्थिर errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
संरचना inode_reference_प्रविष्टिः {
	__u16 अङ्कः;
	संरचना ext2_inode_reference *refs;
};

संरचना irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	पूर्णाङ्क   ref_iter;
	ext2_ino_t	*orig_map;
	संरचना ext2_inode_relocate_प्रविष्टिः *entries;
	संरचना inode_reference_प्रविष्टिः *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(वर्ण *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	संरचना irel_ma 	*ma = 0;
	size_t		आकार;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(परिमाण(संरचना ext2_inode_relocation_सारणी),
				&irel);
	यदि (retval)
		गच्छ errout;
	स्मृतिं_पूरय(irel, 0, परिमाण(संरचना ext2_inode_relocation_सारणी));

	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(name)+1, &irel->name);
	यदि (retval)
		गच्छ errout;
	वाक्यं_प्रतिलिख(irel->name, name);

	retval = ext2fs_get_mem(परिमाण(संरचना irel_ma), &ma);
	यदि (retval)
		गच्छ errout;
	स्मृतिं_पूरय(ma, 0, परिमाण(संरचना irel_ma));
	irel->priv_दत्त = ma;

	आकार = (size_t) (परिमाण(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, परिमाण(ext2_ino_t),
		&ma->orig_map);
	यदि (retval)
		गच्छ errout;
	स्मृतिं_पूरय(ma->orig_map, 0, आकार);

	आकार = (size_t) (परिमाण(संरचना ext2_inode_relocate_प्रविष्टिः) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		परिमाण(संरचना ext2_inode_relocate_प्रविष्टिः), &ma->entries);
	यदि (retval)
		गच्छ errout;
	स्मृतिं_पूरय(ma->entries, 0, आकार);

	आकार = (size_t) (परिमाण(संरचना inode_reference_प्रविष्टिः) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		परिमाण(संरचना inode_reference_प्रविष्टिः), &ma->ref_entries);
	यदि (retval)
		गच्छ errout;
	स्मृतिं_पूरय(ma->ref_entries, 0, आकार);
	ma->max_inode = max_inode;

	/*
	 * Fill in the irel data structure
	 */
	irel->put = ima_put;
	irel->get = ima_get;
	irel->get_by_orig = ima_get_by_orig;
	irel->start_iter = ima_start_iter;
	irel->अग्र = ima_अग्र;
	irel->सङ्कलय_ref = ima_सङ्कलय_ref;
	irel->start_iter_ref = ima_start_iter_ref;
	irel->अग्र_ref = ima_अग्र_ref;
	irel->move = ima_move;
	irel->delete = ima_delete;
	irel->मुक्त = ima_free;

	*new_irel = irel;
	प्रत्यागम 0;

errout:
	ima_free(irel);
	प्रत्यागम retval;
}

स्थिर errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			संरचना ext2_inode_relocate_प्रविष्टिः *ent)
{
	संरचना inode_reference_प्रविष्टिः	*ref_ent;
	संरचना irel_ma 			*ma;
	errcode_t			retval;
	size_t				आकार, old_आकार;

	ma = irel->priv_दत्त;
	यदि (old > ma->max_inode)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	यदि (ma->entries[(अचिह्नित) old].new == 0)
		ent->orig = old;
	अन्यथा
		ent->orig = ma->entries[(अचिह्नित) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (अचिह्नित) old;
	यदि (ref_ent->refs && ent->max_refs !=
	    ma->entries[(अचिह्नित) old].max_refs) {
		आकार = (परिमाण(संरचना ext2_inode_reference) * ent->max_refs);
		old_आकार = (परिमाण(संरचना ext2_inode_reference) *
			    ma->entries[(अचिह्नित) old].max_refs);
		retval = ext2fs_resize_mem(old_आकार, आकार, &ref_ent->refs);
		यदि (retval)
			प्रत्यागम retval;
	}

	ma->entries[(अचिह्नित) old] = *ent;
	ma->orig_map[(अचिह्नित) ent->orig] = old;
	प्रत्यागम 0;
}

स्थिर errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			संरचना ext2_inode_relocate_प्रविष्टिः *ent)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_दत्त;
	यदि (old > ma->max_inode)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	यदि (ma->entries[(अचिह्नित) old].new == 0)
		प्रत्यागम ENOENT;
	*ent = ma->entries[(अचिह्नित) old];
	प्रत्यागम 0;
}

स्थिर errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			संरचना ext2_inode_relocate_प्रविष्टिः *ent)
{
	संरचना irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_दत्त;
	यदि (orig > ma->max_inode)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	ino = ma->orig_map[(अचिह्नित) orig];
	यदि (ino == 0)
		प्रत्यागम ENOENT;
	*old = ino;
	*ent = ma->entries[(अचिह्नित) ino];
	प्रत्यागम 0;
}

स्थिर errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t ima_अग्र(ext2_irel irel, ext2_ino_t *old,
			 संरचना ext2_inode_relocate_प्रविष्टिः *ent)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_दत्त;
	यावत् (++irel->current < ma->max_inode) {
		यदि (ma->entries[(अचिह्नित) irel->current].new == 0)
			अनुवर्त;
		*old = irel->current;
		*ent = ma->entries[(अचिह्नित) irel->current];
		प्रत्यागम 0;
	}
	*old = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t ima_सङ्कलय_ref(ext2_irel irel, ext2_ino_t ino,
			     संरचना ext2_inode_reference *ref)
{
	संरचना irel_ma 	*ma;
	size_t		आकार;
	संरचना inode_reference_प्रविष्टिः *ref_ent;
	संरचना ext2_inode_relocate_प्रविष्टिः *ent;
	errcode_t		retval;

	ma = irel->priv_दत्त;
	यदि (ino > ma->max_inode)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	ref_ent = ma->ref_entries + (अचिह्नित) ino;
	ent = ma->entries + (अचिह्नित) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	यदि (ref_ent->refs == 0) {
		आकार = (size_t) ((परिमाण(संरचना ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			परिमाण(संरचना ext2_inode_reference), &ref_ent->refs);
		यदि (retval)
			प्रत्यागम retval;
		स्मृतिं_पूरय(ref_ent->refs, 0, आकार);
		ref_ent->अङ्कः = 0;
	}

	यदि (ref_ent->अङ्कः >= ent->max_refs)
		प्रत्यागम EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(अचिह्नित) ref_ent->अङ्कः++] = *ref;
	प्रत्यागम 0;
}

स्थिर errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_दत्त;
	यदि (ino > ma->max_inode)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	यदि (ma->entries[(अचिह्नित) ino].new == 0)
		प्रत्यागम ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t ima_अग्र_ref(ext2_irel irel,
			      संरचना ext2_inode_reference *ref)
{
	संरचना irel_ma 	*ma;
	संरचना inode_reference_प्रविष्टिः *ref_ent;

	ma = irel->priv_दत्त;

	ref_ent = ma->ref_entries + ma->ref_current;

	यदि ((ref_ent->refs == रिक्त) ||
	    (ma->ref_iter >= ref_ent->अङ्कः)) {
		ref->खण्डम् = 0;
		ref->विस्थापनम् = 0;
		प्रत्यागम 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	प्रत्यागम 0;
}


स्थिर errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_दत्त;
	यदि ((old > ma->max_inode) || (new > ma->max_inode))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	यदि (ma->entries[(अचिह्नित) old].new == 0)
		प्रत्यागम ENOENT;

	ma->entries[(अचिह्नित) new] = ma->entries[(अचिह्नित) old];
	यदि (ma->ref_entries[(अचिह्नित) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(अचिह्नित) new].refs);
	ma->ref_entries[(अचिह्नित) new] = ma->ref_entries[(अचिह्नित) old];

	ma->entries[(अचिह्नित) old].new = 0;
	ma->ref_entries[(अचिह्नित) old].अङ्कः = 0;
	ma->ref_entries[(अचिह्नित) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	प्रत्यागम 0;
}

स्थिर errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_दत्त;
	यदि (old > ma->max_inode)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;
	यदि (ma->entries[(अचिह्नित) old].new == 0)
		प्रत्यागम ENOENT;

	ma->entries[old].new = 0;
	यदि (ma->ref_entries[(अचिह्नित) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(अचिह्नित) old].refs);
	ma->orig_map[ma->entries[(अचिह्नित) old].orig] = 0;

	ma->ref_entries[(अचिह्नित) old].अङ्कः = 0;
	ma->ref_entries[(अचिह्नित) old].refs = 0;
	प्रत्यागम 0;
}

स्थिर errcode_t ima_free(ext2_irel irel)
{
	संरचना irel_ma 	*ma;
	ext2_ino_t	ino;

	यदि (!irel)
		प्रत्यागम 0;

	ma = irel->priv_दत्त;

	यदि (ma) {
		यदि (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		यदि (ma->entries)
			ext2fs_free_mem(&ma->entries);
		यदि (ma->ref_entries) {
			प्रति (ino = 0; ino <= ma->max_inode; ino++) {
				यदि (ma->ref_entries[(अचिह्नित) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(अचिह्नित) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	यदि (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	प्रत्यागम 0;
}
