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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#सचे HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#सचे HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "e2image.h"

#undef DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

संरचना extent_path {
	वर्ण		*buf;
	पूर्णाङ्क		entries;
	पूर्णाङ्क		max_entries;
	पूर्णाङ्क		वामः;
	पूर्णाङ्क		visit_अङ्कः;
	पूर्णाङ्क		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	शून्य		*curr;
};


संरचना ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	संरचना ext2_inode	*inode;
	संरचना ext2_inode	inodebuf;
	पूर्णाङ्क			type;
	पूर्णाङ्क			level;
	पूर्णाङ्क			max_depth;
	पूर्णाङ्क			max_paths;
	संरचना extent_path	*path;
};

संरचना ext2_extent_path {
	errcode_t		magic;
	पूर्णाङ्क			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
स्थिर शून्य dbg_show_header(संरचना ext3_extent_header *eh)
{
	printf("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

स्थिर शून्य dbg_show_index(संरचना ext3_extent_idx *ix)
{
	printf("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

स्थिर शून्य dbg_show_extent(संरचना ext3_extent *ex)
{
	printf("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

स्थिर शून्य dbg_print_extent(वर्ण *desc, संरचना ext2fs_extent *extent)
{
	सचे (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	सचे (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	सचे (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	सचे (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	सचे (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

स्थिर शून्य dump_path(ध्रुव वर्ण *tag, संरचना ext2_extent_handle *handle,
		      संरचना extent_path *path)
{
	संरचना extent_path *ppp = path;
	printf("%s: level=%d\n", tag, handle->level);

	करोहि {
		printf("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->वामः, ppp->visit_अङ्कः, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (शून्य *)ppp->buf);
		printf("  ");
		dbg_show_header((संरचना ext3_extent_header *)ppp->buf);
		सचे (ppp->curr) {
			printf("  ");
			dbg_show_index(ppp->curr);
			printf("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} याव (ppp >= handle->path);
	fflush(stdout);

	पटिनिवत्त;
}

#अञ्ञथा
#define dbg_show_header(eh) करोहि { } याव (0)
#define dbg_show_index(ix) करोहि { } याव (0)
#define dbg_show_extent(ex) करोहि { } याव (0)
#define dbg_print_extent(desc, ex) करोहि { } याव (0)
#define dump_path(tag, handle, path) करोहि { } याव (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(शून्य *ptr, पूर्णाङ्क size)
{
	पूर्णाङ्क eh_max, entry_size;
	संरचना ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	सचे (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		पटिनिवत्त EXT2_ET_EXTENT_HEADER_BAD;
	सचे (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		पटिनिवत्त EXT2_ET_EXTENT_HEADER_BAD;
	सचे (eh->eh_depth == 0)
		entry_size = परिमाण(संरचना ext3_extent);
	अञ्ञथा
		entry_size = परिमाण(संरचना ext3_extent_idx);

	eh_max = (size - परिमाण(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	सचे ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		पटिनिवत्त EXT2_ET_EXTENT_HEADER_BAD;

	पटिनिवत्त 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
शून्य ext2fs_extent_free(ext2_extent_handle_t handle)
{
	पूर्णाङ्क			i;

	सचे (!handle)
		पटिनिवत्त;

	सचे (handle->path) {
		अत्थाय (i = 1; i < handle->max_paths; i++) {
			सचे (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	पटिनिवत्त ext2fs_extent_open2(fs, ino, रिक्त, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    संरचना ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	संरचना ext2_extent_handle	*handle;
	errcode_t			retval;
	पूर्णाङ्क				i;
	संरचना ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	सचे (!inode)
		सचे ((ino == 0) || (ino > fs->super->s_inodes_गणनम्))
			पटिनिवत्त EXT2_ET_BAD_INODE_अङ्कः;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_extent_handle), &handle);
	सचे (retval)
		पटिनिवत्त retval;
	memset(handle, 0, परिमाण(संरचना ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	सचे (inode) {
		handle->inode = inode;
	} अञ्ञथा {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		सचे (retval)
			गच्छ errout;
	}

	eh = (संरचना ext3_extent_header *) &handle->inode->i_block[0];

	अत्थाय (i=0; i < EXT2_N_BLOCKS; i++)
		सचे (handle->inode->i_block[i])
			भङ्ग;
	सचे (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (परिमाण(handle->inode->i_block) - परिमाण(*eh)) /
			परिमाण(संरचना ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	सचे (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		गच्छ errout;
	}

	retval = ext2fs_extent_header_verify(eh, परिमाण(handle->inode->i_block));
	सचे (retval)
		गच्छ errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    परिमाण(संरचना extent_path),
				    &handle->path);
	handle->path[0].buf = (वर्ण *) handle->inode->i_block;

	handle->path[0].वामः = handle->path[0].entries =
		ext2fs_le16_to_cpu(eh->eh_entries);
	handle->path[0].max_entries = ext2fs_le16_to_cpu(eh->eh_max);
	handle->path[0].curr = 0;
	handle->path[0].end_blk =
		(EXT2_I_SIZE(handle->inode) + fs->blocksize - 1) >>
		 EXT2_BLOCK_SIZE_BITS(fs->super);
	handle->path[0].blk = 0;
	handle->path[0].visit_अङ्कः = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	पटिनिवत्त 0;

errout:
	ext2fs_extent_free(handle);
	पटिनिवत्त retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    पूर्णाङ्क flags, संरचना ext2fs_extent *extent)
{
	संरचना extent_path	*path, *newpath, *tp;
	संरचना ext3_extent_header	*eh;
	संरचना ext3_extent_idx		*ix = 0;
	संरचना ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	पूर्णाङ्क				orig_op, op, l;
	पूर्णाङ्क				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	सचे (!handle->path)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	सचे ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		सचे (handle->level < handle->max_depth) {
			/* interior node */
			सचे (path->visit_अङ्कः == 0) {
				path->visit_अङ्कः++;
				op = EXT2_EXTENT_DOWN;
			} अञ्ञथा सचे (path->वामः > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			अञ्ञथा सचे (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अञ्ञथा
				पटिनिवत्त EXT2_ET_EXTENT_NO_NEXT;
		} अञ्ञथा {
			/* leaf node */
			सचे (path->वामः > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			अञ्ञथा सचे (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अञ्ञथा
				पटिनिवत्त EXT2_ET_EXTENT_NO_NEXT;
		}
		सचे (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	सचे ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		सचे (handle->level < handle->max_depth) {
			/* interior node */
			सचे (path->visit_अङ्कः > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} अञ्ञथा सचे (path->वामः < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			अञ्ञथा सचे (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अञ्ञथा
				पटिनिवत्त EXT2_ET_EXTENT_NO_PREV;
		} अञ्ञथा {
			/* leaf node */
			सचे (path->वामः < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			अञ्ञथा सचे (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अञ्ञथा
				पटिनिवत्त EXT2_ET_EXTENT_NO_PREV;
		}
		सचे (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	सचे (orig_op == EXT2_EXTENT_LAST_LEAF) {
		सचे ((handle->level < handle->max_depth) &&
		    (path->वामः == 0))
			op = EXT2_EXTENT_DOWN;
		अञ्ञथा
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		printf("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	विकल्प (op) {
	पक्ष EXT2_EXTENT_CURRENT:
		ix = path->curr;
		भङ्ग;
	पक्ष EXT2_EXTENT_मूलम्:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	पक्ष EXT2_EXTENT_FIRST_SIB:
		path->वामः = path->entries;
		path->curr = 0;
		/* fallthrough */
	पक्ष EXT2_EXTENT_NEXT_SIB:
		सचे (path->वामः <= 0)
			पटिनिवत्त EXT2_ET_EXTENT_NO_NEXT;
		सचे (path->curr) {
			ix = path->curr;
			ix++;
		} अञ्ञथा {
			eh = (संरचना ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->वामः--;
		path->curr = ix;
		path->visit_अङ्कः = 0;
		भङ्ग;
	पक्ष EXT2_EXTENT_PREV_SIB:
		सचे (!path->curr ||
		    path->वामः+1 >= path->entries)
			पटिनिवत्त EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->वामः++;
		सचे (handle->level < handle->max_depth)
			path->visit_अङ्कः = 1;
		भङ्ग;
	पक्ष EXT2_EXTENT_LAST_SIB:
		eh = (संरचना ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->वामः = 0;
		path->visit_अङ्कः = 0;
		भङ्ग;
	पक्ष EXT2_EXTENT_UP:
		सचे (handle->level <= 0)
			पटिनिवत्त EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		सचे ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_अङ्कः = 0;
		भङ्ग;
	पक्ष EXT2_EXTENT_DOWN:
	पक्ष EXT2_EXTENT_DOWN_AND_LAST:
		सचे (!path->curr ||(handle->level >= handle->max_depth))
			पटिनिवत्त EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		सचे (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			सचे (retval)
				पटिनिवत्त retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		अत्थाय (l = handle->level, tp = path; l > 0; l--, tp--) {
			सचे (blk == tp->blk)
				पटिनिवत्त EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		सचे ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		अञ्ञथा {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			सचे (retval)
				पटिनिवत्त retval;
		}
		handle->level++;

		eh = (संरचना ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		सचे (retval) {
			handle->level--;
			पटिनिवत्त retval;
		}

		सचे (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->वामः = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		सचे (newpath->वामः <= 0)
			पटिनिवत्त EXT2_ET_EXTENT_NO_DOWN;

		सचे (path->वामः > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} अञ्ञथा
			newpath->end_blk = path->end_blk;

		path = newpath;
		सचे (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((संरचना ext3_extent_header *) eh);
			path->curr = ix;
			path->वामः = path->entries - 1;
			path->visit_अङ्कः = 0;
		} अञ्ञथा {
			ix = EXT_LAST_INDEX((संरचना ext3_extent_header *) eh);
			path->curr = ix;
			path->वामः = 0;
			सचे (handle->level < handle->max_depth)
				path->visit_अङ्कः = 1;
		}
#ifdef DEBUG_GET_EXTENT
		printf("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		भङ्ग;
	स्वभाव:
		पटिनिवत्त EXT2_ET_OP_NOT_SUPPORTED;
	}

	सचे (!ix)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	printf("(Left %d)\n", path->वामः);
#endif

	सचे (handle->level == handle->max_depth) {
		ex = (संरचना ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		सचे (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} अञ्ञथा {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		सचे (path->वामः > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} अञ्ञथा
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	सचे (path->visit_अङ्कः)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	सचे (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		गच्छ retry;

	सचे ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->वामः != 0)))
		गच्छ retry;

	सचे (failed_csum)
		पटिनिवत्त EXT2_ET_EXTENT_CSUM_INVALID;

	पटिनिवत्त 0;
}

स्थिर errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	संरचना ext3_extent_idx		*ix;
	संरचना ext3_extent_header	*eh;

	सचे (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} अञ्ञथा {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (संरचना ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		सचे (retval)
			पटिनिवत्त retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	पटिनिवत्त retval;
}

#सचे 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	संरचना ext2fs_extent	extent;
	संरचना ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	सचे (retval)
		पटिनिवत्त retval;

	retval = ext2fs_extent_get_info(handle, &info);
	सचे (retval)
		पटिनिवत्त retval;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_extent_path), &save_path);
	सचे (retval)
		पटिनिवत्त retval;
	memset(save_path, 0, परिमाण(संरचना ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	पटिनिवत्त 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	पटिनिवत्त 0;
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
			      पूर्णाङ्क leaf_level, blk64_t blk)
{
	संरचना ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_मूलम्, &extent);
	सचे (retval) {
		सचे (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		पटिनिवत्त retval;
	}

	सचे (leaf_level > handle->max_depth) {
#ifdef DEBUG
		printf("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		पटिनिवत्त EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	printf("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	याव (1) {
		सचे (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			सचे ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				पटिनिवत्त 0;
			सचे (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				पटिनिवत्त EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			सचे (retval == EXT2_ET_EXTENT_NO_NEXT)
				पटिनिवत्त EXT2_ET_EXTENT_NOT_FOUND;
			सचे (retval)
				पटिनिवत्त retval;
			अनुवर्त;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		सचे (retval == EXT2_ET_EXTENT_NO_NEXT)
			गच्छ go_down;
		सचे (retval)
			पटिनिवत्त retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		सचे (blk == extent.e_lblk)
			गच्छ go_down;
		सचे (blk > extent.e_lblk)
			अनुवर्त;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		सचे (retval)
			पटिनिवत्त retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		सचे (retval)
			पटिनिवत्त retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	पटिनिवत्त ext2fs_extent_goto2(handle, 0, blk);
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
	पूर्णाङ्क				retval = 0;
	पूर्णाङ्क				orig_height;
	blk64_t				start;
	संरचना extent_path		*path;
	संरचना ext2fs_extent		extent;
	संरचना ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	सचे (!(handle->fs->flags & EXT2_FLAG_RW))
		पटिनिवत्त EXT2_ET_RO_FILSYS;

	सचे (!handle->path)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

	path = handle->path + handle->level;
	सचे (!path->curr)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	सचे (retval)
		गच्छ done;

	/* modified node's start block */
	start = extent.e_lblk;

	सचे ((retval = ext2fs_extent_get_info(handle, &info)))
		पटिनिवत्त retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	याव (handle->level > 0 &&
	       (path->वामः == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		सचे (retval)
			गच्छ done;
		सचे (extent.e_lblk == start)
			भङ्ग;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		सचे (retval)
			गच्छ done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	पटिनिवत्त retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				पूर्णाङ्क flags EXT2FS_ATTR((unused)),
				संरचना ext2fs_extent *extent)
{
	संरचना extent_path		*path;
	संरचना ext3_extent_idx		*ix;
	संरचना ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	सचे (!(handle->fs->flags & EXT2_FLAG_RW))
		पटिनिवत्त EXT2_ET_RO_FILSYS;

	सचे (!handle->path)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

	path = handle->path + handle->level;
	सचे (!path->curr)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

#ifdef DEBUG
	printf("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	सचे (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		सचे (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			सचे (extent->e_len > EXT_UNINIT_MAX_LEN)
				पटिनिवत्त EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} अञ्ञथा {
			सचे (extent->e_len > EXT_INIT_MAX_LEN)
				पटिनिवत्त EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} अञ्ञथा {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क splitting_at_eof(संरचना ext2_extent_handle *handle,
			    संरचना extent_path *path)
{
	संरचना extent_path *ppp = path;
	dump_path(__func__, handle, path);

	सचे (handle->level == 0)
		पटिनिवत्त 0;

	करोहि {
		सचे (ppp->वामः)
			पटिनिवत्त 0;
		ppp--;
	} याव (ppp >= handle->path);

	पटिनिवत्त 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
स्थिर errcode_t extent_ग्रन्थिः_split(ext2_extent_handle_t handle,
				   पूर्णाङ्क expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_ग्रन्थिः_pblk;
	blk64_t				new_ग्रन्थिः_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	पूर्णाङ्क				orig_height;
	वर्ण				*block_buf = रिक्त;
	संरचना ext2fs_extent		extent;
	संरचना extent_path		*path, *newpath = 0;
	संरचना ext3_extent_header	*eh, *neweh;
	पूर्णाङ्क				tocopy;
	पूर्णाङ्क				new_मूलम् = 0;
	संरचना ext2_extent_info		info;
	पूर्णाङ्क				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	सचे (!(handle->fs->flags & EXT2_FLAG_RW))
		पटिनिवत्त EXT2_ET_RO_FILSYS;

	सचे (!handle->path)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

#ifdef DEBUG
	printf("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	सचे (retval)
		गच्छ done;

	retval = ext2fs_extent_get_info(handle, &info);
	सचे (retval)
		गच्छ done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (संरचना ext3_extent_header *) path->buf;
	सचे (handle->level == handle->max_depth) {
		संरचना ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} अञ्ञथा {
		संरचना ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	सचे (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		printf("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		सचे (retval)
			गच्छ done;

		retval = extent_ग्रन्थिः_split(handle, expand_allowed);
		सचे (retval)
			गच्छ done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		सचे (retval)
			गच्छ done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	सचे (!path->curr)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (संरचना ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	सचे (handle->level == 0) {
		new_मूलम् = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    परिमाण(संरचना extent_path),
					    &newpath);
		सचे (retval)
			गच्छ done;
	} अञ्ञथा {
		सचे (no_balance)
			tocopy = 1;
		अञ्ञथा
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	printf("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	सचे (!tocopy && !no_balance) {
#ifdef DEBUG
		printf("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		गच्छ done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = malloc(handle->fs->blocksize);
	सचे (!block_buf) {
		retval = ENOMEM;
		गच्छ done;
	}

	सचे (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_ग्रन्थिः_pblk);
	सचे (retval)
		गच्छ done;

#ifdef DEBUG
	printf("will copy to new node at block %lu\n",
	       (unsigned long) new_ग्रन्थिः_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (संरचना ext3_extent_header *) block_buf;
	memcpy(neweh, eh, परिमाण(संरचना ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 परिमाण(संरचना ext3_extent_header)) /
				परिमाण(संरचना ext3_extent));

	/* then the entries for the new block... */
	memcpy(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		परिमाण(संरचना ext3_extent_idx) * tocopy);

	new_ग्रन्थिः_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	सचे (retval)
		गच्छ done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_ग्रन्थिः_pblk, 1,
					block_buf);

	सचे (retval)
		गच्छ done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	सचे (handle->level == 0) {
		memcpy(newpath, path,
		       परिमाण(संरचना extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->वामः = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} अञ्ञथा {
		path->entries -= tocopy;
		path->वामः -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	सचे (retval)
		गच्छ done;

	/* now go up and insert/replace index for new node we created */
	सचे (new_मूलम्) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		सचे (retval)
			गच्छ done;

		extent.e_lblk = new_ग्रन्थिः_start;
		extent.e_pblk = new_ग्रन्थिः_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		सचे (retval)
			गच्छ done;
	} अञ्ञथा {
		__u32 new_ग्रन्थिः_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_ग्रन्थिः_length = new_ग्रन्थिः_start - extent.e_lblk;
		extent.e_len -= new_ग्रन्थिः_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		सचे (retval)
			गच्छ done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_ग्रन्थिः_start;
		extent.e_pblk = new_ग्रन्थिः_pblk;
		extent.e_len = new_ग्रन्थिः_length;
		retval = ext2fs_extent_निवेशय(handle, EXT2_EXTENT_निवेशय_AFTER, &extent);
		सचे (retval)
			गच्छ done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	सचे (retval)
		गच्छ done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	सचे (retval)
		गच्छ done;

done:
	सचे (newpath)
		ext2fs_free_mem(&newpath);
	मुक्त(block_buf);

	पटिनिवत्त retval;
}

errcode_t ext2fs_extent_ग्रन्थिः_split(ext2_extent_handle_t handle)
{
	पटिनिवत्त extent_ग्रन्थिः_split(handle, 0);
}

errcode_t ext2fs_extent_निवेशय(ext2_extent_handle_t handle, पूर्णाङ्क flags,
				      संरचना ext2fs_extent *extent)
{
	संरचना extent_path		*path;
	संरचना ext3_extent_idx		*ix;
	संरचना ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	सचे (!(handle->fs->flags & EXT2_FLAG_RW))
		पटिनिवत्त EXT2_ET_RO_FILSYS;

	सचे (!handle->path)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

#ifdef DEBUG
	printf("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	सचे (path->entries >= path->max_entries) {
		सचे (flags & EXT2_EXTENT_निवेशय_NOSPLIT) {
			पटिनिवत्त EXT2_ET_CANT_निवेशय_EXTENT;
		} अञ्ञथा {
#ifdef DEBUG
			printf("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_ग्रन्थिः_split(handle, 1);
			सचे (retval)
				पटिनिवत्त retval;
			path = handle->path + handle->level;
		}
	}

	eh = (संरचना ext3_extent_header *) path->buf;
	सचे (path->curr) {
		ix = path->curr;
		सचे (flags & EXT2_EXTENT_निवेशय_AFTER) {
			ix++;
			path->वामः--;
		}
	} अञ्ञथा {
		ix = EXT_FIRST_INDEX(eh);
		path->वामः = -1;
	}

	path->curr = ix;

	सचे (path->वामः >= 0)
		memmove(ix + 1, ix,
			(path->वामः+1) * परिमाण(संरचना ext3_extent_idx));
	path->वामः++;
	path->entries++;

	eh = (संरचना ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	सचे (retval)
		गच्छ errout;

	retval = update_path(handle);
	सचे (retval)
		गच्छ errout;

	पटिनिवत्त 0;

errout:
	ext2fs_extent_delete(handle, 0);
	पटिनिवत्त retval;
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
				 blk64_t logical, blk64_t physical, पूर्णाङ्क flags)
{
	errcode_t		ec, retval = 0;
	पूर्णाङ्क			mapped = 1; /* logical is mapped? */
	पूर्णाङ्क			orig_height;
	पूर्णाङ्क			extent_uninit = 0;
	पूर्णाङ्क			prev_uninit = 0;
	पूर्णाङ्क			next_uninit = 0;
	पूर्णाङ्क			new_uninit = 0;
	पूर्णाङ्क			max_len = EXT_INIT_MAX_LEN;
	पूर्णाङ्क			has_prev, has_next;
	blk64_t			orig_lblk;
	संरचना extent_path	*path;
	संरचना ext2fs_extent	extent, next_extent, prev_extent;
	संरचना ext2fs_extent	newextent;
	संरचना ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	printf("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	सचे (!(handle->fs->flags & EXT2_FLAG_RW))
		पटिनिवत्त EXT2_ET_RO_FILSYS;

	सचे (!handle->path)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

	path = handle->path + handle->level;

	सचे (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	सचे (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		सचे (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	सचे ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_निवेशय(handle, 0, &newextent);
		पटिनिवत्त retval;
	}

	/* save our original location in the extent tree */
	सचे ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		सचे (retval != EXT2_ET_NO_CURRENT_ग्रन्थिः)
			पटिनिवत्त retval;
		memset(&extent, 0, परिमाण(extent));
	}
	सचे ((retval = ext2fs_extent_get_info(handle, &info)))
		पटिनिवत्त retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	सचे (retval) {
		सचे (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			सचे (!physical) {
#ifdef DEBUG
				printf("block %llu already unmapped\n",
					logical);
#endif
				गच्छ done;
			}
		} अञ्ञथा
			गच्छ done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	सचे (retval)
		गच्छ done;
	सचे (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	सचे (retval) {
		has_next = 0;
		सचे (retval != EXT2_ET_EXTENT_NO_NEXT)
			गच्छ done;
	} अञ्ञथा {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		सचे (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	सचे (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		गच्छ done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	सचे (retval) {
		has_prev = 0;
		सचे (retval != EXT2_ET_EXTENT_NO_PREV)
			गच्छ done;
	} अञ्ञथा {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		सचे (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	सचे (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		गच्छ done;

	/* check if already pointing to the requested physical */
	सचे (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		printf("physical block (at %llu) unchanged\n", logical);
#endif
		गच्छ done;
	}

	सचे (!mapped) {
#ifdef DEBUG
		printf("mapping unmapped logical block %llu\n", logical);
#endif
		सचे ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((पूर्णाङ्क) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} अञ्ञथा सचे ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((पूर्णाङ्क) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} अञ्ञथा सचे (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((पूर्णाङ्क) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			सचे (retval)
				गच्छ done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} अञ्ञथा सचे (logical < extent.e_lblk)
			retval = ext2fs_extent_निवेशय(handle, 0, &newextent);
		अञ्ञथा
			retval = ext2fs_extent_निवेशय(handle,
				      EXT2_EXTENT_निवेशय_AFTER, &newextent);
		सचे (retval)
			गच्छ done;
		retval = ext2fs_extent_fix_parents(handle);
		सचे (retval)
			गच्छ done;
	} अञ्ञथा सचे ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		printf("(re/un)mapping only block in extent\n");
#endif
		सचे (physical) {
			सचे (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((पूर्णाङ्क) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				सचे (retval)
					गच्छ done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				सचे (retval)
					गच्छ done;
				गच्छ delete_ग्रन्थिः;

			} अञ्ञथा
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} अञ्ञथा {
		delete_ग्रन्थिः:
			retval = ext2fs_extent_delete(handle, 0);
			सचे (retval)
				गच्छ done;
			ec = ext2fs_extent_fix_parents(handle);
			सचे (ec != EXT2_ET_NO_CURRENT_ग्रन्थिः)
				retval = ec;
		}

		सचे (retval)
			गच्छ done;
	} अञ्ञथा सचे (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		printf("(re/un)mapping last block in extent\n");
#endif
		सचे (physical) {
			सचे (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((पूर्णाङ्क) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				सचे (retval)
					गच्छ done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				सचे (retval)
					गच्छ done;
			} अञ्ञथा
				retval = ext2fs_extent_निवेशय(handle,
				      EXT2_EXTENT_निवेशय_AFTER, &newextent);
			सचे (retval)
				गच्छ done;
			retval = ext2fs_extent_fix_parents(handle);
			सचे (retval)
				गच्छ done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			सचे (retval)
				गच्छ done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			सचे (retval)
				गच्छ done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		सचे (retval)
			गच्छ done;
	} अञ्ञथा सचे (logical == extent.e_lblk) {
#ifdef DEBUG
		printf("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		सचे (retval)
			गच्छ done;
		retval = ext2fs_extent_fix_parents(handle);
		सचे (retval)
			गच्छ done;
		सचे (physical) {
			सचे (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((पूर्णाङ्क) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				सचे (retval)
					गच्छ done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} अञ्ञथा
				retval = ext2fs_extent_निवेशय(handle,
							      0, &newextent);
			सचे (retval)
				गच्छ done;
			retval = ext2fs_extent_fix_parents(handle);
			सचे (retval)
				गच्छ done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			सचे (retval)
				गच्छ done;
		}
	} अञ्ञथा {
		__u32	save_length;
		blk64_t	save_lblk;
		संरचना ext2fs_extent save_extent;
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
		सचे (retval)
			गच्छ done;
		/* insert our new extent, if any */
		सचे (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_निवेशय(handle,
					EXT2_EXTENT_निवेशय_AFTER, &newextent);
			सचे (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				सचे (r2 == 0)
					(शून्य)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				गच्छ done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_length - extent.e_len - 1;
		retval = ext2fs_extent_निवेशय(handle,
				EXT2_EXTENT_निवेशय_AFTER, &extent);
		सचे (retval) {
			सचे (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				सचे (r2 == 0)
					(शून्य)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			सचे (r2 == 0)
				(शून्य)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			गच्छ done;
		}
	}

done:
	/* get handle back to its position */
	सचे (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	पटिनिवत्त retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, पूर्णाङ्क flags)
{
	संरचना extent_path		*path;
	वर्ण 				*cp;
	संरचना ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	सचे (!(handle->fs->flags & EXT2_FLAG_RW))
		पटिनिवत्त EXT2_ET_RO_FILSYS;

	सचे (!handle->path)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

#ifdef DEBUG
	{
		संरचना ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		सचे (retval == 0) {
			printf("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	सचे (!path->curr)
		पटिनिवत्त EXT2_ET_NO_CURRENT_ग्रन्थिः;

	cp = path->curr;

	/* Sanity check before memmove() */
	सचे (path->वामः < 0)
		पटिनिवत्त EXT2_ET_EXTENT_LEAF_BAD;

	सचे (path->वामः) {
		memmove(cp, cp + परिमाण(संरचना ext3_extent_idx),
			path->वामः * परिमाण(संरचना ext3_extent_idx));
		path->वामः--;
	} अञ्ञथा {
		संरचना ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	सचे (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	सचे (path->entries == 0 && handle->level) {
		सचे (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			संरचना ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			सचे (retval)
				पटिनिवत्त retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} अञ्ञथा {
		eh = (संरचना ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		सचे ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	पटिनिवत्त retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 संरचना ext2_extent_info *info)
{
	संरचना extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, परिमाण(संरचना ext2_extent_info));

	path = handle->path + handle->level;
	सचे (path) {
		सचे (path->curr)
			info->curr_entry = ((वर्ण *) path->curr - path->buf) /
				परिमाण(संरचना ext3_extent_idx);
		अञ्ञथा
			info->curr_entry = 0;
		info->अङ्कः_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			परिमाण(संरचना ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	पटिनिवत्त 0;
}

size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	size_t iblock_sz = परिमाण(((संरचना ext2_inode *)रिक्त)->i_block);
	size_t iblock_extents = (iblock_sz - परिमाण(संरचना ext3_extent_header)) /
				परिमाण(संरचना ext3_extent);
	size_t extents_per_block = (handle->fs->blocksize -
				    परिमाण(संरचना ext3_extent_header)) /
				   परिमाण(संरचना ext3_extent);
	स्थिर unsigned पूर्णाङ्क last_blocksize = 0;
	स्थिर size_t last_result = 0;

	सचे (last_blocksize && last_blocksize == handle->fs->blocksize)
		पटिनिवत्त last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	पटिनिवत्त last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       संरचना ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	संरचना ext2fs_extent	extent;
	errcode_t		errcode;
	पूर्णाङ्क    			save_flags = fs->flags;

	सचे (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		पटिनिवत्त 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	सचे (errcode) {
		सचे (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		पटिनिवत्त errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_मूलम्, &extent);
	सचे (errcode)
		गच्छ out;

	करोहि {
		/* Skip to the end of a block of leaf nodes */
		सचे (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			सचे (errcode)
				भङ्ग;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		सचे (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} याव (errcode == 0);

out:
	/* Ok if we run off the end */
	सचे (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	पटिनिवत्त errcode;
}

errcode_t ext2fs_decode_extent(संरचना ext2fs_extent *to, शून्य *addr, पूर्णाङ्क len)
{
	संरचना ext3_extent *from = (संरचना ext3_extent *)addr;

	सचे (len != परिमाण(संरचना ext3_extent))
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	सचे (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	पटिनिवत्त 0;
}

errcode_t ext2fs_गणनम्_blocks(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode *inode, blk64_t *ret_गणनम्)
{
	ext2_extent_handle_t	handle = रिक्त;
	संरचना ext2fs_extent	extent;
	errcode_t		errcode;
	पूर्णाङ्क			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	सचे (errcode)
		गच्छ out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_मूलम्, &extent);
	सचे (errcode)
		गच्छ out;

	errcode = ext2fs_get_array(handle->max_depth, परिमाण(blk64_t),
				   &intermediate_nodes);
	सचे (errcode)
		गच्छ out;

	blkcount = handle->level;
	याव (!errcode) {
		सचे (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			अत्थाय (i = 0; i < handle->level; i++) {
				सचे (intermediate_nodes[i] !=
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
	*ret_गणनम् = blkcount;
	ext2fs_extent_free(handle);

	पटिनिवत्त 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
ध्रुव वर्ण *debug_prog_name = "tst_extents";

#endif

