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

#اگر_تعریف_نشده _LINUX_EXT2_FS_H
#تعریف_کن _LINUX_EXT2_FS_H

#ضمن <ext2fs/ext2_types.h>		/* Changed from linux/types.h */

#اگر_تعریف_نشده __GNUC_PREREQ
#إذا defined(__GNUC__) && defined(__GNUC_MINOR__)
#تعریف_کن __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وإلا
#تعریف_کن __GNUC_PREREQ(maj, min) 0
#پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده __nonstring
#اگر_تعریف_شده __has_attribute
#إذا __has_attribute(__nonstring__)
#تعریف_کن __nonstring                    __attribute__((__nonstring__))
#وإلا
#تعریف_کن __nonstring
#پایان_اگر /* __has_attribute(__nonstring__) */
#وإلا
# define __nonstring
#پایان_اگر /* __has_attribute */
#پایان_اگر /* __nonstring */

/*
 * The second extended filesystem constants/structures
 */

/*
 * Define EXT2FS_DEBUG to produce debug messages
 */
#لغو_تعریف EXT2FS_DEBUG

/*
 * Define EXT2_PREALLOCATE to preallocate data blocks for expanding files
 */
#تعریف_کن EXT2_PREALLOCATE
#تعریف_کن EXT2_DEFAULT_PREALLOC_بلوکها	8

/*
 * The second extended file system version
 */
#تعریف_کن EXT2FS_DATE		"95/08/09"
#تعریف_کن EXT2_FS_نسخه		"0.5b"

/*
 * Special inode numbers
 */
#تعریف_کن EXT2_BAD_INO		 1	/* Bad blocks inode */
#تعریف_کن EXT2_ریشه_INO		 2	/* Root inode */
#تعریف_کن EXT4_USR_QUOTA_INO	 3	/* User quota inode */
#تعریف_کن EXT4_GRP_QUOTA_INO	 4	/* Group quota inode */
#تعریف_کن EXT2_BOOT_LOADER_INO	 5	/* Boot loader inode */
#تعریف_کن EXT2_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#تعریف_کن EXT2_RESIZE_INO		 7	/* Reserved group descriptors inode */
#تعریف_کن EXT2_JOURNAL_INO	 8	/* Journal inode */
#تعریف_کن EXT2_EXCLUDE_INO	 9	/* The "exclude" inode, for snapshots */
#تعریف_کن EXT4_REPLICA_INO	10	/* Used by non-upstream feature */

/* First non-reserved inode for old ext2 filesystems */
#تعریف_کن EXT2_GOOD_OLD_اولی_INO	11

/*
 * The second extended file system magic number
 */
#تعریف_کن EXT2_SUPER_MAGIC	0xEF53

#اگر_تعریف_شده __KERNEL__
#تعریف_کن EXT2_SB(sb)	(&((sb)->u.ext2_sb))
#وإلا
/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#تعریف_کن EXT2_SB(sb)	(sb)
#پایان_اگر

/*
 * Maximal count of links to a file
 */
#تعریف_کن EXT2_LINK_MAX		65000

/*
 * Macro-instructions used to manage several block sizes
 */
#تعریف_کن EXT2_MIN_بلوک_LOG_اندازه		10	/* 1024 */
#تعریف_کن EXT2_MAX_بلوک_LOG_اندازه		16	/* 65536 */
#تعریف_کن EXT2_MIN_بلوک_اندازه	(1 << EXT2_MIN_بلوک_LOG_اندازه)
#تعریف_کن EXT2_MAX_بلوک_اندازه	(1 << EXT2_MAX_بلوک_LOG_اندازه)
#اگر_تعریف_شده __KERNEL__
#تعریف_کن EXT2_بلوک_اندازه(s)	((s)->s_blocksize)
#تعریف_کن EXT2_بلوک_اندازه_بیتها(s)	((s)->s_blocksize_بیتها)
#تعریف_کن EXT2_ADDR_در_هر_بلوک_بیتها(s)	(EXT2_SB(s)->addr_در_هر_بلوک_بیتها)
#تعریف_کن EXT2_INODE_اندازه(s)	(EXT2_SB(s)->s_inode_اندازه)
#تعریف_کن EXT2_اولی_INO(s)	(EXT2_SB(s)->s_اولی_ino)
#وإلا
#تعریف_کن EXT2_بلوک_اندازه(s)	(EXT2_MIN_بلوک_اندازه << (s)->s_log_بلوک_اندازه)
#تعریف_کن EXT2_بلوک_اندازه_بیتها(s)	((s)->s_log_بلوک_اندازه + 10)
#تعریف_کن EXT2_INODE_اندازه(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_INODE_اندازه : (s)->s_inode_اندازه)
#تعریف_کن EXT2_اولی_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_اولی_INO : (s)->s_اولی_ino)
#پایان_اگر
#تعریف_کن EXT2_ADDR_در_هر_بلوک(s)	(EXT2_بلوک_اندازه(s) / حجم(__u32))

/*
 * Macro-instructions used to manage allocation clusters
 */
#تعریف_کن EXT2_MIN_CLUSTER_LOG_اندازه	EXT2_MIN_بلوک_LOG_اندازه
#تعریف_کن EXT2_MAX_CLUSTER_LOG_اندازه	29	/* 512MB  */
#تعریف_کن EXT2_MIN_CLUSTER_اندازه		EXT2_MIN_بلوک_اندازه
#تعریف_کن EXT2_MAX_CLUSTER_اندازه		(1 << EXT2_MAX_CLUSTER_LOG_اندازه)
#تعریف_کن EXT2_CLUSTER_اندازه(s)		(EXT2_MIN_بلوک_اندازه << \
						(s)->s_log_cluster_اندازه)
#تعریف_کن EXT2_CLUSTER_اندازه_بیتها(s)	((s)->s_log_cluster_اندازه + 10)

/*
 * Macro-instructions used to manage fragments
 *
 * Note: for backwards compatibility only, for the dump program.
 * Ext2/3/4 will never support fragments....
 */
#تعریف_کن EXT2_MIN_FRAG_اندازه              EXT2_MIN_بلوک_اندازه
#تعریف_کن EXT2_MAX_FRAG_اندازه              EXT2_MAX_بلوک_اندازه
#تعریف_کن EXT2_MIN_FRAG_LOG_اندازه          EXT2_MIN_بلوک_LOG_اندازه
#تعریف_کن EXT2_FRAG_اندازه(s)		EXT2_بلوک_اندازه(s)
#تعریف_کن EXT2_FRAGS_در_هر_بلوک(s)		1

/*
 * ACL structures
 */
بنية ext2_acl_سرآیند	/* Header of Access Control Lists */
{
	__u32	aclh_اندازه;
	__u32	aclh_file_شمار;
	__u32	aclh_acle_شمار;
	__u32	aclh_اولی_acle;
};

بنية ext2_acl_مدخل	/* Access Control List Entry */
{
	__u32	acle_اندازه;
	__u16	acle_perms;	/* Access permissions */
	__u16	acle_type;	/* Type of entry */
	__u16	acle_برچسب;	/* User or group identity */
	__u16	acle_pad1;
	__u32	acle_بعدی;	/* Pointer on next entry for the */
					/* same inode or on next free entry */
};

