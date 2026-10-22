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
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "crc16.h"
#ضمن <assert.h>

#اگر_تعریف_نشده offsetof
#تعریف_کن offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#پایان_اگر

#اگر_تعریف_شده DEBUG
#تعریف_کن STATIC
#وإلا
#تعریف_کن STATIC ساكن
#پایان_اگر

فراغ ext2fs_init_csum_دانه(ext2_filsys fs)
{
	إذا (ext2fs_has_feature_csum_دانه(fs->super))
		fs->csum_دانه = fs->super->s_checksum_دانه;
	وإلا إذا (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_دانه = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 حجم(fs->super->s_uuid));
}

ساكن __u32 ext2fs_mmp_csum(ext2_filsys fs, بنية mmp_struct *mmp)
{
	صحيح جابه_جایی = offsetof(بنية mmp_struct, mmp_checksum);

	أرجع ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)mmp, جابه_جایی);
}

صحيح ext2fs_mmp_csum_verify(ext2_filsys fs, بنية mmp_struct *mmp)
{
	__u32 calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	أرجع ext2fs_le32_مقصد_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_مجموعه(ext2_filsys fs, بنية mmp_struct *mmp)
{
	__u32 بررسی_چرخهای;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	بررسی_چرخهای = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_مقصد_le32(بررسی_چرخهای);

	أرجع 0;
}

صحيح ext2fs_verify_csum_type(ext2_filsys fs, بنية ext2_super_بلوک *sb)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	أرجع sb->s_checksum_type == EXT2_بررسی_چرخهای_32_C_CHKSUM;
}

ساكن __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((استفادهنشده)),
				    بنية ext2_super_بلوک *sb)
{
	صحيح جابه_جایی = offsetof(بنية ext2_super_بلوک, s_checksum);

	أرجع ext2fs_crc32c_le(~0, (بیعلامت حرف *)sb, جابه_جایی);
}

/* NOTE: The input to this function MUST be in LE order */
صحيح ext2fs_superblock_csum_verify(ext2_filsys fs, بنية ext2_super_بلوک *sb)
{
	__u32 flag, calculated;

	إذا (fs->flags & EXT2_FLAG_جابجا_بایتها)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وإلا
		flag = ext2fs_cpu_مقصد_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	إذا (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		أرجع 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	أرجع ext2fs_le32_مقصد_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_مجموعه(ext2_filsys fs,
				     بنية ext2_super_بلوک *sb)
{
	__u32 flag, بررسی_چرخهای;

	إذا (fs->flags & EXT2_FLAG_جابجا_بایتها)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وإلا
		flag = ext2fs_cpu_مقصد_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	إذا (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		أرجع 0;

	بررسی_چرخهای = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_مقصد_le32(بررسی_چرخهای);

	أرجع 0;
}

ساكن errcode_t ext2fs_ext_attr_بلوک_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((استفادهنشده)),
					    blk64_t بلوک,
					    بنية ext2_ext_attr_سرآیند *hdr,
					    __u32 *بررسی_چرخهای)
{
	حرف *buf = (حرف *)hdr;
	__u32 old_بررسی_چرخهای = hdr->h_checksum;

	hdr->h_checksum = 0;
	بلوک = ext2fs_cpu_مقصد_le64(بلوک);
	*بررسی_چرخهای = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)&بلوک,
				حجم(بلوک));
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)buf, fs->blocksize);
	hdr->h_checksum = old_بررسی_چرخهای;

	أرجع 0;
}

