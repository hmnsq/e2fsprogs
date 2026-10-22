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
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "crc16.h"
#ಅನ್ತರ್ಭಾವ <assert.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ offsetof
#ವ್ಯಾಖ್ಯಾನಿಸು offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
#ವ್ಯಾಖ್ಯಾನಿಸು STATIC
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು STATIC ಸ್ಥಿರ
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ext2fs_init_csum_ಬೀಜ(ext2_filsys fs)
{
	ಆದರೆ (ext2fs_has_feature_csum_ಬೀಜ(fs->super))
		fs->csum_ಬೀಜ = fs->super->s_checksum_ಬೀಜ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_ಬೀಜ = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 ಗಾತ್ರ(fs->super->s_uuid));
}

ಸ್ಥಿರ __u32 ext2fs_mmp_csum(ext2_filsys fs, ರಚನೆ mmp_struct *mmp)
{
	ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = offsetof(ರಚನೆ mmp_struct, mmp_checksum);

	ಹಿಂತಿರುಗಿ ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)mmp, ಸ್ಥಾನಾಂತರ);
}

ಪೂರ್ಣಾಂಕ ext2fs_mmp_csum_verify(ext2_filsys fs, ರಚನೆ mmp_struct *mmp)
{
	__u32 calculated;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	ಹಿಂತಿರುಗಿ ext2fs_le32_ಗಮ್ಯ_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_ಗಣ(ext2_filsys fs, ರಚನೆ mmp_struct *mmp)
{
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 0;

	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_ಗಮ್ಯ_le32(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ext2fs_verify_csum_type(ext2_filsys fs, ರಚನೆ ext2_super_ಖಂಡ *sb)
{
	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ sb->s_checksum_type == EXT2_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_C_CHKSUM;
}

ಸ್ಥಿರ __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((ಬಳಸದ)),
				    ರಚನೆ ext2_super_ಖಂಡ *sb)
{
	ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = offsetof(ರಚನೆ ext2_super_ಖಂಡ, s_checksum);

	ಹಿಂತಿರುಗಿ ext2fs_crc32c_le(~0, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)sb, ಸ್ಥಾನಾಂತರ);
}

/* NOTE: The input to this function MUST be in LE order */
ಪೂರ್ಣಾಂಕ ext2fs_superblock_csum_verify(ext2_filsys fs, ರಚನೆ ext2_super_ಖಂಡ *sb)
{
	__u32 flag, calculated;

	ಆದರೆ (fs->flags & EXT2_FLAG_ಅದಲುಬದಲು_ಬೈಟುಗಳು)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ಇಲ್ಲದಿದ್ದರೆ
		flag = ext2fs_cpu_ಗಮ್ಯ_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ಆದರೆ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ಹಿಂತಿರುಗಿ 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	ಹಿಂತಿರುಗಿ ext2fs_le32_ಗಮ್ಯ_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_ಗಣ(ext2_filsys fs,
				     ರಚನೆ ext2_super_ಖಂಡ *sb)
{
	__u32 flag, ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

	ಆದರೆ (fs->flags & EXT2_FLAG_ಅದಲುಬದಲು_ಬೈಟುಗಳು)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ಇಲ್ಲದಿದ್ದರೆ
		flag = ext2fs_cpu_ಗಮ್ಯ_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ಆದರೆ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ಹಿಂತಿರುಗಿ 0;

	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_ಗಮ್ಯ_le32(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ext2fs_ext_attr_ಖಂಡ_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((ಬಳಸದ)),
					    blk64_t ಖಂಡ,
					    ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *hdr,
					    __u32 *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ)
{
	ಅಕ್ಷರ *buf = (ಅಕ್ಷರ *)hdr;
	__u32 old_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = hdr->h_checksum;

	hdr->h_checksum = 0;
	ಖಂಡ = ext2fs_cpu_ಗಮ್ಯ_le64(ಖಂಡ);
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&ಖಂಡ,
				ಗಾತ್ರ(ಖಂಡ));
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)buf, fs->blocksize);
	hdr->h_checksum = old_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ext2fs_ext_attr_ಖಂಡ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t ಖಂಡ,
				      ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *hdr)
{
	__u32 calculated;
	errcode_t retval;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 1;

	retval = ext2fs_ext_attr_ಖಂಡ_csum(fs, inum, ಖಂಡ, hdr, &calculated);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ ext2fs_le32_ಗಮ್ಯ_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_ಖಂಡ_csum_ಗಣ(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t ಖಂಡ,
					 ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *hdr)
{
	errcode_t retval;
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 0;

	retval = ext2fs_ext_attr_ಖಂಡ_csum(fs, inum, ಖಂಡ, hdr, &ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	hdr->h_checksum = ext2fs_cpu_ಗಮ್ಯ_le32(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ __u16 do_nothing16(__u16 x)
{
	ಹಿಂತಿರುಗಿ x;
}

ಸ್ಥಿರ __u16 disk_ಗಮ್ಯ_host16(__u16 x)
{
	ಹಿಂತಿರುಗಿ ext2fs_le16_ಗಮ್ಯ_cpu(x);
}

ಸ್ಥಿರ errcode_t __get_dx_countlimit(ext2_filsys fs,
				     ರಚನೆ ext2_dir_ನಮೂದು *dirent,
				     ರಚನೆ ext2_dx_countlimit **cc,
				     ಪೂರ್ಣಾಂಕ *ಸ್ಥಾನಾಂತರ,
				     ಪೂರ್ಣಾಂಕ need_swab)
{
	ರಚನೆ ext2_dir_ನಮೂದು *dp;
	ರಚನೆ ext2_dx_ಬೇರು_info *ಬೇರು;
	ರಚನೆ ext2_dx_countlimit *c;
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_ಸ್ಥಾನಾಂತರ, max_sane_entries;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_ಗಮ್ಯ_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	ಆದರೆ (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		ಎಣಿಕೆ_ಸ್ಥಾನಾಂತರ = 8;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rec_len == 12) {
		dp = (ರಚನೆ ext2_dir_ನಮೂದು *)(((ಅಕ್ಷರ *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		ಆದರೆ (rec_len != fs->blocksize - 12)
			ಹಿಂತಿರುಗಿ EXT2_ET_DB_NOT_ಸಿಕ್ಕಿತು;
		ಬೇರು = (ರಚನೆ ext2_dx_ಬೇರು_info *)(((ಅಕ್ಷರ *)dp + 12));
		ಆದರೆ (ಬೇರು->reserved_zero ||
		    ಬೇರು->info_ಉದ್ದಳತೆ != ಗಾತ್ರ(ರಚನೆ ext2_dx_ಬೇರು_info))
			ಹಿಂತಿರುಗಿ EXT2_ET_DB_NOT_ಸಿಕ್ಕಿತು;
		ಎಣಿಕೆ_ಸ್ಥಾನಾಂತರ = 32;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ EXT2_ET_DB_NOT_ಸಿಕ್ಕಿತು;

	c = (ರಚನೆ ext2_dx_countlimit *)(((ಅಕ್ಷರ *)dirent) + ಎಣಿಕೆ_ಸ್ಥಾನಾಂತರ);
	max_sane_entries = (fs->blocksize - ಎಣಿಕೆ_ಸ್ಥಾನಾಂತರ) /
			   ಗಾತ್ರ(ರಚನೆ ext2_dx_ನಮೂದು);
	ಆದರೆ (ext2fs_le16_ಗಮ್ಯ_cpu(c->ಎಲ್ಲೆ) > max_sane_entries ||
	    ext2fs_le16_ಗಮ್ಯ_cpu(c->ಎಣಿಕೆ_2) > max_sane_entries)
		ಹಿಂತಿರುಗಿ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ಆದರೆ (ಸ್ಥಾನಾಂತರ)
		*ಸ್ಥಾನಾಂತರ = ಎಣಿಕೆ_ಸ್ಥಾನಾಂತರ;
	ಆದರೆ (cc)
		*cc = c;

	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   ರಚನೆ ext2_dir_ನಮೂದು *dirent,
				   ರಚನೆ ext2_dx_countlimit **cc,
				   ಪೂರ್ಣಾಂಕ *ಸ್ಥಾನಾಂತರ)
{
	ಹಿಂತಿರುಗಿ __get_dx_countlimit(fs, dirent, cc, ಸ್ಥಾನಾಂತರ, 0);
}

ಶೂನ್ಯ ext2fs_initialize_dirent_ಬಾಲ(ext2_filsys fs,
				   ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ *t)
{
	memset(t, 0, ಗಾತ್ರ(ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ));
	ext2fs_ಗಣ_rec_len(fs, ಗಾತ್ರ(ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ),
			   (ರಚನೆ ext2_dir_ನಮೂದು *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ಸ್ಥಿರ errcode_t __get_dirent_tail(ext2_filsys fs,
				   ರಚನೆ ext2_dir_ನಮೂದು *dirent,
				   ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ **tt,
				   ಪೂರ್ಣಾಂಕ need_swab)
{
	ರಚನೆ ext2_dir_ನಮೂದು *d;
	ಶೂನ್ಯ *ತುದಿ;
	ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ *t;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_ಗಮ್ಯ_host16 : do_nothing16);

	ಆದರೆ (fs->blocksize < 1024)
		ಹಿಂತಿರುಗಿ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	ತುದಿ = EXT2_DIRENT_ಬಾಲ(dirent, fs->blocksize);

	ಆಗ ((ಶೂನ್ಯ *) d < ತುದಿ) {
		rec_len = translate(d->rec_len);
		ಆದರೆ ((rec_len < 8) || (rec_len & 0x03))
			ಹಿಂತಿರುಗಿ EXT2_ET_DIR_CORRUPTED;
		d = (ರಚನೆ ext2_dir_ನಮೂದು *)(((ಅಕ್ಷರ *)d) + rec_len);
	}

	ಆದರೆ ((ಅಕ್ಷರ *)d > ((ಅಕ್ಷರ *)dirent + fs->blocksize))
			ಹಿಂತಿರುಗಿ EXT2_ET_DIR_CORRUPTED;
	ಆದರೆ (d != ತುದಿ)
		ಹಿಂತಿರುಗಿ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ *)d;
	ಆದರೆ (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != ಗಾತ್ರ(ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		ಹಿಂತಿರುಗಿ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ಆದರೆ (tt)
		*tt = t;
	ಹಿಂತಿರುಗಿ retval;
}

ಪೂರ್ಣಾಂಕ ext2fs_dirent_has_ಬಾಲ(ext2_filsys fs, ರಚನೆ ext2_dir_ನಮೂದು *dirent)
{
	ಹಿಂತಿರುಗಿ __get_dirent_tail(fs, dirent, ಖಾಲಿ, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ಸ್ಥಿರ errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    ರಚನೆ ext2_dir_ನಮೂದು *dirent, __u32 *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ,
				    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	errcode_t retval;
	ಅಕ್ಷರ *buf = (ಅಕ್ಷರ *)dirent;
	__u32 gen;
	ರಚನೆ ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	inum = ext2fs_cpu_ಗಮ್ಯ_le32(inum);
	gen = ext2fs_cpu_ಗಮ್ಯ_le32(inode.i_generation);
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&inum,
				ಗಾತ್ರ(inum));
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&gen, ಗಾತ್ರ(gen));
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)buf, ಗಾತ್ರ_2);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      ರಚನೆ ext2_dir_ನಮೂದು *dirent)
{
	errcode_t retval;
	__u32 calculated;
	ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (ಅಕ್ಷರ *)t - (ಅಕ್ಷರ *)dirent);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ ext2fs_le32_ಗಮ್ಯ_cpu(t->det_checksum) == calculated;
}

ಸ್ಥಿರ errcode_t ext2fs_dirent_csum_ಗಣ(ext2_filsys fs, ext2_ino_t inum,
					ರಚನೆ ext2_dir_ನಮೂದು *dirent)
{
	errcode_t retval;
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
	ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ,
				    (ಅಕ್ಷರ *)t - (ಅಕ್ಷರ *)dirent);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	t->det_checksum = ext2fs_cpu_ಗಮ್ಯ_le32(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 ರಚನೆ ext2_dir_ನಮೂದು *dirent,
			 __u32 *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, ರಚನೆ ext2_dx_ಬಾಲ **ret_t)
{
	errcode_t retval;
	ಅಕ್ಷರ *buf = (ಅಕ್ಷರ *)dirent;
	ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2;
	__u32 gen, dummy_csum = 0;
	ರಚನೆ ext2_inode inode;
	ರಚನೆ ext2_dx_ಬಾಲ *t;
	ರಚನೆ ext2_dx_countlimit *c;
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_ಸ್ಥಾನಾಂತರ, ಎಲ್ಲೆ, ಎಣಿಕೆ_2;

	retval = __get_dx_countlimit(fs, dirent, &c, &ಎಣಿಕೆ_ಸ್ಥಾನಾಂತರ, 1);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಎಲ್ಲೆ = ext2fs_le16_ಗಮ್ಯ_cpu(c->ಎಲ್ಲೆ);
	ಎಣಿಕೆ_2 = ext2fs_le16_ಗಮ್ಯ_cpu(c->ಎಣಿಕೆ_2);
	ಆದರೆ (ಎಣಿಕೆ_ಸ್ಥಾನಾಂತರ + (ಎಲ್ಲೆ * ಗಾತ್ರ(ರಚನೆ ext2_dx_ನಮೂದು)) >
	    fs->blocksize - ಗಾತ್ರ(ರಚನೆ ext2_dx_ಬಾಲ))
		ಹಿಂತಿರುಗಿ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (ರಚನೆ ext2_dx_ಬಾಲ *)(((ರಚನೆ ext2_dx_ನಮೂದು *)c) + ಎಲ್ಲೆ);

	ಗಾತ್ರ_2 = ಎಣಿಕೆ_ಸ್ಥಾನಾಂತರ + (ಎಣಿಕೆ_2 * ಗಾತ್ರ(ರಚನೆ ext2_dx_ನಮೂದು));

	retval = ext2fs_read_inode(fs, inum, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	inum = ext2fs_cpu_ಗಮ್ಯ_le32(inum);
	gen = ext2fs_cpu_ಗಮ್ಯ_le32(inode.i_generation);
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&inum,
				ಗಾತ್ರ(inum));
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&gen, ಗಾತ್ರ(gen));
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)buf, ಗಾತ್ರ_2);
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)t, 4);
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&dummy_csum, 4);

	ಆದರೆ (ret_t)
		*ret_t = t;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ರಚನೆ ext2_dir_ನಮೂದು *dirent)
{
	__u32 calculated;
	errcode_t retval;
	ರಚನೆ ext2_dx_ಬಾಲ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ ext2fs_le32_ಗಮ್ಯ_cpu(t->dt_checksum) == calculated;
}

ಸ್ಥಿರ errcode_t ext2fs_dx_csum_ಗಣ(ext2_filsys fs, ext2_ino_t inum,
				    ರಚನೆ ext2_dir_ನಮೂದು *dirent)
{
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
	errcode_t retval = 0;
	ರಚನೆ ext2_dx_ಬಾಲ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, &t);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	t->dt_checksum = ext2fs_cpu_ಗಮ್ಯ_le32(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	ಹಿಂತಿರುಗಿ retval;
}

ಪೂರ್ಣಾಂಕ ext2fs_dir_ಖಂಡ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ರಚನೆ ext2_dir_ನಮೂದು *dirent)
{
	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (__get_dirent_tail(fs, dirent, ಖಾಲಿ, 1) == 0)
		ಹಿಂತಿರುಗಿ ext2fs_dirent_csum_verify(fs, inum, dirent);
	ಆದರೆ (__get_dx_countlimit(fs, dirent, ಖಾಲಿ, ಖಾಲಿ, 1) == 0)
		ಹಿಂತಿರುಗಿ ext2fs_dx_csum_verify(fs, inum, dirent);

	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_dir_ಖಂಡ_csum_ಗಣ(ext2_filsys fs, ext2_ino_t inum,
				    ರಚನೆ ext2_dir_ನಮೂದು *dirent)
{
	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (__get_dirent_tail(fs, dirent, ಖಾಲಿ, 1) == 0)
		ಹಿಂತಿರುಗಿ ext2fs_dirent_csum_ಗಣ(fs, inum, dirent);
	ಆದರೆ (__get_dx_countlimit(fs, dirent, ಖಾಲಿ, ಖಾಲಿ, 1) == 0)
		ಹಿಂತಿರುಗಿ ext2fs_dx_csum_ಗಣ(fs, inum, dirent);

	ಆದರೆ (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_EXTENT_ಬಾಲ_ಸ್ಥಾನಾಂತರ(hdr)	(ಗಾತ್ರ(ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ) + \
	(ಗಾತ್ರ(ರಚನೆ ext3_extent) * ext2fs_le16_ಗಮ್ಯ_cpu((hdr)->eh_max)))

ಸ್ಥಿರ ರಚನೆ ext3_extent_ಬಾಲ *get_extent_ಬಾಲ(ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *h)
{
	ಹಿಂತಿರುಗಿ (ರಚನೆ ext3_extent_ಬಾಲ *)(((ಅಕ್ಷರ *)h) +
					   EXT3_EXTENT_ಬಾಲ_ಸ್ಥಾನಾಂತರ(h));
}

ಸ್ಥಿರ errcode_t ext2fs_extent_ಖಂಡ_csum(ext2_filsys fs, ext2_ino_t inum,
					  ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *eh,
					  __u32 *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ)
{
	ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2;
	__u32 gen;
	errcode_t retval;
	ರಚನೆ ext2_inode inode;

	ಗಾತ್ರ_2 = EXT3_EXTENT_ಬಾಲ_ಸ್ಥಾನಾಂತರ(eh) + offsetof(ರಚನೆ ext3_extent_ಬಾಲ,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	inum = ext2fs_cpu_ಗಮ್ಯ_le32(inum);
	gen = ext2fs_cpu_ಗಮ್ಯ_le32(inode.i_generation);
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&inum,
				ಗಾತ್ರ(inum));
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&gen, ಗಾತ್ರ(gen));
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)eh, ಗಾತ್ರ_2);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ext2fs_extent_ಖಂಡ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	ರಚನೆ ext3_extent_ಬಾಲ *t = get_extent_ಬಾಲ(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 1;

	provided = ext2fs_le32_ಗಮ್ಯ_cpu(t->et_checksum);
	retval = ext2fs_extent_ಖಂಡ_csum(fs, inum, eh, &calculated);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ provided == calculated;
}

errcode_t ext2fs_extent_ಖಂಡ_csum_ಗಣ(ext2_filsys fs, ext2_ino_t inum,
				       ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *eh)
{
	errcode_t retval;
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
	ರಚನೆ ext3_extent_ಬಾಲ *t = get_extent_ಬಾಲ(eh);

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_ಖಂಡ_csum(fs, inum, eh, &ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	t->et_checksum = ext2fs_cpu_ಗಮ್ಯ_le32(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	ಹಿಂತಿರುಗಿ retval;
}

ಪೂರ್ಣಾಂಕ ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t ಗುಂಪು,
				    ಅಕ್ಷರ *bitmap, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp = (ರಚನೆ ext4_ಗುಂಪು_desc *)
			ext2fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	__u32 provided, calculated;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)bitmap,
				      ಗಾತ್ರ_2);
	ಆದರೆ (EXT2_DESC_ಗಾತ್ರ(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_ಕೊನೆ)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	ಇಲ್ಲದಿದ್ದರೆ
		calculated &= 0xFFFF;

	ಹಿಂತಿರುಗಿ provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
				       ಅಕ್ಷರ *bitmap, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp = (ರಚನೆ ext4_ಗುಂಪು_desc *)
			ext2fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 0;

	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)bitmap, ಗಾತ್ರ_2);
	gdp->bg_inode_bitmap_csum_lo = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ & 0xFFFF;
	ಆದರೆ (EXT2_DESC_ಗಾತ್ರ(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_ಕೊನೆ)
		gdp->bg_inode_bitmap_csum_hi = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 16;

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ext2fs_ಖಂಡ_bitmap_csum_verify(ext2_filsys fs, dgrp_t ಗುಂಪು,
				    ಅಕ್ಷರ *bitmap, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp = (ರಚನೆ ext4_ಗುಂಪು_desc *)
			ext2fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);
	__u32 provided, calculated;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 1;
	provided = gdp->bg_ಖಂಡ_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)bitmap,
				      ಗಾತ್ರ_2);
	ಆದರೆ (EXT2_DESC_ಗಾತ್ರ(fs->super) >= EXT4_BG_ಖಂಡ_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_ಖಂಡ_bitmap_csum_hi << 16;
	ಇಲ್ಲದಿದ್ದರೆ
		calculated &= 0xFFFF;

	ಹಿಂತಿರುಗಿ provided == calculated;
}

