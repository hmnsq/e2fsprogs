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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <stdlib.h>

#అన్తర్భావ "ext2_fs.h"

#నిర్వచించు sb_struct ext2_super_ఖండం
#నిర్వచించు sb_struct_name "ext2_super_block"

నిర్మాణం sb_struct sb;

#నిర్వచితం_కాకపోతే offsetof
#నిర్వచించు offsetof(type, member)  __builtin_offsetof (type, member)
#అయితే_ముగింపు

#నిర్వచించు check_క్షేత్రం(x, s) cur_స్థానభ్రంశం = do_క్షేత్రం(#x, s, పరిమాణం(sb.x),	       \
						offsetof(నిర్మాణం sb_struct, x), \
						cur_స్థానభ్రంశం)

స్థిర పూర్ణాంకం do_క్షేత్రం(స్థిరరాశి అక్షరం *క్షేత్రం, గుర్తులేని కొలత, గుర్తులేని cur_కొలత,
		    గుర్తులేని స్థానభ్రంశం, గుర్తులేని cur_స్థానభ్రంశం)
{
	ఒకవేళ (కొలత != cur_కొలత) {
		రాయి("error: %s size %u should be %u\n",
		       క్షేత్రం, cur_కొలత, కొలత);
		నిష్క్రమించు(1);
	}
	ఒకవేళ (స్థానభ్రంశం != cur_స్థానభ్రంశం) {
		రాయి("error: %s offset %u should be %u\n",
		       క్షేత్రం, cur_స్థానభ్రంశం, స్థానభ్రంశం);
		నిష్క్రమించు(1);
	}
	రాయి("%8d %-30s %3u\n", స్థానభ్రంశం, క్షేత్రం, కొలత);
	తిరిగి స్థానభ్రంశం + కొలత;
}

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
#ఒకవేళ (__GNUC__ >= 4)
	పూర్ణాంకం cur_స్థానభ్రంశం = 0;

	రాయి("%8s %-30s %3s\n", "offset", "field", "size");
	check_క్షేత్రం(s_inodes_లెక్క, 4);
	check_క్షేత్రం(s_ఖండాలు_లెక్క, 4);
	check_క్షేత్రం(s_r_ఖండాలు_లెక్క, 4);
	check_క్షేత్రం(s_free_ఖండాలు_లెక్క, 4);
	check_క్షేత్రం(s_free_inodes_లెక్క, 4);
	check_క్షేత్రం(s_మొదటి_దత్తాంశం_ఖండం, 4);
	check_క్షేత్రం(s_log_ఖండం_కొలత, 4);
	check_క్షేత్రం(s_log_cluster_కొలత, 4);
	check_క్షేత్రం(s_ఖండాలు_చొప్పున_సమూహం, 4);
	check_క్షేత్రం(s_clusters_చొప్పున_సమూహం, 4);
	check_క్షేత్రం(s_inodes_చొప్పున_సమూహం, 4);
	check_క్షేత్రం(s_mtime, 4);
	check_క్షేత్రం(s_wtime, 4);
	check_క్షేత్రం(s_mnt_లెక్క, 2);
	check_క్షేత్రం(s_max_mnt_లెక్క, 2);
	check_క్షేత్రం(s_magic, 2);
	check_క్షేత్రం(s_state, 2);
	check_క్షేత్రం(s_errors, 2);
	check_క్షేత్రం(s_minor_rev_level, 2);
	check_క్షేత్రం(s_lastcheck, 4);
	check_క్షేత్రం(s_checkinterval, 4);
	check_క్షేత్రం(s_creator_os, 4);
	check_క్షేత్రం(s_rev_level, 4);
	check_క్షేత్రం(s_def_resuid, 2);
	check_క్షేత్రం(s_def_resgid, 2);
	check_క్షేత్రం(s_మొదటి_ino, 4);
	check_క్షేత్రం(s_inode_కొలత, 2);
	check_క్షేత్రం(s_ఖండం_సమూహం_nr, 2);
	check_క్షేత్రం(s_feature_compat, 4);
	check_క్షేత్రం(s_feature_incompat, 4);
	check_క్షేత్రం(s_feature_ro_compat, 4);
	check_క్షేత్రం(s_uuid, 16);
	check_క్షేత్రం(s_volume_name, 16);
	check_క్షేత్రం(s_చివరి_mounted, 64);
	check_క్షేత్రం(s_algorithm_usage_bitmap, 4);
	check_క్షేత్రం(s_prealloc_ఖండాలు, 1);
	check_క్షేత్రం(s_prealloc_dir_ఖండాలు, 1);
	check_క్షేత్రం(s_reserved_gdt_ఖండాలు, 2);
	check_క్షేత్రం(s_journal_uuid, 16);
	check_క్షేత్రం(s_journal_inum, 4);
	check_క్షేత్రం(s_journal_dev, 4);
	check_క్షేత్రం(s_చివరి_orphan, 4);
	check_క్షేత్రం(s_hash_విత్తనం, 4 * 4);
	check_క్షేత్రం(s_def_hash_సంస్కరణ, 1);
	check_క్షేత్రం(s_jnl_backup_type, 1);
	check_క్షేత్రం(s_desc_కొలత, 2);
	check_క్షేత్రం(s_default_mount_opts, 4);
	check_క్షేత్రం(s_మొదటి_meta_bg, 4);
	check_క్షేత్రం(s_mkfs_సమయం, 4);
	check_క్షేత్రం(s_jnl_ఖండాలు, 17 * 4);
	check_క్షేత్రం(s_ఖండాలు_లెక్క_hi, 4);
	check_క్షేత్రం(s_r_ఖండాలు_లెక్క_hi, 4);
	check_క్షేత్రం(s_free_ఖండాలు_hi, 4);
	check_క్షేత్రం(s_min_extra_isize, 2);
	check_క్షేత్రం(s_want_extra_isize, 2);
	check_క్షేత్రం(s_flags, 4);
	check_క్షేత్రం(s_raid_stride, 2);
	check_క్షేత్రం(s_mmp_update_interval, 2);
	check_క్షేత్రం(s_mmp_ఖండం, 8);
	check_క్షేత్రం(s_raid_stripe_వెడల్పు, 4);
	check_క్షేత్రం(s_log_groups_చొప్పున_flex, 1);
	check_క్షేత్రం(s_checksum_type, 1);
	check_క్షేత్రం(s_encryption_level, 1);
	check_క్షేత్రం(s_reserved_pad, 1);
	check_క్షేత్రం(s_kbytes_written, 8);
	check_క్షేత్రం(s_snapshot_inum, 4);
	check_క్షేత్రం(s_snapshot_id, 4);
	check_క్షేత్రం(s_snapshot_r_ఖండాలు_లెక్క, 8);
	check_క్షేత్రం(s_snapshot_list, 4);
	check_క్షేత్రం(s_error_లెక్క, 4);
	check_క్షేత్రం(s_మొదటి_error_సమయం, 4);
	check_క్షేత్రం(s_మొదటి_error_ino, 4);
	check_క్షేత్రం(s_మొదటి_error_ఖండం, 8);
	check_క్షేత్రం(s_మొదటి_error_func, 32);
	check_క్షేత్రం(s_మొదటి_error_పంక్తి, 4);
	check_క్షేత్రం(s_చివరి_error_సమయం, 4);
	check_క్షేత్రం(s_చివరి_error_ino, 4);
	check_క్షేత్రం(s_చివరి_error_పంక్తి, 4);
	check_క్షేత్రం(s_చివరి_error_ఖండం, 8);
	check_క్షేత్రం(s_చివరి_error_func, 32);
	check_క్షేత్రం(s_mount_opts, 64);
	check_క్షేత్రం(s_usr_quota_inum, 4);
	check_క్షేత్రం(s_grp_quota_inum, 4);
	check_క్షేత్రం(s_overhead_clusters, 4);
	check_క్షేత్రం(s_backup_bgs, 8);
	check_క్షేత్రం(s_encrypt_algos, 4);
	check_క్షేత్రం(s_encrypt_pw_salt, 16);
	check_క్షేత్రం(s_lpf_ino, 4);
	check_క్షేత్రం(s_prj_quota_inum, 4);
	check_క్షేత్రం(s_checksum_విత్తనం, 4);
	check_క్షేత్రం(s_wtime_hi, 1);
	check_క్షేత్రం(s_mtime_hi, 1);
	check_క్షేత్రం(s_mkfs_సమయం_hi, 1);
	check_క్షేత్రం(s_lastcheck_hi, 1);
	check_క్షేత్రం(s_మొదటి_error_సమయం_hi, 1);
	check_క్షేత్రం(s_చివరి_error_సమయం_hi, 1);
	check_క్షేత్రం(s_మొదటి_error_errcode, 1);
	check_క్షేత్రం(s_చివరి_error_errcode, 1);
	check_క్షేత్రం(s_encoding, 2);
	check_క్షేత్రం(s_encoding_flags, 2);
	check_క్షేత్రం(s_orphan_file_inum, 4);
	check_క్షేత్రం(s_reserved, 94 * 4);
	check_క్షేత్రం(s_checksum, 4);
	do_క్షేత్రం("Superblock end", 0, 0, cur_స్థానభ్రంశం, 1024);
#అయితే_ముగింపు
	తిరిగి 0;
}
