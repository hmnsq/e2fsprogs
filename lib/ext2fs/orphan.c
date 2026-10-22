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

#ضمن "config.h"
#ضمن <string.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#define EXT4_MAX_ORPHAN_FILE_BLOCKS 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	بنية ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	إذا (err)
		إرجاع err;

	err = ext2fs_punch(fs, ino, &inode, عدم, 0, ~0ULL);
	إذا (err)
		إرجاع err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, حجم(بنية ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	إرجاع err;
}

__u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, حرف *buf)
{
	صحيح inodes_per_ob = ext2fs_inodes_per_orphan_block(fs);
	__u32 crc;

	ino = ext2fs_cpu_to_le32(ino);
	gen = ext2fs_cpu_to_le32(gen);
	blk = ext2fs_cpu_to_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned حرف *)&ino,
			       حجم(ino));
	crc = ext2fs_crc32c_le(crc, (unsigned حرف *)&gen, حجم(gen));
	crc = ext2fs_crc32c_le(crc, (unsigned حرف *)&blk, حجم(blk));
	crc = ext2fs_crc32c_le(crc, (unsigned حرف *)buf,
				inodes_per_ob * حجم(__u32));

	إرجاع crc;
}

بنية mkorphan_info {
	حرف *buf;
	حرف *zerobuf;
	blk_t رقم_blocks;
	blk_t alloc_blocks;
	blk64_t last_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ساكن صحيح mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_block EXT2FS_ATTR((unused)),
			 صحيح		ref_offset EXT2FS_ATTR((unused)),
			 فراغ		*priv_data)
{
	بنية mkorphan_info *oi = (بنية mkorphan_info *)priv_data;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	إذا (blockcnt &&
	    EXT2FS_B2C(fs, oi->last_blk) == EXT2FS_B2C(fs, oi->last_blk + 1)) {
		new_blk = oi->last_blk + 1;
	} وإلا {
		err = ext2fs_new_block2(fs, oi->last_blk, 0, &new_blk);
		إذا (err) {
			oi->err = err;
			إرجاع BLOCK_ABORT;
		}
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
		oi->alloc_blocks++;
	}
	إذا (blockcnt >= 0) {
		إذا (ext2fs_has_feature_metadata_csum(fs->super)) {
			بنية ext4_orphan_block_tail *tail;

			tail = ext2fs_orphan_block_tail(fs, oi->buf);
			tail->ob_checksum =
		ext2fs_cpu_to_le32(ext2fs_do_orphan_file_block_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} وإلا	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	إذا (err) {
		oi->err = err;
		إرجاع BLOCK_ABORT;
	}
	oi->last_blk = new_blk;
	*blocknr = new_blk;
	إذا (blockcnt >= 0 && --oi->رقم_blocks == 0)
		إرجاع BLOCK_CHANGED | BLOCK_ABORT;
	إرجاع BLOCK_CHANGED;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t رقم_blocks)
{
	بنية ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	حرف *buf = عدم, *zerobuf = عدم;
	بنية mkorphan_info oi;
	بنية ext4_orphan_block_tail *ob_tail;
	time_t now;

	إذا (رقم_blocks > EXT4_MAX_ORPHAN_FILE_BLOCKS)
		رقم_blocks = EXT4_MAX_ORPHAN_FILE_BLOCKS;

	إذا (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		إذا (err)
			إرجاع err;
		إذا (EXT2_I_SIZE(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			إذا (err)
				إرجاع err;
		}
	} وإلا {
		err = ext2fs_new_inode(fs, EXT2_جذر_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		إذا (err)
			إرجاع err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, حجم(بنية ext2_inode));
	إذا (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		إذا (err)
			إرجاع err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (err)
		إرجاع err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	إذا (err)
		اذهب out;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_tail = ext2fs_orphan_block_tail(fs, buf);
	ob_tail->ob_magic = ext2fs_cpu_to_le32(EXT4_ORPHAN_BLOCK_MAGIC);
	oi.رقم_blocks = رقم_blocks;
	oi.alloc_blocks = 0;
	oi.last_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_block_iterate3(fs, ino, BLOCK_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	إذا (err)
		اذهب out;
	إذا (oi.err) {
		err = oi.err;
		اذهب out;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	إذا (err)
		اذهب out;
	ext2fs_iblk_set(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	inode.i_links_تعداد = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	ext2fs_iblk_add_blocks(fs, &inode, oi.alloc_blocks);
	err = ext2fs_inode_size_set(fs, &inode,
			(unsigned long long)fs->blocksize * رقم_blocks);
	إذا (err)
		اذهب out;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (err)
		اذهب out;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_set_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
out:
	إذا (buf)
		ext2fs_free_mem(&buf);
	إذا (zerobuf)
		ext2fs_free_mem(&zerobuf);
	إرجاع err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs)
{
	__u64 رقم_blocks = ext2fs_blocks_تعداد(fs->super);
	e2_blkcnt_t blks = 512;

	إذا (رقم_blocks < 128 * 1024)
		blks = 32;
	وإلا إذا (رقم_blocks < 2 * 1024 * 1024)
		blks = رقم_blocks / 4096;
	إرجاع (blks + EXT2FS_CLUSTER_MASK(fs)) & ~EXT2FS_CLUSTER_MASK(fs);
}

ساكن errcode_t ext2fs_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, حرف *buf,
					       __u32 *crcp)
{
	بنية ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		إرجاع retval;
	*crcp = ext2fs_do_orphan_file_block_csum(fs, ino, inode.i_generation,
						 blk, buf);
	إرجاع 0;
}

errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, حرف *buf)
{
	بنية ext4_orphan_block_tail *tail;
	errcode_t ret;
	__u32 crc = 0;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		إرجاع 0;

	tail = ext2fs_orphan_block_tail(fs, buf);
	ret = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	إذا (ret)
		إرجاع 0;
	tail->ob_checksum = ext2fs_cpu_to_le32(crc);
	إرجاع ret;
}

صحيح ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, حرف *buf)
{
	بنية ext4_orphan_block_tail *tail;
	__u32 crc;
	errcode_t retval;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		إرجاع 1;
	retval = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	إذا (retval)
		إرجاع 0;
	tail = ext2fs_orphan_block_tail(fs, buf);
	إرجاع ext2fs_le32_to_cpu(tail->ob_checksum) == crc;
}
