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
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_وقت_H
#ضمن <sys/time.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية process_بلاڪ_struct {
	ext2_ino_t		ino;
	بنية ext2_inode *	inode;
	ext2fs_بلاڪ_bitmap	reserve;
	ext2fs_بلاڪ_bitmap	alloc_map;
	errcode_t		error;
	حرف			*buf;
	صحيح			جوڙيو_dir;
	صحيح			flags;
};

ساكن صحيح process_بلاڪ(ext2_filsys fs, blk64_t *بلاڪ_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_بلاڪ,
			 صحيح ref_وٿي, فراغ *priv_معلومات)
{
	بنية process_بلاڪ_struct *pb;
	errcode_t	retval;
	صحيح		ret;
	blk64_t		بلاڪ, orig;

	pb = (بنية process_بلاڪ_struct *) priv_معلومات;
	بلاڪ = orig = *بلاڪ_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	إذا (ext2fs_test_بلاڪ_bitmap2(pb->reserve, بلاڪ)) {
		افعل {
			إذا (++بلاڪ >= ext2fs_بلاڪون_ڳڻپ(fs->super))
				بلاڪ = fs->super->s_پهريون_معلومات_بلاڪ;
			إذا (بلاڪ == orig) {
				pb->error = EXT2_ET_بلاڪ_ALLOC_FAIL;
				أرجع بلاڪ_ABORT;
			}
		} طالما (ext2fs_test_بلاڪ_bitmap2(pb->reserve, بلاڪ) ||
			 ext2fs_test_بلاڪ_bitmap2(pb->alloc_map, بلاڪ));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		إذا (retval) {
			pb->error = retval;
			أرجع بلاڪ_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, بلاڪ, 1, pb->buf);
		إذا (retval) {
			pb->error = retval;
			أرجع بلاڪ_ABORT;
		}
		*بلاڪ_nr = بلاڪ;
		ext2fs_mark_بلاڪ_bitmap2(pb->alloc_map, بلاڪ);
		ret = بلاڪ_بدليو;
		إذا (pb->flags & EXT2_BMOVE_DEBUG)
			لکو("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (بنانشان) pb->ino, blockcnt, 
			       (بنانشان ڊگهو ڊگهو) orig,
			       (بنانشان ڊگهو ڊگهو) بلاڪ);
	}
	إذا (pb->جوڙيو_dir) {
		retval = ext2fs_جوڙيو_dir_block2(fs->dblist, pb->ino,
					       بلاڪ, blockcnt);
		إذا (retval) {
			pb->error = retval;
			ret |= بلاڪ_ABORT;
		}
	}
	أرجع ret;
}

errcode_t ext2fs_move_بلاڪون(ext2_filsys fs,
			     ext2fs_بلاڪ_bitmap reserve,
			     ext2fs_بلاڪ_bitmap alloc_map,
			     صحيح flags)
{
	ext2_ino_t	ino;
	بنية ext2_inode inode;
	errcode_t	retval;
	بنية process_بلاڪ_struct pb;
	ext2_inode_جاچ	جاچ;
	حرف		*بلاڪ_buf;

	retval = ext2fs_open_inode_جاچ(fs, 0, &جاچ);
	إذا (retval)
		أرجع retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->بلاڪ_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &بلاڪ_buf);
	إذا (retval)
		أرجع retval;
	pb.buf = بلاڪ_buf + fs->blocksize * 3;

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
			أرجع retval;
	}

	retval = ext2fs_get_ايندڙ_inode(جاچ, &ino, &inode);
	إذا (retval)
		أرجع retval;

	طالما (ino) {
		إذا ((inode.i_links_ڳڻپ == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			اذهب ايندڙ;

		pb.ino = ino;
		pb.inode = &inode;

		pb.جوڙيو_dir = (LINUX_S_ISDIR(inode.i_گهڻائي) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_بلاڪ_iterate3(fs, ino, 0, بلاڪ_buf,
					       process_بلاڪ, &pb);
		إذا (retval)
			أرجع retval;
		إذا (pb.error)
			أرجع pb.error;

	ايندڙ:
		retval = ext2fs_get_ايندڙ_inode(جاچ, &ino, &inode);
		إذا (retval == EXT2_ET_BAD_بلاڪ_اندريون_INODE_جدول)
			اذهب ايندڙ;
	}
	أرجع 0;
}

