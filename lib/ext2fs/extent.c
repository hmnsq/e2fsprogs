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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "e2image.h"

#تعریف_لغوه DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

بنية extent_لار {
	حرف		*buf;
	صحيح		entries;
	صحيح		max_entries;
	صحيح		کیڼ;
	صحيح		لیدنه_عدد;
	صحيح		flags;
	blk64_t		پای_ټکی_blk;
	blk64_t		blk;
	فراغ		*curr;
};


بنية ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	بنية ext2_inode	*inode;
	بنية ext2_inode	inodebuf;
	صحيح			type;
	صحيح			level;
	صحيح			max_ژوروالی;
	صحيح			max_paths;
	بنية extent_لار	*لار;
};

بنية ext2_extent_لار {
	errcode_t		magic;
	صحيح			ونې_پاڼه_لوړوالی;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#که_تعریف_شوی DEBUG
ساكن فراغ dbg_show_سرلیک(بنية ext3_extent_سرلیک *eh)
{
	چاپ_کړه("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_منزل_cpu(eh->eh_magic),
			ext2fs_le16_منزل_cpu(eh->eh_entries),
			ext2fs_le16_منزل_cpu(eh->eh_max),
			ext2fs_le16_منزل_cpu(eh->eh_ژوروالی),
			ext2fs_le32_منزل_cpu(eh->eh_generation));
}

ساكن فراغ dbg_show_شاخص(بنية ext3_extent_idx *ix)
{
	چاپ_کړه("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_منزل_cpu(ix->ei_بلاک),
			ext2fs_le32_منزل_cpu(ix->ei_ونې_پاڼه),
			ext2fs_le16_منزل_cpu(ix->ei_ونې_پاڼه_hi),
			ext2fs_le16_منزل_cpu(ix->ei_ناکارول_شوی));
}

ساكن فراغ dbg_show_extent(بنية ext3_extent *ex)
{
	چاپ_کړه("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_منزل_cpu(ex->ee_بلاک),
			ext2fs_le32_منزل_cpu(ex->ee_بلاک) +
			ext2fs_le16_منزل_cpu(ex->ee_len) - 1,
			ext2fs_le16_منزل_cpu(ex->ee_len),
			ext2fs_le32_منزل_cpu(ex->ee_پیل),
			ext2fs_le16_منزل_cpu(ex->ee_پیل_hi));
}

ساكن فراغ dbg_print_extent(حرف *desc, بنية ext2fs_extent *extent)
{
	إذا (desc)
		چاپ_کړه("%s: ", desc);
	چاپ_کړه("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_ونې_پاڼه)
		fputs("LEAF ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_دوهم_لیدنه)
		fputs("2ND_VISIT ", stdout);
	إذا (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

ساكن فراغ dump_لار(ثابت حرف *نښان, بنية ext2_extent_handle *handle,
		      بنية extent_لار *لار)
{
	بنية extent_لار *ppp = لار;
	چاپ_کړه("%s: level=%d\n", نښان, handle->level);

	افعل {
		چاپ_کړه("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       نښان, (ppp - handle->لار), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->کیڼ, ppp->لیدنه_عدد, ppp->flags,
		       ppp->پای_ټکی_blk, ppp->curr, ppp->curr - (فراغ *)ppp->buf);
		چاپ_کړه("  ");
		dbg_show_سرلیک((بنية ext3_extent_سرلیک *)ppp->buf);
		إذا (ppp->curr) {
			چاپ_کړه("  ");
			dbg_show_شاخص(ppp->curr);
			چاپ_کړه("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} طالما (ppp >= handle->لار);
	fflush(stdout);

	أرجع;
}

#وإلا
#تعریف_کړه dbg_show_سرلیک(eh) افعل { } طالما (0)
#تعریف_کړه dbg_show_شاخص(ix) افعل { } طالما (0)
#تعریف_کړه dbg_show_extent(ex) افعل { } طالما (0)
#تعریف_کړه dbg_print_extent(desc, ex) افعل { } طالما (0)
#تعریف_کړه dump_لار(نښان, handle, لار) افعل { } طالما (0)
#د_که_پای

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_سرلیک_verify(فراغ *ptr, صحيح اندازه)
{
	صحيح eh_max, ثبت_اندازه;
	بنية ext3_extent_سرلیک *eh = ptr;

	dbg_show_سرلیک(eh);
	إذا (ext2fs_le16_منزل_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		أرجع EXT2_ET_EXTENT_سرلیک_BAD;
	إذا (ext2fs_le16_منزل_cpu(eh->eh_entries) > ext2fs_le16_منزل_cpu(eh->eh_max))
		أرجع EXT2_ET_EXTENT_سرلیک_BAD;
	إذا (eh->eh_ژوروالی == 0)
		ثبت_اندازه = حجم(بنية ext3_extent);
	وإلا
		ثبت_اندازه = حجم(بنية ext3_extent_idx);

	eh_max = (اندازه - حجم(*eh)) / ثبت_اندازه;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	إذا ((ext2fs_le16_منزل_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_منزل_cpu(eh->eh_max) < (eh_max - 2)))
		أرجع EXT2_ET_EXTENT_سرلیک_BAD;

	أرجع 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
فراغ ext2fs_extent_free(ext2_extent_handle_t handle)
{
	صحيح			i;

	إذا (!handle)
		أرجع;

	إذا (handle->لار) {
		لكل (i = 1; i < handle->max_paths; i++) {
			إذا (handle->لار[i].buf)
				ext2fs_free_mem(&handle->لار[i].buf);
		}
		ext2fs_free_mem(&handle->لار);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	أرجع ext2fs_extent_open2(fs, ino, عدم, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    بنية ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	بنية ext2_extent_handle	*handle;
	errcode_t			retval;
	صحيح				i;
	بنية ext3_extent_سرلیک	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!inode)
		إذا ((ino == 0) || (ino > fs->super->s_inodes_شمېر))
			أرجع EXT2_ET_BAD_INODE_عدد;

	retval = ext2fs_get_mem(حجم(بنية ext2_extent_handle), &handle);
	إذا (retval)
		أرجع retval;
	memset(handle, 0, حجم(بنية ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	إذا (inode) {
		handle->inode = inode;
	} وإلا {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		إذا (retval)
			اذهب errout;
	}

	eh = (بنية ext3_extent_سرلیک *) &handle->inode->i_بلاک[0];

	لكل (i=0; i < EXT2_N_بلاکونه; i++)
		إذا (handle->inode->i_بلاک[i])
			اكسر;
	إذا (i >= EXT2_N_بلاکونه) {
		eh->eh_magic = ext2fs_cpu_منزل_le16(EXT3_EXT_MAGIC);
		eh->eh_ژوروالی = 0;
		eh->eh_entries = 0;
		i = (حجم(handle->inode->i_بلاک) - حجم(*eh)) /
			حجم(بنية ext3_extent);
		eh->eh_max = ext2fs_cpu_منزل_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	إذا (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		اذهب errout;
	}

	retval = ext2fs_extent_سرلیک_verify(eh, حجم(handle->inode->i_بلاک));
	إذا (retval)
		اذهب errout;

	handle->max_ژوروالی = ext2fs_le16_منزل_cpu(eh->eh_ژوروالی);
	handle->type = ext2fs_le16_منزل_cpu(eh->eh_magic);

	handle->max_paths = handle->max_ژوروالی + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    حجم(بنية extent_لار),
				    &handle->لار);
	handle->لار[0].buf = (حرف *) handle->inode->i_بلاک;

	handle->لار[0].کیڼ = handle->لار[0].entries =
		ext2fs_le16_منزل_cpu(eh->eh_entries);
	handle->لار[0].max_entries = ext2fs_le16_منزل_cpu(eh->eh_max);
	handle->لار[0].curr = 0;
	handle->لار[0].پای_ټکی_blk =
		(EXT2_I_اندازه(handle->inode) + fs->blocksize - 1) >>
		 EXT2_بلاک_اندازه_بټونه(fs->super);
	handle->لار[0].blk = 0;
	handle->لار[0].لیدنه_عدد = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	أرجع 0;

errout:
	ext2fs_extent_free(handle);
	أرجع retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    صحيح flags, بنية ext2fs_extent *extent)
{
	بنية extent_لار	*لار, *newpath, *tp;
	بنية ext3_extent_سرلیک	*eh;
	بنية ext3_extent_idx		*ix = 0;
	بنية ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				پای_ټکی_blk;
	صحيح				orig_op, op, l;
	صحيح				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!handle->لار)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

	orig_op = op = flags & EXT2_EXTENT_MOVE_ماسک;

retry:
	لار = handle->لار + handle->level;
	إذا ((orig_op == EXT2_EXTENT_بل) ||
	    (orig_op == EXT2_EXTENT_بل_ونې_پاڼه)) {
		إذا (handle->level < handle->max_ژوروالی) {
			/* interior node */
			إذا (لار->لیدنه_عدد == 0) {
				لار->لیدنه_عدد++;
				op = EXT2_EXTENT_ښکته;
			} وإلا إذا (لار->کیڼ > 0)
				op = EXT2_EXTENT_بل_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_پورته;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_بل;
		} وإلا {
			/* leaf node */
			إذا (لار->کیڼ > 0)
				op = EXT2_EXTENT_بل_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_پورته;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_بل;
		}
		إذا (op != EXT2_EXTENT_بل_SIB) {
#که_تعریف_شوی DEBUG_GET_EXTENT
			چاپ_کړه("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_ښکته) ? "down" :
			       ((op == EXT2_EXTENT_پورته) ? "up" : "unknown"));
#د_که_پای
		}
	}

	إذا ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_ونې_پاڼه)) {
		إذا (handle->level < handle->max_ژوروالی) {
			/* interior node */
			إذا (لار->لیدنه_عدد > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_ښکته_AND_وروستی;
			} وإلا إذا (لار->کیڼ < لار->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_پورته;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_PREV;
		} وإلا {
			/* leaf node */
			إذا (لار->کیڼ < لار->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_پورته;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_PREV;
		}
		إذا (op != EXT2_EXTENT_PREV_SIB) {
#که_تعریف_شوی DEBUG_GET_EXTENT
			چاپ_کړه("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_ښکته_AND_وروستی) ? "down/last" :
			       ((op == EXT2_EXTENT_پورته) ? "up" : "unknown"));
#د_که_پای
		}
	}

	إذا (orig_op == EXT2_EXTENT_وروستی_ونې_پاڼه) {
		إذا ((handle->level < handle->max_ژوروالی) &&
		    (لار->کیڼ == 0))
			op = EXT2_EXTENT_ښکته;
		وإلا
			op = EXT2_EXTENT_وروستی_SIB;
#که_تعریف_شوی DEBUG_GET_EXTENT
		چاپ_کړه("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_ښکته) ? "down" : "last_sib");
#د_که_پای
	}

	بدل (op) {
	حالة EXT2_EXTENT_CURRENT:
		ix = لار->curr;
		اكسر;
	حالة EXT2_EXTENT_ریښه:
		handle->level = 0;
		لار = handle->لار + handle->level;
		/* fallthrough */
	حالة EXT2_EXTENT_لومړی_SIB:
		لار->کیڼ = لار->entries;
		لار->curr = 0;
		/* fallthrough */
	حالة EXT2_EXTENT_بل_SIB:
		إذا (لار->کیڼ <= 0)
			أرجع EXT2_ET_EXTENT_NO_بل;
		إذا (لار->curr) {
			ix = لار->curr;
			ix++;
		} وإلا {
			eh = (بنية ext3_extent_سرلیک *) لار->buf;
			ix = EXT_لومړی_شاخص(eh);
		}
		لار->کیڼ--;
		لار->curr = ix;
		لار->لیدنه_عدد = 0;
		اكسر;
	حالة EXT2_EXTENT_PREV_SIB:
		إذا (!لار->curr ||
		    لار->کیڼ+1 >= لار->entries)
			أرجع EXT2_ET_EXTENT_NO_PREV;
		ix = لار->curr;
		ix--;
		لار->curr = ix;
		لار->کیڼ++;
		إذا (handle->level < handle->max_ژوروالی)
			لار->لیدنه_عدد = 1;
		اكسر;
	حالة EXT2_EXTENT_وروستی_SIB:
		eh = (بنية ext3_extent_سرلیک *) لار->buf;
		لار->curr = EXT_وروستی_EXTENT(eh);
		ix = لار->curr;
		لار->کیڼ = 0;
		لار->لیدنه_عدد = 0;
		اكسر;
	حالة EXT2_EXTENT_پورته:
		إذا (handle->level <= 0)
			أرجع EXT2_ET_EXTENT_NO_پورته;
		handle->level--;
		لار--;
		ix = لار->curr;
		إذا ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_ونې_پاڼه))
			لار->لیدنه_عدد = 0;
		اكسر;
	حالة EXT2_EXTENT_ښکته:
	حالة EXT2_EXTENT_ښکته_AND_وروستی:
		إذا (!لار->curr ||(handle->level >= handle->max_ژوروالی))
			أرجع EXT2_ET_EXTENT_NO_ښکته;

		ix = لار->curr;
		newpath = لار + 1;
		إذا (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			إذا (retval)
				أرجع retval;
		}
		blk = ext2fs_le32_منزل_cpu(ix->ei_ونې_پاڼه) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_ونې_پاڼه_hi) << 32);
		لكل (l = handle->level, tp = لار; l > 0; l--, tp--) {
			إذا (blk == tp->blk)
				أرجع EXT2_ET_EXTENT_دوره;
		}
		newpath->blk = blk;
		إذا ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		وإلا {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			إذا (retval)
				أرجع retval;
		}
		handle->level++;

		eh = (بنية ext3_extent_سرلیک *) newpath->buf;

		retval = ext2fs_extent_سرلیک_verify(eh, handle->fs->blocksize);
		إذا (retval) {
			handle->level--;
			أرجع retval;
		}

		إذا (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_بلاک_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->کیڼ = newpath->entries =
			ext2fs_le16_منزل_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_منزل_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		إذا (newpath->کیڼ <= 0)
			أرجع EXT2_ET_EXTENT_NO_ښکته;

		إذا (لار->کیڼ > 0) {
			ix++;
			newpath->پای_ټکی_blk = ext2fs_le32_منزل_cpu(ix->ei_بلاک);
		} وإلا
			newpath->پای_ټکی_blk = لار->پای_ټکی_blk;

		لار = newpath;
		إذا (op == EXT2_EXTENT_ښکته) {
			ix = EXT_لومړی_شاخص((بنية ext3_extent_سرلیک *) eh);
			لار->curr = ix;
			لار->کیڼ = لار->entries - 1;
			لار->لیدنه_عدد = 0;
		} وإلا {
			ix = EXT_وروستی_شاخص((بنية ext3_extent_سرلیک *) eh);
			لار->curr = ix;
			لار->کیڼ = 0;
			إذا (handle->level < handle->max_ژوروالی)
				لار->لیدنه_عدد = 1;
		}
#که_تعریف_شوی DEBUG_GET_EXTENT
		چاپ_کړه("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_ژوروالی,
			   لار->پای_ټکی_blk);
#د_که_پای
		اكسر;
	افتراضي:
		أرجع EXT2_ET_OP_NOT_SUPPORTED;
	}

	إذا (!ix)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

	extent->e_flags = 0;
#که_تعریف_شوی DEBUG_GET_EXTENT
	چاپ_کړه("(Left %d)\n", لار->کیڼ);
#د_که_پای

	إذا (handle->level == handle->max_ژوروالی) {
		ex = (بنية ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_منزل_cpu(ex->ee_پیل) +
			((__u64) ext2fs_le16_منزل_cpu(ex->ee_پیل_hi) << 32);
		extent->e_lblk = ext2fs_le32_منزل_cpu(ex->ee_بلاک);
		extent->e_len = ext2fs_le16_منزل_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_ونې_پاڼه;
		إذا (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} وإلا {
		extent->e_pblk = ext2fs_le32_منزل_cpu(ix->ei_ونې_پاڼه) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_ونې_پاڼه_hi) << 32);
		extent->e_lblk = ext2fs_le32_منزل_cpu(ix->ei_بلاک);
		إذا (لار->کیڼ > 0) {
			ix++;
			پای_ټکی_blk = ext2fs_le32_منزل_cpu(ix->ei_بلاک);
		} وإلا
			پای_ټکی_blk = لار->پای_ټکی_blk;

		extent->e_len = پای_ټکی_blk - extent->e_lblk;
	}
	إذا (لار->لیدنه_عدد)
		extent->e_flags |= EXT2_EXTENT_FLAGS_دوهم_لیدنه;

	إذا (((orig_op == EXT2_EXTENT_بل_ونې_پاڼه) ||
	     (orig_op == EXT2_EXTENT_PREV_ونې_پاڼه)) &&
	    (handle->level != handle->max_ژوروالی))
		اذهب retry;

	إذا ((orig_op == EXT2_EXTENT_وروستی_ونې_پاڼه) &&
	    ((handle->level != handle->max_ژوروالی) ||
	     (لار->کیڼ != 0)))
		اذهب retry;

	إذا (failed_csum)
		أرجع EXT2_ET_EXTENT_CSUM_INVALID;

	أرجع 0;
}

ساكن errcode_t update_لار(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent_سرلیک	*eh;

	إذا (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} وإلا {
		ix = handle->لار[handle->level - 1].curr;
		blk = ext2fs_le32_منزل_cpu(ix->ei_ونې_پاڼه) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_ونې_پاڼه_hi) << 32);

		/* then update the checksum */
		eh = (بنية ext3_extent_سرلیک *)
				handle->لار[handle->level].buf;
		retval = ext2fs_extent_بلاک_csum_سېټ(handle->fs, handle->ino,
						      eh);
		إذا (retval)
			أرجع retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->لار[handle->level].buf);
	}
	أرجع retval;
}

#إذا 0
errcode_t ext2fs_extent_save_لار(ext2_extent_handle_t handle,
				  ext2_extent_لار_t *ret_لار)
{
	ext2_extent_لار_t	save_لار;
	بنية ext2fs_extent	extent;
	بنية ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_extent_get_info(handle, &info);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(حجم(بنية ext2_extent_لار), &save_لار);
	إذا (retval)
		أرجع retval;
	memset(save_لار, 0, حجم(بنية ext2_extent_لار));

	save_لار->magic = EXT2_ET_MAGIC_EXTENT_لار;
	save_لار->ونې_پاڼه_لوړوالی = info.max_ژوروالی - info.curr_level - 1;
	save_لار->lblk = extent.e_lblk;

	*ret_لار = save_لار;
	أرجع 0;
}

errcode_t ext2fs_extent_free_لار(ext2_extent_لار_t لار)
{
	EXT2_CHECK_MAGIC(لار, EXT2_ET_MAGIC_EXTENT_لار);

	ext2fs_free_mem(&لار);
	أرجع 0;
}
#د_که_پای

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
			      صحيح ونې_پاڼه_level, blk64_t blk)
{
	بنية ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ریښه, &extent);
	إذا (retval) {
		إذا (retval == EXT2_ET_EXTENT_NO_بل)
			retval = EXT2_ET_EXTENT_NOT_وموندل;
		أرجع retval;
	}

	إذا (ونې_پاڼه_level > handle->max_ژوروالی) {
#که_تعریف_شوی DEBUG
		چاپ_کړه("leaf level %d greater than tree depth %d\n",
			ونې_پاڼه_level, handle->max_ژوروالی);
#د_که_پای
		أرجع EXT2_ET_OP_NOT_SUPPORTED;
	}

#که_تعریف_شوی DEBUG
	چاپ_کړه("goto extent ino %u, level %d, %llu\n", handle->ino,
	       ونې_پاڼه_level, blk);
#د_که_پای

#که_تعریف_شوی DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#د_که_پای
	طالما (1) {
		إذا (handle->max_ژوروالی - handle->level == ونې_پاڼه_level) {
			/* block is in this &extent */
			إذا ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				أرجع 0;
			إذا (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				أرجع EXT2_ET_EXTENT_NOT_وموندل;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_بل_SIB,
						   &extent);
			إذا (retval == EXT2_ET_EXTENT_NO_بل)
				أرجع EXT2_ET_EXTENT_NOT_وموندل;
			إذا (retval)
				أرجع retval;
			استمر;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_بل_SIB,
					   &extent);
		إذا (retval == EXT2_ET_EXTENT_NO_بل)
			اذهب go_ښکته;
		إذا (retval)
			أرجع retval;

#که_تعریف_شوی DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#د_که_پای
		إذا (blk == extent.e_lblk)
			اذهب go_ښکته;
		إذا (blk > extent.e_lblk)
			استمر;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		إذا (retval)
			أرجع retval;

#که_تعریف_شوی DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#د_که_پای

	go_ښکته:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ښکته,
					   &extent);
		إذا (retval)
			أرجع retval;

#که_تعریف_شوی DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#د_که_پای
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	أرجع ext2fs_extent_goto2(handle, 0, blk);
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
	صحيح				retval = 0;
	صحيح				orig_لوړوالی;
	blk64_t				پیل;
	بنية extent_لار		*لار;
	بنية ext2fs_extent		extent;
	بنية ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->لار)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

	لار = handle->لار + handle->level;
	إذا (!لار->curr)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب بشپړ;

	/* modified node's start block */
	پیل = extent.e_lblk;

	إذا ((retval = ext2fs_extent_get_info(handle, &info)))
		أرجع retval;
	orig_لوړوالی = info.max_ژوروالی - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	طالما (handle->level > 0 &&
	       (لار->کیڼ == لار->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_پورته, &extent);
		إذا (retval)
			اذهب بشپړ;
		إذا (extent.e_lblk == پیل)
			اكسر;
		لار = handle->لار + handle->level;
		extent.e_len += (extent.e_lblk - پیل);
		extent.e_lblk = پیل;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب بشپړ;
		update_لار(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_لوړوالی, پیل);
بشپړ:
	أرجع retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				صحيح flags EXT2FS_ATTR((ناکارول_شوی)),
				بنية ext2fs_extent *extent)
{
	بنية extent_لار		*لار;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->لار)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

	لار = handle->لار + handle->level;
	إذا (!لار->curr)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

#که_تعریف_شوی DEBUG
	چاپ_کړه("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#د_که_پای

	إذا (handle->level == handle->max_ژوروالی) {
		ex = لار->curr;

		ex->ee_بلاک = ext2fs_cpu_منزل_le32(extent->e_lblk);
		ex->ee_پیل = ext2fs_cpu_منزل_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_پیل_hi = ext2fs_cpu_منزل_le16(extent->e_pblk >> 32);
		إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			إذا (extent->e_len > EXT_UNINIT_MAX_LEN)
				أرجع EXT2_ET_EXTENT_INVALID_اوږدتیا;
			ex->ee_len = ext2fs_cpu_منزل_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} وإلا {
			إذا (extent->e_len > EXT_INIT_MAX_LEN)
				أرجع EXT2_ET_EXTENT_INVALID_اوږدتیا;
			ex->ee_len = ext2fs_cpu_منزل_le16(extent->e_len);
		}
	} وإلا {
		ix = لار->curr;

		ix->ei_ونې_پاڼه = ext2fs_cpu_منزل_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_ونې_پاڼه_hi = ext2fs_cpu_منزل_le16(extent->e_pblk >> 32);
		ix->ei_بلاک = ext2fs_cpu_منزل_le32(extent->e_lblk);
		ix->ei_ناکارول_شوی = 0;
	}
	update_لار(handle);
	أرجع 0;
}

ساكن صحيح splitting_at_eof(بنية ext2_extent_handle *handle,
			    بنية extent_لار *لار)
{
	بنية extent_لار *ppp = لار;
	dump_لار(__func__, handle, لار);

	إذا (handle->level == 0)
		أرجع 0;

	افعل {
		إذا (ppp->کیڼ)
			أرجع 0;
		ppp--;
	} طالما (ppp >= handle->لار);

	أرجع 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
ساكن errcode_t extent_غوټه_وېشل(ext2_extent_handle_t handle,
				   صحيح expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_غوټه_pblk;
	blk64_t				new_غوټه_پیل;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	صحيح				orig_لوړوالی;
	حرف				*بلاک_buf = عدم;
	بنية ext2fs_extent		extent;
	بنية extent_لار		*لار, *newpath = 0;
	بنية ext3_extent_سرلیک	*eh, *neweh;
	صحيح				tocopy;
	صحيح				new_ریښه = 0;
	بنية ext2_extent_info		info;
	صحيح				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->لار)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

#که_تعریف_شوی DEBUG
	چاپ_کړه("splitting node at level %d\n", handle->level);
#د_که_پای
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب بشپړ;

	retval = ext2fs_extent_get_info(handle, &info);
	إذا (retval)
		اذهب بشپړ;

	/* save the position we were originally splitting... */
	orig_لوړوالی = info.max_ژوروالی - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	لار = handle->لار + handle->level;
	eh = (بنية ext3_extent_سرلیک *) لار->buf;
	إذا (handle->level == handle->max_ژوروالی) {
		بنية ext3_extent	*ex;

		ex = EXT_لومړی_EXTENT(eh);
		goal_blk = ext2fs_le32_منزل_cpu(ex->ee_پیل) +
			((__u64) ext2fs_le16_منزل_cpu(ex->ee_پیل_hi) << 32);
	} وإلا {
		بنية ext3_extent_idx	*ix;

		ix = EXT_لومړی_شاخص(eh);
		goal_blk = ext2fs_le32_منزل_cpu(ix->ei_ونې_پاڼه) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_ونې_پاڼه_hi) << 32);
	}
	goal_blk -= EXT2_FS_CLUSTER_نسبت(handle->fs);
	goal_blk &= ~EXT2_FS_CLUSTER_ماسک(handle->fs);

	/* Is there room in the parent for a new entry? */
	إذا (handle->level &&
			(handle->لار[handle->level - 1].entries >=
			 handle->لار[handle->level - 1].max_entries)) {

#که_تعریف_شوی DEBUG
		چاپ_کړه("parent level %d full; splitting it too\n",
							handle->level - 1);
#د_که_پای
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_پورته, &extent);
		إذا (retval)
			اذهب بشپړ;

		retval = extent_غوټه_وېشل(handle, expand_allowed);
		إذا (retval)
			اذهب بشپړ;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_لوړوالی, orig_lblk);
		إذا (retval)
			اذهب بشپړ;
	}

	/* At this point, parent should have room for this split */
	لار = handle->لار + handle->level;
	إذا (!لار->curr)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, لار) : 0;

	/* extent header of the current node we'll split */
	eh = (بنية ext3_extent_سرلیک *)لار->buf;

	/* splitting root level means moving them all out */
	إذا (handle->level == 0) {
		new_ریښه = 1;
		tocopy = ext2fs_le16_منزل_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    حجم(بنية extent_لار),
					    &newpath);
		إذا (retval)
			اذهب بشپړ;
	} وإلا {
		إذا (no_balance)
			tocopy = 1;
		وإلا
			tocopy = ext2fs_le16_منزل_cpu(eh->eh_entries) / 2;
	}

