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
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "crc16.h"
#ضمن <assert.h>

#که_نه_تعریف_شوی offsetof
#تعریف_کړه offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#د_که_پای

#که_تعریف_شوی DEBUG
#تعریف_کړه STATIC
#وإلا
#تعریف_کړه STATIC ساكن
#د_که_پای

فراغ ext2fs_init_csum_تخم(ext2_filsys fs)
{
	إذا (ext2fs_has_feature_csum_تخم(fs->super))
		fs->csum_تخم = fs->super->s_checksum_تخم;
	وإلا إذا (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_تخم = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 حجم(fs->super->s_uuid));
}

ساكن __u32 ext2fs_mmp_csum(ext2_filsys fs, بنية mmp_struct *mmp)
{
	صحيح دننه_واټن = offsetof(بنية mmp_struct, mmp_checksum);

	أرجع ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)mmp, دننه_واټن);
}

صحيح ext2fs_mmp_csum_verify(ext2_filsys fs, بنية mmp_struct *mmp)
{
	__u32 calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	أرجع ext2fs_le32_منزل_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_سېټ(ext2_filsys fs, بنية mmp_struct *mmp)
{
	__u32 دوراني_ازموینه;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	دوراني_ازموینه = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_منزل_le32(دوراني_ازموینه);

	أرجع 0;
}

صحيح ext2fs_verify_csum_type(ext2_filsys fs, بنية ext2_super_بلاک *sb)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	أرجع sb->s_checksum_type == EXT2_دوراني_ازموینه_32_C_CHKSUM;
}

ساكن __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((ناکارول_شوی)),
				    بنية ext2_super_بلاک *sb)
{
	صحيح دننه_واټن = offsetof(بنية ext2_super_بلاک, s_checksum);

	أرجع ext2fs_crc32c_le(~0, (بینښه حرف *)sb, دننه_واټن);
}

/* NOTE: The input to this function MUST be in LE order */
صحيح ext2fs_superblock_csum_verify(ext2_filsys fs, بنية ext2_super_بلاک *sb)
{
	__u32 flag, calculated;

	إذا (fs->flags & EXT2_FLAG_بدلون_بایټونه)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وإلا
		flag = ext2fs_cpu_منزل_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	إذا (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		أرجع 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	أرجع ext2fs_le32_منزل_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_سېټ(ext2_filsys fs,
				     بنية ext2_super_بلاک *sb)
{
	__u32 flag, دوراني_ازموینه;

	إذا (fs->flags & EXT2_FLAG_بدلون_بایټونه)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وإلا
		flag = ext2fs_cpu_منزل_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	إذا (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		أرجع 0;

	دوراني_ازموینه = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_منزل_le32(دوراني_ازموینه);

	أرجع 0;
}

ساكن errcode_t ext2fs_ext_attr_بلاک_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((ناکارول_شوی)),
					    blk64_t بلاک,
					    بنية ext2_ext_attr_سرلیک *hdr,
					    __u32 *دوراني_ازموینه)
{
	حرف *buf = (حرف *)hdr;
	__u32 old_دوراني_ازموینه = hdr->h_checksum;

	hdr->h_checksum = 0;
	بلاک = ext2fs_cpu_منزل_le64(بلاک);
	*دوراني_ازموینه = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)&بلاک,
				حجم(بلاک));
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)buf, fs->blocksize);
	hdr->h_checksum = old_دوراني_ازموینه;

	أرجع 0;
}

