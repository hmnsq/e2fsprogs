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

#اگر_غیر_تعریف_شدہ _LINUX_JBD_H
#تعریف_کرو _LINUX_JBD_H

#شامل "jfs_compat.h"
#تعریف_کرو JFS_DEBUG
#تعریف_کرو jfs_debug jbd_debug

#اگر_غیر_تعریف_شدہ __GNUC__
#تعریف_کرو __FUNCTION__ ""
#اگر_ختم

#تعریف_کرو journal_oom_retry 0

#اگر_تعریف_شدہ CONFIG_JBD_DEBUG
/*
 * Define JBD_EXPENSIVE_CHECKING to enable more expensive internal
 * consistency checks.  By default we don't do this unless
 * CONFIG_JBD_DEBUG is on.
 */
#تعریف_کرو JBD_EXPENSIVE_CHECKING
خارجی صحیح journal_enable_debug;
#ورنہ
#تعریف_کرو journal_enable_debug (-1)
#اگر_ختم /* !CONFIG_JBD_DEBUG */

#اگر_تعریف_شدہ __STDC__
#تعریف_کرو jbd_debug(n, f, a...)						\
	کرو {								\
		اگر ((n) <= journal_enable_debug) {			\
			printk (KERN_DEBUG "(%s, %d): %s: ",		\
				__FILE__, __LINE__, __FUNCTION__);	\
		  	printk (f, ## a);				\
		}							\
	} جب (0)
#ورنہ
#تعریف_کرو jbd_debug(x)		/* AIX doesn't do STDC */
#اگر_ختم /* !__STDC__ */

خارجی خالی * __jbd_kmalloc (حرف *where, size_t سائز, صحیح flags, صحیح retry);
#تعریف_کرو jbd_kmalloc(سائز, flags) \
	__jbd_kmalloc(__FUNCTION__, (سائز), (flags), journal_oom_retry)
#تعریف_کرو jbd_rep_kmalloc(سائز, flags) \
	__jbd_kmalloc(__FUNCTION__, (سائز), (flags), 1)

#تعریف_کرو JBD2_MIN_JOURNAL_بلاکیں 1024
#تعریف_کرو JBD2_MAX_JOURNAL_بلاکیں (JBD2_MIN_JOURNAL_بلاکیں * 10000)
#تعریف_کرو JBD2_DEFAULT_FAST_COMMIT_بلاکیں 256

/*
 * Internal structures used by the logging mechanism:
 */

#تعریف_کرو JBD2_MAGIC_نمبر 0xc03b3998U /* The first 4 bytes of /dev/random! */

/*
 * On-disk structures
 */

/*
 * Descriptor block types:
 */

#تعریف_کرو JBD2_DESCRIPTOR_بلاک	1
#تعریف_کرو JBD2_COMMIT_بلاک	2
#تعریف_کرو JBD2_SUPERBLOCK_V1	3
#تعریف_کرو JBD2_SUPERBLOCK_V2	4
#تعریف_کرو JBD2_REVOKE_بلاک	5
#تعریف_کرو JBD2_FC_بلاک		6

/*
 * Standard header for all descriptor blocks:
 */
قسمتعریف ساخت journal_سرخی_s
{
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
} journal_سرخی_t;

/*
 * Checksum types.
 */
#تعریف_کرو JBD2_چکری_جانچ_32_CHKSUM   1
#تعریف_کرو JBD2_MD5_CHKSUM     2
#تعریف_کرو JBD2_SHA1_CHKSUM    3
#تعریف_کرو JBD2_چکری_جانچ_32_C_CHKSUM  4

#تعریف_کرو JBD2_چکری_جانچ_32_CHKSUM_سائز 4

#تعریف_کرو JBD2_CHECKSUM_بائٹیں (32 / حجم(__u32))
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
ساخت commit_سرخی {
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
	غیرعلامتی حرف	h_chksum_type;
	غیرعلامتی حرف	h_chksum_سائز;
	غیرعلامتی حرف	h_padding[2];
	__be32		h_chksum[JBD2_CHECKSUM_بائٹیں];
	__be64		h_commit_sec;
	__be32		h_commit_nsec;
};

/*
 * The block tag: used to describe a single buffer in the journal
 */
قسمتعریف ساخت journal_بلاک_tag3_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be32		t_flags;	/* See below */
	__be32		t_blocknr_بلند; /* most-significant high 32bits. */
	__be32		t_checksum;	/* crc32c(uuid+seq+block) */
} journal_بلاک_tag3_t;

