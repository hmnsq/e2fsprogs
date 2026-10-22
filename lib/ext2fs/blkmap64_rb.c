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
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر
#إذا HAVE_LINUX_TYPES_H
#ضمن <linux/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"
#ضمن "rbtree.h"

#ضمن <limits.h>

بنية bmap_rb_extent {
	بنية rb_گره گره;
	__u64 شروع;
	__u64 شمار;
};

بنية ext2fs_rb_خصوصی {
	بنية rb_ریشه ریشه;
	بنية bmap_rb_extent *wcursor;
	بنية bmap_rb_extent *rcursor;
	بنية bmap_rb_extent *rcursor_بعدی;
#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
	__u64 mark_اصابت;
	__u64 test_اصابت;
#پایان_اگر
};

مضمن ساكن بنية bmap_rb_extent *گره_مقصد_extent(بنية rb_گره *گره)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	أرجع (بنية bmap_rb_extent *) گره;
}

ساكن صحيح rb_درج_extent(__u64 شروع, __u64 شمار,
			    بنية ext2fs_rb_خصوصی *);
ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#اگر_تعریف_شده DEBUG_RB
ساكن فراغ print_درخت(بنية rb_ریشه *ریشه)
{
	بنية rb_گره *گره = عدم;
	بنية bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	گره = ext2fs_rb_اولی(ریشه);
	لكل (گره = ext2fs_rb_اولی(ریشه); گره != عدم; 
	     گره = ext2fs_rb_بعدی(گره)) {
		ext = گره_مقصد_extent(گره);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(بیعلامت دراز دراز) ext->شروع,
			(بیعلامت دراز دراز) ext->شروع + ext->شمار);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساكن فراغ check_درخت(بنية rb_ریشه *ریشه, ثابت حرف *پیام)
{
	بنية rb_گره *گره;
	بنية bmap_rb_extent *ext, *old = عدم;

	لكل (گره = ext2fs_rb_اولی(ریشه); گره;
	     گره = ext2fs_rb_بعدی(گره)) {
		ext = گره_مقصد_extent(گره);
		إذا (ext->شمار == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(بیعلامت دراز دراز) ext->شروع,
				(بیعلامت دراز دراز) ext->شروع + ext->شمار,
				(بیعلامت دراز دراز) ext->شمار);
			اذهب err_بیرون;
		}
		إذا (ext->شروع + ext->شمار < ext->شروع) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(بیعلامت دراز دراز) ext->شروع,
				(بیعلامت دراز دراز) ext->شروع + ext->شمار,
				(بیعلامت دراز دراز) ext->شمار);
			اذهب err_بیرون;
		}

		إذا (old) {
			إذا (old->شروع > ext->شروع) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(بیعلامت دراز دراز) old->شروع,
					(بیعلامت دراز دراز) old->شروع + old->شمار,
					(بیعلامت دراز دراز) old->شمار);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(بیعلامت دراز دراز) ext->شروع,
					(بیعلامت دراز دراز) ext->شروع + ext->شمار,
					(بیعلامت دراز دراز) ext->شمار);
				اذهب err_بیرون;
			}
			إذا ((old->شروع + old->شمار) >= ext->شروع) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(بیعلامت دراز دراز) old->شروع,
					(بیعلامت دراز دراز) old->شروع + old->شمار,
					(بیعلامت دراز دراز) old->شمار);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(بیعلامت دراز دراز) ext->شروع,
					(بیعلامت دراز دراز) ext->شروع + ext->شمار,
					(بیعلامت دراز دراز) ext->شمار);
				اذهب err_بیرون;
			}
		}
		old = ext;
	}
	أرجع;

err_بیرون:
	fprintf(stderr, "%s\n", پیام);
	print_درخت(ریشه);
	خروج(1);
}
#وإلا
#تعریف_کن check_درخت(ریشه, پیام) افعل {} طالما (0)
#تعریف_کن print_درخت(ریشه) افعل {} طالما (0)
#پایان_اگر