errcode_t ext2fs_ಖಂಡ_bitmap_csum_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು,
				       ಅಕ್ಷರ *bitmap, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp = (ರಚನೆ ext4_ಗುಂಪು_desc *)
			ext2fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, ಗುಂಪು);

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 0;

	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)bitmap, ಗಾತ್ರ_2);
	gdp->bg_ಖಂಡ_bitmap_csum_lo = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ & 0xFFFF;
	ಆದರೆ (EXT2_DESC_ಗಾತ್ರ(fs->super) >= EXT4_BG_ಖಂಡ_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_ಖಂಡ_bitmap_csum_hi = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 16;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       ರಚನೆ ext2_inode_large *inode,
			       __u32 *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, ಪೂರ್ಣಾಂಕ has_hi)
{
	__u32 gen;
	ರಚನೆ ext2_inode_large *desc = inode;
	size_t ಗಾತ್ರ_2 = EXT2_INODE_ಗಾತ್ರ(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	ಆದರೆ (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_ಗಮ್ಯ_le32(inum);
	gen = inode->i_generation;
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&inum,
				ಗಾತ್ರ(inum));
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&gen, ಗಾತ್ರ(gen));
	*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)desc, ಗಾತ್ರ_2);

	inode->i_checksum_lo = old_lo;
	ಆದರೆ (has_hi)
		inode->i_checksum_hi = old_hi;
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     ರಚನೆ ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, has_hi;
	ಅಕ್ಷರ *cp;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 1;

	has_hi = (EXT2_INODE_ಗಾತ್ರ(fs->super) > EXT2_GOOD_OLD_INODE_ಗಾತ್ರ &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_ಕೊನೆ);

	provided = ext2fs_le16_ಗಮ್ಯ_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (has_hi) {
		__u32 hi = ext2fs_le16_ಗಮ್ಯ_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} ಇಲ್ಲದಿದ್ದರೆ
		calculated &= 0xFFFF;

	ಆದರೆ (provided == calculated)
		ಹಿಂತಿರುಗಿ 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	ಗಾಗಿ (cp = (ಅಕ್ಷರ *) inode, i = 0;
	     i < ಗಾತ್ರ(ರಚನೆ ext2_inode);
	     cp++, i++)
		ಆದರೆ (*cp)
			ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_ಗಣ(ext2_filsys fs, ext2_ino_t inum,
			   ರಚನೆ ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
	ಪೂರ್ಣಾಂಕ has_hi;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 0;

	has_hi = (EXT2_INODE_ಗಾತ್ರ(fs->super) > EXT2_GOOD_OLD_INODE_ಗಾತ್ರ &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_ಕೊನೆ);

	retval = ext2fs_inode_csum(fs, inum, inode, &ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, has_hi);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	inode->i_checksum_lo = ext2fs_cpu_ಗಮ್ಯ_le16(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ & 0xFFFF);
	ಆದರೆ (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_ಗಮ್ಯ_le16(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 16);
	ಹಿಂತಿರುಗಿ 0;
}

__u16 ext2fs_ಗುಂಪು_desc_csum(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ರಚನೆ ext2_ಗುಂಪು_desc *desc = ext2fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc,
							 ಗುಂಪು);
	size_t ಸ್ಥಾನಾಂತರ, ಗಾತ್ರ_2 = EXT2_DESC_ಗಾತ್ರ(fs->super);
	__u16 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ರಚನೆ ext4_ಗುಂಪು_desc swabdesc;
	size_t save_ಗಾತ್ರ = ಗಾತ್ರ_2;
	ಸ್ಥಿರಾಂಕ size_t ext4_bg_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext4_ಗುಂಪು_desc);
	ರಚನೆ ext2_ಗುಂಪು_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	ಆದರೆ (ಗಾತ್ರ_2 > ext4_bg_ಗಾತ್ರ)
		ಗಾತ್ರ_2 = ext4_bg_ಗಾತ್ರ;
	ಸ್ಮರಣೆ_ನಕಲು(&swabdesc, desc, ಗಾತ್ರ_2);
	ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(fs, (ರಚನೆ ext2_ಗುಂಪು_desc *) &swabdesc);
	desc = (ರಚನೆ ext2_ಗುಂಪು_desc *) &swabdesc;
	ಗುಂಪು = ext2fs_swab32(ಗುಂಪು);
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
		__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32;

		old_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = desc->bg_checksum;
		desc->bg_checksum = 0;
		ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&ಗುಂಪು,
					 ಗಾತ್ರ(ಗುಂಪು));
		ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 = ext2fs_crc32c_le(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)desc,
					 ಗಾತ್ರ_2);
		desc->bg_checksum = old_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
		ಆದರೆ (save_ಗಾತ್ರ > ext4_bg_ಗಾತ್ರ)
			ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 = ext2fs_crc32c_le(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32,
				     (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)save_desc + ext4_bg_ಗಾತ್ರ,
	                             save_ಗಾತ್ರ - ext4_bg_ಗಾತ್ರ);
