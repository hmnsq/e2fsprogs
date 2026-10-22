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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_LINUX_TYPES_H
#అన్తర్భావ <linux/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"
#అన్తర్భావ "bmap64.h"
#అన్తర్భావ "rbtree.h"

#అన్తర్భావ <limits.h>

నిర్మాణం bmap_rb_extent {
	నిర్మాణం rb_కణుపు కణుపు;
	__u64 ప్రారంభం;
	__u64 లెక్క;
};

నిర్మాణం ext2fs_rb_వ్యక్తిగత {
	నిర్మాణం rb_మూలం మూలం;
	నిర్మాణం bmap_rb_extent *wcursor;
	నిర్మాణం bmap_rb_extent *rcursor;
	నిర్మాణం bmap_rb_extent *rcursor_తదుపరి;
#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
	__u64 mark_తగిలింది;
	__u64 test_తగిలింది;
#అయితే_ముగింపు
};

అంతఃవరుస స్థిర నిర్మాణం bmap_rb_extent *కణుపు_గమ్యం_extent(నిర్మాణం rb_కణుపు *కణుపు)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	తిరిగి (నిర్మాణం bmap_rb_extent *) కణుపు;
}

స్థిర పూర్ణాంకం rb_చొప్పించు_extent(__u64 ప్రారంభం, __u64 లెక్క,
			    నిర్మాణం ext2fs_rb_వ్యక్తిగత *);
స్థిర శూన్య rb_get_new_extent(నిర్మాణం bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#నిర్వచితమైతే DEBUG_RB
స్థిర శూన్య print_వృక్షం(నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం rb_కణుపు *కణుపు = ఖాళీ;
	నిర్మాణం bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	కణుపు = ext2fs_rb_మొదటి(మూలం);
	కొరకు (కణుపు = ext2fs_rb_మొదటి(మూలం); కణుపు != ఖాళీ; 
	     కణుపు = ext2fs_rb_తదుపరి(కణుపు)) {
		ext = కణుపు_గమ్యం_extent(కణుపు);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(గుర్తులేని దీర్ఘ దీర్ఘ) ext->ప్రారంభం,
			(గుర్తులేని దీర్ఘ దీర్ఘ) ext->ప్రారంభం + ext->లెక్క);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

స్థిర శూన్య check_వృక్షం(నిర్మాణం rb_మూలం *మూలం, స్థిరరాశి అక్షరం *సందేశం)
{
	నిర్మాణం rb_కణుపు *కణుపు;
	నిర్మాణం bmap_rb_extent *ext, *old = ఖాళీ;

	కొరకు (కణుపు = ext2fs_rb_మొదటి(మూలం); కణుపు;
	     కణుపు = ext2fs_rb_తదుపరి(కణుపు)) {
		ext = కణుపు_గమ్యం_extent(కణుపు);
		ఒకవేళ (ext->లెక్క == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(గుర్తులేని దీర్ఘ దీర్ఘ) ext->ప్రారంభం,
				(గుర్తులేని దీర్ఘ దీర్ఘ) ext->ప్రారంభం + ext->లెక్క,
				(గుర్తులేని దీర్ఘ దీర్ఘ) ext->లెక్క);
			వెళ్ళు err_బహిర్గతం;
		}
		ఒకవేళ (ext->ప్రారంభం + ext->లెక్క < ext->ప్రారంభం) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(గుర్తులేని దీర్ఘ దీర్ఘ) ext->ప్రారంభం,
				(గుర్తులేని దీర్ఘ దీర్ఘ) ext->ప్రారంభం + ext->లెక్క,
				(గుర్తులేని దీర్ఘ దీర్ఘ) ext->లెక్క);
			వెళ్ళు err_బహిర్గతం;
		}

		ఒకవేళ (old) {
			ఒకవేళ (old->ప్రారంభం > ext->ప్రారంభం) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(గుర్తులేని దీర్ఘ దీర్ఘ) old->ప్రారంభం,
					(గుర్తులేని దీర్ఘ దీర్ఘ) old->ప్రారంభం + old->లెక్క,
					(గుర్తులేని దీర్ఘ దీర్ఘ) old->లెక్క);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(గుర్తులేని దీర్ఘ దీర్ఘ) ext->ప్రారంభం,
					(గుర్తులేని దీర్ఘ దీర్ఘ) ext->ప్రారంభం + ext->లెక్క,
					(గుర్తులేని దీర్ఘ దీర్ఘ) ext->లెక్క);
				వెళ్ళు err_బహిర్గతం;
			}
			ఒకవేళ ((old->ప్రారంభం + old->లెక్క) >= ext->ప్రారంభం) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(గుర్తులేని దీర్ఘ దీర్ఘ) old->ప్రారంభం,
					(గుర్తులేని దీర్ఘ దీర్ఘ) old->ప్రారంభం + old->లెక్క,
					(గుర్తులేని దీర్ఘ దీర్ఘ) old->లెక్క);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(గుర్తులేని దీర్ఘ దీర్ఘ) ext->ప్రారంభం,
					(గుర్తులేని దీర్ఘ దీర్ఘ) ext->ప్రారంభం + ext->లెక్క,
					(గుర్తులేని దీర్ఘ దీర్ఘ) ext->లెక్క);
				వెళ్ళు err_బహిర్గతం;
			}
		}
		old = ext;
	}
	తిరిగి;

