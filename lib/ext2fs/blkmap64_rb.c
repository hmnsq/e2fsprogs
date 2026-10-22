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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#यदि HAVE_LINUX_TYPES_H
#अन्तर्भाव <linux/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "bmap64.h"
#अन्तर्भाव "rbtree.h"

#अन्तर्भाव <limits.h>

संरचना bmap_rb_extent {
	संरचना rb_ग्रन्थिः ग्रन्थिः;
	__u64 start;
	__u64 गणनम्;
};

संरचना ext2fs_rb_वैयक्तिकम् {
	संरचना rb_मूलम् मूलम्;
	संरचना bmap_rb_extent *wcursor;
	संरचना bmap_rb_extent *rcursor;
	संरचना bmap_rb_extent *rcursor_अग्र;
#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
	__u64 mark_लब्धम्;
	__u64 test_लब्धम्;
#यदि_अन्तः
};

अन्तःस्थ स्थिर संरचना bmap_rb_extent *ग्रन्थिः_गन्तव्यम्_extent(संरचना rb_ग्रन्थिः *ग्रन्थिः)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	प्रत्यागम (संरचना bmap_rb_extent *) ग्रन्थिः;
}

स्थिर पूर्णाङ्क rb_निवेशय_extent(__u64 start, __u64 गणनम्,
			    संरचना ext2fs_rb_वैयक्तिकम् *);
स्थिर शून्य rb_get_new_extent(संरचना bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#यदि_परिभाषितम् DEBUG_RB
स्थिर शून्य print_वृक्षः(संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *ग्रन्थिः = रिक्त;
	संरचना bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	ग्रन्थिः = ext2fs_rb_first(मूलम्);
	प्रति (ग्रन्थिः = ext2fs_rb_first(मूलम्); ग्रन्थिः != रिक्त; 
	     ग्रन्थिः = ext2fs_rb_अग्र(ग्रन्थिः)) {
		ext = ग्रन्थिः_गन्तव्यम्_extent(ग्रन्थिः);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(अचिह्नित दीर्घ दीर्घ) ext->start,
			(अचिह्नित दीर्घ दीर्घ) ext->start + ext->गणनम्);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

स्थिर शून्य check_वृक्षः(संरचना rb_मूलम् *मूलम्, ध्रुव वर्ण *सन्देशः)
{
	संरचना rb_ग्रन्थिः *ग्रन्थिः;
	संरचना bmap_rb_extent *ext, *old = रिक्त;

	प्रति (ग्रन्थिः = ext2fs_rb_first(मूलम्); ग्रन्थिः;
	     ग्रन्थिः = ext2fs_rb_अग्र(ग्रन्थिः)) {
		ext = ग्रन्थिः_गन्तव्यम्_extent(ग्रन्थिः);
		यदि (ext->गणनम् == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(अचिह्नित दीर्घ दीर्घ) ext->start,
				(अचिह्नित दीर्घ दीर्घ) ext->start + ext->गणनम्,
				(अचिह्नित दीर्घ दीर्घ) ext->गणनम्);
			गच्छ err_बहिर्गमनम्;
		}
		यदि (ext->start + ext->गणनम् < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(अचिह्नित दीर्घ दीर्घ) ext->start,
				(अचिह्नित दीर्घ दीर्घ) ext->start + ext->गणनम्,
				(अचिह्नित दीर्घ दीर्घ) ext->गणनम्);
			गच्छ err_बहिर्गमनम्;
		}

		यदि (old) {
			यदि (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(अचिह्नित दीर्घ दीर्घ) old->start,
					(अचिह्नित दीर्घ दीर्घ) old->start + old->गणनम्,
					(अचिह्नित दीर्घ दीर्घ) old->गणनम्);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(अचिह्नित दीर्घ दीर्घ) ext->start,
					(अचिह्नित दीर्घ दीर्घ) ext->start + ext->गणनम्,
					(अचिह्नित दीर्घ दीर्घ) ext->गणनम्);
				गच्छ err_बहिर्गमनम्;
			}
			यदि ((old->start + old->गणनम्) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(अचिह्नित दीर्घ दीर्घ) old->start,
					(अचिह्नित दीर्घ दीर्घ) old->start + old->गणनम्,
					(अचिह्नित दीर्घ दीर्घ) old->गणनम्);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(अचिह्नित दीर्घ दीर्घ) ext->start,
					(अचिह्नित दीर्घ दीर्घ) ext->start + ext->गणनम्,
					(अचिह्नित दीर्घ दीर्घ) ext->गणनम्);
				गच्छ err_बहिर्गमनम्;
			}
		}
		old = ext;
	}
	प्रत्यागम;

