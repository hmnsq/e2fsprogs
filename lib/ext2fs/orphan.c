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

#अन्तर्भाव "config.h"
#अन्तर्भाव <string.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#परिभाषय EXT4_MAX_ORPHAN_FILE_खण्डानि 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	संरचना ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	यदि (err)
		प्रत्यागम err;

	err = ext2fs_punch(fs, ino, &inode, रिक्त, 0, ~0ULL);
	यदि (err)
		प्रत्यागम err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	स्मृतिं_पूरय(&inode, 0, परिमाण(संरचना ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	प्रत्यागम err;
}

__u32 ext2fs_do_orphan_file_खण्डम्_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, वर्ण *buf)
{
	पूर्णाङ्क inodes_प्रत्येकम्_ob = ext2fs_inodes_प्रत्येकम्_orphan_खण्डम्(fs);
	__u32 crc;

	ino = ext2fs_cpu_गन्तव्यम्_le32(ino);
	gen = ext2fs_cpu_गन्तव्यम्_le32(gen);
	blk = ext2fs_cpu_गन्तव्यम्_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_बीजम्, (अचिह्नित वर्ण *)&ino,
			       परिमाण(ino));
	crc = ext2fs_crc32c_le(crc, (अचिह्नित वर्ण *)&gen, परिमाण(gen));
	crc = ext2fs_crc32c_le(crc, (अचिह्नित वर्ण *)&blk, परिमाण(blk));
	crc = ext2fs_crc32c_le(crc, (अचिह्नित वर्ण *)buf,
				inodes_प्रत्येकम्_ob * परिमाण(__u32));

	प्रत्यागम crc;
}

