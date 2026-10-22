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
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#إذا HAVE_LINUX_TYPES_H
#ضمن <linux/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"
#ضمن "rbtree.h"

#ضمن <limits.h>

بنية bmap_rb_extent {
	بنية rb_گرہ گرہ;
	__u64 شروع;
	__u64 گنتی;
};

بنية ext2fs_rb_نجی {
	بنية rb_جڑ جڑ;
	بنية bmap_rb_extent *wcursor;
	بنية bmap_rb_extent *rcursor;
	بنية bmap_rb_extent *rcursor_اگلا;
#جے_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	__u64 mark_کامیابی;
	__u64 test_کامیابی;
#جے_انت
};

مضمن ساكن بنية bmap_rb_extent *گرہ_منزل_extent(بنية rb_گرہ *گرہ)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	أرجع (بنية bmap_rb_extent *) گرہ;
}

ساكن صحيح rb_داخل_کرو_extent(__u64 شروع, __u64 گنتی,
			    بنية ext2fs_rb_نجی *);
ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#جے_تعریف_شدہ DEBUG_RB
ساكن فراغ print_درخت(بنية rb_جڑ *جڑ)
{
	بنية rb_گرہ *گرہ = عدم;
	بنية bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	گرہ = ext2fs_rb_پہلا(جڑ);
	لكل (گرہ = ext2fs_rb_پہلا(جڑ); گرہ != عدم; 
	     گرہ = ext2fs_rb_اگلا(گرہ)) {
		ext = گرہ_منزل_extent(گرہ);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(بنانشانی لما لما) ext->شروع,
			(بنانشانی لما لما) ext->شروع + ext->گنتی);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساكن فراغ check_درخت(بنية rb_جڑ *جڑ, ثابت حرف *پیغام)
{
	بنية rb_گرہ *گرہ;
	بنية bmap_rb_extent *ext, *old = عدم;

	لكل (گرہ = ext2fs_rb_پہلا(جڑ); گرہ;
	     گرہ = ext2fs_rb_اگلا(گرہ)) {
		ext = گرہ_منزل_extent(گرہ);
		إذا (ext->گنتی == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(بنانشانی لما لما) ext->شروع,
				(بنانشانی لما لما) ext->شروع + ext->گنتی,
				(بنانشانی لما لما) ext->گنتی);
			اذهب err_باہرلا;
		}
		إذا (ext->شروع + ext->گنتی < ext->شروع) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(بنانشانی لما لما) ext->شروع,
				(بنانشانی لما لما) ext->شروع + ext->گنتی,
				(بنانشانی لما لما) ext->گنتی);
			اذهب err_باہرلا;
		}

		إذا (old) {
			إذا (old->شروع > ext->شروع) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(بنانشانی لما لما) old->شروع,
					(بنانشانی لما لما) old->شروع + old->گنتی,
					(بنانشانی لما لما) old->گنتی);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(بنانشانی لما لما) ext->شروع,
					(بنانشانی لما لما) ext->شروع + ext->گنتی,
					(بنانشانی لما لما) ext->گنتی);
				اذهب err_باہرلا;
			}
			إذا ((old->شروع + old->گنتی) >= ext->شروع) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(بنانشانی لما لما) old->شروع,
					(بنانشانی لما لما) old->شروع + old->گنتی,
					(بنانشانی لما لما) old->گنتی);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(بنانشانی لما لما) ext->شروع,
					(بنانشانی لما لما) ext->شروع + ext->گنتی,
					(بنانشانی لما لما) ext->گنتی);
				اذهب err_باہرلا;
			}
		}
		old = ext;
	}
	أرجع;

err_باہرلا:
	fprintf(stderr, "%s\n", پیغام);
	print_درخت(جڑ);
	باہر_نکلو(1);
}
#وإلا
#تعریف_کرو check_درخت(جڑ, پیغام) افعل {} طالما (0)
#تعریف_کرو print_درخت(جڑ) افعل {} طالما (0)
#جے_انت

ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **ext, __u64 شروع,
			     __u64 گنتی)
{
	بنية bmap_rb_extent *new_ext;
	صحيح retval;

	retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
				&new_ext);
	إذا (retval)
		abort();

	new_ext->شروع = شروع;
	new_ext->گنتی = گنتی;
	*ext = new_ext;
}

