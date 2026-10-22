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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif
#যদি HAVE_SYS_TIME_H
#অন্তর্ভাৱ <sys/time.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

সংরচনা process_block_struct {
	ext2_ino_t		ino;
	সংরচনা ext2_inode *	inode;
	ext2fs_block_bitmap	reserve;
	ext2fs_block_bitmap	alloc_map;
	errcode_t		error;
	ৱর্ণ			*buf;
	পূর্ণাঙ্ক			add_dir;
	পূর্ণাঙ্ক			flags;
};

স্থির পূর্ণাঙ্ক process_block(ext2_filsys fs, blk64_t *block_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_block,
			 পূর্ণাঙ্ক ref_offset, শূন্য *priv_data)
{
	সংরচনা process_block_struct *pb;
	errcode_t	retval;
	পূর্ণাঙ্ক		ret;
	blk64_t		block, orig;

	pb = (সংরচনা process_block_struct *) priv_data;
	block = orig = *block_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	যদি (ext2fs_test_block_bitmap2(pb->reserve, block)) {
		কৰা {
			যদি (++block >= ext2fs_blocks_গুনতি(fs->super))
				block = fs->super->s_first_data_block;
			যদি (block == orig) {
				pb->error = EXT2_ET_BLOCK_ALLOC_FAIL;
				ঘূৰাই BLOCK_ABORT;
			}
		} যেতিয়ালৈ (ext2fs_test_block_bitmap2(pb->reserve, block) ||
			 ext2fs_test_block_bitmap2(pb->alloc_map, block));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		যদি (retval) {
			pb->error = retval;
			ঘূৰাই BLOCK_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, block, 1, pb->buf);
		যদি (retval) {
			pb->error = retval;
			ঘূৰাই BLOCK_ABORT;
		}
		*block_nr = block;
		ext2fs_mark_block_bitmap2(pb->alloc_map, block);
		ret = BLOCK_CHANGED;
		যদি (pb->flags & EXT2_BMOVE_DEBUG)
			printf("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (unsigned) pb->ino, blockcnt, 
			       (unsigned long long) orig,
			       (unsigned long long) block);
	}
	যদি (pb->add_dir) {
		retval = ext2fs_add_dir_block2(fs->dblist, pb->ino,
					       block, blockcnt);
		যদি (retval) {
			pb->error = retval;
			ret |= BLOCK_ABORT;
		}
	}
	ঘূৰাই ret;
}

errcode_t ext2fs_move_blocks(ext2_filsys fs,
			     ext2fs_block_bitmap reserve,
			     ext2fs_block_bitmap alloc_map,
			     পূর্ণাঙ্ক flags)
{
	ext2_ino_t	ino;
	সংরচনা ext2_inode inode;
	errcode_t	retval;
	সংরচনা process_block_struct pb;
	ext2_inode_scan	scan;
	ৱর্ণ		*block_buf;

	retval = ext2fs_open_inode_scan(fs, 0, &scan);
	যদি (retval)
		ঘূৰাই retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->block_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &block_buf);
	যদি (retval)
		ঘূৰাই retval;
	pb.buf = block_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	যদি (flags & EXT2_BMOVE_GET_DBLIST) {
		যদি (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = রিক্ত;
		}
		retval = ext2fs_init_dblist(fs, 0);
		যদি (retval)
			ঘূৰাই retval;
	}

	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	যদি (retval)
		ঘূৰাই retval;

	যেতিয়ালৈ (ino) {
		যদি ((inode.i_links_গুনতি == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			গচ্ছ next;

		pb.ino = ino;
		pb.inode = &inode;

		pb.add_dir = (LINUX_S_ISDIR(inode.i_mode) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_block_iterate3(fs, ino, 0, block_buf,
					       process_block, &pb);
		যদি (retval)
			ঘূৰাই retval;
		যদি (pb.error)
			ঘূৰাই pb.error;

	next:
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		যদি (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE)
			গচ্ছ next;
	}
	ঘূৰাই 0;
}

