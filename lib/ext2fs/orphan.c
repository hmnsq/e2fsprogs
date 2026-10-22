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

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MAX_ORPHAN_FILE_ಖಂಡಗಳು 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	ರಚನೆ ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	err = ext2fs_punch(fs, ino, &inode, ಖಾಲಿ, 0, ~0ULL);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, ಗಾತ್ರ(ರಚನೆ ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	ಹಿಂತಿರುಗಿ err;
}

__u32 ext2fs_do_orphan_file_ಖಂಡ_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, ಅಕ್ಷರ *buf)
{
	ಪೂರ್ಣಾಂಕ inodes_ಪ್ರತಿ_ob = ext2fs_inodes_ಪ್ರತಿ_orphan_ಖಂಡ(fs);
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;

	ino = ext2fs_cpu_ಗಮ್ಯ_le32(ino);
	gen = ext2fs_cpu_ಗಮ್ಯ_le32(gen);
	blk = ext2fs_cpu_ಗಮ್ಯ_le64(blk);
	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(fs->csum_ಬೀಜ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&ino,
			       ಗಾತ್ರ(ino));
	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&gen, ಗಾತ್ರ(gen));
	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&blk, ಗಾತ್ರ(blk));
	ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ext2fs_crc32c_le(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)buf,
				inodes_ಪ್ರತಿ_ob * ಗಾತ್ರ(__u32));

	ಹಿಂತಿರುಗಿ ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
}

