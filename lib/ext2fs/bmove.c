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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_ಸಮಯ_H
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ರಚನೆ process_ಖಂಡ_struct {
	ext2_ino_t		ino;
	ರಚನೆ ext2_inode *	inode;
	ext2fs_ಖಂಡ_bitmap	reserve;
	ext2fs_ಖಂಡ_bitmap	alloc_map;
	errcode_t		error;
	ಅಕ್ಷರ			*buf;
	ಪೂರ್ಣಾಂಕ			ಕೂಡಿಸು_dir;
	ಪೂರ್ಣಾಂಕ			flags;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ process_ಖಂಡ(ext2_filsys fs, blk64_t *ಖಂಡ_nr,
			 e2_blkcnt_t blockcnt, blk64_t ref_ಖಂಡ,
			 ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ, ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ process_ಖಂಡ_struct *pb;
	errcode_t	retval;
	ಪೂರ್ಣಾಂಕ		ret;
	blk64_t		ಖಂಡ, orig;

	pb = (ರಚನೆ process_ಖಂಡ_struct *) priv_ದತ್ತಾಂಶ;
	ಖಂಡ = orig = *ಖಂಡ_nr;
	ret = 0;

	/*
	 * Let's see if this is one which we need to relocate
	 */
	ಆದರೆ (ext2fs_test_ಖಂಡ_bitmap2(pb->reserve, ಖಂಡ)) {
		ಮಾಡು {
			ಆದರೆ (++ಖಂಡ >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))
				ಖಂಡ = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
			ಆದರೆ (ಖಂಡ == orig) {
				pb->error = EXT2_ET_ಖಂಡ_ALLOC_FAIL;
				ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
			}
		} ಆಗ (ext2fs_test_ಖಂಡ_bitmap2(pb->reserve, ಖಂಡ) ||
			 ext2fs_test_ಖಂಡ_bitmap2(pb->alloc_map, ಖಂಡ));

		retval = io_channel_read_blk64(fs->io, orig, 1, pb->buf);
		ಆದರೆ (retval) {
			pb->error = retval;
			ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		}
		retval = io_channel_write_blk64(fs->io, ಖಂಡ, 1, pb->buf);
		ಆದರೆ (retval) {
			pb->error = retval;
			ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		}
		*ಖಂಡ_nr = ಖಂಡ;
		ext2fs_mark_ಖಂಡ_bitmap2(pb->alloc_map, ಖಂಡ);
		ret = ಖಂಡ_ಬದಲಾಯಿತು;
		ಆದರೆ (pb->flags & EXT2_BMOVE_DEBUG)
			ಬರೆ("ino=%u, blockcnt=%lld, %llu->%llu\n",
			       (ಚಿಹ್ನೆರಹಿತ) pb->ino, blockcnt, 
			       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) orig,
			       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಖಂಡ);
	}
	ಆದರೆ (pb->ಕೂಡಿಸು_dir) {
		retval = ext2fs_ಕೂಡಿಸು_dir_block2(fs->dblist, pb->ino,
					       ಖಂಡ, blockcnt);
		ಆದರೆ (retval) {
			pb->error = retval;
			ret |= ಖಂಡ_ABORT;
		}
	}
	ಹಿಂತಿರುಗಿ ret;
}

errcode_t ext2fs_move_ಖಂಡಗಳು(ext2_filsys fs,
			     ext2fs_ಖಂಡ_bitmap reserve,
			     ext2fs_ಖಂಡ_bitmap alloc_map,
			     ಪೂರ್ಣಾಂಕ flags)
{
	ext2_ino_t	ino;
	ರಚನೆ ext2_inode inode;
	errcode_t	retval;
	ರಚನೆ process_ಖಂಡ_struct pb;
	ext2_inode_ಪರಿಶೀಲನೆ	ಪರಿಶೀಲನೆ;
	ಅಕ್ಷರ		*ಖಂಡ_buf;

	retval = ext2fs_open_inode_ಪರಿಶೀಲನೆ(fs, 0, &ಪರಿಶೀಲನೆ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	pb.reserve = reserve;
	pb.error = 0;
	pb.alloc_map = alloc_map ? alloc_map : fs->ಖಂಡ_map;
	pb.flags = flags;

	retval = ext2fs_get_array(4, fs->blocksize, &ಖಂಡ_buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	pb.buf = ಖಂಡ_buf + fs->blocksize * 3;

	/*
	 * If GET_DBLIST is set in the flags field, then we should
	 * gather directory block information while we're doing the
	 * block move.
	 */
	ಆದರೆ (flags & EXT2_BMOVE_GET_DBLIST) {
		ಆದರೆ (fs->dblist) {
			ext2fs_free_dblist(fs->dblist);
			fs->dblist = ಖಾಲಿ;
		}
		retval = ext2fs_init_dblist(fs, 0);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	retval = ext2fs_get_ಮುಂದಿನ_inode(ಪರಿಶೀಲನೆ, &ino, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆಗ (ino) {
		ಆದರೆ ((inode.i_links_ಎಣಿಕೆ == 0) ||
		    !ext2fs_inode_has_valid_blocks2(fs, &inode))
			ಹೋಗು ಮುಂದಿನ;

		pb.ino = ino;
		pb.inode = &inode;

		pb.ಕೂಡಿಸು_dir = (LINUX_S_ISDIR(inode.i_ಬಹುಲಕ) &&
			      flags & EXT2_BMOVE_GET_DBLIST);

		retval = ext2fs_ಖಂಡ_iterate3(fs, ino, 0, ಖಂಡ_buf,
					       process_ಖಂಡ, &pb);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಆದರೆ (pb.error)
			ಹಿಂತಿರುಗಿ pb.error;

	ಮುಂದಿನ:
		retval = ext2fs_get_ಮುಂದಿನ_inode(ಪರಿಶೀಲನೆ, &ino, &inode);
		ಆದರೆ (retval == EXT2_ET_BAD_ಖಂಡ_ಒಳ_INODE_ಕೋಷ್ಟಕ)
			ಹೋಗು ಮುಂದಿನ;
	}
	ಹಿಂತಿರುಗಿ 0;
}