مضمن
ساكن فراغ rb_free_extent(بنية ext2fs_rb_نجی *bp,
			   بنية bmap_rb_extent *ext)
{
	إذا (bp->wcursor == ext)
		bp->wcursor = عدم;
	إذا (bp->rcursor == ext)
		bp->rcursor = عدم;
	إذا (bp->rcursor_اگلا == ext)
		bp->rcursor_اگلا = عدم;
	ext2fs_free_mem(&ext);
}

ساكن errcode_t rb_alloc_نجی_انکڑے (ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_نجی *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم (بنية ext2fs_rb_نجی), &bp);
	إذا (retval)
		أرجع retval;

	bp->جڑ = RB_جڑ;
	bp->rcursor = عدم;
	bp->rcursor_اگلا = عدم;
	bp->wcursor = عدم;

#جے_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	bp->test_کامیابی = 0;
	bp->mark_کامیابی = 0;
#جے_انت

	bitmap->خاص = (فراغ *) bp;
	أرجع 0;
}

ساكن errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((بن_ورتیا)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_نجی_انکڑے (bitmap);
	إذا (retval)
		أرجع retval;

	أرجع 0;
}

ساكن فراغ rb_free_درخت(بنية rb_جڑ *جڑ)
{
	بنية bmap_rb_extent *ext;
	بنية rb_گرہ *گرہ, *اگلا;

	لكل (گرہ = ext2fs_rb_پہلا(جڑ); گرہ; گرہ = اگلا) {
		اگلا = ext2fs_rb_اگلا(گرہ);
		ext = گرہ_منزل_extent(گرہ);
		ext2fs_rb_erase(گرہ, جڑ);
		ext2fs_free_mem(&ext);
	}
}

ساكن فراغ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_نجی *bp;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;

	rb_free_درخت(&bp->جڑ);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساكن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	بنية ext2fs_rb_نجی *src_bp, *dest_bp;
	بنية bmap_rb_extent *src_ext, *dest_ext;
	بنية rb_گرہ *dest_گرہ, *src_گرہ, *dest_آخری, **n;
	errcode_t retval = 0;

	retval = rb_alloc_نجی_انکڑے (dest);
	إذا (retval)
		أرجع retval;

	src_bp = (بنية ext2fs_rb_نجی *) src->خاص;
	dest_bp = (بنية ext2fs_rb_نجی *) dest->خاص;
	src_bp->rcursor = عدم;
	dest_bp->rcursor = عدم;

	src_گرہ = ext2fs_rb_پہلا(&src_bp->جڑ);
	طالما (src_گرہ) {
		src_ext = گرہ_منزل_extent(src_گرہ);
		retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
					&dest_ext);
		إذا (retval)
			اكسر;

		حافظہ_نقل(dest_ext, src_ext, حجم(بنية bmap_rb_extent));

		dest_گرہ = &dest_ext->گرہ;
		n = &dest_bp->جڑ.rb_گرہ;

		dest_آخری = عدم;
		إذا (*n) {
			dest_آخری = ext2fs_rb_آخری(&dest_bp->جڑ);
			n = &(dest_آخری)->rb_دایاں;
		}

		ext2fs_rb_link_گرہ(dest_گرہ, dest_آخری, n);
		ext2fs_rb_داخل_کرو_color(dest_گرہ, &dest_bp->جڑ);

		src_گرہ = ext2fs_rb_اگلا(src_گرہ);
	}

	أرجع retval;
}

ساكن فراغ rb_truncate(__u64 new_max, بنية rb_جڑ *جڑ)
{
	بنية bmap_rb_extent *ext;
	بنية rb_گرہ *گرہ;

	گرہ = ext2fs_rb_آخری(جڑ);
	طالما (گرہ) {
		ext = گرہ_منزل_extent(گرہ);

		إذا ((ext->شروع + ext->گنتی - 1) <= new_max)
			اكسر;
		وإلا إذا (ext->شروع > new_max) {
			ext2fs_rb_erase(گرہ, جڑ);
			ext2fs_free_mem(&ext);
			گرہ = ext2fs_rb_آخری(جڑ);
			استمر;
		} وإلا
			ext->گنتی = new_max - ext->شروع + 1;
	}
}

