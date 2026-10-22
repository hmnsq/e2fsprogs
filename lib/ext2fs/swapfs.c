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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"
#అన్తర్భావ <ext2fs/ext2_ext_attr.h>

శూన్య ext2fs_మార్పిడి_super(నిర్మాణం ext2_super_ఖండం * sb)
{
	పూర్ణాంకం i;

	sb->s_inodes_లెక్క = ext2fs_swab32(sb->s_inodes_లెక్క);
	sb->s_ఖండాలు_లెక్క = ext2fs_swab32(sb->s_ఖండాలు_లెక్క);
	sb->s_r_ఖండాలు_లెక్క = ext2fs_swab32(sb->s_r_ఖండాలు_లెక్క);
	sb->s_free_ఖండాలు_లెక్క = ext2fs_swab32(sb->s_free_ఖండాలు_లెక్క);
	sb->s_free_inodes_లెక్క = ext2fs_swab32(sb->s_free_inodes_లెక్క);
	sb->s_మొదటి_దత్తాంశం_ఖండం = ext2fs_swab32(sb->s_మొదటి_దత్తాంశం_ఖండం);
	sb->s_log_ఖండం_కొలత = ext2fs_swab32(sb->s_log_ఖండం_కొలత);
	sb->s_log_cluster_కొలత = ext2fs_swab32(sb->s_log_cluster_కొలత);
	sb->s_ఖండాలు_చొప్పున_సమూహం = ext2fs_swab32(sb->s_ఖండాలు_చొప్పున_సమూహం);
	sb->s_clusters_చొప్పున_సమూహం = ext2fs_swab32(sb->s_clusters_చొప్పున_సమూహం);
	sb->s_inodes_చొప్పున_సమూహం = ext2fs_swab32(sb->s_inodes_చొప్పున_సమూహం);
	sb->s_mtime = ext2fs_swab32(sb->s_mtime);
	sb->s_wtime = ext2fs_swab32(sb->s_wtime);
	sb->s_mnt_లెక్క = ext2fs_swab16(sb->s_mnt_లెక్క);
	sb->s_max_mnt_లెక్క = ext2fs_swab16(sb->s_max_mnt_లెక్క);
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
	sb->s_మొదటి_ino = ext2fs_swab32(sb->s_మొదటి_ino);
	sb->s_inode_కొలత = ext2fs_swab16(sb->s_inode_కొలత);
	sb->s_ఖండం_సమూహం_nr = ext2fs_swab16(sb->s_ఖండం_సమూహం_nr);
	sb->s_feature_compat = ext2fs_swab32(sb->s_feature_compat);
	sb->s_feature_incompat = ext2fs_swab32(sb->s_feature_incompat);
	sb->s_feature_ro_compat = ext2fs_swab32(sb->s_feature_ro_compat);
	/* sb->s_uuid is __u8 and does not need swabbing */
	/* sb->s_volume_name is char and does not need swabbing */
	/* sb->s_last_mounted is char and does not need swabbing */
	sb->s_algorithm_usage_bitmap = ext2fs_swab32(sb->s_algorithm_usage_bitmap);
	/* sb->s_prealloc_blocks is __u8 and does not need swabbing */
	/* sb->s_prealloc_dir_blocks is __u8 and does not need swabbing */
	sb->s_reserved_gdt_ఖండాలు = ext2fs_swab16(sb->s_reserved_gdt_ఖండాలు);
	/* sb->s_journal_uuid is __u8 and does not need swabbing */
	sb->s_journal_inum = ext2fs_swab32(sb->s_journal_inum);
	sb->s_journal_dev = ext2fs_swab32(sb->s_journal_dev);
	sb->s_చివరి_orphan = ext2fs_swab32(sb->s_చివరి_orphan);
	కొరకు (i = 0; i < 4; i++)
		sb->s_hash_విత్తనం[i] = ext2fs_swab32(sb->s_hash_విత్తనం[i]);
	/* sb->s_def_hash_version is __u8 and does not need swabbing */
	/* sb->s_jnl_backup_type is __u8 and does not need swabbing */
	sb->s_desc_కొలత = ext2fs_swab16(sb->s_desc_కొలత);
	sb->s_default_mount_opts = ext2fs_swab32(sb->s_default_mount_opts);
	sb->s_మొదటి_meta_bg = ext2fs_swab32(sb->s_మొదటి_meta_bg);
	sb->s_mkfs_సమయం = ext2fs_swab32(sb->s_mkfs_సమయం);
	/* if journal backup is for a valid extent-based journal... */
	ఒకవేళ (ext2fs_extent_శీర్షిక_verify(sb->s_jnl_ఖండాలు,
					పరిమాణం(sb->s_jnl_ఖండాలు)) == 0) {
		/* ... swap only the journal i_size and i_size_high,
		 * and the extent data is not swapped on read */
		i = 15;
	} లేకపోతే {
		/* direct/indirect journal: swap it all */
		i = 0;
	}
	కొరకు (; i < 17; i++)
		sb->s_jnl_ఖండాలు[i] = ext2fs_swab32(sb->s_jnl_ఖండాలు[i]);
	sb->s_ఖండాలు_లెక్క_hi = ext2fs_swab32(sb->s_ఖండాలు_లెక్క_hi);
	sb->s_r_ఖండాలు_లెక్క_hi = ext2fs_swab32(sb->s_r_ఖండాలు_లెక్క_hi);
	sb->s_free_ఖండాలు_hi = ext2fs_swab32(sb->s_free_ఖండాలు_hi);
	sb->s_min_extra_isize = ext2fs_swab16(sb->s_min_extra_isize);
	sb->s_want_extra_isize = ext2fs_swab16(sb->s_want_extra_isize);
	sb->s_flags = ext2fs_swab32(sb->s_flags);
	sb->s_raid_stride = ext2fs_swab16(sb->s_raid_stride);
	sb->s_mmp_update_interval = ext2fs_swab16(sb->s_mmp_update_interval);
	sb->s_mmp_ఖండం = ext2fs_swab64(sb->s_mmp_ఖండం);
	sb->s_raid_stripe_వెడల్పు = ext2fs_swab32(sb->s_raid_stripe_వెడల్పు);
	/* sb->s_log_groups_per_flex is __u8 and does not need swabbing */
	/* sb->s_checksum_type is __u8 and does not need swabbing */
	/* sb->s_encryption_level is __u8 and does not need swabbing */
	/* sb->s_reserved_pad is __u8 and does not need swabbing */
	sb->s_kbytes_written = ext2fs_swab64(sb->s_kbytes_written);
	sb->s_snapshot_inum = ext2fs_swab32(sb->s_snapshot_inum);
	sb->s_snapshot_id = ext2fs_swab32(sb->s_snapshot_id);
	sb->s_snapshot_r_ఖండాలు_లెక్క =
		ext2fs_swab64(sb->s_snapshot_r_ఖండాలు_లెక్క);
	sb->s_snapshot_list = ext2fs_swab32(sb->s_snapshot_list);
	sb->s_error_లెక్క = ext2fs_swab32(sb->s_error_లెక్క);
	sb->s_మొదటి_error_సమయం = ext2fs_swab32(sb->s_మొదటి_error_సమయం);
	sb->s_మొదటి_error_ino = ext2fs_swab32(sb->s_మొదటి_error_ino);
	sb->s_మొదటి_error_ఖండం = ext2fs_swab64(sb->s_మొదటి_error_ఖండం);
	/* sb->s_first_error_func is __u8 and does not need swabbing */
	sb->s_చివరి_error_సమయం = ext2fs_swab32(sb->s_చివరి_error_సమయం);
	sb->s_చివరి_error_ino = ext2fs_swab32(sb->s_చివరి_error_ino);
	sb->s_చివరి_error_ఖండం = ext2fs_swab64(sb->s_చివరి_error_ఖండం);
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
	sb->s_checksum_విత్తనం = ext2fs_swab32(sb->s_checksum_విత్తనం);
	/* s_*_time_hi are __u8 and does not need swabbing */
	sb->s_encoding = ext2fs_swab16(sb->s_encoding);
	sb->s_encoding_flags = ext2fs_swab16(sb->s_encoding_flags);
	sb->s_orphan_file_inum = ext2fs_swab32(sb->s_orphan_file_inum);
	/* catch when new fields are used from s_reserved */
	EXT2FS_BUILD_BUG_ON(పరిమాణం(sb->s_reserved) != 94 * పరిమాణం(__le32));
	sb->s_checksum = ext2fs_swab32(sb->s_checksum);
}

