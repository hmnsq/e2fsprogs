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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"
#شامل <ext2fs/ext2_ext_attr.h>

خالی ext2fs_تبادلہ_super(ساخت ext2_super_بلاک * sb)
{
	صحیح i;

	sb->s_inodes_گنتی = ext2fs_swab32(sb->s_inodes_گنتی);
	sb->s_بلاکیں_گنتی = ext2fs_swab32(sb->s_بلاکیں_گنتی);
	sb->s_r_بلاکیں_گنتی = ext2fs_swab32(sb->s_r_بلاکیں_گنتی);
	sb->s_free_بلاکیں_گنتی = ext2fs_swab32(sb->s_free_بلاکیں_گنتی);
	sb->s_free_inodes_گنتی = ext2fs_swab32(sb->s_free_inodes_گنتی);
	sb->s_پہلا_ڈیٹا_بلاک = ext2fs_swab32(sb->s_پہلا_ڈیٹا_بلاک);
	sb->s_log_بلاک_سائز = ext2fs_swab32(sb->s_log_بلاک_سائز);
	sb->s_log_cluster_سائز = ext2fs_swab32(sb->s_log_cluster_سائز);
	sb->s_بلاکیں_فی_گروہ = ext2fs_swab32(sb->s_بلاکیں_فی_گروہ);
	sb->s_clusters_فی_گروہ = ext2fs_swab32(sb->s_clusters_فی_گروہ);
	sb->s_inodes_فی_گروہ = ext2fs_swab32(sb->s_inodes_فی_گروہ);
	sb->s_mtime = ext2fs_swab32(sb->s_mtime);
	sb->s_wtime = ext2fs_swab32(sb->s_wtime);
	sb->s_mnt_گنتی = ext2fs_swab16(sb->s_mnt_گنتی);
	sb->s_max_mnt_گنتی = ext2fs_swab16(sb->s_max_mnt_گنتی);
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
	sb->s_پہلا_ino = ext2fs_swab32(sb->s_پہلا_ino);
	sb->s_inode_سائز = ext2fs_swab16(sb->s_inode_سائز);
	sb->s_بلاک_گروہ_nr = ext2fs_swab16(sb->s_بلاک_گروہ_nr);
	sb->s_feature_compat = ext2fs_swab32(sb->s_feature_compat);
	sb->s_feature_incompat = ext2fs_swab32(sb->s_feature_incompat);
	sb->s_feature_ro_compat = ext2fs_swab32(sb->s_feature_ro_compat);
	/* sb->s_uuid is __u8 and does not need swabbing */
	/* sb->s_volume_name is char and does not need swabbing */
	/* sb->s_last_mounted is char and does not need swabbing */
	sb->s_algorithm_usage_bitmap = ext2fs_swab32(sb->s_algorithm_usage_bitmap);
	/* sb->s_prealloc_blocks is __u8 and does not need swabbing */
	/* sb->s_prealloc_dir_blocks is __u8 and does not need swabbing */
	sb->s_reserved_gdt_بلاکیں = ext2fs_swab16(sb->s_reserved_gdt_بلاکیں);
	/* sb->s_journal_uuid is __u8 and does not need swabbing */
	sb->s_journal_inum = ext2fs_swab32(sb->s_journal_inum);
	sb->s_journal_dev = ext2fs_swab32(sb->s_journal_dev);
	sb->s_آخری_orphan = ext2fs_swab32(sb->s_آخری_orphan);
	برائے (i = 0; i < 4; i++)
		sb->s_hash_بیج[i] = ext2fs_swab32(sb->s_hash_بیج[i]);
	/* sb->s_def_hash_version is __u8 and does not need swabbing */
	/* sb->s_jnl_backup_type is __u8 and does not need swabbing */
	sb->s_desc_سائز = ext2fs_swab16(sb->s_desc_سائز);
	sb->s_default_mount_opts = ext2fs_swab32(sb->s_default_mount_opts);
	sb->s_پہلا_meta_bg = ext2fs_swab32(sb->s_پہلا_meta_bg);
	sb->s_mkfs_وقت = ext2fs_swab32(sb->s_mkfs_وقت);
	/* if journal backup is for a valid extent-based journal... */
	اگر (ext2fs_extent_سرخی_verify(sb->s_jnl_بلاکیں,
					حجم(sb->s_jnl_بلاکیں)) == 0) {
		/* ... swap only the journal i_size and i_size_high,
		 * and the extent data is not swapped on read */
		i = 15;
	} ورنہ {
		/* direct/indirect journal: swap it all */
		i = 0;
	}
	برائے (; i < 17; i++)
		sb->s_jnl_بلاکیں[i] = ext2fs_swab32(sb->s_jnl_بلاکیں[i]);
	sb->s_بلاکیں_گنتی_hi = ext2fs_swab32(sb->s_بلاکیں_گنتی_hi);
	sb->s_r_بلاکیں_گنتی_hi = ext2fs_swab32(sb->s_r_بلاکیں_گنتی_hi);
	sb->s_free_بلاکیں_hi = ext2fs_swab32(sb->s_free_بلاکیں_hi);
	sb->s_min_extra_isize = ext2fs_swab16(sb->s_min_extra_isize);
	sb->s_want_extra_isize = ext2fs_swab16(sb->s_want_extra_isize);
	sb->s_flags = ext2fs_swab32(sb->s_flags);
	sb->s_raid_stride = ext2fs_swab16(sb->s_raid_stride);
	sb->s_mmp_update_interval = ext2fs_swab16(sb->s_mmp_update_interval);
	sb->s_mmp_بلاک = ext2fs_swab64(sb->s_mmp_بلاک);
	sb->s_raid_stripe_چوڑائی = ext2fs_swab32(sb->s_raid_stripe_چوڑائی);
	/* sb->s_log_groups_per_flex is __u8 and does not need swabbing */
	/* sb->s_checksum_type is __u8 and does not need swabbing */
	/* sb->s_encryption_level is __u8 and does not need swabbing */
	/* sb->s_reserved_pad is __u8 and does not need swabbing */
	sb->s_kbytes_written = ext2fs_swab64(sb->s_kbytes_written);
	sb->s_snapshot_inum = ext2fs_swab32(sb->s_snapshot_inum);
	sb->s_snapshot_id = ext2fs_swab32(sb->s_snapshot_id);
	sb->s_snapshot_r_بلاکیں_گنتی =
		ext2fs_swab64(sb->s_snapshot_r_بلاکیں_گنتی);
	sb->s_snapshot_list = ext2fs_swab32(sb->s_snapshot_list);
	sb->s_error_گنتی = ext2fs_swab32(sb->s_error_گنتی);
	sb->s_پہلا_error_وقت = ext2fs_swab32(sb->s_پہلا_error_وقت);
	sb->s_پہلا_error_ino = ext2fs_swab32(sb->s_پہلا_error_ino);
	sb->s_پہلا_error_بلاک = ext2fs_swab64(sb->s_پہلا_error_بلاک);
	/* sb->s_first_error_func is __u8 and does not need swabbing */
	sb->s_آخری_error_وقت = ext2fs_swab32(sb->s_آخری_error_وقت);
	sb->s_آخری_error_ino = ext2fs_swab32(sb->s_آخری_error_ino);
	sb->s_آخری_error_بلاک = ext2fs_swab64(sb->s_آخری_error_بلاک);
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
	sb->s_checksum_بیج = ext2fs_swab32(sb->s_checksum_بیج);
	/* s_*_time_hi are __u8 and does not need swabbing */
	sb->s_encoding = ext2fs_swab16(sb->s_encoding);
	sb->s_encoding_flags = ext2fs_swab16(sb->s_encoding_flags);
	sb->s_orphan_file_inum = ext2fs_swab32(sb->s_orphan_file_inum);
	/* catch when new fields are used from s_reserved */
	EXT2FS_BUILD_BUG_ON(حجم(sb->s_reserved) != 94 * حجم(__le32));
	sb->s_checksum = ext2fs_swab32(sb->s_checksum);
}

