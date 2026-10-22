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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#ଯଦି HAVE_SYS_TIME_H
#ଅନ୍ତର୍ଭାଵ <sys/time.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସଂରଚନା process_block_struct {
	ext2_ino_t		ino;
	ସଂରଚନା ext2_inode *	inode;
	ext2fs_block_bitmap	reserve;
	ext2fs_block_bitmap	alloc_map;
	errcode_t		error;
	ଅକ୍ଷର			*buf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			add_dir;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			flags;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା process_block(ext2_filsys fs, blk64_t *block_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_block,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset, ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା process_block_struct *pb;
	errcode_t	retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ret;
	blk64_t		block, orig;

	pb = (ସଂରଚନା process_block_struct *) priv_data;
	block = orig = *block_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	ଯଦି (ext2fs_test_block_bitmap2(pb->reserve, block)) {
		କର {
			ଯଦି (++block >= ext2fs_blocks_ଗଣନା(fs->super))
				block = fs->super->s_first_data_block;
			ଯଦି (block == orig) {
				pb->error = EXT2_ET_BLOCK_ALLOC_FAIL;
				ଫେରନ୍ତୁ BLOCK_ABORT;
			}
		} ଯେତେବେଳେ (ext2fs_test_block_bitmap2(pb->reserve, block) ||
			 ext2fs_test_block_bitmap2(pb->alloc_map, block));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		ଯଦି (retval) {
			pb->error = retval;
			ଫେରନ୍ତୁ BLOCK_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, block, 1, pb->buf);
		ଯଦି (retval) {
			pb->error = retval;
			ଫେରନ୍ତୁ BLOCK_ABORT;
		}
		*block_nr = block;
		ext2fs_mark_block_bitmap2(pb->alloc_map, block);
		ret = BLOCK_CHANGED;
		ଯଦି (pb->flags & EXT2_BMOVE_DEBUG)
			printf("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (unsigned) pb->ino, blockcnt, 
			       (unsigned long long) orig,
			       (unsigned long long) block);
	}
	ଯଦି (pb->add_dir) {
		retval = ext2fs_add_dir_block2(fs->dblist, pb->ino,
					       block, blockcnt);
		ଯଦି (retval) {
			pb->error = retval;
			ret |= BLOCK_ABORT;
		}
	}
	ଫେରନ୍ତୁ ret;
}

errcode_t ext2fs_move_blocks(ext2_filsys fs,
			     ext2fs_block_bitmap reserve,
			     ext2fs_block_bitmap alloc_map,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ext2_ino_t	ino;
	ସଂରଚନା ext2_inode inode;
	errcode_t	retval;
	ସଂରଚନା process_block_struct pb;
	ext2_inode_scan	scan;
	ଅକ୍ଷର		*block_buf;

	retval = ext2fs_open_inode_scan(fs, 0, &scan);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->block_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &block_buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	pb.buf = block_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	ଯଦି (flags & EXT2_BMOVE_GET_DBLIST) {
		ଯଦି (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = ଖାଲି;
		}
		retval = ext2fs_init_dblist(fs, 0);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}

	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯେତେବେଳେ (ino) {
		ଯଦି ((inode.i_links_ଗଣନା == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			ଯାଅ next;

		pb.ino = ino;
		pb.inode = &inode;

		pb.add_dir = (LINUX_S_ISDIR(inode.i_mode) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_block_iterate3(fs, ino, 0, block_buf,
					       process_block, &pb);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		ଯଦି (pb.error)
			ଫେରନ୍ତୁ pb.error;

	next:
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		ଯଦି (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE)
			ଯାଅ next;
	}
	ଫେରନ୍ତୁ 0;
}

