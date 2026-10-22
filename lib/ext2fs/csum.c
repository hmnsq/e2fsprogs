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
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "crc16.h"
#ضمن <assert.h>

#جے_بنا_تعریف offsetof
#تعریف_کرو offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#جے_انت

#جے_تعریف_شدہ DEBUG
#تعریف_کرو STATIC
#وإلا
#تعریف_کرو STATIC ساكن
#جے_انت

فراغ ext2fs_init_csum_بیج(ext2_filsys fs)
{
	إذا (ext2fs_has_feature_csum_بیج(fs->super))
		fs->csum_بیج = fs->super->s_checksum_بیج;
	وإلا إذا (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_بیج = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 حجم(fs->super->s_uuid));
}

ساكن __u32 ext2fs_mmp_csum(ext2_filsys fs, بنية mmp_struct *mmp)
{
	صحيح وتھ = offsetof(بنية mmp_struct, mmp_checksum);

	أرجع ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)mmp, وتھ);
}

صحيح ext2fs_mmp_csum_verify(ext2_filsys fs, بنية mmp_struct *mmp)
{
	__u32 calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	أرجع ext2fs_le32_منزل_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_سیٹ(ext2_filsys fs, بنية mmp_struct *mmp)
{
	__u32 چکری_جانچ;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	چکری_جانچ = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_منزل_le32(چکری_جانچ);

	أرجع 0;
}

صحيح ext2fs_verify_csum_type(ext2_filsys fs, بنية ext2_super_بلاک *sb)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	أرجع sb->s_checksum_type == EXT2_چکری_جانچ_32_C_CHKSUM;
}

ساكن __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((بن_ورتیا)),
				    بنية ext2_super_بلاک *sb)
{
	صحيح وتھ = offsetof(بنية ext2_super_بلاک, s_checksum);

	أرجع ext2fs_crc32c_le(~0, (بنانشانی حرف *)sb, وتھ);
}

/* NOTE: The input to this function MUST be in LE order */
صحيح ext2fs_superblock_csum_verify(ext2_filsys fs, بنية ext2_super_بلاک *sb)
{
	__u32 flag, calculated;

	إذا (fs->flags & EXT2_FLAG_وٹاندرا_بائٹاں)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وإلا
		flag = ext2fs_cpu_منزل_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	إذا (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		أرجع 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	أرجع ext2fs_le32_منزل_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_سیٹ(ext2_filsys fs,
				     بنية ext2_super_بلاک *sb)
{
	__u32 flag, چکری_جانچ;

	إذا (fs->flags & EXT2_FLAG_وٹاندرا_بائٹاں)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	وإلا
		flag = ext2fs_cpu_منزل_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	إذا (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		أرجع 0;

	چکری_جانچ = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_منزل_le32(چکری_جانچ);

	أرجع 0;
}

ساكن errcode_t ext2fs_ext_attr_بلاک_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((بن_ورتیا)),
					    blk64_t بلاک,
					    بنية ext2_ext_attr_سرخی *hdr,
					    __u32 *چکری_جانچ)
{
	حرف *buf = (حرف *)hdr;
	__u32 old_چکری_جانچ = hdr->h_checksum;

	hdr->h_checksum = 0;
	بلاک = ext2fs_cpu_منزل_le64(بلاک);
	*چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)&بلاک,
				حجم(بلاک));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)buf, fs->blocksize);
	hdr->h_checksum = old_چکری_جانچ;

	أرجع 0;
}

صحيح ext2fs_ext_attr_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t بلاک,
				      بنية ext2_ext_attr_سرخی *hdr)
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