خالی ext2fs_تبادلہ_گروہ_desc2(ext2_filsys fs, ساخت ext2_گروہ_desc *gdp)
{
	ساخت ext4_گروہ_desc *gdp4 = (ساخت ext4_گروہ_desc *)gdp;

	/* Do the 32-bit parts first */
	gdp->bg_بلاک_bitmap = ext2fs_swab32(gdp->bg_بلاک_bitmap);
	gdp->bg_inode_bitmap = ext2fs_swab32(gdp->bg_inode_bitmap);
	gdp->bg_inode_جدول = ext2fs_swab32(gdp->bg_inode_جدول);
	gdp->bg_free_بلاکیں_گنتی = ext2fs_swab16(gdp->bg_free_بلاکیں_گنتی);
	gdp->bg_free_inodes_گنتی = ext2fs_swab16(gdp->bg_free_inodes_گنتی);
	gdp->bg_مستعمل_dirs_گنتی = ext2fs_swab16(gdp->bg_مستعمل_dirs_گنتی);
	gdp->bg_flags = ext2fs_swab16(gdp->bg_flags);
	gdp->bg_exclude_bitmap_lo = ext2fs_swab32(gdp->bg_exclude_bitmap_lo);
	gdp->bg_بلاک_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_بلاک_bitmap_csum_lo);
	gdp->bg_inode_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_inode_bitmap_csum_lo);
	gdp->bg_itable_غیر_مستعمل = ext2fs_swab16(gdp->bg_itable_غیر_مستعمل);
	gdp->bg_checksum = ext2fs_swab16(gdp->bg_checksum);
	/* If we're 32-bit, we're done */
	اگر (fs == تہی || EXT2_DESC_سائز(fs->super) < EXT2_MIN_DESC_سائز_64_ثنائی_ہندسہ)
		واپس;

	/* Swap the 64-bit parts */
	gdp4->bg_بلاک_bitmap_hi = ext2fs_swab32(gdp4->bg_بلاک_bitmap_hi);
	gdp4->bg_inode_bitmap_hi = ext2fs_swab32(gdp4->bg_inode_bitmap_hi);
	gdp4->bg_inode_جدول_hi = ext2fs_swab32(gdp4->bg_inode_جدول_hi);
	gdp4->bg_free_بلاکیں_گنتی_hi =
		ext2fs_swab16(gdp4->bg_free_بلاکیں_گنتی_hi);
	gdp4->bg_free_inodes_گنتی_hi =
		ext2fs_swab16(gdp4->bg_free_inodes_گنتی_hi);
	gdp4->bg_مستعمل_dirs_گنتی_hi =
		ext2fs_swab16(gdp4->bg_مستعمل_dirs_گنتی_hi);
	gdp4->bg_itable_غیر_مستعمل_hi = ext2fs_swab16(gdp4->bg_itable_غیر_مستعمل_hi);
	gdp4->bg_exclude_bitmap_hi = ext2fs_swab16(gdp4->bg_exclude_bitmap_hi);
	gdp4->bg_بلاک_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_بلاک_bitmap_csum_hi);
	gdp4->bg_inode_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_inode_bitmap_csum_hi);
	EXT2FS_BUILD_BUG_ON(حجم(gdp4->bg_reserved) != حجم(__u32));
}

