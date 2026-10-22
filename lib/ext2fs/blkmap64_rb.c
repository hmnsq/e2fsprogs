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
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#यदि HAVE_LINUX_TYPES_H
#अन्तर्भाव <linux/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "bmap64.h"
#अन्तर्भाव "rbtree.h"

#अन्तर्भाव <limits.h>

संरचना bmap_rb_extent {
	संरचना rb_गाँठ गाँठ;
	__u64 start;
	__u64 गिनती;
};

संरचना ext2fs_rb_private {
	संरचना rb_मूल मूल;
	संरचना bmap_rb_extent *wcursor;
	संरचना bmap_rb_extent *rcursor;
	संरचना bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

अन्तःस्थ स्थिर संरचना bmap_rb_extent *गाँठ_to_extent(संरचना rb_गाँठ *गाँठ)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	प्रत्यागम (संरचना bmap_rb_extent *) गाँठ;
}

स्थिर पूर्णाङ्क rb_जोड़ो_extent(__u64 start, __u64 गिनती,
			    संरचना ext2fs_rb_private *);
स्थिर शून्य rb_get_new_extent(संरचना bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
स्थिर शून्य print_वृक्ष(संरचना rb_मूल *मूल)
{
	संरचना rb_गाँठ *गाँठ = रिक्त;
	संरचना bmap_rb_extent *ext;

	ख_लिखो(मानक_त्रुटि, "\t\t\t=================================\n");
	गाँठ = ext2fs_rb_first(मूल);
	प्रति (गाँठ = ext2fs_rb_first(मूल); गाँठ != रिक्त; 
	     गाँठ = ext2fs_rb_next(गाँठ)) {
		ext = गाँठ_to_extent(गाँठ);
		ख_लिखो(मानक_त्रुटि, "\t\t\t--> (%llu -> %llu)\n",
			(अचिन्हित दीर्घ दीर्घ) ext->start,
			(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गिनती);
	}
	ख_लिखो(मानक_त्रुटि, "\t\t\t=================================\n");
}

स्थिर शून्य check_वृक्ष(संरचना rb_मूल *मूल, ध्रुव वर्ण *संदेश)
{
	संरचना rb_गाँठ *गाँठ;
	संरचना bmap_rb_extent *ext, *old = रिक्त;

	प्रति (गाँठ = ext2fs_rb_first(मूल); गाँठ;
	     गाँठ = ext2fs_rb_next(गाँठ)) {
		ext = गाँठ_to_extent(गाँठ);
		यदि (ext->गिनती == 0) {
			ख_लिखो(मानक_त्रुटि, "Tree Error: count is zero\n");
			ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
				(अचिन्हित दीर्घ दीर्घ) ext->start,
				(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गिनती,
				(अचिन्हित दीर्घ दीर्घ) ext->गिनती);
			गच्छ err_out;
		}
		यदि (ext->start + ext->गिनती < ext->start) {
			ख_लिखो(मानक_त्रुटि,
				"Tree Error: start or count is crazy\n");
			ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
				(अचिन्हित दीर्घ दीर्घ) ext->start,
				(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गिनती,
				(अचिन्हित दीर्घ दीर्घ) ext->गिनती);
			गच्छ err_out;
		}

		यदि (old) {
			यदि (old->start > ext->start) {
				ख_लिखो(मानक_त्रुटि, "Tree Error: start is crazy\n");
				ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) old->start,
					(अचिन्हित दीर्घ दीर्घ) old->start + old->गिनती,
					(अचिन्हित दीर्घ दीर्घ) old->गिनती);
				ख_लिखो(मानक_त्रुटि,
					"extent next: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) ext->start,
					(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गिनती,
					(अचिन्हित दीर्घ दीर्घ) ext->गिनती);
				गच्छ err_out;
			}
			यदि ((old->start + old->गिनती) >= ext->start) {
				ख_लिखो(मानक_त्रुटि,
					"Tree Error: extent is crazy\n");
				ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) old->start,
					(अचिन्हित दीर्घ दीर्घ) old->start + old->गिनती,
					(अचिन्हित दीर्घ दीर्घ) old->गिनती);
				ख_लिखो(मानक_त्रुटि,
					"extent next: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) ext->start,
					(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गिनती,
					(अचिन्हित दीर्घ दीर्घ) ext->गिनती);
				गच्छ err_out;
			}
		}
		old = ext;
	}
	प्रत्यागम;

err_out:
	ख_लिखो(मानक_त्रुटि, "%s\n", संदेश);
	print_वृक्ष(मूल);
	निकास(1);
}
#अन्यथा
#define check_वृक्ष(मूल, संदेश) कुरु {} यावत् (0)
#define print_वृक्ष(मूल) कुरु {} यावत् (0)
#endif

