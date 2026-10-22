/*
 * linux/include/linux/jbd.h
 *
 * Written by Stephen C. Tweedie <sct@redhat.com>
 *
 * Copyright 1998-2000 Red Hat, Inc --- All Rights Reserved
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * Definitions for transaction data structures for the buffer cache
 * filesystem journaling support.
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _LINUX_JBD_H
#ವ್ಯಾಖ್ಯಾನಿಸು _LINUX_JBD_H

#ಅನ್ತರ್ಭಾವ "jfs_compat.h"
#ವ್ಯಾಖ್ಯಾನಿಸು JFS_DEBUG
#ವ್ಯಾಖ್ಯಾನಿಸು jfs_debug jbd_debug

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __GNUC__
#ವ್ಯಾಖ್ಯಾನಿಸು __FUNCTION__ ""
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು journal_oom_retry 0

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_JBD_DEBUG
/*
 * Define JBD_EXPENSIVE_CHECKING to enable more expensive internal
 * consistency checks.  By default we don't do this unless
 * CONFIG_JBD_DEBUG is on.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು JBD_EXPENSIVE_CHECKING
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ journal_enable_debug;
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು journal_enable_debug (-1)
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_JBD_DEBUG */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __STDC__
#ವ್ಯಾಖ್ಯಾನಿಸು jbd_debug(n, f, a...)						\
	ಮಾಡು {								\
		ಆದರೆ ((n) <= journal_enable_debug) {			\
			printk (KERN_DEBUG "(%s, %d): %s: ",		\
				__FILE__, __LINE__, __FUNCTION__);	\
		  	printk (f, ## a);				\
		}							\
	} ಆಗ (0)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು jbd_debug(x)		/* AIX doesn't do STDC */
#ಆದರೆ_ಅಂತ್ಯ /* !__STDC__ */

ಬಾಹ್ಯ ಶೂನ್ಯ * __jbd_kmalloc (ಅಕ್ಷರ *where, size_t ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ retry);
#ವ್ಯಾಖ್ಯಾನಿಸು jbd_kmalloc(ಗಾತ್ರ_2, flags) \
	__jbd_kmalloc(__FUNCTION__, (ಗಾತ್ರ_2), (flags), journal_oom_retry)
#ವ್ಯಾಖ್ಯಾನಿಸು jbd_rep_kmalloc(ಗಾತ್ರ_2, flags) \
	__jbd_kmalloc(__FUNCTION__, (ಗಾತ್ರ_2), (flags), 1)

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_MIN_JOURNAL_ಖಂಡಗಳು 1024
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_MAX_JOURNAL_ಖಂಡಗಳು (JBD2_MIN_JOURNAL_ಖಂಡಗಳು * 10000)
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_DEFAULT_FAST_COMMIT_ಖಂಡಗಳು 256

/*
 * Internal structures used by the logging mechanism:
 */

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_MAGIC_ಸಂಖ್ಯೆ 0xc03b3998U /* The first 4 bytes of /dev/random! */

/*
 * On-disk structures
 */

/*
 * Descriptor block types:
 */

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_DESCRIPTOR_ಖಂಡ	1
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_COMMIT_ಖಂಡ	2
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_SUPERBLOCK_V1	3
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_SUPERBLOCK_V2	4
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_REVOKE_ಖಂಡ	5
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FC_ಖಂಡ		6

/*
 * Standard header for all descriptor blocks:
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ journal_ಶೀರ್ಷಿಕೆ_s
{
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
} journal_ಶೀರ್ಷಿಕೆ_t;

/*
 * Checksum types.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_CHKSUM   1
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_MD5_CHKSUM     2
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_SHA1_CHKSUM    3
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_C_CHKSUM  4

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_CHKSUM_ಗಾತ್ರ 4

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_CHECKSUM_ಬೈಟುಗಳು (32 / ಗಾತ್ರ(__u32))
/*
 * Commit block header for storing transactional checksums:
 *
 * NOTE: If FEATURE_COMPAT_CHECKSUM (checksum v1) is set, the h_chksum*
 * fields are used to store a checksum of the descriptor and data blocks.
 *
 * If FEATURE_INCOMPAT_CSUM_V2 (checksum v2) is set, then the h_chksum
 * field is used to store crc32c(uuid+commit_block).  Each journal metadata
 * block gets its own checksum, and data block checksums are stored in
 * journal_block_tag (in the descriptor).  The other h_chksum* fields are
 * not used.
 *
 * If FEATURE_INCOMPAT_CSUM_V3 is set, the descriptor block uses
 * journal_block_tag3_t to store a full 32-bit checksum.  Everything else
 * is the same as v2.
 *
 * Checksum v1, v2, and v3 are mutually exclusive features.
 */
ರಚನೆ commit_ಶೀರ್ಷಿಕೆ {
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	h_chksum_type;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	h_chksum_ಗಾತ್ರ;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	h_padding[2];
	__be32		h_chksum[JBD2_CHECKSUM_ಬೈಟುಗಳು];
	__be64		h_commit_sec;
	__be32		h_commit_nsec;
};

/*
 * The block tag: used to describe a single buffer in the journal
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ journal_ಖಂಡ_tag3_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be32		t_flags;	/* See below */
	__be32		t_blocknr_ಮೇಲಿನ; /* most-significant high 32bits. */
	__be32		t_checksum;	/* crc32c(uuid+seq+block) */
} journal_ಖಂಡ_tag3_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ journal_ಖಂಡ_ಟ್ಯಾಗ್_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be16		t_checksum;	/* truncated crc32c(uuid+seq+block) */
	__be16		t_flags;	/* See below */
	__be32		t_blocknr_ಮೇಲಿನ; /* most-significant high 32bits. */
} journal_ಖಂಡ_ಟ್ಯಾಗ್_t;

