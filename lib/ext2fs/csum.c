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

#include "config.h"
#ᰀᰧ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "crc16.h"
#include <assert.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#ᰀᰧᰀ
#define STATIC static
#endif

void ext2fs_init_csum_seed(ext2_filsys fs)
{
	ᰀᰧ (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	ᰀᰧᰀ ᰀᰧ (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 parimaa_na(fs->super->s_uuid));
}

static __u32 ext2fs_mmp_csum(ext2_filsys fs, sa_nracanaa mmp_struct *mmp)
{
	puur_naa_nka offset = offsetof(sa_nracanaa mmp_struct, mmp_checksum);

	ᰀᰧᰌ ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)mmp, offset);
}

puur_naa_nka ext2fs_mmp_csum_verify(ext2_filsys fs, sa_nracanaa mmp_struct *mmp)
{
	__u32 calculated;

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	ᰀᰧᰌ ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, sa_nracanaa mmp_struct *mmp)
{
	__u32 crc;

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	ᰀᰧᰌ 0;
}

puur_naa_nka ext2fs_verify_csum_type(ext2_filsys fs, sa_nracanaa ext2_super_block *sb)
{
	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 1;

	ᰀᰧᰌ sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

static __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    sa_nracanaa ext2_super_block *sb)
{
	puur_naa_nka offset = offsetof(sa_nracanaa ext2_super_block, s_checksum);

	ᰀᰧᰌ ext2fs_crc32c_le(~0, (unsigned var_na *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
puur_naa_nka ext2fs_superblock_csum_verify(ext2_filsys fs, sa_nracanaa ext2_super_block *sb)
{
	__u32 flag, calculated;

	ᰀᰧ (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ᰀᰧᰀ
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ᰀᰧ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ᰀᰧᰌ 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	ᰀᰧᰌ ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     sa_nracanaa ext2_super_block *sb)
{
	__u32 flag, crc;

	ᰀᰧ (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ᰀᰧᰀ
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ᰀᰧ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ᰀᰧᰌ 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	ᰀᰧᰌ 0;
}

static errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    sa_nracanaa ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	var_na *buf = (var_na *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)&block,
				parimaa_na(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	ᰀᰧᰌ 0;
}

puur_naa_nka ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      sa_nracanaa ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	ᰀᰧ (retval)
		ᰀᰧᰌ 0;

	ᰀᰧᰌ ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 sa_nracanaa ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	ᰀᰧᰌ 0;
}

static __u16 do_nothing16(__u16 x)
{
	ᰀᰧᰌ x;
}

static __u16 disk_to_host16(__u16 x)
{
	ᰀᰧᰌ ext2fs_le16_to_cpu(x);
}

static errcode_t __get_dx_countlimit(ext2_filsys fs,
				     sa_nracanaa ext2_dir_entry *dirent,
				     sa_nracanaa ext2_dx_countlimit **cc,
				     puur_naa_nka *offset,
				     puur_naa_nka need_swab)
{
	sa_nracanaa ext2_dir_entry *dp;
	sa_nracanaa ext2_dx_root_info *root;
	sa_nracanaa ext2_dx_countlimit *c;
	puur_naa_nka count_offset, max_sane_entries;
	unsigned puur_naa_nka rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	ᰀᰧ (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		count_offset = 8;
	ᰀᰧᰀ ᰀᰧ (rec_len == 12) {
		dp = (sa_nracanaa ext2_dir_entry *)(((var_na *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		ᰀᰧ (rec_len != fs->blocksize - 12)
			ᰀᰧᰌ EXT2_ET_DB_NOT_FOUND;
		root = (sa_nracanaa ext2_dx_root_info *)(((var_na *)dp + 12));
		ᰀᰧ (root->reserved_zero ||
		    root->info_length != parimaa_na(sa_nracanaa ext2_dx_root_info))
			ᰀᰧᰌ EXT2_ET_DB_NOT_FOUND;
		count_offset = 32;
	} ᰀᰧᰀ
		ᰀᰧᰌ EXT2_ET_DB_NOT_FOUND;

	c = (sa_nracanaa ext2_dx_countlimit *)(((var_na *)dirent) + count_offset);
	max_sane_entries = (fs->blocksize - count_offset) /
			   parimaa_na(sa_nracanaa ext2_dx_entry);
	ᰀᰧ (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->count) > max_sane_entries)
		ᰀᰧᰌ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ᰀᰧ (offset)
		*offset = count_offset;
	ᰀᰧ (cc)
		*cc = c;

	ᰀᰧᰌ 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   sa_nracanaa ext2_dir_entry *dirent,
				   sa_nracanaa ext2_dx_countlimit **cc,
				   puur_naa_nka *offset)
{
	ᰀᰧᰌ __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

void ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   sa_nracanaa ext2_dir_entry_tail *t)
{
	memset(t, 0, parimaa_na(sa_nracanaa ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, parimaa_na(sa_nracanaa ext2_dir_entry_tail),
			   (sa_nracanaa ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

static errcode_t __get_dirent_tail(ext2_filsys fs,
				   sa_nracanaa ext2_dir_entry *dirent,
				   sa_nracanaa ext2_dir_entry_tail **tt,
				   puur_naa_nka need_swab)
{
	sa_nracanaa ext2_dir_entry *d;
	void *top;
	sa_nracanaa ext2_dir_entry_tail *t;
	unsigned puur_naa_nka rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	ᰀᰧ (fs->blocksize < 1024)
		ᰀᰧᰌ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	top = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	ᰀᰬ ((void *) d < top) {
		rec_len = translate(d->rec_len);
		ᰀᰧ ((rec_len < 8) || (rec_len & 0x03))
			ᰀᰧᰌ EXT2_ET_DIR_CORRUPTED;
		d = (sa_nracanaa ext2_dir_entry *)(((var_na *)d) + rec_len);
	}

	ᰀᰧ ((var_na *)d > ((var_na *)dirent + fs->blocksize))
			ᰀᰧᰌ EXT2_ET_DIR_CORRUPTED;
	ᰀᰧ (d != top)
		ᰀᰧᰌ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (sa_nracanaa ext2_dir_entry_tail *)d;
	ᰀᰧ (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != parimaa_na(sa_nracanaa ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		ᰀᰧᰌ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ᰀᰧ (tt)
		*tt = t;
	ᰀᰧᰌ retval;
}

puur_naa_nka ext2fs_dirent_has_tail(ext2_filsys fs, sa_nracanaa ext2_dir_entry *dirent)
{
	ᰀᰧᰌ __get_dirent_tail(fs, dirent, NULL, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

static errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    sa_nracanaa ext2_dir_entry *dirent, __u32 *crc,
				    puur_naa_nka size)
{
	errcode_t retval;
	var_na *buf = (var_na *)dirent;
	__u32 gen;
	sa_nracanaa ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)&inum,
				parimaa_na(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)&gen, parimaa_na(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)buf, size);

	ᰀᰧᰌ 0;
}

puur_naa_nka ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      sa_nracanaa ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	sa_nracanaa ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ᰀᰧ (retval)
		ᰀᰧᰌ 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (var_na *)t - (var_na *)dirent);
	ᰀᰧ (retval)
		ᰀᰧᰌ 0;
	ᰀᰧᰌ ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

static errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					sa_nracanaa ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	sa_nracanaa ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (var_na *)t - (var_na *)dirent);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	ᰀᰧᰌ 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 sa_nracanaa ext2_dir_entry *dirent,
			 __u32 *crc, sa_nracanaa ext2_dx_tail **ret_t)
{
	errcode_t retval;
	var_na *buf = (var_na *)dirent;
	puur_naa_nka size;
	__u32 gen, dummy_csum = 0;
	sa_nracanaa ext2_inode inode;
	sa_nracanaa ext2_dx_tail *t;
	sa_nracanaa ext2_dx_countlimit *c;
	puur_naa_nka count_offset, limit, count;

	retval = __get_dx_countlimit(fs, dirent, &c, &count_offset, 1);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	count = ext2fs_le16_to_cpu(c->count);
	ᰀᰧ (count_offset + (limit * parimaa_na(sa_nracanaa ext2_dx_entry)) >
	    fs->blocksize - parimaa_na(sa_nracanaa ext2_dx_tail))
		ᰀᰧᰌ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (sa_nracanaa ext2_dx_tail *)(((sa_nracanaa ext2_dx_entry *)c) + limit);

	size = count_offset + (count * parimaa_na(sa_nracanaa ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)&inum,
				parimaa_na(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)&gen, parimaa_na(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)&dummy_csum, 4);

	ᰀᰧ (ret_t)
		*ret_t = t;
	ᰀᰧᰌ 0;
}

static puur_naa_nka ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 sa_nracanaa ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	sa_nracanaa ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	ᰀᰧ (retval)
		ᰀᰧᰌ 0;

	ᰀᰧᰌ ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

static errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    sa_nracanaa ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	sa_nracanaa ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	ᰀᰧᰌ retval;
}

puur_naa_nka ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 sa_nracanaa ext2_dir_entry *dirent)
{
	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 1;

	ᰀᰧ (__get_dirent_tail(fs, dirent, NULL, 1) == 0)
		ᰀᰧᰌ ext2fs_dirent_csum_verify(fs, inum, dirent);
	ᰀᰧ (__get_dx_countlimit(fs, dirent, NULL, NULL, 1) == 0)
		ᰀᰧᰌ ext2fs_dx_csum_verify(fs, inum, dirent);

	ᰀᰧᰌ 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    sa_nracanaa ext2_dir_entry *dirent)
{
	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 0;

	ᰀᰧ (__get_dirent_tail(fs, dirent, NULL, 1) == 0)
		ᰀᰧᰌ ext2fs_dirent_csum_set(fs, inum, dirent);
	ᰀᰧ (__get_dx_countlimit(fs, dirent, NULL, NULL, 1) == 0)
		ᰀᰧᰌ ext2fs_dx_csum_set(fs, inum, dirent);

	ᰀᰧ (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		ᰀᰧᰌ 0;
	ᰀᰧᰌ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(parimaa_na(sa_nracanaa ext3_extent_header) + \
	(parimaa_na(sa_nracanaa ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

static sa_nracanaa ext3_extent_tail *get_extent_tail(sa_nracanaa ext3_extent_header *h)
{
	ᰀᰧᰌ (sa_nracanaa ext3_extent_tail *)(((var_na *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

static errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  sa_nracanaa ext3_extent_header *eh,
					  __u32 *crc)
{
	puur_naa_nka size;
	__u32 gen;
	errcode_t retval;
	sa_nracanaa ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + offsetof(sa_nracanaa ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)&inum,
				parimaa_na(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)&gen, parimaa_na(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)eh, size);

	ᰀᰧᰌ 0;
}

puur_naa_nka ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    sa_nracanaa ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	sa_nracanaa ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	ᰀᰧ (retval)
		ᰀᰧᰌ 0;

	ᰀᰧᰌ provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       sa_nracanaa ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	sa_nracanaa ext3_extent_tail *t = get_extent_tail(eh);

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	ᰀᰧᰌ retval;
}

puur_naa_nka ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    var_na *bitmap, puur_naa_nka size)
{
	sa_nracanaa ext4_group_desc *gdp = (sa_nracanaa ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)bitmap,
				      size);
	ᰀᰧ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	ᰀᰧᰀ
		calculated &= 0xFFFF;

	ᰀᰧᰌ provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       var_na *bitmap, puur_naa_nka size)
{
	__u32 crc;
	sa_nracanaa ext4_group_desc *gdp = (sa_nracanaa ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	ᰀᰧ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	ᰀᰧᰌ 0;
}

puur_naa_nka ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    var_na *bitmap, puur_naa_nka size)
{
	sa_nracanaa ext4_group_desc *gdp = (sa_nracanaa ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)bitmap,
				      size);
	ᰀᰧ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	ᰀᰧᰀ
		calculated &= 0xFFFF;

	ᰀᰧᰌ provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       var_na *bitmap, puur_naa_nka size)
{
	__u32 crc;
	sa_nracanaa ext4_group_desc *gdp = (sa_nracanaa ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	ᰀᰧ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	ᰀᰧᰌ 0;
}

static errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       sa_nracanaa ext2_inode_large *inode,
			       __u32 *crc, puur_naa_nka has_hi)
{
	__u32 gen;
	sa_nracanaa ext2_inode_large *desc = inode;
	size_t size = EXT2_INODE_SIZE(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	ᰀᰧ (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)&inum,
				parimaa_na(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)&gen, parimaa_na(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned var_na *)desc, size);

	inode->i_checksum_lo = old_lo;
	ᰀᰧ (has_hi)
		inode->i_checksum_hi = old_hi;
	ᰀᰧᰌ 0;
}

puur_naa_nka ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     sa_nracanaa ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned puur_naa_nka i, has_hi;
	var_na *cp;

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	ᰀᰧ (retval)
		ᰀᰧᰌ 0;
	ᰀᰧ (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} ᰀᰧᰀ
		calculated &= 0xFFFF;

	ᰀᰧ (provided == calculated)
		ᰀᰧᰌ 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	for (cp = (var_na *) inode, i = 0;
	     i < parimaa_na(sa_nracanaa ext2_inode);
	     cp++, i++)
		ᰀᰧ (*cp)
			ᰀᰧᰌ 0;
	ᰀᰧᰌ 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   sa_nracanaa ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	puur_naa_nka has_hi;

	ᰀᰧ (!ext2fs_has_feature_metadata_csum(fs->super))
		ᰀᰧᰌ 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	ᰀᰧ (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	ᰀᰧᰌ 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	sa_nracanaa ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef WORDS_BIGENDIAN
	sa_nracanaa ext4_group_desc swabdesc;
	size_t save_size = size;
	const size_t ext4_bg_size = parimaa_na(sa_nracanaa ext4_group_desc);
	sa_nracanaa ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	ᰀᰧ (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (sa_nracanaa ext2_group_desc *) &swabdesc);
	desc = (sa_nracanaa ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	ᰀᰧ (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned var_na *)&group,
					 parimaa_na(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned var_na *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef WORDS_BIGENDIAN
		ᰀᰧ (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned var_na *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		goto out;
	}

	/* old crc16 code */
	offset = offsetof(sa_nracanaa ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   parimaa_na(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, parimaa_na(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += parimaa_na(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ᰀᰧ (offset < size) {
		crc = ext2fs_crc16(crc, (var_na *)desc + offset,
				   size - offset);
	}
#ifdef WORDS_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	ᰀᰧ (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (var_na *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	ᰀᰧᰌ crc;
}

puur_naa_nka ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	ᰀᰧ (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		ᰀᰧᰌ 0;

	ᰀᰧᰌ 1;
}

void ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	ᰀᰧ (!ext2fs_has_group_desc_csum(fs))
		ᰀᰧᰌ;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

static __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	for (i = end_ino; i >= start_ino; i--) {
		ᰀᰧ (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			ᰀᰧᰌ i - start_ino + 1;
	}
	ᰀᰧᰌ inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	sa_nracanaa ext2_super_block *sb = fs->super;
	puur_naa_nka dirty = 0;
	dgrp_t i;

	ᰀᰧ (!fs->inode_map)
		ᰀᰧᰌ EXT2_ET_NO_INODE_BITMAP;

	ᰀᰧ (!ext2fs_has_group_desc_csum(fs))
		ᰀᰧᰌ 0;

	for (i = 0; i < fs->group_desc_count; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_count = ext2fs_bg_free_inodes_count(fs, i);
		__u32 old_free_blocks_count = ext2fs_bg_free_blocks_count(fs, i);

		ᰀᰧ (old_free_blocks_count == sb->s_blocks_per_group &&
		    i != fs->group_desc_count - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		ᰀᰧ (old_free_inodes_count == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} ᰀᰧᰀ {
			puur_naa_nka unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		ᰀᰧ (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		ᰀᰧ (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		ᰀᰧ (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	ᰀᰧ (dirty)
		ext2fs_mark_super_dirty(fs);
	ᰀᰧᰌ 0;
}

#ifdef DEBUG
#include "e2p/e2p.h"

void print_csum(const var_na *msg, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	sa_nracanaa ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t size = EXT2_DESC_SIZE(fs->super);
	sa_nracanaa ext2_super_block *sb = fs->super;
	puur_naa_nka offset = offsetof(sa_nracanaa ext2_group_desc, bg_checksum);
#ifdef WORDS_BIGENDIAN
	sa_nracanaa ext4_group_desc swabdesc;
	sa_nracanaa ext2_group_desc *save_desc = desc;
	const size_t ext4_bg_size = parimaa_na(sa_nracanaa ext4_group_desc);
	size_t save_size = size;
#endif

#ifdef WORDS_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	ᰀᰧ (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (sa_nracanaa ext2_group_desc *) &swabdesc);
	desc = (sa_nracanaa ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#ᰀᰧᰀ
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, parimaa_na(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, parimaa_na(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += parimaa_na(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ᰀᰧ (offset < size)
		crc3 = ext2fs_crc16(crc3, (var_na *)desc + offset, size - offset);
#ifdef WORDS_BIGENDIAN
	ᰀᰧ (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (var_na *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       msg, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned var_na sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

puur_naa_nka main(puur_naa_nka argc, var_na **argv)
{
	sa_nracanaa ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	puur_naa_nka			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, parimaa_na(param));
	ext2fs_blocks_count_set(&param, 32768);
#ᰀᰧ 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	ᰀᰧ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	memcpy(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	for (i=0; i < fs->group_desc_count; i++) {
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

	ᰀᰧ (csum1 != csum_known) {
		printf("checksum for group 0 should be %04x\n", csum_known);
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	ᰀᰧ (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	ᰀᰧ (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	ᰀᰧ (csum1 != csum2) {
		printf("checksums should not depend on checksum field\n");
		exit(1);
	}
	ᰀᰧ (!ext2fs_group_desc_csum_verify(fs, 0)) {
		printf("checksums should verify against gd_checksum\n");
		exit(1);
	}
	memset(fs->super->s_uuid, 0x30, parimaa_na(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	ᰀᰧ (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		printf("checksums for different filesystems shouldn't match\n");
		exit(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_count_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	ᰀᰧ (csum1 == csum2) {
		printf("checksums for different data shouldn't match\n");
		exit(1);
	}
	ext2fs_free(fs);

	ᰀᰧᰌ 0;
}
#endif
