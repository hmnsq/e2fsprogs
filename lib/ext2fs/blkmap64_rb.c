/*
 * blkmap64_rb.c --- Simple rb-tree implementation for bitmaps
 *
 * (C)2010 Red Hat, Inc., Lukas Czerner <lczerner@redhat.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#إذا HAVE_LINUX_TYPES_H
#ضمن <linux/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"
#ضمن "rbtree.h"

#ضمن <limits.h>

بنية bmap_rb_extent {
	بنية rb_ڳنڍ ڳنڍ;
	__u64 شروعات;
	__u64 ڳڻپ;
};

بنية ext2fs_rb_ذاتي {
	بنية rb_پاڙ پاڙ;
	بنية bmap_rb_extent *wcursor;
	بنية bmap_rb_extent *rcursor;
	بنية bmap_rb_extent *rcursor_ايندڙ;
#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
	__u64 mark_ڪاميابي;
	__u64 test_ڪاميابي;
#جيڪڏهن_پورو
};

مضمن ساكن بنية bmap_rb_extent *ڳنڍ_منزل_extent(بنية rb_ڳنڍ *ڳنڍ)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	أرجع (بنية bmap_rb_extent *) ڳنڍ;
}

ساكن صحيح rb_وجهو_extent(__u64 شروعات, __u64 ڳڻپ,
			    بنية ext2fs_rb_ذاتي *);
ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#جيڪڏهن_وصف_ٿيل DEBUG_RB
ساكن فراغ print_وڻ(بنية rb_پاڙ *پاڙ)
{
	بنية rb_ڳنڍ *ڳنڍ = عدم;
	بنية bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	ڳنڍ = ext2fs_rb_پهريون(پاڙ);
	لكل (ڳنڍ = ext2fs_rb_پهريون(پاڙ); ڳنڍ != عدم; 
	     ڳنڍ = ext2fs_rb_ايندڙ(ڳنڍ)) {
		ext = ڳنڍ_منزل_extent(ڳنڍ);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(بنانشان ڊگهو ڊگهو) ext->شروعات,
			(بنانشان ڊگهو ڊگهو) ext->شروعات + ext->ڳڻپ);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساكن فراغ check_وڻ(بنية rb_پاڙ *پاڙ, ثابت حرف *نياپو)
{
	بنية rb_ڳنڍ *ڳنڍ;
	بنية bmap_rb_extent *ext, *old = عدم;

	لكل (ڳنڍ = ext2fs_rb_پهريون(پاڙ); ڳنڍ;
	     ڳنڍ = ext2fs_rb_ايندڙ(ڳنڍ)) {
		ext = ڳنڍ_منزل_extent(ڳنڍ);
		إذا (ext->ڳڻپ == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(بنانشان ڊگهو ڊگهو) ext->شروعات,
				(بنانشان ڊگهو ڊگهو) ext->شروعات + ext->ڳڻپ,
				(بنانشان ڊگهو ڊگهو) ext->ڳڻپ);
			اذهب err_ٻاهريون;
		}
		إذا (ext->شروعات + ext->ڳڻپ < ext->شروعات) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(بنانشان ڊگهو ڊگهو) ext->شروعات,
				(بنانشان ڊگهو ڊگهو) ext->شروعات + ext->ڳڻپ,
				(بنانشان ڊگهو ڊگهو) ext->ڳڻپ);
			اذهب err_ٻاهريون;
		}

		إذا (old) {
			إذا (old->شروعات > ext->شروعات) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(بنانشان ڊگهو ڊگهو) old->شروعات,
					(بنانشان ڊگهو ڊگهو) old->شروعات + old->ڳڻپ,
					(بنانشان ڊگهو ڊگهو) old->ڳڻپ);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(بنانشان ڊگهو ڊگهو) ext->شروعات,
					(بنانشان ڊگهو ڊگهو) ext->شروعات + ext->ڳڻپ,
					(بنانشان ڊگهو ڊگهو) ext->ڳڻپ);
				اذهب err_ٻاهريون;
			}
			إذا ((old->شروعات + old->ڳڻپ) >= ext->شروعات) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(بنانشان ڊگهو ڊگهو) old->شروعات,
					(بنانشان ڊگهو ڊگهو) old->شروعات + old->ڳڻپ,
					(بنانشان ڊگهو ڊگهو) old->ڳڻپ);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(بنانشان ڊگهو ڊگهو) ext->شروعات,
					(بنانشان ڊگهو ڊگهو) ext->شروعات + ext->ڳڻپ,
					(بنانشان ڊگهو ڊگهو) ext->ڳڻپ);
				اذهب err_ٻاهريون;
			}
		}
		old = ext;
	}
	أرجع;

err_ٻاهريون:
	fprintf(stderr, "%s\n", نياپو);
	print_وڻ(پاڙ);
	ٻاهر_نڪرو(1);
}
#وإلا
#وصف_ڪريو check_وڻ(پاڙ, نياپو) افعل {} طالما (0)
#وصف_ڪريو print_وڻ(پاڙ) افعل {} طالما (0)
#جيڪڏهن_پورو

ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **ext, __u64 شروعات,
			     __u64 ڳڻپ)
{
	بنية bmap_rb_extent *new_ext;
	صحيح retval;

	retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
				&new_ext);
	إذا (retval)
		abort();

	new_ext->شروعات = شروعات;
	new_ext->ڳڻپ = ڳڻپ;
	*ext = new_ext;
}

مضمن
ساكن فراغ rb_free_extent(بنية ext2fs_rb_ذاتي *bp,
			   بنية bmap_rb_extent *ext)
{
	إذا (bp->wcursor == ext)
		bp->wcursor = عدم;
	إذا (bp->rcursor == ext)
		bp->rcursor = عدم;
	إذا (bp->rcursor_ايندڙ == ext)
		bp->rcursor_ايندڙ = عدم;
	ext2fs_free_mem(&ext);
}

ساكن errcode_t rb_alloc_ذاتي_معلومات (ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_ذاتي *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم (بنية ext2fs_rb_ذاتي), &bp);
	إذا (retval)
		أرجع retval;

	bp->پاڙ = RB_پاڙ;
	bp->rcursor = عدم;
	bp->rcursor_ايندڙ = عدم;
	bp->wcursor = عدم;

#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
	bp->test_ڪاميابي = 0;
	bp->mark_ڪاميابي = 0;
#جيڪڏهن_پورو

	bitmap->خاص = (فراغ *) bp;
	أرجع 0;
}

ساكن errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_ذاتي_معلومات (bitmap);
	إذا (retval)
		أرجع retval;

	أرجع 0;
}

ساكن فراغ rb_free_وڻ(بنية rb_پاڙ *پاڙ)
{
	بنية bmap_rb_extent *ext;
	بنية rb_ڳنڍ *ڳنڍ, *ايندڙ;

	لكل (ڳنڍ = ext2fs_rb_پهريون(پاڙ); ڳنڍ; ڳنڍ = ايندڙ) {
		ايندڙ = ext2fs_rb_ايندڙ(ڳنڍ);
		ext = ڳنڍ_منزل_extent(ڳنڍ);
		ext2fs_rb_erase(ڳنڍ, پاڙ);
		ext2fs_free_mem(&ext);
	}
}

ساكن فراغ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_ذاتي *bp;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;

	rb_free_وڻ(&bp->پاڙ);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساكن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	بنية ext2fs_rb_ذاتي *src_bp, *dest_bp;
	بنية bmap_rb_extent *src_ext, *dest_ext;
	بنية rb_ڳنڍ *dest_ڳنڍ, *src_ڳنڍ, *dest_آخري, **n;
	errcode_t retval = 0;

	retval = rb_alloc_ذاتي_معلومات (dest);
	إذا (retval)
		أرجع retval;

	src_bp = (بنية ext2fs_rb_ذاتي *) src->خاص;
	dest_bp = (بنية ext2fs_rb_ذاتي *) dest->خاص;
	src_bp->rcursor = عدم;
	dest_bp->rcursor = عدم;

	src_ڳنڍ = ext2fs_rb_پهريون(&src_bp->پاڙ);
	طالما (src_ڳنڍ) {
		src_ext = ڳنڍ_منزل_extent(src_ڳنڍ);
		retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
					&dest_ext);
		إذا (retval)
			اكسر;

		يادگيري_نقل(dest_ext, src_ext, حجم(بنية bmap_rb_extent));

		dest_ڳنڍ = &dest_ext->ڳنڍ;
		n = &dest_bp->پاڙ.rb_ڳنڍ;

		dest_آخري = عدم;
		إذا (*n) {
			dest_آخري = ext2fs_rb_آخري(&dest_bp->پاڙ);
			n = &(dest_آخري)->rb_ساڄو;
		}

		ext2fs_rb_link_ڳنڍ(dest_ڳنڍ, dest_آخري, n);
		ext2fs_rb_وجهو_color(dest_ڳنڍ, &dest_bp->پاڙ);

		src_ڳنڍ = ext2fs_rb_ايندڙ(src_ڳنڍ);
	}

	أرجع retval;
}

ساكن فراغ rb_truncate(__u64 new_max, بنية rb_پاڙ *پاڙ)
{
	بنية bmap_rb_extent *ext;
	بنية rb_ڳنڍ *ڳنڍ;

	ڳنڍ = ext2fs_rb_آخري(پاڙ);
	طالما (ڳنڍ) {
		ext = ڳنڍ_منزل_extent(ڳنڍ);

		إذا ((ext->شروعات + ext->ڳڻپ - 1) <= new_max)
			اكسر;
		وإلا إذا (ext->شروعات > new_max) {
			ext2fs_rb_erase(ڳنڍ, پاڙ);
			ext2fs_free_mem(&ext);
			ڳنڍ = ext2fs_rb_آخري(پاڙ);
			استمر;
		} وإلا
			ext->ڳڻپ = new_max - ext->شروعات + 1;
	}
}

ساكن errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_پڇاڙي, __u64 new_real_پڇاڙي)
{
	بنية ext2fs_rb_ذاتي *bp;

	bp = (بنية ext2fs_rb_ذاتي *) bmap->خاص;
	bp->rcursor = عدم;
	bp->wcursor = عدم;

	rb_truncate(((new_پڇاڙي < bmap->پڇاڙي) ? new_پڇاڙي : bmap->پڇاڙي) - bmap->شروعات,
		    &bp->پاڙ);

	bmap->پڇاڙي = new_پڇاڙي;
	bmap->real_پڇاڙي = new_real_پڇاڙي;

	إذا (bmap->پڇاڙي < bmap->real_پڇاڙي)
		rb_وجهو_extent(bmap->پڇاڙي + 1 - bmap->شروعات,
				 bmap->real_پڇاڙي - bmap->پڇاڙي, bp);
	أرجع 0;

}

مضمن ساكن صحيح
rb_test_ٻٽو_انگ(بنية ext2fs_rb_ذاتي *bp, __u64 ٻٽو_انگ)
{
	بنية bmap_rb_extent *rcursor, *ايندڙ_ext = عدم;
	بنية rb_ڳنڍ *والد = عدم, *ايندڙ;
	بنية rb_ڳنڍ **n = &bp->پاڙ.rb_ڳنڍ;
	بنية bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	إذا (!rcursor)
		اذهب ڳولا_وڻ;

	إذا (ٻٽو_انگ >= rcursor->شروعات && ٻٽو_انگ < rcursor->شروعات + rcursor->ڳڻپ) {
#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
		bp->test_ڪاميابي++;
#جيڪڏهن_پورو
		أرجع 1;
	}

	ايندڙ_ext = bp->rcursor_ايندڙ;
	إذا (!ايندڙ_ext) {
		ايندڙ = ext2fs_rb_ايندڙ(&rcursor->ڳنڍ);
		إذا (ايندڙ)
			ايندڙ_ext = ڳنڍ_منزل_extent(ايندڙ);
		bp->rcursor_ايندڙ = ايندڙ_ext;
	}
	إذا (ايندڙ_ext) {
		إذا ((ٻٽو_انگ >= rcursor->شروعات + rcursor->ڳڻپ) &&
		    (ٻٽو_انگ < ايندڙ_ext->شروعات)) {
#جيڪڏهن_وصف_ٿيل BMAP_STATS_OPS
			bp->test_ڪاميابي++;
#جيڪڏهن_پورو
			أرجع 0;
		}
	}
	bp->rcursor = عدم;
	bp->rcursor_ايندڙ = عدم;

	rcursor = bp->wcursor;
	إذا (!rcursor)
		اذهب ڳولا_وڻ;

	إذا (ٻٽو_انگ >= rcursor->شروعات && ٻٽو_انگ < rcursor->شروعات + rcursor->ڳڻپ)
		أرجع 1;

ڳولا_وڻ:

	طالما (*n) {
		والد = *n;
		ext = ڳنڍ_منزل_extent(والد);
		إذا (ٻٽو_انگ < ext->شروعات)
			n = &(*n)->rb_کاٻو;
		وإلا إذا (ٻٽو_انگ >= (ext->شروعات + ext->ڳڻپ))
			n = &(*n)->rb_ساڄو;
		وإلا {
			bp->rcursor = ext;
			bp->rcursor_ايندڙ = عدم;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح rb_وجهو_extent(__u64 شروعات, __u64 ڳڻپ,
			    بنية ext2fs_rb_ذاتي *bp)
{
	بنية rb_پاڙ *پاڙ = &bp->پاڙ;
	بنية rb_ڳنڍ *والد = عدم, **n = &پاڙ->rb_ڳنڍ;
	بنية rb_ڳنڍ *new_ڳنڍ, *ڳنڍ, *ايندڙ;
	بنية bmap_rb_extent *new_ext;
	بنية bmap_rb_extent *ext;
	صحيح retval = 0;

	إذا (ڳڻپ == 0)
		أرجع 0;

	bp->rcursor_ايندڙ = عدم;
	ext = bp->wcursor;
	إذا (ext) {
		إذا (شروعات >= ext->شروعات &&
		    شروعات <= (ext->شروعات + ext->ڳڻپ)) {
#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
			bp->mark_ڪاميابي++;
#جيڪڏهن_پورو
			اذهب got_extent;
		}
	}

	طالما (*n) {
		والد = *n;
		ext = ڳنڍ_منزل_extent(والد);

		إذا (شروعات < ext->شروعات) {
			n = &(*n)->rb_کاٻو;
		} وإلا إذا (شروعات > (ext->شروعات + ext->ڳڻپ)) {
			n = &(*n)->rb_ساڄو;
		} وإلا {
got_extent:
			إذا ((شروعات + ڳڻپ) <= (ext->شروعات + ext->ڳڻپ))
				أرجع 1;

			إذا ((ext->شروعات + ext->ڳڻپ) == شروعات)
				retval = 0;
			وإلا
				retval = 1;

			ڳڻپ += (شروعات - ext->شروعات);
			شروعات = ext->شروعات;
			new_ext = ext;
			new_ڳنڍ = &ext->ڳنڍ;

			اذهب skip_وجهو;
		}
	}

	rb_get_new_extent(&new_ext, شروعات, ڳڻپ);

	new_ڳنڍ = &new_ext->ڳنڍ;
	ext2fs_rb_link_ڳنڍ(new_ڳنڍ, والد, n);
	ext2fs_rb_وجهو_color(new_ڳنڍ, پاڙ);
	bp->wcursor = new_ext;

	ڳنڍ = ext2fs_rb_prev(new_ڳنڍ);
	إذا (ڳنڍ) {
		ext = ڳنڍ_منزل_extent(ڳنڍ);
		إذا ((ext->شروعات + ext->ڳڻپ) == شروعات) {
			شروعات = ext->شروعات;
			ڳڻپ += ext->ڳڻپ;
			ext2fs_rb_erase(ڳنڍ, پاڙ);
			rb_free_extent(bp, ext);
		}
	}

skip_وجهو:
	/* See if we can merge extent to the right */
	لكل (ڳنڍ = ext2fs_rb_ايندڙ(new_ڳنڍ); ڳنڍ != عدم; ڳنڍ = ايندڙ) {
		ايندڙ = ext2fs_rb_ايندڙ(ڳنڍ);
		ext = ڳنڍ_منزل_extent(ڳنڍ);

		إذا ((ext->شروعات + ext->ڳڻپ) <= شروعات)
			استمر;

		/* No more merging */
		إذا ((شروعات + ڳڻپ) < ext->شروعات)
			اكسر;

		/* ext is embedded in new_ext interval */
		إذا ((شروعات + ڳڻپ) >= (ext->شروعات + ext->ڳڻپ)) {
			ext2fs_rb_erase(ڳنڍ, پاڙ);
			rb_free_extent(bp, ext);
			استمر;
		} وإلا {
		/* merge ext with new_ext */
			ڳڻپ += ((ext->شروعات + ext->ڳڻپ) -
				  (شروعات + ڳڻپ));
			ext2fs_rb_erase(ڳنڍ, پاڙ);
			rb_free_extent(bp, ext);
			اكسر;
		}
	}

	new_ext->شروعات = شروعات;
	new_ext->ڳڻپ = ڳڻپ;

	أرجع retval;
}

