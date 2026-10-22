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
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "irel.h"

ساکن errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			 ساخت ext2_inode_relocate_اندراج *ent);
ساکن errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			 ساخت ext2_inode_relocate_اندراج *ent);
ساکن errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 ساخت ext2_inode_relocate_اندراج *ent);
ساکن errcode_t ima_آغاز_iter(ext2_irel irel);
ساکن errcode_t ima_اگلا(ext2_irel irel, ext2_ino_t *old,
			  ساخت ext2_inode_relocate_اندراج *ent);
ساکن errcode_t ima_جوڑو_ref(ext2_irel irel, ext2_ino_t ino,
			     ساخت ext2_inode_reference *ref);
ساکن errcode_t ima_آغاز_iter_ref(ext2_irel irel, ext2_ino_t ino);
ساکن errcode_t ima_اگلا_ref(ext2_irel irel, ساخت ext2_inode_reference *ref);
ساکن errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);
ساکن errcode_t ima_delete(ext2_irel irel, ext2_ino_t old);
ساکن errcode_t ima_free(ext2_irel irel);

/*
 * This data structure stores the array of inode references; there is
 * a structure for each inode.
 */
ساخت inode_reference_اندراج {
	__u16 عدد;
	ساخت ext2_inode_reference *refs;
};

ساخت irel_ma {
	__u32 magic;
	ext2_ino_t max_inode;
	ext2_ino_t ref_current;
	صحیح   ref_iter;
	ext2_ino_t	*orig_map;
	ساخت ext2_inode_relocate_اندراج *entries;
	ساخت inode_reference_اندراج *ref_entries;
};

errcode_t ext2fs_irel_memarray_create(حرف *name, ext2_ino_t max_inode,
				      ext2_irel *new_irel)
{
	ext2_irel		irel = 0;
	errcode_t	retval;
	ساخت irel_ma 	*ma = 0;
	size_t		سائز;

	*new_irel = 0;

	/*
	 * Allocate memory structures
	 */
	retval = ext2fs_get_mem(حجم(ساخت ext2_inode_relocation_جدول),
				&irel);
	اگر (retval)
		جاؤ errout;
	memset(irel, 0, حجم(ساخت ext2_inode_relocation_جدول));

	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &irel->name);
	اگر (retval)
		جاؤ errout;
	strcpy(irel->name, name);

	retval = ext2fs_get_mem(حجم(ساخت irel_ma), &ma);
	اگر (retval)
		جاؤ errout;
	memset(ma, 0, حجم(ساخت irel_ma));
	irel->priv_ڈیٹا = ma;

	سائز = (size_t) (حجم(ext2_ino_t) * (max_inode+1));
	retval = ext2fs_get_array(max_inode+1, حجم(ext2_ino_t),
		&ma->orig_map);
	اگر (retval)
		جاؤ errout;
	memset(ma->orig_map, 0, سائز);

	سائز = (size_t) (حجم(ساخت ext2_inode_relocate_اندراج) *
			 (max_inode+1));
	retval = ext2fs_get_array((max_inode+1,
		حجم(ساخت ext2_inode_relocate_اندراج), &ma->entries);
	اگر (retval)
		جاؤ errout;
	memset(ma->entries, 0, سائز);

	سائز = (size_t) (حجم(ساخت inode_reference_اندراج) *
			 (max_inode+1));
	retval = ext2fs_get_mem(max_inode+1,
		حجم(ساخت inode_reference_اندراج), &ma->ref_entries);
	اگر (retval)
		جاؤ errout;
	memset(ma->ref_entries, 0, سائز);
	ma->max_inode = max_inode;

	/*
	 * Fill in the irel data structure
	 */
	irel->put = ima_put;
	irel->get = ima_get;
	irel->get_by_orig = ima_get_by_orig;
	irel->آغاز_iter = ima_آغاز_iter;
	irel->اگلا = ima_اگلا;
	irel->جوڑو_ref = ima_جوڑو_ref;
	irel->آغاز_iter_ref = ima_آغاز_iter_ref;
	irel->اگلا_ref = ima_اگلا_ref;
	irel->move = ima_move;
	irel->delete = ima_delete;
	irel->آزاد = ima_free;

	*new_irel = irel;
	واپس 0;

errout:
	ima_free(irel);
	واپس retval;
}

