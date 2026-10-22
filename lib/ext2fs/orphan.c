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

#समावेश "config.h"
#समावेश <string.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

#घोषणा EXT4_MAX_ORPHAN_FILE_BLOCKS 512

errcode_t ext2fs_truncate_orphan_file(ext2_filsys fs)
{
	काष्ठा ext2_inode inode;
	errcode_t err;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;

	err = ext2fs_read_inode(fs, ino, &inode);
	अगर (err)
		वापस err;

	err = ext2fs_punch(fs, ino, &inode, शून्य, 0, ~0ULL);
	अगर (err)
		वापस err;

	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
	स_रखो(&inode, 0, माप(काष्ठा ext2_inode));
	err = ext2fs_write_inode(fs, ino, &inode);

	ext2fs_clear_feature_orphan_file(fs->super);
	ext2fs_clear_feature_orphan_present(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;

	वापस err;
}

__u32 ext2fs_do_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
				       __u32 gen, blk64_t blk, अक्षर *buf)
{
	पूर्णांक inodes_per_ob = ext2fs_inodes_per_orphan_block(fs);
	__u32 crc;

	ino = ext2fs_cpu_to_le32(ino);
	gen = ext2fs_cpu_to_le32(gen);
	blk = ext2fs_cpu_to_le64(blk);
	crc = ext2fs_crc32c_le(fs->csum_seed, (अचिन्हित अक्षर *)&ino,
			       माप(ino));
	crc = ext2fs_crc32c_le(crc, (अचिन्हित अक्षर *)&gen, माप(gen));
	crc = ext2fs_crc32c_le(crc, (अचिन्हित अक्षर *)&blk, माप(blk));
	crc = ext2fs_crc32c_le(crc, (अचिन्हित अक्षर *)buf,
				inodes_per_ob * माप(__u32));

	वापस crc;
}

काष्ठा mkorphan_info {
	अक्षर *buf;
	अक्षर *zerobuf;
	blk_t अंक_blocks;
	blk_t alloc_blocks;
	blk64_t last_blk;
	errcode_t err;
	ext2_ino_t ino;
	__u32 generation;
};

अटल पूर्णांक mkorphan_proc(ext2_filsys	fs,
			 blk64_t	*blocknr,
			 e2_blkcnt_t	blockcnt,
			 blk64_t	ref_block EXT2FS_ATTR((unused)),
			 पूर्णांक		ref_offset EXT2FS_ATTR((unused)),
			 व्योम		*priv_तथ्य)
{
	काष्ठा mkorphan_info *oi = (काष्ठा mkorphan_info *)priv_तथ्य;
	blk64_t new_blk;
	errcode_t err;

	/* Can we just continue in currently allocated cluster? */
	अगर (blockcnt &&
	    EXT2FS_B2C(fs, oi->last_blk) == EXT2FS_B2C(fs, oi->last_blk + 1)) {
		new_blk = oi->last_blk + 1;
	} अन्यथा {
		err = ext2fs_new_block2(fs, oi->last_blk, 0, &new_blk);
		अगर (err) {
			oi->err = err;
			वापस BLOCK_ABORT;
		}
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
		oi->alloc_blocks++;
	}
	अगर (blockcnt >= 0) {
		अगर (ext2fs_has_feature_metadata_csum(fs->super)) {
			काष्ठा ext4_orphan_block_पुच्छ *पुच्छ;

			पुच्छ = ext2fs_orphan_block_पुच्छ(fs, oi->buf);
			पुच्छ->ob_checksum =
		ext2fs_cpu_to_le32(ext2fs_do_orphan_file_block_csum(fs,
				    oi->ino, oi->generation, new_blk, oi->buf));
		}
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->buf);
	} अन्यथा	/* zerobuf is used to initialize new indirect blocks... */
		err = io_channel_write_blk64(fs->io, new_blk, 1, oi->zerobuf);
	अगर (err) {
		oi->err = err;
		वापस BLOCK_ABORT;
	}
	oi->last_blk = new_blk;
	*blocknr = new_blk;
	अगर (blockcnt >= 0 && --oi->अंक_blocks == 0)
		वापस BLOCK_CHANGED | BLOCK_ABORT;
	वापस BLOCK_CHANGED;
}

