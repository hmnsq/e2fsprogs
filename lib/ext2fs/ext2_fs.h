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

#నిర్వచితం_కాకపోతే _LINUX_EXT2_FS_H
#నిర్వచించు _LINUX_EXT2_FS_H

#అన్తర్భావ <ext2fs/ext2_types.h>		/* Changed from linux/types.h */

#నిర్వచితం_కాకపోతే __GNUC_PREREQ
#ఒకవేళ defined(__GNUC__) && defined(__GNUC_MINOR__)
#నిర్వచించు __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#లేకపోతే
#నిర్వచించు __GNUC_PREREQ(maj, min) 0
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే __nonstring
#నిర్వచితమైతే __has_attribute
#ఒకవేళ __has_attribute(__nonstring__)
#నిర్వచించు __nonstring                    __attribute__((__nonstring__))
#లేకపోతే
#నిర్వచించు __nonstring
#అయితే_ముగింపు /* __has_attribute(__nonstring__) */
#లేకపోతే
# define __nonstring
#అయితే_ముగింపు /* __has_attribute */
#అయితే_ముగింపు /* __nonstring */

/*
 * The second extended filesystem constants/structures
 */

/*
 * Define EXT2FS_DEBUG to produce debug messages
 */
#నిర్వచనం_రద్దు EXT2FS_DEBUG

/*
 * Define EXT2_PREALLOCATE to preallocate data blocks for expanding files
 */
#నిర్వచించు EXT2_PREALLOCATE
#నిర్వచించు EXT2_DEFAULT_PREALLOC_ఖండాలు	8

/*
 * The second extended file system version
 */
#నిర్వచించు EXT2FS_DATE		"95/08/09"
#నిర్వచించు EXT2_FS_సంస్కరణ		"0.5b"

/*
 * Special inode numbers
 */
#నిర్వచించు EXT2_BAD_INO		 1	/* Bad blocks inode */
#నిర్వచించు EXT2_మూలం_INO		 2	/* Root inode */
#నిర్వచించు EXT4_USR_QUOTA_INO	 3	/* User quota inode */
#నిర్వచించు EXT4_GRP_QUOTA_INO	 4	/* Group quota inode */
#నిర్వచించు EXT2_BOOT_LOADER_INO	 5	/* Boot loader inode */
#నిర్వచించు EXT2_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#నిర్వచించు EXT2_RESIZE_INO		 7	/* Reserved group descriptors inode */
#నిర్వచించు EXT2_JOURNAL_INO	 8	/* Journal inode */
#నిర్వచించు EXT2_EXCLUDE_INO	 9	/* The "exclude" inode, for snapshots */
#నిర్వచించు EXT4_REPLICA_INO	10	/* Used by non-upstream feature */

/* First non-reserved inode for old ext2 filesystems */
#నిర్వచించు EXT2_GOOD_OLD_మొదటి_INO	11

/*
 * The second extended file system magic number
 */
#నిర్వచించు EXT2_SUPER_MAGIC	0xEF53

#నిర్వచితమైతే __KERNEL__
#నిర్వచించు EXT2_SB(sb)	(&((sb)->u.ext2_sb))
#లేకపోతే
/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#నిర్వచించు EXT2_SB(sb)	(sb)
#అయితే_ముగింపు

/*
 * Maximal count of links to a file
 */
#నిర్వచించు EXT2_LINK_MAX		65000

/*
 * Macro-instructions used to manage several block sizes
 */
#నిర్వచించు EXT2_MIN_ఖండం_LOG_కొలత		10	/* 1024 */
#నిర్వచించు EXT2_MAX_ఖండం_LOG_కొలత		16	/* 65536 */
#నిర్వచించు EXT2_MIN_ఖండం_కొలత	(1 << EXT2_MIN_ఖండం_LOG_కొలత)
#నిర్వచించు EXT2_MAX_ఖండం_కొలత	(1 << EXT2_MAX_ఖండం_LOG_కొలత)
#నిర్వచితమైతే __KERNEL__
#నిర్వచించు EXT2_ఖండం_కొలత(s)	((s)->s_blocksize)
#నిర్వచించు EXT2_ఖండం_కొలత_బిట్లు(s)	((s)->s_blocksize_బిట్లు)
#నిర్వచించు EXT2_ADDR_చొప్పున_ఖండం_బిట్లు(s)	(EXT2_SB(s)->addr_చొప్పున_ఖండం_బిట్లు)
#నిర్వచించు EXT2_INODE_కొలత(s)	(EXT2_SB(s)->s_inode_కొలత)
#నిర్వచించు EXT2_మొదటి_INO(s)	(EXT2_SB(s)->s_మొదటి_ino)
#లేకపోతే
#నిర్వచించు EXT2_ఖండం_కొలత(s)	(EXT2_MIN_ఖండం_కొలత << (s)->s_log_ఖండం_కొలత)
#నిర్వచించు EXT2_ఖండం_కొలత_బిట్లు(s)	((s)->s_log_ఖండం_కొలత + 10)
#నిర్వచించు EXT2_INODE_కొలత(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_INODE_కొలత : (s)->s_inode_కొలత)
#నిర్వచించు EXT2_మొదటి_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_మొదటి_INO : (s)->s_మొదటి_ino)
#అయితే_ముగింపు
#నిర్వచించు EXT2_ADDR_చొప్పున_ఖండం(s)	(EXT2_ఖండం_కొలత(s) / పరిమాణం(__u32))

/*
 * Macro-instructions used to manage allocation clusters
 */
#నిర్వచించు EXT2_MIN_CLUSTER_LOG_కొలత	EXT2_MIN_ఖండం_LOG_కొలత
#నిర్వచించు EXT2_MAX_CLUSTER_LOG_కొలత	29	/* 512MB  */
#నిర్వచించు EXT2_MIN_CLUSTER_కొలత		EXT2_MIN_ఖండం_కొలత
#నిర్వచించు EXT2_MAX_CLUSTER_కొలత		(1 << EXT2_MAX_CLUSTER_LOG_కొలత)
#నిర్వచించు EXT2_CLUSTER_కొలత(s)		(EXT2_MIN_ఖండం_కొలత << \
						(s)->s_log_cluster_కొలత)
#నిర్వచించు EXT2_CLUSTER_కొలత_బిట్లు(s)	((s)->s_log_cluster_కొలత + 10)

/*
 * Macro-instructions used to manage fragments
 *
 * Note: for backwards compatibility only, for the dump program.
 * Ext2/3/4 will never support fragments....
 */
#నిర్వచించు EXT2_MIN_FRAG_కొలత              EXT2_MIN_ఖండం_కొలత
#నిర్వచించు EXT2_MAX_FRAG_కొలత              EXT2_MAX_ఖండం_కొలత
#నిర్వచించు EXT2_MIN_FRAG_LOG_కొలత          EXT2_MIN_ఖండం_LOG_కొలత
#నిర్వచించు EXT2_FRAG_కొలత(s)		EXT2_ఖండం_కొలత(s)
#నిర్వచించు EXT2_FRAGS_చొప్పున_ఖండం(s)		1

/*
 * ACL structures
 */
నిర్మాణం ext2_acl_శీర్షిక	/* Header of Access Control Lists */
{
	__u32	aclh_కొలత;
	__u32	aclh_file_లెక్క;
	__u32	aclh_acle_లెక్క;
	__u32	aclh_మొదటి_acle;
};

