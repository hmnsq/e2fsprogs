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

#अन्तर्भाव "config.h"
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "crc16.h"
#अन्तर्भाव <assert.h>

#यदि_अपरिभाषितम् offsetof
#परिभाषय offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#यदि_अन्तः

#यदि_परिभाषितम् DEBUG
#परिभाषय STATIC
#अन्यथा
#परिभाषय STATIC स्थिर
#यदि_अन्तः

शून्य ext2fs_init_csum_बीजम्(ext2_filsys fs)
{
	यदि (ext2fs_has_feature_csum_बीजम्(fs->super))
		fs->csum_बीजम् = fs->super->s_checksum_बीजम्;
	अन्यथा यदि (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_बीजम् = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 परिमाण(fs->super->s_uuid));
}

स्थिर __u32 ext2fs_mmp_csum(ext2_filsys fs, संरचना mmp_struct *mmp)
{
	पूर्णाङ्क विस्थापनम् = offsetof(संरचना mmp_struct, mmp_checksum);

	प्रत्यागम ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)mmp, विस्थापनम्);
}

पूर्णाङ्क ext2fs_mmp_csum_verify(ext2_filsys fs, संरचना mmp_struct *mmp)
{
	__u32 calculated;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	प्रत्यागम ext2fs_le32_गन्तव्यम्_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_समुच्चयः(ext2_filsys fs, संरचना mmp_struct *mmp)
{
	__u32 crc;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_गन्तव्यम्_le32(crc);

	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_verify_csum_type(ext2_filsys fs, संरचना ext2_super_खण्डम् *sb)
{
	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	प्रत्यागम sb->s_checksum_type == EXT2_चक्रीयपरीक्षा_32_C_CHKSUM;
}

स्थिर __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((अप्रयुक्तम्)),
				    संरचना ext2_super_खण्डम् *sb)
{
	पूर्णाङ्क विस्थापनम् = offsetof(संरचना ext2_super_खण्डम्, s_checksum);

	प्रत्यागम ext2fs_crc32c_le(~0, (अचिह्नित वर्ण *)sb, विस्थापनम्);
}

/* NOTE: The input to this function MUST be in LE order */
पूर्णाङ्क ext2fs_superblock_csum_verify(ext2_filsys fs, संरचना ext2_super_खण्डम् *sb)
{
	__u32 flag, calculated;

	यदि (fs->flags & EXT2_FLAG_SWAP_अष्टकानि)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	अन्यथा
		flag = ext2fs_cpu_गन्तव्यम्_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	यदि (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		प्रत्यागम 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	प्रत्यागम ext2fs_le32_गन्तव्यम्_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_समुच्चयः(ext2_filsys fs,
				     संरचना ext2_super_खण्डम् *sb)
{
	__u32 flag, crc;

	यदि (fs->flags & EXT2_FLAG_SWAP_अष्टकानि)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	अन्यथा
		flag = ext2fs_cpu_गन्तव्यम्_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	यदि (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		प्रत्यागम 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_गन्तव्यम्_le32(crc);

	प्रत्यागम 0;
}

स्थिर errcode_t ext2fs_ext_attr_खण्डम्_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((अप्रयुक्तम्)),
					    blk64_t खण्डम्,
					    संरचना ext2_ext_attr_शीर्षकम् *hdr,
					    __u32 *crc)
{
	वर्ण *buf = (वर्ण *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	खण्डम् = ext2fs_cpu_गन्तव्यम्_le64(खण्डम्);
	*crc = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)&खण्डम्,
				परिमाण(खण्डम्));
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_ext_attr_खण्डम्_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t खण्डम्,
				      संरचना ext2_ext_attr_शीर्षकम् *hdr)
{
	__u32 calculated;
	errcode_t retval;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	retval = ext2fs_ext_attr_खण्डम्_csum(fs, inum, खण्डम्, hdr, &calculated);
	यदि (retval)
		प्रत्यागम 0;

	प्रत्यागम ext2fs_le32_गन्तव्यम्_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_खण्डम्_csum_समुच्चयः(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t खण्डम्,
					 संरचना ext2_ext_attr_शीर्षकम् *hdr)
{
	errcode_t retval;
	__u32 crc;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	retval = ext2fs_ext_attr_खण्डम्_csum(fs, inum, खण्डम्, hdr, &crc);
	यदि (retval)
		प्रत्यागम retval;
	hdr->h_checksum = ext2fs_cpu_गन्तव्यम्_le32(crc);
	प्रत्यागम 0;
}

स्थिर __u16 do_nothing16(__u16 x)
{
	प्रत्यागम x;
}

स्थिर __u16 disk_गन्तव्यम्_host16(__u16 x)
{
	प्रत्यागम ext2fs_le16_गन्तव्यम्_cpu(x);
}

स्थिर errcode_t __get_dx_countlimit(ext2_filsys fs,
				     संरचना ext2_dir_प्रविष्टिः *dirent,
				     संरचना ext2_dx_countlimit **cc,
				     पूर्णाङ्क *विस्थापनम्,
				     पूर्णाङ्क need_swab)
{
	संरचना ext2_dir_प्रविष्टिः *dp;
	संरचना ext2_dx_मूलम्_info *मूलम्;
	संरचना ext2_dx_countlimit *c;
	पूर्णाङ्क गणनम्_विस्थापनम्, max_sane_entries;
	अचिह्नित पूर्णाङ्क rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_गन्तव्यम्_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	यदि (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		गणनम्_विस्थापनम् = 8;
	अन्यथा यदि (rec_len == 12) {
		dp = (संरचना ext2_dir_प्रविष्टिः *)(((वर्ण *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		यदि (rec_len != fs->blocksize - 12)
			प्रत्यागम EXT2_ET_DB_NOT_प्राप्तम्;
		मूलम् = (संरचना ext2_dx_मूलम्_info *)(((वर्ण *)dp + 12));
		यदि (मूलम्->reserved_zero ||
		    मूलम्->info_दीर्घता != परिमाण(संरचना ext2_dx_मूलम्_info))
			प्रत्यागम EXT2_ET_DB_NOT_प्राप्तम्;
		गणनम्_विस्थापनम् = 32;
	} अन्यथा
		प्रत्यागम EXT2_ET_DB_NOT_प्राप्तम्;

	c = (संरचना ext2_dx_countlimit *)(((वर्ण *)dirent) + गणनम्_विस्थापनम्);
	max_sane_entries = (fs->blocksize - गणनम्_विस्थापनम्) /
			   परिमाण(संरचना ext2_dx_प्रविष्टिः);
	यदि (ext2fs_le16_गन्तव्यम्_cpu(c->सीमा) > max_sane_entries ||
	    ext2fs_le16_गन्तव्यम्_cpu(c->गणनम्) > max_sane_entries)
		प्रत्यागम EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	यदि (विस्थापनम्)
		*विस्थापनम् = गणनम्_विस्थापनम्;
	यदि (cc)
		*cc = c;

	प्रत्यागम 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   संरचना ext2_dir_प्रविष्टिः *dirent,
				   संरचना ext2_dx_countlimit **cc,
				   पूर्णाङ्क *विस्थापनम्)
{
	प्रत्यागम __get_dx_countlimit(fs, dirent, cc, विस्थापनम्, 0);
}

शून्य ext2fs_initialize_dirent_पुच्छ(ext2_filsys fs,
				   संरचना ext2_dir_प्रविष्टिः_पुच्छ *t)
{
	स्मृतिं_पूरय(t, 0, परिमाण(संरचना ext2_dir_प्रविष्टिः_पुच्छ));
	ext2fs_समुच्चयः_rec_len(fs, परिमाण(संरचना ext2_dir_प्रविष्टिः_पुच्छ),
			   (संरचना ext2_dir_प्रविष्टिः *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

स्थिर errcode_t __get_dirent_tail(ext2_filsys fs,
				   संरचना ext2_dir_प्रविष्टिः *dirent,
				   संरचना ext2_dir_प्रविष्टिः_पुच्छ **tt,
				   पूर्णाङ्क need_swab)
{
	संरचना ext2_dir_प्रविष्टिः *d;
	शून्य *शिखरम्;
	संरचना ext2_dir_प्रविष्टिः_पुच्छ *t;
	अचिह्नित पूर्णाङ्क rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_गन्तव्यम्_host16 : do_nothing16);

	यदि (fs->blocksize < 1024)
		प्रत्यागम EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	शिखरम् = EXT2_DIRENT_पुच्छ(dirent, fs->blocksize);

	यावत् ((शून्य *) d < शिखरम्) {
		rec_len = translate(d->rec_len);
		यदि ((rec_len < 8) || (rec_len & 0x03))
			प्रत्यागम EXT2_ET_DIR_CORRUPTED;
		d = (संरचना ext2_dir_प्रविष्टिः *)(((वर्ण *)d) + rec_len);
	}

	यदि ((वर्ण *)d > ((वर्ण *)dirent + fs->blocksize))
			प्रत्यागम EXT2_ET_DIR_CORRUPTED;
	यदि (d != शिखरम्)
		प्रत्यागम EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (संरचना ext2_dir_प्रविष्टिः_पुच्छ *)d;
	यदि (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != परिमाण(संरचना ext2_dir_प्रविष्टिः_पुच्छ) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		प्रत्यागम EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	यदि (tt)
		*tt = t;
	प्रत्यागम retval;
}

पूर्णाङ्क ext2fs_dirent_has_पुच्छ(ext2_filsys fs, संरचना ext2_dir_प्रविष्टिः *dirent)
{
	प्रत्यागम __get_dirent_tail(fs, dirent, रिक्त, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

स्थिर errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_dir_प्रविष्टिः *dirent, __u32 *crc,
				    पूर्णाङ्क आकार)
{
	errcode_t retval;
	वर्ण *buf = (वर्ण *)dirent;
	__u32 gen;
	संरचना ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	यदि (retval)
		प्रत्यागम retval;

	inum = ext2fs_cpu_गन्तव्यम्_le32(inum);
	gen = ext2fs_cpu_गन्तव्यम्_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)&inum,
				परिमाण(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)&gen, परिमाण(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)buf, आकार);

	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      संरचना ext2_dir_प्रविष्टिः *dirent)
{
	errcode_t retval;
	__u32 calculated;
	संरचना ext2_dir_प्रविष्टिः_पुच्छ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	यदि (retval)
		प्रत्यागम 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (वर्ण *)t - (वर्ण *)dirent);
	यदि (retval)
		प्रत्यागम 0;
	प्रत्यागम ext2fs_le32_गन्तव्यम्_cpu(t->det_checksum) == calculated;
}

स्थिर errcode_t ext2fs_dirent_csum_समुच्चयः(ext2_filsys fs, ext2_ino_t inum,
					संरचना ext2_dir_प्रविष्टिः *dirent)
{
	errcode_t retval;
	__u32 crc;
	संरचना ext2_dir_प्रविष्टिः_पुच्छ *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	यदि (retval)
		प्रत्यागम retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (वर्ण *)t - (वर्ण *)dirent);
	यदि (retval)
		प्रत्यागम retval;
	t->det_checksum = ext2fs_cpu_गन्तव्यम्_le32(crc);
	प्रत्यागम 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 संरचना ext2_dir_प्रविष्टिः *dirent,
			 __u32 *crc, संरचना ext2_dx_पुच्छ **ret_t)
{
	errcode_t retval;
	वर्ण *buf = (वर्ण *)dirent;
	पूर्णाङ्क आकार;
	__u32 gen, dummy_csum = 0;
	संरचना ext2_inode inode;
	संरचना ext2_dx_पुच्छ *t;
	संरचना ext2_dx_countlimit *c;
	पूर्णाङ्क गणनम्_विस्थापनम्, सीमा, गणनम्;

	retval = __get_dx_countlimit(fs, dirent, &c, &गणनम्_विस्थापनम्, 1);
	यदि (retval)
		प्रत्यागम retval;
	सीमा = ext2fs_le16_गन्तव्यम्_cpu(c->सीमा);
	गणनम् = ext2fs_le16_गन्तव्यम्_cpu(c->गणनम्);
	यदि (गणनम्_विस्थापनम् + (सीमा * परिमाण(संरचना ext2_dx_प्रविष्टिः)) >
	    fs->blocksize - परिमाण(संरचना ext2_dx_पुच्छ))
		प्रत्यागम EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (संरचना ext2_dx_पुच्छ *)(((संरचना ext2_dx_प्रविष्टिः *)c) + सीमा);

	आकार = गणनम्_विस्थापनम् + (गणनम् * परिमाण(संरचना ext2_dx_प्रविष्टिः));

	retval = ext2fs_read_inode(fs, inum, &inode);
	यदि (retval)
		प्रत्यागम retval;

	inum = ext2fs_cpu_गन्तव्यम्_le32(inum);
	gen = ext2fs_cpu_गन्तव्यम्_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)&inum,
				परिमाण(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)&gen, परिमाण(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)buf, आकार);
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)&dummy_csum, 4);

	यदि (ret_t)
		*ret_t = t;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 संरचना ext2_dir_प्रविष्टिः *dirent)
{
	__u32 calculated;
	errcode_t retval;
	संरचना ext2_dx_पुच्छ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	यदि (retval)
		प्रत्यागम 0;

	प्रत्यागम ext2fs_le32_गन्तव्यम्_cpu(t->dt_checksum) == calculated;
}

स्थिर errcode_t ext2fs_dx_csum_समुच्चयः(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_dir_प्रविष्टिः *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	संरचना ext2_dx_पुच्छ *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	यदि (retval)
		प्रत्यागम retval;
	t->dt_checksum = ext2fs_cpu_गन्तव्यम्_le32(crc);
	प्रत्यागम retval;
}

पूर्णाङ्क ext2fs_dir_खण्डम्_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 संरचना ext2_dir_प्रविष्टिः *dirent)
{
	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	यदि (__get_dirent_tail(fs, dirent, रिक्त, 1) == 0)
		प्रत्यागम ext2fs_dirent_csum_verify(fs, inum, dirent);
	यदि (__get_dx_countlimit(fs, dirent, रिक्त, रिक्त, 1) == 0)
		प्रत्यागम ext2fs_dx_csum_verify(fs, inum, dirent);

	प्रत्यागम 0;
}

errcode_t ext2fs_dir_खण्डम्_csum_समुच्चयः(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext2_dir_प्रविष्टिः *dirent)
{
	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	यदि (__get_dirent_tail(fs, dirent, रिक्त, 1) == 0)
		प्रत्यागम ext2fs_dirent_csum_समुच्चयः(fs, inum, dirent);
	यदि (__get_dx_countlimit(fs, dirent, रिक्त, रिक्त, 1) == 0)
		प्रत्यागम ext2fs_dx_csum_समुच्चयः(fs, inum, dirent);

	यदि (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		प्रत्यागम 0;
	प्रत्यागम EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#परिभाषय EXT3_EXTENT_पुच्छ_विस्थापनम्(hdr)	(परिमाण(संरचना ext3_extent_शीर्षकम्) + \
	(परिमाण(संरचना ext3_extent) * ext2fs_le16_गन्तव्यम्_cpu((hdr)->eh_max)))

स्थिर संरचना ext3_extent_पुच्छ *get_extent_पुच्छ(संरचना ext3_extent_शीर्षकम् *h)
{
	प्रत्यागम (संरचना ext3_extent_पुच्छ *)(((वर्ण *)h) +
					   EXT3_EXTENT_पुच्छ_विस्थापनम्(h));
}

स्थिर errcode_t ext2fs_extent_खण्डम्_csum(ext2_filsys fs, ext2_ino_t inum,
					  संरचना ext3_extent_शीर्षकम् *eh,
					  __u32 *crc)
{
	पूर्णाङ्क आकार;
	__u32 gen;
	errcode_t retval;
	संरचना ext2_inode inode;

	आकार = EXT3_EXTENT_पुच्छ_विस्थापनम्(eh) + offsetof(संरचना ext3_extent_पुच्छ,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	यदि (retval)
		प्रत्यागम retval;
	inum = ext2fs_cpu_गन्तव्यम्_le32(inum);
	gen = ext2fs_cpu_गन्तव्यम्_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)&inum,
				परिमाण(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)&gen, परिमाण(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)eh, आकार);

	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_extent_खण्डम्_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    संरचना ext3_extent_शीर्षकम् *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	संरचना ext3_extent_पुच्छ *t = get_extent_पुच्छ(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	provided = ext2fs_le32_गन्तव्यम्_cpu(t->et_checksum);
	retval = ext2fs_extent_खण्डम्_csum(fs, inum, eh, &calculated);
	यदि (retval)
		प्रत्यागम 0;

	प्रत्यागम provided == calculated;
}

errcode_t ext2fs_extent_खण्डम्_csum_समुच्चयः(ext2_filsys fs, ext2_ino_t inum,
				       संरचना ext3_extent_शीर्षकम् *eh)
{
	errcode_t retval;
	__u32 crc;
	संरचना ext3_extent_पुच्छ *t = get_extent_पुच्छ(eh);

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_खण्डम्_csum(fs, inum, eh, &crc);
	यदि (retval)
		प्रत्यागम retval;
	t->et_checksum = ext2fs_cpu_गन्तव्यम्_le32(crc);
	प्रत्यागम retval;
}

पूर्णाङ्क ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t गणः,
				    वर्ण *bitmap, पूर्णाङ्क आकार)
{
	संरचना ext4_गणः_desc *gdp = (संरचना ext4_गणः_desc *)
			ext2fs_गणः_desc(fs, fs->गणः_desc, गणः);
	__u32 provided, calculated;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)bitmap,
				      आकार);
	यदि (EXT2_DESC_आकार(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_अन्तः)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	अन्यथा
		calculated &= 0xFFFF;

	प्रत्यागम provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
				       वर्ण *bitmap, पूर्णाङ्क आकार)
{
	__u32 crc;
	संरचना ext4_गणः_desc *gdp = (संरचना ext4_गणः_desc *)
			ext2fs_गणः_desc(fs, fs->गणः_desc, गणः);

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	crc = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)bitmap, आकार);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	यदि (EXT2_DESC_आकार(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_अन्तः)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_खण्डम्_bitmap_csum_verify(ext2_filsys fs, dgrp_t गणः,
				    वर्ण *bitmap, पूर्णाङ्क आकार)
{
	संरचना ext4_गणः_desc *gdp = (संरचना ext4_गणः_desc *)
			ext2fs_गणः_desc(fs, fs->गणः_desc, गणः);
	__u32 provided, calculated;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;
	provided = gdp->bg_खण्डम्_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)bitmap,
				      आकार);
	यदि (EXT2_DESC_आकार(fs->super) >= EXT4_BG_खण्डम्_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_खण्डम्_bitmap_csum_hi << 16;
	अन्यथा
		calculated &= 0xFFFF;

	प्रत्यागम provided == calculated;
}

errcode_t ext2fs_खण्डम्_bitmap_csum_समुच्चयः(ext2_filsys fs, dgrp_t गणः,
				       वर्ण *bitmap, पूर्णाङ्क आकार)
{
	__u32 crc;
	संरचना ext4_गणः_desc *gdp = (संरचना ext4_गणः_desc *)
			ext2fs_गणः_desc(fs, fs->गणः_desc, गणः);

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	crc = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)bitmap, आकार);
	gdp->bg_खण्डम्_bitmap_csum_lo = crc & 0xFFFF;
	यदि (EXT2_DESC_आकार(fs->super) >= EXT4_BG_खण्डम्_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_खण्डम्_bitmap_csum_hi = crc >> 16;

	प्रत्यागम 0;
}

स्थिर errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       संरचना ext2_inode_large *inode,
			       __u32 *crc, पूर्णाङ्क has_hi)
{
	__u32 gen;
	संरचना ext2_inode_large *desc = inode;
	size_t आकार = EXT2_INODE_आकार(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	यदि (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_गन्तव्यम्_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)&inum,
				परिमाण(inum));
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)&gen, परिमाण(gen));
	*crc = ext2fs_crc32c_le(*crc, (अचिह्नित वर्ण *)desc, आकार);

	inode->i_checksum_lo = old_lo;
	यदि (has_hi)
		inode->i_checksum_hi = old_hi;
	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     संरचना ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	अचिह्नित पूर्णाङ्क i, has_hi;
	वर्ण *cp;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;

	has_hi = (EXT2_INODE_आकार(fs->super) > EXT2_GOOD_OLD_INODE_आकार &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_अन्तः);

	provided = ext2fs_le16_गन्तव्यम्_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	यदि (retval)
		प्रत्यागम 0;
	यदि (has_hi) {
		__u32 hi = ext2fs_le16_गन्तव्यम्_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} अन्यथा
		calculated &= 0xFFFF;

	यदि (provided == calculated)
		प्रत्यागम 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	प्रति (cp = (वर्ण *) inode, i = 0;
	     i < परिमाण(संरचना ext2_inode);
	     cp++, i++)
		यदि (*cp)
			प्रत्यागम 0;
	प्रत्यागम 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_समुच्चयः(ext2_filsys fs, ext2_ino_t inum,
			   संरचना ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	पूर्णाङ्क has_hi;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	has_hi = (EXT2_INODE_आकार(fs->super) > EXT2_GOOD_OLD_INODE_आकार &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_अन्तः);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	यदि (retval)
		प्रत्यागम retval;
	inode->i_checksum_lo = ext2fs_cpu_गन्तव्यम्_le16(crc & 0xFFFF);
	यदि (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_गन्तव्यम्_le16(crc >> 16);
	प्रत्यागम 0;
}

__u16 ext2fs_गणः_desc_csum(ext2_filsys fs, dgrp_t गणः)
{
	संरचना ext2_गणः_desc *desc = ext2fs_गणः_desc(fs, fs->गणः_desc,
							 गणः);
	size_t विस्थापनम्, आकार = EXT2_DESC_आकार(fs->super);
	__u16 crc = 0;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	संरचना ext4_गणः_desc swabdesc;
	size_t save_आकार = आकार;
	ध्रुव size_t ext4_bg_आकार = परिमाण(संरचना ext4_गणः_desc);
	संरचना ext2_गणः_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	यदि (आकार > ext4_bg_आकार)
		आकार = ext4_bg_आकार;
	स्मृतिं_प्रतिलिख(&swabdesc, desc, आकार);
	ext2fs_swap_गणः_desc2(fs, (संरचना ext2_गणः_desc *) &swabdesc);
	desc = (संरचना ext2_गणः_desc *) &swabdesc;
	गणः = ext2fs_swab32(गणः);
#यदि_अन्तः

	यदि (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 चक्रीयपरीक्षा_32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		चक्रीयपरीक्षा_32 = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)&गणः,
					 परिमाण(गणः));
		चक्रीयपरीक्षा_32 = ext2fs_crc32c_le(चक्रीयपरीक्षा_32, (अचिह्नित वर्ण *)desc,
					 आकार);
		desc->bg_checksum = old_crc;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
		यदि (save_आकार > ext4_bg_आकार)
			चक्रीयपरीक्षा_32 = ext2fs_crc32c_le(चक्रीयपरीक्षा_32,
				     (अचिह्नित वर्ण *)save_desc + ext4_bg_आकार,
	                             save_आकार - ext4_bg_आकार);
#यदि_अन्तः
		crc = चक्रीयपरीक्षा_32 & 0xFFFF;
		गच्छ बहिर्गमनम्;
	}

	/* old crc16 code */
	विस्थापनम् = offsetof(संरचना ext2_गणः_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   परिमाण(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &गणः, परिमाण(गणः));
	crc = ext2fs_crc16(crc, desc, विस्थापनम्);
	विस्थापनम् += परिमाण(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	यदि (विस्थापनम् < आकार) {
		crc = ext2fs_crc16(crc, (वर्ण *)desc + विस्थापनम्,
				   आकार - विस्थापनम्);
	}
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	यदि (save_आकार > ext4_bg_आकार)
		crc = ext2fs_crc16(crc, (वर्ण *)save_desc + ext4_bg_आकार,
				   save_आकार - ext4_bg_आकार);
#यदि_अन्तः

बहिर्गमनम्:
	प्रत्यागम crc;
}

पूर्णाङ्क ext2fs_गणः_desc_csum_verify(ext2_filsys fs, dgrp_t गणः)
{
	यदि (ext2fs_has_गणः_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, गणः) !=
	     ext2fs_गणः_desc_csum(fs, गणः)))
		प्रत्यागम 0;

	प्रत्यागम 1;
}

शून्य ext2fs_गणः_desc_csum_समुच्चयः(ext2_filsys fs, dgrp_t गणः)
{
	यदि (!ext2fs_has_गणः_desc_csum(fs))
		प्रत्यागम;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_समुच्चयः(fs, गणः, ext2fs_गणः_desc_csum(fs, गणः));
}

स्थिर __u32 find_चरमः_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_प्रत्येकम्_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, अन्तः_ino;

	start_ino = grp_no * inodes_प्रत्येकम्_grp + 1;
	अन्तः_ino = start_ino + inodes_प्रत्येकम्_grp - 1;

	प्रति (i = अन्तः_ino; i >= start_ino; i--) {
		यदि (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			प्रत्यागम i - start_ino + 1;
	}
	प्रत्यागम inodes_प्रत्येकम्_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_समुच्चयः_gdt_csum(ext2_filsys fs)
{
	संरचना ext2_super_खण्डम् *sb = fs->super;
	पूर्णाङ्क dirty = 0;
	dgrp_t i;

	यदि (!fs->inode_map)
		प्रत्यागम EXT2_ET_NO_INODE_BITMAP;

	यदि (!ext2fs_has_गणः_desc_csum(fs))
		प्रत्यागम 0;

	प्रति (i = 0; i < fs->गणः_desc_गणनम्; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_अप्रयुक्तम् = ext2fs_bg_itable_अप्रयुक्तम्(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_गणनम् = ext2fs_bg_free_inodes_गणनम्(fs, i);
		__u32 old_free_खण्डानि_गणनम् = ext2fs_bg_free_खण्डानि_गणनम्(fs, i);

		यदि (old_free_खण्डानि_गणनम् == sb->s_खण्डानि_प्रत्येकम्_गणः &&
		    i != fs->गणः_desc_गणनम् - 1)
			ext2fs_bg_flags_समुच्चयः(fs, i, EXT2_BG_खण्डम्_UNINIT);

		यदि (old_free_inodes_गणनम् == sb->s_inodes_प्रत्येकम्_गणः) {
			ext2fs_bg_flags_समुच्चयः(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_अप्रयुक्तम्_समुच्चयः(fs, i, sb->s_inodes_प्रत्येकम्_गणः);
		} अन्यथा {
			पूर्णाङ्क अप्रयुक्तम् =
				sb->s_inodes_प्रत्येकम्_गणः -
				find_चरमः_inode_ingrp(fs->inode_map,
						      sb->s_inodes_प्रत्येकम्_गणः, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_अप्रयुक्तम्_समुच्चयः(fs, i, अप्रयुक्तम्);
		}

		ext2fs_गणः_desc_csum_समुच्चयः(fs, i);
		यदि (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		यदि (old_अप्रयुक्तम् != ext2fs_bg_itable_अप्रयुक्तम्(fs, i))
			dirty = 1;
		यदि (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	यदि (dirty)
		ext2fs_mark_super_dirty(fs);
	प्रत्यागम 0;
}

#यदि_परिभाषितम् DEBUG
#अन्तर्भाव "e2p/e2p.h"

शून्य print_csum(ध्रुव वर्ण *सन्देशः, ext2_filsys fs, dgrp_t गणः)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	संरचना ext2_गणः_desc *desc = ext2fs_गणः_desc(fs, fs->गणः_desc,
							 गणः);
	size_t आकार = EXT2_DESC_आकार(fs->super);
	संरचना ext2_super_खण्डम् *sb = fs->super;
	पूर्णाङ्क विस्थापनम् = offsetof(संरचना ext2_गणः_desc, bg_checksum);
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	संरचना ext4_गणः_desc swabdesc;
	संरचना ext2_गणः_desc *save_desc = desc;
	ध्रुव size_t ext4_bg_आकार = परिमाण(संरचना ext4_गणः_desc);
	size_t save_आकार = आकार;
#यदि_अन्तः

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	यदि (आकार > ext4_bg_आकार)
		आकार = ext4_bg_आकार;
	स्मृतिं_प्रतिलिख(&swabdesc, desc, आकार);
	ext2fs_swap_गणः_desc2(fs, (संरचना ext2_गणः_desc *) &swabdesc);
	desc = (संरचना ext2_गणः_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(गणः);
#अन्यथा
	swabgroup = गणः;
#यदि_अन्तः

	crc1 = ext2fs_crc16(~0, sb->s_uuid, परिमाण(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, परिमाण(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, विस्थापनम्);
	विस्थापनम् += परिमाण(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	यदि (विस्थापनम् < आकार)
		crc3 = ext2fs_crc16(crc3, (वर्ण *)desc + विस्थापनम्, आकार - विस्थापनम्);
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	यदि (save_आकार > ext4_bg_आकार)
		crc3 = ext2fs_crc16(crc3, (वर्ण *)save_desc + ext4_bg_आकार,
				    save_आकार - ext4_bg_आकार);
#यदि_अन्तः

	प्रारूप_लिख("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       सन्देशः, e2p_uuid2str(sb->s_uuid), crc1, गणः, crc2, crc3,
	       ext2fs_गणः_desc_csum(fs, गणः));
}

अचिह्नित वर्ण sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	संरचना ext2_super_खण्डम् param;
	errcode_t		retval;
	ext2_filsys		fs;
	पूर्णाङ्क			i;
	__u16 csum1, csum2, csum_ज्ञातम् = 0xd3a4;

	स्मृतिं_पूरय(&param, 0, परिमाण(param));
	ext2fs_खण्डानि_गणनम्_समुच्चयः(&param, 32768);
#यदि 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_आकार = 128;
	csum_ज्ञातम् = 0x5b6e;
#यदि_अन्तः

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_द्वयङ्काः, &param,
				   test_io_manager, &fs);
	यदि (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		निर्गच्छ(1);
	}
	स्मृतिं_प्रतिलिख(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	प्रति (i=0; i < fs->गणः_desc_गणनम्; i++) {
		ext2fs_खण्डम्_bitmap_loc_समुच्चयः(fs, i, 124);
		ext2fs_inode_bitmap_loc_समुच्चयः(fs, i, 125);
		ext2fs_inode_सारणी_loc_समुच्चयः(fs, i, 126);
		ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(fs, i, 31119);
		ext2fs_bg_free_inodes_गणनम्_समुच्चयः(fs, i, 15701);
		ext2fs_bg_प्रयुक्तम्_dirs_गणनम्_समुच्चयः(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_गणः_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	यदि (csum1 != csum_ज्ञातम्) {
		प्रारूप_लिख("checksum for group 0 should be %04x\n", csum_ज्ञातम्);
		निर्गच्छ(1);
	}
	csum2 = ext2fs_गणः_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	यदि (csum1 == csum2) {
		प्रारूप_लिख("checksums for different groups shouldn't match\n");
		निर्गच्छ(1);
	}
	csum2 = ext2fs_गणः_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	यदि (csum1 == csum2) {
		प्रारूप_लिख("checksums for different groups shouldn't match\n");
		निर्गच्छ(1);
	}
	ext2fs_bg_checksum_समुच्चयः(fs, 0, csum1);
	csum2 = ext2fs_गणः_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	यदि (csum1 != csum2) {
		प्रारूप_लिख("checksums should not depend on checksum field\n");
		निर्गच्छ(1);
	}
	यदि (!ext2fs_गणः_desc_csum_verify(fs, 0)) {
		प्रारूप_लिख("checksums should verify against gd_checksum\n");
		निर्गच्छ(1);
	}
	स्मृतिं_पूरय(fs->super->s_uuid, 0x30, परिमाण(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	यदि (ext2fs_गणः_desc_csum_verify(fs, 0) != 0) {
		प्रारूप_लिख("checksums for different filesystems shouldn't match\n");
		निर्गच्छ(1);
	}
	csum1 = ext2fs_गणः_desc_csum(fs, 0);
	ext2fs_bg_checksum_समुच्चयः(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(fs, 0, 1);
	csum2 = ext2fs_गणः_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	यदि (csum1 == csum2) {
		प्रारूप_लिख("checksums for different data shouldn't match\n");
		निर्गच्छ(1);
	}
	ext2fs_free(fs);

	प्रत्यागम 0;
}
#यदि_अन्तः
