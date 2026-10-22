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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

#define EXT4_MAX_ORPHAN_FILE_BLOCKS 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	கட்டமைப்பு ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	எனில் (err)
		திரும்பு err;

	err = ext2fs_punch(fs, ino, &inode, வெற்று, 0, ~0ULL);
	எனில் (err)
		திரும்பு err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, அளவு(கட்டமைப்பு ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	திரும்பு err;
}

__u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, எழுத்து *buf)
{
	முழு inodes_per_ob = ext2fs_inodes_per_orphan_block(fs);
	__u32 crc;

	ino = ext2fs_cpu_to_le32(ino);
	gen = ext2fs_cpu_to_le32(gen);
	blk = ext2fs_cpu_to_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned எழுத்து *)&ino,
			       அளவு(ino));
	crc = ext2fs_crc32c_le(crc, (unsigned எழுத்து *)&gen, அளவு(gen));
	crc = ext2fs_crc32c_le(crc, (unsigned எழுத்து *)&blk, அளவு(blk));
	crc = ext2fs_crc32c_le(crc, (unsigned எழுத்து *)buf,
				inodes_per_ob * அளவு(__u32));

	திரும்பு crc;
}

கட்டமைப்பு mkorphan_info {
	எழுத்து *buf;
	எழுத்து *zerobuf;
	blk_t இலக்கம்_blocks;
	blk_t alloc_blocks;
	blk64_t last_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

நிலையான முழு mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_block EXT2FS_ATTR((unused)),
			 முழு		ref_offset EXT2FS_ATTR((unused)),
			 வெறுமை		*priv_data)
{
	கட்டமைப்பு mkorphan_info *oi = (கட்டமைப்பு mkorphan_info *)priv_data;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	எனில் (blockcnt &&
	    EXT2FS_B2C(fs, oi->last_blk) == EXT2FS_B2C(fs, oi->last_blk + 1)) {
		new_blk = oi->last_blk + 1;
	} இல்லையெனில் {
		err = ext2fs_new_block2(fs, oi->last_blk, 0, &new_blk);
		எனில் (err) {
			oi->err = err;
			திரும்பு BLOCK_ABORT;
		}
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
		oi->alloc_blocks++;
	}
	எனில் (blockcnt >= 0) {
		எனில் (ext2fs_has_feature_metadata_csum(fs->super)) {
			கட்டமைப்பு ext4_orphan_block_tail *tail;

			tail = ext2fs_orphan_block_tail(fs, oi->buf);
			tail->ob_checksum =
		ext2fs_cpu_to_le32(ext2fs_do_orphan_file_block_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} இல்லையெனில்	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	எனில் (err) {
		oi->err = err;
		திரும்பு BLOCK_ABORT;
	}
	oi->last_blk = new_blk;
	*blocknr = new_blk;
	எனில் (blockcnt >= 0 && --oi->இலக்கம்_blocks == 0)
		திரும்பு BLOCK_CHANGED | BLOCK_ABORT;
	திரும்பு BLOCK_CHANGED;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t இலக்கம்_blocks)
{
	கட்டமைப்பு ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	எழுத்து *buf = வெற்று, *zerobuf = வெற்று;
	கட்டமைப்பு mkorphan_info oi;
	கட்டமைப்பு ext4_orphan_block_tail *ob_tail;
	time_t now;

	எனில் (இலக்கம்_blocks > EXT4_MAX_ORPHAN_FILE_BLOCKS)
		இலக்கம்_blocks = EXT4_MAX_ORPHAN_FILE_BLOCKS;

	எனில் (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		எனில் (err)
			திரும்பு err;
		எனில் (EXT2_I_SIZE(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			எனில் (err)
				திரும்பு err;
		}
	} இல்லையெனில் {
		err = ext2fs_new_inode(fs, EXT2_வேர்_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		எனில் (err)
			திரும்பு err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, அளவு(கட்டமைப்பு ext2_inode));
	எனில் (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		எனில் (err)
			திரும்பு err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	எனில் (err)
		திரும்பு err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	எனில் (err)
		செல் out;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_tail = ext2fs_orphan_block_tail(fs, buf);
	ob_tail->ob_magic = ext2fs_cpu_to_le32(EXT4_ORPHAN_BLOCK_MAGIC);
	oi.இலக்கம்_blocks = இலக்கம்_blocks;
	oi.alloc_blocks = 0;
	oi.last_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_block_iterate3(fs, ino, BLOCK_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	எனில் (err)
		செல் out;
	எனில் (oi.err) {
		err = oi.err;
		செல் out;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	எனில் (err)
		செல் out;
	ext2fs_iblk_set(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	inode.i_links_மொத்தம் = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	ext2fs_iblk_add_blocks(fs, &inode, oi.alloc_blocks);
	err = ext2fs_inode_size_set(fs, &inode,
			(unsigned long long)fs->blocksize * இலக்கம்_blocks);
	எனில் (err)
		செல் out;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	எனில் (err)
		செல் out;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_set_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
out:
	எனில் (buf)
		ext2fs_free_mem(&buf);
	எனில் (zerobuf)
		ext2fs_free_mem(&zerobuf);
	திரும்பு err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs)
{
	__u64 இலக்கம்_blocks = ext2fs_blocks_மொத்தம்(fs->super);
	e2_blkcnt_t blks = 512;

	எனில் (இலக்கம்_blocks < 128 * 1024)
		blks = 32;
	இல்லையெனில் எனில் (இலக்கம்_blocks < 2 * 1024 * 1024)
		blks = இலக்கம்_blocks / 4096;
	திரும்பு (blks + EXT2FS_CLUSTER_MASK(fs)) & ~EXT2FS_CLUSTER_MASK(fs);
}

நிலையான errcode_t ext2fs_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, எழுத்து *buf,
					       __u32 *crcp)
{
	கட்டமைப்பு ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	எனில் (retval)
		திரும்பு retval;
	*crcp = ext2fs_do_orphan_file_block_csum(fs, ino, inode.i_generation,
						 blk, buf);
	திரும்பு 0;
}

errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, எழுத்து *buf)
{
	கட்டமைப்பு ext4_orphan_block_tail *tail;
	errcode_t ret;
	__u32 crc = 0;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 0;

	tail = ext2fs_orphan_block_tail(fs, buf);
	ret = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	எனில் (ret)
		திரும்பு 0;
	tail->ob_checksum = ext2fs_cpu_to_le32(crc);
	திரும்பு ret;
}

முழு ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, எழுத்து *buf)
{
	கட்டமைப்பு ext4_orphan_block_tail *tail;
	__u32 crc;
	errcode_t retval;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super))
		திரும்பு 1;
	retval = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	எனில் (retval)
		திரும்பு 0;
	tail = ext2fs_orphan_block_tail(fs, buf);
	திரும்பு ext2fs_le32_to_cpu(tail->ob_checksum) == crc;
}