errcode_t ext2fs_ext_attr_بلاک_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t بلاک,
					 بنية ext2_ext_attr_سرخی *hdr)
{
	errcode_t retval;
	__u32 چکری_جانچ;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	retval = ext2fs_ext_attr_بلاک_csum(fs, inum, بلاک, hdr, &چکری_جانچ);
	إذا (retval)
		أرجع retval;
	hdr->h_checksum = ext2fs_cpu_منزل_le32(چکری_جانچ);
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
				     بنية ext2_dir_اندراج *dirent,
				     بنية ext2_dx_countlimit **cc,
				     صحيح *وتھ,
				     صحيح need_swab)
{
	بنية ext2_dir_اندراج *dp;
	بنية ext2_dx_جڑ_info *جڑ;
	بنية ext2_dx_countlimit *c;
	صحيح گنتی_وتھ, max_sane_entries;
	بنانشانی صحيح rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_منزل_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	إذا (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		گنتی_وتھ = 8;
	وإلا إذا (rec_len == 12) {
		dp = (بنية ext2_dir_اندراج *)(((حرف *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		إذا (rec_len != fs->blocksize - 12)
			أرجع EXT2_ET_DB_NOT_لبھیا;
		جڑ = (بنية ext2_dx_جڑ_info *)(((حرف *)dp + 12));
		إذا (جڑ->reserved_zero ||
		    جڑ->info_لمائی != حجم(بنية ext2_dx_جڑ_info))
			أرجع EXT2_ET_DB_NOT_لبھیا;
		گنتی_وتھ = 32;
	} وإلا
		أرجع EXT2_ET_DB_NOT_لبھیا;

	c = (بنية ext2_dx_countlimit *)(((حرف *)dirent) + گنتی_وتھ);
	max_sane_entries = (fs->blocksize - گنتی_وتھ) /
			   حجم(بنية ext2_dx_اندراج);
	إذا (ext2fs_le16_منزل_cpu(c->حد_بندی) > max_sane_entries ||
	    ext2fs_le16_منزل_cpu(c->گنتی) > max_sane_entries)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	إذا (وتھ)
		*وتھ = گنتی_وتھ;
	إذا (cc)
		*cc = c;

	أرجع 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   بنية ext2_dir_اندراج *dirent,
				   بنية ext2_dx_countlimit **cc,
				   صحيح *وتھ)
{
	أرجع __get_dx_countlimit(fs, dirent, cc, وتھ, 0);
}

فراغ ext2fs_initialize_dirent_پوچھ(ext2_filsys fs,
				   بنية ext2_dir_اندراج_پوچھ *t)
{
	memset(t, 0, حجم(بنية ext2_dir_اندراج_پوچھ));
	ext2fs_سیٹ_rec_len(fs, حجم(بنية ext2_dir_اندراج_پوچھ),
			   (بنية ext2_dir_اندراج *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ساكن errcode_t __get_dirent_tail(ext2_filsys fs,
				   بنية ext2_dir_اندراج *dirent,
				   بنية ext2_dir_اندراج_پوچھ **tt,
				   صحيح need_swab)
{
	بنية ext2_dir_اندراج *d;
	فراغ *چوٹی;
	بنية ext2_dir_اندراج_پوچھ *t;
	بنانشانی صحيح rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_منزل_host16 : do_nothing16);

	إذا (fs->blocksize < 1024)
		أرجع EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	چوٹی = EXT2_DIRENT_پوچھ(dirent, fs->blocksize);

	طالما ((فراغ *) d < چوٹی) {
		rec_len = translate(d->rec_len);
		إذا ((rec_len < 8) || (rec_len & 0x03))
			أرجع EXT2_ET_DIR_CORRUPTED;
		d = (بنية ext2_dir_اندراج *)(((حرف *)d) + rec_len);
	}

	إذا ((حرف *)d > ((حرف *)dirent + fs->blocksize))
			أرجع EXT2_ET_DIR_CORRUPTED;
	إذا (d != چوٹی)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (بنية ext2_dir_اندراج_پوچھ *)d;
	إذا (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != حجم(بنية ext2_dir_اندراج_پوچھ) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	إذا (tt)
		*tt = t;
	أرجع retval;
}

صحيح ext2fs_dirent_has_پوچھ(ext2_filsys fs, بنية ext2_dir_اندراج *dirent)
{
	أرجع __get_dirent_tail(fs, dirent, عدم, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ساكن errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_اندراج *dirent, __u32 *چکری_جانچ,
				    صحيح ناپ)
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
	*چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)&inum,
				حجم(inum));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)&gen, حجم(gen));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)buf, ناپ);

	أرجع 0;
}

صحيح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      بنية ext2_dir_اندراج *dirent)
{
	errcode_t retval;
	__u32 calculated;
	بنية ext2_dir_اندراج_پوچھ *t;

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

ساكن errcode_t ext2fs_dirent_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
					بنية ext2_dir_اندراج *dirent)
{
	errcode_t retval;
	__u32 چکری_جانچ;
	بنية ext2_dir_اندراج_پوچھ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	إذا (retval)
		أرجع retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &چکری_جانچ,
				    (حرف *)t - (حرف *)dirent);
	إذا (retval)
		أرجع retval;
	t->det_checksum = ext2fs_cpu_منزل_le32(چکری_جانچ);
	أرجع 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 بنية ext2_dir_اندراج *dirent,
			 __u32 *چکری_جانچ, بنية ext2_dx_پوچھ **ret_t)
{
	errcode_t retval;
	حرف *buf = (حرف *)dirent;
	صحيح ناپ;
	__u32 gen, dummy_csum = 0;
	بنية ext2_inode inode;
	بنية ext2_dx_پوچھ *t;
	بنية ext2_dx_countlimit *c;
	صحيح گنتی_وتھ, حد_بندی, گنتی;

	retval = __get_dx_countlimit(fs, dirent, &c, &گنتی_وتھ, 1);
	إذا (retval)
		أرجع retval;
	حد_بندی = ext2fs_le16_منزل_cpu(c->حد_بندی);
	گنتی = ext2fs_le16_منزل_cpu(c->گنتی);
	إذا (گنتی_وتھ + (حد_بندی * حجم(بنية ext2_dx_اندراج)) >
	    fs->blocksize - حجم(بنية ext2_dx_پوچھ))
		أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (بنية ext2_dx_پوچھ *)(((بنية ext2_dx_اندراج *)c) + حد_بندی);

	ناپ = گنتی_وتھ + (گنتی * حجم(بنية ext2_dx_اندراج));

	retval = ext2fs_read_inode(fs, inum, &inode);
	إذا (retval)
		أرجع retval;

	inum = ext2fs_cpu_منزل_le32(inum);
	gen = ext2fs_cpu_منزل_le32(inode.i_generation);
	*چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)&inum,
				حجم(inum));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)&gen, حجم(gen));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)buf, ناپ);
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)t, 4);
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)&dummy_csum, 4);

	إذا (ret_t)
		*ret_t = t;
	أرجع 0;
}

