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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر HAVE_LINUX_TYPES_H
#شامل <linux/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"
#شامل "bmap64.h"
#شامل "rbtree.h"

#شامل <limits.h>

ساخت bmap_rb_extent {
	ساخت rb_گرہ گرہ;
	__u64 آغاز;
	__u64 گنتی;
};

ساخت ext2fs_rb_نجی {
	ساخت rb_جڑ جڑ;
	ساخت bmap_rb_extent *wcursor;
	ساخت bmap_rb_extent *rcursor;
	ساخت bmap_rb_extent *rcursor_اگلا;
#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	__u64 mark_کامیابی;
	__u64 test_کامیابی;
#اگر_ختم
};

اندرونی ساکن ساخت bmap_rb_extent *گرہ_مقصد_extent(ساخت rb_گرہ *گرہ)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	واپس (ساخت bmap_rb_extent *) گرہ;
}

ساکن صحیح rb_داخل_کرو_extent(__u64 آغاز, __u64 گنتی,
			    ساخت ext2fs_rb_نجی *);
ساکن خالی rb_get_new_extent(ساخت bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#اگر_تعریف_شدہ DEBUG_RB
ساکن خالی print_درخت(ساخت rb_جڑ *جڑ)
{
	ساخت rb_گرہ *گرہ = تہی;
	ساخت bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	گرہ = ext2fs_rb_پہلا(جڑ);
	برائے (گرہ = ext2fs_rb_پہلا(جڑ); گرہ != تہی; 
	     گرہ = ext2fs_rb_اگلا(گرہ)) {
		ext = گرہ_مقصد_extent(گرہ);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(غیرعلامتی طویل طویل) ext->آغاز,
			(غیرعلامتی طویل طویل) ext->آغاز + ext->گنتی);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

ساکن خالی check_درخت(ساخت rb_جڑ *جڑ, ثابت حرف *پیغام)
{
	ساخت rb_گرہ *گرہ;
	ساخت bmap_rb_extent *ext, *old = تہی;

	برائے (گرہ = ext2fs_rb_پہلا(جڑ); گرہ;
	     گرہ = ext2fs_rb_اگلا(گرہ)) {
		ext = گرہ_مقصد_extent(گرہ);
		اگر (ext->گنتی == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(غیرعلامتی طویل طویل) ext->آغاز,
				(غیرعلامتی طویل طویل) ext->آغاز + ext->گنتی,
				(غیرعلامتی طویل طویل) ext->گنتی);
			جاؤ err_برآمد;
		}
		اگر (ext->آغاز + ext->گنتی < ext->آغاز) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(غیرعلامتی طویل طویل) ext->آغاز,
				(غیرعلامتی طویل طویل) ext->آغاز + ext->گنتی,
				(غیرعلامتی طویل طویل) ext->گنتی);
			جاؤ err_برآمد;
		}

		اگر (old) {
			اگر (old->آغاز > ext->آغاز) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(غیرعلامتی طویل طویل) old->آغاز,
					(غیرعلامتی طویل طویل) old->آغاز + old->گنتی,
					(غیرعلامتی طویل طویل) old->گنتی);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(غیرعلامتی طویل طویل) ext->آغاز,
					(غیرعلامتی طویل طویل) ext->آغاز + ext->گنتی,
					(غیرعلامتی طویل طویل) ext->گنتی);
				جاؤ err_برآمد;
			}
			اگر ((old->آغاز + old->گنتی) >= ext->آغاز) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(غیرعلامتی طویل طویل) old->آغاز,
					(غیرعلامتی طویل طویل) old->آغاز + old->گنتی,
					(غیرعلامتی طویل طویل) old->گنتی);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(غیرعلامتی طویل طویل) ext->آغاز,
					(غیرعلامتی طویل طویل) ext->آغاز + ext->گنتی,
					(غیرعلامتی طویل طویل) ext->گنتی);
				جاؤ err_برآمد;
			}
		}
		old = ext;
	}
	واپس;

