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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
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
	__u16 अंक;
	संरचना ext2_inode_reference *refs;
};

संरचना irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	पूर्णांक   ref_iter;
	ext2_ino_t	*orig_map;
	संरचना ext2_inode_relocate_entry *entries;
	संरचना inode_reference_entry *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(अक्षर *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	संरचना irel_ma 	*ma = 0;
	माप_प्रकार		size;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(आकार(संरचना ext2_inode_relocation_table),
				&irel);
	जर (retval)
		जा errout;
	स_रखो(irel, 0, आकार(संरचना ext2_inode_relocation_table));

	retval = ext2fs_get_mem(म_माप(name)+1, &irel->name);
	जर (retval)
		जा errout;
	म_नकल(irel->name, name);

	retval = ext2fs_get_mem(आकार(संरचना irel_ma), &ma);
	जर (retval)
		जा errout;
	स_रखो(ma, 0, आकार(संरचना irel_ma));
	irel->priv_data = ma;

	size = (माप_प्रकार) (आकार(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, आकार(ext2_ino_t),
		&ma->orig_map);
	जर (retval)
		जा errout;
	स_रखो(ma->orig_map, 0, size);

	size = (माप_प्रकार) (आकार(संरचना ext2_inode_relocate_entry) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		आकार(संरचना ext2_inode_relocate_entry), &ma->entries);
	जर (retval)
		जा errout;
	स_रखो(ma->entries, 0, size);

	size = (माप_प्रकार) (आकार(संरचना inode_reference_entry) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		आकार(संरचना inode_reference_entry), &ma->ref_entries);
	जर (retval)
		जा errout;
	स_रखो(ma->ref_entries, 0, size);
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
	परत 0;

errout:
	ima_free(irel);
	परत retval;
}

स्थिर errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			संरचना ext2_inode_relocate_entry *ent)
{
	संरचना inode_reference_entry	*ref_ent;
	संरचना irel_ma 			*ma;
	errcode_t			retval;
	माप_प्रकार				size, old_size;

	ma = irel->priv_data;
	जर (old > ma->max_inode)
		परत EXT2_ET_INVALID_ARGUMENT;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	जर (ma->entries[(अचिन्हित) old].new == 0)
		ent->orig = old;
	अन्यथा
		ent->orig = ma->entries[(अचिन्हित) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (अचिन्हित) old;
	जर (ref_ent->refs && ent->max_refs !=
	    ma->entries[(अचिन्हित) old].max_refs) {
		size = (आकार(संरचना ext2_inode_reference) * ent->max_refs);
		old_size = (आकार(संरचना ext2_inode_reference) *
			    ma->entries[(अचिन्हित) old].max_refs);
		retval = ext2fs_resize_mem(old_size, size, &ref_ent->refs);
		जर (retval)
			परत retval;
	}

	ma->entries[(अचिन्हित) old] = *ent;
	ma->orig_map[(अचिन्हित) ent->orig] = old;
	परत 0;
}

स्थिर errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			संरचना ext2_inode_relocate_entry *ent)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_data;
	जर (old > ma->max_inode)
		परत EXT2_ET_INVALID_ARGUMENT;
	जर (ma->entries[(अचिन्हित) old].new == 0)
		परत ENOENT;
	*ent = ma->entries[(अचिन्हित) old];
	परत 0;
}

स्थिर errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			संरचना ext2_inode_relocate_entry *ent)
{
	संरचना irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_data;
	जर (orig > ma->max_inode)
		परत EXT2_ET_INVALID_ARGUMENT;
	ino = ma->orig_map[(अचिन्हित) orig];
	जर (ino == 0)
		परत ENOENT;
	*old = ino;
	*ent = ma->entries[(अचिन्हित) ino];
	परत 0;
}

स्थिर errcode_t ima_start_iter(ext2_irel irel)
{
	irel->current = 0;
	परत 0;
}

स्थिर errcode_t ima_next(ext2_irel irel, ext2_ino_t *old,
			 संरचना ext2_inode_relocate_entry *ent)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_data;
	पर्यंत (++irel->current < ma->max_inode) {
		जर (ma->entries[(अचिन्हित) irel->current].new == 0)
			सुरूठेवा;
		*old = irel->current;
		*ent = ma->entries[(अचिन्हित) irel->current];
		परत 0;
	}
	*old = 0;
	परत 0;
}

