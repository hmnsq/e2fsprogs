
#నిర్వచితం_కాకపోతే _JFS_COMPAT_H
#నిర్వచించు _JFS_COMPAT_H

#అన్తర్భావ "support/list.h"
#అన్తర్భావ <errno.h>
#నిర్వచితమైతే HAVE_NETINET_లోపల_H
#అన్తర్భావ <netinet/in.h>
#అయితే_ముగింపు
#అన్తర్భావ <arpa/inet.h>
#అన్తర్భావ <stdbool.h>

#నిర్వచించు printk రాయి
#నిర్వచించు KERN_ERR ""
#నిర్వచించు KERN_DEBUG ""

#నిర్వచించు REQ_OP_READ 0
#నిర్వచించు REQ_OP_WRITE 1

#నిర్వచించు cpu_గమ్యం_le16(x)	ext2fs_cpu_గమ్యం_le16(x)
#నిర్వచించు cpu_గమ్యం_be16(x)	ext2fs_cpu_గమ్యం_be16(x)
#నిర్వచించు cpu_గమ్యం_le32(x)	ext2fs_cpu_గమ్యం_le32(x)
#నిర్వచించు cpu_గమ్యం_be32(x)	ext2fs_cpu_గమ్యం_be32(x)
#నిర్వచించు cpu_గమ్యం_le64(x)	ext2fs_cpu_గమ్యం_le64(x)
#నిర్వచించు cpu_గమ్యం_be64(x)	ext2fs_cpu_గమ్యం_be64(x)

#నిర్వచించు le16_గమ్యం_cpu(x)	ext2fs_le16_గమ్యం_cpu(x)
#నిర్వచించు be16_గమ్యం_cpu(x)	ext2fs_be16_గమ్యం_cpu(x)
#నిర్వచించు le32_గమ్యం_cpu(x)	ext2fs_le32_గమ్యం_cpu(x)
#నిర్వచించు be32_గమ్యం_cpu(x)	ext2fs_be32_గమ్యం_cpu(x)
#నిర్వచించు le64_గమ్యం_cpu(x)	ext2fs_le64_గమ్యం_cpu(x)
#నిర్వచించు be64_గమ్యం_cpu(x)	ext2fs_be64_గమ్యం_cpu(x)

రకనిర్వచనం గుర్తులేని పూర్ణాంకం tid_t;
రకనిర్వచనం నిర్మాణం journal_s journal_t;
రకనిర్వచనం నిర్మాణం kdev_s *kdev_t;

నిర్మాణం బఫరు_తల;
నిర్మాణం inode;

రకనిర్వచనం గుర్తులేని పూర్ణాంకం gfp_t;
#నిర్వచించు GFP_KERNEL	0
#నిర్వచించు GFP_NOFS	0
#నిర్వచించు __GFP_NOFAIL	0
#నిర్వచించు JBD2_ట్యాగ్_SIZE32	JBD_ట్యాగ్_SIZE32
#నిర్వచించు JBD2_BARRIER	0
రకనిర్వచనం __u64 u64;
#నిర్వచించు put_bh(x)	brelse(x)

#నిర్వచించు చక్రీయ_తనిఖీ_32_be(x, y, z)	ext2fs_చక్రీయ_తనిఖీ_32_be((x), (y), (z))
#నిర్వచించు spin_తాళం_init(x)
#నిర్వచించు spin_తాళం(x)
#నిర్వచించు spin_unlock(x)
#నిర్వచించు SLAB_HWCACHE_ALIGN	0
#నిర్వచించు SLAB_TEMPORARY		0
#నిర్వచించు KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                పరిమాణం(నిర్మాణం __struct), __alignof__(నిర్మాణం __struct),\
                (__flags), ఖాళీ)

#నిర్వచించు blkdev_issue_flush(kdev)	sync_blockdev(kdev)
#నిర్వచించు is_power_యొక్క_2(x)		((x) != 0 && (((x) & ((x) - 1)) == 0))
#నిర్వచించు roundup_power_యొక్క_two(n)		(1UL << (ext2fs_log2_u32((n) - 1) + 1))
#నిర్వచించు pr_emerg(fmt)
#నిర్వచించు pr_err(...)

గణన passtype {PASS_పరిశీలన, PASS_REVOKE, PASS_REPLAY};

#నిర్వచించు JBD2_FC_REPLAY_STOP		0
#నిర్వచించు JBD2_FC_REPLAY_CONTINUE		1

నిర్మాణం journal_s
{
	గుర్తులేని దీర్ఘ		j_flags;
	పూర్ణాంకం			j_errno;
	నిర్మాణం బఫరు_తల *	j_sb_బఫరు;
	నిర్మాణం journal_superblock_s *j_superblock;
	పూర్ణాంకం			j_format_సంస్కరణ;
	గుర్తులేని దీర్ఘ		j_తల;
	గుర్తులేని దీర్ఘ		j_తోక;
	గుర్తులేని దీర్ఘ		j_fc_మొదటి;
	గుర్తులేని దీర్ఘ		j_fc_off;
	గుర్తులేని దీర్ఘ		j_fc_చివరి;
	గుర్తులేని దీర్ఘ		j_free;
	గుర్తులేని దీర్ఘ		j_మొదటి, j_చివరి;
	kdev_t			j_dev;
	kdev_t			j_fs_dev;
	పూర్ణాంకం			j_blocksize;
	గుర్తులేని పూర్ణాంకం		j_blk_స్థానభ్రంశం;
	గుర్తులేని పూర్ణాంకం		j_మొత్తం_len;
	నిర్మాణం inode *		j_inode;
	tid_t			j_తోక_sequence;
	tid_t			j_transaction_sequence;
	__u8			j_uuid[16];
	నిర్మాణం jbd2_revoke_పట్టిక_s *j_revoke;
	నిర్మాణం jbd2_revoke_పట్టిక_s *j_revoke_పట్టిక[2];
	tid_t			j_failed_commit;
	__u32			j_csum_విత్తనం;
	పూర్ణాంకం (*j_fc_replay_callback)(నిర్మాణం journal_s *journal,
				    నిర్మాణం బఫరు_తల *bh,
				    గణన passtype pass, పూర్ణాంకం off,
				    tid_t expected_tid);

};

#నిర్వచించు is_journal_abort(x) 0

#నిర్వచించు బఫరు_TRACE(bh, info)	చేయి {} అయితే (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#నిర్వచితమైతే NO_INLINE_FUNCS
#నిర్వచించు అంతఃవరుస
#అయితే_ముగింపు

#అయితే_ముగింపు /* _JFS_COMPAT_H */
