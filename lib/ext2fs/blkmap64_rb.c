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
	संरचना rb_गाँठो गाँठो;
	__u64 start;
	__u64 गन्ती;
};

संरचना ext2fs_rb_private {
	संरचना rb_जरा जरा;
	संरचना bmap_rb_extent *wcursor;
	संरचना bmap_rb_extent *rcursor;
	संरचना bmap_rb_extent *rcursor_next;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#endif
};

अन्तःस्थ स्थिर संरचना bmap_rb_extent *गाँठो_to_extent(संरचना rb_गाँठो *गाँठो)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	फर्काऊ (संरचना bmap_rb_extent *) गाँठो;
}

स्थिर पूर्णाङ्क rb_घुसाउ_extent(__u64 start, __u64 गन्ती,
			    संरचना ext2fs_rb_private *);
स्थिर शून्य rb_get_new_extent(संरचना bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#ifdef DEBUG_RB
स्थिर शून्य print_रूख(संरचना rb_जरा *जरा)
{
	संरचना rb_गाँठो *गाँठो = रिक्त;
	संरचना bmap_rb_extent *ext;

	ख_लिखो(मानक_त्रुटि, "\t\t\t=================================\n");
	गाँठो = ext2fs_rb_first(जरा);
	लागि (गाँठो = ext2fs_rb_first(जरा); गाँठो != रिक्त; 
	     गाँठो = ext2fs_rb_next(गाँठो)) {
		ext = गाँठो_to_extent(गाँठो);
		ख_लिखो(मानक_त्रुटि, "\t\t\t--> (%llu -> %llu)\n",
			(अचिन्हित दीर्घ दीर्घ) ext->start,
			(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गन्ती);
	}
	ख_लिखो(मानक_त्रुटि, "\t\t\t=================================\n");
}

स्थिर शून्य check_रूख(संरचना rb_जरा *जरा, अचर वर्ण *सन्देश)
{
	संरचना rb_गाँठो *गाँठो;
	संरचना bmap_rb_extent *ext, *old = रिक्त;

	लागि (गाँठो = ext2fs_rb_first(जरा); गाँठो;
	     गाँठो = ext2fs_rb_next(गाँठो)) {
		ext = गाँठो_to_extent(गाँठो);
		यदि (ext->गन्ती == 0) {
			ख_लिखो(मानक_त्रुटि, "Tree Error: count is zero\n");
			ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
				(अचिन्हित दीर्घ दीर्घ) ext->start,
				(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गन्ती,
				(अचिन्हित दीर्घ दीर्घ) ext->गन्ती);
			गच्छ err_out;
		}
		यदि (ext->start + ext->गन्ती < ext->start) {
			ख_लिखो(मानक_त्रुटि,
				"Tree Error: start or count is crazy\n");
			ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
				(अचिन्हित दीर्घ दीर्घ) ext->start,
				(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गन्ती,
				(अचिन्हित दीर्घ दीर्घ) ext->गन्ती);
			गच्छ err_out;
		}

		यदि (old) {
			यदि (old->start > ext->start) {
				ख_लिखो(मानक_त्रुटि, "Tree Error: start is crazy\n");
				ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) old->start,
					(अचिन्हित दीर्घ दीर्घ) old->start + old->गन्ती,
					(अचिन्हित दीर्घ दीर्घ) old->गन्ती);
				ख_लिखो(मानक_त्रुटि,
					"extent next: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) ext->start,
					(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गन्ती,
					(अचिन्हित दीर्घ दीर्घ) ext->गन्ती);
				गच्छ err_out;
			}
			यदि ((old->start + old->गन्ती) >= ext->start) {
				ख_लिखो(मानक_त्रुटि,
					"Tree Error: extent is crazy\n");
				ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) old->start,
					(अचिन्हित दीर्घ दीर्घ) old->start + old->गन्ती,
					(अचिन्हित दीर्घ दीर्घ) old->गन्ती);
				ख_लिखो(मानक_त्रुटि,
					"extent next: %llu -> %llu (%llu)\n",
					(अचिन्हित दीर्घ दीर्घ) ext->start,
					(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गन्ती,
					(अचिन्हित दीर्घ दीर्घ) ext->गन्ती);
				गच्छ err_out;
			}
		}
		old = ext;
	}
	फर्काऊ;

err_out:
	ख_लिखो(मानक_त्रुटि, "%s\n", सन्देश);
	print_रूख(जरा);
	निकास(1);
}
#अन्यथा
#define check_रूख(जरा, सन्देश) गर {} जबसम्म (0)
#define print_रूख(जरा) गर {} जबसम्म (0)
#endif