ساكن صحيح ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 بنية ext2_dir_اندراج *dirent)
{
	__u32 calculated;
	errcode_t retval;
	بنية ext2_dx_پوچھ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	إذا (retval)
		أرجع 0;

	أرجع ext2fs_le32_منزل_cpu(t->dt_checksum) == calculated;
}

ساكن errcode_t ext2fs_dx_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_اندراج *dirent)
{
	__u32 چکری_جانچ;
	errcode_t retval = 0;
	بنية ext2_dx_پوچھ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &چکری_جانچ, &t);
	إذا (retval)
		أرجع retval;
	t->dt_checksum = ext2fs_cpu_منزل_le32(چکری_جانچ);
	أرجع retval;
}

صحيح ext2fs_dir_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 بنية ext2_dir_اندراج *dirent)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	إذا (__get_dirent_tail(fs, dirent, عدم, 1) == 0)
		أرجع ext2fs_dirent_csum_verify(fs, inum, dirent);
	إذا (__get_dx_countlimit(fs, dirent, عدم, عدم, 1) == 0)
		أرجع ext2fs_dx_csum_verify(fs, inum, dirent);

	أرجع 0;
}

errcode_t ext2fs_dir_بلاک_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext2_dir_اندراج *dirent)
{
	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	إذا (__get_dirent_tail(fs, dirent, عدم, 1) == 0)
		أرجع ext2fs_dirent_csum_سیٹ(fs, inum, dirent);
	إذا (__get_dx_countlimit(fs, dirent, عدم, عدم, 1) == 0)
		أرجع ext2fs_dx_csum_سیٹ(fs, inum, dirent);

	إذا (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		أرجع 0;
	أرجع EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#تعریف_کرو EXT3_EXTENT_پوچھ_وتھ(hdr)	(حجم(بنية ext3_extent_سرخی) + \
	(حجم(بنية ext3_extent) * ext2fs_le16_منزل_cpu((hdr)->eh_max)))

ساكن بنية ext3_extent_پوچھ *get_extent_پوچھ(بنية ext3_extent_سرخی *h)
{
	أرجع (بنية ext3_extent_پوچھ *)(((حرف *)h) +
					   EXT3_EXTENT_پوچھ_وتھ(h));
}

ساكن errcode_t ext2fs_extent_بلاک_csum(ext2_filsys fs, ext2_ino_t inum,
					  بنية ext3_extent_سرخی *eh,
					  __u32 *چکری_جانچ)
{
	صحيح ناپ;
	__u32 gen;
	errcode_t retval;
	بنية ext2_inode inode;

	ناپ = EXT3_EXTENT_پوچھ_وتھ(eh) + offsetof(بنية ext3_extent_پوچھ,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	إذا (retval)
		أرجع retval;
	inum = ext2fs_cpu_منزل_le32(inum);
	gen = ext2fs_cpu_منزل_le32(inode.i_generation);
	*چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)&inum,
				حجم(inum));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)&gen, حجم(gen));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)eh, ناپ);

	أرجع 0;
}