صحيح ext2fs_ext_attr_بلوک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t بلوک,
				      بنية ext2_ext_attr_سرآیند *hdr)
{
	__u32 calculated;
	errcode_t retval;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	retval = ext2fs_ext_attr_بلوک_csum(fs, inum, بلوک, hdr, &calculated);
	إذا (retval)
		أرجع 0;

	أرجع ext2fs_le32_مقصد_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_بلوک_csum_مجموعه(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t بلوک,
					 بنية ext2_ext_attr_سرآیند *hdr)
{
	errcode_t retval;
	__u32 بررسی_چرخهای;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	retval = ext2fs_ext_attr_بلوک_csum(fs, inum, بلوک, hdr, &بررسی_چرخهای);
	إذا (retval)
		أرجع retval;
	hdr->h_checksum = ext2fs_cpu_مقصد_le32(بررسی_چرخهای);
	أرجع 0;
}

ساكن __u16 do_nothing16(__u16 x)
{
	أرجع x;
}

ساكن __u16 disk_مقصد_host16(__u16 x)
{
	أرجع ext2fs_le16_مقصد_cpu(x);
}

ساكن errcode_t __get_dx_countlimit(ext2_filsys fs,
				     بنية ext2_dir_مدخل *dirent,
				     بنية ext2_dx_countlimit **cc,
				     صحيح *جابه_جایی,
				     صحيح need_swab)
{
	بنية ext2_dir_مدخل *dp;
	بنية ext2_dx_ریشه_info *ریشه;
	بنية ext2_dx_countlimit *c;
	صحيح شمار_جابه_جایی, max_sane_entries;
	بیعلامت صحيح rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_مقصد_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	إذا (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		شمار_جابه_جایی = 8;
	وإلا إذا (rec_len == 12) {
		dp = (بنية ext2_dir_مدخل *)(((حرف *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		إذا (rec_len != fs->blocksize - 12)
			أرجع EXT2_ET_DB_NOT_یافت;
		ریشه = (بنية ext2_dx_ریشه_info *)(((حرف *)dp + 12));
		إذا (ریشه->reserved_zero ||
		    ریشه->info_درازی != حجم(بنية ext2_dx_ریشه_info))
			أرجع EXT2_ET_DB_NOT_یافت;
		شمار_جابه_جایی = 32;
	} وإلا
		أرجع EXT2_ET_DB_NOT_یافت;

	c = (بنية ext2_dx_countlimit *)(((حرف *)dirent) + شمار_جابه_جایی);
	max_sane_entries = (fs->blocksize - شمار_جابه_جایی) /
			   حجم(بنية ext2_dx_مدخل);
	إذا (ext2fs_le16_مقصد_cpu(c->حد) > max_sane_entries ||
	    ext2fs_le16_مقصد_cpu(c->شمار) > max_sane_entries)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	إذا (جابه_جایی)
		*جابه_جایی = شمار_جابه_جایی;
	إذا (cc)
		*cc = c;

	أرجع 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   بنية ext2_dir_مدخل *dirent,
				   بنية ext2_dx_countlimit **cc,
				   صحيح *جابه_جایی)
{
	أرجع __get_dx_countlimit(fs, dirent, cc, جابه_جایی, 0);
}

فراغ ext2fs_initialize_dirent_دم(ext2_filsys fs,
				   بنية ext2_dir_مدخل_دم *t)
{
	memset(t, 0, حجم(بنية ext2_dir_مدخل_دم));
	ext2fs_مجموعه_rec_len(fs, حجم(بنية ext2_dir_مدخل_دم),
			   (بنية ext2_dir_مدخل *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ساكن errcode_t __get_dirent_tail(ext2_filsys fs,
				   بنية ext2_dir_مدخل *dirent,
				   بنية ext2_dir_مدخل_دم **tt,
				   صحيح need_swab)
{
	بنية ext2_dir_مدخل *d;
	فراغ *بالاترین;
	بنية ext2_dir_مدخل_دم *t;
	بیعلامت صحيح rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_مقصد_host16 : do_nothing16);

	إذا (fs->blocksize < 1024)
		أرجع EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	بالاترین = EXT2_DIRENT_دم(dirent, fs->blocksize);

	طالما ((فراغ *) d < بالاترین) {
		rec_len = translate(d->rec_len);
		إذا ((rec_len < 8) || (rec_len & 0x03))
			أرجع EXT2_ET_DIR_CORRUPTED;
		d = (بنية ext2_dir_مدخل *)(((حرف *)d) + rec_len);
	}

	إذا ((حرف *)d > ((حرف *)dirent + fs->blocksize))
			أرجع EXT2_ET_DIR_CORRUPTED;
	إذا (d != بالاترین)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (بنية ext2_dir_مدخل_دم *)d;
	إذا (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != حجم(بنية ext2_dir_مدخل_دم) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	إذا (tt)
		*tt = t;
	أرجع retval;
}

صحيح ext2fs_dirent_has_دم(ext2_filsys fs, بنية ext2_dir_مدخل *dirent)
{
	أرجع __get_dirent_tail(fs, dirent, عدم, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ساكن errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_مدخل *dirent, __u32 *بررسی_چرخهای,
				    صحيح اندازه)
{
	errcode_t retval;
	حرف *buf = (حرف *)dirent;
	__u32 gen;
	بنية ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	إذا (retval)
		أرجع retval;

	inum = ext2fs_cpu_مقصد_le32(inum);
	gen = ext2fs_cpu_مقصد_le32(inode.i_generation);
	*بررسی_چرخهای = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)&inum,
				حجم(inum));
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)&gen, حجم(gen));
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)buf, اندازه);

	أرجع 0;
}

صحيح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      بنية ext2_dir_مدخل *dirent)
{
	errcode_t retval;
	__u32 calculated;
	بنية ext2_dir_مدخل_دم *t;

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
	أرجع ext2fs_le32_مقصد_cpu(t->det_checksum) == calculated;
}

ساكن errcode_t ext2fs_dirent_csum_مجموعه(ext2_filsys fs, ext2_ino_t inum,
					بنية ext2_dir_مدخل *dirent)
{
	errcode_t retval;
	__u32 بررسی_چرخهای;
	بنية ext2_dir_مدخل_دم *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	إذا (retval)
		أرجع retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &بررسی_چرخهای,
				    (حرف *)t - (حرف *)dirent);
	إذا (retval)
		أرجع retval;
	t->det_checksum = ext2fs_cpu_مقصد_le32(بررسی_چرخهای);
	أرجع 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 بنية ext2_dir_مدخل *dirent,
			 __u32 *بررسی_چرخهای, بنية ext2_dx_دم **ret_t)
{
	errcode_t retval;
	حرف *buf = (حرف *)dirent;
	صحيح اندازه;
	__u32 gen, dummy_csum = 0;
	بنية ext2_inode inode;
	بنية ext2_dx_دم *t;
	بنية ext2_dx_countlimit *c;
	صحيح شمار_جابه_جایی, حد, شمار;

	retval = __get_dx_countlimit(fs, dirent, &c, &شمار_جابه_جایی, 1);
	إذا (retval)
		أرجع retval;
	حد = ext2fs_le16_مقصد_cpu(c->حد);
	شمار = ext2fs_le16_مقصد_cpu(c->شمار);
	إذا (شمار_جابه_جایی + (حد * حجم(بنية ext2_dx_مدخل)) >
	    fs->blocksize - حجم(بنية ext2_dx_دم))
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (بنية ext2_dx_دم *)(((بنية ext2_dx_مدخل *)c) + حد);

	اندازه = شمار_جابه_جایی + (شمار * حجم(بنية ext2_dx_مدخل));

	retval = ext2fs_read_inode(fs, inum, &inode);
	إذا (retval)
		أرجع retval;

	inum = ext2fs_cpu_مقصد_le32(inum);
	gen = ext2fs_cpu_مقصد_le32(inode.i_generation);
	*بررسی_چرخهای = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)&inum,
				حجم(inum));
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)&gen, حجم(gen));
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)buf, اندازه);
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)t, 4);
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)&dummy_csum, 4);

	إذا (ret_t)
		*ret_t = t;
	أرجع 0;
}