صحيح ext2fs_ext_attr_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t بلاک,
				      بنية ext2_ext_attr_سرلیک *hdr)
{
	__u32 calculated;
	errcode_t retval;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	retval = ext2fs_ext_attr_بلاک_csum(fs, inum, بلاک, hdr, &calculated);
	إذا (retval)
		أرجع 0;

	أرجع ext2fs_le32_منزل_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_بلاک_csum_سېټ(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t بلاک,
					 بنية ext2_ext_attr_سرلیک *hdr)
{
	errcode_t retval;
	__u32 دوراني_ازموینه;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	retval = ext2fs_ext_attr_بلاک_csum(fs, inum, بلاک, hdr, &دوراني_ازموینه);
	إذا (retval)
		أرجع retval;
	hdr->h_checksum = ext2fs_cpu_منزل_le32(دوراني_ازموینه);
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
				     بنية ext2_dir_ثبت *dirent,
				     بنية ext2_dx_countlimit **cc,
				     صحيح *دننه_واټن,
				     صحيح need_swab)
{
	بنية ext2_dir_ثبت *dp;
	بنية ext2_dx_ریښه_info *ریښه;
	بنية ext2_dx_countlimit *c;
	صحيح شمېر_دننه_واټن, max_sane_entries;
	بینښه صحيح rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_منزل_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	إذا (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		شمېر_دننه_واټن = 8;
	وإلا إذا (rec_len == 12) {
		dp = (بنية ext2_dir_ثبت *)(((حرف *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		إذا (rec_len != fs->blocksize - 12)
			أرجع EXT2_ET_DB_NOT_وموندل;
		ریښه = (بنية ext2_dx_ریښه_info *)(((حرف *)dp + 12));
		إذا (ریښه->reserved_zero ||
		    ریښه->info_اوږدتیا != حجم(بنية ext2_dx_ریښه_info))
			أرجع EXT2_ET_DB_NOT_وموندل;
		شمېر_دننه_واټن = 32;
	} وإلا
		أرجع EXT2_ET_DB_NOT_وموندل;

	c = (بنية ext2_dx_countlimit *)(((حرف *)dirent) + شمېر_دننه_واټن);
	max_sane_entries = (fs->blocksize - شمېر_دننه_واټن) /
			   حجم(بنية ext2_dx_ثبت);
	إذا (ext2fs_le16_منزل_cpu(c->حد) > max_sane_entries ||
	    ext2fs_le16_منزل_cpu(c->شمېر) > max_sane_entries)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	إذا (دننه_واټن)
		*دننه_واټن = شمېر_دننه_واټن;
	إذا (cc)
		*cc = c;

	أرجع 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   بنية ext2_dir_ثبت *dirent,
				   بنية ext2_dx_countlimit **cc,
				   صحيح *دننه_واټن)
{
	أرجع __get_dx_countlimit(fs, dirent, cc, دننه_واټن, 0);
}

فراغ ext2fs_initialize_dirent_لکۍ(ext2_filsys fs,
				   بنية ext2_dir_ثبت_لکۍ *t)
{
	memset(t, 0, حجم(بنية ext2_dir_ثبت_لکۍ));
	ext2fs_سېټ_rec_len(fs, حجم(بنية ext2_dir_ثبت_لکۍ),
			   (بنية ext2_dir_ثبت *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ساكن errcode_t __get_dirent_tail(ext2_filsys fs,
				   بنية ext2_dir_ثبت *dirent,
				   بنية ext2_dir_ثبت_لکۍ **tt,
				   صحيح need_swab)
{
	بنية ext2_dir_ثبت *d;
	فراغ *سر;
	بنية ext2_dir_ثبت_لکۍ *t;
	بینښه صحيح rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_منزل_host16 : do_nothing16);

	إذا (fs->blocksize < 1024)
		أرجع EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	سر = EXT2_DIRENT_لکۍ(dirent, fs->blocksize);

	طالما ((فراغ *) d < سر) {
		rec_len = translate(d->rec_len);
		إذا ((rec_len < 8) || (rec_len & 0x03))
			أرجع EXT2_ET_DIR_CORRUPTED;
		d = (بنية ext2_dir_ثبت *)(((حرف *)d) + rec_len);
	}

	إذا ((حرف *)d > ((حرف *)dirent + fs->blocksize))
			أرجع EXT2_ET_DIR_CORRUPTED;
	إذا (d != سر)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (بنية ext2_dir_ثبت_لکۍ *)d;
	إذا (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != حجم(بنية ext2_dir_ثبت_لکۍ) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	إذا (tt)
		*tt = t;
	أرجع retval;
}

صحيح ext2fs_dirent_has_لکۍ(ext2_filsys fs, بنية ext2_dir_ثبت *dirent)
{
	أرجع __get_dirent_tail(fs, dirent, عدم, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ساكن errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_ثبت *dirent, __u32 *دوراني_ازموینه,
				    صحيح اندازه)
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
	*دوراني_ازموینه = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)&inum,
				حجم(inum));
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)&gen, حجم(gen));
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)buf, اندازه);

	أرجع 0;
}

صحيح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      بنية ext2_dir_ثبت *dirent)
{
	errcode_t retval;
	__u32 calculated;
	بنية ext2_dir_ثبت_لکۍ *t;

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

ساكن errcode_t ext2fs_dirent_csum_سېټ(ext2_filsys fs, ext2_ino_t inum,
					بنية ext2_dir_ثبت *dirent)
{
	errcode_t retval;
	__u32 دوراني_ازموینه;
	بنية ext2_dir_ثبت_لکۍ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	إذا (retval)
		أرجع retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &دوراني_ازموینه,
				    (حرف *)t - (حرف *)dirent);
	إذا (retval)
		أرجع retval;
	t->det_checksum = ext2fs_cpu_منزل_le32(دوراني_ازموینه);
	أرجع 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 بنية ext2_dir_ثبت *dirent,
			 __u32 *دوراني_ازموینه, بنية ext2_dx_لکۍ **ret_t)
{
	errcode_t retval;
	حرف *buf = (حرف *)dirent;
	صحيح اندازه;
	__u32 gen, dummy_csum = 0;
	بنية ext2_inode inode;
	بنية ext2_dx_لکۍ *t;
	بنية ext2_dx_countlimit *c;
	صحيح شمېر_دننه_واټن, حد, شمېر;

	retval = __get_dx_countlimit(fs, dirent, &c, &شمېر_دننه_واټن, 1);
	إذا (retval)
		أرجع retval;
	حد = ext2fs_le16_منزل_cpu(c->حد);
	شمېر = ext2fs_le16_منزل_cpu(c->شمېر);
	إذا (شمېر_دننه_واټن + (حد * حجم(بنية ext2_dx_ثبت)) >
	    fs->blocksize - حجم(بنية ext2_dx_لکۍ))
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (بنية ext2_dx_لکۍ *)(((بنية ext2_dx_ثبت *)c) + حد);

	اندازه = شمېر_دننه_واټن + (شمېر * حجم(بنية ext2_dx_ثبت));

	retval = ext2fs_read_inode(fs, inum, &inode);
	إذا (retval)
		أرجع retval;

	inum = ext2fs_cpu_منزل_le32(inum);
	gen = ext2fs_cpu_منزل_le32(inode.i_generation);
	*دوراني_ازموینه = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)&inum,
				حجم(inum));
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)&gen, حجم(gen));
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)buf, اندازه);
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)t, 4);
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)&dummy_csum, 4);

	إذا (ret_t)
		*ret_t = t;
	أرجع 0;
}