قسمتعریف ساخت journal_بلاک_لیبل_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be16		t_checksum;	/* truncated crc32c(uuid+seq+block) */
	__be16		t_flags;	/* See below */
	__be32		t_blocknr_بلند; /* most-significant high 32bits. */
} journal_بلاک_لیبل_t;

/* Tail of descriptor or revoke block, for checksumming */
ساخت jbd2_journal_بلاک_دم {
	__be32		t_checksum;
};

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
قسمتعریف ساخت journal_revoke_سرخی_s
{
	journal_سرخی_t r_سرخی;
	__be32		 r_گنتی;	/* Count of bytes used in the block */
} jbd2_journal_revoke_سرخی_t;

/* Definitions for the journal tag flags word: */
#تعریف_کرو JBD2_FLAG_ESCAPE		1	/* on-disk block is escaped */
#تعریف_کرو JBD2_FLAG_SAME_UUID	2	/* block has same uuid as previous */
#تعریف_کرو JBD2_FLAG_DELETED	4	/* block deleted by this transaction */
#تعریف_کرو JBD2_FLAG_آخری_لیبل	8	/* last tag in this descriptor block */


#تعریف_کرو UUID_سائز 16
#تعریف_کرو JBD2_USERS_MAX 48
#تعریف_کرو JBD2_USERS_سائز (UUID_سائز * JBD2_USERS_MAX)
/*
 * The journal superblock.  All fields are in big-endian byte order.
 */
قسمتعریف ساخت journal_superblock_s
{
/* 0x0000 */
	journal_سرخی_t s_سرخی;

/* 0x000C */
	/* Static information describing the journal */
	__be32	s_blocksize;		/* journal device blocksize */
	__be32	s_maxlen;		/* total blocks in journal file */
	__be32	s_پہلا;		/* first block of log information */

/* 0x0018 */
	/* Dynamic information describing the current state of the log */
	__be32	s_sequence;		/* first commit ID expected in log */
	__be32	s_آغاز;		/* blocknr of start of log */

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
	__be32	s_max_trans_ڈیٹا;	/* Limit of data blocks per trans. */

/* 0x0050 */
	__u8	s_checksum_type;	/* checksum type */
	__u8	s_padding2[3];
/* 0x0054 */
	__be32	s_عدد_fc_blks;		/* Number of fast commit blocks */
/* 0x0058 */
	__be32	s_padding[41];
	__be32	s_checksum;		/* crc32c(superblock) */

/* 0x0100 */
	__u8	s_users[JBD2_USERS_سائز];		/* ids of all fs'es sharing the log */

/* 0x0400 */
} journal_superblock_t;

#تعریف_کرو JBD2_HAS_COMPAT_FEATURE(j,نقاب)					\
	((j)->j_format_نسخہ >= 2 &&					\
	 ((j)->j_superblock->s_feature_compat & ext2fs_cpu_مقصد_be32((نقاب))))
#تعریف_کرو JBD2_HAS_RO_COMPAT_FEATURE(j,نقاب)				\
	((j)->j_format_نسخہ >= 2 &&					\
	 ((j)->j_superblock->s_feature_ro_compat & ext2fs_cpu_مقصد_be32((نقاب))))
#تعریف_کرو JBD2_HAS_INCOMPAT_FEATURE(j,نقاب)				\
	((j)->j_format_نسخہ >= 2 &&					\
	 ((j)->j_superblock->s_feature_incompat & ext2fs_cpu_مقصد_be32((نقاب))))

#تعریف_کرو JBD2_FEATURE_COMPAT_CHECKSUM		0x00000001

#تعریف_کرو JBD2_FEATURE_INCOMPAT_REVOKE		0x00000001
#تعریف_کرو JBD2_FEATURE_INCOMPAT_64_ثنائی_ہندسہ		0x00000002
#تعریف_کرو JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT	0x00000004
#تعریف_کرو JBD2_FEATURE_INCOMPAT_CSUM_V2		0x00000008
#تعریف_کرو JBD2_FEATURE_INCOMPAT_CSUM_V3		0x00000010
#تعریف_کرو JBD2_FEATURE_INCOMPAT_FAST_COMMIT	0x00000020

