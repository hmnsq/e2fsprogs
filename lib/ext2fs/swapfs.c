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
#جيڪڏهن_پورو
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"
#ضمن <ext2fs/ext2_ext_attr.h>

فراغ ext2fs_مٽا_سٽا_super(بنية ext2_super_بلاڪ * sb)
{
	صحيح i;

	sb->s_inodes_ڳڻپ = ext2fs_swab32(sb->s_inodes_ڳڻپ);
	sb->s_بلاڪون_ڳڻپ = ext2fs_swab32(sb->s_بلاڪون_ڳڻپ);
	sb->s_r_بلاڪون_ڳڻپ = ext2fs_swab32(sb->s_r_بلاڪون_ڳڻپ);
	sb->s_free_بلاڪون_ڳڻپ = ext2fs_swab32(sb->s_free_بلاڪون_ڳڻپ);
	sb->s_free_inodes_ڳڻپ = ext2fs_swab32(sb->s_free_inodes_ڳڻپ);
	sb->s_پهريون_معلومات_بلاڪ = ext2fs_swab32(sb->s_پهريون_معلومات_بلاڪ);
	sb->s_log_بلاڪ_سائيز = ext2fs_swab32(sb->s_log_بلاڪ_سائيز);
	sb->s_log_cluster_سائيز = ext2fs_swab32(sb->s_log_cluster_سائيز);
	sb->s_بلاڪون_في_گروهه = ext2fs_swab32(sb->s_بلاڪون_في_گروهه);
	sb->s_clusters_في_گروهه = ext2fs_swab32(sb->s_clusters_في_گروهه);
	sb->s_inodes_في_گروهه = ext2fs_swab32(sb->s_inodes_في_گروهه);
	sb->s_mtime = ext2fs_swab32(sb->s_mtime);
	sb->s_wtime = ext2fs_swab32(sb->s_wtime);
	sb->s_mnt_ڳڻپ = ext2fs_swab16(sb->s_mnt_ڳڻپ);
	sb->s_max_mnt_ڳڻپ = ext2fs_swab16(sb->s_max_mnt_ڳڻپ);
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
	sb->s_پهريون_ino = ext2fs_swab32(sb->s_پهريون_ino);
	sb->s_inode_سائيز = ext2fs_swab16(sb->s_inode_سائيز);
	sb->s_بلاڪ_گروهه_nr = ext2fs_swab16(sb->s_بلاڪ_گروهه_nr);
	sb->s_feature_compat = ext2fs_swab32(sb->s_feature_compat);
	sb->s_feature_incompat = ext2fs_swab32(sb->s_feature_incompat);
	sb->s_feature_ro_compat = ext2fs_swab32(sb->s_feature_ro_compat);
	/* sb->s_uuid is __u8 and does not need swabbing */
	/* sb->s_volume_name is char and does not need swabbing */
	/* sb->s_last_mounted is char and does not need swabbing */
	sb->s_algorithm_usage_bitmap = ext2fs_swab32(sb->s_algorithm_usage_bitmap);
	/* sb->s_prealloc_blocks is __u8 and does not need swabbing */
	/* sb->s_prealloc_dir_blocks is __u8 and does not need swabbing */
	sb->s_reserved_gdt_بلاڪون = ext2fs_swab16(sb->s_reserved_gdt_بلاڪون);
	/* sb->s_journal_uuid is __u8 and does not need swabbing */
	sb->s_journal_inum = ext2fs_swab32(sb->s_journal_inum);
	sb->s_journal_dev = ext2fs_swab32(sb->s_journal_dev);
	sb->s_آخري_orphan = ext2fs_swab32(sb->s_آخري_orphan);
	لكل (i = 0; i < 4; i++)
		sb->s_hash_ٻج[i] = ext2fs_swab32(sb->s_hash_ٻج[i]);
	/* sb->s_def_hash_version is __u8 and does not need swabbing */
	/* sb->s_jnl_backup_type is __u8 and does not need swabbing */
	sb->s_desc_سائيز = ext2fs_swab16(sb->s_desc_سائيز);
	sb->s_default_mount_opts = ext2fs_swab32(sb->s_default_mount_opts);
	sb->s_پهريون_meta_bg = ext2fs_swab32(sb->s_پهريون_meta_bg);
	sb->s_mkfs_وقت = ext2fs_swab32(sb->s_mkfs_وقت);
	/* if journal backup is for a valid extent-based journal... */
	إذا (ext2fs_extent_سرو_verify(sb->s_jnl_بلاڪون,
					حجم(sb->s_jnl_بلاڪون)) == 0) {
		/* ... swap only the journal i_size and i_size_high,
		 * and the extent data is not swapped on read */
		i = 15;
	} وإلا {
		/* direct/indirect journal: swap it all */
		i = 0;
	}
	لكل (; i < 17; i++)
		sb->s_jnl_بلاڪون[i] = ext2fs_swab32(sb->s_jnl_بلاڪون[i]);
	sb->s_بلاڪون_ڳڻپ_hi = ext2fs_swab32(sb->s_بلاڪون_ڳڻپ_hi);
	sb->s_r_بلاڪون_ڳڻپ_hi = ext2fs_swab32(sb->s_r_بلاڪون_ڳڻپ_hi);
	sb->s_free_بلاڪون_hi = ext2fs_swab32(sb->s_free_بلاڪون_hi);
	sb->s_min_extra_isize = ext2fs_swab16(sb->s_min_extra_isize);
	sb->s_want_extra_isize = ext2fs_swab16(sb->s_want_extra_isize);
	sb->s_flags = ext2fs_swab32(sb->s_flags);
	sb->s_raid_stride = ext2fs_swab16(sb->s_raid_stride);
	sb->s_mmp_update_interval = ext2fs_swab16(sb->s_mmp_update_interval);
	sb->s_mmp_بلاڪ = ext2fs_swab64(sb->s_mmp_بلاڪ);
	sb->s_raid_stripe_ويڪر = ext2fs_swab32(sb->s_raid_stripe_ويڪر);
	/* sb->s_log_groups_per_flex is __u8 and does not need swabbing */
	/* sb->s_checksum_type is __u8 and does not need swabbing */
	/* sb->s_encryption_level is __u8 and does not need swabbing */
	/* sb->s_reserved_pad is __u8 and does not need swabbing */
	sb->s_kbytes_written = ext2fs_swab64(sb->s_kbytes_written);
	sb->s_snapshot_inum = ext2fs_swab32(sb->s_snapshot_inum);
	sb->s_snapshot_id = ext2fs_swab32(sb->s_snapshot_id);
	sb->s_snapshot_r_بلاڪون_ڳڻپ =
		ext2fs_swab64(sb->s_snapshot_r_بلاڪون_ڳڻپ);
	sb->s_snapshot_list = ext2fs_swab32(sb->s_snapshot_list);
	sb->s_error_ڳڻپ = ext2fs_swab32(sb->s_error_ڳڻپ);
	sb->s_پهريون_error_وقت = ext2fs_swab32(sb->s_پهريون_error_وقت);
	sb->s_پهريون_error_ino = ext2fs_swab32(sb->s_پهريون_error_ino);
	sb->s_پهريون_error_بلاڪ = ext2fs_swab64(sb->s_پهريون_error_بلاڪ);
	/* sb->s_first_error_func is __u8 and does not need swabbing */
	sb->s_آخري_error_وقت = ext2fs_swab32(sb->s_آخري_error_وقت);
	sb->s_آخري_error_ino = ext2fs_swab32(sb->s_آخري_error_ino);
	sb->s_آخري_error_بلاڪ = ext2fs_swab64(sb->s_آخري_error_بلاڪ);
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
	sb->s_checksum_ٻج = ext2fs_swab32(sb->s_checksum_ٻج);
	/* s_*_time_hi are __u8 and does not need swabbing */
	sb->s_encoding = ext2fs_swab16(sb->s_encoding);
	sb->s_encoding_flags = ext2fs_swab16(sb->s_encoding_flags);
	sb->s_orphan_file_inum = ext2fs_swab32(sb->s_orphan_file_inum);
	/* catch when new fields are used from s_reserved */
	EXT2FS_BUILD_BUG_ON(حجم(sb->s_reserved) != 94 * حجم(__le32));
	sb->s_checksum = ext2fs_swab32(sb->s_checksum);
}

