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

#समावेश "config.h"
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "crc16.h"
#समावेश <assert.h>

#अगर_अघोषित दुरत्व
#घोषणा दुरत्व(TYPE, MEMBER) ((माप_प्रकार) &((TYPE *)0)->MEMBER)
#पूर्ण_अगर

#अगर_घोषित DEBUG
#घोषणा STATIC
#अन्यथा
#घोषणा STATIC अटल
#पूर्ण_अगर

व्योम ext2fs_init_csum_seed(ext2_filsys fs)
{
	अगर (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	अन्यथा अगर (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 माप(fs->super->s_uuid));
}

अटल __u32 ext2fs_mmp_csum(ext2_filsys fs, काष्ठा mmp_struct *mmp)
{
	पूर्णांक offset = दुरत्व(काष्ठा mmp_struct, mmp_checksum);

	वापस ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)mmp, offset);
}

पूर्णांक ext2fs_mmp_csum_verify(ext2_filsys fs, काष्ठा mmp_struct *mmp)
{
	__u32 calculated;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	वापस ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, काष्ठा mmp_struct *mmp)
{
	__u32 crc;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	वापस 0;
}

पूर्णांक ext2fs_verify_csum_type(ext2_filsys fs, काष्ठा ext2_super_block *sb)
{
	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 1;

	वापस sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

अटल __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    काष्ठा ext2_super_block *sb)
{
	पूर्णांक offset = दुरत्व(काष्ठा ext2_super_block, s_checksum);

	वापस ext2fs_crc32c_le(~0, (अचिन्हित अक्षर *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
पूर्णांक ext2fs_superblock_csum_verify(ext2_filsys fs, काष्ठा ext2_super_block *sb)
{
	__u32 flag, calculated;

	अगर (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	अन्यथा
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	अगर (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		वापस 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	वापस ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     काष्ठा ext2_super_block *sb)
{
	__u32 flag, crc;

	अगर (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	अन्यथा
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	अगर (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		वापस 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	वापस 0;
}

अटल errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    काष्ठा ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	अक्षर *buf = (अक्षर *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&block,
				माप(block));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	वापस 0;
}

पूर्णांक ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      काष्ठा ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	अगर (retval)
		वापस 0;

	वापस ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 काष्ठा ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	अगर (retval)
		वापस retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	वापस 0;
}

अटल __u16 do_nothing16(__u16 x)
{
	वापस x;
}

अटल __u16 disk_to_host16(__u16 x)
{
	वापस ext2fs_le16_to_cpu(x);
}

अटल errcode_t __get_dx_countlimit(ext2_filsys fs,
				     काष्ठा ext2_dir_entry *dirent,
				     काष्ठा ext2_dx_countlimit **cc,
				     पूर्णांक *offset,
				     पूर्णांक need_swab)
{
	काष्ठा ext2_dir_entry *dp;
	काष्ठा ext2_dx_मूल_info *मूल;
	काष्ठा ext2_dx_countlimit *c;
	पूर्णांक गिनती_offset, max_sane_entries;
	अचिन्हित पूर्णांक rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	अगर (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		गिनती_offset = 8;
	अन्यथा अगर (rec_len == 12) {
		dp = (काष्ठा ext2_dir_entry *)(((अक्षर *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		अगर (rec_len != fs->blocksize - 12)
			वापस EXT2_ET_DB_NOT_FOUND;
		मूल = (काष्ठा ext2_dx_मूल_info *)(((अक्षर *)dp + 12));
		अगर (मूल->reserved_zero ||
		    मूल->info_लंबाई != माप(काष्ठा ext2_dx_मूल_info))
			वापस EXT2_ET_DB_NOT_FOUND;
		गिनती_offset = 32;
	} अन्यथा
		वापस EXT2_ET_DB_NOT_FOUND;

	c = (काष्ठा ext2_dx_countlimit *)(((अक्षर *)dirent) + गिनती_offset);
	max_sane_entries = (fs->blocksize - गिनती_offset) /
			   माप(काष्ठा ext2_dx_entry);
	अगर (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->गिनती) > max_sane_entries)
		वापस EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	अगर (offset)
		*offset = गिनती_offset;
	अगर (cc)
		*cc = c;

	वापस 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   काष्ठा ext2_dir_entry *dirent,
				   काष्ठा ext2_dx_countlimit **cc,
				   पूर्णांक *offset)
{
	वापस __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

व्योम ext2fs_initialize_dirent_पुच्छ(ext2_filsys fs,
				   काष्ठा ext2_dir_entry_पुच्छ *t)
{
	स_रखो(t, 0, माप(काष्ठा ext2_dir_entry_पुच्छ));
	ext2fs_set_rec_len(fs, माप(काष्ठा ext2_dir_entry_पुच्छ),
			   (काष्ठा ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

अटल errcode_t __get_dirent_tail(ext2_filsys fs,
				   काष्ठा ext2_dir_entry *dirent,
				   काष्ठा ext2_dir_entry_पुच्छ **tt,
				   पूर्णांक need_swab)
{
	काष्ठा ext2_dir_entry *d;
	व्योम *शीर्ष;
	काष्ठा ext2_dir_entry_पुच्छ *t;
	अचिन्हित पूर्णांक rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	अगर (fs->blocksize < 1024)
		वापस EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	शीर्ष = EXT2_DIRENT_पुच्छ(dirent, fs->blocksize);

	जबतक ((व्योम *) d < शीर्ष) {
		rec_len = translate(d->rec_len);
		अगर ((rec_len < 8) || (rec_len & 0x03))
			वापस EXT2_ET_DIR_CORRUPTED;
		d = (काष्ठा ext2_dir_entry *)(((अक्षर *)d) + rec_len);
	}

	अगर ((अक्षर *)d > ((अक्षर *)dirent + fs->blocksize))
			वापस EXT2_ET_DIR_CORRUPTED;
	अगर (d != शीर्ष)
		वापस EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (काष्ठा ext2_dir_entry_पुच्छ *)d;
	अगर (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != माप(काष्ठा ext2_dir_entry_पुच्छ) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		वापस EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	अगर (tt)
		*tt = t;
	वापस retval;
}

पूर्णांक ext2fs_dirent_has_पुच्छ(ext2_filsys fs, काष्ठा ext2_dir_entry *dirent)
{
	वापस __get_dirent_tail(fs, dirent, शून्य, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

अटल errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    काष्ठा ext2_dir_entry *dirent, __u32 *crc,
				    पूर्णांक आकार)
{
	errcode_t retval;
	अक्षर *buf = (अक्षर *)dirent;
	__u32 gen;
	काष्ठा ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	अगर (retval)
		वापस retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&inum,
				माप(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)&gen, माप(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)buf, आकार);

	वापस 0;
}

पूर्णांक ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      काष्ठा ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	काष्ठा ext2_dir_entry_पुच्छ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	अगर (retval)
		वापस 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (अक्षर *)t - (अक्षर *)dirent);
	अगर (retval)
		वापस 0;
	वापस ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

अटल errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					काष्ठा ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	काष्ठा ext2_dir_entry_पुच्छ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	अगर (retval)
		वापस retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (अक्षर *)t - (अक्षर *)dirent);
	अगर (retval)
		वापस retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	वापस 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 काष्ठा ext2_dir_entry *dirent,
			 __u32 *crc, काष्ठा ext2_dx_पुच्छ **ret_t)
{
	errcode_t retval;
	अक्षर *buf = (अक्षर *)dirent;
	पूर्णांक आकार;
	__u32 gen, dummy_csum = 0;
	काष्ठा ext2_inode inode;
	काष्ठा ext2_dx_पुच्छ *t;
	काष्ठा ext2_dx_countlimit *c;
	पूर्णांक गिनती_offset, limit, गिनती;

	retval = __get_dx_countlimit(fs, dirent, &c, &गिनती_offset, 1);
	अगर (retval)
		वापस retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	गिनती = ext2fs_le16_to_cpu(c->गिनती);
	अगर (गिनती_offset + (limit * माप(काष्ठा ext2_dx_entry)) >
	    fs->blocksize - माप(काष्ठा ext2_dx_पुच्छ))
		वापस EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (काष्ठा ext2_dx_पुच्छ *)(((काष्ठा ext2_dx_entry *)c) + limit);

	आकार = गिनती_offset + (गिनती * माप(काष्ठा ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	अगर (retval)
		वापस retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&inum,
				माप(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)&gen, माप(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)buf, आकार);
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)&dummy_csum, 4);

	अगर (ret_t)
		*ret_t = t;
	वापस 0;
}

अटल पूर्णांक ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 काष्ठा ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	काष्ठा ext2_dx_पुच्छ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	अगर (retval)
		वापस 0;

	वापस ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

अटल errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    काष्ठा ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	काष्ठा ext2_dx_पुच्छ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	अगर (retval)
		वापस retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	वापस retval;
}

पूर्णांक ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 काष्ठा ext2_dir_entry *dirent)
{
	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 1;

	अगर (__get_dirent_tail(fs, dirent, शून्य, 1) == 0)
		वापस ext2fs_dirent_csum_verify(fs, inum, dirent);
	अगर (__get_dx_countlimit(fs, dirent, शून्य, शून्य, 1) == 0)
		वापस ext2fs_dx_csum_verify(fs, inum, dirent);

	वापस 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    काष्ठा ext2_dir_entry *dirent)
{
	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 0;

	अगर (__get_dirent_tail(fs, dirent, शून्य, 1) == 0)
		वापस ext2fs_dirent_csum_set(fs, inum, dirent);
	अगर (__get_dx_countlimit(fs, dirent, शून्य, शून्य, 1) == 0)
		वापस ext2fs_dx_csum_set(fs, inum, dirent);

	अगर (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		वापस 0;
	वापस EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#घोषणा EXT3_EXTENT_पुच्छ_OFFSET(hdr)	(माप(काष्ठा ext3_extent_header) + \
	(माप(काष्ठा ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

अटल काष्ठा ext3_extent_पुच्छ *get_extent_पुच्छ(काष्ठा ext3_extent_header *h)
{
	वापस (काष्ठा ext3_extent_पुच्छ *)(((अक्षर *)h) +
					   EXT3_EXTENT_पुच्छ_OFFSET(h));
}

अटल errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  काष्ठा ext3_extent_header *eh,
					  __u32 *crc)
{
	पूर्णांक आकार;
	__u32 gen;
	errcode_t retval;
	काष्ठा ext2_inode inode;

	आकार = EXT3_EXTENT_पुच्छ_OFFSET(eh) + दुरत्व(काष्ठा ext3_extent_पुच्छ,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	अगर (retval)
		वापस retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&inum,
				माप(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)&gen, माप(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)eh, आकार);

	वापस 0;
}

पूर्णांक ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    काष्ठा ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	काष्ठा ext3_extent_पुच्छ *t = get_extent_पुच्छ(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	अगर (retval)
		वापस 0;

	वापस provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       काष्ठा ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	काष्ठा ext3_extent_पुच्छ *t = get_extent_पुच्छ(eh);

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	अगर (retval)
		वापस retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	वापस retval;
}

पूर्णांक ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    अक्षर *bitmap, पूर्णांक आकार)
{
	काष्ठा ext4_group_desc *gdp = (काष्ठा ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)bitmap,
				      आकार);
	अगर (EXT2_DESC_आकार(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	अन्यथा
		calculated &= 0xFFFF;

	वापस provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       अक्षर *bitmap, पूर्णांक आकार)
{
	__u32 crc;
	काष्ठा ext4_group_desc *gdp = (काष्ठा ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)bitmap, आकार);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	अगर (EXT2_DESC_आकार(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	वापस 0;
}

पूर्णांक ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    अक्षर *bitmap, पूर्णांक आकार)
{
	काष्ठा ext4_group_desc *gdp = (काष्ठा ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)bitmap,
				      आकार);
	अगर (EXT2_DESC_आकार(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	अन्यथा
		calculated &= 0xFFFF;

	वापस provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       अक्षर *bitmap, पूर्णांक आकार)
{
	__u32 crc;
	काष्ठा ext4_group_desc *gdp = (काष्ठा ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)bitmap, आकार);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	अगर (EXT2_DESC_आकार(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	वापस 0;
}

अटल errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       काष्ठा ext2_inode_large *inode,
			       __u32 *crc, पूर्णांक has_hi)
{
	__u32 gen;
	काष्ठा ext2_inode_large *desc = inode;
	माप_प्रकार आकार = EXT2_INODE_आकार(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	अगर (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&inum,
				माप(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)&gen, माप(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिन्हित अक्षर *)desc, आकार);

	inode->i_checksum_lo = old_lo;
	अगर (has_hi)
		inode->i_checksum_hi = old_hi;
	वापस 0;
}

पूर्णांक ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     काष्ठा ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	अचिन्हित पूर्णांक i, has_hi;
	अक्षर *cp;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 1;

	has_hi = (EXT2_INODE_आकार(fs->super) > EXT2_GOOD_OLD_INODE_आकार &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	अगर (retval)
		वापस 0;
	अगर (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} अन्यथा
		calculated &= 0xFFFF;

	अगर (provided == calculated)
		वापस 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	क्रम (cp = (अक्षर *) inode, i = 0;
	     i < माप(काष्ठा ext2_inode);
	     cp++, i++)
		अगर (*cp)
			वापस 0;
	वापस 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   काष्ठा ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	पूर्णांक has_hi;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 0;

	has_hi = (EXT2_INODE_आकार(fs->super) > EXT2_GOOD_OLD_INODE_आकार &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	अगर (retval)
		वापस retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	अगर (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	वापस 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	काष्ठा ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	माप_प्रकार offset, आकार = EXT2_DESC_आकार(fs->super);
	__u16 crc = 0;
#अगर_घोषित शब्दगण_BIGENDIAN
	काष्ठा ext4_group_desc swabdesc;
	माप_प्रकार save_आकार = आकार;
	स्थिर माप_प्रकार ext4_bg_आकार = माप(काष्ठा ext4_group_desc);
	काष्ठा ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	अगर (आकार > ext4_bg_आकार)
		आकार = ext4_bg_आकार;
	स_नकल(&swabdesc, desc, आकार);
	ext2fs_swap_group_desc2(fs, (काष्ठा ext2_group_desc *) &swabdesc);
	desc = (काष्ठा ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#पूर्ण_अगर

	अगर (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&group,
					 माप(group));
		crc32 = ext2fs_crc32c_le(crc32, (अचिन्हित अक्षर *)desc,
					 आकार);
		desc->bg_checksum = old_crc;
#अगर_घोषित शब्दगण_BIGENDIAN
		अगर (save_आकार > ext4_bg_आकार)
			crc32 = ext2fs_crc32c_le(crc32,
				     (अचिन्हित अक्षर *)save_desc + ext4_bg_आकार,
	                             save_आकार - ext4_bg_आकार);
#पूर्ण_अगर
		crc = crc32 & 0xFFFF;
		जाओ out;
	}

	/* old crc16 code */
	offset = दुरत्व(काष्ठा ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   माप(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, माप(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += माप(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	अगर (offset < आकार) {
		crc = ext2fs_crc16(crc, (अक्षर *)desc + offset,
				   आकार - offset);
	}
#अगर_घोषित शब्दगण_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	अगर (save_आकार > ext4_bg_आकार)
		crc = ext2fs_crc16(crc, (अक्षर *)save_desc + ext4_bg_आकार,
				   save_आकार - ext4_bg_आकार);
#पूर्ण_अगर

out:
	वापस crc;
}

पूर्णांक ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	अगर (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		वापस 0;

	वापस 1;
}

व्योम ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	अगर (!ext2fs_has_group_desc_csum(fs))
		वापस;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

अटल __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	क्रम (i = end_ino; i >= start_ino; i--) {
		अगर (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			वापस i - start_ino + 1;
	}
	वापस inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	काष्ठा ext2_super_block *sb = fs->super;
	पूर्णांक dirty = 0;
	dgrp_t i;

	अगर (!fs->inode_map)
		वापस EXT2_ET_NO_INODE_BITMAP;

	अगर (!ext2fs_has_group_desc_csum(fs))
		वापस 0;

	क्रम (i = 0; i < fs->group_desc_गिनती; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_गिनती = ext2fs_bg_free_inodes_गिनती(fs, i);
		__u32 old_free_blocks_गिनती = ext2fs_bg_free_blocks_गिनती(fs, i);

		अगर (old_free_blocks_गिनती == sb->s_blocks_per_group &&
		    i != fs->group_desc_गिनती - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		अगर (old_free_inodes_गिनती == sb->s_inodes_per_group) {
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
		अगर (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		अगर (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		अगर (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	अगर (dirty)
		ext2fs_mark_super_dirty(fs);
	वापस 0;
}

#अगर_घोषित DEBUG
#समावेश "e2p/e2p.h"

व्योम print_csum(स्थिर अक्षर *संदेश, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	काष्ठा ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	माप_प्रकार आकार = EXT2_DESC_आकार(fs->super);
	काष्ठा ext2_super_block *sb = fs->super;
	पूर्णांक offset = दुरत्व(काष्ठा ext2_group_desc, bg_checksum);
#अगर_घोषित शब्दगण_BIGENDIAN
	काष्ठा ext4_group_desc swabdesc;
	काष्ठा ext2_group_desc *save_desc = desc;
	स्थिर माप_प्रकार ext4_bg_आकार = माप(काष्ठा ext4_group_desc);
	माप_प्रकार save_आकार = आकार;
#पूर्ण_अगर

#अगर_घोषित शब्दगण_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	अगर (आकार > ext4_bg_आकार)
		आकार = ext4_bg_आकार;
	स_नकल(&swabdesc, desc, आकार);
	ext2fs_swap_group_desc2(fs, (काष्ठा ext2_group_desc *) &swabdesc);
	desc = (काष्ठा ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#अन्यथा
	swabgroup = group;
#पूर्ण_अगर

	crc1 = ext2fs_crc16(~0, sb->s_uuid, माप(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, माप(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += माप(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	अगर (offset < आकार)
		crc3 = ext2fs_crc16(crc3, (अक्षर *)desc + offset, आकार - offset);
#अगर_घोषित शब्दगण_BIGENDIAN
	अगर (save_आकार > ext4_bg_आकार)
		crc3 = ext2fs_crc16(crc3, (अक्षर *)save_desc + ext4_bg_आकार,
				    save_आकार - ext4_bg_आकार);
#पूर्ण_अगर

	म_लिखो("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       संदेश, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

अचिन्हित अक्षर sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	काष्ठा ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	पूर्णांक			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	स_रखो(&param, 0, माप(param));
	ext2fs_blocks_गिनती_set(&param, 32768);
#अगर 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_आकार = 128;
	csum_known = 0x5b6e;
#पूर्ण_अगर

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	अगर (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		निकास(1);
	}
	स_नकल(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	क्रम (i=0; i < fs->group_desc_गिनती; i++) {
		ext2fs_block_bitmap_loc_set(fs, i, 124);
		ext2fs_inode_bitmap_loc_set(fs, i, 125);
		ext2fs_inode_table_loc_set(fs, i, 126);
		ext2fs_bg_free_blocks_गिनती_set(fs, i, 31119);
		ext2fs_bg_free_inodes_गिनती_set(fs, i, 15701);
		ext2fs_bg_used_dirs_गिनती_set(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	अगर (csum1 != csum_known) {
		म_लिखो("checksum for group 0 should be %04x\n", csum_known);
		निकास(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	अगर (csum1 == csum2) {
		म_लिखो("checksums for different groups shouldn't match\n");
		निकास(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	अगर (csum1 == csum2) {
		म_लिखो("checksums for different groups shouldn't match\n");
		निकास(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	अगर (csum1 != csum2) {
		म_लिखो("checksums should not depend on checksum field\n");
		निकास(1);
	}
	अगर (!ext2fs_group_desc_csum_verify(fs, 0)) {
		म_लिखो("checksums should verify against gd_checksum\n");
		निकास(1);
	}
	स_रखो(fs->super->s_uuid, 0x30, माप(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	अगर (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		म_लिखो("checksums for different filesystems shouldn't match\n");
		निकास(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_गिनती_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	अगर (csum1 == csum2) {
		म_लिखो("checksums for different data shouldn't match\n");
		निकास(1);
	}
	ext2fs_free(fs);

	वापस 0;
}
#पूर्ण_अगर
