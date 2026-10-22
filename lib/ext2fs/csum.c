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

#شامل "config.h"
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "crc16.h"
#شامل <assert.h>

#اگر_غیر_تعریف_شدہ offsetof
#تعریف_کرو offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#اگر_ختم

#اگر_تعریف_شدہ DEBUG
#تعریف_کرو STATIC
#ورنہ
#تعریف_کرو STATIC ساکن
#اگر_ختم

خالی ext2fs_init_csum_بیج(ext2_filsys fs)
{
	اگر (ext2fs_has_feature_csum_بیج(fs->super))
		fs->csum_بیج = fs->super->s_checksum_بیج;
	ورنہ اگر (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_بیج = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 حجم(fs->super->s_uuid));
}

ساکن __u32 ext2fs_mmp_csum(ext2_filsys fs, ساخت mmp_struct *mmp)
{
	صحیح انحراف = offsetof(ساخت mmp_struct, mmp_checksum);

	واپس ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)mmp, انحراف);
}

صحیح ext2fs_mmp_csum_verify(ext2_filsys fs, ساخت mmp_struct *mmp)
{
	__u32 calculated;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	واپس ext2fs_le32_مقصد_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_سیٹ(ext2_filsys fs, ساخت mmp_struct *mmp)
{
	__u32 چکری_جانچ;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 0;

	چکری_جانچ = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_مقصد_le32(چکری_جانچ);

	واپس 0;
}

صحیح ext2fs_verify_csum_type(ext2_filsys fs, ساخت ext2_super_بلاک *sb)
{
	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 1;

	واپس sb->s_checksum_type == EXT2_چکری_جانچ_32_C_CHKSUM;
}

ساکن __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((غیر_مستعمل)),
				    ساخت ext2_super_بلاک *sb)
{
	صحیح انحراف = offsetof(ساخت ext2_super_بلاک, s_checksum);

	واپس ext2fs_crc32c_le(~0, (غیرعلامتی حرف *)sb, انحراف);
}

/* NOTE: The input to this function MUST be in LE order */
صحیح ext2fs_superblock_csum_verify(ext2_filsys fs, ساخت ext2_super_بلاک *sb)
{
	__u32 flag, calculated;

	اگر (fs->flags & EXT2_FLAG_تبادلہ_بائٹیں)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ورنہ
		flag = ext2fs_cpu_مقصد_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	اگر (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		واپس 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	واپس ext2fs_le32_مقصد_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_سیٹ(ext2_filsys fs,
				     ساخت ext2_super_بلاک *sb)
{
	__u32 flag, چکری_جانچ;

	اگر (fs->flags & EXT2_FLAG_تبادلہ_بائٹیں)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	ورنہ
		flag = ext2fs_cpu_مقصد_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	اگر (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		واپس 0;

	چکری_جانچ = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_مقصد_le32(چکری_جانچ);

	واپس 0;
}

ساکن errcode_t ext2fs_ext_attr_بلاک_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((غیر_مستعمل)),
					    blk64_t بلاک,
					    ساخت ext2_ext_attr_سرخی *hdr,
					    __u32 *چکری_جانچ)
{
	حرف *buf = (حرف *)hdr;
	__u32 old_چکری_جانچ = hdr->h_checksum;

	hdr->h_checksum = 0;
	بلاک = ext2fs_cpu_مقصد_le64(بلاک);
	*چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)&بلاک,
				حجم(بلاک));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)buf, fs->blocksize);
	hdr->h_checksum = old_چکری_جانچ;

	واپس 0;
}

