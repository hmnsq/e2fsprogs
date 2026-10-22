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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext2_ext_attr.h>

ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_super(ਬਣਤਰ ext2_super_ਖੰਡ * sb)
{
	ਪੂਰਨ i;

	sb->s_inodes_ਗਿਣਤੀ = ext2fs_swab32(sb->s_inodes_ਗਿਣਤੀ);
	sb->s_ਖੰਡਾਂ_ਗਿਣਤੀ = ext2fs_swab32(sb->s_ਖੰਡਾਂ_ਗਿਣਤੀ);
	sb->s_r_ਖੰਡਾਂ_ਗਿਣਤੀ = ext2fs_swab32(sb->s_r_ਖੰਡਾਂ_ਗਿਣਤੀ);
	sb->s_free_ਖੰਡਾਂ_ਗਿਣਤੀ = ext2fs_swab32(sb->s_free_ਖੰਡਾਂ_ਗਿਣਤੀ);
	sb->s_free_inodes_ਗਿਣਤੀ = ext2fs_swab32(sb->s_free_inodes_ਗਿਣਤੀ);
	sb->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ = ext2fs_swab32(sb->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ);
	sb->s_log_ਖੰਡ_ਮਾਪ = ext2fs_swab32(sb->s_log_ਖੰਡ_ਮਾਪ);
	sb->s_log_cluster_ਮਾਪ = ext2fs_swab32(sb->s_log_cluster_ਮਾਪ);
	sb->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ = ext2fs_swab32(sb->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ);
	sb->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ = ext2fs_swab32(sb->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ);
	sb->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ = ext2fs_swab32(sb->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ);
	sb->s_mtime = ext2fs_swab32(sb->s_mtime);
	sb->s_wtime = ext2fs_swab32(sb->s_wtime);
	sb->s_mnt_ਗਿਣਤੀ = ext2fs_swab16(sb->s_mnt_ਗਿਣਤੀ);
	sb->s_max_mnt_ਗਿਣਤੀ = ext2fs_swab16(sb->s_max_mnt_ਗਿਣਤੀ);
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
	sb->s_ਪਹਿਲਾ_ino = ext2fs_swab32(sb->s_ਪਹਿਲਾ_ino);
	sb->s_inode_ਮਾਪ = ext2fs_swab16(sb->s_inode_ਮਾਪ);
	sb->s_ਖੰਡ_ਸਮੂਹ_nr = ext2fs_swab16(sb->s_ਖੰਡ_ਸਮੂਹ_nr);
	sb->s_feature_compat = ext2fs_swab32(sb->s_feature_compat);
	sb->s_feature_incompat = ext2fs_swab32(sb->s_feature_incompat);
	sb->s_feature_ro_compat = ext2fs_swab32(sb->s_feature_ro_compat);
	/* sb->s_uuid is __u8 and does not need swabbing */
	/* sb->s_volume_name is char and does not need swabbing */
	/* sb->s_last_mounted is char and does not need swabbing */
	sb->s_algorithm_usage_bitmap = ext2fs_swab32(sb->s_algorithm_usage_bitmap);
	/* sb->s_prealloc_blocks is __u8 and does not need swabbing */
	/* sb->s_prealloc_dir_blocks is __u8 and does not need swabbing */
	sb->s_reserved_gdt_ਖੰਡਾਂ = ext2fs_swab16(sb->s_reserved_gdt_ਖੰਡਾਂ);
	/* sb->s_journal_uuid is __u8 and does not need swabbing */
	sb->s_journal_inum = ext2fs_swab32(sb->s_journal_inum);
	sb->s_journal_dev = ext2fs_swab32(sb->s_journal_dev);
	sb->s_ਆਖਰੀ_orphan = ext2fs_swab32(sb->s_ਆਖਰੀ_orphan);
	ਲਈ (i = 0; i < 4; i++)
		sb->s_hash_ਬੀਜ[i] = ext2fs_swab32(sb->s_hash_ਬੀਜ[i]);
	/* sb->s_def_hash_version is __u8 and does not need swabbing */
	/* sb->s_jnl_backup_type is __u8 and does not need swabbing */
	sb->s_desc_ਮਾਪ = ext2fs_swab16(sb->s_desc_ਮਾਪ);
	sb->s_default_mount_opts = ext2fs_swab32(sb->s_default_mount_opts);
	sb->s_ਪਹਿਲਾ_meta_bg = ext2fs_swab32(sb->s_ਪਹਿਲਾ_meta_bg);
	sb->s_mkfs_ਸਮਾਂ = ext2fs_swab32(sb->s_mkfs_ਸਮਾਂ);
	/* if journal backup is for a valid extent-based journal... */
	ਜੇ (ext2fs_extent_ਸਿਰਲੇਖ_verify(sb->s_jnl_ਖੰਡਾਂ,
					ਆਕਾਰ(sb->s_jnl_ਖੰਡਾਂ)) == 0) {
		/* ... swap only the journal i_size and i_size_high,
		 * and the extent data is not swapped on read */
		i = 15;
	} ਨਹੀਂਤਾਂ {
		/* direct/indirect journal: swap it all */
		i = 0;
	}
	ਲਈ (; i < 17; i++)
		sb->s_jnl_ਖੰਡਾਂ[i] = ext2fs_swab32(sb->s_jnl_ਖੰਡਾਂ[i]);
	sb->s_ਖੰਡਾਂ_ਗਿਣਤੀ_hi = ext2fs_swab32(sb->s_ਖੰਡਾਂ_ਗਿਣਤੀ_hi);
	sb->s_r_ਖੰਡਾਂ_ਗਿਣਤੀ_hi = ext2fs_swab32(sb->s_r_ਖੰਡਾਂ_ਗਿਣਤੀ_hi);
	sb->s_free_ਖੰਡਾਂ_hi = ext2fs_swab32(sb->s_free_ਖੰਡਾਂ_hi);
	sb->s_min_extra_isize = ext2fs_swab16(sb->s_min_extra_isize);
	sb->s_want_extra_isize = ext2fs_swab16(sb->s_want_extra_isize);
	sb->s_flags = ext2fs_swab32(sb->s_flags);
	sb->s_raid_stride = ext2fs_swab16(sb->s_raid_stride);
	sb->s_mmp_update_interval = ext2fs_swab16(sb->s_mmp_update_interval);
	sb->s_mmp_ਖੰਡ = ext2fs_swab64(sb->s_mmp_ਖੰਡ);
	sb->s_raid_stripe_ਚੌੜਾਈ = ext2fs_swab32(sb->s_raid_stripe_ਚੌੜਾਈ);
	/* sb->s_log_groups_per_flex is __u8 and does not need swabbing */
	/* sb->s_checksum_type is __u8 and does not need swabbing */
	/* sb->s_encryption_level is __u8 and does not need swabbing */
	/* sb->s_reserved_pad is __u8 and does not need swabbing */
	sb->s_kbytes_written = ext2fs_swab64(sb->s_kbytes_written);
	sb->s_snapshot_inum = ext2fs_swab32(sb->s_snapshot_inum);
	sb->s_snapshot_id = ext2fs_swab32(sb->s_snapshot_id);
	sb->s_snapshot_r_ਖੰਡਾਂ_ਗਿਣਤੀ =
		ext2fs_swab64(sb->s_snapshot_r_ਖੰਡਾਂ_ਗਿਣਤੀ);
	sb->s_snapshot_list = ext2fs_swab32(sb->s_snapshot_list);
	sb->s_error_ਗਿਣਤੀ = ext2fs_swab32(sb->s_error_ਗਿਣਤੀ);
	sb->s_ਪਹਿਲਾ_error_ਸਮਾਂ = ext2fs_swab32(sb->s_ਪਹਿਲਾ_error_ਸਮਾਂ);
	sb->s_ਪਹਿਲਾ_error_ino = ext2fs_swab32(sb->s_ਪਹਿਲਾ_error_ino);
	sb->s_ਪਹਿਲਾ_error_ਖੰਡ = ext2fs_swab64(sb->s_ਪਹਿਲਾ_error_ਖੰਡ);
	/* sb->s_first_error_func is __u8 and does not need swabbing */
	sb->s_ਆਖਰੀ_error_ਸਮਾਂ = ext2fs_swab32(sb->s_ਆਖਰੀ_error_ਸਮਾਂ);
	sb->s_ਆਖਰੀ_error_ino = ext2fs_swab32(sb->s_ਆਖਰੀ_error_ino);
	sb->s_ਆਖਰੀ_error_ਖੰਡ = ext2fs_swab64(sb->s_ਆਖਰੀ_error_ਖੰਡ);
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
	sb->s_checksum_ਬੀਜ = ext2fs_swab32(sb->s_checksum_ਬੀਜ);
	/* s_*_time_hi are __u8 and does not need swabbing */
	sb->s_encoding = ext2fs_swab16(sb->s_encoding);
	sb->s_encoding_flags = ext2fs_swab16(sb->s_encoding_flags);
	sb->s_orphan_file_inum = ext2fs_swab32(sb->s_orphan_file_inum);
	/* catch when new fields are used from s_reserved */
	EXT2FS_BUILD_BUG_ON(ਆਕਾਰ(sb->s_reserved) != 94 * ਆਕਾਰ(__le32));
	sb->s_checksum = ext2fs_swab32(sb->s_checksum);
}

ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(ext2_filsys fs, ਬਣਤਰ ext2_ਸਮੂਹ_desc *gdp)
{
	ਬਣਤਰ ext4_ਸਮੂਹ_desc *gdp4 = (ਬਣਤਰ ext4_ਸਮੂਹ_desc *)gdp;

	/* Do the 32-bit parts first */
	gdp->bg_ਖੰਡ_bitmap = ext2fs_swab32(gdp->bg_ਖੰਡ_bitmap);
	gdp->bg_inode_bitmap = ext2fs_swab32(gdp->bg_inode_bitmap);
	gdp->bg_inode_ਸਾਰਣੀ = ext2fs_swab32(gdp->bg_inode_ਸਾਰਣੀ);
	gdp->bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ = ext2fs_swab16(gdp->bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ);
	gdp->bg_free_inodes_ਗਿਣਤੀ = ext2fs_swab16(gdp->bg_free_inodes_ਗਿਣਤੀ);
	gdp->bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ = ext2fs_swab16(gdp->bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ);
	gdp->bg_flags = ext2fs_swab16(gdp->bg_flags);
	gdp->bg_exclude_bitmap_lo = ext2fs_swab32(gdp->bg_exclude_bitmap_lo);
	gdp->bg_ਖੰਡ_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_ਖੰਡ_bitmap_csum_lo);
	gdp->bg_inode_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_inode_bitmap_csum_lo);
	gdp->bg_itable_ਅਣਵਰਤਿਆ = ext2fs_swab16(gdp->bg_itable_ਅਣਵਰਤਿਆ);
	gdp->bg_checksum = ext2fs_swab16(gdp->bg_checksum);
	/* If we're 32-bit, we're done */
	ਜੇ (fs == ਨਿਰਾ || EXT2_DESC_ਮਾਪ(fs->super) < EXT2_MIN_DESC_ਮਾਪ_64_ਦੋਆਧਾਰੀ_ਅੰਕ)
		ਵਾਪਸ;

	/* Swap the 64-bit parts */
	gdp4->bg_ਖੰਡ_bitmap_hi = ext2fs_swab32(gdp4->bg_ਖੰਡ_bitmap_hi);
	gdp4->bg_inode_bitmap_hi = ext2fs_swab32(gdp4->bg_inode_bitmap_hi);
	gdp4->bg_inode_ਸਾਰਣੀ_hi = ext2fs_swab32(gdp4->bg_inode_ਸਾਰਣੀ_hi);
	gdp4->bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_hi =
		ext2fs_swab16(gdp4->bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_hi);
	gdp4->bg_free_inodes_ਗਿਣਤੀ_hi =
		ext2fs_swab16(gdp4->bg_free_inodes_ਗਿਣਤੀ_hi);
	gdp4->bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ_hi =
		ext2fs_swab16(gdp4->bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ_hi);
	gdp4->bg_itable_ਅਣਵਰਤਿਆ_hi = ext2fs_swab16(gdp4->bg_itable_ਅਣਵਰਤਿਆ_hi);
	gdp4->bg_exclude_bitmap_hi = ext2fs_swab16(gdp4->bg_exclude_bitmap_hi);
	gdp4->bg_ਖੰਡ_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_ਖੰਡ_bitmap_csum_hi);
	gdp4->bg_inode_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_inode_bitmap_csum_hi);
	EXT2FS_BUILD_BUG_ON(ਆਕਾਰ(gdp4->bg_reserved) != ਆਕਾਰ(__u32));
}

ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc(ਬਣਤਰ ext2_ਸਮੂਹ_desc *gdp)
{
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(0, gdp);
}


ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr_ਸਿਰਲੇਖ(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ,
				 ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *from_ਸਿਰਲੇਖ)
{
	ਪੂਰਨ n;

	ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ->h_magic    = ext2fs_swab32(from_ਸਿਰਲੇਖ->h_magic);
	ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ->h_ਖੰਡਾਂ   = ext2fs_swab32(from_ਸਿਰਲੇਖ->h_ਖੰਡਾਂ);
	ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ->h_refcount = ext2fs_swab32(from_ਸਿਰਲੇਖ->h_refcount);
	ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ->h_hash     = ext2fs_swab32(from_ਸਿਰਲੇਖ->h_hash);
	ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ->h_checksum = ext2fs_swab32(from_ਸਿਰਲੇਖ->h_checksum);
	ਲਈ (n = 0; n < 3; n++)
		ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ->h_reserved[n] =
			ext2fs_swab32(from_ਸਿਰਲੇਖ->h_reserved[n]);
}

ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr_ਇੰਦਰਾਜ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ,
				ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *from_ਇੰਦਰਾਜ)
{
	ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ->e_ਮੁੱਲ_offs  = ext2fs_swab16(from_ਇੰਦਰਾਜ->e_ਮੁੱਲ_offs);
	ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum  = ext2fs_swab32(from_ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum);
	ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ  = ext2fs_swab32(from_ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ);
	ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ->e_hash	= ext2fs_swab32(from_ਇੰਦਰਾਜ->e_hash);
}

ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr(ਅੱਖਰ *ਮੰਜ਼ਿਲ, ਅੱਖਰ *from, ਪੂਰਨ bufsize, ਪੂਰਨ has_ਸਿਰਲੇਖ)
{
	ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *from_ਸਿਰਲੇਖ =
		(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *)from;
	ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ =
		(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *)ਮੰਜ਼ਿਲ;
	ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *from_ਇੰਦਰਾਜ, *ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ;
	ਅੱਖਰ *from_ਸਿਰਾ = (ਅੱਖਰ *)from_ਸਿਰਲੇਖ + bufsize;

	ਜੇ (ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ != from_ਸਿਰਲੇਖ)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ, from_ਸਿਰਲੇਖ, bufsize);

	ਜੇ (has_ਸਿਰਲੇਖ) {
		ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr_ਸਿਰਲੇਖ(ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ, from_ਸਿਰਲੇਖ);

		from_ਇੰਦਰਾਜ = (ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *)(from_ਸਿਰਲੇਖ+1);
		ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ   = (ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *)(ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ+1);
	} ਨਹੀਂਤਾਂ {
		from_ਇੰਦਰਾਜ = (ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *)from_ਸਿਰਲੇਖ;
		ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ   = (ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *)ਮੰਜ਼ਿਲ_ਸਿਰਲੇਖ;
	}

	ਜਦੋਂ ((ਅੱਖਰ *)from_ਇੰਦਰਾਜ < from_ਸਿਰਾ &&
	       (ਅੱਖਰ *)EXT2_EXT_ATTR_ਅਗਲਾ(from_ਇੰਦਰਾਜ) <= from_ਸਿਰਾ &&
	       *(__u32 *)from_ਇੰਦਰਾਜ) {
		ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr_ਇੰਦਰਾਜ(ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ, from_ਇੰਦਰਾਜ);
		from_ਇੰਦਰਾਜ = EXT2_EXT_ATTR_ਅਗਲਾ(from_ਇੰਦਰਾਜ);
		ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ   = EXT2_EXT_ATTR_ਅਗਲਾ(ਮੰਜ਼ਿਲ_ਇੰਦਰਾਜ);
	}
}

ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_inode_ਭਰਿਆ(ext2_filsys fs, ਬਣਤਰ ext2_inode_large *t,
			    ਬਣਤਰ ext2_inode_large *f, ਪੂਰਨ hostorder,
			    ਪੂਰਨ bufsize)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ i, extra_isize, attr_magic;
	ਪੂਰਨ has_extents = 0, has_inline_ਅੰਕੜੇ = 0, islnk = 0, fast_symlink = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inode_ਮਾਪ;
	__u32 *eaf, *eat;

	/*
	 * Note that t and f may point to the same address. That's why
	 * if (hostorder) condition is executed before swab calls and
	 * if (!hostorder) afterwards.
	 */
	ਜੇ (hostorder) {
		islnk = LINUX_S_ISLNK(f->i_ਬਹੁਲਕ);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(f));
		has_extents = (f->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_ਅੰਕੜੇ = (f->i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL) != 0;
	}

	t->i_ਬਹੁਲਕ = ext2fs_swab16(f->i_ਬਹੁਲਕ);
	t->i_uid = ext2fs_swab16(f->i_uid);
	t->i_ਮਾਪ = ext2fs_swab32(f->i_ਮਾਪ);
	t->i_atime = ext2fs_swab32(f->i_atime);
	t->i_ctime = ext2fs_swab32(f->i_ctime);
	t->i_mtime = ext2fs_swab32(f->i_mtime);
	t->i_dtime = ext2fs_swab32(f->i_dtime);
	t->i_gid = ext2fs_swab16(f->i_gid);
	t->i_links_ਗਿਣਤੀ = ext2fs_swab16(f->i_links_ਗਿਣਤੀ);
	t->i_file_acl = ext2fs_swab32(f->i_file_acl);
	t->i_ਖੰਡਾਂ = ext2fs_swab32(f->i_ਖੰਡਾਂ);
	t->i_flags = ext2fs_swab32(f->i_flags);
	t->i_ਮਾਪ_ਉੱਚਾ = ext2fs_swab32(f->i_ਮਾਪ_ਉੱਚਾ);

	ਜੇ (!hostorder) {
		islnk = LINUX_S_ISLNK(t->i_ਬਹੁਲਕ);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(t));
		has_extents = (t->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_ਅੰਕੜੇ = (t->i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL) != 0;
	}

	/*
	 * Extent data and inline data are swapped on access, not here
	 */
	ਜੇ (!has_extents && !has_inline_ਅੰਕੜੇ && (!islnk || !fast_symlink)) {
		ਲਈ (i = 0; i < EXT2_N_ਖੰਡਾਂ; i++)
			t->i_ਖੰਡ[i] = ext2fs_swab32(f->i_ਖੰਡ[i]);
	} ਨਹੀਂਤਾਂ ਜੇ (t != f) {
		ਲਈ (i = 0; i < EXT2_N_ਖੰਡਾਂ; i++)
			t->i_ਖੰਡ[i] = f->i_ਖੰਡ[i];
	}
	t->i_generation = ext2fs_swab32(f->i_generation);
	t->i_faddr = ext2fs_swab32(f->i_faddr);

	ਚੋਣ (fs->super->s_creator_os) {
	ਹਾਲਤ EXT2_OS_LINUX:
		t->osd1.linux1.l_i_ਸੰਸਕਰਣ =
			ext2fs_swab32(f->osd1.linux1.l_i_ਸੰਸਕਰਣ);
		t->osd2.linux2.l_i_ਖੰਡਾਂ_hi =
			ext2fs_swab16(f->osd2.linux2.l_i_ਖੰਡਾਂ_hi);
		t->osd2.linux2.l_i_file_acl_ਉੱਚਾ =
			ext2fs_swab16(f->osd2.linux2.l_i_file_acl_ਉੱਚਾ);
		t->osd2.linux2.l_i_uid_ਉੱਚਾ =
		  ext2fs_swab16 (f->osd2.linux2.l_i_uid_ਉੱਚਾ);
		t->osd2.linux2.l_i_gid_ਉੱਚਾ =
		  ext2fs_swab16 (f->osd2.linux2.l_i_gid_ਉੱਚਾ);
		t->osd2.linux2.l_i_checksum_lo =
			ext2fs_swab16(f->osd2.linux2.l_i_checksum_lo);
		ਰੋਕੋ;
	ਹਾਲਤ EXT2_OS_HURD:
		t->osd1.hurd1.h_i_translator =
		  ext2fs_swab32 (f->osd1.hurd1.h_i_translator);
		t->osd2.hurd2.h_i_frag = f->osd2.hurd2.h_i_frag;
		t->osd2.hurd2.h_i_fsize = f->osd2.hurd2.h_i_fsize;
		t->osd2.hurd2.h_i_ਬਹੁਲਕ_ਉੱਚਾ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_ਬਹੁਲਕ_ਉੱਚਾ);
		t->osd2.hurd2.h_i_uid_ਉੱਚਾ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_uid_ਉੱਚਾ);
		t->osd2.hurd2.h_i_gid_ਉੱਚਾ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_gid_ਉੱਚਾ);
		t->osd2.hurd2.h_i_author =
		  ext2fs_swab32 (f->osd2.hurd2.h_i_author);
		ਰੋਕੋ;
	ਮੂਲ:
		ਰੋਕੋ;
	}

	ਜੇ (bufsize < (ਪੂਰਨ) (ਆਕਾਰ(ਬਣਤਰ ext2_inode) + ਆਕਾਰ(__u16)))
		ਵਾਪਸ; /* no i_extra_isize field */

	ਜੇ (hostorder)
		extra_isize = f->i_extra_isize;
	t->i_extra_isize = ext2fs_swab16(f->i_extra_isize);
	ਜੇ (!hostorder)
		extra_isize = t->i_extra_isize;
	ਜੇ (extra_isize > EXT2_INODE_ਮਾਪ(fs->super) -
				ਆਕਾਰ(ਬਣਤਰ ext2_inode)) {
		/* this is error case: i_extra_size is too large */
		ਵਾਪਸ;
	}
	ਜੇ (extra_isize & 3)
		ਵਾਪਸ;		/* Illegal inode extra_isize */

	inode_ਮਾਪ = EXT2_GOOD_OLD_INODE_ਮਾਪ + extra_isize;
	ਜੇ (ext2fs_inode_includes(inode_ਮਾਪ, i_checksum_hi))
		t->i_checksum_hi = ext2fs_swab16(f->i_checksum_hi);
	ਜੇ (ext2fs_inode_includes(inode_ਮਾਪ, i_ctime_extra))
		t->i_ctime_extra = ext2fs_swab32(f->i_ctime_extra);
	ਜੇ (ext2fs_inode_includes(inode_ਮਾਪ, i_mtime_extra))
		t->i_mtime_extra = ext2fs_swab32(f->i_mtime_extra);
	ਜੇ (ext2fs_inode_includes(inode_ਮਾਪ, i_atime_extra))
		t->i_atime_extra = ext2fs_swab32(f->i_atime_extra);
	ਜੇ (ext2fs_inode_includes(inode_ਮਾਪ, i_crtime))
		t->i_crtime = ext2fs_swab32(f->i_crtime);
	ਜੇ (ext2fs_inode_includes(inode_ਮਾਪ, i_crtime_extra))
		t->i_crtime_extra = ext2fs_swab32(f->i_crtime_extra);
	ਜੇ (ext2fs_inode_includes(inode_ਮਾਪ, i_ਸੰਸਕਰਣ_hi))
		t->i_ਸੰਸਕਰਣ_hi = ext2fs_swab32(f->i_ਸੰਸਕਰਣ_hi);
	ਜੇ (ext2fs_inode_includes(inode_ਮਾਪ, i_projid))
                t->i_projid = ext2fs_swab32(f->i_projid);
	/* catch new static fields added after i_projid */
	EXT2FS_BUILD_BUG_ON(ਆਕਾਰ(ਬਣਤਰ ext2_inode_large) != 160);

	i = ਆਕਾਰ(ਬਣਤਰ ext2_inode) + extra_isize + ਆਕਾਰ(__u32);
	ਜੇ (bufsize < (ਪੂਰਨ) i)
		ਵਾਪਸ; /* no space for EA magic */

	eaf = (__u32 *) (((ਅੱਖਰ *) f) + ਆਕਾਰ(ਬਣਤਰ ext2_inode) +
					extra_isize);

	attr_magic = *eaf;
	ਜੇ (!hostorder)
		attr_magic = ext2fs_swab32(attr_magic);

	ਜੇ (attr_magic != EXT2_EXT_ATTR_MAGIC)
		ਵਾਪਸ; /* it seems no magic here */

	eat = (__u32 *) (((ਅੱਖਰ *) t) + ਆਕਾਰ(ਬਣਤਰ ext2_inode) +
					extra_isize);
	*eat = ext2fs_swab32(*eaf);

	/* convert EA(s) */
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr((ਅੱਖਰ *) (eat + 1), (ਅੱਖਰ *) (eaf + 1),
			     bufsize - ਆਕਾਰ(ਬਣਤਰ ext2_inode) -
			     extra_isize - ਆਕਾਰ(__u32), 0);

}

ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_inode(ext2_filsys fs, ਬਣਤਰ ext2_inode *t,
		       ਬਣਤਰ ext2_inode *f, ਪੂਰਨ hostorder)
{
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_inode_ਭਰਿਆ(fs, (ਬਣਤਰ ext2_inode_large *) t,
				(ਬਣਤਰ ext2_inode_large *) f, hostorder,
				ਆਕਾਰ(ਬਣਤਰ ext2_inode));
}