ساكن فراغ rb_get_new_extent(بنية bmap_rb_extent **ext, __u64 شروع,
			     __u64 شمار)
{
	بنية bmap_rb_extent *new_ext;
	صحيح retval;

	retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
				&new_ext);
	إذا (retval)
		abort();

	new_ext->شروع = شروع;
	new_ext->شمار = شمار;
	*ext = new_ext;
}

مضمن
ساكن فراغ rb_free_extent(بنية ext2fs_rb_خصوصی *bp,
			   بنية bmap_rb_extent *ext)
{
	إذا (bp->wcursor == ext)
		bp->wcursor = عدم;
	إذا (bp->rcursor == ext)
		bp->rcursor = عدم;
	إذا (bp->rcursor_بعدی == ext)
		bp->rcursor_بعدی = عدم;
	ext2fs_free_mem(&ext);
}

ساكن errcode_t rb_alloc_خصوصی_داده (ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_خصوصی *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم (بنية ext2fs_rb_خصوصی), &bp);
	إذا (retval)
		أرجع retval;

	bp->ریشه = RB_ریشه;
	bp->rcursor = عدم;
	bp->rcursor_بعدی = عدم;
	bp->wcursor = عدم;

#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
	bp->test_اصابت = 0;
	bp->mark_اصابت = 0;
#پایان_اگر

	bitmap->خاص = (فراغ *) bp;
	أرجع 0;
}

ساكن errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((استفادهنشده)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_خصوصی_داده (bitmap);
	إذا (retval)
		أرجع retval;

	أرجع 0;
}

ساكن فراغ rb_free_درخت(بنية rb_ریشه *ریشه)
{
	بنية bmap_rb_extent *ext;
	بنية rb_گره *گره, *بعدی;

	لكل (گره = ext2fs_rb_اولی(ریشه); گره; گره = بعدی) {
		بعدی = ext2fs_rb_بعدی(گره);
		ext = گره_مقصد_extent(گره);
		ext2fs_rb_erase(گره, ریشه);
		ext2fs_free_mem(&ext);
	}
}

ساكن فراغ rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_خصوصی *bp;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;

	rb_free_درخت(&bp->ریشه);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساكن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	بنية ext2fs_rb_خصوصی *src_bp, *dest_bp;
	بنية bmap_rb_extent *src_ext, *dest_ext;
	بنية rb_گره *dest_گره, *src_گره, *dest_آخرین, **n;
	errcode_t retval = 0;

	retval = rb_alloc_خصوصی_داده (dest);
	إذا (retval)
		أرجع retval;

	src_bp = (بنية ext2fs_rb_خصوصی *) src->خاص;
	dest_bp = (بنية ext2fs_rb_خصوصی *) dest->خاص;
	src_bp->rcursor = عدم;
	dest_bp->rcursor = عدم;

	src_گره = ext2fs_rb_اولی(&src_bp->ریشه);
	طالما (src_گره) {
		src_ext = گره_مقصد_extent(src_گره);
		retval = ext2fs_get_mem(حجم (بنية bmap_rb_extent),
					&dest_ext);
		إذا (retval)
			اكسر;

		کپی_حافظه(dest_ext, src_ext, حجم(بنية bmap_rb_extent));

		dest_گره = &dest_ext->گره;
		n = &dest_bp->ریشه.rb_گره;

		dest_آخرین = عدم;
		إذا (*n) {
			dest_آخرین = ext2fs_rb_آخرین(&dest_bp->ریشه);
			n = &(dest_آخرین)->rb_راست;
		}

		ext2fs_rb_link_گره(dest_گره, dest_آخرین, n);
		ext2fs_rb_درج_color(dest_گره, &dest_bp->ریشه);

		src_گره = ext2fs_rb_بعدی(src_گره);
	}

	أرجع retval;
}

ساكن فراغ rb_truncate(__u64 new_max, بنية rb_ریشه *ریشه)
{
	بنية bmap_rb_extent *ext;
	بنية rb_گره *گره;

	گره = ext2fs_rb_آخرین(ریشه);
	طالما (گره) {
		ext = گره_مقصد_extent(گره);

		إذا ((ext->شروع + ext->شمار - 1) <= new_max)
			اكسر;
		وإلا إذا (ext->شروع > new_max) {
			ext2fs_rb_erase(گره, ریشه);
			ext2fs_free_mem(&ext);
			گره = ext2fs_rb_آخرین(ریشه);
			استمر;
		} وإلا
			ext->شمار = new_max - ext->شروع + 1;
	}
}

