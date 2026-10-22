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

#وصف_ڪريو EXT4_MAX_ORPHAN_FILE_بلاڪون 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	بنية ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_punch(fs, ino, &inode, عدم, 0, ~0ULL);
	إذا (err)
		أرجع err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, حجم(بنية ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	أرجع err;
}

__u32 ext2fs_do_orphan_file_بلاڪ_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, حرف *buf)
{
	صحيح inodes_في_ob = ext2fs_inodes_في_orphan_بلاڪ(fs);
	__u32 چڪري_جاچ;

	ino = ext2fs_cpu_منزل_le32(ino);
	gen = ext2fs_cpu_منزل_le32(gen);
	blk = ext2fs_cpu_منزل_le64(blk);
	چڪري_جاچ = ext2fs_crc32c_le(fs->csum_ٻج, (بنانشان حرف *)&ino,
			       حجم(ino));
	چڪري_جاچ = ext2fs_crc32c_le(چڪري_جاچ, (بنانشان حرف *)&gen, حجم(gen));
	چڪري_جاچ = ext2fs_crc32c_le(چڪري_جاچ, (بنانشان حرف *)&blk, حجم(blk));
	چڪري_جاچ = ext2fs_crc32c_le(چڪري_جاچ, (بنانشان حرف *)buf,
				inodes_في_ob * حجم(__u32));

	أرجع چڪري_جاچ;
}