ساكن صحيح rb_remove_extent(__u64 شروعات, __u64 ڳڻپ,
			    بنية ext2fs_rb_ذاتي *bp)
{
	بنية rb_پاڙ *پاڙ = &bp->پاڙ;
	بنية rb_ڳنڍ *والد = عدم, **n = &پاڙ->rb_ڳنڍ;
	بنية rb_ڳنڍ *ڳنڍ;
	بنية bmap_rb_extent *ext;
	__u64 new_شروعات, new_ڳڻپ;
	صحيح retval = 0;

	إذا (ext2fs_rb_خالي_جايون_پاڙ(پاڙ))
		أرجع 0;

	طالما (*n) {
		والد = *n;
		ext = ڳنڍ_منزل_extent(والد);
		إذا (شروعات < ext->شروعات) {
			n = &(*n)->rb_کاٻو;
			استمر;
		} وإلا إذا (شروعات >= (ext->شروعات + ext->ڳڻپ)) {
			n = &(*n)->rb_ساڄو;
			استمر;
		}

		إذا ((شروعات > ext->شروعات) &&
		    (شروعات + ڳڻپ) < (ext->شروعات + ext->ڳڻپ)) {
			/* We have to split extent into two */
			new_شروعات = شروعات + ڳڻپ;
			new_ڳڻپ = (ext->شروعات + ext->ڳڻپ) - new_شروعات;

			ext->ڳڻپ = شروعات - ext->شروعات;

			rb_وجهو_extent(new_شروعات, new_ڳڻپ, bp);
			أرجع 1;
		}

		إذا ((شروعات + ڳڻپ) >= (ext->شروعات + ext->ڳڻپ)) {
			ext->ڳڻپ = شروعات - ext->شروعات;
			retval = 1;
		}

		إذا (0 == ext->ڳڻپ) {
			والد = ext2fs_rb_ايندڙ(&ext->ڳنڍ);
			ext2fs_rb_erase(&ext->ڳنڍ, پاڙ);
			rb_free_extent(bp, ext);
			اكسر;
		}

		إذا (شروعات == ext->شروعات) {
			ext->شروعات += ڳڻپ;
			ext->ڳڻپ -= ڳڻپ;
			أرجع 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	لكل (; والد != عدم; والد = ڳنڍ) {
		ڳنڍ = ext2fs_rb_ايندڙ(والد);
		ext = ڳنڍ_منزل_extent(والد);
		إذا ((ext->شروعات + ext->ڳڻپ) <= شروعات)
			استمر;

		/* No more extents to be removed/truncated */
		إذا ((شروعات + ڳڻپ) < ext->شروعات)
			اكسر;

		/* The entire extent is within the region to be removed */
		إذا ((شروعات + ڳڻپ) >= (ext->شروعات + ext->ڳڻپ)) {
			ext2fs_rb_erase(والد, پاڙ);
			rb_free_extent(bp, ext);
			retval = 1;
			استمر;
		} وإلا {
			/* modify the last extent in region to be removed */
			ext->ڳڻپ -= ((شروعات + ڳڻپ) - ext->شروعات);
			ext->شروعات = شروعات + ڳڻپ;
			retval = 1;
			اكسر;
		}
	}

	أرجع retval;
}

ساكن صحيح rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_ذاتي *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;
	arg -= bitmap->شروعات;

	retval = rb_وجهو_extent(arg, 1, bp);
	check_وڻ(&bp->پاڙ, __func__);
	أرجع retval;
}

ساكن صحيح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_ذاتي *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;
	arg -= bitmap->شروعات;

	retval = rb_remove_extent(arg, 1, bp);
	check_وڻ(&bp->پاڙ, __func__);

	أرجع retval;
}