ساكن errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_سرا, __u64 new_real_سرا)
{
	بنية ext2fs_rb_نجی *bp;

	bp = (بنية ext2fs_rb_نجی *) bmap->خاص;
	bp->rcursor = عدم;
	bp->wcursor = عدم;

	rb_truncate(((new_سرا < bmap->سرا) ? new_سرا : bmap->سرا) - bmap->شروع,
		    &bp->جڑ);

	bmap->سرا = new_سرا;
	bmap->real_سرا = new_real_سرا;

	إذا (bmap->سرا < bmap->real_سرا)
		rb_داخل_کرو_extent(bmap->سرا + 1 - bmap->شروع,
				 bmap->real_سرا - bmap->سرا, bp);
	أرجع 0;

}

مضمن ساكن صحيح
rb_test_ثنائی_ہندسہ(بنية ext2fs_rb_نجی *bp, __u64 ثنائی_ہندسہ)
{
	بنية bmap_rb_extent *rcursor, *اگلا_ext = عدم;
	بنية rb_گرہ *ماپا = عدم, *اگلا;
	بنية rb_گرہ **n = &bp->جڑ.rb_گرہ;
	بنية bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	إذا (!rcursor)
		اذهب تلاش_درخت;

	إذا (ثنائی_ہندسہ >= rcursor->شروع && ثنائی_ہندسہ < rcursor->شروع + rcursor->گنتی) {
#جے_تعریف_شدہ ENABLE_BMAP_STATS_OPS
		bp->test_کامیابی++;
#جے_انت
		أرجع 1;
	}

	اگلا_ext = bp->rcursor_اگلا;
	إذا (!اگلا_ext) {
		اگلا = ext2fs_rb_اگلا(&rcursor->گرہ);
		إذا (اگلا)
			اگلا_ext = گرہ_منزل_extent(اگلا);
		bp->rcursor_اگلا = اگلا_ext;
	}
	إذا (اگلا_ext) {
		إذا ((ثنائی_ہندسہ >= rcursor->شروع + rcursor->گنتی) &&
		    (ثنائی_ہندسہ < اگلا_ext->شروع)) {
#جے_تعریف_شدہ BMAP_STATS_OPS
			bp->test_کامیابی++;
#جے_انت
			أرجع 0;
		}
	}
	bp->rcursor = عدم;
	bp->rcursor_اگلا = عدم;

	rcursor = bp->wcursor;
	إذا (!rcursor)
		اذهب تلاش_درخت;

	إذا (ثنائی_ہندسہ >= rcursor->شروع && ثنائی_ہندسہ < rcursor->شروع + rcursor->گنتی)
		أرجع 1;

تلاش_درخت:

	طالما (*n) {
		ماپا = *n;
		ext = گرہ_منزل_extent(ماپا);
		إذا (ثنائی_ہندسہ < ext->شروع)
			n = &(*n)->rb_بایاں;
		وإلا إذا (ثنائی_ہندسہ >= (ext->شروع + ext->گنتی))
			n = &(*n)->rb_دایاں;
		وإلا {
			bp->rcursor = ext;
			bp->rcursor_اگلا = عدم;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح rb_داخل_کرو_extent(__u64 شروع, __u64 گنتی,
			    بنية ext2fs_rb_نجی *bp)
{
	بنية rb_جڑ *جڑ = &bp->جڑ;
	بنية rb_گرہ *ماپا = عدم, **n = &جڑ->rb_گرہ;
	بنية rb_گرہ *new_گرہ, *گرہ, *اگلا;
	بنية bmap_rb_extent *new_ext;
	بنية bmap_rb_extent *ext;
	صحيح retval = 0;

	إذا (گنتی == 0)
		أرجع 0;

	bp->rcursor_اگلا = عدم;
	ext = bp->wcursor;
	إذا (ext) {
		إذا (شروع >= ext->شروع &&
		    شروع <= (ext->شروع + ext->گنتی)) {
#جے_تعریف_شدہ ENABLE_BMAP_STATS_OPS
			bp->mark_کامیابی++;
#جے_انت
			اذهب got_extent;
		}
	}

	طالما (*n) {
		ماپا = *n;
		ext = گرہ_منزل_extent(ماپا);

		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_بایاں;
		} وإلا إذا (شروع > (ext->شروع + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} وإلا {
got_extent:
			إذا ((شروع + گنتی) <= (ext->شروع + ext->گنتی))
				أرجع 1;

			إذا ((ext->شروع + ext->گنتی) == شروع)
				retval = 0;
			وإلا
				retval = 1;

			گنتی += (شروع - ext->شروع);
			شروع = ext->شروع;
			new_ext = ext;
			new_گرہ = &ext->گرہ;

			اذهب skip_داخل_کرو;
		}
	}

	rb_get_new_extent(&new_ext, شروع, گنتی);

	new_گرہ = &new_ext->گرہ;
	ext2fs_rb_link_گرہ(new_گرہ, ماپا, n);
	ext2fs_rb_داخل_کرو_color(new_گرہ, جڑ);
	bp->wcursor = new_ext;

	گرہ = ext2fs_rb_prev(new_گرہ);
	إذا (گرہ) {
		ext = گرہ_منزل_extent(گرہ);
		إذا ((ext->شروع + ext->گنتی) == شروع) {
			شروع = ext->شروع;
			گنتی += ext->گنتی;
			ext2fs_rb_erase(گرہ, جڑ);
			rb_free_extent(bp, ext);
		}
	}

skip_داخل_کرو:
	/* See if we can merge extent to the right */
	لكل (گرہ = ext2fs_rb_اگلا(new_گرہ); گرہ != عدم; گرہ = اگلا) {
		اگلا = ext2fs_rb_اگلا(گرہ);
		ext = گرہ_منزل_extent(گرہ);

		إذا ((ext->شروع + ext->گنتی) <= شروع)
			استمر;

		/* No more merging */
		إذا ((شروع + گنتی) < ext->شروع)
			اكسر;

		/* ext is embedded in new_ext interval */
		إذا ((شروع + گنتی) >= (ext->شروع + ext->گنتی)) {
			ext2fs_rb_erase(گرہ, جڑ);
			rb_free_extent(bp, ext);
			استمر;
		} وإلا {
		/* merge ext with new_ext */
			گنتی += ((ext->شروع + ext->گنتی) -
				  (شروع + گنتی));
			ext2fs_rb_erase(گرہ, جڑ);
			rb_free_extent(bp, ext);
			اكسر;
		}
	}

	new_ext->شروع = شروع;
	new_ext->گنتی = گنتی;

	أرجع retval;
}

ساكن صحيح rb_remove_extent(__u64 شروع, __u64 گنتی,
			    بنية ext2fs_rb_نجی *bp)
{
	بنية rb_جڑ *جڑ = &bp->جڑ;
	بنية rb_گرہ *ماپا = عدم, **n = &جڑ->rb_گرہ;
	بنية rb_گرہ *گرہ;
	بنية bmap_rb_extent *ext;
	__u64 new_شروع, new_گنتی;
	صحيح retval = 0;

	إذا (ext2fs_rb_خالی_تھاواں_جڑ(جڑ))
		أرجع 0;

	طالما (*n) {
		ماپا = *n;
		ext = گرہ_منزل_extent(ماپا);
		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_بایاں;
			استمر;
		} وإلا إذا (شروع >= (ext->شروع + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
			استمر;
		}

		إذا ((شروع > ext->شروع) &&
		    (شروع + گنتی) < (ext->شروع + ext->گنتی)) {
			/* We have to split extent into two */
			new_شروع = شروع + گنتی;
			new_گنتی = (ext->شروع + ext->گنتی) - new_شروع;

			ext->گنتی = شروع - ext->شروع;

			rb_داخل_کرو_extent(new_شروع, new_گنتی, bp);
			أرجع 1;
		}

		إذا ((شروع + گنتی) >= (ext->شروع + ext->گنتی)) {
			ext->گنتی = شروع - ext->شروع;
			retval = 1;
		}

		إذا (0 == ext->گنتی) {
			ماپا = ext2fs_rb_اگلا(&ext->گرہ);
			ext2fs_rb_erase(&ext->گرہ, جڑ);
			rb_free_extent(bp, ext);
			اكسر;
		}

		إذا (شروع == ext->شروع) {
			ext->شروع += گنتی;
			ext->گنتی -= گنتی;
			أرجع 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	لكل (; ماپا != عدم; ماپا = گرہ) {
		گرہ = ext2fs_rb_اگلا(ماپا);
		ext = گرہ_منزل_extent(ماپا);
		إذا ((ext->شروع + ext->گنتی) <= شروع)
			استمر;

		/* No more extents to be removed/truncated */
		إذا ((شروع + گنتی) < ext->شروع)
			اكسر;

		/* The entire extent is within the region to be removed */
		إذا ((شروع + گنتی) >= (ext->شروع + ext->گنتی)) {
			ext2fs_rb_erase(ماپا, جڑ);
			rb_free_extent(bp, ext);
			retval = 1;
			استمر;
		} وإلا {
			/* modify the last extent in region to be removed */
			ext->گنتی -= ((شروع + گنتی) - ext->شروع);
			ext->شروع = شروع + گنتی;
			retval = 1;
			اكسر;
		}
	}

	أرجع retval;
}

ساكن صحيح rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_نجی *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;
	arg -= bitmap->شروع;

	retval = rb_داخل_کرو_extent(arg, 1, bp);
	check_درخت(&bp->جڑ, __func__);
	أرجع retval;
}

ساكن صحيح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_نجی *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;
	arg -= bitmap->شروع;

	retval = rb_remove_extent(arg, 1, bp);
	check_درخت(&bp->جڑ, __func__);

	أرجع retval;
}

مضمن
ساكن صحيح rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_نجی *bp;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;
	arg -= bitmap->شروع;

	أرجع rb_test_ثنائی_ہندسہ(bp, arg);
}

ساكن فراغ rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				بنانشانی صحيح عدد)
{
	بنية ext2fs_rb_نجی *bp;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;
	arg -= bitmap->شروع;

	rb_داخل_کرو_extent(arg, عدد, bp);
	check_درخت(&bp->جڑ, __func__);
}

ساكن فراغ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  بنانشانی صحيح عدد)
{
	بنية ext2fs_rb_نجی *bp;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;
	arg -= bitmap->شروع;

	rb_remove_extent(arg, عدد, bp);
	check_درخت(&bp->جڑ, __func__);
}

