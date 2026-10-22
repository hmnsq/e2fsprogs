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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"

#define EXT4_MAX_ORPHAN_FILE_BLOCKS 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	સંરચના ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	જો (err)
		પાછા err;

	err = ext2fs_punch(fs, ino, &inode, ખાલી, 0, ~0ULL);
	જો (err)
		પાછા err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, કદ(સંરચના ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	પાછા err;
}

__u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, અક્ષર *buf)
{
	પૂર્ણાંક inodes_per_ob = ext2fs_inodes_per_orphan_block(fs);
	__u32 crc;

	ino = ext2fs_cpu_to_le32(ino);
	gen = ext2fs_cpu_to_le32(gen);
	blk = ext2fs_cpu_to_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)&ino,
			       કદ(ino));
	crc = ext2fs_crc32c_le(crc, (unsigned અક્ષર *)&gen, કદ(gen));
	crc = ext2fs_crc32c_le(crc, (unsigned અક્ષર *)&blk, કદ(blk));
	crc = ext2fs_crc32c_le(crc, (unsigned અક્ષર *)buf,
				inodes_per_ob * કદ(__u32));

	પાછા crc;
}

સંરચના mkorphan_info {
	અક્ષર *buf;
	અક્ષર *zerobuf;
	blk_t અંક_blocks;
	blk_t alloc_blocks;
	blk64_t last_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

સ્થિર પૂર્ણાંક mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_block EXT2FS_ATTR((unused)),
			 પૂર્ણાંક		ref_offset EXT2FS_ATTR((unused)),
			 શૂન્ય		*priv_data)
{
	સંરચના mkorphan_info *oi = (સંરચના mkorphan_info *)priv_data;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	જો (blockcnt &&
	    EXT2FS_B2C(fs, oi->last_blk) == EXT2FS_B2C(fs, oi->last_blk + 1)) {
		new_blk = oi->last_blk + 1;
	} નહીંતર {
		err = ext2fs_new_block2(fs, oi->last_blk, 0, &new_blk);
		જો (err) {
			oi->err = err;
			પાછા BLOCK_ABORT;
		}
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
		oi->alloc_blocks++;
	}
	જો (blockcnt >= 0) {
		જો (ext2fs_has_feature_metadata_csum(fs->super)) {
			સંરચના ext4_orphan_block_tail *tail;

			tail = ext2fs_orphan_block_tail(fs, oi->buf);
			tail->ob_checksum =
		ext2fs_cpu_to_le32(ext2fs_do_orphan_file_block_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} નહીંતર	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	જો (err) {
		oi->err = err;
		પાછા BLOCK_ABORT;
	}
	oi->last_blk = new_blk;
	*blocknr = new_blk;
	જો (blockcnt >= 0 && --oi->અંક_blocks == 0)
		પાછા BLOCK_CHANGED | BLOCK_ABORT;
	પાછા BLOCK_CHANGED;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t અંક_blocks)
{
	સંરચના ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	અક્ષર *buf = ખાલી, *zerobuf = ખાલી;
	સંરચના mkorphan_info oi;
	સંરચના ext4_orphan_block_tail *ob_tail;
	time_t now;

	જો (અંક_blocks > EXT4_MAX_ORPHAN_FILE_BLOCKS)
		અંક_blocks = EXT4_MAX_ORPHAN_FILE_BLOCKS;

	જો (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		જો (err)
			પાછા err;
		જો (EXT2_I_SIZE(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			જો (err)
				પાછા err;
		}
	} નહીંતર {
		err = ext2fs_new_inode(fs, EXT2_મૂળ_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		જો (err)
			પાછા err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, કદ(સંરચના ext2_inode));
	જો (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		જો (err)
			પાછા err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	જો (err)
		પાછા err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	જો (err)
		જાઓ out;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_tail = ext2fs_orphan_block_tail(fs, buf);
	ob_tail->ob_magic = ext2fs_cpu_to_le32(EXT4_ORPHAN_BLOCK_MAGIC);
	oi.અંક_blocks = અંક_blocks;
	oi.alloc_blocks = 0;
	oi.last_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_block_iterate3(fs, ino, BLOCK_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	જો (err)
		જાઓ out;
	જો (oi.err) {
		err = oi.err;
		જાઓ out;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	જો (err)
		જાઓ out;
	ext2fs_iblk_set(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	inode.i_links_ગણતરી = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	ext2fs_iblk_add_blocks(fs, &inode, oi.alloc_blocks);
	err = ext2fs_inode_size_set(fs, &inode,
			(unsigned long long)fs->blocksize * અંક_blocks);
	જો (err)
		જાઓ out;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	જો (err)
		જાઓ out;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_set_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
out:
	જો (buf)
		ext2fs_free_mem(&buf);
	જો (zerobuf)
		ext2fs_free_mem(&zerobuf);
	પાછા err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs)
{
	__u64 અંક_blocks = ext2fs_blocks_ગણતરી(fs->super);
	e2_blkcnt_t blks = 512;

	જો (અંક_blocks < 128 * 1024)
		blks = 32;
	નહીંતર જો (અંક_blocks < 2 * 1024 * 1024)
		blks = અંક_blocks / 4096;
	પાછા (blks + EXT2FS_CLUSTER_MASK(fs)) & ~EXT2FS_CLUSTER_MASK(fs);
}

સ્થિર errcode_t ext2fs_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, અક્ષર *buf,
					       __u32 *crcp)
{
	સંરચના ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	જો (retval)
		પાછા retval;
	*crcp = ext2fs_do_orphan_file_block_csum(fs, ino, inode.i_generation,
						 blk, buf);
	પાછા 0;
}

errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, અક્ષર *buf)
{
	સંરચના ext4_orphan_block_tail *tail;
	errcode_t ret;
	__u32 crc = 0;

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 0;

	tail = ext2fs_orphan_block_tail(fs, buf);
	ret = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	જો (ret)
		પાછા 0;
	tail->ob_checksum = ext2fs_cpu_to_le32(crc);
	પાછા ret;
}

પૂર્ણાંક ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, અક્ષર *buf)
{
	સંરચના ext4_orphan_block_tail *tail;
	__u32 crc;
	errcode_t retval;

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 1;
	retval = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	જો (retval)
		પાછા 0;
	tail = ext2fs_orphan_block_tail(fs, buf);
	પાછા ext2fs_le32_to_cpu(tail->ob_checksum) == crc;
}
