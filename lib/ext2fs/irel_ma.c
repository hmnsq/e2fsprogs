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
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "irel.h"

ساكن errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 بنية ext2_inode_relocate_مدخل *ent);
ساكن errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 بنية ext2_inode_relocate_مدخل *ent);
ساكن errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 بنية ext2_inode_relocate_مدخل *ent);
ساكن errcode_t ima_شروع_iter(ext2_irel irel);
ساكن errcode_t ima_بعدی(ext2_irel irel, ext2_ino_t *old,
			  بنية ext2_inode_relocate_مدخل *ent);
ساكن errcode_t ima_جمع_کن_ref(ext2_irel irel, ext2_ino_t ino,
			     بنية ext2_inode_reference *ref);
ساكن errcode_t ima_شروع_iter_ref(ext2_irel irel, ext2_ino_t ino);
ساكن errcode_t ima_بعدی_ref(ext2_irel irel, بنية ext2_inode_reference *ref);
ساكن errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
ساكن errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
ساكن errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
بنية inode_reference_مدخل {
	__u16 رقم;
	بنية ext2_inode_reference *refs;
};

بنية irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	صحيح   ref_iter;
	ext2_ino_t	*orig_map;
	بنية ext2_inode_relocate_مدخل *entries;
	بنية inode_reference_مدخل *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(حرف *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	بنية irel_ma 	*ma = 0;
	size_t		اندازه;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(حجم(بنية ext2_inode_relocation_جدول),
				&irel);
	إذا (retval)
		اذهب errout;
	memset(irel, 0, حجم(بنية ext2_inode_relocation_جدول));

	retval = ext2fs_get_mem(طول_رشته(name)+1, &irel->name);
	إذا (retval)
		اذهب errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(حجم(بنية irel_ma), &ma);
	إذا (retval)
		اذهب errout;
	memset(ma, 0, حجم(بنية irel_ma));
	irel->priv_داده = ma;

	اندازه = (size_t) (حجم(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, حجم(ext2_ino_t),
		&ma->orig_map);
	إذا (retval)
		اذهب errout;
	memset(ma->orig_map, 0, اندازه);

	اندازه = (size_t) (حجم(بنية ext2_inode_relocate_مدخل) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		حجم(بنية ext2_inode_relocate_مدخل), &ma->entries);
	إذا (retval)
		اذهب errout;
	memset(ma->entries, 0, اندازه);

	اندازه = (size_t) (حجم(بنية inode_reference_مدخل) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		حجم(بنية inode_reference_مدخل), &ma->ref_entries);
	إذا (retval)
		اذهب errout;
	memset(ma->ref_entries, 0, اندازه);
	ma->max_inode = max_inode;

	/*
	 * Fill in the irel data structure
	 */
	irel->put = ima_put;
	irel->get = ima_get;
	irel->get_by_orig = ima_get_by_orig;
	irel->شروع_iter = ima_شروع_iter;
	irel->بعدی = ima_بعدی;
	irel->جمع_کن_ref = ima_جمع_کن_ref;
	irel->شروع_iter_ref = ima_شروع_iter_ref;
	irel->بعدی_ref = ima_بعدی_ref;
	irel->move = ima_move;
	irel->delete = ima_delete;
	irel->حرر = ima_free;

	*new_irel = irel;
	أرجع 0;

errout:
	ima_free(irel);
	أرجع retval;
}

ساكن errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			بنية ext2_inode_relocate_مدخل *ent)
{
	بنية inode_reference_مدخل	*ref_ent;
	بنية irel_ma 			*ma;
	errcode_t			retval;
	size_t				اندازه, old_اندازه;

	ma = irel->priv_داده;
	إذا (old > ma->max_inode)
		أرجع EXT2_ET_INVALID_آرگومان;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	إذا (ma->entries[(بیعلامت) old].new == 0)
		ent->orig = old;
	وإلا
		ent->orig = ma->entries[(بیعلامت) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (بیعلامت) old;
	إذا (ref_ent->refs && ent->max_refs !=
	    ma->entries[(بیعلامت) old].max_refs) {
		اندازه = (حجم(بنية ext2_inode_reference) * ent->max_refs);
		old_اندازه = (حجم(بنية ext2_inode_reference) *
			    ma->entries[(بیعلامت) old].max_refs);
		retval = ext2fs_resize_mem(old_اندازه, اندازه, &ref_ent->refs);
		إذا (retval)
			أرجع retval;
	}

	ma->entries[(بیعلامت) old] = *ent;
	ma->orig_map[(بیعلامت) ent->orig] = old;
	أرجع 0;
}

ساكن errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			بنية ext2_inode_relocate_مدخل *ent)
{
	بنية irel_ma 	*ma;

	ma = irel->priv_داده;
	إذا (old > ma->max_inode)
		أرجع EXT2_ET_INVALID_آرگومان;
	إذا (ma->entries[(بیعلامت) old].new == 0)
		أرجع ENOENT;
	*ent = ma->entries[(بیعلامت) old];
	أرجع 0;
}