فراغ ext2fs_مٽا_سٽا_گروهه_desc2(ext2_filsys fs, بنية ext2_گروهه_desc *gdp)
{
	بنية ext4_گروهه_desc *gdp4 = (بنية ext4_گروهه_desc *)gdp;

	/* Do the 32-bit parts first */
	gdp->bg_بلاڪ_bitmap = ext2fs_swab32(gdp->bg_بلاڪ_bitmap);
	gdp->bg_inode_bitmap = ext2fs_swab32(gdp->bg_inode_bitmap);
	gdp->bg_inode_جدول = ext2fs_swab32(gdp->bg_inode_جدول);
	gdp->bg_free_بلاڪون_ڳڻپ = ext2fs_swab16(gdp->bg_free_بلاڪون_ڳڻپ);
	gdp->bg_free_inodes_ڳڻپ = ext2fs_swab16(gdp->bg_free_inodes_ڳڻپ);
	gdp->bg_استعمال_ٿيل_dirs_ڳڻپ = ext2fs_swab16(gdp->bg_استعمال_ٿيل_dirs_ڳڻپ);
	gdp->bg_flags = ext2fs_swab16(gdp->bg_flags);
	gdp->bg_exclude_bitmap_lo = ext2fs_swab32(gdp->bg_exclude_bitmap_lo);
	gdp->bg_بلاڪ_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_بلاڪ_bitmap_csum_lo);
	gdp->bg_inode_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_inode_bitmap_csum_lo);
	gdp->bg_itable_اڻ_استعمال_ٿيل = ext2fs_swab16(gdp->bg_itable_اڻ_استعمال_ٿيل);
	gdp->bg_checksum = ext2fs_swab16(gdp->bg_checksum);
	/* If we're 32-bit, we're done */
	إذا (fs == عدم || EXT2_DESC_سائيز(fs->super) < EXT2_MIN_DESC_سائيز_64_ٻٽو_انگ)
		أرجع;

	/* Swap the 64-bit parts */
	gdp4->bg_بلاڪ_bitmap_hi = ext2fs_swab32(gdp4->bg_بلاڪ_bitmap_hi);
	gdp4->bg_inode_bitmap_hi = ext2fs_swab32(gdp4->bg_inode_bitmap_hi);
	gdp4->bg_inode_جدول_hi = ext2fs_swab32(gdp4->bg_inode_جدول_hi);
	gdp4->bg_free_بلاڪون_ڳڻپ_hi =
		ext2fs_swab16(gdp4->bg_free_بلاڪون_ڳڻپ_hi);
	gdp4->bg_free_inodes_ڳڻپ_hi =
		ext2fs_swab16(gdp4->bg_free_inodes_ڳڻپ_hi);
	gdp4->bg_استعمال_ٿيل_dirs_ڳڻپ_hi =
		ext2fs_swab16(gdp4->bg_استعمال_ٿيل_dirs_ڳڻپ_hi);
	gdp4->bg_itable_اڻ_استعمال_ٿيل_hi = ext2fs_swab16(gdp4->bg_itable_اڻ_استعمال_ٿيل_hi);
	gdp4->bg_exclude_bitmap_hi = ext2fs_swab16(gdp4->bg_exclude_bitmap_hi);
	gdp4->bg_بلاڪ_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_بلاڪ_bitmap_csum_hi);
	gdp4->bg_inode_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_inode_bitmap_csum_hi);
	EXT2FS_BUILD_BUG_ON(حجم(gdp4->bg_reserved) != حجم(__u32));
}

