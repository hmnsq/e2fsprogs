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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"

#ವ್ಯಾಖ್ಯಾನಿಸು sb_struct ext2_super_ಖಂಡ
#ವ್ಯಾಖ್ಯಾನಿಸು sb_struct_name "ext2_super_block"

ರಚನೆ sb_struct sb;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ offsetof
#ವ್ಯಾಖ್ಯಾನಿಸು offsetof(type, member)  __builtin_offsetof (type, member)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು check_ಕ್ಷೇತ್ರ(x, s) cur_ಸ್ಥಾನಾಂತರ = do_ಕ್ಷೇತ್ರ(#x, s, ಗಾತ್ರ(sb.x),	       \
						offsetof(ರಚನೆ sb_struct, x), \
						cur_ಸ್ಥಾನಾಂತರ)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_ಕ್ಷೇತ್ರ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ, ಚಿಹ್ನೆರಹಿತ ಗಾತ್ರ_2, ಚಿಹ್ನೆರಹಿತ cur_ಗಾತ್ರ,
		    ಚಿಹ್ನೆರಹಿತ ಸ್ಥಾನಾಂತರ, ಚಿಹ್ನೆರಹಿತ cur_ಸ್ಥಾನಾಂತರ)
{
	ಆದರೆ (ಗಾತ್ರ_2 != cur_ಗಾತ್ರ) {
		ಬರೆ("error: %s size %u should be %u\n",
		       ಕ್ಷೇತ್ರ, cur_ಗಾತ್ರ, ಗಾತ್ರ_2);
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (ಸ್ಥಾನಾಂತರ != cur_ಸ್ಥಾನಾಂತರ) {
		ಬರೆ("error: %s offset %u should be %u\n",
		       ಕ್ಷೇತ್ರ, cur_ಸ್ಥಾನಾಂತರ, ಸ್ಥಾನಾಂತರ);
		ನಿರ್ಗಮಿಸು(1);
	}
	ಬರೆ("%8d %-30s %3u\n", ಸ್ಥಾನಾಂತರ, ಕ್ಷೇತ್ರ, ಗಾತ್ರ_2);
	ಹಿಂತಿರುಗಿ ಸ್ಥಾನಾಂತರ + ಗಾತ್ರ_2;
}

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
#ಆದರೆ (__GNUC__ >= 4)
	ಪೂರ್ಣಾಂಕ cur_ಸ್ಥಾನಾಂತರ = 0;

	ಬರೆ("%8s %-30s %3s\n", "offset", "field", "size");
	check_ಕ್ಷೇತ್ರ(s_inodes_ಎಣಿಕೆ, 4);
	check_ಕ್ಷೇತ್ರ(s_ಖಂಡಗಳು_ಎಣಿಕೆ, 4);
	check_ಕ್ಷೇತ್ರ(s_r_ಖಂಡಗಳು_ಎಣಿಕೆ, 4);
	check_ಕ್ಷೇತ್ರ(s_free_ಖಂಡಗಳು_ಎಣಿಕೆ, 4);
	check_ಕ್ಷೇತ್ರ(s_free_inodes_ಎಣಿಕೆ, 4);
	check_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ, 4);
	check_ಕ್ಷೇತ್ರ(s_log_ಖಂಡ_ಗಾತ್ರ, 4);
	check_ಕ್ಷೇತ್ರ(s_log_cluster_ಗಾತ್ರ, 4);
	check_ಕ್ಷೇತ್ರ(s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು, 4);
	check_ಕ್ಷೇತ್ರ(s_clusters_ಪ್ರತಿ_ಗುಂಪು, 4);
	check_ಕ್ಷೇತ್ರ(s_inodes_ಪ್ರತಿ_ಗುಂಪು, 4);
	check_ಕ್ಷೇತ್ರ(s_mtime, 4);
	check_ಕ್ಷೇತ್ರ(s_wtime, 4);
	check_ಕ್ಷೇತ್ರ(s_mnt_ಎಣಿಕೆ, 2);
	check_ಕ್ಷೇತ್ರ(s_max_mnt_ಎಣಿಕೆ, 2);
	check_ಕ್ಷೇತ್ರ(s_magic, 2);
	check_ಕ್ಷೇತ್ರ(s_state, 2);
	check_ಕ್ಷೇತ್ರ(s_errors, 2);
	check_ಕ್ಷೇತ್ರ(s_minor_rev_level, 2);
	check_ಕ್ಷೇತ್ರ(s_lastcheck, 4);
	check_ಕ್ಷೇತ್ರ(s_checkinterval, 4);
	check_ಕ್ಷೇತ್ರ(s_creator_os, 4);
	check_ಕ್ಷೇತ್ರ(s_rev_level, 4);
	check_ಕ್ಷೇತ್ರ(s_def_resuid, 2);
	check_ಕ್ಷೇತ್ರ(s_def_resgid, 2);
	check_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_ino, 4);
	check_ಕ್ಷೇತ್ರ(s_inode_ಗಾತ್ರ, 2);
	check_ಕ್ಷೇತ್ರ(s_ಖಂಡ_ಗುಂಪು_nr, 2);
	check_ಕ್ಷೇತ್ರ(s_feature_compat, 4);
	check_ಕ್ಷೇತ್ರ(s_feature_incompat, 4);
	check_ಕ್ಷೇತ್ರ(s_feature_ro_compat, 4);
	check_ಕ್ಷೇತ್ರ(s_uuid, 16);
	check_ಕ್ಷೇತ್ರ(s_volume_name, 16);
	check_ಕ್ಷೇತ್ರ(s_ಕೊನೆಯ_mounted, 64);
	check_ಕ್ಷೇತ್ರ(s_algorithm_usage_bitmap, 4);
	check_ಕ್ಷೇತ್ರ(s_prealloc_ಖಂಡಗಳು, 1);
	check_ಕ್ಷೇತ್ರ(s_prealloc_dir_ಖಂಡಗಳು, 1);
	check_ಕ್ಷೇತ್ರ(s_reserved_gdt_ಖಂಡಗಳು, 2);
	check_ಕ್ಷೇತ್ರ(s_journal_uuid, 16);
	check_ಕ್ಷೇತ್ರ(s_journal_inum, 4);
	check_ಕ್ಷೇತ್ರ(s_journal_dev, 4);
	check_ಕ್ಷೇತ್ರ(s_ಕೊನೆಯ_orphan, 4);
	check_ಕ್ಷೇತ್ರ(s_hash_ಬೀಜ, 4 * 4);
	check_ಕ್ಷೇತ್ರ(s_def_hash_ಆವೃತ್ತಿ, 1);
	check_ಕ್ಷೇತ್ರ(s_jnl_backup_type, 1);
	check_ಕ್ಷೇತ್ರ(s_desc_ಗಾತ್ರ, 2);
	check_ಕ್ಷೇತ್ರ(s_default_mount_opts, 4);
	check_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_meta_bg, 4);
	check_ಕ್ಷೇತ್ರ(s_mkfs_ಸಮಯ, 4);
	check_ಕ್ಷೇತ್ರ(s_jnl_ಖಂಡಗಳು, 17 * 4);
	check_ಕ್ಷೇತ್ರ(s_ಖಂಡಗಳು_ಎಣಿಕೆ_hi, 4);
	check_ಕ್ಷೇತ್ರ(s_r_ಖಂಡಗಳು_ಎಣಿಕೆ_hi, 4);
	check_ಕ್ಷೇತ್ರ(s_free_ಖಂಡಗಳು_hi, 4);
	check_ಕ್ಷೇತ್ರ(s_min_extra_isize, 2);
	check_ಕ್ಷೇತ್ರ(s_want_extra_isize, 2);
	check_ಕ್ಷೇತ್ರ(s_flags, 4);
	check_ಕ್ಷೇತ್ರ(s_raid_stride, 2);
	check_ಕ್ಷೇತ್ರ(s_mmp_update_interval, 2);
	check_ಕ್ಷೇತ್ರ(s_mmp_ಖಂಡ, 8);
	check_ಕ್ಷೇತ್ರ(s_raid_stripe_ಅಗಲ, 4);
	check_ಕ್ಷೇತ್ರ(s_log_groups_ಪ್ರತಿ_flex, 1);
	check_ಕ್ಷೇತ್ರ(s_checksum_type, 1);
	check_ಕ್ಷೇತ್ರ(s_encryption_level, 1);
	check_ಕ್ಷೇತ್ರ(s_reserved_pad, 1);
	check_ಕ್ಷೇತ್ರ(s_kbytes_written, 8);
	check_ಕ್ಷೇತ್ರ(s_snapshot_inum, 4);
	check_ಕ್ಷೇತ್ರ(s_snapshot_id, 4);
	check_ಕ್ಷೇತ್ರ(s_snapshot_r_ಖಂಡಗಳು_ಎಣಿಕೆ, 8);
	check_ಕ್ಷೇತ್ರ(s_snapshot_list, 4);
	check_ಕ್ಷೇತ್ರ(s_error_ಎಣಿಕೆ, 4);
	check_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_error_ಸಮಯ, 4);
	check_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_error_ino, 4);
	check_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_error_ಖಂಡ, 8);
	check_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_error_func, 32);
	check_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_error_ಸಾಲು, 4);
	check_ಕ್ಷೇತ್ರ(s_ಕೊನೆಯ_error_ಸಮಯ, 4);
	check_ಕ್ಷೇತ್ರ(s_ಕೊನೆಯ_error_ino, 4);
	check_ಕ್ಷೇತ್ರ(s_ಕೊನೆಯ_error_ಸಾಲು, 4);
	check_ಕ್ಷೇತ್ರ(s_ಕೊನೆಯ_error_ಖಂಡ, 8);
	check_ಕ್ಷೇತ್ರ(s_ಕೊನೆಯ_error_func, 32);
	check_ಕ್ಷೇತ್ರ(s_mount_opts, 64);
	check_ಕ್ಷೇತ್ರ(s_usr_quota_inum, 4);
	check_ಕ್ಷೇತ್ರ(s_grp_quota_inum, 4);
	check_ಕ್ಷೇತ್ರ(s_overhead_clusters, 4);
	check_ಕ್ಷೇತ್ರ(s_backup_bgs, 8);
	check_ಕ್ಷೇತ್ರ(s_encrypt_algos, 4);
	check_ಕ್ಷೇತ್ರ(s_encrypt_pw_salt, 16);
	check_ಕ್ಷೇತ್ರ(s_lpf_ino, 4);
	check_ಕ್ಷೇತ್ರ(s_prj_quota_inum, 4);
	check_ಕ್ಷೇತ್ರ(s_checksum_ಬೀಜ, 4);
	check_ಕ್ಷೇತ್ರ(s_wtime_hi, 1);
	check_ಕ್ಷೇತ್ರ(s_mtime_hi, 1);
	check_ಕ್ಷೇತ್ರ(s_mkfs_ಸಮಯ_hi, 1);
	check_ಕ್ಷೇತ್ರ(s_lastcheck_hi, 1);
	check_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_error_ಸಮಯ_hi, 1);
	check_ಕ್ಷೇತ್ರ(s_ಕೊನೆಯ_error_ಸಮಯ_hi, 1);
	check_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_error_errcode, 1);
	check_ಕ್ಷೇತ್ರ(s_ಕೊನೆಯ_error_errcode, 1);
	check_ಕ್ಷೇತ್ರ(s_encoding, 2);
	check_ಕ್ಷೇತ್ರ(s_encoding_flags, 2);
	check_ಕ್ಷೇತ್ರ(s_orphan_file_inum, 4);
	check_ಕ್ಷೇತ್ರ(s_reserved, 94 * 4);
	check_ಕ್ಷೇತ್ರ(s_checksum, 4);
	do_ಕ್ಷೇತ್ರ("Superblock end", 0, 0, cur_ಸ್ಥಾನಾಂತರ, 1024);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ 0;
}