ساكن صحيح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, بنانشانی صحيح len)
{
	بنية rb_گرہ *ماپا = عدم, **n;
	بنية rb_گرہ *گرہ, *اگلا;
	بنية ext2fs_rb_نجی *bp;
	بنية bmap_rb_extent *ext;
	صحيح retval = 1;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;
	n = &bp->جڑ.rb_گرہ;
	شروع -= bitmap->شروع;

	إذا (len == 0 || ext2fs_rb_خالی_تھاواں_جڑ(&bp->جڑ))
		أرجع 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	طالما (*n) {
		ماپا = *n;
		ext = گرہ_منزل_extent(ماپا);
		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_بایاں;
		} وإلا إذا (شروع >= (ext->شروع + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} وإلا {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			أرجع 0;
		}
	}

	گرہ = ماپا;
	طالما (گرہ) {
		اگلا = ext2fs_rb_اگلا(گرہ);
		ext = گرہ_منزل_extent(گرہ);
		گرہ = اگلا;

		إذا ((ext->شروع + ext->گنتی) <= شروع)
			استمر;

		/* No more merging */
		إذا ((شروع + len) <= ext->شروع)
			اكسر;

		retval = 0;
		اكسر;
	}
	أرجع retval;
}

ساكن errcode_t rb_سیٹ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, size_t عدد, فراغ *وچ)
{
	بنية ext2fs_rb_نجی *bp;
	بنانشانی حرف *cp = وچ;
	size_t i;
	صحيح پہلا_سیٹ = -1;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;

	لكل (i = 0; i < عدد; i++) {
		إذا ((i & 7) == 0) {
			بنانشانی حرف c = cp[i/8];
			إذا (c == 0xFF) {
				إذا (پہلا_سیٹ == -1)
					پہلا_سیٹ = i;
				i += 7;
				استمر;
			}
			إذا ((c == 0x00) && (پہلا_سیٹ == -1)) {
				i += 7;
				استمر;
			}
		}
		إذا (ext2fs_test_ثنائی_ہندسہ(i, وچ)) {
			إذا (پہلا_سیٹ == -1)
				پہلا_سیٹ = i;
			استمر;
		}
		إذا (پہلا_سیٹ == -1)
			استمر;

		rb_داخل_کرو_extent(شروع + پہلا_سیٹ - bitmap->شروع,
				 i - پہلا_سیٹ, bp);
		check_درخت(&bp->جڑ, __func__);
		پہلا_سیٹ = -1;
	}
	إذا (پہلا_سیٹ != -1) {
		rb_داخل_کرو_extent(شروع + پہلا_سیٹ - bitmap->شروع,
				 عدد - پہلا_سیٹ, bp);
		check_درخت(&bp->جڑ, __func__);
	}

	أرجع 0;
}

