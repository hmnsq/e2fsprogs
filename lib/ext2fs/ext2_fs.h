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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _LINUX_EXT2_FS_H
#ವ್ಯಾಖ್ಯಾನಿಸು _LINUX_EXT2_FS_H

#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_types.h>		/* Changed from linux/types.h */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __GNUC_PREREQ
#ಆದರೆ defined(__GNUC__) && defined(__GNUC_MINOR__)
#ವ್ಯಾಖ್ಯಾನಿಸು __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು __GNUC_PREREQ(maj, min) 0
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __nonstring
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __has_attribute
#ಆದರೆ __has_attribute(__nonstring__)
#ವ್ಯಾಖ್ಯಾನಿಸು __nonstring                    __attribute__((__nonstring__))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು __nonstring
#ಆದರೆ_ಅಂತ್ಯ /* __has_attribute(__nonstring__) */
#ಇಲ್ಲದಿದ್ದರೆ
# define __nonstring
#ಆದರೆ_ಅಂತ್ಯ /* __has_attribute */
#ಆದರೆ_ಅಂತ್ಯ /* __nonstring */

/*
 * The second extended filesystem constants/structures
 */

/*
 * Define EXT2FS_DEBUG to produce debug messages
 */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು EXT2FS_DEBUG

/*
 * Define EXT2_PREALLOCATE to preallocate data blocks for expanding files
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_PREALLOCATE
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DEFAULT_PREALLOC_ಖಂಡಗಳು	8

/*
 * The second extended file system version
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_DATE		"95/08/09"
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FS_ಆವೃತ್ತಿ		"0.5b"

/*
 * Special inode numbers
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_BAD_INO		 1	/* Bad blocks inode */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಬೇರು_INO		 2	/* Root inode */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_USR_QUOTA_INO	 3	/* User quota inode */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_GRP_QUOTA_INO	 4	/* Group quota inode */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_BOOT_LOADER_INO	 5	/* Boot loader inode */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_RESIZE_INO		 7	/* Reserved group descriptors inode */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_JOURNAL_INO	 8	/* Journal inode */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_EXCLUDE_INO	 9	/* The "exclude" inode, for snapshots */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_REPLICA_INO	10	/* Used by non-upstream feature */

/* First non-reserved inode for old ext2 filesystems */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_GOOD_OLD_ಮೊದಲ_INO	11

