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

నిర్మాణం ext2_ఖండం_relocate_నమోదు {
	blk64_t	new;
	__s16	స్థానభ్రంశం;
	__u16	flags;
	సంఘం {
		blk64_t		ఖండం_ref;
		ext2_ino_t	inode_ref;
	} owner;
};

#నిర్వచించు RELOCATE_TYPE_REF  0x0007
#నిర్వచించు RELOCATE_ఖండం_REF 0x0001
#నిర్వచించు RELOCATE_INODE_REF 0x0002

రకనిర్వచనం నిర్మాణం ext2_ఖండం_relocation_పట్టిక *ext2_brel;

నిర్మాణం ext2_ఖండం_relocation_పట్టిక {
	__u32	magic;
	అక్షరం	*name;
	blk64_t	current;
	శూన్య	*priv_దత్తాంశం;

	/*
	 * Add a block relocation entry.
	 */
	errcode_t (*put)(ext2_brel brel, blk64_t old,
			      నిర్మాణం ext2_ఖండం_relocate_నమోదు *ent);

	/*
	 * Get a block relocation entry.
	 */
	errcode_t (*get)(ext2_brel brel, blk64_t old,
			      నిర్మాణం ext2_ఖండం_relocate_నమోదు *ent);

	/*
	 * Initialize for iterating over the block relocation entries.
	 */
	errcode_t (*ప్రారంభం_iter)(ext2_brel brel);

	/*
	 * The iterator function for the inode relocation entries.
	 * Returns an inode number of 0 when out of entries.
	 */
	errcode_t (*తదుపరి)(ext2_brel brel, blk64_t *old,
			  నిర్మాణం ext2_ఖండం_relocate_నమోదు *ent);

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
	errcode_t (*విడుదల)(ext2_brel brel);
};

errcode_t ext2fs_brel_memarray_create(అక్షరం *name, blk64_t max_ఖండం,
				    ext2_brel *brel);

#నిర్వచించు ext2fs_brel_put(brel, old, ent) ((brel)->put((brel), old, ent))
#నిర్వచించు ext2fs_brel_get(brel, old, ent) ((brel)->get((brel), old, ent))
#నిర్వచించు ext2fs_brel_ప్రారంభం_iter(brel) ((brel)->ప్రారంభం_iter((brel)))
#నిర్వచించు ext2fs_brel_తదుపరి(brel, old, ent) ((brel)->తదుపరి((brel), old, ent))
#నిర్వచించు ext2fs_brel_move(brel, old, new) ((brel)->move((brel), old, new))
#నిర్వచించు ext2fs_brel_delete(brel, old) ((brel)->delete((brel), old))
#నిర్వచించు ext2fs_brel_free(brel) ((brel)->విడుదల((brel)))

