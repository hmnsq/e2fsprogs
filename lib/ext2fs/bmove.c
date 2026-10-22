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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#જો HAVE_SYS_TIME_H
#અન્તર્ભાવ <sys/time.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"

સંરચના process_block_struct {
	ext2_ino_t		ino;
	સંરચના ext2_inode *	inode;
	ext2fs_block_bitmap	reserve;
	ext2fs_block_bitmap	alloc_map;
	errcode_t		error;
	અક્ષર			*buf;
	પૂર્ણાંક			add_dir;
	પૂર્ણાંક			flags;
};

સ્થિર પૂર્ણાંક process_block(ext2_filsys fs, blk64_t *block_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_block,
			 પૂર્ણાંક ref_offset, શૂન્ય *priv_data)
{
	સંરચના process_block_struct *pb;
	errcode_t	retval;
	પૂર્ણાંક		ret;
	blk64_t		block, orig;

	pb = (સંરચના process_block_struct *) priv_data;
	block = orig = *block_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	જો (ext2fs_test_block_bitmap2(pb->reserve, block)) {
		કરો {
			જો (++block >= ext2fs_blocks_ગણતરી(fs->super))
				block = fs->super->s_first_data_block;
			જો (block == orig) {
				pb->error = EXT2_ET_BLOCK_ALLOC_FAIL;
				પાછા BLOCK_ABORT;
			}
		} જ્યારે (ext2fs_test_block_bitmap2(pb->reserve, block) ||
			 ext2fs_test_block_bitmap2(pb->alloc_map, block));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		જો (retval) {
			pb->error = retval;
			પાછા BLOCK_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, block, 1, pb->buf);
		જો (retval) {
			pb->error = retval;
			પાછા BLOCK_ABORT;
		}
		*block_nr = block;
		ext2fs_mark_block_bitmap2(pb->alloc_map, block);
		ret = BLOCK_CHANGED;
		જો (pb->flags & EXT2_BMOVE_DEBUG)
			printf("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (unsigned) pb->ino, blockcnt, 
			       (unsigned long long) orig,
			       (unsigned long long) block);
	}
	જો (pb->add_dir) {
		retval = ext2fs_add_dir_block2(fs->dblist, pb->ino,
					       block, blockcnt);
		જો (retval) {
			pb->error = retval;
			ret |= BLOCK_ABORT;
		}
	}
	પાછા ret;
}

errcode_t ext2fs_move_blocks(ext2_filsys fs,
			     ext2fs_block_bitmap reserve,
			     ext2fs_block_bitmap alloc_map,
			     પૂર્ણાંક flags)
{
	ext2_ino_t	ino;
	સંરચના ext2_inode inode;
	errcode_t	retval;
	સંરચના process_block_struct pb;
	ext2_inode_scan	scan;
	અક્ષર		*block_buf;

	retval = ext2fs_open_inode_scan(fs, 0, &scan);
	જો (retval)
		પાછા retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->block_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &block_buf);
	જો (retval)
		પાછા retval;
	pb.buf = block_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	જો (flags & EXT2_BMOVE_GET_DBLIST) {
		જો (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = ખાલી;
		}
		retval = ext2fs_init_dblist(fs, 0);
		જો (retval)
			પાછા retval;
	}

	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	જો (retval)
		પાછા retval;

	જ્યારે (ino) {
		જો ((inode.i_links_ગણતરી == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			જાઓ next;

		pb.ino = ino;
		pb.inode = &inode;

		pb.add_dir = (LINUX_S_ISDIR(inode.i_mode) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_block_iterate3(fs, ino, 0, block_buf,
					       process_block, &pb);
		જો (retval)
			પાછા retval;
		જો (pb.error)
			પાછા pb.error;

	next:
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		જો (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE)
			જાઓ next;
	}
	પાછા 0;
}

