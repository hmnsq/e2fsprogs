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

#అన్తర్భావ "config.h"
#అన్తర్భావ <string.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

#నిర్వచించు EXT4_MAX_ORPHAN_FILE_ఖండాలు 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	నిర్మాణం ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (err)
		తిరిగి err;

	err = ext2fs_punch(fs, ino, &inode, ఖాళీ, 0, ~0ULL);
	ఒకవేళ (err)
		తిరిగి err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, పరిమాణం(నిర్మాణం ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	తిరిగి err;
}

__u32 ext2fs_do_orphan_file_ఖండం_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, అక్షరం *buf)
{
	పూర్ణాంకం inodes_చొప్పున_ob = ext2fs_inodes_చొప్పున_orphan_ఖండం(fs);
	__u32 చక్రీయ_తనిఖీ;

	ino = ext2fs_cpu_గమ్యం_le32(ino);
	gen = ext2fs_cpu_గమ్యం_le32(gen);
	blk = ext2fs_cpu_గమ్యం_le64(blk);
	చక్రీయ_తనిఖీ = ext2fs_crc32c_le(fs->csum_విత్తనం, (గుర్తులేని అక్షరం *)&ino,
			       పరిమాణం(ino));
	చక్రీయ_తనిఖీ = ext2fs_crc32c_le(చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)&gen, పరిమాణం(gen));
	చక్రీయ_తనిఖీ = ext2fs_crc32c_le(చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)&blk, పరిమాణం(blk));
	చక్రీయ_తనిఖీ = ext2fs_crc32c_le(చక్రీయ_తనిఖీ, (గుర్తులేని అక్షరం *)buf,
				inodes_చొప్పున_ob * పరిమాణం(__u32));

	తిరిగి చక్రీయ_తనిఖీ;
}

