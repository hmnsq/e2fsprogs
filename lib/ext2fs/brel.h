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

संरचना ext2_खण्डम्_relocate_प्रविष्टिः {
	blk64_t	new;
	__s16	विस्थापनम्;
	__u16	flags;
	सङ्घ {
		blk64_t		खण्डम्_ref;
		ext2_ino_t	inode_ref;
	} owner;
};

#परिभाषय RELOCATE_TYPE_REF  0x0007
#परिभाषय RELOCATE_खण्डम्_REF 0x0001
#परिभाषय RELOCATE_INODE_REF 0x0002

प्रकारनिर्देश संरचना ext2_खण्डम्_relocation_सारणी *ext2_brel;

संरचना ext2_खण्डम्_relocation_सारणी {
	__u32	magic;
	वर्ण	*name;
	blk64_t	current;
	शून्य	*priv_दत्त;

	/*
	 * Add a block relocation entry.
	 */
	errcode_t (*put)(ext2_brel brel, blk64_t old,
			      संरचना ext2_खण्डम्_relocate_प्रविष्टिः *ent);

	/*
	 * Get a block relocation entry.
	 */
	errcode_t (*get)(ext2_brel brel, blk64_t old,
			      संरचना ext2_खण्डम्_relocate_प्रविष्टिः *ent);

	/*
	 * Initialize for iterating over the block relocation entries.
	 */
	errcode_t (*start_iter)(ext2_brel brel);

	/*
	 * The iterator function for the inode relocation entries.
	 * Returns an inode number of 0 when out of entries.
	 */
	errcode_t (*अग्र)(ext2_brel brel, blk64_t *old,
			  संरचना ext2_खण्डम्_relocate_प्रविष्टिः *ent);

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
	errcode_t (*मुक्त)(ext2_brel brel);
};

errcode_t ext2fs_brel_memarray_create(वर्ण *name, blk64_t max_खण्डम्,
				    ext2_brel *brel);

#परिभाषय ext2fs_brel_put(brel, old, ent) ((brel)->put((brel), old, ent))
#परिभाषय ext2fs_brel_get(brel, old, ent) ((brel)->get((brel), old, ent))
#परिभाषय ext2fs_brel_start_iter(brel) ((brel)->start_iter((brel)))
#परिभाषय ext2fs_brel_अग्र(brel, old, ent) ((brel)->अग्र((brel), old, ent))
#परिभाषय ext2fs_brel_move(brel, old, new) ((brel)->move((brel), old, new))
#परिभाषय ext2fs_brel_delete(brel, old) ((brel)->delete((brel), old))
#परिभाषय ext2fs_brel_free(brel) ((brel)->मुक्त((brel)))

