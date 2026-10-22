/*
 * This testing program makes sure superblock size is 1024 bytes long
 *
 * Copyright (C) 2007 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <unistd.h>
#ضمن <stdlib.h>

#ضمن "ext2_fs.h"

#تعریف_کرو sb_struct ext2_super_بلاک
#تعریف_کرو sb_struct_name "ext2_super_block"

بنية sb_struct sb;

#جے_بنا_تعریف offsetof
#تعریف_کرو offsetof(type, member)  __builtin_offsetof (type, member)
#جے_انت

#تعریف_کرو check_میدان(x, s) cur_وتھ = do_میدان(#x, s, حجم(sb.x),	       \
						offsetof(بنية sb_struct, x), \
						cur_وتھ)

ساكن صحيح do_میدان(ثابت حرف *میدان, بنانشانی ناپ, بنانشانی cur_ناپ,
		    بنانشانی وتھ, بنانشانی cur_وتھ)
{
	إذا (ناپ != cur_ناپ) {
		لکھو("error: %s size %u should be %u\n",
		       میدان, cur_ناپ, ناپ);
		باہر_نکلو(1);
	}
	إذا (وتھ != cur_وتھ) {
		لکھو("error: %s offset %u should be %u\n",
		       میدان, cur_وتھ, وتھ);
		باہر_نکلو(1);
	}
	لکھو("%8d %-30s %3u\n", وتھ, میدان, ناپ);
	أرجع وتھ + ناپ;
}

صحيح مکھ(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_وتھ = 0;

	لکھو("%8s %-30s %3s\n", "offset", "field", "size");
	check_میدان(s_inodes_گنتی, 4);
	check_میدان(s_بلاکاں_گنتی, 4);
	check_میدان(s_r_بلاکاں_گنتی, 4);
	check_میدان(s_free_بلاکاں_گنتی, 4);
	check_میدان(s_free_inodes_گنتی, 4);
	check_میدان(s_پہلا_انکڑے_بلاک, 4);
	check_میدان(s_log_بلاک_ناپ, 4);
	check_میدان(s_log_cluster_ناپ, 4);
	check_میدان(s_بلاکاں_فی_گروہ, 4);
	check_میدان(s_clusters_فی_گروہ, 4);
	check_میدان(s_inodes_فی_گروہ, 4);
	check_میدان(s_mtime, 4);
	check_میدان(s_wtime, 4);
	check_میدان(s_mnt_گنتی, 2);
	check_میدان(s_max_mnt_گنتی, 2);
	check_میدان(s_magic, 2);
	check_میدان(s_state, 2);
	check_میدان(s_errors, 2);
	check_میدان(s_minor_rev_level, 2);
	check_میدان(s_lastcheck, 4);
	check_میدان(s_checkinterval, 4);
	check_میدان(s_creator_os, 4);
	check_میدان(s_rev_level, 4);
	check_میدان(s_def_resuid, 2);
	check_میدان(s_def_resgid, 2);
	check_میدان(s_پہلا_ino, 4);
	check_میدان(s_inode_ناپ, 2);
	check_میدان(s_بلاک_گروہ_nr, 2);
	check_میدان(s_feature_compat, 4);
	check_میدان(s_feature_incompat, 4);
	check_میدان(s_feature_ro_compat, 4);
	check_میدان(s_uuid, 16);
	check_میدان(s_volume_name, 16);
	check_میدان(s_آخری_mounted, 64);
	check_میدان(s_algorithm_usage_bitmap, 4);
	check_میدان(s_prealloc_بلاکاں, 1);
	check_میدان(s_prealloc_dir_بلاکاں, 1);
	check_میدان(s_reserved_gdt_بلاکاں, 2);
	check_میدان(s_journal_uuid, 16);
	check_میدان(s_journal_inum, 4);
	check_میدان(s_journal_dev, 4);
	check_میدان(s_آخری_orphan, 4);
	check_میدان(s_hash_بیج, 4 * 4);
	check_میدان(s_def_hash_نسخہ, 1);
	check_میدان(s_jnl_backup_type, 1);
	check_میدان(s_desc_ناپ, 2);
	check_میدان(s_default_mount_opts, 4);
	check_میدان(s_پہلا_meta_bg, 4);
	check_میدان(s_mkfs_ویلا, 4);
	check_میدان(s_jnl_بلاکاں, 17 * 4);
	check_میدان(s_بلاکاں_گنتی_hi, 4);
	check_میدان(s_r_بلاکاں_گنتی_hi, 4);
	check_میدان(s_free_بلاکاں_hi, 4);
	check_میدان(s_min_extra_isize, 2);
	check_میدان(s_want_extra_isize, 2);
	check_میدان(s_flags, 4);
	check_میدان(s_raid_stride, 2);
	check_میدان(s_mmp_update_interval, 2);
	check_میدان(s_mmp_بلاک, 8);
	check_میدان(s_raid_stripe_چوڑائی, 4);
	check_میدان(s_log_groups_فی_flex, 1);
	check_میدان(s_checksum_type, 1);
	check_میدان(s_encryption_level, 1);
	check_میدان(s_reserved_pad, 1);
	check_میدان(s_kbytes_written, 8);
	check_میدان(s_snapshot_inum, 4);
	check_میدان(s_snapshot_id, 4);
	check_میدان(s_snapshot_r_بلاکاں_گنتی, 8);
	check_میدان(s_snapshot_list, 4);
	check_میدان(s_error_گنتی, 4);
	check_میدان(s_پہلا_error_ویلا, 4);
	check_میدان(s_پہلا_error_ino, 4);
	check_میدان(s_پہلا_error_بلاک, 8);
	check_میدان(s_پہلا_error_func, 32);
	check_میدان(s_پہلا_error_سطر, 4);
	check_میدان(s_آخری_error_ویلا, 4);
	check_میدان(s_آخری_error_ino, 4);
	check_میدان(s_آخری_error_سطر, 4);
	check_میدان(s_آخری_error_بلاک, 8);
	check_میدان(s_آخری_error_func, 32);
	check_میدان(s_mount_opts, 64);
	check_میدان(s_usr_quota_inum, 4);
	check_میدان(s_grp_quota_inum, 4);
	check_میدان(s_overhead_clusters, 4);
	check_میدان(s_backup_bgs, 8);
	check_میدان(s_encrypt_algos, 4);
	check_میدان(s_encrypt_pw_salt, 16);
	check_میدان(s_lpf_ino, 4);
	check_میدان(s_prj_quota_inum, 4);
	check_میدان(s_checksum_بیج, 4);
	check_میدان(s_wtime_hi, 1);
	check_میدان(s_mtime_hi, 1);
	check_میدان(s_mkfs_ویلا_hi, 1);
	check_میدان(s_lastcheck_hi, 1);
	check_میدان(s_پہلا_error_ویلا_hi, 1);
	check_میدان(s_آخری_error_ویلا_hi, 1);
	check_میدان(s_پہلا_error_errcode, 1);
	check_میدان(s_آخری_error_errcode, 1);
	check_میدان(s_encoding, 2);
	check_میدان(s_encoding_flags, 2);
	check_میدان(s_orphan_file_inum, 4);
	check_میدان(s_reserved, 94 * 4);
	check_میدان(s_checksum, 4);
	do_میدان("Superblock end", 0, 0, cur_وتھ, 1024);
#جے_انت
	أرجع 0;
}
