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

#അന്തര്ഭാവ "config.h"
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "crc16.h"
#അന്തര്ഭാവ <assert.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#അല്ലെങ്കിൽ
#define STATIC സ്ഥിതം
#endif

ശൂന്യം ext2fs_init_csum_seed(ext2_filsys fs)
{
	എങ്കിൽ (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	അല്ലെങ്കിൽ എങ്കിൽ (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 വലുപ്പം(fs->super->s_uuid));
}

സ്ഥിതം __u32 ext2fs_mmp_csum(ext2_filsys fs, ഘടന mmp_struct *mmp)
{
	പൂർണ്ണസംഖ്യ offset = offsetof(ഘടന mmp_struct, mmp_checksum);

	തിരികെ ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)mmp, offset);
}

പൂർണ്ണസംഖ്യ ext2fs_mmp_csum_verify(ext2_filsys fs, ഘടന mmp_struct *mmp)
{
	__u32 calculated;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	തിരികെ ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, ഘടന mmp_struct *mmp)
{
	__u32 crc;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ ext2fs_verify_csum_type(ext2_filsys fs, ഘടന ext2_super_block *sb)
{
	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 1;

	തിരികെ sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

സ്ഥിതം __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    ഘടന ext2_super_block *sb)
{
	പൂർണ്ണസംഖ്യ offset = offsetof(ഘടന ext2_super_block, s_checksum);

	തിരികെ ext2fs_crc32c_le(~0, (unsigned അക്ഷരം *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
പൂർണ്ണസംഖ്യ ext2fs_superblock_csum_verify(ext2_filsys fs, ഘടന ext2_super_block *sb)
{
	__u32 flag, calculated;

	എങ്കിൽ (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	അല്ലെങ്കിൽ
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	എങ്കിൽ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		തിരികെ 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	തിരികെ ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     ഘടന ext2_super_block *sb)
{
	__u32 flag, crc;

	എങ്കിൽ (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	അല്ലെങ്കിൽ
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	എങ്കിൽ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		തിരികെ 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	തിരികെ 0;
}

സ്ഥിതം errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    ഘടന ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	അക്ഷരം *buf = (അക്ഷരം *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)&block,
				വലുപ്പം(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      ഘടന ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	എങ്കിൽ (retval)
		തിരികെ 0;

	തിരികെ ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 ഘടന ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	എങ്കിൽ (retval)
		തിരികെ retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	തിരികെ 0;
}

സ്ഥിതം __u16 do_nothing16(__u16 x)
{
	തിരികെ x;
}

സ്ഥിതം __u16 disk_to_host16(__u16 x)
{
	തിരികെ ext2fs_le16_to_cpu(x);
}

സ്ഥിതം errcode_t __get_dx_countlimit(ext2_filsys fs,
				     ഘടന ext2_dir_entry *dirent,
				     ഘടന ext2_dx_countlimit **cc,
				     പൂർണ്ണസംഖ്യ *offset,
				     പൂർണ്ണസംഖ്യ need_swab)
{
	ഘടന ext2_dir_entry *dp;
	ഘടന ext2_dx_വേര്_info *വേര്;
	ഘടന ext2_dx_countlimit *c;
	പൂർണ്ണസംഖ്യ എണ്ണം_offset, max_sane_entries;
	unsigned പൂർണ്ണസംഖ്യ rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	എങ്കിൽ (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		എണ്ണം_offset = 8;
	അല്ലെങ്കിൽ എങ്കിൽ (rec_len == 12) {
		dp = (ഘടന ext2_dir_entry *)(((അക്ഷരം *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		എങ്കിൽ (rec_len != fs->blocksize - 12)
			തിരികെ EXT2_ET_DB_NOT_FOUND;
		വേര് = (ഘടന ext2_dx_വേര്_info *)(((അക്ഷരം *)dp + 12));
		എങ്കിൽ (വേര്->reserved_zero ||
		    വേര്->info_length != വലുപ്പം(ഘടന ext2_dx_വേര്_info))
			തിരികെ EXT2_ET_DB_NOT_FOUND;
		എണ്ണം_offset = 32;
	} അല്ലെങ്കിൽ
		തിരികെ EXT2_ET_DB_NOT_FOUND;

	c = (ഘടന ext2_dx_countlimit *)(((അക്ഷരം *)dirent) + എണ്ണം_offset);
	max_sane_entries = (fs->blocksize - എണ്ണം_offset) /
			   വലുപ്പം(ഘടന ext2_dx_entry);
	എങ്കിൽ (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->എണ്ണം) > max_sane_entries)
		തിരികെ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	എങ്കിൽ (offset)
		*offset = എണ്ണം_offset;
	എങ്കിൽ (cc)
		*cc = c;

	തിരികെ 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   ഘടന ext2_dir_entry *dirent,
				   ഘടന ext2_dx_countlimit **cc,
				   പൂർണ്ണസംഖ്യ *offset)
{
	തിരികെ __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

ശൂന്യം ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   ഘടന ext2_dir_entry_tail *t)
{
	memset(t, 0, വലുപ്പം(ഘടന ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, വലുപ്പം(ഘടന ext2_dir_entry_tail),
			   (ഘടന ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

സ്ഥിതം errcode_t __get_dirent_tail(ext2_filsys fs,
				   ഘടന ext2_dir_entry *dirent,
				   ഘടന ext2_dir_entry_tail **tt,
				   പൂർണ്ണസംഖ്യ need_swab)
{
	ഘടന ext2_dir_entry *d;
	ശൂന്യം *മുകൾ;
	ഘടന ext2_dir_entry_tail *t;
	unsigned പൂർണ്ണസംഖ്യ rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	എങ്കിൽ (fs->blocksize < 1024)
		തിരികെ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	മുകൾ = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	ആകുമ്പോൾ ((ശൂന്യം *) d < മുകൾ) {
		rec_len = translate(d->rec_len);
		എങ്കിൽ ((rec_len < 8) || (rec_len & 0x03))
			തിരികെ EXT2_ET_DIR_CORRUPTED;
		d = (ഘടന ext2_dir_entry *)(((അക്ഷരം *)d) + rec_len);
	}

	എങ്കിൽ ((അക്ഷരം *)d > ((അക്ഷരം *)dirent + fs->blocksize))
			തിരികെ EXT2_ET_DIR_CORRUPTED;
	എങ്കിൽ (d != മുകൾ)
		തിരികെ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (ഘടന ext2_dir_entry_tail *)d;
	എങ്കിൽ (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != വലുപ്പം(ഘടന ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		തിരികെ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	എങ്കിൽ (tt)
		*tt = t;
	തിരികെ retval;
}

പൂർണ്ണസംഖ്യ ext2fs_dirent_has_tail(ext2_filsys fs, ഘടന ext2_dir_entry *dirent)
{
	തിരികെ __get_dirent_tail(fs, dirent, ശൂന്യ, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

സ്ഥിതം errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    ഘടന ext2_dir_entry *dirent, __u32 *crc,
				    പൂർണ്ണസംഖ്യ size)
{
	errcode_t retval;
	അക്ഷരം *buf = (അക്ഷരം *)dirent;
	__u32 gen;
	ഘടന ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)&inum,
				വലുപ്പം(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)&gen, വലുപ്പം(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)buf, size);

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      ഘടന ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	ഘടന ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	എങ്കിൽ (retval)
		തിരികെ 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (അക്ഷരം *)t - (അക്ഷരം *)dirent);
	എങ്കിൽ (retval)
		തിരികെ 0;
	തിരികെ ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

സ്ഥിതം errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					ഘടന ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	ഘടന ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	എങ്കിൽ (retval)
		തിരികെ retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (അക്ഷരം *)t - (അക്ഷരം *)dirent);
	എങ്കിൽ (retval)
		തിരികെ retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	തിരികെ 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 ഘടന ext2_dir_entry *dirent,
			 __u32 *crc, ഘടന ext2_dx_tail **ret_t)
{
	errcode_t retval;
	അക്ഷരം *buf = (അക്ഷരം *)dirent;
	പൂർണ്ണസംഖ്യ size;
	__u32 gen, dummy_csum = 0;
	ഘടന ext2_inode inode;
	ഘടന ext2_dx_tail *t;
	ഘടന ext2_dx_countlimit *c;
	പൂർണ്ണസംഖ്യ എണ്ണം_offset, limit, എണ്ണം;

	retval = __get_dx_countlimit(fs, dirent, &c, &എണ്ണം_offset, 1);
	എങ്കിൽ (retval)
		തിരികെ retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	എണ്ണം = ext2fs_le16_to_cpu(c->എണ്ണം);
	എങ്കിൽ (എണ്ണം_offset + (limit * വലുപ്പം(ഘടന ext2_dx_entry)) >
	    fs->blocksize - വലുപ്പം(ഘടന ext2_dx_tail))
		തിരികെ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (ഘടന ext2_dx_tail *)(((ഘടന ext2_dx_entry *)c) + limit);

	size = എണ്ണം_offset + (എണ്ണം * വലുപ്പം(ഘടന ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)&inum,
				വലുപ്പം(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)&gen, വലുപ്പം(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)&dummy_csum, 4);

	എങ്കിൽ (ret_t)
		*ret_t = t;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ഘടന ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	ഘടന ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	എങ്കിൽ (retval)
		തിരികെ 0;

	തിരികെ ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

സ്ഥിതം errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    ഘടന ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	ഘടന ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	എങ്കിൽ (retval)
		തിരികെ retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	തിരികെ retval;
}

പൂർണ്ണസംഖ്യ ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ഘടന ext2_dir_entry *dirent)
{
	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 1;

	എങ്കിൽ (__get_dirent_tail(fs, dirent, ശൂന്യ, 1) == 0)
		തിരികെ ext2fs_dirent_csum_verify(fs, inum, dirent);
	എങ്കിൽ (__get_dx_countlimit(fs, dirent, ശൂന്യ, ശൂന്യ, 1) == 0)
		തിരികെ ext2fs_dx_csum_verify(fs, inum, dirent);

	തിരികെ 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    ഘടന ext2_dir_entry *dirent)
{
	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 0;

	എങ്കിൽ (__get_dirent_tail(fs, dirent, ശൂന്യ, 1) == 0)
		തിരികെ ext2fs_dirent_csum_set(fs, inum, dirent);
	എങ്കിൽ (__get_dx_countlimit(fs, dirent, ശൂന്യ, ശൂന്യ, 1) == 0)
		തിരികെ ext2fs_dx_csum_set(fs, inum, dirent);

	എങ്കിൽ (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		തിരികെ 0;
	തിരികെ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(വലുപ്പം(ഘടന ext3_extent_header) + \
	(വലുപ്പം(ഘടന ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

സ്ഥിതം ഘടന ext3_extent_tail *get_extent_tail(ഘടന ext3_extent_header *h)
{
	തിരികെ (ഘടന ext3_extent_tail *)(((അക്ഷരം *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

സ്ഥിതം errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  ഘടന ext3_extent_header *eh,
					  __u32 *crc)
{
	പൂർണ്ണസംഖ്യ size;
	__u32 gen;
	errcode_t retval;
	ഘടന ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + offsetof(ഘടന ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)&inum,
				വലുപ്പം(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)&gen, വലുപ്പം(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)eh, size);

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ഘടന ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	ഘടന ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	എങ്കിൽ (retval)
		തിരികെ 0;

	തിരികെ provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       ഘടന ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	ഘടന ext3_extent_tail *t = get_extent_tail(eh);

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	എങ്കിൽ (retval)
		തിരികെ retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	തിരികെ retval;
}

പൂർണ്ണസംഖ്യ ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    അക്ഷരം *bitmap, പൂർണ്ണസംഖ്യ size)
{
	ഘടന ext4_group_desc *gdp = (ഘടന ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)bitmap,
				      size);
	എങ്കിൽ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	അല്ലെങ്കിൽ
		calculated &= 0xFFFF;

	തിരികെ provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       അക്ഷരം *bitmap, പൂർണ്ണസംഖ്യ size)
{
	__u32 crc;
	ഘടന ext4_group_desc *gdp = (ഘടന ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	എങ്കിൽ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    അക്ഷരം *bitmap, പൂർണ്ണസംഖ്യ size)
{
	ഘടന ext4_group_desc *gdp = (ഘടന ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)bitmap,
				      size);
	എങ്കിൽ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	അല്ലെങ്കിൽ
		calculated &= 0xFFFF;

	തിരികെ provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       അക്ഷരം *bitmap, പൂർണ്ണസംഖ്യ size)
{
	__u32 crc;
	ഘടന ext4_group_desc *gdp = (ഘടന ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	എങ്കിൽ (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	തിരികെ 0;
}

സ്ഥിതം errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       ഘടന ext2_inode_large *inode,
			       __u32 *crc, പൂർണ്ണസംഖ്യ has_hi)
{
	__u32 gen;
	ഘടന ext2_inode_large *desc = inode;
	size_t size = EXT2_INODE_SIZE(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	എങ്കിൽ (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)&inum,
				വലുപ്പം(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)&gen, വലുപ്പം(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned അക്ഷരം *)desc, size);

	inode->i_checksum_lo = old_lo;
	എങ്കിൽ (has_hi)
		inode->i_checksum_hi = old_hi;
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     ഘടന ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned പൂർണ്ണസംഖ്യ i, has_hi;
	അക്ഷരം *cp;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	എങ്കിൽ (retval)
		തിരികെ 0;
	എങ്കിൽ (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} അല്ലെങ്കിൽ
		calculated &= 0xFFFF;

	എങ്കിൽ (provided == calculated)
		തിരികെ 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	വേണ്ടി (cp = (അക്ഷരം *) inode, i = 0;
	     i < വലുപ്പം(ഘടന ext2_inode);
	     cp++, i++)
		എങ്കിൽ (*cp)
			തിരികെ 0;
	തിരികെ 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   ഘടന ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	പൂർണ്ണസംഖ്യ has_hi;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	എങ്കിൽ (retval)
		തിരികെ retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	എങ്കിൽ (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	തിരികെ 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	ഘടന ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef വാക്കുകൾ_BIGENDIAN
	ഘടന ext4_group_desc swabdesc;
	size_t save_size = size;
	സ്ഥിരം size_t ext4_bg_size = വലുപ്പം(ഘടന ext4_group_desc);
	ഘടന ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	എങ്കിൽ (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (ഘടന ext2_group_desc *) &swabdesc);
	desc = (ഘടന ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	എങ്കിൽ (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)&group,
					 വലുപ്പം(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned അക്ഷരം *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef വാക്കുകൾ_BIGENDIAN
		എങ്കിൽ (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned അക്ഷരം *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		പോകുക out;
	}

	/* old crc16 code */
	offset = offsetof(ഘടന ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   വലുപ്പം(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, വലുപ്പം(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += വലുപ്പം(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	എങ്കിൽ (offset < size) {
		crc = ext2fs_crc16(crc, (അക്ഷരം *)desc + offset,
				   size - offset);
	}
#ifdef വാക്കുകൾ_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	എങ്കിൽ (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (അക്ഷരം *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	തിരികെ crc;
}

പൂർണ്ണസംഖ്യ ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	എങ്കിൽ (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		തിരികെ 0;

	തിരികെ 1;
}

ശൂന്യം ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	എങ്കിൽ (!ext2fs_has_group_desc_csum(fs))
		തിരികെ;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

സ്ഥിതം __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	വേണ്ടി (i = end_ino; i >= start_ino; i--) {
		എങ്കിൽ (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			തിരികെ i - start_ino + 1;
	}
	തിരികെ inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	ഘടന ext2_super_block *sb = fs->super;
	പൂർണ്ണസംഖ്യ dirty = 0;
	dgrp_t i;

	എങ്കിൽ (!fs->inode_map)
		തിരികെ EXT2_ET_NO_INODE_BITMAP;

	എങ്കിൽ (!ext2fs_has_group_desc_csum(fs))
		തിരികെ 0;

	വേണ്ടി (i = 0; i < fs->group_desc_എണ്ണം; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_എണ്ണം = ext2fs_bg_free_inodes_എണ്ണം(fs, i);
		__u32 old_free_blocks_എണ്ണം = ext2fs_bg_free_blocks_എണ്ണം(fs, i);

		എങ്കിൽ (old_free_blocks_എണ്ണം == sb->s_blocks_per_group &&
		    i != fs->group_desc_എണ്ണം - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		എങ്കിൽ (old_free_inodes_എണ്ണം == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} അല്ലെങ്കിൽ {
			പൂർണ്ണസംഖ്യ unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		എങ്കിൽ (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		എങ്കിൽ (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		എങ്കിൽ (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	എങ്കിൽ (dirty)
		ext2fs_mark_super_dirty(fs);
	തിരികെ 0;
}

#ifdef DEBUG
#അന്തര്ഭാവ "e2p/e2p.h"

ശൂന്യം print_csum(സ്ഥിരം അക്ഷരം *സന്ദേശം, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	ഘടന ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t size = EXT2_DESC_SIZE(fs->super);
	ഘടന ext2_super_block *sb = fs->super;
	പൂർണ്ണസംഖ്യ offset = offsetof(ഘടന ext2_group_desc, bg_checksum);
#ifdef വാക്കുകൾ_BIGENDIAN
	ഘടന ext4_group_desc swabdesc;
	ഘടന ext2_group_desc *save_desc = desc;
	സ്ഥിരം size_t ext4_bg_size = വലുപ്പം(ഘടന ext4_group_desc);
	size_t save_size = size;
#endif

#ifdef വാക്കുകൾ_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	എങ്കിൽ (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (ഘടന ext2_group_desc *) &swabdesc);
	desc = (ഘടന ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#അല്ലെങ്കിൽ
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, വലുപ്പം(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, വലുപ്പം(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += വലുപ്പം(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	എങ്കിൽ (offset < size)
		crc3 = ext2fs_crc16(crc3, (അക്ഷരം *)desc + offset, size - offset);
#ifdef വാക്കുകൾ_BIGENDIAN
	എങ്കിൽ (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (അക്ഷരം *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       സന്ദേശം, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned അക്ഷരം sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	ഘടന ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	പൂർണ്ണസംഖ്യ			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, വലുപ്പം(param));
	ext2fs_blocks_എണ്ണം_set(&param, 32768);
#എങ്കിൽ 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	എങ്കിൽ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	memcpy(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	വേണ്ടി (i=0; i < fs->group_desc_എണ്ണം; i++) {
		ext2fs_block_bitmap_loc_set(fs, i, 124);
		ext2fs_inode_bitmap_loc_set(fs, i, 125);
		ext2fs_inode_table_loc_set(fs, i, 126);
		ext2fs_bg_free_blocks_എണ്ണം_set(fs, i, 31119);
		ext2fs_bg_free_inodes_എണ്ണം_set(fs, i, 15701);
		ext2fs_bg_used_dirs_എണ്ണം_set(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	എങ്കിൽ (csum1 != csum_known) {
		printf("checksum for group 0 should be %04x\n", csum_known);
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	എങ്കിൽ (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	എങ്കിൽ (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	എങ്കിൽ (csum1 != csum2) {
		printf("checksums should not depend on checksum field\n");
		exit(1);
	}
	എങ്കിൽ (!ext2fs_group_desc_csum_verify(fs, 0)) {
		printf("checksums should verify against gd_checksum\n");
		exit(1);
	}
	memset(fs->super->s_uuid, 0x30, വലുപ്പം(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	എങ്കിൽ (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		printf("checksums for different filesystems shouldn't match\n");
		exit(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_എണ്ണം_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	എങ്കിൽ (csum1 == csum2) {
		printf("checksums for different data shouldn't match\n");
		exit(1);
	}
	ext2fs_free(fs);

	തിരികെ 0;
}
#endif
