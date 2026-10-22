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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
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
	अक्षर		*buf;
	पूर्णांक		entries;
	पूर्णांक		max_entries;
	पूर्णांक		डावा;
	पूर्णांक		visit_अंक;
	पूर्णांक		flags;
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
	पूर्णांक			type;
	पूर्णांक			level;
	पूर्णांक			max_depth;
	पूर्णांक			max_paths;
	संरचना extent_path	*path;
};

संरचना ext2_extent_path {
	errcode_t		magic;
	पूर्णांक			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
स्थिर शून्य dbg_show_header(संरचना ext3_extent_header *eh)
{
	म_लिखो("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

स्थिर शून्य dbg_show_index(संरचना ext3_extent_idx *ix)
{
	म_लिखो("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

स्थिर शून्य dbg_show_extent(संरचना ext3_extent *ex)
{
	म_लिखो("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

स्थिर शून्य dbg_print_extent(अक्षर *desc, संरचना ext2fs_extent *extent)
{
	जर (desc)
		म_लिखो("%s: ", desc);
	म_लिखो("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	जर (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		ख_माला_दो("LEAF ", मानक_निकास);
	जर (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		ख_माला_दो("UNINIT ", मानक_निकास);
	जर (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		ख_माला_दो("2ND_VISIT ", मानक_निकास);
	जर (!extent->e_flags)
		ख_माला_दो("(none)", मानक_निकास);
	ख_अक्षर_दो('\n', मानक_निकास);

}

स्थिर शून्य dump_path(स्थिरांक अक्षर *tag, संरचना ext2_extent_handle *handle,
		      संरचना extent_path *path)
{
	संरचना extent_path *ppp = path;
	म_लिखो("%s: level=%d\n", tag, handle->level);

	करा {
		म_लिखो("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->डावा, ppp->visit_अंक, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (शून्य *)ppp->buf);
		म_लिखो("  ");
		dbg_show_header((संरचना ext3_extent_header *)ppp->buf);
		जर (ppp->curr) {
			म_लिखो("  ");
			dbg_show_index(ppp->curr);
			म_लिखो("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} पर्यंत (ppp >= handle->path);
	ख_साफ(मानक_निकास);

	परत;
}

#अन्यथा
#define dbg_show_header(eh) करा { } पर्यंत (0)
#define dbg_show_index(ix) करा { } पर्यंत (0)
#define dbg_show_extent(ex) करा { } पर्यंत (0)
#define dbg_print_extent(desc, ex) करा { } पर्यंत (0)
#define dump_path(tag, handle, path) करा { } पर्यंत (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(शून्य *ptr, पूर्णांक size)
{
	पूर्णांक eh_max, entry_size;
	संरचना ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	जर (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		परत EXT2_ET_EXTENT_HEADER_BAD;
	जर (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		परत EXT2_ET_EXTENT_HEADER_BAD;
	जर (eh->eh_depth == 0)
		entry_size = आकार(संरचना ext3_extent);
	अन्यथा
		entry_size = आकार(संरचना ext3_extent_idx);

	eh_max = (size - आकार(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	जर ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		परत EXT2_ET_EXTENT_HEADER_BAD;

	परत 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
शून्य ext2fs_extent_free(ext2_extent_handle_t handle)
{
	पूर्णांक			i;

	जर (!handle)
		परत;

	जर (handle->path) {
		साठी (i = 1; i < handle->max_paths; i++) {
			जर (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	परत ext2fs_extent_open2(fs, ino, रिक्त, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    संरचना ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	संरचना ext2_extent_handle	*handle;
	errcode_t			retval;
	पूर्णांक				i;
	संरचना ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जर (!inode)
		जर ((ino == 0) || (ino > fs->super->s_inodes_मोजणी))
			परत EXT2_ET_BAD_INODE_अंक;

	retval = ext2fs_get_mem(आकार(संरचना ext2_extent_handle), &handle);
	जर (retval)
		परत retval;
	स_रखो(handle, 0, आकार(संरचना ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	जर (inode) {
		handle->inode = inode;
	} अन्यथा {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		जर (retval)
			जा errout;
	}

	eh = (संरचना ext3_extent_header *) &handle->inode->i_block[0];

	साठी (i=0; i < EXT2_N_BLOCKS; i++)
		जर (handle->inode->i_block[i])
			थांबा;
	जर (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (आकार(handle->inode->i_block) - आकार(*eh)) /
			आकार(संरचना ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	जर (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		जा errout;
	}

	retval = ext2fs_extent_header_verify(eh, आकार(handle->inode->i_block));
	जर (retval)
		जा errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    आकार(संरचना extent_path),
				    &handle->path);
	handle->path[0].buf = (अक्षर *) handle->inode->i_block;

	handle->path[0].डावा = handle->path[0].entries =
		ext2fs_le16_to_cpu(eh->eh_entries);
	handle->path[0].max_entries = ext2fs_le16_to_cpu(eh->eh_max);
	handle->path[0].curr = 0;
	handle->path[0].end_blk =
		(EXT2_I_SIZE(handle->inode) + fs->blocksize - 1) >>
		 EXT2_BLOCK_SIZE_BITS(fs->super);
	handle->path[0].blk = 0;
	handle->path[0].visit_अंक = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	परत 0;

errout:
	ext2fs_extent_free(handle);
	परत retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    पूर्णांक flags, संरचना ext2fs_extent *extent)
{
	संरचना extent_path	*path, *newpath, *tp;
	संरचना ext3_extent_header	*eh;
	संरचना ext3_extent_idx		*ix = 0;
	संरचना ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	पूर्णांक				orig_op, op, l;
	पूर्णांक				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	जर (!handle->path)
		परत EXT2_ET_NO_CURRENT_गाठ;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	जर ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		जर (handle->level < handle->max_depth) {
			/* interior node */
			जर (path->visit_अंक == 0) {
				path->visit_अंक++;
				op = EXT2_EXTENT_DOWN;
			} अन्यथा जर (path->डावा > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			अन्यथा जर (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अन्यथा
				परत EXT2_ET_EXTENT_NO_NEXT;
		} अन्यथा {
			/* leaf node */
			जर (path->डावा > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			अन्यथा जर (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अन्यथा
				परत EXT2_ET_EXTENT_NO_NEXT;
		}
		जर (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			म_लिखो("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	जर ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		जर (handle->level < handle->max_depth) {
			/* interior node */
			जर (path->visit_अंक > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} अन्यथा जर (path->डावा < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			अन्यथा जर (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अन्यथा
				परत EXT2_ET_EXTENT_NO_PREV;
		} अन्यथा {
			/* leaf node */
			जर (path->डावा < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			अन्यथा जर (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अन्यथा
				परत EXT2_ET_EXTENT_NO_PREV;
		}
		जर (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			म_लिखो("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	जर (orig_op == EXT2_EXTENT_LAST_LEAF) {
		जर ((handle->level < handle->max_depth) &&
		    (path->डावा == 0))
			op = EXT2_EXTENT_DOWN;
		अन्यथा
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		म_लिखो("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	निवड (op) {
	स्थिती EXT2_EXTENT_CURRENT:
		ix = path->curr;
		थांबा;
	स्थिती EXT2_EXTENT_मूळ:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	स्थिती EXT2_EXTENT_FIRST_SIB:
		path->डावा = path->entries;
		path->curr = 0;
		/* fallthrough */
	स्थिती EXT2_EXTENT_NEXT_SIB:
		जर (path->डावा <= 0)
			परत EXT2_ET_EXTENT_NO_NEXT;
		जर (path->curr) {
			ix = path->curr;
			ix++;
		} अन्यथा {
			eh = (संरचना ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->डावा--;
		path->curr = ix;
		path->visit_अंक = 0;
		थांबा;
	स्थिती EXT2_EXTENT_PREV_SIB:
		जर (!path->curr ||
		    path->डावा+1 >= path->entries)
			परत EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->डावा++;
		जर (handle->level < handle->max_depth)
			path->visit_अंक = 1;
		थांबा;
	स्थिती EXT2_EXTENT_LAST_SIB:
		eh = (संरचना ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->डावा = 0;
		path->visit_अंक = 0;
		थांबा;
	स्थिती EXT2_EXTENT_UP:
		जर (handle->level <= 0)
			परत EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		जर ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_अंक = 0;
		थांबा;
	स्थिती EXT2_EXTENT_DOWN:
	स्थिती EXT2_EXTENT_DOWN_AND_LAST:
		जर (!path->curr ||(handle->level >= handle->max_depth))
			परत EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		जर (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			जर (retval)
				परत retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		साठी (l = handle->level, tp = path; l > 0; l--, tp--) {
			जर (blk == tp->blk)
				परत EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		जर ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			स_रखो(newpath->buf, 0, handle->fs->blocksize);
		अन्यथा {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			जर (retval)
				परत retval;
		}
		handle->level++;

		eh = (संरचना ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		जर (retval) {
			handle->level--;
			परत retval;
		}

		जर (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->डावा = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		जर (newpath->डावा <= 0)
			परत EXT2_ET_EXTENT_NO_DOWN;

		जर (path->डावा > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} अन्यथा
			newpath->end_blk = path->end_blk;

		path = newpath;
		जर (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((संरचना ext3_extent_header *) eh);
			path->curr = ix;
			path->डावा = path->entries - 1;
			path->visit_अंक = 0;
		} अन्यथा {
			ix = EXT_LAST_INDEX((संरचना ext3_extent_header *) eh);
			path->curr = ix;
			path->डावा = 0;
			जर (handle->level < handle->max_depth)
				path->visit_अंक = 1;
		}
#ifdef DEBUG_GET_EXTENT
		म_लिखो("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		थांबा;
	पूर्वनिर्धारित:
		परत EXT2_ET_OP_NOT_SUPPORTED;
	}

	जर (!ix)
		परत EXT2_ET_NO_CURRENT_गाठ;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	म_लिखो("(Left %d)\n", path->डावा);
#endif

	जर (handle->level == handle->max_depth) {
		ex = (संरचना ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		जर (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} अन्यथा {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		जर (path->डावा > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} अन्यथा
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	जर (path->visit_अंक)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	जर (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		जा retry;

	जर ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->डावा != 0)))
		जा retry;

	जर (failed_csum)
		परत EXT2_ET_EXTENT_CSUM_INVALID;

	परत 0;
}

स्थिर errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	संरचना ext3_extent_idx		*ix;
	संरचना ext3_extent_header	*eh;

	जर (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} अन्यथा {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (संरचना ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		जर (retval)
			परत retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	परत retval;
}

#जर 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	संरचना ext2fs_extent	extent;
	संरचना ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	जर (retval)
		परत retval;

	retval = ext2fs_extent_get_info(handle, &info);
	जर (retval)
		परत retval;

	retval = ext2fs_get_mem(आकार(संरचना ext2_extent_path), &save_path);
	जर (retval)
		परत retval;
	स_रखो(save_path, 0, आकार(संरचना ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	परत 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	परत 0;
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
			      पूर्णांक leaf_level, blk64_t blk)
{
	संरचना ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_मूळ, &extent);
	जर (retval) {
		जर (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		परत retval;
	}

	जर (leaf_level > handle->max_depth) {
#ifdef DEBUG
		म_लिखो("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		परत EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	म_लिखो("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	पर्यंत (1) {
		जर (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			जर ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				परत 0;
			जर (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				परत EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			जर (retval == EXT2_ET_EXTENT_NO_NEXT)
				परत EXT2_ET_EXTENT_NOT_FOUND;
			जर (retval)
				परत retval;
			सुरूठेवा;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		जर (retval == EXT2_ET_EXTENT_NO_NEXT)
			जा go_down;
		जर (retval)
			परत retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		जर (blk == extent.e_lblk)
			जा go_down;
		जर (blk > extent.e_lblk)
			सुरूठेवा;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		जर (retval)
			परत retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		जर (retval)
			परत retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	परत ext2fs_extent_goto2(handle, 0, blk);
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
	पूर्णांक				retval = 0;
	पूर्णांक				orig_height;
	blk64_t				start;
	संरचना extent_path		*path;
	संरचना ext2fs_extent		extent;
	संरचना ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	जर (!(handle->fs->flags & EXT2_FLAG_RW))
		परत EXT2_ET_RO_FILSYS;

	जर (!handle->path)
		परत EXT2_ET_NO_CURRENT_गाठ;

	path = handle->path + handle->level;
	जर (!path->curr)
		परत EXT2_ET_NO_CURRENT_गाठ;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	जर (retval)
		जा done;

	/* modified node's start block */
	start = extent.e_lblk;

	जर ((retval = ext2fs_extent_get_info(handle, &info)))
		परत retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	पर्यंत (handle->level > 0 &&
	       (path->डावा == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		जर (retval)
			जा done;
		जर (extent.e_lblk == start)
			थांबा;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		जर (retval)
			जा done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	परत retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				पूर्णांक flags EXT2FS_ATTR((unused)),
				संरचना ext2fs_extent *extent)
{
	संरचना extent_path		*path;
	संरचना ext3_extent_idx		*ix;
	संरचना ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	जर (!(handle->fs->flags & EXT2_FLAG_RW))
		परत EXT2_ET_RO_FILSYS;

	जर (!handle->path)
		परत EXT2_ET_NO_CURRENT_गाठ;

	path = handle->path + handle->level;
	जर (!path->curr)
		परत EXT2_ET_NO_CURRENT_गाठ;

#ifdef DEBUG
	म_लिखो("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	जर (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		जर (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			जर (extent->e_len > EXT_UNINIT_MAX_LEN)
				परत EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} अन्यथा {
			जर (extent->e_len > EXT_INIT_MAX_LEN)
				परत EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} अन्यथा {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	परत 0;
}

स्थिर पूर्णांक splitting_at_eof(संरचना ext2_extent_handle *handle,
			    संरचना extent_path *path)
{
	संरचना extent_path *ppp = path;
	dump_path(__func__, handle, path);

	जर (handle->level == 0)
		परत 0;

	करा {
		जर (ppp->डावा)
			परत 0;
		ppp--;
	} पर्यंत (ppp >= handle->path);

	परत 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
स्थिर errcode_t extent_गाठ_split(ext2_extent_handle_t handle,
				   पूर्णांक expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_गाठ_pblk;
	blk64_t				new_गाठ_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	पूर्णांक				orig_height;
	अक्षर				*block_buf = रिक्त;
	संरचना ext2fs_extent		extent;
	संरचना extent_path		*path, *newpath = 0;
	संरचना ext3_extent_header	*eh, *neweh;
	पूर्णांक				tocopy;
	पूर्णांक				new_मूळ = 0;
	संरचना ext2_extent_info		info;
	पूर्णांक				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	जर (!(handle->fs->flags & EXT2_FLAG_RW))
		परत EXT2_ET_RO_FILSYS;

	जर (!handle->path)
		परत EXT2_ET_NO_CURRENT_गाठ;

#ifdef DEBUG
	म_लिखो("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	जर (retval)
		जा done;

	retval = ext2fs_extent_get_info(handle, &info);
	जर (retval)
		जा done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (संरचना ext3_extent_header *) path->buf;
	जर (handle->level == handle->max_depth) {
		संरचना ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} अन्यथा {
		संरचना ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	जर (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		म_लिखो("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		जर (retval)
			जा done;

		retval = extent_गाठ_split(handle, expand_allowed);
		जर (retval)
			जा done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		जर (retval)
			जा done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	जर (!path->curr)
		परत EXT2_ET_NO_CURRENT_गाठ;

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
	जर (handle->level == 0) {
		new_मूळ = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    आकार(संरचना extent_path),
					    &newpath);
		जर (retval)
			जा done;
	} अन्यथा {
		जर (no_balance)
			tocopy = 1;
		अन्यथा
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	म_लिखो("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	जर (!tocopy && !no_balance) {
#ifdef DEBUG
		म_लिखो("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		जा done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = दो_स्मृति(handle->fs->blocksize);
	जर (!block_buf) {
		retval = ENOMEM;
		जा done;
	}

	जर (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_गाठ_pblk);
	जर (retval)
		जा done;

#ifdef DEBUG
	म_लिखो("will copy to new node at block %lu\n",
	       (अचिन्हित दीर्घ) new_गाठ_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (संरचना ext3_extent_header *) block_buf;
	स_नकल(neweh, eh, आकार(संरचना ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 आकार(संरचना ext3_extent_header)) /
				आकार(संरचना ext3_extent));

	/* then the entries for the new block... */
	स_नकल(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		आकार(संरचना ext3_extent_idx) * tocopy);

	new_गाठ_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	जर (retval)
		जा done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_गाठ_pblk, 1,
					block_buf);

	जर (retval)
		जा done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	जर (handle->level == 0) {
		स_नकल(newpath, path,
		       आकार(संरचना extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->डावा = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} अन्यथा {
		path->entries -= tocopy;
		path->डावा -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	जर (retval)
		जा done;

	/* now go up and insert/replace index for new node we created */
	जर (new_मूळ) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		जर (retval)
			जा done;

		extent.e_lblk = new_गाठ_start;
		extent.e_pblk = new_गाठ_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		जर (retval)
			जा done;
	} अन्यथा {
		__u32 new_गाठ_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_गाठ_length = new_गाठ_start - extent.e_lblk;
		extent.e_len -= new_गाठ_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		जर (retval)
			जा done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_गाठ_start;
		extent.e_pblk = new_गाठ_pblk;
		extent.e_len = new_गाठ_length;
		retval = ext2fs_extent_घाला(handle, EXT2_EXTENT_घाला_AFTER, &extent);
		जर (retval)
			जा done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	जर (retval)
		जा done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	जर (retval)
		जा done;

done:
	जर (newpath)
		ext2fs_free_mem(&newpath);
	मुक्त(block_buf);

	परत retval;
}

errcode_t ext2fs_extent_गाठ_split(ext2_extent_handle_t handle)
{
	परत extent_गाठ_split(handle, 0);
}

errcode_t ext2fs_extent_घाला(ext2_extent_handle_t handle, पूर्णांक flags,
				      संरचना ext2fs_extent *extent)
{
	संरचना extent_path		*path;
	संरचना ext3_extent_idx		*ix;
	संरचना ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	जर (!(handle->fs->flags & EXT2_FLAG_RW))
		परत EXT2_ET_RO_FILSYS;

	जर (!handle->path)
		परत EXT2_ET_NO_CURRENT_गाठ;

#ifdef DEBUG
	म_लिखो("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	जर (path->entries >= path->max_entries) {
		जर (flags & EXT2_EXTENT_घाला_NOSPLIT) {
			परत EXT2_ET_CANT_घाला_EXTENT;
		} अन्यथा {
#ifdef DEBUG
			म_लिखो("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_गाठ_split(handle, 1);
			जर (retval)
				परत retval;
			path = handle->path + handle->level;
		}
	}

	eh = (संरचना ext3_extent_header *) path->buf;
	जर (path->curr) {
		ix = path->curr;
		जर (flags & EXT2_EXTENT_घाला_AFTER) {
			ix++;
			path->डावा--;
		}
	} अन्यथा {
		ix = EXT_FIRST_INDEX(eh);
		path->डावा = -1;
	}

	path->curr = ix;

	जर (path->डावा >= 0)
		स_हटाओ(ix + 1, ix,
			(path->डावा+1) * आकार(संरचना ext3_extent_idx));
	path->डावा++;
	path->entries++;

	eh = (संरचना ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	जर (retval)
		जा errout;

	retval = update_path(handle);
	जर (retval)
		जा errout;

	परत 0;

errout:
	ext2fs_extent_delete(handle, 0);
	परत retval;
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
				 blk64_t logical, blk64_t physical, पूर्णांक flags)
{
	errcode_t		ec, retval = 0;
	पूर्णांक			mapped = 1; /* logical is mapped? */
	पूर्णांक			orig_height;
	पूर्णांक			extent_uninit = 0;
	पूर्णांक			prev_uninit = 0;
	पूर्णांक			next_uninit = 0;
	पूर्णांक			new_uninit = 0;
	पूर्णांक			max_len = EXT_INIT_MAX_LEN;
	पूर्णांक			has_prev, has_next;
	blk64_t			orig_lblk;
	संरचना extent_path	*path;
	संरचना ext2fs_extent	extent, next_extent, prev_extent;
	संरचना ext2fs_extent	newextent;
	संरचना ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	म_लिखो("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	जर (!(handle->fs->flags & EXT2_FLAG_RW))
		परत EXT2_ET_RO_FILSYS;

	जर (!handle->path)
		परत EXT2_ET_NO_CURRENT_गाठ;

	path = handle->path + handle->level;

	जर (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	जर (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		जर (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	जर ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_घाला(handle, 0, &newextent);
		परत retval;
	}

	/* save our original location in the extent tree */
	जर ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		जर (retval != EXT2_ET_NO_CURRENT_गाठ)
			परत retval;
		स_रखो(&extent, 0, आकार(extent));
	}
	जर ((retval = ext2fs_extent_get_info(handle, &info)))
		परत retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	जर (retval) {
		जर (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			जर (!physical) {
#ifdef DEBUG
				म_लिखो("block %llu already unmapped\n",
					logical);
#endif
				जा done;
			}
		} अन्यथा
			जा done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	जर (retval)
		जा done;
	जर (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	जर (retval) {
		has_next = 0;
		जर (retval != EXT2_ET_EXTENT_NO_NEXT)
			जा done;
	} अन्यथा {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		जर (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	जर (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		जा done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	जर (retval) {
		has_prev = 0;
		जर (retval != EXT2_ET_EXTENT_NO_PREV)
			जा done;
	} अन्यथा {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		जर (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	जर (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		जा done;

	/* check if already pointing to the requested physical */
	जर (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		म_लिखो("physical block (at %llu) unchanged\n", logical);
#endif
		जा done;
	}

	जर (!mapped) {
#ifdef DEBUG
		म_लिखो("mapping unmapped logical block %llu\n", logical);
#endif
		जर ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((पूर्णांक) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} अन्यथा जर ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((पूर्णांक) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} अन्यथा जर (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((पूर्णांक) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			जर (retval)
				जा done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} अन्यथा जर (logical < extent.e_lblk)
			retval = ext2fs_extent_घाला(handle, 0, &newextent);
		अन्यथा
			retval = ext2fs_extent_घाला(handle,
				      EXT2_EXTENT_घाला_AFTER, &newextent);
		जर (retval)
			जा done;
		retval = ext2fs_extent_fix_parents(handle);
		जर (retval)
			जा done;
	} अन्यथा जर ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		म_लिखो("(re/un)mapping only block in extent\n");
#endif
		जर (physical) {
			जर (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((पूर्णांक) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				जर (retval)
					जा done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				जर (retval)
					जा done;
				जा delete_गाठ;

			} अन्यथा
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} अन्यथा {
		delete_गाठ:
			retval = ext2fs_extent_delete(handle, 0);
			जर (retval)
				जा done;
			ec = ext2fs_extent_fix_parents(handle);
			जर (ec != EXT2_ET_NO_CURRENT_गाठ)
				retval = ec;
		}

		जर (retval)
			जा done;
	} अन्यथा जर (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		म_लिखो("(re/un)mapping last block in extent\n");
#endif
		जर (physical) {
			जर (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((पूर्णांक) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				जर (retval)
					जा done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				जर (retval)
					जा done;
			} अन्यथा
				retval = ext2fs_extent_घाला(handle,
				      EXT2_EXTENT_घाला_AFTER, &newextent);
			जर (retval)
				जा done;
			retval = ext2fs_extent_fix_parents(handle);
			जर (retval)
				जा done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			जर (retval)
				जा done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			जर (retval)
				जा done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		जर (retval)
			जा done;
	} अन्यथा जर (logical == extent.e_lblk) {
#ifdef DEBUG
		म_लिखो("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		जर (retval)
			जा done;
		retval = ext2fs_extent_fix_parents(handle);
		जर (retval)
			जा done;
		जर (physical) {
			जर (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((पूर्णांक) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				जर (retval)
					जा done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} अन्यथा
				retval = ext2fs_extent_घाला(handle,
							      0, &newextent);
			जर (retval)
				जा done;
			retval = ext2fs_extent_fix_parents(handle);
			जर (retval)
				जा done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			जर (retval)
				जा done;
		}
	} अन्यथा {
		__u32	save_length;
		blk64_t	save_lblk;
		संरचना ext2fs_extent save_extent;
		errcode_t r2;

#ifdef DEBUG
		म_लिखो("(re/un)mapping in middle of extent\n");
#endif
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_length = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		जर (retval)
			जा done;
		/* insert our new extent, if any */
		जर (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_घाला(handle,
					EXT2_EXTENT_घाला_AFTER, &newextent);
			जर (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				जर (r2 == 0)
					(शून्य)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				जा done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_length - extent.e_len - 1;
		retval = ext2fs_extent_घाला(handle,
				EXT2_EXTENT_घाला_AFTER, &extent);
		जर (retval) {
			जर (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				जर (r2 == 0)
					(शून्य)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			जर (r2 == 0)
				(शून्य)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			जा done;
		}
	}

done:
	/* get handle back to its position */
	जर (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	परत retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, पूर्णांक flags)
{
	संरचना extent_path		*path;
	अक्षर 				*cp;
	संरचना ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	जर (!(handle->fs->flags & EXT2_FLAG_RW))
		परत EXT2_ET_RO_FILSYS;

	जर (!handle->path)
		परत EXT2_ET_NO_CURRENT_गाठ;

#ifdef DEBUG
	{
		संरचना ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		जर (retval == 0) {
			म_लिखो("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	जर (!path->curr)
		परत EXT2_ET_NO_CURRENT_गाठ;

	cp = path->curr;

	/* Sanity check before memmove() */
	जर (path->डावा < 0)
		परत EXT2_ET_EXTENT_LEAF_BAD;

	जर (path->डावा) {
		स_हटाओ(cp, cp + आकार(संरचना ext3_extent_idx),
			path->डावा * आकार(संरचना ext3_extent_idx));
		path->डावा--;
	} अन्यथा {
		संरचना ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	जर (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	जर (path->entries == 0 && handle->level) {
		जर (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			संरचना ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			जर (retval)
				परत retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} अन्यथा {
		eh = (संरचना ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		जर ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	परत retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 संरचना ext2_extent_info *info)
{
	संरचना extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	स_रखो(info, 0, आकार(संरचना ext2_extent_info));

	path = handle->path + handle->level;
	जर (path) {
		जर (path->curr)
			info->curr_entry = ((अक्षर *) path->curr - path->buf) /
				आकार(संरचना ext3_extent_idx);
		अन्यथा
			info->curr_entry = 0;
		info->अंक_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			आकार(संरचना ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	परत 0;
}

माप_प्रकार ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	माप_प्रकार iblock_sz = आकार(((संरचना ext2_inode *)रिक्त)->i_block);
	माप_प्रकार iblock_extents = (iblock_sz - आकार(संरचना ext3_extent_header)) /
				आकार(संरचना ext3_extent);
	माप_प्रकार extents_per_block = (handle->fs->blocksize -
				    आकार(संरचना ext3_extent_header)) /
				   आकार(संरचना ext3_extent);
	स्थिर अचिन्हित पूर्णांक last_blocksize = 0;
	स्थिर माप_प्रकार last_result = 0;

	जर (last_blocksize && last_blocksize == handle->fs->blocksize)
		परत last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	परत last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       संरचना ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	संरचना ext2fs_extent	extent;
	errcode_t		errcode;
	पूर्णांक    			save_flags = fs->flags;

	जर (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		परत 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	जर (errcode) {
		जर (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		परत errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_मूळ, &extent);
	जर (errcode)
		जा out;

	करा {
		/* Skip to the end of a block of leaf nodes */
		जर (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			जर (errcode)
				थांबा;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		जर (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} पर्यंत (errcode == 0);

out:
	/* Ok if we run off the end */
	जर (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	परत errcode;
}

errcode_t ext2fs_decode_extent(संरचना ext2fs_extent *to, शून्य *addr, पूर्णांक len)
{
	संरचना ext3_extent *from = (संरचना ext3_extent *)addr;

	जर (len != आकार(संरचना ext3_extent))
		परत EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	जर (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	परत 0;
}

errcode_t ext2fs_मोजणी_blocks(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode *inode, blk64_t *ret_मोजणी)
{
	ext2_extent_handle_t	handle = रिक्त;
	संरचना ext2fs_extent	extent;
	errcode_t		errcode;
	पूर्णांक			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	जर (errcode)
		जा out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_मूळ, &extent);
	जर (errcode)
		जा out;

	errcode = ext2fs_get_array(handle->max_depth, आकार(blk64_t),
				   &intermediate_nodes);
	जर (errcode)
		जा out;

	blkcount = handle->level;
	पर्यंत (!errcode) {
		जर (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			साठी (i = 0; i < handle->level; i++) {
				जर (intermediate_nodes[i] !=
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
	*ret_मोजणी = blkcount;
	ext2fs_extent_free(handle);

	परत 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
स्थिरांक अक्षर *debug_prog_name = "tst_extents";

#endif

