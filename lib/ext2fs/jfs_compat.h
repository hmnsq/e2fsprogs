
#جے_بنا_تعریف _JFS_COMPAT_H
#تعریف_کرو _JFS_COMPAT_H

#ضمن "support/list.h"
#ضمن <errno.h>
#جے_تعریف_شدہ HAVE_NETINET_وچ_H
#ضمن <netinet/in.h>
#جے_انت
#ضمن <arpa/inet.h>
#ضمن <stdbool.h>

#تعریف_کرو printk لکھو
#تعریف_کرو KERN_ERR ""
#تعریف_کرو KERN_DEBUG ""

#تعریف_کرو REQ_OP_READ 0
#تعریف_کرو REQ_OP_WRITE 1

#تعریف_کرو cpu_منزل_le16(x)	ext2fs_cpu_منزل_le16(x)
#تعریف_کرو cpu_منزل_be16(x)	ext2fs_cpu_منزل_be16(x)
#تعریف_کرو cpu_منزل_le32(x)	ext2fs_cpu_منزل_le32(x)
#تعریف_کرو cpu_منزل_be32(x)	ext2fs_cpu_منزل_be32(x)
#تعریف_کرو cpu_منزل_le64(x)	ext2fs_cpu_منزل_le64(x)
#تعریف_کرو cpu_منزل_be64(x)	ext2fs_cpu_منزل_be64(x)

#تعریف_کرو le16_منزل_cpu(x)	ext2fs_le16_منزل_cpu(x)
#تعریف_کرو be16_منزل_cpu(x)	ext2fs_be16_منزل_cpu(x)
#تعریف_کرو le32_منزل_cpu(x)	ext2fs_le32_منزل_cpu(x)
#تعریف_کرو be32_منزل_cpu(x)	ext2fs_be32_منزل_cpu(x)
#تعریف_کرو le64_منزل_cpu(x)	ext2fs_le64_منزل_cpu(x)
#تعریف_کرو be64_منزل_cpu(x)	ext2fs_be64_منزل_cpu(x)

تعريفنوع بنانشانی صحيح tid_t;
تعريفنوع بنية journal_s journal_t;
تعريفنوع بنية kdev_s *kdev_t;

بنية بفر_سر;
بنية inode;

تعريفنوع بنانشانی صحيح gfp_t;
#تعریف_کرو GFP_KERNEL	0
#تعریف_کرو GFP_NOFS	0
#تعریف_کرو __GFP_NOFAIL	0
#تعریف_کرو JBD2_ٹیگ_SIZE32	JBD_ٹیگ_SIZE32
#تعریف_کرو JBD2_BARRIER	0
تعريفنوع __u64 u64;
#تعریف_کرو put_bh(x)	brelse(x)

#تعریف_کرو چکری_جانچ_32_be(x, y, z)	ext2fs_چکری_جانچ_32_be((x), (y), (z))
#تعریف_کرو spin_جندرا_init(x)
#تعریف_کرو spin_جندرا(x)
#تعریف_کرو spin_unlock(x)
#تعریف_کرو SLAB_HWCACHE_ALIGN	0
#تعریف_کرو SLAB_TEMPORARY		0
#تعریف_کرو KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                حجم(بنية __struct), __alignof__(بنية __struct),\
                (__flags), عدم)

#تعریف_کرو blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#تعریف_کرو is_power_دا_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#تعریف_کرو roundup_power_دا_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#تعریف_کرو pr_emerg(fmt)
#تعریف_کرو pr_err(...)

تعداد passtype {PASS_جانچ, PASS_REVOKE, PASS_REPLAY};

#تعریف_کرو JBD2_FC_REPLAY_STOP		0
#تعریف_کرو JBD2_FC_REPLAY_CONTINUE		1

بنية journal_s
{
	بنانشانی لما		j_flags;
	صحيح			j_errno;
	بنية بفر_سر *	j_sb_بفر;
	بنية journal_superblock_s *j_superblock;
	صحيح			j_format_نسخہ;
	بنانشانی لما		j_سر;
	بنانشانی لما		j_پوچھ;
	بنانشانی لما		j_fc_پہلا;
	بنانشانی لما		j_fc_off;
	بنانشانی لما		j_fc_آخری;
	بنانشانی لما		j_free;
	بنانشانی لما		j_پہلا, j_آخری;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	صحيح			j_blocksize;
	بنانشانی صحيح		j_blk_وتھ;
	بنانشانی صحيح		j_کل_len;
	بنية inode *		j_inode;
	tid_t			j_پوچھ_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	بنية jbd2_revoke_جدول_s *j_revoke;
	بنية jbd2_revoke_جدول_s *j_revoke_جدول[2];
	tid_t			j_failed_commit;
	__u32			j_csum_بیج;
	صحيح (*j_fc_replay_callback)(بنية journal_s *journal,
				    بنية بفر_سر *bh,
				    تعداد passtype pass, صحيح off,
				    tid_t expected_tid);

};

#تعریف_کرو is_journal_abort(x) 0

#تعریف_کرو بفر_TRACE(bh, info)	افعل {} طالما (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#جے_تعریف_شدہ NO_INLINE_FUNCS
#تعریف_کرو مضمن
#جے_انت

#جے_انت /* _JFS_COMPAT_H */