संरचना mkorphan_info {
	वर्ण *buf;
	वर्ण *zerobuf;
	blk_t अङ्कः_खण्डानि;
	blk_t alloc_खण्डानि;
	blk64_t चरमः_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

स्थिर पूर्णाङ्क mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_खण्डम् EXT2FS_ATTR((अप्रयुक्तम्)),
			 पूर्णाङ्क		ref_विस्थापनम् EXT2FS_ATTR((अप्रयुक्तम्)),
			 शून्य		*priv_दत्त)
{
	संरचना mkorphan_info *oi = (संरचना mkorphan_info *)priv_दत्त;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	यदि (blockcnt &&
	    EXT2FS_B2C(fs, oi->चरमः_blk) == EXT2FS_B2C(fs, oi->चरमः_blk + 1)) {
		new_blk = oi->चरमः_blk + 1;
	} अन्यथा {
		err = ext2fs_new_block2(fs, oi->चरमः_blk, 0, &new_blk);
		यदि (err) {
			oi->err = err;
			प्रत्यागम खण्डम्_ABORT;
		}
		ext2fs_खण्डम्_alloc_stats2(fs, new_blk, +1);
		oi->alloc_खण्डानि++;
	}
	यदि (blockcnt >= 0) {
		यदि (ext2fs_has_feature_metadata_csum(fs->super)) {
			संरचना ext4_orphan_खण्डम्_पुच्छ *पुच्छ;

			पुच्छ = ext2fs_orphan_खण्डम्_पुच्छ(fs, oi->buf);
			पुच्छ->ob_checksum =
		ext2fs_cpu_गन्तव्यम्_le32(ext2fs_do_orphan_file_खण्डम्_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} अन्यथा	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	यदि (err) {
		oi->err = err;
		प्रत्यागम खण्डम्_ABORT;
	}
	oi->चरमः_blk = new_blk;
	*blocknr = new_blk;
	यदि (blockcnt >= 0 && --oi->अङ्कः_खण्डानि == 0)
		प्रत्यागम खण्डम्_परिवर्तितम् | खण्डम्_ABORT;
	प्रत्यागम खण्डम्_परिवर्तितम्;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t अङ्कः_खण्डानि)
{
	संरचना ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	वर्ण *buf = रिक्त, *zerobuf = रिक्त;
	संरचना mkorphan_info oi;
	संरचना ext4_orphan_खण्डम्_पुच्छ *ob_पुच्छ;
	time_t now;

	यदि (अङ्कः_खण्डानि > EXT4_MAX_ORPHAN_FILE_खण्डानि)
		अङ्कः_खण्डानि = EXT4_MAX_ORPHAN_FILE_खण्डानि;

	यदि (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		यदि (err)
			प्रत्यागम err;
		यदि (EXT2_I_आकार(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			यदि (err)
				प्रत्यागम err;
		}
	} अन्यथा {
		err = ext2fs_new_inode(fs, EXT2_मूलम्_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		यदि (err)
			प्रत्यागम err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	स्मृतिं_पूरय(&inode, 0, परिमाण(संरचना ext2_inode));
	यदि (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		यदि (err)
			प्रत्यागम err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (err)
		प्रत्यागम err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	यदि (err)
		गच्छ बहिर्गमनम्;
	स्मृतिं_पूरय(buf, 0, fs->blocksize);
	स्मृतिं_पूरय(zerobuf, 0, fs->blocksize);
	ob_पुच्छ = ext2fs_orphan_खण्डम्_पुच्छ(fs, buf);
	ob_पुच्छ->ob_magic = ext2fs_cpu_गन्तव्यम्_le32(EXT4_ORPHAN_खण्डम्_MAGIC);
	oi.अङ्कः_खण्डानि = अङ्कः_खण्डानि;
	oi.alloc_खण्डानि = 0;
	oi.चरमः_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_खण्डम्_iterate3(fs, ino, खण्डम्_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	यदि (err)
		गच्छ बहिर्गमनम्;
	यदि (oi.err) {
		err = oi.err;
		गच्छ बहिर्गमनम्;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	यदि (err)
		गच्छ बहिर्गमनम्;
	ext2fs_iblk_समुच्चयः(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_समुच्चयः(&inode, i_atime, now);
	ext2fs_inode_xtime_समुच्चयः(&inode, i_ctime, now);
	ext2fs_inode_xtime_समुच्चयः(&inode, i_mtime, now);
	inode.i_links_गणनम् = 1;
	inode.i_बहुलकम् = LINUX_S_IFREG | 0600;
	ext2fs_iblk_सङ्कलय_खण्डानि(fs, &inode, oi.alloc_खण्डानि);
	err = ext2fs_inode_आकार_समुच्चयः(fs, &inode,
			(अचिह्नित दीर्घ दीर्घ)fs->blocksize * अङ्कः_खण्डानि);
	यदि (err)
		गच्छ बहिर्गमनम्;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	यदि (err)
		गच्छ बहिर्गमनम्;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_समुच्चयः_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
बहिर्गमनम्:
	यदि (buf)
		ext2fs_free_mem(&buf);
	यदि (zerobuf)
		ext2fs_free_mem(&zerobuf);
	प्रत्यागम err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_खण्डानि(ext2_filsys fs)
{
	__u64 अङ्कः_खण्डानि = ext2fs_खण्डानि_गणनम्(fs->super);
	e2_blkcnt_t blks = 512;

	यदि (अङ्कः_खण्डानि < 128 * 1024)
		blks = 32;
	अन्यथा यदि (अङ्कः_खण्डानि < 2 * 1024 * 1024)
		blks = अङ्कः_खण्डानि / 4096;
	प्रत्यागम (blks + EXT2_FS_CLUSTER_आवरणम्(fs)) & ~EXT2_FS_CLUSTER_आवरणम्(fs);
}

स्थिर errcode_t ext2fs_orphan_file_खण्डम्_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, वर्ण *buf,
					       __u32 *crcp)
{
	संरचना ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	यदि (retval)
		प्रत्यागम retval;
	*crcp = ext2fs_do_orphan_file_खण्डम्_csum(fs, ino, inode.i_generation,
						 blk, buf);
	प्रत्यागम 0;
}

errcode_t ext2fs_orphan_file_खण्डम्_csum_समुच्चयः(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, वर्ण *buf)
{
	संरचना ext4_orphan_खण्डम्_पुच्छ *पुच्छ;
	errcode_t ret;
	__u32 crc = 0;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 0;

	पुच्छ = ext2fs_orphan_खण्डम्_पुच्छ(fs, buf);
	ret = ext2fs_orphan_file_खण्डम्_csum(fs, ino, blk, buf, &crc);
	यदि (ret)
		प्रत्यागम 0;
	पुच्छ->ob_checksum = ext2fs_cpu_गन्तव्यम्_le32(crc);
	प्रत्यागम ret;
}

पूर्णाङ्क ext2fs_orphan_file_खण्डम्_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, वर्ण *buf)
{
	संरचना ext4_orphan_खण्डम्_पुच्छ *पुच्छ;
	__u32 crc;
	errcode_t retval;

	यदि (!ext2fs_has_feature_metadata_csum(fs->super))
		प्रत्यागम 1;
	retval = ext2fs_orphan_file_खण्डम्_csum(fs, ino, blk, buf, &crc);
	यदि (retval)
		प्रत्यागम 0;
	पुच्छ = ext2fs_orphan_खण्डम्_पुच्छ(fs, buf);
	प्रत्यागम ext2fs_le32_गन्तव्यम्_cpu(पुच्छ->ob_checksum) == crc;
}
