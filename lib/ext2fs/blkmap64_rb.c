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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#जर HAVE_LINUX_TYPES_H
#अन्तर्भाव <linux/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "bmap64.h"
#अन्तर्भाव "rbtree.h"

#अन्तर्भाव <limits.h>

संरचना bmap_rb_extent {
	संरचना rb_गाठ गाठ;
	__u64 start;
	__u64 मोजणी;
};

संरचना ext2fs_rb_private {
	संरचना rb_मूळ मूळ;
	संरचना bmap_rb_extent *wcursor;
	संरचना bmap_rb_extent *rcursor;
	संरचना bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

ओळीत स्थिर संरचना bmap_rb_extent *गाठ_to_extent(संरचना rb_गाठ *गाठ)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	परत (संरचना bmap_rb_extent *) गाठ;
}

स्थिर पूर्णांक rb_घाला_extent(__u64 start, __u64 मोजणी,
			    संरचना ext2fs_rb_private *);
स्थिर शून्य rb_get_new_extent(संरचना bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
स्थिर शून्य print_वृक्ष(संरचना rb_मूळ *मूळ)
{
	संरचना rb_गाठ *गाठ = रिक्त;
	संरचना bmap_rb_extent *ext;

	ख_लिखो(मानक_त्रुटि, "\t\t\t=================================\n");
	गाठ = ext2fs_rb_first(मूळ);
	साठी (गाठ = ext2fs_rb_first(मूळ); गाठ != रिक्त; 
	     गाठ = ext2fs_rb_next(गाठ)) {
		ext = गाठ_to_extent(गाठ);
		ख_लिखो(मानक_त्रुटि, "\t\t\t--> (%llu -> %llu)\n",
			(अचिन्हित दीर्घ दीर्घ) ext->start,
			(अचिन्हित दीर्घ दीर्घ) ext->start + ext->मोजणी);
	}
	ख_लिखो(मानक_त्रुटि, "\t\t\t=================================\n");
}

स्थिर शून्य check_वृक्ष(संरचना rb_मूळ *मूळ, स्थिरांक अक्षर *संदेश)
{
	संरचना rb_गाठ *गाठ;
	संरचना bmap_rb_extent *ext, *old = रिक्त;

	साठी (गाठ = ext2fs_rb_first(मूळ); गाठ;
	     गाठ = ext2fs_rb_next(गाठ)) {
		ext = गाठ_to_extent(गाठ);
		जर (ext->मोजणी == 0) {
			ख_लिखो(मानक_त्रुटि, "Tree Error: count is zero\n");
			ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
				(अचिन्हित दीर्घ दीर्घ) ext->start,
				(अचिन्हित दीर्घ दीर्घ) ext->start + ext->मोजणी,
				(अचिन्हित दीर्घ दीर्घ) ext->मोजणी);
			जा err_out;
		}
		जर (ext->start + ext->मोजणी < ext->start) {
			ख_लिखो(मानक_त्रुटि,
				"Tree Error: start or count is crazy\n");
			ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
				(अचिन्हित दीर्घ दीर्घ) ext->start,
				(अचिन्हित दीर्घ दीर्घ) ext->start + ext->मोजणी,
				(अचिन्हित दीर्घ दीर्घ) ext->मोजणी);
			जा err_out;
		}

		जर (old) {
			जर (old->start > ext->start) {
				ख_लिखो(मानक_त्रुटि, "Tree Error: start is crazy\n");
				ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) old->start,
					(अचिन्हित दीर्घ दीर्घ) old->start + old->मोजणी,
					(अचिन्हित दीर्घ दीर्घ) old->मोजणी);
				ख_लिखो(मानक_त्रुटि,
					"extent next: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) ext->start,
					(अचिन्हित दीर्घ दीर्घ) ext->start + ext->मोजणी,
					(अचिन्हित दीर्घ दीर्घ) ext->मोजणी);
				जा err_out;
			}
			जर ((old->start + old->मोजणी) >= ext->start) {
				ख_लिखो(मानक_त्रुटि,
					"Tree Error: extent is crazy\n");
				ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) old->start,
					(अचिन्हित दीर्घ दीर्घ) old->start + old->मोजणी,
					(अचिन्हित दीर्घ दीर्घ) old->मोजणी);
				ख_लिखो(मानक_त्रुटि,
					"extent next: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) ext->start,
					(अचिन्हित दीर्घ दीर्घ) ext->start + ext->मोजणी,
					(अचिन्हित दीर्घ दीर्घ) ext->मोजणी);
				जा err_out;
			}
		}
		old = ext;
	}
	परत;

