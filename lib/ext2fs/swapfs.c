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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_ext_attr.h>

ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_super(ರಚನೆ ext2_super_ಖಂಡ * sb)
{
	ಪೂರ್ಣಾಂಕ i;

	sb->s_inodes_ಎಣಿಕೆ = ext2fs_swab32(sb->s_inodes_ಎಣಿಕೆ);
	sb->s_ಖಂಡಗಳು_ಎಣಿಕೆ = ext2fs_swab32(sb->s_ಖಂಡಗಳು_ಎಣಿಕೆ);
	sb->s_r_ಖಂಡಗಳು_ಎಣಿಕೆ = ext2fs_swab32(sb->s_r_ಖಂಡಗಳು_ಎಣಿಕೆ);
	sb->s_free_ಖಂಡಗಳು_ಎಣಿಕೆ = ext2fs_swab32(sb->s_free_ಖಂಡಗಳು_ಎಣಿಕೆ);
	sb->s_free_inodes_ಎಣಿಕೆ = ext2fs_swab32(sb->s_free_inodes_ಎಣಿಕೆ);
	sb->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ = ext2fs_swab32(sb->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ);
	sb->s_log_ಖಂಡ_ಗಾತ್ರ = ext2fs_swab32(sb->s_log_ಖಂಡ_ಗಾತ್ರ);
	sb->s_log_cluster_ಗಾತ್ರ = ext2fs_swab32(sb->s_log_cluster_ಗಾತ್ರ);
	sb->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು = ext2fs_swab32(sb->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು);
	sb->s_clusters_ಪ್ರತಿ_ಗುಂಪು = ext2fs_swab32(sb->s_clusters_ಪ್ರತಿ_ಗುಂಪು);
	sb->s_inodes_ಪ್ರತಿ_ಗುಂಪು = ext2fs_swab32(sb->s_inodes_ಪ್ರತಿ_ಗುಂಪು);
	sb->s_mtime = ext2fs_swab32(sb->s_mtime);
	sb->s_wtime = ext2fs_swab32(sb->s_wtime);
	sb->s_mnt_ಎಣಿಕೆ = ext2fs_swab16(sb->s_mnt_ಎಣಿಕೆ);
	sb->s_max_mnt_ಎಣಿಕೆ = ext2fs_swab16(sb->s_max_mnt_ಎಣಿಕೆ);
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
	sb->s_ಮೊದಲ_ino = ext2fs_swab32(sb->s_ಮೊದಲ_ino);
	sb->s_inode_ಗಾತ್ರ = ext2fs_swab16(sb->s_inode_ಗಾತ್ರ);
	sb->s_ಖಂಡ_ಗುಂಪು_nr = ext2fs_swab16(sb->s_ಖಂಡ_ಗುಂಪು_nr);
	sb->s_feature_compat = ext2fs_swab32(sb->s_feature_compat);
	sb->s_feature_incompat = ext2fs_swab32(sb->s_feature_incompat);
	sb->s_feature_ro_compat = ext2fs_swab32(sb->s_feature_ro_compat);
	/* sb->s_uuid is __u8 and does not need swabbing */
	/* sb->s_volume_name is char and does not need swabbing */
	/* sb->s_last_mounted is char and does not need swabbing */
	sb->s_algorithm_usage_bitmap = ext2fs_swab32(sb->s_algorithm_usage_bitmap);
	/* sb->s_prealloc_blocks is __u8 and does not need swabbing */
	/* sb->s_prealloc_dir_blocks is __u8 and does not need swabbing */
	sb->s_reserved_gdt_ಖಂಡಗಳು = ext2fs_swab16(sb->s_reserved_gdt_ಖಂಡಗಳು);
	/* sb->s_journal_uuid is __u8 and does not need swabbing */
	sb->s_journal_inum = ext2fs_swab32(sb->s_journal_inum);
	sb->s_journal_dev = ext2fs_swab32(sb->s_journal_dev);
	sb->s_ಕೊನೆಯ_orphan = ext2fs_swab32(sb->s_ಕೊನೆಯ_orphan);
	ಗಾಗಿ (i = 0; i < 4; i++)
		sb->s_hash_ಬೀಜ[i] = ext2fs_swab32(sb->s_hash_ಬೀಜ[i]);
	/* sb->s_def_hash_version is __u8 and does not need swabbing */
	/* sb->s_jnl_backup_type is __u8 and does not need swabbing */
	sb->s_desc_ಗಾತ್ರ = ext2fs_swab16(sb->s_desc_ಗಾತ್ರ);
	sb->s_default_mount_opts = ext2fs_swab32(sb->s_default_mount_opts);
	sb->s_ಮೊದಲ_meta_bg = ext2fs_swab32(sb->s_ಮೊದಲ_meta_bg);
	sb->s_mkfs_ಸಮಯ = ext2fs_swab32(sb->s_mkfs_ಸಮಯ);
	/* if journal backup is for a valid extent-based journal... */
	ಆದರೆ (ext2fs_extent_ಶೀರ್ಷಿಕೆ_verify(sb->s_jnl_ಖಂಡಗಳು,
					ಗಾತ್ರ(sb->s_jnl_ಖಂಡಗಳು)) == 0) {
		/* ... swap only the journal i_size and i_size_high,
		 * and the extent data is not swapped on read */
		i = 15;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* direct/indirect journal: swap it all */
		i = 0;
	}
	ಗಾಗಿ (; i < 17; i++)
		sb->s_jnl_ಖಂಡಗಳು[i] = ext2fs_swab32(sb->s_jnl_ಖಂಡಗಳು[i]);
	sb->s_ಖಂಡಗಳು_ಎಣಿಕೆ_hi = ext2fs_swab32(sb->s_ಖಂಡಗಳು_ಎಣಿಕೆ_hi);
	sb->s_r_ಖಂಡಗಳು_ಎಣಿಕೆ_hi = ext2fs_swab32(sb->s_r_ಖಂಡಗಳು_ಎಣಿಕೆ_hi);
	sb->s_free_ಖಂಡಗಳು_hi = ext2fs_swab32(sb->s_free_ಖಂಡಗಳು_hi);
	sb->s_min_extra_isize = ext2fs_swab16(sb->s_min_extra_isize);
	sb->s_want_extra_isize = ext2fs_swab16(sb->s_want_extra_isize);
	sb->s_flags = ext2fs_swab32(sb->s_flags);
	sb->s_raid_stride = ext2fs_swab16(sb->s_raid_stride);
	sb->s_mmp_update_interval = ext2fs_swab16(sb->s_mmp_update_interval);
	sb->s_mmp_ಖಂಡ = ext2fs_swab64(sb->s_mmp_ಖಂಡ);
	sb->s_raid_stripe_ಅಗಲ = ext2fs_swab32(sb->s_raid_stripe_ಅಗಲ);
	/* sb->s_log_groups_per_flex is __u8 and does not need swabbing */
	/* sb->s_checksum_type is __u8 and does not need swabbing */
	/* sb->s_encryption_level is __u8 and does not need swabbing */
	/* sb->s_reserved_pad is __u8 and does not need swabbing */
	sb->s_kbytes_written = ext2fs_swab64(sb->s_kbytes_written);
	sb->s_snapshot_inum = ext2fs_swab32(sb->s_snapshot_inum);
	sb->s_snapshot_id = ext2fs_swab32(sb->s_snapshot_id);
	sb->s_snapshot_r_ಖಂಡಗಳು_ಎಣಿಕೆ =
		ext2fs_swab64(sb->s_snapshot_r_ಖಂಡಗಳು_ಎಣಿಕೆ);
	sb->s_snapshot_list = ext2fs_swab32(sb->s_snapshot_list);
	sb->s_error_ಎಣಿಕೆ = ext2fs_swab32(sb->s_error_ಎಣಿಕೆ);
	sb->s_ಮೊದಲ_error_ಸಮಯ = ext2fs_swab32(sb->s_ಮೊದಲ_error_ಸಮಯ);
	sb->s_ಮೊದಲ_error_ino = ext2fs_swab32(sb->s_ಮೊದಲ_error_ino);
	sb->s_ಮೊದಲ_error_ಖಂಡ = ext2fs_swab64(sb->s_ಮೊದಲ_error_ಖಂಡ);
	/* sb->s_first_error_func is __u8 and does not need swabbing */
	sb->s_ಕೊನೆಯ_error_ಸಮಯ = ext2fs_swab32(sb->s_ಕೊನೆಯ_error_ಸಮಯ);
	sb->s_ಕೊನೆಯ_error_ino = ext2fs_swab32(sb->s_ಕೊನೆಯ_error_ino);
	sb->s_ಕೊನೆಯ_error_ಖಂಡ = ext2fs_swab64(sb->s_ಕೊನೆಯ_error_ಖಂಡ);
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
	sb->s_checksum_ಬೀಜ = ext2fs_swab32(sb->s_checksum_ಬೀಜ);
	/* s_*_time_hi are __u8 and does not need swabbing */
	sb->s_encoding = ext2fs_swab16(sb->s_encoding);
	sb->s_encoding_flags = ext2fs_swab16(sb->s_encoding_flags);
	sb->s_orphan_file_inum = ext2fs_swab32(sb->s_orphan_file_inum);
	/* catch when new fields are used from s_reserved */
	EXT2FS_BUILD_BUG_ON(ಗಾತ್ರ(sb->s_reserved) != 94 * ಗಾತ್ರ(__le32));
	sb->s_checksum = ext2fs_swab32(sb->s_checksum);
}

ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(ext2_filsys fs, ರಚನೆ ext2_ಗುಂಪು_desc *gdp)
{
	ರಚನೆ ext4_ಗುಂಪು_desc *gdp4 = (ರಚನೆ ext4_ಗುಂಪು_desc *)gdp;

	/* Do the 32-bit parts first */
	gdp->bg_ಖಂಡ_bitmap = ext2fs_swab32(gdp->bg_ಖಂಡ_bitmap);
	gdp->bg_inode_bitmap = ext2fs_swab32(gdp->bg_inode_bitmap);
	gdp->bg_inode_ಕೋಷ್ಟಕ = ext2fs_swab32(gdp->bg_inode_ಕೋಷ್ಟಕ);
	gdp->bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ = ext2fs_swab16(gdp->bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ);
	gdp->bg_free_inodes_ಎಣಿಕೆ = ext2fs_swab16(gdp->bg_free_inodes_ಎಣಿಕೆ);
	gdp->bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ = ext2fs_swab16(gdp->bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ);
	gdp->bg_flags = ext2fs_swab16(gdp->bg_flags);
	gdp->bg_exclude_bitmap_lo = ext2fs_swab32(gdp->bg_exclude_bitmap_lo);
	gdp->bg_ಖಂಡ_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_ಖಂಡ_bitmap_csum_lo);
	gdp->bg_inode_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_inode_bitmap_csum_lo);
	gdp->bg_itable_ಬಳಸದ = ext2fs_swab16(gdp->bg_itable_ಬಳಸದ);
	gdp->bg_checksum = ext2fs_swab16(gdp->bg_checksum);
	/* If we're 32-bit, we're done */
	ಆದರೆ (fs == ಖಾಲಿ || EXT2_DESC_ಗಾತ್ರ(fs->super) < EXT2_MIN_DESC_ಗಾತ್ರ_64_ಬಿಟ್)
		ಹಿಂತಿರುಗಿ;

	/* Swap the 64-bit parts */
	gdp4->bg_ಖಂಡ_bitmap_hi = ext2fs_swab32(gdp4->bg_ಖಂಡ_bitmap_hi);
	gdp4->bg_inode_bitmap_hi = ext2fs_swab32(gdp4->bg_inode_bitmap_hi);
	gdp4->bg_inode_ಕೋಷ್ಟಕ_hi = ext2fs_swab32(gdp4->bg_inode_ಕೋಷ್ಟಕ_hi);
	gdp4->bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_hi =
		ext2fs_swab16(gdp4->bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_hi);
	gdp4->bg_free_inodes_ಎಣಿಕೆ_hi =
		ext2fs_swab16(gdp4->bg_free_inodes_ಎಣಿಕೆ_hi);
	gdp4->bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ_hi =
		ext2fs_swab16(gdp4->bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ_hi);
	gdp4->bg_itable_ಬಳಸದ_hi = ext2fs_swab16(gdp4->bg_itable_ಬಳಸದ_hi);
	gdp4->bg_exclude_bitmap_hi = ext2fs_swab16(gdp4->bg_exclude_bitmap_hi);
	gdp4->bg_ಖಂಡ_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_ಖಂಡ_bitmap_csum_hi);
	gdp4->bg_inode_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_inode_bitmap_csum_hi);
	EXT2FS_BUILD_BUG_ON(ಗಾತ್ರ(gdp4->bg_reserved) != ಗಾತ್ರ(__u32));
}

ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc(ರಚನೆ ext2_ಗುಂಪು_desc *gdp)
{
	ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(0, gdp);
}


ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_ext_attr_ಶೀರ್ಷಿಕೆ(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ,
				 ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *from_ಶೀರ್ಷಿಕೆ)
{
	ಪೂರ್ಣಾಂಕ n;

	ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ->h_magic    = ext2fs_swab32(from_ಶೀರ್ಷಿಕೆ->h_magic);
	ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ->h_ಖಂಡಗಳು   = ext2fs_swab32(from_ಶೀರ್ಷಿಕೆ->h_ಖಂಡಗಳು);
	ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ->h_refcount = ext2fs_swab32(from_ಶೀರ್ಷಿಕೆ->h_refcount);
	ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ->h_hash     = ext2fs_swab32(from_ಶೀರ್ಷಿಕೆ->h_hash);
	ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ->h_checksum = ext2fs_swab32(from_ಶೀರ್ಷಿಕೆ->h_checksum);
	ಗಾಗಿ (n = 0; n < 3; n++)
		ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ->h_reserved[n] =
			ext2fs_swab32(from_ಶೀರ್ಷಿಕೆ->h_reserved[n]);
}

ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_ext_attr_ನಮೂದು(ರಚನೆ ext2_ext_attr_ನಮೂದು *ಗಮ್ಯ_ನಮೂದು,
				ರಚನೆ ext2_ext_attr_ನಮೂದು *from_ನಮೂದು)
{
	ಗಮ್ಯ_ನಮೂದು->e_ಮೌಲ್ಯ_offs  = ext2fs_swab16(from_ನಮೂದು->e_ಮೌಲ್ಯ_offs);
	ಗಮ್ಯ_ನಮೂದು->e_ಮೌಲ್ಯ_inum  = ext2fs_swab32(from_ನಮೂದು->e_ಮೌಲ್ಯ_inum);
	ಗಮ್ಯ_ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ  = ext2fs_swab32(from_ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ);
	ಗಮ್ಯ_ನಮೂದು->e_hash	= ext2fs_swab32(from_ನಮೂದು->e_hash);
}

ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_ext_attr(ಅಕ್ಷರ *ಗಮ್ಯ, ಅಕ್ಷರ *from, ಪೂರ್ಣಾಂಕ bufsize, ಪೂರ್ಣಾಂಕ has_ಶೀರ್ಷಿಕೆ)
{
	ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *from_ಶೀರ್ಷಿಕೆ =
		(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *)from;
	ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ =
		(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *)ಗಮ್ಯ;
	ರಚನೆ ext2_ext_attr_ನಮೂದು *from_ನಮೂದು, *ಗಮ್ಯ_ನಮೂದು;
	ಅಕ್ಷರ *from_ಕೊನೆ = (ಅಕ್ಷರ *)from_ಶೀರ್ಷಿಕೆ + bufsize;

	ಆದರೆ (ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ != from_ಶೀರ್ಷಿಕೆ)
		ಸ್ಮರಣೆ_ನಕಲು(ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ, from_ಶೀರ್ಷಿಕೆ, bufsize);

	ಆದರೆ (has_ಶೀರ್ಷಿಕೆ) {
		ext2fs_ಅದಲುಬದಲು_ext_attr_ಶೀರ್ಷಿಕೆ(ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ, from_ಶೀರ್ಷಿಕೆ);

		from_ನಮೂದು = (ರಚನೆ ext2_ext_attr_ನಮೂದು *)(from_ಶೀರ್ಷಿಕೆ+1);
		ಗಮ್ಯ_ನಮೂದು   = (ರಚನೆ ext2_ext_attr_ನಮೂದು *)(ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ+1);
	} ಇಲ್ಲದಿದ್ದರೆ {
		from_ನಮೂದು = (ರಚನೆ ext2_ext_attr_ನಮೂದು *)from_ಶೀರ್ಷಿಕೆ;
		ಗಮ್ಯ_ನಮೂದು   = (ರಚನೆ ext2_ext_attr_ನಮೂದು *)ಗಮ್ಯ_ಶೀರ್ಷಿಕೆ;
	}

	ಆಗ ((ಅಕ್ಷರ *)from_ನಮೂದು < from_ಕೊನೆ &&
	       (ಅಕ್ಷರ *)EXT2_EXT_ATTR_ಮುಂದಿನ(from_ನಮೂದು) <= from_ಕೊನೆ &&
	       *(__u32 *)from_ನಮೂದು) {
		ext2fs_ಅದಲುಬದಲು_ext_attr_ನಮೂದು(ಗಮ್ಯ_ನಮೂದು, from_ನಮೂದು);
		from_ನಮೂದು = EXT2_EXT_ATTR_ಮುಂದಿನ(from_ನಮೂದು);
		ಗಮ್ಯ_ನಮೂದು   = EXT2_EXT_ATTR_ಮುಂದಿನ(ಗಮ್ಯ_ನಮೂದು);
	}
}

ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_inode_ತುಂಬಿದ(ext2_filsys fs, ರಚನೆ ext2_inode_large *t,
			    ರಚನೆ ext2_inode_large *f, ಪೂರ್ಣಾಂಕ hostorder,
			    ಪೂರ್ಣಾಂಕ bufsize)
{
	ಚಿಹ್ನೆರಹಿತ i, extra_isize, attr_magic;
	ಪೂರ್ಣಾಂಕ has_extents = 0, has_inline_ದತ್ತಾಂಶ = 0, islnk = 0, fast_symlink = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ inode_ಗಾತ್ರ;
	__u32 *eaf, *eat;

	/*
	 * Note that t and f may point to the same address. That's why
	 * if (hostorder) condition is executed before swab calls and
	 * if (!hostorder) afterwards.
	 */
	ಆದರೆ (hostorder) {
		islnk = LINUX_S_ISLNK(f->i_ಬಹುಲಕ);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(f));
		has_extents = (f->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_ದತ್ತಾಂಶ = (f->i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL) != 0;
	}

	t->i_ಬಹುಲಕ = ext2fs_swab16(f->i_ಬಹುಲಕ);
	t->i_uid = ext2fs_swab16(f->i_uid);
	t->i_ಗಾತ್ರ = ext2fs_swab32(f->i_ಗಾತ್ರ);
	t->i_atime = ext2fs_swab32(f->i_atime);
	t->i_ctime = ext2fs_swab32(f->i_ctime);
	t->i_mtime = ext2fs_swab32(f->i_mtime);
	t->i_dtime = ext2fs_swab32(f->i_dtime);
	t->i_gid = ext2fs_swab16(f->i_gid);
	t->i_links_ಎಣಿಕೆ = ext2fs_swab16(f->i_links_ಎಣಿಕೆ);
	t->i_file_acl = ext2fs_swab32(f->i_file_acl);
	t->i_ಖಂಡಗಳು = ext2fs_swab32(f->i_ಖಂಡಗಳು);
	t->i_flags = ext2fs_swab32(f->i_flags);
	t->i_ಗಾತ್ರ_ಮೇಲಿನ = ext2fs_swab32(f->i_ಗಾತ್ರ_ಮೇಲಿನ);

	ಆದರೆ (!hostorder) {
		islnk = LINUX_S_ISLNK(t->i_ಬಹುಲಕ);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(t));
		has_extents = (t->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_ದತ್ತಾಂಶ = (t->i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL) != 0;
	}

	/*
	 * Extent data and inline data are swapped on access, not here
	 */
	ಆದರೆ (!has_extents && !has_inline_ದತ್ತಾಂಶ && (!islnk || !fast_symlink)) {
		ಗಾಗಿ (i = 0; i < EXT2_N_ಖಂಡಗಳು; i++)
			t->i_ಖಂಡ[i] = ext2fs_swab32(f->i_ಖಂಡ[i]);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (t != f) {
		ಗಾಗಿ (i = 0; i < EXT2_N_ಖಂಡಗಳು; i++)
			t->i_ಖಂಡ[i] = f->i_ಖಂಡ[i];
	}
	t->i_generation = ext2fs_swab32(f->i_generation);
	t->i_faddr = ext2fs_swab32(f->i_faddr);

	ಆಯ್ಕೆ (fs->super->s_creator_os) {
	ಸ್ಥಿತಿ EXT2_OS_LINUX:
		t->osd1.linux1.l_i_ಆವೃತ್ತಿ =
			ext2fs_swab32(f->osd1.linux1.l_i_ಆವೃತ್ತಿ);
		t->osd2.linux2.l_i_ಖಂಡಗಳು_hi =
			ext2fs_swab16(f->osd2.linux2.l_i_ಖಂಡಗಳು_hi);
		t->osd2.linux2.l_i_file_acl_ಮೇಲಿನ =
			ext2fs_swab16(f->osd2.linux2.l_i_file_acl_ಮೇಲಿನ);
		t->osd2.linux2.l_i_uid_ಮೇಲಿನ =
		  ext2fs_swab16 (f->osd2.linux2.l_i_uid_ಮೇಲಿನ);
		t->osd2.linux2.l_i_gid_ಮೇಲಿನ =
		  ext2fs_swab16 (f->osd2.linux2.l_i_gid_ಮೇಲಿನ);
		t->osd2.linux2.l_i_checksum_lo =
			ext2fs_swab16(f->osd2.linux2.l_i_checksum_lo);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2_OS_HURD:
		t->osd1.hurd1.h_i_translator =
		  ext2fs_swab32 (f->osd1.hurd1.h_i_translator);
		t->osd2.hurd2.h_i_frag = f->osd2.hurd2.h_i_frag;
		t->osd2.hurd2.h_i_fsize = f->osd2.hurd2.h_i_fsize;
		t->osd2.hurd2.h_i_ಬಹುಲಕ_ಮೇಲಿನ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_ಬಹುಲಕ_ಮೇಲಿನ);
		t->osd2.hurd2.h_i_uid_ಮೇಲಿನ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_uid_ಮೇಲಿನ);
		t->osd2.hurd2.h_i_gid_ಮೇಲಿನ =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_gid_ಮೇಲಿನ);
		t->osd2.hurd2.h_i_author =
		  ext2fs_swab32 (f->osd2.hurd2.h_i_author);
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (bufsize < (ಪೂರ್ಣಾಂಕ) (ಗಾತ್ರ(ರಚನೆ ext2_inode) + ಗಾತ್ರ(__u16)))
		ಹಿಂತಿರುಗಿ; /* no i_extra_isize field */

	ಆದರೆ (hostorder)
		extra_isize = f->i_extra_isize;
	t->i_extra_isize = ext2fs_swab16(f->i_extra_isize);
	ಆದರೆ (!hostorder)
		extra_isize = t->i_extra_isize;
	ಆದರೆ (extra_isize > EXT2_INODE_ಗಾತ್ರ(fs->super) -
				ಗಾತ್ರ(ರಚನೆ ext2_inode)) {
		/* this is error case: i_extra_size is too large */
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (extra_isize & 3)
		ಹಿಂತಿರುಗಿ;		/* Illegal inode extra_isize */

	inode_ಗಾತ್ರ = EXT2_GOOD_OLD_INODE_ಗಾತ್ರ + extra_isize;
	ಆದರೆ (ext2fs_inode_includes(inode_ಗಾತ್ರ, i_checksum_hi))
		t->i_checksum_hi = ext2fs_swab16(f->i_checksum_hi);
	ಆದರೆ (ext2fs_inode_includes(inode_ಗಾತ್ರ, i_ctime_extra))
		t->i_ctime_extra = ext2fs_swab32(f->i_ctime_extra);
	ಆದರೆ (ext2fs_inode_includes(inode_ಗಾತ್ರ, i_mtime_extra))
		t->i_mtime_extra = ext2fs_swab32(f->i_mtime_extra);
	ಆದರೆ (ext2fs_inode_includes(inode_ಗಾತ್ರ, i_atime_extra))
		t->i_atime_extra = ext2fs_swab32(f->i_atime_extra);
	ಆದರೆ (ext2fs_inode_includes(inode_ಗಾತ್ರ, i_crtime))
		t->i_crtime = ext2fs_swab32(f->i_crtime);
	ಆದರೆ (ext2fs_inode_includes(inode_ಗಾತ್ರ, i_crtime_extra))
		t->i_crtime_extra = ext2fs_swab32(f->i_crtime_extra);
	ಆದರೆ (ext2fs_inode_includes(inode_ಗಾತ್ರ, i_ಆವೃತ್ತಿ_hi))
		t->i_ಆವೃತ್ತಿ_hi = ext2fs_swab32(f->i_ಆವೃತ್ತಿ_hi);
	ಆದರೆ (ext2fs_inode_includes(inode_ಗಾತ್ರ, i_projid))
                t->i_projid = ext2fs_swab32(f->i_projid);
	/* catch new static fields added after i_projid */
	EXT2FS_BUILD_BUG_ON(ಗಾತ್ರ(ರಚನೆ ext2_inode_large) != 160);

	i = ಗಾತ್ರ(ರಚನೆ ext2_inode) + extra_isize + ಗಾತ್ರ(__u32);
	ಆದರೆ (bufsize < (ಪೂರ್ಣಾಂಕ) i)
		ಹಿಂತಿರುಗಿ; /* no space for EA magic */

	eaf = (__u32 *) (((ಅಕ್ಷರ *) f) + ಗಾತ್ರ(ರಚನೆ ext2_inode) +
					extra_isize);

	attr_magic = *eaf;
	ಆದರೆ (!hostorder)
		attr_magic = ext2fs_swab32(attr_magic);

	ಆದರೆ (attr_magic != EXT2_EXT_ATTR_MAGIC)
		ಹಿಂತಿರುಗಿ; /* it seems no magic here */

	eat = (__u32 *) (((ಅಕ್ಷರ *) t) + ಗಾತ್ರ(ರಚನೆ ext2_inode) +
					extra_isize);
	*eat = ext2fs_swab32(*eaf);

	/* convert EA(s) */
	ext2fs_ಅದಲುಬದಲು_ext_attr((ಅಕ್ಷರ *) (eat + 1), (ಅಕ್ಷರ *) (eaf + 1),
			     bufsize - ಗಾತ್ರ(ರಚನೆ ext2_inode) -
			     extra_isize - ಗಾತ್ರ(__u32), 0);

}

ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_inode(ext2_filsys fs, ರಚನೆ ext2_inode *t,
		       ರಚನೆ ext2_inode *f, ಪೂರ್ಣಾಂಕ hostorder)
{
	ext2fs_ಅದಲುಬದಲು_inode_ತುಂಬಿದ(fs, (ರಚನೆ ext2_inode_large *) t,
				(ರಚನೆ ext2_inode_large *) f, hostorder,
				ಗಾತ್ರ(ರಚನೆ ext2_inode));
}