/* Tail of descriptor or revoke block, for checksumming */
ರಚನೆ jbd2_journal_ಖಂಡ_ಬಾಲ {
	__be32		t_checksum;
};

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ journal_revoke_ಶೀರ್ಷಿಕೆ_s
{
	journal_ಶೀರ್ಷಿಕೆ_t r_ಶೀರ್ಷಿಕೆ;
	__be32		 r_ಎಣಿಕೆ;	/* Count of bytes used in the block */
} jbd2_journal_revoke_ಶೀರ್ಷಿಕೆ_t;

/* Definitions for the journal tag flags word: */
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FLAG_ESCAPE		1	/* on-disk block is escaped */
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FLAG_SAME_UUID	2	/* block has same uuid as previous */
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FLAG_DELETED	4	/* block deleted by this transaction */
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FLAG_ಕೊನೆಯ_ಟ್ಯಾಗ್	8	/* last tag in this descriptor block */


#ವ್ಯಾಖ್ಯಾನಿಸು UUID_ಗಾತ್ರ 16
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_USERS_MAX 48
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_USERS_ಗಾತ್ರ (UUID_ಗಾತ್ರ * JBD2_USERS_MAX)
/*
 * The journal superblock.  All fields are in big-endian byte order.
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ journal_superblock_s
{
/* 0x0000 */
	journal_ಶೀರ್ಷಿಕೆ_t s_ಶೀರ್ಷಿಕೆ;

/* 0x000C */
	/* Static information describing the journal */
	__be32	s_blocksize;		/* journal device blocksize */
	__be32	s_maxlen;		/* total blocks in journal file */
	__be32	s_ಮೊದಲ;		/* first block of log information */

/* 0x0018 */
	/* Dynamic information describing the current state of the log */
	__be32	s_sequence;		/* first commit ID expected in log */
	__be32	s_ಆರಂಭ;		/* blocknr of start of log */

/* 0x0020 */
	/* Error value, as set by journal_abort(). */
	__s32	s_errno;

/* 0x0024 */
	/* Remaining fields are only valid in a version-2 superblock */
	__be32	s_feature_compat; 	/* compatible feature set */
	__be32	s_feature_incompat; 	/* incompatible feature set */
	__be32	s_feature_ro_compat; 	/* readonly-compatible feature set */
/* 0x0030 */
	__u8	s_uuid[16];		/* 128-bit uuid for journal */

/* 0x0040 */
	__be32	s_nr_users;		/* Nr of filesystems sharing log */

	__be32	s_dynsuper;		/* Blocknr of dynamic superblock copy*/

/* 0x0048 */
	__be32	s_max_transaction;	/* Limit of journal blocks per trans.*/
	__be32	s_max_trans_ದತ್ತಾಂಶ;	/* Limit of data blocks per trans. */

/* 0x0050 */
	__u8	s_checksum_type;	/* checksum type */
	__u8	s_padding2[3];
/* 0x0054 */
	__be32	s_ಅಂಕೆ_fc_blks;		/* Number of fast commit blocks */