err_బహిర్గతం:
	fprintf(stderr, "%s\n", సందేశం);
	print_వృక్షం(మూలం);
	నిష్క్రమించు(1);
}
#లేకపోతే
#నిర్వచించు check_వృక్షం(మూలం, సందేశం) చేయి {} అయితే (0)
#నిర్వచించు print_వృక్షం(మూలం) చేయి {} అయితే (0)
#అయితే_ముగింపు

స్థిర శూన్య rb_get_new_extent(నిర్మాణం bmap_rb_extent **ext, __u64 ప్రారంభం,
			     __u64 లెక్క)
{
	నిర్మాణం bmap_rb_extent *new_ext;
	పూర్ణాంకం retval;

	retval = ext2fs_get_mem(పరిమాణం (నిర్మాణం bmap_rb_extent),
				&new_ext);
	ఒకవేళ (retval)
		abort();

	new_ext->ప్రారంభం = ప్రారంభం;
	new_ext->లెక్క = లెక్క;
	*ext = new_ext;
}

అంతఃవరుస
స్థిర శూన్య rb_free_extent(నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp,
			   నిర్మాణం bmap_rb_extent *ext)
{
	ఒకవేళ (bp->wcursor == ext)
		bp->wcursor = ఖాళీ;
	ఒకవేళ (bp->rcursor == ext)
		bp->rcursor = ఖాళీ;
	ఒకవేళ (bp->rcursor_తదుపరి == ext)
		bp->rcursor_తదుపరి = ఖాళీ;
	ext2fs_free_mem(&ext);
}

స్థిర errcode_t rb_alloc_వ్యక్తిగత_దత్తాంశం (ext2fs_generic_bitmap_64 bitmap)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(పరిమాణం (నిర్మాణం ext2fs_rb_వ్యక్తిగత), &bp);
	ఒకవేళ (retval)
		తిరిగి retval;

	bp->మూలం = RB_మూలం;
	bp->rcursor = ఖాళీ;
	bp->rcursor_తదుపరి = ఖాళీ;
	bp->wcursor = ఖాళీ;

#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
	bp->test_తగిలింది = 0;
	bp->mark_తగిలింది = 0;
#అయితే_ముగింపు

	bitmap->అంతరంగ = (శూన్య *) bp;
	తిరిగి 0;
}

స్థిర errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((వాడని)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_వ్యక్తిగత_దత్తాంశం (bitmap);
	ఒకవేళ (retval)
		తిరిగి retval;

	తిరిగి 0;
}

స్థిర శూన్య rb_free_వృక్షం(నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం bmap_rb_extent *ext;
	నిర్మాణం rb_కణుపు *కణుపు, *తదుపరి;

	కొరకు (కణుపు = ext2fs_rb_మొదటి(మూలం); కణుపు; కణుపు = తదుపరి) {
		తదుపరి = ext2fs_rb_తదుపరి(కణుపు);
		ext = కణుపు_గమ్యం_extent(కణుపు);
		ext2fs_rb_erase(కణుపు, మూలం);
		ext2fs_free_mem(&ext);
	}
}

స్థిర శూన్య rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;

	rb_free_వృక్షం(&bp->మూలం);
	ext2fs_free_mem(&bp);
	bp = 0;
}

స్థిర errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *src_bp, *dest_bp;
	నిర్మాణం bmap_rb_extent *src_ext, *dest_ext;
	నిర్మాణం rb_కణుపు *dest_కణుపు, *src_కణుపు, *dest_చివరి, **n;
	errcode_t retval = 0;

	retval = rb_alloc_వ్యక్తిగత_దత్తాంశం (dest);
	ఒకవేళ (retval)
		తిరిగి retval;

	src_bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) src->అంతరంగ;
	dest_bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) dest->అంతరంగ;
	src_bp->rcursor = ఖాళీ;
	dest_bp->rcursor = ఖాళీ;

	src_కణుపు = ext2fs_rb_మొదటి(&src_bp->మూలం);
	అయితే (src_కణుపు) {
		src_ext = కణుపు_గమ్యం_extent(src_కణుపు);
		retval = ext2fs_get_mem(పరిమాణం (నిర్మాణం bmap_rb_extent),
					&dest_ext);
		ఒకవేళ (retval)
			ఆపు;

		జ్ఞాపకం_నకలు(dest_ext, src_ext, పరిమాణం(నిర్మాణం bmap_rb_extent));

		dest_కణుపు = &dest_ext->కణుపు;
		n = &dest_bp->మూలం.rb_కణుపు;

		dest_చివరి = ఖాళీ;
		ఒకవేళ (*n) {
			dest_చివరి = ext2fs_rb_చివరి(&dest_bp->మూలం);
			n = &(dest_చివరి)->rb_కుడి;
		}

		ext2fs_rb_link_కణుపు(dest_కణుపు, dest_చివరి, n);
		ext2fs_rb_చొప్పించు_color(dest_కణుపు, &dest_bp->మూలం);

		src_కణుపు = ext2fs_rb_తదుపరి(src_కణుపు);
	}

	తిరిగి retval;
}

