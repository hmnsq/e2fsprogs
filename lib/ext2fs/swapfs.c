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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"
#شامل <ext2fs/ext2_ext_attr.h>

تهی ext2fs_swap_super(ساختار ext2_super_block * sb)
{
	صحیح i;

	sb->s_inodes_شمار = ext2fs_swab32(sb->s_inodes_شمار);
	sb->s_blocks_شمار = ext2fs_swab32(sb->s_blocks_شمار);
	sb->s_r_blocks_شمار = ext2fs_swab32(sb->s_r_blocks_شمار);
	sb->s_free_blocks_شمار = ext2fs_swab32(sb->s_free_blocks_شمار);
	sb->s_free_inodes_شمار = ext2fs_swab32(sb->s_free_inodes_شمار);
	sb->s_first_data_block = ext2fs_swab32(sb->s_first_data_block);
	sb->s_log_block_size = ext2fs_swab32(sb->s_log_block_size);
	sb->s_log_cluster_size = ext2fs_swab32(sb->s_log_cluster_size);
	sb->s_blocks_per_group = ext2fs_swab32(sb->s_blocks_per_group);
	sb->s_clusters_per_group = ext2fs_swab32(sb->s_clusters_per_group);
	sb->s_inodes_per_group = ext2fs_swab32(sb->s_inodes_per_group);
	sb->s_mtime = ext2fs_swab32(sb->s_mtime);
	sb->s_wtime = ext2fs_swab32(sb->s_wtime);
	sb->s_mnt_شمار = ext2fs_swab16(sb->s_mnt_شمار);
	sb->s_max_mnt_شمار = ext2fs_swab16(sb->s_max_mnt_شمار);
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
	sb->s_first_ino = ext2fs_swab32(sb->s_first_ino);
	sb->s_inode_size = ext2fs_swab16(sb->s_inode_size);
	sb->s_block_group_nr = ext2fs_swab16(sb->s_block_group_nr);
	sb->s_feature_compat = ext2fs_swab32(sb->s_feature_compat);
	sb->s_feature_incompat = ext2fs_swab32(sb->s_feature_incompat);
	sb->s_feature_ro_compat = ext2fs_swab32(sb->s_feature_ro_compat);
	/* sb->s_uuid is __u8 and does not need swabbing */
	/* sb->s_volume_name is char and does not need swabbing */
	/* sb->s_last_mounted is char and does not need swabbing */
	sb->s_algorithm_usage_bitmap = ext2fs_swab32(sb->s_algorithm_usage_bitmap);
	/* sb->s_prealloc_blocks is __u8 and does not need swabbing */
	/* sb->s_prealloc_dir_blocks is __u8 and does not need swabbing */
	sb->s_reserved_gdt_blocks = ext2fs_swab16(sb->s_reserved_gdt_blocks);
	/* sb->s_journal_uuid is __u8 and does not need swabbing */
	sb->s_journal_inum = ext2fs_swab32(sb->s_journal_inum);
	sb->s_journal_dev = ext2fs_swab32(sb->s_journal_dev);
	sb->s_last_orphan = ext2fs_swab32(sb->s_last_orphan);
	برای (i = 0; i < 4; i++)
		sb->s_hash_seed[i] = ext2fs_swab32(sb->s_hash_seed[i]);
	/* sb->s_def_hash_version is __u8 and does not need swabbing */
	/* sb->s_jnl_backup_type is __u8 and does not need swabbing */
	sb->s_desc_size = ext2fs_swab16(sb->s_desc_size);
	sb->s_default_mount_opts = ext2fs_swab32(sb->s_default_mount_opts);
	sb->s_first_meta_bg = ext2fs_swab32(sb->s_first_meta_bg);
	sb->s_mkfs_time = ext2fs_swab32(sb->s_mkfs_time);
	/* if journal backup is for a valid extent-based journal... */
	اگر (ext2fs_extent_header_verify(sb->s_jnl_blocks,
					اندازه(sb->s_jnl_blocks)) == 0) {
		/* ... swap only the journal i_size and i_size_high,
		 * and the extent data is not swapped on read */
		i = 15;
	} وگرنه {
		/* direct/indirect journal: swap it all */
		i = 0;
	}
	برای (; i < 17; i++)
		sb->s_jnl_blocks[i] = ext2fs_swab32(sb->s_jnl_blocks[i]);
	sb->s_blocks_شمار_hi = ext2fs_swab32(sb->s_blocks_شمار_hi);
	sb->s_r_blocks_شمار_hi = ext2fs_swab32(sb->s_r_blocks_شمار_hi);
	sb->s_free_blocks_hi = ext2fs_swab32(sb->s_free_blocks_hi);
	sb->s_min_extra_isize = ext2fs_swab16(sb->s_min_extra_isize);
	sb->s_want_extra_isize = ext2fs_swab16(sb->s_want_extra_isize);
	sb->s_flags = ext2fs_swab32(sb->s_flags);
	sb->s_raid_stride = ext2fs_swab16(sb->s_raid_stride);
	sb->s_mmp_update_interval = ext2fs_swab16(sb->s_mmp_update_interval);
	sb->s_mmp_block = ext2fs_swab64(sb->s_mmp_block);
	sb->s_raid_stripe_width = ext2fs_swab32(sb->s_raid_stripe_width);
	/* sb->s_log_groups_per_flex is __u8 and does not need swabbing */
	/* sb->s_checksum_type is __u8 and does not need swabbing */
	/* sb->s_encryption_level is __u8 and does not need swabbing */
	/* sb->s_reserved_pad is __u8 and does not need swabbing */
	sb->s_kbytes_written = ext2fs_swab64(sb->s_kbytes_written);
	sb->s_snapshot_inum = ext2fs_swab32(sb->s_snapshot_inum);
	sb->s_snapshot_id = ext2fs_swab32(sb->s_snapshot_id);
	sb->s_snapshot_r_blocks_شمار =
		ext2fs_swab64(sb->s_snapshot_r_blocks_شمار);
	sb->s_snapshot_list = ext2fs_swab32(sb->s_snapshot_list);
	sb->s_error_شمار = ext2fs_swab32(sb->s_error_شمار);
	sb->s_first_error_time = ext2fs_swab32(sb->s_first_error_time);
	sb->s_first_error_ino = ext2fs_swab32(sb->s_first_error_ino);
	sb->s_first_error_block = ext2fs_swab64(sb->s_first_error_block);
	/* sb->s_first_error_func is __u8 and does not need swabbing */
	sb->s_last_error_time = ext2fs_swab32(sb->s_last_error_time);
	sb->s_last_error_ino = ext2fs_swab32(sb->s_last_error_ino);
	sb->s_last_error_block = ext2fs_swab64(sb->s_last_error_block);
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
	sb->s_checksum_seed = ext2fs_swab32(sb->s_checksum_seed);
	/* s_*_time_hi are __u8 and does not need swabbing */
	sb->s_encoding = ext2fs_swab16(sb->s_encoding);
	sb->s_encoding_flags = ext2fs_swab16(sb->s_encoding_flags);
	sb->s_orphan_file_inum = ext2fs_swab32(sb->s_orphan_file_inum);
	/* catch when new fields are used from s_reserved */
	EXT2FS_BUILD_BUG_ON(اندازه(sb->s_reserved) != 94 * اندازه(__le32));
	sb->s_checksum = ext2fs_swab32(sb->s_checksum);
}

