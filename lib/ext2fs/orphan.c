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

#تعریف_کرو EXT4_MAX_ORPHAN_FILE_بلاکاں 512

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
	__u32 چکری_جانچ;

	ino = ext2fs_cpu_منزل_le32(ino);
	gen = ext2fs_cpu_منزل_le32(gen);
	blk = ext2fs_cpu_منزل_le64(blk);
	چکری_جانچ = ext2fs_crc32c_le(fs->csum_بیج, (بنانشانی حرف *)&ino,
			       حجم(ino));
	چکری_جانچ = ext2fs_crc32c_le(چکری_جانچ, (بنانشانی حرف *)&gen, حجم(gen));
	چکری_جانچ = ext2fs_crc32c_le(چکری_جانچ, (بنانشانی حرف *)&blk, حجم(blk));
	چکری_جانچ = ext2fs_crc32c_le(چکری_جانچ, (بنانشانی حرف *)buf,
				inodes_فی_ob * حجم(__u32));

	أرجع چکری_جانچ;
}

بنية mkorphan_info {
	حرف *buf;
	حرف *zerobuf;
	blk_t عدد_بلاکاں;
	blk_t alloc_بلاکاں;
	blk64_t آخری_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

ساكن صحيح mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_بلاک EXT2FS_ATTR((بن_ورتیا)),
			 صحيح		ref_وتھ EXT2FS_ATTR((بن_ورتیا)),
			 فراغ		*priv_انکڑے)
{
	بنية mkorphan_info *oi = (بنية mkorphan_info *)priv_انکڑے;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	إذا (blockcnt &&
	    EXT2FS_B2C(fs, oi->آخری_blk) == EXT2FS_B2C(fs, oi->آخری_blk + 1)) {
		new_blk = oi->آخری_blk + 1;
	} وإلا {
		err = ext2fs_new_block2(fs, oi->آخری_blk, 0, &new_blk);
		إذا (err) {
			oi->err = err;
			أرجع بلاک_ABORT;
		}
		ext2fs_بلاک_alloc_stats2(fs, new_blk, +1);
		oi->alloc_بلاکاں++;
	}
	إذا (blockcnt >= 0) {
		إذا (ext2fs_has_feature_metadata_csum(fs->super)) {
			بنية ext4_orphan_بلاک_پوچھ *پوچھ;

			پوچھ = ext2fs_orphan_بلاک_پوچھ(fs, oi->buf);
			پوچھ->ob_checksum =
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
	oi->آخری_blk = new_blk;
	*blocknr = new_blk;
	إذا (blockcnt >= 0 && --oi->عدد_بلاکاں == 0)
		أرجع بلاک_بدلیا | بلاک_ABORT;
	أرجع بلاک_بدلیا;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t عدد_بلاکاں)
{
	بنية ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	حرف *buf = عدم, *zerobuf = عدم;
	بنية mkorphan_info oi;
	بنية ext4_orphan_بلاک_پوچھ *ob_پوچھ;
	time_t now;

	إذا (عدد_بلاکاں > EXT4_MAX_ORPHAN_FILE_بلاکاں)
		عدد_بلاکاں = EXT4_MAX_ORPHAN_FILE_بلاکاں;

	إذا (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		إذا (err)
			أرجع err;
		إذا (EXT2_I_ناپ(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			إذا (err)
				أرجع err;
		}
	} وإلا {
		err = ext2fs_new_inode(fs, EXT2_جڑ_INO, LINUX_S_IFREG | 0600,
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
		اذهب باہرلا;
	memset(buf, 0, fs->blocksize);
	memset(zerobuf, 0, fs->blocksize);
	ob_پوچھ = ext2fs_orphan_بلاک_پوچھ(fs, buf);
	ob_پوچھ->ob_magic = ext2fs_cpu_منزل_le32(EXT4_ORPHAN_بلاک_MAGIC);
	oi.عدد_بلاکاں = عدد_بلاکاں;
	oi.alloc_بلاکاں = 0;
	oi.آخری_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_بلاک_iterate3(fs, ino, بلاک_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	إذا (err)
		اذهب باہرلا;
	إذا (oi.err) {
		err = oi.err;
		اذهب باہرلا;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	إذا (err)
		اذهب باہرلا;
	ext2fs_iblk_سیٹ(fs, &inode, 0);
	now = ext2fs_P_get_ویلا(fs);
	ext2fs_inode_xtime_سیٹ(&inode, i_atime, now);
	ext2fs_inode_xtime_سیٹ(&inode, i_ctime, now);
	ext2fs_inode_xtime_سیٹ(&inode, i_mtime, now);
	inode.i_links_گنتی = 1;
	inode.i_منوال = LINUX_S_IFREG | 0600;
	ext2fs_iblk_جوڑو_بلاکاں(fs, &inode, oi.alloc_بلاکاں);
	err = ext2fs_inode_ناپ_سیٹ(fs, &inode,
			(بنانشانی لما لما)fs->blocksize * عدد_بلاکاں);
	إذا (err)
		اذهب باہرلا;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (err)
		اذهب باہرلا;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_سیٹ_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
باہرلا:
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
e2_blkcnt_t ext2fs_default_orphan_file_بلاکاں(ext2_filsys fs)
{
	__u64 عدد_بلاکاں = ext2fs_بلاکاں_گنتی(fs->super);
	e2_blkcnt_t blks = 512;

	إذا (عدد_بلاکاں < 128 * 1024)
		blks = 32;
	وإلا إذا (عدد_بلاکاں < 2 * 1024 * 1024)
		blks = عدد_بلاکاں / 4096;
	أرجع (blks + EXT2_FS_CLUSTER_نقاب(fs)) & ~EXT2_FS_CLUSTER_نقاب(fs);
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

errcode_t ext2fs_orphan_file_بلاک_csum_سیٹ(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, حرف *buf)
{
	بنية ext4_orphan_بلاک_پوچھ *پوچھ;
	errcode_t ret;
	__u32 چکری_جانچ = 0;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 0;

	پوچھ = ext2fs_orphan_بلاک_پوچھ(fs, buf);
	ret = ext2fs_orphan_file_بلاک_csum(fs, ino, blk, buf, &چکری_جانچ);
	إذا (ret)
		أرجع 0;
	پوچھ->ob_checksum = ext2fs_cpu_منزل_le32(چکری_جانچ);
	أرجع ret;
}

صحيح ext2fs_orphan_file_بلاک_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, حرف *buf)
{
	بنية ext4_orphan_بلاک_پوچھ *پوچھ;
	__u32 چکری_جانچ;
	errcode_t retval;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super))
		أرجع 1;
	retval = ext2fs_orphan_file_بلاک_csum(fs, ino, blk, buf, &چکری_جانچ);
	إذا (retval)
		أرجع 0;
	پوچھ = ext2fs_orphan_بلاک_پوچھ(fs, buf);
	أرجع ext2fs_le32_منزل_cpu(پوچھ->ob_checksum) == چکری_جانچ;
}
