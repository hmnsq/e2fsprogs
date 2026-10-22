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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"
#ଅନ୍ତର୍ଭାଵ "e2image.h"

#undef DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

ସଂରଚନା extent_path {
	ଅକ୍ଷର		*buf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		entries;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		max_entries;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ବାମ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		visit_ଅଙ୍କ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		flags;
	blk64_t		end_blk;
	blk64_t		blk;
	ଶୂନ୍ୟ		*curr;
};


ସଂରଚନା ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	ସଂରଚନା ext2_inode	*inode;
	ସଂରଚନା ext2_inode	inodebuf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			type;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			level;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			max_depth;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			max_paths;
	ସଂରଚନା extent_path	*path;
};

ସଂରଚନା ext2_extent_path {
	errcode_t		magic;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			leaf_height;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ifdef DEBUG
ସ୍ଥିର ଶୂନ୍ୟ dbg_show_header(ସଂରଚନା ext3_extent_header *eh)
{
	printf("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_to_cpu(eh->eh_magic),
			ext2fs_le16_to_cpu(eh->eh_entries),
			ext2fs_le16_to_cpu(eh->eh_max),
			ext2fs_le16_to_cpu(eh->eh_depth),
			ext2fs_le32_to_cpu(eh->eh_generation));
}

ସ୍ଥିର ଶୂନ୍ୟ dbg_show_index(ସଂରଚନା ext3_extent_idx *ix)
{
	printf("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_to_cpu(ix->ei_block),
			ext2fs_le32_to_cpu(ix->ei_leaf),
			ext2fs_le16_to_cpu(ix->ei_leaf_hi),
			ext2fs_le16_to_cpu(ix->ei_unused));
}

ସ୍ଥିର ଶୂନ୍ୟ dbg_show_extent(ସଂରଚନା ext3_extent *ex)
{
	printf("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_to_cpu(ex->ee_block),
			ext2fs_le32_to_cpu(ex->ee_block) +
			ext2fs_le16_to_cpu(ex->ee_len) - 1,
			ext2fs_le16_to_cpu(ex->ee_len),
			ext2fs_le32_to_cpu(ex->ee_start),
			ext2fs_le16_to_cpu(ex->ee_start_hi));
}

ସ୍ଥିର ଶୂନ୍ୟ dbg_print_extent(ଅକ୍ଷର *desc, ସଂରଚନା ext2fs_extent *extent)
{
	ଯଦି (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ଯଦି (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	ଯଦି (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ଯଦି (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	ଯଦି (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

ସ୍ଥିର ଶୂନ୍ୟ dump_path(ଅଚଳ ଅକ୍ଷର *tag, ସଂରଚନା ext2_extent_handle *handle,
		      ସଂରଚନା extent_path *path)
{
	ସଂରଚନା extent_path *ppp = path;
	printf("%s: level=%d\n", tag, handle->level);

	କର {
		printf("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       tag, (ppp - handle->path), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->ବାମ, ppp->visit_ଅଙ୍କ, ppp->flags,
		       ppp->end_blk, ppp->curr, ppp->curr - (ଶୂନ୍ୟ *)ppp->buf);
		printf("  ");
		dbg_show_header((ସଂରଚନା ext3_extent_header *)ppp->buf);
		ଯଦି (ppp->curr) {
			printf("  ");
			dbg_show_index(ppp->curr);
			printf("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} ଯେତେବେଳେ (ppp >= handle->path);
	fflush(stdout);

	ଫେରନ୍ତୁ;
}

#ଅନ୍ୟଥା
#define dbg_show_header(eh) କର { } ଯେତେବେଳେ (0)
#define dbg_show_index(ix) କର { } ଯେତେବେଳେ (0)
#define dbg_show_extent(ex) କର { } ଯେତେବେଳେ (0)
#define dbg_print_extent(desc, ex) କର { } ଯେତେବେଳେ (0)
#define dump_path(tag, handle, path) କର { } ଯେତେବେଳେ (0)
#endif

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_header_verify(ଶୂନ୍ୟ *ptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା eh_max, entry_size;
	ସଂରଚନା ext3_extent_header *eh = ptr;

	dbg_show_header(eh);
	ଯଦି (ext2fs_le16_to_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		ଫେରନ୍ତୁ EXT2_ET_EXTENT_HEADER_BAD;
	ଯଦି (ext2fs_le16_to_cpu(eh->eh_entries) > ext2fs_le16_to_cpu(eh->eh_max))
		ଫେରନ୍ତୁ EXT2_ET_EXTENT_HEADER_BAD;
	ଯଦି (eh->eh_depth == 0)
		entry_size = ଆକାର(ସଂରଚନା ext3_extent);
	ଅନ୍ୟଥା
		entry_size = ଆକାର(ସଂରଚନା ext3_extent_idx);

	eh_max = (size - ଆକାର(*eh)) / entry_size;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	ଯଦି ((ext2fs_le16_to_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_to_cpu(eh->eh_max) < (eh_max - 2)))
		ଫେରନ୍ତୁ EXT2_ET_EXTENT_HEADER_BAD;

	ଫେରନ୍ତୁ 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
ଶୂନ୍ୟ ext2fs_extent_free(ext2_extent_handle_t handle)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			i;

	ଯଦି (!handle)
		ଫେରନ୍ତୁ;

	ଯଦି (handle->path) {
		ପାଇଁ (i = 1; i < handle->max_paths; i++) {
			ଯଦି (handle->path[i].buf)
				ext2fs_free_mem(&handle->path[i].buf);
		}
		ext2fs_free_mem(&handle->path);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	ଫେରନ୍ତୁ ext2fs_extent_open2(fs, ino, ଖାଲି, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    ସଂରଚନା ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	ସଂରଚନା ext2_extent_handle	*handle;
	errcode_t			retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				i;
	ସଂରଚନା ext3_extent_header	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (!inode)
		ଯଦି ((ino == 0) || (ino > fs->super->s_inodes_ଗଣନା))
			ଫେରନ୍ତୁ EXT2_ET_BAD_INODE_ଅଙ୍କ;

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_extent_handle), &handle);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	memset(handle, 0, ଆକାର(ସଂରଚନା ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	ଯଦି (inode) {
		handle->inode = inode;
	} ଅନ୍ୟଥା {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		ଯଦି (retval)
			ଯାଅ errout;
	}

	eh = (ସଂରଚନା ext3_extent_header *) &handle->inode->i_block[0];

	ପାଇଁ (i=0; i < EXT2_N_BLOCKS; i++)
		ଯଦି (handle->inode->i_block[i])
			ଭାଙ୍ଗ;
	ଯଦି (i >= EXT2_N_BLOCKS) {
		eh->eh_magic = ext2fs_cpu_to_le16(EXT3_EXT_MAGIC);
		eh->eh_depth = 0;
		eh->eh_entries = 0;
		i = (ଆକାର(handle->inode->i_block) - ଆକାର(*eh)) /
			ଆକାର(ସଂରଚନା ext3_extent);
		eh->eh_max = ext2fs_cpu_to_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	ଯଦି (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		ଯାଅ errout;
	}

	retval = ext2fs_extent_header_verify(eh, ଆକାର(handle->inode->i_block));
	ଯଦି (retval)
		ଯାଅ errout;

	handle->max_depth = ext2fs_le16_to_cpu(eh->eh_depth);
	handle->type = ext2fs_le16_to_cpu(eh->eh_magic);

	handle->max_paths = handle->max_depth + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    ଆକାର(ସଂରଚନା extent_path),
				    &handle->path);
	handle->path[0].buf = (ଅକ୍ଷର *) handle->inode->i_block;

	handle->path[0].ବାମ = handle->path[0].entries =
		ext2fs_le16_to_cpu(eh->eh_entries);
	handle->path[0].max_entries = ext2fs_le16_to_cpu(eh->eh_max);
	handle->path[0].curr = 0;
	handle->path[0].end_blk =
		(EXT2_I_SIZE(handle->inode) + fs->blocksize - 1) >>
		 EXT2_BLOCK_SIZE_BITS(fs->super);
	handle->path[0].blk = 0;
	handle->path[0].visit_ଅଙ୍କ = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	ଫେରନ୍ତୁ 0;

errout:
	ext2fs_extent_free(handle);
	ଫେରନ୍ତୁ retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ସଂରଚନା ext2fs_extent *extent)
{
	ସଂରଚନା extent_path	*path, *newpath, *tp;
	ସଂରଚନା ext3_extent_header	*eh;
	ସଂରଚନା ext3_extent_idx		*ix = 0;
	ସଂରଚନା ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				end_blk;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				orig_op, op, l;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ଯଦି (!handle->path)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

	orig_op = op = flags & EXT2_EXTENT_MOVE_MASK;

retry:
	path = handle->path + handle->level;
	ଯଦି ((orig_op == EXT2_EXTENT_NEXT) ||
	    (orig_op == EXT2_EXTENT_NEXT_LEAF)) {
		ଯଦି (handle->level < handle->max_depth) {
			/* interior node */
			ଯଦି (path->visit_ଅଙ୍କ == 0) {
				path->visit_ଅଙ୍କ++;
				op = EXT2_EXTENT_DOWN;
			} ଅନ୍ୟଥା ଯଦି (path->ବାମ > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			ଅନ୍ୟଥା ଯଦି (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ଅନ୍ୟଥା
				ଫେରନ୍ତୁ EXT2_ET_EXTENT_NO_NEXT;
		} ଅନ୍ୟଥା {
			/* leaf node */
			ଯଦି (path->ବାମ > 0)
				op = EXT2_EXTENT_NEXT_SIB;
			ଅନ୍ୟଥା ଯଦି (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ଅନ୍ୟଥା
				ଫେରନ୍ତୁ EXT2_ET_EXTENT_NO_NEXT;
		}
		ଯଦି (op != EXT2_EXTENT_NEXT_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN) ? "down" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	ଯଦି ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_LEAF)) {
		ଯଦି (handle->level < handle->max_depth) {
			/* interior node */
			ଯଦି (path->visit_ଅଙ୍କ > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_DOWN_AND_LAST;
			} ଅନ୍ୟଥା ଯଦି (path->ବାମ < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			ଅନ୍ୟଥା ଯଦି (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ଅନ୍ୟଥା
				ଫେରନ୍ତୁ EXT2_ET_EXTENT_NO_PREV;
		} ଅନ୍ୟଥା {
			/* leaf node */
			ଯଦି (path->ବାମ < path->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			ଅନ୍ୟଥା ଯଦି (handle->level > 0)
				op = EXT2_EXTENT_UP;
			ଅନ୍ୟଥା
				ଫେରନ୍ତୁ EXT2_ET_EXTENT_NO_PREV;
		}
		ଯଦି (op != EXT2_EXTENT_PREV_SIB) {
#ifdef DEBUG_GET_EXTENT
			printf("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_DOWN_AND_LAST) ? "down/last" :
			       ((op == EXT2_EXTENT_UP) ? "up" : "unknown"));
#endif
		}
	}

	ଯଦି (orig_op == EXT2_EXTENT_LAST_LEAF) {
		ଯଦି ((handle->level < handle->max_depth) &&
		    (path->ବାମ == 0))
			op = EXT2_EXTENT_DOWN;
		ଅନ୍ୟଥା
			op = EXT2_EXTENT_LAST_SIB;
#ifdef DEBUG_GET_EXTENT
		printf("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_DOWN) ? "down" : "last_sib");
#endif
	}

	ବାଛ (op) {
	ସ୍ଥିତି EXT2_EXTENT_CURRENT:
		ix = path->curr;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି EXT2_EXTENT_ମୂଳ:
		handle->level = 0;
		path = handle->path + handle->level;
		/* fallthrough */
	ସ୍ଥିତି EXT2_EXTENT_FIRST_SIB:
		path->ବାମ = path->entries;
		path->curr = 0;
		/* fallthrough */
	ସ୍ଥିତି EXT2_EXTENT_NEXT_SIB:
		ଯଦି (path->ବାମ <= 0)
			ଫେରନ୍ତୁ EXT2_ET_EXTENT_NO_NEXT;
		ଯଦି (path->curr) {
			ix = path->curr;
			ix++;
		} ଅନ୍ୟଥା {
			eh = (ସଂରଚନା ext3_extent_header *) path->buf;
			ix = EXT_FIRST_INDEX(eh);
		}
		path->ବାମ--;
		path->curr = ix;
		path->visit_ଅଙ୍କ = 0;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି EXT2_EXTENT_PREV_SIB:
		ଯଦି (!path->curr ||
		    path->ବାମ+1 >= path->entries)
			ଫେରନ୍ତୁ EXT2_ET_EXTENT_NO_PREV;
		ix = path->curr;
		ix--;
		path->curr = ix;
		path->ବାମ++;
		ଯଦି (handle->level < handle->max_depth)
			path->visit_ଅଙ୍କ = 1;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି EXT2_EXTENT_LAST_SIB:
		eh = (ସଂରଚନା ext3_extent_header *) path->buf;
		path->curr = EXT_LAST_EXTENT(eh);
		ix = path->curr;
		path->ବାମ = 0;
		path->visit_ଅଙ୍କ = 0;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି EXT2_EXTENT_UP:
		ଯଦି (handle->level <= 0)
			ଫେରନ୍ତୁ EXT2_ET_EXTENT_NO_UP;
		handle->level--;
		path--;
		ix = path->curr;
		ଯଦି ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_LEAF))
			path->visit_ଅଙ୍କ = 0;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି EXT2_EXTENT_DOWN:
	ସ୍ଥିତି EXT2_EXTENT_DOWN_AND_LAST:
		ଯଦି (!path->curr ||(handle->level >= handle->max_depth))
			ଫେରନ୍ତୁ EXT2_ET_EXTENT_NO_DOWN;

		ix = path->curr;
		newpath = path + 1;
		ଯଦି (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
		}
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		ପାଇଁ (l = handle->level, tp = path; l > 0; l--, tp--) {
			ଯଦି (blk == tp->blk)
				ଫେରନ୍ତୁ EXT2_ET_EXTENT_CYCLE;
		}
		newpath->blk = blk;
		ଯଦି ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		ଅନ୍ୟଥା {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
		}
		handle->level++;

		eh = (ସଂରଚନା ext3_extent_header *) newpath->buf;

		retval = ext2fs_extent_header_verify(eh, handle->fs->blocksize);
		ଯଦି (retval) {
			handle->level--;
			ଫେରନ୍ତୁ retval;
		}

		ଯଦି (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_block_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->ବାମ = newpath->entries =
			ext2fs_le16_to_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_to_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		ଯଦି (newpath->ବାମ <= 0)
			ଫେରନ୍ତୁ EXT2_ET_EXTENT_NO_DOWN;

		ଯଦି (path->ବାମ > 0) {
			ix++;
			newpath->end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} ଅନ୍ୟଥା
			newpath->end_blk = path->end_blk;

		path = newpath;
		ଯଦି (op == EXT2_EXTENT_DOWN) {
			ix = EXT_FIRST_INDEX((ସଂରଚନା ext3_extent_header *) eh);
			path->curr = ix;
			path->ବାମ = path->entries - 1;
			path->visit_ଅଙ୍କ = 0;
		} ଅନ୍ୟଥା {
			ix = EXT_LAST_INDEX((ସଂରଚନା ext3_extent_header *) eh);
			path->curr = ix;
			path->ବାମ = 0;
			ଯଦି (handle->level < handle->max_depth)
				path->visit_ଅଙ୍କ = 1;
		}
#ifdef DEBUG_GET_EXTENT
		printf("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_depth,
			   path->end_blk);
#endif
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
	}

	ଯଦି (!ix)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

	extent->e_flags = 0;
#ifdef DEBUG_GET_EXTENT
	printf("(Left %d)\n", path->ବାମ);
#endif

	ଯଦି (handle->level == handle->max_depth) {
		ex = (ସଂରଚନା ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ex->ee_block);
		extent->e_len = ext2fs_le16_to_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_LEAF;
		ଯଦି (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} ଅନ୍ୟଥା {
		extent->e_pblk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
		extent->e_lblk = ext2fs_le32_to_cpu(ix->ei_block);
		ଯଦି (path->ବାମ > 0) {
			ix++;
			end_blk = ext2fs_le32_to_cpu(ix->ei_block);
		} ଅନ୍ୟଥା
			end_blk = path->end_blk;

		extent->e_len = end_blk - extent->e_lblk;
	}
	ଯଦି (path->visit_ଅଙ୍କ)
		extent->e_flags |= EXT2_EXTENT_FLAGS_SECOND_VISIT;

	ଯଦି (((orig_op == EXT2_EXTENT_NEXT_LEAF) ||
	     (orig_op == EXT2_EXTENT_PREV_LEAF)) &&
	    (handle->level != handle->max_depth))
		ଯାଅ retry;

	ଯଦି ((orig_op == EXT2_EXTENT_LAST_LEAF) &&
	    ((handle->level != handle->max_depth) ||
	     (path->ବାମ != 0)))
		ଯାଅ retry;

	ଯଦି (failed_csum)
		ଫେରନ୍ତୁ EXT2_ET_EXTENT_CSUM_INVALID;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t update_path(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	ସଂରଚନା ext3_extent_idx		*ix;
	ସଂରଚନା ext3_extent_header	*eh;

	ଯଦି (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} ଅନ୍ୟଥା {
		ix = handle->path[handle->level - 1].curr;
		blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);

		/* then update the checksum */
		eh = (ସଂରଚନା ext3_extent_header *)
				handle->path[handle->level].buf;
		retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino,
						      eh);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->path[handle->level].buf);
	}
	ଫେରନ୍ତୁ retval;
}

#ଯଦି 0
errcode_t ext2fs_extent_save_path(ext2_extent_handle_t handle,
				  ext2_extent_path_t *ret_path)
{
	ext2_extent_path_t	save_path;
	ସଂରଚନା ext2fs_extent	extent;
	ସଂରଚନା ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = ext2fs_extent_get_info(handle, &info);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_extent_path), &save_path);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	memset(save_path, 0, ଆକାର(ସଂରଚନା ext2_extent_path));

	save_path->magic = EXT2_ET_MAGIC_EXTENT_PATH;
	save_path->leaf_height = info.max_depth - info.curr_level - 1;
	save_path->lblk = extent.e_lblk;

	*ret_path = save_path;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_extent_free_path(ext2_extent_path_t path)
{
	EXT2_CHECK_MAGIC(path, EXT2_ET_MAGIC_EXTENT_PATH);

	ext2fs_free_mem(&path);
	ଫେରନ୍ତୁ 0;
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
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା leaf_level, blk64_t blk)
{
	ସଂରଚନା ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ମୂଳ, &extent);
	ଯଦି (retval) {
		ଯଦି (retval == EXT2_ET_EXTENT_NO_NEXT)
			retval = EXT2_ET_EXTENT_NOT_FOUND;
		ଫେରନ୍ତୁ retval;
	}

	ଯଦି (leaf_level > handle->max_depth) {
#ifdef DEBUG
		printf("leaf level %d greater than tree depth %d\n",
			leaf_level, handle->max_depth);
#endif
		ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
	}

#ifdef DEBUG
	printf("goto extent ino %u, level %d, %llu\n", handle->ino,
	       leaf_level, blk);
#endif

#ifdef DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#endif
	ଯେତେବେଳେ (1) {
		ଯଦି (handle->max_depth - handle->level == leaf_level) {
			/* block is in this &extent */
			ଯଦି ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				ଫେରନ୍ତୁ 0;
			ଯଦି (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				ଫେରନ୍ତୁ EXT2_ET_EXTENT_NOT_FOUND;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_SIB,
						   &extent);
			ଯଦି (retval == EXT2_ET_EXTENT_NO_NEXT)
				ଫେରନ୍ତୁ EXT2_ET_EXTENT_NOT_FOUND;
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
			ଜାରି;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_SIB,
					   &extent);
		ଯଦି (retval == EXT2_ET_EXTENT_NO_NEXT)
			ଯାଅ go_down;
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#endif
		ଯଦି (blk == extent.e_lblk)
			ଯାଅ go_down;
		ଯଦି (blk > extent.e_lblk)
			ଜାରି;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#endif

	go_down:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_DOWN,
					   &extent);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

#ifdef DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#endif
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	ଫେରନ୍ତୁ ext2fs_extent_goto2(handle, 0, blk);
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
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				retval = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				orig_height;
	blk64_t				start;
	ସଂରଚନା extent_path		*path;
	ସଂରଚନା ext2fs_extent		extent;
	ସଂରଚନା ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ଯଦି (!(handle->fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	ଯଦି (!handle->path)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

	path = handle->path + handle->level;
	ଯଦି (!path->curr)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ଯଦି (retval)
		ଯାଅ done;

	/* modified node's start block */
	start = extent.e_lblk;

	ଯଦି ((retval = ext2fs_extent_get_info(handle, &info)))
		ଫେରନ୍ତୁ retval;
	orig_height = info.max_depth - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	ଯେତେବେଳେ (handle->level > 0 &&
	       (path->ବାମ == path->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		ଯଦି (retval)
			ଯାଅ done;
		ଯଦି (extent.e_lblk == start)
			ଭାଙ୍ଗ;
		path = handle->path + handle->level;
		extent.e_len += (extent.e_lblk - start);
		extent.e_lblk = start;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ଯଦି (retval)
			ଯାଅ done;
		update_path(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_height, start);
done:
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)),
				ସଂରଚନା ext2fs_extent *extent)
{
	ସଂରଚନା extent_path		*path;
	ସଂରଚନା ext3_extent_idx		*ix;
	ସଂରଚନା ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ଯଦି (!(handle->fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	ଯଦି (!handle->path)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

	path = handle->path + handle->level;
	ଯଦି (!path->curr)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

#ifdef DEBUG
	printf("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	ଯଦି (handle->level == handle->max_depth) {
		ex = path->curr;

		ex->ee_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ex->ee_start = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_start_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ଯଦି (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			ଯଦି (extent->e_len > EXT_UNINIT_MAX_LEN)
				ଫେରନ୍ତୁ EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} ଅନ୍ୟଥା {
			ଯଦି (extent->e_len > EXT_INIT_MAX_LEN)
				ଫେରନ୍ତୁ EXT2_ET_EXTENT_INVALID_LENGTH;
			ex->ee_len = ext2fs_cpu_to_le16(extent->e_len);
		}
	} ଅନ୍ୟଥା {
		ix = path->curr;

		ix->ei_leaf = ext2fs_cpu_to_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_leaf_hi = ext2fs_cpu_to_le16(extent->e_pblk >> 32);
		ix->ei_block = ext2fs_cpu_to_le32(extent->e_lblk);
		ix->ei_unused = 0;
	}
	update_path(handle);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା splitting_at_eof(ସଂରଚନା ext2_extent_handle *handle,
			    ସଂରଚନା extent_path *path)
{
	ସଂରଚନା extent_path *ppp = path;
	dump_path(__func__, handle, path);

	ଯଦି (handle->level == 0)
		ଫେରନ୍ତୁ 0;

	କର {
		ଯଦି (ppp->ବାମ)
			ଫେରନ୍ତୁ 0;
		ppp--;
	} ଯେତେବେଳେ (ppp >= handle->path);

	ଫେରନ୍ତୁ 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
ସ୍ଥିର errcode_t extent_ଗଣ୍ଠି_split(ext2_extent_handle_t handle,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_ଗଣ୍ଠି_pblk;
	blk64_t				new_ଗଣ୍ଠି_start;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				orig_height;
	ଅକ୍ଷର				*block_buf = ଖାଲି;
	ସଂରଚନା ext2fs_extent		extent;
	ସଂରଚନା extent_path		*path, *newpath = 0;
	ସଂରଚନା ext3_extent_header	*eh, *neweh;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				tocopy;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				new_ମୂଳ = 0;
	ସଂରଚନା ext2_extent_info		info;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ଯଦି (!(handle->fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	ଯଦି (!handle->path)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

#ifdef DEBUG
	printf("splitting node at level %d\n", handle->level);
#endif
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ଯଦି (retval)
		ଯାଅ done;

	retval = ext2fs_extent_get_info(handle, &info);
	ଯଦି (retval)
		ଯାଅ done;

	/* save the position we were originally splitting... */
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	path = handle->path + handle->level;
	eh = (ସଂରଚନା ext3_extent_header *) path->buf;
	ଯଦି (handle->level == handle->max_depth) {
		ସଂରଚନା ext3_extent	*ex;

		ex = EXT_FIRST_EXTENT(eh);
		goal_blk = ext2fs_le32_to_cpu(ex->ee_start) +
			((__u64) ext2fs_le16_to_cpu(ex->ee_start_hi) << 32);
	} ଅନ୍ୟଥା {
		ସଂରଚନା ext3_extent_idx	*ix;

		ix = EXT_FIRST_INDEX(eh);
		goal_blk = ext2fs_le32_to_cpu(ix->ei_leaf) +
			((__u64) ext2fs_le16_to_cpu(ix->ei_leaf_hi) << 32);
	}
	goal_blk -= EXT2FS_CLUSTER_RATIO(handle->fs);
	goal_blk &= ~EXT2FS_CLUSTER_MASK(handle->fs);

	/* Is there room in the parent for a new entry? */
	ଯଦି (handle->level &&
			(handle->path[handle->level - 1].entries >=
			 handle->path[handle->level - 1].max_entries)) {

#ifdef DEBUG
		printf("parent level %d full; splitting it too\n",
							handle->level - 1);
#endif
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		ଯଦି (retval)
			ଯାଅ done;

		retval = extent_ଗଣ୍ଠି_split(handle, expand_allowed);
		ଯଦି (retval)
			ଯାଅ done;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
		ଯଦି (retval)
			ଯାଅ done;
	}

	/* At this point, parent should have room for this split */
	path = handle->path + handle->level;
	ଯଦି (!path->curr)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, path) : 0;

	/* extent header of the current node we'll split */
	eh = (ସଂରଚନା ext3_extent_header *)path->buf;

	/* splitting root level means moving them all out */
	ଯଦି (handle->level == 0) {
		new_ମୂଳ = 1;
		tocopy = ext2fs_le16_to_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    ଆକାର(ସଂରଚନା extent_path),
					    &newpath);
		ଯଦି (retval)
			ଯାଅ done;
	} ଅନ୍ୟଥା {
		ଯଦି (no_balance)
			tocopy = 1;
		ଅନ୍ୟଥା
			tocopy = ext2fs_le16_to_cpu(eh->eh_entries) / 2;
	}

#ifdef DEBUG
	printf("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_to_cpu(eh->eh_entries),
				handle->level);
#endif

	ଯଦି (!tocopy && !no_balance) {
#ifdef DEBUG
		printf("Nothing to copy to new block!\n");
#endif
		retval = EXT2_ET_CANT_SPLIT_EXTENT;
		ଯାଅ done;
	}

	/* first we need a new block, or can do nothing. */
	block_buf = malloc(handle->fs->blocksize);
	ଯଦି (!block_buf) {
		retval = ENOMEM;
		ଯାଅ done;
	}

	ଯଦି (!goal_blk)
		goal_blk = ext2fs_find_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, block_buf,
				    &new_ଗଣ୍ଠି_pblk);
	ଯଦି (retval)
		ଯାଅ done;

#ifdef DEBUG
	printf("will copy to new node at block %lu\n",
	       (unsigned long) new_ଗଣ୍ଠି_pblk);
#endif

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (ସଂରଚନା ext3_extent_header *) block_buf;
	memcpy(neweh, eh, ଆକାର(ସଂରଚନା ext3_extent_header));
	neweh->eh_entries = ext2fs_cpu_to_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_to_le16((handle->fs->blocksize -
			 ଆକାର(ସଂରଚନା ext3_extent_header)) /
				ଆକାର(ସଂରଚନା ext3_extent));

	/* then the entries for the new block... */
	memcpy(EXT_FIRST_INDEX(neweh),
		EXT_FIRST_INDEX(eh) +
			(ext2fs_le16_to_cpu(eh->eh_entries) - tocopy),
		ଆକାର(ସଂରଚନା ext3_extent_idx) * tocopy);

	new_ଗଣ୍ଠି_start = ext2fs_le32_to_cpu(EXT_FIRST_INDEX(neweh)->ei_block);

	/* then update the checksum */
	retval = ext2fs_extent_block_csum_set(handle->fs, handle->ino, neweh);
	ଯଦି (retval)
		ଯାଅ done;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_ଗଣ୍ଠି_pblk, 1,
					block_buf);

	ଯଦି (retval)
		ଯାଅ done;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	ଯଦି (handle->level == 0) {
		memcpy(newpath, path,
		       ଆକାର(ସଂରଚନା extent_path) * handle->max_paths);
		handle->path = newpath;
		newpath = path;
		path = handle->path;
		path->entries = 1;
		path->ବାମ = path->max_entries - 1;
		handle->max_depth++;
		handle->max_paths++;
		eh->eh_depth = ext2fs_cpu_to_le16(handle->max_depth);
	} ଅନ୍ୟଥା {
		path->entries -= tocopy;
		path->ବାମ -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_path(handle);
	ଯଦି (retval)
		ଯାଅ done;

	/* now go up and insert/replace index for new node we created */
	ଯଦି (new_ମୂଳ) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_FIRST_SIB, &extent);
		ଯଦି (retval)
			ଯାଅ done;

		extent.e_lblk = new_ଗଣ୍ଠି_start;
		extent.e_pblk = new_ଗଣ୍ଠି_pblk;
		extent.e_len = handle->path[0].end_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ଯଦି (retval)
			ଯାଅ done;
	} ଅନ୍ୟଥା {
		__u32 new_ଗଣ୍ଠି_length;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP, &extent);
		/* will insert after this one; it's length is shorter now */
		new_ଗଣ୍ଠି_length = new_ଗଣ୍ଠି_start - extent.e_lblk;
		extent.e_len -= new_ଗଣ୍ଠି_length;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ଯଦି (retval)
			ଯାଅ done;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_ଗଣ୍ଠି_start;
		extent.e_pblk = new_ଗଣ୍ଠି_pblk;
		extent.e_len = new_ଗଣ୍ଠି_length;
		retval = ext2fs_extent_ଭର୍ତ୍ତି(handle, EXT2_EXTENT_ଭର୍ତ୍ତି_AFTER, &extent);
		ଯଦି (retval)
			ଯାଅ done;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	ଯଦି (retval)
		ଯାଅ done;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_add_blocks(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	ଯଦି (retval)
		ଯାଅ done;

done:
	ଯଦି (newpath)
		ext2fs_free_mem(&newpath);
	ମୁକ୍ତ(block_buf);

	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_extent_ଗଣ୍ଠି_split(ext2_extent_handle_t handle)
{
	ଫେରନ୍ତୁ extent_ଗଣ୍ଠି_split(handle, 0);
}

errcode_t ext2fs_extent_ଭର୍ତ୍ତି(ext2_extent_handle_t handle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
				      ସଂରଚନା ext2fs_extent *extent)
{
	ସଂରଚନା extent_path		*path;
	ସଂରଚନା ext3_extent_idx		*ix;
	ସଂରଚନା ext3_extent_header	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ଯଦି (!(handle->fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	ଯଦି (!handle->path)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

#ifdef DEBUG
	printf("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#endif

	path = handle->path + handle->level;

	ଯଦି (path->entries >= path->max_entries) {
		ଯଦି (flags & EXT2_EXTENT_ଭର୍ତ୍ତି_NOSPLIT) {
			ଫେରନ୍ତୁ EXT2_ET_CANT_ଭର୍ତ୍ତି_EXTENT;
		} ଅନ୍ୟଥା {
#ifdef DEBUG
			printf("node full (level %d) - splitting\n",
				   handle->level);
#endif
			retval = extent_ଗଣ୍ଠି_split(handle, 1);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
			path = handle->path + handle->level;
		}
	}

	eh = (ସଂରଚନା ext3_extent_header *) path->buf;
	ଯଦି (path->curr) {
		ix = path->curr;
		ଯଦି (flags & EXT2_EXTENT_ଭର୍ତ୍ତି_AFTER) {
			ix++;
			path->ବାମ--;
		}
	} ଅନ୍ୟଥା {
		ix = EXT_FIRST_INDEX(eh);
		path->ବାମ = -1;
	}

	path->curr = ix;

	ଯଦି (path->ବାମ >= 0)
		memmove(ix + 1, ix,
			(path->ବାମ+1) * ଆକାର(ସଂରଚନା ext3_extent_idx));
	path->ବାମ++;
	path->entries++;

	eh = (ସଂରଚନା ext3_extent_header *) path->buf;
	eh->eh_entries = ext2fs_cpu_to_le16(path->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	ଯଦି (retval)
		ଯାଅ errout;

	retval = update_path(handle);
	ଯଦି (retval)
		ଯାଅ errout;

	ଫେରନ୍ତୁ 0;

errout:
	ext2fs_extent_delete(handle, 0);
	ଫେରନ୍ତୁ retval;
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
				 blk64_t logical, blk64_t physical, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	errcode_t		ec, retval = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			mapped = 1; /* logical is mapped? */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			orig_height;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			extent_uninit = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			prev_uninit = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			next_uninit = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			new_uninit = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			max_len = EXT_INIT_MAX_LEN;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			has_prev, has_next;
	blk64_t			orig_lblk;
	ସଂରଚନା extent_path	*path;
	ସଂରଚନା ext2fs_extent	extent, next_extent, prev_extent;
	ସଂରଚନା ext2fs_extent	newextent;
	ସଂରଚନା ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ifdef DEBUG
	printf("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#endif

	ଯଦି (!(handle->fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	ଯଦି (!handle->path)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

	path = handle->path + handle->level;

	ଯଦି (flags & EXT2_EXTENT_SET_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	ଯଦି (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_LEAF;
		ଯଦି (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	ଯଦି ((handle->max_depth == 0) && (path->entries == 0)) {
		retval = ext2fs_extent_ଭର୍ତ୍ତି(handle, 0, &newextent);
		ଫେରନ୍ତୁ retval;
	}

	/* save our original location in the extent tree */
	ଯଦି ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		ଯଦି (retval != EXT2_ET_NO_CURRENT_ଗଣ୍ଠି)
			ଫେରନ୍ତୁ retval;
		memset(&extent, 0, ଆକାର(extent));
	}
	ଯଦି ((retval = ext2fs_extent_get_info(handle, &info)))
		ଫେରନ୍ତୁ retval;
	orig_height = info.max_depth - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	ଯଦି (retval) {
		ଯଦି (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			retval = 0;
			mapped = 0;
			ଯଦି (!physical) {
#ifdef DEBUG
				printf("block %llu already unmapped\n",
					logical);
#endif
				ଯାଅ done;
			}
		} ଅନ୍ୟଥା
			ଯାଅ done;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ଯଦି (retval)
		ଯାଅ done;
	ଯଦି (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &next_extent);
	ଯଦି (retval) {
		has_next = 0;
		ଯଦି (retval != EXT2_ET_EXTENT_NO_NEXT)
			ଯାଅ done;
	} ଅନ୍ୟଥା {
		dbg_print_extent("set_bmap: next_extent",
				 &next_extent);
		has_next = 1;
		ଯଦି (next_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			next_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ଯଦି (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		ଯାଅ done;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_LEAF, &prev_extent);
	ଯଦି (retval) {
		has_prev = 0;
		ଯଦି (retval != EXT2_ET_EXTENT_NO_PREV)
			ଯାଅ done;
	} ଅନ୍ୟଥା {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		ଯଦି (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ଯଦି (retval && retval != EXT2_ET_EXTENT_NOT_FOUND)
		ଯାଅ done;

	/* check if already pointing to the requested physical */
	ଯଦି (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ifdef DEBUG
		printf("physical block (at %llu) unchanged\n", logical);
#endif
		ଯାଅ done;
	}

	ଯଦି (!mapped) {
#ifdef DEBUG
		printf("mapping unmapped logical block %llu\n", logical);
#endif
		ଯଦି ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ଅନ୍ୟଥା ଯଦି ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ଅନ୍ୟଥା ଯଦି (has_next &&
			   (logical == next_extent.e_lblk - 1) &&
			   (physical == next_extent.e_pblk - 1) &&
			   (new_uninit == next_uninit) &&
			   ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) next_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &next_extent);
			ଯଦି (retval)
				ଯାଅ done;
			next_extent.e_len++;
			next_extent.e_lblk--;
			next_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &next_extent);
		} ଅନ୍ୟଥା ଯଦି (logical < extent.e_lblk)
			retval = ext2fs_extent_ଭର୍ତ୍ତି(handle, 0, &newextent);
		ଅନ୍ୟଥା
			retval = ext2fs_extent_ଭର୍ତ୍ତି(handle,
				      EXT2_EXTENT_ଭର୍ତ୍ତି_AFTER, &newextent);
		ଯଦି (retval)
			ଯାଅ done;
		retval = ext2fs_extent_fix_parents(handle);
		ଯଦି (retval)
			ଯାଅ done;
	} ଅନ୍ୟଥା ଯଦି ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ifdef DEBUG
		printf("(re/un)mapping only block in extent\n");
#endif
		ଯଦି (physical) {
			ଯଦି (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				ଯଦି (retval)
					ଯାଅ done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_NEXT_LEAF,
							   &extent);
				ଯଦି (retval)
					ଯାଅ done;
				ଯାଅ delete_ଗଣ୍ଠି;

			} ଅନ୍ୟଥା
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} ଅନ୍ୟଥା {
		delete_ଗଣ୍ଠି:
			retval = ext2fs_extent_delete(handle, 0);
			ଯଦି (retval)
				ଯାଅ done;
			ec = ext2fs_extent_fix_parents(handle);
			ଯଦି (ec != EXT2_ET_NO_CURRENT_ଗଣ୍ଠି)
				retval = ec;
		}

		ଯଦି (retval)
			ଯାଅ done;
	} ଅନ୍ୟଥା ଯଦି (logical == extent.e_lblk + extent.e_len - 1)  {
#ifdef DEBUG
		printf("(re/un)mapping last block in extent\n");
#endif
		ଯଦି (physical) {
			ଯଦି (has_next &&
			    (logical == (next_extent.e_lblk - 1)) &&
			    (physical == (next_extent.e_pblk - 1)) &&
			    (new_uninit == next_uninit) &&
			    ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) next_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_NEXT_LEAF, &next_extent);
				ଯଦି (retval)
					ଯାଅ done;
				next_extent.e_len++;
				next_extent.e_lblk--;
				next_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &next_extent);
				ଯଦି (retval)
					ଯାଅ done;
			} ଅନ୍ୟଥା
				retval = ext2fs_extent_ଭର୍ତ୍ତି(handle,
				      EXT2_EXTENT_ଭର୍ତ୍ତି_AFTER, &newextent);
			ଯଦି (retval)
				ଯାଅ done;
			retval = ext2fs_extent_fix_parents(handle);
			ଯଦି (retval)
				ଯାଅ done;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			ଯଦି (retval)
				ଯାଅ done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			ଯଦି (retval)
				ଯାଅ done;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ଯଦି (retval)
			ଯାଅ done;
	} ଅନ୍ୟଥା ଯଦି (logical == extent.e_lblk) {
#ifdef DEBUG
		printf("(re/un)mapping first block in extent\n");
#endif
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ଯଦି (retval)
			ଯାଅ done;
		retval = ext2fs_extent_fix_parents(handle);
		ଯଦି (retval)
			ଯାଅ done;
		ଯଦି (physical) {
			ଯଦି (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_LEAF, &prev_extent);
				ଯଦି (retval)
					ଯାଅ done;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} ଅନ୍ୟଥା
				retval = ext2fs_extent_ଭର୍ତ୍ତି(handle,
							      0, &newextent);
			ଯଦି (retval)
				ଯାଅ done;
			retval = ext2fs_extent_fix_parents(handle);
			ଯଦି (retval)
				ଯାଅ done;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &extent);
			ଯଦି (retval)
				ଯାଅ done;
		}
	} ଅନ୍ୟଥା {
		__u32	save_length;
		blk64_t	save_lblk;
		ସଂରଚନା ext2fs_extent save_extent;
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
		ଯଦି (retval)
			ଯାଅ done;
		/* insert our new extent, if any */
		ଯଦି (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_ଭର୍ତ୍ତି(handle,
					EXT2_EXTENT_ଭର୍ତ୍ତି_AFTER, &newextent);
			ଯଦି (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				ଯଦି (r2 == 0)
					(ଶୂନ୍ୟ)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				ଯାଅ done;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_length - extent.e_len - 1;
		retval = ext2fs_extent_ଭର୍ତ୍ତି(handle,
				EXT2_EXTENT_ଭର୍ତ୍ତି_AFTER, &extent);
		ଯଦି (retval) {
			ଯଦି (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				ଯଦି (r2 == 0)
					(ଶୂନ୍ୟ)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			ଯଦି (r2 == 0)
				(ଶୂନ୍ୟ)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			ଯାଅ done;
		}
	}

done:
	/* get handle back to its position */
	ଯଦି (orig_height > handle->max_depth)
		orig_height = handle->max_depth; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_height, orig_lblk);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା extent_path		*path;
	ଅକ୍ଷର 				*cp;
	ସଂରଚନା ext3_extent_header	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ଯଦି (!(handle->fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	ଯଦି (!handle->path)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

#ifdef DEBUG
	{
		ସଂରଚନା ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		ଯଦି (retval == 0) {
			printf("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#endif

	path = handle->path + handle->level;
	ଯଦି (!path->curr)
		ଫେରନ୍ତୁ EXT2_ET_NO_CURRENT_ଗଣ୍ଠି;

	cp = path->curr;

	/* Sanity check before memmove() */
	ଯଦି (path->ବାମ < 0)
		ଫେରନ୍ତୁ EXT2_ET_EXTENT_LEAF_BAD;

	ଯଦି (path->ବାମ) {
		memmove(cp, cp + ଆକାର(ସଂରଚନା ext3_extent_idx),
			path->ବାମ * ଆକାର(ସଂରଚନା ext3_extent_idx));
		path->ବାମ--;
	} ଅନ୍ୟଥା {
		ସଂରଚନା ext3_extent_idx	*ix = path->curr;
		ix--;
		path->curr = ix;
	}
	ଯଦି (--path->entries == 0)
		path->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	ଯଦି (path->entries == 0 && handle->level) {
		ଯଦି (!(flags & EXT2_EXTENT_DELETE_KEEP_EMPTY)) {
			ସଂରଚନା ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_UP,
								&extent);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_blocks -=
				(handle->fs->blocksize *
				 EXT2FS_CLUSTER_RATIO(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_block_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} ଅନ୍ୟଥା {
		eh = (ସଂରଚନା ext3_extent_header *) path->buf;
		eh->eh_entries = ext2fs_cpu_to_le16(path->entries);
		ଯଦି ((path->entries == 0) && (handle->level == 0)) {
			eh->eh_depth = 0;
			handle->max_depth = 0;
		}
		retval = update_path(handle);
	}
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 ସଂରଚନା ext2_extent_info *info)
{
	ସଂରଚନା extent_path		*path;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, ଆକାର(ସଂରଚନା ext2_extent_info));

	path = handle->path + handle->level;
	ଯଦି (path) {
		ଯଦି (path->curr)
			info->curr_entry = ((ଅକ୍ଷର *) path->curr - path->buf) /
				ଆକାର(ସଂରଚନା ext3_extent_idx);
		ଅନ୍ୟଥା
			info->curr_entry = 0;
		info->ଅଙ୍କ_entries = path->entries;
		info->max_entries = path->max_entries;
		info->bytes_avail = (path->max_entries - path->entries) *
			ଆକାର(ସଂରଚନା ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_depth = handle->max_depth;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	ଫେରନ୍ତୁ 0;
}

size_t ext2fs_max_extent_depth(ext2_extent_handle_t handle)
{
	size_t iblock_sz = ଆକାର(((ସଂରଚନା ext2_inode *)ଖାଲି)->i_block);
	size_t iblock_extents = (iblock_sz - ଆକାର(ସଂରଚନା ext3_extent_header)) /
				ଆକାର(ସଂରଚନା ext3_extent);
	size_t extents_per_block = (handle->fs->blocksize -
				    ଆକାର(ସଂରଚନା ext3_extent_header)) /
				   ଆକାର(ସଂରଚନା ext3_extent);
	ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_blocksize = 0;
	ସ୍ଥିର size_t last_result = 0;

	ଯଦି (last_blocksize && last_blocksize == handle->fs->blocksize)
		ଫେରନ୍ତୁ last_result;

	last_result = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_per_block));
	last_blocksize = handle->fs->blocksize;
	ଫେରନ୍ତୁ last_result;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       ସଂରଚନା ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	ସଂରଚନା ext2fs_extent	extent;
	errcode_t		errcode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା    			save_flags = fs->flags;

	ଯଦି (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		ଫେରନ୍ତୁ 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ଯଦି (errcode) {
		ଯଦି (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		ଫେରନ୍ତୁ errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ମୂଳ, &extent);
	ଯଦି (errcode)
		ଯାଅ out;

	କର {
		/* Skip to the end of a block of leaf nodes */
		ଯଦି (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_LAST_SIB,
						    &extent);
			ଯଦି (errcode)
				ଭାଙ୍ଗ;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT, &extent);
		ଯଦି (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_path(handle);
	} ଯେତେବେଳେ (errcode == 0);

out:
	/* Ok if we run off the end */
	ଯଦି (errcode == EXT2_ET_EXTENT_NO_NEXT)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	ଫେରନ୍ତୁ errcode;
}

errcode_t ext2fs_decode_extent(ସଂରଚନା ext2fs_extent *to, ଶୂନ୍ୟ *addr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
	ସଂରଚନା ext3_extent *from = (ସଂରଚନା ext3_extent *)addr;

	ଯଦି (len != ଆକାର(ସଂରଚନା ext3_extent))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	to->e_pblk = ext2fs_le32_to_cpu(from->ee_start) +
		((__u64) ext2fs_le16_to_cpu(from->ee_start_hi)
			<< 32);
	to->e_lblk = ext2fs_le32_to_cpu(from->ee_block);
	to->e_len = ext2fs_le16_to_cpu(from->ee_len);
	to->e_flags = EXT2_EXTENT_FLAGS_LEAF;
	ଯଦି (to->e_len > EXT_INIT_MAX_LEN) {
		to->e_len -= EXT_INIT_MAX_LEN;
		to->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_ଗଣନା_blocks(ext2_filsys fs, ext2_ino_t ino,
			      ସଂରଚନା ext2_inode *inode, blk64_t *ret_ଗଣନା)
{
	ext2_extent_handle_t	handle = ଖାଲି;
	ସଂରଚନା ext2fs_extent	extent;
	errcode_t		errcode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ଯଦି (errcode)
		ଯାଅ out;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ମୂଳ, &extent);
	ଯଦି (errcode)
		ଯାଅ out;

	errcode = ext2fs_get_array(handle->max_depth, ଆକାର(blk64_t),
				   &intermediate_nodes);
	ଯଦି (errcode)
		ଯାଅ out;

	blkcount = handle->level;
	ଯେତେବେଳେ (!errcode) {
		ଯଦି (extent.e_flags & EXT2_EXTENT_FLAGS_LEAF) {
			blkcount += extent.e_len;
			ପାଇଁ (i = 0; i < handle->level; i++) {
				ଯଦି (intermediate_nodes[i] !=
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
	*ret_ଗଣନା = blkcount;
	ext2fs_extent_free(handle);

	ଫେରନ୍ତୁ 0;
}

#ifdef DEBUG
/*
 * Override debugfs's prompt
 */
ଅଚଳ ଅକ୍ଷର *debug_prog_name = "tst_extents";

#endif