تهی ext2fs_swap_group_desc2(ext2_filsys fs, ساختار ext2_group_desc *gdp)
{
	ساختار ext4_group_desc *gdp4 = (ساختار ext4_group_desc *)gdp;

	/* Do the 32-bit parts first */
	gdp->bg_block_bitmap = ext2fs_swab32(gdp->bg_block_bitmap);
	gdp->bg_inode_bitmap = ext2fs_swab32(gdp->bg_inode_bitmap);
	gdp->bg_inode_table = ext2fs_swab32(gdp->bg_inode_table);
	gdp->bg_free_blocks_شمار = ext2fs_swab16(gdp->bg_free_blocks_شمار);
	gdp->bg_free_inodes_شمار = ext2fs_swab16(gdp->bg_free_inodes_شمار);
	gdp->bg_used_dirs_شمار = ext2fs_swab16(gdp->bg_used_dirs_شمار);
	gdp->bg_flags = ext2fs_swab16(gdp->bg_flags);
	gdp->bg_exclude_bitmap_lo = ext2fs_swab32(gdp->bg_exclude_bitmap_lo);
	gdp->bg_block_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_block_bitmap_csum_lo);
	gdp->bg_inode_bitmap_csum_lo =
		ext2fs_swab16(gdp->bg_inode_bitmap_csum_lo);
	gdp->bg_itable_unused = ext2fs_swab16(gdp->bg_itable_unused);
	gdp->bg_checksum = ext2fs_swab16(gdp->bg_checksum);
	/* If we're 32-bit, we're done */
	اگر (fs == پوچ || EXT2_DESC_SIZE(fs->super) < EXT2_MIN_DESC_SIZE_64BIT)
		بازگشت;

	/* Swap the 64-bit parts */
	gdp4->bg_block_bitmap_hi = ext2fs_swab32(gdp4->bg_block_bitmap_hi);
	gdp4->bg_inode_bitmap_hi = ext2fs_swab32(gdp4->bg_inode_bitmap_hi);
	gdp4->bg_inode_table_hi = ext2fs_swab32(gdp4->bg_inode_table_hi);
	gdp4->bg_free_blocks_شمار_hi =
		ext2fs_swab16(gdp4->bg_free_blocks_شمار_hi);
	gdp4->bg_free_inodes_شمار_hi =
		ext2fs_swab16(gdp4->bg_free_inodes_شمار_hi);
	gdp4->bg_used_dirs_شمار_hi =
		ext2fs_swab16(gdp4->bg_used_dirs_شمار_hi);
	gdp4->bg_itable_unused_hi = ext2fs_swab16(gdp4->bg_itable_unused_hi);
	gdp4->bg_exclude_bitmap_hi = ext2fs_swab16(gdp4->bg_exclude_bitmap_hi);
	gdp4->bg_block_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_block_bitmap_csum_hi);
	gdp4->bg_inode_bitmap_csum_hi =
		ext2fs_swab16(gdp4->bg_inode_bitmap_csum_hi);
	EXT2FS_BUILD_BUG_ON(اندازه(gdp4->bg_reserved) != اندازه(__u32));
}

