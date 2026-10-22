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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * Return the group # of a block
 */
dgrp_t ext2fs_ಗುಂಪು_ದ_blk2(ext2_filsys fs, blk64_t blk)
{
	ಹಿಂತಿರುಗಿ (blk - fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) /
		fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_ಗುಂಪು_ಮೊದಲ_block2(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ಹಿಂತಿರುಗಿ fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ +
		EXT2_GROUPS_ಗಮ್ಯ_ಖಂಡಗಳು(fs->super, ಗುಂಪು);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ಹಿಂತಿರುಗಿ (ಗುಂಪು == fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1 ?
		ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super) - 1 :
		ext2fs_ಗುಂಪು_ಮೊದಲ_block2(fs, ಗುಂಪು) +
			(fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು - 1));
}

/*
 * Return the number of blocks in a group
 */
ಪೂರ್ಣಾಂಕ ext2fs_ಗುಂಪು_ಖಂಡಗಳು_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಖಂಡಗಳು;

	ಆದರೆ (ಗುಂಪು == fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1) {
		ಅಂಕೆ_ಖಂಡಗಳು = (ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super) -
				fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) %
			      fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
		ಆದರೆ (!ಅಂಕೆ_ಖಂಡಗಳು)
			ಅಂಕೆ_ಖಂಡಗಳು = fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಅಂಕೆ_ಖಂಡಗಳು = fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;

	ಹಿಂತಿರುಗಿ ಅಂಕೆ_ಖಂಡಗಳು;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_ದತ್ತಾಂಶ_blocks2(ext2_filsys fs,
					ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ (inode->i_ಖಂಡಗಳು |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_ಖಂಡಗಳು_hi << 32 : 0)) -
		(inode->i_file_acl ? EXT2_CLUSTER_ಗಾತ್ರ(fs->super) >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_ಖಂಡಗಳು(ext2_filsys fs,
					ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ (inode->i_ಖಂಡಗಳು |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_ಖಂಡಗಳು_hi << 32 : 0));
}

/*
 * Return the inode i_blocks in stat (512 byte) units
 */
blk64_t ext2fs_get_stat_i_ಖಂಡಗಳು(ext2_filsys fs,
				 ರಚನೆ ext2_inode *inode)
{
	blk64_t	ret = inode->i_ಖಂಡಗಳು;

	ಆದರೆ (ext2fs_has_feature_huge_file(fs->super)) {
		ret += ((ದೀರ್ಘ ದೀರ್ಘ) inode->osd2.linux2.l_i_ಖಂಡಗಳು_hi) << 32;
		ಆದರೆ (inode->i_flags & EXT4_HUGE_FILE_FL)
			ret *= (fs->blocksize / 512);
	}
	ಹಿಂತಿರುಗಿ ret;
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(ರಚನೆ ext2_super_ಖಂಡ *super)
{
	ಹಿಂತಿರುಗಿ super->s_ಖಂಡಗಳು_ಎಣಿಕೆ |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_ಖಂಡಗಳು_ಎಣಿಕೆ_hi << 32 : 0);
}

/*
 * Set the fs block count
 */
ಶೂನ್ಯ ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(ರಚನೆ ext2_super_ಖಂಡ *super, blk64_t blk)
{
	super->s_ಖಂಡಗಳು_ಎಣಿಕೆ = blk;
	ಆದರೆ (ext2fs_has_feature_64bit(super))
		super->s_ಖಂಡಗಳು_ಎಣಿಕೆ_hi = (__u64) blk >> 32;
}

/*
 * Add to the current fs block count
 */