నిర్మాణం ext2_acl_నమోదు	/* Access Control List Entry */
{
	__u32	acle_కొలత;
	__u16	acle_perms;	/* Access permissions */
	__u16	acle_type;	/* Type of entry */
	__u16	acle_ట్యాగ్;	/* User or group identity */
	__u16	acle_pad1;
	__u32	acle_తదుపరి;	/* Pointer on next entry for the */
					/* same inode or on next free entry */
};

/*
 * Structure of a blocks group descriptor
 */
నిర్మాణం ext2_సమూహం_desc
{
	__u32	bg_ఖండం_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_పట్టిక;		/* Inodes table block */
	__u16	bg_free_ఖండాలు_లెక్క;	/* Free blocks count */
	__u16	bg_free_inodes_లెక్క;	/* Free inodes count */
	__u16	bg_వాడిన_dirs_లెక్క;	/* Directories count */
	__u16	bg_flags;
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_ఖండం_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_వాడని;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(s_uuid+group_num+group_desc)*/
};

/*
 * Structure of a blocks group descriptor
 */
నిర్మాణం ext4_సమూహం_desc
{
	__u32	bg_ఖండం_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_పట్టిక;		/* Inodes table block */
	__u16	bg_free_ఖండాలు_లెక్క;	/* Free blocks count */
	__u16	bg_free_inodes_లెక్క;	/* Free inodes count */
	__u16	bg_వాడిన_dirs_లెక్క;	/* Directories count */
	__u16	bg_flags;		/* EXT4_BG_flags (INODE_UNINIT, etc) */
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_ఖండం_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_వాడని;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(sb_uuid+group+desc) */
	__u32	bg_ఖండం_bitmap_hi;	/* Blocks bitmap block MSB */
	__u32	bg_inode_bitmap_hi;	/* Inodes bitmap block MSB */
	__u32	bg_inode_పట్టిక_hi;	/* Inodes table block MSB */
	__u16	bg_free_ఖండాలు_లెక్క_hi;/* Free blocks count MSB */
	__u16	bg_free_inodes_లెక్క_hi;/* Free inodes count MSB */
	__u16	bg_వాడిన_dirs_లెక్క_hi;	/* Directories count MSB */
	__u16	bg_itable_వాడని_hi;	/* Unused inodes count MSB */
	__u32	bg_exclude_bitmap_hi;	/* Exclude bitmap block MSB */
	__u16	bg_ఖండం_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u16	bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u32	bg_reserved;
};

#నిర్వచించు EXT4_BG_INODE_BITMAP_CSUM_HI_చివర	\
	(offsetof(నిర్మాణం ext4_సమూహం_desc, bg_inode_bitmap_csum_hi) + \
	 పరిమాణం(__u16))
#నిర్వచించు EXT4_BG_ఖండం_BITMAP_CSUM_HI_LOCATION	\
	(offsetof(నిర్మాణం ext4_సమూహం_desc, bg_ఖండం_bitmap_csum_hi) + \
	 పరిమాణం(__u16))

#నిర్వచించు EXT2_BG_INODE_UNINIT	0x0001 /* Inode table/bitmap not initialized */
#నిర్వచించు EXT2_BG_ఖండం_UNINIT	0x0002 /* Block bitmap not initialized */
#నిర్వచించు EXT2_BG_INODE_ZEROED	0x0004 /* On-disk itable initialized to zero */

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
నిర్మాణం ext2_dx_మూలం_info {
	__u32 reserved_zero;
	__u8 hash_సంస్కరణ; /* 0 now, 1 at release */
	__u8 info_నిడివి; /* 8 */
	__u8 పరోక్ష_స్థాయిలు;
	__u8 వాడని_flags;
};

#నిర్వచించు EXT2_HASH_LEGACY		0
#నిర్వచించు EXT2_HASH_HALF_MD4		1
#నిర్వచించు EXT2_HASH_TEA			2
#నిర్వచించు EXT2_HASH_LEGACY_UNSIGNED	3 /* reserved for userspace lib */
#నిర్వచించు EXT2_HASH_HALF_MD4_UNSIGNED	4 /* reserved for userspace lib */
#నిర్వచించు EXT2_HASH_TEA_UNSIGNED		5 /* reserved for userspace lib */
#నిర్వచించు EXT2_HASH_SIPHASH		6

#నిర్వచించు EXT2_HASH_FLAG_INCOMPAT	0x1

#నిర్వచించు EXT4_DX_ఖండం_ముసుగు 0x0fffffff

నిర్మాణం ext2_dx_నమోదు {
	__le32 hash;
	__le32 ఖండం;
};

నిర్మాణం ext2_dx_countlimit {
	__le16 హద్దు;
	__le16 లెక్క;
};

/*
 * This goes at the end of each htree block.
 */
నిర్మాణం ext2_dx_తోక {
	__le32 dt_reserved;
	__le32 dt_checksum;	/* crc32c(uuid+inum+dxblock) */
};

/*
 * Macro-instructions used to manage group descriptors
 */
#నిర్వచించు EXT2_MIN_DESC_కొలత             32
#నిర్వచించు EXT2_MIN_DESC_కొలత_64_బిట్టు       64
#నిర్వచించు EXT2_MAX_DESC_కొలత             EXT2_MIN_ఖండం_కొలత
#నిర్వచించు EXT2_DESC_కొలత(s)                                                \
	(ext2fs_has_feature_64bit(s) ? (s)->s_desc_కొలత : EXT2_MIN_DESC_కొలత)

#నిర్వచించు EXT2_ఖండాలు_చొప్పున_సమూహం(s)	(EXT2_SB(s)->s_ఖండాలు_చొప్పున_సమూహం)
#నిర్వచించు EXT2_INODES_చొప్పున_సమూహం(s)	(EXT2_SB(s)->s_inodes_చొప్పున_సమూహం)
#నిర్వచించు EXT2_CLUSTERS_చొప్పున_సమూహం(s)	(EXT2_SB(s)->s_clusters_చొప్పున_సమూహం)
#నిర్వచించు EXT2_INODES_చొప్పున_ఖండం(s)	(EXT2_ఖండం_కొలత(s)/EXT2_INODE_కొలత(s))
/* limits imposed by 16-bit value gd_free_{blocks,inode}_count */
#నిర్వచించు EXT2_MAX_ఖండాలు_చొప్పున_సమూహం(s)	((((గుర్తులేని) 1 << 16) - 8) *	\
					 (EXT2_CLUSTER_కొలత(s) / \
					  EXT2_ఖండం_కొలత(s)))
#నిర్వచించు EXT2_MAX_CLUSTERS_చొప్పున_సమూహం(s)	(((గుర్తులేని) 1 << 16) - 8)
#నిర్వచించు EXT2_MAX_INODES_చొప్పున_సమూహం(s)	(((గుర్తులేని) 1 << 16) - \
					 EXT2_INODES_చొప్పున_ఖండం(s))
#నిర్వచితమైతే __KERNEL__
#నిర్వచించు EXT2_DESC_చొప్పున_ఖండం(s)		(EXT2_SB(s)->s_desc_చొప్పున_ఖండం)
#నిర్వచించు EXT2_DESC_చొప్పున_ఖండం_బిట్లు(s)	(EXT2_SB(s)->s_desc_చొప్పున_ఖండం_బిట్లు)
#లేకపోతే
#నిర్వచించు EXT2_DESC_చొప్పున_ఖండం(s)		(EXT2_ఖండం_కొలత(s) / EXT2_DESC_కొలత(s))
#అయితే_ముగింపు

