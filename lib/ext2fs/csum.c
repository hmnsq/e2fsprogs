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

#अन्तर्भाव "config.h"
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "crc16.h"
#अन्तर्भाव <assert.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#अन्यथा
#define STATIC स्थिर
#endif

शून्य ext2fs_init_csum_seed(ext2_filsys fs)
{
	यदि (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	अन्यथा यदि (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 परिमाण(fs->super->s_uuid));
}

स्थिर __u32 ext2fs_mmp_csum(ext2_filsys fs, संरचना mmp_struct *mmp)
{
	पूर्णाङ्क offset = offsetof(संरचना mmp_struct, mmp_checksum);

	प्रत्यागम ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)mmp, offset);
}

पूर्णाङ्क ext2fs_mmp_csum_verify(ext2_filsys fs, संरचना mmp_struct *mmp)
{
	__u32 calculated;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	प्रत्यागम ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, संरचना mmp_struct *mmp)
{
	__u32 crc;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_verify_csum_type(ext2_filsys fs, संरचना ext2_super_block *sb)
{
	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	प्रत्यागम sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

स्थिर __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    संरचना ext2_super_block *sb)
{
	पूर्णाङ्क offset = offsetof(संरचना ext2_super_block, s_checksum);

	प्रत्यागम ext2fs_crc32c_le(~0, (unsigned वर्ण *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
पूर्णाङ्क ext2fs_superblock_csum_verify(ext2_filsys fs, संरचना ext2_super_block *sb)
{
	__u32 flag, calculated;

	यदि (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	अन्यथा
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	यदि (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		प्रत्यागम 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	प्रत्यागम ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     संरचना ext2_super_block *sb)
{
	__u32 flag, crc;

	यदि (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	अन्यथा
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	यदि (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		प्रत्यागम 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	प्रत्यागम 0;
}

स्थिर errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    संरचना ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	वर्ण *buf = (वर्ण *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)&block,
				परिमाण(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      संरचना ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	यदि (retval)
		प्रत्यागम 0;

	प्रत्यागम ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 संरचना ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	यदि (retval)
		प्रत्यागम retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	प्रत्यागम 0;
}

स्थिर __u16 do_nothing16(__u16 x)
{
	प्रत्यागम x;
}

स्थिर __u16 disk_to_host16(__u16 x)
{
	प्रत्यागम ext2fs_le16_to_cpu(x);
}

स्थिर errcode_t __get_dx_countlimit(ext2_filsys fs,
				     संरचना ext2_dir_entry *dirent,
				     संरचना ext2_dx_countlimit **cc,
				     पूर्णाङ्क *offset,
				     पूर्णाङ्क need_swab)
{
	संरचना ext2_dir_entry *dp;
	संरचना ext2_dx_मूळ_info *मूळ;
	संरचना ext2_dx_countlimit *c;
	पूर्णाङ्क मोजणी_offset, max_sane_entries;
	unsigned पूर्णाङ्क rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	यदि (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		मोजणी_offset = 8;
	अन्यथा यदि (rec_len == 12) {
		dp = (संरचना ext2_dir_entry *)(((वर्ण *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		यदि (rec_len != fs->blocksize - 12)
			प्रत्यागम EXT2_ET_DB_NOT_FOUND;
		मूळ = (संरचना ext2_dx_मूळ_info *)(((वर्ण *)dp + 12));
		यदि (मूळ->reserved_zero ||
		    मूळ->info_length != परिमाण(संरचना ext2_dx_मूळ_info))
			प्रत्यागम EXT2_ET_DB_NOT_FOUND;
		मोजणी_offset = 32;
	} अन्यथा
		प्रत्यागम EXT2_ET_DB_NOT_FOUND;

	c = (संरचना ext2_dx_countlimit *)(((वर्ण *)dirent) + मोजणी_offset);
	max_sane_entries = (fs->blocksize - मोजणी_offset) /
			   परिमाण(संरचना ext2_dx_entry);
	यदि (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->मोजणी) > max_sane_entries)
		प्रत्यागम EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	यदि (offset)
		*offset = मोजणी_offset;
	यदि (cc)
		*cc = c;

	प्रत्यागम 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   संरचना ext2_dir_entry *dirent,
				   संरचना ext2_dx_countlimit **cc,
				   पूर्णाङ्क *offset)
{
	प्रत्यागम __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

शून्य ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   संरचना ext2_dir_entry_tail *t)
{
	memset(t, 0, परिमाण(संरचना ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, परिमाण(संरचना ext2_dir_entry_tail),
			   (संरचना ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

स्थिर errcode_t __get_dirent_tail(ext2_filsys fs,
				   संरचना ext2_dir_entry *dirent,
				   संरचना ext2_dir_entry_tail **tt,
				   पूर्णाङ्क need_swab)
{
	संरचना ext2_dir_entry *d;
	शून्य *शीर्ष;
	संरचना ext2_dir_entry_tail *t;
	unsigned पूर्णाङ्क rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	यदि (fs->blocksize < 1024)
		प्रत्यागम EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	शीर्ष = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	यावत् ((शून्य *) d < शीर्ष) {
		rec_len = translate(d->rec_len);
		यदि ((rec_len < 8) || (rec_len & 0x03))
			प्रत्यागम EXT2_ET_DIR_CORRUPTED;
		d = (संरचना ext2_dir_entry *)(((वर्ण *)d) + rec_len);
	}

	यदि ((वर्ण *)d > ((वर्ण *)dirent + fs->blocksize))
			प्रत्यागम EXT2_ET_DIR_CORRUPTED;
	यदि (d != शीर्ष)
		प्रत्यागम EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (संरचना ext2_dir_entry_tail *)d;
	यदि (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != परिमाण(संरचना ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		प्रत्यागम EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	यदि (tt)
		*tt = t;
	प्रत्यागम retval;
}

पूर्णाङ्क ext2fs_dirent_has_tail(ext2_filsys fs, संरचना ext2_dir_entry *dirent)
{
	प्रत्यागम __get_dirent_tail(fs, dirent, रिक्त, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

स्थिर errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_dir_entry *dirent, __u32 *crc,
				    पूर्णाङ्क size)
{
	errcode_t retval;
	वर्ण *buf = (वर्ण *)dirent;
	__u32 gen;
	संरचना ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	यदि (retval)
		प्रत्यागम retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)&inum,
				परिमाण(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)&gen, परिमाण(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)buf, size);

	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      संरचना ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	संरचना ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	यदि (retval)
		प्रत्यागम 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (वर्ण *)t - (वर्ण *)dirent);
	यदि (retval)
		प्रत्यागम 0;
	प्रत्यागम ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

स्थिर errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					संरचना ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	संरचना ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	यदि (retval)
		प्रत्यागम retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (वर्ण *)t - (वर्ण *)dirent);
	यदि (retval)
		प्रत्यागम retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	प्रत्यागम 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 संरचना ext2_dir_entry *dirent,
			 __u32 *crc, संरचना ext2_dx_tail **ret_t)
{
	errcode_t retval;
	वर्ण *buf = (वर्ण *)dirent;
	पूर्णाङ्क size;
	__u32 gen, dummy_csum = 0;
	संरचना ext2_inode inode;
	संरचना ext2_dx_tail *t;
	संरचना ext2_dx_countlimit *c;
	पूर्णाङ्क मोजणी_offset, limit, मोजणी;

	retval = __get_dx_countlimit(fs, dirent, &c, &मोजणी_offset, 1);
	यदि (retval)
		प्रत्यागम retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	मोजणी = ext2fs_le16_to_cpu(c->मोजणी);
	यदि (मोजणी_offset + (limit * परिमाण(संरचना ext2_dx_entry)) >
	    fs->blocksize - परिमाण(संरचना ext2_dx_tail))
		प्रत्यागम EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (संरचना ext2_dx_tail *)(((संरचना ext2_dx_entry *)c) + limit);

	size = मोजणी_offset + (मोजणी * परिमाण(संरचना ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	यदि (retval)
		प्रत्यागम retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)&inum,
				परिमाण(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)&gen, परिमाण(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)&dummy_csum, 4);

	यदि (ret_t)
		*ret_t = t;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 संरचना ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	संरचना ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	यदि (retval)
		प्रत्यागम 0;

	प्रत्यागम ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

स्थिर errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	संरचना ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	यदि (retval)
		प्रत्यागम retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	प्रत्यागम retval;
}

पूर्णाङ्क ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 संरचना ext2_dir_entry *dirent)
{
	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	यदि (__get_dirent_tail(fs, dirent, रिक्त, 1) == 0)
		प्रत्यागम ext2fs_dirent_csum_verify(fs, inum, dirent);
	यदि (__get_dx_countlimit(fs, dirent, रिक्त, रिक्त, 1) == 0)
		प्रत्यागम ext2fs_dx_csum_verify(fs, inum, dirent);

	प्रत्यागम 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_dir_entry *dirent)
{
	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	यदि (__get_dirent_tail(fs, dirent, रिक्त, 1) == 0)
		प्रत्यागम ext2fs_dirent_csum_set(fs, inum, dirent);
	यदि (__get_dx_countlimit(fs, dirent, रिक्त, रिक्त, 1) == 0)
		प्रत्यागम ext2fs_dx_csum_set(fs, inum, dirent);

	यदि (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		प्रत्यागम 0;
	प्रत्यागम EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(परिमाण(संरचना ext3_extent_header) + \
	(परिमाण(संरचना ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

स्थिर संरचना ext3_extent_tail *get_extent_tail(संरचना ext3_extent_header *h)
{
	प्रत्यागम (संरचना ext3_extent_tail *)(((वर्ण *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

स्थिर errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  संरचना ext3_extent_header *eh,
					  __u32 *crc)
{
	पूर्णाङ्क size;
	__u32 gen;
	errcode_t retval;
	संरचना ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + offsetof(संरचना ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	यदि (retval)
		प्रत्यागम retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)&inum,
				परिमाण(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)&gen, परिमाण(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)eh, size);

	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	संरचना ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	यदि (retval)
		प्रत्यागम 0;

	प्रत्यागम provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       संरचना ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	संरचना ext3_extent_tail *t = get_extent_tail(eh);

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	यदि (retval)
		प्रत्यागम retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	प्रत्यागम retval;
}

पूर्णाङ्क ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    वर्ण *bitmap, पूर्णाङ्क size)
{
	संरचना ext4_group_desc *gdp = (संरचना ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)bitmap,
				      size);
	यदि (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	अन्यथा
		calculated &= 0xFFFF;

	प्रत्यागम provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       वर्ण *bitmap, पूर्णाङ्क size)
{
	__u32 crc;
	संरचना ext4_group_desc *gdp = (संरचना ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	यदि (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    वर्ण *bitmap, पूर्णाङ्क size)
{
	संरचना ext4_group_desc *gdp = (संरचना ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)bitmap,
				      size);
	यदि (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	अन्यथा
		calculated &= 0xFFFF;

	प्रत्यागम provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       वर्ण *bitmap, पूर्णाङ्क size)
{
	__u32 crc;
	संरचना ext4_group_desc *gdp = (संरचना ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	यदि (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	प्रत्यागम 0;
}

स्थिर errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       संरचना ext2_inode_large *inode,
			       __u32 *crc, पूर्णाङ्क has_hi)
{
	__u32 gen;
	संरचना ext2_inode_large *desc = inode;
	size_t size = EXT2_INODE_SIZE(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	यदि (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)&inum,
				परिमाण(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)&gen, परिमाण(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned वर्ण *)desc, size);

	inode->i_checksum_lo = old_lo;
	यदि (has_hi)
		inode->i_checksum_hi = old_hi;
	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     संरचना ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned पूर्णाङ्क i, has_hi;
	वर्ण *cp;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	यदि (retval)
		प्रत्यागम 0;
	यदि (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} अन्यथा
		calculated &= 0xFFFF;

	यदि (provided == calculated)
		प्रत्यागम 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	प्रति (cp = (वर्ण *) inode, i = 0;
	     i < परिमाण(संरचना ext2_inode);
	     cp++, i++)
		यदि (*cp)
			प्रत्यागम 0;
	प्रत्यागम 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   संरचना ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	पूर्णाङ्क has_hi;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	यदि (retval)
		प्रत्यागम retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	यदि (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	प्रत्यागम 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	संरचना ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef शब्द_BIGENDIAN
	संरचना ext4_group_desc swabdesc;
	size_t save_size = size;
	ध्रुव size_t ext4_bg_size = परिमाण(संरचना ext4_group_desc);
	संरचना ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	यदि (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (संरचना ext2_group_desc *) &swabdesc);
	desc = (संरचना ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	यदि (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)&group,
					 परिमाण(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned वर्ण *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef शब्द_BIGENDIAN
		यदि (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned वर्ण *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		गच्छ out;
	}

	/* old crc16 code */
	offset = offsetof(संरचना ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   परिमाण(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, परिमाण(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += परिमाण(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	यदि (offset < size) {
		crc = ext2fs_crc16(crc, (वर्ण *)desc + offset,
				   size - offset);
	}
#ifdef शब्द_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	यदि (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (वर्ण *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	प्रत्यागम crc;
}

पूर्णाङ्क ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	यदि (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		प्रत्यागम 0;

	प्रत्यागम 1;
}

शून्य ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	यदि (!ext2fs_has_group_desc_csum(fs))
		प्रत्यागम;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

स्थिर __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	प्रति (i = end_ino; i >= start_ino; i--) {
		यदि (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			प्रत्यागम i - start_ino + 1;
	}
	प्रत्यागम inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	संरचना ext2_super_block *sb = fs->super;
	पूर्णाङ्क dirty = 0;
	dgrp_t i;

	यदि (!fs->inode_map)
		प्रत्यागम EXT2_ET_NO_INODE_BITMAP;

	यदि (!ext2fs_has_group_desc_csum(fs))
		प्रत्यागम 0;

	प्रति (i = 0; i < fs->group_desc_मोजणी; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_मोजणी = ext2fs_bg_free_inodes_मोजणी(fs, i);
		__u32 old_free_blocks_मोजणी = ext2fs_bg_free_blocks_मोजणी(fs, i);

		यदि (old_free_blocks_मोजणी == sb->s_blocks_per_group &&
		    i != fs->group_desc_मोजणी - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		यदि (old_free_inodes_मोजणी == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} अन्यथा {
			पूर्णाङ्क unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		यदि (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		यदि (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		यदि (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	यदि (dirty)
		ext2fs_mark_super_dirty(fs);
	प्रत्यागम 0;
}

#ifdef DEBUG
#अन्तर्भाव "e2p/e2p.h"

शून्य print_csum(ध्रुव वर्ण *संदेश, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	संरचना ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t size = EXT2_DESC_SIZE(fs->super);
	संरचना ext2_super_block *sb = fs->super;
	पूर्णाङ्क offset = offsetof(संरचना ext2_group_desc, bg_checksum);
#ifdef शब्द_BIGENDIAN
	संरचना ext4_group_desc swabdesc;
	संरचना ext2_group_desc *save_desc = desc;
	ध्रुव size_t ext4_bg_size = परिमाण(संरचना ext4_group_desc);
	size_t save_size = size;
#endif

#ifdef शब्द_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	यदि (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (संरचना ext2_group_desc *) &swabdesc);
	desc = (संरचना ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#अन्यथा
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, परिमाण(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, परिमाण(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += परिमाण(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	यदि (offset < size)
		crc3 = ext2fs_crc16(crc3, (वर्ण *)desc + offset, size - offset);
#ifdef शब्द_BIGENDIAN
	यदि (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (वर्ण *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       संदेश, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned वर्ण sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	संरचना ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	पूर्णाङ्क			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, परिमाण(param));
	ext2fs_blocks_मोजणी_set(&param, 32768);
#यदि 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	यदि (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	memcpy(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	प्रति (i=0; i < fs->group_desc_मोजणी; i++) {
		ext2fs_block_bitmap_loc_set(fs, i, 124);
		ext2fs_inode_bitmap_loc_set(fs, i, 125);
		ext2fs_inode_table_loc_set(fs, i, 126);
		ext2fs_bg_free_blocks_मोजणी_set(fs, i, 31119);
		ext2fs_bg_free_inodes_मोजणी_set(fs, i, 15701);
		ext2fs_bg_used_dirs_मोजणी_set(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	यदि (csum1 != csum_known) {
		printf("checksum for group 0 should be %04x\n", csum_known);
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	यदि (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	यदि (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	यदि (csum1 != csum2) {
		printf("checksums should not depend on checksum field\n");
		exit(1);
	}
	यदि (!ext2fs_group_desc_csum_verify(fs, 0)) {
		printf("checksums should verify against gd_checksum\n");
		exit(1);
	}
	memset(fs->super->s_uuid, 0x30, परिमाण(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	यदि (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		printf("checksums for different filesystems shouldn't match\n");
		exit(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_मोजणी_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	यदि (csum1 == csum2) {
		printf("checksums for different data shouldn't match\n");
		exit(1);
	}
	ext2fs_free(fs);

	प्रत्यागम 0;
}
#endif