/*
 * Structure of a blocks group descriptor
 */
بنية ext2_گروه_desc
{
	__u32	bg_بلوک_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_جدول;		/* Inodes table block */
	__u16	bg_free_بلوکها_شمار;	/* Free blocks count */
	__u16	bg_free_inodes_شمار;	/* Free inodes count */
	__u16	bg_استفاده_شده_dirs_شمار;	/* Directories count */
	__u16	bg_flags;
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_بلوک_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_استفادهنشده;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(s_uuid+group_num+group_desc)*/
};

/*
 * Structure of a blocks group descriptor
 */
بنية ext4_گروه_desc
{
	__u32	bg_بلوک_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_جدول;		/* Inodes table block */
	__u16	bg_free_بلوکها_شمار;	/* Free blocks count */
	__u16	bg_free_inodes_شمار;	/* Free inodes count */
	__u16	bg_استفاده_شده_dirs_شمار;	/* Directories count */
	__u16	bg_flags;		/* EXT4_BG_flags (INODE_UNINIT, etc) */
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_بلوک_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_استفادهنشده;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(sb_uuid+group+desc) */
	__u32	bg_بلوک_bitmap_hi;	/* Blocks bitmap block MSB */
	__u32	bg_inode_bitmap_hi;	/* Inodes bitmap block MSB */
	__u32	bg_inode_جدول_hi;	/* Inodes table block MSB */
	__u16	bg_free_بلوکها_شمار_hi;/* Free blocks count MSB */
	__u16	bg_free_inodes_شمار_hi;/* Free inodes count MSB */
	__u16	bg_استفاده_شده_dirs_شمار_hi;	/* Directories count MSB */
	__u16	bg_itable_استفادهنشده_hi;	/* Unused inodes count MSB */
	__u32	bg_exclude_bitmap_hi;	/* Exclude bitmap block MSB */
	__u16	bg_بلوک_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u16	bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u32	bg_reserved;
};

#تعریف_کن EXT4_BG_INODE_BITMAP_CSUM_HI_انتها	\
	(offsetof(بنية ext4_گروه_desc, bg_inode_bitmap_csum_hi) + \
	 حجم(__u16))
#تعریف_کن EXT4_BG_بلوک_BITMAP_CSUM_HI_LOCATION	\
	(offsetof(بنية ext4_گروه_desc, bg_بلوک_bitmap_csum_hi) + \
	 حجم(__u16))

#تعریف_کن EXT2_BG_INODE_UNINIT	0x0001 /* Inode table/bitmap not initialized */
#تعریف_کن EXT2_BG_بلوک_UNINIT	0x0002 /* Block bitmap not initialized */
#تعریف_کن EXT2_BG_INODE_ZEROED	0x0004 /* On-disk itable initialized to zero */

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
بنية ext2_dx_ریشه_info {
	__u32 reserved_zero;
	__u8 hash_نسخه; /* 0 now, 1 at release */
	__u8 info_درازی; /* 8 */
	__u8 غیرمستقیم_سطوح;
	__u8 استفادهنشده_flags;
};

#تعریف_کن EXT2_HASH_LEGACY		0
#تعریف_کن EXT2_HASH_HALF_MD4		1
#تعریف_کن EXT2_HASH_TEA			2
#تعریف_کن EXT2_HASH_LEGACY_UNSIGNED	3 /* reserved for userspace lib */
#تعریف_کن EXT2_HASH_HALF_MD4_UNSIGNED	4 /* reserved for userspace lib */
#تعریف_کن EXT2_HASH_TEA_UNSIGNED		5 /* reserved for userspace lib */
#تعریف_کن EXT2_HASH_SIPHASH		6

#تعریف_کن EXT2_HASH_FLAG_INCOMPAT	0x1

#تعریف_کن EXT4_DX_بلوک_ماسک 0x0fffffff

بنية ext2_dx_مدخل {
	__le32 hash;
	__le32 بلوک;
};

بنية ext2_dx_countlimit {
	__le16 حد;
	__le16 شمار;
};

/*
 * This goes at the end of each htree block.
 */
بنية ext2_dx_دم {
	__le32 dt_reserved;
	__le32 dt_checksum;	/* crc32c(uuid+inum+dxblock) */
};

/*
 * Macro-instructions used to manage group descriptors
 */
#تعریف_کن EXT2_MIN_DESC_اندازه             32
#تعریف_کن EXT2_MIN_DESC_اندازه_64_بیت       64
#تعریف_کن EXT2_MAX_DESC_اندازه             EXT2_MIN_بلوک_اندازه
#تعریف_کن EXT2_DESC_اندازه(s)                                                \
	(ext2fs_has_feature_64bit(s) ? (s)->s_desc_اندازه : EXT2_MIN_DESC_اندازه)

#تعریف_کن EXT2_بلوکها_در_هر_گروه(s)	(EXT2_SB(s)->s_بلوکها_در_هر_گروه)
#تعریف_کن EXT2_INODES_در_هر_گروه(s)	(EXT2_SB(s)->s_inodes_در_هر_گروه)
#تعریف_کن EXT2_CLUSTERS_در_هر_گروه(s)	(EXT2_SB(s)->s_clusters_در_هر_گروه)
#تعریف_کن EXT2_INODES_در_هر_بلوک(s)	(EXT2_بلوک_اندازه(s)/EXT2_INODE_اندازه(s))
/* limits imposed by 16-bit value gd_free_{blocks,inode}_count */
#تعریف_کن EXT2_MAX_بلوکها_در_هر_گروه(s)	((((بیعلامت) 1 << 16) - 8) *	\
					 (EXT2_CLUSTER_اندازه(s) / \
					  EXT2_بلوک_اندازه(s)))
#تعریف_کن EXT2_MAX_CLUSTERS_در_هر_گروه(s)	(((بیعلامت) 1 << 16) - 8)
#تعریف_کن EXT2_MAX_INODES_در_هر_گروه(s)	(((بیعلامت) 1 << 16) - \
					 EXT2_INODES_در_هر_بلوک(s))
#اگر_تعریف_شده __KERNEL__
#تعریف_کن EXT2_DESC_در_هر_بلوک(s)		(EXT2_SB(s)->s_desc_در_هر_بلوک)
#تعریف_کن EXT2_DESC_در_هر_بلوک_بیتها(s)	(EXT2_SB(s)->s_desc_در_هر_بلوک_بیتها)
#وإلا
#تعریف_کن EXT2_DESC_در_هر_بلوک(s)		(EXT2_بلوک_اندازه(s) / EXT2_DESC_اندازه(s))
#پایان_اگر

#تعریف_کن EXT2_GROUPS_مقصد_بلوکها(s, g)   ((blk64_t) EXT2_بلوکها_در_هر_گروه(s) * \
				       (g))
