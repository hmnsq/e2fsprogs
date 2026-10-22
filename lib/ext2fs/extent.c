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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"
#شامل "e2image.h"

#تعریف_منسوخ DEBUG

/*
 * Definitions to be dropped in lib/ext2fs/ext2fs.h
 */

/*
 * Private definitions
 */

ساخت extent_راستہ {
	حرف		*buf;
	صحیح		entries;
	صحیح		max_entries;
	صحیح		بایاں;
	صحیح		دورہ_عدد;
	صحیح		flags;
	blk64_t		انتہا_blk;
	blk64_t		blk;
	خالی		*curr;
};


ساخت ext2_extent_handle {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t 		ino;
	ساخت ext2_inode	*inode;
	ساخت ext2_inode	inodebuf;
	صحیح			type;
	صحیح			level;
	صحیح			max_گہرائی;
	صحیح			max_paths;
	ساخت extent_راستہ	*راستہ;
};

ساخت ext2_extent_راستہ {
	errcode_t		magic;
	صحیح			برگ_اونچائی;
	blk64_t			lblk;
};

/*
 *  Useful Debugging stuff
 */

#اگر_تعریف_شدہ DEBUG
ساکن خالی dbg_show_سرخی(ساخت ext3_extent_سرخی *eh)
{
	لکھو("header: magic=%x entries=%u max=%u depth=%u generation=%u\n",
			ext2fs_le16_مقصد_cpu(eh->eh_magic),
			ext2fs_le16_مقصد_cpu(eh->eh_entries),
			ext2fs_le16_مقصد_cpu(eh->eh_max),
			ext2fs_le16_مقصد_cpu(eh->eh_گہرائی),
			ext2fs_le32_مقصد_cpu(eh->eh_generation));
}

ساکن خالی dbg_show_اشاریہ(ساخت ext3_extent_idx *ix)
{
	لکھو("index: block=%u leaf=%u leaf_hi=%u unused=%u\n",
			ext2fs_le32_مقصد_cpu(ix->ei_بلاک),
			ext2fs_le32_مقصد_cpu(ix->ei_برگ),
			ext2fs_le16_مقصد_cpu(ix->ei_برگ_hi),
			ext2fs_le16_مقصد_cpu(ix->ei_غیر_مستعمل));
}

ساکن خالی dbg_show_extent(ساخت ext3_extent *ex)
{
	لکھو("extent: block=%u-%u len=%u start=%u start_hi=%u\n",
			ext2fs_le32_مقصد_cpu(ex->ee_بلاک),
			ext2fs_le32_مقصد_cpu(ex->ee_بلاک) +
			ext2fs_le16_مقصد_cpu(ex->ee_len) - 1,
			ext2fs_le16_مقصد_cpu(ex->ee_len),
			ext2fs_le32_مقصد_cpu(ex->ee_آغاز),
			ext2fs_le16_مقصد_cpu(ex->ee_آغاز_hi));
}

ساکن خالی dbg_print_extent(حرف *desc, ساخت ext2fs_extent *extent)
{
	اگر (desc)
		لکھو("%s: ", desc);
	لکھو("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_برگ)
		fputs("LEAF ", stdout);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_دوسرا_دورہ)
		fputs("2ND_VISIT ", stdout);
	اگر (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}

ساکن خالی dump_راستہ(ثابت حرف *لیبل, ساخت ext2_extent_handle *handle,
		      ساخت extent_راستہ *راستہ)
{
	ساخت extent_راستہ *ppp = راستہ;
	لکھو("%s: level=%d\n", لیبل, handle->level);

	کرو {
		لکھو("%s: path=%ld buf=%p entries=%d max_entries=%d left=%d "
		       "visit_num=%d flags=0x%x end_blk=%llu curr=%p(%ld)\n",
		       لیبل, (ppp - handle->راستہ), ppp->buf, ppp->entries,
		       ppp->max_entries, ppp->بایاں, ppp->دورہ_عدد, ppp->flags,
		       ppp->انتہا_blk, ppp->curr, ppp->curr - (خالی *)ppp->buf);
		لکھو("  ");
		dbg_show_سرخی((ساخت ext3_extent_سرخی *)ppp->buf);
		اگر (ppp->curr) {
			لکھو("  ");
			dbg_show_اشاریہ(ppp->curr);
			لکھو("  ");
			dbg_show_extent(ppp->curr);
		}
		ppp--;
	} جب (ppp >= handle->راستہ);
	fflush(stdout);

	واپس;
}

#ورنہ
#تعریف_کرو dbg_show_سرخی(eh) کرو { } جب (0)
#تعریف_کرو dbg_show_اشاریہ(ix) کرو { } جب (0)
#تعریف_کرو dbg_show_extent(ex) کرو { } جب (0)
#تعریف_کرو dbg_print_extent(desc, ex) کرو { } جب (0)
#تعریف_کرو dump_راستہ(لیبل, handle, راستہ) کرو { } جب (0)
#اگر_ختم

/*
 * Verify the extent header as being sane
 */
errcode_t ext2fs_extent_سرخی_verify(خالی *ptr, صحیح سائز)
{
	صحیح eh_max, اندراج_سائز;
	ساخت ext3_extent_سرخی *eh = ptr;

	dbg_show_سرخی(eh);
	اگر (ext2fs_le16_مقصد_cpu(eh->eh_magic) != EXT3_EXT_MAGIC)
		واپس EXT2_ET_EXTENT_سرخی_BAD;
	اگر (ext2fs_le16_مقصد_cpu(eh->eh_entries) > ext2fs_le16_مقصد_cpu(eh->eh_max))
		واپس EXT2_ET_EXTENT_سرخی_BAD;
	اگر (eh->eh_گہرائی == 0)
		اندراج_سائز = حجم(ساخت ext3_extent);
	ورنہ
		اندراج_سائز = حجم(ساخت ext3_extent_idx);

	eh_max = (سائز - حجم(*eh)) / اندراج_سائز;
	/* Allow two extent-sized items at the end of the block, for
	 * ext4_extent_tail with checksum in the future. */
	اگر ((ext2fs_le16_مقصد_cpu(eh->eh_max) > eh_max) ||
	    (ext2fs_le16_مقصد_cpu(eh->eh_max) < (eh_max - 2)))
		واپس EXT2_ET_EXTENT_سرخی_BAD;

	واپس 0;
}


/*
 * Begin functions to handle an inode's extent information
 */
خالی ext2fs_extent_free(ext2_extent_handle_t handle)
{
	صحیح			i;

	اگر (!handle)
		واپس;

	اگر (handle->راستہ) {
		برائے (i = 1; i < handle->max_paths; i++) {
			اگر (handle->راستہ[i].buf)
				ext2fs_free_mem(&handle->راستہ[i].buf);
		}
		ext2fs_free_mem(&handle->راستہ);
	}
	ext2fs_free_mem(&handle);
}

errcode_t ext2fs_extent_open(ext2_filsys fs, ext2_ino_t ino,
				    ext2_extent_handle_t *ret_handle)
{
	واپس ext2fs_extent_open2(fs, ino, تہی, ret_handle);
}

errcode_t ext2fs_extent_open2(ext2_filsys fs, ext2_ino_t ino,
				    ساخت ext2_inode *inode,
				    ext2_extent_handle_t *ret_handle)
{
	ساخت ext2_extent_handle	*handle;
	errcode_t			retval;
	صحیح				i;
	ساخت ext3_extent_سرخی	*eh;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!inode)
		اگر ((ino == 0) || (ino > fs->super->s_inodes_گنتی))
			واپس EXT2_ET_BAD_INODE_عدد;

	retval = ext2fs_get_mem(حجم(ساخت ext2_extent_handle), &handle);
	اگر (retval)
		واپس retval;
	memset(handle, 0, حجم(ساخت ext2_extent_handle));

	handle->ino = ino;
	handle->fs = fs;

	اگر (inode) {
		handle->inode = inode;
	} ورنہ {
		handle->inode = &handle->inodebuf;
		retval = ext2fs_read_inode(fs, ino, handle->inode);
		اگر (retval)
			جاؤ errout;
	}

	eh = (ساخت ext3_extent_سرخی *) &handle->inode->i_بلاک[0];

	برائے (i=0; i < EXT2_N_بلاکیں; i++)
		اگر (handle->inode->i_بلاک[i])
			توڑو;
	اگر (i >= EXT2_N_بلاکیں) {
		eh->eh_magic = ext2fs_cpu_مقصد_le16(EXT3_EXT_MAGIC);
		eh->eh_گہرائی = 0;
		eh->eh_entries = 0;
		i = (حجم(handle->inode->i_بلاک) - حجم(*eh)) /
			حجم(ساخت ext3_extent);
		eh->eh_max = ext2fs_cpu_مقصد_le16(i);
		handle->inode->i_flags |= EXT4_EXTENTS_FL;
	}

	اگر (!(handle->inode->i_flags & EXT4_EXTENTS_FL)) {
		retval = EXT2_ET_INODE_NOT_EXTENT;
		جاؤ errout;
	}

	retval = ext2fs_extent_سرخی_verify(eh, حجم(handle->inode->i_بلاک));
	اگر (retval)
		جاؤ errout;

	handle->max_گہرائی = ext2fs_le16_مقصد_cpu(eh->eh_گہرائی);
	handle->type = ext2fs_le16_مقصد_cpu(eh->eh_magic);

	handle->max_paths = handle->max_گہرائی + 1;
	retval = ext2fs_get_memzero(handle->max_paths *
				    حجم(ساخت extent_راستہ),
				    &handle->راستہ);
	handle->راستہ[0].buf = (حرف *) handle->inode->i_بلاک;

	handle->راستہ[0].بایاں = handle->راستہ[0].entries =
		ext2fs_le16_مقصد_cpu(eh->eh_entries);
	handle->راستہ[0].max_entries = ext2fs_le16_مقصد_cpu(eh->eh_max);
	handle->راستہ[0].curr = 0;
	handle->راستہ[0].انتہا_blk =
		(EXT2_I_سائز(handle->inode) + fs->blocksize - 1) >>
		 EXT2_بلاک_سائز_بٹ(fs->super);
	handle->راستہ[0].blk = 0;
	handle->راستہ[0].دورہ_عدد = 1;
	handle->level = 0;
	handle->magic = EXT2_ET_MAGIC_EXTENT_HANDLE;

	*ret_handle = handle;
	واپس 0;

errout:
	ext2fs_extent_free(handle);
	واپس retval;
}

