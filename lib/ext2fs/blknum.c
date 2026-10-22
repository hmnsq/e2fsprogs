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
dgrp_t ext2fs_गणः_सम्बन्धि_blk2(ext2_filsys fs, blk64_t blk)
{
	प्रत्यागम (blk - fs->super->s_first_दत्त_खण्डम्) /
		fs->super->s_खण्डानि_प्रत्येकम्_गणः;
}

/*
 * Return the first block (inclusive) in a group
 */
blk64_t ext2fs_गणः_first_block2(ext2_filsys fs, dgrp_t गणः)
{
	प्रत्यागम fs->super->s_first_दत्त_खण्डम् +
		EXT2_GROUPS_गन्तव्यम्_खण्डानि(fs->super, गणः);
}

/*
 * Return the last block (inclusive) in a group
 */
blk64_t ext2fs_गणः_चरमः_block2(ext2_filsys fs, dgrp_t गणः)
{
	प्रत्यागम (गणः == fs->गणः_desc_गणनम् - 1 ?
		ext2fs_खण्डानि_गणनम्(fs->super) - 1 :
		ext2fs_गणः_first_block2(fs, गणः) +
			(fs->super->s_खण्डानि_प्रत्येकम्_गणः - 1));
}

/*
 * Return the number of blocks in a group
 */
पूर्णाङ्क ext2fs_गणः_खण्डानि_गणनम्(ext2_filsys fs, dgrp_t गणः)
{
	पूर्णाङ्क अङ्कः_खण्डानि;

	यदि (गणः == fs->गणः_desc_गणनम् - 1) {
		अङ्कः_खण्डानि = (ext2fs_खण्डानि_गणनम्(fs->super) -
				fs->super->s_first_दत्त_खण्डम्) %
			      fs->super->s_खण्डानि_प्रत्येकम्_गणः;
		यदि (!अङ्कः_खण्डानि)
			अङ्कः_खण्डानि = fs->super->s_खण्डानि_प्रत्येकम्_गणः;
	} अन्यथा
		अङ्कः_खण्डानि = fs->super->s_खण्डानि_प्रत्येकम्_गणः;

	प्रत्यागम अङ्कः_खण्डानि;
}

/*
 * Return the inode data block count
 */
blk64_t ext2fs_inode_दत्त_blocks2(ext2_filsys fs,
					संरचना ext2_inode *inode)
{
	प्रत्यागम (inode->i_खण्डानि |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64) inode->osd2.linux2.l_i_खण्डानि_hi << 32 : 0)) -
		(inode->i_file_acl ? EXT2_CLUSTER_आकार(fs->super) >> 9 : 0);
}

/*
 * Return the inode i_blocks count
 */
blk64_t ext2fs_inode_i_खण्डानि(ext2_filsys fs,
					संरचना ext2_inode *inode)
{
	प्रत्यागम (inode->i_खण्डानि |
		(ext2fs_has_feature_huge_file(fs->super) ?
		 (__u64)inode->osd2.linux2.l_i_खण्डानि_hi << 32 : 0));
}

/*
 * Return the inode i_blocks in stat (512 byte) units
 */
blk64_t ext2fs_get_stat_i_खण्डानि(ext2_filsys fs,
				 संरचना ext2_inode *inode)
{
	blk64_t	ret = inode->i_खण्डानि;

	यदि (ext2fs_has_feature_huge_file(fs->super)) {
		ret += ((दीर्घ दीर्घ) inode->osd2.linux2.l_i_खण्डानि_hi) << 32;
		यदि (inode->i_flags & EXT4_HUGE_FILE_FL)
			ret *= (fs->blocksize / 512);
	}
	प्रत्यागम ret;
}

/*
 * Return the fs block count
 */
blk64_t ext2fs_खण्डानि_गणनम्(संरचना ext2_super_खण्डम् *super)
{
	प्रत्यागम super->s_खण्डानि_गणनम् |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_खण्डानि_गणनम्_hi << 32 : 0);
}

/*
 * Set the fs block count
 */
शून्य ext2fs_खण्डानि_गणनम्_समुच्चयः(संरचना ext2_super_खण्डम् *super, blk64_t blk)
{
	super->s_खण्डानि_गणनम् = blk;
	यदि (ext2fs_has_feature_64bit(super))
		super->s_खण्डानि_गणनम्_hi = (__u64) blk >> 32;
}

/*
 * Add to the current fs block count
 */