فراغ ext2fs_مٽا_سٽا_گروهه_desc(بنية ext2_گروهه_desc *gdp)
{
	ext2fs_مٽا_سٽا_گروهه_desc2(0, gdp);
}


فراغ ext2fs_مٽا_سٽا_ext_attr_سرو(بنية ext2_ext_attr_سرو *منزل_سرو,
				 بنية ext2_ext_attr_سرو *from_سرو)
{
	صحيح n;

	منزل_سرو->h_magic    = ext2fs_swab32(from_سرو->h_magic);
	منزل_سرو->h_بلاڪون   = ext2fs_swab32(from_سرو->h_بلاڪون);
	منزل_سرو->h_refcount = ext2fs_swab32(from_سرو->h_refcount);
	منزل_سرو->h_hash     = ext2fs_swab32(from_سرو->h_hash);
	منزل_سرو->h_checksum = ext2fs_swab32(from_سرو->h_checksum);
	لكل (n = 0; n < 3; n++)
		منزل_سرو->h_reserved[n] =
			ext2fs_swab32(from_سرو->h_reserved[n]);
}

فراغ ext2fs_مٽا_سٽا_ext_attr_داخلا(بنية ext2_ext_attr_داخلا *منزل_داخلا,
				بنية ext2_ext_attr_داخلا *from_داخلا)
{
	منزل_داخلا->e_قدر_offs  = ext2fs_swab16(from_داخلا->e_قدر_offs);
	منزل_داخلا->e_قدر_inum  = ext2fs_swab32(from_داخلا->e_قدر_inum);
	منزل_داخلا->e_قدر_سائيز  = ext2fs_swab32(from_داخلا->e_قدر_سائيز);
	منزل_داخلا->e_hash	= ext2fs_swab32(from_داخلا->e_hash);
}