#که_تعریف_شوی DEBUG
	چاپ_کړه("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_منزل_cpu(eh->eh_entries),
				handle->level);
#د_که_پای

	إذا (!tocopy && !no_balance) {
#که_تعریف_شوی DEBUG
		چاپ_کړه("Nothing to copy to new block!\n");
#د_که_پای
		retval = EXT2_ET_CANT_وېشل_EXTENT;
		اذهب بشپړ;
	}

	/* first we need a new block, or can do nothing. */
	بلاک_buf = حافظه_ورکړه(handle->fs->blocksize);
	إذا (!بلاک_buf) {
		retval = ENOMEM;
		اذهب بشپړ;
	}

	إذا (!goal_blk)
		goal_blk = ext2fs_پیدا_کړه_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, بلاک_buf,
				    &new_غوټه_pblk);
	إذا (retval)
		اذهب بشپړ;

#که_تعریف_شوی DEBUG
	چاپ_کړه("will copy to new node at block %lu\n",
	       (بینښه اوږد) new_غوټه_pblk);
#د_که_پای

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (بنية ext3_extent_سرلیک *) بلاک_buf;
	حافظه_نقل(neweh, eh, حجم(بنية ext3_extent_سرلیک));
	neweh->eh_entries = ext2fs_cpu_منزل_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_منزل_le16((handle->fs->blocksize -
			 حجم(بنية ext3_extent_سرلیک)) /
				حجم(بنية ext3_extent));

	/* then the entries for the new block... */
	حافظه_نقل(EXT_لومړی_شاخص(neweh),
		EXT_لومړی_شاخص(eh) +
			(ext2fs_le16_منزل_cpu(eh->eh_entries) - tocopy),
		حجم(بنية ext3_extent_idx) * tocopy);

	new_غوټه_پیل = ext2fs_le32_منزل_cpu(EXT_لومړی_شاخص(neweh)->ei_بلاک);

	/* then update the checksum */
	retval = ext2fs_extent_بلاک_csum_سېټ(handle->fs, handle->ino, neweh);
	إذا (retval)
		اذهب بشپړ;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_غوټه_pblk, 1,
					بلاک_buf);

	إذا (retval)
		اذهب بشپړ;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	إذا (handle->level == 0) {
		حافظه_نقل(newpath, لار,
		       حجم(بنية extent_لار) * handle->max_paths);
		handle->لار = newpath;
		newpath = لار;
		لار = handle->لار;
		لار->entries = 1;
		لار->کیڼ = لار->max_entries - 1;
		handle->max_ژوروالی++;
		handle->max_paths++;
		eh->eh_ژوروالی = ext2fs_cpu_منزل_le16(handle->max_ژوروالی);
	} وإلا {
		لار->entries -= tocopy;
		لار->کیڼ -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_منزل_le16(لار->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_لار(handle);
	إذا (retval)
		اذهب بشپړ;

	/* now go up and insert/replace index for new node we created */
	إذا (new_ریښه) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_لومړی_SIB, &extent);
		إذا (retval)
			اذهب بشپړ;

		extent.e_lblk = new_غوټه_پیل;
		extent.e_pblk = new_غوټه_pblk;
		extent.e_len = handle->لار[0].پای_ټکی_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب بشپړ;
	} وإلا {
		__u32 new_غوټه_اوږدتیا;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_پورته, &extent);
		/* will insert after this one; it's length is shorter now */
		new_غوټه_اوږدتیا = new_غوټه_پیل - extent.e_lblk;
		extent.e_len -= new_غوټه_اوږدتیا;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب بشپړ;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_غوټه_پیل;
		extent.e_pblk = new_غوټه_pblk;
		extent.e_len = new_غوټه_اوږدتیا;
		retval = ext2fs_extent_ورننباسه(handle, EXT2_EXTENT_ورننباسه_AFTER, &extent);
		إذا (retval)
			اذهب بشپړ;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_لوړوالی, orig_lblk);
	إذا (retval)
		اذهب بشپړ;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_جمع_کړه_بلاکونه(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	إذا (retval)
		اذهب بشپړ;

بشپړ:
	إذا (newpath)
		ext2fs_free_mem(&newpath);
	حرر(بلاک_buf);

	أرجع retval;
}

