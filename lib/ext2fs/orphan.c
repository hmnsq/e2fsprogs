/*
 * orphan.c --- utility function to handle orphan file
 *
 * Copyright (C) 2015 Jan Kara.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <string.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#define EXT4_MAX_ORPHAN_FILE_BLOCKS 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	संरचना ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	सचे (err)
		पटिनिवत्त err;

	err = ext2fs_punch(fs, ino, &inode, रिक्त, 0, ~0ULL);
	सचे (err)
		पटिनिवत्त err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, परिमाण(संरचना ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	पटिनिवत्त err;
}

__u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, वर्ण *buf)
{
	पूर्णाङ्क inodes_per_ob = ext2fs_inodes_per_orphan_block(fs);
	__u32 crc;

	ino = ext2fs_cpu_to_le32(ino);
	gen = ext2fs_cpu_to_le32(gen);
	blk = ext2fs_cpu_to_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned वर्ण *)&ino,
			       परिमाण(ino));
	crc = ext2fs_crc32c_le(crc, (unsigned वर्ण *)&gen, परिमाण(gen));
	crc = ext2fs_crc32c_le(crc, (unsigned वर्ण *)&blk, परिमाण(blk));
	crc = ext2fs_crc32c_le(crc, (unsigned वर्ण *)buf,
				inodes_per_ob * परिमाण(__u32));

	पटिनिवत्त crc;
}

संरचना mkorphan_info {
	वर्ण *buf;
	वर्ण *zerobuf;
	blk_t अङ्कः_blocks;
	blk_t alloc_blocks;
	blk64_t last_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

स्थिर पूर्णाङ्क mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_block EXT2FS_ATTR((unused)),
			 पूर्णाङ्क		ref_offset EXT2FS_ATTR((unused)),
			 शून्य		*priv_data)
{
	संरचना mkorphan_info *oi = (संरचना mkorphan_info *)priv_data;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	सचे (blockcnt &&
	    EXT2FS_B2C(fs, oi->last_blk) == EXT2FS_B2C(fs, oi->last_blk + 1)) {
		new_blk = oi->last_blk + 1;
	} अञ्ञथा {
		err = ext2fs_new_block2(fs, oi->last_blk, 0, &new_blk);
		सचे (err) {
			oi->err = err;
			पटिनिवत्त BLOCK_ABORT;
		}
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
		oi->alloc_blocks++;
	}
	सचे (blockcnt >= 0) {
		सचे (ext2fs_has_feature_metadata_csum(fs->super)) {
			संरचना ext4_orphan_block_tail *tail;

			tail = ext2fs_orphan_block_tail(fs, oi->buf);
			tail->ob_checksum =
		ext2fs_cpu_to_le32(ext2fs_do_orphan_file_block_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} अञ्ञथा	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	सचे (err) {
		oi->err = err;
		पटिनिवत्त BLOCK_ABORT;
	}
	oi->last_blk = new_blk;
	*blocknr = new_blk;
	सचे (blockcnt >= 0 && --oi->अङ्कः_blocks == 0)
		पटिनिवत्त BLOCK_CHANGED | BLOCK_ABORT;
	पटिनिवत्त BLOCK_CHANGED;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t अङ्कः_blocks)
{
	संरचना ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	वर्ण *buf = रिक्त, *zerobuf = रिक्त;
	संरचना mkorphan_info oi;
	संरचना ext4_orphan_block_tail *ob_tail;
	time_t now;

	सचे (अङ्कः_blocks > EXT4_MAX_ORPHAN_FILE_BLOCKS)
		अङ्कः_blocks = EXT4_MAX_ORPHAN_FILE_BLOCKS;

	सचे (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		सचे (err)
			पटिनिवत्त err;
		सचे (EXT2_I_SIZE(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			सचे (err)
				पटिनिवत्त err;
		}
	} अञ्ञथा {
		err = ext2fs_new_inode(fs, EXT2_मूलम्_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		सचे (err)
			पटिनिवत्त err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, परिमाण(संरचना ext2_inode));
	सचे (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		सचे (err)
			पटिनिवत्त err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	सचे (err)
		पटिनिवत्त err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	सचे (err)
		गच्छ out;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_tail = ext2fs_orphan_block_tail(fs, buf);
	ob_tail->ob_magic = ext2fs_cpu_to_le32(EXT4_ORPHAN_BLOCK_MAGIC);
	oi.अङ्कः_blocks = अङ्कः_blocks;
	oi.alloc_blocks = 0;
	oi.last_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_block_iterate3(fs, ino, BLOCK_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	सचे (err)
		गच्छ out;
	सचे (oi.err) {
		err = oi.err;
		गच्छ out;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	सचे (err)
		गच्छ out;
	ext2fs_iblk_set(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	inode.i_links_गणनम् = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	ext2fs_iblk_add_blocks(fs, &inode, oi.alloc_blocks);
	err = ext2fs_inode_size_set(fs, &inode,
			(unsigned long long)fs->blocksize * अङ्कः_blocks);
	सचे (err)
		गच्छ out;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	सचे (err)
		गच्छ out;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_set_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
out:
	सचे (buf)
		ext2fs_free_mem(&buf);
	सचे (zerobuf)
		ext2fs_free_mem(&zerobuf);
	पटिनिवत्त err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs)
{
	__u64 अङ्कः_blocks = ext2fs_blocks_गणनम्(fs->super);
	e2_blkcnt_t blks = 512;

	सचे (अङ्कः_blocks < 128 * 1024)
		blks = 32;
	अञ्ञथा सचे (अङ्कः_blocks < 2 * 1024 * 1024)
		blks = अङ्कः_blocks / 4096;
	पटिनिवत्त (blks + EXT2FS_CLUSTER_MASK(fs)) & ~EXT2FS_CLUSTER_MASK(fs);
}

स्थिर errcode_t ext2fs_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, वर्ण *buf,
					       __u32 *crcp)
{
	संरचना ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	सचे (retval)
		पटिनिवत्त retval;
	*crcp = ext2fs_do_orphan_file_block_csum(fs, ino, inode.i_generation,
						 blk, buf);
	पटिनिवत्त 0;
}

errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, वर्ण *buf)
{
	संरचना ext4_orphan_block_tail *tail;
	errcode_t ret;
	__u32 crc = 0;

	सचे (!ext2fs_has_feature_metadata_csum(fs->super))
		पटिनिवत्त 0;

	tail = ext2fs_orphan_block_tail(fs, buf);
	ret = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	सचे (ret)
		पटिनिवत्त 0;
	tail->ob_checksum = ext2fs_cpu_to_le32(crc);
	पटिनिवत्त ret;
}

पूर्णाङ्क ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, वर्ण *buf)
{
	संरचना ext4_orphan_block_tail *tail;
	__u32 crc;
	errcode_t retval;

	सचे (!ext2fs_has_feature_metadata_csum(fs->super))
		पटिनिवत्त 1;
	retval = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	सचे (retval)
		पटिनिवत्त 0;
	tail = ext2fs_orphan_block_tail(fs, buf);
	पटिनिवत्त ext2fs_le32_to_cpu(tail->ob_checksum) == crc;
}