err_बहिर्गमनम्:
	fprintf(stderr, "%s\n", सन्देशः);
	print_वृक्षः(मूलम्);
	निर्गच्छ(1);
}
#अन्यथा
#परिभाषय check_वृक्षः(मूलम्, सन्देशः) कुरु {} यावत् (0)
#परिभाषय print_वृक्षः(मूलम्) कुरु {} यावत् (0)
#यदि_अन्तः

स्थिर शून्य rb_get_new_extent(संरचना bmap_rb_extent **ext, __u64 start,
			     __u64 गणनम्)
{
	संरचना bmap_rb_extent *new_ext;
	पूर्णाङ्क retval;

	retval = ext2fs_get_mem(परिमाण (संरचना bmap_rb_extent),
				&new_ext);
	यदि (retval)
		abort();

	new_ext->start = start;
	new_ext->गणनम् = गणनम्;
	*ext = new_ext;
}

अन्तःस्थ
स्थिर शून्य rb_free_extent(संरचना ext2fs_rb_वैयक्तिकम् *bp,
			   संरचना bmap_rb_extent *ext)
{
	यदि (bp->wcursor == ext)
		bp->wcursor = रिक्त;
	यदि (bp->rcursor == ext)
		bp->rcursor = रिक्त;
	यदि (bp->rcursor_अग्र == ext)
		bp->rcursor_अग्र = रिक्त;
	ext2fs_free_mem(&ext);
}

स्थिर errcode_t rb_alloc_वैयक्तिकम्_दत्त (ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(परिमाण (संरचना ext2fs_rb_वैयक्तिकम्), &bp);
	यदि (retval)
		प्रत्यागम retval;

	bp->मूलम् = RB_मूलम्;
	bp->rcursor = रिक्त;
	bp->rcursor_अग्र = रिक्त;
	bp->wcursor = रिक्त;

#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
	bp->test_लब्धम् = 0;
	bp->mark_लब्धम् = 0;
#यदि_अन्तः

	bitmap->गुप्त = (शून्य *) bp;
	प्रत्यागम 0;
}

स्थिर errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((अप्रयुक्तम्)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_वैयक्तिकम्_दत्त (bitmap);
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम 0;
}

स्थिर शून्य rb_free_वृक्षः(संरचना rb_मूलम् *मूलम्)
{
	संरचना bmap_rb_extent *ext;
	संरचना rb_ग्रन्थिः *ग्रन्थिः, *अग्र;

	प्रति (ग्रन्थिः = ext2fs_rb_first(मूलम्); ग्रन्थिः; ग्रन्थिः = अग्र) {
		अग्र = ext2fs_rb_अग्र(ग्रन्थिः);
		ext = ग्रन्थिः_गन्तव्यम्_extent(ग्रन्थिः);
		ext2fs_rb_erase(ग्रन्थिः, मूलम्);
		ext2fs_free_mem(&ext);
	}
}

स्थिर शून्य rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;

	rb_free_वृक्षः(&bp->मूलम्);
	ext2fs_free_mem(&bp);
	bp = 0;
}

