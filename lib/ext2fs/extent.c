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
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "e2image.h"

#تعریف_رد DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

بنية extent_رستہ {
	حرف		*buf;
	صحيح		entries;
	صحيح		max_entries;
	صحيح		بایاں;
	صحيح		ویکھو_عدد;
	صحيح		flags;
	blk64_t		سرا_blk;
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
	صحيح			max_ڈونگھائی;
	صحيح			max_paths;
	بنية extent_رستہ	*رستہ;
};

بنية ext2_extent_رستہ {
	errcode_t		magic;
	صحيح			پتی_اچائی;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#جے_تعریف_شدہ DEBUG
ساكن فراغ dbg_show_سرخی(بنية ext3_extent_سرخی *eh)
{
	لکھو("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_منزل_cpu(eh->eh_magic),
			ext2fs_le16_منزل_cpu(eh->eh_entries),
			ext2fs_le16_منزل_cpu(eh->eh_max),
			ext2fs_le16_منزل_cpu(eh->eh_ڈونگھائی),
			ext2fs_le32_منزل_cpu(eh->eh_generation));
}

ساكن فراغ dbg_show_اشاریہ(بنية ext3_extent_idx *ix)
{
	لکھو("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_منزل_cpu(ix->ei_بلاک),
			ext2fs_le32_منزل_cpu(ix->ei_پتی),
			ext2fs_le16_منزل_cpu(ix->ei_پتی_hi),
			ext2fs_le16_منزل_cpu(ix->ei_بن_ورتیا));
}

ساكن فراغ dbg_show_extent(بنية ext3_extent *ex)
{
	لکھو("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_منزل_cpu(ex->ee_بلاک),
			ext2fs_le32_منزل_cpu(ex->ee_بلاک) +
			ext2fs_le16_منزل_cpu(ex->ee_len) - 1,
			ext2fs_le16_منزل_cpu(ex->ee_len),
			ext2fs_le32_منزل_cpu(ex->ee_شروع),
			ext2fs_le16_منزل_cpu(ex->ee_شروع_hi));
}

ساكن فراغ dbg_print_extent(حرف *desc, بنية ext2fs_extent *extent)
{
	إذا (desc)
		لکھو("%s: ", desc);
	لکھو("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_پتی)
		fputs("LEAF ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_دوجا_ویکھو)
		fputs("2ND_VISIT ", stdout);
	إذا (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

ساكن فراغ dump_رستہ(ثابت حرف *ٹیگ, بنية ext2_extent_handle *handle,
		      بنية extent_رستہ *رستہ)
{
	بنية extent_رستہ *ppp = رستہ;
	لکھو("%s: level=%d\n", ٹیگ, handle->level);

	افعل {
		لکھو("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       ٹیگ, (ppp - handle->رستہ), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->بایاں, ppp->ویکھو_عدد, ppp->flags,
		       ppp->سرا_blk, ppp->curr, ppp->curr - (فراغ *)ppp->buf);
		لکھو("  ");
		dbg_show_سرخی((بنية ext3_extent_سرخی *)ppp->buf);
		إذا (ppp->curr) {
			لکھو("  ");
			dbg_show_اشاریہ(ppp->curr);
			لکھو("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} طالما (ppp >= handle->رستہ);
	fflush(stdout);

	أرجع;
}

#وإلا
#تعریف_کرو dbg_show_سرخی(eh) افعل { } طالما (0)
#تعریف_کرو dbg_show_اشاریہ(ix) افعل { } طالما (0)
#تعریف_کرو dbg_show_extent(ex) افعل { } طالما (0)
#تعریف_کرو dbg_print_extent(desc, ex) افعل { } طالما (0)
#تعریف_کرو dump_رستہ(ٹیگ, handle, رستہ) افعل { } طالما (0)
#جے_انت

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_سرخی_verify(فراغ *ptr, صحيح ناپ)
{
	صحيح eh_max, اندراج_ناپ;
	بنية ext3_extent_سرخی *eh = ptr;

	dbg_show_سرخی(eh);
	إذا (ext2fs_le16_منزل_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		أرجع EXT2_ET_EXTENT_سرخی_BAD;
	إذا (ext2fs_le16_منزل_cpu(eh->eh_entries) > ext2fs_le16_منزل_cpu(eh->eh_max))
		أرجع EXT2_ET_EXTENT_سرخی_BAD;
	إذا (eh->eh_ڈونگھائی == 0)
		اندراج_ناپ = حجم(بنية ext3_extent);
	وإلا
		اندراج_ناپ = حجم(بنية ext3_extent_idx);

	eh_max = (ناپ - حجم(*eh)) / اندراج_ناپ;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	إذا ((ext2fs_le16_منزل_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_منزل_cpu(eh->eh_max) < (eh_max - 2)))
		أرجع EXT2_ET_EXTENT_سرخی_BAD;

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

	إذا (handle->رستہ) {
		لكل (i = 1; i < handle->max_paths; i++) {
			إذا (handle->رستہ[i].buf)
				ext2fs_free_mem(&handle->رستہ[i].buf);
		}
		ext2fs_free_mem(&handle->رستہ);
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
	بنية ext3_extent_سرخی	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!inode)
		إذا ((ino == 0) || (ino > fs->super->s_inodes_گنتی))
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

	eh = (بنية ext3_extent_سرخی *) &handle->inode->i_بلاک[0];

	لكل (i=0; i < EXT2_N_بلاکاں; i++)
		إذا (handle->inode->i_بلاک[i])
			اكسر;
	إذا (i >= EXT2_N_بلاکاں) {
		eh->eh_magic = ext2fs_cpu_منزل_le16(EXT3_EXT_MAGIC);
		eh->eh_ڈونگھائی = 0;
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

	retval = ext2fs_extent_سرخی_verify(eh, حجم(handle->inode->i_بلاک));
	إذا (retval)
		اذهب errout;

	handle->max_ڈونگھائی = ext2fs_le16_منزل_cpu(eh->eh_ڈونگھائی);
	handle->type = ext2fs_le16_منزل_cpu(eh->eh_magic);

	handle->max_paths = handle->max_ڈونگھائی + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    حجم(بنية extent_رستہ),
				    &handle->رستہ);
	handle->رستہ[0].buf = (حرف *) handle->inode->i_بلاک;

	handle->رستہ[0].بایاں = handle->رستہ[0].entries =
		ext2fs_le16_منزل_cpu(eh->eh_entries);
	handle->رستہ[0].max_entries = ext2fs_le16_منزل_cpu(eh->eh_max);
	handle->رستہ[0].curr = 0;
	handle->رستہ[0].سرا_blk =
		(EXT2_I_ناپ(handle->inode) + fs->blocksize - 1) >>
		 EXT2_بلاک_ناپ_بٹ(fs->super);
	handle->رستہ[0].blk = 0;
	handle->رستہ[0].ویکھو_عدد = 1;
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
	بنية extent_رستہ	*رستہ, *newpath, *tp;
	بنية ext3_extent_سرخی	*eh;
	بنية ext3_extent_idx		*ix = 0;
	بنية ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				سرا_blk;
	صحيح				orig_op, op, l;
	صحيح				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!handle->رستہ)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

	orig_op = op = flags & EXT2_EXTENT_MOVE_نقاب;

retry:
	رستہ = handle->رستہ + handle->level;
	إذا ((orig_op == EXT2_EXTENT_اگلا) ||
	    (orig_op == EXT2_EXTENT_اگلا_پتی)) {
		إذا (handle->level < handle->max_ڈونگھائی) {
			/* interior node */
			إذا (رستہ->ویکھو_عدد == 0) {
				رستہ->ویکھو_عدد++;
				op = EXT2_EXTENT_تھلے;
			} وإلا إذا (رستہ->بایاں > 0)
				op = EXT2_EXTENT_اگلا_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_اتے;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_اگلا;
		} وإلا {
			/* leaf node */
			إذا (رستہ->بایاں > 0)
				op = EXT2_EXTENT_اگلا_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_اتے;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_اگلا;
		}
		إذا (op != EXT2_EXTENT_اگلا_SIB) {
#جے_تعریف_شدہ DEBUG_GET_EXTENT
			لکھو("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_تھلے) ? "down" :
			       ((op == EXT2_EXTENT_اتے) ? "up" : "unknown"));
#جے_انت
		}
	}

	إذا ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_پتی)) {
		إذا (handle->level < handle->max_ڈونگھائی) {
			/* interior node */
			إذا (رستہ->ویکھو_عدد > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_تھلے_AND_آخری;
			} وإلا إذا (رستہ->بایاں < رستہ->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_اتے;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_PREV;
		} وإلا {
			/* leaf node */
			إذا (رستہ->بایاں < رستہ->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_اتے;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_PREV;
		}
		إذا (op != EXT2_EXTENT_PREV_SIB) {
#جے_تعریف_شدہ DEBUG_GET_EXTENT
			لکھو("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_تھلے_AND_آخری) ? "down/last" :
			       ((op == EXT2_EXTENT_اتے) ? "up" : "unknown"));
#جے_انت
		}
	}

	إذا (orig_op == EXT2_EXTENT_آخری_پتی) {
		إذا ((handle->level < handle->max_ڈونگھائی) &&
		    (رستہ->بایاں == 0))
			op = EXT2_EXTENT_تھلے;
		وإلا
			op = EXT2_EXTENT_آخری_SIB;
#جے_تعریف_شدہ DEBUG_GET_EXTENT
		لکھو("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_تھلے) ? "down" : "last_sib");
#جے_انت
	}

	بدل (op) {
	حالة EXT2_EXTENT_CURRENT:
		ix = رستہ->curr;
		اكسر;
	حالة EXT2_EXTENT_جڑ:
		handle->level = 0;
		رستہ = handle->رستہ + handle->level;
		/* fallthrough */
	حالة EXT2_EXTENT_پہلا_SIB:
		رستہ->بایاں = رستہ->entries;
		رستہ->curr = 0;
		/* fallthrough */
	حالة EXT2_EXTENT_اگلا_SIB:
		إذا (رستہ->بایاں <= 0)
			أرجع EXT2_ET_EXTENT_NO_اگلا;
		إذا (رستہ->curr) {
			ix = رستہ->curr;
			ix++;
		} وإلا {
			eh = (بنية ext3_extent_سرخی *) رستہ->buf;
			ix = EXT_پہلا_اشاریہ(eh);
		}
		رستہ->بایاں--;
		رستہ->curr = ix;
		رستہ->ویکھو_عدد = 0;
		اكسر;
	حالة EXT2_EXTENT_PREV_SIB:
		إذا (!رستہ->curr ||
		    رستہ->بایاں+1 >= رستہ->entries)
			أرجع EXT2_ET_EXTENT_NO_PREV;
		ix = رستہ->curr;
		ix--;
		رستہ->curr = ix;
		رستہ->بایاں++;
		إذا (handle->level < handle->max_ڈونگھائی)
			رستہ->ویکھو_عدد = 1;
		اكسر;
	حالة EXT2_EXTENT_آخری_SIB:
		eh = (بنية ext3_extent_سرخی *) رستہ->buf;
		رستہ->curr = EXT_آخری_EXTENT(eh);
		ix = رستہ->curr;
		رستہ->بایاں = 0;
		رستہ->ویکھو_عدد = 0;
		اكسر;
	حالة EXT2_EXTENT_اتے:
		إذا (handle->level <= 0)
			أرجع EXT2_ET_EXTENT_NO_اتے;
		handle->level--;
		رستہ--;
		ix = رستہ->curr;
		إذا ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_پتی))
			رستہ->ویکھو_عدد = 0;
		اكسر;
	حالة EXT2_EXTENT_تھلے:
	حالة EXT2_EXTENT_تھلے_AND_آخری:
		إذا (!رستہ->curr ||(handle->level >= handle->max_ڈونگھائی))
			أرجع EXT2_ET_EXTENT_NO_تھلے;

		ix = رستہ->curr;
		newpath = رستہ + 1;
		إذا (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			إذا (retval)
				أرجع retval;
		}
		blk = ext2fs_le32_منزل_cpu(ix->ei_پتی) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_پتی_hi) << 32);
		لكل (l = handle->level, tp = رستہ; l > 0; l--, tp--) {
			إذا (blk == tp->blk)
				أرجع EXT2_ET_EXTENT_چکر;
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

		eh = (بنية ext3_extent_سرخی *) newpath->buf;

		retval = ext2fs_extent_سرخی_verify(eh, handle->fs->blocksize);
		إذا (retval) {
			handle->level--;
			أرجع retval;
		}

		إذا (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_بلاک_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->بایاں = newpath->entries =
			ext2fs_le16_منزل_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_منزل_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		إذا (newpath->بایاں <= 0)
			أرجع EXT2_ET_EXTENT_NO_تھلے;

		إذا (رستہ->بایاں > 0) {
			ix++;
			newpath->سرا_blk = ext2fs_le32_منزل_cpu(ix->ei_بلاک);
		} وإلا
			newpath->سرا_blk = رستہ->سرا_blk;

		رستہ = newpath;
		إذا (op == EXT2_EXTENT_تھلے) {
			ix = EXT_پہلا_اشاریہ((بنية ext3_extent_سرخی *) eh);
			رستہ->curr = ix;
			رستہ->بایاں = رستہ->entries - 1;
			رستہ->ویکھو_عدد = 0;
		} وإلا {
			ix = EXT_آخری_اشاریہ((بنية ext3_extent_سرخی *) eh);
			رستہ->curr = ix;
			رستہ->بایاں = 0;
			إذا (handle->level < handle->max_ڈونگھائی)
				رستہ->ویکھو_عدد = 1;
		}
#جے_تعریف_شدہ DEBUG_GET_EXTENT
		لکھو("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_ڈونگھائی,
			   رستہ->سرا_blk);
#جے_انت
		اكسر;
	افتراضي:
		أرجع EXT2_ET_OP_NOT_SUPPORTED;
	}

	إذا (!ix)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

	extent->e_flags = 0;
#جے_تعریف_شدہ DEBUG_GET_EXTENT
	لکھو("(Left %d)\n", رستہ->بایاں);
#جے_انت

	إذا (handle->level == handle->max_ڈونگھائی) {
		ex = (بنية ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_منزل_cpu(ex->ee_شروع) +
			((__u64) ext2fs_le16_منزل_cpu(ex->ee_شروع_hi) << 32);
		extent->e_lblk = ext2fs_le32_منزل_cpu(ex->ee_بلاک);
		extent->e_len = ext2fs_le16_منزل_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_پتی;
		إذا (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} وإلا {
		extent->e_pblk = ext2fs_le32_منزل_cpu(ix->ei_پتی) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_پتی_hi) << 32);
		extent->e_lblk = ext2fs_le32_منزل_cpu(ix->ei_بلاک);
		إذا (رستہ->بایاں > 0) {
			ix++;
			سرا_blk = ext2fs_le32_منزل_cpu(ix->ei_بلاک);
		} وإلا
			سرا_blk = رستہ->سرا_blk;

		extent->e_len = سرا_blk - extent->e_lblk;
	}
	إذا (رستہ->ویکھو_عدد)
		extent->e_flags |= EXT2_EXTENT_FLAGS_دوجا_ویکھو;

	إذا (((orig_op == EXT2_EXTENT_اگلا_پتی) ||
	     (orig_op == EXT2_EXTENT_PREV_پتی)) &&
	    (handle->level != handle->max_ڈونگھائی))
		اذهب retry;

	إذا ((orig_op == EXT2_EXTENT_آخری_پتی) &&
	    ((handle->level != handle->max_ڈونگھائی) ||
	     (رستہ->بایاں != 0)))
		اذهب retry;

	إذا (failed_csum)
		أرجع EXT2_ET_EXTENT_CSUM_INVALID;

	أرجع 0;
}

ساكن errcode_t update_رستہ(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent_سرخی	*eh;

	إذا (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} وإلا {
		ix = handle->رستہ[handle->level - 1].curr;
		blk = ext2fs_le32_منزل_cpu(ix->ei_پتی) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_پتی_hi) << 32);

		/* then update the checksum */
		eh = (بنية ext3_extent_سرخی *)
				handle->رستہ[handle->level].buf;
		retval = ext2fs_extent_بلاک_csum_سیٹ(handle->fs, handle->ino,
						      eh);
		إذا (retval)
			أرجع retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->رستہ[handle->level].buf);
	}
	أرجع retval;
}

#إذا 0
errcode_t ext2fs_extent_save_رستہ(ext2_extent_handle_t handle,
				  ext2_extent_رستہ_t *ret_رستہ)
{
	ext2_extent_رستہ_t	save_رستہ;
	بنية ext2fs_extent	extent;
	بنية ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_extent_get_info(handle, &info);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(حجم(بنية ext2_extent_رستہ), &save_رستہ);
	إذا (retval)
		أرجع retval;
	memset(save_رستہ, 0, حجم(بنية ext2_extent_رستہ));

	save_رستہ->magic = EXT2_ET_MAGIC_EXTENT_رستہ;
	save_رستہ->پتی_اچائی = info.max_ڈونگھائی - info.curr_level - 1;
	save_رستہ->lblk = extent.e_lblk;

	*ret_رستہ = save_رستہ;
	أرجع 0;
}

errcode_t ext2fs_extent_free_رستہ(ext2_extent_رستہ_t رستہ)
{
	EXT2_CHECK_MAGIC(رستہ, EXT2_ET_MAGIC_EXTENT_رستہ);

	ext2fs_free_mem(&رستہ);
	أرجع 0;
}
#جے_انت

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
			      صحيح پتی_level, blk64_t blk)
{
	بنية ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_جڑ, &extent);
	إذا (retval) {
		إذا (retval == EXT2_ET_EXTENT_NO_اگلا)
			retval = EXT2_ET_EXTENT_NOT_لبھیا;
		أرجع retval;
	}

	إذا (پتی_level > handle->max_ڈونگھائی) {
#جے_تعریف_شدہ DEBUG
		لکھو("leaf level %d greater than tree depth %d\n",
			پتی_level, handle->max_ڈونگھائی);
#جے_انت
		أرجع EXT2_ET_OP_NOT_SUPPORTED;
	}

#جے_تعریف_شدہ DEBUG
	لکھو("goto extent ino %u, level %d, %llu\n", handle->ino,
	       پتی_level, blk);
#جے_انت

#جے_تعریف_شدہ DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#جے_انت
	طالما (1) {
		إذا (handle->max_ڈونگھائی - handle->level == پتی_level) {
			/* block is in this &extent */
			إذا ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				أرجع 0;
			إذا (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				أرجع EXT2_ET_EXTENT_NOT_لبھیا;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_اگلا_SIB,
						   &extent);
			إذا (retval == EXT2_ET_EXTENT_NO_اگلا)
				أرجع EXT2_ET_EXTENT_NOT_لبھیا;
			إذا (retval)
				أرجع retval;
			استمر;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_اگلا_SIB,
					   &extent);
		إذا (retval == EXT2_ET_EXTENT_NO_اگلا)
			اذهب go_تھلے;
		إذا (retval)
			أرجع retval;

#جے_تعریف_شدہ DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#جے_انت
		إذا (blk == extent.e_lblk)
			اذهب go_تھلے;
		إذا (blk > extent.e_lblk)
			استمر;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		إذا (retval)
			أرجع retval;

#جے_تعریف_شدہ DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#جے_انت

	go_تھلے:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_تھلے,
					   &extent);
		إذا (retval)
			أرجع retval;

#جے_تعریف_شدہ DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#جے_انت
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
	صحيح				orig_اچائی;
	blk64_t				شروع;
	بنية extent_رستہ		*رستہ;
	بنية ext2fs_extent		extent;
	بنية ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستہ)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

	رستہ = handle->رستہ + handle->level;
	إذا (!رستہ->curr)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب پورا;

	/* modified node's start block */
	شروع = extent.e_lblk;

	إذا ((retval = ext2fs_extent_get_info(handle, &info)))
		أرجع retval;
	orig_اچائی = info.max_ڈونگھائی - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	طالما (handle->level > 0 &&
	       (رستہ->بایاں == رستہ->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_اتے, &extent);
		إذا (retval)
			اذهب پورا;
		إذا (extent.e_lblk == شروع)
			اكسر;
		رستہ = handle->رستہ + handle->level;
		extent.e_len += (extent.e_lblk - شروع);
		extent.e_lblk = شروع;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورا;
		update_رستہ(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_اچائی, شروع);
پورا:
	أرجع retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				صحيح flags EXT2FS_ATTR((بن_ورتیا)),
				بنية ext2fs_extent *extent)
{
	بنية extent_رستہ		*رستہ;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستہ)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

	رستہ = handle->رستہ + handle->level;
	إذا (!رستہ->curr)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

#جے_تعریف_شدہ DEBUG
	لکھو("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#جے_انت

	إذا (handle->level == handle->max_ڈونگھائی) {
		ex = رستہ->curr;

		ex->ee_بلاک = ext2fs_cpu_منزل_le32(extent->e_lblk);
		ex->ee_شروع = ext2fs_cpu_منزل_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_شروع_hi = ext2fs_cpu_منزل_le16(extent->e_pblk >> 32);
		إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			إذا (extent->e_len > EXT_UNINIT_MAX_LEN)
				أرجع EXT2_ET_EXTENT_INVALID_لمائی;
			ex->ee_len = ext2fs_cpu_منزل_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} وإلا {
			إذا (extent->e_len > EXT_INIT_MAX_LEN)
				أرجع EXT2_ET_EXTENT_INVALID_لمائی;
			ex->ee_len = ext2fs_cpu_منزل_le16(extent->e_len);
		}
	} وإلا {
		ix = رستہ->curr;

		ix->ei_پتی = ext2fs_cpu_منزل_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_پتی_hi = ext2fs_cpu_منزل_le16(extent->e_pblk >> 32);
		ix->ei_بلاک = ext2fs_cpu_منزل_le32(extent->e_lblk);
		ix->ei_بن_ورتیا = 0;
	}
	update_رستہ(handle);
	أرجع 0;
}

ساكن صحيح splitting_at_eof(بنية ext2_extent_handle *handle,
			    بنية extent_رستہ *رستہ)
{
	بنية extent_رستہ *ppp = رستہ;
	dump_رستہ(__func__, handle, رستہ);

	إذا (handle->level == 0)
		أرجع 0;

	افعل {
		إذا (ppp->بایاں)
			أرجع 0;
		ppp--;
	} طالما (ppp >= handle->رستہ);

	أرجع 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
ساكن errcode_t extent_گرہ_ونڈو(ext2_extent_handle_t handle,
				   صحيح expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_گرہ_pblk;
	blk64_t				new_گرہ_شروع;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	صحيح				orig_اچائی;
	حرف				*بلاک_buf = عدم;
	بنية ext2fs_extent		extent;
	بنية extent_رستہ		*رستہ, *newpath = 0;
	بنية ext3_extent_سرخی	*eh, *neweh;
	صحيح				tocopy;
	صحيح				new_جڑ = 0;
	بنية ext2_extent_info		info;
	صحيح				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستہ)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

#جے_تعریف_شدہ DEBUG
	لکھو("splitting node at level %d\n", handle->level);
#جے_انت
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب پورا;

	retval = ext2fs_extent_get_info(handle, &info);
	إذا (retval)
		اذهب پورا;

	/* save the position we were originally splitting... */
	orig_اچائی = info.max_ڈونگھائی - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	رستہ = handle->رستہ + handle->level;
	eh = (بنية ext3_extent_سرخی *) رستہ->buf;
	إذا (handle->level == handle->max_ڈونگھائی) {
		بنية ext3_extent	*ex;

		ex = EXT_پہلا_EXTENT(eh);
		goal_blk = ext2fs_le32_منزل_cpu(ex->ee_شروع) +
			((__u64) ext2fs_le16_منزل_cpu(ex->ee_شروع_hi) << 32);
	} وإلا {
		بنية ext3_extent_idx	*ix;

		ix = EXT_پہلا_اشاریہ(eh);
		goal_blk = ext2fs_le32_منزل_cpu(ix->ei_پتی) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_پتی_hi) << 32);
	}
	goal_blk -= EXT2_FS_CLUSTER_تناسب(handle->fs);
	goal_blk &= ~EXT2_FS_CLUSTER_نقاب(handle->fs);

	/* Is there room in the parent for a new entry? */
	إذا (handle->level &&
			(handle->رستہ[handle->level - 1].entries >=
			 handle->رستہ[handle->level - 1].max_entries)) {

#جے_تعریف_شدہ DEBUG
		لکھو("parent level %d full; splitting it too\n",
							handle->level - 1);
#جے_انت
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_اتے, &extent);
		إذا (retval)
			اذهب پورا;

		retval = extent_گرہ_ونڈو(handle, expand_allowed);
		إذا (retval)
			اذهب پورا;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_اچائی, orig_lblk);
		إذا (retval)
			اذهب پورا;
	}

	/* At this point, parent should have room for this split */
	رستہ = handle->رستہ + handle->level;
	إذا (!رستہ->curr)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, رستہ) : 0;

	/* extent header of the current node we'll split */
	eh = (بنية ext3_extent_سرخی *)رستہ->buf;

	/* splitting root level means moving them all out */
	إذا (handle->level == 0) {
		new_جڑ = 1;
		tocopy = ext2fs_le16_منزل_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    حجم(بنية extent_رستہ),
					    &newpath);
		إذا (retval)
			اذهب پورا;
	} وإلا {
		إذا (no_balance)
			tocopy = 1;
		وإلا
			tocopy = ext2fs_le16_منزل_cpu(eh->eh_entries) / 2;
	}