ರಚನೆ mkorphan_info {
	ಅಕ್ಷರ *buf;
	ಅಕ್ಷರ *zerobuf;
	blk_t ಅಂಕೆ_ಖಂಡಗಳು;
	blk_t alloc_ಖಂಡಗಳು;
	blk64_t ಕೊನೆಯ_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_ಖಂಡ EXT2FS_ATTR((ಬಳಸದ)),
			 ಪೂರ್ಣಾಂಕ		ref_ಸ್ಥಾನಾಂತರ EXT2FS_ATTR((ಬಳಸದ)),
			 ಶೂನ್ಯ		*priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ mkorphan_info *oi = (ರಚನೆ mkorphan_info *)priv_ದತ್ತಾಂಶ;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	ಆದರೆ (blockcnt &&
	    EXT2FS_B2C(fs, oi->ಕೊನೆಯ_blk) == EXT2FS_B2C(fs, oi->ಕೊನೆಯ_blk + 1)) {
		new_blk = oi->ಕೊನೆಯ_blk + 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		err = ext2fs_new_block2(fs, oi->ಕೊನೆಯ_blk, 0, &new_blk);
		ಆದರೆ (err) {
			oi->err = err;
			ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		}
		ext2fs_ಖಂಡ_alloc_stats2(fs, new_blk, +1);
		oi->alloc_ಖಂಡಗಳು++;
	}
	ಆದರೆ (blockcnt >= 0) {
		ಆದರೆ (ext2fs_has_feature_metadata_csum(fs->super)) {
			ರಚನೆ ext4_orphan_ಖಂಡ_ಬಾಲ *ಬಾಲ;

			ಬಾಲ = ext2fs_orphan_ಖಂಡ_ಬಾಲ(fs, oi->buf);
			ಬಾಲ->ob_checksum =
		ext2fs_cpu_ಗಮ್ಯ_le32(ext2fs_do_orphan_file_ಖಂಡ_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} ಇಲ್ಲದಿದ್ದರೆ	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	ಆದರೆ (err) {
		oi->err = err;
		ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
	}
	oi->ಕೊನೆಯ_blk = new_blk;
	*blocknr = new_blk;
	ಆದರೆ (blockcnt >= 0 && --oi->ಅಂಕೆ_ಖಂಡಗಳು == 0)
		ಹಿಂತಿರುಗಿ ಖಂಡ_ಬದಲಾಯಿತು | ಖಂಡ_ABORT;
	ಹಿಂತಿರುಗಿ ಖಂಡ_ಬದಲಾಯಿತು;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t ಅಂಕೆ_ಖಂಡಗಳು)
{
	ರಚನೆ ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	ಅಕ್ಷರ *buf = ಖಾಲಿ, *zerobuf = ಖಾಲಿ;
	ರಚನೆ mkorphan_info oi;
	ರಚನೆ ext4_orphan_ಖಂಡ_ಬಾಲ *ob_ಬಾಲ;
	time_t now;

	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು > EXT4_MAX_ORPHAN_FILE_ಖಂಡಗಳು)
		ಅಂಕೆ_ಖಂಡಗಳು = EXT4_MAX_ORPHAN_FILE_ಖಂಡಗಳು;

	ಆದರೆ (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ err;
		ಆದರೆ (EXT2_I_ಗಾತ್ರ(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ err;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		err = ext2fs_new_inode(fs, EXT2_ಬೇರು_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, ಗಾತ್ರ(ರಚನೆ ext2_inode));
	ಆದರೆ (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_ಬಾಲ = ext2fs_orphan_ಖಂಡ_ಬಾಲ(fs, buf);
	ob_ಬಾಲ->ob_magic = ext2fs_cpu_ಗಮ್ಯ_le32(EXT4_ORPHAN_ಖಂಡ_MAGIC);
	oi.ಅಂಕೆ_ಖಂಡಗಳು = ಅಂಕೆ_ಖಂಡಗಳು;
	oi.alloc_ಖಂಡಗಳು = 0;
	oi.ಕೊನೆಯ_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_ಖಂಡ_iterate3(fs, ino, ಖಂಡ_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;
	ಆದರೆ (oi.err) {
		err = oi.err;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;
	ext2fs_iblk_ಗಣ(fs, &inode, 0);
	now = ext2fs_P_get_ಸಮಯ(fs);
	ext2fs_inode_xtime_ಗಣ(&inode, i_atime, now);
	ext2fs_inode_xtime_ಗಣ(&inode, i_ctime, now);
	ext2fs_inode_xtime_ಗಣ(&inode, i_mtime, now);
	inode.i_links_ಎಣಿಕೆ = 1;
	inode.i_ಬಹುಲಕ = LINUX_S_IFREG | 0600;
	ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(fs, &inode, oi.alloc_ಖಂಡಗಳು);
	err = ext2fs_inode_ಗಾತ್ರ_ಗಣ(fs, &inode,
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)fs->blocksize * ಅಂಕೆ_ಖಂಡಗಳು);
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_ಗಣ_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
ಹೊರಹೋಗು:
	ಆದರೆ (buf)
		ext2fs_free_mem(&buf);
	ಆದರೆ (zerobuf)
		ext2fs_free_mem(&zerobuf);
	ಹಿಂತಿರುಗಿ err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_ಖಂಡಗಳು(ext2_filsys fs)
{
	__u64 ಅಂಕೆ_ಖಂಡಗಳು = ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super);
	e2_blkcnt_t blks = 512;

	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < 128 * 1024)
		blks = 32;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < 2 * 1024 * 1024)
		blks = ಅಂಕೆ_ಖಂಡಗಳು / 4096;
	ಹಿಂತಿರುಗಿ (blks + EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs)) & ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
}

ಸ್ಥಿರ errcode_t ext2fs_orphan_file_ಖಂಡ_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, ಅಕ್ಷರ *buf,
					       __u32 *crcp)
{
	ರಚನೆ ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	*crcp = ext2fs_do_orphan_file_ಖಂಡ_csum(fs, ino, inode.i_generation,
						 blk, buf);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_orphan_file_ಖಂಡ_csum_ಗಣ(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, ಅಕ್ಷರ *buf)
{
	ರಚನೆ ext4_orphan_ಖಂಡ_ಬಾಲ *ಬಾಲ;
	errcode_t ret;
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 0;

	ಬಾಲ = ext2fs_orphan_ಖಂಡ_ಬಾಲ(fs, buf);
	ret = ext2fs_orphan_file_ಖಂಡ_csum(fs, ino, blk, buf, &ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ 0;
	ಬಾಲ->ob_checksum = ext2fs_cpu_ಗಮ್ಯ_le32(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ ext2fs_orphan_file_ಖಂಡ_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, ಅಕ್ಷರ *buf)
{
	ರಚನೆ ext4_orphan_ಖಂಡ_ಬಾಲ *ಬಾಲ;
	__u32 ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
	errcode_t retval;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super))
		ಹಿಂತಿರುಗಿ 1;
	retval = ext2fs_orphan_file_ಖಂಡ_csum(fs, ino, blk, buf, &ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ 0;
	ಬಾಲ = ext2fs_orphan_ಖಂಡ_ಬಾಲ(fs, buf);
	ಹಿಂತಿರುಗಿ ext2fs_le32_ಗಮ್ಯ_cpu(ಬಾಲ->ob_checksum) == ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
}
