/*
 * blknum.c --- Functions to handle blk64_t and high/low 64-bit block
 * number.
 *
 * Copyright IBM Corporation, 2007
 * Author Jose R. Santos <jrs@us.ibm.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ "ext2fs.h"

/*
 * Return the group # of a block
 */
dgrp_t ext2fs_సమూహం_యొక్క_blk2(ext2_filsys fs, blk64_t blk)
{
	తిరిగి (blk - fs->super->s_మొదటి_దత్తాంశం_ఖండం) /
		fs->super->s_ఖండాలు_చొప్పున_సమూహం;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_సమూహం_మొదటి_block2(ext2_filsys fs, dgrp_t సమూహం)
{
	తిరిగి fs->super->s_మొదటి_దత్తాంశం_ఖండం +
		EXT2_GROUPS_గమ్యం_ఖండాలు(fs->super, సమూహం);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_సమూహం_చివరి_block2(ext2_filsys fs, dgrp_t సమూహం)
{
	తిరిగి (సమూహం == fs->సమూహం_desc_లెక్క - 1 ?
		ext2fs_ఖండాలు_లెక్క(fs->super) - 1 :
		ext2fs_సమూహం_మొదటి_block2(fs, సమూహం) +
			(fs->super->s_ఖండాలు_చొప్పున_సమూహం - 1));
}

/*
 * Return the number of blocks in a group
 */
పూర్ణాంకం ext2fs_సమూహం_ఖండాలు_లెక్క(ext2_filsys fs, dgrp_t సమూహం)
{
	పూర్ణాంకం అంకె_ఖండాలు;

	ఒకవేళ (సమూహం == fs->సమూహం_desc_లెక్క - 1) {
		అంకె_ఖండాలు = (ext2fs_ఖండాలు_లెక్క(fs->super) -
				fs->super->s_మొదటి_దత్తాంశం_ఖండం) %
			      fs->super->s_ఖండాలు_చొప్పున_సమూహం;
		ఒకవేళ (!అంకె_ఖండాలు)
			అంకె_ఖండాలు = fs->super->s_ఖండాలు_చొప్పున_సమూహం;
	} లేకపోతే
		అంకె_ఖండాలు = fs->super->s_ఖండాలు_చొప్పున_సమూహం;

	తిరిగి అంకె_ఖండాలు;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_దత్తాంశం_blocks2(ext2_filsys fs,
					నిర్మాణం ext2_inode *inode)
{
	తిరిగి (inode->i_ఖండాలు |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_ఖండాలు_hi << 32 : 0)) -
		(inode->i_file_acl ? EXT2_CLUSTER_కొలత(fs->super) >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_ఖండాలు(ext2_filsys fs,
					నిర్మాణం ext2_inode *inode)
{
	తిరిగి (inode->i_ఖండాలు |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_ఖండాలు_hi << 32 : 0));
}

/*
 * Return the inode i_blocks in stat (512 byte) units
 */
blk64_t ext2fs_get_stat_i_ఖండాలు(ext2_filsys fs,
				 నిర్మాణం ext2_inode *inode)
{
	blk64_t	ret = inode->i_ఖండాలు;

	ఒకవేళ (ext2fs_has_feature_huge_file(fs->super)) {
		ret += ((దీర్ఘ దీర్ఘ) inode->osd2.linux2.l_i_ఖండాలు_hi) << 32;
		ఒకవేళ (inode->i_flags & EXT4_HUGE_FILE_FL)
			ret *= (fs->blocksize / 512);
	}
	తిరిగి ret;
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_ఖండాలు_లెక్క(నిర్మాణం ext2_super_ఖండం *super)
{
	తిరిగి super->s_ఖండాలు_లెక్క |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_ఖండాలు_లెక్క_hi << 32 : 0);
}

/*
 * Set the fs block count
 */
శూన్య ext2fs_ఖండాలు_లెక్క_సమితి(నిర్మాణం ext2_super_ఖండం *super, blk64_t blk)
{
	super->s_ఖండాలు_లెక్క = blk;
	ఒకవేళ (ext2fs_has_feature_64bit(super))
		super->s_ఖండాలు_లెక్క_hi = (__u64) blk >> 32;
}

/*
 * Add to the current fs block count
 */
శూన్య ext2fs_ఖండాలు_లెక్క_కూడు(నిర్మాణం ext2_super_ఖండం *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_ఖండాలు_లెక్క(super) + blk;
	ext2fs_ఖండాలు_లెక్క_సమితి(super, tmp);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_ఖండాలు_లెక్క(నిర్మాణం ext2_super_ఖండం *super)
{
	తిరిగి super->s_r_ఖండాలు_లెక్క |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_ఖండాలు_లెక్క_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
శూన్య ext2fs_r_ఖండాలు_లెక్క_సమితి(నిర్మాణం ext2_super_ఖండం *super, blk64_t blk)
{
	super->s_r_ఖండాలు_లెక్క = blk;
	ఒకవేళ (ext2fs_has_feature_64bit(super))
		super->s_r_ఖండాలు_లెక్క_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
శూన్య ext2fs_r_ఖండాలు_లెక్క_కూడు(నిర్మాణం ext2_super_ఖండం *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_ఖండాలు_లెక్క(super) + blk;
	ext2fs_r_ఖండాలు_లెక్క_సమితి(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_ఖండాలు_లెక్క(నిర్మాణం ext2_super_ఖండం *super)
{
	తిరిగి super->s_free_ఖండాలు_లెక్క |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_ఖండాలు_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
శూన్య ext2fs_free_ఖండాలు_లెక్క_సమితి(నిర్మాణం ext2_super_ఖండం *super, blk64_t blk)
{
	super->s_free_ఖండాలు_లెక్క = blk;
	ఒకవేళ (ext2fs_has_feature_64bit(super))
		super->s_free_ఖండాలు_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
శూన్య ext2fs_free_ఖండాలు_లెక్క_కూడు(నిర్మాణం ext2_super_ఖండం *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_ఖండాలు_లెక్క(super) + blk;
	ext2fs_free_ఖండాలు_లెక్క_సమితి(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
నిర్మాణం ext2_సమూహం_desc *ext2fs_సమూహం_desc(ext2_filsys fs,
					  నిర్మాణం opaque_ext2_సమూహం_desc *gdp,
					  dgrp_t సమూహం)
{
	నిర్మాణం ext2_సమూహం_desc *ret_gdp;
	errcode_t	retval;
	స్థిర అక్షరం	*buf = 0;
	స్థిర గుర్తులేని	bufsize = 0;
	blk64_t		blk;
	పూర్ణాంకం		desc_కొలత = EXT2_DESC_కొలత(fs->super) & ~7;
	పూర్ణాంకం		desc_చొప్పున_blk = EXT2_DESC_చొప్పున_ఖండం(fs->super);

	ఒకవేళ (సమూహం > fs->సమూహం_desc_లెక్క)
		తిరిగి ఖాళీ;
	ఒకవేళ (gdp)
		తిరిగి (నిర్మాణం ext2_సమూహం_desc *)((అక్షరం *)gdp +
						  సమూహం * desc_కొలత);
	/*
	 * If fs->group_desc wasn't read in when the file system was
	 * opened, then read it on demand here.
	 */
	ఒకవేళ (bufsize < fs->blocksize)
		ext2fs_free_mem(&buf);
	ఒకవేళ (!buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ఒకవేళ (retval)
			తిరిగి ఖాళీ;
		bufsize = fs->blocksize;
	}
	blk = ext2fs_descriptor_ఖండం_loc2(fs, fs->super->s_మొదటి_దత్తాంశం_ఖండం,
					   సమూహం / desc_చొప్పున_blk);
	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	ఒకవేళ (retval)
		తిరిగి ఖాళీ;
	ret_gdp = (నిర్మాణం ext2_సమూహం_desc *)
		(buf + ((సమూహం % desc_చొప్పున_blk) * desc_కొలత));
#నిర్వచితమైతే పదాలు_BIGENDIAN
	ext2fs_మార్పిడి_సమూహం_desc2(fs, ret_gdp);
#అయితే_ముగింపు
	తిరిగి ret_gdp;
}

/* Do the same but as an ext4 group desc for internal use here */
స్థిర నిర్మాణం ext4_సమూహం_desc *ext4fs_సమూహం_desc(ext2_filsys fs,
					  నిర్మాణం opaque_ext2_సమూహం_desc *gdp,
					  dgrp_t సమూహం)
{
	తిరిగి (నిర్మాణం ext4_సమూహం_desc *)ext2fs_సమూహం_desc(fs, gdp, సమూహం);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_ఖండం_bitmap_checksum(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;
	__u32 csum;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	csum = gdp->bg_ఖండం_bitmap_csum_lo;
	ఒకవేళ (EXT2_DESC_కొలత(fs->super) >= EXT4_BG_ఖండం_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_ఖండం_bitmap_csum_hi << 16);
	తిరిగి csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_ఖండం_bitmap_loc(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	తిరిగి gdp->bg_ఖండం_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_ఖండం_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
శూన్య ext2fs_ఖండం_bitmap_loc_సమితి(ext2_filsys fs, dgrp_t సమూహం, blk64_t blk)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_ఖండం_bitmap = blk;
	ఒకవేళ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_ఖండం_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap checksum of a group
 */
__u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;
	__u32 csum;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	csum = gdp->bg_inode_bitmap_csum_lo;
	ఒకవేళ (EXT2_DESC_కొలత(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_చివర)
		csum |= ((__u32)gdp->bg_inode_bitmap_csum_hi << 16);
	తిరిగి csum;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	తిరిగి gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

/*
 * Set the inode bitmap block of a group
 */
శూన్య ext2fs_inode_bitmap_loc_సమితి(ext2_filsys fs, dgrp_t సమూహం, blk64_t blk)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_inode_bitmap = blk;
	ఒకవేళ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_పట్టిక_loc(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	తిరిగి gdp->bg_inode_పట్టిక |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_పట్టిక_hi << 32 : 0);
}

/*
 * Set the inode table block of a group
 */
శూన్య ext2fs_inode_పట్టిక_loc_సమితి(ext2_filsys fs, dgrp_t సమూహం, blk64_t blk)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_inode_పట్టిక = blk;
	ఒకవేళ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_పట్టిక_hi = (__u64) blk >> 32;
}

/*
 * Return the free blocks count of a group
 */
__u32 ext2fs_bg_free_ఖండాలు_లెక్క(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	తిరిగి gdp->bg_free_ఖండాలు_లెక్క |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_ఖండాలు_లెక్క_hi << 16 : 0);
}

/*
 * Set the free blocks count of a group
 */
శూన్య ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(ext2_filsys fs, dgrp_t సమూహం, __u32 n)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_free_ఖండాలు_లెక్క = n;

	ఒకవేళ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_ఖండాలు_లెక్క_hi = (__u32) n >> 16;
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_లెక్క(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	తిరిగి gdp->bg_free_inodes_లెక్క |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_లెక్క_hi << 16 : 0);
}

/*
 * Set the free inodes count of a group
 */
శూన్య ext2fs_bg_free_inodes_లెక్క_సమితి(ext2_filsys fs, dgrp_t సమూహం, __u32 n)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_free_inodes_లెక్క = n;
	ఒకవేళ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_inodes_లెక్క_hi = (__u32) n >> 16;
}

/*
 * Return the used dirs count of a group
 */
__u32 ext2fs_bg_వాడిన_dirs_లెక్క(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	తిరిగి gdp->bg_వాడిన_dirs_లెక్క |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_వాడిన_dirs_లెక్క_hi << 16 : 0);
}

/*
 * Set the used dirs count of a group
 */
శూన్య ext2fs_bg_వాడిన_dirs_లెక్క_సమితి(ext2_filsys fs, dgrp_t సమూహం, __u32 n)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_వాడిన_dirs_లెక్క = n;
	ఒకవేళ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_వాడిన_dirs_లెక్క_hi = (__u32) n >> 16;
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_వాడని(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	తిరిగి gdp->bg_itable_వాడని |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_వాడని_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
శూన్య ext2fs_bg_itable_వాడని_సమితి(ext2_filsys fs, dgrp_t సమూహం, __u32 n)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_itable_వాడని = n;
	ఒకవేళ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_వాడని_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	తిరిగి gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
శూన్య ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_flags = 0;
	తిరిగి;
}

/*
 * Get the value of a particular flag for this block group
 */
పూర్ణాంకం ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t సమూహం, __u16 bg_flag)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	తిరిగి gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
శూన్య ext2fs_bg_flags_సమితి(ext2_filsys fs, dgrp_t సమూహం, __u16 bg_flags)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_flags |= bg_flags;
	తిరిగి;
}

/*
 * Clear a flag or set of flags for this block group
 */
శూన్య ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t సమూహం, __u16 bg_flags)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_flags &= ~bg_flags;
	తిరిగి;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	తిరిగి gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
శూన్య ext2fs_bg_checksum_సమితి(ext2_filsys fs, dgrp_t సమూహం, __u16 checksum)
{
	నిర్మాణం ext4_సమూహం_desc *gdp;

	gdp = ext4fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	gdp->bg_checksum = checksum;
	తిరిగి;
}

/*
 * Get the acl block of a file
 */
blk64_t ext2fs_file_acl_ఖండం(ext2_filsys fs, స్థిరరాశి నిర్మాణం ext2_inode *inode)
{
	blk64_t	blk = inode->i_file_acl;

	ఒకవేళ (fs && ext2fs_has_feature_64bit(fs->super))
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_ఎగువ) << 32;
	తిరిగి blk;
}

/*
 * Set the acl block of a file
 */
శూన్య ext2fs_file_acl_ఖండం_సమితి(ext2_filsys fs, నిర్మాణం ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	ఒకవేళ (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_ఎగువ = (__u64) blk >> 32;
}

/*
 * Set the size of the inode
 */
errcode_t ext2fs_inode_కొలత_సమితి(ext2_filsys fs, నిర్మాణం ext2_inode *inode,
				ext2_off64_t కొలత)
{
	ఒకవేళ (కొలత < 0)
		తిరిగి EINVAL;

	/* If writing a large inode, set the large_file or large_dir flag */
	ఒకవేళ (ext2fs_needs_large_file_feature(కొలత)) {
		పూర్ణాంకం dirty_sb = 0;

		ఒకవేళ (LINUX_S_ISREG(inode->i_బహుళకం)) {
			ఒకవేళ (!ext2fs_has_feature_large_file(fs->super)) {
				ext2fs_సమితి_feature_large_file(fs->super);
				dirty_sb = 1;
			}
		} లేకపోతే ఒకవేళ (LINUX_S_ISDIR(inode->i_బహుళకం)) {
			ఒకవేళ (!ext2fs_has_feature_largedir(fs->super)) {
				ext2fs_సమితి_feature_largedir(fs->super);
				dirty_sb = 1;
			}
		} లేకపోతే {
			/* Only regular files get to be larger than 4GB */
			తిరిగి EXT2_ET_FILE_TOO_BIG;
		}
		ఒకవేళ (dirty_sb) {
			ఒకవేళ (fs->super->s_rev_level == EXT2_GOOD_OLD_REV)
				ext2fs_update_dynamic_rev(fs);
			ext2fs_mark_super_dirty(fs);
		}
	}

	inode->i_కొలత = కొలత & 0xffffffff;
	inode->i_కొలత_ఎగువ = (కొలత >> 32);

	తిరిగి 0;
}

