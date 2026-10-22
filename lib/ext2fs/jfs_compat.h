
#ifndef _JFS_COMPAT_H
#define _JFS_COMPAT_H

#ಅನ್ತರ್ಭಾವ "support/list.h"
#ಅನ್ತರ್ಭಾವ <errno.h>
#ifdef HAVE_NETINET_IN_H
#ಅನ್ತರ್ಭಾವ <netinet/in.h>
#endif
#ಅನ್ತರ್ಭಾವ <arpa/inet.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>

#define printk printf
#define KERN_ERR ""
#define KERN_DEBUG ""

#define REQ_OP_READ 0
#define REQ_OP_WRITE 1

#define cpu_to_le16(x)	ext2fs_cpu_to_le16(x)
#define cpu_to_be16(x)	ext2fs_cpu_to_be16(x)
#define cpu_to_le32(x)	ext2fs_cpu_to_le32(x)
#define cpu_to_be32(x)	ext2fs_cpu_to_be32(x)
#define cpu_to_le64(x)	ext2fs_cpu_to_le64(x)
#define cpu_to_be64(x)	ext2fs_cpu_to_be64(x)

#define le16_to_cpu(x)	ext2fs_le16_to_cpu(x)
#define be16_to_cpu(x)	ext2fs_be16_to_cpu(x)
#define le32_to_cpu(x)	ext2fs_le32_to_cpu(x)
#define be32_to_cpu(x)	ext2fs_be32_to_cpu(x)
#define le64_to_cpu(x)	ext2fs_le64_to_cpu(x)
#define be64_to_cpu(x)	ext2fs_be64_to_cpu(x)

ಪ್ರಕಾರನಿರ್ದೇಶ unsigned ಪೂರ್ಣಾಙ್ಕ tid_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ journal_s journal_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ kdev_s *kdev_t;

ಸಂರಚನಾ buffer_head;
ಸಂರಚನಾ inode;

ಪ್ರಕಾರನಿರ್ದೇಶ unsigned ಪೂರ್ಣಾಙ್ಕ gfp_t;
#define GFP_KERNEL	0
#define GFP_NOFS	0
#define __GFP_NOFAIL	0
#define JBD2_TAG_SIZE32	JBD_TAG_SIZE32
#define JBD2_BARRIER	0
ಪ್ರಕಾರನಿರ್ದೇಶ __u64 u64;
#define put_bh(x)	brelse(x)

#define crc32_be(x, y, z)	ext2fs_crc32_be((x), (y), (z))
#define spin_lock_init(x)
#define spin_lock(x)
#define spin_unlock(x)
#define SLAB_HWCACHE_ALIGN	0
#define SLAB_TEMPORARY		0
#define KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                ಪರಿಮಾಣ(ಸಂರಚನಾ __struct), __alignof__(ಸಂರಚನಾ __struct),\
                (__flags), ರಿಕ್ತ)

#define blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#define is_power_of_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#define roundup_power_of_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#define pr_emerg(fmt)
#define pr_err(...)

ಗಣನಾ passtype {PASS_SCAN, PASS_REVOKE, PASS_REPLAY};

#define JBD2_FC_REPLAY_STOP		0
#define JBD2_FC_REPLAY_CONTINUE		1

ಸಂರಚನಾ journal_s
{
	unsigned long		j_flags;
	ಪೂರ್ಣಾಙ್ಕ			j_errno;
	ಸಂರಚನಾ buffer_head *	j_sb_buffer;
	ಸಂರಚನಾ journal_superblock_s *j_superblock;
	ಪೂರ್ಣಾಙ್ಕ			j_format_version;
	unsigned long		j_head;
	unsigned long		j_tail;
	unsigned long		j_fc_first;
	unsigned long		j_fc_off;
	unsigned long		j_fc_last;
	unsigned long		j_free;
	unsigned long		j_first, j_last;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	ಪೂರ್ಣಾಙ್ಕ			j_blocksize;
	unsigned ಪೂರ್ಣಾಙ್ಕ		j_blk_offset;
	unsigned ಪೂರ್ಣಾಙ್ಕ		j_total_len;
	ಸಂರಚನಾ inode *		j_inode;
	tid_t			j_tail_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	ಸಂರಚನಾ jbd2_revoke_table_s *j_revoke;
	ಸಂರಚನಾ jbd2_revoke_table_s *j_revoke_table[2];
	tid_t			j_failed_commit;
	__u32			j_csum_seed;
	ಪೂರ್ಣಾಙ್ಕ (*j_fc_replay_callback)(ಸಂರಚನಾ journal_s *journal,
				    ಸಂರಚನಾ buffer_head *bh,
				    ಗಣನಾ passtype pass, ಪೂರ್ಣಾಙ್ಕ off,
				    tid_t expected_tid);

};

#define is_journal_abort(x) 0

#define BUFFER_TRACE(bh, info)	ಕುರು {} ಯಾವತ್ (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#ifdef NO_INLINE_FUNCS
#define ಅನ್ತಃಸ್ಥ
#endif

#endif /* _JFS_COMPAT_H */