err_برآمد:
	fprintf(stderr, "%s\n", پیغام);
	print_درخت(جڑ);
	باہر_نکلو(1);
}
#ورنہ
#تعریف_کرو check_درخت(جڑ, پیغام) کرو {} جب (0)
#تعریف_کرو print_درخت(جڑ) کرو {} جب (0)
#اگر_ختم

ساکن خالی rb_get_new_extent(ساخت bmap_rb_extent **ext, __u64 آغاز,
			     __u64 گنتی)
{
	ساخت bmap_rb_extent *new_ext;
	صحیح retval;

	retval = ext2fs_get_mem(حجم (ساخت bmap_rb_extent),
				&new_ext);
	اگر (retval)
		abort();

	new_ext->آغاز = آغاز;
	new_ext->گنتی = گنتی;
	*ext = new_ext;
}

اندرونی
ساکن خالی rb_free_extent(ساخت ext2fs_rb_نجی *bp,
			   ساخت bmap_rb_extent *ext)
{
	اگر (bp->wcursor == ext)
		bp->wcursor = تہی;
	اگر (bp->rcursor == ext)
		bp->rcursor = تہی;
	اگر (bp->rcursor_اگلا == ext)
		bp->rcursor_اگلا = تہی;
	ext2fs_free_mem(&ext);
}

ساکن errcode_t rb_alloc_نجی_ڈیٹا (ext2fs_generic_bitmap_64 bitmap)
{
	ساخت ext2fs_rb_نجی *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم (ساخت ext2fs_rb_نجی), &bp);
	اگر (retval)
		واپس retval;

	bp->جڑ = RB_جڑ;
	bp->rcursor = تہی;
	bp->rcursor_اگلا = تہی;
	bp->wcursor = تہی;

#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	bp->test_کامیابی = 0;
	bp->mark_کامیابی = 0;
#اگر_ختم

	bitmap->نجی = (خالی *) bp;
	واپس 0;
}

ساکن errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((غیر_مستعمل)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_نجی_ڈیٹا (bitmap);
	اگر (retval)
		واپس retval;

	واپس 0;
}

ساکن خالی rb_free_درخت(ساخت rb_جڑ *جڑ)
{
	ساخت bmap_rb_extent *ext;
	ساخت rb_گرہ *گرہ, *اگلا;

	برائے (گرہ = ext2fs_rb_پہلا(جڑ); گرہ; گرہ = اگلا) {
		اگلا = ext2fs_rb_اگلا(گرہ);
		ext = گرہ_مقصد_extent(گرہ);
		ext2fs_rb_erase(گرہ, جڑ);
		ext2fs_free_mem(&ext);
	}
}

ساکن خالی rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ساخت ext2fs_rb_نجی *bp;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;

	rb_free_درخت(&bp->جڑ);
	ext2fs_free_mem(&bp);
	bp = 0;
}

ساکن errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ساخت ext2fs_rb_نجی *src_bp, *dest_bp;
	ساخت bmap_rb_extent *src_ext, *dest_ext;
	ساخت rb_گرہ *dest_گرہ, *src_گرہ, *dest_آخری, **n;
	errcode_t retval = 0;

	retval = rb_alloc_نجی_ڈیٹا (dest);
	اگر (retval)
		واپس retval;

	src_bp = (ساخت ext2fs_rb_نجی *) src->نجی;
	dest_bp = (ساخت ext2fs_rb_نجی *) dest->نجی;
	src_bp->rcursor = تہی;
	dest_bp->rcursor = تہی;

	src_گرہ = ext2fs_rb_پہلا(&src_bp->جڑ);
	جب (src_گرہ) {
		src_ext = گرہ_مقصد_extent(src_گرہ);
		retval = ext2fs_get_mem(حجم (ساخت bmap_rb_extent),
					&dest_ext);
		اگر (retval)
			توڑو;

		حافظہ_نقل(dest_ext, src_ext, حجم(ساخت bmap_rb_extent));

		dest_گرہ = &dest_ext->گرہ;
		n = &dest_bp->جڑ.rb_گرہ;

		dest_آخری = تہی;
		اگر (*n) {
			dest_آخری = ext2fs_rb_آخری(&dest_bp->جڑ);
			n = &(dest_آخری)->rb_دایاں;
		}

		ext2fs_rb_link_گرہ(dest_گرہ, dest_آخری, n);
		ext2fs_rb_داخل_کرو_color(dest_گرہ, &dest_bp->جڑ);

		src_گرہ = ext2fs_rb_اگلا(src_گرہ);
	}

	واپس retval;
}

