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

#यदि_अपरिभाषितम् _LINUX_EXT2_FS_H
#परिभाषय _LINUX_EXT2_FS_H

#अन्तर्भाव <ext2fs/ext2_types.h>		/* Changed from linux/types.h */

#यदि_अपरिभाषितम् __GNUC_PREREQ
#यदि defined(__GNUC__) && defined(__GNUC_MINOR__)
#परिभाषय __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#अन्यथा
#परिभाषय __GNUC_PREREQ(maj, min) 0
#यदि_अन्तः
#यदि_अन्तः

#यदि_अपरिभाषितम् __nonstring
#यदि_परिभाषितम् __has_attribute
#यदि __has_attribute(__nonstring__)
#परिभाषय __nonstring                    __attribute__((__nonstring__))
#अन्यथा
#परिभाषय __nonstring
#यदि_अन्तः /* __has_attribute(__nonstring__) */
#अन्यथा
# define __nonstring
#यदि_अन्तः /* __has_attribute */
#यदि_अन्तः /* __nonstring */

/*
 * The second extended filesystem constants/structures
 */

/*
 * Define EXT2FS_DEBUG to produce debug messages
 */
#अपरिभाषय EXT2FS_DEBUG

/*
 * Define EXT2_PREALLOCATE to preallocate data blocks for expanding files
 */
#परिभाषय EXT2_PREALLOCATE
#परिभाषय EXT2_DEFAULT_PREALLOC_खण्डानि	8

/*
 * The second extended file system version
 */
#परिभाषय EXT2FS_DATE		"95/08/09"
#परिभाषय EXT2_FS_संस्करणम्		"0.5b"

/*
 * Special inode numbers
 */
#परिभाषय EXT2_BAD_INO		 1	/* Bad blocks inode */
#परिभाषय EXT2_मूलम्_INO		 2	/* Root inode */
#परिभाषय EXT4_USR_QUOTA_INO	 3	/* User quota inode */
#परिभाषय EXT4_GRP_QUOTA_INO	 4	/* Group quota inode */
#परिभाषय EXT2_BOOT_LOADER_INO	 5	/* Boot loader inode */
#परिभाषय EXT2_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#परिभाषय EXT2_RESIZE_INO		 7	/* Reserved group descriptors inode */
#परिभाषय EXT2_JOURNAL_INO	 8	/* Journal inode */
#परिभाषय EXT2_EXCLUDE_INO	 9	/* The "exclude" inode, for snapshots */
#परिभाषय EXT4_REPLICA_INO	10	/* Used by non-upstream feature */

/* First non-reserved inode for old ext2 filesystems */
#परिभाषय EXT2_GOOD_OLD_FIRST_INO	11

/*
 * The second extended file system magic number
 */
#परिभाषय EXT2_SUPER_MAGIC	0xEF53

#यदि_परिभाषितम् __KERNEL__
#परिभाषय EXT2_SB(sb)	(&((sb)->u.ext2_sb))
#अन्यथा
/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#परिभाषय EXT2_SB(sb)	(sb)
#यदि_अन्तः

/*
 * Maximal count of links to a file
 */
#परिभाषय EXT2_LINK_MAX		65000

/*
 * Macro-instructions used to manage several block sizes
 */
#परिभाषय EXT2_MIN_खण्डम्_LOG_आकार		10	/* 1024 */
#परिभाषय EXT2_MAX_खण्डम्_LOG_आकार		16	/* 65536 */
#परिभाषय EXT2_MIN_खण्डम्_आकार	(1 << EXT2_MIN_खण्डम्_LOG_आकार)
#परिभाषय EXT2_MAX_खण्डम्_आकार	(1 << EXT2_MAX_खण्डम्_LOG_आकार)
#यदि_परिभाषितम् __KERNEL__
#परिभाषय EXT2_खण्डम्_आकार(s)	((s)->s_blocksize)
#परिभाषय EXT2_खण्डम्_आकार_द्वयङ्काः(s)	((s)->s_blocksize_द्वयङ्काः)
#परिभाषय EXT2_ADDR_प्रत्येकम्_खण्डम्_द्वयङ्काः(s)	(EXT2_SB(s)->addr_प्रत्येकम्_खण्डम्_द्वयङ्काः)
#परिभाषय EXT2_INODE_आकार(s)	(EXT2_SB(s)->s_inode_आकार)
#परिभाषय EXT2_FIRST_INO(s)	(EXT2_SB(s)->s_first_ino)
#अन्यथा
#परिभाषय EXT2_खण्डम्_आकार(s)	(EXT2_MIN_खण्डम्_आकार << (s)->s_log_खण्डम्_आकार)
#परिभाषय EXT2_खण्डम्_आकार_द्वयङ्काः(s)	((s)->s_log_खण्डम्_आकार + 10)
#परिभाषय EXT2_INODE_आकार(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_INODE_आकार : (s)->s_inode_आकार)
#परिभाषय EXT2_FIRST_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_FIRST_INO : (s)->s_first_ino)
#यदि_अन्तः
#परिभाषय EXT2_ADDR_प्रत्येकम्_खण्डम्(s)	(EXT2_खण्डम्_आकार(s) / परिमाण(__u32))

/*
 * Macro-instructions used to manage allocation clusters
 */
#परिभाषय EXT2_MIN_CLUSTER_LOG_आकार	EXT2_MIN_खण्डम्_LOG_आकार
#परिभाषय EXT2_MAX_CLUSTER_LOG_आकार	29	/* 512MB  */
#परिभाषय EXT2_MIN_CLUSTER_आकार		EXT2_MIN_खण्डम्_आकार
#परिभाषय EXT2_MAX_CLUSTER_आकार		(1 << EXT2_MAX_CLUSTER_LOG_आकार)
#परिभाषय EXT2_CLUSTER_आकार(s)		(EXT2_MIN_खण्डम्_आकार << \
						(s)->s_log_cluster_आकार)
#परिभाषय EXT2_CLUSTER_आकार_द्वयङ्काः(s)	((s)->s_log_cluster_आकार + 10)

/*
 * Macro-instructions used to manage fragments
 *
 * Note: for backwards compatibility only, for the dump program.
 * Ext2/3/4 will never support fragments....
 */
#परिभाषय EXT2_MIN_FRAG_आकार              EXT2_MIN_खण्डम्_आकार
#परिभाषय EXT2_MAX_FRAG_आकार              EXT2_MAX_खण्डम्_आकार
#परिभाषय EXT2_MIN_FRAG_LOG_आकार          EXT2_MIN_खण्डम्_LOG_आकार
#परिभाषय EXT2_FRAG_आकार(s)		EXT2_खण्डम्_आकार(s)
#परिभाषय EXT2_FRAGS_प्रत्येकम्_खण्डम्(s)		1

/*
 * ACL structures
 */
संरचना ext2_acl_शीर्षकम्	/* Header of Access Control Lists */
{
	__u32	aclh_आकार;
	__u32	aclh_file_गणनम्;
	__u32	aclh_acle_गणनम्;
	__u32	aclh_first_acle;
};

संरचना ext2_acl_प्रविष्टिः	/* Access Control List Entry */
{
	__u32	acle_आकार;
	__u16	acle_perms;	/* Access permissions */
	__u16	acle_type;	/* Type of entry */
	__u16	acle_चिह्नकम्;	/* User or group identity */
	__u16	acle_pad1;
	__u32	acle_अग्र;	/* Pointer on next entry for the */
					/* same inode or on next free entry */
};