صحيح ext2fs_extent_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    بنية ext3_extent_سرخی *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	بنية ext3_extent_پوچھ *t = get_extent_پوچھ(eh);

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

errcode_t ext2fs_extent_بلاک_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
				       بنية ext3_extent_سرخی *eh)
{
	errcode_t retval;
	__u32 چکری_جانچ;
	بنية ext3_extent_پوچھ *t = get_extent_پوچھ(eh);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_بلاک_csum(fs, inum, eh, &چکری_جانچ);
	إذا (retval)
		أرجع retval;
	t->et_checksum = ext2fs_cpu_منزل_le32(چکری_جانچ);
	أرجع retval;
}

صحيح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروہ,
				    حرف *bitmap, صحيح ناپ)
{
	بنية ext4_گروہ_desc *gdp = (بنية ext4_گروہ_desc *)
			ext2fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	__u32 provided, calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)bitmap,
				      ناپ);
	إذا (EXT2_DESC_ناپ(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_سرا)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	وإلا
		calculated &= 0xFFFF;

	أرجع provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ,
				       حرف *bitmap, صحيح ناپ)
{
	__u32 چکری_جانچ;
	بنية ext4_گروہ_desc *gdp = (بنية ext4_گروہ_desc *)
			ext2fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)bitmap, ناپ);
	gdp->bg_inode_bitmap_csum_lo = چکری_جانچ & 0xFFFF;
	إذا (EXT2_DESC_ناپ(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_سرا)
		gdp->bg_inode_bitmap_csum_hi = چکری_جانچ >> 16;

	أرجع 0;
}

صحيح ext2fs_بلاک_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروہ,
				    حرف *bitmap, صحيح ناپ)
{
	بنية ext4_گروہ_desc *gdp = (بنية ext4_گروہ_desc *)
			ext2fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	__u32 provided, calculated;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	provided = gdp->bg_بلاک_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)bitmap,
				      ناپ);
	إذا (EXT2_DESC_ناپ(fs->super) >= EXT4_BG_بلاک_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_بلاک_bitmap_csum_hi << 16;
	وإلا
		calculated &= 0xFFFF;

	أرجع provided == calculated;
}

errcode_t ext2fs_بلاک_bitmap_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ,
				       حرف *bitmap, صحيح ناپ)
{
	__u32 چکری_جانچ;
	بنية ext4_گروہ_desc *gdp = (بنية ext4_گروہ_desc *)
			ext2fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)bitmap, ناپ);
	gdp->bg_بلاک_bitmap_csum_lo = چکری_جانچ & 0xFFFF;
	إذا (EXT2_DESC_ناپ(fs->super) >= EXT4_BG_بلاک_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_بلاک_bitmap_csum_hi = چکری_جانچ >> 16;

	أرجع 0;
}