ساكن صحيح ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 بنية ext2_dir_مدخل *dirent)
{
	__u32 calculated;
	errcode_t retval;
	بنية ext2_dx_دم *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	إذا (retval)
		أرجع 0;

	أرجع ext2fs_le32_مقصد_cpu(t->dt_checksum) == calculated;
}

ساكن errcode_t ext2fs_dx_csum_مجموعه(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_مدخل *dirent)
{
	__u32 بررسی_چرخهای;
	errcode_t retval = 0;
	بنية ext2_dx_دم *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &بررسی_چرخهای, &t);
	إذا (retval)
		أرجع retval;
	t->dt_checksum = ext2fs_cpu_مقصد_le32(بررسی_چرخهای);
	أرجع retval;
}

صحيح ext2fs_dir_بلوک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 بنية ext2_dir_مدخل *dirent)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	إذا (__get_dirent_tail(fs, dirent, عدم, 1) == 0)
		أرجع ext2fs_dirent_csum_verify(fs, inum, dirent);
	إذا (__get_dx_countlimit(fs, dirent, عدم, عدم, 1) == 0)
		أرجع ext2fs_dx_csum_verify(fs, inum, dirent);

	أرجع 0;
}

errcode_t ext2fs_dir_بلوک_csum_مجموعه(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_مدخل *dirent)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	إذا (__get_dirent_tail(fs, dirent, عدم, 1) == 0)
		أرجع ext2fs_dirent_csum_مجموعه(fs, inum, dirent);
	إذا (__get_dx_countlimit(fs, dirent, عدم, عدم, 1) == 0)
		أرجع ext2fs_dx_csum_مجموعه(fs, inum, dirent);

	إذا (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		أرجع 0;
	أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#تعریف_کن EXT3_EXTENT_دم_جابه_جایی(hdr)	(حجم(بنية ext3_extent_سرآیند) + \
	(حجم(بنية ext3_extent) * ext2fs_le16_مقصد_cpu((hdr)->eh_max)))

