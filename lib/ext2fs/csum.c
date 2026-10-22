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

#అన్తర్భావ "config.h"
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "crc16.h"
#అన్తర్భావ <assert.h>

#నిర్వచితం_కాకపోతే offsetof
#నిర్వచించు offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#అయితే_ముగింపు

#నిర్వచితమైతే DEBUG
#నిర్వచించు STATIC
#లేకపోతే
#నిర్వచించు STATIC స్థిర
#అయితే_ముగింపు

శూన్య ext2fs_init_csum_విత్తనం(ext2_filsys fs)
{
	ఒకవేళ (ext2fs_has_feature_csum_విత్తనం(fs->super))
		fs->csum_విత్తనం = fs->super->s_checksum_విత్తనం;
	లేకపోతే ఒకవేళ (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_విత్తనం = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 పరిమాణం(fs->super->s_uuid));
}

స్థిర __u32 ext2fs_mmp_csum(ext2_filsys fs, నిర్మాణం mmp_struct *mmp)
{
	పూర్ణాంకం స్థానభ్రంశం = offsetof(నిర్మాణం mmp_struct, mmp_checksum);

	తిరిగి ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)mmp, స్థానభ్రంశం);
}

పూర్ణాంకం ext2fs_mmp_csum_verify(ext2_filsys fs, నిర్మాణం mmp_struct *mmp)
{
	__u32 calculated;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	తిరిగి ext2fs_le32_గమ్యం_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_సమితి(ext2_filsys fs, నిర్మాణం mmp_struct *mmp)
{
	__u32 చక్రీయ_తనిఖీ;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 0;

	చక్రీయ_తనిఖీ = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_గమ్యం_le32(చక్రీయ_తనిఖీ);

	తిరిగి 0;
}

పూర్ణాంకం ext2fs_verify_csum_type(ext2_filsys fs, నిర్మాణం ext2_super_ఖండం *sb)
{
	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 1;

	తిరిగి sb->s_checksum_type == EXT2_చక్రీయ_తనిఖీ_32_C_CHKSUM;
}

స్థిర __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((వాడని)),
				    నిర్మాణం ext2_super_ఖండం *sb)
{
	పూర్ణాంకం స్థానభ్రంశం = offsetof(నిర్మాణం ext2_super_ఖండం, s_checksum);

	తిరిగి ext2fs_crc32c_le(~0, (గుర్తులేని అక్షరం *)sb, స్థానభ్రంశం);
}

/* NOTE: The input to this function MUST be in LE order */
పూర్ణాంకం ext2fs_superblock_csum_verify(ext2_filsys fs, నిర్మాణం ext2_super_ఖండం *sb)
{
	__u32 flag, calculated;

	ఒకవేళ (fs->flags & EXT2_FLAG_మార్పిడి_బైట్లు)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	లేకపోతే
		flag = ext2fs_cpu_గమ్యం_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ఒకవేళ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		తిరిగి 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	తిరిగి ext2fs_le32_గమ్యం_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_సమితి(ext2_filsys fs,
				     నిర్మాణం ext2_super_ఖండం *sb)
{
	__u32 flag, చక్రీయ_తనిఖీ;

	ఒకవేళ (fs->flags & EXT2_FLAG_మార్పిడి_బైట్లు)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	లేకపోతే
		flag = ext2fs_cpu_గమ్యం_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	ఒకవేళ (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		తిరిగి 0;

	చక్రీయ_తనిఖీ = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_గమ్యం_le32(చక్రీయ_తనిఖీ);

	తిరిగి 0;
}

స్థిర errcode_t ext2fs_ext_attr_ఖండం_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((వాడని)),
					    blk64_t ఖండం,
					    నిర్మాణం ext2_ext_attr_శీర్షిక *hdr,
					    __u32 *చక్రీయ_తనిఖీ)
{
	అక్షరం *buf = (అక్షరం *)hdr;
	__u32 old_చక్రీయ_తనిఖీ = hdr->h_checksum;

	hdr->h_checksum = 0;
	ఖండం = ext2fs_cpu_గమ్యం_le64(ఖండం);
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)&ఖండం,
				పరిమాణం(ఖండం));
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)buf, fs->blocksize);
	hdr->h_checksum = old_చక్రీయ_తనిఖీ;

	తిరిగి 0;
}

