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

ರಚನೆ ext2_inode_reference {
	blk64_t	ಖಂಡ;
	__u16 ಸ್ಥಾನಾಂತರ;
};

ರಚನೆ ext2_inode_relocate_ನಮೂದು {
	ext2_ino_t	new;
	ext2_ino_t	orig;
	__u16		flags;
	__u16		max_refs;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_inode_relocation_ಕೋಷ್ಟಕ *ext2_irel;

ರಚನೆ ext2_inode_relocation_ಕೋಷ್ಟಕ {
	__u32	magic;
	ಅಕ್ಷರ	*name;
	ext2_ino_t	current;
	ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ;

	/*
	 * Add an inode relocation entry.
	 */
	errcode_t (*put)(ext2_irel irel, ext2_ino_t old,
			      ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent);
	/*
	 * Get an inode relocation entry.
	 */
	errcode_t (*get)(ext2_irel irel, ext2_ino_t old,
			      ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent);

	/*
	 * Get an inode relocation entry by its original inode number
	 */
	errcode_t (*get_by_orig)(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent);

	/*
	 * Initialize for iterating over the inode relocation entries.
	 */
	errcode_t (*ಆರಂಭ_iter)(ext2_irel irel);

	/*
	 * The iterator function for the inode relocation entries.
	 * Returns an inode number of 0 when out of entries.
	 */
	errcode_t (*ಮುಂದಿನ)(ext2_irel irel, ext2_ino_t *old,
			  ರಚನೆ ext2_inode_relocate_ನಮೂದು *ent);

	/*
	 * Add an inode reference (i.e., note the fact that a
	 * particular block/offset contains a reference to an inode)
	 */
	errcode_t (*ಕೂಡಿಸು_ref)(ext2_irel irel, ext2_ino_t ino,
			     ರಚನೆ ext2_inode_reference *ref);

	/*
	 * Initialize for iterating over the inode references for a
	 * particular inode.
	 */
	errcode_t (*ಆರಂಭ_iter_ref)(ext2_irel irel, ext2_ino_t ino);

	/*
	 * The iterator function for the inode references for an
	 * inode.  The references for only one inode can be iterator
	 * over at a time, as the iterator state is stored in ext2_irel.
	 */
	errcode_t (*ಮುಂದಿನ_ref)(ext2_irel irel,
			      ರಚನೆ ext2_inode_reference *ref);

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
	errcode_t (*ಬಿಡುಗಡೆ)(ext2_irel irel);
};

errcode_t ext2fs_irel_memarray_create(ಅಕ್ಷರ *name, ext2_ino_t max_inode,
				    ext2_irel *irel);

#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_put(irel, old, ent) ((irel)->put((irel), old, ent))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_get(irel, old, ent) ((irel)->get((irel), old, ent))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_get_by_orig(irel, orig, old, ent) \
			((irel)->get_by_orig((irel), orig, old, ent))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_ಆರಂಭ_iter(irel) ((irel)->ಆರಂಭ_iter((irel)))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_ಮುಂದಿನ(irel, old, ent) ((irel)->ಮುಂದಿನ((irel), old, ent))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_ಕೂಡಿಸು_ref(irel, ino, ref) ((irel)->ಕೂಡಿಸು_ref((irel), ino, ref))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_ಆರಂಭ_iter_ref(irel, ino) ((irel)->ಆರಂಭ_iter_ref((irel), ino))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_ಮುಂದಿನ_ref(irel, ref) ((irel)->ಮುಂದಿನ_ref((irel), ref))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_move(irel, old, new) ((irel)->move((irel), old, new))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_delete(irel, old) ((irel)->delete((irel), old))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_irel_free(irel) ((irel)->ಬಿಡುಗಡೆ((irel)))
