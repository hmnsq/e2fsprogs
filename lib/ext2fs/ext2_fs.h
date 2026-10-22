/*
 *  linux/include/linux/ext2_fs.h
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/include/linux/minix_fs.h
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#جيڪڏهن_وصف_نه_ٿيل _LINUX_EXT2_FS_H
#وصف_ڪريو _LINUX_EXT2_FS_H

#ضمن <ext2fs/ext2_types.h>		/* Changed from linux/types.h */

#جيڪڏهن_وصف_نه_ٿيل __GNUC_PREREQ
#إذا defined(__GNUC__) && defined(__GNUC_MINOR__)
#وصف_ڪريو __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وإلا
#وصف_ڪريو __GNUC_PREREQ(maj, min) 0
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل __nonstring
#جيڪڏهن_وصف_ٿيل __has_attribute
#إذا __has_attribute(__nonstring__)
#وصف_ڪريو __nonstring                    __attribute__((__nonstring__))
#وإلا
#وصف_ڪريو __nonstring
#جيڪڏهن_پورو /* __has_attribute(__nonstring__) */
#وإلا
# define __nonstring
#جيڪڏهن_پورو /* __has_attribute */
#جيڪڏهن_پورو /* __nonstring */

/*
 * The second extended filesystem constants/structures
 */

/*
 * Define EXT2FS_DEBUG to produce debug messages
 */
#وصف_رد EXT2FS_DEBUG

/*
 * Define EXT2_PREALLOCATE to preallocate data blocks for expanding files
 */
#وصف_ڪريو EXT2_PREALLOCATE
#وصف_ڪريو EXT2_DEFAULT_PREALLOC_بلاڪون	8

/*
 * The second extended file system version
 */
#وصف_ڪريو EXT2FS_DATE		"95/08/09"
#وصف_ڪريو EXT2_FS_نسخو		"0.5b"

/*
 * Special inode numbers
 */
#وصف_ڪريو EXT2_BAD_INO		 1	/* Bad blocks inode */
#وصف_ڪريو EXT2_پاڙ_INO		 2	/* Root inode */
#وصف_ڪريو EXT4_USR_QUOTA_INO	 3	/* User quota inode */
#وصف_ڪريو EXT4_GRP_QUOTA_INO	 4	/* Group quota inode */
#وصف_ڪريو EXT2_BOOT_LOADER_INO	 5	/* Boot loader inode */
#وصف_ڪريو EXT2_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#وصف_ڪريو EXT2_RESIZE_INO		 7	/* Reserved group descriptors inode */
#وصف_ڪريو EXT2_JOURNAL_INO	 8	/* Journal inode */
#وصف_ڪريو EXT2_EXCLUDE_INO	 9	/* The "exclude" inode, for snapshots */
#وصف_ڪريو EXT4_REPLICA_INO	10	/* Used by non-upstream feature */

/* First non-reserved inode for old ext2 filesystems */
#وصف_ڪريو EXT2_GOOD_OLD_پهريون_INO	11

/*
 * The second extended file system magic number
 */
#وصف_ڪريو EXT2_SUPER_MAGIC	0xEF53

#جيڪڏهن_وصف_ٿيل __KERNEL__
#وصف_ڪريو EXT2_SB(sb)	(&((sb)->u.ext2_sb))
#وإلا
/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#وصف_ڪريو EXT2_SB(sb)	(sb)
#جيڪڏهن_پورو

/*
 * Maximal count of links to a file
 */
#وصف_ڪريو EXT2_LINK_MAX		65000

/*
 * Macro-instructions used to manage several block sizes
 */
#وصف_ڪريو EXT2_MIN_بلاڪ_LOG_سائيز		10	/* 1024 */
#وصف_ڪريو EXT2_MAX_بلاڪ_LOG_سائيز		16	/* 65536 */
#وصف_ڪريو EXT2_MIN_بلاڪ_سائيز	(1 << EXT2_MIN_بلاڪ_LOG_سائيز)
#وصف_ڪريو EXT2_MAX_بلاڪ_سائيز	(1 << EXT2_MAX_بلاڪ_LOG_سائيز)
#جيڪڏهن_وصف_ٿيل __KERNEL__
#وصف_ڪريو EXT2_بلاڪ_سائيز(s)	((s)->s_blocksize)
#وصف_ڪريو EXT2_بلاڪ_سائيز_بٽ(s)	((s)->s_blocksize_بٽ)
#وصف_ڪريو EXT2_ADDR_في_بلاڪ_بٽ(s)	(EXT2_SB(s)->addr_في_بلاڪ_بٽ)
#وصف_ڪريو EXT2_INODE_سائيز(s)	(EXT2_SB(s)->s_inode_سائيز)
#وصف_ڪريو EXT2_پهريون_INO(s)	(EXT2_SB(s)->s_پهريون_ino)
#وإلا
#وصف_ڪريو EXT2_بلاڪ_سائيز(s)	(EXT2_MIN_بلاڪ_سائيز << (s)->s_log_بلاڪ_سائيز)
#وصف_ڪريو EXT2_بلاڪ_سائيز_بٽ(s)	((s)->s_log_بلاڪ_سائيز + 10)
#وصف_ڪريو EXT2_INODE_سائيز(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_INODE_سائيز : (s)->s_inode_سائيز)
#وصف_ڪريو EXT2_پهريون_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_پهريون_INO : (s)->s_پهريون_ino)
#جيڪڏهن_پورو
#وصف_ڪريو EXT2_ADDR_في_بلاڪ(s)	(EXT2_بلاڪ_سائيز(s) / حجم(__u32))

/*
 * Macro-instructions used to manage allocation clusters
 */
#وصف_ڪريو EXT2_MIN_CLUSTER_LOG_سائيز	EXT2_MIN_بلاڪ_LOG_سائيز
#وصف_ڪريو EXT2_MAX_CLUSTER_LOG_سائيز	29	/* 512MB  */
#وصف_ڪريو EXT2_MIN_CLUSTER_سائيز		EXT2_MIN_بلاڪ_سائيز
#وصف_ڪريو EXT2_MAX_CLUSTER_سائيز		(1 << EXT2_MAX_CLUSTER_LOG_سائيز)
#وصف_ڪريو EXT2_CLUSTER_سائيز(s)		(EXT2_MIN_بلاڪ_سائيز << \
						(s)->s_log_cluster_سائيز)
#وصف_ڪريو EXT2_CLUSTER_سائيز_بٽ(s)	((s)->s_log_cluster_سائيز + 10)

/*
 * Macro-instructions used to manage fragments
 *
 * Note: for backwards compatibility only, for the dump program.
 * Ext2/3/4 will never support fragments....
 */
#وصف_ڪريو EXT2_MIN_FRAG_سائيز              EXT2_MIN_بلاڪ_سائيز
#وصف_ڪريو EXT2_MAX_FRAG_سائيز              EXT2_MAX_بلاڪ_سائيز
#وصف_ڪريو EXT2_MIN_FRAG_LOG_سائيز          EXT2_MIN_بلاڪ_LOG_سائيز
#وصف_ڪريو EXT2_FRAG_سائيز(s)		EXT2_بلاڪ_سائيز(s)
#وصف_ڪريو EXT2_FRAGS_في_بلاڪ(s)		1

/*
 * ACL structures
 */
بنية ext2_acl_سرو	/* Header of Access Control Lists */
{
	__u32	aclh_سائيز;
	__u32	aclh_file_ڳڻپ;
	__u32	aclh_acle_ڳڻپ;
	__u32	aclh_پهريون_acle;
};

بنية ext2_acl_داخلا	/* Access Control List Entry */
{
	__u32	acle_سائيز;
	__u16	acle_perms;	/* Access permissions */
	__u16	acle_type;	/* Type of entry */
	__u16	acle_ٽيگ;	/* User or group identity */
	__u16	acle_pad1;
	__u32	acle_ايندڙ;	/* Pointer on next entry for the */
					/* same inode or on next free entry */
};

/*
 * Structure of a blocks group descriptor
 */