ساكن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, size_t عدد, فراغ *باہرلا)
{

	بنية rb_گرہ *ماپا = عدم, *اگلا, **n;
	بنية ext2fs_rb_نجی *bp;
	بنية bmap_rb_extent *ext;
	__u64 گنتی, pos;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;
	n = &bp->جڑ.rb_گرہ;
	شروع -= bitmap->شروع;

	إذا (ext2fs_rb_خالی_تھاواں_جڑ(&bp->جڑ))
		أرجع 0;

	طالما (*n) {
		ماپا = *n;
		ext = گرہ_منزل_extent(ماپا);
		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_بایاں;
		} وإلا إذا (شروع >= (ext->شروع + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} وإلا
			اكسر;
	}

	memset(باہرلا, 0, (عدد + 7) >> 3);

	لكل (; ماپا != عدم; ماپا = اگلا) {
		اگلا = ext2fs_rb_اگلا(ماپا);
		ext = گرہ_منزل_extent(ماپا);

		pos = ext->شروع;
		گنتی = ext->گنتی;
		إذا (pos >= شروع + عدد)
			اكسر;
		إذا (pos < شروع) {
			إذا (pos + گنتی <  شروع)
				استمر;
			گنتی -= شروع - pos;
			pos = شروع;
		}
		إذا (pos + گنتی > شروع + عدد)
			گنتی = شروع + عدد - pos;

		طالما (گنتی > 0) {
			إذا ((گنتی >= 8) &&
			    ((pos - شروع) % 8) == 0) {
				صحيح nbytes = گنتی >> 3;
				صحيح وتھ = (pos - شروع) >> 3;

				memset(((حرف *) باہرلا) + وتھ, 0xFF, nbytes);
				pos += nbytes << 3;
				گنتی -= nbytes << 3;
				استمر;
			}
			ext2fs_fast_سیٹ_bit64((pos - شروع), باہرلا);
			pos++;
			گنتی--;
		}
	}
	أرجع 0;
}

