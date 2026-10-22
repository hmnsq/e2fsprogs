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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#अगर HAVE_LINUX_TYPES_H
#समावेश <linux/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"
#समावेश "bmap64.h"
#समावेश "rbtree.h"

#समावेश <limits.h>

काष्ठा bmap_rb_extent {
	काष्ठा rb_गाँठ गाँठ;
	__u64 start;
	__u64 गिनती;
};

काष्ठा ext2fs_rb_private {
	काष्ठा rb_मूल मूल;
	काष्ठा bmap_rb_extent *wcursor;
	काष्ठा bmap_rb_extent *rcursor;
	काष्ठा bmap_rb_extent *rcursor_अग्र;
#अगर_घोषित ENABLE_BMAP_STATS_OPS
	__u64 mark_hit;
	__u64 test_hit;
#पूर्ण_अगर
};

अंतरभूत अटल काष्ठा bmap_rb_extent *गाँठ_to_extent(काष्ठा rb_गाँठ *गाँठ)
{
	/*
	 * This depends on the fact the struct rb_node is at the
	 * beginning of the bmap_rb_extent structure.  We use this
	 * instead of the ext2fs_rb_entry macro because it causes gcc
	 * -Wall to generate a huge amount of noise.
	 */
	वापस (काष्ठा bmap_rb_extent *) गाँठ;
}

अटल पूर्णांक rb_जोड़ो_extent(__u64 start, __u64 गिनती,
			    काष्ठा ext2fs_rb_private *);
अटल व्योम rb_get_new_extent(काष्ठा bmap_rb_extent **, __u64, __u64);

/* #define DEBUG_RB */

#अगर_घोषित DEBUG_RB
अटल व्योम print_वृक्ष(काष्ठा rb_मूल *मूल)
{
	काष्ठा rb_गाँठ *गाँठ = शून्य;
	काष्ठा bmap_rb_extent *ext;

	ख_लिखो(मानक_त्रुटि, "\t\t\t=================================\n");
	गाँठ = ext2fs_rb_first(मूल);
	क्रम (गाँठ = ext2fs_rb_first(मूल); गाँठ != शून्य; 
	     गाँठ = ext2fs_rb_अग्र(गाँठ)) {
		ext = गाँठ_to_extent(गाँठ);
		ख_लिखो(मानक_त्रुटि, "\t\t\t--> (%llu -> %llu)\n",
			(अचिन्हित दीर्घ दीर्घ) ext->start,
			(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गिनती);
	}
	ख_लिखो(मानक_त्रुटि, "\t\t\t=================================\n");
}

अटल व्योम check_वृक्ष(काष्ठा rb_मूल *मूल, स्थिर अक्षर *संदेश)
{
	काष्ठा rb_गाँठ *गाँठ;
	काष्ठा bmap_rb_extent *ext, *old = शून्य;

	क्रम (गाँठ = ext2fs_rb_first(मूल); गाँठ;
	     गाँठ = ext2fs_rb_अग्र(गाँठ)) {
		ext = गाँठ_to_extent(गाँठ);
		अगर (ext->गिनती == 0) {
			ख_लिखो(मानक_त्रुटि, "Tree Error: count is zero\n");
			ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
				(अचिन्हित दीर्घ दीर्घ) ext->start,
				(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गिनती,
				(अचिन्हित दीर्घ दीर्घ) ext->गिनती);
			जाओ err_out;
		}
		अगर (ext->start + ext->गिनती < ext->start) {
			ख_लिखो(मानक_त्रुटि,
				"Tree Error: start or count is crazy\n");
			ख_लिखो(मानक_त्रुटि, "extent: %llu -> %llu (%llu)\n",
				(अचिन्हित दीर्घ दीर्घ) ext->start,
				(अचिन्हित दीर्घ दीर्घ) ext->start + ext->गिनती,
				(अचिन्हित दीर्घ दीर्घ) ext->गिनती);
			जाओ err_out;
		}

		अगर (old) {
			अगर (old->start > ext->start) {
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
				जाओ err_out;
			}
			अगर ((old->start + old->गिनती) >= ext->start) {
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
				जाओ err_out;
			}
		}
		old = ext;
	}
	वापस;

err_out:
	ख_लिखो(मानक_त्रुटि, "%s\n", संदेश);
	print_वृक्ष(मूल);
	निकास(1);
}
#अन्यथा
#घोषणा check_वृक्ष(मूल, संदेश) करो {} जबतक (0)
#घोषणा print_वृक्ष(मूल) करो {} जबतक (0)
#पूर्ण_अगर

अटल व्योम rb_get_new_extent(काष्ठा bmap_rb_extent **ext, __u64 start,
			     __u64 गिनती)
{
	काष्ठा bmap_rb_extent *new_ext;
	पूर्णांक retval;

	retval = ext2fs_get_mem(माप (काष्ठा bmap_rb_extent),
				&new_ext);
	अगर (retval)
		पात();

	new_ext->start = start;
	new_ext->गिनती = गिनती;
	*ext = new_ext;
}

अंतरभूत
अटल व्योम rb_free_extent(काष्ठा ext2fs_rb_private *bp,
			   काष्ठा bmap_rb_extent *ext)
{
	अगर (bp->wcursor == ext)
		bp->wcursor = शून्य;
	अगर (bp->rcursor == ext)
		bp->rcursor = शून्य;
	अगर (bp->rcursor_अग्र == ext)
		bp->rcursor_अग्र = शून्य;
	ext2fs_free_mem(&ext);
}

अटल errcode_t rb_alloc_private_तथ्य (ext2fs_generic_bitmap_64 bitmap)
{
	काष्ठा ext2fs_rb_private *bp;
	errcode_t	retval;

	retval = ext2fs_get_mem(माप (काष्ठा ext2fs_rb_private), &bp);
	अगर (retval)
		वापस retval;

	bp->मूल = RB_मूल;
	bp->rcursor = शून्य;
	bp->rcursor_अग्र = शून्य;
	bp->wcursor = शून्य;

#अगर_घोषित ENABLE_BMAP_STATS_OPS
	bp->test_hit = 0;
	bp->mark_hit = 0;
#पूर्ण_अगर

	bitmap->निजी = (व्योम *) bp;
	वापस 0;
}

अटल errcode_t rb_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	errcode_t	retval;

	retval = rb_alloc_private_तथ्य (bitmap);
	अगर (retval)
		वापस retval;

	वापस 0;
}

