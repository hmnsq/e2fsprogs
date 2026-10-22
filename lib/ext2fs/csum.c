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

#அந்தர்பாவ "config.h"
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "crc16.h"
#அந்தர்பாவ <assert.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#இல்லையெனில்
#define STATIC நிலையான
#endif

வெறுமை ext2fs_init_csum_seed(ext2_filsys fs)
{
	எனில் (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	இல்லையெனில் எனில் (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 அளவு(fs->super->s_uuid));
}

நிலையான __u32 ext2fs_mmp_csum(ext2_filsys fs, கட்டமைப்பு mmp_struct *mmp)
{
	முழு offset = offsetof(கட்டமைப்பு mmp_struct, mmp_checksum);

	திரும்பு ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)mmp, offset);
}

முழு ext2fs_mmp_csum_verify(ext2_filsys fs, கட்டமைப்பு mmp_struct *mmp)
{
	__u32 calculated;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	திரும்பு ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, கட்டமைப்பு mmp_struct *mmp)
{
	__u32 crc;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	திரும்பு 0;
}

முழு ext2fs_verify_csum_type(ext2_filsys fs, கட்டமைப்பு ext2_super_block *sb)
{
	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 1;

	திரும்பு sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

நிலையான __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    கட்டமைப்பு ext2_super_block *sb)
{
	முழு offset = offsetof(கட்டமைப்பு ext2_super_block, s_checksum);

	திரும்பு ext2fs_crc32c_le(~0, (unsigned எழுத்து *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
முழு ext2fs_superblock_csum_verify(ext2_filsys fs, கட்டமைப்பு ext2_super_block *sb)
{
	__u32 flag, calculated;

	எனில் (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	இல்லையெனில்
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	எனில் (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		திரும்பு 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	திரும்பு ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     கட்டமைப்பு ext2_super_block *sb)
{
	__u32 flag, crc;

	எனில் (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	இல்லையெனில்
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	எனில் (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		திரும்பு 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	திரும்பு 0;
}

நிலையான errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    கட்டமைப்பு ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	எழுத்து *buf = (எழுத்து *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)&block,
				அளவு(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	திரும்பு 0;
}

முழு ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      கட்டமைப்பு ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	எனில் (retval)
		திரும்பு 0;

	திரும்பு ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 கட்டமைப்பு ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	எனில் (retval)
		திரும்பு retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	திரும்பு 0;
}

நிலையான __u16 do_nothing16(__u16 x)
{
	திரும்பு x;
}

நிலையான __u16 disk_to_host16(__u16 x)
{
	திரும்பு ext2fs_le16_to_cpu(x);
}

நிலையான errcode_t __get_dx_countlimit(ext2_filsys fs,
				     கட்டமைப்பு ext2_dir_entry *dirent,
				     கட்டமைப்பு ext2_dx_countlimit **cc,
				     முழு *offset,
				     முழு need_swab)
{
	கட்டமைப்பு ext2_dir_entry *dp;
	கட்டமைப்பு ext2_dx_வேர்_info *வேர்;
	கட்டமைப்பு ext2_dx_countlimit *c;
	முழு மொத்தம்_offset, max_sane_entries;
	unsigned முழு rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	எனில் (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		மொத்தம்_offset = 8;
	இல்லையெனில் எனில் (rec_len == 12) {
		dp = (கட்டமைப்பு ext2_dir_entry *)(((எழுத்து *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		எனில் (rec_len != fs->blocksize - 12)
			திரும்பு EXT2_ET_DB_NOT_FOUND;
		வேர் = (கட்டமைப்பு ext2_dx_வேர்_info *)(((எழுத்து *)dp + 12));
		எனில் (வேர்->reserved_zero ||
		    வேர்->info_length != அளவு(கட்டமைப்பு ext2_dx_வேர்_info))
			திரும்பு EXT2_ET_DB_NOT_FOUND;
		மொத்தம்_offset = 32;
	} இல்லையெனில்
		திரும்பு EXT2_ET_DB_NOT_FOUND;

	c = (கட்டமைப்பு ext2_dx_countlimit *)(((எழுத்து *)dirent) + மொத்தம்_offset);
	max_sane_entries = (fs->blocksize - மொத்தம்_offset) /
			   அளவு(கட்டமைப்பு ext2_dx_entry);
	எனில் (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->மொத்தம்) > max_sane_entries)
		திரும்பு EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	எனில் (offset)
		*offset = மொத்தம்_offset;
	எனில் (cc)
		*cc = c;

	திரும்பு 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   கட்டமைப்பு ext2_dir_entry *dirent,
				   கட்டமைப்பு ext2_dx_countlimit **cc,
				   முழு *offset)
{
	திரும்பு __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

வெறுமை ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   கட்டமைப்பு ext2_dir_entry_tail *t)
{
	memset(t, 0, அளவு(கட்டமைப்பு ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, அளவு(கட்டமைப்பு ext2_dir_entry_tail),
			   (கட்டமைப்பு ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

நிலையான errcode_t __get_dirent_tail(ext2_filsys fs,
				   கட்டமைப்பு ext2_dir_entry *dirent,
				   கட்டமைப்பு ext2_dir_entry_tail **tt,
				   முழு need_swab)
{
	கட்டமைப்பு ext2_dir_entry *d;
	வெறுமை *உச்சி;
	கட்டமைப்பு ext2_dir_entry_tail *t;
	unsigned முழு rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	எனில் (fs->blocksize < 1024)
		திரும்பு EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	உச்சி = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	போது ((வெறுமை *) d < உச்சி) {
		rec_len = translate(d->rec_len);
		எனில் ((rec_len < 8) || (rec_len & 0x03))
			திரும்பு EXT2_ET_DIR_CORRUPTED;
		d = (கட்டமைப்பு ext2_dir_entry *)(((எழுத்து *)d) + rec_len);
	}

	எனில் ((எழுத்து *)d > ((எழுத்து *)dirent + fs->blocksize))
			திரும்பு EXT2_ET_DIR_CORRUPTED;
	எனில் (d != உச்சி)
		திரும்பு EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (கட்டமைப்பு ext2_dir_entry_tail *)d;
	எனில் (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != அளவு(கட்டமைப்பு ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		திரும்பு EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	எனில் (tt)
		*tt = t;
	திரும்பு retval;
}

முழு ext2fs_dirent_has_tail(ext2_filsys fs, கட்டமைப்பு ext2_dir_entry *dirent)
{
	திரும்பு __get_dirent_tail(fs, dirent, வெற்று, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

நிலையான errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    கட்டமைப்பு ext2_dir_entry *dirent, __u32 *crc,
				    முழு size)
{
	errcode_t retval;
	எழுத்து *buf = (எழுத்து *)dirent;
	__u32 gen;
	கட்டமைப்பு ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	எனில் (retval)
		திரும்பு retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)&inum,
				அளவு(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)&gen, அளவு(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)buf, size);

	திரும்பு 0;
}

முழு ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      கட்டமைப்பு ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	கட்டமைப்பு ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	எனில் (retval)
		திரும்பு 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (எழுத்து *)t - (எழுத்து *)dirent);
	எனில் (retval)
		திரும்பு 0;
	திரும்பு ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

நிலையான errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					கட்டமைப்பு ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	கட்டமைப்பு ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	எனில் (retval)
		திரும்பு retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (எழுத்து *)t - (எழுத்து *)dirent);
	எனில் (retval)
		திரும்பு retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	திரும்பு 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 கட்டமைப்பு ext2_dir_entry *dirent,
			 __u32 *crc, கட்டமைப்பு ext2_dx_tail **ret_t)
{
	errcode_t retval;
	எழுத்து *buf = (எழுத்து *)dirent;
	முழு size;
	__u32 gen, dummy_csum = 0;
	கட்டமைப்பு ext2_inode inode;
	கட்டமைப்பு ext2_dx_tail *t;
	கட்டமைப்பு ext2_dx_countlimit *c;
	முழு மொத்தம்_offset, limit, மொத்தம்;

	retval = __get_dx_countlimit(fs, dirent, &c, &மொத்தம்_offset, 1);
	எனில் (retval)
		திரும்பு retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	மொத்தம் = ext2fs_le16_to_cpu(c->மொத்தம்);
	எனில் (மொத்தம்_offset + (limit * அளவு(கட்டமைப்பு ext2_dx_entry)) >
	    fs->blocksize - அளவு(கட்டமைப்பு ext2_dx_tail))
		திரும்பு EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (கட்டமைப்பு ext2_dx_tail *)(((கட்டமைப்பு ext2_dx_entry *)c) + limit);

	size = மொத்தம்_offset + (மொத்தம் * அளவு(கட்டமைப்பு ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	எனில் (retval)
		திரும்பு retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)&inum,
				அளவு(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)&gen, அளவு(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)&dummy_csum, 4);

	எனில் (ret_t)
		*ret_t = t;
	திரும்பு 0;
}

நிலையான முழு ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 கட்டமைப்பு ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	கட்டமைப்பு ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	எனில் (retval)
		திரும்பு 0;

	திரும்பு ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

நிலையான errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    கட்டமைப்பு ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	கட்டமைப்பு ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	எனில் (retval)
		திரும்பு retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	திரும்பு retval;
}

முழு ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 கட்டமைப்பு ext2_dir_entry *dirent)
{
	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 1;

	எனில் (__get_dirent_tail(fs, dirent, வெற்று, 1) == 0)
		திரும்பு ext2fs_dirent_csum_verify(fs, inum, dirent);
	எனில் (__get_dx_countlimit(fs, dirent, வெற்று, வெற்று, 1) == 0)
		திரும்பு ext2fs_dx_csum_verify(fs, inum, dirent);

	திரும்பு 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    கட்டமைப்பு ext2_dir_entry *dirent)
{
	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 0;

	எனில் (__get_dirent_tail(fs, dirent, வெற்று, 1) == 0)
		திரும்பு ext2fs_dirent_csum_set(fs, inum, dirent);
	எனில் (__get_dx_countlimit(fs, dirent, வெற்று, வெற்று, 1) == 0)
		திரும்பு ext2fs_dx_csum_set(fs, inum, dirent);

	எனில் (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		திரும்பு 0;
	திரும்பு EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(அளவு(கட்டமைப்பு ext3_extent_header) + \
	(அளவு(கட்டமைப்பு ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

நிலையான கட்டமைப்பு ext3_extent_tail *get_extent_tail(கட்டமைப்பு ext3_extent_header *h)
{
	திரும்பு (கட்டமைப்பு ext3_extent_tail *)(((எழுத்து *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

நிலையான errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  கட்டமைப்பு ext3_extent_header *eh,
					  __u32 *crc)
{
	முழு size;
	__u32 gen;
	errcode_t retval;
	கட்டமைப்பு ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + offsetof(கட்டமைப்பு ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	எனில் (retval)
		திரும்பு retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)&inum,
				அளவு(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)&gen, அளவு(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)eh, size);

	திரும்பு 0;
}

முழு ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    கட்டமைப்பு ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	கட்டமைப்பு ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	எனில் (retval)
		திரும்பு 0;

	திரும்பு provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       கட்டமைப்பு ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	கட்டமைப்பு ext3_extent_tail *t = get_extent_tail(eh);

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	எனில் (retval)
		திரும்பு retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	திரும்பு retval;
}

முழு ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    எழுத்து *bitmap, முழு size)
{
	கட்டமைப்பு ext4_group_desc *gdp = (கட்டமைப்பு ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)bitmap,
				      size);
	எனில் (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	இல்லையெனில்
		calculated &= 0xFFFF;

	திரும்பு provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       எழுத்து *bitmap, முழு size)
{
	__u32 crc;
	கட்டமைப்பு ext4_group_desc *gdp = (கட்டமைப்பு ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	எனில் (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	திரும்பு 0;
}

முழு ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    எழுத்து *bitmap, முழு size)
{
	கட்டமைப்பு ext4_group_desc *gdp = (கட்டமைப்பு ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)bitmap,
				      size);
	எனில் (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	இல்லையெனில்
		calculated &= 0xFFFF;

	திரும்பு provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       எழுத்து *bitmap, முழு size)
{
	__u32 crc;
	கட்டமைப்பு ext4_group_desc *gdp = (கட்டமைப்பு ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	எனில் (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	திரும்பு 0;
}

நிலையான errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       கட்டமைப்பு ext2_inode_large *inode,
			       __u32 *crc, முழு has_hi)
{
	__u32 gen;
	கட்டமைப்பு ext2_inode_large *desc = inode;
	size_t size = EXT2_INODE_SIZE(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	எனில் (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)&inum,
				அளவு(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)&gen, அளவு(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned எழுத்து *)desc, size);

	inode->i_checksum_lo = old_lo;
	எனில் (has_hi)
		inode->i_checksum_hi = old_hi;
	திரும்பு 0;
}

முழு ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     கட்டமைப்பு ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned முழு i, has_hi;
	எழுத்து *cp;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	எனில் (retval)
		திரும்பு 0;
	எனில் (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} இல்லையெனில்
		calculated &= 0xFFFF;

	எனில் (provided == calculated)
		திரும்பு 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	க்கு (cp = (எழுத்து *) inode, i = 0;
	     i < அளவு(கட்டமைப்பு ext2_inode);
	     cp++, i++)
		எனில் (*cp)
			திரும்பு 0;
	திரும்பு 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   கட்டமைப்பு ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	முழு has_hi;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	எனில் (retval)
		திரும்பு retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	எனில் (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	திரும்பு 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	கட்டமைப்பு ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef சொற்கள்_BIGENDIAN
	கட்டமைப்பு ext4_group_desc swabdesc;
	size_t save_size = size;
	மாறிலி size_t ext4_bg_size = அளவு(கட்டமைப்பு ext4_group_desc);
	கட்டமைப்பு ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	எனில் (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (கட்டமைப்பு ext2_group_desc *) &swabdesc);
	desc = (கட்டமைப்பு ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	எனில் (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)&group,
					 அளவு(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned எழுத்து *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef சொற்கள்_BIGENDIAN
		எனில் (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned எழுத்து *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		செல் out;
	}

	/* old crc16 code */
	offset = offsetof(கட்டமைப்பு ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   அளவு(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, அளவு(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += அளவு(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	எனில் (offset < size) {
		crc = ext2fs_crc16(crc, (எழுத்து *)desc + offset,
				   size - offset);
	}
#ifdef சொற்கள்_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	எனில் (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (எழுத்து *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	திரும்பு crc;
}

முழு ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	எனில் (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		திரும்பு 0;

	திரும்பு 1;
}

வெறுமை ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	எனில் (!ext2fs_has_group_desc_csum(fs))
		திரும்பு;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

நிலையான __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	க்கு (i = end_ino; i >= start_ino; i--) {
		எனில் (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			திரும்பு i - start_ino + 1;
	}
	திரும்பு inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	கட்டமைப்பு ext2_super_block *sb = fs->super;
	முழு dirty = 0;
	dgrp_t i;

	எனில் (!fs->inode_map)
		திரும்பு EXT2_ET_NO_INODE_BITMAP;

	எனில் (!ext2fs_has_group_desc_csum(fs))
		திரும்பு 0;

	க்கு (i = 0; i < fs->group_desc_மொத்தம்; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_மொத்தம் = ext2fs_bg_free_inodes_மொத்தம்(fs, i);
		__u32 old_free_blocks_மொத்தம் = ext2fs_bg_free_blocks_மொத்தம்(fs, i);

		எனில் (old_free_blocks_மொத்தம் == sb->s_blocks_per_group &&
		    i != fs->group_desc_மொத்தம் - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		எனில் (old_free_inodes_மொத்தம் == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} இல்லையெனில் {
			முழு unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		எனில் (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		எனில் (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		எனில் (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	எனில் (dirty)
		ext2fs_mark_super_dirty(fs);
	திரும்பு 0;
}

#ifdef DEBUG
#அந்தர்பாவ "e2p/e2p.h"

வெறுமை print_csum(மாறிலி எழுத்து *செய்தி, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	கட்டமைப்பு ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t size = EXT2_DESC_SIZE(fs->super);
	கட்டமைப்பு ext2_super_block *sb = fs->super;
	முழு offset = offsetof(கட்டமைப்பு ext2_group_desc, bg_checksum);
#ifdef சொற்கள்_BIGENDIAN
	கட்டமைப்பு ext4_group_desc swabdesc;
	கட்டமைப்பு ext2_group_desc *save_desc = desc;
	மாறிலி size_t ext4_bg_size = அளவு(கட்டமைப்பு ext4_group_desc);
	size_t save_size = size;
#endif

#ifdef சொற்கள்_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	எனில் (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (கட்டமைப்பு ext2_group_desc *) &swabdesc);
	desc = (கட்டமைப்பு ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#இல்லையெனில்
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, அளவு(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, அளவு(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += அளவு(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	எனில் (offset < size)
		crc3 = ext2fs_crc16(crc3, (எழுத்து *)desc + offset, size - offset);
#ifdef சொற்கள்_BIGENDIAN
	எனில் (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (எழுத்து *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       செய்தி, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned எழுத்து sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

முழு முதன்மை(முழு argc, எழுத்து **argv)
{
	கட்டமைப்பு ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	முழு			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, அளவு(param));
	ext2fs_blocks_மொத்தம்_set(&param, 32768);
#எனில் 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	எனில் (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	memcpy(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	க்கு (i=0; i < fs->group_desc_மொத்தம்; i++) {
		ext2fs_block_bitmap_loc_set(fs, i, 124);
		ext2fs_inode_bitmap_loc_set(fs, i, 125);
		ext2fs_inode_table_loc_set(fs, i, 126);
		ext2fs_bg_free_blocks_மொத்தம்_set(fs, i, 31119);
		ext2fs_bg_free_inodes_மொத்தம்_set(fs, i, 15701);
		ext2fs_bg_used_dirs_மொத்தம்_set(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	எனில் (csum1 != csum_known) {
		printf("checksum for group 0 should be %04x\n", csum_known);
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	எனில் (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	எனில் (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	எனில் (csum1 != csum2) {
		printf("checksums should not depend on checksum field\n");
		exit(1);
	}
	எனில் (!ext2fs_group_desc_csum_verify(fs, 0)) {
		printf("checksums should verify against gd_checksum\n");
		exit(1);
	}
	memset(fs->super->s_uuid, 0x30, அளவு(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	எனில் (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		printf("checksums for different filesystems shouldn't match\n");
		exit(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_மொத்தம்_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	எனில் (csum1 == csum2) {
		printf("checksums for different data shouldn't match\n");
		exit(1);
	}
	ext2fs_free(fs);

	திரும்பு 0;
}
#endif