مضمن
ساكن صحيح rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_ذاتي *bp;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;
	arg -= bitmap->شروعات;

	أرجع rb_test_ٻٽو_انگ(bp, arg);
}

ساكن فراغ rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				بنانشان صحيح انگ)
{
	بنية ext2fs_rb_ذاتي *bp;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;
	arg -= bitmap->شروعات;

	rb_وجهو_extent(arg, انگ, bp);
	check_وڻ(&bp->پاڙ, __func__);
}

ساكن فراغ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  بنانشان صحيح انگ)
{
	بنية ext2fs_rb_ذاتي *bp;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;
	arg -= bitmap->شروعات;

	rb_remove_extent(arg, انگ, bp);
	check_وڻ(&bp->پاڙ, __func__);
}

ساكن صحيح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروعات, بنانشان صحيح len)
{
	بنية rb_ڳنڍ *والد = عدم, **n;
	بنية rb_ڳنڍ *ڳنڍ, *ايندڙ;
	بنية ext2fs_rb_ذاتي *bp;
	بنية bmap_rb_extent *ext;
	صحيح retval = 1;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;
	n = &bp->پاڙ.rb_ڳنڍ;
	شروعات -= bitmap->شروعات;

	إذا (len == 0 || ext2fs_rb_خالي_جايون_پاڙ(&bp->پاڙ))
		أرجع 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	طالما (*n) {
		والد = *n;
		ext = ڳنڍ_منزل_extent(والد);
		إذا (شروعات < ext->شروعات) {
			n = &(*n)->rb_کاٻو;
		} وإلا إذا (شروعات >= (ext->شروعات + ext->ڳڻپ)) {
			n = &(*n)->rb_ساڄو;
		} وإلا {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			أرجع 0;
		}
	}

	ڳنڍ = والد;
	طالما (ڳنڍ) {
		ايندڙ = ext2fs_rb_ايندڙ(ڳنڍ);
		ext = ڳنڍ_منزل_extent(ڳنڍ);
		ڳنڍ = ايندڙ;

		إذا ((ext->شروعات + ext->ڳڻپ) <= شروعات)
			استمر;

		/* No more merging */
		إذا ((شروعات + len) <= ext->شروعات)
			اكسر;

		retval = 0;
		اكسر;
	}
	أرجع retval;
}