ساكن errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_انتها, __u64 new_real_انتها)
{
	بنية ext2fs_rb_خصوصی *bp;

	bp = (بنية ext2fs_rb_خصوصی *) bmap->خاص;
	bp->rcursor = عدم;
	bp->wcursor = عدم;

	rb_truncate(((new_انتها < bmap->انتها) ? new_انتها : bmap->انتها) - bmap->شروع,
		    &bp->ریشه);

	bmap->انتها = new_انتها;
	bmap->real_انتها = new_real_انتها;

	إذا (bmap->انتها < bmap->real_انتها)
		rb_درج_extent(bmap->انتها + 1 - bmap->شروع,
				 bmap->real_انتها - bmap->انتها, bp);
	أرجع 0;

}

مضمن ساكن صحيح
rb_test_بیت(بنية ext2fs_rb_خصوصی *bp, __u64 بیت)
{
	بنية bmap_rb_extent *rcursor, *بعدی_ext = عدم;
	بنية rb_گره *والد = عدم, *بعدی;
	بنية rb_گره **n = &bp->ریشه.rb_گره;
	بنية bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	إذا (!rcursor)
		اذهب درخت_جستجو;

	إذا (بیت >= rcursor->شروع && بیت < rcursor->شروع + rcursor->شمار) {
#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
		bp->test_اصابت++;
#پایان_اگر
		أرجع 1;
	}

	بعدی_ext = bp->rcursor_بعدی;
	إذا (!بعدی_ext) {
		بعدی = ext2fs_rb_بعدی(&rcursor->گره);
		إذا (بعدی)
			بعدی_ext = گره_مقصد_extent(بعدی);
		bp->rcursor_بعدی = بعدی_ext;
	}
	إذا (بعدی_ext) {
		إذا ((بیت >= rcursor->شروع + rcursor->شمار) &&
		    (بیت < بعدی_ext->شروع)) {
#اگر_تعریف_شده BMAP_STATS_OPS
			bp->test_اصابت++;
#پایان_اگر
			أرجع 0;
		}
	}
	bp->rcursor = عدم;
	bp->rcursor_بعدی = عدم;

	rcursor = bp->wcursor;
	إذا (!rcursor)
		اذهب درخت_جستجو;

	إذا (بیت >= rcursor->شروع && بیت < rcursor->شروع + rcursor->شمار)
		أرجع 1;

درخت_جستجو:

	طالما (*n) {
		والد = *n;
		ext = گره_مقصد_extent(والد);
		إذا (بیت < ext->شروع)
			n = &(*n)->rb_چپ;
		وإلا إذا (بیت >= (ext->شروع + ext->شمار))
			n = &(*n)->rb_راست;
		وإلا {
			bp->rcursor = ext;
			bp->rcursor_بعدی = عدم;
			أرجع 1;
		}
	}
	أرجع 0;
}