صحیح ext2fs_ext_attr_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t بلاک,
				      ساخت ext2_ext_attr_سرخی *hdr)
{
	__u32 calculated;
	errcode_t retval;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 1;

	retval = ext2fs_ext_attr_بلاک_csum(fs, inum, بلاک, hdr, &calculated);
	اگر (retval)
		واپس 0;

	واپس ext2fs_le32_مقصد_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_بلاک_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t بلاک,
					 ساخت ext2_ext_attr_سرخی *hdr)
{
	errcode_t retval;
	__u32 چکری_جانچ;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 0;

	retval = ext2fs_ext_attr_بلاک_csum(fs, inum, بلاک, hdr, &چکری_جانچ);
	اگر (retval)
		واپس retval;
	hdr->h_checksum = ext2fs_cpu_مقصد_le32(چکری_جانچ);
	واپس 0;
}

ساکن __u16 do_nothing16(__u16 x)
{
	واپس x;
}

ساکن __u16 disk_مقصد_host16(__u16 x)
{
	واپس ext2fs_le16_مقصد_cpu(x);
}

ساکن errcode_t __get_dx_countlimit(ext2_filsys fs,
				     ساخت ext2_dir_اندراج *dirent,
				     ساخت ext2_dx_countlimit **cc,
				     صحیح *انحراف,
				     صحیح need_swab)
{
	ساخت ext2_dir_اندراج *dp;
	ساخت ext2_dx_جڑ_info *جڑ;
	ساخت ext2_dx_countlimit *c;
	صحیح گنتی_انحراف, max_sane_entries;
	غیرعلامتی صحیح rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_مقصد_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	اگر (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		گنتی_انحراف = 8;
	ورنہ اگر (rec_len == 12) {
		dp = (ساخت ext2_dir_اندراج *)(((حرف *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		اگر (rec_len != fs->blocksize - 12)
			واپس EXT2_ET_DB_NOT_ملا;
		جڑ = (ساخت ext2_dx_جڑ_info *)(((حرف *)dp + 12));
		اگر (جڑ->reserved_zero ||
		    جڑ->info_لمبائی != حجم(ساخت ext2_dx_جڑ_info))
			واپس EXT2_ET_DB_NOT_ملا;
		گنتی_انحراف = 32;
	} ورنہ
		واپس EXT2_ET_DB_NOT_ملا;

	c = (ساخت ext2_dx_countlimit *)(((حرف *)dirent) + گنتی_انحراف);
	max_sane_entries = (fs->blocksize - گنتی_انحراف) /
			   حجم(ساخت ext2_dx_اندراج);
	اگر (ext2fs_le16_مقصد_cpu(c->حد_بندی) > max_sane_entries ||
	    ext2fs_le16_مقصد_cpu(c->گنتی) > max_sane_entries)
		واپس EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	اگر (انحراف)
		*انحراف = گنتی_انحراف;
	اگر (cc)
		*cc = c;

	واپس 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   ساخت ext2_dir_اندراج *dirent,
				   ساخت ext2_dx_countlimit **cc,
				   صحیح *انحراف)
{
	واپس __get_dx_countlimit(fs, dirent, cc, انحراف, 0);
}

خالی ext2fs_initialize_dirent_دم(ext2_filsys fs,
				   ساخت ext2_dir_اندراج_دم *t)
{
	memset(t, 0, حجم(ساخت ext2_dir_اندراج_دم));
	ext2fs_سیٹ_rec_len(fs, حجم(ساخت ext2_dir_اندراج_دم),
			   (ساخت ext2_dir_اندراج *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

ساکن errcode_t __get_dirent_tail(ext2_filsys fs,
				   ساخت ext2_dir_اندراج *dirent,
				   ساخت ext2_dir_اندراج_دم **tt,
				   صحیح need_swab)
{
	ساخت ext2_dir_اندراج *d;
	خالی *چوٹی;
	ساخت ext2_dir_اندراج_دم *t;
	غیرعلامتی صحیح rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_مقصد_host16 : do_nothing16);

	اگر (fs->blocksize < 1024)
		واپس EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	چوٹی = EXT2_DIRENT_دم(dirent, fs->blocksize);

	جب ((خالی *) d < چوٹی) {
		rec_len = translate(d->rec_len);
		اگر ((rec_len < 8) || (rec_len & 0x03))
			واپس EXT2_ET_DIR_CORRUPTED;
		d = (ساخت ext2_dir_اندراج *)(((حرف *)d) + rec_len);
	}

	اگر ((حرف *)d > ((حرف *)dirent + fs->blocksize))
			واپس EXT2_ET_DIR_CORRUPTED;
	اگر (d != چوٹی)
		واپس EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (ساخت ext2_dir_اندراج_دم *)d;
	اگر (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != حجم(ساخت ext2_dir_اندراج_دم) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		واپس EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	اگر (tt)
		*tt = t;
	واپس retval;
}

صحیح ext2fs_dirent_has_دم(ext2_filsys fs, ساخت ext2_dir_اندراج *dirent)
{
	واپس __get_dirent_tail(fs, dirent, تہی, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

ساکن errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    ساخت ext2_dir_اندراج *dirent, __u32 *چکری_جانچ,
				    صحیح سائز)
{
	errcode_t retval;
	حرف *buf = (حرف *)dirent;
	__u32 gen;
	ساخت ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	اگر (retval)
		واپس retval;

	inum = ext2fs_cpu_مقصد_le32(inum);
	gen = ext2fs_cpu_مقصد_le32(inode.i_generation);
	*چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)&inum,
				حجم(inum));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)&gen, حجم(gen));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)buf, سائز);

	واپس 0;
}

صحیح ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      ساخت ext2_dir_اندراج *dirent)
{
	errcode_t retval;
	__u32 calculated;
	ساخت ext2_dir_اندراج_دم *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	اگر (retval)
		واپس 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (حرف *)t - (حرف *)dirent);
	اگر (retval)
		واپس 0;
	واپس ext2fs_le32_مقصد_cpu(t->det_checksum) == calculated;
}