శూన్య ext2fs_మార్పిడి_సమూహం_desc2(ext2_filsys fs, నిర్మాణం ext2_సమూహం_desc *gdp)
{
	నిర్మాణం ext4_సమూహం_desc *gdp4 = (నిర్మాణం ext4_సమూహం_desc *)gdp;

	/* Do the 32-bit parts first */
	gdp->bg_ఖండం_bitmap = ext2fs_swab32(gdp->bg_ఖండం_bitmap);
	gdp->bg_inode_bitmap = ext2fs_swab32(gdp->bg_inode_bitmap);
	gdp->bg_inode_పట్టిక = ext2fs_swab32(gdp->bg_inode_పట్టిక);
	gdp->bg_free_ఖండాలు_లెక్క = ext2fs_swab16(gdp->bg_free_ఖండాలు_లెక్క);
	gdp->bg_free_inodes_లెక్క = ext2fs_swab16(gdp->bg_free_inodes_లెక్క);
	gdp->bg_వాడిన_dirs_లెక్క = ext2fs_swab16(gdp->bg_వాడిన_dirs_లెక్క);
	gdp->bg_flags = ext2fs_swab16(gdp->bg_flags);
	gdp->bg_exclude_bitmap_lo = ext2fs_swab32(gdp->bg_exclude_bitmap_lo);
	gdp->bg_ఖండం_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_ఖండం_bitmap_csum_lo);
	gdp->bg_inode_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_inode_bitmap_csum_lo);
	gdp->bg_itable_వాడని = ext2fs_swab16(gdp->bg_itable_వాడని);
	gdp->bg_checksum = ext2fs_swab16(gdp->bg_checksum);
	/* If we're 32-bit, we're done */
	ఒకవేళ (fs == ఖాళీ || EXT2_DESC_కొలత(fs->super) < EXT2_MIN_DESC_కొలత_64_బిట్టు)
		తిరిగి;

	/* Swap the 64-bit parts */
	gdp4->bg_ఖండం_bitmap_hi = ext2fs_swab32(gdp4->bg_ఖండం_bitmap_hi);
	gdp4->bg_inode_bitmap_hi = ext2fs_swab32(gdp4->bg_inode_bitmap_hi);
	gdp4->bg_inode_పట్టిక_hi = ext2fs_swab32(gdp4->bg_inode_పట్టిక_hi);
	gdp4->bg_free_ఖండాలు_లెక్క_hi =
		ext2fs_swab16(gdp4->bg_free_ఖండాలు_లెక్క_hi);
	gdp4->bg_free_inodes_లెక్క_hi =
		ext2fs_swab16(gdp4->bg_free_inodes_లెక్క_hi);
	gdp4->bg_వాడిన_dirs_లెక్క_hi =
		ext2fs_swab16(gdp4->bg_వాడిన_dirs_లెక్క_hi);
	gdp4->bg_itable_వాడని_hi = ext2fs_swab16(gdp4->bg_itable_వాడని_hi);
	gdp4->bg_exclude_bitmap_hi = ext2fs_swab16(gdp4->bg_exclude_bitmap_hi);
	gdp4->bg_ఖండం_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_ఖండం_bitmap_csum_hi);
	gdp4->bg_inode_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_inode_bitmap_csum_hi);
	EXT2FS_BUILD_BUG_ON(పరిమాణం(gdp4->bg_reserved) != పరిమాణం(__u32));
}

