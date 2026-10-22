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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

#define EXT4_MAX_ORPHAN_FILE_BLOCKS 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	ഘടന ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (err)
		തിരികെ err;

	err = ext2fs_punch(fs, ino, &inode, ശൂന്യ, 0, ~0ULL);
	എങ്കിൽ (err)
		തിരികെ err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, വലുപ്പം(ഘടന ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	തിരികെ err;
}

__u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, അക്ഷരം *buf)
{
	പൂർണ്ണസംഖ്യ inodes_per_ob = ext2fs_inodes_per_orphan_block(fs);
	__u32 crc;

	ino = ext2fs_cpu_to_le32(ino);
	gen = ext2fs_cpu_to_le32(gen);
	blk = ext2fs_cpu_to_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned അക്ഷരം *)&ino,
			       വലുപ്പം(ino));
	crc = ext2fs_crc32c_le(crc, (unsigned അക്ഷരം *)&gen, വലുപ്പം(gen));
	crc = ext2fs_crc32c_le(crc, (unsigned അക്ഷരം *)&blk, വലുപ്പം(blk));
	crc = ext2fs_crc32c_le(crc, (unsigned അക്ഷരം *)buf,
				inodes_per_ob * വലുപ്പം(__u32));

	തിരികെ crc;
}

ഘടന mkorphan_info {
	അക്ഷരം *buf;
	അക്ഷരം *zerobuf;
	blk_t അക്കം_blocks;
	blk_t alloc_blocks;
	blk64_t last_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_block EXT2FS_ATTR((unused)),
			 പൂർണ്ണസംഖ്യ		ref_offset EXT2FS_ATTR((unused)),
			 ശൂന്യം		*priv_data)
{
	ഘടന mkorphan_info *oi = (ഘടന mkorphan_info *)priv_data;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	എങ്കിൽ (blockcnt &&
	    EXT2FS_B2C(fs, oi->last_blk) == EXT2FS_B2C(fs, oi->last_blk + 1)) {
		new_blk = oi->last_blk + 1;
	} അല്ലെങ്കിൽ {
		err = ext2fs_new_block2(fs, oi->last_blk, 0, &new_blk);
		എങ്കിൽ (err) {
			oi->err = err;
			തിരികെ BLOCK_ABORT;
		}
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
		oi->alloc_blocks++;
	}
	എങ്കിൽ (blockcnt >= 0) {
		എങ്കിൽ (ext2fs_has_feature_metadata_csum(fs->super)) {
			ഘടന ext4_orphan_block_tail *tail;

			tail = ext2fs_orphan_block_tail(fs, oi->buf);
			tail->ob_checksum =
		ext2fs_cpu_to_le32(ext2fs_do_orphan_file_block_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} അല്ലെങ്കിൽ	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	എങ്കിൽ (err) {
		oi->err = err;
		തിരികെ BLOCK_ABORT;
	}
	oi->last_blk = new_blk;
	*blocknr = new_blk;
	എങ്കിൽ (blockcnt >= 0 && --oi->അക്കം_blocks == 0)
		തിരികെ BLOCK_CHANGED | BLOCK_ABORT;
	തിരികെ BLOCK_CHANGED;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t അക്കം_blocks)
{
	ഘടന ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	അക്ഷരം *buf = ശൂന്യ, *zerobuf = ശൂന്യ;
	ഘടന mkorphan_info oi;
	ഘടന ext4_orphan_block_tail *ob_tail;
	time_t now;

	എങ്കിൽ (അക്കം_blocks > EXT4_MAX_ORPHAN_FILE_BLOCKS)
		അക്കം_blocks = EXT4_MAX_ORPHAN_FILE_BLOCKS;

	എങ്കിൽ (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		എങ്കിൽ (err)
			തിരികെ err;
		എങ്കിൽ (EXT2_I_SIZE(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			എങ്കിൽ (err)
				തിരികെ err;
		}
	} അല്ലെങ്കിൽ {
		err = ext2fs_new_inode(fs, EXT2_വേര്_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		എങ്കിൽ (err)
			തിരികെ err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, വലുപ്പം(ഘടന ext2_inode));
	എങ്കിൽ (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		എങ്കിൽ (err)
			തിരികെ err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	എങ്കിൽ (err)
		തിരികെ err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	എങ്കിൽ (err)
		പോകുക out;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_tail = ext2fs_orphan_block_tail(fs, buf);
	ob_tail->ob_magic = ext2fs_cpu_to_le32(EXT4_ORPHAN_BLOCK_MAGIC);
	oi.അക്കം_blocks = അക്കം_blocks;
	oi.alloc_blocks = 0;
	oi.last_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_block_iterate3(fs, ino, BLOCK_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	എങ്കിൽ (err)
		പോകുക out;
	എങ്കിൽ (oi.err) {
		err = oi.err;
		പോകുക out;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (err)
		പോകുക out;
	ext2fs_iblk_set(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	inode.i_links_എണ്ണം = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	ext2fs_iblk_add_blocks(fs, &inode, oi.alloc_blocks);
	err = ext2fs_inode_size_set(fs, &inode,
			(unsigned long long)fs->blocksize * അക്കം_blocks);
	എങ്കിൽ (err)
		പോകുക out;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	എങ്കിൽ (err)
		പോകുക out;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_set_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
out:
	എങ്കിൽ (buf)
		ext2fs_free_mem(&buf);
	എങ്കിൽ (zerobuf)
		ext2fs_free_mem(&zerobuf);
	തിരികെ err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs)
{
	__u64 അക്കം_blocks = ext2fs_blocks_എണ്ണം(fs->super);
	e2_blkcnt_t blks = 512;

	എങ്കിൽ (അക്കം_blocks < 128 * 1024)
		blks = 32;
	അല്ലെങ്കിൽ എങ്കിൽ (അക്കം_blocks < 2 * 1024 * 1024)
		blks = അക്കം_blocks / 4096;
	തിരികെ (blks + EXT2FS_CLUSTER_MASK(fs)) & ~EXT2FS_CLUSTER_MASK(fs);
}

സ്ഥിതം errcode_t ext2fs_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, അക്ഷരം *buf,
					       __u32 *crcp)
{
	ഘടന ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;
	*crcp = ext2fs_do_orphan_file_block_csum(fs, ino, inode.i_generation,
						 blk, buf);
	തിരികെ 0;
}

errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, അക്ഷരം *buf)
{
	ഘടന ext4_orphan_block_tail *tail;
	errcode_t ret;
	__u32 crc = 0;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 0;

	tail = ext2fs_orphan_block_tail(fs, buf);
	ret = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	എങ്കിൽ (ret)
		തിരികെ 0;
	tail->ob_checksum = ext2fs_cpu_to_le32(crc);
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, അക്ഷരം *buf)
{
	ഘടന ext4_orphan_block_tail *tail;
	__u32 crc;
	errcode_t retval;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super))
		തിരികെ 1;
	retval = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	എങ്കിൽ (retval)
		തിരികെ 0;
	tail = ext2fs_orphan_block_tail(fs, buf);
	തിരികെ ext2fs_le32_to_cpu(tail->ob_checksum) == crc;
}
