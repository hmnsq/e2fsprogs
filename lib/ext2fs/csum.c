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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "crc16.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ offsetof
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STATIC
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STATIC ਸਥਿਰ
#ਜੇ_ਅੰਤ

ਖਾਲੀ ext2fs_init_csum_ਬੀਜ(ext2_filsys fs)
{
	ਜੇ (ext2fs_has_feature_csum_ਬੀਜ(fs->super))
		fs->csum_ਬੀਜ = fs->super->s_checksum_ਬੀਜ;
	ਨਹੀਂਤਾਂ ਜੇ (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_ਬੀਜ = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 ਆਕਾਰ(fs->super->s_uuid));
}

ਸਥਿਰ __u32 ext2fs_mmp_csum(ext2_filsys fs, ਬਣਤਰ mmp_struct *mmp)
{
	ਪੂਰਨ ਵਿੱਥ = offsetof(ਬਣਤਰ mmp_struct, mmp_checksum);

	ਵਾਪਸ ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)mmp, ਵਿੱਥ);
}

ਪੂਰਨ ext2fs_mmp_csum_verify(ext2_filsys fs, ਬਣਤਰ mmp_struct *mmp)
{
	__u32 calculated;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	ਵਾਪਸ ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_ਸੈੱਟ(ext2_filsys fs, ਬਣਤਰ mmp_struct *mmp)
{
	__u32 ਚੱਕਰੀ_ਜਾਂਚ;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 0;

	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਚੱਕਰੀ_ਜਾਂਚ);

	ਵਾਪਸ 0;
}

ਪੂਰਨ ext2fs_verify_csum_type(ext2_filsys fs, ਬਣਤਰ ext2_super_ਖੰਡ *sb)
{
	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 1;

	ਵਾਪਸ sb->s_checksum_type == EXT2_ਚੱਕਰੀ_ਜਾਂਚ_32_C_CHKSUM;
}

ਸਥਿਰ __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
				    ਬਣਤਰ ext2_super_ਖੰਡ *sb)
{
	ਪੂਰਨ ਵਿੱਥ = offsetof(ਬਣਤਰ ext2_super_ਖੰਡ, s_checksum);

	ਵਾਪਸ ext2fs_crc32c_le(~0, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)sb, ਵਿੱਥ);
}

/* NOTE: The input to this function MUST be in LE order */
ਪੂਰਨ ext2fs_superblock_csum_verify(ext2_filsys fs, ਬਣਤਰ ext2_super_ਖੰਡ *sb)
{
	__u32 flag, calculated;

	ਜੇ (fs->flags & EXT2_FLAG_ਅਦਲਾ_ਬਦਲੀ_ਬਾਈਟਾਂ)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ਨਹੀਂਤਾਂ
		flag = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ਜੇ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ਵਾਪਸ 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	ਵਾਪਸ ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_ਸੈੱਟ(ext2_filsys fs,
				     ਬਣਤਰ ext2_super_ਖੰਡ *sb)
{
	__u32 flag, ਚੱਕਰੀ_ਜਾਂਚ;

	ਜੇ (fs->flags & EXT2_FLAG_ਅਦਲਾ_ਬਦਲੀ_ਬਾਈਟਾਂ)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ਨਹੀਂਤਾਂ
		flag = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ਜੇ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		ਵਾਪਸ 0;

	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਚੱਕਰੀ_ਜਾਂਚ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ext2fs_ext_attr_ਖੰਡ_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
					    blk64_t ਖੰਡ,
					    ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *hdr,
					    __u32 *ਚੱਕਰੀ_ਜਾਂਚ)
{
	ਅੱਖਰ *buf = (ਅੱਖਰ *)hdr;
	__u32 old_ਚੱਕਰੀ_ਜਾਂਚ = hdr->h_checksum;

	hdr->h_checksum = 0;
	ਖੰਡ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le64(ਖੰਡ);
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&ਖੰਡ,
				ਆਕਾਰ(ਖੰਡ));
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)buf, fs->blocksize);
	hdr->h_checksum = old_ਚੱਕਰੀ_ਜਾਂਚ;

	ਵਾਪਸ 0;
}

