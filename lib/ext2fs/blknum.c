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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব "ext2fs.h"

/*
 * Return the group # of a block
 */
dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t blk)
{
	ফেরত (blk - fs->super->s_first_তথ্য_block) /
		fs->super->s_blocks_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group)
{
	ফেরত fs->super->s_first_তথ্য_block +
		EXT2_GROUPS_TO_BLOCKS(fs->super, group);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group)
{
	ফেরত (group == fs->group_desc_গুনতি - 1 ?
		ext2fs_blocks_গুনতি(fs->super) - 1 :
		ext2fs_group_first_block2(fs, group) +
			(fs->super->s_blocks_per_group - 1));
}

/*
 * Return the number of blocks in a group
 */
পূর্ণসংখ্যা ext2fs_group_blocks_গুনতি(ext2_filsys fs, dgrp_t group)
{
	পূর্ণসংখ্যা অঙ্ক_blocks;

	যদি (group == fs->group_desc_গুনতি - 1) {
		অঙ্ক_blocks = (ext2fs_blocks_গুনতি(fs->super) -
				fs->super->s_first_তথ্য_block) %
			      fs->super->s_blocks_per_group;
		যদি (!অঙ্ক_blocks)
			অঙ্ক_blocks = fs->super->s_blocks_per_group;
	} অন্যথা
		অঙ্ক_blocks = fs->super->s_blocks_per_group;

	ফেরত অঙ্ক_blocks;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_তথ্য_blocks2(ext2_filsys fs,
					কাঠামো ext2_inode *inode)
{
	ফেরত (inode->i_blocks |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_blocks_hi << 32 : 0)) -
		(inode->i_file_acl ? EXT2_CLUSTER_আকার(fs->super) >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_blocks(ext2_filsys fs,
					কাঠামো ext2_inode *inode)
{
	ফেরত (inode->i_blocks |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_blocks_hi << 32 : 0));
}

/*
 * Return the inode i_blocks in stat (512 byte) units
 */
blk64_t ext2fs_get_stat_i_blocks(ext2_filsys fs,
				 কাঠামো ext2_inode *inode)
{
	blk64_t	ret = inode->i_blocks;

	যদি (ext2fs_has_feature_huge_file(fs->super)) {
		ret += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;
		যদি (inode->i_flags & EXT4_HUGE_FILE_FL)
			ret *= (fs->blocksize / 512);
	}
	ফেরত ret;
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_blocks_গুনতি(কাঠামো ext2_super_block *super)
{
	ফেরত super->s_blocks_গুনতি |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_blocks_গুনতি_hi << 32 : 0);
}

/*
 * Set the fs block count
 */
শূন্য ext2fs_blocks_গুনতি_set(কাঠামো ext2_super_block *super, blk64_t blk)
{
	super->s_blocks_গুনতি = blk;
	যদি (ext2fs_has_feature_64bit(super))
		super->s_blocks_গুনতি_hi = (__u64) blk >> 32;
}

/*
 * Add to the current fs block count
 */
শূন্য ext2fs_blocks_গুনতি_add(কাঠামো ext2_super_block *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_blocks_গুনতি(super) + blk;
	ext2fs_blocks_গুনতি_set(super, tmp);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_blocks_গুনতি(কাঠামো ext2_super_block *super)
{
	ফেরত super->s_r_blocks_গুনতি |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_blocks_গুনতি_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
শূন্য ext2fs_r_blocks_গুনতি_set(কাঠামো ext2_super_block *super, blk64_t blk)
{
	super->s_r_blocks_গুনতি = blk;
	যদি (ext2fs_has_feature_64bit(super))
		super->s_r_blocks_গুনতি_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
শূন্য ext2fs_r_blocks_গুনতি_add(কাঠামো ext2_super_block *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_blocks_গুনতি(super) + blk;
	ext2fs_r_blocks_গুনতি_set(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_blocks_গুনতি(কাঠামো ext2_super_block *super)
{
	ফেরত super->s_free_blocks_গুনতি |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_blocks_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
শূন্য ext2fs_free_blocks_গুনতি_set(কাঠামো ext2_super_block *super, blk64_t blk)
{
	super->s_free_blocks_গুনতি = blk;
	যদি (ext2fs_has_feature_64bit(super))
		super->s_free_blocks_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
শূন্য ext2fs_free_blocks_গুনতি_add(কাঠামো ext2_super_block *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_blocks_গুনতি(super) + blk;
	ext2fs_free_blocks_গুনতি_set(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
কাঠামো ext2_group_desc *ext2fs_group_desc(ext2_filsys fs,
					  কাঠামো opaque_ext2_group_desc *gdp,
					  dgrp_t group)
{
	কাঠামো ext2_group_desc *ret_gdp;
	errcode_t	retval;
	স্থির অক্ষর	*buf = 0;
	স্থির unsigned	bufsize = 0;
	blk64_t		blk;
	পূর্ণসংখ্যা		desc_আকার = EXT2_DESC_আকার(fs->super) & ~7;
	পূর্ণসংখ্যা		desc_per_blk = EXT2_DESC_PER_BLOCK(fs->super);

	যদি (group > fs->group_desc_গুনতি)
		ফেরত ফাঁকা;
	যদি (gdp)
		ফেরত (কাঠামো ext2_group_desc *)((অক্ষর *)gdp +
						  group * desc_আকার);
	/*
	 * If fs->group_desc wasn't read in when the file system was
	 * opened, then read it on demand here.
	 */
	যদি (bufsize < fs->blocksize)
		ext2fs_free_mem(&buf);
	যদি (!buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		যদি (retval)
			ফেরত ফাঁকা;
		bufsize = fs->blocksize;
	}
	blk = ext2fs_descriptor_block_loc2(fs, fs->super->s_first_তথ্য_block,
					   group / desc_per_blk);
	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	যদি (retval)
		ফেরত ফাঁকা;
	ret_gdp = (কাঠামো ext2_group_desc *)
		(buf + ((group % desc_per_blk) * desc_আকার));
#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_swap_group_desc2(fs, ret_gdp);
#endif
	ফেরত ret_gdp;
}

/* Do the same but as an ext4 group desc for internal use here */
স্থির কাঠামো ext4_group_desc *ext4fs_group_desc(ext2_filsys fs,
					  কাঠামো opaque_ext2_group_desc *gdp,
					  dgrp_t group)
{
	ফেরত (কাঠামো ext4_group_desc *)ext2fs_group_desc(fs, gdp, group);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;
	__u32 csum;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	csum = gdp->bg_block_bitmap_csum_lo;
	যদি (EXT2_DESC_আকার(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_block_bitmap_csum_hi << 16);
	ফেরত csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_block_bitmap_loc(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	ফেরত gdp->bg_block_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_block_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
শূন্য ext2fs_block_bitmap_loc_set(ext2_filsys fs, dgrp_t group, blk64_t blk)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_block_bitmap = blk;
	যদি (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_block_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap checksum of a group
 */
__u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;
	__u32 csum;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	csum = gdp->bg_inode_bitmap_csum_lo;
	যদি (EXT2_DESC_আকার(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		csum |= ((__u32)gdp->bg_inode_bitmap_csum_hi << 16);
	ফেরত csum;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	ফেরত gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

/*
 * Set the inode bitmap block of a group
 */
শূন্য ext2fs_inode_bitmap_loc_set(ext2_filsys fs, dgrp_t group, blk64_t blk)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_inode_bitmap = blk;
	যদি (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_table_loc(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	ফেরত gdp->bg_inode_table |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_table_hi << 32 : 0);
}

/*
 * Set the inode table block of a group
 */
শূন্য ext2fs_inode_table_loc_set(ext2_filsys fs, dgrp_t group, blk64_t blk)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_inode_table = blk;
	যদি (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_table_hi = (__u64) blk >> 32;
}

/*
 * Return the free blocks count of a group
 */
__u32 ext2fs_bg_free_blocks_গুনতি(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	ফেরত gdp->bg_free_blocks_গুনতি |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_blocks_গুনতি_hi << 16 : 0);
}

/*
 * Set the free blocks count of a group
 */
শূন্য ext2fs_bg_free_blocks_গুনতি_set(ext2_filsys fs, dgrp_t group, __u32 n)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_free_blocks_গুনতি = n;

	যদি (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_blocks_গুনতি_hi = (__u32) n >> 16;
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_গুনতি(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	ফেরত gdp->bg_free_inodes_গুনতি |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_গুনতি_hi << 16 : 0);
}

/*
 * Set the free inodes count of a group
 */
শূন্য ext2fs_bg_free_inodes_গুনতি_set(ext2_filsys fs, dgrp_t group, __u32 n)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_free_inodes_গুনতি = n;
	যদি (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_inodes_গুনতি_hi = (__u32) n >> 16;
}

/*
 * Return the used dirs count of a group
 */
__u32 ext2fs_bg_used_dirs_গুনতি(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	ফেরত gdp->bg_used_dirs_গুনতি |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_used_dirs_গুনতি_hi << 16 : 0);
}

/*
 * Set the used dirs count of a group
 */
শূন্য ext2fs_bg_used_dirs_গুনতি_set(ext2_filsys fs, dgrp_t group, __u32 n)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_used_dirs_গুনতি = n;
	যদি (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_used_dirs_গুনতি_hi = (__u32) n >> 16;
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_unused(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	ফেরত gdp->bg_itable_unused |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_unused_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
শূন্য ext2fs_bg_itable_unused_set(ext2_filsys fs, dgrp_t group, __u32 n)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_itable_unused = n;
	যদি (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_unused_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	ফেরত gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
শূন্য ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_flags = 0;
	ফেরত;
}

/*
 * Get the value of a particular flag for this block group
 */
পূর্ণসংখ্যা ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t group, __u16 bg_flag)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	ফেরত gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
শূন্য ext2fs_bg_flags_set(ext2_filsys fs, dgrp_t group, __u16 bg_flags)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_flags |= bg_flags;
	ফেরত;
}

/*
 * Clear a flag or set of flags for this block group
 */
শূন্য ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t group, __u16 bg_flags)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_flags &= ~bg_flags;
	ফেরত;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	ফেরত gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
শূন্য ext2fs_bg_checksum_set(ext2_filsys fs, dgrp_t group, __u16 checksum)
{
	কাঠামো ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_checksum = checksum;
	ফেরত;
}

/*
 * Get the acl block of a file
 */
blk64_t ext2fs_file_acl_block(ext2_filsys fs, ধ্রুবক কাঠামো ext2_inode *inode)
{
	blk64_t	blk = inode->i_file_acl;

	যদি (fs && ext2fs_has_feature_64bit(fs->super))
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_উপরের) << 32;
	ফেরত blk;
}

/*
 * Set the acl block of a file
 */
শূন্য ext2fs_file_acl_block_set(ext2_filsys fs, কাঠামো ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	যদি (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_উপরের = (__u64) blk >> 32;
}

/*
 * Set the size of the inode
 */
errcode_t ext2fs_inode_আকার_set(ext2_filsys fs, কাঠামো ext2_inode *inode,
				ext2_off64_t আকার_2)
{
	যদি (আকার_2 < 0)
		ফেরত EINVAL;

	/* If writing a large inode, set the large_file or large_dir flag */
	যদি (ext2fs_needs_large_file_feature(আকার_2)) {
		পূর্ণসংখ্যা dirty_sb = 0;

		যদি (LINUX_S_ISREG(inode->i_mode)) {
			যদি (!ext2fs_has_feature_large_file(fs->super)) {
				ext2fs_set_feature_large_file(fs->super);
				dirty_sb = 1;
			}
		} অন্যথা যদি (LINUX_S_ISDIR(inode->i_mode)) {
			যদি (!ext2fs_has_feature_largedir(fs->super)) {
				ext2fs_set_feature_largedir(fs->super);
				dirty_sb = 1;
			}
		} অন্যথা {
			/* Only regular files get to be larger than 4GB */
			ফেরত EXT2_ET_FILE_TOO_BIG;
		}
		যদি (dirty_sb) {
			যদি (fs->super->s_rev_level == EXT2_GOOD_OLD_REV)
				ext2fs_update_dynamic_rev(fs);
			ext2fs_mark_super_dirty(fs);
		}
	}

	inode->i_আকার = আকার_2 & 0xffffffff;
	inode->i_আকার_উপরের = (আকার_2 >> 32);

	ফেরত 0;
}

