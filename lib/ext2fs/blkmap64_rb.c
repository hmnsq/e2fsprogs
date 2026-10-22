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
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जइ HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जइ HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#जइ HAVE_LINUX_TYPES_H
#अन्तर्भाव <linux/types.h>
#endif

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

संरचना ext2fs_rb_private {
	संरचना rb_मूलम् मूलम्;
	संरचना bmap_rb_extent *wcursor;
	संरचना bmap_rb_extent *rcursor;
	संरचना bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

अन्तःस्थ स्थिर संरचना bmap_rb_extent *ग्रन्थिः_to_extent(संरचना rb_ग्रन्थिः *ग्रन्थिः)
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
			    संरचना ext2fs_rb_private *);
स्थिर शून्य rb_get_new_extent(संरचना bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
स्थिर शून्य print_वृक्षः(संरचना rb_मूलम् *मूलम्)
{
	संरचना rb_ग्रन्थिः *ग्रन्थिः = रिक्त;
	संरचना bmap_rb_extent *ext;

	fprintf(stderr, "\t\t\t=================================\n");
	ग्रन्थिः = ext2fs_rb_first(मूलम्);
	प्रति (ग्रन्थिः = ext2fs_rb_first(मूलम्); ग्रन्थिः != रिक्त; 
	     ग्रन्थिः = ext2fs_rb_next(ग्रन्थिः)) {
		ext = ग्रन्थिः_to_extent(ग्रन्थिः);
		fprintf(stderr, "\t\t\t--> (%llu -> %llu)\n",
			(unsigned long long) ext->start,
			(unsigned long long) ext->start + ext->गणनम्);
	}
	fprintf(stderr, "\t\t\t=================================\n");
}

स्थिर शून्य check_वृक्षः(संरचना rb_मूलम् *मूलम्, ध्रुव वर्ण *सन्देशः)
{
	संरचना rb_ग्रन्थिः *ग्रन्थिः;
	संरचना bmap_rb_extent *ext, *old = रिक्त;

	प्रति (ग्रन्थिः = ext2fs_rb_first(मूलम्); ग्रन्थिः;
	     ग्रन्थिः = ext2fs_rb_next(ग्रन्थिः)) {
		ext = ग्रन्थिः_to_extent(ग्रन्थिः);
		जइ (ext->गणनम् == 0) {
			fprintf(stderr, "Tree Error: count is zero\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->गणनम्,
				(unsigned long long) ext->गणनम्);
			गच्छ err_out;
		}
		जइ (ext->start + ext->गणनम् < ext->start) {
			fprintf(stderr,
				"Tree Error: start or count is crazy\n");
			fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
				(unsigned long long) ext->start,
				(unsigned long long) ext->start + ext->गणनम्,
				(unsigned long long) ext->गणनम्);
			गच्छ err_out;
		}

		जइ (old) {
			जइ (old->start > ext->start) {
				fprintf(stderr, "Tree Error: start is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->गणनम्,
					(unsigned long long) old->गणनम्);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->गणनम्,
					(unsigned long long) ext->गणनम्);
				गच्छ err_out;
			}
			जइ ((old->start + old->गणनम्) >= ext->start) {
				fprintf(stderr,
					"Tree Error: extent is crazy\n");
				fprintf(stderr, "extent: %llu -> %llu (%llu)\n",
					(unsigned long long) old->start,
					(unsigned long long) old->start + old->गणनम्,
					(unsigned long long) old->गणनम्);
				fprintf(stderr,
					"extent next: %llu -> %llu (%llu)\n",
					(unsigned long long) ext->start,
					(unsigned long long) ext->start + ext->गणनम्,
					(unsigned long long) ext->गणनम्);
				गच्छ err_out;
			}
		}
		old = ext;
	}
	प्रत्यागम;

err_out:
	fprintf(stderr, "%s\n", सन्देशः);
	print_वृक्षः(मूलम्);
	exit(1);
}
#अण्णहा
#define check_वृक्षः(मूलम्, सन्देशः) करेहि {} जाव (0)
#define print_वृक्षः(मूलम्) करेहि {} जाव (0)
#endif

स्थिर शून्य rb_get_new_extent(संरचना bmap_rb_extent **ext, __u64 start,
			     __u64 गणनम्)
{
	संरचना bmap_rb_extent *new_ext;
	पूर्णाङ्क retval;

	retval = ext2fs_get_mem(परिमाण (संरचना bmap_rb_extent),
				&new_ext);
	जइ (retval)
		abort();

	new_ext->start = start;
	new_ext->गणनम् = गणनम्;
	*ext = new_ext;
}

