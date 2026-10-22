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

#اگر_غیر_تعریف_شدہ _LINUX_EXT2_FS_H
#تعریف_کرو _LINUX_EXT2_FS_H

#شامل <ext2fs/ext2_types.h>		/* Changed from linux/types.h */

#اگر_غیر_تعریف_شدہ __GNUC_PREREQ
#اگر defined(__GNUC__) && defined(__GNUC_MINOR__)
#تعریف_کرو __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ورنہ
#تعریف_کرو __GNUC_PREREQ(maj, min) 0
#اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ __nonstring
#اگر_تعریف_شدہ __has_attribute
#اگر __has_attribute(__nonstring__)
#تعریف_کرو __nonstring                    __attribute__((__nonstring__))
#ورنہ
#تعریف_کرو __nonstring
#اگر_ختم /* __has_attribute(__nonstring__) */
#ورنہ
# define __nonstring
#اگر_ختم /* __has_attribute */
#اگر_ختم /* __nonstring */

/*
 * The second extended filesystem constants/structures
 */

/*
 * Define EXT2FS_DEBUG to produce debug messages
 */
#تعریف_منسوخ EXT2FS_DEBUG

/*
 * Define EXT2_PREALLOCATE to preallocate data blocks for expanding files
 */
#تعریف_کرو EXT2_PREALLOCATE
#تعریف_کرو EXT2_DEFAULT_PREALLOC_بلاکیں	8

/*
 * The second extended file system version
 */
#تعریف_کرو EXT2FS_DATE		"95/08/09"
#تعریف_کرو EXT2_FS_نسخہ		"0.5b"

/*
 * Special inode numbers
 */
#تعریف_کرو EXT2_BAD_INO		 1	/* Bad blocks inode */
#تعریف_کرو EXT2_جڑ_INO		 2	/* Root inode */
#تعریف_کرو EXT4_USR_QUOTA_INO	 3	/* User quota inode */
#تعریف_کرو EXT4_GRP_QUOTA_INO	 4	/* Group quota inode */
#تعریف_کرو EXT2_BOOT_LOADER_INO	 5	/* Boot loader inode */
#تعریف_کرو EXT2_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#تعریف_کرو EXT2_RESIZE_INO		 7	/* Reserved group descriptors inode */
#تعریف_کرو EXT2_JOURNAL_INO	 8	/* Journal inode */
#تعریف_کرو EXT2_EXCLUDE_INO	 9	/* The "exclude" inode, for snapshots */
#تعریف_کرو EXT4_REPLICA_INO	10	/* Used by non-upstream feature */

/* First non-reserved inode for old ext2 filesystems */
#تعریف_کرو EXT2_GOOD_OLD_پہلا_INO	11

/*
 * The second extended file system magic number
 */
#تعریف_کرو EXT2_SUPER_MAGIC	0xEF53

#اگر_تعریف_شدہ __KERNEL__
#تعریف_کرو EXT2_SB(sb)	(&((sb)->u.ext2_sb))
#ورنہ
/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#تعریف_کرو EXT2_SB(sb)	(sb)
#اگر_ختم

/*
 * Maximal count of links to a file
 */
#تعریف_کرو EXT2_LINK_MAX		65000

/*
 * Macro-instructions used to manage several block sizes
 */
#تعریف_کرو EXT2_MIN_بلاک_LOG_سائز		10	/* 1024 */
#تعریف_کرو EXT2_MAX_بلاک_LOG_سائز		16	/* 65536 */
#تعریف_کرو EXT2_MIN_بلاک_سائز	(1 << EXT2_MIN_بلاک_LOG_سائز)
#تعریف_کرو EXT2_MAX_بلاک_سائز	(1 << EXT2_MAX_بلاک_LOG_سائز)
#اگر_تعریف_شدہ __KERNEL__
#تعریف_کرو EXT2_بلاک_سائز(s)	((s)->s_blocksize)
#تعریف_کرو EXT2_بلاک_سائز_بٹ(s)	((s)->s_blocksize_بٹ)
#تعریف_کرو EXT2_ADDR_فی_بلاک_بٹ(s)	(EXT2_SB(s)->addr_فی_بلاک_بٹ)
#تعریف_کرو EXT2_INODE_سائز(s)	(EXT2_SB(s)->s_inode_سائز)
#تعریف_کرو EXT2_پہلا_INO(s)	(EXT2_SB(s)->s_پہلا_ino)
#ورنہ
#تعریف_کرو EXT2_بلاک_سائز(s)	(EXT2_MIN_بلاک_سائز << (s)->s_log_بلاک_سائز)
#تعریف_کرو EXT2_بلاک_سائز_بٹ(s)	((s)->s_log_بلاک_سائز + 10)
#تعریف_کرو EXT2_INODE_سائز(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_INODE_سائز : (s)->s_inode_سائز)
#تعریف_کرو EXT2_پہلا_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_پہلا_INO : (s)->s_پہلا_ino)
#اگر_ختم
#تعریف_کرو EXT2_ADDR_فی_بلاک(s)	(EXT2_بلاک_سائز(s) / حجم(__u32))

/*
 * Macro-instructions used to manage allocation clusters
 */
#تعریف_کرو EXT2_MIN_CLUSTER_LOG_سائز	EXT2_MIN_بلاک_LOG_سائز
#تعریف_کرو EXT2_MAX_CLUSTER_LOG_سائز	29	/* 512MB  */
#تعریف_کرو EXT2_MIN_CLUSTER_سائز		EXT2_MIN_بلاک_سائز
#تعریف_کرو EXT2_MAX_CLUSTER_سائز		(1 << EXT2_MAX_CLUSTER_LOG_سائز)
#تعریف_کرو EXT2_CLUSTER_سائز(s)		(EXT2_MIN_بلاک_سائز << \
						(s)->s_log_cluster_سائز)
#تعریف_کرو EXT2_CLUSTER_سائز_بٹ(s)	((s)->s_log_cluster_سائز + 10)

/*
 * Macro-instructions used to manage fragments
 *
 * Note: for backwards compatibility only, for the dump program.
 * Ext2/3/4 will never support fragments....
 */
#تعریف_کرو EXT2_MIN_FRAG_سائز              EXT2_MIN_بلاک_سائز
#تعریف_کرو EXT2_MAX_FRAG_سائز              EXT2_MAX_بلاک_سائز
#تعریف_کرو EXT2_MIN_FRAG_LOG_سائز          EXT2_MIN_بلاک_LOG_سائز
#تعریف_کرو EXT2_FRAG_سائز(s)		EXT2_بلاک_سائز(s)
#تعریف_کرو EXT2_FRAGS_فی_بلاک(s)		1

/*
 * ACL structures
 */
ساخت ext2_acl_سرخی	/* Header of Access Control Lists */
{
	__u32	aclh_سائز;
	__u32	aclh_file_گنتی;
	__u32	aclh_acle_گنتی;
	__u32	aclh_پہلا_acle;
};