ساکن خالی rb_truncate(__u64 new_max, ساخت rb_جڑ *جڑ)
{
	ساخت bmap_rb_extent *ext;
	ساخت rb_گرہ *گرہ;

	گرہ = ext2fs_rb_آخری(جڑ);
	جب (گرہ) {
		ext = گرہ_مقصد_extent(گرہ);

		اگر ((ext->آغاز + ext->گنتی - 1) <= new_max)
			توڑو;
		ورنہ اگر (ext->آغاز > new_max) {
			ext2fs_rb_erase(گرہ, جڑ);
			ext2fs_free_mem(&ext);
			گرہ = ext2fs_rb_آخری(جڑ);
			جاری;
		} ورنہ
			ext->گنتی = new_max - ext->آغاز + 1;
	}
}

ساکن errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_انتہا, __u64 new_real_انتہا)
{
	ساخت ext2fs_rb_نجی *bp;

	bp = (ساخت ext2fs_rb_نجی *) bmap->نجی;
	bp->rcursor = تہی;
	bp->wcursor = تہی;

	rb_truncate(((new_انتہا < bmap->انتہا) ? new_انتہا : bmap->انتہا) - bmap->آغاز,
		    &bp->جڑ);

	bmap->انتہا = new_انتہا;
	bmap->real_انتہا = new_real_انتہا;

	اگر (bmap->انتہا < bmap->real_انتہا)
		rb_داخل_کرو_extent(bmap->انتہا + 1 - bmap->آغاز,
				 bmap->real_انتہا - bmap->انتہا, bp);
	واپس 0;

}

اندرونی ساکن صحیح
rb_test_ثنائی_ہندسہ(ساخت ext2fs_rb_نجی *bp, __u64 ثنائی_ہندسہ)
{
	ساخت bmap_rb_extent *rcursor, *اگلا_ext = تہی;
	ساخت rb_گرہ *والد = تہی, *اگلا;
	ساخت rb_گرہ **n = &bp->جڑ.rb_گرہ;
	ساخت bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	اگر (!rcursor)
		جاؤ تلاش_درخت;

	اگر (ثنائی_ہندسہ >= rcursor->آغاز && ثنائی_ہندسہ < rcursor->آغاز + rcursor->گنتی) {
#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
		bp->test_کامیابی++;
#اگر_ختم
		واپس 1;
	}

	اگلا_ext = bp->rcursor_اگلا;
	اگر (!اگلا_ext) {
		اگلا = ext2fs_rb_اگلا(&rcursor->گرہ);
		اگر (اگلا)
			اگلا_ext = گرہ_مقصد_extent(اگلا);
		bp->rcursor_اگلا = اگلا_ext;
	}
	اگر (اگلا_ext) {
		اگر ((ثنائی_ہندسہ >= rcursor->آغاز + rcursor->گنتی) &&
		    (ثنائی_ہندسہ < اگلا_ext->آغاز)) {
#اگر_تعریف_شدہ BMAP_STATS_OPS
			bp->test_کامیابی++;
#اگر_ختم
			واپس 0;
		}
	}
	bp->rcursor = تہی;
	bp->rcursor_اگلا = تہی;

	rcursor = bp->wcursor;
	اگر (!rcursor)
		جاؤ تلاش_درخت;

	اگر (ثنائی_ہندسہ >= rcursor->آغاز && ثنائی_ہندسہ < rcursor->آغاز + rcursor->گنتی)
		واپس 1;

تلاش_درخت:

	جب (*n) {
		والد = *n;
		ext = گرہ_مقصد_extent(والد);
		اگر (ثنائی_ہندسہ < ext->آغاز)
			n = &(*n)->rb_بایاں;
		ورنہ اگر (ثنائی_ہندسہ >= (ext->آغاز + ext->گنتی))
			n = &(*n)->rb_دایاں;
		ورنہ {
			bp->rcursor = ext;
			bp->rcursor_اگلا = تہی;
			واپس 1;
		}
	}
	واپس 0;
}