فراغ ext2fs_مٽا_سٽا_ext_attr(حرف *منزل, حرف *from, صحيح bufsize, صحيح has_سرو)
{
	بنية ext2_ext_attr_سرو *from_سرو =
		(بنية ext2_ext_attr_سرو *)from;
	بنية ext2_ext_attr_سرو *منزل_سرو =
		(بنية ext2_ext_attr_سرو *)منزل;
	بنية ext2_ext_attr_داخلا *from_داخلا, *منزل_داخلا;
	حرف *from_پڇاڙي = (حرف *)from_سرو + bufsize;

	إذا (منزل_سرو != from_سرو)
		يادگيري_نقل(منزل_سرو, from_سرو, bufsize);

	إذا (has_سرو) {
		ext2fs_مٽا_سٽا_ext_attr_سرو(منزل_سرو, from_سرو);

		from_داخلا = (بنية ext2_ext_attr_داخلا *)(from_سرو+1);
		منزل_داخلا   = (بنية ext2_ext_attr_داخلا *)(منزل_سرو+1);
	} وإلا {
		from_داخلا = (بنية ext2_ext_attr_داخلا *)from_سرو;
		منزل_داخلا   = (بنية ext2_ext_attr_داخلا *)منزل_سرو;
	}

	طالما ((حرف *)from_داخلا < from_پڇاڙي &&
	       (حرف *)EXT2_EXT_ATTR_ايندڙ(from_داخلا) <= from_پڇاڙي &&
	       *(__u32 *)from_داخلا) {
		ext2fs_مٽا_سٽا_ext_attr_داخلا(منزل_داخلا, from_داخلا);
		from_داخلا = EXT2_EXT_ATTR_ايندڙ(from_داخلا);
		منزل_داخلا   = EXT2_EXT_ATTR_ايندڙ(منزل_داخلا);
	}
}

