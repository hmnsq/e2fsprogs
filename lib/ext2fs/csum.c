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
						 পরিমাণ(fs->super->s_uuid));
}

স্থির __u32 ext2fs_mmp_csum(ext2_filsys fs, সংরচনা mmp_struct *mmp)
{
	পূর্ণাঙ্ক offset = offsetof(সংরচনা mmp_struct, mmp_checksum);

	প্রত্যাগম ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)mmp, offset);
}

পূর্ণাঙ্ক ext2fs_mmp_csum_verify(ext2_filsys fs, সংরচনা mmp_struct *mmp)
{
	__u32 calculated;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	প্রত্যাগম ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, সংরচনা mmp_struct *mmp)
{
	__u32 crc;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	প্রত্যাগম 0;
}

পূর্ণাঙ্ক ext2fs_verify_csum_type(ext2_filsys fs, সংরচনা ext2_super_block *sb)
{
	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 1;

	প্রত্যাগম sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

স্থির __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    সংরচনা ext2_super_block *sb)
{
	পূর্ণাঙ্ক offset = offsetof(সংরচনা ext2_super_block, s_checksum);

	প্রত্যাগম ext2fs_crc32c_le(~0, (unsigned বর্ণ *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
পূর্ণাঙ্ক ext2fs_superblock_csum_verify(ext2_filsys fs, সংরচনা ext2_super_block *sb)
{
	__u32 flag, calculated;

	যদি (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	অন্যথা
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	যদি (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		প্রত্যাগম 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	প্রত্যাগম ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     সংরচনা ext2_super_block *sb)
{
	__u32 flag, crc;

	যদি (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	অন্যথা
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	যদি (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		প্রত্যাগম 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	প্রত্যাগম 0;
}

স্থির errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    সংরচনা ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	বর্ণ *buf = (বর্ণ *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)&block,
				পরিমাণ(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	প্রত্যাগম 0;
}

পূর্ণাঙ্ক ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      সংরচনা ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	যদি (retval)
		প্রত্যাগম 0;

	প্রত্যাগম ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 সংরচনা ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	যদি (retval)
		প্রত্যাগম retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	প্রত্যাগম 0;
}

স্থির __u16 do_nothing16(__u16 x)
{
	প্রত্যাগম x;
}

স্থির __u16 disk_to_host16(__u16 x)
{
	প্রত্যাগম ext2fs_le16_to_cpu(x);
}

স্থির errcode_t __get_dx_countlimit(ext2_filsys fs,
				     সংরচনা ext2_dir_entry *dirent,
				     সংরচনা ext2_dx_countlimit **cc,
				     পূর্ণাঙ্ক *offset,
				     পূর্ণাঙ্ক need_swab)
{
	সংরচনা ext2_dir_entry *dp;
	সংরচনা ext2_dx_মূল_info *মূল;
	সংরচনা ext2_dx_countlimit *c;
	পূর্ণাঙ্ক গুনতি_offset, max_sane_entries;
	unsigned পূর্ণাঙ্ক rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	যদি (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		গুনতি_offset = 8;
	অন্যথা যদি (rec_len == 12) {
		dp = (সংরচনা ext2_dir_entry *)(((বর্ণ *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		যদি (rec_len != fs->blocksize - 12)
			প্রত্যাগম EXT2_ET_DB_NOT_FOUND;
		মূল = (সংরচনা ext2_dx_মূল_info *)(((বর্ণ *)dp + 12));
		যদি (মূল->reserved_zero ||
		    মূল->info_length != পরিমাণ(সংরচনা ext2_dx_মূল_info))
			প্রত্যাগম EXT2_ET_DB_NOT_FOUND;
		গুনতি_offset = 32;
	} অন্যথা
		প্রত্যাগম EXT2_ET_DB_NOT_FOUND;

	c = (সংরচনা ext2_dx_countlimit *)(((বর্ণ *)dirent) + গুনতি_offset);
	max_sane_entries = (fs->blocksize - গুনতি_offset) /
			   পরিমাণ(সংরচনা ext2_dx_entry);
	যদি (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->গুনতি) > max_sane_entries)
		প্রত্যাগম EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	যদি (offset)
		*offset = গুনতি_offset;
	যদি (cc)
		*cc = c;

	প্রত্যাগম 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   সংরচনা ext2_dir_entry *dirent,
				   সংরচনা ext2_dx_countlimit **cc,
				   পূর্ণাঙ্ক *offset)
{
	প্রত্যাগম __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

শূন্য ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   সংরচনা ext2_dir_entry_tail *t)
{
	memset(t, 0, পরিমাণ(সংরচনা ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, পরিমাণ(সংরচনা ext2_dir_entry_tail),
			   (সংরচনা ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

স্থির errcode_t __get_dirent_tail(ext2_filsys fs,
				   সংরচনা ext2_dir_entry *dirent,
				   সংরচনা ext2_dir_entry_tail **tt,
				   পূর্ণাঙ্ক need_swab)
{
	সংরচনা ext2_dir_entry *d;
	শূন্য *শীর্ষ;
	সংরচনা ext2_dir_entry_tail *t;
	unsigned পূর্ণাঙ্ক rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	যদি (fs->blocksize < 1024)
		প্রত্যাগম EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	শীর্ষ = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	যাবত্ ((শূন্য *) d < শীর্ষ) {
		rec_len = translate(d->rec_len);
		যদি ((rec_len < 8) || (rec_len & 0x03))
			প্রত্যাগম EXT2_ET_DIR_CORRUPTED;
		d = (সংরচনা ext2_dir_entry *)(((বর্ণ *)d) + rec_len);
	}

	যদি ((বর্ণ *)d > ((বর্ণ *)dirent + fs->blocksize))
			প্রত্যাগম EXT2_ET_DIR_CORRUPTED;
	যদি (d != শীর্ষ)
		প্রত্যাগম EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (সংরচনা ext2_dir_entry_tail *)d;
	যদি (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != পরিমাণ(সংরচনা ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		প্রত্যাগম EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	যদি (tt)
		*tt = t;
	প্রত্যাগম retval;
}

পূর্ণাঙ্ক ext2fs_dirent_has_tail(ext2_filsys fs, সংরচনা ext2_dir_entry *dirent)
{
	প্রত্যাগম __get_dirent_tail(fs, dirent, রিক্ত, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

স্থির errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    সংরচনা ext2_dir_entry *dirent, __u32 *crc,
				    পূর্ণাঙ্ক size)
{
	errcode_t retval;
	বর্ণ *buf = (বর্ণ *)dirent;
	__u32 gen;
	সংরচনা ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	যদি (retval)
		প্রত্যাগম retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)&inum,
				পরিমাণ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)&gen, পরিমাণ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)buf, size);

	প্রত্যাগম 0;
}

পূর্ণাঙ্ক ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      সংরচনা ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	সংরচনা ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	যদি (retval)
		প্রত্যাগম 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (বর্ণ *)t - (বর্ণ *)dirent);
	যদি (retval)
		প্রত্যাগম 0;
	প্রত্যাগম ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

স্থির errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					সংরচনা ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	সংরচনা ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	যদি (retval)
		প্রত্যাগম retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (বর্ণ *)t - (বর্ণ *)dirent);
	যদি (retval)
		প্রত্যাগম retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	প্রত্যাগম 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 সংরচনা ext2_dir_entry *dirent,
			 __u32 *crc, সংরচনা ext2_dx_tail **ret_t)
{
	errcode_t retval;
	বর্ণ *buf = (বর্ণ *)dirent;
	পূর্ণাঙ্ক size;
	__u32 gen, dummy_csum = 0;
	সংরচনা ext2_inode inode;
	সংরচনা ext2_dx_tail *t;
	সংরচনা ext2_dx_countlimit *c;
	পূর্ণাঙ্ক গুনতি_offset, limit, গুনতি;

	retval = __get_dx_countlimit(fs, dirent, &c, &গুনতি_offset, 1);
	যদি (retval)
		প্রত্যাগম retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	গুনতি = ext2fs_le16_to_cpu(c->গুনতি);
	যদি (গুনতি_offset + (limit * পরিমাণ(সংরচনা ext2_dx_entry)) >
	    fs->blocksize - পরিমাণ(সংরচনা ext2_dx_tail))
		প্রত্যাগম EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (সংরচনা ext2_dx_tail *)(((সংরচনা ext2_dx_entry *)c) + limit);

	size = গুনতি_offset + (গুনতি * পরিমাণ(সংরচনা ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	যদি (retval)
		প্রত্যাগম retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)&inum,
				পরিমাণ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)&gen, পরিমাণ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)&dummy_csum, 4);

	যদি (ret_t)
		*ret_t = t;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 সংরচনা ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	সংরচনা ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	যদি (retval)
		প্রত্যাগম 0;

	প্রত্যাগম ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

স্থির errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    সংরচনা ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	সংরচনা ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	যদি (retval)
		প্রত্যাগম retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	প্রত্যাগম retval;
}

পূর্ণাঙ্ক ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 সংরচনা ext2_dir_entry *dirent)
{
	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 1;

	যদি (__get_dirent_tail(fs, dirent, রিক্ত, 1) == 0)
		প্রত্যাগম ext2fs_dirent_csum_verify(fs, inum, dirent);
	যদি (__get_dx_countlimit(fs, dirent, রিক্ত, রিক্ত, 1) == 0)
		প্রত্যাগম ext2fs_dx_csum_verify(fs, inum, dirent);

	প্রত্যাগম 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    সংরচনা ext2_dir_entry *dirent)
{
	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 0;

	যদি (__get_dirent_tail(fs, dirent, রিক্ত, 1) == 0)
		প্রত্যাগম ext2fs_dirent_csum_set(fs, inum, dirent);
	যদি (__get_dx_countlimit(fs, dirent, রিক্ত, রিক্ত, 1) == 0)
		প্রত্যাগম ext2fs_dx_csum_set(fs, inum, dirent);

	যদি (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		প্রত্যাগম 0;
	প্রত্যাগম EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(পরিমাণ(সংরচনা ext3_extent_header) + \
	(পরিমাণ(সংরচনা ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

স্থির সংরচনা ext3_extent_tail *get_extent_tail(সংরচনা ext3_extent_header *h)
{
	প্রত্যাগম (সংরচনা ext3_extent_tail *)(((বর্ণ *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

স্থির errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  সংরচনা ext3_extent_header *eh,
					  __u32 *crc)
{
	পূর্ণাঙ্ক size;
	__u32 gen;
	errcode_t retval;
	সংরচনা ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + offsetof(সংরচনা ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	যদি (retval)
		প্রত্যাগম retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)&inum,
				পরিমাণ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)&gen, পরিমাণ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)eh, size);

	প্রত্যাগম 0;
}

পূর্ণাঙ্ক ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    সংরচনা ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	সংরচনা ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	যদি (retval)
		প্রত্যাগম 0;

	প্রত্যাগম provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       সংরচনা ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	সংরচনা ext3_extent_tail *t = get_extent_tail(eh);

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	যদি (retval)
		প্রত্যাগম retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	প্রত্যাগম retval;
}

পূর্ণাঙ্ক ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    বর্ণ *bitmap, পূর্ণাঙ্ক size)
{
	সংরচনা ext4_group_desc *gdp = (সংরচনা ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)bitmap,
				      size);
	যদি (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	অন্যথা
		calculated &= 0xFFFF;

	প্রত্যাগম provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       বর্ণ *bitmap, পূর্ণাঙ্ক size)
{
	__u32 crc;
	সংরচনা ext4_group_desc *gdp = (সংরচনা ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	যদি (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	প্রত্যাগম 0;
}

পূর্ণাঙ্ক ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    বর্ণ *bitmap, পূর্ণাঙ্ক size)
{
	সংরচনা ext4_group_desc *gdp = (সংরচনা ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)bitmap,
				      size);
	যদি (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	অন্যথা
		calculated &= 0xFFFF;

	প্রত্যাগম provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       বর্ণ *bitmap, পূর্ণাঙ্ক size)
{
	__u32 crc;
	সংরচনা ext4_group_desc *gdp = (সংরচনা ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	যদি (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	প্রত্যাগম 0;
}

স্থির errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       সংরচনা ext2_inode_large *inode,
			       __u32 *crc, পূর্ণাঙ্ক has_hi)
{
	__u32 gen;
	সংরচনা ext2_inode_large *desc = inode;
	size_t size = EXT2_INODE_SIZE(fs->super);
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
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)&inum,
				পরিমাণ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)&gen, পরিমাণ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned বর্ণ *)desc, size);

	inode->i_checksum_lo = old_lo;
	যদি (has_hi)
		inode->i_checksum_hi = old_hi;
	প্রত্যাগম 0;
}

পূর্ণাঙ্ক ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     সংরচনা ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned পূর্ণাঙ্ক i, has_hi;
	বর্ণ *cp;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	যদি (retval)
		প্রত্যাগম 0;
	যদি (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} অন্যথা
		calculated &= 0xFFFF;

	যদি (provided == calculated)
		প্রত্যাগম 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	প্রতি (cp = (বর্ণ *) inode, i = 0;
	     i < পরিমাণ(সংরচনা ext2_inode);
	     cp++, i++)
		যদি (*cp)
			প্রত্যাগম 0;
	প্রত্যাগম 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   সংরচনা ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	পূর্ণাঙ্ক has_hi;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		প্রত্যাগম 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	যদি (retval)
		প্রত্যাগম retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	যদি (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	প্রত্যাগম 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	সংরচনা ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef শব্দগুলি_BIGENDIAN
	সংরচনা ext4_group_desc swabdesc;
	size_t save_size = size;
	ধ্রুব size_t ext4_bg_size = পরিমাণ(সংরচনা ext4_group_desc);
	সংরচনা ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	যদি (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (সংরচনা ext2_group_desc *) &swabdesc);
	desc = (সংরচনা ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	যদি (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned বর্ণ *)&group,
					 পরিমাণ(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned বর্ণ *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef শব্দগুলি_BIGENDIAN
		যদি (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned বর্ণ *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		গচ্ছ out;
	}

	/* old crc16 code */
	offset = offsetof(সংরচনা ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   পরিমাণ(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, পরিমাণ(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += পরিমাণ(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	যদি (offset < size) {
		crc = ext2fs_crc16(crc, (বর্ণ *)desc + offset,
				   size - offset);
	}
#ifdef শব্দগুলি_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	যদি (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (বর্ণ *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	প্রত্যাগম crc;
}

পূর্ণাঙ্ক ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	যদি (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		প্রত্যাগম 0;

	প্রত্যাগম 1;
}

শূন্য ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	যদি (!ext2fs_has_group_desc_csum(fs))
		প্রত্যাগম;

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

	প্রতি (i = end_ino; i >= start_ino; i--) {
		যদি (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			প্রত্যাগম i - start_ino + 1;
	}
	প্রত্যাগম inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	সংরচনা ext2_super_block *sb = fs->super;
	পূর্ণাঙ্ক dirty = 0;
	dgrp_t i;

	যদি (!fs->inode_map)
		প্রত্যাগম EXT2_ET_NO_INODE_BITMAP;

	যদি (!ext2fs_has_group_desc_csum(fs))
		প্রত্যাগম 0;

	প্রতি (i = 0; i < fs->group_desc_গুনতি; i++) {
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
			পূর্ণাঙ্ক unused =
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
	প্রত্যাগম 0;
}

#ifdef DEBUG
#অন্তর্ভাব "e2p/e2p.h"

শূন্য print_csum(ধ্রুব বর্ণ *বার্তা, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	সংরচনা ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t size = EXT2_DESC_SIZE(fs->super);
	সংরচনা ext2_super_block *sb = fs->super;
	পূর্ণাঙ্ক offset = offsetof(সংরচনা ext2_group_desc, bg_checksum);
#ifdef শব্দগুলি_BIGENDIAN
	সংরচনা ext4_group_desc swabdesc;
	সংরচনা ext2_group_desc *save_desc = desc;
	ধ্রুব size_t ext4_bg_size = পরিমাণ(সংরচনা ext4_group_desc);
	size_t save_size = size;
#endif

#ifdef শব্দগুলি_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	যদি (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (সংরচনা ext2_group_desc *) &swabdesc);
	desc = (সংরচনা ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#অন্যথা
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, পরিমাণ(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, পরিমাণ(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += পরিমাণ(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	যদি (offset < size)
		crc3 = ext2fs_crc16(crc3, (বর্ণ *)desc + offset, size - offset);
#ifdef শব্দগুলি_BIGENDIAN
	যদি (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (বর্ণ *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       বার্তা, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned বর্ণ sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, বর্ণ **argv)
{
	সংরচনা ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	পূর্ণাঙ্ক			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, পরিমাণ(param));
	ext2fs_blocks_গুনতি_set(&param, 32768);
#যদি 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
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

	প্রতি (i=0; i < fs->group_desc_গুনতি; i++) {
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
	memset(fs->super->s_uuid, 0x30, পরিমাণ(fs->super->s_uuid));
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

	প্রত্যাগম 0;
}
#endif
