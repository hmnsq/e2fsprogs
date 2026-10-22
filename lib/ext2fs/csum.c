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

#ضمن "config.h"
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "crc16.h"
#ضمن <assert.h>

#جيڪڏهن_وصف_نه_ٿيل offsetof
#وصف_ڪريو offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل DEBUG
#وصف_ڪريو STATIC
#وإلا
#وصف_ڪريو STATIC ساكن
#جيڪڏهن_پورو

فراغ ext2fs_init_csum_ٻج(ext2_filsys fs)
{
	إذا (ext2fs_has_feature_csum_ٻج(fs->super))
		fs->csum_ٻج = fs->super->s_checksum_ٻج;
	وإلا إذا (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_ٻج = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 حجم(fs->super->s_uuid));
}

ساكن __u32 ext2fs_mmp_csum(ext2_filsys fs, بنية mmp_struct *mmp)
{
	صحيح وٿي = offsetof(بنية mmp_struct, mmp_checksum);

	أرجع ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)mmp, وٿي);
}

صحيح ext2fs_mmp_csum_verify(ext2_filsys fs, بنية mmp_struct *mmp)
{
	__u32 calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	أرجع ext2fs_le32_منزل_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_سيٽ(ext2_filsys fs, بنية mmp_struct *mmp)
{
	__u32 چڪري_جاچ;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	چڪري_جاچ = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_منزل_le32(چڪري_جاچ);

	أرجع 0;
}

صحيح ext2fs_verify_csum_type(ext2_filsys fs, بنية ext2_super_بلاڪ *sb)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	أرجع sb->s_checksum_type == EXT2_چڪري_جاچ_32_C_CHKSUM;
}

ساكن __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
				    بنية ext2_super_بلاڪ *sb)
{
	صحيح وٿي = offsetof(بنية ext2_super_بلاڪ, s_checksum);

	أرجع ext2fs_crc32c_le(~0, (بنانشان حرف *)sb, وٿي);
}

/* NOTE: The input to this function MUST be in LE order */
صحيح ext2fs_superblock_csum_verify(ext2_filsys fs, بنية ext2_super_بلاڪ *sb)
{
	__u32 flag, calculated;

	إذا (fs->flags & EXT2_FLAG_مٽا_سٽا_بائيٽون)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وإلا
		flag = ext2fs_cpu_منزل_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	إذا (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		أرجع 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	أرجع ext2fs_le32_منزل_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_سيٽ(ext2_filsys fs,
				     بنية ext2_super_بلاڪ *sb)
{
	__u32 flag, چڪري_جاچ;

	إذا (fs->flags & EXT2_FLAG_مٽا_سٽا_بائيٽون)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وإلا
		flag = ext2fs_cpu_منزل_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	إذا (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		أرجع 0;

	چڪري_جاچ = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_منزل_le32(چڪري_جاچ);

	أرجع 0;
}

ساكن errcode_t ext2fs_ext_attr_بلاڪ_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
					    blk64_t بلاڪ,
					    بنية ext2_ext_attr_سرو *hdr,
					    __u32 *چڪري_جاچ)
{
	حرف *buf = (حرف *)hdr;
	__u32 old_چڪري_جاچ = hdr->h_checksum;

	hdr->h_checksum = 0;
	بلاڪ = ext2fs_cpu_منزل_le64(بلاڪ);
	*چڪري_جاچ = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)&بلاڪ,
				حجم(بلاڪ));
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)buf, fs->blocksize);
	hdr->h_checksum = old_چڪري_جاچ;

	أرجع 0;
}

