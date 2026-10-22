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

#include "config.h"
#include <stdio.h>
#include <string.h>
#གལ་སྲིད HAVE_UNISTD_H
#include <unistd.h>
#endif
#གལ་སྲིད HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#གལ་སྲིད HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

sa_nracanaa process_block_struct {
	ext2_ino_t		ino;
	sa_nracanaa ext2_inode *	inode;
	ext2fs_block_bitmap	reserve;
	ext2fs_block_bitmap	alloc_map;
	errcode_t		error;
	var_na			*buf;
	ཧྲིལ			add_dir;
	ཧྲིལ			flags;
};

static ཧྲིལ process_block(ext2_filsys fs, blk64_t *block_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_block,
			 ཧྲིལ ref_offset, void *priv_data)
{
	sa_nracanaa process_block_struct *pb;
	errcode_t	retval;
	ཧྲིལ		ret;
	blk64_t		block, orig;

	pb = (sa_nracanaa process_block_struct *) priv_data;
	block = orig = *block_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	གལ་སྲིད (ext2fs_test_block_bitmap2(pb->reserve, block)) {
		do {
			གལ་སྲིད (++block >= ext2fs_blocks_count(fs->super))
				block = fs->super->s_first_data_block;
			གལ་སྲིད (block == orig) {
				pb->error = EXT2_ET_BLOCK_ALLOC_FAIL;
				ལོག BLOCK_ABORT;
			}
		} སྐབས (ext2fs_test_block_bitmap2(pb->reserve, block) ||
			 ext2fs_test_block_bitmap2(pb->alloc_map, block));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		གལ་སྲིད (retval) {
			pb->error = retval;
			ལོག BLOCK_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, block, 1, pb->buf);
		གལ་སྲིད (retval) {
			pb->error = retval;
			ལོག BLOCK_ABORT;
		}
		*block_nr = block;
		ext2fs_mark_block_bitmap2(pb->alloc_map, block);
		ret = BLOCK_CHANGED;
		གལ་སྲིད (pb->flags & EXT2_BMOVE_DEBUG)
			printf("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (unsigned) pb->ino, blockcnt, 
			       (unsigned long long) orig,
			       (unsigned long long) block);
	}
	གལ་སྲིད (pb->add_dir) {
		retval = ext2fs_add_dir_block2(fs->dblist, pb->ino,
					       block, blockcnt);
		གལ་སྲིད (retval) {
			pb->error = retval;
			ret |= BLOCK_ABORT;
		}
	}
	ལོག ret;
}

errcode_t ext2fs_move_blocks(ext2_filsys fs,
			     ext2fs_block_bitmap reserve,
			     ext2fs_block_bitmap alloc_map,
			     ཧྲིལ flags)
{
	ext2_ino_t	ino;
	sa_nracanaa ext2_inode inode;
	errcode_t	retval;
	sa_nracanaa process_block_struct pb;
	ext2_inode_scan	scan;
	var_na		*block_buf;

	retval = ext2fs_open_inode_scan(fs, 0, &scan);
	གལ་སྲིད (retval)
		ལོག retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->block_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &block_buf);
	གལ་སྲིད (retval)
		ལོག retval;
	pb.buf = block_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	གལ་སྲིད (flags & EXT2_BMOVE_GET_DBLIST) {
		གལ་སྲིད (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = NULL;
		}
		retval = ext2fs_init_dblist(fs, 0);
		གལ་སྲིད (retval)
			ལོག retval;
	}

	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	གལ་སྲིད (retval)
		ལོག retval;

	སྐབས (ino) {
		གལ་སྲིད ((inode.i_links_count == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			goto next;

		pb.ino = ino;
		pb.inode = &inode;

		pb.add_dir = (LINUX_S_ISDIR(inode.i_mode) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_block_iterate3(fs, ino, 0, block_buf,
					       process_block, &pb);
		གལ་སྲིད (retval)
			ལོག retval;
		གལ་སྲིད (pb.error)
			ལོག pb.error;

	next:
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		གལ་སྲིད (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE)
			goto next;
	}
	ལོག 0;
}