تهی ext2fs_swap_group_desc(ساختار ext2_group_desc *gdp)
{
	ext2fs_swap_group_desc2(0, gdp);
}


تهی ext2fs_swap_ext_attr_header(ساختار ext2_ext_attr_header *to_header,
				 ساختار ext2_ext_attr_header *from_header)
{
	صحیح n;

	to_header->h_magic    = ext2fs_swab32(from_header->h_magic);
	to_header->h_blocks   = ext2fs_swab32(from_header->h_blocks);
	to_header->h_refcount = ext2fs_swab32(from_header->h_refcount);
	to_header->h_hash     = ext2fs_swab32(from_header->h_hash);
	to_header->h_checksum = ext2fs_swab32(from_header->h_checksum);
	برای (n = 0; n < 3; n++)
		to_header->h_reserved[n] =
			ext2fs_swab32(from_header->h_reserved[n]);
}

تهی ext2fs_swap_ext_attr_entry(ساختار ext2_ext_attr_entry *to_entry,
				ساختار ext2_ext_attr_entry *from_entry)
{
	to_entry->e_مقدار_offs  = ext2fs_swab16(from_entry->e_مقدار_offs);
	to_entry->e_مقدار_inum  = ext2fs_swab32(from_entry->e_مقدار_inum);
	to_entry->e_مقدار_size  = ext2fs_swab32(from_entry->e_مقدار_size);
	to_entry->e_hash	= ext2fs_swab32(from_entry->e_hash);
}