فراغ ext2fs_مٽا_سٽا_inode_ڀريل(ext2_filsys fs, بنية ext2_inode_large *t,
			    بنية ext2_inode_large *f, صحيح hostorder,
			    صحيح bufsize)
{
	بنانشان i, extra_isize, attr_magic;
	صحيح has_extents = 0, has_inline_معلومات = 0, islnk = 0, fast_symlink = 0;
	بنانشان صحيح inode_سائيز;
	__u32 *eaf, *eat;

	/*
	 * Note that t and f may point to the same address. That's why
	 * if (hostorder) condition is executed before swab calls and
	 * if (!hostorder) afterwards.
	 */
	إذا (hostorder) {
		islnk = LINUX_S_ISLNK(f->i_گهڻائي);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(f));
		has_extents = (f->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_معلومات = (f->i_flags & EXT4_INLINE_معلومات_FL) != 0;
	}

	t->i_گهڻائي = ext2fs_swab16(f->i_گهڻائي);
	t->i_uid = ext2fs_swab16(f->i_uid);
	t->i_سائيز = ext2fs_swab32(f->i_سائيز);
	t->i_atime = ext2fs_swab32(f->i_atime);
	t->i_ctime = ext2fs_swab32(f->i_ctime);
	t->i_mtime = ext2fs_swab32(f->i_mtime);
	t->i_dtime = ext2fs_swab32(f->i_dtime);
	t->i_gid = ext2fs_swab16(f->i_gid);
	t->i_links_ڳڻپ = ext2fs_swab16(f->i_links_ڳڻپ);
	t->i_file_acl = ext2fs_swab32(f->i_file_acl);
	t->i_بلاڪون = ext2fs_swab32(f->i_بلاڪون);
	t->i_flags = ext2fs_swab32(f->i_flags);
	t->i_سائيز_مٿاهون = ext2fs_swab32(f->i_سائيز_مٿاهون);

	إذا (!hostorder) {
		islnk = LINUX_S_ISLNK(t->i_گهڻائي);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(t));
		has_extents = (t->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_معلومات = (t->i_flags & EXT4_INLINE_معلومات_FL) != 0;
	}

	/*
	 * Extent data and inline data are swapped on access, not here
	 */
	إذا (!has_extents && !has_inline_معلومات && (!islnk || !fast_symlink)) {
		لكل (i = 0; i < EXT2_N_بلاڪون; i++)
			t->i_بلاڪ[i] = ext2fs_swab32(f->i_بلاڪ[i]);
	} وإلا إذا (t != f) {
		لكل (i = 0; i < EXT2_N_بلاڪون; i++)
			t->i_بلاڪ[i] = f->i_بلاڪ[i];
	}
	t->i_generation = ext2fs_swab32(f->i_generation);
	t->i_faddr = ext2fs_swab32(f->i_faddr);

	بدل (fs->super->s_creator_os) {
	حالة EXT2_OS_LINUX:
		t->osd1.linux1.l_i_نسخو =
			ext2fs_swab32(f->osd1.linux1.l_i_نسخو);
		t->osd2.linux2.l_i_بلاڪون_hi =
			ext2fs_swab16(f->osd2.linux2.l_i_بلاڪون_hi);
		t->osd2.linux2.l_i_file_acl_مٿاهون =
			ext2fs_swab16(f->osd2.linux2.l_i_file_acl_مٿاهون);
		t->osd2.linux2.l_i_uid_مٿاهون =
		  ext2fs_swab16 (f->osd2.linux2.l_i_uid_مٿاهون);
		t->osd2.linux2.l_i_gid_مٿاهون =
		  ext2fs_swab16 (f->osd2.linux2.l_i_gid_مٿاهون);
		t->osd2.linux2.l_i_checksum_lo =
			ext2fs_swab16(f->osd2.linux2.l_i_checksum_lo);
		اكسر;
	حالة EXT2_OS_HURD:
		t->osd1.hurd1.h_i_translator =
		  ext2fs_swab32 (f->osd1.hurd1.h_i_translator);
		t->osd2.hurd2.h_i_frag = f->osd2.hurd2.h_i_frag;
		t->osd2.hurd2.h_i_fsize = f->osd2.hurd2.h_i_fsize;
		t->osd2.hurd2.h_i_گهڻائي_مٿاهون =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_گهڻائي_مٿاهون);
		t->osd2.hurd2.h_i_uid_مٿاهون =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_uid_مٿاهون);
		t->osd2.hurd2.h_i_gid_مٿاهون =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_gid_مٿاهون);
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
	إذا (extra_isize > EXT2_INODE_سائيز(fs->super) -
				حجم(بنية ext2_inode)) {
		/* this is error case: i_extra_size is too large */
		أرجع;
	}
	إذا (extra_isize & 3)
		أرجع;		/* Illegal inode extra_isize */

	inode_سائيز = EXT2_GOOD_OLD_INODE_سائيز + extra_isize;
	إذا (ext2fs_inode_includes(inode_سائيز, i_checksum_hi))
		t->i_checksum_hi = ext2fs_swab16(f->i_checksum_hi);
	إذا (ext2fs_inode_includes(inode_سائيز, i_ctime_extra))
		t->i_ctime_extra = ext2fs_swab32(f->i_ctime_extra);
	إذا (ext2fs_inode_includes(inode_سائيز, i_mtime_extra))
		t->i_mtime_extra = ext2fs_swab32(f->i_mtime_extra);
	إذا (ext2fs_inode_includes(inode_سائيز, i_atime_extra))
		t->i_atime_extra = ext2fs_swab32(f->i_atime_extra);
	إذا (ext2fs_inode_includes(inode_سائيز, i_crtime))
		t->i_crtime = ext2fs_swab32(f->i_crtime);
	إذا (ext2fs_inode_includes(inode_سائيز, i_crtime_extra))
		t->i_crtime_extra = ext2fs_swab32(f->i_crtime_extra);
	إذا (ext2fs_inode_includes(inode_سائيز, i_نسخو_hi))
		t->i_نسخو_hi = ext2fs_swab32(f->i_نسخو_hi);
	إذا (ext2fs_inode_includes(inode_سائيز, i_projid))
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
	ext2fs_مٽا_سٽا_ext_attr((حرف *) (eat + 1), (حرف *) (eaf + 1),
			     bufsize - حجم(بنية ext2_inode) -
			     extra_isize - حجم(__u32), 0);

}