स्थिर errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	संरचना ext2fs_rb_वैयक्तिकम् *src_bp, *dest_bp;
	संरचना bmap_rb_extent *src_ext, *dest_ext;
	संरचना rb_ग्रन्थिः *dest_ग्रन्थिः, *src_ग्रन्थिः, *dest_चरमः, **n;
	errcode_t retval = 0;

	retval = rb_alloc_वैयक्तिकम्_दत्त (dest);
	यदि (retval)
		प्रत्यागम retval;

	src_bp = (संरचना ext2fs_rb_वैयक्तिकम् *) src->गुप्त;
	dest_bp = (संरचना ext2fs_rb_वैयक्तिकम् *) dest->गुप्त;
	src_bp->rcursor = रिक्त;
	dest_bp->rcursor = रिक्त;

	src_ग्रन्थिः = ext2fs_rb_first(&src_bp->मूलम्);
	यावत् (src_ग्रन्थिः) {
		src_ext = ग्रन्थिः_गन्तव्यम्_extent(src_ग्रन्थिः);
		retval = ext2fs_get_mem(परिमाण (संरचना bmap_rb_extent),
					&dest_ext);
		यदि (retval)
			भङ्ग;

		स्मृतिं_प्रतिलिख(dest_ext, src_ext, परिमाण(संरचना bmap_rb_extent));

		dest_ग्रन्थिः = &dest_ext->ग्रन्थिः;
		n = &dest_bp->मूलम्.rb_ग्रन्थिः;

		dest_चरमः = रिक्त;
		यदि (*n) {
			dest_चरमः = ext2fs_rb_चरमः(&dest_bp->मूलम्);
			n = &(dest_चरमः)->rb_दक्षिणः;
		}

		ext2fs_rb_link_ग्रन्थिः(dest_ग्रन्थिः, dest_चरमः, n);
		ext2fs_rb_निवेशय_color(dest_ग्रन्थिः, &dest_bp->मूलम्);

		src_ग्रन्थिः = ext2fs_rb_अग्र(src_ग्रन्थिः);
	}

	प्रत्यागम retval;
}

स्थिर शून्य rb_truncate(__u64 new_max, संरचना rb_मूलम् *मूलम्)
{
	संरचना bmap_rb_extent *ext;
	संरचना rb_ग्रन्थिः *ग्रन्थिः;

	ग्रन्थिः = ext2fs_rb_चरमः(मूलम्);
	यावत् (ग्रन्थिः) {
		ext = ग्रन्थिः_गन्तव्यम्_extent(ग्रन्थिः);

		यदि ((ext->start + ext->गणनम् - 1) <= new_max)
			भङ्ग;
		अन्यथा यदि (ext->start > new_max) {
			ext2fs_rb_erase(ग्रन्थिः, मूलम्);
			ext2fs_free_mem(&ext);
			ग्रन्थिः = ext2fs_rb_चरमः(मूलम्);
			अनुवर्त;
		} अन्यथा
			ext->गणनम् = new_max - ext->start + 1;
	}
}

स्थिर errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_अन्तः, __u64 new_real_अन्तः)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bmap->गुप्त;
	bp->rcursor = रिक्त;
	bp->wcursor = रिक्त;

	rb_truncate(((new_अन्तः < bmap->अन्तः) ? new_अन्तः : bmap->अन्तः) - bmap->start,
		    &bp->मूलम्);

	bmap->अन्तः = new_अन्तः;
	bmap->real_अन्तः = new_real_अन्तः;

	यदि (bmap->अन्तः < bmap->real_अन्तः)
		rb_निवेशय_extent(bmap->अन्तः + 1 - bmap->start,
				 bmap->real_अन्तः - bmap->अन्तः, bp);
	प्रत्यागम 0;

}