/*
 * This function is responsible for (optionally) moving through the
 * extent tree and then returning the current extent
 */
errcode_t ext2fs_extent_get(ext2_extent_handle_t handle,
			    صحیح flags, ساخت ext2fs_extent *extent)
{
	ساخت extent_راستہ	*راستہ, *newpath, *tp;
	ساخت ext3_extent_سرخی	*eh;
	ساخت ext3_extent_idx		*ix = 0;
	ساخت ext3_extent		*ex;
	errcode_t			retval;
	blk64_t				blk;
	blk64_t				انتہا_blk;
	صحیح				orig_op, op, l;
	صحیح				failed_csum = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!handle->راستہ)
		واپس EXT2_ET_NO_CURRENT_گرہ;

	orig_op = op = flags & EXT2_EXTENT_MOVE_نقاب;

retry:
	راستہ = handle->راستہ + handle->level;
	اگر ((orig_op == EXT2_EXTENT_اگلا) ||
	    (orig_op == EXT2_EXTENT_اگلا_برگ)) {
		اگر (handle->level < handle->max_گہرائی) {
			/* interior node */
			اگر (راستہ->دورہ_عدد == 0) {
				راستہ->دورہ_عدد++;
				op = EXT2_EXTENT_نیچے_کو;
			} ورنہ اگر (راستہ->بایاں > 0)
				op = EXT2_EXTENT_اگلا_SIB;
			ورنہ اگر (handle->level > 0)
				op = EXT2_EXTENT_اوپر;
			ورنہ
				واپس EXT2_ET_EXTENT_NO_اگلا;
		} ورنہ {
			/* leaf node */
			اگر (راستہ->بایاں > 0)
				op = EXT2_EXTENT_اگلا_SIB;
			ورنہ اگر (handle->level > 0)
				op = EXT2_EXTENT_اوپر;
			ورنہ
				واپس EXT2_ET_EXTENT_NO_اگلا;
		}
		اگر (op != EXT2_EXTENT_اگلا_SIB) {
#اگر_تعریف_شدہ DEBUG_GET_EXTENT
			لکھو("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_نیچے_کو) ? "down" :
			       ((op == EXT2_EXTENT_اوپر) ? "up" : "unknown"));
#اگر_ختم
		}
	}

	اگر ((orig_op == EXT2_EXTENT_پچھلا) ||
	    (orig_op == EXT2_EXTENT_پچھلا_برگ)) {
		اگر (handle->level < handle->max_گہرائی) {
			/* interior node */
			اگر (راستہ->دورہ_عدد > 0 ) {
				/* path->visit_num = 0; */
				op = EXT2_EXTENT_نیچے_کو_AND_آخری;
			} ورنہ اگر (راستہ->بایاں < راستہ->entries-1)
				op = EXT2_EXTENT_پچھلا_SIB;
			ورنہ اگر (handle->level > 0)
				op = EXT2_EXTENT_اوپر;
			ورنہ
				واپس EXT2_ET_EXTENT_NO_پچھلا;
		} ورنہ {
			/* leaf node */
			اگر (راستہ->بایاں < راستہ->entries-1)
				op = EXT2_EXTENT_پچھلا_SIB;
			ورنہ اگر (handle->level > 0)
				op = EXT2_EXTENT_اوپر;
			ورنہ
				واپس EXT2_ET_EXTENT_NO_پچھلا;
		}
		اگر (op != EXT2_EXTENT_پچھلا_SIB) {
#اگر_تعریف_شدہ DEBUG_GET_EXTENT
			لکھو("<<<< OP = %s\n",
			       (op == EXT2_EXTENT_نیچے_کو_AND_آخری) ? "down/last" :
			       ((op == EXT2_EXTENT_اوپر) ? "up" : "unknown"));
#اگر_ختم
		}
	}

	اگر (orig_op == EXT2_EXTENT_آخری_برگ) {
		اگر ((handle->level < handle->max_گہرائی) &&
		    (راستہ->بایاں == 0))
			op = EXT2_EXTENT_نیچے_کو;
		ورنہ
			op = EXT2_EXTENT_آخری_SIB;
#اگر_تعریف_شدہ DEBUG_GET_EXTENT
		لکھو("<<<< OP = %s\n",
			   (op == EXT2_EXTENT_نیچے_کو) ? "down" : "last_sib");
#اگر_ختم
	}

	انتخاب (op) {
	حالت EXT2_EXTENT_CURRENT:
		ix = راستہ->curr;
		توڑو;
	حالت EXT2_EXTENT_جڑ:
		handle->level = 0;
		راستہ = handle->راستہ + handle->level;
		/* fallthrough */
	حالت EXT2_EXTENT_پہلا_SIB:
		راستہ->بایاں = راستہ->entries;
		راستہ->curr = 0;
		/* fallthrough */
	حالت EXT2_EXTENT_اگلا_SIB:
		اگر (راستہ->بایاں <= 0)
			واپس EXT2_ET_EXTENT_NO_اگلا;
		اگر (راستہ->curr) {
			ix = راستہ->curr;
			ix++;
		} ورنہ {
			eh = (ساخت ext3_extent_سرخی *) راستہ->buf;
			ix = EXT_پہلا_اشاریہ(eh);
		}
		راستہ->بایاں--;
		راستہ->curr = ix;
		راستہ->دورہ_عدد = 0;
		توڑو;
	حالت EXT2_EXTENT_پچھلا_SIB:
		اگر (!راستہ->curr ||
		    راستہ->بایاں+1 >= راستہ->entries)
			واپس EXT2_ET_EXTENT_NO_پچھلا;
		ix = راستہ->curr;
		ix--;
		راستہ->curr = ix;
		راستہ->بایاں++;
		اگر (handle->level < handle->max_گہرائی)
			راستہ->دورہ_عدد = 1;
		توڑو;
	حالت EXT2_EXTENT_آخری_SIB:
		eh = (ساخت ext3_extent_سرخی *) راستہ->buf;
		راستہ->curr = EXT_آخری_EXTENT(eh);
		ix = راستہ->curr;
		راستہ->بایاں = 0;
		راستہ->دورہ_عدد = 0;
		توڑو;
	حالت EXT2_EXTENT_اوپر:
		اگر (handle->level <= 0)
			واپس EXT2_ET_EXTENT_NO_اوپر;
		handle->level--;
		راستہ--;
		ix = راستہ->curr;
		اگر ((orig_op == EXT2_EXTENT_پچھلا) ||
		    (orig_op == EXT2_EXTENT_پچھلا_برگ))
			راستہ->دورہ_عدد = 0;
		توڑو;
	حالت EXT2_EXTENT_نیچے_کو:
	حالت EXT2_EXTENT_نیچے_کو_AND_آخری:
		اگر (!راستہ->curr ||(handle->level >= handle->max_گہرائی))
			واپس EXT2_ET_EXTENT_NO_نیچے_کو;

		ix = راستہ->curr;
		newpath = راستہ + 1;
		اگر (!newpath->buf) {
			retval = ext2fs_get_mem(handle->fs->blocksize,
						&newpath->buf);
			اگر (retval)
				واپس retval;
		}
		blk = ext2fs_le32_مقصد_cpu(ix->ei_برگ) +
			((__u64) ext2fs_le16_مقصد_cpu(ix->ei_برگ_hi) << 32);
		برائے (l = handle->level, tp = راستہ; l > 0; l--, tp--) {
			اگر (blk == tp->blk)
				واپس EXT2_ET_EXTENT_چکر;
		}
		newpath->blk = blk;
		اگر ((handle->fs->flags & EXT2_FLAG_IMAGE_FILE) &&
		    (handle->fs->io != handle->fs->image_io))
			memset(newpath->buf, 0, handle->fs->blocksize);
		ورنہ {
			retval = io_channel_read_blk64(handle->fs->io,
						     blk, 1, newpath->buf);
			اگر (retval)
				واپس retval;
		}
		handle->level++;

		eh = (ساخت ext3_extent_سرخی *) newpath->buf;

		retval = ext2fs_extent_سرخی_verify(eh, handle->fs->blocksize);
		اگر (retval) {
			handle->level--;
			واپس retval;
		}

		اگر (!(handle->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_extent_بلاک_csum_verify(handle->fs, handle->ino,
						     eh))
			failed_csum = 1;

		newpath->بایاں = newpath->entries =
			ext2fs_le16_مقصد_cpu(eh->eh_entries);
		newpath->max_entries = ext2fs_le16_مقصد_cpu(eh->eh_max);

		/* Make sure there is at least one extent present */
		اگر (newpath->بایاں <= 0)
			واپس EXT2_ET_EXTENT_NO_نیچے_کو;

		اگر (راستہ->بایاں > 0) {
			ix++;
			newpath->انتہا_blk = ext2fs_le32_مقصد_cpu(ix->ei_بلاک);
		} ورنہ
			newpath->انتہا_blk = راستہ->انتہا_blk;

		راستہ = newpath;
		اگر (op == EXT2_EXTENT_نیچے_کو) {
			ix = EXT_پہلا_اشاریہ((ساخت ext3_extent_سرخی *) eh);
			راستہ->curr = ix;
			راستہ->بایاں = راستہ->entries - 1;
			راستہ->دورہ_عدد = 0;
		} ورنہ {
			ix = EXT_آخری_اشاریہ((ساخت ext3_extent_سرخی *) eh);
			راستہ->curr = ix;
			راستہ->بایاں = 0;
			اگر (handle->level < handle->max_گہرائی)
				راستہ->دورہ_عدد = 1;
		}
#اگر_تعریف_شدہ DEBUG_GET_EXTENT
		لکھو("Down to level %d/%d, end_blk=%llu\n",
			   handle->level, handle->max_گہرائی,
			   راستہ->انتہا_blk);
#اگر_ختم
		توڑو;
	طےشدہ:
		واپس EXT2_ET_OP_NOT_SUPPORTED;
	}

	اگر (!ix)
		واپس EXT2_ET_NO_CURRENT_گرہ;

	extent->e_flags = 0;
#اگر_تعریف_شدہ DEBUG_GET_EXTENT
	لکھو("(Left %d)\n", راستہ->بایاں);
#اگر_ختم

	اگر (handle->level == handle->max_گہرائی) {
		ex = (ساخت ext3_extent *) ix;

		extent->e_pblk = ext2fs_le32_مقصد_cpu(ex->ee_آغاز) +
			((__u64) ext2fs_le16_مقصد_cpu(ex->ee_آغاز_hi) << 32);
		extent->e_lblk = ext2fs_le32_مقصد_cpu(ex->ee_بلاک);
		extent->e_len = ext2fs_le16_مقصد_cpu(ex->ee_len);
		extent->e_flags |= EXT2_EXTENT_FLAGS_برگ;
		اگر (extent->e_len > EXT_INIT_MAX_LEN) {
			extent->e_len -= EXT_INIT_MAX_LEN;
			extent->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
		}
	} ورنہ {
		extent->e_pblk = ext2fs_le32_مقصد_cpu(ix->ei_برگ) +
			((__u64) ext2fs_le16_مقصد_cpu(ix->ei_برگ_hi) << 32);
		extent->e_lblk = ext2fs_le32_مقصد_cpu(ix->ei_بلاک);
		اگر (راستہ->بایاں > 0) {
			ix++;
			انتہا_blk = ext2fs_le32_مقصد_cpu(ix->ei_بلاک);
		} ورنہ
			انتہا_blk = راستہ->انتہا_blk;

		extent->e_len = انتہا_blk - extent->e_lblk;
	}
	اگر (راستہ->دورہ_عدد)
		extent->e_flags |= EXT2_EXTENT_FLAGS_دوسرا_دورہ;

	اگر (((orig_op == EXT2_EXTENT_اگلا_برگ) ||
	     (orig_op == EXT2_EXTENT_پچھلا_برگ)) &&
	    (handle->level != handle->max_گہرائی))
		جاؤ retry;

	اگر ((orig_op == EXT2_EXTENT_آخری_برگ) &&
	    ((handle->level != handle->max_گہرائی) ||
	     (راستہ->بایاں != 0)))
		جاؤ retry;

	اگر (failed_csum)
		واپس EXT2_ET_EXTENT_CSUM_INVALID;

	واپس 0;
}

