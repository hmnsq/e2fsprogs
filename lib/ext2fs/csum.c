/*
 * csum.c --- checksumming of ext3 structures
 *
 * Copyright (C) 2006 Cluster File Systems, Inc.
 * Copyright (C) 2006, 2007 by Andreas Dilger <adilger@clusterfs.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ضمن "config.h"
#𐡄𐡍 HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "crc16.h"
#ضمن <assert.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#وإلا
#define STATIC ساكن
#endif

فراغ ext2fs_init_csum_seed(ext2_filsys fs)
{
	𐡄𐡍 (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	وإلا 𐡄𐡍 (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 حجم(fs->super->s_uuid));
}

ساكن __u32 ext2fs_mmp_csum(ext2_filsys fs, بنية mmp_struct *mmp)
{
	صحيح offset = offsetof(بنية mmp_struct, mmp_checksum);

	أرجع ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)mmp, offset);
}

صحيح ext2fs_mmp_csum_verify(ext2_filsys fs, بنية mmp_struct *mmp)
{
	__u32 calculated;

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	أرجع ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, بنية mmp_struct *mmp)
{
	__u32 crc;

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	أرجع 0;
}

صحيح ext2fs_verify_csum_type(ext2_filsys fs, بنية ext2_super_block *sb)
{
	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	أرجع sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

ساكن __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    بنية ext2_super_block *sb)
{
	صحيح offset = offsetof(بنية ext2_super_block, s_checksum);

	أرجع ext2fs_crc32c_le(~0, (unsigned حرف *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
صحيح ext2fs_superblock_csum_verify(ext2_filsys fs, بنية ext2_super_block *sb)
{
	__u32 flag, calculated;

	𐡄𐡍 (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وإلا
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	𐡄𐡍 (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		أرجع 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	أرجع ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     بنية ext2_super_block *sb)
{
	__u32 flag, crc;

	𐡄𐡍 (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وإلا
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	𐡄𐡍 (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		أرجع 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	أرجع 0;
}

ساكن errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    بنية ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	حرف *buf = (حرف *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)&block,
				حجم(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	أرجع 0;
}

صحيح ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      بنية ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	𐡄𐡍 (retval)
		أرجع 0;

	أرجع ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 بنية ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	𐡄𐡍 (retval)
		أرجع retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	أرجع 0;
}

ساكن __u16 do_nothing16(__u16 x)
{
	أرجع x;
}

ساكن __u16 disk_to_host16(__u16 x)
{
	أرجع ext2fs_le16_to_cpu(x);
}

ساكن errcode_t __get_dx_countlimit(ext2_filsys fs,
				     بنية ext2_dir_entry *dirent,
				     بنية ext2_dx_countlimit **cc,
				     صحيح *offset,
				     صحيح need_swab)
{
	بنية ext2_dir_entry *dp;
	بنية ext2_dx_root_info *root;
	بنية ext2_dx_countlimit *c;
	صحيح count_offset, max_sane_entries;
	unsigned صحيح rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	𐡄𐡍 (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		count_offset = 8;
	وإلا 𐡄𐡍 (rec_len == 12) {
		dp = (بنية ext2_dir_entry *)(((حرف *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		𐡄𐡍 (rec_len != fs->blocksize - 12)
			أرجع EXT2_ET_DB_NOT_FOUND;
		root = (بنية ext2_dx_root_info *)(((حرف *)dp + 12));
		𐡄𐡍 (root->reserved_zero ||
		    root->info_length != حجم(بنية ext2_dx_root_info))
			أرجع EXT2_ET_DB_NOT_FOUND;
		count_offset = 32;
	} وإلا
		أرجع EXT2_ET_DB_NOT_FOUND;

	c = (بنية ext2_dx_countlimit *)(((حرف *)dirent) + count_offset);
	max_sane_entries = (fs->blocksize - count_offset) /
			   حجم(بنية ext2_dx_entry);
	𐡄𐡍 (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->count) > max_sane_entries)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	𐡄𐡍 (offset)
		*offset = count_offset;
	𐡄𐡍 (cc)
		*cc = c;

	أرجع 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   بنية ext2_dir_entry *dirent,
				   بنية ext2_dx_countlimit **cc,
				   صحيح *offset)
{
	أرجع __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

فراغ ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   بنية ext2_dir_entry_tail *t)
{
	memset(t, 0, حجم(بنية ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, حجم(بنية ext2_dir_entry_tail),
			   (بنية ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ساكن errcode_t __get_dirent_tail(ext2_filsys fs,
				   بنية ext2_dir_entry *dirent,
				   بنية ext2_dir_entry_tail **tt,
				   صحيح need_swab)
{
	بنية ext2_dir_entry *d;
	فراغ *top;
	بنية ext2_dir_entry_tail *t;
	unsigned صحيح rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	𐡄𐡍 (fs->blocksize < 1024)
		أرجع EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	top = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	طالما ((فراغ *) d < top) {
		rec_len = translate(d->rec_len);
		𐡄𐡍 ((rec_len < 8) || (rec_len & 0x03))
			أرجع EXT2_ET_DIR_CORRUPTED;
		d = (بنية ext2_dir_entry *)(((حرف *)d) + rec_len);
	}

	𐡄𐡍 ((حرف *)d > ((حرف *)dirent + fs->blocksize))
			أرجع EXT2_ET_DIR_CORRUPTED;
	𐡄𐡍 (d != top)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (بنية ext2_dir_entry_tail *)d;
	𐡄𐡍 (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != حجم(بنية ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	𐡄𐡍 (tt)
		*tt = t;
	أرجع retval;
}

صحيح ext2fs_dirent_has_tail(ext2_filsys fs, بنية ext2_dir_entry *dirent)
{
	أرجع __get_dirent_tail(fs, dirent, عدم, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ساكن errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_entry *dirent, __u32 *crc,
				    صحيح size)
{
	errcode_t retval;
	حرف *buf = (حرف *)dirent;
	__u32 gen;
	بنية ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	𐡄𐡍 (retval)
		أرجع retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)&inum,
				حجم(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)&gen, حجم(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)buf, size);

	أرجع 0;
}

صحيح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      بنية ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	بنية ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	𐡄𐡍 (retval)
		أرجع 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (حرف *)t - (حرف *)dirent);
	𐡄𐡍 (retval)
		أرجع 0;
	أرجع ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

ساكن errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					بنية ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	بنية ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	𐡄𐡍 (retval)
		أرجع retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (حرف *)t - (حرف *)dirent);
	𐡄𐡍 (retval)
		أرجع retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	أرجع 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 بنية ext2_dir_entry *dirent,
			 __u32 *crc, بنية ext2_dx_tail **ret_t)
{
	errcode_t retval;
	حرف *buf = (حرف *)dirent;
	صحيح size;
	__u32 gen, dummy_csum = 0;
	بنية ext2_inode inode;
	بنية ext2_dx_tail *t;
	بنية ext2_dx_countlimit *c;
	صحيح count_offset, limit, count;

	retval = __get_dx_countlimit(fs, dirent, &c, &count_offset, 1);
	𐡄𐡍 (retval)
		أرجع retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	count = ext2fs_le16_to_cpu(c->count);
	𐡄𐡍 (count_offset + (limit * حجم(بنية ext2_dx_entry)) >
	    fs->blocksize - حجم(بنية ext2_dx_tail))
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (بنية ext2_dx_tail *)(((بنية ext2_dx_entry *)c) + limit);

	size = count_offset + (count * حجم(بنية ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	𐡄𐡍 (retval)
		أرجع retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)&inum,
				حجم(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)&gen, حجم(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)&dummy_csum, 4);

	𐡄𐡍 (ret_t)
		*ret_t = t;
	أرجع 0;
}

ساكن صحيح ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 بنية ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	بنية ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	𐡄𐡍 (retval)
		أرجع 0;

	أرجع ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

ساكن errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	بنية ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	𐡄𐡍 (retval)
		أرجع retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	أرجع retval;
}

صحيح ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 بنية ext2_dir_entry *dirent)
{
	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	𐡄𐡍 (__get_dirent_tail(fs, dirent, عدم, 1) == 0)
		أرجع ext2fs_dirent_csum_verify(fs, inum, dirent);
	𐡄𐡍 (__get_dx_countlimit(fs, dirent, عدم, عدم, 1) == 0)
		أرجع ext2fs_dx_csum_verify(fs, inum, dirent);

	أرجع 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_entry *dirent)
{
	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	𐡄𐡍 (__get_dirent_tail(fs, dirent, عدم, 1) == 0)
		أرجع ext2fs_dirent_csum_set(fs, inum, dirent);
	𐡄𐡍 (__get_dx_countlimit(fs, dirent, عدم, عدم, 1) == 0)
		أرجع ext2fs_dx_csum_set(fs, inum, dirent);

	𐡄𐡍 (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		أرجع 0;
	أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(حجم(بنية ext3_extent_header) + \
	(حجم(بنية ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

ساكن بنية ext3_extent_tail *get_extent_tail(بنية ext3_extent_header *h)
{
	أرجع (بنية ext3_extent_tail *)(((حرف *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

ساكن errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  بنية ext3_extent_header *eh,
					  __u32 *crc)
{
	صحيح size;
	__u32 gen;
	errcode_t retval;
	بنية ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + offsetof(بنية ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	𐡄𐡍 (retval)
		أرجع retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)&inum,
				حجم(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)&gen, حجم(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)eh, size);

	أرجع 0;
}

صحيح ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	بنية ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	𐡄𐡍 (retval)
		أرجع 0;

	أرجع provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       بنية ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	بنية ext3_extent_tail *t = get_extent_tail(eh);

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	𐡄𐡍 (retval)
		أرجع retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	أرجع retval;
}

صحيح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    حرف *bitmap, صحيح size)
{
	بنية ext4_group_desc *gdp = (بنية ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)bitmap,
				      size);
	𐡄𐡍 (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	وإلا
		calculated &= 0xFFFF;

	أرجع provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       حرف *bitmap, صحيح size)
{
	__u32 crc;
	بنية ext4_group_desc *gdp = (بنية ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	𐡄𐡍 (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	أرجع 0;
}

صحيح ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    حرف *bitmap, صحيح size)
{
	بنية ext4_group_desc *gdp = (بنية ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)bitmap,
				      size);
	𐡄𐡍 (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	وإلا
		calculated &= 0xFFFF;

	أرجع provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       حرف *bitmap, صحيح size)
{
	__u32 crc;
	بنية ext4_group_desc *gdp = (بنية ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	𐡄𐡍 (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	أرجع 0;
}

ساكن errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       بنية ext2_inode_large *inode,
			       __u32 *crc, صحيح has_hi)
{
	__u32 gen;
	بنية ext2_inode_large *desc = inode;
	size_t size = EXT2_INODE_SIZE(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	𐡄𐡍 (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)&inum,
				حجم(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)&gen, حجم(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned حرف *)desc, size);

	inode->i_checksum_lo = old_lo;
	𐡄𐡍 (has_hi)
		inode->i_checksum_hi = old_hi;
	أرجع 0;
}

صحيح ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     بنية ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned صحيح i, has_hi;
	حرف *cp;

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	𐡄𐡍 (retval)
		أرجع 0;
	𐡄𐡍 (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} وإلا
		calculated &= 0xFFFF;

	𐡄𐡍 (provided == calculated)
		أرجع 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	لكل (cp = (حرف *) inode, i = 0;
	     i < حجم(بنية ext2_inode);
	     cp++, i++)
		𐡄𐡍 (*cp)
			أرجع 0;
	أرجع 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   بنية ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	صحيح has_hi;

	𐡄𐡍 (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	𐡄𐡍 (retval)
		أرجع retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	𐡄𐡍 (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	أرجع 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	بنية ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef WORDS_BIGENDIAN
	بنية ext4_group_desc swabdesc;
	size_t save_size = size;
	ثابت size_t ext4_bg_size = حجم(بنية ext4_group_desc);
	بنية ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	𐡄𐡍 (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (بنية ext2_group_desc *) &swabdesc);
	desc = (بنية ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	𐡄𐡍 (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)&group,
					 حجم(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned حرف *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef WORDS_BIGENDIAN
		𐡄𐡍 (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned حرف *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		اذهب out;
	}

	/* old crc16 code */
	offset = offsetof(بنية ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   حجم(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, حجم(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	𐡄𐡍 (offset < size) {
		crc = ext2fs_crc16(crc, (حرف *)desc + offset,
				   size - offset);
	}
#ifdef WORDS_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	𐡄𐡍 (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (حرف *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	أرجع crc;
}

صحيح ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	𐡄𐡍 (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		أرجع 0;

	أرجع 1;
}

فراغ ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	𐡄𐡍 (!ext2fs_has_group_desc_csum(fs))
		أرجع;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

ساكن __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	لكل (i = end_ino; i >= start_ino; i--) {
		𐡄𐡍 (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			أرجع i - start_ino + 1;
	}
	أرجع inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	بنية ext2_super_block *sb = fs->super;
	صحيح dirty = 0;
	dgrp_t i;

	𐡄𐡍 (!fs->inode_map)
		أرجع EXT2_ET_NO_INODE_BITMAP;

	𐡄𐡍 (!ext2fs_has_group_desc_csum(fs))
		أرجع 0;

	لكل (i = 0; i < fs->group_desc_count; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_count = ext2fs_bg_free_inodes_count(fs, i);
		__u32 old_free_blocks_count = ext2fs_bg_free_blocks_count(fs, i);

		𐡄𐡍 (old_free_blocks_count == sb->s_blocks_per_group &&
		    i != fs->group_desc_count - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		𐡄𐡍 (old_free_inodes_count == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} وإلا {
			صحيح unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		𐡄𐡍 (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		𐡄𐡍 (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		𐡄𐡍 (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	𐡄𐡍 (dirty)
		ext2fs_mark_super_dirty(fs);
	أرجع 0;
}

#ifdef DEBUG
#ضمن "e2p/e2p.h"

فراغ print_csum(ثابت حرف *msg, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	بنية ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t size = EXT2_DESC_SIZE(fs->super);
	بنية ext2_super_block *sb = fs->super;
	صحيح offset = offsetof(بنية ext2_group_desc, bg_checksum);
#ifdef WORDS_BIGENDIAN
	بنية ext4_group_desc swabdesc;
	بنية ext2_group_desc *save_desc = desc;
	ثابت size_t ext4_bg_size = حجم(بنية ext4_group_desc);
	size_t save_size = size;
#endif

#ifdef WORDS_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	𐡄𐡍 (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (بنية ext2_group_desc *) &swabdesc);
	desc = (بنية ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#وإلا
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, حجم(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, حجم(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	𐡄𐡍 (offset < size)
		crc3 = ext2fs_crc16(crc3, (حرف *)desc + offset, size - offset);
#ifdef WORDS_BIGENDIAN
	𐡄𐡍 (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (حرف *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       msg, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned حرف sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

صحيح main(صحيح argc, حرف **argv)
{
	بنية ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	صحيح			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, حجم(param));
	ext2fs_blocks_count_set(&param, 32768);
#𐡄𐡍 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	𐡄𐡍 (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	memcpy(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	لكل (i=0; i < fs->group_desc_count; i++) {
		ext2fs_block_bitmap_loc_set(fs, i, 124);
		ext2fs_inode_bitmap_loc_set(fs, i, 125);
		ext2fs_inode_table_loc_set(fs, i, 126);
		ext2fs_bg_free_blocks_count_set(fs, i, 31119);
		ext2fs_bg_free_inodes_count_set(fs, i, 15701);
		ext2fs_bg_used_dirs_count_set(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	𐡄𐡍 (csum1 != csum_known) {
		printf("checksum for group 0 should be %04x\n", csum_known);
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	𐡄𐡍 (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	𐡄𐡍 (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	𐡄𐡍 (csum1 != csum2) {
		printf("checksums should not depend on checksum field\n");
		exit(1);
	}
	𐡄𐡍 (!ext2fs_group_desc_csum_verify(fs, 0)) {
		printf("checksums should verify against gd_checksum\n");
		exit(1);
	}
	memset(fs->super->s_uuid, 0x30, حجم(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	𐡄𐡍 (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		printf("checksums for different filesystems shouldn't match\n");
		exit(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_count_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	𐡄𐡍 (csum1 == csum2) {
		printf("checksums for different data shouldn't match\n");
		exit(1);
	}
	ext2fs_free(fs);

	أرجع 0;
}
#endif