अन्तःस्थ
स्थिर शून्य rb_free_extent(संरचना ext2fs_rb_private *bp,
			   संरचना bmap_rb_extent *ext)
{
	जइ (bp->wcursor == ext)
		bp->wcursor = रिक्त;
	जइ (bp->rcursor == ext)
		bp->rcursor = रिक्त;
	जइ (bp->rcursor_next == ext)
		bp->rcursor_next = रिक्त;
	ext2fs_free_mem(&ext);
}

स्थिर errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(परिमाण (संरचना ext2fs_rb_private), &bp);
	जइ (retval)
		प्रत्यागम retval;

	bp->मूलम् = RB_मूलम्;
	bp->rcursor = रिक्त;
	bp->rcursor_next = रिक्त;
	bp->wcursor = रिक्त;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->गुप्त = (शून्य *) bp;
	प्रत्यागम 0;
}

स्थिर errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	जइ (retval)
		प्रत्यागम retval;

	प्रत्यागम 0;
}

स्थिर शून्य rb_free_वृक्षः(संरचना rb_मूलम् *मूलम्)
{
	संरचना bmap_rb_extent *ext;
	संरचना rb_ग्रन्थिः *ग्रन्थिः, *next;

	प्रति (ग्रन्थिः = ext2fs_rb_first(मूलम्); ग्रन्थिः; ग्रन्थिः = next) {
		next = ext2fs_rb_next(ग्रन्थिः);
		ext = ग्रन्थिः_to_extent(ग्रन्थिः);
		ext2fs_rb_erase(ग्रन्थिः, मूलम्);
		ext2fs_free_mem(&ext);
	}
}

स्थिर शून्य rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	rb_free_वृक्षः(&bp->मूलम्);
	ext2fs_free_mem(&bp);
	bp = 0;
}

स्थिर errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	संरचना ext2fs_rb_private *src_bp, *dest_bp;
	संरचना bmap_rb_extent *src_ext, *dest_ext;
	संरचना rb_ग्रन्थिः *dest_ग्रन्थिः, *src_ग्रन्थिः, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	जइ (retval)
		प्रत्यागम retval;

	src_bp = (संरचना ext2fs_rb_private *) src->गुप्त;
	dest_bp = (संरचना ext2fs_rb_private *) dest->गुप्त;
	src_bp->rcursor = रिक्त;
	dest_bp->rcursor = रिक्त;

	src_ग्रन्थिः = ext2fs_rb_first(&src_bp->मूलम्);
	जाव (src_ग्रन्थिः) {
		src_ext = ग्रन्थिः_to_extent(src_ग्रन्थिः);
		retval = ext2fs_get_mem(परिमाण (संरचना bmap_rb_extent),
					&dest_ext);
		जइ (retval)
			भङ्ग;

		memcpy(dest_ext, src_ext, परिमाण(संरचना bmap_rb_extent));

		dest_ग्रन्थिः = &dest_ext->ग्रन्थिः;
		n = &dest_bp->मूलम्.rb_ग्रन्थिः;

		dest_last = रिक्त;
		जइ (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->मूलम्);
			n = &(dest_last)->rb_दक्षिणः;
		}

		ext2fs_rb_link_ग्रन्थिः(dest_ग्रन्थिः, dest_last, n);
		ext2fs_rb_निवेशय_color(dest_ग्रन्थिः, &dest_bp->मूलम्);

		src_ग्रन्थिः = ext2fs_rb_next(src_ग्रन्थिः);
	}

	प्रत्यागम retval;
}

स्थिर शून्य rb_truncate(__u64 new_max, संरचना rb_मूलम् *मूलम्)
{
	संरचना bmap_rb_extent *ext;
	संरचना rb_ग्रन्थिः *ग्रन्थिः;

	ग्रन्थिः = ext2fs_rb_last(मूलम्);
	जाव (ग्रन्थिः) {
		ext = ग्रन्थिः_to_extent(ग्रन्थिः);

		जइ ((ext->start + ext->गणनम् - 1) <= new_max)
			भङ्ग;
		अण्णहा जइ (ext->start > new_max) {
			ext2fs_rb_erase(ग्रन्थिः, मूलम्);
			ext2fs_free_mem(&ext);
			ग्रन्थिः = ext2fs_rb_last(मूलम्);
			अनुवर्त;
		} अण्णहा
			ext->गणनम् = new_max - ext->start + 1;
	}
}