ساکن errcode_t update_راستہ(ext2_extent_handle_t handle)
{
	blk64_t				blk;
	errcode_t			retval;
	ساخت ext3_extent_idx		*ix;
	ساخت ext3_extent_سرخی	*eh;

	اگر (handle->level == 0) {
		retval = ext2fs_write_inode(handle->fs, handle->ino,
					    handle->inode);
	} ورنہ {
		ix = handle->راستہ[handle->level - 1].curr;
		blk = ext2fs_le32_مقصد_cpu(ix->ei_برگ) +
			((__u64) ext2fs_le16_مقصد_cpu(ix->ei_برگ_hi) << 32);

		/* then update the checksum */
		eh = (ساخت ext3_extent_سرخی *)
				handle->راستہ[handle->level].buf;
		retval = ext2fs_extent_بلاک_csum_سیٹ(handle->fs, handle->ino,
						      eh);
		اگر (retval)
			واپس retval;

		retval = io_channel_write_blk64(handle->fs->io,
				      blk, 1, handle->راستہ[handle->level].buf);
	}
	واپس retval;
}

#اگر 0
errcode_t ext2fs_extent_save_راستہ(ext2_extent_handle_t handle,
				  ext2_extent_راستہ_t *ret_راستہ)
{
	ext2_extent_راستہ_t	save_راستہ;
	ساخت ext2fs_extent	extent;
	ساخت ext2_extent_info	info;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	اگر (retval)
		واپس retval;

	retval = ext2fs_extent_get_info(handle, &info);
	اگر (retval)
		واپس retval;

	retval = ext2fs_get_mem(حجم(ساخت ext2_extent_راستہ), &save_راستہ);
	اگر (retval)
		واپس retval;
	memset(save_راستہ, 0, حجم(ساخت ext2_extent_راستہ));

	save_راستہ->magic = EXT2_ET_MAGIC_EXTENT_راستہ;
	save_راستہ->برگ_اونچائی = info.max_گہرائی - info.curr_level - 1;
	save_راستہ->lblk = extent.e_lblk;

	*ret_راستہ = save_راستہ;
	واپس 0;
}

