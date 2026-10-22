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

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"
#ضمن <ext2fs/ext2_ext_attr.h>

فراغ ext2fs_بدلون_super(بنية ext2_super_بلاک * sb)
{
	صحيح i;

	sb->s_inodes_شمېر = ext2fs_swab32(sb->s_inodes_شمېر);
	sb->s_بلاکونه_شمېر = ext2fs_swab32(sb->s_بلاکونه_شمېر);
	sb->s_r_بلاکونه_شمېر = ext2fs_swab32(sb->s_r_بلاکونه_شمېر);
	sb->s_free_بلاکونه_شمېر = ext2fs_swab32(sb->s_free_بلاکونه_شمېر);
	sb->s_free_inodes_شمېر = ext2fs_swab32(sb->s_free_inodes_شمېر);
	sb->s_لومړی_معلومات_بلاک = ext2fs_swab32(sb->s_لومړی_معلومات_بلاک);
	sb->s_log_بلاک_اندازه = ext2fs_swab32(sb->s_log_بلاک_اندازه);
	sb->s_log_cluster_اندازه = ext2fs_swab32(sb->s_log_cluster_اندازه);
	sb->s_بلاکونه_فی_ډله = ext2fs_swab32(sb->s_بلاکونه_فی_ډله);
	sb->s_clusters_فی_ډله = ext2fs_swab32(sb->s_clusters_فی_ډله);
	sb->s_inodes_فی_ډله = ext2fs_swab32(sb->s_inodes_فی_ډله);
	sb->s_mtime = ext2fs_swab32(sb->s_mtime);
	sb->s_wtime = ext2fs_swab32(sb->s_wtime);
	sb->s_mnt_شمېر = ext2fs_swab16(sb->s_mnt_شمېر);
	sb->s_max_mnt_شمېر = ext2fs_swab16(sb->s_max_mnt_شمېر);
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
	sb->s_لومړی_ino = ext2fs_swab32(sb->s_لومړی_ino);
	sb->s_inode_اندازه = ext2fs_swab16(sb->s_inode_اندازه);
	sb->s_بلاک_ډله_nr = ext2fs_swab16(sb->s_بلاک_ډله_nr);
	sb->s_feature_compat = ext2fs_swab32(sb->s_feature_compat);
	sb->s_feature_incompat = ext2fs_swab32(sb->s_feature_incompat);
	sb->s_feature_ro_compat = ext2fs_swab32(sb->s_feature_ro_compat);
	/* sb->s_uuid is __u8 and does not need swabbing */
	/* sb->s_volume_name is char and does not need swabbing */
	/* sb->s_last_mounted is char and does not need swabbing */
	sb->s_algorithm_usage_bitmap = ext2fs_swab32(sb->s_algorithm_usage_bitmap);
	/* sb->s_prealloc_blocks is __u8 and does not need swabbing */
	/* sb->s_prealloc_dir_blocks is __u8 and does not need swabbing */
	sb->s_reserved_gdt_بلاکونه = ext2fs_swab16(sb->s_reserved_gdt_بلاکونه);
	/* sb->s_journal_uuid is __u8 and does not need swabbing */
	sb->s_journal_inum = ext2fs_swab32(sb->s_journal_inum);
	sb->s_journal_dev = ext2fs_swab32(sb->s_journal_dev);
	sb->s_وروستی_orphan = ext2fs_swab32(sb->s_وروستی_orphan);
	لكل (i = 0; i < 4; i++)
		sb->s_hash_تخم[i] = ext2fs_swab32(sb->s_hash_تخم[i]);
	/* sb->s_def_hash_version is __u8 and does not need swabbing */
	/* sb->s_jnl_backup_type is __u8 and does not need swabbing */
	sb->s_desc_اندازه = ext2fs_swab16(sb->s_desc_اندازه);
	sb->s_default_mount_opts = ext2fs_swab32(sb->s_default_mount_opts);
	sb->s_لومړی_meta_bg = ext2fs_swab32(sb->s_لومړی_meta_bg);
	sb->s_mkfs_وخت = ext2fs_swab32(sb->s_mkfs_وخت);
	/* if journal backup is for a valid extent-based journal... */
	إذا (ext2fs_extent_سرلیک_verify(sb->s_jnl_بلاکونه,
					حجم(sb->s_jnl_بلاکونه)) == 0) {
		/* ... swap only the journal i_size and i_size_high,
		 * and the extent data is not swapped on read */
		i = 15;
	} وإلا {
		/* direct/indirect journal: swap it all */
		i = 0;
	}
	لكل (; i < 17; i++)
		sb->s_jnl_بلاکونه[i] = ext2fs_swab32(sb->s_jnl_بلاکونه[i]);
	sb->s_بلاکونه_شمېر_hi = ext2fs_swab32(sb->s_بلاکونه_شمېر_hi);
	sb->s_r_بلاکونه_شمېر_hi = ext2fs_swab32(sb->s_r_بلاکونه_شمېر_hi);
	sb->s_free_بلاکونه_hi = ext2fs_swab32(sb->s_free_بلاکونه_hi);
	sb->s_min_extra_isize = ext2fs_swab16(sb->s_min_extra_isize);
	sb->s_want_extra_isize = ext2fs_swab16(sb->s_want_extra_isize);
	sb->s_flags = ext2fs_swab32(sb->s_flags);
	sb->s_raid_stride = ext2fs_swab16(sb->s_raid_stride);
	sb->s_mmp_update_interval = ext2fs_swab16(sb->s_mmp_update_interval);
	sb->s_mmp_بلاک = ext2fs_swab64(sb->s_mmp_بلاک);
	sb->s_raid_stripe_پلنوالی = ext2fs_swab32(sb->s_raid_stripe_پلنوالی);
	/* sb->s_log_groups_per_flex is __u8 and does not need swabbing */
	/* sb->s_checksum_type is __u8 and does not need swabbing */
	/* sb->s_encryption_level is __u8 and does not need swabbing */
	/* sb->s_reserved_pad is __u8 and does not need swabbing */
	sb->s_kbytes_written = ext2fs_swab64(sb->s_kbytes_written);
	sb->s_snapshot_inum = ext2fs_swab32(sb->s_snapshot_inum);
	sb->s_snapshot_id = ext2fs_swab32(sb->s_snapshot_id);
	sb->s_snapshot_r_بلاکونه_شمېر =
		ext2fs_swab64(sb->s_snapshot_r_بلاکونه_شمېر);
	sb->s_snapshot_list = ext2fs_swab32(sb->s_snapshot_list);
	sb->s_error_شمېر = ext2fs_swab32(sb->s_error_شمېر);
	sb->s_لومړی_error_وخت = ext2fs_swab32(sb->s_لومړی_error_وخت);
	sb->s_لومړی_error_ino = ext2fs_swab32(sb->s_لومړی_error_ino);
	sb->s_لومړی_error_بلاک = ext2fs_swab64(sb->s_لومړی_error_بلاک);
	/* sb->s_first_error_func is __u8 and does not need swabbing */
	sb->s_وروستی_error_وخت = ext2fs_swab32(sb->s_وروستی_error_وخت);
	sb->s_وروستی_error_ino = ext2fs_swab32(sb->s_وروستی_error_ino);
	sb->s_وروستی_error_بلاک = ext2fs_swab64(sb->s_وروستی_error_بلاک);
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
	sb->s_checksum_تخم = ext2fs_swab32(sb->s_checksum_تخم);
	/* s_*_time_hi are __u8 and does not need swabbing */
	sb->s_encoding = ext2fs_swab16(sb->s_encoding);
	sb->s_encoding_flags = ext2fs_swab16(sb->s_encoding_flags);
	sb->s_orphan_file_inum = ext2fs_swab32(sb->s_orphan_file_inum);
	/* catch when new fields are used from s_reserved */
	EXT2FS_BUILD_BUG_ON(حجم(sb->s_reserved) != 94 * حجم(__le32));
	sb->s_checksum = ext2fs_swab32(sb->s_checksum);
}

