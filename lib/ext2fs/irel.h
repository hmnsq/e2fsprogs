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

संरचना ext2_inode_reference {
	blk64_t	खण्डम्;
	__u16 विस्थापनम्;
};

संरचना ext2_inode_relocate_प्रविष्टिः {
	ext2_ino_t	new;
	ext2_ino_t	orig;
	__u16		flags;
	__u16		max_refs;
};

प्रकारनिर्देश संरचना ext2_inode_relocation_सारणी *ext2_irel;

संरचना ext2_inode_relocation_सारणी {
	__u32	magic;
	वर्ण	*name;
	ext2_ino_t	current;
	शून्य	*priv_दत्त;

	/*
	 * Add an inode relocation entry.
	 */
	errcode_t (*put)(ext2_irel irel, ext2_ino_t old,
			      संरचना ext2_inode_relocate_प्रविष्टिः *ent);
	/*
	 * Get an inode relocation entry.
	 */
	errcode_t (*get)(ext2_irel irel, ext2_ino_t old,
			      संरचना ext2_inode_relocate_प्रविष्टिः *ent);

	/*
	 * Get an inode relocation entry by its original inode number
	 */
	errcode_t (*get_by_orig)(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 संरचना ext2_inode_relocate_प्रविष्टिः *ent);

	/*
	 * Initialize for iterating over the inode relocation entries.
	 */
	errcode_t (*start_iter)(ext2_irel irel);

	/*
	 * The iterator function for the inode relocation entries.
	 * Returns an inode number of 0 when out of entries.
	 */
	errcode_t (*अग्र)(ext2_irel irel, ext2_ino_t *old,
			  संरचना ext2_inode_relocate_प्रविष्टिः *ent);

	/*
	 * Add an inode reference (i.e., note the fact that a
	 * particular block/offset contains a reference to an inode)
	 */
	errcode_t (*सङ्कलय_ref)(ext2_irel irel, ext2_ino_t ino,
			     संरचना ext2_inode_reference *ref);

	/*
	 * Initialize for iterating over the inode references for a
	 * particular inode.
	 */
	errcode_t (*start_iter_ref)(ext2_irel irel, ext2_ino_t ino);

	/*
	 * The iterator function for the inode references for an
	 * inode.  The references for only one inode can be iterator
	 * over at a time, as the iterator state is stored in ext2_irel.
	 */
	errcode_t (*अग्र_ref)(ext2_irel irel,
			      संरचना ext2_inode_reference *ref);

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
	errcode_t (*मुक्त)(ext2_irel irel);
};

errcode_t ext2fs_irel_memarray_create(वर्ण *name, ext2_ino_t max_inode,
				    ext2_irel *irel);

#परिभाषय ext2fs_irel_put(irel, old, ent) ((irel)->put((irel), old, ent))
#परिभाषय ext2fs_irel_get(irel, old, ent) ((irel)->get((irel), old, ent))
#परिभाषय ext2fs_irel_get_by_orig(irel, orig, old, ent) \
			((irel)->get_by_orig((irel), orig, old, ent))
#परिभाषय ext2fs_irel_start_iter(irel) ((irel)->start_iter((irel)))
#परिभाषय ext2fs_irel_अग्र(irel, old, ent) ((irel)->अग्र((irel), old, ent))
#परिभाषय ext2fs_irel_सङ्कलय_ref(irel, ino, ref) ((irel)->सङ्कलय_ref((irel), ino, ref))
#परिभाषय ext2fs_irel_start_iter_ref(irel, ino) ((irel)->start_iter_ref((irel), ino))
#परिभाषय ext2fs_irel_अग्र_ref(irel, ref) ((irel)->अग्र_ref((irel), ref))
#परिभाषय ext2fs_irel_move(irel, old, new) ((irel)->move((irel), old, new))
#परिभाषय ext2fs_irel_delete(irel, old) ((irel)->delete((irel), old))
#परिभाषय ext2fs_irel_free(irel) ((irel)->मुक्त((irel)))