ಶೂನ್ಯ ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(ರಚನೆ ext2_super_ಖಂಡ *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(super) + blk;
	ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(super, tmp);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ(ರಚನೆ ext2_super_ಖಂಡ *super)
{
	ಹಿಂತಿರುಗಿ super->s_r_ಖಂಡಗಳು_ಎಣಿಕೆ |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_ಖಂಡಗಳು_ಎಣಿಕೆ_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
ಶೂನ್ಯ ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(ರಚನೆ ext2_super_ಖಂಡ *super, blk64_t blk)
{
	super->s_r_ಖಂಡಗಳು_ಎಣಿಕೆ = blk;
	ಆದರೆ (ext2fs_has_feature_64bit(super))
		super->s_r_ಖಂಡಗಳು_ಎಣಿಕೆ_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
ಶೂನ್ಯ ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(ರಚನೆ ext2_super_ಖಂಡ *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ(super) + blk;
	ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ(ರಚನೆ ext2_super_ಖಂಡ *super)
{
	ಹಿಂತಿರುಗಿ super->s_free_ಖಂಡಗಳು_ಎಣಿಕೆ |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_ಖಂಡಗಳು_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
ಶೂನ್ಯ ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(ರಚನೆ ext2_super_ಖಂಡ *super, blk64_t blk)
{
	super->s_free_ಖಂಡಗಳು_ಎಣಿಕೆ = blk;
	ಆದರೆ (ext2fs_has_feature_64bit(super))
		super->s_free_ಖಂಡಗಳು_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
ಶೂನ್ಯ ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(ರಚನೆ ext2_super_ಖಂಡ *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ(super) + blk;
	ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
ರಚನೆ ext2_ಗುಂಪು_desc *ext2fs_ಗುಂಪು_desc(ext2_filsys fs,
					  ರಚನೆ opaque_ext2_ಗುಂಪು_desc *gdp,
					  dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext2_ಗುಂಪು_desc *ret_gdp;
	errcode_t	retval;
	ಸ್ಥಿರ ಅಕ್ಷರ	*buf = 0;
	ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ	bufsize = 0;
	blk64_t		blk;
	ಪೂರ್ಣಾಂಕ		desc_ಗಾತ್ರ = EXT2_DESC_ಗಾತ್ರ(fs->super) & ~7;
	ಪೂರ್ಣಾಂಕ		desc_ಪ್ರತಿ_blk = EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(fs->super);

	ಆದರೆ (ಗುಂಪು > fs->ಗುಂಪು_desc_ಎಣಿಕೆ)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಆದರೆ (gdp)
		ಹಿಂತಿರುಗಿ (ರಚನೆ ext2_ಗುಂಪು_desc *)((ಅಕ್ಷರ *)gdp +
						  ಗುಂಪು * desc_ಗಾತ್ರ);
	/*
	 * If fs->group_desc wasn't read in when the file system was
	 * opened, then read it on demand here.
	 */
	ಆದರೆ (bufsize < fs->blocksize)
		ext2fs_free_mem(&buf);
	ಆದರೆ (!buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		bufsize = fs->blocksize;
	}
	blk = ext2fs_descriptor_ಖಂಡ_loc2(fs, fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ,
					   ಗುಂಪು / desc_ಪ್ರತಿ_blk);
	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ret_gdp = (ರಚನೆ ext2_ಗುಂಪು_desc *)
		(buf + ((ಗುಂಪು % desc_ಪ್ರತಿ_blk) * desc_ಗಾತ್ರ));
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(fs, ret_gdp);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ ret_gdp;
}

/* Do the same but as an ext4 group desc for internal use here */
ಸ್ಥಿರ ರಚನೆ ext4_ಗುಂಪು_desc *ext4fs_ಗುಂಪು_desc(ext2_filsys fs,
					  ರಚನೆ opaque_ext2_ಗುಂಪು_desc *gdp,
					  dgrp_t ಗುಂಪು)
{
	ಹಿಂತಿರುಗಿ (ರಚನೆ ext4_ಗುಂಪು_desc *)ext2fs_ಗುಂಪು_desc(fs, gdp, ಗುಂಪು);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_ಖಂಡ_bitmap_checksum(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;
	__u32 csum;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	csum = gdp->bg_ಖಂಡ_bitmap_csum_lo;
	ಆದರೆ (EXT2_DESC_ಗಾತ್ರ(fs->super) >= EXT4_BG_ಖಂಡ_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_ಖಂಡ_bitmap_csum_hi << 16);
	ಹಿಂತಿರುಗಿ csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_ಖಂಡ_bitmap_loc(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ gdp->bg_ಖಂಡ_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_ಖಂಡ_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
ಶೂನ್ಯ ext2fs_ಖಂಡ_bitmap_loc_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, blk64_t blk)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_ಖಂಡ_bitmap = blk;
	ಆದರೆ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_ಖಂಡ_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap checksum of a group
 */
__u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;
	__u32 csum;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	csum = gdp->bg_inode_bitmap_csum_lo;
	ಆದರೆ (EXT2_DESC_ಗಾತ್ರ(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_ಕೊನೆ)
		csum |= ((__u32)gdp->bg_inode_bitmap_csum_hi << 16);
	ಹಿಂತಿರುಗಿ csum;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

/*
 * Set the inode bitmap block of a group
 */
ಶೂನ್ಯ ext2fs_inode_bitmap_loc_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, blk64_t blk)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_inode_bitmap = blk;
	ಆದರೆ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_ಕೋಷ್ಟಕ_loc(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ gdp->bg_inode_ಕೋಷ್ಟಕ |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_ಕೋಷ್ಟಕ_hi << 32 : 0);
}

/*
 * Set the inode table block of a group
 */
ಶೂನ್ಯ ext2fs_inode_ಕೋಷ್ಟಕ_loc_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, blk64_t blk)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_inode_ಕೋಷ್ಟಕ = blk;
	ಆದರೆ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_ಕೋಷ್ಟಕ_hi = (__u64) blk >> 32;
}

/*
 * Return the free blocks count of a group
 */
__u32 ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ gdp->bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_hi << 16 : 0);
}

/*
 * Set the free blocks count of a group
 */
ಶೂನ್ಯ ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, __u32 n)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ = n;

	ಆದರೆ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_hi = (__u32) n >> 16;
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ gdp->bg_free_inodes_ಎಣಿಕೆ |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_ಎಣಿಕೆ_hi << 16 : 0);
}

/*
 * Set the free inodes count of a group
 */
ಶೂನ್ಯ ext2fs_bg_free_inodes_ಎಣಿಕೆ_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, __u32 n)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_free_inodes_ಎಣಿಕೆ = n;
	ಆದರೆ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_inodes_ಎಣಿಕೆ_hi = (__u32) n >> 16;
}