ساكن errcode_t rb_سيٽ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروعات, size_t انگ, فراغ *اندريون)
{
	بنية ext2fs_rb_ذاتي *bp;
	بنانشان حرف *cp = اندريون;
	size_t i;
	صحيح پهريون_سيٽ = -1;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;

	لكل (i = 0; i < انگ; i++) {
		إذا ((i & 7) == 0) {
			بنانشان حرف c = cp[i/8];
			إذا (c == 0xFF) {
				إذا (پهريون_سيٽ == -1)
					پهريون_سيٽ = i;
				i += 7;
				استمر;
			}
			إذا ((c == 0x00) && (پهريون_سيٽ == -1)) {
				i += 7;
				استمر;
			}
		}
		إذا (ext2fs_test_ٻٽو_انگ(i, اندريون)) {
			إذا (پهريون_سيٽ == -1)
				پهريون_سيٽ = i;
			استمر;
		}
		إذا (پهريون_سيٽ == -1)
			استمر;

		rb_وجهو_extent(شروعات + پهريون_سيٽ - bitmap->شروعات,
				 i - پهريون_سيٽ, bp);
		check_وڻ(&bp->پاڙ, __func__);
		پهريون_سيٽ = -1;
	}
	إذا (پهريون_سيٽ != -1) {
		rb_وجهو_extent(شروعات + پهريون_سيٽ - bitmap->شروعات,
				 انگ - پهريون_سيٽ, bp);
		check_وڻ(&bp->پاڙ, __func__);
	}

	أرجع 0;
}