ساكن errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       بنية ext2_inode_large *inode,
			       __u32 *چکری_جانچ, صحيح has_hi)
{
	__u32 gen;
	بنية ext2_inode_large *desc = inode;
	size_t ناپ = EXT2_INODE_ناپ(fs->super);
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
	*چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)&inum,
				حجم(inum));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)&gen, حجم(gen));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (بنانشانی حرف *)desc, ناپ);

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
	بنانشانی صحيح i, has_hi;
	حرف *cp;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;

	has_hi = (EXT2_INODE_ناپ(fs->super) > EXT2_GOOD_OLD_INODE_ناپ &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_سرا);

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

errcode_t ext2fs_inode_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
			   بنية ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 چکری_جانچ;
	صحيح has_hi;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	has_hi = (EXT2_INODE_ناپ(fs->super) > EXT2_GOOD_OLD_INODE_ناپ &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_سرا);

	retval = ext2fs_inode_csum(fs, inum, inode, &چکری_جانچ, has_hi);
	إذا (retval)
		أرجع retval;
	inode->i_checksum_lo = ext2fs_cpu_منزل_le16(چکری_جانچ & 0xFFFF);
	إذا (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_منزل_le16(چکری_جانچ >> 16);
	أرجع 0;
}

__u16 ext2fs_گروہ_desc_csum(ext2_filsys fs, dgrp_t گروہ)
{
	بنية ext2_گروہ_desc *desc = ext2fs_گروہ_desc(fs, fs->گروہ_desc,
							 گروہ);
	size_t وتھ, ناپ = EXT2_DESC_ناپ(fs->super);
	__u16 چکری_جانچ = 0;
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	بنية ext4_گروہ_desc swabdesc;
	size_t save_ناپ = ناپ;
	ثابت size_t ext4_bg_ناپ = حجم(بنية ext4_گروہ_desc);
	بنية ext2_گروہ_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	إذا (ناپ > ext4_bg_ناپ)
		ناپ = ext4_bg_ناپ;
	حافظہ_نقل(&swabdesc, desc, ناپ);
	ext2fs_وٹاندرا_گروہ_desc2(fs, (بنية ext2_گروہ_desc *) &swabdesc);
	desc = (بنية ext2_گروہ_desc *) &swabdesc;
	گروہ = ext2fs_swab32(گروہ);
#جے_انت

	إذا (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_چکری_جانچ;
		__u32 چکری_جانچ_32;

		old_چکری_جانچ = desc->bg_checksum;
		desc->bg_checksum = 0;
		چکری_جانچ_32 = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)&گروہ,
					 حجم(گروہ));
		چکری_جانچ_32 = ext2fs_crc32c_le(چکری_جانچ_32, (بنانشانی حرف *)desc,
					 ناپ);
		desc->bg_checksum = old_چکری_جانچ;
#جے_تعریف_شدہ الفاظ_BIGENDIAN
		إذا (save_ناپ > ext4_bg_ناپ)
			چکری_جانچ_32 = ext2fs_crc32c_le(چکری_جانچ_32,
				     (بنانشانی حرف *)save_desc + ext4_bg_ناپ,
	                             save_ناپ - ext4_bg_ناپ);
#جے_انت
		چکری_جانچ = چکری_جانچ_32 & 0xFFFF;
		اذهب باہرلا;
	}

	/* old crc16 code */
	وتھ = offsetof(بنية ext2_گروہ_desc, bg_checksum);
	چکری_جانچ = ext2fs_crc16(~0, fs->super->s_uuid,
			   حجم(fs->super->s_uuid));
	چکری_جانچ = ext2fs_crc16(چکری_جانچ, &گروہ, حجم(گروہ));
	چکری_جانچ = ext2fs_crc16(چکری_جانچ, desc, وتھ);
	وتھ += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	إذا (وتھ < ناپ) {
		چکری_جانچ = ext2fs_crc16(چکری_جانچ, (حرف *)desc + وتھ,
				   ناپ - وتھ);
	}
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	إذا (save_ناپ > ext4_bg_ناپ)
		چکری_جانچ = ext2fs_crc16(چکری_جانچ, (حرف *)save_desc + ext4_bg_ناپ,
				   save_ناپ - ext4_bg_ناپ);
