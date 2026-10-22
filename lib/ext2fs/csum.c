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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "crc16.h"
#ଅନ୍ତର୍ଭାଵ <assert.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#ଅନ୍ୟଥା
#define STATIC ସ୍ଥିର
#endif

ଶୂନ୍ୟ ext2fs_init_csum_seed(ext2_filsys fs)
{
	ଯଦି (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	ଅନ୍ୟଥା ଯଦି (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 ଆକାର(fs->super->s_uuid));
}

ସ୍ଥିର __u32 ext2fs_mmp_csum(ext2_filsys fs, ସଂରଚନା mmp_struct *mmp)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = offsetof(ସଂରଚନା mmp_struct, mmp_checksum);

	ଫେରନ୍ତୁ ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)mmp, offset);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_mmp_csum_verify(ext2_filsys fs, ସଂରଚନା mmp_struct *mmp)
{
	__u32 calculated;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	ଫେରନ୍ତୁ ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, ସଂରଚନା mmp_struct *mmp)
{
	__u32 crc;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_verify_csum_type(ext2_filsys fs, ସଂରଚନା ext2_super_block *sb)
{
	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 1;

	ଫେରନ୍ତୁ sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

ସ୍ଥିର __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    ସଂରଚନା ext2_super_block *sb)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = offsetof(ସଂରଚନା ext2_super_block, s_checksum);

	ଫେରନ୍ତୁ ext2fs_crc32c_le(~0, (unsigned ଅକ୍ଷର *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_superblock_csum_verify(ext2_filsys fs, ସଂରଚନା ext2_super_block *sb)
{
	__u32 flag, calculated;

	ଯଦି (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ଅନ୍ୟଥା
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ଯଦି (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ଫେରନ୍ତୁ 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	ଫେରନ୍ତୁ ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     ସଂରଚନା ext2_super_block *sb)
{
	__u32 flag, crc;

	ଯଦି (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ଅନ୍ୟଥା
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ଯଦି (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ଫେରନ୍ତୁ 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    ସଂରଚନା ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	ଅକ୍ଷର *buf = (ଅକ୍ଷର *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)&block,
				ଆକାର(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      ସଂରଚନା ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	ଯଦି (retval)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 ସଂରଚନା ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର __u16 do_nothing16(__u16 x)
{
	ଫେରନ୍ତୁ x;
}

ସ୍ଥିର __u16 disk_to_host16(__u16 x)
{
	ଫେରନ୍ତୁ ext2fs_le16_to_cpu(x);
}

ସ୍ଥିର errcode_t __get_dx_countlimit(ext2_filsys fs,
				     ସଂରଚନା ext2_dir_entry *dirent,
				     ସଂରଚନା ext2_dx_countlimit **cc,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା *offset,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା need_swab)
{
	ସଂରଚନା ext2_dir_entry *dp;
	ସଂରଚନା ext2_dx_ମୂଳ_info *ମୂଳ;
	ସଂରଚନା ext2_dx_countlimit *c;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_offset, max_sane_entries;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	ଯଦି (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		ଗଣନା_offset = 8;
	ଅନ୍ୟଥା ଯଦି (rec_len == 12) {
		dp = (ସଂରଚନା ext2_dir_entry *)(((ଅକ୍ଷର *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		ଯଦି (rec_len != fs->blocksize - 12)
			ଫେରନ୍ତୁ EXT2_ET_DB_NOT_FOUND;
		ମୂଳ = (ସଂରଚନା ext2_dx_ମୂଳ_info *)(((ଅକ୍ଷର *)dp + 12));
		ଯଦି (ମୂଳ->reserved_zero ||
		    ମୂଳ->info_length != ଆକାର(ସଂରଚନା ext2_dx_ମୂଳ_info))
			ଫେରନ୍ତୁ EXT2_ET_DB_NOT_FOUND;
		ଗଣନା_offset = 32;
	} ଅନ୍ୟଥା
		ଫେରନ୍ତୁ EXT2_ET_DB_NOT_FOUND;

	c = (ସଂରଚନା ext2_dx_countlimit *)(((ଅକ୍ଷର *)dirent) + ଗଣନା_offset);
	max_sane_entries = (fs->blocksize - ଗଣନା_offset) /
			   ଆକାର(ସଂରଚନା ext2_dx_entry);
	ଯଦି (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->ଗଣନା_2) > max_sane_entries)
		ଫେରନ୍ତୁ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ଯଦି (offset)
		*offset = ଗଣନା_offset;
	ଯଦି (cc)
		*cc = c;

	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   ସଂରଚନା ext2_dir_entry *dirent,
				   ସଂରଚନା ext2_dx_countlimit **cc,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା *offset)
{
	ଫେରନ୍ତୁ __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

ଶୂନ୍ୟ ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   ସଂରଚନା ext2_dir_entry_tail *t)
{
	memset(t, 0, ଆକାର(ସଂରଚନା ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, ଆକାର(ସଂରଚନା ext2_dir_entry_tail),
			   (ସଂରଚନା ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ସ୍ଥିର errcode_t __get_dirent_tail(ext2_filsys fs,
				   ସଂରଚନା ext2_dir_entry *dirent,
				   ସଂରଚନା ext2_dir_entry_tail **tt,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା need_swab)
{
	ସଂରଚନା ext2_dir_entry *d;
	ଶୂନ୍ୟ *ଶୀର୍ଷ;
	ସଂରଚନା ext2_dir_entry_tail *t;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	ଯଦି (fs->blocksize < 1024)
		ଫେରନ୍ତୁ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	ଶୀର୍ଷ = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	ଯେତେବେଳେ ((ଶୂନ୍ୟ *) d < ଶୀର୍ଷ) {
		rec_len = translate(d->rec_len);
		ଯଦି ((rec_len < 8) || (rec_len & 0x03))
			ଫେରନ୍ତୁ EXT2_ET_DIR_CORRUPTED;
		d = (ସଂରଚନା ext2_dir_entry *)(((ଅକ୍ଷର *)d) + rec_len);
	}

	ଯଦି ((ଅକ୍ଷର *)d > ((ଅକ୍ଷର *)dirent + fs->blocksize))
			ଫେରନ୍ତୁ EXT2_ET_DIR_CORRUPTED;
	ଯଦି (d != ଶୀର୍ଷ)
		ଫେରନ୍ତୁ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (ସଂରଚନା ext2_dir_entry_tail *)d;
	ଯଦି (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != ଆକାର(ସଂରଚନା ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		ଫେରନ୍ତୁ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ଯଦି (tt)
		*tt = t;
	ଫେରନ୍ତୁ retval;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_dirent_has_tail(ext2_filsys fs, ସଂରଚନା ext2_dir_entry *dirent)
{
	ଫେରନ୍ତୁ __get_dirent_tail(fs, dirent, ଖାଲି, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ସ୍ଥିର errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    ସଂରଚନା ext2_dir_entry *dirent, __u32 *crc,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	errcode_t retval;
	ଅକ୍ଷର *buf = (ଅକ୍ଷର *)dirent;
	__u32 gen;
	ସଂରଚନା ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)&inum,
				ଆକାର(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)&gen, ଆକାର(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)buf, size);

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      ସଂରଚନା ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	ସଂରଚନା ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ଯଦି (retval)
		ଫେରନ୍ତୁ 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (ଅକ୍ଷର *)t - (ଅକ୍ଷର *)dirent);
	ଯଦି (retval)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

ସ୍ଥିର errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					ସଂରଚନା ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	ସଂରଚନା ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (ଅକ୍ଷର *)t - (ଅକ୍ଷର *)dirent);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 ସଂରଚନା ext2_dir_entry *dirent,
			 __u32 *crc, ସଂରଚନା ext2_dx_tail **ret_t)
{
	errcode_t retval;
	ଅକ୍ଷର *buf = (ଅକ୍ଷର *)dirent;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା size;
	__u32 gen, dummy_csum = 0;
	ସଂରଚନା ext2_inode inode;
	ସଂରଚନା ext2_dx_tail *t;
	ସଂରଚନା ext2_dx_countlimit *c;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_offset, limit, ଗଣନା_2;

	retval = __get_dx_countlimit(fs, dirent, &c, &ଗଣନା_offset, 1);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	ଗଣନା_2 = ext2fs_le16_to_cpu(c->ଗଣନା_2);
	ଯଦି (ଗଣନା_offset + (limit * ଆକାର(ସଂରଚନା ext2_dx_entry)) >
	    fs->blocksize - ଆକାର(ସଂରଚନା ext2_dx_tail))
		ଫେରନ୍ତୁ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (ସଂରଚନା ext2_dx_tail *)(((ସଂରଚନା ext2_dx_entry *)c) + limit);

	size = ଗଣନା_offset + (ଗଣନା_2 * ଆକାର(ସଂରଚନା ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)&inum,
				ଆକାର(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)&gen, ଆକାର(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)&dummy_csum, 4);

	ଯଦି (ret_t)
		*ret_t = t;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ସଂରଚନା ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	ସଂରଚନା ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	ଯଦି (retval)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

ସ୍ଥିର errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    ସଂରଚନା ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	ସଂରଚନା ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	ଫେରନ୍ତୁ retval;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ସଂରଚନା ext2_dir_entry *dirent)
{
	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 1;

	ଯଦି (__get_dirent_tail(fs, dirent, ଖାଲି, 1) == 0)
		ଫେରନ୍ତୁ ext2fs_dirent_csum_verify(fs, inum, dirent);
	ଯଦି (__get_dx_countlimit(fs, dirent, ଖାଲି, ଖାଲି, 1) == 0)
		ଫେରନ୍ତୁ ext2fs_dx_csum_verify(fs, inum, dirent);

	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    ସଂରଚନା ext2_dir_entry *dirent)
{
	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 0;

	ଯଦି (__get_dirent_tail(fs, dirent, ଖାଲି, 1) == 0)
		ଫେରନ୍ତୁ ext2fs_dirent_csum_set(fs, inum, dirent);
	ଯଦି (__get_dx_countlimit(fs, dirent, ଖାଲି, ଖାଲି, 1) == 0)
		ଫେରନ୍ତୁ ext2fs_dx_csum_set(fs, inum, dirent);

	ଯଦି (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(ଆକାର(ସଂରଚନା ext3_extent_header) + \
	(ଆକାର(ସଂରଚନା ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

ସ୍ଥିର ସଂରଚନା ext3_extent_tail *get_extent_tail(ସଂରଚନା ext3_extent_header *h)
{
	ଫେରନ୍ତୁ (ସଂରଚନା ext3_extent_tail *)(((ଅକ୍ଷର *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

ସ୍ଥିର errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  ସଂରଚନା ext3_extent_header *eh,
					  __u32 *crc)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା size;
	__u32 gen;
	errcode_t retval;
	ସଂରଚନା ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + offsetof(ସଂରଚନା ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)&inum,
				ଆକାର(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)&gen, ଆକାର(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)eh, size);

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ସଂରଚନା ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	ସଂରଚନା ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	ଯଦି (retval)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       ସଂରଚନା ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	ସଂରଚନା ext3_extent_tail *t = get_extent_tail(eh);

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	ଫେରନ୍ତୁ retval;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    ଅକ୍ଷର *bitmap, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	ସଂରଚନା ext4_group_desc *gdp = (ସଂରଚନା ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)bitmap,
				      size);
	ଯଦି (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	ଅନ୍ୟଥା
		calculated &= 0xFFFF;

	ଫେରନ୍ତୁ provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       ଅକ୍ଷର *bitmap, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	__u32 crc;
	ସଂରଚନା ext4_group_desc *gdp = (ସଂରଚନା ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	ଯଦି (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    ଅକ୍ଷର *bitmap, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	ସଂରଚନା ext4_group_desc *gdp = (ସଂରଚନା ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)bitmap,
				      size);
	ଯଦି (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	ଅନ୍ୟଥା
		calculated &= 0xFFFF;

	ଫେରନ୍ତୁ provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       ଅକ୍ଷର *bitmap, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	__u32 crc;
	ସଂରଚନା ext4_group_desc *gdp = (ସଂରଚନା ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	ଯଦି (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       ସଂରଚନା ext2_inode_large *inode,
			       __u32 *crc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_hi)
{
	__u32 gen;
	ସଂରଚନା ext2_inode_large *desc = inode;
	size_t size = EXT2_INODE_SIZE(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	ଯଦି (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)&inum,
				ଆକାର(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)&gen, ଆକାର(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned ଅକ୍ଷର *)desc, size);

	inode->i_checksum_lo = old_lo;
	ଯଦି (has_hi)
		inode->i_checksum_hi = old_hi;
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     ସଂରଚନା ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, has_hi;
	ଅକ୍ଷର *cp;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	ଯଦି (retval)
		ଫେରନ୍ତୁ 0;
	ଯଦି (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} ଅନ୍ୟଥା
		calculated &= 0xFFFF;

	ଯଦି (provided == calculated)
		ଫେରନ୍ତୁ 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	ପାଇଁ (cp = (ଅକ୍ଷର *) inode, i = 0;
	     i < ଆକାର(ସଂରଚନା ext2_inode);
	     cp++, i++)
		ଯଦି (*cp)
			ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   ସଂରଚନା ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_hi;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	ଯଦି (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	ଫେରନ୍ତୁ 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	ସଂରଚନା ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ସଂରଚନା ext4_group_desc swabdesc;
	size_t save_size = size;
	ଅଚଳ size_t ext4_bg_size = ଆକାର(ସଂରଚନା ext4_group_desc);
	ସଂରଚନା ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	ଯଦି (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (ସଂରଚନା ext2_group_desc *) &swabdesc);
	desc = (ସଂରଚନା ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	ଯଦି (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)&group,
					 ଆକାର(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned ଅକ୍ଷର *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
		ଯଦି (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned ଅକ୍ଷର *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		ଯାଅ out;
	}

	/* old crc16 code */
	offset = offsetof(ସଂରଚନା ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   ଆକାର(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, ଆକାର(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += ଆକାର(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ଯଦି (offset < size) {
		crc = ext2fs_crc16(crc, (ଅକ୍ଷର *)desc + offset,
				   size - offset);
	}
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	ଯଦି (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (ଅକ୍ଷର *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	ଫେରନ୍ତୁ crc;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	ଯଦି (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ 1;
}

ଶୂନ୍ୟ ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	ଯଦି (!ext2fs_has_group_desc_csum(fs))
		ଫେରନ୍ତୁ;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

ସ୍ଥିର __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	ପାଇଁ (i = end_ino; i >= start_ino; i--) {
		ଯଦି (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			ଫେରନ୍ତୁ i - start_ino + 1;
	}
	ଫେରନ୍ତୁ inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	ସଂରଚନା ext2_super_block *sb = fs->super;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirty = 0;
	dgrp_t i;

	ଯଦି (!fs->inode_map)
		ଫେରନ୍ତୁ EXT2_ET_NO_INODE_BITMAP;

	ଯଦି (!ext2fs_has_group_desc_csum(fs))
		ଫେରନ୍ତୁ 0;

	ପାଇଁ (i = 0; i < fs->group_desc_ଗଣନା; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_ଗଣନା = ext2fs_bg_free_inodes_ଗଣନା(fs, i);
		__u32 old_free_blocks_ଗଣନା = ext2fs_bg_free_blocks_ଗଣନା(fs, i);

		ଯଦି (old_free_blocks_ଗଣନା == sb->s_blocks_per_group &&
		    i != fs->group_desc_ଗଣନା - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		ଯଦି (old_free_inodes_ଗଣନା == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} ଅନ୍ୟଥା {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		ଯଦି (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		ଯଦି (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		ଯଦି (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	ଯଦି (dirty)
		ext2fs_mark_super_dirty(fs);
	ଫେରନ୍ତୁ 0;
}

#ifdef DEBUG
#ଅନ୍ତର୍ଭାଵ "e2p/e2p.h"

ଶୂନ୍ୟ print_csum(ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	ସଂରଚନା ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t size = EXT2_DESC_SIZE(fs->super);
	ସଂରଚନା ext2_super_block *sb = fs->super;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = offsetof(ସଂରଚନା ext2_group_desc, bg_checksum);
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ସଂରଚନା ext4_group_desc swabdesc;
	ସଂରଚନା ext2_group_desc *save_desc = desc;
	ଅଚଳ size_t ext4_bg_size = ଆକାର(ସଂରଚନା ext4_group_desc);
	size_t save_size = size;
#endif

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	ଯଦି (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (ସଂରଚନା ext2_group_desc *) &swabdesc);
	desc = (ସଂରଚନା ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#ଅନ୍ୟଥା
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, ଆକାର(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, ଆକାର(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += ଆକାର(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ଯଦି (offset < size)
		crc3 = ext2fs_crc16(crc3, (ଅକ୍ଷର *)desc + offset, size - offset);
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ଯଦି (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (ଅକ୍ଷର *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       ସନ୍ଦେଶ, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned ଅକ୍ଷର sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	ସଂରଚନା ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, ଆକାର(param));
	ext2fs_blocks_ଗଣନା_set(&param, 32768);
#ଯଦି 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	ଯଦି (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	memcpy(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	ପାଇଁ (i=0; i < fs->group_desc_ଗଣନା; i++) {
		ext2fs_block_bitmap_loc_set(fs, i, 124);
		ext2fs_inode_bitmap_loc_set(fs, i, 125);
		ext2fs_inode_table_loc_set(fs, i, 126);
		ext2fs_bg_free_blocks_ଗଣନା_set(fs, i, 31119);
		ext2fs_bg_free_inodes_ଗଣନା_set(fs, i, 15701);
		ext2fs_bg_used_dirs_ଗଣନା_set(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	ଯଦି (csum1 != csum_known) {
		printf("checksum for group 0 should be %04x\n", csum_known);
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	ଯଦି (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	ଯଦି (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	ଯଦି (csum1 != csum2) {
		printf("checksums should not depend on checksum field\n");
		exit(1);
	}
	ଯଦି (!ext2fs_group_desc_csum_verify(fs, 0)) {
		printf("checksums should verify against gd_checksum\n");
		exit(1);
	}
	memset(fs->super->s_uuid, 0x30, ଆକାର(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	ଯଦି (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		printf("checksums for different filesystems shouldn't match\n");
		exit(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_ଗଣନା_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	ଯଦି (csum1 == csum2) {
		printf("checksums for different data shouldn't match\n");
		exit(1);
	}
	ext2fs_free(fs);

	ଫେରନ୍ତୁ 0;
}
#endif