अटल व्योम rb_free_वृक्ष(काष्ठा rb_मूल *मूल)
{
	काष्ठा bmap_rb_extent *ext;
	काष्ठा rb_गाँठ *गाँठ, *अग्र;

	क्रम (गाँठ = ext2fs_rb_first(मूल); गाँठ; गाँठ = अग्र) {
		अग्र = ext2fs_rb_अग्र(गाँठ);
		ext = गाँठ_to_extent(गाँठ);
		ext2fs_rb_erase(गाँठ, मूल);
		ext2fs_free_mem(&ext);
	}
}

अटल व्योम rb_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	काष्ठा ext2fs_rb_private *bp;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;

	rb_free_वृक्ष(&bp->मूल);
	ext2fs_free_mem(&bp);
	bp = 0;
}

अटल errcode_t rb_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	काष्ठा ext2fs_rb_private *src_bp, *dest_bp;
	काष्ठा bmap_rb_extent *src_ext, *dest_ext;
	काष्ठा rb_गाँठ *dest_गाँठ, *src_गाँठ, *dest_last, **n;
	errcode_t retval = 0;

	retval = rb_alloc_private_तथ्य (dest);
	अगर (retval)
		वापस retval;

	src_bp = (काष्ठा ext2fs_rb_private *) src->निजी;
	dest_bp = (काष्ठा ext2fs_rb_private *) dest->निजी;
	src_bp->rcursor = शून्य;
	dest_bp->rcursor = शून्य;

	src_गाँठ = ext2fs_rb_first(&src_bp->मूल);
	जबतक (src_गाँठ) {
		src_ext = गाँठ_to_extent(src_गाँठ);
		retval = ext2fs_get_mem(माप (काष्ठा bmap_rb_extent),
					&dest_ext);
		अगर (retval)
			अवरोध;

		स_नकल(dest_ext, src_ext, माप(काष्ठा bmap_rb_extent));

		dest_गाँठ = &dest_ext->गाँठ;
		n = &dest_bp->मूल.rb_गाँठ;

		dest_last = शून्य;
		अगर (*n) {
			dest_last = ext2fs_rb_last(&dest_bp->मूल);
			n = &(dest_last)->rb_दायाँ;
		}

		ext2fs_rb_link_गाँठ(dest_गाँठ, dest_last, n);
		ext2fs_rb_जोड़ो_color(dest_गाँठ, &dest_bp->मूल);

		src_गाँठ = ext2fs_rb_अग्र(src_गाँठ);
	}

	वापस retval;
}