#تعریف_کن EXT2_GROUPS_مقصد_CLUSTERS(s, g) ((blk64_t) EXT2_CLUSTERS_در_هر_گروه(s) * \
				       (g))

/*
 * Constants relative to the data blocks
 */
#تعریف_کن EXT2_NDIR_بلوکها		12
#تعریف_کن EXT2_IND_بلوک			EXT2_NDIR_بلوکها
#تعریف_کن EXT2_DIND_بلوک			(EXT2_IND_بلوک + 1)
#تعریف_کن EXT2_TIND_بلوک			(EXT2_DIND_بلوک + 1)
#تعریف_کن EXT2_N_بلوکها			(EXT2_TIND_بلوک + 1)

/*
 * Inode flags
 */
#تعریف_کن EXT2_SECRM_FL			0x00000001 /* Secure deletion */
#تعریف_کن EXT2_UNRM_FL			0x00000002 /* Undelete */
#تعریف_کن EXT2_COMPR_FL			0x00000004 /* Compress file */
#تعریف_کن EXT2_SYNC_FL			0x00000008 /* Synchronous updates */
#تعریف_کن EXT2_IMMUTABLE_FL		0x00000010 /* Immutable file */
#تعریف_کن EXT2_APPEND_FL			0x00000020 /* writes to file may only append */
#تعریف_کن EXT2_NODUMP_FL			0x00000040 /* do not dump file */
#تعریف_کن EXT2_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#تعریف_کن EXT2_DIRTY_FL			0x00000100
#تعریف_کن EXT2_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#تعریف_کن EXT2_NOCOMPR_FL			0x00000400 /* Access raw compressed data */
	/* nb: was previously EXT2_ECOMPR_FL */
#تعریف_کن EXT4_ENCRYPT_FL			0x00000800 /* encrypted inode */
/* End compression flags --- maybe not all used */
#تعریف_کن EXT2_BTREE_FL			0x00001000 /* btree format dir */
#تعریف_کن EXT2_شاخص_FL			0x00001000 /* hash-indexed directory */
#تعریف_کن EXT2_IMAGIC_FL			0x00002000
#تعریف_کن EXT3_JOURNAL_داده_FL		0x00004000 /* file data should be journaled */
#تعریف_کن EXT2_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#تعریف_کن EXT2_DIRSYNC_FL 		0x00010000 /* Synchronous directory modifications */
#تعریف_کن EXT2_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#تعریف_کن EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#تعریف_کن EXT4_EXTENTS_FL 		0x00080000 /* Inode uses extents */
#تعریف_کن EXT4_VERITY_FL			0x00100000 /* Verity protected inode */
#تعریف_کن EXT4_EA_INODE_FL	        0x00200000 /* Inode used for large EA */
/* EXT4_EOFBLOCKS_FL 0x00400000 was here */
#تعریف_کن FS_NOCOW_FL			0x00800000 /* Do not cow file */
#تعریف_کن EXT4_SNAPFILE_FL		0x01000000  /* Inode is a snapshot */
#تعریف_کن FS_DAX_FL			0x02000000 /* Inode is DAX */
#تعریف_کن EXT4_SNAPFILE_DELETED_FL	0x04000000  /* Snapshot is being deleted */
#تعریف_کن EXT4_SNAPFILE_SHRUNK_FL		0x08000000  /* Snapshot shrink has completed */
#تعریف_کن EXT4_INLINE_داده_FL		0x10000000 /* Inode has inline data */
#تعریف_کن EXT4_PROJINHERIT_FL		0x20000000 /* Create with parents projid */
#تعریف_کن EXT4_CASEFOLD_FL		0x40000000 /* Casefolded file */
#تعریف_کن EXT2_RESERVED_FL		0x80000000 /* reserved for ext2 lib */

#تعریف_کن EXT2_FL_USER_VISIBLE		0x604BDFFF /* User visible flags */
#تعریف_کن EXT2_FL_USER_MODIFIABLE		0x604B80FF /* User modifiable flags */

/*
 * ioctl commands
 */

/* Used for online resize */
بنية ext2_new_گروه_درون_داد {
	__u32 گروه;		/* Group number for this data */
	__u32 بلوک_bitmap;	/* Absolute block number of block bitmap */
	__u32 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u32 inode_جدول;	/* Absolute block number of inode table start */
	__u32 بلوکها_شمار;	/* Total number of blocks in this group */
	__u16 reserved_بلوکها;	/* Number of reserved blocks in this group */
	__u16 استفادهنشده;		/* Number of reserved GDT blocks in group */
};

بنية ext4_new_گروه_درون_داد {
	__u32 گروه;		/* Group number for this data */
	__u64 بلوک_bitmap;	/* Absolute block number of block bitmap */
	__u64 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u64 inode_جدول;	/* Absolute block number of inode table start */
	__u32 بلوکها_شمار;	/* Total number of blocks in this group */
	__u16 reserved_بلوکها;	/* Number of reserved blocks in this group */
	__u16 استفادهنشده;
};

#اگر_تعریف_شده __GNU__			/* Needed for the Hurd */
#تعریف_کن IOT_ext2_new_گروه_درون_داد _IOT (_IOTS(__u32), 5, _IOTS(__u16), 2, 0, 0)
#پایان_اگر

#تعریف_کن EXT2_IOC_GETFLAGS		_IOR('f', 1, دراز)
#تعریف_کن EXT2_IOC_SETFLAGS		_IOW('f', 2, دراز)
#تعریف_کن EXT2_IOC_GETVERSION		_IOR('v', 1, دراز)
#تعریف_کن EXT2_IOC_SETVERSION		_IOW('v', 2, دراز)
#تعریف_کن EXT2_IOC_GETVERSION_NEW		_IOR('f', 3, دراز)
#تعریف_کن EXT2_IOC_SETVERSION_NEW		_IOW('f', 4, دراز)
#تعریف_کن EXT2_IOC_گروه_EXTEND		_IOW('f', 7, بیعلامت دراز)
#تعریف_کن EXT2_IOC_گروه_جمع_کن		_IOW('f', 8,بنية ext2_new_گروه_درون_داد)
#تعریف_کن EXT4_IOC_گروه_جمع_کن		_IOW('f', 8,بنية ext4_new_گروه_درون_داد)
#تعریف_کن EXT4_IOC_RESIZE_FS		_IOW('f', 16, __u64)

/*
 * Structure of an inode on the disk
 */