शून्य ext2fs_खण्डानि_गणनम्_सङ्कलय(संरचना ext2_super_खण्डम् *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_खण्डानि_गणनम्(super) + blk;
	ext2fs_खण्डानि_गणनम्_समुच्चयः(super, tmp);
}

/*
 * Return the fs reserved block count
 */
blk64_t ext2fs_r_खण्डानि_गणनम्(संरचना ext2_super_खण्डम् *super)
{
	प्रत्यागम super->s_r_खण्डानि_गणनम् |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_r_खण्डानि_गणनम्_hi << 32 : 0);
}

/*
 * Set the fs reserved block count
 */
शून्य ext2fs_r_खण्डानि_गणनम्_समुच्चयः(संरचना ext2_super_खण्डम् *super, blk64_t blk)
{
	super->s_r_खण्डानि_गणनम् = blk;
	यदि (ext2fs_has_feature_64bit(super))
		super->s_r_खण्डानि_गणनम्_hi = (__u64) blk >> 32;
}

/*
 * Add to the current reserved fs block count
 */
शून्य ext2fs_r_खण्डानि_गणनम्_सङ्कलय(संरचना ext2_super_खण्डम् *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_r_खण्डानि_गणनम्(super) + blk;
	ext2fs_r_खण्डानि_गणनम्_समुच्चयः(super, tmp);
}

/*
 * Return the fs free block count
 */
blk64_t ext2fs_free_खण्डानि_गणनम्(संरचना ext2_super_खण्डम् *super)
{
	प्रत्यागम super->s_free_खण्डानि_गणनम् |
		(ext2fs_has_feature_64bit(super) ?
		(__u64) super->s_free_खण्डानि_hi << 32 : 0);
}

/*
 * Set the fs free block count
 */
शून्य ext2fs_free_खण्डानि_गणनम्_समुच्चयः(संरचना ext2_super_खण्डम् *super, blk64_t blk)
{
	super->s_free_खण्डानि_गणनम् = blk;
	यदि (ext2fs_has_feature_64bit(super))
		super->s_free_खण्डानि_hi = (__u64) blk >> 32;
}

/*
 * Add to the current free fs block count
 */
शून्य ext2fs_free_खण्डानि_गणनम्_सङ्कलय(संरचना ext2_super_खण्डम् *super, blk64_t blk)
{
	blk64_t tmp;
	tmp = ext2fs_free_खण्डानि_गणनम्(super) + blk;
	ext2fs_free_खण्डानि_गणनम्_समुच्चयः(super, tmp);
}

/*
 * Get a pointer to a block group descriptor.  We need the explicit
 * pointer to the group desc for code that swaps block group
 * descriptors before writing them out, as it wants to make a copy and
 * do the swap there.
 */
संरचना ext2_गणः_desc *ext2fs_गणः_desc(ext2_filsys fs,
					  संरचना opaque_ext2_गणः_desc *gdp,
					  dgrp_t गणः)
{
	संरचना ext2_गणः_desc *ret_gdp;
	errcode_t	retval;
	स्थिर वर्ण	*buf = 0;
	स्थिर अचिह्नित	bufsize = 0;
	blk64_t		blk;
	पूर्णाङ्क		desc_आकार = EXT2_DESC_आकार(fs->super) & ~7;
	पूर्णाङ्क		desc_प्रत्येकम्_blk = EXT2_DESC_प्रत्येकम्_खण्डम्(fs->super);

	यदि (गणः > fs->गणः_desc_गणनम्)
		प्रत्यागम रिक्त;
	यदि (gdp)
		प्रत्यागम (संरचना ext2_गणः_desc *)((वर्ण *)gdp +
						  गणः * desc_आकार);
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
	blk = ext2fs_descriptor_खण्डम्_loc2(fs, fs->super->s_first_दत्त_खण्डम्,
					   गणः / desc_प्रत्येकम्_blk);
	retval = io_channel_read_blk(fs->io, blk, 1, buf);
	यदि (retval)
		प्रत्यागम रिक्त;
	ret_gdp = (संरचना ext2_गणः_desc *)
		(buf + ((गणः % desc_प्रत्येकम्_blk) * desc_आकार));
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	ext2fs_swap_गणः_desc2(fs, ret_gdp);
#यदि_अन्तः
	प्रत्यागम ret_gdp;
}

/* Do the same but as an ext4 group desc for internal use here */
स्थिर संरचना ext4_गणः_desc *ext4fs_गणः_desc(ext2_filsys fs,
					  संरचना opaque_ext2_गणः_desc *gdp,
					  dgrp_t गणः)
{
	प्रत्यागम (संरचना ext4_गणः_desc *)ext2fs_गणः_desc(fs, gdp, गणः);
}