ਪੂਰਨ ext2fs_ext_attr_ਖੰਡ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t ਖੰਡ,
				      ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *hdr)
{
	__u32 calculated;
	errcode_t retval;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 1;

	retval = ext2fs_ext_attr_ਖੰਡ_csum(fs, inum, ਖੰਡ, hdr, &calculated);
	ਜੇ (retval)
		ਵਾਪਸ 0;

	ਵਾਪਸ ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_ਖੰਡ_csum_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t ਖੰਡ,
					 ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *hdr)
{
	errcode_t retval;
	__u32 ਚੱਕਰੀ_ਜਾਂਚ;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 0;

	retval = ext2fs_ext_attr_ਖੰਡ_csum(fs, inum, ਖੰਡ, hdr, &ਚੱਕਰੀ_ਜਾਂਚ);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	hdr->h_checksum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਚੱਕਰੀ_ਜਾਂਚ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ __u16 do_nothing16(__u16 x)
{
	ਵਾਪਸ x;
}

ਸਥਿਰ __u16 disk_ਮੰਜ਼ਿਲ_host16(__u16 x)
{
	ਵਾਪਸ ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(x);
}

ਸਥਿਰ errcode_t __get_dx_countlimit(ext2_filsys fs,
				     ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
				     ਬਣਤਰ ext2_dx_countlimit **cc,
				     ਪੂਰਨ *ਵਿੱਥ,
				     ਪੂਰਨ need_swab)
{
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dp;
	ਬਣਤਰ ext2_dx_ਜੜ੍ਹ_info *ਜੜ੍ਹ;
	ਬਣਤਰ ext2_dx_countlimit *c;
	ਪੂਰਨ ਗਿਣਤੀ_ਵਿੱਥ, max_sane_entries;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_ਮੰਜ਼ਿਲ_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	ਜੇ (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		ਗਿਣਤੀ_ਵਿੱਥ = 8;
	ਨਹੀਂਤਾਂ ਜੇ (rec_len == 12) {
		dp = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)(((ਅੱਖਰ *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		ਜੇ (rec_len != fs->blocksize - 12)
			ਵਾਪਸ EXT2_ET_DB_NOT_ਮਿਲਿਆ;
		ਜੜ੍ਹ = (ਬਣਤਰ ext2_dx_ਜੜ੍ਹ_info *)(((ਅੱਖਰ *)dp + 12));
		ਜੇ (ਜੜ੍ਹ->reserved_zero ||
		    ਜੜ੍ਹ->info_ਲੰਮਾਈ != ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਜੜ੍ਹ_info))
			ਵਾਪਸ EXT2_ET_DB_NOT_ਮਿਲਿਆ;
		ਗਿਣਤੀ_ਵਿੱਥ = 32;
	} ਨਹੀਂਤਾਂ
		ਵਾਪਸ EXT2_ET_DB_NOT_ਮਿਲਿਆ;

	c = (ਬਣਤਰ ext2_dx_countlimit *)(((ਅੱਖਰ *)dirent) + ਗਿਣਤੀ_ਵਿੱਥ);
	max_sane_entries = (fs->blocksize - ਗਿਣਤੀ_ਵਿੱਥ) /
			   ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ);
	ਜੇ (ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(c->ਹੱਦਬੰਦੀ) > max_sane_entries ||
	    ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(c->ਗਿਣਤੀ_2) > max_sane_entries)
		ਵਾਪਸ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ਜੇ (ਵਿੱਥ)
		*ਵਿੱਥ = ਗਿਣਤੀ_ਵਿੱਥ;
	ਜੇ (cc)
		*cc = c;

	ਵਾਪਸ 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
				   ਬਣਤਰ ext2_dx_countlimit **cc,
				   ਪੂਰਨ *ਵਿੱਥ)
{
	ਵਾਪਸ __get_dx_countlimit(fs, dirent, cc, ਵਿੱਥ, 0);
}

ਖਾਲੀ ext2fs_initialize_dirent_ਪੂਛ(ext2_filsys fs,
				   ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ *t)
{
	memset(t, 0, ਆਕਾਰ(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ));
	ext2fs_ਸੈੱਟ_rec_len(fs, ਆਕਾਰ(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ),
			   (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ਸਥਿਰ errcode_t __get_dirent_tail(ext2_filsys fs,
				   ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
				   ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ **tt,
				   ਪੂਰਨ need_swab)
{
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *d;
	ਖਾਲੀ *ਸਿਖਰ;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ *t;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_ਮੰਜ਼ਿਲ_host16 : do_nothing16);

	ਜੇ (fs->blocksize < 1024)
		ਵਾਪਸ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	ਸਿਖਰ = EXT2_DIRENT_ਪੂਛ(dirent, fs->blocksize);

	ਜਦੋਂ ((ਖਾਲੀ *) d < ਸਿਖਰ) {
		rec_len = translate(d->rec_len);
		ਜੇ ((rec_len < 8) || (rec_len & 0x03))
			ਵਾਪਸ EXT2_ET_DIR_CORRUPTED;
		d = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)(((ਅੱਖਰ *)d) + rec_len);
	}

	ਜੇ ((ਅੱਖਰ *)d > ((ਅੱਖਰ *)dirent + fs->blocksize))
			ਵਾਪਸ EXT2_ET_DIR_CORRUPTED;
	ਜੇ (d != ਸਿਖਰ)
		ਵਾਪਸ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ *)d;
	ਜੇ (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != ਆਕਾਰ(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		ਵਾਪਸ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ਜੇ (tt)
		*tt = t;
	ਵਾਪਸ retval;
}

ਪੂਰਨ ext2fs_dirent_has_ਪੂਛ(ext2_filsys fs, ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent)
{
	ਵਾਪਸ __get_dirent_tail(fs, dirent, ਨਿਰਾ, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ਸਥਿਰ errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent, __u32 *ਚੱਕਰੀ_ਜਾਂਚ,
				    ਪੂਰਨ ਮਾਪ)
{
	errcode_t retval;
	ਅੱਖਰ *buf = (ਅੱਖਰ *)dirent;
	__u32 gen;
	ਬਣਤਰ ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	inum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(inum);
	gen = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(inode.i_generation);
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&inum,
				ਆਕਾਰ(inum));
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&gen, ਆਕਾਰ(gen));
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)buf, ਮਾਪ);

	ਵਾਪਸ 0;
}

ਪੂਰਨ ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent)
{
	errcode_t retval;
	__u32 calculated;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ਜੇ (retval)
		ਵਾਪਸ 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (ਅੱਖਰ *)t - (ਅੱਖਰ *)dirent);
	ਜੇ (retval)
		ਵਾਪਸ 0;
	ਵਾਪਸ ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(t->det_checksum) == calculated;
}