ساكن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروعات, size_t انگ, فراغ *ٻاهريون)
{

	بنية rb_ڳنڍ *والد = عدم, *ايندڙ, **n;
	بنية ext2fs_rb_ذاتي *bp;
	بنية bmap_rb_extent *ext;
	__u64 ڳڻپ, pos;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;
	n = &bp->پاڙ.rb_ڳنڍ;
	شروعات -= bitmap->شروعات;

	إذا (ext2fs_rb_خالي_جايون_پاڙ(&bp->پاڙ))
		أرجع 0;

	طالما (*n) {
		والد = *n;
		ext = ڳنڍ_منزل_extent(والد);
		إذا (شروعات < ext->شروعات) {
			n = &(*n)->rb_کاٻو;
		} وإلا إذا (شروعات >= (ext->شروعات + ext->ڳڻپ)) {
			n = &(*n)->rb_ساڄو;
		} وإلا
			اكسر;
	}

	memset(ٻاهريون, 0, (انگ + 7) >> 3);

	لكل (; والد != عدم; والد = ايندڙ) {
		ايندڙ = ext2fs_rb_ايندڙ(والد);
		ext = ڳنڍ_منزل_extent(والد);

		pos = ext->شروعات;
		ڳڻپ = ext->ڳڻپ;
		إذا (pos >= شروعات + انگ)
			اكسر;
		إذا (pos < شروعات) {
			إذا (pos + ڳڻپ <  شروعات)
				استمر;
			ڳڻپ -= شروعات - pos;
			pos = شروعات;
		}
		إذا (pos + ڳڻپ > شروعات + انگ)
			ڳڻپ = شروعات + انگ - pos;

		طالما (ڳڻپ > 0) {
			إذا ((ڳڻپ >= 8) &&
			    ((pos - شروعات) % 8) == 0) {
				صحيح nbytes = ڳڻپ >> 3;
				صحيح وٿي = (pos - شروعات) >> 3;

				memset(((حرف *) ٻاهريون) + وٿي, 0xFF, nbytes);
				pos += nbytes << 3;
				ڳڻپ -= nbytes << 3;
				استمر;
			}
			ext2fs_fast_سيٽ_bit64((pos - شروعات), ٻاهريون);
			pos++;
			ڳڻپ--;
		}
	}
	أرجع 0;
}