خالی ext2fs_تبادلہ_گروہ_desc(ساخت ext2_گروہ_desc *gdp)
{
	ext2fs_تبادلہ_گروہ_desc2(0, gdp);
}


خالی ext2fs_تبادلہ_ext_attr_سرخی(ساخت ext2_ext_attr_سرخی *مقصد_سرخی,
				 ساخت ext2_ext_attr_سرخی *from_سرخی)
{
	صحیح n;

	مقصد_سرخی->h_magic    = ext2fs_swab32(from_سرخی->h_magic);
	مقصد_سرخی->h_بلاکیں   = ext2fs_swab32(from_سرخی->h_بلاکیں);
	مقصد_سرخی->h_refcount = ext2fs_swab32(from_سرخی->h_refcount);
	مقصد_سرخی->h_hash     = ext2fs_swab32(from_سرخی->h_hash);
	مقصد_سرخی->h_checksum = ext2fs_swab32(from_سرخی->h_checksum);
	برائے (n = 0; n < 3; n++)
		مقصد_سرخی->h_reserved[n] =
			ext2fs_swab32(from_سرخی->h_reserved[n]);
}

خالی ext2fs_تبادلہ_ext_attr_اندراج(ساخت ext2_ext_attr_اندراج *مقصد_اندراج,
				ساخت ext2_ext_attr_اندراج *from_اندراج)
{
	مقصد_اندراج->e_قدر_offs  = ext2fs_swab16(from_اندراج->e_قدر_offs);
	مقصد_اندراج->e_قدر_inum  = ext2fs_swab32(from_اندراج->e_قدر_inum);
	مقصد_اندراج->e_قدر_سائز  = ext2fs_swab32(from_اندراج->e_قدر_سائز);
	مقصد_اندراج->e_hash	= ext2fs_swab32(from_اندراج->e_hash);
}