ਸਥਿਰ errcode_t ext2fs_dirent_csum_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t inum,
					ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent)
{
	errcode_t retval;
	__u32 ਚੱਕਰੀ_ਜਾਂਚ;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &ਚੱਕਰੀ_ਜਾਂਚ,
				    (ਅੱਖਰ *)t - (ਅੱਖਰ *)dirent);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	t->det_checksum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਚੱਕਰੀ_ਜਾਂਚ);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
			 __u32 *ਚੱਕਰੀ_ਜਾਂਚ, ਬਣਤਰ ext2_dx_ਪੂਛ **ret_t)
{
	errcode_t retval;
	ਅੱਖਰ *buf = (ਅੱਖਰ *)dirent;
	ਪੂਰਨ ਮਾਪ;
	__u32 gen, dummy_csum = 0;
	ਬਣਤਰ ext2_inode inode;
	ਬਣਤਰ ext2_dx_ਪੂਛ *t;
	ਬਣਤਰ ext2_dx_countlimit *c;
	ਪੂਰਨ ਗਿਣਤੀ_ਵਿੱਥ, ਹੱਦਬੰਦੀ, ਗਿਣਤੀ_2;

	retval = __get_dx_countlimit(fs, dirent, &c, &ਗਿਣਤੀ_ਵਿੱਥ, 1);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਹੱਦਬੰਦੀ = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(c->ਹੱਦਬੰਦੀ);
	ਗਿਣਤੀ_2 = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(c->ਗਿਣਤੀ_2);
	ਜੇ (ਗਿਣਤੀ_ਵਿੱਥ + (ਹੱਦਬੰਦੀ * ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ)) >
	    fs->blocksize - ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਪੂਛ))
		ਵਾਪਸ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (ਬਣਤਰ ext2_dx_ਪੂਛ *)(((ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ *)c) + ਹੱਦਬੰਦੀ);

	ਮਾਪ = ਗਿਣਤੀ_ਵਿੱਥ + (ਗਿਣਤੀ_2 * ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ));

	retval = ext2fs_read_inode(fs, inum, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	inum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(inum);
	gen = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(inode.i_generation);
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&inum,
				ਆਕਾਰ(inum));
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&gen, ਆਕਾਰ(gen));
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)buf, ਮਾਪ);
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)t, 4);
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&dummy_csum, 4);

	ਜੇ (ret_t)
		*ret_t = t;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent)
{
	__u32 calculated;
	errcode_t retval;
	ਬਣਤਰ ext2_dx_ਪੂਛ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	ਜੇ (retval)
		ਵਾਪਸ 0;

	ਵਾਪਸ ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(t->dt_checksum) == calculated;
}