ساخت ext2_acl_اندراج	/* Access Control List Entry */
{
	__u32	acle_سائز;
	__u16	acle_perms;	/* Access permissions */
	__u16	acle_type;	/* Type of entry */
	__u16	acle_لیبل;	/* User or group identity */
	__u16	acle_pad1;
	__u32	acle_اگلا;	/* Pointer on next entry for the */
					/* same inode or on next free entry */
};

/*
 * Structure of a blocks group descriptor
 */
ساخت ext2_گروہ_desc
{
	__u32	bg_بلاک_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_جدول;		/* Inodes table block */
	__u16	bg_free_بلاکیں_گنتی;	/* Free blocks count */
	__u16	bg_free_inodes_گنتی;	/* Free inodes count */
	__u16	bg_مستعمل_dirs_گنتی;	/* Directories count */
	__u16	bg_flags;
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_بلاک_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_غیر_مستعمل;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(s_uuid+group_num+group_desc)*/
};

/*
 * Structure of a blocks group descriptor
 */
ساخت ext4_گروہ_desc
{
	__u32	bg_بلاک_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_جدول;		/* Inodes table block */
	__u16	bg_free_بلاکیں_گنتی;	/* Free blocks count */
	__u16	bg_free_inodes_گنتی;	/* Free inodes count */
	__u16	bg_مستعمل_dirs_گنتی;	/* Directories count */
	__u16	bg_flags;		/* EXT4_BG_flags (INODE_UNINIT, etc) */
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_بلاک_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_غیر_مستعمل;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(sb_uuid+group+desc) */
	__u32	bg_بلاک_bitmap_hi;	/* Blocks bitmap block MSB */
	__u32	bg_inode_bitmap_hi;	/* Inodes bitmap block MSB */
	__u32	bg_inode_جدول_hi;	/* Inodes table block MSB */
	__u16	bg_free_بلاکیں_گنتی_hi;/* Free blocks count MSB */
	__u16	bg_free_inodes_گنتی_hi;/* Free inodes count MSB */
	__u16	bg_مستعمل_dirs_گنتی_hi;	/* Directories count MSB */
	__u16	bg_itable_غیر_مستعمل_hi;	/* Unused inodes count MSB */
	__u32	bg_exclude_bitmap_hi;	/* Exclude bitmap block MSB */
	__u16	bg_بلاک_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u16	bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u32	bg_reserved;
};

#تعریف_کرو EXT4_BG_INODE_BITMAP_CSUM_HI_انتہا	\
	(offsetof(ساخت ext4_گروہ_desc, bg_inode_bitmap_csum_hi) + \
	 حجم(__u16))
#تعریف_کرو EXT4_BG_بلاک_BITMAP_CSUM_HI_LOCATION	\
	(offsetof(ساخت ext4_گروہ_desc, bg_بلاک_bitmap_csum_hi) + \
	 حجم(__u16))

#تعریف_کرو EXT2_BG_INODE_UNINIT	0x0001 /* Inode table/bitmap not initialized */
#تعریف_کرو EXT2_BG_بلاک_UNINIT	0x0002 /* Block bitmap not initialized */
#تعریف_کرو EXT2_BG_INODE_ZEROED	0x0004 /* On-disk itable initialized to zero */

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
ساخت ext2_dx_جڑ_info {
	__u32 reserved_zero;
	__u8 hash_نسخہ; /* 0 now, 1 at release */
	__u8 info_لمبائی; /* 8 */
	__u8 بالواسطہ_سطحیں;
	__u8 غیر_مستعمل_flags;
};

#تعریف_کرو EXT2_HASH_LEGACY		0
#تعریف_کرو EXT2_HASH_HALF_MD4		1
#تعریف_کرو EXT2_HASH_TEA			2
#تعریف_کرو EXT2_HASH_LEGACY_UNSIGNED	3 /* reserved for userspace lib */
#تعریف_کرو EXT2_HASH_HALF_MD4_UNSIGNED	4 /* reserved for userspace lib */
#تعریف_کرو EXT2_HASH_TEA_UNSIGNED		5 /* reserved for userspace lib */
#تعریف_کرو EXT2_HASH_SIPHASH		6

#تعریف_کرو EXT2_HASH_FLAG_INCOMPAT	0x1

#تعریف_کرو EXT4_DX_بلاک_نقاب 0x0fffffff

ساخت ext2_dx_اندراج {
	__le32 hash;
	__le32 بلاک;
};

ساخت ext2_dx_countlimit {
	__le16 حد_بندی;
	__le16 گنتی;
};

/*
 * This goes at the end of each htree block.
 */
ساخت ext2_dx_دم {
	__le32 dt_reserved;
	__le32 dt_checksum;	/* crc32c(uuid+inum+dxblock) */
};

/*
 * Macro-instructions used to manage group descriptors
 */
#تعریف_کرو EXT2_MIN_DESC_سائز             32
#تعریف_کرو EXT2_MIN_DESC_سائز_64_ثنائی_ہندسہ       64
#تعریف_کرو EXT2_MAX_DESC_سائز             EXT2_MIN_بلاک_سائز
#تعریف_کرو EXT2_DESC_سائز(s)                                                \
	(ext2fs_has_feature_64bit(s) ? (s)->s_desc_سائز : EXT2_MIN_DESC_سائز)

#تعریف_کرو EXT2_بلاکیں_فی_گروہ(s)	(EXT2_SB(s)->s_بلاکیں_فی_گروہ)
#تعریف_کرو EXT2_INODES_فی_گروہ(s)	(EXT2_SB(s)->s_inodes_فی_گروہ)
#تعریف_کرو EXT2_CLUSTERS_فی_گروہ(s)	(EXT2_SB(s)->s_clusters_فی_گروہ)
#تعریف_کرو EXT2_INODES_فی_بلاک(s)	(EXT2_بلاک_سائز(s)/EXT2_INODE_سائز(s))
/* limits imposed by 16-bit value gd_free_{blocks,inode}_count */
#تعریف_کرو EXT2_MAX_بلاکیں_فی_گروہ(s)	((((غیرعلامتی) 1 << 16) - 8) *	\
					 (EXT2_CLUSTER_سائز(s) / \
					  EXT2_بلاک_سائز(s)))
#تعریف_کرو EXT2_MAX_CLUSTERS_فی_گروہ(s)	(((غیرعلامتی) 1 << 16) - 8)
#تعریف_کرو EXT2_MAX_INODES_فی_گروہ(s)	(((غیرعلامتی) 1 << 16) - \
					 EXT2_INODES_فی_بلاک(s))
#اگر_تعریف_شدہ __KERNEL__
#تعریف_کرو EXT2_DESC_فی_بلاک(s)		(EXT2_SB(s)->s_desc_فی_بلاک)
#تعریف_کرو EXT2_DESC_فی_بلاک_بٹ(s)	(EXT2_SB(s)->s_desc_فی_بلاک_بٹ)
#ورنہ
#تعریف_کرو EXT2_DESC_فی_بلاک(s)		(EXT2_بلاک_سائز(s) / EXT2_DESC_سائز(s))
#اگر_ختم

#تعریف_کرو EXT2_GROUPS_مقصد_بلاکیں(s, g)   ((blk64_t) EXT2_بلاکیں_فی_گروہ(s) * \
				       (g))