errcode_t ext2fs_create_orphan_file(ext2_filsys fs, blk_t अंक_blocks)
{
	काष्ठा ext2_inode inode;
	ext2_ino_t ino = fs->super->s_orphan_file_inum;
	errcode_t err;
	अक्षर *buf = शून्य, *zerobuf = शून्य;
	काष्ठा mkorphan_info oi;
	काष्ठा ext4_orphan_block_पुच्छ *ob_पुच्छ;
	समय_प्रकार now;

	अगर (अंक_blocks > EXT4_MAX_ORPHAN_FILE_BLOCKS)
		अंक_blocks = EXT4_MAX_ORPHAN_FILE_BLOCKS;

	अगर (ino) {
		err = ext2fs_read_inode(fs, ino, &inode);
		अगर (err)
			वापस err;
		अगर (EXT2_I_आकार(&inode)) {
			err = ext2fs_truncate_orphan_file(fs);
			अगर (err)
				वापस err;
		}
	} अन्यथा {
		err = ext2fs_new_inode(fs, EXT2_मूल_INO, LINUX_S_IFREG | 0600,
				       0, &ino);
		अगर (err)
			वापस err;
		ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	}

	स_रखो(&inode, 0, माप(काष्ठा ext2_inode));
	अगर (ext2fs_has_feature_extents(fs->super)) {
		inode.i_flags |= EXT4_EXTENTS_FL;
		err = ext2fs_write_inode(fs, ino, &inode);
		अगर (err)
			वापस err;
	}

	err = ext2fs_get_mem(fs->blocksize, &buf);
	अगर (err)
		वापस err;
	err = ext2fs_get_mem(fs->blocksize, &zerobuf);
	अगर (err)
		जाओ out;
	स_रखो(buf, 0, fs->blocksize);
	स_रखो(zerobuf, 0, fs->blocksize);
	ob_पुच्छ = ext2fs_orphan_block_पुच्छ(fs, buf);
	ob_पुच्छ->ob_magic = ext2fs_cpu_to_le32(EXT4_ORPHAN_BLOCK_MAGIC);
	oi.अंक_blocks = अंक_blocks;
	oi.alloc_blocks = 0;
	oi.last_blk = 0;
	oi.generation = inode.i_generation;
	oi.ino = ino;
	oi.buf = buf;
	oi.zerobuf = zerobuf;
	oi.err = 0;
	err = ext2fs_block_iterate3(fs, ino, BLOCK_FLAG_APPEND,
				    0, mkorphan_proc, &oi);
	अगर (err)
		जाओ out;
	अगर (oi.err) {
		err = oi.err;
		जाओ out;
	}

	/* Reread inode after blocks were allocated */
	err = ext2fs_read_inode(fs, ino, &inode);
	अगर (err)
		जाओ out;
	ext2fs_iblk_set(fs, &inode, 0);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	inode.i_links_गिनती = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	ext2fs_iblk_add_blocks(fs, &inode, oi.alloc_blocks);
	err = ext2fs_inode_आकार_set(fs, &inode,
			(अचिन्हित दीर्घ दीर्घ)fs->blocksize * अंक_blocks);
	अगर (err)
		जाओ out;
	err = ext2fs_write_new_inode(fs, ino, &inode);
	अगर (err)
		जाओ out;

	fs->super->s_orphan_file_inum = ino;
	ext2fs_set_feature_orphan_file(fs->super);
	ext2fs_mark_super_dirty(fs);
	/* Need to update group descriptors as well */
	fs->flags &= ~EXT2_FLAG_SUPER_ONLY;
out:
	अगर (buf)
		ext2fs_free_mem(&buf);
	अगर (zerobuf)
		ext2fs_free_mem(&zerobuf);
	वापस err;
}

/*
 * Find reasonable size for orphan file. We choose orphan file size to be
 * between 32 and 512 filesystem blocks and not more than 1/4096 of the
 * filesystem unless it is really small.
 */
e2_blkcnt_t ext2fs_default_orphan_file_blocks(ext2_filsys fs)
{
	__u64 अंक_blocks = ext2fs_blocks_गिनती(fs->super);
	e2_blkcnt_t blks = 512;

	अगर (अंक_blocks < 128 * 1024)
		blks = 32;
	अन्यथा अगर (अंक_blocks < 2 * 1024 * 1024)
		blks = अंक_blocks / 4096;
	वापस (blks + EXT2FS_CLUSTER_MASK(fs)) & ~EXT2FS_CLUSTER_MASK(fs);
}

अटल errcode_t ext2fs_orphan_file_block_csum(ext2_filsys fs, ext2_ino_t ino,
					       blk64_t blk, अक्षर *buf,
					       __u32 *crcp)
{
	काष्ठा ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	अगर (retval)
		वापस retval;
	*crcp = ext2fs_do_orphan_file_block_csum(fs, ino, inode.i_generation,
						 blk, buf);
	वापस 0;
}

errcode_t ext2fs_orphan_file_block_csum_set(ext2_filsys fs, ext2_ino_t ino,
					    blk64_t blk, अक्षर *buf)
{
	काष्ठा ext4_orphan_block_पुच्छ *पुच्छ;
	errcode_t ret;
	__u32 crc = 0;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 0;

	पुच्छ = ext2fs_orphan_block_पुच्छ(fs, buf);
	ret = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	अगर (ret)
		वापस 0;
	पुच्छ->ob_checksum = ext2fs_cpu_to_le32(crc);
	वापस ret;
}

पूर्णांक ext2fs_orphan_file_block_csum_verify(ext2_filsys fs, ext2_ino_t ino,
					 blk64_t blk, अक्षर *buf)
{
	काष्ठा ext4_orphan_block_पुच्छ *पुच्छ;
	__u32 crc;
	errcode_t retval;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super))
		वापस 1;
	retval = ext2fs_orphan_file_block_csum(fs, ino, blk, buf, &crc);
	अगर (retval)
		वापस 0;
	पुच्छ = ext2fs_orphan_block_पुच्छ(fs, buf);
	वापस ext2fs_le32_to_cpu(पुच्छ->ob_checksum) == crc;
}
