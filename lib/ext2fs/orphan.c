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

#شامل "config.h"
#شامل <string.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

#تعریف_کرو EXT4_MAX_ORPHAN_FILE_بلاکیں 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	ساخت ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	اگر (err)
		واپس err;

	err = ext2fs_punch(fs, ino, &inode, تہی, 0, ~0ULL);
	اگر (err)
		واپس err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	memset(&inode, 0, حجم(ساخت ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	واپس err;
}

__u32 ext2fs_do_orphan_file_بلاک_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, حرف *buf)
{
	صحیح inodes_فی_ob = ext2fs_inodes_فی_orphan_بلاک(fs);
	__u32 چکری_جانچ;

	ino = ext2fs_cpu_مقصد_le32(ino);
	gen = ext2fs_cpu_مقصد_le32(gen);
	blk = ext2fs_cpu_مقصد_le64(blk);
	چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (غیرعلامتی حرف *)&ino,
			       حجم(ino));
	چکری_جانچ = ext2fs_crc32c_le(چکری_جانچ, (غیرعلامتی حرف *)&gen, حجم(gen));
	چکری_جانچ = ext2fs_crc32c_le(چکری_جانچ, (غیرعلامتی حرف *)&blk, حجم(blk));
	چکری_جانچ = ext2fs_crc32c_le(چکری_جانچ, (غیرعلامتی حرف *)buf,
				inodes_فی_ob * حجم(__u32));

	واپس چکری_جانچ;
}