/* 0x0058 */
	__be32	s_padding[41];
	__be32	s_checksum;		/* crc32c(superblock) */

/* 0x0100 */
	__u8	s_users[JBD2_USERS_ಗಾತ್ರ];		/* ids of all fs'es sharing the log */

/* 0x0400 */
} journal_superblock_t;

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_HAS_COMPAT_FEATURE(j,ಮುಖವಾಡ)					\
	((j)->j_format_ಆವೃತ್ತಿ >= 2 &&					\
	 ((j)->j_superblock->s_feature_compat & ext2fs_cpu_ಗಮ್ಯ_be32((ಮುಖವಾಡ))))
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_HAS_RO_COMPAT_FEATURE(j,ಮುಖವಾಡ)				\
	((j)->j_format_ಆವೃತ್ತಿ >= 2 &&					\
	 ((j)->j_superblock->s_feature_ro_compat & ext2fs_cpu_ಗಮ್ಯ_be32((ಮುಖವಾಡ))))
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_HAS_INCOMPAT_FEATURE(j,ಮುಖವಾಡ)				\
	((j)->j_format_ಆವೃತ್ತಿ >= 2 &&					\
	 ((j)->j_superblock->s_feature_incompat & ext2fs_cpu_ಗಮ್ಯ_be32((ಮುಖವಾಡ))))

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_COMPAT_CHECKSUM		0x00000001

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_INCOMPAT_REVOKE		0x00000001
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_INCOMPAT_64_ಬಿಟ್		0x00000002
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT	0x00000004
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_INCOMPAT_CSUM_V2		0x00000008
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_INCOMPAT_CSUM_V3		0x00000010
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_INCOMPAT_FAST_COMMIT	0x00000020

/* Features known to this kernel version: */
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_ತಿಳಿದ_COMPAT_FEATURES	0
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_ತಿಳಿದ_ROCOMPAT_FEATURES	0
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_ತಿಳಿದ_INCOMPAT_FEATURES	(JBD2_FEATURE_INCOMPAT_REVOKE|\
					 JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT| \
					 JBD2_FEATURE_INCOMPAT_64_ಬಿಟ್|\
					 JBD2_FEATURE_INCOMPAT_CSUM_V2|	\
					 JBD2_FEATURE_INCOMPAT_CSUM_V3 | \
					 JBD2_FEATURE_INCOMPAT_FAST_COMMIT)

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NO_INLINE_FUNCS
ಬಾಹ್ಯ size_t journal_ಟ್ಯಾಗ್_ಬೈಟುಗಳು(journal_t *journal);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ jbd2_journal_has_csum_v2or3(journal_t *journal);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ jbd2_journal_get_ಅಂಕೆ_fc_blks(journal_superblock_t *jsb);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ tid_gt(tid_t x, tid_t y) EXT2FS_ATTR((ಬಳಸದ));
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ tid_geq(tid_t x, tid_t y) EXT2FS_ATTR((ಬಳಸದ));
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ E2FSCK_INCLUDE_INLINE_FUNCS
#ಆದರೆ (__STDC_VERSION__ >= 199901L)
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಬಾಹ್ಯ ಸಾಲೊಳಗೆ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಸಾಲೊಳಗೆ
#ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ /* !E2FSCK_INCLUDE_INLINE FUNCS */
#ಆದರೆ (__STDC_VERSION__ >= 199901L)
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಸಾಲೊಳಗೆ
#ಇಲ್ಲದಿದ್ದರೆ /* not C99 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಬಾಹ್ಯ __inline__
#ಇಲ್ಲದಿದ್ದರೆ				/* For Watcom C */
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಬಾಹ್ಯ ಸಾಲೊಳಗೆ
#ಆದರೆ_ಅಂತ್ಯ /* __GNUC__ */
#ಆದರೆ_ಅಂತ್ಯ /* __STDC_VERSION__ >= 199901L */
#ಆದರೆ_ಅಂತ್ಯ /* INCLUDE_INLINE_FUNCS */