ساكن صحيح ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 بنية ext2_dir_ثبت *dirent)
{
	__u32 calculated;
	errcode_t retval;
	بنية ext2_dx_لکۍ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	إذا (retval)
		أرجع 0;

	أرجع ext2fs_le32_منزل_cpu(t->dt_checksum) == calculated;
}

ساكن errcode_t ext2fs_dx_csum_سېټ(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_ثبت *dirent)
{
	__u32 دوراني_ازموینه;
	errcode_t retval = 0;
	بنية ext2_dx_لکۍ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &دوراني_ازموینه, &t);
	إذا (retval)
		أرجع retval;
	t->dt_checksum = ext2fs_cpu_منزل_le32(دوراني_ازموینه);
	أرجع retval;
}

صحيح ext2fs_dir_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 بنية ext2_dir_ثبت *dirent)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	إذا (__get_dirent_tail(fs, dirent, عدم, 1) == 0)
		أرجع ext2fs_dirent_csum_verify(fs, inum, dirent);
	إذا (__get_dx_countlimit(fs, dirent, عدم, عدم, 1) == 0)
		أرجع ext2fs_dx_csum_verify(fs, inum, dirent);

	أرجع 0;
}

errcode_t ext2fs_dir_بلاک_csum_سېټ(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_ثبت *dirent)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	إذا (__get_dirent_tail(fs, dirent, عدم, 1) == 0)
		أرجع ext2fs_dirent_csum_سېټ(fs, inum, dirent);
	إذا (__get_dx_countlimit(fs, dirent, عدم, عدم, 1) == 0)
		أرجع ext2fs_dx_csum_سېټ(fs, inum, dirent);

	إذا (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		أرجع 0;
	أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#تعریف_کړه EXT3_EXTENT_لکۍ_دننه_واټن(hdr)	(حجم(بنية ext3_extent_سرلیک) + \
	(حجم(بنية ext3_extent) * ext2fs_le16_منزل_cpu((hdr)->eh_max)))