अन्तःस्थ स्थिर पूर्णाङ्क
rb_test_bit(संरचना ext2fs_rb_वैयक्तिकम् *bp, __u64 bit)
{
	संरचना bmap_rb_extent *rcursor, *अग्र_ext = रिक्त;
	संरचना rb_ग्रन्थिः *parent = रिक्त, *अग्र;
	संरचना rb_ग्रन्थिः **n = &bp->मूलम्.rb_ग्रन्थिः;
	संरचना bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	यदि (!rcursor)
		गच्छ अन्वेषण_वृक्षः;

	यदि (bit >= rcursor->start && bit < rcursor->start + rcursor->गणनम्) {
#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
		bp->test_लब्धम्++;
#यदि_अन्तः
		प्रत्यागम 1;
	}

	अग्र_ext = bp->rcursor_अग्र;
	यदि (!अग्र_ext) {
		अग्र = ext2fs_rb_अग्र(&rcursor->ग्रन्थिः);
		यदि (अग्र)
			अग्र_ext = ग्रन्थिः_गन्तव्यम्_extent(अग्र);
		bp->rcursor_अग्र = अग्र_ext;
	}
	यदि (अग्र_ext) {
		यदि ((bit >= rcursor->start + rcursor->गणनम्) &&
		    (bit < अग्र_ext->start)) {
#यदि_परिभाषितम् BMAP_STATS_OPS
			bp->test_लब्धम्++;
#यदि_अन्तः
			प्रत्यागम 0;
		}
	}
	bp->rcursor = रिक्त;
	bp->rcursor_अग्र = रिक्त;

	rcursor = bp->wcursor;
	यदि (!rcursor)
		गच्छ अन्वेषण_वृक्षः;

	यदि (bit >= rcursor->start && bit < rcursor->start + rcursor->गणनम्)
		प्रत्यागम 1;

अन्वेषण_वृक्षः:

	यावत् (*n) {
		parent = *n;
		ext = ग्रन्थिः_गन्तव्यम्_extent(parent);
		यदि (bit < ext->start)
			n = &(*n)->rb_वामः;
		अन्यथा यदि (bit >= (ext->start + ext->गणनम्))
			n = &(*n)->rb_दक्षिणः;
		अन्यथा {
			bp->rcursor = ext;
			bp->rcursor_अग्र = रिक्त;
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क rb_निवेशय_extent(__u64 start, __u64 गणनम्,
			    संरचना ext2fs_rb_वैयक्तिकम् *bp)
{
	संरचना rb_मूलम् *मूलम् = &bp->मूलम्;
	संरचना rb_ग्रन्थिः *parent = रिक्त, **n = &मूलम्->rb_ग्रन्थिः;
	संरचना rb_ग्रन्थिः *new_ग्रन्थिः, *ग्रन्थिः, *अग्र;
	संरचना bmap_rb_extent *new_ext;
	संरचना bmap_rb_extent *ext;
	पूर्णाङ्क retval = 0;

	यदि (गणनम् == 0)
		प्रत्यागम 0;

	bp->rcursor_अग्र = रिक्त;
	ext = bp->wcursor;
	यदि (ext) {
		यदि (start >= ext->start &&
		    start <= (ext->start + ext->गणनम्)) {
#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
			bp->mark_लब्धम्++;
#यदि_अन्तः
			गच्छ got_extent;
		}
	}

	यावत् (*n) {
		parent = *n;
		ext = ग्रन्थिः_गन्तव्यम्_extent(parent);

		यदि (start < ext->start) {
			n = &(*n)->rb_वामः;
		} अन्यथा यदि (start > (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
		} अन्यथा {
got_extent:
			यदि ((start + गणनम्) <= (ext->start + ext->गणनम्))
				प्रत्यागम 1;

			यदि ((ext->start + ext->गणनम्) == start)
				retval = 0;
			अन्यथा
				retval = 1;

			गणनम् += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_ग्रन्थिः = &ext->ग्रन्थिः;

			गच्छ skip_निवेशय;
		}
	}

	rb_get_new_extent(&new_ext, start, गणनम्);

	new_ग्रन्थिः = &new_ext->ग्रन्थिः;
	ext2fs_rb_link_ग्रन्थिः(new_ग्रन्थिः, parent, n);
	ext2fs_rb_निवेशय_color(new_ग्रन्थिः, मूलम्);
	bp->wcursor = new_ext;

	ग्रन्थिः = ext2fs_rb_पूर्व(new_ग्रन्थिः);
	यदि (ग्रन्थिः) {
		ext = ग्रन्थिः_गन्तव्यम्_extent(ग्रन्थिः);
		यदि ((ext->start + ext->गणनम्) == start) {
			start = ext->start;
			गणनम् += ext->गणनम्;
			ext2fs_rb_erase(ग्रन्थिः, मूलम्);
			rb_free_extent(bp, ext);
		}
	}

skip_निवेशय:
	/* See if we can merge extent to the right */
	प्रति (ग्रन्थिः = ext2fs_rb_अग्र(new_ग्रन्थिः); ग्रन्थिः != रिक्त; ग्रन्थिः = अग्र) {
		अग्र = ext2fs_rb_अग्र(ग्रन्थिः);
		ext = ग्रन्थिः_गन्तव्यम्_extent(ग्रन्थिः);

		यदि ((ext->start + ext->गणनम्) <= start)
			अनुवर्त;

		/* No more merging */
		यदि ((start + गणनम्) < ext->start)
			भङ्ग;

		/* ext is embedded in new_ext interval */
		यदि ((start + गणनम्) >= (ext->start + ext->गणनम्)) {
			ext2fs_rb_erase(ग्रन्थिः, मूलम्);
			rb_free_extent(bp, ext);
			अनुवर्त;
		} अन्यथा {
		/* merge ext with new_ext */
			गणनम् += ((ext->start + ext->गणनम्) -
				  (start + गणनम्));
			ext2fs_rb_erase(ग्रन्थिः, मूलम्);
			rb_free_extent(bp, ext);
			भङ्ग;
		}
	}

	new_ext->start = start;
	new_ext->गणनम् = गणनम्;

	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क rb_remove_extent(__u64 start, __u64 गणनम्,
			    संरचना ext2fs_rb_वैयक्तिकम् *bp)
{
	संरचना rb_मूलम् *मूलम् = &bp->मूलम्;
	संरचना rb_ग्रन्थिः *parent = रिक्त, **n = &मूलम्->rb_ग्रन्थिः;
	संरचना rb_ग्रन्थिः *ग्रन्थिः;
	संरचना bmap_rb_extent *ext;
	__u64 new_start, new_गणनम्;
	पूर्णाङ्क retval = 0;

	यदि (ext2fs_rb_रिक्तानि_मूलम्(मूलम्))
		प्रत्यागम 0;

	यावत् (*n) {
		parent = *n;
		ext = ग्रन्थिः_गन्तव्यम्_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_वामः;
			अनुवर्त;
		} अन्यथा यदि (start >= (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
			अनुवर्त;
		}

		यदि ((start > ext->start) &&
		    (start + गणनम्) < (ext->start + ext->गणनम्)) {
			/* We have to split extent into two */
			new_start = start + गणनम्;
			new_गणनम् = (ext->start + ext->गणनम्) - new_start;

			ext->गणनम् = start - ext->start;

			rb_निवेशय_extent(new_start, new_गणनम्, bp);
			प्रत्यागम 1;
		}

		यदि ((start + गणनम्) >= (ext->start + ext->गणनम्)) {
			ext->गणनम् = start - ext->start;
			retval = 1;
		}

		यदि (0 == ext->गणनम्) {
			parent = ext2fs_rb_अग्र(&ext->ग्रन्थिः);
			ext2fs_rb_erase(&ext->ग्रन्थिः, मूलम्);
			rb_free_extent(bp, ext);
			भङ्ग;
		}

		यदि (start == ext->start) {
			ext->start += गणनम्;
			ext->गणनम् -= गणनम्;
			प्रत्यागम 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	प्रति (; parent != रिक्त; parent = ग्रन्थिः) {
		ग्रन्थिः = ext2fs_rb_अग्र(parent);
		ext = ग्रन्थिः_गन्तव्यम्_extent(parent);
		यदि ((ext->start + ext->गणनम्) <= start)
			अनुवर्त;

		/* No more extents to be removed/truncated */
		यदि ((start + गणनम्) < ext->start)
			भङ्ग;

		/* The entire extent is within the region to be removed */
		यदि ((start + गणनम्) >= (ext->start + ext->गणनम्)) {
			ext2fs_rb_erase(parent, मूलम्);
			rb_free_extent(bp, ext);
			retval = 1;
			अनुवर्त;
		} अन्यथा {
			/* modify the last extent in region to be removed */
			ext->गणनम् -= ((start + गणनम्) - ext->start);
			ext->start = start + गणनम्;
			retval = 1;
			भङ्ग;
		}
	}

	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;
	पूर्णाङ्क retval;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;
	arg -= bitmap->start;

	retval = rb_निवेशय_extent(arg, 1, bp);
	check_वृक्षः(&bp->मूलम्, __func__);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;
	पूर्णाङ्क retval;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_वृक्षः(&bp->मूलम्, __func__);

	प्रत्यागम retval;
}

अन्तःस्थ
स्थिर पूर्णाङ्क rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;
	arg -= bitmap->start;

	प्रत्यागम rb_test_bit(bp, arg);
}

स्थिर शून्य rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				अचिह्नित पूर्णाङ्क अङ्कः)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;
	arg -= bitmap->start;

	rb_निवेशय_extent(arg, अङ्कः, bp);
	check_वृक्षः(&bp->मूलम्, __func__);
}

स्थिर शून्य rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  अचिह्नित पूर्णाङ्क अङ्कः)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;
	arg -= bitmap->start;

	rb_remove_extent(arg, अङ्कः, bp);
	check_वृक्षः(&bp->मूलम्, __func__);
}

स्थिर पूर्णाङ्क rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, अचिह्नित पूर्णाङ्क len)
{
	संरचना rb_ग्रन्थिः *parent = रिक्त, **n;
	संरचना rb_ग्रन्थिः *ग्रन्थिः, *अग्र;
	संरचना ext2fs_rb_वैयक्तिकम् *bp;
	संरचना bmap_rb_extent *ext;
	पूर्णाङ्क retval = 1;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;
	n = &bp->मूलम्.rb_ग्रन्थिः;
	start -= bitmap->start;

	यदि (len == 0 || ext2fs_rb_रिक्तानि_मूलम्(&bp->मूलम्))
		प्रत्यागम 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	यावत् (*n) {
		parent = *n;
		ext = ग्रन्थिः_गन्तव्यम्_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_वामः;
		} अन्यथा यदि (start >= (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
		} अन्यथा {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			प्रत्यागम 0;
		}
	}

	ग्रन्थिः = parent;
	यावत् (ग्रन्थिः) {
		अग्र = ext2fs_rb_अग्र(ग्रन्थिः);
		ext = ग्रन्थिः_गन्तव्यम्_extent(ग्रन्थिः);
		ग्रन्थिः = अग्र;

		यदि ((ext->start + ext->गणनम्) <= start)
			अनुवर्त;

		/* No more merging */
		यदि ((start + len) <= ext->start)
			भङ्ग;

		retval = 0;
		भङ्ग;
	}
	प्रत्यागम retval;
}

स्थिर errcode_t rb_समुच्चयः_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t अङ्कः, शून्य *अन्तर्)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;
	अचिह्नित वर्ण *cp = अन्तर्;
	size_t i;
	पूर्णाङ्क first_समुच्चयः = -1;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;

	प्रति (i = 0; i < अङ्कः; i++) {
		यदि ((i & 7) == 0) {
			अचिह्नित वर्ण c = cp[i/8];
			यदि (c == 0xFF) {
				यदि (first_समुच्चयः == -1)
					first_समुच्चयः = i;
				i += 7;
				अनुवर्त;
			}
			यदि ((c == 0x00) && (first_समुच्चयः == -1)) {
				i += 7;
				अनुवर्त;
			}
		}
		यदि (ext2fs_test_bit(i, अन्तर्)) {
			यदि (first_समुच्चयः == -1)
				first_समुच्चयः = i;
			अनुवर्त;
		}
		यदि (first_समुच्चयः == -1)
			अनुवर्त;

		rb_निवेशय_extent(start + first_समुच्चयः - bitmap->start,
				 i - first_समुच्चयः, bp);
		check_वृक्षः(&bp->मूलम्, __func__);
		first_समुच्चयः = -1;
	}
	यदि (first_समुच्चयः != -1) {
		rb_निवेशय_extent(start + first_समुच्चयः - bitmap->start,
				 अङ्कः - first_समुच्चयः, bp);
		check_वृक्षः(&bp->मूलम्, __func__);
	}

	प्रत्यागम 0;
}

