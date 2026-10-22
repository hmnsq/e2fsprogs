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

#تعریف_کړه sb_struct ext2_super_بلاک
#تعریف_کړه sb_struct_name "ext2_super_block"

بنية sb_struct sb;

#که_نه_تعریف_شوی offsetof
#تعریف_کړه offsetof(type, member)  __builtin_offsetof (type, member)
#د_که_پای

#تعریف_کړه check_ډګر(x, s) cur_دننه_واټن = do_ډګر(#x, s, حجم(sb.x),	       \
						offsetof(بنية sb_struct, x), \
						cur_دننه_واټن)

ساكن صحيح do_ډګر(ثابت حرف *ډګر, بینښه اندازه, بینښه cur_اندازه,
		    بینښه دننه_واټن, بینښه cur_دننه_واټن)
{
	إذا (اندازه != cur_اندازه) {
		چاپ_کړه("error: %s size %u should be %u\n",
		       ډګر, cur_اندازه, اندازه);
		وتل(1);
	}
	إذا (دننه_واټن != cur_دننه_واټن) {
		چاپ_کړه("error: %s offset %u should be %u\n",
		       ډګر, cur_دننه_واټن, دننه_واټن);
		وتل(1);
	}
	چاپ_کړه("%8d %-30s %3u\n", دننه_واټن, ډګر, اندازه);
	أرجع دننه_واټن + اندازه;
}

صحيح اصلي(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_دننه_واټن = 0;

	چاپ_کړه("%8s %-30s %3s\n", "offset", "field", "size");
	check_ډګر(s_inodes_شمېر, 4);
	check_ډګر(s_بلاکونه_شمېر, 4);
	check_ډګر(s_r_بلاکونه_شمېر, 4);
	check_ډګر(s_free_بلاکونه_شمېر, 4);
	check_ډګر(s_free_inodes_شمېر, 4);
	check_ډګر(s_لومړی_معلومات_بلاک, 4);
	check_ډګر(s_log_بلاک_اندازه, 4);
	check_ډګر(s_log_cluster_اندازه, 4);
	check_ډګر(s_بلاکونه_فی_ډله, 4);
	check_ډګر(s_clusters_فی_ډله, 4);
	check_ډګر(s_inodes_فی_ډله, 4);
	check_ډګر(s_mtime, 4);
	check_ډګر(s_wtime, 4);
	check_ډګر(s_mnt_شمېر, 2);
	check_ډګر(s_max_mnt_شمېر, 2);
	check_ډګر(s_magic, 2);
	check_ډګر(s_state, 2);
	check_ډګر(s_errors, 2);
	check_ډګر(s_minor_rev_level, 2);
	check_ډګر(s_lastcheck, 4);
	check_ډګر(s_checkinterval, 4);
	check_ډګر(s_creator_os, 4);
	check_ډګر(s_rev_level, 4);
	check_ډګر(s_def_resuid, 2);
	check_ډګر(s_def_resgid, 2);
	check_ډګر(s_لومړی_ino, 4);
	check_ډګر(s_inode_اندازه, 2);
	check_ډګر(s_بلاک_ډله_nr, 2);
	check_ډګر(s_feature_compat, 4);
	check_ډګر(s_feature_incompat, 4);
	check_ډګر(s_feature_ro_compat, 4);
	check_ډګر(s_uuid, 16);
	check_ډګر(s_volume_name, 16);
	check_ډګر(s_وروستی_mounted, 64);
	check_ډګر(s_algorithm_usage_bitmap, 4);
	check_ډګر(s_prealloc_بلاکونه, 1);
	check_ډګر(s_prealloc_dir_بلاکونه, 1);
	check_ډګر(s_reserved_gdt_بلاکونه, 2);
	check_ډګر(s_journal_uuid, 16);
	check_ډګر(s_journal_inum, 4);
	check_ډګر(s_journal_dev, 4);
	check_ډګر(s_وروستی_orphan, 4);
	check_ډګر(s_hash_تخم, 4 * 4);
	check_ډګر(s_def_hash_نسخه, 1);
	check_ډګر(s_jnl_backup_type, 1);
	check_ډګر(s_desc_اندازه, 2);
	check_ډګر(s_default_mount_opts, 4);
	check_ډګر(s_لومړی_meta_bg, 4);
	check_ډګر(s_mkfs_وخت, 4);
	check_ډګر(s_jnl_بلاکونه, 17 * 4);
	check_ډګر(s_بلاکونه_شمېر_hi, 4);
	check_ډګر(s_r_بلاکونه_شمېر_hi, 4);
	check_ډګر(s_free_بلاکونه_hi, 4);
	check_ډګر(s_min_extra_isize, 2);
	check_ډګر(s_want_extra_isize, 2);
	check_ډګر(s_flags, 4);
	check_ډګر(s_raid_stride, 2);
	check_ډګر(s_mmp_update_interval, 2);
	check_ډګر(s_mmp_بلاک, 8);
	check_ډګر(s_raid_stripe_پلنوالی, 4);
	check_ډګر(s_log_groups_فی_flex, 1);
	check_ډګر(s_checksum_type, 1);
	check_ډګر(s_encryption_level, 1);
	check_ډګر(s_reserved_pad, 1);
	check_ډګر(s_kbytes_written, 8);
	check_ډګر(s_snapshot_inum, 4);
	check_ډګر(s_snapshot_id, 4);
	check_ډګر(s_snapshot_r_بلاکونه_شمېر, 8);
	check_ډګر(s_snapshot_list, 4);
	check_ډګر(s_error_شمېر, 4);
	check_ډګر(s_لومړی_error_وخت, 4);
	check_ډګر(s_لومړی_error_ino, 4);
	check_ډګر(s_لومړی_error_بلاک, 8);
	check_ډګر(s_لومړی_error_func, 32);
	check_ډګر(s_لومړی_error_کرښه, 4);
	check_ډګر(s_وروستی_error_وخت, 4);
	check_ډګر(s_وروستی_error_ino, 4);
	check_ډګر(s_وروستی_error_کرښه, 4);
	check_ډګر(s_وروستی_error_بلاک, 8);
	check_ډګر(s_وروستی_error_func, 32);
	check_ډګر(s_mount_opts, 64);
	check_ډګر(s_usr_quota_inum, 4);
	check_ډګر(s_grp_quota_inum, 4);
	check_ډګر(s_overhead_clusters, 4);
	check_ډګر(s_backup_bgs, 8);
	check_ډګر(s_encrypt_algos, 4);
	check_ډګر(s_encrypt_pw_salt, 16);
	check_ډګر(s_lpf_ino, 4);
	check_ډګر(s_prj_quota_inum, 4);
	check_ډګر(s_checksum_تخم, 4);
	check_ډګر(s_wtime_hi, 1);
	check_ډګر(s_mtime_hi, 1);
	check_ډګر(s_mkfs_وخت_hi, 1);
	check_ډګر(s_lastcheck_hi, 1);
	check_ډګر(s_لومړی_error_وخت_hi, 1);
	check_ډګر(s_وروستی_error_وخت_hi, 1);
	check_ډګر(s_لومړی_error_errcode, 1);
	check_ډګر(s_وروستی_error_errcode, 1);
	check_ډګر(s_encoding, 2);
	check_ډګر(s_encoding_flags, 2);
	check_ډګر(s_orphan_file_inum, 4);
	check_ډګر(s_reserved, 94 * 4);
	check_ډګر(s_checksum, 4);
	do_ډګر("Superblock end", 0, 0, cur_دننه_واټن, 1024);
#د_که_پای
	أرجع 0;
}
