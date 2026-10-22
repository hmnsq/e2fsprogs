
#اگر_تعریف_نشده _JFS_COMPAT_H
#تعریف_کن _JFS_COMPAT_H

#ضمن "support/list.h"
#ضمن <errno.h>
#اگر_تعریف_شده HAVE_NETINET_درون_H
#ضمن <netinet/in.h>
#پایان_اگر
#ضمن <arpa/inet.h>
#ضمن <stdbool.h>

#تعریف_کن printk چاپ_کن
#تعریف_کن KERN_ERR ""
#تعریف_کن KERN_DEBUG ""

#تعریف_کن REQ_OP_READ 0
#تعریف_کن REQ_OP_WRITE 1

#تعریف_کن cpu_مقصد_le16(x)	ext2fs_cpu_مقصد_le16(x)
#تعریف_کن cpu_مقصد_be16(x)	ext2fs_cpu_مقصد_be16(x)
#تعریف_کن cpu_مقصد_le32(x)	ext2fs_cpu_مقصد_le32(x)
#تعریف_کن cpu_مقصد_be32(x)	ext2fs_cpu_مقصد_be32(x)
#تعریف_کن cpu_مقصد_le64(x)	ext2fs_cpu_مقصد_le64(x)
#تعریف_کن cpu_مقصد_be64(x)	ext2fs_cpu_مقصد_be64(x)

#تعریف_کن le16_مقصد_cpu(x)	ext2fs_le16_مقصد_cpu(x)
#تعریف_کن be16_مقصد_cpu(x)	ext2fs_be16_مقصد_cpu(x)
#تعریف_کن le32_مقصد_cpu(x)	ext2fs_le32_مقصد_cpu(x)
#تعریف_کن be32_مقصد_cpu(x)	ext2fs_be32_مقصد_cpu(x)
#تعریف_کن le64_مقصد_cpu(x)	ext2fs_le64_مقصد_cpu(x)
#تعریف_کن be64_مقصد_cpu(x)	ext2fs_be64_مقصد_cpu(x)

تعريفنوع بیعلامت صحيح tid_t;
تعريفنوع بنية journal_s journal_t;
تعريفنوع بنية kdev_s *kdev_t;

بنية میانگیر_سر;
بنية inode;

تعريفنوع بیعلامت صحيح gfp_t;
#تعریف_کن GFP_KERNEL	0
#تعریف_کن GFP_NOFS	0
#تعریف_کن __GFP_NOFAIL	0
#تعریف_کن JBD2_برچسب_SIZE32	JBD_برچسب_SIZE32
#تعریف_کن JBD2_BARRIER	0
تعريفنوع __u64 u64;
#تعریف_کن put_bh(x)	brelse(x)

#تعریف_کن بررسی_چرخهای_32_be(x, y, z)	ext2fs_بررسی_چرخهای_32_be((x), (y), (z))
#تعریف_کن spin_قفل_init(x)
#تعریف_کن spin_قفل(x)
#تعریف_کن spin_unlock(x)
#تعریف_کن SLAB_HWCACHE_ALIGN	0
#تعریف_کن SLAB_TEMPORARY		0
#تعریف_کن KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                حجم(بنية __struct), __alignof__(بنية __struct),\
                (__flags), عدم)

#تعریف_کن blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#تعریف_کن is_power_از_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#تعریف_کن roundup_power_از_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#تعریف_کن pr_emerg(fmt)
#تعریف_کن pr_err(...)

تعداد passtype {PASS_پیمایش, PASS_REVOKE, PASS_REPLAY};

#تعریف_کن JBD2_FC_REPLAY_STOP		0
#تعریف_کن JBD2_FC_REPLAY_CONTINUE		1

بنية journal_s
{
	بیعلامت دراز		j_flags;
	صحيح			j_errno;
	بنية میانگیر_سر *	j_sb_میانگیر;
	بنية journal_superblock_s *j_superblock;
	صحيح			j_format_نسخه;
	بیعلامت دراز		j_سر;
	بیعلامت دراز		j_دم;
	بیعلامت دراز		j_fc_اولی;
	بیعلامت دراز		j_fc_off;
	بیعلامت دراز		j_fc_آخرین;
	بیعلامت دراز		j_free;
	بیعلامت دراز		j_اولی, j_آخرین;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	صحيح			j_blocksize;
	بیعلامت صحيح		j_blk_جابه_جایی;
	بیعلامت صحيح		j_مجموع_len;
	بنية inode *		j_inode;
	tid_t			j_دم_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	بنية jbd2_revoke_جدول_s *j_revoke;
	بنية jbd2_revoke_جدول_s *j_revoke_جدول[2];
	tid_t			j_failed_commit;
	__u32			j_csum_دانه;
	صحيح (*j_fc_replay_callback)(بنية journal_s *journal,
				    بنية میانگیر_سر *bh,
				    تعداد passtype pass, صحيح off,
				    tid_t expected_tid);

};

#تعریف_کن is_journal_abort(x) 0

#تعریف_کن میانگیر_TRACE(bh, info)	افعل {} طالما (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#اگر_تعریف_شده NO_INLINE_FUNCS
#تعریف_کن مضمن
#پایان_اگر

#پایان_اگر /* _JFS_COMPAT_H */