ساكن فراغ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_ذاتي *bp;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;

	rb_free_وڻ(&bp->پاڙ);
	bp->rcursor = عدم;
	bp->rcursor_ايندڙ = عدم;
	bp->wcursor = عدم;
	check_وڻ(&bp->پاڙ, __func__);
}

ساكن errcode_t rb_ڳوليو_پهريون_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 شروعات, __u64 پڇاڙي, __u64 *ٻاهريون)
{
	بنية rb_ڳنڍ *والد = عدم, **n;
	بنية ext2fs_rb_ذاتي *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;
	n = &bp->پاڙ.rb_ڳنڍ;
	شروعات -= bitmap->شروعات;
	پڇاڙي -= bitmap->شروعات;

	إذا (شروعات > پڇاڙي)
		أرجع EINVAL;

	إذا (ext2fs_rb_خالي_جايون_پاڙ(&bp->پاڙ))
		أرجع ENOENT;

	طالما (*n) {
		والد = *n;
		ext = ڳنڍ_منزل_extent(والد);
		إذا (شروعات < ext->شروعات) {
			n = &(*n)->rb_کاٻو;
		} وإلا إذا (شروعات >= (ext->شروعات + ext->ڳڻپ)) {
			n = &(*n)->rb_ساڄو;
		} وإلا إذا (ext->شروعات + ext->ڳڻپ <= پڇاڙي) {
			*ٻاهريون = ext->شروعات + ext->ڳڻپ + bitmap->شروعات;
			أرجع 0;
		} وإلا
			أرجع ENOENT;
	}

	*ٻاهريون = شروعات + bitmap->شروعات;
	أرجع 0;
}

