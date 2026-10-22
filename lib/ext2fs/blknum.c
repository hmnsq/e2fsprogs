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

#ضمن "config.h"
#ضمن "ext2fs.h"

/*
 * Return the group # of a block
 */
dgrp_t ext2fs_ډله_د_blk2(ext2_filsys fs, blk64_t blk)
{
	أرجع (blk - fs->super->s_لومړی_معلومات_بلاک) /
		fs->super->s_بلاکونه_فی_ډله;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_ډله_لومړی_block2(ext2_filsys fs, dgrp_t ډله)
{
	أرجع fs->super->s_لومړی_معلومات_بلاک +
		EXT2_GROUPS_منزل_بلاکونه(fs->super, ډله);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_ډله_وروستی_block2(ext2_filsys fs, dgrp_t ډله)
{
	أرجع (ډله == fs->ډله_desc_شمېر - 1 ?
		ext2fs_بلاکونه_شمېر(fs->super) - 1 :
		ext2fs_ډله_لومړی_block2(fs, ډله) +
			(fs->super->s_بلاکونه_فی_ډله - 1));
}

/*
 * Return the number of blocks in a group
 */
صحيح ext2fs_ډله_بلاکونه_شمېر(ext2_filsys fs, dgrp_t ډله)
{
	صحيح عدد_بلاکونه;

	إذا (ډله == fs->ډله_desc_شمېر - 1) {
		عدد_بلاکونه = (ext2fs_بلاکونه_شمېر(fs->super) -
				fs->super->s_لومړی_معلومات_بلاک) %
			      fs->super->s_بلاکونه_فی_ډله;
		إذا (!عدد_بلاکونه)
			عدد_بلاکونه = fs->super->s_بلاکونه_فی_ډله;
	} وإلا
		عدد_بلاکونه = fs->super->s_بلاکونه_فی_ډله;

	أرجع عدد_بلاکونه;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_معلومات_blocks2(ext2_filsys fs,
					بنية ext2_inode *inode)
{
	أرجع (inode->i_بلاکونه |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_بلاکونه_hi << 32 : 0)) -
		(inode->i_file_acl ? EXT2_CLUSTER_اندازه(fs->super) >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_بلاکونه(ext2_filsys fs,
					بنية ext2_inode *inode)
{
	أرجع (inode->i_بلاکونه |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_بلاکونه_hi << 32 : 0));
}

/*
 * Return the inode i_blocks in stat (512 byte) units
 */
blk64_t ext2fs_get_stat_i_بلاکونه(ext2_filsys fs,
				 بنية ext2_inode *inode)
{
	blk64_t	ret = inode->i_بلاکونه;

	إذا (ext2fs_has_feature_huge_file(fs->super)) {
		ret += ((اوږد اوږد) inode->osd2.linux2.l_i_بلاکونه_hi) << 32;
		إذا (inode->i_flags & EXT4_HUGE_FILE_FL)
			ret *= (fs->blocksize / 512);
	}
	أرجع ret;
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_بلاکونه_شمېر(بنية ext2_super_بلاک *super)
{
	أرجع super->s_بلاکونه_شمېر |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_بلاکونه_شمېر_hi << 32 : 0);
}

/*
 * Set the fs block count
 */
فراغ ext2fs_بلاکونه_شمېر_سېټ(بنية ext2_super_بلاک *super, blk64_t blk)
{
	super->s_بلاکونه_شمېر = blk;
	إذا (ext2fs_has_feature_64bit(super))
		super->s_بلاکونه_شمېر_hi = (__u64) blk >> 32;
}

/*
 * Add to the current fs block count
 */
فراغ ext2fs_بلاکونه_شمېر_جمع_کړه(بنية ext2_super_بلاک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_بلاکونه_شمېر(super) + blk;
	ext2fs_بلاکونه_شمېر_سېټ(super, tmp);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_بلاکونه_شمېر(بنية ext2_super_بلاک *super)
{
	أرجع super->s_r_بلاکونه_شمېر |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_بلاکونه_شمېر_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
فراغ ext2fs_r_بلاکونه_شمېر_سېټ(بنية ext2_super_بلاک *super, blk64_t blk)
{
	super->s_r_بلاکونه_شمېر = blk;
	إذا (ext2fs_has_feature_64bit(super))
		super->s_r_بلاکونه_شمېر_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
فراغ ext2fs_r_بلاکونه_شمېر_جمع_کړه(بنية ext2_super_بلاک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_بلاکونه_شمېر(super) + blk;
	ext2fs_r_بلاکونه_شمېر_سېټ(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_بلاکونه_شمېر(بنية ext2_super_بلاک *super)
{
	أرجع super->s_free_بلاکونه_شمېر |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_بلاکونه_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
فراغ ext2fs_free_بلاکونه_شمېر_سېټ(بنية ext2_super_بلاک *super, blk64_t blk)
{
	super->s_free_بلاکونه_شمېر = blk;
	إذا (ext2fs_has_feature_64bit(super))
		super->s_free_بلاکونه_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
فراغ ext2fs_free_بلاکونه_شمېر_جمع_کړه(بنية ext2_super_بلاک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_بلاکونه_شمېر(super) + blk;
	ext2fs_free_بلاکونه_شمېر_سېټ(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
بنية ext2_ډله_desc *ext2fs_ډله_desc(ext2_filsys fs,
					  بنية opaque_ext2_ډله_desc *gdp,
					  dgrp_t ډله)
{
	بنية ext2_ډله_desc *ret_gdp;
	errcode_t	retval;
	ساكن حرف	*buf = 0;
	ساكن بینښه	bufsize = 0;
	blk64_t		blk;
	صحيح		desc_اندازه = EXT2_DESC_اندازه(fs->super) & ~7;
	صحيح		desc_فی_blk = EXT2_DESC_فی_بلاک(fs->super);

	إذا (ډله > fs->ډله_desc_شمېر)
		أرجع عدم;
	إذا (gdp)
		أرجع (بنية ext2_ډله_desc *)((حرف *)gdp +
						  ډله * desc_اندازه);
	/*
	 * If fs->group_desc wasn't read in when the file system was
	 * opened, then read it on demand here.
	 */
	إذا (bufsize < fs->blocksize)
		ext2fs_free_mem(&buf);
	إذا (!buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		إذا (retval)
			أرجع عدم;
		bufsize = fs->blocksize;
	}
	blk = ext2fs_descriptor_بلاک_loc2(fs, fs->super->s_لومړی_معلومات_بلاک,
					   ډله / desc_فی_blk);
	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	إذا (retval)
		أرجع عدم;
	ret_gdp = (بنية ext2_ډله_desc *)
		(buf + ((ډله % desc_فی_blk) * desc_اندازه));
#که_تعریف_شوی ټکي_BIGENDIAN
	ext2fs_بدلون_ډله_desc2(fs, ret_gdp);
#د_که_پای
	أرجع ret_gdp;
}

/* Do the same but as an ext4 group desc for internal use here */
ساكن بنية ext4_ډله_desc *ext4fs_ډله_desc(ext2_filsys fs,
					  بنية opaque_ext2_ډله_desc *gdp,
					  dgrp_t ډله)
{
	أرجع (بنية ext4_ډله_desc *)ext2fs_ډله_desc(fs, gdp, ډله);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_بلاک_bitmap_checksum(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;
	__u32 csum;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	csum = gdp->bg_بلاک_bitmap_csum_lo;
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_بلاک_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_بلاک_bitmap_csum_hi << 16);
	أرجع csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_بلاک_bitmap_loc(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	أرجع gdp->bg_بلاک_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_بلاک_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
فراغ ext2fs_بلاک_bitmap_loc_سېټ(ext2_filsys fs, dgrp_t ډله, blk64_t blk)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_بلاک_bitmap = blk;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_بلاک_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap checksum of a group
 */
__u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;
	__u32 csum;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	csum = gdp->bg_inode_bitmap_csum_lo;
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_پای_ټکی)
		csum |= ((__u32)gdp->bg_inode_bitmap_csum_hi << 16);
	أرجع csum;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	أرجع gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

/*
 * Set the inode bitmap block of a group
 */
فراغ ext2fs_inode_bitmap_loc_سېټ(ext2_filsys fs, dgrp_t ډله, blk64_t blk)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_inode_bitmap = blk;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_جدول_loc(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	أرجع gdp->bg_inode_جدول |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_جدول_hi << 32 : 0);
}

/*
 * Set the inode table block of a group
 */
فراغ ext2fs_inode_جدول_loc_سېټ(ext2_filsys fs, dgrp_t ډله, blk64_t blk)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_inode_جدول = blk;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_جدول_hi = (__u64) blk >> 32;
}

/*
 * Return the free blocks count of a group
 */
__u32 ext2fs_bg_free_بلاکونه_شمېر(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	أرجع gdp->bg_free_بلاکونه_شمېر |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_بلاکونه_شمېر_hi << 16 : 0);
}

/*
 * Set the free blocks count of a group
 */
فراغ ext2fs_bg_free_بلاکونه_شمېر_سېټ(ext2_filsys fs, dgrp_t ډله, __u32 n)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_free_بلاکونه_شمېر = n;

	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_بلاکونه_شمېر_hi = (__u32) n >> 16;
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_شمېر(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	أرجع gdp->bg_free_inodes_شمېر |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_شمېر_hi << 16 : 0);
}

/*
 * Set the free inodes count of a group
 */
فراغ ext2fs_bg_free_inodes_شمېر_سېټ(ext2_filsys fs, dgrp_t ډله, __u32 n)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_free_inodes_شمېر = n;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_inodes_شمېر_hi = (__u32) n >> 16;
}

/*
 * Return the used dirs count of a group
 */
__u32 ext2fs_bg_کارول_شوی_dirs_شمېر(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	أرجع gdp->bg_کارول_شوی_dirs_شمېر |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_کارول_شوی_dirs_شمېر_hi << 16 : 0);
}

/*
 * Set the used dirs count of a group
 */
فراغ ext2fs_bg_کارول_شوی_dirs_شمېر_سېټ(ext2_filsys fs, dgrp_t ډله, __u32 n)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_کارول_شوی_dirs_شمېر = n;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_کارول_شوی_dirs_شمېر_hi = (__u32) n >> 16;
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_ناکارول_شوی(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	أرجع gdp->bg_itable_ناکارول_شوی |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_ناکارول_شوی_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
فراغ ext2fs_bg_itable_ناکارول_شوی_سېټ(ext2_filsys fs, dgrp_t ډله, __u32 n)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_itable_ناکارول_شوی = n;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_ناکارول_شوی_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	أرجع gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
فراغ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_flags = 0;
	أرجع;
}

/*
 * Get the value of a particular flag for this block group
 */
صحيح ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t ډله, __u16 bg_flag)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	أرجع gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
فراغ ext2fs_bg_flags_سېټ(ext2_filsys fs, dgrp_t ډله, __u16 bg_flags)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_flags |= bg_flags;
	أرجع;
}

/*
 * Clear a flag or set of flags for this block group
 */
فراغ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t ډله, __u16 bg_flags)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_flags &= ~bg_flags;
	أرجع;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	أرجع gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
فراغ ext2fs_bg_checksum_سېټ(ext2_filsys fs, dgrp_t ډله, __u16 checksum)
{
	بنية ext4_ډله_desc *gdp;

	gdp = ext4fs_ډله_desc(fs, fs->ډله_desc, ډله);
	gdp->bg_checksum = checksum;
	أرجع;
}

/*
 * Get the acl block of a file
 */
blk64_t ext2fs_file_acl_بلاک(ext2_filsys fs, ثابت بنية ext2_inode *inode)
{
	blk64_t	blk = inode->i_file_acl;

	إذا (fs && ext2fs_has_feature_64bit(fs->super))
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_لوړ) << 32;
	أرجع blk;
}

/*
 * Set the acl block of a file
 */
فراغ ext2fs_file_acl_بلاک_سېټ(ext2_filsys fs, بنية ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	إذا (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_لوړ = (__u64) blk >> 32;
}

/*
 * Set the size of the inode
 */
errcode_t ext2fs_inode_اندازه_سېټ(ext2_filsys fs, بنية ext2_inode *inode,
				ext2_off64_t اندازه)
{
	إذا (اندازه < 0)
		أرجع EINVAL;

	/* If writing a large inode, set the large_file or large_dir flag */
	إذا (ext2fs_needs_large_file_feature(اندازه)) {
		صحيح dirty_sb = 0;

		إذا (LINUX_S_ISREG(inode->i_ډېرپېښ)) {
			إذا (!ext2fs_has_feature_large_file(fs->super)) {
				ext2fs_سېټ_feature_large_file(fs->super);
				dirty_sb = 1;
			}
		} وإلا إذا (LINUX_S_ISDIR(inode->i_ډېرپېښ)) {
			إذا (!ext2fs_has_feature_largedir(fs->super)) {
				ext2fs_سېټ_feature_largedir(fs->super);
				dirty_sb = 1;
			}
		} وإلا {
			/* Only regular files get to be larger than 4GB */
			أرجع EXT2_ET_FILE_TOO_BIG;
		}
		إذا (dirty_sb) {
			إذا (fs->super->s_rev_level == EXT2_GOOD_OLD_REV)
				ext2fs_update_dynamic_rev(fs);
			ext2fs_mark_super_dirty(fs);
		}
	}

	inode->i_اندازه = اندازه & 0xffffffff;
	inode->i_اندازه_لوړ = (اندازه >> 32);

	أرجع 0;
}

