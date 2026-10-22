
#ifndef _JFS_COMPAT_H
#define _JFS_COMPAT_H

#অন্তর্ভাব "support/list.h"
#অন্তর্ভাব <errno.h>
#ifdef HAVE_NETINET_IN_H
#অন্তর্ভাব <netinet/in.h>
#endif
#অন্তর্ভাব <arpa/inet.h>
#অন্তর্ভাব <stdbool.h>

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

প্রকারলেখ unsigned পূর্ণসংখ্যা tid_t;
প্রকারলেখ কাঠামো journal_s journal_t;
প্রকারলেখ কাঠামো kdev_s *kdev_t;

কাঠামো বাফার_শীর্ষ;
কাঠামো inode;

প্রকারলেখ unsigned পূর্ণসংখ্যা gfp_t;
#define GFP_KERNEL	0
#define GFP_NOFS	0
#define __GFP_NOFAIL	0
#define JBD2_TAG_SIZE32	JBD_TAG_SIZE32
#define JBD2_BARRIER	0
প্রকারলেখ __u64 u64;
#define put_bh(x)	brelse(x)

#define crc32_be(x, y, z)	ext2fs_crc32_be((x), (y), (z))
#define spin_lock_init(x)
#define spin_lock(x)
#define spin_unlock(x)
#define SLAB_HWCACHE_ALIGN	0
#define SLAB_TEMPORARY		0
#define KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                আকার(কাঠামো __struct), __alignof__(কাঠামো __struct),\
                (__flags), ফাঁকা)

#define blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#define is_power_of_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#define roundup_power_of_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#define pr_emerg(fmt)
#define pr_err(...)

গণনা passtype {PASS_SCAN, PASS_REVOKE, PASS_REPLAY};

#define JBD2_FC_REPLAY_STOP		0
#define JBD2_FC_REPLAY_CONTINUE		1

কাঠামো journal_s
{
	unsigned long		j_flags;
	পূর্ণসংখ্যা			j_errno;
	কাঠামো বাফার_শীর্ষ *	j_sb_বাফার;
	কাঠামো journal_superblock_s *j_superblock;
	পূর্ণসংখ্যা			j_format_version;
	unsigned long		j_শীর্ষ;
	unsigned long		j_লেজ;
	unsigned long		j_fc_first;
	unsigned long		j_fc_off;
	unsigned long		j_fc_last;
	unsigned long		j_free;
	unsigned long		j_first, j_last;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	পূর্ণসংখ্যা			j_blocksize;
	unsigned পূর্ণসংখ্যা		j_blk_offset;
	unsigned পূর্ণসংখ্যা		j_যোগ_len;
	কাঠামো inode *		j_inode;
	tid_t			j_লেজ_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	কাঠামো jbd2_revoke_table_s *j_revoke;
	কাঠামো jbd2_revoke_table_s *j_revoke_table[2];
	tid_t			j_failed_commit;
	__u32			j_csum_seed;
	পূর্ণসংখ্যা (*j_fc_replay_callback)(কাঠামো journal_s *journal,
				    কাঠামো বাফার_শীর্ষ *bh,
				    গণনা passtype pass, পূর্ণসংখ্যা off,
				    tid_t expected_tid);

};

#define is_journal_abort(x) 0

#define বাফার_TRACE(bh, info)	করো {} যখন (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#ifdef NO_INLINE_FUNCS
#define অন্তঃরেখ
#endif

#endif /* _JFS_COMPAT_H */
