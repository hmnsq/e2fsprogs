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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#define EXT4_MAX_ORPHAN_FILE_BLOCKS 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	ಸಂರಚನಾ ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

	err = ext2fs_punch(fs, ino, &inode, ರಿಕ್ತ, 0, ~0ULL);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	ಪ್ರತ್ಯಾಗಮ err;
}

__u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, ವರ್ಣ *buf)
{
	ಪೂರ್ಣಾಙ್ಕ inodes_per_ob = ext2fs_inodes_per_orphan_block(fs);
	__u32 crc;

	ino = ext2fs_cpu_to_le32(ino);
	gen = ext2fs_cpu_to_le32(gen);
	blk = ext2fs_cpu_to_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ವರ್ಣ *)&ino,
			       ಪರಿಮಾಣ(ino));
	crc = ext2fs_crc32c_le(crc, (unsigned ವರ್ಣ *)&gen, ಪರಿಮಾಣ(gen));
	crc = ext2fs_crc32c_le(crc, (unsigned ವರ್ಣ *)&blk, ಪರಿಮಾಣ(blk));
	crc = ext2fs_crc32c_le(crc, (unsigned ವರ್ಣ *)buf,
				inodes_per_ob * ಪರಿಮಾಣ(__u32));

	ಪ್ರತ್ಯಾಗಮ crc;
}

ಸಂರಚನಾ mkorphan_info {
	ವರ್ಣ *buf;
	ವರ್ಣ *zerobuf;
	blk_t ಅಂಕೆ_blocks;
	blk_t alloc_blocks;
	blk64_t last_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_block EXT2FS_ATTR((unused)),
			 ಪೂರ್ಣಾಙ್ಕ		ref_offset EXT2FS_ATTR((unused)),
			 ಶೂನ್ಯ		*priv_data)
{
	ಸಂರಚನಾ mkorphan_info *oi = (ಸಂರಚನಾ mkorphan_info *)priv_data;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	ಯದಿ (blockcnt &&
	    EXT2FS_B2C(fs, oi->last_blk) == EXT2FS_B2C(fs, oi->last_blk + 1)) {
		new_blk = oi->last_blk + 1;
	} ಅನ್ಯಥಾ {
		err = ext2fs_new_block2(fs, oi->last_blk, 0, &new_blk);
		ಯದಿ (err) {
			oi->err = err;
			ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		}
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
		oi->alloc_blocks++;
	}
	ಯದಿ (blockcnt >= 0) {
		ಯದಿ (ext2fs_has_feature_metadata_csum(fs->super)) {
			ಸಂರಚನಾ ext4_orphan_block_tail *tail;

			tail = ext2fs_orphan_block_tail(fs, oi->buf);
			tail->ob_checksum =
		ext2fs_cpu_to_le32(ext2fs_do_orphan_file_block_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} ಅನ್ಯಥಾ	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	ಯದಿ (err) {
		oi->err = err;
		ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
	}
	oi->last_blk = new_blk;
	*blocknr = new_blk;
	ಯದಿ (blockcnt >= 0 && --oi->ಅಂಕೆ_blocks == 0)
		ಪ್ರತ್ಯಾಗಮ BLOCK_CHANGED | BLOCK_ABORT;
	ಪ್ರತ್ಯಾಗಮ BLOCK_CHANGED;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t ಅಂಕೆ_blocks)
{
	ಸಂರಚನಾ ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	ವರ್ಣ *buf = ರಿಕ್ತ, *zerobuf = ರಿಕ್ತ;
	ಸಂರಚನಾ mkorphan_info oi;
	ಸಂರಚನಾ ext4_orphan_block_tail *ob_tail;
	time_t now;

	ಯದಿ (ಅಂಕೆ_blocks > EXT4_MAX_ORPHAN_FILE_BLOCKS)
		ಅಂಕೆ_blocks = EXT4_MAX_ORPHAN_FILE_BLOCKS;

	ಯದಿ (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ err;
		ಯದಿ (EXT2_I_SIZE(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ err;
		}
	} ಅನ್ಯಥಾ {
		err = ext2fs_new_inode(fs, EXT2_ಬೇರು_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode));
	ಯದಿ (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	ಯದಿ (err)
		ಗಚ್ಛ out;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_tail = ext2fs_orphan_block_tail(fs, buf);
	ob_tail->ob_magic = ext2fs_cpu_to_le32(EXT4_ORPHAN_BLOCK_MAGIC);
	oi.ಅಂಕೆ_blocks = ಅಂಕೆ_blocks;
	oi.alloc_blocks = 0;
	oi.last_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_block_iterate3(fs, ino, BLOCK_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	ಯದಿ (err)
		ಗಚ್ಛ out;
	ಯದಿ (oi.err) {
		err = oi.err;
		ಗಚ್ಛ out;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	ಯದಿ (err)
		ಗಚ್ಛ out;
	ext2fs_iblk_set(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	inode.i_links_ಎಣಿಕೆ = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	ext2fs_iblk_add_blocks(fs, &inode, oi.alloc_blocks);
	err = ext2fs_inode_size_set(fs, &inode,
			(unsigned long long)fs->blocksize * ಅಂಕೆ_blocks);
	ಯದಿ (err)
		ಗಚ್ಛ out;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	ಯದಿ (err)
		ಗಚ್ಛ out;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_set_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
out:
	ಯದಿ (buf)
		ext2fs_free_mem(&buf);
	ಯದಿ (zerobuf)
		ext2fs_free_mem(&zerobuf);
	ಪ್ರತ್ಯಾಗಮ err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs)
{
	__u64 ಅಂಕೆ_blocks = ext2fs_blocks_ಎಣಿಕೆ(fs->super);
	e2_blkcnt_t blks = 512;

	ಯದಿ (ಅಂಕೆ_blocks < 128 * 1024)
		blks = 32;
	ಅನ್ಯಥಾ ಯದಿ (ಅಂಕೆ_blocks < 2 * 1024 * 1024)
		blks = ಅಂಕೆ_blocks / 4096;
	ಪ್ರತ್ಯಾಗಮ (blks + EXT2FS_CLUSTER_MASK(fs)) & ~EXT2FS_CLUSTER_MASK(fs);
}

ಸ್ಥಿರ errcode_t ext2fs_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, ವರ್ಣ *buf,
					       __u32 *crcp)
{
	ಸಂರಚನಾ ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	*crcp = ext2fs_do_orphan_file_block_csum(fs, ino, inode.i_generation,
						 blk, buf);
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, ವರ್ಣ *buf)
{
	ಸಂರಚನಾ ext4_orphan_block_tail *tail;
	errcode_t ret;
	__u32 crc = 0;

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 0;

	tail = ext2fs_orphan_block_tail(fs, buf);
	ret = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ 0;
	tail->ob_checksum = ext2fs_cpu_to_le32(crc);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, ವರ್ಣ *buf)
{
	ಸಂರಚನಾ ext4_orphan_block_tail *tail;
	__u32 crc;
	errcode_t retval;

	ಯದಿ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಪ್ರತ್ಯಾಗಮ 1;
	retval = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ 0;
	tail = ext2fs_orphan_block_tail(fs, buf);
	ಪ್ರತ್ಯಾಗಮ ext2fs_le32_to_cpu(tail->ob_checksum) == crc;
}
