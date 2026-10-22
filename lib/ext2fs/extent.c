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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"
#شامل "e2image.h"

#undef DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

ساختار extent_path {
	نویسه		*buf;
	صحیح		entries;
	صحیح		max_entries;
	صحیح		چپ;
	صحیح		visit_رقم;
	صحیح		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	تهی		*curr;
};


ساختار ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	ساختار ext2_inode	*inode;
	ساختار ext2_inode	inodebuf;
	صحیح			type;
	صحیح			level;
	صحیح			max_depth;
	صحیح			max_paths;
	ساختار extent_path	*path;
};

ساختار ext2_extent_path {
	errcode_t		magic;
	صحیح			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
ایستا تهی dbg_show_header(ساختار ext3_extent_header *eh)
{
	printf("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

ایستا تهی dbg_show_index(ساختار ext3_extent_idx *ix)
{
	printf("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

ایستا تهی dbg_show_extent(ساختار ext3_extent *ex)
{
	printf("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

ایستا تهی dbg_print_extent(نویسه *desc, ساختار ext2fs_extent *extent)
{
	اگر (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	اگر (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

ایستا تهی dump_path(ثابت نویسه *tag, ساختار ext2_extent_handle *handle,
		      ساختار extent_path *path)
{
	ساختار extent_path *ppp = path;
	printf("%s: level=%d\n", tag, handle->level);

	انجامبده {
		printf("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->چپ, ppp->visit_رقم, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (تهی *)ppp->buf);
		printf("  ");
		dbg_show_header((ساختار ext3_extent_header *)ppp->buf);
		اگر (ppp->curr) {
			printf("  ");
			dbg_show_index(ppp->curr);
			printf("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} وقتی (ppp >= handle->path);
	fflush(stdout);

	بازگشت;
}

#وگرنه
#define dbg_show_header(eh) انجامبده { } وقتی (0)
#define dbg_show_index(ix) انجامبده { } وقتی (0)
#define dbg_show_extent(ex) انجامبده { } وقتی (0)
#define dbg_print_extent(desc, ex) انجامبده { } وقتی (0)
#define dump_path(tag, handle, path) انجامبده { } وقتی (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(تهی *ptr, صحیح size)
{
	صحیح eh_max, entry_size;
	ساختار ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	اگر (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		بازگشت EXT2_ET_EXTENT_HEADER_BAD;
	اگر (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		بازگشت EXT2_ET_EXTENT_HEADER_BAD;
	اگر (eh->eh_depth == 0)
		entry_size = اندازه(ساختار ext3_extent);
	وگرنه
		entry_size = اندازه(ساختار ext3_extent_idx);

	eh_max = (size - اندازه(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	اگر ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		بازگشت EXT2_ET_EXTENT_HEADER_BAD;

	بازگشت 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
تهی ext2fs_extent_free(ext2_extent_handle_t handle)
{
	صحیح			i;

	اگر (!handle)
		بازگشت;

	اگر (handle->path) {
		برای (i = 1; i < handle->max_paths; i++) {
			اگر (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	بازگشت ext2fs_extent_open2(fs, ino, پوچ, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    ساختار ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	ساختار ext2_extent_handle	*handle;
	errcode_t			retval;
	صحیح				i;
	ساختار ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!inode)
		اگر ((ino == 0) || (ino > fs->super->s_inodes_شمار))
			بازگشت EXT2_ET_BAD_INODE_رقم;

	retval = ext2fs_get_mem(اندازه(ساختار ext2_extent_handle), &handle);
	اگر (retval)
		بازگشت retval;
	memset(handle, 0, اندازه(ساختار ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	اگر (inode) {
		handle->inode = inode;
	} وگرنه {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		اگر (retval)
			برو errout;
	}

	eh = (ساختار ext3_extent_header *) &handle->inode->i_block[0];

	برای (i=0; i < EXT2_N_BLOCKS; i++)
		اگر (handle->inode->i_block[i])
			بشکن;
	اگر (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (اندازه(handle->inode->i_block) - اندازه(*eh)) /
			اندازه(ساختار ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	اگر (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		برو errout;
	}

	retval = ext2fs_extent_header_verify(eh, اندازه(handle->inode->i_block));
	اگر (retval)
		برو errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    اندازه(ساختار extent_path),
				    &handle->path);
	handle->path[0].buf = (نویسه *) handle->inode->i_block;

	handle->path[0].چپ = handle->path[0].entries =
		ext2fs_le16_to_cpu(eh->eh_entries);
	handle->path[0].max_entries = ext2fs_le16_to_cpu(eh->eh_max);
	handle->path[0].curr = 0;
	handle->path[0].end_blk =
		(EXT2_I_SIZE(handle->inode) + fs->blocksize - 1) >>
		 EXT2_BLOCK_SIZE_BITS(fs->super);
	handle->path[0].blk = 0;
	handle->path[0].visit_رقم = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	بازگشت 0;

errout:
	ext2fs_extent_free(handle);
	بازگشت retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    صحیح flags, ساختار ext2fs_extent *extent)
{
	ساختار extent_path	*path, *newpath, *tp;
	ساختار ext3_extent_header	*eh;
	ساختار ext3_extent_idx		*ix = 0;
	ساختار ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	صحیح				orig_op, op, l;
	صحیح				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!handle->path)
		بازگشت EXT2_ET_NO_CURRENT_گره;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	اگر ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		اگر (handle->level < handle->max_depth) {
			/* interior node */
			اگر (path->visit_رقم == 0) {
				path->visit_رقم++;
				op = EXT2_EXTENT_DOWN;
			} وگرنه اگر (path->چپ > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			وگرنه اگر (handle->level > 0)
				op = EXT2_EXTENT_UP;
			وگرنه
				بازگشت EXT2_ET_EXTENT_NO_NEXT;
		} وگرنه {
			/* leaf node */
			اگر (path->چپ > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			وگرنه اگر (handle->level > 0)
				op = EXT2_EXTENT_UP;
			وگرنه
				بازگشت EXT2_ET_EXTENT_NO_NEXT;
		}
		اگر (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	اگر ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		اگر (handle->level < handle->max_depth) {
			/* interior node */
			اگر (path->visit_رقم > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} وگرنه اگر (path->چپ < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			وگرنه اگر (handle->level > 0)
				op = EXT2_EXTENT_UP;
			وگرنه
				بازگشت EXT2_ET_EXTENT_NO_PREV;
		} وگرنه {
			/* leaf node */
			اگر (path->چپ < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			وگرنه اگر (handle->level > 0)
				op = EXT2_EXTENT_UP;
			وگرنه
				بازگشت EXT2_ET_EXTENT_NO_PREV;
		}
		اگر (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	اگر (orig_op == EXT2_EXTENT_LAST_LEAF) {
		اگر ((handle->level < handle->max_depth) &&
		    (path->چپ == 0))
			op = EXT2_EXTENT_DOWN;
		وگرنه
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		printf("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	انتخاب (op) {
	حالت EXT2_EXTENT_CURRENT:
		ix = path->curr;
		بشکن;
	حالت EXT2_EXTENT_ریشه:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	حالت EXT2_EXTENT_FIRST_SIB:
		path->چپ = path->entries;
		path->curr = 0;
		/* fallthrough */
	حالت EXT2_EXTENT_NEXT_SIB:
		اگر (path->چپ <= 0)
			بازگشت EXT2_ET_EXTENT_NO_NEXT;
		اگر (path->curr) {
			ix = path->curr;
			ix++;
		} وگرنه {
			eh = (ساختار ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->چپ--;
		path->curr = ix;
		path->visit_رقم = 0;
		بشکن;
	حالت EXT2_EXTENT_PREV_SIB:
		اگر (!path->curr ||
		    path->چپ+1 >= path->entries)
			بازگشت EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->چپ++;
		اگر (handle->level < handle->max_depth)
			path->visit_رقم = 1;
		بشکن;
	حالت EXT2_EXTENT_LAST_SIB:
		eh = (ساختار ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->چپ = 0;
		path->visit_رقم = 0;
		بشکن;
	حالت EXT2_EXTENT_UP:
		اگر (handle->level <= 0)
			بازگشت EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		اگر ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_رقم = 0;
		بشکن;
	حالت EXT2_EXTENT_DOWN:
	حالت EXT2_EXTENT_DOWN_AND_LAST:
		اگر (!path->curr ||(handle->level >= handle->max_depth))
			بازگشت EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		اگر (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			اگر (retval)
				بازگشت retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		برای (l = handle->level, tp = path; l > 0; l--, tp--) {
			اگر (blk == tp->blk)
				بازگشت EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		اگر ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		وگرنه {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			اگر (retval)
				بازگشت retval;
		}
		handle->level++;

		eh = (ساختار ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		اگر (retval) {
			handle->level--;
			بازگشت retval;
		}

		اگر (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->چپ = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		اگر (newpath->چپ <= 0)
			بازگشت EXT2_ET_EXTENT_NO_DOWN;

		اگر (path->چپ > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} وگرنه
			newpath->end_blk = path->end_blk;

		path = newpath;
		اگر (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((ساختار ext3_extent_header *) eh);
			path->curr = ix;
			path->چپ = path->entries - 1;
			path->visit_رقم = 0;
		} وگرنه {
			ix = EXT_LAST_INDEX((ساختار ext3_extent_header *) eh);
			path->curr = ix;
			path->چپ = 0;
			اگر (handle->level < handle->max_depth)
				path->visit_رقم = 1;
		}
#ifdef DEBUG_GET_EXTENT
		printf("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		بشکن;
	پیشفرض:
		بازگشت EXT2_ET_OP_NOT_SUPPORTED;
	}

	اگر (!ix)
		بازگشت EXT2_ET_NO_CURRENT_گره;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	printf("(Left %d)\n", path->چپ);
#endif

	اگر (handle->level == handle->max_depth) {
		ex = (ساختار ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		اگر (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} وگرنه {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		اگر (path->چپ > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} وگرنه
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	اگر (path->visit_رقم)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	اگر (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		برو retry;

	اگر ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->چپ != 0)))
		برو retry;

	اگر (failed_csum)
		بازگشت EXT2_ET_EXTENT_CSUM_INVALID;

	بازگشت 0;
}

ایستا errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	ساختار ext3_extent_idx		*ix;
	ساختار ext3_extent_header	*eh;

	اگر (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} وگرنه {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (ساختار ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		اگر (retval)
			بازگشت retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	بازگشت retval;
}

#اگر 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	ساختار ext2fs_extent	extent;
	ساختار ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	اگر (retval)
		بازگشت retval;

	retval = ext2fs_extent_get_info(handle, &info);
	اگر (retval)
		بازگشت retval;

	retval = ext2fs_get_mem(اندازه(ساختار ext2_extent_path), &save_path);
	اگر (retval)
		بازگشت retval;
	memset(save_path, 0, اندازه(ساختار ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	بازگشت 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	بازگشت 0;
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
			      صحیح leaf_level, blk64_t blk)
{
	ساختار ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ریشه, &extent);
	اگر (retval) {
		اگر (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		بازگشت retval;
	}

	اگر (leaf_level > handle->max_depth) {
#ifdef DEBUG
		printf("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		بازگشت EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	printf("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	وقتی (1) {
		اگر (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			اگر ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				بازگشت 0;
			اگر (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				بازگشت EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			اگر (retval == EXT2_ET_EXTENT_NO_NEXT)
				بازگشت EXT2_ET_EXTENT_NOT_FOUND;
			اگر (retval)
				بازگشت retval;
			ادامه;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		اگر (retval == EXT2_ET_EXTENT_NO_NEXT)
			برو go_down;
		اگر (retval)
			بازگشت retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		اگر (blk == extent.e_lblk)
			برو go_down;
		اگر (blk > extent.e_lblk)
			ادامه;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		اگر (retval)
			بازگشت retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		اگر (retval)
			بازگشت retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	بازگشت ext2fs_extent_goto2(handle, 0, blk);
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
	صحیح				retval = 0;
	صحیح				orig_height;
	blk64_t				start;
	ساختار extent_path		*path;
	ساختار ext2fs_extent		extent;
	ساختار ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

	اگر (!handle->path)
		بازگشت EXT2_ET_NO_CURRENT_گره;

	path = handle->path + handle->level;
	اگر (!path->curr)
		بازگشت EXT2_ET_NO_CURRENT_گره;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	اگر (retval)
		برو done;

	/* modified node's start block */
	start = extent.e_lblk;

	اگر ((retval = ext2fs_extent_get_info(handle, &info)))
		بازگشت retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	وقتی (handle->level > 0 &&
	       (path->چپ == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		اگر (retval)
			برو done;
		اگر (extent.e_lblk == start)
			بشکن;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			برو done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	بازگشت retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				صحیح flags EXT2FS_ATTR((unused)),
				ساختار ext2fs_extent *extent)
{
	ساختار extent_path		*path;
	ساختار ext3_extent_idx		*ix;
	ساختار ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

	اگر (!handle->path)
		بازگشت EXT2_ET_NO_CURRENT_گره;

	path = handle->path + handle->level;
	اگر (!path->curr)
		بازگشت EXT2_ET_NO_CURRENT_گره;

#ifdef DEBUG
	printf("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	اگر (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		اگر (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			اگر (extent->e_len > EXT_UNINIT_MAX_LEN)
				بازگشت EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} وگرنه {
			اگر (extent->e_len > EXT_INIT_MAX_LEN)
				بازگشت EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} وگرنه {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	بازگشت 0;
}

ایستا صحیح splitting_at_eof(ساختار ext2_extent_handle *handle,
			    ساختار extent_path *path)
{
	ساختار extent_path *ppp = path;
	dump_path(__func__, handle, path);

	اگر (handle->level == 0)
		بازگشت 0;

	انجامبده {
		اگر (ppp->چپ)
			بازگشت 0;
		ppp--;
	} وقتی (ppp >= handle->path);

	بازگشت 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
ایستا errcode_t extent_گره_split(ext2_extent_handle_t handle,
				   صحیح expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_گره_pblk;
	blk64_t				new_گره_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	صحیح				orig_height;
	نویسه				*block_buf = پوچ;
	ساختار ext2fs_extent		extent;
	ساختار extent_path		*path, *newpath = 0;
	ساختار ext3_extent_header	*eh, *neweh;
	صحیح				tocopy;
	صحیح				new_ریشه = 0;
	ساختار ext2_extent_info		info;
	صحیح				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

	اگر (!handle->path)
		بازگشت EXT2_ET_NO_CURRENT_گره;

#ifdef DEBUG
	printf("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	اگر (retval)
		برو done;

	retval = ext2fs_extent_get_info(handle, &info);
	اگر (retval)
		برو done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (ساختار ext3_extent_header *) path->buf;
	اگر (handle->level == handle->max_depth) {
		ساختار ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} وگرنه {
		ساختار ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	اگر (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		printf("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		اگر (retval)
			برو done;

		retval = extent_گره_split(handle, expand_allowed);
		اگر (retval)
			برو done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		اگر (retval)
			برو done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	اگر (!path->curr)
		بازگشت EXT2_ET_NO_CURRENT_گره;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (ساختار ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	اگر (handle->level == 0) {
		new_ریشه = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    اندازه(ساختار extent_path),
					    &newpath);
		اگر (retval)
			برو done;
	} وگرنه {
		اگر (no_balance)
			tocopy = 1;
		وگرنه
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	printf("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	اگر (!tocopy && !no_balance) {
#ifdef DEBUG
		printf("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		برو done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = malloc(handle->fs->blocksize);
	اگر (!block_buf) {
		retval = ENOMEM;
		برو done;
	}

	اگر (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_گره_pblk);
	اگر (retval)
		برو done;

#ifdef DEBUG
	printf("will copy to new node at block %lu\n",
	       (unsigned long) new_گره_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (ساختار ext3_extent_header *) block_buf;
	memcpy(neweh, eh, اندازه(ساختار ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 اندازه(ساختار ext3_extent_header)) /
				اندازه(ساختار ext3_extent));

	/* then the entries for the new block... */
	memcpy(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		اندازه(ساختار ext3_extent_idx) * tocopy);

	new_گره_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	اگر (retval)
		برو done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_گره_pblk, 1,
					block_buf);

	اگر (retval)
		برو done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	اگر (handle->level == 0) {
		memcpy(newpath, path,
		       اندازه(ساختار extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->چپ = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} وگرنه {
		path->entries -= tocopy;
		path->چپ -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	اگر (retval)
		برو done;

	/* now go up and insert/replace index for new node we created */
	اگر (new_ریشه) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		اگر (retval)
			برو done;

		extent.e_lblk = new_گره_start;
		extent.e_pblk = new_گره_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			برو done;
	} وگرنه {
		__u32 new_گره_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_گره_length = new_گره_start - extent.e_lblk;
		extent.e_len -= new_گره_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			برو done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_گره_start;
		extent.e_pblk = new_گره_pblk;
		extent.e_len = new_گره_length;
		retval = ext2fs_extent_درج(handle, EXT2_EXTENT_درج_AFTER, &extent);
		اگر (retval)
			برو done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	اگر (retval)
		برو done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	اگر (retval)
		برو done;

done:
	اگر (newpath)
		ext2fs_free_mem(&newpath);
	آزاد(block_buf);

	بازگشت retval;
}

errcode_t ext2fs_extent_گره_split(ext2_extent_handle_t handle)
{
	بازگشت extent_گره_split(handle, 0);
}

errcode_t ext2fs_extent_درج(ext2_extent_handle_t handle, صحیح flags,
				      ساختار ext2fs_extent *extent)
{
	ساختار extent_path		*path;
	ساختار ext3_extent_idx		*ix;
	ساختار ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

	اگر (!handle->path)
		بازگشت EXT2_ET_NO_CURRENT_گره;

#ifdef DEBUG
	printf("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	اگر (path->entries >= path->max_entries) {
		اگر (flags & EXT2_EXTENT_درج_NOSPLIT) {
			بازگشت EXT2_ET_CANT_درج_EXTENT;
		} وگرنه {
#ifdef DEBUG
			printf("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_گره_split(handle, 1);
			اگر (retval)
				بازگشت retval;
			path = handle->path + handle->level;
		}
	}

	eh = (ساختار ext3_extent_header *) path->buf;
	اگر (path->curr) {
		ix = path->curr;
		اگر (flags & EXT2_EXTENT_درج_AFTER) {
			ix++;
			path->چپ--;
		}
	} وگرنه {
		ix = EXT_FIRST_INDEX(eh);
		path->چپ = -1;
	}

	path->curr = ix;

	اگر (path->چپ >= 0)
		memmove(ix + 1, ix,
			(path->چپ+1) * اندازه(ساختار ext3_extent_idx));
	path->چپ++;
	path->entries++;

	eh = (ساختار ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	اگر (retval)
		برو errout;

	retval = update_path(handle);
	اگر (retval)
		برو errout;

	بازگشت 0;

errout:
	ext2fs_extent_delete(handle, 0);
	بازگشت retval;
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
				 blk64_t logical, blk64_t physical, صحیح flags)
{
	errcode_t		ec, retval = 0;
	صحیح			mapped = 1; /* logical is mapped? */
	صحیح			orig_height;
	صحیح			extent_uninit = 0;
	صحیح			prev_uninit = 0;
	صحیح			next_uninit = 0;
	صحیح			new_uninit = 0;
	صحیح			max_len = EXT_INIT_MAX_LEN;
	صحیح			has_prev, has_next;
	blk64_t			orig_lblk;
	ساختار extent_path	*path;
	ساختار ext2fs_extent	extent, next_extent, prev_extent;
	ساختار ext2fs_extent	newextent;
	ساختار ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	printf("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

	اگر (!handle->path)
		بازگشت EXT2_ET_NO_CURRENT_گره;

	path = handle->path + handle->level;

	اگر (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	اگر (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		اگر (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	اگر ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_درج(handle, 0, &newextent);
		بازگشت retval;
	}

	/* save our original location in the extent tree */
	اگر ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		اگر (retval != EXT2_ET_NO_CURRENT_گره)
			بازگشت retval;
		memset(&extent, 0, اندازه(extent));
	}
	اگر ((retval = ext2fs_extent_get_info(handle, &info)))
		بازگشت retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	اگر (retval) {
		اگر (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			اگر (!physical) {
#ifdef DEBUG
				printf("block %llu already unmapped\n",
					logical);
#endif
				برو done;
			}
		} وگرنه
			برو done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	اگر (retval)
		برو done;
	اگر (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	اگر (retval) {
		has_next = 0;
		اگر (retval != EXT2_ET_EXTENT_NO_NEXT)
			برو done;
	} وگرنه {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		اگر (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	اگر (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		برو done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	اگر (retval) {
		has_prev = 0;
		اگر (retval != EXT2_ET_EXTENT_NO_PREV)
			برو done;
	} وگرنه {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		اگر (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	اگر (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		برو done;

	/* check if already pointing to the requested physical */
	اگر (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		printf("physical block (at %llu) unchanged\n", logical);
#endif
		برو done;
	}

	اگر (!mapped) {
#ifdef DEBUG
		printf("mapping unmapped logical block %llu\n", logical);
#endif
		اگر ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((صحیح) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} وگرنه اگر ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((صحیح) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} وگرنه اگر (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((صحیح) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			اگر (retval)
				برو done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} وگرنه اگر (logical < extent.e_lblk)
			retval = ext2fs_extent_درج(handle, 0, &newextent);
		وگرنه
			retval = ext2fs_extent_درج(handle,
				      EXT2_EXTENT_درج_AFTER, &newextent);
		اگر (retval)
			برو done;
		retval = ext2fs_extent_fix_parents(handle);
		اگر (retval)
			برو done;
	} وگرنه اگر ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		printf("(re/un)mapping only block in extent\n");
#endif
		اگر (physical) {
			اگر (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحیح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				اگر (retval)
					برو done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				اگر (retval)
					برو done;
				برو delete_گره;

			} وگرنه
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} وگرنه {
		delete_گره:
			retval = ext2fs_extent_delete(handle, 0);
			اگر (retval)
				برو done;
			ec = ext2fs_extent_fix_parents(handle);
			اگر (ec != EXT2_ET_NO_CURRENT_گره)
				retval = ec;
		}

		اگر (retval)
			برو done;
	} وگرنه اگر (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		printf("(re/un)mapping last block in extent\n");
#endif
		اگر (physical) {
			اگر (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((صحیح) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				اگر (retval)
					برو done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				اگر (retval)
					برو done;
			} وگرنه
				retval = ext2fs_extent_درج(handle,
				      EXT2_EXTENT_درج_AFTER, &newextent);
			اگر (retval)
				برو done;
			retval = ext2fs_extent_fix_parents(handle);
			اگر (retval)
				برو done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			اگر (retval)
				برو done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			اگر (retval)
				برو done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			برو done;
	} وگرنه اگر (logical == extent.e_lblk) {
#ifdef DEBUG
		printf("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			برو done;
		retval = ext2fs_extent_fix_parents(handle);
		اگر (retval)
			برو done;
		اگر (physical) {
			اگر (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحیح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				اگر (retval)
					برو done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} وگرنه
				retval = ext2fs_extent_درج(handle,
							      0, &newextent);
			اگر (retval)
				برو done;
			retval = ext2fs_extent_fix_parents(handle);
			اگر (retval)
				برو done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			اگر (retval)
				برو done;
		}
	} وگرنه {
		__u32	save_length;
		blk64_t	save_lblk;
		ساختار ext2fs_extent save_extent;
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
		اگر (retval)
			برو done;
		/* insert our new extent, if any */
		اگر (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_درج(handle,
					EXT2_EXTENT_درج_AFTER, &newextent);
			اگر (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				اگر (r2 == 0)
					(تهی)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				برو done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_length - extent.e_len - 1;
		retval = ext2fs_extent_درج(handle,
				EXT2_EXTENT_درج_AFTER, &extent);
		اگر (retval) {
			اگر (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				اگر (r2 == 0)
					(تهی)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			اگر (r2 == 0)
				(تهی)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			برو done;
		}
	}

done:
	/* get handle back to its position */
	اگر (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	بازگشت retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحیح flags)
{
	ساختار extent_path		*path;
	نویسه 				*cp;
	ساختار ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

	اگر (!handle->path)
		بازگشت EXT2_ET_NO_CURRENT_گره;

#ifdef DEBUG
	{
		ساختار ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		اگر (retval == 0) {
			printf("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	اگر (!path->curr)
		بازگشت EXT2_ET_NO_CURRENT_گره;

	cp = path->curr;

	/* Sanity check before memmove() */
	اگر (path->چپ < 0)
		بازگشت EXT2_ET_EXTENT_LEAF_BAD;

	اگر (path->چپ) {
		memmove(cp, cp + اندازه(ساختار ext3_extent_idx),
			path->چپ * اندازه(ساختار ext3_extent_idx));
		path->چپ--;
	} وگرنه {
		ساختار ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	اگر (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	اگر (path->entries == 0 && handle->level) {
		اگر (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			ساختار ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			اگر (retval)
				بازگشت retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} وگرنه {
		eh = (ساختار ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		اگر ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	بازگشت retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 ساختار ext2_extent_info *info)
{
	ساختار extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, اندازه(ساختار ext2_extent_info));

	path = handle->path + handle->level;
	اگر (path) {
		اگر (path->curr)
			info->curr_entry = ((نویسه *) path->curr - path->buf) /
				اندازه(ساختار ext3_extent_idx);
		وگرنه
			info->curr_entry = 0;
		info->رقم_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			اندازه(ساختار ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	بازگشت 0;
}

size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	size_t iblock_sz = اندازه(((ساختار ext2_inode *)پوچ)->i_block);
	size_t iblock_extents = (iblock_sz - اندازه(ساختار ext3_extent_header)) /
				اندازه(ساختار ext3_extent);
	size_t extents_per_block = (handle->fs->blocksize -
				    اندازه(ساختار ext3_extent_header)) /
				   اندازه(ساختار ext3_extent);
	ایستا unsigned صحیح last_blocksize = 0;
	ایستا size_t last_result = 0;

	اگر (last_blocksize && last_blocksize == handle->fs->blocksize)
		بازگشت last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	بازگشت last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       ساختار ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	ساختار ext2fs_extent	extent;
	errcode_t		errcode;
	صحیح    			save_flags = fs->flags;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		بازگشت 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	اگر (errcode) {
		اگر (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		بازگشت errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ریشه, &extent);
	اگر (errcode)
		برو out;

	انجامبده {
		/* Skip to the end of a block of leaf nodes */
		اگر (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			اگر (errcode)
				بشکن;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		اگر (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} وقتی (errcode == 0);

out:
	/* Ok if we run off the end */
	اگر (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	بازگشت errcode;
}

errcode_t ext2fs_decode_extent(ساختار ext2fs_extent *to, تهی *addr, صحیح len)
{
	ساختار ext3_extent *from = (ساختار ext3_extent *)addr;

	اگر (len != اندازه(ساختار ext3_extent))
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	اگر (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	بازگشت 0;
}

errcode_t ext2fs_شمار_blocks(ext2_filsys fs, ext2_ino_t ino,
			      ساختار ext2_inode *inode, blk64_t *ret_شمار)
{
	ext2_extent_handle_t	handle = پوچ;
	ساختار ext2fs_extent	extent;
	errcode_t		errcode;
	صحیح			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	اگر (errcode)
		برو out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ریشه, &extent);
	اگر (errcode)
		برو out;

	errcode = ext2fs_get_array(handle->max_depth, اندازه(blk64_t),
				   &intermediate_nodes);
	اگر (errcode)
		برو out;

	blkcount = handle->level;
	وقتی (!errcode) {
		اگر (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			برای (i = 0; i < handle->level; i++) {
				اگر (intermediate_nodes[i] !=
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
	*ret_شمار = blkcount;
	ext2fs_extent_free(handle);

	بازگشت 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
ثابت نویسه *debug_prog_name = "tst_extents";

#endif