अटल व्योम rb_truncate(__u64 new_max, काष्ठा rb_मूल *मूल)
{
	काष्ठा bmap_rb_extent *ext;
	काष्ठा rb_गाँठ *गाँठ;

	गाँठ = ext2fs_rb_last(मूल);
	जबतक (गाँठ) {
		ext = गाँठ_to_extent(गाँठ);

		अगर ((ext->start + ext->गिनती - 1) <= new_max)
			अवरोध;
		अन्यथा अगर (ext->start > new_max) {
			ext2fs_rb_erase(गाँठ, मूल);
			ext2fs_free_mem(&ext);
			गाँठ = ext2fs_rb_last(मूल);
			जारी;
		} अन्यथा
			ext->गिनती = new_max - ext->start + 1;
	}
}

अटल errcode_t rb_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	काष्ठा ext2fs_rb_private *bp;

	bp = (काष्ठा ext2fs_rb_private *) bmap->निजी;
	bp->rcursor = शून्य;
	bp->wcursor = शून्य;

	rb_truncate(((new_end < bmap->end) ? new_end : bmap->end) - bmap->start,
		    &bp->मूल);

	bmap->end = new_end;
	bmap->real_end = new_real_end;

	अगर (bmap->end < bmap->real_end)
		rb_जोड़ो_extent(bmap->end + 1 - bmap->start,
				 bmap->real_end - bmap->end, bp);
	वापस 0;

}

अंतरभूत अटल पूर्णांक
rb_test_bit(काष्ठा ext2fs_rb_private *bp, __u64 bit)
{
	काष्ठा bmap_rb_extent *rcursor, *अग्र_ext = शून्य;
	काष्ठा rb_गाँठ *parent = शून्य, *अग्र;
	काष्ठा rb_गाँठ **n = &bp->मूल.rb_गाँठ;
	काष्ठा bmap_rb_extent *ext;

	rcursor = bp->rcursor;
	अगर (!rcursor)
		जाओ खोज_वृक्ष;

	अगर (bit >= rcursor->start && bit < rcursor->start + rcursor->गिनती) {
#अगर_घोषित ENABLE_BMAP_STATS_OPS
		bp->test_hit++;
#पूर्ण_अगर
		वापस 1;
	}

	अग्र_ext = bp->rcursor_अग्र;
	अगर (!अग्र_ext) {
		अग्र = ext2fs_rb_अग्र(&rcursor->गाँठ);
		अगर (अग्र)
			अग्र_ext = गाँठ_to_extent(अग्र);
		bp->rcursor_अग्र = अग्र_ext;
	}
	अगर (अग्र_ext) {
		अगर ((bit >= rcursor->start + rcursor->गिनती) &&
		    (bit < अग्र_ext->start)) {
#अगर_घोषित BMAP_STATS_OPS
			bp->test_hit++;
#पूर्ण_अगर
			वापस 0;
		}
	}
	bp->rcursor = शून्य;
	bp->rcursor_अग्र = शून्य;

	rcursor = bp->wcursor;
	अगर (!rcursor)
		जाओ खोज_वृक्ष;

	अगर (bit >= rcursor->start && bit < rcursor->start + rcursor->गिनती)
		वापस 1;

खोज_वृक्ष:

	जबतक (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		अगर (bit < ext->start)
			n = &(*n)->rb_बायाँ;
		अन्यथा अगर (bit >= (ext->start + ext->गिनती))
			n = &(*n)->rb_दायाँ;
		अन्यथा {
			bp->rcursor = ext;
			bp->rcursor_अग्र = शून्य;
			वापस 1;
		}
	}
	वापस 0;
}

