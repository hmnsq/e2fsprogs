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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MAX_ORPHAN_FILE_ਖੰਡਾਂ 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	ਬਣਤਰ ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (err)
		ਵਾਪਸ err;

	err = ext2fs_punch(fs, ino, &inode, ਨਿਰਾ, 0, ~0ULL);
	ਜੇ (err)
		ਵਾਪਸ err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, ਆਕਾਰ(ਬਣਤਰ ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	ਵਾਪਸ err;
}

__u32 ext2fs_do_orphan_file_ਖੰਡ_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, ਅੱਖਰ *buf)
{
	ਪੂਰਨ inodes_ਪ੍ਰਤੀ_ob = ext2fs_inodes_ਪ੍ਰਤੀ_orphan_ਖੰਡ(fs);
	__u32 ਚੱਕਰੀ_ਜਾਂਚ;

	ino = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ino);
	gen = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(gen);
	blk = ext2fs_cpu_ਮੰਜ਼ਿਲ_le64(blk);
	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(fs->csum_ਬੀਜ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&ino,
			       ਆਕਾਰ(ino));
	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&gen, ਆਕਾਰ(gen));
	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&blk, ਆਕਾਰ(blk));
	ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(ਚੱਕਰੀ_ਜਾਂਚ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)buf,
				inodes_ਪ੍ਰਤੀ_ob * ਆਕਾਰ(__u32));

	ਵਾਪਸ ਚੱਕਰੀ_ਜਾਂਚ;
}