فراغ ext2fs_مٽا_سٽا_inode(ext2_filsys fs, بنية ext2_inode *t,
		       بنية ext2_inode *f, صحيح hostorder)
{
	ext2fs_مٽا_سٽا_inode_ڀريل(fs, (بنية ext2_inode_large *) t,
				(بنية ext2_inode_large *) f, hostorder,
				حجم(بنية ext2_inode));
}

فراغ ext2fs_مٽا_سٽا_mmp(بنية mmp_struct *mmp)
{
	mmp->mmp_magic = ext2fs_swab32(mmp->mmp_magic);
	mmp->mmp_seq = ext2fs_swab32(mmp->mmp_seq);
	mmp->mmp_وقت = ext2fs_swab64(mmp->mmp_وقت);
	mmp->mmp_check_interval = ext2fs_swab16(mmp->mmp_check_interval);
	mmp->mmp_checksum = ext2fs_swab32(mmp->mmp_checksum);
}

errcode_t ext2fs_dirent_swab_اندريون(ext2_filsys fs, حرف *buf, صحيح flags)
{
	أرجع ext2fs_dirent_swab_in2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, حرف *buf,
				 size_t سائيز, صحيح flags)
{
	errcode_t	retval;
	حرف		*p, *پڇاڙي;
	بنية ext2_dir_داخلا *dirent;
	بنانشان صحيح	name_len, rec_len, کاٻو;

	p = (حرف *) buf;
	پڇاڙي = (حرف *) buf + سائيز;
	کاٻو = سائيز;
	طالما (p < پڇاڙي-8) {
		dirent = (بنية ext2_dir_داخلا *) p;
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
			إذا (!(fs->flags & EXT2_FLAG_IGNORE_مٽا_سٽا_DIRENT))
				أرجع EXT2_ET_DIR_CORRUPTED;
		} وإلا إذا (((name_len & 0xFF) + 8) > rec_len)
			إذا (!(fs->flags & EXT2_FLAG_IGNORE_مٽا_سٽا_DIRENT))
				أرجع EXT2_ET_DIR_CORRUPTED;
		إذا (rec_len > کاٻو)
			إذا (!(fs->flags & EXT2_FLAG_IGNORE_مٽا_سٽا_DIRENT))
				أرجع EXT2_ET_DIR_CORRUPTED;
		کاٻو -= rec_len;
		p += rec_len;
	}

	أرجع 0;
}

errcode_t ext2fs_dirent_swab_ٻاهريون(ext2_filsys fs, حرف *buf, صحيح flags)
{
	أرجع ext2fs_dirent_swab_out2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, حرف *buf,
				  size_t سائيز, صحيح flags)
{
	errcode_t	retval;
	حرف		*p, *پڇاڙي;
	بنانشان صحيح	rec_len;
	بنية ext2_dir_داخلا *dirent;

	p = buf;
	پڇاڙي = buf + سائيز;
	طالما (p < پڇاڙي) {
		dirent = (بنية ext2_dir_داخلا *) p;
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
		إذا (rec_len > سائيز)
			أرجع EXT2_ET_DIR_CORRUPTED;
		سائيز -= rec_len;

		إذا (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
	}

	أرجع 0;
}
