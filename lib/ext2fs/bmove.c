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
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#यदि HAVE_SYS_TIME_H
#अन्तर्भाव <sys/time.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना process_खण्डम्_struct {
	ext2_ino_t		ino;
	संरचना ext2_inode *	inode;
	ext2fs_खण्डम्_bitmap	reserve;
	ext2fs_खण्डम्_bitmap	alloc_map;
	errcode_t		error;
	वर्ण			*buf;
	पूर्णाङ्क			सङ्कलय_dir;
	पूर्णाङ्क			flags;
};

स्थिर पूर्णाङ्क process_खण्डम्(ext2_filsys fs, blk64_t *खण्डम्_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_खण्डम्,
			 पूर्णाङ्क ref_विस्थापनम्, शून्य *priv_दत्त)
{
	संरचना process_खण्डम्_struct *pb;
	errcode_t	retval;
	पूर्णाङ्क		ret;
	blk64_t		खण्डम्, orig;

	pb = (संरचना process_खण्डम्_struct *) priv_दत्त;
	खण्डम् = orig = *खण्डम्_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	यदि (ext2fs_test_खण्डम्_bitmap2(pb->reserve, खण्डम्)) {
		कुरु {
			यदि (++खण्डम् >= ext2fs_खण्डानि_गणनम्(fs->super))
				खण्डम् = fs->super->s_first_दत्त_खण्डम्;
			यदि (खण्डम् == orig) {
				pb->error = EXT2_ET_खण्डम्_ALLOC_FAIL;
				प्रत्यागम खण्डम्_ABORT;
			}
		} यावत् (ext2fs_test_खण्डम्_bitmap2(pb->reserve, खण्डम्) ||
			 ext2fs_test_खण्डम्_bitmap2(pb->alloc_map, खण्डम्));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		यदि (retval) {
			pb->error = retval;
			प्रत्यागम खण्डम्_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, खण्डम्, 1, pb->buf);
		यदि (retval) {
			pb->error = retval;
			प्रत्यागम खण्डम्_ABORT;
		}
		*खण्डम्_nr = खण्डम्;
		ext2fs_mark_खण्डम्_bitmap2(pb->alloc_map, खण्डम्);
		ret = खण्डम्_परिवर्तितम्;
		यदि (pb->flags & EXT2_BMOVE_DEBUG)
			प्रारूप_लिख("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (अचिह्नित) pb->ino, blockcnt, 
			       (अचिह्नित दीर्घ दीर्घ) orig,
			       (अचिह्नित दीर्घ दीर्घ) खण्डम्);
	}
	यदि (pb->सङ्कलय_dir) {
		retval = ext2fs_सङ्कलय_dir_block2(fs->dblist, pb->ino,
					       खण्डम्, blockcnt);
		यदि (retval) {
			pb->error = retval;
			ret |= खण्डम्_ABORT;
		}
	}
	प्रत्यागम ret;
}

errcode_t ext2fs_move_खण्डानि(ext2_filsys fs,
			     ext2fs_खण्डम्_bitmap reserve,
			     ext2fs_खण्डम्_bitmap alloc_map,
			     पूर्णाङ्क flags)
{
	ext2_ino_t	ino;
	संरचना ext2_inode inode;
	errcode_t	retval;
	संरचना process_खण्डम्_struct pb;
	ext2_inode_अवलोकनम्	अवलोकनम्;
	वर्ण		*खण्डम्_buf;

	retval = ext2fs_open_inode_अवलोकनम्(fs, 0, &अवलोकनम्);
	यदि (retval)
		प्रत्यागम retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->खण्डम्_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &खण्डम्_buf);
	यदि (retval)
		प्रत्यागम retval;
	pb.buf = खण्डम्_buf + fs->blocksize * 3;

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

	retval = ext2fs_get_अग्र_inode(अवलोकनम्, &ino, &inode);
	यदि (retval)
		प्रत्यागम retval;

	यावत् (ino) {
		यदि ((inode.i_links_गणनम् == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			गच्छ अग्र;

		pb.ino = ino;
		pb.inode = &inode;

		pb.सङ्कलय_dir = (LINUX_S_ISDIR(inode.i_बहुलकम्) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_खण्डम्_iterate3(fs, ino, 0, खण्डम्_buf,
					       process_खण्डम्, &pb);
		यदि (retval)
			प्रत्यागम retval;
		यदि (pb.error)
			प्रत्यागम pb.error;

	अग्र:
		retval = ext2fs_get_अग्र_inode(अवलोकनम्, &ino, &inode);
		यदि (retval == EXT2_ET_BAD_खण्डम्_अन्तर्_INODE_सारणी)
			गच्छ अग्र;
	}
	प्रत्यागम 0;
}