స్థిర శూన్య rb_truncate(__u64 new_max, నిర్మాణం rb_మూలం *మూలం)
{
	నిర్మాణం bmap_rb_extent *ext;
	నిర్మాణం rb_కణుపు *కణుపు;

	కణుపు = ext2fs_rb_చివరి(మూలం);
	అయితే (కణుపు) {
		ext = కణుపు_గమ్యం_extent(కణుపు);

		ఒకవేళ ((ext->ప్రారంభం + ext->లెక్క - 1) <= new_max)
			ఆపు;
		లేకపోతే ఒకవేళ (ext->ప్రారంభం > new_max) {
			ext2fs_rb_erase(కణుపు, మూలం);
			ext2fs_free_mem(&ext);
			కణుపు = ext2fs_rb_చివరి(మూలం);
			కొనసాగు;
		} లేకపోతే
			ext->లెక్క = new_max - ext->ప్రారంభం + 1;
	}
}

స్థిర errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_చివర, __u64 new_real_చివర)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bmap->అంతరంగ;
	bp->rcursor = ఖాళీ;
	bp->wcursor = ఖాళీ;

	rb_truncate(((new_చివర < bmap->చివర) ? new_చివర : bmap->చివర) - bmap->ప్రారంభం,
		    &bp->మూలం);

	bmap->చివర = new_చివర;
	bmap->real_చివర = new_real_చివర;

	ఒకవేళ (bmap->చివర < bmap->real_చివర)
		rb_చొప్పించు_extent(bmap->చివర + 1 - bmap->ప్రారంభం,
				 bmap->real_చివర - bmap->చివర, bp);
	తిరిగి 0;

}