err_out:
	ख_लिखो(मानक_त्रुटि, "%s\n", संदेश);
	print_वृक्ष(मूळ);
	निकास(1);
}
#अन्यथा
#define check_वृक्ष(मूळ, संदेश) करा {} पर्यंत (0)
#define print_वृक्ष(मूळ) करा {} पर्यंत (0)
#endif

स्थिर शून्य rb_get_new_extent(संरचना bmap_rb_extent **ext, __u64 start,
			     __u64 मोजणी)
{
	संरचना bmap_rb_extent *new_ext;
	पूर्णांक retval;

	retval = ext2fs_get_mem(आकार (संरचना bmap_rb_extent),
				&new_ext);
	जर (retval)
		पात();

	new_ext->start = start;
	new_ext->मोजणी = मोजणी;
	*ext = new_ext;
}

ओळीत
स्थिर शून्य rb_free_extent(संरचना ext2fs_rb_private *bp,
			   संरचना bmap_rb_extent *ext)
{
	जर (bp->wcursor == ext)
		bp->wcursor = रिक्त;
	जर (bp->rcursor == ext)
		bp->rcursor = रिक्त;
	जर (bp->rcursor_next == ext)
		bp->rcursor_next = रिक्त;
	ext2fs_free_mem(&ext);
}

स्थिर errcode_t rb_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(आकार (संरचना ext2fs_rb_private), &bp);
	जर (retval)
		परत retval;

	bp->मूळ = RB_मूळ;
	bp->rcursor = रिक्त;
	bp->rcursor_next = रिक्त;
	bp->wcursor = रिक्त;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->खाजगी = (शून्य *) bp;
	परत 0;
}

स्थिर errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	जर (retval)
		परत retval;

	परत 0;
}

स्थिर शून्य rb_free_वृक्ष(संरचना rb_मूळ *मूळ)
{
	संरचना bmap_rb_extent *ext;
	संरचना rb_गाठ *गाठ, *next;

	साठी (गाठ = ext2fs_rb_first(मूळ); गाठ; गाठ = next) {
		next = ext2fs_rb_next(गाठ);
		ext = गाठ_to_extent(गाठ);
		ext2fs_rb_erase(गाठ, मूळ);
		ext2fs_free_mem(&ext);
	}
}

स्थिर शून्य rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;

	rb_free_वृक्ष(&bp->मूळ);
	ext2fs_free_mem(&bp);
	bp = 0;
}

स्थिर errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	संरचना ext2fs_rb_private *src_bp, *dest_bp;
	संरचना bmap_rb_extent *src_ext, *dest_ext;
	संरचना rb_गाठ *dest_गाठ, *src_गाठ, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	जर (retval)
		परत retval;

	src_bp = (संरचना ext2fs_rb_private *) src->खाजगी;
	dest_bp = (संरचना ext2fs_rb_private *) dest->खाजगी;
	src_bp->rcursor = रिक्त;
	dest_bp->rcursor = रिक्त;

	src_गाठ = ext2fs_rb_first(&src_bp->मूळ);
	पर्यंत (src_गाठ) {
		src_ext = गाठ_to_extent(src_गाठ);
		retval = ext2fs_get_mem(आकार (संरचना bmap_rb_extent),
					&dest_ext);
		जर (retval)
			थांबा;

		स_नकल(dest_ext, src_ext, आकार(संरचना bmap_rb_extent));

		dest_गाठ = &dest_ext->गाठ;
		n = &dest_bp->मूळ.rb_गाठ;

		dest_last = रिक्त;
		जर (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->मूळ);
			n = &(dest_last)->rb_उजवा;
		}

		ext2fs_rb_link_गाठ(dest_गाठ, dest_last, n);
		ext2fs_rb_घाला_color(dest_गाठ, &dest_bp->मूळ);

		src_गाठ = ext2fs_rb_next(src_गाठ);
	}

	परत retval;
}

स्थिर शून्य rb_truncate(__u64 new_max, संरचना rb_मूळ *मूळ)
{
	संरचना bmap_rb_extent *ext;
	संरचना rb_गाठ *गाठ;

	गाठ = ext2fs_rb_last(मूळ);
	पर्यंत (गाठ) {
		ext = गाठ_to_extent(गाठ);

		जर ((ext->start + ext->मोजणी - 1) <= new_max)
			थांबा;
		अन्यथा जर (ext->start > new_max) {
			ext2fs_rb_erase(गाठ, मूळ);
			ext2fs_free_mem(&ext);
			गाठ = ext2fs_rb_last(मूळ);
			सुरूठेवा;
		} अन्यथा
			ext->मोजणी = new_max - ext->start + 1;
	}
}