ساکن errcode_t ext2fs_dirent_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
					ساخت ext2_dir_اندراج *dirent)
{
	errcode_t retval;
	__u32 چکری_جانچ;
	ساخت ext2_dir_اندراج_دم *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	اگر (retval)
		واپس retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &چکری_جانچ,
				    (حرف *)t - (حرف *)dirent);
	اگر (retval)
		واپس retval;
	t->det_checksum = ext2fs_cpu_مقصد_le32(چکری_جانچ);
	واپس 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 ساخت ext2_dir_اندراج *dirent,
			 __u32 *چکری_جانچ, ساخت ext2_dx_دم **ret_t)
{
	errcode_t retval;
	حرف *buf = (حرف *)dirent;
	صحیح سائز;
	__u32 gen, dummy_csum = 0;
	ساخت ext2_inode inode;
	ساخت ext2_dx_دم *t;
	ساخت ext2_dx_countlimit *c;
	صحیح گنتی_انحراف, حد_بندی, گنتی;

	retval = __get_dx_countlimit(fs, dirent, &c, &گنتی_انحراف, 1);
	اگر (retval)
		واپس retval;
	حد_بندی = ext2fs_le16_مقصد_cpu(c->حد_بندی);
	گنتی = ext2fs_le16_مقصد_cpu(c->گنتی);
	اگر (گنتی_انحراف + (حد_بندی * حجم(ساخت ext2_dx_اندراج)) >
	    fs->blocksize - حجم(ساخت ext2_dx_دم))
		واپس EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (ساخت ext2_dx_دم *)(((ساخت ext2_dx_اندراج *)c) + حد_بندی);

	سائز = گنتی_انحراف + (گنتی * حجم(ساخت ext2_dx_اندراج));

	retval = ext2fs_read_inode(fs, inum, &inode);
	اگر (retval)
		واپس retval;

	inum = ext2fs_cpu_مقصد_le32(inum);
	gen = ext2fs_cpu_مقصد_le32(inode.i_generation);
	*چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)&inum,
				حجم(inum));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)&gen, حجم(gen));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)buf, سائز);
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)t, 4);
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)&dummy_csum, 4);

	اگر (ret_t)
		*ret_t = t;
	واپس 0;
}