#ಆದರೆ_ಅಂತ್ಯ
		ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 & 0xFFFF;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* old crc16 code */
	ಸ್ಥಾನಾಂತರ = offsetof(ರಚನೆ ext2_ಗುಂಪು_desc, bg_checksum);
	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc16(~0, fs->super->s_uuid,
			   ಗಾತ್ರ(fs->super->s_uuid));
	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc16(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, &ಗುಂಪು, ಗಾತ್ರ(ಗುಂಪು));
	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc16(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, desc, ಸ್ಥಾನಾಂತರ);
	ಸ್ಥಾನಾಂತರ += ಗಾತ್ರ(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ಆದರೆ (ಸ್ಥಾನಾಂತರ < ಗಾತ್ರ_2) {
		ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc16(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಅಕ್ಷರ *)desc + ಸ್ಥಾನಾಂತರ,
				   ಗಾತ್ರ_2 - ಸ್ಥಾನಾಂತರ);
	}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	ಆದರೆ (save_ಗಾತ್ರ > ext4_bg_ಗಾತ್ರ)
		ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc16(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಅಕ್ಷರ *)save_desc + ext4_bg_ಗಾತ್ರ,
				   save_ಗಾತ್ರ - ext4_bg_ಗಾತ್ರ);
#ಆದರೆ_ಅಂತ್ಯ

ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
}