ساكن بنية ext3_extent_لکۍ *get_extent_لکۍ(بنية ext3_extent_سرلیک *h)
{
	أرجع (بنية ext3_extent_لکۍ *)(((حرف *)h) +
					   EXT3_EXTENT_لکۍ_دننه_واټن(h));
}

ساكن errcode_t ext2fs_extent_بلاک_csum(ext2_filsys fs, ext2_ino_t inum,
					  بنية ext3_extent_سرلیک *eh,
					  __u32 *دوراني_ازموینه)
{
	صحيح اندازه;
	__u32 gen;
	errcode_t retval;
	بنية ext2_inode inode;

	اندازه = EXT3_EXTENT_لکۍ_دننه_واټن(eh) + offsetof(بنية ext3_extent_لکۍ,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	إذا (retval)
		أرجع retval;
	inum = ext2fs_cpu_منزل_le32(inum);
	gen = ext2fs_cpu_منزل_le32(inode.i_generation);
	*دوراني_ازموینه = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)&inum,
				حجم(inum));
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)&gen, حجم(gen));
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)eh, اندازه);

	أرجع 0;
}

صحيح ext2fs_extent_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext3_extent_سرلیک *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	بنية ext3_extent_لکۍ *t = get_extent_لکۍ(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	provided = ext2fs_le32_منزل_cpu(t->et_checksum);
	retval = ext2fs_extent_بلاک_csum(fs, inum, eh, &calculated);
	إذا (retval)
		أرجع 0;

	أرجع provided == calculated;
}

errcode_t ext2fs_extent_بلاک_csum_سېټ(ext2_filsys fs, ext2_ino_t inum,
				       بنية ext3_extent_سرلیک *eh)
{
	errcode_t retval;
	__u32 دوراني_ازموینه;
	بنية ext3_extent_لکۍ *t = get_extent_لکۍ(eh);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_بلاک_csum(fs, inum, eh, &دوراني_ازموینه);
	إذا (retval)
		أرجع retval;
	t->et_checksum = ext2fs_cpu_منزل_le32(دوراني_ازموینه);
	أرجع retval;
}

صحيح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t ډله,
				    حرف *bitmap, صحيح اندازه)
{
	بنية ext4_ډله_desc *gdp = (بنية ext4_ډله_desc *)
			ext2fs_ډله_desc(fs, fs->ډله_desc, ډله);
	__u32 provided, calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)bitmap,
				      اندازه);
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_پای_ټکی)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	وإلا
		calculated &= 0xFFFF;

	أرجع provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_سېټ(ext2_filsys fs, dgrp_t ډله,
				       حرف *bitmap, صحيح اندازه)
{
	__u32 دوراني_ازموینه;
	بنية ext4_ډله_desc *gdp = (بنية ext4_ډله_desc *)
			ext2fs_ډله_desc(fs, fs->ډله_desc, ډله);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	دوراني_ازموینه = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)bitmap, اندازه);
	gdp->bg_inode_bitmap_csum_lo = دوراني_ازموینه & 0xFFFF;
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_پای_ټکی)
		gdp->bg_inode_bitmap_csum_hi = دوراني_ازموینه >> 16;

	أرجع 0;
}

صحيح ext2fs_بلاک_bitmap_csum_verify(ext2_filsys fs, dgrp_t ډله,
				    حرف *bitmap, صحيح اندازه)
{
	بنية ext4_ډله_desc *gdp = (بنية ext4_ډله_desc *)
			ext2fs_ډله_desc(fs, fs->ډله_desc, ډله);
	__u32 provided, calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	provided = gdp->bg_بلاک_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)bitmap,
				      اندازه);
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_بلاک_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_بلاک_bitmap_csum_hi << 16;
	وإلا
		calculated &= 0xFFFF;

	أرجع provided == calculated;
}

errcode_t ext2fs_بلاک_bitmap_csum_سېټ(ext2_filsys fs, dgrp_t ډله,
				       حرف *bitmap, صحيح اندازه)
{
	__u32 دوراني_ازموینه;
	بنية ext4_ډله_desc *gdp = (بنية ext4_ډله_desc *)
			ext2fs_ډله_desc(fs, fs->ډله_desc, ډله);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	دوراني_ازموینه = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)bitmap, اندازه);
	gdp->bg_بلاک_bitmap_csum_lo = دوراني_ازموینه & 0xFFFF;
	إذا (EXT2_DESC_اندازه(fs->super) >= EXT4_BG_بلاک_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_بلاک_bitmap_csum_hi = دوراني_ازموینه >> 16;

	أرجع 0;
}