ساکن صحیح ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ساخت ext2_dir_اندراج *dirent)
{
	__u32 calculated;
	errcode_t retval;
	ساخت ext2_dx_دم *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	اگر (retval)
		واپس 0;

	واپس ext2fs_le32_مقصد_cpu(t->dt_checksum) == calculated;
}

ساکن errcode_t ext2fs_dx_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
				    ساخت ext2_dir_اندراج *dirent)
{
	__u32 چکری_جانچ;
	errcode_t retval = 0;
	ساخت ext2_dx_دم *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &چکری_جانچ, &t);
	اگر (retval)
		واپس retval;
	t->dt_checksum = ext2fs_cpu_مقصد_le32(چکری_جانچ);
	واپس retval;
}

صحیح ext2fs_dir_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 ساخت ext2_dir_اندراج *dirent)
{
	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 1;

	اگر (__get_dirent_tail(fs, dirent, تہی, 1) == 0)
		واپس ext2fs_dirent_csum_verify(fs, inum, dirent);
	اگر (__get_dx_countlimit(fs, dirent, تہی, تہی, 1) == 0)
		واپس ext2fs_dx_csum_verify(fs, inum, dirent);

	واپس 0;
}

errcode_t ext2fs_dir_بلاک_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
				    ساخت ext2_dir_اندراج *dirent)
{
	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 0;

	اگر (__get_dirent_tail(fs, dirent, تہی, 1) == 0)
		واپس ext2fs_dirent_csum_سیٹ(fs, inum, dirent);
	اگر (__get_dx_countlimit(fs, dirent, تہی, تہی, 1) == 0)
		واپس ext2fs_dx_csum_سیٹ(fs, inum, dirent);

	اگر (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		واپس 0;
	واپس EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#تعریف_کرو EXT3_EXTENT_دم_انحراف(hdr)	(حجم(ساخت ext3_extent_سرخی) + \
	(حجم(ساخت ext3_extent) * ext2fs_le16_مقصد_cpu((hdr)->eh_max)))

ساکن ساخت ext3_extent_دم *get_extent_دم(ساخت ext3_extent_سرخی *h)
{
	واپس (ساخت ext3_extent_دم *)(((حرف *)h) +
					   EXT3_EXTENT_دم_انحراف(h));
}

ساکن errcode_t ext2fs_extent_بلاک_csum(ext2_filsys fs, ext2_ino_t inum,
					  ساخت ext3_extent_سرخی *eh,
					  __u32 *چکری_جانچ)
{
	صحیح سائز;
	__u32 gen;
	errcode_t retval;
	ساخت ext2_inode inode;

	سائز = EXT3_EXTENT_دم_انحراف(eh) + offsetof(ساخت ext3_extent_دم,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	اگر (retval)
		واپس retval;
	inum = ext2fs_cpu_مقصد_le32(inum);
	gen = ext2fs_cpu_مقصد_le32(inode.i_generation);
	*چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)&inum,
				حجم(inum));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)&gen, حجم(gen));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)eh, سائز);

	واپس 0;
}

صحیح ext2fs_extent_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    ساخت ext3_extent_سرخی *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	ساخت ext3_extent_دم *t = get_extent_دم(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 1;

	provided = ext2fs_le32_مقصد_cpu(t->et_checksum);
	retval = ext2fs_extent_بلاک_csum(fs, inum, eh, &calculated);
	اگر (retval)
		واپس 0;

	واپس provided == calculated;
}

errcode_t ext2fs_extent_بلاک_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
				       ساخت ext3_extent_سرخی *eh)
{
	errcode_t retval;
	__u32 چکری_جانچ;
	ساخت ext3_extent_دم *t = get_extent_دم(eh);

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_بلاک_csum(fs, inum, eh, &چکری_جانچ);
	اگر (retval)
		واپس retval;
	t->et_checksum = ext2fs_cpu_مقصد_le32(چکری_جانچ);
	واپس retval;
}

