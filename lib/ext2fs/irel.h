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

నిర్మాణం ext2_inode_reference {
	blk64_t	ఖండం;
	__u16 స్థానభ్రంశం;
};

నిర్మాణం ext2_inode_relocate_నమోదు {
	ext2_ino_t	new;
	ext2_ino_t	orig;
	__u16		flags;
	__u16		max_refs;
};

రకనిర్వచనం నిర్మాణం ext2_inode_relocation_పట్టిక *ext2_irel;

నిర్మాణం ext2_inode_relocation_పట్టిక {
	__u32	magic;
	అక్షరం	*name;
	ext2_ino_t	current;
	శూన్య	*priv_దత్తాంశం;

	/*
	 * Add an inode relocation entry.
	 */
	errcode_t (*put)(ext2_irel irel, ext2_ino_t old,
			      నిర్మాణం ext2_inode_relocate_నమోదు *ent);
	/*
	 * Get an inode relocation entry.
	 */
	errcode_t (*get)(ext2_irel irel, ext2_ino_t old,
			      నిర్మాణం ext2_inode_relocate_నమోదు *ent);

	/*
	 * Get an inode relocation entry by its original inode number
	 */
	errcode_t (*get_by_orig)(ext2_irel irel, ext2_ino_t orig, ext2_ino_t *old,
				 నిర్మాణం ext2_inode_relocate_నమోదు *ent);

	/*
	 * Initialize for iterating over the inode relocation entries.
	 */
	errcode_t (*ప్రారంభం_iter)(ext2_irel irel);

	/*
	 * The iterator function for the inode relocation entries.
	 * Returns an inode number of 0 when out of entries.
	 */
	errcode_t (*తదుపరి)(ext2_irel irel, ext2_ino_t *old,
			  నిర్మాణం ext2_inode_relocate_నమోదు *ent);

	/*
	 * Add an inode reference (i.e., note the fact that a
	 * particular block/offset contains a reference to an inode)
	 */
	errcode_t (*కూడు_ref)(ext2_irel irel, ext2_ino_t ino,
			     నిర్మాణం ext2_inode_reference *ref);

	/*
	 * Initialize for iterating over the inode references for a
	 * particular inode.
	 */
	errcode_t (*ప్రారంభం_iter_ref)(ext2_irel irel, ext2_ino_t ino);

	/*
	 * The iterator function for the inode references for an
	 * inode.  The references for only one inode can be iterator
	 * over at a time, as the iterator state is stored in ext2_irel.
	 */
	errcode_t (*తదుపరి_ref)(ext2_irel irel,
			      నిర్మాణం ext2_inode_reference *ref);

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
	errcode_t (*విడుదల)(ext2_irel irel);
};

errcode_t ext2fs_irel_memarray_create(అక్షరం *name, ext2_ino_t max_inode,
				    ext2_irel *irel);

#నిర్వచించు ext2fs_irel_put(irel, old, ent) ((irel)->put((irel), old, ent))
#నిర్వచించు ext2fs_irel_get(irel, old, ent) ((irel)->get((irel), old, ent))
#నిర్వచించు ext2fs_irel_get_by_orig(irel, orig, old, ent) \
			((irel)->get_by_orig((irel), orig, old, ent))
#నిర్వచించు ext2fs_irel_ప్రారంభం_iter(irel) ((irel)->ప్రారంభం_iter((irel)))
#నిర్వచించు ext2fs_irel_తదుపరి(irel, old, ent) ((irel)->తదుపరి((irel), old, ent))
#నిర్వచించు ext2fs_irel_కూడు_ref(irel, ino, ref) ((irel)->కూడు_ref((irel), ino, ref))
#నిర్వచించు ext2fs_irel_ప్రారంభం_iter_ref(irel, ino) ((irel)->ప్రారంభం_iter_ref((irel), ino))
#నిర్వచించు ext2fs_irel_తదుపరి_ref(irel, ref) ((irel)->తదుపరి_ref((irel), ref))
#నిర్వచించు ext2fs_irel_move(irel, old, new) ((irel)->move((irel), old, new))
#నిర్వచించు ext2fs_irel_delete(irel, old) ((irel)->delete((irel), old))
#నిర్వచించు ext2fs_irel_free(irel) ((irel)->విడుదల((irel)))