అంతఃవరుస స్థిర పూర్ణాంకం
rb_test_బిట్టు(నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp, __u64 బిట్టు)
{
	నిర్మాణం bmap_rb_extent *rcursor, *తదుపరి_ext = ఖాళీ;
	నిర్మాణం rb_కణుపు *మాతృ = ఖాళీ, *తదుపరి;
	నిర్మాణం rb_కణుపు **n = &bp->మూలం.rb_కణుపు;
	నిర్మాణం bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	ఒకవేళ (!rcursor)
		వెళ్ళు శోధన_వృక్షం;

	ఒకవేళ (బిట్టు >= rcursor->ప్రారంభం && బిట్టు < rcursor->ప్రారంభం + rcursor->లెక్క) {
#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
		bp->test_తగిలింది++;
#అయితే_ముగింపు
		తిరిగి 1;
	}

	తదుపరి_ext = bp->rcursor_తదుపరి;
	ఒకవేళ (!తదుపరి_ext) {
		తదుపరి = ext2fs_rb_తదుపరి(&rcursor->కణుపు);
		ఒకవేళ (తదుపరి)
			తదుపరి_ext = కణుపు_గమ్యం_extent(తదుపరి);
		bp->rcursor_తదుపరి = తదుపరి_ext;
	}
	ఒకవేళ (తదుపరి_ext) {
		ఒకవేళ ((బిట్టు >= rcursor->ప్రారంభం + rcursor->లెక్క) &&
		    (బిట్టు < తదుపరి_ext->ప్రారంభం)) {
#నిర్వచితమైతే BMAP_STATS_OPS
			bp->test_తగిలింది++;
#అయితే_ముగింపు
			తిరిగి 0;
		}
	}
	bp->rcursor = ఖాళీ;
	bp->rcursor_తదుపరి = ఖాళీ;

	rcursor = bp->wcursor;
	ఒకవేళ (!rcursor)
		వెళ్ళు శోధన_వృక్షం;

	ఒకవేళ (బిట్టు >= rcursor->ప్రారంభం && బిట్టు < rcursor->ప్రారంభం + rcursor->లెక్క)
		తిరిగి 1;

శోధన_వృక్షం:

	అయితే (*n) {
		మాతృ = *n;
		ext = కణుపు_గమ్యం_extent(మాతృ);
		ఒకవేళ (బిట్టు < ext->ప్రారంభం)
			n = &(*n)->rb_ఎడమ;
		లేకపోతే ఒకవేళ (బిట్టు >= (ext->ప్రారంభం + ext->లెక్క))
			n = &(*n)->rb_కుడి;
		లేకపోతే {
			bp->rcursor = ext;
			bp->rcursor_తదుపరి = ఖాళీ;
			తిరిగి 1;
		}
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం rb_చొప్పించు_extent(__u64 ప్రారంభం, __u64 లెక్క,
			    నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp)
{
	నిర్మాణం rb_మూలం *మూలం = &bp->మూలం;
	నిర్మాణం rb_కణుపు *మాతృ = ఖాళీ, **n = &మూలం->rb_కణుపు;
	నిర్మాణం rb_కణుపు *new_కణుపు, *కణుపు, *తదుపరి;
	నిర్మాణం bmap_rb_extent *new_ext;
	నిర్మాణం bmap_rb_extent *ext;
	పూర్ణాంకం retval = 0;

	ఒకవేళ (లెక్క == 0)
		తిరిగి 0;

	bp->rcursor_తదుపరి = ఖాళీ;
	ext = bp->wcursor;
	ఒకవేళ (ext) {
		ఒకవేళ (ప్రారంభం >= ext->ప్రారంభం &&
		    ప్రారంభం <= (ext->ప్రారంభం + ext->లెక్క)) {
#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
			bp->mark_తగిలింది++;
#అయితే_ముగింపు
			వెళ్ళు got_extent;
		}
	}

	అయితే (*n) {
		మాతృ = *n;
		ext = కణుపు_గమ్యం_extent(మాతృ);

		ఒకవేళ (ప్రారంభం < ext->ప్రారంభం) {
			n = &(*n)->rb_ఎడమ;
		} లేకపోతే ఒకవేళ (ప్రారంభం > (ext->ప్రారంభం + ext->లెక్క)) {
			n = &(*n)->rb_కుడి;
		} లేకపోతే {
got_extent:
			ఒకవేళ ((ప్రారంభం + లెక్క) <= (ext->ప్రారంభం + ext->లెక్క))
				తిరిగి 1;

			ఒకవేళ ((ext->ప్రారంభం + ext->లెక్క) == ప్రారంభం)
				retval = 0;
			లేకపోతే
				retval = 1;

			లెక్క += (ప్రారంభం - ext->ప్రారంభం);
			ప్రారంభం = ext->ప్రారంభం;
			new_ext = ext;
			new_కణుపు = &ext->కణుపు;

			వెళ్ళు skip_చొప్పించు;
		}
	}

	rb_get_new_extent(&new_ext, ప్రారంభం, లెక్క);

	new_కణుపు = &new_ext->కణుపు;
	ext2fs_rb_link_కణుపు(new_కణుపు, మాతృ, n);
	ext2fs_rb_చొప్పించు_color(new_కణుపు, మూలం);
	bp->wcursor = new_ext;

	కణుపు = ext2fs_rb_prev(new_కణుపు);
	ఒకవేళ (కణుపు) {
		ext = కణుపు_గమ్యం_extent(కణుపు);
		ఒకవేళ ((ext->ప్రారంభం + ext->లెక్క) == ప్రారంభం) {
			ప్రారంభం = ext->ప్రారంభం;
			లెక్క += ext->లెక్క;
			ext2fs_rb_erase(కణుపు, మూలం);
			rb_free_extent(bp, ext);
		}
	}

skip_చొప్పించు:
	/* See if we can merge extent to the right */
	కొరకు (కణుపు = ext2fs_rb_తదుపరి(new_కణుపు); కణుపు != ఖాళీ; కణుపు = తదుపరి) {
		తదుపరి = ext2fs_rb_తదుపరి(కణుపు);
		ext = కణుపు_గమ్యం_extent(కణుపు);

		ఒకవేళ ((ext->ప్రారంభం + ext->లెక్క) <= ప్రారంభం)
			కొనసాగు;

		/* No more merging */
		ఒకవేళ ((ప్రారంభం + లెక్క) < ext->ప్రారంభం)
			ఆపు;

		/* ext is embedded in new_ext interval */
		ఒకవేళ ((ప్రారంభం + లెక్క) >= (ext->ప్రారంభం + ext->లెక్క)) {
			ext2fs_rb_erase(కణుపు, మూలం);
			rb_free_extent(bp, ext);
			కొనసాగు;
		} లేకపోతే {
		/* merge ext with new_ext */
			లెక్క += ((ext->ప్రారంభం + ext->లెక్క) -
				  (ప్రారంభం + లెక్క));
			ext2fs_rb_erase(కణుపు, మూలం);
			rb_free_extent(bp, ext);
			ఆపు;
		}
	}

	new_ext->ప్రారంభం = ప్రారంభం;
	new_ext->లెక్క = లెక్క;

	తిరిగి retval;
}

స్థిర పూర్ణాంకం rb_remove_extent(__u64 ప్రారంభం, __u64 లెక్క,
			    నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp)
{
	నిర్మాణం rb_మూలం *మూలం = &bp->మూలం;
	నిర్మాణం rb_కణుపు *మాతృ = ఖాళీ, **n = &మూలం->rb_కణుపు;
	నిర్మాణం rb_కణుపు *కణుపు;
	నిర్మాణం bmap_rb_extent *ext;
	__u64 new_ప్రారంభం, new_లెక్క;
	పూర్ణాంకం retval = 0;

	ఒకవేళ (ext2fs_rb_ఖాళీలు_మూలం(మూలం))
		తిరిగి 0;

	అయితే (*n) {
		మాతృ = *n;
		ext = కణుపు_గమ్యం_extent(మాతృ);
		ఒకవేళ (ప్రారంభం < ext->ప్రారంభం) {
			n = &(*n)->rb_ఎడమ;
			కొనసాగు;
		} లేకపోతే ఒకవేళ (ప్రారంభం >= (ext->ప్రారంభం + ext->లెక్క)) {
			n = &(*n)->rb_కుడి;
			కొనసాగు;
		}

		ఒకవేళ ((ప్రారంభం > ext->ప్రారంభం) &&
		    (ప్రారంభం + లెక్క) < (ext->ప్రారంభం + ext->లెక్క)) {
			/* We have to split extent into two */
			new_ప్రారంభం = ప్రారంభం + లెక్క;
			new_లెక్క = (ext->ప్రారంభం + ext->లెక్క) - new_ప్రారంభం;

			ext->లెక్క = ప్రారంభం - ext->ప్రారంభం;

			rb_చొప్పించు_extent(new_ప్రారంభం, new_లెక్క, bp);
			తిరిగి 1;
		}

		ఒకవేళ ((ప్రారంభం + లెక్క) >= (ext->ప్రారంభం + ext->లెక్క)) {
			ext->లెక్క = ప్రారంభం - ext->ప్రారంభం;
			retval = 1;
		}

		ఒకవేళ (0 == ext->లెక్క) {
			మాతృ = ext2fs_rb_తదుపరి(&ext->కణుపు);
			ext2fs_rb_erase(&ext->కణుపు, మూలం);
			rb_free_extent(bp, ext);
			ఆపు;
		}

		ఒకవేళ (ప్రారంభం == ext->ప్రారంభం) {
			ext->ప్రారంభం += లెక్క;
			ext->లెక్క -= లెక్క;
			తిరిగి 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	కొరకు (; మాతృ != ఖాళీ; మాతృ = కణుపు) {
		కణుపు = ext2fs_rb_తదుపరి(మాతృ);
		ext = కణుపు_గమ్యం_extent(మాతృ);
		ఒకవేళ ((ext->ప్రారంభం + ext->లెక్క) <= ప్రారంభం)
			కొనసాగు;

		/* No more extents to be removed/truncated */
		ఒకవేళ ((ప్రారంభం + లెక్క) < ext->ప్రారంభం)
			ఆపు;

		/* The entire extent is within the region to be removed */
		ఒకవేళ ((ప్రారంభం + లెక్క) >= (ext->ప్రారంభం + ext->లెక్క)) {
			ext2fs_rb_erase(మాతృ, మూలం);
			rb_free_extent(bp, ext);
			retval = 1;
			కొనసాగు;
		} లేకపోతే {
			/* modify the last extent in region to be removed */
			ext->లెక్క -= ((ప్రారంభం + లెక్క) - ext->ప్రారంభం);
			ext->ప్రారంభం = ప్రారంభం + లెక్క;
			retval = 1;
			ఆపు;
		}
	}

	తిరిగి retval;
}

స్థిర పూర్ణాంకం rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;
	పూర్ణాంకం retval;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;
	arg -= bitmap->ప్రారంభం;

	retval = rb_చొప్పించు_extent(arg, 1, bp);
	check_వృక్షం(&bp->మూలం, __func__);
	తిరిగి retval;
}

స్థిర పూర్ణాంకం rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;
	పూర్ణాంకం retval;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;
	arg -= bitmap->ప్రారంభం;

	retval = rb_remove_extent(arg, 1, bp);
	check_వృక్షం(&bp->మూలం, __func__);

	తిరిగి retval;
}

అంతఃవరుస
స్థిర పూర్ణాంకం rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;
	arg -= bitmap->ప్రారంభం;

	తిరిగి rb_test_బిట్టు(bp, arg);
}

స్థిర శూన్య rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				గుర్తులేని పూర్ణాంకం అంకె)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;
	arg -= bitmap->ప్రారంభం;

	rb_చొప్పించు_extent(arg, అంకె, bp);
	check_వృక్షం(&bp->మూలం, __func__);
}

