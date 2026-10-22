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

#अन्तर्भाव "config.h"
#अन्तर्भाव "ext2fs.h"

/*
 * Return the group # of a block
 */
dgrp_t ext2fs_group_of_blk2(ext2_filsys fs, blk64_t blk)
{
	प्रत्यागम (blk - fs->super->s_first_data_block) /
		fs->super->s_blocks_per_group;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_group_first_block2(ext2_filsys fs, dgrp_t group)
{
	प्रत्यागम fs->super->s_first_data_block +
		EXT2_GROUPS_TO_BLOCKS(fs->super, group);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_group_last_block2(ext2_filsys fs, dgrp_t group)
{
	प्रत्यागम (group == fs->group_desc_मोजणी - 1 ?
		ext2fs_blocks_मोजणी(fs->super) - 1 :
		ext2fs_group_first_block2(fs, group) +
			(fs->super->s_blocks_per_group - 1));
}

/*
 * Return the number of blocks in a group
 */
पूर्णाङ्क ext2fs_group_blocks_मोजणी(ext2_filsys fs, dgrp_t group)
{
	पूर्णाङ्क अंक_blocks;

	यदि (group == fs->group_desc_मोजणी - 1) {
		अंक_blocks = (ext2fs_blocks_मोजणी(fs->super) -
				fs->super->s_first_data_block) %
			      fs->super->s_blocks_per_group;
		यदि (!अंक_blocks)
			अंक_blocks = fs->super->s_blocks_per_group;
	} अन्यथा
		अंक_blocks = fs->super->s_blocks_per_group;

	प्रत्यागम अंक_blocks;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_data_blocks2(ext2_filsys fs,
					संरचना ext2_inode *inode)
{
	प्रत्यागम (inode->i_blocks |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_blocks_hi << 32 : 0)) -
		(inode->i_file_acl ? EXT2_CLUSTER_SIZE(fs->super) >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_blocks(ext2_filsys fs,
					संरचना ext2_inode *inode)
{
	प्रत्यागम (inode->i_blocks |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_blocks_hi << 32 : 0));
}

/*
 * Return the inode i_blocks in stat (512 byte) units
 */
blk64_t ext2fs_get_stat_i_blocks(ext2_filsys fs,
				 संरचना ext2_inode *inode)
{
	blk64_t	ret = inode->i_blocks;

	यदि (ext2fs_has_feature_huge_file(fs->super)) {
		ret += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;
		यदि (inode->i_flags & EXT4_HUGE_FILE_FL)
			ret *= (fs->blocksize / 512);
	}
	प्रत्यागम ret;
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_blocks_मोजणी(संरचना ext2_super_block *super)
{
	प्रत्यागम super->s_blocks_मोजणी |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_blocks_मोजणी_hi << 32 : 0);
}

/*
 * Set the fs block count
 */
शून्य ext2fs_blocks_मोजणी_set(संरचना ext2_super_block *super, blk64_t blk)
{
	super->s_blocks_मोजणी = blk;
	यदि (ext2fs_has_feature_64bit(super))
		super->s_blocks_मोजणी_hi = (__u64) blk >> 32;
}

/*
 * Add to the current fs block count
 */
शून्य ext2fs_blocks_मोजणी_add(संरचना ext2_super_block *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_blocks_मोजणी(super) + blk;
	ext2fs_blocks_मोजणी_set(super, tmp);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_blocks_मोजणी(संरचना ext2_super_block *super)
{
	प्रत्यागम super->s_r_blocks_मोजणी |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_blocks_मोजणी_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
शून्य ext2fs_r_blocks_मोजणी_set(संरचना ext2_super_block *super, blk64_t blk)
{
	super->s_r_blocks_मोजणी = blk;
	यदि (ext2fs_has_feature_64bit(super))
		super->s_r_blocks_मोजणी_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
शून्य ext2fs_r_blocks_मोजणी_add(संरचना ext2_super_block *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_blocks_मोजणी(super) + blk;
	ext2fs_r_blocks_मोजणी_set(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_blocks_मोजणी(संरचना ext2_super_block *super)
{
	प्रत्यागम super->s_free_blocks_मोजणी |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_blocks_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
शून्य ext2fs_free_blocks_मोजणी_set(संरचना ext2_super_block *super, blk64_t blk)
{
	super->s_free_blocks_मोजणी = blk;
	यदि (ext2fs_has_feature_64bit(super))
		super->s_free_blocks_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
शून्य ext2fs_free_blocks_मोजणी_add(संरचना ext2_super_block *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_blocks_मोजणी(super) + blk;
	ext2fs_free_blocks_मोजणी_set(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
संरचना ext2_group_desc *ext2fs_group_desc(ext2_filsys fs,
					  संरचना opaque_ext2_group_desc *gdp,
					  dgrp_t group)
{
	संरचना ext2_group_desc *ret_gdp;
	errcode_t	retval;
	स्थिर वर्ण	*buf = 0;
	स्थिर unsigned	bufsize = 0;
	blk64_t		blk;
	पूर्णाङ्क		desc_size = EXT2_DESC_SIZE(fs->super) & ~7;
	पूर्णाङ्क		desc_per_blk = EXT2_DESC_PER_BLOCK(fs->super);

	यदि (group > fs->group_desc_मोजणी)
		प्रत्यागम रिक्त;
	यदि (gdp)
		प्रत्यागम (संरचना ext2_group_desc *)((वर्ण *)gdp +
						  group * desc_size);
	/*
	 * If fs->group_desc wasn't read in when the file system was
	 * opened, then read it on demand here.
	 */
	यदि (bufsize < fs->blocksize)
		ext2fs_free_mem(&buf);
	यदि (!buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		यदि (retval)
			प्रत्यागम रिक्त;
		bufsize = fs->blocksize;
	}
	blk = ext2fs_descriptor_block_loc2(fs, fs->super->s_first_data_block,
					   group / desc_per_blk);
	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	यदि (retval)
		प्रत्यागम रिक्त;
	ret_gdp = (संरचना ext2_group_desc *)
		(buf + ((group % desc_per_blk) * desc_size));
#ifdef शब्द_BIGENDIAN
	ext2fs_swap_group_desc2(fs, ret_gdp);
#endif
	प्रत्यागम ret_gdp;
}

/* Do the same but as an ext4 group desc for internal use here */
स्थिर संरचना ext4_group_desc *ext4fs_group_desc(ext2_filsys fs,
					  संरचना opaque_ext2_group_desc *gdp,
					  dgrp_t group)
{
	प्रत्यागम (संरचना ext4_group_desc *)ext2fs_group_desc(fs, gdp, group);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_block_bitmap_checksum(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;
	__u32 csum;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	csum = gdp->bg_block_bitmap_csum_lo;
	यदि (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_block_bitmap_csum_hi << 16);
	प्रत्यागम csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_block_bitmap_loc(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	प्रत्यागम gdp->bg_block_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_block_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
शून्य ext2fs_block_bitmap_loc_set(ext2_filsys fs, dgrp_t group, blk64_t blk)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_block_bitmap = blk;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_block_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap checksum of a group
 */
__u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;
	__u32 csum;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	csum = gdp->bg_inode_bitmap_csum_lo;
	यदि (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		csum |= ((__u32)gdp->bg_inode_bitmap_csum_hi << 16);
	प्रत्यागम csum;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	प्रत्यागम gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

/*
 * Set the inode bitmap block of a group
 */
शून्य ext2fs_inode_bitmap_loc_set(ext2_filsys fs, dgrp_t group, blk64_t blk)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_inode_bitmap = blk;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_table_loc(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	प्रत्यागम gdp->bg_inode_table |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_table_hi << 32 : 0);
}

/*
 * Set the inode table block of a group
 */
शून्य ext2fs_inode_table_loc_set(ext2_filsys fs, dgrp_t group, blk64_t blk)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_inode_table = blk;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_table_hi = (__u64) blk >> 32;
}

/*
 * Return the free blocks count of a group
 */
__u32 ext2fs_bg_free_blocks_मोजणी(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	प्रत्यागम gdp->bg_free_blocks_मोजणी |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_blocks_मोजणी_hi << 16 : 0);
}

/*
 * Set the free blocks count of a group
 */
शून्य ext2fs_bg_free_blocks_मोजणी_set(ext2_filsys fs, dgrp_t group, __u32 n)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_free_blocks_मोजणी = n;

	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_blocks_मोजणी_hi = (__u32) n >> 16;
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_मोजणी(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	प्रत्यागम gdp->bg_free_inodes_मोजणी |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_मोजणी_hi << 16 : 0);
}

/*
 * Set the free inodes count of a group
 */
शून्य ext2fs_bg_free_inodes_मोजणी_set(ext2_filsys fs, dgrp_t group, __u32 n)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_free_inodes_मोजणी = n;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_inodes_मोजणी_hi = (__u32) n >> 16;
}

/*
 * Return the used dirs count of a group
 */
__u32 ext2fs_bg_used_dirs_मोजणी(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	प्रत्यागम gdp->bg_used_dirs_मोजणी |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_used_dirs_मोजणी_hi << 16 : 0);
}

/*
 * Set the used dirs count of a group
 */
शून्य ext2fs_bg_used_dirs_मोजणी_set(ext2_filsys fs, dgrp_t group, __u32 n)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_used_dirs_मोजणी = n;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_used_dirs_मोजणी_hi = (__u32) n >> 16;
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_unused(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	प्रत्यागम gdp->bg_itable_unused |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_unused_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
शून्य ext2fs_bg_itable_unused_set(ext2_filsys fs, dgrp_t group, __u32 n)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_itable_unused = n;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_unused_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	प्रत्यागम gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
शून्य ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_flags = 0;
	प्रत्यागम;
}

/*
 * Get the value of a particular flag for this block group
 */
पूर्णाङ्क ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t group, __u16 bg_flag)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	प्रत्यागम gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
शून्य ext2fs_bg_flags_set(ext2_filsys fs, dgrp_t group, __u16 bg_flags)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_flags |= bg_flags;
	प्रत्यागम;
}

/*
 * Clear a flag or set of flags for this block group
 */
शून्य ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t group, __u16 bg_flags)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_flags &= ~bg_flags;
	प्रत्यागम;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t group)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	प्रत्यागम gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
शून्य ext2fs_bg_checksum_set(ext2_filsys fs, dgrp_t group, __u16 checksum)
{
	संरचना ext4_group_desc *gdp;

	gdp = ext4fs_group_desc(fs, fs->group_desc, group);
	gdp->bg_checksum = checksum;
	प्रत्यागम;
}

/*
 * Get the acl block of a file
 */
blk64_t ext2fs_file_acl_block(ext2_filsys fs, ध्रुव संरचना ext2_inode *inode)
{
	blk64_t	blk = inode->i_file_acl;

	यदि (fs && ext2fs_has_feature_64bit(fs->super))
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_वरचा) << 32;
	प्रत्यागम blk;
}

/*
 * Set the acl block of a file
 */
शून्य ext2fs_file_acl_block_set(ext2_filsys fs, संरचना ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	यदि (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_वरचा = (__u64) blk >> 32;
}

/*
 * Set the size of the inode
 */
errcode_t ext2fs_inode_size_set(ext2_filsys fs, संरचना ext2_inode *inode,
				ext2_off64_t size)
{
	यदि (size < 0)
		प्रत्यागम EINVAL;

	/* If writing a large inode, set the large_file or large_dir flag */
	यदि (ext2fs_needs_large_file_feature(size)) {
		पूर्णाङ्क dirty_sb = 0;

		यदि (LINUX_S_ISREG(inode->i_mode)) {
			यदि (!ext2fs_has_feature_large_file(fs->super)) {
				ext2fs_set_feature_large_file(fs->super);
				dirty_sb = 1;
			}
		} अन्यथा यदि (LINUX_S_ISDIR(inode->i_mode)) {
			यदि (!ext2fs_has_feature_largedir(fs->super)) {
				ext2fs_set_feature_largedir(fs->super);
				dirty_sb = 1;
			}
		} अन्यथा {
			/* Only regular files get to be larger than 4GB */
			प्रत्यागम EXT2_ET_FILE_TOO_BIG;
		}
		यदि (dirty_sb) {
			यदि (fs->super->s_rev_level == EXT2_GOOD_OLD_REV)
				ext2fs_update_dynamic_rev(fs);
			ext2fs_mark_super_dirty(fs);
		}
	}

	inode->i_size = size & 0xffffffff;
	inode->i_size_वरचा = (size >> 32);

	प्रत्यागम 0;
}