#నిర్వచించు EXT2_GROUPS_గమ్యం_ఖండాలు(s, g)   ((blk64_t) EXT2_ఖండాలు_చొప్పున_సమూహం(s) * \
				       (g))
#నిర్వచించు EXT2_GROUPS_గమ్యం_CLUSTERS(s, g) ((blk64_t) EXT2_CLUSTERS_చొప్పున_సమూహం(s) * \
				       (g))

/*
 * Constants relative to the data blocks
 */
#నిర్వచించు EXT2_NDIR_ఖండాలు		12
#నిర్వచించు EXT2_IND_ఖండం			EXT2_NDIR_ఖండాలు
#నిర్వచించు EXT2_DIND_ఖండం			(EXT2_IND_ఖండం + 1)
#నిర్వచించు EXT2_TIND_ఖండం			(EXT2_DIND_ఖండం + 1)
#నిర్వచించు EXT2_N_ఖండాలు			(EXT2_TIND_ఖండం + 1)

/*
 * Inode flags
 */
#నిర్వచించు EXT2_SECRM_FL			0x00000001 /* Secure deletion */
#నిర్వచించు EXT2_UNRM_FL			0x00000002 /* Undelete */
#నిర్వచించు EXT2_COMPR_FL			0x00000004 /* Compress file */
#నిర్వచించు EXT2_SYNC_FL			0x00000008 /* Synchronous updates */
#నిర్వచించు EXT2_IMMUTABLE_FL		0x00000010 /* Immutable file */
#నిర్వచించు EXT2_APPEND_FL			0x00000020 /* writes to file may only append */
#నిర్వచించు EXT2_NODUMP_FL			0x00000040 /* do not dump file */
#నిర్వచించు EXT2_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#నిర్వచించు EXT2_DIRTY_FL			0x00000100
#నిర్వచించు EXT2_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#నిర్వచించు EXT2_NOCOMPR_FL			0x00000400 /* Access raw compressed data */
	/* nb: was previously EXT2_ECOMPR_FL */
#నిర్వచించు EXT4_ENCRYPT_FL			0x00000800 /* encrypted inode */
/* End compression flags --- maybe not all used */
#నిర్వచించు EXT2_BTREE_FL			0x00001000 /* btree format dir */
#నిర్వచించు EXT2_సూచీ_FL			0x00001000 /* hash-indexed directory */
#నిర్వచించు EXT2_IMAGIC_FL			0x00002000
#నిర్వచించు EXT3_JOURNAL_దత్తాంశం_FL		0x00004000 /* file data should be journaled */
#నిర్వచించు EXT2_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#నిర్వచించు EXT2_DIRSYNC_FL 		0x00010000 /* Synchronous directory modifications */
#నిర్వచించు EXT2_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#నిర్వచించు EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#నిర్వచించు EXT4_EXTENTS_FL 		0x00080000 /* Inode uses extents */
#నిర్వచించు EXT4_VERITY_FL			0x00100000 /* Verity protected inode */
#నిర్వచించు EXT4_EA_INODE_FL	        0x00200000 /* Inode used for large EA */
/* EXT4_EOFBLOCKS_FL 0x00400000 was here */
#నిర్వచించు FS_NOCOW_FL			0x00800000 /* Do not cow file */
#నిర్వచించు EXT4_SNAPFILE_FL		0x01000000  /* Inode is a snapshot */
#నిర్వచించు FS_DAX_FL			0x02000000 /* Inode is DAX */
#నిర్వచించు EXT4_SNAPFILE_DELETED_FL	0x04000000  /* Snapshot is being deleted */
#నిర్వచించు EXT4_SNAPFILE_SHRUNK_FL		0x08000000  /* Snapshot shrink has completed */
#నిర్వచించు EXT4_INLINE_దత్తాంశం_FL		0x10000000 /* Inode has inline data */
#నిర్వచించు EXT4_PROJINHERIT_FL		0x20000000 /* Create with parents projid */
#నిర్వచించు EXT4_CASEFOLD_FL		0x40000000 /* Casefolded file */
#నిర్వచించు EXT2_RESERVED_FL		0x80000000 /* reserved for ext2 lib */

#నిర్వచించు EXT2_FL_USER_VISIBLE		0x604BDFFF /* User visible flags */
#నిర్వచించు EXT2_FL_USER_MODIFIABLE		0x604B80FF /* User modifiable flags */

/*
 * ioctl commands
 */

/* Used for online resize */
నిర్మాణం ext2_new_సమూహం_ఆదానం {
	__u32 సమూహం;		/* Group number for this data */
	__u32 ఖండం_bitmap;	/* Absolute block number of block bitmap */
	__u32 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u32 inode_పట్టిక;	/* Absolute block number of inode table start */
	__u32 ఖండాలు_లెక్క;	/* Total number of blocks in this group */
	__u16 reserved_ఖండాలు;	/* Number of reserved blocks in this group */
	__u16 వాడని;		/* Number of reserved GDT blocks in group */
};

నిర్మాణం ext4_new_సమూహం_ఆదానం {
	__u32 సమూహం;		/* Group number for this data */
	__u64 ఖండం_bitmap;	/* Absolute block number of block bitmap */
	__u64 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u64 inode_పట్టిక;	/* Absolute block number of inode table start */
	__u32 ఖండాలు_లెక్క;	/* Total number of blocks in this group */
	__u16 reserved_ఖండాలు;	/* Number of reserved blocks in this group */
	__u16 వాడని;
};

#నిర్వచితమైతే __GNU__			/* Needed for the Hurd */
#నిర్వచించు IOT_ext2_new_సమూహం_ఆదానం _IOT (_IOTS(__u32), 5, _IOTS(__u16), 2, 0, 0)
#అయితే_ముగింపు

#నిర్వచించు EXT2_IOC_GETFLAGS		_IOR('f', 1, దీర్ఘ)
#నిర్వచించు EXT2_IOC_SETFLAGS		_IOW('f', 2, దీర్ఘ)
#నిర్వచించు EXT2_IOC_GETVERSION		_IOR('v', 1, దీర్ఘ)
#నిర్వచించు EXT2_IOC_SETVERSION		_IOW('v', 2, దీర్ఘ)
#నిర్వచించు EXT2_IOC_GETVERSION_NEW		_IOR('f', 3, దీర్ఘ)
#నిర్వచించు EXT2_IOC_SETVERSION_NEW		_IOW('f', 4, దీర్ఘ)
#నిర్వచించు EXT2_IOC_సమూహం_EXTEND		_IOW('f', 7, గుర్తులేని దీర్ఘ)
#నిర్వచించు EXT2_IOC_సమూహం_కూడు		_IOW('f', 8,నిర్మాణం ext2_new_సమూహం_ఆదానం)
#నిర్వచించు EXT4_IOC_సమూహం_కూడు		_IOW('f', 8,నిర్మాణం ext4_new_సమూహం_ఆదానం)
#నిర్వచించు EXT4_IOC_RESIZE_FS		_IOW('f', 16, __u64)

/*
 * Structure of an inode on the disk
 */
