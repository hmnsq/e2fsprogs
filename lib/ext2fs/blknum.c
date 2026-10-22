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

#شامل "config.h"
#شامل "ext2fs.h"

/*
 * Return the group # of a block
 */
dgrp_t ext2fs_گروہ_کا_blk2(ext2_filsys fs, blk64_t blk)
{
	واپس (blk - fs->super->s_پہلا_ڈیٹا_بلاک) /
		fs->super->s_بلاکیں_فی_گروہ;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_گروہ_پہلا_block2(ext2_filsys fs, dgrp_t گروہ)
{
	واپس fs->super->s_پہلا_ڈیٹا_بلاک +
		EXT2_GROUPS_مقصد_بلاکیں(fs->super, گروہ);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_گروہ_آخری_block2(ext2_filsys fs, dgrp_t گروہ)
{
	واپس (گروہ == fs->گروہ_desc_گنتی - 1 ?
		ext2fs_بلاکیں_گنتی(fs->super) - 1 :
		ext2fs_گروہ_پہلا_block2(fs, گروہ) +
			(fs->super->s_بلاکیں_فی_گروہ - 1));
}

/*
 * Return the number of blocks in a group
 */
صحیح ext2fs_گروہ_بلاکیں_گنتی(ext2_filsys fs, dgrp_t گروہ)
{
	صحیح عدد_بلاکیں;

	اگر (گروہ == fs->گروہ_desc_گنتی - 1) {
		عدد_بلاکیں = (ext2fs_بلاکیں_گنتی(fs->super) -
				fs->super->s_پہلا_ڈیٹا_بلاک) %
			      fs->super->s_بلاکیں_فی_گروہ;
		اگر (!عدد_بلاکیں)
			عدد_بلاکیں = fs->super->s_بلاکیں_فی_گروہ;
	} ورنہ
		عدد_بلاکیں = fs->super->s_بلاکیں_فی_گروہ;

	واپس عدد_بلاکیں;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_ڈیٹا_blocks2(ext2_filsys fs,
					ساخت ext2_inode *inode)
{
	واپس (inode->i_بلاکیں |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_بلاکیں_hi << 32 : 0)) -
		(inode->i_file_acl ? EXT2_CLUSTER_سائز(fs->super) >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_بلاکیں(ext2_filsys fs,
					ساخت ext2_inode *inode)
{
	واپس (inode->i_بلاکیں |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_بلاکیں_hi << 32 : 0));
}

/*
 * Return the inode i_blocks in stat (512 byte) units
 */
blk64_t ext2fs_get_stat_i_بلاکیں(ext2_filsys fs,
				 ساخت ext2_inode *inode)
{
	blk64_t	ret = inode->i_بلاکیں;

	اگر (ext2fs_has_feature_huge_file(fs->super)) {
		ret += ((طویل طویل) inode->osd2.linux2.l_i_بلاکیں_hi) << 32;
		اگر (inode->i_flags & EXT4_HUGE_FILE_FL)
			ret *= (fs->blocksize / 512);
	}
	واپس ret;
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_بلاکیں_گنتی(ساخت ext2_super_بلاک *super)
{
	واپس super->s_بلاکیں_گنتی |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_بلاکیں_گنتی_hi << 32 : 0);
}

/*
 * Set the fs block count
 */
خالی ext2fs_بلاکیں_گنتی_سیٹ(ساخت ext2_super_بلاک *super, blk64_t blk)
{
	super->s_بلاکیں_گنتی = blk;
	اگر (ext2fs_has_feature_64bit(super))
		super->s_بلاکیں_گنتی_hi = (__u64) blk >> 32;
}

/*
 * Add to the current fs block count
 */
خالی ext2fs_بلاکیں_گنتی_جوڑو(ساخت ext2_super_بلاک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_بلاکیں_گنتی(super) + blk;
	ext2fs_بلاکیں_گنتی_سیٹ(super, tmp);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_بلاکیں_گنتی(ساخت ext2_super_بلاک *super)
{
	واپس super->s_r_بلاکیں_گنتی |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_بلاکیں_گنتی_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
خالی ext2fs_r_بلاکیں_گنتی_سیٹ(ساخت ext2_super_بلاک *super, blk64_t blk)
{
	super->s_r_بلاکیں_گنتی = blk;
	اگر (ext2fs_has_feature_64bit(super))
		super->s_r_بلاکیں_گنتی_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
خالی ext2fs_r_بلاکیں_گنتی_جوڑو(ساخت ext2_super_بلاک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_بلاکیں_گنتی(super) + blk;
	ext2fs_r_بلاکیں_گنتی_سیٹ(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_بلاکیں_گنتی(ساخت ext2_super_بلاک *super)
{
	واپس super->s_free_بلاکیں_گنتی |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_بلاکیں_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
خالی ext2fs_free_بلاکیں_گنتی_سیٹ(ساخت ext2_super_بلاک *super, blk64_t blk)
{
	super->s_free_بلاکیں_گنتی = blk;
	اگر (ext2fs_has_feature_64bit(super))
		super->s_free_بلاکیں_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
خالی ext2fs_free_بلاکیں_گنتی_جوڑو(ساخت ext2_super_بلاک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_بلاکیں_گنتی(super) + blk;
	ext2fs_free_بلاکیں_گنتی_سیٹ(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
ساخت ext2_گروہ_desc *ext2fs_گروہ_desc(ext2_filsys fs,
					  ساخت opaque_ext2_گروہ_desc *gdp,
					  dgrp_t گروہ)
{
	ساخت ext2_گروہ_desc *ret_gdp;
	errcode_t	retval;
	ساکن حرف	*buf = 0;
	ساکن غیرعلامتی	bufsize = 0;
	blk64_t		blk;
	صحیح		desc_سائز = EXT2_DESC_سائز(fs->super) & ~7;
	صحیح		desc_فی_blk = EXT2_DESC_فی_بلاک(fs->super);

	اگر (گروہ > fs->گروہ_desc_گنتی)
		واپس تہی;
	اگر (gdp)
		واپس (ساخت ext2_گروہ_desc *)((حرف *)gdp +
						  گروہ * desc_سائز);
	/*
	 * If fs->group_desc wasn't read in when the file system was
	 * opened, then read it on demand here.
	 */
	اگر (bufsize < fs->blocksize)
		ext2fs_free_mem(&buf);
	اگر (!buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		اگر (retval)
			واپس تہی;
		bufsize = fs->blocksize;
	}
	blk = ext2fs_descriptor_بلاک_loc2(fs, fs->super->s_پہلا_ڈیٹا_بلاک,
					   گروہ / desc_فی_blk);
	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	اگر (retval)
		واپس تہی;
	ret_gdp = (ساخت ext2_گروہ_desc *)
		(buf + ((گروہ % desc_فی_blk) * desc_سائز));
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_تبادلہ_گروہ_desc2(fs, ret_gdp);
#اگر_ختم
	واپس ret_gdp;
}

/* Do the same but as an ext4 group desc for internal use here */
ساکن ساخت ext4_گروہ_desc *ext4fs_گروہ_desc(ext2_filsys fs,
					  ساخت opaque_ext2_گروہ_desc *gdp,
					  dgrp_t گروہ)
{
	واپس (ساخت ext4_گروہ_desc *)ext2fs_گروہ_desc(fs, gdp, گروہ);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_بلاک_bitmap_checksum(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;
	__u32 csum;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	csum = gdp->bg_بلاک_bitmap_csum_lo;
	اگر (EXT2_DESC_سائز(fs->super) >= EXT4_BG_بلاک_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_بلاک_bitmap_csum_hi << 16);
	واپس csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_بلاک_bitmap_loc(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	واپس gdp->bg_بلاک_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_بلاک_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
خالی ext2fs_بلاک_bitmap_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ, blk64_t blk)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_بلاک_bitmap = blk;
	اگر (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_بلاک_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap checksum of a group
 */
__u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;
	__u32 csum;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	csum = gdp->bg_inode_bitmap_csum_lo;
	اگر (EXT2_DESC_سائز(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_انتہا)
		csum |= ((__u32)gdp->bg_inode_bitmap_csum_hi << 16);
	واپس csum;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	واپس gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

/*
 * Set the inode bitmap block of a group
 */
خالی ext2fs_inode_bitmap_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ, blk64_t blk)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_inode_bitmap = blk;
	اگر (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_جدول_loc(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	واپس gdp->bg_inode_جدول |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_جدول_hi << 32 : 0);
}

/*
 * Set the inode table block of a group
 */
خالی ext2fs_inode_جدول_loc_سیٹ(ext2_filsys fs, dgrp_t گروہ, blk64_t blk)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_inode_جدول = blk;
	اگر (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_جدول_hi = (__u64) blk >> 32;
}

/*
 * Return the free blocks count of a group
 */
__u32 ext2fs_bg_free_بلاکیں_گنتی(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	واپس gdp->bg_free_بلاکیں_گنتی |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_بلاکیں_گنتی_hi << 16 : 0);
}

/*
 * Set the free blocks count of a group
 */
خالی ext2fs_bg_free_بلاکیں_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u32 n)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_free_بلاکیں_گنتی = n;

	اگر (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_بلاکیں_گنتی_hi = (__u32) n >> 16;
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_گنتی(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	واپس gdp->bg_free_inodes_گنتی |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_گنتی_hi << 16 : 0);
}

/*
 * Set the free inodes count of a group
 */
خالی ext2fs_bg_free_inodes_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u32 n)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_free_inodes_گنتی = n;
	اگر (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_inodes_گنتی_hi = (__u32) n >> 16;
}

/*
 * Return the used dirs count of a group
 */
__u32 ext2fs_bg_مستعمل_dirs_گنتی(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	واپس gdp->bg_مستعمل_dirs_گنتی |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_مستعمل_dirs_گنتی_hi << 16 : 0);
}

/*
 * Set the used dirs count of a group
 */
خالی ext2fs_bg_مستعمل_dirs_گنتی_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u32 n)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_مستعمل_dirs_گنتی = n;
	اگر (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_مستعمل_dirs_گنتی_hi = (__u32) n >> 16;
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_غیر_مستعمل(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	واپس gdp->bg_itable_غیر_مستعمل |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_غیر_مستعمل_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
خالی ext2fs_bg_itable_غیر_مستعمل_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u32 n)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_itable_غیر_مستعمل = n;
	اگر (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_غیر_مستعمل_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	واپس gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
خالی ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_flags = 0;
	واپس;
}

/*
 * Get the value of a particular flag for this block group
 */
صحیح ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flag)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	واپس gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
خالی ext2fs_bg_flags_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flags)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_flags |= bg_flags;
	واپس;
}

/*
 * Clear a flag or set of flags for this block group
 */
خالی ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t گروہ, __u16 bg_flags)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_flags &= ~bg_flags;
	واپس;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	واپس gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
خالی ext2fs_bg_checksum_سیٹ(ext2_filsys fs, dgrp_t گروہ, __u16 checksum)
{
	ساخت ext4_گروہ_desc *gdp;

	gdp = ext4fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	gdp->bg_checksum = checksum;
	واپس;
}

/*
 * Get the acl block of a file
 */
blk64_t ext2fs_file_acl_بلاک(ext2_filsys fs, ثابت ساخت ext2_inode *inode)
{
	blk64_t	blk = inode->i_file_acl;

	اگر (fs && ext2fs_has_feature_64bit(fs->super))
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_بلند) << 32;
	واپس blk;
}

/*
 * Set the acl block of a file
 */
خالی ext2fs_file_acl_بلاک_سیٹ(ext2_filsys fs, ساخت ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	اگر (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_بلند = (__u64) blk >> 32;
}

/*
 * Set the size of the inode
 */
errcode_t ext2fs_inode_سائز_سیٹ(ext2_filsys fs, ساخت ext2_inode *inode,
				ext2_off64_t سائز)
{
	اگر (سائز < 0)
		واپس EINVAL;

	/* If writing a large inode, set the large_file or large_dir flag */
	اگر (ext2fs_needs_large_file_feature(سائز)) {
		صحیح dirty_sb = 0;

		اگر (LINUX_S_ISREG(inode->i_منوال)) {
			اگر (!ext2fs_has_feature_large_file(fs->super)) {
				ext2fs_سیٹ_feature_large_file(fs->super);
				dirty_sb = 1;
			}
		} ورنہ اگر (LINUX_S_ISDIR(inode->i_منوال)) {
			اگر (!ext2fs_has_feature_largedir(fs->super)) {
				ext2fs_سیٹ_feature_largedir(fs->super);
				dirty_sb = 1;
			}
		} ورنہ {
			/* Only regular files get to be larger than 4GB */
			واپس EXT2_ET_FILE_TOO_BIG;
		}
		اگر (dirty_sb) {
			اگر (fs->super->s_rev_level == EXT2_GOOD_OLD_REV)
				ext2fs_update_dynamic_rev(fs);
			ext2fs_mark_super_dirty(fs);
		}
	}

	inode->i_سائز = سائز & 0xffffffff;
	inode->i_سائز_بلند = (سائز >> 32);

	واپس 0;
}

