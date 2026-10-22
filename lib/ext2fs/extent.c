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
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "e2image.h"

#لغو_تعریف DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

بنية extent_مسیر {
	حرف		*buf;
	صحيح		entries;
	صحيح		max_entries;
	صحيح		چپ;
	صحيح		بازدید_رقم;
	صحيح		flags;
	blk64_t		انتها_blk;
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
	صحيح			max_عمق;
	صحيح			max_paths;
	بنية extent_مسیر	*مسیر;
};

بنية ext2_extent_مسیر {
	errcode_t		magic;
	صحيح			برگ_ارتفاع;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#اگر_تعریف_شده DEBUG
ساكن فراغ dbg_show_سرآیند(بنية ext3_extent_سرآیند *eh)
{
	چاپ_کن("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_مقصد_cpu(eh->eh_magic),
			ext2fs_le16_مقصد_cpu(eh->eh_entries),
			ext2fs_le16_مقصد_cpu(eh->eh_max),
			ext2fs_le16_مقصد_cpu(eh->eh_عمق),
			ext2fs_le32_مقصد_cpu(eh->eh_generation));
}

ساكن فراغ dbg_show_شاخص(بنية ext3_extent_idx *ix)
{
	چاپ_کن("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_مقصد_cpu(ix->ei_بلوک),
			ext2fs_le32_مقصد_cpu(ix->ei_برگ),
			ext2fs_le16_مقصد_cpu(ix->ei_برگ_hi),
			ext2fs_le16_مقصد_cpu(ix->ei_استفادهنشده));
}

ساكن فراغ dbg_show_extent(بنية ext3_extent *ex)
{
	چاپ_کن("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_مقصد_cpu(ex->ee_بلوک),
			ext2fs_le32_مقصد_cpu(ex->ee_بلوک) +
			ext2fs_le16_مقصد_cpu(ex->ee_len) - 1,
			ext2fs_le16_مقصد_cpu(ex->ee_len),
			ext2fs_le32_مقصد_cpu(ex->ee_شروع),
			ext2fs_le16_مقصد_cpu(ex->ee_شروع_hi));
}

ساكن فراغ dbg_print_extent(حرف *desc, بنية ext2fs_extent *extent)
{
	إذا (desc)
		چاپ_کن("%s: ", desc);
	چاپ_کن("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_برگ)
		fputs("LEAF ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_دومی_بازدید)
		fputs("2ND_VISIT ", stdout);
	إذا (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

ساكن فراغ dump_مسیر(ثابت حرف *برچسب, بنية ext2_extent_handle *handle,
		      بنية extent_مسیر *مسیر)
{
	بنية extent_مسیر *ppp = مسیر;
	چاپ_کن("%s: level=%d\n", برچسب, handle->level);

	افعل {
		چاپ_کن("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       برچسب, (ppp - handle->مسیر), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->چپ, ppp->بازدید_رقم, ppp->flags,
		       ppp->انتها_blk, ppp->curr, ppp->curr - (فراغ *)ppp->buf);
		چاپ_کن("  ");
		dbg_show_سرآیند((بنية ext3_extent_سرآیند *)ppp->buf);
		إذا (ppp->curr) {
			چاپ_کن("  ");
			dbg_show_شاخص(ppp->curr);
			چاپ_کن("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} طالما (ppp >= handle->مسیر);
	fflush(stdout);

	أرجع;
}

#وإلا
#تعریف_کن dbg_show_سرآیند(eh) افعل { } طالما (0)
#تعریف_کن dbg_show_شاخص(ix) افعل { } طالما (0)
#تعریف_کن dbg_show_extent(ex) افعل { } طالما (0)
#تعریف_کن dbg_print_extent(desc, ex) افعل { } طالما (0)
#تعریف_کن dump_مسیر(برچسب, handle, مسیر) افعل { } طالما (0)
#پایان_اگر

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_سرآیند_verify(فراغ *ptr, صحيح اندازه)
{
	صحيح eh_max, مدخل_اندازه;
	بنية ext3_extent_سرآیند *eh = ptr;

	dbg_show_سرآیند(eh);
	إذا (ext2fs_le16_مقصد_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		أرجع EXT2_ET_EXTENT_سرآیند_BAD;
	إذا (ext2fs_le16_مقصد_cpu(eh->eh_entries) > ext2fs_le16_مقصد_cpu(eh->eh_max))
		أرجع EXT2_ET_EXTENT_سرآیند_BAD;
	إذا (eh->eh_عمق == 0)
		مدخل_اندازه = حجم(بنية ext3_extent);
	وإلا
		مدخل_اندازه = حجم(بنية ext3_extent_idx);

	eh_max = (اندازه - حجم(*eh)) / مدخل_اندازه;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	إذا ((ext2fs_le16_مقصد_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_مقصد_cpu(eh->eh_max) < (eh_max - 2)))
		أرجع EXT2_ET_EXTENT_سرآیند_BAD;

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

	إذا (handle->مسیر) {
		لكل (i = 1; i < handle->max_paths; i++) {
			إذا (handle->مسیر[i].buf)
				ext2fs_free_mem(&handle->مسیر[i].buf);
		}
		ext2fs_free_mem(&handle->مسیر);
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
	بنية ext3_extent_سرآیند	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!inode)
		إذا ((ino == 0) || (ino > fs->super->s_inodes_شمار))
			أرجع EXT2_ET_BAD_INODE_رقم;

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

	eh = (بنية ext3_extent_سرآیند *) &handle->inode->i_بلوک[0];

	لكل (i=0; i < EXT2_N_بلوکها; i++)
		إذا (handle->inode->i_بلوک[i])
			اكسر;
	إذا (i >= EXT2_N_بلوکها) {
		eh->eh_magic = ext2fs_cpu_مقصد_le16(EXT3_EXT_MAGIC);
		eh->eh_عمق = 0;
		eh->eh_entries = 0;
		i = (حجم(handle->inode->i_بلوک) - حجم(*eh)) /
			حجم(بنية ext3_extent);
		eh->eh_max = ext2fs_cpu_مقصد_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	إذا (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		اذهب errout;
	}

	retval = ext2fs_extent_سرآیند_verify(eh, حجم(handle->inode->i_بلوک));
	إذا (retval)
		اذهب errout;

	handle->max_عمق = ext2fs_le16_مقصد_cpu(eh->eh_عمق);
	handle->type = ext2fs_le16_مقصد_cpu(eh->eh_magic);

	handle->max_paths = handle->max_عمق + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    حجم(بنية extent_مسیر),
				    &handle->مسیر);
	handle->مسیر[0].buf = (حرف *) handle->inode->i_بلوک;

	handle->مسیر[0].چپ = handle->مسیر[0].entries =
		ext2fs_le16_مقصد_cpu(eh->eh_entries);
	handle->مسیر[0].max_entries = ext2fs_le16_مقصد_cpu(eh->eh_max);
	handle->مسیر[0].curr = 0;
	handle->مسیر[0].انتها_blk =
		(EXT2_I_اندازه(handle->inode) + fs->blocksize - 1) >>
		 EXT2_بلوک_اندازه_بیتها(fs->super);
	handle->مسیر[0].blk = 0;
	handle->مسیر[0].بازدید_رقم = 1;
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
	بنية extent_مسیر	*مسیر, *newpath, *tp;
	بنية ext3_extent_سرآیند	*eh;
	بنية ext3_extent_idx		*ix = 0;
	بنية ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				انتها_blk;
	صحيح				orig_op, op, l;
	صحيح				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!handle->مسیر)
		أرجع EXT2_ET_NO_CURRENT_گره;

	orig_op = op = flags & EXT2_EXTENT_MOVE_ماسک;

retry:
	مسیر = handle->مسیر + handle->level;
	إذا ((orig_op == EXT2_EXTENT_بعدی) ||
	    (orig_op == EXT2_EXTENT_بعدی_برگ)) {
		إذا (handle->level < handle->max_عمق) {
			/* interior node */
			إذا (مسیر->بازدید_رقم == 0) {
				مسیر->بازدید_رقم++;
				op = EXT2_EXTENT_پایین;
			} وإلا إذا (مسیر->چپ > 0)
				op = EXT2_EXTENT_بعدی_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_به_بالا;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_بعدی;
		} وإلا {
			/* leaf node */
			إذا (مسیر->چپ > 0)
				op = EXT2_EXTENT_بعدی_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_به_بالا;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_بعدی;
		}
		إذا (op != EXT2_EXTENT_بعدی_SIB) {
#اگر_تعریف_شده DEBUG_GET_EXTENT
			چاپ_کن("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_پایین) ? "down" :
			       ((op == EXT2_EXTENT_به_بالا) ? "up" : "unknown"));
#پایان_اگر
		}
	}

	إذا ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_برگ)) {
		إذا (handle->level < handle->max_عمق) {
			/* interior node */
			إذا (مسیر->بازدید_رقم > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_پایین_AND_آخرین;
			} وإلا إذا (مسیر->چپ < مسیر->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_به_بالا;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_PREV;
		} وإلا {
			/* leaf node */
			إذا (مسیر->چپ < مسیر->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_به_بالا;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_PREV;
		}
		إذا (op != EXT2_EXTENT_PREV_SIB) {
#اگر_تعریف_شده DEBUG_GET_EXTENT
			چاپ_کن("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_پایین_AND_آخرین) ? "down/last" :
			       ((op == EXT2_EXTENT_به_بالا) ? "up" : "unknown"));
#پایان_اگر
		}
	}

	إذا (orig_op == EXT2_EXTENT_آخرین_برگ) {
		إذا ((handle->level < handle->max_عمق) &&
		    (مسیر->چپ == 0))
			op = EXT2_EXTENT_پایین;
		وإلا
			op = EXT2_EXTENT_آخرین_SIB;
#اگر_تعریف_شده DEBUG_GET_EXTENT
		چاپ_کن("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_پایین) ? "down" : "last_sib");
#پایان_اگر
	}

	بدل (op) {
	حالة EXT2_EXTENT_CURRENT:
		ix = مسیر->curr;
		اكسر;
	حالة EXT2_EXTENT_ریشه:
		handle->level = 0;
		مسیر = handle->مسیر + handle->level;
		/* fallthrough */
	حالة EXT2_EXTENT_اولی_SIB:
		مسیر->چپ = مسیر->entries;
		مسیر->curr = 0;
		/* fallthrough */
	حالة EXT2_EXTENT_بعدی_SIB:
		إذا (مسیر->چپ <= 0)
			أرجع EXT2_ET_EXTENT_NO_بعدی;
		إذا (مسیر->curr) {
			ix = مسیر->curr;
			ix++;
		} وإلا {
			eh = (بنية ext3_extent_سرآیند *) مسیر->buf;
			ix = EXT_اولی_شاخص(eh);
		}
		مسیر->چپ--;
		مسیر->curr = ix;
		مسیر->بازدید_رقم = 0;
		اكسر;
	حالة EXT2_EXTENT_PREV_SIB:
		إذا (!مسیر->curr ||
		    مسیر->چپ+1 >= مسیر->entries)
			أرجع EXT2_ET_EXTENT_NO_PREV;
		ix = مسیر->curr;
		ix--;
		مسیر->curr = ix;
		مسیر->چپ++;
		إذا (handle->level < handle->max_عمق)
			مسیر->بازدید_رقم = 1;
		اكسر;
	حالة EXT2_EXTENT_آخرین_SIB:
		eh = (بنية ext3_extent_سرآیند *) مسیر->buf;
		مسیر->curr = EXT_آخرین_EXTENT(eh);
		ix = مسیر->curr;
		مسیر->چپ = 0;
		مسیر->بازدید_رقم = 0;
		اكسر;
	حالة EXT2_EXTENT_به_بالا:
		إذا (handle->level <= 0)
			أرجع EXT2_ET_EXTENT_NO_به_بالا;
		handle->level--;
		مسیر--;
		ix = مسیر->curr;
		إذا ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_برگ))
			مسیر->بازدید_رقم = 0;
		اكسر;
	حالة EXT2_EXTENT_پایین:
	حالة EXT2_EXTENT_پایین_AND_آخرین:
		إذا (!مسیر->curr ||(handle->level >= handle->max_عمق))
			أرجع EXT2_ET_EXTENT_NO_پایین;

		ix = مسیر->curr;
		newpath = مسیر + 1;
		إذا (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			إذا (retval)
				أرجع retval;
		}
		blk = ext2fs_le32_مقصد_cpu(ix->ei_برگ) +
			((__u64) ext2fs_le16_مقصد_cpu(ix->ei_برگ_hi) << 32);
		لكل (l = handle->level, tp = مسیر; l > 0; l--, tp--) {
			إذا (blk == tp->blk)
				أرجع EXT2_ET_EXTENT_چرخه;
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

		eh = (بنية ext3_extent_سرآیند *) newpath->buf;

		retval = ext2fs_extent_سرآیند_verify(eh, handle->fs->blocksize);
		إذا (retval) {
			handle->level--;
			أرجع retval;
		}

		إذا (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_بلوک_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->چپ = newpath->entries =
			ext2fs_le16_مقصد_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_مقصد_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		إذا (newpath->چپ <= 0)
			أرجع EXT2_ET_EXTENT_NO_پایین;

		إذا (مسیر->چپ > 0) {
			ix++;
			newpath->انتها_blk = ext2fs_le32_مقصد_cpu(ix->ei_بلوک);
		} وإلا
			newpath->انتها_blk = مسیر->انتها_blk;

		مسیر = newpath;
		إذا (op == EXT2_EXTENT_پایین) {
			ix = EXT_اولی_شاخص((بنية ext3_extent_سرآیند *) eh);
			مسیر->curr = ix;
			مسیر->چپ = مسیر->entries - 1;
			مسیر->بازدید_رقم = 0;
		} وإلا {
			ix = EXT_آخرین_شاخص((بنية ext3_extent_سرآیند *) eh);
			مسیر->curr = ix;
			مسیر->چپ = 0;
			إذا (handle->level < handle->max_عمق)
				مسیر->بازدید_رقم = 1;
		}
#اگر_تعریف_شده DEBUG_GET_EXTENT
		چاپ_کن("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_عمق,
			   مسیر->انتها_blk);
#پایان_اگر
		اكسر;
	افتراضي:
		أرجع EXT2_ET_OP_NOT_SUPPORTED;
	}

	إذا (!ix)
		أرجع EXT2_ET_NO_CURRENT_گره;

	extent->e_flags = 0;
#اگر_تعریف_شده DEBUG_GET_EXTENT
	چاپ_کن("(Left %d)\n", مسیر->چپ);
#پایان_اگر

	إذا (handle->level == handle->max_عمق) {
		ex = (بنية ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_مقصد_cpu(ex->ee_شروع) +
			((__u64) ext2fs_le16_مقصد_cpu(ex->ee_شروع_hi) << 32);
		extent->e_lblk = ext2fs_le32_مقصد_cpu(ex->ee_بلوک);
		extent->e_len = ext2fs_le16_مقصد_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_برگ;
		إذا (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} وإلا {
		extent->e_pblk = ext2fs_le32_مقصد_cpu(ix->ei_برگ) +
			((__u64) ext2fs_le16_مقصد_cpu(ix->ei_برگ_hi) << 32);
		extent->e_lblk = ext2fs_le32_مقصد_cpu(ix->ei_بلوک);
		إذا (مسیر->چپ > 0) {
			ix++;
			انتها_blk = ext2fs_le32_مقصد_cpu(ix->ei_بلوک);
		} وإلا
			انتها_blk = مسیر->انتها_blk;

		extent->e_len = انتها_blk - extent->e_lblk;
	}
	إذا (مسیر->بازدید_رقم)
		extent->e_flags |= EXT2_EXTENT_FLAGS_دومی_بازدید;

	إذا (((orig_op == EXT2_EXTENT_بعدی_برگ) ||
	     (orig_op == EXT2_EXTENT_PREV_برگ)) &&
	    (handle->level != handle->max_عمق))
		اذهب retry;

	إذا ((orig_op == EXT2_EXTENT_آخرین_برگ) &&
	    ((handle->level != handle->max_عمق) ||
	     (مسیر->چپ != 0)))
		اذهب retry;

	إذا (failed_csum)
		أرجع EXT2_ET_EXTENT_CSUM_INVALID;

	أرجع 0;
}

ساكن errcode_t update_مسیر(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent_سرآیند	*eh;

	إذا (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} وإلا {
		ix = handle->مسیر[handle->level - 1].curr;
		blk = ext2fs_le32_مقصد_cpu(ix->ei_برگ) +
			((__u64) ext2fs_le16_مقصد_cpu(ix->ei_برگ_hi) << 32);

		/* then update the checksum */
		eh = (بنية ext3_extent_سرآیند *)
				handle->مسیر[handle->level].buf;
		retval = ext2fs_extent_بلوک_csum_مجموعه(handle->fs, handle->ino,
						      eh);
		إذا (retval)
			أرجع retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->مسیر[handle->level].buf);
	}
	أرجع retval;
}

#إذا 0
errcode_t ext2fs_extent_save_مسیر(ext2_extent_handle_t handle,
				  ext2_extent_مسیر_t *ret_مسیر)
{
	ext2_extent_مسیر_t	save_مسیر;
	بنية ext2fs_extent	extent;
	بنية ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_extent_get_info(handle, &info);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(حجم(بنية ext2_extent_مسیر), &save_مسیر);
	إذا (retval)
		أرجع retval;
	memset(save_مسیر, 0, حجم(بنية ext2_extent_مسیر));

	save_مسیر->magic = EXT2_ET_MAGIC_EXTENT_مسیر;
	save_مسیر->برگ_ارتفاع = info.max_عمق - info.curr_level - 1;
	save_مسیر->lblk = extent.e_lblk;

	*ret_مسیر = save_مسیر;
	أرجع 0;
}

errcode_t ext2fs_extent_free_مسیر(ext2_extent_مسیر_t مسیر)
{
	EXT2_CHECK_MAGIC(مسیر, EXT2_ET_MAGIC_EXTENT_مسیر);

	ext2fs_free_mem(&مسیر);
	أرجع 0;
}
#پایان_اگر

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
			      صحيح برگ_level, blk64_t blk)
{
	بنية ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ریشه, &extent);
	إذا (retval) {
		إذا (retval == EXT2_ET_EXTENT_NO_بعدی)
			retval = EXT2_ET_EXTENT_NOT_یافت;
		أرجع retval;
	}

	إذا (برگ_level > handle->max_عمق) {
#اگر_تعریف_شده DEBUG
		چاپ_کن("leaf level %d greater than tree depth %d\n",
			برگ_level, handle->max_عمق);
#پایان_اگر
		أرجع EXT2_ET_OP_NOT_SUPPORTED;
	}

#اگر_تعریف_شده DEBUG
	چاپ_کن("goto extent ino %u, level %d, %llu\n", handle->ino,
	       برگ_level, blk);
#پایان_اگر

#اگر_تعریف_شده DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#پایان_اگر
	طالما (1) {
		إذا (handle->max_عمق - handle->level == برگ_level) {
			/* block is in this &extent */
			إذا ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				أرجع 0;
			إذا (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				أرجع EXT2_ET_EXTENT_NOT_یافت;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_بعدی_SIB,
						   &extent);
			إذا (retval == EXT2_ET_EXTENT_NO_بعدی)
				أرجع EXT2_ET_EXTENT_NOT_یافت;
			إذا (retval)
				أرجع retval;
			استمر;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_بعدی_SIB,
					   &extent);
		إذا (retval == EXT2_ET_EXTENT_NO_بعدی)
			اذهب go_پایین;
		إذا (retval)
			أرجع retval;

#اگر_تعریف_شده DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#پایان_اگر
		إذا (blk == extent.e_lblk)
			اذهب go_پایین;
		إذا (blk > extent.e_lblk)
			استمر;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		إذا (retval)
			أرجع retval;

#اگر_تعریف_شده DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#پایان_اگر

	go_پایین:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_پایین,
					   &extent);
		إذا (retval)
			أرجع retval;

#اگر_تعریف_شده DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#پایان_اگر
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
	صحيح				orig_ارتفاع;
	blk64_t				شروع;
	بنية extent_مسیر		*مسیر;
	بنية ext2fs_extent		extent;
	بنية ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->مسیر)
		أرجع EXT2_ET_NO_CURRENT_گره;

	مسیر = handle->مسیر + handle->level;
	إذا (!مسیر->curr)
		أرجع EXT2_ET_NO_CURRENT_گره;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب انجامشده;

	/* modified node's start block */
	شروع = extent.e_lblk;

	إذا ((retval = ext2fs_extent_get_info(handle, &info)))
		أرجع retval;
	orig_ارتفاع = info.max_عمق - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	طالما (handle->level > 0 &&
	       (مسیر->چپ == مسیر->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_به_بالا, &extent);
		إذا (retval)
			اذهب انجامشده;
		إذا (extent.e_lblk == شروع)
			اكسر;
		مسیر = handle->مسیر + handle->level;
		extent.e_len += (extent.e_lblk - شروع);
		extent.e_lblk = شروع;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب انجامشده;
		update_مسیر(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_ارتفاع, شروع);
انجامشده:
	أرجع retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				صحيح flags EXT2FS_ATTR((استفادهنشده)),
				بنية ext2fs_extent *extent)
{
	بنية extent_مسیر		*مسیر;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->مسیر)
		أرجع EXT2_ET_NO_CURRENT_گره;

	مسیر = handle->مسیر + handle->level;
	إذا (!مسیر->curr)
		أرجع EXT2_ET_NO_CURRENT_گره;

#اگر_تعریف_شده DEBUG
	چاپ_کن("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#پایان_اگر

	إذا (handle->level == handle->max_عمق) {
		ex = مسیر->curr;

		ex->ee_بلوک = ext2fs_cpu_مقصد_le32(extent->e_lblk);
		ex->ee_شروع = ext2fs_cpu_مقصد_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_شروع_hi = ext2fs_cpu_مقصد_le16(extent->e_pblk >> 32);
		إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			إذا (extent->e_len > EXT_UNINIT_MAX_LEN)
				أرجع EXT2_ET_EXTENT_INVALID_درازی;
			ex->ee_len = ext2fs_cpu_مقصد_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} وإلا {
			إذا (extent->e_len > EXT_INIT_MAX_LEN)
				أرجع EXT2_ET_EXTENT_INVALID_درازی;
			ex->ee_len = ext2fs_cpu_مقصد_le16(extent->e_len);
		}
	} وإلا {
		ix = مسیر->curr;

		ix->ei_برگ = ext2fs_cpu_مقصد_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_برگ_hi = ext2fs_cpu_مقصد_le16(extent->e_pblk >> 32);
		ix->ei_بلوک = ext2fs_cpu_مقصد_le32(extent->e_lblk);
		ix->ei_استفادهنشده = 0;
	}
	update_مسیر(handle);
	أرجع 0;
}

ساكن صحيح splitting_at_eof(بنية ext2_extent_handle *handle,
			    بنية extent_مسیر *مسیر)
{
	بنية extent_مسیر *ppp = مسیر;
	dump_مسیر(__func__, handle, مسیر);

	إذا (handle->level == 0)
		أرجع 0;

	افعل {
		إذا (ppp->چپ)
			أرجع 0;
		ppp--;
	} طالما (ppp >= handle->مسیر);

	أرجع 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
ساكن errcode_t extent_گره_تقسیم(ext2_extent_handle_t handle,
				   صحيح expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_گره_pblk;
	blk64_t				new_گره_شروع;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	صحيح				orig_ارتفاع;
	حرف				*بلوک_buf = عدم;
	بنية ext2fs_extent		extent;
	بنية extent_مسیر		*مسیر, *newpath = 0;
	بنية ext3_extent_سرآیند	*eh, *neweh;
	صحيح				tocopy;
	صحيح				new_ریشه = 0;
	بنية ext2_extent_info		info;
	صحيح				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->مسیر)
		أرجع EXT2_ET_NO_CURRENT_گره;

#اگر_تعریف_شده DEBUG
	چاپ_کن("splitting node at level %d\n", handle->level);
#پایان_اگر
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب انجامشده;

	retval = ext2fs_extent_get_info(handle, &info);
	إذا (retval)
		اذهب انجامشده;

	/* save the position we were originally splitting... */
	orig_ارتفاع = info.max_عمق - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	مسیر = handle->مسیر + handle->level;
	eh = (بنية ext3_extent_سرآیند *) مسیر->buf;
	إذا (handle->level == handle->max_عمق) {
		بنية ext3_extent	*ex;

		ex = EXT_اولی_EXTENT(eh);
		goal_blk = ext2fs_le32_مقصد_cpu(ex->ee_شروع) +
			((__u64) ext2fs_le16_مقصد_cpu(ex->ee_شروع_hi) << 32);
	} وإلا {
		بنية ext3_extent_idx	*ix;

		ix = EXT_اولی_شاخص(eh);
		goal_blk = ext2fs_le32_مقصد_cpu(ix->ei_برگ) +
			((__u64) ext2fs_le16_مقصد_cpu(ix->ei_برگ_hi) << 32);
	}
	goal_blk -= EXT2_FS_CLUSTER_نسبت(handle->fs);
	goal_blk &= ~EXT2_FS_CLUSTER_ماسک(handle->fs);

	/* Is there room in the parent for a new entry? */
	إذا (handle->level &&
			(handle->مسیر[handle->level - 1].entries >=
			 handle->مسیر[handle->level - 1].max_entries)) {

#اگر_تعریف_شده DEBUG
		چاپ_کن("parent level %d full; splitting it too\n",
							handle->level - 1);
#پایان_اگر
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_به_بالا, &extent);
		إذا (retval)
			اذهب انجامشده;

		retval = extent_گره_تقسیم(handle, expand_allowed);
		إذا (retval)
			اذهب انجامشده;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_ارتفاع, orig_lblk);
		إذا (retval)
			اذهب انجامشده;
	}

	/* At this point, parent should have room for this split */
	مسیر = handle->مسیر + handle->level;
	إذا (!مسیر->curr)
		أرجع EXT2_ET_NO_CURRENT_گره;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, مسیر) : 0;

	/* extent header of the current node we'll split */
	eh = (بنية ext3_extent_سرآیند *)مسیر->buf;

	/* splitting root level means moving them all out */
	إذا (handle->level == 0) {
		new_ریشه = 1;
		tocopy = ext2fs_le16_مقصد_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    حجم(بنية extent_مسیر),
					    &newpath);
		إذا (retval)
			اذهب انجامشده;
	} وإلا {
		إذا (no_balance)
			tocopy = 1;
		وإلا
			tocopy = ext2fs_le16_مقصد_cpu(eh->eh_entries) / 2;
	}

#اگر_تعریف_شده DEBUG
	چاپ_کن("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_مقصد_cpu(eh->eh_entries),
				handle->level);
#پایان_اگر

	إذا (!tocopy && !no_balance) {
#اگر_تعریف_شده DEBUG
		چاپ_کن("Nothing to copy to new block!\n");
#پایان_اگر
		retval = EXT2_ET_CANT_تقسیم_EXTENT;
		اذهب انجامشده;
	}

	/* first we need a new block, or can do nothing. */
	بلوک_buf = حافظه_بده(handle->fs->blocksize);
	إذا (!بلوک_buf) {
		retval = ENOMEM;
		اذهب انجامشده;
	}

	إذا (!goal_blk)
		goal_blk = ext2fs_پیدا_کن_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, بلوک_buf,
				    &new_گره_pblk);
	إذا (retval)
		اذهب انجامشده;

#اگر_تعریف_شده DEBUG
	چاپ_کن("will copy to new node at block %lu\n",
	       (بیعلامت دراز) new_گره_pblk);
#پایان_اگر

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (بنية ext3_extent_سرآیند *) بلوک_buf;
	کپی_حافظه(neweh, eh, حجم(بنية ext3_extent_سرآیند));
	neweh->eh_entries = ext2fs_cpu_مقصد_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_مقصد_le16((handle->fs->blocksize -
			 حجم(بنية ext3_extent_سرآیند)) /
				حجم(بنية ext3_extent));

	/* then the entries for the new block... */
	کپی_حافظه(EXT_اولی_شاخص(neweh),
		EXT_اولی_شاخص(eh) +
			(ext2fs_le16_مقصد_cpu(eh->eh_entries) - tocopy),
		حجم(بنية ext3_extent_idx) * tocopy);

	new_گره_شروع = ext2fs_le32_مقصد_cpu(EXT_اولی_شاخص(neweh)->ei_بلوک);

	/* then update the checksum */
	retval = ext2fs_extent_بلوک_csum_مجموعه(handle->fs, handle->ino, neweh);
	إذا (retval)
		اذهب انجامشده;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_گره_pblk, 1,
					بلوک_buf);

	إذا (retval)
		اذهب انجامشده;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	إذا (handle->level == 0) {
		کپی_حافظه(newpath, مسیر,
		       حجم(بنية extent_مسیر) * handle->max_paths);
		handle->مسیر = newpath;
		newpath = مسیر;
		مسیر = handle->مسیر;
		مسیر->entries = 1;
		مسیر->چپ = مسیر->max_entries - 1;
		handle->max_عمق++;
		handle->max_paths++;
		eh->eh_عمق = ext2fs_cpu_مقصد_le16(handle->max_عمق);
	} وإلا {
		مسیر->entries -= tocopy;
		مسیر->چپ -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_مقصد_le16(مسیر->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_مسیر(handle);
	إذا (retval)
		اذهب انجامشده;

	/* now go up and insert/replace index for new node we created */
	إذا (new_ریشه) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_اولی_SIB, &extent);
		إذا (retval)
			اذهب انجامشده;

		extent.e_lblk = new_گره_شروع;
		extent.e_pblk = new_گره_pblk;
		extent.e_len = handle->مسیر[0].انتها_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب انجامشده;
	} وإلا {
		__u32 new_گره_درازی;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_به_بالا, &extent);
		/* will insert after this one; it's length is shorter now */
		new_گره_درازی = new_گره_شروع - extent.e_lblk;
		extent.e_len -= new_گره_درازی;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب انجامشده;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_گره_شروع;
		extent.e_pblk = new_گره_pblk;
		extent.e_len = new_گره_درازی;
		retval = ext2fs_extent_درج(handle, EXT2_EXTENT_درج_AFTER, &extent);
		إذا (retval)
			اذهب انجامشده;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_ارتفاع, orig_lblk);
	إذا (retval)
		اذهب انجامشده;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_جمع_کن_بلوکها(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	إذا (retval)
		اذهب انجامشده;

انجامشده:
	إذا (newpath)
		ext2fs_free_mem(&newpath);
	حرر(بلوک_buf);

	أرجع retval;
}

errcode_t ext2fs_extent_گره_تقسیم(ext2_extent_handle_t handle)
{
	أرجع extent_گره_تقسیم(handle, 0);
}

errcode_t ext2fs_extent_درج(ext2_extent_handle_t handle, صحيح flags,
				      بنية ext2fs_extent *extent)
{
	بنية extent_مسیر		*مسیر;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent_سرآیند	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->مسیر)
		أرجع EXT2_ET_NO_CURRENT_گره;

#اگر_تعریف_شده DEBUG
	چاپ_کن("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#پایان_اگر

	مسیر = handle->مسیر + handle->level;

	إذا (مسیر->entries >= مسیر->max_entries) {
		إذا (flags & EXT2_EXTENT_درج_NOSPLIT) {
			أرجع EXT2_ET_CANT_درج_EXTENT;
		} وإلا {
#اگر_تعریف_شده DEBUG
			چاپ_کن("node full (level %d) - splitting\n",
				   handle->level);
#پایان_اگر
			retval = extent_گره_تقسیم(handle, 1);
			إذا (retval)
				أرجع retval;
			مسیر = handle->مسیر + handle->level;
		}
	}

	eh = (بنية ext3_extent_سرآیند *) مسیر->buf;
	إذا (مسیر->curr) {
		ix = مسیر->curr;
		إذا (flags & EXT2_EXTENT_درج_AFTER) {
			ix++;
			مسیر->چپ--;
		}
	} وإلا {
		ix = EXT_اولی_شاخص(eh);
		مسیر->چپ = -1;
	}

	مسیر->curr = ix;

	إذا (مسیر->چپ >= 0)
		memmove(ix + 1, ix,
			(مسیر->چپ+1) * حجم(بنية ext3_extent_idx));
	مسیر->چپ++;
	مسیر->entries++;

	eh = (بنية ext3_extent_سرآیند *) مسیر->buf;
	eh->eh_entries = ext2fs_cpu_مقصد_le16(مسیر->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	إذا (retval)
		اذهب errout;

	retval = update_مسیر(handle);
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

errcode_t ext2fs_extent_مجموعه_bmap(ext2_extent_handle_t handle,
				 blk64_t logical, blk64_t physical, صحيح flags)
{
	errcode_t		ec, retval = 0;
	صحيح			mapped = 1; /* logical is mapped? */
	صحيح			orig_ارتفاع;
	صحيح			extent_uninit = 0;
	صحيح			prev_uninit = 0;
	صحيح			بعدی_uninit = 0;
	صحيح			new_uninit = 0;
	صحيح			max_len = EXT_INIT_MAX_LEN;
	صحيح			has_prev, has_بعدی;
	blk64_t			orig_lblk;
	بنية extent_مسیر	*مسیر;
	بنية ext2fs_extent	extent, بعدی_extent, prev_extent;
	بنية ext2fs_extent	newextent;
	بنية ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#اگر_تعریف_شده DEBUG
	چاپ_کن("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#پایان_اگر

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->مسیر)
		أرجع EXT2_ET_NO_CURRENT_گره;

	مسیر = handle->مسیر + handle->level;

	إذا (flags & EXT2_EXTENT_مجموعه_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	إذا (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_برگ;
		إذا (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	إذا ((handle->max_عمق == 0) && (مسیر->entries == 0)) {
		retval = ext2fs_extent_درج(handle, 0, &newextent);
		أرجع retval;
	}

	/* save our original location in the extent tree */
	إذا ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		إذا (retval != EXT2_ET_NO_CURRENT_گره)
			أرجع retval;
		memset(&extent, 0, حجم(extent));
	}
	إذا ((retval = ext2fs_extent_get_info(handle, &info)))
		أرجع retval;
	orig_ارتفاع = info.max_عمق - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval) {
		إذا (retval == EXT2_ET_EXTENT_NOT_یافت) {
			retval = 0;
			mapped = 0;
			إذا (!physical) {
#اگر_تعریف_شده DEBUG
				چاپ_کن("block %llu already unmapped\n",
					logical);
#پایان_اگر
				اذهب انجامشده;
			}
		} وإلا
			اذهب انجامشده;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب انجامشده;
	إذا (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_بعدی_برگ, &بعدی_extent);
	إذا (retval) {
		has_بعدی = 0;
		إذا (retval != EXT2_ET_EXTENT_NO_بعدی)
			اذهب انجامشده;
	} وإلا {
		dbg_print_extent("set_bmap: next_extent",
				 &بعدی_extent);
		has_بعدی = 1;
		إذا (بعدی_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			بعدی_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval && retval != EXT2_ET_EXTENT_NOT_یافت)
		اذهب انجامشده;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_برگ, &prev_extent);
	إذا (retval) {
		has_prev = 0;
		إذا (retval != EXT2_ET_EXTENT_NO_PREV)
			اذهب انجامشده;
	} وإلا {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		إذا (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval && retval != EXT2_ET_EXTENT_NOT_یافت)
		اذهب انجامشده;

	/* check if already pointing to the requested physical */
	إذا (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#اگر_تعریف_شده DEBUG
		چاپ_کن("physical block (at %llu) unchanged\n", logical);
#پایان_اگر
		اذهب انجامشده;
	}

	إذا (!mapped) {
#اگر_تعریف_شده DEBUG
		چاپ_کن("mapping unmapped logical block %llu\n", logical);
#پایان_اگر
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
		} وإلا إذا (has_بعدی &&
			   (logical == بعدی_extent.e_lblk - 1) &&
			   (physical == بعدی_extent.e_pblk - 1) &&
			   (new_uninit == بعدی_uninit) &&
			   ((صحيح) بعدی_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_بعدی_برگ,
						   &بعدی_extent);
			إذا (retval)
				اذهب انجامشده;
			بعدی_extent.e_len++;
			بعدی_extent.e_lblk--;
			بعدی_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &بعدی_extent);
		} وإلا إذا (logical < extent.e_lblk)
			retval = ext2fs_extent_درج(handle, 0, &newextent);
		وإلا
			retval = ext2fs_extent_درج(handle,
				      EXT2_EXTENT_درج_AFTER, &newextent);
		إذا (retval)
			اذهب انجامشده;
		retval = ext2fs_extent_fix_parents(handle);
		إذا (retval)
			اذهب انجامشده;
	} وإلا إذا ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#اگر_تعریف_شده DEBUG
		چاپ_کن("(re/un)mapping only block in extent\n");
#پایان_اگر
		إذا (physical) {
			إذا (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحيح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_برگ, &prev_extent);
				إذا (retval)
					اذهب انجامشده;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_بعدی_برگ,
							   &extent);
				إذا (retval)
					اذهب انجامشده;
				اذهب delete_گره;

			} وإلا
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} وإلا {
		delete_گره:
			retval = ext2fs_extent_delete(handle, 0);
			إذا (retval)
				اذهب انجامشده;
			ec = ext2fs_extent_fix_parents(handle);
			إذا (ec != EXT2_ET_NO_CURRENT_گره)
				retval = ec;
		}

		إذا (retval)
			اذهب انجامشده;
	} وإلا إذا (logical == extent.e_lblk + extent.e_len - 1)  {
#اگر_تعریف_شده DEBUG
		چاپ_کن("(re/un)mapping last block in extent\n");
#پایان_اگر
		إذا (physical) {
			إذا (has_بعدی &&
			    (logical == (بعدی_extent.e_lblk - 1)) &&
			    (physical == (بعدی_extent.e_pblk - 1)) &&
			    (new_uninit == بعدی_uninit) &&
			    ((صحيح) بعدی_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_بعدی_برگ, &بعدی_extent);
				إذا (retval)
					اذهب انجامشده;
				بعدی_extent.e_len++;
				بعدی_extent.e_lblk--;
				بعدی_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &بعدی_extent);
				إذا (retval)
					اذهب انجامشده;
			} وإلا
				retval = ext2fs_extent_درج(handle,
				      EXT2_EXTENT_درج_AFTER, &newextent);
			إذا (retval)
				اذهب انجامشده;
			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval)
				اذهب انجامشده;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			إذا (retval)
				اذهب انجامشده;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			إذا (retval)
				اذهب انجامشده;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب انجامشده;
	} وإلا إذا (logical == extent.e_lblk) {
#اگر_تعریف_شده DEBUG
		چاپ_کن("(re/un)mapping first block in extent\n");
#پایان_اگر
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب انجامشده;
		retval = ext2fs_extent_fix_parents(handle);
		إذا (retval)
			اذهب انجامشده;
		إذا (physical) {
			إذا (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحيح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_برگ, &prev_extent);
				إذا (retval)
					اذهب انجامشده;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} وإلا
				retval = ext2fs_extent_درج(handle,
							      0, &newextent);
			إذا (retval)
				اذهب انجامشده;
			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval)
				اذهب انجامشده;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_بعدی_برگ,
						   &extent);
			إذا (retval)
				اذهب انجامشده;
		}
	} وإلا {
		__u32	save_درازی;
		blk64_t	save_lblk;
		بنية ext2fs_extent save_extent;
		errcode_t r2;

#اگر_تعریف_شده DEBUG
		چاپ_کن("(re/un)mapping in middle of extent\n");
#پایان_اگر
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_درازی = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب انجامشده;
		/* insert our new extent, if any */
		إذا (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_درج(handle,
					EXT2_EXTENT_درج_AFTER, &newextent);
			إذا (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				إذا (r2 == 0)
					(فراغ)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				اذهب انجامشده;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_درازی - extent.e_len - 1;
		retval = ext2fs_extent_درج(handle,
				EXT2_EXTENT_درج_AFTER, &extent);
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
			اذهب انجامشده;
		}
	}

انجامشده:
	/* get handle back to its position */
	إذا (orig_ارتفاع > handle->max_عمق)
		orig_ارتفاع = handle->max_عمق; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_ارتفاع, orig_lblk);
	أرجع retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحيح flags)
{
	بنية extent_مسیر		*مسیر;
	حرف 				*cp;
	بنية ext3_extent_سرآیند	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->مسیر)
		أرجع EXT2_ET_NO_CURRENT_گره;