स्थिर शून्य rb_get_new_extent(संरचना bmap_rb_extent **ext, __u64 start,
			     __u64 गन्ती)
{
	संरचना bmap_rb_extent *new_ext;
	पूर्णाङ्क retval;

	retval = ext2fs_get_mem(परिमाण (संरचना bmap_rb_extent),
				&new_ext);
	यदि (retval)
		पात();

	new_ext->start = start;
	new_ext->गन्ती = गन्ती;
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
		फर्काऊ retval;

	bp->जरा = RB_जरा;
	bp->rcursor = रिक्त;
	bp->rcursor_next = रिक्त;
	bp->wcursor = रिक्त;

#ifdef ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#endif

	bitmap->गुप्त = (शून्य *) bp;
	फर्काऊ 0;
}

स्थिर errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_data (bitmap);
	यदि (retval)
		फर्काऊ retval;

	फर्काऊ 0;
}

स्थिर शून्य rb_free_रूख(संरचना rb_जरा *जरा)
{
	संरचना bmap_rb_extent *ext;
	संरचना rb_गाँठो *गाँठो, *next;

	लागि (गाँठो = ext2fs_rb_first(जरा); गाँठो; गाँठो = next) {
		next = ext2fs_rb_next(गाँठो);
		ext = गाँठो_to_extent(गाँठो);
		ext2fs_rb_erase(गाँठो, जरा);
		ext2fs_free_mem(&ext);
	}
}

स्थिर शून्य rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	rb_free_रूख(&bp->जरा);
	ext2fs_free_mem(&bp);
	bp = 0;
}

स्थिर errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	संरचना ext2fs_rb_private *src_bp, *dest_bp;
	संरचना bmap_rb_extent *src_ext, *dest_ext;
	संरचना rb_गाँठो *dest_गाँठो, *src_गाँठो, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_data (dest);
	यदि (retval)
		फर्काऊ retval;

	src_bp = (संरचना ext2fs_rb_private *) src->गुप्त;
	dest_bp = (संरचना ext2fs_rb_private *) dest->गुप्त;
	src_bp->rcursor = रिक्त;
	dest_bp->rcursor = रिक्त;

	src_गाँठो = ext2fs_rb_first(&src_bp->जरा);
	जबसम्म (src_गाँठो) {
		src_ext = गाँठो_to_extent(src_गाँठो);
		retval = ext2fs_get_mem(परिमाण (संरचना bmap_rb_extent),
					&dest_ext);
		यदि (retval)
			रोक;

		स_नकल(dest_ext, src_ext, परिमाण(संरचना bmap_rb_extent));

		dest_गाँठो = &dest_ext->गाँठो;
		n = &dest_bp->जरा.rb_गाँठो;

		dest_last = रिक्त;
		यदि (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->जरा);
			n = &(dest_last)->rb_दायाँ;
		}

		ext2fs_rb_link_गाँठो(dest_गाँठो, dest_last, n);
		ext2fs_rb_घुसाउ_color(dest_गाँठो, &dest_bp->जरा);

		src_गाँठो = ext2fs_rb_next(src_गाँठो);
	}

	फर्काऊ retval;
}

स्थिर शून्य rb_truncate(__u64 new_max, संरचना rb_जरा *जरा)
{
	संरचना bmap_rb_extent *ext;
	संरचना rb_गाँठो *गाँठो;

	गाँठो = ext2fs_rb_last(जरा);
	जबसम्म (गाँठो) {
		ext = गाँठो_to_extent(गाँठो);

		यदि ((ext->start + ext->गन्ती - 1) <= new_max)
			रोक;
		अन्यथा यदि (ext->start > new_max) {
			ext2fs_rb_erase(गाँठो, जरा);
			ext2fs_free_mem(&ext);
			गाँठो = ext2fs_rb_last(जरा);
			जारी;
		} अन्यथा
			ext->गन्ती = new_max - ext->start + 1;
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
		    &bp->जरा);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	यदि (bmap->end < bmap->real_end)
		rb_घुसाउ_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	फर्काऊ 0;

}