ساكن بنية ext3_extent_دم *get_extent_دم(بنية ext3_extent_سرآیند *h)
{
	أرجع (بنية ext3_extent_دم *)(((حرف *)h) +
					   EXT3_EXTENT_دم_جابه_جایی(h));
}

ساكن errcode_t ext2fs_extent_بلوک_csum(ext2_filsys fs, ext2_ino_t inum,
					  بنية ext3_extent_سرآیند *eh,
					  __u32 *بررسی_چرخهای)
{
	صحيح اندازه;
	__u32 gen;
	errcode_t retval;
	بنية ext2_inode inode;

	اندازه = EXT3_EXTENT_دم_جابه_جایی(eh) + offsetof(بنية ext3_extent_دم,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	إذا (retval)
		أرجع retval;
	inum = ext2fs_cpu_مقصد_le32(inum);
	gen = ext2fs_cpu_مقصد_le32(inode.i_generation);
	*بررسی_چرخهای = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)&inum,
				حجم(inum));
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)&gen, حجم(gen));
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)eh, اندازه);

	أرجع 0;
}

صحيح ext2fs_extent_بلوک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext3_extent_سرآیند *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	بنية ext3_extent_دم *t = get_extent_دم(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	provided = ext2fs_le32_مقصد_cpu(t->et_checksum);
	retval = ext2fs_extent_بلوک_csum(fs, inum, eh, &calculated);
	إذا (retval)
		أرجع 0;

	أرجع provided == calculated;
}

errcode_t ext2fs_extent_بلوک_csum_مجموعه(ext2_filsys fs, ext2_ino_t inum,
				       بنية ext3_extent_سرآیند *eh)
{
	errcode_t retval;
	__u32 بررسی_چرخهای;
	بنية ext3_extent_دم *t = get_extent_دم(eh);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_بلوک_csum(fs, inum, eh, &بررسی_چرخهای);
	إذا (retval)
		أرجع retval;
	t->et_checksum = ext2fs_cpu_مقصد_le32(بررسی_چرخهای);
	أرجع retval;
}