errcode_t ext2fs_extent_free_راستہ(ext2_extent_راستہ_t راستہ)
{
	EXT2_CHECK_MAGIC(راستہ, EXT2_ET_MAGIC_EXTENT_راستہ);

	ext2fs_free_mem(&راستہ);
	واپس 0;
}
#اگر_ختم

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
			      صحیح برگ_level, blk64_t blk)
{
	ساخت ext2fs_extent	extent;
	errcode_t		retval;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_جڑ, &extent);
	اگر (retval) {
		اگر (retval == EXT2_ET_EXTENT_NO_اگلا)
			retval = EXT2_ET_EXTENT_NOT_ملا;
		واپس retval;
	}

	اگر (برگ_level > handle->max_گہرائی) {
#اگر_تعریف_شدہ DEBUG
		لکھو("leaf level %d greater than tree depth %d\n",
			برگ_level, handle->max_گہرائی);
#اگر_ختم
		واپس EXT2_ET_OP_NOT_SUPPORTED;
	}

#اگر_تعریف_شدہ DEBUG
	لکھو("goto extent ino %u, level %d, %llu\n", handle->ino,
	       برگ_level, blk);
#اگر_ختم

#اگر_تعریف_شدہ DEBUG_GOTO_EXTENTS
	dbg_print_extent("root", &extent);
#اگر_ختم
	جب (1) {
		اگر (handle->max_گہرائی - handle->level == برگ_level) {
			/* block is in this &extent */
			اگر ((blk >= extent.e_lblk) &&
			    (blk < extent.e_lblk + extent.e_len))
				واپس 0;
			اگر (blk < extent.e_lblk) {
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_پچھلا_SIB,
							   &extent);
				واپس EXT2_ET_EXTENT_NOT_ملا;
			}
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_اگلا_SIB,
						   &extent);
			اگر (retval == EXT2_ET_EXTENT_NO_اگلا)
				واپس EXT2_ET_EXTENT_NOT_ملا;
			اگر (retval)
				واپس retval;
			جاری;
		}

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_اگلا_SIB,
					   &extent);
		اگر (retval == EXT2_ET_EXTENT_NO_اگلا)
			جاؤ go_نیچے_کو;
		اگر (retval)
			واپس retval;

#اگر_تعریف_شدہ DEBUG_GOTO_EXTENTS
		dbg_print_extent("next", &extent);
#اگر_ختم
		اگر (blk == extent.e_lblk)
			جاؤ go_نیچے_کو;
		اگر (blk > extent.e_lblk)
			جاری;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_پچھلا_SIB,
					   &extent);
		اگر (retval)
			واپس retval;

#اگر_تعریف_شدہ DEBUG_GOTO_EXTENTS
		dbg_print_extent("prev", &extent);
#اگر_ختم

	go_نیچے_کو:
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_نیچے_کو,
					   &extent);
		اگر (retval)
			واپس retval;

#اگر_تعریف_شدہ DEBUG_GOTO_EXTENTS
		dbg_print_extent("down", &extent);
#اگر_ختم
	}
}

errcode_t ext2fs_extent_goto(ext2_extent_handle_t handle,
			     blk64_t blk)
{
	واپس ext2fs_extent_goto2(handle, 0, blk);
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
	صحیح				retval = 0;
	صحیح				orig_اونچائی;
	blk64_t				آغاز;
	ساخت extent_راستہ		*راستہ;
	ساخت ext2fs_extent		extent;
	ساخت ext2_extent_info		info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	اگر (!handle->راستہ)
		واپس EXT2_ET_NO_CURRENT_گرہ;

	راستہ = handle->راستہ + handle->level;
	اگر (!راستہ->curr)
		واپس EXT2_ET_NO_CURRENT_گرہ;

	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	اگر (retval)
		جاؤ مکمل;

	/* modified node's start block */
	آغاز = extent.e_lblk;

	اگر ((retval = ext2fs_extent_get_info(handle, &info)))
		واپس retval;
	orig_اونچائی = info.max_گہرائی - info.curr_level;

	/* traverse up until index not first, or startblk matches, or top */
	جب (handle->level > 0 &&
	       (راستہ->بایاں == راستہ->entries - 1)) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_اوپر, &extent);
		اگر (retval)
			جاؤ مکمل;
		اگر (extent.e_lblk == آغاز)
			توڑو;
		راستہ = handle->راستہ + handle->level;
		extent.e_len += (extent.e_lblk - آغاز);
		extent.e_lblk = آغاز;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			جاؤ مکمل;
		update_راستہ(handle);
	}

	/* put handle back to where we started */
	retval = ext2fs_extent_goto2(handle, orig_اونچائی, آغاز);
مکمل:
	واپس retval;
}

errcode_t ext2fs_extent_replace(ext2_extent_handle_t handle,
				صحیح flags EXT2FS_ATTR((غیر_مستعمل)),
				ساخت ext2fs_extent *extent)
{
	ساخت extent_راستہ		*راستہ;
	ساخت ext3_extent_idx		*ix;
	ساخت ext3_extent		*ex;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	اگر (!handle->راستہ)
		واپس EXT2_ET_NO_CURRENT_گرہ;

	راستہ = handle->راستہ + handle->level;
	اگر (!راستہ->curr)
		واپس EXT2_ET_NO_CURRENT_گرہ;

#اگر_تعریف_شدہ DEBUG
	لکھو("extent replace: %u ", handle->ino);
	dbg_print_extent(0, extent);
#اگر_ختم

	اگر (handle->level == handle->max_گہرائی) {
		ex = راستہ->curr;

		ex->ee_بلاک = ext2fs_cpu_مقصد_le32(extent->e_lblk);
		ex->ee_آغاز = ext2fs_cpu_مقصد_le32(extent->e_pblk & 0xFFFFFFFF);
		ex->ee_آغاز_hi = ext2fs_cpu_مقصد_le16(extent->e_pblk >> 32);
		اگر (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT) {
			اگر (extent->e_len > EXT_UNINIT_MAX_LEN)
				واپس EXT2_ET_EXTENT_INVALID_لمبائی;
			ex->ee_len = ext2fs_cpu_مقصد_le16(extent->e_len +
							EXT_INIT_MAX_LEN);
		} ورنہ {
			اگر (extent->e_len > EXT_INIT_MAX_LEN)
				واپس EXT2_ET_EXTENT_INVALID_لمبائی;
			ex->ee_len = ext2fs_cpu_مقصد_le16(extent->e_len);
		}
	} ورنہ {
		ix = راستہ->curr;

		ix->ei_برگ = ext2fs_cpu_مقصد_le32(extent->e_pblk & 0xFFFFFFFF);
		ix->ei_برگ_hi = ext2fs_cpu_مقصد_le16(extent->e_pblk >> 32);
		ix->ei_بلاک = ext2fs_cpu_مقصد_le32(extent->e_lblk);
		ix->ei_غیر_مستعمل = 0;
	}
	update_راستہ(handle);
	واپس 0;
}

ساکن صحیح splitting_at_eof(ساخت ext2_extent_handle *handle,
			    ساخت extent_راستہ *راستہ)
{
	ساخت extent_راستہ *ppp = راستہ;
	dump_راستہ(__func__, handle, راستہ);

	اگر (handle->level == 0)
		واپس 0;

	کرو {
		اگر (ppp->بایاں)
			واپس 0;
		ppp--;
	} جب (ppp >= handle->راستہ);

	واپس 1;
}

/*
 * allocate a new block, move half the current node to it, and update parent
 *
 * handle will be left pointing at original record.
 */