పూర్ణాంకం ext2fs_ext_attr_ఖండం_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t ఖండం,
				      నిర్మాణం ext2_ext_attr_శీర్షిక *hdr)
{
	__u32 calculated;
	errcode_t retval;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 1;

	retval = ext2fs_ext_attr_ఖండం_csum(fs, inum, ఖండం, hdr, &calculated);
	ఒకవేళ (retval)
		తిరిగి 0;

	తిరిగి ext2fs_le32_గమ్యం_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_ఖండం_csum_సమితి(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t ఖండం,
					 నిర్మాణం ext2_ext_attr_శీర్షిక *hdr)
{
	errcode_t retval;
	__u32 చక్రీయ_తనిఖీ;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 0;

	retval = ext2fs_ext_attr_ఖండం_csum(fs, inum, ఖండం, hdr, &చక్రీయ_తనిఖీ);
	ఒకవేళ (retval)
		తిరిగి retval;
	hdr->h_checksum = ext2fs_cpu_గమ్యం_le32(చక్రీయ_తనిఖీ);
	తిరిగి 0;
}

స్థిర __u16 do_nothing16(__u16 x)
{
	తిరిగి x;
}

స్థిర __u16 disk_గమ్యం_host16(__u16 x)
{
	తిరిగి ext2fs_le16_గమ్యం_cpu(x);
}

స్థిర errcode_t __get_dx_countlimit(ext2_filsys fs,
				     నిర్మాణం ext2_dir_నమోదు *dirent,
				     నిర్మాణం ext2_dx_countlimit **cc,
				     పూర్ణాంకం *స్థానభ్రంశం,
				     పూర్ణాంకం need_swab)
{
	నిర్మాణం ext2_dir_నమోదు *dp;
	నిర్మాణం ext2_dx_మూలం_info *మూలం;
	నిర్మాణం ext2_dx_countlimit *c;
	పూర్ణాంకం లెక్క_స్థానభ్రంశం, max_sane_entries;
	గుర్తులేని పూర్ణాంకం rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_గమ్యం_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	ఒకవేళ (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		లెక్క_స్థానభ్రంశం = 8;
	లేకపోతే ఒకవేళ (rec_len == 12) {
		dp = (నిర్మాణం ext2_dir_నమోదు *)(((అక్షరం *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		ఒకవేళ (rec_len != fs->blocksize - 12)
			తిరిగి EXT2_ET_DB_NOT_దొరికింది;
		మూలం = (నిర్మాణం ext2_dx_మూలం_info *)(((అక్షరం *)dp + 12));
		ఒకవేళ (మూలం->reserved_zero ||
		    మూలం->info_నిడివి != పరిమాణం(నిర్మాణం ext2_dx_మూలం_info))
			తిరిగి EXT2_ET_DB_NOT_దొరికింది;
		లెక్క_స్థానభ్రంశం = 32;
	} లేకపోతే
		తిరిగి EXT2_ET_DB_NOT_దొరికింది;

	c = (నిర్మాణం ext2_dx_countlimit *)(((అక్షరం *)dirent) + లెక్క_స్థానభ్రంశం);
	max_sane_entries = (fs->blocksize - లెక్క_స్థానభ్రంశం) /
			   పరిమాణం(నిర్మాణం ext2_dx_నమోదు);
	ఒకవేళ (ext2fs_le16_గమ్యం_cpu(c->హద్దు) > max_sane_entries ||
	    ext2fs_le16_గమ్యం_cpu(c->లెక్క) > max_sane_entries)
		తిరిగి EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ఒకవేళ (స్థానభ్రంశం)
		*స్థానభ్రంశం = లెక్క_స్థానభ్రంశం;
	ఒకవేళ (cc)
		*cc = c;

	తిరిగి 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   నిర్మాణం ext2_dir_నమోదు *dirent,
				   నిర్మాణం ext2_dx_countlimit **cc,
				   పూర్ణాంకం *స్థానభ్రంశం)
{
	తిరిగి __get_dx_countlimit(fs, dirent, cc, స్థానభ్రంశం, 0);
}

శూన్య ext2fs_initialize_dirent_తోక(ext2_filsys fs,
				   నిర్మాణం ext2_dir_నమోదు_తోక *t)
{
	memset(t, 0, పరిమాణం(నిర్మాణం ext2_dir_నమోదు_తోక));
	ext2fs_సమితి_rec_len(fs, పరిమాణం(నిర్మాణం ext2_dir_నమోదు_తోక),
			   (నిర్మాణం ext2_dir_నమోదు *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

స్థిర errcode_t __get_dirent_tail(ext2_filsys fs,
				   నిర్మాణం ext2_dir_నమోదు *dirent,
				   నిర్మాణం ext2_dir_నమోదు_తోక **tt,
				   పూర్ణాంకం need_swab)
{
	నిర్మాణం ext2_dir_నమోదు *d;
	శూన్య *శిఖరం;
	నిర్మాణం ext2_dir_నమోదు_తోక *t;
	గుర్తులేని పూర్ణాంకం rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_గమ్యం_host16 : do_nothing16);

	ఒకవేళ (fs->blocksize < 1024)
		తిరిగి EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	శిఖరం = EXT2_DIRENT_తోక(dirent, fs->blocksize);

	అయితే ((శూన్య *) d < శిఖరం) {
		rec_len = translate(d->rec_len);
		ఒకవేళ ((rec_len < 8) || (rec_len & 0x03))
			తిరిగి EXT2_ET_DIR_CORRUPTED;
		d = (నిర్మాణం ext2_dir_నమోదు *)(((అక్షరం *)d) + rec_len);
	}

	ఒకవేళ ((అక్షరం *)d > ((అక్షరం *)dirent + fs->blocksize))
			తిరిగి EXT2_ET_DIR_CORRUPTED;
	ఒకవేళ (d != శిఖరం)
		తిరిగి EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (నిర్మాణం ext2_dir_నమోదు_తోక *)d;
	ఒకవేళ (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != పరిమాణం(నిర్మాణం ext2_dir_నమోదు_తోక) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		తిరిగి EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	ఒకవేళ (tt)
		*tt = t;
	తిరిగి retval;
}

పూర్ణాంకం ext2fs_dirent_has_తోక(ext2_filsys fs, నిర్మాణం ext2_dir_నమోదు *dirent)
{
	తిరిగి __get_dirent_tail(fs, dirent, ఖాళీ, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

స్థిర errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    నిర్మాణం ext2_dir_నమోదు *dirent, __u32 *చక్రీయ_తనిఖీ,
				    పూర్ణాంకం కొలత)
{
	errcode_t retval;
	అక్షరం *buf = (అక్షరం *)dirent;
	__u32 gen;
	నిర్మాణం ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	inum = ext2fs_cpu_గమ్యం_le32(inum);
	gen = ext2fs_cpu_గమ్యం_le32(inode.i_generation);
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)&inum,
				పరిమాణం(inum));
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)&gen, పరిమాణం(gen));
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)buf, కొలత);

	తిరిగి 0;
}

పూర్ణాంకం ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      నిర్మాణం ext2_dir_నమోదు *dirent)
{
	errcode_t retval;
	__u32 calculated;
	నిర్మాణం ext2_dir_నమోదు_తోక *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ఒకవేళ (retval)
		తిరిగి 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (అక్షరం *)t - (అక్షరం *)dirent);
	ఒకవేళ (retval)
		తిరిగి 0;
	తిరిగి ext2fs_le32_గమ్యం_cpu(t->det_checksum) == calculated;
}

స్థిర errcode_t ext2fs_dirent_csum_సమితి(ext2_filsys fs, ext2_ino_t inum,
					నిర్మాణం ext2_dir_నమోదు *dirent)
{
	errcode_t retval;
	__u32 చక్రీయ_తనిఖీ;
	నిర్మాణం ext2_dir_నమోదు_తోక *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	ఒకవేళ (retval)
		తిరిగి retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &చక్రీయ_తనిఖీ,
				    (అక్షరం *)t - (అక్షరం *)dirent);
	ఒకవేళ (retval)
		తిరిగి retval;
	t->det_checksum = ext2fs_cpu_గమ్యం_le32(చక్రీయ_తనిఖీ);
	తిరిగి 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 నిర్మాణం ext2_dir_నమోదు *dirent,
			 __u32 *చక్రీయ_తనిఖీ, నిర్మాణం ext2_dx_తోక **ret_t)
{
	errcode_t retval;
	అక్షరం *buf = (అక్షరం *)dirent;
	పూర్ణాంకం కొలత;
	__u32 gen, dummy_csum = 0;
	నిర్మాణం ext2_inode inode;
	నిర్మాణం ext2_dx_తోక *t;
	నిర్మాణం ext2_dx_countlimit *c;
	పూర్ణాంకం లెక్క_స్థానభ్రంశం, హద్దు, లెక్క;

	retval = __get_dx_countlimit(fs, dirent, &c, &లెక్క_స్థానభ్రంశం, 1);
	ఒకవేళ (retval)
		తిరిగి retval;
	హద్దు = ext2fs_le16_గమ్యం_cpu(c->హద్దు);
	లెక్క = ext2fs_le16_గమ్యం_cpu(c->లెక్క);
	ఒకవేళ (లెక్క_స్థానభ్రంశం + (హద్దు * పరిమాణం(నిర్మాణం ext2_dx_నమోదు)) >
	    fs->blocksize - పరిమాణం(నిర్మాణం ext2_dx_తోక))
		తిరిగి EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (నిర్మాణం ext2_dx_తోక *)(((నిర్మాణం ext2_dx_నమోదు *)c) + హద్దు);

	కొలత = లెక్క_స్థానభ్రంశం + (లెక్క * పరిమాణం(నిర్మాణం ext2_dx_నమోదు));

	retval = ext2fs_read_inode(fs, inum, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	inum = ext2fs_cpu_గమ్యం_le32(inum);
	gen = ext2fs_cpu_గమ్యం_le32(inode.i_generation);
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)&inum,
				పరిమాణం(inum));
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)&gen, పరిమాణం(gen));
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)buf, కొలత);
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)t, 4);
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)&dummy_csum, 4);

	ఒకవేళ (ret_t)
		*ret_t = t;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 నిర్మాణం ext2_dir_నమోదు *dirent)
{
	__u32 calculated;
	errcode_t retval;
	నిర్మాణం ext2_dx_తోక *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	ఒకవేళ (retval)
		తిరిగి 0;

	తిరిగి ext2fs_le32_గమ్యం_cpu(t->dt_checksum) == calculated;
}

