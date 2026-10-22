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
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر
#إذا HAVE_SYS_زمان_H
#ضمن <sys/time.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية process_بلوک_struct {
	ext2_ino_t		ino;
	بنية ext2_inode *	inode;
	ext2fs_بلوک_bitmap	reserve;
	ext2fs_بلوک_bitmap	alloc_map;
	errcode_t		error;
	حرف			*buf;
	صحيح			جمع_کن_dir;
	صحيح			flags;
};

ساكن صحيح process_بلوک(ext2_filsys fs, blk64_t *بلوک_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_بلوک,
			 صحيح ref_جابه_جایی, فراغ *priv_داده)
{
	بنية process_بلوک_struct *pb;
	errcode_t	retval;
	صحيح		ret;
	blk64_t		بلوک, orig;

	pb = (بنية process_بلوک_struct *) priv_داده;
	بلوک = orig = *بلوک_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	إذا (ext2fs_test_بلوک_bitmap2(pb->reserve, بلوک)) {
		افعل {
			إذا (++بلوک >= ext2fs_بلوکها_شمار(fs->super))
				بلوک = fs->super->s_اولی_داده_بلوک;
			إذا (بلوک == orig) {
				pb->error = EXT2_ET_بلوک_ALLOC_FAIL;
				أرجع بلوک_ABORT;
			}
		} طالما (ext2fs_test_بلوک_bitmap2(pb->reserve, بلوک) ||
			 ext2fs_test_بلوک_bitmap2(pb->alloc_map, بلوک));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		إذا (retval) {
			pb->error = retval;
			أرجع بلوک_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, بلوک, 1, pb->buf);
		إذا (retval) {
			pb->error = retval;
			أرجع بلوک_ABORT;
		}
		*بلوک_nr = بلوک;
		ext2fs_mark_بلوک_bitmap2(pb->alloc_map, بلوک);
		ret = بلوک_تغییریافته;
		إذا (pb->flags & EXT2_BMOVE_DEBUG)
			چاپ_کن("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (بیعلامت) pb->ino, blockcnt, 
			       (بیعلامت دراز دراز) orig,
			       (بیعلامت دراز دراز) بلوک);
	}
	إذا (pb->جمع_کن_dir) {
		retval = ext2fs_جمع_کن_dir_block2(fs->dblist, pb->ino,
					       بلوک, blockcnt);
		إذا (retval) {
			pb->error = retval;
			ret |= بلوک_ABORT;
		}
	}
	أرجع ret;
}

errcode_t ext2fs_move_بلوکها(ext2_filsys fs,
			     ext2fs_بلوک_bitmap reserve,
			     ext2fs_بلوک_bitmap alloc_map,
			     صحيح flags)
{
	ext2_ino_t	ino;
	بنية ext2_inode inode;
	errcode_t	retval;
	بنية process_بلوک_struct pb;
	ext2_inode_پیمایش	پیمایش;
	حرف		*بلوک_buf;

	retval = ext2fs_open_inode_پیمایش(fs, 0, &پیمایش);
	إذا (retval)
		أرجع retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->بلوک_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &بلوک_buf);
	إذا (retval)
		أرجع retval;
	pb.buf = بلوک_buf + fs->blocksize * 3;

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

	retval = ext2fs_get_بعدی_inode(پیمایش, &ino, &inode);
	إذا (retval)
		أرجع retval;

	طالما (ino) {
		إذا ((inode.i_links_شمار == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			اذهب بعدی;

		pb.ino = ino;
		pb.inode = &inode;

		pb.جمع_کن_dir = (LINUX_S_ISDIR(inode.i_نما) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_بلوک_iterate3(fs, ino, 0, بلوک_buf,
					       process_بلوک, &pb);
		إذا (retval)
			أرجع retval;
		إذا (pb.error)
			أرجع pb.error;

	بعدی:
		retval = ext2fs_get_بعدی_inode(پیمایش, &ino, &inode);
		إذا (retval == EXT2_ET_BAD_بلوک_درون_INODE_جدول)
			اذهب بعدی;
	}
	أرجع 0;
}