صحيح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروه,
				    حرف *bitmap, صحيح اندازه)
{
	بنية ext4_گروه_desc *gdp = (بنية ext4_گروه_desc *)
			ext2fs_گروه_desc(fs, fs->گروه_desc, گروه);
	__u32 provided, calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)bitmap,
				      اندازه);
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_انتها)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	وإلا
		calculated &= 0xFFFF;

	أرجع provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_مجموعه(ext2_filsys fs, dgrp_t گروه,
				       حرف *bitmap, صحيح اندازه)
{
	__u32 بررسی_چرخهای;
	بنية ext4_گروه_desc *gdp = (بنية ext4_گروه_desc *)
			ext2fs_گروه_desc(fs, fs->گروه_desc, گروه);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	بررسی_چرخهای = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)bitmap, اندازه);
	gdp->bg_inode_bitmap_csum_lo = بررسی_چرخهای & 0xFFFF;
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_انتها)
		gdp->bg_inode_bitmap_csum_hi = بررسی_چرخهای >> 16;

	أرجع 0;
}

صحيح ext2fs_بلوک_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروه,
				    حرف *bitmap, صحيح اندازه)
{
	بنية ext4_گروه_desc *gdp = (بنية ext4_گروه_desc *)
			ext2fs_گروه_desc(fs, fs->گروه_desc, گروه);
	__u32 provided, calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	provided = gdp->bg_بلوک_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)bitmap,
				      اندازه);
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_بلوک_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_بلوک_bitmap_csum_hi << 16;
	وإلا
		calculated &= 0xFFFF;

	أرجع provided == calculated;
}

errcode_t ext2fs_بلوک_bitmap_csum_مجموعه(ext2_filsys fs, dgrp_t گروه,
				       حرف *bitmap, صحيح اندازه)
{
	__u32 بررسی_چرخهای;
	بنية ext4_گروه_desc *gdp = (بنية ext4_گروه_desc *)
			ext2fs_گروه_desc(fs, fs->گروه_desc, گروه);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	بررسی_چرخهای = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)bitmap, اندازه);
	gdp->bg_بلوک_bitmap_csum_lo = بررسی_چرخهای & 0xFFFF;
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_بلوک_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_بلوک_bitmap_csum_hi = بررسی_چرخهای >> 16;

	أرجع 0;
}

