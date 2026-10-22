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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"
#అన్తర్భావ "e2image.h"

#నిర్వచనం_రద్దు DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

నిర్మాణం extent_మార్గం {
	అక్షరం		*buf;
	పూర్ణాంకం		entries;
	పూర్ణాంకం		max_entries;
	పూర్ణాంకం		ఎడమ;
	పూర్ణాంకం		సందర్శించు_అంకె;
	పూర్ణాంకం		flags;
	blk64_t		చివర_blk;
	blk64_t		blk;
	శూన్య		*curr;
};


నిర్మాణం ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	నిర్మాణం ext2_inode	*inode;
	నిర్మాణం ext2_inode	inodebuf;
	పూర్ణాంకం			type;
	పూర్ణాంకం			level;
	పూర్ణాంకం			max_లోతు;
	పూర్ణాంకం			max_paths;
	నిర్మాణం extent_మార్గం	*మార్గం;
};

నిర్మాణం ext2_extent_మార్గం {
	errcode_t		magic;
	పూర్ణాంకం			ఆకు_ఎత్తు;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#నిర్వచితమైతే DEBUG
స్థిర శూన్య dbg_show_శీర్షిక(నిర్మాణం ext3_extent_శీర్షిక *eh)
{
	రాయి("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_గమ్యం_cpu(eh->eh_magic),
			ext2fs_le16_గమ్యం_cpu(eh->eh_entries),
			ext2fs_le16_గమ్యం_cpu(eh->eh_max),
			ext2fs_le16_గమ్యం_cpu(eh->eh_లోతు),
			ext2fs_le32_గమ్యం_cpu(eh->eh_generation));
}

స్థిర శూన్య dbg_show_సూచీ(నిర్మాణం ext3_extent_idx *ix)
{
	రాయి("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_గమ్యం_cpu(ix->ei_ఖండం),
			ext2fs_le32_గమ్యం_cpu(ix->ei_ఆకు),
			ext2fs_le16_గమ్యం_cpu(ix->ei_ఆకు_hi),
			ext2fs_le16_గమ్యం_cpu(ix->ei_వాడని));
}

స్థిర శూన్య dbg_show_extent(నిర్మాణం ext3_extent *ex)
{
	రాయి("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_గమ్యం_cpu(ex->ee_ఖండం),
			ext2fs_le32_గమ్యం_cpu(ex->ee_ఖండం) +
			ext2fs_le16_గమ్యం_cpu(ex->ee_len) - 1,
			ext2fs_le16_గమ్యం_cpu(ex->ee_len),
			ext2fs_le32_గమ్యం_cpu(ex->ee_ప్రారంభం),
			ext2fs_le16_గమ్యం_cpu(ex->ee_ప్రారంభం_hi));
}

స్థిర శూన్య dbg_print_extent(అక్షరం *desc, నిర్మాణం ext2fs_extent *extent)
{
	ఒకవేళ (desc)
		రాయి("%s: ", desc);
	రాయి("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ఒకవేళ (extent->e_flags & EXT2_EXTENT_FLAGS_ఆకు)
		fputs("LEAF ", stdout);
	ఒకవేళ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ఒకవేళ (extent->e_flags & EXT2_EXTENT_FLAGS_రెండవ_సందర్శించు)
		fputs("2ND_VISIT ", stdout);
	ఒకవేళ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

స్థిర శూన్య dump_మార్గం(స్థిరరాశి అక్షరం *ట్యాగ్, నిర్మాణం ext2_extent_handle *handle,
		      నిర్మాణం extent_మార్గం *మార్గం)
{
	నిర్మాణం extent_మార్గం *ppp = మార్గం;
	రాయి("%s: level=%d\n", ట్యాగ్, handle->level);

	చేయి {
		రాయి("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       ట్యాగ్, (ppp - handle->మార్గం), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->ఎడమ, ppp->సందర్శించు_అంకె, ppp->flags,
		       ppp->చివర_blk, ppp->curr, ppp->curr - (శూన్య *)ppp->buf);
		రాయి("  ");
		dbg_show_శీర్షిక((నిర్మాణం ext3_extent_శీర్షిక *)ppp->buf);
		ఒకవేళ (ppp->curr) {
			రాయి("  ");
			dbg_show_సూచీ(ppp->curr);
			రాయి("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} అయితే (ppp >= handle->మార్గం);
	fflush(stdout);

	తిరిగి;
}

#లేకపోతే
#నిర్వచించు dbg_show_శీర్షిక(eh) చేయి { } అయితే (0)
#నిర్వచించు dbg_show_సూచీ(ix) చేయి { } అయితే (0)
#నిర్వచించు dbg_show_extent(ex) చేయి { } అయితే (0)
#నిర్వచించు dbg_print_extent(desc, ex) చేయి { } అయితే (0)
#నిర్వచించు dump_మార్గం(ట్యాగ్, handle, మార్గం) చేయి { } అయితే (0)
#అయితే_ముగింపు

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_శీర్షిక_verify(శూన్య *ptr, పూర్ణాంకం కొలత)
{
	పూర్ణాంకం eh_max, నమోదు_కొలత;
	నిర్మాణం ext3_extent_శీర్షిక *eh = ptr;

	dbg_show_శీర్షిక(eh);
	ఒకవేళ (ext2fs_le16_గమ్యం_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		తిరిగి EXT2_ET_EXTENT_శీర్షిక_BAD;
	ఒకవేళ (ext2fs_le16_గమ్యం_cpu(eh->eh_entries) > ext2fs_le16_గమ్యం_cpu(eh->eh_max))
		తిరిగి EXT2_ET_EXTENT_శీర్షిక_BAD;
	ఒకవేళ (eh->eh_లోతు == 0)
		నమోదు_కొలత = పరిమాణం(నిర్మాణం ext3_extent);
	లేకపోతే
		నమోదు_కొలత = పరిమాణం(నిర్మాణం ext3_extent_idx);

	eh_max = (కొలత - పరిమాణం(*eh)) / నమోదు_కొలత;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	ఒకవేళ ((ext2fs_le16_గమ్యం_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_గమ్యం_cpu(eh->eh_max) < (eh_max - 2)))
		తిరిగి EXT2_ET_EXTENT_శీర్షిక_BAD;

	తిరిగి 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
శూన్య ext2fs_extent_free(ext2_extent_handle_t handle)
{
	పూర్ణాంకం			i;

	ఒకవేళ (!handle)
		తిరిగి;

	ఒకవేళ (handle->మార్గం) {
		కొరకు (i = 1; i < handle->max_paths; i++) {
			ఒకవేళ (handle->మార్గం[i].buf)
				ext2fs_free_mem(&handle->మార్గం[i].buf);
		}
		ext2fs_free_mem(&handle->మార్గం);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	తిరిగి ext2fs_extent_open2(fs, ino, ఖాళీ, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    నిర్మాణం ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	నిర్మాణం ext2_extent_handle	*handle;
	errcode_t			retval;
	పూర్ణాంకం				i;
	నిర్మాణం ext3_extent_శీర్షిక	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!inode)
		ఒకవేళ ((ino == 0) || (ino > fs->super->s_inodes_లెక్క))
			తిరిగి EXT2_ET_BAD_INODE_అంకె;

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_extent_handle), &handle);
	ఒకవేళ (retval)
		తిరిగి retval;
	memset(handle, 0, పరిమాణం(నిర్మాణం ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	ఒకవేళ (inode) {
		handle->inode = inode;
	} లేకపోతే {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}

	eh = (నిర్మాణం ext3_extent_శీర్షిక *) &handle->inode->i_ఖండం[0];

	కొరకు (i=0; i < EXT2_N_ఖండాలు; i++)
		ఒకవేళ (handle->inode->i_ఖండం[i])
			ఆపు;
	ఒకవేళ (i >= EXT2_N_ఖండాలు) {
		eh->eh_magic = ext2fs_cpu_గమ్యం_le16(EXT3_EXT_MAGIC);
		eh->eh_లోతు = 0;
		eh->eh_entries = 0;
		i = (పరిమాణం(handle->inode->i_ఖండం) - పరిమాణం(*eh)) /
			పరిమాణం(నిర్మాణం ext3_extent);
		eh->eh_max = ext2fs_cpu_గమ్యం_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	ఒకవేళ (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		వెళ్ళు errout;
	}

	retval = ext2fs_extent_శీర్షిక_verify(eh, పరిమాణం(handle->inode->i_ఖండం));
	ఒకవేళ (retval)
		వెళ్ళు errout;

	handle->max_లోతు = ext2fs_le16_గమ్యం_cpu(eh->eh_లోతు);
	handle->type = ext2fs_le16_గమ్యం_cpu(eh->eh_magic);

	handle->max_paths = handle->max_లోతు + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    పరిమాణం(నిర్మాణం extent_మార్గం),
				    &handle->మార్గం);
	handle->మార్గం[0].buf = (అక్షరం *) handle->inode->i_ఖండం;

	handle->మార్గం[0].ఎడమ = handle->మార్గం[0].entries =
		ext2fs_le16_గమ్యం_cpu(eh->eh_entries);
	handle->మార్గం[0].max_entries = ext2fs_le16_గమ్యం_cpu(eh->eh_max);
	handle->మార్గం[0].curr = 0;
	handle->మార్గం[0].చివర_blk =
		(EXT2_I_కొలత(handle->inode) + fs->blocksize - 1) >>
		 EXT2_ఖండం_కొలత_బిట్లు(fs->super);
	handle->మార్గం[0].blk = 0;
	handle->మార్గం[0].సందర్శించు_అంకె = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	తిరిగి 0;

errout:
	ext2fs_extent_free(handle);
	తిరిగి retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    పూర్ణాంకం flags, నిర్మాణం ext2fs_extent *extent)
{
	నిర్మాణం extent_మార్గం	*మార్గం, *newpath, *tp;
	నిర్మాణం ext3_extent_శీర్షిక	*eh;
	నిర్మాణం ext3_extent_idx		*ix = 0;
	నిర్మాణం ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				చివర_blk;
	పూర్ణాంకం				orig_op, op, l;
	పూర్ణాంకం				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ఒకవేళ (!handle->మార్గం)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

	orig_op = op = flags & EXT2_EXTENT_MOVE_ముసుగు;

retry:
	మార్గం = handle->మార్గం + handle->level;
	ఒకవేళ ((orig_op == EXT2_EXTENT_తదుపరి) ||
	    (orig_op == EXT2_EXTENT_తదుపరి_ఆకు)) {
		ఒకవేళ (handle->level < handle->max_లోతు) {
			/* interior node */
			ఒకవేళ (మార్గం->సందర్శించు_అంకె == 0) {
				మార్గం->సందర్శించు_అంకె++;
				op = EXT2_EXTENT_కిందికి;
			} లేకపోతే ఒకవేళ (మార్గం->ఎడమ > 0)
				op = EXT2_EXTENT_తదుపరి_SIB;
			లేకపోతే ఒకవేళ (handle->level > 0)
				op = EXT2_EXTENT_పైకి;
			లేకపోతే
				తిరిగి EXT2_ET_EXTENT_NO_తదుపరి;
		} లేకపోతే {
			/* leaf node */
			ఒకవేళ (మార్గం->ఎడమ > 0)
				op = EXT2_EXTENT_తదుపరి_SIB;
			లేకపోతే ఒకవేళ (handle->level > 0)
				op = EXT2_EXTENT_పైకి;
			లేకపోతే
				తిరిగి EXT2_ET_EXTENT_NO_తదుపరి;
		}
		ఒకవేళ (op != EXT2_EXTENT_తదుపరి_SIB) {
#నిర్వచితమైతే DEBUG_GET_EXTENT
			రాయి("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_కిందికి) ? "down" :
			       ((op == EXT2_EXTENT_పైకి) ? "up" : "unknown"));
#అయితే_ముగింపు
		}
	}

	ఒకవేళ ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_ఆకు)) {
		ఒకవేళ (handle->level < handle->max_లోతు) {
			/* interior node */
			ఒకవేళ (మార్గం->సందర్శించు_అంకె > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_కిందికి_AND_చివరి;
			} లేకపోతే ఒకవేళ (మార్గం->ఎడమ < మార్గం->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			లేకపోతే ఒకవేళ (handle->level > 0)
				op = EXT2_EXTENT_పైకి;
			లేకపోతే
				తిరిగి EXT2_ET_EXTENT_NO_PREV;
		} లేకపోతే {
			/* leaf node */
			ఒకవేళ (మార్గం->ఎడమ < మార్గం->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			లేకపోతే ఒకవేళ (handle->level > 0)
				op = EXT2_EXTENT_పైకి;
			లేకపోతే
				తిరిగి EXT2_ET_EXTENT_NO_PREV;
		}
		ఒకవేళ (op != EXT2_EXTENT_PREV_SIB) {
#నిర్వచితమైతే DEBUG_GET_EXTENT
			రాయి("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_కిందికి_AND_చివరి) ? "down/last" :
			       ((op == EXT2_EXTENT_పైకి) ? "up" : "unknown"));
#అయితే_ముగింపు
		}
	}

	ఒకవేళ (orig_op == EXT2_EXTENT_చివరి_ఆకు) {
		ఒకవేళ ((handle->level < handle->max_లోతు) &&
		    (మార్గం->ఎడమ == 0))
			op = EXT2_EXTENT_కిందికి;
		లేకపోతే
			op = EXT2_EXTENT_చివరి_SIB;
#నిర్వచితమైతే DEBUG_GET_EXTENT
		రాయి("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_కిందికి) ? "down" : "last_sib");
#అయితే_ముగింపు
	}

	ఎంపిక (op) {
	స్థితి EXT2_EXTENT_CURRENT:
		ix = మార్గం->curr;
		ఆపు;
	స్థితి EXT2_EXTENT_మూలం:
		handle->level = 0;
		మార్గం = handle->మార్గం + handle->level;
		/* fallthrough */
	స్థితి EXT2_EXTENT_మొదటి_SIB:
		మార్గం->ఎడమ = మార్గం->entries;
		మార్గం->curr = 0;
		/* fallthrough */
	స్థితి EXT2_EXTENT_తదుపరి_SIB:
		ఒకవేళ (మార్గం->ఎడమ <= 0)
			తిరిగి EXT2_ET_EXTENT_NO_తదుపరి;
		ఒకవేళ (మార్గం->curr) {
			ix = మార్గం->curr;
			ix++;
		} లేకపోతే {
			eh = (నిర్మాణం ext3_extent_శీర్షిక *) మార్గం->buf;
			ix = EXT_మొదటి_సూచీ(eh);
		}
		మార్గం->ఎడమ--;
		మార్గం->curr = ix;
		మార్గం->సందర్శించు_అంకె = 0;
		ఆపు;
	స్థితి EXT2_EXTENT_PREV_SIB:
		ఒకవేళ (!మార్గం->curr ||
		    మార్గం->ఎడమ+1 >= మార్గం->entries)
			తిరిగి EXT2_ET_EXTENT_NO_PREV;
		ix = మార్గం->curr;
		ix--;
		మార్గం->curr = ix;
		మార్గం->ఎడమ++;
		ఒకవేళ (handle->level < handle->max_లోతు)
			మార్గం->సందర్శించు_అంకె = 1;
		ఆపు;
	స్థితి EXT2_EXTENT_చివరి_SIB:
		eh = (నిర్మాణం ext3_extent_శీర్షిక *) మార్గం->buf;
		మార్గం->curr = EXT_చివరి_EXTENT(eh);
		ix = మార్గం->curr;
		మార్గం->ఎడమ = 0;
		మార్గం->సందర్శించు_అంకె = 0;
		ఆపు;
	స్థితి EXT2_EXTENT_పైకి:
		ఒకవేళ (handle->level <= 0)
			తిరిగి EXT2_ET_EXTENT_NO_పైకి;
		handle->level--;
		మార్గం--;
		ix = మార్గం->curr;
		ఒకవేళ ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_ఆకు))
			మార్గం->సందర్శించు_అంకె = 0;
		ఆపు;
	స్థితి EXT2_EXTENT_కిందికి:
	స్థితి EXT2_EXTENT_కిందికి_AND_చివరి:
		ఒకవేళ (!మార్గం->curr ||(handle->level >= handle->max_లోతు))
			తిరిగి EXT2_ET_EXTENT_NO_కిందికి;

		ix = మార్గం->curr;
		newpath = మార్గం + 1;
		ఒకవేళ (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			ఒకవేళ (retval)
				తిరిగి retval;
		}
		blk = ext2fs_le32_గమ్యం_cpu(ix->ei_ఆకు) +
			((__u64) ext2fs_le16_గమ్యం_cpu(ix->ei_ఆకు_hi) << 32);
		కొరకు (l = handle->level, tp = మార్గం; l > 0; l--, tp--) {
			ఒకవేళ (blk == tp->blk)
				తిరిగి EXT2_ET_EXTENT_చక్రం;
		}
		newpath->blk = blk;
		ఒకవేళ ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		లేకపోతే {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			ఒకవేళ (retval)
				తిరిగి retval;
		}
		handle->level++;

		eh = (నిర్మాణం ext3_extent_శీర్షిక *) newpath->buf;

		retval = ext2fs_extent_శీర్షిక_verify(eh, handle->fs->blocksize);
		ఒకవేళ (retval) {
			handle->level--;
			తిరిగి retval;
		}

		ఒకవేళ (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_ఖండం_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->ఎడమ = newpath->entries =
			ext2fs_le16_గమ్యం_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_గమ్యం_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		ఒకవేళ (newpath->ఎడమ <= 0)
			తిరిగి EXT2_ET_EXTENT_NO_కిందికి;

		ఒకవేళ (మార్గం->ఎడమ > 0) {
			ix++;
			newpath->చివర_blk = ext2fs_le32_గమ్యం_cpu(ix->ei_ఖండం);
		} లేకపోతే
			newpath->చివర_blk = మార్గం->చివర_blk;

		మార్గం = newpath;
		ఒకవేళ (op == EXT2_EXTENT_కిందికి) {
			ix = EXT_మొదటి_సూచీ((నిర్మాణం ext3_extent_శీర్షిక *) eh);
			మార్గం->curr = ix;
			మార్గం->ఎడమ = మార్గం->entries - 1;
			మార్గం->సందర్శించు_అంకె = 0;
		} లేకపోతే {
			ix = EXT_చివరి_సూచీ((నిర్మాణం ext3_extent_శీర్షిక *) eh);
			మార్గం->curr = ix;
			మార్గం->ఎడమ = 0;
			ఒకవేళ (handle->level < handle->max_లోతు)
				మార్గం->సందర్శించు_అంకె = 1;
		}
#నిర్వచితమైతే DEBUG_GET_EXTENT
		రాయి("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_లోతు,
			   మార్గం->చివర_blk);
#అయితే_ముగింపు
		ఆపు;
	అప్రమేయ:
		తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
	}

	ఒకవేళ (!ix)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

	extent->e_flags = 0;
#నిర్వచితమైతే DEBUG_GET_EXTENT
	రాయి("(Left %d)\n", మార్గం->ఎడమ);
#అయితే_ముగింపు

	ఒకవేళ (handle->level == handle->max_లోతు) {
		ex = (నిర్మాణం ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_గమ్యం_cpu(ex->ee_ప్రారంభం) +
			((__u64) ext2fs_le16_గమ్యం_cpu(ex->ee_ప్రారంభం_hi) << 32);
		extent->e_lblk = ext2fs_le32_గమ్యం_cpu(ex->ee_ఖండం);
		extent->e_len = ext2fs_le16_గమ్యం_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_ఆకు;
		ఒకవేళ (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} లేకపోతే {
		extent->e_pblk = ext2fs_le32_గమ్యం_cpu(ix->ei_ఆకు) +
			((__u64) ext2fs_le16_గమ్యం_cpu(ix->ei_ఆకు_hi) << 32);
		extent->e_lblk = ext2fs_le32_గమ్యం_cpu(ix->ei_ఖండం);
		ఒకవేళ (మార్గం->ఎడమ > 0) {
			ix++;
			చివర_blk = ext2fs_le32_గమ్యం_cpu(ix->ei_ఖండం);
		} లేకపోతే
			చివర_blk = మార్గం->చివర_blk;

		extent->e_len = చివర_blk - extent->e_lblk;
	}
	ఒకవేళ (మార్గం->సందర్శించు_అంకె)
		extent->e_flags |= EXT2_EXTENT_FLAGS_రెండవ_సందర్శించు;

	ఒకవేళ (((orig_op == EXT2_EXTENT_తదుపరి_ఆకు) ||
	     (orig_op == EXT2_EXTENT_PREV_ఆకు)) &&
	    (handle->level != handle->max_లోతు))
		వెళ్ళు retry;

	ఒకవేళ ((orig_op == EXT2_EXTENT_చివరి_ఆకు) &&
	    ((handle->level != handle->max_లోతు) ||
	     (మార్గం->ఎడమ != 0)))
		వెళ్ళు retry;

	ఒకవేళ (failed_csum)
		తిరిగి EXT2_ET_EXTENT_CSUM_INVALID;

	తిరిగి 0;
}

స్థిర errcode_t update_మార్గం(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	నిర్మాణం ext3_extent_idx		*ix;
	నిర్మాణం ext3_extent_శీర్షిక	*eh;

	ఒకవేళ (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} లేకపోతే {
		ix = handle->మార్గం[handle->level - 1].curr;
		blk = ext2fs_le32_గమ్యం_cpu(ix->ei_ఆకు) +
			((__u64) ext2fs_le16_గమ్యం_cpu(ix->ei_ఆకు_hi) << 32);

		/* then update the checksum */
		eh = (నిర్మాణం ext3_extent_శీర్షిక *)
				handle->మార్గం[handle->level].buf;
		retval = ext2fs_extent_ఖండం_csum_సమితి(handle->fs, handle->ino,
						      eh);
		ఒకవేళ (retval)
			తిరిగి retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->మార్గం[handle->level].buf);
	}
	తిరిగి retval;
}

#ఒకవేళ 0
errcode_t ext2fs_extent_save_మార్గం(ext2_extent_handle_t handle,
				  ext2_extent_మార్గం_t *ret_మార్గం)
{
	ext2_extent_మార్గం_t	save_మార్గం;
	నిర్మాణం ext2fs_extent	extent;
	నిర్మాణం ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = ext2fs_extent_get_info(handle, &info);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_extent_మార్గం), &save_మార్గం);
	ఒకవేళ (retval)
		తిరిగి retval;
	memset(save_మార్గం, 0, పరిమాణం(నిర్మాణం ext2_extent_మార్గం));

	save_మార్గం->magic = EXT2_ET_MAGIC_EXTENT_మార్గం;
	save_మార్గం->ఆకు_ఎత్తు = info.max_లోతు - info.curr_level - 1;
	save_మార్గం->lblk = extent.e_lblk;

	*ret_మార్గం = save_మార్గం;
	తిరిగి 0;
}

errcode_t ext2fs_extent_free_మార్గం(ext2_extent_మార్గం_t మార్గం)
{
	EXT2_CHECK_MAGIC(మార్గం, EXT2_ET_MAGIC_EXTENT_మార్గం);

	ext2fs_free_mem(&మార్గం);
	తిరిగి 0;
}
#అయితే_ముగింపు

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
			      పూర్ణాంకం ఆకు_level, blk64_t blk)
{
	నిర్మాణం ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_మూలం, &extent);
	ఒకవేళ (retval) {
		ఒకవేళ (retval == EXT2_ET_EXTENT_NO_తదుపరి)
			retval = EXT2_ET_EXTENT_NOT_దొరికింది;
		తిరిగి retval;
	}

	ఒకవేళ (ఆకు_level > handle->max_లోతు) {
#నిర్వచితమైతే DEBUG
		రాయి("leaf level %d greater than tree depth %d\n",
			ఆకు_level, handle->max_లోతు);
#అయితే_ముగింపు
		తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
	}

#నిర్వచితమైతే DEBUG
	రాయి("goto extent ino %u, level %d, %llu\n", handle->ino,
	       ఆకు_level, blk);
#అయితే_ముగింపు

#నిర్వచితమైతే DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#అయితే_ముగింపు
	అయితే (1) {
		ఒకవేళ (handle->max_లోతు - handle->level == ఆకు_level) {
			/* block is in this &extent */
			ఒకవేళ ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				తిరిగి 0;
			ఒకవేళ (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				తిరిగి EXT2_ET_EXTENT_NOT_దొరికింది;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_తదుపరి_SIB,
						   &extent);
			ఒకవేళ (retval == EXT2_ET_EXTENT_NO_తదుపరి)
				తిరిగి EXT2_ET_EXTENT_NOT_దొరికింది;
			ఒకవేళ (retval)
				తిరిగి retval;
			కొనసాగు;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_తదుపరి_SIB,
					   &extent);
		ఒకవేళ (retval == EXT2_ET_EXTENT_NO_తదుపరి)
			వెళ్ళు go_కిందికి;
		ఒకవేళ (retval)
			తిరిగి retval;

#నిర్వచితమైతే DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#అయితే_ముగింపు
		ఒకవేళ (blk == extent.e_lblk)
			వెళ్ళు go_కిందికి;
		ఒకవేళ (blk > extent.e_lblk)
			కొనసాగు;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		ఒకవేళ (retval)
			తిరిగి retval;

#నిర్వచితమైతే DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#అయితే_ముగింపు

	go_కిందికి:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_కిందికి,
					   &extent);
		ఒకవేళ (retval)
			తిరిగి retval;

#నిర్వచితమైతే DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#అయితే_ముగింపు
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	తిరిగి ext2fs_extent_goto2(handle, 0, blk);
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
	పూర్ణాంకం				retval = 0;
	పూర్ణాంకం				orig_ఎత్తు;
	blk64_t				ప్రారంభం;
	నిర్మాణం extent_మార్గం		*మార్గం;
	నిర్మాణం ext2fs_extent		extent;
	నిర్మాణం ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ఒకవేళ (!(handle->fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	ఒకవేళ (!handle->మార్గం)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

	మార్గం = handle->మార్గం + handle->level;
	ఒకవేళ (!మార్గం->curr)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ఒకవేళ (retval)
		వెళ్ళు పూర్తి;

	/* modified node's start block */
	ప్రారంభం = extent.e_lblk;

	ఒకవేళ ((retval = ext2fs_extent_get_info(handle, &info)))
		తిరిగి retval;
	orig_ఎత్తు = info.max_లోతు - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	అయితే (handle->level > 0 &&
	       (మార్గం->ఎడమ == మార్గం->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_పైకి, &extent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
		ఒకవేళ (extent.e_lblk == ప్రారంభం)
			ఆపు;
		మార్గం = handle->మార్గం + handle->level;
		extent.e_len += (extent.e_lblk - ప్రారంభం);
		extent.e_lblk = ప్రారంభం;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
		update_మార్గం(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_ఎత్తు, ప్రారంభం);
పూర్తి:
	తిరిగి retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				పూర్ణాంకం flags EXT2FS_ATTR((వాడని)),
				నిర్మాణం ext2fs_extent *extent)
{
	నిర్మాణం extent_మార్గం		*మార్గం;
	నిర్మాణం ext3_extent_idx		*ix;
	నిర్మాణం ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ఒకవేళ (!(handle->fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	ఒకవేళ (!handle->మార్గం)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

	మార్గం = handle->మార్గం + handle->level;
	ఒకవేళ (!మార్గం->curr)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

#నిర్వచితమైతే DEBUG
	రాయి("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#అయితే_ముగింపు

	ఒకవేళ (handle->level == handle->max_లోతు) {
		ex = మార్గం->curr;

		ex->ee_ఖండం = ext2fs_cpu_గమ్యం_le32(extent->e_lblk);
		ex->ee_ప్రారంభం = ext2fs_cpu_గమ్యం_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_ప్రారంభం_hi = ext2fs_cpu_గమ్యం_le16(extent->e_pblk >> 32);
		ఒకవేళ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			ఒకవేళ (extent->e_len > EXT_UNINIT_MAX_LEN)
				తిరిగి EXT2_ET_EXTENT_INVALID_నిడివి;
			ex->ee_len = ext2fs_cpu_గమ్యం_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} లేకపోతే {
			ఒకవేళ (extent->e_len > EXT_INIT_MAX_LEN)
				తిరిగి EXT2_ET_EXTENT_INVALID_నిడివి;
			ex->ee_len = ext2fs_cpu_గమ్యం_le16(extent->e_len);
		}
	} లేకపోతే {
		ix = మార్గం->curr;

		ix->ei_ఆకు = ext2fs_cpu_గమ్యం_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_ఆకు_hi = ext2fs_cpu_గమ్యం_le16(extent->e_pblk >> 32);
		ix->ei_ఖండం = ext2fs_cpu_గమ్యం_le32(extent->e_lblk);
		ix->ei_వాడని = 0;
	}
	update_మార్గం(handle);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం splitting_at_eof(నిర్మాణం ext2_extent_handle *handle,
			    నిర్మాణం extent_మార్గం *మార్గం)
{
	నిర్మాణం extent_మార్గం *ppp = మార్గం;
	dump_మార్గం(__func__, handle, మార్గం);

	ఒకవేళ (handle->level == 0)
		తిరిగి 0;

	చేయి {
		ఒకవేళ (ppp->ఎడమ)
			తిరిగి 0;
		ppp--;
	} అయితే (ppp >= handle->మార్గం);

	తిరిగి 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
స్థిర errcode_t extent_కణుపు_విభజించు(ext2_extent_handle_t handle,
				   పూర్ణాంకం expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_కణుపు_pblk;
	blk64_t				new_కణుపు_ప్రారంభం;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	పూర్ణాంకం				orig_ఎత్తు;
	అక్షరం				*ఖండం_buf = ఖాళీ;
	నిర్మాణం ext2fs_extent		extent;
	నిర్మాణం extent_మార్గం		*మార్గం, *newpath = 0;
	నిర్మాణం ext3_extent_శీర్షిక	*eh, *neweh;
	పూర్ణాంకం				tocopy;
	పూర్ణాంకం				new_మూలం = 0;
	నిర్మాణం ext2_extent_info		info;
	పూర్ణాంకం				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ఒకవేళ (!(handle->fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	ఒకవేళ (!handle->మార్గం)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

#నిర్వచితమైతే DEBUG
	రాయి("splitting node at level %d\n", handle->level);
#అయితే_ముగింపు
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ఒకవేళ (retval)
		వెళ్ళు పూర్తి;

	retval = ext2fs_extent_get_info(handle, &info);
	ఒకవేళ (retval)
		వెళ్ళు పూర్తి;

	/* save the position we were originally splitting... */
	orig_ఎత్తు = info.max_లోతు - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	మార్గం = handle->మార్గం + handle->level;
	eh = (నిర్మాణం ext3_extent_శీర్షిక *) మార్గం->buf;
	ఒకవేళ (handle->level == handle->max_లోతు) {
		నిర్మాణం ext3_extent	*ex;

		ex = EXT_మొదటి_EXTENT(eh);
		goal_blk = ext2fs_le32_గమ్యం_cpu(ex->ee_ప్రారంభం) +
			((__u64) ext2fs_le16_గమ్యం_cpu(ex->ee_ప్రారంభం_hi) << 32);
	} లేకపోతే {
		నిర్మాణం ext3_extent_idx	*ix;

		ix = EXT_మొదటి_సూచీ(eh);
		goal_blk = ext2fs_le32_గమ్యం_cpu(ix->ei_ఆకు) +
			((__u64) ext2fs_le16_గమ్యం_cpu(ix->ei_ఆకు_hi) << 32);
	}
	goal_blk -= EXT2_FS_CLUSTER_నిష్పత్తి(handle->fs);
	goal_blk &= ~EXT2_FS_CLUSTER_ముసుగు(handle->fs);

	/* Is there room in the parent for a new entry? */
	ఒకవేళ (handle->level &&
			(handle->మార్గం[handle->level - 1].entries >=
			 handle->మార్గం[handle->level - 1].max_entries)) {

#నిర్వచితమైతే DEBUG
		రాయి("parent level %d full; splitting it too\n",
							handle->level - 1);
#అయితే_ముగింపు
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_పైకి, &extent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;

		retval = extent_కణుపు_విభజించు(handle, expand_allowed);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_ఎత్తు, orig_lblk);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
	}

	/* At this point, parent should have room for this split */
	మార్గం = handle->మార్గం + handle->level;
	ఒకవేళ (!మార్గం->curr)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, మార్గం) : 0;

	/* extent header of the current node we'll split */
	eh = (నిర్మాణం ext3_extent_శీర్షిక *)మార్గం->buf;

	/* splitting root level means moving them all out */
	ఒకవేళ (handle->level == 0) {
		new_మూలం = 1;
		tocopy = ext2fs_le16_గమ్యం_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    పరిమాణం(నిర్మాణం extent_మార్గం),
					    &newpath);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
	} లేకపోతే {
		ఒకవేళ (no_balance)
			tocopy = 1;
		లేకపోతే
			tocopy = ext2fs_le16_గమ్యం_cpu(eh->eh_entries) / 2;
	}

#నిర్వచితమైతే DEBUG
	రాయి("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_గమ్యం_cpu(eh->eh_entries),
				handle->level);
#అయితే_ముగింపు

	ఒకవేళ (!tocopy && !no_balance) {
#నిర్వచితమైతే DEBUG
		రాయి("Nothing to copy to new block!\n");
#అయితే_ముగింపు
		retval = EXT2_ET_CANT_విభజించు_EXTENT;
		వెళ్ళు పూర్తి;
	}

	/* first we need a new block, or can do nothing. */
	ఖండం_buf = జ్ఞాపకం_ఇవ్వు(handle->fs->blocksize);
	ఒకవేళ (!ఖండం_buf) {
		retval = ENOMEM;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (!goal_blk)
		goal_blk = ext2fs_కనుగొను_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, ఖండం_buf,
				    &new_కణుపు_pblk);
	ఒకవేళ (retval)
		వెళ్ళు పూర్తి;

#నిర్వచితమైతే DEBUG
	రాయి("will copy to new node at block %lu\n",
	       (గుర్తులేని దీర్ఘ) new_కణుపు_pblk);
#అయితే_ముగింపు

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (నిర్మాణం ext3_extent_శీర్షిక *) ఖండం_buf;
	జ్ఞాపకం_నకలు(neweh, eh, పరిమాణం(నిర్మాణం ext3_extent_శీర్షిక));
	neweh->eh_entries = ext2fs_cpu_గమ్యం_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_గమ్యం_le16((handle->fs->blocksize -
			 పరిమాణం(నిర్మాణం ext3_extent_శీర్షిక)) /
				పరిమాణం(నిర్మాణం ext3_extent));

	/* then the entries for the new block... */
	జ్ఞాపకం_నకలు(EXT_మొదటి_సూచీ(neweh),
		EXT_మొదటి_సూచీ(eh) +
			(ext2fs_le16_గమ్యం_cpu(eh->eh_entries) - tocopy),
		పరిమాణం(నిర్మాణం ext3_extent_idx) * tocopy);

	new_కణుపు_ప్రారంభం = ext2fs_le32_గమ్యం_cpu(EXT_మొదటి_సూచీ(neweh)->ei_ఖండం);

	/* then update the checksum */
	retval = ext2fs_extent_ఖండం_csum_సమితి(handle->fs, handle->ino, neweh);
	ఒకవేళ (retval)
		వెళ్ళు పూర్తి;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_కణుపు_pblk, 1,
					ఖండం_buf);

	ఒకవేళ (retval)
		వెళ్ళు పూర్తి;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	ఒకవేళ (handle->level == 0) {
		జ్ఞాపకం_నకలు(newpath, మార్గం,
		       పరిమాణం(నిర్మాణం extent_మార్గం) * handle->max_paths);
		handle->మార్గం = newpath;
		newpath = మార్గం;
		మార్గం = handle->మార్గం;
		మార్గం->entries = 1;
		మార్గం->ఎడమ = మార్గం->max_entries - 1;
		handle->max_లోతు++;
		handle->max_paths++;
		eh->eh_లోతు = ext2fs_cpu_గమ్యం_le16(handle->max_లోతు);
	} లేకపోతే {
		మార్గం->entries -= tocopy;
		మార్గం->ఎడమ -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_గమ్యం_le16(మార్గం->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_మార్గం(handle);
	ఒకవేళ (retval)
		వెళ్ళు పూర్తి;

	/* now go up and insert/replace index for new node we created */
	ఒకవేళ (new_మూలం) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_మొదటి_SIB, &extent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;

		extent.e_lblk = new_కణుపు_ప్రారంభం;
		extent.e_pblk = new_కణుపు_pblk;
		extent.e_len = handle->మార్గం[0].చివర_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
	} లేకపోతే {
		__u32 new_కణుపు_నిడివి;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_పైకి, &extent);
		/* will insert after this one; it's length is shorter now */
		new_కణుపు_నిడివి = new_కణుపు_ప్రారంభం - extent.e_lblk;
		extent.e_len -= new_కణుపు_నిడివి;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_కణుపు_ప్రారంభం;
		extent.e_pblk = new_కణుపు_pblk;
		extent.e_len = new_కణుపు_నిడివి;
		retval = ext2fs_extent_చొప్పించు(handle, EXT2_EXTENT_చొప్పించు_AFTER, &extent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_ఎత్తు, orig_lblk);
	ఒకవేళ (retval)
		వెళ్ళు పూర్తి;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_కూడు_ఖండాలు(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	ఒకవేళ (retval)
		వెళ్ళు పూర్తి;

పూర్తి:
	ఒకవేళ (newpath)
		ext2fs_free_mem(&newpath);
	విడుదల(ఖండం_buf);

	తిరిగి retval;
}

errcode_t ext2fs_extent_కణుపు_విభజించు(ext2_extent_handle_t handle)
{
	తిరిగి extent_కణుపు_విభజించు(handle, 0);
}

errcode_t ext2fs_extent_చొప్పించు(ext2_extent_handle_t handle, పూర్ణాంకం flags,
				      నిర్మాణం ext2fs_extent *extent)
{
	నిర్మాణం extent_మార్గం		*మార్గం;
	నిర్మాణం ext3_extent_idx		*ix;
	నిర్మాణం ext3_extent_శీర్షిక	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ఒకవేళ (!(handle->fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	ఒకవేళ (!handle->మార్గం)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

#నిర్వచితమైతే DEBUG
	రాయి("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#అయితే_ముగింపు

	మార్గం = handle->మార్గం + handle->level;

	ఒకవేళ (మార్గం->entries >= మార్గం->max_entries) {
		ఒకవేళ (flags & EXT2_EXTENT_చొప్పించు_NOSPLIT) {
			తిరిగి EXT2_ET_CANT_చొప్పించు_EXTENT;
		} లేకపోతే {
#నిర్వచితమైతే DEBUG
			రాయి("node full (level %d) - splitting\n",
				   handle->level);
#అయితే_ముగింపు
			retval = extent_కణుపు_విభజించు(handle, 1);
			ఒకవేళ (retval)
				తిరిగి retval;
			మార్గం = handle->మార్గం + handle->level;
		}
	}

	eh = (నిర్మాణం ext3_extent_శీర్షిక *) మార్గం->buf;
	ఒకవేళ (మార్గం->curr) {
		ix = మార్గం->curr;
		ఒకవేళ (flags & EXT2_EXTENT_చొప్పించు_AFTER) {
			ix++;
			మార్గం->ఎడమ--;
		}
	} లేకపోతే {
		ix = EXT_మొదటి_సూచీ(eh);
		మార్గం->ఎడమ = -1;
	}

	మార్గం->curr = ix;

	ఒకవేళ (మార్గం->ఎడమ >= 0)
		memmove(ix + 1, ix,
			(మార్గం->ఎడమ+1) * పరిమాణం(నిర్మాణం ext3_extent_idx));
	మార్గం->ఎడమ++;
	మార్గం->entries++;

	eh = (నిర్మాణం ext3_extent_శీర్షిక *) మార్గం->buf;
	eh->eh_entries = ext2fs_cpu_గమ్యం_le16(మార్గం->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	ఒకవేళ (retval)
		వెళ్ళు errout;

	retval = update_మార్గం(handle);
	ఒకవేళ (retval)
		వెళ్ళు errout;

	తిరిగి 0;

errout:
	ext2fs_extent_delete(handle, 0);
	తిరిగి retval;
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

errcode_t ext2fs_extent_సమితి_bmap(ext2_extent_handle_t handle,
				 blk64_t logical, blk64_t physical, పూర్ణాంకం flags)
{
	errcode_t		ec, retval = 0;
	పూర్ణాంకం			mapped = 1; /* logical is mapped? */
	పూర్ణాంకం			orig_ఎత్తు;
	పూర్ణాంకం			extent_uninit = 0;
	పూర్ణాంకం			prev_uninit = 0;
	పూర్ణాంకం			తదుపరి_uninit = 0;
	పూర్ణాంకం			new_uninit = 0;
	పూర్ణాంకం			max_len = EXT_INIT_MAX_LEN;
	పూర్ణాంకం			has_prev, has_తదుపరి;
	blk64_t			orig_lblk;
	నిర్మాణం extent_మార్గం	*మార్గం;
	నిర్మాణం ext2fs_extent	extent, తదుపరి_extent, prev_extent;
	నిర్మాణం ext2fs_extent	newextent;
	నిర్మాణం ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#నిర్వచితమైతే DEBUG
	రాయి("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#అయితే_ముగింపు

	ఒకవేళ (!(handle->fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	ఒకవేళ (!handle->మార్గం)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

	మార్గం = handle->మార్గం + handle->level;

	ఒకవేళ (flags & EXT2_EXTENT_సమితి_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	ఒకవేళ (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_ఆకు;
		ఒకవేళ (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	ఒకవేళ ((handle->max_లోతు == 0) && (మార్గం->entries == 0)) {
		retval = ext2fs_extent_చొప్పించు(handle, 0, &newextent);
		తిరిగి retval;
	}

	/* save our original location in the extent tree */
	ఒకవేళ ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		ఒకవేళ (retval != EXT2_ET_NO_CURRENT_కణుపు)
			తిరిగి retval;
		memset(&extent, 0, పరిమాణం(extent));
	}
	ఒకవేళ ((retval = ext2fs_extent_get_info(handle, &info)))
		తిరిగి retval;
	orig_ఎత్తు = info.max_లోతు - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	ఒకవేళ (retval) {
		ఒకవేళ (retval == EXT2_ET_EXTENT_NOT_దొరికింది) {
			retval = 0;
			mapped = 0;
			ఒకవేళ (!physical) {
#నిర్వచితమైతే DEBUG
				రాయి("block %llu already unmapped\n",
					logical);
#అయితే_ముగింపు
				వెళ్ళు పూర్తి;
			}
		} లేకపోతే
			వెళ్ళు పూర్తి;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ఒకవేళ (retval)
		వెళ్ళు పూర్తి;
	ఒకవేళ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_తదుపరి_ఆకు, &తదుపరి_extent);
	ఒకవేళ (retval) {
		has_తదుపరి = 0;
		ఒకవేళ (retval != EXT2_ET_EXTENT_NO_తదుపరి)
			వెళ్ళు పూర్తి;
	} లేకపోతే {
		dbg_print_extent("set_bmap: next_extent",
				 &తదుపరి_extent);
		has_తదుపరి = 1;
		ఒకవేళ (తదుపరి_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			తదుపరి_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ఒకవేళ (retval && retval != EXT2_ET_EXTENT_NOT_దొరికింది)
		వెళ్ళు పూర్తి;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_ఆకు, &prev_extent);
	ఒకవేళ (retval) {
		has_prev = 0;
		ఒకవేళ (retval != EXT2_ET_EXTENT_NO_PREV)
			వెళ్ళు పూర్తి;
	} లేకపోతే {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		ఒకవేళ (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	ఒకవేళ (retval && retval != EXT2_ET_EXTENT_NOT_దొరికింది)
		వెళ్ళు పూర్తి;

	/* check if already pointing to the requested physical */
	ఒకవేళ (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#నిర్వచితమైతే DEBUG
		రాయి("physical block (at %llu) unchanged\n", logical);
#అయితే_ముగింపు
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (!mapped) {
#నిర్వచితమైతే DEBUG
		రాయి("mapping unmapped logical block %llu\n", logical);
#అయితే_ముగింపు
		ఒకవేళ ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((పూర్ణాంకం) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} లేకపోతే ఒకవేళ ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((పూర్ణాంకం) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} లేకపోతే ఒకవేళ (has_తదుపరి &&
			   (logical == తదుపరి_extent.e_lblk - 1) &&
			   (physical == తదుపరి_extent.e_pblk - 1) &&
			   (new_uninit == తదుపరి_uninit) &&
			   ((పూర్ణాంకం) తదుపరి_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_తదుపరి_ఆకు,
						   &తదుపరి_extent);
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
			తదుపరి_extent.e_len++;
			తదుపరి_extent.e_lblk--;
			తదుపరి_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &తదుపరి_extent);
		} లేకపోతే ఒకవేళ (logical < extent.e_lblk)
			retval = ext2fs_extent_చొప్పించు(handle, 0, &newextent);
		లేకపోతే
			retval = ext2fs_extent_చొప్పించు(handle,
				      EXT2_EXTENT_చొప్పించు_AFTER, &newextent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
		retval = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
	} లేకపోతే ఒకవేళ ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#నిర్వచితమైతే DEBUG
		రాయి("(re/un)mapping only block in extent\n");
#అయితే_ముగింపు
		ఒకవేళ (physical) {
			ఒకవేళ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((పూర్ణాంకం) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_ఆకు, &prev_extent);
				ఒకవేళ (retval)
					వెళ్ళు పూర్తి;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_తదుపరి_ఆకు,
							   &extent);
				ఒకవేళ (retval)
					వెళ్ళు పూర్తి;
				వెళ్ళు delete_కణుపు;

			} లేకపోతే
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} లేకపోతే {
		delete_కణుపు:
			retval = ext2fs_extent_delete(handle, 0);
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
			ec = ext2fs_extent_fix_parents(handle);
			ఒకవేళ (ec != EXT2_ET_NO_CURRENT_కణుపు)
				retval = ec;
		}

		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
	} లేకపోతే ఒకవేళ (logical == extent.e_lblk + extent.e_len - 1)  {
#నిర్వచితమైతే DEBUG
		రాయి("(re/un)mapping last block in extent\n");
#అయితే_ముగింపు
		ఒకవేళ (physical) {
			ఒకవేళ (has_తదుపరి &&
			    (logical == (తదుపరి_extent.e_lblk - 1)) &&
			    (physical == (తదుపరి_extent.e_pblk - 1)) &&
			    (new_uninit == తదుపరి_uninit) &&
			    ((పూర్ణాంకం) తదుపరి_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_తదుపరి_ఆకు, &తదుపరి_extent);
				ఒకవేళ (retval)
					వెళ్ళు పూర్తి;
				తదుపరి_extent.e_len++;
				తదుపరి_extent.e_lblk--;
				తదుపరి_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &తదుపరి_extent);
				ఒకవేళ (retval)
					వెళ్ళు పూర్తి;
			} లేకపోతే
				retval = ext2fs_extent_చొప్పించు(handle,
				      EXT2_EXTENT_చొప్పించు_AFTER, &newextent);
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
			retval = ext2fs_extent_fix_parents(handle);
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
	} లేకపోతే ఒకవేళ (logical == extent.e_lblk) {
#నిర్వచితమైతే DEBUG
		రాయి("(re/un)mapping first block in extent\n");
#అయితే_ముగింపు
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
		retval = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
		ఒకవేళ (physical) {
			ఒకవేళ (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((పూర్ణాంకం) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_ఆకు, &prev_extent);
				ఒకవేళ (retval)
					వెళ్ళు పూర్తి;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} లేకపోతే
				retval = ext2fs_extent_చొప్పించు(handle,
							      0, &newextent);
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
			retval = ext2fs_extent_fix_parents(handle);
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_తదుపరి_ఆకు,
						   &extent);
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
		}
	} లేకపోతే {
		__u32	save_నిడివి;
		blk64_t	save_lblk;
		నిర్మాణం ext2fs_extent save_extent;
		errcode_t r2;

#నిర్వచితమైతే DEBUG
		రాయి("(re/un)mapping in middle of extent\n");
#అయితే_ముగింపు
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_నిడివి = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
		/* insert our new extent, if any */
		ఒకవేళ (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_చొప్పించు(handle,
					EXT2_EXTENT_చొప్పించు_AFTER, &newextent);
			ఒకవేళ (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				ఒకవేళ (r2 == 0)
					(శూన్య)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				వెళ్ళు పూర్తి;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_నిడివి - extent.e_len - 1;
		retval = ext2fs_extent_చొప్పించు(handle,
				EXT2_EXTENT_చొప్పించు_AFTER, &extent);
		ఒకవేళ (retval) {
			ఒకవేళ (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				ఒకవేళ (r2 == 0)
					(శూన్య)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			ఒకవేళ (r2 == 0)
				(శూన్య)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			వెళ్ళు పూర్తి;
		}
	}

పూర్తి:
	/* get handle back to its position */
	ఒకవేళ (orig_ఎత్తు > handle->max_లోతు)
		orig_ఎత్తు = handle->max_లోతు; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_ఎత్తు, orig_lblk);
	తిరిగి retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, పూర్ణాంకం flags)
{
	నిర్మాణం extent_మార్గం		*మార్గం;
	అక్షరం 				*cp;
	నిర్మాణం ext3_extent_శీర్షిక	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	ఒకవేళ (!(handle->fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	ఒకవేళ (!handle->మార్గం)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

#నిర్వచితమైతే DEBUG
	{
		నిర్మాణం ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		ఒకవేళ (retval == 0) {
			రాయి("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#అయితే_ముగింపు

	మార్గం = handle->మార్గం + handle->level;
	ఒకవేళ (!మార్గం->curr)
		తిరిగి EXT2_ET_NO_CURRENT_కణుపు;

	cp = మార్గం->curr;

	/* Sanity check before memmove() */
	ఒకవేళ (మార్గం->ఎడమ < 0)
		తిరిగి EXT2_ET_EXTENT_ఆకు_BAD;

	ఒకవేళ (మార్గం->ఎడమ) {
		memmove(cp, cp + పరిమాణం(నిర్మాణం ext3_extent_idx),
			మార్గం->ఎడమ * పరిమాణం(నిర్మాణం ext3_extent_idx));
		మార్గం->ఎడమ--;
	} లేకపోతే {
		నిర్మాణం ext3_extent_idx	*ix = మార్గం->curr;
		ix--;
		మార్గం->curr = ix;
	}
	ఒకవేళ (--మార్గం->entries == 0)
		మార్గం->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	ఒకవేళ (మార్గం->entries == 0 && handle->level) {
		ఒకవేళ (!(flags & EXT2_EXTENT_DELETE_KEEP_ఖాళీలు)) {
			నిర్మాణం ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_పైకి,
								&extent);
			ఒకవేళ (retval)
				తిరిగి retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_ఖండాలు -=
				(handle->fs->blocksize *
				 EXT2_FS_CLUSTER_నిష్పత్తి(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_ఖండం_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} లేకపోతే {
		eh = (నిర్మాణం ext3_extent_శీర్షిక *) మార్గం->buf;
		eh->eh_entries = ext2fs_cpu_గమ్యం_le16(మార్గం->entries);
		ఒకవేళ ((మార్గం->entries == 0) && (handle->level == 0)) {
			eh->eh_లోతు = 0;
			handle->max_లోతు = 0;
		}
		retval = update_మార్గం(handle);
	}
	తిరిగి retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 నిర్మాణం ext2_extent_info *info)
{
	నిర్మాణం extent_మార్గం		*మార్గం;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, పరిమాణం(నిర్మాణం ext2_extent_info));

	మార్గం = handle->మార్గం + handle->level;
	ఒకవేళ (మార్గం) {
		ఒకవేళ (మార్గం->curr)
			info->curr_నమోదు = ((అక్షరం *) మార్గం->curr - మార్గం->buf) /
				పరిమాణం(నిర్మాణం ext3_extent_idx);
		లేకపోతే
			info->curr_నమోదు = 0;
		info->అంకె_entries = మార్గం->entries;
		info->max_entries = మార్గం->max_entries;
		info->బైట్లు_avail = (మార్గం->max_entries - మార్గం->entries) *
			పరిమాణం(నిర్మాణం ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_లోతు = handle->max_లోతు;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	తిరిగి 0;
}

size_t ext2fs_max_extent_లోతు(ext2_extent_handle_t handle)
{
	size_t iblock_sz = పరిమాణం(((నిర్మాణం ext2_inode *)ఖాళీ)->i_ఖండం);
	size_t iblock_extents = (iblock_sz - పరిమాణం(నిర్మాణం ext3_extent_శీర్షిక)) /
				పరిమాణం(నిర్మాణం ext3_extent);
	size_t extents_చొప్పున_ఖండం = (handle->fs->blocksize -
				    పరిమాణం(నిర్మాణం ext3_extent_శీర్షిక)) /
				   పరిమాణం(నిర్మాణం ext3_extent);
	స్థిర గుర్తులేని పూర్ణాంకం చివరి_blocksize = 0;
	స్థిర size_t చివరి_ఫలితం = 0;

	ఒకవేళ (చివరి_blocksize && చివరి_blocksize == handle->fs->blocksize)
		తిరిగి చివరి_ఫలితం;

	చివరి_ఫలితం = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_చొప్పున_ఖండం));
	చివరి_blocksize = handle->fs->blocksize;
	తిరిగి చివరి_ఫలితం;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       నిర్మాణం ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	నిర్మాణం ext2fs_extent	extent;
	errcode_t		errcode;
	పూర్ణాంకం    			save_flags = fs->flags;

	ఒకవేళ (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		తిరిగి 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ఒకవేళ (errcode) {
		ఒకవేళ (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		తిరిగి errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_మూలం, &extent);
	ఒకవేళ (errcode)
		వెళ్ళు బహిర్గతం;

	చేయి {
		/* Skip to the end of a block of leaf nodes */
		ఒకవేళ (extent.e_flags & EXT2_EXTENT_FLAGS_ఆకు) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_చివరి_SIB,
						    &extent);
			ఒకవేళ (errcode)
				ఆపు;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_తదుపరి, &extent);
		ఒకవేళ (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_మార్గం(handle);
	} అయితే (errcode == 0);

బహిర్గతం:
	/* Ok if we run off the end */
	ఒకవేళ (errcode == EXT2_ET_EXTENT_NO_తదుపరి)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	తిరిగి errcode;
}

errcode_t ext2fs_decode_extent(నిర్మాణం ext2fs_extent *గమ్యం, శూన్య *addr, పూర్ణాంకం len)
{
	నిర్మాణం ext3_extent *from = (నిర్మాణం ext3_extent *)addr;

	ఒకవేళ (len != పరిమాణం(నిర్మాణం ext3_extent))
		తిరిగి EXT2_ET_INVALID_వాదం;

	గమ్యం->e_pblk = ext2fs_le32_గమ్యం_cpu(from->ee_ప్రారంభం) +
		((__u64) ext2fs_le16_గమ్యం_cpu(from->ee_ప్రారంభం_hi)
			<< 32);
	గమ్యం->e_lblk = ext2fs_le32_గమ్యం_cpu(from->ee_ఖండం);
	గమ్యం->e_len = ext2fs_le16_గమ్యం_cpu(from->ee_len);
	గమ్యం->e_flags = EXT2_EXTENT_FLAGS_ఆకు;
	ఒకవేళ (గమ్యం->e_len > EXT_INIT_MAX_LEN) {
		గమ్యం->e_len -= EXT_INIT_MAX_LEN;
		గమ్యం->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	తిరిగి 0;
}

errcode_t ext2fs_లెక్క_ఖండాలు(ext2_filsys fs, ext2_ino_t ino,
			      నిర్మాణం ext2_inode *inode, blk64_t *ret_లెక్క)
{
	ext2_extent_handle_t	handle = ఖాళీ;
	నిర్మాణం ext2fs_extent	extent;
	errcode_t		errcode;
	పూర్ణాంకం			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	ఒకవేళ (errcode)
		వెళ్ళు బహిర్గతం;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_మూలం, &extent);
	ఒకవేళ (errcode)
		వెళ్ళు బహిర్గతం;

	errcode = ext2fs_get_array(handle->max_లోతు, పరిమాణం(blk64_t),
				   &intermediate_nodes);
	ఒకవేళ (errcode)
		వెళ్ళు బహిర్గతం;

	blkcount = handle->level;
	అయితే (!errcode) {
		ఒకవేళ (extent.e_flags & EXT2_EXTENT_FLAGS_ఆకు) {
			blkcount += extent.e_len;
			కొరకు (i = 0; i < handle->level; i++) {
				ఒకవేళ (intermediate_nodes[i] !=
					handle->మార్గం[i].చివర_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->మార్గం[i].చివర_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_తదుపరి, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
బహిర్గతం:
	*ret_లెక్క = blkcount;
	ext2fs_extent_free(handle);

	తిరిగి 0;
}

#నిర్వచితమైతే DEBUG
/*
 * Override debugfs's prompt
 */
స్థిరరాశి అక్షరం *debug_prog_name = "tst_extents";

#అయితే_ముగింపు