ساكن errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       بنية ext2_inode_large *inode,
			       __u32 *دوراني_ازموینه, صحيح has_hi)
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

	inum = ext2fs_cpu_منزل_le32(inum);
	gen = inode->i_generation;
	*دوراني_ازموینه = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)&inum,
				حجم(inum));
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)&gen, حجم(gen));
	*دوراني_ازموینه = ext2fs_crc32c_le(*دوراني_ازموینه, (بینښه حرف *)desc, اندازه);

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
	بینښه صحيح i, has_hi;
	حرف *cp;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	has_hi = (EXT2_INODE_اندازه(fs->super) > EXT2_GOOD_OLD_INODE_اندازه &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_پای_ټکی);

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

errcode_t ext2fs_inode_csum_سېټ(ext2_filsys fs, ext2_ino_t inum,
			   بنية ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 دوراني_ازموینه;
	صحيح has_hi;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	has_hi = (EXT2_INODE_اندازه(fs->super) > EXT2_GOOD_OLD_INODE_اندازه &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_پای_ټکی);

	retval = ext2fs_inode_csum(fs, inum, inode, &دوراني_ازموینه, has_hi);
	إذا (retval)
		أرجع retval;
	inode->i_checksum_lo = ext2fs_cpu_منزل_le16(دوراني_ازموینه & 0xFFFF);
	إذا (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_منزل_le16(دوراني_ازموینه >> 16);
	أرجع 0;
}

__u16 ext2fs_ډله_desc_csum(ext2_filsys fs, dgrp_t ډله)
{
	بنية ext2_ډله_desc *desc = ext2fs_ډله_desc(fs, fs->ډله_desc,
							 ډله);
	size_t دننه_واټن, اندازه = EXT2_DESC_اندازه(fs->super);
	__u16 دوراني_ازموینه = 0;
#که_تعریف_شوی ټکي_BIGENDIAN
	بنية ext4_ډله_desc swabdesc;
	size_t save_اندازه = اندازه;
	ثابت size_t ext4_bg_اندازه = حجم(بنية ext4_ډله_desc);
	بنية ext2_ډله_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	إذا (اندازه > ext4_bg_اندازه)
		اندازه = ext4_bg_اندازه;
	حافظه_نقل(&swabdesc, desc, اندازه);
	ext2fs_بدلون_ډله_desc2(fs, (بنية ext2_ډله_desc *) &swabdesc);
	desc = (بنية ext2_ډله_desc *) &swabdesc;
	ډله = ext2fs_swab32(ډله);
#د_که_پای

	إذا (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_دوراني_ازموینه;
		__u32 دوراني_ازموینه_32;

		old_دوراني_ازموینه = desc->bg_checksum;
		desc->bg_checksum = 0;
		دوراني_ازموینه_32 = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)&ډله,
					 حجم(ډله));
		دوراني_ازموینه_32 = ext2fs_crc32c_le(دوراني_ازموینه_32, (بینښه حرف *)desc,
					 اندازه);
		desc->bg_checksum = old_دوراني_ازموینه;
#که_تعریف_شوی ټکي_BIGENDIAN
		إذا (save_اندازه > ext4_bg_اندازه)
			دوراني_ازموینه_32 = ext2fs_crc32c_le(دوراني_ازموینه_32,
				     (بینښه حرف *)save_desc + ext4_bg_اندازه,
	                             save_اندازه - ext4_bg_اندازه);
#د_که_پای
		دوراني_ازموینه = دوراني_ازموینه_32 & 0xFFFF;
		اذهب بهر;
	}

	/* old crc16 code */
	دننه_واټن = offsetof(بنية ext2_ډله_desc, bg_checksum);
	دوراني_ازموینه = ext2fs_crc16(~0, fs->super->s_uuid,
			   حجم(fs->super->s_uuid));
	دوراني_ازموینه = ext2fs_crc16(دوراني_ازموینه, &ډله, حجم(ډله));
	دوراني_ازموینه = ext2fs_crc16(دوراني_ازموینه, desc, دننه_واټن);
	دننه_واټن += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	إذا (دننه_واټن < اندازه) {
		دوراني_ازموینه = ext2fs_crc16(دوراني_ازموینه, (حرف *)desc + دننه_واټن,
				   اندازه - دننه_واټن);
	}