ساكن errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       بنية ext2_inode_large *inode,
			       __u32 *بررسی_چرخهای, صحيح has_hi)
{
	__u32 gen;
	بنية ext2_inode_large *desc = inode;
	size_t اندازه = EXT2_INODE_اندازه(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	إذا (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_مقصد_le32(inum);
	gen = inode->i_generation;
	*بررسی_چرخهای = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)&inum,
				حجم(inum));
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)&gen, حجم(gen));
	*بررسی_چرخهای = ext2fs_crc32c_le(*بررسی_چرخهای, (بیعلامت حرف *)desc, اندازه);

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
	بیعلامت صحيح i, has_hi;
	حرف *cp;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	has_hi = (EXT2_INODE_اندازه(fs->super) > EXT2_GOOD_OLD_INODE_اندازه &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_انتها);

	provided = ext2fs_le16_مقصد_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	إذا (retval)
		أرجع 0;
	إذا (has_hi) {
		__u32 hi = ext2fs_le16_مقصد_cpu(inode->i_checksum_hi);
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

errcode_t ext2fs_inode_csum_مجموعه(ext2_filsys fs, ext2_ino_t inum,
			   بنية ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 بررسی_چرخهای;
	صحيح has_hi;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	has_hi = (EXT2_INODE_اندازه(fs->super) > EXT2_GOOD_OLD_INODE_اندازه &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_انتها);

	retval = ext2fs_inode_csum(fs, inum, inode, &بررسی_چرخهای, has_hi);
	إذا (retval)
		أرجع retval;
	inode->i_checksum_lo = ext2fs_cpu_مقصد_le16(بررسی_چرخهای & 0xFFFF);
	إذا (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_مقصد_le16(بررسی_چرخهای >> 16);
	أرجع 0;
}

__u16 ext2fs_گروه_desc_csum(ext2_filsys fs, dgrp_t گروه)
{
	بنية ext2_گروه_desc *desc = ext2fs_گروه_desc(fs, fs->گروه_desc,
							 گروه);
	size_t جابه_جایی, اندازه = EXT2_DESC_اندازه(fs->super);
	__u16 بررسی_چرخهای = 0;
#اگر_تعریف_شده واژگان_BIGENDIAN
	بنية ext4_گروه_desc swabdesc;
	size_t save_اندازه = اندازه;
	ثابت size_t ext4_bg_اندازه = حجم(بنية ext4_گروه_desc);
	بنية ext2_گروه_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	إذا (اندازه > ext4_bg_اندازه)
		اندازه = ext4_bg_اندازه;
	کپی_حافظه(&swabdesc, desc, اندازه);
	ext2fs_جابجا_گروه_desc2(fs, (بنية ext2_گروه_desc *) &swabdesc);
	desc = (بنية ext2_گروه_desc *) &swabdesc;
	گروه = ext2fs_swab32(گروه);
#پایان_اگر

	إذا (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_بررسی_چرخهای;
		__u32 بررسی_چرخهای_32;

		old_بررسی_چرخهای = desc->bg_checksum;
		desc->bg_checksum = 0;
		بررسی_چرخهای_32 = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)&گروه,
					 حجم(گروه));
		بررسی_چرخهای_32 = ext2fs_crc32c_le(بررسی_چرخهای_32, (بیعلامت حرف *)desc,
					 اندازه);
		desc->bg_checksum = old_بررسی_چرخهای;
#اگر_تعریف_شده واژگان_BIGENDIAN
		إذا (save_اندازه > ext4_bg_اندازه)
			بررسی_چرخهای_32 = ext2fs_crc32c_le(بررسی_چرخهای_32,
				     (بیعلامت حرف *)save_desc + ext4_bg_اندازه,
	                             save_اندازه - ext4_bg_اندازه);
#پایان_اگر
		بررسی_چرخهای = بررسی_چرخهای_32 & 0xFFFF;
		اذهب بیرون;
	}

	/* old crc16 code */
	جابه_جایی = offsetof(بنية ext2_گروه_desc, bg_checksum);
	بررسی_چرخهای = ext2fs_crc16(~0, fs->super->s_uuid,
			   حجم(fs->super->s_uuid));
	بررسی_چرخهای = ext2fs_crc16(بررسی_چرخهای, &گروه, حجم(گروه));
	بررسی_چرخهای = ext2fs_crc16(بررسی_چرخهای, desc, جابه_جایی);
	جابه_جایی += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	إذا (جابه_جایی < اندازه) {
		بررسی_چرخهای = ext2fs_crc16(بررسی_چرخهای, (حرف *)desc + جابه_جایی,
				   اندازه - جابه_جایی);
	}
#اگر_تعریف_شده واژگان_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	إذا (save_اندازه > ext4_bg_اندازه)
		بررسی_چرخهای = ext2fs_crc16(بررسی_چرخهای, (حرف *)save_desc + ext4_bg_اندازه,
				   save_اندازه - ext4_bg_اندازه);
#پایان_اگر

بیرون:
	أرجع بررسی_چرخهای;
}

صحيح ext2fs_گروه_desc_csum_verify(ext2_filsys fs, dgrp_t گروه)
{
	إذا (ext2fs_has_گروه_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, گروه) !=
	     ext2fs_گروه_desc_csum(fs, گروه)))
		أرجع 0;

	أرجع 1;
}