స్థిర శూన్య rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  గుర్తులేని పూర్ణాంకం అంకె)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;
	arg -= bitmap->ప్రారంభం;

	rb_remove_extent(arg, అంకె, bp);
	check_వృక్షం(&bp->మూలం, __func__);
}

స్థిర పూర్ణాంకం rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ప్రారంభం, గుర్తులేని పూర్ణాంకం len)
{
	నిర్మాణం rb_కణుపు *మాతృ = ఖాళీ, **n;
	నిర్మాణం rb_కణుపు *కణుపు, *తదుపరి;
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;
	నిర్మాణం bmap_rb_extent *ext;
	పూర్ణాంకం retval = 1;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;
	n = &bp->మూలం.rb_కణుపు;
	ప్రారంభం -= bitmap->ప్రారంభం;

	ఒకవేళ (len == 0 || ext2fs_rb_ఖాళీలు_మూలం(&bp->మూలం))
		తిరిగి 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	అయితే (*n) {
		మాతృ = *n;
		ext = కణుపు_గమ్యం_extent(మాతృ);
		ఒకవేళ (ప్రారంభం < ext->ప్రారంభం) {
			n = &(*n)->rb_ఎడమ;
		} లేకపోతే ఒకవేళ (ప్రారంభం >= (ext->ప్రారంభం + ext->లెక్క)) {
			n = &(*n)->rb_కుడి;
		} లేకపోతే {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			తిరిగి 0;
		}
	}

	కణుపు = మాతృ;
	అయితే (కణుపు) {
		తదుపరి = ext2fs_rb_తదుపరి(కణుపు);
		ext = కణుపు_గమ్యం_extent(కణుపు);
		కణుపు = తదుపరి;

		ఒకవేళ ((ext->ప్రారంభం + ext->లెక్క) <= ప్రారంభం)
			కొనసాగు;

		/* No more merging */
		ఒకవేళ ((ప్రారంభం + len) <= ext->ప్రారంభం)
			ఆపు;

		retval = 0;
		ఆపు;
	}
	తిరిగి retval;
}

