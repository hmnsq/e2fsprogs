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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_ਸਮਾਂ_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਬਣਤਰ process_ਖੰਡ_struct {
	ext2_ino_t		ino;
	ਬਣਤਰ ext2_inode *	inode;
	ext2fs_ਖੰਡ_bitmap	reserve;
	ext2fs_ਖੰਡ_bitmap	alloc_map;
	errcode_t		error;
	ਅੱਖਰ			*buf;
	ਪੂਰਨ			ਜੋੜੋ_dir;
	ਪੂਰਨ			flags;
};

ਸਥਿਰ ਪੂਰਨ process_ਖੰਡ(ext2_filsys fs, blk64_t *ਖੰਡ_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_ਖੰਡ,
			 ਪੂਰਨ ref_ਵਿੱਥ, ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ process_ਖੰਡ_struct *pb;
	errcode_t	retval;
	ਪੂਰਨ		ret;
	blk64_t		ਖੰਡ, orig;

	pb = (ਬਣਤਰ process_ਖੰਡ_struct *) priv_ਅੰਕੜੇ;
	ਖੰਡ = orig = *ਖੰਡ_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	ਜੇ (ext2fs_test_ਖੰਡ_bitmap2(pb->reserve, ਖੰਡ)) {
		ਕਰੋ {
			ਜੇ (++ਖੰਡ >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))
				ਖੰਡ = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
			ਜੇ (ਖੰਡ == orig) {
				pb->error = EXT2_ET_ਖੰਡ_ALLOC_FAIL;
				ਵਾਪਸ ਖੰਡ_ABORT;
			}
		} ਜਦੋਂ (ext2fs_test_ਖੰਡ_bitmap2(pb->reserve, ਖੰਡ) ||
			 ext2fs_test_ਖੰਡ_bitmap2(pb->alloc_map, ਖੰਡ));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		ਜੇ (retval) {
			pb->error = retval;
			ਵਾਪਸ ਖੰਡ_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, ਖੰਡ, 1, pb->buf);
		ਜੇ (retval) {
			pb->error = retval;
			ਵਾਪਸ ਖੰਡ_ABORT;
		}
		*ਖੰਡ_nr = ਖੰਡ;
		ext2fs_mark_ਖੰਡ_bitmap2(pb->alloc_map, ਖੰਡ);
		ret = ਖੰਡ_ਬਦਲਿਆ;
		ਜੇ (pb->flags & EXT2_BMOVE_DEBUG)
			ਲਿਖੋ("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (ਬਿਨਾਨਿਸ਼ਾਨੀ) pb->ino, blockcnt, 
			       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) orig,
			       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਖੰਡ);
	}
	ਜੇ (pb->ਜੋੜੋ_dir) {
		retval = ext2fs_ਜੋੜੋ_dir_block2(fs->dblist, pb->ino,
					       ਖੰਡ, blockcnt);
		ਜੇ (retval) {
			pb->error = retval;
			ret |= ਖੰਡ_ABORT;
		}
	}
	ਵਾਪਸ ret;
}

errcode_t ext2fs_move_ਖੰਡਾਂ(ext2_filsys fs,
			     ext2fs_ਖੰਡ_bitmap reserve,
			     ext2fs_ਖੰਡ_bitmap alloc_map,
			     ਪੂਰਨ flags)
{
	ext2_ino_t	ino;
	ਬਣਤਰ ext2_inode inode;
	errcode_t	retval;
	ਬਣਤਰ process_ਖੰਡ_struct pb;
	ext2_inode_ਜਾਂਚ	ਜਾਂਚ;
	ਅੱਖਰ		*ਖੰਡ_buf;

	retval = ext2fs_open_inode_ਜਾਂਚ(fs, 0, &ਜਾਂਚ);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->ਖੰਡ_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &ਖੰਡ_buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	pb.buf = ਖੰਡ_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	ਜੇ (flags & EXT2_BMOVE_GET_DBLIST) {
		ਜੇ (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = ਨਿਰਾ;
		}
		retval = ext2fs_init_dblist(fs, 0);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	retval = ext2fs_get_ਅਗਲਾ_inode(ਜਾਂਚ, &ino, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜਦੋਂ (ino) {
		ਜੇ ((inode.i_links_ਗਿਣਤੀ == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			ਜਾਓ ਅਗਲਾ;

		pb.ino = ino;
		pb.inode = &inode;

		pb.ਜੋੜੋ_dir = (LINUX_S_ISDIR(inode.i_ਬਹੁਲਕ) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_ਖੰਡ_iterate3(fs, ino, 0, ਖੰਡ_buf,
					       process_ਖੰਡ, &pb);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਜੇ (pb.error)
			ਵਾਪਸ pb.error;

	ਅਗਲਾ:
		retval = ext2fs_get_ਅਗਲਾ_inode(ਜਾਂਚ, &ino, &inode);
		ਜੇ (retval == EXT2_ET_BAD_ਖੰਡ_ਵਿੱਚ_INODE_ਸਾਰਣੀ)
			ਜਾਓ ਅਗਲਾ;
	}
	ਵਾਪਸ 0;
}