بنية ext2_inode {
/*00*/	__u16	i_نما;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_اندازه;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_شمار;	/* Links count */
	__u32	i_بلوکها;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	اتحاد {
		بنية {
			__u32	l_i_نسخه; /* was l_i_reserved1 */
		} linux1;
		بنية {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_بلوک[EXT2_N_بلوکها];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_اندازه_بالا;
/*70*/	__u32	i_faddr;	/* Fragment address */
	اتحاد {
		بنية {
			__u16	l_i_بلوکها_hi;
			__u16	l_i_file_acl_بالا;
			__u16	l_i_uid_بالا;	/* these 2 fields    */
			__u16	l_i_gid_بالا;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		بنية {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_نما_بالا;
			__u16	h_i_uid_بالا;
			__u16	h_i_gid_بالا;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
};

/*
 * Permanent part of an large inode on the disk
 */
بنية ext2_inode_large {
/*00*/	__u16	i_نما;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_اندازه;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode Change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_شمار;	/* Links count */
	__u32	i_بلوکها;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	اتحاد {
		بنية {
			__u32	l_i_نسخه; /* was l_i_reserved1 */
		} linux1;
		بنية {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_بلوک[EXT2_N_بلوکها];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_اندازه_بالا;
/*70*/	__u32	i_faddr;	/* Fragment address */
	اتحاد {
		بنية {
			__u16	l_i_بلوکها_hi;
			__u16	l_i_file_acl_بالا;
			__u16	l_i_uid_بالا;	/* these 2 fields    */
			__u16	l_i_gid_بالا;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		بنية {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_نما_بالا;
			__u16	h_i_uid_بالا;
			__u16	h_i_gid_بالا;
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
	__u32	i_نسخه_hi;	/* high 32 bits for 64-bit version */
/*9c*/	__u32   i_projid;       /* Project ID */
};

#تعریف_کن EXT4_INODE_CSUM_HI_EXTRA_انتها	\
	(offsetof(بنية ext2_inode_large, i_checksum_hi) + حجم(__u16) - \
	 EXT2_GOOD_OLD_INODE_اندازه)

#تعریف_کن EXT4_EPOCH_بیتها 2
#تعریف_کن EXT4_EPOCH_ماسک ((1 << EXT4_EPOCH_بیتها) - 1)

#تعریف_کن i_checksum_lo	osd2.linux2.l_i_checksum_lo

#تعریف_کن ext2fs_inode_includes(اندازه, فیلد)				\
	((اندازه) >= (حجم(((بنية ext2_inode_large *)0)->فیلد) +	\
		    offsetof(بنية ext2_inode_large, فیلد)))

#إذا defined(__KERNEL__) || defined(__linux__)
#تعریف_کن i_reserved1	osd1.linux1.l_i_reserved1
#تعریف_کن i_frag		osd2.linux2.l_i_frag
#تعریف_کن i_fsize		osd2.linux2.l_i_fsize
#تعریف_کن i_uid_کم	i_uid
#تعریف_کن i_gid_کم	i_gid
#تعریف_کن i_uid_بالا	osd2.linux2.l_i_uid_بالا
#تعریف_کن i_gid_بالا	osd2.linux2.l_i_gid_بالا
#وإلا
#إذا defined(__GNU__)

#تعریف_کن i_translator	osd1.hurd1.h_i_translator
#تعریف_کن i_frag		osd2.hurd2.h_i_frag;
#تعریف_کن i_fsize		osd2.hurd2.h_i_fsize;
#تعریف_کن i_uid_بالا	osd2.hurd2.h_i_uid_بالا
#تعریف_کن i_gid_بالا	osd2.hurd2.h_i_gid_بالا
#تعریف_کن i_author	osd2.hurd2.h_i_author

#پایان_اگر  /* __GNU__ */
#پایان_اگر	/* defined(__KERNEL__) || defined(__linux__) */

#تعریف_کن inode_uid(inode)	((inode).i_uid | (بیعلامت)(inode).osd2.linux2.l_i_uid_بالا << 16)
#تعریف_کن inode_gid(inode)	((inode).i_gid | (بیعلامت)(inode).osd2.linux2.l_i_gid_بالا << 16)
#تعریف_کن inode_projid(inode)	((inode).i_projid)
#تعریف_کن ext2fs_مجموعه_i_uid_بالا(inode,x) ((inode).osd2.linux2.l_i_uid_بالا = (x))
#تعریف_کن ext2fs_مجموعه_i_gid_بالا(inode,x) ((inode).osd2.linux2.l_i_gid_بالا = (x))

ساكن مضمن
بنية ext2_inode *EXT2_INODE(بنية ext2_inode_large *large_inode)
{
	أرجع (بنية ext2_inode *) large_inode;
}

/*
 * File system states
 */
#تعریف_کن EXT2_VALID_FS			0x0001	/* Unmounted cleanly */
#تعریف_کن EXT2_ERROR_FS			0x0002	/* Errors detected */
#تعریف_کن EXT3_ORPHAN_FS			0x0004	/* Orphans being recovered */
#تعریف_کن EXT4_FC_REPLAY			0x0020	/* Ext4 fast commit replay ongoing */

/*
 * Misc. filesystem flags
 */
#تعریف_کن EXT2_FLAGS_SIGNED_HASH		0x0001  /* Signed dirhash in use */
#تعریف_کن EXT2_FLAGS_UNSIGNED_HASH	0x0002  /* Unsigned dirhash in use */
#تعریف_کن EXT2_FLAGS_TEST_FILESYS		0x0004	/* OK for use on development code */
#تعریف_کن EXT2_FLAGS_IS_SNAPSHOT		0x0010	/* This is a snapshot image */
#تعریف_کن EXT2_FLAGS_FIX_SNAPSHOT		0x0020	/* Snapshot inodes corrupted */
#تعریف_کن EXT2_FLAGS_FIX_EXCLUDE		0x0040	/* Exclude bitmaps corrupted */

/*
 * Mount flags
 */
#تعریف_کن EXT2_MOUNT_CHECK		0x0001	/* Do mount-time checks */
#تعریف_کن EXT2_MOUNT_GRPID		0x0004	/* Create files with directory's group */
#تعریف_کن EXT2_MOUNT_DEBUG		0x0008	/* Some debugging messages */
#تعریف_کن EXT2_MOUNT_ERRORS_CONT		0x0010	/* Continue on errors */
#تعریف_کن EXT2_MOUNT_ERRORS_RO		0x0020	/* Remount fs ro on errors */
#تعریف_کن EXT2_MOUNT_ERRORS_PANIC		0x0040	/* Panic on errors */
#تعریف_کن EXT2_MOUNT_MINIX_DF		0x0080	/* Mimics the Minix statfs */
#تعریف_کن EXT2_MOUNT_NO_UID32		0x0200  /* Disable 32-bit UIDs */

#تعریف_کن clear_opt(o, opt)		o &= ~EXT2_MOUNT_##opt
#تعریف_کن مجموعه_opt(o, opt)			o |= EXT2_MOUNT_##opt
#تعریف_کن test_opt(sb, opt)		(EXT2_SB(sb)->s_mount_opt & \
					 EXT2_MOUNT_##opt)
/*
 * Maximal mount counts between two filesystem checks
 */
#تعریف_کن EXT2_DFL_MAX_MNT_شمار		20	/* Allow 20 mounts */
#تعریف_کن EXT2_DFL_CHECKINTERVAL		0	/* Don't use interval check */

/*
 * Behaviour when detecting errors
 */
#تعریف_کن EXT2_ERRORS_CONTINUE		1	/* Continue execution */
#تعریف_کن EXT2_ERRORS_RO			2	/* Remount fs read-only */
#تعریف_کن EXT2_ERRORS_PANIC		3	/* Panic */
#تعریف_کن EXT2_ERRORS_DEFAULT		EXT2_ERRORS_CONTINUE

#إذا (__GNUC__ >= 4)
#تعریف_کن ext4_offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#وإلا
#تعریف_کن ext4_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#پایان_اگر

/* Metadata checksum algorithms */
#تعریف_کن EXT2_بررسی_چرخهای_32_C_CHKSUM		1

/* Encryption algorithms, key size and key reference len */
#تعریف_کن EXT4_ENCRYPTION_نما_INVALID		0
#تعریف_کن EXT4_ENCRYPTION_نما_AES_256_XTS	1
#تعریف_کن EXT4_ENCRYPTION_نما_AES_256_GCM	2
#تعریف_کن EXT4_ENCRYPTION_نما_AES_256_CBC	3
#تعریف_کن EXT4_ENCRYPTION_نما_AES_256_CTS	4

#تعریف_کن EXT4_AES_256_XTS_کلید_اندازه		64
#تعریف_کن EXT4_AES_256_GCM_کلید_اندازه		32
#تعریف_کن EXT4_AES_256_CBC_کلید_اندازه		32
#تعریف_کن EXT4_AES_256_CTS_کلید_اندازه		32
#تعریف_کن EXT4_MAX_کلید_اندازه			64

#تعریف_کن EXT4_کلید_DESCRIPTOR_اندازه		8
#تعریف_کن EXT4_CRYPTO_بلوک_اندازه			16

/* Password derivation constants */
#تعریف_کن EXT4_MAX_PASSPHRASE_اندازه		1024
#تعریف_کن EXT4_MAX_SALT_اندازه			256
#تعریف_کن EXT4_PBKDF2_ITERATIONS			0xFFFF

#تعریف_کن EXT2_LABEL_LEN				16

/*
 * Policy provided via an ioctl on the topmost directory. This
 * structure is also in the kernel.
 */
بنية ext4_encryption_policy {
  حرف نسخه;
  حرف contents_encryption_نما;
  حرف filenames_encryption_نما;
  حرف flags;
  حرف master_کلید_descriptor[EXT4_کلید_DESCRIPTOR_اندازه];
} __attribute__((__packed__));

بنية ext4_encryption_کلید {
        __u32 نما;
        حرف raw[EXT4_MAX_کلید_اندازه];
        __u32 اندازه;
} __attribute__((__packed__));

/*
 * Structure of the super block
 */
بنية ext2_super_بلوک {
/*000*/	__u32	s_inodes_شمار;		/* Inodes count */
	__u32	s_بلوکها_شمار;		/* Blocks count */
	__u32	s_r_بلوکها_شمار;	/* Reserved blocks count */
	__u32	s_free_بلوکها_شمار;	/* Free blocks count */
/*010*/	__u32	s_free_inodes_شمار;	/* Free inodes count */
	__u32	s_اولی_داده_بلوک;	/* First Data Block */
	__u32	s_log_بلوک_اندازه;	/* Block size */
	__u32	s_log_cluster_اندازه;	/* Allocation cluster size */
/*020*/	__u32	s_بلوکها_در_هر_گروه;	/* # Blocks per group */
	__u32	s_clusters_در_هر_گروه;	/* # Fragments per group */
	__u32	s_inodes_در_هر_گروه;	/* # Inodes per group */
	__u32	s_mtime;		/* Mount time */
/*030*/	__u32	s_wtime;		/* Write time */
	__u16	s_mnt_شمار;		/* Mount count */
	__s16	s_max_mnt_شمار;	/* Maximal mount count */
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
	__u32	s_اولی_ino;		/* First non-reserved inode */
	__u16   s_inode_اندازه;		/* size of inode structure */
	__u16	s_بلوک_گروه_nr;	/* block group # of this superblock */
	__u32	s_feature_compat;	/* compatible feature set */
/*060*/	__u32	s_feature_incompat;	/* incompatible feature set */
	__u32	s_feature_ro_compat;	/* readonly-compatible feature set */
/*068*/	__u8	s_uuid[16] __nonstring;		/* 128-bit uuid for volume */
/*078*/	__u8	s_volume_name[EXT2_LABEL_LEN] __nonstring;	/* volume name, no NUL? */
/*088*/	__u8	s_آخرین_mounted[64] __nonstring;	/* directory last mounted on, no NUL? */
/*0c8*/	__u32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_بلوکها;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_بلوکها;	/* Nr to preallocate for dirs */
	__u16	s_reserved_gdt_بلوکها;	/* Per group table for online growth */
	/*
	 * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
/*0d0*/	__u8	s_journal_uuid[16] __nonstring;	/* uuid of journal superblock */
/*0e0*/	__u32	s_journal_inum;		/* inode number of journal file */
	__u32	s_journal_dev;		/* device number of journal file */
	__u32	s_آخرین_orphan;		/* start of list of inodes to delete */
/*0ec*/	__u32	s_hash_دانه[4];		/* HTREE hash seed */
/*0fc*/	__u8	s_def_hash_نسخه;	/* Default hash version to use */
	__u8	s_jnl_backup_type;	/* Default type of journal backup */
	__u16	s_desc_اندازه;		/* Group desc. size: INCOMPAT_64BIT */
/*100*/	__u32	s_default_mount_opts;	/* default EXT2_MOUNT_* flags used */
	__u32	s_اولی_meta_bg;	/* First metablock group */
	__u32	s_mkfs_زمان;		/* When the filesystem was created */
/*10c*/	__u32	s_jnl_بلوکها[17];	/* Backup of the journal inode */
/*150*/	__u32	s_بلوکها_شمار_hi;	/* Blocks count high 32bits */
	__u32	s_r_بلوکها_شمار_hi;	/* Reserved blocks count high 32 bits*/
	__u32	s_free_بلوکها_hi;	/* Free blocks count */
	__u16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__u16	s_want_extra_isize;	/* New inodes should reserve # bytes */
/*160*/	__u32	s_flags;		/* Miscellaneous flags */
	__u16	s_raid_stride;		/* RAID stride in blocks */
	__u16	s_mmp_update_interval;  /* # seconds to wait in MMP checking */
	__u64	s_mmp_بلوک;		/* Block for multi-mount protection */
/*170*/	__u32	s_raid_stripe_عرض;	/* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_در_هر_flex;	/* FLEX_BG group size */
	__u8	s_checksum_type;	/* metadata checksum algorithm */
	__u8	s_encryption_level;	/* versioning level for encryption */
	__u8	s_reserved_pad;		/* Padding to next 32bits */
	__u64	s_kbytes_written;	/* nr of lifetime kilobytes written */
/*180*/	__u32	s_snapshot_inum;	/* Inode number of active snapshot */
	__u32	s_snapshot_id;		/* sequential ID of active snapshot */
	__u64	s_snapshot_r_بلوکها_شمار; /* active snapshot reserved blocks */
/*190*/	__u32	s_snapshot_list;	/* inode number of disk snapshot list */
#تعریف_کن EXT4_S_ERR_شروع ext4_offsetof(بنية ext2_super_بلوک, s_error_شمار)
	__u32	s_error_شمار;		/* number of fs errors */
	__u32	s_اولی_error_زمان;	/* first time an error happened */
	__u32	s_اولی_error_ino;	/* inode involved in first error */
/*1a0*/	__u64	s_اولی_error_بلوک;	/* block involved in first error */
	__u8	s_اولی_error_func[32] __nonstring;	/* function where error hit, no NUL? */
/*1c8*/	__u32	s_اولی_error_سطر;	/* line number where error happened */
	__u32	s_آخرین_error_زمان;	/* most recent time of an error */
/*1d0*/	__u32	s_آخرین_error_ino;	/* inode involved in last error */
	__u32	s_آخرین_error_سطر;	/* line number where error happened */
	__u64	s_آخرین_error_بلوک;	/* block involved of last error */
/*1e0*/	__u8	s_آخرین_error_func[32] __nonstring;	/* function where error hit, no NUL? */
#تعریف_کن EXT4_S_ERR_انتها ext4_offsetof(بنية ext2_super_بلوک, s_mount_opts)
/*200*/	__u8	s_mount_opts[64] __nonstring;	/* default mount options, no NUL? */
/*240*/	__u32	s_usr_quota_inum;	/* inode number of user quota file */
	__u32	s_grp_quota_inum;	/* inode number of group quota file */
	__u32	s_overhead_clusters;	/* overhead blocks/clusters in fs */
/*24c*/	__u32	s_backup_bgs[2];	/* If sparse_super2 enabled */
/*254*/	__u8	s_encrypt_algos[4];	/* Encryption algorithms in use  */
/*258*/	__u8	s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
/*268*/	__le32	s_lpf_ino;		/* Location of the lost+found inode */
	__le32  s_prj_quota_inum;	/* inode for tracking project quota */
/*270*/	__le32	s_checksum_دانه;	/* crc32c(orig_uuid) if csum_seed set */
/*274*/	__u8	s_wtime_hi;
	__u8	s_mtime_hi;
	__u8	s_mkfs_زمان_hi;
	__u8	s_lastcheck_hi;
	__u8	s_اولی_error_زمان_hi;
	__u8	s_آخرین_error_زمان_hi;
	__u8	s_اولی_error_errcode;
	__u8    s_آخرین_error_errcode;
/*27c*/ __le16	s_encoding;		/* Filename charset encoding */
	__le16	s_encoding_flags;	/* Filename charset encoding flags */
	__le32  s_orphan_file_inum;	/* Inode for tracking orphan inodes */
	__le32	s_reserved[94];		/* Padding to the end of the block */
/*3fc*/	__u32	s_checksum;		/* crc32c(superblock) */
};

#تعریف_کن EXT4_S_ERR_LEN (EXT4_S_ERR_انتها - EXT4_S_ERR_شروع)
#تعریف_کن EXT2_LEN_STR(buf) (صحيح)حجم(buf), (حرف *)buf

/*
 * Codes for operating systems
 */
#تعریف_کن EXT2_OS_LINUX		0
#تعریف_کن EXT2_OS_HURD		1
#تعریف_کن EXT2_OBSO_OS_MASIX	2
#تعریف_کن EXT2_OS_FREEBSD		3
#تعریف_کن EXT2_OS_LITES		4

/*
 * Revision levels
 */
#تعریف_کن EXT2_GOOD_OLD_REV	0	/* The good old (original) format */
#تعریف_کن EXT2_DYNAMIC_REV	1	/* V2 format w/ dynamic inode sizes */

#تعریف_کن EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV
#تعریف_کن EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV

#تعریف_کن EXT2_GOOD_OLD_INODE_اندازه 128

#تعریف_کن EXT4_EXTRA_TIMESTAMP_MAX	(((int64_t)1 << 34) - 1  + INT32_MIN)
#تعریف_کن EXT4_NON_EXTRA_TIMESTAMP_MAX	INT32_MAX
#تعریف_کن EXT4_TIMESTAMP_MIN		INT32_MIN

/*
 * Journal inode backup types
 */
#تعریف_کن EXT3_JNL_BACKUP_بلوکها	1

/*
 * Feature set definitions
 */

#تعریف_کن EXT2_HAS_COMPAT_FEATURE(sb,ماسک)			\
	( EXT2_SB(sb)->s_feature_compat & (ماسک) )
#تعریف_کن EXT2_HAS_RO_COMPAT_FEATURE(sb,ماسک)			\
	( EXT2_SB(sb)->s_feature_ro_compat & (ماسک) )
#تعریف_کن EXT2_HAS_INCOMPAT_FEATURE(sb,ماسک)			\
	( EXT2_SB(sb)->s_feature_incompat & (ماسک) )

#تعریف_کن EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001
#تعریف_کن EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002
#تعریف_کن EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
#تعریف_کن EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008
#تعریف_کن EXT2_FEATURE_COMPAT_RESIZE_INODE	0x0010
#تعریف_کن EXT2_FEATURE_COMPAT_DIR_شاخص		0x0020
#تعریف_کن EXT2_FEATURE_COMPAT_LAZY_BG		0x0040
/* #define EXT2_FEATURE_COMPAT_EXCLUDE_INODE	0x0080 not used, legacy */
#تعریف_کن EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP	0x0100
#تعریف_کن EXT4_FEATURE_COMPAT_SPARSE_SUPER2	0x0200
#تعریف_کن EXT4_FEATURE_COMPAT_FAST_COMMIT		0x0400
#تعریف_کن EXT4_FEATURE_COMPAT_STABLE_INODES	0x0800
#تعریف_کن EXT4_FEATURE_COMPAT_ORPHAN_FILE		0x1000

#تعریف_کن EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#تعریف_کن EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004 not used */
#تعریف_کن EXT4_FEATURE_RO_COMPAT_HUGE_FILE	0x0008
#تعریف_کن EXT4_FEATURE_RO_COMPAT_GDT_CSUM		0x0010
#تعریف_کن EXT4_FEATURE_RO_COMPAT_DIR_NLINK	0x0020
#تعریف_کن EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE	0x0040
#تعریف_کن EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT	0x0080
#تعریف_کن EXT4_FEATURE_RO_COMPAT_QUOTA		0x0100
#تعریف_کن EXT4_FEATURE_RO_COMPAT_BIGALLOC		0x0200
/*
 * METADATA_CSUM implies GDT_CSUM.  When METADATA_CSUM is set, group
 * descriptor checksums use the same algorithm as all other data
 * structures' checksums.
 */
#تعریف_کن EXT4_FEATURE_RO_COMPAT_METADATA_CSUM	0x0400
#تعریف_کن EXT4_FEATURE_RO_COMPAT_REPLICA		0x0800
#تعریف_کن EXT4_FEATURE_RO_COMPAT_READONLY		0x1000
#تعریف_کن EXT4_FEATURE_RO_COMPAT_PROJECT		0x2000 /* Project quota */
#تعریف_کن EXT4_FEATURE_RO_COMPAT_SHARED_بلوکها	0x4000
#تعریف_کن EXT4_FEATURE_RO_COMPAT_VERITY		0x8000
#تعریف_کن EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT	0x10000

#تعریف_کن EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001
#تعریف_کن EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002
#تعریف_کن EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 /* Needs recovery */
#تعریف_کن EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 /* Journal device */
#تعریف_کن EXT2_FEATURE_INCOMPAT_META_BG		0x0010
#تعریف_کن EXT3_FEATURE_INCOMPAT_EXTENTS		0x0040
#تعریف_کن EXT4_FEATURE_INCOMPAT_64_بیت		0x0080
#تعریف_کن EXT4_FEATURE_INCOMPAT_MMP		0x0100
#تعریف_کن EXT4_FEATURE_INCOMPAT_FLEX_BG		0x0200
#تعریف_کن EXT4_FEATURE_INCOMPAT_EA_INODE		0x0400
#تعریف_کن EXT4_FEATURE_INCOMPAT_DIRDATA		0x1000
#تعریف_کن EXT4_FEATURE_INCOMPAT_CSUM_دانه		0x2000
#تعریف_کن EXT4_FEATURE_INCOMPAT_LARGEDIR		0x4000 /* >2GB or 3-lvl htree */
#تعریف_کن EXT4_FEATURE_INCOMPAT_INLINE_داده	0x8000 /* data in inode */
#تعریف_کن EXT4_FEATURE_INCOMPAT_ENCRYPT		0x10000
#تعریف_کن EXT4_FEATURE_INCOMPAT_CASEFOLD		0x20000

#تعریف_کن EXT4_FEATURE_COMPAT_FUNCS(name, ver, flagname) \
ساكن مضمن صحيح ext2fs_has_feature_##name(بنية ext2_super_بلوک *sb) \
{ \
	أرجع ((EXT2_SB(sb)->s_feature_compat & \
		 EXT##ver##_FEATURE_COMPAT_##flagname) != 0); \
} \
ساكن مضمن فراغ ext2fs_مجموعه_feature##name(بنية ext2_super_بلوک *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat |= \
		EXT##ver##_FEATURE_COMPAT_##flagname; \
} \
ساكن مضمن فراغ ext2fs_clear_feature_##name(بنية ext2_super_بلوک *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat &= \
		~EXT##ver##_FEATURE_COMPAT_##flagname; \
}

#تعریف_کن EXT4_FEATURE_RO_COMPAT_FUNCS(name, ver, flagname) \
ساكن مضمن صحيح ext2fs_has_feature_##name(بنية ext2_super_بلوک *sb) \
{ \
	أرجع ((EXT2_SB(sb)->s_feature_ro_compat & \
		 EXT##ver##_FEATURE_RO_COMPAT_##flagname) != 0); \
} \
ساكن مضمن فراغ ext2fs_مجموعه_feature##name(بنية ext2_super_بلوک *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat |= \
		EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
} \
ساكن مضمن فراغ ext2fs_clear_feature_##name(بنية ext2_super_بلوک *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat &= \
		~EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
}

#تعریف_کن EXT4_FEATURE_INCOMPAT_FUNCS(name, ver, flagname) \
ساكن مضمن صحيح ext2fs_has_feature_##name(بنية ext2_super_بلوک *sb) \
{ \
	أرجع ((EXT2_SB(sb)->s_feature_incompat & \
		 EXT##ver##_FEATURE_INCOMPAT_##flagname) != 0); \
} \
ساكن مضمن فراغ ext2fs_مجموعه_feature##name(بنية ext2_super_بلوک *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat |= \
		EXT##ver##_FEATURE_INCOMPAT_##flagname; \
} \
ساكن مضمن فراغ ext2fs_clear_feature_##name(بنية ext2_super_بلوک *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat &= \
		~EXT##ver##_FEATURE_INCOMPAT_##flagname; \
}

EXT4_FEATURE_COMPAT_FUNCS(dir_prealloc,		2, DIR_PREALLOC)
EXT4_FEATURE_COMPAT_FUNCS(imagic_inodes,	2, IMAGIC_INODES)
EXT4_FEATURE_COMPAT_FUNCS(journal,		3, HAS_JOURNAL)
EXT4_FEATURE_COMPAT_FUNCS(xattr,		2, EXT_ATTR)
EXT4_FEATURE_COMPAT_FUNCS(resize_inode,		2, RESIZE_INODE)
EXT4_FEATURE_COMPAT_FUNCS(dir_شاخص,		2, DIR_شاخص)
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
EXT4_FEATURE_RO_COMPAT_FUNCS(shared_بلوکها,	4, SHARED_بلوکها)
EXT4_FEATURE_RO_COMPAT_FUNCS(verity,		4, VERITY)
EXT4_FEATURE_RO_COMPAT_FUNCS(orphan_present,	4, ORPHAN_PRESENT)

EXT4_FEATURE_INCOMPAT_FUNCS(compression,	2, COMPRESSION)
EXT4_FEATURE_INCOMPAT_FUNCS(filetype,		2, FILETYPE)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_needs_recovery,	3, RECOVER)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_dev,	3, JOURNAL_DEV)
EXT4_FEATURE_INCOMPAT_FUNCS(meta_bg,		2, META_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(extents,		3, EXTENTS)
EXT4_FEATURE_INCOMPAT_FUNCS(64بیت,		4, 64بیت_2)
EXT4_FEATURE_INCOMPAT_FUNCS(mmp,		4, MMP)
EXT4_FEATURE_INCOMPAT_FUNCS(flex_bg,		4, FLEX_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(ea_inode,		4, EA_INODE)
EXT4_FEATURE_INCOMPAT_FUNCS(dirdata,		4, DIRDATA)
EXT4_FEATURE_INCOMPAT_FUNCS(csum_دانه,		4, CSUM_دانه)
EXT4_FEATURE_INCOMPAT_FUNCS(largedir,		4, LARGEDIR)
EXT4_FEATURE_INCOMPAT_FUNCS(inline_داده,	4, INLINE_داده)
EXT4_FEATURE_INCOMPAT_FUNCS(encrypt,		4, ENCRYPT)
EXT4_FEATURE_INCOMPAT_FUNCS(casefold,		4, CASEFOLD)

#تعریف_کن EXT2_FEATURE_COMPAT_SUPP	0
#تعریف_کن EXT2_FEATURE_INCOMPAT_SUPP    (EXT2_FEATURE_INCOMPAT_FILETYPE| \
				       EXT4_FEATURE_INCOMPAT_MMP| \
				       EXT4_FEATURE_INCOMPAT_LARGEDIR| \
				       EXT4_FEATURE_INCOMPAT_EA_INODE)
#تعریف_کن EXT2_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
					 EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \
					 EXT4_FEATURE_RO_COMPAT_DIR_NLINK| \
					 EXT2_FEATURE_RO_COMPAT_BTREE_DIR| \
					 EXT4_FEATURE_RO_COMPAT_VERITY)

/*
 * Default values for user and/or group using reserved blocks
 */
#تعریف_کن EXT2_DEF_RESUID		0
#تعریف_کن EXT2_DEF_RESGID		0

/*
 * Default mount options
 */
#تعریف_کن EXT2_DEFM_DEBUG		0x0001
#تعریف_کن EXT2_DEFM_BSDGROUPS	0x0002
#تعریف_کن EXT2_DEFM_XATTR_USER	0x0004
#تعریف_کن EXT2_DEFM_ACL		0x0008
#تعریف_کن EXT2_DEFM_UID16		0x0010
#تعریف_کن EXT3_DEFM_JMODE		0x0060
#تعریف_کن EXT3_DEFM_JMODE_داده	0x0020
#تعریف_کن EXT3_DEFM_JMODE_ORDERED	0x0040
#تعریف_کن EXT3_DEFM_JMODE_WBACK	0x0060
#تعریف_کن EXT4_DEFM_NOBARRIER	0x0100
#تعریف_کن EXT4_DEFM_بلوک_VALIDITY 0x0200
#تعریف_کن EXT4_DEFM_DISCARD	0x0400
#تعریف_کن EXT4_DEFM_NODELALLOC	0x0800

ساكن مضمن صحيح ext4_hash_درون_dirent(ثابت بنية ext2_inode *inode)
{
	أرجع (inode->i_flags & EXT4_ENCRYPT_FL) &&
		(inode->i_flags & EXT4_CASEFOLD_FL);
}

/*
 * Structure of a directory entry
 */
#تعریف_کن EXT2_NAME_LEN 255

بنية ext2_dir_مدخل {
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
بنية ext2_dir_مدخل_2 {
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

بنية ext2_dir_مدخل_hash {
	__le32 hash;
	__le32 minor_hash;
};

#تعریف_کن EXT2_DIRENT_HASHES(مدخل) \
	((بنية ext2_dir_مدخل_hash *) &مدخل->name[\
		(ext2fs_dirent_name_len(مدخل) + \
			EXT2_DIR_دور) & ~EXT2_DIR_دور])
#تعریف_کن EXT2_DIRENT_HASH(مدخل) \
		ext2fs_le32_مقصد_cpu(EXT2_DIRENT_HASHES(مدخل)->hash)
#تعریف_کن EXT2_DIRENT_MINOR_HASH(مدخل) \
		ext2fs_le32_مقصد_cpu(EXT2_DIRENT_HASHES(مدخل)->minor_hash)

/*
 * This is a bogus directory entry at the end of each leaf block that
 * records checksums.
 */
بنية ext2_dir_مدخل_دم {
	__u32	det_reserved_zero1;	/* Pretend to be unused */
	__u16	det_rec_len;		/* 12 */
	__u16	det_reserved_name_len;	/* 0xDE00, fake namelen/filetype */
	__u32	det_checksum;		/* crc32c(uuid+inode+dirent) */
};

/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
#تعریف_کن EXT2_FT_UNKNOWN		0
#تعریف_کن EXT2_FT_REG_FILE	1
#تعریف_کن EXT2_FT_DIR		2
#تعریف_کن EXT2_FT_CHRDEV		3
#تعریف_کن EXT2_FT_BLKDEV		4
#تعریف_کن EXT2_FT_FIFO		5
#تعریف_کن EXT2_FT_SOCK		6
#تعریف_کن EXT2_FT_SYMLINK		7

#تعریف_کن EXT2_FT_MAX		8

/*
 * Annoyingly, e2fsprogs always swab16s ext2_dir_entry.name_len, so we
 * have to build ext2_dir_entry_tail with that assumption too.  This
 * constant helps to build the dir_entry_tail to look like it has an
 * "invalid" file type.
 */
#تعریف_کن EXT2_DIR_NAME_LEN_CSUM	0xDE00

/*
 * EXT2_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
#تعریف_کن EXT2_DIR_مدخل_سرآیند_LEN	8
#تعریف_کن EXT2_DIR_مدخل_HASH_LEN		8
#تعریف_کن EXT2_DIR_PAD			4
#تعریف_کن EXT2_DIR_دور			(EXT2_DIR_PAD - 1)
#تعریف_کن EXT2_DIR_REC_LEN(name_len) ext2fs_dir_rec_len(name_len, 0)

ساكن مضمن بیعلامت صحيح ext2fs_dir_rec_len(__u8 name_len,
						صحيح extended)
{
	صحيح rec_len = (name_len + EXT2_DIR_مدخل_سرآیند_LEN + EXT2_DIR_دور);

	rec_len &= ~EXT2_DIR_دور;
	إذا (extended)
		rec_len += EXT2_DIR_مدخل_HASH_LEN;
	أرجع rec_len;
}

#تعریف_کن EXT4_ORPHAN_بلوک_MAGIC 0x0b10ca04

/* Structure at the tail of orphan block */
بنية ext4_orphan_بلوک_دم {
	__le32 ob_magic;
	__le32 ob_checksum;
};

/*
 * Constants for ext4's extended time encoding
 */
#تعریف_کن EXT4_EPOCH_بیتها 2
#تعریف_کن EXT4_EPOCH_ماسک ((1 << EXT4_EPOCH_بیتها) - 1)
#تعریف_کن EXT4_NSEC_ماسک  (~0UL << EXT4_EPOCH_بیتها)

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
#تعریف_کن EXT4_MMP_MAGIC     0x004D4D50U /* ASCII for MMP */
#تعریف_کن EXT4_MMP_SEQ_CLEAN 0xFF4D4D50U /* mmp_seq value for clean unmount */
#تعریف_کن EXT4_MMP_SEQ_FSCK  0xE24D4D50U /* mmp_seq value when being fscked */
#تعریف_کن EXT4_MMP_SEQ_MAX   0xE24D4D4FU /* maximum valid mmp_seq value */

/* Not endian-annotated; it's swapped at read/write time */
بنية mmp_struct {
	__u32	mmp_magic;		/* Magic number for MMP */
	__u32	mmp_seq;		/* Sequence no. updated periodically */
	__u64	mmp_زمان;		/* Time last updated (seconds) */
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
#تعریف_کن EXT4_MMP_UPDATE_INTERVAL	5

/*
 * Maximum interval for MMP update in seconds.
 */
#تعریف_کن EXT4_MMP_MAX_UPDATE_INTERVAL	300

/*
 * Minimum interval for MMP checking in seconds.
 */
#تعریف_کن EXT4_MMP_MIN_CHECK_INTERVAL     5

/*
 * Minimum size of inline data.
 */
#تعریف_کن EXT4_MIN_INLINE_داده_اندازه	((حجم(__u32) * EXT2_N_بلوکها))

/*
 * Size of a parent inode in inline data directory.
 */
#تعریف_کن EXT4_INLINE_داده_DOTDOT_اندازه	(4)

#تعریف_کن EXT4_ENC_UTF8_12_1	1

#تعریف_کن EXT4_ENC_STRICT_نما_FL			(1 << 0) /* Reject invalid sequences */

#پایان_اگر	/* _LINUX_EXT2_FS_H */
