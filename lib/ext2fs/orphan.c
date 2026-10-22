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

#تعریف_کن EXT4_MAX_ORPHAN_FILE_بلوکها 512

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

__u32 ext2fs_do_orphan_file_بلوک_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, حرف *buf)
{
	صحيح inodes_در_هر_ob = ext2fs_inodes_در_هر_orphan_بلوک(fs);
	__u32 بررسی_چرخهای;

	ino = ext2fs_cpu_مقصد_le32(ino);
	gen = ext2fs_cpu_مقصد_le32(gen);
	blk = ext2fs_cpu_مقصد_le64(blk);
	بررسی_چرخهای = ext2fs_crc32c_le(fs->csum_دانه, (بیعلامت حرف *)&ino,
			       حجم(ino));
	بررسی_چرخهای = ext2fs_crc32c_le(بررسی_چرخهای, (بیعلامت حرف *)&gen, حجم(gen));
	بررسی_چرخهای = ext2fs_crc32c_le(بررسی_چرخهای, (بیعلامت حرف *)&blk, حجم(blk));
	بررسی_چرخهای = ext2fs_crc32c_le(بررسی_چرخهای, (بیعلامت حرف *)buf,
				inodes_در_هر_ob * حجم(__u32));

	أرجع بررسی_چرخهای;
}

بنية mkorphan_info {
	حرف *buf;
	حرف *zerobuf;
	blk_t رقم_بلوکها;
	blk_t alloc_بلوکها;
	blk64_t آخرین_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ساكن صحيح mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_بلوک EXT2FS_ATTR((استفادهنشده)),
			 صحيح		ref_جابه_جایی EXT2FS_ATTR((استفادهنشده)),
			 فراغ		*priv_داده)
{
	بنية mkorphan_info *oi = (بنية mkorphan_info *)priv_داده;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	إذا (blockcnt &&
	    EXT2FS_B2C(fs, oi->آخرین_blk) == EXT2FS_B2C(fs, oi->آخرین_blk + 1)) {
		new_blk = oi->آخرین_blk + 1;
	} وإلا {
		err = ext2fs_new_block2(fs, oi->آخرین_blk, 0, &new_blk);
		إذا (err) {
			oi->err = err;
			أرجع بلوک_ABORT;
		}
		ext2fs_بلوک_alloc_stats2(fs, new_blk, +1);
		oi->alloc_بلوکها++;
	}
	إذا (blockcnt >= 0) {
		إذا (ext2fs_has_feature_metadata_csum(fs->super)) {
			بنية ext4_orphan_بلوک_دم *دم;

			دم = ext2fs_orphan_بلوک_دم(fs, oi->buf);
			دم->ob_checksum =
		ext2fs_cpu_مقصد_le32(ext2fs_do_orphan_file_بلوک_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} وإلا	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	إذا (err) {
		oi->err = err;
		أرجع بلوک_ABORT;
	}
	oi->آخرین_blk = new_blk;
	*blocknr = new_blk;
	إذا (blockcnt >= 0 && --oi->رقم_بلوکها == 0)
		أرجع بلوک_تغییریافته | بلوک_ABORT;
	أرجع بلوک_تغییریافته;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t رقم_بلوکها)
{
	بنية ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	حرف *buf = عدم, *zerobuf = عدم;
	بنية mkorphan_info oi;
	بنية ext4_orphan_بلوک_دم *ob_دم;
	time_t now;

	إذا (رقم_بلوکها > EXT4_MAX_ORPHAN_FILE_بلوکها)
		رقم_بلوکها = EXT4_MAX_ORPHAN_FILE_بلوکها;

	إذا (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		إذا (err)
			أرجع err;
		إذا (EXT2_I_اندازه(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			إذا (err)
				أرجع err;
		}
	} وإلا {
		err = ext2fs_new_inode(fs, EXT2_ریشه_INO, LINUX_S_IFREG | 0600,
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
		اذهب بیرون;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_دم = ext2fs_orphan_بلوک_دم(fs, buf);
	ob_دم->ob_magic = ext2fs_cpu_مقصد_le32(EXT4_ORPHAN_بلوک_MAGIC);
	oi.رقم_بلوکها = رقم_بلوکها;
	oi.alloc_بلوکها = 0;
	oi.آخرین_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_بلوک_iterate3(fs, ino, بلوک_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	إذا (err)
		اذهب بیرون;
	إذا (oi.err) {
		err = oi.err;
		اذهب بیرون;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	إذا (err)
		اذهب بیرون;
	ext2fs_iblk_مجموعه(fs, &inode, 0);
	now = ext2fs_P_get_زمان(fs);
	ext2fs_inode_xtime_مجموعه(&inode, i_atime, now);
	ext2fs_inode_xtime_مجموعه(&inode, i_ctime, now);
	ext2fs_inode_xtime_مجموعه(&inode, i_mtime, now);
	inode.i_links_شمار = 1;
	inode.i_نما = LINUX_S_IFREG | 0600;
	ext2fs_iblk_جمع_کن_بلوکها(fs, &inode, oi.alloc_بلوکها);
	err = ext2fs_inode_اندازه_مجموعه(fs, &inode,
			(بیعلامت دراز دراز)fs->blocksize * رقم_بلوکها);
	إذا (err)
		اذهب بیرون;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (err)
		اذهب بیرون;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_مجموعه_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
بیرون:
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
e2_blkcnt_t ext2fs_default_orphan_file_بلوکها(ext2_filsys fs)
{
	__u64 رقم_بلوکها = ext2fs_بلوکها_شمار(fs->super);
	e2_blkcnt_t blks = 512;

	إذا (رقم_بلوکها < 128 * 1024)
		blks = 32;
	وإلا إذا (رقم_بلوکها < 2 * 1024 * 1024)
		blks = رقم_بلوکها / 4096;
	أرجع (blks + EXT2_FS_CLUSTER_ماسک(fs)) & ~EXT2_FS_CLUSTER_ماسک(fs);
}

ساكن errcode_t ext2fs_orphan_file_بلوک_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, حرف *buf,
					       __u32 *crcp)
{
	بنية ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	*crcp = ext2fs_do_orphan_file_بلوک_csum(fs, ino, inode.i_generation,
						 blk, buf);
	أرجع 0;
}

errcode_t ext2fs_orphan_file_بلوک_csum_مجموعه(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, حرف *buf)
{
	بنية ext4_orphan_بلوک_دم *دم;
	errcode_t ret;
	__u32 بررسی_چرخهای = 0;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	دم = ext2fs_orphan_بلوک_دم(fs, buf);
	ret = ext2fs_orphan_file_بلوک_csum(fs, ino, blk, buf, &بررسی_چرخهای);
	إذا (ret)
		أرجع 0;
	دم->ob_checksum = ext2fs_cpu_مقصد_le32(بررسی_چرخهای);
	أرجع ret;
}

صحيح ext2fs_orphan_file_بلوک_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, حرف *buf)
{
	بنية ext4_orphan_بلوک_دم *دم;
	__u32 بررسی_چرخهای;
	errcode_t retval;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	retval = ext2fs_orphan_file_بلوک_csum(fs, ino, blk, buf, &بررسی_چرخهای);
	إذا (retval)
		أرجع 0;
	دم = ext2fs_orphan_بلوک_دم(fs, buf);
	أرجع ext2fs_le32_مقصد_cpu(دم->ob_checksum) == بررسی_چرخهای;
}
