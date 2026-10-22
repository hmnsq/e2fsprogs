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

#که_نه_تعریف_شوی _LINUX_JBD_H
#تعریف_کړه _LINUX_JBD_H

#ضمن "jfs_compat.h"
#تعریف_کړه JFS_DEBUG
#تعریف_کړه jfs_debug jbd_debug

#که_نه_تعریف_شوی __GNUC__
#تعریف_کړه __FUNCTION__ ""
#د_که_پای

#تعریف_کړه journal_oom_retry 0

#که_تعریف_شوی CONFIG_JBD_DEBUG
/*
 * Define JBD_EXPENSIVE_CHECKING to enable more expensive internal
 * consistency checks.  By default we don't do this unless
 * CONFIG_JBD_DEBUG is on.
 */
#تعریف_کړه JBD_EXPENSIVE_CHECKING
خارجي صحيح journal_enable_debug;
#وإلا
#تعریف_کړه journal_enable_debug (-1)
#د_که_پای /* !CONFIG_JBD_DEBUG */

#که_تعریف_شوی __STDC__
#تعریف_کړه jbd_debug(n, f, a...)						\
	افعل {								\
		إذا ((n) <= journal_enable_debug) {			\
			printk (KERN_DEBUG "(%s, %d): %s: ",		\
				__FILE__, __LINE__, __FUNCTION__);	\
		  	printk (f, ## a);				\
		}							\
	} طالما (0)
#وإلا
#تعریف_کړه jbd_debug(x)		/* AIX doesn't do STDC */
#د_که_پای /* !__STDC__ */

خارجي فراغ * __jbd_kmalloc (حرف *where, size_t اندازه, صحيح flags, صحيح retry);
#تعریف_کړه jbd_kmalloc(اندازه, flags) \
	__jbd_kmalloc(__FUNCTION__, (اندازه), (flags), journal_oom_retry)
#تعریف_کړه jbd_rep_kmalloc(اندازه, flags) \
	__jbd_kmalloc(__FUNCTION__, (اندازه), (flags), 1)

#تعریف_کړه JBD2_MIN_JOURNAL_بلاکونه 1024
#تعریف_کړه JBD2_MAX_JOURNAL_بلاکونه (JBD2_MIN_JOURNAL_بلاکونه * 10000)
#تعریف_کړه JBD2_DEFAULT_FAST_COMMIT_بلاکونه 256

/*
 * Internal structures used by the logging mechanism:
 */

#تعریف_کړه JBD2_MAGIC_شمېره 0xc03b3998U /* The first 4 bytes of /dev/random! */

/*
 * On-disk structures
 */

/*
 * Descriptor block types:
 */

#تعریف_کړه JBD2_DESCRIPTOR_بلاک	1
#تعریف_کړه JBD2_COMMIT_بلاک	2
#تعریف_کړه JBD2_SUPERBLOCK_V1	3
#تعریف_کړه JBD2_SUPERBLOCK_V2	4
#تعریف_کړه JBD2_REVOKE_بلاک	5
#تعریف_کړه JBD2_FC_بلاک		6

/*
 * Standard header for all descriptor blocks:
 */
تعريفنوع بنية journal_سرلیک_s
{
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
} journal_سرلیک_t;

/*
 * Checksum types.
 */
#تعریف_کړه JBD2_دوراني_ازموینه_32_CHKSUM   1
#تعریف_کړه JBD2_MD5_CHKSUM     2
#تعریف_کړه JBD2_SHA1_CHKSUM    3
#تعریف_کړه JBD2_دوراني_ازموینه_32_C_CHKSUM  4

#تعریف_کړه JBD2_دوراني_ازموینه_32_CHKSUM_اندازه 4

#تعریف_کړه JBD2_CHECKSUM_بایټونه (32 / حجم(__u32))
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
بنية commit_سرلیک {
	__be32		h_magic;
	__be32		h_blocktype;
	__be32		h_sequence;
	بینښه حرف	h_chksum_type;
	بینښه حرف	h_chksum_اندازه;
	بینښه حرف	h_padding[2];
	__be32		h_chksum[JBD2_CHECKSUM_بایټونه];
	__be64		h_commit_sec;
	__be32		h_commit_nsec;
};

/*
 * The block tag: used to describe a single buffer in the journal
 */
تعريفنوع بنية journal_بلاک_tag3_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be32		t_flags;	/* See below */
	__be32		t_blocknr_لوړ; /* most-significant high 32bits. */
	__be32		t_checksum;	/* crc32c(uuid+seq+block) */
} journal_بلاک_tag3_t;

