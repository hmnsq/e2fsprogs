
#ifndef _JFS_COMPAT_H
#define _JFS_COMPAT_H

#अन्तर्भाव "support/list.h"
#अन्तर्भाव <errno.h>
#ifdef HAVE_NETINET_IN_H
#अन्तर्भाव <netinet/in.h>
#endif
#अन्तर्भाव <arpa/inet.h>
#अन्तर्भाव <stdbool.h>

#define printk म_लिखो
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

प्रकारनिर्देश अचिन्हित पूर्णाङ्क tid_t;
प्रकारनिर्देश संरचना journal_s journal_t;
प्रकारनिर्देश संरचना kdev_s *kdev_t;

संरचना buffer_head;
संरचना inode;

प्रकारनिर्देश अचिन्हित पूर्णाङ्क gfp_t;
#define GFP_KERNEL	0
#define GFP_NOFS	0
#define __GFP_NOFAIL	0
#define JBD2_TAG_SIZE32	JBD_TAG_SIZE32
#define JBD2_BARRIER	0
प्रकारनिर्देश __u64 u64;
#define put_bh(x)	brelse(x)

#define crc32_be(x, y, z)	ext2fs_crc32_be((x), (y), (z))
#define spin_lock_init(x)
#define spin_lock(x)
#define spin_unlock(x)
#define SLAB_HWCACHE_ALIGN	0
#define SLAB_TEMPORARY		0
#define KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                परिमाण(संरचना __struct), __alignof__(संरचना __struct),\
                (__flags), रिक्त)

#define blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#define is_power_of_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#define roundup_power_of_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#define pr_emerg(fmt)
#define pr_err(...)

गणना passtype {PASS_SCAN, PASS_REVOKE, PASS_REPLAY};

#define JBD2_FC_REPLAY_STOP		0
#define JBD2_FC_REPLAY_CONTINUE		1

संरचना journal_s
{
	अचिन्हित दीर्घ		j_flags;
	पूर्णाङ्क			j_errno;
	संरचना buffer_head *	j_sb_buffer;
	संरचना journal_superblock_s *j_superblock;
	पूर्णाङ्क			j_format_version;
	अचिन्हित दीर्घ		j_head;
	अचिन्हित दीर्घ		j_tail;
	अचिन्हित दीर्घ		j_fc_first;
	अचिन्हित दीर्घ		j_fc_off;
	अचिन्हित दीर्घ		j_fc_last;
	अचिन्हित दीर्घ		j_free;
	अचिन्हित दीर्घ		j_first, j_last;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	पूर्णाङ्क			j_blocksize;
	अचिन्हित पूर्णाङ्क		j_blk_offset;
	अचिन्हित पूर्णाङ्क		j_total_len;
	संरचना inode *		j_inode;
	tid_t			j_tail_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	संरचना jbd2_revoke_table_s *j_revoke;
	संरचना jbd2_revoke_table_s *j_revoke_table[2];
	tid_t			j_failed_commit;
	__u32			j_csum_seed;
	पूर्णाङ्क (*j_fc_replay_callback)(संरचना journal_s *journal,
				    संरचना buffer_head *bh,
				    गणना passtype pass, पूर्णाङ्क off,
				    tid_t expected_tid);

};

#define is_journal_abort(x) 0

#define BUFFER_TRACE(bh, info)	गर {} जबसम्म (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#ifdef NO_INLINE_FUNCS
#define अन्तःस्थ
#endif

#endif /* _JFS_COMPAT_H */