ಶೂನ್ಯ ext2fs_ಅದಲುಬದಲು_mmp(ರಚನೆ mmp_struct *mmp)
{
	mmp->mmp_magic = ext2fs_swab32(mmp->mmp_magic);
	mmp->mmp_seq = ext2fs_swab32(mmp->mmp_seq);
	mmp->mmp_ಸಮಯ = ext2fs_swab64(mmp->mmp_ಸಮಯ);
	mmp->mmp_check_interval = ext2fs_swab16(mmp->mmp_check_interval);
	mmp->mmp_checksum = ext2fs_swab32(mmp->mmp_checksum);
}

errcode_t ext2fs_dirent_swab_ಒಳ(ext2_filsys fs, ಅಕ್ಷರ *buf, ಪೂರ್ಣಾಂಕ flags)
{
	ಹಿಂತಿರುಗಿ ext2fs_dirent_swab_in2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, ಅಕ್ಷರ *buf,
				 size_t ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ flags)
{
	errcode_t	retval;
	ಅಕ್ಷರ		*p, *ಕೊನೆ;
	ರಚನೆ ext2_dir_ನಮೂದು *dirent;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	name_len, rec_len, ಎಡ;

	p = (ಅಕ್ಷರ *) buf;
	ಕೊನೆ = (ಅಕ್ಷರ *) buf + ಗಾತ್ರ_2;
	ಎಡ = ಗಾತ್ರ_2;
	ಆಗ (p < ಕೊನೆ-8) {
		dirent = (ರಚನೆ ext2_dir_ನಮೂದು *) p;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		name_len = dirent->name_len;
		ಆದರೆ (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಆದರೆ ((rec_len < 8) || (rec_len % 4)) {
			rec_len = 8;
			ಆದರೆ (!(fs->flags & EXT2_FLAG_IGNORE_ಅದಲುಬದಲು_DIRENT))
				ಹಿಂತಿರುಗಿ EXT2_ET_DIR_CORRUPTED;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (((name_len & 0xFF) + 8) > rec_len)
			ಆದರೆ (!(fs->flags & EXT2_FLAG_IGNORE_ಅದಲುಬದಲು_DIRENT))
				ಹಿಂತಿರುಗಿ EXT2_ET_DIR_CORRUPTED;
		ಆದರೆ (rec_len > ಎಡ)
			ಆದರೆ (!(fs->flags & EXT2_FLAG_IGNORE_ಅದಲುಬದಲು_DIRENT))
				ಹಿಂತಿರುಗಿ EXT2_ET_DIR_CORRUPTED;
		ಎಡ -= rec_len;
		p += rec_len;
	}

	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_dirent_swab_ಹೊರಹೋಗು(ext2_filsys fs, ಅಕ್ಷರ *buf, ಪೂರ್ಣಾಂಕ flags)
{
	ಹಿಂತಿರುಗಿ ext2fs_dirent_swab_out2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, ಅಕ್ಷರ *buf,
				  size_t ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ flags)
{
	errcode_t	retval;
	ಅಕ್ಷರ		*p, *ಕೊನೆ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	rec_len;
	ರಚನೆ ext2_dir_ನಮೂದು *dirent;

	p = buf;
	ಕೊನೆ = buf + ಗಾತ್ರ_2;
	ಆಗ (p < ಕೊನೆ) {
		dirent = (ರಚನೆ ext2_dir_ನಮೂದು *) p;
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಆದರೆ ((rec_len < 8) ||
		    (rec_len % 4)) {
			ext2fs_free_mem(&buf);
			ಹಿಂತಿರುಗಿ EXT2_ET_DIR_CORRUPTED;
		}
		p += rec_len;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		ಆದರೆ (rec_len > ಗಾತ್ರ_2)
			ಹಿಂತಿರುಗಿ EXT2_ET_DIR_CORRUPTED;
		ಗಾತ್ರ_2 -= rec_len;

		ಆದರೆ (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
	}

	ಹಿಂತಿರುಗಿ 0;
}