अन्तःस्थ स्थिर पूर्णाङ्क
rb_test_bit(संरचना ext2fs_rb_private *bp, __u64 bit)
{
	संरचना bmap_rb_extent *rcursor, *next_ext = रिक्त;
	संरचना rb_गाँठो *parent = रिक्त, *next;
	संरचना rb_गाँठो **n = &bp->जरा.rb_गाँठो;
	संरचना bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	यदि (!rcursor)
		गच्छ खोज_रूख;

	यदि (bit >= rcursor->start && bit < rcursor->start + rcursor->गन्ती) {
#ifdef ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#endif
		फर्काऊ 1;
	}

	next_ext = bp->rcursor_next;
	यदि (!next_ext) {
		next = ext2fs_rb_next(&rcursor->गाँठो);
		यदि (next)
			next_ext = गाँठो_to_extent(next);
		bp->rcursor_next = next_ext;
	}
	यदि (next_ext) {
		यदि ((bit >= rcursor->start + rcursor->गन्ती) &&
		    (bit < next_ext->start)) {
#ifdef BMAP_STATS_OPS
			bp->test_hit++;
#endif
			फर्काऊ 0;
		}
	}
	bp->rcursor = रिक्त;
	bp->rcursor_next = रिक्त;

	rcursor = bp->wcursor;
	यदि (!rcursor)
		गच्छ खोज_रूख;

	यदि (bit >= rcursor->start && bit < rcursor->start + rcursor->गन्ती)
		फर्काऊ 1;

खोज_रूख:

	जबसम्म (*n) {
		parent = *n;
		ext = गाँठो_to_extent(parent);
		यदि (bit < ext->start)
			n = &(*n)->rb_बायाँ;
		अन्यथा यदि (bit >= (ext->start + ext->गन्ती))
			n = &(*n)->rb_दायाँ;
		अन्यथा {
			bp->rcursor = ext;
			bp->rcursor_next = रिक्त;
			फर्काऊ 1;
		}
	}
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क rb_घुसाउ_extent(__u64 start, __u64 गन्ती,
			    संरचना ext2fs_rb_private *bp)
{
	संरचना rb_जरा *जरा = &bp->जरा;
	संरचना rb_गाँठो *parent = रिक्त, **n = &जरा->rb_गाँठो;
	संरचना rb_गाँठो *new_गाँठो, *गाँठो, *next;
	संरचना bmap_rb_extent *new_ext;
	संरचना bmap_rb_extent *ext;
	पूर्णाङ्क retval = 0;

	यदि (गन्ती == 0)
		फर्काऊ 0;

	bp->rcursor_next = रिक्त;
	ext = bp->wcursor;
	यदि (ext) {
		यदि (start >= ext->start &&
		    start <= (ext->start + ext->गन्ती)) {
#ifdef ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#endif
			गच्छ got_extent;
		}
	}

	जबसम्म (*n) {
		parent = *n;
		ext = गाँठो_to_extent(parent);

		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा यदि (start > (ext->start + ext->गन्ती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा {
got_extent:
			यदि ((start + गन्ती) <= (ext->start + ext->गन्ती))
				फर्काऊ 1;

			यदि ((ext->start + ext->गन्ती) == start)
				retval = 0;
			अन्यथा
				retval = 1;

			गन्ती += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_गाँठो = &ext->गाँठो;

			गच्छ skip_घुसाउ;
		}
	}

	rb_get_new_extent(&new_ext, start, गन्ती);

	new_गाँठो = &new_ext->गाँठो;
	ext2fs_rb_link_गाँठो(new_गाँठो, parent, n);
	ext2fs_rb_घुसाउ_color(new_गाँठो, जरा);
	bp->wcursor = new_ext;

	गाँठो = ext2fs_rb_prev(new_गाँठो);
	यदि (गाँठो) {
		ext = गाँठो_to_extent(गाँठो);
		यदि ((ext->start + ext->गन्ती) == start) {
			start = ext->start;
			गन्ती += ext->गन्ती;
			ext2fs_rb_erase(गाँठो, जरा);
			rb_free_extent(bp, ext);
		}
	}

skip_घुसाउ:
	/* See if we can merge extent to the right */
	लागि (गाँठो = ext2fs_rb_next(new_गाँठो); गाँठो != रिक्त; गाँठो = next) {
		next = ext2fs_rb_next(गाँठो);
		ext = गाँठो_to_extent(गाँठो);

		यदि ((ext->start + ext->गन्ती) <= start)
			जारी;

		/* No more merging */
		यदि ((start + गन्ती) < ext->start)
			रोक;

		/* ext is embedded in new_ext interval */
		यदि ((start + गन्ती) >= (ext->start + ext->गन्ती)) {
			ext2fs_rb_erase(गाँठो, जरा);
			rb_free_extent(bp, ext);
			जारी;
		} अन्यथा {
		/* merge ext with new_ext */
			गन्ती += ((ext->start + ext->गन्ती) -
				  (start + गन्ती));
			ext2fs_rb_erase(गाँठो, जरा);
			rb_free_extent(bp, ext);
			रोक;
		}
	}

	new_ext->start = start;
	new_ext->गन्ती = गन्ती;

	फर्काऊ retval;
}

स्थिर पूर्णाङ्क rb_remove_extent(__u64 start, __u64 गन्ती,
			    संरचना ext2fs_rb_private *bp)
{
	संरचना rb_जरा *जरा = &bp->जरा;
	संरचना rb_गाँठो *parent = रिक्त, **n = &जरा->rb_गाँठो;
	संरचना rb_गाँठो *गाँठो;
	संरचना bmap_rb_extent *ext;
	__u64 new_start, new_गन्ती;
	पूर्णाङ्क retval = 0;

	यदि (ext2fs_rb_empty_जरा(जरा))
		फर्काऊ 0;

	जबसम्म (*n) {
		parent = *n;
		ext = गाँठो_to_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
			जारी;
		} अन्यथा यदि (start >= (ext->start + ext->गन्ती)) {
			n = &(*n)->rb_दायाँ;
			जारी;
		}

		यदि ((start > ext->start) &&
		    (start + गन्ती) < (ext->start + ext->गन्ती)) {
			/* We have to split extent into two */
			new_start = start + गन्ती;
			new_गन्ती = (ext->start + ext->गन्ती) - new_start;

			ext->गन्ती = start - ext->start;

			rb_घुसाउ_extent(new_start, new_गन्ती, bp);
			फर्काऊ 1;
		}

		यदि ((start + गन्ती) >= (ext->start + ext->गन्ती)) {
			ext->गन्ती = start - ext->start;
			retval = 1;
		}

		यदि (0 == ext->गन्ती) {
			parent = ext2fs_rb_next(&ext->गाँठो);
			ext2fs_rb_erase(&ext->गाँठो, जरा);
			rb_free_extent(bp, ext);
			रोक;
		}

		यदि (start == ext->start) {
			ext->start += गन्ती;
			ext->गन्ती -= गन्ती;
			फर्काऊ 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	लागि (; parent != रिक्त; parent = गाँठो) {
		गाँठो = ext2fs_rb_next(parent);
		ext = गाँठो_to_extent(parent);
		यदि ((ext->start + ext->गन्ती) <= start)
			जारी;

		/* No more extents to be removed/truncated */
		यदि ((start + गन्ती) < ext->start)
			रोक;

		/* The entire extent is within the region to be removed */
		यदि ((start + गन्ती) >= (ext->start + ext->गन्ती)) {
			ext2fs_rb_erase(parent, जरा);
			rb_free_extent(bp, ext);
			retval = 1;
			जारी;
		} अन्यथा {
			/* modify the last extent in region to be removed */
			ext->गन्ती -= ((start + गन्ती) - ext->start);
			ext->start = start + गन्ती;
			retval = 1;
			रोक;
		}
	}

	फर्काऊ retval;
}

स्थिर पूर्णाङ्क rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_private *bp;
	पूर्णाङ्क retval;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	retval = rb_घुसाउ_extent(arg, 1, bp);
	check_रूख(&bp->जरा, __func__);
	फर्काऊ retval;
}

स्थिर पूर्णाङ्क rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_private *bp;
	पूर्णाङ्क retval;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_रूख(&bp->जरा, __func__);

	फर्काऊ retval;
}

अन्तःस्थ
स्थिर पूर्णाङ्क rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	फर्काऊ rb_test_bit(bp, arg);
}

स्थिर शून्य rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				अचिन्हित पूर्णाङ्क अङ्क)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	rb_घुसाउ_extent(arg, अङ्क, bp);
	check_रूख(&bp->जरा, __func__);
}

