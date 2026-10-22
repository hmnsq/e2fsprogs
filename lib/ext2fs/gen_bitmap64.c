/*
 * gen_bitmap64.c --- routines to read, write, and manipulate the new qinode and
 * block bitmaps.
 *
 * Copyright (C) 2007, 2008 Theodore Ts'o.
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
#समावेश <errno.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_SYS_TIME_H
#समावेश <sys/time.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"
#समावेश "bmap64.h"

/*
 * Design of 64-bit bitmaps
 *
 * In order maintain ABI compatibility with programs that don't
 * understand about 64-bit blocks/inodes,
 * ext2fs_allocate_inode_bitmap() and ext2fs_allocate_block_bitmap()
 * will create old-style bitmaps unless the application passes the
 * flag EXT2_FLAG_64BITS to ext2fs_open().  If this flag is
 * passed, then we know the application has been recompiled, so we can
 * use the new-style bitmaps.  If it is not passed, we have to return
 * an error if trying to open a filesystem which needs 64-bit bitmaps.
 *
 * The new bitmaps use a new set of structure magic numbers, so that
 * both the old-style and new-style interfaces can identify which
 * version of the data structure was used.  Both the old-style and
 * new-style interfaces will support either type of bitmap, although
 * of course 64-bit operation will only be possible when both the
 * new-style interface and the new-style bitmap are used.
 *
 * For example, the new bitmap interfaces will check the structure
 * magic numbers and so will be able to detect old-stype bitmap.  If
 * they see an old-style bitmap, they will pass it to the gen_bitmap.c
 * functions for handling.  The same will be true for the old
 * interfaces as well.
 *
 * The new-style interfaces will have several different back-end
 * implementations, so we can support different encodings that are
 * appropriate for different applications.  In general the default
 * should be whatever makes sense, and what the application/library
 * will use.  However, e2fsck may need specialized implementations for
 * its own uses.  For example, when doing parent directory pointer
 * loop detections in pass 3, the bitmap will *always* be sparse, so
 * e2fsck can request an encoding which is optimized for that.
 */

अटल व्योम warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			पूर्णांक code, __u64 arg)
{
#अगर_अघोषित OMIT_COM_ERR
	अगर (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%llu for %s", (अचिन्हित दीर्घ दीर्घ) arg,
			bitmap->description);
	अन्यथा
		com_err(0, bitmap->base_error_code + code, "#%llu",
			(अचिन्हित दीर्घ दीर्घ) arg);
#पूर्ण_अगर
}