स्थिर errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t अङ्कः, शून्य *बहिर्गमनम्)
{

	संरचना rb_ग्रन्थिः *parent = रिक्त, *अग्र, **n;
	संरचना ext2fs_rb_वैयक्तिकम् *bp;
	संरचना bmap_rb_extent *ext;
	__u64 गणनम्, pos;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;
	n = &bp->मूलम्.rb_ग्रन्थिः;
	start -= bitmap->start;

	यदि (ext2fs_rb_रिक्तानि_मूलम्(&bp->मूलम्))
		प्रत्यागम 0;

	यावत् (*n) {
		parent = *n;
		ext = ग्रन्थिः_गन्तव्यम्_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_वामः;
		} अन्यथा यदि (start >= (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
		} अन्यथा
			भङ्ग;
	}

	स्मृतिं_पूरय(बहिर्गमनम्, 0, (अङ्कः + 7) >> 3);

	प्रति (; parent != रिक्त; parent = अग्र) {
		अग्र = ext2fs_rb_अग्र(parent);
		ext = ग्रन्थिः_गन्तव्यम्_extent(parent);

		pos = ext->start;
		गणनम् = ext->गणनम्;
		यदि (pos >= start + अङ्कः)
			भङ्ग;
		यदि (pos < start) {
			यदि (pos + गणनम् <  start)
				अनुवर्त;
			गणनम् -= start - pos;
			pos = start;
		}
		यदि (pos + गणनम् > start + अङ्कः)
			गणनम् = start + अङ्कः - pos;

		यावत् (गणनम् > 0) {
			यदि ((गणनम् >= 8) &&
			    ((pos - start) % 8) == 0) {
				पूर्णाङ्क nbytes = गणनम् >> 3;
				पूर्णाङ्क विस्थापनम् = (pos - start) >> 3;

				स्मृतिं_पूरय(((वर्ण *) बहिर्गमनम्) + विस्थापनम्, 0xFF, nbytes);
				pos += nbytes << 3;
				गणनम् -= nbytes << 3;
				अनुवर्त;
			}
			ext2fs_fast_समुच्चयः_bit64((pos - start), बहिर्गमनम्);
			pos++;
			गणनम्--;
		}
	}
	प्रत्यागम 0;
}

