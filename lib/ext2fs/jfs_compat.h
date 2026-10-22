
#جيڪڏهن_وصف_نه_ٿيل _JFS_COMPAT_H
#وصف_ڪريو _JFS_COMPAT_H

#ضمن "support/list.h"
#ضمن <errno.h>
#جيڪڏهن_وصف_ٿيل HAVE_NETINET_اندريون_H
#ضمن <netinet/in.h>
#جيڪڏهن_پورو
#ضمن <arpa/inet.h>
#ضمن <stdbool.h>

#وصف_ڪريو printk لکو
#وصف_ڪريو KERN_ERR ""
#وصف_ڪريو KERN_DEBUG ""

#وصف_ڪريو REQ_OP_READ 0
#وصف_ڪريو REQ_OP_WRITE 1

#وصف_ڪريو cpu_منزل_le16(x)	ext2fs_cpu_منزل_le16(x)
#وصف_ڪريو cpu_منزل_be16(x)	ext2fs_cpu_منزل_be16(x)
#وصف_ڪريو cpu_منزل_le32(x)	ext2fs_cpu_منزل_le32(x)
#وصف_ڪريو cpu_منزل_be32(x)	ext2fs_cpu_منزل_be32(x)
#وصف_ڪريو cpu_منزل_le64(x)	ext2fs_cpu_منزل_le64(x)
#وصف_ڪريو cpu_منزل_be64(x)	ext2fs_cpu_منزل_be64(x)

#وصف_ڪريو le16_منزل_cpu(x)	ext2fs_le16_منزل_cpu(x)
#وصف_ڪريو be16_منزل_cpu(x)	ext2fs_be16_منزل_cpu(x)
#وصف_ڪريو le32_منزل_cpu(x)	ext2fs_le32_منزل_cpu(x)
#وصف_ڪريو be32_منزل_cpu(x)	ext2fs_be32_منزل_cpu(x)
#وصف_ڪريو le64_منزل_cpu(x)	ext2fs_le64_منزل_cpu(x)
#وصف_ڪريو be64_منزل_cpu(x)	ext2fs_be64_منزل_cpu(x)

تعريفنوع بنانشان صحيح tid_t;
تعريفنوع بنية journal_s journal_t;
تعريفنوع بنية kdev_s *kdev_t;

بنية بفر_سر;
بنية inode;

تعريفنوع بنانشان صحيح gfp_t;
#وصف_ڪريو GFP_KERNEL	0
#وصف_ڪريو GFP_NOFS	0
#وصف_ڪريو __GFP_NOFAIL	0
#وصف_ڪريو JBD2_ٽيگ_SIZE32	JBD_ٽيگ_SIZE32
#وصف_ڪريو JBD2_BARRIER	0
تعريفنوع __u64 u64;
#وصف_ڪريو put_bh(x)	brelse(x)

#وصف_ڪريو چڪري_جاچ_32_be(x, y, z)	ext2fs_چڪري_جاچ_32_be((x), (y), (z))
#وصف_ڪريو spin_تالو_init(x)
#وصف_ڪريو spin_تالو(x)
#وصف_ڪريو spin_unlock(x)
#وصف_ڪريو SLAB_HWCACHE_ALIGN	0
#وصف_ڪريو SLAB_TEMPORARY		0
#وصف_ڪريو KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                حجم(بنية __struct), __alignof__(بنية __struct),\
                (__flags), عدم)

#وصف_ڪريو blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#وصف_ڪريو is_power_جو_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#وصف_ڪريو roundup_power_جو_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#وصف_ڪريو pr_emerg(fmt)
#وصف_ڪريو pr_err(...)

تعداد passtype {PASS_جاچ, PASS_REVOKE, PASS_REPLAY};

#وصف_ڪريو JBD2_FC_REPLAY_STOP		0
#وصف_ڪريو JBD2_FC_REPLAY_CONTINUE		1

بنية journal_s
{
	بنانشان ڊگهو		j_flags;
	صحيح			j_errno;
	بنية بفر_سر *	j_sb_بفر;
	بنية journal_superblock_s *j_superblock;
	صحيح			j_format_نسخو;
	بنانشان ڊگهو		j_سر;
	بنانشان ڊگهو		j_پڇ;
	بنانشان ڊگهو		j_fc_پهريون;
	بنانشان ڊگهو		j_fc_off;
	بنانشان ڊگهو		j_fc_آخري;
	بنانشان ڊگهو		j_free;
	بنانشان ڊگهو		j_پهريون, j_آخري;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	صحيح			j_blocksize;
	بنانشان صحيح		j_blk_وٿي;
	بنانشان صحيح		j_ڪل_len;
	بنية inode *		j_inode;
	tid_t			j_پڇ_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	بنية jbd2_revoke_جدول_s *j_revoke;
	بنية jbd2_revoke_جدول_s *j_revoke_جدول[2];
	tid_t			j_failed_commit;
	__u32			j_csum_ٻج;
	صحيح (*j_fc_replay_callback)(بنية journal_s *journal,
				    بنية بفر_سر *bh,
				    تعداد passtype pass, صحيح off,
				    tid_t expected_tid);

};

#وصف_ڪريو is_journal_abort(x) 0

#وصف_ڪريو بفر_TRACE(bh, info)	افعل {} طالما (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#جيڪڏهن_وصف_ٿيل NO_INLINE_FUNCS
#وصف_ڪريو مضمن
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* _JFS_COMPAT_H */