#جے_تعریف_شدہ DEBUG
	لکھو("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_منزل_cpu(eh->eh_entries),
				handle->level);
#جے_انت

	إذا (!tocopy && !no_balance) {
#جے_تعریف_شدہ DEBUG
		لکھو("Nothing to copy to new block!\n");
#جے_انت
		retval = EXT2_ET_CANT_ونڈو_EXTENT;
		اذهب پورا;
	}

	/* first we need a new block, or can do nothing. */
	بلاک_buf = حافظہ_دیو(handle->fs->blocksize);
	إذا (!بلاک_buf) {
		retval = ENOMEM;
		اذهب پورا;
	}

	إذا (!goal_blk)
		goal_blk = ext2fs_لبھو_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, بلاک_buf,
				    &new_گرہ_pblk);
	إذا (retval)
		اذهب پورا;

#جے_تعریف_شدہ DEBUG
	لکھو("will copy to new node at block %lu\n",
	       (بنانشانی لما) new_گرہ_pblk);
#جے_انت

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (بنية ext3_extent_سرخی *) بلاک_buf;
	حافظہ_نقل(neweh, eh, حجم(بنية ext3_extent_سرخی));
	neweh->eh_entries = ext2fs_cpu_منزل_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_منزل_le16((handle->fs->blocksize -
			 حجم(بنية ext3_extent_سرخی)) /
				حجم(بنية ext3_extent));

	/* then the entries for the new block... */
	حافظہ_نقل(EXT_پہلا_اشاریہ(neweh),
		EXT_پہلا_اشاریہ(eh) +
			(ext2fs_le16_منزل_cpu(eh->eh_entries) - tocopy),
		حجم(بنية ext3_extent_idx) * tocopy);

	new_گرہ_شروع = ext2fs_le32_منزل_cpu(EXT_پہلا_اشاریہ(neweh)->ei_بلاک);

	/* then update the checksum */
	retval = ext2fs_extent_بلاک_csum_سیٹ(handle->fs, handle->ino, neweh);
	إذا (retval)
		اذهب پورا;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_گرہ_pblk, 1,
					بلاک_buf);

	إذا (retval)
		اذهب پورا;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	إذا (handle->level == 0) {
		حافظہ_نقل(newpath, رستہ,
		       حجم(بنية extent_رستہ) * handle->max_paths);
		handle->رستہ = newpath;
		newpath = رستہ;
		رستہ = handle->رستہ;
		رستہ->entries = 1;
		رستہ->بایاں = رستہ->max_entries - 1;
		handle->max_ڈونگھائی++;
		handle->max_paths++;
		eh->eh_ڈونگھائی = ext2fs_cpu_منزل_le16(handle->max_ڈونگھائی);
	} وإلا {
		رستہ->entries -= tocopy;
		رستہ->بایاں -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_منزل_le16(رستہ->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_رستہ(handle);
	إذا (retval)
		اذهب پورا;

	/* now go up and insert/replace index for new node we created */
	إذا (new_جڑ) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_پہلا_SIB, &extent);
		إذا (retval)
			اذهب پورا;

		extent.e_lblk = new_گرہ_شروع;
		extent.e_pblk = new_گرہ_pblk;
		extent.e_len = handle->رستہ[0].سرا_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورا;
	} وإلا {
		__u32 new_گرہ_لمائی;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_اتے, &extent);
		/* will insert after this one; it's length is shorter now */
		new_گرہ_لمائی = new_گرہ_شروع - extent.e_lblk;
		extent.e_len -= new_گرہ_لمائی;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورا;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_گرہ_شروع;
		extent.e_pblk = new_گرہ_pblk;
		extent.e_len = new_گرہ_لمائی;
		retval = ext2fs_extent_داخل_کرو(handle, EXT2_EXTENT_داخل_کرو_AFTER, &extent);
		إذا (retval)
			اذهب پورا;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_اچائی, orig_lblk);
	إذا (retval)
		اذهب پورا;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_جوڑو_بلاکاں(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	إذا (retval)
		اذهب پورا;

پورا:
	إذا (newpath)
		ext2fs_free_mem(&newpath);
	حرر(بلاک_buf);

	أرجع retval;
}

