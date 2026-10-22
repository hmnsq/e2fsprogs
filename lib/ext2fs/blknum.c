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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

/*
 * Return the group # of a block
 */
dgrp_t ext2fs_ਸਮੂਹ_ਦਾ_blk2(ext2_filsys fs, blk64_t blk)
{
	ਵਾਪਸ (blk - fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) /
		fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਵਾਪਸ fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ +
		EXT2_GROUPS_ਮੰਜ਼ਿਲ_ਖੰਡਾਂ(fs->super, ਸਮੂਹ);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਵਾਪਸ (ਸਮੂਹ == fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1 ?
		ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super) - 1 :
		ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(fs, ਸਮੂਹ) +
			(fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ - 1));
}

/*
 * Return the number of blocks in a group
 */
ਪੂਰਨ ext2fs_ਸਮੂਹ_ਖੰਡਾਂ_ਗਿਣਤੀ(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਪੂਰਨ ਅੰਕ_ਖੰਡਾਂ;

	ਜੇ (ਸਮੂਹ == fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1) {
		ਅੰਕ_ਖੰਡਾਂ = (ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super) -
				fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) %
			      fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
		ਜੇ (!ਅੰਕ_ਖੰਡਾਂ)
			ਅੰਕ_ਖੰਡਾਂ = fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
	} ਨਹੀਂਤਾਂ
		ਅੰਕ_ਖੰਡਾਂ = fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;

	ਵਾਪਸ ਅੰਕ_ਖੰਡਾਂ;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_ਅੰਕੜੇ_blocks2(ext2_filsys fs,
					ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ (inode->i_ਖੰਡਾਂ |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_ਖੰਡਾਂ_hi << 32 : 0)) -
		(inode->i_file_acl ? EXT2_CLUSTER_ਮਾਪ(fs->super) >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_ਖੰਡਾਂ(ext2_filsys fs,
					ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ (inode->i_ਖੰਡਾਂ |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_ਖੰਡਾਂ_hi << 32 : 0));
}

/*
 * Return the inode i_blocks in stat (512 byte) units
 */
blk64_t ext2fs_get_stat_i_ਖੰਡਾਂ(ext2_filsys fs,
				 ਬਣਤਰ ext2_inode *inode)
{
	blk64_t	ret = inode->i_ਖੰਡਾਂ;

	ਜੇ (ext2fs_has_feature_huge_file(fs->super)) {
		ret += ((ਲੰਮਾ ਲੰਮਾ) inode->osd2.linux2.l_i_ਖੰਡਾਂ_hi) << 32;
		ਜੇ (inode->i_flags & EXT4_HUGE_FILE_FL)
			ret *= (fs->blocksize / 512);
	}
	ਵਾਪਸ ret;
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(ਬਣਤਰ ext2_super_ਖੰਡ *super)
{
	ਵਾਪਸ super->s_ਖੰਡਾਂ_ਗਿਣਤੀ |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_ਖੰਡਾਂ_ਗਿਣਤੀ_hi << 32 : 0);
}

/*
 * Set the fs block count
 */
ਖਾਲੀ ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(ਬਣਤਰ ext2_super_ਖੰਡ *super, blk64_t blk)
{
	super->s_ਖੰਡਾਂ_ਗਿਣਤੀ = blk;
	ਜੇ (ext2fs_has_feature_64bit(super))
		super->s_ਖੰਡਾਂ_ਗਿਣਤੀ_hi = (__u64) blk >> 32;
}

/*
 * Add to the current fs block count
 */
