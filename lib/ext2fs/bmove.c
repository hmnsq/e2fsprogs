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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#എങ്കിൽ HAVE_SYS_TIME_H
#അന്തര്ഭാവ <sys/time.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

ഘടന process_block_struct {
	ext2_ino_t		ino;
	ഘടന ext2_inode *	inode;
	ext2fs_block_bitmap	reserve;
	ext2fs_block_bitmap	alloc_map;
	errcode_t		error;
	അക്ഷരം			*buf;
	പൂർണ്ണസംഖ്യ			add_dir;
	പൂർണ്ണസംഖ്യ			flags;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ process_block(ext2_filsys fs, blk64_t *block_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_block,
			 പൂർണ്ണസംഖ്യ ref_offset, ശൂന്യം *priv_data)
{
	ഘടന process_block_struct *pb;
	errcode_t	retval;
	പൂർണ്ണസംഖ്യ		ret;
	blk64_t		block, orig;

	pb = (ഘടന process_block_struct *) priv_data;
	block = orig = *block_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	എങ്കിൽ (ext2fs_test_block_bitmap2(pb->reserve, block)) {
		ചെയ്യുക {
			എങ്കിൽ (++block >= ext2fs_blocks_എണ്ണം(fs->super))
				block = fs->super->s_first_data_block;
			എങ്കിൽ (block == orig) {
				pb->error = EXT2_ET_BLOCK_ALLOC_FAIL;
				തിരികെ BLOCK_ABORT;
			}
		} ആകുമ്പോൾ (ext2fs_test_block_bitmap2(pb->reserve, block) ||
			 ext2fs_test_block_bitmap2(pb->alloc_map, block));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		എങ്കിൽ (retval) {
			pb->error = retval;
			തിരികെ BLOCK_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, block, 1, pb->buf);
		എങ്കിൽ (retval) {
			pb->error = retval;
			തിരികെ BLOCK_ABORT;
		}
		*block_nr = block;
		ext2fs_mark_block_bitmap2(pb->alloc_map, block);
		ret = BLOCK_CHANGED;
		എങ്കിൽ (pb->flags & EXT2_BMOVE_DEBUG)
			printf("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (unsigned) pb->ino, blockcnt, 
			       (unsigned long long) orig,
			       (unsigned long long) block);
	}
	എങ്കിൽ (pb->add_dir) {
		retval = ext2fs_add_dir_block2(fs->dblist, pb->ino,
					       block, blockcnt);
		എങ്കിൽ (retval) {
			pb->error = retval;
			ret |= BLOCK_ABORT;
		}
	}
	തിരികെ ret;
}

errcode_t ext2fs_move_blocks(ext2_filsys fs,
			     ext2fs_block_bitmap reserve,
			     ext2fs_block_bitmap alloc_map,
			     പൂർണ്ണസംഖ്യ flags)
{
	ext2_ino_t	ino;
	ഘടന ext2_inode inode;
	errcode_t	retval;
	ഘടന process_block_struct pb;
	ext2_inode_scan	scan;
	അക്ഷരം		*block_buf;

	retval = ext2fs_open_inode_scan(fs, 0, &scan);
	എങ്കിൽ (retval)
		തിരികെ retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->block_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &block_buf);
	എങ്കിൽ (retval)
		തിരികെ retval;
	pb.buf = block_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	എങ്കിൽ (flags & EXT2_BMOVE_GET_DBLIST) {
		എങ്കിൽ (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = ശൂന്യ;
		}
		retval = ext2fs_init_dblist(fs, 0);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}

	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	ആകുമ്പോൾ (ino) {
		എങ്കിൽ ((inode.i_links_എണ്ണം == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			പോകുക next;

		pb.ino = ino;
		pb.inode = &inode;

		pb.add_dir = (LINUX_S_ISDIR(inode.i_mode) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_block_iterate3(fs, ino, 0, block_buf,
					       process_block, &pb);
		എങ്കിൽ (retval)
			തിരികെ retval;
		എങ്കിൽ (pb.error)
			തിരികെ pb.error;

	next:
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		എങ്കിൽ (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE)
			പോകുക next;
	}
	തിരികെ 0;
}