ساکن صحیح rb_داخل_کرو_extent(__u64 آغاز, __u64 گنتی,
			    ساخت ext2fs_rb_نجی *bp)
{
	ساخت rb_جڑ *جڑ = &bp->جڑ;
	ساخت rb_گرہ *والد = تہی, **n = &جڑ->rb_گرہ;
	ساخت rb_گرہ *new_گرہ, *گرہ, *اگلا;
	ساخت bmap_rb_extent *new_ext;
	ساخت bmap_rb_extent *ext;
	صحیح retval = 0;

	اگر (گنتی == 0)
		واپس 0;

	bp->rcursor_اگلا = تہی;
	ext = bp->wcursor;
	اگر (ext) {
		اگر (آغاز >= ext->آغاز &&
		    آغاز <= (ext->آغاز + ext->گنتی)) {
#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
			bp->mark_کامیابی++;
#اگر_ختم
			جاؤ got_extent;
		}
	}

	جب (*n) {
		والد = *n;
		ext = گرہ_مقصد_extent(والد);

		اگر (آغاز < ext->آغاز) {
			n = &(*n)->rb_بایاں;
		} ورنہ اگر (آغاز > (ext->آغاز + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} ورنہ {
got_extent:
			اگر ((آغاز + گنتی) <= (ext->آغاز + ext->گنتی))
				واپس 1;

			اگر ((ext->آغاز + ext->گنتی) == آغاز)
				retval = 0;
			ورنہ
				retval = 1;

			گنتی += (آغاز - ext->آغاز);
			آغاز = ext->آغاز;
			new_ext = ext;
			new_گرہ = &ext->گرہ;

			جاؤ skip_داخل_کرو;
		}
	}

	rb_get_new_extent(&new_ext, آغاز, گنتی);

	new_گرہ = &new_ext->گرہ;
	ext2fs_rb_link_گرہ(new_گرہ, والد, n);
	ext2fs_rb_داخل_کرو_color(new_گرہ, جڑ);
	bp->wcursor = new_ext;

	گرہ = ext2fs_rb_پچھلا(new_گرہ);
	اگر (گرہ) {
		ext = گرہ_مقصد_extent(گرہ);
		اگر ((ext->آغاز + ext->گنتی) == آغاز) {
			آغاز = ext->آغاز;
			گنتی += ext->گنتی;
			ext2fs_rb_erase(گرہ, جڑ);
			rb_free_extent(bp, ext);
		}
	}

skip_داخل_کرو:
	/* See if we can merge extent to the right */
	برائے (گرہ = ext2fs_rb_اگلا(new_گرہ); گرہ != تہی; گرہ = اگلا) {
		اگلا = ext2fs_rb_اگلا(گرہ);
		ext = گرہ_مقصد_extent(گرہ);

		اگر ((ext->آغاز + ext->گنتی) <= آغاز)
			جاری;

		/* No more merging */
		اگر ((آغاز + گنتی) < ext->آغاز)
			توڑو;

		/* ext is embedded in new_ext interval */
		اگر ((آغاز + گنتی) >= (ext->آغاز + ext->گنتی)) {
			ext2fs_rb_erase(گرہ, جڑ);
			rb_free_extent(bp, ext);
			جاری;
		} ورنہ {
		/* merge ext with new_ext */
			گنتی += ((ext->آغاز + ext->گنتی) -
				  (آغاز + گنتی));
			ext2fs_rb_erase(گرہ, جڑ);
			rb_free_extent(bp, ext);
			توڑو;
		}
	}

	new_ext->آغاز = آغاز;
	new_ext->گنتی = گنتی;

	واپس retval;
}

ساکن صحیح rb_remove_extent(__u64 آغاز, __u64 گنتی,
			    ساخت ext2fs_rb_نجی *bp)
{
	ساخت rb_جڑ *جڑ = &bp->جڑ;
	ساخت rb_گرہ *والد = تہی, **n = &جڑ->rb_گرہ;
	ساخت rb_گرہ *گرہ;
	ساخت bmap_rb_extent *ext;
	__u64 new_آغاز, new_گنتی;
	صحیح retval = 0;

	اگر (ext2fs_rb_خالی_خانے_جڑ(جڑ))
		واپس 0;

	جب (*n) {
		والد = *n;
		ext = گرہ_مقصد_extent(والد);
		اگر (آغاز < ext->آغاز) {
			n = &(*n)->rb_بایاں;
			جاری;
		} ورنہ اگر (آغاز >= (ext->آغاز + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
			جاری;
		}

		اگر ((آغاز > ext->آغاز) &&
		    (آغاز + گنتی) < (ext->آغاز + ext->گنتی)) {
			/* We have to split extent into two */
			new_آغاز = آغاز + گنتی;
			new_گنتی = (ext->آغاز + ext->گنتی) - new_آغاز;

			ext->گنتی = آغاز - ext->آغاز;

			rb_داخل_کرو_extent(new_آغاز, new_گنتی, bp);
			واپس 1;
		}

		اگر ((آغاز + گنتی) >= (ext->آغاز + ext->گنتی)) {
			ext->گنتی = آغاز - ext->آغاز;
			retval = 1;
		}

		اگر (0 == ext->گنتی) {
			والد = ext2fs_rb_اگلا(&ext->گرہ);
			ext2fs_rb_erase(&ext->گرہ, جڑ);
			rb_free_extent(bp, ext);
			توڑو;
		}

		اگر (آغاز == ext->آغاز) {
			ext->آغاز += گنتی;
			ext->گنتی -= گنتی;
			واپس 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	برائے (; والد != تہی; والد = گرہ) {
		گرہ = ext2fs_rb_اگلا(والد);
		ext = گرہ_مقصد_extent(والد);
		اگر ((ext->آغاز + ext->گنتی) <= آغاز)
			جاری;

		/* No more extents to be removed/truncated */
		اگر ((آغاز + گنتی) < ext->آغاز)
			توڑو;

		/* The entire extent is within the region to be removed */
		اگر ((آغاز + گنتی) >= (ext->آغاز + ext->گنتی)) {
			ext2fs_rb_erase(والد, جڑ);
			rb_free_extent(bp, ext);
			retval = 1;
			جاری;
		} ورنہ {
			/* modify the last extent in region to be removed */
			ext->گنتی -= ((آغاز + گنتی) - ext->آغاز);
			ext->آغاز = آغاز + گنتی;
			retval = 1;
			توڑو;
		}
	}

	واپس retval;
}

ساکن صحیح rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ساخت ext2fs_rb_نجی *bp;
	صحیح retval;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;
	arg -= bitmap->آغاز;

	retval = rb_داخل_کرو_extent(arg, 1, bp);
	check_درخت(&bp->جڑ, __func__);
	واپس retval;
}

ساکن صحیح rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ساخت ext2fs_rb_نجی *bp;
	صحیح retval;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;
	arg -= bitmap->آغاز;

	retval = rb_remove_extent(arg, 1, bp);
	check_درخت(&bp->جڑ, __func__);

	واپس retval;
}

اندرونی
ساکن صحیح rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ساخت ext2fs_rb_نجی *bp;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;
	arg -= bitmap->آغاز;

	واپس rb_test_ثنائی_ہندسہ(bp, arg);
}

ساکن خالی rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				غیرعلامتی صحیح عدد)
{
	ساخت ext2fs_rb_نجی *bp;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;
	arg -= bitmap->آغاز;

	rb_داخل_کرو_extent(arg, عدد, bp);
	check_درخت(&bp->جڑ, __func__);
}