صحيح ext2fs_ext_attr_بلاڪ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t بلاڪ,
				      بنية ext2_ext_attr_سرو *hdr)
{
	__u32 calculated;
	errcode_t retval;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	retval = ext2fs_ext_attr_بلاڪ_csum(fs, inum, بلاڪ, hdr, &calculated);
	إذا (retval)
		أرجع 0;

	أرجع ext2fs_le32_منزل_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_بلاڪ_csum_سيٽ(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t بلاڪ,
					 بنية ext2_ext_attr_سرو *hdr)
{
	errcode_t retval;
	__u32 چڪري_جاچ;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	retval = ext2fs_ext_attr_بلاڪ_csum(fs, inum, بلاڪ, hdr, &چڪري_جاچ);
	إذا (retval)
		أرجع retval;
	hdr->h_checksum = ext2fs_cpu_منزل_le32(چڪري_جاچ);
	أرجع 0;
}

ساكن __u16 do_nothing16(__u16 x)
{
	أرجع x;
}

ساكن __u16 disk_منزل_host16(__u16 x)
{
	أرجع ext2fs_le16_منزل_cpu(x);
}

ساكن errcode_t __get_dx_countlimit(ext2_filsys fs,
				     بنية ext2_dir_داخلا *dirent,
				     بنية ext2_dx_countlimit **cc,
				     صحيح *وٿي,
				     صحيح need_swab)
{
	بنية ext2_dir_داخلا *dp;
	بنية ext2_dx_پاڙ_info *پاڙ;
	بنية ext2_dx_countlimit *c;
	صحيح ڳڻپ_وٿي, max_sane_entries;
	بنانشان صحيح rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_منزل_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	إذا (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		ڳڻپ_وٿي = 8;
	وإلا إذا (rec_len == 12) {
		dp = (بنية ext2_dir_داخلا *)(((حرف *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		إذا (rec_len != fs->blocksize - 12)
			أرجع EXT2_ET_DB_NOT_مليو;
		پاڙ = (بنية ext2_dx_پاڙ_info *)(((حرف *)dp + 12));
		إذا (پاڙ->reserved_zero ||
		    پاڙ->info_ڊيگهائي != حجم(بنية ext2_dx_پاڙ_info))
			أرجع EXT2_ET_DB_NOT_مليو;
		ڳڻپ_وٿي = 32;
	} وإلا
		أرجع EXT2_ET_DB_NOT_مليو;

	c = (بنية ext2_dx_countlimit *)(((حرف *)dirent) + ڳڻپ_وٿي);
	max_sane_entries = (fs->blocksize - ڳڻپ_وٿي) /
			   حجم(بنية ext2_dx_داخلا);
	إذا (ext2fs_le16_منزل_cpu(c->حد_بندي) > max_sane_entries ||
	    ext2fs_le16_منزل_cpu(c->ڳڻپ) > max_sane_entries)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	إذا (وٿي)
		*وٿي = ڳڻپ_وٿي;
	إذا (cc)
		*cc = c;

	أرجع 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   بنية ext2_dir_داخلا *dirent,
				   بنية ext2_dx_countlimit **cc,
				   صحيح *وٿي)
{
	أرجع __get_dx_countlimit(fs, dirent, cc, وٿي, 0);
}

فراغ ext2fs_initialize_dirent_پڇ(ext2_filsys fs,
				   بنية ext2_dir_داخلا_پڇ *t)
{
	memset(t, 0, حجم(بنية ext2_dir_داخلا_پڇ));
	ext2fs_سيٽ_rec_len(fs, حجم(بنية ext2_dir_داخلا_پڇ),
			   (بنية ext2_dir_داخلا *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ساكن errcode_t __get_dirent_tail(ext2_filsys fs,
				   بنية ext2_dir_داخلا *dirent,
				   بنية ext2_dir_داخلا_پڇ **tt,
				   صحيح need_swab)
{
	بنية ext2_dir_داخلا *d;
	فراغ *چوٽي;
	بنية ext2_dir_داخلا_پڇ *t;
	بنانشان صحيح rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_منزل_host16 : do_nothing16);

	إذا (fs->blocksize < 1024)
		أرجع EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	چوٽي = EXT2_DIRENT_پڇ(dirent, fs->blocksize);

	طالما ((فراغ *) d < چوٽي) {
		rec_len = translate(d->rec_len);
		إذا ((rec_len < 8) || (rec_len & 0x03))
			أرجع EXT2_ET_DIR_CORRUPTED;
		d = (بنية ext2_dir_داخلا *)(((حرف *)d) + rec_len);
	}

	إذا ((حرف *)d > ((حرف *)dirent + fs->blocksize))
			أرجع EXT2_ET_DIR_CORRUPTED;
	إذا (d != چوٽي)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (بنية ext2_dir_داخلا_پڇ *)d;
	إذا (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != حجم(بنية ext2_dir_داخلا_پڇ) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	إذا (tt)
		*tt = t;
	أرجع retval;
}

صحيح ext2fs_dirent_has_پڇ(ext2_filsys fs, بنية ext2_dir_داخلا *dirent)
{
	أرجع __get_dirent_tail(fs, dirent, عدم, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ساكن errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_داخلا *dirent, __u32 *چڪري_جاچ,
				    صحيح سائيز)
{
	errcode_t retval;
	حرف *buf = (حرف *)dirent;
	__u32 gen;
	بنية ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	إذا (retval)
		أرجع retval;

	inum = ext2fs_cpu_منزل_le32(inum);
	gen = ext2fs_cpu_منزل_le32(inode.i_generation);
	*چڪري_جاچ = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)&inum,
				حجم(inum));
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)&gen, حجم(gen));
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)buf, سائيز);

	أرجع 0;
}

صحيح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      بنية ext2_dir_داخلا *dirent)
{
	errcode_t retval;
	__u32 calculated;
	بنية ext2_dir_داخلا_پڇ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	إذا (retval)
		أرجع 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (حرف *)t - (حرف *)dirent);
	إذا (retval)
		أرجع 0;
	أرجع ext2fs_le32_منزل_cpu(t->det_checksum) == calculated;
}

ساكن errcode_t ext2fs_dirent_csum_سيٽ(ext2_filsys fs, ext2_ino_t inum,
					بنية ext2_dir_داخلا *dirent)
{
	errcode_t retval;
	__u32 چڪري_جاچ;
	بنية ext2_dir_داخلا_پڇ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	إذا (retval)
		أرجع retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &چڪري_جاچ,
				    (حرف *)t - (حرف *)dirent);
	إذا (retval)
		أرجع retval;
	t->det_checksum = ext2fs_cpu_منزل_le32(چڪري_جاچ);
	أرجع 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 بنية ext2_dir_داخلا *dirent,
			 __u32 *چڪري_جاچ, بنية ext2_dx_پڇ **ret_t)
{
	errcode_t retval;
	حرف *buf = (حرف *)dirent;
	صحيح سائيز;
	__u32 gen, dummy_csum = 0;
	بنية ext2_inode inode;
	بنية ext2_dx_پڇ *t;
	بنية ext2_dx_countlimit *c;
	صحيح ڳڻپ_وٿي, حد_بندي, ڳڻپ;

	retval = __get_dx_countlimit(fs, dirent, &c, &ڳڻپ_وٿي, 1);
	إذا (retval)
		أرجع retval;
	حد_بندي = ext2fs_le16_منزل_cpu(c->حد_بندي);
	ڳڻپ = ext2fs_le16_منزل_cpu(c->ڳڻپ);
	إذا (ڳڻپ_وٿي + (حد_بندي * حجم(بنية ext2_dx_داخلا)) >
	    fs->blocksize - حجم(بنية ext2_dx_پڇ))
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (بنية ext2_dx_پڇ *)(((بنية ext2_dx_داخلا *)c) + حد_بندي);

	سائيز = ڳڻپ_وٿي + (ڳڻپ * حجم(بنية ext2_dx_داخلا));

	retval = ext2fs_read_inode(fs, inum, &inode);
	إذا (retval)
		أرجع retval;

	inum = ext2fs_cpu_منزل_le32(inum);
	gen = ext2fs_cpu_منزل_le32(inode.i_generation);
	*چڪري_جاچ = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)&inum,
				حجم(inum));
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)&gen, حجم(gen));
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)buf, سائيز);
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)t, 4);
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)&dummy_csum, 4);

	إذا (ret_t)
		*ret_t = t;
	أرجع 0;
}