/*
 * Structure of a blocks group descriptor
 */
संरचना ext2_गणः_desc
{
	__u32	bg_खण्डम्_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_सारणी;		/* Inodes table block */
	__u16	bg_free_खण्डानि_गणनम्;	/* Free blocks count */
	__u16	bg_free_inodes_गणनम्;	/* Free inodes count */
	__u16	bg_प्रयुक्तम्_dirs_गणनम्;	/* Directories count */
	__u16	bg_flags;
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_खण्डम्_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_अप्रयुक्तम्;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(s_uuid+group_num+group_desc)*/
};

/*
 * Structure of a blocks group descriptor
 */
संरचना ext4_गणः_desc
{
	__u32	bg_खण्डम्_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_सारणी;		/* Inodes table block */
	__u16	bg_free_खण्डानि_गणनम्;	/* Free blocks count */
	__u16	bg_free_inodes_गणनम्;	/* Free inodes count */
	__u16	bg_प्रयुक्तम्_dirs_गणनम्;	/* Directories count */
	__u16	bg_flags;		/* EXT4_BG_flags (INODE_UNINIT, etc) */
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_खण्डम्_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_अप्रयुक्तम्;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(sb_uuid+group+desc) */
	__u32	bg_खण्डम्_bitmap_hi;	/* Blocks bitmap block MSB */
	__u32	bg_inode_bitmap_hi;	/* Inodes bitmap block MSB */
	__u32	bg_inode_सारणी_hi;	/* Inodes table block MSB */
	__u16	bg_free_खण्डानि_गणनम्_hi;/* Free blocks count MSB */
	__u16	bg_free_inodes_गणनम्_hi;/* Free inodes count MSB */
	__u16	bg_प्रयुक्तम्_dirs_गणनम्_hi;	/* Directories count MSB */
	__u16	bg_itable_अप्रयुक्तम्_hi;	/* Unused inodes count MSB */
	__u32	bg_exclude_bitmap_hi;	/* Exclude bitmap block MSB */
	__u16	bg_खण्डम्_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u16	bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u32	bg_reserved;
};

#परिभाषय EXT4_BG_INODE_BITMAP_CSUM_HI_अन्तः	\
	(offsetof(संरचना ext4_गणः_desc, bg_inode_bitmap_csum_hi) + \
	 परिमाण(__u16))
#परिभाषय EXT4_BG_खण्डम्_BITMAP_CSUM_HI_LOCATION	\
	(offsetof(संरचना ext4_गणः_desc, bg_खण्डम्_bitmap_csum_hi) + \
	 परिमाण(__u16))

#परिभाषय EXT2_BG_INODE_UNINIT	0x0001 /* Inode table/bitmap not initialized */
#परिभाषय EXT2_BG_खण्डम्_UNINIT	0x0002 /* Block bitmap not initialized */
#परिभाषय EXT2_BG_INODE_ZEROED	0x0004 /* On-disk itable initialized to zero */

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
संरचना ext2_dx_मूलम्_info {
	__u32 reserved_zero;
	__u8 hash_संस्करणम्; /* 0 now, 1 at release */
	__u8 info_दीर्घता; /* 8 */
	__u8 परोक्षम्_स्तराः;
	__u8 अप्रयुक्तम्_flags;
};

#परिभाषय EXT2_HASH_LEGACY		0
#परिभाषय EXT2_HASH_HALF_MD4		1
#परिभाषय EXT2_HASH_TEA			2
#परिभाषय EXT2_HASH_LEGACY_UNSIGNED	3 /* reserved for userspace lib */
#परिभाषय EXT2_HASH_HALF_MD4_UNSIGNED	4 /* reserved for userspace lib */
#परिभाषय EXT2_HASH_TEA_UNSIGNED		5 /* reserved for userspace lib */
#परिभाषय EXT2_HASH_SIPHASH		6

#परिभाषय EXT2_HASH_FLAG_INCOMPAT	0x1

#परिभाषय EXT4_DX_खण्डम्_आवरणम् 0x0fffffff

संरचना ext2_dx_प्रविष्टिः {
	__le32 hash;
	__le32 खण्डम्;
};

संरचना ext2_dx_countlimit {
	__le16 सीमा;
	__le16 गणनम्;
};

/*
 * This goes at the end of each htree block.
 */
संरचना ext2_dx_पुच्छ {
	__le32 dt_reserved;
	__le32 dt_checksum;	/* crc32c(uuid+inum+dxblock) */
};

/*
 * Macro-instructions used to manage group descriptors
 */
#परिभाषय EXT2_MIN_DESC_आकार             32
#परिभाषय EXT2_MIN_DESC_आकार_64_BIT       64
#परिभाषय EXT2_MAX_DESC_आकार             EXT2_MIN_खण्डम्_आकार
#परिभाषय EXT2_DESC_आकार(s)                                                \
	(ext2fs_has_feature_64bit(s) ? (s)->s_desc_आकार : EXT2_MIN_DESC_आकार)

#परिभाषय EXT2_खण्डानि_प्रत्येकम्_गणः(s)	(EXT2_SB(s)->s_खण्डानि_प्रत्येकम्_गणः)
#परिभाषय EXT2_INODES_प्रत्येकम्_गणः(s)	(EXT2_SB(s)->s_inodes_प्रत्येकम्_गणः)
#परिभाषय EXT2_CLUSTERS_प्रत्येकम्_गणः(s)	(EXT2_SB(s)->s_clusters_प्रत्येकम्_गणः)
#परिभाषय EXT2_INODES_प्रत्येकम्_खण्डम्(s)	(EXT2_खण्डम्_आकार(s)/EXT2_INODE_आकार(s))
/* limits imposed by 16-bit value gd_free_{blocks,inode}_count */
#परिभाषय EXT2_MAX_खण्डानि_प्रत्येकम्_गणः(s)	((((अचिह्नित) 1 << 16) - 8) *	\
					 (EXT2_CLUSTER_आकार(s) / \
					  EXT2_खण्डम्_आकार(s)))
#परिभाषय EXT2_MAX_CLUSTERS_प्रत्येकम्_गणः(s)	(((अचिह्नित) 1 << 16) - 8)
#परिभाषय EXT2_MAX_INODES_प्रत्येकम्_गणः(s)	(((अचिह्नित) 1 << 16) - \
					 EXT2_INODES_प्रत्येकम्_खण्डम्(s))
#यदि_परिभाषितम् __KERNEL__
#परिभाषय EXT2_DESC_प्रत्येकम्_खण्डम्(s)		(EXT2_SB(s)->s_desc_प्रत्येकम्_खण्डम्)
#परिभाषय EXT2_DESC_प्रत्येकम्_खण्डम्_द्वयङ्काः(s)	(EXT2_SB(s)->s_desc_प्रत्येकम्_खण्डम्_द्वयङ्काः)
#अन्यथा
#परिभाषय EXT2_DESC_प्रत्येकम्_खण्डम्(s)		(EXT2_खण्डम्_आकार(s) / EXT2_DESC_आकार(s))
#यदि_अन्तः

#परिभाषय EXT2_GROUPS_गन्तव्यम्_खण्डानि(s, g)   ((blk64_t) EXT2_खण्डानि_प्रत्येकम्_गणः(s) * \
				       (g))
