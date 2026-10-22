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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

#define EXT4_MAX_ORPHAN_FILE_BLOCKS 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	ସଂରଚନା ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;

	err = ext2fs_punch(fs, ino, &inode, ଖାଲି, 0, ~0ULL);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, ଆକାର(ସଂରଚନା ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	ଫେରନ୍ତୁ err;
}

__u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, ଅକ୍ଷର *buf)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା inodes_per_ob = ext2fs_inodes_per_orphan_block(fs);
	__u32 crc;

	ino = ext2fs_cpu_to_le32(ino);
	gen = ext2fs_cpu_to_le32(gen);
	blk = ext2fs_cpu_to_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned ଅକ୍ଷର *)&ino,
			       ଆକାର(ino));
	crc = ext2fs_crc32c_le(crc, (unsigned ଅକ୍ଷର *)&gen, ଆକାର(gen));
	crc = ext2fs_crc32c_le(crc, (unsigned ଅକ୍ଷର *)&blk, ଆକାର(blk));
	crc = ext2fs_crc32c_le(crc, (unsigned ଅକ୍ଷର *)buf,
				inodes_per_ob * ଆକାର(__u32));

	ଫେରନ୍ତୁ crc;
}

ସଂରଚନା mkorphan_info {
	ଅକ୍ଷର *buf;
	ଅକ୍ଷର *zerobuf;
	blk_t ଅଙ୍କ_blocks;
	blk_t alloc_blocks;
	blk64_t last_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_block EXT2FS_ATTR((unused)),
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ref_offset EXT2FS_ATTR((unused)),
			 ଶୂନ୍ୟ		*priv_data)
{
	ସଂରଚନା mkorphan_info *oi = (ସଂରଚନା mkorphan_info *)priv_data;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	ଯଦି (blockcnt &&
	    EXT2FS_B2C(fs, oi->last_blk) == EXT2FS_B2C(fs, oi->last_blk + 1)) {
		new_blk = oi->last_blk + 1;
	} ଅନ୍ୟଥା {
		err = ext2fs_new_block2(fs, oi->last_blk, 0, &new_blk);
		ଯଦି (err) {
			oi->err = err;
			ଫେରନ୍ତୁ BLOCK_ABORT;
		}
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
		oi->alloc_blocks++;
	}
	ଯଦି (blockcnt >= 0) {
		ଯଦି (ext2fs_has_feature_metadata_csum(fs->super)) {
			ସଂରଚନା ext4_orphan_block_tail *tail;

			tail = ext2fs_orphan_block_tail(fs, oi->buf);
			tail->ob_checksum =
		ext2fs_cpu_to_le32(ext2fs_do_orphan_file_block_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} ଅନ୍ୟଥା	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	ଯଦି (err) {
		oi->err = err;
		ଫେରନ୍ତୁ BLOCK_ABORT;
	}
	oi->last_blk = new_blk;
	*blocknr = new_blk;
	ଯଦି (blockcnt >= 0 && --oi->ଅଙ୍କ_blocks == 0)
		ଫେରନ୍ତୁ BLOCK_CHANGED | BLOCK_ABORT;
	ଫେରନ୍ତୁ BLOCK_CHANGED;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t ଅଙ୍କ_blocks)
{
	ସଂରଚନା ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	ଅକ୍ଷର *buf = ଖାଲି, *zerobuf = ଖାଲି;
	ସଂରଚନା mkorphan_info oi;
	ସଂରଚନା ext4_orphan_block_tail *ob_tail;
	time_t now;

	ଯଦି (ଅଙ୍କ_blocks > EXT4_MAX_ORPHAN_FILE_BLOCKS)
		ଅଙ୍କ_blocks = EXT4_MAX_ORPHAN_FILE_BLOCKS;

	ଯଦି (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		ଯଦି (err)
			ଫେରନ୍ତୁ err;
		ଯଦି (EXT2_I_SIZE(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			ଯଦି (err)
				ଫେରନ୍ତୁ err;
		}
	} ଅନ୍ୟଥା {
		err = ext2fs_new_inode(fs, EXT2_ମୂଳ_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		ଯଦି (err)
			ଫେରନ୍ତୁ err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, ଆକାର(ସଂରଚନା ext2_inode));
	ଯଦି (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		ଯଦି (err)
			ଫେରନ୍ତୁ err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	ଯଦି (err)
		ଯାଅ out;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_tail = ext2fs_orphan_block_tail(fs, buf);
	ob_tail->ob_magic = ext2fs_cpu_to_le32(EXT4_ORPHAN_BLOCK_MAGIC);
	oi.ଅଙ୍କ_blocks = ଅଙ୍କ_blocks;
	oi.alloc_blocks = 0;
	oi.last_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_block_iterate3(fs, ino, BLOCK_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	ଯଦି (err)
		ଯାଅ out;
	ଯଦି (oi.err) {
		err = oi.err;
		ଯାଅ out;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (err)
		ଯାଅ out;
	ext2fs_iblk_set(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	inode.i_links_ଗଣନା = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	ext2fs_iblk_add_blocks(fs, &inode, oi.alloc_blocks);
	err = ext2fs_inode_size_set(fs, &inode,
			(unsigned long long)fs->blocksize * ଅଙ୍କ_blocks);
	ଯଦି (err)
		ଯାଅ out;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	ଯଦି (err)
		ଯାଅ out;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_set_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
out:
	ଯଦି (buf)
		ext2fs_free_mem(&buf);
	ଯଦି (zerobuf)
		ext2fs_free_mem(&zerobuf);
	ଫେରନ୍ତୁ err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs)
{
	__u64 ଅଙ୍କ_blocks = ext2fs_blocks_ଗଣନା(fs->super);
	e2_blkcnt_t blks = 512;

	ଯଦି (ଅଙ୍କ_blocks < 128 * 1024)
		blks = 32;
	ଅନ୍ୟଥା ଯଦି (ଅଙ୍କ_blocks < 2 * 1024 * 1024)
		blks = ଅଙ୍କ_blocks / 4096;
	ଫେରନ୍ତୁ (blks + EXT2FS_CLUSTER_MASK(fs)) & ~EXT2FS_CLUSTER_MASK(fs);
}

ସ୍ଥିର errcode_t ext2fs_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, ଅକ୍ଷର *buf,
					       __u32 *crcp)
{
	ସଂରଚନା ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	*crcp = ext2fs_do_orphan_file_block_csum(fs, ino, inode.i_generation,
						 blk, buf);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, ଅକ୍ଷର *buf)
{
	ସଂରଚନା ext4_orphan_block_tail *tail;
	errcode_t ret;
	__u32 crc = 0;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 0;

	tail = ext2fs_orphan_block_tail(fs, buf);
	ret = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	ଯଦି (ret)
		ଫେରନ୍ତୁ 0;
	tail->ob_checksum = ext2fs_cpu_to_le32(crc);
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, ଅକ୍ଷର *buf)
{
	ସଂରଚନା ext4_orphan_block_tail *tail;
	__u32 crc;
	errcode_t retval;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super))
		ଫେରନ୍ତୁ 1;
	retval = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	ଯଦି (retval)
		ଫେରନ୍ତୁ 0;
	tail = ext2fs_orphan_block_tail(fs, buf);
	ଫେରନ୍ତୁ ext2fs_le32_to_cpu(tail->ob_checksum) == crc;
}
