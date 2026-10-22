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
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "e2image.h"

#وصف_رد DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

بنية extent_رستو {
	حرف		*buf;
	صحيح		entries;
	صحيح		max_entries;
	صحيح		کاٻو;
	صحيح		گهمو_انگ;
	صحيح		flags;
	blk64_t		پڇاڙي_blk;
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
	صحيح			max_اونهائي;
	صحيح			max_paths;
	بنية extent_رستو	*رستو;
};

بنية ext2_extent_رستو {
	errcode_t		magic;
	صحيح			پن_اوچائي;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#جيڪڏهن_وصف_ٿيل DEBUG
ساكن فراغ dbg_show_سرو(بنية ext3_extent_سرو *eh)
{
	لکو("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_منزل_cpu(eh->eh_magic),
			ext2fs_le16_منزل_cpu(eh->eh_entries),
			ext2fs_le16_منزل_cpu(eh->eh_max),
			ext2fs_le16_منزل_cpu(eh->eh_اونهائي),
			ext2fs_le32_منزل_cpu(eh->eh_generation));
}

ساكن فراغ dbg_show_اشاريو(بنية ext3_extent_idx *ix)
{
	لکو("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_منزل_cpu(ix->ei_بلاڪ),
			ext2fs_le32_منزل_cpu(ix->ei_پن),
			ext2fs_le16_منزل_cpu(ix->ei_پن_hi),
			ext2fs_le16_منزل_cpu(ix->ei_اڻ_استعمال_ٿيل));
}

ساكن فراغ dbg_show_extent(بنية ext3_extent *ex)
{
	لکو("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_منزل_cpu(ex->ee_بلاڪ),
			ext2fs_le32_منزل_cpu(ex->ee_بلاڪ) +
			ext2fs_le16_منزل_cpu(ex->ee_len) - 1,
			ext2fs_le16_منزل_cpu(ex->ee_len),
			ext2fs_le32_منزل_cpu(ex->ee_شروعات),
			ext2fs_le16_منزل_cpu(ex->ee_شروعات_hi));
}

ساكن فراغ dbg_print_extent(حرف *desc, بنية ext2fs_extent *extent)
{
	إذا (desc)
		لکو("%s: ", desc);
	لکو("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_پن)
		fputs("LEAF ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_ٻيو_گهمو)
		fputs("2ND_VISIT ", stdout);
	إذا (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

ساكن فراغ dump_رستو(ثابت حرف *ٽيگ, بنية ext2_extent_handle *handle,
		      بنية extent_رستو *رستو)
{
	بنية extent_رستو *ppp = رستو;
	لکو("%s: level=%d\n", ٽيگ, handle->level);

	افعل {
		لکو("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       ٽيگ, (ppp - handle->رستو), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->کاٻو, ppp->گهمو_انگ, ppp->flags,
		       ppp->پڇاڙي_blk, ppp->curr, ppp->curr - (فراغ *)ppp->buf);
		لکو("  ");
		dbg_show_سرو((بنية ext3_extent_سرو *)ppp->buf);
		إذا (ppp->curr) {
			لکو("  ");
			dbg_show_اشاريو(ppp->curr);
			لکو("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} طالما (ppp >= handle->رستو);
	fflush(stdout);

	أرجع;
}

#وإلا
#وصف_ڪريو dbg_show_سرو(eh) افعل { } طالما (0)
#وصف_ڪريو dbg_show_اشاريو(ix) افعل { } طالما (0)
#وصف_ڪريو dbg_show_extent(ex) افعل { } طالما (0)
#وصف_ڪريو dbg_print_extent(desc, ex) افعل { } طالما (0)
#وصف_ڪريو dump_رستو(ٽيگ, handle, رستو) افعل { } طالما (0)
#جيڪڏهن_پورو

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_سرو_verify(فراغ *ptr, صحيح سائيز)
{
	صحيح eh_max, داخلا_سائيز;
	بنية ext3_extent_سرو *eh = ptr;

	dbg_show_سرو(eh);
	إذا (ext2fs_le16_منزل_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		أرجع EXT2_ET_EXTENT_سرو_BAD;
	إذا (ext2fs_le16_منزل_cpu(eh->eh_entries) > ext2fs_le16_منزل_cpu(eh->eh_max))
		أرجع EXT2_ET_EXTENT_سرو_BAD;
	إذا (eh->eh_اونهائي == 0)
		داخلا_سائيز = حجم(بنية ext3_extent);
	وإلا
		داخلا_سائيز = حجم(بنية ext3_extent_idx);

	eh_max = (سائيز - حجم(*eh)) / داخلا_سائيز;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	إذا ((ext2fs_le16_منزل_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_منزل_cpu(eh->eh_max) < (eh_max - 2)))
		أرجع EXT2_ET_EXTENT_سرو_BAD;

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

	إذا (handle->رستو) {
		لكل (i = 1; i < handle->max_paths; i++) {
			إذا (handle->رستو[i].buf)
				ext2fs_free_mem(&handle->رستو[i].buf);
		}
		ext2fs_free_mem(&handle->رستو);
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
	بنية ext3_extent_سرو	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!inode)
		إذا ((ino == 0) || (ino > fs->super->s_inodes_ڳڻپ))
			أرجع EXT2_ET_BAD_INODE_انگ;

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

	eh = (بنية ext3_extent_سرو *) &handle->inode->i_بلاڪ[0];

	لكل (i=0; i < EXT2_N_بلاڪون; i++)
		إذا (handle->inode->i_بلاڪ[i])
			اكسر;
	إذا (i >= EXT2_N_بلاڪون) {
		eh->eh_magic = ext2fs_cpu_منزل_le16(EXT3_EXT_MAGIC);
		eh->eh_اونهائي = 0;
		eh->eh_entries = 0;
		i = (حجم(handle->inode->i_بلاڪ) - حجم(*eh)) /
			حجم(بنية ext3_extent);
		eh->eh_max = ext2fs_cpu_منزل_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	إذا (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		اذهب errout;
	}

	retval = ext2fs_extent_سرو_verify(eh, حجم(handle->inode->i_بلاڪ));
	إذا (retval)
		اذهب errout;

	handle->max_اونهائي = ext2fs_le16_منزل_cpu(eh->eh_اونهائي);
	handle->type = ext2fs_le16_منزل_cpu(eh->eh_magic);

	handle->max_paths = handle->max_اونهائي + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    حجم(بنية extent_رستو),
				    &handle->رستو);
	handle->رستو[0].buf = (حرف *) handle->inode->i_بلاڪ;

	handle->رستو[0].کاٻو = handle->رستو[0].entries =
		ext2fs_le16_منزل_cpu(eh->eh_entries);
	handle->رستو[0].max_entries = ext2fs_le16_منزل_cpu(eh->eh_max);
	handle->رستو[0].curr = 0;
	handle->رستو[0].پڇاڙي_blk =
		(EXT2_I_سائيز(handle->inode) + fs->blocksize - 1) >>
		 EXT2_بلاڪ_سائيز_بٽ(fs->super);
	handle->رستو[0].blk = 0;
	handle->رستو[0].گهمو_انگ = 1;
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
	بنية extent_رستو	*رستو, *newpath, *tp;
	بنية ext3_extent_سرو	*eh;
	بنية ext3_extent_idx		*ix = 0;
	بنية ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				پڇاڙي_blk;
	صحيح				orig_op, op, l;
	صحيح				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!handle->رستو)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

	orig_op = op = flags & EXT2_EXTENT_MOVE_نقاب;

retry:
	رستو = handle->رستو + handle->level;
	إذا ((orig_op == EXT2_EXTENT_ايندڙ) ||
	    (orig_op == EXT2_EXTENT_ايندڙ_پن)) {
		إذا (handle->level < handle->max_اونهائي) {
			/* interior node */
			إذا (رستو->گهمو_انگ == 0) {
				رستو->گهمو_انگ++;
				op = EXT2_EXTENT_هيٺ_ڏانهن;
			} وإلا إذا (رستو->کاٻو > 0)
				op = EXT2_EXTENT_ايندڙ_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_مٿي;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_ايندڙ;
		} وإلا {
			/* leaf node */
			إذا (رستو->کاٻو > 0)
				op = EXT2_EXTENT_ايندڙ_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_مٿي;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_ايندڙ;
		}
		إذا (op != EXT2_EXTENT_ايندڙ_SIB) {
#جيڪڏهن_وصف_ٿيل DEBUG_GET_EXTENT
			لکو("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_هيٺ_ڏانهن) ? "down" :
			       ((op == EXT2_EXTENT_مٿي) ? "up" : "unknown"));
#جيڪڏهن_پورو
		}
	}

	إذا ((orig_op == EXT2_EXTENT_PREV) ||
	    (orig_op == EXT2_EXTENT_PREV_پن)) {
		إذا (handle->level < handle->max_اونهائي) {
			/* interior node */
			إذا (رستو->گهمو_انگ > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_هيٺ_ڏانهن_AND_آخري;
			} وإلا إذا (رستو->کاٻو < رستو->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_مٿي;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_PREV;
		} وإلا {
			/* leaf node */
			إذا (رستو->کاٻو < رستو->entries-1)
				op = EXT2_EXTENT_PREV_SIB;
			وإلا إذا (handle->level > 0)
				op = EXT2_EXTENT_مٿي;
			وإلا
				أرجع EXT2_ET_EXTENT_NO_PREV;
		}
		إذا (op != EXT2_EXTENT_PREV_SIB) {
#جيڪڏهن_وصف_ٿيل DEBUG_GET_EXTENT
			لکو("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_هيٺ_ڏانهن_AND_آخري) ? "down/last" :
			       ((op == EXT2_EXTENT_مٿي) ? "up" : "unknown"));
#جيڪڏهن_پورو
		}
	}

	إذا (orig_op == EXT2_EXTENT_آخري_پن) {
		إذا ((handle->level < handle->max_اونهائي) &&
		    (رستو->کاٻو == 0))
			op = EXT2_EXTENT_هيٺ_ڏانهن;
		وإلا
			op = EXT2_EXTENT_آخري_SIB;
#جيڪڏهن_وصف_ٿيل DEBUG_GET_EXTENT
		لکو("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_هيٺ_ڏانهن) ? "down" : "last_sib");
#جيڪڏهن_پورو
	}

	بدل (op) {
	حالة EXT2_EXTENT_CURRENT:
		ix = رستو->curr;
		اكسر;
	حالة EXT2_EXTENT_پاڙ:
		handle->level = 0;
		رستو = handle->رستو + handle->level;
		/* fallthrough */
	حالة EXT2_EXTENT_پهريون_SIB:
		رستو->کاٻو = رستو->entries;
		رستو->curr = 0;
		/* fallthrough */
	حالة EXT2_EXTENT_ايندڙ_SIB:
		إذا (رستو->کاٻو <= 0)
			أرجع EXT2_ET_EXTENT_NO_ايندڙ;
		إذا (رستو->curr) {
			ix = رستو->curr;
			ix++;
		} وإلا {
			eh = (بنية ext3_extent_سرو *) رستو->buf;
			ix = EXT_پهريون_اشاريو(eh);
		}
		رستو->کاٻو--;
		رستو->curr = ix;
		رستو->گهمو_انگ = 0;
		اكسر;
	حالة EXT2_EXTENT_PREV_SIB:
		إذا (!رستو->curr ||
		    رستو->کاٻو+1 >= رستو->entries)
			أرجع EXT2_ET_EXTENT_NO_PREV;
		ix = رستو->curr;
		ix--;
		رستو->curr = ix;
		رستو->کاٻو++;
		إذا (handle->level < handle->max_اونهائي)
			رستو->گهمو_انگ = 1;
		اكسر;
	حالة EXT2_EXTENT_آخري_SIB:
		eh = (بنية ext3_extent_سرو *) رستو->buf;
		رستو->curr = EXT_آخري_EXTENT(eh);
		ix = رستو->curr;
		رستو->کاٻو = 0;
		رستو->گهمو_انگ = 0;
		اكسر;
	حالة EXT2_EXTENT_مٿي:
		إذا (handle->level <= 0)
			أرجع EXT2_ET_EXTENT_NO_مٿي;
		handle->level--;
		رستو--;
		ix = رستو->curr;
		إذا ((orig_op == EXT2_EXTENT_PREV) ||
		    (orig_op == EXT2_EXTENT_PREV_پن))
			رستو->گهمو_انگ = 0;
		اكسر;
	حالة EXT2_EXTENT_هيٺ_ڏانهن:
	حالة EXT2_EXTENT_هيٺ_ڏانهن_AND_آخري:
		إذا (!رستو->curr ||(handle->level >= handle->max_اونهائي))
			أرجع EXT2_ET_EXTENT_NO_هيٺ_ڏانهن;

		ix = رستو->curr;
		newpath = رستو + 1;
		إذا (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			إذا (retval)
				أرجع retval;
		}
		blk = ext2fs_le32_منزل_cpu(ix->ei_پن) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_پن_hi) << 32);
		لكل (l = handle->level, tp = رستو; l > 0; l--, tp--) {
			إذا (blk == tp->blk)
				أرجع EXT2_ET_EXTENT_چڪر;
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

		eh = (بنية ext3_extent_سرو *) newpath->buf;

		retval = ext2fs_extent_سرو_verify(eh, handle->fs->blocksize);
		إذا (retval) {
			handle->level--;
			أرجع retval;
		}

		إذا (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_بلاڪ_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->کاٻو = newpath->entries =
			ext2fs_le16_منزل_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_منزل_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		إذا (newpath->کاٻو <= 0)
			أرجع EXT2_ET_EXTENT_NO_هيٺ_ڏانهن;

		إذا (رستو->کاٻو > 0) {
			ix++;
			newpath->پڇاڙي_blk = ext2fs_le32_منزل_cpu(ix->ei_بلاڪ);
		} وإلا
			newpath->پڇاڙي_blk = رستو->پڇاڙي_blk;

		رستو = newpath;
		إذا (op == EXT2_EXTENT_هيٺ_ڏانهن) {
			ix = EXT_پهريون_اشاريو((بنية ext3_extent_سرو *) eh);
			رستو->curr = ix;
			رستو->کاٻو = رستو->entries - 1;
			رستو->گهمو_انگ = 0;
		} وإلا {
			ix = EXT_آخري_اشاريو((بنية ext3_extent_سرو *) eh);
			رستو->curr = ix;
			رستو->کاٻو = 0;
			إذا (handle->level < handle->max_اونهائي)
				رستو->گهمو_انگ = 1;
		}
#جيڪڏهن_وصف_ٿيل DEBUG_GET_EXTENT
		لکو("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_اونهائي,
			   رستو->پڇاڙي_blk);
#جيڪڏهن_پورو
		اكسر;
	افتراضي:
		أرجع EXT2_ET_OP_NOT_SUPPORTED;
	}

	إذا (!ix)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

	extent->e_flags = 0;
#جيڪڏهن_وصف_ٿيل DEBUG_GET_EXTENT
	لکو("(Left %d)\n", رستو->کاٻو);
#جيڪڏهن_پورو

	إذا (handle->level == handle->max_اونهائي) {
		ex = (بنية ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_منزل_cpu(ex->ee_شروعات) +
			((__u64) ext2fs_le16_منزل_cpu(ex->ee_شروعات_hi) << 32);
		extent->e_lblk = ext2fs_le32_منزل_cpu(ex->ee_بلاڪ);
		extent->e_len = ext2fs_le16_منزل_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_پن;
		إذا (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} وإلا {
		extent->e_pblk = ext2fs_le32_منزل_cpu(ix->ei_پن) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_پن_hi) << 32);
		extent->e_lblk = ext2fs_le32_منزل_cpu(ix->ei_بلاڪ);
		إذا (رستو->کاٻو > 0) {
			ix++;
			پڇاڙي_blk = ext2fs_le32_منزل_cpu(ix->ei_بلاڪ);
		} وإلا
			پڇاڙي_blk = رستو->پڇاڙي_blk;

		extent->e_len = پڇاڙي_blk - extent->e_lblk;
	}
	إذا (رستو->گهمو_انگ)
		extent->e_flags |= EXT2_EXTENT_FLAGS_ٻيو_گهمو;

	إذا (((orig_op == EXT2_EXTENT_ايندڙ_پن) ||
	     (orig_op == EXT2_EXTENT_PREV_پن)) &&
	    (handle->level != handle->max_اونهائي))
		اذهب retry;

	إذا ((orig_op == EXT2_EXTENT_آخري_پن) &&
	    ((handle->level != handle->max_اونهائي) ||
	     (رستو->کاٻو != 0)))
		اذهب retry;

	إذا (failed_csum)
		أرجع EXT2_ET_EXTENT_CSUM_INVALID;

	أرجع 0;
}

ساكن errcode_t update_رستو(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent_سرو	*eh;

	إذا (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} وإلا {
		ix = handle->رستو[handle->level - 1].curr;
		blk = ext2fs_le32_منزل_cpu(ix->ei_پن) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_پن_hi) << 32);

		/* then update the checksum */
		eh = (بنية ext3_extent_سرو *)
				handle->رستو[handle->level].buf;
		retval = ext2fs_extent_بلاڪ_csum_سيٽ(handle->fs, handle->ino,
						      eh);
		إذا (retval)
			أرجع retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->رستو[handle->level].buf);
	}
	أرجع retval;
}

#إذا 0
errcode_t ext2fs_extent_save_رستو(ext2_extent_handle_t handle,
				  ext2_extent_رستو_t *ret_رستو)
{
	ext2_extent_رستو_t	save_رستو;
	بنية ext2fs_extent	extent;
	بنية ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_extent_get_info(handle, &info);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(حجم(بنية ext2_extent_رستو), &save_رستو);
	إذا (retval)
		أرجع retval;
	memset(save_رستو, 0, حجم(بنية ext2_extent_رستو));

	save_رستو->magic = EXT2_ET_MAGIC_EXTENT_رستو;
	save_رستو->پن_اوچائي = info.max_اونهائي - info.curr_level - 1;
	save_رستو->lblk = extent.e_lblk;

	*ret_رستو = save_رستو;
	أرجع 0;
}

errcode_t ext2fs_extent_free_رستو(ext2_extent_رستو_t رستو)
{
	EXT2_CHECK_MAGIC(رستو, EXT2_ET_MAGIC_EXTENT_رستو);

	ext2fs_free_mem(&رستو);
	أرجع 0;
}
#جيڪڏهن_پورو

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
			      صحيح پن_level, blk64_t blk)
{
	بنية ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_پاڙ, &extent);
	إذا (retval) {
		إذا (retval == EXT2_ET_EXTENT_NO_ايندڙ)
			retval = EXT2_ET_EXTENT_NOT_مليو;
		أرجع retval;
	}

	إذا (پن_level > handle->max_اونهائي) {
#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("leaf level %d greater than tree depth %d\n",
			پن_level, handle->max_اونهائي);
#جيڪڏهن_پورو
		أرجع EXT2_ET_OP_NOT_SUPPORTED;
	}

#جيڪڏهن_وصف_ٿيل DEBUG
	لکو("goto extent ino %u, level %d, %llu\n", handle->ino,
	       پن_level, blk);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#جيڪڏهن_پورو
	طالما (1) {
		إذا (handle->max_اونهائي - handle->level == پن_level) {
			/* block is in this &extent */
			إذا ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				أرجع 0;
			إذا (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_PREV_SIB,
							   &extent);
				أرجع EXT2_ET_EXTENT_NOT_مليو;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ايندڙ_SIB,
						   &extent);
			إذا (retval == EXT2_ET_EXTENT_NO_ايندڙ)
				أرجع EXT2_ET_EXTENT_NOT_مليو;
			إذا (retval)
				أرجع retval;
			استمر;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_ايندڙ_SIB,
					   &extent);
		إذا (retval == EXT2_ET_EXTENT_NO_ايندڙ)
			اذهب go_هيٺ_ڏانهن;
		إذا (retval)
			أرجع retval;

#جيڪڏهن_وصف_ٿيل DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#جيڪڏهن_پورو
		إذا (blk == extent.e_lblk)
			اذهب go_هيٺ_ڏانهن;
		إذا (blk > extent.e_lblk)
			استمر;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_SIB,
					   &extent);
		إذا (retval)
			أرجع retval;

#جيڪڏهن_وصف_ٿيل DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#جيڪڏهن_پورو

	go_هيٺ_ڏانهن:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_هيٺ_ڏانهن,
					   &extent);
		إذا (retval)
			أرجع retval;

#جيڪڏهن_وصف_ٿيل DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#جيڪڏهن_پورو
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
	صحيح				orig_اوچائي;
	blk64_t				شروعات;
	بنية extent_رستو		*رستو;
	بنية ext2fs_extent		extent;
	بنية ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستو)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

	رستو = handle->رستو + handle->level;
	إذا (!رستو->curr)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب پورو;

	/* modified node's start block */
	شروعات = extent.e_lblk;

	إذا ((retval = ext2fs_extent_get_info(handle, &info)))
		أرجع retval;
	orig_اوچائي = info.max_اونهائي - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	طالما (handle->level > 0 &&
	       (رستو->کاٻو == رستو->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_مٿي, &extent);
		إذا (retval)
			اذهب پورو;
		إذا (extent.e_lblk == شروعات)
			اكسر;
		رستو = handle->رستو + handle->level;
		extent.e_len += (extent.e_lblk - شروعات);
		extent.e_lblk = شروعات;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورو;
		update_رستو(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_اوچائي, شروعات);
پورو:
	أرجع retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
				بنية ext2fs_extent *extent)
{
	بنية extent_رستو		*رستو;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستو)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

	رستو = handle->رستو + handle->level;
	إذا (!رستو->curr)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

#جيڪڏهن_وصف_ٿيل DEBUG
	لکو("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#جيڪڏهن_پورو

	إذا (handle->level == handle->max_اونهائي) {
		ex = رستو->curr;

		ex->ee_بلاڪ = ext2fs_cpu_منزل_le32(extent->e_lblk);
		ex->ee_شروعات = ext2fs_cpu_منزل_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_شروعات_hi = ext2fs_cpu_منزل_le16(extent->e_pblk >> 32);
		إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			إذا (extent->e_len > EXT_UNINIT_MAX_LEN)
				أرجع EXT2_ET_EXTENT_INVALID_ڊيگهائي;
			ex->ee_len = ext2fs_cpu_منزل_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} وإلا {
			إذا (extent->e_len > EXT_INIT_MAX_LEN)
				أرجع EXT2_ET_EXTENT_INVALID_ڊيگهائي;
			ex->ee_len = ext2fs_cpu_منزل_le16(extent->e_len);
		}
	} وإلا {
		ix = رستو->curr;

		ix->ei_پن = ext2fs_cpu_منزل_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_پن_hi = ext2fs_cpu_منزل_le16(extent->e_pblk >> 32);
		ix->ei_بلاڪ = ext2fs_cpu_منزل_le32(extent->e_lblk);
		ix->ei_اڻ_استعمال_ٿيل = 0;
	}
	update_رستو(handle);
	أرجع 0;
}

ساكن صحيح splitting_at_eof(بنية ext2_extent_handle *handle,
			    بنية extent_رستو *رستو)
{
	بنية extent_رستو *ppp = رستو;
	dump_رستو(__func__, handle, رستو);

	إذا (handle->level == 0)
		أرجع 0;

	افعل {
		إذا (ppp->کاٻو)
			أرجع 0;
		ppp--;
	} طالما (ppp >= handle->رستو);

	أرجع 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
ساكن errcode_t extent_ڳنڍ_ورهايو(ext2_extent_handle_t handle,
				   صحيح expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_ڳنڍ_pblk;
	blk64_t				new_ڳنڍ_شروعات;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	صحيح				orig_اوچائي;
	حرف				*بلاڪ_buf = عدم;
	بنية ext2fs_extent		extent;
	بنية extent_رستو		*رستو, *newpath = 0;
	بنية ext3_extent_سرو	*eh, *neweh;
	صحيح				tocopy;
	صحيح				new_پاڙ = 0;
	بنية ext2_extent_info		info;
	صحيح				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستو)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

#جيڪڏهن_وصف_ٿيل DEBUG
	لکو("splitting node at level %d\n", handle->level);
#جيڪڏهن_پورو
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب پورو;

	retval = ext2fs_extent_get_info(handle, &info);
	إذا (retval)
		اذهب پورو;

	/* save the position we were originally splitting... */
	orig_اوچائي = info.max_اونهائي - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	رستو = handle->رستو + handle->level;
	eh = (بنية ext3_extent_سرو *) رستو->buf;
	إذا (handle->level == handle->max_اونهائي) {
		بنية ext3_extent	*ex;

		ex = EXT_پهريون_EXTENT(eh);
		goal_blk = ext2fs_le32_منزل_cpu(ex->ee_شروعات) +
			((__u64) ext2fs_le16_منزل_cpu(ex->ee_شروعات_hi) << 32);
	} وإلا {
		بنية ext3_extent_idx	*ix;

		ix = EXT_پهريون_اشاريو(eh);
		goal_blk = ext2fs_le32_منزل_cpu(ix->ei_پن) +
			((__u64) ext2fs_le16_منزل_cpu(ix->ei_پن_hi) << 32);
	}
	goal_blk -= EXT2_FS_CLUSTER_تناسب(handle->fs);
	goal_blk &= ~EXT2_FS_CLUSTER_نقاب(handle->fs);

	/* Is there room in the parent for a new entry? */
	إذا (handle->level &&
			(handle->رستو[handle->level - 1].entries >=
			 handle->رستو[handle->level - 1].max_entries)) {

#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("parent level %d full; splitting it too\n",
							handle->level - 1);
#جيڪڏهن_پورو
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_مٿي, &extent);
		إذا (retval)
			اذهب پورو;

		retval = extent_ڳنڍ_ورهايو(handle, expand_allowed);
		إذا (retval)
			اذهب پورو;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_اوچائي, orig_lblk);
		إذا (retval)
			اذهب پورو;
	}

	/* At this point, parent should have room for this split */
	رستو = handle->رستو + handle->level;
	إذا (!رستو->curr)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, رستو) : 0;

	/* extent header of the current node we'll split */
	eh = (بنية ext3_extent_سرو *)رستو->buf;

	/* splitting root level means moving them all out */
	إذا (handle->level == 0) {
		new_پاڙ = 1;
		tocopy = ext2fs_le16_منزل_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    حجم(بنية extent_رستو),
					    &newpath);
		إذا (retval)
			اذهب پورو;
	} وإلا {
		إذا (no_balance)
			tocopy = 1;
		وإلا
			tocopy = ext2fs_le16_منزل_cpu(eh->eh_entries) / 2;
	}