ساکن خالی rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  غیرعلامتی صحیح عدد)
{
	ساخت ext2fs_rb_نجی *bp;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;
	arg -= bitmap->آغاز;

	rb_remove_extent(arg, عدد, bp);
	check_درخت(&bp->جڑ, __func__);
}

ساکن صحیح rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 آغاز, غیرعلامتی صحیح len)
{
	ساخت rb_گرہ *والد = تہی, **n;
	ساخت rb_گرہ *گرہ, *اگلا;
	ساخت ext2fs_rb_نجی *bp;
	ساخت bmap_rb_extent *ext;
	صحیح retval = 1;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;
	n = &bp->جڑ.rb_گرہ;
	آغاز -= bitmap->آغاز;

	اگر (len == 0 || ext2fs_rb_خالی_خانے_جڑ(&bp->جڑ))
		واپس 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	جب (*n) {
		والد = *n;
		ext = گرہ_مقصد_extent(والد);
		اگر (آغاز < ext->آغاز) {
			n = &(*n)->rb_بایاں;
		} ورنہ اگر (آغاز >= (ext->آغاز + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} ورنہ {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			واپس 0;
		}
	}

	گرہ = والد;
	جب (گرہ) {
		اگلا = ext2fs_rb_اگلا(گرہ);
		ext = گرہ_مقصد_extent(گرہ);
		گرہ = اگلا;

		اگر ((ext->آغاز + ext->گنتی) <= آغاز)
			جاری;

		/* No more merging */
		اگر ((آغاز + len) <= ext->آغاز)
			توڑو;

		retval = 0;
		توڑو;
	}
	واپس retval;
}

ساکن errcode_t rb_سیٹ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 آغاز, size_t عدد, خالی *اندر)
{
	ساخت ext2fs_rb_نجی *bp;
	غیرعلامتی حرف *cp = اندر;
	size_t i;
	صحیح پہلا_سیٹ = -1;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;

	برائے (i = 0; i < عدد; i++) {
		اگر ((i & 7) == 0) {
			غیرعلامتی حرف c = cp[i/8];
			اگر (c == 0xFF) {
				اگر (پہلا_سیٹ == -1)
					پہلا_سیٹ = i;
				i += 7;
				جاری;
			}
			اگر ((c == 0x00) && (پہلا_سیٹ == -1)) {
				i += 7;
				جاری;
			}
		}
		اگر (ext2fs_test_ثنائی_ہندسہ(i, اندر)) {
			اگر (پہلا_سیٹ == -1)
				پہلا_سیٹ = i;
			جاری;
		}
		اگر (پہلا_سیٹ == -1)
			جاری;

		rb_داخل_کرو_extent(آغاز + پہلا_سیٹ - bitmap->آغاز,
				 i - پہلا_سیٹ, bp);
		check_درخت(&bp->جڑ, __func__);
		پہلا_سیٹ = -1;
	}
	اگر (پہلا_سیٹ != -1) {
		rb_داخل_کرو_extent(آغاز + پہلا_سیٹ - bitmap->آغاز,
				 عدد - پہلا_سیٹ, bp);
		check_درخت(&bp->جڑ, __func__);
	}

	واپس 0;
}

