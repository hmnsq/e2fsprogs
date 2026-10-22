/*
 * brel.h
 *
 * Copyright (C) 1996, 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

بنية ext2_بلاک_relocate_ثبت {
	blk64_t	new;
	__s16	دننه_واټن;
	__u16	flags;
	اتحاد {
		blk64_t		بلاک_ref;
		ext2_ino_t	inode_ref;
	} owner;
};

#تعریف_کړه RELOCATE_TYPE_REF  0x0007
#تعریف_کړه RELOCATE_بلاک_REF 0x0001
#تعریف_کړه RELOCATE_INODE_REF 0x0002

تعريفنوع بنية ext2_بلاک_relocation_جدول *ext2_brel;

بنية ext2_بلاک_relocation_جدول {
	__u32	magic;
	حرف	*name;
	blk64_t	current;
	فراغ	*priv_معلومات;

	/*
	 * Add a block relocation entry.
	 */
	errcode_t (*put)(ext2_brel brel, blk64_t old,
			      بنية ext2_بلاک_relocate_ثبت *ent);

	/*
	 * Get a block relocation entry.
	 */
	errcode_t (*get)(ext2_brel brel, blk64_t old,
			      بنية ext2_بلاک_relocate_ثبت *ent);

	/*
	 * Initialize for iterating over the block relocation entries.
	 */
	errcode_t (*پیل_iter)(ext2_brel brel);

	/*
	 * The iterator function for the inode relocation entries.
	 * Returns an inode number of 0 when out of entries.
	 */
	errcode_t (*بل)(ext2_brel brel, blk64_t *old,
			  بنية ext2_بلاک_relocate_ثبت *ent);

	/*
	 * Move the inode relocation table from one block number to
	 * another.
	 */
	errcode_t (*move)(ext2_brel brel, blk64_t old, blk_t new);

	/*
	 * Remove a block relocation entry.
	 */
	errcode_t (*delete)(ext2_brel brel, blk64_t old);


	/*
	 * Free the block relocation table.
	 */
	errcode_t (*حرر)(ext2_brel brel);
};

errcode_t ext2fs_brel_memarray_create(حرف *name, blk64_t max_بلاک,
				    ext2_brel *brel);

#تعریف_کړه ext2fs_brel_put(brel, old, ent) ((brel)->put((brel), old, ent))
#تعریف_کړه ext2fs_brel_get(brel, old, ent) ((brel)->get((brel), old, ent))
#تعریف_کړه ext2fs_brel_پیل_iter(brel) ((brel)->پیل_iter((brel)))
#تعریف_کړه ext2fs_brel_بل(brel, old, ent) ((brel)->بل((brel), old, ent))
#تعریف_کړه ext2fs_brel_move(brel, old, new) ((brel)->move((brel), old, new))
#تعریف_کړه ext2fs_brel_delete(brel, old) ((brel)->delete((brel), old))
#تعریف_کړه ext2fs_brel_free(brel) ((brel)->حرر((brel)))