ساكن صحيح ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 بنية ext2_dir_داخلا *dirent)
{
	__u32 calculated;
	errcode_t retval;
	بنية ext2_dx_پڇ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	إذا (retval)
		أرجع 0;

	أرجع ext2fs_le32_منزل_cpu(t->dt_checksum) == calculated;
}

ساكن errcode_t ext2fs_dx_csum_سيٽ(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_داخلا *dirent)
{
	__u32 چڪري_جاچ;
	errcode_t retval = 0;
	بنية ext2_dx_پڇ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &چڪري_جاچ, &t);
	إذا (retval)
		أرجع retval;
	t->dt_checksum = ext2fs_cpu_منزل_le32(چڪري_جاچ);
	أرجع retval;
}

صحيح ext2fs_dir_بلاڪ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 بنية ext2_dir_داخلا *dirent)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	إذا (__get_dirent_tail(fs, dirent, عدم, 1) == 0)
		أرجع ext2fs_dirent_csum_verify(fs, inum, dirent);
	إذا (__get_dx_countlimit(fs, dirent, عدم, عدم, 1) == 0)
		أرجع ext2fs_dx_csum_verify(fs, inum, dirent);

	أرجع 0;
}

errcode_t ext2fs_dir_بلاڪ_csum_سيٽ(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_داخلا *dirent)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	إذا (__get_dirent_tail(fs, dirent, عدم, 1) == 0)
		أرجع ext2fs_dirent_csum_سيٽ(fs, inum, dirent);
	إذا (__get_dx_countlimit(fs, dirent, عدم, عدم, 1) == 0)
		أرجع ext2fs_dx_csum_سيٽ(fs, inum, dirent);

	إذا (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		أرجع 0;
	أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#وصف_ڪريو EXT3_EXTENT_پڇ_وٿي(hdr)	(حجم(بنية ext3_extent_سرو) + \
	(حجم(بنية ext3_extent) * ext2fs_le16_منزل_cpu((hdr)->eh_max)))

