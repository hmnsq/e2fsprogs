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
dgrp_t ext2fs_گروہ_دا_blk2(ext2_filsys fs, blk64_t blk)
{
	أرجع (blk - fs->super->s_پہلا_انکڑے_بلاک) /
		fs->super->s_بلاکاں_فی_گروہ;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_گروہ_پہلا_block2(ext2_filsys fs, dgrp_t گروہ)
{
	أرجع fs->super->s_پہلا_انکڑے_بلاک +
		EXT2_GROUPS_منزل_بلاکاں(fs->super, گروہ);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_گروہ_آخری_block2(ext2_filsys fs, dgrp_t گروہ)
{
	أرجع (گروہ == fs->گروہ_desc_گنتی - 1 ?
		ext2fs_بلاکاں_گنتی(fs->super) - 1 :
		ext2fs_گروہ_پہلا_block2(fs, گروہ) +
			(fs->super->s_بلاکاں_فی_گروہ - 1));
}

/*
 * Return the number of blocks in a group
 */
صحيح ext2fs_گروہ_بلاکاں_گنتی(ext2_filsys fs, dgrp_t گروہ)
{
	صحيح عدد_بلاکاں;

	إذا (گروہ == fs->گروہ_desc_گنتی - 1) {
		عدد_بلاکاں = (ext2fs_بلاکاں_گنتی(fs->super) -
				fs->super->s_پہلا_انکڑے_بلاک) %
			      fs->super->s_بلاکاں_فی_گروہ;
		إذا (!عدد_بلاکاں)
			عدد_بلاکاں = fs->super->s_بلاکاں_فی_گروہ;
	} وإلا
		عدد_بلاکاں = fs->super->s_بلاکاں_فی_گروہ;

	أرجع عدد_بلاکاں;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_انکڑے_blocks2(ext2_filsys fs,
					بنية ext2_inode *inode)
{
	أرجع (inode->i_بلاکاں |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_بلاکاں_hi << 32 : 0)) -
		(inode->i_file_acl ? EXT2_CLUSTER_ناپ(fs->super) >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_بلاکاں(ext2_filsys fs,
					بنية ext2_inode *inode)
{
	أرجع (inode->i_بلاکاں |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_بلاکاں_hi << 32 : 0));
}

/*
 * Return the inode i_blocks in stat (512 byte) units
 */
blk64_t ext2fs_get_stat_i_بلاکاں(ext2_filsys fs,
				 بنية ext2_inode *inode)
{
	blk64_t	ret = inode->i_بلاکاں;

	إذا (ext2fs_has_feature_huge_file(fs->super)) {
		ret += ((لما لما) inode->osd2.linux2.l_i_بلاکاں_hi) << 32;
		إذا (inode->i_flags & EXT4_HUGE_FILE_FL)
			ret *= (fs->blocksize / 512);
	}
	أرجع ret;
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_بلاکاں_گنتی(بنية ext2_super_بلاک *super)
{
	أرجع super->s_بلاکاں_گنتی |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_بلاکاں_گنتی_hi << 32 : 0);
}

/*
 * Set the fs block count
 */
فراغ ext2fs_بلاکاں_گنتی_سیٹ(بنية ext2_super_بلاک *super, blk64_t blk)
{
	super->s_بلاکاں_گنتی = blk;
	إذا (ext2fs_has_feature_64bit(super))
		super->s_بلاکاں_گنتی_hi = (__u64) blk >> 32;
}

/*
 * Add to the current fs block count
 */
فراغ ext2fs_بلاکاں_گنتی_جوڑو(بنية ext2_super_بلاک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_بلاکاں_گنتی(super) + blk;
	ext2fs_بلاکاں_گنتی_سیٹ(super, tmp);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_بلاکاں_گنتی(بنية ext2_super_بلاک *super)
{
	أرجع super->s_r_بلاکاں_گنتی |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_بلاکاں_گنتی_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
فراغ ext2fs_r_بلاکاں_گنتی_سیٹ(بنية ext2_super_بلاک *super, blk64_t blk)
{
	super->s_r_بلاکاں_گنتی = blk;
	إذا (ext2fs_has_feature_64bit(super))
		super->s_r_بلاکاں_گنتی_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
فراغ ext2fs_r_بلاکاں_گنتی_جوڑو(بنية ext2_super_بلاک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_بلاکاں_گنتی(super) + blk;
	ext2fs_r_بلاکاں_گنتی_سیٹ(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_بلاکاں_گنتی(بنية ext2_super_بلاک *super)
{
	أرجع super->s_free_بلاکاں_گنتی |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_بلاکاں_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
فراغ ext2fs_free_بلاکاں_گنتی_سیٹ(بنية ext2_super_بلاک *super, blk64_t blk)
{
	super->s_free_بلاکاں_گنتی = blk;
	إذا (ext2fs_has_feature_64bit(super))
		super->s_free_بلاکاں_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
فراغ ext2fs_free_بلاکاں_گنتی_جوڑو(بنية ext2_super_بلاک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_بلاکاں_گنتی(super) + blk;
	ext2fs_free_بلاکاں_گنتی_سیٹ(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
بنية ext2_گروہ_desc *ext2fs_گروہ_desc(ext2_filsys fs,
					  بنية opaque_ext2_گروہ_desc *gdp,
					  dgrp_t گروہ)
{
	بنية ext2_گروہ_desc *ret_gdp;
	errcode_t	retval;
	ساكن حرف	*buf = 0;
	ساكن بنانشانی	bufsize = 0;
	blk64_t		blk;
	صحيح		desc_ناپ = EXT2_DESC_ناپ(fs->super) & ~7;
	صحيح		desc_فی_blk = EXT2_DESC_فی_بلاک(fs->super);

	إذا (گروہ > fs->گروہ_desc_گنتی)
		أرجع عدم;
	إذا (gdp)
		أرجع (بنية ext2_گروہ_desc *)((حرف *)gdp +
						  گروہ * desc_ناپ);
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
	blk = ext2fs_descriptor_بلاک_loc2(fs, fs->super->s_پہلا_انکڑے_بلاک,
					   گروہ / desc_فی_blk);
	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	إذا (retval)
		أرجع عدم;
	ret_gdp = (بنية ext2_گروہ_desc *)
		(buf + ((گروہ % desc_فی_blk) * desc_ناپ));
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_وٹاندرا_گروہ_desc2(fs, ret_gdp);
#جے_انت
	أرجع ret_gdp;
}

/* Do the same but as an ext4 group desc for internal use here */
ساكن بنية ext4_گروہ_desc *ext4fs_گروہ_desc(ext2_filsys fs,
					  بنية opaque_ext2_گروہ_desc *gdp,
					  dgrp_t گروہ)
{
	أرجع (بنية ext4_گروہ_desc *)ext2fs_گروہ_desc(fs, gdp, گروہ);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_بلاک_bitmap_checksum(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;
	__u32 csum;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	csum = gdp->bg_بلاک_bitmap_csum_lo;
	إذا (EXT2_DESC_ناپ(fs->super) >= EXT4_BG_بلاک_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_بلاک_bitmap_csum_hi << 16);
	أرجع csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_بلاک_bitmap_loc(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	أرجع gdp->bg_بلاک_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_بلاک_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
فراغ ext2fs_بلاک_bitmap_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ, blk64_t blk)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_بلاک_bitmap = blk;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_بلاک_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap checksum of a group
 */
__u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;
	__u32 csum;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	csum = gdp->bg_inode_bitmap_csum_lo;
	إذا (EXT2_DESC_ناپ(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_سرا)
		csum |= ((__u32)gdp->bg_inode_bitmap_csum_hi << 16);
	أرجع csum;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	أرجع gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

/*
 * Set the inode bitmap block of a group
 */
فراغ ext2fs_inode_bitmap_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ, blk64_t blk)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_inode_bitmap = blk;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_جدول_loc(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	أرجع gdp->bg_inode_جدول |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_جدول_hi << 32 : 0);
}

/*
 * Set the inode table block of a group
 */
فراغ ext2fs_inode_جدول_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ, blk64_t blk)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_inode_جدول = blk;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_جدول_hi = (__u64) blk >> 32;
}

/*
 * Return the free blocks count of a group
 */
__u32 ext2fs_bg_free_بلاکاں_گنتی(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	أرجع gdp->bg_free_بلاکاں_گنتی |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_بلاکاں_گنتی_hi << 16 : 0);
}

/*
 * Set the free blocks count of a group
 */
فراغ ext2fs_bg_free_بلاکاں_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u32 n)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_free_بلاکاں_گنتی = n;

	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_بلاکاں_گنتی_hi = (__u32) n >> 16;
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_گنتی(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	أرجع gdp->bg_free_inodes_گنتی |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_گنتی_hi << 16 : 0);
}

/*
 * Set the free inodes count of a group
 */
فراغ ext2fs_bg_free_inodes_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u32 n)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_free_inodes_گنتی = n;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_inodes_گنتی_hi = (__u32) n >> 16;
}

/*
 * Return the used dirs count of a group
 */
__u32 ext2fs_bg_ورتیا_dirs_گنتی(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	أرجع gdp->bg_ورتیا_dirs_گنتی |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_ورتیا_dirs_گنتی_hi << 16 : 0);
}

/*
 * Set the used dirs count of a group
 */
فراغ ext2fs_bg_ورتیا_dirs_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u32 n)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_ورتیا_dirs_گنتی = n;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_ورتیا_dirs_گنتی_hi = (__u32) n >> 16;
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_بن_ورتیا(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	أرجع gdp->bg_itable_بن_ورتیا |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_بن_ورتیا_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
فراغ ext2fs_bg_itable_بن_ورتیا_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u32 n)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_itable_بن_ورتیا = n;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_بن_ورتیا_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	أرجع gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
فراغ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_flags = 0;
	أرجع;
}

/*
 * Get the value of a particular flag for this block group
 */
صحيح ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flag)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	أرجع gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
فراغ ext2fs_bg_flags_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flags)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_flags |= bg_flags;
	أرجع;
}