ಪೂರ್ಣಾಂಕ ext2fs_ಗುಂಪು_desc_csum_verify(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ಆದರೆ (ext2fs_has_ಗುಂಪು_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, ಗುಂಪು) !=
	     ext2fs_ಗುಂಪು_desc_csum(fs, ಗುಂಪು)))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ 1;
}

ಶೂನ್ಯ ext2fs_ಗುಂಪು_desc_csum_ಗಣ(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ಆದರೆ (!ext2fs_has_ಗುಂಪು_desc_csum(fs))
		ಹಿಂತಿರುಗಿ;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_ಗಣ(fs, ಗುಂಪು, ext2fs_ಗುಂಪು_desc_csum(fs, ಗುಂಪು));
}

ಸ್ಥಿರ __u32 ಕಂಡುಹಿಡಿ_ಕೊನೆಯ_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_ಪ್ರತಿ_grp, dgrp_t grp_no)
{
	ext2_ino_t i, ಆರಂಭ_ino, ಕೊನೆ_ino;

	ಆರಂಭ_ino = grp_no * inodes_ಪ್ರತಿ_grp + 1;
	ಕೊನೆ_ino = ಆರಂಭ_ino + inodes_ಪ್ರತಿ_grp - 1;

	ಗಾಗಿ (i = ಕೊನೆ_ino; i >= ಆರಂಭ_ino; i--) {
		ಆದರೆ (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			ಹಿಂತಿರುಗಿ i - ಆರಂಭ_ino + 1;
	}
	ಹಿಂತಿರುಗಿ inodes_ಪ್ರತಿ_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_ಗಣ_gdt_csum(ext2_filsys fs)
{
	ರಚನೆ ext2_super_ಖಂಡ *sb = fs->super;
	ಪೂರ್ಣಾಂಕ dirty = 0;
	dgrp_t i;

	ಆದರೆ (!fs->inode_map)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_INODE_BITMAP;

	ಆದರೆ (!ext2fs_has_ಗುಂಪು_desc_csum(fs))
		ಹಿಂತಿರುಗಿ 0;

	ಗಾಗಿ (i = 0; i < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_ಬಳಸದ = ext2fs_bg_itable_ಬಳಸದ(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_ಎಣಿಕೆ = ext2fs_bg_free_inodes_ಎಣಿಕೆ(fs, i);
		__u32 old_free_ಖಂಡಗಳು_ಎಣಿಕೆ = ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, i);

		ಆದರೆ (old_free_ಖಂಡಗಳು_ಎಣಿಕೆ == sb->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು &&
		    i != fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1)
			ext2fs_bg_flags_ಗಣ(fs, i, EXT2_BG_ಖಂಡ_UNINIT);

		ಆದರೆ (old_free_inodes_ಎಣಿಕೆ == sb->s_inodes_ಪ್ರತಿ_ಗುಂಪು) {
			ext2fs_bg_flags_ಗಣ(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_ಬಳಸದ_ಗಣ(fs, i, sb->s_inodes_ಪ್ರತಿ_ಗುಂಪು);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಪೂರ್ಣಾಂಕ ಬಳಸದ =
				sb->s_inodes_ಪ್ರತಿ_ಗುಂಪು -
				ಕಂಡುಹಿಡಿ_ಕೊನೆಯ_inode_ingrp(fs->inode_map,
						      sb->s_inodes_ಪ್ರತಿ_ಗುಂಪು, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_ಬಳಸದ_ಗಣ(fs, i, ಬಳಸದ);
		}

		ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, i);
		ಆದರೆ (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		ಆದರೆ (old_ಬಳಸದ != ext2fs_bg_itable_ಬಳಸದ(fs, i))
			dirty = 1;
		ಆದರೆ (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	ಆದರೆ (dirty)
		ext2fs_mark_super_dirty(fs);
	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
#ಅನ್ತರ್ಭಾವ "e2p/e2p.h"

ಶೂನ್ಯ print_csum(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	ರಚನೆ ext2_ಗುಂಪು_desc *desc = ext2fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc,
							 ಗುಂಪು);
	size_t ಗಾತ್ರ_2 = EXT2_DESC_ಗಾತ್ರ(fs->super);
	ರಚನೆ ext2_super_ಖಂಡ *sb = fs->super;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = offsetof(ರಚನೆ ext2_ಗುಂಪು_desc, bg_checksum);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ರಚನೆ ext4_ಗುಂಪು_desc swabdesc;
	ರಚನೆ ext2_ಗುಂಪು_desc *save_desc = desc;
	ಸ್ಥಿರಾಂಕ size_t ext4_bg_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext4_ಗುಂಪು_desc);
	size_t save_ಗಾತ್ರ = ಗಾತ್ರ_2;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	ಆದರೆ (ಗಾತ್ರ_2 > ext4_bg_ಗಾತ್ರ)
		ಗಾತ್ರ_2 = ext4_bg_ಗಾತ್ರ;
	ಸ್ಮರಣೆ_ನಕಲು(&swabdesc, desc, ಗಾತ್ರ_2);
	ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(fs, (ರಚನೆ ext2_ಗುಂಪು_desc *) &swabdesc);
	desc = (ರಚನೆ ext2_ಗುಂಪು_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(ಗುಂಪು);
#ಇಲ್ಲದಿದ್ದರೆ
	swabgroup = ಗುಂಪು;
#ಆದರೆ_ಅಂತ್ಯ

	crc1 = ext2fs_crc16(~0, sb->s_uuid, ಗಾತ್ರ(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, ಗಾತ್ರ(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, ಸ್ಥಾನಾಂತರ);
	ಸ್ಥಾನಾಂತರ += ಗಾತ್ರ(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ಆದರೆ (ಸ್ಥಾನಾಂತರ < ಗಾತ್ರ_2)
		crc3 = ext2fs_crc16(crc3, (ಅಕ್ಷರ *)desc + ಸ್ಥಾನಾಂತರ, ಗಾತ್ರ_2 - ಸ್ಥಾನಾಂತರ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ಆದರೆ (save_ಗಾತ್ರ > ext4_bg_ಗಾತ್ರ)
		crc3 = ext2fs_crc16(crc3, (ಅಕ್ಷರ *)save_desc + ext4_bg_ಗಾತ್ರ,
				    save_ಗಾತ್ರ - ext4_bg_ಗಾತ್ರ);
#ಆದರೆ_ಅಂತ್ಯ

	ಬರೆ("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       ಸಂದೇಶ, e2p_uuid2str(sb->s_uuid), crc1, ಗುಂಪು, crc2, crc3,
	       ext2fs_ಗುಂಪು_desc_csum(fs, ಗುಂಪು));
}

ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ರಚನೆ ext2_super_ಖಂಡ param;
	errcode_t		retval;
	ext2_filsys		fs;
	ಪೂರ್ಣಾಂಕ			i;
	__u16 csum1, csum2, csum_ತಿಳಿದ = 0xd3a4;

	memset(&param, 0, ಗಾತ್ರ(param));
	ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(&param, 32768);
#ಆದರೆ 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64_ಬಿಟ್;
	param.s_desc_ಗಾತ್ರ = 128;
	csum_ತಿಳಿದ = 0x5b6e;
#ಆದರೆ_ಅಂತ್ಯ

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು, &param,
				   test_io_manager, &fs);
	ಆದರೆ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಸ್ಮರಣೆ_ನಕಲು(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	ಗಾಗಿ (i=0; i < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++) {
		ext2fs_ಖಂಡ_bitmap_loc_ಗಣ(fs, i, 124);
		ext2fs_inode_bitmap_loc_ಗಣ(fs, i, 125);
		ext2fs_inode_ಕೋಷ್ಟಕ_loc_ಗಣ(fs, i, 126);
		ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs, i, 31119);
		ext2fs_bg_free_inodes_ಎಣಿಕೆ_ಗಣ(fs, i, 15701);
		ext2fs_bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ_ಗಣ(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_ಗುಂಪು_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	ಆದರೆ (csum1 != csum_ತಿಳಿದ) {
		ಬರೆ("checksum for group 0 should be %04x\n", csum_ತಿಳಿದ);
		ನಿರ್ಗಮಿಸು(1);
	}
	csum2 = ext2fs_ಗುಂಪು_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	ಆದರೆ (csum1 == csum2) {
		ಬರೆ("checksums for different groups shouldn't match\n");
		ನಿರ್ಗಮಿಸು(1);
	}
	csum2 = ext2fs_ಗುಂಪು_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	ಆದರೆ (csum1 == csum2) {
		ಬರೆ("checksums for different groups shouldn't match\n");
		ನಿರ್ಗಮಿಸು(1);
	}
	ext2fs_bg_checksum_ಗಣ(fs, 0, csum1);
	csum2 = ext2fs_ಗುಂಪು_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	ಆದರೆ (csum1 != csum2) {
		ಬರೆ("checksums should not depend on checksum field\n");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (!ext2fs_ಗುಂಪು_desc_csum_verify(fs, 0)) {
		ಬರೆ("checksums should verify against gd_checksum\n");
		ನಿರ್ಗಮಿಸು(1);
	}
	memset(fs->super->s_uuid, 0x30, ಗಾತ್ರ(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	ಆದರೆ (ext2fs_ಗುಂಪು_desc_csum_verify(fs, 0) != 0) {
		ಬರೆ("checksums for different filesystems shouldn't match\n");
		ನಿರ್ಗಮಿಸು(1);
	}
	csum1 = ext2fs_ಗುಂಪು_desc_csum(fs, 0);
	ext2fs_bg_checksum_ಗಣ(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs, 0, 1);
	csum2 = ext2fs_ಗುಂಪು_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	ಆದರೆ (csum1 == csum2) {
		ಬರೆ("checksums for different data shouldn't match\n");
		ನಿರ್ಗಮಿಸು(1);
	}
	ext2fs_free(fs);

	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ
