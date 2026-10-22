/*
 * swapfs.c --- swap ext2 filesystem data structures
 *
 * Copyright (C) 1995, 1996, 2002 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव <ext2fs/ext2_ext_attr.h>

शून्य ext2fs_swap_super(संरचना ext2_super_खण्डम् * sb)
{
	पूर्णाङ्क i;

	sb->s_inodes_गणनम् = ext2fs_swab32(sb->s_inodes_गणनम्);
	sb->s_खण्डानि_गणनम् = ext2fs_swab32(sb->s_खण्डानि_गणनम्);
	sb->s_r_खण्डानि_गणनम् = ext2fs_swab32(sb->s_r_खण्डानि_गणनम्);
	sb->s_free_खण्डानि_गणनम् = ext2fs_swab32(sb->s_free_खण्डानि_गणनम्);
	sb->s_free_inodes_गणनम् = ext2fs_swab32(sb->s_free_inodes_गणनम्);
	sb->s_first_दत्त_खण्डम् = ext2fs_swab32(sb->s_first_दत्त_खण्डम्);
	sb->s_log_खण्डम्_आकार = ext2fs_swab32(sb->s_log_खण्डम्_आकार);
	sb->s_log_cluster_आकार = ext2fs_swab32(sb->s_log_cluster_आकार);
	sb->s_खण्डानि_प्रत्येकम्_गणः = ext2fs_swab32(sb->s_खण्डानि_प्रत्येकम्_गणः);
	sb->s_clusters_प्रत्येकम्_गणः = ext2fs_swab32(sb->s_clusters_प्रत्येकम्_गणः);
	sb->s_inodes_प्रत्येकम्_गणः = ext2fs_swab32(sb->s_inodes_प्रत्येकम्_गणः);
	sb->s_mtime = ext2fs_swab32(sb->s_mtime);
	sb->s_wtime = ext2fs_swab32(sb->s_wtime);
	sb->s_mnt_गणनम् = ext2fs_swab16(sb->s_mnt_गणनम्);
	sb->s_max_mnt_गणनम् = ext2fs_swab16(sb->s_max_mnt_गणनम्);
	sb->s_magic = ext2fs_swab16(sb->s_magic);
	sb->s_state = ext2fs_swab16(sb->s_state);
	sb->s_errors = ext2fs_swab16(sb->s_errors);
	sb->s_minor_rev_level = ext2fs_swab16(sb->s_minor_rev_level);
	sb->s_lastcheck = ext2fs_swab32(sb->s_lastcheck);
	sb->s_checkinterval = ext2fs_swab32(sb->s_checkinterval);
	sb->s_creator_os = ext2fs_swab32(sb->s_creator_os);
	sb->s_rev_level = ext2fs_swab32(sb->s_rev_level);
	sb->s_def_resuid = ext2fs_swab16(sb->s_def_resuid);
	sb->s_def_resgid = ext2fs_swab16(sb->s_def_resgid);
	sb->s_first_ino = ext2fs_swab32(sb->s_first_ino);
	sb->s_inode_आकार = ext2fs_swab16(sb->s_inode_आकार);
	sb->s_खण्डम्_गणः_nr = ext2fs_swab16(sb->s_खण्डम्_गणः_nr);
	sb->s_feature_compat = ext2fs_swab32(sb->s_feature_compat);
	sb->s_feature_incompat = ext2fs_swab32(sb->s_feature_incompat);
	sb->s_feature_ro_compat = ext2fs_swab32(sb->s_feature_ro_compat);
	/* sb->s_uuid is __u8 and does not need swabbing */
	/* sb->s_volume_name is char and does not need swabbing */
	/* sb->s_last_mounted is char and does not need swabbing */
	sb->s_algorithm_usage_bitmap = ext2fs_swab32(sb->s_algorithm_usage_bitmap);
	/* sb->s_prealloc_blocks is __u8 and does not need swabbing */
	/* sb->s_prealloc_dir_blocks is __u8 and does not need swabbing */
	sb->s_reserved_gdt_खण्डानि = ext2fs_swab16(sb->s_reserved_gdt_खण्डानि);
	/* sb->s_journal_uuid is __u8 and does not need swabbing */
	sb->s_journal_inum = ext2fs_swab32(sb->s_journal_inum);
	sb->s_journal_dev = ext2fs_swab32(sb->s_journal_dev);
	sb->s_चरमः_orphan = ext2fs_swab32(sb->s_चरमः_orphan);
	प्रति (i = 0; i < 4; i++)
		sb->s_hash_बीजम्[i] = ext2fs_swab32(sb->s_hash_बीजम्[i]);
	/* sb->s_def_hash_version is __u8 and does not need swabbing */
	/* sb->s_jnl_backup_type is __u8 and does not need swabbing */
	sb->s_desc_आकार = ext2fs_swab16(sb->s_desc_आकार);
	sb->s_default_mount_opts = ext2fs_swab32(sb->s_default_mount_opts);
	sb->s_first_meta_bg = ext2fs_swab32(sb->s_first_meta_bg);
	sb->s_mkfs_time = ext2fs_swab32(sb->s_mkfs_time);
	/* if journal backup is for a valid extent-based journal... */
	यदि (ext2fs_extent_शीर्षकम्_verify(sb->s_jnl_खण्डानि,
					परिमाण(sb->s_jnl_खण्डानि)) == 0) {
		/* ... swap only the journal i_size and i_size_high,
		 * and the extent data is not swapped on read */
		i = 15;
	} अन्यथा {
		/* direct/indirect journal: swap it all */
		i = 0;
	}
	प्रति (; i < 17; i++)
		sb->s_jnl_खण्डानि[i] = ext2fs_swab32(sb->s_jnl_खण्डानि[i]);
	sb->s_खण्डानि_गणनम्_hi = ext2fs_swab32(sb->s_खण्डानि_गणनम्_hi);
	sb->s_r_खण्डानि_गणनम्_hi = ext2fs_swab32(sb->s_r_खण्डानि_गणनम्_hi);
	sb->s_free_खण्डानि_hi = ext2fs_swab32(sb->s_free_खण्डानि_hi);
	sb->s_min_extra_isize = ext2fs_swab16(sb->s_min_extra_isize);
	sb->s_want_extra_isize = ext2fs_swab16(sb->s_want_extra_isize);
	sb->s_flags = ext2fs_swab32(sb->s_flags);
	sb->s_raid_stride = ext2fs_swab16(sb->s_raid_stride);
	sb->s_mmp_update_interval = ext2fs_swab16(sb->s_mmp_update_interval);
	sb->s_mmp_खण्डम् = ext2fs_swab64(sb->s_mmp_खण्डम्);
	sb->s_raid_stripe_विस्तारः = ext2fs_swab32(sb->s_raid_stripe_विस्तारः);
	/* sb->s_log_groups_per_flex is __u8 and does not need swabbing */
	/* sb->s_checksum_type is __u8 and does not need swabbing */
	/* sb->s_encryption_level is __u8 and does not need swabbing */
	/* sb->s_reserved_pad is __u8 and does not need swabbing */
	sb->s_kbytes_written = ext2fs_swab64(sb->s_kbytes_written);
	sb->s_snapshot_inum = ext2fs_swab32(sb->s_snapshot_inum);
	sb->s_snapshot_id = ext2fs_swab32(sb->s_snapshot_id);
	sb->s_snapshot_r_खण्डानि_गणनम् =
		ext2fs_swab64(sb->s_snapshot_r_खण्डानि_गणनम्);
	sb->s_snapshot_list = ext2fs_swab32(sb->s_snapshot_list);
	sb->s_error_गणनम् = ext2fs_swab32(sb->s_error_गणनम्);
	sb->s_first_error_time = ext2fs_swab32(sb->s_first_error_time);
	sb->s_first_error_ino = ext2fs_swab32(sb->s_first_error_ino);
	sb->s_first_error_खण्डम् = ext2fs_swab64(sb->s_first_error_खण्डम्);
	/* sb->s_first_error_func is __u8 and does not need swabbing */
	sb->s_चरमः_error_time = ext2fs_swab32(sb->s_चरमः_error_time);
	sb->s_चरमः_error_ino = ext2fs_swab32(sb->s_चरमः_error_ino);
	sb->s_चरमः_error_खण्डम् = ext2fs_swab64(sb->s_चरमः_error_खण्डम्);
	/* sb->s_last_error_func is __u8 and does not need swabbing */
	/* sb->s_mount_opts is __u8 and does not need swabbing */
	sb->s_usr_quota_inum = ext2fs_swab32(sb->s_usr_quota_inum);
	sb->s_grp_quota_inum = ext2fs_swab32(sb->s_grp_quota_inum);
	sb->s_overhead_clusters = ext2fs_swab32(sb->s_overhead_clusters);
	sb->s_backup_bgs[0] = ext2fs_swab32(sb->s_backup_bgs[0]);
	sb->s_backup_bgs[1] = ext2fs_swab32(sb->s_backup_bgs[1]);
	/* sb->s_encrypt_algos is __u8 and does not need swabbing */
	/* sb->s_encrypt_pw_salt is __u8 and does not need swabbing */
	sb->s_lpf_ino = ext2fs_swab32(sb->s_lpf_ino);
	sb->s_prj_quota_inum = ext2fs_swab32(sb->s_prj_quota_inum);
	sb->s_checksum_बीजम् = ext2fs_swab32(sb->s_checksum_बीजम्);
	/* s_*_time_hi are __u8 and does not need swabbing */
	sb->s_encoding = ext2fs_swab16(sb->s_encoding);
	sb->s_encoding_flags = ext2fs_swab16(sb->s_encoding_flags);
	sb->s_orphan_file_inum = ext2fs_swab32(sb->s_orphan_file_inum);
	/* catch when new fields are used from s_reserved */
	EXT2FS_BUILD_BUG_ON(परिमाण(sb->s_reserved) != 94 * परिमाण(__le32));
	sb->s_checksum = ext2fs_swab32(sb->s_checksum);
}

शून्य ext2fs_swap_गणः_desc2(ext2_filsys fs, संरचना ext2_गणः_desc *gdp)
{
	संरचना ext4_गणः_desc *gdp4 = (संरचना ext4_गणः_desc *)gdp;

	/* Do the 32-bit parts first */
	gdp->bg_खण्डम्_bitmap = ext2fs_swab32(gdp->bg_खण्डम्_bitmap);
	gdp->bg_inode_bitmap = ext2fs_swab32(gdp->bg_inode_bitmap);
	gdp->bg_inode_सारणी = ext2fs_swab32(gdp->bg_inode_सारणी);
	gdp->bg_free_खण्डानि_गणनम् = ext2fs_swab16(gdp->bg_free_खण्डानि_गणनम्);
	gdp->bg_free_inodes_गणनम् = ext2fs_swab16(gdp->bg_free_inodes_गणनम्);
	gdp->bg_प्रयुक्तम्_dirs_गणनम् = ext2fs_swab16(gdp->bg_प्रयुक्तम्_dirs_गणनम्);
	gdp->bg_flags = ext2fs_swab16(gdp->bg_flags);
	gdp->bg_exclude_bitmap_lo = ext2fs_swab32(gdp->bg_exclude_bitmap_lo);
	gdp->bg_खण्डम्_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_खण्डम्_bitmap_csum_lo);
	gdp->bg_inode_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_inode_bitmap_csum_lo);
	gdp->bg_itable_अप्रयुक्तम् = ext2fs_swab16(gdp->bg_itable_अप्रयुक्तम्);
	gdp->bg_checksum = ext2fs_swab16(gdp->bg_checksum);
	/* If we're 32-bit, we're done */
	यदि (fs == रिक्त || EXT2_DESC_आकार(fs->super) < EXT2_MIN_DESC_आकार_64_BIT)
		प्रत्यागम;

	/* Swap the 64-bit parts */
	gdp4->bg_खण्डम्_bitmap_hi = ext2fs_swab32(gdp4->bg_खण्डम्_bitmap_hi);
	gdp4->bg_inode_bitmap_hi = ext2fs_swab32(gdp4->bg_inode_bitmap_hi);
	gdp4->bg_inode_सारणी_hi = ext2fs_swab32(gdp4->bg_inode_सारणी_hi);
	gdp4->bg_free_खण्डानि_गणनम्_hi =
		ext2fs_swab16(gdp4->bg_free_खण्डानि_गणनम्_hi);
	gdp4->bg_free_inodes_गणनम्_hi =
		ext2fs_swab16(gdp4->bg_free_inodes_गणनम्_hi);
	gdp4->bg_प्रयुक्तम्_dirs_गणनम्_hi =
		ext2fs_swab16(gdp4->bg_प्रयुक्तम्_dirs_गणनम्_hi);
	gdp4->bg_itable_अप्रयुक्तम्_hi = ext2fs_swab16(gdp4->bg_itable_अप्रयुक्तम्_hi);
	gdp4->bg_exclude_bitmap_hi = ext2fs_swab16(gdp4->bg_exclude_bitmap_hi);
	gdp4->bg_खण्डम्_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_खण्डम्_bitmap_csum_hi);
	gdp4->bg_inode_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_inode_bitmap_csum_hi);
	EXT2FS_BUILD_BUG_ON(परिमाण(gdp4->bg_reserved) != परिमाण(__u32));
}

शून्य ext2fs_swap_गणः_desc(संरचना ext2_गणः_desc *gdp)
{
	ext2fs_swap_गणः_desc2(0, gdp);
}


शून्य ext2fs_swap_ext_attr_शीर्षकम्(संरचना ext2_ext_attr_शीर्षकम् *गन्तव्यम्_शीर्षकम्,
				 संरचना ext2_ext_attr_शीर्षकम् *from_शीर्षकम्)
{
	पूर्णाङ्क n;

	गन्तव्यम्_शीर्षकम्->h_magic    = ext2fs_swab32(from_शीर्षकम्->h_magic);
	गन्तव्यम्_शीर्षकम्->h_खण्डानि   = ext2fs_swab32(from_शीर्षकम्->h_खण्डानि);
	गन्तव्यम्_शीर्षकम्->h_refcount = ext2fs_swab32(from_शीर्षकम्->h_refcount);
	गन्तव्यम्_शीर्षकम्->h_hash     = ext2fs_swab32(from_शीर्षकम्->h_hash);
	गन्तव्यम्_शीर्षकम्->h_checksum = ext2fs_swab32(from_शीर्षकम्->h_checksum);
	प्रति (n = 0; n < 3; n++)
		गन्तव्यम्_शीर्षकम्->h_reserved[n] =
			ext2fs_swab32(from_शीर्षकम्->h_reserved[n]);
}

शून्य ext2fs_swap_ext_attr_प्रविष्टिः(संरचना ext2_ext_attr_प्रविष्टिः *गन्तव्यम्_प्रविष्टिः,
				संरचना ext2_ext_attr_प्रविष्टिः *from_प्रविष्टिः)
{
	गन्तव्यम्_प्रविष्टिः->e_मानम्_offs  = ext2fs_swab16(from_प्रविष्टिः->e_मानम्_offs);
	गन्तव्यम्_प्रविष्टिः->e_मानम्_inum  = ext2fs_swab32(from_प्रविष्टिः->e_मानम्_inum);
	गन्तव्यम्_प्रविष्टिः->e_मानम्_आकार  = ext2fs_swab32(from_प्रविष्टिः->e_मानम्_आकार);
	गन्तव्यम्_प्रविष्टिः->e_hash	= ext2fs_swab32(from_प्रविष्टिः->e_hash);
}

शून्य ext2fs_swap_ext_attr(वर्ण *गन्तव्यम्, वर्ण *from, पूर्णाङ्क bufsize, पूर्णाङ्क has_शीर्षकम्)
{
	संरचना ext2_ext_attr_शीर्षकम् *from_शीर्षकम् =
		(संरचना ext2_ext_attr_शीर्षकम् *)from;
	संरचना ext2_ext_attr_शीर्षकम् *गन्तव्यम्_शीर्षकम् =
		(संरचना ext2_ext_attr_शीर्षकम् *)गन्तव्यम्;
	संरचना ext2_ext_attr_प्रविष्टिः *from_प्रविष्टिः, *गन्तव्यम्_प्रविष्टिः;
	वर्ण *from_अन्तः = (वर्ण *)from_शीर्षकम् + bufsize;

	यदि (गन्तव्यम्_शीर्षकम् != from_शीर्षकम्)
		स्मृतिं_प्रतिलिख(गन्तव्यम्_शीर्षकम्, from_शीर्षकम्, bufsize);

	यदि (has_शीर्षकम्) {
		ext2fs_swap_ext_attr_शीर्षकम्(गन्तव्यम्_शीर्षकम्, from_शीर्षकम्);

		from_प्रविष्टिः = (संरचना ext2_ext_attr_प्रविष्टिः *)(from_शीर्षकम्+1);
		गन्तव्यम्_प्रविष्टिः   = (संरचना ext2_ext_attr_प्रविष्टिः *)(गन्तव्यम्_शीर्षकम्+1);
	} अन्यथा {
		from_प्रविष्टिः = (संरचना ext2_ext_attr_प्रविष्टिः *)from_शीर्षकम्;
		गन्तव्यम्_प्रविष्टिः   = (संरचना ext2_ext_attr_प्रविष्टिः *)गन्तव्यम्_शीर्षकम्;
	}

	यावत् ((वर्ण *)from_प्रविष्टिः < from_अन्तः &&
	       (वर्ण *)EXT2_EXT_ATTR_अग्र(from_प्रविष्टिः) <= from_अन्तः &&
	       *(__u32 *)from_प्रविष्टिः) {
		ext2fs_swap_ext_attr_प्रविष्टिः(गन्तव्यम्_प्रविष्टिः, from_प्रविष्टिः);
		from_प्रविष्टिः = EXT2_EXT_ATTR_अग्र(from_प्रविष्टिः);
		गन्तव्यम्_प्रविष्टिः   = EXT2_EXT_ATTR_अग्र(गन्तव्यम्_प्रविष्टिः);
	}
}

शून्य ext2fs_swap_inode_पूर्णम्(ext2_filsys fs, संरचना ext2_inode_large *t,
			    संरचना ext2_inode_large *f, पूर्णाङ्क hostorder,
			    पूर्णाङ्क bufsize)
{
	अचिह्नित i, extra_isize, attr_magic;
	पूर्णाङ्क has_extents = 0, has_inline_दत्त = 0, islnk = 0, fast_symlink = 0;
	अचिह्नित पूर्णाङ्क inode_आकार;
	__u32 *eaf, *eat;

	/*
	 * Note that t and f may point to the same address. That's why
	 * if (hostorder) condition is executed before swab calls and
	 * if (!hostorder) afterwards.
	 */
	यदि (hostorder) {
		islnk = LINUX_S_ISLNK(f->i_बहुलकम्);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(f));
		has_extents = (f->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_दत्त = (f->i_flags & EXT4_INLINE_दत्त_FL) != 0;
	}

	t->i_बहुलकम् = ext2fs_swab16(f->i_बहुलकम्);
	t->i_uid = ext2fs_swab16(f->i_uid);
	t->i_आकार = ext2fs_swab32(f->i_आकार);
	t->i_atime = ext2fs_swab32(f->i_atime);
	t->i_ctime = ext2fs_swab32(f->i_ctime);
	t->i_mtime = ext2fs_swab32(f->i_mtime);
	t->i_dtime = ext2fs_swab32(f->i_dtime);
	t->i_gid = ext2fs_swab16(f->i_gid);
	t->i_links_गणनम् = ext2fs_swab16(f->i_links_गणनम्);
	t->i_file_acl = ext2fs_swab32(f->i_file_acl);
	t->i_खण्डानि = ext2fs_swab32(f->i_खण्डानि);
	t->i_flags = ext2fs_swab32(f->i_flags);
	t->i_आकार_उच्चः = ext2fs_swab32(f->i_आकार_उच्चः);

	यदि (!hostorder) {
		islnk = LINUX_S_ISLNK(t->i_बहुलकम्);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(t));
		has_extents = (t->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_दत्त = (t->i_flags & EXT4_INLINE_दत्त_FL) != 0;
	}

	/*
	 * Extent data and inline data are swapped on access, not here
	 */
	यदि (!has_extents && !has_inline_दत्त && (!islnk || !fast_symlink)) {
		प्रति (i = 0; i < EXT2_N_खण्डानि; i++)
			t->i_खण्डम्[i] = ext2fs_swab32(f->i_खण्डम्[i]);
	} अन्यथा यदि (t != f) {
		प्रति (i = 0; i < EXT2_N_खण्डानि; i++)
			t->i_खण्डम्[i] = f->i_खण्डम्[i];
	}
	t->i_generation = ext2fs_swab32(f->i_generation);
	t->i_faddr = ext2fs_swab32(f->i_faddr);

	विकल्प (fs->super->s_creator_os) {
	पक्ष EXT2_OS_LINUX:
		t->osd1.linux1.l_i_संस्करणम् =
			ext2fs_swab32(f->osd1.linux1.l_i_संस्करणम्);
		t->osd2.linux2.l_i_खण्डानि_hi =
			ext2fs_swab16(f->osd2.linux2.l_i_खण्डानि_hi);
		t->osd2.linux2.l_i_file_acl_उच्चः =
			ext2fs_swab16(f->osd2.linux2.l_i_file_acl_उच्चः);
		t->osd2.linux2.l_i_uid_उच्चः =
		  ext2fs_swab16 (f->osd2.linux2.l_i_uid_उच्चः);
		t->osd2.linux2.l_i_gid_उच्चः =
		  ext2fs_swab16 (f->osd2.linux2.l_i_gid_उच्चः);
		t->osd2.linux2.l_i_checksum_lo =
			ext2fs_swab16(f->osd2.linux2.l_i_checksum_lo);
		भङ्ग;
	पक्ष EXT2_OS_HURD:
		t->osd1.hurd1.h_i_translator =
		  ext2fs_swab32 (f->osd1.hurd1.h_i_translator);
		t->osd2.hurd2.h_i_frag = f->osd2.hurd2.h_i_frag;
		t->osd2.hurd2.h_i_fsize = f->osd2.hurd2.h_i_fsize;
		t->osd2.hurd2.h_i_बहुलकम्_उच्चः =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_बहुलकम्_उच्चः);
		t->osd2.hurd2.h_i_uid_उच्चः =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_uid_उच्चः);
		t->osd2.hurd2.h_i_gid_उच्चः =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_gid_उच्चः);
		t->osd2.hurd2.h_i_author =
		  ext2fs_swab32 (f->osd2.hurd2.h_i_author);
		भङ्ग;
	स्वभाव:
		भङ्ग;
	}

	यदि (bufsize < (पूर्णाङ्क) (परिमाण(संरचना ext2_inode) + परिमाण(__u16)))
		प्रत्यागम; /* no i_extra_isize field */

	यदि (hostorder)
		extra_isize = f->i_extra_isize;
	t->i_extra_isize = ext2fs_swab16(f->i_extra_isize);
	यदि (!hostorder)
		extra_isize = t->i_extra_isize;
	यदि (extra_isize > EXT2_INODE_आकार(fs->super) -
				परिमाण(संरचना ext2_inode)) {
		/* this is error case: i_extra_size is too large */
		प्रत्यागम;
	}
	यदि (extra_isize & 3)
		प्रत्यागम;		/* Illegal inode extra_isize */

	inode_आकार = EXT2_GOOD_OLD_INODE_आकार + extra_isize;
	यदि (ext2fs_inode_includes(inode_आकार, i_checksum_hi))
		t->i_checksum_hi = ext2fs_swab16(f->i_checksum_hi);
	यदि (ext2fs_inode_includes(inode_आकार, i_ctime_extra))
		t->i_ctime_extra = ext2fs_swab32(f->i_ctime_extra);
	यदि (ext2fs_inode_includes(inode_आकार, i_mtime_extra))
		t->i_mtime_extra = ext2fs_swab32(f->i_mtime_extra);
	यदि (ext2fs_inode_includes(inode_आकार, i_atime_extra))
		t->i_atime_extra = ext2fs_swab32(f->i_atime_extra);
	यदि (ext2fs_inode_includes(inode_आकार, i_crtime))
		t->i_crtime = ext2fs_swab32(f->i_crtime);
	यदि (ext2fs_inode_includes(inode_आकार, i_crtime_extra))
		t->i_crtime_extra = ext2fs_swab32(f->i_crtime_extra);
	यदि (ext2fs_inode_includes(inode_आकार, i_संस्करणम्_hi))
		t->i_संस्करणम्_hi = ext2fs_swab32(f->i_संस्करणम्_hi);
	यदि (ext2fs_inode_includes(inode_आकार, i_projid))
                t->i_projid = ext2fs_swab32(f->i_projid);
	/* catch new static fields added after i_projid */
	EXT2FS_BUILD_BUG_ON(परिमाण(संरचना ext2_inode_large) != 160);

	i = परिमाण(संरचना ext2_inode) + extra_isize + परिमाण(__u32);
	यदि (bufsize < (पूर्णाङ्क) i)
		प्रत्यागम; /* no space for EA magic */

	eaf = (__u32 *) (((वर्ण *) f) + परिमाण(संरचना ext2_inode) +
					extra_isize);

	attr_magic = *eaf;
	यदि (!hostorder)
		attr_magic = ext2fs_swab32(attr_magic);

	यदि (attr_magic != EXT2_EXT_ATTR_MAGIC)
		प्रत्यागम; /* it seems no magic here */

	eat = (__u32 *) (((वर्ण *) t) + परिमाण(संरचना ext2_inode) +
					extra_isize);
	*eat = ext2fs_swab32(*eaf);

	/* convert EA(s) */
	ext2fs_swap_ext_attr((वर्ण *) (eat + 1), (वर्ण *) (eaf + 1),
			     bufsize - परिमाण(संरचना ext2_inode) -
			     extra_isize - परिमाण(__u32), 0);

}

शून्य ext2fs_swap_inode(ext2_filsys fs, संरचना ext2_inode *t,
		       संरचना ext2_inode *f, पूर्णाङ्क hostorder)
{
	ext2fs_swap_inode_पूर्णम्(fs, (संरचना ext2_inode_large *) t,
				(संरचना ext2_inode_large *) f, hostorder,
				परिमाण(संरचना ext2_inode));
}

शून्य ext2fs_swap_mmp(संरचना mmp_struct *mmp)
{
	mmp->mmp_magic = ext2fs_swab32(mmp->mmp_magic);
	mmp->mmp_seq = ext2fs_swab32(mmp->mmp_seq);
	mmp->mmp_time = ext2fs_swab64(mmp->mmp_time);
	mmp->mmp_check_interval = ext2fs_swab16(mmp->mmp_check_interval);
	mmp->mmp_checksum = ext2fs_swab32(mmp->mmp_checksum);
}

errcode_t ext2fs_dirent_swab_अन्तर्(ext2_filsys fs, वर्ण *buf, पूर्णाङ्क flags)
{
	प्रत्यागम ext2fs_dirent_swab_in2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, वर्ण *buf,
				 size_t आकार, पूर्णाङ्क flags)
{
	errcode_t	retval;
	वर्ण		*p, *अन्तः;
	संरचना ext2_dir_प्रविष्टिः *dirent;
	अचिह्नित पूर्णाङ्क	name_len, rec_len, वामः;

	p = (वर्ण *) buf;
	अन्तः = (वर्ण *) buf + आकार;
	वामः = आकार;
	यावत् (p < अन्तः-8) {
		dirent = (संरचना ext2_dir_प्रविष्टिः *) p;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		name_len = dirent->name_len;
		यदि (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		यदि (retval)
			प्रत्यागम retval;
		यदि ((rec_len < 8) || (rec_len % 4)) {
			rec_len = 8;
			यदि (!(fs->flags & EXT2_FLAG_IGNORE_SWAP_DIRENT))
				प्रत्यागम EXT2_ET_DIR_CORRUPTED;
		} अन्यथा यदि (((name_len & 0xFF) + 8) > rec_len)
			यदि (!(fs->flags & EXT2_FLAG_IGNORE_SWAP_DIRENT))
				प्रत्यागम EXT2_ET_DIR_CORRUPTED;
		यदि (rec_len > वामः)
			यदि (!(fs->flags & EXT2_FLAG_IGNORE_SWAP_DIRENT))
				प्रत्यागम EXT2_ET_DIR_CORRUPTED;
		वामः -= rec_len;
		p += rec_len;
	}

	प्रत्यागम 0;
}

errcode_t ext2fs_dirent_swab_बहिर्गमनम्(ext2_filsys fs, वर्ण *buf, पूर्णाङ्क flags)
{
	प्रत्यागम ext2fs_dirent_swab_out2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, वर्ण *buf,
				  size_t आकार, पूर्णाङ्क flags)
{
	errcode_t	retval;
	वर्ण		*p, *अन्तः;
	अचिह्नित पूर्णाङ्क	rec_len;
	संरचना ext2_dir_प्रविष्टिः *dirent;

	p = buf;
	अन्तः = buf + आकार;
	यावत् (p < अन्तः) {
		dirent = (संरचना ext2_dir_प्रविष्टिः *) p;
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		यदि (retval)
			प्रत्यागम retval;
		यदि ((rec_len < 8) ||
		    (rec_len % 4)) {
			ext2fs_free_mem(&buf);
			प्रत्यागम EXT2_ET_DIR_CORRUPTED;
		}
		p += rec_len;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		यदि (rec_len > आकार)
			प्रत्यागम EXT2_ET_DIR_CORRUPTED;
		आकार -= rec_len;

		यदि (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
	}

	प्रत्यागम 0;
}