تهی ext2fs_swap_ext_attr(نویسه *to, نویسه *from, صحیح bufsize, صحیح has_header)
{
	ساختار ext2_ext_attr_header *from_header =
		(ساختار ext2_ext_attr_header *)from;
	ساختار ext2_ext_attr_header *to_header =
		(ساختار ext2_ext_attr_header *)to;
	ساختار ext2_ext_attr_entry *from_entry, *to_entry;
	نویسه *from_end = (نویسه *)from_header + bufsize;

	اگر (to_header != from_header)
		memcpy(to_header, from_header, bufsize);

	اگر (has_header) {
		ext2fs_swap_ext_attr_header(to_header, from_header);

		from_entry = (ساختار ext2_ext_attr_entry *)(from_header+1);
		to_entry   = (ساختار ext2_ext_attr_entry *)(to_header+1);
	} وگرنه {
		from_entry = (ساختار ext2_ext_attr_entry *)from_header;
		to_entry   = (ساختار ext2_ext_attr_entry *)to_header;
	}

	وقتی ((نویسه *)from_entry < from_end &&
	       (نویسه *)EXT2_EXT_ATTR_NEXT(from_entry) <= from_end &&
	       *(__u32 *)from_entry) {
		ext2fs_swap_ext_attr_entry(to_entry, from_entry);
		from_entry = EXT2_EXT_ATTR_NEXT(from_entry);
		to_entry   = EXT2_EXT_ATTR_NEXT(to_entry);
	}
}

