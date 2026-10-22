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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif
#ಯದಿ HAVE_SYS_TIME_H
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸಂರಚನಾ process_block_struct {
	ext2_ino_t		ino;
	ಸಂರಚನಾ ext2_inode *	inode;
	ext2fs_block_bitmap	reserve;
	ext2fs_block_bitmap	alloc_map;
	errcode_t		error;
	ವರ್ಣ			*buf;
	ಪೂರ್ಣಾಙ್ಕ			add_dir;
	ಪೂರ್ಣಾಙ್ಕ			flags;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ process_block(ext2_filsys fs, blk64_t *block_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_block,
			 ಪೂರ್ಣಾಙ್ಕ ref_offset, ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ process_block_struct *pb;
	errcode_t	retval;
	ಪೂರ್ಣಾಙ್ಕ		ret;
	blk64_t		block, orig;

	pb = (ಸಂರಚನಾ process_block_struct *) priv_data;
	block = orig = *block_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	ಯದಿ (ext2fs_test_block_bitmap2(pb->reserve, block)) {
		ಕುರು {
			ಯದಿ (++block >= ext2fs_blocks_ಎಣಿಕೆ(fs->super))
				block = fs->super->s_first_data_block;
			ಯದಿ (block == orig) {
				pb->error = EXT2_ET_BLOCK_ALLOC_FAIL;
				ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
			}
		} ಯಾವತ್ (ext2fs_test_block_bitmap2(pb->reserve, block) ||
			 ext2fs_test_block_bitmap2(pb->alloc_map, block));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		ಯದಿ (retval) {
			pb->error = retval;
			ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, block, 1, pb->buf);
		ಯದಿ (retval) {
			pb->error = retval;
			ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		}
		*block_nr = block;
		ext2fs_mark_block_bitmap2(pb->alloc_map, block);
		ret = BLOCK_CHANGED;
		ಯದಿ (pb->flags & EXT2_BMOVE_DEBUG)
			printf("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (unsigned) pb->ino, blockcnt, 
			       (unsigned long long) orig,
			       (unsigned long long) block);
	}
	ಯದಿ (pb->add_dir) {
		retval = ext2fs_add_dir_block2(fs->dblist, pb->ino,
					       block, blockcnt);
		ಯದಿ (retval) {
			pb->error = retval;
			ret |= BLOCK_ABORT;
		}
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}

errcode_t ext2fs_move_blocks(ext2_filsys fs,
			     ext2fs_block_bitmap reserve,
			     ext2fs_block_bitmap alloc_map,
			     ಪೂರ್ಣಾಙ್ಕ flags)
{
	ext2_ino_t	ino;
	ಸಂರಚನಾ ext2_inode inode;
	errcode_t	retval;
	ಸಂರಚನಾ process_block_struct pb;
	ext2_inode_scan	scan;
	ವರ್ಣ		*block_buf;

	retval = ext2fs_open_inode_scan(fs, 0, &scan);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->block_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &block_buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	pb.buf = block_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	ಯದಿ (flags & EXT2_BMOVE_GET_DBLIST) {
		ಯದಿ (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = ರಿಕ್ತ;
		}
		retval = ext2fs_init_dblist(fs, 0);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯಾವತ್ (ino) {
		ಯದಿ ((inode.i_links_ಎಣಿಕೆ == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			ಗಚ್ಛ next;

		pb.ino = ino;
		pb.inode = &inode;

		pb.add_dir = (LINUX_S_ISDIR(inode.i_mode) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_block_iterate3(fs, ino, 0, block_buf,
					       process_block, &pb);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		ಯದಿ (pb.error)
			ಪ್ರತ್ಯಾಗಮ pb.error;

	next:
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		ಯದಿ (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE)
			ಗಚ್ಛ next;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