स्थिर शून्य rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  अचिन्हित पूर्णाङ्क अङ्क)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	arg -= bitmap->start;

	rb_remove_extent(arg, अङ्क, bp);
	check_रूख(&bp->जरा, __func__);
}

स्थिर पूर्णाङ्क rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, अचिन्हित पूर्णाङ्क len)
{
	संरचना rb_गाँठो *parent = रिक्त, **n;
	संरचना rb_गाँठो *गाँठो, *next;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;
	पूर्णाङ्क retval = 1;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->जरा.rb_गाँठो;
	start -= bitmap->start;

	यदि (len == 0 || ext2fs_rb_empty_जरा(&bp->जरा))
		फर्काऊ 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	जबसम्म (*n) {
		parent = *n;
		ext = गाँठो_to_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा यदि (start >= (ext->start + ext->गन्ती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			फर्काऊ 0;
		}
	}

	गाँठो = parent;
	जबसम्म (गाँठो) {
		next = ext2fs_rb_next(गाँठो);
		ext = गाँठो_to_extent(गाँठो);
		गाँठो = next;

		यदि ((ext->start + ext->गन्ती) <= start)
			जारी;

		/* No more merging */
		यदि ((start + len) <= ext->start)
			रोक;

		retval = 0;
		रोक;
	}
	फर्काऊ retval;
}

स्थिर errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अङ्क, शून्य *in)
{
	संरचना ext2fs_rb_private *bp;
	अचिन्हित वर्ण *cp = in;
	माप_प्रकार i;
	पूर्णाङ्क first_set = -1;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	लागि (i = 0; i < अङ्क; i++) {
		यदि ((i & 7) == 0) {
			अचिन्हित वर्ण c = cp[i/8];
			यदि (c == 0xFF) {
				यदि (first_set == -1)
					first_set = i;
				i += 7;
				जारी;
			}
			यदि ((c == 0x00) && (first_set == -1)) {
				i += 7;
				जारी;
			}
		}
		यदि (ext2fs_test_bit(i, in)) {
			यदि (first_set == -1)
				first_set = i;
			जारी;
		}
		यदि (first_set == -1)
			जारी;

		rb_घुसाउ_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_रूख(&bp->जरा, __func__);
		first_set = -1;
	}
	यदि (first_set != -1) {
		rb_घुसाउ_extent(start + first_set - bitmap->start,
				 अङ्क - first_set, bp);
		check_रूख(&bp->जरा, __func__);
	}

	फर्काऊ 0;
}