శూన్య ext2fs_మార్పిడి_సమూహం_desc(నిర్మాణం ext2_సమూహం_desc *gdp)
{
	ext2fs_మార్పిడి_సమూహం_desc2(0, gdp);
}


శూన్య ext2fs_మార్పిడి_ext_attr_శీర్షిక(నిర్మాణం ext2_ext_attr_శీర్షిక *గమ్యం_శీర్షిక,
				 నిర్మాణం ext2_ext_attr_శీర్షిక *from_శీర్షిక)
{
	పూర్ణాంకం n;

	గమ్యం_శీర్షిక->h_magic    = ext2fs_swab32(from_శీర్షిక->h_magic);
	గమ్యం_శీర్షిక->h_ఖండాలు   = ext2fs_swab32(from_శీర్షిక->h_ఖండాలు);
	గమ్యం_శీర్షిక->h_refcount = ext2fs_swab32(from_శీర్షిక->h_refcount);
	గమ్యం_శీర్షిక->h_hash     = ext2fs_swab32(from_శీర్షిక->h_hash);
	గమ్యం_శీర్షిక->h_checksum = ext2fs_swab32(from_శీర్షిక->h_checksum);
	కొరకు (n = 0; n < 3; n++)
		గమ్యం_శీర్షిక->h_reserved[n] =
			ext2fs_swab32(from_శీర్షిక->h_reserved[n]);
}