بنية mkorphan_info {
	حرف *buf;
	حرف *zerobuf;
	blk_t انگ_بلاڪون;
	blk_t alloc_بلاڪون;
	blk64_t آخري_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ساكن صحيح mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_بلاڪ EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			 صحيح		ref_وٿي EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			 فراغ		*priv_معلومات)
{
	بنية mkorphan_info *oi = (بنية mkorphan_info *)priv_معلومات;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	إذا (blockcnt &&
	    EXT2FS_B2C(fs, oi->آخري_blk) == EXT2FS_B2C(fs, oi->آخري_blk + 1)) {
		new_blk = oi->آخري_blk + 1;
	} وإلا {
		err = ext2fs_new_block2(fs, oi->آخري_blk, 0, &new_blk);
		إذا (err) {
			oi->err = err;
			أرجع بلاڪ_ABORT;
		}
		ext2fs_بلاڪ_alloc_stats2(fs, new_blk, +1);
		oi->alloc_بلاڪون++;
	}
	إذا (blockcnt >= 0) {
		إذا (ext2fs_has_feature_metadata_csum(fs->super)) {
			بنية ext4_orphan_بلاڪ_پڇ *پڇ;

			پڇ = ext2fs_orphan_بلاڪ_پڇ(fs, oi->buf);
			پڇ->ob_checksum =
		ext2fs_cpu_منزل_le32(ext2fs_do_orphan_file_بلاڪ_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} وإلا	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	إذا (err) {
		oi->err = err;
		أرجع بلاڪ_ABORT;
	}
	oi->آخري_blk = new_blk;
	*blocknr = new_blk;
	إذا (blockcnt >= 0 && --oi->انگ_بلاڪون == 0)
		أرجع بلاڪ_بدليو | بلاڪ_ABORT;
	أرجع بلاڪ_بدليو;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t انگ_بلاڪون)
{
	بنية ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	حرف *buf = عدم, *zerobuf = عدم;
	بنية mkorphan_info oi;
	بنية ext4_orphan_بلاڪ_پڇ *ob_پڇ;
	time_t now;

	إذا (انگ_بلاڪون > EXT4_MAX_ORPHAN_FILE_بلاڪون)
		انگ_بلاڪون = EXT4_MAX_ORPHAN_FILE_بلاڪون;

	إذا (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		إذا (err)
			أرجع err;
		إذا (EXT2_I_سائيز(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			إذا (err)
				أرجع err;
		}
	} وإلا {
		err = ext2fs_new_inode(fs, EXT2_پاڙ_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		إذا (err)
			أرجع err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, حجم(بنية ext2_inode));
	إذا (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		إذا (err)
			أرجع err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (err)
		أرجع err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	إذا (err)
		اذهب ٻاهريون;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_پڇ = ext2fs_orphan_بلاڪ_پڇ(fs, buf);
	ob_پڇ->ob_magic = ext2fs_cpu_منزل_le32(EXT4_ORPHAN_بلاڪ_MAGIC);
	oi.انگ_بلاڪون = انگ_بلاڪون;
	oi.alloc_بلاڪون = 0;
	oi.آخري_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_بلاڪ_iterate3(fs, ino, بلاڪ_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	إذا (err)
		اذهب ٻاهريون;
	إذا (oi.err) {
		err = oi.err;
		اذهب ٻاهريون;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	إذا (err)
		اذهب ٻاهريون;
	ext2fs_iblk_سيٽ(fs, &inode, 0);
	now = ext2fs_P_get_وقت(fs);
	ext2fs_inode_xtime_سيٽ(&inode, i_atime, now);
	ext2fs_inode_xtime_سيٽ(&inode, i_ctime, now);
	ext2fs_inode_xtime_سيٽ(&inode, i_mtime, now);
	inode.i_links_ڳڻپ = 1;
	inode.i_گهڻائي = LINUX_S_IFREG | 0600;
	ext2fs_iblk_جوڙيو_بلاڪون(fs, &inode, oi.alloc_بلاڪون);
	err = ext2fs_inode_سائيز_سيٽ(fs, &inode,
			(بنانشان ڊگهو ڊگهو)fs->blocksize * انگ_بلاڪون);
	إذا (err)
		اذهب ٻاهريون;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (err)
		اذهب ٻاهريون;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_سيٽ_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
ٻاهريون:
	إذا (buf)
		ext2fs_free_mem(&buf);
	إذا (zerobuf)
		ext2fs_free_mem(&zerobuf);
	أرجع err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_بلاڪون(ext2_filsys fs)
{
	__u64 انگ_بلاڪون = ext2fs_بلاڪون_ڳڻپ(fs->super);
	e2_blkcnt_t blks = 512;

	إذا (انگ_بلاڪون < 128 * 1024)
		blks = 32;
	وإلا إذا (انگ_بلاڪون < 2 * 1024 * 1024)
		blks = انگ_بلاڪون / 4096;
	أرجع (blks + EXT2_FS_CLUSTER_نقاب(fs)) & ~EXT2_FS_CLUSTER_نقاب(fs);
}

ساكن errcode_t ext2fs_orphan_file_بلاڪ_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, حرف *buf,
					       __u32 *crcp)
{
	بنية ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	*crcp = ext2fs_do_orphan_file_بلاڪ_csum(fs, ino, inode.i_generation,
						 blk, buf);
	أرجع 0;
}

errcode_t ext2fs_orphan_file_بلاڪ_csum_سيٽ(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, حرف *buf)
{
	بنية ext4_orphan_بلاڪ_پڇ *پڇ;
	errcode_t ret;
	__u32 چڪري_جاچ = 0;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	پڇ = ext2fs_orphan_بلاڪ_پڇ(fs, buf);
	ret = ext2fs_orphan_file_بلاڪ_csum(fs, ino, blk, buf, &چڪري_جاچ);
	إذا (ret)
		أرجع 0;
	پڇ->ob_checksum = ext2fs_cpu_منزل_le32(چڪري_جاچ);
	أرجع ret;
}

صحيح ext2fs_orphan_file_بلاڪ_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, حرف *buf)
{
	بنية ext4_orphan_بلاڪ_پڇ *پڇ;
	__u32 چڪري_جاچ;
	errcode_t retval;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	retval = ext2fs_orphan_file_بلاڪ_csum(fs, ino, blk, buf, &چڪري_جاچ);
	إذا (retval)
		أرجع 0;
	پڇ = ext2fs_orphan_بلاڪ_پڇ(fs, buf);
	أرجع ext2fs_le32_منزل_cpu(پڇ->ob_checksum) == چڪري_جاچ;
}