/*
 * Return the used dirs count of a group
 */
__u32 ext2fs_bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ gdp->bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ_hi << 16 : 0);
}

/*
 * Set the used dirs count of a group
 */
ಶೂನ್ಯ ext2fs_bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, __u32 n)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ = n;
	ಆದರೆ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ_hi = (__u32) n >> 16;
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_ಬಳಸದ(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ gdp->bg_itable_ಬಳಸದ |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_ಬಳಸದ_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
ಶೂನ್ಯ ext2fs_bg_itable_ಬಳಸದ_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, __u32 n)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_itable_ಬಳಸದ = n;
	ಆದರೆ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_ಬಳಸದ_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
ಶೂನ್ಯ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_flags = 0;
	ಹಿಂತಿರುಗಿ;
}

/*
 * Get the value of a particular flag for this block group
 */
ಪೂರ್ಣಾಂಕ ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t ಗುಂಪು, __u16 bg_flag)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
ಶೂನ್ಯ ext2fs_bg_flags_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, __u16 bg_flags)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_flags |= bg_flags;
	ಹಿಂತಿರುಗಿ;
}

/*
 * Clear a flag or set of flags for this block group
 */
ಶೂನ್ಯ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t ಗುಂಪು, __u16 bg_flags)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_flags &= ~bg_flags;
	ಹಿಂತಿರುಗಿ;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
ಶೂನ್ಯ ext2fs_bg_checksum_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು, __u16 checksum)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp;

	gdp = ext4fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	gdp->bg_checksum = checksum;
	ಹಿಂತಿರುಗಿ;
}

/*
 * Get the acl block of a file
 */
blk64_t ext2fs_file_acl_ಖಂಡ(ext2_filsys fs, ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_inode *inode)
{
	blk64_t	blk = inode->i_file_acl;

	ಆದರೆ (fs && ext2fs_has_feature_64bit(fs->super))
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_ಮೇಲಿನ) << 32;
	ಹಿಂತಿರುಗಿ blk;
}

/*
 * Set the acl block of a file
 */
ಶೂನ್ಯ ext2fs_file_acl_ಖಂಡ_ಗಣ(ext2_filsys fs, ರಚನೆ ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	ಆದರೆ (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_ಮೇಲಿನ = (__u64) blk >> 32;
}

/*
 * Set the size of the inode
 */
errcode_t ext2fs_inode_ಗಾತ್ರ_ಗಣ(ext2_filsys fs, ರಚನೆ ext2_inode *inode,
				ext2_off64_t ಗಾತ್ರ_2)
{
	ಆದರೆ (ಗಾತ್ರ_2 < 0)
		ಹಿಂತಿರುಗಿ EINVAL;

	/* If writing a large inode, set the large_file or large_dir flag */
	ಆದರೆ (ext2fs_needs_large_file_feature(ಗಾತ್ರ_2)) {
		ಪೂರ್ಣಾಂಕ dirty_sb = 0;

		ಆದರೆ (LINUX_S_ISREG(inode->i_ಬಹುಲಕ)) {
			ಆದರೆ (!ext2fs_has_feature_large_file(fs->super)) {
				ext2fs_ಗಣ_feature_large_file(fs->super);
				dirty_sb = 1;
			}
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (LINUX_S_ISDIR(inode->i_ಬಹುಲಕ)) {
			ಆದರೆ (!ext2fs_has_feature_largedir(fs->super)) {
				ext2fs_ಗಣ_feature_largedir(fs->super);
				dirty_sb = 1;
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Only regular files get to be larger than 4GB */
			ಹಿಂತಿರುಗಿ EXT2_ET_FILE_TOO_BIG;
		}
		ಆದರೆ (dirty_sb) {
			ಆದರೆ (fs->super->s_rev_level == EXT2_GOOD_OLD_REV)
				ext2fs_update_dynamic_rev(fs);
			ext2fs_mark_super_dirty(fs);
		}
	}

	inode->i_ಗಾತ್ರ = ಗಾತ್ರ_2 & 0xffffffff;
	inode->i_ಗಾತ್ರ_ಮೇಲಿನ = (ಗಾತ್ರ_2 >> 32);

	ಹಿಂತಿರುಗಿ 0;
}