تهی ext2fs_swap_inode_full(ext2_filsys fs, ساختار ext2_inode_large *t,
			    ساختار ext2_inode_large *f, صحیح hostorder,
			    صحیح bufsize)
{
	unsigned i, extra_isize, attr_magic;
	صحیح has_extents = 0, has_inline_data = 0, islnk = 0, fast_symlink = 0;
	unsigned صحیح inode_size;
	__u32 *eaf, *eat;

	/*
	 * Note that t and f may point to the same address. That's why
	 * if (hostorder) condition is executed before swab calls and
	 * if (!hostorder) afterwards.
	 */
	اگر (hostorder) {
		islnk = LINUX_S_ISLNK(f->i_mode);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(f));
		has_extents = (f->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_data = (f->i_flags & EXT4_INLINE_DATA_FL) != 0;
	}

	t->i_mode = ext2fs_swab16(f->i_mode);
	t->i_uid = ext2fs_swab16(f->i_uid);
	t->i_size = ext2fs_swab32(f->i_size);
	t->i_atime = ext2fs_swab32(f->i_atime);
	t->i_ctime = ext2fs_swab32(f->i_ctime);
	t->i_mtime = ext2fs_swab32(f->i_mtime);
	t->i_dtime = ext2fs_swab32(f->i_dtime);
	t->i_gid = ext2fs_swab16(f->i_gid);
	t->i_links_شمار = ext2fs_swab16(f->i_links_شمار);
	t->i_file_acl = ext2fs_swab32(f->i_file_acl);
	t->i_blocks = ext2fs_swab32(f->i_blocks);
	t->i_flags = ext2fs_swab32(f->i_flags);
	t->i_size_بالا = ext2fs_swab32(f->i_size_بالا);

	اگر (!hostorder) {
		islnk = LINUX_S_ISLNK(t->i_mode);
		fast_symlink = ext2fs_is_fast_symlink(EXT2_INODE(t));
		has_extents = (t->i_flags & EXT4_EXTENTS_FL) != 0;
		has_inline_data = (t->i_flags & EXT4_INLINE_DATA_FL) != 0;
	}

	/*
	 * Extent data and inline data are swapped on access, not here
	 */
	اگر (!has_extents && !has_inline_data && (!islnk || !fast_symlink)) {
		برای (i = 0; i < EXT2_N_BLOCKS; i++)
			t->i_block[i] = ext2fs_swab32(f->i_block[i]);
	} وگرنه اگر (t != f) {
		برای (i = 0; i < EXT2_N_BLOCKS; i++)
			t->i_block[i] = f->i_block[i];
	}
	t->i_generation = ext2fs_swab32(f->i_generation);
	t->i_faddr = ext2fs_swab32(f->i_faddr);

	انتخاب (fs->super->s_creator_os) {
	حالت EXT2_OS_LINUX:
		t->osd1.linux1.l_i_version =
			ext2fs_swab32(f->osd1.linux1.l_i_version);
		t->osd2.linux2.l_i_blocks_hi =
			ext2fs_swab16(f->osd2.linux2.l_i_blocks_hi);
		t->osd2.linux2.l_i_file_acl_بالا =
			ext2fs_swab16(f->osd2.linux2.l_i_file_acl_بالا);
		t->osd2.linux2.l_i_uid_بالا =
		  ext2fs_swab16 (f->osd2.linux2.l_i_uid_بالا);
		t->osd2.linux2.l_i_gid_بالا =
		  ext2fs_swab16 (f->osd2.linux2.l_i_gid_بالا);
		t->osd2.linux2.l_i_checksum_lo =
			ext2fs_swab16(f->osd2.linux2.l_i_checksum_lo);
		بشکن;
	حالت EXT2_OS_HURD:
		t->osd1.hurd1.h_i_translator =
		  ext2fs_swab32 (f->osd1.hurd1.h_i_translator);
		t->osd2.hurd2.h_i_frag = f->osd2.hurd2.h_i_frag;
		t->osd2.hurd2.h_i_fsize = f->osd2.hurd2.h_i_fsize;
		t->osd2.hurd2.h_i_mode_بالا =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_mode_بالا);
		t->osd2.hurd2.h_i_uid_بالا =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_uid_بالا);
		t->osd2.hurd2.h_i_gid_بالا =
		  ext2fs_swab16 (f->osd2.hurd2.h_i_gid_بالا);
		t->osd2.hurd2.h_i_author =
		  ext2fs_swab32 (f->osd2.hurd2.h_i_author);
		بشکن;
	پیشفرض:
		بشکن;
	}

	اگر (bufsize < (صحیح) (اندازه(ساختار ext2_inode) + اندازه(__u16)))
		بازگشت; /* no i_extra_isize field */

	اگر (hostorder)
		extra_isize = f->i_extra_isize;
	t->i_extra_isize = ext2fs_swab16(f->i_extra_isize);
	اگر (!hostorder)
		extra_isize = t->i_extra_isize;
	اگر (extra_isize > EXT2_INODE_SIZE(fs->super) -
				اندازه(ساختار ext2_inode)) {
		/* this is error case: i_extra_size is too large */
		بازگشت;
	}
	اگر (extra_isize & 3)
		بازگشت;		/* Illegal inode extra_isize */

	inode_size = EXT2_GOOD_OLD_INODE_SIZE + extra_isize;
	اگر (ext2fs_inode_includes(inode_size, i_checksum_hi))
		t->i_checksum_hi = ext2fs_swab16(f->i_checksum_hi);
	اگر (ext2fs_inode_includes(inode_size, i_ctime_extra))
		t->i_ctime_extra = ext2fs_swab32(f->i_ctime_extra);
	اگر (ext2fs_inode_includes(inode_size, i_mtime_extra))
		t->i_mtime_extra = ext2fs_swab32(f->i_mtime_extra);
	اگر (ext2fs_inode_includes(inode_size, i_atime_extra))
		t->i_atime_extra = ext2fs_swab32(f->i_atime_extra);
	اگر (ext2fs_inode_includes(inode_size, i_crtime))
		t->i_crtime = ext2fs_swab32(f->i_crtime);
	اگر (ext2fs_inode_includes(inode_size, i_crtime_extra))
		t->i_crtime_extra = ext2fs_swab32(f->i_crtime_extra);
	اگر (ext2fs_inode_includes(inode_size, i_version_hi))
		t->i_version_hi = ext2fs_swab32(f->i_version_hi);
	اگر (ext2fs_inode_includes(inode_size, i_projid))
                t->i_projid = ext2fs_swab32(f->i_projid);
	/* catch new static fields added after i_projid */
	EXT2FS_BUILD_BUG_ON(اندازه(ساختار ext2_inode_large) != 160);

	i = اندازه(ساختار ext2_inode) + extra_isize + اندازه(__u32);
	اگر (bufsize < (صحیح) i)
		بازگشت; /* no space for EA magic */

	eaf = (__u32 *) (((نویسه *) f) + اندازه(ساختار ext2_inode) +
					extra_isize);

	attr_magic = *eaf;
	اگر (!hostorder)
		attr_magic = ext2fs_swab32(attr_magic);

	اگر (attr_magic != EXT2_EXT_ATTR_MAGIC)
		بازگشت; /* it seems no magic here */

	eat = (__u32 *) (((نویسه *) t) + اندازه(ساختار ext2_inode) +
					extra_isize);
	*eat = ext2fs_swab32(*eaf);

	/* convert EA(s) */
	ext2fs_swap_ext_attr((نویسه *) (eat + 1), (نویسه *) (eaf + 1),
			     bufsize - اندازه(ساختار ext2_inode) -
			     extra_isize - اندازه(__u32), 0);

}

