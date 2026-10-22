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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "ext2_fs.h"

#परिभाषय sb_struct ext2_super_खण्डम्
#परिभाषय sb_struct_name "ext2_super_block"

संरचना sb_struct sb;

#यदि_अपरिभाषितम् offsetof
#परिभाषय offsetof(type, member)  __builtin_offsetof (type, member)
#यदि_अन्तः

#परिभाषय check_क्षेत्रम्(x, s) cur_विस्थापनम् = do_क्षेत्रम्(#x, s, परिमाण(sb.x),	       \
						offsetof(संरचना sb_struct, x), \
						cur_विस्थापनम्)

स्थिर पूर्णाङ्क do_क्षेत्रम्(ध्रुव वर्ण *क्षेत्रम्, अचिह्नित आकार, अचिह्नित cur_आकार,
		    अचिह्नित विस्थापनम्, अचिह्नित cur_विस्थापनम्)
{
	यदि (आकार != cur_आकार) {
		प्रारूप_लिख("error: %s size %u should be %u\n",
		       क्षेत्रम्, cur_आकार, आकार);
		निर्गच्छ(1);
	}
	यदि (विस्थापनम् != cur_विस्थापनम्) {
		प्रारूप_लिख("error: %s offset %u should be %u\n",
		       क्षेत्रम्, cur_विस्थापनम्, विस्थापनम्);
		निर्गच्छ(1);
	}
	प्रारूप_लिख("%8d %-30s %3u\n", विस्थापनम्, क्षेत्रम्, आकार);
	प्रत्यागम विस्थापनम् + आकार;
}

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
#यदि (__GNUC__ >= 4)
	पूर्णाङ्क cur_विस्थापनम् = 0;

	प्रारूप_लिख("%8s %-30s %3s\n", "offset", "field", "size");
	check_क्षेत्रम्(s_inodes_गणनम्, 4);
	check_क्षेत्रम्(s_खण्डानि_गणनम्, 4);
	check_क्षेत्रम्(s_r_खण्डानि_गणनम्, 4);
	check_क्षेत्रम्(s_free_खण्डानि_गणनम्, 4);
	check_क्षेत्रम्(s_free_inodes_गणनम्, 4);
	check_क्षेत्रम्(s_first_दत्त_खण्डम्, 4);
	check_क्षेत्रम्(s_log_खण्डम्_आकार, 4);
	check_क्षेत्रम्(s_log_cluster_आकार, 4);
	check_क्षेत्रम्(s_खण्डानि_प्रत्येकम्_गणः, 4);
	check_क्षेत्रम्(s_clusters_प्रत्येकम्_गणः, 4);
	check_क्षेत्रम्(s_inodes_प्रत्येकम्_गणः, 4);
	check_क्षेत्रम्(s_mtime, 4);
	check_क्षेत्रम्(s_wtime, 4);
	check_क्षेत्रम्(s_mnt_गणनम्, 2);
	check_क्षेत्रम्(s_max_mnt_गणनम्, 2);
	check_क्षेत्रम्(s_magic, 2);
	check_क्षेत्रम्(s_state, 2);
	check_क्षेत्रम्(s_errors, 2);
	check_क्षेत्रम्(s_minor_rev_level, 2);
	check_क्षेत्रम्(s_lastcheck, 4);
	check_क्षेत्रम्(s_checkinterval, 4);
	check_क्षेत्रम्(s_creator_os, 4);
	check_क्षेत्रम्(s_rev_level, 4);
	check_क्षेत्रम्(s_def_resuid, 2);
	check_क्षेत्रम्(s_def_resgid, 2);
	check_क्षेत्रम्(s_first_ino, 4);
	check_क्षेत्रम्(s_inode_आकार, 2);
	check_क्षेत्रम्(s_खण्डम्_गणः_nr, 2);
	check_क्षेत्रम्(s_feature_compat, 4);
	check_क्षेत्रम्(s_feature_incompat, 4);
	check_क्षेत्रम्(s_feature_ro_compat, 4);
	check_क्षेत्रम्(s_uuid, 16);
	check_क्षेत्रम्(s_volume_name, 16);
	check_क्षेत्रम्(s_चरमः_mounted, 64);
	check_क्षेत्रम्(s_algorithm_usage_bitmap, 4);
	check_क्षेत्रम्(s_prealloc_खण्डानि, 1);
	check_क्षेत्रम्(s_prealloc_dir_खण्डानि, 1);
	check_क्षेत्रम्(s_reserved_gdt_खण्डानि, 2);
	check_क्षेत्रम्(s_journal_uuid, 16);
	check_क्षेत्रम्(s_journal_inum, 4);
	check_क्षेत्रम्(s_journal_dev, 4);
	check_क्षेत्रम्(s_चरमः_orphan, 4);
	check_क्षेत्रम्(s_hash_बीजम्, 4 * 4);
	check_क्षेत्रम्(s_def_hash_संस्करणम्, 1);
	check_क्षेत्रम्(s_jnl_backup_type, 1);
	check_क्षेत्रम्(s_desc_आकार, 2);
	check_क्षेत्रम्(s_default_mount_opts, 4);
	check_क्षेत्रम्(s_first_meta_bg, 4);
	check_क्षेत्रम्(s_mkfs_time, 4);
	check_क्षेत्रम्(s_jnl_खण्डानि, 17 * 4);
	check_क्षेत्रम्(s_खण्डानि_गणनम्_hi, 4);
	check_क्षेत्रम्(s_r_खण्डानि_गणनम्_hi, 4);
	check_क्षेत्रम्(s_free_खण्डानि_hi, 4);
	check_क्षेत्रम्(s_min_extra_isize, 2);
	check_क्षेत्रम्(s_want_extra_isize, 2);
	check_क्षेत्रम्(s_flags, 4);
	check_क्षेत्रम्(s_raid_stride, 2);
	check_क्षेत्रम्(s_mmp_update_interval, 2);
	check_क्षेत्रम्(s_mmp_खण्डम्, 8);
	check_क्षेत्रम्(s_raid_stripe_विस्तारः, 4);
	check_क्षेत्रम्(s_log_groups_प्रत्येकम्_flex, 1);
	check_क्षेत्रम्(s_checksum_type, 1);
	check_क्षेत्रम्(s_encryption_level, 1);
	check_क्षेत्रम्(s_reserved_pad, 1);
	check_क्षेत्रम्(s_kbytes_written, 8);
	check_क्षेत्रम्(s_snapshot_inum, 4);
	check_क्षेत्रम्(s_snapshot_id, 4);
	check_क्षेत्रम्(s_snapshot_r_खण्डानि_गणनम्, 8);
	check_क्षेत्रम्(s_snapshot_list, 4);
	check_क्षेत्रम्(s_error_गणनम्, 4);
	check_क्षेत्रम्(s_first_error_time, 4);
	check_क्षेत्रम्(s_first_error_ino, 4);
	check_क्षेत्रम्(s_first_error_खण्डम्, 8);
	check_क्षेत्रम्(s_first_error_func, 32);
	check_क्षेत्रम्(s_first_error_पङ्क्तिः, 4);
	check_क्षेत्रम्(s_चरमः_error_time, 4);
	check_क्षेत्रम्(s_चरमः_error_ino, 4);
	check_क्षेत्रम्(s_चरमः_error_पङ्क्तिः, 4);
	check_क्षेत्रम्(s_चरमः_error_खण्डम्, 8);
	check_क्षेत्रम्(s_चरमः_error_func, 32);
	check_क्षेत्रम्(s_mount_opts, 64);
	check_क्षेत्रम्(s_usr_quota_inum, 4);
	check_क्षेत्रम्(s_grp_quota_inum, 4);
	check_क्षेत्रम्(s_overhead_clusters, 4);
	check_क्षेत्रम्(s_backup_bgs, 8);
	check_क्षेत्रम्(s_encrypt_algos, 4);
	check_क्षेत्रम्(s_encrypt_pw_salt, 16);
	check_क्षेत्रम्(s_lpf_ino, 4);
	check_क्षेत्रम्(s_prj_quota_inum, 4);
	check_क्षेत्रम्(s_checksum_बीजम्, 4);
	check_क्षेत्रम्(s_wtime_hi, 1);
	check_क्षेत्रम्(s_mtime_hi, 1);
	check_क्षेत्रम्(s_mkfs_time_hi, 1);
	check_क्षेत्रम्(s_lastcheck_hi, 1);
	check_क्षेत्रम्(s_first_error_time_hi, 1);
	check_क्षेत्रम्(s_चरमः_error_time_hi, 1);
	check_क्षेत्रम्(s_first_error_errcode, 1);
	check_क्षेत्रम्(s_चरमः_error_errcode, 1);
	check_क्षेत्रम्(s_encoding, 2);
	check_क्षेत्रम्(s_encoding_flags, 2);
	check_क्षेत्रम्(s_orphan_file_inum, 4);
	check_क्षेत्रम्(s_reserved, 94 * 4);
	check_क्षेत्रम्(s_checksum, 4);
	do_क्षेत्रम्("Superblock end", 0, 0, cur_विस्थापनम्, 1024);
#यदि_अन्तः
	प्रत्यागम 0;
}
