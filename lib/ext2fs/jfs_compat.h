
#ifndef _JFS_COMPAT_H
#define _JFS_COMPAT_H

#ضمن "support/list.h"
#ضمن <errno.h>
#ifdef HAVE_NETINET_IN_H
#ضمن <netinet/in.h>
#endif
#ضمن <arpa/inet.h>
#ضمن <stdbool.h>

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

تعريفنوع unsigned صحيح tid_t;
تعريفنوع بنية journal_s journal_t;
تعريفنوع بنية kdev_s *kdev_t;

بنية buffer_head;
بنية inode;

تعريفنوع unsigned صحيح gfp_t;
#define GFP_KERNEL	0
#define GFP_NOFS	0
#define __GFP_NOFAIL	0
#define JBD2_TAG_SIZE32	JBD_TAG_SIZE32
#define JBD2_BARRIER	0
تعريفنوع __u64 u64;
#define put_bh(x)	brelse(x)

#define crc32_be(x, y, z)	ext2fs_crc32_be((x), (y), (z))
#define spin_lock_init(x)
#define spin_lock(x)
#define spin_unlock(x)
#define SLAB_HWCACHE_ALIGN	0
#define SLAB_TEMPORARY		0
#define KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                حجم(بنية __struct), __alignof__(بنية __struct),\
                (__flags), عدم)

#define blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#define is_power_of_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#define roundup_power_of_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#define pr_emerg(fmt)
#define pr_err(...)

تعداد passtype {PASS_SCAN, PASS_REVOKE, PASS_REPLAY};

#define JBD2_FC_REPLAY_STOP		0
#define JBD2_FC_REPLAY_CONTINUE		1

بنية journal_s
{
	unsigned long		j_flags;
	صحيح			j_errno;
	بنية buffer_head *	j_sb_buffer;
	بنية journal_superblock_s *j_superblock;
	صحيح			j_format_version;
	unsigned long		j_head;
	unsigned long		j_tail;
	unsigned long		j_fc_first;
	unsigned long		j_fc_off;
	unsigned long		j_fc_last;
	unsigned long		j_free;
	unsigned long		j_first, j_last;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	صحيح			j_blocksize;
	unsigned صحيح		j_blk_offset;
	unsigned صحيح		j_total_len;
	بنية inode *		j_inode;
	tid_t			j_tail_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	بنية jbd2_revoke_table_s *j_revoke;
	بنية jbd2_revoke_table_s *j_revoke_table[2];
	tid_t			j_failed_commit;
	__u32			j_csum_seed;
	صحيح (*j_fc_replay_callback)(بنية journal_s *journal,
				    بنية buffer_head *bh,
				    تعداد passtype pass, صحيح off,
				    tid_t expected_tid);

};

#define is_journal_abort(x) 0

#define BUFFER_TRACE(bh, info)	𐡏𐡁𐡃 {} طالما (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#ifdef NO_INLINE_FUNCS
#define مضمن
#endif

#endif /* _JFS_COMPAT_H */
