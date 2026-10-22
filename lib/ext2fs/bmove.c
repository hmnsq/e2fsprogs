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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#எனில் HAVE_SYS_TIME_H
#அந்தர்பாவ <sys/time.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

கட்டமைப்பு process_block_struct {
	ext2_ino_t		ino;
	கட்டமைப்பு ext2_inode *	inode;
	ext2fs_block_bitmap	reserve;
	ext2fs_block_bitmap	alloc_map;
	errcode_t		error;
	எழுத்து			*buf;
	முழு			add_dir;
	முழு			flags;
};

நிலையான முழு process_block(ext2_filsys fs, blk64_t *block_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_block,
			 முழு ref_offset, வெறுமை *priv_data)
{
	கட்டமைப்பு process_block_struct *pb;
	errcode_t	retval;
	முழு		ret;
	blk64_t		block, orig;

	pb = (கட்டமைப்பு process_block_struct *) priv_data;
	block = orig = *block_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	எனில் (ext2fs_test_block_bitmap2(pb->reserve, block)) {
		செய் {
			எனில் (++block >= ext2fs_blocks_மொத்தம்(fs->super))
				block = fs->super->s_first_data_block;
			எனில் (block == orig) {
				pb->error = EXT2_ET_BLOCK_ALLOC_FAIL;
				திரும்பு BLOCK_ABORT;
			}
		} போது (ext2fs_test_block_bitmap2(pb->reserve, block) ||
			 ext2fs_test_block_bitmap2(pb->alloc_map, block));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		எனில் (retval) {
			pb->error = retval;
			திரும்பு BLOCK_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, block, 1, pb->buf);
		எனில் (retval) {
			pb->error = retval;
			திரும்பு BLOCK_ABORT;
		}
		*block_nr = block;
		ext2fs_mark_block_bitmap2(pb->alloc_map, block);
		ret = BLOCK_CHANGED;
		எனில் (pb->flags & EXT2_BMOVE_DEBUG)
			printf("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (unsigned) pb->ino, blockcnt, 
			       (unsigned long long) orig,
			       (unsigned long long) block);
	}
	எனில் (pb->add_dir) {
		retval = ext2fs_add_dir_block2(fs->dblist, pb->ino,
					       block, blockcnt);
		எனில் (retval) {
			pb->error = retval;
			ret |= BLOCK_ABORT;
		}
	}
	திரும்பு ret;
}

errcode_t ext2fs_move_blocks(ext2_filsys fs,
			     ext2fs_block_bitmap reserve,
			     ext2fs_block_bitmap alloc_map,
			     முழு flags)
{
	ext2_ino_t	ino;
	கட்டமைப்பு ext2_inode inode;
	errcode_t	retval;
	கட்டமைப்பு process_block_struct pb;
	ext2_inode_scan	scan;
	எழுத்து		*block_buf;

	retval = ext2fs_open_inode_scan(fs, 0, &scan);
	எனில் (retval)
		திரும்பு retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->block_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &block_buf);
	எனில் (retval)
		திரும்பு retval;
	pb.buf = block_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	எனில் (flags & EXT2_BMOVE_GET_DBLIST) {
		எனில் (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = வெற்று;
		}
		retval = ext2fs_init_dblist(fs, 0);
		எனில் (retval)
			திரும்பு retval;
	}

	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	எனில் (retval)
		திரும்பு retval;

	போது (ino) {
		எனில் ((inode.i_links_மொத்தம் == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			செல் next;

		pb.ino = ino;
		pb.inode = &inode;

		pb.add_dir = (LINUX_S_ISDIR(inode.i_mode) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_block_iterate3(fs, ino, 0, block_buf,
					       process_block, &pb);
		எனில் (retval)
			திரும்பு retval;
		எனில் (pb.error)
			திரும்பு pb.error;

	next:
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		எனில் (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE)
			செல் next;
	}
	திரும்பு 0;
}