स्थिर errcode_t ima_add_ref(ext2_irel irel, ext2_ino_t ino,
			     संरचना ext2_inode_reference *ref)
{
	संरचना irel_ma 	*ma;
	माप_प्रकार		size;
	संरचना inode_reference_entry *ref_ent;
	संरचना ext2_inode_relocate_entry *ent;
	errcode_t		retval;

	ma = irel->priv_data;
	जर (ino > ma->max_inode)
		परत EXT2_ET_INVALID_ARGUMENT;

	ref_ent = ma->ref_entries + (अचिन्हित) ino;
	ent = ma->entries + (अचिन्हित) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	जर (ref_ent->refs == 0) {
		size = (माप_प्रकार) ((आकार(संरचना ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			आकार(संरचना ext2_inode_reference), &ref_ent->refs);
		जर (retval)
			परत retval;
		स_रखो(ref_ent->refs, 0, size);
		ref_ent->अंक = 0;
	}

	जर (ref_ent->अंक >= ent->max_refs)
		परत EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(अचिन्हित) ref_ent->अंक++] = *ref;
	परत 0;
}

स्थिर errcode_t ima_start_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_data;
	जर (ino > ma->max_inode)
		परत EXT2_ET_INVALID_ARGUMENT;
	जर (ma->entries[(अचिन्हित) ino].new == 0)
		परत ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	परत 0;
}

स्थिर errcode_t ima_next_ref(ext2_irel irel,
			      संरचना ext2_inode_reference *ref)
{
	संरचना irel_ma 	*ma;
	संरचना inode_reference_entry *ref_ent;

	ma = irel->priv_data;

	ref_ent = ma->ref_entries + ma->ref_current;

	जर ((ref_ent->refs == रिक्त) ||
	    (ma->ref_iter >= ref_ent->अंक)) {
		ref->block = 0;
		ref->offset = 0;
		परत 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	परत 0;
}


स्थिर errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_data;
	जर ((old > ma->max_inode) || (new > ma->max_inode))
		परत EXT2_ET_INVALID_ARGUMENT;
	जर (ma->entries[(अचिन्हित) old].new == 0)
		परत ENOENT;

	ma->entries[(अचिन्हित) new] = ma->entries[(अचिन्हित) old];
	जर (ma->ref_entries[(अचिन्हित) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(अचिन्हित) new].refs);
	ma->ref_entries[(अचिन्हित) new] = ma->ref_entries[(अचिन्हित) old];

	ma->entries[(अचिन्हित) old].new = 0;
	ma->ref_entries[(अचिन्हित) old].अंक = 0;
	ma->ref_entries[(अचिन्हित) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	परत 0;
}

स्थिर errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	संरचना irel_ma 	*ma;

	ma = irel->priv_data;
	जर (old > ma->max_inode)
		परत EXT2_ET_INVALID_ARGUMENT;
	जर (ma->entries[(अचिन्हित) old].new == 0)
		परत ENOENT;

	ma->entries[old].new = 0;
	जर (ma->ref_entries[(अचिन्हित) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(अचिन्हित) old].refs);
	ma->orig_map[ma->entries[(अचिन्हित) old].orig] = 0;

	ma->ref_entries[(अचिन्हित) old].अंक = 0;
	ma->ref_entries[(अचिन्हित) old].refs = 0;
	परत 0;
}

स्थिर errcode_t ima_free(ext2_irel irel)
{
	संरचना irel_ma 	*ma;
	ext2_ino_t	ino;

	जर (!irel)
		परत 0;

	ma = irel->priv_data;

	जर (ma) {
		जर (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		जर (ma->entries)
			ext2fs_free_mem(&ma->entries);
		जर (ma->ref_entries) {
			साठी (ino = 0; ino <= ma->max_inode; ino++) {
				जर (ma->ref_entries[(अचिन्हित) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(अचिन्हित) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	जर (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	परत 0;
}