ساکن errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 آغاز, size_t عدد, خالی *برآمد)
{

	ساخت rb_گرہ *والد = تہی, *اگلا, **n;
	ساخت ext2fs_rb_نجی *bp;
	ساخت bmap_rb_extent *ext;
	__u64 گنتی, pos;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;
	n = &bp->جڑ.rb_گرہ;
	آغاز -= bitmap->آغاز;

	اگر (ext2fs_rb_خالی_خانے_جڑ(&bp->جڑ))
		واپس 0;

	جب (*n) {
		والد = *n;
		ext = گرہ_مقصد_extent(والد);
		اگر (آغاز < ext->آغاز) {
			n = &(*n)->rb_بایاں;
		} ورنہ اگر (آغاز >= (ext->آغاز + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} ورنہ
			توڑو;
	}

	memset(برآمد, 0, (عدد + 7) >> 3);

	برائے (; والد != تہی; والد = اگلا) {
		اگلا = ext2fs_rb_اگلا(والد);
		ext = گرہ_مقصد_extent(والد);

		pos = ext->آغاز;
		گنتی = ext->گنتی;
		اگر (pos >= آغاز + عدد)
			توڑو;
		اگر (pos < آغاز) {
			اگر (pos + گنتی <  آغاز)
				جاری;
			گنتی -= آغاز - pos;
			pos = آغاز;
		}
		اگر (pos + گنتی > آغاز + عدد)
			گنتی = آغاز + عدد - pos;

		جب (گنتی > 0) {
			اگر ((گنتی >= 8) &&
			    ((pos - آغاز) % 8) == 0) {
				صحیح nbytes = گنتی >> 3;
				صحیح انحراف = (pos - آغاز) >> 3;

				memset(((حرف *) برآمد) + انحراف, 0xFF, nbytes);
				pos += nbytes << 3;
				گنتی -= nbytes << 3;
				جاری;
			}
			ext2fs_fast_سیٹ_bit64((pos - آغاز), برآمد);
			pos++;
			گنتی--;
		}
	}
	واپس 0;
}

ساکن خالی rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ساخت ext2fs_rb_نجی *bp;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;

	rb_free_درخت(&bp->جڑ);
	bp->rcursor = تہی;
	bp->rcursor_اگلا = تہی;
	bp->wcursor = تہی;
	check_درخت(&bp->جڑ, __func__);
}

