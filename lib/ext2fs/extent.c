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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "e2image.h"

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

ਬਣਤਰ extent_ਰਸਤਾ {
	ਅੱਖਰ		*buf;
	ਪੂਰਨ		entries;
	ਪੂਰਨ		max_entries;
	ਪੂਰਨ		ਖੱਬਾ;
	ਪੂਰਨ		ਦੇਖੋ_ਅੰਕ;
	ਪੂਰਨ		flags;
	blk64_t		ਸਿਰਾ_blk;
	blk64_t		blk;
	ਖਾਲੀ		*curr;
};


ਬਣਤਰ ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	ਬਣਤਰ ext2_inode	*inode;
	ਬਣਤਰ ext2_inode	inodebuf;
	ਪੂਰਨ			type;
	ਪੂਰਨ			level;
	ਪੂਰਨ			max_ਡੂੰਘਾਈ;
	ਪੂਰਨ			max_paths;
	ਬਣਤਰ extent_ਰਸਤਾ	*ਰਸਤਾ;
};

ਬਣਤਰ ext2_extent_ਰਸਤਾ {
	errcode_t		magic;
	ਪੂਰਨ			ਪੱਤੀ_ਉਚਾਈ;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
ਸਥਿਰ ਖਾਲੀ dbg_show_ਸਿਰਲੇਖ(ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *eh)
{
	ਲਿਖੋ("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_magic),
			ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_entries),
			ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_max),
			ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_ਡੂੰਘਾਈ),
			ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(eh->eh_generation));
}

ਸਥਿਰ ਖਾਲੀ dbg_show_ਸੂਚਕਾਂਕ(ਬਣਤਰ ext3_extent_idx *ix)
{
	ਲਿਖੋ("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਖੰਡ),
			ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਪੱਤੀ),
			ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਪੱਤੀ_hi),
			ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਅਣਵਰਤਿਆ));
}

ਸਥਿਰ ਖਾਲੀ dbg_show_extent(ਬਣਤਰ ext3_extent *ex)
{
	ਲਿਖੋ("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ex->ee_ਖੰਡ),
			ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ex->ee_ਖੰਡ) +
			ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ex->ee_len) - 1,
			ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ex->ee_len),
			ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ex->ee_ਸ਼ੁਰੂ),
			ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ex->ee_ਸ਼ੁਰੂ_hi));
}

ਸਥਿਰ ਖਾਲੀ dbg_print_extent(ਅੱਖਰ *desc, ਬਣਤਰ ext2fs_extent *extent)
{
	ਜੇ (desc)
		ਲਿਖੋ("%s: ", desc);
	ਲਿਖੋ("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ਜੇ (extent->e_flags & EXT2_EXTENT_FLAGS_ਪੱਤੀ)
		fputs("LEAF ", stdout);
	ਜੇ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ਜੇ (extent->e_flags & EXT2_EXTENT_FLAGS_ਦੂਜਾ_ਦੇਖੋ)
		fputs("2ND_VISIT ", stdout);
	ਜੇ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

ਸਥਿਰ ਖਾਲੀ dump_ਰਸਤਾ(ਅਚਲ ਅੱਖਰ *ਟੈਗ, ਬਣਤਰ ext2_extent_handle *handle,
		      ਬਣਤਰ extent_ਰਸਤਾ *ਰਸਤਾ)
{
	ਬਣਤਰ extent_ਰਸਤਾ *ppp = ਰਸਤਾ;
	ਲਿਖੋ("%s: level=%d\n", ਟੈਗ, handle->level);

	ਕਰੋ {
		ਲਿਖੋ("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       ਟੈਗ, (ppp - handle->ਰਸਤਾ), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->ਖੱਬਾ, ppp->ਦੇਖੋ_ਅੰਕ, ppp->flags,
		       ppp->ਸਿਰਾ_blk, ppp->curr, ppp->curr - (ਖਾਲੀ *)ppp->buf);
		ਲਿਖੋ("  ");
		dbg_show_ਸਿਰਲੇਖ((ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *)ppp->buf);
		ਜੇ (ppp->curr) {
			ਲਿਖੋ("  ");
			dbg_show_ਸੂਚਕਾਂਕ(ppp->curr);
			ਲਿਖੋ("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} ਜਦੋਂ (ppp >= handle->ਰਸਤਾ);
	fflush(stdout);

	ਵਾਪਸ;
}

#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dbg_show_ਸਿਰਲੇਖ(eh) ਕਰੋ { } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dbg_show_ਸੂਚਕਾਂਕ(ix) ਕਰੋ { } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dbg_show_extent(ex) ਕਰੋ { } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dbg_print_extent(desc, ex) ਕਰੋ { } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dump_ਰਸਤਾ(ਟੈਗ, handle, ਰਸਤਾ) ਕਰੋ { } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_ਸਿਰਲੇਖ_verify(ਖਾਲੀ *ptr, ਪੂਰਨ ਮਾਪ)
{
	ਪੂਰਨ eh_max, ਇੰਦਰਾਜ_ਮਾਪ;
	ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *eh = ptr;

	dbg_show_ਸਿਰਲੇਖ(eh);
	ਜੇ (ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		ਵਾਪਸ EXT2_ET_EXTENT_ਸਿਰਲੇਖ_BAD;
	ਜੇ (ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_entries) > ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_max))
		ਵਾਪਸ EXT2_ET_EXTENT_ਸਿਰਲੇਖ_BAD;
	ਜੇ (eh->eh_ਡੂੰਘਾਈ == 0)
		ਇੰਦਰਾਜ_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext3_extent);
	ਨਹੀਂਤਾਂ
		ਇੰਦਰਾਜ_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext3_extent_idx);

	eh_max = (ਮਾਪ - ਆਕਾਰ(*eh)) / ਇੰਦਰਾਜ_ਮਾਪ;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	ਜੇ ((ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_max) < (eh_max - 2)))
		ਵਾਪਸ EXT2_ET_EXTENT_ਸਿਰਲੇਖ_BAD;

	ਵਾਪਸ 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
ਖਾਲੀ ext2fs_extent_free(ext2_extent_handle_t handle)
{
	ਪੂਰਨ			i;

	ਜੇ (!handle)
		ਵਾਪਸ;

	ਜੇ (handle->ਰਸਤਾ) {
		ਲਈ (i = 1; i < handle->max_paths; i++) {
			ਜੇ (handle->ਰਸਤਾ[i].buf)
				ext2fs_free_mem(&handle->ਰਸਤਾ[i].buf);
		}
		ext2fs_free_mem(&handle->ਰਸਤਾ);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	ਵਾਪਸ ext2fs_extent_open2(fs, ino, ਨਿਰਾ, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    ਬਣਤਰ ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	ਬਣਤਰ ext2_extent_handle	*handle;
	errcode_t			retval;
	ਪੂਰਨ				i;
	ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!inode)
		ਜੇ ((ino == 0) || (ino > fs->super->s_inodes_ਗਿਣਤੀ))
			ਵਾਪਸ EXT2_ET_BAD_INODE_ਅੰਕ;

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_extent_handle), &handle);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	memset(handle, 0, ਆਕਾਰ(ਬਣਤਰ ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	ਜੇ (inode) {
		handle->inode = inode;
	} ਨਹੀਂਤਾਂ {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		ਜੇ (retval)
			ਜਾਓ errout;
	}

	eh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) &handle->inode->i_ਖੰਡ[0];

	ਲਈ (i=0; i < EXT2_N_ਖੰਡਾਂ; i++)
		ਜੇ (handle->inode->i_ਖੰਡ[i])
			ਰੋਕੋ;
	ਜੇ (i >= EXT2_N_ਖੰਡਾਂ) {
		eh->eh_magic = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(EXT3_EXT_MAGIC);
		eh->eh_ਡੂੰਘਾਈ = 0;
		eh->eh_entries = 0;
		i = (ਆਕਾਰ(handle->inode->i_ਖੰਡ) - ਆਕਾਰ(*eh)) /
			ਆਕਾਰ(ਬਣਤਰ ext3_extent);
		eh->eh_max = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	ਜੇ (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		ਜਾਓ errout;
	}

	retval = ext2fs_extent_ਸਿਰਲੇਖ_verify(eh, ਆਕਾਰ(handle->inode->i_ਖੰਡ));
	ਜੇ (retval)
		ਜਾਓ errout;

	handle->max_ਡੂੰਘਾਈ = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_ਡੂੰਘਾਈ);
	handle->type = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_magic);

	handle->max_paths = handle->max_ਡੂੰਘਾਈ + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    ਆਕਾਰ(ਬਣਤਰ extent_ਰਸਤਾ),
				    &handle->ਰਸਤਾ);
	handle->ਰਸਤਾ[0].buf = (ਅੱਖਰ *) handle->inode->i_ਖੰਡ;

	handle->ਰਸਤਾ[0].ਖੱਬਾ = handle->ਰਸਤਾ[0].entries =
		ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_entries);
	handle->ਰਸਤਾ[0].max_entries = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_max);
	handle->ਰਸਤਾ[0].curr = 0;
	handle->ਰਸਤਾ[0].ਸਿਰਾ_blk =
		(EXT2_I_ਮਾਪ(handle->inode) + fs->blocksize - 1) >>
		 EXT2_ਖੰਡ_ਮਾਪ_ਬਿੱਟ(fs->super);
	handle->ਰਸਤਾ[0].blk = 0;
	handle->ਰਸਤਾ[0].ਦੇਖੋ_ਅੰਕ = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	ਵਾਪਸ 0;