#अगर_घोषित ENABLE_BMAP_STATS_OPS
#घोषणा INC_STAT(map, name) map->stats.name
#अन्यथा
#घोषणा INC_STAT(map, name) ;;
#पूर्ण_अगर


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    पूर्णांक type, __u64 start, __u64 end,
				    __u64 real_end,
				    स्थिर अक्षर *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	काष्ठा ext2_bitmap_ops	*ops;
	ext2_ino_t अंक_dirs;
	errcode_t retval;

	अगर (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	चयन (type) {
	हाल EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		अवरोध;
	हाल EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		अवरोध;
	हाल EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_अंक_dirs(fs, &अंक_dirs);
		अगर (retval || अंक_dirs > (fs->super->s_inodes_गिनती / 320))
			ops = &ext2fs_blkmap64_bitarray;
		अन्यथा
			ops = &ext2fs_blkmap64_rbtree;
		अवरोध;
	शेष:
		वापस EINVAL;
	}

	retval = ext2fs_get_memzero(माप(काष्ठा ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	अगर (retval)
		वापस retval;

#अगर_घोषित ENABLE_BMAP_STATS
	अगर (gettimeofday(&bitmap->stats.created,
			 (काष्ठा timezone *) शून्य) == -1) {
		लिखो_त्रुटि("gettimeofday");
		ext2fs_free_mem(&bitmap);
		वापस 1;
	}
	bitmap->stats.type = type;
#पूर्ण_अगर

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_bits = 0;
	चयन (magic) {
	हाल EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		अवरोध;
	हाल EXT2_ET_MAGIC_BLOCK_BITMAP64:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bitmap->cluster_bits = fs->cluster_ratio_bits;
		अवरोध;
	शेष:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	अगर (descr) {
		retval = ext2fs_get_mem(म_माप(descr)+1, &bitmap->description);
		अगर (retval) {
			ext2fs_free_mem(&bitmap);
			वापस retval;
		}
		म_नकल(bitmap->description, descr);
	} अन्यथा
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	अगर (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		वापस retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	वापस 0;
}

#अगर_घोषित ENABLE_BMAP_STATS
अटल व्योम ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	काष्ठा ext2_bmap_statistics *stats = &bitmap->stats;
#अगर_घोषित ENABLE_BMAP_STATS_OPS
	भग्न mark_seq_perc = 0.0, test_seq_perc = 0.0;
	भग्न mark_back_perc = 0.0, test_back_perc = 0.0;
	काष्ठा timeval now;
	द्विगुन inuse;

	अगर (stats->test_गिनती) {
		test_seq_perc = ((भग्न)stats->test_seq /
				 stats->test_गिनती) * 100;
		test_back_perc = ((भग्न)stats->test_back /
				  stats->test_गिनती) * 100;
	}

	अगर (stats->mark_गिनती) {
		mark_seq_perc = ((भग्न)stats->mark_seq /
				 stats->mark_गिनती) * 100;
		mark_back_perc = ((भग्न)stats->mark_back /
				  stats->mark_गिनती) * 100;
	}

	अगर (gettimeofday(&now, (काष्ठा timezone *) शून्य) == -1) {
		लिखो_त्रुटि("gettimeofday");
		वापस;
	}

	inuse = (द्विगुन) now.tv_sec + \
		(((द्विगुन) now.tv_usec) * 0.000001);
	inuse -= (द्विगुन) stats->created.tv_sec + \
		(((द्विगुन) stats->created.tv_usec) * 0.000001);
#पूर्ण_अगर /* ENABLE_BMAP_STATS_OPS */

	ख_लिखो(मानक_त्रुटि, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	ख_लिखो(मानक_त्रुटि, "=================================================\n");
#अगर_घोषित ENABLE_BMAP_STATS_OPS
	ख_लिखो(मानक_त्रुटि, "%16llu bits long\n",
		bitmap->real_end - bitmap->start);
	ख_लिखो(मानक_त्रुटि, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_गिनती, stats->resize_गिनती);
	ख_लिखो(मानक_त्रुटि, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_गिनती, stats->unmark_गिनती);
	ख_लिखो(मानक_त्रुटि, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_गिनती, stats->mark_ext_गिनती);
	ख_लिखो(मानक_त्रुटि, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_गिनती, stats->test_ext_गिनती);
	ख_लिखो(मानक_त्रुटि, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->set_range_गिनती, stats->get_range_गिनती);
	ख_लिखो(मानक_त्रुटि, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_गिनती, stats->test_seq, test_seq_perc);
	ख_लिखो(मानक_त्रुटि, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_back, test_back_perc);
	ख_लिखो(मानक_त्रुटि, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_back, mark_back_perc, inuse);
#पूर्ण_अगर /* ENABLE_BMAP_STATS_OPS */
}
#पूर्ण_अगर

व्योम ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	अगर (!bmap)
		वापस;

	अगर (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		वापस;
	}

	अगर (!EXT2FS_IS_64_BITMAP(bmap))
		वापस;

#अगर_घोषित ENABLE_BMAP_STATS
	अगर (ext2fs_safe_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#पूर्ण_अगर

	bmap->bitmap_ops->free_bmap(bmap);

	अगर (bmap->description) {
		ext2fs_free_mem(&bmap->description);
		bmap->description = 0;
	}
	bmap->magic = 0;
	ext2fs_free_mem(&bmap);
}

errcode_t ext2fs_copy_generic_bmap(ext2fs_generic_bitmap gen_src,
				   ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_64 src = (ext2fs_generic_bitmap_64) gen_src;
	अक्षर *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	अगर (!src)
		वापस EINVAL;

	अगर (EXT2FS_IS_32_BITMAP(src))
		वापस ext2fs_copy_generic_bitmap(gen_src, dest);

	अगर (!EXT2FS_IS_64_BITMAP(src))
		वापस EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(माप(काष्ठा ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	अगर (retval)
		वापस retval;


#अगर_घोषित ENABLE_BMAP_STATS_OPS
	src->stats.copy_गिनती++;
#पूर्ण_अगर
#अगर_घोषित ENABLE_BMAP_STATS
	अगर (gettimeofday(&new_bmap->stats.created,
			 (काष्ठा timezone *) शून्य) == -1) {
		लिखो_त्रुटि("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		वापस 1;
	}
	new_bmap->stats.type = src->stats.type;
#पूर्ण_अगर

	/* Copy all the high-level parts over */
	new_bmap->magic = src->magic;
	new_bmap->fs = src->fs;
	new_bmap->start = src->start;
	new_bmap->end = src->end;
	new_bmap->real_end = src->real_end;
	new_bmap->bitmap_ops = src->bitmap_ops;
	new_bmap->base_error_code = src->base_error_code;
	new_bmap->cluster_bits = src->cluster_bits;

	descr = src->description;
	अगर (descr) {
		retval = ext2fs_get_mem(म_माप(descr)+10, &new_descr);
		अगर (retval) {
			ext2fs_free_mem(&new_bmap);
			वापस retval;
		}
		म_नकल(new_descr, "copy of ");
		म_जोड़ो(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	अगर (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		वापस retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	वापस 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_end,
				     __u64 new_real_end)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	अगर (!bmap)
		वापस EINVAL;

	अगर (EXT2FS_IS_32_BITMAP(bmap))
		वापस ext2fs_resize_generic_bitmap(gen_bmap->magic, new_end,
						    new_real_end, gen_bmap);

	अगर (!EXT2FS_IS_64_BITMAP(bmap))
		वापस EINVAL;

	INC_STAT(bmap, resize_गिनती);

	वापस bmap->bitmap_ops->resize_bmap(bmap, new_end, new_real_end);
}

errcode_t ext2fs_fudge_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 end, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	अगर (!bitmap)
		वापस EINVAL;

	अगर (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		पूर्णांक retval;

		retval = ext2fs_fudge_generic_bitmap_end(gen_bitmap,
							 bitmap->magic,
							 neq, end, &tmp_oend);
		अगर (oend)
			*oend = tmp_oend;
		वापस retval;
	}

	अगर (!EXT2FS_IS_64_BITMAP(bitmap))
		वापस EINVAL;

	अगर (end > bitmap->real_end)
		वापस neq;
	अगर (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	वापस 0;
}

__u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	अगर (!bitmap)
		वापस EINVAL;

	अगर (EXT2FS_IS_32_BITMAP(bitmap))
		वापस ext2fs_get_generic_bitmap_start(gen_bitmap);

	अगर (!EXT2FS_IS_64_BITMAP(bitmap))
		वापस EINVAL;

	वापस bitmap->start;
}

__u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	अगर (!bitmap)
		वापस EINVAL;

	अगर (EXT2FS_IS_32_BITMAP(bitmap))
		वापस ext2fs_get_generic_bitmap_end(gen_bitmap);

	अगर (!EXT2FS_IS_64_BITMAP(bitmap))
		वापस EINVAL;

	वापस bitmap->end;
}

व्योम ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	अगर (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	अन्यथा
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

पूर्णांक ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	अगर (!bitmap)
		वापस 0;

	अगर (EXT2FS_IS_32_BITMAP(bitmap)) {
		अगर (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			वापस 0;
		}
		वापस ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	अगर (!EXT2FS_IS_64_BITMAP(bitmap))
		वापस 0;

	arg >>= bitmap->cluster_bits;

#अगर_घोषित ENABLE_BMAP_STATS_OPS
	अगर (arg == bitmap->stats.last_marked + 1)
		bitmap->stats.mark_seq++;
	अगर (arg < bitmap->stats.last_marked)
		bitmap->stats.mark_back++;
	bitmap->stats.last_marked = arg;
	bitmap->stats.mark_गिनती++;
#पूर्ण_अगर

	अगर ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		वापस 0;
	}

	वापस bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

पूर्णांक ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	अगर (!bitmap)
		वापस 0;

	अगर (EXT2FS_IS_32_BITMAP(bitmap)) {
		अगर (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			वापस 0;
		}
		वापस ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	अगर (!EXT2FS_IS_64_BITMAP(bitmap))
		वापस 0;

	arg >>= bitmap->cluster_bits;

	INC_STAT(bitmap, unmark_गिनती);

	अगर ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		वापस 0;
	}

	वापस bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

पूर्णांक ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	अगर (!bitmap)
		वापस 0;

	अगर (EXT2FS_IS_32_BITMAP(bitmap)) {
		अगर (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			वापस 0;
		}
		वापस ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	अगर (!EXT2FS_IS_64_BITMAP(bitmap))
		वापस 0;

	arg >>= bitmap->cluster_bits;

#अगर_घोषित ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_गिनती++;
	अगर (arg == bitmap->stats.last_tested + 1)
		bitmap->stats.test_seq++;
	अगर (arg < bitmap->stats.last_tested)
		bitmap->stats.test_back++;
	bitmap->stats.last_tested = arg;
#पूर्ण_अगर

	अगर ((arg < bitmap->start) || (arg > bitmap->end)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		वापस 0;
	}

	वापस bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_set_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, अचिन्हित पूर्णांक अंक,
					व्योम *in)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	अगर (!bmap)
		वापस EINVAL;

	अगर (EXT2FS_IS_32_BITMAP(bmap)) {
		अगर ((start+अंक-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			वापस EINVAL;
		}
		वापस ext2fs_set_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, अंक, in);
	}

	अगर (!EXT2FS_IS_64_BITMAP(bmap))
		वापस EINVAL;

	INC_STAT(bmap, set_range_गिनती);

	वापस bmap->bitmap_ops->set_bmap_range(bmap, start, अंक, in);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 start, अचिन्हित पूर्णांक अंक,
					व्योम *out)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	अगर (!bmap)
		वापस EINVAL;

	अगर (EXT2FS_IS_32_BITMAP(bmap)) {
		अगर ((start+अंक-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			वापस EINVAL;
		}
		वापस ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       start, अंक, out);
	}

	अगर (!EXT2FS_IS_64_BITMAP(bmap))
		वापस EINVAL;

	INC_STAT(bmap, get_range_गिनती);

	वापस bmap->bitmap_ops->get_bmap_range(bmap, start, अंक, out);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	अगर (!bm1 || !bm2)
		वापस EINVAL;
	अगर (bm1->magic != bm2->magic)
		वापस EINVAL;

	/* Now we know both bitmaps have the same magic */
	अगर (EXT2FS_IS_32_BITMAP(bm1))
		वापस ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	अगर (!EXT2FS_IS_64_BITMAP(bm1))
		वापस EINVAL;

	अगर ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end))
		वापस neq;

	क्रम (i = bm1->start; i < bm1->end; i++) {
		पूर्णांक ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		अगर (ret1 != ret2) {
			वापस neq;
		}
	}

	वापस 0;
}

व्योम ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	start, अंक;

	अगर (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_set_generic_bitmap_padding(gen_bmap);
		वापस;
	}

	start = bmap->end + 1;
	अंक = bmap->real_end - bmap->end;
	bmap->bitmap_ops->mark_bmap_extent(bmap, start, अंक);
	/* XXX ought to warn on error */
}

पूर्णांक ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				    blk64_t block, अचिन्हित पूर्णांक अंक)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + अंक;

	अगर (!bmap)
		वापस EINVAL;

	अगर (अंक == 1)
		वापस !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, block);

	अगर (EXT2FS_IS_32_BITMAP(bmap)) {
		अगर ((block & ~0xffffffffULL) ||
		    ((block+अंक-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			वापस EINVAL;
		}
		वापस ext2fs_test_block_bitmap_range(
			(ext2fs_generic_bitmap) bmap, block, अंक);
	}

	अगर (!EXT2FS_IS_64_BITMAP(bmap))
		वापस EINVAL;

	INC_STAT(bmap, test_ext_गिनती);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	अंक = end - block;

	अगर ((block < bmap->start) || (block > bmap->end) ||
	    (block+अंक-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST, block,
				   bmap->description);
		वापस EINVAL;
	}

	वापस bmap->bitmap_ops->test_clear_bmap_extent(bmap, block, अंक);
}

व्योम ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				     blk64_t block, अचिन्हित पूर्णांक अंक)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + अंक;

	अगर (!bmap)
		वापस;

	अगर (EXT2FS_IS_32_BITMAP(bmap)) {
		अगर ((block & ~0xffffffffULL) ||
		    ((block+अंक-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			वापस;
		}
		ext2fs_mark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
					       block, अंक);
	}

	अगर (!EXT2FS_IS_64_BITMAP(bmap))
		वापस;

	INC_STAT(bmap, mark_ext_गिनती);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	अंक = end - block;

	अगर ((block < bmap->start) || (block > bmap->end) ||
	    (block+अंक-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bmap->description);
		वापस;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, block, अंक);
}

व्योम ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap gen_bmap,
				       blk64_t block, अचिन्हित पूर्णांक अंक)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	end = block + अंक;

	अगर (!bmap)
		वापस;

	अगर (EXT2FS_IS_32_BITMAP(bmap)) {
		अगर ((block & ~0xffffffffULL) ||
		    ((block+अंक-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			वापस;
		}
		ext2fs_unmark_block_bitmap_range((ext2fs_generic_bitmap) bmap,
						 block, अंक);
	}

	अगर (!EXT2FS_IS_64_BITMAP(bmap))
		वापस;

	INC_STAT(bmap, unmark_ext_गिनती);

	/* convert to clusters if necessary */
	block >>= bmap->cluster_bits;
	end += (1ULL << bmap->cluster_bits) - 1;
	end >>= bmap->cluster_bits;
	अंक = end - block;

	अगर ((block < bmap->start) || (block > bmap->end) ||
	    (block+अंक-1 > bmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bmap->description);
		वापस;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, block, अंक);
}

व्योम ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, स्थिर अक्षर *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#अगर_अघोषित OMIT_COM_ERR
	अगर (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	अन्यथा
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap", func);
#पूर्ण_अगर
}

errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_block_bitmap *bitmap)
{
	ext2fs_generic_bitmap_64 bmap, cmap;
	ext2fs_block_bitmap	gen_bmap = *bitmap, gen_cmap;
	errcode_t		retval;
	blk64_t			i, अग्र, b_end, c_end;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	अगर (fs->cluster_ratio_bits == ext2fs_get_bitmap_granularity(gen_bmap))
		वापस 0;	/* Nothing to do */

	retval = ext2fs_allocate_block_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	अगर (retval)
		वापस retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->start;
	b_end = bmap->end;
	bmap->end = bmap->real_end;
	c_end = cmap->end;
	cmap->end = cmap->real_end;
	जबतक (i < bmap->real_end) {
		retval = ext2fs_find_first_set_block_bitmap2(gen_bmap,
						i, bmap->real_end, &अग्र);
		अगर (retval)
			अवरोध;
		ext2fs_mark_block_bitmap2(gen_cmap, अग्र);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, अग्र) + 1);
	}
	bmap->end = b_end;
	cmap->end = c_end;
	ext2fs_free_block_bitmap(gen_bmap);
	*bitmap = (ext2fs_block_bitmap) cmap;
	वापस 0;
}

errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	अगर (!bitmap)
		वापस EINVAL;

	अगर (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		अगर (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			वापस EINVAL;
		}

		retval = ext2fs_find_first_zero_generic_bitmap(bitmap, start,
							       end, &blk);
		अगर (retval == 0)
			*out = blk;
		वापस retval;
	}

	अगर (!EXT2FS_IS_64_BITMAP(bitmap))
		वापस EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	अगर (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		वापस EINVAL;
	}

	अगर (bmap64->bitmap_ops->find_first_zero) {
		retval = bmap64->bitmap_ops->find_first_zero(bmap64, cstart,
							     cend, &cout);
		अगर (retval)
			वापस retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		वापस 0;
	}

	क्रम (cout = cstart; cout <= cend; cout++)
		अगर (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			जाओ found;

	वापस ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 start, __u64 end, __u64 *out)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	अगर (!bitmap)
		वापस EINVAL;

	अगर (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		अगर (((start) & ~0xffffffffULL) ||
		    ((end) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, start);
			वापस EINVAL;
		}

		retval = ext2fs_find_first_set_generic_bitmap(bitmap, start,
							      end, &blk);
		अगर (retval == 0)
			*out = blk;
		वापस retval;
	}

	अगर (!EXT2FS_IS_64_BITMAP(bitmap))
		वापस EINVAL;

	cstart = start >> bmap64->cluster_bits;
	cend = end >> bmap64->cluster_bits;

	अगर (cstart < bmap64->start || cend > bmap64->end || start > end) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, start);
		वापस EINVAL;
	}

	अगर (bmap64->bitmap_ops->find_first_set) {
		retval = bmap64->bitmap_ops->find_first_set(bmap64, cstart,
							    cend, &cout);
		अगर (retval)
			वापस retval;
	found:
		cout <<= bmap64->cluster_bits;
		*out = (cout >= start) ? cout : start;
		वापस 0;
	}

	क्रम (cout = cstart; cout <= cend; cout++)
		अगर (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			जाओ found;

	वापस ENOENT;
}