فراغ ext2fs_بدلون_ډله_desc2(ext2_filsys fs, بنية ext2_ډله_desc *gdp)
{
	بنية ext4_ډله_desc *gdp4 = (بنية ext4_ډله_desc *)gdp;

	/* Do the 32-bit parts first */
	gdp->bg_بلاک_bitmap = ext2fs_swab32(gdp->bg_بلاک_bitmap);
	gdp->bg_inode_bitmap = ext2fs_swab32(gdp->bg_inode_bitmap);
	gdp->bg_inode_جدول = ext2fs_swab32(gdp->bg_inode_جدول);
	gdp->bg_free_بلاکونه_شمېر = ext2fs_swab16(gdp->bg_free_بلاکونه_شمېر);
	gdp->bg_free_inodes_شمېر = ext2fs_swab16(gdp->bg_free_inodes_شمېر);
	gdp->bg_کارول_شوی_dirs_شمېر = ext2fs_swab16(gdp->bg_کارول_شوی_dirs_شمېر);
	gdp->bg_flags = ext2fs_swab16(gdp->bg_flags);
	gdp->bg_exclude_bitmap_lo = ext2fs_swab32(gdp->bg_exclude_bitmap_lo);
	gdp->bg_بلاک_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_بلاک_bitmap_csum_lo);
	gdp->bg_inode_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_inode_bitmap_csum_lo);
	gdp->bg_itable_ناکارول_شوی = ext2fs_swab16(gdp->bg_itable_ناکارول_شوی);
	gdp->bg_checksum = ext2fs_swab16(gdp->bg_checksum);
	/* If we're 32-bit, we're done */
	إذا (fs == عدم || EXT2_DESC_اندازه(fs->super) < EXT2_MIN_DESC_اندازه_64_بټ)
		أرجع;

	/* Swap the 64-bit parts */
	gdp4->bg_بلاک_bitmap_hi = ext2fs_swab32(gdp4->bg_بلاک_bitmap_hi);
	gdp4->bg_inode_bitmap_hi = ext2fs_swab32(gdp4->bg_inode_bitmap_hi);
	gdp4->bg_inode_جدول_hi = ext2fs_swab32(gdp4->bg_inode_جدول_hi);
	gdp4->bg_free_بلاکونه_شمېر_hi =
		ext2fs_swab16(gdp4->bg_free_بلاکونه_شمېر_hi);
	gdp4->bg_free_inodes_شمېر_hi =
		ext2fs_swab16(gdp4->bg_free_inodes_شمېر_hi);
	gdp4->bg_کارول_شوی_dirs_شمېر_hi =
		ext2fs_swab16(gdp4->bg_کارول_شوی_dirs_شمېر_hi);
	gdp4->bg_itable_ناکارول_شوی_hi = ext2fs_swab16(gdp4->bg_itable_ناکارول_شوی_hi);
	gdp4->bg_exclude_bitmap_hi = ext2fs_swab16(gdp4->bg_exclude_bitmap_hi);
	gdp4->bg_بلاک_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_بلاک_bitmap_csum_hi);
	gdp4->bg_inode_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_inode_bitmap_csum_hi);
	EXT2FS_BUILD_BUG_ON(حجم(gdp4->bg_reserved) != حجم(__u32));
}