errout:
	ext2fs_extent_free(handle);
	ਵਾਪਸ retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    ਪੂਰਨ flags, ਬਣਤਰ ext2fs_extent *extent)
{
	ਬਣਤਰ extent_ਰਸਤਾ	*ਰਸਤਾ, *newpath, *tp;
	ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ	*eh;
	ਬਣਤਰ ext3_extent_idx		*ix = 0;
	ਬਣਤਰ ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				ਸਿਰਾ_blk;
	ਪੂਰਨ				orig_op, op, l;
	ਪੂਰਨ				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ਜੇ (!handle->ਰਸਤਾ)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

	orig_op = op = flags & EXT2_EXTENT_MOVE_ਨਕਾਬ;

retry:
	ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;
	ਜੇ ((orig_op == EXT2_EXTENT_ਅਗਲਾ) ||
	    (orig_op == EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ)) {
		ਜੇ (handle->level < handle->max_ਡੂੰਘਾਈ) {
			/* interior node */
			ਜੇ (ਰਸਤਾ->ਦੇਖੋ_ਅੰਕ == 0) {
				ਰਸਤਾ->ਦੇਖੋ_ਅੰਕ++;
				op = EXT2_EXTENT_ਥੱਲੇ;
			} ਨਹੀਂਤਾਂ ਜੇ (ਰਸਤਾ->ਖੱਬਾ > 0)
				op = EXT2_EXTENT_ਅਗਲਾ_SIB;
			ਨਹੀਂਤਾਂ ਜੇ (handle->level > 0)
				op = EXT2_EXTENT_ਉੱਪਰ;
			ਨਹੀਂਤਾਂ
				ਵਾਪਸ EXT2_ET_EXTENT_NO_ਅਗਲਾ;
		} ਨਹੀਂਤਾਂ {
			/* leaf node */
			ਜੇ (ਰਸਤਾ->ਖੱਬਾ > 0)
				op = EXT2_EXTENT_ਅਗਲਾ_SIB;
			ਨਹੀਂਤਾਂ ਜੇ (handle->level > 0)
				op = EXT2_EXTENT_ਉੱਪਰ;
			ਨਹੀਂਤਾਂ
				ਵਾਪਸ EXT2_ET_EXTENT_NO_ਅਗਲਾ;
		}
		ਜੇ (op != EXT2_EXTENT_ਅਗਲਾ_SIB) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_GET_EXTENT
			ਲਿਖੋ("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_ਥੱਲੇ) ? "down" :
			       ((op == EXT2_EXTENT_ਉੱਪਰ) ? "up" : "unknown"));
#ਜੇ_ਅੰਤ
		}
	}

	ਜੇ ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_ਪੱਤੀ)) {
		ਜੇ (handle->level < handle->max_ਡੂੰਘਾਈ) {
			/* interior node */
			ਜੇ (ਰਸਤਾ->ਦੇਖੋ_ਅੰਕ > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_ਥੱਲੇ_AND_ਆਖਰੀ;
			} ਨਹੀਂਤਾਂ ਜੇ (ਰਸਤਾ->ਖੱਬਾ < ਰਸਤਾ->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			ਨਹੀਂਤਾਂ ਜੇ (handle->level > 0)
				op = EXT2_EXTENT_ਉੱਪਰ;
			ਨਹੀਂਤਾਂ
				ਵਾਪਸ EXT2_ET_EXTENT_NO_PREV;
		} ਨਹੀਂਤਾਂ {
			/* leaf node */
			ਜੇ (ਰਸਤਾ->ਖੱਬਾ < ਰਸਤਾ->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			ਨਹੀਂਤਾਂ ਜੇ (handle->level > 0)
				op = EXT2_EXTENT_ਉੱਪਰ;
			ਨਹੀਂਤਾਂ
				ਵਾਪਸ EXT2_ET_EXTENT_NO_PREV;
		}
		ਜੇ (op != EXT2_EXTENT_PREV_SIB) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_GET_EXTENT
			ਲਿਖੋ("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_ਥੱਲੇ_AND_ਆਖਰੀ) ? "down/last" :
			       ((op == EXT2_EXTENT_ਉੱਪਰ) ? "up" : "unknown"));
#ਜੇ_ਅੰਤ
		}
	}

	ਜੇ (orig_op == EXT2_EXTENT_ਆਖਰੀ_ਪੱਤੀ) {
		ਜੇ ((handle->level < handle->max_ਡੂੰਘਾਈ) &&
		    (ਰਸਤਾ->ਖੱਬਾ == 0))
			op = EXT2_EXTENT_ਥੱਲੇ;
		ਨਹੀਂਤਾਂ
			op = EXT2_EXTENT_ਆਖਰੀ_SIB;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_GET_EXTENT
		ਲਿਖੋ("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_ਥੱਲੇ) ? "down" : "last_sib");
#ਜੇ_ਅੰਤ
	}

	ਚੋਣ (op) {
	ਹਾਲਤ EXT2_EXTENT_CURRENT:
		ix = ਰਸਤਾ->curr;
		ਰੋਕੋ;
	ਹਾਲਤ EXT2_EXTENT_ਜੜ੍ਹ:
		handle->level = 0;
		ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;
		/* fallthrough */
	ਹਾਲਤ EXT2_EXTENT_ਪਹਿਲਾ_SIB:
		ਰਸਤਾ->ਖੱਬਾ = ਰਸਤਾ->entries;
		ਰਸਤਾ->curr = 0;
		/* fallthrough */
	ਹਾਲਤ EXT2_EXTENT_ਅਗਲਾ_SIB:
		ਜੇ (ਰਸਤਾ->ਖੱਬਾ <= 0)
			ਵਾਪਸ EXT2_ET_EXTENT_NO_ਅਗਲਾ;
		ਜੇ (ਰਸਤਾ->curr) {
			ix = ਰਸਤਾ->curr;
			ix++;
		} ਨਹੀਂਤਾਂ {
			eh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) ਰਸਤਾ->buf;
			ix = EXT_ਪਹਿਲਾ_ਸੂਚਕਾਂਕ(eh);
		}
		ਰਸਤਾ->ਖੱਬਾ--;
		ਰਸਤਾ->curr = ix;
		ਰਸਤਾ->ਦੇਖੋ_ਅੰਕ = 0;
		ਰੋਕੋ;
	ਹਾਲਤ EXT2_EXTENT_PREV_SIB:
		ਜੇ (!ਰਸਤਾ->curr ||
		    ਰਸਤਾ->ਖੱਬਾ+1 >= ਰਸਤਾ->entries)
			ਵਾਪਸ EXT2_ET_EXTENT_NO_PREV;
		ix = ਰਸਤਾ->curr;
		ix--;
		ਰਸਤਾ->curr = ix;
		ਰਸਤਾ->ਖੱਬਾ++;
		ਜੇ (handle->level < handle->max_ਡੂੰਘਾਈ)
			ਰਸਤਾ->ਦੇਖੋ_ਅੰਕ = 1;
		ਰੋਕੋ;
	ਹਾਲਤ EXT2_EXTENT_ਆਖਰੀ_SIB:
		eh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) ਰਸਤਾ->buf;
		ਰਸਤਾ->curr = EXT_ਆਖਰੀ_EXTENT(eh);
		ix = ਰਸਤਾ->curr;
		ਰਸਤਾ->ਖੱਬਾ = 0;
		ਰਸਤਾ->ਦੇਖੋ_ਅੰਕ = 0;
		ਰੋਕੋ;
	ਹਾਲਤ EXT2_EXTENT_ਉੱਪਰ:
		ਜੇ (handle->level <= 0)
			ਵਾਪਸ EXT2_ET_EXTENT_NO_ਉੱਪਰ;
		handle->level--;
		ਰਸਤਾ--;
		ix = ਰਸਤਾ->curr;
		ਜੇ ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_ਪੱਤੀ))
			ਰਸਤਾ->ਦੇਖੋ_ਅੰਕ = 0;
		ਰੋਕੋ;
	ਹਾਲਤ EXT2_EXTENT_ਥੱਲੇ:
	ਹਾਲਤ EXT2_EXTENT_ਥੱਲੇ_AND_ਆਖਰੀ:
		ਜੇ (!ਰਸਤਾ->curr ||(handle->level >= handle->max_ਡੂੰਘਾਈ))
			ਵਾਪਸ EXT2_ET_EXTENT_NO_ਥੱਲੇ;

		ix = ਰਸਤਾ->curr;
		newpath = ਰਸਤਾ + 1;
		ਜੇ (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			ਜੇ (retval)
				ਵਾਪਸ retval;
		}
		blk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਪੱਤੀ) +
			((__u64) ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਪੱਤੀ_hi) << 32);
		ਲਈ (l = handle->level, tp = ਰਸਤਾ; l > 0; l--, tp--) {
			ਜੇ (blk == tp->blk)
				ਵਾਪਸ EXT2_ET_EXTENT_ਚੱਕਰ;
		}
		newpath->blk = blk;
		ਜੇ ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		ਨਹੀਂਤਾਂ {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			ਜੇ (retval)
				ਵਾਪਸ retval;
		}
		handle->level++;

		eh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) newpath->buf;

		retval = ext2fs_extent_ਸਿਰਲੇਖ_verify(eh, handle->fs->blocksize);
		ਜੇ (retval) {
			handle->level--;
			ਵਾਪਸ retval;
		}

		ਜੇ (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_ਖੰਡ_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->ਖੱਬਾ = newpath->entries =
			ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		ਜੇ (newpath->ਖੱਬਾ <= 0)
			ਵਾਪਸ EXT2_ET_EXTENT_NO_ਥੱਲੇ;

		ਜੇ (ਰਸਤਾ->ਖੱਬਾ > 0) {
			ix++;
			newpath->ਸਿਰਾ_blk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਖੰਡ);
		} ਨਹੀਂਤਾਂ
			newpath->ਸਿਰਾ_blk = ਰਸਤਾ->ਸਿਰਾ_blk;

		ਰਸਤਾ = newpath;
		ਜੇ (op == EXT2_EXTENT_ਥੱਲੇ) {
			ix = EXT_ਪਹਿਲਾ_ਸੂਚਕਾਂਕ((ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) eh);
			ਰਸਤਾ->curr = ix;
			ਰਸਤਾ->ਖੱਬਾ = ਰਸਤਾ->entries - 1;
			ਰਸਤਾ->ਦੇਖੋ_ਅੰਕ = 0;
		} ਨਹੀਂਤਾਂ {
			ix = EXT_ਆਖਰੀ_ਸੂਚਕਾਂਕ((ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) eh);
			ਰਸਤਾ->curr = ix;
			ਰਸਤਾ->ਖੱਬਾ = 0;
			ਜੇ (handle->level < handle->max_ਡੂੰਘਾਈ)
				ਰਸਤਾ->ਦੇਖੋ_ਅੰਕ = 1;
		}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_GET_EXTENT
		ਲਿਖੋ("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_ਡੂੰਘਾਈ,
			   ਰਸਤਾ->ਸਿਰਾ_blk);
