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

#شامل "config.h"
#شامل <string.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

#define EXT4_MAX_ORPHAN_FILE_BLOCKS 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	ساختار ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	اگر (err)
		بازگشت err;

	err = ext2fs_punch(fs, ino, &inode, پوچ, 0, ~0ULL);
	اگر (err)
		بازگشت err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, اندازه(ساختار ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	بازگشت err;
}

__u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, نویسه *buf)
{
	صحیح inodes_per_ob = ext2fs_inodes_per_orphan_block(fs);
	__u32 crc;

	ino = ext2fs_cpu_to_le32(ino);
	gen = ext2fs_cpu_to_le32(gen);
	blk = ext2fs_cpu_to_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned نویسه *)&ino,
			       اندازه(ino));
	crc = ext2fs_crc32c_le(crc, (unsigned نویسه *)&gen, اندازه(gen));
	crc = ext2fs_crc32c_le(crc, (unsigned نویسه *)&blk, اندازه(blk));
	crc = ext2fs_crc32c_le(crc, (unsigned نویسه *)buf,
				inodes_per_ob * اندازه(__u32));

	بازگشت crc;
}

ساختار mkorphan_info {
	نویسه *buf;
	نویسه *zerobuf;
	blk_t رقم_blocks;
	blk_t alloc_blocks;
	blk64_t last_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ایستا صحیح mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_block EXT2FS_ATTR((unused)),
			 صحیح		ref_offset EXT2FS_ATTR((unused)),
			 تهی		*priv_data)
{
	ساختار mkorphan_info *oi = (ساختار mkorphan_info *)priv_data;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	اگر (blockcnt &&
	    EXT2FS_B2C(fs, oi->last_blk) == EXT2FS_B2C(fs, oi->last_blk + 1)) {
		new_blk = oi->last_blk + 1;
	} وگرنه {
		err = ext2fs_new_block2(fs, oi->last_blk, 0, &new_blk);
		اگر (err) {
			oi->err = err;
			بازگشت BLOCK_ABORT;
		}
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
		oi->alloc_blocks++;
	}
	اگر (blockcnt >= 0) {
		اگر (ext2fs_has_feature_metadata_csum(fs->super)) {
			ساختار ext4_orphan_block_tail *tail;

			tail = ext2fs_orphan_block_tail(fs, oi->buf);
			tail->ob_checksum =
		ext2fs_cpu_to_le32(ext2fs_do_orphan_file_block_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} وگرنه	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	اگر (err) {
		oi->err = err;
		بازگشت BLOCK_ABORT;
	}
	oi->last_blk = new_blk;
	*blocknr = new_blk;
	اگر (blockcnt >= 0 && --oi->رقم_blocks == 0)
		بازگشت BLOCK_CHANGED | BLOCK_ABORT;
	بازگشت BLOCK_CHANGED;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t رقم_blocks)
{
	ساختار ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	نویسه *buf = پوچ, *zerobuf = پوچ;
	ساختار mkorphan_info oi;
	ساختار ext4_orphan_block_tail *ob_tail;
	time_t now;

	اگر (رقم_blocks > EXT4_MAX_ORPHAN_FILE_BLOCKS)
		رقم_blocks = EXT4_MAX_ORPHAN_FILE_BLOCKS;

	اگر (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		اگر (err)
			بازگشت err;
		اگر (EXT2_I_SIZE(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			اگر (err)
				بازگشت err;
		}
	} وگرنه {
		err = ext2fs_new_inode(fs, EXT2_ریشه_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		اگر (err)
			بازگشت err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, اندازه(ساختار ext2_inode));
	اگر (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		اگر (err)
			بازگشت err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (err)
		بازگشت err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	اگر (err)
		برو out;
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
	اگر (err)
		برو out;
	اگر (oi.err) {
		err = oi.err;
		برو out;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	اگر (err)
		برو out;
	ext2fs_iblk_set(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	inode.i_links_شمار = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	ext2fs_iblk_add_blocks(fs, &inode, oi.alloc_blocks);
	err = ext2fs_inode_size_set(fs, &inode,
			(unsigned long long)fs->blocksize * رقم_blocks);
	اگر (err)
		برو out;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	اگر (err)
		برو out;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_set_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
out:
	اگر (buf)
		ext2fs_free_mem(&buf);
	اگر (zerobuf)
		ext2fs_free_mem(&zerobuf);
	بازگشت err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs)
{
	__u64 رقم_blocks = ext2fs_blocks_شمار(fs->super);
	e2_blkcnt_t blks = 512;

	اگر (رقم_blocks < 128 * 1024)
		blks = 32;
	وگرنه اگر (رقم_blocks < 2 * 1024 * 1024)
		blks = رقم_blocks / 4096;
	بازگشت (blks + EXT2FS_CLUSTER_MASK(fs)) & ~EXT2FS_CLUSTER_MASK(fs);
}

ایستا errcode_t ext2fs_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, نویسه *buf,
					       __u32 *crcp)
{
	ساختار ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		بازگشت retval;
	*crcp = ext2fs_do_orphan_file_block_csum(fs, ino, inode.i_generation,
						 blk, buf);
	بازگشت 0;
}

errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, نویسه *buf)
{
	ساختار ext4_orphan_block_tail *tail;
	errcode_t ret;
	__u32 crc = 0;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 0;

	tail = ext2fs_orphan_block_tail(fs, buf);
	ret = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	اگر (ret)
		بازگشت 0;
	tail->ob_checksum = ext2fs_cpu_to_le32(crc);
	بازگشت ret;
}

صحیح ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, نویسه *buf)
{
	ساختار ext4_orphan_block_tail *tail;
	__u32 crc;
	errcode_t retval;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		بازگشت 1;
	retval = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	اگر (retval)
		بازگشت 0;
	tail = ext2fs_orphan_block_tail(fs, buf);
	بازگشت ext2fs_le32_to_cpu(tail->ob_checksum) == crc;
}