శూన్య ext2fs_మార్పిడి_ext_attr_నమోదు(నిర్మాణం ext2_ext_attr_నమోదు *గమ్యం_నమోదు,
				నిర్మాణం ext2_ext_attr_నమోదు *from_నమోదు)
{
	గమ్యం_నమోదు->e_విలువ_offs  = ext2fs_swab16(from_నమోదు->e_విలువ_offs);
	గమ్యం_నమోదు->e_విలువ_inum  = ext2fs_swab32(from_నమోదు->e_విలువ_inum);
	గమ్యం_నమోదు->e_విలువ_కొలత  = ext2fs_swab32(from_నమోదు->e_విలువ_కొలత);
	గమ్యం_నమోదు->e_hash	= ext2fs_swab32(from_నమోదు->e_hash);
}

శూన్య ext2fs_మార్పిడి_ext_attr(అక్షరం *గమ్యం, అక్షరం *from, పూర్ణాంకం bufsize, పూర్ణాంకం has_శీర్షిక)
{
	నిర్మాణం ext2_ext_attr_శీర్షిక *from_శీర్షిక =
		(నిర్మాణం ext2_ext_attr_శీర్షిక *)from;
	నిర్మాణం ext2_ext_attr_శీర్షిక *గమ్యం_శీర్షిక =
		(నిర్మాణం ext2_ext_attr_శీర్షిక *)గమ్యం;
	నిర్మాణం ext2_ext_attr_నమోదు *from_నమోదు, *గమ్యం_నమోదు;
	అక్షరం *from_చివర = (అక్షరం *)from_శీర్షిక + bufsize;

	ఒకవేళ (గమ్యం_శీర్షిక != from_శీర్షిక)
		జ్ఞాపకం_నకలు(గమ్యం_శీర్షిక, from_శీర్షిక, bufsize);

	ఒకవేళ (has_శీర్షిక) {
		ext2fs_మార్పిడి_ext_attr_శీర్షిక(గమ్యం_శీర్షిక, from_శీర్షిక);

		from_నమోదు = (నిర్మాణం ext2_ext_attr_నమోదు *)(from_శీర్షిక+1);
		గమ్యం_నమోదు   = (నిర్మాణం ext2_ext_attr_నమోదు *)(గమ్యం_శీర్షిక+1);
	} లేకపోతే {
		from_నమోదు = (నిర్మాణం ext2_ext_attr_నమోదు *)from_శీర్షిక;
		గమ్యం_నమోదు   = (నిర్మాణం ext2_ext_attr_నమోదు *)గమ్యం_శీర్షిక;
	}

	అయితే ((అక్షరం *)from_నమోదు < from_చివర &&
	       (అక్షరం *)EXT2_EXT_ATTR_తదుపరి(from_నమోదు) <= from_చివర &&
	       *(__u32 *)from_నమోదు) {
		ext2fs_మార్పిడి_ext_attr_నమోదు(గమ్యం_నమోదు, from_నమోదు);
		from_నమోదు = EXT2_EXT_ATTR_తదుపరి(from_నమోదు);
		గమ్యం_నమోదు   = EXT2_EXT_ATTR_తదుపరి(గమ్యం_నమోదు);
	}
}

