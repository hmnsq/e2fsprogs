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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _LINUX_JBD_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _LINUX_JBD_H

#ਅਨ੍ਤਰ੍ਭਾਵ "jfs_compat.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JFS_DEBUG
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ jfs_debug jbd_debug

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __GNUC__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __FUNCTION__ ""
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ journal_oom_retry 0

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_JBD_DEBUG
/*
 * Define JBD_EXPENSIVE_CHECKING to enable more expensive internal
 * consistency checks.  By default we don't do this unless
 * CONFIG_JBD_DEBUG is on.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD_EXPENSIVE_CHECKING
ਬਾਹਰੀ ਪੂਰਨ journal_enable_debug;
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ journal_enable_debug (-1)
#ਜੇ_ਅੰਤ /* !CONFIG_JBD_DEBUG */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __STDC__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ jbd_debug(n, f, a...)						\
	ਕਰੋ {								\
		ਜੇ ((n) <= journal_enable_debug) {			\
			printk (KERN_DEBUG "(%s, %d): %s: ",		\
				__FILE__, __LINE__, __FUNCTION__);	\
		  	printk (f, ## a);				\
		}							\
	} ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ jbd_debug(x)		/* AIX doesn't do STDC */
#ਜੇ_ਅੰਤ /* !__STDC__ */

ਬਾਹਰੀ ਖਾਲੀ * __jbd_kmalloc (ਅੱਖਰ *where, size_t ਮਾਪ, ਪੂਰਨ flags, ਪੂਰਨ retry);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ jbd_kmalloc(ਮਾਪ, flags) \
	__jbd_kmalloc(__FUNCTION__, (ਮਾਪ), (flags), journal_oom_retry)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ jbd_rep_kmalloc(ਮਾਪ, flags) \
	__jbd_kmalloc(__FUNCTION__, (ਮਾਪ), (flags), 1)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_MIN_JOURNAL_ਖੰਡਾਂ 1024
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_MAX_JOURNAL_ਖੰਡਾਂ (JBD2_MIN_JOURNAL_ਖੰਡਾਂ * 10000)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_DEFAULT_FAST_COMMIT_ਖੰਡਾਂ 256

/*
 * Internal structures used by the logging mechanism:
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_MAGIC_ਸੰਖਿਆ 0xc03b3998U /* The first 4 bytes of /dev/random! */

/*
 * On-disk structures
 */

/*
 * Descriptor block types:
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_DESCRIPTOR_ਖੰਡ	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_COMMIT_ਖੰਡ	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_SUPERBLOCK_V1	3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_SUPERBLOCK_V2	4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_REVOKE_ਖੰਡ	5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FC_ਖੰਡ		6

/*
 * Standard header for all descriptor blocks:
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ journal_ਸਿਰਲੇਖ_s
{
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
} journal_ਸਿਰਲੇਖ_t;

/*
 * Checksum types.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_ਚੱਕਰੀ_ਜਾਂਚ_32_CHKSUM   1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_MD5_CHKSUM     2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_SHA1_CHKSUM    3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_ਚੱਕਰੀ_ਜਾਂਚ_32_C_CHKSUM  4

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_ਚੱਕਰੀ_ਜਾਂਚ_32_CHKSUM_ਮਾਪ 4

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_CHECKSUM_ਬਾਈਟਾਂ (32 / ਆਕਾਰ(__u32))
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
ਬਣਤਰ commit_ਸਿਰਲੇਖ {
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	h_chksum_type;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	h_chksum_ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	h_padding[2];
	__be32		h_chksum[JBD2_CHECKSUM_ਬਾਈਟਾਂ];
	__be64		h_commit_sec;
	__be32		h_commit_nsec;
};

/*
 * The block tag: used to describe a single buffer in the journal
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ journal_ਖੰਡ_tag3_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be32		t_flags;	/* See below */
	__be32		t_blocknr_ਉੱਚਾ; /* most-significant high 32bits. */
	__be32		t_checksum;	/* crc32c(uuid+seq+block) */
} journal_ਖੰਡ_tag3_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ journal_ਖੰਡ_ਟੈਗ_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be16		t_checksum;	/* truncated crc32c(uuid+seq+block) */
	__be16		t_flags;	/* See below */
	__be32		t_blocknr_ਉੱਚਾ; /* most-significant high 32bits. */
} journal_ਖੰਡ_ਟੈਗ_t;

