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

#تعریف_کړه EXT4_MAX_ORPHAN_FILE_بلاکونه 512

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

__u32 ext2fs_do_orphan_file_بلاک_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, حرف *buf)
{
	صحيح inodes_فی_ob = ext2fs_inodes_فی_orphan_بلاک(fs);
	__u32 دوراني_ازموینه;

	ino = ext2fs_cpu_منزل_le32(ino);
	gen = ext2fs_cpu_منزل_le32(gen);
	blk = ext2fs_cpu_منزل_le64(blk);
	دوراني_ازموینه = ext2fs_crc32c_le(fs->csum_تخم, (بینښه حرف *)&ino,
			       حجم(ino));
	دوراني_ازموینه = ext2fs_crc32c_le(دوراني_ازموینه, (بینښه حرف *)&gen, حجم(gen));
	دوراني_ازموینه = ext2fs_crc32c_le(دوراني_ازموینه, (بینښه حرف *)&blk, حجم(blk));
	دوراني_ازموینه = ext2fs_crc32c_le(دوراني_ازموینه, (بینښه حرف *)buf,
				inodes_فی_ob * حجم(__u32));

	أرجع دوراني_ازموینه;
}

بنية mkorphan_info {
	حرف *buf;
	حرف *zerobuf;
	blk_t عدد_بلاکونه;
	blk_t alloc_بلاکونه;
	blk64_t وروستی_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ساكن صحيح mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_بلاک EXT2FS_ATTR((ناکارول_شوی)),
			 صحيح		ref_دننه_واټن EXT2FS_ATTR((ناکارول_شوی)),
			 فراغ		*priv_معلومات)
{
	بنية mkorphan_info *oi = (بنية mkorphan_info *)priv_معلومات;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	إذا (blockcnt &&
	    EXT2FS_B2C(fs, oi->وروستی_blk) == EXT2FS_B2C(fs, oi->وروستی_blk + 1)) {
		new_blk = oi->وروستی_blk + 1;
	} وإلا {
		err = ext2fs_new_block2(fs, oi->وروستی_blk, 0, &new_blk);
		إذا (err) {
			oi->err = err;
			أرجع بلاک_ABORT;
		}
		ext2fs_بلاک_alloc_stats2(fs, new_blk, +1);
		oi->alloc_بلاکونه++;
	}
	إذا (blockcnt >= 0) {
		إذا (ext2fs_has_feature_metadata_csum(fs->super)) {
			بنية ext4_orphan_بلاک_لکۍ *لکۍ;

			لکۍ = ext2fs_orphan_بلاک_لکۍ(fs, oi->buf);
			لکۍ->ob_checksum =
		ext2fs_cpu_منزل_le32(ext2fs_do_orphan_file_بلاک_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} وإلا	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	إذا (err) {
		oi->err = err;
		أرجع بلاک_ABORT;
	}
	oi->وروستی_blk = new_blk;
	*blocknr = new_blk;
	إذا (blockcnt >= 0 && --oi->عدد_بلاکونه == 0)
		أرجع بلاک_بدل_شوی | بلاک_ABORT;
	أرجع بلاک_بدل_شوی;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t عدد_بلاکونه)
{
	بنية ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	حرف *buf = عدم, *zerobuf = عدم;
	بنية mkorphan_info oi;
	بنية ext4_orphan_بلاک_لکۍ *ob_لکۍ;
	time_t now;

	إذا (عدد_بلاکونه > EXT4_MAX_ORPHAN_FILE_بلاکونه)
		عدد_بلاکونه = EXT4_MAX_ORPHAN_FILE_بلاکونه;

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
		err = ext2fs_new_inode(fs, EXT2_ریښه_INO, LINUX_S_IFREG | 0600,
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
		اذهب بهر;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_لکۍ = ext2fs_orphan_بلاک_لکۍ(fs, buf);
	ob_لکۍ->ob_magic = ext2fs_cpu_منزل_le32(EXT4_ORPHAN_بلاک_MAGIC);
	oi.عدد_بلاکونه = عدد_بلاکونه;
	oi.alloc_بلاکونه = 0;
	oi.وروستی_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_بلاک_iterate3(fs, ino, بلاک_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	إذا (err)
		اذهب بهر;
	إذا (oi.err) {
		err = oi.err;
		اذهب بهر;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	إذا (err)
		اذهب بهر;
	ext2fs_iblk_سېټ(fs, &inode, 0);
	now = ext2fs_P_get_وخت(fs);
	ext2fs_inode_xtime_سېټ(&inode, i_atime, now);
	ext2fs_inode_xtime_سېټ(&inode, i_ctime, now);
	ext2fs_inode_xtime_سېټ(&inode, i_mtime, now);
	inode.i_links_شمېر = 1;
	inode.i_ډېرپېښ = LINUX_S_IFREG | 0600;
	ext2fs_iblk_جمع_کړه_بلاکونه(fs, &inode, oi.alloc_بلاکونه);
	err = ext2fs_inode_اندازه_سېټ(fs, &inode,
			(بینښه اوږد اوږد)fs->blocksize * عدد_بلاکونه);
	إذا (err)
		اذهب بهر;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (err)
		اذهب بهر;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_سېټ_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
بهر:
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
e2_blkcnt_t ext2fs_default_orphan_file_بلاکونه(ext2_filsys fs)
{
	__u64 عدد_بلاکونه = ext2fs_بلاکونه_شمېر(fs->super);
	e2_blkcnt_t blks = 512;

	إذا (عدد_بلاکونه < 128 * 1024)
		blks = 32;
	وإلا إذا (عدد_بلاکونه < 2 * 1024 * 1024)
		blks = عدد_بلاکونه / 4096;
	أرجع (blks + EXT2_FS_CLUSTER_ماسک(fs)) & ~EXT2_FS_CLUSTER_ماسک(fs);
}

ساكن errcode_t ext2fs_orphan_file_بلاک_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, حرف *buf,
					       __u32 *crcp)
{
	بنية ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	*crcp = ext2fs_do_orphan_file_بلاک_csum(fs, ino, inode.i_generation,
						 blk, buf);
	أرجع 0;
}

errcode_t ext2fs_orphan_file_بلاک_csum_سېټ(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, حرف *buf)
{
	بنية ext4_orphan_بلاک_لکۍ *لکۍ;
	errcode_t ret;
	__u32 دوراني_ازموینه = 0;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	لکۍ = ext2fs_orphan_بلاک_لکۍ(fs, buf);
	ret = ext2fs_orphan_file_بلاک_csum(fs, ino, blk, buf, &دوراني_ازموینه);
	إذا (ret)
		أرجع 0;
	لکۍ->ob_checksum = ext2fs_cpu_منزل_le32(دوراني_ازموینه);
	أرجع ret;
}

صحيح ext2fs_orphan_file_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, حرف *buf)
{
	بنية ext4_orphan_بلاک_لکۍ *لکۍ;
	__u32 دوراني_ازموینه;
	errcode_t retval;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	retval = ext2fs_orphan_file_بلاک_csum(fs, ino, blk, buf, &دوراني_ازموینه);
	إذا (retval)
		أرجع 0;
	لکۍ = ext2fs_orphan_بلاک_لکۍ(fs, buf);
	أرجع ext2fs_le32_منزل_cpu(لکۍ->ob_checksum) == دوراني_ازموینه;
}