स्थिर errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अङ्क, शून्य *out)
{

	संरचना rb_गाँठो *parent = रिक्त, *next, **n;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;
	__u64 गन्ती, pos;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->जरा.rb_गाँठो;
	start -= bitmap->start;

	यदि (ext2fs_rb_empty_जरा(&bp->जरा))
		फर्काऊ 0;

	जबसम्म (*n) {
		parent = *n;
		ext = गाँठो_to_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा यदि (start >= (ext->start + ext->गन्ती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा
			रोक;
	}

	स_रखो(out, 0, (अङ्क + 7) >> 3);

	लागि (; parent != रिक्त; parent = next) {
		next = ext2fs_rb_next(parent);
		ext = गाँठो_to_extent(parent);

		pos = ext->start;
		गन्ती = ext->गन्ती;
		यदि (pos >= start + अङ्क)
			रोक;
		यदि (pos < start) {
			यदि (pos + गन्ती <  start)
				जारी;
			गन्ती -= start - pos;
			pos = start;
		}
		यदि (pos + गन्ती > start + अङ्क)
			गन्ती = start + अङ्क - pos;

		जबसम्म (गन्ती > 0) {
			यदि ((गन्ती >= 8) &&
			    ((pos - start) % 8) == 0) {
				पूर्णाङ्क nbytes = गन्ती >> 3;
				पूर्णाङ्क offset = (pos - start) >> 3;

				स_रखो(((वर्ण *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				गन्ती -= nbytes << 3;
				जारी;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			गन्ती--;
		}
	}
	फर्काऊ 0;
}

स्थिर शून्य rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	rb_free_रूख(&bp->जरा);
	bp->rcursor = रिक्त;
	bp->rcursor_next = रिक्त;
	bp->wcursor = रिक्त;
	check_रूख(&bp->जरा, __func__);
}

स्थिर errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	संरचना rb_गाँठो *parent = रिक्त, **n;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->जरा.rb_गाँठो;
	start -= bitmap->start;
	end -= bitmap->start;

	यदि (start > end)
		फर्काऊ EINVAL;

	यदि (ext2fs_rb_empty_जरा(&bp->जरा))
		फर्काऊ ENOENT;

	जबसम्म (*n) {
		parent = *n;
		ext = गाँठो_to_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा यदि (start >= (ext->start + ext->गन्ती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा यदि (ext->start + ext->गन्ती <= end) {
			*out = ext->start + ext->गन्ती + bitmap->start;
			फर्काऊ 0;
		} अन्यथा
			फर्काऊ ENOENT;
	}

	*out = start + bitmap->start;
	फर्काऊ 0;
}

स्थिर errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	संरचना rb_गाँठो *parent = रिक्त, **n;
	संरचना rb_गाँठो *गाँठो;
	संरचना ext2fs_rb_private *bp;
	संरचना bmap_rb_extent *ext;

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;
	n = &bp->जरा.rb_गाँठो;
	start -= bitmap->start;
	end -= bitmap->start;

	यदि (start > end)
		फर्काऊ EINVAL;

	यदि (ext2fs_rb_empty_जरा(&bp->जरा))
		फर्काऊ ENOENT;

	जबसम्म (*n) {
		parent = *n;
		ext = गाँठो_to_extent(parent);
		यदि (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा यदि (start >= (ext->start + ext->गन्ती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा {
			/* The start bit is set */
			*out = start + bitmap->start;
			फर्काऊ 0;
		}
	}

	गाँठो = parent;
	ext = गाँठो_to_extent(गाँठो);
	यदि (ext->start < start) {
		गाँठो = ext2fs_rb_next(गाँठो);
		यदि (गाँठो == रिक्त)
			फर्काऊ ENOENT;
		ext = गाँठो_to_extent(गाँठो);
	}
	यदि (ext->start <= end) {
		*out = ext->start + bitmap->start;
		फर्काऊ 0;
	}
	फर्काऊ ENOENT;
}

#ifdef ENABLE_BMAP_STATS
स्थिर शून्य rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	संरचना ext2fs_rb_private *bp;
	संरचना rb_गाँठो *गाँठो = रिक्त;
	संरचना bmap_rb_extent *ext;
	__u64 गन्ती = 0;
	__u64 max_size = 0;
	__u64 min_size = अच_दीर्घ_उच्च;
	__u64 size = 0, avg_size = 0;
	द्विगुण eff;
#ifdef ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	द्विगुण m_hit = 0.0, t_hit = 0.0;
#endif

	bp = (संरचना ext2fs_rb_private *) bitmap->गुप्त;

	लागि (गाँठो = ext2fs_rb_first(&bp->जरा); गाँठो != रिक्त;
	     गाँठो = ext2fs_rb_next(गाँठो)) {
		ext = गाँठो_to_extent(गाँठो);
		गन्ती++;
		यदि (ext->गन्ती > max_size)
			max_size = ext->गन्ती;
		यदि (ext->गन्ती < min_size)
			min_size = ext->गन्ती;
		size += ext->गन्ती;
	}

	यदि (गन्ती)
		avg_size = size / गन्ती;
	यदि (min_size == अच_दीर्घ_उच्च)
		min_size = 0;
	eff = (द्विगुण)((गन्ती * परिमाण(संरचना bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#ifdef ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_गन्ती + bitmap->stats.mark_ext_गन्ती;
	test_all = bitmap->stats.test_गन्ती + bitmap->stats.test_ext_गन्ती;
	यदि (mark_all)
		m_hit = ((द्विगुण)bp->mark_hit / mark_all) * 100;
	यदि (test_all)
		t_hit = ((द्विगुण)bp->test_hit / test_all) * 100;

	ख_लिखो(मानक_त्रुटि, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#endif
	ख_लिखो(मानक_त्रुटि, "%16llu extents (%llu bytes)\n",
		(अचिन्हित दीर्घ दीर्घ) गन्ती, (अचिन्हित दीर्घ दीर्घ)
		((गन्ती * परिमाण(संरचना bmap_rb_extent)) +
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