/* journal feature predicate functions */
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
_INLINE_ ಪೂರ್ಣಾಂಕ jbd2_has_feature_##name(journal_t *j); \
_INLINE_ ಪೂರ್ಣಾಂಕ jbd2_has_feature_##name(journal_t *j) \
{ \
	ಹಿಂತಿರುಗಿ ((j)->j_format_ಆವೃತ್ತಿ >= 2 && \
		((j)->j_superblock->s_feature_compat & \
		 ext2fs_cpu_ಗಮ್ಯ_be32(JBD2_FEATURE_COMPAT_##flagname)) != 0); \
} \
_INLINE_ ಶೂನ್ಯ jbd2_ಗಣ_feature##name(journal_t *j); \
_INLINE_ ಶೂನ್ಯ jbd2_ಗಣ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat |= \
		ext2fs_cpu_ಗಮ್ಯ_be32(JBD2_FEATURE_COMPAT_##flagname); \
} \
_INLINE_ ಶೂನ್ಯ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ ಶೂನ್ಯ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat &= \
		~ext2fs_cpu_ಗಮ್ಯ_be32(JBD2_FEATURE_COMPAT_##flagname); \
}

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
_INLINE_ ಪೂರ್ಣಾಂಕ jbd2_has_feature_##name(journal_t *j);	\
_INLINE_ ಪೂರ್ಣಾಂಕ jbd2_has_feature_##name(journal_t *j) \
{ \
	ಹಿಂತಿರುಗಿ ((j)->j_format_ಆವೃತ್ತಿ >= 2 && \
		((j)->j_superblock->s_feature_ro_compat & \
		 ext2fs_cpu_ಗಮ್ಯ_be32(JBD2_FEATURE_RO_COMPAT_##flagname)) != 0); \
} \
_INLINE_ ಶೂನ್ಯ jbd2_ಗಣ_feature##name(journal_t *j); \
_INLINE_ ಶೂನ್ಯ jbd2_ಗಣ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat |= \
		ext2fs_cpu_ಗಮ್ಯ_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
} \
_INLINE_ ಶೂನ್ಯ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ ಶೂನ್ಯ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat &= \
		~ext2fs_cpu_ಗಮ್ಯ_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
}

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
_INLINE_ ಪೂರ್ಣಾಂಕ jbd2_has_feature_##name(journal_t *j); \
_INLINE_ ಪೂರ್ಣಾಂಕ jbd2_has_feature_##name(journal_t *j) \
{ \
	ಹಿಂತಿರುಗಿ ((j)->j_format_ಆವೃತ್ತಿ >= 2 && \
		((j)->j_superblock->s_feature_incompat & \
		 ext2fs_cpu_ಗಮ್ಯ_be32(JBD2_FEATURE_INCOMPAT_##flagname)) != 0); \
} \
_INLINE_ ಶೂನ್ಯ jbd2_ಗಣ_feature##name(journal_t *j); \
_INLINE_ ಶೂನ್ಯ jbd2_ಗಣ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat |= \
		ext2fs_cpu_ಗಮ್ಯ_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
} \
_INLINE_ ಶೂನ್ಯ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ ಶೂನ್ಯ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat &= \
		~ext2fs_cpu_ಗಮ್ಯ_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
}

#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ jbd2_has_feature_##name(journal_t *j); \
ಬಾಹ್ಯ ಶೂನ್ಯ jbd2_ಗಣ_feature##name(journal_t *j); \
ಬಾಹ್ಯ ಶೂನ್ಯ jbd2_clear_feature_##name(journal_t *j);

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ jbd2_has_feature_##name(journal_t *j); \
ಬಾಹ್ಯ ಶೂನ್ಯ jbd2_ಗಣ_feature##name(journal_t *j); \
ಬಾಹ್ಯ ಶೂನ್ಯ jbd2_clear_feature_##name(journal_t *j);

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ jbd2_has_feature_##name(journal_t *j); \
ಬಾಹ್ಯ ಶೂನ್ಯ jbd2_ಗಣ_feature##name(journal_t *j); \
ಬಾಹ್ಯ ಶೂನ್ಯ jbd2_clear_feature_##name(journal_t *j);

#ಆದರೆ_ಅಂತ್ಯ /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

JBD2_FEATURE_COMPAT_FUNCS(checksum,		CHECKSUM)

JBD2_FEATURE_INCOMPAT_FUNCS(revoke,		REVOKE)
JBD2_FEATURE_INCOMPAT_FUNCS(64ಬಿಟ್,		64ಬಿಟ್_2)
JBD2_FEATURE_INCOMPAT_FUNCS(async_commit,	ASYNC_COMMIT)
JBD2_FEATURE_INCOMPAT_FUNCS(csum2,		CSUM_V2)
JBD2_FEATURE_INCOMPAT_FUNCS(csum3,		CSUM_V3)
JBD2_FEATURE_INCOMPAT_FUNCS(fast_commit,	FAST_COMMIT)

#ಆದರೆ (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
/*
 * helper functions to deal with 32 or 64bit block numbers.
 */
_INLINE_ size_t journal_ಟ್ಯಾಗ್_ಬೈಟುಗಳು(journal_t *journal)
{
	size_t sz;

	ಆದರೆ (jbd2_has_feature_csum3(journal))
		ಹಿಂತಿರುಗಿ ಗಾತ್ರ(journal_ಖಂಡ_tag3_t);

	sz = ಗಾತ್ರ(journal_ಖಂಡ_ಟ್ಯಾಗ್_t);

	ಆದರೆ (jbd2_has_feature_csum2(journal))
		sz += ಗಾತ್ರ(__u16);

	ಆದರೆ (jbd2_has_feature_64bit(journal))
		ಹಿಂತಿರುಗಿ sz;

	ಹಿಂತಿರುಗಿ sz - ಗಾತ್ರ(__u32);
}

_INLINE_ ಪೂರ್ಣಾಂಕ jbd2_journal_has_csum_v2or3(journal_t *journal)
{
	ಆದರೆ (jbd2_has_feature_csum2(journal) || jbd2_has_feature_csum3(journal))
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}

_INLINE_ ಪೂರ್ಣಾಂಕ jbd2_journal_get_ಅಂಕೆ_fc_blks(journal_superblock_t *jsb)
{
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ_fc_ಖಂಡಗಳು = be32_ಗಮ್ಯ_cpu(jsb->s_ಅಂಕೆ_fc_blks);

	ಹಿಂತಿರುಗಿ ಅಂಕೆ_fc_ಖಂಡಗಳು ? ಅಂಕೆ_fc_ಖಂಡಗಳು : JBD2_DEFAULT_FAST_COMMIT_ಖಂಡಗಳು;
}

/* Comparison functions for transaction IDs: perform comparisons using
 * modulo arithmetic so that they work over sequence number wraps. */

_INLINE_ ಪೂರ್ಣಾಂಕ tid_gt(tid_t x, tid_t y)
{
	ಪೂರ್ಣಾಂಕ difference = (x - y);
	ಹಿಂತಿರುಗಿ (difference > 0);
}

_INLINE_ ಪೂರ್ಣಾಂಕ tid_geq(tid_t x, tid_t y)
{
	ಪೂರ್ಣಾಂಕ difference = (x - y);
	ಹಿಂತಿರುಗಿ (difference >= 0);
}
#ಆದರೆ_ಅಂತ್ಯ /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು _INLINE_

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ journal_ಖಂಡಗಳು_ಪ್ರತಿ_ಪುಟ(ರಚನೆ inode *inode);

/*
 * Definitions which augment the buffer_head layer
 */

/* journaling buffer types */
#ವ್ಯಾಖ್ಯಾನಿಸು BJ_None		0	/* Not journaled */
#ವ್ಯಾಖ್ಯಾನಿಸು BJ_Sync_ದತ್ತಾಂಶ	1	/* Normal data: flush before commit */
#ವ್ಯಾಖ್ಯಾನಿಸು BJ_Async_ದತ್ತಾಂಶ	2	/* writepage data: wait on it before commit */
#ವ್ಯಾಖ್ಯಾನಿಸು BJ_Metadata	3	/* Normal journaled metadata */
#ವ್ಯಾಖ್ಯಾನಿಸು BJ_Forget	4	/* Buffer superseded by this transaction */
#ವ್ಯಾಖ್ಯಾನಿಸು BJ_IO		5	/* Buffer is for temporary IO use */
#ವ್ಯಾಖ್ಯಾನಿಸು BJ_Shadow	6	/* Buffer contents being shadowed to the log */
#ವ್ಯಾಖ್ಯಾನಿಸು BJ_LogCtl	7	/* Buffer contains log descriptors */
#ವ್ಯಾಖ್ಯಾನಿಸು BJ_Reserved	8	/* Buffer is reserved for access by journal */
#ವ್ಯಾಖ್ಯಾನಿಸು BJ_Types	9

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ jbd_ಖಂಡಗಳು_ಪ್ರತಿ_ಪುಟ(ರಚನೆ inode *inode);

#ಆದರೆ_ಅಂತ್ಯ	/* _LINUX_JBD_H */