خالی ext2fs_تبادلہ_ext_attr(حرف *مقصد, حرف *from, صحیح bufsize, صحیح has_سرخی)
{
	ساخت ext2_ext_attr_سرخی *from_سرخی =
		(ساخت ext2_ext_attr_سرخی *)from;
	ساخت ext2_ext_attr_سرخی *مقصد_سرخی =
		(ساخت ext2_ext_attr_سرخی *)مقصد;
	ساخت ext2_ext_attr_اندراج *from_اندراج, *مقصد_اندراج;
	حرف *from_انتہا = (حرف *)from_سرخی + bufsize;

	اگر (مقصد_سرخی != from_سرخی)
		حافظہ_نقل(مقصد_سرخی, from_سرخی, bufsize);

	اگر (has_سرخی) {
		ext2fs_تبادلہ_ext_attr_سرخی(مقصد_سرخی, from_سرخی);

		from_اندراج = (ساخت ext2_ext_attr_اندراج *)(from_سرخی+1);
		مقصد_اندراج   = (ساخت ext2_ext_attr_اندراج *)(مقصد_سرخی+1);
	} ورنہ {
		from_اندراج = (ساخت ext2_ext_attr_اندراج *)from_سرخی;
		مقصد_اندراج   = (ساخت ext2_ext_attr_اندراج *)مقصد_سرخی;
	}

	جب ((حرف *)from_اندراج < from_انتہا &&
	       (حرف *)EXT2_EXT_ATTR_اگلا(from_اندراج) <= from_انتہا &&
	       *(__u32 *)from_اندراج) {
		ext2fs_تبادلہ_ext_attr_اندراج(مقصد_اندراج, from_اندراج);
		from_اندراج = EXT2_EXT_ATTR_اگلا(from_اندراج);
		مقصد_اندراج   = EXT2_EXT_ATTR_اگلا(مقصد_اندراج);
	}
}