#ਜੇ_ਅੰਤ
		ਰੋਕੋ;
	ਮੂਲ:
		ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
	}

	ਜੇ (!ix)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

	extent->e_flags = 0;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_GET_EXTENT
	ਲਿਖੋ("(Left %d)\n", ਰਸਤਾ->ਖੱਬਾ);
#ਜੇ_ਅੰਤ

	ਜੇ (handle->level == handle->max_ਡੂੰਘਾਈ) {
		ex = (ਬਣਤਰ ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ex->ee_ਸ਼ੁਰੂ) +
			((__u64) ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ex->ee_ਸ਼ੁਰੂ_hi) << 32);
		extent->e_lblk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ex->ee_ਖੰਡ);
		extent->e_len = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_ਪੱਤੀ;
		ਜੇ (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} ਨਹੀਂਤਾਂ {
		extent->e_pblk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਪੱਤੀ) +
			((__u64) ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਪੱਤੀ_hi) << 32);
		extent->e_lblk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਖੰਡ);
		ਜੇ (ਰਸਤਾ->ਖੱਬਾ > 0) {
			ix++;
			ਸਿਰਾ_blk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਖੰਡ);
		} ਨਹੀਂਤਾਂ
			ਸਿਰਾ_blk = ਰਸਤਾ->ਸਿਰਾ_blk;

		extent->e_len = ਸਿਰਾ_blk - extent->e_lblk;
	}
	ਜੇ (ਰਸਤਾ->ਦੇਖੋ_ਅੰਕ)
		extent->e_flags |= EXT2_EXTENT_FLAGS_ਦੂਜਾ_ਦੇਖੋ;

	ਜੇ (((orig_op == EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ) ||
	     (orig_op == EXT2_EXTENT_PREV_ਪੱਤੀ)) &&
	    (handle->level != handle->max_ਡੂੰਘਾਈ))
		ਜਾਓ retry;

	ਜੇ ((orig_op == EXT2_EXTENT_ਆਖਰੀ_ਪੱਤੀ) &&
	    ((handle->level != handle->max_ਡੂੰਘਾਈ) ||
	     (ਰਸਤਾ->ਖੱਬਾ != 0)))
		ਜਾਓ retry;

	ਜੇ (failed_csum)
		ਵਾਪਸ EXT2_ET_EXTENT_CSUM_INVALID;

	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t update_ਰਸਤਾ(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	ਬਣਤਰ ext3_extent_idx		*ix;
	ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ	*eh;

	ਜੇ (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} ਨਹੀਂਤਾਂ {
		ix = handle->ਰਸਤਾ[handle->level - 1].curr;
		blk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਪੱਤੀ) +
			((__u64) ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਪੱਤੀ_hi) << 32);

		/* then update the checksum */
		eh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *)
				handle->ਰਸਤਾ[handle->level].buf;
		retval = ext2fs_extent_ਖੰਡ_csum_ਸੈੱਟ(handle->fs, handle->ino,
						      eh);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->ਰਸਤਾ[handle->level].buf);
	}
	ਵਾਪਸ retval;
}