صحیح ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروہ,
				    حرف *bitmap, صحیح سائز)
{
	ساخت ext4_گروہ_desc *gdp = (ساخت ext4_گروہ_desc *)
			ext2fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	__u32 provided, calculated;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)bitmap,
				      سائز);
	اگر (EXT2_DESC_سائز(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_انتہا)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	ورنہ
		calculated &= 0xFFFF;

	واپس provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ,
				       حرف *bitmap, صحیح سائز)
{
	__u32 چکری_جانچ;
	ساخت ext4_گروہ_desc *gdp = (ساخت ext4_گروہ_desc *)
			ext2fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 0;

	چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)bitmap, سائز);
	gdp->bg_inode_bitmap_csum_lo = چکری_جانچ & 0xFFFF;
	اگر (EXT2_DESC_سائز(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_انتہا)
		gdp->bg_inode_bitmap_csum_hi = چکری_جانچ >> 16;

	واپس 0;
}

صحیح ext2fs_بلاک_bitmap_csum_verify(ext2_filsys fs, dgrp_t گروہ,
				    حرف *bitmap, صحیح سائز)
{
	ساخت ext4_گروہ_desc *gdp = (ساخت ext4_گروہ_desc *)
			ext2fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);
	__u32 provided, calculated;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 1;
	provided = gdp->bg_بلاک_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)bitmap,
				      سائز);
	اگر (EXT2_DESC_سائز(fs->super) >= EXT4_BG_بلاک_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_بلاک_bitmap_csum_hi << 16;
	ورنہ
		calculated &= 0xFFFF;

	واپس provided == calculated;
}

errcode_t ext2fs_بلاک_bitmap_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ,
				       حرف *bitmap, صحیح سائز)
{
	__u32 چکری_جانچ;
	ساخت ext4_گروہ_desc *gdp = (ساخت ext4_گروہ_desc *)
			ext2fs_گروہ_desc(fs, fs->گروہ_desc, گروہ);

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 0;

	چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)bitmap, سائز);
	gdp->bg_بلاک_bitmap_csum_lo = چکری_جانچ & 0xFFFF;
	اگر (EXT2_DESC_سائز(fs->super) >= EXT4_BG_بلاک_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_بلاک_bitmap_csum_hi = چکری_جانچ >> 16;

	واپس 0;
}

ساکن errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       ساخت ext2_inode_large *inode,
			       __u32 *چکری_جانچ, صحیح has_hi)
{
	__u32 gen;
	ساخت ext2_inode_large *desc = inode;
	size_t سائز = EXT2_INODE_سائز(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	اگر (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_مقصد_le32(inum);
	gen = inode->i_generation;
	*چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)&inum,
				حجم(inum));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)&gen, حجم(gen));
	*چکری_جانچ = ext2fs_crc32c_le(*چکری_جانچ, (غیرعلامتی حرف *)desc, سائز);

	inode->i_checksum_lo = old_lo;
	اگر (has_hi)
		inode->i_checksum_hi = old_hi;
	واپس 0;
}