خالی ext2fs_تبادلہ_inode_بھرا(ext2_filsys fs, ساخت ext2_inode_large *t,
			    ساخت ext2_inode_large *f, صحیح hostorder,
			    صحیح bufsize)
{
	غیرعلامتی i, extra_isize, attr_magic;
	صحیح has_extents = 0, has_inline_ڈیٹا = 0, islnk = 0, fast_symlink = 0;
	غیرعلامتی صحیح inode_سائز;
	__u32 *eaf, *eat;

	/*
	 * Note that t and f may point to the same address. That's why
	 * if (hostorder) condition is executed before swab calls and
	 * if (!hostorder) afterwards.
	 */
	اگر (hostorder) {
		islnk = LINUX_S_ISLNK(f->i_منوال);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(f));
		has_extents = (f->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_ڈیٹا = (f->i_flags & EXT4_INLINE_ڈیٹا_FL) != 0;
	}

	t->i_منوال = ext2fs_swab16(f->i_منوال);
	t->i_uid = ext2fs_swab16(f->i_uid);
	t->i_سائز = ext2fs_swab32(f->i_سائز);
	t->i_atime = ext2fs_swab32(f->i_atime);
	t->i_ctime = ext2fs_swab32(f->i_ctime);
	t->i_mtime = ext2fs_swab32(f->i_mtime);
	t->i_dtime = ext2fs_swab32(f->i_dtime);
	t->i_gid = ext2fs_swab16(f->i_gid);
	t->i_links_گنتی = ext2fs_swab16(f->i_links_گنتی);
	t->i_file_acl = ext2fs_swab32(f->i_file_acl);
	t->i_بلاکیں = ext2fs_swab32(f->i_بلاکیں);
	t->i_flags = ext2fs_swab32(f->i_flags);
	t->i_سائز_بلند = ext2fs_swab32(f->i_سائز_بلند);

	اگر (!hostorder) {
		islnk = LINUX_S_ISLNK(t->i_منوال);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(t));
		has_extents = (t->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_ڈیٹا = (t->i_flags & EXT4_INLINE_ڈیٹا_FL) != 0;
	}

	/*
	 * Extent data and inline data are swapped on access, not here
	 */
	اگر (!has_extents && !has_inline_ڈیٹا && (!islnk || !fast_symlink)) {
		برائے (i = 0; i < EXT2_N_بلاکیں; i++)
			t->i_بلاک[i] = ext2fs_swab32(f->i_بلاک[i]);
	} ورنہ اگر (t != f) {
		برائے (i = 0; i < EXT2_N_بلاکیں; i++)
			t->i_بلاک[i] = f->i_بلاک[i];
	}
	t->i_generation = ext2fs_swab32(f->i_generation);
	t->i_faddr = ext2fs_swab32(f->i_faddr);

	انتخاب (fs->super->s_creator_os) {
	حالت EXT2_OS_LINUX:
		t->osd1.linux1.l_i_نسخہ =
			ext2fs_swab32(f->osd1.linux1.l_i_نسخہ);
		t->osd2.linux2.l_i_بلاکیں_hi =
			ext2fs_swab16(f->osd2.linux2.l_i_بلاکیں_hi);
		t->osd2.linux2.l_i_file_acl_بلند =
			ext2fs_swab16(f->osd2.linux2.l_i_file_acl_بلند);
		t->osd2.linux2.l_i_uid_بلند =
		  ext2fs_swab16 (f->osd2.linux2.l_i_uid_بلند);
		t->osd2.linux2.l_i_gid_بلند =
		  ext2fs_swab16 (f->osd2.linux2.l_i_gid_بلند);
		t->osd2.linux2.l_i_checksum_lo =
			ext2fs_swab16(f->osd2.linux2.l_i_checksum_lo);
		توڑو;
	حالت EXT2_OS_HURD:
		t->osd1.hurd1.h_i_translator =
		  ext2fs_swab32 (f->osd1.hurd1.h_i_translator);
		t->osd2.hurd2.h_i_frag = f->osd2.hurd2.h_i_frag;
		t->osd2.hurd2.h_i_fsize = f->osd2.hurd2.h_i_fsize;
		t->osd2.hurd2.h_i_منوال_بلند =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_منوال_بلند);
		t->osd2.hurd2.h_i_uid_بلند =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_uid_بلند);
		t->osd2.hurd2.h_i_gid_بلند =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_gid_بلند);
		t->osd2.hurd2.h_i_author =
		  ext2fs_swab32 (f->osd2.hurd2.h_i_author);
		توڑو;
	طےشدہ:
		توڑو;
	}

	اگر (bufsize < (صحیح) (حجم(ساخت ext2_inode) + حجم(__u16)))
		واپس; /* no i_extra_isize field */

	اگر (hostorder)
		extra_isize = f->i_extra_isize;
	t->i_extra_isize = ext2fs_swab16(f->i_extra_isize);
	اگر (!hostorder)
		extra_isize = t->i_extra_isize;
	اگر (extra_isize > EXT2_INODE_سائز(fs->super) -
				حجم(ساخت ext2_inode)) {
		/* this is error case: i_extra_size is too large */
		واپس;
	}
	اگر (extra_isize & 3)
		واپس;		/* Illegal inode extra_isize */

	inode_سائز = EXT2_GOOD_OLD_INODE_سائز + extra_isize;
	اگر (ext2fs_inode_includes(inode_سائز, i_checksum_hi))
		t->i_checksum_hi = ext2fs_swab16(f->i_checksum_hi);
	اگر (ext2fs_inode_includes(inode_سائز, i_ctime_extra))
		t->i_ctime_extra = ext2fs_swab32(f->i_ctime_extra);
	اگر (ext2fs_inode_includes(inode_سائز, i_mtime_extra))
		t->i_mtime_extra = ext2fs_swab32(f->i_mtime_extra);
	اگر (ext2fs_inode_includes(inode_سائز, i_atime_extra))
		t->i_atime_extra = ext2fs_swab32(f->i_atime_extra);
	اگر (ext2fs_inode_includes(inode_سائز, i_crtime))
		t->i_crtime = ext2fs_swab32(f->i_crtime);
	اگر (ext2fs_inode_includes(inode_سائز, i_crtime_extra))
		t->i_crtime_extra = ext2fs_swab32(f->i_crtime_extra);
	اگر (ext2fs_inode_includes(inode_سائز, i_نسخہ_hi))
		t->i_نسخہ_hi = ext2fs_swab32(f->i_نسخہ_hi);
	اگر (ext2fs_inode_includes(inode_سائز, i_projid))
                t->i_projid = ext2fs_swab32(f->i_projid);
	/* catch new static fields added after i_projid */
	EXT2FS_BUILD_BUG_ON(حجم(ساخت ext2_inode_large) != 160);

	i = حجم(ساخت ext2_inode) + extra_isize + حجم(__u32);
	اگر (bufsize < (صحیح) i)
		واپس; /* no space for EA magic */

	eaf = (__u32 *) (((حرف *) f) + حجم(ساخت ext2_inode) +
					extra_isize);

	attr_magic = *eaf;
	اگر (!hostorder)
		attr_magic = ext2fs_swab32(attr_magic);

	اگر (attr_magic != EXT2_EXT_ATTR_MAGIC)
		واپس; /* it seems no magic here */

	eat = (__u32 *) (((حرف *) t) + حجم(ساخت ext2_inode) +
					extra_isize);
	*eat = ext2fs_swab32(*eaf);

	/* convert EA(s) */
	ext2fs_تبادلہ_ext_attr((حرف *) (eat + 1), (حرف *) (eaf + 1),
			     bufsize - حجم(ساخت ext2_inode) -
			     extra_isize - حجم(__u32), 0);

}