स्थिर शून्य rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;

	rb_free_वृक्षः(&bp->मूलम्);
	bp->rcursor = रिक्त;
	bp->rcursor_अग्र = रिक्त;
	bp->wcursor = रिक्त;
	check_वृक्षः(&bp->मूलम्, __func__);
}

स्थिर errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 अन्तः, __u64 *बहिर्गमनम्)
{
	संरचना rb_ग्रन्थिः *parent = रिक्त, **n;
	संरचना ext2fs_rb_वैयक्तिकम् *bp;
	संरचना bmap_rb_extent *ext;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;
	n = &bp->मूलम्.rb_ग्रन्थिः;
	start -= bitmap->start;
	अन्तः -= bitmap->start;

	यदि (start > अन्तः)
		प्रत्यागम EINVAL;

	यदि (ext2fs_rb_रिक्तानि_मूलम्(&bp->मूलम्))
		प्रत्यागम ENOENT;

	यावत् (*n) {
		parent = *n;
		ext = ग्रन्थिः_गन्तव्यम्_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_वामः;
		} अन्यथा यदि (start >= (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
		} अन्यथा यदि (ext->start + ext->गणनम् <= अन्तः) {
			*बहिर्गमनम् = ext->start + ext->गणनम् + bitmap->start;
			प्रत्यागम 0;
		} अन्यथा
			प्रत्यागम ENOENT;
	}

	*बहिर्गमनम् = start + bitmap->start;
	प्रत्यागम 0;
}