#جيڪڏهن_وصف_ٿيل DEBUG
	لکو("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_منزل_cpu(eh->eh_entries),
				handle->level);
#جيڪڏهن_پورو

	إذا (!tocopy && !no_balance) {
#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("Nothing to copy to new block!\n");
#جيڪڏهن_پورو
		retval = EXT2_ET_CANT_ورهايو_EXTENT;
		اذهب پورو;
	}

	/* first we need a new block, or can do nothing. */
	بلاڪ_buf = يادگيري_ڏيو(handle->fs->blocksize);
	إذا (!بلاڪ_buf) {
		retval = ENOMEM;
		اذهب پورو;
	}

	إذا (!goal_blk)
		goal_blk = ext2fs_ڳوليو_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, بلاڪ_buf,
				    &new_ڳنڍ_pblk);
	إذا (retval)
		اذهب پورو;

#جيڪڏهن_وصف_ٿيل DEBUG
	لکو("will copy to new node at block %lu\n",
	       (بنانشان ڊگهو) new_ڳنڍ_pblk);
#جيڪڏهن_پورو

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (بنية ext3_extent_سرو *) بلاڪ_buf;
	يادگيري_نقل(neweh, eh, حجم(بنية ext3_extent_سرو));
	neweh->eh_entries = ext2fs_cpu_منزل_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_منزل_le16((handle->fs->blocksize -
			 حجم(بنية ext3_extent_سرو)) /
				حجم(بنية ext3_extent));

	/* then the entries for the new block... */
	يادگيري_نقل(EXT_پهريون_اشاريو(neweh),
		EXT_پهريون_اشاريو(eh) +
			(ext2fs_le16_منزل_cpu(eh->eh_entries) - tocopy),
		حجم(بنية ext3_extent_idx) * tocopy);

	new_ڳنڍ_شروعات = ext2fs_le32_منزل_cpu(EXT_پهريون_اشاريو(neweh)->ei_بلاڪ);

	/* then update the checksum */
	retval = ext2fs_extent_بلاڪ_csum_سيٽ(handle->fs, handle->ino, neweh);
	إذا (retval)
		اذهب پورو;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_ڳنڍ_pblk, 1,
					بلاڪ_buf);

	إذا (retval)
		اذهب پورو;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	إذا (handle->level == 0) {
		يادگيري_نقل(newpath, رستو,
		       حجم(بنية extent_رستو) * handle->max_paths);
		handle->رستو = newpath;
		newpath = رستو;
		رستو = handle->رستو;
		رستو->entries = 1;
		رستو->کاٻو = رستو->max_entries - 1;
		handle->max_اونهائي++;
		handle->max_paths++;
		eh->eh_اونهائي = ext2fs_cpu_منزل_le16(handle->max_اونهائي);
	} وإلا {
		رستو->entries -= tocopy;
		رستو->کاٻو -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_منزل_le16(رستو->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_رستو(handle);
	إذا (retval)
		اذهب پورو;

	/* now go up and insert/replace index for new node we created */
	إذا (new_پاڙ) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_پهريون_SIB, &extent);
		إذا (retval)
			اذهب پورو;

		extent.e_lblk = new_ڳنڍ_شروعات;
		extent.e_pblk = new_ڳنڍ_pblk;
		extent.e_len = handle->رستو[0].پڇاڙي_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورو;
	} وإلا {
		__u32 new_ڳنڍ_ڊيگهائي;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_مٿي, &extent);
		/* will insert after this one; it's length is shorter now */
		new_ڳنڍ_ڊيگهائي = new_ڳنڍ_شروعات - extent.e_lblk;
		extent.e_len -= new_ڳنڍ_ڊيگهائي;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورو;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_ڳنڍ_شروعات;
		extent.e_pblk = new_ڳنڍ_pblk;
		extent.e_len = new_ڳنڍ_ڊيگهائي;
		retval = ext2fs_extent_وجهو(handle, EXT2_EXTENT_وجهو_AFTER, &extent);
		إذا (retval)
			اذهب پورو;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_اوچائي, orig_lblk);
	إذا (retval)
		اذهب پورو;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_جوڙيو_بلاڪون(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	إذا (retval)
		اذهب پورو;

پورو:
	إذا (newpath)
		ext2fs_free_mem(&newpath);
	حرر(بلاڪ_buf);

	أرجع retval;
}

