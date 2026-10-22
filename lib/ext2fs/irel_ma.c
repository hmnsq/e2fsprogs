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

#समावेश "config.h"
#समावेश <fcntl.h>
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "irel.h"

अटल errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 काष्ठा ext2_inode_relocate_entry *ent);
अटल errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 काष्ठा ext2_inode_relocate_entry *ent);
अटल errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 काष्ठा ext2_inode_relocate_entry *ent);
अटल errcode_t ima_start_iter(ext2_irel irel);
अटल errcode_t ima_अग्र(ext2_irel irel, ext2_ino_t *old,
			  काष्ठा ext2_inode_relocate_entry *ent);
अटल errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     काष्ठा ext2_inode_reference *ref);
अटल errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino);
अटल errcode_t ima_अग्र_ref(ext2_irel irel, काष्ठा ext2_inode_reference *ref);
अटल errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
अटल errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
अटल errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
काष्ठा inode_reference_entry {
	__u16 अंक;
	काष्ठा ext2_inode_reference *refs;
};

काष्ठा irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	पूर्णांक   ref_iter;
	ext2_ino_t	*orig_map;
	काष्ठा ext2_inode_relocate_entry *entries;
	काष्ठा inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(अक्षर *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	काष्ठा irel_ma 	*ma = 0;
	माप_प्रकार		आकार;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(माप(काष्ठा ext2_inode_relocation_table),
				&irel);
	अगर (retval)
		जाओ errout;
	स_रखो(irel, 0, माप(काष्ठा ext2_inode_relocation_table));

	retval = ext2fs_get_mem(म_माप(name)+1, &irel->name);
	अगर (retval)
		जाओ errout;
	म_नकल(irel->name, name);

	retval = ext2fs_get_mem(माप(काष्ठा irel_ma), &ma);
	अगर (retval)
		जाओ errout;
	स_रखो(ma, 0, माप(काष्ठा irel_ma));
	irel->priv_तथ्य = ma;

	आकार = (माप_प्रकार) (माप(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, माप(ext2_ino_t),
		&ma->orig_map);
	अगर (retval)
		जाओ errout;
	स_रखो(ma->orig_map, 0, आकार);

	आकार = (माप_प्रकार) (माप(काष्ठा ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		माप(काष्ठा ext2_inode_relocate_entry), &ma->entries);
	अगर (retval)
		जाओ errout;
	स_रखो(ma->entries, 0, आकार);

	आकार = (माप_प्रकार) (माप(काष्ठा inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		माप(काष्ठा inode_reference_entry), &ma->ref_entries);
	अगर (retval)
		जाओ errout;
	स_रखो(ma->ref_entries, 0, आकार);
	ma->max_inode = max_inode;

	/*
	 * Fill in the irel data structure
	 */
	irel->put = ima_put;
	irel->get = ima_get;
	irel->get_by_orig = ima_get_by_orig;
	irel->start_iter = ima_start_iter;
	irel->अग्र = ima_अग्र;
	irel->add_ref = ima_add_ref;
	irel->start_iter_ref = ima_start_iter_ref;
	irel->अग्र_ref = ima_अग्र_ref;
	irel->move = ima_move;
	irel->delete = ima_delete;
	irel->मुक्त = ima_free;

	*new_irel = irel;
	वापस 0;

errout:
	ima_free(irel);
	वापस retval;
}

अटल errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			काष्ठा ext2_inode_relocate_entry *ent)
{
	काष्ठा inode_reference_entry	*ref_ent;
	काष्ठा irel_ma 			*ma;
	errcode_t			retval;
	माप_प्रकार				आकार, old_आकार;

	ma = irel->priv_तथ्य;
	अगर (old > ma->max_inode)
		वापस EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	अगर (ma->entries[(अचिन्हित) old].new == 0)
		ent->orig = old;
	अन्यथा
		ent->orig = ma->entries[(अचिन्हित) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (अचिन्हित) old;
	अगर (ref_ent->refs && ent->max_refs !=
	    ma->entries[(अचिन्हित) old].max_refs) {
		आकार = (माप(काष्ठा ext2_inode_reference) * ent->max_refs);
		old_आकार = (माप(काष्ठा ext2_inode_reference) *
			    ma->entries[(अचिन्हित) old].max_refs);
		retval = ext2fs_resize_mem(old_आकार, आकार, &ref_ent->refs);
		अगर (retval)
			वापस retval;
	}

	ma->entries[(अचिन्हित) old] = *ent;
	ma->orig_map[(अचिन्हित) ent->orig] = old;
	वापस 0;
}

अटल errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			काष्ठा ext2_inode_relocate_entry *ent)
{
	काष्ठा irel_ma 	*ma;

	ma = irel->priv_तथ्य;
	अगर (old > ma->max_inode)
		वापस EXT2_ET_INVALID_ARGUMENT;
	अगर (ma->entries[(अचिन्हित) old].new == 0)
		वापस ENOENT;
	*ent = ma->entries[(अचिन्हित) old];
	वापस 0;
}

अटल errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			काष्ठा ext2_inode_relocate_entry *ent)
{
	काष्ठा irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_तथ्य;
	अगर (orig > ma->max_inode)
		वापस EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(अचिन्हित) orig];
	अगर (ino == 0)
		वापस ENOENT;
	*old = ino;
	*ent = ma->entries[(अचिन्हित) ino];
	वापस 0;
}