#परिभाषय EXT2_GROUPS_गन्तव्यम्_CLUSTERS(s, g) ((blk64_t) EXT2_CLUSTERS_प्रत्येकम्_गणः(s) * \
				       (g))

/*
 * Constants relative to the data blocks
 */
#परिभाषय EXT2_NDIR_खण्डानि		12
#परिभाषय EXT2_IND_खण्डम्			EXT2_NDIR_खण्डानि
#परिभाषय EXT2_DIND_खण्डम्			(EXT2_IND_खण्डम् + 1)
#परिभाषय EXT2_TIND_खण्डम्			(EXT2_DIND_खण्डम् + 1)
#परिभाषय EXT2_N_खण्डानि			(EXT2_TIND_खण्डम् + 1)

/*
 * Inode flags
 */
#परिभाषय EXT2_SECRM_FL			0x00000001 /* Secure deletion */
#परिभाषय EXT2_UNRM_FL			0x00000002 /* Undelete */
#परिभाषय EXT2_COMPR_FL			0x00000004 /* Compress file */
#परिभाषय EXT2_SYNC_FL			0x00000008 /* Synchronous updates */
#परिभाषय EXT2_IMMUTABLE_FL		0x00000010 /* Immutable file */
#परिभाषय EXT2_APPEND_FL			0x00000020 /* writes to file may only append */
#परिभाषय EXT2_NODUMP_FL			0x00000040 /* do not dump file */
#परिभाषय EXT2_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#परिभाषय EXT2_DIRTY_FL			0x00000100
#परिभाषय EXT2_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#परिभाषय EXT2_NOCOMPR_FL			0x00000400 /* Access raw compressed data */
	/* nb: was previously EXT2_ECOMPR_FL */
#परिभाषय EXT4_ENCRYPT_FL			0x00000800 /* encrypted inode */
/* End compression flags --- maybe not all used */
#परिभाषय EXT2_BTREE_FL			0x00001000 /* btree format dir */
#परिभाषय EXT2_सूचकांक_FL			0x00001000 /* hash-indexed directory */
#परिभाषय EXT2_IMAGIC_FL			0x00002000
#परिभाषय EXT3_JOURNAL_दत्त_FL		0x00004000 /* file data should be journaled */
#परिभाषय EXT2_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#परिभाषय EXT2_DIRSYNC_FL 		0x00010000 /* Synchronous directory modifications */
#परिभाषय EXT2_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#परिभाषय EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#परिभाषय EXT4_EXTENTS_FL 		0x00080000 /* Inode uses extents */
#परिभाषय EXT4_VERITY_FL			0x00100000 /* Verity protected inode */
#परिभाषय EXT4_EA_INODE_FL	        0x00200000 /* Inode used for large EA */
/* EXT4_EOFBLOCKS_FL 0x00400000 was here */
#परिभाषय FS_NOCOW_FL			0x00800000 /* Do not cow file */
#परिभाषय EXT4_SNAPFILE_FL		0x01000000  /* Inode is a snapshot */
#परिभाषय FS_DAX_FL			0x02000000 /* Inode is DAX */
#परिभाषय EXT4_SNAPFILE_DELETED_FL	0x04000000  /* Snapshot is being deleted */
#परिभाषय EXT4_SNAPFILE_SHRUNK_FL		0x08000000  /* Snapshot shrink has completed */
#परिभाषय EXT4_INLINE_दत्त_FL		0x10000000 /* Inode has inline data */
#परिभाषय EXT4_PROJINHERIT_FL		0x20000000 /* Create with parents projid */
#परिभाषय EXT4_CASEFOLD_FL		0x40000000 /* Casefolded file */
#परिभाषय EXT2_RESERVED_FL		0x80000000 /* reserved for ext2 lib */

#परिभाषय EXT2_FL_USER_VISIBLE		0x604BDFFF /* User visible flags */
#परिभाषय EXT2_FL_USER_MODIFIABLE		0x604B80FF /* User modifiable flags */

/*
 * ioctl commands
 */

/* Used for online resize */
संरचना ext2_new_गणः_निवेशः {
	__u32 गणः;		/* Group number for this data */
	__u32 खण्डम्_bitmap;	/* Absolute block number of block bitmap */
	__u32 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u32 inode_सारणी;	/* Absolute block number of inode table start */
	__u32 खण्डानि_गणनम्;	/* Total number of blocks in this group */
	__u16 reserved_खण्डानि;	/* Number of reserved blocks in this group */
	__u16 अप्रयुक्तम्;		/* Number of reserved GDT blocks in group */
};

संरचना ext4_new_गणः_निवेशः {
	__u32 गणः;		/* Group number for this data */
	__u64 खण्डम्_bitmap;	/* Absolute block number of block bitmap */
	__u64 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u64 inode_सारणी;	/* Absolute block number of inode table start */
	__u32 खण्डानि_गणनम्;	/* Total number of blocks in this group */
	__u16 reserved_खण्डानि;	/* Number of reserved blocks in this group */
	__u16 अप्रयुक्तम्;
};

#यदि_परिभाषितम् __GNU__			/* Needed for the Hurd */
#परिभाषय IOT_ext2_new_गणः_निवेशः _IOT (_IOTS(__u32), 5, _IOTS(__u16), 2, 0, 0)
#यदि_अन्तः

#परिभाषय EXT2_IOC_GETFLAGS		_IOR('f', 1, दीर्घ)
#परिभाषय EXT2_IOC_SETFLAGS		_IOW('f', 2, दीर्घ)
#परिभाषय EXT2_IOC_GETVERSION		_IOR('v', 1, दीर्घ)
#परिभाषय EXT2_IOC_SETVERSION		_IOW('v', 2, दीर्घ)
#परिभाषय EXT2_IOC_GETVERSION_NEW		_IOR('f', 3, दीर्घ)
#परिभाषय EXT2_IOC_SETVERSION_NEW		_IOW('f', 4, दीर्घ)
#परिभाषय EXT2_IOC_गणः_EXTEND		_IOW('f', 7, अचिह्नित दीर्घ)
#परिभाषय EXT2_IOC_गणः_सङ्कलय		_IOW('f', 8,संरचना ext2_new_गणः_निवेशः)
#परिभाषय EXT4_IOC_गणः_सङ्कलय		_IOW('f', 8,संरचना ext4_new_गणः_निवेशः)
#परिभाषय EXT4_IOC_RESIZE_FS		_IOW('f', 16, __u64)

/*
 * Structure of an inode on the disk
 */