فراغ ext2fs_بدلون_ډله_desc(بنية ext2_ډله_desc *gdp)
{
	ext2fs_بدلون_ډله_desc2(0, gdp);
}


فراغ ext2fs_بدلون_ext_attr_سرلیک(بنية ext2_ext_attr_سرلیک *منزل_سرلیک,
				 بنية ext2_ext_attr_سرلیک *from_سرلیک)
{
	صحيح n;

	منزل_سرلیک->h_magic    = ext2fs_swab32(from_سرلیک->h_magic);
	منزل_سرلیک->h_بلاکونه   = ext2fs_swab32(from_سرلیک->h_بلاکونه);
	منزل_سرلیک->h_refcount = ext2fs_swab32(from_سرلیک->h_refcount);
	منزل_سرلیک->h_hash     = ext2fs_swab32(from_سرلیک->h_hash);
	منزل_سرلیک->h_checksum = ext2fs_swab32(from_سرلیک->h_checksum);
	لكل (n = 0; n < 3; n++)
		منزل_سرلیک->h_reserved[n] =
			ext2fs_swab32(from_سرلیک->h_reserved[n]);
}

فراغ ext2fs_بدلون_ext_attr_ثبت(بنية ext2_ext_attr_ثبت *منزل_ثبت,
				بنية ext2_ext_attr_ثبت *from_ثبت)
{
	منزل_ثبت->e_ارزښت_offs  = ext2fs_swab16(from_ثبت->e_ارزښت_offs);
	منزل_ثبت->e_ارزښت_inum  = ext2fs_swab32(from_ثبت->e_ارزښت_inum);
	منزل_ثبت->e_ارزښت_اندازه  = ext2fs_swab32(from_ثبت->e_ارزښت_اندازه);
	منزل_ثبت->e_hash	= ext2fs_swab32(from_ثبت->e_hash);
}