अटल errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	वापस 0;
}

अटल errcode_t ima_अग्र(ext2_irel irel, ext2_ino_t *old,
			 काष्ठा ext2_inode_relocate_entry *ent)
{
	काष्ठा irel_ma 	*ma;

	ma = irel->priv_तथ्य;
	जबतक (++irel->current < ma->max_inode) {
		अगर (ma->entries[(अचिन्हित) irel->current].new == 0)
			जारी;
		*old = irel->current;
		*ent = ma->entries[(अचिन्हित) irel->current];
		वापस 0;
	}
	*old = 0;
	वापस 0;
}

अटल errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     काष्ठा ext2_inode_reference *ref)
{
	काष्ठा irel_ma 	*ma;
	माप_प्रकार		आकार;
	काष्ठा inode_reference_entry *ref_ent;
	काष्ठा ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_तथ्य;
	अगर (ino > ma->max_inode)
		वापस EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (अचिन्हित) ino;
	ent = ma->entries + (अचिन्हित) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	अगर (ref_ent->refs == 0) {
		आकार = (माप_प्रकार) ((माप(काष्ठा ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			माप(काष्ठा ext2_inode_reference), &ref_ent->refs);
		अगर (retval)
			वापस retval;
		स_रखो(ref_ent->refs, 0, आकार);
		ref_ent->अंक = 0;
	}

	अगर (ref_ent->अंक >= ent->max_refs)
		वापस EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(अचिन्हित) ref_ent->अंक++] = *ref;
	वापस 0;
}

अटल errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	काष्ठा irel_ma 	*ma;

	ma = irel->priv_तथ्य;
	अगर (ino > ma->max_inode)
		वापस EXT2_ET_INVALID_ARGUMENT;
	अगर (ma->entries[(अचिन्हित) ino].new == 0)
		वापस ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	वापस 0;
}

अटल errcode_t ima_अग्र_ref(ext2_irel irel,
			      काष्ठा ext2_inode_reference *ref)
{
	काष्ठा irel_ma 	*ma;
	काष्ठा inode_reference_entry *ref_ent;

	ma = irel->priv_तथ्य;

	ref_ent = ma->ref_entries + ma->ref_current;

	अगर ((ref_ent->refs == शून्य) ||
	    (ma->ref_iter >= ref_ent->अंक)) {
		ref->block = 0;
		ref->offset = 0;
		वापस 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	वापस 0;
}


अटल errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	काष्ठा irel_ma 	*ma;

	ma = irel->priv_तथ्य;
	अगर ((old > ma->max_inode) || (new > ma->max_inode))
		वापस EXT2_ET_INVALID_ARGUMENT;
	अगर (ma->entries[(अचिन्हित) old].new == 0)
		वापस ENOENT;

	ma->entries[(अचिन्हित) new] = ma->entries[(अचिन्हित) old];
	अगर (ma->ref_entries[(अचिन्हित) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(अचिन्हित) new].refs);
	ma->ref_entries[(अचिन्हित) new] = ma->ref_entries[(अचिन्हित) old];

	ma->entries[(अचिन्हित) old].new = 0;
	ma->ref_entries[(अचिन्हित) old].अंक = 0;
	ma->ref_entries[(अचिन्हित) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	वापस 0;
}

अटल errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	काष्ठा irel_ma 	*ma;

	ma = irel->priv_तथ्य;
	अगर (old > ma->max_inode)
		वापस EXT2_ET_INVALID_ARGUMENT;
	अगर (ma->entries[(अचिन्हित) old].new == 0)
		वापस ENOENT;

	ma->entries[old].new = 0;
	अगर (ma->ref_entries[(अचिन्हित) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(अचिन्हित) old].refs);
	ma->orig_map[ma->entries[(अचिन्हित) old].orig] = 0;

	ma->ref_entries[(अचिन्हित) old].अंक = 0;
	ma->ref_entries[(अचिन्हित) old].refs = 0;
	वापस 0;
}

अटल errcode_t ima_free(ext2_irel irel)
{
	काष्ठा irel_ma 	*ma;
	ext2_ino_t	ino;

	अगर (!irel)
		वापस 0;

	ma = irel->priv_तथ्य;

	अगर (ma) {
		अगर (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		अगर (ma->entries)
			ext2fs_free_mem(&ma->entries);
		अगर (ma->ref_entries) {
			क्रम (ino = 0; ino <= ma->max_inode; ino++) {
				अगर (ma->ref_entries[(अचिन्हित) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(अचिन्हित) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	अगर (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	वापस 0;
}