خالی ext2fs_تبادلہ_inode(ext2_filsys fs, ساخت ext2_inode *t,
		       ساخت ext2_inode *f, صحیح hostorder)
{
	ext2fs_تبادلہ_inode_بھرا(fs, (ساخت ext2_inode_large *) t,
				(ساخت ext2_inode_large *) f, hostorder,
				حجم(ساخت ext2_inode));
}

خالی ext2fs_تبادلہ_mmp(ساخت mmp_struct *mmp)
{
	mmp->mmp_magic = ext2fs_swab32(mmp->mmp_magic);
	mmp->mmp_seq = ext2fs_swab32(mmp->mmp_seq);
	mmp->mmp_وقت = ext2fs_swab64(mmp->mmp_وقت);
	mmp->mmp_check_interval = ext2fs_swab16(mmp->mmp_check_interval);
	mmp->mmp_checksum = ext2fs_swab32(mmp->mmp_checksum);
}

errcode_t ext2fs_dirent_swab_اندر(ext2_filsys fs, حرف *buf, صحیح flags)
{
	واپس ext2fs_dirent_swab_in2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, حرف *buf,
				 size_t سائز, صحیح flags)
{
	errcode_t	retval;
	حرف		*p, *انتہا;
	ساخت ext2_dir_اندراج *dirent;
	غیرعلامتی صحیح	name_len, rec_len, بایاں;

	p = (حرف *) buf;
	انتہا = (حرف *) buf + سائز;
	بایاں = سائز;
	جب (p < انتہا-8) {
		dirent = (ساخت ext2_dir_اندراج *) p;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		name_len = dirent->name_len;
		اگر (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		اگر (retval)
			واپس retval;
		اگر ((rec_len < 8) || (rec_len % 4)) {
			rec_len = 8;
			اگر (!(fs->flags & EXT2_FLAG_IGNORE_تبادلہ_DIRENT))
				واپس EXT2_ET_DIR_CORRUPTED;
		} ورنہ اگر (((name_len & 0xFF) + 8) > rec_len)
			اگر (!(fs->flags & EXT2_FLAG_IGNORE_تبادلہ_DIRENT))
				واپس EXT2_ET_DIR_CORRUPTED;
		اگر (rec_len > بایاں)
			اگر (!(fs->flags & EXT2_FLAG_IGNORE_تبادلہ_DIRENT))
				واپس EXT2_ET_DIR_CORRUPTED;
		بایاں -= rec_len;
		p += rec_len;
	}

	واپس 0;
}

errcode_t ext2fs_dirent_swab_برآمد(ext2_filsys fs, حرف *buf, صحیح flags)
{
	واپس ext2fs_dirent_swab_out2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, حرف *buf,
				  size_t سائز, صحیح flags)
{
	errcode_t	retval;
	حرف		*p, *انتہا;
	غیرعلامتی صحیح	rec_len;
	ساخت ext2_dir_اندراج *dirent;

	p = buf;
	انتہا = buf + سائز;
	جب (p < انتہا) {
		dirent = (ساخت ext2_dir_اندراج *) p;
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		اگر (retval)
			واپس retval;
		اگر ((rec_len < 8) ||
		    (rec_len % 4)) {
			ext2fs_free_mem(&buf);
			واپس EXT2_ET_DIR_CORRUPTED;
		}
		p += rec_len;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		اگر (rec_len > سائز)
			واپس EXT2_ET_DIR_CORRUPTED;
		سائز -= rec_len;

		اگر (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
	}

	واپس 0;
}