ਸਥਿਰ errcode_t ext2fs_dx_csum_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t inum,
				    ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent)
{
	__u32 ਚੱਕਰੀ_ਜਾਂਚ;
	errcode_t retval = 0;
	ਬਣਤਰ ext2_dx_ਪੂਛ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &ਚੱਕਰੀ_ਜਾਂਚ, &t);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	t->dt_checksum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਚੱਕਰੀ_ਜਾਂਚ);
	ਵਾਪਸ retval;
}

ਪੂਰਨ ext2fs_dir_ਖੰਡ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent)
{
	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 1;

	ਜੇ (__get_dirent_tail(fs, dirent, ਨਿਰਾ, 1) == 0)
		ਵਾਪਸ ext2fs_dirent_csum_verify(fs, inum, dirent);
	ਜੇ (__get_dx_countlimit(fs, dirent, ਨਿਰਾ, ਨਿਰਾ, 1) == 0)
		ਵਾਪਸ ext2fs_dx_csum_verify(fs, inum, dirent);

	ਵਾਪਸ 0;
}

errcode_t ext2fs_dir_ਖੰਡ_csum_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t inum,
				    ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent)
{
	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 0;

	ਜੇ (__get_dirent_tail(fs, dirent, ਨਿਰਾ, 1) == 0)
		ਵਾਪਸ ext2fs_dirent_csum_ਸੈੱਟ(fs, inum, dirent);
	ਜੇ (__get_dx_countlimit(fs, dirent, ਨਿਰਾ, ਨਿਰਾ, 1) == 0)
		ਵਾਪਸ ext2fs_dx_csum_ਸੈੱਟ(fs, inum, dirent);

	ਜੇ (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		ਵਾਪਸ 0;
	ਵਾਪਸ EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_EXTENT_ਪੂਛ_ਵਿੱਥ(hdr)	(ਆਕਾਰ(ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ) + \
	(ਆਕਾਰ(ਬਣਤਰ ext3_extent) * ext2fs_le16_ਮੰਜ਼ਿਲ_cpu((hdr)->eh_max)))

ਸਥਿਰ ਬਣਤਰ ext3_extent_ਪੂਛ *get_extent_ਪੂਛ(ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *h)
{
	ਵਾਪਸ (ਬਣਤਰ ext3_extent_ਪੂਛ *)(((ਅੱਖਰ *)h) +
					   EXT3_EXTENT_ਪੂਛ_ਵਿੱਥ(h));
}

ਸਥਿਰ errcode_t ext2fs_extent_ਖੰਡ_csum(ext2_filsys fs, ext2_ino_t inum,
					  ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *eh,
					  __u32 *ਚੱਕਰੀ_ਜਾਂਚ)
{
	ਪੂਰਨ ਮਾਪ;
	__u32 gen;
	errcode_t retval;
	ਬਣਤਰ ext2_inode inode;

	ਮਾਪ = EXT3_EXTENT_ਪੂਛ_ਵਿੱਥ(eh) + offsetof(ਬਣਤਰ ext3_extent_ਪੂਛ,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	inum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(inum);
	gen = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(inode.i_generation);
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&inum,
				ਆਕਾਰ(inum));
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&gen, ਆਕਾਰ(gen));
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)eh, ਮਾਪ);

	ਵਾਪਸ 0;
}

ਪੂਰਨ ext2fs_extent_ਖੰਡ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	ਬਣਤਰ ext3_extent_ਪੂਛ *t = get_extent_ਪੂਛ(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 1;

	provided = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(t->et_checksum);
	retval = ext2fs_extent_ਖੰਡ_csum(fs, inum, eh, &calculated);
	ਜੇ (retval)
		ਵਾਪਸ 0;

	ਵਾਪਸ provided == calculated;
}

errcode_t ext2fs_extent_ਖੰਡ_csum_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t inum,
				       ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *eh)
{
	errcode_t retval;
	__u32 ਚੱਕਰੀ_ਜਾਂਚ;
	ਬਣਤਰ ext3_extent_ਪੂਛ *t = get_extent_ਪੂਛ(eh);

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_ਖੰਡ_csum(fs, inum, eh, &ਚੱਕਰੀ_ਜਾਂਚ);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	t->et_checksum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਚੱਕਰੀ_ਜਾਂਚ);
	ਵਾਪਸ retval;
}

