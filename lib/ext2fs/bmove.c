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
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#إذا HAVE_SYS_ویلا_H
#ضمن <sys/time.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية process_بلاک_struct {
	ext2_ino_t		ino;
	بنية ext2_inode *	inode;
	ext2fs_بلاک_bitmap	reserve;
	ext2fs_بلاک_bitmap	alloc_map;
	errcode_t		error;
	حرف			*buf;
	صحيح			جوڑو_dir;
	صحيح			flags;
};

ساكن صحيح process_بلاک(ext2_filsys fs, blk64_t *بلاک_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_بلاک,
			 صحيح ref_وتھ, فراغ *priv_انکڑے)
{
	بنية process_بلاک_struct *pb;
	errcode_t	retval;
	صحيح		ret;
	blk64_t		بلاک, orig;

	pb = (بنية process_بلاک_struct *) priv_انکڑے;
	بلاک = orig = *بلاک_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	إذا (ext2fs_test_بلاک_bitmap2(pb->reserve, بلاک)) {
		افعل {
			إذا (++بلاک >= ext2fs_بلاکاں_گنتی(fs->super))
				بلاک = fs->super->s_پہلا_انکڑے_بلاک;
			إذا (بلاک == orig) {
				pb->error = EXT2_ET_بلاک_ALLOC_FAIL;
				أرجع بلاک_ABORT;
			}
		} طالما (ext2fs_test_بلاک_bitmap2(pb->reserve, بلاک) ||
			 ext2fs_test_بلاک_bitmap2(pb->alloc_map, بلاک));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		إذا (retval) {
			pb->error = retval;
			أرجع بلاک_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, بلاک, 1, pb->buf);
		إذا (retval) {
			pb->error = retval;
			أرجع بلاک_ABORT;
		}
		*بلاک_nr = بلاک;
		ext2fs_mark_بلاک_bitmap2(pb->alloc_map, بلاک);
		ret = بلاک_بدلیا;
		إذا (pb->flags & EXT2_BMOVE_DEBUG)
			لکھو("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (بنانشانی) pb->ino, blockcnt, 
			       (بنانشانی لما لما) orig,
			       (بنانشانی لما لما) بلاک);
	}
	إذا (pb->جوڑو_dir) {
		retval = ext2fs_جوڑو_dir_block2(fs->dblist, pb->ino,
					       بلاک, blockcnt);
		إذا (retval) {
			pb->error = retval;
			ret |= بلاک_ABORT;
		}
	}
	أرجع ret;
}

errcode_t ext2fs_move_بلاکاں(ext2_filsys fs,
			     ext2fs_بلاک_bitmap reserve,
			     ext2fs_بلاک_bitmap alloc_map,
			     صحيح flags)
{
	ext2_ino_t	ino;
	بنية ext2_inode inode;
	errcode_t	retval;
	بنية process_بلاک_struct pb;
	ext2_inode_جانچ	جانچ;
	حرف		*بلاک_buf;

	retval = ext2fs_open_inode_جانچ(fs, 0, &جانچ);
	إذا (retval)
		أرجع retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->بلاک_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &بلاک_buf);
	إذا (retval)
		أرجع retval;
	pb.buf = بلاک_buf + fs->blocksize * 3;

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

	retval = ext2fs_get_اگلا_inode(جانچ, &ino, &inode);
	إذا (retval)
		أرجع retval;

	طالما (ino) {
		إذا ((inode.i_links_گنتی == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			اذهب اگلا;

		pb.ino = ino;
		pb.inode = &inode;

		pb.جوڑو_dir = (LINUX_S_ISDIR(inode.i_منوال) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_بلاک_iterate3(fs, ino, 0, بلاک_buf,
					       process_بلاک, &pb);
		إذا (retval)
			أرجع retval;
		إذا (pb.error)
			أرجع pb.error;

	اگلا:
		retval = ext2fs_get_اگلا_inode(جانچ, &ino, &inode);
		إذا (retval == EXT2_ET_BAD_بلاک_وچ_INODE_جدول)
			اذهب اگلا;
	}
	أرجع 0;
}