ساكن صحيح rb_درج_extent(__u64 شروع, __u64 شمار,
			    بنية ext2fs_rb_خصوصی *bp)
{
	بنية rb_ریشه *ریشه = &bp->ریشه;
	بنية rb_گره *والد = عدم, **n = &ریشه->rb_گره;
	بنية rb_گره *new_گره, *گره, *بعدی;
	بنية bmap_rb_extent *new_ext;
	بنية bmap_rb_extent *ext;
	صحيح retval = 0;

	إذا (شمار == 0)
		أرجع 0;

	bp->rcursor_بعدی = عدم;
	ext = bp->wcursor;
	إذا (ext) {
		إذا (شروع >= ext->شروع &&
		    شروع <= (ext->شروع + ext->شمار)) {
#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
			bp->mark_اصابت++;
#پایان_اگر
			اذهب got_extent;
		}
	}

	طالما (*n) {
		والد = *n;
		ext = گره_مقصد_extent(والد);

		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_چپ;
		} وإلا إذا (شروع > (ext->شروع + ext->شمار)) {
			n = &(*n)->rb_راست;
		} وإلا {
got_extent:
			إذا ((شروع + شمار) <= (ext->شروع + ext->شمار))
				أرجع 1;

			إذا ((ext->شروع + ext->شمار) == شروع)
				retval = 0;
			وإلا
				retval = 1;

			شمار += (شروع - ext->شروع);
			شروع = ext->شروع;
			new_ext = ext;
			new_گره = &ext->گره;

			اذهب skip_درج;
		}
	}

	rb_get_new_extent(&new_ext, شروع, شمار);

	new_گره = &new_ext->گره;
	ext2fs_rb_link_گره(new_گره, والد, n);
	ext2fs_rb_درج_color(new_گره, ریشه);
	bp->wcursor = new_ext;

	گره = ext2fs_rb_prev(new_گره);
	إذا (گره) {
		ext = گره_مقصد_extent(گره);
		إذا ((ext->شروع + ext->شمار) == شروع) {
			شروع = ext->شروع;
			شمار += ext->شمار;
			ext2fs_rb_erase(گره, ریشه);
			rb_free_extent(bp, ext);
		}
	}

skip_درج:
	/* See if we can merge extent to the right */
	لكل (گره = ext2fs_rb_بعدی(new_گره); گره != عدم; گره = بعدی) {
		بعدی = ext2fs_rb_بعدی(گره);
		ext = گره_مقصد_extent(گره);

		إذا ((ext->شروع + ext->شمار) <= شروع)
			استمر;

		/* No more merging */
		إذا ((شروع + شمار) < ext->شروع)
			اكسر;

		/* ext is embedded in new_ext interval */
		إذا ((شروع + شمار) >= (ext->شروع + ext->شمار)) {
			ext2fs_rb_erase(گره, ریشه);
			rb_free_extent(bp, ext);
			استمر;
		} وإلا {
		/* merge ext with new_ext */
			شمار += ((ext->شروع + ext->شمار) -
				  (شروع + شمار));
			ext2fs_rb_erase(گره, ریشه);
			rb_free_extent(bp, ext);
			اكسر;
		}
	}

	new_ext->شروع = شروع;
	new_ext->شمار = شمار;

	أرجع retval;
}

ساكن صحيح rb_remove_extent(__u64 شروع, __u64 شمار,
			    بنية ext2fs_rb_خصوصی *bp)
{
	بنية rb_ریشه *ریشه = &bp->ریشه;
	بنية rb_گره *والد = عدم, **n = &ریشه->rb_گره;
	بنية rb_گره *گره;
	بنية bmap_rb_extent *ext;
	__u64 new_شروع, new_شمار;
	صحيح retval = 0;

	إذا (ext2fs_rb_خالی_ریشه(ریشه))
		أرجع 0;

	طالما (*n) {
		والد = *n;
		ext = گره_مقصد_extent(والد);
		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_چپ;
			استمر;
		} وإلا إذا (شروع >= (ext->شروع + ext->شمار)) {
			n = &(*n)->rb_راست;
			استمر;
		}

		إذا ((شروع > ext->شروع) &&
		    (شروع + شمار) < (ext->شروع + ext->شمار)) {
			/* We have to split extent into two */
			new_شروع = شروع + شمار;
			new_شمار = (ext->شروع + ext->شمار) - new_شروع;

			ext->شمار = شروع - ext->شروع;

			rb_درج_extent(new_شروع, new_شمار, bp);
			أرجع 1;
		}

		إذا ((شروع + شمار) >= (ext->شروع + ext->شمار)) {
			ext->شمار = شروع - ext->شروع;
			retval = 1;
		}

		إذا (0 == ext->شمار) {
			والد = ext2fs_rb_بعدی(&ext->گره);
			ext2fs_rb_erase(&ext->گره, ریشه);
			rb_free_extent(bp, ext);
			اكسر;
		}

		إذا (شروع == ext->شروع) {
			ext->شروع += شمار;
			ext->شمار -= شمار;
			أرجع 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	لكل (; والد != عدم; والد = گره) {
		گره = ext2fs_rb_بعدی(والد);
		ext = گره_مقصد_extent(والد);
		إذا ((ext->شروع + ext->شمار) <= شروع)
			استمر;

		/* No more extents to be removed/truncated */
		إذا ((شروع + شمار) < ext->شروع)
			اكسر;

		/* The entire extent is within the region to be removed */
		إذا ((شروع + شمار) >= (ext->شروع + ext->شمار)) {
			ext2fs_rb_erase(والد, ریشه);
			rb_free_extent(bp, ext);
			retval = 1;
			استمر;
		} وإلا {
			/* modify the last extent in region to be removed */
			ext->شمار -= ((شروع + شمار) - ext->شروع);
			ext->شروع = شروع + شمار;
			retval = 1;
			اكسر;
		}
	}

	أرجع retval;
}

ساكن صحيح rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_خصوصی *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;
	arg -= bitmap->شروع;

	retval = rb_درج_extent(arg, 1, bp);
	check_درخت(&bp->ریشه, __func__);
	أرجع retval;
}