స్థిర errcode_t ext2fs_dx_csum_సమితి(ext2_filsys fs, ext2_ino_t inum,
				    నిర్మాణం ext2_dir_నమోదు *dirent)
{
	__u32 చక్రీయ_తనిఖీ;
	errcode_t retval = 0;
	నిర్మాణం ext2_dx_తోక *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &చక్రీయ_తనిఖీ, &t);
	ఒకవేళ (retval)
		తిరిగి retval;
	t->dt_checksum = ext2fs_cpu_గమ్యం_le32(చక్రీయ_తనిఖీ);
	తిరిగి retval;
}

పూర్ణాంకం ext2fs_dir_ఖండం_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 నిర్మాణం ext2_dir_నమోదు *dirent)
{
	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 1;

	ఒకవేళ (__get_dirent_tail(fs, dirent, ఖాళీ, 1) == 0)
		తిరిగి ext2fs_dirent_csum_verify(fs, inum, dirent);
	ఒకవేళ (__get_dx_countlimit(fs, dirent, ఖాళీ, ఖాళీ, 1) == 0)
		తిరిగి ext2fs_dx_csum_verify(fs, inum, dirent);

	తిరిగి 0;
}

errcode_t ext2fs_dir_ఖండం_csum_సమితి(ext2_filsys fs, ext2_ino_t inum,
				    నిర్మాణం ext2_dir_నమోదు *dirent)
{
	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 0;

	ఒకవేళ (__get_dirent_tail(fs, dirent, ఖాళీ, 1) == 0)
		తిరిగి ext2fs_dirent_csum_సమితి(fs, inum, dirent);
	ఒకవేళ (__get_dx_countlimit(fs, dirent, ఖాళీ, ఖాళీ, 1) == 0)
		తిరిగి ext2fs_dx_csum_సమితి(fs, inum, dirent);

	ఒకవేళ (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		తిరిగి 0;
	తిరిగి EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#నిర్వచించు EXT3_EXTENT_తోక_స్థానభ్రంశం(hdr)	(పరిమాణం(నిర్మాణం ext3_extent_శీర్షిక) + \
	(పరిమాణం(నిర్మాణం ext3_extent) * ext2fs_le16_గమ్యం_cpu((hdr)->eh_max)))

స్థిర నిర్మాణం ext3_extent_తోక *get_extent_తోక(నిర్మాణం ext3_extent_శీర్షిక *h)
{
	తిరిగి (నిర్మాణం ext3_extent_తోక *)(((అక్షరం *)h) +
					   EXT3_EXTENT_తోక_స్థానభ్రంశం(h));
}

స్థిర errcode_t ext2fs_extent_ఖండం_csum(ext2_filsys fs, ext2_ino_t inum,
					  నిర్మాణం ext3_extent_శీర్షిక *eh,
					  __u32 *చక్రీయ_తనిఖీ)
{
	పూర్ణాంకం కొలత;
	__u32 gen;
	errcode_t retval;
	నిర్మాణం ext2_inode inode;

	కొలత = EXT3_EXTENT_తోక_స్థానభ్రంశం(eh) + offsetof(నిర్మాణం ext3_extent_తోక,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;
	inum = ext2fs_cpu_గమ్యం_le32(inum);
	gen = ext2fs_cpu_గమ్యం_le32(inode.i_generation);
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)&inum,
				పరిమాణం(inum));
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)&gen, పరిమాణం(gen));
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)eh, కొలత);

	తిరిగి 0;
}