/* Tail of descriptor or revoke block, for checksumming */
ਬਣਤਰ jbd2_journal_ਖੰਡ_ਪੂਛ {
	__be32		t_checksum;
};

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ journal_revoke_ਸਿਰਲੇਖ_s
{
	journal_ਸਿਰਲੇਖ_t r_ਸਿਰਲੇਖ;
	__be32		 r_ਗਿਣਤੀ;	/* Count of bytes used in the block */
} jbd2_journal_revoke_ਸਿਰਲੇਖ_t;

/* Definitions for the journal tag flags word: */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FLAG_ESCAPE		1	/* on-disk block is escaped */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FLAG_SAME_UUID	2	/* block has same uuid as previous */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FLAG_DELETED	4	/* block deleted by this transaction */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FLAG_ਆਖਰੀ_ਟੈਗ	8	/* last tag in this descriptor block */


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UUID_ਮਾਪ 16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_USERS_MAX 48
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_USERS_ਮਾਪ (UUID_ਮਾਪ * JBD2_USERS_MAX)
/*
 * The journal superblock.  All fields are in big-endian byte order.
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ journal_superblock_s
{
/* 0x0000 */
	journal_ਸਿਰਲੇਖ_t s_ਸਿਰਲੇਖ;

/* 0x000C */
	/* Static information describing the journal */
	__be32	s_blocksize;		/* journal device blocksize */
	__be32	s_maxlen;		/* total blocks in journal file */
	__be32	s_ਪਹਿਲਾ;		/* first block of log information */

/* 0x0018 */
	/* Dynamic information describing the current state of the log */
	__be32	s_sequence;		/* first commit ID expected in log */
	__be32	s_ਸ਼ੁਰੂ;		/* blocknr of start of log */

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
	__be32	s_max_trans_ਅੰਕੜੇ;	/* Limit of data blocks per trans. */

/* 0x0050 */
	__u8	s_checksum_type;	/* checksum type */
	__u8	s_padding2[3];
/* 0x0054 */
	__be32	s_ਅੰਕ_fc_blks;		/* Number of fast commit blocks */
/* 0x0058 */
	__be32	s_padding[41];
	__be32	s_checksum;		/* crc32c(superblock) */

/* 0x0100 */
	__u8	s_users[JBD2_USERS_ਮਾਪ];		/* ids of all fs'es sharing the log */

/* 0x0400 */
} journal_superblock_t;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_HAS_COMPAT_FEATURE(j,ਨਕਾਬ)					\
	((j)->j_format_ਸੰਸਕਰਣ >= 2 &&					\
	 ((j)->j_superblock->s_feature_compat & ext2fs_cpu_ਮੰਜ਼ਿਲ_be32((ਨਕਾਬ))))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_HAS_RO_COMPAT_FEATURE(j,ਨਕਾਬ)				\
	((j)->j_format_ਸੰਸਕਰਣ >= 2 &&					\
	 ((j)->j_superblock->s_feature_ro_compat & ext2fs_cpu_ਮੰਜ਼ਿਲ_be32((ਨਕਾਬ))))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_HAS_INCOMPAT_FEATURE(j,ਨਕਾਬ)				\
	((j)->j_format_ਸੰਸਕਰਣ >= 2 &&					\
	 ((j)->j_superblock->s_feature_incompat & ext2fs_cpu_ਮੰਜ਼ਿਲ_be32((ਨਕਾਬ))))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_COMPAT_CHECKSUM		0x00000001

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_INCOMPAT_REVOKE		0x00000001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_INCOMPAT_64_ਦੋਆਧਾਰੀ_ਅੰਕ		0x00000002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT	0x00000004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_INCOMPAT_CSUM_V2		0x00000008
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_INCOMPAT_CSUM_V3		0x00000010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_INCOMPAT_FAST_COMMIT	0x00000020

/* Features known to this kernel version: */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_ਗਿਆਤ_COMPAT_FEATURES	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_ਗਿਆਤ_ROCOMPAT_FEATURES	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_ਗਿਆਤ_INCOMPAT_FEATURES	(JBD2_FEATURE_INCOMPAT_REVOKE|\
					 JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT| \
					 JBD2_FEATURE_INCOMPAT_64_ਦੋਆਧਾਰੀ_ਅੰਕ|\
					 JBD2_FEATURE_INCOMPAT_CSUM_V2|	\
					 JBD2_FEATURE_INCOMPAT_CSUM_V3 | \
					 JBD2_FEATURE_INCOMPAT_FAST_COMMIT)

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NO_INLINE_FUNCS
ਬਾਹਰੀ size_t journal_ਟੈਗ_ਬਾਈਟਾਂ(journal_t *journal);
ਬਾਹਰੀ ਪੂਰਨ jbd2_journal_has_csum_v2or3(journal_t *journal);
ਬਾਹਰੀ ਪੂਰਨ jbd2_journal_get_ਅੰਕ_fc_blks(journal_superblock_t *jsb);
ਬਾਹਰੀ ਪੂਰਨ tid_gt(tid_t x, tid_t y) EXT2FS_ATTR((ਅਣਵਰਤਿਆ));
ਬਾਹਰੀ ਪੂਰਨ tid_geq(tid_t x, tid_t y) EXT2FS_ATTR((ਅਣਵਰਤਿਆ));
#ਜੇ_ਅੰਤ