صحیح ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     ساخت ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	غیرعلامتی صحیح i, has_hi;
	حرف *cp;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 1;

	has_hi = (EXT2_INODE_سائز(fs->super) > EXT2_GOOD_OLD_INODE_سائز &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_انتہا);

	provided = ext2fs_le16_مقصد_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	اگر (retval)
		واپس 0;
	اگر (has_hi) {
		__u32 hi = ext2fs_le16_مقصد_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} ورنہ
		calculated &= 0xFFFF;

	اگر (provided == calculated)
		واپس 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	برائے (cp = (حرف *) inode, i = 0;
	     i < حجم(ساخت ext2_inode);
	     cp++, i++)
		اگر (*cp)
			واپس 0;
	واپس 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_سیٹ(ext2_filsys fs, ext2_ino_t inum,
			   ساخت ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 چکری_جانچ;
	صحیح has_hi;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 0;

	has_hi = (EXT2_INODE_سائز(fs->super) > EXT2_GOOD_OLD_INODE_سائز &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_انتہا);

	retval = ext2fs_inode_csum(fs, inum, inode, &چکری_جانچ, has_hi);
	اگر (retval)
		واپس retval;
	inode->i_checksum_lo = ext2fs_cpu_مقصد_le16(چکری_جانچ & 0xFFFF);
	اگر (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_مقصد_le16(چکری_جانچ >> 16);
	واپس 0;
}

__u16 ext2fs_گروہ_desc_csum(ext2_filsys fs, dgrp_t گروہ)
{
	ساخت ext2_گروہ_desc *desc = ext2fs_گروہ_desc(fs, fs->گروہ_desc,
							 گروہ);
	size_t انحراف, سائز = EXT2_DESC_سائز(fs->super);
	__u16 چکری_جانچ = 0;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ساخت ext4_گروہ_desc swabdesc;
	size_t save_سائز = سائز;
	ثابت size_t ext4_bg_سائز = حجم(ساخت ext4_گروہ_desc);
	ساخت ext2_گروہ_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	اگر (سائز > ext4_bg_سائز)
		سائز = ext4_bg_سائز;
	حافظہ_نقل(&swabdesc, desc, سائز);
	ext2fs_تبادلہ_گروہ_desc2(fs, (ساخت ext2_گروہ_desc *) &swabdesc);
	desc = (ساخت ext2_گروہ_desc *) &swabdesc;
	گروہ = ext2fs_swab32(گروہ);
#اگر_ختم

	اگر (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_چکری_جانچ;
		__u32 چکری_جانچ_32;

		old_چکری_جانچ = desc->bg_checksum;
		desc->bg_checksum = 0;
		چکری_جانچ_32 = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)&گروہ,
					 حجم(گروہ));
		چکری_جانچ_32 = ext2fs_crc32c_le(چکری_جانچ_32, (غیرعلامتی حرف *)desc,
					 سائز);
		desc->bg_checksum = old_چکری_جانچ;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
		اگر (save_سائز > ext4_bg_سائز)
			چکری_جانچ_32 = ext2fs_crc32c_le(چکری_جانچ_32,
				     (غیرعلامتی حرف *)save_desc + ext4_bg_سائز,
	                             save_سائز - ext4_bg_سائز);
#اگر_ختم
		چکری_جانچ = چکری_جانچ_32 & 0xFFFF;
		جاؤ برآمد;
	}

	/* old crc16 code */
	انحراف = offsetof(ساخت ext2_گروہ_desc, bg_checksum);
	چکری_جانچ = ext2fs_crc16(~0, fs->super->s_uuid,
			   حجم(fs->super->s_uuid));
	چکری_جانچ = ext2fs_crc16(چکری_جانچ, &گروہ, حجم(گروہ));
	چکری_جانچ = ext2fs_crc16(چکری_جانچ, desc, انحراف);
	انحراف += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	اگر (انحراف < سائز) {
		چکری_جانچ = ext2fs_crc16(چکری_جانچ, (حرف *)desc + انحراف,
				   سائز - انحراف);
	}
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	اگر (save_سائز > ext4_bg_سائز)
		چکری_جانچ = ext2fs_crc16(چکری_جانچ, (حرف *)save_desc + ext4_bg_سائز,
				   save_سائز - ext4_bg_سائز);
#اگر_ختم

برآمد:
	واپس چکری_جانچ;
}

صحیح ext2fs_گروہ_desc_csum_verify(ext2_filsys fs, dgrp_t گروہ)
{
	اگر (ext2fs_has_گروہ_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, گروہ) !=
	     ext2fs_گروہ_desc_csum(fs, گروہ)))
		واپس 0;

	واپس 1;
}

