
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _JFS_COMPAT_H
#ವ್ಯಾಖ್ಯಾನಿಸು _JFS_COMPAT_H

#ಅನ್ತರ್ಭಾವ "support/list.h"
#ಅನ್ತರ್ಭಾವ <errno.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_NETINET_ಒಳ_H
#ಅನ್ತರ್ಭಾವ <netinet/in.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <arpa/inet.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>

#ವ್ಯಾಖ್ಯಾನಿಸು printk ಬರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು KERN_ERR ""
#ವ್ಯಾಖ್ಯಾನಿಸು KERN_DEBUG ""

#ವ್ಯಾಖ್ಯಾನಿಸು REQ_OP_READ 0
#ವ್ಯಾಖ್ಯಾನಿಸು REQ_OP_WRITE 1

#ವ್ಯಾಖ್ಯಾನಿಸು cpu_ಗಮ್ಯ_le16(x)	ext2fs_cpu_ಗಮ್ಯ_le16(x)
#ವ್ಯಾಖ್ಯಾನಿಸು cpu_ಗಮ್ಯ_be16(x)	ext2fs_cpu_ಗಮ್ಯ_be16(x)
#ವ್ಯಾಖ್ಯಾನಿಸು cpu_ಗಮ್ಯ_le32(x)	ext2fs_cpu_ಗಮ್ಯ_le32(x)
#ವ್ಯಾಖ್ಯಾನಿಸು cpu_ಗಮ್ಯ_be32(x)	ext2fs_cpu_ಗಮ್ಯ_be32(x)
#ವ್ಯಾಖ್ಯಾನಿಸು cpu_ಗಮ್ಯ_le64(x)	ext2fs_cpu_ಗಮ್ಯ_le64(x)
#ವ್ಯಾಖ್ಯಾನಿಸು cpu_ಗಮ್ಯ_be64(x)	ext2fs_cpu_ಗಮ್ಯ_be64(x)

#ವ್ಯಾಖ್ಯಾನಿಸು le16_ಗಮ್ಯ_cpu(x)	ext2fs_le16_ಗಮ್ಯ_cpu(x)
#ವ್ಯಾಖ್ಯಾನಿಸು be16_ಗಮ್ಯ_cpu(x)	ext2fs_be16_ಗಮ್ಯ_cpu(x)
#ವ್ಯಾಖ್ಯಾನಿಸು le32_ಗಮ್ಯ_cpu(x)	ext2fs_le32_ಗಮ್ಯ_cpu(x)
#ವ್ಯಾಖ್ಯಾನಿಸು be32_ಗಮ್ಯ_cpu(x)	ext2fs_be32_ಗಮ್ಯ_cpu(x)
#ವ್ಯಾಖ್ಯಾನಿಸು le64_ಗಮ್ಯ_cpu(x)	ext2fs_le64_ಗಮ್ಯ_cpu(x)
#ವ್ಯಾಖ್ಯಾನಿಸು be64_ಗಮ್ಯ_cpu(x)	ext2fs_be64_ಗಮ್ಯ_cpu(x)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ tid_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ journal_s journal_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ kdev_s *kdev_t;

ರಚನೆ ಬಫರ್_ತಲೆ;
ರಚನೆ inode;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ gfp_t;
#ವ್ಯಾಖ್ಯಾನಿಸು GFP_KERNEL	0
#ವ್ಯಾಖ್ಯಾನಿಸು GFP_NOFS	0
#ವ್ಯಾಖ್ಯಾನಿಸು __GFP_NOFAIL	0
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_ಟ್ಯಾಗ್_SIZE32	JBD_ಟ್ಯಾಗ್_SIZE32
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_BARRIER	0
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __u64 u64;
#ವ್ಯಾಖ್ಯಾನಿಸು put_bh(x)	brelse(x)

#ವ್ಯಾಖ್ಯಾನಿಸು ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_be(x, y, z)	ext2fs_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_be((x), (y), (z))
#ವ್ಯಾಖ್ಯಾನಿಸು spin_ಬೀಗ_init(x)
#ವ್ಯಾಖ್ಯಾನಿಸು spin_ಬೀಗ(x)
#ವ್ಯಾಖ್ಯಾನಿಸು spin_unlock(x)
#ವ್ಯಾಖ್ಯಾನಿಸು SLAB_HWCACHE_ALIGN	0
#ವ್ಯಾಖ್ಯಾನಿಸು SLAB_TEMPORARY		0
#ವ್ಯಾಖ್ಯಾನಿಸು KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                ಗಾತ್ರ(ರಚನೆ __struct), __alignof__(ರಚನೆ __struct),\
                (__flags), ಖಾಲಿ)

#ವ್ಯಾಖ್ಯಾನಿಸು blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#ವ್ಯಾಖ್ಯಾನಿಸು is_power_ದ_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#ವ್ಯಾಖ್ಯಾನಿಸು roundup_power_ದ_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#ವ್ಯಾಖ್ಯಾನಿಸು pr_emerg(fmt)
#ವ್ಯಾಖ್ಯಾನಿಸು pr_err(...)

ಎಣಿಕೆ passtype {PASS_ಪರಿಶೀಲನೆ, PASS_REVOKE, PASS_REPLAY};

#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FC_REPLAY_STOP		0
#ವ್ಯಾಖ್ಯಾನಿಸು JBD2_FC_REPLAY_CONTINUE		1

ರಚನೆ journal_s
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		j_flags;
	ಪೂರ್ಣಾಂಕ			j_errno;
	ರಚನೆ ಬಫರ್_ತಲೆ *	j_sb_ಬಫರ್;
	ರಚನೆ journal_superblock_s *j_superblock;
	ಪೂರ್ಣಾಂಕ			j_format_ಆವೃತ್ತಿ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		j_ತಲೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		j_ಬಾಲ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		j_fc_ಮೊದಲ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		j_fc_off;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		j_fc_ಕೊನೆಯ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		j_free;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		j_ಮೊದಲ, j_ಕೊನೆಯ;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	ಪೂರ್ಣಾಂಕ			j_blocksize;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		j_blk_ಸ್ಥಾನಾಂತರ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		j_ಒಟ್ಟು_len;
	ರಚನೆ inode *		j_inode;
	tid_t			j_ಬಾಲ_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	ರಚನೆ jbd2_revoke_ಕೋಷ್ಟಕ_s *j_revoke;
	ರಚನೆ jbd2_revoke_ಕೋಷ್ಟಕ_s *j_revoke_ಕೋಷ್ಟಕ[2];
	tid_t			j_failed_commit;
	__u32			j_csum_ಬೀಜ;
	ಪೂರ್ಣಾಂಕ (*j_fc_replay_callback)(ರಚನೆ journal_s *journal,
				    ರಚನೆ ಬಫರ್_ತಲೆ *bh,
				    ಎಣಿಕೆ passtype pass, ಪೂರ್ಣಾಂಕ off,
				    tid_t expected_tid);

};

#ವ್ಯಾಖ್ಯಾನಿಸು is_journal_abort(x) 0

#ವ್ಯಾಖ್ಯಾನಿಸು ಬಫರ್_TRACE(bh, info)	ಮಾಡು {} ಆಗ (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NO_INLINE_FUNCS
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಾಲೊಳಗೆ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* _JFS_COMPAT_H */