بنية ext2_گروهه_desc
{
	__u32	bg_بلاڪ_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_جدول;		/* Inodes table block */
	__u16	bg_free_بلاڪون_ڳڻپ;	/* Free blocks count */
	__u16	bg_free_inodes_ڳڻپ;	/* Free inodes count */
	__u16	bg_استعمال_ٿيل_dirs_ڳڻپ;	/* Directories count */
	__u16	bg_flags;
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_بلاڪ_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_اڻ_استعمال_ٿيل;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(s_uuid+group_num+group_desc)*/
};

/*
 * Structure of a blocks group descriptor
 */
بنية ext4_گروهه_desc
{
	__u32	bg_بلاڪ_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_جدول;		/* Inodes table block */
	__u16	bg_free_بلاڪون_ڳڻپ;	/* Free blocks count */
	__u16	bg_free_inodes_ڳڻپ;	/* Free inodes count */
	__u16	bg_استعمال_ٿيل_dirs_ڳڻپ;	/* Directories count */
	__u16	bg_flags;		/* EXT4_BG_flags (INODE_UNINIT, etc) */
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_بلاڪ_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_اڻ_استعمال_ٿيل;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(sb_uuid+group+desc) */
	__u32	bg_بلاڪ_bitmap_hi;	/* Blocks bitmap block MSB */
	__u32	bg_inode_bitmap_hi;	/* Inodes bitmap block MSB */
	__u32	bg_inode_جدول_hi;	/* Inodes table block MSB */
	__u16	bg_free_بلاڪون_ڳڻپ_hi;/* Free blocks count MSB */
	__u16	bg_free_inodes_ڳڻپ_hi;/* Free inodes count MSB */
	__u16	bg_استعمال_ٿيل_dirs_ڳڻپ_hi;	/* Directories count MSB */
	__u16	bg_itable_اڻ_استعمال_ٿيل_hi;	/* Unused inodes count MSB */
	__u32	bg_exclude_bitmap_hi;	/* Exclude bitmap block MSB */
	__u16	bg_بلاڪ_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u16	bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u32	bg_reserved;
};

#وصف_ڪريو EXT4_BG_INODE_BITMAP_CSUM_HI_پڇاڙي	\
	(offsetof(بنية ext4_گروهه_desc, bg_inode_bitmap_csum_hi) + \
	 حجم(__u16))
#وصف_ڪريو EXT4_BG_بلاڪ_BITMAP_CSUM_HI_LOCATION	\
	(offsetof(بنية ext4_گروهه_desc, bg_بلاڪ_bitmap_csum_hi) + \
	 حجم(__u16))

#وصف_ڪريو EXT2_BG_INODE_UNINIT	0x0001 /* Inode table/bitmap not initialized */
#وصف_ڪريو EXT2_BG_بلاڪ_UNINIT	0x0002 /* Block bitmap not initialized */
#وصف_ڪريو EXT2_BG_INODE_ZEROED	0x0004 /* On-disk itable initialized to zero */

/*
 * Data structures used by the directory indexing feature
 *
 * Note: all of the multibyte integer fields are little endian.
 */

/*
 * Note: dx_root_info is laid out so that if it should somehow get
 * overlaid by a dirent the two low bits of the hash version will be
 * zero.  Therefore, the hash version mod 4 should never be 0.
 * Sincerely, the paranoia department.
 */
بنية ext2_dx_پاڙ_info {
	__u32 reserved_zero;
	__u8 hash_نسخو; /* 0 now, 1 at release */
	__u8 info_ڊيگهائي; /* 8 */
	__u8 اڻسڌو_سطحون;
	__u8 اڻ_استعمال_ٿيل_flags;
};

#وصف_ڪريو EXT2_HASH_LEGACY		0
#وصف_ڪريو EXT2_HASH_HALF_MD4		1
#وصف_ڪريو EXT2_HASH_TEA			2
#وصف_ڪريو EXT2_HASH_LEGACY_UNSIGNED	3 /* reserved for userspace lib */
#وصف_ڪريو EXT2_HASH_HALF_MD4_UNSIGNED	4 /* reserved for userspace lib */
#وصف_ڪريو EXT2_HASH_TEA_UNSIGNED		5 /* reserved for userspace lib */
#وصف_ڪريو EXT2_HASH_SIPHASH		6

#وصف_ڪريو EXT2_HASH_FLAG_INCOMPAT	0x1

#وصف_ڪريو EXT4_DX_بلاڪ_نقاب 0x0fffffff

بنية ext2_dx_داخلا {
	__le32 hash;
	__le32 بلاڪ;
};

بنية ext2_dx_countlimit {
	__le16 حد_بندي;
	__le16 ڳڻپ;
};

/*
 * This goes at the end of each htree block.
 */
بنية ext2_dx_پڇ {
	__le32 dt_reserved;
	__le32 dt_checksum;	/* crc32c(uuid+inum+dxblock) */
};

/*
 * Macro-instructions used to manage group descriptors
 */
#وصف_ڪريو EXT2_MIN_DESC_سائيز             32
#وصف_ڪريو EXT2_MIN_DESC_سائيز_64_ٻٽو_انگ       64
#وصف_ڪريو EXT2_MAX_DESC_سائيز             EXT2_MIN_بلاڪ_سائيز
#وصف_ڪريو EXT2_DESC_سائيز(s)                                                \
	(ext2fs_has_feature_64bit(s) ? (s)->s_desc_سائيز : EXT2_MIN_DESC_سائيز)

#وصف_ڪريو EXT2_بلاڪون_في_گروهه(s)	(EXT2_SB(s)->s_بلاڪون_في_گروهه)
#وصف_ڪريو EXT2_INODES_في_گروهه(s)	(EXT2_SB(s)->s_inodes_في_گروهه)
#وصف_ڪريو EXT2_CLUSTERS_في_گروهه(s)	(EXT2_SB(s)->s_clusters_في_گروهه)
#وصف_ڪريو EXT2_INODES_في_بلاڪ(s)	(EXT2_بلاڪ_سائيز(s)/EXT2_INODE_سائيز(s))
/* limits imposed by 16-bit value gd_free_{blocks,inode}_count */
#وصف_ڪريو EXT2_MAX_بلاڪون_في_گروهه(s)	((((بنانشان) 1 << 16) - 8) *	\
					 (EXT2_CLUSTER_سائيز(s) / \
					  EXT2_بلاڪ_سائيز(s)))
#وصف_ڪريو EXT2_MAX_CLUSTERS_في_گروهه(s)	(((بنانشان) 1 << 16) - 8)
#وصف_ڪريو EXT2_MAX_INODES_في_گروهه(s)	(((بنانشان) 1 << 16) - \
					 EXT2_INODES_في_بلاڪ(s))
#جيڪڏهن_وصف_ٿيل __KERNEL__
#وصف_ڪريو EXT2_DESC_في_بلاڪ(s)		(EXT2_SB(s)->s_desc_في_بلاڪ)
#وصف_ڪريو EXT2_DESC_في_بلاڪ_بٽ(s)	(EXT2_SB(s)->s_desc_في_بلاڪ_بٽ)
#وإلا
#وصف_ڪريو EXT2_DESC_في_بلاڪ(s)		(EXT2_بلاڪ_سائيز(s) / EXT2_DESC_سائيز(s))
#جيڪڏهن_پورو

#وصف_ڪريو EXT2_GROUPS_منزل_بلاڪون(s, g)   ((blk64_t) EXT2_بلاڪون_في_گروهه(s) * \
				       (g))
#وصف_ڪريو EXT2_GROUPS_منزل_CLUSTERS(s, g) ((blk64_t) EXT2_CLUSTERS_في_گروهه(s) * \
				       (g))

/*
 * Constants relative to the data blocks
 */
#وصف_ڪريو EXT2_NDIR_بلاڪون		12
#وصف_ڪريو EXT2_IND_بلاڪ			EXT2_NDIR_بلاڪون
#وصف_ڪريو EXT2_DIND_بلاڪ			(EXT2_IND_بلاڪ + 1)
#وصف_ڪريو EXT2_TIND_بلاڪ			(EXT2_DIND_بلاڪ + 1)
#وصف_ڪريو EXT2_N_بلاڪون			(EXT2_TIND_بلاڪ + 1)

/*
 * Inode flags
 */
