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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sb_struct ext2_super_ਖੰਡ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sb_struct_name "ext2_super_block"

ਬਣਤਰ sb_struct sb;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ offsetof
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ offsetof(type, member)  __builtin_offsetof (type, member)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ check_ਖੇਤਰ(x, s) cur_ਵਿੱਥ = do_ਖੇਤਰ(#x, s, ਆਕਾਰ(sb.x),	       \
						offsetof(ਬਣਤਰ sb_struct, x), \
						cur_ਵਿੱਥ)

ਸਥਿਰ ਪੂਰਨ do_ਖੇਤਰ(ਅਚਲ ਅੱਖਰ *ਖੇਤਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ, ਬਿਨਾਨਿਸ਼ਾਨੀ cur_ਮਾਪ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ, ਬਿਨਾਨਿਸ਼ਾਨੀ cur_ਵਿੱਥ)
{
	ਜੇ (ਮਾਪ != cur_ਮਾਪ) {
		ਲਿਖੋ("error: %s size %u should be %u\n",
		       ਖੇਤਰ, cur_ਮਾਪ, ਮਾਪ);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (ਵਿੱਥ != cur_ਵਿੱਥ) {
		ਲਿਖੋ("error: %s offset %u should be %u\n",
		       ਖੇਤਰ, cur_ਵਿੱਥ, ਵਿੱਥ);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਿਖੋ("%8d %-30s %3u\n", ਵਿੱਥ, ਖੇਤਰ, ਮਾਪ);
	ਵਾਪਸ ਵਿੱਥ + ਮਾਪ;
}

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
#ਜੇ (__GNUC__ >= 4)
	ਪੂਰਨ cur_ਵਿੱਥ = 0;

	ਲਿਖੋ("%8s %-30s %3s\n", "offset", "field", "size");
	check_ਖੇਤਰ(s_inodes_ਗਿਣਤੀ, 4);
	check_ਖੇਤਰ(s_ਖੰਡਾਂ_ਗਿਣਤੀ, 4);
	check_ਖੇਤਰ(s_r_ਖੰਡਾਂ_ਗਿਣਤੀ, 4);
	check_ਖੇਤਰ(s_free_ਖੰਡਾਂ_ਗਿਣਤੀ, 4);
	check_ਖੇਤਰ(s_free_inodes_ਗਿਣਤੀ, 4);
	check_ਖੇਤਰ(s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ, 4);
	check_ਖੇਤਰ(s_log_ਖੰਡ_ਮਾਪ, 4);
	check_ਖੇਤਰ(s_log_cluster_ਮਾਪ, 4);
	check_ਖੇਤਰ(s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ, 4);
	check_ਖੇਤਰ(s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ, 4);
	check_ਖੇਤਰ(s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ, 4);
	check_ਖੇਤਰ(s_mtime, 4);
	check_ਖੇਤਰ(s_wtime, 4);
	check_ਖੇਤਰ(s_mnt_ਗਿਣਤੀ, 2);
	check_ਖੇਤਰ(s_max_mnt_ਗਿਣਤੀ, 2);
	check_ਖੇਤਰ(s_magic, 2);
	check_ਖੇਤਰ(s_state, 2);
	check_ਖੇਤਰ(s_errors, 2);
	check_ਖੇਤਰ(s_minor_rev_level, 2);
	check_ਖੇਤਰ(s_lastcheck, 4);
	check_ਖੇਤਰ(s_checkinterval, 4);
	check_ਖੇਤਰ(s_creator_os, 4);
	check_ਖੇਤਰ(s_rev_level, 4);
	check_ਖੇਤਰ(s_def_resuid, 2);
	check_ਖੇਤਰ(s_def_resgid, 2);
	check_ਖੇਤਰ(s_ਪਹਿਲਾ_ino, 4);
	check_ਖੇਤਰ(s_inode_ਮਾਪ, 2);
	check_ਖੇਤਰ(s_ਖੰਡ_ਸਮੂਹ_nr, 2);
	check_ਖੇਤਰ(s_feature_compat, 4);
	check_ਖੇਤਰ(s_feature_incompat, 4);
	check_ਖੇਤਰ(s_feature_ro_compat, 4);
	check_ਖੇਤਰ(s_uuid, 16);
	check_ਖੇਤਰ(s_volume_name, 16);
	check_ਖੇਤਰ(s_ਆਖਰੀ_mounted, 64);
	check_ਖੇਤਰ(s_algorithm_usage_bitmap, 4);
	check_ਖੇਤਰ(s_prealloc_ਖੰਡਾਂ, 1);
	check_ਖੇਤਰ(s_prealloc_dir_ਖੰਡਾਂ, 1);
	check_ਖੇਤਰ(s_reserved_gdt_ਖੰਡਾਂ, 2);
	check_ਖੇਤਰ(s_journal_uuid, 16);
	check_ਖੇਤਰ(s_journal_inum, 4);
	check_ਖੇਤਰ(s_journal_dev, 4);
	check_ਖੇਤਰ(s_ਆਖਰੀ_orphan, 4);
	check_ਖੇਤਰ(s_hash_ਬੀਜ, 4 * 4);
	check_ਖੇਤਰ(s_def_hash_ਸੰਸਕਰਣ, 1);
	check_ਖੇਤਰ(s_jnl_backup_type, 1);
	check_ਖੇਤਰ(s_desc_ਮਾਪ, 2);
	check_ਖੇਤਰ(s_default_mount_opts, 4);
	check_ਖੇਤਰ(s_ਪਹਿਲਾ_meta_bg, 4);
	check_ਖੇਤਰ(s_mkfs_ਸਮਾਂ, 4);
	check_ਖੇਤਰ(s_jnl_ਖੰਡਾਂ, 17 * 4);
	check_ਖੇਤਰ(s_ਖੰਡਾਂ_ਗਿਣਤੀ_hi, 4);
	check_ਖੇਤਰ(s_r_ਖੰਡਾਂ_ਗਿਣਤੀ_hi, 4);
	check_ਖੇਤਰ(s_free_ਖੰਡਾਂ_hi, 4);
	check_ਖੇਤਰ(s_min_extra_isize, 2);
	check_ਖੇਤਰ(s_want_extra_isize, 2);
	check_ਖੇਤਰ(s_flags, 4);
	check_ਖੇਤਰ(s_raid_stride, 2);
	check_ਖੇਤਰ(s_mmp_update_interval, 2);
	check_ਖੇਤਰ(s_mmp_ਖੰਡ, 8);
	check_ਖੇਤਰ(s_raid_stripe_ਚੌੜਾਈ, 4);
	check_ਖੇਤਰ(s_log_groups_ਪ੍ਰਤੀ_flex, 1);
	check_ਖੇਤਰ(s_checksum_type, 1);
	check_ਖੇਤਰ(s_encryption_level, 1);
	check_ਖੇਤਰ(s_reserved_pad, 1);
	check_ਖੇਤਰ(s_kbytes_written, 8);
	check_ਖੇਤਰ(s_snapshot_inum, 4);
	check_ਖੇਤਰ(s_snapshot_id, 4);
	check_ਖੇਤਰ(s_snapshot_r_ਖੰਡਾਂ_ਗਿਣਤੀ, 8);
	check_ਖੇਤਰ(s_snapshot_list, 4);
	check_ਖੇਤਰ(s_error_ਗਿਣਤੀ, 4);
	check_ਖੇਤਰ(s_ਪਹਿਲਾ_error_ਸਮਾਂ, 4);
	check_ਖੇਤਰ(s_ਪਹਿਲਾ_error_ino, 4);
	check_ਖੇਤਰ(s_ਪਹਿਲਾ_error_ਖੰਡ, 8);
	check_ਖੇਤਰ(s_ਪਹਿਲਾ_error_func, 32);
	check_ਖੇਤਰ(s_ਪਹਿਲਾ_error_ਸਤਰ, 4);
	check_ਖੇਤਰ(s_ਆਖਰੀ_error_ਸਮਾਂ, 4);
	check_ਖੇਤਰ(s_ਆਖਰੀ_error_ino, 4);
	check_ਖੇਤਰ(s_ਆਖਰੀ_error_ਸਤਰ, 4);
	check_ਖੇਤਰ(s_ਆਖਰੀ_error_ਖੰਡ, 8);
	check_ਖੇਤਰ(s_ਆਖਰੀ_error_func, 32);
	check_ਖੇਤਰ(s_mount_opts, 64);
	check_ਖੇਤਰ(s_usr_quota_inum, 4);
	check_ਖੇਤਰ(s_grp_quota_inum, 4);
	check_ਖੇਤਰ(s_overhead_clusters, 4);
	check_ਖੇਤਰ(s_backup_bgs, 8);
	check_ਖੇਤਰ(s_encrypt_algos, 4);
	check_ਖੇਤਰ(s_encrypt_pw_salt, 16);
	check_ਖੇਤਰ(s_lpf_ino, 4);
	check_ਖੇਤਰ(s_prj_quota_inum, 4);
	check_ਖੇਤਰ(s_checksum_ਬੀਜ, 4);
	check_ਖੇਤਰ(s_wtime_hi, 1);
	check_ਖੇਤਰ(s_mtime_hi, 1);
	check_ਖੇਤਰ(s_mkfs_ਸਮਾਂ_hi, 1);
	check_ਖੇਤਰ(s_lastcheck_hi, 1);
	check_ਖੇਤਰ(s_ਪਹਿਲਾ_error_ਸਮਾਂ_hi, 1);
	check_ਖੇਤਰ(s_ਆਖਰੀ_error_ਸਮਾਂ_hi, 1);
	check_ਖੇਤਰ(s_ਪਹਿਲਾ_error_errcode, 1);
	check_ਖੇਤਰ(s_ਆਖਰੀ_error_errcode, 1);
	check_ਖੇਤਰ(s_encoding, 2);
	check_ਖੇਤਰ(s_encoding_flags, 2);
	check_ਖੇਤਰ(s_orphan_file_inum, 4);
	check_ਖੇਤਰ(s_reserved, 94 * 4);
	check_ਖੇਤਰ(s_checksum, 4);
	do_ਖੇਤਰ("Superblock end", 0, 0, cur_ਵਿੱਥ, 1024);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ 0;
}
