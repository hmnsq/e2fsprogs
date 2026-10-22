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

ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು {
	blk64_t	new;
	__s16	ಸ್ಥಾನಾಂತರ;
	__u16	flags;
	ಒಕ್ಕೂಟ {
		blk64_t		ಖಂಡ_ref;
		ext2_ino_t	inode_ref;
	} owner;
};

#ವ್ಯಾಖ್ಯಾನಿಸು RELOCATE_TYPE_REF  0x0007
#ವ್ಯಾಖ್ಯಾನಿಸು RELOCATE_ಖಂಡ_REF 0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು RELOCATE_INODE_REF 0x0002

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_ಖಂಡ_relocation_ಕೋಷ್ಟಕ *ext2_brel;

ರಚನೆ ext2_ಖಂಡ_relocation_ಕೋಷ್ಟಕ {
	__u32	magic;
	ಅಕ್ಷರ	*name;
	blk64_t	current;
	ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ;

	/*
	 * Add a block relocation entry.
	 */
	errcode_t (*put)(ext2_brel brel, blk64_t old,
			      ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು *ent);

	/*
	 * Get a block relocation entry.
	 */
	errcode_t (*get)(ext2_brel brel, blk64_t old,
			      ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು *ent);

	/*
	 * Initialize for iterating over the block relocation entries.
	 */
	errcode_t (*ಆರಂಭ_iter)(ext2_brel brel);

	/*
	 * The iterator function for the inode relocation entries.
	 * Returns an inode number of 0 when out of entries.
	 */
	errcode_t (*ಮುಂದಿನ)(ext2_brel brel, blk64_t *old,
			  ರಚನೆ ext2_ಖಂಡ_relocate_ನಮೂದು *ent);

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
	errcode_t (*ಬಿಡುಗಡೆ)(ext2_brel brel);
};

errcode_t ext2fs_brel_memarray_create(ಅಕ್ಷರ *name, blk64_t max_ಖಂಡ,
				    ext2_brel *brel);

#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_brel_put(brel, old, ent) ((brel)->put((brel), old, ent))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_brel_get(brel, old, ent) ((brel)->get((brel), old, ent))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_brel_ಆರಂಭ_iter(brel) ((brel)->ಆರಂಭ_iter((brel)))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_brel_ಮುಂದಿನ(brel, old, ent) ((brel)->ಮುಂದಿನ((brel), old, ent))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_brel_move(brel, old, new) ((brel)->move((brel), old, new))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_brel_delete(brel, old) ((brel)->delete((brel), old))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_brel_free(brel) ((brel)->ಬಿಡುಗಡೆ((brel)))