ਖਾਲੀ ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(ਬਣਤਰ ext2_super_ਖੰਡ *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(super) + blk;
	ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(super, tmp);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ(ਬਣਤਰ ext2_super_ਖੰਡ *super)
{
	ਵਾਪਸ super->s_r_ਖੰਡਾਂ_ਗਿਣਤੀ |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_ਖੰਡਾਂ_ਗਿਣਤੀ_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
ਖਾਲੀ ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(ਬਣਤਰ ext2_super_ਖੰਡ *super, blk64_t blk)
{
	super->s_r_ਖੰਡਾਂ_ਗਿਣਤੀ = blk;
	ਜੇ (ext2fs_has_feature_64bit(super))
		super->s_r_ਖੰਡਾਂ_ਗਿਣਤੀ_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
ਖਾਲੀ ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(ਬਣਤਰ ext2_super_ਖੰਡ *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ(super) + blk;
	ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ(ਬਣਤਰ ext2_super_ਖੰਡ *super)
{
	ਵਾਪਸ super->s_free_ਖੰਡਾਂ_ਗਿਣਤੀ |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_ਖੰਡਾਂ_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
ਖਾਲੀ ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(ਬਣਤਰ ext2_super_ਖੰਡ *super, blk64_t blk)
{
	super->s_free_ਖੰਡਾਂ_ਗਿਣਤੀ = blk;
	ਜੇ (ext2fs_has_feature_64bit(super))
		super->s_free_ਖੰਡਾਂ_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
ਖਾਲੀ ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(ਬਣਤਰ ext2_super_ਖੰਡ *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ(super) + blk;
	ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
ਬਣਤਰ ext2_ਸਮੂਹ_desc *ext2fs_ਸਮੂਹ_desc(ext2_filsys fs,
					  ਬਣਤਰ opaque_ext2_ਸਮੂਹ_desc *gdp,
					  dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext2_ਸਮੂਹ_desc *ret_gdp;
	errcode_t	retval;
	ਸਥਿਰ ਅੱਖਰ	*buf = 0;
	ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ	bufsize = 0;
	blk64_t		blk;
	ਪੂਰਨ		desc_ਮਾਪ = EXT2_DESC_ਮਾਪ(fs->super) & ~7;
	ਪੂਰਨ		desc_ਪ੍ਰਤੀ_blk = EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(fs->super);

	ਜੇ (ਸਮੂਹ > fs->ਸਮੂਹ_desc_ਗਿਣਤੀ)
		ਵਾਪਸ ਨਿਰਾ;
	ਜੇ (gdp)
		ਵਾਪਸ (ਬਣਤਰ ext2_ਸਮੂਹ_desc *)((ਅੱਖਰ *)gdp +
						  ਸਮੂਹ * desc_ਮਾਪ);
	/*
	 * If fs->group_desc wasn't read in when the file system was
	 * opened, then read it on demand here.
	 */
	ਜੇ (bufsize < fs->blocksize)
		ext2fs_free_mem(&buf);
	ਜੇ (!buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ਜੇ (retval)
			ਵਾਪਸ ਨਿਰਾ;
		bufsize = fs->blocksize;
	}
	blk = ext2fs_descriptor_ਖੰਡ_loc2(fs, fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ,
					   ਸਮੂਹ / desc_ਪ੍ਰਤੀ_blk);
	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	ਜੇ (retval)
		ਵਾਪਸ ਨਿਰਾ;
	ret_gdp = (ਬਣਤਰ ext2_ਸਮੂਹ_desc *)
		(buf + ((ਸਮੂਹ % desc_ਪ੍ਰਤੀ_blk) * desc_ਮਾਪ));
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(fs, ret_gdp);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ ret_gdp;
}

/* Do the same but as an ext4 group desc for internal use here */
ਸਥਿਰ ਬਣਤਰ ext4_ਸਮੂਹ_desc *ext4fs_ਸਮੂਹ_desc(ext2_filsys fs,
					  ਬਣਤਰ opaque_ext2_ਸਮੂਹ_desc *gdp,
					  dgrp_t ਸਮੂਹ)
{
	ਵਾਪਸ (ਬਣਤਰ ext4_ਸਮੂਹ_desc *)ext2fs_ਸਮੂਹ_desc(fs, gdp, ਸਮੂਹ);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_ਖੰਡ_bitmap_checksum(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;
	__u32 csum;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	csum = gdp->bg_ਖੰਡ_bitmap_csum_lo;
	ਜੇ (EXT2_DESC_ਮਾਪ(fs->super) >= EXT4_BG_ਖੰਡ_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_ਖੰਡ_bitmap_csum_hi << 16);
	ਵਾਪਸ csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_ਖੰਡ_bitmap_loc(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	ਵਾਪਸ gdp->bg_ਖੰਡ_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_ਖੰਡ_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
ਖਾਲੀ ext2fs_ਖੰਡ_bitmap_loc_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, blk64_t blk)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_ਖੰਡ_bitmap = blk;
	ਜੇ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_ਖੰਡ_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap checksum of a group
 */
__u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;
	__u32 csum;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	csum = gdp->bg_inode_bitmap_csum_lo;
	ਜੇ (EXT2_DESC_ਮਾਪ(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_ਸਿਰਾ)
		csum |= ((__u32)gdp->bg_inode_bitmap_csum_hi << 16);
	ਵਾਪਸ csum;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	ਵਾਪਸ gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

/*
 * Set the inode bitmap block of a group
 */
ਖਾਲੀ ext2fs_inode_bitmap_loc_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, blk64_t blk)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_inode_bitmap = blk;
	ਜੇ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_ਸਾਰਣੀ_loc(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	ਵਾਪਸ gdp->bg_inode_ਸਾਰਣੀ |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_ਸਾਰਣੀ_hi << 32 : 0);
}

/*
 * Set the inode table block of a group
 */
ਖਾਲੀ ext2fs_inode_ਸਾਰਣੀ_loc_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, blk64_t blk)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_inode_ਸਾਰਣੀ = blk;
	ਜੇ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_ਸਾਰਣੀ_hi = (__u64) blk >> 32;
}

/*
 * Return the free blocks count of a group
 */
__u32 ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	ਵਾਪਸ gdp->bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_hi << 16 : 0);
}

/*
 * Set the free blocks count of a group
 */
ਖਾਲੀ ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u32 n)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ = n;

	ਜੇ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_hi = (__u32) n >> 16;
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_ਗਿਣਤੀ(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	ਵਾਪਸ gdp->bg_free_inodes_ਗਿਣਤੀ |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_ਗਿਣਤੀ_hi << 16 : 0);
}

/*
 * Set the free inodes count of a group
 */
ਖਾਲੀ ext2fs_bg_free_inodes_ਗਿਣਤੀ_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u32 n)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_free_inodes_ਗਿਣਤੀ = n;
	ਜੇ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_inodes_ਗਿਣਤੀ_hi = (__u32) n >> 16;
}

