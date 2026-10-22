/*
 * bmove.c --- Move blocks around to make way for a particular
 * 	filesystem structure.
 *
 * Copyright (C) 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#यदि HAVE_SYS_TIME_H
#अन्तर्भाव <sys/time.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना process_block_struct {
	ext2_ino_t		ino;
	संरचना ext2_inode *	inode;
	ext2fs_block_bitmap	reserve;
	ext2fs_block_bitmap	alloc_map;
	errcode_t		error;
	वर्ण			*buf;
	पूर्णाङ्क			add_dir;
	पूर्णाङ्क			flags;
};

स्थिर पूर्णाङ्क process_block(ext2_filsys fs, blk64_t *block_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_block,
			 पूर्णाङ्क ref_offset, शून्य *priv_data)
{
	संरचना process_block_struct *pb;
	errcode_t	retval;
	पूर्णाङ्क		ret;
	blk64_t		block, orig;

	pb = (संरचना process_block_struct *) priv_data;
	block = orig = *block_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	यदि (ext2fs_test_block_bitmap2(pb->reserve, block)) {
		कुरु {
			यदि (++block >= ext2fs_blocks_मोजणी(fs->super))
				block = fs->super->s_first_data_block;
			यदि (block == orig) {
				pb->error = EXT2_ET_BLOCK_ALLOC_FAIL;
				प्रत्यागम BLOCK_ABORT;
			}
		} यावत् (ext2fs_test_block_bitmap2(pb->reserve, block) ||
			 ext2fs_test_block_bitmap2(pb->alloc_map, block));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		यदि (retval) {
			pb->error = retval;
			प्रत्यागम BLOCK_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, block, 1, pb->buf);
		यदि (retval) {
			pb->error = retval;
			प्रत्यागम BLOCK_ABORT;
		}
		*block_nr = block;
		ext2fs_mark_block_bitmap2(pb->alloc_map, block);
		ret = BLOCK_CHANGED;
		यदि (pb->flags & EXT2_BMOVE_DEBUG)
			printf("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (unsigned) pb->ino, blockcnt, 
			       (unsigned long long) orig,
			       (unsigned long long) block);
	}
	यदि (pb->add_dir) {
		retval = ext2fs_add_dir_block2(fs->dblist, pb->ino,
					       block, blockcnt);
		यदि (retval) {
			pb->error = retval;
			ret |= BLOCK_ABORT;
		}
	}
	प्रत्यागम ret;
}

errcode_t ext2fs_move_blocks(ext2_filsys fs,
			     ext2fs_block_bitmap reserve,
			     ext2fs_block_bitmap alloc_map,
			     पूर्णाङ्क flags)
{
	ext2_ino_t	ino;
	संरचना ext2_inode inode;
	errcode_t	retval;
	संरचना process_block_struct pb;
	ext2_inode_scan	scan;
	वर्ण		*block_buf;

	retval = ext2fs_open_inode_scan(fs, 0, &scan);
	यदि (retval)
		प्रत्यागम retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->block_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &block_buf);
	यदि (retval)
		प्रत्यागम retval;
	pb.buf = block_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	यदि (flags & EXT2_BMOVE_GET_DBLIST) {
		यदि (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = रिक्त;
		}
		retval = ext2fs_init_dblist(fs, 0);
		यदि (retval)
			प्रत्यागम retval;
	}

	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	यदि (retval)
		प्रत्यागम retval;

	यावत् (ino) {
		यदि ((inode.i_links_मोजणी == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			गच्छ next;

		pb.ino = ino;
		pb.inode = &inode;

		pb.add_dir = (LINUX_S_ISDIR(inode.i_mode) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_block_iterate3(fs, ino, 0, block_buf,
					       process_block, &pb);
		यदि (retval)
			प्रत्यागम retval;
		यदि (pb.error)
			प्रत्यागम pb.error;

	next:
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		यदि (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE)
			गच्छ next;
	}
	प्रत्यागम 0;
}