ਪੂਰਨ ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t ਸਮੂਹ,
				    ਅੱਖਰ *bitmap, ਪੂਰਨ ਮਾਪ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp = (ਬਣਤਰ ext4_ਸਮੂਹ_desc *)
			ext2fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	__u32 provided, calculated;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)bitmap,
				      ਮਾਪ);
	ਜੇ (EXT2_DESC_ਮਾਪ(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_ਸਿਰਾ)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	ਨਹੀਂਤਾਂ
		calculated &= 0xFFFF;

	ਵਾਪਸ provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
				       ਅੱਖਰ *bitmap, ਪੂਰਨ ਮਾਪ)
{
	__u32 ਚੱਕਰੀ_ਜਾਂਚ;
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp = (ਬਣਤਰ ext4_ਸਮੂਹ_desc *)
			ext2fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 0;

	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)bitmap, ਮਾਪ);
	gdp->bg_inode_bitmap_csum_lo = ਚੱਕਰੀ_ਜਾਂਚ & 0xFFFF;
	ਜੇ (EXT2_DESC_ਮਾਪ(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_ਸਿਰਾ)
		gdp->bg_inode_bitmap_csum_hi = ਚੱਕਰੀ_ਜਾਂਚ >> 16;

	ਵਾਪਸ 0;
}

ਪੂਰਨ ext2fs_ਖੰਡ_bitmap_csum_verify(ext2_filsys fs, dgrp_t ਸਮੂਹ,
				    ਅੱਖਰ *bitmap, ਪੂਰਨ ਮਾਪ)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp = (ਬਣਤਰ ext4_ਸਮੂਹ_desc *)
			ext2fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);
	__u32 provided, calculated;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 1;
	provided = gdp->bg_ਖੰਡ_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)bitmap,
				      ਮਾਪ);
	ਜੇ (EXT2_DESC_ਮਾਪ(fs->super) >= EXT4_BG_ਖੰਡ_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_ਖੰਡ_bitmap_csum_hi << 16;
	ਨਹੀਂਤਾਂ
		calculated &= 0xFFFF;

	ਵਾਪਸ provided == calculated;
}

errcode_t ext2fs_ਖੰਡ_bitmap_csum_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
				       ਅੱਖਰ *bitmap, ਪੂਰਨ ਮਾਪ)
{
	__u32 ਚੱਕਰੀ_ਜਾਂਚ;
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp = (ਬਣਤਰ ext4_ਸਮੂਹ_desc *)
			ext2fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, ਸਮੂਹ);

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 0;

	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)bitmap, ਮਾਪ);
	gdp->bg_ਖੰਡ_bitmap_csum_lo = ਚੱਕਰੀ_ਜਾਂਚ & 0xFFFF;
	ਜੇ (EXT2_DESC_ਮਾਪ(fs->super) >= EXT4_BG_ਖੰਡ_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_ਖੰਡ_bitmap_csum_hi = ਚੱਕਰੀ_ਜਾਂਚ >> 16;

	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       ਬਣਤਰ ext2_inode_large *inode,
			       __u32 *ਚੱਕਰੀ_ਜਾਂਚ, ਪੂਰਨ has_hi)
{
	__u32 gen;
	ਬਣਤਰ ext2_inode_large *desc = inode;
	size_t ਮਾਪ = EXT2_INODE_ਮਾਪ(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	ਜੇ (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(inum);
	gen = inode->i_generation;
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&inum,
				ਆਕਾਰ(inum));
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&gen, ਆਕਾਰ(gen));
	*ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(*ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)desc, ਮਾਪ);

	inode->i_checksum_lo = old_lo;
	ਜੇ (has_hi)
		inode->i_checksum_hi = old_hi;
	ਵਾਪਸ 0;
}

ਪੂਰਨ ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     ਬਣਤਰ ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, has_hi;
	ਅੱਖਰ *cp;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 1;

	has_hi = (EXT2_INODE_ਮਾਪ(fs->super) > EXT2_GOOD_OLD_INODE_ਮਾਪ &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_ਸਿਰਾ);

	provided = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	ਜੇ (retval)
		ਵਾਪਸ 0;
	ਜੇ (has_hi) {
		__u32 hi = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} ਨਹੀਂਤਾਂ
		calculated &= 0xFFFF;

	ਜੇ (provided == calculated)
		ਵਾਪਸ 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	ਲਈ (cp = (ਅੱਖਰ *) inode, i = 0;
	     i < ਆਕਾਰ(ਬਣਤਰ ext2_inode);
	     cp++, i++)
		ਜੇ (*cp)
			ਵਾਪਸ 0;
	ਵਾਪਸ 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t inum,
			   ਬਣਤਰ ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 ਚੱਕਰੀ_ਜਾਂਚ;
	ਪੂਰਨ has_hi;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 0;

	has_hi = (EXT2_INODE_ਮਾਪ(fs->super) > EXT2_GOOD_OLD_INODE_ਮਾਪ &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_ਸਿਰਾ);

	retval = ext2fs_inode_csum(fs, inum, inode, &ਚੱਕਰੀ_ਜਾਂਚ, has_hi);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	inode->i_checksum_lo = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(ਚੱਕਰੀ_ਜਾਂਚ & 0xFFFF);
	ਜੇ (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(ਚੱਕਰੀ_ਜਾਂਚ >> 16);
	ਵਾਪਸ 0;
}

