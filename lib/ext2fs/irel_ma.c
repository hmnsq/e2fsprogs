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

#అన్తర్భావ "config.h"
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "irel.h"

స్థిర errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 నిర్మాణం ext2_inode_relocate_నమోదు *ent);
స్థిర errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 నిర్మాణం ext2_inode_relocate_నమోదు *ent);
స్థిర errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 నిర్మాణం ext2_inode_relocate_నమోదు *ent);
స్థిర errcode_t ima_ప్రారంభం_iter(ext2_irel irel);
స్థిర errcode_t ima_తదుపరి(ext2_irel irel, ext2_ino_t *old,
			  నిర్మాణం ext2_inode_relocate_నమోదు *ent);
స్థిర errcode_t ima_కూడు_ref(ext2_irel irel, ext2_ino_t ino,
			     నిర్మాణం ext2_inode_reference *ref);
స్థిర errcode_t ima_ప్రారంభం_iter_ref(ext2_irel irel, ext2_ino_t ino);
స్థిర errcode_t ima_తదుపరి_ref(ext2_irel irel, నిర్మాణం ext2_inode_reference *ref);
స్థిర errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
స్థిర errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
స్థిర errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
నిర్మాణం inode_reference_నమోదు {
	__u16 అంకె;
	నిర్మాణం ext2_inode_reference *refs;
};

నిర్మాణం irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	పూర్ణాంకం   ref_iter;
	ext2_ino_t	*orig_map;
	నిర్మాణం ext2_inode_relocate_నమోదు *entries;
	నిర్మాణం inode_reference_నమోదు *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(అక్షరం *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	నిర్మాణం irel_ma 	*ma = 0;
	size_t		కొలత;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_inode_relocation_పట్టిక),
				&irel);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	memset(irel, 0, పరిమాణం(నిర్మాణం ext2_inode_relocation_పట్టిక));

	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(name)+1, &irel->name);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం irel_ma), &ma);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	memset(ma, 0, పరిమాణం(నిర్మాణం irel_ma));
	irel->priv_దత్తాంశం = ma;

	కొలత = (size_t) (పరిమాణం(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, పరిమాణం(ext2_ino_t),
		&ma->orig_map);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	memset(ma->orig_map, 0, కొలత);

	కొలత = (size_t) (పరిమాణం(నిర్మాణం ext2_inode_relocate_నమోదు) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		పరిమాణం(నిర్మాణం ext2_inode_relocate_నమోదు), &ma->entries);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	memset(ma->entries, 0, కొలత);

	కొలత = (size_t) (పరిమాణం(నిర్మాణం inode_reference_నమోదు) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		పరిమాణం(నిర్మాణం inode_reference_నమోదు), &ma->ref_entries);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	memset(ma->ref_entries, 0, కొలత);
	ma->max_inode = max_inode;

	/*
	 * Fill in the irel data structure
	 */
	irel->put = ima_put;
	irel->get = ima_get;
	irel->get_by_orig = ima_get_by_orig;
	irel->ప్రారంభం_iter = ima_ప్రారంభం_iter;
	irel->తదుపరి = ima_తదుపరి;
	irel->కూడు_ref = ima_కూడు_ref;
	irel->ప్రారంభం_iter_ref = ima_ప్రారంభం_iter_ref;
	irel->తదుపరి_ref = ima_తదుపరి_ref;
	irel->move = ima_move;
	irel->delete = ima_delete;
	irel->విడుదల = ima_free;

	*new_irel = irel;
	తిరిగి 0;

errout:
	ima_free(irel);
	తిరిగి retval;
}