పూర్ణాంకం ext2fs_extent_ఖండం_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    నిర్మాణం ext3_extent_శీర్షిక *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	నిర్మాణం ext3_extent_తోక *t = get_extent_తోక(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 1;

	provided = ext2fs_le32_గమ్యం_cpu(t->et_checksum);
	retval = ext2fs_extent_ఖండం_csum(fs, inum, eh, &calculated);
	ఒకవేళ (retval)
		తిరిగి 0;

	తిరిగి provided == calculated;
}

errcode_t ext2fs_extent_ఖండం_csum_సమితి(ext2_filsys fs, ext2_ino_t inum,
				       నిర్మాణం ext3_extent_శీర్షిక *eh)
{
	errcode_t retval;
	__u32 చక్రీయ_తనిఖీ;
	నిర్మాణం ext3_extent_తోక *t = get_extent_తోక(eh);

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_ఖండం_csum(fs, inum, eh, &చక్రీయ_తనిఖీ);
	ఒకవేళ (retval)
		తిరిగి retval;
	t->et_checksum = ext2fs_cpu_గమ్యం_le32(చక్రీయ_తనిఖీ);
	తిరిగి retval;
}

పూర్ణాంకం ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t సమూహం,
				    అక్షరం *bitmap, పూర్ణాంకం కొలత)
{
	నిర్మాణం ext4_సమూహం_desc *gdp = (నిర్మాణం ext4_సమూహం_desc *)
			ext2fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	__u32 provided, calculated;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)bitmap,
				      కొలత);
	ఒకవేళ (EXT2_DESC_కొలత(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_చివర)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	లేకపోతే
		calculated &= 0xFFFF;

	తిరిగి provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_సమితి(ext2_filsys fs, dgrp_t సమూహం,
				       అక్షరం *bitmap, పూర్ణాంకం కొలత)
{
	__u32 చక్రీయ_తనిఖీ;
	నిర్మాణం ext4_సమూహం_desc *gdp = (నిర్మాణం ext4_సమూహం_desc *)
			ext2fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 0;

	చక్రీయ_తనిఖీ = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)bitmap, కొలత);
	gdp->bg_inode_bitmap_csum_lo = చక్రీయ_తనిఖీ & 0xFFFF;
	ఒకవేళ (EXT2_DESC_కొలత(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_చివర)
		gdp->bg_inode_bitmap_csum_hi = చక్రీయ_తనిఖీ >> 16;

	తిరిగి 0;
}