स्थिर errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bmap->खाजगी;
	bp->rcursor = रिक्त;
	bp->wcursor = रिक्त;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->मूळ);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	जर (bmap->end < bmap->real_end)
		rb_घाला_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	परत 0;

}

ओळीत स्थिर पूर्णांक
rb_test_bit(संरचना ext2fs_rb_private *bp, __u64 bit)
{
	संरचना bmap_rb_extent *rcursor, *next_ext = रिक्त;
	संरचना rb_गाठ *parent = रिक्त, *next;
	संरचना rb_गाठ **n = &bp->मूळ.rb_गाठ;
	संरचना bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	जर (!rcursor)
		जा शोध_वृक्ष;

	जर (bit >= rcursor->start && bit < rcursor->start + rcursor->मोजणी) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		परत 1;
	}

	next_ext = bp->rcursor_next;
	जर (!next_ext) {
		next = ext2fs_rb_next(&rcursor->गाठ);
		जर (next)
			next_ext = गाठ_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	जर (next_ext) {
		जर ((bit >= rcursor->start + rcursor->मोजणी) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			परत 0;
		}
	}
	bp->rcursor = रिक्त;
	bp->rcursor_next = रिक्त;

	rcursor = bp->wcursor;
	जर (!rcursor)
		जा शोध_वृक्ष;

	जर (bit >= rcursor->start && bit < rcursor->start + rcursor->मोजणी)
		परत 1;

शोध_वृक्ष:

	पर्यंत (*n) {
		parent = *n;
		ext = गाठ_to_extent(parent);
		जर (bit < ext->start)
			n = &(*n)->rb_डावा;
		अन्यथा जर (bit >= (ext->start + ext->मोजणी))
			n = &(*n)->rb_उजवा;
		अन्यथा {
			bp->rcursor = ext;
			bp->rcursor_next = रिक्त;
			परत 1;
		}
	}
	परत 0;
}

स्थिर पूर्णांक rb_घाला_extent(__u64 start, __u64 मोजणी,
			    संरचना ext2fs_rb_private *bp)
{
	संरचना rb_मूळ *मूळ = &bp->मूळ;
	संरचना rb_गाठ *parent = रिक्त, **n = &मूळ->rb_गाठ;
	संरचना rb_गाठ *new_गाठ, *गाठ, *next;
	संरचना bmap_rb_extent *new_ext;
	संरचना bmap_rb_extent *ext;
	पूर्णांक retval = 0;

	जर (मोजणी == 0)
		परत 0;

	bp->rcursor_next = रिक्त;
	ext = bp->wcursor;
	जर (ext) {
		जर (start >= ext->start &&
		    start <= (ext->start + ext->मोजणी)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			जा got_extent;
		}
	}

	पर्यंत (*n) {
		parent = *n;
		ext = गाठ_to_extent(parent);

		जर (start < ext->start) {
			n = &(*n)->rb_डावा;
		} अन्यथा जर (start > (ext->start + ext->मोजणी)) {
			n = &(*n)->rb_उजवा;
		} अन्यथा {
got_extent:
			जर ((start + मोजणी) <= (ext->start + ext->मोजणी))
				परत 1;

			जर ((ext->start + ext->मोजणी) == start)
				retval = 0;
			अन्यथा
				retval = 1;

			मोजणी += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_गाठ = &ext->गाठ;

			जा skip_घाला;
		}
	}

	rb_get_new_extent(&new_ext, start, मोजणी);

	new_गाठ = &new_ext->गाठ;
	ext2fs_rb_link_गाठ(new_गाठ, parent, n);
	ext2fs_rb_घाला_color(new_गाठ, मूळ);
	bp->wcursor = new_ext;

	गाठ = ext2fs_rb_prev(new_गाठ);
	जर (गाठ) {
		ext = गाठ_to_extent(गाठ);
		जर ((ext->start + ext->मोजणी) == start) {
			start = ext->start;
			मोजणी += ext->मोजणी;
			ext2fs_rb_erase(गाठ, मूळ);
			rb_free_extent(bp, ext);
		}
	}

skip_घाला:
	/* See if we can merge extent to the right */
	साठी (गाठ = ext2fs_rb_next(new_गाठ); गाठ != रिक्त; गाठ = next) {
		next = ext2fs_rb_next(गाठ);
		ext = गाठ_to_extent(गाठ);

		जर ((ext->start + ext->मोजणी) <= start)
			सुरूठेवा;

		/* No more merging */
		जर ((start + मोजणी) < ext->start)
			थांबा;

		/* ext is embedded in new_ext interval */
		जर ((start + मोजणी) >= (ext->start + ext->मोजणी)) {
			ext2fs_rb_erase(गाठ, मूळ);
			rb_free_extent(bp, ext);
			सुरूठेवा;
		} अन्यथा {
		/* merge ext with new_ext */
			मोजणी += ((ext->start + ext->मोजणी) -
				  (start + मोजणी));
			ext2fs_rb_erase(गाठ, मूळ);
			rb_free_extent(bp, ext);
			थांबा;
		}
	}

	new_ext->start = start;
	new_ext->मोजणी = मोजणी;

	परत retval;
}

