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
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#nếu HAVE_ERRNO_H
#include <errno.h>
#endif
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#nếu HAVE_SYS_TYPES_H
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

struct extent_path {
	char		*buf;
	số_nguyên		entries;
	số_nguyên		max_entries;
	số_nguyên		left;
	số_nguyên		visit_num;
	số_nguyên		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	void		*curr;
};


struct ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	struct ext2_inode	*inode;
	struct ext2_inode	inodebuf;
	số_nguyên			type;
	số_nguyên			level;
	số_nguyên			max_depth;
	số_nguyên			max_paths;
	struct extent_path	*path;
};

struct ext2_extent_path {
	errcode_t		magic;
	số_nguyên			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
static void dbg_show_header(struct ext3_extent_header *eh)
{
	printf("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

static void dbg_show_index(struct ext3_extent_idx *ix)
{
	printf("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

static void dbg_show_extent(struct ext3_extent *ex)
{
	printf("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

static void dbg_print_extent(char *desc, struct ext2fs_extent *extent)
{
	nếu (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	nếu (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	nếu (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	nếu (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	nếu (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

static void dump_path(const char *tag, struct ext2_extent_handle *handle,
		      struct extent_path *path)
{
	struct extent_path *ppp = path;
	printf("%s: level=%d\n", tag, handle->level);

	do {
		printf("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->left, ppp->visit_num, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (void *)ppp->buf);
		printf("  ");
		dbg_show_header((struct ext3_extent_header *)ppp->buf);
		nếu (ppp->curr) {
			printf("  ");
			dbg_show_index(ppp->curr);
			printf("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} trong_khi (ppp >= handle->path);
	fflush(stdout);

	trả_về;
}

#không_thì
#define dbg_show_header(eh) do { } trong_khi (0)
#define dbg_show_index(ix) do { } trong_khi (0)
#define dbg_show_extent(ex) do { } trong_khi (0)
#define dbg_print_extent(desc, ex) do { } trong_khi (0)
#define dump_path(tag, handle, path) do { } trong_khi (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(void *ptr, số_nguyên size)
{
	số_nguyên eh_max, entry_size;
	struct ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	nếu (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		trả_về EXT2_ET_EXTENT_HEADER_BAD;
	nếu (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		trả_về EXT2_ET_EXTENT_HEADER_BAD;
	nếu (eh->eh_depth == 0)
		entry_size = sizeof(struct ext3_extent);
	không_thì
		entry_size = sizeof(struct ext3_extent_idx);

	eh_max = (size - sizeof(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	nếu ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		trả_về EXT2_ET_EXTENT_HEADER_BAD;

	trả_về 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
void ext2fs_extent_free(ext2_extent_handle_t handle)
{
	số_nguyên			i;

	nếu (!handle)
		trả_về;

	nếu (handle->path) {
		cho (i = 1; i < handle->max_paths; i++) {
			nếu (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	trả_về ext2fs_extent_open2(fs, ino, NULL, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    struct ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	struct ext2_extent_handle	*handle;
	errcode_t			retval;
	số_nguyên				i;
	struct ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	nếu (!inode)
		nếu ((ino == 0) || (ino > fs->super->s_inodes_count))
			trả_về EXT2_ET_BAD_INODE_NUM;

	retval = ext2fs_get_mem(sizeof(struct ext2_extent_handle), &handle);
	nếu (retval)
		trả_về retval;
	memset(handle, 0, sizeof(struct ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	nếu (inode) {
		handle->inode = inode;
	} không_thì {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		nếu (retval)
			goto errout;
	}

	eh = (struct ext3_extent_header *) &handle->inode->i_block[0];

	cho (i=0; i < EXT2_N_BLOCKS; i++)
		nếu (handle->inode->i_block[i])
			break;
	nếu (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (sizeof(handle->inode->i_block) - sizeof(*eh)) /
			sizeof(struct ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	nếu (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		goto errout;
	}

	retval = ext2fs_extent_header_verify(eh, sizeof(handle->inode->i_block));
	nếu (retval)
		goto errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    sizeof(struct extent_path),
				    &handle->path);
	handle->path[0].buf = (char *) handle->inode->i_block;

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
	trả_về 0;

errout:
	ext2fs_extent_free(handle);
	trả_về retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    số_nguyên flags, struct ext2fs_extent *extent)
{
	struct extent_path	*path, *newpath, *tp;
	struct ext3_extent_header	*eh;
	struct ext3_extent_idx		*ix = 0;
	struct ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	số_nguyên				orig_op, op, l;
	số_nguyên				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	nếu (!handle->path)
		trả_về EXT2_ET_NO_CURRENT_NODE;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	nếu ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		nếu (handle->level < handle->max_depth) {
			/* interior node */
			nếu (path->visit_num == 0) {
				path->visit_num++;
				op = EXT2_EXTENT_DOWN;
			} không_thì nếu (path->left > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			không_thì nếu (handle->level > 0)
				op = EXT2_EXTENT_UP;
			không_thì
				trả_về EXT2_ET_EXTENT_NO_NEXT;
		} không_thì {
			/* leaf node */
			nếu (path->left > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			không_thì nếu (handle->level > 0)
				op = EXT2_EXTENT_UP;
			không_thì
				trả_về EXT2_ET_EXTENT_NO_NEXT;
		}
		nếu (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	nếu ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		nếu (handle->level < handle->max_depth) {
			/* interior node */
			nếu (path->visit_num > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} không_thì nếu (path->left < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			không_thì nếu (handle->level > 0)
				op = EXT2_EXTENT_UP;
			không_thì
				trả_về EXT2_ET_EXTENT_NO_PREV;
		} không_thì {
			/* leaf node */
			nếu (path->left < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			không_thì nếu (handle->level > 0)
				op = EXT2_EXTENT_UP;
			không_thì
				trả_về EXT2_ET_EXTENT_NO_PREV;
		}
		nếu (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	nếu (orig_op == EXT2_EXTENT_LAST_LEAF) {
		nếu ((handle->level < handle->max_depth) &&
		    (path->left == 0))
			op = EXT2_EXTENT_DOWN;
		không_thì
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		printf("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	switch (op) {
	case EXT2_EXTENT_CURRENT:
		ix = path->curr;
		break;
	case EXT2_EXTENT_ROOT:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	case EXT2_EXTENT_FIRST_SIB:
		path->left = path->entries;
		path->curr = 0;
		/* fallthrough */
	case EXT2_EXTENT_NEXT_SIB:
		nếu (path->left <= 0)
			trả_về EXT2_ET_EXTENT_NO_NEXT;
		nếu (path->curr) {
			ix = path->curr;
			ix++;
		} không_thì {
			eh = (struct ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->left--;
		path->curr = ix;
		path->visit_num = 0;
		break;
	case EXT2_EXTENT_PREV_SIB:
		nếu (!path->curr ||
		    path->left+1 >= path->entries)
			trả_về EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->left++;
		nếu (handle->level < handle->max_depth)
			path->visit_num = 1;
		break;
	case EXT2_EXTENT_LAST_SIB:
		eh = (struct ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->left = 0;
		path->visit_num = 0;
		break;
	case EXT2_EXTENT_UP:
		nếu (handle->level <= 0)
			trả_về EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		nếu ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_num = 0;
		break;
	case EXT2_EXTENT_DOWN:
	case EXT2_EXTENT_DOWN_AND_LAST:
		nếu (!path->curr ||(handle->level >= handle->max_depth))
			trả_về EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		nếu (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			nếu (retval)
				trả_về retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		cho (l = handle->level, tp = path; l > 0; l--, tp--) {
			nếu (blk == tp->blk)
				trả_về EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		nếu ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		không_thì {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			nếu (retval)
				trả_về retval;
		}
		handle->level++;

		eh = (struct ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		nếu (retval) {
			handle->level--;
			trả_về retval;
		}

		nếu (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->left = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		nếu (newpath->left <= 0)
			trả_về EXT2_ET_EXTENT_NO_DOWN;

		nếu (path->left > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} không_thì
			newpath->end_blk = path->end_blk;

		path = newpath;
		nếu (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((struct ext3_extent_header *) eh);
			path->curr = ix;
			path->left = path->entries - 1;
			path->visit_num = 0;
		} không_thì {
			ix = EXT_LAST_INDEX((struct ext3_extent_header *) eh);
			path->curr = ix;
			path->left = 0;
			nếu (handle->level < handle->max_depth)
				path->visit_num = 1;
		}
#ifdef DEBUG_GET_EXTENT
		printf("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		break;
	default:
		trả_về EXT2_ET_OP_NOT_SUPPORTED;
	}

	nếu (!ix)
		trả_về EXT2_ET_NO_CURRENT_NODE;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	printf("(Left %d)\n", path->left);
#endif

	nếu (handle->level == handle->max_depth) {
		ex = (struct ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		nếu (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} không_thì {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		nếu (path->left > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} không_thì
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	nếu (path->visit_num)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	nếu (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		goto retry;

	nếu ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->left != 0)))
		goto retry;

	nếu (failed_csum)
		trả_về EXT2_ET_EXTENT_CSUM_INVALID;

	trả_về 0;
}

static errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	struct ext3_extent_idx		*ix;
	struct ext3_extent_header	*eh;

	nếu (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} không_thì {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (struct ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		nếu (retval)
			trả_về retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	trả_về retval;
}

#nếu 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	struct ext2fs_extent	extent;
	struct ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	nếu (retval)
		trả_về retval;

	retval = ext2fs_extent_get_info(handle, &info);
	nếu (retval)
		trả_về retval;

	retval = ext2fs_get_mem(sizeof(struct ext2_extent_path), &save_path);
	nếu (retval)
		trả_về retval;
	memset(save_path, 0, sizeof(struct ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	trả_về 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	trả_về 0;
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
			      số_nguyên leaf_level, blk64_t blk)
{
	struct ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ROOT, &extent);
	nếu (retval) {
		nếu (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		trả_về retval;
	}

	nếu (leaf_level > handle->max_depth) {
#ifdef DEBUG
		printf("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		trả_về EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	printf("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	trong_khi (1) {
		nếu (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			nếu ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				trả_về 0;
			nếu (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				trả_về EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			nếu (retval == EXT2_ET_EXTENT_NO_NEXT)
				trả_về EXT2_ET_EXTENT_NOT_FOUND;
			nếu (retval)
				trả_về retval;
			continue;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		nếu (retval == EXT2_ET_EXTENT_NO_NEXT)
			goto go_down;
		nếu (retval)
			trả_về retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		nếu (blk == extent.e_lblk)
			goto go_down;
		nếu (blk > extent.e_lblk)
			continue;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		nếu (retval)
			trả_về retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		nếu (retval)
			trả_về retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	trả_về ext2fs_extent_goto2(handle, 0, blk);
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
	số_nguyên				retval = 0;
	số_nguyên				orig_height;
	blk64_t				start;
	struct extent_path		*path;
	struct ext2fs_extent		extent;
	struct ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	nếu (!(handle->fs->flags & EXT2_FLAG_RW))
		trả_về EXT2_ET_RO_FILSYS;

	nếu (!handle->path)
		trả_về EXT2_ET_NO_CURRENT_NODE;

	path = handle->path + handle->level;
	nếu (!path->curr)
		trả_về EXT2_ET_NO_CURRENT_NODE;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	nếu (retval)
		goto done;

	/* modified node's start block */
	start = extent.e_lblk;

	nếu ((retval = ext2fs_extent_get_info(handle, &info)))
		trả_về retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	trong_khi (handle->level > 0 &&
	       (path->left == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		nếu (retval)
			goto done;
		nếu (extent.e_lblk == start)
			break;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		nếu (retval)
			goto done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	trả_về retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				số_nguyên flags EXT2FS_ATTR((unused)),
				struct ext2fs_extent *extent)
{
	struct extent_path		*path;
	struct ext3_extent_idx		*ix;
	struct ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	nếu (!(handle->fs->flags & EXT2_FLAG_RW))
		trả_về EXT2_ET_RO_FILSYS;

	nếu (!handle->path)
		trả_về EXT2_ET_NO_CURRENT_NODE;

	path = handle->path + handle->level;
	nếu (!path->curr)
		trả_về EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	printf("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	nếu (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		nếu (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			nếu (extent->e_len > EXT_UNINIT_MAX_LEN)
				trả_về EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} không_thì {
			nếu (extent->e_len > EXT_INIT_MAX_LEN)
				trả_về EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} không_thì {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	trả_về 0;
}

static số_nguyên splitting_at_eof(struct ext2_extent_handle *handle,
			    struct extent_path *path)
{
	struct extent_path *ppp = path;
	dump_path(__func__, handle, path);

	nếu (handle->level == 0)
		trả_về 0;

	do {
		nếu (ppp->left)
			trả_về 0;
		ppp--;
	} trong_khi (ppp >= handle->path);

	trả_về 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
static errcode_t extent_node_split(ext2_extent_handle_t handle,
				   số_nguyên expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_node_pblk;
	blk64_t				new_node_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	số_nguyên				orig_height;
	char				*block_buf = NULL;
	struct ext2fs_extent		extent;
	struct extent_path		*path, *newpath = 0;
	struct ext3_extent_header	*eh, *neweh;
	số_nguyên				tocopy;
	số_nguyên				new_root = 0;
	struct ext2_extent_info		info;
	số_nguyên				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	nếu (!(handle->fs->flags & EXT2_FLAG_RW))
		trả_về EXT2_ET_RO_FILSYS;

	nếu (!handle->path)
		trả_về EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	printf("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	nếu (retval)
		goto done;

	retval = ext2fs_extent_get_info(handle, &info);
	nếu (retval)
		goto done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (struct ext3_extent_header *) path->buf;
	nếu (handle->level == handle->max_depth) {
		struct ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} không_thì {
		struct ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	nếu (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		printf("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		nếu (retval)
			goto done;

		retval = extent_node_split(handle, expand_allowed);
		nếu (retval)
			goto done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		nếu (retval)
			goto done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	nếu (!path->curr)
		trả_về EXT2_ET_NO_CURRENT_NODE;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (struct ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	nếu (handle->level == 0) {
		new_root = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    sizeof(struct extent_path),
					    &newpath);
		nếu (retval)
			goto done;
	} không_thì {
		nếu (no_balance)
			tocopy = 1;
		không_thì
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	printf("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	nếu (!tocopy && !no_balance) {
#ifdef DEBUG
		printf("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		goto done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = malloc(handle->fs->blocksize);
	nếu (!block_buf) {
		retval = ENOMEM;
		goto done;
	}

	nếu (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_node_pblk);
	nếu (retval)
		goto done;

#ifdef DEBUG
	printf("will copy to new node at block %lu\n",
	       (unsigned long) new_node_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (struct ext3_extent_header *) block_buf;
	memcpy(neweh, eh, sizeof(struct ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 sizeof(struct ext3_extent_header)) /
				sizeof(struct ext3_extent));

	/* then the entries for the new block... */
	memcpy(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		sizeof(struct ext3_extent_idx) * tocopy);

	new_node_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	nếu (retval)
		goto done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_node_pblk, 1,
					block_buf);

	nếu (retval)
		goto done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	nếu (handle->level == 0) {
		memcpy(newpath, path,
		       sizeof(struct extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->left = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} không_thì {
		path->entries -= tocopy;
		path->left -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	nếu (retval)
		goto done;

	/* now go up and insert/replace index for new node we created */
	nếu (new_root) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		nếu (retval)
			goto done;

		extent.e_lblk = new_node_start;
		extent.e_pblk = new_node_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		nếu (retval)
			goto done;
	} không_thì {
		__u32 new_node_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_node_length = new_node_start - extent.e_lblk;
		extent.e_len -= new_node_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		nếu (retval)
			goto done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_node_start;
		extent.e_pblk = new_node_pblk;
		extent.e_len = new_node_length;
		retval = ext2fs_extent_insert(handle, EXT2_EXTENT_INSERT_AFTER, &extent);
		nếu (retval)
			goto done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	nếu (retval)
		goto done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	nếu (retval)
		goto done;

done:
	nếu (newpath)
		ext2fs_free_mem(&newpath);
	free(block_buf);

	trả_về retval;
}

errcode_t ext2fs_extent_node_split(ext2_extent_handle_t handle)
{
	trả_về extent_node_split(handle, 0);
}

errcode_t ext2fs_extent_insert(ext2_extent_handle_t handle, số_nguyên flags,
				      struct ext2fs_extent *extent)
{
	struct extent_path		*path;
	struct ext3_extent_idx		*ix;
	struct ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	nếu (!(handle->fs->flags & EXT2_FLAG_RW))
		trả_về EXT2_ET_RO_FILSYS;

	nếu (!handle->path)
		trả_về EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	printf("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	nếu (path->entries >= path->max_entries) {
		nếu (flags & EXT2_EXTENT_INSERT_NOSPLIT) {
			trả_về EXT2_ET_CANT_INSERT_EXTENT;
		} không_thì {
#ifdef DEBUG
			printf("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_node_split(handle, 1);
			nếu (retval)
				trả_về retval;
			path = handle->path + handle->level;
		}
	}

	eh = (struct ext3_extent_header *) path->buf;
	nếu (path->curr) {
		ix = path->curr;
		nếu (flags & EXT2_EXTENT_INSERT_AFTER) {
			ix++;
			path->left--;
		}
	} không_thì {
		ix = EXT_FIRST_INDEX(eh);
		path->left = -1;
	}

	path->curr = ix;

	nếu (path->left >= 0)
		memmove(ix + 1, ix,
			(path->left+1) * sizeof(struct ext3_extent_idx));
	path->left++;
	path->entries++;

	eh = (struct ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	nếu (retval)
		goto errout;

	retval = update_path(handle);
	nếu (retval)
		goto errout;

	trả_về 0;

errout:
	ext2fs_extent_delete(handle, 0);
	trả_về retval;
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
				 blk64_t logical, blk64_t physical, số_nguyên flags)
{
	errcode_t		ec, retval = 0;
	số_nguyên			mapped = 1; /* logical is mapped? */
	số_nguyên			orig_height;
	số_nguyên			extent_uninit = 0;
	số_nguyên			prev_uninit = 0;
	số_nguyên			next_uninit = 0;
	số_nguyên			new_uninit = 0;
	số_nguyên			max_len = EXT_INIT_MAX_LEN;
	số_nguyên			has_prev, has_next;
	blk64_t			orig_lblk;
	struct extent_path	*path;
	struct ext2fs_extent	extent, next_extent, prev_extent;
	struct ext2fs_extent	newextent;
	struct ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	printf("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	nếu (!(handle->fs->flags & EXT2_FLAG_RW))
		trả_về EXT2_ET_RO_FILSYS;

	nếu (!handle->path)
		trả_về EXT2_ET_NO_CURRENT_NODE;

	path = handle->path + handle->level;

	nếu (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	nếu (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		nếu (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	nếu ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_insert(handle, 0, &newextent);
		trả_về retval;
	}

	/* save our original location in the extent tree */
	nếu ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		nếu (retval != EXT2_ET_NO_CURRENT_NODE)
			trả_về retval;
		memset(&extent, 0, sizeof(extent));
	}
	nếu ((retval = ext2fs_extent_get_info(handle, &info)))
		trả_về retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	nếu (retval) {
		nếu (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			nếu (!physical) {
#ifdef DEBUG
				printf("block %llu already unmapped\n",
					logical);
#endif
				goto done;
			}
		} không_thì
			goto done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	nếu (retval)
		goto done;
	nếu (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	nếu (retval) {
		has_next = 0;
		nếu (retval != EXT2_ET_EXTENT_NO_NEXT)
			goto done;
	} không_thì {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		nếu (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	nếu (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		goto done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	nếu (retval) {
		has_prev = 0;
		nếu (retval != EXT2_ET_EXTENT_NO_PREV)
			goto done;
	} không_thì {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		nếu (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	nếu (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		goto done;

	/* check if already pointing to the requested physical */
	nếu (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		printf("physical block (at %llu) unchanged\n", logical);
#endif
		goto done;
	}

	nếu (!mapped) {
#ifdef DEBUG
		printf("mapping unmapped logical block %llu\n", logical);
#endif
		nếu ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((số_nguyên) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} không_thì nếu ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((số_nguyên) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} không_thì nếu (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((số_nguyên) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			nếu (retval)
				goto done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} không_thì nếu (logical < extent.e_lblk)
			retval = ext2fs_extent_insert(handle, 0, &newextent);
		không_thì
			retval = ext2fs_extent_insert(handle,
				      EXT2_EXTENT_INSERT_AFTER, &newextent);
		nếu (retval)
			goto done;
		retval = ext2fs_extent_fix_parents(handle);
		nếu (retval)
			goto done;
	} không_thì nếu ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		printf("(re/un)mapping only block in extent\n");
#endif
		nếu (physical) {
			nếu (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((số_nguyên) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				nếu (retval)
					goto done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				nếu (retval)
					goto done;
				goto delete_node;

			} không_thì
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} không_thì {
		delete_node:
			retval = ext2fs_extent_delete(handle, 0);
			nếu (retval)
				goto done;
			ec = ext2fs_extent_fix_parents(handle);
			nếu (ec != EXT2_ET_NO_CURRENT_NODE)
				retval = ec;
		}

		nếu (retval)
			goto done;
	} không_thì nếu (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		printf("(re/un)mapping last block in extent\n");
#endif
		nếu (physical) {
			nếu (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((số_nguyên) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				nếu (retval)
					goto done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				nếu (retval)
					goto done;
			} không_thì
				retval = ext2fs_extent_insert(handle,
				      EXT2_EXTENT_INSERT_AFTER, &newextent);
			nếu (retval)
				goto done;
			retval = ext2fs_extent_fix_parents(handle);
			nếu (retval)
				goto done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			nếu (retval)
				goto done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			nếu (retval)
				goto done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		nếu (retval)
			goto done;
	} không_thì nếu (logical == extent.e_lblk) {
#ifdef DEBUG
		printf("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		nếu (retval)
			goto done;
		retval = ext2fs_extent_fix_parents(handle);
		nếu (retval)
			goto done;
		nếu (physical) {
			nếu (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((số_nguyên) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				nếu (retval)
					goto done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} không_thì
				retval = ext2fs_extent_insert(handle,
							      0, &newextent);
			nếu (retval)
				goto done;
			retval = ext2fs_extent_fix_parents(handle);
			nếu (retval)
				goto done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			nếu (retval)
				goto done;
		}
	} không_thì {
		__u32	save_length;
		blk64_t	save_lblk;
		struct ext2fs_extent save_extent;
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
		nếu (retval)
			goto done;
		/* insert our new extent, if any */
		nếu (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_insert(handle,
					EXT2_EXTENT_INSERT_AFTER, &newextent);
			nếu (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				nếu (r2 == 0)
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
		nếu (retval) {
			nếu (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				nếu (r2 == 0)
					(void)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			nếu (r2 == 0)
				(void)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			goto done;
		}
	}

done:
	/* get handle back to its position */
	nếu (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	trả_về retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, số_nguyên flags)
{
	struct extent_path		*path;
	char 				*cp;
	struct ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	nếu (!(handle->fs->flags & EXT2_FLAG_RW))
		trả_về EXT2_ET_RO_FILSYS;

	nếu (!handle->path)
		trả_về EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	{
		struct ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		nếu (retval == 0) {
			printf("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	nếu (!path->curr)
		trả_về EXT2_ET_NO_CURRENT_NODE;

	cp = path->curr;

	/* Sanity check before memmove() */
	nếu (path->left < 0)
		trả_về EXT2_ET_EXTENT_LEAF_BAD;

	nếu (path->left) {
		memmove(cp, cp + sizeof(struct ext3_extent_idx),
			path->left * sizeof(struct ext3_extent_idx));
		path->left--;
	} không_thì {
		struct ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	nếu (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	nếu (path->entries == 0 && handle->level) {
		nếu (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			struct ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			nếu (retval)
				trả_về retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} không_thì {
		eh = (struct ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		nếu ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	trả_về retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 struct ext2_extent_info *info)
{
	struct extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, sizeof(struct ext2_extent_info));

	path = handle->path + handle->level;
	nếu (path) {
		nếu (path->curr)
			info->curr_entry = ((char *) path->curr - path->buf) /
				sizeof(struct ext3_extent_idx);
		không_thì
			info->curr_entry = 0;
		info->num_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			sizeof(struct ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	trả_về 0;
}

size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	size_t iblock_sz = sizeof(((struct ext2_inode *)NULL)->i_block);
	size_t iblock_extents = (iblock_sz - sizeof(struct ext3_extent_header)) /
				sizeof(struct ext3_extent);
	size_t extents_per_block = (handle->fs->blocksize -
				    sizeof(struct ext3_extent_header)) /
				   sizeof(struct ext3_extent);
	static unsigned số_nguyên last_blocksize = 0;
	static size_t last_result = 0;

	nếu (last_blocksize && last_blocksize == handle->fs->blocksize)
		trả_về last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	trả_về last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       struct ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	struct ext2fs_extent	extent;
	errcode_t		errcode;
	số_nguyên    			save_flags = fs->flags;

	nếu (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		trả_về 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	nếu (errcode) {
		nếu (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		trả_về errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ROOT, &extent);
	nếu (errcode)
		goto out;

	do {
		/* Skip to the end of a block of leaf nodes */
		nếu (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			nếu (errcode)
				break;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		nếu (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} trong_khi (errcode == 0);

out:
	/* Ok if we run off the end */
	nếu (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	trả_về errcode;
}

errcode_t ext2fs_decode_extent(struct ext2fs_extent *to, void *addr, số_nguyên len)
{
	struct ext3_extent *from = (struct ext3_extent *)addr;

	nếu (len != sizeof(struct ext3_extent))
		trả_về EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	nếu (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	trả_về 0;
}

errcode_t ext2fs_count_blocks(ext2_filsys fs, ext2_ino_t ino,
			      struct ext2_inode *inode, blk64_t *ret_count)
{
	ext2_extent_handle_t	handle = NULL;
	struct ext2fs_extent	extent;
	errcode_t		errcode;
	số_nguyên			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	nếu (errcode)
		goto out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ROOT, &extent);
	nếu (errcode)
		goto out;

	errcode = ext2fs_get_array(handle->max_depth, sizeof(blk64_t),
				   &intermediate_nodes);
	nếu (errcode)
		goto out;

	blkcount = handle->level;
	trong_khi (!errcode) {
		nếu (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			cho (i = 0; i < handle->level; i++) {
				nếu (intermediate_nodes[i] !=
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

	trả_về 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
const char *debug_prog_name = "tst_extents";

#endif