స్థిర errcode_t rb_సమితి_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ప్రారంభం, size_t అంకె, శూన్య *లోపల)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;
	గుర్తులేని అక్షరం *cp = లోపల;
	size_t i;
	పూర్ణాంకం మొదటి_సమితి = -1;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;

	కొరకు (i = 0; i < అంకె; i++) {
		ఒకవేళ ((i & 7) == 0) {
			గుర్తులేని అక్షరం c = cp[i/8];
			ఒకవేళ (c == 0xFF) {
				ఒకవేళ (మొదటి_సమితి == -1)
					మొదటి_సమితి = i;
				i += 7;
				కొనసాగు;
			}
			ఒకవేళ ((c == 0x00) && (మొదటి_సమితి == -1)) {
				i += 7;
				కొనసాగు;
			}
		}
		ఒకవేళ (ext2fs_test_బిట్టు(i, లోపల)) {
			ఒకవేళ (మొదటి_సమితి == -1)
				మొదటి_సమితి = i;
			కొనసాగు;
		}
		ఒకవేళ (మొదటి_సమితి == -1)
			కొనసాగు;

		rb_చొప్పించు_extent(ప్రారంభం + మొదటి_సమితి - bitmap->ప్రారంభం,
				 i - మొదటి_సమితి, bp);
		check_వృక్షం(&bp->మూలం, __func__);
		మొదటి_సమితి = -1;
	}
	ఒకవేళ (మొదటి_సమితి != -1) {
		rb_చొప్పించు_extent(ప్రారంభం + మొదటి_సమితి - bitmap->ప్రారంభం,
				 అంకె - మొదటి_సమితి, bp);
		check_వృక్షం(&bp->మూలం, __func__);
	}

	తిరిగి 0;
}