ساكن بنية ext3_extent_پڇ *get_extent_پڇ(بنية ext3_extent_سرو *h)
{
	أرجع (بنية ext3_extent_پڇ *)(((حرف *)h) +
					   EXT3_EXTENT_پڇ_وٿي(h));
}

ساكن errcode_t ext2fs_extent_بلاڪ_csum(ext2_filsys fs, ext2_ino_t inum,
					  بنية ext3_extent_سرو *eh,
					  __u32 *چڪري_جاچ)
{
	صحيح سائيز;
	__u32 gen;
	errcode_t retval;
	بنية ext2_inode inode;

	سائيز = EXT3_EXTENT_پڇ_وٿي(eh) + offsetof(بنية ext3_extent_پڇ,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	إذا (retval)
		أرجع retval;
	inum = ext2fs_cpu_منزل_le32(inum);
	gen = ext2fs_cpu_منزل_le32(inode.i_generation);
	*چڪري_جاچ = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)&inum,
				حجم(inum));
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)&gen, حجم(gen));
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)eh, سائيز);

	أرجع 0;
}

صحيح ext2fs_extent_بلاڪ_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext3_extent_سرو *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	بنية ext3_extent_پڇ *t = get_extent_پڇ(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	provided = ext2fs_le32_منزل_cpu(t->et_checksum);
	retval = ext2fs_extent_بلاڪ_csum(fs, inum, eh, &calculated);
	إذا (retval)
		أرجع 0;

	أرجع provided == calculated;
}

errcode_t ext2fs_extent_بلاڪ_csum_سيٽ(ext2_filsys fs, ext2_ino_t inum,
				       بنية ext3_extent_سرو *eh)
{
	errcode_t retval;
	__u32 چڪري_جاچ;
	بنية ext3_extent_پڇ *t = get_extent_پڇ(eh);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_بلاڪ_csum(fs, inum, eh, &چڪري_جاچ);
	إذا (retval)
		أرجع retval;
	t->et_checksum = ext2fs_cpu_منزل_le32(چڪري_جاچ);
	أرجع retval;
}

