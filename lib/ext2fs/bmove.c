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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_సమయం_H
#అన్తర్భావ <sys/time.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

నిర్మాణం process_ఖండం_struct {
	ext2_ino_t		ino;
	నిర్మాణం ext2_inode *	inode;
	ext2fs_ఖండం_bitmap	reserve;
	ext2fs_ఖండం_bitmap	alloc_map;
	errcode_t		error;
	అక్షరం			*buf;
	పూర్ణాంకం			కూడు_dir;
	పూర్ణాంకం			flags;
};

స్థిర పూర్ణాంకం process_ఖండం(ext2_filsys fs, blk64_t *ఖండం_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_ఖండం,
			 పూర్ణాంకం ref_స్థానభ్రంశం, శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం process_ఖండం_struct *pb;
	errcode_t	retval;
	పూర్ణాంకం		ret;
	blk64_t		ఖండం, orig;

	pb = (నిర్మాణం process_ఖండం_struct *) priv_దత్తాంశం;
	ఖండం = orig = *ఖండం_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	ఒకవేళ (ext2fs_test_ఖండం_bitmap2(pb->reserve, ఖండం)) {
		చేయి {
			ఒకవేళ (++ఖండం >= ext2fs_ఖండాలు_లెక్క(fs->super))
				ఖండం = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
			ఒకవేళ (ఖండం == orig) {
				pb->error = EXT2_ET_ఖండం_ALLOC_FAIL;
				తిరిగి ఖండం_ABORT;
			}
		} అయితే (ext2fs_test_ఖండం_bitmap2(pb->reserve, ఖండం) ||
			 ext2fs_test_ఖండం_bitmap2(pb->alloc_map, ఖండం));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		ఒకవేళ (retval) {
			pb->error = retval;
			తిరిగి ఖండం_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, ఖండం, 1, pb->buf);
		ఒకవేళ (retval) {
			pb->error = retval;
			తిరిగి ఖండం_ABORT;
		}
		*ఖండం_nr = ఖండం;
		ext2fs_mark_ఖండం_bitmap2(pb->alloc_map, ఖండం);
		ret = ఖండం_మారింది;
		ఒకవేళ (pb->flags & EXT2_BMOVE_DEBUG)
			రాయి("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (గుర్తులేని) pb->ino, blockcnt, 
			       (గుర్తులేని దీర్ఘ దీర్ఘ) orig,
			       (గుర్తులేని దీర్ఘ దీర్ఘ) ఖండం);
	}
	ఒకవేళ (pb->కూడు_dir) {
		retval = ext2fs_కూడు_dir_block2(fs->dblist, pb->ino,
					       ఖండం, blockcnt);
		ఒకవేళ (retval) {
			pb->error = retval;
			ret |= ఖండం_ABORT;
		}
	}
	తిరిగి ret;
}

errcode_t ext2fs_move_ఖండాలు(ext2_filsys fs,
			     ext2fs_ఖండం_bitmap reserve,
			     ext2fs_ఖండం_bitmap alloc_map,
			     పూర్ణాంకం flags)
{
	ext2_ino_t	ino;
	నిర్మాణం ext2_inode inode;
	errcode_t	retval;
	నిర్మాణం process_ఖండం_struct pb;
	ext2_inode_పరిశీలన	పరిశీలన;
	అక్షరం		*ఖండం_buf;

	retval = ext2fs_open_inode_పరిశీలన(fs, 0, &పరిశీలన);
	ఒకవేళ (retval)
		తిరిగి retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->ఖండం_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &ఖండం_buf);
	ఒకవేళ (retval)
		తిరిగి retval;
	pb.buf = ఖండం_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	ఒకవేళ (flags & EXT2_BMOVE_GET_DBLIST) {
		ఒకవేళ (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = ఖాళీ;
		}
		retval = ext2fs_init_dblist(fs, 0);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	retval = ext2fs_get_తదుపరి_inode(పరిశీలన, &ino, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	అయితే (ino) {
		ఒకవేళ ((inode.i_links_లెక్క == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			వెళ్ళు తదుపరి;

		pb.ino = ino;
		pb.inode = &inode;

		pb.కూడు_dir = (LINUX_S_ISDIR(inode.i_బహుళకం) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_ఖండం_iterate3(fs, ino, 0, ఖండం_buf,
					       process_ఖండం, &pb);
		ఒకవేళ (retval)
			తిరిగి retval;
		ఒకవేళ (pb.error)
			తిరిగి pb.error;

	తదుపరి:
		retval = ext2fs_get_తదుపరి_inode(పరిశీలన, &ino, &inode);
		ఒకవేళ (retval == EXT2_ET_BAD_ఖండం_లోపల_INODE_పట్టిక)
			వెళ్ళు తదుపరి;
	}
	తిరిగి 0;
}