పూర్ణాంకం ext2fs_ఖండం_bitmap_csum_verify(ext2_filsys fs, dgrp_t సమూహం,
				    అక్షరం *bitmap, పూర్ణాంకం కొలత)
{
	నిర్మాణం ext4_సమూహం_desc *gdp = (నిర్మాణం ext4_సమూహం_desc *)
			ext2fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);
	__u32 provided, calculated;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 1;
	provided = gdp->bg_ఖండం_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)bitmap,
				      కొలత);
	ఒకవేళ (EXT2_DESC_కొలత(fs->super) >= EXT4_BG_ఖండం_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_ఖండం_bitmap_csum_hi << 16;
	లేకపోతే
		calculated &= 0xFFFF;

	తిరిగి provided == calculated;
}

errcode_t ext2fs_ఖండం_bitmap_csum_సమితి(ext2_filsys fs, dgrp_t సమూహం,
				       అక్షరం *bitmap, పూర్ణాంకం కొలత)
{
	__u32 చక్రీయ_తనిఖీ;
	నిర్మాణం ext4_సమూహం_desc *gdp = (నిర్మాణం ext4_సమూహం_desc *)
			ext2fs_సమూహం_desc(fs, fs->సమూహం_desc, సమూహం);

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 0;

	చక్రీయ_తనిఖీ = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)bitmap, కొలత);
	gdp->bg_ఖండం_bitmap_csum_lo = చక్రీయ_తనిఖీ & 0xFFFF;
	ఒకవేళ (EXT2_DESC_కొలత(fs->super) >= EXT4_BG_ఖండం_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_ఖండం_bitmap_csum_hi = చక్రీయ_తనిఖీ >> 16;

	తిరిగి 0;
}

స్థిర errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       నిర్మాణం ext2_inode_large *inode,
			       __u32 *చక్రీయ_తనిఖీ, పూర్ణాంకం has_hi)
{
	__u32 gen;
	నిర్మాణం ext2_inode_large *desc = inode;
	size_t కొలత = EXT2_INODE_కొలత(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	ఒకవేళ (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_గమ్యం_le32(inum);
	gen = inode->i_generation;
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)&inum,
				పరిమాణం(inum));
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)&gen, పరిమాణం(gen));
	*చక్రీయ_తనిఖీ = ext2fs_crc32c_le(*చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)desc, కొలత);

	inode->i_checksum_lo = old_lo;
	ఒకవేళ (has_hi)
		inode->i_checksum_hi = old_hi;
	తిరిగి 0;
}

