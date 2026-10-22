
#अगर_अघोषित _JFS_COMPAT_H
#घोषणा _JFS_COMPAT_H

#समावेश "support/list.h"
#समावेश <errno.h>
#अगर_घोषित HAVE_NETINET_IN_H
#समावेश <netinet/in.h>
#पूर्ण_अगर
#समावेश <arpa/inet.h>
#समावेश <stdbool.h>

#घोषणा printk म_लिखो
#घोषणा KERN_ERR ""
#घोषणा KERN_DEBUG ""

#घोषणा REQ_OP_READ 0
#घोषणा REQ_OP_WRITE 1

#घोषणा cpu_to_le16(x)	ext2fs_cpu_to_le16(x)
#घोषणा cpu_to_be16(x)	ext2fs_cpu_to_be16(x)
#घोषणा cpu_to_le32(x)	ext2fs_cpu_to_le32(x)
#घोषणा cpu_to_be32(x)	ext2fs_cpu_to_be32(x)
#घोषणा cpu_to_le64(x)	ext2fs_cpu_to_le64(x)
#घोषणा cpu_to_be64(x)	ext2fs_cpu_to_be64(x)

#घोषणा le16_to_cpu(x)	ext2fs_le16_to_cpu(x)
#घोषणा be16_to_cpu(x)	ext2fs_be16_to_cpu(x)
#घोषणा le32_to_cpu(x)	ext2fs_le32_to_cpu(x)
#घोषणा be32_to_cpu(x)	ext2fs_be32_to_cpu(x)
#घोषणा le64_to_cpu(x)	ext2fs_le64_to_cpu(x)
#घोषणा be64_to_cpu(x)	ext2fs_be64_to_cpu(x)

प्रकार अचिन्हित पूर्णांक tid_t;
प्रकार काष्ठा journal_s journal_t;
प्रकार काष्ठा kdev_s *kdev_t;

काष्ठा बफर_शीर्ष;
काष्ठा inode;

प्रकार अचिन्हित पूर्णांक gfp_t;
#घोषणा GFP_KERNEL	0
#घोषणा GFP_NOFS	0
#घोषणा __GFP_NOFAIL	0
#घोषणा JBD2_TAG_SIZE32	JBD_TAG_SIZE32
#घोषणा JBD2_BARRIER	0
प्रकार __u64 u64;
#घोषणा put_bh(x)	brelse(x)

#घोषणा crc32_be(x, y, z)	ext2fs_crc32_be((x), (y), (z))
#घोषणा spin_lock_init(x)
#घोषणा spin_lock(x)
#घोषणा spin_unlock(x)
#घोषणा SLAB_HWCACHE_ALIGN	0
#घोषणा SLAB_TEMPORARY		0
#घोषणा KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                माप(काष्ठा __struct), __alignof__(काष्ठा __struct),\
                (__flags), शून्य)

#घोषणा blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#घोषणा is_power_of_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#घोषणा roundup_power_of_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#घोषणा pr_emerg(fmt)
#घोषणा pr_err(...)

क्रमागत passtype {PASS_SCAN, PASS_REVOKE, PASS_REPLAY};

#घोषणा JBD2_FC_REPLAY_STOP		0
#घोषणा JBD2_FC_REPLAY_CONTINUE		1

काष्ठा journal_s
{
	अचिन्हित दीर्घ		j_flags;
	पूर्णांक			j_errno;
	काष्ठा बफर_शीर्ष *	j_sb_बफर;
	काष्ठा journal_superblock_s *j_superblock;
	पूर्णांक			j_format_version;
	अचिन्हित दीर्घ		j_शीर्ष;
	अचिन्हित दीर्घ		j_पुच्छ;
	अचिन्हित दीर्घ		j_fc_first;
	अचिन्हित दीर्घ		j_fc_off;
	अचिन्हित दीर्घ		j_fc_last;
	अचिन्हित दीर्घ		j_free;
	अचिन्हित दीर्घ		j_first, j_last;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	पूर्णांक			j_blocksize;
	अचिन्हित पूर्णांक		j_blk_offset;
	अचिन्हित पूर्णांक		j_योग_len;
	काष्ठा inode *		j_inode;
	tid_t			j_पुच्छ_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	काष्ठा jbd2_revoke_table_s *j_revoke;
	काष्ठा jbd2_revoke_table_s *j_revoke_table[2];
	tid_t			j_failed_commit;
	__u32			j_csum_seed;
	पूर्णांक (*j_fc_replay_callback)(काष्ठा journal_s *journal,
				    काष्ठा बफर_शीर्ष *bh,
				    क्रमागत passtype pass, पूर्णांक off,
				    tid_t expected_tid);

};

#घोषणा is_journal_abort(x) 0

#घोषणा बफर_TRACE(bh, info)	करो {} जबतक (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#अगर_घोषित NO_INLINE_FUNCS
#घोषणा अंतरभूत
#पूर्ण_अगर

#पूर्ण_अगर /* _JFS_COMPAT_H */