#که_تعریف_شوی ټکي_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	إذا (save_اندازه > ext4_bg_اندازه)
		دوراني_ازموینه = ext2fs_crc16(دوراني_ازموینه, (حرف *)save_desc + ext4_bg_اندازه,
				   save_اندازه - ext4_bg_اندازه);
#د_که_پای

بهر:
	أرجع دوراني_ازموینه;
}

صحيح ext2fs_ډله_desc_csum_verify(ext2_filsys fs, dgrp_t ډله)
{
	إذا (ext2fs_has_ډله_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, ډله) !=
	     ext2fs_ډله_desc_csum(fs, ډله)))
		أرجع 0;

	أرجع 1;
}

فراغ ext2fs_ډله_desc_csum_سېټ(ext2_filsys fs, dgrp_t ډله)
{
	إذا (!ext2fs_has_ډله_desc_csum(fs))
		أرجع;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_سېټ(fs, ډله, ext2fs_ډله_desc_csum(fs, ډله));
}

ساكن __u32 پیدا_کړه_وروستی_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_فی_grp, dgrp_t grp_no)
{
	ext2_ino_t i, پیل_ino, پای_ټکی_ino;

	پیل_ino = grp_no * inodes_فی_grp + 1;
	پای_ټکی_ino = پیل_ino + inodes_فی_grp - 1;

	لكل (i = پای_ټکی_ino; i >= پیل_ino; i--) {
		إذا (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			أرجع i - پیل_ino + 1;
	}
	أرجع inodes_فی_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_سېټ_gdt_csum(ext2_filsys fs)
{
	بنية ext2_super_بلاک *sb = fs->super;
	صحيح dirty = 0;
	dgrp_t i;

	إذا (!fs->inode_map)
		أرجع EXT2_ET_NO_INODE_BITMAP;

	إذا (!ext2fs_has_ډله_desc_csum(fs))
		أرجع 0;

	لكل (i = 0; i < fs->ډله_desc_شمېر; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_ناکارول_شوی = ext2fs_bg_itable_ناکارول_شوی(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_شمېر = ext2fs_bg_free_inodes_شمېر(fs, i);
		__u32 old_free_بلاکونه_شمېر = ext2fs_bg_free_بلاکونه_شمېر(fs, i);

		إذا (old_free_بلاکونه_شمېر == sb->s_بلاکونه_فی_ډله &&
		    i != fs->ډله_desc_شمېر - 1)
			ext2fs_bg_flags_سېټ(fs, i, EXT2_BG_بلاک_UNINIT);

		إذا (old_free_inodes_شمېر == sb->s_inodes_فی_ډله) {
			ext2fs_bg_flags_سېټ(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_ناکارول_شوی_سېټ(fs, i, sb->s_inodes_فی_ډله);
		} وإلا {
			صحيح ناکارول_شوی =
				sb->s_inodes_فی_ډله -
				پیدا_کړه_وروستی_inode_ingrp(fs->inode_map,
						      sb->s_inodes_فی_ډله, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_ناکارول_شوی_سېټ(fs, i, ناکارول_شوی);
		}

		ext2fs_ډله_desc_csum_سېټ(fs, i);
		إذا (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		إذا (old_ناکارول_شوی != ext2fs_bg_itable_ناکارول_شوی(fs, i))
			dirty = 1;
		إذا (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	إذا (dirty)
		ext2fs_mark_super_dirty(fs);
	أرجع 0;
}

#که_تعریف_شوی DEBUG
#ضمن "e2p/e2p.h"

فراغ print_csum(ثابت حرف *پیغام, ext2_filsys fs, dgrp_t ډله)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	بنية ext2_ډله_desc *desc = ext2fs_ډله_desc(fs, fs->ډله_desc,
							 ډله);
	size_t اندازه = EXT2_DESC_اندازه(fs->super);
	بنية ext2_super_بلاک *sb = fs->super;
	صحيح دننه_واټن = offsetof(بنية ext2_ډله_desc, bg_checksum);
#که_تعریف_شوی ټکي_BIGENDIAN
	بنية ext4_ډله_desc swabdesc;
	بنية ext2_ډله_desc *save_desc = desc;
	ثابت size_t ext4_bg_اندازه = حجم(بنية ext4_ډله_desc);
	size_t save_اندازه = اندازه;
#د_که_پای

#که_تعریف_شوی ټکي_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	إذا (اندازه > ext4_bg_اندازه)
		اندازه = ext4_bg_اندازه;
	حافظه_نقل(&swabdesc, desc, اندازه);
	ext2fs_بدلون_ډله_desc2(fs, (بنية ext2_ډله_desc *) &swabdesc);
	desc = (بنية ext2_ډله_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(ډله);
#وإلا
	swabgroup = ډله;
#د_که_پای

	crc1 = ext2fs_crc16(~0, sb->s_uuid, حجم(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, حجم(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, دننه_واټن);
	دننه_واټن += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	إذا (دننه_واټن < اندازه)
		crc3 = ext2fs_crc16(crc3, (حرف *)desc + دننه_واټن, اندازه - دننه_واټن);
#که_تعریف_شوی ټکي_BIGENDIAN
	إذا (save_اندازه > ext4_bg_اندازه)
		crc3 = ext2fs_crc16(crc3, (حرف *)save_desc + ext4_bg_اندازه,
				    save_اندازه - ext4_bg_اندازه);
#د_که_پای

	چاپ_کړه("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       پیغام, e2p_uuid2str(sb->s_uuid), crc1, ډله, crc2, crc3,
	       ext2fs_ډله_desc_csum(fs, ډله));
}

بینښه حرف sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

صحيح اصلي(صحيح argc, حرف **argv)
{
	بنية ext2_super_بلاک param;
	errcode_t		retval;
	ext2_filsys		fs;
	صحيح			i;
	__u16 csum1, csum2, csum_معلوم = 0xd3a4;

	memset(&param, 0, حجم(param));
	ext2fs_بلاکونه_شمېر_سېټ(&param, 32768);
#إذا 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64_بټ;
	param.s_desc_اندازه = 128;
	csum_معلوم = 0x5b6e;
#د_که_پای

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بټونه, &param,
				   test_io_manager, &fs);
	إذا (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		وتل(1);
	}
	حافظه_نقل(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	لكل (i=0; i < fs->ډله_desc_شمېر; i++) {
		ext2fs_بلاک_bitmap_loc_سېټ(fs, i, 124);
		ext2fs_inode_bitmap_loc_سېټ(fs, i, 125);
		ext2fs_inode_جدول_loc_سېټ(fs, i, 126);
		ext2fs_bg_free_بلاکونه_شمېر_سېټ(fs, i, 31119);
		ext2fs_bg_free_inodes_شمېر_سېټ(fs, i, 15701);
		ext2fs_bg_کارول_شوی_dirs_شمېر_سېټ(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_ډله_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	إذا (csum1 != csum_معلوم) {
		چاپ_کړه("checksum for group 0 should be %04x\n", csum_معلوم);
		وتل(1);
	}
	csum2 = ext2fs_ډله_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	إذا (csum1 == csum2) {
		چاپ_کړه("checksums for different groups shouldn't match\n");
		وتل(1);
	}
	csum2 = ext2fs_ډله_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	إذا (csum1 == csum2) {
		چاپ_کړه("checksums for different groups shouldn't match\n");
		وتل(1);
	}
	ext2fs_bg_checksum_سېټ(fs, 0, csum1);
	csum2 = ext2fs_ډله_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	إذا (csum1 != csum2) {
		چاپ_کړه("checksums should not depend on checksum field\n");
		وتل(1);
	}
	إذا (!ext2fs_ډله_desc_csum_verify(fs, 0)) {
		چاپ_کړه("checksums should verify against gd_checksum\n");
		وتل(1);
	}
	memset(fs->super->s_uuid, 0x30, حجم(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	إذا (ext2fs_ډله_desc_csum_verify(fs, 0) != 0) {
		چاپ_کړه("checksums for different filesystems shouldn't match\n");
		وتل(1);
	}
	csum1 = ext2fs_ډله_desc_csum(fs, 0);
	ext2fs_bg_checksum_سېټ(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_بلاکونه_شمېر_سېټ(fs, 0, 1);
	csum2 = ext2fs_ډله_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	إذا (csum1 == csum2) {
		چاپ_کړه("checksums for different data shouldn't match\n");
		وتل(1);
	}
	ext2fs_free(fs);

	أرجع 0;
}
#د_که_پای