#ਜੇ (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ E2FSCK_INCLUDE_INLINE_FUNCS
#ਜੇ (__STDC_VERSION__ >= 199901L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਬਾਹਰੀ ਸਤਰਅੰਦਰ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ /* !E2FSCK_INCLUDE_INLINE FUNCS */
#ਜੇ (__STDC_VERSION__ >= 199901L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਸਤਰਅੰਦਰ
#ਨਹੀਂਤਾਂ /* not C99 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਬਾਹਰੀ __inline__
#ਨਹੀਂਤਾਂ				/* For Watcom C */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਬਾਹਰੀ ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ /* __GNUC__ */
#ਜੇ_ਅੰਤ /* __STDC_VERSION__ >= 199901L */
#ਜੇ_ਅੰਤ /* INCLUDE_INLINE_FUNCS */

/* journal feature predicate functions */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
_INLINE_ ਪੂਰਨ jbd2_has_feature_##name(journal_t *j); \
_INLINE_ ਪੂਰਨ jbd2_has_feature_##name(journal_t *j) \
{ \
	ਵਾਪਸ ((j)->j_format_ਸੰਸਕਰਣ >= 2 && \
		((j)->j_superblock->s_feature_compat & \
		 ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(JBD2_FEATURE_COMPAT_##flagname)) != 0); \
} \
_INLINE_ ਖਾਲੀ jbd2_ਸੈੱਟ_feature##name(journal_t *j); \
_INLINE_ ਖਾਲੀ jbd2_ਸੈੱਟ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat |= \
		ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(JBD2_FEATURE_COMPAT_##flagname); \
} \
_INLINE_ ਖਾਲੀ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ ਖਾਲੀ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat &= \
		~ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(JBD2_FEATURE_COMPAT_##flagname); \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
_INLINE_ ਪੂਰਨ jbd2_has_feature_##name(journal_t *j);	\
_INLINE_ ਪੂਰਨ jbd2_has_feature_##name(journal_t *j) \
{ \
	ਵਾਪਸ ((j)->j_format_ਸੰਸਕਰਣ >= 2 && \
		((j)->j_superblock->s_feature_ro_compat & \
		 ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(JBD2_FEATURE_RO_COMPAT_##flagname)) != 0); \
} \
_INLINE_ ਖਾਲੀ jbd2_ਸੈੱਟ_feature##name(journal_t *j); \
_INLINE_ ਖਾਲੀ jbd2_ਸੈੱਟ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat |= \
		ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
} \
_INLINE_ ਖਾਲੀ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ ਖਾਲੀ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat &= \
		~ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
_INLINE_ ਪੂਰਨ jbd2_has_feature_##name(journal_t *j); \
_INLINE_ ਪੂਰਨ jbd2_has_feature_##name(journal_t *j) \
{ \
	ਵਾਪਸ ((j)->j_format_ਸੰਸਕਰਣ >= 2 && \
		((j)->j_superblock->s_feature_incompat & \
		 ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(JBD2_FEATURE_INCOMPAT_##flagname)) != 0); \
} \
_INLINE_ ਖਾਲੀ jbd2_ਸੈੱਟ_feature##name(journal_t *j); \
_INLINE_ ਖਾਲੀ jbd2_ਸੈੱਟ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat |= \
		ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
} \
_INLINE_ ਖਾਲੀ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ ਖਾਲੀ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat &= \
		~ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
}

#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
ਬਾਹਰੀ ਪੂਰਨ jbd2_has_feature_##name(journal_t *j); \
ਬਾਹਰੀ ਖਾਲੀ jbd2_ਸੈੱਟ_feature##name(journal_t *j); \
ਬਾਹਰੀ ਖਾਲੀ jbd2_clear_feature_##name(journal_t *j);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
ਬਾਹਰੀ ਪੂਰਨ jbd2_has_feature_##name(journal_t *j); \
ਬਾਹਰੀ ਖਾਲੀ jbd2_ਸੈੱਟ_feature##name(journal_t *j); \
ਬਾਹਰੀ ਖਾਲੀ jbd2_clear_feature_##name(journal_t *j);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
ਬਾਹਰੀ ਪੂਰਨ jbd2_has_feature_##name(journal_t *j); \
ਬਾਹਰੀ ਖਾਲੀ jbd2_ਸੈੱਟ_feature##name(journal_t *j); \
ਬਾਹਰੀ ਖਾਲੀ jbd2_clear_feature_##name(journal_t *j);

#ਜੇ_ਅੰਤ /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

JBD2_FEATURE_COMPAT_FUNCS(checksum,		CHECKSUM)

JBD2_FEATURE_INCOMPAT_FUNCS(revoke,		REVOKE)
JBD2_FEATURE_INCOMPAT_FUNCS(64ਦੋਆਧਾਰੀ_ਅੰਕ,		64ਦੋਆਧਾਰੀ_ਅੰਕ_2)
JBD2_FEATURE_INCOMPAT_FUNCS(async_commit,	ASYNC_COMMIT)
JBD2_FEATURE_INCOMPAT_FUNCS(csum2,		CSUM_V2)
JBD2_FEATURE_INCOMPAT_FUNCS(csum3,		CSUM_V3)
JBD2_FEATURE_INCOMPAT_FUNCS(fast_commit,	FAST_COMMIT)

#ਜੇ (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
/*
 * helper functions to deal with 32 or 64bit block numbers.
 */
_INLINE_ size_t journal_ਟੈਗ_ਬਾਈਟਾਂ(journal_t *journal)
{
	size_t sz;

	ਜੇ (jbd2_has_feature_csum3(journal))
		ਵਾਪਸ ਆਕਾਰ(journal_ਖੰਡ_tag3_t);

	sz = ਆਕਾਰ(journal_ਖੰਡ_ਟੈਗ_t);

	ਜੇ (jbd2_has_feature_csum2(journal))
		sz += ਆਕਾਰ(__u16);

	ਜੇ (jbd2_has_feature_64bit(journal))
		ਵਾਪਸ sz;

	ਵਾਪਸ sz - ਆਕਾਰ(__u32);
}

_INLINE_ ਪੂਰਨ jbd2_journal_has_csum_v2or3(journal_t *journal)
{
	ਜੇ (jbd2_has_feature_csum2(journal) || jbd2_has_feature_csum3(journal))
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}

_INLINE_ ਪੂਰਨ jbd2_journal_get_ਅੰਕ_fc_blks(journal_superblock_t *jsb)
{
	ਪੂਰਨ ਅੰਕ_fc_ਖੰਡਾਂ = be32_ਮੰਜ਼ਿਲ_cpu(jsb->s_ਅੰਕ_fc_blks);

	ਵਾਪਸ ਅੰਕ_fc_ਖੰਡਾਂ ? ਅੰਕ_fc_ਖੰਡਾਂ : JBD2_DEFAULT_FAST_COMMIT_ਖੰਡਾਂ;
}

/* Comparison functions for transaction IDs: perform comparisons using
 * modulo arithmetic so that they work over sequence number wraps. */

_INLINE_ ਪੂਰਨ tid_gt(tid_t x, tid_t y)
{
	ਪੂਰਨ difference = (x - y);
	ਵਾਪਸ (difference > 0);
}

_INLINE_ ਪੂਰਨ tid_geq(tid_t x, tid_t y)
{
	ਪੂਰਨ difference = (x - y);
	ਵਾਪਸ (difference >= 0);
}
#ਜੇ_ਅੰਤ /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ _INLINE_

ਬਾਹਰੀ ਪੂਰਨ journal_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਪੰਨਾ(ਬਣਤਰ inode *inode);

/*
 * Definitions which augment the buffer_head layer
 */

/* journaling buffer types */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BJ_None		0	/* Not journaled */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BJ_Sync_ਅੰਕੜੇ	1	/* Normal data: flush before commit */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BJ_Async_ਅੰਕੜੇ	2	/* writepage data: wait on it before commit */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BJ_Metadata	3	/* Normal journaled metadata */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BJ_Forget	4	/* Buffer superseded by this transaction */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BJ_IO		5	/* Buffer is for temporary IO use */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BJ_Shadow	6	/* Buffer contents being shadowed to the log */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BJ_LogCtl	7	/* Buffer contains log descriptors */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BJ_Reserved	8	/* Buffer is reserved for access by journal */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BJ_Types	9

ਬਾਹਰੀ ਪੂਰਨ jbd_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਪੰਨਾ(ਬਣਤਰ inode *inode);

#ਜੇ_ਅੰਤ	/* _LINUX_JBD_H */