صحيح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروهه,
				    حرف *bitmap, صحيح سائيز)
{
	بنية ext4_گروهه_desc *gdp = (بنية ext4_گروهه_desc *)
			ext2fs_گروهه_desc(fs, fs->گروهه_desc, گروهه);
	__u32 provided, calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)bitmap,
				      سائيز);
	إذا (EXT2_DESC_سائيز(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_پڇاڙي)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	وإلا
		calculated &= 0xFFFF;

	أرجع provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_سيٽ(ext2_filsys fs, dgrp_t گروهه,
				       حرف *bitmap, صحيح سائيز)
{
	__u32 چڪري_جاچ;
	بنية ext4_گروهه_desc *gdp = (بنية ext4_گروهه_desc *)
			ext2fs_گروهه_desc(fs, fs->گروهه_desc, گروهه);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	چڪري_جاچ = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)bitmap, سائيز);
	gdp->bg_inode_bitmap_csum_lo = چڪري_جاچ & 0xFFFF;
	إذا (EXT2_DESC_سائيز(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_پڇاڙي)
		gdp->bg_inode_bitmap_csum_hi = چڪري_جاچ >> 16;

	أرجع 0;
}

صحيح ext2fs_بلاڪ_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروهه,
				    حرف *bitmap, صحيح سائيز)
{
	بنية ext4_گروهه_desc *gdp = (بنية ext4_گروهه_desc *)
			ext2fs_گروهه_desc(fs, fs->گروهه_desc, گروهه);
	__u32 provided, calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	provided = gdp->bg_بلاڪ_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)bitmap,
				      سائيز);
	إذا (EXT2_DESC_سائيز(fs->super) >= EXT4_BG_بلاڪ_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_بلاڪ_bitmap_csum_hi << 16;
	وإلا
		calculated &= 0xFFFF;

	أرجع provided == calculated;
}

errcode_t ext2fs_بلاڪ_bitmap_csum_سيٽ(ext2_filsys fs, dgrp_t گروهه,
				       حرف *bitmap, صحيح سائيز)
{
	__u32 چڪري_جاچ;
	بنية ext4_گروهه_desc *gdp = (بنية ext4_گروهه_desc *)
			ext2fs_گروهه_desc(fs, fs->گروهه_desc, گروهه);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	چڪري_جاچ = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)bitmap, سائيز);
	gdp->bg_بلاڪ_bitmap_csum_lo = چڪري_جاچ & 0xFFFF;
	إذا (EXT2_DESC_سائيز(fs->super) >= EXT4_BG_بلاڪ_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_بلاڪ_bitmap_csum_hi = چڪري_جاچ >> 16;

	أرجع 0;
}

ساكن errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       بنية ext2_inode_large *inode,
			       __u32 *چڪري_جاچ, صحيح has_hi)
{
	__u32 gen;
	بنية ext2_inode_large *desc = inode;
	size_t سائيز = EXT2_INODE_سائيز(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	إذا (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_منزل_le32(inum);
	gen = inode->i_generation;
	*چڪري_جاچ = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)&inum,
				حجم(inum));
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)&gen, حجم(gen));
	*چڪري_جاچ = ext2fs_crc32c_le(*چڪري_جاچ, (بنانشان حرف *)desc, سائيز);

	inode->i_checksum_lo = old_lo;
	إذا (has_hi)
		inode->i_checksum_hi = old_hi;
	أرجع 0;
}

