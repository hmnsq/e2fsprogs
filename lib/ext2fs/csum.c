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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "crc16.h"
#ಅನ್ತರ್ಭಾವ <assert.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#ಅನ್ಯಥಾ
#define STATIC ಸ್ಥಿರ
#endif

ಶೂನ್ಯ ext2fs_init_csum_seed(ext2_filsys fs)
{
	ಯದಿ (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	ಅನ್ಯಥಾ ಯದಿ (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 ಪರಿಮಾಣ(fs->super->s_uuid));
}

ಸ್ಥಿರ __u32 ext2fs_mmp_csum(ext2_filsys fs, ಸಂರಚನಾ mmp_struct *mmp)
{
	ಪೂರ್ಣಾಙ್ಕ offset = offsetof(ಸಂರಚನಾ mmp_struct, mmp_checksum);

	ಪ್ರತ್ಯಾಗಮ ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)mmp, offset);
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_mmp_csum_verify(ext2_filsys fs, ಸಂರಚನಾ mmp_struct *mmp)
{
	__u32 calculated;

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	ಪ್ರತ್ಯಾಗಮ ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, ಸಂರಚನಾ mmp_struct *mmp)
{
	__u32 crc;

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_verify_csum_type(ext2_filsys fs, ಸಂರಚನಾ ext2_super_block *sb)
{
	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತ್ಯಾಗಮ sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

ಸ್ಥಿರ __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    ಸಂರಚನಾ ext2_super_block *sb)
{
	ಪೂರ್ಣಾಙ್ಕ offset = offsetof(ಸಂರಚನಾ ext2_super_block, s_checksum);

	ಪ್ರತ್ಯಾಗಮ ext2fs_crc32c_le(~0, (unsigned ವರ್ಣ *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
ಪೂರ್ಣಾಙ್ಕ ext2fs_superblock_csum_verify(ext2_filsys fs, ಸಂರಚನಾ ext2_super_block *sb)
{
	__u32 flag, calculated;

	ಯದಿ (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ಅನ್ಯಥಾ
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ಯದಿ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ಪ್ರತ್ಯಾಗಮ 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	ಪ್ರತ್ಯಾಗಮ ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     ಸಂರಚನಾ ext2_super_block *sb)
{
	__u32 flag, crc;

	ಯದಿ (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ಅನ್ಯಥಾ
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ಯದಿ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ಪ್ರತ್ಯಾಗಮ 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    ಸಂರಚನಾ ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	ವರ್ಣ *buf = (ವರ್ಣ *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)&block,
				ಪರಿಮಾಣ(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      ಸಂರಚನಾ ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 ಸಂರಚನಾ ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ __u16 do_nothing16(__u16 x)
{
	ಪ್ರತ್ಯಾಗಮ x;
}

ಸ್ಥಿರ __u16 disk_to_host16(__u16 x)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_le16_to_cpu(x);
}

ಸ್ಥಿರ errcode_t __get_dx_countlimit(ext2_filsys fs,
				     ಸಂರಚನಾ ext2_dir_entry *dirent,
				     ಸಂರಚನಾ ext2_dx_countlimit **cc,
				     ಪೂರ್ಣಾಙ್ಕ *offset,
				     ಪೂರ್ಣಾಙ್ಕ need_swab)
{
	ಸಂರಚನಾ ext2_dir_entry *dp;
	ಸಂರಚನಾ ext2_dx_ಬೇರು_info *ಬೇರು;
	ಸಂರಚನಾ ext2_dx_countlimit *c;
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_offset, max_sane_entries;
	unsigned ಪೂರ್ಣಾಙ್ಕ rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	ಯದಿ (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		ಎಣಿಕೆ_offset = 8;
	ಅನ್ಯಥಾ ಯದಿ (rec_len == 12) {
		dp = (ಸಂರಚನಾ ext2_dir_entry *)(((ವರ್ಣ *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		ಯದಿ (rec_len != fs->blocksize - 12)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_DB_NOT_FOUND;
		ಬೇರು = (ಸಂರಚನಾ ext2_dx_ಬೇರು_info *)(((ವರ್ಣ *)dp + 12));
		ಯದಿ (ಬೇರು->reserved_zero ||
		    ಬೇರು->info_length != ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_ಬೇರು_info))
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_DB_NOT_FOUND;
		ಎಣಿಕೆ_offset = 32;
	} ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DB_NOT_FOUND;

	c = (ಸಂರಚನಾ ext2_dx_countlimit *)(((ವರ್ಣ *)dirent) + ಎಣಿಕೆ_offset);
	max_sane_entries = (fs->blocksize - ಎಣಿಕೆ_offset) /
			   ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_entry);
	ಯದಿ (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->ಎಣಿಕೆ) > max_sane_entries)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ಯದಿ (offset)
		*offset = ಎಣಿಕೆ_offset;
	ಯದಿ (cc)
		*cc = c;

	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   ಸಂರಚನಾ ext2_dir_entry *dirent,
				   ಸಂರಚನಾ ext2_dx_countlimit **cc,
				   ಪೂರ್ಣಾಙ್ಕ *offset)
{
	ಪ್ರತ್ಯಾಗಮ __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

ಶೂನ್ಯ ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   ಸಂರಚನಾ ext2_dir_entry_tail *t)
{
	memset(t, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dir_entry_tail),
			   (ಸಂರಚನಾ ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ಸ್ಥಿರ errcode_t __get_dirent_tail(ext2_filsys fs,
				   ಸಂರಚನಾ ext2_dir_entry *dirent,
				   ಸಂರಚನಾ ext2_dir_entry_tail **tt,
				   ಪೂರ್ಣಾಙ್ಕ need_swab)
{
	ಸಂರಚನಾ ext2_dir_entry *d;
	ಶೂನ್ಯ *ತುದಿ;
	ಸಂರಚನಾ ext2_dir_entry_tail *t;
	unsigned ಪೂರ್ಣಾಙ್ಕ rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	ಯದಿ (fs->blocksize < 1024)
		ಪ್ರತ್ಯಾಗಮ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	ತುದಿ = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	ಯಾವತ್ ((ಶೂನ್ಯ *) d < ತುದಿ) {
		rec_len = translate(d->rec_len);
		ಯದಿ ((rec_len < 8) || (rec_len & 0x03))
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_CORRUPTED;
		d = (ಸಂರಚನಾ ext2_dir_entry *)(((ವರ್ಣ *)d) + rec_len);
	}

	ಯದಿ ((ವರ್ಣ *)d > ((ವರ್ಣ *)dirent + fs->blocksize))
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_CORRUPTED;
	ಯದಿ (d != ತುದಿ)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (ಸಂರಚನಾ ext2_dir_entry_tail *)d;
	ಯದಿ (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ಯದಿ (tt)
		*tt = t;
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_dirent_has_tail(ext2_filsys fs, ಸಂರಚನಾ ext2_dir_entry *dirent)
{
	ಪ್ರತ್ಯಾಗಮ __get_dirent_tail(fs, dirent, ರಿಕ್ತ, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ಸ್ಥಿರ errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    ಸಂರಚನಾ ext2_dir_entry *dirent, __u32 *crc,
				    ಪೂರ್ಣಾಙ್ಕ size)
{
	errcode_t retval;
	ವರ್ಣ *buf = (ವರ್ಣ *)dirent;
	__u32 gen;
	ಸಂರಚನಾ ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)&inum,
				ಪರಿಮಾಣ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)&gen, ಪರಿಮಾಣ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)buf, size);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      ಸಂರಚನಾ ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	ಸಂರಚನಾ ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (ವರ್ಣ *)t - (ವರ್ಣ *)dirent);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

ಸ್ಥಿರ errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					ಸಂರಚನಾ ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	ಸಂರಚನಾ ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (ವರ್ಣ *)t - (ವರ್ಣ *)dirent);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 ಸಂರಚನಾ ext2_dir_entry *dirent,
			 __u32 *crc, ಸಂರಚನಾ ext2_dx_tail **ret_t)
{
	errcode_t retval;
	ವರ್ಣ *buf = (ವರ್ಣ *)dirent;
	ಪೂರ್ಣಾಙ್ಕ size;
	__u32 gen, dummy_csum = 0;
	ಸಂರಚನಾ ext2_inode inode;
	ಸಂರಚನಾ ext2_dx_tail *t;
	ಸಂರಚನಾ ext2_dx_countlimit *c;
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_offset, limit, ಎಣಿಕೆ;

	retval = __get_dx_countlimit(fs, dirent, &c, &ಎಣಿಕೆ_offset, 1);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	ಎಣಿಕೆ = ext2fs_le16_to_cpu(c->ಎಣಿಕೆ);
	ಯದಿ (ಎಣಿಕೆ_offset + (limit * ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_entry)) >
	    fs->blocksize - ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_tail))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (ಸಂರಚನಾ ext2_dx_tail *)(((ಸಂರಚನಾ ext2_dx_entry *)c) + limit);

	size = ಎಣಿಕೆ_offset + (ಎಣಿಕೆ * ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)&inum,
				ಪರಿಮಾಣ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)&gen, ಪರಿಮಾಣ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)&dummy_csum, 4);

	ಯದಿ (ret_t)
		*ret_t = t;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ಸಂರಚನಾ ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	ಸಂರಚನಾ ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

ಸ್ಥಿರ errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    ಸಂರಚನಾ ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	ಸಂರಚನಾ ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ಸಂರಚನಾ ext2_dir_entry *dirent)
{
	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 1;

	ಯದಿ (__get_dirent_tail(fs, dirent, ರಿಕ್ತ, 1) == 0)
		ಪ್ರತ್ಯಾಗಮ ext2fs_dirent_csum_verify(fs, inum, dirent);
	ಯದಿ (__get_dx_countlimit(fs, dirent, ರಿಕ್ತ, ರಿಕ್ತ, 1) == 0)
		ಪ್ರತ್ಯಾಗಮ ext2fs_dx_csum_verify(fs, inum, dirent);

	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    ಸಂರಚನಾ ext2_dir_entry *dirent)
{
	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (__get_dirent_tail(fs, dirent, ರಿಕ್ತ, 1) == 0)
		ಪ್ರತ್ಯಾಗಮ ext2fs_dirent_csum_set(fs, inum, dirent);
	ಯದಿ (__get_dx_countlimit(fs, dirent, ರಿಕ್ತ, ರಿಕ್ತ, 1) == 0)
		ಪ್ರತ್ಯಾಗಮ ext2fs_dx_csum_set(fs, inum, dirent);

	ಯದಿ (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(ಪರಿಮಾಣ(ಸಂರಚನಾ ext3_extent_header) + \
	(ಪರಿಮಾಣ(ಸಂರಚನಾ ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

ಸ್ಥಿರ ಸಂರಚನಾ ext3_extent_tail *get_extent_tail(ಸಂರಚನಾ ext3_extent_header *h)
{
	ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ ext3_extent_tail *)(((ವರ್ಣ *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

ಸ್ಥಿರ errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  ಸಂರಚನಾ ext3_extent_header *eh,
					  __u32 *crc)
{
	ಪೂರ್ಣಾಙ್ಕ size;
	__u32 gen;
	errcode_t retval;
	ಸಂರಚನಾ ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + offsetof(ಸಂರಚನಾ ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)&inum,
				ಪರಿಮಾಣ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)&gen, ಪರಿಮಾಣ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)eh, size);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ಸಂರಚನಾ ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	ಸಂರಚನಾ ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       ಸಂರಚನಾ ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	ಸಂರಚನಾ ext3_extent_tail *t = get_extent_tail(eh);

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    ವರ್ಣ *bitmap, ಪೂರ್ಣಾಙ್ಕ size)
{
	ಸಂರಚನಾ ext4_group_desc *gdp = (ಸಂರಚನಾ ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)bitmap,
				      size);
	ಯದಿ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	ಅನ್ಯಥಾ
		calculated &= 0xFFFF;

	ಪ್ರತ್ಯಾಗಮ provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       ವರ್ಣ *bitmap, ಪೂರ್ಣಾಙ್ಕ size)
{
	__u32 crc;
	ಸಂರಚನಾ ext4_group_desc *gdp = (ಸಂರಚನಾ ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	ಯದಿ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    ವರ್ಣ *bitmap, ಪೂರ್ಣಾಙ್ಕ size)
{
	ಸಂರಚನಾ ext4_group_desc *gdp = (ಸಂರಚನಾ ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)bitmap,
				      size);
	ಯದಿ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	ಅನ್ಯಥಾ
		calculated &= 0xFFFF;

	ಪ್ರತ್ಯಾಗಮ provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       ವರ್ಣ *bitmap, ಪೂರ್ಣಾಙ್ಕ size)
{
	__u32 crc;
	ಸಂರಚನಾ ext4_group_desc *gdp = (ಸಂರಚನಾ ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	ಯದಿ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       ಸಂರಚನಾ ext2_inode_large *inode,
			       __u32 *crc, ಪೂರ್ಣಾಙ್ಕ has_hi)
{
	__u32 gen;
	ಸಂರಚನಾ ext2_inode_large *desc = inode;
	size_t size = EXT2_INODE_SIZE(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	ಯದಿ (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)&inum,
				ಪರಿಮಾಣ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)&gen, ಪರಿಮಾಣ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ವರ್ಣ *)desc, size);

	inode->i_checksum_lo = old_lo;
	ಯದಿ (has_hi)
		inode->i_checksum_hi = old_hi;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     ಸಂರಚನಾ ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned ಪೂರ್ಣಾಙ್ಕ i, has_hi;
	ವರ್ಣ *cp;

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} ಅನ್ಯಥಾ
		calculated &= 0xFFFF;

	ಯದಿ (provided == calculated)
		ಪ್ರತ್ಯಾಗಮ 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	ಪ್ರತಿ (cp = (ವರ್ಣ *) inode, i = 0;
	     i < ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode);
	     cp++, i++)
		ಯದಿ (*cp)
			ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   ಸಂರಚನಾ ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	ಪೂರ್ಣಾಙ್ಕ has_hi;

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	ಯದಿ (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	ಪ್ರತ್ಯಾಗಮ 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	ಸಂರಚನಾ ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef ಪದಗಳು_BIGENDIAN
	ಸಂರಚನಾ ext4_group_desc swabdesc;
	size_t save_size = size;
	ಧ್ರುವ size_t ext4_bg_size = ಪರಿಮಾಣ(ಸಂರಚನಾ ext4_group_desc);
	ಸಂರಚನಾ ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	ಯದಿ (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (ಸಂರಚನಾ ext2_group_desc *) &swabdesc);
	desc = (ಸಂರಚನಾ ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	ಯದಿ (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)&group,
					 ಪರಿಮಾಣ(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned ವರ್ಣ *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef ಪದಗಳು_BIGENDIAN
		ಯದಿ (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned ವರ್ಣ *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		ಗಚ್ಛ out;
	}

	/* old crc16 code */
	offset = offsetof(ಸಂರಚನಾ ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   ಪರಿಮಾಣ(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, ಪರಿಮಾಣ(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += ಪರಿಮಾಣ(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ಯದಿ (offset < size) {
		crc = ext2fs_crc16(crc, (ವರ್ಣ *)desc + offset,
				   size - offset);
	}
#ifdef ಪದಗಳು_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	ಯದಿ (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (ವರ್ಣ *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	ಪ್ರತ್ಯಾಗಮ crc;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	ಯದಿ (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಶೂನ್ಯ ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	ಯದಿ (!ext2fs_has_group_desc_csum(fs))
		ಪ್ರತ್ಯಾಗಮ;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

ಸ್ಥಿರ __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	ಪ್ರತಿ (i = end_ino; i >= start_ino; i--) {
		ಯದಿ (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			ಪ್ರತ್ಯಾಗಮ i - start_ino + 1;
	}
	ಪ್ರತ್ಯಾಗಮ inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	ಸಂರಚನಾ ext2_super_block *sb = fs->super;
	ಪೂರ್ಣಾಙ್ಕ dirty = 0;
	dgrp_t i;

	ಯದಿ (!fs->inode_map)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_INODE_BITMAP;

	ಯದಿ (!ext2fs_has_group_desc_csum(fs))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತಿ (i = 0; i < fs->group_desc_ಎಣಿಕೆ; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_ಎಣಿಕೆ = ext2fs_bg_free_inodes_ಎಣಿಕೆ(fs, i);
		__u32 old_free_blocks_ಎಣಿಕೆ = ext2fs_bg_free_blocks_ಎಣಿಕೆ(fs, i);

		ಯದಿ (old_free_blocks_ಎಣಿಕೆ == sb->s_blocks_per_group &&
		    i != fs->group_desc_ಎಣಿಕೆ - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		ಯದಿ (old_free_inodes_ಎಣಿಕೆ == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} ಅನ್ಯಥಾ {
			ಪೂರ್ಣಾಙ್ಕ unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		ಯದಿ (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		ಯದಿ (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		ಯದಿ (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	ಯದಿ (dirty)
		ext2fs_mark_super_dirty(fs);
	ಪ್ರತ್ಯಾಗಮ 0;
}

#ifdef DEBUG
#ಅನ್ತರ್ಭಾವ "e2p/e2p.h"

ಶೂನ್ಯ print_csum(ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	ಸಂರಚನಾ ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t size = EXT2_DESC_SIZE(fs->super);
	ಸಂರಚನಾ ext2_super_block *sb = fs->super;
	ಪೂರ್ಣಾಙ್ಕ offset = offsetof(ಸಂರಚನಾ ext2_group_desc, bg_checksum);
#ifdef ಪದಗಳು_BIGENDIAN
	ಸಂರಚನಾ ext4_group_desc swabdesc;
	ಸಂರಚನಾ ext2_group_desc *save_desc = desc;
	ಧ್ರುವ size_t ext4_bg_size = ಪರಿಮಾಣ(ಸಂರಚನಾ ext4_group_desc);
	size_t save_size = size;
#endif

#ifdef ಪದಗಳು_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	ಯದಿ (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (ಸಂರಚನಾ ext2_group_desc *) &swabdesc);
	desc = (ಸಂರಚನಾ ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#ಅನ್ಯಥಾ
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, ಪರಿಮಾಣ(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, ಪರಿಮಾಣ(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += ಪರಿಮಾಣ(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ಯದಿ (offset < size)
		crc3 = ext2fs_crc16(crc3, (ವರ್ಣ *)desc + offset, size - offset);
#ifdef ಪದಗಳು_BIGENDIAN
	ಯದಿ (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (ವರ್ಣ *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       ಸಂದೇಶ, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned ವರ್ಣ sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

ಪೂರ್ಣಾಙ್ಕ main(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
	ಸಂರಚನಾ ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	ಪೂರ್ಣಾಙ್ಕ			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, ಪರಿಮಾಣ(param));
	ext2fs_blocks_ಎಣಿಕೆ_set(&param, 32768);
#ಯದಿ 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	ಯದಿ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	memcpy(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	ಪ್ರತಿ (i=0; i < fs->group_desc_ಎಣಿಕೆ; i++) {
		ext2fs_block_bitmap_loc_set(fs, i, 124);
		ext2fs_inode_bitmap_loc_set(fs, i, 125);
		ext2fs_inode_table_loc_set(fs, i, 126);
		ext2fs_bg_free_blocks_ಎಣಿಕೆ_set(fs, i, 31119);
		ext2fs_bg_free_inodes_ಎಣಿಕೆ_set(fs, i, 15701);
		ext2fs_bg_used_dirs_ಎಣಿಕೆ_set(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	ಯದಿ (csum1 != csum_known) {
		printf("checksum for group 0 should be %04x\n", csum_known);
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	ಯದಿ (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	ಯದಿ (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	ಯದಿ (csum1 != csum2) {
		printf("checksums should not depend on checksum field\n");
		exit(1);
	}
	ಯದಿ (!ext2fs_group_desc_csum_verify(fs, 0)) {
		printf("checksums should verify against gd_checksum\n");
		exit(1);
	}
	memset(fs->super->s_uuid, 0x30, ಪರಿಮಾಣ(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	ಯದಿ (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		printf("checksums for different filesystems shouldn't match\n");
		exit(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_ಎಣಿಕೆ_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	ಯದಿ (csum1 == csum2) {
		printf("checksums for different data shouldn't match\n");
		exit(1);
	}
	ext2fs_free(fs);

	ಪ್ರತ್ಯಾಗಮ 0;
}
#endif
