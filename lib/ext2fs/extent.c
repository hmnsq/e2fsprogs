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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"
#અન્તર્ભાવ "e2image.h"

#undef DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

સંરચના extent_path {
	અક્ષર		*buf;
	પૂર્ણાંક		entries;
	પૂર્ણાંક		max_entries;
	પૂર્ણાંક		ડાબું;
	પૂર્ણાંક		visit_અંક;
	પૂર્ણાંક		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	શૂન્ય		*curr;
};


સંરચના ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	સંરચના ext2_inode	*inode;
	સંરચના ext2_inode	inodebuf;
	પૂર્ણાંક			type;
	પૂર્ણાંક			level;
	પૂર્ણાંક			max_depth;
	પૂર્ણાંક			max_paths;
	સંરચના extent_path	*path;
};

સંરચના ext2_extent_path {
	errcode_t		magic;
	પૂર્ણાંક			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
સ્થિર શૂન્ય dbg_show_header(સંરચના ext3_extent_header *eh)
{
	printf("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

સ્થિર શૂન્ય dbg_show_index(સંરચના ext3_extent_idx *ix)
{
	printf("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

સ્થિર શૂન્ય dbg_show_extent(સંરચના ext3_extent *ex)
{
	printf("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

સ્થિર શૂન્ય dbg_print_extent(અક્ષર *desc, સંરચના ext2fs_extent *extent)
{
	જો (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	જો (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	જો (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	જો (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	જો (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

સ્થિર શૂન્ય dump_path(અચલ અક્ષર *tag, સંરચના ext2_extent_handle *handle,
		      સંરચના extent_path *path)
{
	સંરચના extent_path *ppp = path;
	printf("%s: level=%d\n", tag, handle->level);

	કરો {
		printf("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->ડાબું, ppp->visit_અંક, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (શૂન્ય *)ppp->buf);
		printf("  ");
		dbg_show_header((સંરચના ext3_extent_header *)ppp->buf);
		જો (ppp->curr) {
			printf("  ");
			dbg_show_index(ppp->curr);
			printf("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} જ્યારે (ppp >= handle->path);
	fflush(stdout);

	પાછા;
}

#નહીંતર
#define dbg_show_header(eh) કરો { } જ્યારે (0)
#define dbg_show_index(ix) કરો { } જ્યારે (0)
#define dbg_show_extent(ex) કરો { } જ્યારે (0)
#define dbg_print_extent(desc, ex) કરો { } જ્યારે (0)
#define dump_path(tag, handle, path) કરો { } જ્યારે (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(શૂન્ય *ptr, પૂર્ણાંક size)
{
	પૂર્ણાંક eh_max, entry_size;
	સંરચના ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	જો (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		પાછા EXT2_ET_EXTENT_HEADER_BAD;
	જો (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		પાછા EXT2_ET_EXTENT_HEADER_BAD;
	જો (eh->eh_depth == 0)
		entry_size = કદ(સંરચના ext3_extent);
	નહીંતર
		entry_size = કદ(સંરચના ext3_extent_idx);

	eh_max = (size - કદ(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	જો ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		પાછા EXT2_ET_EXTENT_HEADER_BAD;

	પાછા 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
શૂન્ય ext2fs_extent_free(ext2_extent_handle_t handle)
{
	પૂર્ણાંક			i;

	જો (!handle)
		પાછા;

	જો (handle->path) {
		માટે (i = 1; i < handle->max_paths; i++) {
			જો (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	પાછા ext2fs_extent_open2(fs, ino, ખાલી, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    સંરચના ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	સંરચના ext2_extent_handle	*handle;
	errcode_t			retval;
	પૂર્ણાંક				i;
	સંરચના ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (!inode)
		જો ((ino == 0) || (ino > fs->super->s_inodes_ગણતરી))
			પાછા EXT2_ET_BAD_INODE_અંક;

	retval = ext2fs_get_mem(કદ(સંરચના ext2_extent_handle), &handle);
	જો (retval)
		પાછા retval;
	memset(handle, 0, કદ(સંરચના ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	જો (inode) {
		handle->inode = inode;
	} નહીંતર {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		જો (retval)
			જાઓ errout;
	}

	eh = (સંરચના ext3_extent_header *) &handle->inode->i_block[0];

	માટે (i=0; i < EXT2_N_BLOCKS; i++)
		જો (handle->inode->i_block[i])
			તોડો;
	જો (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (કદ(handle->inode->i_block) - કદ(*eh)) /
			કદ(સંરચના ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	જો (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		જાઓ errout;
	}

	retval = ext2fs_extent_header_verify(eh, કદ(handle->inode->i_block));
	જો (retval)
		જાઓ errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    કદ(સંરચના extent_path),
				    &handle->path);
	handle->path[0].buf = (અક્ષર *) handle->inode->i_block;

	handle->path[0].ડાબું = handle->path[0].entries =
		ext2fs_le16_to_cpu(eh->eh_entries);
	handle->path[0].max_entries = ext2fs_le16_to_cpu(eh->eh_max);
	handle->path[0].curr = 0;
	handle->path[0].end_blk =
		(EXT2_I_SIZE(handle->inode) + fs->blocksize - 1) >>
		 EXT2_BLOCK_SIZE_BITS(fs->super);
	handle->path[0].blk = 0;
	handle->path[0].visit_અંક = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	પાછા 0;

errout:
	ext2fs_extent_free(handle);
	પાછા retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    પૂર્ણાંક flags, સંરચના ext2fs_extent *extent)
{
	સંરચના extent_path	*path, *newpath, *tp;
	સંરચના ext3_extent_header	*eh;
	સંરચના ext3_extent_idx		*ix = 0;
	સંરચના ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	પૂર્ણાંક				orig_op, op, l;
	પૂર્ણાંક				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	જો (!handle->path)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	જો ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		જો (handle->level < handle->max_depth) {
			/* interior node */
			જો (path->visit_અંક == 0) {
				path->visit_અંક++;
				op = EXT2_EXTENT_DOWN;
			} નહીંતર જો (path->ડાબું > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			નહીંતર જો (handle->level > 0)
				op = EXT2_EXTENT_UP;
			નહીંતર
				પાછા EXT2_ET_EXTENT_NO_NEXT;
		} નહીંતર {
			/* leaf node */
			જો (path->ડાબું > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			નહીંતર જો (handle->level > 0)
				op = EXT2_EXTENT_UP;
			નહીંતર
				પાછા EXT2_ET_EXTENT_NO_NEXT;
		}
		જો (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	જો ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		જો (handle->level < handle->max_depth) {
			/* interior node */
			જો (path->visit_અંક > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} નહીંતર જો (path->ડાબું < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			નહીંતર જો (handle->level > 0)
				op = EXT2_EXTENT_UP;
			નહીંતર
				પાછા EXT2_ET_EXTENT_NO_PREV;
		} નહીંતર {
			/* leaf node */
			જો (path->ડાબું < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			નહીંતર જો (handle->level > 0)
				op = EXT2_EXTENT_UP;
			નહીંતર
				પાછા EXT2_ET_EXTENT_NO_PREV;
		}
		જો (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	જો (orig_op == EXT2_EXTENT_LAST_LEAF) {
		જો ((handle->level < handle->max_depth) &&
		    (path->ડાબું == 0))
			op = EXT2_EXTENT_DOWN;
		નહીંતર
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		printf("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	પસંદગી (op) {
	સ્થિતિ EXT2_EXTENT_CURRENT:
		ix = path->curr;
		તોડો;
	સ્થિતિ EXT2_EXTENT_મૂળ:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	સ્થિતિ EXT2_EXTENT_FIRST_SIB:
		path->ડાબું = path->entries;
		path->curr = 0;
		/* fallthrough */
	સ્થિતિ EXT2_EXTENT_NEXT_SIB:
		જો (path->ડાબું <= 0)
			પાછા EXT2_ET_EXTENT_NO_NEXT;
		જો (path->curr) {
			ix = path->curr;
			ix++;
		} નહીંતર {
			eh = (સંરચના ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->ડાબું--;
		path->curr = ix;
		path->visit_અંક = 0;
		તોડો;
	સ્થિતિ EXT2_EXTENT_PREV_SIB:
		જો (!path->curr ||
		    path->ડાબું+1 >= path->entries)
			પાછા EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->ડાબું++;
		જો (handle->level < handle->max_depth)
			path->visit_અંક = 1;
		તોડો;
	સ્થિતિ EXT2_EXTENT_LAST_SIB:
		eh = (સંરચના ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->ડાબું = 0;
		path->visit_અંક = 0;
		તોડો;
	સ્થિતિ EXT2_EXTENT_UP:
		જો (handle->level <= 0)
			પાછા EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		જો ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_અંક = 0;
		તોડો;
	સ્થિતિ EXT2_EXTENT_DOWN:
	સ્થિતિ EXT2_EXTENT_DOWN_AND_LAST:
		જો (!path->curr ||(handle->level >= handle->max_depth))
			પાછા EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		જો (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			જો (retval)
				પાછા retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		માટે (l = handle->level, tp = path; l > 0; l--, tp--) {
			જો (blk == tp->blk)
				પાછા EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		જો ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		નહીંતર {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			જો (retval)
				પાછા retval;
		}
		handle->level++;

		eh = (સંરચના ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		જો (retval) {
			handle->level--;
			પાછા retval;
		}

		જો (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->ડાબું = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		જો (newpath->ડાબું <= 0)
			પાછા EXT2_ET_EXTENT_NO_DOWN;

		જો (path->ડાબું > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} નહીંતર
			newpath->end_blk = path->end_blk;

		path = newpath;
		જો (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((સંરચના ext3_extent_header *) eh);
			path->curr = ix;
			path->ડાબું = path->entries - 1;
			path->visit_અંક = 0;
		} નહીંતર {
			ix = EXT_LAST_INDEX((સંરચના ext3_extent_header *) eh);
			path->curr = ix;
			path->ડાબું = 0;
			જો (handle->level < handle->max_depth)
				path->visit_અંક = 1;
		}
#ifdef DEBUG_GET_EXTENT
		printf("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		તોડો;
	મૂળભૂત:
		પાછા EXT2_ET_OP_NOT_SUPPORTED;
	}

	જો (!ix)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	printf("(Left %d)\n", path->ડાબું);
#endif

	જો (handle->level == handle->max_depth) {
		ex = (સંરચના ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		જો (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} નહીંતર {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		જો (path->ડાબું > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} નહીંતર
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	જો (path->visit_અંક)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	જો (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		જાઓ retry;

	જો ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->ડાબું != 0)))
		જાઓ retry;

	જો (failed_csum)
		પાછા EXT2_ET_EXTENT_CSUM_INVALID;

	પાછા 0;
}

સ્થિર errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	સંરચના ext3_extent_idx		*ix;
	સંરચના ext3_extent_header	*eh;

	જો (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} નહીંતર {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (સંરચના ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		જો (retval)
			પાછા retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	પાછા retval;
}

#જો 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	સંરચના ext2fs_extent	extent;
	સંરચના ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	જો (retval)
		પાછા retval;

	retval = ext2fs_extent_get_info(handle, &info);
	જો (retval)
		પાછા retval;

	retval = ext2fs_get_mem(કદ(સંરચના ext2_extent_path), &save_path);
	જો (retval)
		પાછા retval;
	memset(save_path, 0, કદ(સંરચના ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	પાછા 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	પાછા 0;
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
			      પૂર્ણાંક leaf_level, blk64_t blk)
{
	સંરચના ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_મૂળ, &extent);
	જો (retval) {
		જો (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		પાછા retval;
	}

	જો (leaf_level > handle->max_depth) {
#ifdef DEBUG
		printf("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		પાછા EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	printf("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	જ્યારે (1) {
		જો (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			જો ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				પાછા 0;
			જો (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				પાછા EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			જો (retval == EXT2_ET_EXTENT_NO_NEXT)
				પાછા EXT2_ET_EXTENT_NOT_FOUND;
			જો (retval)
				પાછા retval;
			ચાલુ;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		જો (retval == EXT2_ET_EXTENT_NO_NEXT)
			જાઓ go_down;
		જો (retval)
			પાછા retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		જો (blk == extent.e_lblk)
			જાઓ go_down;
		જો (blk > extent.e_lblk)
			ચાલુ;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		જો (retval)
			પાછા retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		જો (retval)
			પાછા retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	પાછા ext2fs_extent_goto2(handle, 0, blk);
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
	પૂર્ણાંક				retval = 0;
	પૂર્ણાંક				orig_height;
	blk64_t				start;
	સંરચના extent_path		*path;
	સંરચના ext2fs_extent		extent;
	સંરચના ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	જો (!(handle->fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

	જો (!handle->path)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

	path = handle->path + handle->level;
	જો (!path->curr)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	જો (retval)
		જાઓ done;

	/* modified node's start block */
	start = extent.e_lblk;

	જો ((retval = ext2fs_extent_get_info(handle, &info)))
		પાછા retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	જ્યારે (handle->level > 0 &&
	       (path->ડાબું == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		જો (retval)
			જાઓ done;
		જો (extent.e_lblk == start)
			તોડો;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		જો (retval)
			જાઓ done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	પાછા retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				પૂર્ણાંક flags EXT2FS_ATTR((unused)),
				સંરચના ext2fs_extent *extent)
{
	સંરચના extent_path		*path;
	સંરચના ext3_extent_idx		*ix;
	સંરચના ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	જો (!(handle->fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

	જો (!handle->path)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

	path = handle->path + handle->level;
	જો (!path->curr)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

#ifdef DEBUG
	printf("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	જો (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		જો (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			જો (extent->e_len > EXT_UNINIT_MAX_LEN)
				પાછા EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} નહીંતર {
			જો (extent->e_len > EXT_INIT_MAX_LEN)
				પાછા EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} નહીંતર {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	પાછા 0;
}

સ્થિર પૂર્ણાંક splitting_at_eof(સંરચના ext2_extent_handle *handle,
			    સંરચના extent_path *path)
{
	સંરચના extent_path *ppp = path;
	dump_path(__func__, handle, path);

	જો (handle->level == 0)
		પાછા 0;

	કરો {
		જો (ppp->ડાબું)
			પાછા 0;
		ppp--;
	} જ્યારે (ppp >= handle->path);

	પાછા 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
સ્થિર errcode_t extent_ગાંઠ_split(ext2_extent_handle_t handle,
				   પૂર્ણાંક expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_ગાંઠ_pblk;
	blk64_t				new_ગાંઠ_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	પૂર્ણાંક				orig_height;
	અક્ષર				*block_buf = ખાલી;
	સંરચના ext2fs_extent		extent;
	સંરચના extent_path		*path, *newpath = 0;
	સંરચના ext3_extent_header	*eh, *neweh;
	પૂર્ણાંક				tocopy;
	પૂર્ણાંક				new_મૂળ = 0;
	સંરચના ext2_extent_info		info;
	પૂર્ણાંક				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	જો (!(handle->fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

	જો (!handle->path)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

#ifdef DEBUG
	printf("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	જો (retval)
		જાઓ done;

	retval = ext2fs_extent_get_info(handle, &info);
	જો (retval)
		જાઓ done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (સંરચના ext3_extent_header *) path->buf;
	જો (handle->level == handle->max_depth) {
		સંરચના ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} નહીંતર {
		સંરચના ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	જો (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		printf("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		જો (retval)
			જાઓ done;

		retval = extent_ગાંઠ_split(handle, expand_allowed);
		જો (retval)
			જાઓ done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		જો (retval)
			જાઓ done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	જો (!path->curr)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (સંરચના ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	જો (handle->level == 0) {
		new_મૂળ = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    કદ(સંરચના extent_path),
					    &newpath);
		જો (retval)
			જાઓ done;
	} નહીંતર {
		જો (no_balance)
			tocopy = 1;
		નહીંતર
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	printf("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	જો (!tocopy && !no_balance) {
#ifdef DEBUG
		printf("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		જાઓ done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = malloc(handle->fs->blocksize);
	જો (!block_buf) {
		retval = ENOMEM;
		જાઓ done;
	}

	જો (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_ગાંઠ_pblk);
	જો (retval)
		જાઓ done;

#ifdef DEBUG
	printf("will copy to new node at block %lu\n",
	       (unsigned long) new_ગાંઠ_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (સંરચના ext3_extent_header *) block_buf;
	memcpy(neweh, eh, કદ(સંરચના ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 કદ(સંરચના ext3_extent_header)) /
				કદ(સંરચના ext3_extent));

	/* then the entries for the new block... */
	memcpy(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		કદ(સંરચના ext3_extent_idx) * tocopy);

	new_ગાંઠ_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	જો (retval)
		જાઓ done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_ગાંઠ_pblk, 1,
					block_buf);

	જો (retval)
		જાઓ done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	જો (handle->level == 0) {
		memcpy(newpath, path,
		       કદ(સંરચના extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->ડાબું = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} નહીંતર {
		path->entries -= tocopy;
		path->ડાબું -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	જો (retval)
		જાઓ done;

	/* now go up and insert/replace index for new node we created */
	જો (new_મૂળ) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		જો (retval)
			જાઓ done;

		extent.e_lblk = new_ગાંઠ_start;
		extent.e_pblk = new_ગાંઠ_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		જો (retval)
			જાઓ done;
	} નહીંતર {
		__u32 new_ગાંઠ_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_ગાંઠ_length = new_ગાંઠ_start - extent.e_lblk;
		extent.e_len -= new_ગાંઠ_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		જો (retval)
			જાઓ done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_ગાંઠ_start;
		extent.e_pblk = new_ગાંઠ_pblk;
		extent.e_len = new_ગાંઠ_length;
		retval = ext2fs_extent_ઉમેરો(handle, EXT2_EXTENT_ઉમેરો_AFTER, &extent);
		જો (retval)
			જાઓ done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	જો (retval)
		જાઓ done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	જો (retval)
		જાઓ done;

done:
	જો (newpath)
		ext2fs_free_mem(&newpath);
	મુક્ત(block_buf);

	પાછા retval;
}

errcode_t ext2fs_extent_ગાંઠ_split(ext2_extent_handle_t handle)
{
	પાછા extent_ગાંઠ_split(handle, 0);
}

errcode_t ext2fs_extent_ઉમેરો(ext2_extent_handle_t handle, પૂર્ણાંક flags,
				      સંરચના ext2fs_extent *extent)
{
	સંરચના extent_path		*path;
	સંરચના ext3_extent_idx		*ix;
	સંરચના ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	જો (!(handle->fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

	જો (!handle->path)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

#ifdef DEBUG
	printf("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	જો (path->entries >= path->max_entries) {
		જો (flags & EXT2_EXTENT_ઉમેરો_NOSPLIT) {
			પાછા EXT2_ET_CANT_ઉમેરો_EXTENT;
		} નહીંતર {
#ifdef DEBUG
			printf("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_ગાંઠ_split(handle, 1);
			જો (retval)
				પાછા retval;
			path = handle->path + handle->level;
		}
	}

	eh = (સંરચના ext3_extent_header *) path->buf;
	જો (path->curr) {
		ix = path->curr;
		જો (flags & EXT2_EXTENT_ઉમેરો_AFTER) {
			ix++;
			path->ડાબું--;
		}
	} નહીંતર {
		ix = EXT_FIRST_INDEX(eh);
		path->ડાબું = -1;
	}

	path->curr = ix;

	જો (path->ડાબું >= 0)
		memmove(ix + 1, ix,
			(path->ડાબું+1) * કદ(સંરચના ext3_extent_idx));
	path->ડાબું++;
	path->entries++;

	eh = (સંરચના ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	જો (retval)
		જાઓ errout;

	retval = update_path(handle);
	જો (retval)
		જાઓ errout;

	પાછા 0;

errout:
	ext2fs_extent_delete(handle, 0);
	પાછા retval;
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
				 blk64_t logical, blk64_t physical, પૂર્ણાંક flags)
{
	errcode_t		ec, retval = 0;
	પૂર્ણાંક			mapped = 1; /* logical is mapped? */
	પૂર્ણાંક			orig_height;
	પૂર્ણાંક			extent_uninit = 0;
	પૂર્ણાંક			prev_uninit = 0;
	પૂર્ણાંક			next_uninit = 0;
	પૂર્ણાંક			new_uninit = 0;
	પૂર્ણાંક			max_len = EXT_INIT_MAX_LEN;
	પૂર્ણાંક			has_prev, has_next;
	blk64_t			orig_lblk;
	સંરચના extent_path	*path;
	સંરચના ext2fs_extent	extent, next_extent, prev_extent;
	સંરચના ext2fs_extent	newextent;
	સંરચના ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	printf("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	જો (!(handle->fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

	જો (!handle->path)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

	path = handle->path + handle->level;

	જો (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	જો (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		જો (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	જો ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_ઉમેરો(handle, 0, &newextent);
		પાછા retval;
	}

	/* save our original location in the extent tree */
	જો ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		જો (retval != EXT2_ET_NO_CURRENT_ગાંઠ)
			પાછા retval;
		memset(&extent, 0, કદ(extent));
	}
	જો ((retval = ext2fs_extent_get_info(handle, &info)))
		પાછા retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	જો (retval) {
		જો (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			જો (!physical) {
#ifdef DEBUG
				printf("block %llu already unmapped\n",
					logical);
#endif
				જાઓ done;
			}
		} નહીંતર
			જાઓ done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	જો (retval)
		જાઓ done;
	જો (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	જો (retval) {
		has_next = 0;
		જો (retval != EXT2_ET_EXTENT_NO_NEXT)
			જાઓ done;
	} નહીંતર {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		જો (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	જો (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		જાઓ done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	જો (retval) {
		has_prev = 0;
		જો (retval != EXT2_ET_EXTENT_NO_PREV)
			જાઓ done;
	} નહીંતર {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		જો (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	જો (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		જાઓ done;

	/* check if already pointing to the requested physical */
	જો (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		printf("physical block (at %llu) unchanged\n", logical);
#endif
		જાઓ done;
	}

	જો (!mapped) {
#ifdef DEBUG
		printf("mapping unmapped logical block %llu\n", logical);
#endif
		જો ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((પૂર્ણાંક) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} નહીંતર જો ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((પૂર્ણાંક) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} નહીંતર જો (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((પૂર્ણાંક) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			જો (retval)
				જાઓ done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} નહીંતર જો (logical < extent.e_lblk)
			retval = ext2fs_extent_ઉમેરો(handle, 0, &newextent);
		નહીંતર
			retval = ext2fs_extent_ઉમેરો(handle,
				      EXT2_EXTENT_ઉમેરો_AFTER, &newextent);
		જો (retval)
			જાઓ done;
		retval = ext2fs_extent_fix_parents(handle);
		જો (retval)
			જાઓ done;
	} નહીંતર જો ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		printf("(re/un)mapping only block in extent\n");
#endif
		જો (physical) {
			જો (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((પૂર્ણાંક) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				જો (retval)
					જાઓ done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				જો (retval)
					જાઓ done;
				જાઓ delete_ગાંઠ;

			} નહીંતર
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} નહીંતર {
		delete_ગાંઠ:
			retval = ext2fs_extent_delete(handle, 0);
			જો (retval)
				જાઓ done;
			ec = ext2fs_extent_fix_parents(handle);
			જો (ec != EXT2_ET_NO_CURRENT_ગાંઠ)
				retval = ec;
		}

		જો (retval)
			જાઓ done;
	} નહીંતર જો (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		printf("(re/un)mapping last block in extent\n");
#endif
		જો (physical) {
			જો (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((પૂર્ણાંક) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				જો (retval)
					જાઓ done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				જો (retval)
					જાઓ done;
			} નહીંતર
				retval = ext2fs_extent_ઉમેરો(handle,
				      EXT2_EXTENT_ઉમેરો_AFTER, &newextent);
			જો (retval)
				જાઓ done;
			retval = ext2fs_extent_fix_parents(handle);
			જો (retval)
				જાઓ done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			જો (retval)
				જાઓ done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			જો (retval)
				જાઓ done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		જો (retval)
			જાઓ done;
	} નહીંતર જો (logical == extent.e_lblk) {
#ifdef DEBUG
		printf("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		જો (retval)
			જાઓ done;
		retval = ext2fs_extent_fix_parents(handle);
		જો (retval)
			જાઓ done;
		જો (physical) {
			જો (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((પૂર્ણાંક) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				જો (retval)
					જાઓ done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} નહીંતર
				retval = ext2fs_extent_ઉમેરો(handle,
							      0, &newextent);
			જો (retval)
				જાઓ done;
			retval = ext2fs_extent_fix_parents(handle);
			જો (retval)
				જાઓ done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			જો (retval)
				જાઓ done;
		}
	} નહીંતર {
		__u32	save_length;
		blk64_t	save_lblk;
		સંરચના ext2fs_extent save_extent;
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
		જો (retval)
			જાઓ done;
		/* insert our new extent, if any */
		જો (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_ઉમેરો(handle,
					EXT2_EXTENT_ઉમેરો_AFTER, &newextent);
			જો (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				જો (r2 == 0)
					(શૂન્ય)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				જાઓ done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_length - extent.e_len - 1;
		retval = ext2fs_extent_ઉમેરો(handle,
				EXT2_EXTENT_ઉમેરો_AFTER, &extent);
		જો (retval) {
			જો (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				જો (r2 == 0)
					(શૂન્ય)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			જો (r2 == 0)
				(શૂન્ય)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			જાઓ done;
		}
	}

done:
	/* get handle back to its position */
	જો (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	પાછા retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, પૂર્ણાંક flags)
{
	સંરચના extent_path		*path;
	અક્ષર 				*cp;
	સંરચના ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	જો (!(handle->fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

	જો (!handle->path)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

#ifdef DEBUG
	{
		સંરચના ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		જો (retval == 0) {
			printf("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	જો (!path->curr)
		પાછા EXT2_ET_NO_CURRENT_ગાંઠ;

	cp = path->curr;

	/* Sanity check before memmove() */
	જો (path->ડાબું < 0)
		પાછા EXT2_ET_EXTENT_LEAF_BAD;

	જો (path->ડાબું) {
		memmove(cp, cp + કદ(સંરચના ext3_extent_idx),
			path->ડાબું * કદ(સંરચના ext3_extent_idx));
		path->ડાબું--;
	} નહીંતર {
		સંરચના ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	જો (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	જો (path->entries == 0 && handle->level) {
		જો (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			સંરચના ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			જો (retval)
				પાછા retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} નહીંતર {
		eh = (સંરચના ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		જો ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	પાછા retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 સંરચના ext2_extent_info *info)
{
	સંરચના extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, કદ(સંરચના ext2_extent_info));

	path = handle->path + handle->level;
	જો (path) {
		જો (path->curr)
			info->curr_entry = ((અક્ષર *) path->curr - path->buf) /
				કદ(સંરચના ext3_extent_idx);
		નહીંતર
			info->curr_entry = 0;
		info->અંક_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			કદ(સંરચના ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	પાછા 0;
}

size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	size_t iblock_sz = કદ(((સંરચના ext2_inode *)ખાલી)->i_block);
	size_t iblock_extents = (iblock_sz - કદ(સંરચના ext3_extent_header)) /
				કદ(સંરચના ext3_extent);
	size_t extents_per_block = (handle->fs->blocksize -
				    કદ(સંરચના ext3_extent_header)) /
				   કદ(સંરચના ext3_extent);
	સ્થિર unsigned પૂર્ણાંક last_blocksize = 0;
	સ્થિર size_t last_result = 0;

	જો (last_blocksize && last_blocksize == handle->fs->blocksize)
		પાછા last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	પાછા last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       સંરચના ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	સંરચના ext2fs_extent	extent;
	errcode_t		errcode;
	પૂર્ણાંક    			save_flags = fs->flags;

	જો (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		પાછા 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	જો (errcode) {
		જો (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		પાછા errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_મૂળ, &extent);
	જો (errcode)
		જાઓ out;

	કરો {
		/* Skip to the end of a block of leaf nodes */
		જો (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			જો (errcode)
				તોડો;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		જો (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} જ્યારે (errcode == 0);

out:
	/* Ok if we run off the end */
	જો (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	પાછા errcode;
}

errcode_t ext2fs_decode_extent(સંરચના ext2fs_extent *to, શૂન્ય *addr, પૂર્ણાંક len)
{
	સંરચના ext3_extent *from = (સંરચના ext3_extent *)addr;

	જો (len != કદ(સંરચના ext3_extent))
		પાછા EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	જો (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	પાછા 0;
}

errcode_t ext2fs_ગણતરી_blocks(ext2_filsys fs, ext2_ino_t ino,
			      સંરચના ext2_inode *inode, blk64_t *ret_ગણતરી)
{
	ext2_extent_handle_t	handle = ખાલી;
	સંરચના ext2fs_extent	extent;
	errcode_t		errcode;
	પૂર્ણાંક			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	જો (errcode)
		જાઓ out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_મૂળ, &extent);
	જો (errcode)
		જાઓ out;

	errcode = ext2fs_get_array(handle->max_depth, કદ(blk64_t),
				   &intermediate_nodes);
	જો (errcode)
		જાઓ out;

	blkcount = handle->level;
	જ્યારે (!errcode) {
		જો (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			માટે (i = 0; i < handle->level; i++) {
				જો (intermediate_nodes[i] !=
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
	*ret_ગણતરી = blkcount;
	ext2fs_extent_free(handle);

	પાછા 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
અચલ અક્ષર *debug_prog_name = "tst_extents";

#endif

