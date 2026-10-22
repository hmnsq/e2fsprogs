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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"
#समावेश "e2image.h"

#अघोषित DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

काष्ठा extent_path {
	अक्षर		*buf;
	पूर्णांक		entries;
	पूर्णांक		max_entries;
	पूर्णांक		बायाँ;
	पूर्णांक		visit_अंक;
	पूर्णांक		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	व्योम		*curr;
};


काष्ठा ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	काष्ठा ext2_inode	*inode;
	काष्ठा ext2_inode	inodebuf;
	पूर्णांक			type;
	पूर्णांक			level;
	पूर्णांक			max_depth;
	पूर्णांक			max_paths;
	काष्ठा extent_path	*path;
};

काष्ठा ext2_extent_path {
	errcode_t		magic;
	पूर्णांक			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#अगर_घोषित DEBUG
अटल व्योम dbg_show_header(काष्ठा ext3_extent_header *eh)
{
	म_लिखो("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

अटल व्योम dbg_show_सूचकांक(काष्ठा ext3_extent_idx *ix)
{
	म_लिखो("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

अटल व्योम dbg_show_extent(काष्ठा ext3_extent *ex)
{
	म_लिखो("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

अटल व्योम dbg_print_extent(अक्षर *desc, काष्ठा ext2fs_extent *extent)
{
	अगर (desc)
		म_लिखो("%s: ", desc);
	म_लिखो("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	अगर (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		ख_माला_दो("LEAF ", मानक_निकास);
	अगर (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		ख_माला_दो("UNINIT ", मानक_निकास);
	अगर (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		ख_माला_दो("2ND_VISIT ", मानक_निकास);
	अगर (!extent->e_flags)
		ख_माला_दो("(none)", मानक_निकास);
	ख_अक्षर_दो('\n', मानक_निकास);

}

अटल व्योम dump_path(स्थिर अक्षर *tag, काष्ठा ext2_extent_handle *handle,
		      काष्ठा extent_path *path)
{
	काष्ठा extent_path *ppp = path;
	म_लिखो("%s: level=%d\n", tag, handle->level);

	करो {
		म_लिखो("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->बायाँ, ppp->visit_अंक, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (व्योम *)ppp->buf);
		म_लिखो("  ");
		dbg_show_header((काष्ठा ext3_extent_header *)ppp->buf);
		अगर (ppp->curr) {
			म_लिखो("  ");
			dbg_show_सूचकांक(ppp->curr);
			म_लिखो("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} जबतक (ppp >= handle->path);
	ख_साफ(मानक_निकास);

	वापस;
}

#अन्यथा
#घोषणा dbg_show_header(eh) करो { } जबतक (0)
#घोषणा dbg_show_सूचकांक(ix) करो { } जबतक (0)
#घोषणा dbg_show_extent(ex) करो { } जबतक (0)
#घोषणा dbg_print_extent(desc, ex) करो { } जबतक (0)
#घोषणा dump_path(tag, handle, path) करो { } जबतक (0)
#पूर्ण_अगर

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(व्योम *ptr, पूर्णांक आकार)
{
	पूर्णांक eh_max, entry_आकार;
	काष्ठा ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	अगर (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		वापस EXT2_ET_EXTENT_HEADER_BAD;
	अगर (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		वापस EXT2_ET_EXTENT_HEADER_BAD;
	अगर (eh->eh_depth == 0)
		entry_आकार = माप(काष्ठा ext3_extent);
	अन्यथा
		entry_आकार = माप(काष्ठा ext3_extent_idx);

	eh_max = (आकार - माप(*eh)) / entry_आकार;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	अगर ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		वापस EXT2_ET_EXTENT_HEADER_BAD;

	वापस 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
व्योम ext2fs_extent_free(ext2_extent_handle_t handle)
{
	पूर्णांक			i;

	अगर (!handle)
		वापस;

	अगर (handle->path) {
		क्रम (i = 1; i < handle->max_paths; i++) {
			अगर (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	वापस ext2fs_extent_open2(fs, ino, शून्य, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    काष्ठा ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	काष्ठा ext2_extent_handle	*handle;
	errcode_t			retval;
	पूर्णांक				i;
	काष्ठा ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (!inode)
		अगर ((ino == 0) || (ino > fs->super->s_inodes_गिनती))
			वापस EXT2_ET_BAD_INODE_अंक;

	retval = ext2fs_get_mem(माप(काष्ठा ext2_extent_handle), &handle);
	अगर (retval)
		वापस retval;
	स_रखो(handle, 0, माप(काष्ठा ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	अगर (inode) {
		handle->inode = inode;
	} अन्यथा {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		अगर (retval)
			जाओ errout;
	}

	eh = (काष्ठा ext3_extent_header *) &handle->inode->i_block[0];

	क्रम (i=0; i < EXT2_N_BLOCKS; i++)
		अगर (handle->inode->i_block[i])
			अवरोध;
	अगर (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (माप(handle->inode->i_block) - माप(*eh)) /
			माप(काष्ठा ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	अगर (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		जाओ errout;
	}

	retval = ext2fs_extent_header_verify(eh, माप(handle->inode->i_block));
	अगर (retval)
		जाओ errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    माप(काष्ठा extent_path),
				    &handle->path);
	handle->path[0].buf = (अक्षर *) handle->inode->i_block;

	handle->path[0].बायाँ = handle->path[0].entries =
		ext2fs_le16_to_cpu(eh->eh_entries);
	handle->path[0].max_entries = ext2fs_le16_to_cpu(eh->eh_max);
	handle->path[0].curr = 0;
	handle->path[0].end_blk =
		(EXT2_I_आकार(handle->inode) + fs->blocksize - 1) >>
		 EXT2_BLOCK_आकार_BITS(fs->super);
	handle->path[0].blk = 0;
	handle->path[0].visit_अंक = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	वापस 0;

errout:
	ext2fs_extent_free(handle);
	वापस retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    पूर्णांक flags, काष्ठा ext2fs_extent *extent)
{
	काष्ठा extent_path	*path, *newpath, *tp;
	काष्ठा ext3_extent_header	*eh;
	काष्ठा ext3_extent_idx		*ix = 0;
	काष्ठा ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	पूर्णांक				orig_op, op, l;
	पूर्णांक				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	अगर (!handle->path)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	अगर ((orig_op == EXT2_EXTENT_अग्र) ||
	    (orig_op == EXT2_EXTENT_अग्र_LEAF)) {
		अगर (handle->level < handle->max_depth) {
			/* interior node */
			अगर (path->visit_अंक == 0) {
				path->visit_अंक++;
				op = EXT2_EXTENT_DOWN;
			} अन्यथा अगर (path->बायाँ > 0)
				op = EXT2_EXTENT_अग्र_SIB;
			अन्यथा अगर (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अन्यथा
				वापस EXT2_ET_EXTENT_NO_अग्र;
		} अन्यथा {
			/* leaf node */
			अगर (path->बायाँ > 0)
				op = EXT2_EXTENT_अग्र_SIB;
			अन्यथा अगर (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अन्यथा
				वापस EXT2_ET_EXTENT_NO_अग्र;
		}
		अगर (op != EXT2_EXTENT_अग्र_SIB) {
#अगर_घोषित DEBUG_GET_EXTENT
			म_लिखो("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#पूर्ण_अगर
		}
	}

	अगर ((orig_op == EXT2_EXTENT_पूर्व) ||
	    (orig_op == EXT2_EXTENT_पूर्व_LEAF)) {
		अगर (handle->level < handle->max_depth) {
			/* interior node */
			अगर (path->visit_अंक > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} अन्यथा अगर (path->बायाँ < path->entries-1)
				op = EXT2_EXTENT_पूर्व_SIB;
			अन्यथा अगर (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अन्यथा
				वापस EXT2_ET_EXTENT_NO_पूर्व;
		} अन्यथा {
			/* leaf node */
			अगर (path->बायाँ < path->entries-1)
				op = EXT2_EXTENT_पूर्व_SIB;
			अन्यथा अगर (handle->level > 0)
				op = EXT2_EXTENT_UP;
			अन्यथा
				वापस EXT2_ET_EXTENT_NO_पूर्व;
		}
		अगर (op != EXT2_EXTENT_पूर्व_SIB) {
#अगर_घोषित DEBUG_GET_EXTENT
			म_लिखो("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#पूर्ण_अगर
		}
	}

	अगर (orig_op == EXT2_EXTENT_LAST_LEAF) {
		अगर ((handle->level < handle->max_depth) &&
		    (path->बायाँ == 0))
			op = EXT2_EXTENT_DOWN;
		अन्यथा
			op = EXT2_EXTENT_LAST_SIB;
#अगर_घोषित DEBUG_GET_EXTENT
		म_लिखो("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#पूर्ण_अगर
	}

	चयन (op) {
	हाल EXT2_EXTENT_CURRENT:
		ix = path->curr;
		अवरोध;
	हाल EXT2_EXTENT_मूल:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	हाल EXT2_EXTENT_FIRST_SIB:
		path->बायाँ = path->entries;
		path->curr = 0;
		/* fallthrough */
	हाल EXT2_EXTENT_अग्र_SIB:
		अगर (path->बायाँ <= 0)
			वापस EXT2_ET_EXTENT_NO_अग्र;
		अगर (path->curr) {
			ix = path->curr;
			ix++;
		} अन्यथा {
			eh = (काष्ठा ext3_extent_header *) path->buf;
			ix = EXT_FIRST_सूचकांक(eh);
		}
		path->बायाँ--;
		path->curr = ix;
		path->visit_अंक = 0;
		अवरोध;
	हाल EXT2_EXTENT_पूर्व_SIB:
		अगर (!path->curr ||
		    path->बायाँ+1 >= path->entries)
			वापस EXT2_ET_EXTENT_NO_पूर्व;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->बायाँ++;
		अगर (handle->level < handle->max_depth)
			path->visit_अंक = 1;
		अवरोध;
	हाल EXT2_EXTENT_LAST_SIB:
		eh = (काष्ठा ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->बायाँ = 0;
		path->visit_अंक = 0;
		अवरोध;
	हाल EXT2_EXTENT_UP:
		अगर (handle->level <= 0)
			वापस EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		अगर ((orig_op == EXT2_EXTENT_पूर्व) ||
		    (orig_op == EXT2_EXTENT_पूर्व_LEAF))
			path->visit_अंक = 0;
		अवरोध;
	हाल EXT2_EXTENT_DOWN:
	हाल EXT2_EXTENT_DOWN_AND_LAST:
		अगर (!path->curr ||(handle->level >= handle->max_depth))
			वापस EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		अगर (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			अगर (retval)
				वापस retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		क्रम (l = handle->level, tp = path; l > 0; l--, tp--) {
			अगर (blk == tp->blk)
				वापस EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		अगर ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			स_रखो(newpath->buf, 0, handle->fs->blocksize);
		अन्यथा {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			अगर (retval)
				वापस retval;
		}
		handle->level++;

		eh = (काष्ठा ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		अगर (retval) {
			handle->level--;
			वापस retval;
		}

		अगर (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->बायाँ = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		अगर (newpath->बायाँ <= 0)
			वापस EXT2_ET_EXTENT_NO_DOWN;

		अगर (path->बायाँ > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} अन्यथा
			newpath->end_blk = path->end_blk;

		path = newpath;
		अगर (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_सूचकांक((काष्ठा ext3_extent_header *) eh);
			path->curr = ix;
			path->बायाँ = path->entries - 1;
			path->visit_अंक = 0;
		} अन्यथा {
			ix = EXT_LAST_सूचकांक((काष्ठा ext3_extent_header *) eh);
			path->curr = ix;
			path->बायाँ = 0;
			अगर (handle->level < handle->max_depth)
				path->visit_अंक = 1;
		}
#अगर_घोषित DEBUG_GET_EXTENT
		म_लिखो("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#पूर्ण_अगर
		अवरोध;
	शेष:
		वापस EXT2_ET_OP_NOT_SUPPORTED;
	}

	अगर (!ix)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

	extent->e_flags = 0;
#अगर_घोषित DEBUG_GET_EXTENT
	म_लिखो("(Left %d)\n", path->बायाँ);
#पूर्ण_अगर

	अगर (handle->level == handle->max_depth) {
		ex = (काष्ठा ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		अगर (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} अन्यथा {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		अगर (path->बायाँ > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} अन्यथा
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	अगर (path->visit_अंक)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	अगर (((orig_op == EXT2_EXTENT_अग्र_LEAF) ||
	     (orig_op == EXT2_EXTENT_पूर्व_LEAF)) &&
	    (handle->level != handle->max_depth))
		जाओ retry;

	अगर ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->बायाँ != 0)))
		जाओ retry;

	अगर (failed_csum)
		वापस EXT2_ET_EXTENT_CSUM_INVALID;

	वापस 0;
}

अटल errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	काष्ठा ext3_extent_idx		*ix;
	काष्ठा ext3_extent_header	*eh;

	अगर (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} अन्यथा {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (काष्ठा ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		अगर (retval)
			वापस retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	वापस retval;
}

#अगर 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	काष्ठा ext2fs_extent	extent;
	काष्ठा ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	अगर (retval)
		वापस retval;

	retval = ext2fs_extent_get_info(handle, &info);
	अगर (retval)
		वापस retval;

	retval = ext2fs_get_mem(माप(काष्ठा ext2_extent_path), &save_path);
	अगर (retval)
		वापस retval;
	स_रखो(save_path, 0, माप(काष्ठा ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	वापस 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	वापस 0;
}
#पूर्ण_अगर

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
	काष्ठा ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_मूल, &extent);
	अगर (retval) {
		अगर (retval == EXT2_ET_EXTENT_NO_अग्र)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		वापस retval;
	}

	अगर (leaf_level > handle->max_depth) {
#अगर_घोषित DEBUG
		म_लिखो("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#पूर्ण_अगर
		वापस EXT2_ET_OP_NOT_SUPPORTED;
	}

#अगर_घोषित DEBUG
	म_लिखो("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#पूर्ण_अगर

#अगर_घोषित DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#पूर्ण_अगर
	जबतक (1) {
		अगर (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			अगर ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				वापस 0;
			अगर (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_पूर्व_SIB,
							   &extent);
				वापस EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_अग्र_SIB,
						   &extent);
			अगर (retval == EXT2_ET_EXTENT_NO_अग्र)
				वापस EXT2_ET_EXTENT_NOT_FOUND;
			अगर (retval)
				वापस retval;
			जारी;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र_SIB,
					   &extent);
		अगर (retval == EXT2_ET_EXTENT_NO_अग्र)
			जाओ go_down;
		अगर (retval)
			वापस retval;

#अगर_घोषित DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#पूर्ण_अगर
		अगर (blk == extent.e_lblk)
			जाओ go_down;
		अगर (blk > extent.e_lblk)
			जारी;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_पूर्व_SIB,
					   &extent);
		अगर (retval)
			वापस retval;

#अगर_घोषित DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#पूर्ण_अगर

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		अगर (retval)
			वापस retval;

#अगर_घोषित DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#पूर्ण_अगर
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	वापस ext2fs_extent_goto2(handle, 0, blk);
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
	काष्ठा extent_path		*path;
	काष्ठा ext2fs_extent		extent;
	काष्ठा ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	अगर (!(handle->fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	अगर (!handle->path)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

	path = handle->path + handle->level;
	अगर (!path->curr)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	अगर (retval)
		जाओ done;

	/* modified node's start block */
	start = extent.e_lblk;

	अगर ((retval = ext2fs_extent_get_info(handle, &info)))
		वापस retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	जबतक (handle->level > 0 &&
	       (path->बायाँ == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		अगर (retval)
			जाओ done;
		अगर (extent.e_lblk == start)
			अवरोध;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		अगर (retval)
			जाओ done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	वापस retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				पूर्णांक flags EXT2FS_ATTR((unused)),
				काष्ठा ext2fs_extent *extent)
{
	काष्ठा extent_path		*path;
	काष्ठा ext3_extent_idx		*ix;
	काष्ठा ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	अगर (!(handle->fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	अगर (!handle->path)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

	path = handle->path + handle->level;
	अगर (!path->curr)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

#अगर_घोषित DEBUG
	म_लिखो("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#पूर्ण_अगर

	अगर (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		अगर (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			अगर (extent->e_len > EXT_UNINIT_MAX_LEN)
				वापस EXT2_ET_EXTENT_INVALID_लंबाई;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} अन्यथा {
			अगर (extent->e_len > EXT_INIT_MAX_LEN)
				वापस EXT2_ET_EXTENT_INVALID_लंबाई;
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
	वापस 0;
}

अटल पूर्णांक splitting_at_eof(काष्ठा ext2_extent_handle *handle,
			    काष्ठा extent_path *path)
{
	काष्ठा extent_path *ppp = path;
	dump_path(__func__, handle, path);

	अगर (handle->level == 0)
		वापस 0;

	करो {
		अगर (ppp->बायाँ)
			वापस 0;
		ppp--;
	} जबतक (ppp >= handle->path);

	वापस 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
अटल errcode_t extent_गाँठ_split(ext2_extent_handle_t handle,
				   पूर्णांक expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_गाँठ_pblk;
	blk64_t				new_गाँठ_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	पूर्णांक				orig_height;
	अक्षर				*block_buf = शून्य;
	काष्ठा ext2fs_extent		extent;
	काष्ठा extent_path		*path, *newpath = 0;
	काष्ठा ext3_extent_header	*eh, *neweh;
	पूर्णांक				tocopy;
	पूर्णांक				new_मूल = 0;
	काष्ठा ext2_extent_info		info;
	पूर्णांक				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	अगर (!(handle->fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	अगर (!handle->path)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

#अगर_घोषित DEBUG
	म_लिखो("splitting node at level %d\n", handle->level);
#पूर्ण_अगर
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	अगर (retval)
		जाओ done;

	retval = ext2fs_extent_get_info(handle, &info);
	अगर (retval)
		जाओ done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (काष्ठा ext3_extent_header *) path->buf;
	अगर (handle->level == handle->max_depth) {
		काष्ठा ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} अन्यथा {
		काष्ठा ext3_extent_idx	*ix;

		ix = EXT_FIRST_सूचकांक(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	अगर (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#अगर_घोषित DEBUG
		म_लिखो("parent level %d full; splitting it too\n",
							handle->level - 1);
#पूर्ण_अगर
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		अगर (retval)
			जाओ done;

		retval = extent_गाँठ_split(handle, expand_allowed);
		अगर (retval)
			जाओ done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		अगर (retval)
			जाओ done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	अगर (!path->curr)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (काष्ठा ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	अगर (handle->level == 0) {
		new_मूल = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    माप(काष्ठा extent_path),
					    &newpath);
		अगर (retval)
			जाओ done;
	} अन्यथा {
		अगर (no_balance)
			tocopy = 1;
		अन्यथा
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#अगर_घोषित DEBUG
	म_लिखो("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#पूर्ण_अगर

	अगर (!tocopy && !no_balance) {
#अगर_घोषित DEBUG
		म_लिखो("Nothing to copy to new block!\n");
#पूर्ण_अगर
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		जाओ done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = दो_स्मृति(handle->fs->blocksize);
	अगर (!block_buf) {
		retval = ENOMEM;
		जाओ done;
	}

	अगर (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_गाँठ_pblk);
	अगर (retval)
		जाओ done;

#अगर_घोषित DEBUG
	म_लिखो("will copy to new node at block %lu\n",
	       (अचिन्हित दीर्घ) new_गाँठ_pblk);
#पूर्ण_अगर

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (काष्ठा ext3_extent_header *) block_buf;
	स_नकल(neweh, eh, माप(काष्ठा ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 माप(काष्ठा ext3_extent_header)) /
				माप(काष्ठा ext3_extent));

	/* then the entries for the new block... */
	स_नकल(EXT_FIRST_सूचकांक(neweh),
		EXT_FIRST_सूचकांक(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		माप(काष्ठा ext3_extent_idx) * tocopy);

	new_गाँठ_start = ext2fs_le32_to_cpu(EXT_FIRST_सूचकांक(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	अगर (retval)
		जाओ done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_गाँठ_pblk, 1,
					block_buf);

	अगर (retval)
		जाओ done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	अगर (handle->level == 0) {
		स_नकल(newpath, path,
		       माप(काष्ठा extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->बायाँ = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} अन्यथा {
		path->entries -= tocopy;
		path->बायाँ -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	अगर (retval)
		जाओ done;

	/* now go up and insert/replace index for new node we created */
	अगर (new_मूल) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		अगर (retval)
			जाओ done;

		extent.e_lblk = new_गाँठ_start;
		extent.e_pblk = new_गाँठ_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		अगर (retval)
			जाओ done;
	} अन्यथा {
		__u32 new_गाँठ_लंबाई;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_गाँठ_लंबाई = new_गाँठ_start - extent.e_lblk;
		extent.e_len -= new_गाँठ_लंबाई;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		अगर (retval)
			जाओ done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_गाँठ_start;
		extent.e_pblk = new_गाँठ_pblk;
		extent.e_len = new_गाँठ_लंबाई;
		retval = ext2fs_extent_जोड़ो(handle, EXT2_EXTENT_जोड़ो_AFTER, &extent);
		अगर (retval)
			जाओ done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	अगर (retval)
		जाओ done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	अगर (retval)
		जाओ done;

done:
	अगर (newpath)
		ext2fs_free_mem(&newpath);
	मुक्त(block_buf);

	वापस retval;
}

errcode_t ext2fs_extent_गाँठ_split(ext2_extent_handle_t handle)
{
	वापस extent_गाँठ_split(handle, 0);
}

errcode_t ext2fs_extent_जोड़ो(ext2_extent_handle_t handle, पूर्णांक flags,
				      काष्ठा ext2fs_extent *extent)
{
	काष्ठा extent_path		*path;
	काष्ठा ext3_extent_idx		*ix;
	काष्ठा ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	अगर (!(handle->fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	अगर (!handle->path)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

#अगर_घोषित DEBUG
	म_लिखो("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#पूर्ण_अगर

	path = handle->path + handle->level;

	अगर (path->entries >= path->max_entries) {
		अगर (flags & EXT2_EXTENT_जोड़ो_NOSPLIT) {
			वापस EXT2_ET_CANT_जोड़ो_EXTENT;
		} अन्यथा {
#अगर_घोषित DEBUG
			म_लिखो("node full (level %d) - splitting\n",
				   handle->level);
#पूर्ण_अगर
			retval = extent_गाँठ_split(handle, 1);
			अगर (retval)
				वापस retval;
			path = handle->path + handle->level;
		}
	}

	eh = (काष्ठा ext3_extent_header *) path->buf;
	अगर (path->curr) {
		ix = path->curr;
		अगर (flags & EXT2_EXTENT_जोड़ो_AFTER) {
			ix++;
			path->बायाँ--;
		}
	} अन्यथा {
		ix = EXT_FIRST_सूचकांक(eh);
		path->बायाँ = -1;
	}

	path->curr = ix;

	अगर (path->बायाँ >= 0)
		स_हटाओ(ix + 1, ix,
			(path->बायाँ+1) * माप(काष्ठा ext3_extent_idx));
	path->बायाँ++;
	path->entries++;

	eh = (काष्ठा ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	अगर (retval)
		जाओ errout;

	retval = update_path(handle);
	अगर (retval)
		जाओ errout;

	वापस 0;

errout:
	ext2fs_extent_delete(handle, 0);
	वापस retval;
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
	पूर्णांक			पूर्व_uninit = 0;
	पूर्णांक			अग्र_uninit = 0;
	पूर्णांक			new_uninit = 0;
	पूर्णांक			max_len = EXT_INIT_MAX_LEN;
	पूर्णांक			has_पूर्व, has_अग्र;
	blk64_t			orig_lblk;
	काष्ठा extent_path	*path;
	काष्ठा ext2fs_extent	extent, अग्र_extent, पूर्व_extent;
	काष्ठा ext2fs_extent	newextent;
	काष्ठा ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#अगर_घोषित DEBUG
	म_लिखो("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#पूर्ण_अगर

	अगर (!(handle->fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	अगर (!handle->path)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

	path = handle->path + handle->level;

	अगर (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	अगर (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		अगर (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	अगर ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_जोड़ो(handle, 0, &newextent);
		वापस retval;
	}

	/* save our original location in the extent tree */
	अगर ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		अगर (retval != EXT2_ET_NO_CURRENT_गाँठ)
			वापस retval;
		स_रखो(&extent, 0, माप(extent));
	}
	अगर ((retval = ext2fs_extent_get_info(handle, &info)))
		वापस retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	अगर (retval) {
		अगर (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			अगर (!physical) {
#अगर_घोषित DEBUG
				म_लिखो("block %llu already unmapped\n",
					logical);
#पूर्ण_अगर
				जाओ done;
			}
		} अन्यथा
			जाओ done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	अगर (retval)
		जाओ done;
	अगर (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र_LEAF, &अग्र_extent);
	अगर (retval) {
		has_अग्र = 0;
		अगर (retval != EXT2_ET_EXTENT_NO_अग्र)
			जाओ done;
	} अन्यथा {
		dbg_print_extent("set_bmap: next_extent",
				 &अग्र_extent);
		has_अग्र = 1;
		अगर (अग्र_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			अग्र_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	अगर (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		जाओ done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_पूर्व_LEAF, &पूर्व_extent);
	अगर (retval) {
		has_पूर्व = 0;
		अगर (retval != EXT2_ET_EXTENT_NO_पूर्व)
			जाओ done;
	} अन्यथा {
		has_पूर्व = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &पूर्व_extent);
		अगर (पूर्व_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			पूर्व_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	अगर (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		जाओ done;

	/* check if already pointing to the requested physical */
	अगर (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#अगर_घोषित DEBUG
		म_लिखो("physical block (at %llu) unchanged\n", logical);
#पूर्ण_अगर
		जाओ done;
	}

	अगर (!mapped) {
#अगर_घोषित DEBUG
		म_लिखो("mapping unmapped logical block %llu\n", logical);
#पूर्ण_अगर
		अगर ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((पूर्णांक) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} अन्यथा अगर ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((पूर्णांक) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} अन्यथा अगर (has_अग्र &&
			   (logical == अग्र_extent.e_lblk - 1) &&
			   (physical == अग्र_extent.e_pblk - 1) &&
			   (new_uninit == अग्र_uninit) &&
			   ((पूर्णांक) अग्र_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_अग्र_LEAF,
						   &अग्र_extent);
			अगर (retval)
				जाओ done;
			अग्र_extent.e_len++;
			अग्र_extent.e_lblk--;
			अग्र_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &अग्र_extent);
		} अन्यथा अगर (logical < extent.e_lblk)
			retval = ext2fs_extent_जोड़ो(handle, 0, &newextent);
		अन्यथा
			retval = ext2fs_extent_जोड़ो(handle,
				      EXT2_EXTENT_जोड़ो_AFTER, &newextent);
		अगर (retval)
			जाओ done;
		retval = ext2fs_extent_fix_parents(handle);
		अगर (retval)
			जाओ done;
	} अन्यथा अगर ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#अगर_घोषित DEBUG
		म_लिखो("(re/un)mapping only block in extent\n");
#पूर्ण_अगर
		अगर (physical) {
			अगर (has_पूर्व &&
			    (logical == (पूर्व_extent.e_lblk +
					 पूर्व_extent.e_len)) &&
			    (physical == (पूर्व_extent.e_pblk +
					  पूर्व_extent.e_len)) &&
			    (new_uninit == पूर्व_uninit) &&
			    ((पूर्णांक) पूर्व_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_पूर्व_LEAF, &पूर्व_extent);
				अगर (retval)
					जाओ done;
				पूर्व_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &पूर्व_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_अग्र_LEAF,
							   &extent);
				अगर (retval)
					जाओ done;
				जाओ delete_गाँठ;

			} अन्यथा
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} अन्यथा {
		delete_गाँठ:
			retval = ext2fs_extent_delete(handle, 0);
			अगर (retval)
				जाओ done;
			ec = ext2fs_extent_fix_parents(handle);
			अगर (ec != EXT2_ET_NO_CURRENT_गाँठ)
				retval = ec;
		}

		अगर (retval)
			जाओ done;
	} अन्यथा अगर (logical == extent.e_lblk + extent.e_len - 1)  {
#अगर_घोषित DEBUG
		म_लिखो("(re/un)mapping last block in extent\n");
#पूर्ण_अगर
		अगर (physical) {
			अगर (has_अग्र &&
			    (logical == (अग्र_extent.e_lblk - 1)) &&
			    (physical == (अग्र_extent.e_pblk - 1)) &&
			    (new_uninit == अग्र_uninit) &&
			    ((पूर्णांक) अग्र_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_अग्र_LEAF, &अग्र_extent);
				अगर (retval)
					जाओ done;
				अग्र_extent.e_len++;
				अग्र_extent.e_lblk--;
				अग्र_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &अग्र_extent);
				अगर (retval)
					जाओ done;
			} अन्यथा
				retval = ext2fs_extent_जोड़ो(handle,
				      EXT2_EXTENT_जोड़ो_AFTER, &newextent);
			अगर (retval)
				जाओ done;
			retval = ext2fs_extent_fix_parents(handle);
			अगर (retval)
				जाओ done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			अगर (retval)
				जाओ done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			अगर (retval)
				जाओ done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		अगर (retval)
			जाओ done;
	} अन्यथा अगर (logical == extent.e_lblk) {
#अगर_घोषित DEBUG
		म_लिखो("(re/un)mapping first block in extent\n");
#पूर्ण_अगर
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		अगर (retval)
			जाओ done;
		retval = ext2fs_extent_fix_parents(handle);
		अगर (retval)
			जाओ done;
		अगर (physical) {
			अगर (has_पूर्व &&
			    (logical == (पूर्व_extent.e_lblk +
					 पूर्व_extent.e_len)) &&
			    (physical == (पूर्व_extent.e_pblk +
					  पूर्व_extent.e_len)) &&
			    (new_uninit == पूर्व_uninit) &&
			    ((पूर्णांक) पूर्व_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_पूर्व_LEAF, &पूर्व_extent);
				अगर (retval)
					जाओ done;
				पूर्व_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &पूर्व_extent);
			} अन्यथा
				retval = ext2fs_extent_जोड़ो(handle,
							      0, &newextent);
			अगर (retval)
				जाओ done;
			retval = ext2fs_extent_fix_parents(handle);
			अगर (retval)
				जाओ done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_अग्र_LEAF,
						   &extent);
			अगर (retval)
				जाओ done;
		}
	} अन्यथा {
		__u32	save_लंबाई;
		blk64_t	save_lblk;
		काष्ठा ext2fs_extent save_extent;
		errcode_t r2;

#अगर_घोषित DEBUG
		म_लिखो("(re/un)mapping in middle of extent\n");
#पूर्ण_अगर
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_लंबाई = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		अगर (retval)
			जाओ done;
		/* insert our new extent, if any */
		अगर (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_जोड़ो(handle,
					EXT2_EXTENT_जोड़ो_AFTER, &newextent);
			अगर (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				अगर (r2 == 0)
					(व्योम)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				जाओ done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_लंबाई - extent.e_len - 1;
		retval = ext2fs_extent_जोड़ो(handle,
				EXT2_EXTENT_जोड़ो_AFTER, &extent);
		अगर (retval) {
			अगर (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				अगर (r2 == 0)
					(व्योम)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			अगर (r2 == 0)
				(व्योम)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			जाओ done;
		}
	}

done:
	/* get handle back to its position */
	अगर (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	वापस retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, पूर्णांक flags)
{
	काष्ठा extent_path		*path;
	अक्षर 				*cp;
	काष्ठा ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	अगर (!(handle->fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	अगर (!handle->path)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

#अगर_घोषित DEBUG
	{
		काष्ठा ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		अगर (retval == 0) {
			म_लिखो("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#पूर्ण_अगर

	path = handle->path + handle->level;
	अगर (!path->curr)
		वापस EXT2_ET_NO_CURRENT_गाँठ;

	cp = path->curr;

	/* Sanity check before memmove() */
	अगर (path->बायाँ < 0)
		वापस EXT2_ET_EXTENT_LEAF_BAD;

	अगर (path->बायाँ) {
		स_हटाओ(cp, cp + माप(काष्ठा ext3_extent_idx),
			path->बायाँ * माप(काष्ठा ext3_extent_idx));
		path->बायाँ--;
	} अन्यथा {
		काष्ठा ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	अगर (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	अगर (path->entries == 0 && handle->level) {
		अगर (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			काष्ठा ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			अगर (retval)
				वापस retval;

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
		eh = (काष्ठा ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		अगर ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	वापस retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 काष्ठा ext2_extent_info *info)
{
	काष्ठा extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	स_रखो(info, 0, माप(काष्ठा ext2_extent_info));

	path = handle->path + handle->level;
	अगर (path) {
		अगर (path->curr)
			info->curr_entry = ((अक्षर *) path->curr - path->buf) /
				माप(काष्ठा ext3_extent_idx);
		अन्यथा
			info->curr_entry = 0;
		info->अंक_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			माप(काष्ठा ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	वापस 0;
}

माप_प्रकार ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	माप_प्रकार iblock_sz = माप(((काष्ठा ext2_inode *)शून्य)->i_block);
	माप_प्रकार iblock_extents = (iblock_sz - माप(काष्ठा ext3_extent_header)) /
				माप(काष्ठा ext3_extent);
	माप_प्रकार extents_per_block = (handle->fs->blocksize -
				    माप(काष्ठा ext3_extent_header)) /
				   माप(काष्ठा ext3_extent);
	अटल अचिन्हित पूर्णांक last_blocksize = 0;
	अटल माप_प्रकार last_परिणाम = 0;

	अगर (last_blocksize && last_blocksize == handle->fs->blocksize)
		वापस last_परिणाम;

	last_परिणाम = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	वापस last_परिणाम;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       काष्ठा ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	काष्ठा ext2fs_extent	extent;
	errcode_t		errcode;
	पूर्णांक    			save_flags = fs->flags;

	अगर (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		वापस 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	अगर (errcode) {
		अगर (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		वापस errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_मूल, &extent);
	अगर (errcode)
		जाओ out;

	करो {
		/* Skip to the end of a block of leaf nodes */
		अगर (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			अगर (errcode)
				अवरोध;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र, &extent);
		अगर (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} जबतक (errcode == 0);

out:
	/* Ok if we run off the end */
	अगर (errcode == EXT2_ET_EXTENT_NO_अग्र)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	वापस errcode;
}

errcode_t ext2fs_decode_extent(काष्ठा ext2fs_extent *to, व्योम *addr, पूर्णांक len)
{
	काष्ठा ext3_extent *from = (काष्ठा ext3_extent *)addr;

	अगर (len != माप(काष्ठा ext3_extent))
		वापस EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	अगर (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	वापस 0;
}

errcode_t ext2fs_गिनती_blocks(ext2_filsys fs, ext2_ino_t ino,
			      काष्ठा ext2_inode *inode, blk64_t *ret_गिनती)
{
	ext2_extent_handle_t	handle = शून्य;
	काष्ठा ext2fs_extent	extent;
	errcode_t		errcode;
	पूर्णांक			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	अगर (errcode)
		जाओ out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_मूल, &extent);
	अगर (errcode)
		जाओ out;

	errcode = ext2fs_get_array(handle->max_depth, माप(blk64_t),
				   &intermediate_nodes);
	अगर (errcode)
		जाओ out;

	blkcount = handle->level;
	जबतक (!errcode) {
		अगर (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			क्रम (i = 0; i < handle->level; i++) {
				अगर (intermediate_nodes[i] !=
					handle->path[i].end_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->path[i].end_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
out:
	*ret_गिनती = blkcount;
	ext2fs_extent_free(handle);

	वापस 0;
}

#अगर_घोषित DEBUG
/*
 * Override debugfs's prompt
 */
स्थिर अक्षर *debug_prog_name = "tst_extents";

#पूर्ण_अगर