संरचना ext2_inode {
/*00*/	__u16	i_बहुलकम्;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_आकार;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_गणनम्;	/* Links count */
	__u32	i_खण्डानि;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	सङ्घ {
		संरचना {
			__u32	l_i_संस्करणम्; /* was l_i_reserved1 */
		} linux1;
		संरचना {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_खण्डम्[EXT2_N_खण्डानि];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_आकार_उच्चः;
/*70*/	__u32	i_faddr;	/* Fragment address */
	सङ्घ {
		संरचना {
			__u16	l_i_खण्डानि_hi;
			__u16	l_i_file_acl_उच्चः;
			__u16	l_i_uid_उच्चः;	/* these 2 fields    */
			__u16	l_i_gid_उच्चः;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		संरचना {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_बहुलकम्_उच्चः;
			__u16	h_i_uid_उच्चः;
			__u16	h_i_gid_उच्चः;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
};

/*
 * Permanent part of an large inode on the disk
 */
संरचना ext2_inode_large {
/*00*/	__u16	i_बहुलकम्;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_आकार;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode Change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_गणनम्;	/* Links count */
	__u32	i_खण्डानि;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	सङ्घ {
		संरचना {
			__u32	l_i_संस्करणम्; /* was l_i_reserved1 */
		} linux1;
		संरचना {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_खण्डम्[EXT2_N_खण्डानि];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_आकार_उच्चः;
/*70*/	__u32	i_faddr;	/* Fragment address */
	सङ्घ {
		संरचना {
			__u16	l_i_खण्डानि_hi;
			__u16	l_i_file_acl_उच्चः;
			__u16	l_i_uid_उच्चः;	/* these 2 fields    */
			__u16	l_i_gid_उच्चः;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		संरचना {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_बहुलकम्_उच्चः;
			__u16	h_i_uid_उच्चः;
			__u16	h_i_gid_उच्चः;
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
	__u32	i_संस्करणम्_hi;	/* high 32 bits for 64-bit version */
/*9c*/	__u32   i_projid;       /* Project ID */
};

#परिभाषय EXT4_INODE_CSUM_HI_EXTRA_अन्तः	\
	(offsetof(संरचना ext2_inode_large, i_checksum_hi) + परिमाण(__u16) - \
	 EXT2_GOOD_OLD_INODE_आकार)

#परिभाषय EXT4_EPOCH_द्वयङ्काः 2
#परिभाषय EXT4_EPOCH_आवरणम् ((1 << EXT4_EPOCH_द्वयङ्काः) - 1)

#परिभाषय i_checksum_lo	osd2.linux2.l_i_checksum_lo

#परिभाषय ext2fs_inode_includes(आकार, क्षेत्रम्)				\
	((आकार) >= (परिमाण(((संरचना ext2_inode_large *)0)->क्षेत्रम्) +	\
		    offsetof(संरचना ext2_inode_large, क्षेत्रम्)))

#यदि defined(__KERNEL__) || defined(__linux__)
#परिभाषय i_reserved1	osd1.linux1.l_i_reserved1
#परिभाषय i_frag		osd2.linux2.l_i_frag
#परिभाषय i_fsize		osd2.linux2.l_i_fsize
#परिभाषय i_uid_नीचः	i_uid
#परिभाषय i_gid_नीचः	i_gid
#परिभाषय i_uid_उच्चः	osd2.linux2.l_i_uid_उच्चः
#परिभाषय i_gid_उच्चः	osd2.linux2.l_i_gid_उच्चः
#अन्यथा
#यदि defined(__GNU__)

#परिभाषय i_translator	osd1.hurd1.h_i_translator
#परिभाषय i_frag		osd2.hurd2.h_i_frag;
#परिभाषय i_fsize		osd2.hurd2.h_i_fsize;
#परिभाषय i_uid_उच्चः	osd2.hurd2.h_i_uid_उच्चः
#परिभाषय i_gid_उच्चः	osd2.hurd2.h_i_gid_उच्चः
#परिभाषय i_author	osd2.hurd2.h_i_author

#यदि_अन्तः  /* __GNU__ */
#यदि_अन्तः	/* defined(__KERNEL__) || defined(__linux__) */

#परिभाषय inode_uid(inode)	((inode).i_uid | (अचिह्नित)(inode).osd2.linux2.l_i_uid_उच्चः << 16)
#परिभाषय inode_gid(inode)	((inode).i_gid | (अचिह्नित)(inode).osd2.linux2.l_i_gid_उच्चः << 16)
#परिभाषय inode_projid(inode)	((inode).i_projid)
#परिभाषय ext2fs_समुच्चयः_i_uid_उच्चः(inode,x) ((inode).osd2.linux2.l_i_uid_उच्चः = (x))
#परिभाषय ext2fs_समुच्चयः_i_gid_उच्चः(inode,x) ((inode).osd2.linux2.l_i_gid_उच्चः = (x))

स्थिर अन्तःस्थ
संरचना ext2_inode *EXT2_INODE(संरचना ext2_inode_large *large_inode)
{
	प्रत्यागम (संरचना ext2_inode *) large_inode;
}

/*
 * File system states
 */
#परिभाषय EXT2_VALID_FS			0x0001	/* Unmounted cleanly */
#परिभाषय EXT2_ERROR_FS			0x0002	/* Errors detected */
#परिभाषय EXT3_ORPHAN_FS			0x0004	/* Orphans being recovered */
#परिभाषय EXT4_FC_REPLAY			0x0020	/* Ext4 fast commit replay ongoing */

/*
 * Misc. filesystem flags
 */
#परिभाषय EXT2_FLAGS_SIGNED_HASH		0x0001  /* Signed dirhash in use */
#परिभाषय EXT2_FLAGS_UNSIGNED_HASH	0x0002  /* Unsigned dirhash in use */
#परिभाषय EXT2_FLAGS_TEST_FILESYS		0x0004	/* OK for use on development code */
#परिभाषय EXT2_FLAGS_IS_SNAPSHOT		0x0010	/* This is a snapshot image */
#परिभाषय EXT2_FLAGS_FIX_SNAPSHOT		0x0020	/* Snapshot inodes corrupted */
#परिभाषय EXT2_FLAGS_FIX_EXCLUDE		0x0040	/* Exclude bitmaps corrupted */

/*
 * Mount flags
 */
#परिभाषय EXT2_MOUNT_CHECK		0x0001	/* Do mount-time checks */
#परिभाषय EXT2_MOUNT_GRPID		0x0004	/* Create files with directory's group */
#परिभाषय EXT2_MOUNT_DEBUG		0x0008	/* Some debugging messages */
#परिभाषय EXT2_MOUNT_ERRORS_CONT		0x0010	/* Continue on errors */
#परिभाषय EXT2_MOUNT_ERRORS_RO		0x0020	/* Remount fs ro on errors */
#परिभाषय EXT2_MOUNT_ERRORS_PANIC		0x0040	/* Panic on errors */
#परिभाषय EXT2_MOUNT_MINIX_DF		0x0080	/* Mimics the Minix statfs */
#परिभाषय EXT2_MOUNT_NO_UID32		0x0200  /* Disable 32-bit UIDs */

#परिभाषय clear_opt(o, opt)		o &= ~EXT2_MOUNT_##opt
#परिभाषय समुच्चयः_opt(o, opt)			o |= EXT2_MOUNT_##opt
#परिभाषय test_opt(sb, opt)		(EXT2_SB(sb)->s_mount_opt & \
					 EXT2_MOUNT_##opt)
/*
 * Maximal mount counts between two filesystem checks
 */
#परिभाषय EXT2_DFL_MAX_MNT_गणनम्		20	/* Allow 20 mounts */
#परिभाषय EXT2_DFL_CHECKINTERVAL		0	/* Don't use interval check */

/*
 * Behaviour when detecting errors
 */
#परिभाषय EXT2_ERRORS_CONTINUE		1	/* Continue execution */
#परिभाषय EXT2_ERRORS_RO			2	/* Remount fs read-only */
#परिभाषय EXT2_ERRORS_PANIC		3	/* Panic */
#परिभाषय EXT2_ERRORS_DEFAULT		EXT2_ERRORS_CONTINUE

#यदि (__GNUC__ >= 4)
#परिभाषय ext4_offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#अन्यथा
#परिभाषय ext4_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#यदि_अन्तः

/* Metadata checksum algorithms */
#परिभाषय EXT2_चक्रीयपरीक्षा_32_C_CHKSUM		1

/* Encryption algorithms, key size and key reference len */
#परिभाषय EXT4_ENCRYPTION_बहुलकम्_INVALID		0
#परिभाषय EXT4_ENCRYPTION_बहुलकम्_AES_256_XTS	1
#परिभाषय EXT4_ENCRYPTION_बहुलकम्_AES_256_GCM	2
#परिभाषय EXT4_ENCRYPTION_बहुलकम्_AES_256_CBC	3
#परिभाषय EXT4_ENCRYPTION_बहुलकम्_AES_256_CTS	4

#परिभाषय EXT4_AES_256_XTS_कुञ्चिका_आकार		64
#परिभाषय EXT4_AES_256_GCM_कुञ्चिका_आकार		32
#परिभाषय EXT4_AES_256_CBC_कुञ्चिका_आकार		32
#परिभाषय EXT4_AES_256_CTS_कुञ्चिका_आकार		32
#परिभाषय EXT4_MAX_कुञ्चिका_आकार			64

#परिभाषय EXT4_कुञ्चिका_DESCRIPTOR_आकार		8
#परिभाषय EXT4_CRYPTO_खण्डम्_आकार			16

/* Password derivation constants */
#परिभाषय EXT4_MAX_PASSPHRASE_आकार		1024
#परिभाषय EXT4_MAX_SALT_आकार			256
#परिभाषय EXT4_PBKDF2_ITERATIONS			0xFFFF

#परिभाषय EXT2_LABEL_LEN				16

/*
 * Policy provided via an ioctl on the topmost directory. This
 * structure is also in the kernel.
 */
संरचना ext4_encryption_policy {
  वर्ण संस्करणम्;
  वर्ण contents_encryption_बहुलकम्;
  वर्ण filenames_encryption_बहुलकम्;
  वर्ण flags;
  वर्ण master_कुञ्चिका_descriptor[EXT4_कुञ्चिका_DESCRIPTOR_आकार];
} __attribute__((__packed__));

संरचना ext4_encryption_कुञ्चिका {
        __u32 बहुलकम्;
        वर्ण raw[EXT4_MAX_कुञ्चिका_आकार];
        __u32 आकार;
} __attribute__((__packed__));

/*
 * Structure of the super block
 */
संरचना ext2_super_खण्डम् {
/*000*/	__u32	s_inodes_गणनम्;		/* Inodes count */
	__u32	s_खण्डानि_गणनम्;		/* Blocks count */
	__u32	s_r_खण्डानि_गणनम्;	/* Reserved blocks count */
	__u32	s_free_खण्डानि_गणनम्;	/* Free blocks count */
/*010*/	__u32	s_free_inodes_गणनम्;	/* Free inodes count */
	__u32	s_first_दत्त_खण्डम्;	/* First Data Block */
	__u32	s_log_खण्डम्_आकार;	/* Block size */
	__u32	s_log_cluster_आकार;	/* Allocation cluster size */
/*020*/	__u32	s_खण्डानि_प्रत्येकम्_गणः;	/* # Blocks per group */
	__u32	s_clusters_प्रत्येकम्_गणः;	/* # Fragments per group */
	__u32	s_inodes_प्रत्येकम्_गणः;	/* # Inodes per group */
	__u32	s_mtime;		/* Mount time */
/*030*/	__u32	s_wtime;		/* Write time */
	__u16	s_mnt_गणनम्;		/* Mount count */
	__s16	s_max_mnt_गणनम्;	/* Maximal mount count */
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
	__u32	s_first_ino;		/* First non-reserved inode */
	__u16   s_inode_आकार;		/* size of inode structure */
	__u16	s_खण्डम्_गणः_nr;	/* block group # of this superblock */
	__u32	s_feature_compat;	/* compatible feature set */
/*060*/	__u32	s_feature_incompat;	/* incompatible feature set */
	__u32	s_feature_ro_compat;	/* readonly-compatible feature set */
/*068*/	__u8	s_uuid[16] __nonstring;		/* 128-bit uuid for volume */
/*078*/	__u8	s_volume_name[EXT2_LABEL_LEN] __nonstring;	/* volume name, no NUL? */
/*088*/	__u8	s_चरमः_mounted[64] __nonstring;	/* directory last mounted on, no NUL? */
/*0c8*/	__u32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_खण्डानि;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_खण्डानि;	/* Nr to preallocate for dirs */
	__u16	s_reserved_gdt_खण्डानि;	/* Per group table for online growth */
	/*
	 * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
/*0d0*/	__u8	s_journal_uuid[16] __nonstring;	/* uuid of journal superblock */
/*0e0*/	__u32	s_journal_inum;		/* inode number of journal file */
	__u32	s_journal_dev;		/* device number of journal file */
	__u32	s_चरमः_orphan;		/* start of list of inodes to delete */
/*0ec*/	__u32	s_hash_बीजम्[4];		/* HTREE hash seed */
/*0fc*/	__u8	s_def_hash_संस्करणम्;	/* Default hash version to use */
	__u8	s_jnl_backup_type;	/* Default type of journal backup */
	__u16	s_desc_आकार;		/* Group desc. size: INCOMPAT_64BIT */
/*100*/	__u32	s_default_mount_opts;	/* default EXT2_MOUNT_* flags used */
	__u32	s_first_meta_bg;	/* First metablock group */
	__u32	s_mkfs_time;		/* When the filesystem was created */
/*10c*/	__u32	s_jnl_खण्डानि[17];	/* Backup of the journal inode */
/*150*/	__u32	s_खण्डानि_गणनम्_hi;	/* Blocks count high 32bits */
	__u32	s_r_खण्डानि_गणनम्_hi;	/* Reserved blocks count high 32 bits*/
	__u32	s_free_खण्डानि_hi;	/* Free blocks count */
	__u16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__u16	s_want_extra_isize;	/* New inodes should reserve # bytes */
/*160*/	__u32	s_flags;		/* Miscellaneous flags */
	__u16	s_raid_stride;		/* RAID stride in blocks */
	__u16	s_mmp_update_interval;  /* # seconds to wait in MMP checking */
	__u64	s_mmp_खण्डम्;		/* Block for multi-mount protection */
/*170*/	__u32	s_raid_stripe_विस्तारः;	/* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_प्रत्येकम्_flex;	/* FLEX_BG group size */
	__u8	s_checksum_type;	/* metadata checksum algorithm */
	__u8	s_encryption_level;	/* versioning level for encryption */
	__u8	s_reserved_pad;		/* Padding to next 32bits */
	__u64	s_kbytes_written;	/* nr of lifetime kilobytes written */
/*180*/	__u32	s_snapshot_inum;	/* Inode number of active snapshot */
	__u32	s_snapshot_id;		/* sequential ID of active snapshot */
	__u64	s_snapshot_r_खण्डानि_गणनम्; /* active snapshot reserved blocks */
/*190*/	__u32	s_snapshot_list;	/* inode number of disk snapshot list */
#परिभाषय EXT4_S_ERR_START ext4_offsetof(संरचना ext2_super_खण्डम्, s_error_गणनम्)
	__u32	s_error_गणनम्;		/* number of fs errors */
	__u32	s_first_error_time;	/* first time an error happened */
	__u32	s_first_error_ino;	/* inode involved in first error */
/*1a0*/	__u64	s_first_error_खण्डम्;	/* block involved in first error */
	__u8	s_first_error_func[32] __nonstring;	/* function where error hit, no NUL? */
/*1c8*/	__u32	s_first_error_पङ्क्तिः;	/* line number where error happened */
	__u32	s_चरमः_error_time;	/* most recent time of an error */
/*1d0*/	__u32	s_चरमः_error_ino;	/* inode involved in last error */
	__u32	s_चरमः_error_पङ्क्तिः;	/* line number where error happened */
	__u64	s_चरमः_error_खण्डम्;	/* block involved of last error */
/*1e0*/	__u8	s_चरमः_error_func[32] __nonstring;	/* function where error hit, no NUL? */
#परिभाषय EXT4_S_ERR_अन्तः ext4_offsetof(संरचना ext2_super_खण्डम्, s_mount_opts)
/*200*/	__u8	s_mount_opts[64] __nonstring;	/* default mount options, no NUL? */
/*240*/	__u32	s_usr_quota_inum;	/* inode number of user quota file */
	__u32	s_grp_quota_inum;	/* inode number of group quota file */
	__u32	s_overhead_clusters;	/* overhead blocks/clusters in fs */
/*24c*/	__u32	s_backup_bgs[2];	/* If sparse_super2 enabled */
/*254*/	__u8	s_encrypt_algos[4];	/* Encryption algorithms in use  */
/*258*/	__u8	s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
/*268*/	__le32	s_lpf_ino;		/* Location of the lost+found inode */
	__le32  s_prj_quota_inum;	/* inode for tracking project quota */
/*270*/	__le32	s_checksum_बीजम्;	/* crc32c(orig_uuid) if csum_seed set */
/*274*/	__u8	s_wtime_hi;
	__u8	s_mtime_hi;
	__u8	s_mkfs_time_hi;
	__u8	s_lastcheck_hi;
	__u8	s_first_error_time_hi;
	__u8	s_चरमः_error_time_hi;
	__u8	s_first_error_errcode;
	__u8    s_चरमः_error_errcode;
/*27c*/ __le16	s_encoding;		/* Filename charset encoding */
	__le16	s_encoding_flags;	/* Filename charset encoding flags */
	__le32  s_orphan_file_inum;	/* Inode for tracking orphan inodes */
	__le32	s_reserved[94];		/* Padding to the end of the block */
/*3fc*/	__u32	s_checksum;		/* crc32c(superblock) */
};

#परिभाषय EXT4_S_ERR_LEN (EXT4_S_ERR_अन्तः - EXT4_S_ERR_START)
#परिभाषय EXT2_LEN_STR(buf) (पूर्णाङ्क)परिमाण(buf), (वर्ण *)buf

/*
 * Codes for operating systems
 */
#परिभाषय EXT2_OS_LINUX		0
#परिभाषय EXT2_OS_HURD		1
#परिभाषय EXT2_OBSO_OS_MASIX	2
#परिभाषय EXT2_OS_FREEBSD		3
#परिभाषय EXT2_OS_LITES		4

/*
 * Revision levels
 */
#परिभाषय EXT2_GOOD_OLD_REV	0	/* The good old (original) format */
#परिभाषय EXT2_DYNAMIC_REV	1	/* V2 format w/ dynamic inode sizes */

#परिभाषय EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV
#परिभाषय EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV

#परिभाषय EXT2_GOOD_OLD_INODE_आकार 128

#परिभाषय EXT4_EXTRA_TIMESTAMP_MAX	(((int64_t)1 << 34) - 1  + INT32_MIN)
#परिभाषय EXT4_NON_EXTRA_TIMESTAMP_MAX	INT32_MAX
#परिभाषय EXT4_TIMESTAMP_MIN		INT32_MIN

/*
 * Journal inode backup types
 */
#परिभाषय EXT3_JNL_BACKUP_खण्डानि	1

/*
 * Feature set definitions
 */

#परिभाषय EXT2_HAS_COMPAT_FEATURE(sb,आवरणम्)			\
	( EXT2_SB(sb)->s_feature_compat & (आवरणम्) )
#परिभाषय EXT2_HAS_RO_COMPAT_FEATURE(sb,आवरणम्)			\
	( EXT2_SB(sb)->s_feature_ro_compat & (आवरणम्) )
#परिभाषय EXT2_HAS_INCOMPAT_FEATURE(sb,आवरणम्)			\
	( EXT2_SB(sb)->s_feature_incompat & (आवरणम्) )

#परिभाषय EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001
#परिभाषय EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002
#परिभाषय EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
#परिभाषय EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008
#परिभाषय EXT2_FEATURE_COMPAT_RESIZE_INODE	0x0010
#परिभाषय EXT2_FEATURE_COMPAT_DIR_सूचकांक		0x0020
#परिभाषय EXT2_FEATURE_COMPAT_LAZY_BG		0x0040
/* #define EXT2_FEATURE_COMPAT_EXCLUDE_INODE	0x0080 not used, legacy */
#परिभाषय EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP	0x0100
#परिभाषय EXT4_FEATURE_COMPAT_SPARSE_SUPER2	0x0200
#परिभाषय EXT4_FEATURE_COMPAT_FAST_COMMIT		0x0400
#परिभाषय EXT4_FEATURE_COMPAT_STABLE_INODES	0x0800
#परिभाषय EXT4_FEATURE_COMPAT_ORPHAN_FILE		0x1000

#परिभाषय EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#परिभाषय EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004 not used */
#परिभाषय EXT4_FEATURE_RO_COMPAT_HUGE_FILE	0x0008
#परिभाषय EXT4_FEATURE_RO_COMPAT_GDT_CSUM		0x0010
#परिभाषय EXT4_FEATURE_RO_COMPAT_DIR_NLINK	0x0020
#परिभाषय EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE	0x0040
#परिभाषय EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT	0x0080
#परिभाषय EXT4_FEATURE_RO_COMPAT_QUOTA		0x0100
#परिभाषय EXT4_FEATURE_RO_COMPAT_BIGALLOC		0x0200
/*
 * METADATA_CSUM implies GDT_CSUM.  When METADATA_CSUM is set, group
 * descriptor checksums use the same algorithm as all other data
 * structures' checksums.
 */
#परिभाषय EXT4_FEATURE_RO_COMPAT_METADATA_CSUM	0x0400
#परिभाषय EXT4_FEATURE_RO_COMPAT_REPLICA		0x0800
#परिभाषय EXT4_FEATURE_RO_COMPAT_READONLY		0x1000
#परिभाषय EXT4_FEATURE_RO_COMPAT_PROJECT		0x2000 /* Project quota */
#परिभाषय EXT4_FEATURE_RO_COMPAT_SHARED_खण्डानि	0x4000
#परिभाषय EXT4_FEATURE_RO_COMPAT_VERITY		0x8000
#परिभाषय EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT	0x10000

#परिभाषय EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001
#परिभाषय EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002
#परिभाषय EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 /* Needs recovery */
#परिभाषय EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 /* Journal device */
#परिभाषय EXT2_FEATURE_INCOMPAT_META_BG		0x0010
#परिभाषय EXT3_FEATURE_INCOMPAT_EXTENTS		0x0040
#परिभाषय EXT4_FEATURE_INCOMPAT_64BIT		0x0080
#परिभाषय EXT4_FEATURE_INCOMPAT_MMP		0x0100
#परिभाषय EXT4_FEATURE_INCOMPAT_FLEX_BG		0x0200
#परिभाषय EXT4_FEATURE_INCOMPAT_EA_INODE		0x0400
#परिभाषय EXT4_FEATURE_INCOMPAT_DIRDATA		0x1000
#परिभाषय EXT4_FEATURE_INCOMPAT_CSUM_बीजम्		0x2000
#परिभाषय EXT4_FEATURE_INCOMPAT_LARGEDIR		0x4000 /* >2GB or 3-lvl htree */
#परिभाषय EXT4_FEATURE_INCOMPAT_INLINE_दत्त	0x8000 /* data in inode */
#परिभाषय EXT4_FEATURE_INCOMPAT_ENCRYPT		0x10000
#परिभाषय EXT4_FEATURE_INCOMPAT_CASEFOLD		0x20000

#परिभाषय EXT4_FEATURE_COMPAT_FUNCS(name, ver, flagname) \
स्थिर अन्तःस्थ पूर्णाङ्क ext2fs_has_feature_##name(संरचना ext2_super_खण्डम् *sb) \
{ \
	प्रत्यागम ((EXT2_SB(sb)->s_feature_compat & \
		 EXT##ver##_FEATURE_COMPAT_##flagname) != 0); \
} \
स्थिर अन्तःस्थ शून्य ext2fs_समुच्चयः_feature##name(संरचना ext2_super_खण्डम् *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat |= \
		EXT##ver##_FEATURE_COMPAT_##flagname; \
} \
स्थिर अन्तःस्थ शून्य ext2fs_clear_feature_##name(संरचना ext2_super_खण्डम् *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat &= \
		~EXT##ver##_FEATURE_COMPAT_##flagname; \
}

#परिभाषय EXT4_FEATURE_RO_COMPAT_FUNCS(name, ver, flagname) \
स्थिर अन्तःस्थ पूर्णाङ्क ext2fs_has_feature_##name(संरचना ext2_super_खण्डम् *sb) \
{ \
	प्रत्यागम ((EXT2_SB(sb)->s_feature_ro_compat & \
		 EXT##ver##_FEATURE_RO_COMPAT_##flagname) != 0); \
} \
स्थिर अन्तःस्थ शून्य ext2fs_समुच्चयः_feature##name(संरचना ext2_super_खण्डम् *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat |= \
		EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
} \
स्थिर अन्तःस्थ शून्य ext2fs_clear_feature_##name(संरचना ext2_super_खण्डम् *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat &= \
		~EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
}

#परिभाषय EXT4_FEATURE_INCOMPAT_FUNCS(name, ver, flagname) \
स्थिर अन्तःस्थ पूर्णाङ्क ext2fs_has_feature_##name(संरचना ext2_super_खण्डम् *sb) \
{ \
	प्रत्यागम ((EXT2_SB(sb)->s_feature_incompat & \
		 EXT##ver##_FEATURE_INCOMPAT_##flagname) != 0); \
} \
स्थिर अन्तःस्थ शून्य ext2fs_समुच्चयः_feature##name(संरचना ext2_super_खण्डम् *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat |= \
		EXT##ver##_FEATURE_INCOMPAT_##flagname; \
} \
स्थिर अन्तःस्थ शून्य ext2fs_clear_feature_##name(संरचना ext2_super_खण्डम् *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat &= \
		~EXT##ver##_FEATURE_INCOMPAT_##flagname; \
}

EXT4_FEATURE_COMPAT_FUNCS(dir_prealloc,		2, DIR_PREALLOC)
EXT4_FEATURE_COMPAT_FUNCS(imagic_inodes,	2, IMAGIC_INODES)
EXT4_FEATURE_COMPAT_FUNCS(journal,		3, HAS_JOURNAL)
EXT4_FEATURE_COMPAT_FUNCS(xattr,		2, EXT_ATTR)
EXT4_FEATURE_COMPAT_FUNCS(resize_inode,		2, RESIZE_INODE)
EXT4_FEATURE_COMPAT_FUNCS(dir_सूचकांक,		2, DIR_सूचकांक)
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
EXT4_FEATURE_RO_COMPAT_FUNCS(shared_खण्डानि,	4, SHARED_खण्डानि)
EXT4_FEATURE_RO_COMPAT_FUNCS(verity,		4, VERITY)
EXT4_FEATURE_RO_COMPAT_FUNCS(orphan_present,	4, ORPHAN_PRESENT)

EXT4_FEATURE_INCOMPAT_FUNCS(compression,	2, COMPRESSION)
EXT4_FEATURE_INCOMPAT_FUNCS(filetype,		2, FILETYPE)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_needs_recovery,	3, RECOVER)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_dev,	3, JOURNAL_DEV)
EXT4_FEATURE_INCOMPAT_FUNCS(meta_bg,		2, META_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(extents,		3, EXTENTS)
EXT4_FEATURE_INCOMPAT_FUNCS(64bit,		4, 64BIT)
EXT4_FEATURE_INCOMPAT_FUNCS(mmp,		4, MMP)
EXT4_FEATURE_INCOMPAT_FUNCS(flex_bg,		4, FLEX_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(ea_inode,		4, EA_INODE)
EXT4_FEATURE_INCOMPAT_FUNCS(dirdata,		4, DIRDATA)
EXT4_FEATURE_INCOMPAT_FUNCS(csum_बीजम्,		4, CSUM_बीजम्)
EXT4_FEATURE_INCOMPAT_FUNCS(largedir,		4, LARGEDIR)
EXT4_FEATURE_INCOMPAT_FUNCS(inline_दत्त,	4, INLINE_दत्त)
EXT4_FEATURE_INCOMPAT_FUNCS(encrypt,		4, ENCRYPT)
EXT4_FEATURE_INCOMPAT_FUNCS(casefold,		4, CASEFOLD)

#परिभाषय EXT2_FEATURE_COMPAT_SUPP	0
#परिभाषय EXT2_FEATURE_INCOMPAT_SUPP    (EXT2_FEATURE_INCOMPAT_FILETYPE| \
				       EXT4_FEATURE_INCOMPAT_MMP| \
				       EXT4_FEATURE_INCOMPAT_LARGEDIR| \
				       EXT4_FEATURE_INCOMPAT_EA_INODE)
#परिभाषय EXT2_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
					 EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \
					 EXT4_FEATURE_RO_COMPAT_DIR_NLINK| \
					 EXT2_FEATURE_RO_COMPAT_BTREE_DIR| \
					 EXT4_FEATURE_RO_COMPAT_VERITY)

/*
 * Default values for user and/or group using reserved blocks
 */
#परिभाषय EXT2_DEF_RESUID		0
#परिभाषय EXT2_DEF_RESGID		0

/*
 * Default mount options
 */
#परिभाषय EXT2_DEFM_DEBUG		0x0001
#परिभाषय EXT2_DEFM_BSDGROUPS	0x0002
#परिभाषय EXT2_DEFM_XATTR_USER	0x0004
#परिभाषय EXT2_DEFM_ACL		0x0008
#परिभाषय EXT2_DEFM_UID16		0x0010
#परिभाषय EXT3_DEFM_JMODE		0x0060
#परिभाषय EXT3_DEFM_JMODE_दत्त	0x0020
#परिभाषय EXT3_DEFM_JMODE_ORDERED	0x0040
#परिभाषय EXT3_DEFM_JMODE_WBACK	0x0060
#परिभाषय EXT4_DEFM_NOBARRIER	0x0100
#परिभाषय EXT4_DEFM_खण्डम्_VALIDITY 0x0200
#परिभाषय EXT4_DEFM_DISCARD	0x0400
#परिभाषय EXT4_DEFM_NODELALLOC	0x0800

स्थिर अन्तःस्थ पूर्णाङ्क ext4_hash_अन्तर्_dirent(ध्रुव संरचना ext2_inode *inode)
{
	प्रत्यागम (inode->i_flags & EXT4_ENCRYPT_FL) &&
		(inode->i_flags & EXT4_CASEFOLD_FL);
}

/*
 * Structure of a directory entry
 */
#परिभाषय EXT2_NAME_LEN 255

संरचना ext2_dir_प्रविष्टिः {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u16	name_len;		/* Name length */
	वर्ण	name[EXT2_NAME_LEN];	/* File name */
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
संरचना ext2_dir_प्रविष्टिः_2 {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u8	name_len;		/* Name length */
	__u8	file_type;
	वर्ण	name[EXT2_NAME_LEN];	/* File name */
};

/*
 * Hashes for ext4_dir_entry for casefolded and ecrypted directories.
 * This is located at the first 4 bit aligned location after the name.
 */

संरचना ext2_dir_प्रविष्टिः_hash {
	__le32 hash;
	__le32 minor_hash;
};

#परिभाषय EXT2_DIRENT_HASHES(प्रविष्टिः) \
	((संरचना ext2_dir_प्रविष्टिः_hash *) &प्रविष्टिः->name[\
		(ext2fs_dirent_name_len(प्रविष्टिः) + \
			EXT2_DIR_आवर्तनम्) & ~EXT2_DIR_आवर्तनम्])
#परिभाषय EXT2_DIRENT_HASH(प्रविष्टिः) \
		ext2fs_le32_गन्तव्यम्_cpu(EXT2_DIRENT_HASHES(प्रविष्टिः)->hash)
#परिभाषय EXT2_DIRENT_MINOR_HASH(प्रविष्टिः) \
		ext2fs_le32_गन्तव्यम्_cpu(EXT2_DIRENT_HASHES(प्रविष्टिः)->minor_hash)

/*
 * This is a bogus directory entry at the end of each leaf block that
 * records checksums.
 */
संरचना ext2_dir_प्रविष्टिः_पुच्छ {
	__u32	det_reserved_zero1;	/* Pretend to be unused */
	__u16	det_rec_len;		/* 12 */
	__u16	det_reserved_name_len;	/* 0xDE00, fake namelen/filetype */
	__u32	det_checksum;		/* crc32c(uuid+inode+dirent) */
};

/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
#परिभाषय EXT2_FT_UNKNOWN		0
#परिभाषय EXT2_FT_REG_FILE	1
#परिभाषय EXT2_FT_DIR		2
#परिभाषय EXT2_FT_CHRDEV		3
#परिभाषय EXT2_FT_BLKDEV		4
#परिभाषय EXT2_FT_FIFO		5
#परिभाषय EXT2_FT_SOCK		6
#परिभाषय EXT2_FT_SYMLINK		7

#परिभाषय EXT2_FT_MAX		8

/*
 * Annoyingly, e2fsprogs always swab16s ext2_dir_entry.name_len, so we
 * have to build ext2_dir_entry_tail with that assumption too.  This
 * constant helps to build the dir_entry_tail to look like it has an
 * "invalid" file type.
 */
#परिभाषय EXT2_DIR_NAME_LEN_CSUM	0xDE00

/*
 * EXT2_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
#परिभाषय EXT2_DIR_प्रविष्टिः_शीर्षकम्_LEN	8
#परिभाषय EXT2_DIR_प्रविष्टिः_HASH_LEN		8
#परिभाषय EXT2_DIR_PAD			4
#परिभाषय EXT2_DIR_आवर्तनम्			(EXT2_DIR_PAD - 1)
#परिभाषय EXT2_DIR_REC_LEN(name_len) ext2fs_dir_rec_len(name_len, 0)

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क ext2fs_dir_rec_len(__u8 name_len,
						पूर्णाङ्क extended)
{
	पूर्णाङ्क rec_len = (name_len + EXT2_DIR_प्रविष्टिः_शीर्षकम्_LEN + EXT2_DIR_आवर्तनम्);

	rec_len &= ~EXT2_DIR_आवर्तनम्;
	यदि (extended)
		rec_len += EXT2_DIR_प्रविष्टिः_HASH_LEN;
	प्रत्यागम rec_len;
}

#परिभाषय EXT4_ORPHAN_खण्डम्_MAGIC 0x0b10ca04

/* Structure at the tail of orphan block */
संरचना ext4_orphan_खण्डम्_पुच्छ {
	__le32 ob_magic;
	__le32 ob_checksum;
};

/*
 * Constants for ext4's extended time encoding
 */
#परिभाषय EXT4_EPOCH_द्वयङ्काः 2
#परिभाषय EXT4_EPOCH_आवरणम् ((1 << EXT4_EPOCH_द्वयङ्काः) - 1)
#परिभाषय EXT4_NSEC_आवरणम्  (~0UL << EXT4_EPOCH_द्वयङ्काः)

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
#परिभाषय EXT4_MMP_MAGIC     0x004D4D50U /* ASCII for MMP */
#परिभाषय EXT4_MMP_SEQ_CLEAN 0xFF4D4D50U /* mmp_seq value for clean unmount */
#परिभाषय EXT4_MMP_SEQ_FSCK  0xE24D4D50U /* mmp_seq value when being fscked */
#परिभाषय EXT4_MMP_SEQ_MAX   0xE24D4D4FU /* maximum valid mmp_seq value */

/* Not endian-annotated; it's swapped at read/write time */
संरचना mmp_struct {
	__u32	mmp_magic;		/* Magic number for MMP */
	__u32	mmp_seq;		/* Sequence no. updated periodically */
	__u64	mmp_time;		/* Time last updated (seconds) */
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
#परिभाषय EXT4_MMP_UPDATE_INTERVAL	5

/*
 * Maximum interval for MMP update in seconds.
 */
#परिभाषय EXT4_MMP_MAX_UPDATE_INTERVAL	300

/*
 * Minimum interval for MMP checking in seconds.
 */
#परिभाषय EXT4_MMP_MIN_CHECK_INTERVAL     5

/*
 * Minimum size of inline data.
 */
#परिभाषय EXT4_MIN_INLINE_दत्त_आकार	((परिमाण(__u32) * EXT2_N_खण्डानि))

/*
 * Size of a parent inode in inline data directory.
 */
#परिभाषय EXT4_INLINE_दत्त_DOTDOT_आकार	(4)

#परिभाषय EXT4_ENC_UTF8_12_1	1

#परिभाषय EXT4_ENC_STRICT_बहुलकम्_FL			(1 << 0) /* Reject invalid sequences */

#यदि_अन्तः	/* _LINUX_EXT2_FS_H */