تعريفنوع بنية journal_بلاک_نښان_s
{
	__be32		t_blocknr;	/* The on-disk block number */
	__be16		t_checksum;	/* truncated crc32c(uuid+seq+block) */
	__be16		t_flags;	/* See below */
	__be32		t_blocknr_لوړ; /* most-significant high 32bits. */
} journal_بلاک_نښان_t;

/* Tail of descriptor or revoke block, for checksumming */
بنية jbd2_journal_بلاک_لکۍ {
	__be32		t_checksum;
};

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
تعريفنوع بنية journal_revoke_سرلیک_s
{
	journal_سرلیک_t r_سرلیک;
	__be32		 r_شمېر;	/* Count of bytes used in the block */
} jbd2_journal_revoke_سرلیک_t;

/* Definitions for the journal tag flags word: */
#تعریف_کړه JBD2_FLAG_ESCAPE		1	/* on-disk block is escaped */
#تعریف_کړه JBD2_FLAG_SAME_UUID	2	/* block has same uuid as previous */
#تعریف_کړه JBD2_FLAG_DELETED	4	/* block deleted by this transaction */
#تعریف_کړه JBD2_FLAG_وروستی_نښان	8	/* last tag in this descriptor block */


#تعریف_کړه UUID_اندازه 16
#تعریف_کړه JBD2_USERS_MAX 48
#تعریف_کړه JBD2_USERS_اندازه (UUID_اندازه * JBD2_USERS_MAX)
/*
 * The journal superblock.  All fields are in big-endian byte order.
 */
تعريفنوع بنية journal_superblock_s
{
/* 0x0000 */
	journal_سرلیک_t s_سرلیک;

/* 0x000C */
	/* Static information describing the journal */
	__be32	s_blocksize;		/* journal device blocksize */
	__be32	s_maxlen;		/* total blocks in journal file */
	__be32	s_لومړی;		/* first block of log information */

/* 0x0018 */
	/* Dynamic information describing the current state of the log */
	__be32	s_sequence;		/* first commit ID expected in log */
	__be32	s_پیل;		/* blocknr of start of log */

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
	__be32	s_max_trans_معلومات;	/* Limit of data blocks per trans. */

/* 0x0050 */
	__u8	s_checksum_type;	/* checksum type */
	__u8	s_padding2[3];
/* 0x0054 */
	__be32	s_عدد_fc_blks;		/* Number of fast commit blocks */
/* 0x0058 */
	__be32	s_padding[41];
	__be32	s_checksum;		/* crc32c(superblock) */

/* 0x0100 */
	__u8	s_users[JBD2_USERS_اندازه];		/* ids of all fs'es sharing the log */

/* 0x0400 */
} journal_superblock_t;

#تعریف_کړه JBD2_HAS_COMPAT_FEATURE(j,ماسک)					\
	((j)->j_format_نسخه >= 2 &&					\
	 ((j)->j_superblock->s_feature_compat & ext2fs_cpu_منزل_be32((ماسک))))
#تعریف_کړه JBD2_HAS_RO_COMPAT_FEATURE(j,ماسک)				\
	((j)->j_format_نسخه >= 2 &&					\
	 ((j)->j_superblock->s_feature_ro_compat & ext2fs_cpu_منزل_be32((ماسک))))
#تعریف_کړه JBD2_HAS_INCOMPAT_FEATURE(j,ماسک)				\
	((j)->j_format_نسخه >= 2 &&					\
	 ((j)->j_superblock->s_feature_incompat & ext2fs_cpu_منزل_be32((ماسک))))

#تعریف_کړه JBD2_FEATURE_COMPAT_CHECKSUM		0x00000001

#تعریف_کړه JBD2_FEATURE_INCOMPAT_REVOKE		0x00000001
#تعریف_کړه JBD2_FEATURE_INCOMPAT_64_بټ		0x00000002
#تعریف_کړه JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT	0x00000004
#تعریف_کړه JBD2_FEATURE_INCOMPAT_CSUM_V2		0x00000008
#تعریف_کړه JBD2_FEATURE_INCOMPAT_CSUM_V3		0x00000010
#تعریف_کړه JBD2_FEATURE_INCOMPAT_FAST_COMMIT	0x00000020