శూన్య ext2fs_మార్పిడి_inode_నిండిన(ext2_filsys fs, నిర్మాణం ext2_inode_large *t,
			    నిర్మాణం ext2_inode_large *f, పూర్ణాంకం hostorder,
			    పూర్ణాంకం bufsize)
{
	గుర్తులేని i, extra_isize, attr_magic;
	పూర్ణాంకం has_extents = 0, has_inline_దత్తాంశం = 0, islnk = 0, fast_symlink = 0;
	గుర్తులేని పూర్ణాంకం inode_కొలత;
	__u32 *eaf, *eat;

	/*
	 * Note that t and f may point to the same address. That's why
	 * if (hostorder) condition is executed before swab calls and
	 * if (!hostorder) afterwards.
	 */
	ఒకవేళ (hostorder) {
		islnk = LINUX_S_ISLNK(f->i_బహుళకం);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(f));
		has_extents = (f->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_దత్తాంశం = (f->i_flags & EXT4_INLINE_దత్తాంశం_FL) != 0;
	}

	t->i_బహుళకం = ext2fs_swab16(f->i_బహుళకం);
	t->i_uid = ext2fs_swab16(f->i_uid);
	t->i_కొలత = ext2fs_swab32(f->i_కొలత);
	t->i_atime = ext2fs_swab32(f->i_atime);
	t->i_ctime = ext2fs_swab32(f->i_ctime);
	t->i_mtime = ext2fs_swab32(f->i_mtime);
	t->i_dtime = ext2fs_swab32(f->i_dtime);
	t->i_gid = ext2fs_swab16(f->i_gid);
	t->i_links_లెక్క = ext2fs_swab16(f->i_links_లెక్క);
	t->i_file_acl = ext2fs_swab32(f->i_file_acl);
	t->i_ఖండాలు = ext2fs_swab32(f->i_ఖండాలు);
	t->i_flags = ext2fs_swab32(f->i_flags);
	t->i_కొలత_ఎగువ = ext2fs_swab32(f->i_కొలత_ఎగువ);

	ఒకవేళ (!hostorder) {
		islnk = LINUX_S_ISLNK(t->i_బహుళకం);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(t));
		has_extents = (t->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_దత్తాంశం = (t->i_flags & EXT4_INLINE_దత్తాంశం_FL) != 0;
	}

	/*
	 * Extent data and inline data are swapped on access, not here
	 */
	ఒకవేళ (!has_extents && !has_inline_దత్తాంశం && (!islnk || !fast_symlink)) {
		కొరకు (i = 0; i < EXT2_N_ఖండాలు; i++)
			t->i_ఖండం[i] = ext2fs_swab32(f->i_ఖండం[i]);
	} లేకపోతే ఒకవేళ (t != f) {
		కొరకు (i = 0; i < EXT2_N_ఖండాలు; i++)
			t->i_ఖండం[i] = f->i_ఖండం[i];
	}
	t->i_generation = ext2fs_swab32(f->i_generation);
	t->i_faddr = ext2fs_swab32(f->i_faddr);

	ఎంపిక (fs->super->s_creator_os) {
	స్థితి EXT2_OS_LINUX:
		t->osd1.linux1.l_i_సంస్కరణ =
			ext2fs_swab32(f->osd1.linux1.l_i_సంస్కరణ);
		t->osd2.linux2.l_i_ఖండాలు_hi =
			ext2fs_swab16(f->osd2.linux2.l_i_ఖండాలు_hi);
		t->osd2.linux2.l_i_file_acl_ఎగువ =
			ext2fs_swab16(f->osd2.linux2.l_i_file_acl_ఎగువ);
		t->osd2.linux2.l_i_uid_ఎగువ =
		  ext2fs_swab16 (f->osd2.linux2.l_i_uid_ఎగువ);
		t->osd2.linux2.l_i_gid_ఎగువ =
		  ext2fs_swab16 (f->osd2.linux2.l_i_gid_ఎగువ);
		t->osd2.linux2.l_i_checksum_lo =
			ext2fs_swab16(f->osd2.linux2.l_i_checksum_lo);
		ఆపు;
	స్థితి EXT2_OS_HURD:
		t->osd1.hurd1.h_i_translator =
		  ext2fs_swab32 (f->osd1.hurd1.h_i_translator);
		t->osd2.hurd2.h_i_frag = f->osd2.hurd2.h_i_frag;
		t->osd2.hurd2.h_i_fsize = f->osd2.hurd2.h_i_fsize;
		t->osd2.hurd2.h_i_బహుళకం_ఎగువ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_బహుళకం_ఎగువ);
		t->osd2.hurd2.h_i_uid_ఎగువ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_uid_ఎగువ);
		t->osd2.hurd2.h_i_gid_ఎగువ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_gid_ఎగువ);
		t->osd2.hurd2.h_i_author =
		  ext2fs_swab32 (f->osd2.hurd2.h_i_author);
		ఆపు;
	అప్రమేయ:
		ఆపు;
	}

	ఒకవేళ (bufsize < (పూర్ణాంకం) (పరిమాణం(నిర్మాణం ext2_inode) + పరిమాణం(__u16)))
		తిరిగి; /* no i_extra_isize field */

	ఒకవేళ (hostorder)
		extra_isize = f->i_extra_isize;
	t->i_extra_isize = ext2fs_swab16(f->i_extra_isize);
	ఒకవేళ (!hostorder)
		extra_isize = t->i_extra_isize;
	ఒకవేళ (extra_isize > EXT2_INODE_కొలత(fs->super) -
				పరిమాణం(నిర్మాణం ext2_inode)) {
		/* this is error case: i_extra_size is too large */
		తిరిగి;
	}
	ఒకవేళ (extra_isize & 3)
		తిరిగి;		/* Illegal inode extra_isize */

	inode_కొలత = EXT2_GOOD_OLD_INODE_కొలత + extra_isize;
	ఒకవేళ (ext2fs_inode_includes(inode_కొలత, i_checksum_hi))
		t->i_checksum_hi = ext2fs_swab16(f->i_checksum_hi);
	ఒకవేళ (ext2fs_inode_includes(inode_కొలత, i_ctime_extra))
		t->i_ctime_extra = ext2fs_swab32(f->i_ctime_extra);
	ఒకవేళ (ext2fs_inode_includes(inode_కొలత, i_mtime_extra))
		t->i_mtime_extra = ext2fs_swab32(f->i_mtime_extra);
	ఒకవేళ (ext2fs_inode_includes(inode_కొలత, i_atime_extra))
		t->i_atime_extra = ext2fs_swab32(f->i_atime_extra);
	ఒకవేళ (ext2fs_inode_includes(inode_కొలత, i_crtime))
		t->i_crtime = ext2fs_swab32(f->i_crtime);
	ఒకవేళ (ext2fs_inode_includes(inode_కొలత, i_crtime_extra))
		t->i_crtime_extra = ext2fs_swab32(f->i_crtime_extra);
	ఒకవేళ (ext2fs_inode_includes(inode_కొలత, i_సంస్కరణ_hi))
		t->i_సంస్కరణ_hi = ext2fs_swab32(f->i_సంస్కరణ_hi);
	ఒకవేళ (ext2fs_inode_includes(inode_కొలత, i_projid))
                t->i_projid = ext2fs_swab32(f->i_projid);
	/* catch new static fields added after i_projid */
	EXT2FS_BUILD_BUG_ON(పరిమాణం(నిర్మాణం ext2_inode_large) != 160);

	i = పరిమాణం(నిర్మాణం ext2_inode) + extra_isize + పరిమాణం(__u32);
	ఒకవేళ (bufsize < (పూర్ణాంకం) i)
		తిరిగి; /* no space for EA magic */

	eaf = (__u32 *) (((అక్షరం *) f) + పరిమాణం(నిర్మాణం ext2_inode) +
					extra_isize);

	attr_magic = *eaf;
	ఒకవేళ (!hostorder)
		attr_magic = ext2fs_swab32(attr_magic);

	ఒకవేళ (attr_magic != EXT2_EXT_ATTR_MAGIC)
		తిరిగి; /* it seems no magic here */

	eat = (__u32 *) (((అక్షరం *) t) + పరిమాణం(నిర్మాణం ext2_inode) +
					extra_isize);
	*eat = ext2fs_swab32(*eaf);

	/* convert EA(s) */
	ext2fs_మార్పిడి_ext_attr((అక్షరం *) (eat + 1), (అక్షరం *) (eaf + 1),
			     bufsize - పరిమాణం(నిర్మాణం ext2_inode) -
			     extra_isize - పరిమాణం(__u32), 0);

}