స్థిర errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			నిర్మాణం ext2_inode_relocate_నమోదు *ent)
{
	నిర్మాణం inode_reference_నమోదు	*ref_ent;
	నిర్మాణం irel_ma 			*ma;
	errcode_t			retval;
	size_t				కొలత, old_కొలత;

	ma = irel->priv_దత్తాంశం;
	ఒకవేళ (old > ma->max_inode)
		తిరిగి EXT2_ET_INVALID_వాదం;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	ఒకవేళ (ma->entries[(గుర్తులేని) old].new == 0)
		ent->orig = old;
	లేకపోతే
		ent->orig = ma->entries[(గుర్తులేని) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (గుర్తులేని) old;
	ఒకవేళ (ref_ent->refs && ent->max_refs !=
	    ma->entries[(గుర్తులేని) old].max_refs) {
		కొలత = (పరిమాణం(నిర్మాణం ext2_inode_reference) * ent->max_refs);
		old_కొలత = (పరిమాణం(నిర్మాణం ext2_inode_reference) *
			    ma->entries[(గుర్తులేని) old].max_refs);
		retval = ext2fs_resize_mem(old_కొలత, కొలత, &ref_ent->refs);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	ma->entries[(గుర్తులేని) old] = *ent;
	ma->orig_map[(గుర్తులేని) ent->orig] = old;
	తిరిగి 0;
}

స్థిర errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			నిర్మాణం ext2_inode_relocate_నమోదు *ent)
{
	నిర్మాణం irel_ma 	*ma;

	ma = irel->priv_దత్తాంశం;
	ఒకవేళ (old > ma->max_inode)
		తిరిగి EXT2_ET_INVALID_వాదం;
	ఒకవేళ (ma->entries[(గుర్తులేని) old].new == 0)
		తిరిగి ENOENT;
	*ent = ma->entries[(గుర్తులేని) old];
	తిరిగి 0;
}

స్థిర errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			నిర్మాణం ext2_inode_relocate_నమోదు *ent)
{
	నిర్మాణం irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_దత్తాంశం;
	ఒకవేళ (orig > ma->max_inode)
		తిరిగి EXT2_ET_INVALID_వాదం;
	ino = ma->orig_map[(గుర్తులేని) orig];
	ఒకవేళ (ino == 0)
		తిరిగి ENOENT;
	*old = ino;
	*ent = ma->entries[(గుర్తులేని) ino];
	తిరిగి 0;
}

స్థిర errcode_t ima_ప్రారంభం_iter(ext2_irel irel)
{
	irel->current = 0;
	తిరిగి 0;
}

స్థిర errcode_t ima_తదుపరి(ext2_irel irel, ext2_ino_t *old,
			 నిర్మాణం ext2_inode_relocate_నమోదు *ent)
{
	నిర్మాణం irel_ma 	*ma;

	ma = irel->priv_దత్తాంశం;
	అయితే (++irel->current < ma->max_inode) {
		ఒకవేళ (ma->entries[(గుర్తులేని) irel->current].new == 0)
			కొనసాగు;
		*old = irel->current;
		*ent = ma->entries[(గుర్తులేని) irel->current];
		తిరిగి 0;
	}
	*old = 0;
	తిరిగి 0;
}

స్థిర errcode_t ima_కూడు_ref(ext2_irel irel, ext2_ino_t ino,
			     నిర్మాణం ext2_inode_reference *ref)
{
	నిర్మాణం irel_ma 	*ma;
	size_t		కొలత;
	నిర్మాణం inode_reference_నమోదు *ref_ent;
	నిర్మాణం ext2_inode_relocate_నమోదు *ent;
	errcode_t		retval;

	ma = irel->priv_దత్తాంశం;
	ఒకవేళ (ino > ma->max_inode)
		తిరిగి EXT2_ET_INVALID_వాదం;

	ref_ent = ma->ref_entries + (గుర్తులేని) ino;
	ent = ma->entries + (గుర్తులేని) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	ఒకవేళ (ref_ent->refs == 0) {
		కొలత = (size_t) ((పరిమాణం(నిర్మాణం ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			పరిమాణం(నిర్మాణం ext2_inode_reference), &ref_ent->refs);
		ఒకవేళ (retval)
			తిరిగి retval;
		memset(ref_ent->refs, 0, కొలత);
		ref_ent->అంకె = 0;
	}

	ఒకవేళ (ref_ent->అంకె >= ent->max_refs)
		తిరిగి EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(గుర్తులేని) ref_ent->అంకె++] = *ref;
	తిరిగి 0;
}

స్థిర errcode_t ima_ప్రారంభం_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	నిర్మాణం irel_ma 	*ma;

	ma = irel->priv_దత్తాంశం;
	ఒకవేళ (ino > ma->max_inode)
		తిరిగి EXT2_ET_INVALID_వాదం;
	ఒకవేళ (ma->entries[(గుర్తులేని) ino].new == 0)
		తిరిగి ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	తిరిగి 0;
}