فراغ ext2fs_بدلون_ext_attr(حرف *منزل, حرف *from, صحيح bufsize, صحيح has_سرلیک)
{
	بنية ext2_ext_attr_سرلیک *from_سرلیک =
		(بنية ext2_ext_attr_سرلیک *)from;
	بنية ext2_ext_attr_سرلیک *منزل_سرلیک =
		(بنية ext2_ext_attr_سرلیک *)منزل;
	بنية ext2_ext_attr_ثبت *from_ثبت, *منزل_ثبت;
	حرف *from_پای_ټکی = (حرف *)from_سرلیک + bufsize;

	إذا (منزل_سرلیک != from_سرلیک)
		حافظه_نقل(منزل_سرلیک, from_سرلیک, bufsize);

	إذا (has_سرلیک) {
		ext2fs_بدلون_ext_attr_سرلیک(منزل_سرلیک, from_سرلیک);

		from_ثبت = (بنية ext2_ext_attr_ثبت *)(from_سرلیک+1);
		منزل_ثبت   = (بنية ext2_ext_attr_ثبت *)(منزل_سرلیک+1);
	} وإلا {
		from_ثبت = (بنية ext2_ext_attr_ثبت *)from_سرلیک;
		منزل_ثبت   = (بنية ext2_ext_attr_ثبت *)منزل_سرلیک;
	}

	طالما ((حرف *)from_ثبت < from_پای_ټکی &&
	       (حرف *)EXT2_EXT_ATTR_بل(from_ثبت) <= from_پای_ټکی &&
	       *(__u32 *)from_ثبت) {
		ext2fs_بدلون_ext_attr_ثبت(منزل_ثبت, from_ثبت);
		from_ثبت = EXT2_EXT_ATTR_بل(from_ثبت);
		منزل_ثبت   = EXT2_EXT_ATTR_بل(منزل_ثبت);
	}
}