अटल पूर्णांक rb_जोड़ो_extent(__u64 start, __u64 गिनती,
			    काष्ठा ext2fs_rb_private *bp)
{
	काष्ठा rb_मूल *मूल = &bp->मूल;
	काष्ठा rb_गाँठ *parent = शून्य, **n = &मूल->rb_गाँठ;
	काष्ठा rb_गाँठ *new_गाँठ, *गाँठ, *अग्र;
	काष्ठा bmap_rb_extent *new_ext;
	काष्ठा bmap_rb_extent *ext;
	पूर्णांक retval = 0;

	अगर (गिनती == 0)
		वापस 0;

	bp->rcursor_अग्र = शून्य;
	ext = bp->wcursor;
	अगर (ext) {
		अगर (start >= ext->start &&
		    start <= (ext->start + ext->गिनती)) {
#अगर_घोषित ENABLE_BMAP_STATS_OPS
			bp->mark_hit++;
#पूर्ण_अगर
			जाओ got_extent;
		}
	}

	जबतक (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);

		अगर (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा अगर (start > (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा {
got_extent:
			अगर ((start + गिनती) <= (ext->start + ext->गिनती))
				वापस 1;

			अगर ((ext->start + ext->गिनती) == start)
				retval = 0;
			अन्यथा
				retval = 1;

			गिनती += (start - ext->start);
			start = ext->start;
			new_ext = ext;
			new_गाँठ = &ext->गाँठ;

			जाओ skip_जोड़ो;
		}
	}

	rb_get_new_extent(&new_ext, start, गिनती);

	new_गाँठ = &new_ext->गाँठ;
	ext2fs_rb_link_गाँठ(new_गाँठ, parent, n);
	ext2fs_rb_जोड़ो_color(new_गाँठ, मूल);
	bp->wcursor = new_ext;

	गाँठ = ext2fs_rb_पूर्व(new_गाँठ);
	अगर (गाँठ) {
		ext = गाँठ_to_extent(गाँठ);
		अगर ((ext->start + ext->गिनती) == start) {
			start = ext->start;
			गिनती += ext->गिनती;
			ext2fs_rb_erase(गाँठ, मूल);
			rb_free_extent(bp, ext);
		}
	}

skip_जोड़ो:
	/* See if we can merge extent to the right */
	क्रम (गाँठ = ext2fs_rb_अग्र(new_गाँठ); गाँठ != शून्य; गाँठ = अग्र) {
		अग्र = ext2fs_rb_अग्र(गाँठ);
		ext = गाँठ_to_extent(गाँठ);

		अगर ((ext->start + ext->गिनती) <= start)
			जारी;

		/* No more merging */
		अगर ((start + गिनती) < ext->start)
			अवरोध;

		/* ext is embedded in new_ext interval */
		अगर ((start + गिनती) >= (ext->start + ext->गिनती)) {
			ext2fs_rb_erase(गाँठ, मूल);
			rb_free_extent(bp, ext);
			जारी;
		} अन्यथा {
		/* merge ext with new_ext */
			गिनती += ((ext->start + ext->गिनती) -
				  (start + गिनती));
			ext2fs_rb_erase(गाँठ, मूल);
			rb_free_extent(bp, ext);
			अवरोध;
		}
	}

	new_ext->start = start;
	new_ext->गिनती = गिनती;

	वापस retval;
}

अटल पूर्णांक rb_remove_extent(__u64 start, __u64 गिनती,
			    काष्ठा ext2fs_rb_private *bp)
{
	काष्ठा rb_मूल *मूल = &bp->मूल;
	काष्ठा rb_गाँठ *parent = शून्य, **n = &मूल->rb_गाँठ;
	काष्ठा rb_गाँठ *गाँठ;
	काष्ठा bmap_rb_extent *ext;
	__u64 new_start, new_गिनती;
	पूर्णांक retval = 0;

	अगर (ext2fs_rb_empty_मूल(मूल))
		वापस 0;

	जबतक (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		अगर (start < ext->start) {
			n = &(*n)->rb_बायाँ;
			जारी;
		} अन्यथा अगर (start >= (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
			जारी;
		}

		अगर ((start > ext->start) &&
		    (start + गिनती) < (ext->start + ext->गिनती)) {
			/* We have to split extent into two */
			new_start = start + गिनती;
			new_गिनती = (ext->start + ext->गिनती) - new_start;

			ext->गिनती = start - ext->start;

			rb_जोड़ो_extent(new_start, new_गिनती, bp);
			वापस 1;
		}

		अगर ((start + गिनती) >= (ext->start + ext->गिनती)) {
			ext->गिनती = start - ext->start;
			retval = 1;
		}

		अगर (0 == ext->गिनती) {
			parent = ext2fs_rb_अग्र(&ext->गाँठ);
			ext2fs_rb_erase(&ext->गाँठ, मूल);
			rb_free_extent(bp, ext);
			अवरोध;
		}

		अगर (start == ext->start) {
			ext->start += गिनती;
			ext->गिनती -= गिनती;
			वापस 1;
		}
	}

	/* See if we should delete or truncate extent on the right */
	क्रम (; parent != शून्य; parent = गाँठ) {
		गाँठ = ext2fs_rb_अग्र(parent);
		ext = गाँठ_to_extent(parent);
		अगर ((ext->start + ext->गिनती) <= start)
			जारी;

		/* No more extents to be removed/truncated */
		अगर ((start + गिनती) < ext->start)
			अवरोध;

		/* The entire extent is within the region to be removed */
		अगर ((start + गिनती) >= (ext->start + ext->गिनती)) {
			ext2fs_rb_erase(parent, मूल);
			rb_free_extent(bp, ext);
			retval = 1;
			जारी;
		} अन्यथा {
			/* modify the last extent in region to be removed */
			ext->गिनती -= ((start + गिनती) - ext->start);
			ext->start = start + गिनती;
			retval = 1;
			अवरोध;
		}
	}

	वापस retval;
}

अटल पूर्णांक rb_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	काष्ठा ext2fs_rb_private *bp;
	पूर्णांक retval;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;
	arg -= bitmap->start;

	retval = rb_जोड़ो_extent(arg, 1, bp);
	check_वृक्ष(&bp->मूल, __func__);
	वापस retval;
}

अटल पूर्णांक rb_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	काष्ठा ext2fs_rb_private *bp;
	पूर्णांक retval;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;
	arg -= bitmap->start;

	retval = rb_remove_extent(arg, 1, bp);
	check_वृक्ष(&bp->मूल, __func__);

	वापस retval;
}

अंतरभूत
अटल पूर्णांक rb_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	काष्ठा ext2fs_rb_private *bp;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;
	arg -= bitmap->start;

	वापस rb_test_bit(bp, arg);
}