ساخت mkorphan_info {
	حرف *buf;
	حرف *zerobuf;
	blk_t عدد_بلاکیں;
	blk_t alloc_بلاکیں;
	blk64_t آخری_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ساکن صحیح mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_بلاک EXT2FS_ATTR((غیر_مستعمل)),
			 صحیح		ref_انحراف EXT2FS_ATTR((غیر_مستعمل)),
			 خالی		*priv_ڈیٹا)
{
	ساخت mkorphan_info *oi = (ساخت mkorphan_info *)priv_ڈیٹا;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	اگر (blockcnt &&
	    EXT2FS_B2C(fs, oi->آخری_blk) == EXT2FS_B2C(fs, oi->آخری_blk + 1)) {
		new_blk = oi->آخری_blk + 1;
	} ورنہ {
		err = ext2fs_new_block2(fs, oi->آخری_blk, 0, &new_blk);
		اگر (err) {
			oi->err = err;
			واپس بلاک_ABORT;
		}
		ext2fs_بلاک_alloc_stats2(fs, new_blk, +1);
		oi->alloc_بلاکیں++;
	}
	اگر (blockcnt >= 0) {
		اگر (ext2fs_has_feature_metadata_csum(fs->super)) {
			ساخت ext4_orphan_بلاک_دم *دم;

			دم = ext2fs_orphan_بلاک_دم(fs, oi->buf);
			دم->ob_checksum =
		ext2fs_cpu_مقصد_le32(ext2fs_do_orphan_file_بلاک_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} ورنہ	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	اگر (err) {
		oi->err = err;
		واپس بلاک_ABORT;
	}
	oi->آخری_blk = new_blk;
	*blocknr = new_blk;
	اگر (blockcnt >= 0 && --oi->عدد_بلاکیں == 0)
		واپس بلاک_بدلا | بلاک_ABORT;
	واپس بلاک_بدلا;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t عدد_بلاکیں)
{
	ساخت ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	حرف *buf = تہی, *zerobuf = تہی;
	ساخت mkorphan_info oi;
	ساخت ext4_orphan_بلاک_دم *ob_دم;
	time_t now;

	اگر (عدد_بلاکیں > EXT4_MAX_ORPHAN_FILE_بلاکیں)
		عدد_بلاکیں = EXT4_MAX_ORPHAN_FILE_بلاکیں;

	اگر (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		اگر (err)
			واپس err;
		اگر (EXT2_I_سائز(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			اگر (err)
				واپس err;
		}
	} ورنہ {
		err = ext2fs_new_inode(fs, EXT2_جڑ_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		اگر (err)
			واپس err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	memset(&inode, 0, حجم(ساخت ext2_inode));
	اگر (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		اگر (err)
			واپس err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (err)
		واپس err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	اگر (err)
		جاؤ برآمد;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_دم = ext2fs_orphan_بلاک_دم(fs, buf);
	ob_دم->ob_magic = ext2fs_cpu_مقصد_le32(EXT4_ORPHAN_بلاک_MAGIC);
	oi.عدد_بلاکیں = عدد_بلاکیں;
	oi.alloc_بلاکیں = 0;
	oi.آخری_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_بلاک_iterate3(fs, ino, بلاک_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	اگر (err)
		جاؤ برآمد;
	اگر (oi.err) {
		err = oi.err;
		جاؤ برآمد;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	اگر (err)
		جاؤ برآمد;
	ext2fs_iblk_سیٹ(fs, &inode, 0);
	now = ext2fs_P_get_وقت(fs);
	ext2fs_inode_xtime_سیٹ(&inode, i_atime, now);
	ext2fs_inode_xtime_سیٹ(&inode, i_ctime, now);
	ext2fs_inode_xtime_سیٹ(&inode, i_mtime, now);
	inode.i_links_گنتی = 1;
	inode.i_منوال = LINUX_S_IFREG | 0600;
	ext2fs_iblk_جوڑو_بلاکیں(fs, &inode, oi.alloc_بلاکیں);
	err = ext2fs_inode_سائز_سیٹ(fs, &inode,
			(غیرعلامتی طویل طویل)fs->blocksize * عدد_بلاکیں);
	اگر (err)
		جاؤ برآمد;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	اگر (err)
		جاؤ برآمد;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_سیٹ_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
برآمد:
	اگر (buf)
		ext2fs_free_mem(&buf);
	اگر (zerobuf)
		ext2fs_free_mem(&zerobuf);
	واپس err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_بلاکیں(ext2_filsys fs)
{
	__u64 عدد_بلاکیں = ext2fs_بلاکیں_گنتی(fs->super);
	e2_blkcnt_t blks = 512;

	اگر (عدد_بلاکیں < 128 * 1024)
		blks = 32;
	ورنہ اگر (عدد_بلاکیں < 2 * 1024 * 1024)
		blks = عدد_بلاکیں / 4096;
	واپس (blks + EXT2_FS_CLUSTER_نقاب(fs)) & ~EXT2_FS_CLUSTER_نقاب(fs);
}

ساکن errcode_t ext2fs_orphan_file_بلاک_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, حرف *buf,
					       __u32 *crcp)
{
	ساخت ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		واپس retval;
	*crcp = ext2fs_do_orphan_file_بلاک_csum(fs, ino, inode.i_generation,
						 blk, buf);
	واپس 0;
}

errcode_t ext2fs_orphan_file_بلاک_csum_سیٹ(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, حرف *buf)
{
	ساخت ext4_orphan_بلاک_دم *دم;
	errcode_t ret;
	__u32 چکری_جانچ = 0;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 0;

	دم = ext2fs_orphan_بلاک_دم(fs, buf);
	ret = ext2fs_orphan_file_بلاک_csum(fs, ino, blk, buf, &چکری_جانچ);
	اگر (ret)
		واپس 0;
	دم->ob_checksum = ext2fs_cpu_مقصد_le32(چکری_جانچ);
	واپس ret;
}

صحیح ext2fs_orphan_file_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, حرف *buf)
{
	ساخت ext4_orphan_بلاک_دم *دم;
	__u32 چکری_جانچ;
	errcode_t retval;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super))
		واپس 1;
	retval = ext2fs_orphan_file_بلاک_csum(fs, ino, blk, buf, &چکری_جانچ);
	اگر (retval)
		واپس 0;
	دم = ext2fs_orphan_بلاک_دم(fs, buf);
	واپس ext2fs_le32_مقصد_cpu(دم->ob_checksum) == چکری_جانچ;
}