errcode_t ext2fs_extent_گرہ_ونڈو(ext2_extent_handle_t handle)
{
	أرجع extent_گرہ_ونڈو(handle, 0);
}

errcode_t ext2fs_extent_داخل_کرو(ext2_extent_handle_t handle, صحيح flags,
				      بنية ext2fs_extent *extent)
{
	بنية extent_رستہ		*رستہ;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent_سرخی	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستہ)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

#جے_تعریف_شدہ DEBUG
	لکھو("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#جے_انت

	رستہ = handle->رستہ + handle->level;

	إذا (رستہ->entries >= رستہ->max_entries) {
		إذا (flags & EXT2_EXTENT_داخل_کرو_NOSPLIT) {
			أرجع EXT2_ET_CANT_داخل_کرو_EXTENT;
		} وإلا {
#جے_تعریف_شدہ DEBUG
			لکھو("node full (level %d) - splitting\n",
				   handle->level);
#جے_انت
			retval = extent_گرہ_ونڈو(handle, 1);
			إذا (retval)
				أرجع retval;
			رستہ = handle->رستہ + handle->level;
		}
	}

	eh = (بنية ext3_extent_سرخی *) رستہ->buf;
	إذا (رستہ->curr) {
		ix = رستہ->curr;
		إذا (flags & EXT2_EXTENT_داخل_کرو_AFTER) {
			ix++;
			رستہ->بایاں--;
		}
	} وإلا {
		ix = EXT_پہلا_اشاریہ(eh);
		رستہ->بایاں = -1;
	}

	رستہ->curr = ix;

	إذا (رستہ->بایاں >= 0)
		memmove(ix + 1, ix,
			(رستہ->بایاں+1) * حجم(بنية ext3_extent_idx));
	رستہ->بایاں++;
	رستہ->entries++;

	eh = (بنية ext3_extent_سرخی *) رستہ->buf;
	eh->eh_entries = ext2fs_cpu_منزل_le16(رستہ->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	إذا (retval)
		اذهب errout;

	retval = update_رستہ(handle);
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

errcode_t ext2fs_extent_سیٹ_bmap(ext2_extent_handle_t handle,
				 blk64_t logical, blk64_t physical, صحيح flags)
{
	errcode_t		ec, retval = 0;
	صحيح			mapped = 1; /* logical is mapped? */
	صحيح			orig_اچائی;
	صحيح			extent_uninit = 0;
	صحيح			prev_uninit = 0;
	صحيح			اگلا_uninit = 0;
	صحيح			new_uninit = 0;
	صحيح			max_len = EXT_INIT_MAX_LEN;
	صحيح			has_prev, has_اگلا;
	blk64_t			orig_lblk;
	بنية extent_رستہ	*رستہ;
	بنية ext2fs_extent	extent, اگلا_extent, prev_extent;
	بنية ext2fs_extent	newextent;
	بنية ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#جے_تعریف_شدہ DEBUG
	لکھو("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#جے_انت

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستہ)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

	رستہ = handle->رستہ + handle->level;

	إذا (flags & EXT2_EXTENT_سیٹ_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	إذا (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_پتی;
		إذا (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	إذا ((handle->max_ڈونگھائی == 0) && (رستہ->entries == 0)) {
		retval = ext2fs_extent_داخل_کرو(handle, 0, &newextent);
		أرجع retval;
	}

	/* save our original location in the extent tree */
	إذا ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		إذا (retval != EXT2_ET_NO_CURRENT_گرہ)
			أرجع retval;
		memset(&extent, 0, حجم(extent));
	}
	إذا ((retval = ext2fs_extent_get_info(handle, &info)))
		أرجع retval;
	orig_اچائی = info.max_ڈونگھائی - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval) {
		إذا (retval == EXT2_ET_EXTENT_NOT_لبھیا) {
			retval = 0;
			mapped = 0;
			إذا (!physical) {
#جے_تعریف_شدہ DEBUG
				لکھو("block %llu already unmapped\n",
					logical);
#جے_انت
				اذهب پورا;
			}
		} وإلا
			اذهب پورا;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب پورا;
	إذا (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_اگلا_پتی, &اگلا_extent);
	إذا (retval) {
		has_اگلا = 0;
		إذا (retval != EXT2_ET_EXTENT_NO_اگلا)
			اذهب پورا;
	} وإلا {
		dbg_print_extent("set_bmap: next_extent",
				 &اگلا_extent);
		has_اگلا = 1;
		إذا (اگلا_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			اگلا_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval && retval != EXT2_ET_EXTENT_NOT_لبھیا)
		اذهب پورا;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_پتی, &prev_extent);
	إذا (retval) {
		has_prev = 0;
		إذا (retval != EXT2_ET_EXTENT_NO_PREV)
			اذهب پورا;
	} وإلا {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		إذا (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval && retval != EXT2_ET_EXTENT_NOT_لبھیا)
		اذهب پورا;

	/* check if already pointing to the requested physical */
	إذا (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#جے_تعریف_شدہ DEBUG
		لکھو("physical block (at %llu) unchanged\n", logical);
#جے_انت
		اذهب پورا;
	}

	إذا (!mapped) {
#جے_تعریف_شدہ DEBUG
		لکھو("mapping unmapped logical block %llu\n", logical);
#جے_انت
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
		} وإلا إذا (has_اگلا &&
			   (logical == اگلا_extent.e_lblk - 1) &&
			   (physical == اگلا_extent.e_pblk - 1) &&
			   (new_uninit == اگلا_uninit) &&
			   ((صحيح) اگلا_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_اگلا_پتی,
						   &اگلا_extent);
			إذا (retval)
				اذهب پورا;
			اگلا_extent.e_len++;
			اگلا_extent.e_lblk--;
			اگلا_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &اگلا_extent);
		} وإلا إذا (logical < extent.e_lblk)
			retval = ext2fs_extent_داخل_کرو(handle, 0, &newextent);
		وإلا
			retval = ext2fs_extent_داخل_کرو(handle,
				      EXT2_EXTENT_داخل_کرو_AFTER, &newextent);
		إذا (retval)
			اذهب پورا;
		retval = ext2fs_extent_fix_parents(handle);
		إذا (retval)
			اذهب پورا;
	} وإلا إذا ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#جے_تعریف_شدہ DEBUG
		لکھو("(re/un)mapping only block in extent\n");
#جے_انت
		إذا (physical) {
			إذا (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحيح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_پتی, &prev_extent);
				إذا (retval)
					اذهب پورا;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_اگلا_پتی,
							   &extent);
				إذا (retval)
					اذهب پورا;
				اذهب delete_گرہ;

			} وإلا
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} وإلا {
		delete_گرہ:
			retval = ext2fs_extent_delete(handle, 0);
			إذا (retval)
				اذهب پورا;
			ec = ext2fs_extent_fix_parents(handle);
			إذا (ec != EXT2_ET_NO_CURRENT_گرہ)
				retval = ec;
		}

		إذا (retval)
			اذهب پورا;
	} وإلا إذا (logical == extent.e_lblk + extent.e_len - 1)  {
#جے_تعریف_شدہ DEBUG
		لکھو("(re/un)mapping last block in extent\n");
#جے_انت
		إذا (physical) {
			إذا (has_اگلا &&
			    (logical == (اگلا_extent.e_lblk - 1)) &&
			    (physical == (اگلا_extent.e_pblk - 1)) &&
			    (new_uninit == اگلا_uninit) &&
			    ((صحيح) اگلا_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_اگلا_پتی, &اگلا_extent);
				إذا (retval)
					اذهب پورا;
				اگلا_extent.e_len++;
				اگلا_extent.e_lblk--;
				اگلا_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &اگلا_extent);
				إذا (retval)
					اذهب پورا;
			} وإلا
				retval = ext2fs_extent_داخل_کرو(handle,
				      EXT2_EXTENT_داخل_کرو_AFTER, &newextent);
			إذا (retval)
				اذهب پورا;
			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval)
				اذهب پورا;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			إذا (retval)
				اذهب پورا;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			إذا (retval)
				اذهب پورا;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورا;
	} وإلا إذا (logical == extent.e_lblk) {
#جے_تعریف_شدہ DEBUG
		لکھو("(re/un)mapping first block in extent\n");
#جے_انت
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورا;
		retval = ext2fs_extent_fix_parents(handle);
		إذا (retval)
			اذهب پورا;
		إذا (physical) {
			إذا (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحيح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_پتی, &prev_extent);
				إذا (retval)
					اذهب پورا;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} وإلا
				retval = ext2fs_extent_داخل_کرو(handle,
							      0, &newextent);
			إذا (retval)
				اذهب پورا;
			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval)
				اذهب پورا;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_اگلا_پتی,
						   &extent);
			إذا (retval)
				اذهب پورا;
		}
	} وإلا {
		__u32	save_لمائی;
		blk64_t	save_lblk;
		بنية ext2fs_extent save_extent;
		errcode_t r2;

#جے_تعریف_شدہ DEBUG
		لکھو("(re/un)mapping in middle of extent\n");
#جے_انت
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_لمائی = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورا;
		/* insert our new extent, if any */
		إذا (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_داخل_کرو(handle,
					EXT2_EXTENT_داخل_کرو_AFTER, &newextent);
			إذا (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				إذا (r2 == 0)
					(فراغ)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				اذهب پورا;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_لمائی - extent.e_len - 1;
		retval = ext2fs_extent_داخل_کرو(handle,
				EXT2_EXTENT_داخل_کرو_AFTER, &extent);
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
			اذهب پورا;
		}
	}

پورا:
	/* get handle back to its position */
	إذا (orig_اچائی > handle->max_ڈونگھائی)
		orig_اچائی = handle->max_ڈونگھائی; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_اچائی, orig_lblk);
	أرجع retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحيح flags)
{
	بنية extent_رستہ		*رستہ;
	حرف 				*cp;
	بنية ext3_extent_سرخی	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستہ)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