స్థిర errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ప్రారంభం, size_t అంకె, శూన్య *బహిర్గతం)
{

	నిర్మాణం rb_కణుపు *మాతృ = ఖాళీ, *తదుపరి, **n;
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;
	నిర్మాణం bmap_rb_extent *ext;
	__u64 లెక్క, pos;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;
	n = &bp->మూలం.rb_కణుపు;
	ప్రారంభం -= bitmap->ప్రారంభం;

	ఒకవేళ (ext2fs_rb_ఖాళీలు_మూలం(&bp->మూలం))
		తిరిగి 0;

	అయితే (*n) {
		మాతృ = *n;
		ext = కణుపు_గమ్యం_extent(మాతృ);
		ఒకవేళ (ప్రారంభం < ext->ప్రారంభం) {
			n = &(*n)->rb_ఎడమ;
		} లేకపోతే ఒకవేళ (ప్రారంభం >= (ext->ప్రారంభం + ext->లెక్క)) {
			n = &(*n)->rb_కుడి;
		} లేకపోతే
			ఆపు;
	}

	memset(బహిర్గతం, 0, (అంకె + 7) >> 3);

	కొరకు (; మాతృ != ఖాళీ; మాతృ = తదుపరి) {
		తదుపరి = ext2fs_rb_తదుపరి(మాతృ);
		ext = కణుపు_గమ్యం_extent(మాతృ);

		pos = ext->ప్రారంభం;
		లెక్క = ext->లెక్క;
		ఒకవేళ (pos >= ప్రారంభం + అంకె)
			ఆపు;
		ఒకవేళ (pos < ప్రారంభం) {
			ఒకవేళ (pos + లెక్క <  ప్రారంభం)
				కొనసాగు;
			లెక్క -= ప్రారంభం - pos;
			pos = ప్రారంభం;
		}
		ఒకవేళ (pos + లెక్క > ప్రారంభం + అంకె)
			లెక్క = ప్రారంభం + అంకె - pos;

		అయితే (లెక్క > 0) {
			ఒకవేళ ((లెక్క >= 8) &&
			    ((pos - ప్రారంభం) % 8) == 0) {
				పూర్ణాంకం nbytes = లెక్క >> 3;
				పూర్ణాంకం స్థానభ్రంశం = (pos - ప్రారంభం) >> 3;

				memset(((అక్షరం *) బహిర్గతం) + స్థానభ్రంశం, 0xFF, nbytes);
				pos += nbytes << 3;
				లెక్క -= nbytes << 3;
				కొనసాగు;
			}
			ext2fs_fast_సమితి_bit64((pos - ప్రారంభం), బహిర్గతం);
			pos++;
			లెక్క--;
		}
	}
	తిరిగి 0;
}

స్థిర శూన్య rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;

	rb_free_వృక్షం(&bp->మూలం);
	bp->rcursor = ఖాళీ;
	bp->rcursor_తదుపరి = ఖాళీ;
	bp->wcursor = ఖాళీ;
	check_వృక్షం(&bp->మూలం, __func__);
}

స్థిర errcode_t rb_కనుగొను_మొదటి_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 ప్రారంభం, __u64 చివర, __u64 *బహిర్గతం)
{
	నిర్మాణం rb_కణుపు *మాతృ = ఖాళీ, **n;
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;
	నిర్మాణం bmap_rb_extent *ext;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;
	n = &bp->మూలం.rb_కణుపు;
	ప్రారంభం -= bitmap->ప్రారంభం;
	చివర -= bitmap->ప్రారంభం;

	ఒకవేళ (ప్రారంభం > చివర)
		తిరిగి EINVAL;

	ఒకవేళ (ext2fs_rb_ఖాళీలు_మూలం(&bp->మూలం))
		తిరిగి ENOENT;

	అయితే (*n) {
		మాతృ = *n;
		ext = కణుపు_గమ్యం_extent(మాతృ);
		ఒకవేళ (ప్రారంభం < ext->ప్రారంభం) {
			n = &(*n)->rb_ఎడమ;
		} లేకపోతే ఒకవేళ (ప్రారంభం >= (ext->ప్రారంభం + ext->లెక్క)) {
			n = &(*n)->rb_కుడి;
		} లేకపోతే ఒకవేళ (ext->ప్రారంభం + ext->లెక్క <= చివర) {
			*బహిర్గతం = ext->ప్రారంభం + ext->లెక్క + bitmap->ప్రారంభం;
			తిరిగి 0;
		} లేకపోతే
			తిరిగి ENOENT;
	}

	*బహిర్గతం = ప్రారంభం + bitmap->ప్రారంభం;
	తిరిగి 0;
}