ਖਾਲੀ ext2fs_ਅਦਲਾ_ਬਦਲੀ_mmp(ਬਣਤਰ mmp_struct *mmp)
{
	mmp->mmp_magic = ext2fs_swab32(mmp->mmp_magic);
	mmp->mmp_seq = ext2fs_swab32(mmp->mmp_seq);
	mmp->mmp_ਸਮਾਂ = ext2fs_swab64(mmp->mmp_ਸਮਾਂ);
	mmp->mmp_check_interval = ext2fs_swab16(mmp->mmp_check_interval);
	mmp->mmp_checksum = ext2fs_swab32(mmp->mmp_checksum);
}

errcode_t ext2fs_dirent_swab_ਵਿੱਚ(ext2_filsys fs, ਅੱਖਰ *buf, ਪੂਰਨ flags)
{
	ਵਾਪਸ ext2fs_dirent_swab_in2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, ਅੱਖਰ *buf,
				 size_t ਮਾਪ, ਪੂਰਨ flags)
{
	errcode_t	retval;
	ਅੱਖਰ		*p, *ਸਿਰਾ;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	name_len, rec_len, ਖੱਬਾ;

	p = (ਅੱਖਰ *) buf;
	ਸਿਰਾ = (ਅੱਖਰ *) buf + ਮਾਪ;
	ਖੱਬਾ = ਮਾਪ;
	ਜਦੋਂ (p < ਸਿਰਾ-8) {
		dirent = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) p;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		name_len = dirent->name_len;
		ਜੇ (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਜੇ ((rec_len < 8) || (rec_len % 4)) {
			rec_len = 8;
			ਜੇ (!(fs->flags & EXT2_FLAG_IGNORE_ਅਦਲਾ_ਬਦਲੀ_DIRENT))
				ਵਾਪਸ EXT2_ET_DIR_CORRUPTED;
		} ਨਹੀਂਤਾਂ ਜੇ (((name_len & 0xFF) + 8) > rec_len)
			ਜੇ (!(fs->flags & EXT2_FLAG_IGNORE_ਅਦਲਾ_ਬਦਲੀ_DIRENT))
				ਵਾਪਸ EXT2_ET_DIR_CORRUPTED;
		ਜੇ (rec_len > ਖੱਬਾ)
			ਜੇ (!(fs->flags & EXT2_FLAG_IGNORE_ਅਦਲਾ_ਬਦਲੀ_DIRENT))
				ਵਾਪਸ EXT2_ET_DIR_CORRUPTED;
		ਖੱਬਾ -= rec_len;
		p += rec_len;
	}

	ਵਾਪਸ 0;
}

errcode_t ext2fs_dirent_swab_ਬਾਹਰਲਾ(ext2_filsys fs, ਅੱਖਰ *buf, ਪੂਰਨ flags)
{
	ਵਾਪਸ ext2fs_dirent_swab_out2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, ਅੱਖਰ *buf,
				  size_t ਮਾਪ, ਪੂਰਨ flags)
{
	errcode_t	retval;
	ਅੱਖਰ		*p, *ਸਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	rec_len;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent;

	p = buf;
	ਸਿਰਾ = buf + ਮਾਪ;
	ਜਦੋਂ (p < ਸਿਰਾ) {
		dirent = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) p;
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਜੇ ((rec_len < 8) ||
		    (rec_len % 4)) {
			ext2fs_free_mem(&buf);
			ਵਾਪਸ EXT2_ET_DIR_CORRUPTED;
		}
		p += rec_len;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		ਜੇ (rec_len > ਮਾਪ)
			ਵਾਪਸ EXT2_ET_DIR_CORRUPTED;
		ਮਾਪ -= rec_len;

		ਜੇ (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
	}

	ਵਾਪਸ 0;
}