/*
 * Return the block bitmap checksum of a group
 */
__u32 ext2fs_खण्डम्_bitmap_checksum(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;
	__u32 csum;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	csum = gdp->bg_खण्डम्_bitmap_csum_lo;
	यदि (EXT2_DESC_आकार(fs->super) >= EXT4_BG_खण्डम्_BITMAP_CSUM_HI_LOCATION)
		csum |= ((__u32)gdp->bg_खण्डम्_bitmap_csum_hi << 16);
	प्रत्यागम csum;
}

/*
 * Return the block bitmap block of a group
 */
blk64_t ext2fs_खण्डम्_bitmap_loc(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	प्रत्यागम gdp->bg_खण्डम्_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64)gdp->bg_खण्डम्_bitmap_hi << 32 : 0);
}

/*
 * Set the block bitmap block of a group
 */
शून्य ext2fs_खण्डम्_bitmap_loc_समुच्चयः(ext2_filsys fs, dgrp_t गणः, blk64_t blk)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_खण्डम्_bitmap = blk;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_खण्डम्_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode bitmap checksum of a group
 */
__u32 ext2fs_inode_bitmap_checksum(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;
	__u32 csum;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	csum = gdp->bg_inode_bitmap_csum_lo;
	यदि (EXT2_DESC_आकार(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_अन्तः)
		csum |= ((__u32)gdp->bg_inode_bitmap_csum_hi << 16);
	प्रत्यागम csum;
}

/*
 * Return the inode bitmap block of a group
 */
blk64_t ext2fs_inode_bitmap_loc(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	प्रत्यागम gdp->bg_inode_bitmap |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_bitmap_hi << 32 : 0);
}

/*
 * Set the inode bitmap block of a group
 */
शून्य ext2fs_inode_bitmap_loc_समुच्चयः(ext2_filsys fs, dgrp_t गणः, blk64_t blk)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_inode_bitmap = blk;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_bitmap_hi = (__u64) blk >> 32;
}

/*
 * Return the inode table block of a group
 */
blk64_t ext2fs_inode_सारणी_loc(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	प्रत्यागम gdp->bg_inode_सारणी |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u64) gdp->bg_inode_सारणी_hi << 32 : 0);
}

/*
 * Set the inode table block of a group
 */
शून्य ext2fs_inode_सारणी_loc_समुच्चयः(ext2_filsys fs, dgrp_t गणः, blk64_t blk)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_inode_सारणी = blk;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_inode_सारणी_hi = (__u64) blk >> 32;
}

/*
 * Return the free blocks count of a group
 */
__u32 ext2fs_bg_free_खण्डानि_गणनम्(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	प्रत्यागम gdp->bg_free_खण्डानि_गणनम् |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_खण्डानि_गणनम्_hi << 16 : 0);
}

/*
 * Set the free blocks count of a group
 */
शून्य ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(ext2_filsys fs, dgrp_t गणः, __u32 n)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_free_खण्डानि_गणनम् = n;

	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_खण्डानि_गणनम्_hi = (__u32) n >> 16;
}

/*
 * Return the free inodes count of a group
 */
__u32 ext2fs_bg_free_inodes_गणनम्(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	प्रत्यागम gdp->bg_free_inodes_गणनम् |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_free_inodes_गणनम्_hi << 16 : 0);
}

/*
 * Set the free inodes count of a group
 */
शून्य ext2fs_bg_free_inodes_गणनम्_समुच्चयः(ext2_filsys fs, dgrp_t गणः, __u32 n)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_free_inodes_गणनम् = n;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_free_inodes_गणनम्_hi = (__u32) n >> 16;
}

/*
 * Return the used dirs count of a group
 */
__u32 ext2fs_bg_प्रयुक्तम्_dirs_गणनम्(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	प्रत्यागम gdp->bg_प्रयुक्तम्_dirs_गणनम् |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_प्रयुक्तम्_dirs_गणनम्_hi << 16 : 0);
}

/*
 * Set the used dirs count of a group
 */
शून्य ext2fs_bg_प्रयुक्तम्_dirs_गणनम्_समुच्चयः(ext2_filsys fs, dgrp_t गणः, __u32 n)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_प्रयुक्तम्_dirs_गणनम् = n;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_प्रयुक्तम्_dirs_गणनम्_hi = (__u32) n >> 16;
}