స్థిర errcode_t rb_కనుగొను_మొదటి_సమితి(ext2fs_generic_bitmap_64 bitmap,
				   __u64 ప్రారంభం, __u64 చివర, __u64 *బహిర్గతం)
{
	నిర్మాణం rb_కణుపు *మాతృ = ఖాళీ, **n;
	నిర్మాణం rb_కణుపు *కణుపు;
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;
	నిర్మాణం bmap_rb_extent *ext;

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;
	n = &bp->మూలం.rb_కణుపు;
	ప్రారంభం -= bitmap->ప్రారంభం;
	చివర -= bitmap->ప్రారంభం;

	ఒకవేళ (ప్రారంభం > చివర)
		తిరిగి EINVAL;

	ఒకవేళ (ext2fs_rb_ఖాళీలు_మూలం(&bp->మూలం))
		తిరిగి ENOENT;

	అయితే (*n) {
		మాతృ = *n;
		ext = కణుపు_గమ్యం_extent(మాతృ);
		ఒకవేళ (ప్రారంభం < ext->ప్రారంభం) {
			n = &(*n)->rb_ఎడమ;
		} లేకపోతే ఒకవేళ (ప్రారంభం >= (ext->ప్రారంభం + ext->లెక్క)) {
			n = &(*n)->rb_కుడి;
		} లేకపోతే {
			/* The start bit is set */
			*బహిర్గతం = ప్రారంభం + bitmap->ప్రారంభం;
			తిరిగి 0;
		}
	}

	కణుపు = మాతృ;
	ext = కణుపు_గమ్యం_extent(కణుపు);
	ఒకవేళ (ext->ప్రారంభం < ప్రారంభం) {
		కణుపు = ext2fs_rb_తదుపరి(కణుపు);
		ఒకవేళ (కణుపు == ఖాళీ)
			తిరిగి ENOENT;
		ext = కణుపు_గమ్యం_extent(కణుపు);
	}
	ఒకవేళ (ext->ప్రారంభం <= చివర) {
		*బహిర్గతం = ext->ప్రారంభం + bitmap->ప్రారంభం;
		తిరిగి 0;
	}
	తిరిగి ENOENT;
}

#నిర్వచితమైతే ENABLE_BMAP_STATS
స్థిర శూన్య rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	నిర్మాణం ext2fs_rb_వ్యక్తిగత *bp;
	నిర్మాణం rb_కణుపు *కణుపు = ఖాళీ;
	నిర్మాణం bmap_rb_extent *ext;
	__u64 లెక్క = 0;
	__u64 max_కొలత = 0;
	__u64 min_కొలత = ULONG_MAX;
	__u64 కొలత = 0, avg_కొలత = 0;
	ద్విగుణ eff;
#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	ద్విగుణ m_తగిలింది = 0.0, t_తగిలింది = 0.0;
#అయితే_ముగింపు

	bp = (నిర్మాణం ext2fs_rb_వ్యక్తిగత *) bitmap->అంతరంగ;

	కొరకు (కణుపు = ext2fs_rb_మొదటి(&bp->మూలం); కణుపు != ఖాళీ;
	     కణుపు = ext2fs_rb_తదుపరి(కణుపు)) {
		ext = కణుపు_గమ్యం_extent(కణుపు);
		లెక్క++;
		ఒకవేళ (ext->లెక్క > max_కొలత)
			max_కొలత = ext->లెక్క;
		ఒకవేళ (ext->లెక్క < min_కొలత)
			min_కొలత = ext->లెక్క;
		కొలత += ext->లెక్క;
	}

	ఒకవేళ (లెక్క)
		avg_కొలత = కొలత / లెక్క;
	ఒకవేళ (min_కొలత == ULONG_MAX)
		min_కొలత = 0;
	eff = (ద్విగుణ)((లెక్క * పరిమాణం(నిర్మాణం bmap_rb_extent)) << 3) /
	      (bitmap->real_చివర - bitmap->ప్రారంభం);
#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_లెక్క + bitmap->stats.mark_ext_లెక్క;
	test_all = bitmap->stats.test_లెక్క + bitmap->stats.test_ext_లెక్క;
	ఒకవేళ (mark_all)
		m_తగిలింది = ((ద్విగుణ)bp->mark_తగిలింది / mark_all) * 100;
	ఒకవేళ (test_all)
		t_తగిలింది = ((ద్విగుణ)bp->test_తగిలింది / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_తగిలింది, t_తగిలింది, bp->mark_తగిలింది, m_తగిలింది);
#అయితే_ముగింపు
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(గుర్తులేని దీర్ఘ దీర్ఘ) లెక్క, (గుర్తులేని దీర్ఘ దీర్ఘ)
		((లెక్క * పరిమాణం(నిర్మాణం bmap_rb_extent)) +
		 పరిమాణం(నిర్మాణం ext2fs_rb_వ్యక్తిగత)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(గుర్తులేని దీర్ఘ దీర్ఘ) min_కొలత);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(గుర్తులేని దీర్ఘ దీర్ఘ) max_కొలత, (గుర్తులేని దీర్ఘ దీర్ఘ) avg_కొలత);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(గుర్తులేని దీర్ఘ దీర్ఘ) కొలత,
		(గుర్తులేని దీర్ఘ దీర్ఘ) bitmap->real_చివర - bitmap->ప్రారంభం);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#లేకపోతే
స్థిర శూన్య rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((వాడని)))
{
}
#అయితే_ముగింపు

నిర్మాణం ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
	.సమితి_bmap_range = rb_సమితి_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.కనుగొను_మొదటి_zero = rb_కనుగొను_మొదటి_zero,
	.కనుగొను_మొదటి_సమితి = rb_కనుగొను_మొదటి_సమితి,
};