فراغ ext2fs_بدلون_inode_ډک(ext2_filsys fs, بنية ext2_inode_large *t,
			    بنية ext2_inode_large *f, صحيح hostorder,
			    صحيح bufsize)
{
	بینښه i, extra_isize, attr_magic;
	صحيح has_extents = 0, has_inline_معلومات = 0, islnk = 0, fast_symlink = 0;
	بینښه صحيح inode_اندازه;
	__u32 *eaf, *eat;

	/*
	 * Note that t and f may point to the same address. That's why
	 * if (hostorder) condition is executed before swab calls and
	 * if (!hostorder) afterwards.
	 */
	إذا (hostorder) {
		islnk = LINUX_S_ISLNK(f->i_ډېرپېښ);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(f));
		has_extents = (f->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_معلومات = (f->i_flags & EXT4_INLINE_معلومات_FL) != 0;
	}

	t->i_ډېرپېښ = ext2fs_swab16(f->i_ډېرپېښ);
	t->i_uid = ext2fs_swab16(f->i_uid);
	t->i_اندازه = ext2fs_swab32(f->i_اندازه);
	t->i_atime = ext2fs_swab32(f->i_atime);
	t->i_ctime = ext2fs_swab32(f->i_ctime);
	t->i_mtime = ext2fs_swab32(f->i_mtime);
	t->i_dtime = ext2fs_swab32(f->i_dtime);
	t->i_gid = ext2fs_swab16(f->i_gid);
	t->i_links_شمېر = ext2fs_swab16(f->i_links_شمېر);
	t->i_file_acl = ext2fs_swab32(f->i_file_acl);
	t->i_بلاکونه = ext2fs_swab32(f->i_بلاکونه);
	t->i_flags = ext2fs_swab32(f->i_flags);
	t->i_اندازه_لوړ = ext2fs_swab32(f->i_اندازه_لوړ);

	إذا (!hostorder) {
		islnk = LINUX_S_ISLNK(t->i_ډېرپېښ);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(t));
		has_extents = (t->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_معلومات = (t->i_flags & EXT4_INLINE_معلومات_FL) != 0;
	}

	/*
	 * Extent data and inline data are swapped on access, not here
	 */
	إذا (!has_extents && !has_inline_معلومات && (!islnk || !fast_symlink)) {
		لكل (i = 0; i < EXT2_N_بلاکونه; i++)
			t->i_بلاک[i] = ext2fs_swab32(f->i_بلاک[i]);
	} وإلا إذا (t != f) {
		لكل (i = 0; i < EXT2_N_بلاکونه; i++)
			t->i_بلاک[i] = f->i_بلاک[i];
	}
	t->i_generation = ext2fs_swab32(f->i_generation);
	t->i_faddr = ext2fs_swab32(f->i_faddr);

	بدل (fs->super->s_creator_os) {
	حالة EXT2_OS_LINUX:
		t->osd1.linux1.l_i_نسخه =
			ext2fs_swab32(f->osd1.linux1.l_i_نسخه);
		t->osd2.linux2.l_i_بلاکونه_hi =
			ext2fs_swab16(f->osd2.linux2.l_i_بلاکونه_hi);
		t->osd2.linux2.l_i_file_acl_لوړ =
			ext2fs_swab16(f->osd2.linux2.l_i_file_acl_لوړ);
		t->osd2.linux2.l_i_uid_لوړ =
		  ext2fs_swab16 (f->osd2.linux2.l_i_uid_لوړ);
		t->osd2.linux2.l_i_gid_لوړ =
		  ext2fs_swab16 (f->osd2.linux2.l_i_gid_لوړ);
		t->osd2.linux2.l_i_checksum_lo =
			ext2fs_swab16(f->osd2.linux2.l_i_checksum_lo);
		اكسر;
	حالة EXT2_OS_HURD:
		t->osd1.hurd1.h_i_translator =
		  ext2fs_swab32 (f->osd1.hurd1.h_i_translator);
		t->osd2.hurd2.h_i_frag = f->osd2.hurd2.h_i_frag;
		t->osd2.hurd2.h_i_fsize = f->osd2.hurd2.h_i_fsize;
		t->osd2.hurd2.h_i_ډېرپېښ_لوړ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_ډېرپېښ_لوړ);
		t->osd2.hurd2.h_i_uid_لوړ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_uid_لوړ);
		t->osd2.hurd2.h_i_gid_لوړ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_gid_لوړ);
		t->osd2.hurd2.h_i_author =
		  ext2fs_swab32 (f->osd2.hurd2.h_i_author);
		اكسر;
	افتراضي:
		اكسر;
	}

	إذا (bufsize < (صحيح) (حجم(بنية ext2_inode) + حجم(__u16)))
		أرجع; /* no i_extra_isize field */

	إذا (hostorder)
		extra_isize = f->i_extra_isize;
	t->i_extra_isize = ext2fs_swab16(f->i_extra_isize);
	إذا (!hostorder)
		extra_isize = t->i_extra_isize;
	إذا (extra_isize > EXT2_INODE_اندازه(fs->super) -
				حجم(بنية ext2_inode)) {
		/* this is error case: i_extra_size is too large */
		أرجع;
	}
	إذا (extra_isize & 3)
		أرجع;		/* Illegal inode extra_isize */

	inode_اندازه = EXT2_GOOD_OLD_INODE_اندازه + extra_isize;
	إذا (ext2fs_inode_includes(inode_اندازه, i_checksum_hi))
		t->i_checksum_hi = ext2fs_swab16(f->i_checksum_hi);
	إذا (ext2fs_inode_includes(inode_اندازه, i_ctime_extra))
		t->i_ctime_extra = ext2fs_swab32(f->i_ctime_extra);
	إذا (ext2fs_inode_includes(inode_اندازه, i_mtime_extra))
		t->i_mtime_extra = ext2fs_swab32(f->i_mtime_extra);
	إذا (ext2fs_inode_includes(inode_اندازه, i_atime_extra))
		t->i_atime_extra = ext2fs_swab32(f->i_atime_extra);
	إذا (ext2fs_inode_includes(inode_اندازه, i_crtime))
		t->i_crtime = ext2fs_swab32(f->i_crtime);
	إذا (ext2fs_inode_includes(inode_اندازه, i_crtime_extra))
		t->i_crtime_extra = ext2fs_swab32(f->i_crtime_extra);
	إذا (ext2fs_inode_includes(inode_اندازه, i_نسخه_hi))
		t->i_نسخه_hi = ext2fs_swab32(f->i_نسخه_hi);
	إذا (ext2fs_inode_includes(inode_اندازه, i_projid))
                t->i_projid = ext2fs_swab32(f->i_projid);
	/* catch new static fields added after i_projid */
	EXT2FS_BUILD_BUG_ON(حجم(بنية ext2_inode_large) != 160);

	i = حجم(بنية ext2_inode) + extra_isize + حجم(__u32);
	إذا (bufsize < (صحيح) i)
		أرجع; /* no space for EA magic */

	eaf = (__u32 *) (((حرف *) f) + حجم(بنية ext2_inode) +
					extra_isize);

	attr_magic = *eaf;
	إذا (!hostorder)
		attr_magic = ext2fs_swab32(attr_magic);

	إذا (attr_magic != EXT2_EXT_ATTR_MAGIC)
		أرجع; /* it seems no magic here */

	eat = (__u32 *) (((حرف *) t) + حجم(بنية ext2_inode) +
					extra_isize);
	*eat = ext2fs_swab32(*eaf);

	/* convert EA(s) */
	ext2fs_بدلون_ext_attr((حرف *) (eat + 1), (حرف *) (eaf + 1),
			     bufsize - حجم(بنية ext2_inode) -
			     extra_isize - حجم(__u32), 0);

}

