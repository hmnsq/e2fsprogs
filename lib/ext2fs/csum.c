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
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "crc16.h"
#अन्तर्भाव <assert.h>

#ifndef दुरत्व
#define दुरत्व(TYPE, MEMBER) ((माप_प्रकार) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#अन्यथा
#define STATIC स्थिर
#endif

शून्य ext2fs_init_csum_seed(ext2_filsys fs)
{
	जर (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	अन्यथा जर (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 आकार(fs->super->s_uuid));
}

स्थिर __u32 ext2fs_mmp_csum(ext2_filsys fs, संरचना mmp_struct *mmp)
{
	पूर्णांक offset = दुरत्व(संरचना mmp_struct, mmp_checksum);

	परत ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)mmp, offset);
}

पूर्णांक ext2fs_mmp_csum_verify(ext2_filsys fs, संरचना mmp_struct *mmp)
{
	__u32 calculated;

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	परत ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, संरचना mmp_struct *mmp)
{
	__u32 crc;

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	परत 0;
}

पूर्णांक ext2fs_verify_csum_type(ext2_filsys fs, संरचना ext2_super_block *sb)
{
	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 1;

	परत sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

स्थिर __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    संरचना ext2_super_block *sb)
{
	पूर्णांक offset = दुरत्व(संरचना ext2_super_block, s_checksum);

	परत ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
पूर्णांक ext2fs_superblock_csum_verify(ext2_filsys fs, संरचना ext2_super_block *sb)
{
	__u32 flag, calculated;

	जर (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	अन्यथा
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	जर (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		परत 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	परत ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     संरचना ext2_super_block *sb)
{
	__u32 flag, crc;

	जर (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	अन्यथा
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	जर (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		परत 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	परत 0;
}

स्थिर errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    संरचना ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	अक्षर *buf = (अक्षर *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&block,
				आकार(block));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	परत 0;
}

पूर्णांक ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      संरचना ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	जर (retval)
		परत 0;

	परत ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 संरचना ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	जर (retval)
		परत retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	परत 0;
}

स्थिर __u16 do_nothing16(__u16 x)
{
	परत x;
}

स्थिर __u16 disk_to_host16(__u16 x)
{
	परत ext2fs_le16_to_cpu(x);
}

स्थिर errcode_t __get_dx_countlimit(ext2_filsys fs,
				     संरचना ext2_dir_entry *dirent,
				     संरचना ext2_dx_countlimit **cc,
				     पूर्णांक *offset,
				     पूर्णांक need_swab)
{
	संरचना ext2_dir_entry *dp;
	संरचना ext2_dx_मूळ_info *मूळ;
	संरचना ext2_dx_countlimit *c;
	पूर्णांक मोजणी_offset, max_sane_entries;
	अचिन्हित पूर्णांक rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	जर (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		मोजणी_offset = 8;
	अन्यथा जर (rec_len == 12) {
		dp = (संरचना ext2_dir_entry *)(((अक्षर *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		जर (rec_len != fs->blocksize - 12)
			परत EXT2_ET_DB_NOT_FOUND;
		मूळ = (संरचना ext2_dx_मूळ_info *)(((अक्षर *)dp + 12));
		जर (मूळ->reserved_zero ||
		    मूळ->info_length != आकार(संरचना ext2_dx_मूळ_info))
			परत EXT2_ET_DB_NOT_FOUND;
		मोजणी_offset = 32;
	} अन्यथा
		परत EXT2_ET_DB_NOT_FOUND;

	c = (संरचना ext2_dx_countlimit *)(((अक्षर *)dirent) + मोजणी_offset);
	max_sane_entries = (fs->blocksize - मोजणी_offset) /
			   आकार(संरचना ext2_dx_entry);
	जर (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->मोजणी) > max_sane_entries)
		परत EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	जर (offset)
		*offset = मोजणी_offset;
	जर (cc)
		*cc = c;

	परत 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   संरचना ext2_dir_entry *dirent,
				   संरचना ext2_dx_countlimit **cc,
				   पूर्णांक *offset)
{
	परत __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

शून्य ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   संरचना ext2_dir_entry_tail *t)
{
	स_रखो(t, 0, आकार(संरचना ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, आकार(संरचना ext2_dir_entry_tail),
			   (संरचना ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

स्थिर errcode_t __get_dirent_tail(ext2_filsys fs,
				   संरचना ext2_dir_entry *dirent,
				   संरचना ext2_dir_entry_tail **tt,
				   पूर्णांक need_swab)
{
	संरचना ext2_dir_entry *d;
	शून्य *शीर्ष;
	संरचना ext2_dir_entry_tail *t;
	अचिन्हित पूर्णांक rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	जर (fs->blocksize < 1024)
		परत EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	शीर्ष = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	पर्यंत ((शून्य *) d < शीर्ष) {
		rec_len = translate(d->rec_len);
		जर ((rec_len < 8) || (rec_len & 0x03))
			परत EXT2_ET_DIR_CORRUPTED;
		d = (संरचना ext2_dir_entry *)(((अक्षर *)d) + rec_len);
	}

	जर ((अक्षर *)d > ((अक्षर *)dirent + fs->blocksize))
			परत EXT2_ET_DIR_CORRUPTED;
	जर (d != शीर्ष)
		परत EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (संरचना ext2_dir_entry_tail *)d;
	जर (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != आकार(संरचना ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		परत EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	जर (tt)
		*tt = t;
	परत retval;
}

पूर्णांक ext2fs_dirent_has_tail(ext2_filsys fs, संरचना ext2_dir_entry *dirent)
{
	परत __get_dirent_tail(fs, dirent, रिक्त, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

स्थिर errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_dir_entry *dirent, __u32 *crc,
				    पूर्णांक size)
{
	errcode_t retval;
	अक्षर *buf = (अक्षर *)dirent;
	__u32 gen;
	संरचना ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	जर (retval)
		परत retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&inum,
				आकार(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)&gen, आकार(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)buf, size);

	परत 0;
}

पूर्णांक ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      संरचना ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	संरचना ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	जर (retval)
		परत 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (अक्षर *)t - (अक्षर *)dirent);
	जर (retval)
		परत 0;
	परत ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

स्थिर errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					संरचना ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	संरचना ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	जर (retval)
		परत retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (अक्षर *)t - (अक्षर *)dirent);
	जर (retval)
		परत retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	परत 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 संरचना ext2_dir_entry *dirent,
			 __u32 *crc, संरचना ext2_dx_tail **ret_t)
{
	errcode_t retval;
	अक्षर *buf = (अक्षर *)dirent;
	पूर्णांक size;
	__u32 gen, dummy_csum = 0;
	संरचना ext2_inode inode;
	संरचना ext2_dx_tail *t;
	संरचना ext2_dx_countlimit *c;
	पूर्णांक मोजणी_offset, limit, मोजणी;

	retval = __get_dx_countlimit(fs, dirent, &c, &मोजणी_offset, 1);
	जर (retval)
		परत retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	मोजणी = ext2fs_le16_to_cpu(c->मोजणी);
	जर (मोजणी_offset + (limit * आकार(संरचना ext2_dx_entry)) >
	    fs->blocksize - आकार(संरचना ext2_dx_tail))
		परत EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (संरचना ext2_dx_tail *)(((संरचना ext2_dx_entry *)c) + limit);

	size = मोजणी_offset + (मोजणी * आकार(संरचना ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	जर (retval)
		परत retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&inum,
				आकार(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)&gen, आकार(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)&dummy_csum, 4);

	जर (ret_t)
		*ret_t = t;
	परत 0;
}

स्थिर पूर्णांक ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 संरचना ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	संरचना ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	जर (retval)
		परत 0;

	परत ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

स्थिर errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	संरचना ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	जर (retval)
		परत retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	परत retval;
}

पूर्णांक ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 संरचना ext2_dir_entry *dirent)
{
	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 1;

	जर (__get_dirent_tail(fs, dirent, रिक्त, 1) == 0)
		परत ext2fs_dirent_csum_verify(fs, inum, dirent);
	जर (__get_dx_countlimit(fs, dirent, रिक्त, रिक्त, 1) == 0)
		परत ext2fs_dx_csum_verify(fs, inum, dirent);

	परत 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_dir_entry *dirent)
{
	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 0;

	जर (__get_dirent_tail(fs, dirent, रिक्त, 1) == 0)
		परत ext2fs_dirent_csum_set(fs, inum, dirent);
	जर (__get_dx_countlimit(fs, dirent, रिक्त, रिक्त, 1) == 0)
		परत ext2fs_dx_csum_set(fs, inum, dirent);

	जर (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		परत 0;
	परत EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(आकार(संरचना ext3_extent_header) + \
	(आकार(संरचना ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

स्थिर संरचना ext3_extent_tail *get_extent_tail(संरचना ext3_extent_header *h)
{
	परत (संरचना ext3_extent_tail *)(((अक्षर *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

स्थिर errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  संरचना ext3_extent_header *eh,
					  __u32 *crc)
{
	पूर्णांक size;
	__u32 gen;
	errcode_t retval;
	संरचना ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + दुरत्व(संरचना ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	जर (retval)
		परत retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&inum,
				आकार(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)&gen, आकार(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)eh, size);

	परत 0;
}

पूर्णांक ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	संरचना ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	जर (retval)
		परत 0;

	परत provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       संरचना ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	संरचना ext3_extent_tail *t = get_extent_tail(eh);

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	जर (retval)
		परत retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	परत retval;
}

पूर्णांक ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    अक्षर *bitmap, पूर्णांक size)
{
	संरचना ext4_group_desc *gdp = (संरचना ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)bitmap,
				      size);
	जर (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	अन्यथा
		calculated &= 0xFFFF;

	परत provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       अक्षर *bitmap, पूर्णांक size)
{
	__u32 crc;
	संरचना ext4_group_desc *gdp = (संरचना ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	जर (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	परत 0;
}

पूर्णांक ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    अक्षर *bitmap, पूर्णांक size)
{
	संरचना ext4_group_desc *gdp = (संरचना ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)bitmap,
				      size);
	जर (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	अन्यथा
		calculated &= 0xFFFF;

	परत provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       अक्षर *bitmap, पूर्णांक size)
{
	__u32 crc;
	संरचना ext4_group_desc *gdp = (संरचना ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	जर (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	परत 0;
}

स्थिर errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       संरचना ext2_inode_large *inode,
			       __u32 *crc, पूर्णांक has_hi)
{
	__u32 gen;
	संरचना ext2_inode_large *desc = inode;
	माप_प्रकार size = EXT2_INODE_SIZE(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	जर (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&inum,
				आकार(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)&gen, आकार(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)desc, size);

	inode->i_checksum_lo = old_lo;
	जर (has_hi)
		inode->i_checksum_hi = old_hi;
	परत 0;
}

पूर्णांक ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     संरचना ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	अचिन्हित पूर्णांक i, has_hi;
	अक्षर *cp;

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	जर (retval)
		परत 0;
	जर (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} अन्यथा
		calculated &= 0xFFFF;

	जर (provided == calculated)
		परत 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	साठी (cp = (अक्षर *) inode, i = 0;
	     i < आकार(संरचना ext2_inode);
	     cp++, i++)
		जर (*cp)
			परत 0;
	परत 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   संरचना ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	पूर्णांक has_hi;

	जर (!ext2fs_has_feature_metadata_csum(fs->super))
		परत 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	जर (retval)
		परत retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	जर (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	परत 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	संरचना ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	माप_प्रकार offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef शब्द_BIGENDIAN
	संरचना ext4_group_desc swabdesc;
	माप_प्रकार save_size = size;
	स्थिरांक माप_प्रकार ext4_bg_size = आकार(संरचना ext4_group_desc);
	संरचना ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	जर (size > ext4_bg_size)
		size = ext4_bg_size;
	स_नकल(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (संरचना ext2_group_desc *) &swabdesc);
	desc = (संरचना ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	जर (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&group,
					 आकार(group));
		crc32 = ext2fs_crc32c_le(crc32, (अचिन्हित अक्षर *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef शब्द_BIGENDIAN
		जर (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (अचिन्हित अक्षर *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		जा out;
	}

	/* old crc16 code */
	offset = दुरत्व(संरचना ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   आकार(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, आकार(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += आकार(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	जर (offset < size) {
		crc = ext2fs_crc16(crc, (अक्षर *)desc + offset,
				   size - offset);
	}
#ifdef शब्द_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	जर (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (अक्षर *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	परत crc;
}

पूर्णांक ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	जर (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		परत 0;

	परत 1;
}

शून्य ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	जर (!ext2fs_has_group_desc_csum(fs))
		परत;

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

	साठी (i = end_ino; i >= start_ino; i--) {
		जर (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			परत i - start_ino + 1;
	}
	परत inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	संरचना ext2_super_block *sb = fs->super;
	पूर्णांक dirty = 0;
	dgrp_t i;

	जर (!fs->inode_map)
		परत EXT2_ET_NO_INODE_BITMAP;

	जर (!ext2fs_has_group_desc_csum(fs))
		परत 0;

	साठी (i = 0; i < fs->group_desc_मोजणी; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_मोजणी = ext2fs_bg_free_inodes_मोजणी(fs, i);
		__u32 old_free_blocks_मोजणी = ext2fs_bg_free_blocks_मोजणी(fs, i);

		जर (old_free_blocks_मोजणी == sb->s_blocks_per_group &&
		    i != fs->group_desc_मोजणी - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		जर (old_free_inodes_मोजणी == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} अन्यथा {
			पूर्णांक unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		जर (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		जर (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		जर (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	जर (dirty)
		ext2fs_mark_super_dirty(fs);
	परत 0;
}

#ifdef DEBUG
#अन्तर्भाव "e2p/e2p.h"

शून्य print_csum(स्थिरांक अक्षर *संदेश, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	संरचना ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	माप_प्रकार size = EXT2_DESC_SIZE(fs->super);
	संरचना ext2_super_block *sb = fs->super;
	पूर्णांक offset = दुरत्व(संरचना ext2_group_desc, bg_checksum);
#ifdef शब्द_BIGENDIAN
	संरचना ext4_group_desc swabdesc;
	संरचना ext2_group_desc *save_desc = desc;
	स्थिरांक माप_प्रकार ext4_bg_size = आकार(संरचना ext4_group_desc);
	माप_प्रकार save_size = size;
#endif

#ifdef शब्द_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	जर (size > ext4_bg_size)
		size = ext4_bg_size;
	स_नकल(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (संरचना ext2_group_desc *) &swabdesc);
	desc = (संरचना ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#अन्यथा
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, आकार(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, आकार(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += आकार(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	जर (offset < size)
		crc3 = ext2fs_crc16(crc3, (अक्षर *)desc + offset, size - offset);
#ifdef शब्द_BIGENDIAN
	जर (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (अक्षर *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	म_लिखो("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       संदेश, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

अचिन्हित अक्षर sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

पूर्णांक main(पूर्णांक argc, अक्षर **argv)
{
	संरचना ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	पूर्णांक			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	स_रखो(&param, 0, आकार(param));
	ext2fs_blocks_मोजणी_set(&param, 32768);
#जर 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	जर (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		निकास(1);
	}
	स_नकल(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	साठी (i=0; i < fs->group_desc_मोजणी; i++) {
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

	जर (csum1 != csum_known) {
		म_लिखो("checksum for group 0 should be %04x\n", csum_known);
		निकास(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	जर (csum1 == csum2) {
		म_लिखो("checksums for different groups shouldn't match\n");
		निकास(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	जर (csum1 == csum2) {
		म_लिखो("checksums for different groups shouldn't match\n");
		निकास(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	जर (csum1 != csum2) {
		म_लिखो("checksums should not depend on checksum field\n");
		निकास(1);
	}
	जर (!ext2fs_group_desc_csum_verify(fs, 0)) {
		म_लिखो("checksums should verify against gd_checksum\n");
		निकास(1);
	}
	स_रखो(fs->super->s_uuid, 0x30, आकार(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	जर (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		म_लिखो("checksums for different filesystems shouldn't match\n");
		निकास(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_मोजणी_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	जर (csum1 == csum2) {
		म_लिखो("checksums for different data shouldn't match\n");
		निकास(1);
	}
	ext2fs_free(fs);

	परत 0;
}
#endif