/* Features known to this kernel version: */
#تعریف_کرو JBD2_معلوم_COMPAT_FEATURES	0
#تعریف_کرو JBD2_معلوم_ROCOMPAT_FEATURES	0
#تعریف_کرو JBD2_معلوم_INCOMPAT_FEATURES	(JBD2_FEATURE_INCOMPAT_REVOKE|\
					 JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT| \
					 JBD2_FEATURE_INCOMPAT_64_ثنائی_ہندسہ|\
					 JBD2_FEATURE_INCOMPAT_CSUM_V2|	\
					 JBD2_FEATURE_INCOMPAT_CSUM_V3 | \
					 JBD2_FEATURE_INCOMPAT_FAST_COMMIT)

#اگر_تعریف_شدہ NO_INLINE_FUNCS
خارجی size_t journal_لیبل_بائٹیں(journal_t *journal);
خارجی صحیح jbd2_journal_has_csum_v2or3(journal_t *journal);
خارجی صحیح jbd2_journal_get_عدد_fc_blks(journal_superblock_t *jsb);
خارجی صحیح tid_gt(tid_t x, tid_t y) EXT2FS_ATTR((غیر_مستعمل));
خارجی صحیح tid_geq(tid_t x, tid_t y) EXT2FS_ATTR((غیر_مستعمل));
#اگر_ختم

#اگر (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#اگر_تعریف_شدہ E2FSCK_INCLUDE_INLINE_FUNCS
#اگر (__STDC_VERSION__ >= 199901L)
#تعریف_کرو _INLINE_ خارجی اندرونی
#ورنہ
#تعریف_کرو _INLINE_ اندرونی
#اگر_ختم
#ورنہ /* !E2FSCK_INCLUDE_INLINE FUNCS */
#اگر (__STDC_VERSION__ >= 199901L)
#تعریف_کرو _INLINE_ اندرونی
#ورنہ /* not C99 */
#اگر_تعریف_شدہ __GNUC__
#تعریف_کرو _INLINE_ خارجی __inline__
#ورنہ				/* For Watcom C */
#تعریف_کرو _INLINE_ خارجی اندرونی
#اگر_ختم /* __GNUC__ */
#اگر_ختم /* __STDC_VERSION__ >= 199901L */
#اگر_ختم /* INCLUDE_INLINE_FUNCS */

