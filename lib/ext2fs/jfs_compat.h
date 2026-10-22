
#که_نه_تعریف_شوی _JFS_COMPAT_H
#تعریف_کړه _JFS_COMPAT_H

#ضمن "support/list.h"
#ضمن <errno.h>
#که_تعریف_شوی HAVE_NETINET_دننه_H
#ضمن <netinet/in.h>
#د_که_پای
#ضمن <arpa/inet.h>
#ضمن <stdbool.h>

#تعریف_کړه printk چاپ_کړه
#تعریف_کړه KERN_ERR ""
#تعریف_کړه KERN_DEBUG ""

#تعریف_کړه REQ_OP_READ 0
#تعریف_کړه REQ_OP_WRITE 1

#تعریف_کړه cpu_منزل_le16(x)	ext2fs_cpu_منزل_le16(x)
#تعریف_کړه cpu_منزل_be16(x)	ext2fs_cpu_منزل_be16(x)
#تعریف_کړه cpu_منزل_le32(x)	ext2fs_cpu_منزل_le32(x)
#تعریف_کړه cpu_منزل_be32(x)	ext2fs_cpu_منزل_be32(x)
#تعریف_کړه cpu_منزل_le64(x)	ext2fs_cpu_منزل_le64(x)
#تعریف_کړه cpu_منزل_be64(x)	ext2fs_cpu_منزل_be64(x)

#تعریف_کړه le16_منزل_cpu(x)	ext2fs_le16_منزل_cpu(x)
#تعریف_کړه be16_منزل_cpu(x)	ext2fs_be16_منزل_cpu(x)
#تعریف_کړه le32_منزل_cpu(x)	ext2fs_le32_منزل_cpu(x)
#تعریف_کړه be32_منزل_cpu(x)	ext2fs_be32_منزل_cpu(x)
#تعریف_کړه le64_منزل_cpu(x)	ext2fs_le64_منزل_cpu(x)
#تعریف_کړه be64_منزل_cpu(x)	ext2fs_be64_منزل_cpu(x)

تعريفنوع بینښه صحيح tid_t;
تعريفنوع بنية journal_s journal_t;
تعريفنوع بنية kdev_s *kdev_t;

بنية بفر_سر;
بنية inode;

تعريفنوع بینښه صحيح gfp_t;
#تعریف_کړه GFP_KERNEL	0
#تعریف_کړه GFP_NOFS	0
#تعریف_کړه __GFP_NOFAIL	0
#تعریف_کړه JBD2_نښان_SIZE32	JBD_نښان_SIZE32
#تعریف_کړه JBD2_BARRIER	0
تعريفنوع __u64 u64;
#تعریف_کړه put_bh(x)	brelse(x)

#تعریف_کړه دوراني_ازموینه_32_be(x, y, z)	ext2fs_دوراني_ازموینه_32_be((x), (y), (z))
#تعریف_کړه spin_قلف_init(x)
#تعریف_کړه spin_قلف(x)
#تعریف_کړه spin_unlock(x)
#تعریف_کړه SLAB_HWCACHE_ALIGN	0
#تعریف_کړه SLAB_TEMPORARY		0
#تعریف_کړه KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                حجم(بنية __struct), __alignof__(بنية __struct),\
                (__flags), عدم)

#تعریف_کړه blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#تعریف_کړه is_power_د_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#تعریف_کړه roundup_power_د_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#تعریف_کړه pr_emerg(fmt)
#تعریف_کړه pr_err(...)

تعداد passtype {PASS_لټون, PASS_REVOKE, PASS_REPLAY};

#تعریف_کړه JBD2_FC_REPLAY_STOP		0
#تعریف_کړه JBD2_FC_REPLAY_CONTINUE		1

بنية journal_s
{
	بینښه اوږد		j_flags;
	صحيح			j_errno;
	بنية بفر_سر *	j_sb_بفر;
	بنية journal_superblock_s *j_superblock;
	صحيح			j_format_نسخه;
	بینښه اوږد		j_سر;
	بینښه اوږد		j_لکۍ;
	بینښه اوږد		j_fc_لومړی;
	بینښه اوږد		j_fc_off;
	بینښه اوږد		j_fc_وروستی;
	بینښه اوږد		j_free;
	بینښه اوږد		j_لومړی, j_وروستی;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	صحيح			j_blocksize;
	بینښه صحيح		j_blk_دننه_واټن;
	بینښه صحيح		j_ټول_len;
	بنية inode *		j_inode;
	tid_t			j_لکۍ_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	بنية jbd2_revoke_جدول_s *j_revoke;
	بنية jbd2_revoke_جدول_s *j_revoke_جدول[2];
	tid_t			j_failed_commit;
	__u32			j_csum_تخم;
	صحيح (*j_fc_replay_callback)(بنية journal_s *journal,
				    بنية بفر_سر *bh,
				    تعداد passtype pass, صحيح off,
				    tid_t expected_tid);

};

#تعریف_کړه is_journal_abort(x) 0

#تعریف_کړه بفر_TRACE(bh, info)	افعل {} طالما (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#که_تعریف_شوی NO_INLINE_FUNCS
#تعریف_کړه مضمن
#د_که_پای

#د_که_پای /* _JFS_COMPAT_H */