ساكن errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			بنية ext2_inode_relocate_مدخل *ent)
{
	بنية irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_داده;
	إذا (orig > ma->max_inode)
		أرجع EXT2_ET_INVALID_آرگومان;
	ino = ma->orig_map[(بیعلامت) orig];
	إذا (ino == 0)
		أرجع ENOENT;
	*old = ino;
	*ent = ma->entries[(بیعلامت) ino];
	أرجع 0;
}

ساكن errcode_t ima_شروع_iter(ext2_irel irel)
{
	irel->current = 0;
	أرجع 0;
}

ساكن errcode_t ima_بعدی(ext2_irel irel, ext2_ino_t *old,
			 بنية ext2_inode_relocate_مدخل *ent)
{
	بنية irel_ma 	*ma;

	ma = irel->priv_داده;
	طالما (++irel->current < ma->max_inode) {
		إذا (ma->entries[(بیعلامت) irel->current].new == 0)
			استمر;
		*old = irel->current;
		*ent = ma->entries[(بیعلامت) irel->current];
		أرجع 0;
	}
	*old = 0;
	أرجع 0;
}

ساكن errcode_t ima_جمع_کن_ref(ext2_irel irel, ext2_ino_t ino,
			     بنية ext2_inode_reference *ref)
{
	بنية irel_ma 	*ma;
	size_t		اندازه;
	بنية inode_reference_مدخل *ref_ent;
	بنية ext2_inode_relocate_مدخل *ent;
	errcode_t		retval;

	ma = irel->priv_داده;
	إذا (ino > ma->max_inode)
		أرجع EXT2_ET_INVALID_آرگومان;

	ref_ent = ma->ref_entries + (بیعلامت) ino;
	ent = ma->entries + (بیعلامت) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	إذا (ref_ent->refs == 0) {
		اندازه = (size_t) ((حجم(بنية ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			حجم(بنية ext2_inode_reference), &ref_ent->refs);
		إذا (retval)
			أرجع retval;
		memset(ref_ent->refs, 0, اندازه);
		ref_ent->رقم = 0;
	}

	إذا (ref_ent->رقم >= ent->max_refs)
		أرجع EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(بیعلامت) ref_ent->رقم++] = *ref;
	أرجع 0;
}

ساكن errcode_t ima_شروع_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	بنية irel_ma 	*ma;

	ma = irel->priv_داده;
	إذا (ino > ma->max_inode)
		أرجع EXT2_ET_INVALID_آرگومان;
	إذا (ma->entries[(بیعلامت) ino].new == 0)
		أرجع ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	أرجع 0;
}

ساكن errcode_t ima_بعدی_ref(ext2_irel irel,
			      بنية ext2_inode_reference *ref)
{
	بنية irel_ma 	*ma;
	بنية inode_reference_مدخل *ref_ent;

	ma = irel->priv_داده;

	ref_ent = ma->ref_entries + ma->ref_current;

	إذا ((ref_ent->refs == عدم) ||
	    (ma->ref_iter >= ref_ent->رقم)) {
		ref->بلوک = 0;
		ref->جابه_جایی = 0;
		أرجع 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	أرجع 0;
}


ساكن errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	بنية irel_ma 	*ma;

	ma = irel->priv_داده;
	إذا ((old > ma->max_inode) || (new > ma->max_inode))
		أرجع EXT2_ET_INVALID_آرگومان;
	إذا (ma->entries[(بیعلامت) old].new == 0)
		أرجع ENOENT;

	ma->entries[(بیعلامت) new] = ma->entries[(بیعلامت) old];
	إذا (ma->ref_entries[(بیعلامت) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(بیعلامت) new].refs);
	ma->ref_entries[(بیعلامت) new] = ma->ref_entries[(بیعلامت) old];

	ma->entries[(بیعلامت) old].new = 0;
	ma->ref_entries[(بیعلامت) old].رقم = 0;
	ma->ref_entries[(بیعلامت) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	أرجع 0;
}

ساكن errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	بنية irel_ma 	*ma;

	ma = irel->priv_داده;
	إذا (old > ma->max_inode)
		أرجع EXT2_ET_INVALID_آرگومان;
	إذا (ma->entries[(بیعلامت) old].new == 0)
		أرجع ENOENT;

	ma->entries[old].new = 0;
	إذا (ma->ref_entries[(بیعلامت) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(بیعلامت) old].refs);
	ma->orig_map[ma->entries[(بیعلامت) old].orig] = 0;

	ma->ref_entries[(بیعلامت) old].رقم = 0;
	ma->ref_entries[(بیعلامت) old].refs = 0;
	أرجع 0;
}

ساكن errcode_t ima_free(ext2_irel irel)
{
	بنية irel_ma 	*ma;
	ext2_ino_t	ino;

	إذا (!irel)
		أرجع 0;

	ma = irel->priv_داده;

	إذا (ma) {
		إذا (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		إذا (ma->entries)
			ext2fs_free_mem(&ma->entries);
		إذا (ma->ref_entries) {
			لكل (ino = 0; ino <= ma->max_inode; ino++) {
				إذا (ma->ref_entries[(بیعلامت) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(بیعلامت) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	إذا (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	أرجع 0;
}