నిర్మాణం ext2_inode {
/*00*/	__u16	i_బహుళకం;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_కొలత;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_లెక్క;	/* Links count */
	__u32	i_ఖండాలు;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	సంఘం {
		నిర్మాణం {
			__u32	l_i_సంస్కరణ; /* was l_i_reserved1 */
		} linux1;
		నిర్మాణం {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_ఖండం[EXT2_N_ఖండాలు];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_కొలత_ఎగువ;
/*70*/	__u32	i_faddr;	/* Fragment address */
	సంఘం {
		నిర్మాణం {
			__u16	l_i_ఖండాలు_hi;
			__u16	l_i_file_acl_ఎగువ;
			__u16	l_i_uid_ఎగువ;	/* these 2 fields    */
			__u16	l_i_gid_ఎగువ;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		నిర్మాణం {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_బహుళకం_ఎగువ;
			__u16	h_i_uid_ఎగువ;
			__u16	h_i_gid_ఎగువ;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
};

/*
 * Permanent part of an large inode on the disk
 */
నిర్మాణం ext2_inode_large {
/*00*/	__u16	i_బహుళకం;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_కొలత;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode Change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_లెక్క;	/* Links count */
	__u32	i_ఖండాలు;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	సంఘం {
		నిర్మాణం {
			__u32	l_i_సంస్కరణ; /* was l_i_reserved1 */
		} linux1;
		నిర్మాణం {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_ఖండం[EXT2_N_ఖండాలు];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_కొలత_ఎగువ;
/*70*/	__u32	i_faddr;	/* Fragment address */
	సంఘం {
		నిర్మాణం {
			__u16	l_i_ఖండాలు_hi;
			__u16	l_i_file_acl_ఎగువ;
			__u16	l_i_uid_ఎగువ;	/* these 2 fields    */
			__u16	l_i_gid_ఎగువ;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		నిర్మాణం {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_బహుళకం_ఎగువ;
			__u16	h_i_uid_ఎగువ;
			__u16	h_i_gid_ఎగువ;
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
	__u32	i_సంస్కరణ_hi;	/* high 32 bits for 64-bit version */
/*9c*/	__u32   i_projid;       /* Project ID */
};

#నిర్వచించు EXT4_INODE_CSUM_HI_EXTRA_చివర	\
	(offsetof(నిర్మాణం ext2_inode_large, i_checksum_hi) + పరిమాణం(__u16) - \
	 EXT2_GOOD_OLD_INODE_కొలత)

#నిర్వచించు EXT4_EPOCH_బిట్లు 2
#నిర్వచించు EXT4_EPOCH_ముసుగు ((1 << EXT4_EPOCH_బిట్లు) - 1)

#నిర్వచించు i_checksum_lo	osd2.linux2.l_i_checksum_lo

#నిర్వచించు ext2fs_inode_includes(కొలత, క్షేత్రం)				\
	((కొలత) >= (పరిమాణం(((నిర్మాణం ext2_inode_large *)0)->క్షేత్రం) +	\
		    offsetof(నిర్మాణం ext2_inode_large, క్షేత్రం)))

#ఒకవేళ defined(__KERNEL__) || defined(__linux__)
#నిర్వచించు i_reserved1	osd1.linux1.l_i_reserved1
#నిర్వచించు i_frag		osd2.linux2.l_i_frag
#నిర్వచించు i_fsize		osd2.linux2.l_i_fsize
#నిర్వచించు i_uid_దిగువ	i_uid
#నిర్వచించు i_gid_దిగువ	i_gid
#నిర్వచించు i_uid_ఎగువ	osd2.linux2.l_i_uid_ఎగువ
#నిర్వచించు i_gid_ఎగువ	osd2.linux2.l_i_gid_ఎగువ
#లేకపోతే
#ఒకవేళ defined(__GNU__)

#నిర్వచించు i_translator	osd1.hurd1.h_i_translator
#నిర్వచించు i_frag		osd2.hurd2.h_i_frag;
#నిర్వచించు i_fsize		osd2.hurd2.h_i_fsize;
#నిర్వచించు i_uid_ఎగువ	osd2.hurd2.h_i_uid_ఎగువ
#నిర్వచించు i_gid_ఎగువ	osd2.hurd2.h_i_gid_ఎగువ
#నిర్వచించు i_author	osd2.hurd2.h_i_author

#అయితే_ముగింపు  /* __GNU__ */
#అయితే_ముగింపు	/* defined(__KERNEL__) || defined(__linux__) */

#నిర్వచించు inode_uid(inode)	((inode).i_uid | (గుర్తులేని)(inode).osd2.linux2.l_i_uid_ఎగువ << 16)
#నిర్వచించు inode_gid(inode)	((inode).i_gid | (గుర్తులేని)(inode).osd2.linux2.l_i_gid_ఎగువ << 16)
#నిర్వచించు inode_projid(inode)	((inode).i_projid)
#నిర్వచించు ext2fs_సమితి_i_uid_ఎగువ(inode,x) ((inode).osd2.linux2.l_i_uid_ఎగువ = (x))
#నిర్వచించు ext2fs_సమితి_i_gid_ఎగువ(inode,x) ((inode).osd2.linux2.l_i_gid_ఎగువ = (x))

స్థిర అంతఃవరుస
నిర్మాణం ext2_inode *EXT2_INODE(నిర్మాణం ext2_inode_large *large_inode)
{
	తిరిగి (నిర్మాణం ext2_inode *) large_inode;
}

/*
 * File system states
 */
#నిర్వచించు EXT2_VALID_FS			0x0001	/* Unmounted cleanly */
#నిర్వచించు EXT2_ERROR_FS			0x0002	/* Errors detected */
#నిర్వచించు EXT3_ORPHAN_FS			0x0004	/* Orphans being recovered */
#నిర్వచించు EXT4_FC_REPLAY			0x0020	/* Ext4 fast commit replay ongoing */

/*
 * Misc. filesystem flags
 */
#నిర్వచించు EXT2_FLAGS_SIGNED_HASH		0x0001  /* Signed dirhash in use */
#నిర్వచించు EXT2_FLAGS_UNSIGNED_HASH	0x0002  /* Unsigned dirhash in use */
#నిర్వచించు EXT2_FLAGS_TEST_FILESYS		0x0004	/* OK for use on development code */
#నిర్వచించు EXT2_FLAGS_IS_SNAPSHOT		0x0010	/* This is a snapshot image */
#నిర్వచించు EXT2_FLAGS_FIX_SNAPSHOT		0x0020	/* Snapshot inodes corrupted */
#నిర్వచించు EXT2_FLAGS_FIX_EXCLUDE		0x0040	/* Exclude bitmaps corrupted */

/*
 * Mount flags
 */
#నిర్వచించు EXT2_MOUNT_CHECK		0x0001	/* Do mount-time checks */
#నిర్వచించు EXT2_MOUNT_GRPID		0x0004	/* Create files with directory's group */
#నిర్వచించు EXT2_MOUNT_DEBUG		0x0008	/* Some debugging messages */
#నిర్వచించు EXT2_MOUNT_ERRORS_CONT		0x0010	/* Continue on errors */
#నిర్వచించు EXT2_MOUNT_ERRORS_RO		0x0020	/* Remount fs ro on errors */
#నిర్వచించు EXT2_MOUNT_ERRORS_PANIC		0x0040	/* Panic on errors */
#నిర్వచించు EXT2_MOUNT_MINIX_DF		0x0080	/* Mimics the Minix statfs */
#నిర్వచించు EXT2_MOUNT_NO_UID32		0x0200  /* Disable 32-bit UIDs */

#నిర్వచించు clear_opt(o, opt)		o &= ~EXT2_MOUNT_##opt
#నిర్వచించు సమితి_opt(o, opt)			o |= EXT2_MOUNT_##opt
#నిర్వచించు test_opt(sb, opt)		(EXT2_SB(sb)->s_mount_opt & \
					 EXT2_MOUNT_##opt)
/*
 * Maximal mount counts between two filesystem checks
 */
#నిర్వచించు EXT2_DFL_MAX_MNT_లెక్క		20	/* Allow 20 mounts */
#నిర్వచించు EXT2_DFL_CHECKINTERVAL		0	/* Don't use interval check */

/*
 * Behaviour when detecting errors
 */
#నిర్వచించు EXT2_ERRORS_CONTINUE		1	/* Continue execution */
#నిర్వచించు EXT2_ERRORS_RO			2	/* Remount fs read-only */
#నిర్వచించు EXT2_ERRORS_PANIC		3	/* Panic */
#నిర్వచించు EXT2_ERRORS_DEFAULT		EXT2_ERRORS_CONTINUE

#ఒకవేళ (__GNUC__ >= 4)
#నిర్వచించు ext4_offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#లేకపోతే
#నిర్వచించు ext4_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#అయితే_ముగింపు

/* Metadata checksum algorithms */
#నిర్వచించు EXT2_చక్రీయ_తనిఖీ_32_C_CHKSUM		1

/* Encryption algorithms, key size and key reference len */
#నిర్వచించు EXT4_ENCRYPTION_బహుళకం_INVALID		0
#నిర్వచించు EXT4_ENCRYPTION_బహుళకం_AES_256_XTS	1
#నిర్వచించు EXT4_ENCRYPTION_బహుళకం_AES_256_GCM	2
#నిర్వచించు EXT4_ENCRYPTION_బహుళకం_AES_256_CBC	3
#నిర్వచించు EXT4_ENCRYPTION_బహుళకం_AES_256_CTS	4

#నిర్వచించు EXT4_AES_256_XTS_కీలకం_కొలత		64
#నిర్వచించు EXT4_AES_256_GCM_కీలకం_కొలత		32
#నిర్వచించు EXT4_AES_256_CBC_కీలకం_కొలత		32
#నిర్వచించు EXT4_AES_256_CTS_కీలకం_కొలత		32
#నిర్వచించు EXT4_MAX_కీలకం_కొలత			64

#నిర్వచించు EXT4_కీలకం_DESCRIPTOR_కొలత		8
#నిర్వచించు EXT4_CRYPTO_ఖండం_కొలత			16

/* Password derivation constants */
#నిర్వచించు EXT4_MAX_PASSPHRASE_కొలత		1024
#నిర్వచించు EXT4_MAX_SALT_కొలత			256
#నిర్వచించు EXT4_PBKDF2_ITERATIONS			0xFFFF

#నిర్వచించు EXT2_LABEL_LEN				16

/*
 * Policy provided via an ioctl on the topmost directory. This
 * structure is also in the kernel.
 */
నిర్మాణం ext4_encryption_policy {
  అక్షరం సంస్కరణ;
  అక్షరం contents_encryption_బహుళకం;
  అక్షరం filenames_encryption_బహుళకం;
  అక్షరం flags;
  అక్షరం master_కీలకం_descriptor[EXT4_కీలకం_DESCRIPTOR_కొలత];
} __attribute__((__packed__));

నిర్మాణం ext4_encryption_కీలకం {
        __u32 బహుళకం;
        అక్షరం raw[EXT4_MAX_కీలకం_కొలత];
        __u32 కొలత;
} __attribute__((__packed__));

/*
 * Structure of the super block
 */
నిర్మాణం ext2_super_ఖండం {
/*000*/	__u32	s_inodes_లెక్క;		/* Inodes count */
	__u32	s_ఖండాలు_లెక్క;		/* Blocks count */
	__u32	s_r_ఖండాలు_లెక్క;	/* Reserved blocks count */
	__u32	s_free_ఖండాలు_లెక్క;	/* Free blocks count */
/*010*/	__u32	s_free_inodes_లెక్క;	/* Free inodes count */
	__u32	s_మొదటి_దత్తాంశం_ఖండం;	/* First Data Block */
	__u32	s_log_ఖండం_కొలత;	/* Block size */
	__u32	s_log_cluster_కొలత;	/* Allocation cluster size */
/*020*/	__u32	s_ఖండాలు_చొప్పున_సమూహం;	/* # Blocks per group */
	__u32	s_clusters_చొప్పున_సమూహం;	/* # Fragments per group */
	__u32	s_inodes_చొప్పున_సమూహం;	/* # Inodes per group */
	__u32	s_mtime;		/* Mount time */
/*030*/	__u32	s_wtime;		/* Write time */
	__u16	s_mnt_లెక్క;		/* Mount count */
	__s16	s_max_mnt_లెక్క;	/* Maximal mount count */
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
	__u32	s_మొదటి_ino;		/* First non-reserved inode */
	__u16   s_inode_కొలత;		/* size of inode structure */
	__u16	s_ఖండం_సమూహం_nr;	/* block group # of this superblock */
	__u32	s_feature_compat;	/* compatible feature set */
/*060*/	__u32	s_feature_incompat;	/* incompatible feature set */
	__u32	s_feature_ro_compat;	/* readonly-compatible feature set */
/*068*/	__u8	s_uuid[16] __nonstring;		/* 128-bit uuid for volume */
/*078*/	__u8	s_volume_name[EXT2_LABEL_LEN] __nonstring;	/* volume name, no NUL? */
/*088*/	__u8	s_చివరి_mounted[64] __nonstring;	/* directory last mounted on, no NUL? */
/*0c8*/	__u32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_ఖండాలు;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_ఖండాలు;	/* Nr to preallocate for dirs */
	__u16	s_reserved_gdt_ఖండాలు;	/* Per group table for online growth */
	/*
	 * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
/*0d0*/	__u8	s_journal_uuid[16] __nonstring;	/* uuid of journal superblock */
/*0e0*/	__u32	s_journal_inum;		/* inode number of journal file */
	__u32	s_journal_dev;		/* device number of journal file */
	__u32	s_చివరి_orphan;		/* start of list of inodes to delete */
/*0ec*/	__u32	s_hash_విత్తనం[4];		/* HTREE hash seed */
/*0fc*/	__u8	s_def_hash_సంస్కరణ;	/* Default hash version to use */
	__u8	s_jnl_backup_type;	/* Default type of journal backup */
	__u16	s_desc_కొలత;		/* Group desc. size: INCOMPAT_64BIT */
/*100*/	__u32	s_default_mount_opts;	/* default EXT2_MOUNT_* flags used */
	__u32	s_మొదటి_meta_bg;	/* First metablock group */
	__u32	s_mkfs_సమయం;		/* When the filesystem was created */
/*10c*/	__u32	s_jnl_ఖండాలు[17];	/* Backup of the journal inode */
/*150*/	__u32	s_ఖండాలు_లెక్క_hi;	/* Blocks count high 32bits */
	__u32	s_r_ఖండాలు_లెక్క_hi;	/* Reserved blocks count high 32 bits*/
	__u32	s_free_ఖండాలు_hi;	/* Free blocks count */
	__u16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__u16	s_want_extra_isize;	/* New inodes should reserve # bytes */
/*160*/	__u32	s_flags;		/* Miscellaneous flags */
	__u16	s_raid_stride;		/* RAID stride in blocks */
	__u16	s_mmp_update_interval;  /* # seconds to wait in MMP checking */
	__u64	s_mmp_ఖండం;		/* Block for multi-mount protection */
/*170*/	__u32	s_raid_stripe_వెడల్పు;	/* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_చొప్పున_flex;	/* FLEX_BG group size */
	__u8	s_checksum_type;	/* metadata checksum algorithm */
	__u8	s_encryption_level;	/* versioning level for encryption */
	__u8	s_reserved_pad;		/* Padding to next 32bits */
	__u64	s_kbytes_written;	/* nr of lifetime kilobytes written */
/*180*/	__u32	s_snapshot_inum;	/* Inode number of active snapshot */
	__u32	s_snapshot_id;		/* sequential ID of active snapshot */
	__u64	s_snapshot_r_ఖండాలు_లెక్క; /* active snapshot reserved blocks */
/*190*/	__u32	s_snapshot_list;	/* inode number of disk snapshot list */
#నిర్వచించు EXT4_S_ERR_ప్రారంభం ext4_offsetof(నిర్మాణం ext2_super_ఖండం, s_error_లెక్క)
	__u32	s_error_లెక్క;		/* number of fs errors */
	__u32	s_మొదటి_error_సమయం;	/* first time an error happened */
	__u32	s_మొదటి_error_ino;	/* inode involved in first error */
/*1a0*/	__u64	s_మొదటి_error_ఖండం;	/* block involved in first error */
	__u8	s_మొదటి_error_func[32] __nonstring;	/* function where error hit, no NUL? */
/*1c8*/	__u32	s_మొదటి_error_పంక్తి;	/* line number where error happened */
	__u32	s_చివరి_error_సమయం;	/* most recent time of an error */
/*1d0*/	__u32	s_చివరి_error_ino;	/* inode involved in last error */
	__u32	s_చివరి_error_పంక్తి;	/* line number where error happened */
	__u64	s_చివరి_error_ఖండం;	/* block involved of last error */
/*1e0*/	__u8	s_చివరి_error_func[32] __nonstring;	/* function where error hit, no NUL? */
#నిర్వచించు EXT4_S_ERR_చివర ext4_offsetof(నిర్మాణం ext2_super_ఖండం, s_mount_opts)
/*200*/	__u8	s_mount_opts[64] __nonstring;	/* default mount options, no NUL? */
/*240*/	__u32	s_usr_quota_inum;	/* inode number of user quota file */
	__u32	s_grp_quota_inum;	/* inode number of group quota file */
	__u32	s_overhead_clusters;	/* overhead blocks/clusters in fs */
/*24c*/	__u32	s_backup_bgs[2];	/* If sparse_super2 enabled */
/*254*/	__u8	s_encrypt_algos[4];	/* Encryption algorithms in use  */
/*258*/	__u8	s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
/*268*/	__le32	s_lpf_ino;		/* Location of the lost+found inode */
	__le32  s_prj_quota_inum;	/* inode for tracking project quota */
/*270*/	__le32	s_checksum_విత్తనం;	/* crc32c(orig_uuid) if csum_seed set */
/*274*/	__u8	s_wtime_hi;
	__u8	s_mtime_hi;
	__u8	s_mkfs_సమయం_hi;
	__u8	s_lastcheck_hi;
	__u8	s_మొదటి_error_సమయం_hi;
	__u8	s_చివరి_error_సమయం_hi;
	__u8	s_మొదటి_error_errcode;
	__u8    s_చివరి_error_errcode;
/*27c*/ __le16	s_encoding;		/* Filename charset encoding */
	__le16	s_encoding_flags;	/* Filename charset encoding flags */
	__le32  s_orphan_file_inum;	/* Inode for tracking orphan inodes */
	__le32	s_reserved[94];		/* Padding to the end of the block */
/*3fc*/	__u32	s_checksum;		/* crc32c(superblock) */
};

#నిర్వచించు EXT4_S_ERR_LEN (EXT4_S_ERR_చివర - EXT4_S_ERR_ప్రారంభం)
#నిర్వచించు EXT2_LEN_STR(buf) (పూర్ణాంకం)పరిమాణం(buf), (అక్షరం *)buf

/*
 * Codes for operating systems
 */
#నిర్వచించు EXT2_OS_LINUX		0
#నిర్వచించు EXT2_OS_HURD		1
#నిర్వచించు EXT2_OBSO_OS_MASIX	2
#నిర్వచించు EXT2_OS_FREEBSD		3
#నిర్వచించు EXT2_OS_LITES		4

/*
 * Revision levels
 */
#నిర్వచించు EXT2_GOOD_OLD_REV	0	/* The good old (original) format */
#నిర్వచించు EXT2_DYNAMIC_REV	1	/* V2 format w/ dynamic inode sizes */

#నిర్వచించు EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV
#నిర్వచించు EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV

#నిర్వచించు EXT2_GOOD_OLD_INODE_కొలత 128

#నిర్వచించు EXT4_EXTRA_TIMESTAMP_MAX	(((int64_t)1 << 34) - 1  + INT32_MIN)
#నిర్వచించు EXT4_NON_EXTRA_TIMESTAMP_MAX	INT32_MAX
#నిర్వచించు EXT4_TIMESTAMP_MIN		INT32_MIN

/*
 * Journal inode backup types
 */
#నిర్వచించు EXT3_JNL_BACKUP_ఖండాలు	1

/*
 * Feature set definitions
 */

#నిర్వచించు EXT2_HAS_COMPAT_FEATURE(sb,ముసుగు)			\
	( EXT2_SB(sb)->s_feature_compat & (ముసుగు) )
#నిర్వచించు EXT2_HAS_RO_COMPAT_FEATURE(sb,ముసుగు)			\
	( EXT2_SB(sb)->s_feature_ro_compat & (ముసుగు) )
#నిర్వచించు EXT2_HAS_INCOMPAT_FEATURE(sb,ముసుగు)			\
	( EXT2_SB(sb)->s_feature_incompat & (ముసుగు) )

#నిర్వచించు EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001
#నిర్వచించు EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002
#నిర్వచించు EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
#నిర్వచించు EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008
#నిర్వచించు EXT2_FEATURE_COMPAT_RESIZE_INODE	0x0010
#నిర్వచించు EXT2_FEATURE_COMPAT_DIR_సూచీ		0x0020
#నిర్వచించు EXT2_FEATURE_COMPAT_LAZY_BG		0x0040
/* #define EXT2_FEATURE_COMPAT_EXCLUDE_INODE	0x0080 not used, legacy */
#నిర్వచించు EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP	0x0100
#నిర్వచించు EXT4_FEATURE_COMPAT_SPARSE_SUPER2	0x0200
#నిర్వచించు EXT4_FEATURE_COMPAT_FAST_COMMIT		0x0400
#నిర్వచించు EXT4_FEATURE_COMPAT_STABLE_INODES	0x0800
#నిర్వచించు EXT4_FEATURE_COMPAT_ORPHAN_FILE		0x1000

#నిర్వచించు EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#నిర్వచించు EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004 not used */
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_HUGE_FILE	0x0008
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_GDT_CSUM		0x0010
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_DIR_NLINK	0x0020
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE	0x0040
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT	0x0080
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_QUOTA		0x0100
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_BIGALLOC		0x0200
/*
 * METADATA_CSUM implies GDT_CSUM.  When METADATA_CSUM is set, group
 * descriptor checksums use the same algorithm as all other data
 * structures' checksums.
 */
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_METADATA_CSUM	0x0400
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_REPLICA		0x0800
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_READONLY		0x1000
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_PROJECT		0x2000 /* Project quota */
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_SHARED_ఖండాలు	0x4000
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_VERITY		0x8000
#నిర్వచించు EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT	0x10000

#నిర్వచించు EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001
#నిర్వచించు EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002
#నిర్వచించు EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 /* Needs recovery */
#నిర్వచించు EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 /* Journal device */
#నిర్వచించు EXT2_FEATURE_INCOMPAT_META_BG		0x0010
#నిర్వచించు EXT3_FEATURE_INCOMPAT_EXTENTS		0x0040
#నిర్వచించు EXT4_FEATURE_INCOMPAT_64_బిట్టు		0x0080
#నిర్వచించు EXT4_FEATURE_INCOMPAT_MMP		0x0100
#నిర్వచించు EXT4_FEATURE_INCOMPAT_FLEX_BG		0x0200
#నిర్వచించు EXT4_FEATURE_INCOMPAT_EA_INODE		0x0400
#నిర్వచించు EXT4_FEATURE_INCOMPAT_DIRDATA		0x1000
#నిర్వచించు EXT4_FEATURE_INCOMPAT_CSUM_విత్తనం		0x2000
#నిర్వచించు EXT4_FEATURE_INCOMPAT_LARGEDIR		0x4000 /* >2GB or 3-lvl htree */
#నిర్వచించు EXT4_FEATURE_INCOMPAT_INLINE_దత్తాంశం	0x8000 /* data in inode */
#నిర్వచించు EXT4_FEATURE_INCOMPAT_ENCRYPT		0x10000
#నిర్వచించు EXT4_FEATURE_INCOMPAT_CASEFOLD		0x20000

#నిర్వచించు EXT4_FEATURE_COMPAT_FUNCS(name, ver, flagname) \
స్థిర అంతఃవరుస పూర్ణాంకం ext2fs_has_feature_##name(నిర్మాణం ext2_super_ఖండం *sb) \
{ \
	తిరిగి ((EXT2_SB(sb)->s_feature_compat & \
		 EXT##ver##_FEATURE_COMPAT_##flagname) != 0); \
} \
స్థిర అంతఃవరుస శూన్య ext2fs_సమితి_feature##name(నిర్మాణం ext2_super_ఖండం *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat |= \
		EXT##ver##_FEATURE_COMPAT_##flagname; \
} \
స్థిర అంతఃవరుస శూన్య ext2fs_clear_feature_##name(నిర్మాణం ext2_super_ఖండం *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat &= \
		~EXT##ver##_FEATURE_COMPAT_##flagname; \
}

#నిర్వచించు EXT4_FEATURE_RO_COMPAT_FUNCS(name, ver, flagname) \
స్థిర అంతఃవరుస పూర్ణాంకం ext2fs_has_feature_##name(నిర్మాణం ext2_super_ఖండం *sb) \
{ \
	తిరిగి ((EXT2_SB(sb)->s_feature_ro_compat & \
		 EXT##ver##_FEATURE_RO_COMPAT_##flagname) != 0); \
} \
స్థిర అంతఃవరుస శూన్య ext2fs_సమితి_feature##name(నిర్మాణం ext2_super_ఖండం *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat |= \
		EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
} \
స్థిర అంతఃవరుస శూన్య ext2fs_clear_feature_##name(నిర్మాణం ext2_super_ఖండం *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat &= \
		~EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
}

#నిర్వచించు EXT4_FEATURE_INCOMPAT_FUNCS(name, ver, flagname) \
స్థిర అంతఃవరుస పూర్ణాంకం ext2fs_has_feature_##name(నిర్మాణం ext2_super_ఖండం *sb) \
{ \
	తిరిగి ((EXT2_SB(sb)->s_feature_incompat & \
		 EXT##ver##_FEATURE_INCOMPAT_##flagname) != 0); \
} \
స్థిర అంతఃవరుస శూన్య ext2fs_సమితి_feature##name(నిర్మాణం ext2_super_ఖండం *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat |= \
		EXT##ver##_FEATURE_INCOMPAT_##flagname; \
} \
స్థిర అంతఃవరుస శూన్య ext2fs_clear_feature_##name(నిర్మాణం ext2_super_ఖండం *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat &= \
		~EXT##ver##_FEATURE_INCOMPAT_##flagname; \
}

EXT4_FEATURE_COMPAT_FUNCS(dir_prealloc,		2, DIR_PREALLOC)
EXT4_FEATURE_COMPAT_FUNCS(imagic_inodes,	2, IMAGIC_INODES)
EXT4_FEATURE_COMPAT_FUNCS(journal,		3, HAS_JOURNAL)
EXT4_FEATURE_COMPAT_FUNCS(xattr,		2, EXT_ATTR)
EXT4_FEATURE_COMPAT_FUNCS(resize_inode,		2, RESIZE_INODE)
EXT4_FEATURE_COMPAT_FUNCS(dir_సూచీ,		2, DIR_సూచీ)
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
EXT4_FEATURE_RO_COMPAT_FUNCS(shared_ఖండాలు,	4, SHARED_ఖండాలు)
EXT4_FEATURE_RO_COMPAT_FUNCS(verity,		4, VERITY)
EXT4_FEATURE_RO_COMPAT_FUNCS(orphan_present,	4, ORPHAN_PRESENT)

EXT4_FEATURE_INCOMPAT_FUNCS(compression,	2, COMPRESSION)
EXT4_FEATURE_INCOMPAT_FUNCS(filetype,		2, FILETYPE)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_needs_recovery,	3, RECOVER)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_dev,	3, JOURNAL_DEV)
EXT4_FEATURE_INCOMPAT_FUNCS(meta_bg,		2, META_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(extents,		3, EXTENTS)
EXT4_FEATURE_INCOMPAT_FUNCS(64బిట్టు,		4, 64బిట్టు_2)
EXT4_FEATURE_INCOMPAT_FUNCS(mmp,		4, MMP)
EXT4_FEATURE_INCOMPAT_FUNCS(flex_bg,		4, FLEX_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(ea_inode,		4, EA_INODE)
EXT4_FEATURE_INCOMPAT_FUNCS(dirdata,		4, DIRDATA)
EXT4_FEATURE_INCOMPAT_FUNCS(csum_విత్తనం,		4, CSUM_విత్తనం)
EXT4_FEATURE_INCOMPAT_FUNCS(largedir,		4, LARGEDIR)
EXT4_FEATURE_INCOMPAT_FUNCS(inline_దత్తాంశం,	4, INLINE_దత్తాంశం)
EXT4_FEATURE_INCOMPAT_FUNCS(encrypt,		4, ENCRYPT)
EXT4_FEATURE_INCOMPAT_FUNCS(casefold,		4, CASEFOLD)

#నిర్వచించు EXT2_FEATURE_COMPAT_SUPP	0
#నిర్వచించు EXT2_FEATURE_INCOMPAT_SUPP    (EXT2_FEATURE_INCOMPAT_FILETYPE| \
				       EXT4_FEATURE_INCOMPAT_MMP| \
				       EXT4_FEATURE_INCOMPAT_LARGEDIR| \
				       EXT4_FEATURE_INCOMPAT_EA_INODE)
#నిర్వచించు EXT2_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
					 EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \
					 EXT4_FEATURE_RO_COMPAT_DIR_NLINK| \
					 EXT2_FEATURE_RO_COMPAT_BTREE_DIR| \
					 EXT4_FEATURE_RO_COMPAT_VERITY)

/*
 * Default values for user and/or group using reserved blocks
 */
#నిర్వచించు EXT2_DEF_RESUID		0
#నిర్వచించు EXT2_DEF_RESGID		0

/*
 * Default mount options
 */
#నిర్వచించు EXT2_DEFM_DEBUG		0x0001
#నిర్వచించు EXT2_DEFM_BSDGROUPS	0x0002
#నిర్వచించు EXT2_DEFM_XATTR_USER	0x0004
#నిర్వచించు EXT2_DEFM_ACL		0x0008
#నిర్వచించు EXT2_DEFM_UID16		0x0010
#నిర్వచించు EXT3_DEFM_JMODE		0x0060
#నిర్వచించు EXT3_DEFM_JMODE_దత్తాంశం	0x0020
#నిర్వచించు EXT3_DEFM_JMODE_ORDERED	0x0040
#నిర్వచించు EXT3_DEFM_JMODE_WBACK	0x0060
#నిర్వచించు EXT4_DEFM_NOBARRIER	0x0100
#నిర్వచించు EXT4_DEFM_ఖండం_VALIDITY 0x0200
#నిర్వచించు EXT4_DEFM_DISCARD	0x0400
#నిర్వచించు EXT4_DEFM_NODELALLOC	0x0800

స్థిర అంతఃవరుస పూర్ణాంకం ext4_hash_లోపల_dirent(స్థిరరాశి నిర్మాణం ext2_inode *inode)
{
	తిరిగి (inode->i_flags & EXT4_ENCRYPT_FL) &&
		(inode->i_flags & EXT4_CASEFOLD_FL);
}

/*
 * Structure of a directory entry
 */
#నిర్వచించు EXT2_NAME_LEN 255

నిర్మాణం ext2_dir_నమోదు {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u16	name_len;		/* Name length */
	అక్షరం	name[EXT2_NAME_LEN];	/* File name */
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
నిర్మాణం ext2_dir_నమోదు_2 {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u8	name_len;		/* Name length */
	__u8	file_type;
	అక్షరం	name[EXT2_NAME_LEN];	/* File name */
};

/*
 * Hashes for ext4_dir_entry for casefolded and ecrypted directories.
 * This is located at the first 4 bit aligned location after the name.
 */

నిర్మాణం ext2_dir_నమోదు_hash {
	__le32 hash;
	__le32 minor_hash;
};

#నిర్వచించు EXT2_DIRENT_HASHES(నమోదు_2) \
	((నిర్మాణం ext2_dir_నమోదు_hash *) &నమోదు_2->name[\
		(ext2fs_dirent_name_len(నమోదు_2) + \
			EXT2_DIR_విడత) & ~EXT2_DIR_విడత])
#నిర్వచించు EXT2_DIRENT_HASH(నమోదు_2) \
		ext2fs_le32_గమ్యం_cpu(EXT2_DIRENT_HASHES(నమోదు_2)->hash)
#నిర్వచించు EXT2_DIRENT_MINOR_HASH(నమోదు_2) \
		ext2fs_le32_గమ్యం_cpu(EXT2_DIRENT_HASHES(నమోదు_2)->minor_hash)

/*
 * This is a bogus directory entry at the end of each leaf block that
 * records checksums.
 */
నిర్మాణం ext2_dir_నమోదు_తోక {
	__u32	det_reserved_zero1;	/* Pretend to be unused */
	__u16	det_rec_len;		/* 12 */
	__u16	det_reserved_name_len;	/* 0xDE00, fake namelen/filetype */
	__u32	det_checksum;		/* crc32c(uuid+inode+dirent) */
};

/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
#నిర్వచించు EXT2_FT_UNKNOWN		0
#నిర్వచించు EXT2_FT_REG_FILE	1
#నిర్వచించు EXT2_FT_DIR		2
#నిర్వచించు EXT2_FT_CHRDEV		3
#నిర్వచించు EXT2_FT_BLKDEV		4
#నిర్వచించు EXT2_FT_FIFO		5
#నిర్వచించు EXT2_FT_SOCK		6
#నిర్వచించు EXT2_FT_SYMLINK		7

#నిర్వచించు EXT2_FT_MAX		8

/*
 * Annoyingly, e2fsprogs always swab16s ext2_dir_entry.name_len, so we
 * have to build ext2_dir_entry_tail with that assumption too.  This
 * constant helps to build the dir_entry_tail to look like it has an
 * "invalid" file type.
 */
#నిర్వచించు EXT2_DIR_NAME_LEN_CSUM	0xDE00

/*
 * EXT2_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
#నిర్వచించు EXT2_DIR_నమోదు_శీర్షిక_LEN	8
#నిర్వచించు EXT2_DIR_నమోదు_HASH_LEN		8
#నిర్వచించు EXT2_DIR_PAD			4
#నిర్వచించు EXT2_DIR_విడత			(EXT2_DIR_PAD - 1)
#నిర్వచించు EXT2_DIR_REC_LEN(name_len) ext2fs_dir_rec_len(name_len, 0)

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం ext2fs_dir_rec_len(__u8 name_len,
						పూర్ణాంకం extended)
{
	పూర్ణాంకం rec_len = (name_len + EXT2_DIR_నమోదు_శీర్షిక_LEN + EXT2_DIR_విడత);

	rec_len &= ~EXT2_DIR_విడత;
	ఒకవేళ (extended)
		rec_len += EXT2_DIR_నమోదు_HASH_LEN;
	తిరిగి rec_len;
}

#నిర్వచించు EXT4_ORPHAN_ఖండం_MAGIC 0x0b10ca04

/* Structure at the tail of orphan block */
నిర్మాణం ext4_orphan_ఖండం_తోక {
	__le32 ob_magic;
	__le32 ob_checksum;
};

/*
 * Constants for ext4's extended time encoding
 */
#నిర్వచించు EXT4_EPOCH_బిట్లు 2
#నిర్వచించు EXT4_EPOCH_ముసుగు ((1 << EXT4_EPOCH_బిట్లు) - 1)
#నిర్వచించు EXT4_NSEC_ముసుగు  (~0UL << EXT4_EPOCH_బిట్లు)

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
#నిర్వచించు EXT4_MMP_MAGIC     0x004D4D50U /* ASCII for MMP */
#నిర్వచించు EXT4_MMP_SEQ_CLEAN 0xFF4D4D50U /* mmp_seq value for clean unmount */
#నిర్వచించు EXT4_MMP_SEQ_FSCK  0xE24D4D50U /* mmp_seq value when being fscked */
#నిర్వచించు EXT4_MMP_SEQ_MAX   0xE24D4D4FU /* maximum valid mmp_seq value */

/* Not endian-annotated; it's swapped at read/write time */
నిర్మాణం mmp_struct {
	__u32	mmp_magic;		/* Magic number for MMP */
	__u32	mmp_seq;		/* Sequence no. updated periodically */
	__u64	mmp_సమయం;		/* Time last updated (seconds) */
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
#నిర్వచించు EXT4_MMP_UPDATE_INTERVAL	5

/*
 * Maximum interval for MMP update in seconds.
 */
#నిర్వచించు EXT4_MMP_MAX_UPDATE_INTERVAL	300

/*
 * Minimum interval for MMP checking in seconds.
 */
#నిర్వచించు EXT4_MMP_MIN_CHECK_INTERVAL     5

/*
 * Minimum size of inline data.
 */
#నిర్వచించు EXT4_MIN_INLINE_దత్తాంశం_కొలత	((పరిమాణం(__u32) * EXT2_N_ఖండాలు))

/*
 * Size of a parent inode in inline data directory.
 */
#నిర్వచించు EXT4_INLINE_దత్తాంశం_DOTDOT_కొలత	(4)

#నిర్వచించు EXT4_ENC_UTF8_12_1	1

#నిర్వచించు EXT4_ENC_STRICT_బహుళకం_FL			(1 << 0) /* Reject invalid sequences */

#అయితే_ముగింపు	/* _LINUX_EXT2_FS_H */