ساکن errcode_t ima_put(ext2_irel irel, ext2_ino_t old,
			ساخت ext2_inode_relocate_اندراج *ent)
{
	ساخت inode_reference_اندراج	*ref_ent;
	ساخت irel_ma 			*ma;
	errcode_t			retval;
	size_t				سائز, old_سائز;

	ma = irel->priv_ڈیٹا;
	اگر (old > ma->max_inode)
		واپس EXT2_ET_INVALID_دلیل;

	/*
	 * Force the orig field to the correct value; the application
	 * program shouldn't be messing with this field.
	 */
	اگر (ma->entries[(غیرعلامتی) old].new == 0)
		ent->orig = old;
	ورنہ
		ent->orig = ma->entries[(غیرعلامتی) old].orig;

	/*
	 * If max_refs has changed, reallocate the refs array
	 */
	ref_ent = ma->ref_entries + (غیرعلامتی) old;
	اگر (ref_ent->refs && ent->max_refs !=
	    ma->entries[(غیرعلامتی) old].max_refs) {
		سائز = (حجم(ساخت ext2_inode_reference) * ent->max_refs);
		old_سائز = (حجم(ساخت ext2_inode_reference) *
			    ma->entries[(غیرعلامتی) old].max_refs);
		retval = ext2fs_resize_mem(old_سائز, سائز, &ref_ent->refs);
		اگر (retval)
			واپس retval;
	}

	ma->entries[(غیرعلامتی) old] = *ent;
	ma->orig_map[(غیرعلامتی) ent->orig] = old;
	واپس 0;
}

ساکن errcode_t ima_get(ext2_irel irel, ext2_ino_t old,
			ساخت ext2_inode_relocate_اندراج *ent)
{
	ساخت irel_ma 	*ma;

	ma = irel->priv_ڈیٹا;
	اگر (old > ma->max_inode)
		واپس EXT2_ET_INVALID_دلیل;
	اگر (ma->entries[(غیرعلامتی) old].new == 0)
		واپس ENOENT;
	*ent = ma->entries[(غیرعلامتی) old];
	واپس 0;
}

ساکن errcode_t ima_get_by_orig(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
			ساخت ext2_inode_relocate_اندراج *ent)
{
	ساخت irel_ma 	*ma;
	ext2_ino_t	ino;

	ma = irel->priv_ڈیٹا;
	اگر (orig > ma->max_inode)
		واپس EXT2_ET_INVALID_دلیل;
	ino = ma->orig_map[(غیرعلامتی) orig];
	اگر (ino == 0)
		واپس ENOENT;
	*old = ino;
	*ent = ma->entries[(غیرعلامتی) ino];
	واپس 0;
}

ساکن errcode_t ima_آغاز_iter(ext2_irel irel)
{
	irel->current = 0;
	واپس 0;
}

ساکن errcode_t ima_اگلا(ext2_irel irel, ext2_ino_t *old,
			 ساخت ext2_inode_relocate_اندراج *ent)
{
	ساخت irel_ma 	*ma;

	ma = irel->priv_ڈیٹا;
	جب (++irel->current < ma->max_inode) {
		اگر (ma->entries[(غیرعلامتی) irel->current].new == 0)
			جاری;
		*old = irel->current;
		*ent = ma->entries[(غیرعلامتی) irel->current];
		واپس 0;
	}
	*old = 0;
	واپس 0;
}

ساکن errcode_t ima_جوڑو_ref(ext2_irel irel, ext2_ino_t ino,
			     ساخت ext2_inode_reference *ref)
{
	ساخت irel_ma 	*ma;
	size_t		سائز;
	ساخت inode_reference_اندراج *ref_ent;
	ساخت ext2_inode_relocate_اندراج *ent;
	errcode_t		retval;

	ma = irel->priv_ڈیٹا;
	اگر (ino > ma->max_inode)
		واپس EXT2_ET_INVALID_دلیل;

	ref_ent = ma->ref_entries + (غیرعلامتی) ino;
	ent = ma->entries + (غیرعلامتی) ino;

	/*
	 * If the inode reference array doesn't exist, create it.
	 */
	اگر (ref_ent->refs == 0) {
		سائز = (size_t) ((حجم(ساخت ext2_inode_reference) *
				  ent->max_refs));
		retval = ext2fs_get_array(ent->max_refs,
			حجم(ساخت ext2_inode_reference), &ref_ent->refs);
		اگر (retval)
			واپس retval;
		memset(ref_ent->refs, 0, سائز);
		ref_ent->عدد = 0;
	}

	اگر (ref_ent->عدد >= ent->max_refs)
		واپس EXT2_ET_TOO_MANY_REFS;

	ref_ent->refs[(غیرعلامتی) ref_ent->عدد++] = *ref;
	واپس 0;
}