#جے_انت

باہرلا:
	أرجع چکری_جانچ;
}

صحيح ext2fs_گروہ_desc_csum_verify(ext2_filsys fs, dgrp_t گروہ)
{
	إذا (ext2fs_has_گروہ_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, گروہ) !=
	     ext2fs_گروہ_desc_csum(fs, گروہ)))
		أرجع 0;

	أرجع 1;
}

فراغ ext2fs_گروہ_desc_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ)
{
	إذا (!ext2fs_has_گروہ_desc_csum(fs))
		أرجع;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_سیٹ(fs, گروہ, ext2fs_گروہ_desc_csum(fs, گروہ));
}

ساكن __u32 لبھو_آخری_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_فی_grp, dgrp_t grp_no)
{
	ext2_ino_t i, شروع_ino, سرا_ino;

	شروع_ino = grp_no * inodes_فی_grp + 1;
	سرا_ino = شروع_ino + inodes_فی_grp - 1;

	لكل (i = سرا_ino; i >= شروع_ino; i--) {
		إذا (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			أرجع i - شروع_ino + 1;
	}
	أرجع inodes_فی_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_سیٹ_gdt_csum(ext2_filsys fs)
{
	بنية ext2_super_بلاک *sb = fs->super;
	صحيح dirty = 0;
	dgrp_t i;

	إذا (!fs->inode_map)
		أرجع EXT2_ET_NO_INODE_BITMAP;

	إذا (!ext2fs_has_گروہ_desc_csum(fs))
		أرجع 0;

	لكل (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_بن_ورتیا = ext2fs_bg_itable_بن_ورتیا(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_گنتی = ext2fs_bg_free_inodes_گنتی(fs, i);
		__u32 old_free_بلاکاں_گنتی = ext2fs_bg_free_بلاکاں_گنتی(fs, i);

		إذا (old_free_بلاکاں_گنتی == sb->s_بلاکاں_فی_گروہ &&
		    i != fs->گروہ_desc_گنتی - 1)
			ext2fs_bg_flags_سیٹ(fs, i, EXT2_BG_بلاک_UNINIT);

		إذا (old_free_inodes_گنتی == sb->s_inodes_فی_گروہ) {
			ext2fs_bg_flags_سیٹ(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_بن_ورتیا_سیٹ(fs, i, sb->s_inodes_فی_گروہ);
		} وإلا {
			صحيح بن_ورتیا =
				sb->s_inodes_فی_گروہ -
				لبھو_آخری_inode_ingrp(fs->inode_map,
						      sb->s_inodes_فی_گروہ, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_بن_ورتیا_سیٹ(fs, i, بن_ورتیا);
		}

		ext2fs_گروہ_desc_csum_سیٹ(fs, i);
		إذا (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		إذا (old_بن_ورتیا != ext2fs_bg_itable_بن_ورتیا(fs, i))
			dirty = 1;
		إذا (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	إذا (dirty)
		ext2fs_mark_super_dirty(fs);
	أرجع 0;
}

#جے_تعریف_شدہ DEBUG
#ضمن "e2p/e2p.h"

فراغ print_csum(ثابت حرف *پیغام, ext2_filsys fs, dgrp_t گروہ)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	بنية ext2_گروہ_desc *desc = ext2fs_گروہ_desc(fs, fs->گروہ_desc,
							 گروہ);
	size_t ناپ = EXT2_DESC_ناپ(fs->super);
	بنية ext2_super_بلاک *sb = fs->super;
	صحيح وتھ = offsetof(بنية ext2_گروہ_desc, bg_checksum);
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	بنية ext4_گروہ_desc swabdesc;
	بنية ext2_گروہ_desc *save_desc = desc;
	ثابت size_t ext4_bg_ناپ = حجم(بنية ext4_گروہ_desc);
	size_t save_ناپ = ناپ;
#جے_انت

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	إذا (ناپ > ext4_bg_ناپ)
		ناپ = ext4_bg_ناپ;
	حافظہ_نقل(&swabdesc, desc, ناپ);
	ext2fs_وٹاندرا_گروہ_desc2(fs, (بنية ext2_گروہ_desc *) &swabdesc);
	desc = (بنية ext2_گروہ_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(گروہ);
#وإلا
	swabgroup = گروہ;
#جے_انت

	crc1 = ext2fs_crc16(~0, sb->s_uuid, حجم(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, حجم(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, وتھ);
	وتھ += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	إذا (وتھ < ناپ)
		crc3 = ext2fs_crc16(crc3, (حرف *)desc + وتھ, ناپ - وتھ);
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	إذا (save_ناپ > ext4_bg_ناپ)
		crc3 = ext2fs_crc16(crc3, (حرف *)save_desc + ext4_bg_ناپ,
				    save_ناپ - ext4_bg_ناپ);
#جے_انت

	لکھو("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       پیغام, e2p_uuid2str(sb->s_uuid), crc1, گروہ, crc2, crc3,
	       ext2fs_گروہ_desc_csum(fs, گروہ));
}

بنانشانی حرف sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

صحيح مکھ(صحيح argc, حرف **argv)
{
	بنية ext2_super_بلاک param;
	errcode_t		retval;
	ext2_filsys		fs;
	صحيح			i;
	__u16 csum1, csum2, csum_معلوم = 0xd3a4;

	memset(&param, 0, حجم(param));
	ext2fs_بلاکاں_گنتی_سیٹ(&param, 32768);
#إذا 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64_ثنائی_ہندسہ;
	param.s_desc_ناپ = 128;
	csum_معلوم = 0x5b6e;
#جے_انت

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بٹ, &param,
				   test_io_manager, &fs);
	إذا (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		باہر_نکلو(1);
	}
	حافظہ_نقل(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	لكل (i=0; i < fs->گروہ_desc_گنتی; i++) {
		ext2fs_بلاک_bitmap_loc_سیٹ(fs, i, 124);
		ext2fs_inode_bitmap_loc_سیٹ(fs, i, 125);
		ext2fs_inode_جدول_loc_سیٹ(fs, i, 126);
		ext2fs_bg_free_بلاکاں_گنتی_سیٹ(fs, i, 31119);
		ext2fs_bg_free_inodes_گنتی_سیٹ(fs, i, 15701);
		ext2fs_bg_ورتیا_dirs_گنتی_سیٹ(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_گروہ_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	إذا (csum1 != csum_معلوم) {
		لکھو("checksum for group 0 should be %04x\n", csum_معلوم);
		باہر_نکلو(1);
	}
	csum2 = ext2fs_گروہ_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	إذا (csum1 == csum2) {
		لکھو("checksums for different groups shouldn't match\n");
		باہر_نکلو(1);
	}
	csum2 = ext2fs_گروہ_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	إذا (csum1 == csum2) {
		لکھو("checksums for different groups shouldn't match\n");
		باہر_نکلو(1);
	}
	ext2fs_bg_checksum_سیٹ(fs, 0, csum1);
	csum2 = ext2fs_گروہ_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	إذا (csum1 != csum2) {
		لکھو("checksums should not depend on checksum field\n");
		باہر_نکلو(1);
	}
	إذا (!ext2fs_گروہ_desc_csum_verify(fs, 0)) {
		لکھو("checksums should verify against gd_checksum\n");
		باہر_نکلو(1);
	}
	memset(fs->super->s_uuid, 0x30, حجم(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	إذا (ext2fs_گروہ_desc_csum_verify(fs, 0) != 0) {
		لکھو("checksums for different filesystems shouldn't match\n");
		باہر_نکلو(1);
	}
	csum1 = ext2fs_گروہ_desc_csum(fs, 0);
	ext2fs_bg_checksum_سیٹ(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_بلاکاں_گنتی_سیٹ(fs, 0, 1);
	csum2 = ext2fs_گروہ_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	إذا (csum1 == csum2) {
		لکھو("checksums for different data shouldn't match\n");
		باہر_نکلو(1);
	}
	ext2fs_free(fs);

	أرجع 0;
}
#جے_انت