ساکن errcode_t extent_گرہ_بانٹو(ext2_extent_handle_t handle,
				   صحیح expand_allowed)
{
	errcode_t			retval = 0;
	blk64_t				new_گرہ_pblk;
	blk64_t				new_گرہ_آغاز;
	blk64_t				orig_lblk;
	blk64_t				goal_blk = 0;
	صحیح				orig_اونچائی;
	حرف				*بلاک_buf = تہی;
	ساخت ext2fs_extent		extent;
	ساخت extent_راستہ		*راستہ, *newpath = 0;
	ساخت ext3_extent_سرخی	*eh, *neweh;
	صحیح				tocopy;
	صحیح				new_جڑ = 0;
	ساخت ext2_extent_info		info;
	صحیح				no_balance;

	/* basic sanity */
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	اگر (!handle->راستہ)
		واپس EXT2_ET_NO_CURRENT_گرہ;

#اگر_تعریف_شدہ DEBUG
	لکھو("splitting node at level %d\n", handle->level);
#اگر_ختم
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	اگر (retval)
		جاؤ مکمل;

	retval = ext2fs_extent_get_info(handle, &info);
	اگر (retval)
		جاؤ مکمل;

	/* save the position we were originally splitting... */
	orig_اونچائی = info.max_گہرائی - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* Try to put the index block before the first extent */
	راستہ = handle->راستہ + handle->level;
	eh = (ساخت ext3_extent_سرخی *) راستہ->buf;
	اگر (handle->level == handle->max_گہرائی) {
		ساخت ext3_extent	*ex;

		ex = EXT_پہلا_EXTENT(eh);
		goal_blk = ext2fs_le32_مقصد_cpu(ex->ee_آغاز) +
			((__u64) ext2fs_le16_مقصد_cpu(ex->ee_آغاز_hi) << 32);
	} ورنہ {
		ساخت ext3_extent_idx	*ix;

		ix = EXT_پہلا_اشاریہ(eh);
		goal_blk = ext2fs_le32_مقصد_cpu(ix->ei_برگ) +
			((__u64) ext2fs_le16_مقصد_cpu(ix->ei_برگ_hi) << 32);
	}
	goal_blk -= EXT2_FS_CLUSTER_تناسب(handle->fs);
	goal_blk &= ~EXT2_FS_CLUSTER_نقاب(handle->fs);

	/* Is there room in the parent for a new entry? */
	اگر (handle->level &&
			(handle->راستہ[handle->level - 1].entries >=
			 handle->راستہ[handle->level - 1].max_entries)) {

#اگر_تعریف_شدہ DEBUG
		لکھو("parent level %d full; splitting it too\n",
							handle->level - 1);
#اگر_ختم
		/* split the parent */
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_اوپر, &extent);
		اگر (retval)
			جاؤ مکمل;

		retval = extent_گرہ_بانٹو(handle, expand_allowed);
		اگر (retval)
			جاؤ مکمل;

		/* get handle back to our original split position */
		retval = ext2fs_extent_goto2(handle, orig_اونچائی, orig_lblk);
		اگر (retval)
			جاؤ مکمل;
	}

	/* At this point, parent should have room for this split */
	راستہ = handle->راستہ + handle->level;
	اگر (!راستہ->curr)
		واپس EXT2_ET_NO_CURRENT_گرہ;

	/*
	 * Normally, we try to split a full node in half.  This doesn't turn
	 * out so well if we're tacking extents on the end of the file because
	 * then we're stuck with a tree of half-full extent blocks.  This of
	 * course doesn't apply to the root level.
	 */
	no_balance = expand_allowed ? splitting_at_eof(handle, راستہ) : 0;

	/* extent header of the current node we'll split */
	eh = (ساخت ext3_extent_سرخی *)راستہ->buf;

	/* splitting root level means moving them all out */
	اگر (handle->level == 0) {
		new_جڑ = 1;
		tocopy = ext2fs_le16_مقصد_cpu(eh->eh_entries);
		retval = ext2fs_get_memzero((handle->max_paths + 1) *
					    حجم(ساخت extent_راستہ),
					    &newpath);
		اگر (retval)
			جاؤ مکمل;
	} ورنہ {
		اگر (no_balance)
			tocopy = 1;
		ورنہ
			tocopy = ext2fs_le16_مقصد_cpu(eh->eh_entries) / 2;
	}

#اگر_تعریف_شدہ DEBUG
	لکھو("will copy out %d of %d entries at level %d\n",
				tocopy, ext2fs_le16_مقصد_cpu(eh->eh_entries),
				handle->level);
#اگر_ختم

	اگر (!tocopy && !no_balance) {
#اگر_تعریف_شدہ DEBUG
		لکھو("Nothing to copy to new block!\n");
#اگر_ختم
		retval = EXT2_ET_CANT_بانٹو_EXTENT;
		جاؤ مکمل;
	}

	/* first we need a new block, or can do nothing. */
	بلاک_buf = حافظہ_دو(handle->fs->blocksize);
	اگر (!بلاک_buf) {
		retval = ENOMEM;
		جاؤ مکمل;
	}

	اگر (!goal_blk)
		goal_blk = ext2fs_ڈھونڈو_inode_goal(handle->fs, handle->ino,
						  handle->inode, 0);
	retval = ext2fs_alloc_block2(handle->fs, goal_blk, بلاک_buf,
				    &new_گرہ_pblk);
	اگر (retval)
		جاؤ مکمل;

#اگر_تعریف_شدہ DEBUG
	لکھو("will copy to new node at block %lu\n",
	       (غیرعلامتی طویل) new_گرہ_pblk);
#اگر_ختم

	/* Copy data into new block buffer */
	/* First the header for the new block... */
	neweh = (ساخت ext3_extent_سرخی *) بلاک_buf;
	حافظہ_نقل(neweh, eh, حجم(ساخت ext3_extent_سرخی));
	neweh->eh_entries = ext2fs_cpu_مقصد_le16(tocopy);
	neweh->eh_max = ext2fs_cpu_مقصد_le16((handle->fs->blocksize -
			 حجم(ساخت ext3_extent_سرخی)) /
				حجم(ساخت ext3_extent));

	/* then the entries for the new block... */
	حافظہ_نقل(EXT_پہلا_اشاریہ(neweh),
		EXT_پہلا_اشاریہ(eh) +
			(ext2fs_le16_مقصد_cpu(eh->eh_entries) - tocopy),
		حجم(ساخت ext3_extent_idx) * tocopy);

	new_گرہ_آغاز = ext2fs_le32_مقصد_cpu(EXT_پہلا_اشاریہ(neweh)->ei_بلاک);

	/* then update the checksum */
	retval = ext2fs_extent_بلاک_csum_سیٹ(handle->fs, handle->ino, neweh);
	اگر (retval)
		جاؤ مکمل;

	/* ...and write the new node block out to disk. */
	retval = io_channel_write_blk64(handle->fs->io, new_گرہ_pblk, 1,
					بلاک_buf);

	اگر (retval)
		جاؤ مکمل;

	/* OK! we've created the new node; now adjust the tree */

	/* current path now has fewer active entries, we copied some out */
	اگر (handle->level == 0) {
		حافظہ_نقل(newpath, راستہ,
		       حجم(ساخت extent_راستہ) * handle->max_paths);
		handle->راستہ = newpath;
		newpath = راستہ;
		راستہ = handle->راستہ;
		راستہ->entries = 1;
		راستہ->بایاں = راستہ->max_entries - 1;
		handle->max_گہرائی++;
		handle->max_paths++;
		eh->eh_گہرائی = ext2fs_cpu_مقصد_le16(handle->max_گہرائی);
	} ورنہ {
		راستہ->entries -= tocopy;
		راستہ->بایاں -= tocopy;
	}

	eh->eh_entries = ext2fs_cpu_مقصد_le16(راستہ->entries);
	/* this writes out the node, incl. the modified header */
	retval = update_راستہ(handle);
	اگر (retval)
		جاؤ مکمل;

	/* now go up and insert/replace index for new node we created */
	اگر (new_جڑ) {
		retval = ext2fs_extent_get(handle, EXT2_EXTENT_پہلا_SIB, &extent);
		اگر (retval)
			جاؤ مکمل;

		extent.e_lblk = new_گرہ_آغاز;
		extent.e_pblk = new_گرہ_pblk;
		extent.e_len = handle->راستہ[0].انتہا_blk - extent.e_lblk;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			جاؤ مکمل;
	} ورنہ {
		__u32 new_گرہ_لمبائی;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_اوپر, &extent);
		/* will insert after this one; it's length is shorter now */
		new_گرہ_لمبائی = new_گرہ_آغاز - extent.e_lblk;
		extent.e_len -= new_گرہ_لمبائی;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			جاؤ مکمل;

		/* now set up the new extent and insert it */
		extent.e_lblk = new_گرہ_آغاز;
		extent.e_pblk = new_گرہ_pblk;
		extent.e_len = new_گرہ_لمبائی;
		retval = ext2fs_extent_داخل_کرو(handle, EXT2_EXTENT_داخل_کرو_AFTER, &extent);
		اگر (retval)
			جاؤ مکمل;
	}

	/* get handle back to our original position */
	retval = ext2fs_extent_goto2(handle, orig_اونچائی, orig_lblk);
	اگر (retval)
		جاؤ مکمل;

	/* new node hooked in, so update inode block count (do this here?) */
	ext2fs_iblk_جوڑو_بلاکیں(handle->fs, handle->inode, 1);
	retval = ext2fs_write_inode(handle->fs, handle->ino,
				    handle->inode);
	اگر (retval)
		جاؤ مکمل;

مکمل:
	اگر (newpath)
		ext2fs_free_mem(&newpath);
	آزاد(بلاک_buf);

	واپس retval;
}