صحيح ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     بنية ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	بنانشان صحيح i, has_hi;
	حرف *cp;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	has_hi = (EXT2_INODE_سائيز(fs->super) > EXT2_GOOD_OLD_INODE_سائيز &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_پڇاڙي);

	provided = ext2fs_le16_منزل_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	إذا (retval)
		أرجع 0;
	إذا (has_hi) {
		__u32 hi = ext2fs_le16_منزل_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} وإلا
		calculated &= 0xFFFF;

	إذا (provided == calculated)
		أرجع 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	لكل (cp = (حرف *) inode, i = 0;
	     i < حجم(بنية ext2_inode);
	     cp++, i++)
		إذا (*cp)
			أرجع 0;
	أرجع 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_سيٽ(ext2_filsys fs, ext2_ino_t inum,
			   بنية ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 چڪري_جاچ;
	صحيح has_hi;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	has_hi = (EXT2_INODE_سائيز(fs->super) > EXT2_GOOD_OLD_INODE_سائيز &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_پڇاڙي);

	retval = ext2fs_inode_csum(fs, inum, inode, &چڪري_جاچ, has_hi);
	إذا (retval)
		أرجع retval;
	inode->i_checksum_lo = ext2fs_cpu_منزل_le16(چڪري_جاچ & 0xFFFF);
	إذا (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_منزل_le16(چڪري_جاچ >> 16);
	أرجع 0;
}

__u16 ext2fs_گروهه_desc_csum(ext2_filsys fs, dgrp_t گروهه)
{
	بنية ext2_گروهه_desc *desc = ext2fs_گروهه_desc(fs, fs->گروهه_desc,
							 گروهه);
	size_t وٿي, سائيز = EXT2_DESC_سائيز(fs->super);
	__u16 چڪري_جاچ = 0;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	بنية ext4_گروهه_desc swabdesc;
	size_t save_سائيز = سائيز;
	ثابت size_t ext4_bg_سائيز = حجم(بنية ext4_گروهه_desc);
	بنية ext2_گروهه_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	إذا (سائيز > ext4_bg_سائيز)
		سائيز = ext4_bg_سائيز;
	يادگيري_نقل(&swabdesc, desc, سائيز);
	ext2fs_مٽا_سٽا_گروهه_desc2(fs, (بنية ext2_گروهه_desc *) &swabdesc);
	desc = (بنية ext2_گروهه_desc *) &swabdesc;
	گروهه = ext2fs_swab32(گروهه);
#جيڪڏهن_پورو

	إذا (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_چڪري_جاچ;
		__u32 چڪري_جاچ_32;

		old_چڪري_جاچ = desc->bg_checksum;
		desc->bg_checksum = 0;
		چڪري_جاچ_32 = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)&گروهه,
					 حجم(گروهه));
		چڪري_جاچ_32 = ext2fs_crc32c_le(چڪري_جاچ_32, (بنانشان حرف *)desc,
					 سائيز);
		desc->bg_checksum = old_چڪري_جاچ;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
		إذا (save_سائيز > ext4_bg_سائيز)
			چڪري_جاچ_32 = ext2fs_crc32c_le(چڪري_جاچ_32,
				     (بنانشان حرف *)save_desc + ext4_bg_سائيز,
	                             save_سائيز - ext4_bg_سائيز);
#جيڪڏهن_پورو
		چڪري_جاچ = چڪري_جاچ_32 & 0xFFFF;
		اذهب ٻاهريون;
	}

	/* old crc16 code */
	وٿي = offsetof(بنية ext2_گروهه_desc, bg_checksum);
	چڪري_جاچ = ext2fs_crc16(~0, fs->super->s_uuid,
			   حجم(fs->super->s_uuid));
	چڪري_جاچ = ext2fs_crc16(چڪري_جاچ, &گروهه, حجم(گروهه));
	چڪري_جاچ = ext2fs_crc16(چڪري_جاچ, desc, وٿي);
	وٿي += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	إذا (وٿي < سائيز) {
		چڪري_جاچ = ext2fs_crc16(چڪري_جاچ, (حرف *)desc + وٿي,
				   سائيز - وٿي);
	}
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	إذا (save_سائيز > ext4_bg_سائيز)
		چڪري_جاچ = ext2fs_crc16(چڪري_جاچ, (حرف *)save_desc + ext4_bg_سائيز,
				   save_سائيز - ext4_bg_سائيز);
#جيڪڏهن_پورو

ٻاهريون:
	أرجع چڪري_جاچ;
}

صحيح ext2fs_گروهه_desc_csum_verify(ext2_filsys fs, dgrp_t گروهه)
{
	إذا (ext2fs_has_گروهه_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, گروهه) !=
	     ext2fs_گروهه_desc_csum(fs, گروهه)))
		أرجع 0;

	أرجع 1;
}

