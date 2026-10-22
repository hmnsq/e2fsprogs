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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر HAVE_SYS_وقت_H
#شامل <sys/time.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

ساخت process_بلاک_struct {
	ext2_ino_t		ino;
	ساخت ext2_inode *	inode;
	ext2fs_بلاک_bitmap	reserve;
	ext2fs_بلاک_bitmap	alloc_map;
	errcode_t		error;
	حرف			*buf;
	صحیح			جوڑو_dir;
	صحیح			flags;
};

ساکن صحیح process_بلاک(ext2_filsys fs, blk64_t *بلاک_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_بلاک,
			 صحیح ref_انحراف, خالی *priv_ڈیٹا)
{
	ساخت process_بلاک_struct *pb;
	errcode_t	retval;
	صحیح		ret;
	blk64_t		بلاک, orig;

	pb = (ساخت process_بلاک_struct *) priv_ڈیٹا;
	بلاک = orig = *بلاک_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	اگر (ext2fs_test_بلاک_bitmap2(pb->reserve, بلاک)) {
		کرو {
			اگر (++بلاک >= ext2fs_بلاکیں_گنتی(fs->super))
				بلاک = fs->super->s_پہلا_ڈیٹا_بلاک;
			اگر (بلاک == orig) {
				pb->error = EXT2_ET_بلاک_ALLOC_FAIL;
				واپس بلاک_ABORT;
			}
		} جب (ext2fs_test_بلاک_bitmap2(pb->reserve, بلاک) ||
			 ext2fs_test_بلاک_bitmap2(pb->alloc_map, بلاک));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		اگر (retval) {
			pb->error = retval;
			واپس بلاک_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, بلاک, 1, pb->buf);
		اگر (retval) {
			pb->error = retval;
			واپس بلاک_ABORT;
		}
		*بلاک_nr = بلاک;
		ext2fs_mark_بلاک_bitmap2(pb->alloc_map, بلاک);
		ret = بلاک_بدلا;
		اگر (pb->flags & EXT2_BMOVE_DEBUG)
			لکھو("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (غیرعلامتی) pb->ino, blockcnt, 
			       (غیرعلامتی طویل طویل) orig,
			       (غیرعلامتی طویل طویل) بلاک);
	}
	اگر (pb->جوڑو_dir) {
		retval = ext2fs_جوڑو_dir_block2(fs->dblist, pb->ino,
					       بلاک, blockcnt);
		اگر (retval) {
			pb->error = retval;
			ret |= بلاک_ABORT;
		}
	}
	واپس ret;
}

errcode_t ext2fs_move_بلاکیں(ext2_filsys fs,
			     ext2fs_بلاک_bitmap reserve,
			     ext2fs_بلاک_bitmap alloc_map,
			     صحیح flags)
{
	ext2_ino_t	ino;
	ساخت ext2_inode inode;
	errcode_t	retval;
	ساخت process_بلاک_struct pb;
	ext2_inode_جائزہ	جائزہ;
	حرف		*بلاک_buf;

	retval = ext2fs_open_inode_جائزہ(fs, 0, &جائزہ);
	اگر (retval)
		واپس retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->بلاک_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &بلاک_buf);
	اگر (retval)
		واپس retval;
	pb.buf = بلاک_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	اگر (flags & EXT2_BMOVE_GET_DBLIST) {
		اگر (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = تہی;
		}
		retval = ext2fs_init_dblist(fs, 0);
		اگر (retval)
			واپس retval;
	}

	retval = ext2fs_get_اگلا_inode(جائزہ, &ino, &inode);
	اگر (retval)
		واپس retval;

	جب (ino) {
		اگر ((inode.i_links_گنتی == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			جاؤ اگلا;

		pb.ino = ino;
		pb.inode = &inode;

		pb.جوڑو_dir = (LINUX_S_ISDIR(inode.i_منوال) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_بلاک_iterate3(fs, ino, 0, بلاک_buf,
					       process_بلاک, &pb);
		اگر (retval)
			واپس retval;
		اگر (pb.error)
			واپس pb.error;

	اگلا:
		retval = ext2fs_get_اگلا_inode(جائزہ, &ino, &inode);
		اگر (retval == EXT2_ET_BAD_بلاک_اندر_INODE_جدول)
			جاؤ اگلا;
	}
	واپس 0;
}