/* journal feature predicate functions */
#تعریف_کرو JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
_INLINE_ صحیح jbd2_has_feature_##name(journal_t *j); \
_INLINE_ صحیح jbd2_has_feature_##name(journal_t *j) \
{ \
	واپس ((j)->j_format_نسخہ >= 2 && \
		((j)->j_superblock->s_feature_compat & \
		 ext2fs_cpu_مقصد_be32(JBD2_FEATURE_COMPAT_##flagname)) != 0); \
} \
_INLINE_ خالی jbd2_سیٹ_feature##name(journal_t *j); \
_INLINE_ خالی jbd2_سیٹ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat |= \
		ext2fs_cpu_مقصد_be32(JBD2_FEATURE_COMPAT_##flagname); \
} \
_INLINE_ خالی jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ خالی jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat &= \
		~ext2fs_cpu_مقصد_be32(JBD2_FEATURE_COMPAT_##flagname); \
}

#تعریف_کرو JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
_INLINE_ صحیح jbd2_has_feature_##name(journal_t *j);	\
_INLINE_ صحیح jbd2_has_feature_##name(journal_t *j) \
{ \
	واپس ((j)->j_format_نسخہ >= 2 && \
		((j)->j_superblock->s_feature_ro_compat & \
		 ext2fs_cpu_مقصد_be32(JBD2_FEATURE_RO_COMPAT_##flagname)) != 0); \
} \
_INLINE_ خالی jbd2_سیٹ_feature##name(journal_t *j); \
_INLINE_ خالی jbd2_سیٹ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat |= \
		ext2fs_cpu_مقصد_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
} \
_INLINE_ خالی jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ خالی jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat &= \
		~ext2fs_cpu_مقصد_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
}

#تعریف_کرو JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
_INLINE_ صحیح jbd2_has_feature_##name(journal_t *j); \
_INLINE_ صحیح jbd2_has_feature_##name(journal_t *j) \
{ \
	واپس ((j)->j_format_نسخہ >= 2 && \
		((j)->j_superblock->s_feature_incompat & \
		 ext2fs_cpu_مقصد_be32(JBD2_FEATURE_INCOMPAT_##flagname)) != 0); \
} \
_INLINE_ خالی jbd2_سیٹ_feature##name(journal_t *j); \
_INLINE_ خالی jbd2_سیٹ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat |= \
		ext2fs_cpu_مقصد_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
} \
_INLINE_ خالی jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ خالی jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat &= \
		~ext2fs_cpu_مقصد_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
}

#ورنہ
#تعریف_کرو JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
خارجی صحیح jbd2_has_feature_##name(journal_t *j); \
خارجی خالی jbd2_سیٹ_feature##name(journal_t *j); \
خارجی خالی jbd2_clear_feature_##name(journal_t *j);

#تعریف_کرو JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
خارجی صحیح jbd2_has_feature_##name(journal_t *j); \
خارجی خالی jbd2_سیٹ_feature##name(journal_t *j); \
خارجی خالی jbd2_clear_feature_##name(journal_t *j);

#تعریف_کرو JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
خارجی صحیح jbd2_has_feature_##name(journal_t *j); \
خارجی خالی jbd2_سیٹ_feature##name(journal_t *j); \
خارجی خالی jbd2_clear_feature_##name(journal_t *j);

#اگر_ختم /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

JBD2_FEATURE_COMPAT_FUNCS(checksum,		CHECKSUM)

JBD2_FEATURE_INCOMPAT_FUNCS(revoke,		REVOKE)
JBD2_FEATURE_INCOMPAT_FUNCS(64ثنائی_ہندسہ,		64ثنائی_ہندسہ_2)
JBD2_FEATURE_INCOMPAT_FUNCS(async_commit,	ASYNC_COMMIT)
JBD2_FEATURE_INCOMPAT_FUNCS(csum2,		CSUM_V2)
JBD2_FEATURE_INCOMPAT_FUNCS(csum3,		CSUM_V3)
JBD2_FEATURE_INCOMPAT_FUNCS(fast_commit,	FAST_COMMIT)

#اگر (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
/*
 * helper functions to deal with 32 or 64bit block numbers.
 */
_INLINE_ size_t journal_لیبل_بائٹیں(journal_t *journal)
{
	size_t sz;

	اگر (jbd2_has_feature_csum3(journal))
		واپس حجم(journal_بلاک_tag3_t);

	sz = حجم(journal_بلاک_لیبل_t);

	اگر (jbd2_has_feature_csum2(journal))
		sz += حجم(__u16);

	اگر (jbd2_has_feature_64bit(journal))
		واپس sz;

	واپس sz - حجم(__u32);
}

_INLINE_ صحیح jbd2_journal_has_csum_v2or3(journal_t *journal)
{
	اگر (jbd2_has_feature_csum2(journal) || jbd2_has_feature_csum3(journal))
		واپس 1;

	واپس 0;
}

_INLINE_ صحیح jbd2_journal_get_عدد_fc_blks(journal_superblock_t *jsb)
{
	صحیح عدد_fc_بلاکیں = be32_مقصد_cpu(jsb->s_عدد_fc_blks);

	واپس عدد_fc_بلاکیں ? عدد_fc_بلاکیں : JBD2_DEFAULT_FAST_COMMIT_بلاکیں;
}

/* Comparison functions for transaction IDs: perform comparisons using
 * modulo arithmetic so that they work over sequence number wraps. */

_INLINE_ صحیح tid_gt(tid_t x, tid_t y)
{
	صحیح difference = (x - y);
	واپس (difference > 0);
}

_INLINE_ صحیح tid_geq(tid_t x, tid_t y)
{
	صحیح difference = (x - y);
	واپس (difference >= 0);
}
#اگر_ختم /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

#تعریف_منسوخ _INLINE_

خارجی صحیح journal_بلاکیں_فی_صفحہ(ساخت inode *inode);

/*
 * Definitions which augment the buffer_head layer
 */

/* journaling buffer types */
#تعریف_کرو BJ_None		0	/* Not journaled */
#تعریف_کرو BJ_Sync_ڈیٹا	1	/* Normal data: flush before commit */
#تعریف_کرو BJ_Async_ڈیٹا	2	/* writepage data: wait on it before commit */
#تعریف_کرو BJ_Metadata	3	/* Normal journaled metadata */
#تعریف_کرو BJ_Forget	4	/* Buffer superseded by this transaction */
#تعریف_کرو BJ_IO		5	/* Buffer is for temporary IO use */
#تعریف_کرو BJ_Shadow	6	/* Buffer contents being shadowed to the log */
#تعریف_کرو BJ_LogCtl	7	/* Buffer contains log descriptors */
#تعریف_کرو BJ_Reserved	8	/* Buffer is reserved for access by journal */
#تعریف_کرو BJ_Types	9

خارجی صحیح jbd_بلاکیں_فی_صفحہ(ساخت inode *inode);

#اگر_ختم	/* _LINUX_JBD_H */