فراغ ext2fs_گروهه_desc_csum_سيٽ(ext2_filsys fs, dgrp_t گروهه)
{
	إذا (!ext2fs_has_گروهه_desc_csum(fs))
		أرجع;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_سيٽ(fs, گروهه, ext2fs_گروهه_desc_csum(fs, گروهه));
}

ساكن __u32 ڳوليو_آخري_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_في_grp, dgrp_t grp_no)
{
	ext2_ino_t i, شروعات_ino, پڇاڙي_ino;

	شروعات_ino = grp_no * inodes_في_grp + 1;
	پڇاڙي_ino = شروعات_ino + inodes_في_grp - 1;

	لكل (i = پڇاڙي_ino; i >= شروعات_ino; i--) {
		إذا (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			أرجع i - شروعات_ino + 1;
	}
	أرجع inodes_في_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_سيٽ_gdt_csum(ext2_filsys fs)
{
	بنية ext2_super_بلاڪ *sb = fs->super;
	صحيح dirty = 0;
	dgrp_t i;

	إذا (!fs->inode_map)
		أرجع EXT2_ET_NO_INODE_BITMAP;

	إذا (!ext2fs_has_گروهه_desc_csum(fs))
		أرجع 0;

	لكل (i = 0; i < fs->گروهه_desc_ڳڻپ; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_اڻ_استعمال_ٿيل = ext2fs_bg_itable_اڻ_استعمال_ٿيل(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_ڳڻپ = ext2fs_bg_free_inodes_ڳڻپ(fs, i);
		__u32 old_free_بلاڪون_ڳڻپ = ext2fs_bg_free_بلاڪون_ڳڻپ(fs, i);

		إذا (old_free_بلاڪون_ڳڻپ == sb->s_بلاڪون_في_گروهه &&
		    i != fs->گروهه_desc_ڳڻپ - 1)
			ext2fs_bg_flags_سيٽ(fs, i, EXT2_BG_بلاڪ_UNINIT);

		إذا (old_free_inodes_ڳڻپ == sb->s_inodes_في_گروهه) {
			ext2fs_bg_flags_سيٽ(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_اڻ_استعمال_ٿيل_سيٽ(fs, i, sb->s_inodes_في_گروهه);
		} وإلا {
			صحيح اڻ_استعمال_ٿيل =
				sb->s_inodes_في_گروهه -
				ڳوليو_آخري_inode_ingrp(fs->inode_map,
						      sb->s_inodes_في_گروهه, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_اڻ_استعمال_ٿيل_سيٽ(fs, i, اڻ_استعمال_ٿيل);
		}

		ext2fs_گروهه_desc_csum_سيٽ(fs, i);
		إذا (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		إذا (old_اڻ_استعمال_ٿيل != ext2fs_bg_itable_اڻ_استعمال_ٿيل(fs, i))
			dirty = 1;
		إذا (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	إذا (dirty)
		ext2fs_mark_super_dirty(fs);
	أرجع 0;
}

#جيڪڏهن_وصف_ٿيل DEBUG
#ضمن "e2p/e2p.h"

فراغ print_csum(ثابت حرف *نياپو, ext2_filsys fs, dgrp_t گروهه)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	بنية ext2_گروهه_desc *desc = ext2fs_گروهه_desc(fs, fs->گروهه_desc,
							 گروهه);
	size_t سائيز = EXT2_DESC_سائيز(fs->super);
	بنية ext2_super_بلاڪ *sb = fs->super;
	صحيح وٿي = offsetof(بنية ext2_گروهه_desc, bg_checksum);
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	بنية ext4_گروهه_desc swabdesc;
	بنية ext2_گروهه_desc *save_desc = desc;
	ثابت size_t ext4_bg_سائيز = حجم(بنية ext4_گروهه_desc);
	size_t save_سائيز = سائيز;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	إذا (سائيز > ext4_bg_سائيز)
		سائيز = ext4_bg_سائيز;
	يادگيري_نقل(&swabdesc, desc, سائيز);
	ext2fs_مٽا_سٽا_گروهه_desc2(fs, (بنية ext2_گروهه_desc *) &swabdesc);
	desc = (بنية ext2_گروهه_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(گروهه);
#وإلا
	swabgroup = گروهه;
#جيڪڏهن_پورو

	crc1 = ext2fs_crc16(~0, sb->s_uuid, حجم(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, حجم(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, وٿي);
	وٿي += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	إذا (وٿي < سائيز)
		crc3 = ext2fs_crc16(crc3, (حرف *)desc + وٿي, سائيز - وٿي);
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	إذا (save_سائيز > ext4_bg_سائيز)
		crc3 = ext2fs_crc16(crc3, (حرف *)save_desc + ext4_bg_سائيز,
				    save_سائيز - ext4_bg_سائيز);
#جيڪڏهن_پورو

	لکو("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       نياپو, e2p_uuid2str(sb->s_uuid), crc1, گروهه, crc2, crc3,
	       ext2fs_گروهه_desc_csum(fs, گروهه));
}

بنانشان حرف sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

صحيح مکيه(صحيح argc, حرف **argv)
{
	بنية ext2_super_بلاڪ param;
	errcode_t		retval;
	ext2_filsys		fs;
	صحيح			i;
	__u16 csum1, csum2, csum_معلوم = 0xd3a4;

	memset(&param, 0, حجم(param));
	ext2fs_بلاڪون_ڳڻپ_سيٽ(&param, 32768);
#إذا 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64_ٻٽو_انگ;
	param.s_desc_سائيز = 128;
	csum_معلوم = 0x5b6e;
#جيڪڏهن_پورو

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بٽ, &param,
				   test_io_manager, &fs);
	إذا (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		ٻاهر_نڪرو(1);
	}
	يادگيري_نقل(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	لكل (i=0; i < fs->گروهه_desc_ڳڻپ; i++) {
		ext2fs_بلاڪ_bitmap_loc_سيٽ(fs, i, 124);
		ext2fs_inode_bitmap_loc_سيٽ(fs, i, 125);
		ext2fs_inode_جدول_loc_سيٽ(fs, i, 126);
		ext2fs_bg_free_بلاڪون_ڳڻپ_سيٽ(fs, i, 31119);
		ext2fs_bg_free_inodes_ڳڻپ_سيٽ(fs, i, 15701);
		ext2fs_bg_استعمال_ٿيل_dirs_ڳڻپ_سيٽ(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_گروهه_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	إذا (csum1 != csum_معلوم) {
		لکو("checksum for group 0 should be %04x\n", csum_معلوم);
		ٻاهر_نڪرو(1);
	}
	csum2 = ext2fs_گروهه_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	إذا (csum1 == csum2) {
		لکو("checksums for different groups shouldn't match\n");
		ٻاهر_نڪرو(1);
	}
	csum2 = ext2fs_گروهه_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	إذا (csum1 == csum2) {
		لکو("checksums for different groups shouldn't match\n");
		ٻاهر_نڪرو(1);
	}
	ext2fs_bg_checksum_سيٽ(fs, 0, csum1);
	csum2 = ext2fs_گروهه_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	إذا (csum1 != csum2) {
		لکو("checksums should not depend on checksum field\n");
		ٻاهر_نڪرو(1);
	}
	إذا (!ext2fs_گروهه_desc_csum_verify(fs, 0)) {
		لکو("checksums should verify against gd_checksum\n");
		ٻاهر_نڪرو(1);
	}
	memset(fs->super->s_uuid, 0x30, حجم(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	إذا (ext2fs_گروهه_desc_csum_verify(fs, 0) != 0) {
		لکو("checksums for different filesystems shouldn't match\n");
		ٻاهر_نڪرو(1);
	}
	csum1 = ext2fs_گروهه_desc_csum(fs, 0);
	ext2fs_bg_checksum_سيٽ(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_بلاڪون_ڳڻپ_سيٽ(fs, 0, 1);
	csum2 = ext2fs_گروهه_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	إذا (csum1 == csum2) {
		لکو("checksums for different data shouldn't match\n");
		ٻاهر_نڪرو(1);
	}
	ext2fs_free(fs);

	أرجع 0;
}
#جيڪڏهن_پورو