/*
 * Return the unused inodes count of a group
 */
__u32 ext2fs_bg_itable_अप्रयुक्तम्(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	प्रत्यागम gdp->bg_itable_अप्रयुक्तम् |
		(ext2fs_has_feature_64bit(fs->super) ?
		 (__u32) gdp->bg_itable_अप्रयुक्तम्_hi << 16 : 0);
}

/*
 * Set the unused inodes count of a group
 */
शून्य ext2fs_bg_itable_अप्रयुक्तम्_समुच्चयः(ext2_filsys fs, dgrp_t गणः, __u32 n)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_itable_अप्रयुक्तम् = n;
	यदि (ext2fs_has_feature_64bit(fs->super))
		gdp->bg_itable_अप्रयुक्तम्_hi = (__u32) n >> 16;
}

/*
 * Get the flags for this block group
 */
__u16 ext2fs_bg_flags(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	प्रत्यागम gdp->bg_flags;
}

/*
 * Zero out the flags for this block group
 */
शून्य ext2fs_bg_flags_zap(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_flags = 0;
	प्रत्यागम;
}

/*
 * Get the value of a particular flag for this block group
 */
पूर्णाङ्क ext2fs_bg_flags_test(ext2_filsys fs, dgrp_t गणः, __u16 bg_flag)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	प्रत्यागम gdp->bg_flags & bg_flag;
}

/*
 * Set a flag or set of flags for this block group
 */
शून्य ext2fs_bg_flags_समुच्चयः(ext2_filsys fs, dgrp_t गणः, __u16 bg_flags)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_flags |= bg_flags;
	प्रत्यागम;
}

/*
 * Clear a flag or set of flags for this block group
 */
शून्य ext2fs_bg_flags_clear(ext2_filsys fs, dgrp_t गणः, __u16 bg_flags)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_flags &= ~bg_flags;
	प्रत्यागम;
}

/*
 * Get the checksum for this block group
 */
__u16 ext2fs_bg_checksum(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	प्रत्यागम gdp->bg_checksum;
}

/*
 * Set the checksum for this block group to a previously calculated value
 */
शून्य ext2fs_bg_checksum_समुच्चयः(ext2_filsys fs, dgrp_t गणः, __u16 checksum)
{
	संरचना ext4_गणः_desc *gdp;

	gdp = ext4fs_गणः_desc(fs, fs->गणः_desc, गणः);
	gdp->bg_checksum = checksum;
	प्रत्यागम;
}

/*
 * Get the acl block of a file
 */
blk64_t ext2fs_file_acl_खण्डम्(ext2_filsys fs, ध्रुव संरचना ext2_inode *inode)
{
	blk64_t	blk = inode->i_file_acl;

	यदि (fs && ext2fs_has_feature_64bit(fs->super))
		blk |= ((__u64) inode->osd2.linux2.l_i_file_acl_उच्चः) << 32;
	प्रत्यागम blk;
}

/*
 * Set the acl block of a file
 */
शून्य ext2fs_file_acl_खण्डम्_समुच्चयः(ext2_filsys fs, संरचना ext2_inode *inode,
			       blk64_t blk)
{
	inode->i_file_acl = blk;
	यदि (fs && ext2fs_has_feature_64bit(fs->super))
		inode->osd2.linux2.l_i_file_acl_उच्चः = (__u64) blk >> 32;
}

/*
 * Set the size of the inode
 */
errcode_t ext2fs_inode_आकार_समुच्चयः(ext2_filsys fs, संरचना ext2_inode *inode,
				ext2_off64_t आकार)
{
	यदि (आकार < 0)
		प्रत्यागम EINVAL;

	/* If writing a large inode, set the large_file or large_dir flag */
	यदि (ext2fs_needs_large_file_feature(आकार)) {
		पूर्णाङ्क dirty_sb = 0;

		यदि (LINUX_S_ISREG(inode->i_बहुलकम्)) {
			यदि (!ext2fs_has_feature_large_file(fs->super)) {
				ext2fs_समुच्चयः_feature_large_file(fs->super);
				dirty_sb = 1;
			}
		} अन्यथा यदि (LINUX_S_ISDIR(inode->i_बहुलकम्)) {
			यदि (!ext2fs_has_feature_largedir(fs->super)) {
				ext2fs_समुच्चयः_feature_largedir(fs->super);
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

	inode->i_आकार = आकार & 0xffffffff;
	inode->i_आकार_उच्चः = (आकार >> 32);

	प्रत्यागम 0;
}