#تعریف_کرو EXT2_GROUPS_مقصد_CLUSTERS(s, g) ((blk64_t) EXT2_CLUSTERS_فی_گروہ(s) * \
				       (g))

/*
 * Constants relative to the data blocks
 */
#تعریف_کرو EXT2_NDIR_بلاکیں		12
#تعریف_کرو EXT2_IND_بلاک			EXT2_NDIR_بلاکیں
#تعریف_کرو EXT2_DIND_بلاک			(EXT2_IND_بلاک + 1)
#تعریف_کرو EXT2_TIND_بلاک			(EXT2_DIND_بلاک + 1)
#تعریف_کرو EXT2_N_بلاکیں			(EXT2_TIND_بلاک + 1)

/*
 * Inode flags
 */
#تعریف_کرو EXT2_SECRM_FL			0x00000001 /* Secure deletion */
#تعریف_کرو EXT2_UNRM_FL			0x00000002 /* Undelete */
#تعریف_کرو EXT2_COMPR_FL			0x00000004 /* Compress file */
#تعریف_کرو EXT2_SYNC_FL			0x00000008 /* Synchronous updates */
#تعریف_کرو EXT2_IMMUTABLE_FL		0x00000010 /* Immutable file */
#تعریف_کرو EXT2_APPEND_FL			0x00000020 /* writes to file may only append */
#تعریف_کرو EXT2_NODUMP_FL			0x00000040 /* do not dump file */
#تعریف_کرو EXT2_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#تعریف_کرو EXT2_DIRTY_FL			0x00000100
#تعریف_کرو EXT2_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#تعریف_کرو EXT2_NOCOMPR_FL			0x00000400 /* Access raw compressed data */
	/* nb: was previously EXT2_ECOMPR_FL */
#تعریف_کرو EXT4_ENCRYPT_FL			0x00000800 /* encrypted inode */
/* End compression flags --- maybe not all used */
#تعریف_کرو EXT2_BTREE_FL			0x00001000 /* btree format dir */
#تعریف_کرو EXT2_اشاریہ_FL			0x00001000 /* hash-indexed directory */
#تعریف_کرو EXT2_IMAGIC_FL			0x00002000
#تعریف_کرو EXT3_JOURNAL_ڈیٹا_FL		0x00004000 /* file data should be journaled */
#تعریف_کرو EXT2_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#تعریف_کرو EXT2_DIRSYNC_FL 		0x00010000 /* Synchronous directory modifications */
#تعریف_کرو EXT2_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#تعریف_کرو EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#تعریف_کرو EXT4_EXTENTS_FL 		0x00080000 /* Inode uses extents */
#تعریف_کرو EXT4_VERITY_FL			0x00100000 /* Verity protected inode */
#تعریف_کرو EXT4_EA_INODE_FL	        0x00200000 /* Inode used for large EA */
/* EXT4_EOFBLOCKS_FL 0x00400000 was here */
#تعریف_کرو FS_NOCOW_FL			0x00800000 /* Do not cow file */
#تعریف_کرو EXT4_SNAPFILE_FL		0x01000000  /* Inode is a snapshot */
#تعریف_کرو FS_DAX_FL			0x02000000 /* Inode is DAX */
#تعریف_کرو EXT4_SNAPFILE_DELETED_FL	0x04000000  /* Snapshot is being deleted */
#تعریف_کرو EXT4_SNAPFILE_SHRUNK_FL		0x08000000  /* Snapshot shrink has completed */
#تعریف_کرو EXT4_INLINE_ڈیٹا_FL		0x10000000 /* Inode has inline data */
#تعریف_کرو EXT4_PROJINHERIT_FL		0x20000000 /* Create with parents projid */
#تعریف_کرو EXT4_CASEFOLD_FL		0x40000000 /* Casefolded file */
#تعریف_کرو EXT2_RESERVED_FL		0x80000000 /* reserved for ext2 lib */

#تعریف_کرو EXT2_FL_USER_VISIBLE		0x604BDFFF /* User visible flags */
#تعریف_کرو EXT2_FL_USER_MODIFIABLE		0x604B80FF /* User modifiable flags */

/*
 * ioctl commands
 */

/* Used for online resize */
ساخت ext2_new_گروہ_داخلہ {
	__u32 گروہ;		/* Group number for this data */
	__u32 بلاک_bitmap;	/* Absolute block number of block bitmap */
	__u32 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u32 inode_جدول;	/* Absolute block number of inode table start */
	__u32 بلاکیں_گنتی;	/* Total number of blocks in this group */
	__u16 reserved_بلاکیں;	/* Number of reserved blocks in this group */
	__u16 غیر_مستعمل;		/* Number of reserved GDT blocks in group */
};

ساخت ext4_new_گروہ_داخلہ {
	__u32 گروہ;		/* Group number for this data */
	__u64 بلاک_bitmap;	/* Absolute block number of block bitmap */
	__u64 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u64 inode_جدول;	/* Absolute block number of inode table start */
	__u32 بلاکیں_گنتی;	/* Total number of blocks in this group */
	__u16 reserved_بلاکیں;	/* Number of reserved blocks in this group */
	__u16 غیر_مستعمل;
};

#اگر_تعریف_شدہ __GNU__			/* Needed for the Hurd */
#تعریف_کرو IOT_ext2_new_گروہ_داخلہ _IOT (_IOTS(__u32), 5, _IOTS(__u16), 2, 0, 0)
#اگر_ختم

#تعریف_کرو EXT2_IOC_GETFLAGS		_IOR('f', 1, طویل)
#تعریف_کرو EXT2_IOC_SETFLAGS		_IOW('f', 2, طویل)
#تعریف_کرو EXT2_IOC_GETVERSION		_IOR('v', 1, طویل)
#تعریف_کرو EXT2_IOC_SETVERSION		_IOW('v', 2, طویل)
#تعریف_کرو EXT2_IOC_GETVERSION_NEW		_IOR('f', 3, طویل)
#تعریف_کرو EXT2_IOC_SETVERSION_NEW		_IOW('f', 4, طویل)
#تعریف_کرو EXT2_IOC_گروہ_EXTEND		_IOW('f', 7, غیرعلامتی طویل)
#تعریف_کرو EXT2_IOC_گروہ_جوڑو		_IOW('f', 8,ساخت ext2_new_گروہ_داخلہ)
#تعریف_کرو EXT4_IOC_گروہ_جوڑو		_IOW('f', 8,ساخت ext4_new_گروہ_داخلہ)
#تعریف_کرو EXT4_IOC_RESIZE_FS		_IOW('f', 16, __u64)

/*
 * Structure of an inode on the disk
 */