स्थिर शून्य rb_get_new_extent(संरचना bmap_rb_extent **ext, __u64 start,
			     __u64 गिनती)
{
	संरचना bmap_rb_extent *new_ext;
	पूर्णाङ्क retval;

	retval = ext2fs_get_mem(परिमाण (संरचना bmap_rb_extent),
				&new_ext);
	यदि (retval)
		पात();

	new_ext->start = start;
	new_ext->गिनती = गिनती;
	*ext = new_ext;
}

अन्तःस्थ
स्थिर शून्य rb_free_extent(संरचना ext2fs_rb_private *bp,
			   संरचना bmap_rb_extent *ext)
{
	यदि (bp->wcursor == ext)
		bp->wcursor = रिक्त;
	यदि (bp->rcursor == ext)
		bp->rcursor = रिक्त;
	यदि (bp->rcursor_next == ext)
		bp->rcursor_next = रिक्त;
	ext2fs_free_mem(&ext);
}

स्थिर errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(परिमाण (संरचना ext2fs_rb_private), &bp);
	यदि (retval)
		प्रत्यागम retval;

	bp->मूल = RB_मूल;
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
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम 0;
}

स्थिर शून्य rb_free_वृक्ष(संरचना rb_मूल *मूल)
{
	संरचना bmap_rb_extent *ext;
	संरचना rb_गाँठ *गाँठ, *next;

	प्रति (गाँठ = ext2fs_rb_first(मूल); गाँठ; गाँठ = next) {
		next = ext2fs_rb_next(गाँठ);
		ext = गाँठ_to_extent(गाँठ);
		ext2fs_rb_erase(गाँठ, मूल);
		ext2fs_free_mem(&ext);
	}
}

स्थिर शून्य rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	rb_free_वृक्ष(&bp->मूल);
	ext2fs_free_mem(&bp);
	bp = 0;
}

स्थिर errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	संरचना ext2fs_rb_private *src_bp, *dest_bp;
	संरचना bmap_rb_extent *src_ext, *dest_ext;
	संरचना rb_गाँठ *dest_गाँठ, *src_गाँठ, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	यदि (retval)
		प्रत्यागम retval;

	src_bp = (संरचना ext2fs_rb_private *) src->गुप्त;
	dest_bp = (संरचना ext2fs_rb_private *) dest->गुप्त;
	src_bp->rcursor = रिक्त;
	dest_bp->rcursor = रिक्त;

	src_गाँठ = ext2fs_rb_first(&src_bp->मूल);
	यावत् (src_गाँठ) {
		src_ext = गाँठ_to_extent(src_गाँठ);
		retval = ext2fs_get_mem(परिमाण (संरचना bmap_rb_extent),
					&dest_ext);
		यदि (retval)
			भङ्ग;

		स_नकल(dest_ext, src_ext, परिमाण(संरचना bmap_rb_extent));

		dest_गाँठ = &dest_ext->गाँठ;
		n = &dest_bp->मूल.rb_गाँठ;

		dest_last = रिक्त;
		यदि (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->मूल);
			n = &(dest_last)->rb_दायाँ;
		}

		ext2fs_rb_link_गाँठ(dest_गाँठ, dest_last, n);
		ext2fs_rb_जोड़ो_color(dest_गाँठ, &dest_bp->मूल);

		src_गाँठ = ext2fs_rb_next(src_गाँठ);
	}

	प्रत्यागम retval;
}