فراغ ext2fs_بدلون_inode(ext2_filsys fs, بنية ext2_inode *t,
		       بنية ext2_inode *f, صحيح hostorder)
{
	ext2fs_بدلون_inode_ډک(fs, (بنية ext2_inode_large *) t,
				(بنية ext2_inode_large *) f, hostorder,
				حجم(بنية ext2_inode));
}

فراغ ext2fs_بدلون_mmp(بنية mmp_struct *mmp)
{
	mmp->mmp_magic = ext2fs_swab32(mmp->mmp_magic);
	mmp->mmp_seq = ext2fs_swab32(mmp->mmp_seq);
	mmp->mmp_وخت = ext2fs_swab64(mmp->mmp_وخت);
	mmp->mmp_check_interval = ext2fs_swab16(mmp->mmp_check_interval);
	mmp->mmp_checksum = ext2fs_swab32(mmp->mmp_checksum);
}

errcode_t ext2fs_dirent_swab_دننه(ext2_filsys fs, حرف *buf, صحيح flags)
{
	أرجع ext2fs_dirent_swab_in2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, حرف *buf,
				 size_t اندازه, صحيح flags)
{
	errcode_t	retval;
	حرف		*p, *پای_ټکی;
	بنية ext2_dir_ثبت *dirent;
	بینښه صحيح	name_len, rec_len, کیڼ;

	p = (حرف *) buf;
	پای_ټکی = (حرف *) buf + اندازه;
	کیڼ = اندازه;
	طالما (p < پای_ټکی-8) {
		dirent = (بنية ext2_dir_ثبت *) p;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		name_len = dirent->name_len;
		إذا (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		إذا (retval)
			أرجع retval;
		إذا ((rec_len < 8) || (rec_len % 4)) {
			rec_len = 8;
			إذا (!(fs->flags & EXT2_FLAG_IGNORE_بدلون_DIRENT))
				أرجع EXT2_ET_DIR_CORRUPTED;
		} وإلا إذا (((name_len & 0xFF) + 8) > rec_len)
			إذا (!(fs->flags & EXT2_FLAG_IGNORE_بدلون_DIRENT))
				أرجع EXT2_ET_DIR_CORRUPTED;
		إذا (rec_len > کیڼ)
			إذا (!(fs->flags & EXT2_FLAG_IGNORE_بدلون_DIRENT))
				أرجع EXT2_ET_DIR_CORRUPTED;
		کیڼ -= rec_len;
		p += rec_len;
	}

	أرجع 0;
}

errcode_t ext2fs_dirent_swab_بهر(ext2_filsys fs, حرف *buf, صحيح flags)
{
	أرجع ext2fs_dirent_swab_out2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, حرف *buf,
				  size_t اندازه, صحيح flags)
{
	errcode_t	retval;
	حرف		*p, *پای_ټکی;
	بینښه صحيح	rec_len;
	بنية ext2_dir_ثبت *dirent;

	p = buf;
	پای_ټکی = buf + اندازه;
	طالما (p < پای_ټکی) {
		dirent = (بنية ext2_dir_ثبت *) p;
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		إذا (retval)
			أرجع retval;
		إذا ((rec_len < 8) ||
		    (rec_len % 4)) {
			ext2fs_free_mem(&buf);
			أرجع EXT2_ET_DIR_CORRUPTED;
		}
		p += rec_len;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		إذا (rec_len > اندازه)
			أرجع EXT2_ET_DIR_CORRUPTED;
		اندازه -= rec_len;

		إذا (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
	}

	أرجع 0;
}