स्थिर पूर्णांक rb_remove_extent(__u64 start, __u64 मोजणी,
			    संरचना ext2fs_rb_private *bp)
{
	संरचना rb_मूळ *मूळ = &bp->मूळ;
	संरचना rb_गाठ *parent = रिक्त, **n = &मूळ->rb_गाठ;
	संरचना rb_गाठ *गाठ;
	संरचना bmap_rb_extent *ext;
	__u64 new_start, new_मोजणी;
	पूर्णांक retval = 0;

	जर (ext2fs_rb_empty_मूळ(मूळ))
		परत 0;

	पर्यंत (*n) {
		parent = *n;
		ext = गाठ_to_extent(parent);
		जर (start < ext->start) {
			n = &(*n)->rb_डावा;
			सुरूठेवा;
		} अन्यथा जर (start >= (ext->start + ext->मोजणी)) {
			n = &(*n)->rb_उजवा;
			सुरूठेवा;
		}

		जर ((start > ext->start) &&
		    (start + मोजणी) < (ext->start + ext->मोजणी)) {
			/* We have to split extent into two */
			new_start = start + मोजणी;
			new_मोजणी = (ext->start + ext->मोजणी) - new_start;

			ext->मोजणी = start - ext->start;

			rb_घाला_extent(new_start, new_मोजणी, bp);
			परत 1;
		}

		जर ((start + मोजणी) >= (ext->start + ext->मोजणी)) {
			ext->मोजणी = start - ext->start;
			retval = 1;
		}

		जर (0 == ext->मोजणी) {
			parent = ext2fs_rb_next(&ext->गाठ);
			ext2fs_rb_erase(&ext->गाठ, मूळ);
			rb_free_extent(bp, ext);
			थांबा;
		}

		जर (start == ext->start) {
			ext->start += मोजणी;
			ext->मोजणी -= मोजणी;
			परत 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	साठी (; parent != रिक्त; parent = गाठ) {
		गाठ = ext2fs_rb_next(parent);
		ext = गाठ_to_extent(parent);
		जर ((ext->start + ext->मोजणी) <= start)
			सुरूठेवा;

		/* No more extents to be removed/truncated */
		जर ((start + मोजणी) < ext->start)
			थांबा;

		/* The entire extent is within the region to be removed */
		जर ((start + मोजणी) >= (ext->start + ext->मोजणी)) {
			ext2fs_rb_erase(parent, मूळ);
			rb_free_extent(bp, ext);
			retval = 1;
			सुरूठेवा;
		} अन्यथा {
			/* modify the last extent in region to be removed */
			ext->मोजणी -= ((start + मोजणी) - ext->start);
			ext->start = start + मोजणी;
			retval = 1;
			थांबा;
		}
	}

	परत retval;
}

स्थिर पूर्णांक rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_private *bp;
	पूर्णांक retval;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;
	arg -= bitmap->start;

	retval = rb_घाला_extent(arg, 1, bp);
	check_वृक्ष(&bp->मूळ, __func__);
	परत retval;
}

स्थिर पूर्णांक rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_private *bp;
	पूर्णांक retval;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_वृक्ष(&bp->मूळ, __func__);

	परत retval;
}

ओळीत
स्थिर पूर्णांक rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;
	arg -= bitmap->start;

	परत rb_test_bit(bp, arg);
}

