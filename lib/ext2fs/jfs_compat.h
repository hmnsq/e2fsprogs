
#اگر_غیر_تعریف_شدہ _JFS_COMPAT_H
#تعریف_کرو _JFS_COMPAT_H

#شامل "support/list.h"
#شامل <errno.h>
#اگر_تعریف_شدہ HAVE_NETINET_اندر_H
#شامل <netinet/in.h>
#اگر_ختم
#شامل <arpa/inet.h>
#شامل <stdbool.h>

#تعریف_کرو printk لکھو
#تعریف_کرو KERN_ERR ""
#تعریف_کرو KERN_DEBUG ""

#تعریف_کرو REQ_OP_READ 0
#تعریف_کرو REQ_OP_WRITE 1

#تعریف_کرو cpu_مقصد_le16(x)	ext2fs_cpu_مقصد_le16(x)
#تعریف_کرو cpu_مقصد_be16(x)	ext2fs_cpu_مقصد_be16(x)
#تعریف_کرو cpu_مقصد_le32(x)	ext2fs_cpu_مقصد_le32(x)
#تعریف_کرو cpu_مقصد_be32(x)	ext2fs_cpu_مقصد_be32(x)
#تعریف_کرو cpu_مقصد_le64(x)	ext2fs_cpu_مقصد_le64(x)
#تعریف_کرو cpu_مقصد_be64(x)	ext2fs_cpu_مقصد_be64(x)

#تعریف_کرو le16_مقصد_cpu(x)	ext2fs_le16_مقصد_cpu(x)
#تعریف_کرو be16_مقصد_cpu(x)	ext2fs_be16_مقصد_cpu(x)
#تعریف_کرو le32_مقصد_cpu(x)	ext2fs_le32_مقصد_cpu(x)
#تعریف_کرو be32_مقصد_cpu(x)	ext2fs_be32_مقصد_cpu(x)
#تعریف_کرو le64_مقصد_cpu(x)	ext2fs_le64_مقصد_cpu(x)
#تعریف_کرو be64_مقصد_cpu(x)	ext2fs_be64_مقصد_cpu(x)

قسمتعریف غیرعلامتی صحیح tid_t;
قسمتعریف ساخت journal_s journal_t;
قسمتعریف ساخت kdev_s *kdev_t;

ساخت بفر_سر;
ساخت inode;

قسمتعریف غیرعلامتی صحیح gfp_t;
#تعریف_کرو GFP_KERNEL	0
#تعریف_کرو GFP_NOFS	0
#تعریف_کرو __GFP_NOFAIL	0
#تعریف_کرو JBD2_لیبل_SIZE32	JBD_لیبل_SIZE32
#تعریف_کرو JBD2_BARRIER	0
قسمتعریف __u64 u64;
#تعریف_کرو put_bh(x)	brelse(x)

#تعریف_کرو چکری_جانچ_32_be(x, y, z)	ext2fs_چکری_جانچ_32_be((x), (y), (z))
#تعریف_کرو spin_تالا_init(x)
#تعریف_کرو spin_تالا(x)
#تعریف_کرو spin_unlock(x)
#تعریف_کرو SLAB_HWCACHE_ALIGN	0
#تعریف_کرو SLAB_TEMPORARY		0
#تعریف_کرو KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                حجم(ساخت __struct), __alignof__(ساخت __struct),\
                (__flags), تہی)

#تعریف_کرو blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#تعریف_کرو is_power_کا_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#تعریف_کرو roundup_power_کا_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#تعریف_کرو pr_emerg(fmt)
#تعریف_کرو pr_err(...)

شمار passtype {PASS_جائزہ, PASS_REVOKE, PASS_REPLAY};

#تعریف_کرو JBD2_FC_REPLAY_STOP		0
#تعریف_کرو JBD2_FC_REPLAY_CONTINUE		1

ساخت journal_s
{
	غیرعلامتی طویل		j_flags;
	صحیح			j_errno;
	ساخت بفر_سر *	j_sb_بفر;
	ساخت journal_superblock_s *j_superblock;
	صحیح			j_format_نسخہ;
	غیرعلامتی طویل		j_سر;
	غیرعلامتی طویل		j_دم;
	غیرعلامتی طویل		j_fc_پہلا;
	غیرعلامتی طویل		j_fc_off;
	غیرعلامتی طویل		j_fc_آخری;
	غیرعلامتی طویل		j_free;
	غیرعلامتی طویل		j_پہلا, j_آخری;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	صحیح			j_blocksize;
	غیرعلامتی صحیح		j_blk_انحراف;
	غیرعلامتی صحیح		j_کل_len;
	ساخت inode *		j_inode;
	tid_t			j_دم_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	ساخت jbd2_revoke_جدول_s *j_revoke;
	ساخت jbd2_revoke_جدول_s *j_revoke_جدول[2];
	tid_t			j_failed_commit;
	__u32			j_csum_بیج;
	صحیح (*j_fc_replay_callback)(ساخت journal_s *journal,
				    ساخت بفر_سر *bh,
				    شمار passtype pass, صحیح off,
				    tid_t expected_tid);

};

#تعریف_کرو is_journal_abort(x) 0

#تعریف_کرو بفر_TRACE(bh, info)	کرو {} جب (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#اگر_تعریف_شدہ NO_INLINE_FUNCS
#تعریف_کرو اندرونی
#اگر_ختم

#اگر_ختم /* _JFS_COMPAT_H */