#اگر_تعریف_شده DEBUG
	{
		بنية ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		إذا (retval == 0) {
			چاپ_کن("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#پایان_اگر

	مسیر = handle->مسیر + handle->level;
	إذا (!مسیر->curr)
		أرجع EXT2_ET_NO_CURRENT_گره;

	cp = مسیر->curr;

	/* Sanity check before memmove() */
	إذا (مسیر->چپ < 0)
		أرجع EXT2_ET_EXTENT_برگ_BAD;

	إذا (مسیر->چپ) {
		memmove(cp, cp + حجم(بنية ext3_extent_idx),
			مسیر->چپ * حجم(بنية ext3_extent_idx));
		مسیر->چپ--;
	} وإلا {
		بنية ext3_extent_idx	*ix = مسیر->curr;
		ix--;
		مسیر->curr = ix;
	}
	إذا (--مسیر->entries == 0)
		مسیر->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	إذا (مسیر->entries == 0 && handle->level) {
		إذا (!(flags & EXT2_EXTENT_DELETE_KEEP_خالی)) {
			بنية ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_به_بالا,
								&extent);
			إذا (retval)
				أرجع retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_بلوکها -=
				(handle->fs->blocksize *
				 EXT2_FS_CLUSTER_نسبت(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_بلوک_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} وإلا {
		eh = (بنية ext3_extent_سرآیند *) مسیر->buf;
		eh->eh_entries = ext2fs_cpu_مقصد_le16(مسیر->entries);
		إذا ((مسیر->entries == 0) && (handle->level == 0)) {
			eh->eh_عمق = 0;
			handle->max_عمق = 0;
		}
		retval = update_مسیر(handle);
	}
	أرجع retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 بنية ext2_extent_info *info)
{
	بنية extent_مسیر		*مسیر;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, حجم(بنية ext2_extent_info));

	مسیر = handle->مسیر + handle->level;
	إذا (مسیر) {
		إذا (مسیر->curr)
			info->curr_مدخل = ((حرف *) مسیر->curr - مسیر->buf) /
				حجم(بنية ext3_extent_idx);
		وإلا
			info->curr_مدخل = 0;
		info->رقم_entries = مسیر->entries;
		info->max_entries = مسیر->max_entries;
		info->بایتها_avail = (مسیر->max_entries - مسیر->entries) *
			حجم(بنية ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_عمق = handle->max_عمق;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	أرجع 0;
}

size_t ext2fs_max_extent_عمق(ext2_extent_handle_t handle)
{
	size_t iblock_sz = حجم(((بنية ext2_inode *)عدم)->i_بلوک);
	size_t iblock_extents = (iblock_sz - حجم(بنية ext3_extent_سرآیند)) /
				حجم(بنية ext3_extent);
	size_t extents_در_هر_بلوک = (handle->fs->blocksize -
				    حجم(بنية ext3_extent_سرآیند)) /
				   حجم(بنية ext3_extent);
	ساكن بیعلامت صحيح آخرین_blocksize = 0;
	ساكن size_t آخرین_نتیجه = 0;

	إذا (آخرین_blocksize && آخرین_blocksize == handle->fs->blocksize)
		أرجع آخرین_نتیجه;

	آخرین_نتیجه = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_در_هر_بلوک));
	آخرین_blocksize = handle->fs->blocksize;
	أرجع آخرین_نتیجه;
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
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ریشه, &extent);
	إذا (errcode)
		اذهب بیرون;

	افعل {
		/* Skip to the end of a block of leaf nodes */
		إذا (extent.e_flags & EXT2_EXTENT_FLAGS_برگ) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_آخرین_SIB,
						    &extent);
			إذا (errcode)
				اكسر;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_بعدی, &extent);
		إذا (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_مسیر(handle);
	} طالما (errcode == 0);

بیرون:
	/* Ok if we run off the end */
	إذا (errcode == EXT2_ET_EXTENT_NO_بعدی)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	أرجع errcode;
}

errcode_t ext2fs_decode_extent(بنية ext2fs_extent *مقصد, فراغ *addr, صحيح len)
{
	بنية ext3_extent *from = (بنية ext3_extent *)addr;

	إذا (len != حجم(بنية ext3_extent))
		أرجع EXT2_ET_INVALID_آرگومان;

	مقصد->e_pblk = ext2fs_le32_مقصد_cpu(from->ee_شروع) +
		((__u64) ext2fs_le16_مقصد_cpu(from->ee_شروع_hi)
			<< 32);
	مقصد->e_lblk = ext2fs_le32_مقصد_cpu(from->ee_بلوک);
	مقصد->e_len = ext2fs_le16_مقصد_cpu(from->ee_len);
	مقصد->e_flags = EXT2_EXTENT_FLAGS_برگ;
	إذا (مقصد->e_len > EXT_INIT_MAX_LEN) {
		مقصد->e_len -= EXT_INIT_MAX_LEN;
		مقصد->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	أرجع 0;
}

errcode_t ext2fs_شمار_بلوکها(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode, blk64_t *ret_شمار)
{
	ext2_extent_handle_t	handle = عدم;
	بنية ext2fs_extent	extent;
	errcode_t		errcode;
	صحيح			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	إذا (errcode)
		اذهب بیرون;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ریشه, &extent);
	إذا (errcode)
		اذهب بیرون;

	errcode = ext2fs_get_array(handle->max_عمق, حجم(blk64_t),
				   &intermediate_nodes);
	إذا (errcode)
		اذهب بیرون;

	blkcount = handle->level;
	طالما (!errcode) {
		إذا (extent.e_flags & EXT2_EXTENT_FLAGS_برگ) {
			blkcount += extent.e_len;
			لكل (i = 0; i < handle->level; i++) {
				إذا (intermediate_nodes[i] !=
					handle->مسیر[i].انتها_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->مسیر[i].انتها_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_بعدی, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
بیرون:
	*ret_شمار = blkcount;
	ext2fs_extent_free(handle);

	أرجع 0;
}

#اگر_تعریف_شده DEBUG
/*
 * Override debugfs's prompt
 */
ثابت حرف *debug_prog_name = "tst_extents";

#پایان_اگر