स्थिर शून्य rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				अचिन्हित पूर्णांक अंक)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;
	arg -= bitmap->start;

	rb_घाला_extent(arg, अंक, bp);
	check_वृक्ष(&bp->मूळ, __func__);
}

स्थिर शून्य rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  अचिन्हित पूर्णांक अंक)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;
	arg -= bitmap->start;

	rb_remove_extent(arg, अंक, bp);
	check_वृक्ष(&bp->मूळ, __func__);
}

स्थिर पूर्णांक rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, अचिन्हित पूर्णांक len)
{
	संरचना rb_गाठ *parent = रिक्त, **n;
	संरचना rb_गाठ *गाठ, *next;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;
	पूर्णांक retval = 1;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;
	n = &bp->मूळ.rb_गाठ;
	start -= bitmap->start;

	जर (len == 0 || ext2fs_rb_empty_मूळ(&bp->मूळ))
		परत 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	पर्यंत (*n) {
		parent = *n;
		ext = गाठ_to_extent(parent);
		जर (start < ext->start) {
			n = &(*n)->rb_डावा;
		} अन्यथा जर (start >= (ext->start + ext->मोजणी)) {
			n = &(*n)->rb_उजवा;
		} अन्यथा {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			परत 0;
		}
	}

	गाठ = parent;
	पर्यंत (गाठ) {
		next = ext2fs_rb_next(गाठ);
		ext = गाठ_to_extent(गाठ);
		गाठ = next;

		जर ((ext->start + ext->मोजणी) <= start)
			सुरूठेवा;

		/* No more merging */
		जर ((start + len) <= ext->start)
			थांबा;

		retval = 0;
		थांबा;
	}
	परत retval;
}

स्थिर errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अंक, शून्य *in)
{
	संरचना ext2fs_rb_private *bp;
	अचिन्हित अक्षर *cp = in;
	माप_प्रकार i;
	पूर्णांक first_set = -1;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;

	साठी (i = 0; i < अंक; i++) {
		जर ((i & 7) == 0) {
			अचिन्हित अक्षर c = cp[i/8];
			जर (c == 0xFF) {
				जर (first_set == -1)
					first_set = i;
				i += 7;
				सुरूठेवा;
			}
			जर ((c == 0x00) && (first_set == -1)) {
				i += 7;
				सुरूठेवा;
			}
		}
		जर (ext2fs_test_bit(i, in)) {
			जर (first_set == -1)
				first_set = i;
			सुरूठेवा;
		}
		जर (first_set == -1)
			सुरूठेवा;

		rb_घाला_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_वृक्ष(&bp->मूळ, __func__);
		first_set = -1;
	}
	जर (first_set != -1) {
		rb_घाला_extent(start + first_set - bitmap->start,
				 अंक - first_set, bp);
		check_वृक्ष(&bp->मूळ, __func__);
	}

	परत 0;
}