errcode_t ext2fs_extent_گرہ_بانٹو(ext2_extent_handle_t handle)
{
	واپس extent_گرہ_بانٹو(handle, 0);
}

errcode_t ext2fs_extent_داخل_کرو(ext2_extent_handle_t handle, صحیح flags,
				      ساخت ext2fs_extent *extent)
{
	ساخت extent_راستہ		*راستہ;
	ساخت ext3_extent_idx		*ix;
	ساخت ext3_extent_سرخی	*eh;
	errcode_t			retval;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	اگر (!handle->راستہ)
		واپس EXT2_ET_NO_CURRENT_گرہ;

#اگر_تعریف_شدہ DEBUG
	لکھو("extent insert: %u ", handle->ino);
	dbg_print_extent(0, extent);
#اگر_ختم

	راستہ = handle->راستہ + handle->level;

	اگر (راستہ->entries >= راستہ->max_entries) {
		اگر (flags & EXT2_EXTENT_داخل_کرو_NOSPLIT) {
			واپس EXT2_ET_CANT_داخل_کرو_EXTENT;
		} ورنہ {
#اگر_تعریف_شدہ DEBUG
			لکھو("node full (level %d) - splitting\n",
				   handle->level);
#اگر_ختم
			retval = extent_گرہ_بانٹو(handle, 1);
			اگر (retval)
				واپس retval;
			راستہ = handle->راستہ + handle->level;
		}
	}

	eh = (ساخت ext3_extent_سرخی *) راستہ->buf;
	اگر (راستہ->curr) {
		ix = راستہ->curr;
		اگر (flags & EXT2_EXTENT_داخل_کرو_AFTER) {
			ix++;
			راستہ->بایاں--;
		}
	} ورنہ {
		ix = EXT_پہلا_اشاریہ(eh);
		راستہ->بایاں = -1;
	}

	راستہ->curr = ix;

	اگر (راستہ->بایاں >= 0)
		memmove(ix + 1, ix,
			(راستہ->بایاں+1) * حجم(ساخت ext3_extent_idx));
	راستہ->بایاں++;
	راستہ->entries++;

	eh = (ساخت ext3_extent_سرخی *) راستہ->buf;
	eh->eh_entries = ext2fs_cpu_مقصد_le16(راستہ->entries);

	retval = ext2fs_extent_replace(handle, 0, extent);
	اگر (retval)
		جاؤ errout;

	retval = update_راستہ(handle);
	اگر (retval)
		جاؤ errout;

	واپس 0;

errout:
	ext2fs_extent_delete(handle, 0);
	واپس retval;
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
				 blk64_t logical, blk64_t physical, صحیح flags)
{
	errcode_t		ec, retval = 0;
	صحیح			mapped = 1; /* logical is mapped? */
	صحیح			orig_اونچائی;
	صحیح			extent_uninit = 0;
	صحیح			پچھلا_uninit = 0;
	صحیح			اگلا_uninit = 0;
	صحیح			new_uninit = 0;
	صحیح			max_len = EXT_INIT_MAX_LEN;
	صحیح			has_پچھلا, has_اگلا;
	blk64_t			orig_lblk;
	ساخت extent_راستہ	*راستہ;
	ساخت ext2fs_extent	extent, اگلا_extent, پچھلا_extent;
	ساخت ext2fs_extent	newextent;
	ساخت ext2_extent_info	info;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

#اگر_تعریف_شدہ DEBUG
	لکھو("set_bmap ino %u log %lld phys %lld flags %d\n",
	       handle->ino, logical, physical, flags);
#اگر_ختم

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	اگر (!handle->راستہ)
		واپس EXT2_ET_NO_CURRENT_گرہ;

	راستہ = handle->راستہ + handle->level;

	اگر (flags & EXT2_EXTENT_سیٹ_BMAP_UNINIT) {
		new_uninit = 1;
		max_len = EXT_UNINIT_MAX_LEN;
	}

	/* if (re)mapping, set up new extent to insert */
	اگر (physical) {
		newextent.e_len = 1;
		newextent.e_pblk = physical;
		newextent.e_lblk = logical;
		newextent.e_flags = EXT2_EXTENT_FLAGS_برگ;
		اگر (new_uninit)
			newextent.e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	/* special case if the extent tree is completely empty */
	اگر ((handle->max_گہرائی == 0) && (راستہ->entries == 0)) {
		retval = ext2fs_extent_داخل_کرو(handle, 0, &newextent);
		واپس retval;
	}

	/* save our original location in the extent tree */
	اگر ((retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&extent))) {
		اگر (retval != EXT2_ET_NO_CURRENT_گرہ)
			واپس retval;
		memset(&extent, 0, حجم(extent));
	}
	اگر ((retval = ext2fs_extent_get_info(handle, &info)))
		واپس retval;
	orig_اونچائی = info.max_گہرائی - info.curr_level;
	orig_lblk = extent.e_lblk;

	/* go to the logical spot we want to (re/un)map */
	retval = ext2fs_extent_goto(handle, logical);
	اگر (retval) {
		اگر (retval == EXT2_ET_EXTENT_NOT_ملا) {
			retval = 0;
			mapped = 0;
			اگر (!physical) {
#اگر_تعریف_شدہ DEBUG
				لکھو("block %llu already unmapped\n",
					logical);
#اگر_ختم
				جاؤ مکمل;
			}
		} ورنہ
			جاؤ مکمل;
	}

	/*
	 * This may be the extent *before* the requested logical,
	 * if it's currently unmapped.
	 *
	 * Get the previous and next leaf extents, if they are present.
	 */
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	اگر (retval)
		جاؤ مکمل;
	اگر (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		extent_uninit = 1;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_اگلا_برگ, &اگلا_extent);
	اگر (retval) {
		has_اگلا = 0;
		اگر (retval != EXT2_ET_EXTENT_NO_اگلا)
			جاؤ مکمل;
	} ورنہ {
		dbg_print_extent("set_bmap: next_extent",
				 &اگلا_extent);
		has_اگلا = 1;
		اگر (اگلا_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			اگلا_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	اگر (retval && retval != EXT2_ET_EXTENT_NOT_ملا)
		جاؤ مکمل;
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_پچھلا_برگ, &پچھلا_extent);
	اگر (retval) {
		has_پچھلا = 0;
		اگر (retval != EXT2_ET_EXTENT_NO_پچھلا)
			جاؤ مکمل;
	} ورنہ {
		has_پچھلا = 1;
		dbg_print_extent("set_bmap: prev_extent",
				 &پچھلا_extent);
		اگر (پچھلا_extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			پچھلا_uninit = 1;
	}
	retval = ext2fs_extent_goto(handle, logical);
	اگر (retval && retval != EXT2_ET_EXTENT_NOT_ملا)
		جاؤ مکمل;

	/* check if already pointing to the requested physical */
	اگر (mapped && (new_uninit == extent_uninit) &&
	    (extent.e_pblk + (logical - extent.e_lblk) == physical)) {
#اگر_تعریف_شدہ DEBUG
		لکھو("physical block (at %llu) unchanged\n", logical);
#اگر_ختم
		جاؤ مکمل;
	}

	اگر (!mapped) {
#اگر_تعریف_شدہ DEBUG
		لکھو("mapping unmapped logical block %llu\n", logical);
#اگر_ختم
		اگر ((logical == extent.e_lblk + extent.e_len) &&
		    (physical == extent.e_pblk + extent.e_len) &&
		    (new_uninit == extent_uninit) &&
		    ((صحیح) extent.e_len < max_len-1)) {
			extent.e_len++;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ورنہ اگر ((logical == extent.e_lblk - 1) &&
			   (physical == extent.e_pblk - 1) &&
			   (new_uninit == extent_uninit) &&
			   ((صحیح) extent.e_len < max_len - 1)) {
			extent.e_len++;
			extent.e_lblk--;
			extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &extent);
		} ورنہ اگر (has_اگلا &&
			   (logical == اگلا_extent.e_lblk - 1) &&
			   (physical == اگلا_extent.e_pblk - 1) &&
			   (new_uninit == اگلا_uninit) &&
			   ((صحیح) اگلا_extent.e_len < max_len - 1)) {
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_اگلا_برگ,
						   &اگلا_extent);
			اگر (retval)
				جاؤ مکمل;
			اگلا_extent.e_len++;
			اگلا_extent.e_lblk--;
			اگلا_extent.e_pblk--;
			retval = ext2fs_extent_replace(handle, 0, &اگلا_extent);
		} ورنہ اگر (logical < extent.e_lblk)
			retval = ext2fs_extent_داخل_کرو(handle, 0, &newextent);
		ورنہ
			retval = ext2fs_extent_داخل_کرو(handle,
				      EXT2_EXTENT_داخل_کرو_AFTER, &newextent);
		اگر (retval)
			جاؤ مکمل;
		retval = ext2fs_extent_fix_parents(handle);
		اگر (retval)
			جاؤ مکمل;
	} ورنہ اگر ((logical == extent.e_lblk) && (extent.e_len == 1))  {
#اگر_تعریف_شدہ DEBUG
		لکھو("(re/un)mapping only block in extent\n");
#اگر_ختم
		اگر (physical) {
			اگر (has_پچھلا &&
			    (logical == (پچھلا_extent.e_lblk +
					 پچھلا_extent.e_len)) &&
			    (physical == (پچھلا_extent.e_pblk +
					  پچھلا_extent.e_len)) &&
			    (new_uninit == پچھلا_uninit) &&
			    ((صحیح) پچھلا_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_پچھلا_برگ, &پچھلا_extent);
				اگر (retval)
					جاؤ مکمل;
				پچھلا_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &پچھلا_extent);
				retval = ext2fs_extent_get(handle,
							   EXT2_EXTENT_اگلا_برگ,
							   &extent);
				اگر (retval)
					جاؤ مکمل;
				جاؤ delete_گرہ;

			} ورنہ
				retval = ext2fs_extent_replace(handle, 0, &newextent);
		} ورنہ {
		delete_گرہ:
			retval = ext2fs_extent_delete(handle, 0);
			اگر (retval)
				جاؤ مکمل;
			ec = ext2fs_extent_fix_parents(handle);
			اگر (ec != EXT2_ET_NO_CURRENT_گرہ)
				retval = ec;
		}

		اگر (retval)
			جاؤ مکمل;
	} ورنہ اگر (logical == extent.e_lblk + extent.e_len - 1)  {
#اگر_تعریف_شدہ DEBUG
		لکھو("(re/un)mapping last block in extent\n");
#اگر_ختم
		اگر (physical) {
			اگر (has_اگلا &&
			    (logical == (اگلا_extent.e_lblk - 1)) &&
			    (physical == (اگلا_extent.e_pblk - 1)) &&
			    (new_uninit == اگلا_uninit) &&
			    ((صحیح) اگلا_extent.e_len < max_len - 1)) {
				retval = ext2fs_extent_get(handle,
					EXT2_EXTENT_اگلا_برگ, &اگلا_extent);
				اگر (retval)
					جاؤ مکمل;
				اگلا_extent.e_len++;
				اگلا_extent.e_lblk--;
				اگلا_extent.e_pblk--;
				retval = ext2fs_extent_replace(handle, 0,
							       &اگلا_extent);
				اگر (retval)
					جاؤ مکمل;
			} ورنہ
				retval = ext2fs_extent_داخل_کرو(handle,
				      EXT2_EXTENT_داخل_کرو_AFTER, &newextent);
			اگر (retval)
				جاؤ مکمل;
			retval = ext2fs_extent_fix_parents(handle);
			اگر (retval)
				جاؤ مکمل;
			/*
			 * Now pointing at inserted extent; move back to prev.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, logical);
			اگر (retval)
				جاؤ مکمل;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_CURRENT,
						   &extent);
			اگر (retval)
				جاؤ مکمل;
		}
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			جاؤ مکمل;
	} ورنہ اگر (logical == extent.e_lblk) {
#اگر_تعریف_شدہ DEBUG
		لکھو("(re/un)mapping first block in extent\n");
#اگر_ختم
		extent.e_pblk++;
		extent.e_lblk++;
		extent.e_len--;
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			جاؤ مکمل;
		retval = ext2fs_extent_fix_parents(handle);
		اگر (retval)
			جاؤ مکمل;
		اگر (physical) {
			اگر (has_پچھلا &&
			    (logical == (پچھلا_extent.e_lblk +
					 پچھلا_extent.e_len)) &&
			    (physical == (پچھلا_extent.e_pblk +
					  پچھلا_extent.e_len)) &&
			    (new_uninit == پچھلا_uninit) &&
			    ((صحیح) پچھلا_extent.e_len < max_len-1)) {
				retval = ext2fs_extent_get(handle, 
					EXT2_EXTENT_پچھلا_برگ, &پچھلا_extent);
				اگر (retval)
					جاؤ مکمل;
				پچھلا_extent.e_len++;
				retval = ext2fs_extent_replace(handle, 0,
							       &پچھلا_extent);
			} ورنہ
				retval = ext2fs_extent_داخل_کرو(handle,
							      0, &newextent);
			اگر (retval)
				جاؤ مکمل;
			retval = ext2fs_extent_fix_parents(handle);
			اگر (retval)
				جاؤ مکمل;
			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_اگلا_برگ,
						   &extent);
			اگر (retval)
				جاؤ مکمل;
		}
	} ورنہ {
		__u32	save_لمبائی;
		blk64_t	save_lblk;
		ساخت ext2fs_extent save_extent;
		errcode_t r2;

#اگر_تعریف_شدہ DEBUG
		لکھو("(re/un)mapping in middle of extent\n");
#اگر_ختم
		/* need to split this extent; later */
		save_lblk = extent.e_lblk;
		save_لمبائی = extent.e_len;
		save_extent = extent;

		/* shorten pre-split extent */
		extent.e_len = (logical - extent.e_lblk);
		retval = ext2fs_extent_replace(handle, 0, &extent);
		اگر (retval)
			جاؤ مکمل;
		/* insert our new extent, if any */
		اگر (physical) {
			/* insert new extent after current */
			retval = ext2fs_extent_داخل_کرو(handle,
					EXT2_EXTENT_داخل_کرو_AFTER, &newextent);
			اگر (retval) {
				r2 = ext2fs_extent_goto(handle, save_lblk);
				اگر (r2 == 0)
					(خالی)ext2fs_extent_replace(handle, 0,
							      &save_extent);
				جاؤ مکمل;
			}
		}
		/* add post-split extent */
		extent.e_pblk += extent.e_len + 1;
		extent.e_lblk += extent.e_len + 1;
		extent.e_len = save_لمبائی - extent.e_len - 1;
		retval = ext2fs_extent_داخل_کرو(handle,
				EXT2_EXTENT_داخل_کرو_AFTER, &extent);
		اگر (retval) {
			اگر (physical) {
				r2 = ext2fs_extent_goto(handle,
							newextent.e_lblk);
				اگر (r2 == 0)
					(خالی)ext2fs_extent_delete(handle, 0);
			}
			r2 = ext2fs_extent_goto(handle, save_lblk);
			اگر (r2 == 0)
				(خالی)ext2fs_extent_replace(handle, 0,
							    &save_extent);
			جاؤ مکمل;
		}
	}

مکمل:
	/* get handle back to its position */
	اگر (orig_اونچائی > handle->max_گہرائی)
		orig_اونچائی = handle->max_گہرائی; /* In case we shortened the tree */
	ext2fs_extent_goto2(handle, orig_اونچائی, orig_lblk);
	واپس retval;
}

errcode_t ext2fs_extent_delete(ext2_extent_handle_t handle, صحیح flags)
{
	ساخت extent_راستہ		*راستہ;
	حرف 				*cp;
	ساخت ext3_extent_سرخی	*eh;
	errcode_t			retval = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	اگر (!(handle->fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	اگر (!handle->راستہ)
		واپس EXT2_ET_NO_CURRENT_گرہ;

#اگر_تعریف_شدہ DEBUG
	{
		ساخت ext2fs_extent	extent;

		retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					   &extent);
		اگر (retval == 0) {
			لکھو("extent delete %u ", handle->ino);
			dbg_print_extent(0, &extent);
		}
	}
#اگر_ختم

	راستہ = handle->راستہ + handle->level;
	اگر (!راستہ->curr)
		واپس EXT2_ET_NO_CURRENT_گرہ;

	cp = راستہ->curr;

	/* Sanity check before memmove() */
	اگر (راستہ->بایاں < 0)
		واپس EXT2_ET_EXTENT_برگ_BAD;

	اگر (راستہ->بایاں) {
		memmove(cp, cp + حجم(ساخت ext3_extent_idx),
			راستہ->بایاں * حجم(ساخت ext3_extent_idx));
		راستہ->بایاں--;
	} ورنہ {
		ساخت ext3_extent_idx	*ix = راستہ->curr;
		ix--;
		راستہ->curr = ix;
	}
	اگر (--راستہ->entries == 0)
		راستہ->curr = 0;

	/* if non-root node has no entries left, remove it & parent ptr to it */
	اگر (راستہ->entries == 0 && handle->level) {
		اگر (!(flags & EXT2_EXTENT_DELETE_KEEP_خالی_خانے)) {
			ساخت ext2fs_extent	extent;

			retval = ext2fs_extent_get(handle, EXT2_EXTENT_اوپر,
								&extent);
			اگر (retval)
				واپس retval;

			retval = ext2fs_extent_delete(handle, flags);
			handle->inode->i_بلاکیں -=
				(handle->fs->blocksize *
				 EXT2_FS_CLUSTER_تناسب(handle->fs)) / 512;
			retval = ext2fs_write_inode(handle->fs, handle->ino,
						    handle->inode);
			ext2fs_بلاک_alloc_stats2(handle->fs,
						  extent.e_pblk, -1);
		}
	} ورنہ {
		eh = (ساخت ext3_extent_سرخی *) راستہ->buf;
		eh->eh_entries = ext2fs_cpu_مقصد_le16(راستہ->entries);
		اگر ((راستہ->entries == 0) && (handle->level == 0)) {
			eh->eh_گہرائی = 0;
			handle->max_گہرائی = 0;
		}
		retval = update_راستہ(handle);
	}
	واپس retval;
}

errcode_t ext2fs_extent_get_info(ext2_extent_handle_t handle,
				 ساخت ext2_extent_info *info)
{
	ساخت extent_راستہ		*راستہ;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EXTENT_HANDLE);

	memset(info, 0, حجم(ساخت ext2_extent_info));

	راستہ = handle->راستہ + handle->level;
	اگر (راستہ) {
		اگر (راستہ->curr)
			info->curr_اندراج = ((حرف *) راستہ->curr - راستہ->buf) /
				حجم(ساخت ext3_extent_idx);
		ورنہ
			info->curr_اندراج = 0;
		info->عدد_entries = راستہ->entries;
		info->max_entries = راستہ->max_entries;
		info->بائٹیں_avail = (راستہ->max_entries - راستہ->entries) *
			حجم(ساخت ext3_extent);
	}

	info->curr_level = handle->level;
	info->max_گہرائی = handle->max_گہرائی;
	info->max_lblk = EXT_MAX_EXTENT_LBLK;
	info->max_pblk = EXT_MAX_EXTENT_PBLK;
	info->max_len = EXT_INIT_MAX_LEN;
	info->max_uninit_len = EXT_UNINIT_MAX_LEN;

	واپس 0;
}

size_t ext2fs_max_extent_گہرائی(ext2_extent_handle_t handle)
{
	size_t iblock_sz = حجم(((ساخت ext2_inode *)تہی)->i_بلاک);
	size_t iblock_extents = (iblock_sz - حجم(ساخت ext3_extent_سرخی)) /
				حجم(ساخت ext3_extent);
	size_t extents_فی_بلاک = (handle->fs->blocksize -
				    حجم(ساخت ext3_extent_سرخی)) /
				   حجم(ساخت ext3_extent);
	ساکن غیرعلامتی صحیح آخری_blocksize = 0;
	ساکن size_t آخری_نتیجہ = 0;

	اگر (آخری_blocksize && آخری_blocksize == handle->fs->blocksize)
		واپس آخری_نتیجہ;

	آخری_نتیجہ = 1 + ((ext2fs_log2_u64(EXT_MAX_EXTENT_LBLK) -
			    ext2fs_log2_u64(iblock_extents)) /
			   ext2fs_log2_u32(extents_فی_بلاک));
	آخری_blocksize = handle->fs->blocksize;
	واپس آخری_نتیجہ;
}

errcode_t ext2fs_fix_extents_checksums(ext2_filsys fs, ext2_ino_t ino,
				       ساخت ext2_inode *inode)
{
	ext2_extent_handle_t	handle;
	ساخت ext2fs_extent	extent;
	errcode_t		errcode;
	صحیح    			save_flags = fs->flags;

	اگر (!ext2fs_has_feature_metadata_csum(fs->super) ||
	    (inode && !(inode->i_flags & EXT4_EXTENTS_FL)))
		واپس 0;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	اگر (errcode) {
		اگر (errcode == EXT2_ET_INODE_NOT_EXTENT)
			errcode = 0;
		واپس errcode;
	}

	fs->flags &= ~EXT2_FLAG_IGNORE_CSUM_ERRORS;
	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_جڑ, &extent);
	اگر (errcode)
		جاؤ برآمد;

	کرو {
		/* Skip to the end of a block of leaf nodes */
		اگر (extent.e_flags & EXT2_EXTENT_FLAGS_برگ) {
			errcode = ext2fs_extent_get(handle,
						    EXT2_EXTENT_آخری_SIB,
						    &extent);
			اگر (errcode)
				توڑو;
		}

		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_اگلا, &extent);
		اگر (errcode == EXT2_ET_EXTENT_CSUM_INVALID)
			errcode = update_راستہ(handle);
	} جب (errcode == 0);

