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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"

#define EXT4_MAX_ORPHAN_FILE_BLOCKS 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	কাঠামো ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	যদি (err)
		ফেরত err;

	err = ext2fs_punch(fs, ino, &inode, ফাঁকা, 0, ~0ULL);
	যদি (err)
		ফেরত err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, আকার(কাঠামো ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	ফেরত err;
}

__u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, অক্ষর *buf)
{
	পূর্ণসংখ্যা inodes_per_ob = ext2fs_inodes_per_orphan_block(fs);
	__u32 crc;

	ino = ext2fs_cpu_to_le32(ino);
	gen = ext2fs_cpu_to_le32(gen);
	blk = ext2fs_cpu_to_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned অক্ষর *)&ino,
			       আকার(ino));
	crc = ext2fs_crc32c_le(crc, (unsigned অক্ষর *)&gen, আকার(gen));
	crc = ext2fs_crc32c_le(crc, (unsigned অক্ষর *)&blk, আকার(blk));
	crc = ext2fs_crc32c_le(crc, (unsigned অক্ষর *)buf,
				inodes_per_ob * আকার(__u32));

	ফেরত crc;
}

কাঠামো mkorphan_info {
	অক্ষর *buf;
	অক্ষর *zerobuf;
	blk_t অঙ্ক_blocks;
	blk_t alloc_blocks;
	blk64_t last_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

স্থির পূর্ণসংখ্যা mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_block EXT2FS_ATTR((unused)),
			 পূর্ণসংখ্যা		ref_offset EXT2FS_ATTR((unused)),
			 শূন্য		*priv_তথ্য)
{
	কাঠামো mkorphan_info *oi = (কাঠামো mkorphan_info *)priv_তথ্য;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	যদি (blockcnt &&
	    EXT2FS_B2C(fs, oi->last_blk) == EXT2FS_B2C(fs, oi->last_blk + 1)) {
		new_blk = oi->last_blk + 1;
	} অন্যথা {
		err = ext2fs_new_block2(fs, oi->last_blk, 0, &new_blk);
		যদি (err) {
			oi->err = err;
			ফেরত BLOCK_ABORT;
		}
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
		oi->alloc_blocks++;
	}
	যদি (blockcnt >= 0) {
		যদি (ext2fs_has_feature_metadata_csum(fs->super)) {
			কাঠামো ext4_orphan_block_লেজ *লেজ;

			লেজ = ext2fs_orphan_block_লেজ(fs, oi->buf);
			লেজ->ob_checksum =
		ext2fs_cpu_to_le32(ext2fs_do_orphan_file_block_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} অন্যথা	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	যদি (err) {
		oi->err = err;
		ফেরত BLOCK_ABORT;
	}
	oi->last_blk = new_blk;
	*blocknr = new_blk;
	যদি (blockcnt >= 0 && --oi->অঙ্ক_blocks == 0)
		ফেরত BLOCK_CHANGED | BLOCK_ABORT;
	ফেরত BLOCK_CHANGED;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t অঙ্ক_blocks)
{
	কাঠামো ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	অক্ষর *buf = ফাঁকা, *zerobuf = ফাঁকা;
	কাঠামো mkorphan_info oi;
	কাঠামো ext4_orphan_block_লেজ *ob_লেজ;
	time_t now;

	যদি (অঙ্ক_blocks > EXT4_MAX_ORPHAN_FILE_BLOCKS)
		অঙ্ক_blocks = EXT4_MAX_ORPHAN_FILE_BLOCKS;

	যদি (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		যদি (err)
			ফেরত err;
		যদি (EXT2_I_আকার(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			যদি (err)
				ফেরত err;
		}
	} অন্যথা {
		err = ext2fs_new_inode(fs, EXT2_মূল_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		যদি (err)
			ফেরত err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, আকার(কাঠামো ext2_inode));
	যদি (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		যদি (err)
			ফেরত err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (err)
		ফেরত err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	যদি (err)
		যাও out;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_লেজ = ext2fs_orphan_block_লেজ(fs, buf);
	ob_লেজ->ob_magic = ext2fs_cpu_to_le32(EXT4_ORPHAN_BLOCK_MAGIC);
	oi.অঙ্ক_blocks = অঙ্ক_blocks;
	oi.alloc_blocks = 0;
	oi.last_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_block_iterate3(fs, ino, BLOCK_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	যদি (err)
		যাও out;
	যদি (oi.err) {
		err = oi.err;
		যাও out;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	যদি (err)
		যাও out;
	ext2fs_iblk_set(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	inode.i_links_গুনতি = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	ext2fs_iblk_add_blocks(fs, &inode, oi.alloc_blocks);
	err = ext2fs_inode_আকার_set(fs, &inode,
			(unsigned long long)fs->blocksize * অঙ্ক_blocks);
	যদি (err)
		যাও out;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	যদি (err)
		যাও out;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_set_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
out:
	যদি (buf)
		ext2fs_free_mem(&buf);
	যদি (zerobuf)
		ext2fs_free_mem(&zerobuf);
	ফেরত err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs)
{
	__u64 অঙ্ক_blocks = ext2fs_blocks_গুনতি(fs->super);
	e2_blkcnt_t blks = 512;

	যদি (অঙ্ক_blocks < 128 * 1024)
		blks = 32;
	অন্যথা যদি (অঙ্ক_blocks < 2 * 1024 * 1024)
		blks = অঙ্ক_blocks / 4096;
	ফেরত (blks + EXT2FS_CLUSTER_MASK(fs)) & ~EXT2FS_CLUSTER_MASK(fs);
}

স্থির errcode_t ext2fs_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, অক্ষর *buf,
					       __u32 *crcp)
{
	কাঠামো ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ফেরত retval;
	*crcp = ext2fs_do_orphan_file_block_csum(fs, ino, inode.i_generation,
						 blk, buf);
	ফেরত 0;
}

errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, অক্ষর *buf)
{
	কাঠামো ext4_orphan_block_লেজ *লেজ;
	errcode_t ret;
	__u32 crc = 0;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 0;

	লেজ = ext2fs_orphan_block_লেজ(fs, buf);
	ret = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	যদি (ret)
		ফেরত 0;
	লেজ->ob_checksum = ext2fs_cpu_to_le32(crc);
	ফেরত ret;
}

পূর্ণসংখ্যা ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, অক্ষর *buf)
{
	কাঠামো ext4_orphan_block_লেজ *লেজ;
	__u32 crc;
	errcode_t retval;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super))
		ফেরত 1;
	retval = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	যদি (retval)
		ফেরত 0;
	লেজ = ext2fs_orphan_block_লেজ(fs, buf);
	ফেরত ext2fs_le32_to_cpu(লেজ->ob_checksum) == crc;
}