فراغ ext2fs_گروه_desc_csum_مجموعه(ext2_filsys fs, dgrp_t گروه)
{
	إذا (!ext2fs_has_گروه_desc_csum(fs))
		أرجع;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_مجموعه(fs, گروه, ext2fs_گروه_desc_csum(fs, گروه));
}

ساكن __u32 پیدا_کن_آخرین_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_در_هر_grp, dgrp_t grp_no)
{
	ext2_ino_t i, شروع_ino, انتها_ino;

	شروع_ino = grp_no * inodes_در_هر_grp + 1;
	انتها_ino = شروع_ino + inodes_در_هر_grp - 1;

	لكل (i = انتها_ino; i >= شروع_ino; i--) {
		إذا (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			أرجع i - شروع_ino + 1;
	}
	أرجع inodes_در_هر_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_مجموعه_gdt_csum(ext2_filsys fs)
{
	بنية ext2_super_بلوک *sb = fs->super;
	صحيح dirty = 0;
	dgrp_t i;

	إذا (!fs->inode_map)
		أرجع EXT2_ET_NO_INODE_BITMAP;

	إذا (!ext2fs_has_گروه_desc_csum(fs))
		أرجع 0;

	لكل (i = 0; i < fs->گروه_desc_شمار; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_استفادهنشده = ext2fs_bg_itable_استفادهنشده(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_شمار = ext2fs_bg_free_inodes_شمار(fs, i);
		__u32 old_free_بلوکها_شمار = ext2fs_bg_free_بلوکها_شمار(fs, i);

		إذا (old_free_بلوکها_شمار == sb->s_بلوکها_در_هر_گروه &&
		    i != fs->گروه_desc_شمار - 1)
			ext2fs_bg_flags_مجموعه(fs, i, EXT2_BG_بلوک_UNINIT);

		إذا (old_free_inodes_شمار == sb->s_inodes_در_هر_گروه) {
			ext2fs_bg_flags_مجموعه(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_استفادهنشده_مجموعه(fs, i, sb->s_inodes_در_هر_گروه);
		} وإلا {
			صحيح استفادهنشده =
				sb->s_inodes_در_هر_گروه -
				پیدا_کن_آخرین_inode_ingrp(fs->inode_map,
						      sb->s_inodes_در_هر_گروه, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_استفادهنشده_مجموعه(fs, i, استفادهنشده);
		}

		ext2fs_گروه_desc_csum_مجموعه(fs, i);
		إذا (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		إذا (old_استفادهنشده != ext2fs_bg_itable_استفادهنشده(fs, i))
			dirty = 1;
		إذا (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	إذا (dirty)
		ext2fs_mark_super_dirty(fs);
	أرجع 0;
}

#اگر_تعریف_شده DEBUG
#ضمن "e2p/e2p.h"

فراغ print_csum(ثابت حرف *پیام, ext2_filsys fs, dgrp_t گروه)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	بنية ext2_گروه_desc *desc = ext2fs_گروه_desc(fs, fs->گروه_desc,
							 گروه);
	size_t اندازه = EXT2_DESC_اندازه(fs->super);
	بنية ext2_super_بلوک *sb = fs->super;
	صحيح جابه_جایی = offsetof(بنية ext2_گروه_desc, bg_checksum);
#اگر_تعریف_شده واژگان_BIGENDIAN
	بنية ext4_گروه_desc swabdesc;
	بنية ext2_گروه_desc *save_desc = desc;
	ثابت size_t ext4_bg_اندازه = حجم(بنية ext4_گروه_desc);
	size_t save_اندازه = اندازه;
#پایان_اگر

#اگر_تعریف_شده واژگان_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	إذا (اندازه > ext4_bg_اندازه)
		اندازه = ext4_bg_اندازه;
	کپی_حافظه(&swabdesc, desc, اندازه);
	ext2fs_جابجا_گروه_desc2(fs, (بنية ext2_گروه_desc *) &swabdesc);
	desc = (بنية ext2_گروه_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(گروه);
#وإلا
	swabgroup = گروه;
#پایان_اگر

	crc1 = ext2fs_crc16(~0, sb->s_uuid, حجم(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, حجم(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, جابه_جایی);
	جابه_جایی += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	إذا (جابه_جایی < اندازه)
		crc3 = ext2fs_crc16(crc3, (حرف *)desc + جابه_جایی, اندازه - جابه_جایی);
#اگر_تعریف_شده واژگان_BIGENDIAN
	إذا (save_اندازه > ext4_bg_اندازه)
		crc3 = ext2fs_crc16(crc3, (حرف *)save_desc + ext4_bg_اندازه,
				    save_اندازه - ext4_bg_اندازه);
#پایان_اگر

	چاپ_کن("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       پیام, e2p_uuid2str(sb->s_uuid), crc1, گروه, crc2, crc3,
	       ext2fs_گروه_desc_csum(fs, گروه));
}

بیعلامت حرف sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

صحيح اصلی(صحيح argc, حرف **argv)
{
	بنية ext2_super_بلوک param;
	errcode_t		retval;
	ext2_filsys		fs;
	صحيح			i;
	__u16 csum1, csum2, csum_معلوم = 0xd3a4;

	memset(&param, 0, حجم(param));
	ext2fs_بلوکها_شمار_مجموعه(&param, 32768);
#إذا 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64_بیت;
	param.s_desc_اندازه = 128;
	csum_معلوم = 0x5b6e;
#پایان_اگر

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بیتها, &param,
				   test_io_manager, &fs);
	إذا (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		خروج(1);
	}
	کپی_حافظه(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	لكل (i=0; i < fs->گروه_desc_شمار; i++) {
		ext2fs_بلوک_bitmap_loc_مجموعه(fs, i, 124);
		ext2fs_inode_bitmap_loc_مجموعه(fs, i, 125);
		ext2fs_inode_جدول_loc_مجموعه(fs, i, 126);
		ext2fs_bg_free_بلوکها_شمار_مجموعه(fs, i, 31119);
		ext2fs_bg_free_inodes_شمار_مجموعه(fs, i, 15701);
		ext2fs_bg_استفاده_شده_dirs_شمار_مجموعه(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_گروه_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	إذا (csum1 != csum_معلوم) {
		چاپ_کن("checksum for group 0 should be %04x\n", csum_معلوم);
		خروج(1);
	}
	csum2 = ext2fs_گروه_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	إذا (csum1 == csum2) {
		چاپ_کن("checksums for different groups shouldn't match\n");
		خروج(1);
	}
	csum2 = ext2fs_گروه_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	إذا (csum1 == csum2) {
		چاپ_کن("checksums for different groups shouldn't match\n");
		خروج(1);
	}
	ext2fs_bg_checksum_مجموعه(fs, 0, csum1);
	csum2 = ext2fs_گروه_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	إذا (csum1 != csum2) {
		چاپ_کن("checksums should not depend on checksum field\n");
		خروج(1);
	}
	إذا (!ext2fs_گروه_desc_csum_verify(fs, 0)) {
		چاپ_کن("checksums should verify against gd_checksum\n");
		خروج(1);
	}
	memset(fs->super->s_uuid, 0x30, حجم(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	إذا (ext2fs_گروه_desc_csum_verify(fs, 0) != 0) {
		چاپ_کن("checksums for different filesystems shouldn't match\n");
		خروج(1);
	}
	csum1 = ext2fs_گروه_desc_csum(fs, 0);
	ext2fs_bg_checksum_مجموعه(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_بلوکها_شمار_مجموعه(fs, 0, 1);
	csum2 = ext2fs_گروه_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	إذا (csum1 == csum2) {
		چاپ_کن("checksums for different data shouldn't match\n");
		خروج(1);
	}
	ext2fs_free(fs);

	أرجع 0;
}
#پایان_اگر
