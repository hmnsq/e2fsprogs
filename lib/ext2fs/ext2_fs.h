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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _LINUX_EXT2_FS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _LINUX_EXT2_FS_H

#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext2_types.h>		/* Changed from linux/types.h */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __GNUC_PREREQ
#ਜੇ defined(__GNUC__) && defined(__GNUC_MINOR__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __GNUC_PREREQ(maj, min) 0
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __nonstring
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __has_attribute
#ਜੇ __has_attribute(__nonstring__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __nonstring                    __attribute__((__nonstring__))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __nonstring
#ਜੇ_ਅੰਤ /* __has_attribute(__nonstring__) */
#ਨਹੀਂਤਾਂ
# define __nonstring
#ਜੇ_ਅੰਤ /* __has_attribute */
#ਜੇ_ਅੰਤ /* __nonstring */

/*
 * The second extended filesystem constants/structures
 */

/*
 * Define EXT2FS_DEBUG to produce debug messages
 */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ EXT2FS_DEBUG

/*
 * Define EXT2_PREALLOCATE to preallocate data blocks for expanding files
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_PREALLOCATE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DEFAULT_PREALLOC_ਖੰਡਾਂ	8

/*
 * The second extended file system version
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_DATE		"95/08/09"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FS_ਸੰਸਕਰਣ		"0.5b"

/*
 * Special inode numbers
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_BAD_INO		 1	/* Bad blocks inode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਜੜ੍ਹ_INO		 2	/* Root inode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_USR_QUOTA_INO	 3	/* User quota inode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_GRP_QUOTA_INO	 4	/* Group quota inode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_BOOT_LOADER_INO	 5	/* Boot loader inode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_RESIZE_INO		 7	/* Reserved group descriptors inode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_JOURNAL_INO	 8	/* Journal inode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_EXCLUDE_INO	 9	/* The "exclude" inode, for snapshots */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_REPLICA_INO	10	/* Used by non-upstream feature */

/* First non-reserved inode for old ext2 filesystems */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_GOOD_OLD_ਪਹਿਲਾ_INO	11

/*
 * The second extended file system magic number
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SUPER_MAGIC	0xEF53

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __KERNEL__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SB(sb)	(&((sb)->u.ext2_sb))
#ਨਹੀਂਤਾਂ
/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SB(sb)	(sb)
#ਜੇ_ਅੰਤ

/*
 * Maximal count of links to a file
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_LINK_MAX		65000

/*
 * Macro-instructions used to manage several block sizes
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MIN_ਖੰਡ_LOG_ਮਾਪ		10	/* 1024 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MAX_ਖੰਡ_LOG_ਮਾਪ		16	/* 65536 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MIN_ਖੰਡ_ਮਾਪ	(1 << EXT2_MIN_ਖੰਡ_LOG_ਮਾਪ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MAX_ਖੰਡ_ਮਾਪ	(1 << EXT2_MAX_ਖੰਡ_LOG_ਮਾਪ)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __KERNEL__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਖੰਡ_ਮਾਪ(s)	((s)->s_blocksize)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਖੰਡ_ਮਾਪ_ਬਿੱਟ(s)	((s)->s_blocksize_ਬਿੱਟ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ADDR_ਪ੍ਰਤੀ_ਖੰਡ_ਬਿੱਟ(s)	(EXT2_SB(s)->addr_ਪ੍ਰਤੀ_ਖੰਡ_ਬਿੱਟ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_INODE_ਮਾਪ(s)	(EXT2_SB(s)->s_inode_ਮਾਪ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਪਹਿਲਾ_INO(s)	(EXT2_SB(s)->s_ਪਹਿਲਾ_ino)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਖੰਡ_ਮਾਪ(s)	(EXT2_MIN_ਖੰਡ_ਮਾਪ << (s)->s_log_ਖੰਡ_ਮਾਪ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਖੰਡ_ਮਾਪ_ਬਿੱਟ(s)	((s)->s_log_ਖੰਡ_ਮਾਪ + 10)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_INODE_ਮਾਪ(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_INODE_ਮਾਪ : (s)->s_inode_ਮਾਪ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਪਹਿਲਾ_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_ਪਹਿਲਾ_INO : (s)->s_ਪਹਿਲਾ_ino)
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ADDR_ਪ੍ਰਤੀ_ਖੰਡ(s)	(EXT2_ਖੰਡ_ਮਾਪ(s) / ਆਕਾਰ(__u32))

/*
 * Macro-instructions used to manage allocation clusters
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MIN_CLUSTER_LOG_ਮਾਪ	EXT2_MIN_ਖੰਡ_LOG_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MAX_CLUSTER_LOG_ਮਾਪ	29	/* 512MB  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MIN_CLUSTER_ਮਾਪ		EXT2_MIN_ਖੰਡ_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MAX_CLUSTER_ਮਾਪ		(1 << EXT2_MAX_CLUSTER_LOG_ਮਾਪ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CLUSTER_ਮਾਪ(s)		(EXT2_MIN_ਖੰਡ_ਮਾਪ << \
						(s)->s_log_cluster_ਮਾਪ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CLUSTER_ਮਾਪ_ਬਿੱਟ(s)	((s)->s_log_cluster_ਮਾਪ + 10)

/*
 * Macro-instructions used to manage fragments
 *
 * Note: for backwards compatibility only, for the dump program.
 * Ext2/3/4 will never support fragments....
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MIN_FRAG_ਮਾਪ              EXT2_MIN_ਖੰਡ_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MAX_FRAG_ਮਾਪ              EXT2_MAX_ਖੰਡ_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MIN_FRAG_LOG_ਮਾਪ          EXT2_MIN_ਖੰਡ_LOG_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FRAG_ਮਾਪ(s)		EXT2_ਖੰਡ_ਮਾਪ(s)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FRAGS_ਪ੍ਰਤੀ_ਖੰਡ(s)		1

/*
 * ACL structures
 */
ਬਣਤਰ ext2_acl_ਸਿਰਲੇਖ	/* Header of Access Control Lists */
{
	__u32	aclh_ਮਾਪ;
	__u32	aclh_file_ਗਿਣਤੀ;
	__u32	aclh_acle_ਗਿਣਤੀ;
	__u32	aclh_ਪਹਿਲਾ_acle;
};

ਬਣਤਰ ext2_acl_ਇੰਦਰਾਜ	/* Access Control List Entry */
{
	__u32	acle_ਮਾਪ;
	__u16	acle_perms;	/* Access permissions */
	__u16	acle_type;	/* Type of entry */
	__u16	acle_ਟੈਗ;	/* User or group identity */
	__u16	acle_pad1;
	__u32	acle_ਅਗਲਾ;	/* Pointer on next entry for the */
					/* same inode or on next free entry */
};