errcode_t ext2fs_extent_ڳنڍ_ورهايو(ext2_extent_handle_t handle)
{
	أرجع extent_ڳنڍ_ورهايو(handle, 0);
}

errcode_t ext2fs_extent_وجهو(ext2_extent_handle_t handle, صحيح flags,
				      بنية ext2fs_extent *extent)
{
	بنية extent_رستو		*رستو;
	بنية ext3_extent_idx		*ix;
	بنية ext3_extent_سرو	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستو)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

#جيڪڏهن_وصف_ٿيل DEBUG
	لکو("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#جيڪڏهن_پورو

	رستو = handle->رستو + handle->level;

	إذا (رستو->entries >= رستو->max_entries) {
		إذا (flags & EXT2_EXTENT_وجهو_NOSPLIT) {
			أرجع EXT2_ET_CANT_وجهو_EXTENT;
		} وإلا {
#جيڪڏهن_وصف_ٿيل DEBUG
			لکو("node full (level %d) - splitting\n",
				   handle->level);
#جيڪڏهن_پورو
			retval = extent_ڳنڍ_ورهايو(handle, 1);
			إذا (retval)
				أرجع retval;
			رستو = handle->رستو + handle->level;
		}
	}

	eh = (بنية ext3_extent_سرو *) رستو->buf;
	إذا (رستو->curr) {
		ix = رستو->curr;
		إذا (flags & EXT2_EXTENT_وجهو_AFTER) {
			ix++;
			رستو->کاٻو--;
		}
	} وإلا {
		ix = EXT_پهريون_اشاريو(eh);
		رستو->کاٻو = -1;
	}

	رستو->curr = ix;

	إذا (رستو->کاٻو >= 0)
		memmove(ix + 1, ix,
			(رستو->کاٻو+1) * حجم(بنية ext3_extent_idx));
	رستو->کاٻو++;
	رستو->entries++;

	eh = (بنية ext3_extent_سرو *) رستو->buf;
	eh->eh_entries = ext2fs_cpu_منزل_le16(رستو->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	إذا (retval)
		اذهب errout;

	retval = update_رستو(handle);
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

errcode_t ext2fs_extent_سيٽ_bmap(ext2_extent_handle_t handle,
				 blk64_t logical, blk64_t physical, صحيح flags)
{
	errcode_t		ec, retval = 0;
	صحيح			mapped = 1; /* logical is mapped? */
	صحيح			orig_اوچائي;
	صحيح			extent_uninit = 0;
	صحيح			prev_uninit = 0;
	صحيح			ايندڙ_uninit = 0;
	صحيح			new_uninit = 0;
	صحيح			max_len = EXT_INIT_MAX_LEN;
	صحيح			has_prev, has_ايندڙ;
	blk64_t			orig_lblk;
	بنية extent_رستو	*رستو;
	بنية ext2fs_extent	extent, ايندڙ_extent, prev_extent;
	بنية ext2fs_extent	newextent;
	بنية ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#جيڪڏهن_وصف_ٿيل DEBUG
	لکو("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#جيڪڏهن_پورو

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستو)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

	رستو = handle->رستو + handle->level;

	إذا (flags & EXT2_EXTENT_سيٽ_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	إذا (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_پن;
		إذا (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	إذا ((handle->max_اونهائي == 0) && (رستو->entries == 0)) {
		retval = ext2fs_extent_وجهو(handle, 0, &newextent);
		أرجع retval;
	}

	/* save our original location in the extent tree */
	إذا ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		إذا (retval != EXT2_ET_NO_CURRENT_ڳنڍ)
			أرجع retval;
		memset(&extent, 0, حجم(extent));
	}
	إذا ((retval = ext2fs_extent_get_info(handle, &info)))
		أرجع retval;
	orig_اوچائي = info.max_اونهائي - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval) {
		إذا (retval == EXT2_ET_EXTENT_NOT_مليو) {
			retval = 0;
			mapped = 0;
			إذا (!physical) {
#جيڪڏهن_وصف_ٿيل DEBUG
				لکو("block %llu already unmapped\n",
					logical);
#جيڪڏهن_پورو
				اذهب پورو;
			}
		} وإلا
			اذهب پورو;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		اذهب پورو;
	إذا (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_ايندڙ_پن, &ايندڙ_extent);
	إذا (retval) {
		has_ايندڙ = 0;
		إذا (retval != EXT2_ET_EXTENT_NO_ايندڙ)
			اذهب پورو;
	} وإلا {
		dbg_print_extent("set_bmap: next_extent",
				 &ايندڙ_extent);
		has_ايندڙ = 1;
		إذا (ايندڙ_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			ايندڙ_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval && retval != EXT2_ET_EXTENT_NOT_مليو)
		اذهب پورو;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_PREV_پن, &prev_extent);
	إذا (retval) {
		has_prev = 0;
		إذا (retval != EXT2_ET_EXTENT_NO_PREV)
			اذهب پورو;
	} وإلا {
		has_prev = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &prev_extent);
		إذا (prev_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			prev_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	إذا (retval && retval != EXT2_ET_EXTENT_NOT_مليو)
		اذهب پورو;

	/* check if already pointing to the requested physical */
	إذا (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("physical block (at %llu) unchanged\n", logical);
#جيڪڏهن_پورو
		اذهب پورو;
	}

	إذا (!mapped) {
#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("mapping unmapped logical block %llu\n", logical);
#جيڪڏهن_پورو
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
		} وإلا إذا (has_ايندڙ &&
			   (logical == ايندڙ_extent.e_lblk - 1) &&
			   (physical == ايندڙ_extent.e_pblk - 1) &&
			   (new_uninit == ايندڙ_uninit) &&
			   ((صحيح) ايندڙ_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ايندڙ_پن,
						   &ايندڙ_extent);
			إذا (retval)
				اذهب پورو;
			ايندڙ_extent.e_len++;
			ايندڙ_extent.e_lblk--;
			ايندڙ_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &ايندڙ_extent);
		} وإلا إذا (logical < extent.e_lblk)
			retval = ext2fs_extent_وجهو(handle, 0, &newextent);
		وإلا
			retval = ext2fs_extent_وجهو(handle,
				      EXT2_EXTENT_وجهو_AFTER, &newextent);
		إذا (retval)
			اذهب پورو;
		retval = ext2fs_extent_fix_parents(handle);
		إذا (retval)
			اذهب پورو;
	} وإلا إذا ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("(re/un)mapping only block in extent\n");
#جيڪڏهن_پورو
		إذا (physical) {
			إذا (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحيح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_PREV_پن, &prev_extent);
				إذا (retval)
					اذهب پورو;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_ايندڙ_پن,
							   &extent);
				إذا (retval)
					اذهب پورو;
				اذهب delete_ڳنڍ;

			} وإلا
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} وإلا {
		delete_ڳنڍ:
			retval = ext2fs_extent_delete(handle, 0);
			إذا (retval)
				اذهب پورو;
			ec = ext2fs_extent_fix_parents(handle);
			إذا (ec != EXT2_ET_NO_CURRENT_ڳنڍ)
				retval = ec;
		}

		إذا (retval)
			اذهب پورو;
	} وإلا إذا (logical == extent.e_lblk + extent.e_len - 1)  {
#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("(re/un)mapping last block in extent\n");
#جيڪڏهن_پورو
		إذا (physical) {
			إذا (has_ايندڙ &&
			    (logical == (ايندڙ_extent.e_lblk - 1)) &&
			    (physical == (ايندڙ_extent.e_pblk - 1)) &&
			    (new_uninit == ايندڙ_uninit) &&
			    ((صحيح) ايندڙ_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_ايندڙ_پن, &ايندڙ_extent);
				إذا (retval)
					اذهب پورو;
				ايندڙ_extent.e_len++;
				ايندڙ_extent.e_lblk--;
				ايندڙ_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &ايندڙ_extent);
				إذا (retval)
					اذهب پورو;
			} وإلا
				retval = ext2fs_extent_وجهو(handle,
				      EXT2_EXTENT_وجهو_AFTER, &newextent);
			إذا (retval)
				اذهب پورو;
			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval)
				اذهب پورو;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			إذا (retval)
				اذهب پورو;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			إذا (retval)
				اذهب پورو;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورو;
	} وإلا إذا (logical == extent.e_lblk) {
#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("(re/un)mapping first block in extent\n");
#جيڪڏهن_پورو
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورو;
		retval = ext2fs_extent_fix_parents(handle);
		إذا (retval)
			اذهب پورو;
		إذا (physical) {
			إذا (has_prev &&
			    (logical == (prev_extent.e_lblk +
					 prev_extent.e_len)) &&
			    (physical == (prev_extent.e_pblk +
					  prev_extent.e_len)) &&
			    (new_uninit == prev_uninit) &&
			    ((صحيح) prev_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_PREV_پن, &prev_extent);
				إذا (retval)
					اذهب پورو;
				prev_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &prev_extent);
			} وإلا
				retval = ext2fs_extent_وجهو(handle,
							      0, &newextent);
			إذا (retval)
				اذهب پورو;
			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval)
				اذهب پورو;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ايندڙ_پن,
						   &extent);
			إذا (retval)
				اذهب پورو;
		}
	} وإلا {
		__u32	save_ڊيگهائي;
		blk64_t	save_lblk;
		بنية ext2fs_extent save_extent;
		errcode_t r2;

#جيڪڏهن_وصف_ٿيل DEBUG
		لکو("(re/un)mapping in middle of extent\n");
#جيڪڏهن_پورو
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_ڊيگهائي = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		إذا (retval)
			اذهب پورو;
		/* insert our new extent, if any */
		إذا (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_وجهو(handle,
					EXT2_EXTENT_وجهو_AFTER, &newextent);
			إذا (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				إذا (r2 == 0)
					(فراغ)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				اذهب پورو;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_ڊيگهائي - extent.e_len - 1;
		retval = ext2fs_extent_وجهو(handle,
				EXT2_EXTENT_وجهو_AFTER, &extent);
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
			اذهب پورو;
		}
	}

پورو:
	/* get handle back to its position */
	إذا (orig_اوچائي > handle->max_اونهائي)
		orig_اوچائي = handle->max_اونهائي; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_اوچائي, orig_lblk);
	أرجع retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحيح flags)
{
	بنية extent_رستو		*رستو;
	حرف 				*cp;
	بنية ext3_extent_سرو	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	إذا (!(handle->fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!handle->رستو)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

#جيڪڏهن_وصف_ٿيل DEBUG
	{
		بنية ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		إذا (retval == 0) {
			لکو("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#جيڪڏهن_پورو

	رستو = handle->رستو + handle->level;
	إذا (!رستو->curr)
		أرجع EXT2_ET_NO_CURRENT_ڳنڍ;

	cp = رستو->curr;

	/* Sanity check before memmove() */
	إذا (رستو->کاٻو < 0)
		أرجع EXT2_ET_EXTENT_پن_BAD;

	إذا (رستو->کاٻو) {
		memmove(cp, cp + حجم(بنية ext3_extent_idx),
			رستو->کاٻو * حجم(بنية ext3_extent_idx));
		رستو->کاٻو--;
	} وإلا {
		بنية ext3_extent_idx	*ix = رستو->curr;
		ix--;
		رستو->curr = ix;
	}
	إذا (--رستو->entries == 0)
		رستو->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	إذا (رستو->entries == 0 && handle->level) {
		إذا (!(flags & EXT2_EXTENT_DELETE_KEEP_خالي_جايون)) {
			بنية ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_مٿي,
								&extent);
			إذا (retval)
				أرجع retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_بلاڪون -=
				(handle->fs->blocksize *
				 EXT2_FS_CLUSTER_تناسب(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_بلاڪ_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} وإلا {
		eh = (بنية ext3_extent_سرو *) رستو->buf;
		eh->eh_entries = ext2fs_cpu_منزل_le16(رستو->entries);
		إذا ((رستو->entries == 0) && (handle->level == 0)) {
			eh->eh_اونهائي = 0;
			handle->max_اونهائي = 0;
		}
		retval = update_رستو(handle);
	}
	أرجع retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 بنية ext2_extent_info *info)
{
	بنية extent_رستو		*رستو;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, حجم(بنية ext2_extent_info));

	رستو = handle->رستو + handle->level;
	إذا (رستو) {
		إذا (رستو->curr)
			info->curr_داخلا = ((حرف *) رستو->curr - رستو->buf) /
				حجم(بنية ext3_extent_idx);
		وإلا
			info->curr_داخلا = 0;
		info->انگ_entries = رستو->entries;
		info->max_entries = رستو->max_entries;
		info->بائيٽون_avail = (رستو->max_entries - رستو->entries) *
			حجم(بنية ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_اونهائي = handle->max_اونهائي;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	أرجع 0;
}

size_t ext2fs_max_extent_اونهائي(ext2_extent_handle_t handle)
{
	size_t iblock_sz = حجم(((بنية ext2_inode *)عدم)->i_بلاڪ);
	size_t iblock_extents = (iblock_sz - حجم(بنية ext3_extent_سرو)) /
				حجم(بنية ext3_extent);
	size_t extents_في_بلاڪ = (handle->fs->blocksize -
				    حجم(بنية ext3_extent_سرو)) /
				   حجم(بنية ext3_extent);
	ساكن بنانشان صحيح آخري_blocksize = 0;
	ساكن size_t آخري_نتيجو = 0;

	إذا (آخري_blocksize && آخري_blocksize == handle->fs->blocksize)
		أرجع آخري_نتيجو;

	آخري_نتيجو = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_في_بلاڪ));
	آخري_blocksize = handle->fs->blocksize;
	أرجع آخري_نتيجو;
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
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_پاڙ, &extent);
	إذا (errcode)
		اذهب ٻاهريون;

	افعل {
		/* Skip to the end of a block of leaf nodes */
		إذا (extent.e_flags & EXT2_EXTENT_FLAGS_پن) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_آخري_SIB,
						    &extent);
			إذا (errcode)
				اكسر;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ايندڙ, &extent);
		إذا (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_رستو(handle);
	} طالما (errcode == 0);

ٻاهريون:
	/* Ok if we run off the end */
	إذا (errcode == EXT2_ET_EXTENT_NO_ايندڙ)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	أرجع errcode;
}

errcode_t ext2fs_decode_extent(بنية ext2fs_extent *منزل, فراغ *addr, صحيح len)
{
	بنية ext3_extent *from = (بنية ext3_extent *)addr;

	إذا (len != حجم(بنية ext3_extent))
		أرجع EXT2_ET_INVALID_دليل;

	منزل->e_pblk = ext2fs_le32_منزل_cpu(from->ee_شروعات) +
		((__u64) ext2fs_le16_منزل_cpu(from->ee_شروعات_hi)
			<< 32);
	منزل->e_lblk = ext2fs_le32_منزل_cpu(from->ee_بلاڪ);
	منزل->e_len = ext2fs_le16_منزل_cpu(from->ee_len);
	منزل->e_flags = EXT2_EXTENT_FLAGS_پن;
	إذا (منزل->e_len > EXT_INIT_MAX_LEN) {
		منزل->e_len -= EXT_INIT_MAX_LEN;
		منزل->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	أرجع 0;
}

errcode_t ext2fs_ڳڻپ_بلاڪون(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode, blk64_t *ret_ڳڻپ)
{
	ext2_extent_handle_t	handle = عدم;
	بنية ext2fs_extent	extent;
	errcode_t		errcode;
	صحيح			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	إذا (errcode)
		اذهب ٻاهريون;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_پاڙ, &extent);
	إذا (errcode)
		اذهب ٻاهريون;

	errcode = ext2fs_get_array(handle->max_اونهائي, حجم(blk64_t),
				   &intermediate_nodes);
	إذا (errcode)
		اذهب ٻاهريون;

	blkcount = handle->level;
	طالما (!errcode) {
		إذا (extent.e_flags & EXT2_EXTENT_FLAGS_پن) {
			blkcount += extent.e_len;
			لكل (i = 0; i < handle->level; i++) {
				إذا (intermediate_nodes[i] !=
					handle->رستو[i].پڇاڙي_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->رستو[i].پڇاڙي_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_ايندڙ, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
ٻاهريون:
	*ret_ڳڻپ = blkcount;
	ext2fs_extent_free(handle);

	أرجع 0;
}

#جيڪڏهن_وصف_ٿيل DEBUG
/*
 * Override debugfs's prompt
 */
ثابت حرف *debug_prog_name = "tst_extents";

#جيڪڏهن_پورو

