/*
 * irel.h
 *
 * Copyright (C) 1996, 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

بنية ext2_inode_reference {
	blk64_t	بلاک;
	__u16 دننه_واټن;
};

بنية ext2_inode_relocate_ثبت {
	ext2_ino_t	new;
	ext2_ino_t	orig;
	__u16		flags;
	__u16		max_refs;
};

تعريفنوع بنية ext2_inode_relocation_جدول *ext2_irel;

بنية ext2_inode_relocation_جدول {
	__u32	magic;
	حرف	*name;
	ext2_ino_t	current;
	فراغ	*priv_معلومات;

	/*
	 * Add an inode relocation entry.
	 */
	errcode_t (*put)(ext2_irel irel, ext2_ino_t old,
			      بنية ext2_inode_relocate_ثبت *ent);
	/*
	 * Get an inode relocation entry.
	 */
	errcode_t (*get)(ext2_irel irel, ext2_ino_t old,
			      بنية ext2_inode_relocate_ثبت *ent);

	/*
	 * Get an inode relocation entry by its original inode number
	 */
	errcode_t (*get_by_orig)(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 بنية ext2_inode_relocate_ثبت *ent);

	/*
	 * Initialize for iterating over the inode relocation entries.
	 */
	errcode_t (*پیل_iter)(ext2_irel irel);

	/*
	 * The iterator function for the inode relocation entries.
	 * Returns an inode number of 0 when out of entries.
	 */
	errcode_t (*بل)(ext2_irel irel, ext2_ino_t *old,
			  بنية ext2_inode_relocate_ثبت *ent);

	/*
	 * Add an inode reference (i.e., note the fact that a
	 * particular block/offset contains a reference to an inode)
	 */
	errcode_t (*جمع_کړه_ref)(ext2_irel irel, ext2_ino_t ino,
			     بنية ext2_inode_reference *ref);

	/*
	 * Initialize for iterating over the inode references for a
	 * particular inode.
	 */
	errcode_t (*پیل_iter_ref)(ext2_irel irel, ext2_ino_t ino);

	/*
	 * The iterator function for the inode references for an
	 * inode.  The references for only one inode can be iterator
	 * over at a time, as the iterator state is stored in ext2_irel.
	 */
	errcode_t (*بل_ref)(ext2_irel irel,
			      بنية ext2_inode_reference *ref);

	/*
	 * Move the inode relocation table from one inode number to
	 * another.  Note that the inode references also must move.
	 */
	errcode_t (*move)(ext2_irel irel, ext2_ino_t old, ext2_ino_t new);

	/*
	 * Remove an inode relocation entry, along with all of the
	 * inode references.
	 */
	errcode_t (*delete)(ext2_irel irel, ext2_ino_t old);

	/*
	 * Free the inode relocation table.
	 */
	errcode_t (*حرر)(ext2_irel irel);
};

errcode_t ext2fs_irel_memarray_create(حرف *name, ext2_ino_t max_inode,
				    ext2_irel *irel);

#تعریف_کړه ext2fs_irel_put(irel, old, ent) ((irel)->put((irel), old, ent))
#تعریف_کړه ext2fs_irel_get(irel, old, ent) ((irel)->get((irel), old, ent))
#تعریف_کړه ext2fs_irel_get_by_orig(irel, orig, old, ent) \
			((irel)->get_by_orig((irel), orig, old, ent))
#تعریف_کړه ext2fs_irel_پیل_iter(irel) ((irel)->پیل_iter((irel)))
#تعریف_کړه ext2fs_irel_بل(irel, old, ent) ((irel)->بل((irel), old, ent))
#تعریف_کړه ext2fs_irel_جمع_کړه_ref(irel, ino, ref) ((irel)->جمع_کړه_ref((irel), ino, ref))
#تعریف_کړه ext2fs_irel_پیل_iter_ref(irel, ino) ((irel)->پیل_iter_ref((irel), ino))
#تعریف_کړه ext2fs_irel_بل_ref(irel, ref) ((irel)->بل_ref((irel), ref))
#تعریف_کړه ext2fs_irel_move(irel, old, new) ((irel)->move((irel), old, new))
#تعریف_کړه ext2fs_irel_delete(irel, old) ((irel)->delete((irel), old))
#تعریف_کړه ext2fs_irel_free(irel) ((irel)->حرر((irel)))