/*
 * Structure of a blocks group descriptor
 */
ਬਣਤਰ ext2_ਸਮੂਹ_desc
{
	__u32	bg_ਖੰਡ_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_ਸਾਰਣੀ;		/* Inodes table block */
	__u16	bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ;	/* Free blocks count */
	__u16	bg_free_inodes_ਗਿਣਤੀ;	/* Free inodes count */
	__u16	bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ;	/* Directories count */
	__u16	bg_flags;
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_ਖੰਡ_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_ਅਣਵਰਤਿਆ;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(s_uuid+group_num+group_desc)*/
};

/*
 * Structure of a blocks group descriptor
 */
ਬਣਤਰ ext4_ਸਮੂਹ_desc
{
	__u32	bg_ਖੰਡ_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_ਸਾਰਣੀ;		/* Inodes table block */
	__u16	bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ;	/* Free blocks count */
	__u16	bg_free_inodes_ਗਿਣਤੀ;	/* Free inodes count */
	__u16	bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ;	/* Directories count */
	__u16	bg_flags;		/* EXT4_BG_flags (INODE_UNINIT, etc) */
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_ਖੰਡ_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_ਅਣਵਰਤਿਆ;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(sb_uuid+group+desc) */
	__u32	bg_ਖੰਡ_bitmap_hi;	/* Blocks bitmap block MSB */
	__u32	bg_inode_bitmap_hi;	/* Inodes bitmap block MSB */
	__u32	bg_inode_ਸਾਰਣੀ_hi;	/* Inodes table block MSB */
	__u16	bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_hi;/* Free blocks count MSB */
	__u16	bg_free_inodes_ਗਿਣਤੀ_hi;/* Free inodes count MSB */
	__u16	bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ_hi;	/* Directories count MSB */
	__u16	bg_itable_ਅਣਵਰਤਿਆ_hi;	/* Unused inodes count MSB */
	__u32	bg_exclude_bitmap_hi;	/* Exclude bitmap block MSB */
	__u16	bg_ਖੰਡ_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u16	bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u32	bg_reserved;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_BG_INODE_BITMAP_CSUM_HI_ਸਿਰਾ	\
	(offsetof(ਬਣਤਰ ext4_ਸਮੂਹ_desc, bg_inode_bitmap_csum_hi) + \
	 ਆਕਾਰ(__u16))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_BG_ਖੰਡ_BITMAP_CSUM_HI_LOCATION	\
	(offsetof(ਬਣਤਰ ext4_ਸਮੂਹ_desc, bg_ਖੰਡ_bitmap_csum_hi) + \
	 ਆਕਾਰ(__u16))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_BG_INODE_UNINIT	0x0001 /* Inode table/bitmap not initialized */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_BG_ਖੰਡ_UNINIT	0x0002 /* Block bitmap not initialized */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_BG_INODE_ZEROED	0x0004 /* On-disk itable initialized to zero */

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
ਬਣਤਰ ext2_dx_ਜੜ੍ਹ_info {
	__u32 reserved_zero;
	__u8 hash_ਸੰਸਕਰਣ; /* 0 now, 1 at release */
	__u8 info_ਲੰਮਾਈ; /* 8 */
	__u8 ਅਸਿੱਧਾ_ਪੱਧਰ;
	__u8 ਅਣਵਰਤਿਆ_flags;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HASH_LEGACY		0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HASH_HALF_MD4		1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HASH_TEA			2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HASH_LEGACY_UNSIGNED	3 /* reserved for userspace lib */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HASH_HALF_MD4_UNSIGNED	4 /* reserved for userspace lib */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HASH_TEA_UNSIGNED		5 /* reserved for userspace lib */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HASH_SIPHASH		6

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HASH_FLAG_INCOMPAT	0x1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_DX_ਖੰਡ_ਨਕਾਬ 0x0fffffff

ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ {
	__le32 hash;
	__le32 ਖੰਡ;
};

ਬਣਤਰ ext2_dx_countlimit {
	__le16 ਹੱਦਬੰਦੀ;
	__le16 ਗਿਣਤੀ_2;
};

/*
 * This goes at the end of each htree block.
 */
ਬਣਤਰ ext2_dx_ਪੂਛ {
	__le32 dt_reserved;
	__le32 dt_checksum;	/* crc32c(uuid+inum+dxblock) */
};

/*
 * Macro-instructions used to manage group descriptors
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MIN_DESC_ਮਾਪ             32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MIN_DESC_ਮਾਪ_64_ਦੋਆਧਾਰੀ_ਅੰਕ       64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MAX_DESC_ਮਾਪ             EXT2_MIN_ਖੰਡ_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DESC_ਮਾਪ(s)                                                \
	(ext2fs_has_feature_64bit(s) ? (s)->s_desc_ਮਾਪ : EXT2_MIN_DESC_ਮਾਪ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(s)	(EXT2_SB(s)->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(s)	(EXT2_SB(s)->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(s)	(EXT2_SB(s)->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_INODES_ਪ੍ਰਤੀ_ਖੰਡ(s)	(EXT2_ਖੰਡ_ਮਾਪ(s)/EXT2_INODE_ਮਾਪ(s))
/* limits imposed by 16-bit value gd_free_{blocks,inode}_count */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MAX_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(s)	((((ਬਿਨਾਨਿਸ਼ਾਨੀ) 1 << 16) - 8) *	\
					 (EXT2_CLUSTER_ਮਾਪ(s) / \
					  EXT2_ਖੰਡ_ਮਾਪ(s)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MAX_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(s)	(((ਬਿਨਾਨਿਸ਼ਾਨੀ) 1 << 16) - 8)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MAX_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(s)	(((ਬਿਨਾਨਿਸ਼ਾਨੀ) 1 << 16) - \
					 EXT2_INODES_ਪ੍ਰਤੀ_ਖੰਡ(s))
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __KERNEL__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(s)		(EXT2_SB(s)->s_desc_ਪ੍ਰਤੀ_ਖੰਡ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ_ਬਿੱਟ(s)	(EXT2_SB(s)->s_desc_ਪ੍ਰਤੀ_ਖੰਡ_ਬਿੱਟ)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(s)		(EXT2_ਖੰਡ_ਮਾਪ(s) / EXT2_DESC_ਮਾਪ(s))
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_GROUPS_ਮੰਜ਼ਿਲ_ਖੰਡਾਂ(s, g)   ((blk64_t) EXT2_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(s) * \
				       (g))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_GROUPS_ਮੰਜ਼ਿਲ_CLUSTERS(s, g) ((blk64_t) EXT2_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(s) * \
				       (g))

/*
 * Constants relative to the data blocks
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_NDIR_ਖੰਡਾਂ		12
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IND_ਖੰਡ			EXT2_NDIR_ਖੰਡਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIND_ਖੰਡ			(EXT2_IND_ਖੰਡ + 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_TIND_ਖੰਡ			(EXT2_DIND_ਖੰਡ + 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_N_ਖੰਡਾਂ			(EXT2_TIND_ਖੰਡ + 1)

/*
 * Inode flags
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SECRM_FL			0x00000001 /* Secure deletion */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_UNRM_FL			0x00000002 /* Undelete */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_COMPR_FL			0x00000004 /* Compress file */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_SYNC_FL			0x00000008 /* Synchronous updates */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IMMUTABLE_FL		0x00000010 /* Immutable file */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_APPEND_FL			0x00000020 /* writes to file may only append */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_NODUMP_FL			0x00000040 /* do not dump file */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIRTY_FL			0x00000100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_NOCOMPR_FL			0x00000400 /* Access raw compressed data */
	/* nb: was previously EXT2_ECOMPR_FL */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ENCRYPT_FL			0x00000800 /* encrypted inode */
/* End compression flags --- maybe not all used */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_BTREE_FL			0x00001000 /* btree format dir */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਸੂਚਕਾਂਕ_FL			0x00001000 /* hash-indexed directory */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IMAGIC_FL			0x00002000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_JOURNAL_ਅੰਕੜੇ_FL		0x00004000 /* file data should be journaled */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIRSYNC_FL 		0x00010000 /* Synchronous directory modifications */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_EXTENTS_FL 		0x00080000 /* Inode uses extents */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_VERITY_FL			0x00100000 /* Verity protected inode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_EA_INODE_FL	        0x00200000 /* Inode used for large EA */
/* EXT4_EOFBLOCKS_FL 0x00400000 was here */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FS_NOCOW_FL			0x00800000 /* Do not cow file */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_SNAPFILE_FL		0x01000000  /* Inode is a snapshot */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FS_DAX_FL			0x02000000 /* Inode is DAX */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_SNAPFILE_DELETED_FL	0x04000000  /* Snapshot is being deleted */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_SNAPFILE_SHRUNK_FL		0x08000000  /* Snapshot shrink has completed */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_INLINE_ਅੰਕੜੇ_FL		0x10000000 /* Inode has inline data */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_PROJINHERIT_FL		0x20000000 /* Create with parents projid */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_CASEFOLD_FL		0x40000000 /* Casefolded file */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_RESERVED_FL		0x80000000 /* reserved for ext2 lib */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FL_USER_VISIBLE		0x604BDFFF /* User visible flags */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FL_USER_MODIFIABLE		0x604B80FF /* User modifiable flags */

/*
 * ioctl commands
 */

/* Used for online resize */
ਬਣਤਰ ext2_new_ਸਮੂਹ_ਨਿਵੇਸ਼ {
	__u32 ਸਮੂਹ;		/* Group number for this data */
	__u32 ਖੰਡ_bitmap;	/* Absolute block number of block bitmap */
	__u32 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u32 inode_ਸਾਰਣੀ;	/* Absolute block number of inode table start */
	__u32 ਖੰਡਾਂ_ਗਿਣਤੀ;	/* Total number of blocks in this group */
	__u16 reserved_ਖੰਡਾਂ;	/* Number of reserved blocks in this group */
	__u16 ਅਣਵਰਤਿਆ;		/* Number of reserved GDT blocks in group */
};

ਬਣਤਰ ext4_new_ਸਮੂਹ_ਨਿਵੇਸ਼ {
	__u32 ਸਮੂਹ;		/* Group number for this data */
	__u64 ਖੰਡ_bitmap;	/* Absolute block number of block bitmap */
	__u64 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u64 inode_ਸਾਰਣੀ;	/* Absolute block number of inode table start */
	__u32 ਖੰਡਾਂ_ਗਿਣਤੀ;	/* Total number of blocks in this group */
	__u16 reserved_ਖੰਡਾਂ;	/* Number of reserved blocks in this group */
	__u16 ਅਣਵਰਤਿਆ;
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNU__			/* Needed for the Hurd */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IOT_ext2_new_ਸਮੂਹ_ਨਿਵੇਸ਼ _IOT (_IOTS(__u32), 5, _IOTS(__u16), 2, 0, 0)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IOC_GETFLAGS		_IOR('f', 1, ਲੰਮਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IOC_SETFLAGS		_IOW('f', 2, ਲੰਮਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IOC_GETVERSION		_IOR('v', 1, ਲੰਮਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IOC_SETVERSION		_IOW('v', 2, ਲੰਮਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IOC_GETVERSION_NEW		_IOR('f', 3, ਲੰਮਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IOC_SETVERSION_NEW		_IOW('f', 4, ਲੰਮਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IOC_ਸਮੂਹ_EXTEND		_IOW('f', 7, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_IOC_ਸਮੂਹ_ਜੋੜੋ		_IOW('f', 8,ਬਣਤਰ ext2_new_ਸਮੂਹ_ਨਿਵੇਸ਼)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_IOC_ਸਮੂਹ_ਜੋੜੋ		_IOW('f', 8,ਬਣਤਰ ext4_new_ਸਮੂਹ_ਨਿਵੇਸ਼)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_IOC_RESIZE_FS		_IOW('f', 16, __u64)

/*
 * Structure of an inode on the disk
 */
ਬਣਤਰ ext2_inode {
/*00*/	__u16	i_ਬਹੁਲਕ;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_ਮਾਪ;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_ਗਿਣਤੀ;	/* Links count */
	__u32	i_ਖੰਡਾਂ;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	ਸੰਘ {
		ਬਣਤਰ {
			__u32	l_i_ਸੰਸਕਰਣ; /* was l_i_reserved1 */
		} linux1;
		ਬਣਤਰ {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_ਖੰਡ[EXT2_N_ਖੰਡਾਂ];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_ਮਾਪ_ਉੱਚਾ;
/*70*/	__u32	i_faddr;	/* Fragment address */
	ਸੰਘ {
		ਬਣਤਰ {
			__u16	l_i_ਖੰਡਾਂ_hi;
			__u16	l_i_file_acl_ਉੱਚਾ;
			__u16	l_i_uid_ਉੱਚਾ;	/* these 2 fields    */
			__u16	l_i_gid_ਉੱਚਾ;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		ਬਣਤਰ {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_ਬਹੁਲਕ_ਉੱਚਾ;
			__u16	h_i_uid_ਉੱਚਾ;
			__u16	h_i_gid_ਉੱਚਾ;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
};

/*
 * Permanent part of an large inode on the disk
 */
ਬਣਤਰ ext2_inode_large {
/*00*/	__u16	i_ਬਹੁਲਕ;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_ਮਾਪ;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode Change time */
/*10*/	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_ਗਿਣਤੀ;	/* Links count */
	__u32	i_ਖੰਡਾਂ;	/* Blocks count */
/*20*/	__u32	i_flags;	/* File flags */
	ਸੰਘ {
		ਬਣਤਰ {
			__u32	l_i_ਸੰਸਕਰਣ; /* was l_i_reserved1 */
		} linux1;
		ਬਣਤਰ {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
/*28*/	__u32	i_ਖੰਡ[EXT2_N_ਖੰਡਾਂ];/* Pointers to blocks */
/*64*/	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_ਮਾਪ_ਉੱਚਾ;
/*70*/	__u32	i_faddr;	/* Fragment address */
	ਸੰਘ {
		ਬਣਤਰ {
			__u16	l_i_ਖੰਡਾਂ_hi;
			__u16	l_i_file_acl_ਉੱਚਾ;
			__u16	l_i_uid_ਉੱਚਾ;	/* these 2 fields    */
			__u16	l_i_gid_ਉੱਚਾ;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		ਬਣਤਰ {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_ਬਹੁਲਕ_ਉੱਚਾ;
			__u16	h_i_uid_ਉੱਚਾ;
			__u16	h_i_gid_ਉੱਚਾ;
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
	__u32	i_ਸੰਸਕਰਣ_hi;	/* high 32 bits for 64-bit version */
/*9c*/	__u32   i_projid;       /* Project ID */
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_INODE_CSUM_HI_EXTRA_ਸਿਰਾ	\
	(offsetof(ਬਣਤਰ ext2_inode_large, i_checksum_hi) + ਆਕਾਰ(__u16) - \
	 EXT2_GOOD_OLD_INODE_ਮਾਪ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_EPOCH_ਬਿੱਟ 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_EPOCH_ਨਕਾਬ ((1 << EXT4_EPOCH_ਬਿੱਟ) - 1)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_checksum_lo	osd2.linux2.l_i_checksum_lo

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_inode_includes(ਮਾਪ, ਖੇਤਰ)				\
	((ਮਾਪ) >= (ਆਕਾਰ(((ਬਣਤਰ ext2_inode_large *)0)->ਖੇਤਰ) +	\
		    offsetof(ਬਣਤਰ ext2_inode_large, ਖੇਤਰ)))

#ਜੇ defined(__KERNEL__) || defined(__linux__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_reserved1	osd1.linux1.l_i_reserved1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_frag		osd2.linux2.l_i_frag
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_fsize		osd2.linux2.l_i_fsize
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_uid_ਨੀਵਾਂ	i_uid
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_gid_ਨੀਵਾਂ	i_gid
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_uid_ਉੱਚਾ	osd2.linux2.l_i_uid_ਉੱਚਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_gid_ਉੱਚਾ	osd2.linux2.l_i_gid_ਉੱਚਾ
#ਨਹੀਂਤਾਂ
#ਜੇ defined(__GNU__)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_translator	osd1.hurd1.h_i_translator
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_frag		osd2.hurd2.h_i_frag;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_fsize		osd2.hurd2.h_i_fsize;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_uid_ਉੱਚਾ	osd2.hurd2.h_i_uid_ਉੱਚਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_gid_ਉੱਚਾ	osd2.hurd2.h_i_gid_ਉੱਚਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ i_author	osd2.hurd2.h_i_author

#ਜੇ_ਅੰਤ  /* __GNU__ */
#ਜੇ_ਅੰਤ	/* defined(__KERNEL__) || defined(__linux__) */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ inode_uid(inode)	((inode).i_uid | (ਬਿਨਾਨਿਸ਼ਾਨੀ)(inode).osd2.linux2.l_i_uid_ਉੱਚਾ << 16)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ inode_gid(inode)	((inode).i_gid | (ਬਿਨਾਨਿਸ਼ਾਨੀ)(inode).osd2.linux2.l_i_gid_ਉੱਚਾ << 16)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ inode_projid(inode)	((inode).i_projid)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_ਸੈੱਟ_i_uid_ਉੱਚਾ(inode,x) ((inode).osd2.linux2.l_i_uid_ਉੱਚਾ = (x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_ਸੈੱਟ_i_gid_ਉੱਚਾ(inode,x) ((inode).osd2.linux2.l_i_gid_ਉੱਚਾ = (x))

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਬਣਤਰ ext2_inode *EXT2_INODE(ਬਣਤਰ ext2_inode_large *large_inode)
{
	ਵਾਪਸ (ਬਣਤਰ ext2_inode *) large_inode;
}

/*
 * File system states
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_VALID_FS			0x0001	/* Unmounted cleanly */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ERROR_FS			0x0002	/* Errors detected */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_ORPHAN_FS			0x0004	/* Orphans being recovered */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FC_REPLAY			0x0020	/* Ext4 fast commit replay ongoing */

/*
 * Misc. filesystem flags
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAGS_SIGNED_HASH		0x0001  /* Signed dirhash in use */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAGS_UNSIGNED_HASH	0x0002  /* Unsigned dirhash in use */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAGS_TEST_FILESYS		0x0004	/* OK for use on development code */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAGS_IS_SNAPSHOT		0x0010	/* This is a snapshot image */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAGS_FIX_SNAPSHOT		0x0020	/* Snapshot inodes corrupted */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FLAGS_FIX_EXCLUDE		0x0040	/* Exclude bitmaps corrupted */

/*
 * Mount flags
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MOUNT_CHECK		0x0001	/* Do mount-time checks */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MOUNT_GRPID		0x0004	/* Create files with directory's group */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MOUNT_DEBUG		0x0008	/* Some debugging messages */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MOUNT_ERRORS_CONT		0x0010	/* Continue on errors */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MOUNT_ERRORS_RO		0x0020	/* Remount fs ro on errors */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MOUNT_ERRORS_PANIC		0x0040	/* Panic on errors */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MOUNT_MINIX_DF		0x0080	/* Mimics the Minix statfs */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MOUNT_NO_UID32		0x0200  /* Disable 32-bit UIDs */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ clear_opt(o, opt)		o &= ~EXT2_MOUNT_##opt
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_opt(o, opt)			o |= EXT2_MOUNT_##opt
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ test_opt(sb, opt)		(EXT2_SB(sb)->s_mount_opt & \
					 EXT2_MOUNT_##opt)
/*
 * Maximal mount counts between two filesystem checks
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DFL_MAX_MNT_ਗਿਣਤੀ		20	/* Allow 20 mounts */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DFL_CHECKINTERVAL		0	/* Don't use interval check */

/*
 * Behaviour when detecting errors
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ERRORS_CONTINUE		1	/* Continue execution */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ERRORS_RO			2	/* Remount fs read-only */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ERRORS_PANIC		3	/* Panic */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ERRORS_DEFAULT		EXT2_ERRORS_CONTINUE

#ਜੇ (__GNUC__ >= 4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext4_offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext4_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#ਜੇ_ਅੰਤ

/* Metadata checksum algorithms */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ਚੱਕਰੀ_ਜਾਂਚ_32_C_CHKSUM		1

/* Encryption algorithms, key size and key reference len */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ENCRYPTION_ਬਹੁਲਕ_INVALID		0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ENCRYPTION_ਬਹੁਲਕ_AES_256_XTS	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ENCRYPTION_ਬਹੁਲਕ_AES_256_GCM	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ENCRYPTION_ਬਹੁਲਕ_AES_256_CBC	3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ENCRYPTION_ਬਹੁਲਕ_AES_256_CTS	4

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_AES_256_XTS_ਕੁੰਜੀ_ਮਾਪ		64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_AES_256_GCM_ਕੁੰਜੀ_ਮਾਪ		32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_AES_256_CBC_ਕੁੰਜੀ_ਮਾਪ		32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_AES_256_CTS_ਕੁੰਜੀ_ਮਾਪ		32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MAX_ਕੁੰਜੀ_ਮਾਪ			64

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ਕੁੰਜੀ_DESCRIPTOR_ਮਾਪ		8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_CRYPTO_ਖੰਡ_ਮਾਪ			16

/* Password derivation constants */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MAX_PASSPHRASE_ਮਾਪ		1024
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MAX_SALT_ਮਾਪ			256
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_PBKDF2_ITERATIONS			0xFFFF

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_LABEL_LEN				16

/*
 * Policy provided via an ioctl on the topmost directory. This
 * structure is also in the kernel.
 */
ਬਣਤਰ ext4_encryption_policy {
  ਅੱਖਰ ਸੰਸਕਰਣ;
  ਅੱਖਰ contents_encryption_ਬਹੁਲਕ;
  ਅੱਖਰ filenames_encryption_ਬਹੁਲਕ;
  ਅੱਖਰ flags;
  ਅੱਖਰ master_ਕੁੰਜੀ_descriptor[EXT4_ਕੁੰਜੀ_DESCRIPTOR_ਮਾਪ];
} __attribute__((__packed__));

ਬਣਤਰ ext4_encryption_ਕੁੰਜੀ {
        __u32 ਬਹੁਲਕ;
        ਅੱਖਰ raw[EXT4_MAX_ਕੁੰਜੀ_ਮਾਪ];
        __u32 ਮਾਪ;
} __attribute__((__packed__));

/*
 * Structure of the super block
 */
ਬਣਤਰ ext2_super_ਖੰਡ {
/*000*/	__u32	s_inodes_ਗਿਣਤੀ;		/* Inodes count */
	__u32	s_ਖੰਡਾਂ_ਗਿਣਤੀ;		/* Blocks count */
	__u32	s_r_ਖੰਡਾਂ_ਗਿਣਤੀ;	/* Reserved blocks count */
	__u32	s_free_ਖੰਡਾਂ_ਗਿਣਤੀ;	/* Free blocks count */
/*010*/	__u32	s_free_inodes_ਗਿਣਤੀ;	/* Free inodes count */
	__u32	s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;	/* First Data Block */
	__u32	s_log_ਖੰਡ_ਮਾਪ;	/* Block size */
	__u32	s_log_cluster_ਮਾਪ;	/* Allocation cluster size */
/*020*/	__u32	s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;	/* # Blocks per group */
	__u32	s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ;	/* # Fragments per group */
	__u32	s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ;	/* # Inodes per group */
	__u32	s_mtime;		/* Mount time */
/*030*/	__u32	s_wtime;		/* Write time */
	__u16	s_mnt_ਗਿਣਤੀ;		/* Mount count */
	__s16	s_max_mnt_ਗਿਣਤੀ;	/* Maximal mount count */
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
	__u32	s_ਪਹਿਲਾ_ino;		/* First non-reserved inode */
	__u16   s_inode_ਮਾਪ;		/* size of inode structure */
	__u16	s_ਖੰਡ_ਸਮੂਹ_nr;	/* block group # of this superblock */
	__u32	s_feature_compat;	/* compatible feature set */
/*060*/	__u32	s_feature_incompat;	/* incompatible feature set */
	__u32	s_feature_ro_compat;	/* readonly-compatible feature set */
/*068*/	__u8	s_uuid[16] __nonstring;		/* 128-bit uuid for volume */
/*078*/	__u8	s_volume_name[EXT2_LABEL_LEN] __nonstring;	/* volume name, no NUL? */
/*088*/	__u8	s_ਆਖਰੀ_mounted[64] __nonstring;	/* directory last mounted on, no NUL? */
/*0c8*/	__u32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_ਖੰਡਾਂ;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_ਖੰਡਾਂ;	/* Nr to preallocate for dirs */
	__u16	s_reserved_gdt_ਖੰਡਾਂ;	/* Per group table for online growth */
	/*
	 * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
/*0d0*/	__u8	s_journal_uuid[16] __nonstring;	/* uuid of journal superblock */
/*0e0*/	__u32	s_journal_inum;		/* inode number of journal file */
	__u32	s_journal_dev;		/* device number of journal file */
	__u32	s_ਆਖਰੀ_orphan;		/* start of list of inodes to delete */
/*0ec*/	__u32	s_hash_ਬੀਜ[4];		/* HTREE hash seed */
/*0fc*/	__u8	s_def_hash_ਸੰਸਕਰਣ;	/* Default hash version to use */
	__u8	s_jnl_backup_type;	/* Default type of journal backup */
	__u16	s_desc_ਮਾਪ;		/* Group desc. size: INCOMPAT_64BIT */
/*100*/	__u32	s_default_mount_opts;	/* default EXT2_MOUNT_* flags used */
	__u32	s_ਪਹਿਲਾ_meta_bg;	/* First metablock group */
	__u32	s_mkfs_ਸਮਾਂ;		/* When the filesystem was created */
/*10c*/	__u32	s_jnl_ਖੰਡਾਂ[17];	/* Backup of the journal inode */
/*150*/	__u32	s_ਖੰਡਾਂ_ਗਿਣਤੀ_hi;	/* Blocks count high 32bits */
	__u32	s_r_ਖੰਡਾਂ_ਗਿਣਤੀ_hi;	/* Reserved blocks count high 32 bits*/
	__u32	s_free_ਖੰਡਾਂ_hi;	/* Free blocks count */
	__u16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__u16	s_want_extra_isize;	/* New inodes should reserve # bytes */
/*160*/	__u32	s_flags;		/* Miscellaneous flags */
	__u16	s_raid_stride;		/* RAID stride in blocks */
	__u16	s_mmp_update_interval;  /* # seconds to wait in MMP checking */
	__u64	s_mmp_ਖੰਡ;		/* Block for multi-mount protection */
/*170*/	__u32	s_raid_stripe_ਚੌੜਾਈ;	/* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_ਪ੍ਰਤੀ_flex;	/* FLEX_BG group size */
	__u8	s_checksum_type;	/* metadata checksum algorithm */
	__u8	s_encryption_level;	/* versioning level for encryption */
	__u8	s_reserved_pad;		/* Padding to next 32bits */
	__u64	s_kbytes_written;	/* nr of lifetime kilobytes written */
/*180*/	__u32	s_snapshot_inum;	/* Inode number of active snapshot */
	__u32	s_snapshot_id;		/* sequential ID of active snapshot */
	__u64	s_snapshot_r_ਖੰਡਾਂ_ਗਿਣਤੀ; /* active snapshot reserved blocks */
/*190*/	__u32	s_snapshot_list;	/* inode number of disk snapshot list */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_S_ERR_ਸ਼ੁਰੂ ext4_offsetof(ਬਣਤਰ ext2_super_ਖੰਡ, s_error_ਗਿਣਤੀ)
	__u32	s_error_ਗਿਣਤੀ;		/* number of fs errors */
	__u32	s_ਪਹਿਲਾ_error_ਸਮਾਂ;	/* first time an error happened */
	__u32	s_ਪਹਿਲਾ_error_ino;	/* inode involved in first error */
/*1a0*/	__u64	s_ਪਹਿਲਾ_error_ਖੰਡ;	/* block involved in first error */
	__u8	s_ਪਹਿਲਾ_error_func[32] __nonstring;	/* function where error hit, no NUL? */
/*1c8*/	__u32	s_ਪਹਿਲਾ_error_ਸਤਰ;	/* line number where error happened */
	__u32	s_ਆਖਰੀ_error_ਸਮਾਂ;	/* most recent time of an error */
/*1d0*/	__u32	s_ਆਖਰੀ_error_ino;	/* inode involved in last error */
	__u32	s_ਆਖਰੀ_error_ਸਤਰ;	/* line number where error happened */
	__u64	s_ਆਖਰੀ_error_ਖੰਡ;	/* block involved of last error */
/*1e0*/	__u8	s_ਆਖਰੀ_error_func[32] __nonstring;	/* function where error hit, no NUL? */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_S_ERR_ਸਿਰਾ ext4_offsetof(ਬਣਤਰ ext2_super_ਖੰਡ, s_mount_opts)
/*200*/	__u8	s_mount_opts[64] __nonstring;	/* default mount options, no NUL? */
/*240*/	__u32	s_usr_quota_inum;	/* inode number of user quota file */
	__u32	s_grp_quota_inum;	/* inode number of group quota file */
	__u32	s_overhead_clusters;	/* overhead blocks/clusters in fs */
/*24c*/	__u32	s_backup_bgs[2];	/* If sparse_super2 enabled */
/*254*/	__u8	s_encrypt_algos[4];	/* Encryption algorithms in use  */
/*258*/	__u8	s_encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
/*268*/	__le32	s_lpf_ino;		/* Location of the lost+found inode */
	__le32  s_prj_quota_inum;	/* inode for tracking project quota */
/*270*/	__le32	s_checksum_ਬੀਜ;	/* crc32c(orig_uuid) if csum_seed set */
/*274*/	__u8	s_wtime_hi;
	__u8	s_mtime_hi;
	__u8	s_mkfs_ਸਮਾਂ_hi;
	__u8	s_lastcheck_hi;
	__u8	s_ਪਹਿਲਾ_error_ਸਮਾਂ_hi;
	__u8	s_ਆਖਰੀ_error_ਸਮਾਂ_hi;
	__u8	s_ਪਹਿਲਾ_error_errcode;
	__u8    s_ਆਖਰੀ_error_errcode;
/*27c*/ __le16	s_encoding;		/* Filename charset encoding */
	__le16	s_encoding_flags;	/* Filename charset encoding flags */
	__le32  s_orphan_file_inum;	/* Inode for tracking orphan inodes */
	__le32	s_reserved[94];		/* Padding to the end of the block */
/*3fc*/	__u32	s_checksum;		/* crc32c(superblock) */
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_S_ERR_LEN (EXT4_S_ERR_ਸਿਰਾ - EXT4_S_ERR_ਸ਼ੁਰੂ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_LEN_STR(buf) (ਪੂਰਨ)ਆਕਾਰ(buf), (ਅੱਖਰ *)buf

/*
 * Codes for operating systems
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_OS_LINUX		0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_OS_HURD		1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_OBSO_OS_MASIX	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_OS_FREEBSD		3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_OS_LITES		4

/*
 * Revision levels
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_GOOD_OLD_REV	0	/* The good old (original) format */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DYNAMIC_REV	1	/* V2 format w/ dynamic inode sizes */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_GOOD_OLD_INODE_ਮਾਪ 128

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_EXTRA_TIMESTAMP_MAX	(((int64_t)1 << 34) - 1  + INT32_MIN)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_NON_EXTRA_TIMESTAMP_MAX	INT32_MAX
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_TIMESTAMP_MIN		INT32_MIN

/*
 * Journal inode backup types
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_JNL_BACKUP_ਖੰਡਾਂ	1

/*
 * Feature set definitions
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HAS_COMPAT_FEATURE(sb,ਨਕਾਬ)			\
	( EXT2_SB(sb)->s_feature_compat & (ਨਕਾਬ) )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HAS_RO_COMPAT_FEATURE(sb,ਨਕਾਬ)			\
	( EXT2_SB(sb)->s_feature_ro_compat & (ਨਕਾਬ) )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_HAS_INCOMPAT_FEATURE(sb,ਨਕਾਬ)			\
	( EXT2_SB(sb)->s_feature_incompat & (ਨਕਾਬ) )

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_COMPAT_RESIZE_INODE	0x0010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_COMPAT_DIR_ਸੂਚਕਾਂਕ		0x0020
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_COMPAT_LAZY_BG		0x0040
/* #define EXT2_FEATURE_COMPAT_EXCLUDE_INODE	0x0080 not used, legacy */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP	0x0100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_COMPAT_SPARSE_SUPER2	0x0200
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_COMPAT_FAST_COMMIT		0x0400
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_COMPAT_STABLE_INODES	0x0800
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_COMPAT_ORPHAN_FILE		0x1000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004 not used */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_HUGE_FILE	0x0008
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_GDT_CSUM		0x0010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_DIR_NLINK	0x0020
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE	0x0040
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT	0x0080
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_QUOTA		0x0100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_BIGALLOC		0x0200
/*
 * METADATA_CSUM implies GDT_CSUM.  When METADATA_CSUM is set, group
 * descriptor checksums use the same algorithm as all other data
 * structures' checksums.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_METADATA_CSUM	0x0400
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_REPLICA		0x0800
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_READONLY		0x1000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_PROJECT		0x2000 /* Project quota */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_SHARED_ਖੰਡਾਂ	0x4000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_VERITY		0x8000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT	0x10000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 /* Needs recovery */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 /* Journal device */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_INCOMPAT_META_BG		0x0010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_FEATURE_INCOMPAT_EXTENTS		0x0040
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_64_ਦੋਆਧਾਰੀ_ਅੰਕ		0x0080
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_MMP		0x0100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_FLEX_BG		0x0200
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_EA_INODE		0x0400
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_DIRDATA		0x1000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_CSUM_ਬੀਜ		0x2000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_LARGEDIR		0x4000 /* >2GB or 3-lvl htree */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_INLINE_ਅੰਕੜੇ	0x8000 /* data in inode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_ENCRYPT		0x10000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_CASEFOLD		0x20000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_COMPAT_FUNCS(name, ver, flagname) \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ext2fs_has_feature_##name(ਬਣਤਰ ext2_super_ਖੰਡ *sb) \
{ \
	ਵਾਪਸ ((EXT2_SB(sb)->s_feature_compat & \
		 EXT##ver##_FEATURE_COMPAT_##flagname) != 0); \
} \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_ਸੈੱਟ_feature##name(ਬਣਤਰ ext2_super_ਖੰਡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat |= \
		EXT##ver##_FEATURE_COMPAT_##flagname; \
} \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_clear_feature_##name(ਬਣਤਰ ext2_super_ਖੰਡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_compat &= \
		~EXT##ver##_FEATURE_COMPAT_##flagname; \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_RO_COMPAT_FUNCS(name, ver, flagname) \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ext2fs_has_feature_##name(ਬਣਤਰ ext2_super_ਖੰਡ *sb) \
{ \
	ਵਾਪਸ ((EXT2_SB(sb)->s_feature_ro_compat & \
		 EXT##ver##_FEATURE_RO_COMPAT_##flagname) != 0); \
} \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_ਸੈੱਟ_feature##name(ਬਣਤਰ ext2_super_ਖੰਡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat |= \
		EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
} \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_clear_feature_##name(ਬਣਤਰ ext2_super_ਖੰਡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_ro_compat &= \
		~EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_FEATURE_INCOMPAT_FUNCS(name, ver, flagname) \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ext2fs_has_feature_##name(ਬਣਤਰ ext2_super_ਖੰਡ *sb) \
{ \
	ਵਾਪਸ ((EXT2_SB(sb)->s_feature_incompat & \
		 EXT##ver##_FEATURE_INCOMPAT_##flagname) != 0); \
} \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_ਸੈੱਟ_feature##name(ਬਣਤਰ ext2_super_ਖੰਡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat |= \
		EXT##ver##_FEATURE_INCOMPAT_##flagname; \
} \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ext2fs_clear_feature_##name(ਬਣਤਰ ext2_super_ਖੰਡ *sb) \
{ \
	EXT2_SB(sb)->s_feature_incompat &= \
		~EXT##ver##_FEATURE_INCOMPAT_##flagname; \
}

EXT4_FEATURE_COMPAT_FUNCS(dir_prealloc,		2, DIR_PREALLOC)
EXT4_FEATURE_COMPAT_FUNCS(imagic_inodes,	2, IMAGIC_INODES)
EXT4_FEATURE_COMPAT_FUNCS(journal,		3, HAS_JOURNAL)
EXT4_FEATURE_COMPAT_FUNCS(xattr,		2, EXT_ATTR)
EXT4_FEATURE_COMPAT_FUNCS(resize_inode,		2, RESIZE_INODE)
EXT4_FEATURE_COMPAT_FUNCS(dir_ਸੂਚਕਾਂਕ,		2, DIR_ਸੂਚਕਾਂਕ)
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
EXT4_FEATURE_RO_COMPAT_FUNCS(shared_ਖੰਡਾਂ,	4, SHARED_ਖੰਡਾਂ)
EXT4_FEATURE_RO_COMPAT_FUNCS(verity,		4, VERITY)
EXT4_FEATURE_RO_COMPAT_FUNCS(orphan_present,	4, ORPHAN_PRESENT)

EXT4_FEATURE_INCOMPAT_FUNCS(compression,	2, COMPRESSION)
EXT4_FEATURE_INCOMPAT_FUNCS(filetype,		2, FILETYPE)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_needs_recovery,	3, RECOVER)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_dev,	3, JOURNAL_DEV)
EXT4_FEATURE_INCOMPAT_FUNCS(meta_bg,		2, META_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(extents,		3, EXTENTS)
EXT4_FEATURE_INCOMPAT_FUNCS(64ਦੋਆਧਾਰੀ_ਅੰਕ,		4, 64ਦੋਆਧਾਰੀ_ਅੰਕ_2)
EXT4_FEATURE_INCOMPAT_FUNCS(mmp,		4, MMP)
EXT4_FEATURE_INCOMPAT_FUNCS(flex_bg,		4, FLEX_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(ea_inode,		4, EA_INODE)
EXT4_FEATURE_INCOMPAT_FUNCS(dirdata,		4, DIRDATA)
EXT4_FEATURE_INCOMPAT_FUNCS(csum_ਬੀਜ,		4, CSUM_ਬੀਜ)
EXT4_FEATURE_INCOMPAT_FUNCS(largedir,		4, LARGEDIR)
EXT4_FEATURE_INCOMPAT_FUNCS(inline_ਅੰਕੜੇ,	4, INLINE_ਅੰਕੜੇ)
EXT4_FEATURE_INCOMPAT_FUNCS(encrypt,		4, ENCRYPT)
EXT4_FEATURE_INCOMPAT_FUNCS(casefold,		4, CASEFOLD)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_COMPAT_SUPP	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_INCOMPAT_SUPP    (EXT2_FEATURE_INCOMPAT_FILETYPE| \
				       EXT4_FEATURE_INCOMPAT_MMP| \
				       EXT4_FEATURE_INCOMPAT_LARGEDIR| \
				       EXT4_FEATURE_INCOMPAT_EA_INODE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
					 EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \
					 EXT4_FEATURE_RO_COMPAT_DIR_NLINK| \
					 EXT2_FEATURE_RO_COMPAT_BTREE_DIR| \
					 EXT4_FEATURE_RO_COMPAT_VERITY)

/*
 * Default values for user and/or group using reserved blocks
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DEF_RESUID		0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DEF_RESGID		0

/*
 * Default mount options
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DEFM_DEBUG		0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DEFM_BSDGROUPS	0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DEFM_XATTR_USER	0x0004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DEFM_ACL		0x0008
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DEFM_UID16		0x0010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_DEFM_JMODE		0x0060
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_DEFM_JMODE_ਅੰਕੜੇ	0x0020
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_DEFM_JMODE_ORDERED	0x0040
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT3_DEFM_JMODE_WBACK	0x0060
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_DEFM_NOBARRIER	0x0100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_DEFM_ਖੰਡ_VALIDITY 0x0200
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_DEFM_DISCARD	0x0400
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_DEFM_NODELALLOC	0x0800

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ext4_hash_ਵਿੱਚ_dirent(ਅਚਲ ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ (inode->i_flags & EXT4_ENCRYPT_FL) &&
		(inode->i_flags & EXT4_CASEFOLD_FL);
}

/*
 * Structure of a directory entry
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_NAME_LEN 255

ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u16	name_len;		/* Name length */
	ਅੱਖਰ	name[EXT2_NAME_LEN];	/* File name */
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
ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_2 {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u8	name_len;		/* Name length */
	__u8	file_type;
	ਅੱਖਰ	name[EXT2_NAME_LEN];	/* File name */
};

/*
 * Hashes for ext4_dir_entry for casefolded and ecrypted directories.
 * This is located at the first 4 bit aligned location after the name.
 */

ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_hash {
	__le32 hash;
	__le32 minor_hash;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIRENT_HASHES(ਇੰਦਰਾਜ) \
	((ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_hash *) &ਇੰਦਰਾਜ->name[\
		(ext2fs_dirent_name_len(ਇੰਦਰਾਜ) + \
			EXT2_DIR_ਗੇੜਾ) & ~EXT2_DIR_ਗੇੜਾ])
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIRENT_HASH(ਇੰਦਰਾਜ) \
		ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(EXT2_DIRENT_HASHES(ਇੰਦਰਾਜ)->hash)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIRENT_MINOR_HASH(ਇੰਦਰਾਜ) \
		ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(EXT2_DIRENT_HASHES(ਇੰਦਰਾਜ)->minor_hash)

/*
 * This is a bogus directory entry at the end of each leaf block that
 * records checksums.
 */
ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ {
	__u32	det_reserved_zero1;	/* Pretend to be unused */
	__u16	det_rec_len;		/* 12 */
	__u16	det_reserved_name_len;	/* 0xDE00, fake namelen/filetype */
	__u32	det_checksum;		/* crc32c(uuid+inode+dirent) */
};

/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_UNKNOWN		0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_REG_FILE	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_DIR		2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_CHRDEV		3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_BLKDEV		4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_FIFO		5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_SOCK		6
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_SYMLINK		7

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_MAX		8

/*
 * Annoyingly, e2fsprogs always swab16s ext2_dir_entry.name_len, so we
 * have to build ext2_dir_entry_tail with that assumption too.  This
 * constant helps to build the dir_entry_tail to look like it has an
 * "invalid" file type.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIR_NAME_LEN_CSUM	0xDE00

/*
 * EXT2_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIR_ਇੰਦਰਾਜ_ਸਿਰਲੇਖ_LEN	8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIR_ਇੰਦਰਾਜ_HASH_LEN		8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIR_PAD			4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIR_ਗੇੜਾ			(EXT2_DIR_PAD - 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DIR_REC_LEN(name_len) ext2fs_dir_rec_len(name_len, 0)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ext2fs_dir_rec_len(__u8 name_len,
						ਪੂਰਨ extended)
{
	ਪੂਰਨ rec_len = (name_len + EXT2_DIR_ਇੰਦਰਾਜ_ਸਿਰਲੇਖ_LEN + EXT2_DIR_ਗੇੜਾ);

	rec_len &= ~EXT2_DIR_ਗੇੜਾ;
	ਜੇ (extended)
		rec_len += EXT2_DIR_ਇੰਦਰਾਜ_HASH_LEN;
	ਵਾਪਸ rec_len;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ORPHAN_ਖੰਡ_MAGIC 0x0b10ca04

/* Structure at the tail of orphan block */
ਬਣਤਰ ext4_orphan_ਖੰਡ_ਪੂਛ {
	__le32 ob_magic;
	__le32 ob_checksum;
};

/*
 * Constants for ext4's extended time encoding
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_EPOCH_ਬਿੱਟ 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_EPOCH_ਨਕਾਬ ((1 << EXT4_EPOCH_ਬਿੱਟ) - 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_NSEC_ਨਕਾਬ  (~0UL << EXT4_EPOCH_ਬਿੱਟ)

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MMP_MAGIC     0x004D4D50U /* ASCII for MMP */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MMP_SEQ_CLEAN 0xFF4D4D50U /* mmp_seq value for clean unmount */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MMP_SEQ_FSCK  0xE24D4D50U /* mmp_seq value when being fscked */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MMP_SEQ_MAX   0xE24D4D4FU /* maximum valid mmp_seq value */

/* Not endian-annotated; it's swapped at read/write time */
ਬਣਤਰ mmp_struct {
	__u32	mmp_magic;		/* Magic number for MMP */
	__u32	mmp_seq;		/* Sequence no. updated periodically */
	__u64	mmp_ਸਮਾਂ;		/* Time last updated (seconds) */
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MMP_UPDATE_INTERVAL	5

/*
 * Maximum interval for MMP update in seconds.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MMP_MAX_UPDATE_INTERVAL	300

/*
 * Minimum interval for MMP checking in seconds.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MMP_MIN_CHECK_INTERVAL     5

/*
 * Minimum size of inline data.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ	((ਆਕਾਰ(__u32) * EXT2_N_ਖੰਡਾਂ))

/*
 * Size of a parent inode in inline data directory.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_INLINE_ਅੰਕੜੇ_DOTDOT_ਮਾਪ	(4)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ENC_UTF8_12_1	1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ENC_STRICT_ਬਹੁਲਕ_FL			(1 << 0) /* Reject invalid sequences */

#ਜੇ_ਅੰਤ	/* _LINUX_EXT2_FS_H */