#ਜੇ 0
errcode_t ext2fs_extent_save_ਰਸਤਾ(ext2_extent_handle_t handle,
				  ext2_extent_ਰਸਤਾ_t *ret_ਰਸਤਾ)
{
	ext2_extent_ਰਸਤਾ_t	save_ਰਸਤਾ;
	ਬਣਤਰ ext2fs_extent	extent;
	ਬਣਤਰ ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = ext2fs_extent_get_info(handle, &info);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_extent_ਰਸਤਾ), &save_ਰਸਤਾ);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	memset(save_ਰਸਤਾ, 0, ਆਕਾਰ(ਬਣਤਰ ext2_extent_ਰਸਤਾ));

	save_ਰਸਤਾ->magic = EXT2_ET_MAGIC_EXTENT_ਰਸਤਾ;
	save_ਰਸਤਾ->ਪੱਤੀ_ਉਚਾਈ = info.max_ਡੂੰਘਾਈ - info.curr_level - 1;
	save_ਰਸਤਾ->lblk = extent.e_lblk;

	*ret_ਰਸਤਾ = save_ਰਸਤਾ;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_extent_free_ਰਸਤਾ(ext2_extent_ਰਸਤਾ_t ਰਸਤਾ)
{
	EXT2_CHECK_MAGIC(ਰਸਤਾ, EXT2_ET_MAGIC_EXTENT_ਰਸਤਾ);

	ext2fs_free_mem(&ਰਸਤਾ);
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ

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
			      ਪੂਰਨ ਪੱਤੀ_level, blk64_t blk)
{
	ਬਣਤਰ ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ਜੜ੍ਹ, &extent);
	ਜੇ (retval) {
		ਜੇ (retval == EXT2_ET_EXTENT_NO_ਅਗਲਾ)
			retval = EXT2_ET_EXTENT_NOT_ਮਿਲਿਆ;
		ਵਾਪਸ retval;
	}

	ਜੇ (ਪੱਤੀ_level > handle->max_ਡੂੰਘਾਈ) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("leaf level %d greater than tree depth %d\n",
			ਪੱਤੀ_level, handle->max_ਡੂੰਘਾਈ);
