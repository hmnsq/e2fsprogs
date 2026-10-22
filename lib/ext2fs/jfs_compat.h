
#यदि_अपरिभाषितम् _JFS_COMPAT_H
#परिभाषय _JFS_COMPAT_H

#अन्तर्भाव "support/list.h"
#अन्तर्भाव <errno.h>
#यदि_परिभाषितम् HAVE_NETINET_अन्तर्_H
#अन्तर्भाव <netinet/in.h>
#यदि_अन्तः
#अन्तर्भाव <arpa/inet.h>
#अन्तर्भाव <stdbool.h>

#परिभाषय printk प्रारूप_लिख
#परिभाषय KERN_ERR ""
#परिभाषय KERN_DEBUG ""

#परिभाषय REQ_OP_READ 0
#परिभाषय REQ_OP_WRITE 1

#परिभाषय cpu_गन्तव्यम्_le16(x)	ext2fs_cpu_गन्तव्यम्_le16(x)
#परिभाषय cpu_गन्तव्यम्_be16(x)	ext2fs_cpu_गन्तव्यम्_be16(x)
#परिभाषय cpu_गन्तव्यम्_le32(x)	ext2fs_cpu_गन्तव्यम्_le32(x)
#परिभाषय cpu_गन्तव्यम्_be32(x)	ext2fs_cpu_गन्तव्यम्_be32(x)
#परिभाषय cpu_गन्तव्यम्_le64(x)	ext2fs_cpu_गन्तव्यम्_le64(x)
#परिभाषय cpu_गन्तव्यम्_be64(x)	ext2fs_cpu_गन्तव्यम्_be64(x)

#परिभाषय le16_गन्तव्यम्_cpu(x)	ext2fs_le16_गन्तव्यम्_cpu(x)
#परिभाषय be16_गन्तव्यम्_cpu(x)	ext2fs_be16_गन्तव्यम्_cpu(x)
#परिभाषय le32_गन्तव्यम्_cpu(x)	ext2fs_le32_गन्तव्यम्_cpu(x)
#परिभाषय be32_गन्तव्यम्_cpu(x)	ext2fs_be32_गन्तव्यम्_cpu(x)
#परिभाषय le64_गन्तव्यम्_cpu(x)	ext2fs_le64_गन्तव्यम्_cpu(x)
#परिभाषय be64_गन्तव्यम्_cpu(x)	ext2fs_be64_गन्तव्यम्_cpu(x)

प्रकारनिर्देश अचिह्नित पूर्णाङ्क tid_t;
प्रकारनिर्देश संरचना journal_s journal_t;
प्रकारनिर्देश संरचना kdev_s *kdev_t;

संरचना बफर_शीर्ष;
संरचना inode;

प्रकारनिर्देश अचिह्नित पूर्णाङ्क gfp_t;
#परिभाषय GFP_KERNEL	0
#परिभाषय GFP_NOFS	0
#परिभाषय __GFP_NOFAIL	0
#परिभाषय JBD2_चिह्नकम्_SIZE32	JBD_चिह्नकम्_SIZE32
#परिभाषय JBD2_BARRIER	0
प्रकारनिर्देश __u64 u64;
#परिभाषय put_bh(x)	brelse(x)

#परिभाषय चक्रीयपरीक्षा_32_be(x, y, z)	ext2fs_चक्रीयपरीक्षा_32_be((x), (y), (z))
#परिभाषय spin_तालकम्_init(x)
#परिभाषय spin_तालकम्(x)
#परिभाषय spin_unlock(x)
#परिभाषय SLAB_HWCACHE_ALIGN	0
#परिभाषय SLAB_TEMPORARY		0
#परिभाषय KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                परिमाण(संरचना __struct), __alignof__(संरचना __struct),\
                (__flags), रिक्त)

#परिभाषय blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#परिभाषय is_power_सम्बन्धि_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#परिभाषय roundup_power_सम्बन्धि_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#परिभाषय pr_emerg(fmt)
#परिभाषय pr_err(...)

गणना passtype {PASS_अवलोकनम्, PASS_REVOKE, PASS_REPLAY};

#परिभाषय JBD2_FC_REPLAY_STOP		0
#परिभाषय JBD2_FC_REPLAY_CONTINUE		1

संरचना journal_s
{
	अचिह्नित दीर्घ		j_flags;
	पूर्णाङ्क			j_errno;
	संरचना बफर_शीर्ष *	j_sb_बफर;
	संरचना journal_superblock_s *j_superblock;
	पूर्णाङ्क			j_format_संस्करणम्;
	अचिह्नित दीर्घ		j_शीर्ष;
	अचिह्नित दीर्घ		j_पुच्छ;
	अचिह्नित दीर्घ		j_fc_first;
	अचिह्नित दीर्घ		j_fc_off;
	अचिह्नित दीर्घ		j_fc_चरमः;
	अचिह्नित दीर्घ		j_free;
	अचिह्नित दीर्घ		j_first, j_चरमः;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	पूर्णाङ्क			j_blocksize;
	अचिह्नित पूर्णाङ्क		j_blk_विस्थापनम्;
	अचिह्नित पूर्णाङ्क		j_योग_len;
	संरचना inode *		j_inode;
	tid_t			j_पुच्छ_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	संरचना jbd2_revoke_सारणी_s *j_revoke;
	संरचना jbd2_revoke_सारणी_s *j_revoke_सारणी[2];
	tid_t			j_failed_commit;
	__u32			j_csum_बीजम्;
	पूर्णाङ्क (*j_fc_replay_callback)(संरचना journal_s *journal,
				    संरचना बफर_शीर्ष *bh,
				    गणना passtype pass, पूर्णाङ्क off,
				    tid_t expected_tid);

};

#परिभाषय is_journal_abort(x) 0

#परिभाषय बफर_TRACE(bh, info)	कुरु {} यावत् (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#यदि_परिभाषितम् NO_INLINE_FUNCS
#परिभाषय अन्तःस्थ
#यदि_अन्तः

#यदि_अन्तः /* _JFS_COMPAT_H */
