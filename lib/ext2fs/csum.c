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

#شامل "config.h"
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "crc16.h"
#شامل <assert.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#وگرنه
#define STATIC ایستا
#endif

تهی ext2fs_init_csum_seed(ext2_filsys fs)
{
	اگر (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	وگرنه اگر (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 اندازه(fs->super->s_uuid));
}

ایستا __u32 ext2fs_mmp_csum(ext2_filsys fs, ساختار mmp_struct *mmp)
{
	صحیح offset = offsetof(ساختار mmp_struct, mmp_checksum);

	بازگشت ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)mmp, offset);
}

صحیح ext2fs_mmp_csum_verify(ext2_filsys fs, ساختار mmp_struct *mmp)
{
	__u32 calculated;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	بازگشت ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, ساختار mmp_struct *mmp)
{
	__u32 crc;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	بازگشت 0;
}

صحیح ext2fs_verify_csum_type(ext2_filsys fs, ساختار ext2_super_block *sb)
{
	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 1;

	بازگشت sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

ایستا __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    ساختار ext2_super_block *sb)
{
	صحیح offset = offsetof(ساختار ext2_super_block, s_checksum);

	بازگشت ext2fs_crc32c_le(~0, (unsigned نویسه *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
صحیح ext2fs_superblock_csum_verify(ext2_filsys fs, ساختار ext2_super_block *sb)
{
	__u32 flag, calculated;

	اگر (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وگرنه
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	اگر (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		بازگشت 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	بازگشت ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     ساختار ext2_super_block *sb)
{
	__u32 flag, crc;

	اگر (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وگرنه
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	اگر (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		بازگشت 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	بازگشت 0;
}

ایستا errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    ساختار ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	نویسه *buf = (نویسه *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)&block,
				اندازه(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	بازگشت 0;
}

صحیح ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      ساختار ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	اگر (retval)
		بازگشت 0;

	بازگشت ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 ساختار ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	اگر (retval)
		بازگشت retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	بازگشت 0;
}

ایستا __u16 do_nothing16(__u16 x)
{
	بازگشت x;
}

ایستا __u16 disk_to_host16(__u16 x)
{
	بازگشت ext2fs_le16_to_cpu(x);
}

ایستا errcode_t __get_dx_countlimit(ext2_filsys fs,
				     ساختار ext2_dir_entry *dirent,
				     ساختار ext2_dx_countlimit **cc,
				     صحیح *offset,
				     صحیح need_swab)
{
	ساختار ext2_dir_entry *dp;
	ساختار ext2_dx_ریشه_info *ریشه;
	ساختار ext2_dx_countlimit *c;
	صحیح شمار_offset, max_sane_entries;
	unsigned صحیح rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	اگر (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		شمار_offset = 8;
	وگرنه اگر (rec_len == 12) {
		dp = (ساختار ext2_dir_entry *)(((نویسه *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		اگر (rec_len != fs->blocksize - 12)
			بازگشت EXT2_ET_DB_NOT_FOUND;
		ریشه = (ساختار ext2_dx_ریشه_info *)(((نویسه *)dp + 12));
		اگر (ریشه->reserved_zero ||
		    ریشه->info_length != اندازه(ساختار ext2_dx_ریشه_info))
			بازگشت EXT2_ET_DB_NOT_FOUND;
		شمار_offset = 32;
	} وگرنه
		بازگشت EXT2_ET_DB_NOT_FOUND;

	c = (ساختار ext2_dx_countlimit *)(((نویسه *)dirent) + شمار_offset);
	max_sane_entries = (fs->blocksize - شمار_offset) /
			   اندازه(ساختار ext2_dx_entry);
	اگر (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->شمار) > max_sane_entries)
		بازگشت EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	اگر (offset)
		*offset = شمار_offset;
	اگر (cc)
		*cc = c;

	بازگشت 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   ساختار ext2_dir_entry *dirent,
				   ساختار ext2_dx_countlimit **cc,
				   صحیح *offset)
{
	بازگشت __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

تهی ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   ساختار ext2_dir_entry_tail *t)
{
	memset(t, 0, اندازه(ساختار ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, اندازه(ساختار ext2_dir_entry_tail),
			   (ساختار ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ایستا errcode_t __get_dirent_tail(ext2_filsys fs,
				   ساختار ext2_dir_entry *dirent,
				   ساختار ext2_dir_entry_tail **tt,
				   صحیح need_swab)
{
	ساختار ext2_dir_entry *d;
	تهی *بالاترین;
	ساختار ext2_dir_entry_tail *t;
	unsigned صحیح rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	اگر (fs->blocksize < 1024)
		بازگشت EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	بالاترین = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	وقتی ((تهی *) d < بالاترین) {
		rec_len = translate(d->rec_len);
		اگر ((rec_len < 8) || (rec_len & 0x03))
			بازگشت EXT2_ET_DIR_CORRUPTED;
		d = (ساختار ext2_dir_entry *)(((نویسه *)d) + rec_len);
	}

	اگر ((نویسه *)d > ((نویسه *)dirent + fs->blocksize))
			بازگشت EXT2_ET_DIR_CORRUPTED;
	اگر (d != بالاترین)
		بازگشت EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (ساختار ext2_dir_entry_tail *)d;
	اگر (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != اندازه(ساختار ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		بازگشت EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	اگر (tt)
		*tt = t;
	بازگشت retval;
}

صحیح ext2fs_dirent_has_tail(ext2_filsys fs, ساختار ext2_dir_entry *dirent)
{
	بازگشت __get_dirent_tail(fs, dirent, پوچ, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ایستا errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    ساختار ext2_dir_entry *dirent, __u32 *crc,
				    صحیح size)
{
	errcode_t retval;
	نویسه *buf = (نویسه *)dirent;
	__u32 gen;
	ساختار ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	اگر (retval)
		بازگشت retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)&inum,
				اندازه(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)&gen, اندازه(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)buf, size);

	بازگشت 0;
}

صحیح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      ساختار ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	ساختار ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	اگر (retval)
		بازگشت 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (نویسه *)t - (نویسه *)dirent);
	اگر (retval)
		بازگشت 0;
	بازگشت ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

ایستا errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					ساختار ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	ساختار ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	اگر (retval)
		بازگشت retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (نویسه *)t - (نویسه *)dirent);
	اگر (retval)
		بازگشت retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	بازگشت 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 ساختار ext2_dir_entry *dirent,
			 __u32 *crc, ساختار ext2_dx_tail **ret_t)
{
	errcode_t retval;
	نویسه *buf = (نویسه *)dirent;
	صحیح size;
	__u32 gen, dummy_csum = 0;
	ساختار ext2_inode inode;
	ساختار ext2_dx_tail *t;
	ساختار ext2_dx_countlimit *c;
	صحیح شمار_offset, limit, شمار;

	retval = __get_dx_countlimit(fs, dirent, &c, &شمار_offset, 1);
	اگر (retval)
		بازگشت retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	شمار = ext2fs_le16_to_cpu(c->شمار);
	اگر (شمار_offset + (limit * اندازه(ساختار ext2_dx_entry)) >
	    fs->blocksize - اندازه(ساختار ext2_dx_tail))
		بازگشت EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (ساختار ext2_dx_tail *)(((ساختار ext2_dx_entry *)c) + limit);

	size = شمار_offset + (شمار * اندازه(ساختار ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	اگر (retval)
		بازگشت retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)&inum,
				اندازه(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)&gen, اندازه(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)&dummy_csum, 4);

	اگر (ret_t)
		*ret_t = t;
	بازگشت 0;
}

ایستا صحیح ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ساختار ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	ساختار ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	اگر (retval)
		بازگشت 0;

	بازگشت ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

ایستا errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    ساختار ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	ساختار ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	اگر (retval)
		بازگشت retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	بازگشت retval;
}

صحیح ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ساختار ext2_dir_entry *dirent)
{
	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 1;

	اگر (__get_dirent_tail(fs, dirent, پوچ, 1) == 0)
		بازگشت ext2fs_dirent_csum_verify(fs, inum, dirent);
	اگر (__get_dx_countlimit(fs, dirent, پوچ, پوچ, 1) == 0)
		بازگشت ext2fs_dx_csum_verify(fs, inum, dirent);

	بازگشت 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    ساختار ext2_dir_entry *dirent)
{
	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 0;

	اگر (__get_dirent_tail(fs, dirent, پوچ, 1) == 0)
		بازگشت ext2fs_dirent_csum_set(fs, inum, dirent);
	اگر (__get_dx_countlimit(fs, dirent, پوچ, پوچ, 1) == 0)
		بازگشت ext2fs_dx_csum_set(fs, inum, dirent);

	اگر (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		بازگشت 0;
	بازگشت EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(اندازه(ساختار ext3_extent_header) + \
	(اندازه(ساختار ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

ایستا ساختار ext3_extent_tail *get_extent_tail(ساختار ext3_extent_header *h)
{
	بازگشت (ساختار ext3_extent_tail *)(((نویسه *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

ایستا errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  ساختار ext3_extent_header *eh,
					  __u32 *crc)
{
	صحیح size;
	__u32 gen;
	errcode_t retval;
	ساختار ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + offsetof(ساختار ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	اگر (retval)
		بازگشت retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)&inum,
				اندازه(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)&gen, اندازه(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)eh, size);

	بازگشت 0;
}

صحیح ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ساختار ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	ساختار ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	اگر (retval)
		بازگشت 0;

	بازگشت provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       ساختار ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	ساختار ext3_extent_tail *t = get_extent_tail(eh);

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	اگر (retval)
		بازگشت retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	بازگشت retval;
}

صحیح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    نویسه *bitmap, صحیح size)
{
	ساختار ext4_group_desc *gdp = (ساختار ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)bitmap,
				      size);
	اگر (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	وگرنه
		calculated &= 0xFFFF;

	بازگشت provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       نویسه *bitmap, صحیح size)
{
	__u32 crc;
	ساختار ext4_group_desc *gdp = (ساختار ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	اگر (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	بازگشت 0;
}

صحیح ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    نویسه *bitmap, صحیح size)
{
	ساختار ext4_group_desc *gdp = (ساختار ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)bitmap,
				      size);
	اگر (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	وگرنه
		calculated &= 0xFFFF;

	بازگشت provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       نویسه *bitmap, صحیح size)
{
	__u32 crc;
	ساختار ext4_group_desc *gdp = (ساختار ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	اگر (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	بازگشت 0;
}

ایستا errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       ساختار ext2_inode_large *inode,
			       __u32 *crc, صحیح has_hi)
{
	__u32 gen;
	ساختار ext2_inode_large *desc = inode;
	size_t size = EXT2_INODE_SIZE(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	اگر (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)&inum,
				اندازه(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)&gen, اندازه(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned نویسه *)desc, size);

	inode->i_checksum_lo = old_lo;
	اگر (has_hi)
		inode->i_checksum_hi = old_hi;
	بازگشت 0;
}

صحیح ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     ساختار ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned صحیح i, has_hi;
	نویسه *cp;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	اگر (retval)
		بازگشت 0;
	اگر (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} وگرنه
		calculated &= 0xFFFF;

	اگر (provided == calculated)
		بازگشت 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	برای (cp = (نویسه *) inode, i = 0;
	     i < اندازه(ساختار ext2_inode);
	     cp++, i++)
		اگر (*cp)
			بازگشت 0;
	بازگشت 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   ساختار ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	صحیح has_hi;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	اگر (retval)
		بازگشت retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	اگر (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	بازگشت 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	ساختار ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef واژگان_BIGENDIAN
	ساختار ext4_group_desc swabdesc;
	size_t save_size = size;
	ثابت size_t ext4_bg_size = اندازه(ساختار ext4_group_desc);
	ساختار ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	اگر (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (ساختار ext2_group_desc *) &swabdesc);
	desc = (ساختار ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	اگر (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)&group,
					 اندازه(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned نویسه *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef واژگان_BIGENDIAN
		اگر (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned نویسه *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		برو out;
	}

	/* old crc16 code */
	offset = offsetof(ساختار ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   اندازه(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, اندازه(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += اندازه(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	اگر (offset < size) {
		crc = ext2fs_crc16(crc, (نویسه *)desc + offset,
				   size - offset);
	}
#ifdef واژگان_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	اگر (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (نویسه *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	بازگشت crc;
}

صحیح ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	اگر (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		بازگشت 0;

	بازگشت 1;
}

تهی ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	اگر (!ext2fs_has_group_desc_csum(fs))
		بازگشت;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

ایستا __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	برای (i = end_ino; i >= start_ino; i--) {
		اگر (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			بازگشت i - start_ino + 1;
	}
	بازگشت inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	ساختار ext2_super_block *sb = fs->super;
	صحیح dirty = 0;
	dgrp_t i;

	اگر (!fs->inode_map)
		بازگشت EXT2_ET_NO_INODE_BITMAP;

	اگر (!ext2fs_has_group_desc_csum(fs))
		بازگشت 0;

	برای (i = 0; i < fs->group_desc_شمار; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_شمار = ext2fs_bg_free_inodes_شمار(fs, i);
		__u32 old_free_blocks_شمار = ext2fs_bg_free_blocks_شمار(fs, i);

		اگر (old_free_blocks_شمار == sb->s_blocks_per_group &&
		    i != fs->group_desc_شمار - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		اگر (old_free_inodes_شمار == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} وگرنه {
			صحیح unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		اگر (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		اگر (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		اگر (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	اگر (dirty)
		ext2fs_mark_super_dirty(fs);
	بازگشت 0;
}

#ifdef DEBUG
#شامل "e2p/e2p.h"

تهی print_csum(ثابت نویسه *پیام, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	ساختار ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t size = EXT2_DESC_SIZE(fs->super);
	ساختار ext2_super_block *sb = fs->super;
	صحیح offset = offsetof(ساختار ext2_group_desc, bg_checksum);
#ifdef واژگان_BIGENDIAN
	ساختار ext4_group_desc swabdesc;
	ساختار ext2_group_desc *save_desc = desc;
	ثابت size_t ext4_bg_size = اندازه(ساختار ext4_group_desc);
	size_t save_size = size;
#endif

#ifdef واژگان_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	اگر (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (ساختار ext2_group_desc *) &swabdesc);
	desc = (ساختار ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#وگرنه
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, اندازه(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, اندازه(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += اندازه(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	اگر (offset < size)
		crc3 = ext2fs_crc16(crc3, (نویسه *)desc + offset, size - offset);
#ifdef واژگان_BIGENDIAN
	اگر (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (نویسه *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       پیام, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned نویسه sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

صحیح اصلی(صحیح argc, نویسه **argv)
{
	ساختار ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	صحیح			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, اندازه(param));
	ext2fs_blocks_شمار_set(&param, 32768);
#اگر 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	اگر (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	memcpy(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	برای (i=0; i < fs->group_desc_شمار; i++) {
		ext2fs_block_bitmap_loc_set(fs, i, 124);
		ext2fs_inode_bitmap_loc_set(fs, i, 125);
		ext2fs_inode_table_loc_set(fs, i, 126);
		ext2fs_bg_free_blocks_شمار_set(fs, i, 31119);
		ext2fs_bg_free_inodes_شمار_set(fs, i, 15701);
		ext2fs_bg_used_dirs_شمار_set(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	اگر (csum1 != csum_known) {
		printf("checksum for group 0 should be %04x\n", csum_known);
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	اگر (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	اگر (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	اگر (csum1 != csum2) {
		printf("checksums should not depend on checksum field\n");
		exit(1);
	}
	اگر (!ext2fs_group_desc_csum_verify(fs, 0)) {
		printf("checksums should verify against gd_checksum\n");
		exit(1);
	}
	memset(fs->super->s_uuid, 0x30, اندازه(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	اگر (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		printf("checksums for different filesystems shouldn't match\n");
		exit(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_شمار_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	اگر (csum1 == csum2) {
		printf("checksums for different data shouldn't match\n");
		exit(1);
	}
	ext2fs_free(fs);

	بازگشت 0;
}
#endif
