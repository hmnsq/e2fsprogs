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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᯀᯮᯝ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ᯀᯮᯝ HAVE_ERRNO_H
#include <errno.h>
#endif
#ᯀᯮᯝ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᯀᯮᯝ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"
#include "e2image.h"

#undef DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

sa_nracanaa extent_path {
	var_na		*buf;
	puur_naa_nka		entries;
	puur_naa_nka		max_entries;
	puur_naa_nka		left;
	puur_naa_nka		visit_num;
	puur_naa_nka		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	void		*curr;
};


sa_nracanaa ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	sa_nracanaa ext2_inode	*inode;
	sa_nracanaa ext2_inode	inodebuf;
	puur_naa_nka			type;
	puur_naa_nka			level;
	puur_naa_nka			max_depth;
	puur_naa_nka			max_paths;
	sa_nracanaa extent_path	*path;
};

sa_nracanaa ext2_extent_path {
	errcode_t		magic;
	puur_naa_nka			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
static void dbg_show_header(sa_nracanaa ext3_extent_header *eh)
{
	printf("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

static void dbg_show_index(sa_nracanaa ext3_extent_idx *ix)
{
	printf("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

static void dbg_show_extent(sa_nracanaa ext3_extent *ex)
{
	printf("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

static void dbg_print_extent(var_na *desc, sa_nracanaa ext2fs_extent *extent)
{
	ᯀᯮᯝ (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ᯀᯮᯝ (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	ᯀᯮᯝ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ᯀᯮᯝ (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	ᯀᯮᯝ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

static void dump_path(const var_na *tag, sa_nracanaa ext2_extent_handle *handle,
		      sa_nracanaa extent_path *path)
{
	sa_nracanaa extent_path *ppp = path;
	printf("%s: level=%d\n", tag, handle->level);

	do {
		printf("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->left, ppp->visit_num, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (void *)ppp->buf);
		printf("  ");
		dbg_show_header((sa_nracanaa ext3_extent_header *)ppp->buf);
		ᯀᯮᯝ (ppp->curr) {
			printf("  ");
			dbg_show_index(ppp->curr);
			printf("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} ᯀᯬᯝ (ppp >= handle->path);
	fflush(stdout);

	ᯀᯮᯞ;
}

#ᯀᯮᯝᯂᯪ
#define dbg_show_header(eh) do { } ᯀᯬᯝ (0)
#define dbg_show_index(ix) do { } ᯀᯬᯝ (0)
#define dbg_show_extent(ex) do { } ᯀᯬᯝ (0)
#define dbg_print_extent(desc, ex) do { } ᯀᯬᯝ (0)
#define dump_path(tag, handle, path) do { } ᯀᯬᯝ (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(void *ptr, puur_naa_nka size)
{
	puur_naa_nka eh_max, entry_size;
	sa_nracanaa ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	ᯀᯮᯝ (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		ᯀᯮᯞ EXT2_ET_EXTENT_HEADER_BAD;
	ᯀᯮᯝ (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		ᯀᯮᯞ EXT2_ET_EXTENT_HEADER_BAD;
	ᯀᯮᯝ (eh->eh_depth == 0)
		entry_size = parimaa_na(sa_nracanaa ext3_extent);
	ᯀᯮᯝᯂᯪ
		entry_size = parimaa_na(sa_nracanaa ext3_extent_idx);

	eh_max = (size - parimaa_na(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	ᯀᯮᯝ ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		ᯀᯮᯞ EXT2_ET_EXTENT_HEADER_BAD;

	ᯀᯮᯞ 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
void ext2fs_extent_free(ext2_extent_handle_t handle)
{
	puur_naa_nka			i;

	ᯀᯮᯝ (!handle)
		ᯀᯮᯞ;

	ᯀᯮᯝ (handle->path) {
		for (i = 1; i < handle->max_paths; i++) {
			ᯀᯮᯝ (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	ᯀᯮᯞ ext2fs_extent_open2(fs, ino, NULL, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    sa_nracanaa ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	sa_nracanaa ext2_extent_handle	*handle;
	errcode_t			retval;
	puur_naa_nka				i;
	sa_nracanaa ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ᯀᯮᯝ (!inode)
		ᯀᯮᯝ ((ino == 0) || (ino > fs->super->s_inodes_count))
			ᯀᯮᯞ EXT2_ET_BAD_INODE_NUM;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_extent_handle), &handle);
	ᯀᯮᯝ (retval)
		ᯀᯮᯞ retval;
	memset(handle, 0, parimaa_na(sa_nracanaa ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	ᯀᯮᯝ (inode) {
		handle->inode = inode;
	} ᯀᯮᯝᯂᯪ {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		ᯀᯮᯝ (retval)
			goto errout;
	}

	eh = (sa_nracanaa ext3_extent_header *) &handle->inode->i_block[0];

	for (i=0; i < EXT2_N_BLOCKS; i++)
		ᯀᯮᯝ (handle->inode->i_block[i])
			bha_nga;
	ᯀᯮᯝ (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (parimaa_na(handle->inode->i_block) - parimaa_na(*eh)) /
			parimaa_na(sa_nracanaa ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	ᯀᯮᯝ (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		goto errout;
	}

	retval = ext2fs_extent_header_verify(eh, parimaa_na(handle->inode->i_block));
	ᯀᯮᯝ (retval)
		goto errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    parimaa_na(sa_nracanaa extent_path),
				    &handle->path);
	handle->path[0].buf = (var_na *) handle->inode->i_block;

	handle->path[0].left = handle->path[0].entries =
		ext2fs_le16_to_cpu(eh->eh_entries);
	handle->path[0].max_entries = ext2fs_le16_to_cpu(eh->eh_max);
	handle->path[0].curr = 0;
	handle->path[0].end_blk =
		(EXT2_I_SIZE(handle->inode) + fs->blocksize - 1) >>
		 EXT2_BLOCK_SIZE_BITS(fs->super);
	handle->path[0].blk = 0;
	handle->path[0].visit_num = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	ᯀᯮᯞ 0;

errout:
	ext2fs_extent_free(handle);
	ᯀᯮᯞ retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    puur_naa_nka flags, sa_nracanaa ext2fs_extent *extent)
{
	sa_nracanaa extent_path	*path, *newpath, *tp;
	sa_nracanaa ext3_extent_header	*eh;
	sa_nracanaa ext3_extent_idx		*ix = 0;
	sa_nracanaa ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	puur_naa_nka				orig_op, op, l;
	puur_naa_nka				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ᯀᯮᯝ (!handle->path)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	ᯀᯮᯝ ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		ᯀᯮᯝ (handle->level < handle->max_depth) {
			/* interior node */
			ᯀᯮᯝ (path->visit_num == 0) {
				path->visit_num++;
				op = EXT2_EXTENT_DOWN;
			} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (path->left > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			ᯀᯮᯝᯂᯪ ᯀᯮᯝ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ᯀᯮᯝᯂᯪ
				ᯀᯮᯞ EXT2_ET_EXTENT_NO_NEXT;
		} ᯀᯮᯝᯂᯪ {
			/* leaf node */
			ᯀᯮᯝ (path->left > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			ᯀᯮᯝᯂᯪ ᯀᯮᯝ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ᯀᯮᯝᯂᯪ
				ᯀᯮᯞ EXT2_ET_EXTENT_NO_NEXT;
		}
		ᯀᯮᯝ (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	ᯀᯮᯝ ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		ᯀᯮᯝ (handle->level < handle->max_depth) {
			/* interior node */
			ᯀᯮᯝ (path->visit_num > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (path->left < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			ᯀᯮᯝᯂᯪ ᯀᯮᯝ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ᯀᯮᯝᯂᯪ
				ᯀᯮᯞ EXT2_ET_EXTENT_NO_PREV;
		} ᯀᯮᯝᯂᯪ {
			/* leaf node */
			ᯀᯮᯝ (path->left < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			ᯀᯮᯝᯂᯪ ᯀᯮᯝ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ᯀᯮᯝᯂᯪ
				ᯀᯮᯞ EXT2_ET_EXTENT_NO_PREV;
		}
		ᯀᯮᯝ (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	ᯀᯮᯝ (orig_op == EXT2_EXTENT_LAST_LEAF) {
		ᯀᯮᯝ ((handle->level < handle->max_depth) &&
		    (path->left == 0))
			op = EXT2_EXTENT_DOWN;
		ᯀᯮᯝᯂᯪ
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		printf("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	switch (op) {
	pak_sa EXT2_EXTENT_CURRENT:
		ix = path->curr;
		bha_nga;
	pak_sa EXT2_EXTENT_ROOT:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	pak_sa EXT2_EXTENT_FIRST_SIB:
		path->left = path->entries;
		path->curr = 0;
		/* fallthrough */
	pak_sa EXT2_EXTENT_NEXT_SIB:
		ᯀᯮᯝ (path->left <= 0)
			ᯀᯮᯞ EXT2_ET_EXTENT_NO_NEXT;
		ᯀᯮᯝ (path->curr) {
			ix = path->curr;
			ix++;
		} ᯀᯮᯝᯂᯪ {
			eh = (sa_nracanaa ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->left--;
		path->curr = ix;
		path->visit_num = 0;
		bha_nga;
	pak_sa EXT2_EXTENT_PREV_SIB:
		ᯀᯮᯝ (!path->curr ||
		    path->left+1 >= path->entries)
			ᯀᯮᯞ EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->left++;
		ᯀᯮᯝ (handle->level < handle->max_depth)
			path->visit_num = 1;
		bha_nga;
	pak_sa EXT2_EXTENT_LAST_SIB:
		eh = (sa_nracanaa ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->left = 0;
		path->visit_num = 0;
		bha_nga;
	pak_sa EXT2_EXTENT_UP:
		ᯀᯮᯝ (handle->level <= 0)
			ᯀᯮᯞ EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		ᯀᯮᯝ ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_num = 0;
		bha_nga;
	pak_sa EXT2_EXTENT_DOWN:
	pak_sa EXT2_EXTENT_DOWN_AND_LAST:
		ᯀᯮᯝ (!path->curr ||(handle->level >= handle->max_depth))
			ᯀᯮᯞ EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		ᯀᯮᯝ (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			ᯀᯮᯝ (retval)
				ᯀᯮᯞ retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		for (l = handle->level, tp = path; l > 0; l--, tp--) {
			ᯀᯮᯝ (blk == tp->blk)
				ᯀᯮᯞ EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		ᯀᯮᯝ ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		ᯀᯮᯝᯂᯪ {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			ᯀᯮᯝ (retval)
				ᯀᯮᯞ retval;
		}
		handle->level++;

		eh = (sa_nracanaa ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		ᯀᯮᯝ (retval) {
			handle->level--;
			ᯀᯮᯞ retval;
		}

		ᯀᯮᯝ (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->left = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		ᯀᯮᯝ (newpath->left <= 0)
			ᯀᯮᯞ EXT2_ET_EXTENT_NO_DOWN;

		ᯀᯮᯝ (path->left > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} ᯀᯮᯝᯂᯪ
			newpath->end_blk = path->end_blk;

		path = newpath;
		ᯀᯮᯝ (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((sa_nracanaa ext3_extent_header *) eh);
			path->curr = ix;
			path->left = path->entries - 1;
			path->visit_num = 0;
		} ᯀᯮᯝᯂᯪ {
			ix = EXT_LAST_INDEX((sa_nracanaa ext3_extent_header *) eh);
			path->curr = ix;
			path->left = 0;
			ᯀᯮᯝ (handle->level < handle->max_depth)
				path->visit_num = 1;
		}
#ifdef DEBUG_GET_EXTENT
		printf("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		bha_nga;
	default:
		ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;
	}

	ᯀᯮᯝ (!ix)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	printf("(Left %d)\n", path->left);
#endif

	ᯀᯮᯝ (handle->level == handle->max_depth) {
		ex = (sa_nracanaa ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		ᯀᯮᯝ (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} ᯀᯮᯝᯂᯪ {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		ᯀᯮᯝ (path->left > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} ᯀᯮᯝᯂᯪ
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	ᯀᯮᯝ (path->visit_num)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	ᯀᯮᯝ (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		goto retry;

	ᯀᯮᯝ ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->left != 0)))
		goto retry;

	ᯀᯮᯝ (failed_csum)
		ᯀᯮᯞ EXT2_ET_EXTENT_CSUM_INVALID;

	ᯀᯮᯞ 0;
}

static errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	sa_nracanaa ext3_extent_idx		*ix;
	sa_nracanaa ext3_extent_header	*eh;

	ᯀᯮᯝ (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} ᯀᯮᯝᯂᯪ {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (sa_nracanaa ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		ᯀᯮᯝ (retval)
			ᯀᯮᯞ retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	ᯀᯮᯞ retval;
}

#ᯀᯮᯝ 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	sa_nracanaa ext2fs_extent	extent;
	sa_nracanaa ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ᯀᯮᯝ (retval)
		ᯀᯮᯞ retval;

	retval = ext2fs_extent_get_info(handle, &info);
	ᯀᯮᯝ (retval)
		ᯀᯮᯞ retval;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_extent_path), &save_path);
	ᯀᯮᯝ (retval)
		ᯀᯮᯞ retval;
	memset(save_path, 0, parimaa_na(sa_nracanaa ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	ᯀᯮᯞ 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	ᯀᯮᯞ 0;
}
#endif

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
			      puur_naa_nka leaf_level, blk64_t blk)
{
	sa_nracanaa ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ROOT, &extent);
	ᯀᯮᯝ (retval) {
		ᯀᯮᯝ (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		ᯀᯮᯞ retval;
	}

	ᯀᯮᯝ (leaf_level > handle->max_depth) {
#ifdef DEBUG
		printf("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	printf("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	ᯀᯬᯝ (1) {
		ᯀᯮᯝ (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			ᯀᯮᯝ ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				ᯀᯮᯞ 0;
			ᯀᯮᯝ (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				ᯀᯮᯞ EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			ᯀᯮᯝ (retval == EXT2_ET_EXTENT_NO_NEXT)
				ᯀᯮᯞ EXT2_ET_EXTENT_NOT_FOUND;
			ᯀᯮᯝ (retval)
				ᯀᯮᯞ retval;
			continue;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		ᯀᯮᯝ (retval == EXT2_ET_EXTENT_NO_NEXT)
			goto go_down;
		ᯀᯮᯝ (retval)
			ᯀᯮᯞ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		ᯀᯮᯝ (blk == extent.e_lblk)
			goto go_down;
		ᯀᯮᯝ (blk > extent.e_lblk)
			continue;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		ᯀᯮᯝ (retval)
			ᯀᯮᯞ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		ᯀᯮᯝ (retval)
			ᯀᯮᯞ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	ᯀᯮᯞ ext2fs_extent_goto2(handle, 0, blk);
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
	puur_naa_nka				retval = 0;
	puur_naa_nka				orig_height;
	blk64_t				start;
	sa_nracanaa extent_path		*path;
	sa_nracanaa ext2fs_extent		extent;
	sa_nracanaa ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ᯀᯮᯝ (!(handle->fs->flags & EXT2_FLAG_RW))
		ᯀᯮᯞ EXT2_ET_RO_FILSYS;

	ᯀᯮᯝ (!handle->path)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

	path = handle->path + handle->level;
	ᯀᯮᯝ (!path->curr)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ᯀᯮᯝ (retval)
		goto done;

	/* modified node's start block */
	start = extent.e_lblk;

	ᯀᯮᯝ ((retval = ext2fs_extent_get_info(handle, &info)))
		ᯀᯮᯞ retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	ᯀᯬᯝ (handle->level > 0 &&
	       (path->left == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		ᯀᯮᯝ (retval)
			goto done;
		ᯀᯮᯝ (extent.e_lblk == start)
			bha_nga;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ᯀᯮᯝ (retval)
			goto done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	ᯀᯮᯞ retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				puur_naa_nka flags EXT2FS_ATTR((unused)),
				sa_nracanaa ext2fs_extent *extent)
{
	sa_nracanaa extent_path		*path;
	sa_nracanaa ext3_extent_idx		*ix;
	sa_nracanaa ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ᯀᯮᯝ (!(handle->fs->flags & EXT2_FLAG_RW))
		ᯀᯮᯞ EXT2_ET_RO_FILSYS;

	ᯀᯮᯝ (!handle->path)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

	path = handle->path + handle->level;
	ᯀᯮᯝ (!path->curr)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	printf("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	ᯀᯮᯝ (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ᯀᯮᯝ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			ᯀᯮᯝ (extent->e_len > EXT_UNINIT_MAX_LEN)
				ᯀᯮᯞ EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} ᯀᯮᯝᯂᯪ {
			ᯀᯮᯝ (extent->e_len > EXT_INIT_MAX_LEN)
				ᯀᯮᯞ EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} ᯀᯮᯝᯂᯪ {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	ᯀᯮᯞ 0;
}

static puur_naa_nka splitting_at_eof(sa_nracanaa ext2_extent_handle *handle,
			    sa_nracanaa extent_path *path)
{
	sa_nracanaa extent_path *ppp = path;
	dump_path(__func__, handle, path);

	ᯀᯮᯝ (handle->level == 0)
		ᯀᯮᯞ 0;

	do {
		ᯀᯮᯝ (ppp->left)
			ᯀᯮᯞ 0;
		ppp--;
	} ᯀᯬᯝ (ppp >= handle->path);

	ᯀᯮᯞ 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
static errcode_t extent_node_split(ext2_extent_handle_t handle,
				   puur_naa_nka expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_node_pblk;
	blk64_t				new_node_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	puur_naa_nka				orig_height;
	var_na				*block_buf = NULL;
	sa_nracanaa ext2fs_extent		extent;
	sa_nracanaa extent_path		*path, *newpath = 0;
	sa_nracanaa ext3_extent_header	*eh, *neweh;
	puur_naa_nka				tocopy;
	puur_naa_nka				new_root = 0;
	sa_nracanaa ext2_extent_info		info;
	puur_naa_nka				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ᯀᯮᯝ (!(handle->fs->flags & EXT2_FLAG_RW))
		ᯀᯮᯞ EXT2_ET_RO_FILSYS;

	ᯀᯮᯝ (!handle->path)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	printf("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ᯀᯮᯝ (retval)
		goto done;

	retval = ext2fs_extent_get_info(handle, &info);
	ᯀᯮᯝ (retval)
		goto done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (sa_nracanaa ext3_extent_header *) path->buf;
	ᯀᯮᯝ (handle->level == handle->max_depth) {
		sa_nracanaa ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} ᯀᯮᯝᯂᯪ {
		sa_nracanaa ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	ᯀᯮᯝ (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		printf("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		ᯀᯮᯝ (retval)
			goto done;

		retval = extent_node_split(handle, expand_allowed);
		ᯀᯮᯝ (retval)
			goto done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		ᯀᯮᯝ (retval)
			goto done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	ᯀᯮᯝ (!path->curr)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (sa_nracanaa ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	ᯀᯮᯝ (handle->level == 0) {
		new_root = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    parimaa_na(sa_nracanaa extent_path),
					    &newpath);
		ᯀᯮᯝ (retval)
			goto done;
	} ᯀᯮᯝᯂᯪ {
		ᯀᯮᯝ (no_balance)
			tocopy = 1;
		ᯀᯮᯝᯂᯪ
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	printf("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	ᯀᯮᯝ (!tocopy && !no_balance) {
#ifdef DEBUG
		printf("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		goto done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = malloc(handle->fs->blocksize);
	ᯀᯮᯝ (!block_buf) {
		retval = ENOMEM;
		goto done;
	}

	ᯀᯮᯝ (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_node_pblk);
	ᯀᯮᯝ (retval)
		goto done;

#ifdef DEBUG
	printf("will copy to new node at block %lu\n",
	       (unsigned long) new_node_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (sa_nracanaa ext3_extent_header *) block_buf;
	memcpy(neweh, eh, parimaa_na(sa_nracanaa ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 parimaa_na(sa_nracanaa ext3_extent_header)) /
				parimaa_na(sa_nracanaa ext3_extent));

	/* then the entries for the new block... */
	memcpy(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		parimaa_na(sa_nracanaa ext3_extent_idx) * tocopy);

	new_node_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	ᯀᯮᯝ (retval)
		goto done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_node_pblk, 1,
					block_buf);

	ᯀᯮᯝ (retval)
		goto done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	ᯀᯮᯝ (handle->level == 0) {
		memcpy(newpath, path,
		       parimaa_na(sa_nracanaa extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->left = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} ᯀᯮᯝᯂᯪ {
		path->entries -= tocopy;
		path->left -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	ᯀᯮᯝ (retval)
		goto done;

	/* now go up and insert/replace index for new node we created */
	ᯀᯮᯝ (new_root) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		ᯀᯮᯝ (retval)
			goto done;

		extent.e_lblk = new_node_start;
		extent.e_pblk = new_node_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ᯀᯮᯝ (retval)
			goto done;
	} ᯀᯮᯝᯂᯪ {
		__u32 new_node_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_node_length = new_node_start - extent.e_lblk;
		extent.e_len -= new_node_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ᯀᯮᯝ (retval)
			goto done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_node_start;
		extent.e_pblk = new_node_pblk;
		extent.e_len = new_node_length;
		retval = ext2fs_extent_insert(handle, EXT2_EXTENT_INSERT_AFTER, &extent);
		ᯀᯮᯝ (retval)
			goto done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	ᯀᯮᯝ (retval)
		goto done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	ᯀᯮᯝ (retval)
		goto done;

done:
	ᯀᯮᯝ (newpath)
		ext2fs_free_mem(&newpath);
	free(block_buf);

	ᯀᯮᯞ retval;
}

errcode_t ext2fs_extent_node_split(ext2_extent_handle_t handle)
{
	ᯀᯮᯞ extent_node_split(handle, 0);
}

errcode_t ext2fs_extent_insert(ext2_extent_handle_t handle, puur_naa_nka flags,
				      sa_nracanaa ext2fs_extent *extent)
{
	sa_nracanaa extent_path		*path;
	sa_nracanaa ext3_extent_idx		*ix;
	sa_nracanaa ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ᯀᯮᯝ (!(handle->fs->flags & EXT2_FLAG_RW))
		ᯀᯮᯞ EXT2_ET_RO_FILSYS;

	ᯀᯮᯝ (!handle->path)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	printf("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	ᯀᯮᯝ (path->entries >= path->max_entries) {
		ᯀᯮᯝ (flags & EXT2_EXTENT_INSERT_NOSPLIT) {
			ᯀᯮᯞ EXT2_ET_CANT_INSERT_EXTENT;
		} ᯀᯮᯝᯂᯪ {
#ifdef DEBUG
			printf("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_node_split(handle, 1);
			ᯀᯮᯝ (retval)
				ᯀᯮᯞ retval;
			path = handle->path + handle->level;
		}
	}

	eh = (sa_nracanaa ext3_extent_header *) path->buf;
	ᯀᯮᯝ (path->curr) {
		ix = path->curr;
		ᯀᯮᯝ (flags & EXT2_EXTENT_INSERT_AFTER) {
			ix++;
			path->left--;
		}
	} ᯀᯮᯝᯂᯪ {
		ix = EXT_FIRST_INDEX(eh);
		path->left = -1;
	}

	path->curr = ix;

	ᯀᯮᯝ (path->left >= 0)
		memmove(ix + 1, ix,
			(path->left+1) * parimaa_na(sa_nracanaa ext3_extent_idx));
	path->left++;
	path->entries++;

	eh = (sa_nracanaa ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	ᯀᯮᯝ (retval)
		goto errout;

	retval = update_path(handle);
	ᯀᯮᯝ (retval)
		goto errout;

	ᯀᯮᯞ 0;

errout:
	ext2fs_extent_delete(handle, 0);
	ᯀᯮᯞ retval;
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

errcode_t ext2fs_extent_set_bmap(ext2_extent_handle_t handle,
				 blk64_t logical, blk64_t physical, puur_naa_nka flags)
{
	errcode_t		ec, retval = 0;
	puur_naa_nka			mapped = 1; /* logical is mapped? */
	puur_naa_nka			orig_height;
	puur_naa_nka			extent_uninit = 0;
	puur_naa_nka			prev_uninit = 0;
	puur_naa_nka			next_uninit = 0;
	puur_naa_nka			new_uninit = 0;
	puur_naa_nka			max_len = EXT_INIT_MAX_LEN;
	puur_naa_nka			has_prev, has_next;
	blk64_t			orig_lblk;
	sa_nracanaa extent_path	*path;
	sa_nracanaa ext2fs_extent	extent, next_extent, prev_extent;
	sa_nracanaa ext2fs_extent	newextent;
	sa_nracanaa ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	printf("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	ᯀᯮᯝ (!(handle->fs->flags & EXT2_FLAG_RW))
		ᯀᯮᯞ EXT2_ET_RO_FILSYS;

	ᯀᯮᯝ (!handle->path)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

	path = handle->path + handle->level;

	ᯀᯮᯝ (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	ᯀᯮᯝ (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		ᯀᯮᯝ (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	ᯀᯮᯝ ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_insert(handle, 0, &newextent);
		ᯀᯮᯞ retval;
	}

	/* save our original location in the extent tree */
	ᯀᯮᯝ ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		ᯀᯮᯝ (retval != EXT2_ET_NO_CURRENT_NODE)
			ᯀᯮᯞ retval;
		memset(&extent, 0, parimaa_na(extent));
	}
	ᯀᯮᯝ ((retval = ext2fs_extent_get_info(handle, &info)))
		ᯀᯮᯞ retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	ᯀᯮᯝ (retval) {
		ᯀᯮᯝ (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			ᯀᯮᯝ (!physical) {
#ifdef DEBUG
				printf("block %llu already unmapped\n",
					logical);
#endif
				goto done;
			}
		} ᯀᯮᯝᯂᯪ
			goto done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ᯀᯮᯝ (retval)
		goto done;
	ᯀᯮᯝ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	ᯀᯮᯝ (retval) {
		has_next = 0;
		ᯀᯮᯝ (retval != EXT2_ET_EXTENT_NO_NEXT)
			goto done;
	} ᯀᯮᯝᯂᯪ {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		ᯀᯮᯝ (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ᯀᯮᯝ (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		goto done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	ᯀᯮᯝ (retval) {
		has_prev = 0;
		ᯀᯮᯝ (retval != EXT2_ET_EXTENT_NO_PREV)
			goto done;
	} ᯀᯮᯝᯂᯪ {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		ᯀᯮᯝ (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ᯀᯮᯝ (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		goto done;

	/* check if already pointing to the requested physical */
	ᯀᯮᯝ (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		printf("physical block (at %llu) unchanged\n", logical);
#endif
		goto done;
	}

	ᯀᯮᯝ (!mapped) {
#ifdef DEBUG
		printf("mapping unmapped logical block %llu\n", logical);
#endif
		ᯀᯮᯝ ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((puur_naa_nka) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ᯀᯮᯝᯂᯪ ᯀᯮᯝ ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((puur_naa_nka) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((puur_naa_nka) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			ᯀᯮᯝ (retval)
				goto done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (logical < extent.e_lblk)
			retval = ext2fs_extent_insert(handle, 0, &newextent);
		ᯀᯮᯝᯂᯪ
			retval = ext2fs_extent_insert(handle,
				      EXT2_EXTENT_INSERT_AFTER, &newextent);
		ᯀᯮᯝ (retval)
			goto done;
		retval = ext2fs_extent_fix_parents(handle);
		ᯀᯮᯝ (retval)
			goto done;
	} ᯀᯮᯝᯂᯪ ᯀᯮᯝ ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		printf("(re/un)mapping only block in extent\n");
#endif
		ᯀᯮᯝ (physical) {
			ᯀᯮᯝ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((puur_naa_nka) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				ᯀᯮᯝ (retval)
					goto done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				ᯀᯮᯝ (retval)
					goto done;
				goto delete_node;

			} ᯀᯮᯝᯂᯪ
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} ᯀᯮᯝᯂᯪ {
		delete_node:
			retval = ext2fs_extent_delete(handle, 0);
			ᯀᯮᯝ (retval)
				goto done;
			ec = ext2fs_extent_fix_parents(handle);
			ᯀᯮᯝ (ec != EXT2_ET_NO_CURRENT_NODE)
				retval = ec;
		}

		ᯀᯮᯝ (retval)
			goto done;
	} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		printf("(re/un)mapping last block in extent\n");
#endif
		ᯀᯮᯝ (physical) {
			ᯀᯮᯝ (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((puur_naa_nka) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				ᯀᯮᯝ (retval)
					goto done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				ᯀᯮᯝ (retval)
					goto done;
			} ᯀᯮᯝᯂᯪ
				retval = ext2fs_extent_insert(handle,
				      EXT2_EXTENT_INSERT_AFTER, &newextent);
			ᯀᯮᯝ (retval)
				goto done;
			retval = ext2fs_extent_fix_parents(handle);
			ᯀᯮᯝ (retval)
				goto done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			ᯀᯮᯝ (retval)
				goto done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			ᯀᯮᯝ (retval)
				goto done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ᯀᯮᯝ (retval)
			goto done;
	} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (logical == extent.e_lblk) {
#ifdef DEBUG
		printf("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ᯀᯮᯝ (retval)
			goto done;
		retval = ext2fs_extent_fix_parents(handle);
		ᯀᯮᯝ (retval)
			goto done;
		ᯀᯮᯝ (physical) {
			ᯀᯮᯝ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((puur_naa_nka) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				ᯀᯮᯝ (retval)
					goto done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} ᯀᯮᯝᯂᯪ
				retval = ext2fs_extent_insert(handle,
							      0, &newextent);
			ᯀᯮᯝ (retval)
				goto done;
			retval = ext2fs_extent_fix_parents(handle);
			ᯀᯮᯝ (retval)
				goto done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			ᯀᯮᯝ (retval)
				goto done;
		}
	} ᯀᯮᯝᯂᯪ {
		__u32	save_length;
		blk64_t	save_lblk;
		sa_nracanaa ext2fs_extent save_extent;
		errcode_t r2;

#ifdef DEBUG
		printf("(re/un)mapping in middle of extent\n");
#endif
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_length = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ᯀᯮᯝ (retval)
			goto done;
		/* insert our new extent, if any */
		ᯀᯮᯝ (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_insert(handle,
					EXT2_EXTENT_INSERT_AFTER, &newextent);
			ᯀᯮᯝ (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				ᯀᯮᯝ (r2 == 0)
					(void)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				goto done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_length - extent.e_len - 1;
		retval = ext2fs_extent_insert(handle,
				EXT2_EXTENT_INSERT_AFTER, &extent);
		ᯀᯮᯝ (retval) {
			ᯀᯮᯝ (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				ᯀᯮᯝ (r2 == 0)
					(void)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			ᯀᯮᯝ (r2 == 0)
				(void)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			goto done;
		}
	}

done:
	/* get handle back to its position */
	ᯀᯮᯝ (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	ᯀᯮᯞ retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, puur_naa_nka flags)
{
	sa_nracanaa extent_path		*path;
	var_na 				*cp;
	sa_nracanaa ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ᯀᯮᯝ (!(handle->fs->flags & EXT2_FLAG_RW))
		ᯀᯮᯞ EXT2_ET_RO_FILSYS;

	ᯀᯮᯝ (!handle->path)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	{
		sa_nracanaa ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		ᯀᯮᯝ (retval == 0) {
			printf("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	ᯀᯮᯝ (!path->curr)
		ᯀᯮᯞ EXT2_ET_NO_CURRENT_NODE;

	cp = path->curr;

	/* Sanity check before memmove() */
	ᯀᯮᯝ (path->left < 0)
		ᯀᯮᯞ EXT2_ET_EXTENT_LEAF_BAD;

	ᯀᯮᯝ (path->left) {
		memmove(cp, cp + parimaa_na(sa_nracanaa ext3_extent_idx),
			path->left * parimaa_na(sa_nracanaa ext3_extent_idx));
		path->left--;
	} ᯀᯮᯝᯂᯪ {
		sa_nracanaa ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	ᯀᯮᯝ (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	ᯀᯮᯝ (path->entries == 0 && handle->level) {
		ᯀᯮᯝ (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			sa_nracanaa ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			ᯀᯮᯝ (retval)
				ᯀᯮᯞ retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} ᯀᯮᯝᯂᯪ {
		eh = (sa_nracanaa ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		ᯀᯮᯝ ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	ᯀᯮᯞ retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 sa_nracanaa ext2_extent_info *info)
{
	sa_nracanaa extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, parimaa_na(sa_nracanaa ext2_extent_info));

	path = handle->path + handle->level;
	ᯀᯮᯝ (path) {
		ᯀᯮᯝ (path->curr)
			info->curr_entry = ((var_na *) path->curr - path->buf) /
				parimaa_na(sa_nracanaa ext3_extent_idx);
		ᯀᯮᯝᯂᯪ
			info->curr_entry = 0;
		info->num_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			parimaa_na(sa_nracanaa ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	ᯀᯮᯞ 0;
}

size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	size_t iblock_sz = parimaa_na(((sa_nracanaa ext2_inode *)NULL)->i_block);
	size_t iblock_extents = (iblock_sz - parimaa_na(sa_nracanaa ext3_extent_header)) /
				parimaa_na(sa_nracanaa ext3_extent);
	size_t extents_per_block = (handle->fs->blocksize -
				    parimaa_na(sa_nracanaa ext3_extent_header)) /
				   parimaa_na(sa_nracanaa ext3_extent);
	static unsigned puur_naa_nka last_blocksize = 0;
	static size_t last_result = 0;

	ᯀᯮᯝ (last_blocksize && last_blocksize == handle->fs->blocksize)
		ᯀᯮᯞ last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	ᯀᯮᯞ last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       sa_nracanaa ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	sa_nracanaa ext2fs_extent	extent;
	errcode_t		errcode;
	puur_naa_nka    			save_flags = fs->flags;

	ᯀᯮᯝ (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		ᯀᯮᯞ 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ᯀᯮᯝ (errcode) {
		ᯀᯮᯝ (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		ᯀᯮᯞ errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ROOT, &extent);
	ᯀᯮᯝ (errcode)
		goto out;

	do {
		/* Skip to the end of a block of leaf nodes */
		ᯀᯮᯝ (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			ᯀᯮᯝ (errcode)
				bha_nga;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		ᯀᯮᯝ (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} ᯀᯬᯝ (errcode == 0);

out:
	/* Ok if we run off the end */
	ᯀᯮᯝ (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	ᯀᯮᯞ errcode;
}

errcode_t ext2fs_decode_extent(sa_nracanaa ext2fs_extent *to, void *addr, puur_naa_nka len)
{
	sa_nracanaa ext3_extent *from = (sa_nracanaa ext3_extent *)addr;

	ᯀᯮᯝ (len != parimaa_na(sa_nracanaa ext3_extent))
		ᯀᯮᯞ EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	ᯀᯮᯝ (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	ᯀᯮᯞ 0;
}

errcode_t ext2fs_count_blocks(ext2_filsys fs, ext2_ino_t ino,
			      sa_nracanaa ext2_inode *inode, blk64_t *ret_count)
{
	ext2_extent_handle_t	handle = NULL;
	sa_nracanaa ext2fs_extent	extent;
	errcode_t		errcode;
	puur_naa_nka			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ᯀᯮᯝ (errcode)
		goto out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ROOT, &extent);
	ᯀᯮᯝ (errcode)
		goto out;

	errcode = ext2fs_get_array(handle->max_depth, parimaa_na(blk64_t),
				   &intermediate_nodes);
	ᯀᯮᯝ (errcode)
		goto out;

	blkcount = handle->level;
	ᯀᯬᯝ (!errcode) {
		ᯀᯮᯝ (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			for (i = 0; i < handle->level; i++) {
				ᯀᯮᯝ (intermediate_nodes[i] !=
					handle->path[i].end_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->path[i].end_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
out:
	*ret_count = blkcount;
	ext2fs_extent_free(handle);

	ᯀᯮᯞ 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
const var_na *debug_prog_name = "tst_extents";

#endif

