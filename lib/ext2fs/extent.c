/*
 * extent.c --- routines to implement extents support
 *
 * Copyright (C) 2007 Theodore Ts'o.
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
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "e2image.h"

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

ರಚನೆ extent_ಮಾರ್ಗ {
	ಅಕ್ಷರ		*buf;
	ಪೂರ್ಣಾಂಕ		entries;
	ಪೂರ್ಣಾಂಕ		max_entries;
	ಪೂರ್ಣಾಂಕ		ಎಡ;
	ಪೂರ್ಣಾಂಕ		ಭೇಟಿ_ಅಂಕೆ;
	ಪೂರ್ಣಾಂಕ		flags;
	blk64_t		ಕೊನೆ_blk;
	blk64_t		blk;
	ಶೂನ್ಯ		*curr;
};


ರಚನೆ ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	ರಚನೆ ext2_inode	*inode;
	ರಚನೆ ext2_inode	inodebuf;
	ಪೂರ್ಣಾಂಕ			type;
	ಪೂರ್ಣಾಂಕ			level;
	ಪೂರ್ಣಾಂಕ			max_ಆಳ;
	ಪೂರ್ಣಾಂಕ			max_paths;
	ರಚನೆ extent_ಮಾರ್ಗ	*ಮಾರ್ಗ;
};

ರಚನೆ ext2_extent_ಮಾರ್ಗ {
	errcode_t		magic;
	ಪೂರ್ಣಾಂಕ			ಎಲೆ_ಎತ್ತರ;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
ಸ್ಥಿರ ಶೂನ್ಯ dbg_show_ಶೀರ್ಷಿಕೆ(ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *eh)
{
	ಬರೆ("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_magic),
			ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_entries),
			ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_max),
			ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_ಆಳ),
			ext2fs_le32_ಗಮ್ಯ_cpu(eh->eh_generation));
}

ಸ್ಥಿರ ಶೂನ್ಯ dbg_show_ಸೂಚ್ಯಂಕ(ರಚನೆ ext3_extent_idx *ix)
{
	ಬರೆ("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_ಗಮ್ಯ_cpu(ix->ei_ಖಂಡ),
			ext2fs_le32_ಗಮ್ಯ_cpu(ix->ei_ಎಲೆ),
			ext2fs_le16_ಗಮ್ಯ_cpu(ix->ei_ಎಲೆ_hi),
			ext2fs_le16_ಗಮ್ಯ_cpu(ix->ei_ಬಳಸದ));
}

ಸ್ಥಿರ ಶೂನ್ಯ dbg_show_extent(ರಚನೆ ext3_extent *ex)
{
	ಬರೆ("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_ಗಮ್ಯ_cpu(ex->ee_ಖಂಡ),
			ext2fs_le32_ಗಮ್ಯ_cpu(ex->ee_ಖಂಡ) +
			ext2fs_le16_ಗಮ್ಯ_cpu(ex->ee_len) - 1,
			ext2fs_le16_ಗಮ್ಯ_cpu(ex->ee_len),
			ext2fs_le32_ಗಮ್ಯ_cpu(ex->ee_ಆರಂಭ),
			ext2fs_le16_ಗಮ್ಯ_cpu(ex->ee_ಆರಂಭ_hi));
}

ಸ್ಥಿರ ಶೂನ್ಯ dbg_print_extent(ಅಕ್ಷರ *desc, ರಚನೆ ext2fs_extent *extent)
{
	ಆದರೆ (desc)
		ಬರೆ("%s: ", desc);
	ಬರೆ("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ಆದರೆ (extent->e_flags & EXT2_EXTENT_FLAGS_ಎಲೆ)
		fputs("LEAF ", stdout);
	ಆದರೆ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ಆದರೆ (extent->e_flags & EXT2_EXTENT_FLAGS_ಎರಡನೇ_ಭೇಟಿ)
		fputs("2ND_VISIT ", stdout);
	ಆದರೆ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

ಸ್ಥಿರ ಶೂನ್ಯ dump_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್, ರಚನೆ ext2_extent_handle *handle,
		      ರಚನೆ extent_ಮಾರ್ಗ *ಮಾರ್ಗ)
{
	ರಚನೆ extent_ಮಾರ್ಗ *ppp = ಮಾರ್ಗ;
	ಬರೆ("%s: level=%d\n", ಟ್ಯಾಗ್, handle->level);

	ಮಾಡು {
		ಬರೆ("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       ಟ್ಯಾಗ್, (ppp - handle->ಮಾರ್ಗ), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->ಎಡ, ppp->ಭೇಟಿ_ಅಂಕೆ, ppp->flags,
		       ppp->ಕೊನೆ_blk, ppp->curr, ppp->curr - (ಶೂನ್ಯ *)ppp->buf);
		ಬರೆ("  ");
		dbg_show_ಶೀರ್ಷಿಕೆ((ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *)ppp->buf);
		ಆದರೆ (ppp->curr) {
			ಬರೆ("  ");
			dbg_show_ಸೂಚ್ಯಂಕ(ppp->curr);
			ಬರೆ("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} ಆಗ (ppp >= handle->ಮಾರ್ಗ);
	fflush(stdout);

	ಹಿಂತಿರುಗಿ;
}

#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು dbg_show_ಶೀರ್ಷಿಕೆ(eh) ಮಾಡು { } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು dbg_show_ಸೂಚ್ಯಂಕ(ix) ಮಾಡು { } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು dbg_show_extent(ex) ಮಾಡು { } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು dbg_print_extent(desc, ex) ಮಾಡು { } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು dump_ಮಾರ್ಗ(ಟ್ಯಾಗ್, handle, ಮಾರ್ಗ) ಮಾಡು { } ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_ಶೀರ್ಷಿಕೆ_verify(ಶೂನ್ಯ *ptr, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	ಪೂರ್ಣಾಂಕ eh_max, ನಮೂದು_ಗಾತ್ರ;
	ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *eh = ptr;

	dbg_show_ಶೀರ್ಷಿಕೆ(eh);
	ಆದರೆ (ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_ಶೀರ್ಷಿಕೆ_BAD;
	ಆದರೆ (ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_entries) > ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_max))
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_ಶೀರ್ಷಿಕೆ_BAD;
	ಆದರೆ (eh->eh_ಆಳ == 0)
		ನಮೂದು_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext3_extent);
	ಇಲ್ಲದಿದ್ದರೆ
		ನಮೂದು_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext3_extent_idx);

	eh_max = (ಗಾತ್ರ_2 - ಗಾತ್ರ(*eh)) / ನಮೂದು_ಗಾತ್ರ;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	ಆದರೆ ((ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_max) < (eh_max - 2)))
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_ಶೀರ್ಷಿಕೆ_BAD;

	ಹಿಂತಿರುಗಿ 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
ಶೂನ್ಯ ext2fs_extent_free(ext2_extent_handle_t handle)
{
	ಪೂರ್ಣಾಂಕ			i;

	ಆದರೆ (!handle)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (handle->ಮಾರ್ಗ) {
		ಗಾಗಿ (i = 1; i < handle->max_paths; i++) {
			ಆದರೆ (handle->ಮಾರ್ಗ[i].buf)
				ext2fs_free_mem(&handle->ಮಾರ್ಗ[i].buf);
		}
		ext2fs_free_mem(&handle->ಮಾರ್ಗ);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	ಹಿಂತಿರುಗಿ ext2fs_extent_open2(fs, ino, ಖಾಲಿ, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    ರಚನೆ ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	ರಚನೆ ext2_extent_handle	*handle;
	errcode_t			retval;
	ಪೂರ್ಣಾಂಕ				i;
	ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!inode)
		ಆದರೆ ((ino == 0) || (ino > fs->super->s_inodes_ಎಣಿಕೆ))
			ಹಿಂತಿರುಗಿ EXT2_ET_BAD_INODE_ಅಂಕೆ;

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_extent_handle), &handle);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	memset(handle, 0, ಗಾತ್ರ(ರಚನೆ ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	ಆದರೆ (inode) {
		handle->inode = inode;
	} ಇಲ್ಲದಿದ್ದರೆ {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}

	eh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) &handle->inode->i_ಖಂಡ[0];

	ಗಾಗಿ (i=0; i < EXT2_N_ಖಂಡಗಳು; i++)
		ಆದರೆ (handle->inode->i_ಖಂಡ[i])
			ನಿಲ್ಲಿಸು;
	ಆದರೆ (i >= EXT2_N_ಖಂಡಗಳು) {
		eh->eh_magic = ext2fs_cpu_ಗಮ್ಯ_le16(EXT3_EXT_MAGIC);
		eh->eh_ಆಳ = 0;
		eh->eh_entries = 0;
		i = (ಗಾತ್ರ(handle->inode->i_ಖಂಡ) - ಗಾತ್ರ(*eh)) /
			ಗಾತ್ರ(ರಚನೆ ext3_extent);
		eh->eh_max = ext2fs_cpu_ಗಮ್ಯ_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	ಆದರೆ (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		ಹೋಗು errout;
	}

	retval = ext2fs_extent_ಶೀರ್ಷಿಕೆ_verify(eh, ಗಾತ್ರ(handle->inode->i_ಖಂಡ));
	ಆದರೆ (retval)
		ಹೋಗು errout;

	handle->max_ಆಳ = ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_ಆಳ);
	handle->type = ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_magic);

	handle->max_paths = handle->max_ಆಳ + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    ಗಾತ್ರ(ರಚನೆ extent_ಮಾರ್ಗ),
				    &handle->ಮಾರ್ಗ);
	handle->ಮಾರ್ಗ[0].buf = (ಅಕ್ಷರ *) handle->inode->i_ಖಂಡ;

	handle->ಮಾರ್ಗ[0].ಎಡ = handle->ಮಾರ್ಗ[0].entries =
		ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_entries);
	handle->ಮಾರ್ಗ[0].max_entries = ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_max);
	handle->ಮಾರ್ಗ[0].curr = 0;
	handle->ಮಾರ್ಗ[0].ಕೊನೆ_blk =
		(EXT2_I_ಗಾತ್ರ(handle->inode) + fs->blocksize - 1) >>
		 EXT2_ಖಂಡ_ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು(fs->super);
	handle->ಮಾರ್ಗ[0].blk = 0;
	handle->ಮಾರ್ಗ[0].ಭೇಟಿ_ಅಂಕೆ = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	ಹಿಂತಿರುಗಿ 0;

errout:
	ext2fs_extent_free(handle);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    ಪೂರ್ಣಾಂಕ flags, ರಚನೆ ext2fs_extent *extent)
{
	ರಚನೆ extent_ಮಾರ್ಗ	*ಮಾರ್ಗ, *newpath, *tp;
	ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ	*eh;
	ರಚನೆ ext3_extent_idx		*ix = 0;
	ರಚನೆ ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				ಕೊನೆ_blk;
	ಪೂರ್ಣಾಂಕ				orig_op, op, l;
	ಪೂರ್ಣಾಂಕ				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ಆದರೆ (!handle->ಮಾರ್ಗ)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

	orig_op = op = flags & EXT2_EXTENT_MOVE_ಮುಖವಾಡ;

retry:
	ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;
	ಆದರೆ ((orig_op == EXT2_EXTENT_ಮುಂದಿನ) ||
	    (orig_op == EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ)) {
		ಆದರೆ (handle->level < handle->max_ಆಳ) {
			/* interior node */
			ಆದರೆ (ಮಾರ್ಗ->ಭೇಟಿ_ಅಂಕೆ == 0) {
				ಮಾರ್ಗ->ಭೇಟಿ_ಅಂಕೆ++;
				op = EXT2_EXTENT_ಕೆಳಕ್ಕೆ;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮಾರ್ಗ->ಎಡ > 0)
				op = EXT2_EXTENT_ಮುಂದಿನ_SIB;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (handle->level > 0)
				op = EXT2_EXTENT_ಮೇಲೆ;
			ಇಲ್ಲದಿದ್ದರೆ
				ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NO_ಮುಂದಿನ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* leaf node */
			ಆದರೆ (ಮಾರ್ಗ->ಎಡ > 0)
				op = EXT2_EXTENT_ಮುಂದಿನ_SIB;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (handle->level > 0)
				op = EXT2_EXTENT_ಮೇಲೆ;
			ಇಲ್ಲದಿದ್ದರೆ
				ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NO_ಮುಂದಿನ;
		}
		ಆದರೆ (op != EXT2_EXTENT_ಮುಂದಿನ_SIB) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_GET_EXTENT
			ಬರೆ("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_ಕೆಳಕ್ಕೆ) ? "down" :
			       ((op == EXT2_EXTENT_ಮೇಲೆ) ? "up" : "unknown"));
#ಆದರೆ_ಅಂತ್ಯ
		}
	}

	ಆದರೆ ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_ಎಲೆ)) {
		ಆದರೆ (handle->level < handle->max_ಆಳ) {
			/* interior node */
			ಆದರೆ (ಮಾರ್ಗ->ಭೇಟಿ_ಅಂಕೆ > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_ಕೆಳಕ್ಕೆ_AND_ಕೊನೆಯ;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮಾರ್ಗ->ಎಡ < ಮಾರ್ಗ->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (handle->level > 0)
				op = EXT2_EXTENT_ಮೇಲೆ;
			ಇಲ್ಲದಿದ್ದರೆ
				ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NO_PREV;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* leaf node */
			ಆದರೆ (ಮಾರ್ಗ->ಎಡ < ಮಾರ್ಗ->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (handle->level > 0)
				op = EXT2_EXTENT_ಮೇಲೆ;
			ಇಲ್ಲದಿದ್ದರೆ
				ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NO_PREV;
		}
		ಆದರೆ (op != EXT2_EXTENT_PREV_SIB) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_GET_EXTENT
			ಬರೆ("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_ಕೆಳಕ್ಕೆ_AND_ಕೊನೆಯ) ? "down/last" :
			       ((op == EXT2_EXTENT_ಮೇಲೆ) ? "up" : "unknown"));
#ಆದರೆ_ಅಂತ್ಯ
		}
	}

	ಆದರೆ (orig_op == EXT2_EXTENT_ಕೊನೆಯ_ಎಲೆ) {
		ಆದರೆ ((handle->level < handle->max_ಆಳ) &&
		    (ಮಾರ್ಗ->ಎಡ == 0))
			op = EXT2_EXTENT_ಕೆಳಕ್ಕೆ;
		ಇಲ್ಲದಿದ್ದರೆ
			op = EXT2_EXTENT_ಕೊನೆಯ_SIB;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_GET_EXTENT
		ಬರೆ("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_ಕೆಳಕ್ಕೆ) ? "down" : "last_sib");
#ಆದರೆ_ಅಂತ್ಯ
	}

	ಆಯ್ಕೆ (op) {
	ಸ್ಥಿತಿ EXT2_EXTENT_CURRENT:
		ix = ಮಾರ್ಗ->curr;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2_EXTENT_ಬೇರು:
		handle->level = 0;
		ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;
		/* fallthrough */
	ಸ್ಥಿತಿ EXT2_EXTENT_ಮೊದಲ_SIB:
		ಮಾರ್ಗ->ಎಡ = ಮಾರ್ಗ->entries;
		ಮಾರ್ಗ->curr = 0;
		/* fallthrough */
	ಸ್ಥಿತಿ EXT2_EXTENT_ಮುಂದಿನ_SIB:
		ಆದರೆ (ಮಾರ್ಗ->ಎಡ <= 0)
			ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NO_ಮುಂದಿನ;
		ಆದರೆ (ಮಾರ್ಗ->curr) {
			ix = ಮಾರ್ಗ->curr;
			ix++;
		} ಇಲ್ಲದಿದ್ದರೆ {
			eh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) ಮಾರ್ಗ->buf;
			ix = EXT_ಮೊದಲ_ಸೂಚ್ಯಂಕ(eh);
		}
		ಮಾರ್ಗ->ಎಡ--;
		ಮಾರ್ಗ->curr = ix;
		ಮಾರ್ಗ->ಭೇಟಿ_ಅಂಕೆ = 0;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2_EXTENT_PREV_SIB:
		ಆದರೆ (!ಮಾರ್ಗ->curr ||
		    ಮಾರ್ಗ->ಎಡ+1 >= ಮಾರ್ಗ->entries)
			ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NO_PREV;
		ix = ಮಾರ್ಗ->curr;
		ix--;
		ಮಾರ್ಗ->curr = ix;
		ಮಾರ್ಗ->ಎಡ++;
		ಆದರೆ (handle->level < handle->max_ಆಳ)
			ಮಾರ್ಗ->ಭೇಟಿ_ಅಂಕೆ = 1;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2_EXTENT_ಕೊನೆಯ_SIB:
		eh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) ಮಾರ್ಗ->buf;
		ಮಾರ್ಗ->curr = EXT_ಕೊನೆಯ_EXTENT(eh);
		ix = ಮಾರ್ಗ->curr;
		ಮಾರ್ಗ->ಎಡ = 0;
		ಮಾರ್ಗ->ಭೇಟಿ_ಅಂಕೆ = 0;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2_EXTENT_ಮೇಲೆ:
		ಆದರೆ (handle->level <= 0)
			ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NO_ಮೇಲೆ;
		handle->level--;
		ಮಾರ್ಗ--;
		ix = ಮಾರ್ಗ->curr;
		ಆದರೆ ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_ಎಲೆ))
			ಮಾರ್ಗ->ಭೇಟಿ_ಅಂಕೆ = 0;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2_EXTENT_ಕೆಳಕ್ಕೆ:
	ಸ್ಥಿತಿ EXT2_EXTENT_ಕೆಳಕ್ಕೆ_AND_ಕೊನೆಯ:
		ಆದರೆ (!ಮಾರ್ಗ->curr ||(handle->level >= handle->max_ಆಳ))
			ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NO_ಕೆಳಕ್ಕೆ;

		ix = ಮಾರ್ಗ->curr;
		newpath = ಮಾರ್ಗ + 1;
		ಆದರೆ (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
		}
		blk = ext2fs_le32_ಗಮ್ಯ_cpu(ix->ei_ಎಲೆ) +
			((__u64) ext2fs_le16_ಗಮ್ಯ_cpu(ix->ei_ಎಲೆ_hi) << 32);
		ಗಾಗಿ (l = handle->level, tp = ಮಾರ್ಗ; l > 0; l--, tp--) {
			ಆದರೆ (blk == tp->blk)
				ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_ಚಕ್ರ;
		}
		newpath->blk = blk;
		ಆದರೆ ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		ಇಲ್ಲದಿದ್ದರೆ {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
		}
		handle->level++;

		eh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) newpath->buf;

		retval = ext2fs_extent_ಶೀರ್ಷಿಕೆ_verify(eh, handle->fs->blocksize);
		ಆದರೆ (retval) {
			handle->level--;
			ಹಿಂತಿರುಗಿ retval;
		}

		ಆದರೆ (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_ಖಂಡ_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->ಎಡ = newpath->entries =
			ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		ಆದರೆ (newpath->ಎಡ <= 0)
			ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NO_ಕೆಳಕ್ಕೆ;

		ಆದರೆ (ಮಾರ್ಗ->ಎಡ > 0) {
			ix++;
			newpath->ಕೊನೆ_blk = ext2fs_le32_ಗಮ್ಯ_cpu(ix->ei_ಖಂಡ);
		} ಇಲ್ಲದಿದ್ದರೆ
			newpath->ಕೊನೆ_blk = ಮಾರ್ಗ->ಕೊನೆ_blk;

		ಮಾರ್ಗ = newpath;
		ಆದರೆ (op == EXT2_EXTENT_ಕೆಳಕ್ಕೆ) {
			ix = EXT_ಮೊದಲ_ಸೂಚ್ಯಂಕ((ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) eh);
			ಮಾರ್ಗ->curr = ix;
			ಮಾರ್ಗ->ಎಡ = ಮಾರ್ಗ->entries - 1;
			ಮಾರ್ಗ->ಭೇಟಿ_ಅಂಕೆ = 0;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ix = EXT_ಕೊನೆಯ_ಸೂಚ್ಯಂಕ((ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) eh);
			ಮಾರ್ಗ->curr = ix;
			ಮಾರ್ಗ->ಎಡ = 0;
			ಆದರೆ (handle->level < handle->max_ಆಳ)
				ಮಾರ್ಗ->ಭೇಟಿ_ಅಂಕೆ = 1;
		}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_GET_EXTENT
		ಬರೆ("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_ಆಳ,
			   ಮಾರ್ಗ->ಕೊನೆ_blk);
#ಆದರೆ_ಅಂತ್ಯ
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
	}

	ಆದರೆ (!ix)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

	extent->e_flags = 0;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_GET_EXTENT
	ಬರೆ("(Left %d)\n", ಮಾರ್ಗ->ಎಡ);
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (handle->level == handle->max_ಆಳ) {
		ex = (ರಚನೆ ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_ಗಮ್ಯ_cpu(ex->ee_ಆರಂಭ) +
			((__u64) ext2fs_le16_ಗಮ್ಯ_cpu(ex->ee_ಆರಂಭ_hi) << 32);
		extent->e_lblk = ext2fs_le32_ಗಮ್ಯ_cpu(ex->ee_ಖಂಡ);
		extent->e_len = ext2fs_le16_ಗಮ್ಯ_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_ಎಲೆ;
		ಆದರೆ (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		extent->e_pblk = ext2fs_le32_ಗಮ್ಯ_cpu(ix->ei_ಎಲೆ) +
			((__u64) ext2fs_le16_ಗಮ್ಯ_cpu(ix->ei_ಎಲೆ_hi) << 32);
		extent->e_lblk = ext2fs_le32_ಗಮ್ಯ_cpu(ix->ei_ಖಂಡ);
		ಆದರೆ (ಮಾರ್ಗ->ಎಡ > 0) {
			ix++;
			ಕೊನೆ_blk = ext2fs_le32_ಗಮ್ಯ_cpu(ix->ei_ಖಂಡ);
		} ಇಲ್ಲದಿದ್ದರೆ
			ಕೊನೆ_blk = ಮಾರ್ಗ->ಕೊನೆ_blk;

		extent->e_len = ಕೊನೆ_blk - extent->e_lblk;
	}
	ಆದರೆ (ಮಾರ್ಗ->ಭೇಟಿ_ಅಂಕೆ)
		extent->e_flags |= EXT2_EXTENT_FLAGS_ಎರಡನೇ_ಭೇಟಿ;

	ಆದರೆ (((orig_op == EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ) ||
	     (orig_op == EXT2_EXTENT_PREV_ಎಲೆ)) &&
	    (handle->level != handle->max_ಆಳ))
		ಹೋಗು retry;

	ಆದರೆ ((orig_op == EXT2_EXTENT_ಕೊನೆಯ_ಎಲೆ) &&
	    ((handle->level != handle->max_ಆಳ) ||
	     (ಮಾರ್ಗ->ಎಡ != 0)))
		ಹೋಗು retry;

	ಆದರೆ (failed_csum)
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_CSUM_INVALID;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t update_ಮಾರ್ಗ(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	ರಚನೆ ext3_extent_idx		*ix;
	ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ	*eh;

	ಆದರೆ (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ix = handle->ಮಾರ್ಗ[handle->level - 1].curr;
		blk = ext2fs_le32_ಗಮ್ಯ_cpu(ix->ei_ಎಲೆ) +
			((__u64) ext2fs_le16_ಗಮ್ಯ_cpu(ix->ei_ಎಲೆ_hi) << 32);

		/* then update the checksum */
		eh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *)
				handle->ಮಾರ್ಗ[handle->level].buf;
		retval = ext2fs_extent_ಖಂಡ_csum_ಗಣ(handle->fs, handle->ino,
						      eh);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->ಮಾರ್ಗ[handle->level].buf);
	}
	ಹಿಂತಿರುಗಿ retval;
}

#ಆದರೆ 0
errcode_t ext2fs_extent_save_ಮಾರ್ಗ(ext2_extent_handle_t handle,
				  ext2_extent_ಮಾರ್ಗ_t *ret_ಮಾರ್ಗ)
{
	ext2_extent_ಮಾರ್ಗ_t	save_ಮಾರ್ಗ;
	ರಚನೆ ext2fs_extent	extent;
	ರಚನೆ ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = ext2fs_extent_get_info(handle, &info);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_extent_ಮಾರ್ಗ), &save_ಮಾರ್ಗ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	memset(save_ಮಾರ್ಗ, 0, ಗಾತ್ರ(ರಚನೆ ext2_extent_ಮಾರ್ಗ));

	save_ಮಾರ್ಗ->magic = EXT2_ET_MAGIC_EXTENT_ಮಾರ್ಗ;
	save_ಮಾರ್ಗ->ಎಲೆ_ಎತ್ತರ = info.max_ಆಳ - info.curr_level - 1;
	save_ಮಾರ್ಗ->lblk = extent.e_lblk;

	*ret_ಮಾರ್ಗ = save_ಮಾರ್ಗ;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_extent_free_ಮಾರ್ಗ(ext2_extent_ಮಾರ್ಗ_t ಮಾರ್ಗ)
{
	EXT2_CHECK_MAGIC(ಮಾರ್ಗ, EXT2_ET_MAGIC_EXTENT_ಮಾರ್ಗ);

	ext2fs_free_mem(&ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Go to the node at leaf_level which contains logical block blk.
 *
 * leaf_level is height from the leaf node level, i.e.
 * leaf_level 0 is at leaf node, leaf_level 1 is 1 above etc.
 *
 * If "blk" has no mapping (hole) then handle is left at last
 * extent before blk.
 */
errcode_t ext2fs_extent_goto2(ext2_extent_handle_t handle,
			      ಪೂರ್ಣಾಂಕ ಎಲೆ_level, blk64_t blk)
{
	ರಚನೆ ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ಬೇರು, &extent);
	ಆದರೆ (retval) {
		ಆದರೆ (retval == EXT2_ET_EXTENT_NO_ಮುಂದಿನ)
			retval = EXT2_ET_EXTENT_NOT_ಸಿಕ್ಕಿತು;
		ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ (ಎಲೆ_level > handle->max_ಆಳ) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("leaf level %d greater than tree depth %d\n",
			ಎಲೆ_level, handle->max_ಆಳ);
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	ಬರೆ("goto extent ino %u, level %d, %llu\n", handle->ino,
	       ಎಲೆ_level, blk);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#ಆದರೆ_ಅಂತ್ಯ
	ಆಗ (1) {
		ಆದರೆ (handle->max_ಆಳ - handle->level == ಎಲೆ_level) {
			/* block is in this &extent */
			ಆದರೆ ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				ಹಿಂತಿರುಗಿ 0;
			ಆದರೆ (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NOT_ಸಿಕ್ಕಿತು;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ಮುಂದಿನ_SIB,
						   &extent);
			ಆದರೆ (retval == EXT2_ET_EXTENT_NO_ಮುಂದಿನ)
				ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_NOT_ಸಿಕ್ಕಿತು;
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
			ಮುಂದುವರಿ;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ಮುಂದಿನ_SIB,
					   &extent);
		ಆದರೆ (retval == EXT2_ET_EXTENT_NO_ಮುಂದಿನ)
			ಹೋಗು go_ಕೆಳಕ್ಕೆ;
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ (blk == extent.e_lblk)
			ಹೋಗು go_ಕೆಳಕ್ಕೆ;
		ಆದರೆ (blk > extent.e_lblk)
			ಮುಂದುವರಿ;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#ಆದರೆ_ಅಂತ್ಯ

	go_ಕೆಳಕ್ಕೆ:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ಕೆಳಕ್ಕೆ,
					   &extent);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#ಆದರೆ_ಅಂತ್ಯ
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	ಹಿಂತಿರುಗಿ ext2fs_extent_goto2(handle, 0, blk);
}

/*
 * Traverse back up to root fixing parents of current node as needed.
 *
 * If we changed start of first entry in a node, fix parent index start
 * and so on.
 *
 * Safe to call for any position in node; if not at the first entry,
 * it will simply return.
 *
 * Note a subtlety of this function -- if there happen to be two extents
 * mapping the same lblk and someone calls fix_parents on the second of the two
 * extents, the position of the extent handle after the call will be the second
 * extent if nothing happened, or the first extent if something did.  A caller
 * in this situation must use ext2fs_extent_goto() after calling this function.
 * Or simply don't map the same lblk with two extents, ever.
 */
errcode_t ext2fs_extent_fix_parents(ext2_extent_handle_t handle)
{
	ಪೂರ್ಣಾಂಕ				retval = 0;
	ಪೂರ್ಣಾಂಕ				orig_ಎತ್ತರ;
	blk64_t				ಆರಂಭ;
	ರಚನೆ extent_ಮಾರ್ಗ		*ಮಾರ್ಗ;
	ರಚನೆ ext2fs_extent		extent;
	ರಚನೆ ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ಆದರೆ (!(handle->fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	ಆದರೆ (!handle->ಮಾರ್ಗ)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

	ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;
	ಆದರೆ (!ಮಾರ್ಗ->curr)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ಆದರೆ (retval)
		ಹೋಗು ಮುಗಿದ;

	/* modified node's start block */
	ಆರಂಭ = extent.e_lblk;

	ಆದರೆ ((retval = ext2fs_extent_get_info(handle, &info)))
		ಹಿಂತಿರುಗಿ retval;
	orig_ಎತ್ತರ = info.max_ಆಳ - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	ಆಗ (handle->level > 0 &&
	       (ಮಾರ್ಗ->ಎಡ == ಮಾರ್ಗ->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ಮೇಲೆ, &extent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
		ಆದರೆ (extent.e_lblk == ಆರಂಭ)
			ನಿಲ್ಲಿಸು;
		ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;
		extent.e_len += (extent.e_lblk - ಆರಂಭ);
		extent.e_lblk = ಆರಂಭ;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
		update_ಮಾರ್ಗ(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_ಎತ್ತರ, ಆರಂಭ);
ಮುಗಿದ:
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)),
				ರಚನೆ ext2fs_extent *extent)
{
	ರಚನೆ extent_ಮಾರ್ಗ		*ಮಾರ್ಗ;
	ರಚನೆ ext3_extent_idx		*ix;
	ರಚನೆ ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ಆದರೆ (!(handle->fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	ಆದರೆ (!handle->ಮಾರ್ಗ)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

	ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;
	ಆದರೆ (!ಮಾರ್ಗ->curr)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	ಬರೆ("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (handle->level == handle->max_ಆಳ) {
		ex = ಮಾರ್ಗ->curr;

		ex->ee_ಖಂಡ = ext2fs_cpu_ಗಮ್ಯ_le32(extent->e_lblk);
		ex->ee_ಆರಂಭ = ext2fs_cpu_ಗಮ್ಯ_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_ಆರಂಭ_hi = ext2fs_cpu_ಗಮ್ಯ_le16(extent->e_pblk >> 32);
		ಆದರೆ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			ಆದರೆ (extent->e_len > EXT_UNINIT_MAX_LEN)
				ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_INVALID_ಉದ್ದಳತೆ;
			ex->ee_len = ext2fs_cpu_ಗಮ್ಯ_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (extent->e_len > EXT_INIT_MAX_LEN)
				ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_INVALID_ಉದ್ದಳತೆ;
			ex->ee_len = ext2fs_cpu_ಗಮ್ಯ_le16(extent->e_len);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ix = ಮಾರ್ಗ->curr;

		ix->ei_ಎಲೆ = ext2fs_cpu_ಗಮ್ಯ_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_ಎಲೆ_hi = ext2fs_cpu_ಗಮ್ಯ_le16(extent->e_pblk >> 32);
		ix->ei_ಖಂಡ = ext2fs_cpu_ಗಮ್ಯ_le32(extent->e_lblk);
		ix->ei_ಬಳಸದ = 0;
	}
	update_ಮಾರ್ಗ(handle);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ splitting_at_eof(ರಚನೆ ext2_extent_handle *handle,
			    ರಚನೆ extent_ಮಾರ್ಗ *ಮಾರ್ಗ)
{
	ರಚನೆ extent_ಮಾರ್ಗ *ppp = ಮಾರ್ಗ;
	dump_ಮಾರ್ಗ(__func__, handle, ಮಾರ್ಗ);

	ಆದರೆ (handle->level == 0)
		ಹಿಂತಿರುಗಿ 0;

	ಮಾಡು {
		ಆದರೆ (ppp->ಎಡ)
			ಹಿಂತಿರುಗಿ 0;
		ppp--;
	} ಆಗ (ppp >= handle->ಮಾರ್ಗ);

	ಹಿಂತಿರುಗಿ 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
ಸ್ಥಿರ errcode_t extent_ಗಂಟು_ವಿಭಜಿಸು(ext2_extent_handle_t handle,
				   ಪೂರ್ಣಾಂಕ expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_ಗಂಟು_pblk;
	blk64_t				new_ಗಂಟು_ಆರಂಭ;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	ಪೂರ್ಣಾಂಕ				orig_ಎತ್ತರ;
	ಅಕ್ಷರ				*ಖಂಡ_buf = ಖಾಲಿ;
	ರಚನೆ ext2fs_extent		extent;
	ರಚನೆ extent_ಮಾರ್ಗ		*ಮಾರ್ಗ, *newpath = 0;
	ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ	*eh, *neweh;
	ಪೂರ್ಣಾಂಕ				tocopy;
	ಪೂರ್ಣಾಂಕ				new_ಬೇರು = 0;
	ರಚನೆ ext2_extent_info		info;
	ಪೂರ್ಣಾಂಕ				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ಆದರೆ (!(handle->fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	ಆದರೆ (!handle->ಮಾರ್ಗ)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	ಬರೆ("splitting node at level %d\n", handle->level);
#ಆದರೆ_ಅಂತ್ಯ
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ಆದರೆ (retval)
		ಹೋಗು ಮುಗಿದ;

	retval = ext2fs_extent_get_info(handle, &info);
	ಆದರೆ (retval)
		ಹೋಗು ಮುಗಿದ;

	/* save the position we were originally splitting... */
	orig_ಎತ್ತರ = info.max_ಆಳ - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;
	eh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) ಮಾರ್ಗ->buf;
	ಆದರೆ (handle->level == handle->max_ಆಳ) {
		ರಚನೆ ext3_extent	*ex;

		ex = EXT_ಮೊದಲ_EXTENT(eh);
		goal_blk = ext2fs_le32_ಗಮ್ಯ_cpu(ex->ee_ಆರಂಭ) +
			((__u64) ext2fs_le16_ಗಮ್ಯ_cpu(ex->ee_ಆರಂಭ_hi) << 32);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ ext3_extent_idx	*ix;

		ix = EXT_ಮೊದಲ_ಸೂಚ್ಯಂಕ(eh);
		goal_blk = ext2fs_le32_ಗಮ್ಯ_cpu(ix->ei_ಎಲೆ) +
			((__u64) ext2fs_le16_ಗಮ್ಯ_cpu(ix->ei_ಎಲೆ_hi) << 32);
	}
	goal_blk -= EXT2_FS_CLUSTER_ಅನುಪಾತ(handle->fs);
	goal_blk &= ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(handle->fs);

	/* Is there room in the parent for a new entry? */
	ಆದರೆ (handle->level &&
			(handle->ಮಾರ್ಗ[handle->level - 1].entries >=
			 handle->ಮಾರ್ಗ[handle->level - 1].max_entries)) {

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("parent level %d full; splitting it too\n",
							handle->level - 1);
#ಆದರೆ_ಅಂತ್ಯ
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ಮೇಲೆ, &extent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;

		retval = extent_ಗಂಟು_ವಿಭಜಿಸು(handle, expand_allowed);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_ಎತ್ತರ, orig_lblk);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
	}

	/* At this point, parent should have room for this split */
	ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;
	ಆದರೆ (!ಮಾರ್ಗ->curr)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, ಮಾರ್ಗ) : 0;

	/* extent header of the current node we'll split */
	eh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *)ಮಾರ್ಗ->buf;

	/* splitting root level means moving them all out */
	ಆದರೆ (handle->level == 0) {
		new_ಬೇರು = 1;
		tocopy = ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    ಗಾತ್ರ(ರಚನೆ extent_ಮಾರ್ಗ),
					    &newpath);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (no_balance)
			tocopy = 1;
		ಇಲ್ಲದಿದ್ದರೆ
			tocopy = ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_entries) / 2;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	ಬರೆ("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_entries),
				handle->level);
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (!tocopy && !no_balance) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("Nothing to copy to new block!\n");
#ಆದರೆ_ಅಂತ್ಯ
		retval = EXT2_ET_CANT_ವಿಭಜಿಸು_EXTENT;
		ಹೋಗು ಮುಗಿದ;
	}

	/* first we need a new block, or can do nothing. */
	ಖಂಡ_buf = ಸ್ಮರಣೆ_ಕೊಡು(handle->fs->blocksize);
	ಆದರೆ (!ಖಂಡ_buf) {
		retval = ENOMEM;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (!goal_blk)
		goal_blk = ext2fs_ಕಂಡುಹಿಡಿ_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, ಖಂಡ_buf,
				    &new_ಗಂಟು_pblk);
	ಆದರೆ (retval)
		ಹೋಗು ಮುಗಿದ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	ಬರೆ("will copy to new node at block %lu\n",
	       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) new_ಗಂಟು_pblk);
#ಆದರೆ_ಅಂತ್ಯ

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) ಖಂಡ_buf;
	ಸ್ಮರಣೆ_ನಕಲು(neweh, eh, ಗಾತ್ರ(ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ));
	neweh->eh_entries = ext2fs_cpu_ಗಮ್ಯ_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_ಗಮ್ಯ_le16((handle->fs->blocksize -
			 ಗಾತ್ರ(ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ)) /
				ಗಾತ್ರ(ರಚನೆ ext3_extent));

	/* then the entries for the new block... */
	ಸ್ಮರಣೆ_ನಕಲು(EXT_ಮೊದಲ_ಸೂಚ್ಯಂಕ(neweh),
		EXT_ಮೊದಲ_ಸೂಚ್ಯಂಕ(eh) +
			(ext2fs_le16_ಗಮ್ಯ_cpu(eh->eh_entries) - tocopy),
		ಗಾತ್ರ(ರಚನೆ ext3_extent_idx) * tocopy);

	new_ಗಂಟು_ಆರಂಭ = ext2fs_le32_ಗಮ್ಯ_cpu(EXT_ಮೊದಲ_ಸೂಚ್ಯಂಕ(neweh)->ei_ಖಂಡ);

	/* then update the checksum */
	retval = ext2fs_extent_ಖಂಡ_csum_ಗಣ(handle->fs, handle->ino, neweh);
	ಆದರೆ (retval)
		ಹೋಗು ಮುಗಿದ;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_ಗಂಟು_pblk, 1,
					ಖಂಡ_buf);

	ಆದರೆ (retval)
		ಹೋಗು ಮುಗಿದ;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	ಆದರೆ (handle->level == 0) {
		ಸ್ಮರಣೆ_ನಕಲು(newpath, ಮಾರ್ಗ,
		       ಗಾತ್ರ(ರಚನೆ extent_ಮಾರ್ಗ) * handle->max_paths);
		handle->ಮಾರ್ಗ = newpath;
		newpath = ಮಾರ್ಗ;
		ಮಾರ್ಗ = handle->ಮಾರ್ಗ;
		ಮಾರ್ಗ->entries = 1;
		ಮಾರ್ಗ->ಎಡ = ಮಾರ್ಗ->max_entries - 1;
		handle->max_ಆಳ++;
		handle->max_paths++;
		eh->eh_ಆಳ = ext2fs_cpu_ಗಮ್ಯ_le16(handle->max_ಆಳ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಮಾರ್ಗ->entries -= tocopy;
		ಮಾರ್ಗ->ಎಡ -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_ಗಮ್ಯ_le16(ಮಾರ್ಗ->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_ಮಾರ್ಗ(handle);
	ಆದರೆ (retval)
		ಹೋಗು ಮುಗಿದ;

	/* now go up and insert/replace index for new node we created */
	ಆದರೆ (new_ಬೇರು) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ಮೊದಲ_SIB, &extent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;

		extent.e_lblk = new_ಗಂಟು_ಆರಂಭ;
		extent.e_pblk = new_ಗಂಟು_pblk;
		extent.e_len = handle->ಮಾರ್ಗ[0].ಕೊನೆ_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		__u32 new_ಗಂಟು_ಉದ್ದಳತೆ;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ಮೇಲೆ, &extent);
		/* will insert after this one; it's length is shorter now */
		new_ಗಂಟು_ಉದ್ದಳತೆ = new_ಗಂಟು_ಆರಂಭ - extent.e_lblk;
		extent.e_len -= new_ಗಂಟು_ಉದ್ದಳತೆ;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_ಗಂಟು_ಆರಂಭ;
		extent.e_pblk = new_ಗಂಟು_pblk;
		extent.e_len = new_ಗಂಟು_ಉದ್ದಳತೆ;
		retval = ext2fs_extent_ಸೇರಿಸು(handle, EXT2_EXTENT_ಸೇರಿಸು_AFTER, &extent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_ಎತ್ತರ, orig_lblk);
	ಆದರೆ (retval)
		ಹೋಗು ಮುಗಿದ;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	ಆದರೆ (retval)
		ಹೋಗು ಮುಗಿದ;

ಮುಗಿದ:
	ಆದರೆ (newpath)
		ext2fs_free_mem(&newpath);
	ಬಿಡುಗಡೆ(ಖಂಡ_buf);

	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_extent_ಗಂಟು_ವಿಭಜಿಸು(ext2_extent_handle_t handle)
{
	ಹಿಂತಿರುಗಿ extent_ಗಂಟು_ವಿಭಜಿಸು(handle, 0);
}

errcode_t ext2fs_extent_ಸೇರಿಸು(ext2_extent_handle_t handle, ಪೂರ್ಣಾಂಕ flags,
				      ರಚನೆ ext2fs_extent *extent)
{
	ರಚನೆ extent_ಮಾರ್ಗ		*ಮಾರ್ಗ;
	ರಚನೆ ext3_extent_idx		*ix;
	ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ಆದರೆ (!(handle->fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	ಆದರೆ (!handle->ಮಾರ್ಗ)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	ಬರೆ("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#ಆದರೆ_ಅಂತ್ಯ

	ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;

	ಆದರೆ (ಮಾರ್ಗ->entries >= ಮಾರ್ಗ->max_entries) {
		ಆದರೆ (flags & EXT2_EXTENT_ಸೇರಿಸು_NOSPLIT) {
			ಹಿಂತಿರುಗಿ EXT2_ET_CANT_ಸೇರಿಸು_EXTENT;
		} ಇಲ್ಲದಿದ್ದರೆ {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
			ಬರೆ("node full (level %d) - splitting\n",
				   handle->level);
#ಆದರೆ_ಅಂತ್ಯ
			retval = extent_ಗಂಟು_ವಿಭಜಿಸು(handle, 1);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
			ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;
		}
	}

	eh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) ಮಾರ್ಗ->buf;
	ಆದರೆ (ಮಾರ್ಗ->curr) {
		ix = ಮಾರ್ಗ->curr;
		ಆದರೆ (flags & EXT2_EXTENT_ಸೇರಿಸು_AFTER) {
			ix++;
			ಮಾರ್ಗ->ಎಡ--;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ix = EXT_ಮೊದಲ_ಸೂಚ್ಯಂಕ(eh);
		ಮಾರ್ಗ->ಎಡ = -1;
	}

	ಮಾರ್ಗ->curr = ix;

	ಆದರೆ (ಮಾರ್ಗ->ಎಡ >= 0)
		memmove(ix + 1, ix,
			(ಮಾರ್ಗ->ಎಡ+1) * ಗಾತ್ರ(ರಚನೆ ext3_extent_idx));
	ಮಾರ್ಗ->ಎಡ++;
	ಮಾರ್ಗ->entries++;

	eh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) ಮಾರ್ಗ->buf;
	eh->eh_entries = ext2fs_cpu_ಗಮ್ಯ_le16(ಮಾರ್ಗ->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	ಆದರೆ (retval)
		ಹೋಗು errout;

	retval = update_ಮಾರ್ಗ(handle);
	ಆದರೆ (retval)
		ಹೋಗು errout;

	ಹಿಂತಿರುಗಿ 0;

errout:
	ext2fs_extent_delete(handle, 0);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Sets the physical block for a logical file block in the extent tree.
 *
 * May: map unmapped, unmap mapped, or remap mapped blocks.
 *
 * Mapping an unmapped block adds a single-block extent.
 *
 * Unmapping first or last block modifies extent in-place
 *  - But may need to fix parent's starts too in first-block case
 *
 * Mapping any unmapped block requires adding a (single-block) extent
 * and inserting into proper point in tree.
 *
 * Modifying (unmapping or remapping) a block in the middle
 * of an extent requires splitting the extent.
 *  - Remapping case requires new single-block extent.
 *
 * Remapping first or last block adds an extent.
 *
 * We really need extent adding to be smart about merging.
 */

errcode_t ext2fs_extent_ಗಣ_bmap(ext2_extent_handle_t handle,
				 blk64_t logical, blk64_t physical, ಪೂರ್ಣಾಂಕ flags)
{
	errcode_t		ec, retval = 0;
	ಪೂರ್ಣಾಂಕ			mapped = 1; /* logical is mapped? */
	ಪೂರ್ಣಾಂಕ			orig_ಎತ್ತರ;
	ಪೂರ್ಣಾಂಕ			extent_uninit = 0;
	ಪೂರ್ಣಾಂಕ			prev_uninit = 0;
	ಪೂರ್ಣಾಂಕ			ಮುಂದಿನ_uninit = 0;
	ಪೂರ್ಣಾಂಕ			new_uninit = 0;
	ಪೂರ್ಣಾಂಕ			max_len = EXT_INIT_MAX_LEN;
	ಪೂರ್ಣಾಂಕ			has_prev, has_ಮುಂದಿನ;
	blk64_t			orig_lblk;
	ರಚನೆ extent_ಮಾರ್ಗ	*ಮಾರ್ಗ;
	ರಚನೆ ext2fs_extent	extent, ಮುಂದಿನ_extent, prev_extent;
	ರಚನೆ ext2fs_extent	newextent;
	ರಚನೆ ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	ಬರೆ("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (!(handle->fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	ಆದರೆ (!handle->ಮಾರ್ಗ)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

	ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;

	ಆದರೆ (flags & EXT2_EXTENT_ಗಣ_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	ಆದರೆ (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_ಎಲೆ;
		ಆದರೆ (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	ಆದರೆ ((handle->max_ಆಳ == 0) && (ಮಾರ್ಗ->entries == 0)) {
		retval = ext2fs_extent_ಸೇರಿಸು(handle, 0, &newextent);
		ಹಿಂತಿರುಗಿ retval;
	}

	/* save our original location in the extent tree */
	ಆದರೆ ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		ಆದರೆ (retval != EXT2_ET_NO_CURRENT_ಗಂಟು)
			ಹಿಂತಿರುಗಿ retval;
		memset(&extent, 0, ಗಾತ್ರ(extent));
	}
	ಆದರೆ ((retval = ext2fs_extent_get_info(handle, &info)))
		ಹಿಂತಿರುಗಿ retval;
	orig_ಎತ್ತರ = info.max_ಆಳ - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	ಆದರೆ (retval) {
		ಆದರೆ (retval == EXT2_ET_EXTENT_NOT_ಸಿಕ್ಕಿತು) {
			retval = 0;
			mapped = 0;
			ಆದರೆ (!physical) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
				ಬರೆ("block %llu already unmapped\n",
					logical);
#ಆದರೆ_ಅಂತ್ಯ
				ಹೋಗು ಮುಗಿದ;
			}
		} ಇಲ್ಲದಿದ್ದರೆ
			ಹೋಗು ಮುಗಿದ;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ಆದರೆ (retval)
		ಹೋಗು ಮುಗಿದ;
	ಆದರೆ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ, &ಮುಂದಿನ_extent);
	ಆದರೆ (retval) {
		has_ಮುಂದಿನ = 0;
		ಆದರೆ (retval != EXT2_ET_EXTENT_NO_ಮುಂದಿನ)
			ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		dbg_print_extent("set_bmap: next_extent",
				 &ಮುಂದಿನ_extent);
		has_ಮುಂದಿನ = 1;
		ಆದರೆ (ಮುಂದಿನ_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			ಮುಂದಿನ_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ಆದರೆ (retval && retval != EXT2_ET_EXTENT_NOT_ಸಿಕ್ಕಿತು)
		ಹೋಗು ಮುಗಿದ;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_ಎಲೆ, &prev_extent);
	ಆದರೆ (retval) {
		has_prev = 0;
		ಆದರೆ (retval != EXT2_ET_EXTENT_NO_PREV)
			ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		ಆದರೆ (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ಆದರೆ (retval && retval != EXT2_ET_EXTENT_NOT_ಸಿಕ್ಕಿತು)
		ಹೋಗು ಮುಗಿದ;

	/* check if already pointing to the requested physical */
	ಆದರೆ (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("physical block (at %llu) unchanged\n", logical);
#ಆದರೆ_ಅಂತ್ಯ
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (!mapped) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("mapping unmapped logical block %llu\n", logical);
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((ಪೂರ್ಣಾಂಕ) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((ಪೂರ್ಣಾಂಕ) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_ಮುಂದಿನ &&
			   (logical == ಮುಂದಿನ_extent.e_lblk - 1) &&
			   (physical == ಮುಂದಿನ_extent.e_pblk - 1) &&
			   (new_uninit == ಮುಂದಿನ_uninit) &&
			   ((ಪೂರ್ಣಾಂಕ) ಮುಂದಿನ_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ,
						   &ಮುಂದಿನ_extent);
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
			ಮುಂದಿನ_extent.e_len++;
			ಮುಂದಿನ_extent.e_lblk--;
			ಮುಂದಿನ_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &ಮುಂದಿನ_extent);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (logical < extent.e_lblk)
			retval = ext2fs_extent_ಸೇರಿಸು(handle, 0, &newextent);
		ಇಲ್ಲದಿದ್ದರೆ
			retval = ext2fs_extent_ಸೇರಿಸು(handle,
				      EXT2_EXTENT_ಸೇರಿಸು_AFTER, &newextent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
		retval = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("(re/un)mapping only block in extent\n");
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ (physical) {
			ಆದರೆ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((ಪೂರ್ಣಾಂಕ) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_ಎಲೆ, &prev_extent);
				ಆದರೆ (retval)
					ಹೋಗು ಮುಗಿದ;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ,
							   &extent);
				ಆದರೆ (retval)
					ಹೋಗು ಮುಗಿದ;
				ಹೋಗು delete_ಗಂಟು;

			} ಇಲ್ಲದಿದ್ದರೆ
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} ಇಲ್ಲದಿದ್ದರೆ {
		delete_ಗಂಟು:
			retval = ext2fs_extent_delete(handle, 0);
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
			ec = ext2fs_extent_fix_parents(handle);
			ಆದರೆ (ec != EXT2_ET_NO_CURRENT_ಗಂಟು)
				retval = ec;
		}

		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (logical == extent.e_lblk + extent.e_len - 1)  {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("(re/un)mapping last block in extent\n");
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ (physical) {
			ಆದರೆ (has_ಮುಂದಿನ &&
			    (logical == (ಮುಂದಿನ_extent.e_lblk - 1)) &&
			    (physical == (ಮುಂದಿನ_extent.e_pblk - 1)) &&
			    (new_uninit == ಮುಂದಿನ_uninit) &&
			    ((ಪೂರ್ಣಾಂಕ) ಮುಂದಿನ_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ, &ಮುಂದಿನ_extent);
				ಆದರೆ (retval)
					ಹೋಗು ಮುಗಿದ;
				ಮುಂದಿನ_extent.e_len++;
				ಮುಂದಿನ_extent.e_lblk--;
				ಮುಂದಿನ_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &ಮುಂದಿನ_extent);
				ಆದರೆ (retval)
					ಹೋಗು ಮುಗಿದ;
			} ಇಲ್ಲದಿದ್ದರೆ
				retval = ext2fs_extent_ಸೇರಿಸು(handle,
				      EXT2_EXTENT_ಸೇರಿಸು_AFTER, &newextent);
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
			retval = ext2fs_extent_fix_parents(handle);
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (logical == extent.e_lblk) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("(re/un)mapping first block in extent\n");
#ಆದರೆ_ಅಂತ್ಯ
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
		retval = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
		ಆದರೆ (physical) {
			ಆದರೆ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((ಪೂರ್ಣಾಂಕ) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_ಎಲೆ, &prev_extent);
				ಆದರೆ (retval)
					ಹೋಗು ಮುಗಿದ;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} ಇಲ್ಲದಿದ್ದರೆ
				retval = ext2fs_extent_ಸೇರಿಸು(handle,
							      0, &newextent);
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
			retval = ext2fs_extent_fix_parents(handle);
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ,
						   &extent);
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		__u32	save_ಉದ್ದಳತೆ;
		blk64_t	save_lblk;
		ರಚನೆ ext2fs_extent save_extent;
		errcode_t r2;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
		ಬರೆ("(re/un)mapping in middle of extent\n");
#ಆದರೆ_ಅಂತ್ಯ
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_ಉದ್ದಳತೆ = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
		/* insert our new extent, if any */
		ಆದರೆ (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_ಸೇರಿಸು(handle,
					EXT2_EXTENT_ಸೇರಿಸು_AFTER, &newextent);
			ಆದರೆ (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				ಆದರೆ (r2 == 0)
					(ಶೂನ್ಯ)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				ಹೋಗು ಮುಗಿದ;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_ಉದ್ದಳತೆ - extent.e_len - 1;
		retval = ext2fs_extent_ಸೇರಿಸು(handle,
				EXT2_EXTENT_ಸೇರಿಸು_AFTER, &extent);
		ಆದರೆ (retval) {
			ಆದರೆ (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				ಆದರೆ (r2 == 0)
					(ಶೂನ್ಯ)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			ಆದರೆ (r2 == 0)
				(ಶೂನ್ಯ)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			ಹೋಗು ಮುಗಿದ;
		}
	}

ಮುಗಿದ:
	/* get handle back to its position */
	ಆದರೆ (orig_ಎತ್ತರ > handle->max_ಆಳ)
		orig_ಎತ್ತರ = handle->max_ಆಳ; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_ಎತ್ತರ, orig_lblk);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ extent_ಮಾರ್ಗ		*ಮಾರ್ಗ;
	ಅಕ್ಷರ 				*cp;
	ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ಆದರೆ (!(handle->fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	ಆದರೆ (!handle->ಮಾರ್ಗ)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	{
		ರಚನೆ ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		ಆದರೆ (retval == 0) {
			ಬರೆ("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#ಆದರೆ_ಅಂತ್ಯ

	ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;
	ಆದರೆ (!ಮಾರ್ಗ->curr)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_CURRENT_ಗಂಟು;

	cp = ಮಾರ್ಗ->curr;

	/* Sanity check before memmove() */
	ಆದರೆ (ಮಾರ್ಗ->ಎಡ < 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTENT_ಎಲೆ_BAD;

	ಆದರೆ (ಮಾರ್ಗ->ಎಡ) {
		memmove(cp, cp + ಗಾತ್ರ(ರಚನೆ ext3_extent_idx),
			ಮಾರ್ಗ->ಎಡ * ಗಾತ್ರ(ರಚನೆ ext3_extent_idx));
		ಮಾರ್ಗ->ಎಡ--;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ ext3_extent_idx	*ix = ಮಾರ್ಗ->curr;
		ix--;
		ಮಾರ್ಗ->curr = ix;
	}
	ಆದರೆ (--ಮಾರ್ಗ->entries == 0)
		ಮಾರ್ಗ->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	ಆದರೆ (ಮಾರ್ಗ->entries == 0 && handle->level) {
		ಆದರೆ (!(flags & EXT2_EXTENT_DELETE_KEEP_ಖಾಲಿಗಳು)) {
			ರಚನೆ ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_ಮೇಲೆ,
								&extent);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_ಖಂಡಗಳು -=
				(handle->fs->blocksize *
				 EXT2_FS_CLUSTER_ಅನುಪಾತ(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_ಖಂಡ_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		eh = (ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ *) ಮಾರ್ಗ->buf;
		eh->eh_entries = ext2fs_cpu_ಗಮ್ಯ_le16(ಮಾರ್ಗ->entries);
		ಆದರೆ ((ಮಾರ್ಗ->entries == 0) && (handle->level == 0)) {
			eh->eh_ಆಳ = 0;
			handle->max_ಆಳ = 0;
		}
		retval = update_ಮಾರ್ಗ(handle);
	}
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 ರಚನೆ ext2_extent_info *info)
{
	ರಚನೆ extent_ಮಾರ್ಗ		*ಮಾರ್ಗ;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, ಗಾತ್ರ(ರಚನೆ ext2_extent_info));

	ಮಾರ್ಗ = handle->ಮಾರ್ಗ + handle->level;
	ಆದರೆ (ಮಾರ್ಗ) {
		ಆದರೆ (ಮಾರ್ಗ->curr)
			info->curr_ನಮೂದು = ((ಅಕ್ಷರ *) ಮಾರ್ಗ->curr - ಮಾರ್ಗ->buf) /
				ಗಾತ್ರ(ರಚನೆ ext3_extent_idx);
		ಇಲ್ಲದಿದ್ದರೆ
			info->curr_ನಮೂದು = 0;
		info->ಅಂಕೆ_entries = ಮಾರ್ಗ->entries;
		info->max_entries = ಮಾರ್ಗ->max_entries;
		info->ಬೈಟುಗಳು_avail = (ಮಾರ್ಗ->max_entries - ಮಾರ್ಗ->entries) *
			ಗಾತ್ರ(ರಚನೆ ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_ಆಳ = handle->max_ಆಳ;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	ಹಿಂತಿರುಗಿ 0;
}

size_t ext2fs_max_extent_ಆಳ(ext2_extent_handle_t handle)
{
	size_t iblock_sz = ಗಾತ್ರ(((ರಚನೆ ext2_inode *)ಖಾಲಿ)->i_ಖಂಡ);
	size_t iblock_extents = (iblock_sz - ಗಾತ್ರ(ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ)) /
				ಗಾತ್ರ(ರಚನೆ ext3_extent);
	size_t extents_ಪ್ರತಿ_ಖಂಡ = (handle->fs->blocksize -
				    ಗಾತ್ರ(ರಚನೆ ext3_extent_ಶೀರ್ಷಿಕೆ)) /
				   ಗಾತ್ರ(ರಚನೆ ext3_extent);
	ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_blocksize = 0;
	ಸ್ಥಿರ size_t ಕೊನೆಯ_ಫಲಿತಾಂಶ = 0;

	ಆದರೆ (ಕೊನೆಯ_blocksize && ಕೊನೆಯ_blocksize == handle->fs->blocksize)
		ಹಿಂತಿರುಗಿ ಕೊನೆಯ_ಫಲಿತಾಂಶ;

	ಕೊನೆಯ_ಫಲಿತಾಂಶ = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_ಪ್ರತಿ_ಖಂಡ));
	ಕೊನೆಯ_blocksize = handle->fs->blocksize;
	ಹಿಂತಿರುಗಿ ಕೊನೆಯ_ಫಲಿತಾಂಶ;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       ರಚನೆ ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	ರಚನೆ ext2fs_extent	extent;
	errcode_t		errcode;
	ಪೂರ್ಣಾಂಕ    			save_flags = fs->flags;

	ಆದರೆ (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		ಹಿಂತಿರುಗಿ 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ಆದರೆ (errcode) {
		ಆದರೆ (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		ಹಿಂತಿರುಗಿ errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ಬೇರು, &extent);
	ಆದರೆ (errcode)
		ಹೋಗು ಹೊರಹೋಗು;

	ಮಾಡು {
		/* Skip to the end of a block of leaf nodes */
		ಆದರೆ (extent.e_flags & EXT2_EXTENT_FLAGS_ಎಲೆ) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_ಕೊನೆಯ_SIB,
						    &extent);
			ಆದರೆ (errcode)
				ನಿಲ್ಲಿಸು;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ಮುಂದಿನ, &extent);
		ಆದರೆ (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_ಮಾರ್ಗ(handle);
	} ಆಗ (errcode == 0);

ಹೊರಹೋಗು:
	/* Ok if we run off the end */
	ಆದರೆ (errcode == EXT2_ET_EXTENT_NO_ಮುಂದಿನ)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	ಹಿಂತಿರುಗಿ errcode;
}

errcode_t ext2fs_decode_extent(ರಚನೆ ext2fs_extent *ಗಮ್ಯ, ಶೂನ್ಯ *addr, ಪೂರ್ಣಾಂಕ len)
{
	ರಚನೆ ext3_extent *from = (ರಚನೆ ext3_extent *)addr;

	ಆದರೆ (len != ಗಾತ್ರ(ರಚನೆ ext3_extent))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಗಮ್ಯ->e_pblk = ext2fs_le32_ಗಮ್ಯ_cpu(from->ee_ಆರಂಭ) +
		((__u64) ext2fs_le16_ಗಮ್ಯ_cpu(from->ee_ಆರಂಭ_hi)
			<< 32);
	ಗಮ್ಯ->e_lblk = ext2fs_le32_ಗಮ್ಯ_cpu(from->ee_ಖಂಡ);
	ಗಮ್ಯ->e_len = ext2fs_le16_ಗಮ್ಯ_cpu(from->ee_len);
	ಗಮ್ಯ->e_flags = EXT2_EXTENT_FLAGS_ಎಲೆ;
	ಆದರೆ (ಗಮ್ಯ->e_len > EXT_INIT_MAX_LEN) {
		ಗಮ್ಯ->e_len -= EXT_INIT_MAX_LEN;
		ಗಮ್ಯ->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_ಎಣಿಕೆ_ಖಂಡಗಳು(ext2_filsys fs, ext2_ino_t ino,
			      ರಚನೆ ext2_inode *inode, blk64_t *ret_ಎಣಿಕೆ)
{
	ext2_extent_handle_t	handle = ಖಾಲಿ;
	ರಚನೆ ext2fs_extent	extent;
	errcode_t		errcode;
	ಪೂರ್ಣಾಂಕ			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ಆದರೆ (errcode)
		ಹೋಗು ಹೊರಹೋಗು;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ಬೇರು, &extent);
	ಆದರೆ (errcode)
		ಹೋಗು ಹೊರಹೋಗು;

	errcode = ext2fs_get_array(handle->max_ಆಳ, ಗಾತ್ರ(blk64_t),
				   &intermediate_nodes);
	ಆದರೆ (errcode)
		ಹೋಗು ಹೊರಹೋಗು;

	blkcount = handle->level;
	ಆಗ (!errcode) {
		ಆದರೆ (extent.e_flags & EXT2_EXTENT_FLAGS_ಎಲೆ) {
			blkcount += extent.e_len;
			ಗಾಗಿ (i = 0; i < handle->level; i++) {
				ಆದರೆ (intermediate_nodes[i] !=
					handle->ಮಾರ್ಗ[i].ಕೊನೆ_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->ಮಾರ್ಗ[i].ಕೊನೆ_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ಮುಂದಿನ, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
ಹೊರಹೋಗು:
	*ret_ಎಣಿಕೆ = blkcount;
	ext2fs_extent_free(handle);

	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
/*
 * Override debugfs's prompt
 */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *debug_prog_name = "tst_extents";

#ಆದರೆ_ಅಂತ್ಯ