ساكن صحيح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_خصوصی *bp;
	صحيح retval;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;
	arg -= bitmap->شروع;

	retval = rb_remove_extent(arg, 1, bp);
	check_درخت(&bp->ریشه, __func__);

	أرجع retval;
}

مضمن
ساكن صحيح rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	بنية ext2fs_rb_خصوصی *bp;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;
	arg -= bitmap->شروع;

	أرجع rb_test_بیت(bp, arg);
}

ساكن فراغ rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				بیعلامت صحيح رقم)
{
	بنية ext2fs_rb_خصوصی *bp;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;
	arg -= bitmap->شروع;

	rb_درج_extent(arg, رقم, bp);
	check_درخت(&bp->ریشه, __func__);
}

ساكن فراغ rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  بیعلامت صحيح رقم)
{
	بنية ext2fs_rb_خصوصی *bp;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;
	arg -= bitmap->شروع;

	rb_remove_extent(arg, رقم, bp);
	check_درخت(&bp->ریشه, __func__);
}

ساكن صحيح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, بیعلامت صحيح len)
{
	بنية rb_گره *والد = عدم, **n;
	بنية rb_گره *گره, *بعدی;
	بنية ext2fs_rb_خصوصی *bp;
	بنية bmap_rb_extent *ext;
	صحيح retval = 1;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;
	n = &bp->ریشه.rb_گره;
	شروع -= bitmap->شروع;

	إذا (len == 0 || ext2fs_rb_خالی_ریشه(&bp->ریشه))
		أرجع 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	طالما (*n) {
		والد = *n;
		ext = گره_مقصد_extent(والد);
		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_چپ;
		} وإلا إذا (شروع >= (ext->شروع + ext->شمار)) {
			n = &(*n)->rb_راست;
		} وإلا {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			أرجع 0;
		}
	}

	گره = والد;
	طالما (گره) {
		بعدی = ext2fs_rb_بعدی(گره);
		ext = گره_مقصد_extent(گره);
		گره = بعدی;

		إذا ((ext->شروع + ext->شمار) <= شروع)
			استمر;

		/* No more merging */
		إذا ((شروع + len) <= ext->شروع)
			اكسر;

		retval = 0;
		اكسر;
	}
	أرجع retval;
}

ساكن errcode_t rb_مجموعه_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, size_t رقم, فراغ *درون)
{
	بنية ext2fs_rb_خصوصی *bp;
	بیعلامت حرف *cp = درون;
	size_t i;
	صحيح اولی_مجموعه = -1;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;

	لكل (i = 0; i < رقم; i++) {
		إذا ((i & 7) == 0) {
			بیعلامت حرف c = cp[i/8];
			إذا (c == 0xFF) {
				إذا (اولی_مجموعه == -1)
					اولی_مجموعه = i;
				i += 7;
				استمر;
			}
			إذا ((c == 0x00) && (اولی_مجموعه == -1)) {
				i += 7;
				استمر;
			}
		}
		إذا (ext2fs_test_بیت(i, درون)) {
			إذا (اولی_مجموعه == -1)
				اولی_مجموعه = i;
			استمر;
		}
		إذا (اولی_مجموعه == -1)
			استمر;

		rb_درج_extent(شروع + اولی_مجموعه - bitmap->شروع,
				 i - اولی_مجموعه, bp);
		check_درخت(&bp->ریشه, __func__);
		اولی_مجموعه = -1;
	}
	إذا (اولی_مجموعه != -1) {
		rb_درج_extent(شروع + اولی_مجموعه - bitmap->شروع,
				 رقم - اولی_مجموعه, bp);
		check_درخت(&bp->ریشه, __func__);
	}

	أرجع 0;
}