ساکن errcode_t rb_ڈھونڈو_پہلا_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 آغاز, __u64 انتہا, __u64 *برآمد)
{
	ساخت rb_گرہ *والد = تہی, **n;
	ساخت ext2fs_rb_نجی *bp;
	ساخت bmap_rb_extent *ext;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;
	n = &bp->جڑ.rb_گرہ;
	آغاز -= bitmap->آغاز;
	انتہا -= bitmap->آغاز;

	اگر (آغاز > انتہا)
		واپس EINVAL;

	اگر (ext2fs_rb_خالی_خانے_جڑ(&bp->جڑ))
		واپس ENOENT;

	جب (*n) {
		والد = *n;
		ext = گرہ_مقصد_extent(والد);
		اگر (آغاز < ext->آغاز) {
			n = &(*n)->rb_بایاں;
		} ورنہ اگر (آغاز >= (ext->آغاز + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} ورنہ اگر (ext->آغاز + ext->گنتی <= انتہا) {
			*برآمد = ext->آغاز + ext->گنتی + bitmap->آغاز;
			واپس 0;
		} ورنہ
			واپس ENOENT;
	}

	*برآمد = آغاز + bitmap->آغاز;
	واپس 0;
}

ساکن errcode_t rb_ڈھونڈو_پہلا_سیٹ(ext2fs_generic_bitmap_64 bitmap,
				   __u64 آغاز, __u64 انتہا, __u64 *برآمد)
{
	ساخت rb_گرہ *والد = تہی, **n;
	ساخت rb_گرہ *گرہ;
	ساخت ext2fs_rb_نجی *bp;
	ساخت bmap_rb_extent *ext;

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;
	n = &bp->جڑ.rb_گرہ;
	آغاز -= bitmap->آغاز;
	انتہا -= bitmap->آغاز;

	اگر (آغاز > انتہا)
		واپس EINVAL;

	اگر (ext2fs_rb_خالی_خانے_جڑ(&bp->جڑ))
		واپس ENOENT;

	جب (*n) {
		والد = *n;
		ext = گرہ_مقصد_extent(والد);
		اگر (آغاز < ext->آغاز) {
			n = &(*n)->rb_بایاں;
		} ورنہ اگر (آغاز >= (ext->آغاز + ext->گنتی)) {
			n = &(*n)->rb_دایاں;
		} ورنہ {
			/* The start bit is set */
			*برآمد = آغاز + bitmap->آغاز;
			واپس 0;
		}
	}

	گرہ = والد;
	ext = گرہ_مقصد_extent(گرہ);
	اگر (ext->آغاز < آغاز) {
		گرہ = ext2fs_rb_اگلا(گرہ);
		اگر (گرہ == تہی)
			واپس ENOENT;
		ext = گرہ_مقصد_extent(گرہ);
	}
	اگر (ext->آغاز <= انتہا) {
		*برآمد = ext->آغاز + bitmap->آغاز;
		واپس 0;
	}
	واپس ENOENT;
}

#اگر_تعریف_شدہ ENABLE_BMAP_STATS
ساکن خالی rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	ساخت ext2fs_rb_نجی *bp;
	ساخت rb_گرہ *گرہ = تہی;
	ساخت bmap_rb_extent *ext;
	__u64 گنتی = 0;
	__u64 max_سائز = 0;
	__u64 min_سائز = ULONG_MAX;
	__u64 سائز = 0, avg_سائز = 0;
	دوہرا eff;
#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	دوہرا m_کامیابی = 0.0, t_کامیابی = 0.0;
#اگر_ختم

	bp = (ساخت ext2fs_rb_نجی *) bitmap->نجی;

	برائے (گرہ = ext2fs_rb_پہلا(&bp->جڑ); گرہ != تہی;
	     گرہ = ext2fs_rb_اگلا(گرہ)) {
		ext = گرہ_مقصد_extent(گرہ);
		گنتی++;
		اگر (ext->گنتی > max_سائز)
			max_سائز = ext->گنتی;
		اگر (ext->گنتی < min_سائز)
			min_سائز = ext->گنتی;
		سائز += ext->گنتی;
	}

	اگر (گنتی)
		avg_سائز = سائز / گنتی;
	اگر (min_سائز == ULONG_MAX)
		min_سائز = 0;
	eff = (دوہرا)((گنتی * حجم(ساخت bmap_rb_extent)) << 3) /
	      (bitmap->real_انتہا - bitmap->آغاز);
#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_گنتی + bitmap->stats.mark_ext_گنتی;
	test_all = bitmap->stats.test_گنتی + bitmap->stats.test_ext_گنتی;
	اگر (mark_all)
		m_کامیابی = ((دوہرا)bp->mark_کامیابی / mark_all) * 100;
	اگر (test_all)
		t_کامیابی = ((دوہرا)bp->test_کامیابی / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_کامیابی, t_کامیابی, bp->mark_کامیابی, m_کامیابی);
#اگر_ختم
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(غیرعلامتی طویل طویل) گنتی, (غیرعلامتی طویل طویل)
		((گنتی * حجم(ساخت bmap_rb_extent)) +
		 حجم(ساخت ext2fs_rb_نجی)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(غیرعلامتی طویل طویل) min_سائز);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(غیرعلامتی طویل طویل) max_سائز, (غیرعلامتی طویل طویل) avg_سائز);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(غیرعلامتی طویل طویل) سائز,
		(غیرعلامتی طویل طویل) bitmap->real_انتہا - bitmap->آغاز);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#ورنہ
ساکن خالی rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((غیر_مستعمل)))
{
}
#اگر_ختم

ساخت ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
	.ڈھونڈو_پہلا_zero = rb_ڈھونڈو_پہلا_zero,
	.ڈھونڈو_پہلا_سیٹ = rb_ڈھونڈو_پہلا_سیٹ,
};