errcode_t ext2fs_extent_غوټه_وېشل(ext2_extent_handle_t handle)
{
	أرجع extent_غوټه_وېشل(handle, 0);
}

errcode_t ext2fs_extent_ورننباسه(ext2_extent_handle_t handle, صحيح flags,
				      بنية ext2fs_extent *extent)
{
	بنية extent_لار		*لار;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent_سرلیک	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->لار)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

#که_تعریف_شوی DEBUG
	چاپ_کړه("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#د_که_پای

	لار = handle->لار + handle->level;

	إذا (لار->entries >= لار->max_entries) {
		إذا (flags & EXT2_EXTENT_ورننباسه_NOSPLIT) {
			أرجع EXT2_ET_CANT_ورننباسه_EXTENT;
		} وإلا {
#که_تعریف_شوی DEBUG
			چاپ_کړه("node full (level %d) - splitting\n",
				   handle->level);
#د_که_پای
			retval = extent_غوټه_وېشل(handle, 1);
			إذا (retval)
				أرجع retval;
			لار = handle->لار + handle->level;
		}
	}

	eh = (بنية ext3_extent_سرلیک *) لار->buf;
	إذا (لار->curr) {
		ix = لار->curr;
		إذا (flags & EXT2_EXTENT_ورننباسه_AFTER) {
			ix++;
			لار->کیڼ--;
		}
	} وإلا {
		ix = EXT_لومړی_شاخص(eh);
		لار->کیڼ = -1;
	}

	لار->curr = ix;

	إذا (لار->کیڼ >= 0)
		memmove(ix + 1, ix,
			(لار->کیڼ+1) * حجم(بنية ext3_extent_idx));
	لار->کیڼ++;
	لار->entries++;

	eh = (بنية ext3_extent_سرلیک *) لار->buf;
	eh->eh_entries = ext2fs_cpu_منزل_le16(لار->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	إذا (retval)
		اذهب errout;

	retval = update_لار(handle);
	إذا (retval)
		اذهب errout;

	أرجع 0;

errout:
	ext2fs_extent_delete(handle, 0);
	أرجع retval;
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

errcode_t ext2fs_extent_سېټ_bmap(ext2_extent_handle_t handle,
				 blk64_t logical, blk64_t physical, صحيح flags)
{
	errcode_t		ec, retval = 0;
	صحيح			mapped = 1; /* logical is mapped? */
	صحيح			orig_لوړوالی;
	صحيح			extent_uninit = 0;
	صحيح			prev_uninit = 0;
	صحيح			بل_uninit = 0;
	صحيح			new_uninit = 0;
	صحيح			max_len = EXT_INIT_MAX_LEN;
	صحيح			has_prev, has_بل;
	blk64_t			orig_lblk;
	بنية extent_لار	*لار;
	بنية ext2fs_extent	extent, بل_extent, prev_extent;
	بنية ext2fs_extent	newextent;
	بنية ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#که_تعریف_شوی DEBUG
	چاپ_کړه("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#د_که_پای

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->لار)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

	لار = handle->لار + handle->level;

	إذا (flags & EXT2_EXTENT_سېټ_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	إذا (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_ونې_پاڼه;
		إذا (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	إذا ((handle->max_ژوروالی == 0) && (لار->entries == 0)) {
		retval = ext2fs_extent_ورننباسه(handle, 0, &newextent);
		أرجع retval;
	}

	/* save our original location in the extent tree */
	إذا ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		إذا (retval != EXT2_ET_NO_CURRENT_غوټه)
			أرجع retval;
		memset(&extent, 0, حجم(extent));
	}
	إذا ((retval = ext2fs_extent_get_info(handle, &info)))
		أرجع retval;
	orig_لوړوالی = info.max_ژوروالی - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval) {
		إذا (retval == EXT2_ET_EXTENT_NOT_وموندل) {
			retval = 0;
			mapped = 0;
			إذا (!physical) {
#که_تعریف_شوی DEBUG
				چاپ_کړه("block %llu already unmapped\n",
					logical);
#د_که_پای
				اذهب بشپړ;
			}
		} وإلا
			اذهب بشپړ;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب بشپړ;
	إذا (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_بل_ونې_پاڼه, &بل_extent);
	إذا (retval) {
		has_بل = 0;
		إذا (retval != EXT2_ET_EXTENT_NO_بل)
			اذهب بشپړ;
	} وإلا {
		dbg_print_extent("set_bmap: next_extent",
				 &بل_extent);
		has_بل = 1;
		إذا (بل_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			بل_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval && retval != EXT2_ET_EXTENT_NOT_وموندل)
		اذهب بشپړ;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_ونې_پاڼه, &prev_extent);
	إذا (retval) {
		has_prev = 0;
		إذا (retval != EXT2_ET_EXTENT_NO_PREV)
			اذهب بشپړ;
	} وإلا {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		إذا (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval && retval != EXT2_ET_EXTENT_NOT_وموندل)
		اذهب بشپړ;

	/* check if already pointing to the requested physical */
	إذا (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#که_تعریف_شوی DEBUG
		چاپ_کړه("physical block (at %llu) unchanged\n", logical);
#د_که_پای
		اذهب بشپړ;
	}

	إذا (!mapped) {
#که_تعریف_شوی DEBUG
		چاپ_کړه("mapping unmapped logical block %llu\n", logical);
#د_که_پای
		إذا ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((صحيح) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} وإلا إذا ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((صحيح) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} وإلا إذا (has_بل &&
			   (logical == بل_extent.e_lblk - 1) &&
			   (physical == بل_extent.e_pblk - 1) &&
			   (new_uninit == بل_uninit) &&
			   ((صحيح) بل_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_بل_ونې_پاڼه,
						   &بل_extent);
			إذا (retval)
				اذهب بشپړ;
			بل_extent.e_len++;
			بل_extent.e_lblk--;
			بل_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &بل_extent);
		} وإلا إذا (logical < extent.e_lblk)
			retval = ext2fs_extent_ورننباسه(handle, 0, &newextent);
		وإلا
			retval = ext2fs_extent_ورننباسه(handle,
				      EXT2_EXTENT_ورننباسه_AFTER, &newextent);
		إذا (retval)
			اذهب بشپړ;
		retval = ext2fs_extent_fix_parents(handle);
		إذا (retval)
			اذهب بشپړ;
	} وإلا إذا ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#که_تعریف_شوی DEBUG
		چاپ_کړه("(re/un)mapping only block in extent\n");
#د_که_پای
		إذا (physical) {
			إذا (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحيح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_ونې_پاڼه, &prev_extent);
				إذا (retval)
					اذهب بشپړ;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_بل_ونې_پاڼه,
							   &extent);
				إذا (retval)
					اذهب بشپړ;
				اذهب delete_غوټه;

			} وإلا
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} وإلا {
		delete_غوټه:
			retval = ext2fs_extent_delete(handle, 0);
			إذا (retval)
				اذهب بشپړ;
			ec = ext2fs_extent_fix_parents(handle);
			إذا (ec != EXT2_ET_NO_CURRENT_غوټه)
				retval = ec;
		}

		إذا (retval)
			اذهب بشپړ;
	} وإلا إذا (logical == extent.e_lblk + extent.e_len - 1)  {
#که_تعریف_شوی DEBUG
		چاپ_کړه("(re/un)mapping last block in extent\n");
#د_که_پای
		إذا (physical) {
			إذا (has_بل &&
			    (logical == (بل_extent.e_lblk - 1)) &&
			    (physical == (بل_extent.e_pblk - 1)) &&
			    (new_uninit == بل_uninit) &&
			    ((صحيح) بل_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_بل_ونې_پاڼه, &بل_extent);
				إذا (retval)
					اذهب بشپړ;
				بل_extent.e_len++;
				بل_extent.e_lblk--;
				بل_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &بل_extent);
				إذا (retval)
					اذهب بشپړ;
			} وإلا
				retval = ext2fs_extent_ورننباسه(handle,
				      EXT2_EXTENT_ورننباسه_AFTER, &newextent);
			إذا (retval)
				اذهب بشپړ;
			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval)
				اذهب بشپړ;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			إذا (retval)
				اذهب بشپړ;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			إذا (retval)
				اذهب بشپړ;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب بشپړ;
	} وإلا إذا (logical == extent.e_lblk) {
#که_تعریف_شوی DEBUG
		چاپ_کړه("(re/un)mapping first block in extent\n");
#د_که_پای
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب بشپړ;
		retval = ext2fs_extent_fix_parents(handle);
		إذا (retval)
			اذهب بشپړ;
		إذا (physical) {
			إذا (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحيح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_ونې_پاڼه, &prev_extent);
				إذا (retval)
					اذهب بشپړ;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} وإلا
				retval = ext2fs_extent_ورننباسه(handle,
							      0, &newextent);
			إذا (retval)
				اذهب بشپړ;
			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval)
				اذهب بشپړ;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_بل_ونې_پاڼه,
						   &extent);
			إذا (retval)
				اذهب بشپړ;
		}
	} وإلا {
		__u32	save_اوږدتیا;
		blk64_t	save_lblk;
		بنية ext2fs_extent save_extent;
		errcode_t r2;

#که_تعریف_شوی DEBUG
		چاپ_کړه("(re/un)mapping in middle of extent\n");
#د_که_پای
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_اوږدتیا = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب بشپړ;
		/* insert our new extent, if any */
		إذا (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_ورننباسه(handle,
					EXT2_EXTENT_ورننباسه_AFTER, &newextent);
			إذا (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				إذا (r2 == 0)
					(فراغ)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				اذهب بشپړ;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_اوږدتیا - extent.e_len - 1;
		retval = ext2fs_extent_ورننباسه(handle,
				EXT2_EXTENT_ورننباسه_AFTER, &extent);
		إذا (retval) {
			إذا (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				إذا (r2 == 0)
					(فراغ)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			إذا (r2 == 0)
				(فراغ)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			اذهب بشپړ;
		}
	}

بشپړ:
	/* get handle back to its position */
	إذا (orig_لوړوالی > handle->max_ژوروالی)
		orig_لوړوالی = handle->max_ژوروالی; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_لوړوالی, orig_lblk);
	أرجع retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحيح flags)
{
	بنية extent_لار		*لار;
	حرف 				*cp;
	بنية ext3_extent_سرلیک	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->لار)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

#که_تعریف_شوی DEBUG
	{
		بنية ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		إذا (retval == 0) {
			چاپ_کړه("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#د_که_پای

	لار = handle->لار + handle->level;
	إذا (!لار->curr)
		أرجع EXT2_ET_NO_CURRENT_غوټه;

	cp = لار->curr;

	/* Sanity check before memmove() */
	إذا (لار->کیڼ < 0)
		أرجع EXT2_ET_EXTENT_ونې_پاڼه_BAD;

	إذا (لار->کیڼ) {
		memmove(cp, cp + حجم(بنية ext3_extent_idx),
			لار->کیڼ * حجم(بنية ext3_extent_idx));
		لار->کیڼ--;
	} وإلا {
		بنية ext3_extent_idx	*ix = لار->curr;
		ix--;
		لار->curr = ix;
	}
	إذا (--لار->entries == 0)
		لار->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	إذا (لار->entries == 0 && handle->level) {
		إذا (!(flags & EXT2_EXTENT_DELETE_KEEP_تشه)) {
			بنية ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_پورته,
								&extent);
			إذا (retval)
				أرجع retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_بلاکونه -=
				(handle->fs->blocksize *
				 EXT2_FS_CLUSTER_نسبت(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_بلاک_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} وإلا {
		eh = (بنية ext3_extent_سرلیک *) لار->buf;
		eh->eh_entries = ext2fs_cpu_منزل_le16(لار->entries);
		إذا ((لار->entries == 0) && (handle->level == 0)) {
			eh->eh_ژوروالی = 0;
			handle->max_ژوروالی = 0;
		}
		retval = update_لار(handle);
	}
	أرجع retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 بنية ext2_extent_info *info)
{
	بنية extent_لار		*لار;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, حجم(بنية ext2_extent_info));

	لار = handle->لار + handle->level;
	إذا (لار) {
		إذا (لار->curr)
			info->curr_ثبت = ((حرف *) لار->curr - لار->buf) /
				حجم(بنية ext3_extent_idx);
		وإلا
			info->curr_ثبت = 0;
		info->عدد_entries = لار->entries;
		info->max_entries = لار->max_entries;
		info->بایټونه_avail = (لار->max_entries - لار->entries) *
			حجم(بنية ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_ژوروالی = handle->max_ژوروالی;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	أرجع 0;
}

size_t ext2fs_max_extent_ژوروالی(ext2_extent_handle_t handle)
{
	size_t iblock_sz = حجم(((بنية ext2_inode *)عدم)->i_بلاک);
	size_t iblock_extents = (iblock_sz - حجم(بنية ext3_extent_سرلیک)) /
				حجم(بنية ext3_extent);
	size_t extents_فی_بلاک = (handle->fs->blocksize -
				    حجم(بنية ext3_extent_سرلیک)) /
				   حجم(بنية ext3_extent);
	ساكن بینښه صحيح وروستی_blocksize = 0;
	ساكن size_t وروستی_پایله = 0;

	إذا (وروستی_blocksize && وروستی_blocksize == handle->fs->blocksize)
		أرجع وروستی_پایله;

	وروستی_پایله = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_فی_بلاک));
	وروستی_blocksize = handle->fs->blocksize;
	أرجع وروستی_پایله;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       بنية ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	بنية ext2fs_extent	extent;
	errcode_t		errcode;
	صحيح    			save_flags = fs->flags;

	إذا (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		أرجع 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	إذا (errcode) {
		إذا (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		أرجع errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ریښه, &extent);
	إذا (errcode)
		اذهب بهر;

	افعل {
		/* Skip to the end of a block of leaf nodes */
		إذا (extent.e_flags & EXT2_EXTENT_FLAGS_ونې_پاڼه) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_وروستی_SIB,
						    &extent);
			إذا (errcode)
				اكسر;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_بل, &extent);
		إذا (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_لار(handle);
	} طالما (errcode == 0);

بهر:
	/* Ok if we run off the end */
	إذا (errcode == EXT2_ET_EXTENT_NO_بل)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	أرجع errcode;
}

errcode_t ext2fs_decode_extent(بنية ext2fs_extent *منزل, فراغ *addr, صحيح len)
{
	بنية ext3_extent *from = (بنية ext3_extent *)addr;

	إذا (len != حجم(بنية ext3_extent))
		أرجع EXT2_ET_INVALID_دلیل;

	منزل->e_pblk = ext2fs_le32_منزل_cpu(from->ee_پیل) +
		((__u64) ext2fs_le16_منزل_cpu(from->ee_پیل_hi)
			<< 32);
	منزل->e_lblk = ext2fs_le32_منزل_cpu(from->ee_بلاک);
	منزل->e_len = ext2fs_le16_منزل_cpu(from->ee_len);
	منزل->e_flags = EXT2_EXTENT_FLAGS_ونې_پاڼه;
	إذا (منزل->e_len > EXT_INIT_MAX_LEN) {
		منزل->e_len -= EXT_INIT_MAX_LEN;
		منزل->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	أرجع 0;
}

errcode_t ext2fs_شمېر_بلاکونه(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode, blk64_t *ret_شمېر)
{
	ext2_extent_handle_t	handle = عدم;
	بنية ext2fs_extent	extent;
	errcode_t		errcode;
	صحيح			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	إذا (errcode)
		اذهب بهر;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ریښه, &extent);
	إذا (errcode)
		اذهب بهر;

	errcode = ext2fs_get_array(handle->max_ژوروالی, حجم(blk64_t),
				   &intermediate_nodes);
	إذا (errcode)
		اذهب بهر;

	blkcount = handle->level;
	طالما (!errcode) {
		إذا (extent.e_flags & EXT2_EXTENT_FLAGS_ونې_پاڼه) {
			blkcount += extent.e_len;
			لكل (i = 0; i < handle->level; i++) {
				إذا (intermediate_nodes[i] !=
					handle->لار[i].پای_ټکی_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->لار[i].پای_ټکی_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_بل, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
بهر:
	*ret_شمېر = blkcount;
	ext2fs_extent_free(handle);

	أرجع 0;
}

#که_تعریف_شوی DEBUG
/*
 * Override debugfs's prompt
 */
ثابت حرف *debug_prog_name = "tst_extents";

#د_که_پای

