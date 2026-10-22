
#ifndef _JFS_COMPAT_H
#define _JFS_COMPAT_H

#ଅନ୍ତର୍ଭାଵ "support/list.h"
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ifdef HAVE_NETINET_IN_H
#ଅନ୍ତର୍ଭାଵ <netinet/in.h>
#endif
#ଅନ୍ତର୍ଭାଵ <arpa/inet.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>

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

ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା tid_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା journal_s journal_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା kdev_s *kdev_t;

ସଂରଚନା buffer_head;
ସଂରଚନା inode;

ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା gfp_t;
#define GFP_KERNEL	0
#define GFP_NOFS	0
#define __GFP_NOFAIL	0
#define JBD2_TAG_SIZE32	JBD_TAG_SIZE32
#define JBD2_BARRIER	0
ପ୍ରକାରବ୍ୟାଖ୍ୟା __u64 u64;
#define put_bh(x)	brelse(x)

#define crc32_be(x, y, z)	ext2fs_crc32_be((x), (y), (z))
#define spin_lock_init(x)
#define spin_lock(x)
#define spin_unlock(x)
#define SLAB_HWCACHE_ALIGN	0
#define SLAB_TEMPORARY		0
#define KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                ଆକାର(ସଂରଚନା __struct), __alignof__(ସଂରଚନା __struct),\
                (__flags), ଖାଲି)

#define blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#define is_power_of_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#define roundup_power_of_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#define pr_emerg(fmt)
#define pr_err(...)

ଗଣନା passtype {PASS_SCAN, PASS_REVOKE, PASS_REPLAY};

#define JBD2_FC_REPLAY_STOP		0
#define JBD2_FC_REPLAY_CONTINUE		1

ସଂରଚନା journal_s
{
	unsigned long		j_flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			j_errno;
	ସଂରଚନା buffer_head *	j_sb_buffer;
	ସଂରଚନା journal_superblock_s *j_superblock;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			j_format_version;
	unsigned long		j_head;
	unsigned long		j_tail;
	unsigned long		j_fc_first;
	unsigned long		j_fc_off;
	unsigned long		j_fc_last;
	unsigned long		j_free;
	unsigned long		j_first, j_last;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			j_blocksize;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		j_blk_offset;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		j_total_len;
	ସଂରଚନା inode *		j_inode;
	tid_t			j_tail_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	ସଂରଚନା jbd2_revoke_table_s *j_revoke;
	ସଂରଚନା jbd2_revoke_table_s *j_revoke_table[2];
	tid_t			j_failed_commit;
	__u32			j_csum_seed;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*j_fc_replay_callback)(ସଂରଚନା journal_s *journal,
				    ସଂରଚନା buffer_head *bh,
				    ଗଣନା passtype pass, ପୂର୍ଣ୍ଣସଂଖ୍ୟା off,
				    tid_t expected_tid);

};

#define is_journal_abort(x) 0

#define BUFFER_TRACE(bh, info)	କର {} ଯେତେବେଳେ (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#ifdef NO_INLINE_FUNCS
#define ଧାଡ଼ିଭିତରେ
#endif

#endif /* _JFS_COMPAT_H */