__u16 ext2fs_ਸਮੂਹ_desc_csum(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਬਣਤਰ ext2_ਸਮੂਹ_desc *desc = ext2fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc,
							 ਸਮੂਹ);
	size_t ਵਿੱਥ, ਮਾਪ = EXT2_DESC_ਮਾਪ(fs->super);
	__u16 ਚੱਕਰੀ_ਜਾਂਚ = 0;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ਬਣਤਰ ext4_ਸਮੂਹ_desc swabdesc;
	size_t save_ਮਾਪ = ਮਾਪ;
	ਅਚਲ size_t ext4_bg_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext4_ਸਮੂਹ_desc);
	ਬਣਤਰ ext2_ਸਮੂਹ_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	ਜੇ (ਮਾਪ > ext4_bg_ਮਾਪ)
		ਮਾਪ = ext4_bg_ਮਾਪ;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&swabdesc, desc, ਮਾਪ);
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(fs, (ਬਣਤਰ ext2_ਸਮੂਹ_desc *) &swabdesc);
	desc = (ਬਣਤਰ ext2_ਸਮੂਹ_desc *) &swabdesc;
	ਸਮੂਹ = ext2fs_swab32(ਸਮੂਹ);
#ਜੇ_ਅੰਤ

	ਜੇ (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_ਚੱਕਰੀ_ਜਾਂਚ;
		__u32 ਚੱਕਰੀ_ਜਾਂਚ_32;

		old_ਚੱਕਰੀ_ਜਾਂਚ = desc->bg_checksum;
		desc->bg_checksum = 0;
		ਚੱਕਰੀ_ਜਾਂਚ_32 = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&ਸਮੂਹ,
					 ਆਕਾਰ(ਸਮੂਹ));
		ਚੱਕਰੀ_ਜਾਂਚ_32 = ext2fs_crc32c_le(ਚੱਕਰੀ_ਜਾਂਚ_32, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)desc,
					 ਮਾਪ);
		desc->bg_checksum = old_ਚੱਕਰੀ_ਜਾਂਚ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
		ਜੇ (save_ਮਾਪ > ext4_bg_ਮਾਪ)
			ਚੱਕਰੀ_ਜਾਂਚ_32 = ext2fs_crc32c_le(ਚੱਕਰੀ_ਜਾਂਚ_32,
				     (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)save_desc + ext4_bg_ਮਾਪ,
	                             save_ਮਾਪ - ext4_bg_ਮਾਪ);
#ਜੇ_ਅੰਤ
		ਚੱਕਰੀ_ਜਾਂਚ = ਚੱਕਰੀ_ਜਾਂਚ_32 & 0xFFFF;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* old crc16 code */
	ਵਿੱਥ = offsetof(ਬਣਤਰ ext2_ਸਮੂਹ_desc, bg_checksum);
	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc16(~0, fs->super->s_uuid,
			   ਆਕਾਰ(fs->super->s_uuid));
	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc16(ਚੱਕਰੀ_ਜਾਂਚ, &ਸਮੂਹ, ਆਕਾਰ(ਸਮੂਹ));
	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc16(ਚੱਕਰੀ_ਜਾਂਚ, desc, ਵਿੱਥ);
	ਵਿੱਥ += ਆਕਾਰ(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ਜੇ (ਵਿੱਥ < ਮਾਪ) {
		ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc16(ਚੱਕਰੀ_ਜਾਂਚ, (ਅੱਖਰ *)desc + ਵਿੱਥ,
				   ਮਾਪ - ਵਿੱਥ);
	}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	ਜੇ (save_ਮਾਪ > ext4_bg_ਮਾਪ)
		ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc16(ਚੱਕਰੀ_ਜਾਂਚ, (ਅੱਖਰ *)save_desc + ext4_bg_ਮਾਪ,
				   save_ਮਾਪ - ext4_bg_ਮਾਪ);
#ਜੇ_ਅੰਤ

ਬਾਹਰਲਾ:
	ਵਾਪਸ ਚੱਕਰੀ_ਜਾਂਚ;
}