ساكن فراغ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_نجی *bp;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;

	rb_free_درخت(&bp->جڑ);
	bp->rcursor = عدم;
	bp->rcursor_اگلا = عدم;
	bp->wcursor = عدم;
	check_درخت(&bp->جڑ, __func__);
}

ساكن errcode_t rb_لبھو_پہلا_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 شروع, __u64 سرا, __u64 *باہرلا)
{
	بنية rb_گرہ *ماپا = عدم, **n;
	بنية ext2fs_rb_نجی *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;
	n = &bp->جڑ.rb_گرہ;
	شروع -= bitmap->شروع;
	سرا -= bitmap->شروع;

	إذا (شروع > سرا)
		أرجع EINVAL;

	إذا (ext2fs_rb_خالی_تھاواں_جڑ(&bp->جڑ))
		أرجع ENOENT;

	طالما (*n) {
		ماپا = *n;
		ext = گرہ_منزل_extent(ماپا);
		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_بایاں;
		} وإلا إذا (شروع >= (ext->شروع + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} وإلا إذا (ext->شروع + ext->گنتی <= سرا) {
			*باہرلا = ext->شروع + ext->گنتی + bitmap->شروع;
			أرجع 0;
		} وإلا
			أرجع ENOENT;
	}

	*باہرلا = شروع + bitmap->شروع;
	أرجع 0;
}

