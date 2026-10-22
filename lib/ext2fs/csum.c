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

#অন্তর্ভাব "config.h"
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "crc16.h"
#অন্তর্ভাব <assert.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#অন্যথা
#define STATIC স্থির
#endif

শূন্য ext2fs_init_csum_seed(ext2_filsys fs)
{
	যদি (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	অন্যথা যদি (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 আকার(fs->super->s_uuid));
}

স্থির __u32 ext2fs_mmp_csum(ext2_filsys fs, কাঠামো mmp_struct *mmp)
{
	পূর্ণসংখ্যা offset = offsetof(কাঠামো mmp_struct, mmp_checksum);

	ফেরত ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)mmp, offset);
}

পূর্ণসংখ্যা ext2fs_mmp_csum_verify(ext2_filsys fs, কাঠামো mmp_struct *mmp)
{
	__u32 calculated;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	ফেরত ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, কাঠামো mmp_struct *mmp)
{
	__u32 crc;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	ফেরত 0;
}

পূর্ণসংখ্যা ext2fs_verify_csum_type(ext2_filsys fs, কাঠামো ext2_super_block *sb)
{
	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 1;

	ফেরত sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

স্থির __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    কাঠামো ext2_super_block *sb)
{
	পূর্ণসংখ্যা offset = offsetof(কাঠামো ext2_super_block, s_checksum);

	ফেরত ext2fs_crc32c_le(~0, (unsigned অক্ষর *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
পূর্ণসংখ্যা ext2fs_superblock_csum_verify(ext2_filsys fs, কাঠামো ext2_super_block *sb)
{
	__u32 flag, calculated;

	যদি (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	অন্যথা
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	যদি (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ফেরত 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	ফেরত ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     কাঠামো ext2_super_block *sb)
{
	__u32 flag, crc;

	যদি (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	অন্যথা
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	যদি (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ফেরত 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	ফেরত 0;
}

স্থির errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    কাঠামো ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	অক্ষর *buf = (অক্ষর *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)&block,
				আকার(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	ফেরত 0;
}

পূর্ণসংখ্যা ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      কাঠামো ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	যদি (retval)
		ফেরত 0;

	ফেরত ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 কাঠামো ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	যদি (retval)
		ফেরত retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	ফেরত 0;
}

স্থির __u16 do_nothing16(__u16 x)
{
	ফেরত x;
}

স্থির __u16 disk_to_host16(__u16 x)
{
	ফেরত ext2fs_le16_to_cpu(x);
}

স্থির errcode_t __get_dx_countlimit(ext2_filsys fs,
				     কাঠামো ext2_dir_entry *dirent,
				     কাঠামো ext2_dx_countlimit **cc,
				     পূর্ণসংখ্যা *offset,
				     পূর্ণসংখ্যা need_swab)
{
	কাঠামো ext2_dir_entry *dp;
	কাঠামো ext2_dx_মূল_info *মূল;
	কাঠামো ext2_dx_countlimit *c;
	পূর্ণসংখ্যা গুনতি_offset, max_sane_entries;
	unsigned পূর্ণসংখ্যা rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	যদি (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		গুনতি_offset = 8;
	অন্যথা যদি (rec_len == 12) {
		dp = (কাঠামো ext2_dir_entry *)(((অক্ষর *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		যদি (rec_len != fs->blocksize - 12)
			ফেরত EXT2_ET_DB_NOT_FOUND;
		মূল = (কাঠামো ext2_dx_মূল_info *)(((অক্ষর *)dp + 12));
		যদি (মূল->reserved_zero ||
		    মূল->info_দৈর্ঘ্য != আকার(কাঠামো ext2_dx_মূল_info))
			ফেরত EXT2_ET_DB_NOT_FOUND;
		গুনতি_offset = 32;
	} অন্যথা
		ফেরত EXT2_ET_DB_NOT_FOUND;

	c = (কাঠামো ext2_dx_countlimit *)(((অক্ষর *)dirent) + গুনতি_offset);
	max_sane_entries = (fs->blocksize - গুনতি_offset) /
			   আকার(কাঠামো ext2_dx_entry);
	যদি (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->গুনতি) > max_sane_entries)
		ফেরত EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	যদি (offset)
		*offset = গুনতি_offset;
	যদি (cc)
		*cc = c;

	ফেরত 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   কাঠামো ext2_dir_entry *dirent,
				   কাঠামো ext2_dx_countlimit **cc,
				   পূর্ণসংখ্যা *offset)
{
	ফেরত __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

শূন্য ext2fs_initialize_dirent_লেজ(ext2_filsys fs,
				   কাঠামো ext2_dir_entry_লেজ *t)
{
	memset(t, 0, আকার(কাঠামো ext2_dir_entry_লেজ));
	ext2fs_set_rec_len(fs, আকার(কাঠামো ext2_dir_entry_লেজ),
			   (কাঠামো ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

স্থির errcode_t __get_dirent_tail(ext2_filsys fs,
				   কাঠামো ext2_dir_entry *dirent,
				   কাঠামো ext2_dir_entry_লেজ **tt,
				   পূর্ণসংখ্যা need_swab)
{
	কাঠামো ext2_dir_entry *d;
	শূন্য *শীর্ষ;
	কাঠামো ext2_dir_entry_লেজ *t;
	unsigned পূর্ণসংখ্যা rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	যদি (fs->blocksize < 1024)
		ফেরত EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	শীর্ষ = EXT2_DIRENT_লেজ(dirent, fs->blocksize);

	যখন ((শূন্য *) d < শীর্ষ) {
		rec_len = translate(d->rec_len);
		যদি ((rec_len < 8) || (rec_len & 0x03))
			ফেরত EXT2_ET_DIR_CORRUPTED;
		d = (কাঠামো ext2_dir_entry *)(((অক্ষর *)d) + rec_len);
	}

	যদি ((অক্ষর *)d > ((অক্ষর *)dirent + fs->blocksize))
			ফেরত EXT2_ET_DIR_CORRUPTED;
	যদি (d != শীর্ষ)
		ফেরত EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (কাঠামো ext2_dir_entry_লেজ *)d;
	যদি (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != আকার(কাঠামো ext2_dir_entry_লেজ) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		ফেরত EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	যদি (tt)
		*tt = t;
	ফেরত retval;
}

পূর্ণসংখ্যা ext2fs_dirent_has_লেজ(ext2_filsys fs, কাঠামো ext2_dir_entry *dirent)
{
	ফেরত __get_dirent_tail(fs, dirent, ফাঁকা, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

স্থির errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    কাঠামো ext2_dir_entry *dirent, __u32 *crc,
				    পূর্ণসংখ্যা আকার_2)
{
	errcode_t retval;
	অক্ষর *buf = (অক্ষর *)dirent;
	__u32 gen;
	কাঠামো ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	যদি (retval)
		ফেরত retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)&inum,
				আকার(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)&gen, আকার(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)buf, আকার_2);

	ফেরত 0;
}

পূর্ণসংখ্যা ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      কাঠামো ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	কাঠামো ext2_dir_entry_লেজ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	যদি (retval)
		ফেরত 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (অক্ষর *)t - (অক্ষর *)dirent);
	যদি (retval)
		ফেরত 0;
	ফেরত ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

স্থির errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					কাঠামো ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	কাঠামো ext2_dir_entry_লেজ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	যদি (retval)
		ফেরত retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (অক্ষর *)t - (অক্ষর *)dirent);
	যদি (retval)
		ফেরত retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	ফেরত 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 কাঠামো ext2_dir_entry *dirent,
			 __u32 *crc, কাঠামো ext2_dx_লেজ **ret_t)
{
	errcode_t retval;
	অক্ষর *buf = (অক্ষর *)dirent;
	পূর্ণসংখ্যা আকার_2;
	__u32 gen, dummy_csum = 0;
	কাঠামো ext2_inode inode;
	কাঠামো ext2_dx_লেজ *t;
	কাঠামো ext2_dx_countlimit *c;
	পূর্ণসংখ্যা গুনতি_offset, limit, গুনতি;

	retval = __get_dx_countlimit(fs, dirent, &c, &গুনতি_offset, 1);
	যদি (retval)
		ফেরত retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	গুনতি = ext2fs_le16_to_cpu(c->গুনতি);
	যদি (গুনতি_offset + (limit * আকার(কাঠামো ext2_dx_entry)) >
	    fs->blocksize - আকার(কাঠামো ext2_dx_লেজ))
		ফেরত EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (কাঠামো ext2_dx_লেজ *)(((কাঠামো ext2_dx_entry *)c) + limit);

	আকার_2 = গুনতি_offset + (গুনতি * আকার(কাঠামো ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	যদি (retval)
		ফেরত retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)&inum,
				আকার(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)&gen, আকার(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)buf, আকার_2);
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)&dummy_csum, 4);

	যদি (ret_t)
		*ret_t = t;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 কাঠামো ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	কাঠামো ext2_dx_লেজ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	যদি (retval)
		ফেরত 0;

	ফেরত ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

স্থির errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    কাঠামো ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	কাঠামো ext2_dx_লেজ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	যদি (retval)
		ফেরত retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	ফেরত retval;
}

পূর্ণসংখ্যা ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 কাঠামো ext2_dir_entry *dirent)
{
	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 1;

	যদি (__get_dirent_tail(fs, dirent, ফাঁকা, 1) == 0)
		ফেরত ext2fs_dirent_csum_verify(fs, inum, dirent);
	যদি (__get_dx_countlimit(fs, dirent, ফাঁকা, ফাঁকা, 1) == 0)
		ফেরত ext2fs_dx_csum_verify(fs, inum, dirent);

	ফেরত 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    কাঠামো ext2_dir_entry *dirent)
{
	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 0;

	যদি (__get_dirent_tail(fs, dirent, ফাঁকা, 1) == 0)
		ফেরত ext2fs_dirent_csum_set(fs, inum, dirent);
	যদি (__get_dx_countlimit(fs, dirent, ফাঁকা, ফাঁকা, 1) == 0)
		ফেরত ext2fs_dx_csum_set(fs, inum, dirent);

	যদি (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		ফেরত 0;
	ফেরত EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_লেজ_OFFSET(hdr)	(আকার(কাঠামো ext3_extent_header) + \
	(আকার(কাঠামো ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

স্থির কাঠামো ext3_extent_লেজ *get_extent_লেজ(কাঠামো ext3_extent_header *h)
{
	ফেরত (কাঠামো ext3_extent_লেজ *)(((অক্ষর *)h) +
					   EXT3_EXTENT_লেজ_OFFSET(h));
}

স্থির errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  কাঠামো ext3_extent_header *eh,
					  __u32 *crc)
{
	পূর্ণসংখ্যা আকার_2;
	__u32 gen;
	errcode_t retval;
	কাঠামো ext2_inode inode;

	আকার_2 = EXT3_EXTENT_লেজ_OFFSET(eh) + offsetof(কাঠামো ext3_extent_লেজ,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	যদি (retval)
		ফেরত retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)&inum,
				আকার(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)&gen, আকার(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)eh, আকার_2);

	ফেরত 0;
}

পূর্ণসংখ্যা ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    কাঠামো ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	কাঠামো ext3_extent_লেজ *t = get_extent_লেজ(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	যদি (retval)
		ফেরত 0;

	ফেরত provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       কাঠামো ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	কাঠামো ext3_extent_লেজ *t = get_extent_লেজ(eh);

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	যদি (retval)
		ফেরত retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	ফেরত retval;
}

পূর্ণসংখ্যা ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    অক্ষর *bitmap, পূর্ণসংখ্যা আকার_2)
{
	কাঠামো ext4_group_desc *gdp = (কাঠামো ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)bitmap,
				      আকার_2);
	যদি (EXT2_DESC_আকার(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	অন্যথা
		calculated &= 0xFFFF;

	ফেরত provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       অক্ষর *bitmap, পূর্ণসংখ্যা আকার_2)
{
	__u32 crc;
	কাঠামো ext4_group_desc *gdp = (কাঠামো ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)bitmap, আকার_2);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	যদি (EXT2_DESC_আকার(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	ফেরত 0;
}

পূর্ণসংখ্যা ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    অক্ষর *bitmap, পূর্ণসংখ্যা আকার_2)
{
	কাঠামো ext4_group_desc *gdp = (কাঠামো ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)bitmap,
				      আকার_2);
	যদি (EXT2_DESC_আকার(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	অন্যথা
		calculated &= 0xFFFF;

	ফেরত provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       অক্ষর *bitmap, পূর্ণসংখ্যা আকার_2)
{
	__u32 crc;
	কাঠামো ext4_group_desc *gdp = (কাঠামো ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)bitmap, আকার_2);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	যদি (EXT2_DESC_আকার(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	ফেরত 0;
}

স্থির errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       কাঠামো ext2_inode_large *inode,
			       __u32 *crc, পূর্ণসংখ্যা has_hi)
{
	__u32 gen;
	কাঠামো ext2_inode_large *desc = inode;
	size_t আকার_2 = EXT2_INODE_আকার(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	যদি (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)&inum,
				আকার(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)&gen, আকার(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned অক্ষর *)desc, আকার_2);

	inode->i_checksum_lo = old_lo;
	যদি (has_hi)
		inode->i_checksum_hi = old_hi;
	ফেরত 0;
}

পূর্ণসংখ্যা ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     কাঠামো ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned পূর্ণসংখ্যা i, has_hi;
	অক্ষর *cp;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 1;

	has_hi = (EXT2_INODE_আকার(fs->super) > EXT2_GOOD_OLD_INODE_আকার &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	যদি (retval)
		ফেরত 0;
	যদি (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} অন্যথা
		calculated &= 0xFFFF;

	যদি (provided == calculated)
		ফেরত 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	জন্য (cp = (অক্ষর *) inode, i = 0;
	     i < আকার(কাঠামো ext2_inode);
	     cp++, i++)
		যদি (*cp)
			ফেরত 0;
	ফেরত 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   কাঠামো ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	পূর্ণসংখ্যা has_hi;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 0;

	has_hi = (EXT2_INODE_আকার(fs->super) > EXT2_GOOD_OLD_INODE_আকার &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	যদি (retval)
		ফেরত retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	যদি (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	ফেরত 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	কাঠামো ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, আকার_2 = EXT2_DESC_আকার(fs->super);
	__u16 crc = 0;
#ifdef শব্দগুলি_BIGENDIAN
	কাঠামো ext4_group_desc swabdesc;
	size_t save_আকার = আকার_2;
	ধ্রুবক size_t ext4_bg_আকার = আকার(কাঠামো ext4_group_desc);
	কাঠামো ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	যদি (আকার_2 > ext4_bg_আকার)
		আকার_2 = ext4_bg_আকার;
	memcpy(&swabdesc, desc, আকার_2);
	ext2fs_swap_group_desc2(fs, (কাঠামো ext2_group_desc *) &swabdesc);
	desc = (কাঠামো ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	যদি (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)&group,
					 আকার(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned অক্ষর *)desc,
					 আকার_2);
		desc->bg_checksum = old_crc;
#ifdef শব্দগুলি_BIGENDIAN
		যদি (save_আকার > ext4_bg_আকার)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned অক্ষর *)save_desc + ext4_bg_আকার,
	                             save_আকার - ext4_bg_আকার);
#endif
		crc = crc32 & 0xFFFF;
		যাও out;
	}

	/* old crc16 code */
	offset = offsetof(কাঠামো ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   আকার(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, আকার(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += আকার(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	যদি (offset < আকার_2) {
		crc = ext2fs_crc16(crc, (অক্ষর *)desc + offset,
				   আকার_2 - offset);
	}
#ifdef শব্দগুলি_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	যদি (save_আকার > ext4_bg_আকার)
		crc = ext2fs_crc16(crc, (অক্ষর *)save_desc + ext4_bg_আকার,
				   save_আকার - ext4_bg_আকার);
#endif

out:
	ফেরত crc;
}

পূর্ণসংখ্যা ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	যদি (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		ফেরত 0;

	ফেরত 1;
}

শূন্য ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	যদি (!ext2fs_has_group_desc_csum(fs))
		ফেরত;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

স্থির __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	জন্য (i = end_ino; i >= start_ino; i--) {
		যদি (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			ফেরত i - start_ino + 1;
	}
	ফেরত inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	কাঠামো ext2_super_block *sb = fs->super;
	পূর্ণসংখ্যা dirty = 0;
	dgrp_t i;

	যদি (!fs->inode_map)
		ফেরত EXT2_ET_NO_INODE_BITMAP;

	যদি (!ext2fs_has_group_desc_csum(fs))
		ফেরত 0;

	জন্য (i = 0; i < fs->group_desc_গুনতি; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_গুনতি = ext2fs_bg_free_inodes_গুনতি(fs, i);
		__u32 old_free_blocks_গুনতি = ext2fs_bg_free_blocks_গুনতি(fs, i);

		যদি (old_free_blocks_গুনতি == sb->s_blocks_per_group &&
		    i != fs->group_desc_গুনতি - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		যদি (old_free_inodes_গুনতি == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} অন্যথা {
			পূর্ণসংখ্যা unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		যদি (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		যদি (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		যদি (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	যদি (dirty)
		ext2fs_mark_super_dirty(fs);
	ফেরত 0;
}

#ifdef DEBUG
#অন্তর্ভাব "e2p/e2p.h"

শূন্য print_csum(ধ্রুবক অক্ষর *বার্তা, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	কাঠামো ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t আকার_2 = EXT2_DESC_আকার(fs->super);
	কাঠামো ext2_super_block *sb = fs->super;
	পূর্ণসংখ্যা offset = offsetof(কাঠামো ext2_group_desc, bg_checksum);
#ifdef শব্দগুলি_BIGENDIAN
	কাঠামো ext4_group_desc swabdesc;
	কাঠামো ext2_group_desc *save_desc = desc;
	ধ্রুবক size_t ext4_bg_আকার = আকার(কাঠামো ext4_group_desc);
	size_t save_আকার = আকার_2;
#endif

#ifdef শব্দগুলি_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	যদি (আকার_2 > ext4_bg_আকার)
		আকার_2 = ext4_bg_আকার;
	memcpy(&swabdesc, desc, আকার_2);
	ext2fs_swap_group_desc2(fs, (কাঠামো ext2_group_desc *) &swabdesc);
	desc = (কাঠামো ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#অন্যথা
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, আকার(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, আকার(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += আকার(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	যদি (offset < আকার_2)
		crc3 = ext2fs_crc16(crc3, (অক্ষর *)desc + offset, আকার_2 - offset);
#ifdef শব্দগুলি_BIGENDIAN
	যদি (save_আকার > ext4_bg_আকার)
		crc3 = ext2fs_crc16(crc3, (অক্ষর *)save_desc + ext4_bg_আকার,
				    save_আকার - ext4_bg_আকার);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       বার্তা, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned অক্ষর sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

পূর্ণসংখ্যা প্রধান(পূর্ণসংখ্যা argc, অক্ষর **argv)
{
	কাঠামো ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	পূর্ণসংখ্যা			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, আকার(param));
	ext2fs_blocks_গুনতি_set(&param, 32768);
#যদি 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_আকার = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	যদি (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	memcpy(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	জন্য (i=0; i < fs->group_desc_গুনতি; i++) {
		ext2fs_block_bitmap_loc_set(fs, i, 124);
		ext2fs_inode_bitmap_loc_set(fs, i, 125);
		ext2fs_inode_table_loc_set(fs, i, 126);
		ext2fs_bg_free_blocks_গুনতি_set(fs, i, 31119);
		ext2fs_bg_free_inodes_গুনতি_set(fs, i, 15701);
		ext2fs_bg_used_dirs_গুনতি_set(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	যদি (csum1 != csum_known) {
		printf("checksum for group 0 should be %04x\n", csum_known);
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	যদি (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	যদি (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	যদি (csum1 != csum2) {
		printf("checksums should not depend on checksum field\n");
		exit(1);
	}
	যদি (!ext2fs_group_desc_csum_verify(fs, 0)) {
		printf("checksums should verify against gd_checksum\n");
		exit(1);
	}
	memset(fs->super->s_uuid, 0x30, আকার(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	যদি (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		printf("checksums for different filesystems shouldn't match\n");
		exit(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_গুনতি_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	যদি (csum1 == csum2) {
		printf("checksums for different data shouldn't match\n");
		exit(1);
	}
	ext2fs_free(fs);

	ফেরত 0;
}
#endif