/*
 * The second extended file system magic number
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SUPER_MAGIC	0xEF53

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __KERNEL__
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SB(sb)	(&((sb)->u.ext2_sb))
#ಇಲ್ಲದಿದ್ದರೆ
/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SB(sb)	(sb)
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Maximal count of links to a file
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_LINK_MAX		65000

/*
 * Macro-instructions used to manage several block sizes
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MIN_ಖಂಡ_LOG_ಗಾತ್ರ		10	/* 1024 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MAX_ಖಂಡ_LOG_ಗಾತ್ರ		16	/* 65536 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MIN_ಖಂಡ_ಗಾತ್ರ	(1 << EXT2_MIN_ಖಂಡ_LOG_ಗಾತ್ರ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MAX_ಖಂಡ_ಗಾತ್ರ	(1 << EXT2_MAX_ಖಂಡ_LOG_ಗಾತ್ರ)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __KERNEL__
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಖಂಡ_ಗಾತ್ರ(s)	((s)->s_blocksize)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಖಂಡ_ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು(s)	((s)->s_blocksize_ಬಿಟ್ಟುಗಳು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ADDR_ಪ್ರತಿ_ಖಂಡ_ಬಿಟ್ಟುಗಳು(s)	(EXT2_SB(s)->addr_ಪ್ರತಿ_ಖಂಡ_ಬಿಟ್ಟುಗಳು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_INODE_ಗಾತ್ರ(s)	(EXT2_SB(s)->s_inode_ಗಾತ್ರ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಮೊದಲ_INO(s)	(EXT2_SB(s)->s_ಮೊದಲ_ino)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಖಂಡ_ಗಾತ್ರ(s)	(EXT2_MIN_ಖಂಡ_ಗಾತ್ರ << (s)->s_log_ಖಂಡ_ಗಾತ್ರ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಖಂಡ_ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು(s)	((s)->s_log_ಖಂಡ_ಗಾತ್ರ + 10)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_INODE_ಗಾತ್ರ(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_INODE_ಗಾತ್ರ : (s)->s_inode_ಗಾತ್ರ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಮೊದಲ_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_ಮೊದಲ_INO : (s)->s_ಮೊದಲ_ino)
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ADDR_ಪ್ರತಿ_ಖಂಡ(s)	(EXT2_ಖಂಡ_ಗಾತ್ರ(s) / ಗಾತ್ರ(__u32))

/*
 * Macro-instructions used to manage allocation clusters
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MIN_CLUSTER_LOG_ಗಾತ್ರ	EXT2_MIN_ಖಂಡ_LOG_ಗಾತ್ರ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MAX_CLUSTER_LOG_ಗಾತ್ರ	29	/* 512MB  */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MIN_CLUSTER_ಗಾತ್ರ		EXT2_MIN_ಖಂಡ_ಗಾತ್ರ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MAX_CLUSTER_ಗಾತ್ರ		(1 << EXT2_MAX_CLUSTER_LOG_ಗಾತ್ರ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CLUSTER_ಗಾತ್ರ(s)		(EXT2_MIN_ಖಂಡ_ಗಾತ್ರ << \
						(s)->s_log_cluster_ಗಾತ್ರ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CLUSTER_ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು(s)	((s)->s_log_cluster_ಗಾತ್ರ + 10)

/*
 * Macro-instructions used to manage fragments
 *
 * Note: for backwards compatibility only, for the dump program.
 * Ext2/3/4 will never support fragments....
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MIN_FRAG_ಗಾತ್ರ              EXT2_MIN_ಖಂಡ_ಗಾತ್ರ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MAX_FRAG_ಗಾತ್ರ              EXT2_MAX_ಖಂಡ_ಗಾತ್ರ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MIN_FRAG_LOG_ಗಾತ್ರ          EXT2_MIN_ಖಂಡ_LOG_ಗಾತ್ರ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FRAG_ಗಾತ್ರ(s)		EXT2_ಖಂಡ_ಗಾತ್ರ(s)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FRAGS_ಪ್ರತಿ_ಖಂಡ(s)		1

/*
 * ACL structures
 */
ರಚನೆ ext2_acl_ಶೀರ್ಷಿಕೆ	/* Header of Access Control Lists */
{
	__u32	aclh_ಗಾತ್ರ;
	__u32	aclh_file_ಎಣಿಕೆ;
	__u32	aclh_acle_ಎಣಿಕೆ;
	__u32	aclh_ಮೊದಲ_acle;
};

ರಚನೆ ext2_acl_ನಮೂದು	/* Access Control List Entry */
{
	__u32	acle_ಗಾತ್ರ;
	__u16	acle_perms;	/* Access permissions */
	__u16	acle_type;	/* Type of entry */
	__u16	acle_ಟ್ಯಾಗ್;	/* User or group identity */
	__u16	acle_pad1;
	__u32	acle_ಮುಂದಿನ;	/* Pointer on next entry for the */
					/* same inode or on next free entry */
};

/*
 * Structure of a blocks group descriptor
 */
ರಚನೆ ext2_ಗುಂಪು_desc
{
	__u32	bg_ಖಂಡ_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_ಕೋಷ್ಟಕ;		/* Inodes table block */
	__u16	bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ;	/* Free blocks count */
	__u16	bg_free_inodes_ಎಣಿಕೆ;	/* Free inodes count */
	__u16	bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ;	/* Directories count */
	__u16	bg_flags;
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_ಖಂಡ_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_ಬಳಸದ;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(s_uuid+group_num+group_desc)*/
};

/*
 * Structure of a blocks group descriptor
 */
ರಚನೆ ext4_ಗುಂಪು_desc
{
	__u32	bg_ಖಂಡ_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_ಕೋಷ್ಟಕ;		/* Inodes table block */
	__u16	bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ;	/* Free blocks count */
	__u16	bg_free_inodes_ಎಣಿಕೆ;	/* Free inodes count */
	__u16	bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ;	/* Directories count */
	__u16	bg_flags;		/* EXT4_BG_flags (INODE_UNINIT, etc) */
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_ಖಂಡ_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_ಬಳಸದ;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(sb_uuid+group+desc) */
	__u32	bg_ಖಂಡ_bitmap_hi;	/* Blocks bitmap block MSB */
	__u32	bg_inode_bitmap_hi;	/* Inodes bitmap block MSB */
	__u32	bg_inode_ಕೋಷ್ಟಕ_hi;	/* Inodes table block MSB */
	__u16	bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_hi;/* Free blocks count MSB */
	__u16	bg_free_inodes_ಎಣಿಕೆ_hi;/* Free inodes count MSB */
	__u16	bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ_hi;	/* Directories count MSB */
	__u16	bg_itable_ಬಳಸದ_hi;	/* Unused inodes count MSB */
	__u32	bg_exclude_bitmap_hi;	/* Exclude bitmap block MSB */
	__u16	bg_ಖಂಡ_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u16	bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u32	bg_reserved;
};

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_BG_INODE_BITMAP_CSUM_HI_ಕೊನೆ	\
	(offsetof(ರಚನೆ ext4_ಗುಂಪು_desc, bg_inode_bitmap_csum_hi) + \
	 ಗಾತ್ರ(__u16))
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_BG_ಖಂಡ_BITMAP_CSUM_HI_LOCATION	\
	(offsetof(ರಚನೆ ext4_ಗುಂಪು_desc, bg_ಖಂಡ_bitmap_csum_hi) + \
	 ಗಾತ್ರ(__u16))

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_BG_INODE_UNINIT	0x0001 /* Inode table/bitmap not initialized */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_BG_ಖಂಡ_UNINIT	0x0002 /* Block bitmap not initialized */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_BG_INODE_ZEROED	0x0004 /* On-disk itable initialized to zero */

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
ರಚನೆ ext2_dx_ಬೇರು_info {
	__u32 reserved_zero;
	__u8 hash_ಆವೃತ್ತಿ; /* 0 now, 1 at release */
	__u8 info_ಉದ್ದಳತೆ; /* 8 */
	__u8 ಪರೋಕ್ಷ_ಮಟ್ಟಗಳು;
	__u8 ಬಳಸದ_flags;
};

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HASH_LEGACY		0
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HASH_HALF_MD4		1
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HASH_TEA			2
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HASH_LEGACY_UNSIGNED	3 /* reserved for userspace lib */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HASH_HALF_MD4_UNSIGNED	4 /* reserved for userspace lib */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HASH_TEA_UNSIGNED		5 /* reserved for userspace lib */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HASH_SIPHASH		6

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HASH_FLAG_INCOMPAT	0x1

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_DX_ಖಂಡ_ಮುಖವಾಡ 0x0fffffff

ರಚನೆ ext2_dx_ನಮೂದು {
	__le32 hash;
	__le32 ಖಂಡ;
};

ರಚನೆ ext2_dx_countlimit {
	__le16 ಎಲ್ಲೆ;
	__le16 ಎಣಿಕೆ_2;
};

/*
 * This goes at the end of each htree block.
 */
ರಚನೆ ext2_dx_ಬಾಲ {
	__le32 dt_reserved;
	__le32 dt_checksum;	/* crc32c(uuid+inum+dxblock) */
};

/*
 * Macro-instructions used to manage group descriptors
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MIN_DESC_ಗಾತ್ರ             32
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MIN_DESC_ಗಾತ್ರ_64_ಬಿಟ್       64
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MAX_DESC_ಗಾತ್ರ             EXT2_MIN_ಖಂಡ_ಗಾತ್ರ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DESC_ಗಾತ್ರ(s)                                                \
	(ext2fs_has_feature_64bit(s) ? (s)->s_desc_ಗಾತ್ರ : EXT2_MIN_DESC_ಗಾತ್ರ)

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(s)	(EXT2_SB(s)->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(s)	(EXT2_SB(s)->s_inodes_ಪ್ರತಿ_ಗುಂಪು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(s)	(EXT2_SB(s)->s_clusters_ಪ್ರತಿ_ಗುಂಪು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_INODES_ಪ್ರತಿ_ಖಂಡ(s)	(EXT2_ಖಂಡ_ಗಾತ್ರ(s)/EXT2_INODE_ಗಾತ್ರ(s))
/* limits imposed by 16-bit value gd_free_{blocks,inode}_count */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MAX_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(s)	((((ಚಿಹ್ನೆರಹಿತ) 1 << 16) - 8) *	\
					 (EXT2_CLUSTER_ಗಾತ್ರ(s) / \
					  EXT2_ಖಂಡ_ಗಾತ್ರ(s)))
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MAX_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(s)	(((ಚಿಹ್ನೆರಹಿತ) 1 << 16) - 8)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MAX_INODES_ಪ್ರತಿ_ಗುಂಪು(s)	(((ಚಿಹ್ನೆರಹಿತ) 1 << 16) - \
					 EXT2_INODES_ಪ್ರತಿ_ಖಂಡ(s))
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __KERNEL__
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(s)		(EXT2_SB(s)->s_desc_ಪ್ರತಿ_ಖಂಡ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DESC_ಪ್ರತಿ_ಖಂಡ_ಬಿಟ್ಟುಗಳು(s)	(EXT2_SB(s)->s_desc_ಪ್ರತಿ_ಖಂಡ_ಬಿಟ್ಟುಗಳು)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(s)		(EXT2_ಖಂಡ_ಗಾತ್ರ(s) / EXT2_DESC_ಗಾತ್ರ(s))
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_GROUPS_ಗಮ್ಯ_ಖಂಡಗಳು(s, g)   ((blk64_t) EXT2_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(s) * \
				       (g))
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_GROUPS_ಗಮ್ಯ_CLUSTERS(s, g) ((blk64_t) EXT2_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(s) * \
				       (g))

/*
 * Constants relative to the data blocks
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_NDIR_ಖಂಡಗಳು		12
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IND_ಖಂಡ			EXT2_NDIR_ಖಂಡಗಳು
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIND_ಖಂಡ			(EXT2_IND_ಖಂಡ + 1)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_TIND_ಖಂಡ			(EXT2_DIND_ಖಂಡ + 1)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_N_ಖಂಡಗಳು			(EXT2_TIND_ಖಂಡ + 1)

/*
 * Inode flags
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SECRM_FL			0x00000001 /* Secure deletion */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_UNRM_FL			0x00000002 /* Undelete */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_COMPR_FL			0x00000004 /* Compress file */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_SYNC_FL			0x00000008 /* Synchronous updates */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IMMUTABLE_FL		0x00000010 /* Immutable file */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_APPEND_FL			0x00000020 /* writes to file may only append */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_NODUMP_FL			0x00000040 /* do not dump file */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIRTY_FL			0x00000100
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_NOCOMPR_FL			0x00000400 /* Access raw compressed data */
	/* nb: was previously EXT2_ECOMPR_FL */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ENCRYPT_FL			0x00000800 /* encrypted inode */
/* End compression flags --- maybe not all used */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_BTREE_FL			0x00001000 /* btree format dir */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಸೂಚ್ಯಂಕ_FL			0x00001000 /* hash-indexed directory */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IMAGIC_FL			0x00002000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_JOURNAL_ದತ್ತಾಂಶ_FL		0x00004000 /* file data should be journaled */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIRSYNC_FL 		0x00010000 /* Synchronous directory modifications */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_EXTENTS_FL 		0x00080000 /* Inode uses extents */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_VERITY_FL			0x00100000 /* Verity protected inode */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_EA_INODE_FL	        0x00200000 /* Inode used for large EA */
/* EXT4_EOFBLOCKS_FL 0x00400000 was here */
#ವ್ಯಾಖ್ಯಾನಿಸು FS_NOCOW_FL			0x00800000 /* Do not cow file */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_SNAPFILE_FL		0x01000000  /* Inode is a snapshot */
#ವ್ಯಾಖ್ಯಾನಿಸು FS_DAX_FL			0x02000000 /* Inode is DAX */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_SNAPFILE_DELETED_FL	0x04000000  /* Snapshot is being deleted */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_SNAPFILE_SHRUNK_FL		0x08000000  /* Snapshot shrink has completed */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_INLINE_ದತ್ತಾಂಶ_FL		0x10000000 /* Inode has inline data */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_PROJINHERIT_FL		0x20000000 /* Create with parents projid */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_CASEFOLD_FL		0x40000000 /* Casefolded file */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_RESERVED_FL		0x80000000 /* reserved for ext2 lib */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FL_USER_VISIBLE		0x604BDFFF /* User visible flags */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FL_USER_MODIFIABLE		0x604B80FF /* User modifiable flags */

/*
 * ioctl commands
 */

/* Used for online resize */
ರಚನೆ ext2_new_ಗುಂಪು_ಒಳಹರಿವು {
	__u32 ಗುಂಪು;		/* Group number for this data */
	__u32 ಖಂಡ_bitmap;	/* Absolute block number of block bitmap */
	__u32 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u32 inode_ಕೋಷ್ಟಕ;	/* Absolute block number of inode table start */
	__u32 ಖಂಡಗಳು_ಎಣಿಕೆ;	/* Total number of blocks in this group */
	__u16 reserved_ಖಂಡಗಳು;	/* Number of reserved blocks in this group */
	__u16 ಬಳಸದ;		/* Number of reserved GDT blocks in group */
};

ರಚನೆ ext4_new_ಗುಂಪು_ಒಳಹರಿವು {
	__u32 ಗುಂಪು;		/* Group number for this data */
	__u64 ಖಂಡ_bitmap;	/* Absolute block number of block bitmap */
	__u64 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u64 inode_ಕೋಷ್ಟಕ;	/* Absolute block number of inode table start */
	__u32 ಖಂಡಗಳು_ಎಣಿಕೆ;	/* Total number of blocks in this group */
	__u16 reserved_ಖಂಡಗಳು;	/* Number of reserved blocks in this group */
	__u16 ಬಳಸದ;
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNU__			/* Needed for the Hurd */
#ವ್ಯಾಖ್ಯಾನಿಸು IOT_ext2_new_ಗುಂಪು_ಒಳಹರಿವು _IOT (_IOTS(__u32), 5, _IOTS(__u16), 2, 0, 0)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IOC_GETFLAGS		_IOR('f', 1, ದೀರ್ಘ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IOC_SETFLAGS		_IOW('f', 2, ದೀರ್ಘ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IOC_GETVERSION		_IOR('v', 1, ದೀರ್ಘ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IOC_SETVERSION		_IOW('v', 2, ದೀರ್ಘ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IOC_GETVERSION_NEW		_IOR('f', 3, ದೀರ್ಘ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IOC_SETVERSION_NEW		_IOW('f', 4, ದೀರ್ಘ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IOC_ಗುಂಪು_EXTEND		_IOW('f', 7, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_IOC_ಗುಂಪು_ಕೂಡಿಸು		_IOW('f', 8,ರಚನೆ ext2_new_ಗುಂಪು_ಒಳಹರಿವು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_IOC_ಗುಂಪು_ಕೂಡಿಸು		_IOW('f', 8,ರಚನೆ ext4_new_ಗುಂಪು_ಒಳಹರಿವು)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_IOC_RESIZE_FS		_IOW('f', 16, __u64)

/*
 * Structure of an inode on the disk
 */
ರಚನೆ ext2_inode {
/*00*/	__u16	i_ಬಹುಲಕ;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_ಗಾತ್ರ;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_ಎಣಿಕೆ;	/* Links count */
	__u32	i_ಖಂಡಗಳು;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	ಒಕ್ಕೂಟ {
		ರಚನೆ {
			__u32	l_i_ಆವೃತ್ತಿ; /* was l_i_reserved1 */
		} linux1;
		ರಚನೆ {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_ಖಂಡ[EXT2_N_ಖಂಡಗಳು];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_ಗಾತ್ರ_ಮೇಲಿನ;
/*70*/	__u32	i_faddr;	/* Fragment address */
	ಒಕ್ಕೂಟ {
		ರಚನೆ {
			__u16	l_i_ಖಂಡಗಳು_hi;
			__u16	l_i_file_acl_ಮೇಲಿನ;
			__u16	l_i_uid_ಮೇಲಿನ;	/* these 2 fields    */
			__u16	l_i_gid_ಮೇಲಿನ;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		ರಚನೆ {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_ಬಹುಲಕ_ಮೇಲಿನ;
			__u16	h_i_uid_ಮೇಲಿನ;
			__u16	h_i_gid_ಮೇಲಿನ;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
};

/*
 * Permanent part of an large inode on the disk
 */
ರಚನೆ ext2_inode_large {
/*00*/	__u16	i_ಬಹುಲಕ;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_ಗಾತ್ರ;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode Change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_ಎಣಿಕೆ;	/* Links count */
	__u32	i_ಖಂಡಗಳು;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	ಒಕ್ಕೂಟ {
		ರಚನೆ {
			__u32	l_i_ಆವೃತ್ತಿ; /* was l_i_reserved1 */
		} linux1;
		ರಚನೆ {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_ಖಂಡ[EXT2_N_ಖಂಡಗಳು];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_ಗಾತ್ರ_ಮೇಲಿನ;
/*70*/	__u32	i_faddr;	/* Fragment address */
	ಒಕ್ಕೂಟ {
		ರಚನೆ {
			__u16	l_i_ಖಂಡಗಳು_hi;
			__u16	l_i_file_acl_ಮೇಲಿನ;
			__u16	l_i_uid_ಮೇಲಿನ;	/* these 2 fields    */
			__u16	l_i_gid_ಮೇಲಿನ;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		ರಚನೆ {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_ಬಹುಲಕ_ಮೇಲಿನ;
			__u16	h_i_uid_ಮೇಲಿನ;
			__u16	h_i_gid_ಮೇಲಿನ;
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
	__u32	i_ಆವೃತ್ತಿ_hi;	/* high 32 bits for 64-bit version */
/*9c*/	__u32   i_projid;       /* Project ID */
};

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_INODE_CSUM_HI_EXTRA_ಕೊನೆ	\
	(offsetof(ರಚನೆ ext2_inode_large, i_checksum_hi) + ಗಾತ್ರ(__u16) - \
	 EXT2_GOOD_OLD_INODE_ಗಾತ್ರ)

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_EPOCH_ಬಿಟ್ಟುಗಳು 2
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_EPOCH_ಮುಖವಾಡ ((1 << EXT4_EPOCH_ಬಿಟ್ಟುಗಳು) - 1)

#ವ್ಯಾಖ್ಯಾನಿಸು i_checksum_lo	osd2.linux2.l_i_checksum_lo

#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_inode_includes(ಗಾತ್ರ_2, ಕ್ಷೇತ್ರ)				\
	((ಗಾತ್ರ_2) >= (ಗಾತ್ರ(((ರಚನೆ ext2_inode_large *)0)->ಕ್ಷೇತ್ರ) +	\
		    offsetof(ರಚನೆ ext2_inode_large, ಕ್ಷೇತ್ರ)))

#ಆದರೆ defined(__KERNEL__) || defined(__linux__)
#ವ್ಯಾಖ್ಯಾನಿಸು i_reserved1	osd1.linux1.l_i_reserved1
#ವ್ಯಾಖ್ಯಾನಿಸು i_frag		osd2.linux2.l_i_frag
#ವ್ಯಾಖ್ಯಾನಿಸು i_fsize		osd2.linux2.l_i_fsize
#ವ್ಯಾಖ್ಯಾನಿಸು i_uid_ಕೆಳಗಿನ	i_uid
#ವ್ಯಾಖ್ಯಾನಿಸು i_gid_ಕೆಳಗಿನ	i_gid
#ವ್ಯಾಖ್ಯಾನಿಸು i_uid_ಮೇಲಿನ	osd2.linux2.l_i_uid_ಮೇಲಿನ
#ವ್ಯಾಖ್ಯಾನಿಸು i_gid_ಮೇಲಿನ	osd2.linux2.l_i_gid_ಮೇಲಿನ
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ defined(__GNU__)

#ವ್ಯಾಖ್ಯಾನಿಸು i_translator	osd1.hurd1.h_i_translator
#ವ್ಯಾಖ್ಯಾನಿಸು i_frag		osd2.hurd2.h_i_frag;
#ವ್ಯಾಖ್ಯಾನಿಸು i_fsize		osd2.hurd2.h_i_fsize;
#ವ್ಯಾಖ್ಯಾನಿಸು i_uid_ಮೇಲಿನ	osd2.hurd2.h_i_uid_ಮೇಲಿನ
#ವ್ಯಾಖ್ಯಾನಿಸು i_gid_ಮೇಲಿನ	osd2.hurd2.h_i_gid_ಮೇಲಿನ
#ವ್ಯಾಖ್ಯಾನಿಸು i_author	osd2.hurd2.h_i_author

#ಆದರೆ_ಅಂತ್ಯ  /* __GNU__ */
#ಆದರೆ_ಅಂತ್ಯ	/* defined(__KERNEL__) || defined(__linux__) */

#ವ್ಯಾಖ್ಯಾನಿಸು inode_uid(inode)	((inode).i_uid | (ಚಿಹ್ನೆರಹಿತ)(inode).osd2.linux2.l_i_uid_ಮೇಲಿನ << 16)
#ವ್ಯಾಖ್ಯಾನಿಸು inode_gid(inode)	((inode).i_gid | (ಚಿಹ್ನೆರಹಿತ)(inode).osd2.linux2.l_i_gid_ಮೇಲಿನ << 16)
#ವ್ಯಾಖ್ಯಾನಿಸು inode_projid(inode)	((inode).i_projid)
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_ಗಣ_i_uid_ಮೇಲಿನ(inode,x) ((inode).osd2.linux2.l_i_uid_ಮೇಲಿನ = (x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_ಗಣ_i_gid_ಮೇಲಿನ(inode,x) ((inode).osd2.linux2.l_i_gid_ಮೇಲಿನ = (x))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ರಚನೆ ext2_inode *EXT2_INODE(ರಚನೆ ext2_inode_large *large_inode)
{
	ಹಿಂತಿರುಗಿ (ರಚನೆ ext2_inode *) large_inode;
}

/*
 * File system states
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_VALID_FS			0x0001	/* Unmounted cleanly */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ERROR_FS			0x0002	/* Errors detected */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_ORPHAN_FS			0x0004	/* Orphans being recovered */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FC_REPLAY			0x0020	/* Ext4 fast commit replay ongoing */

/*
 * Misc. filesystem flags
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAGS_SIGNED_HASH		0x0001  /* Signed dirhash in use */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAGS_UNSIGNED_HASH	0x0002  /* Unsigned dirhash in use */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAGS_TEST_FILESYS		0x0004	/* OK for use on development code */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAGS_IS_SNAPSHOT		0x0010	/* This is a snapshot image */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAGS_FIX_SNAPSHOT		0x0020	/* Snapshot inodes corrupted */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FLAGS_FIX_EXCLUDE		0x0040	/* Exclude bitmaps corrupted */

/*
 * Mount flags
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MOUNT_CHECK		0x0001	/* Do mount-time checks */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MOUNT_GRPID		0x0004	/* Create files with directory's group */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MOUNT_DEBUG		0x0008	/* Some debugging messages */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MOUNT_ERRORS_CONT		0x0010	/* Continue on errors */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MOUNT_ERRORS_RO		0x0020	/* Remount fs ro on errors */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MOUNT_ERRORS_PANIC		0x0040	/* Panic on errors */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MOUNT_MINIX_DF		0x0080	/* Mimics the Minix statfs */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MOUNT_NO_UID32		0x0200  /* Disable 32-bit UIDs */

#ವ್ಯಾಖ್ಯಾನಿಸು clear_opt(o, opt)		o &= ~EXT2_MOUNT_##opt
#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_opt(o, opt)			o |= EXT2_MOUNT_##opt
#ವ್ಯಾಖ್ಯಾನಿಸು test_opt(sb, opt)		(EXT2_SB(sb)->s_mount_opt & \
					 EXT2_MOUNT_##opt)
/*
 * Maximal mount counts between two filesystem checks
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DFL_MAX_MNT_ಎಣಿಕೆ		20	/* Allow 20 mounts */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DFL_CHECKINTERVAL		0	/* Don't use interval check */

/*
 * Behaviour when detecting errors
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ERRORS_CONTINUE		1	/* Continue execution */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ERRORS_RO			2	/* Remount fs read-only */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ERRORS_PANIC		3	/* Panic */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ERRORS_DEFAULT		EXT2_ERRORS_CONTINUE

#ಆದರೆ (__GNUC__ >= 4)
#ವ್ಯಾಖ್ಯಾನಿಸು ext4_offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ext4_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#ಆದರೆ_ಅಂತ್ಯ

/* Metadata checksum algorithms */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_C_CHKSUM		1

/* Encryption algorithms, key size and key reference len */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ENCRYPTION_ಬಹುಲಕ_INVALID		0
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ENCRYPTION_ಬಹುಲಕ_AES_256_XTS	1
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ENCRYPTION_ಬಹುಲಕ_AES_256_GCM	2
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ENCRYPTION_ಬಹುಲಕ_AES_256_CBC	3
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ENCRYPTION_ಬಹುಲಕ_AES_256_CTS	4

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_AES_256_XTS_ಕೀಲಿ_ಗಾತ್ರ		64
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_AES_256_GCM_ಕೀಲಿ_ಗಾತ್ರ		32
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_AES_256_CBC_ಕೀಲಿ_ಗಾತ್ರ		32
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_AES_256_CTS_ಕೀಲಿ_ಗಾತ್ರ		32
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MAX_ಕೀಲಿ_ಗಾತ್ರ			64

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ಕೀಲಿ_DESCRIPTOR_ಗಾತ್ರ		8
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_CRYPTO_ಖಂಡ_ಗಾತ್ರ			16

/* Password derivation constants */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MAX_PASSPHRASE_ಗಾತ್ರ		1024
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MAX_SALT_ಗಾತ್ರ			256
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_PBKDF2_ITERATIONS			0xFFFF

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_LABEL_LEN				16

/*
 * Policy provided via an ioctl on the topmost directory. This
 * structure is also in the kernel.
 */
ರಚನೆ ext4_encryption_policy {
  ಅಕ್ಷರ ಆವೃತ್ತಿ;
  ಅಕ್ಷರ contents_encryption_ಬಹುಲಕ;
  ಅಕ್ಷರ filenames_encryption_ಬಹುಲಕ;
  ಅಕ್ಷರ flags;
  ಅಕ್ಷರ master_ಕೀಲಿ_descriptor[EXT4_ಕೀಲಿ_DESCRIPTOR_ಗಾತ್ರ];
} __attribute__((__packed__));

ರಚನೆ ext4_encryption_ಕೀಲಿ {
        __u32 ಬಹುಲಕ;
        ಅಕ್ಷರ raw[EXT4_MAX_ಕೀಲಿ_ಗಾತ್ರ];
        __u32 ಗಾತ್ರ_2;
} __attribute__((__packed__));

/*
 * Structure of the super block
 */
ರಚನೆ ext2_super_ಖಂಡ {
/*000*/	__u32	s_inodes_ಎಣಿಕೆ;		/* Inodes count */
	__u32	s_ಖಂಡಗಳು_ಎಣಿಕೆ;		/* Blocks count */
	__u32	s_r_ಖಂಡಗಳು_ಎಣಿಕೆ;	/* Reserved blocks count */
	__u32	s_free_ಖಂಡಗಳು_ಎಣಿಕೆ;	/* Free blocks count */
/*010*/	__u32	s_free_inodes_ಎಣಿಕೆ;	/* Free inodes count */
	__u32	s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;	/* First Data Block */
	__u32	s_log_ಖಂಡ_ಗಾತ್ರ;	/* Block size */
	__u32	s_log_cluster_ಗಾತ್ರ;	/* Allocation cluster size */
/*020*/	__u32	s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;	/* # Blocks per group */
	__u32	s_clusters_ಪ್ರತಿ_ಗುಂಪು;	/* # Fragments per group */
	__u32	s_inodes_ಪ್ರತಿ_ಗುಂಪು;	/* # Inodes per group */
	__u32	s_mtime;		/* Mount time */
/*030*/	__u32	s_wtime;		/* Write time */
	__u16	s_mnt_ಎಣಿಕೆ;		/* Mount count */
	__s16	s_max_mnt_ಎಣಿಕೆ;	/* Maximal mount count */
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
	__u32	s_ಮೊದಲ_ino;		/* First non-reserved inode */
	__u16   s_inode_ಗಾತ್ರ;		/* size of inode structure */
	__u16	s_ಖಂಡ_ಗುಂಪು_nr;	/* block group # of this superblock */
	__u32	s_feature_compat;	/* compatible feature set */
/*060*/	__u32	s_feature_incompat;	/* incompatible feature set */
	__u32	s_feature_ro_compat;	/* readonly-compatible feature set */
/*068*/	__u8	s_uuid[16] __nonstring;		/* 128-bit uuid for volume */
/*078*/	__u8	s_volume_name[EXT2_LABEL_LEN] __nonstring;	/* volume name, no NUL? */
/*088*/	__u8	s_ಕೊನೆಯ_mounted[64] __nonstring;	/* directory last mounted on, no NUL? */
/*0c8*/	__u32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_ಖಂಡಗಳು;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_ಖಂಡಗಳು;	/* Nr to preallocate for dirs */
	__u16	s_reserved_gdt_ಖಂಡಗಳು;	/* Per group table for online growth */
	/*
	 * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
/*0d0*/	__u8	s_journal_uuid[16] __nonstring;	/* uuid of journal superblock */
/*0e0*/	__u32	s_journal_inum;		/* inode number of journal file */
	__u32	s_journal_dev;		/* device number of journal file */
	__u32	s_ಕೊನೆಯ_orphan;		/* start of list of inodes to delete */
/*0ec*/	__u32	s_hash_ಬೀಜ[4];		/* HTREE hash seed */
/*0fc*/	__u8	s_def_hash_ಆವೃತ್ತಿ;	/* Default hash version to use */
	__u8	s_jnl_backup_type;	/* Default type of journal backup */
	__u16	s_desc_ಗಾತ್ರ;		/* Group desc. size: INCOMPAT_64BIT */
/*100*/	__u32	s_default_mount_opts;	/* default EXT2_MOUNT_* flags used */
	__u32	s_ಮೊದಲ_meta_bg;	/* First metablock group */
	__u32	s_mkfs_ಸಮಯ;		/* When the filesystem was created */
/*10c*/	__u32	s_jnl_ಖಂಡಗಳು[17];	/* Backup of the journal inode */
/*150*/	__u32	s_ಖಂಡಗಳು_ಎಣಿಕೆ_hi;	/* Blocks count high 32bits */
	__u32	s_r_ಖಂಡಗಳು_ಎಣಿಕೆ_hi;	/* Reserved blocks count high 32 bits*/
	__u32	s_free_ಖಂಡಗಳು_hi;	/* Free blocks count */
	__u16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__u16	s_want_extra_isize;	/* New inodes should reserve # bytes */
/*160*/	__u32	s_flags;		/* Miscellaneous flags */
	__u16	s_raid_stride;		/* RAID stride in blocks */
	__u16	s_mmp_update_interval;  /* # seconds to wait in MMP checking */
	__u64	s_mmp_ಖಂಡ;		/* Block for multi-mount protection */
/*170*/	__u32	s_raid_stripe_ಅಗಲ;	/* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_ಪ್ರತಿ_flex;	/* FLEX_BG group size */
	__u8	s_checksum_type;	/* metadata checksum algorithm */
	__u8	s_encryption_level;	/* versioning level for encryption */
	__u8	s_reserved_pad;		/* Padding to next 32bits */
	__u64	s_kbytes_written;	/* nr of lifetime kilobytes written */
/*180*/	__u32	s_snapshot_inum;	/* Inode number of active snapshot */
	__u32	s_snapshot_id;		/* sequential ID of active snapshot */
	__u64	s_snapshot_r_ಖಂಡಗಳು_ಎಣಿಕೆ; /* active snapshot reserved blocks */
/*190*/	__u32	s_snapshot_list;	/* inode number of disk snapshot list */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_S_ERR_ಆರಂಭ ext4_offsetof(ರಚನೆ ext2_super_ಖಂಡ, s_error_ಎಣಿಕೆ)
	__u32	s_error_ಎಣಿಕೆ;		/* number of fs errors */
	__u32	s_ಮೊದಲ_error_ಸಮಯ;	/* first time an error happened */
	__u32	s_ಮೊದಲ_error_ino;	/* inode involved in first error */
/*1a0*/	__u64	s_ಮೊದಲ_error_ಖಂಡ;	/* block involved in first error */
	__u8	s_ಮೊದಲ_error_func[32] __nonstring;	/* function where error hit, no NUL? */
/*1c8*/	__u32	s_ಮೊದಲ_error_ಸಾಲು;	/* line number where error happened */
	__u32	s_ಕೊನೆಯ_error_ಸಮಯ;	/* most recent time of an error */
/*1d0*/	__u32	s_ಕೊನೆಯ_error_ino;	/* inode involved in last error */
	__u32	s_ಕೊನೆಯ_error_ಸಾಲು;	/* line number where error happened */
	__u64	s_ಕೊನೆಯ_error_ಖಂಡ;	/* block involved of last error */
/*1e0*/	__u8	s_ಕೊನೆಯ_error_func[32] __nonstring;	/* function where error hit, no NUL? */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_S_ERR_ಕೊನೆ ext4_offsetof(ರಚನೆ ext2_super_ಖಂಡ, s_mount_opts)
/*200*/	__u8	s_mount_opts[64] __nonstring;	/* default mount options, no NUL? */
/*240*/	__u32	s_usr_quota_inum;	/* inode number of user quota file */
	__u32	s_grp_quota_inum;	/* inode number of group quota file */
	__u32	s_overhead_clusters;	/* overhead blocks/clusters in fs */
/*24c*/	__u32	s_backup_bgs[2];	/* If sparse_super2 enabled */
/*254*/	__u8	s_encrypt_algos[4];	/* Encryption algorithms in use  */
/*258*/	__u8	s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
/*268*/	__le32	s_lpf_ino;		/* Location of the lost+found inode */
	__le32  s_prj_quota_inum;	/* inode for tracking project quota */
/*270*/	__le32	s_checksum_ಬೀಜ;	/* crc32c(orig_uuid) if csum_seed set */
/*274*/	__u8	s_wtime_hi;
	__u8	s_mtime_hi;
	__u8	s_mkfs_ಸಮಯ_hi;
	__u8	s_lastcheck_hi;
	__u8	s_ಮೊದಲ_error_ಸಮಯ_hi;
	__u8	s_ಕೊನೆಯ_error_ಸಮಯ_hi;
	__u8	s_ಮೊದಲ_error_errcode;
	__u8    s_ಕೊನೆಯ_error_errcode;
/*27c*/ __le16	s_encoding;		/* Filename charset encoding */
	__le16	s_encoding_flags;	/* Filename charset encoding flags */
	__le32  s_orphan_file_inum;	/* Inode for tracking orphan inodes */
	__le32	s_reserved[94];		/* Padding to the end of the block */
/*3fc*/	__u32	s_checksum;		/* crc32c(superblock) */
};

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_S_ERR_LEN (EXT4_S_ERR_ಕೊನೆ - EXT4_S_ERR_ಆರಂಭ)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_LEN_STR(buf) (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(buf), (ಅಕ್ಷರ *)buf

/*
 * Codes for operating systems
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_OS_LINUX		0
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_OS_HURD		1
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_OBSO_OS_MASIX	2
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_OS_FREEBSD		3
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_OS_LITES		4

/*
 * Revision levels
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_GOOD_OLD_REV	0	/* The good old (original) format */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DYNAMIC_REV	1	/* V2 format w/ dynamic inode sizes */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_GOOD_OLD_INODE_ಗಾತ್ರ 128

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_EXTRA_TIMESTAMP_MAX	(((int64_t)1 << 34) - 1  + INT32_MIN)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_NON_EXTRA_TIMESTAMP_MAX	INT32_MAX
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_TIMESTAMP_MIN		INT32_MIN

/*
 * Journal inode backup types
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_JNL_BACKUP_ಖಂಡಗಳು	1

/*
 * Feature set definitions
 */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HAS_COMPAT_FEATURE(sb,ಮುಖವಾಡ)			\
	( EXT2_SB(sb)->s_feature_compat & (ಮುಖವಾಡ) )
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HAS_RO_COMPAT_FEATURE(sb,ಮುಖವಾಡ)			\
	( EXT2_SB(sb)->s_feature_ro_compat & (ಮುಖವಾಡ) )
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_HAS_INCOMPAT_FEATURE(sb,ಮುಖವಾಡ)			\
	( EXT2_SB(sb)->s_feature_incompat & (ಮುಖವಾಡ) )

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_COMPAT_RESIZE_INODE	0x0010
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_COMPAT_DIR_ಸೂಚ್ಯಂಕ		0x0020
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_COMPAT_LAZY_BG		0x0040
/* #define EXT2_FEATURE_COMPAT_EXCLUDE_INODE	0x0080 not used, legacy */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP	0x0100
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_COMPAT_SPARSE_SUPER2	0x0200
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_COMPAT_FAST_COMMIT		0x0400
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_COMPAT_STABLE_INODES	0x0800
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_COMPAT_ORPHAN_FILE		0x1000

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004 not used */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_HUGE_FILE	0x0008
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_GDT_CSUM		0x0010
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_DIR_NLINK	0x0020
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE	0x0040
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT	0x0080
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_QUOTA		0x0100
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_BIGALLOC		0x0200
/*
 * METADATA_CSUM implies GDT_CSUM.  When METADATA_CSUM is set, group
 * descriptor checksums use the same algorithm as all other data
 * structures' checksums.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_METADATA_CSUM	0x0400
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_REPLICA		0x0800
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_READONLY		0x1000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_PROJECT		0x2000 /* Project quota */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_SHARED_ಖಂಡಗಳು	0x4000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_VERITY		0x8000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT	0x10000

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 /* Needs recovery */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 /* Journal device */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_INCOMPAT_META_BG		0x0010
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_FEATURE_INCOMPAT_EXTENTS		0x0040
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_64_ಬಿಟ್		0x0080
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_MMP		0x0100
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_FLEX_BG		0x0200
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_EA_INODE		0x0400
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_DIRDATA		0x1000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_CSUM_ಬೀಜ		0x2000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_LARGEDIR		0x4000 /* >2GB or 3-lvl htree */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_INLINE_ದತ್ತಾಂಶ	0x8000 /* data in inode */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_ENCRYPT		0x10000
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_CASEFOLD		0x20000

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_COMPAT_FUNCS(name, ver, flagname) \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ext2fs_has_feature_##name(ರಚನೆ ext2_super_ಖಂಡ *sb) \
{ \
	ಹಿಂತಿರುಗಿ ((EXT2_SB(sb)->s_feature_compat & \
		 EXT##ver##_FEATURE_COMPAT_##flagname) != 0); \
} \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_ಗಣ_feature##name(ರಚನೆ ext2_super_ಖಂಡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat |= \
		EXT##ver##_FEATURE_COMPAT_##flagname; \
} \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_clear_feature_##name(ರಚನೆ ext2_super_ಖಂಡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat &= \
		~EXT##ver##_FEATURE_COMPAT_##flagname; \
}

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_RO_COMPAT_FUNCS(name, ver, flagname) \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ext2fs_has_feature_##name(ರಚನೆ ext2_super_ಖಂಡ *sb) \
{ \
	ಹಿಂತಿರುಗಿ ((EXT2_SB(sb)->s_feature_ro_compat & \
		 EXT##ver##_FEATURE_RO_COMPAT_##flagname) != 0); \
} \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_ಗಣ_feature##name(ರಚನೆ ext2_super_ಖಂಡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat |= \
		EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
} \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_clear_feature_##name(ರಚನೆ ext2_super_ಖಂಡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat &= \
		~EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
}

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_FEATURE_INCOMPAT_FUNCS(name, ver, flagname) \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ext2fs_has_feature_##name(ರಚನೆ ext2_super_ಖಂಡ *sb) \
{ \
	ಹಿಂತಿರುಗಿ ((EXT2_SB(sb)->s_feature_incompat & \
		 EXT##ver##_FEATURE_INCOMPAT_##flagname) != 0); \
} \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_ಗಣ_feature##name(ರಚನೆ ext2_super_ಖಂಡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat |= \
		EXT##ver##_FEATURE_INCOMPAT_##flagname; \
} \
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ext2fs_clear_feature_##name(ರಚನೆ ext2_super_ಖಂಡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat &= \
		~EXT##ver##_FEATURE_INCOMPAT_##flagname; \
}

EXT4_FEATURE_COMPAT_FUNCS(dir_prealloc,		2, DIR_PREALLOC)
EXT4_FEATURE_COMPAT_FUNCS(imagic_inodes,	2, IMAGIC_INODES)
EXT4_FEATURE_COMPAT_FUNCS(journal,		3, HAS_JOURNAL)
EXT4_FEATURE_COMPAT_FUNCS(xattr,		2, EXT_ATTR)
EXT4_FEATURE_COMPAT_FUNCS(resize_inode,		2, RESIZE_INODE)
EXT4_FEATURE_COMPAT_FUNCS(dir_ಸೂಚ್ಯಂಕ,		2, DIR_ಸೂಚ್ಯಂಕ)
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
EXT4_FEATURE_RO_COMPAT_FUNCS(shared_ಖಂಡಗಳು,	4, SHARED_ಖಂಡಗಳು)
EXT4_FEATURE_RO_COMPAT_FUNCS(verity,		4, VERITY)
EXT4_FEATURE_RO_COMPAT_FUNCS(orphan_present,	4, ORPHAN_PRESENT)

EXT4_FEATURE_INCOMPAT_FUNCS(compression,	2, COMPRESSION)
EXT4_FEATURE_INCOMPAT_FUNCS(filetype,		2, FILETYPE)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_needs_recovery,	3, RECOVER)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_dev,	3, JOURNAL_DEV)
EXT4_FEATURE_INCOMPAT_FUNCS(meta_bg,		2, META_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(extents,		3, EXTENTS)
EXT4_FEATURE_INCOMPAT_FUNCS(64ಬಿಟ್,		4, 64ಬಿಟ್_2)
EXT4_FEATURE_INCOMPAT_FUNCS(mmp,		4, MMP)
EXT4_FEATURE_INCOMPAT_FUNCS(flex_bg,		4, FLEX_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(ea_inode,		4, EA_INODE)
EXT4_FEATURE_INCOMPAT_FUNCS(dirdata,		4, DIRDATA)
EXT4_FEATURE_INCOMPAT_FUNCS(csum_ಬೀಜ,		4, CSUM_ಬೀಜ)
EXT4_FEATURE_INCOMPAT_FUNCS(largedir,		4, LARGEDIR)
EXT4_FEATURE_INCOMPAT_FUNCS(inline_ದತ್ತಾಂಶ,	4, INLINE_ದತ್ತಾಂಶ)
EXT4_FEATURE_INCOMPAT_FUNCS(encrypt,		4, ENCRYPT)
EXT4_FEATURE_INCOMPAT_FUNCS(casefold,		4, CASEFOLD)

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_COMPAT_SUPP	0
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_INCOMPAT_SUPP    (EXT2_FEATURE_INCOMPAT_FILETYPE| \
				       EXT4_FEATURE_INCOMPAT_MMP| \
				       EXT4_FEATURE_INCOMPAT_LARGEDIR| \
				       EXT4_FEATURE_INCOMPAT_EA_INODE)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
					 EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \
					 EXT4_FEATURE_RO_COMPAT_DIR_NLINK| \
					 EXT2_FEATURE_RO_COMPAT_BTREE_DIR| \
					 EXT4_FEATURE_RO_COMPAT_VERITY)

/*
 * Default values for user and/or group using reserved blocks
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DEF_RESUID		0
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DEF_RESGID		0

/*
 * Default mount options
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DEFM_DEBUG		0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DEFM_BSDGROUPS	0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DEFM_XATTR_USER	0x0004
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DEFM_ACL		0x0008
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DEFM_UID16		0x0010
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_DEFM_JMODE		0x0060
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_DEFM_JMODE_ದತ್ತಾಂಶ	0x0020
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_DEFM_JMODE_ORDERED	0x0040
#ವ್ಯಾಖ್ಯಾನಿಸು EXT3_DEFM_JMODE_WBACK	0x0060
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_DEFM_NOBARRIER	0x0100
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_DEFM_ಖಂಡ_VALIDITY 0x0200
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_DEFM_DISCARD	0x0400
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_DEFM_NODELALLOC	0x0800

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ext4_hash_ಒಳ_dirent(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ (inode->i_flags & EXT4_ENCRYPT_FL) &&
		(inode->i_flags & EXT4_CASEFOLD_FL);
}

/*
 * Structure of a directory entry
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_NAME_LEN 255

ರಚನೆ ext2_dir_ನಮೂದು {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u16	name_len;		/* Name length */
	ಅಕ್ಷರ	name[EXT2_NAME_LEN];	/* File name */
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
ರಚನೆ ext2_dir_ನಮೂದು_2 {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u8	name_len;		/* Name length */
	__u8	file_type;
	ಅಕ್ಷರ	name[EXT2_NAME_LEN];	/* File name */
};

/*
 * Hashes for ext4_dir_entry for casefolded and ecrypted directories.
 * This is located at the first 4 bit aligned location after the name.
 */

ರಚನೆ ext2_dir_ನಮೂದು_hash {
	__le32 hash;
	__le32 minor_hash;
};

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIRENT_HASHES(ನಮೂದು) \
	((ರಚನೆ ext2_dir_ನಮೂದು_hash *) &ನಮೂದು->name[\
		(ext2fs_dirent_name_len(ನಮೂದು) + \
			EXT2_DIR_ಸುತ್ತು) & ~EXT2_DIR_ಸುತ್ತು])
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIRENT_HASH(ನಮೂದು) \
		ext2fs_le32_ಗಮ್ಯ_cpu(EXT2_DIRENT_HASHES(ನಮೂದು)->hash)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIRENT_MINOR_HASH(ನಮೂದು) \
		ext2fs_le32_ಗಮ್ಯ_cpu(EXT2_DIRENT_HASHES(ನಮೂದು)->minor_hash)

/*
 * This is a bogus directory entry at the end of each leaf block that
 * records checksums.
 */
ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ {
	__u32	det_reserved_zero1;	/* Pretend to be unused */
	__u16	det_rec_len;		/* 12 */
	__u16	det_reserved_name_len;	/* 0xDE00, fake namelen/filetype */
	__u32	det_checksum;		/* crc32c(uuid+inode+dirent) */
};

/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_UNKNOWN		0
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_REG_FILE	1
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_DIR		2
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_CHRDEV		3
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_BLKDEV		4
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_FIFO		5
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_SOCK		6
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_SYMLINK		7

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_MAX		8

/*
 * Annoyingly, e2fsprogs always swab16s ext2_dir_entry.name_len, so we
 * have to build ext2_dir_entry_tail with that assumption too.  This
 * constant helps to build the dir_entry_tail to look like it has an
 * "invalid" file type.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIR_NAME_LEN_CSUM	0xDE00

/*
 * EXT2_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIR_ನಮೂದು_ಶೀರ್ಷಿಕೆ_LEN	8
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIR_ನಮೂದು_HASH_LEN		8
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIR_PAD			4
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIR_ಸುತ್ತು			(EXT2_DIR_PAD - 1)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DIR_REC_LEN(name_len) ext2fs_dir_rec_len(name_len, 0)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ext2fs_dir_rec_len(__u8 name_len,
						ಪೂರ್ಣಾಂಕ extended)
{
	ಪೂರ್ಣಾಂಕ rec_len = (name_len + EXT2_DIR_ನಮೂದು_ಶೀರ್ಷಿಕೆ_LEN + EXT2_DIR_ಸುತ್ತು);

	rec_len &= ~EXT2_DIR_ಸುತ್ತು;
	ಆದರೆ (extended)
		rec_len += EXT2_DIR_ನಮೂದು_HASH_LEN;
	ಹಿಂತಿರುಗಿ rec_len;
}

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ORPHAN_ಖಂಡ_MAGIC 0x0b10ca04

/* Structure at the tail of orphan block */
ರಚನೆ ext4_orphan_ಖಂಡ_ಬಾಲ {
	__le32 ob_magic;
	__le32 ob_checksum;
};

/*
 * Constants for ext4's extended time encoding
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_EPOCH_ಬಿಟ್ಟುಗಳು 2
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_EPOCH_ಮುಖವಾಡ ((1 << EXT4_EPOCH_ಬಿಟ್ಟುಗಳು) - 1)
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_NSEC_ಮುಖವಾಡ  (~0UL << EXT4_EPOCH_ಬಿಟ್ಟುಗಳು)

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
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MMP_MAGIC     0x004D4D50U /* ASCII for MMP */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MMP_SEQ_CLEAN 0xFF4D4D50U /* mmp_seq value for clean unmount */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MMP_SEQ_FSCK  0xE24D4D50U /* mmp_seq value when being fscked */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MMP_SEQ_MAX   0xE24D4D4FU /* maximum valid mmp_seq value */

/* Not endian-annotated; it's swapped at read/write time */
ರಚನೆ mmp_struct {
	__u32	mmp_magic;		/* Magic number for MMP */
	__u32	mmp_seq;		/* Sequence no. updated periodically */
	__u64	mmp_ಸಮಯ;		/* Time last updated (seconds) */
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
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MMP_UPDATE_INTERVAL	5

/*
 * Maximum interval for MMP update in seconds.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MMP_MAX_UPDATE_INTERVAL	300

/*
 * Minimum interval for MMP checking in seconds.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MMP_MIN_CHECK_INTERVAL     5

/*
 * Minimum size of inline data.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ	((ಗಾತ್ರ(__u32) * EXT2_N_ಖಂಡಗಳು))

/*
 * Size of a parent inode in inline data directory.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_INLINE_ದತ್ತಾಂಶ_DOTDOT_ಗಾತ್ರ	(4)

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ENC_UTF8_12_1	1

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ENC_STRICT_ಬಹುಲಕ_FL			(1 << 0) /* Reject invalid sequences */

#ಆದರೆ_ಅಂತ್ಯ	/* _LINUX_EXT2_FS_H */