ਪੂਰਨ ext2fs_ਸਮੂਹ_desc_csum_verify(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਜੇ (ext2fs_has_ਸਮੂਹ_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, ਸਮੂਹ) !=
	     ext2fs_ਸਮੂਹ_desc_csum(fs, ਸਮੂਹ)))
		ਵਾਪਸ 0;

	ਵਾਪਸ 1;
}

ਖਾਲੀ ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਜੇ (!ext2fs_has_ਸਮੂਹ_desc_csum(fs))
		ਵਾਪਸ;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_ਸੈੱਟ(fs, ਸਮੂਹ, ext2fs_ਸਮੂਹ_desc_csum(fs, ਸਮੂਹ));
}

ਸਥਿਰ __u32 ਲੱਭੋ_ਆਖਰੀ_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_ਪ੍ਰਤੀ_grp, dgrp_t grp_no)
{
	ext2_ino_t i, ਸ਼ੁਰੂ_ino, ਸਿਰਾ_ino;

	ਸ਼ੁਰੂ_ino = grp_no * inodes_ਪ੍ਰਤੀ_grp + 1;
	ਸਿਰਾ_ino = ਸ਼ੁਰੂ_ino + inodes_ਪ੍ਰਤੀ_grp - 1;

	ਲਈ (i = ਸਿਰਾ_ino; i >= ਸ਼ੁਰੂ_ino; i--) {
		ਜੇ (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			ਵਾਪਸ i - ਸ਼ੁਰੂ_ino + 1;
	}
	ਵਾਪਸ inodes_ਪ੍ਰਤੀ_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_ਸੈੱਟ_gdt_csum(ext2_filsys fs)
{
	ਬਣਤਰ ext2_super_ਖੰਡ *sb = fs->super;
	ਪੂਰਨ dirty = 0;
	dgrp_t i;

	ਜੇ (!fs->inode_map)
		ਵਾਪਸ EXT2_ET_NO_INODE_BITMAP;

	ਜੇ (!ext2fs_has_ਸਮੂਹ_desc_csum(fs))
		ਵਾਪਸ 0;

	ਲਈ (i = 0; i < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_ਅਣਵਰਤਿਆ = ext2fs_bg_itable_ਅਣਵਰਤਿਆ(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_ਗਿਣਤੀ = ext2fs_bg_free_inodes_ਗਿਣਤੀ(fs, i);
		__u32 old_free_ਖੰਡਾਂ_ਗਿਣਤੀ = ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, i);

		ਜੇ (old_free_ਖੰਡਾਂ_ਗਿਣਤੀ == sb->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ &&
		    i != fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1)
			ext2fs_bg_flags_ਸੈੱਟ(fs, i, EXT2_BG_ਖੰਡ_UNINIT);

		ਜੇ (old_free_inodes_ਗਿਣਤੀ == sb->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ) {
			ext2fs_bg_flags_ਸੈੱਟ(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_ਅਣਵਰਤਿਆ_ਸੈੱਟ(fs, i, sb->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ);
		} ਨਹੀਂਤਾਂ {
			ਪੂਰਨ ਅਣਵਰਤਿਆ =
				sb->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ -
				ਲੱਭੋ_ਆਖਰੀ_inode_ingrp(fs->inode_map,
						      sb->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_ਅਣਵਰਤਿਆ_ਸੈੱਟ(fs, i, ਅਣਵਰਤਿਆ);
		}

		ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, i);
		ਜੇ (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		ਜੇ (old_ਅਣਵਰਤਿਆ != ext2fs_bg_itable_ਅਣਵਰਤਿਆ(fs, i))
			dirty = 1;
		ਜੇ (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	ਜੇ (dirty)
		ext2fs_mark_super_dirty(fs);
	ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
#ਅਨ੍ਤਰ੍ਭਾਵ "e2p/e2p.h"

ਖਾਲੀ print_csum(ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	ਬਣਤਰ ext2_ਸਮੂਹ_desc *desc = ext2fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc,
							 ਸਮੂਹ);
	size_t ਮਾਪ = EXT2_DESC_ਮਾਪ(fs->super);
	ਬਣਤਰ ext2_super_ਖੰਡ *sb = fs->super;
	ਪੂਰਨ ਵਿੱਥ = offsetof(ਬਣਤਰ ext2_ਸਮੂਹ_desc, bg_checksum);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ਬਣਤਰ ext4_ਸਮੂਹ_desc swabdesc;
	ਬਣਤਰ ext2_ਸਮੂਹ_desc *save_desc = desc;
	ਅਚਲ size_t ext4_bg_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext4_ਸਮੂਹ_desc);
	size_t save_ਮਾਪ = ਮਾਪ;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	ਜੇ (ਮਾਪ > ext4_bg_ਮਾਪ)
		ਮਾਪ = ext4_bg_ਮਾਪ;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&swabdesc, desc, ਮਾਪ);
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(fs, (ਬਣਤਰ ext2_ਸਮੂਹ_desc *) &swabdesc);
	desc = (ਬਣਤਰ ext2_ਸਮੂਹ_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(ਸਮੂਹ);
#ਨਹੀਂਤਾਂ
	swabgroup = ਸਮੂਹ;
#ਜੇ_ਅੰਤ

	crc1 = ext2fs_crc16(~0, sb->s_uuid, ਆਕਾਰ(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, ਆਕਾਰ(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, ਵਿੱਥ);
	ਵਿੱਥ += ਆਕਾਰ(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ਜੇ (ਵਿੱਥ < ਮਾਪ)
		crc3 = ext2fs_crc16(crc3, (ਅੱਖਰ *)desc + ਵਿੱਥ, ਮਾਪ - ਵਿੱਥ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ਜੇ (save_ਮਾਪ > ext4_bg_ਮਾਪ)
		crc3 = ext2fs_crc16(crc3, (ਅੱਖਰ *)save_desc + ext4_bg_ਮਾਪ,
				    save_ਮਾਪ - ext4_bg_ਮਾਪ);
#ਜੇ_ਅੰਤ

	ਲਿਖੋ("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       ਸੁਨੇਹਾ, e2p_uuid2str(sb->s_uuid), crc1, ਸਮੂਹ, crc2, crc3,
	       ext2fs_ਸਮੂਹ_desc_csum(fs, ਸਮੂਹ));
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਬਣਤਰ ext2_super_ਖੰਡ param;
	errcode_t		retval;
	ext2_filsys		fs;
	ਪੂਰਨ			i;
	__u16 csum1, csum2, csum_ਗਿਆਤ = 0xd3a4;

	memset(&param, 0, ਆਕਾਰ(param));
	ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(&param, 32768);
#ਜੇ 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64_ਦੋਆਧਾਰੀ_ਅੰਕ;
	param.s_desc_ਮਾਪ = 128;
	csum_ਗਿਆਤ = 0x5b6e;
#ਜੇ_ਅੰਤ

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_ਬਿੱਟ, &param,
				   test_io_manager, &fs);
	ਜੇ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	ਲਈ (i=0; i < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++) {
		ext2fs_ਖੰਡ_bitmap_loc_ਸੈੱਟ(fs, i, 124);
		ext2fs_inode_bitmap_loc_ਸੈੱਟ(fs, i, 125);
		ext2fs_inode_ਸਾਰਣੀ_loc_ਸੈੱਟ(fs, i, 126);
		ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs, i, 31119);
		ext2fs_bg_free_inodes_ਗਿਣਤੀ_ਸੈੱਟ(fs, i, 15701);
		ext2fs_bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ_ਸੈੱਟ(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_ਸਮੂਹ_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	ਜੇ (csum1 != csum_ਗਿਆਤ) {
		ਲਿਖੋ("checksum for group 0 should be %04x\n", csum_ਗਿਆਤ);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	csum2 = ext2fs_ਸਮੂਹ_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	ਜੇ (csum1 == csum2) {
		ਲਿਖੋ("checksums for different groups shouldn't match\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	csum2 = ext2fs_ਸਮੂਹ_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	ਜੇ (csum1 == csum2) {
		ਲਿਖੋ("checksums for different groups shouldn't match\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ext2fs_bg_checksum_ਸੈੱਟ(fs, 0, csum1);
	csum2 = ext2fs_ਸਮੂਹ_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	ਜੇ (csum1 != csum2) {
		ਲਿਖੋ("checksums should not depend on checksum field\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (!ext2fs_ਸਮੂਹ_desc_csum_verify(fs, 0)) {
		ਲਿਖੋ("checksums should verify against gd_checksum\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	memset(fs->super->s_uuid, 0x30, ਆਕਾਰ(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	ਜੇ (ext2fs_ਸਮੂਹ_desc_csum_verify(fs, 0) != 0) {
		ਲਿਖੋ("checksums for different filesystems shouldn't match\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	csum1 = ext2fs_ਸਮੂਹ_desc_csum(fs, 0);
	ext2fs_bg_checksum_ਸੈੱਟ(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs, 0, 1);
	csum2 = ext2fs_ਸਮੂਹ_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	ਜੇ (csum1 == csum2) {
		ਲਿਖੋ("checksums for different data shouldn't match\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ext2fs_free(fs);

	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ
