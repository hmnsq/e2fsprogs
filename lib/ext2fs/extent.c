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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"
#அந்தர்பாவ "e2image.h"

#undef DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

கட்டமைப்பு extent_path {
	எழுத்து		*buf;
	முழு		entries;
	முழு		max_entries;
	முழு		இடது;
	முழு		visit_இலக்கம்;
	முழு		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	வெறுமை		*curr;
};


கட்டமைப்பு ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	கட்டமைப்பு ext2_inode	*inode;
	கட்டமைப்பு ext2_inode	inodebuf;
	முழு			type;
	முழு			level;
	முழு			max_depth;
	முழு			max_paths;
	கட்டமைப்பு extent_path	*path;
};

கட்டமைப்பு ext2_extent_path {
	errcode_t		magic;
	முழு			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
நிலையான வெறுமை dbg_show_header(கட்டமைப்பு ext3_extent_header *eh)
{
	printf("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

நிலையான வெறுமை dbg_show_index(கட்டமைப்பு ext3_extent_idx *ix)
{
	printf("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

நிலையான வெறுமை dbg_show_extent(கட்டமைப்பு ext3_extent *ex)
{
	printf("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

நிலையான வெறுமை dbg_print_extent(எழுத்து *desc, கட்டமைப்பு ext2fs_extent *extent)
{
	எனில் (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	எனில் (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	எனில் (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	எனில் (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	எனில் (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

நிலையான வெறுமை dump_path(மாறிலி எழுத்து *tag, கட்டமைப்பு ext2_extent_handle *handle,
		      கட்டமைப்பு extent_path *path)
{
	கட்டமைப்பு extent_path *ppp = path;
	printf("%s: level=%d\n", tag, handle->level);

	செய் {
		printf("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->இடது, ppp->visit_இலக்கம், ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (வெறுமை *)ppp->buf);
		printf("  ");
		dbg_show_header((கட்டமைப்பு ext3_extent_header *)ppp->buf);
		எனில் (ppp->curr) {
			printf("  ");
			dbg_show_index(ppp->curr);
			printf("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} போது (ppp >= handle->path);
	fflush(stdout);

	திரும்பு;
}

#இல்லையெனில்
#define dbg_show_header(eh) செய் { } போது (0)
#define dbg_show_index(ix) செய் { } போது (0)
#define dbg_show_extent(ex) செய் { } போது (0)
#define dbg_print_extent(desc, ex) செய் { } போது (0)
#define dump_path(tag, handle, path) செய் { } போது (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(வெறுமை *ptr, முழு size)
{
	முழு eh_max, entry_size;
	கட்டமைப்பு ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	எனில் (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		திரும்பு EXT2_ET_EXTENT_HEADER_BAD;
	எனில் (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		திரும்பு EXT2_ET_EXTENT_HEADER_BAD;
	எனில் (eh->eh_depth == 0)
		entry_size = அளவு(கட்டமைப்பு ext3_extent);
	இல்லையெனில்
		entry_size = அளவு(கட்டமைப்பு ext3_extent_idx);

	eh_max = (size - அளவு(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	எனில் ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		திரும்பு EXT2_ET_EXTENT_HEADER_BAD;

	திரும்பு 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
வெறுமை ext2fs_extent_free(ext2_extent_handle_t handle)
{
	முழு			i;

	எனில் (!handle)
		திரும்பு;

	எனில் (handle->path) {
		க்கு (i = 1; i < handle->max_paths; i++) {
			எனில் (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	திரும்பு ext2fs_extent_open2(fs, ino, வெற்று, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    கட்டமைப்பு ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	கட்டமைப்பு ext2_extent_handle	*handle;
	errcode_t			retval;
	முழு				i;
	கட்டமைப்பு ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (!inode)
		எனில் ((ino == 0) || (ino > fs->super->s_inodes_மொத்தம்))
			திரும்பு EXT2_ET_BAD_INODE_இலக்கம்;

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_extent_handle), &handle);
	எனில் (retval)
		திரும்பு retval;
	memset(handle, 0, அளவு(கட்டமைப்பு ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	எனில் (inode) {
		handle->inode = inode;
	} இல்லையெனில் {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		எனில் (retval)
			செல் errout;
	}

	eh = (கட்டமைப்பு ext3_extent_header *) &handle->inode->i_block[0];

	க்கு (i=0; i < EXT2_N_BLOCKS; i++)
		எனில் (handle->inode->i_block[i])
			நிறுத்து;
	எனில் (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (அளவு(handle->inode->i_block) - அளவு(*eh)) /
			அளவு(கட்டமைப்பு ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	எனில் (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		செல் errout;
	}

	retval = ext2fs_extent_header_verify(eh, அளவு(handle->inode->i_block));
	எனில் (retval)
		செல் errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    அளவு(கட்டமைப்பு extent_path),
				    &handle->path);
	handle->path[0].buf = (எழுத்து *) handle->inode->i_block;

	handle->path[0].இடது = handle->path[0].entries =
		ext2fs_le16_to_cpu(eh->eh_entries);
	handle->path[0].max_entries = ext2fs_le16_to_cpu(eh->eh_max);
	handle->path[0].curr = 0;
	handle->path[0].end_blk =
		(EXT2_I_SIZE(handle->inode) + fs->blocksize - 1) >>
		 EXT2_BLOCK_SIZE_BITS(fs->super);
	handle->path[0].blk = 0;
	handle->path[0].visit_இலக்கம் = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	திரும்பு 0;

errout:
	ext2fs_extent_free(handle);
	திரும்பு retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    முழு flags, கட்டமைப்பு ext2fs_extent *extent)
{
	கட்டமைப்பு extent_path	*path, *newpath, *tp;
	கட்டமைப்பு ext3_extent_header	*eh;
	கட்டமைப்பு ext3_extent_idx		*ix = 0;
	கட்டமைப்பு ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	முழு				orig_op, op, l;
	முழு				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	எனில் (!handle->path)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	எனில் ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		எனில் (handle->level < handle->max_depth) {
			/* interior node */
			எனில் (path->visit_இலக்கம் == 0) {
				path->visit_இலக்கம்++;
				op = EXT2_EXTENT_DOWN;
			} இல்லையெனில் எனில் (path->இடது > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			இல்லையெனில் எனில் (handle->level > 0)
				op = EXT2_EXTENT_UP;
			இல்லையெனில்
				திரும்பு EXT2_ET_EXTENT_NO_NEXT;
		} இல்லையெனில் {
			/* leaf node */
			எனில் (path->இடது > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			இல்லையெனில் எனில் (handle->level > 0)
				op = EXT2_EXTENT_UP;
			இல்லையெனில்
				திரும்பு EXT2_ET_EXTENT_NO_NEXT;
		}
		எனில் (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	எனில் ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		எனில் (handle->level < handle->max_depth) {
			/* interior node */
			எனில் (path->visit_இலக்கம் > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} இல்லையெனில் எனில் (path->இடது < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			இல்லையெனில் எனில் (handle->level > 0)
				op = EXT2_EXTENT_UP;
			இல்லையெனில்
				திரும்பு EXT2_ET_EXTENT_NO_PREV;
		} இல்லையெனில் {
			/* leaf node */
			எனில் (path->இடது < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			இல்லையெனில் எனில் (handle->level > 0)
				op = EXT2_EXTENT_UP;
			இல்லையெனில்
				திரும்பு EXT2_ET_EXTENT_NO_PREV;
		}
		எனில் (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	எனில் (orig_op == EXT2_EXTENT_LAST_LEAF) {
		எனில் ((handle->level < handle->max_depth) &&
		    (path->இடது == 0))
			op = EXT2_EXTENT_DOWN;
		இல்லையெனில்
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		printf("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	தேர்வு (op) {
	நிலை EXT2_EXTENT_CURRENT:
		ix = path->curr;
		நிறுத்து;
	நிலை EXT2_EXTENT_வேர்:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	நிலை EXT2_EXTENT_FIRST_SIB:
		path->இடது = path->entries;
		path->curr = 0;
		/* fallthrough */
	நிலை EXT2_EXTENT_NEXT_SIB:
		எனில் (path->இடது <= 0)
			திரும்பு EXT2_ET_EXTENT_NO_NEXT;
		எனில் (path->curr) {
			ix = path->curr;
			ix++;
		} இல்லையெனில் {
			eh = (கட்டமைப்பு ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->இடது--;
		path->curr = ix;
		path->visit_இலக்கம் = 0;
		நிறுத்து;
	நிலை EXT2_EXTENT_PREV_SIB:
		எனில் (!path->curr ||
		    path->இடது+1 >= path->entries)
			திரும்பு EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->இடது++;
		எனில் (handle->level < handle->max_depth)
			path->visit_இலக்கம் = 1;
		நிறுத்து;
	நிலை EXT2_EXTENT_LAST_SIB:
		eh = (கட்டமைப்பு ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->இடது = 0;
		path->visit_இலக்கம் = 0;
		நிறுத்து;
	நிலை EXT2_EXTENT_UP:
		எனில் (handle->level <= 0)
			திரும்பு EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		எனில் ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_இலக்கம் = 0;
		நிறுத்து;
	நிலை EXT2_EXTENT_DOWN:
	நிலை EXT2_EXTENT_DOWN_AND_LAST:
		எனில் (!path->curr ||(handle->level >= handle->max_depth))
			திரும்பு EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		எனில் (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			எனில் (retval)
				திரும்பு retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		க்கு (l = handle->level, tp = path; l > 0; l--, tp--) {
			எனில் (blk == tp->blk)
				திரும்பு EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		எனில் ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		இல்லையெனில் {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			எனில் (retval)
				திரும்பு retval;
		}
		handle->level++;

		eh = (கட்டமைப்பு ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		எனில் (retval) {
			handle->level--;
			திரும்பு retval;
		}

		எனில் (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->இடது = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		எனில் (newpath->இடது <= 0)
			திரும்பு EXT2_ET_EXTENT_NO_DOWN;

		எனில் (path->இடது > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} இல்லையெனில்
			newpath->end_blk = path->end_blk;

		path = newpath;
		எனில் (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((கட்டமைப்பு ext3_extent_header *) eh);
			path->curr = ix;
			path->இடது = path->entries - 1;
			path->visit_இலக்கம் = 0;
		} இல்லையெனில் {
			ix = EXT_LAST_INDEX((கட்டமைப்பு ext3_extent_header *) eh);
			path->curr = ix;
			path->இடது = 0;
			எனில் (handle->level < handle->max_depth)
				path->visit_இலக்கம் = 1;
		}
#ifdef DEBUG_GET_EXTENT
		printf("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		நிறுத்து;
	இயல்பு:
		திரும்பு EXT2_ET_OP_NOT_SUPPORTED;
	}

	எனில் (!ix)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	printf("(Left %d)\n", path->இடது);
#endif

	எனில் (handle->level == handle->max_depth) {
		ex = (கட்டமைப்பு ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		எனில் (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} இல்லையெனில் {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		எனில் (path->இடது > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} இல்லையெனில்
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	எனில் (path->visit_இலக்கம்)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	எனில் (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		செல் retry;

	எனில் ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->இடது != 0)))
		செல் retry;

	எனில் (failed_csum)
		திரும்பு EXT2_ET_EXTENT_CSUM_INVALID;

	திரும்பு 0;
}

நிலையான errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	கட்டமைப்பு ext3_extent_idx		*ix;
	கட்டமைப்பு ext3_extent_header	*eh;

	எனில் (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} இல்லையெனில் {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (கட்டமைப்பு ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		எனில் (retval)
			திரும்பு retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	திரும்பு retval;
}

#எனில் 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	கட்டமைப்பு ext2fs_extent	extent;
	கட்டமைப்பு ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	எனில் (retval)
		திரும்பு retval;

	retval = ext2fs_extent_get_info(handle, &info);
	எனில் (retval)
		திரும்பு retval;

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_extent_path), &save_path);
	எனில் (retval)
		திரும்பு retval;
	memset(save_path, 0, அளவு(கட்டமைப்பு ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	திரும்பு 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	திரும்பு 0;
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
			      முழு leaf_level, blk64_t blk)
{
	கட்டமைப்பு ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_வேர், &extent);
	எனில் (retval) {
		எனில் (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		திரும்பு retval;
	}

	எனில் (leaf_level > handle->max_depth) {
#ifdef DEBUG
		printf("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		திரும்பு EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	printf("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	போது (1) {
		எனில் (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			எனில் ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				திரும்பு 0;
			எனில் (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				திரும்பு EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			எனில் (retval == EXT2_ET_EXTENT_NO_NEXT)
				திரும்பு EXT2_ET_EXTENT_NOT_FOUND;
			எனில் (retval)
				திரும்பு retval;
			தொடர்;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		எனில் (retval == EXT2_ET_EXTENT_NO_NEXT)
			செல் go_down;
		எனில் (retval)
			திரும்பு retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		எனில் (blk == extent.e_lblk)
			செல் go_down;
		எனில் (blk > extent.e_lblk)
			தொடர்;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		எனில் (retval)
			திரும்பு retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		எனில் (retval)
			திரும்பு retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	திரும்பு ext2fs_extent_goto2(handle, 0, blk);
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
	முழு				retval = 0;
	முழு				orig_height;
	blk64_t				start;
	கட்டமைப்பு extent_path		*path;
	கட்டமைப்பு ext2fs_extent		extent;
	கட்டமைப்பு ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	எனில் (!(handle->fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

	எனில் (!handle->path)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

	path = handle->path + handle->level;
	எனில் (!path->curr)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	எனில் (retval)
		செல் done;

	/* modified node's start block */
	start = extent.e_lblk;

	எனில் ((retval = ext2fs_extent_get_info(handle, &info)))
		திரும்பு retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	போது (handle->level > 0 &&
	       (path->இடது == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		எனில் (retval)
			செல் done;
		எனில் (extent.e_lblk == start)
			நிறுத்து;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		எனில் (retval)
			செல் done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	திரும்பு retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				முழு flags EXT2FS_ATTR((unused)),
				கட்டமைப்பு ext2fs_extent *extent)
{
	கட்டமைப்பு extent_path		*path;
	கட்டமைப்பு ext3_extent_idx		*ix;
	கட்டமைப்பு ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	எனில் (!(handle->fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

	எனில் (!handle->path)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

	path = handle->path + handle->level;
	எனில் (!path->curr)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

#ifdef DEBUG
	printf("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	எனில் (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		எனில் (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			எனில் (extent->e_len > EXT_UNINIT_MAX_LEN)
				திரும்பு EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} இல்லையெனில் {
			எனில் (extent->e_len > EXT_INIT_MAX_LEN)
				திரும்பு EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} இல்லையெனில் {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	திரும்பு 0;
}

நிலையான முழு splitting_at_eof(கட்டமைப்பு ext2_extent_handle *handle,
			    கட்டமைப்பு extent_path *path)
{
	கட்டமைப்பு extent_path *ppp = path;
	dump_path(__func__, handle, path);

	எனில் (handle->level == 0)
		திரும்பு 0;

	செய் {
		எனில் (ppp->இடது)
			திரும்பு 0;
		ppp--;
	} போது (ppp >= handle->path);

	திரும்பு 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
நிலையான errcode_t extent_முனை_split(ext2_extent_handle_t handle,
				   முழு expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_முனை_pblk;
	blk64_t				new_முனை_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	முழு				orig_height;
	எழுத்து				*block_buf = வெற்று;
	கட்டமைப்பு ext2fs_extent		extent;
	கட்டமைப்பு extent_path		*path, *newpath = 0;
	கட்டமைப்பு ext3_extent_header	*eh, *neweh;
	முழு				tocopy;
	முழு				new_வேர் = 0;
	கட்டமைப்பு ext2_extent_info		info;
	முழு				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	எனில் (!(handle->fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

	எனில் (!handle->path)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

#ifdef DEBUG
	printf("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	எனில் (retval)
		செல் done;

	retval = ext2fs_extent_get_info(handle, &info);
	எனில் (retval)
		செல் done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (கட்டமைப்பு ext3_extent_header *) path->buf;
	எனில் (handle->level == handle->max_depth) {
		கட்டமைப்பு ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} இல்லையெனில் {
		கட்டமைப்பு ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	எனில் (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		printf("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		எனில் (retval)
			செல் done;

		retval = extent_முனை_split(handle, expand_allowed);
		எனில் (retval)
			செல் done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		எனில் (retval)
			செல் done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	எனில் (!path->curr)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (கட்டமைப்பு ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	எனில் (handle->level == 0) {
		new_வேர் = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    அளவு(கட்டமைப்பு extent_path),
					    &newpath);
		எனில் (retval)
			செல் done;
	} இல்லையெனில் {
		எனில் (no_balance)
			tocopy = 1;
		இல்லையெனில்
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	printf("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	எனில் (!tocopy && !no_balance) {
#ifdef DEBUG
		printf("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		செல் done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = malloc(handle->fs->blocksize);
	எனில் (!block_buf) {
		retval = ENOMEM;
		செல் done;
	}

	எனில் (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_முனை_pblk);
	எனில் (retval)
		செல் done;

#ifdef DEBUG
	printf("will copy to new node at block %lu\n",
	       (unsigned long) new_முனை_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (கட்டமைப்பு ext3_extent_header *) block_buf;
	memcpy(neweh, eh, அளவு(கட்டமைப்பு ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 அளவு(கட்டமைப்பு ext3_extent_header)) /
				அளவு(கட்டமைப்பு ext3_extent));

	/* then the entries for the new block... */
	memcpy(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		அளவு(கட்டமைப்பு ext3_extent_idx) * tocopy);

	new_முனை_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	எனில் (retval)
		செல் done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_முனை_pblk, 1,
					block_buf);

	எனில் (retval)
		செல் done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	எனில் (handle->level == 0) {
		memcpy(newpath, path,
		       அளவு(கட்டமைப்பு extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->இடது = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} இல்லையெனில் {
		path->entries -= tocopy;
		path->இடது -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	எனில் (retval)
		செல் done;

	/* now go up and insert/replace index for new node we created */
	எனில் (new_வேர்) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		எனில் (retval)
			செல் done;

		extent.e_lblk = new_முனை_start;
		extent.e_pblk = new_முனை_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		எனில் (retval)
			செல் done;
	} இல்லையெனில் {
		__u32 new_முனை_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_முனை_length = new_முனை_start - extent.e_lblk;
		extent.e_len -= new_முனை_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		எனில் (retval)
			செல் done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_முனை_start;
		extent.e_pblk = new_முனை_pblk;
		extent.e_len = new_முனை_length;
		retval = ext2fs_extent_செருகு(handle, EXT2_EXTENT_செருகு_AFTER, &extent);
		எனில் (retval)
			செல் done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	எனில் (retval)
		செல் done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	எனில் (retval)
		செல் done;

done:
	எனில் (newpath)
		ext2fs_free_mem(&newpath);
	விடுவி(block_buf);

	திரும்பு retval;
}

errcode_t ext2fs_extent_முனை_split(ext2_extent_handle_t handle)
{
	திரும்பு extent_முனை_split(handle, 0);
}

errcode_t ext2fs_extent_செருகு(ext2_extent_handle_t handle, முழு flags,
				      கட்டமைப்பு ext2fs_extent *extent)
{
	கட்டமைப்பு extent_path		*path;
	கட்டமைப்பு ext3_extent_idx		*ix;
	கட்டமைப்பு ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	எனில் (!(handle->fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

	எனில் (!handle->path)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

#ifdef DEBUG
	printf("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	எனில் (path->entries >= path->max_entries) {
		எனில் (flags & EXT2_EXTENT_செருகு_NOSPLIT) {
			திரும்பு EXT2_ET_CANT_செருகு_EXTENT;
		} இல்லையெனில் {
#ifdef DEBUG
			printf("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_முனை_split(handle, 1);
			எனில் (retval)
				திரும்பு retval;
			path = handle->path + handle->level;
		}
	}

	eh = (கட்டமைப்பு ext3_extent_header *) path->buf;
	எனில் (path->curr) {
		ix = path->curr;
		எனில் (flags & EXT2_EXTENT_செருகு_AFTER) {
			ix++;
			path->இடது--;
		}
	} இல்லையெனில் {
		ix = EXT_FIRST_INDEX(eh);
		path->இடது = -1;
	}

	path->curr = ix;

	எனில் (path->இடது >= 0)
		memmove(ix + 1, ix,
			(path->இடது+1) * அளவு(கட்டமைப்பு ext3_extent_idx));
	path->இடது++;
	path->entries++;

	eh = (கட்டமைப்பு ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	எனில் (retval)
		செல் errout;

	retval = update_path(handle);
	எனில் (retval)
		செல் errout;

	திரும்பு 0;

errout:
	ext2fs_extent_delete(handle, 0);
	திரும்பு retval;
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
				 blk64_t logical, blk64_t physical, முழு flags)
{
	errcode_t		ec, retval = 0;
	முழு			mapped = 1; /* logical is mapped? */
	முழு			orig_height;
	முழு			extent_uninit = 0;
	முழு			prev_uninit = 0;
	முழு			next_uninit = 0;
	முழு			new_uninit = 0;
	முழு			max_len = EXT_INIT_MAX_LEN;
	முழு			has_prev, has_next;
	blk64_t			orig_lblk;
	கட்டமைப்பு extent_path	*path;
	கட்டமைப்பு ext2fs_extent	extent, next_extent, prev_extent;
	கட்டமைப்பு ext2fs_extent	newextent;
	கட்டமைப்பு ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	printf("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	எனில் (!(handle->fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

	எனில் (!handle->path)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

	path = handle->path + handle->level;

	எனில் (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	எனில் (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		எனில் (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	எனில் ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_செருகு(handle, 0, &newextent);
		திரும்பு retval;
	}

	/* save our original location in the extent tree */
	எனில் ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		எனில் (retval != EXT2_ET_NO_CURRENT_முனை)
			திரும்பு retval;
		memset(&extent, 0, அளவு(extent));
	}
	எனில் ((retval = ext2fs_extent_get_info(handle, &info)))
		திரும்பு retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	எனில் (retval) {
		எனில் (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			எனில் (!physical) {
#ifdef DEBUG
				printf("block %llu already unmapped\n",
					logical);
#endif
				செல் done;
			}
		} இல்லையெனில்
			செல் done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	எனில் (retval)
		செல் done;
	எனில் (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	எனில் (retval) {
		has_next = 0;
		எனில் (retval != EXT2_ET_EXTENT_NO_NEXT)
			செல் done;
	} இல்லையெனில் {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		எனில் (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	எனில் (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		செல் done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	எனில் (retval) {
		has_prev = 0;
		எனில் (retval != EXT2_ET_EXTENT_NO_PREV)
			செல் done;
	} இல்லையெனில் {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		எனில் (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	எனில் (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		செல் done;

	/* check if already pointing to the requested physical */
	எனில் (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		printf("physical block (at %llu) unchanged\n", logical);
#endif
		செல் done;
	}

	எனில் (!mapped) {
#ifdef DEBUG
		printf("mapping unmapped logical block %llu\n", logical);
#endif
		எனில் ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((முழு) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} இல்லையெனில் எனில் ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((முழு) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} இல்லையெனில் எனில் (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((முழு) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			எனில் (retval)
				செல் done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} இல்லையெனில் எனில் (logical < extent.e_lblk)
			retval = ext2fs_extent_செருகு(handle, 0, &newextent);
		இல்லையெனில்
			retval = ext2fs_extent_செருகு(handle,
				      EXT2_EXTENT_செருகு_AFTER, &newextent);
		எனில் (retval)
			செல் done;
		retval = ext2fs_extent_fix_parents(handle);
		எனில் (retval)
			செல் done;
	} இல்லையெனில் எனில் ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		printf("(re/un)mapping only block in extent\n");
#endif
		எனில் (physical) {
			எனில் (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((முழு) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				எனில் (retval)
					செல் done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				எனில் (retval)
					செல் done;
				செல் delete_முனை;

			} இல்லையெனில்
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} இல்லையெனில் {
		delete_முனை:
			retval = ext2fs_extent_delete(handle, 0);
			எனில் (retval)
				செல் done;
			ec = ext2fs_extent_fix_parents(handle);
			எனில் (ec != EXT2_ET_NO_CURRENT_முனை)
				retval = ec;
		}

		எனில் (retval)
			செல் done;
	} இல்லையெனில் எனில் (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		printf("(re/un)mapping last block in extent\n");
#endif
		எனில் (physical) {
			எனில் (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((முழு) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				எனில் (retval)
					செல் done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				எனில் (retval)
					செல் done;
			} இல்லையெனில்
				retval = ext2fs_extent_செருகு(handle,
				      EXT2_EXTENT_செருகு_AFTER, &newextent);
			எனில் (retval)
				செல் done;
			retval = ext2fs_extent_fix_parents(handle);
			எனில் (retval)
				செல் done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			எனில் (retval)
				செல் done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			எனில் (retval)
				செல் done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		எனில் (retval)
			செல் done;
	} இல்லையெனில் எனில் (logical == extent.e_lblk) {
#ifdef DEBUG
		printf("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		எனில் (retval)
			செல் done;
		retval = ext2fs_extent_fix_parents(handle);
		எனில் (retval)
			செல் done;
		எனில் (physical) {
			எனில் (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((முழு) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				எனில் (retval)
					செல் done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} இல்லையெனில்
				retval = ext2fs_extent_செருகு(handle,
							      0, &newextent);
			எனில் (retval)
				செல் done;
			retval = ext2fs_extent_fix_parents(handle);
			எனில் (retval)
				செல் done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			எனில் (retval)
				செல் done;
		}
	} இல்லையெனில் {
		__u32	save_length;
		blk64_t	save_lblk;
		கட்டமைப்பு ext2fs_extent save_extent;
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
		எனில் (retval)
			செல் done;
		/* insert our new extent, if any */
		எனில் (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_செருகு(handle,
					EXT2_EXTENT_செருகு_AFTER, &newextent);
			எனில் (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				எனில் (r2 == 0)
					(வெறுமை)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				செல் done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_length - extent.e_len - 1;
		retval = ext2fs_extent_செருகு(handle,
				EXT2_EXTENT_செருகு_AFTER, &extent);
		எனில் (retval) {
			எனில் (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				எனில் (r2 == 0)
					(வெறுமை)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			எனில் (r2 == 0)
				(வெறுமை)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			செல் done;
		}
	}

done:
	/* get handle back to its position */
	எனில் (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	திரும்பு retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, முழு flags)
{
	கட்டமைப்பு extent_path		*path;
	எழுத்து 				*cp;
	கட்டமைப்பு ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	எனில் (!(handle->fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

	எனில் (!handle->path)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

#ifdef DEBUG
	{
		கட்டமைப்பு ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		எனில் (retval == 0) {
			printf("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	எனில் (!path->curr)
		திரும்பு EXT2_ET_NO_CURRENT_முனை;

	cp = path->curr;

	/* Sanity check before memmove() */
	எனில் (path->இடது < 0)
		திரும்பு EXT2_ET_EXTENT_LEAF_BAD;

	எனில் (path->இடது) {
		memmove(cp, cp + அளவு(கட்டமைப்பு ext3_extent_idx),
			path->இடது * அளவு(கட்டமைப்பு ext3_extent_idx));
		path->இடது--;
	} இல்லையெனில் {
		கட்டமைப்பு ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	எனில் (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	எனில் (path->entries == 0 && handle->level) {
		எனில் (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			கட்டமைப்பு ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			எனில் (retval)
				திரும்பு retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} இல்லையெனில் {
		eh = (கட்டமைப்பு ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		எனில் ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	திரும்பு retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 கட்டமைப்பு ext2_extent_info *info)
{
	கட்டமைப்பு extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, அளவு(கட்டமைப்பு ext2_extent_info));

	path = handle->path + handle->level;
	எனில் (path) {
		எனில் (path->curr)
			info->curr_entry = ((எழுத்து *) path->curr - path->buf) /
				அளவு(கட்டமைப்பு ext3_extent_idx);
		இல்லையெனில்
			info->curr_entry = 0;
		info->இலக்கம்_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			அளவு(கட்டமைப்பு ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	திரும்பு 0;
}

size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	size_t iblock_sz = அளவு(((கட்டமைப்பு ext2_inode *)வெற்று)->i_block);
	size_t iblock_extents = (iblock_sz - அளவு(கட்டமைப்பு ext3_extent_header)) /
				அளவு(கட்டமைப்பு ext3_extent);
	size_t extents_per_block = (handle->fs->blocksize -
				    அளவு(கட்டமைப்பு ext3_extent_header)) /
				   அளவு(கட்டமைப்பு ext3_extent);
	நிலையான unsigned முழு last_blocksize = 0;
	நிலையான size_t last_result = 0;

	எனில் (last_blocksize && last_blocksize == handle->fs->blocksize)
		திரும்பு last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	திரும்பு last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       கட்டமைப்பு ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	கட்டமைப்பு ext2fs_extent	extent;
	errcode_t		errcode;
	முழு    			save_flags = fs->flags;

	எனில் (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		திரும்பு 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	எனில் (errcode) {
		எனில் (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		திரும்பு errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_வேர், &extent);
	எனில் (errcode)
		செல் out;

	செய் {
		/* Skip to the end of a block of leaf nodes */
		எனில் (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			எனில் (errcode)
				நிறுத்து;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		எனில் (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} போது (errcode == 0);

out:
	/* Ok if we run off the end */
	எனில் (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	திரும்பு errcode;
}

errcode_t ext2fs_decode_extent(கட்டமைப்பு ext2fs_extent *to, வெறுமை *addr, முழு len)
{
	கட்டமைப்பு ext3_extent *from = (கட்டமைப்பு ext3_extent *)addr;

	எனில் (len != அளவு(கட்டமைப்பு ext3_extent))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	எனில் (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	திரும்பு 0;
}

errcode_t ext2fs_மொத்தம்_blocks(ext2_filsys fs, ext2_ino_t ino,
			      கட்டமைப்பு ext2_inode *inode, blk64_t *ret_மொத்தம்)
{
	ext2_extent_handle_t	handle = வெற்று;
	கட்டமைப்பு ext2fs_extent	extent;
	errcode_t		errcode;
	முழு			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	எனில் (errcode)
		செல் out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_வேர், &extent);
	எனில் (errcode)
		செல் out;

	errcode = ext2fs_get_array(handle->max_depth, அளவு(blk64_t),
				   &intermediate_nodes);
	எனில் (errcode)
		செல் out;

	blkcount = handle->level;
	போது (!errcode) {
		எனில் (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			க்கு (i = 0; i < handle->level; i++) {
				எனில் (intermediate_nodes[i] !=
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
	*ret_மொத்தம் = blkcount;
	ext2fs_extent_free(handle);

	திரும்பு 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
மாறிலி எழுத்து *debug_prog_name = "tst_extents";

#endif