/* Features known to this kernel version: */
#تعریف_کړه JBD2_معلوم_COMPAT_FEATURES	0
#تعریف_کړه JBD2_معلوم_ROCOMPAT_FEATURES	0
#تعریف_کړه JBD2_معلوم_INCOMPAT_FEATURES	(JBD2_FEATURE_INCOMPAT_REVOKE|\
					 JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT| \
					 JBD2_FEATURE_INCOMPAT_64_بټ|\
					 JBD2_FEATURE_INCOMPAT_CSUM_V2|	\
					 JBD2_FEATURE_INCOMPAT_CSUM_V3 | \
					 JBD2_FEATURE_INCOMPAT_FAST_COMMIT)

#که_تعریف_شوی NO_INLINE_FUNCS
خارجي size_t journal_نښان_بایټونه(journal_t *journal);
خارجي صحيح jbd2_journal_has_csum_v2or3(journal_t *journal);
خارجي صحيح jbd2_journal_get_عدد_fc_blks(journal_superblock_t *jsb);
خارجي صحيح tid_gt(tid_t x, tid_t y) EXT2FS_ATTR((ناکارول_شوی));
خارجي صحيح tid_geq(tid_t x, tid_t y) EXT2FS_ATTR((ناکارول_شوی));
#د_که_پای

#إذا (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#که_تعریف_شوی E2FSCK_INCLUDE_INLINE_FUNCS
#إذا (__STDC_VERSION__ >= 199901L)
#تعریف_کړه _INLINE_ خارجي مضمن
#وإلا
#تعریف_کړه _INLINE_ مضمن
#د_که_پای
#وإلا /* !E2FSCK_INCLUDE_INLINE FUNCS */
#إذا (__STDC_VERSION__ >= 199901L)
#تعریف_کړه _INLINE_ مضمن
#وإلا /* not C99 */
#که_تعریف_شوی __GNUC__
#تعریف_کړه _INLINE_ خارجي __inline__
#وإلا				/* For Watcom C */
#تعریف_کړه _INLINE_ خارجي مضمن
#د_که_پای /* __GNUC__ */
#د_که_پای /* __STDC_VERSION__ >= 199901L */
#د_که_پای /* INCLUDE_INLINE_FUNCS */