स्थिर errcode_t rb_find_first_समुच्चयः(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 अन्तः, __u64 *बहिर्गमनम्)
{
	संरचना rb_ग्रन्थिः *parent = रिक्त, **n;
	संरचना rb_ग्रन्थिः *ग्रन्थिः;
	संरचना ext2fs_rb_वैयक्तिकम् *bp;
	संरचना bmap_rb_extent *ext;

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;
	n = &bp->मूलम्.rb_ग्रन्थिः;
	start -= bitmap->start;
	अन्तः -= bitmap->start;

	यदि (start > अन्तः)
		प्रत्यागम EINVAL;

	यदि (ext2fs_rb_रिक्तानि_मूलम्(&bp->मूलम्))
		प्रत्यागम ENOENT;

	यावत् (*n) {
		parent = *n;
		ext = ग्रन्थिः_गन्तव्यम्_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_वामः;
		} अन्यथा यदि (start >= (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
		} अन्यथा {
			/* The start bit is set */
			*बहिर्गमनम् = start + bitmap->start;
			प्रत्यागम 0;
		}
	}

	ग्रन्थिः = parent;
	ext = ग्रन्थिः_गन्तव्यम्_extent(ग्रन्थिः);
	यदि (ext->start < start) {
		ग्रन्थिः = ext2fs_rb_अग्र(ग्रन्थिः);
		यदि (ग्रन्थिः == रिक्त)
			प्रत्यागम ENOENT;
		ext = ग्रन्थिः_गन्तव्यम्_extent(ग्रन्थिः);
	}
	यदि (ext->start <= अन्तः) {
		*बहिर्गमनम् = ext->start + bitmap->start;
		प्रत्यागम 0;
	}
	प्रत्यागम ENOENT;
}