/*
 * Clear a flag or set of flags for this block group
 */
فراغ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flags)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_flags &= ~bg_flags;
	أرجع;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	أرجع gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
فراغ ext2fs_bg_checksum_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u16 checksum)
{
	بنية ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
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
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_بلند) << 32;
	أرجع blk;
}

/*
 * Set the acl block of a file
 */
فراغ ext2fs_file_acl_بلاک_سیٹ(ext2_filsys fs, بنية ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	إذا (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_بلند = (__u64) blk >> 32;
}

/*
 * Set the size of the inode
 */
errcode_t ext2fs_inode_ناپ_سیٹ(ext2_filsys fs, بنية ext2_inode *inode,
				ext2_off64_t ناپ)
{
	إذا (ناپ < 0)
		أرجع EINVAL;

	/* If writing a large inode, set the large_file or large_dir flag */
	إذا (ext2fs_needs_large_file_feature(ناپ)) {
		صحيح dirty_sb = 0;

		إذا (LINUX_S_ISREG(inode->i_منوال)) {
			إذا (!ext2fs_has_feature_large_file(fs->super)) {
				ext2fs_سیٹ_feature_large_file(fs->super);
				dirty_sb = 1;
			}
		} وإلا إذا (LINUX_S_ISDIR(inode->i_منوال)) {
			إذا (!ext2fs_has_feature_largedir(fs->super)) {
				ext2fs_سیٹ_feature_largedir(fs->super);
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

	inode->i_ناپ = ناپ & 0xffffffff;
	inode->i_ناپ_بلند = (ناپ >> 32);

	أرجع 0;
}

