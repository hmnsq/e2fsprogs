
#ifndef _JFS_COMPAT_H
#define _JFS_COMPAT_H

#شامل "support/list.h"
#شامل <errno.h>
#ifdef HAVE_NETINET_IN_H
#شامل <netinet/in.h>
#endif
#شامل <arpa/inet.h>
#شامل <stdbool.h>

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

تعریفنوع unsigned صحیح tid_t;
تعریفنوع ساختار journal_s journal_t;
تعریفنوع ساختار kdev_s *kdev_t;

ساختار buffer_head;
ساختار inode;

تعریفنوع unsigned صحیح gfp_t;
#define GFP_KERNEL	0
#define GFP_NOFS	0
#define __GFP_NOFAIL	0
#define JBD2_TAG_SIZE32	JBD_TAG_SIZE32
#define JBD2_BARRIER	0
تعریفنوع __u64 u64;
#define put_bh(x)	brelse(x)

#define crc32_be(x, y, z)	ext2fs_crc32_be((x), (y), (z))
#define spin_lock_init(x)
#define spin_lock(x)
#define spin_unlock(x)
#define SLAB_HWCACHE_ALIGN	0
#define SLAB_TEMPORARY		0
#define KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                اندازه(ساختار __struct), __alignof__(ساختار __struct),\
                (__flags), پوچ)

#define blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#define is_power_of_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#define roundup_power_of_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#define pr_emerg(fmt)
#define pr_err(...)

شمارش passtype {PASS_SCAN, PASS_REVOKE, PASS_REPLAY};

#define JBD2_FC_REPLAY_STOP		0
#define JBD2_FC_REPLAY_CONTINUE		1

ساختار journal_s
{
	unsigned long		j_flags;
	صحیح			j_errno;
	ساختار buffer_head *	j_sb_buffer;
	ساختار journal_superblock_s *j_superblock;
	صحیح			j_format_version;
	unsigned long		j_head;
	unsigned long		j_tail;
	unsigned long		j_fc_first;
	unsigned long		j_fc_off;
	unsigned long		j_fc_last;
	unsigned long		j_free;
	unsigned long		j_first, j_last;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	صحیح			j_blocksize;
	unsigned صحیح		j_blk_offset;
	unsigned صحیح		j_total_len;
	ساختار inode *		j_inode;
	tid_t			j_tail_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	ساختار jbd2_revoke_table_s *j_revoke;
	ساختار jbd2_revoke_table_s *j_revoke_table[2];
	tid_t			j_failed_commit;
	__u32			j_csum_seed;
	صحیح (*j_fc_replay_callback)(ساختار journal_s *journal,
				    ساختار buffer_head *bh,
				    شمارش passtype pass, صحیح off,
				    tid_t expected_tid);

};

#define is_journal_abort(x) 0

#define BUFFER_TRACE(bh, info)	انجامبده {} وقتی (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#ifdef NO_INLINE_FUNCS
#define درونخطی
#endif

#endif /* _JFS_COMPAT_H */