ساكن errcode_t rb_ڳوليو_پهريون_سيٽ(ext2fs_generic_bitmap_64 bitmap,
				   __u64 شروعات, __u64 پڇاڙي, __u64 *ٻاهريون)
{
	بنية rb_ڳنڍ *والد = عدم, **n;
	بنية rb_ڳنڍ *ڳنڍ;
	بنية ext2fs_rb_ذاتي *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;
	n = &bp->پاڙ.rb_ڳنڍ;
	شروعات -= bitmap->شروعات;
	پڇاڙي -= bitmap->شروعات;

	إذا (شروعات > پڇاڙي)
		أرجع EINVAL;

	إذا (ext2fs_rb_خالي_جايون_پاڙ(&bp->پاڙ))
		أرجع ENOENT;

	طالما (*n) {
		والد = *n;
		ext = ڳنڍ_منزل_extent(والد);
		إذا (شروعات < ext->شروعات) {
			n = &(*n)->rb_کاٻو;
		} وإلا إذا (شروعات >= (ext->شروعات + ext->ڳڻپ)) {
			n = &(*n)->rb_ساڄو;
		} وإلا {
			/* The start bit is set */
			*ٻاهريون = شروعات + bitmap->شروعات;
			أرجع 0;
		}
	}

	ڳنڍ = والد;
	ext = ڳنڍ_منزل_extent(ڳنڍ);
	إذا (ext->شروعات < شروعات) {
		ڳنڍ = ext2fs_rb_ايندڙ(ڳنڍ);
		إذا (ڳنڍ == عدم)
			أرجع ENOENT;
		ext = ڳنڍ_منزل_extent(ڳنڍ);
	}
	إذا (ext->شروعات <= پڇاڙي) {
		*ٻاهريون = ext->شروعات + bitmap->شروعات;
		أرجع 0;
	}
	أرجع ENOENT;
}