నిర్మాణం mkorphan_info {
	అక్షరం *buf;
	అక్షరం *zerobuf;
	blk_t అంకె_ఖండాలు;
	blk_t alloc_ఖండాలు;
	blk64_t చివరి_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

స్థిర పూర్ణాంకం mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_ఖండం EXT2FS_ATTR((వాడని)),
			 పూర్ణాంకం		ref_స్థానభ్రంశం EXT2FS_ATTR((వాడని)),
			 శూన్య		*priv_దత్తాంశం)
{
	నిర్మాణం mkorphan_info *oi = (నిర్మాణం mkorphan_info *)priv_దత్తాంశం;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	ఒకవేళ (blockcnt &&
	    EXT2FS_B2C(fs, oi->చివరి_blk) == EXT2FS_B2C(fs, oi->చివరి_blk + 1)) {
		new_blk = oi->చివరి_blk + 1;
	} లేకపోతే {
		err = ext2fs_new_block2(fs, oi->చివరి_blk, 0, &new_blk);
		ఒకవేళ (err) {
			oi->err = err;
			తిరిగి ఖండం_ABORT;
		}
		ext2fs_ఖండం_alloc_stats2(fs, new_blk, +1);
		oi->alloc_ఖండాలు++;
	}
	ఒకవేళ (blockcnt >= 0) {
		ఒకవేళ (ext2fs_has_feature_metadata_csum(fs->super)) {
			నిర్మాణం ext4_orphan_ఖండం_తోక *తోక;

			తోక = ext2fs_orphan_ఖండం_తోక(fs, oi->buf);
			తోక->ob_checksum =
		ext2fs_cpu_గమ్యం_le32(ext2fs_do_orphan_file_ఖండం_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} లేకపోతే	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	ఒకవేళ (err) {
		oi->err = err;
		తిరిగి ఖండం_ABORT;
	}
	oi->చివరి_blk = new_blk;
	*blocknr = new_blk;
	ఒకవేళ (blockcnt >= 0 && --oi->అంకె_ఖండాలు == 0)
		తిరిగి ఖండం_మారింది | ఖండం_ABORT;
	తిరిగి ఖండం_మారింది;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t అంకె_ఖండాలు)
{
	నిర్మాణం ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	అక్షరం *buf = ఖాళీ, *zerobuf = ఖాళీ;
	నిర్మాణం mkorphan_info oi;
	నిర్మాణం ext4_orphan_ఖండం_తోక *ob_తోక;
	time_t now;

	ఒకవేళ (అంకె_ఖండాలు > EXT4_MAX_ORPHAN_FILE_ఖండాలు)
		అంకె_ఖండాలు = EXT4_MAX_ORPHAN_FILE_ఖండాలు;

	ఒకవేళ (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		ఒకవేళ (err)
			తిరిగి err;
		ఒకవేళ (EXT2_I_కొలత(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			ఒకవేళ (err)
				తిరిగి err;
		}
	} లేకపోతే {
		err = ext2fs_new_inode(fs, EXT2_మూలం_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		ఒకవేళ (err)
			తిరిగి err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, పరిమాణం(నిర్మాణం ext2_inode));
	ఒకవేళ (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		ఒకవేళ (err)
			తిరిగి err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	ఒకవేళ (err)
		తిరిగి err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_తోక = ext2fs_orphan_ఖండం_తోక(fs, buf);
	ob_తోక->ob_magic = ext2fs_cpu_గమ్యం_le32(EXT4_ORPHAN_ఖండం_MAGIC);
	oi.అంకె_ఖండాలు = అంకె_ఖండాలు;
	oi.alloc_ఖండాలు = 0;
	oi.చివరి_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_ఖండం_iterate3(fs, ino, ఖండం_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;
	ఒకవేళ (oi.err) {
		err = oi.err;
		వెళ్ళు బహిర్గతం;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;
	ext2fs_iblk_సమితి(fs, &inode, 0);
	now = ext2fs_P_get_సమయం(fs);
	ext2fs_inode_xtime_సమితి(&inode, i_atime, now);
	ext2fs_inode_xtime_సమితి(&inode, i_ctime, now);
	ext2fs_inode_xtime_సమితి(&inode, i_mtime, now);
	inode.i_links_లెక్క = 1;
	inode.i_బహుళకం = LINUX_S_IFREG | 0600;
	ext2fs_iblk_కూడు_ఖండాలు(fs, &inode, oi.alloc_ఖండాలు);
	err = ext2fs_inode_కొలత_సమితి(fs, &inode,
			(గుర్తులేని దీర్ఘ దీర్ఘ)fs->blocksize * అంకె_ఖండాలు);
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_సమితి_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
బహిర్గతం:
	ఒకవేళ (buf)
		ext2fs_free_mem(&buf);
	ఒకవేళ (zerobuf)
		ext2fs_free_mem(&zerobuf);
	తిరిగి err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_ఖండాలు(ext2_filsys fs)
{
	__u64 అంకె_ఖండాలు = ext2fs_ఖండాలు_లెక్క(fs->super);
	e2_blkcnt_t blks = 512;

	ఒకవేళ (అంకె_ఖండాలు < 128 * 1024)
		blks = 32;
	లేకపోతే ఒకవేళ (అంకె_ఖండాలు < 2 * 1024 * 1024)
		blks = అంకె_ఖండాలు / 4096;
	తిరిగి (blks + EXT2_FS_CLUSTER_ముసుగు(fs)) & ~EXT2_FS_CLUSTER_ముసుగు(fs);
}

స్థిర errcode_t ext2fs_orphan_file_ఖండం_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, అక్షరం *buf,
					       __u32 *crcp)
{
	నిర్మాణం ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;
	*crcp = ext2fs_do_orphan_file_ఖండం_csum(fs, ino, inode.i_generation,
						 blk, buf);
	తిరిగి 0;
}

errcode_t ext2fs_orphan_file_ఖండం_csum_సమితి(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, అక్షరం *buf)
{
	నిర్మాణం ext4_orphan_ఖండం_తోక *తోక;
	errcode_t ret;
	__u32 చక్రీయ_తనిఖీ = 0;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 0;

	తోక = ext2fs_orphan_ఖండం_తోక(fs, buf);
	ret = ext2fs_orphan_file_ఖండం_csum(fs, ino, blk, buf, &చక్రీయ_తనిఖీ);
	ఒకవేళ (ret)
		తిరిగి 0;
	తోక->ob_checksum = ext2fs_cpu_గమ్యం_le32(చక్రీయ_తనిఖీ);
	తిరిగి ret;
}

పూర్ణాంకం ext2fs_orphan_file_ఖండం_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, అక్షరం *buf)
{
	నిర్మాణం ext4_orphan_ఖండం_తోక *తోక;
	__u32 చక్రీయ_తనిఖీ;
	errcode_t retval;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super))
		తిరిగి 1;
	retval = ext2fs_orphan_file_ఖండం_csum(fs, ino, blk, buf, &చక్రీయ_తనిఖీ);
	ఒకవేళ (retval)
		తిరిగి 0;
	తోక = ext2fs_orphan_ఖండం_తోక(fs, buf);
	తిరిగి ext2fs_le32_గమ్యం_cpu(తోక->ob_checksum) == చక్రీయ_తనిఖీ;
}