برآمد:
	/* Ok if we run off the end */
	اگر (errcode == EXT2_ET_EXTENT_NO_اگلا)
		errcode = 0;
	ext2fs_extent_free(handle);
	fs->flags = save_flags;
	واپس errcode;
}

errcode_t ext2fs_decode_extent(ساخت ext2fs_extent *مقصد, خالی *addr, صحیح len)
{
	ساخت ext3_extent *from = (ساخت ext3_extent *)addr;

	اگر (len != حجم(ساخت ext3_extent))
		واپس EXT2_ET_INVALID_دلیل;

	مقصد->e_pblk = ext2fs_le32_مقصد_cpu(from->ee_آغاز) +
		((__u64) ext2fs_le16_مقصد_cpu(from->ee_آغاز_hi)
			<< 32);
	مقصد->e_lblk = ext2fs_le32_مقصد_cpu(from->ee_بلاک);
	مقصد->e_len = ext2fs_le16_مقصد_cpu(from->ee_len);
	مقصد->e_flags = EXT2_EXTENT_FLAGS_برگ;
	اگر (مقصد->e_len > EXT_INIT_MAX_LEN) {
		مقصد->e_len -= EXT_INIT_MAX_LEN;
		مقصد->e_flags |= EXT2_EXTENT_FLAGS_UNINIT;
	}

	واپس 0;
}

