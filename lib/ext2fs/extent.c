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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"
#അന്തര്ഭാവ "e2image.h"

#undef DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

ഘടന extent_path {
	അക്ഷരം		*buf;
	പൂർണ്ണസംഖ്യ		entries;
	പൂർണ്ണസംഖ്യ		max_entries;
	പൂർണ്ണസംഖ്യ		ഇടത്;
	പൂർണ്ണസംഖ്യ		visit_അക്കം;
	പൂർണ്ണസംഖ്യ		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	ശൂന്യം		*curr;
};


ഘടന ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	ഘടന ext2_inode	*inode;
	ഘടന ext2_inode	inodebuf;
	പൂർണ്ണസംഖ്യ			type;
	പൂർണ്ണസംഖ്യ			level;
	പൂർണ്ണസംഖ്യ			max_depth;
	പൂർണ്ണസംഖ്യ			max_paths;
	ഘടന extent_path	*path;
};

ഘടന ext2_extent_path {
	errcode_t		magic;
	പൂർണ്ണസംഖ്യ			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
സ്ഥിതം ശൂന്യം dbg_show_header(ഘടന ext3_extent_header *eh)
{
	printf("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

സ്ഥിതം ശൂന്യം dbg_show_index(ഘടന ext3_extent_idx *ix)
{
	printf("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

സ്ഥിതം ശൂന്യം dbg_show_extent(ഘടന ext3_extent *ex)
{
	printf("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

സ്ഥിതം ശൂന്യം dbg_print_extent(അക്ഷരം *desc, ഘടന ext2fs_extent *extent)
{
	എങ്കിൽ (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	എങ്കിൽ (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	എങ്കിൽ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	എങ്കിൽ (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	എങ്കിൽ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

സ്ഥിതം ശൂന്യം dump_path(സ്ഥിരം അക്ഷരം *tag, ഘടന ext2_extent_handle *handle,
		      ഘടന extent_path *path)
{
	ഘടന extent_path *ppp = path;
	printf("%s: level=%d\n", tag, handle->level);

	ചെയ്യുക {
		printf("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->ഇടത്, ppp->visit_അക്കം, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (ശൂന്യം *)ppp->buf);
		printf("  ");
		dbg_show_header((ഘടന ext3_extent_header *)ppp->buf);
		എങ്കിൽ (ppp->curr) {
			printf("  ");
			dbg_show_index(ppp->curr);
			printf("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} ആകുമ്പോൾ (ppp >= handle->path);
	fflush(stdout);

	തിരികെ;
}

#അല്ലെങ്കിൽ
#define dbg_show_header(eh) ചെയ്യുക { } ആകുമ്പോൾ (0)
#define dbg_show_index(ix) ചെയ്യുക { } ആകുമ്പോൾ (0)
#define dbg_show_extent(ex) ചെയ്യുക { } ആകുമ്പോൾ (0)
#define dbg_print_extent(desc, ex) ചെയ്യുക { } ആകുമ്പോൾ (0)
#define dump_path(tag, handle, path) ചെയ്യുക { } ആകുമ്പോൾ (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(ശൂന്യം *ptr, പൂർണ്ണസംഖ്യ size)
{
	പൂർണ്ണസംഖ്യ eh_max, entry_size;
	ഘടന ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	എങ്കിൽ (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		തിരികെ EXT2_ET_EXTENT_HEADER_BAD;
	എങ്കിൽ (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		തിരികെ EXT2_ET_EXTENT_HEADER_BAD;
	എങ്കിൽ (eh->eh_depth == 0)
		entry_size = വലുപ്പം(ഘടന ext3_extent);
	അല്ലെങ്കിൽ
		entry_size = വലുപ്പം(ഘടന ext3_extent_idx);

	eh_max = (size - വലുപ്പം(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	എങ്കിൽ ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		തിരികെ EXT2_ET_EXTENT_HEADER_BAD;

	തിരികെ 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
ശൂന്യം ext2fs_extent_free(ext2_extent_handle_t handle)
{
	പൂർണ്ണസംഖ്യ			i;

	എങ്കിൽ (!handle)
		തിരികെ;

	എങ്കിൽ (handle->path) {
		വേണ്ടി (i = 1; i < handle->max_paths; i++) {
			എങ്കിൽ (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	തിരികെ ext2fs_extent_open2(fs, ino, ശൂന്യ, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    ഘടന ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	ഘടന ext2_extent_handle	*handle;
	errcode_t			retval;
	പൂർണ്ണസംഖ്യ				i;
	ഘടന ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (!inode)
		എങ്കിൽ ((ino == 0) || (ino > fs->super->s_inodes_എണ്ണം))
			തിരികെ EXT2_ET_BAD_INODE_അക്കം;

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_extent_handle), &handle);
	എങ്കിൽ (retval)
		തിരികെ retval;
	memset(handle, 0, വലുപ്പം(ഘടന ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	എങ്കിൽ (inode) {
		handle->inode = inode;
	} അല്ലെങ്കിൽ {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		എങ്കിൽ (retval)
			പോകുക errout;
	}

	eh = (ഘടന ext3_extent_header *) &handle->inode->i_block[0];

	വേണ്ടി (i=0; i < EXT2_N_BLOCKS; i++)
		എങ്കിൽ (handle->inode->i_block[i])
			നിർത്തുക;
	എങ്കിൽ (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (വലുപ്പം(handle->inode->i_block) - വലുപ്പം(*eh)) /
			വലുപ്പം(ഘടന ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	എങ്കിൽ (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		പോകുക errout;
	}

	retval = ext2fs_extent_header_verify(eh, വലുപ്പം(handle->inode->i_block));
	എങ്കിൽ (retval)
		പോകുക errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    വലുപ്പം(ഘടന extent_path),
				    &handle->path);
	handle->path[0].buf = (അക്ഷരം *) handle->inode->i_block;

	handle->path[0].ഇടത് = handle->path[0].entries =
		ext2fs_le16_to_cpu(eh->eh_entries);
	handle->path[0].max_entries = ext2fs_le16_to_cpu(eh->eh_max);
	handle->path[0].curr = 0;
	handle->path[0].end_blk =
		(EXT2_I_SIZE(handle->inode) + fs->blocksize - 1) >>
		 EXT2_BLOCK_SIZE_BITS(fs->super);
	handle->path[0].blk = 0;
	handle->path[0].visit_അക്കം = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	തിരികെ 0;

errout:
	ext2fs_extent_free(handle);
	തിരികെ retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    പൂർണ്ണസംഖ്യ flags, ഘടന ext2fs_extent *extent)
{
	ഘടന extent_path	*path, *newpath, *tp;
	ഘടന ext3_extent_header	*eh;
	ഘടന ext3_extent_idx		*ix = 0;
	ഘടന ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	പൂർണ്ണസംഖ്യ				orig_op, op, l;
	പൂർണ്ണസംഖ്യ				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	എങ്കിൽ (!handle->path)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	എങ്കിൽ ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		എങ്കിൽ (handle->level < handle->max_depth) {
			/* interior node */
			എങ്കിൽ (path->visit_അക്കം == 0) {
				path->visit_അക്കം++;
				op = EXT2_EXTENT_DOWN;
			} അല്ലെങ്കിൽ എങ്കിൽ (path->ഇടത് > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			അല്ലെങ്കിൽ എങ്കിൽ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			അല്ലെങ്കിൽ
				തിരികെ EXT2_ET_EXTENT_NO_NEXT;
		} അല്ലെങ്കിൽ {
			/* leaf node */
			എങ്കിൽ (path->ഇടത് > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			അല്ലെങ്കിൽ എങ്കിൽ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			അല്ലെങ്കിൽ
				തിരികെ EXT2_ET_EXTENT_NO_NEXT;
		}
		എങ്കിൽ (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	എങ്കിൽ ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		എങ്കിൽ (handle->level < handle->max_depth) {
			/* interior node */
			എങ്കിൽ (path->visit_അക്കം > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} അല്ലെങ്കിൽ എങ്കിൽ (path->ഇടത് < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			അല്ലെങ്കിൽ എങ്കിൽ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			അല്ലെങ്കിൽ
				തിരികെ EXT2_ET_EXTENT_NO_PREV;
		} അല്ലെങ്കിൽ {
			/* leaf node */
			എങ്കിൽ (path->ഇടത് < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			അല്ലെങ്കിൽ എങ്കിൽ (handle->level > 0)
				op = EXT2_EXTENT_UP;
			അല്ലെങ്കിൽ
				തിരികെ EXT2_ET_EXTENT_NO_PREV;
		}
		എങ്കിൽ (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	എങ്കിൽ (orig_op == EXT2_EXTENT_LAST_LEAF) {
		എങ്കിൽ ((handle->level < handle->max_depth) &&
		    (path->ഇടത് == 0))
			op = EXT2_EXTENT_DOWN;
		അല്ലെങ്കിൽ
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		printf("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	തിരഞ്ഞെടുപ്പ് (op) {
	അവസ്ഥ EXT2_EXTENT_CURRENT:
		ix = path->curr;
		നിർത്തുക;
	അവസ്ഥ EXT2_EXTENT_വേര്:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	അവസ്ഥ EXT2_EXTENT_FIRST_SIB:
		path->ഇടത് = path->entries;
		path->curr = 0;
		/* fallthrough */
	അവസ്ഥ EXT2_EXTENT_NEXT_SIB:
		എങ്കിൽ (path->ഇടത് <= 0)
			തിരികെ EXT2_ET_EXTENT_NO_NEXT;
		എങ്കിൽ (path->curr) {
			ix = path->curr;
			ix++;
		} അല്ലെങ്കിൽ {
			eh = (ഘടന ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->ഇടത്--;
		path->curr = ix;
		path->visit_അക്കം = 0;
		നിർത്തുക;
	അവസ്ഥ EXT2_EXTENT_PREV_SIB:
		എങ്കിൽ (!path->curr ||
		    path->ഇടത്+1 >= path->entries)
			തിരികെ EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->ഇടത്++;
		എങ്കിൽ (handle->level < handle->max_depth)
			path->visit_അക്കം = 1;
		നിർത്തുക;
	അവസ്ഥ EXT2_EXTENT_LAST_SIB:
		eh = (ഘടന ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->ഇടത് = 0;
		path->visit_അക്കം = 0;
		നിർത്തുക;
	അവസ്ഥ EXT2_EXTENT_UP:
		എങ്കിൽ (handle->level <= 0)
			തിരികെ EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		എങ്കിൽ ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_അക്കം = 0;
		നിർത്തുക;
	അവസ്ഥ EXT2_EXTENT_DOWN:
	അവസ്ഥ EXT2_EXTENT_DOWN_AND_LAST:
		എങ്കിൽ (!path->curr ||(handle->level >= handle->max_depth))
			തിരികെ EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		എങ്കിൽ (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			എങ്കിൽ (retval)
				തിരികെ retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		വേണ്ടി (l = handle->level, tp = path; l > 0; l--, tp--) {
			എങ്കിൽ (blk == tp->blk)
				തിരികെ EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		എങ്കിൽ ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		അല്ലെങ്കിൽ {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			എങ്കിൽ (retval)
				തിരികെ retval;
		}
		handle->level++;

		eh = (ഘടന ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		എങ്കിൽ (retval) {
			handle->level--;
			തിരികെ retval;
		}

		എങ്കിൽ (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->ഇടത് = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		എങ്കിൽ (newpath->ഇടത് <= 0)
			തിരികെ EXT2_ET_EXTENT_NO_DOWN;

		എങ്കിൽ (path->ഇടത് > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} അല്ലെങ്കിൽ
			newpath->end_blk = path->end_blk;

		path = newpath;
		എങ്കിൽ (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((ഘടന ext3_extent_header *) eh);
			path->curr = ix;
			path->ഇടത് = path->entries - 1;
			path->visit_അക്കം = 0;
		} അല്ലെങ്കിൽ {
			ix = EXT_LAST_INDEX((ഘടന ext3_extent_header *) eh);
			path->curr = ix;
			path->ഇടത് = 0;
			എങ്കിൽ (handle->level < handle->max_depth)
				path->visit_അക്കം = 1;
		}
#ifdef DEBUG_GET_EXTENT
		printf("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		തിരികെ EXT2_ET_OP_NOT_SUPPORTED;
	}

	എങ്കിൽ (!ix)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	printf("(Left %d)\n", path->ഇടത്);
#endif

	എങ്കിൽ (handle->level == handle->max_depth) {
		ex = (ഘടന ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		എങ്കിൽ (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} അല്ലെങ്കിൽ {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		എങ്കിൽ (path->ഇടത് > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} അല്ലെങ്കിൽ
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	എങ്കിൽ (path->visit_അക്കം)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	എങ്കിൽ (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		പോകുക retry;

	എങ്കിൽ ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->ഇടത് != 0)))
		പോകുക retry;

	എങ്കിൽ (failed_csum)
		തിരികെ EXT2_ET_EXTENT_CSUM_INVALID;

	തിരികെ 0;
}

സ്ഥിതം errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	ഘടന ext3_extent_idx		*ix;
	ഘടന ext3_extent_header	*eh;

	എങ്കിൽ (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} അല്ലെങ്കിൽ {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (ഘടന ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		എങ്കിൽ (retval)
			തിരികെ retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	തിരികെ retval;
}

#എങ്കിൽ 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	ഘടന ext2fs_extent	extent;
	ഘടന ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = ext2fs_extent_get_info(handle, &info);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_extent_path), &save_path);
	എങ്കിൽ (retval)
		തിരികെ retval;
	memset(save_path, 0, വലുപ്പം(ഘടന ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	തിരികെ 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	തിരികെ 0;
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
			      പൂർണ്ണസംഖ്യ leaf_level, blk64_t blk)
{
	ഘടന ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_വേര്, &extent);
	എങ്കിൽ (retval) {
		എങ്കിൽ (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		തിരികെ retval;
	}

	എങ്കിൽ (leaf_level > handle->max_depth) {
#ifdef DEBUG
		printf("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		തിരികെ EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	printf("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	ആകുമ്പോൾ (1) {
		എങ്കിൽ (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			എങ്കിൽ ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				തിരികെ 0;
			എങ്കിൽ (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				തിരികെ EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			എങ്കിൽ (retval == EXT2_ET_EXTENT_NO_NEXT)
				തിരികെ EXT2_ET_EXTENT_NOT_FOUND;
			എങ്കിൽ (retval)
				തിരികെ retval;
			തുടരുക;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		എങ്കിൽ (retval == EXT2_ET_EXTENT_NO_NEXT)
			പോകുക go_down;
		എങ്കിൽ (retval)
			തിരികെ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		എങ്കിൽ (blk == extent.e_lblk)
			പോകുക go_down;
		എങ്കിൽ (blk > extent.e_lblk)
			തുടരുക;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		എങ്കിൽ (retval)
			തിരികെ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		എങ്കിൽ (retval)
			തിരികെ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	തിരികെ ext2fs_extent_goto2(handle, 0, blk);
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
	പൂർണ്ണസംഖ്യ				retval = 0;
	പൂർണ്ണസംഖ്യ				orig_height;
	blk64_t				start;
	ഘടന extent_path		*path;
	ഘടന ext2fs_extent		extent;
	ഘടന ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	എങ്കിൽ (!(handle->fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

	എങ്കിൽ (!handle->path)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

	path = handle->path + handle->level;
	എങ്കിൽ (!path->curr)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	എങ്കിൽ (retval)
		പോകുക done;

	/* modified node's start block */
	start = extent.e_lblk;

	എങ്കിൽ ((retval = ext2fs_extent_get_info(handle, &info)))
		തിരികെ retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	ആകുമ്പോൾ (handle->level > 0 &&
	       (path->ഇടത് == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		എങ്കിൽ (retval)
			പോകുക done;
		എങ്കിൽ (extent.e_lblk == start)
			നിർത്തുക;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		എങ്കിൽ (retval)
			പോകുക done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	തിരികെ retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)),
				ഘടന ext2fs_extent *extent)
{
	ഘടന extent_path		*path;
	ഘടന ext3_extent_idx		*ix;
	ഘടന ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	എങ്കിൽ (!(handle->fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

	എങ്കിൽ (!handle->path)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

	path = handle->path + handle->level;
	എങ്കിൽ (!path->curr)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

#ifdef DEBUG
	printf("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	എങ്കിൽ (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		എങ്കിൽ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			എങ്കിൽ (extent->e_len > EXT_UNINIT_MAX_LEN)
				തിരികെ EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (extent->e_len > EXT_INIT_MAX_LEN)
				തിരികെ EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} അല്ലെങ്കിൽ {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ splitting_at_eof(ഘടന ext2_extent_handle *handle,
			    ഘടന extent_path *path)
{
	ഘടന extent_path *ppp = path;
	dump_path(__func__, handle, path);

	എങ്കിൽ (handle->level == 0)
		തിരികെ 0;

	ചെയ്യുക {
		എങ്കിൽ (ppp->ഇടത്)
			തിരികെ 0;
		ppp--;
	} ആകുമ്പോൾ (ppp >= handle->path);

	തിരികെ 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
സ്ഥിതം errcode_t extent_കണ്ണി_split(ext2_extent_handle_t handle,
				   പൂർണ്ണസംഖ്യ expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_കണ്ണി_pblk;
	blk64_t				new_കണ്ണി_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	പൂർണ്ണസംഖ്യ				orig_height;
	അക്ഷരം				*block_buf = ശൂന്യ;
	ഘടന ext2fs_extent		extent;
	ഘടന extent_path		*path, *newpath = 0;
	ഘടന ext3_extent_header	*eh, *neweh;
	പൂർണ്ണസംഖ്യ				tocopy;
	പൂർണ്ണസംഖ്യ				new_വേര് = 0;
	ഘടന ext2_extent_info		info;
	പൂർണ്ണസംഖ്യ				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	എങ്കിൽ (!(handle->fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

	എങ്കിൽ (!handle->path)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

#ifdef DEBUG
	printf("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	എങ്കിൽ (retval)
		പോകുക done;

	retval = ext2fs_extent_get_info(handle, &info);
	എങ്കിൽ (retval)
		പോകുക done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (ഘടന ext3_extent_header *) path->buf;
	എങ്കിൽ (handle->level == handle->max_depth) {
		ഘടന ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} അല്ലെങ്കിൽ {
		ഘടന ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	എങ്കിൽ (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		printf("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		എങ്കിൽ (retval)
			പോകുക done;

		retval = extent_കണ്ണി_split(handle, expand_allowed);
		എങ്കിൽ (retval)
			പോകുക done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		എങ്കിൽ (retval)
			പോകുക done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	എങ്കിൽ (!path->curr)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (ഘടന ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	എങ്കിൽ (handle->level == 0) {
		new_വേര് = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    വലുപ്പം(ഘടന extent_path),
					    &newpath);
		എങ്കിൽ (retval)
			പോകുക done;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (no_balance)
			tocopy = 1;
		അല്ലെങ്കിൽ
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	printf("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	എങ്കിൽ (!tocopy && !no_balance) {
#ifdef DEBUG
		printf("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		പോകുക done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = malloc(handle->fs->blocksize);
	എങ്കിൽ (!block_buf) {
		retval = ENOMEM;
		പോകുക done;
	}

	എങ്കിൽ (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_കണ്ണി_pblk);
	എങ്കിൽ (retval)
		പോകുക done;

#ifdef DEBUG
	printf("will copy to new node at block %lu\n",
	       (unsigned long) new_കണ്ണി_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (ഘടന ext3_extent_header *) block_buf;
	memcpy(neweh, eh, വലുപ്പം(ഘടന ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 വലുപ്പം(ഘടന ext3_extent_header)) /
				വലുപ്പം(ഘടന ext3_extent));

	/* then the entries for the new block... */
	memcpy(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		വലുപ്പം(ഘടന ext3_extent_idx) * tocopy);

	new_കണ്ണി_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	എങ്കിൽ (retval)
		പോകുക done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_കണ്ണി_pblk, 1,
					block_buf);

	എങ്കിൽ (retval)
		പോകുക done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	എങ്കിൽ (handle->level == 0) {
		memcpy(newpath, path,
		       വലുപ്പം(ഘടന extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->ഇടത് = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} അല്ലെങ്കിൽ {
		path->entries -= tocopy;
		path->ഇടത് -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	എങ്കിൽ (retval)
		പോകുക done;

	/* now go up and insert/replace index for new node we created */
	എങ്കിൽ (new_വേര്) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		എങ്കിൽ (retval)
			പോകുക done;

		extent.e_lblk = new_കണ്ണി_start;
		extent.e_pblk = new_കണ്ണി_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		എങ്കിൽ (retval)
			പോകുക done;
	} അല്ലെങ്കിൽ {
		__u32 new_കണ്ണി_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_കണ്ണി_length = new_കണ്ണി_start - extent.e_lblk;
		extent.e_len -= new_കണ്ണി_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		എങ്കിൽ (retval)
			പോകുക done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_കണ്ണി_start;
		extent.e_pblk = new_കണ്ണി_pblk;
		extent.e_len = new_കണ്ണി_length;
		retval = ext2fs_extent_തിരുകുക(handle, EXT2_EXTENT_തിരുകുക_AFTER, &extent);
		എങ്കിൽ (retval)
			പോകുക done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	എങ്കിൽ (retval)
		പോകുക done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	എങ്കിൽ (retval)
		പോകുക done;

done:
	എങ്കിൽ (newpath)
		ext2fs_free_mem(&newpath);
	സ്വതന്ത്രം(block_buf);

	തിരികെ retval;
}

errcode_t ext2fs_extent_കണ്ണി_split(ext2_extent_handle_t handle)
{
	തിരികെ extent_കണ്ണി_split(handle, 0);
}

errcode_t ext2fs_extent_തിരുകുക(ext2_extent_handle_t handle, പൂർണ്ണസംഖ്യ flags,
				      ഘടന ext2fs_extent *extent)
{
	ഘടന extent_path		*path;
	ഘടന ext3_extent_idx		*ix;
	ഘടന ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	എങ്കിൽ (!(handle->fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

	എങ്കിൽ (!handle->path)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

#ifdef DEBUG
	printf("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	എങ്കിൽ (path->entries >= path->max_entries) {
		എങ്കിൽ (flags & EXT2_EXTENT_തിരുകുക_NOSPLIT) {
			തിരികെ EXT2_ET_CANT_തിരുകുക_EXTENT;
		} അല്ലെങ്കിൽ {
#ifdef DEBUG
			printf("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_കണ്ണി_split(handle, 1);
			എങ്കിൽ (retval)
				തിരികെ retval;
			path = handle->path + handle->level;
		}
	}

	eh = (ഘടന ext3_extent_header *) path->buf;
	എങ്കിൽ (path->curr) {
		ix = path->curr;
		എങ്കിൽ (flags & EXT2_EXTENT_തിരുകുക_AFTER) {
			ix++;
			path->ഇടത്--;
		}
	} അല്ലെങ്കിൽ {
		ix = EXT_FIRST_INDEX(eh);
		path->ഇടത് = -1;
	}

	path->curr = ix;

	എങ്കിൽ (path->ഇടത് >= 0)
		memmove(ix + 1, ix,
			(path->ഇടത്+1) * വലുപ്പം(ഘടന ext3_extent_idx));
	path->ഇടത്++;
	path->entries++;

	eh = (ഘടന ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	എങ്കിൽ (retval)
		പോകുക errout;

	retval = update_path(handle);
	എങ്കിൽ (retval)
		പോകുക errout;

	തിരികെ 0;

errout:
	ext2fs_extent_delete(handle, 0);
	തിരികെ retval;
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
				 blk64_t logical, blk64_t physical, പൂർണ്ണസംഖ്യ flags)
{
	errcode_t		ec, retval = 0;
	പൂർണ്ണസംഖ്യ			mapped = 1; /* logical is mapped? */
	പൂർണ്ണസംഖ്യ			orig_height;
	പൂർണ്ണസംഖ്യ			extent_uninit = 0;
	പൂർണ്ണസംഖ്യ			prev_uninit = 0;
	പൂർണ്ണസംഖ്യ			next_uninit = 0;
	പൂർണ്ണസംഖ്യ			new_uninit = 0;
	പൂർണ്ണസംഖ്യ			max_len = EXT_INIT_MAX_LEN;
	പൂർണ്ണസംഖ്യ			has_prev, has_next;
	blk64_t			orig_lblk;
	ഘടന extent_path	*path;
	ഘടന ext2fs_extent	extent, next_extent, prev_extent;
	ഘടന ext2fs_extent	newextent;
	ഘടന ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	printf("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	എങ്കിൽ (!(handle->fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

	എങ്കിൽ (!handle->path)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

	path = handle->path + handle->level;

	എങ്കിൽ (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	എങ്കിൽ (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		എങ്കിൽ (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	എങ്കിൽ ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_തിരുകുക(handle, 0, &newextent);
		തിരികെ retval;
	}

	/* save our original location in the extent tree */
	എങ്കിൽ ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		എങ്കിൽ (retval != EXT2_ET_NO_CURRENT_കണ്ണി)
			തിരികെ retval;
		memset(&extent, 0, വലുപ്പം(extent));
	}
	എങ്കിൽ ((retval = ext2fs_extent_get_info(handle, &info)))
		തിരികെ retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	എങ്കിൽ (retval) {
		എങ്കിൽ (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			എങ്കിൽ (!physical) {
#ifdef DEBUG
				printf("block %llu already unmapped\n",
					logical);
#endif
				പോകുക done;
			}
		} അല്ലെങ്കിൽ
			പോകുക done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	എങ്കിൽ (retval)
		പോകുക done;
	എങ്കിൽ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	എങ്കിൽ (retval) {
		has_next = 0;
		എങ്കിൽ (retval != EXT2_ET_EXTENT_NO_NEXT)
			പോകുക done;
	} അല്ലെങ്കിൽ {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		എങ്കിൽ (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	എങ്കിൽ (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		പോകുക done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	എങ്കിൽ (retval) {
		has_prev = 0;
		എങ്കിൽ (retval != EXT2_ET_EXTENT_NO_PREV)
			പോകുക done;
	} അല്ലെങ്കിൽ {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		എങ്കിൽ (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	എങ്കിൽ (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		പോകുക done;

	/* check if already pointing to the requested physical */
	എങ്കിൽ (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		printf("physical block (at %llu) unchanged\n", logical);
#endif
		പോകുക done;
	}

	എങ്കിൽ (!mapped) {
#ifdef DEBUG
		printf("mapping unmapped logical block %llu\n", logical);
#endif
		എങ്കിൽ ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((പൂർണ്ണസംഖ്യ) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} അല്ലെങ്കിൽ എങ്കിൽ ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((പൂർണ്ണസംഖ്യ) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} അല്ലെങ്കിൽ എങ്കിൽ (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((പൂർണ്ണസംഖ്യ) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			എങ്കിൽ (retval)
				പോകുക done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} അല്ലെങ്കിൽ എങ്കിൽ (logical < extent.e_lblk)
			retval = ext2fs_extent_തിരുകുക(handle, 0, &newextent);
		അല്ലെങ്കിൽ
			retval = ext2fs_extent_തിരുകുക(handle,
				      EXT2_EXTENT_തിരുകുക_AFTER, &newextent);
		എങ്കിൽ (retval)
			പോകുക done;
		retval = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (retval)
			പോകുക done;
	} അല്ലെങ്കിൽ എങ്കിൽ ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		printf("(re/un)mapping only block in extent\n");
#endif
		എങ്കിൽ (physical) {
			എങ്കിൽ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((പൂർണ്ണസംഖ്യ) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				എങ്കിൽ (retval)
					പോകുക done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				എങ്കിൽ (retval)
					പോകുക done;
				പോകുക delete_കണ്ണി;

			} അല്ലെങ്കിൽ
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} അല്ലെങ്കിൽ {
		delete_കണ്ണി:
			retval = ext2fs_extent_delete(handle, 0);
			എങ്കിൽ (retval)
				പോകുക done;
			ec = ext2fs_extent_fix_parents(handle);
			എങ്കിൽ (ec != EXT2_ET_NO_CURRENT_കണ്ണി)
				retval = ec;
		}

		എങ്കിൽ (retval)
			പോകുക done;
	} അല്ലെങ്കിൽ എങ്കിൽ (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		printf("(re/un)mapping last block in extent\n");
#endif
		എങ്കിൽ (physical) {
			എങ്കിൽ (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((പൂർണ്ണസംഖ്യ) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				എങ്കിൽ (retval)
					പോകുക done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				എങ്കിൽ (retval)
					പോകുക done;
			} അല്ലെങ്കിൽ
				retval = ext2fs_extent_തിരുകുക(handle,
				      EXT2_EXTENT_തിരുകുക_AFTER, &newextent);
			എങ്കിൽ (retval)
				പോകുക done;
			retval = ext2fs_extent_fix_parents(handle);
			എങ്കിൽ (retval)
				പോകുക done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			എങ്കിൽ (retval)
				പോകുക done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			എങ്കിൽ (retval)
				പോകുക done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		എങ്കിൽ (retval)
			പോകുക done;
	} അല്ലെങ്കിൽ എങ്കിൽ (logical == extent.e_lblk) {
#ifdef DEBUG
		printf("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		എങ്കിൽ (retval)
			പോകുക done;
		retval = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (retval)
			പോകുക done;
		എങ്കിൽ (physical) {
			എങ്കിൽ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((പൂർണ്ണസംഖ്യ) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				എങ്കിൽ (retval)
					പോകുക done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} അല്ലെങ്കിൽ
				retval = ext2fs_extent_തിരുകുക(handle,
							      0, &newextent);
			എങ്കിൽ (retval)
				പോകുക done;
			retval = ext2fs_extent_fix_parents(handle);
			എങ്കിൽ (retval)
				പോകുക done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			എങ്കിൽ (retval)
				പോകുക done;
		}
	} അല്ലെങ്കിൽ {
		__u32	save_length;
		blk64_t	save_lblk;
		ഘടന ext2fs_extent save_extent;
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
		എങ്കിൽ (retval)
			പോകുക done;
		/* insert our new extent, if any */
		എങ്കിൽ (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_തിരുകുക(handle,
					EXT2_EXTENT_തിരുകുക_AFTER, &newextent);
			എങ്കിൽ (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				എങ്കിൽ (r2 == 0)
					(ശൂന്യം)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				പോകുക done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_length - extent.e_len - 1;
		retval = ext2fs_extent_തിരുകുക(handle,
				EXT2_EXTENT_തിരുകുക_AFTER, &extent);
		എങ്കിൽ (retval) {
			എങ്കിൽ (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				എങ്കിൽ (r2 == 0)
					(ശൂന്യം)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			എങ്കിൽ (r2 == 0)
				(ശൂന്യം)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			പോകുക done;
		}
	}

done:
	/* get handle back to its position */
	എങ്കിൽ (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	തിരികെ retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന extent_path		*path;
	അക്ഷരം 				*cp;
	ഘടന ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	എങ്കിൽ (!(handle->fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

	എങ്കിൽ (!handle->path)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

#ifdef DEBUG
	{
		ഘടന ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		എങ്കിൽ (retval == 0) {
			printf("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	എങ്കിൽ (!path->curr)
		തിരികെ EXT2_ET_NO_CURRENT_കണ്ണി;

	cp = path->curr;

	/* Sanity check before memmove() */
	എങ്കിൽ (path->ഇടത് < 0)
		തിരികെ EXT2_ET_EXTENT_LEAF_BAD;

	എങ്കിൽ (path->ഇടത്) {
		memmove(cp, cp + വലുപ്പം(ഘടന ext3_extent_idx),
			path->ഇടത് * വലുപ്പം(ഘടന ext3_extent_idx));
		path->ഇടത്--;
	} അല്ലെങ്കിൽ {
		ഘടന ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	എങ്കിൽ (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	എങ്കിൽ (path->entries == 0 && handle->level) {
		എങ്കിൽ (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			ഘടന ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			എങ്കിൽ (retval)
				തിരികെ retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} അല്ലെങ്കിൽ {
		eh = (ഘടന ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		എങ്കിൽ ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	തിരികെ retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 ഘടന ext2_extent_info *info)
{
	ഘടന extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, വലുപ്പം(ഘടന ext2_extent_info));

	path = handle->path + handle->level;
	എങ്കിൽ (path) {
		എങ്കിൽ (path->curr)
			info->curr_entry = ((അക്ഷരം *) path->curr - path->buf) /
				വലുപ്പം(ഘടന ext3_extent_idx);
		അല്ലെങ്കിൽ
			info->curr_entry = 0;
		info->അക്കം_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			വലുപ്പം(ഘടന ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	തിരികെ 0;
}

size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	size_t iblock_sz = വലുപ്പം(((ഘടന ext2_inode *)ശൂന്യ)->i_block);
	size_t iblock_extents = (iblock_sz - വലുപ്പം(ഘടന ext3_extent_header)) /
				വലുപ്പം(ഘടന ext3_extent);
	size_t extents_per_block = (handle->fs->blocksize -
				    വലുപ്പം(ഘടന ext3_extent_header)) /
				   വലുപ്പം(ഘടന ext3_extent);
	സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ last_blocksize = 0;
	സ്ഥിതം size_t last_result = 0;

	എങ്കിൽ (last_blocksize && last_blocksize == handle->fs->blocksize)
		തിരികെ last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	തിരികെ last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       ഘടന ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	ഘടന ext2fs_extent	extent;
	errcode_t		errcode;
	പൂർണ്ണസംഖ്യ    			save_flags = fs->flags;

	എങ്കിൽ (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		തിരികെ 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	എങ്കിൽ (errcode) {
		എങ്കിൽ (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		തിരികെ errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_വേര്, &extent);
	എങ്കിൽ (errcode)
		പോകുക out;

	ചെയ്യുക {
		/* Skip to the end of a block of leaf nodes */
		എങ്കിൽ (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			എങ്കിൽ (errcode)
				നിർത്തുക;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		എങ്കിൽ (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} ആകുമ്പോൾ (errcode == 0);

out:
	/* Ok if we run off the end */
	എങ്കിൽ (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	തിരികെ errcode;
}

errcode_t ext2fs_decode_extent(ഘടന ext2fs_extent *to, ശൂന്യം *addr, പൂർണ്ണസംഖ്യ len)
{
	ഘടന ext3_extent *from = (ഘടന ext3_extent *)addr;

	എങ്കിൽ (len != വലുപ്പം(ഘടന ext3_extent))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	എങ്കിൽ (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	തിരികെ 0;
}

errcode_t ext2fs_എണ്ണം_blocks(ext2_filsys fs, ext2_ino_t ino,
			      ഘടന ext2_inode *inode, blk64_t *ret_എണ്ണം)
{
	ext2_extent_handle_t	handle = ശൂന്യ;
	ഘടന ext2fs_extent	extent;
	errcode_t		errcode;
	പൂർണ്ണസംഖ്യ			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	എങ്കിൽ (errcode)
		പോകുക out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_വേര്, &extent);
	എങ്കിൽ (errcode)
		പോകുക out;

	errcode = ext2fs_get_array(handle->max_depth, വലുപ്പം(blk64_t),
				   &intermediate_nodes);
	എങ്കിൽ (errcode)
		പോകുക out;

	blkcount = handle->level;
	ആകുമ്പോൾ (!errcode) {
		എങ്കിൽ (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			വേണ്ടി (i = 0; i < handle->level; i++) {
				എങ്കിൽ (intermediate_nodes[i] !=
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
	*ret_എണ്ണം = blkcount;
	ext2fs_extent_free(handle);

	തിരികെ 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
സ്ഥിരം അക്ഷരം *debug_prog_name = "tst_extents";

#endif