శూన్య ext2fs_మార్పిడి_inode(ext2_filsys fs, నిర్మాణం ext2_inode *t,
		       నిర్మాణం ext2_inode *f, పూర్ణాంకం hostorder)
{
	ext2fs_మార్పిడి_inode_నిండిన(fs, (నిర్మాణం ext2_inode_large *) t,
				(నిర్మాణం ext2_inode_large *) f, hostorder,
				పరిమాణం(నిర్మాణం ext2_inode));
}

శూన్య ext2fs_మార్పిడి_mmp(నిర్మాణం mmp_struct *mmp)
{
	mmp->mmp_magic = ext2fs_swab32(mmp->mmp_magic);
	mmp->mmp_seq = ext2fs_swab32(mmp->mmp_seq);
	mmp->mmp_సమయం = ext2fs_swab64(mmp->mmp_సమయం);
	mmp->mmp_check_interval = ext2fs_swab16(mmp->mmp_check_interval);
	mmp->mmp_checksum = ext2fs_swab32(mmp->mmp_checksum);
}

errcode_t ext2fs_dirent_swab_లోపల(ext2_filsys fs, అక్షరం *buf, పూర్ణాంకం flags)
{
	తిరిగి ext2fs_dirent_swab_in2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, అక్షరం *buf,
				 size_t కొలత, పూర్ణాంకం flags)
{
	errcode_t	retval;
	అక్షరం		*p, *చివర;
	నిర్మాణం ext2_dir_నమోదు *dirent;
	గుర్తులేని పూర్ణాంకం	name_len, rec_len, ఎడమ;

	p = (అక్షరం *) buf;
	చివర = (అక్షరం *) buf + కొలత;
	ఎడమ = కొలత;
	అయితే (p < చివర-8) {
		dirent = (నిర్మాణం ext2_dir_నమోదు *) p;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		name_len = dirent->name_len;
		ఒకవేళ (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		ఒకవేళ (retval)
			తిరిగి retval;
		ఒకవేళ ((rec_len < 8) || (rec_len % 4)) {
			rec_len = 8;
			ఒకవేళ (!(fs->flags & EXT2_FLAG_IGNORE_మార్పిడి_DIRENT))
				తిరిగి EXT2_ET_DIR_CORRUPTED;
		} లేకపోతే ఒకవేళ (((name_len & 0xFF) + 8) > rec_len)
			ఒకవేళ (!(fs->flags & EXT2_FLAG_IGNORE_మార్పిడి_DIRENT))
				తిరిగి EXT2_ET_DIR_CORRUPTED;
		ఒకవేళ (rec_len > ఎడమ)
			ఒకవేళ (!(fs->flags & EXT2_FLAG_IGNORE_మార్పిడి_DIRENT))
				తిరిగి EXT2_ET_DIR_CORRUPTED;
		ఎడమ -= rec_len;
		p += rec_len;
	}

	తిరిగి 0;
}

errcode_t ext2fs_dirent_swab_బహిర్గతం(ext2_filsys fs, అక్షరం *buf, పూర్ణాంకం flags)
{
	తిరిగి ext2fs_dirent_swab_out2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, అక్షరం *buf,
				  size_t కొలత, పూర్ణాంకం flags)
{
	errcode_t	retval;
	అక్షరం		*p, *చివర;
	గుర్తులేని పూర్ణాంకం	rec_len;
	నిర్మాణం ext2_dir_నమోదు *dirent;

	p = buf;
	చివర = buf + కొలత;
	అయితే (p < చివర) {
		dirent = (నిర్మాణం ext2_dir_నమోదు *) p;
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		ఒకవేళ (retval)
			తిరిగి retval;
		ఒకవేళ ((rec_len < 8) ||
		    (rec_len % 4)) {
			ext2fs_free_mem(&buf);
			తిరిగి EXT2_ET_DIR_CORRUPTED;
		}
		p += rec_len;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		ఒకవేళ (rec_len > కొలత)
			తిరిగి EXT2_ET_DIR_CORRUPTED;
		కొలత -= rec_len;

		ఒకవేళ (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
	}

	తిరిగి 0;
}
