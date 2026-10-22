
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _JFS_COMPAT_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _JFS_COMPAT_H

#ਅਨ੍ਤਰ੍ਭਾਵ "support/list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_NETINET_ਵਿੱਚ_H
#ਅਨ੍ਤਰ੍ਭਾਵ <netinet/in.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <arpa/inet.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ printk ਲਿਖੋ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KERN_ERR ""
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KERN_DEBUG ""

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REQ_OP_READ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REQ_OP_WRITE 1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cpu_ਮੰਜ਼ਿਲ_le16(x)	ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cpu_ਮੰਜ਼ਿਲ_be16(x)	ext2fs_cpu_ਮੰਜ਼ਿਲ_be16(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cpu_ਮੰਜ਼ਿਲ_le32(x)	ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cpu_ਮੰਜ਼ਿਲ_be32(x)	ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cpu_ਮੰਜ਼ਿਲ_le64(x)	ext2fs_cpu_ਮੰਜ਼ਿਲ_le64(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cpu_ਮੰਜ਼ਿਲ_be64(x)	ext2fs_cpu_ਮੰਜ਼ਿਲ_be64(x)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ le16_ਮੰਜ਼ਿਲ_cpu(x)	ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ be16_ਮੰਜ਼ਿਲ_cpu(x)	ext2fs_be16_ਮੰਜ਼ਿਲ_cpu(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ le32_ਮੰਜ਼ਿਲ_cpu(x)	ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ be32_ਮੰਜ਼ਿਲ_cpu(x)	ext2fs_be32_ਮੰਜ਼ਿਲ_cpu(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ le64_ਮੰਜ਼ਿਲ_cpu(x)	ext2fs_le64_ਮੰਜ਼ਿਲ_cpu(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ be64_ਮੰਜ਼ਿਲ_cpu(x)	ext2fs_be64_ਮੰਜ਼ਿਲ_cpu(x)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ tid_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ journal_s journal_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ kdev_s *kdev_t;

ਬਣਤਰ ਬਫਰ_ਸਿਰ;
ਬਣਤਰ inode;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ gfp_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GFP_KERNEL	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GFP_NOFS	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __GFP_NOFAIL	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_ਟੈਗ_SIZE32	JBD_ਟੈਗ_SIZE32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_BARRIER	0
ਕਿਸਮਪਰਿਭਾਸ਼ਾ __u64 u64;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ put_bh(x)	brelse(x)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਚੱਕਰੀ_ਜਾਂਚ_32_be(x, y, z)	ext2fs_ਚੱਕਰੀ_ਜਾਂਚ_32_be((x), (y), (z))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ spin_ਤਾਲਾ_init(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ spin_ਤਾਲਾ(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ spin_unlock(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SLAB_HWCACHE_ALIGN	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SLAB_TEMPORARY		0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                ਆਕਾਰ(ਬਣਤਰ __struct), __alignof__(ਬਣਤਰ __struct),\
                (__flags), ਨਿਰਾ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ is_power_ਦਾ_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ roundup_power_ਦਾ_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pr_emerg(fmt)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pr_err(...)

ਗਿਣਤੀ passtype {PASS_ਜਾਂਚ, PASS_REVOKE, PASS_REPLAY};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FC_REPLAY_STOP		0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JBD2_FC_REPLAY_CONTINUE		1

ਬਣਤਰ journal_s
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		j_flags;
	ਪੂਰਨ			j_errno;
	ਬਣਤਰ ਬਫਰ_ਸਿਰ *	j_sb_ਬਫਰ;
	ਬਣਤਰ journal_superblock_s *j_superblock;
	ਪੂਰਨ			j_format_ਸੰਸਕਰਣ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		j_ਸਿਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		j_ਪੂਛ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		j_fc_ਪਹਿਲਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		j_fc_off;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		j_fc_ਆਖਰੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		j_free;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		j_ਪਹਿਲਾ, j_ਆਖਰੀ;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	ਪੂਰਨ			j_blocksize;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		j_blk_ਵਿੱਥ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		j_ਕੁੱਲ_len;
	ਬਣਤਰ inode *		j_inode;
	tid_t			j_ਪੂਛ_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	ਬਣਤਰ jbd2_revoke_ਸਾਰਣੀ_s *j_revoke;
	ਬਣਤਰ jbd2_revoke_ਸਾਰਣੀ_s *j_revoke_ਸਾਰਣੀ[2];
	tid_t			j_failed_commit;
	__u32			j_csum_ਬੀਜ;
	ਪੂਰਨ (*j_fc_replay_callback)(ਬਣਤਰ journal_s *journal,
				    ਬਣਤਰ ਬਫਰ_ਸਿਰ *bh,
				    ਗਿਣਤੀ passtype pass, ਪੂਰਨ off,
				    tid_t expected_tid);

};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ is_journal_abort(x) 0

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਬਫਰ_TRACE(bh, info)	ਕਰੋ {} ਜਦੋਂ (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NO_INLINE_FUNCS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* _JFS_COMPAT_H */