#وصف_ڪريو EXT2_SECRM_FL			0x00000001 /* Secure deletion */
#وصف_ڪريو EXT2_UNRM_FL			0x00000002 /* Undelete */
#وصف_ڪريو EXT2_COMPR_FL			0x00000004 /* Compress file */
#وصف_ڪريو EXT2_SYNC_FL			0x00000008 /* Synchronous updates */
#وصف_ڪريو EXT2_IMMUTABLE_FL		0x00000010 /* Immutable file */
#وصف_ڪريو EXT2_APPEND_FL			0x00000020 /* writes to file may only append */
#وصف_ڪريو EXT2_NODUMP_FL			0x00000040 /* do not dump file */
#وصف_ڪريو EXT2_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#وصف_ڪريو EXT2_DIRTY_FL			0x00000100
#وصف_ڪريو EXT2_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#وصف_ڪريو EXT2_NOCOMPR_FL			0x00000400 /* Access raw compressed data */
	/* nb: was previously EXT2_ECOMPR_FL */
#وصف_ڪريو EXT4_ENCRYPT_FL			0x00000800 /* encrypted inode */
/* End compression flags --- maybe not all used */
#وصف_ڪريو EXT2_BTREE_FL			0x00001000 /* btree format dir */
#وصف_ڪريو EXT2_اشاريو_FL			0x00001000 /* hash-indexed directory */
#وصف_ڪريو EXT2_IMAGIC_FL			0x00002000
#وصف_ڪريو EXT3_JOURNAL_معلومات_FL		0x00004000 /* file data should be journaled */
#وصف_ڪريو EXT2_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#وصف_ڪريو EXT2_DIRSYNC_FL 		0x00010000 /* Synchronous directory modifications */
#وصف_ڪريو EXT2_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#وصف_ڪريو EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#وصف_ڪريو EXT4_EXTENTS_FL 		0x00080000 /* Inode uses extents */
#وصف_ڪريو EXT4_VERITY_FL			0x00100000 /* Verity protected inode */
#وصف_ڪريو EXT4_EA_INODE_FL	        0x00200000 /* Inode used for large EA */
/* EXT4_EOFBLOCKS_FL 0x00400000 was here */
#وصف_ڪريو FS_NOCOW_FL			0x00800000 /* Do not cow file */
#وصف_ڪريو EXT4_SNAPFILE_FL		0x01000000  /* Inode is a snapshot */
#وصف_ڪريو FS_DAX_FL			0x02000000 /* Inode is DAX */
#وصف_ڪريو EXT4_SNAPFILE_DELETED_FL	0x04000000  /* Snapshot is being deleted */
#وصف_ڪريو EXT4_SNAPFILE_SHRUNK_FL		0x08000000  /* Snapshot shrink has completed */
#وصف_ڪريو EXT4_INLINE_معلومات_FL		0x10000000 /* Inode has inline data */
#وصف_ڪريو EXT4_PROJINHERIT_FL		0x20000000 /* Create with parents projid */
#وصف_ڪريو EXT4_CASEFOLD_FL		0x40000000 /* Casefolded file */
#وصف_ڪريو EXT2_RESERVED_FL		0x80000000 /* reserved for ext2 lib */

#وصف_ڪريو EXT2_FL_USER_VISIBLE		0x604BDFFF /* User visible flags */
#وصف_ڪريو EXT2_FL_USER_MODIFIABLE		0x604B80FF /* User modifiable flags */

/*
 * ioctl commands
 */

/* Used for online resize */
بنية ext2_new_گروهه_داخلا {
	__u32 گروهه;		/* Group number for this data */
	__u32 بلاڪ_bitmap;	/* Absolute block number of block bitmap */
	__u32 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u32 inode_جدول;	/* Absolute block number of inode table start */
	__u32 بلاڪون_ڳڻپ;	/* Total number of blocks in this group */
	__u16 reserved_بلاڪون;	/* Number of reserved blocks in this group */
	__u16 اڻ_استعمال_ٿيل;		/* Number of reserved GDT blocks in group */
};

بنية ext4_new_گروهه_داخلا {
	__u32 گروهه;		/* Group number for this data */
	__u64 بلاڪ_bitmap;	/* Absolute block number of block bitmap */
	__u64 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u64 inode_جدول;	/* Absolute block number of inode table start */
	__u32 بلاڪون_ڳڻپ;	/* Total number of blocks in this group */
	__u16 reserved_بلاڪون;	/* Number of reserved blocks in this group */
	__u16 اڻ_استعمال_ٿيل;
};

#جيڪڏهن_وصف_ٿيل __GNU__			/* Needed for the Hurd */
#وصف_ڪريو IOT_ext2_new_گروهه_داخلا _IOT (_IOTS(__u32), 5, _IOTS(__u16), 2, 0, 0)
#جيڪڏهن_پورو

#وصف_ڪريو EXT2_IOC_GETFLAGS		_IOR('f', 1, ڊگهو)
#وصف_ڪريو EXT2_IOC_SETFLAGS		_IOW('f', 2, ڊگهو)
#وصف_ڪريو EXT2_IOC_GETVERSION		_IOR('v', 1, ڊگهو)
#وصف_ڪريو EXT2_IOC_SETVERSION		_IOW('v', 2, ڊگهو)
#وصف_ڪريو EXT2_IOC_GETVERSION_NEW		_IOR('f', 3, ڊگهو)
#وصف_ڪريو EXT2_IOC_SETVERSION_NEW		_IOW('f', 4, ڊگهو)
#وصف_ڪريو EXT2_IOC_گروهه_EXTEND		_IOW('f', 7, بنانشان ڊگهو)
#وصف_ڪريو EXT2_IOC_گروهه_جوڙيو		_IOW('f', 8,بنية ext2_new_گروهه_داخلا)
#وصف_ڪريو EXT4_IOC_گروهه_جوڙيو		_IOW('f', 8,بنية ext4_new_گروهه_داخلا)
#وصف_ڪريو EXT4_IOC_RESIZE_FS		_IOW('f', 16, __u64)

/*
 * Structure of an inode on the disk
 */