errcode_t ext2fs_गिनती_used_blocks(ext2_filsys fs, blk64_t start,
				   blk64_t end, blk64_t *out)
{
	blk64_t		अग्र;
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	जबतक (start < end) {
		retval = ext2fs_find_first_set_block_bitmap2(fs->block_map,
							start, end, &अग्र);
		अगर (retval) {
			अगर (retval == ENOENT)
				retval = 0;
			अवरोध;
		}
		start = अग्र;

		retval = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
							start, end, &अग्र);
		अगर (retval == 0) {
			tot_set += अग्र - start;
			start  = अग्र + 1;
		} अन्यथा अगर (retval == ENOENT) {
			retval = 0;
			tot_set += end - start + 1;
			अवरोध;
		} अन्यथा
			अवरोध;
	}

	अगर (!retval)
		*out = tot_set;
	वापस retval;
}

errcode_t ext2fs_गिनती_used_clusters(ext2_filsys fs, blk64_t start,
				     blk64_t end, blk64_t *out)
{
	blk64_t		tot_set = 0;
	errcode_t	retval = 0;

	retval = ext2fs_गिनती_used_blocks(fs, start, end, &tot_set);

	अगर (!retval)
		*out = EXT2_FS_अंक_B2_C(fs, tot_set);
	वापस retval;
}