ساكن errcode_t rb_لبھو_پہلا_سیٹ(ext2fs_generic_bitmap_64 bitmap,
				   __u64 شروع, __u64 سرا, __u64 *باہرلا)
{
	بنية rb_گرہ *ماپا = عدم, **n;
	بنية rb_گرہ *گرہ;
	بنية ext2fs_rb_نجی *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;
	n = &bp->جڑ.rb_گرہ;
	شروع -= bitmap->شروع;
	سرا -= bitmap->شروع;

	إذا (شروع > سرا)
		أرجع EINVAL;

	إذا (ext2fs_rb_خالی_تھاواں_جڑ(&bp->جڑ))
		أرجع ENOENT;

	طالما (*n) {
		ماپا = *n;
		ext = گرہ_منزل_extent(ماپا);
		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_بایاں;
		} وإلا إذا (شروع >= (ext->شروع + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} وإلا {
			/* The start bit is set */
			*باہرلا = شروع + bitmap->شروع;
			أرجع 0;
		}
	}

	گرہ = ماپا;
	ext = گرہ_منزل_extent(گرہ);
	إذا (ext->شروع < شروع) {
		گرہ = ext2fs_rb_اگلا(گرہ);
		إذا (گرہ == عدم)
			أرجع ENOENT;
		ext = گرہ_منزل_extent(گرہ);
	}
	إذا (ext->شروع <= سرا) {
		*باہرلا = ext->شروع + bitmap->شروع;
		أرجع 0;
	}
	أرجع ENOENT;
}

#جے_تعریف_شدہ ENABLE_BMAP_STATS
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_نجی *bp;
	بنية rb_گرہ *گرہ = عدم;
	بنية bmap_rb_extent *ext;
	__u64 گنتی = 0;
	__u64 max_ناپ = 0;
	__u64 min_ناپ = ULONG_MAX;
	__u64 ناپ = 0, avg_ناپ = 0;
	مزدوج eff;
#جے_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	مزدوج m_کامیابی = 0.0, t_کامیابی = 0.0;
#جے_انت

	bp = (بنية ext2fs_rb_نجی *) bitmap->خاص;

	لكل (گرہ = ext2fs_rb_پہلا(&bp->جڑ); گرہ != عدم;
	     گرہ = ext2fs_rb_اگلا(گرہ)) {
		ext = گرہ_منزل_extent(گرہ);
		گنتی++;
		إذا (ext->گنتی > max_ناپ)
			max_ناپ = ext->گنتی;
		إذا (ext->گنتی < min_ناپ)
			min_ناپ = ext->گنتی;
		ناپ += ext->گنتی;
	}

	إذا (گنتی)
		avg_ناپ = ناپ / گنتی;
	إذا (min_ناپ == ULONG_MAX)
		min_ناپ = 0;
	eff = (مزدوج)((گنتی * حجم(بنية bmap_rb_extent)) << 3) /
	      (bitmap->real_سرا - bitmap->شروع);
#جے_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_گنتی + bitmap->stats.mark_ext_گنتی;
	test_all = bitmap->stats.test_گنتی + bitmap->stats.test_ext_گنتی;
	إذا (mark_all)
		m_کامیابی = ((مزدوج)bp->mark_کامیابی / mark_all) * 100;
	إذا (test_all)
		t_کامیابی = ((مزدوج)bp->test_کامیابی / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_کامیابی, t_کامیابی, bp->mark_کامیابی, m_کامیابی);
#جے_انت
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(بنانشانی لما لما) گنتی, (بنانشانی لما لما)
		((گنتی * حجم(بنية bmap_rb_extent)) +
		 حجم(بنية ext2fs_rb_نجی)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(بنانشانی لما لما) min_ناپ);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(بنانشانی لما لما) max_ناپ, (بنانشانی لما لما) avg_ناپ);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(بنانشانی لما لما) ناپ,
		(بنانشانی لما لما) bitmap->real_سرا - bitmap->شروع);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#وإلا
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((بن_ورتیا)))
{
}
#جے_انت

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
	.سیٹ_bmap_range = rb_سیٹ_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.لبھو_پہلا_zero = rb_لبھو_پہلا_zero,
	.لبھو_پہلا_سیٹ = rb_لبھو_پہلا_سیٹ,
};