ساكن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, size_t رقم, فراغ *بیرون)
{

	بنية rb_گره *والد = عدم, *بعدی, **n;
	بنية ext2fs_rb_خصوصی *bp;
	بنية bmap_rb_extent *ext;
	__u64 شمار, pos;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;
	n = &bp->ریشه.rb_گره;
	شروع -= bitmap->شروع;

	إذا (ext2fs_rb_خالی_ریشه(&bp->ریشه))
		أرجع 0;

	طالما (*n) {
		والد = *n;
		ext = گره_مقصد_extent(والد);
		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_چپ;
		} وإلا إذا (شروع >= (ext->شروع + ext->شمار)) {
			n = &(*n)->rb_راست;
		} وإلا
			اكسر;
	}

	memset(بیرون, 0, (رقم + 7) >> 3);

	لكل (; والد != عدم; والد = بعدی) {
		بعدی = ext2fs_rb_بعدی(والد);
		ext = گره_مقصد_extent(والد);

		pos = ext->شروع;
		شمار = ext->شمار;
		إذا (pos >= شروع + رقم)
			اكسر;
		إذا (pos < شروع) {
			إذا (pos + شمار <  شروع)
				استمر;
			شمار -= شروع - pos;
			pos = شروع;
		}
		إذا (pos + شمار > شروع + رقم)
			شمار = شروع + رقم - pos;

		طالما (شمار > 0) {
			إذا ((شمار >= 8) &&
			    ((pos - شروع) % 8) == 0) {
				صحيح nbytes = شمار >> 3;
				صحيح جابه_جایی = (pos - شروع) >> 3;

				memset(((حرف *) بیرون) + جابه_جایی, 0xFF, nbytes);
				pos += nbytes << 3;
				شمار -= nbytes << 3;
				استمر;
			}
			ext2fs_fast_مجموعه_bit64((pos - شروع), بیرون);
			pos++;
			شمار--;
		}
	}
	أرجع 0;
}

ساكن فراغ rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_خصوصی *bp;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;

	rb_free_درخت(&bp->ریشه);
	bp->rcursor = عدم;
	bp->rcursor_بعدی = عدم;
	bp->wcursor = عدم;
	check_درخت(&bp->ریشه, __func__);
}

ساكن errcode_t rb_پیدا_کن_اولی_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 شروع, __u64 انتها, __u64 *بیرون)
{
	بنية rb_گره *والد = عدم, **n;
	بنية ext2fs_rb_خصوصی *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;
	n = &bp->ریشه.rb_گره;
	شروع -= bitmap->شروع;
	انتها -= bitmap->شروع;

	إذا (شروع > انتها)
		أرجع EINVAL;

	إذا (ext2fs_rb_خالی_ریشه(&bp->ریشه))
		أرجع ENOENT;

	طالما (*n) {
		والد = *n;
		ext = گره_مقصد_extent(والد);
		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_چپ;
		} وإلا إذا (شروع >= (ext->شروع + ext->شمار)) {
			n = &(*n)->rb_راست;
		} وإلا إذا (ext->شروع + ext->شمار <= انتها) {
			*بیرون = ext->شروع + ext->شمار + bitmap->شروع;
			أرجع 0;
		} وإلا
			أرجع ENOENT;
	}

	*بیرون = شروع + bitmap->شروع;
	أرجع 0;
}