#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_ذاتي *bp;
	بنية rb_ڳنڍ *ڳنڍ = عدم;
	بنية bmap_rb_extent *ext;
	__u64 ڳڻپ = 0;
	__u64 max_سائيز = 0;
	__u64 min_سائيز = ULONG_MAX;
	__u64 سائيز = 0, avg_سائيز = 0;
	مزدوج eff;
#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	مزدوج m_ڪاميابي = 0.0, t_ڪاميابي = 0.0;
#جيڪڏهن_پورو

	bp = (بنية ext2fs_rb_ذاتي *) bitmap->خاص;

	لكل (ڳنڍ = ext2fs_rb_پهريون(&bp->پاڙ); ڳنڍ != عدم;
	     ڳنڍ = ext2fs_rb_ايندڙ(ڳنڍ)) {
		ext = ڳنڍ_منزل_extent(ڳنڍ);
		ڳڻپ++;
		إذا (ext->ڳڻپ > max_سائيز)
			max_سائيز = ext->ڳڻپ;
		إذا (ext->ڳڻپ < min_سائيز)
			min_سائيز = ext->ڳڻپ;
		سائيز += ext->ڳڻپ;
	}

	إذا (ڳڻپ)
		avg_سائيز = سائيز / ڳڻپ;
	إذا (min_سائيز == ULONG_MAX)
		min_سائيز = 0;
	eff = (مزدوج)((ڳڻپ * حجم(بنية bmap_rb_extent)) << 3) /
	      (bitmap->real_پڇاڙي - bitmap->شروعات);
#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_ڳڻپ + bitmap->stats.mark_ext_ڳڻپ;
	test_all = bitmap->stats.test_ڳڻپ + bitmap->stats.test_ext_ڳڻپ;
	إذا (mark_all)
		m_ڪاميابي = ((مزدوج)bp->mark_ڪاميابي / mark_all) * 100;
	إذا (test_all)
		t_ڪاميابي = ((مزدوج)bp->test_ڪاميابي / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_ڪاميابي, t_ڪاميابي, bp->mark_ڪاميابي, m_ڪاميابي);
#جيڪڏهن_پورو
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(بنانشان ڊگهو ڊگهو) ڳڻپ, (بنانشان ڊگهو ڊگهو)
		((ڳڻپ * حجم(بنية bmap_rb_extent)) +
		 حجم(بنية ext2fs_rb_ذاتي)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(بنانشان ڊگهو ڊگهو) min_سائيز);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(بنانشان ڊگهو ڊگهو) max_سائيز, (بنانشان ڊگهو ڊگهو) avg_سائيز);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(بنانشان ڊگهو ڊگهو) سائيز,
		(بنانشان ڊگهو ڊگهو) bitmap->real_پڇاڙي - bitmap->شروعات);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#وإلا
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
}
#جيڪڏهن_پورو

بنية ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
	.type = EXT2FS_BMAP64_RBTREE,
	.new_bmap = rb_new_bmap,
	.free_bmap = rb_free_bmap,
	.copy_bmap = rb_copy_bmap,
	.resize_bmap = rb_resize_bmap,
	.mark_bmap = rb_mark_bmap,
	.unmark_bmap = rb_unmark_bmap,
	.test_bmap = rb_test_bmap,
	.test_clear_bmap_extent = rb_test_clear_bmap_extent,
	.mark_bmap_extent = rb_mark_bmap_extent,
	.unmark_bmap_extent = rb_unmark_bmap_extent,
	.سيٽ_bmap_range = rb_سيٽ_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.ڳوليو_پهريون_zero = rb_ڳوليو_پهريون_zero,
	.ڳوليو_پهريون_سيٽ = rb_ڳوليو_پهريون_سيٽ,
};