ساکن errcode_t ima_آغاز_iter_ref(ext2_irel irel, ext2_ino_t ino)
{
	ساخت irel_ma 	*ma;

	ma = irel->priv_ڈیٹا;
	اگر (ino > ma->max_inode)
		واپس EXT2_ET_INVALID_دلیل;
	اگر (ma->entries[(غیرعلامتی) ino].new == 0)
		واپس ENOENT;
	ma->ref_current = ino;
	ma->ref_iter = 0;
	واپس 0;
}

ساکن errcode_t ima_اگلا_ref(ext2_irel irel,
			      ساخت ext2_inode_reference *ref)
{
	ساخت irel_ma 	*ma;
	ساخت inode_reference_اندراج *ref_ent;

	ma = irel->priv_ڈیٹا;

	ref_ent = ma->ref_entries + ma->ref_current;

	اگر ((ref_ent->refs == تہی) ||
	    (ma->ref_iter >= ref_ent->عدد)) {
		ref->بلاک = 0;
		ref->انحراف = 0;
		واپس 0;
	}
	*ref = ref_ent->refs[ma->ref_iter++];
	واپس 0;
}


ساکن errcode_t ima_move(ext2_irel irel, ext2_ino_t old, ext2_ino_t new)
{
	ساخت irel_ma 	*ma;

	ma = irel->priv_ڈیٹا;
	اگر ((old > ma->max_inode) || (new > ma->max_inode))
		واپس EXT2_ET_INVALID_دلیل;
	اگر (ma->entries[(غیرعلامتی) old].new == 0)
		واپس ENOENT;

	ma->entries[(غیرعلامتی) new] = ma->entries[(غیرعلامتی) old];
	اگر (ma->ref_entries[(غیرعلامتی) new].refs)
		ext2fs_free_mem(&ma->ref_entries[(غیرعلامتی) new].refs);
	ma->ref_entries[(غیرعلامتی) new] = ma->ref_entries[(غیرعلامتی) old];

	ma->entries[(غیرعلامتی) old].new = 0;
	ma->ref_entries[(غیرعلامتی) old].عدد = 0;
	ma->ref_entries[(غیرعلامتی) old].refs = 0;

	ma->orig_map[ma->entries[new].orig] = new;
	واپس 0;
}

ساکن errcode_t ima_delete(ext2_irel irel, ext2_ino_t old)
{
	ساخت irel_ma 	*ma;

	ma = irel->priv_ڈیٹا;
	اگر (old > ma->max_inode)
		واپس EXT2_ET_INVALID_دلیل;
	اگر (ma->entries[(غیرعلامتی) old].new == 0)
		واپس ENOENT;

	ma->entries[old].new = 0;
	اگر (ma->ref_entries[(غیرعلامتی) old].refs)
		ext2fs_free_mem(&ma->ref_entries[(غیرعلامتی) old].refs);
	ma->orig_map[ma->entries[(غیرعلامتی) old].orig] = 0;

	ma->ref_entries[(غیرعلامتی) old].عدد = 0;
	ma->ref_entries[(غیرعلامتی) old].refs = 0;
	واپس 0;
}

ساکن errcode_t ima_free(ext2_irel irel)
{
	ساخت irel_ma 	*ma;
	ext2_ino_t	ino;

	اگر (!irel)
		واپس 0;

	ma = irel->priv_ڈیٹا;

	اگر (ma) {
		اگر (ma->orig_map)
			ext2fs_free_mem(&ma->orig_map);
		اگر (ma->entries)
			ext2fs_free_mem(&ma->entries);
		اگر (ma->ref_entries) {
			برائے (ino = 0; ino <= ma->max_inode; ino++) {
				اگر (ma->ref_entries[(غیرعلامتی) ino].refs)
					ext2fs_free_mem(&ma->ref_entries[(غیرعلامتی) ino].refs);
			}
			ext2fs_free_mem(&ma->ref_entries);
		}
		ext2fs_free_mem(&ma);
	}
	اگر (irel->name)
		ext2fs_free_mem(&irel->name);
	ext2fs_free_mem(&irel);
	واپس 0;
}
