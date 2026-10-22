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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <unistd.h>
#समावेश <stdlib.h>

#समावेश "ext2_fs.h"

#घोषणा sb_struct ext2_super_block
#घोषणा sb_struct_name "ext2_super_block"

काष्ठा sb_struct sb;

#अगर_अघोषित दुरत्व
#घोषणा दुरत्व(type, member)  __builtin_offsetof (type, member)
#पूर्ण_अगर

#घोषणा check_field(x, s) cur_offset = do_field(#x, s, माप(sb.x),	       \
						दुरत्व(काष्ठा sb_struct, x), \
						cur_offset)

अटल पूर्णांक do_field(स्थिर अक्षर *field, अचिन्हित आकार, अचिन्हित cur_आकार,
		    अचिन्हित offset, अचिन्हित cur_offset)
{
	अगर (आकार != cur_आकार) {
		म_लिखो("error: %s size %u should be %u\n",
		       field, cur_आकार, आकार);
		निकास(1);
	}
	अगर (offset != cur_offset) {
		म_लिखो("error: %s offset %u should be %u\n",
		       field, cur_offset, offset);
		निकास(1);
	}
	म_लिखो("%8d %-30s %3u\n", offset, field, आकार);
	वापस offset + आकार;
}

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
#अगर (__GNUC__ >= 4)
	पूर्णांक cur_offset = 0;

	म_लिखो("%8s %-30s %3s\n", "offset", "field", "size");
	check_field(s_inodes_गिनती, 4);
	check_field(s_blocks_गिनती, 4);
	check_field(s_r_blocks_गिनती, 4);
	check_field(s_free_blocks_गिनती, 4);
	check_field(s_free_inodes_गिनती, 4);
	check_field(s_first_तथ्य_block, 4);
	check_field(s_log_block_आकार, 4);
	check_field(s_log_cluster_आकार, 4);
	check_field(s_blocks_per_group, 4);
	check_field(s_clusters_per_group, 4);
	check_field(s_inodes_per_group, 4);
	check_field(s_mtime, 4);
	check_field(s_wtime, 4);
	check_field(s_mnt_गिनती, 2);
	check_field(s_max_mnt_गिनती, 2);
	check_field(s_magic, 2);
	check_field(s_state, 2);
	check_field(s_errors, 2);
	check_field(s_minor_rev_level, 2);
	check_field(s_lastcheck, 4);
	check_field(s_checkinterval, 4);
	check_field(s_creator_os, 4);
	check_field(s_rev_level, 4);
	check_field(s_def_resuid, 2);
	check_field(s_def_resgid, 2);
	check_field(s_first_ino, 4);
	check_field(s_inode_आकार, 2);
	check_field(s_block_group_nr, 2);
	check_field(s_feature_compat, 4);
	check_field(s_feature_incompat, 4);
	check_field(s_feature_ro_compat, 4);
	check_field(s_uuid, 16);
	check_field(s_volume_name, 16);
	check_field(s_last_mounted, 64);
	check_field(s_algorithm_usage_bitmap, 4);
	check_field(s_prealloc_blocks, 1);
	check_field(s_prealloc_dir_blocks, 1);
	check_field(s_reserved_gdt_blocks, 2);
	check_field(s_journal_uuid, 16);
	check_field(s_journal_inum, 4);
	check_field(s_journal_dev, 4);
	check_field(s_last_orphan, 4);
	check_field(s_hash_seed, 4 * 4);
	check_field(s_def_hash_version, 1);
	check_field(s_jnl_backup_type, 1);
	check_field(s_desc_आकार, 2);
	check_field(s_default_mount_opts, 4);
	check_field(s_first_meta_bg, 4);
	check_field(s_mkfs_time, 4);
	check_field(s_jnl_blocks, 17 * 4);
	check_field(s_blocks_गिनती_hi, 4);
	check_field(s_r_blocks_गिनती_hi, 4);
	check_field(s_free_blocks_hi, 4);
	check_field(s_min_extra_isize, 2);
	check_field(s_want_extra_isize, 2);
	check_field(s_flags, 4);
	check_field(s_raid_stride, 2);
	check_field(s_mmp_update_interval, 2);
	check_field(s_mmp_block, 8);
	check_field(s_raid_stripe_width, 4);
	check_field(s_log_groups_per_flex, 1);
	check_field(s_checksum_type, 1);
	check_field(s_encryption_level, 1);
	check_field(s_reserved_pad, 1);
	check_field(s_kbytes_written, 8);
	check_field(s_snapshot_inum, 4);
	check_field(s_snapshot_id, 4);
	check_field(s_snapshot_r_blocks_गिनती, 8);
	check_field(s_snapshot_list, 4);
	check_field(s_error_गिनती, 4);
	check_field(s_first_error_time, 4);
	check_field(s_first_error_ino, 4);
	check_field(s_first_error_block, 8);
	check_field(s_first_error_func, 32);
	check_field(s_first_error_पंक्ति, 4);
	check_field(s_last_error_time, 4);
	check_field(s_last_error_ino, 4);
	check_field(s_last_error_पंक्ति, 4);
	check_field(s_last_error_block, 8);
	check_field(s_last_error_func, 32);
	check_field(s_mount_opts, 64);
	check_field(s_usr_quota_inum, 4);
	check_field(s_grp_quota_inum, 4);
	check_field(s_overhead_clusters, 4);
	check_field(s_backup_bgs, 8);
	check_field(s_encrypt_algos, 4);
	check_field(s_encrypt_pw_salt, 16);
	check_field(s_lpf_ino, 4);
	check_field(s_prj_quota_inum, 4);
	check_field(s_checksum_seed, 4);
	check_field(s_wtime_hi, 1);
	check_field(s_mtime_hi, 1);
	check_field(s_mkfs_time_hi, 1);
	check_field(s_lastcheck_hi, 1);
	check_field(s_first_error_time_hi, 1);
	check_field(s_last_error_time_hi, 1);
	check_field(s_first_error_errcode, 1);
	check_field(s_last_error_errcode, 1);
	check_field(s_encoding, 2);
	check_field(s_encoding_flags, 2);
	check_field(s_orphan_file_inum, 4);
	check_field(s_reserved, 94 * 4);
	check_field(s_checksum, 4);
	do_field("Superblock end", 0, 0, cur_offset, 1024);
#पूर्ण_अगर
	वापस 0;
}