/* journal feature predicate functions */
#تعریف_کړه JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j); \
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j) \
{ \
	أرجع ((j)->j_format_نسخه >= 2 && \
		((j)->j_superblock->s_feature_compat & \
		 ext2fs_cpu_منزل_be32(JBD2_FEATURE_COMPAT_##flagname)) != 0); \
} \
_INLINE_ فراغ jbd2_سېټ_feature##name(journal_t *j); \
_INLINE_ فراغ jbd2_سېټ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat |= \
		ext2fs_cpu_منزل_be32(JBD2_FEATURE_COMPAT_##flagname); \
} \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_compat &= \
		~ext2fs_cpu_منزل_be32(JBD2_FEATURE_COMPAT_##flagname); \
}

#تعریف_کړه JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j);	\
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j) \
{ \
	أرجع ((j)->j_format_نسخه >= 2 && \
		((j)->j_superblock->s_feature_ro_compat & \
		 ext2fs_cpu_منزل_be32(JBD2_FEATURE_RO_COMPAT_##flagname)) != 0); \
} \
_INLINE_ فراغ jbd2_سېټ_feature##name(journal_t *j); \
_INLINE_ فراغ jbd2_سېټ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat |= \
		ext2fs_cpu_منزل_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
} \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_ro_compat &= \
		~ext2fs_cpu_منزل_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
}

#تعریف_کړه JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j); \
_INLINE_ صحيح jbd2_has_feature_##name(journal_t *j) \
{ \
	أرجع ((j)->j_format_نسخه >= 2 && \
		((j)->j_superblock->s_feature_incompat & \
		 ext2fs_cpu_منزل_be32(JBD2_FEATURE_INCOMPAT_##flagname)) != 0); \
} \
_INLINE_ فراغ jbd2_سېټ_feature##name(journal_t *j); \
_INLINE_ فراغ jbd2_سېټ_feature##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat |= \
		ext2fs_cpu_منزل_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
} \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ فراغ jbd2_clear_feature_##name(journal_t *j) \
{ \
	(j)->j_superblock->s_feature_incompat &= \
		~ext2fs_cpu_منزل_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
}

#وإلا
#تعریف_کړه JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
خارجي صحيح jbd2_has_feature_##name(journal_t *j); \
خارجي فراغ jbd2_سېټ_feature##name(journal_t *j); \
خارجي فراغ jbd2_clear_feature_##name(journal_t *j);

#تعریف_کړه JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
خارجي صحيح jbd2_has_feature_##name(journal_t *j); \
خارجي فراغ jbd2_سېټ_feature##name(journal_t *j); \
خارجي فراغ jbd2_clear_feature_##name(journal_t *j);

#تعریف_کړه JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
خارجي صحيح jbd2_has_feature_##name(journal_t *j); \
خارجي فراغ jbd2_سېټ_feature##name(journal_t *j); \
خارجي فراغ jbd2_clear_feature_##name(journal_t *j);

#د_که_پای /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

JBD2_FEATURE_COMPAT_FUNCS(checksum,		CHECKSUM)

JBD2_FEATURE_INCOMPAT_FUNCS(revoke,		REVOKE)
JBD2_FEATURE_INCOMPAT_FUNCS(64بټ,		64بټ_2)
JBD2_FEATURE_INCOMPAT_FUNCS(async_commit,	ASYNC_COMMIT)
JBD2_FEATURE_INCOMPAT_FUNCS(csum2,		CSUM_V2)
JBD2_FEATURE_INCOMPAT_FUNCS(csum3,		CSUM_V3)
JBD2_FEATURE_INCOMPAT_FUNCS(fast_commit,	FAST_COMMIT)

#إذا (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
/*
 * helper functions to deal with 32 or 64bit block numbers.
 */
_INLINE_ size_t journal_نښان_بایټونه(journal_t *journal)
{
	size_t sz;

	إذا (jbd2_has_feature_csum3(journal))
		أرجع حجم(journal_بلاک_tag3_t);

	sz = حجم(journal_بلاک_نښان_t);

	إذا (jbd2_has_feature_csum2(journal))
		sz += حجم(__u16);

	إذا (jbd2_has_feature_64bit(journal))
		أرجع sz;

	أرجع sz - حجم(__u32);
}

_INLINE_ صحيح jbd2_journal_has_csum_v2or3(journal_t *journal)
{
	إذا (jbd2_has_feature_csum2(journal) || jbd2_has_feature_csum3(journal))
		أرجع 1;

	أرجع 0;
}

_INLINE_ صحيح jbd2_journal_get_عدد_fc_blks(journal_superblock_t *jsb)
{
	صحيح عدد_fc_بلاکونه = be32_منزل_cpu(jsb->s_عدد_fc_blks);

	أرجع عدد_fc_بلاکونه ? عدد_fc_بلاکونه : JBD2_DEFAULT_FAST_COMMIT_بلاکونه;
}

/* Comparison functions for transaction IDs: perform comparisons using
 * modulo arithmetic so that they work over sequence number wraps. */

_INLINE_ صحيح tid_gt(tid_t x, tid_t y)
{
	صحيح difference = (x - y);
	أرجع (difference > 0);
}

_INLINE_ صحيح tid_geq(tid_t x, tid_t y)
{
	صحيح difference = (x - y);
	أرجع (difference >= 0);
}
#د_که_پای /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

#تعریف_لغوه _INLINE_

خارجي صحيح journal_بلاکونه_فی_پاڼه(بنية inode *inode);

/*
 * Definitions which augment the buffer_head layer
 */

/* journaling buffer types */
#تعریف_کړه BJ_None		0	/* Not journaled */
#تعریف_کړه BJ_Sync_معلومات	1	/* Normal data: flush before commit */
#تعریف_کړه BJ_Async_معلومات	2	/* writepage data: wait on it before commit */
#تعریف_کړه BJ_Metadata	3	/* Normal journaled metadata */
#تعریف_کړه BJ_Forget	4	/* Buffer superseded by this transaction */
#تعریف_کړه BJ_IO		5	/* Buffer is for temporary IO use */
#تعریف_کړه BJ_Shadow	6	/* Buffer contents being shadowed to the log */
#تعریف_کړه BJ_LogCtl	7	/* Buffer contains log descriptors */
#تعریف_کړه BJ_Reserved	8	/* Buffer is reserved for access by journal */
#تعریف_کړه BJ_Types	9

خارجي صحيح jbd_بلاکونه_فی_پاڼه(بنية inode *inode);

#د_که_پای	/* _LINUX_JBD_H */
