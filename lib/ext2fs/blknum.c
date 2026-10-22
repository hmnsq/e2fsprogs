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
dgrp_t ext2fs_گروه_از_blk2(ext2_filsys fs, blk64_t blk)
{
	أرجع (blk - fs->super->s_اولی_داده_بلوک) /
		fs->super->s_بلوکها_در_هر_گروه;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_گروه_اولی_block2(ext2_filsys fs, dgrp_t گروه)
{
	أرجع fs->super->s_اولی_داده_بلوک +
		EXT2_GROUPS_مقصد_بلوکها(fs->super, گروه);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_گروه_آخرین_block2(ext2_filsys fs, dgrp_t گروه)
{
	أرجع (گروه == fs->گروه_desc_شمار - 1 ?
		ext2fs_بلوکها_شمار(fs->super) - 1 :
		ext2fs_گروه_اولی_block2(fs, گروه) +
			(fs->super->s_بلوکها_در_هر_گروه - 1));
}

/*
 * Return the number of blocks in a group
 */
صحيح ext2fs_گروه_بلوکها_شمار(ext2_filsys fs, dgrp_t گروه)
{
	صحيح رقم_بلوکها;

	إذا (گروه == fs->گروه_desc_شمار - 1) {
		رقم_بلوکها = (ext2fs_بلوکها_شمار(fs->super) -
				fs->super->s_اولی_داده_بلوک) %
			      fs->super->s_بلوکها_در_هر_گروه;
		إذا (!رقم_بلوکها)
			رقم_بلوکها = fs->super->s_بلوکها_در_هر_گروه;
	} وإلا
		رقم_بلوکها = fs->super->s_بلوکها_در_هر_گروه;

	أرجع رقم_بلوکها;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_داده_blocks2(ext2_filsys fs,
					بنية ext2_inode *inode)
{
	أرجع (inode->i_بلوکها |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_بلوکها_hi << 32 : 0)) -
		(inode->i_file_acl ? EXT2_CLUSTER_اندازه(fs->super) >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_بلوکها(ext2_filsys fs,
					بنية ext2_inode *inode)
{
	أرجع (inode->i_بلوکها |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_بلوکها_hi << 32 : 0));
}

/*
 * Return the inode i_blocks in stat (512 byte) units
 */
blk64_t ext2fs_get_stat_i_بلوکها(ext2_filsys fs,
				 بنية ext2_inode *inode)
{
	blk64_t	ret = inode->i_بلوکها;

	إذا (ext2fs_has_feature_huge_file(fs->super)) {
		ret += ((دراز دراز) inode->osd2.linux2.l_i_بلوکها_hi) << 32;
		إذا (inode->i_flags & EXT4_HUGE_FILE_FL)
			ret *= (fs->blocksize / 512);
	}
	أرجع ret;
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_بلوکها_شمار(بنية ext2_super_بلوک *super)
{
	أرجع super->s_بلوکها_شمار |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_بلوکها_شمار_hi << 32 : 0);
}

/*
 * Set the fs block count
 */
فراغ ext2fs_بلوکها_شمار_مجموعه(بنية ext2_super_بلوک *super, blk64_t blk)
{
	super->s_بلوکها_شمار = blk;
	إذا (ext2fs_has_feature_64bit(super))
		super->s_بلوکها_شمار_hi = (__u64) blk >> 32;
}

/*
 * Add to the current fs block count
 */
فراغ ext2fs_بلوکها_شمار_جمع_کن(بنية ext2_super_بلوک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_بلوکها_شمار(super) + blk;
	ext2fs_بلوکها_شمار_مجموعه(super, tmp);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_بلوکها_شمار(بنية ext2_super_بلوک *super)
{
	أرجع super->s_r_بلوکها_شمار |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_بلوکها_شمار_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
فراغ ext2fs_r_بلوکها_شمار_مجموعه(بنية ext2_super_بلوک *super, blk64_t blk)
{
	super->s_r_بلوکها_شمار = blk;
	إذا (ext2fs_has_feature_64bit(super))
		super->s_r_بلوکها_شمار_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
فراغ ext2fs_r_بلوکها_شمار_جمع_کن(بنية ext2_super_بلوک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_بلوکها_شمار(super) + blk;
	ext2fs_r_بلوکها_شمار_مجموعه(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_بلوکها_شمار(بنية ext2_super_بلوک *super)
{
	أرجع super->s_free_بلوکها_شمار |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_بلوکها_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
فراغ ext2fs_free_بلوکها_شمار_مجموعه(بنية ext2_super_بلوک *super, blk64_t blk)
{
	super->s_free_بلوکها_شمار = blk;
	إذا (ext2fs_has_feature_64bit(super))
		super->s_free_بلوکها_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
فراغ ext2fs_free_بلوکها_شمار_جمع_کن(بنية ext2_super_بلوک *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_بلوکها_شمار(super) + blk;
	ext2fs_free_بلوکها_شمار_مجموعه(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
بنية ext2_گروه_desc *ext2fs_گروه_desc(ext2_filsys fs,
					  بنية opaque_ext2_گروه_desc *gdp,
					  dgrp_t گروه)
{
	بنية ext2_گروه_desc *ret_gdp;
	errcode_t	retval;
	ساكن حرف	*buf = 0;
	ساكن بیعلامت	bufsize = 0;
	blk64_t		blk;
	صحيح		desc_اندازه = EXT2_DESC_اندازه(fs->super) & ~7;
	صحيح		desc_در_هر_blk = EXT2_DESC_در_هر_بلوک(fs->super);

	إذا (گروه > fs->گروه_desc_شمار)
		أرجع عدم;
	إذا (gdp)
		أرجع (بنية ext2_گروه_desc *)((حرف *)gdp +
						  گروه * desc_اندازه);
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
	blk = ext2fs_descriptor_بلوک_loc2(fs, fs->super->s_اولی_داده_بلوک,
					   گروه / desc_در_هر_blk);
	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	إذا (retval)
		أرجع عدم;
	ret_gdp = (بنية ext2_گروه_desc *)
		(buf + ((گروه % desc_در_هر_blk) * desc_اندازه));
#اگر_تعریف_شده واژگان_BIGENDIAN
	ext2fs_جابجا_گروه_desc2(fs, ret_gdp);
#پایان_اگر
	أرجع ret_gdp;
}

/* Do the same but as an ext4 group desc for internal use here */
ساكن بنية ext4_گروه_desc *ext4fs_گروه_desc(ext2_filsys fs,
					  بنية opaque_ext2_گروه_desc *gdp,
					  dgrp_t گروه)
{
	أرجع (بنية ext4_گروه_desc *)ext2fs_گروه_desc(fs, gdp, گروه);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_بلوک_bitmap_checksum(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;
	__u32 csum;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	csum = gdp->bg_بلوک_bitmap_csum_lo;
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_بلوک_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_بلوک_bitmap_csum_hi << 16);
	أرجع csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_بلوک_bitmap_loc(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	أرجع gdp->bg_بلوک_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_بلوک_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
فراغ ext2fs_بلوک_bitmap_loc_مجموعه(ext2_filsys fs, dgrp_t گروه, blk64_t blk)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_بلوک_bitmap = blk;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_بلوک_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap checksum of a group
 */
__u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;
	__u32 csum;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	csum = gdp->bg_inode_bitmap_csum_lo;
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_انتها)
		csum |= ((__u32)gdp->bg_inode_bitmap_csum_hi << 16);
	أرجع csum;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	أرجع gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

/*
 * Set the inode bitmap block of a group
 */
فراغ ext2fs_inode_bitmap_loc_مجموعه(ext2_filsys fs, dgrp_t گروه, blk64_t blk)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_inode_bitmap = blk;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_جدول_loc(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	أرجع gdp->bg_inode_جدول |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_جدول_hi << 32 : 0);
}

/*
 * Set the inode table block of a group
 */
فراغ ext2fs_inode_جدول_loc_مجموعه(ext2_filsys fs, dgrp_t گروه, blk64_t blk)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_inode_جدول = blk;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_جدول_hi = (__u64) blk >> 32;
}

/*
 * Return the free blocks count of a group
 */
__u32 ext2fs_bg_free_بلوکها_شمار(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	أرجع gdp->bg_free_بلوکها_شمار |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_بلوکها_شمار_hi << 16 : 0);
}

/*
 * Set the free blocks count of a group
 */
فراغ ext2fs_bg_free_بلوکها_شمار_مجموعه(ext2_filsys fs, dgrp_t گروه, __u32 n)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_free_بلوکها_شمار = n;

	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_بلوکها_شمار_hi = (__u32) n >> 16;
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_شمار(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	أرجع gdp->bg_free_inodes_شمار |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_شمار_hi << 16 : 0);
}

/*
 * Set the free inodes count of a group
 */
فراغ ext2fs_bg_free_inodes_شمار_مجموعه(ext2_filsys fs, dgrp_t گروه, __u32 n)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_free_inodes_شمار = n;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_inodes_شمار_hi = (__u32) n >> 16;
}

/*
 * Return the used dirs count of a group
 */
__u32 ext2fs_bg_استفاده_شده_dirs_شمار(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	أرجع gdp->bg_استفاده_شده_dirs_شمار |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_استفاده_شده_dirs_شمار_hi << 16 : 0);
}

/*
 * Set the used dirs count of a group
 */
فراغ ext2fs_bg_استفاده_شده_dirs_شمار_مجموعه(ext2_filsys fs, dgrp_t گروه, __u32 n)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_استفاده_شده_dirs_شمار = n;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_استفاده_شده_dirs_شمار_hi = (__u32) n >> 16;
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_استفادهنشده(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	أرجع gdp->bg_itable_استفادهنشده |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_استفادهنشده_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
فراغ ext2fs_bg_itable_استفادهنشده_مجموعه(ext2_filsys fs, dgrp_t گروه, __u32 n)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_itable_استفادهنشده = n;
	إذا (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_استفادهنشده_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	أرجع gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
فراغ ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_flags = 0;
	أرجع;
}

/*
 * Get the value of a particular flag for this block group
 */
صحيح ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t گروه, __u16 bg_flag)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	أرجع gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
فراغ ext2fs_bg_flags_مجموعه(ext2_filsys fs, dgrp_t گروه, __u16 bg_flags)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_flags |= bg_flags;
	أرجع;
}

/*
 * Clear a flag or set of flags for this block group
 */
فراغ ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t گروه, __u16 bg_flags)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_flags &= ~bg_flags;
	أرجع;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	أرجع gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
فراغ ext2fs_bg_checksum_مجموعه(ext2_filsys fs, dgrp_t گروه, __u16 checksum)
{
	بنية ext4_گروه_desc *gdp;

	gdp = ext4fs_گروه_desc(fs, fs->گروه_desc, گروه);
	gdp->bg_checksum = checksum;
	أرجع;
}

/*
 * Get the acl block of a file
 */
blk64_t ext2fs_file_acl_بلوک(ext2_filsys fs, ثابت بنية ext2_inode *inode)
{
	blk64_t	blk = inode->i_file_acl;

	إذا (fs && ext2fs_has_feature_64bit(fs->super))
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_بالا) << 32;
	أرجع blk;
}

/*
 * Set the acl block of a file
 */
فراغ ext2fs_file_acl_بلوک_مجموعه(ext2_filsys fs, بنية ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	إذا (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_بالا = (__u64) blk >> 32;
}

/*
 * Set the size of the inode
 */
errcode_t ext2fs_inode_اندازه_مجموعه(ext2_filsys fs, بنية ext2_inode *inode,
				ext2_off64_t اندازه)
{
	إذا (اندازه < 0)
		أرجع EINVAL;

	/* If writing a large inode, set the large_file or large_dir flag */
	إذا (ext2fs_needs_large_file_feature(اندازه)) {
		صحيح dirty_sb = 0;

		إذا (LINUX_S_ISREG(inode->i_نما)) {
			إذا (!ext2fs_has_feature_large_file(fs->super)) {
				ext2fs_مجموعه_feature_large_file(fs->super);
				dirty_sb = 1;
			}
		} وإلا إذا (LINUX_S_ISDIR(inode->i_نما)) {
			إذا (!ext2fs_has_feature_largedir(fs->super)) {
				ext2fs_مجموعه_feature_largedir(fs->super);
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
	inode->i_اندازه_بالا = (اندازه >> 32);

	أرجع 0;
}