स्थिर शून्य rb_truncate(__u64 new_max, संरचना rb_मूल *मूल)
{
	संरचना bmap_rb_extent *ext;
	संरचना rb_गाँठ *गाँठ;

	गाँठ = ext2fs_rb_last(मूल);
	यावत् (गाँठ) {
		ext = गाँठ_to_extent(गाँठ);

		यदि ((ext->start + ext->गिनती - 1) <= new_max)
			भङ्ग;
		अन्यथा यदि (ext->start > new_max) {
			ext2fs_rb_erase(गाँठ, मूल);
			ext2fs_free_mem(&ext);
			गाँठ = ext2fs_rb_last(मूल);
			अनुवर्त;
		} अन्यथा
			ext->गिनती = new_max - ext->start + 1;
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
		    &bp->मूल);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	यदि (bmap->end < bmap->real_end)
		rb_जोड़ो_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	प्रत्यागम 0;

}

अन्तःस्थ स्थिर पूर्णाङ्क
rb_test_bit(संरचना ext2fs_rb_private *bp, __u64 bit)
{
	संरचना bmap_rb_extent *rcursor, *next_ext = रिक्त;
	संरचना rb_गाँठ *parent = रिक्त, *next;
	संरचना rb_गाँठ **n = &bp->मूल.rb_गाँठ;
	संरचना bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	यदि (!rcursor)
		गच्छ खोज_वृक्ष;

	यदि (bit >= rcursor->start && bit < rcursor->start + rcursor->गिनती) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		प्रत्यागम 1;
	}

	next_ext = bp->rcursor_next;
	यदि (!next_ext) {
		next = ext2fs_rb_next(&rcursor->गाँठ);
		यदि (next)
			next_ext = गाँठ_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	यदि (next_ext) {
		यदि ((bit >= rcursor->start + rcursor->गिनती) &&
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
	यदि (!rcursor)
		गच्छ खोज_वृक्ष;

	यदि (bit >= rcursor->start && bit < rcursor->start + rcursor->गिनती)
		प्रत्यागम 1;

खोज_वृक्ष:

	यावत् (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		यदि (bit < ext->start)
			n = &(*n)->rb_बायाँ;
		अन्यथा यदि (bit >= (ext->start + ext->गिनती))
			n = &(*n)->rb_दायाँ;
		अन्यथा {
			bp->rcursor = ext;
			bp->rcursor_next = रिक्त;
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क rb_जोड़ो_extent(__u64 start, __u64 गिनती,
			    संरचना ext2fs_rb_private *bp)
{
	संरचना rb_मूल *मूल = &bp->मूल;
	संरचना rb_गाँठ *parent = रिक्त, **n = &मूल->rb_गाँठ;
	संरचना rb_गाँठ *new_गाँठ, *गाँठ, *next;
	संरचना bmap_rb_extent *new_ext;
	संरचना bmap_rb_extent *ext;
	पूर्णाङ्क retval = 0;

	यदि (गिनती == 0)
		प्रत्यागम 0;

	bp->rcursor_next = रिक्त;
	ext = bp->wcursor;
	यदि (ext) {
		यदि (start >= ext->start &&
		    start <= (ext->start + ext->गिनती)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			गच्छ got_extent;
		}
	}

	यावत् (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);

		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा यदि (start > (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा {
got_extent:
			यदि ((start + गिनती) <= (ext->start + ext->गिनती))
				प्रत्यागम 1;

			यदि ((ext->start + ext->गिनती) == start)
				retval = 0;
			अन्यथा
				retval = 1;

			गिनती += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_गाँठ = &ext->गाँठ;

			गच्छ skip_जोड़ो;
		}
	}

	rb_get_new_extent(&new_ext, start, गिनती);

	new_गाँठ = &new_ext->गाँठ;
	ext2fs_rb_link_गाँठ(new_गाँठ, parent, n);
	ext2fs_rb_जोड़ो_color(new_गाँठ, मूल);
	bp->wcursor = new_ext;

	गाँठ = ext2fs_rb_prev(new_गाँठ);
	यदि (गाँठ) {
		ext = गाँठ_to_extent(गाँठ);
		यदि ((ext->start + ext->गिनती) == start) {
			start = ext->start;
			गिनती += ext->गिनती;
			ext2fs_rb_erase(गाँठ, मूल);
			rb_free_extent(bp, ext);
		}
	}

skip_जोड़ो:
	/* See if we can merge extent to the right */
	प्रति (गाँठ = ext2fs_rb_next(new_गाँठ); गाँठ != रिक्त; गाँठ = next) {
		next = ext2fs_rb_next(गाँठ);
		ext = गाँठ_to_extent(गाँठ);

		यदि ((ext->start + ext->गिनती) <= start)
			अनुवर्त;

		/* No more merging */
		यदि ((start + गिनती) < ext->start)
			भङ्ग;

		/* ext is embedded in new_ext interval */
		यदि ((start + गिनती) >= (ext->start + ext->गिनती)) {
			ext2fs_rb_erase(गाँठ, मूल);
			rb_free_extent(bp, ext);
			अनुवर्त;
		} अन्यथा {
		/* merge ext with new_ext */
			गिनती += ((ext->start + ext->गिनती) -
				  (start + गिनती));
			ext2fs_rb_erase(गाँठ, मूल);
			rb_free_extent(bp, ext);
			भङ्ग;
		}
	}

	new_ext->start = start;
	new_ext->गिनती = गिनती;

	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क rb_remove_extent(__u64 start, __u64 गिनती,
			    संरचना ext2fs_rb_private *bp)
{
	संरचना rb_मूल *मूल = &bp->मूल;
	संरचना rb_गाँठ *parent = रिक्त, **n = &मूल->rb_गाँठ;
	संरचना rb_गाँठ *गाँठ;
	संरचना bmap_rb_extent *ext;
	__u64 new_start, new_गिनती;
	पूर्णाङ्क retval = 0;

	यदि (ext2fs_rb_empty_मूल(मूल))
		प्रत्यागम 0;

	यावत् (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
			अनुवर्त;
		} अन्यथा यदि (start >= (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
			अनुवर्त;
		}

		यदि ((start > ext->start) &&
		    (start + गिनती) < (ext->start + ext->गिनती)) {
			/* We have to split extent into two */
			new_start = start + गिनती;
			new_गिनती = (ext->start + ext->गिनती) - new_start;

			ext->गिनती = start - ext->start;

			rb_जोड़ो_extent(new_start, new_गिनती, bp);
			प्रत्यागम 1;
		}

		यदि ((start + गिनती) >= (ext->start + ext->गिनती)) {
			ext->गिनती = start - ext->start;
			retval = 1;
		}

		यदि (0 == ext->गिनती) {
			parent = ext2fs_rb_next(&ext->गाँठ);
			ext2fs_rb_erase(&ext->गाँठ, मूल);
			rb_free_extent(bp, ext);
			भङ्ग;
		}

		यदि (start == ext->start) {
			ext->start += गिनती;
			ext->गिनती -= गिनती;
			प्रत्यागम 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	प्रति (; parent != रिक्त; parent = गाँठ) {
		गाँठ = ext2fs_rb_next(parent);
		ext = गाँठ_to_extent(parent);
		यदि ((ext->start + ext->गिनती) <= start)
			अनुवर्त;

		/* No more extents to be removed/truncated */
		यदि ((start + गिनती) < ext->start)
			भङ्ग;

		/* The entire extent is within the region to be removed */
		यदि ((start + गिनती) >= (ext->start + ext->गिनती)) {
			ext2fs_rb_erase(parent, मूल);
			rb_free_extent(bp, ext);
			retval = 1;
			अनुवर्त;
		} अन्यथा {
			/* modify the last extent in region to be removed */
			ext->गिनती -= ((start + गिनती) - ext->start);
			ext->start = start + गिनती;
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

	retval = rb_जोड़ो_extent(arg, 1, bp);
	check_वृक्ष(&bp->मूल, __func__);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_private *bp;
	पूर्णाङ्क retval;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_वृक्ष(&bp->मूल, __func__);

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
				अचिन्हित पूर्णाङ्क अंक)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	rb_जोड़ो_extent(arg, अंक, bp);
	check_वृक्ष(&bp->मूल, __func__);
}

स्थिर शून्य rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  अचिन्हित पूर्णाङ्क अंक)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	rb_remove_extent(arg, अंक, bp);
	check_वृक्ष(&bp->मूल, __func__);
}

स्थिर पूर्णाङ्क rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, अचिन्हित पूर्णाङ्क len)
{
	संरचना rb_गाँठ *parent = रिक्त, **n;
	संरचना rb_गाँठ *गाँठ, *next;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;
	पूर्णाङ्क retval = 1;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->मूल.rb_गाँठ;
	start -= bitmap->start;

	यदि (len == 0 || ext2fs_rb_empty_मूल(&bp->मूल))
		प्रत्यागम 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	यावत् (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा यदि (start >= (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			प्रत्यागम 0;
		}
	}

	गाँठ = parent;
	यावत् (गाँठ) {
		next = ext2fs_rb_next(गाँठ);
		ext = गाँठ_to_extent(गाँठ);
		गाँठ = next;

		यदि ((ext->start + ext->गिनती) <= start)
			अनुवर्त;

		/* No more merging */
		यदि ((start + len) <= ext->start)
			भङ्ग;

		retval = 0;
		भङ्ग;
	}
	प्रत्यागम retval;
}

स्थिर errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अंक, शून्य *in)
{
	संरचना ext2fs_rb_private *bp;
	अचिन्हित वर्ण *cp = in;
	माप_प्रकार i;
	पूर्णाङ्क first_set = -1;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	प्रति (i = 0; i < अंक; i++) {
		यदि ((i & 7) == 0) {
			अचिन्हित वर्ण c = cp[i/8];
			यदि (c == 0xFF) {
				यदि (first_set == -1)
					first_set = i;
				i += 7;
				अनुवर्त;
			}
			यदि ((c == 0x00) && (first_set == -1)) {
				i += 7;
				अनुवर्त;
			}
		}
		यदि (ext2fs_test_bit(i, in)) {
			यदि (first_set == -1)
				first_set = i;
			अनुवर्त;
		}
		यदि (first_set == -1)
			अनुवर्त;

		rb_जोड़ो_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_वृक्ष(&bp->मूल, __func__);
		first_set = -1;
	}
	यदि (first_set != -1) {
		rb_जोड़ो_extent(start + first_set - bitmap->start,
				 अंक - first_set, bp);
		check_वृक्ष(&bp->मूल, __func__);
	}

	प्रत्यागम 0;
}

स्थिर errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अंक, शून्य *out)
{

	संरचना rb_गाँठ *parent = रिक्त, *next, **n;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;
	__u64 गिनती, pos;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->मूल.rb_गाँठ;
	start -= bitmap->start;

	यदि (ext2fs_rb_empty_मूल(&bp->मूल))
		प्रत्यागम 0;

	यावत् (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा यदि (start >= (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा
			भङ्ग;
	}

	स_रखो(out, 0, (अंक + 7) >> 3);

	प्रति (; parent != रिक्त; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = गाँठ_to_extent(parent);

		pos = ext->start;
		गिनती = ext->गिनती;
		यदि (pos >= start + अंक)
			भङ्ग;
		यदि (pos < start) {
			यदि (pos + गिनती <  start)
				अनुवर्त;
			गिनती -= start - pos;
			pos = start;
		}
		यदि (pos + गिनती > start + अंक)
			गिनती = start + अंक - pos;

		यावत् (गिनती > 0) {
			यदि ((गिनती >= 8) &&
			    ((pos - start) % 8) == 0) {
				पूर्णाङ्क nbytes = गिनती >> 3;
				पूर्णाङ्क offset = (pos - start) >> 3;

				स_रखो(((वर्ण *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				गिनती -= nbytes << 3;
				अनुवर्त;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			गिनती--;
		}
	}
	प्रत्यागम 0;
}

स्थिर शून्य rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	rb_free_वृक्ष(&bp->मूल);
	bp->rcursor = रिक्त;
	bp->rcursor_next = रिक्त;
	bp->wcursor = रिक्त;
	check_वृक्ष(&bp->मूल, __func__);
}

स्थिर errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	संरचना rb_गाँठ *parent = रिक्त, **n;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->मूल.rb_गाँठ;
	start -= bitmap->start;
	end -= bitmap->start;

	यदि (start > end)
		प्रत्यागम EINVAL;

	यदि (ext2fs_rb_empty_मूल(&bp->मूल))
		प्रत्यागम ENOENT;

	यावत् (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा यदि (start >= (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा यदि (ext->start + ext->गिनती <= end) {
			*out = ext->start + ext->गिनती + bitmap->start;
			प्रत्यागम 0;
		} अन्यथा
			प्रत्यागम ENOENT;
	}

	*out = start + bitmap->start;
	प्रत्यागम 0;
}

स्थिर errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	संरचना rb_गाँठ *parent = रिक्त, **n;
	संरचना rb_गाँठ *गाँठ;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->मूल.rb_गाँठ;
	start -= bitmap->start;
	end -= bitmap->start;

	यदि (start > end)
		प्रत्यागम EINVAL;

	यदि (ext2fs_rb_empty_मूल(&bp->मूल))
		प्रत्यागम ENOENT;

	यावत् (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा यदि (start >= (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा {
			/* The start bit is set */
			*out = start + bitmap->start;
			प्रत्यागम 0;
		}
	}

	गाँठ = parent;
	ext = गाँठ_to_extent(गाँठ);
	यदि (ext->start < start) {
		गाँठ = ext2fs_rb_next(गाँठ);
		यदि (गाँठ == रिक्त)
			प्रत्यागम ENOENT;
		ext = गाँठ_to_extent(गाँठ);
	}
	यदि (ext->start <= end) {
		*out = ext->start + bitmap->start;
		प्रत्यागम 0;
	}
	प्रत्यागम ENOENT;
}

#ifdef ENABLE_BMAP_STATS
स्थिर शून्य rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;
	संरचना rb_गाँठ *गाँठ = रिक्त;
	संरचना bmap_rb_extent *ext;
	__u64 गिनती = 0;
	__u64 max_size = 0;
	__u64 min_size = अच_दीर्घ_उच्च;
	__u64 size = 0, avg_size = 0;
	द्विगुण eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	द्विगुण m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	प्रति (गाँठ = ext2fs_rb_first(&bp->मूल); गाँठ != रिक्त;
	     गाँठ = ext2fs_rb_next(गाँठ)) {
		ext = गाँठ_to_extent(गाँठ);
		गिनती++;
		यदि (ext->गिनती > max_size)
			max_size = ext->गिनती;
		यदि (ext->गिनती < min_size)
			min_size = ext->गिनती;
		size += ext->गिनती;
	}

	यदि (गिनती)
		avg_size = size / गिनती;
	यदि (min_size == अच_दीर्घ_उच्च)
		min_size = 0;
	eff = (द्विगुण)((गिनती * परिमाण(संरचना bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_गिनती + bitmap->stats.mark_ext_गिनती;
	test_all = bitmap->stats.test_गिनती + bitmap->stats.test_ext_गिनती;
	यदि (mark_all)
		m_hit = ((द्विगुण)bp->mark_hit / mark_all) * 100;
	यदि (test_all)
		t_hit = ((द्विगुण)bp->test_hit / test_all) * 100;

	ख_लिखो(मानक_त्रुटि, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	ख_लिखो(मानक_त्रुटि, "%16llu extents (%llu bytes)\n",
		(अचिन्हित दीर्घ दीर्घ) गिनती, (अचिन्हित दीर्घ दीर्घ)
		((गिनती * परिमाण(संरचना bmap_rb_extent)) +
		 परिमाण(संरचना ext2fs_rb_private)));
 	ख_लिखो(मानक_त्रुटि, "%16llu bits minimum size\n",
		(अचिन्हित दीर्घ दीर्घ) min_size);
	ख_लिखो(मानक_त्रुटि, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(अचिन्हित दीर्घ दीर्घ) max_size, (अचिन्हित दीर्घ दीर्घ) avg_size);
	ख_लिखो(मानक_त्रुटि, "%16llu bits set in bitmap (out of %llu)\n",
		(अचिन्हित दीर्घ दीर्घ) size,
		(अचिन्हित दीर्घ दीर्घ) bitmap->real_end - bitmap->start);
	ख_लिखो(मानक_त्रुटि,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#अन्यथा
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
