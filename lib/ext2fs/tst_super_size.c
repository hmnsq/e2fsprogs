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

#تعریف_کن sb_struct ext2_super_بلوک
#تعریف_کن sb_struct_name "ext2_super_block"

بنية sb_struct sb;

#اگر_تعریف_نشده offsetof
#تعریف_کن offsetof(type, member)  __builtin_offsetof (type, member)
#پایان_اگر

#تعریف_کن check_فیلد(x, s) cur_جابه_جایی = do_فیلد(#x, s, حجم(sb.x),	       \
						offsetof(بنية sb_struct, x), \
						cur_جابه_جایی)

ساكن صحيح do_فیلد(ثابت حرف *فیلد, بیعلامت اندازه, بیعلامت cur_اندازه,
		    بیعلامت جابه_جایی, بیعلامت cur_جابه_جایی)
{
	إذا (اندازه != cur_اندازه) {
		چاپ_کن("error: %s size %u should be %u\n",
		       فیلد, cur_اندازه, اندازه);
		خروج(1);
	}
	إذا (جابه_جایی != cur_جابه_جایی) {
		چاپ_کن("error: %s offset %u should be %u\n",
		       فیلد, cur_جابه_جایی, جابه_جایی);
		خروج(1);
	}
	چاپ_کن("%8d %-30s %3u\n", جابه_جایی, فیلد, اندازه);
	أرجع جابه_جایی + اندازه;
}

صحيح اصلی(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_جابه_جایی = 0;

	چاپ_کن("%8s %-30s %3s\n", "offset", "field", "size");
	check_فیلد(s_inodes_شمار, 4);
	check_فیلد(s_بلوکها_شمار, 4);
	check_فیلد(s_r_بلوکها_شمار, 4);
	check_فیلد(s_free_بلوکها_شمار, 4);
	check_فیلد(s_free_inodes_شمار, 4);
	check_فیلد(s_اولی_داده_بلوک, 4);
	check_فیلد(s_log_بلوک_اندازه, 4);
	check_فیلد(s_log_cluster_اندازه, 4);
	check_فیلد(s_بلوکها_در_هر_گروه, 4);
	check_فیلد(s_clusters_در_هر_گروه, 4);
	check_فیلد(s_inodes_در_هر_گروه, 4);
	check_فیلد(s_mtime, 4);
	check_فیلد(s_wtime, 4);
	check_فیلد(s_mnt_شمار, 2);
	check_فیلد(s_max_mnt_شمار, 2);
	check_فیلد(s_magic, 2);
	check_فیلد(s_state, 2);
	check_فیلد(s_errors, 2);
	check_فیلد(s_minor_rev_level, 2);
	check_فیلد(s_lastcheck, 4);
	check_فیلد(s_checkinterval, 4);
	check_فیلد(s_creator_os, 4);
	check_فیلد(s_rev_level, 4);
	check_فیلد(s_def_resuid, 2);
	check_فیلد(s_def_resgid, 2);
	check_فیلد(s_اولی_ino, 4);
	check_فیلد(s_inode_اندازه, 2);
	check_فیلد(s_بلوک_گروه_nr, 2);
	check_فیلد(s_feature_compat, 4);
	check_فیلد(s_feature_incompat, 4);
	check_فیلد(s_feature_ro_compat, 4);
	check_فیلد(s_uuid, 16);
	check_فیلد(s_volume_name, 16);
	check_فیلد(s_آخرین_mounted, 64);
	check_فیلد(s_algorithm_usage_bitmap, 4);
	check_فیلد(s_prealloc_بلوکها, 1);
	check_فیلد(s_prealloc_dir_بلوکها, 1);
	check_فیلد(s_reserved_gdt_بلوکها, 2);
	check_فیلد(s_journal_uuid, 16);
	check_فیلد(s_journal_inum, 4);
	check_فیلد(s_journal_dev, 4);
	check_فیلد(s_آخرین_orphan, 4);
	check_فیلد(s_hash_دانه, 4 * 4);
	check_فیلد(s_def_hash_نسخه, 1);
	check_فیلد(s_jnl_backup_type, 1);
	check_فیلد(s_desc_اندازه, 2);
	check_فیلد(s_default_mount_opts, 4);
	check_فیلد(s_اولی_meta_bg, 4);
	check_فیلد(s_mkfs_زمان, 4);
	check_فیلد(s_jnl_بلوکها, 17 * 4);
	check_فیلد(s_بلوکها_شمار_hi, 4);
	check_فیلد(s_r_بلوکها_شمار_hi, 4);
	check_فیلد(s_free_بلوکها_hi, 4);
	check_فیلد(s_min_extra_isize, 2);
	check_فیلد(s_want_extra_isize, 2);
	check_فیلد(s_flags, 4);
	check_فیلد(s_raid_stride, 2);
	check_فیلد(s_mmp_update_interval, 2);
	check_فیلد(s_mmp_بلوک, 8);
	check_فیلد(s_raid_stripe_عرض, 4);
	check_فیلد(s_log_groups_در_هر_flex, 1);
	check_فیلد(s_checksum_type, 1);
	check_فیلد(s_encryption_level, 1);
	check_فیلد(s_reserved_pad, 1);
	check_فیلد(s_kbytes_written, 8);
	check_فیلد(s_snapshot_inum, 4);
	check_فیلد(s_snapshot_id, 4);
	check_فیلد(s_snapshot_r_بلوکها_شمار, 8);
	check_فیلد(s_snapshot_list, 4);
	check_فیلد(s_error_شمار, 4);
	check_فیلد(s_اولی_error_زمان, 4);
	check_فیلد(s_اولی_error_ino, 4);
	check_فیلد(s_اولی_error_بلوک, 8);
	check_فیلد(s_اولی_error_func, 32);
	check_فیلد(s_اولی_error_سطر, 4);
	check_فیلد(s_آخرین_error_زمان, 4);
	check_فیلد(s_آخرین_error_ino, 4);
	check_فیلد(s_آخرین_error_سطر, 4);
	check_فیلد(s_آخرین_error_بلوک, 8);
	check_فیلد(s_آخرین_error_func, 32);
	check_فیلد(s_mount_opts, 64);
	check_فیلد(s_usr_quota_inum, 4);
	check_فیلد(s_grp_quota_inum, 4);
	check_فیلد(s_overhead_clusters, 4);
	check_فیلد(s_backup_bgs, 8);
	check_فیلد(s_encrypt_algos, 4);
	check_فیلد(s_encrypt_pw_salt, 16);
	check_فیلد(s_lpf_ino, 4);
	check_فیلد(s_prj_quota_inum, 4);
	check_فیلد(s_checksum_دانه, 4);
	check_فیلد(s_wtime_hi, 1);
	check_فیلد(s_mtime_hi, 1);
	check_فیلد(s_mkfs_زمان_hi, 1);
	check_فیلد(s_lastcheck_hi, 1);
	check_فیلد(s_اولی_error_زمان_hi, 1);
	check_فیلد(s_آخرین_error_زمان_hi, 1);
	check_فیلد(s_اولی_error_errcode, 1);
	check_فیلد(s_آخرین_error_errcode, 1);
	check_فیلد(s_encoding, 2);
	check_فیلد(s_encoding_flags, 2);
	check_فیلد(s_orphan_file_inum, 4);
	check_فیلد(s_reserved, 94 * 4);
	check_فیلد(s_checksum, 4);
	do_فیلد("Superblock end", 0, 0, cur_جابه_جایی, 1024);
#پایان_اگر
	أرجع 0;
}
