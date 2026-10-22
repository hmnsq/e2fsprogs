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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ܐܢ HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ܐܢ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "e2image.h"

#undef DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

بنية extent_path {
	حرف		*buf;
	صحيح		entries;
	صحيح		max_entries;
	صحيح		left;
	صحيح		visit_num;
	صحيح		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	فراغ		*curr;
};


بنية ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	بنية ext2_inode	*inode;
	بنية ext2_inode	inodebuf;
	صحيح			type;
	صحيح			level;
	صحيح			max_depth;
	صحيح			max_paths;
	بنية extent_path	*path;
};

بنية ext2_extent_path {
	errcode_t		magic;
	صحيح			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
ساكن فراغ dbg_show_header(بنية ext3_extent_header *eh)
{
	printf("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

ساكن فراغ dbg_show_index(بنية ext3_extent_idx *ix)
{
	printf("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

ساكن فراغ dbg_show_extent(بنية ext3_extent *ex)
{
	printf("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

ساكن فراغ dbg_print_extent(حرف *desc, بنية ext2fs_extent *extent)
{
	ܐܢ (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ܐܢ (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	ܐܢ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ܐܢ (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	ܐܢ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

ساكن فراغ dump_path(ثابت حرف *tag, بنية ext2_extent_handle *handle,
		      بنية extent_path *path)
{
	بنية extent_path *ppp = path;
	printf("%s: level=%d\n", tag, handle->level);

	ܥܒܕ {
		printf("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->left, ppp->visit_num, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (فراغ *)ppp->buf);
		printf("  ");
		dbg_show_header((بنية ext3_extent_header *)ppp->buf);
		ܐܢ (ppp->curr) {
			printf("  ");
			dbg_show_index(ppp->curr);
			printf("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} ܟܡܐ (ppp >= handle->path);
	fflush(stdout);

	ܦܢܐ;
}

#ܐܚܪܢܐ
#define dbg_show_header(eh) ܥܒܕ { } ܟܡܐ (0)
#define dbg_show_index(ix) ܥܒܕ { } ܟܡܐ (0)
#define dbg_show_extent(ex) ܥܒܕ { } ܟܡܐ (0)
#define dbg_print_extent(desc, ex) ܥܒܕ { } ܟܡܐ (0)
#define dump_path(tag, handle, path) ܥܒܕ { } ܟܡܐ (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(فراغ *ptr, صحيح size)
{
	صحيح eh_max, entry_size;
	بنية ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	ܐܢ (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		ܦܢܐ EXT2_ET_EXTENT_HEADER_BAD;
	ܐܢ (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		ܦܢܐ EXT2_ET_EXTENT_HEADER_BAD;
	ܐܢ (eh->eh_depth == 0)
		entry_size = حجم(بنية ext3_extent);
	ܐܚܪܢܐ
		entry_size = حجم(بنية ext3_extent_idx);

	eh_max = (size - حجم(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	ܐܢ ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		ܦܢܐ EXT2_ET_EXTENT_HEADER_BAD;

	ܦܢܐ 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
فراغ ext2fs_extent_free(ext2_extent_handle_t handle)
{
	صحيح			i;

	ܐܢ (!handle)
		ܦܢܐ;

	ܐܢ (handle->path) {
		لكل (i = 1; i < handle->max_paths; i++) {
			ܐܢ (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	ܦܢܐ ext2fs_extent_open2(fs, ino, عدم, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    بنية ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	بنية ext2_extent_handle	*handle;
	errcode_t			retval;
	صحيح				i;
	بنية ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ܐܢ (!inode)
		ܐܢ ((ino == 0) || (ino > fs->super->s_inodes_count))
			ܦܢܐ EXT2_ET_BAD_INODE_NUM;

	retval = ext2fs_get_mem(حجم(بنية ext2_extent_handle), &handle);
	ܐܢ (retval)
		ܦܢܐ retval;
	memset(handle, 0, حجم(بنية ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	ܐܢ (inode) {
		handle->inode = inode;
	} ܐܚܪܢܐ {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		ܐܢ (retval)
			اذهب errout;
	}

	eh = (بنية ext3_extent_header *) &handle->inode->i_block[0];

	لكل (i=0; i < EXT2_N_BLOCKS; i++)
		ܐܢ (handle->inode->i_block[i])
			اكسر;
	ܐܢ (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (حجم(handle->inode->i_block) - حجم(*eh)) /
			حجم(بنية ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	ܐܢ (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		اذهب errout;
	}

	retval = ext2fs_extent_header_verify(eh, حجم(handle->inode->i_block));
	ܐܢ (retval)
		اذهب errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    حجم(بنية extent_path),
				    &handle->path);
	handle->path[0].buf = (حرف *) handle->inode->i_block;

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
	ܦܢܐ 0;

errout:
	ext2fs_extent_free(handle);
	ܦܢܐ retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    صحيح flags, بنية ext2fs_extent *extent)
{
	بنية extent_path	*path, *newpath, *tp;
	بنية ext3_extent_header	*eh;
	بنية ext3_extent_idx		*ix = 0;
	بنية ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	صحيح				orig_op, op, l;
	صحيح				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ܐܢ (!handle->path)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	ܐܢ ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		ܐܢ (handle->level < handle->max_depth) {
			/* interior node */
			ܐܢ (path->visit_num == 0) {
				path->visit_num++;
				op = EXT2_EXTENT_DOWN;
			} ܐܚܪܢܐ ܐܢ (path->left > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			ܐܚܪܢܐ ܐܢ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ܐܚܪܢܐ
				ܦܢܐ EXT2_ET_EXTENT_NO_NEXT;
		} ܐܚܪܢܐ {
			/* leaf node */
			ܐܢ (path->left > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			ܐܚܪܢܐ ܐܢ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ܐܚܪܢܐ
				ܦܢܐ EXT2_ET_EXTENT_NO_NEXT;
		}
		ܐܢ (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	ܐܢ ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		ܐܢ (handle->level < handle->max_depth) {
			/* interior node */
			ܐܢ (path->visit_num > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} ܐܚܪܢܐ ܐܢ (path->left < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			ܐܚܪܢܐ ܐܢ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ܐܚܪܢܐ
				ܦܢܐ EXT2_ET_EXTENT_NO_PREV;
		} ܐܚܪܢܐ {
			/* leaf node */
			ܐܢ (path->left < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			ܐܚܪܢܐ ܐܢ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ܐܚܪܢܐ
				ܦܢܐ EXT2_ET_EXTENT_NO_PREV;
		}
		ܐܢ (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	ܐܢ (orig_op == EXT2_EXTENT_LAST_LEAF) {
		ܐܢ ((handle->level < handle->max_depth) &&
		    (path->left == 0))
			op = EXT2_EXTENT_DOWN;
		ܐܚܪܢܐ
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		printf("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	بدل (op) {
	حالة EXT2_EXTENT_CURRENT:
		ix = path->curr;
		اكسر;
	حالة EXT2_EXTENT_ROOT:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	حالة EXT2_EXTENT_FIRST_SIB:
		path->left = path->entries;
		path->curr = 0;
		/* fallthrough */
	حالة EXT2_EXTENT_NEXT_SIB:
		ܐܢ (path->left <= 0)
			ܦܢܐ EXT2_ET_EXTENT_NO_NEXT;
		ܐܢ (path->curr) {
			ix = path->curr;
			ix++;
		} ܐܚܪܢܐ {
			eh = (بنية ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->left--;
		path->curr = ix;
		path->visit_num = 0;
		اكسر;
	حالة EXT2_EXTENT_PREV_SIB:
		ܐܢ (!path->curr ||
		    path->left+1 >= path->entries)
			ܦܢܐ EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->left++;
		ܐܢ (handle->level < handle->max_depth)
			path->visit_num = 1;
		اكسر;
	حالة EXT2_EXTENT_LAST_SIB:
		eh = (بنية ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->left = 0;
		path->visit_num = 0;
		اكسر;
	حالة EXT2_EXTENT_UP:
		ܐܢ (handle->level <= 0)
			ܦܢܐ EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		ܐܢ ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_num = 0;
		اكسر;
	حالة EXT2_EXTENT_DOWN:
	حالة EXT2_EXTENT_DOWN_AND_LAST:
		ܐܢ (!path->curr ||(handle->level >= handle->max_depth))
			ܦܢܐ EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		ܐܢ (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			ܐܢ (retval)
				ܦܢܐ retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		لكل (l = handle->level, tp = path; l > 0; l--, tp--) {
			ܐܢ (blk == tp->blk)
				ܦܢܐ EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		ܐܢ ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		ܐܚܪܢܐ {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			ܐܢ (retval)
				ܦܢܐ retval;
		}
		handle->level++;

		eh = (بنية ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		ܐܢ (retval) {
			handle->level--;
			ܦܢܐ retval;
		}

		ܐܢ (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->left = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		ܐܢ (newpath->left <= 0)
			ܦܢܐ EXT2_ET_EXTENT_NO_DOWN;

		ܐܢ (path->left > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} ܐܚܪܢܐ
			newpath->end_blk = path->end_blk;

		path = newpath;
		ܐܢ (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((بنية ext3_extent_header *) eh);
			path->curr = ix;
			path->left = path->entries - 1;
			path->visit_num = 0;
		} ܐܚܪܢܐ {
			ix = EXT_LAST_INDEX((بنية ext3_extent_header *) eh);
			path->curr = ix;
			path->left = 0;
			ܐܢ (handle->level < handle->max_depth)
				path->visit_num = 1;
		}
#ifdef DEBUG_GET_EXTENT
		printf("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		اكسر;
	افتراضي:
		ܦܢܐ EXT2_ET_OP_NOT_SUPPORTED;
	}

	ܐܢ (!ix)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	printf("(Left %d)\n", path->left);
#endif

	ܐܢ (handle->level == handle->max_depth) {
		ex = (بنية ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		ܐܢ (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} ܐܚܪܢܐ {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		ܐܢ (path->left > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} ܐܚܪܢܐ
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	ܐܢ (path->visit_num)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	ܐܢ (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		اذهب retry;

	ܐܢ ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->left != 0)))
		اذهب retry;

	ܐܢ (failed_csum)
		ܦܢܐ EXT2_ET_EXTENT_CSUM_INVALID;

	ܦܢܐ 0;
}

ساكن errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent_header	*eh;

	ܐܢ (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} ܐܚܪܢܐ {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (بنية ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		ܐܢ (retval)
			ܦܢܐ retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	ܦܢܐ retval;
}

#ܐܢ 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	بنية ext2fs_extent	extent;
	بنية ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ܐܢ (retval)
		ܦܢܐ retval;

	retval = ext2fs_extent_get_info(handle, &info);
	ܐܢ (retval)
		ܦܢܐ retval;

	retval = ext2fs_get_mem(حجم(بنية ext2_extent_path), &save_path);
	ܐܢ (retval)
		ܦܢܐ retval;
	memset(save_path, 0, حجم(بنية ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	ܦܢܐ 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	ܦܢܐ 0;
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
			      صحيح leaf_level, blk64_t blk)
{
	بنية ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ROOT, &extent);
	ܐܢ (retval) {
		ܐܢ (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		ܦܢܐ retval;
	}

	ܐܢ (leaf_level > handle->max_depth) {
#ifdef DEBUG
		printf("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		ܦܢܐ EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	printf("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	ܟܡܐ (1) {
		ܐܢ (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			ܐܢ ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				ܦܢܐ 0;
			ܐܢ (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				ܦܢܐ EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			ܐܢ (retval == EXT2_ET_EXTENT_NO_NEXT)
				ܦܢܐ EXT2_ET_EXTENT_NOT_FOUND;
			ܐܢ (retval)
				ܦܢܐ retval;
			استمر;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		ܐܢ (retval == EXT2_ET_EXTENT_NO_NEXT)
			اذهب go_down;
		ܐܢ (retval)
			ܦܢܐ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		ܐܢ (blk == extent.e_lblk)
			اذهب go_down;
		ܐܢ (blk > extent.e_lblk)
			استمر;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		ܐܢ (retval)
			ܦܢܐ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		ܐܢ (retval)
			ܦܢܐ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	ܦܢܐ ext2fs_extent_goto2(handle, 0, blk);
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
	صحيح				retval = 0;
	صحيح				orig_height;
	blk64_t				start;
	بنية extent_path		*path;
	بنية ext2fs_extent		extent;
	بنية ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ܐܢ (!(handle->fs->flags & EXT2_FLAG_RW))
		ܦܢܐ EXT2_ET_RO_FILSYS;

	ܐܢ (!handle->path)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

	path = handle->path + handle->level;
	ܐܢ (!path->curr)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ܐܢ (retval)
		اذهب done;

	/* modified node's start block */
	start = extent.e_lblk;

	ܐܢ ((retval = ext2fs_extent_get_info(handle, &info)))
		ܦܢܐ retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	ܟܡܐ (handle->level > 0 &&
	       (path->left == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		ܐܢ (retval)
			اذهب done;
		ܐܢ (extent.e_lblk == start)
			اكسر;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ܐܢ (retval)
			اذهب done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	ܦܢܐ retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				صحيح flags EXT2FS_ATTR((unused)),
				بنية ext2fs_extent *extent)
{
	بنية extent_path		*path;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ܐܢ (!(handle->fs->flags & EXT2_FLAG_RW))
		ܦܢܐ EXT2_ET_RO_FILSYS;

	ܐܢ (!handle->path)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

	path = handle->path + handle->level;
	ܐܢ (!path->curr)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	printf("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	ܐܢ (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ܐܢ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			ܐܢ (extent->e_len > EXT_UNINIT_MAX_LEN)
				ܦܢܐ EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} ܐܚܪܢܐ {
			ܐܢ (extent->e_len > EXT_INIT_MAX_LEN)
				ܦܢܐ EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} ܐܚܪܢܐ {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	ܦܢܐ 0;
}

ساكن صحيح splitting_at_eof(بنية ext2_extent_handle *handle,
			    بنية extent_path *path)
{
	بنية extent_path *ppp = path;
	dump_path(__func__, handle, path);

	ܐܢ (handle->level == 0)
		ܦܢܐ 0;

	ܥܒܕ {
		ܐܢ (ppp->left)
			ܦܢܐ 0;
		ppp--;
	} ܟܡܐ (ppp >= handle->path);

	ܦܢܐ 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
ساكن errcode_t extent_node_split(ext2_extent_handle_t handle,
				   صحيح expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_node_pblk;
	blk64_t				new_node_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	صحيح				orig_height;
	حرف				*block_buf = عدم;
	بنية ext2fs_extent		extent;
	بنية extent_path		*path, *newpath = 0;
	بنية ext3_extent_header	*eh, *neweh;
	صحيح				tocopy;
	صحيح				new_root = 0;
	بنية ext2_extent_info		info;
	صحيح				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ܐܢ (!(handle->fs->flags & EXT2_FLAG_RW))
		ܦܢܐ EXT2_ET_RO_FILSYS;

	ܐܢ (!handle->path)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	printf("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ܐܢ (retval)
		اذهب done;

	retval = ext2fs_extent_get_info(handle, &info);
	ܐܢ (retval)
		اذهب done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (بنية ext3_extent_header *) path->buf;
	ܐܢ (handle->level == handle->max_depth) {
		بنية ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} ܐܚܪܢܐ {
		بنية ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	ܐܢ (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		printf("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		ܐܢ (retval)
			اذهب done;

		retval = extent_node_split(handle, expand_allowed);
		ܐܢ (retval)
			اذهب done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		ܐܢ (retval)
			اذهب done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	ܐܢ (!path->curr)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (بنية ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	ܐܢ (handle->level == 0) {
		new_root = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    حجم(بنية extent_path),
					    &newpath);
		ܐܢ (retval)
			اذهب done;
	} ܐܚܪܢܐ {
		ܐܢ (no_balance)
			tocopy = 1;
		ܐܚܪܢܐ
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	printf("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	ܐܢ (!tocopy && !no_balance) {
#ifdef DEBUG
		printf("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		اذهب done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = malloc(handle->fs->blocksize);
	ܐܢ (!block_buf) {
		retval = ENOMEM;
		اذهب done;
	}

	ܐܢ (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_node_pblk);
	ܐܢ (retval)
		اذهب done;

#ifdef DEBUG
	printf("will copy to new node at block %lu\n",
	       (unsigned long) new_node_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (بنية ext3_extent_header *) block_buf;
	memcpy(neweh, eh, حجم(بنية ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 حجم(بنية ext3_extent_header)) /
				حجم(بنية ext3_extent));

	/* then the entries for the new block... */
	memcpy(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		حجم(بنية ext3_extent_idx) * tocopy);

	new_node_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	ܐܢ (retval)
		اذهب done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_node_pblk, 1,
					block_buf);

	ܐܢ (retval)
		اذهب done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	ܐܢ (handle->level == 0) {
		memcpy(newpath, path,
		       حجم(بنية extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->left = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} ܐܚܪܢܐ {
		path->entries -= tocopy;
		path->left -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	ܐܢ (retval)
		اذهب done;

	/* now go up and insert/replace index for new node we created */
	ܐܢ (new_root) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		ܐܢ (retval)
			اذهب done;

		extent.e_lblk = new_node_start;
		extent.e_pblk = new_node_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ܐܢ (retval)
			اذهب done;
	} ܐܚܪܢܐ {
		__u32 new_node_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_node_length = new_node_start - extent.e_lblk;
		extent.e_len -= new_node_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ܐܢ (retval)
			اذهب done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_node_start;
		extent.e_pblk = new_node_pblk;
		extent.e_len = new_node_length;
		retval = ext2fs_extent_insert(handle, EXT2_EXTENT_INSERT_AFTER, &extent);
		ܐܢ (retval)
			اذهب done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	ܐܢ (retval)
		اذهب done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	ܐܢ (retval)
		اذهب done;

done:
	ܐܢ (newpath)
		ext2fs_free_mem(&newpath);
	حرر(block_buf);

	ܦܢܐ retval;
}

errcode_t ext2fs_extent_node_split(ext2_extent_handle_t handle)
{
	ܦܢܐ extent_node_split(handle, 0);
}

errcode_t ext2fs_extent_insert(ext2_extent_handle_t handle, صحيح flags,
				      بنية ext2fs_extent *extent)
{
	بنية extent_path		*path;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ܐܢ (!(handle->fs->flags & EXT2_FLAG_RW))
		ܦܢܐ EXT2_ET_RO_FILSYS;

	ܐܢ (!handle->path)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	printf("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	ܐܢ (path->entries >= path->max_entries) {
		ܐܢ (flags & EXT2_EXTENT_INSERT_NOSPLIT) {
			ܦܢܐ EXT2_ET_CANT_INSERT_EXTENT;
		} ܐܚܪܢܐ {
#ifdef DEBUG
			printf("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_node_split(handle, 1);
			ܐܢ (retval)
				ܦܢܐ retval;
			path = handle->path + handle->level;
		}
	}

	eh = (بنية ext3_extent_header *) path->buf;
	ܐܢ (path->curr) {
		ix = path->curr;
		ܐܢ (flags & EXT2_EXTENT_INSERT_AFTER) {
			ix++;
			path->left--;
		}
	} ܐܚܪܢܐ {
		ix = EXT_FIRST_INDEX(eh);
		path->left = -1;
	}

	path->curr = ix;

	ܐܢ (path->left >= 0)
		memmove(ix + 1, ix,
			(path->left+1) * حجم(بنية ext3_extent_idx));
	path->left++;
	path->entries++;

	eh = (بنية ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	ܐܢ (retval)
		اذهب errout;

	retval = update_path(handle);
	ܐܢ (retval)
		اذهب errout;

	ܦܢܐ 0;

errout:
	ext2fs_extent_delete(handle, 0);
	ܦܢܐ retval;
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
				 blk64_t logical, blk64_t physical, صحيح flags)
{
	errcode_t		ec, retval = 0;
	صحيح			mapped = 1; /* logical is mapped? */
	صحيح			orig_height;
	صحيح			extent_uninit = 0;
	صحيح			prev_uninit = 0;
	صحيح			next_uninit = 0;
	صحيح			new_uninit = 0;
	صحيح			max_len = EXT_INIT_MAX_LEN;
	صحيح			has_prev, has_next;
	blk64_t			orig_lblk;
	بنية extent_path	*path;
	بنية ext2fs_extent	extent, next_extent, prev_extent;
	بنية ext2fs_extent	newextent;
	بنية ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	printf("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	ܐܢ (!(handle->fs->flags & EXT2_FLAG_RW))
		ܦܢܐ EXT2_ET_RO_FILSYS;

	ܐܢ (!handle->path)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

	path = handle->path + handle->level;

	ܐܢ (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	ܐܢ (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		ܐܢ (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	ܐܢ ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_insert(handle, 0, &newextent);
		ܦܢܐ retval;
	}

	/* save our original location in the extent tree */
	ܐܢ ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		ܐܢ (retval != EXT2_ET_NO_CURRENT_NODE)
			ܦܢܐ retval;
		memset(&extent, 0, حجم(extent));
	}
	ܐܢ ((retval = ext2fs_extent_get_info(handle, &info)))
		ܦܢܐ retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	ܐܢ (retval) {
		ܐܢ (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			ܐܢ (!physical) {
#ifdef DEBUG
				printf("block %llu already unmapped\n",
					logical);
#endif
				اذهب done;
			}
		} ܐܚܪܢܐ
			اذهب done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ܐܢ (retval)
		اذهب done;
	ܐܢ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	ܐܢ (retval) {
		has_next = 0;
		ܐܢ (retval != EXT2_ET_EXTENT_NO_NEXT)
			اذهب done;
	} ܐܚܪܢܐ {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		ܐܢ (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ܐܢ (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		اذهب done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	ܐܢ (retval) {
		has_prev = 0;
		ܐܢ (retval != EXT2_ET_EXTENT_NO_PREV)
			اذهب done;
	} ܐܚܪܢܐ {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		ܐܢ (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ܐܢ (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		اذهب done;

	/* check if already pointing to the requested physical */
	ܐܢ (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		printf("physical block (at %llu) unchanged\n", logical);
#endif
		اذهب done;
	}

	ܐܢ (!mapped) {
#ifdef DEBUG
		printf("mapping unmapped logical block %llu\n", logical);
#endif
		ܐܢ ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((صحيح) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ܐܚܪܢܐ ܐܢ ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((صحيح) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ܐܚܪܢܐ ܐܢ (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((صحيح) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			ܐܢ (retval)
				اذهب done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} ܐܚܪܢܐ ܐܢ (logical < extent.e_lblk)
			retval = ext2fs_extent_insert(handle, 0, &newextent);
		ܐܚܪܢܐ
			retval = ext2fs_extent_insert(handle,
				      EXT2_EXTENT_INSERT_AFTER, &newextent);
		ܐܢ (retval)
			اذهب done;
		retval = ext2fs_extent_fix_parents(handle);
		ܐܢ (retval)
			اذهب done;
	} ܐܚܪܢܐ ܐܢ ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		printf("(re/un)mapping only block in extent\n");
#endif
		ܐܢ (physical) {
			ܐܢ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحيح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				ܐܢ (retval)
					اذهب done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				ܐܢ (retval)
					اذهب done;
				اذهب delete_node;

			} ܐܚܪܢܐ
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} ܐܚܪܢܐ {
		delete_node:
			retval = ext2fs_extent_delete(handle, 0);
			ܐܢ (retval)
				اذهب done;
			ec = ext2fs_extent_fix_parents(handle);
			ܐܢ (ec != EXT2_ET_NO_CURRENT_NODE)
				retval = ec;
		}

		ܐܢ (retval)
			اذهب done;
	} ܐܚܪܢܐ ܐܢ (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		printf("(re/un)mapping last block in extent\n");
#endif
		ܐܢ (physical) {
			ܐܢ (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((صحيح) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				ܐܢ (retval)
					اذهب done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				ܐܢ (retval)
					اذهب done;
			} ܐܚܪܢܐ
				retval = ext2fs_extent_insert(handle,
				      EXT2_EXTENT_INSERT_AFTER, &newextent);
			ܐܢ (retval)
				اذهب done;
			retval = ext2fs_extent_fix_parents(handle);
			ܐܢ (retval)
				اذهب done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			ܐܢ (retval)
				اذهب done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			ܐܢ (retval)
				اذهب done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ܐܢ (retval)
			اذهب done;
	} ܐܚܪܢܐ ܐܢ (logical == extent.e_lblk) {
#ifdef DEBUG
		printf("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ܐܢ (retval)
			اذهب done;
		retval = ext2fs_extent_fix_parents(handle);
		ܐܢ (retval)
			اذهب done;
		ܐܢ (physical) {
			ܐܢ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحيح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				ܐܢ (retval)
					اذهب done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} ܐܚܪܢܐ
				retval = ext2fs_extent_insert(handle,
							      0, &newextent);
			ܐܢ (retval)
				اذهب done;
			retval = ext2fs_extent_fix_parents(handle);
			ܐܢ (retval)
				اذهب done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			ܐܢ (retval)
				اذهب done;
		}
	} ܐܚܪܢܐ {
		__u32	save_length;
		blk64_t	save_lblk;
		بنية ext2fs_extent save_extent;
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
		ܐܢ (retval)
			اذهب done;
		/* insert our new extent, if any */
		ܐܢ (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_insert(handle,
					EXT2_EXTENT_INSERT_AFTER, &newextent);
			ܐܢ (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				ܐܢ (r2 == 0)
					(فراغ)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				اذهب done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_length - extent.e_len - 1;
		retval = ext2fs_extent_insert(handle,
				EXT2_EXTENT_INSERT_AFTER, &extent);
		ܐܢ (retval) {
			ܐܢ (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				ܐܢ (r2 == 0)
					(فراغ)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			ܐܢ (r2 == 0)
				(فراغ)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			اذهب done;
		}
	}

done:
	/* get handle back to its position */
	ܐܢ (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	ܦܢܐ retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحيح flags)
{
	بنية extent_path		*path;
	حرف 				*cp;
	بنية ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ܐܢ (!(handle->fs->flags & EXT2_FLAG_RW))
		ܦܢܐ EXT2_ET_RO_FILSYS;

	ܐܢ (!handle->path)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

#ifdef DEBUG
	{
		بنية ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		ܐܢ (retval == 0) {
			printf("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	ܐܢ (!path->curr)
		ܦܢܐ EXT2_ET_NO_CURRENT_NODE;

	cp = path->curr;

	/* Sanity check before memmove() */
	ܐܢ (path->left < 0)
		ܦܢܐ EXT2_ET_EXTENT_LEAF_BAD;

	ܐܢ (path->left) {
		memmove(cp, cp + حجم(بنية ext3_extent_idx),
			path->left * حجم(بنية ext3_extent_idx));
		path->left--;
	} ܐܚܪܢܐ {
		بنية ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	ܐܢ (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	ܐܢ (path->entries == 0 && handle->level) {
		ܐܢ (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			بنية ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			ܐܢ (retval)
				ܦܢܐ retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} ܐܚܪܢܐ {
		eh = (بنية ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		ܐܢ ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	ܦܢܐ retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 بنية ext2_extent_info *info)
{
	بنية extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, حجم(بنية ext2_extent_info));

	path = handle->path + handle->level;
	ܐܢ (path) {
		ܐܢ (path->curr)
			info->curr_entry = ((حرف *) path->curr - path->buf) /
				حجم(بنية ext3_extent_idx);
		ܐܚܪܢܐ
			info->curr_entry = 0;
		info->num_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			حجم(بنية ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	ܦܢܐ 0;
}

size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	size_t iblock_sz = حجم(((بنية ext2_inode *)عدم)->i_block);
	size_t iblock_extents = (iblock_sz - حجم(بنية ext3_extent_header)) /
				حجم(بنية ext3_extent);
	size_t extents_per_block = (handle->fs->blocksize -
				    حجم(بنية ext3_extent_header)) /
				   حجم(بنية ext3_extent);
	ساكن unsigned صحيح last_blocksize = 0;
	ساكن size_t last_result = 0;

	ܐܢ (last_blocksize && last_blocksize == handle->fs->blocksize)
		ܦܢܐ last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	ܦܢܐ last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       بنية ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	بنية ext2fs_extent	extent;
	errcode_t		errcode;
	صحيح    			save_flags = fs->flags;

	ܐܢ (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		ܦܢܐ 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ܐܢ (errcode) {
		ܐܢ (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		ܦܢܐ errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ROOT, &extent);
	ܐܢ (errcode)
		اذهب out;

	ܥܒܕ {
		/* Skip to the end of a block of leaf nodes */
		ܐܢ (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			ܐܢ (errcode)
				اكسر;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		ܐܢ (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} ܟܡܐ (errcode == 0);

out:
	/* Ok if we run off the end */
	ܐܢ (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	ܦܢܐ errcode;
}

errcode_t ext2fs_decode_extent(بنية ext2fs_extent *to, فراغ *addr, صحيح len)
{
	بنية ext3_extent *from = (بنية ext3_extent *)addr;

	ܐܢ (len != حجم(بنية ext3_extent))
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	ܐܢ (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	ܦܢܐ 0;
}

errcode_t ext2fs_count_blocks(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode, blk64_t *ret_count)
{
	ext2_extent_handle_t	handle = عدم;
	بنية ext2fs_extent	extent;
	errcode_t		errcode;
	صحيح			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ܐܢ (errcode)
		اذهب out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ROOT, &extent);
	ܐܢ (errcode)
		اذهب out;

	errcode = ext2fs_get_array(handle->max_depth, حجم(blk64_t),
				   &intermediate_nodes);
	ܐܢ (errcode)
		اذهب out;

	blkcount = handle->level;
	ܟܡܐ (!errcode) {
		ܐܢ (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			لكل (i = 0; i < handle->level; i++) {
				ܐܢ (intermediate_nodes[i] !=
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

	ܦܢܐ 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
ثابت حرف *debug_prog_name = "tst_extents";

#endif

