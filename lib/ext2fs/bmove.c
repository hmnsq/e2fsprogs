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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#إذا HAVE_SYS_TIME_H
#ضمن <sys/time.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية process_block_struct {
	ext2_ino_t		ino;
	بنية ext2_inode *	inode;
	ext2fs_block_bitmap	reserve;
	ext2fs_block_bitmap	alloc_map;
	errcode_t		error;
	حرف			*buf;
	صحيح			add_dir;
	صحيح			flags;
};

ساكن صحيح process_block(ext2_filsys fs, blk64_t *block_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_block,
			 صحيح ref_offset, فراغ *priv_data)
{
	بنية process_block_struct *pb;
	errcode_t	retval;
	صحيح		ret;
	blk64_t		block, orig;

	pb = (بنية process_block_struct *) priv_data;
	block = orig = *block_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	إذا (ext2fs_test_block_bitmap2(pb->reserve, block)) {
		افعل {
			إذا (++block >= ext2fs_blocks_تعداد(fs->super))
				block = fs->super->s_first_data_block;
			إذا (block == orig) {
				pb->error = EXT2_ET_BLOCK_ALLOC_FAIL;
				إرجاع BLOCK_ABORT;
			}
		} بينما (ext2fs_test_block_bitmap2(pb->reserve, block) ||
			 ext2fs_test_block_bitmap2(pb->alloc_map, block));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		إذا (retval) {
			pb->error = retval;
			إرجاع BLOCK_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, block, 1, pb->buf);
		إذا (retval) {
			pb->error = retval;
			إرجاع BLOCK_ABORT;
		}
		*block_nr = block;
		ext2fs_mark_block_bitmap2(pb->alloc_map, block);
		ret = BLOCK_CHANGED;
		إذا (pb->flags & EXT2_BMOVE_DEBUG)
			printf("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (unsigned) pb->ino, blockcnt, 
			       (unsigned long long) orig,
			       (unsigned long long) block);
	}
	إذا (pb->add_dir) {
		retval = ext2fs_add_dir_block2(fs->dblist, pb->ino,
					       block, blockcnt);
		إذا (retval) {
			pb->error = retval;
			ret |= BLOCK_ABORT;
		}
	}
	إرجاع ret;
}

errcode_t ext2fs_move_blocks(ext2_filsys fs,
			     ext2fs_block_bitmap reserve,
			     ext2fs_block_bitmap alloc_map,
			     صحيح flags)
{
	ext2_ino_t	ino;
	بنية ext2_inode inode;
	errcode_t	retval;
	بنية process_block_struct pb;
	ext2_inode_scan	scan;
	حرف		*block_buf;

	retval = ext2fs_open_inode_scan(fs, 0, &scan);
	إذا (retval)
		إرجاع retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->block_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &block_buf);
	إذا (retval)
		إرجاع retval;
	pb.buf = block_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	إذا (flags & EXT2_BMOVE_GET_DBLIST) {
		إذا (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = عدم;
		}
		retval = ext2fs_init_dblist(fs, 0);
		إذا (retval)
			إرجاع retval;
	}

	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	إذا (retval)
		إرجاع retval;

	بينما (ino) {
		إذا ((inode.i_links_تعداد == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			اذهب next;

		pb.ino = ino;
		pb.inode = &inode;

		pb.add_dir = (LINUX_S_ISDIR(inode.i_mode) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_block_iterate3(fs, ino, 0, block_buf,
					       process_block, &pb);
		إذا (retval)
			إرجاع retval;
		إذا (pb.error)
			إرجاع pb.error;

	next:
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		إذا (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE)
			اذهب next;
	}
	إرجاع 0;
}