#यदि_परिभाषितम् ENABLE_BMAP_STATS
स्थिर शून्य rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_वैयक्तिकम् *bp;
	संरचना rb_ग्रन्थिः *ग्रन्थिः = रिक्त;
	संरचना bmap_rb_extent *ext;
	__u64 गणनम् = 0;
	__u64 max_आकार = 0;
	__u64 min_आकार = ULONG_MAX;
	__u64 आकार = 0, avg_आकार = 0;
	द्विगुण eff;
#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	द्विगुण m_लब्धम् = 0.0, t_लब्धम् = 0.0;
#यदि_अन्तः

	bp = (संरचना ext2fs_rb_वैयक्तिकम् *) bitmap->गुप्त;

	प्रति (ग्रन्थिः = ext2fs_rb_first(&bp->मूलम्); ग्रन्थिः != रिक्त;
	     ग्रन्थिः = ext2fs_rb_अग्र(ग्रन्थिः)) {
		ext = ग्रन्थिः_गन्तव्यम्_extent(ग्रन्थिः);
		गणनम्++;
		यदि (ext->गणनम् > max_आकार)
			max_आकार = ext->गणनम्;
		यदि (ext->गणनम् < min_आकार)
			min_आकार = ext->गणनम्;
		आकार += ext->गणनम्;
	}

	यदि (गणनम्)
		avg_आकार = आकार / गणनम्;
	यदि (min_आकार == ULONG_MAX)
		min_आकार = 0;
	eff = (द्विगुण)((गणनम् * परिमाण(संरचना bmap_rb_extent)) << 3) /
	      (bitmap->real_अन्तः - bitmap->start);
#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_गणनम् + bitmap->stats.mark_ext_गणनम्;
	test_all = bitmap->stats.test_गणनम् + bitmap->stats.test_ext_गणनम्;
	यदि (mark_all)
		m_लब्धम् = ((द्विगुण)bp->mark_लब्धम् / mark_all) * 100;
	यदि (test_all)
		t_लब्धम् = ((द्विगुण)bp->test_लब्धम् / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_लब्धम्, t_लब्धम्, bp->mark_लब्धम्, m_लब्धम्);
#यदि_अन्तः
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(अचिह्नित दीर्घ दीर्घ) गणनम्, (अचिह्नित दीर्घ दीर्घ)
		((गणनम् * परिमाण(संरचना bmap_rb_extent)) +
		 परिमाण(संरचना ext2fs_rb_वैयक्तिकम्)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(अचिह्नित दीर्घ दीर्घ) min_आकार);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(अचिह्नित दीर्घ दीर्घ) max_आकार, (अचिह्नित दीर्घ दीर्घ) avg_आकार);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(अचिह्नित दीर्घ दीर्घ) आकार,
		(अचिह्नित दीर्घ दीर्घ) bitmap->real_अन्तः - bitmap->start);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#अन्यथा
स्थिर शून्य rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((अप्रयुक्तम्)))
{
}
#यदि_अन्तः

संरचना ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
	.समुच्चयः_bmap_range = rb_समुच्चयः_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.find_first_zero = rb_find_first_zero,
	.find_first_समुच्चयः = rb_find_first_समुच्चयः,
};