/*
 * Return the used dirs count of a group
 */
__u32 ext2fs_bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	ਵਾਪਸ gdp->bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ_hi << 16 : 0);
}

/*
 * Set the used dirs count of a group
 */
ਖਾਲੀ ext2fs_bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u32 n)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ = n;
	ਜੇ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ_hi = (__u32) n >> 16;
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_ਅਣਵਰਤਿਆ(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	ਵਾਪਸ gdp->bg_itable_ਅਣਵਰਤਿਆ |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_ਅਣਵਰਤਿਆ_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
ਖਾਲੀ ext2fs_bg_itable_ਅਣਵਰਤਿਆ_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u32 n)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_itable_ਅਣਵਰਤਿਆ = n;
	ਜੇ (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_ਅਣਵਰਤਿਆ_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	ਵਾਪਸ gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
ਖਾਲੀ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_flags = 0;
	ਵਾਪਸ;
}

/*
 * Get the value of a particular flag for this block group
 */
ਪੂਰਨ ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u16 bg_flag)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	ਵਾਪਸ gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
ਖਾਲੀ ext2fs_bg_flags_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u16 bg_flags)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_flags |= bg_flags;
	ਵਾਪਸ;
}

/*
 * Clear a flag or set of flags for this block group
 */
ਖਾਲੀ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u16 bg_flags)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_flags &= ~bg_flags;
	ਵਾਪਸ;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	ਵਾਪਸ gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
ਖਾਲੀ ext2fs_bg_checksum_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ, __u16 checksum)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp;

	gdp = ext4fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	gdp->bg_checksum = checksum;
	ਵਾਪਸ;
}

/*
 * Get the acl block of a file
 */
blk64_t ext2fs_file_acl_ਖੰਡ(ext2_filsys fs, ਅਚਲ ਬਣਤਰ ext2_inode *inode)
{
	blk64_t	blk = inode->i_file_acl;

	ਜੇ (fs && ext2fs_has_feature_64bit(fs->super))
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_ਉੱਚਾ) << 32;
	ਵਾਪਸ blk;
}

/*
 * Set the acl block of a file
 */
ਖਾਲੀ ext2fs_file_acl_ਖੰਡ_ਸੈੱਟ(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	ਜੇ (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_ਉੱਚਾ = (__u64) blk >> 32;
}

/*
 * Set the size of the inode
 */
errcode_t ext2fs_inode_ਮਾਪ_ਸੈੱਟ(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode,
				ext2_off64_t ਮਾਪ)
{
	ਜੇ (ਮਾਪ < 0)
		ਵਾਪਸ EINVAL;

	/* If writing a large inode, set the large_file or large_dir flag */
	ਜੇ (ext2fs_needs_large_file_feature(ਮਾਪ)) {
		ਪੂਰਨ dirty_sb = 0;

		ਜੇ (LINUX_S_ISREG(inode->i_ਬਹੁਲਕ)) {
			ਜੇ (!ext2fs_has_feature_large_file(fs->super)) {
				ext2fs_ਸੈੱਟ_feature_large_file(fs->super);
				dirty_sb = 1;
			}
		} ਨਹੀਂਤਾਂ ਜੇ (LINUX_S_ISDIR(inode->i_ਬਹੁਲਕ)) {
			ਜੇ (!ext2fs_has_feature_largedir(fs->super)) {
				ext2fs_ਸੈੱਟ_feature_largedir(fs->super);
				dirty_sb = 1;
			}
		} ਨਹੀਂਤਾਂ {
			/* Only regular files get to be larger than 4GB */
			ਵਾਪਸ EXT2_ET_FILE_TOO_BIG;
		}
		ਜੇ (dirty_sb) {
			ਜੇ (fs->super->s_rev_level == EXT2_GOOD_OLD_REV)
				ext2fs_update_dynamic_rev(fs);
			ext2fs_mark_super_dirty(fs);
		}
	}

	inode->i_ਮਾਪ = ਮਾਪ & 0xffffffff;
	inode->i_ਮਾਪ_ਉੱਚਾ = (ਮਾਪ >> 32);

	ਵਾਪਸ 0;
}