పూర్ణాంకం ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     నిర్మాణం ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	గుర్తులేని పూర్ణాంకం i, has_hi;
	అక్షరం *cp;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 1;

	has_hi = (EXT2_INODE_కొలత(fs->super) > EXT2_GOOD_OLD_INODE_కొలత &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_చివర);

	provided = ext2fs_le16_గమ్యం_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	ఒకవేళ (retval)
		తిరిగి 0;
	ఒకవేళ (has_hi) {
		__u32 hi = ext2fs_le16_గమ్యం_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} లేకపోతే
		calculated &= 0xFFFF;

	ఒకవేళ (provided == calculated)
		తిరిగి 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	కొరకు (cp = (అక్షరం *) inode, i = 0;
	     i < పరిమాణం(నిర్మాణం ext2_inode);
	     cp++, i++)
		ఒకవేళ (*cp)
			తిరిగి 0;
	తిరిగి 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_సమితి(ext2_filsys fs, ext2_ino_t inum,
			   నిర్మాణం ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 చక్రీయ_తనిఖీ;
	పూర్ణాంకం has_hi;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 0;

	has_hi = (EXT2_INODE_కొలత(fs->super) > EXT2_GOOD_OLD_INODE_కొలత &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_చివర);

	retval = ext2fs_inode_csum(fs, inum, inode, &చక్రీయ_తనిఖీ, has_hi);
	ఒకవేళ (retval)
		తిరిగి retval;
	inode->i_checksum_lo = ext2fs_cpu_గమ్యం_le16(చక్రీయ_తనిఖీ & 0xFFFF);
	ఒకవేళ (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_గమ్యం_le16(చక్రీయ_తనిఖీ >> 16);
	తిరిగి 0;
}

__u16 ext2fs_సమూహం_desc_csum(ext2_filsys fs, dgrp_t సమూహం)
{
	నిర్మాణం ext2_సమూహం_desc *desc = ext2fs_సమూహం_desc(fs, fs->సమూహం_desc,
							 సమూహం);
	size_t స్థానభ్రంశం, కొలత = EXT2_DESC_కొలత(fs->super);
	__u16 చక్రీయ_తనిఖీ = 0;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	నిర్మాణం ext4_సమూహం_desc swabdesc;
	size_t save_కొలత = కొలత;
	స్థిరరాశి size_t ext4_bg_కొలత = పరిమాణం(నిర్మాణం ext4_సమూహం_desc);
	నిర్మాణం ext2_సమూహం_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	ఒకవేళ (కొలత > ext4_bg_కొలత)
		కొలత = ext4_bg_కొలత;
	జ్ఞాపకం_నకలు(&swabdesc, desc, కొలత);
	ext2fs_మార్పిడి_సమూహం_desc2(fs, (నిర్మాణం ext2_సమూహం_desc *) &swabdesc);
	desc = (నిర్మాణం ext2_సమూహం_desc *) &swabdesc;
	సమూహం = ext2fs_swab32(సమూహం);
#అయితే_ముగింపు

	ఒకవేళ (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_చక్రీయ_తనిఖీ;
		__u32 చక్రీయ_తనిఖీ_32;

		old_చక్రీయ_తనిఖీ = desc->bg_checksum;
		desc->bg_checksum = 0;
		చక్రీయ_తనిఖీ_32 = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)&సమూహం,
					 పరిమాణం(సమూహం));
		చక్రీయ_తనిఖీ_32 = ext2fs_crc32c_le(చక్రీయ_తనిఖీ_32, (గుర్తులేని అక్షరం *)desc,
					 కొలత);
		desc->bg_checksum = old_చక్రీయ_తనిఖీ;
#నిర్వచితమైతే పదాలు_BIGENDIAN
		ఒకవేళ (save_కొలత > ext4_bg_కొలత)
			చక్రీయ_తనిఖీ_32 = ext2fs_crc32c_le(చక్రీయ_తనిఖీ_32,
				     (గుర్తులేని అక్షరం *)save_desc + ext4_bg_కొలత,
	                             save_కొలత - ext4_bg_కొలత);
#అయితే_ముగింపు
		చక్రీయ_తనిఖీ = చక్రీయ_తనిఖీ_32 & 0xFFFF;
		వెళ్ళు బహిర్గతం;
	}

	/* old crc16 code */
	స్థానభ్రంశం = offsetof(నిర్మాణం ext2_సమూహం_desc, bg_checksum);
	చక్రీయ_తనిఖీ = ext2fs_crc16(~0, fs->super->s_uuid,
			   పరిమాణం(fs->super->s_uuid));
	చక్రీయ_తనిఖీ = ext2fs_crc16(చక్రీయ_తనిఖీ, &సమూహం, పరిమాణం(సమూహం));
	చక్రీయ_తనిఖీ = ext2fs_crc16(చక్రీయ_తనిఖీ, desc, స్థానభ్రంశం);
	స్థానభ్రంశం += పరిమాణం(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ఒకవేళ (స్థానభ్రంశం < కొలత) {
		చక్రీయ_తనిఖీ = ext2fs_crc16(చక్రీయ_తనిఖీ, (అక్షరం *)desc + స్థానభ్రంశం,
				   కొలత - స్థానభ్రంశం);
	}
#నిర్వచితమైతే పదాలు_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	ఒకవేళ (save_కొలత > ext4_bg_కొలత)
		చక్రీయ_తనిఖీ = ext2fs_crc16(చక్రీయ_తనిఖీ, (అక్షరం *)save_desc + ext4_bg_కొలత,
				   save_కొలత - ext4_bg_కొలత);
#అయితే_ముగింపు

బహిర్గతం:
	తిరిగి చక్రీయ_తనిఖీ;
}