ਬਣਤਰ mkorphan_info {
	ਅੱਖਰ *buf;
	ਅੱਖਰ *zerobuf;
	blk_t ਅੰਕ_ਖੰਡਾਂ;
	blk_t alloc_ਖੰਡਾਂ;
	blk64_t ਆਖਰੀ_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ਸਥਿਰ ਪੂਰਨ mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_ਖੰਡ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			 ਪੂਰਨ		ref_ਵਿੱਥ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			 ਖਾਲੀ		*priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ mkorphan_info *oi = (ਬਣਤਰ mkorphan_info *)priv_ਅੰਕੜੇ;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	ਜੇ (blockcnt &&
	    EXT2FS_B2C(fs, oi->ਆਖਰੀ_blk) == EXT2FS_B2C(fs, oi->ਆਖਰੀ_blk + 1)) {
		new_blk = oi->ਆਖਰੀ_blk + 1;
	} ਨਹੀਂਤਾਂ {
		err = ext2fs_new_block2(fs, oi->ਆਖਰੀ_blk, 0, &new_blk);
		ਜੇ (err) {
			oi->err = err;
			ਵਾਪਸ ਖੰਡ_ABORT;
		}
		ext2fs_ਖੰਡ_alloc_stats2(fs, new_blk, +1);
		oi->alloc_ਖੰਡਾਂ++;
	}
	ਜੇ (blockcnt >= 0) {
		ਜੇ (ext2fs_has_feature_metadata_csum(fs->super)) {
			ਬਣਤਰ ext4_orphan_ਖੰਡ_ਪੂਛ *ਪੂਛ;

			ਪੂਛ = ext2fs_orphan_ਖੰਡ_ਪੂਛ(fs, oi->buf);
			ਪੂਛ->ob_checksum =
		ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ext2fs_do_orphan_file_ਖੰਡ_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} ਨਹੀਂਤਾਂ	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	ਜੇ (err) {
		oi->err = err;
		ਵਾਪਸ ਖੰਡ_ABORT;
	}
	oi->ਆਖਰੀ_blk = new_blk;
	*blocknr = new_blk;
	ਜੇ (blockcnt >= 0 && --oi->ਅੰਕ_ਖੰਡਾਂ == 0)
		ਵਾਪਸ ਖੰਡ_ਬਦਲਿਆ | ਖੰਡ_ABORT;
	ਵਾਪਸ ਖੰਡ_ਬਦਲਿਆ;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t ਅੰਕ_ਖੰਡਾਂ)
{
	ਬਣਤਰ ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	ਅੱਖਰ *buf = ਨਿਰਾ, *zerobuf = ਨਿਰਾ;
	ਬਣਤਰ mkorphan_info oi;
	ਬਣਤਰ ext4_orphan_ਖੰਡ_ਪੂਛ *ob_ਪੂਛ;
	time_t now;

	ਜੇ (ਅੰਕ_ਖੰਡਾਂ > EXT4_MAX_ORPHAN_FILE_ਖੰਡਾਂ)
		ਅੰਕ_ਖੰਡਾਂ = EXT4_MAX_ORPHAN_FILE_ਖੰਡਾਂ;

	ਜੇ (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		ਜੇ (err)
			ਵਾਪਸ err;
		ਜੇ (EXT2_I_ਮਾਪ(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			ਜੇ (err)
				ਵਾਪਸ err;
		}
	} ਨਹੀਂਤਾਂ {
		err = ext2fs_new_inode(fs, EXT2_ਜੜ੍ਹ_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		ਜੇ (err)
			ਵਾਪਸ err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, ਆਕਾਰ(ਬਣਤਰ ext2_inode));
	ਜੇ (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		ਜੇ (err)
			ਵਾਪਸ err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	ਜੇ (err)
		ਵਾਪਸ err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_ਪੂਛ = ext2fs_orphan_ਖੰਡ_ਪੂਛ(fs, buf);
	ob_ਪੂਛ->ob_magic = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(EXT4_ORPHAN_ਖੰਡ_MAGIC);
	oi.ਅੰਕ_ਖੰਡਾਂ = ਅੰਕ_ਖੰਡਾਂ;
	oi.alloc_ਖੰਡਾਂ = 0;
	oi.ਆਖਰੀ_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_ਖੰਡ_iterate3(fs, ino, ਖੰਡ_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;
	ਜੇ (oi.err) {
		err = oi.err;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;
	ext2fs_iblk_ਸੈੱਟ(fs, &inode, 0);
	now = ext2fs_P_get_ਸਮਾਂ(fs);
	ext2fs_inode_xtime_ਸੈੱਟ(&inode, i_atime, now);
	ext2fs_inode_xtime_ਸੈੱਟ(&inode, i_ctime, now);
	ext2fs_inode_xtime_ਸੈੱਟ(&inode, i_mtime, now);
	inode.i_links_ਗਿਣਤੀ = 1;
	inode.i_ਬਹੁਲਕ = LINUX_S_IFREG | 0600;
	ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(fs, &inode, oi.alloc_ਖੰਡਾਂ);
	err = ext2fs_inode_ਮਾਪ_ਸੈੱਟ(fs, &inode,
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)fs->blocksize * ਅੰਕ_ਖੰਡਾਂ);
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_ਸੈੱਟ_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
ਬਾਹਰਲਾ:
	ਜੇ (buf)
		ext2fs_free_mem(&buf);
	ਜੇ (zerobuf)
		ext2fs_free_mem(&zerobuf);
	ਵਾਪਸ err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_ਖੰਡਾਂ(ext2_filsys fs)
{
	__u64 ਅੰਕ_ਖੰਡਾਂ = ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super);
	e2_blkcnt_t blks = 512;

	ਜੇ (ਅੰਕ_ਖੰਡਾਂ < 128 * 1024)
		blks = 32;
	ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕ_ਖੰਡਾਂ < 2 * 1024 * 1024)
		blks = ਅੰਕ_ਖੰਡਾਂ / 4096;
	ਵਾਪਸ (blks + EXT2_FS_CLUSTER_ਨਕਾਬ(fs)) & ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
}

ਸਥਿਰ errcode_t ext2fs_orphan_file_ਖੰਡ_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, ਅੱਖਰ *buf,
					       __u32 *crcp)
{
	ਬਣਤਰ ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	*crcp = ext2fs_do_orphan_file_ਖੰਡ_csum(fs, ino, inode.i_generation,
						 blk, buf);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_orphan_file_ਖੰਡ_csum_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, ਅੱਖਰ *buf)
{
	ਬਣਤਰ ext4_orphan_ਖੰਡ_ਪੂਛ *ਪੂਛ;
	errcode_t ret;
	__u32 ਚੱਕਰੀ_ਜਾਂਚ = 0;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 0;

	ਪੂਛ = ext2fs_orphan_ਖੰਡ_ਪੂਛ(fs, buf);
	ret = ext2fs_orphan_file_ਖੰਡ_csum(fs, ino, blk, buf, &ਚੱਕਰੀ_ਜਾਂਚ);
	ਜੇ (ret)
		ਵਾਪਸ 0;
	ਪੂਛ->ob_checksum = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਚੱਕਰੀ_ਜਾਂਚ);
	ਵਾਪਸ ret;
}

ਪੂਰਨ ext2fs_orphan_file_ਖੰਡ_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, ਅੱਖਰ *buf)
{
	ਬਣਤਰ ext4_orphan_ਖੰਡ_ਪੂਛ *ਪੂਛ;
	__u32 ਚੱਕਰੀ_ਜਾਂਚ;
	errcode_t retval;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super))
		ਵਾਪਸ 1;
	retval = ext2fs_orphan_file_ਖੰਡ_csum(fs, ino, blk, buf, &ਚੱਕਰੀ_ਜਾਂਚ);
	ਜੇ (retval)
		ਵਾਪਸ 0;
	ਪੂਛ = ext2fs_orphan_ਖੰਡ_ਪੂਛ(fs, buf);
	ਵਾਪਸ ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ਪੂਛ->ob_checksum) == ਚੱਕਰੀ_ਜਾਂਚ;
}