تهی ext2fs_swap_inode(ext2_filsys fs, ساختار ext2_inode *t,
		       ساختار ext2_inode *f, صحیح hostorder)
{
	ext2fs_swap_inode_full(fs, (ساختار ext2_inode_large *) t,
				(ساختار ext2_inode_large *) f, hostorder,
				اندازه(ساختار ext2_inode));
}

تهی ext2fs_swap_mmp(ساختار mmp_struct *mmp)
{
	mmp->mmp_magic = ext2fs_swab32(mmp->mmp_magic);
	mmp->mmp_seq = ext2fs_swab32(mmp->mmp_seq);
	mmp->mmp_time = ext2fs_swab64(mmp->mmp_time);
	mmp->mmp_check_interval = ext2fs_swab16(mmp->mmp_check_interval);
	mmp->mmp_checksum = ext2fs_swab32(mmp->mmp_checksum);
}

errcode_t ext2fs_dirent_swab_in(ext2_filsys fs, نویسه *buf, صحیح flags)
{
	بازگشت ext2fs_dirent_swab_in2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_in2(ext2_filsys fs, نویسه *buf,
				 size_t size, صحیح flags)
{
	errcode_t	retval;
	نویسه		*p, *end;
	ساختار ext2_dir_entry *dirent;
	unsigned صحیح	name_len, rec_len, چپ;

	p = (نویسه *) buf;
	end = (نویسه *) buf + size;
	چپ = size;
	وقتی (p < end-8) {
		dirent = (ساختار ext2_dir_entry *) p;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		name_len = dirent->name_len;
		اگر (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		اگر (retval)
			بازگشت retval;
		اگر ((rec_len < 8) || (rec_len % 4)) {
			rec_len = 8;
			اگر (!(fs->flags & EXT2_FLAG_IGNORE_SWAP_DIRENT))
				بازگشت EXT2_ET_DIR_CORRUPTED;
		} وگرنه اگر (((name_len & 0xFF) + 8) > rec_len)
			اگر (!(fs->flags & EXT2_FLAG_IGNORE_SWAP_DIRENT))
				بازگشت EXT2_ET_DIR_CORRUPTED;
		اگر (rec_len > چپ)
			اگر (!(fs->flags & EXT2_FLAG_IGNORE_SWAP_DIRENT))
				بازگشت EXT2_ET_DIR_CORRUPTED;
		چپ -= rec_len;
		p += rec_len;
	}

	بازگشت 0;
}

errcode_t ext2fs_dirent_swab_out(ext2_filsys fs, نویسه *buf, صحیح flags)
{
	بازگشت ext2fs_dirent_swab_out2(fs, buf, fs->blocksize, flags);
}

errcode_t ext2fs_dirent_swab_out2(ext2_filsys fs, نویسه *buf,
				  size_t size, صحیح flags)
{
	errcode_t	retval;
	نویسه		*p, *end;
	unsigned صحیح	rec_len;
	ساختار ext2_dir_entry *dirent;

	p = buf;
	end = buf + size;
	وقتی (p < end) {
		dirent = (ساختار ext2_dir_entry *) p;
		retval = ext2fs_get_rec_len(fs, dirent, &rec_len);
		اگر (retval)
			بازگشت retval;
		اگر ((rec_len < 8) ||
		    (rec_len % 4)) {
			ext2fs_free_mem(&buf);
			بازگشت EXT2_ET_DIR_CORRUPTED;
		}
		p += rec_len;
		dirent->inode = ext2fs_swab32(dirent->inode);
		dirent->rec_len = ext2fs_swab16(dirent->rec_len);
		dirent->name_len = ext2fs_swab16(dirent->name_len);
		اگر (rec_len > size)
			بازگشت EXT2_ET_DIR_CORRUPTED;
		size -= rec_len;

		اگر (flags & EXT2_DIRBLOCK_V2_STRUCT)
			dirent->name_len = ext2fs_swab16(dirent->name_len);
	}

	بازگشت 0;
}