పూర్ణాంకం ext2fs_సమూహం_desc_csum_verify(ext2_filsys fs, dgrp_t సమూహం)
{
	ఒకవేళ (ext2fs_has_సమూహం_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, సమూహం) !=
	     ext2fs_సమూహం_desc_csum(fs, సమూహం)))
		తిరిగి 0;

	తిరిగి 1;
}

శూన్య ext2fs_సమూహం_desc_csum_సమితి(ext2_filsys fs, dgrp_t సమూహం)
{
	ఒకవేళ (!ext2fs_has_సమూహం_desc_csum(fs))
		తిరిగి;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_సమితి(fs, సమూహం, ext2fs_సమూహం_desc_csum(fs, సమూహం));
}

స్థిర __u32 కనుగొను_చివరి_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_చొప్పున_grp, dgrp_t grp_no)
{
	ext2_ino_t i, ప్రారంభం_ino, చివర_ino;

	ప్రారంభం_ino = grp_no * inodes_చొప్పున_grp + 1;
	చివర_ino = ప్రారంభం_ino + inodes_చొప్పున_grp - 1;

	కొరకు (i = చివర_ino; i >= ప్రారంభం_ino; i--) {
		ఒకవేళ (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			తిరిగి i - ప్రారంభం_ino + 1;
	}
	తిరిగి inodes_చొప్పున_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_సమితి_gdt_csum(ext2_filsys fs)
{
	నిర్మాణం ext2_super_ఖండం *sb = fs->super;
	పూర్ణాంకం dirty = 0;
	dgrp_t i;

	ఒకవేళ (!fs->inode_map)
		తిరిగి EXT2_ET_NO_INODE_BITMAP;

	ఒకవేళ (!ext2fs_has_సమూహం_desc_csum(fs))
		తిరిగి 0;

	కొరకు (i = 0; i < fs->సమూహం_desc_లెక్క; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_వాడని = ext2fs_bg_itable_వాడని(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_లెక్క = ext2fs_bg_free_inodes_లెక్క(fs, i);
		__u32 old_free_ఖండాలు_లెక్క = ext2fs_bg_free_ఖండాలు_లెక్క(fs, i);

		ఒకవేళ (old_free_ఖండాలు_లెక్క == sb->s_ఖండాలు_చొప్పున_సమూహం &&
		    i != fs->సమూహం_desc_లెక్క - 1)
			ext2fs_bg_flags_సమితి(fs, i, EXT2_BG_ఖండం_UNINIT);

		ఒకవేళ (old_free_inodes_లెక్క == sb->s_inodes_చొప్పున_సమూహం) {
			ext2fs_bg_flags_సమితి(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_వాడని_సమితి(fs, i, sb->s_inodes_చొప్పున_సమూహం);
		} లేకపోతే {
			పూర్ణాంకం వాడని =
				sb->s_inodes_చొప్పున_సమూహం -
				కనుగొను_చివరి_inode_ingrp(fs->inode_map,
						      sb->s_inodes_చొప్పున_సమూహం, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_వాడని_సమితి(fs, i, వాడని);
		}

		ext2fs_సమూహం_desc_csum_సమితి(fs, i);
		ఒకవేళ (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		ఒకవేళ (old_వాడని != ext2fs_bg_itable_వాడని(fs, i))
			dirty = 1;
		ఒకవేళ (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	ఒకవేళ (dirty)
		ext2fs_mark_super_dirty(fs);
	తిరిగి 0;
}

#నిర్వచితమైతే DEBUG
#అన్తర్భావ "e2p/e2p.h"

శూన్య print_csum(స్థిరరాశి అక్షరం *సందేశం, ext2_filsys fs, dgrp_t సమూహం)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	నిర్మాణం ext2_సమూహం_desc *desc = ext2fs_సమూహం_desc(fs, fs->సమూహం_desc,
							 సమూహం);
	size_t కొలత = EXT2_DESC_కొలత(fs->super);
	నిర్మాణం ext2_super_ఖండం *sb = fs->super;
	పూర్ణాంకం స్థానభ్రంశం = offsetof(నిర్మాణం ext2_సమూహం_desc, bg_checksum);
#నిర్వచితమైతే పదాలు_BIGENDIAN
	నిర్మాణం ext4_సమూహం_desc swabdesc;
	నిర్మాణం ext2_సమూహం_desc *save_desc = desc;
	స్థిరరాశి size_t ext4_bg_కొలత = పరిమాణం(నిర్మాణం ext4_సమూహం_desc);
	size_t save_కొలత = కొలత;
#అయితే_ముగింపు

#నిర్వచితమైతే పదాలు_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	ఒకవేళ (కొలత > ext4_bg_కొలత)
		కొలత = ext4_bg_కొలత;
	జ్ఞాపకం_నకలు(&swabdesc, desc, కొలత);
	ext2fs_మార్పిడి_సమూహం_desc2(fs, (నిర్మాణం ext2_సమూహం_desc *) &swabdesc);
	desc = (నిర్మాణం ext2_సమూహం_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(సమూహం);
#లేకపోతే
	swabgroup = సమూహం;
#అయితే_ముగింపు

	crc1 = ext2fs_crc16(~0, sb->s_uuid, పరిమాణం(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, పరిమాణం(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, స్థానభ్రంశం);
	స్థానభ్రంశం += పరిమాణం(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	ఒకవేళ (స్థానభ్రంశం < కొలత)
		crc3 = ext2fs_crc16(crc3, (అక్షరం *)desc + స్థానభ్రంశం, కొలత - స్థానభ్రంశం);
#నిర్వచితమైతే పదాలు_BIGENDIAN
	ఒకవేళ (save_కొలత > ext4_bg_కొలత)
		crc3 = ext2fs_crc16(crc3, (అక్షరం *)save_desc + ext4_bg_కొలత,
				    save_కొలత - ext4_bg_కొలత);
#అయితే_ముగింపు

	రాయి("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       సందేశం, e2p_uuid2str(sb->s_uuid), crc1, సమూహం, crc2, crc3,
	       ext2fs_సమూహం_desc_csum(fs, సమూహం));
}

గుర్తులేని అక్షరం sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	నిర్మాణం ext2_super_ఖండం param;
	errcode_t		retval;
	ext2_filsys		fs;
	పూర్ణాంకం			i;
	__u16 csum1, csum2, csum_తెలిసిన = 0xd3a4;

	memset(&param, 0, పరిమాణం(param));
	ext2fs_ఖండాలు_లెక్క_సమితి(&param, 32768);
#ఒకవేళ 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64_బిట్టు;
	param.s_desc_కొలత = 128;
	csum_తెలిసిన = 0x5b6e;
#అయితే_ముగింపు

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_బిట్లు, &param,
				   test_io_manager, &fs);
	ఒకవేళ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		నిష్క్రమించు(1);
	}
	జ్ఞాపకం_నకలు(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	కొరకు (i=0; i < fs->సమూహం_desc_లెక్క; i++) {
		ext2fs_ఖండం_bitmap_loc_సమితి(fs, i, 124);
		ext2fs_inode_bitmap_loc_సమితి(fs, i, 125);
		ext2fs_inode_పట్టిక_loc_సమితి(fs, i, 126);
		ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(fs, i, 31119);
		ext2fs_bg_free_inodes_లెక్క_సమితి(fs, i, 15701);
		ext2fs_bg_వాడిన_dirs_లెక్క_సమితి(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_సమూహం_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	ఒకవేళ (csum1 != csum_తెలిసిన) {
		రాయి("checksum for group 0 should be %04x\n", csum_తెలిసిన);
		నిష్క్రమించు(1);
	}
	csum2 = ext2fs_సమూహం_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	ఒకవేళ (csum1 == csum2) {
		రాయి("checksums for different groups shouldn't match\n");
		నిష్క్రమించు(1);
	}
	csum2 = ext2fs_సమూహం_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	ఒకవేళ (csum1 == csum2) {
		రాయి("checksums for different groups shouldn't match\n");
		నిష్క్రమించు(1);
	}
	ext2fs_bg_checksum_సమితి(fs, 0, csum1);
	csum2 = ext2fs_సమూహం_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	ఒకవేళ (csum1 != csum2) {
		రాయి("checksums should not depend on checksum field\n");
		నిష్క్రమించు(1);
	}
	ఒకవేళ (!ext2fs_సమూహం_desc_csum_verify(fs, 0)) {
		రాయి("checksums should verify against gd_checksum\n");
		నిష్క్రమించు(1);
	}
	memset(fs->super->s_uuid, 0x30, పరిమాణం(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	ఒకవేళ (ext2fs_సమూహం_desc_csum_verify(fs, 0) != 0) {
		రాయి("checksums for different filesystems shouldn't match\n");
		నిష్క్రమించు(1);
	}
	csum1 = ext2fs_సమూహం_desc_csum(fs, 0);
	ext2fs_bg_checksum_సమితి(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(fs, 0, 1);
	csum2 = ext2fs_సమూహం_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	ఒకవేళ (csum1 == csum2) {
		రాయి("checksums for different data shouldn't match\n");
		నిష్క్రమించు(1);
	}
	ext2fs_free(fs);

	తిరిగి 0;
}
#అయితే_ముగింపు
