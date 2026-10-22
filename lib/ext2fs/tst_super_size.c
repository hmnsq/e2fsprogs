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

#وصف_ڪريو sb_struct ext2_super_بلاڪ
#وصف_ڪريو sb_struct_name "ext2_super_block"

بنية sb_struct sb;

#جيڪڏهن_وصف_نه_ٿيل offsetof
#وصف_ڪريو offsetof(type, member)  __builtin_offsetof (type, member)
#جيڪڏهن_پورو

#وصف_ڪريو check_ميدان(x, s) cur_وٿي = do_ميدان(#x, s, حجم(sb.x),	       \
						offsetof(بنية sb_struct, x), \
						cur_وٿي)

ساكن صحيح do_ميدان(ثابت حرف *ميدان, بنانشان سائيز, بنانشان cur_سائيز,
		    بنانشان وٿي, بنانشان cur_وٿي)
{
	إذا (سائيز != cur_سائيز) {
		لکو("error: %s size %u should be %u\n",
		       ميدان, cur_سائيز, سائيز);
		ٻاهر_نڪرو(1);
	}
	إذا (وٿي != cur_وٿي) {
		لکو("error: %s offset %u should be %u\n",
		       ميدان, cur_وٿي, وٿي);
		ٻاهر_نڪرو(1);
	}
	لکو("%8d %-30s %3u\n", وٿي, ميدان, سائيز);
	أرجع وٿي + سائيز;
}