ساكن errcode_t rb_پیدا_کن_اولی_مجموعه(ext2fs_generic_bitmap_64 bitmap,
				   __u64 شروع, __u64 انتها, __u64 *بیرون)
{
	بنية rb_گره *والد = عدم, **n;
	بنية rb_گره *گره;
	بنية ext2fs_rb_خصوصی *bp;
	بنية bmap_rb_extent *ext;

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;
	n = &bp->ریشه.rb_گره;
	شروع -= bitmap->شروع;
	انتها -= bitmap->شروع;

	إذا (شروع > انتها)
		أرجع EINVAL;

	إذا (ext2fs_rb_خالی_ریشه(&bp->ریشه))
		أرجع ENOENT;

	طالما (*n) {
		والد = *n;
		ext = گره_مقصد_extent(والد);
		إذا (شروع < ext->شروع) {
			n = &(*n)->rb_چپ;
		} وإلا إذا (شروع >= (ext->شروع + ext->شمار)) {
			n = &(*n)->rb_راست;
		} وإلا {
			/* The start bit is set */
			*بیرون = شروع + bitmap->شروع;
			أرجع 0;
		}
	}

	گره = والد;
	ext = گره_مقصد_extent(گره);
	إذا (ext->شروع < شروع) {
		گره = ext2fs_rb_بعدی(گره);
		إذا (گره == عدم)
			أرجع ENOENT;
		ext = گره_مقصد_extent(گره);
	}
	إذا (ext->شروع <= انتها) {
		*بیرون = ext->شروع + bitmap->شروع;
		أرجع 0;
	}
	أرجع ENOENT;
}

#اگر_تعریف_شده ENABLE_BMAP_STATS
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	بنية ext2fs_rb_خصوصی *bp;
	بنية rb_گره *گره = عدم;
	بنية bmap_rb_extent *ext;
	__u64 شمار = 0;
	__u64 max_اندازه = 0;
	__u64 min_اندازه = ULONG_MAX;
	__u64 اندازه = 0, avg_اندازه = 0;
	مزدوج eff;
#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	مزدوج m_اصابت = 0.0, t_اصابت = 0.0;
#پایان_اگر

	bp = (بنية ext2fs_rb_خصوصی *) bitmap->خاص;

	لكل (گره = ext2fs_rb_اولی(&bp->ریشه); گره != عدم;
	     گره = ext2fs_rb_بعدی(گره)) {
		ext = گره_مقصد_extent(گره);
		شمار++;
		إذا (ext->شمار > max_اندازه)
			max_اندازه = ext->شمار;
		إذا (ext->شمار < min_اندازه)
			min_اندازه = ext->شمار;
		اندازه += ext->شمار;
	}

	إذا (شمار)
		avg_اندازه = اندازه / شمار;
	إذا (min_اندازه == ULONG_MAX)
		min_اندازه = 0;
	eff = (مزدوج)((شمار * حجم(بنية bmap_rb_extent)) << 3) /
	      (bitmap->real_انتها - bitmap->شروع);
#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_شمار + bitmap->stats.mark_ext_شمار;
	test_all = bitmap->stats.test_شمار + bitmap->stats.test_ext_شمار;
	إذا (mark_all)
		m_اصابت = ((مزدوج)bp->mark_اصابت / mark_all) * 100;
	إذا (test_all)
		t_اصابت = ((مزدوج)bp->test_اصابت / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_اصابت, t_اصابت, bp->mark_اصابت, m_اصابت);
#پایان_اگر
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(بیعلامت دراز دراز) شمار, (بیعلامت دراز دراز)
		((شمار * حجم(بنية bmap_rb_extent)) +
		 حجم(بنية ext2fs_rb_خصوصی)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(بیعلامت دراز دراز) min_اندازه);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(بیعلامت دراز دراز) max_اندازه, (بیعلامت دراز دراز) avg_اندازه);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(بیعلامت دراز دراز) اندازه,
		(بیعلامت دراز دراز) bitmap->real_انتها - bitmap->شروع);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#وإلا
ساكن فراغ rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((استفادهنشده)))
{
}
#پایان_اگر

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
	.مجموعه_bmap_range = rb_مجموعه_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.پیدا_کن_اولی_zero = rb_پیدا_کن_اولی_zero,
	.پیدا_کن_اولی_مجموعه = rb_پیدا_کن_اولی_مجموعه,
};