خالی ext2fs_گروہ_desc_csum_سیٹ(ext2_filsys fs, dgrp_t گروہ)
{
	اگر (!ext2fs_has_گروہ_desc_csum(fs))
		واپس;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_سیٹ(fs, گروہ, ext2fs_گروہ_desc_csum(fs, گروہ));
}

ساکن __u32 ڈھونڈو_آخری_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_فی_grp, dgrp_t grp_no)
{
	ext2_ino_t i, آغاز_ino, انتہا_ino;

	آغاز_ino = grp_no * inodes_فی_grp + 1;
	انتہا_ino = آغاز_ino + inodes_فی_grp - 1;

	برائے (i = انتہا_ino; i >= آغاز_ino; i--) {
		اگر (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			واپس i - آغاز_ino + 1;
	}
	واپس inodes_فی_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_سیٹ_gdt_csum(ext2_filsys fs)
{
	ساخت ext2_super_بلاک *sb = fs->super;
	صحیح dirty = 0;
	dgrp_t i;

	اگر (!fs->inode_map)
		واپس EXT2_ET_NO_INODE_BITMAP;

	اگر (!ext2fs_has_گروہ_desc_csum(fs))
		واپس 0;

	برائے (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_غیر_مستعمل = ext2fs_bg_itable_غیر_مستعمل(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_گنتی = ext2fs_bg_free_inodes_گنتی(fs, i);
		__u32 old_free_بلاکیں_گنتی = ext2fs_bg_free_بلاکیں_گنتی(fs, i);

		اگر (old_free_بلاکیں_گنتی == sb->s_بلاکیں_فی_گروہ &&
		    i != fs->گروہ_desc_گنتی - 1)
			ext2fs_bg_flags_سیٹ(fs, i, EXT2_BG_بلاک_UNINIT);

		اگر (old_free_inodes_گنتی == sb->s_inodes_فی_گروہ) {
			ext2fs_bg_flags_سیٹ(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_غیر_مستعمل_سیٹ(fs, i, sb->s_inodes_فی_گروہ);
		} ورنہ {
			صحیح غیر_مستعمل =
				sb->s_inodes_فی_گروہ -
				ڈھونڈو_آخری_inode_ingrp(fs->inode_map,
						      sb->s_inodes_فی_گروہ, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_غیر_مستعمل_سیٹ(fs, i, غیر_مستعمل);
		}

		ext2fs_گروہ_desc_csum_سیٹ(fs, i);
		اگر (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		اگر (old_غیر_مستعمل != ext2fs_bg_itable_غیر_مستعمل(fs, i))
			dirty = 1;
		اگر (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	اگر (dirty)
		ext2fs_mark_super_dirty(fs);
	واپس 0;
}

#اگر_تعریف_شدہ DEBUG
#شامل "e2p/e2p.h"

خالی print_csum(ثابت حرف *پیغام, ext2_filsys fs, dgrp_t گروہ)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	ساخت ext2_گروہ_desc *desc = ext2fs_گروہ_desc(fs, fs->گروہ_desc,
							 گروہ);
	size_t سائز = EXT2_DESC_سائز(fs->super);
	ساخت ext2_super_بلاک *sb = fs->super;
	صحیح انحراف = offsetof(ساخت ext2_گروہ_desc, bg_checksum);
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ساخت ext4_گروہ_desc swabdesc;
	ساخت ext2_گروہ_desc *save_desc = desc;
	ثابت size_t ext4_bg_سائز = حجم(ساخت ext4_گروہ_desc);
	size_t save_سائز = سائز;
#اگر_ختم

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	اگر (سائز > ext4_bg_سائز)
		سائز = ext4_bg_سائز;
	حافظہ_نقل(&swabdesc, desc, سائز);
	ext2fs_تبادلہ_گروہ_desc2(fs, (ساخت ext2_گروہ_desc *) &swabdesc);
	desc = (ساخت ext2_گروہ_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(گروہ);
#ورنہ
	swabgroup = گروہ;
#اگر_ختم

	crc1 = ext2fs_crc16(~0, sb->s_uuid, حجم(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, حجم(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, انحراف);
	انحراف += حجم(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	اگر (انحراف < سائز)
		crc3 = ext2fs_crc16(crc3, (حرف *)desc + انحراف, سائز - انحراف);
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	اگر (save_سائز > ext4_bg_سائز)
		crc3 = ext2fs_crc16(crc3, (حرف *)save_desc + ext4_bg_سائز,
				    save_سائز - ext4_bg_سائز);
#اگر_ختم

	لکھو("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       پیغام, e2p_uuid2str(sb->s_uuid), crc1, گروہ, crc2, crc3,
	       ext2fs_گروہ_desc_csum(fs, گروہ));
}

غیرعلامتی حرف sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

صحیح مین(صحیح argc, حرف **argv)
{
	ساخت ext2_super_بلاک param;
	errcode_t		retval;
	ext2_filsys		fs;
	صحیح			i;
	__u16 csum1, csum2, csum_معلوم = 0xd3a4;

	memset(&param, 0, حجم(param));
	ext2fs_بلاکیں_گنتی_سیٹ(&param, 32768);
#اگر 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64_ثنائی_ہندسہ;
	param.s_desc_سائز = 128;
	csum_معلوم = 0x5b6e;
#اگر_ختم

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بٹ, &param,
				   test_io_manager, &fs);
	اگر (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		باہر_نکلو(1);
	}
	حافظہ_نقل(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	برائے (i=0; i < fs->گروہ_desc_گنتی; i++) {
		ext2fs_بلاک_bitmap_loc_سیٹ(fs, i, 124);
		ext2fs_inode_bitmap_loc_سیٹ(fs, i, 125);
		ext2fs_inode_جدول_loc_سیٹ(fs, i, 126);
		ext2fs_bg_free_بلاکیں_گنتی_سیٹ(fs, i, 31119);
		ext2fs_bg_free_inodes_گنتی_سیٹ(fs, i, 15701);
		ext2fs_bg_مستعمل_dirs_گنتی_سیٹ(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_گروہ_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	اگر (csum1 != csum_معلوم) {
		لکھو("checksum for group 0 should be %04x\n", csum_معلوم);
		باہر_نکلو(1);
	}
	csum2 = ext2fs_گروہ_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	اگر (csum1 == csum2) {
		لکھو("checksums for different groups shouldn't match\n");
		باہر_نکلو(1);
	}
	csum2 = ext2fs_گروہ_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	اگر (csum1 == csum2) {
		لکھو("checksums for different groups shouldn't match\n");
		باہر_نکلو(1);
	}
	ext2fs_bg_checksum_سیٹ(fs, 0, csum1);
	csum2 = ext2fs_گروہ_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	اگر (csum1 != csum2) {
		لکھو("checksums should not depend on checksum field\n");
		باہر_نکلو(1);
	}
	اگر (!ext2fs_گروہ_desc_csum_verify(fs, 0)) {
		لکھو("checksums should verify against gd_checksum\n");
		باہر_نکلو(1);
	}
	memset(fs->super->s_uuid, 0x30, حجم(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	اگر (ext2fs_گروہ_desc_csum_verify(fs, 0) != 0) {
		لکھو("checksums for different filesystems shouldn't match\n");
		باہر_نکلو(1);
	}
	csum1 = ext2fs_گروہ_desc_csum(fs, 0);
	ext2fs_bg_checksum_سیٹ(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_بلاکیں_گنتی_سیٹ(fs, 0, 1);
	csum2 = ext2fs_گروہ_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	اگر (csum1 == csum2) {
		لکھو("checksums for different data shouldn't match\n");
		باہر_نکلو(1);
	}
	ext2fs_free(fs);

	واپس 0;
}
#اگر_ختم