స్థిర errcode_t ima_తదుపరి_ref(ext2_irel irel,
			      నిర్మాణం ext2_inode_reference *ref)
{
	నిర్మాణం irel_ma 	*ma;
	నిర్మాణం inode_reference_నమోదు *ref_ent;

	ma = irel->priv_దత్తాంశం;

	ref_ent = ma->ref_entries + ma->ref_current;

	ఒకవేళ ((ref_ent->refs == ఖాళీ) ||
	    (ma->ref_iter >= ref_ent->అంకె)) {
		ref->ఖండం = 0;
		ref->స్థానభ్రంశం = 0;
		తిరిగి 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	తిరిగి 0;
}


స్థిర errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	నిర్మాణం irel_ma 	*ma;

	ma = irel->priv_దత్తాంశం;
	ఒకవేళ ((old > ma->max_inode) || (new > ma->max_inode))
		తిరిగి EXT2_ET_INVALID_వాదం;
	ఒకవేళ (ma->entries[(గుర్తులేని) old].new == 0)
		తిరిగి ENOENT;

	ma->entries[(గుర్తులేని) new] = ma->entries[(గుర్తులేని) old];
	ఒకవేళ (ma->ref_entries[(గుర్తులేని) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(గుర్తులేని) new].refs);
	ma->ref_entries[(గుర్తులేని) new] = ma->ref_entries[(గుర్తులేని) old];

	ma->entries[(గుర్తులేని) old].new = 0;
	ma->ref_entries[(గుర్తులేని) old].అంకె = 0;
	ma->ref_entries[(గుర్తులేని) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	తిరిగి 0;
}

స్థిర errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	నిర్మాణం irel_ma 	*ma;

	ma = irel->priv_దత్తాంశం;
	ఒకవేళ (old > ma->max_inode)
		తిరిగి EXT2_ET_INVALID_వాదం;
	ఒకవేళ (ma->entries[(గుర్తులేని) old].new == 0)
		తిరిగి ENOENT;

	ma->entries[old].new = 0;
	ఒకవేళ (ma->ref_entries[(గుర్తులేని) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(గుర్తులేని) old].refs);
	ma->orig_map[ma->entries[(గుర్తులేని) old].orig] = 0;

	ma->ref_entries[(గుర్తులేని) old].అంకె = 0;
	ma->ref_entries[(గుర్తులేని) old].refs = 0;
	తిరిగి 0;
}

స్థిర errcode_t ima_free(ext2_irel irel)
{
	నిర్మాణం irel_ma 	*ma;
	ext2_ino_t	ino;

	ఒకవేళ (!irel)
		తిరిగి 0;

	ma = irel->priv_దత్తాంశం;

	ఒకవేళ (ma) {
		ఒకవేళ (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		ఒకవేళ (ma->entries)
			ext2fs_free_mem(&ma->entries);
		ఒకవేళ (ma->ref_entries) {
			కొరకు (ino = 0; ino <= ma->max_inode; ino++) {
				ఒకవేళ (ma->ref_entries[(గుర్తులేని) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(గుర్తులేని) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	ఒకవేళ (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	తిరిగి 0;
}