अटल व्योम rb_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				अचिन्हित पूर्णांक अंक)
{
	काष्ठा ext2fs_rb_private *bp;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;
	arg -= bitmap->start;

	rb_जोड़ो_extent(arg, अंक, bp);
	check_वृक्ष(&bp->मूल, __func__);
}

अटल व्योम rb_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  अचिन्हित पूर्णांक अंक)
{
	काष्ठा ext2fs_rb_private *bp;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;
	arg -= bitmap->start;

	rb_remove_extent(arg, अंक, bp);
	check_वृक्ष(&bp->मूल, __func__);
}

अटल पूर्णांक rb_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, अचिन्हित पूर्णांक len)
{
	काष्ठा rb_गाँठ *parent = शून्य, **n;
	काष्ठा rb_गाँठ *गाँठ, *अग्र;
	काष्ठा ext2fs_rb_private *bp;
	काष्ठा bmap_rb_extent *ext;
	पूर्णांक retval = 1;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;
	n = &bp->मूल.rb_गाँठ;
	start -= bitmap->start;

	अगर (len == 0 || ext2fs_rb_empty_मूल(&bp->मूल))
		वापस 1;

	/*
	 * If we find nothing, we should examine whole extent, but
	 * when we find match, the extent is not clean, thus be return
	 * false.
	 */
	जबतक (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		अगर (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा अगर (start >= (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा {
			/*
			 * We found extent int the tree -> extent is not
			 * clean
			 */
			वापस 0;
		}
	}

	गाँठ = parent;
	जबतक (गाँठ) {
		अग्र = ext2fs_rb_अग्र(गाँठ);
		ext = गाँठ_to_extent(गाँठ);
		गाँठ = अग्र;

		अगर ((ext->start + ext->गिनती) <= start)
			जारी;

		/* No more merging */
		अगर ((start + len) <= ext->start)
			अवरोध;

		retval = 0;
		अवरोध;
	}
	वापस retval;
}

अटल errcode_t rb_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अंक, व्योम *in)
{
	काष्ठा ext2fs_rb_private *bp;
	अचिन्हित अक्षर *cp = in;
	माप_प्रकार i;
	पूर्णांक first_set = -1;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;

	क्रम (i = 0; i < अंक; i++) {
		अगर ((i & 7) == 0) {
			अचिन्हित अक्षर c = cp[i/8];
			अगर (c == 0xFF) {
				अगर (first_set == -1)
					first_set = i;
				i += 7;
				जारी;
			}
			अगर ((c == 0x00) && (first_set == -1)) {
				i += 7;
				जारी;
			}
		}
		अगर (ext2fs_test_bit(i, in)) {
			अगर (first_set == -1)
				first_set = i;
			जारी;
		}
		अगर (first_set == -1)
			जारी;

		rb_जोड़ो_extent(start + first_set - bitmap->start,
				 i - first_set, bp);
		check_वृक्ष(&bp->मूल, __func__);
		first_set = -1;
	}
	अगर (first_set != -1) {
		rb_जोड़ो_extent(start + first_set - bitmap->start,
				 अंक - first_set, bp);
		check_वृक्ष(&bp->मूल, __func__);
	}

	वापस 0;
}

अटल errcode_t rb_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अंक, व्योम *out)
{

	काष्ठा rb_गाँठ *parent = शून्य, *अग्र, **n;
	काष्ठा ext2fs_rb_private *bp;
	काष्ठा bmap_rb_extent *ext;
	__u64 गिनती, pos;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;
	n = &bp->मूल.rb_गाँठ;
	start -= bitmap->start;

	अगर (ext2fs_rb_empty_मूल(&bp->मूल))
		वापस 0;

	जबतक (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		अगर (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा अगर (start >= (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा
			अवरोध;
	}

	स_रखो(out, 0, (अंक + 7) >> 3);

	क्रम (; parent != शून्य; parent = अग्र) {
		अग्र = ext2fs_rb_अग्र(parent);
		ext = गाँठ_to_extent(parent);

		pos = ext->start;
		गिनती = ext->गिनती;
		अगर (pos >= start + अंक)
			अवरोध;
		अगर (pos < start) {
			अगर (pos + गिनती <  start)
				जारी;
			गिनती -= start - pos;
			pos = start;
		}
		अगर (pos + गिनती > start + अंक)
			गिनती = start + अंक - pos;

		जबतक (गिनती > 0) {
			अगर ((गिनती >= 8) &&
			    ((pos - start) % 8) == 0) {
				पूर्णांक nbytes = गिनती >> 3;
				पूर्णांक offset = (pos - start) >> 3;

				स_रखो(((अक्षर *) out) + offset, 0xFF, nbytes);
				pos += nbytes << 3;
				गिनती -= nbytes << 3;
				जारी;
			}
			ext2fs_fast_set_bit64((pos - start), out);
			pos++;
			गिनती--;
		}
	}
	वापस 0;
}

अटल व्योम rb_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	काष्ठा ext2fs_rb_private *bp;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;

	rb_free_वृक्ष(&bp->मूल);
	bp->rcursor = शून्य;
	bp->rcursor_अग्र = शून्य;
	bp->wcursor = शून्य;
	check_वृक्ष(&bp->मूल, __func__);
}

अटल errcode_t rb_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	काष्ठा rb_गाँठ *parent = शून्य, **n;
	काष्ठा ext2fs_rb_private *bp;
	काष्ठा bmap_rb_extent *ext;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;
	n = &bp->मूल.rb_गाँठ;
	start -= bitmap->start;
	end -= bitmap->start;

	अगर (start > end)
		वापस EINVAL;

	अगर (ext2fs_rb_empty_मूल(&bp->मूल))
		वापस ENOENT;

	जबतक (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		अगर (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा अगर (start >= (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा अगर (ext->start + ext->गिनती <= end) {
			*out = ext->start + ext->गिनती + bitmap->start;
			वापस 0;
		} अन्यथा
			वापस ENOENT;
	}

	*out = start + bitmap->start;
	वापस 0;
}

अटल errcode_t rb_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				   __u64 start, __u64 end, __u64 *out)
{
	काष्ठा rb_गाँठ *parent = शून्य, **n;
	काष्ठा rb_गाँठ *गाँठ;
	काष्ठा ext2fs_rb_private *bp;
	काष्ठा bmap_rb_extent *ext;

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;
	n = &bp->मूल.rb_गाँठ;
	start -= bitmap->start;
	end -= bitmap->start;

	अगर (start > end)
		वापस EINVAL;

	अगर (ext2fs_rb_empty_मूल(&bp->मूल))
		वापस ENOENT;

	जबतक (*n) {
		parent = *n;
		ext = गाँठ_to_extent(parent);
		अगर (start < ext->start) {
			n = &(*n)->rb_बायाँ;
		} अन्यथा अगर (start >= (ext->start + ext->गिनती)) {
			n = &(*n)->rb_दायाँ;
		} अन्यथा {
			/* The start bit is set */
			*out = start + bitmap->start;
			वापस 0;
		}
	}

	गाँठ = parent;
	ext = गाँठ_to_extent(गाँठ);
	अगर (ext->start < start) {
		गाँठ = ext2fs_rb_अग्र(गाँठ);
		अगर (गाँठ == शून्य)
			वापस ENOENT;
		ext = गाँठ_to_extent(गाँठ);
	}
	अगर (ext->start <= end) {
		*out = ext->start + bitmap->start;
		वापस 0;
	}
	वापस ENOENT;
}

#अगर_घोषित ENABLE_BMAP_STATS
अटल व्योम rb_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	काष्ठा ext2fs_rb_private *bp;
	काष्ठा rb_गाँठ *गाँठ = शून्य;
	काष्ठा bmap_rb_extent *ext;
	__u64 गिनती = 0;
	__u64 max_आकार = 0;
	__u64 min_आकार = अच_दीर्घ_उच्च;
	__u64 आकार = 0, avg_आकार = 0;
	द्विगुन eff;
#अगर_घोषित ENABLE_BMAP_STATS_OPS
	__u64 mark_all, test_all;
	द्विगुन m_hit = 0.0, t_hit = 0.0;
#पूर्ण_अगर

	bp = (काष्ठा ext2fs_rb_private *) bitmap->निजी;

	क्रम (गाँठ = ext2fs_rb_first(&bp->मूल); गाँठ != शून्य;
	     गाँठ = ext2fs_rb_अग्र(गाँठ)) {
		ext = गाँठ_to_extent(गाँठ);
		गिनती++;
		अगर (ext->गिनती > max_आकार)
			max_आकार = ext->गिनती;
		अगर (ext->गिनती < min_आकार)
			min_आकार = ext->गिनती;
		आकार += ext->गिनती;
	}

	अगर (गिनती)
		avg_आकार = आकार / गिनती;
	अगर (min_आकार == अच_दीर्घ_उच्च)
		min_आकार = 0;
	eff = (द्विगुन)((गिनती * माप(काष्ठा bmap_rb_extent)) << 3) /
	      (bitmap->real_end - bitmap->start);
#अगर_घोषित ENABLE_BMAP_STATS_OPS
	mark_all = bitmap->stats.mark_गिनती + bitmap->stats.mark_ext_गिनती;
	test_all = bitmap->stats.test_गिनती + bitmap->stats.test_ext_गिनती;
	अगर (mark_all)
		m_hit = ((द्विगुन)bp->mark_hit / mark_all) * 100;
	अगर (test_all)
		t_hit = ((द्विगुन)bp->test_hit / test_all) * 100;

	ख_लिखो(मानक_त्रुटि, "%16llu cache hits on test (%.2f%%)\n"
		"%16llu cache hits on mark (%.2f%%)\n",
		bp->test_hit, t_hit, bp->mark_hit, m_hit);
#पूर्ण_अगर
	ख_लिखो(मानक_त्रुटि, "%16llu extents (%llu bytes)\n",
		(अचिन्हित दीर्घ दीर्घ) गिनती, (अचिन्हित दीर्घ दीर्घ)
		((गिनती * माप(काष्ठा bmap_rb_extent)) +
		 माप(काष्ठा ext2fs_rb_private)));
 	ख_लिखो(मानक_त्रुटि, "%16llu bits minimum size\n",
		(अचिन्हित दीर्घ दीर्घ) min_आकार);
	ख_लिखो(मानक_त्रुटि, "%16llu bits maximum size\n"
		"%16llu bits average size\n",
		(अचिन्हित दीर्घ दीर्घ) max_आकार, (अचिन्हित दीर्घ दीर्घ) avg_आकार);
	ख_लिखो(मानक_त्रुटि, "%16llu bits set in bitmap (out of %llu)\n",
		(अचिन्हित दीर्घ दीर्घ) आकार,
		(अचिन्हित दीर्घ दीर्घ) bitmap->real_end - bitmap->start);
	ख_लिखो(मानक_त्रुटि,
		"%16.4lf memory / bitmap bit memory ratio (bitarray = 1)\n",
		eff);
}
#अन्यथा
अटल व्योम rb_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#पूर्ण_अगर

काष्ठा ext2_bitmap_ops ext2fs_blkmap64_rbtree = {
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