صحيح مکيه(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_وٿي = 0;

	لکو("%8s %-30s %3s\n", "offset", "field", "size");
	check_ميدان(s_inodes_ڳڻپ, 4);
	check_ميدان(s_بلاڪون_ڳڻپ, 4);
	check_ميدان(s_r_بلاڪون_ڳڻپ, 4);
	check_ميدان(s_free_بلاڪون_ڳڻپ, 4);
	check_ميدان(s_free_inodes_ڳڻپ, 4);
	check_ميدان(s_پهريون_معلومات_بلاڪ, 4);
	check_ميدان(s_log_بلاڪ_سائيز, 4);
	check_ميدان(s_log_cluster_سائيز, 4);
	check_ميدان(s_بلاڪون_في_گروهه, 4);
	check_ميدان(s_clusters_في_گروهه, 4);
	check_ميدان(s_inodes_في_گروهه, 4);
	check_ميدان(s_mtime, 4);
	check_ميدان(s_wtime, 4);
	check_ميدان(s_mnt_ڳڻپ, 2);
	check_ميدان(s_max_mnt_ڳڻپ, 2);
	check_ميدان(s_magic, 2);
	check_ميدان(s_state, 2);
	check_ميدان(s_errors, 2);
	check_ميدان(s_minor_rev_level, 2);
	check_ميدان(s_lastcheck, 4);
	check_ميدان(s_checkinterval, 4);
	check_ميدان(s_creator_os, 4);
	check_ميدان(s_rev_level, 4);
	check_ميدان(s_def_resuid, 2);
	check_ميدان(s_def_resgid, 2);
	check_ميدان(s_پهريون_ino, 4);
	check_ميدان(s_inode_سائيز, 2);
	check_ميدان(s_بلاڪ_گروهه_nr, 2);
	check_ميدان(s_feature_compat, 4);
	check_ميدان(s_feature_incompat, 4);
	check_ميدان(s_feature_ro_compat, 4);
	check_ميدان(s_uuid, 16);
	check_ميدان(s_volume_name, 16);
	check_ميدان(s_آخري_mounted, 64);
	check_ميدان(s_algorithm_usage_bitmap, 4);
	check_ميدان(s_prealloc_بلاڪون, 1);
	check_ميدان(s_prealloc_dir_بلاڪون, 1);
	check_ميدان(s_reserved_gdt_بلاڪون, 2);
	check_ميدان(s_journal_uuid, 16);
	check_ميدان(s_journal_inum, 4);
	check_ميدان(s_journal_dev, 4);
	check_ميدان(s_آخري_orphan, 4);
	check_ميدان(s_hash_ٻج, 4 * 4);
	check_ميدان(s_def_hash_نسخو, 1);
	check_ميدان(s_jnl_backup_type, 1);
	check_ميدان(s_desc_سائيز, 2);
	check_ميدان(s_default_mount_opts, 4);
	check_ميدان(s_پهريون_meta_bg, 4);
	check_ميدان(s_mkfs_وقت, 4);
	check_ميدان(s_jnl_بلاڪون, 17 * 4);
	check_ميدان(s_بلاڪون_ڳڻپ_hi, 4);
	check_ميدان(s_r_بلاڪون_ڳڻپ_hi, 4);
	check_ميدان(s_free_بلاڪون_hi, 4);
	check_ميدان(s_min_extra_isize, 2);
	check_ميدان(s_want_extra_isize, 2);
	check_ميدان(s_flags, 4);
	check_ميدان(s_raid_stride, 2);
	check_ميدان(s_mmp_update_interval, 2);
	check_ميدان(s_mmp_بلاڪ, 8);
	check_ميدان(s_raid_stripe_ويڪر, 4);
	check_ميدان(s_log_groups_في_flex, 1);
	check_ميدان(s_checksum_type, 1);
	check_ميدان(s_encryption_level, 1);
	check_ميدان(s_reserved_pad, 1);
	check_ميدان(s_kbytes_written, 8);
	check_ميدان(s_snapshot_inum, 4);
	check_ميدان(s_snapshot_id, 4);
	check_ميدان(s_snapshot_r_بلاڪون_ڳڻپ, 8);
	check_ميدان(s_snapshot_list, 4);
	check_ميدان(s_error_ڳڻپ, 4);
	check_ميدان(s_پهريون_error_وقت, 4);
	check_ميدان(s_پهريون_error_ino, 4);
	check_ميدان(s_پهريون_error_بلاڪ, 8);
	check_ميدان(s_پهريون_error_func, 32);
	check_ميدان(s_پهريون_error_سٽ, 4);
	check_ميدان(s_آخري_error_وقت, 4);
	check_ميدان(s_آخري_error_ino, 4);
	check_ميدان(s_آخري_error_سٽ, 4);
	check_ميدان(s_آخري_error_بلاڪ, 8);
	check_ميدان(s_آخري_error_func, 32);
	check_ميدان(s_mount_opts, 64);
	check_ميدان(s_usr_quota_inum, 4);
	check_ميدان(s_grp_quota_inum, 4);
	check_ميدان(s_overhead_clusters, 4);
	check_ميدان(s_backup_bgs, 8);
	check_ميدان(s_encrypt_algos, 4);
	check_ميدان(s_encrypt_pw_salt, 16);
	check_ميدان(s_lpf_ino, 4);
	check_ميدان(s_prj_quota_inum, 4);
	check_ميدان(s_checksum_ٻج, 4);
	check_ميدان(s_wtime_hi, 1);
	check_ميدان(s_mtime_hi, 1);
	check_ميدان(s_mkfs_وقت_hi, 1);
	check_ميدان(s_lastcheck_hi, 1);
	check_ميدان(s_پهريون_error_وقت_hi, 1);
	check_ميدان(s_آخري_error_وقت_hi, 1);
	check_ميدان(s_پهريون_error_errcode, 1);
	check_ميدان(s_آخري_error_errcode, 1);
	check_ميدان(s_encoding, 2);
	check_ميدان(s_encoding_flags, 2);
	check_ميدان(s_orphan_file_inum, 4);
	check_ميدان(s_reserved, 94 * 4);
	check_ميدان(s_checksum, 4);
	do_ميدان("Superblock end", 0, 0, cur_وٿي, 1024);
#جيڪڏهن_پورو
	أرجع 0;
}