#ਜੇ_ਅੰਤ
		ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	ਲਿਖੋ("goto extent ino %u, level %d, %llu\n", handle->ino,
	       ਪੱਤੀ_level, blk);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#ਜੇ_ਅੰਤ
	ਜਦੋਂ (1) {
		ਜੇ (handle->max_ਡੂੰਘਾਈ - handle->level == ਪੱਤੀ_level) {
			/* block is in this &extent */
			ਜੇ ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				ਵਾਪਸ 0;
			ਜੇ (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				ਵਾਪਸ EXT2_ET_EXTENT_NOT_ਮਿਲਿਆ;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ਅਗਲਾ_SIB,
						   &extent);
			ਜੇ (retval == EXT2_ET_EXTENT_NO_ਅਗਲਾ)
				ਵਾਪਸ EXT2_ET_EXTENT_NOT_ਮਿਲਿਆ;
			ਜੇ (retval)
				ਵਾਪਸ retval;
			ਜਾਰੀ;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ਅਗਲਾ_SIB,
					   &extent);
		ਜੇ (retval == EXT2_ET_EXTENT_NO_ਅਗਲਾ)
			ਜਾਓ go_ਥੱਲੇ;
		ਜੇ (retval)
			ਵਾਪਸ retval;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#ਜੇ_ਅੰਤ
		ਜੇ (blk == extent.e_lblk)
			ਜਾਓ go_ਥੱਲੇ;
		ਜੇ (blk > extent.e_lblk)
			ਜਾਰੀ;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		ਜੇ (retval)
			ਵਾਪਸ retval;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#ਜੇ_ਅੰਤ

	go_ਥੱਲੇ:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ਥੱਲੇ,
					   &extent);
		ਜੇ (retval)
			ਵਾਪਸ retval;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#ਜੇ_ਅੰਤ
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	ਵਾਪਸ ext2fs_extent_goto2(handle, 0, blk);
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
	ਪੂਰਨ				retval = 0;
	ਪੂਰਨ				orig_ਉਚਾਈ;
	blk64_t				ਸ਼ੁਰੂ;
	ਬਣਤਰ extent_ਰਸਤਾ		*ਰਸਤਾ;
	ਬਣਤਰ ext2fs_extent		extent;
	ਬਣਤਰ ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ਜੇ (!(handle->fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	ਜੇ (!handle->ਰਸਤਾ)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

	ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;
	ਜੇ (!ਰਸਤਾ->curr)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ਜੇ (retval)
		ਜਾਓ ਪੂਰਾ;

	/* modified node's start block */
	ਸ਼ੁਰੂ = extent.e_lblk;

	ਜੇ ((retval = ext2fs_extent_get_info(handle, &info)))
		ਵਾਪਸ retval;
	orig_ਉਚਾਈ = info.max_ਡੂੰਘਾਈ - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	ਜਦੋਂ (handle->level > 0 &&
	       (ਰਸਤਾ->ਖੱਬਾ == ਰਸਤਾ->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ਉੱਪਰ, &extent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
		ਜੇ (extent.e_lblk == ਸ਼ੁਰੂ)
			ਰੋਕੋ;
		ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;
		extent.e_len += (extent.e_lblk - ਸ਼ੁਰੂ);
		extent.e_lblk = ਸ਼ੁਰੂ;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
		update_ਰਸਤਾ(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_ਉਚਾਈ, ਸ਼ੁਰੂ);
ਪੂਰਾ:
	ਵਾਪਸ retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
				ਬਣਤਰ ext2fs_extent *extent)
{
	ਬਣਤਰ extent_ਰਸਤਾ		*ਰਸਤਾ;
	ਬਣਤਰ ext3_extent_idx		*ix;
	ਬਣਤਰ ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ਜੇ (!(handle->fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	ਜੇ (!handle->ਰਸਤਾ)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

	ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;
	ਜੇ (!ਰਸਤਾ->curr)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	ਲਿਖੋ("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#ਜੇ_ਅੰਤ

	ਜੇ (handle->level == handle->max_ਡੂੰਘਾਈ) {
		ex = ਰਸਤਾ->curr;

		ex->ee_ਖੰਡ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(extent->e_lblk);
		ex->ee_ਸ਼ੁਰੂ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_ਸ਼ੁਰੂ_hi = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(extent->e_pblk >> 32);
		ਜੇ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			ਜੇ (extent->e_len > EXT_UNINIT_MAX_LEN)
				ਵਾਪਸ EXT2_ET_EXTENT_INVALID_ਲੰਮਾਈ;
			ex->ee_len = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} ਨਹੀਂਤਾਂ {
			ਜੇ (extent->e_len > EXT_INIT_MAX_LEN)
				ਵਾਪਸ EXT2_ET_EXTENT_INVALID_ਲੰਮਾਈ;
			ex->ee_len = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(extent->e_len);
		}
	} ਨਹੀਂਤਾਂ {
		ix = ਰਸਤਾ->curr;

		ix->ei_ਪੱਤੀ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_ਪੱਤੀ_hi = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(extent->e_pblk >> 32);
		ix->ei_ਖੰਡ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(extent->e_lblk);
		ix->ei_ਅਣਵਰਤਿਆ = 0;
	}
	update_ਰਸਤਾ(handle);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ splitting_at_eof(ਬਣਤਰ ext2_extent_handle *handle,
			    ਬਣਤਰ extent_ਰਸਤਾ *ਰਸਤਾ)
{
	ਬਣਤਰ extent_ਰਸਤਾ *ppp = ਰਸਤਾ;
	dump_ਰਸਤਾ(__func__, handle, ਰਸਤਾ);

	ਜੇ (handle->level == 0)
		ਵਾਪਸ 0;

	ਕਰੋ {
		ਜੇ (ppp->ਖੱਬਾ)
			ਵਾਪਸ 0;
		ppp--;
	} ਜਦੋਂ (ppp >= handle->ਰਸਤਾ);

	ਵਾਪਸ 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
ਸਥਿਰ errcode_t extent_ਗੰਢ_ਵੰਡੋ(ext2_extent_handle_t handle,
				   ਪੂਰਨ expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_ਗੰਢ_pblk;
	blk64_t				new_ਗੰਢ_ਸ਼ੁਰੂ;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	ਪੂਰਨ				orig_ਉਚਾਈ;
	ਅੱਖਰ				*ਖੰਡ_buf = ਨਿਰਾ;
	ਬਣਤਰ ext2fs_extent		extent;
	ਬਣਤਰ extent_ਰਸਤਾ		*ਰਸਤਾ, *newpath = 0;
	ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ	*eh, *neweh;
	ਪੂਰਨ				tocopy;
	ਪੂਰਨ				new_ਜੜ੍ਹ = 0;
	ਬਣਤਰ ext2_extent_info		info;
	ਪੂਰਨ				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ਜੇ (!(handle->fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	ਜੇ (!handle->ਰਸਤਾ)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	ਲਿਖੋ("splitting node at level %d\n", handle->level);
#ਜੇ_ਅੰਤ
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ਜੇ (retval)
		ਜਾਓ ਪੂਰਾ;

	retval = ext2fs_extent_get_info(handle, &info);
	ਜੇ (retval)
		ਜਾਓ ਪੂਰਾ;

	/* save the position we were originally splitting... */
	orig_ਉਚਾਈ = info.max_ਡੂੰਘਾਈ - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;
	eh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) ਰਸਤਾ->buf;
	ਜੇ (handle->level == handle->max_ਡੂੰਘਾਈ) {
		ਬਣਤਰ ext3_extent	*ex;

		ex = EXT_ਪਹਿਲਾ_EXTENT(eh);
		goal_blk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ex->ee_ਸ਼ੁਰੂ) +
			((__u64) ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ex->ee_ਸ਼ੁਰੂ_hi) << 32);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ ext3_extent_idx	*ix;

		ix = EXT_ਪਹਿਲਾ_ਸੂਚਕਾਂਕ(eh);
		goal_blk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਪੱਤੀ) +
			((__u64) ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ix->ei_ਪੱਤੀ_hi) << 32);
	}
	goal_blk -= EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(handle->fs);
	goal_blk &= ~EXT2_FS_CLUSTER_ਨਕਾਬ(handle->fs);

	/* Is there room in the parent for a new entry? */
	ਜੇ (handle->level &&
			(handle->ਰਸਤਾ[handle->level - 1].entries >=
			 handle->ਰਸਤਾ[handle->level - 1].max_entries)) {

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("parent level %d full; splitting it too\n",
							handle->level - 1);
#ਜੇ_ਅੰਤ
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ਉੱਪਰ, &extent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;

		retval = extent_ਗੰਢ_ਵੰਡੋ(handle, expand_allowed);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_ਉਚਾਈ, orig_lblk);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
	}

	/* At this point, parent should have room for this split */
	ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;
	ਜੇ (!ਰਸਤਾ->curr)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, ਰਸਤਾ) : 0;

	/* extent header of the current node we'll split */
	eh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *)ਰਸਤਾ->buf;

	/* splitting root level means moving them all out */
	ਜੇ (handle->level == 0) {
		new_ਜੜ੍ਹ = 1;
		tocopy = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    ਆਕਾਰ(ਬਣਤਰ extent_ਰਸਤਾ),
					    &newpath);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (no_balance)
			tocopy = 1;
		ਨਹੀਂਤਾਂ
			tocopy = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_entries) / 2;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	ਲਿਖੋ("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_entries),
				handle->level);
#ਜੇ_ਅੰਤ

	ਜੇ (!tocopy && !no_balance) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("Nothing to copy to new block!\n");
#ਜੇ_ਅੰਤ
		retval = EXT2_ET_CANT_ਵੰਡੋ_EXTENT;
		ਜਾਓ ਪੂਰਾ;
	}

	/* first we need a new block, or can do nothing. */
	ਖੰਡ_buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ(handle->fs->blocksize);
	ਜੇ (!ਖੰਡ_buf) {
		retval = ENOMEM;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (!goal_blk)
		goal_blk = ext2fs_ਲੱਭੋ_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, ਖੰਡ_buf,
				    &new_ਗੰਢ_pblk);
	ਜੇ (retval)
		ਜਾਓ ਪੂਰਾ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	ਲਿਖੋ("will copy to new node at block %lu\n",
	       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) new_ਗੰਢ_pblk);