ساخت ext2_inode {
/*00*/	__u16	i_منوال;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_سائز;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_گنتی;	/* Links count */
	__u32	i_بلاکیں;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	اتحاد {
		ساخت {
			__u32	l_i_نسخہ; /* was l_i_reserved1 */
		} linux1;
		ساخت {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_بلاک[EXT2_N_بلاکیں];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_سائز_بلند;
/*70*/	__u32	i_faddr;	/* Fragment address */
	اتحاد {
		ساخت {
			__u16	l_i_بلاکیں_hi;
			__u16	l_i_file_acl_بلند;
			__u16	l_i_uid_بلند;	/* these 2 fields    */
			__u16	l_i_gid_بلند;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		ساخت {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_منوال_بلند;
			__u16	h_i_uid_بلند;
			__u16	h_i_gid_بلند;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
};

/*
 * Permanent part of an large inode on the disk
 */
ساخت ext2_inode_large {
/*00*/	__u16	i_منوال;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_سائز;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode Change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_گنتی;	/* Links count */
	__u32	i_بلاکیں;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	اتحاد {
		ساخت {
			__u32	l_i_نسخہ; /* was l_i_reserved1 */
		} linux1;
		ساخت {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_بلاک[EXT2_N_بلاکیں];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_سائز_بلند;
/*70*/	__u32	i_faddr;	/* Fragment address */
	اتحاد {
		ساخت {
			__u16	l_i_بلاکیں_hi;
			__u16	l_i_file_acl_بلند;
			__u16	l_i_uid_بلند;	/* these 2 fields    */
			__u16	l_i_gid_بلند;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		ساخت {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_منوال_بلند;
			__u16	h_i_uid_بلند;
			__u16	h_i_gid_بلند;
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
	__u32	i_نسخہ_hi;	/* high 32 bits for 64-bit version */
/*9c*/	__u32   i_projid;       /* Project ID */
};

#تعریف_کرو EXT4_INODE_CSUM_HI_EXTRA_انتہا	\
	(offsetof(ساخت ext2_inode_large, i_checksum_hi) + حجم(__u16) - \
	 EXT2_GOOD_OLD_INODE_سائز)

#تعریف_کرو EXT4_EPOCH_بٹ 2
#تعریف_کرو EXT4_EPOCH_نقاب ((1 << EXT4_EPOCH_بٹ) - 1)

#تعریف_کرو i_checksum_lo	osd2.linux2.l_i_checksum_lo

#تعریف_کرو ext2fs_inode_includes(سائز, میدان)				\
	((سائز) >= (حجم(((ساخت ext2_inode_large *)0)->میدان) +	\
		    offsetof(ساخت ext2_inode_large, میدان)))

#اگر defined(__KERNEL__) || defined(__linux__)
#تعریف_کرو i_reserved1	osd1.linux1.l_i_reserved1
#تعریف_کرو i_frag		osd2.linux2.l_i_frag
#تعریف_کرو i_fsize		osd2.linux2.l_i_fsize
#تعریف_کرو i_uid_کم	i_uid
#تعریف_کرو i_gid_کم	i_gid
#تعریف_کرو i_uid_بلند	osd2.linux2.l_i_uid_بلند
#تعریف_کرو i_gid_بلند	osd2.linux2.l_i_gid_بلند
#ورنہ
#اگر defined(__GNU__)

#تعریف_کرو i_translator	osd1.hurd1.h_i_translator
#تعریف_کرو i_frag		osd2.hurd2.h_i_frag;
#تعریف_کرو i_fsize		osd2.hurd2.h_i_fsize;
#تعریف_کرو i_uid_بلند	osd2.hurd2.h_i_uid_بلند
#تعریف_کرو i_gid_بلند	osd2.hurd2.h_i_gid_بلند
#تعریف_کرو i_author	osd2.hurd2.h_i_author

#اگر_ختم  /* __GNU__ */
#اگر_ختم	/* defined(__KERNEL__) || defined(__linux__) */

#تعریف_کرو inode_uid(inode)	((inode).i_uid | (غیرعلامتی)(inode).osd2.linux2.l_i_uid_بلند << 16)
#تعریف_کرو inode_gid(inode)	((inode).i_gid | (غیرعلامتی)(inode).osd2.linux2.l_i_gid_بلند << 16)
#تعریف_کرو inode_projid(inode)	((inode).i_projid)
#تعریف_کرو ext2fs_سیٹ_i_uid_بلند(inode,x) ((inode).osd2.linux2.l_i_uid_بلند = (x))
#تعریف_کرو ext2fs_سیٹ_i_gid_بلند(inode,x) ((inode).osd2.linux2.l_i_gid_بلند = (x))

ساکن اندرونی
ساخت ext2_inode *EXT2_INODE(ساخت ext2_inode_large *large_inode)
{
	واپس (ساخت ext2_inode *) large_inode;
}

/*
 * File system states
 */
#تعریف_کرو EXT2_VALID_FS			0x0001	/* Unmounted cleanly */
#تعریف_کرو EXT2_ERROR_FS			0x0002	/* Errors detected */
#تعریف_کرو EXT3_ORPHAN_FS			0x0004	/* Orphans being recovered */
#تعریف_کرو EXT4_FC_REPLAY			0x0020	/* Ext4 fast commit replay ongoing */

/*
 * Misc. filesystem flags
 */
#تعریف_کرو EXT2_FLAGS_SIGNED_HASH		0x0001  /* Signed dirhash in use */
#تعریف_کرو EXT2_FLAGS_UNSIGNED_HASH	0x0002  /* Unsigned dirhash in use */
#تعریف_کرو EXT2_FLAGS_TEST_FILESYS		0x0004	/* OK for use on development code */
#تعریف_کرو EXT2_FLAGS_IS_SNAPSHOT		0x0010	/* This is a snapshot image */
#تعریف_کرو EXT2_FLAGS_FIX_SNAPSHOT		0x0020	/* Snapshot inodes corrupted */
#تعریف_کرو EXT2_FLAGS_FIX_EXCLUDE		0x0040	/* Exclude bitmaps corrupted */

/*
 * Mount flags
 */
#تعریف_کرو EXT2_MOUNT_CHECK		0x0001	/* Do mount-time checks */
#تعریف_کرو EXT2_MOUNT_GRPID		0x0004	/* Create files with directory's group */
#تعریف_کرو EXT2_MOUNT_DEBUG		0x0008	/* Some debugging messages */
#تعریف_کرو EXT2_MOUNT_ERRORS_CONT		0x0010	/* Continue on errors */
#تعریف_کرو EXT2_MOUNT_ERRORS_RO		0x0020	/* Remount fs ro on errors */
#تعریف_کرو EXT2_MOUNT_ERRORS_PANIC		0x0040	/* Panic on errors */
#تعریف_کرو EXT2_MOUNT_MINIX_DF		0x0080	/* Mimics the Minix statfs */
#تعریف_کرو EXT2_MOUNT_NO_UID32		0x0200  /* Disable 32-bit UIDs */

#تعریف_کرو clear_opt(o, opt)		o &= ~EXT2_MOUNT_##opt
#تعریف_کرو سیٹ_opt(o, opt)			o |= EXT2_MOUNT_##opt
#تعریف_کرو test_opt(sb, opt)		(EXT2_SB(sb)->s_mount_opt & \
					 EXT2_MOUNT_##opt)
/*
 * Maximal mount counts between two filesystem checks
 */
#تعریف_کرو EXT2_DFL_MAX_MNT_گنتی		20	/* Allow 20 mounts */
#تعریف_کرو EXT2_DFL_CHECKINTERVAL		0	/* Don't use interval check */

/*
 * Behaviour when detecting errors
 */
#تعریف_کرو EXT2_ERRORS_CONTINUE		1	/* Continue execution */
#تعریف_کرو EXT2_ERRORS_RO			2	/* Remount fs read-only */
#تعریف_کرو EXT2_ERRORS_PANIC		3	/* Panic */
#تعریف_کرو EXT2_ERRORS_DEFAULT		EXT2_ERRORS_CONTINUE

#اگر (__GNUC__ >= 4)
#تعریف_کرو ext4_offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#ورنہ
#تعریف_کرو ext4_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#اگر_ختم

/* Metadata checksum algorithms */
#تعریف_کرو EXT2_چکری_جانچ_32_C_CHKSUM		1

/* Encryption algorithms, key size and key reference len */
#تعریف_کرو EXT4_ENCRYPTION_منوال_INVALID		0
#تعریف_کرو EXT4_ENCRYPTION_منوال_AES_256_XTS	1
#تعریف_کرو EXT4_ENCRYPTION_منوال_AES_256_GCM	2
#تعریف_کرو EXT4_ENCRYPTION_منوال_AES_256_CBC	3
#تعریف_کرو EXT4_ENCRYPTION_منوال_AES_256_CTS	4

#تعریف_کرو EXT4_AES_256_XTS_کلید_سائز		64
#تعریف_کرو EXT4_AES_256_GCM_کلید_سائز		32
#تعریف_کرو EXT4_AES_256_CBC_کلید_سائز		32
#تعریف_کرو EXT4_AES_256_CTS_کلید_سائز		32
#تعریف_کرو EXT4_MAX_کلید_سائز			64

#تعریف_کرو EXT4_کلید_DESCRIPTOR_سائز		8
#تعریف_کرو EXT4_CRYPTO_بلاک_سائز			16

/* Password derivation constants */
#تعریف_کرو EXT4_MAX_PASSPHRASE_سائز		1024
#تعریف_کرو EXT4_MAX_SALT_سائز			256
#تعریف_کرو EXT4_PBKDF2_ITERATIONS			0xFFFF

#تعریف_کرو EXT2_LABEL_LEN				16

/*
 * Policy provided via an ioctl on the topmost directory. This
 * structure is also in the kernel.
 */
ساخت ext4_encryption_policy {
  حرف نسخہ;
  حرف contents_encryption_منوال;
  حرف filenames_encryption_منوال;
  حرف flags;
  حرف master_کلید_descriptor[EXT4_کلید_DESCRIPTOR_سائز];
} __attribute__((__packed__));

ساخت ext4_encryption_کلید {
        __u32 منوال;
        حرف raw[EXT4_MAX_کلید_سائز];
        __u32 سائز;
} __attribute__((__packed__));

/*
 * Structure of the super block
 */
ساخت ext2_super_بلاک {
/*000*/	__u32	s_inodes_گنتی;		/* Inodes count */
	__u32	s_بلاکیں_گنتی;		/* Blocks count */
	__u32	s_r_بلاکیں_گنتی;	/* Reserved blocks count */
	__u32	s_free_بلاکیں_گنتی;	/* Free blocks count */
/*010*/	__u32	s_free_inodes_گنتی;	/* Free inodes count */
	__u32	s_پہلا_ڈیٹا_بلاک;	/* First Data Block */
	__u32	s_log_بلاک_سائز;	/* Block size */
	__u32	s_log_cluster_سائز;	/* Allocation cluster size */
/*020*/	__u32	s_بلاکیں_فی_گروہ;	/* # Blocks per group */
	__u32	s_clusters_فی_گروہ;	/* # Fragments per group */
	__u32	s_inodes_فی_گروہ;	/* # Inodes per group */
	__u32	s_mtime;		/* Mount time */
/*030*/	__u32	s_wtime;		/* Write time */
	__u16	s_mnt_گنتی;		/* Mount count */
	__s16	s_max_mnt_گنتی;	/* Maximal mount count */
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
	__u32	s_پہلا_ino;		/* First non-reserved inode */
	__u16   s_inode_سائز;		/* size of inode structure */
	__u16	s_بلاک_گروہ_nr;	/* block group # of this superblock */
	__u32	s_feature_compat;	/* compatible feature set */
/*060*/	__u32	s_feature_incompat;	/* incompatible feature set */
	__u32	s_feature_ro_compat;	/* readonly-compatible feature set */
/*068*/	__u8	s_uuid[16] __nonstring;		/* 128-bit uuid for volume */
/*078*/	__u8	s_volume_name[EXT2_LABEL_LEN] __nonstring;	/* volume name, no NUL? */
/*088*/	__u8	s_آخری_mounted[64] __nonstring;	/* directory last mounted on, no NUL? */
/*0c8*/	__u32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_بلاکیں;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_بلاکیں;	/* Nr to preallocate for dirs */
	__u16	s_reserved_gdt_بلاکیں;	/* Per group table for online growth */
	/*
	 * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
/*0d0*/	__u8	s_journal_uuid[16] __nonstring;	/* uuid of journal superblock */
/*0e0*/	__u32	s_journal_inum;		/* inode number of journal file */
	__u32	s_journal_dev;		/* device number of journal file */
	__u32	s_آخری_orphan;		/* start of list of inodes to delete */
/*0ec*/	__u32	s_hash_بیج[4];		/* HTREE hash seed */
/*0fc*/	__u8	s_def_hash_نسخہ;	/* Default hash version to use */
	__u8	s_jnl_backup_type;	/* Default type of journal backup */
	__u16	s_desc_سائز;		/* Group desc. size: INCOMPAT_64BIT */
/*100*/	__u32	s_default_mount_opts;	/* default EXT2_MOUNT_* flags used */
	__u32	s_پہلا_meta_bg;	/* First metablock group */
	__u32	s_mkfs_وقت;		/* When the filesystem was created */
/*10c*/	__u32	s_jnl_بلاکیں[17];	/* Backup of the journal inode */
/*150*/	__u32	s_بلاکیں_گنتی_hi;	/* Blocks count high 32bits */
	__u32	s_r_بلاکیں_گنتی_hi;	/* Reserved blocks count high 32 bits*/
	__u32	s_free_بلاکیں_hi;	/* Free blocks count */
	__u16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__u16	s_want_extra_isize;	/* New inodes should reserve # bytes */
/*160*/	__u32	s_flags;		/* Miscellaneous flags */
	__u16	s_raid_stride;		/* RAID stride in blocks */
	__u16	s_mmp_update_interval;  /* # seconds to wait in MMP checking */
	__u64	s_mmp_بلاک;		/* Block for multi-mount protection */
/*170*/	__u32	s_raid_stripe_چوڑائی;	/* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_فی_flex;	/* FLEX_BG group size */
	__u8	s_checksum_type;	/* metadata checksum algorithm */
	__u8	s_encryption_level;	/* versioning level for encryption */
	__u8	s_reserved_pad;		/* Padding to next 32bits */
	__u64	s_kbytes_written;	/* nr of lifetime kilobytes written */
/*180*/	__u32	s_snapshot_inum;	/* Inode number of active snapshot */
	__u32	s_snapshot_id;		/* sequential ID of active snapshot */
	__u64	s_snapshot_r_بلاکیں_گنتی; /* active snapshot reserved blocks */
/*190*/	__u32	s_snapshot_list;	/* inode number of disk snapshot list */
#تعریف_کرو EXT4_S_ERR_آغاز ext4_offsetof(ساخت ext2_super_بلاک, s_error_گنتی)
	__u32	s_error_گنتی;		/* number of fs errors */
	__u32	s_پہلا_error_وقت;	/* first time an error happened */
	__u32	s_پہلا_error_ino;	/* inode involved in first error */
/*1a0*/	__u64	s_پہلا_error_بلاک;	/* block involved in first error */
	__u8	s_پہلا_error_func[32] __nonstring;	/* function where error hit, no NUL? */
/*1c8*/	__u32	s_پہلا_error_سطر;	/* line number where error happened */
	__u32	s_آخری_error_وقت;	/* most recent time of an error */
/*1d0*/	__u32	s_آخری_error_ino;	/* inode involved in last error */
	__u32	s_آخری_error_سطر;	/* line number where error happened */
	__u64	s_آخری_error_بلاک;	/* block involved of last error */
/*1e0*/	__u8	s_آخری_error_func[32] __nonstring;	/* function where error hit, no NUL? */
#تعریف_کرو EXT4_S_ERR_انتہا ext4_offsetof(ساخت ext2_super_بلاک, s_mount_opts)
/*200*/	__u8	s_mount_opts[64] __nonstring;	/* default mount options, no NUL? */
/*240*/	__u32	s_usr_quota_inum;	/* inode number of user quota file */
	__u32	s_grp_quota_inum;	/* inode number of group quota file */
	__u32	s_overhead_clusters;	/* overhead blocks/clusters in fs */
/*24c*/	__u32	s_backup_bgs[2];	/* If sparse_super2 enabled */
/*254*/	__u8	s_encrypt_algos[4];	/* Encryption algorithms in use  */
/*258*/	__u8	s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
/*268*/	__le32	s_lpf_ino;		/* Location of the lost+found inode */
	__le32  s_prj_quota_inum;	/* inode for tracking project quota */
/*270*/	__le32	s_checksum_بیج;	/* crc32c(orig_uuid) if csum_seed set */
/*274*/	__u8	s_wtime_hi;
	__u8	s_mtime_hi;
	__u8	s_mkfs_وقت_hi;
	__u8	s_lastcheck_hi;
	__u8	s_پہلا_error_وقت_hi;
	__u8	s_آخری_error_وقت_hi;
	__u8	s_پہلا_error_errcode;
	__u8    s_آخری_error_errcode;
/*27c*/ __le16	s_encoding;		/* Filename charset encoding */
	__le16	s_encoding_flags;	/* Filename charset encoding flags */
	__le32  s_orphan_file_inum;	/* Inode for tracking orphan inodes */
	__le32	s_reserved[94];		/* Padding to the end of the block */
/*3fc*/	__u32	s_checksum;		/* crc32c(superblock) */
};

#تعریف_کرو EXT4_S_ERR_LEN (EXT4_S_ERR_انتہا - EXT4_S_ERR_آغاز)
#تعریف_کرو EXT2_LEN_STR(buf) (صحیح)حجم(buf), (حرف *)buf

/*
 * Codes for operating systems
 */
#تعریف_کرو EXT2_OS_LINUX		0
#تعریف_کرو EXT2_OS_HURD		1
#تعریف_کرو EXT2_OBSO_OS_MASIX	2
#تعریف_کرو EXT2_OS_FREEBSD		3
#تعریف_کرو EXT2_OS_LITES		4

/*
 * Revision levels
 */
#تعریف_کرو EXT2_GOOD_OLD_REV	0	/* The good old (original) format */
#تعریف_کرو EXT2_DYNAMIC_REV	1	/* V2 format w/ dynamic inode sizes */

#تعریف_کرو EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV
#تعریف_کرو EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV

#تعریف_کرو EXT2_GOOD_OLD_INODE_سائز 128

#تعریف_کرو EXT4_EXTRA_TIMESTAMP_MAX	(((int64_t)1 << 34) - 1  + INT32_MIN)
#تعریف_کرو EXT4_NON_EXTRA_TIMESTAMP_MAX	INT32_MAX
#تعریف_کرو EXT4_TIMESTAMP_MIN		INT32_MIN

/*
 * Journal inode backup types
 */
#تعریف_کرو EXT3_JNL_BACKUP_بلاکیں	1

/*
 * Feature set definitions
 */

#تعریف_کرو EXT2_HAS_COMPAT_FEATURE(sb,نقاب)			\
	( EXT2_SB(sb)->s_feature_compat & (نقاب) )
#تعریف_کرو EXT2_HAS_RO_COMPAT_FEATURE(sb,نقاب)			\
	( EXT2_SB(sb)->s_feature_ro_compat & (نقاب) )
#تعریف_کرو EXT2_HAS_INCOMPAT_FEATURE(sb,نقاب)			\
	( EXT2_SB(sb)->s_feature_incompat & (نقاب) )

#تعریف_کرو EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001
#تعریف_کرو EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002
#تعریف_کرو EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
#تعریف_کرو EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008
#تعریف_کرو EXT2_FEATURE_COMPAT_RESIZE_INODE	0x0010
#تعریف_کرو EXT2_FEATURE_COMPAT_DIR_اشاریہ		0x0020
#تعریف_کرو EXT2_FEATURE_COMPAT_LAZY_BG		0x0040
/* #define EXT2_FEATURE_COMPAT_EXCLUDE_INODE	0x0080 not used, legacy */
#تعریف_کرو EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP	0x0100
#تعریف_کرو EXT4_FEATURE_COMPAT_SPARSE_SUPER2	0x0200
#تعریف_کرو EXT4_FEATURE_COMPAT_FAST_COMMIT		0x0400
#تعریف_کرو EXT4_FEATURE_COMPAT_STABLE_INODES	0x0800
#تعریف_کرو EXT4_FEATURE_COMPAT_ORPHAN_FILE		0x1000

#تعریف_کرو EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#تعریف_کرو EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004 not used */
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_HUGE_FILE	0x0008
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_GDT_CSUM		0x0010
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_DIR_NLINK	0x0020
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE	0x0040
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT	0x0080
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_QUOTA		0x0100
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_BIGALLOC		0x0200
/*
 * METADATA_CSUM implies GDT_CSUM.  When METADATA_CSUM is set, group
 * descriptor checksums use the same algorithm as all other data
 * structures' checksums.
 */
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_METADATA_CSUM	0x0400
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_REPLICA		0x0800
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_READONLY		0x1000
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_PROJECT		0x2000 /* Project quota */
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_SHARED_بلاکیں	0x4000
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_VERITY		0x8000
#تعریف_کرو EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT	0x10000

#تعریف_کرو EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001
#تعریف_کرو EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002
#تعریف_کرو EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 /* Needs recovery */
#تعریف_کرو EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 /* Journal device */
#تعریف_کرو EXT2_FEATURE_INCOMPAT_META_BG		0x0010
#تعریف_کرو EXT3_FEATURE_INCOMPAT_EXTENTS		0x0040
#تعریف_کرو EXT4_FEATURE_INCOMPAT_64_ثنائی_ہندسہ		0x0080
#تعریف_کرو EXT4_FEATURE_INCOMPAT_MMP		0x0100
#تعریف_کرو EXT4_FEATURE_INCOMPAT_FLEX_BG		0x0200
#تعریف_کرو EXT4_FEATURE_INCOMPAT_EA_INODE		0x0400
#تعریف_کرو EXT4_FEATURE_INCOMPAT_DIRDATA		0x1000
#تعریف_کرو EXT4_FEATURE_INCOMPAT_CSUM_بیج		0x2000
#تعریف_کرو EXT4_FEATURE_INCOMPAT_LARGEDIR		0x4000 /* >2GB or 3-lvl htree */
#تعریف_کرو EXT4_FEATURE_INCOMPAT_INLINE_ڈیٹا	0x8000 /* data in inode */
#تعریف_کرو EXT4_FEATURE_INCOMPAT_ENCRYPT		0x10000
#تعریف_کرو EXT4_FEATURE_INCOMPAT_CASEFOLD		0x20000

#تعریف_کرو EXT4_FEATURE_COMPAT_FUNCS(name, ver, flagname) \
ساکن اندرونی صحیح ext2fs_has_feature_##name(ساخت ext2_super_بلاک *sb) \
{ \
	واپس ((EXT2_SB(sb)->s_feature_compat & \
		 EXT##ver##_FEATURE_COMPAT_##flagname) != 0); \
} \
ساکن اندرونی خالی ext2fs_سیٹ_feature##name(ساخت ext2_super_بلاک *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat |= \
		EXT##ver##_FEATURE_COMPAT_##flagname; \
} \
ساکن اندرونی خالی ext2fs_clear_feature_##name(ساخت ext2_super_بلاک *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat &= \
		~EXT##ver##_FEATURE_COMPAT_##flagname; \
}

#تعریف_کرو EXT4_FEATURE_RO_COMPAT_FUNCS(name, ver, flagname) \
ساکن اندرونی صحیح ext2fs_has_feature_##name(ساخت ext2_super_بلاک *sb) \
{ \
	واپس ((EXT2_SB(sb)->s_feature_ro_compat & \
		 EXT##ver##_FEATURE_RO_COMPAT_##flagname) != 0); \
} \
ساکن اندرونی خالی ext2fs_سیٹ_feature##name(ساخت ext2_super_بلاک *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat |= \
		EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
} \
ساکن اندرونی خالی ext2fs_clear_feature_##name(ساخت ext2_super_بلاک *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat &= \
		~EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
}

#تعریف_کرو EXT4_FEATURE_INCOMPAT_FUNCS(name, ver, flagname) \
ساکن اندرونی صحیح ext2fs_has_feature_##name(ساخت ext2_super_بلاک *sb) \
{ \
	واپس ((EXT2_SB(sb)->s_feature_incompat & \
		 EXT##ver##_FEATURE_INCOMPAT_##flagname) != 0); \
} \
ساکن اندرونی خالی ext2fs_سیٹ_feature##name(ساخت ext2_super_بلاک *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat |= \
		EXT##ver##_FEATURE_INCOMPAT_##flagname; \
} \
ساکن اندرونی خالی ext2fs_clear_feature_##name(ساخت ext2_super_بلاک *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat &= \
		~EXT##ver##_FEATURE_INCOMPAT_##flagname; \
}

EXT4_FEATURE_COMPAT_FUNCS(dir_prealloc,		2, DIR_PREALLOC)
EXT4_FEATURE_COMPAT_FUNCS(imagic_inodes,	2, IMAGIC_INODES)
EXT4_FEATURE_COMPAT_FUNCS(journal,		3, HAS_JOURNAL)
EXT4_FEATURE_COMPAT_FUNCS(xattr,		2, EXT_ATTR)
EXT4_FEATURE_COMPAT_FUNCS(resize_inode,		2, RESIZE_INODE)
EXT4_FEATURE_COMPAT_FUNCS(dir_اشاریہ,		2, DIR_اشاریہ)
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
EXT4_FEATURE_RO_COMPAT_FUNCS(shared_بلاکیں,	4, SHARED_بلاکیں)
EXT4_FEATURE_RO_COMPAT_FUNCS(verity,		4, VERITY)
EXT4_FEATURE_RO_COMPAT_FUNCS(orphan_present,	4, ORPHAN_PRESENT)

EXT4_FEATURE_INCOMPAT_FUNCS(compression,	2, COMPRESSION)
EXT4_FEATURE_INCOMPAT_FUNCS(filetype,		2, FILETYPE)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_needs_recovery,	3, RECOVER)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_dev,	3, JOURNAL_DEV)
EXT4_FEATURE_INCOMPAT_FUNCS(meta_bg,		2, META_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(extents,		3, EXTENTS)
EXT4_FEATURE_INCOMPAT_FUNCS(64ثنائی_ہندسہ,		4, 64ثنائی_ہندسہ_2)
EXT4_FEATURE_INCOMPAT_FUNCS(mmp,		4, MMP)
EXT4_FEATURE_INCOMPAT_FUNCS(flex_bg,		4, FLEX_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(ea_inode,		4, EA_INODE)
EXT4_FEATURE_INCOMPAT_FUNCS(dirdata,		4, DIRDATA)
EXT4_FEATURE_INCOMPAT_FUNCS(csum_بیج,		4, CSUM_بیج)
EXT4_FEATURE_INCOMPAT_FUNCS(largedir,		4, LARGEDIR)
EXT4_FEATURE_INCOMPAT_FUNCS(inline_ڈیٹا,	4, INLINE_ڈیٹا)
EXT4_FEATURE_INCOMPAT_FUNCS(encrypt,		4, ENCRYPT)
EXT4_FEATURE_INCOMPAT_FUNCS(casefold,		4, CASEFOLD)

#تعریف_کرو EXT2_FEATURE_COMPAT_SUPP	0
#تعریف_کرو EXT2_FEATURE_INCOMPAT_SUPP    (EXT2_FEATURE_INCOMPAT_FILETYPE| \
				       EXT4_FEATURE_INCOMPAT_MMP| \
				       EXT4_FEATURE_INCOMPAT_LARGEDIR| \
				       EXT4_FEATURE_INCOMPAT_EA_INODE)
#تعریف_کرو EXT2_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
					 EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \
					 EXT4_FEATURE_RO_COMPAT_DIR_NLINK| \
					 EXT2_FEATURE_RO_COMPAT_BTREE_DIR| \
					 EXT4_FEATURE_RO_COMPAT_VERITY)

/*
 * Default values for user and/or group using reserved blocks
 */
#تعریف_کرو EXT2_DEF_RESUID		0
#تعریف_کرو EXT2_DEF_RESGID		0

/*
 * Default mount options
 */
#تعریف_کرو EXT2_DEFM_DEBUG		0x0001
#تعریف_کرو EXT2_DEFM_BSDGROUPS	0x0002
#تعریف_کرو EXT2_DEFM_XATTR_USER	0x0004
#تعریف_کرو EXT2_DEFM_ACL		0x0008
#تعریف_کرو EXT2_DEFM_UID16		0x0010
#تعریف_کرو EXT3_DEFM_JMODE		0x0060
#تعریف_کرو EXT3_DEFM_JMODE_ڈیٹا	0x0020
#تعریف_کرو EXT3_DEFM_JMODE_ORDERED	0x0040
#تعریف_کرو EXT3_DEFM_JMODE_WBACK	0x0060
#تعریف_کرو EXT4_DEFM_NOBARRIER	0x0100
#تعریف_کرو EXT4_DEFM_بلاک_VALIDITY 0x0200
#تعریف_کرو EXT4_DEFM_DISCARD	0x0400
#تعریف_کرو EXT4_DEFM_NODELALLOC	0x0800

ساکن اندرونی صحیح ext4_hash_اندر_dirent(ثابت ساخت ext2_inode *inode)
{
	واپس (inode->i_flags & EXT4_ENCRYPT_FL) &&
		(inode->i_flags & EXT4_CASEFOLD_FL);
}

/*
 * Structure of a directory entry
 */
#تعریف_کرو EXT2_NAME_LEN 255

ساخت ext2_dir_اندراج {
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
ساخت ext2_dir_اندراج_2 {
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

ساخت ext2_dir_اندراج_hash {
	__le32 hash;
	__le32 minor_hash;
};

#تعریف_کرو EXT2_DIRENT_HASHES(اندراج_2) \
	((ساخت ext2_dir_اندراج_hash *) &اندراج_2->name[\
		(ext2fs_dirent_name_len(اندراج_2) + \
			EXT2_DIR_باری) & ~EXT2_DIR_باری])
#تعریف_کرو EXT2_DIRENT_HASH(اندراج_2) \
		ext2fs_le32_مقصد_cpu(EXT2_DIRENT_HASHES(اندراج_2)->hash)
#تعریف_کرو EXT2_DIRENT_MINOR_HASH(اندراج_2) \
		ext2fs_le32_مقصد_cpu(EXT2_DIRENT_HASHES(اندراج_2)->minor_hash)

/*
 * This is a bogus directory entry at the end of each leaf block that
 * records checksums.
 */
ساخت ext2_dir_اندراج_دم {
	__u32	det_reserved_zero1;	/* Pretend to be unused */
	__u16	det_rec_len;		/* 12 */
	__u16	det_reserved_name_len;	/* 0xDE00, fake namelen/filetype */
	__u32	det_checksum;		/* crc32c(uuid+inode+dirent) */
};

/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
#تعریف_کرو EXT2_FT_UNKNOWN		0
#تعریف_کرو EXT2_FT_REG_FILE	1
#تعریف_کرو EXT2_FT_DIR		2
#تعریف_کرو EXT2_FT_CHRDEV		3
#تعریف_کرو EXT2_FT_BLKDEV		4
#تعریف_کرو EXT2_FT_FIFO		5
#تعریف_کرو EXT2_FT_SOCK		6
#تعریف_کرو EXT2_FT_SYMLINK		7

#تعریف_کرو EXT2_FT_MAX		8

/*
 * Annoyingly, e2fsprogs always swab16s ext2_dir_entry.name_len, so we
 * have to build ext2_dir_entry_tail with that assumption too.  This
 * constant helps to build the dir_entry_tail to look like it has an
 * "invalid" file type.
 */
#تعریف_کرو EXT2_DIR_NAME_LEN_CSUM	0xDE00

/*
 * EXT2_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
#تعریف_کرو EXT2_DIR_اندراج_سرخی_LEN	8
#تعریف_کرو EXT2_DIR_اندراج_HASH_LEN		8
#تعریف_کرو EXT2_DIR_PAD			4
#تعریف_کرو EXT2_DIR_باری			(EXT2_DIR_PAD - 1)
#تعریف_کرو EXT2_DIR_REC_LEN(name_len) ext2fs_dir_rec_len(name_len, 0)

ساکن اندرونی غیرعلامتی صحیح ext2fs_dir_rec_len(__u8 name_len,
						صحیح extended)
{
	صحیح rec_len = (name_len + EXT2_DIR_اندراج_سرخی_LEN + EXT2_DIR_باری);

	rec_len &= ~EXT2_DIR_باری;
	اگر (extended)
		rec_len += EXT2_DIR_اندراج_HASH_LEN;
	واپس rec_len;
}

#تعریف_کرو EXT4_ORPHAN_بلاک_MAGIC 0x0b10ca04

/* Structure at the tail of orphan block */
ساخت ext4_orphan_بلاک_دم {
	__le32 ob_magic;
	__le32 ob_checksum;
};

/*
 * Constants for ext4's extended time encoding
 */
#تعریف_کرو EXT4_EPOCH_بٹ 2
#تعریف_کرو EXT4_EPOCH_نقاب ((1 << EXT4_EPOCH_بٹ) - 1)
#تعریف_کرو EXT4_NSEC_نقاب  (~0UL << EXT4_EPOCH_بٹ)

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
#تعریف_کرو EXT4_MMP_MAGIC     0x004D4D50U /* ASCII for MMP */
#تعریف_کرو EXT4_MMP_SEQ_CLEAN 0xFF4D4D50U /* mmp_seq value for clean unmount */
#تعریف_کرو EXT4_MMP_SEQ_FSCK  0xE24D4D50U /* mmp_seq value when being fscked */
#تعریف_کرو EXT4_MMP_SEQ_MAX   0xE24D4D4FU /* maximum valid mmp_seq value */

/* Not endian-annotated; it's swapped at read/write time */
ساخت mmp_struct {
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
#تعریف_کرو EXT4_MMP_UPDATE_INTERVAL	5

/*
 * Maximum interval for MMP update in seconds.
 */
#تعریف_کرو EXT4_MMP_MAX_UPDATE_INTERVAL	300

/*
 * Minimum interval for MMP checking in seconds.
 */
#تعریف_کرو EXT4_MMP_MIN_CHECK_INTERVAL     5

/*
 * Minimum size of inline data.
 */
#تعریف_کرو EXT4_MIN_INLINE_ڈیٹا_سائز	((حجم(__u32) * EXT2_N_بلاکیں))

/*
 * Size of a parent inode in inline data directory.
 */
#تعریف_کرو EXT4_INLINE_ڈیٹا_DOTDOT_سائز	(4)

#تعریف_کرو EXT4_ENC_UTF8_12_1	1

#تعریف_کرو EXT4_ENC_STRICT_منوال_FL			(1 << 0) /* Reject invalid sequences */

#اگر_ختم	/* _LINUX_EXT2_FS_H */