errcode_t ext2fs_گنتی_بلاکیں(ext2_filsys fs, ext2_ino_t ino,
			      ساخت ext2_inode *inode, blk64_t *ret_گنتی)
{
	ext2_extent_handle_t	handle = تہی;
	ساخت ext2fs_extent	extent;
	errcode_t		errcode;
	صحیح			i;
	blk64_t			blkcount = 0;
	blk64_t			*intermediate_nodes;

	errcode = ext2fs_extent_open2(fs, ino, inode, &handle);
	اگر (errcode)
		جاؤ برآمد;

	errcode = ext2fs_extent_get(handle, EXT2_EXTENT_جڑ, &extent);
	اگر (errcode)
		جاؤ برآمد;

	errcode = ext2fs_get_array(handle->max_گہرائی, حجم(blk64_t),
				   &intermediate_nodes);
	اگر (errcode)
		جاؤ برآمد;

	blkcount = handle->level;
	جب (!errcode) {
		اگر (extent.e_flags & EXT2_EXTENT_FLAGS_برگ) {
			blkcount += extent.e_len;
			برائے (i = 0; i < handle->level; i++) {
				اگر (intermediate_nodes[i] !=
					handle->راستہ[i].انتہا_blk) {
					blkcount++;
					intermediate_nodes[i] =
						handle->راستہ[i].انتہا_blk;
				}
			}
		}
		errcode = ext2fs_extent_get(handle, EXT2_EXTENT_اگلا, &extent);
	}
	ext2fs_free_mem(&intermediate_nodes);
برآمد:
	*ret_گنتی = blkcount;
	ext2fs_extent_free(handle);

	واپس 0;
}

#اگر_تعریف_شدہ DEBUG
/*
 * Override debugfs's prompt
 */
ثابت حرف *debug_prog_name = "tst_extents";

#اگر_ختم