स्थिर errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अंक, शून्य *out)
{

	संरचना rb_गाठ *parent = रिक्त, *next, **n;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;
	__u64 मोजणी, pos;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;
	n = &bp->मूळ.rb_गाठ;
	start -= bitmap->start;

	जर (ext2fs_rb_empty_मूळ(&bp->मूळ))
		परत 0;

	पर्यंत (*n) {
		parent = *n;
		ext = गाठ_to_extent(parent);
		जर (start < ext->start) {
			n = &(*n)->rb_डावा;
		} अन्यथा जर (start >= (ext->start + ext->मोजणी)) {
			n = &(*n)->rb_उजवा;
		} अन्यथा
			थांबा;
	}

	स_रखो(out, 0, (अंक + 7) >> 3);

	साठी (; parent != रिक्त; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = गाठ_to_extent(parent);

		pos = ext->start;
		मोजणी = ext->मोजणी;
		जर (pos >= start + अंक)
			थांबा;
		जर (pos < start) {
			जर (pos + मोजणी <  start)
				सुरूठेवा;
			मोजणी -= start - pos;
			pos = start;
		}
		जर (pos + मोजणी > start + अंक)
			मोजणी = start + अंक - pos;

		पर्यंत (मोजणी > 0) {
			जर ((मोजणी >= 8) &&
			    ((pos - start) % 8) == 0) {
				पूर्णांक nbytes = मोजणी >> 3;
				पूर्णांक offset = (pos - start) >> 3;

				स_रखो(((अक्षर *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				मोजणी -= nbytes << 3;
				सुरूठेवा;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			मोजणी--;
		}
	}
	परत 0;
}

स्थिर शून्य rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;

	rb_free_वृक्ष(&bp->मूळ);
	bp->rcursor = रिक्त;
	bp->rcursor_next = रिक्त;
	bp->wcursor = रिक्त;
	check_वृक्ष(&bp->मूळ, __func__);
}

स्थिर errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	संरचना rb_गाठ *parent = रिक्त, **n;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;
	n = &bp->मूळ.rb_गाठ;
	start -= bitmap->start;
	end -= bitmap->start;

	जर (start > end)
		परत EINVAL;

	जर (ext2fs_rb_empty_मूळ(&bp->मूळ))
		परत ENOENT;

	पर्यंत (*n) {
		parent = *n;
		ext = गाठ_to_extent(parent);
		जर (start < ext->start) {
			n = &(*n)->rb_डावा;
		} अन्यथा जर (start >= (ext->start + ext->मोजणी)) {
			n = &(*n)->rb_उजवा;
		} अन्यथा जर (ext->start + ext->मोजणी <= end) {
			*out = ext->start + ext->मोजणी + bitmap->start;
			परत 0;
		} अन्यथा
			परत ENOENT;
	}

	*out = start + bitmap->start;
	परत 0;
}

स्थिर errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	संरचना rb_गाठ *parent = रिक्त, **n;
	संरचना rb_गाठ *गाठ;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;
	n = &bp->मूळ.rb_गाठ;
	start -= bitmap->start;
	end -= bitmap->start;

	जर (start > end)
		परत EINVAL;

	जर (ext2fs_rb_empty_मूळ(&bp->मूळ))
		परत ENOENT;

	पर्यंत (*n) {
		parent = *n;
		ext = गाठ_to_extent(parent);
		जर (start < ext->start) {
			n = &(*n)->rb_डावा;
		} अन्यथा जर (start >= (ext->start + ext->मोजणी)) {
			n = &(*n)->rb_उजवा;
		} अन्यथा {
			/* The start bit is set */
			*out = start + bitmap->start;
			परत 0;
		}
	}

	गाठ = parent;
	ext = गाठ_to_extent(गाठ);
	जर (ext->start < start) {
		गाठ = ext2fs_rb_next(गाठ);
		जर (गाठ == रिक्त)
			परत ENOENT;
		ext = गाठ_to_extent(गाठ);
	}
	जर (ext->start <= end) {
		*out = ext->start + bitmap->start;
		परत 0;
	}
	परत ENOENT;
}

#ifdef ENABLE_BMAP_STATS
स्थिर शून्य rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;
	संरचना rb_गाठ *गाठ = रिक्त;
	संरचना bmap_rb_extent *ext;
	__u64 मोजणी = 0;
	__u64 max_size = 0;
	__u64 min_size = अच_दीर्घ_उच्च;
	__u64 size = 0, avg_size = 0;
	द्विगुण eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	द्विगुण m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (संरचना ext2fs_rb_private *) bitmap->खाजगी;

	साठी (गाठ = ext2fs_rb_first(&bp->मूळ); गाठ != रिक्त;
	     गाठ = ext2fs_rb_next(गाठ)) {
		ext = गाठ_to_extent(गाठ);
		मोजणी++;
		जर (ext->मोजणी > max_size)
			max_size = ext->मोजणी;
		जर (ext->मोजणी < min_size)
			min_size = ext->मोजणी;
		size += ext->मोजणी;
	}

	जर (मोजणी)
		avg_size = size / मोजणी;
	जर (min_size == अच_दीर्घ_उच्च)
		min_size = 0;
	eff = (द्विगुण)((मोजणी * आकार(संरचना bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_मोजणी + bitmap->stats.mark_ext_मोजणी;
	test_all = bitmap->stats.test_मोजणी + bitmap->stats.test_ext_मोजणी;
	जर (mark_all)
		m_hit = ((द्विगुण)bp->mark_hit / mark_all) * 100;
	जर (test_all)
		t_hit = ((द्विगुण)bp->test_hit / test_all) * 100;

	ख_लिखो(मानक_त्रुटि, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	ख_लिखो(मानक_त्रुटि, "%16llu extents (%llu bytes)\n",
		(अचिन्हित दीर्घ दीर्घ) मोजणी, (अचिन्हित दीर्घ दीर्घ)
		((मोजणी * आकार(संरचना bmap_rb_extent)) +
		 आकार(संरचना ext2fs_rb_private)));
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
