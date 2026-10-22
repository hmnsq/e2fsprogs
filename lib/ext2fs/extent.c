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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"
#অন্তর্ভাৱ "e2image.h"

#undef DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

সংরচনা extent_path {
	ৱর্ণ		*buf;
	পূর্ণাঙ্ক		entries;
	পূর্ণাঙ্ক		max_entries;
	পূর্ণাঙ্ক		বাম;
	পূর্ণাঙ্ক		visit_অঙ্ক;
	পূর্ণাঙ্ক		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	শূন্য		*curr;
};


সংরচনা ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	সংরচনা ext2_inode	*inode;
	সংরচনা ext2_inode	inodebuf;
	পূর্ণাঙ্ক			type;
	পূর্ণাঙ্ক			level;
	পূর্ণাঙ্ক			max_depth;
	পূর্ণাঙ্ক			max_paths;
	সংরচনা extent_path	*path;
};

সংরচনা ext2_extent_path {
	errcode_t		magic;
	পূর্ণাঙ্ক			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
স্থির শূন্য dbg_show_header(সংরচনা ext3_extent_header *eh)
{
	printf("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

স্থির শূন্য dbg_show_index(সংরচনা ext3_extent_idx *ix)
{
	printf("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

স্থির শূন্য dbg_show_extent(সংরচনা ext3_extent *ex)
{
	printf("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

স্থির শূন্য dbg_print_extent(ৱর্ণ *desc, সংরচনা ext2fs_extent *extent)
{
	যদি (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	যদি (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	যদি (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	যদি (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	যদি (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

স্থির শূন্য dump_path(ধ্রুৱ ৱর্ণ *tag, সংরচনা ext2_extent_handle *handle,
		      সংরচনা extent_path *path)
{
	সংরচনা extent_path *ppp = path;
	printf("%s: level=%d\n", tag, handle->level);

	কৰা {
		printf("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->বাম, ppp->visit_অঙ্ক, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (শূন্য *)ppp->buf);
		printf("  ");
		dbg_show_header((সংরচনা ext3_extent_header *)ppp->buf);
		যদি (ppp->curr) {
			printf("  ");
			dbg_show_index(ppp->curr);
			printf("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} যেতিয়ালৈ (ppp >= handle->path);
	fflush(stdout);

	ঘূৰাই;
}

#অন্যথা
#define dbg_show_header(eh) কৰা { } যেতিয়ালৈ (0)
#define dbg_show_index(ix) কৰা { } যেতিয়ালৈ (0)
#define dbg_show_extent(ex) কৰা { } যেতিয়ালৈ (0)
#define dbg_print_extent(desc, ex) কৰা { } যেতিয়ালৈ (0)
#define dump_path(tag, handle, path) কৰা { } যেতিয়ালৈ (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(শূন্য *ptr, পূর্ণাঙ্ক size)
{
	পূর্ণাঙ্ক eh_max, entry_size;
	সংরচনা ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	যদি (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		ঘূৰাই EXT2_ET_EXTENT_HEADER_BAD;
	যদি (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		ঘূৰাই EXT2_ET_EXTENT_HEADER_BAD;
	যদি (eh->eh_depth == 0)
		entry_size = পরিমাণ(সংরচনা ext3_extent);
	অন্যথা
		entry_size = পরিমাণ(সংরচনা ext3_extent_idx);

	eh_max = (size - পরিমাণ(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	যদি ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		ঘূৰাই EXT2_ET_EXTENT_HEADER_BAD;

	ঘূৰাই 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
শূন্য ext2fs_extent_free(ext2_extent_handle_t handle)
{
	পূর্ণাঙ্ক			i;

	যদি (!handle)
		ঘূৰাই;

	যদি (handle->path) {
		বাবে (i = 1; i < handle->max_paths; i++) {
			যদি (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	ঘূৰাই ext2fs_extent_open2(fs, ino, রিক্ত, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    সংরচনা ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	সংরচনা ext2_extent_handle	*handle;
	errcode_t			retval;
	পূর্ণাঙ্ক				i;
	সংরচনা ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!inode)
		যদি ((ino == 0) || (ino > fs->super->s_inodes_গুনতি))
			ঘূৰাই EXT2_ET_BAD_INODE_অঙ্ক;

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা ext2_extent_handle), &handle);
	যদি (retval)
		ঘূৰাই retval;
	memset(handle, 0, পরিমাণ(সংরচনা ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	যদি (inode) {
		handle->inode = inode;
	} অন্যথা {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		যদি (retval)
			গচ্ছ errout;
	}

	eh = (সংরচনা ext3_extent_header *) &handle->inode->i_block[0];

	বাবে (i=0; i < EXT2_N_BLOCKS; i++)
		যদি (handle->inode->i_block[i])
			ভঙ্গ;
	যদি (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (পরিমাণ(handle->inode->i_block) - পরিমাণ(*eh)) /
			পরিমাণ(সংরচনা ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	যদি (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		গচ্ছ errout;
	}

	retval = ext2fs_extent_header_verify(eh, পরিমাণ(handle->inode->i_block));
	যদি (retval)
		গচ্ছ errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    পরিমাণ(সংরচনা extent_path),
				    &handle->path);
	handle->path[0].buf = (ৱর্ণ *) handle->inode->i_block;

	handle->path[0].বাম = handle->path[0].entries =
		ext2fs_le16_to_cpu(eh->eh_entries);
	handle->path[0].max_entries = ext2fs_le16_to_cpu(eh->eh_max);
	handle->path[0].curr = 0;
	handle->path[0].end_blk =
		(EXT2_I_SIZE(handle->inode) + fs->blocksize - 1) >>
		 EXT2_BLOCK_SIZE_BITS(fs->super);
	handle->path[0].blk = 0;
	handle->path[0].visit_অঙ্ক = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	ঘূৰাই 0;

errout:
	ext2fs_extent_free(handle);
	ঘূৰাই retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    পূর্ণাঙ্ক flags, সংরচনা ext2fs_extent *extent)
{
	সংরচনা extent_path	*path, *newpath, *tp;
	সংরচনা ext3_extent_header	*eh;
	সংরচনা ext3_extent_idx		*ix = 0;
	সংরচনা ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	পূর্ণাঙ্ক				orig_op, op, l;
	পূর্ণাঙ্ক				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	যদি (!handle->path)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	যদি ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		যদি (handle->level < handle->max_depth) {
			/* interior node */
			যদি (path->visit_অঙ্ক == 0) {
				path->visit_অঙ্ক++;
				op = EXT2_EXTENT_DOWN;
			} অন্যথা যদি (path->বাম > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			অন্যথা যদি (handle->level > 0)
				op = EXT2_EXTENT_UP;
			অন্যথা
				ঘূৰাই EXT2_ET_EXTENT_NO_NEXT;
		} অন্যথা {
			/* leaf node */
			যদি (path->বাম > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			অন্যথা যদি (handle->level > 0)
				op = EXT2_EXTENT_UP;
			অন্যথা
				ঘূৰাই EXT2_ET_EXTENT_NO_NEXT;
		}
		যদি (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	যদি ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		যদি (handle->level < handle->max_depth) {
			/* interior node */
			যদি (path->visit_অঙ্ক > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} অন্যথা যদি (path->বাম < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			অন্যথা যদি (handle->level > 0)
				op = EXT2_EXTENT_UP;
			অন্যথা
				ঘূৰাই EXT2_ET_EXTENT_NO_PREV;
		} অন্যথা {
			/* leaf node */
			যদি (path->বাম < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			অন্যথা যদি (handle->level > 0)
				op = EXT2_EXTENT_UP;
			অন্যথা
				ঘূৰাই EXT2_ET_EXTENT_NO_PREV;
		}
		যদি (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	যদি (orig_op == EXT2_EXTENT_LAST_LEAF) {
		যদি ((handle->level < handle->max_depth) &&
		    (path->বাম == 0))
			op = EXT2_EXTENT_DOWN;
		অন্যথা
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		printf("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	ৱিকল্প (op) {
	পক্ষ EXT2_EXTENT_CURRENT:
		ix = path->curr;
		ভঙ্গ;
	পক্ষ EXT2_EXTENT_মূল:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	পক্ষ EXT2_EXTENT_FIRST_SIB:
		path->বাম = path->entries;
		path->curr = 0;
		/* fallthrough */
	পক্ষ EXT2_EXTENT_NEXT_SIB:
		যদি (path->বাম <= 0)
			ঘূৰাই EXT2_ET_EXTENT_NO_NEXT;
		যদি (path->curr) {
			ix = path->curr;
			ix++;
		} অন্যথা {
			eh = (সংরচনা ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->বাম--;
		path->curr = ix;
		path->visit_অঙ্ক = 0;
		ভঙ্গ;
	পক্ষ EXT2_EXTENT_PREV_SIB:
		যদি (!path->curr ||
		    path->বাম+1 >= path->entries)
			ঘূৰাই EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->বাম++;
		যদি (handle->level < handle->max_depth)
			path->visit_অঙ্ক = 1;
		ভঙ্গ;
	পক্ষ EXT2_EXTENT_LAST_SIB:
		eh = (সংরচনা ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->বাম = 0;
		path->visit_অঙ্ক = 0;
		ভঙ্গ;
	পক্ষ EXT2_EXTENT_UP:
		যদি (handle->level <= 0)
			ঘূৰাই EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		যদি ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_অঙ্ক = 0;
		ভঙ্গ;
	পক্ষ EXT2_EXTENT_DOWN:
	পক্ষ EXT2_EXTENT_DOWN_AND_LAST:
		যদি (!path->curr ||(handle->level >= handle->max_depth))
			ঘূৰাই EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		যদি (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			যদি (retval)
				ঘূৰাই retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		বাবে (l = handle->level, tp = path; l > 0; l--, tp--) {
			যদি (blk == tp->blk)
				ঘূৰাই EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		যদি ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		অন্যথা {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			যদি (retval)
				ঘূৰাই retval;
		}
		handle->level++;

		eh = (সংরচনা ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		যদি (retval) {
			handle->level--;
			ঘূৰাই retval;
		}

		যদি (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->বাম = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		যদি (newpath->বাম <= 0)
			ঘূৰাই EXT2_ET_EXTENT_NO_DOWN;

		যদি (path->বাম > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} অন্যথা
			newpath->end_blk = path->end_blk;

		path = newpath;
		যদি (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((সংরচনা ext3_extent_header *) eh);
			path->curr = ix;
			path->বাম = path->entries - 1;
			path->visit_অঙ্ক = 0;
		} অন্যথা {
			ix = EXT_LAST_INDEX((সংরচনা ext3_extent_header *) eh);
			path->curr = ix;
			path->বাম = 0;
			যদি (handle->level < handle->max_depth)
				path->visit_অঙ্ক = 1;
		}
#ifdef DEBUG_GET_EXTENT
		printf("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		ভঙ্গ;
	স্ৱভাৱ:
		ঘূৰাই EXT2_ET_OP_NOT_SUPPORTED;
	}

	যদি (!ix)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	printf("(Left %d)\n", path->বাম);
#endif

	যদি (handle->level == handle->max_depth) {
		ex = (সংরচনা ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		যদি (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} অন্যথা {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		যদি (path->বাম > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} অন্যথা
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	যদি (path->visit_অঙ্ক)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	যদি (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		গচ্ছ retry;

	যদি ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->বাম != 0)))
		গচ্ছ retry;

	যদি (failed_csum)
		ঘূৰাই EXT2_ET_EXTENT_CSUM_INVALID;

	ঘূৰাই 0;
}

স্থির errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	সংরচনা ext3_extent_idx		*ix;
	সংরচনা ext3_extent_header	*eh;

	যদি (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} অন্যথা {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (সংরচনা ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		যদি (retval)
			ঘূৰাই retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	ঘূৰাই retval;
}

#যদি 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	সংরচনা ext2fs_extent	extent;
	সংরচনা ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	যদি (retval)
		ঘূৰাই retval;

	retval = ext2fs_extent_get_info(handle, &info);
	যদি (retval)
		ঘূৰাই retval;

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা ext2_extent_path), &save_path);
	যদি (retval)
		ঘূৰাই retval;
	memset(save_path, 0, পরিমাণ(সংরচনা ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	ঘূৰাই 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	ঘূৰাই 0;
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
			      পূর্ণাঙ্ক leaf_level, blk64_t blk)
{
	সংরচনা ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_মূল, &extent);
	যদি (retval) {
		যদি (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		ঘূৰাই retval;
	}

	যদি (leaf_level > handle->max_depth) {
#ifdef DEBUG
		printf("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		ঘূৰাই EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	printf("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	যেতিয়ালৈ (1) {
		যদি (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			যদি ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				ঘূৰাই 0;
			যদি (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				ঘূৰাই EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			যদি (retval == EXT2_ET_EXTENT_NO_NEXT)
				ঘূৰাই EXT2_ET_EXTENT_NOT_FOUND;
			যদি (retval)
				ঘূৰাই retval;
			অনুৱর্ত;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		যদি (retval == EXT2_ET_EXTENT_NO_NEXT)
			গচ্ছ go_down;
		যদি (retval)
			ঘূৰাই retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		যদি (blk == extent.e_lblk)
			গচ্ছ go_down;
		যদি (blk > extent.e_lblk)
			অনুৱর্ত;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		যদি (retval)
			ঘূৰাই retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		যদি (retval)
			ঘূৰাই retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	ঘূৰাই ext2fs_extent_goto2(handle, 0, blk);
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
	পূর্ণাঙ্ক				retval = 0;
	পূর্ণাঙ্ক				orig_height;
	blk64_t				start;
	সংরচনা extent_path		*path;
	সংরচনা ext2fs_extent		extent;
	সংরচনা ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	যদি (!(handle->fs->flags & EXT2_FLAG_RW))
		ঘূৰাই EXT2_ET_RO_FILSYS;

	যদি (!handle->path)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

	path = handle->path + handle->level;
	যদি (!path->curr)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	যদি (retval)
		গচ্ছ done;

	/* modified node's start block */
	start = extent.e_lblk;

	যদি ((retval = ext2fs_extent_get_info(handle, &info)))
		ঘূৰাই retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	যেতিয়ালৈ (handle->level > 0 &&
	       (path->বাম == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		যদি (retval)
			গচ্ছ done;
		যদি (extent.e_lblk == start)
			ভঙ্গ;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		যদি (retval)
			গচ্ছ done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	ঘূৰাই retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				পূর্ণাঙ্ক flags EXT2FS_ATTR((unused)),
				সংরচনা ext2fs_extent *extent)
{
	সংরচনা extent_path		*path;
	সংরচনা ext3_extent_idx		*ix;
	সংরচনা ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	যদি (!(handle->fs->flags & EXT2_FLAG_RW))
		ঘূৰাই EXT2_ET_RO_FILSYS;

	যদি (!handle->path)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

	path = handle->path + handle->level;
	যদি (!path->curr)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

#ifdef DEBUG
	printf("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	যদি (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		যদি (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			যদি (extent->e_len > EXT_UNINIT_MAX_LEN)
				ঘূৰাই EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} অন্যথা {
			যদি (extent->e_len > EXT_INIT_MAX_LEN)
				ঘূৰাই EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} অন্যথা {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক splitting_at_eof(সংরচনা ext2_extent_handle *handle,
			    সংরচনা extent_path *path)
{
	সংরচনা extent_path *ppp = path;
	dump_path(__func__, handle, path);

	যদি (handle->level == 0)
		ঘূৰাই 0;

	কৰা {
		যদি (ppp->বাম)
			ঘূৰাই 0;
		ppp--;
	} যেতিয়ালৈ (ppp >= handle->path);

	ঘূৰাই 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
স্থির errcode_t extent_গ্ৰন্থি_split(ext2_extent_handle_t handle,
				   পূর্ণাঙ্ক expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_গ্ৰন্থি_pblk;
	blk64_t				new_গ্ৰন্থি_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	পূর্ণাঙ্ক				orig_height;
	ৱর্ণ				*block_buf = রিক্ত;
	সংরচনা ext2fs_extent		extent;
	সংরচনা extent_path		*path, *newpath = 0;
	সংরচনা ext3_extent_header	*eh, *neweh;
	পূর্ণাঙ্ক				tocopy;
	পূর্ণাঙ্ক				new_মূল = 0;
	সংরচনা ext2_extent_info		info;
	পূর্ণাঙ্ক				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	যদি (!(handle->fs->flags & EXT2_FLAG_RW))
		ঘূৰাই EXT2_ET_RO_FILSYS;

	যদি (!handle->path)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

#ifdef DEBUG
	printf("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	যদি (retval)
		গচ্ছ done;

	retval = ext2fs_extent_get_info(handle, &info);
	যদি (retval)
		গচ্ছ done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (সংরচনা ext3_extent_header *) path->buf;
	যদি (handle->level == handle->max_depth) {
		সংরচনা ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} অন্যথা {
		সংরচনা ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	যদি (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		printf("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		যদি (retval)
			গচ্ছ done;

		retval = extent_গ্ৰন্থি_split(handle, expand_allowed);
		যদি (retval)
			গচ্ছ done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		যদি (retval)
			গচ্ছ done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	যদি (!path->curr)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (সংরচনা ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	যদি (handle->level == 0) {
		new_মূল = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    পরিমাণ(সংরচনা extent_path),
					    &newpath);
		যদি (retval)
			গচ্ছ done;
	} অন্যথা {
		যদি (no_balance)
			tocopy = 1;
		অন্যথা
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	printf("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	যদি (!tocopy && !no_balance) {
#ifdef DEBUG
		printf("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		গচ্ছ done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = malloc(handle->fs->blocksize);
	যদি (!block_buf) {
		retval = ENOMEM;
		গচ্ছ done;
	}

	যদি (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_গ্ৰন্থি_pblk);
	যদি (retval)
		গচ্ছ done;

#ifdef DEBUG
	printf("will copy to new node at block %lu\n",
	       (unsigned long) new_গ্ৰন্থি_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (সংরচনা ext3_extent_header *) block_buf;
	memcpy(neweh, eh, পরিমাণ(সংরচনা ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 পরিমাণ(সংরচনা ext3_extent_header)) /
				পরিমাণ(সংরচনা ext3_extent));

	/* then the entries for the new block... */
	memcpy(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		পরিমাণ(সংরচনা ext3_extent_idx) * tocopy);

	new_গ্ৰন্থি_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	যদি (retval)
		গচ্ছ done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_গ্ৰন্থি_pblk, 1,
					block_buf);

	যদি (retval)
		গচ্ছ done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	যদি (handle->level == 0) {
		memcpy(newpath, path,
		       পরিমাণ(সংরচনা extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->বাম = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} অন্যথা {
		path->entries -= tocopy;
		path->বাম -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	যদি (retval)
		গচ্ছ done;

	/* now go up and insert/replace index for new node we created */
	যদি (new_মূল) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		যদি (retval)
			গচ্ছ done;

		extent.e_lblk = new_গ্ৰন্থি_start;
		extent.e_pblk = new_গ্ৰন্থি_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		যদি (retval)
			গচ্ছ done;
	} অন্যথা {
		__u32 new_গ্ৰন্থি_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_গ্ৰন্থি_length = new_গ্ৰন্থি_start - extent.e_lblk;
		extent.e_len -= new_গ্ৰন্থি_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		যদি (retval)
			গচ্ছ done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_গ্ৰন্থি_start;
		extent.e_pblk = new_গ্ৰন্থি_pblk;
		extent.e_len = new_গ্ৰন্থি_length;
		retval = ext2fs_extent_ঢোকাও(handle, EXT2_EXTENT_ঢোকাও_AFTER, &extent);
		যদি (retval)
			গচ্ছ done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	যদি (retval)
		গচ্ছ done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	যদি (retval)
		গচ্ছ done;

done:
	যদি (newpath)
		ext2fs_free_mem(&newpath);
	মুক্ত(block_buf);

	ঘূৰাই retval;
}

errcode_t ext2fs_extent_গ্ৰন্থি_split(ext2_extent_handle_t handle)
{
	ঘূৰাই extent_গ্ৰন্থি_split(handle, 0);
}

errcode_t ext2fs_extent_ঢোকাও(ext2_extent_handle_t handle, পূর্ণাঙ্ক flags,
				      সংরচনা ext2fs_extent *extent)
{
	সংরচনা extent_path		*path;
	সংরচনা ext3_extent_idx		*ix;
	সংরচনা ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	যদি (!(handle->fs->flags & EXT2_FLAG_RW))
		ঘূৰাই EXT2_ET_RO_FILSYS;

	যদি (!handle->path)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

#ifdef DEBUG
	printf("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	যদি (path->entries >= path->max_entries) {
		যদি (flags & EXT2_EXTENT_ঢোকাও_NOSPLIT) {
			ঘূৰাই EXT2_ET_CANT_ঢোকাও_EXTENT;
		} অন্যথা {
#ifdef DEBUG
			printf("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_গ্ৰন্থি_split(handle, 1);
			যদি (retval)
				ঘূৰাই retval;
			path = handle->path + handle->level;
		}
	}

	eh = (সংরচনা ext3_extent_header *) path->buf;
	যদি (path->curr) {
		ix = path->curr;
		যদি (flags & EXT2_EXTENT_ঢোকাও_AFTER) {
			ix++;
			path->বাম--;
		}
	} অন্যথা {
		ix = EXT_FIRST_INDEX(eh);
		path->বাম = -1;
	}

	path->curr = ix;

	যদি (path->বাম >= 0)
		memmove(ix + 1, ix,
			(path->বাম+1) * পরিমাণ(সংরচনা ext3_extent_idx));
	path->বাম++;
	path->entries++;

	eh = (সংরচনা ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	যদি (retval)
		গচ্ছ errout;

	retval = update_path(handle);
	যদি (retval)
		গচ্ছ errout;

	ঘূৰাই 0;

errout:
	ext2fs_extent_delete(handle, 0);
	ঘূৰাই retval;
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
				 blk64_t logical, blk64_t physical, পূর্ণাঙ্ক flags)
{
	errcode_t		ec, retval = 0;
	পূর্ণাঙ্ক			mapped = 1; /* logical is mapped? */
	পূর্ণাঙ্ক			orig_height;
	পূর্ণাঙ্ক			extent_uninit = 0;
	পূর্ণাঙ্ক			prev_uninit = 0;
	পূর্ণাঙ্ক			next_uninit = 0;
	পূর্ণাঙ্ক			new_uninit = 0;
	পূর্ণাঙ্ক			max_len = EXT_INIT_MAX_LEN;
	পূর্ণাঙ্ক			has_prev, has_next;
	blk64_t			orig_lblk;
	সংরচনা extent_path	*path;
	সংরচনা ext2fs_extent	extent, next_extent, prev_extent;
	সংরচনা ext2fs_extent	newextent;
	সংরচনা ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	printf("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	যদি (!(handle->fs->flags & EXT2_FLAG_RW))
		ঘূৰাই EXT2_ET_RO_FILSYS;

	যদি (!handle->path)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

	path = handle->path + handle->level;

	যদি (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	যদি (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		যদি (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	যদি ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_ঢোকাও(handle, 0, &newextent);
		ঘূৰাই retval;
	}

	/* save our original location in the extent tree */
	যদি ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		যদি (retval != EXT2_ET_NO_CURRENT_গ্ৰন্থি)
			ঘূৰাই retval;
		memset(&extent, 0, পরিমাণ(extent));
	}
	যদি ((retval = ext2fs_extent_get_info(handle, &info)))
		ঘূৰাই retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	যদি (retval) {
		যদি (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			যদি (!physical) {
#ifdef DEBUG
				printf("block %llu already unmapped\n",
					logical);
#endif
				গচ্ছ done;
			}
		} অন্যথা
			গচ্ছ done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	যদি (retval)
		গচ্ছ done;
	যদি (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	যদি (retval) {
		has_next = 0;
		যদি (retval != EXT2_ET_EXTENT_NO_NEXT)
			গচ্ছ done;
	} অন্যথা {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		যদি (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	যদি (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		গচ্ছ done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	যদি (retval) {
		has_prev = 0;
		যদি (retval != EXT2_ET_EXTENT_NO_PREV)
			গচ্ছ done;
	} অন্যথা {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		যদি (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	যদি (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		গচ্ছ done;

	/* check if already pointing to the requested physical */
	যদি (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		printf("physical block (at %llu) unchanged\n", logical);
#endif
		গচ্ছ done;
	}

	যদি (!mapped) {
#ifdef DEBUG
		printf("mapping unmapped logical block %llu\n", logical);
#endif
		যদি ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((পূর্ণাঙ্ক) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} অন্যথা যদি ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((পূর্ণাঙ্ক) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} অন্যথা যদি (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((পূর্ণাঙ্ক) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			যদি (retval)
				গচ্ছ done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} অন্যথা যদি (logical < extent.e_lblk)
			retval = ext2fs_extent_ঢোকাও(handle, 0, &newextent);
		অন্যথা
			retval = ext2fs_extent_ঢোকাও(handle,
				      EXT2_EXTENT_ঢোকাও_AFTER, &newextent);
		যদি (retval)
			গচ্ছ done;
		retval = ext2fs_extent_fix_parents(handle);
		যদি (retval)
			গচ্ছ done;
	} অন্যথা যদি ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		printf("(re/un)mapping only block in extent\n");
#endif
		যদি (physical) {
			যদি (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((পূর্ণাঙ্ক) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				যদি (retval)
					গচ্ছ done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				যদি (retval)
					গচ্ছ done;
				গচ্ছ delete_গ্ৰন্থি;

			} অন্যথা
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} অন্যথা {
		delete_গ্ৰন্থি:
			retval = ext2fs_extent_delete(handle, 0);
			যদি (retval)
				গচ্ছ done;
			ec = ext2fs_extent_fix_parents(handle);
			যদি (ec != EXT2_ET_NO_CURRENT_গ্ৰন্থি)
				retval = ec;
		}

		যদি (retval)
			গচ্ছ done;
	} অন্যথা যদি (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		printf("(re/un)mapping last block in extent\n");
#endif
		যদি (physical) {
			যদি (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((পূর্ণাঙ্ক) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				যদি (retval)
					গচ্ছ done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				যদি (retval)
					গচ্ছ done;
			} অন্যথা
				retval = ext2fs_extent_ঢোকাও(handle,
				      EXT2_EXTENT_ঢোকাও_AFTER, &newextent);
			যদি (retval)
				গচ্ছ done;
			retval = ext2fs_extent_fix_parents(handle);
			যদি (retval)
				গচ্ছ done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			যদি (retval)
				গচ্ছ done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			যদি (retval)
				গচ্ছ done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		যদি (retval)
			গচ্ছ done;
	} অন্যথা যদি (logical == extent.e_lblk) {
#ifdef DEBUG
		printf("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		যদি (retval)
			গচ্ছ done;
		retval = ext2fs_extent_fix_parents(handle);
		যদি (retval)
			গচ্ছ done;
		যদি (physical) {
			যদি (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((পূর্ণাঙ্ক) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				যদি (retval)
					গচ্ছ done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} অন্যথা
				retval = ext2fs_extent_ঢোকাও(handle,
							      0, &newextent);
			যদি (retval)
				গচ্ছ done;
			retval = ext2fs_extent_fix_parents(handle);
			যদি (retval)
				গচ্ছ done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			যদি (retval)
				গচ্ছ done;
		}
	} অন্যথা {
		__u32	save_length;
		blk64_t	save_lblk;
		সংরচনা ext2fs_extent save_extent;
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
		যদি (retval)
			গচ্ছ done;
		/* insert our new extent, if any */
		যদি (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_ঢোকাও(handle,
					EXT2_EXTENT_ঢোকাও_AFTER, &newextent);
			যদি (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				যদি (r2 == 0)
					(শূন্য)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				গচ্ছ done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_length - extent.e_len - 1;
		retval = ext2fs_extent_ঢোকাও(handle,
				EXT2_EXTENT_ঢোকাও_AFTER, &extent);
		যদি (retval) {
			যদি (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				যদি (r2 == 0)
					(শূন্য)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			যদি (r2 == 0)
				(শূন্য)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			গচ্ছ done;
		}
	}

done:
	/* get handle back to its position */
	যদি (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	ঘূৰাই retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, পূর্ণাঙ্ক flags)
{
	সংরচনা extent_path		*path;
	ৱর্ণ 				*cp;
	সংরচনা ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	যদি (!(handle->fs->flags & EXT2_FLAG_RW))
		ঘূৰাই EXT2_ET_RO_FILSYS;

	যদি (!handle->path)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

#ifdef DEBUG
	{
		সংরচনা ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		যদি (retval == 0) {
			printf("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	যদি (!path->curr)
		ঘূৰাই EXT2_ET_NO_CURRENT_গ্ৰন্থি;

	cp = path->curr;

	/* Sanity check before memmove() */
	যদি (path->বাম < 0)
		ঘূৰাই EXT2_ET_EXTENT_LEAF_BAD;

	যদি (path->বাম) {
		memmove(cp, cp + পরিমাণ(সংরচনা ext3_extent_idx),
			path->বাম * পরিমাণ(সংরচনা ext3_extent_idx));
		path->বাম--;
	} অন্যথা {
		সংরচনা ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	যদি (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	যদি (path->entries == 0 && handle->level) {
		যদি (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			সংরচনা ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			যদি (retval)
				ঘূৰাই retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} অন্যথা {
		eh = (সংরচনা ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		যদি ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	ঘূৰাই retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 সংরচনা ext2_extent_info *info)
{
	সংরচনা extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, পরিমাণ(সংরচনা ext2_extent_info));

	path = handle->path + handle->level;
	যদি (path) {
		যদি (path->curr)
			info->curr_entry = ((ৱর্ণ *) path->curr - path->buf) /
				পরিমাণ(সংরচনা ext3_extent_idx);
		অন্যথা
			info->curr_entry = 0;
		info->অঙ্ক_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			পরিমাণ(সংরচনা ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	ঘূৰাই 0;
}

size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	size_t iblock_sz = পরিমাণ(((সংরচনা ext2_inode *)রিক্ত)->i_block);
	size_t iblock_extents = (iblock_sz - পরিমাণ(সংরচনা ext3_extent_header)) /
				পরিমাণ(সংরচনা ext3_extent);
	size_t extents_per_block = (handle->fs->blocksize -
				    পরিমাণ(সংরচনা ext3_extent_header)) /
				   পরিমাণ(সংরচনা ext3_extent);
	স্থির unsigned পূর্ণাঙ্ক last_blocksize = 0;
	স্থির size_t last_result = 0;

	যদি (last_blocksize && last_blocksize == handle->fs->blocksize)
		ঘূৰাই last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	ঘূৰাই last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       সংরচনা ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	সংরচনা ext2fs_extent	extent;
	errcode_t		errcode;
	পূর্ণাঙ্ক    			save_flags = fs->flags;

	যদি (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		ঘূৰাই 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	যদি (errcode) {
		যদি (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		ঘূৰাই errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_মূল, &extent);
	যদি (errcode)
		গচ্ছ out;

	কৰা {
		/* Skip to the end of a block of leaf nodes */
		যদি (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			যদি (errcode)
				ভঙ্গ;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		যদি (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} যেতিয়ালৈ (errcode == 0);

out:
	/* Ok if we run off the end */
	যদি (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	ঘূৰাই errcode;
}

errcode_t ext2fs_decode_extent(সংরচনা ext2fs_extent *to, শূন্য *addr, পূর্ণাঙ্ক len)
{
	সংরচনা ext3_extent *from = (সংরচনা ext3_extent *)addr;

	যদি (len != পরিমাণ(সংরচনা ext3_extent))
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	যদি (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	ঘূৰাই 0;
}

errcode_t ext2fs_গুনতি_blocks(ext2_filsys fs, ext2_ino_t ino,
			      সংরচনা ext2_inode *inode, blk64_t *ret_গুনতি)
{
	ext2_extent_handle_t	handle = রিক্ত;
	সংরচনা ext2fs_extent	extent;
	errcode_t		errcode;
	পূর্ণাঙ্ক			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	যদি (errcode)
		গচ্ছ out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_মূল, &extent);
	যদি (errcode)
		গচ্ছ out;

	errcode = ext2fs_get_array(handle->max_depth, পরিমাণ(blk64_t),
				   &intermediate_nodes);
	যদি (errcode)
		গচ্ছ out;

	blkcount = handle->level;
	যেতিয়ালৈ (!errcode) {
		যদি (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			বাবে (i = 0; i < handle->level; i++) {
				যদি (intermediate_nodes[i] !=
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
	*ret_গুনতি = blkcount;
	ext2fs_extent_free(handle);

	ঘূৰাই 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
ধ্রুৱ ৱর্ণ *debug_prog_name = "tst_extents";

#endif

