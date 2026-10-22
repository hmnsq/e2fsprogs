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

ساخت ext2_بلاک_relocate_اندراج {
	blk64_t	new;
	__s16	انحراف;
	__u16	flags;
	اتحاد {
		blk64_t		بلاک_ref;
		ext2_ino_t	inode_ref;
	} owner;
};

#تعریف_کرو RELOCATE_TYPE_REF  0x0007
#تعریف_کرو RELOCATE_بلاک_REF 0x0001
#تعریف_کرو RELOCATE_INODE_REF 0x0002

قسمتعریف ساخت ext2_بلاک_relocation_جدول *ext2_brel;

ساخت ext2_بلاک_relocation_جدول {
	__u32	magic;
	حرف	*name;
	blk64_t	current;
	خالی	*priv_ڈیٹا;

	/*
	 * Add a block relocation entry.
	 */
	errcode_t (*put)(ext2_brel brel, blk64_t old,
			      ساخت ext2_بلاک_relocate_اندراج *ent);

	/*
	 * Get a block relocation entry.
	 */
	errcode_t (*get)(ext2_brel brel, blk64_t old,
			      ساخت ext2_بلاک_relocate_اندراج *ent);

	/*
	 * Initialize for iterating over the block relocation entries.
	 */
	errcode_t (*آغاز_iter)(ext2_brel brel);

	/*
	 * The iterator function for the inode relocation entries.
	 * Returns an inode number of 0 when out of entries.
	 */
	errcode_t (*اگلا)(ext2_brel brel, blk64_t *old,
			  ساخت ext2_بلاک_relocate_اندراج *ent);

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
	errcode_t (*آزاد)(ext2_brel brel);
};

errcode_t ext2fs_brel_memarray_create(حرف *name, blk64_t max_بلاک,
				    ext2_brel *brel);

#تعریف_کرو ext2fs_brel_put(brel, old, ent) ((brel)->put((brel), old, ent))
#تعریف_کرو ext2fs_brel_get(brel, old, ent) ((brel)->get((brel), old, ent))
#تعریف_کرو ext2fs_brel_آغاز_iter(brel) ((brel)->آغاز_iter((brel)))
#تعریف_کرو ext2fs_brel_اگلا(brel, old, ent) ((brel)->اگلا((brel), old, ent))
#تعریف_کرو ext2fs_brel_move(brel, old, new) ((brel)->move((brel), old, new))
#تعریف_کرو ext2fs_brel_delete(brel, old) ((brel)->delete((brel), old))
#تعریف_کرو ext2fs_brel_free(brel) ((brel)->آزاد((brel)))