स्थिर errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bmap->गुप्त;
	bp->rcursor = रिक्त;
	bp->wcursor = रिक्त;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->मूलम्);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	जइ (bmap->end < bmap->real_end)
		rb_निवेशय_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	प्रत्यागम 0;

}

अन्तःस्थ स्थिर पूर्णाङ्क
rb_test_bit(संरचना ext2fs_rb_private *bp, __u64 bit)
{
	संरचना bmap_rb_extent *rcursor, *next_ext = रिक्त;
	संरचना rb_ग्रन्थिः *parent = रिक्त, *next;
	संरचना rb_ग्रन्थिः **n = &bp->मूलम्.rb_ग्रन्थिः;
	संरचना bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	जइ (!rcursor)
		गच्छ अन्वेषण_वृक्षः;

	जइ (bit >= rcursor->start && bit < rcursor->start + rcursor->गणनम्) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		प्रत्यागम 1;
	}

	next_ext = bp->rcursor_next;
	जइ (!next_ext) {
		next = ext2fs_rb_next(&rcursor->ग्रन्थिः);
		जइ (next)
			next_ext = ग्रन्थिः_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	जइ (next_ext) {
		जइ ((bit >= rcursor->start + rcursor->गणनम्) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			प्रत्यागम 0;
		}
	}
	bp->rcursor = रिक्त;
	bp->rcursor_next = रिक्त;

	rcursor = bp->wcursor;
	जइ (!rcursor)
		गच्छ अन्वेषण_वृक्षः;

	जइ (bit >= rcursor->start && bit < rcursor->start + rcursor->गणनम्)
		प्रत्यागम 1;

अन्वेषण_वृक्षः:

	जाव (*n) {
		parent = *n;
		ext = ग्रन्थिः_to_extent(parent);
		जइ (bit < ext->start)
			n = &(*n)->rb_वामः;
		अण्णहा जइ (bit >= (ext->start + ext->गणनम्))
			n = &(*n)->rb_दक्षिणः;
		अण्णहा {
			bp->rcursor = ext;
			bp->rcursor_next = रिक्त;
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क rb_निवेशय_extent(__u64 start, __u64 गणनम्,
			    संरचना ext2fs_rb_private *bp)
{
	संरचना rb_मूलम् *मूलम् = &bp->मूलम्;
	संरचना rb_ग्रन्थिः *parent = रिक्त, **n = &मूलम्->rb_ग्रन्थिः;
	संरचना rb_ग्रन्थिः *new_ग्रन्थिः, *ग्रन्थिः, *next;
	संरचना bmap_rb_extent *new_ext;
	संरचना bmap_rb_extent *ext;
	पूर्णाङ्क retval = 0;

	जइ (गणनम् == 0)
		प्रत्यागम 0;

	bp->rcursor_next = रिक्त;
	ext = bp->wcursor;
	जइ (ext) {
		जइ (start >= ext->start &&
		    start <= (ext->start + ext->गणनम्)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			गच्छ got_extent;
		}
	}

	जाव (*n) {
		parent = *n;
		ext = ग्रन्थिः_to_extent(parent);

		जइ (start < ext->start) {
			n = &(*n)->rb_वामः;
		} अण्णहा जइ (start > (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
		} अण्णहा {
got_extent:
			जइ ((start + गणनम्) <= (ext->start + ext->गणनम्))
				प्रत्यागम 1;

			जइ ((ext->start + ext->गणनम्) == start)
				retval = 0;
			अण्णहा
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

	ग्रन्थिः = ext2fs_rb_prev(new_ग्रन्थिः);
	जइ (ग्रन्थिः) {
		ext = ग्रन्थिः_to_extent(ग्रन्थिः);
		जइ ((ext->start + ext->गणनम्) == start) {
			start = ext->start;
			गणनम् += ext->गणनम्;
			ext2fs_rb_erase(ग्रन्थिः, मूलम्);
			rb_free_extent(bp, ext);
		}
	}

skip_निवेशय:
	/* See if we can merge extent to the right */
	प्रति (ग्रन्थिः = ext2fs_rb_next(new_ग्रन्थिः); ग्रन्थिः != रिक्त; ग्रन्थिः = next) {
		next = ext2fs_rb_next(ग्रन्थिः);
		ext = ग्रन्थिः_to_extent(ग्रन्थिः);

		जइ ((ext->start + ext->गणनम्) <= start)
			अनुवर्त;

		/* No more merging */
		जइ ((start + गणनम्) < ext->start)
			भङ्ग;

		/* ext is embedded in new_ext interval */
		जइ ((start + गणनम्) >= (ext->start + ext->गणनम्)) {
			ext2fs_rb_erase(ग्रन्थिः, मूलम्);
			rb_free_extent(bp, ext);
			अनुवर्त;
		} अण्णहा {
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
			    संरचना ext2fs_rb_private *bp)
{
	संरचना rb_मूलम् *मूलम् = &bp->मूलम्;
	संरचना rb_ग्रन्थिः *parent = रिक्त, **n = &मूलम्->rb_ग्रन्थिः;
	संरचना rb_ग्रन्थिः *ग्रन्थिः;
	संरचना bmap_rb_extent *ext;
	__u64 new_start, new_गणनम्;
	पूर्णाङ्क retval = 0;

	जइ (ext2fs_rb_empty_मूलम्(मूलम्))
		प्रत्यागम 0;

	जाव (*n) {
		parent = *n;
		ext = ग्रन्थिः_to_extent(parent);
		जइ (start < ext->start) {
			n = &(*n)->rb_वामः;
			अनुवर्त;
		} अण्णहा जइ (start >= (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
			अनुवर्त;
		}

		जइ ((start > ext->start) &&
		    (start + गणनम्) < (ext->start + ext->गणनम्)) {
			/* We have to split extent into two */
			new_start = start + गणनम्;
			new_गणनम् = (ext->start + ext->गणनम्) - new_start;

			ext->गणनम् = start - ext->start;

			rb_निवेशय_extent(new_start, new_गणनम्, bp);
			प्रत्यागम 1;
		}

		जइ ((start + गणनम्) >= (ext->start + ext->गणनम्)) {
			ext->गणनम् = start - ext->start;
			retval = 1;
		}

		जइ (0 == ext->गणनम्) {
			parent = ext2fs_rb_next(&ext->ग्रन्थिः);
			ext2fs_rb_erase(&ext->ग्रन्थिः, मूलम्);
			rb_free_extent(bp, ext);
			भङ्ग;
		}

		जइ (start == ext->start) {
			ext->start += गणनम्;
			ext->गणनम् -= गणनम्;
			प्रत्यागम 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	प्रति (; parent != रिक्त; parent = ग्रन्थिः) {
		ग्रन्थिः = ext2fs_rb_next(parent);
		ext = ग्रन्थिः_to_extent(parent);
		जइ ((ext->start + ext->गणनम्) <= start)
			अनुवर्त;

		/* No more extents to be removed/truncated */
		जइ ((start + गणनम्) < ext->start)
			भङ्ग;

		/* The entire extent is within the region to be removed */
		जइ ((start + गणनम्) >= (ext->start + ext->गणनम्)) {
			ext2fs_rb_erase(parent, मूलम्);
			rb_free_extent(bp, ext);
			retval = 1;
			अनुवर्त;
		} अण्णहा {
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
	संरचना ext2fs_rb_private *bp;
	पूर्णाङ्क retval;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	retval = rb_निवेशय_extent(arg, 1, bp);
	check_वृक्षः(&bp->मूलम्, __func__);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_private *bp;
	पूर्णाङ्क retval;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_वृक्षः(&bp->मूलम्, __func__);

	प्रत्यागम retval;
}

अन्तःस्थ
स्थिर पूर्णाङ्क rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	प्रत्यागम rb_test_bit(bp, arg);
}

स्थिर शून्य rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned पूर्णाङ्क अङ्कः)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	rb_निवेशय_extent(arg, अङ्कः, bp);
	check_वृक्षः(&bp->मूलम्, __func__);
}

स्थिर शून्य rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned पूर्णाङ्क अङ्कः)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	rb_remove_extent(arg, अङ्कः, bp);
	check_वृक्षः(&bp->मूलम्, __func__);
}

स्थिर पूर्णाङ्क rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned पूर्णाङ्क len)
{
	संरचना rb_ग्रन्थिः *parent = रिक्त, **n;
	संरचना rb_ग्रन्थिः *ग्रन्थिः, *next;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;
	पूर्णाङ्क retval = 1;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->मूलम्.rb_ग्रन्थिः;
	start -= bitmap->start;

	जइ (len == 0 || ext2fs_rb_empty_मूलम्(&bp->मूलम्))
		प्रत्यागम 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	जाव (*n) {
		parent = *n;
		ext = ग्रन्थिः_to_extent(parent);
		जइ (start < ext->start) {
			n = &(*n)->rb_वामः;
		} अण्णहा जइ (start >= (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
		} अण्णहा {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			प्रत्यागम 0;
		}
	}

	ग्रन्थिः = parent;
	जाव (ग्रन्थिः) {
		next = ext2fs_rb_next(ग्रन्थिः);
		ext = ग्रन्थिः_to_extent(ग्रन्थिः);
		ग्रन्थिः = next;

		जइ ((ext->start + ext->गणनम्) <= start)
			अनुवर्त;

		/* No more merging */
		जइ ((start + len) <= ext->start)
			भङ्ग;

		retval = 0;
		भङ्ग;
	}
	प्रत्यागम retval;
}

स्थिर errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t अङ्कः, शून्य *in)
{
	संरचना ext2fs_rb_private *bp;
	unsigned वर्ण *cp = in;
	size_t i;
	पूर्णाङ्क first_set = -1;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	प्रति (i = 0; i < अङ्कः; i++) {
		जइ ((i & 7) == 0) {
			unsigned वर्ण c = cp[i/8];
			जइ (c == 0xFF) {
				जइ (first_set == -1)
					first_set = i;
				i += 7;
				अनुवर्त;
			}
			जइ ((c == 0x00) && (first_set == -1)) {
				i += 7;
				अनुवर्त;
			}
		}
		जइ (ext2fs_test_bit(i, in)) {
			जइ (first_set == -1)
				first_set = i;
			अनुवर्त;
		}
		जइ (first_set == -1)
			अनुवर्त;

		rb_निवेशय_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_वृक्षः(&bp->मूलम्, __func__);
		first_set = -1;
	}
	जइ (first_set != -1) {
		rb_निवेशय_extent(start + first_set - bitmap->start,
				 अङ्कः - first_set, bp);
		check_वृक्षः(&bp->मूलम्, __func__);
	}

	प्रत्यागम 0;
}

स्थिर errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t अङ्कः, शून्य *out)
{

	संरचना rb_ग्रन्थिः *parent = रिक्त, *next, **n;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;
	__u64 गणनम्, pos;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->मूलम्.rb_ग्रन्थिः;
	start -= bitmap->start;

	जइ (ext2fs_rb_empty_मूलम्(&bp->मूलम्))
		प्रत्यागम 0;

	जाव (*n) {
		parent = *n;
		ext = ग्रन्थिः_to_extent(parent);
		जइ (start < ext->start) {
			n = &(*n)->rb_वामः;
		} अण्णहा जइ (start >= (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
		} अण्णहा
			भङ्ग;
	}

	memset(out, 0, (अङ्कः + 7) >> 3);

	प्रति (; parent != रिक्त; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = ग्रन्थिः_to_extent(parent);

		pos = ext->start;
		गणनम् = ext->गणनम्;
		जइ (pos >= start + अङ्कः)
			भङ्ग;
		जइ (pos < start) {
			जइ (pos + गणनम् <  start)
				अनुवर्त;
			गणनम् -= start - pos;
			pos = start;
		}
		जइ (pos + गणनम् > start + अङ्कः)
			गणनम् = start + अङ्कः - pos;

		जाव (गणनम् > 0) {
			जइ ((गणनम् >= 8) &&
			    ((pos - start) % 8) == 0) {
				पूर्णाङ्क nbytes = गणनम् >> 3;
				पूर्णाङ्क offset = (pos - start) >> 3;

				memset(((वर्ण *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				गणनम् -= nbytes << 3;
				अनुवर्त;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			गणनम्--;
		}
	}
	प्रत्यागम 0;
}

स्थिर शून्य rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	rb_free_वृक्षः(&bp->मूलम्);
	bp->rcursor = रिक्त;
	bp->rcursor_next = रिक्त;
	bp->wcursor = रिक्त;
	check_वृक्षः(&bp->मूलम्, __func__);
}

स्थिर errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	संरचना rb_ग्रन्थिः *parent = रिक्त, **n;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->मूलम्.rb_ग्रन्थिः;
	start -= bitmap->start;
	end -= bitmap->start;

	जइ (start > end)
		प्रत्यागम EINVAL;

	जइ (ext2fs_rb_empty_मूलम्(&bp->मूलम्))
		प्रत्यागम ENOENT;

	जाव (*n) {
		parent = *n;
		ext = ग्रन्थिः_to_extent(parent);
		जइ (start < ext->start) {
			n = &(*n)->rb_वामः;
		} अण्णहा जइ (start >= (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
		} अण्णहा जइ (ext->start + ext->गणनम् <= end) {
			*out = ext->start + ext->गणनम् + bitmap->start;
			प्रत्यागम 0;
		} अण्णहा
			प्रत्यागम ENOENT;
	}

	*out = start + bitmap->start;
	प्रत्यागम 0;
}

स्थिर errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	संरचना rb_ग्रन्थिः *parent = रिक्त, **n;
	संरचना rb_ग्रन्थिः *ग्रन्थिः;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->मूलम्.rb_ग्रन्थिः;
	start -= bitmap->start;
	end -= bitmap->start;

	जइ (start > end)
		प्रत्यागम EINVAL;

	जइ (ext2fs_rb_empty_मूलम्(&bp->मूलम्))
		प्रत्यागम ENOENT;

	जाव (*n) {
		parent = *n;
		ext = ग्रन्थिः_to_extent(parent);
		जइ (start < ext->start) {
			n = &(*n)->rb_वामः;
		} अण्णहा जइ (start >= (ext->start + ext->गणनम्)) {
			n = &(*n)->rb_दक्षिणः;
		} अण्णहा {
			/* The start bit is set */
			*out = start + bitmap->start;
			प्रत्यागम 0;
		}
	}

	ग्रन्थिः = parent;
	ext = ग्रन्थिः_to_extent(ग्रन्थिः);
	जइ (ext->start < start) {
		ग्रन्थिः = ext2fs_rb_next(ग्रन्थिः);
		जइ (ग्रन्थिः == रिक्त)
			प्रत्यागम ENOENT;
		ext = ग्रन्थिः_to_extent(ग्रन्थिः);
	}
	जइ (ext->start <= end) {
		*out = ext->start + bitmap->start;
		प्रत्यागम 0;
	}
	प्रत्यागम ENOENT;
}

#ifdef ENABLE_BMAP_STATS
स्थिर शून्य rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;
	संरचना rb_ग्रन्थिः *ग्रन्थिः = रिक्त;
	संरचना bmap_rb_extent *ext;
	__u64 गणनम् = 0;
	__u64 max_size = 0;
	__u64 min_size = ULONG_MAX;
	__u64 size = 0, avg_size = 0;
	द्विगुण eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	द्विगुण m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	प्रति (ग्रन्थिः = ext2fs_rb_first(&bp->मूलम्); ग्रन्थिः != रिक्त;
	     ग्रन्थिः = ext2fs_rb_next(ग्रन्थिः)) {
		ext = ग्रन्थिः_to_extent(ग्रन्थिः);
		गणनम्++;
		जइ (ext->गणनम् > max_size)
			max_size = ext->गणनम्;
		जइ (ext->गणनम् < min_size)
			min_size = ext->गणनम्;
		size += ext->गणनम्;
	}

	जइ (गणनम्)
		avg_size = size / गणनम्;
	जइ (min_size == ULONG_MAX)
		min_size = 0;
	eff = (द्विगुण)((गणनम् * परिमाण(संरचना bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_गणनम् + bitmap->stats.mark_ext_गणनम्;
	test_all = bitmap->stats.test_गणनम् + bitmap->stats.test_ext_गणनम्;
	जइ (mark_all)
		m_hit = ((द्विगुण)bp->mark_hit / mark_all) * 100;
	जइ (test_all)
		t_hit = ((द्विगुण)bp->test_hit / test_all) * 100;

	fprintf(stderr, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	fprintf(stderr, "%16llu extents (%llu bytes)\n",
		(unsigned long long) गणनम्, (unsigned long long)
		((गणनम् * परिमाण(संरचना bmap_rb_extent)) +
		 परिमाण(संरचना ext2fs_rb_private)));
 	fprintf(stderr, "%16llu bits minimum size\n",
		(unsigned long long) min_size);
	fprintf(stderr, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(unsigned long long) max_size, (unsigned long long) avg_size);
	fprintf(stderr, "%16llu bits set in bitmap (out of %llu)\n",
		(unsigned long long) size,
		(unsigned long long) bitmap->real_end - bitmap->start);
	fprintf(stderr,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#अण्णहा
स्थिर शून्य rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

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
	.set_bmap_range = rb_set_bmap_range,
	.get_bmap_range = rb_get_bmap_range,
	.clear_bmap = rb_clear_bmap,
	.print_stats = rb_print_stats,
	.find_first_zero = rb_find_first_zero,
	.find_first_set = rb_find_first_set,
};