بنية ext2_inode {
/*00*/	__u16	i_گهڻائي;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_سائيز;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_ڳڻپ;	/* Links count */
	__u32	i_بلاڪون;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	اتحاد {
		بنية {
			__u32	l_i_نسخو; /* was l_i_reserved1 */
		} linux1;
		بنية {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_بلاڪ[EXT2_N_بلاڪون];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_سائيز_مٿاهون;
/*70*/	__u32	i_faddr;	/* Fragment address */
	اتحاد {
		بنية {
			__u16	l_i_بلاڪون_hi;
			__u16	l_i_file_acl_مٿاهون;
			__u16	l_i_uid_مٿاهون;	/* these 2 fields    */
			__u16	l_i_gid_مٿاهون;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		بنية {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_گهڻائي_مٿاهون;
			__u16	h_i_uid_مٿاهون;
			__u16	h_i_gid_مٿاهون;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
};

/*
 * Permanent part of an large inode on the disk
 */
بنية ext2_inode_large {
/*00*/	__u16	i_گهڻائي;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_سائيز;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode Change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_ڳڻپ;	/* Links count */
	__u32	i_بلاڪون;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	اتحاد {
		بنية {
			__u32	l_i_نسخو; /* was l_i_reserved1 */
		} linux1;
		بنية {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_بلاڪ[EXT2_N_بلاڪون];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_سائيز_مٿاهون;
/*70*/	__u32	i_faddr;	/* Fragment address */
	اتحاد {
		بنية {
			__u16	l_i_بلاڪون_hi;
			__u16	l_i_file_acl_مٿاهون;
			__u16	l_i_uid_مٿاهون;	/* these 2 fields    */
			__u16	l_i_gid_مٿاهون;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		بنية {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_گهڻائي_مٿاهون;
			__u16	h_i_uid_مٿاهون;
			__u16	h_i_gid_مٿاهون;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
/*80*/	__u16	i_extra_isize;
	__u16	i_checksum_hi;	/* crc32c(uuid+inum+inode) */
	__u32	i_ctime_extra;	/* extra Change time (nsec << 2 | epoch) */
	__u32	i_mtime_extra;	/* extra Modification time (nsec << 2 | epoch) */
	__u32	i_atime_extra;	/* extra Access time (nsec << 2 | epoch) */
/*90*/	__u32	i_crtime;	/* File creation time */
	__u32	i_crtime_extra;	/* extra File creation time (nsec << 2 | epoch)*/
	__u32	i_نسخو_hi;	/* high 32 bits for 64-bit version */
/*9c*/	__u32   i_projid;       /* Project ID */
};

#وصف_ڪريو EXT4_INODE_CSUM_HI_EXTRA_پڇاڙي	\
	(offsetof(بنية ext2_inode_large, i_checksum_hi) + حجم(__u16) - \
	 EXT2_GOOD_OLD_INODE_سائيز)

#وصف_ڪريو EXT4_EPOCH_بٽ 2
#وصف_ڪريو EXT4_EPOCH_نقاب ((1 << EXT4_EPOCH_بٽ) - 1)

#وصف_ڪريو i_checksum_lo	osd2.linux2.l_i_checksum_lo

#وصف_ڪريو ext2fs_inode_includes(سائيز, ميدان)				\
	((سائيز) >= (حجم(((بنية ext2_inode_large *)0)->ميدان) +	\
		    offsetof(بنية ext2_inode_large, ميدان)))

#إذا defined(__KERNEL__) || defined(__linux__)
#وصف_ڪريو i_reserved1	osd1.linux1.l_i_reserved1
#وصف_ڪريو i_frag		osd2.linux2.l_i_frag
#وصف_ڪريو i_fsize		osd2.linux2.l_i_fsize
#وصف_ڪريو i_uid_هيٺيون	i_uid
#وصف_ڪريو i_gid_هيٺيون	i_gid
#وصف_ڪريو i_uid_مٿاهون	osd2.linux2.l_i_uid_مٿاهون
#وصف_ڪريو i_gid_مٿاهون	osd2.linux2.l_i_gid_مٿاهون
#وإلا
#إذا defined(__GNU__)

#وصف_ڪريو i_translator	osd1.hurd1.h_i_translator
#وصف_ڪريو i_frag		osd2.hurd2.h_i_frag;
#وصف_ڪريو i_fsize		osd2.hurd2.h_i_fsize;
#وصف_ڪريو i_uid_مٿاهون	osd2.hurd2.h_i_uid_مٿاهون
#وصف_ڪريو i_gid_مٿاهون	osd2.hurd2.h_i_gid_مٿاهون
#وصف_ڪريو i_author	osd2.hurd2.h_i_author

#جيڪڏهن_پورو  /* __GNU__ */
#جيڪڏهن_پورو	/* defined(__KERNEL__) || defined(__linux__) */

#وصف_ڪريو inode_uid(inode)	((inode).i_uid | (بنانشان)(inode).osd2.linux2.l_i_uid_مٿاهون << 16)
#وصف_ڪريو inode_gid(inode)	((inode).i_gid | (بنانشان)(inode).osd2.linux2.l_i_gid_مٿاهون << 16)
#وصف_ڪريو inode_projid(inode)	((inode).i_projid)
#وصف_ڪريو ext2fs_سيٽ_i_uid_مٿاهون(inode,x) ((inode).osd2.linux2.l_i_uid_مٿاهون = (x))
#وصف_ڪريو ext2fs_سيٽ_i_gid_مٿاهون(inode,x) ((inode).osd2.linux2.l_i_gid_مٿاهون = (x))

ساكن مضمن
بنية ext2_inode *EXT2_INODE(بنية ext2_inode_large *large_inode)
{
	أرجع (بنية ext2_inode *) large_inode;
}

/*
 * File system states
 */
#وصف_ڪريو EXT2_VALID_FS			0x0001	/* Unmounted cleanly */
#وصف_ڪريو EXT2_ERROR_FS			0x0002	/* Errors detected */
#وصف_ڪريو EXT3_ORPHAN_FS			0x0004	/* Orphans being recovered */
#وصف_ڪريو EXT4_FC_REPLAY			0x0020	/* Ext4 fast commit replay ongoing */

/*
 * Misc. filesystem flags
 */
#وصف_ڪريو EXT2_FLAGS_SIGNED_HASH		0x0001  /* Signed dirhash in use */
#وصف_ڪريو EXT2_FLAGS_UNSIGNED_HASH	0x0002  /* Unsigned dirhash in use */
#وصف_ڪريو EXT2_FLAGS_TEST_FILESYS		0x0004	/* OK for use on development code */
#وصف_ڪريو EXT2_FLAGS_IS_SNAPSHOT		0x0010	/* This is a snapshot image */
#وصف_ڪريو EXT2_FLAGS_FIX_SNAPSHOT		0x0020	/* Snapshot inodes corrupted */
#وصف_ڪريو EXT2_FLAGS_FIX_EXCLUDE		0x0040	/* Exclude bitmaps corrupted */

/*
 * Mount flags
 */
#وصف_ڪريو EXT2_MOUNT_CHECK		0x0001	/* Do mount-time checks */
#وصف_ڪريو EXT2_MOUNT_GRPID		0x0004	/* Create files with directory's group */
#وصف_ڪريو EXT2_MOUNT_DEBUG		0x0008	/* Some debugging messages */
#وصف_ڪريو EXT2_MOUNT_ERRORS_CONT		0x0010	/* Continue on errors */
#وصف_ڪريو EXT2_MOUNT_ERRORS_RO		0x0020	/* Remount fs ro on errors */
#وصف_ڪريو EXT2_MOUNT_ERRORS_PANIC		0x0040	/* Panic on errors */
#وصف_ڪريو EXT2_MOUNT_MINIX_DF		0x0080	/* Mimics the Minix statfs */
#وصف_ڪريو EXT2_MOUNT_NO_UID32		0x0200  /* Disable 32-bit UIDs */

#وصف_ڪريو clear_opt(o, opt)		o &= ~EXT2_MOUNT_##opt
#وصف_ڪريو سيٽ_opt(o, opt)			o |= EXT2_MOUNT_##opt
#وصف_ڪريو test_opt(sb, opt)		(EXT2_SB(sb)->s_mount_opt & \
					 EXT2_MOUNT_##opt)
/*
 * Maximal mount counts between two filesystem checks
 */
#وصف_ڪريو EXT2_DFL_MAX_MNT_ڳڻپ		20	/* Allow 20 mounts */
#وصف_ڪريو EXT2_DFL_CHECKINTERVAL		0	/* Don't use interval check */

/*
 * Behaviour when detecting errors
 */
#وصف_ڪريو EXT2_ERRORS_CONTINUE		1	/* Continue execution */
#وصف_ڪريو EXT2_ERRORS_RO			2	/* Remount fs read-only */
#وصف_ڪريو EXT2_ERRORS_PANIC		3	/* Panic */
#وصف_ڪريو EXT2_ERRORS_DEFAULT		EXT2_ERRORS_CONTINUE

#إذا (__GNUC__ >= 4)
#وصف_ڪريو ext4_offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#وإلا
#وصف_ڪريو ext4_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#جيڪڏهن_پورو

/* Metadata checksum algorithms */
#وصف_ڪريو EXT2_چڪري_جاچ_32_C_CHKSUM		1

/* Encryption algorithms, key size and key reference len */
#وصف_ڪريو EXT4_ENCRYPTION_گهڻائي_INVALID		0
#وصف_ڪريو EXT4_ENCRYPTION_گهڻائي_AES_256_XTS	1
#وصف_ڪريو EXT4_ENCRYPTION_گهڻائي_AES_256_GCM	2
#وصف_ڪريو EXT4_ENCRYPTION_گهڻائي_AES_256_CBC	3
#وصف_ڪريو EXT4_ENCRYPTION_گهڻائي_AES_256_CTS	4

#وصف_ڪريو EXT4_AES_256_XTS_ڪنجي_سائيز		64
#وصف_ڪريو EXT4_AES_256_GCM_ڪنجي_سائيز		32
#وصف_ڪريو EXT4_AES_256_CBC_ڪنجي_سائيز		32
#وصف_ڪريو EXT4_AES_256_CTS_ڪنجي_سائيز		32
#وصف_ڪريو EXT4_MAX_ڪنجي_سائيز			64

#وصف_ڪريو EXT4_ڪنجي_DESCRIPTOR_سائيز		8
#وصف_ڪريو EXT4_CRYPTO_بلاڪ_سائيز			16

/* Password derivation constants */
#وصف_ڪريو EXT4_MAX_PASSPHRASE_سائيز		1024
#وصف_ڪريو EXT4_MAX_SALT_سائيز			256
#وصف_ڪريو EXT4_PBKDF2_ITERATIONS			0xFFFF

#وصف_ڪريو EXT2_LABEL_LEN				16

/*
 * Policy provided via an ioctl on the topmost directory. This
 * structure is also in the kernel.
 */
بنية ext4_encryption_policy {
  حرف نسخو;
  حرف contents_encryption_گهڻائي;
  حرف filenames_encryption_گهڻائي;
  حرف flags;
  حرف master_ڪنجي_descriptor[EXT4_ڪنجي_DESCRIPTOR_سائيز];
} __attribute__((__packed__));

بنية ext4_encryption_ڪنجي {
        __u32 گهڻائي;
        حرف raw[EXT4_MAX_ڪنجي_سائيز];
        __u32 سائيز;
} __attribute__((__packed__));

/*
 * Structure of the super block
 */
بنية ext2_super_بلاڪ {
/*000*/	__u32	s_inodes_ڳڻپ;		/* Inodes count */
	__u32	s_بلاڪون_ڳڻپ;		/* Blocks count */
	__u32	s_r_بلاڪون_ڳڻپ;	/* Reserved blocks count */
	__u32	s_free_بلاڪون_ڳڻپ;	/* Free blocks count */
/*010*/	__u32	s_free_inodes_ڳڻپ;	/* Free inodes count */
	__u32	s_پهريون_معلومات_بلاڪ;	/* First Data Block */
	__u32	s_log_بلاڪ_سائيز;	/* Block size */
	__u32	s_log_cluster_سائيز;	/* Allocation cluster size */
/*020*/	__u32	s_بلاڪون_في_گروهه;	/* # Blocks per group */
	__u32	s_clusters_في_گروهه;	/* # Fragments per group */
	__u32	s_inodes_في_گروهه;	/* # Inodes per group */
	__u32	s_mtime;		/* Mount time */
/*030*/	__u32	s_wtime;		/* Write time */
	__u16	s_mnt_ڳڻپ;		/* Mount count */
	__s16	s_max_mnt_ڳڻپ;	/* Maximal mount count */
	__u16	s_magic;		/* Magic signature */
	__u16	s_state;		/* File system state */
	__u16	s_errors;		/* Behaviour when detecting errors */
	__u16	s_minor_rev_level;	/* minor revision level */
/*040*/	__u32	s_lastcheck;		/* time of last check */
	__u32	s_checkinterval;	/* max. time between checks */
	__u32	s_creator_os;		/* OS */
	__u32	s_rev_level;		/* Revision level */
/*050*/	__u16	s_def_resuid;		/* Default uid for reserved blocks */
	__u16	s_def_resgid;		/* Default gid for reserved blocks */
	/*
	 * These fields are for EXT2_DYNAMIC_REV superblocks only.
	 *
	 * Note: the difference between the compatible feature set and
	 * the incompatible feature set is that if there is a bit set
	 * in the incompatible feature set that the kernel doesn't
	 * know about, it should refuse to mount the filesystem.
	 *
	 * e2fsck's requirements are more strict; if it doesn't know
	 * about a feature in either the compatible or incompatible
	 * feature set, it must abort and not try to meddle with
	 * things it doesn't understand...
	 */
	__u32	s_پهريون_ino;		/* First non-reserved inode */
	__u16   s_inode_سائيز;		/* size of inode structure */
	__u16	s_بلاڪ_گروهه_nr;	/* block group # of this superblock */
	__u32	s_feature_compat;	/* compatible feature set */
/*060*/	__u32	s_feature_incompat;	/* incompatible feature set */
	__u32	s_feature_ro_compat;	/* readonly-compatible feature set */
/*068*/	__u8	s_uuid[16] __nonstring;		/* 128-bit uuid for volume */
/*078*/	__u8	s_volume_name[EXT2_LABEL_LEN] __nonstring;	/* volume name, no NUL? */
/*088*/	__u8	s_آخري_mounted[64] __nonstring;	/* directory last mounted on, no NUL? */
/*0c8*/	__u32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_بلاڪون;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_بلاڪون;	/* Nr to preallocate for dirs */
	__u16	s_reserved_gdt_بلاڪون;	/* Per group table for online growth */
	/*
	 * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
/*0d0*/	__u8	s_journal_uuid[16] __nonstring;	/* uuid of journal superblock */
/*0e0*/	__u32	s_journal_inum;		/* inode number of journal file */
	__u32	s_journal_dev;		/* device number of journal file */
	__u32	s_آخري_orphan;		/* start of list of inodes to delete */
/*0ec*/	__u32	s_hash_ٻج[4];		/* HTREE hash seed */
/*0fc*/	__u8	s_def_hash_نسخو;	/* Default hash version to use */
	__u8	s_jnl_backup_type;	/* Default type of journal backup */
	__u16	s_desc_سائيز;		/* Group desc. size: INCOMPAT_64BIT */
/*100*/	__u32	s_default_mount_opts;	/* default EXT2_MOUNT_* flags used */
	__u32	s_پهريون_meta_bg;	/* First metablock group */
	__u32	s_mkfs_وقت;		/* When the filesystem was created */
/*10c*/	__u32	s_jnl_بلاڪون[17];	/* Backup of the journal inode */
/*150*/	__u32	s_بلاڪون_ڳڻپ_hi;	/* Blocks count high 32bits */
	__u32	s_r_بلاڪون_ڳڻپ_hi;	/* Reserved blocks count high 32 bits*/
	__u32	s_free_بلاڪون_hi;	/* Free blocks count */
	__u16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__u16	s_want_extra_isize;	/* New inodes should reserve # bytes */
/*160*/	__u32	s_flags;		/* Miscellaneous flags */
	__u16	s_raid_stride;		/* RAID stride in blocks */
	__u16	s_mmp_update_interval;  /* # seconds to wait in MMP checking */
	__u64	s_mmp_بلاڪ;		/* Block for multi-mount protection */
/*170*/	__u32	s_raid_stripe_ويڪر;	/* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_في_flex;	/* FLEX_BG group size */
	__u8	s_checksum_type;	/* metadata checksum algorithm */
	__u8	s_encryption_level;	/* versioning level for encryption */
	__u8	s_reserved_pad;		/* Padding to next 32bits */
	__u64	s_kbytes_written;	/* nr of lifetime kilobytes written */
/*180*/	__u32	s_snapshot_inum;	/* Inode number of active snapshot */
	__u32	s_snapshot_id;		/* sequential ID of active snapshot */
	__u64	s_snapshot_r_بلاڪون_ڳڻپ; /* active snapshot reserved blocks */
/*190*/	__u32	s_snapshot_list;	/* inode number of disk snapshot list */
#وصف_ڪريو EXT4_S_ERR_شروعات ext4_offsetof(بنية ext2_super_بلاڪ, s_error_ڳڻپ)
	__u32	s_error_ڳڻپ;		/* number of fs errors */
	__u32	s_پهريون_error_وقت;	/* first time an error happened */
	__u32	s_پهريون_error_ino;	/* inode involved in first error */
/*1a0*/	__u64	s_پهريون_error_بلاڪ;	/* block involved in first error */
	__u8	s_پهريون_error_func[32] __nonstring;	/* function where error hit, no NUL? */
/*1c8*/	__u32	s_پهريون_error_سٽ;	/* line number where error happened */
	__u32	s_آخري_error_وقت;	/* most recent time of an error */
/*1d0*/	__u32	s_آخري_error_ino;	/* inode involved in last error */
	__u32	s_آخري_error_سٽ;	/* line number where error happened */
	__u64	s_آخري_error_بلاڪ;	/* block involved of last error */
/*1e0*/	__u8	s_آخري_error_func[32] __nonstring;	/* function where error hit, no NUL? */
#وصف_ڪريو EXT4_S_ERR_پڇاڙي ext4_offsetof(بنية ext2_super_بلاڪ, s_mount_opts)
/*200*/	__u8	s_mount_opts[64] __nonstring;	/* default mount options, no NUL? */
/*240*/	__u32	s_usr_quota_inum;	/* inode number of user quota file */
	__u32	s_grp_quota_inum;	/* inode number of group quota file */
	__u32	s_overhead_clusters;	/* overhead blocks/clusters in fs */
/*24c*/	__u32	s_backup_bgs[2];	/* If sparse_super2 enabled */
/*254*/	__u8	s_encrypt_algos[4];	/* Encryption algorithms in use  */
/*258*/	__u8	s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
/*268*/	__le32	s_lpf_ino;		/* Location of the lost+found inode */
	__le32  s_prj_quota_inum;	/* inode for tracking project quota */
/*270*/	__le32	s_checksum_ٻج;	/* crc32c(orig_uuid) if csum_seed set */
/*274*/	__u8	s_wtime_hi;
	__u8	s_mtime_hi;
	__u8	s_mkfs_وقت_hi;
	__u8	s_lastcheck_hi;
	__u8	s_پهريون_error_وقت_hi;
	__u8	s_آخري_error_وقت_hi;
	__u8	s_پهريون_error_errcode;
	__u8    s_آخري_error_errcode;
/*27c*/ __le16	s_encoding;		/* Filename charset encoding */
	__le16	s_encoding_flags;	/* Filename charset encoding flags */
	__le32  s_orphan_file_inum;	/* Inode for tracking orphan inodes */
	__le32	s_reserved[94];		/* Padding to the end of the block */
/*3fc*/	__u32	s_checksum;		/* crc32c(superblock) */
};

#وصف_ڪريو EXT4_S_ERR_LEN (EXT4_S_ERR_پڇاڙي - EXT4_S_ERR_شروعات)
#وصف_ڪريو EXT2_LEN_STR(buf) (صحيح)حجم(buf), (حرف *)buf

/*
 * Codes for operating systems
 */
#وصف_ڪريو EXT2_OS_LINUX		0
#وصف_ڪريو EXT2_OS_HURD		1
#وصف_ڪريو EXT2_OBSO_OS_MASIX	2
#وصف_ڪريو EXT2_OS_FREEBSD		3
#وصف_ڪريو EXT2_OS_LITES		4

/*
 * Revision levels
 */
#وصف_ڪريو EXT2_GOOD_OLD_REV	0	/* The good old (original) format */
#وصف_ڪريو EXT2_DYNAMIC_REV	1	/* V2 format w/ dynamic inode sizes */

#وصف_ڪريو EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV
#وصف_ڪريو EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV

#وصف_ڪريو EXT2_GOOD_OLD_INODE_سائيز 128

#وصف_ڪريو EXT4_EXTRA_TIMESTAMP_MAX	(((int64_t)1 << 34) - 1  + INT32_MIN)
#وصف_ڪريو EXT4_NON_EXTRA_TIMESTAMP_MAX	INT32_MAX
#وصف_ڪريو EXT4_TIMESTAMP_MIN		INT32_MIN

/*
 * Journal inode backup types
 */
#وصف_ڪريو EXT3_JNL_BACKUP_بلاڪون	1

/*
 * Feature set definitions
 */

#وصف_ڪريو EXT2_HAS_COMPAT_FEATURE(sb,نقاب)			\
	( EXT2_SB(sb)->s_feature_compat & (نقاب) )
#وصف_ڪريو EXT2_HAS_RO_COMPAT_FEATURE(sb,نقاب)			\
	( EXT2_SB(sb)->s_feature_ro_compat & (نقاب) )
#وصف_ڪريو EXT2_HAS_INCOMPAT_FEATURE(sb,نقاب)			\
	( EXT2_SB(sb)->s_feature_incompat & (نقاب) )

#وصف_ڪريو EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001
#وصف_ڪريو EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002
#وصف_ڪريو EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
#وصف_ڪريو EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008
#وصف_ڪريو EXT2_FEATURE_COMPAT_RESIZE_INODE	0x0010
#وصف_ڪريو EXT2_FEATURE_COMPAT_DIR_اشاريو		0x0020
#وصف_ڪريو EXT2_FEATURE_COMPAT_LAZY_BG		0x0040
/* #define EXT2_FEATURE_COMPAT_EXCLUDE_INODE	0x0080 not used, legacy */
#وصف_ڪريو EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP	0x0100
#وصف_ڪريو EXT4_FEATURE_COMPAT_SPARSE_SUPER2	0x0200
#وصف_ڪريو EXT4_FEATURE_COMPAT_FAST_COMMIT		0x0400
#وصف_ڪريو EXT4_FEATURE_COMPAT_STABLE_INODES	0x0800
#وصف_ڪريو EXT4_FEATURE_COMPAT_ORPHAN_FILE		0x1000

#وصف_ڪريو EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#وصف_ڪريو EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004 not used */
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_HUGE_FILE	0x0008
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_GDT_CSUM		0x0010
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_DIR_NLINK	0x0020
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE	0x0040
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT	0x0080
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_QUOTA		0x0100
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_BIGALLOC		0x0200
/*
 * METADATA_CSUM implies GDT_CSUM.  When METADATA_CSUM is set, group
 * descriptor checksums use the same algorithm as all other data
 * structures' checksums.
 */
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_METADATA_CSUM	0x0400
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_REPLICA		0x0800
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_READONLY		0x1000
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_PROJECT		0x2000 /* Project quota */
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_SHARED_بلاڪون	0x4000
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_VERITY		0x8000
#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT	0x10000

#وصف_ڪريو EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001
#وصف_ڪريو EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002
#وصف_ڪريو EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 /* Needs recovery */
#وصف_ڪريو EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 /* Journal device */
#وصف_ڪريو EXT2_FEATURE_INCOMPAT_META_BG		0x0010
#وصف_ڪريو EXT3_FEATURE_INCOMPAT_EXTENTS		0x0040
#وصف_ڪريو EXT4_FEATURE_INCOMPAT_64_ٻٽو_انگ		0x0080
#وصف_ڪريو EXT4_FEATURE_INCOMPAT_MMP		0x0100
#وصف_ڪريو EXT4_FEATURE_INCOMPAT_FLEX_BG		0x0200
#وصف_ڪريو EXT4_FEATURE_INCOMPAT_EA_INODE		0x0400
#وصف_ڪريو EXT4_FEATURE_INCOMPAT_DIRDATA		0x1000
#وصف_ڪريو EXT4_FEATURE_INCOMPAT_CSUM_ٻج		0x2000
#وصف_ڪريو EXT4_FEATURE_INCOMPAT_LARGEDIR		0x4000 /* >2GB or 3-lvl htree */
#وصف_ڪريو EXT4_FEATURE_INCOMPAT_INLINE_معلومات	0x8000 /* data in inode */
#وصف_ڪريو EXT4_FEATURE_INCOMPAT_ENCRYPT		0x10000
#وصف_ڪريو EXT4_FEATURE_INCOMPAT_CASEFOLD		0x20000

#وصف_ڪريو EXT4_FEATURE_COMPAT_FUNCS(name, ver, flagname) \
ساكن مضمن صحيح ext2fs_has_feature_##name(بنية ext2_super_بلاڪ *sb) \
{ \
	أرجع ((EXT2_SB(sb)->s_feature_compat & \
		 EXT##ver##_FEATURE_COMPAT_##flagname) != 0); \
} \
ساكن مضمن فراغ ext2fs_سيٽ_feature##name(بنية ext2_super_بلاڪ *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat |= \
		EXT##ver##_FEATURE_COMPAT_##flagname; \
} \
ساكن مضمن فراغ ext2fs_clear_feature_##name(بنية ext2_super_بلاڪ *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat &= \
		~EXT##ver##_FEATURE_COMPAT_##flagname; \
}

#وصف_ڪريو EXT4_FEATURE_RO_COMPAT_FUNCS(name, ver, flagname) \
ساكن مضمن صحيح ext2fs_has_feature_##name(بنية ext2_super_بلاڪ *sb) \
{ \
	أرجع ((EXT2_SB(sb)->s_feature_ro_compat & \
		 EXT##ver##_FEATURE_RO_COMPAT_##flagname) != 0); \
} \
ساكن مضمن فراغ ext2fs_سيٽ_feature##name(بنية ext2_super_بلاڪ *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat |= \
		EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
} \
ساكن مضمن فراغ ext2fs_clear_feature_##name(بنية ext2_super_بلاڪ *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat &= \
		~EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
}

#وصف_ڪريو EXT4_FEATURE_INCOMPAT_FUNCS(name, ver, flagname) \
ساكن مضمن صحيح ext2fs_has_feature_##name(بنية ext2_super_بلاڪ *sb) \
{ \
	أرجع ((EXT2_SB(sb)->s_feature_incompat & \
		 EXT##ver##_FEATURE_INCOMPAT_##flagname) != 0); \
} \
ساكن مضمن فراغ ext2fs_سيٽ_feature##name(بنية ext2_super_بلاڪ *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat |= \
		EXT##ver##_FEATURE_INCOMPAT_##flagname; \
} \
ساكن مضمن فراغ ext2fs_clear_feature_##name(بنية ext2_super_بلاڪ *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat &= \
		~EXT##ver##_FEATURE_INCOMPAT_##flagname; \
}

EXT4_FEATURE_COMPAT_FUNCS(dir_prealloc,		2, DIR_PREALLOC)
EXT4_FEATURE_COMPAT_FUNCS(imagic_inodes,	2, IMAGIC_INODES)
EXT4_FEATURE_COMPAT_FUNCS(journal,		3, HAS_JOURNAL)
EXT4_FEATURE_COMPAT_FUNCS(xattr,		2, EXT_ATTR)
EXT4_FEATURE_COMPAT_FUNCS(resize_inode,		2, RESIZE_INODE)
EXT4_FEATURE_COMPAT_FUNCS(dir_اشاريو,		2, DIR_اشاريو)
EXT4_FEATURE_COMPAT_FUNCS(lazy_bg,		2, LAZY_BG)
EXT4_FEATURE_COMPAT_FUNCS(exclude_bitmap,	2, EXCLUDE_BITMAP)
EXT4_FEATURE_COMPAT_FUNCS(sparse_super2,	4, SPARSE_SUPER2)
EXT4_FEATURE_COMPAT_FUNCS(fast_commit,		4, FAST_COMMIT)
EXT4_FEATURE_COMPAT_FUNCS(stable_inodes,	4, STABLE_INODES)
EXT4_FEATURE_COMPAT_FUNCS(orphan_file,		4, ORPHAN_FILE)

EXT4_FEATURE_RO_COMPAT_FUNCS(sparse_super,	2, SPARSE_SUPER)
EXT4_FEATURE_RO_COMPAT_FUNCS(large_file,	2, LARGE_FILE)
EXT4_FEATURE_RO_COMPAT_FUNCS(huge_file,		4, HUGE_FILE)
EXT4_FEATURE_RO_COMPAT_FUNCS(gdt_csum,		4, GDT_CSUM)
EXT4_FEATURE_RO_COMPAT_FUNCS(dir_nlink,		4, DIR_NLINK)
EXT4_FEATURE_RO_COMPAT_FUNCS(extra_isize,	4, EXTRA_ISIZE)
EXT4_FEATURE_RO_COMPAT_FUNCS(has_snapshot,	4, HAS_SNAPSHOT)
EXT4_FEATURE_RO_COMPAT_FUNCS(quota,		4, QUOTA)
EXT4_FEATURE_RO_COMPAT_FUNCS(bigalloc,		4, BIGALLOC)
EXT4_FEATURE_RO_COMPAT_FUNCS(metadata_csum,	4, METADATA_CSUM)
EXT4_FEATURE_RO_COMPAT_FUNCS(replica,		4, REPLICA)
EXT4_FEATURE_RO_COMPAT_FUNCS(readonly,		4, READONLY)
EXT4_FEATURE_RO_COMPAT_FUNCS(project,		4, PROJECT)
EXT4_FEATURE_RO_COMPAT_FUNCS(shared_بلاڪون,	4, SHARED_بلاڪون)
EXT4_FEATURE_RO_COMPAT_FUNCS(verity,		4, VERITY)
EXT4_FEATURE_RO_COMPAT_FUNCS(orphan_present,	4, ORPHAN_PRESENT)

EXT4_FEATURE_INCOMPAT_FUNCS(compression,	2, COMPRESSION)
EXT4_FEATURE_INCOMPAT_FUNCS(filetype,		2, FILETYPE)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_needs_recovery,	3, RECOVER)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_dev,	3, JOURNAL_DEV)
EXT4_FEATURE_INCOMPAT_FUNCS(meta_bg,		2, META_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(extents,		3, EXTENTS)
EXT4_FEATURE_INCOMPAT_FUNCS(64ٻٽو_انگ,		4, 64ٻٽو_انگ_2)
EXT4_FEATURE_INCOMPAT_FUNCS(mmp,		4, MMP)
EXT4_FEATURE_INCOMPAT_FUNCS(flex_bg,		4, FLEX_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(ea_inode,		4, EA_INODE)
EXT4_FEATURE_INCOMPAT_FUNCS(dirdata,		4, DIRDATA)
EXT4_FEATURE_INCOMPAT_FUNCS(csum_ٻج,		4, CSUM_ٻج)
EXT4_FEATURE_INCOMPAT_FUNCS(largedir,		4, LARGEDIR)
EXT4_FEATURE_INCOMPAT_FUNCS(inline_معلومات,	4, INLINE_معلومات)
EXT4_FEATURE_INCOMPAT_FUNCS(encrypt,		4, ENCRYPT)
EXT4_FEATURE_INCOMPAT_FUNCS(casefold,		4, CASEFOLD)

#وصف_ڪريو EXT2_FEATURE_COMPAT_SUPP	0
#وصف_ڪريو EXT2_FEATURE_INCOMPAT_SUPP    (EXT2_FEATURE_INCOMPAT_FILETYPE| \
				       EXT4_FEATURE_INCOMPAT_MMP| \
				       EXT4_FEATURE_INCOMPAT_LARGEDIR| \
				       EXT4_FEATURE_INCOMPAT_EA_INODE)
#وصف_ڪريو EXT2_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
					 EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \
					 EXT4_FEATURE_RO_COMPAT_DIR_NLINK| \
					 EXT2_FEATURE_RO_COMPAT_BTREE_DIR| \
					 EXT4_FEATURE_RO_COMPAT_VERITY)

/*
 * Default values for user and/or group using reserved blocks
 */
#وصف_ڪريو EXT2_DEF_RESUID		0
#وصف_ڪريو EXT2_DEF_RESGID		0

/*
 * Default mount options
 */
#وصف_ڪريو EXT2_DEFM_DEBUG		0x0001
#وصف_ڪريو EXT2_DEFM_BSDGROUPS	0x0002
#وصف_ڪريو EXT2_DEFM_XATTR_USER	0x0004
#وصف_ڪريو EXT2_DEFM_ACL		0x0008
#وصف_ڪريو EXT2_DEFM_UID16		0x0010
#وصف_ڪريو EXT3_DEFM_JMODE		0x0060
#وصف_ڪريو EXT3_DEFM_JMODE_معلومات	0x0020
#وصف_ڪريو EXT3_DEFM_JMODE_ORDERED	0x0040
#وصف_ڪريو EXT3_DEFM_JMODE_WBACK	0x0060
#وصف_ڪريو EXT4_DEFM_NOBARRIER	0x0100
#وصف_ڪريو EXT4_DEFM_بلاڪ_VALIDITY 0x0200
#وصف_ڪريو EXT4_DEFM_DISCARD	0x0400
#وصف_ڪريو EXT4_DEFM_NODELALLOC	0x0800

ساكن مضمن صحيح ext4_hash_اندريون_dirent(ثابت بنية ext2_inode *inode)
{
	أرجع (inode->i_flags & EXT4_ENCRYPT_FL) &&
		(inode->i_flags & EXT4_CASEFOLD_FL);
}

/*
 * Structure of a directory entry
 */
#وصف_ڪريو EXT2_NAME_LEN 255

بنية ext2_dir_داخلا {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u16	name_len;		/* Name length */
	حرف	name[EXT2_NAME_LEN];	/* File name */
};

/*
 * The new version of the directory entry.  Since EXT2 structures are
 * stored in intel byte order, and the name_len field could never be
 * bigger than 255 chars, it's safe to reclaim the extra byte for the
 * file_type field.
 *
 * This structure is deprecated due to endian issues. Please use struct
 * ext2_dir_entry and accessor functions
 *   ext2fs_dirent_name_len
 *   ext2fs_dirent_set_name_len
 *   ext2fs_dirent_file_type
 *   ext2fs_dirent_set_file_type
 * to get and set name_len and file_type fields.
 */
بنية ext2_dir_داخلا_2 {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u8	name_len;		/* Name length */
	__u8	file_type;
	حرف	name[EXT2_NAME_LEN];	/* File name */
};

/*
 * Hashes for ext4_dir_entry for casefolded and ecrypted directories.
 * This is located at the first 4 bit aligned location after the name.
 */

بنية ext2_dir_داخلا_hash {
	__le32 hash;
	__le32 minor_hash;
};

#وصف_ڪريو EXT2_DIRENT_HASHES(داخلا) \
	((بنية ext2_dir_داخلا_hash *) &داخلا->name[\
		(ext2fs_dirent_name_len(داخلا) + \
			EXT2_DIR_ڦيرو) & ~EXT2_DIR_ڦيرو])
#وصف_ڪريو EXT2_DIRENT_HASH(داخلا) \
		ext2fs_le32_منزل_cpu(EXT2_DIRENT_HASHES(داخلا)->hash)
#وصف_ڪريو EXT2_DIRENT_MINOR_HASH(داخلا) \
		ext2fs_le32_منزل_cpu(EXT2_DIRENT_HASHES(داخلا)->minor_hash)

/*
 * This is a bogus directory entry at the end of each leaf block that
 * records checksums.
 */
بنية ext2_dir_داخلا_پڇ {
	__u32	det_reserved_zero1;	/* Pretend to be unused */
	__u16	det_rec_len;		/* 12 */
	__u16	det_reserved_name_len;	/* 0xDE00, fake namelen/filetype */
	__u32	det_checksum;		/* crc32c(uuid+inode+dirent) */
};

/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
#وصف_ڪريو EXT2_FT_UNKNOWN		0
#وصف_ڪريو EXT2_FT_REG_FILE	1
#وصف_ڪريو EXT2_FT_DIR		2
#وصف_ڪريو EXT2_FT_CHRDEV		3
#وصف_ڪريو EXT2_FT_BLKDEV		4
#وصف_ڪريو EXT2_FT_FIFO		5
#وصف_ڪريو EXT2_FT_SOCK		6
#وصف_ڪريو EXT2_FT_SYMLINK		7

#وصف_ڪريو EXT2_FT_MAX		8

/*
 * Annoyingly, e2fsprogs always swab16s ext2_dir_entry.name_len, so we
 * have to build ext2_dir_entry_tail with that assumption too.  This
 * constant helps to build the dir_entry_tail to look like it has an
 * "invalid" file type.
 */
#وصف_ڪريو EXT2_DIR_NAME_LEN_CSUM	0xDE00

/*
 * EXT2_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
#وصف_ڪريو EXT2_DIR_داخلا_سرو_LEN	8
#وصف_ڪريو EXT2_DIR_داخلا_HASH_LEN		8
#وصف_ڪريو EXT2_DIR_PAD			4
#وصف_ڪريو EXT2_DIR_ڦيرو			(EXT2_DIR_PAD - 1)
#وصف_ڪريو EXT2_DIR_REC_LEN(name_len) ext2fs_dir_rec_len(name_len, 0)

ساكن مضمن بنانشان صحيح ext2fs_dir_rec_len(__u8 name_len,
						صحيح extended)
{
	صحيح rec_len = (name_len + EXT2_DIR_داخلا_سرو_LEN + EXT2_DIR_ڦيرو);

	rec_len &= ~EXT2_DIR_ڦيرو;
	إذا (extended)
		rec_len += EXT2_DIR_داخلا_HASH_LEN;
	أرجع rec_len;
}

#وصف_ڪريو EXT4_ORPHAN_بلاڪ_MAGIC 0x0b10ca04

/* Structure at the tail of orphan block */
بنية ext4_orphan_بلاڪ_پڇ {
	__le32 ob_magic;
	__le32 ob_checksum;
};

/*
 * Constants for ext4's extended time encoding
 */
#وصف_ڪريو EXT4_EPOCH_بٽ 2
#وصف_ڪريو EXT4_EPOCH_نقاب ((1 << EXT4_EPOCH_بٽ) - 1)
#وصف_ڪريو EXT4_NSEC_نقاب  (~0UL << EXT4_EPOCH_بٽ)

/*
 * This structure is used for multiple mount protection. It is written
 * into the block number saved in the s_mmp_block field in the superblock.
 * Programs that check MMP should assume that if SEQ_FSCK (or any unknown
 * code above SEQ_MAX) is present then it is NOT safe to use the filesystem,
 * regardless of how old the timestamp is.
 *
 * The timestamp in the MMP structure will be updated by e2fsck at some
 * arbitrary intervals (start of passes, after every few groups of inodes
 * in pass1 and pass1b).  There is no guarantee that e2fsck is updating
 * the MMP block in a timely manner, and the updates it does are purely
 * for the convenience of the sysadmin and not for automatic validation.
 *
 * Note: Only the mmp_seq value is used to determine whether the MMP block
 *	is being updated.  The mmp_time, mmp_nodename, and mmp_bdevname
 *	fields are only for informational purposes for the administrator,
 *	due to clock skew between nodes and hostname HA service takeover.
 */
#وصف_ڪريو EXT4_MMP_MAGIC     0x004D4D50U /* ASCII for MMP */
#وصف_ڪريو EXT4_MMP_SEQ_CLEAN 0xFF4D4D50U /* mmp_seq value for clean unmount */
#وصف_ڪريو EXT4_MMP_SEQ_FSCK  0xE24D4D50U /* mmp_seq value when being fscked */
#وصف_ڪريو EXT4_MMP_SEQ_MAX   0xE24D4D4FU /* maximum valid mmp_seq value */

/* Not endian-annotated; it's swapped at read/write time */
بنية mmp_struct {
	__u32	mmp_magic;		/* Magic number for MMP */
	__u32	mmp_seq;		/* Sequence no. updated periodically */
	__u64	mmp_وقت;		/* Time last updated (seconds) */
	__u8	mmp_nodename[64] __nonstring;	/* Node updating MMP block, no NUL? */
	__u8	mmp_bdevname[32] __nonstring;	/* Bdev updating MMP block, no NUL? */
	__u16	mmp_check_interval;	/* Changed mmp_check_interval */
	__u16	mmp_pad1;
	__u32	mmp_pad2[226];
	__u32	mmp_checksum;		/* crc32c(uuid+mmp_block) */
};

/*
 * Default interval for MMP update in seconds.
 */
#وصف_ڪريو EXT4_MMP_UPDATE_INTERVAL	5

/*
 * Maximum interval for MMP update in seconds.
 */
#وصف_ڪريو EXT4_MMP_MAX_UPDATE_INTERVAL	300

/*
 * Minimum interval for MMP checking in seconds.
 */
#وصف_ڪريو EXT4_MMP_MIN_CHECK_INTERVAL     5

/*
 * Minimum size of inline data.
 */
#وصف_ڪريو EXT4_MIN_INLINE_معلومات_سائيز	((حجم(__u32) * EXT2_N_بلاڪون))

/*
 * Size of a parent inode in inline data directory.
 */
#وصف_ڪريو EXT4_INLINE_معلومات_DOTDOT_سائيز	(4)

#وصف_ڪريو EXT4_ENC_UTF8_12_1	1

#وصف_ڪريو EXT4_ENC_STRICT_گهڻائي_FL			(1 << 0) /* Reject invalid sequences */

#جيڪڏهن_پورو	/* _LINUX_EXT2_FS_H */