#ਜੇ_ਅੰਤ

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) ਖੰਡ_buf;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(neweh, eh, ਆਕਾਰ(ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ));
	neweh->eh_entries = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16((handle->fs->blocksize -
			 ਆਕਾਰ(ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ)) /
				ਆਕਾਰ(ਬਣਤਰ ext3_extent));

	/* then the entries for the new block... */
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(EXT_ਪਹਿਲਾ_ਸੂਚਕਾਂਕ(neweh),
		EXT_ਪਹਿਲਾ_ਸੂਚਕਾਂਕ(eh) +
			(ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(eh->eh_entries) - tocopy),
		ਆਕਾਰ(ਬਣਤਰ ext3_extent_idx) * tocopy);

	new_ਗੰਢ_ਸ਼ੁਰੂ = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(EXT_ਪਹਿਲਾ_ਸੂਚਕਾਂਕ(neweh)->ei_ਖੰਡ);

	/* then update the checksum */
	retval = ext2fs_extent_ਖੰਡ_csum_ਸੈੱਟ(handle->fs, handle->ino, neweh);
	ਜੇ (retval)
		ਜਾਓ ਪੂਰਾ;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_ਗੰਢ_pblk, 1,
					ਖੰਡ_buf);

	ਜੇ (retval)
		ਜਾਓ ਪੂਰਾ;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	ਜੇ (handle->level == 0) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(newpath, ਰਸਤਾ,
		       ਆਕਾਰ(ਬਣਤਰ extent_ਰਸਤਾ) * handle->max_paths);
		handle->ਰਸਤਾ = newpath;
		newpath = ਰਸਤਾ;
		ਰਸਤਾ = handle->ਰਸਤਾ;
		ਰਸਤਾ->entries = 1;
		ਰਸਤਾ->ਖੱਬਾ = ਰਸਤਾ->max_entries - 1;
		handle->max_ਡੂੰਘਾਈ++;
		handle->max_paths++;
		eh->eh_ਡੂੰਘਾਈ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(handle->max_ਡੂੰਘਾਈ);
	} ਨਹੀਂਤਾਂ {
		ਰਸਤਾ->entries -= tocopy;
		ਰਸਤਾ->ਖੱਬਾ -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(ਰਸਤਾ->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_ਰਸਤਾ(handle);
	ਜੇ (retval)
		ਜਾਓ ਪੂਰਾ;

	/* now go up and insert/replace index for new node we created */
	ਜੇ (new_ਜੜ੍ਹ) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ਪਹਿਲਾ_SIB, &extent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;

		extent.e_lblk = new_ਗੰਢ_ਸ਼ੁਰੂ;
		extent.e_pblk = new_ਗੰਢ_pblk;
		extent.e_len = handle->ਰਸਤਾ[0].ਸਿਰਾ_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ {
		__u32 new_ਗੰਢ_ਲੰਮਾਈ;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ਉੱਪਰ, &extent);
		/* will insert after this one; it's length is shorter now */
		new_ਗੰਢ_ਲੰਮਾਈ = new_ਗੰਢ_ਸ਼ੁਰੂ - extent.e_lblk;
		extent.e_len -= new_ਗੰਢ_ਲੰਮਾਈ;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_ਗੰਢ_ਸ਼ੁਰੂ;
		extent.e_pblk = new_ਗੰਢ_pblk;
		extent.e_len = new_ਗੰਢ_ਲੰਮਾਈ;
		retval = ext2fs_extent_ਪਾਓ(handle, EXT2_EXTENT_ਪਾਓ_AFTER, &extent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_ਉਚਾਈ, orig_lblk);
	ਜੇ (retval)
		ਜਾਓ ਪੂਰਾ;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	ਜੇ (retval)
		ਜਾਓ ਪੂਰਾ;

ਪੂਰਾ:
	ਜੇ (newpath)
		ext2fs_free_mem(&newpath);
	ਮੁਕਤ(ਖੰਡ_buf);

	ਵਾਪਸ retval;
}

errcode_t ext2fs_extent_ਗੰਢ_ਵੰਡੋ(ext2_extent_handle_t handle)
{
	ਵਾਪਸ extent_ਗੰਢ_ਵੰਡੋ(handle, 0);
}

errcode_t ext2fs_extent_ਪਾਓ(ext2_extent_handle_t handle, ਪੂਰਨ flags,
				      ਬਣਤਰ ext2fs_extent *extent)
{
	ਬਣਤਰ extent_ਰਸਤਾ		*ਰਸਤਾ;
	ਬਣਤਰ ext3_extent_idx		*ix;
	ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ਜੇ (!(handle->fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	ਜੇ (!handle->ਰਸਤਾ)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	ਲਿਖੋ("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#ਜੇ_ਅੰਤ

	ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;

	ਜੇ (ਰਸਤਾ->entries >= ਰਸਤਾ->max_entries) {
		ਜੇ (flags & EXT2_EXTENT_ਪਾਓ_NOSPLIT) {
			ਵਾਪਸ EXT2_ET_CANT_ਪਾਓ_EXTENT;
		} ਨਹੀਂਤਾਂ {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
			ਲਿਖੋ("node full (level %d) - splitting\n",
				   handle->level);
#ਜੇ_ਅੰਤ
			retval = extent_ਗੰਢ_ਵੰਡੋ(handle, 1);
			ਜੇ (retval)
				ਵਾਪਸ retval;
			ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;
		}
	}

	eh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) ਰਸਤਾ->buf;
	ਜੇ (ਰਸਤਾ->curr) {
		ix = ਰਸਤਾ->curr;
		ਜੇ (flags & EXT2_EXTENT_ਪਾਓ_AFTER) {
			ix++;
			ਰਸਤਾ->ਖੱਬਾ--;
		}
	} ਨਹੀਂਤਾਂ {
		ix = EXT_ਪਹਿਲਾ_ਸੂਚਕਾਂਕ(eh);
		ਰਸਤਾ->ਖੱਬਾ = -1;
	}

	ਰਸਤਾ->curr = ix;

	ਜੇ (ਰਸਤਾ->ਖੱਬਾ >= 0)
		memmove(ix + 1, ix,
			(ਰਸਤਾ->ਖੱਬਾ+1) * ਆਕਾਰ(ਬਣਤਰ ext3_extent_idx));
	ਰਸਤਾ->ਖੱਬਾ++;
	ਰਸਤਾ->entries++;

	eh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) ਰਸਤਾ->buf;
	eh->eh_entries = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(ਰਸਤਾ->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	ਜੇ (retval)
		ਜਾਓ errout;

	retval = update_ਰਸਤਾ(handle);
	ਜੇ (retval)
		ਜਾਓ errout;

	ਵਾਪਸ 0;

errout:
	ext2fs_extent_delete(handle, 0);
	ਵਾਪਸ retval;
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

errcode_t ext2fs_extent_ਸੈੱਟ_bmap(ext2_extent_handle_t handle,
				 blk64_t logical, blk64_t physical, ਪੂਰਨ flags)
{
	errcode_t		ec, retval = 0;
	ਪੂਰਨ			mapped = 1; /* logical is mapped? */
	ਪੂਰਨ			orig_ਉਚਾਈ;
	ਪੂਰਨ			extent_uninit = 0;
	ਪੂਰਨ			prev_uninit = 0;
	ਪੂਰਨ			ਅਗਲਾ_uninit = 0;
	ਪੂਰਨ			new_uninit = 0;
	ਪੂਰਨ			max_len = EXT_INIT_MAX_LEN;
	ਪੂਰਨ			has_prev, has_ਅਗਲਾ;
	blk64_t			orig_lblk;
	ਬਣਤਰ extent_ਰਸਤਾ	*ਰਸਤਾ;
	ਬਣਤਰ ext2fs_extent	extent, ਅਗਲਾ_extent, prev_extent;
	ਬਣਤਰ ext2fs_extent	newextent;
	ਬਣਤਰ ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	ਲਿਖੋ("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#ਜੇ_ਅੰਤ

	ਜੇ (!(handle->fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	ਜੇ (!handle->ਰਸਤਾ)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

	ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;

	ਜੇ (flags & EXT2_EXTENT_ਸੈੱਟ_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	ਜੇ (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_ਪੱਤੀ;
		ਜੇ (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	ਜੇ ((handle->max_ਡੂੰਘਾਈ == 0) && (ਰਸਤਾ->entries == 0)) {
		retval = ext2fs_extent_ਪਾਓ(handle, 0, &newextent);
		ਵਾਪਸ retval;
	}

	/* save our original location in the extent tree */
	ਜੇ ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		ਜੇ (retval != EXT2_ET_NO_CURRENT_ਗੰਢ)
			ਵਾਪਸ retval;
		memset(&extent, 0, ਆਕਾਰ(extent));
	}
	ਜੇ ((retval = ext2fs_extent_get_info(handle, &info)))
		ਵਾਪਸ retval;
	orig_ਉਚਾਈ = info.max_ਡੂੰਘਾਈ - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	ਜੇ (retval) {
		ਜੇ (retval == EXT2_ET_EXTENT_NOT_ਮਿਲਿਆ) {
			retval = 0;
			mapped = 0;
			ਜੇ (!physical) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
				ਲਿਖੋ("block %llu already unmapped\n",
					logical);
#ਜੇ_ਅੰਤ
				ਜਾਓ ਪੂਰਾ;
			}
		} ਨਹੀਂਤਾਂ
			ਜਾਓ ਪੂਰਾ;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ਜੇ (retval)
		ਜਾਓ ਪੂਰਾ;
	ਜੇ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ, &ਅਗਲਾ_extent);
	ਜੇ (retval) {
		has_ਅਗਲਾ = 0;
		ਜੇ (retval != EXT2_ET_EXTENT_NO_ਅਗਲਾ)
			ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ {
		dbg_print_extent("set_bmap: next_extent",
				 &ਅਗਲਾ_extent);
		has_ਅਗਲਾ = 1;
		ਜੇ (ਅਗਲਾ_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			ਅਗਲਾ_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ਜੇ (retval && retval != EXT2_ET_EXTENT_NOT_ਮਿਲਿਆ)
		ਜਾਓ ਪੂਰਾ;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_ਪੱਤੀ, &prev_extent);
	ਜੇ (retval) {
		has_prev = 0;
		ਜੇ (retval != EXT2_ET_EXTENT_NO_PREV)
			ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		ਜੇ (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ਜੇ (retval && retval != EXT2_ET_EXTENT_NOT_ਮਿਲਿਆ)
		ਜਾਓ ਪੂਰਾ;

	/* check if already pointing to the requested physical */
	ਜੇ (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("physical block (at %llu) unchanged\n", logical);
#ਜੇ_ਅੰਤ
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (!mapped) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("mapping unmapped logical block %llu\n", logical);
#ਜੇ_ਅੰਤ
		ਜੇ ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((ਪੂਰਨ) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ਨਹੀਂਤਾਂ ਜੇ ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((ਪੂਰਨ) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ਨਹੀਂਤਾਂ ਜੇ (has_ਅਗਲਾ &&
			   (logical == ਅਗਲਾ_extent.e_lblk - 1) &&
			   (physical == ਅਗਲਾ_extent.e_pblk - 1) &&
			   (new_uninit == ਅਗਲਾ_uninit) &&
			   ((ਪੂਰਨ) ਅਗਲਾ_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ,
						   &ਅਗਲਾ_extent);
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
			ਅਗਲਾ_extent.e_len++;
			ਅਗਲਾ_extent.e_lblk--;
			ਅਗਲਾ_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &ਅਗਲਾ_extent);
		} ਨਹੀਂਤਾਂ ਜੇ (logical < extent.e_lblk)
			retval = ext2fs_extent_ਪਾਓ(handle, 0, &newextent);
		ਨਹੀਂਤਾਂ
			retval = ext2fs_extent_ਪਾਓ(handle,
				      EXT2_EXTENT_ਪਾਓ_AFTER, &newextent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
		retval = ext2fs_extent_fix_parents(handle);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ ਜੇ ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("(re/un)mapping only block in extent\n");
#ਜੇ_ਅੰਤ
		ਜੇ (physical) {
			ਜੇ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((ਪੂਰਨ) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_ਪੱਤੀ, &prev_extent);
				ਜੇ (retval)
					ਜਾਓ ਪੂਰਾ;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ,
							   &extent);
				ਜੇ (retval)
					ਜਾਓ ਪੂਰਾ;
				ਜਾਓ delete_ਗੰਢ;

			} ਨਹੀਂਤਾਂ
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} ਨਹੀਂਤਾਂ {
		delete_ਗੰਢ:
			retval = ext2fs_extent_delete(handle, 0);
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
			ec = ext2fs_extent_fix_parents(handle);
			ਜੇ (ec != EXT2_ET_NO_CURRENT_ਗੰਢ)
				retval = ec;
		}

		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (logical == extent.e_lblk + extent.e_len - 1)  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("(re/un)mapping last block in extent\n");
#ਜੇ_ਅੰਤ
		ਜੇ (physical) {
			ਜੇ (has_ਅਗਲਾ &&
			    (logical == (ਅਗਲਾ_extent.e_lblk - 1)) &&
			    (physical == (ਅਗਲਾ_extent.e_pblk - 1)) &&
			    (new_uninit == ਅਗਲਾ_uninit) &&
			    ((ਪੂਰਨ) ਅਗਲਾ_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ, &ਅਗਲਾ_extent);
				ਜੇ (retval)
					ਜਾਓ ਪੂਰਾ;
				ਅਗਲਾ_extent.e_len++;
				ਅਗਲਾ_extent.e_lblk--;
				ਅਗਲਾ_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &ਅਗਲਾ_extent);
				ਜੇ (retval)
					ਜਾਓ ਪੂਰਾ;
			} ਨਹੀਂਤਾਂ
				retval = ext2fs_extent_ਪਾਓ(handle,
				      EXT2_EXTENT_ਪਾਓ_AFTER, &newextent);
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
			retval = ext2fs_extent_fix_parents(handle);
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (logical == extent.e_lblk) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("(re/un)mapping first block in extent\n");
#ਜੇ_ਅੰਤ
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
		retval = ext2fs_extent_fix_parents(handle);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
		ਜੇ (physical) {
			ਜੇ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((ਪੂਰਨ) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_ਪੱਤੀ, &prev_extent);
				ਜੇ (retval)
					ਜਾਓ ਪੂਰਾ;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} ਨਹੀਂਤਾਂ
				retval = ext2fs_extent_ਪਾਓ(handle,
							      0, &newextent);
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
			retval = ext2fs_extent_fix_parents(handle);
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ,
						   &extent);
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
		}
	} ਨਹੀਂਤਾਂ {
		__u32	save_ਲੰਮਾਈ;
		blk64_t	save_lblk;
		ਬਣਤਰ ext2fs_extent save_extent;
		errcode_t r2;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
		ਲਿਖੋ("(re/un)mapping in middle of extent\n");
#ਜੇ_ਅੰਤ
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_ਲੰਮਾਈ = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
		/* insert our new extent, if any */
		ਜੇ (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_ਪਾਓ(handle,
					EXT2_EXTENT_ਪਾਓ_AFTER, &newextent);
			ਜੇ (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				ਜੇ (r2 == 0)
					(ਖਾਲੀ)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				ਜਾਓ ਪੂਰਾ;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_ਲੰਮਾਈ - extent.e_len - 1;
		retval = ext2fs_extent_ਪਾਓ(handle,
				EXT2_EXTENT_ਪਾਓ_AFTER, &extent);
		ਜੇ (retval) {
			ਜੇ (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				ਜੇ (r2 == 0)
					(ਖਾਲੀ)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			ਜੇ (r2 == 0)
				(ਖਾਲੀ)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			ਜਾਓ ਪੂਰਾ;
		}
	}

ਪੂਰਾ:
	/* get handle back to its position */
	ਜੇ (orig_ਉਚਾਈ > handle->max_ਡੂੰਘਾਈ)
		orig_ਉਚਾਈ = handle->max_ਡੂੰਘਾਈ; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_ਉਚਾਈ, orig_lblk);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, ਪੂਰਨ flags)
{
	ਬਣਤਰ extent_ਰਸਤਾ		*ਰਸਤਾ;
	ਅੱਖਰ 				*cp;
	ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ਜੇ (!(handle->fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	ਜੇ (!handle->ਰਸਤਾ)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	{
		ਬਣਤਰ ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		ਜੇ (retval == 0) {
			ਲਿਖੋ("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#ਜੇ_ਅੰਤ

	ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;
	ਜੇ (!ਰਸਤਾ->curr)
		ਵਾਪਸ EXT2_ET_NO_CURRENT_ਗੰਢ;

	cp = ਰਸਤਾ->curr;

	/* Sanity check before memmove() */
	ਜੇ (ਰਸਤਾ->ਖੱਬਾ < 0)
		ਵਾਪਸ EXT2_ET_EXTENT_ਪੱਤੀ_BAD;

	ਜੇ (ਰਸਤਾ->ਖੱਬਾ) {
		memmove(cp, cp + ਆਕਾਰ(ਬਣਤਰ ext3_extent_idx),
			ਰਸਤਾ->ਖੱਬਾ * ਆਕਾਰ(ਬਣਤਰ ext3_extent_idx));
		ਰਸਤਾ->ਖੱਬਾ--;
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ ext3_extent_idx	*ix = ਰਸਤਾ->curr;
		ix--;
		ਰਸਤਾ->curr = ix;
	}
	ਜੇ (--ਰਸਤਾ->entries == 0)
		ਰਸਤਾ->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	ਜੇ (ਰਸਤਾ->entries == 0 && handle->level) {
		ਜੇ (!(flags & EXT2_EXTENT_DELETE_KEEP_ਖਾਲੀ_ਥਾਵਾਂ)) {
			ਬਣਤਰ ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_ਉੱਪਰ,
								&extent);
			ਜੇ (retval)
				ਵਾਪਸ retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_ਖੰਡਾਂ -=
				(handle->fs->blocksize *
				 EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_ਖੰਡ_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} ਨਹੀਂਤਾਂ {
		eh = (ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ *) ਰਸਤਾ->buf;
		eh->eh_entries = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(ਰਸਤਾ->entries);
		ਜੇ ((ਰਸਤਾ->entries == 0) && (handle->level == 0)) {
			eh->eh_ਡੂੰਘਾਈ = 0;
			handle->max_ਡੂੰਘਾਈ = 0;
		}
		retval = update_ਰਸਤਾ(handle);
	}
	ਵਾਪਸ retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 ਬਣਤਰ ext2_extent_info *info)
{
	ਬਣਤਰ extent_ਰਸਤਾ		*ਰਸਤਾ;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, ਆਕਾਰ(ਬਣਤਰ ext2_extent_info));

	ਰਸਤਾ = handle->ਰਸਤਾ + handle->level;
	ਜੇ (ਰਸਤਾ) {
		ਜੇ (ਰਸਤਾ->curr)
			info->curr_ਇੰਦਰਾਜ = ((ਅੱਖਰ *) ਰਸਤਾ->curr - ਰਸਤਾ->buf) /
				ਆਕਾਰ(ਬਣਤਰ ext3_extent_idx);
		ਨਹੀਂਤਾਂ
			info->curr_ਇੰਦਰਾਜ = 0;
		info->ਅੰਕ_entries = ਰਸਤਾ->entries;
		info->max_entries = ਰਸਤਾ->max_entries;
		info->ਬਾਈਟਾਂ_avail = (ਰਸਤਾ->max_entries - ਰਸਤਾ->entries) *
			ਆਕਾਰ(ਬਣਤਰ ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_ਡੂੰਘਾਈ = handle->max_ਡੂੰਘਾਈ;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	ਵਾਪਸ 0;
}

size_t ext2fs_max_extent_ਡੂੰਘਾਈ(ext2_extent_handle_t handle)
{
	size_t iblock_sz = ਆਕਾਰ(((ਬਣਤਰ ext2_inode *)ਨਿਰਾ)->i_ਖੰਡ);
	size_t iblock_extents = (iblock_sz - ਆਕਾਰ(ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ)) /
				ਆਕਾਰ(ਬਣਤਰ ext3_extent);
	size_t extents_ਪ੍ਰਤੀ_ਖੰਡ = (handle->fs->blocksize -
				    ਆਕਾਰ(ਬਣਤਰ ext3_extent_ਸਿਰਲੇਖ)) /
				   ਆਕਾਰ(ਬਣਤਰ ext3_extent);
	ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_blocksize = 0;
	ਸਥਿਰ size_t ਆਖਰੀ_ਨਤੀਜਾ = 0;

	ਜੇ (ਆਖਰੀ_blocksize && ਆਖਰੀ_blocksize == handle->fs->blocksize)
		ਵਾਪਸ ਆਖਰੀ_ਨਤੀਜਾ;

	ਆਖਰੀ_ਨਤੀਜਾ = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_ਪ੍ਰਤੀ_ਖੰਡ));
	ਆਖਰੀ_blocksize = handle->fs->blocksize;
	ਵਾਪਸ ਆਖਰੀ_ਨਤੀਜਾ;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       ਬਣਤਰ ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	ਬਣਤਰ ext2fs_extent	extent;
	errcode_t		errcode;
	ਪੂਰਨ    			save_flags = fs->flags;

	ਜੇ (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		ਵਾਪਸ 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ਜੇ (errcode) {
		ਜੇ (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		ਵਾਪਸ errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ਜੜ੍ਹ, &extent);
	ਜੇ (errcode)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਕਰੋ {
		/* Skip to the end of a block of leaf nodes */
		ਜੇ (extent.e_flags & EXT2_EXTENT_FLAGS_ਪੱਤੀ) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_ਆਖਰੀ_SIB,
						    &extent);
			ਜੇ (errcode)
				ਰੋਕੋ;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ਅਗਲਾ, &extent);
		ਜੇ (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_ਰਸਤਾ(handle);
	} ਜਦੋਂ (errcode == 0);

ਬਾਹਰਲਾ:
	/* Ok if we run off the end */
	ਜੇ (errcode == EXT2_ET_EXTENT_NO_ਅਗਲਾ)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	ਵਾਪਸ errcode;
}

errcode_t ext2fs_decode_extent(ਬਣਤਰ ext2fs_extent *ਮੰਜ਼ਿਲ, ਖਾਲੀ *addr, ਪੂਰਨ len)
{
	ਬਣਤਰ ext3_extent *from = (ਬਣਤਰ ext3_extent *)addr;

	ਜੇ (len != ਆਕਾਰ(ਬਣਤਰ ext3_extent))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਮੰਜ਼ਿਲ->e_pblk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(from->ee_ਸ਼ੁਰੂ) +
		((__u64) ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(from->ee_ਸ਼ੁਰੂ_hi)
			<< 32);
	ਮੰਜ਼ਿਲ->e_lblk = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(from->ee_ਖੰਡ);
	ਮੰਜ਼ਿਲ->e_len = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(from->ee_len);
	ਮੰਜ਼ਿਲ->e_flags = EXT2_EXTENT_FLAGS_ਪੱਤੀ;
	ਜੇ (ਮੰਜ਼ਿਲ->e_len > EXT_INIT_MAX_LEN) {
		ਮੰਜ਼ਿਲ->e_len -= EXT_INIT_MAX_LEN;
		ਮੰਜ਼ਿਲ->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	ਵਾਪਸ 0;
}

errcode_t ext2fs_ਗਿਣਤੀ_ਖੰਡਾਂ(ext2_filsys fs, ext2_ino_t ino,
			      ਬਣਤਰ ext2_inode *inode, blk64_t *ret_ਗਿਣਤੀ)
{
	ext2_extent_handle_t	handle = ਨਿਰਾ;
	ਬਣਤਰ ext2fs_extent	extent;
	errcode_t		errcode;
	ਪੂਰਨ			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ਜੇ (errcode)
		ਜਾਓ ਬਾਹਰਲਾ;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ਜੜ੍ਹ, &extent);
	ਜੇ (errcode)
		ਜਾਓ ਬਾਹਰਲਾ;

	errcode = ext2fs_get_array(handle->max_ਡੂੰਘਾਈ, ਆਕਾਰ(blk64_t),
				   &intermediate_nodes);
	ਜੇ (errcode)
		ਜਾਓ ਬਾਹਰਲਾ;

	blkcount = handle->level;
	ਜਦੋਂ (!errcode) {
		ਜੇ (extent.e_flags & EXT2_EXTENT_FLAGS_ਪੱਤੀ) {
			blkcount += extent.e_len;
			ਲਈ (i = 0; i < handle->level; i++) {
				ਜੇ (intermediate_nodes[i] !=
					handle->ਰਸਤਾ[i].ਸਿਰਾ_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->ਰਸਤਾ[i].ਸਿਰਾ_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ਅਗਲਾ, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
ਬਾਹਰਲਾ:
	*ret_ਗਿਣਤੀ = blkcount;
	ext2fs_extent_free(handle);

	ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
/*
 * Override debugfs's prompt
 */
ਅਚਲ ਅੱਖਰ *debug_prog_name = "tst_extents";

#ਜੇ_ਅੰਤ