#جے_تعریف_شدہ DEBUG
	{
		بنية ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		إذا (retval == 0) {
			لکھو("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#جے_انت

	رستہ = handle->رستہ + handle->level;
	إذا (!رستہ->curr)
		أرجع EXT2_ET_NO_CURRENT_گرہ;

	cp = رستہ->curr;

	/* Sanity check before memmove() */
	إذا (رستہ->بایاں < 0)
		أرجع EXT2_ET_EXTENT_پتی_BAD;

	إذا (رستہ->بایاں) {
		memmove(cp, cp + حجم(بنية ext3_extent_idx),
			رستہ->بایاں * حجم(بنية ext3_extent_idx));
		رستہ->بایاں--;
	} وإلا {
		بنية ext3_extent_idx	*ix = رستہ->curr;
		ix--;
		رستہ->curr = ix;
	}
	إذا (--رستہ->entries == 0)
		رستہ->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	إذا (رستہ->entries == 0 && handle->level) {
		إذا (!(flags & EXT2_EXTENT_DELETE_KEEP_خالی_تھاواں)) {
			بنية ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_اتے,
								&extent);
			إذا (retval)
				أرجع retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_بلاکاں -=
				(handle->fs->blocksize *
				 EXT2_FS_CLUSTER_تناسب(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_بلاک_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} وإلا {
		eh = (بنية ext3_extent_سرخی *) رستہ->buf;
		eh->eh_entries = ext2fs_cpu_منزل_le16(رستہ->entries);
		إذا ((رستہ->entries == 0) && (handle->level == 0)) {
			eh->eh_ڈونگھائی = 0;
			handle->max_ڈونگھائی = 0;
		}
		retval = update_رستہ(handle);
	}
	أرجع retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 بنية ext2_extent_info *info)
{
	بنية extent_رستہ		*رستہ;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, حجم(بنية ext2_extent_info));

	رستہ = handle->رستہ + handle->level;
	إذا (رستہ) {
		إذا (رستہ->curr)
			info->curr_اندراج = ((حرف *) رستہ->curr - رستہ->buf) /
				حجم(بنية ext3_extent_idx);
		وإلا
			info->curr_اندراج = 0;
		info->عدد_entries = رستہ->entries;
		info->max_entries = رستہ->max_entries;
		info->بائٹاں_avail = (رستہ->max_entries - رستہ->entries) *
			حجم(بنية ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_ڈونگھائی = handle->max_ڈونگھائی;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	أرجع 0;
}

size_t ext2fs_max_extent_ڈونگھائی(ext2_extent_handle_t handle)
{
	size_t iblock_sz = حجم(((بنية ext2_inode *)عدم)->i_بلاک);
	size_t iblock_extents = (iblock_sz - حجم(بنية ext3_extent_سرخی)) /
				حجم(بنية ext3_extent);
	size_t extents_فی_بلاک = (handle->fs->blocksize -
				    حجم(بنية ext3_extent_سرخی)) /
				   حجم(بنية ext3_extent);
	ساكن بنانشانی صحيح آخری_blocksize = 0;
	ساكن size_t آخری_نتیجہ = 0;

	إذا (آخری_blocksize && آخری_blocksize == handle->fs->blocksize)
		أرجع آخری_نتیجہ;

	آخری_نتیجہ = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_فی_بلاک));
	آخری_blocksize = handle->fs->blocksize;
	أرجع آخری_نتیجہ;
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
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_جڑ, &extent);
	إذا (errcode)
		اذهب باہرلا;

	افعل {
		/* Skip to the end of a block of leaf nodes */
		إذا (extent.e_flags & EXT2_EXTENT_FLAGS_پتی) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_آخری_SIB,
						    &extent);
			إذا (errcode)
				اكسر;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_اگلا, &extent);
		إذا (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_رستہ(handle);
	} طالما (errcode == 0);

باہرلا:
	/* Ok if we run off the end */
	إذا (errcode == EXT2_ET_EXTENT_NO_اگلا)
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

	منزل->e_pblk = ext2fs_le32_منزل_cpu(from->ee_شروع) +
		((__u64) ext2fs_le16_منزل_cpu(from->ee_شروع_hi)
			<< 32);
	منزل->e_lblk = ext2fs_le32_منزل_cpu(from->ee_بلاک);
	منزل->e_len = ext2fs_le16_منزل_cpu(from->ee_len);
	منزل->e_flags = EXT2_EXTENT_FLAGS_پتی;
	إذا (منزل->e_len > EXT_INIT_MAX_LEN) {
		منزل->e_len -= EXT_INIT_MAX_LEN;
		منزل->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	أرجع 0;
}

errcode_t ext2fs_گنتی_بلاکاں(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode, blk64_t *ret_گنتی)
{
	ext2_extent_handle_t	handle = عدم;
	بنية ext2fs_extent	extent;
	errcode_t		errcode;
	صحيح			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	إذا (errcode)
		اذهب باہرلا;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_جڑ, &extent);
	إذا (errcode)
		اذهب باہرلا;

	errcode = ext2fs_get_array(handle->max_ڈونگھائی, حجم(blk64_t),
				   &intermediate_nodes);
	إذا (errcode)
		اذهب باہرلا;

	blkcount = handle->level;
	طالما (!errcode) {
		إذا (extent.e_flags & EXT2_EXTENT_FLAGS_پتی) {
			blkcount += extent.e_len;
			لكل (i = 0; i < handle->level; i++) {
				إذا (intermediate_nodes[i] !=
					handle->رستہ[i].سرا_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->رستہ[i].سرا_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_اگلا, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
باہرلا:
	*ret_گنتی = blkcount;
	ext2fs_extent_free(handle);

	أرجع 0;
}

#جے_تعریف_شدہ DEBUG
/*
 * Override debugfs's prompt
 */
ثابت حرف *debug_prog_name = "tst_extents";

#جے_انت

