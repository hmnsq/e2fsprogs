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

ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ {
	blk64_t	new;
	__s16	ਵਿੱਥ;
	__u16	flags;
	ਸੰਘ {
		blk64_t		ਖੰਡ_ref;
		ext2_ino_t	inode_ref;
	} owner;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RELOCATE_TYPE_REF  0x0007
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RELOCATE_ਖੰਡ_REF 0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RELOCATE_INODE_REF 0x0002

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_ਖੰਡ_relocation_ਸਾਰਣੀ *ext2_brel;

ਬਣਤਰ ext2_ਖੰਡ_relocation_ਸਾਰਣੀ {
	__u32	magic;
	ਅੱਖਰ	*name;
	blk64_t	current;
	ਖਾਲੀ	*priv_ਅੰਕੜੇ;

	/*
	 * Add a block relocation entry.
	 */
	errcode_t (*put)(ext2_brel brel, blk64_t old,
			      ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ *ent);

	/*
	 * Get a block relocation entry.
	 */
	errcode_t (*get)(ext2_brel brel, blk64_t old,
			      ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ *ent);

	/*
	 * Initialize for iterating over the block relocation entries.
	 */
	errcode_t (*ਸ਼ੁਰੂ_iter)(ext2_brel brel);

	/*
	 * The iterator function for the inode relocation entries.
	 * Returns an inode number of 0 when out of entries.
	 */
	errcode_t (*ਅਗਲਾ)(ext2_brel brel, blk64_t *old,
			  ਬਣਤਰ ext2_ਖੰਡ_relocate_ਇੰਦਰਾਜ *ent);

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
	errcode_t (*ਮੁਕਤ)(ext2_brel brel);
};

errcode_t ext2fs_brel_memarray_create(ਅੱਖਰ *name, blk64_t max_ਖੰਡ,
				    ext2_brel *brel);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_brel_put(brel, old, ent) ((brel)->put((brel), old, ent))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_brel_get(brel, old, ent) ((brel)->get((brel), old, ent))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_brel_ਸ਼ੁਰੂ_iter(brel) ((brel)->ਸ਼ੁਰੂ_iter((brel)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_brel_ਅਗਲਾ(brel, old, ent) ((brel)->ਅਗਲਾ((brel), old, ent))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_brel_move(brel, old, new) ((brel)->move((brel), old, new))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_brel_delete(brel, old) ((brel)->delete((brel), old))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_brel_free(brel) ((brel)->ਮੁਕਤ((brel)))

