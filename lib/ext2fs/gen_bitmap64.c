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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_ਸਮਾਂ_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bmap64.h"

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

ਸਥਿਰ ਖਾਲੀ warn_bitmap(ext2fs_generic_bitmap_64 bitmap,
			ਪੂਰਨ ਕੋਡ, __u64 arg)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
	ਜੇ (bitmap->description)
		com_err(0, bitmap->ਅਧਾਰ_error_ਕੋਡ+ਕੋਡ,
			"#%llu for %s", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) arg,
			bitmap->description);
	ਨਹੀਂਤਾਂ
		com_err(0, bitmap->ਅਧਾਰ_error_ਕੋਡ + ਕੋਡ, "#%llu",
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) arg);
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INC_STAT(map, name) map->stats.name
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INC_STAT(map, name) ;;
#ਜੇ_ਅੰਤ


errcode_t ext2fs_alloc_generic_bmap(ext2_filsys fs, errcode_t magic,
				    ਪੂਰਨ type, __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ,
				    __u64 real_ਸਿਰਾ,
				    ਅਚਲ ਅੱਖਰ *descr,
				    ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_64 bitmap;
	ਬਣਤਰ ext2_bitmap_ops	*ops;
	ext2_ino_t ਅੰਕ_dirs;
	errcode_t retval;

	ਜੇ (!type)
		type = EXT2FS_BMAP64_BITARRAY;

	ਚੋਣ (type) {
	ਹਾਲਤ EXT2FS_BMAP64_BITARRAY:
		ops = &ext2fs_blkmap64_bitarray;
		ਰੋਕੋ;
	ਹਾਲਤ EXT2FS_BMAP64_RBTREE:
		ops = &ext2fs_blkmap64_rbtree;
		ਰੋਕੋ;
	ਹਾਲਤ EXT2FS_BMAP64_AUTODIR:
		retval = ext2fs_get_ਅੰਕ_dirs(fs, &ਅੰਕ_dirs);
		ਜੇ (retval || ਅੰਕ_dirs > (fs->super->s_inodes_ਗਿਣਤੀ / 320))
			ops = &ext2fs_blkmap64_bitarray;
		ਨਹੀਂਤਾਂ
			ops = &ext2fs_blkmap64_rbtree;
		ਰੋਕੋ;
	ਮੂਲ:
		ਵਾਪਸ EINVAL;
	}

	retval = ext2fs_get_memzero(ਆਕਾਰ(ਬਣਤਰ ext2fs_struct_generic_bitmap_64),
				    &bitmap);
	ਜੇ (retval)
		ਵਾਪਸ retval;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS
	ਜੇ (gettimeofday(&bitmap->stats.created,
			 (ਬਣਤਰ timezone *) ਨਿਰਾ) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&bitmap);
		ਵਾਪਸ 1;
	}
	bitmap->stats.type = type;
#ਜੇ_ਅੰਤ

	/* XXX factor out, repeated in copy_bmap */
	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->ਸ਼ੁਰੂ = ਸ਼ੁਰੂ;
	bitmap->ਸਿਰਾ = ਸਿਰਾ;
	bitmap->real_ਸਿਰਾ = real_ਸਿਰਾ;
	bitmap->bitmap_ops = ops;
	bitmap->cluster_ਬਿੱਟ = 0;
	ਚੋਣ (magic) {
	ਹਾਲਤ EXT2_ET_MAGIC_INODE_BITMAP64:
		bitmap->ਅਧਾਰ_error_ਕੋਡ = EXT2_ET_BAD_INODE_MARK;
		ਰੋਕੋ;
	ਹਾਲਤ EXT2_ET_MAGIC_ਖੰਡ_BITMAP64:
		bitmap->ਅਧਾਰ_error_ਕੋਡ = EXT2_ET_BAD_ਖੰਡ_MARK;
		bitmap->cluster_ਬਿੱਟ = fs->cluster_ਅਨੁਪਾਤ_ਬਿੱਟ;
		ਰੋਕੋ;
	ਮੂਲ:
		bitmap->ਅਧਾਰ_error_ਕੋਡ = EXT2_ET_BAD_GENERIC_MARK;
	}
	ਜੇ (descr) {
		retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(descr)+1, &bitmap->description);
		ਜੇ (retval) {
			ext2fs_free_mem(&bitmap);
			ਵਾਪਸ retval;
		}
		strcpy(bitmap->description, descr);
	} ਨਹੀਂਤਾਂ
		bitmap->description = 0;

	retval = bitmap->bitmap_ops->new_bmap(fs, bitmap);
	ਜੇ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ਵਾਪਸ retval;
	}

	*ret = (ext2fs_generic_bitmap) bitmap;
	ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS
ਸਥਿਰ ਖਾਲੀ ext2fs_print_bmap_statistics(ext2fs_generic_bitmap_64 bitmap)
{
	ਬਣਤਰ ext2_bmap_statistics *stats = &bitmap->stats;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
	ਦਸ਼ਮਲਵ mark_seq_perc = 0.0, test_seq_perc = 0.0;
	ਦਸ਼ਮਲਵ mark_ਪਿੱਠ_perc = 0.0, test_ਪਿੱਠ_perc = 0.0;
	ਬਣਤਰ timeval now;
	ਦੁੱਗਣਾ inuse;

	ਜੇ (stats->test_ਗਿਣਤੀ) {
		test_seq_perc = ((ਦਸ਼ਮਲਵ)stats->test_seq /
				 stats->test_ਗਿਣਤੀ) * 100;
		test_ਪਿੱਠ_perc = ((ਦਸ਼ਮਲਵ)stats->test_ਪਿੱਠ /
				  stats->test_ਗਿਣਤੀ) * 100;
	}

	ਜੇ (stats->mark_ਗਿਣਤੀ) {
		mark_seq_perc = ((ਦਸ਼ਮਲਵ)stats->mark_seq /
				 stats->mark_ਗਿਣਤੀ) * 100;
		mark_ਪਿੱਠ_perc = ((ਦਸ਼ਮਲਵ)stats->mark_ਪਿੱਠ /
				  stats->mark_ਗਿਣਤੀ) * 100;
	}

	ਜੇ (gettimeofday(&now, (ਬਣਤਰ timezone *) ਨਿਰਾ) == -1) {
		perror("gettimeofday");
		ਵਾਪਸ;
	}

	inuse = (ਦੁੱਗਣਾ) now.tv_sec + \
		(((ਦੁੱਗਣਾ) now.tv_usec) * 0.000001);
	inuse -= (ਦੁੱਗਣਾ) stats->created.tv_sec + \
		(((ਦੁੱਗਣਾ) stats->created.tv_usec) * 0.000001);
#ਜੇ_ਅੰਤ /* ENABLE_BMAP_STATS_OPS */

	fprintf(stderr, "\n[+] %s bitmap (type %d)\n", bitmap->description,
		stats->type);
	fprintf(stderr, "=================================================\n");
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
	fprintf(stderr, "%16llu bits long\n",
		bitmap->real_ਸਿਰਾ - bitmap->ਸ਼ੁਰੂ);
	fprintf(stderr, "%16lu copy_bmap\n%16lu resize_bmap\n",
		stats->copy_ਗਿਣਤੀ, stats->resize_ਗਿਣਤੀ);
	fprintf(stderr, "%16lu mark bmap\n%16lu unmark_bmap\n",
		stats->mark_ਗਿਣਤੀ, stats->unmark_ਗਿਣਤੀ);
	fprintf(stderr, "%16lu test_bmap\n%16lu mark_bmap_extent\n",
		stats->test_ਗਿਣਤੀ, stats->mark_ext_ਗਿਣਤੀ);
	fprintf(stderr, "%16lu unmark_bmap_extent\n"
		"%16lu test_clear_bmap_extent\n",
		stats->unmark_ext_ਗਿਣਤੀ, stats->test_ext_ਗਿਣਤੀ);
	fprintf(stderr, "%16lu set_bmap_range\n%16lu set_bmap_range\n",
		stats->ਸੈੱਟ_range_ਗਿਣਤੀ, stats->get_range_ਗਿਣਤੀ);
	fprintf(stderr, "%16lu clear_bmap\n%16lu contiguous bit test (%.2f%%)\n",
		stats->clear_ਗਿਣਤੀ, stats->test_seq, test_seq_perc);
	fprintf(stderr, "%16lu contiguous bit mark (%.2f%%)\n"
		"%16llu bits tested backwards (%.2f%%)\n",
		stats->mark_seq, mark_seq_perc,
		stats->test_ਪਿੱਠ, test_ਪਿੱਠ_perc);
	fprintf(stderr, "%16llu bits marked backwards (%.2f%%)\n"
		"%16.2f seconds in use\n",
		stats->mark_ਪਿੱਠ, mark_ਪਿੱਠ_perc, inuse);
#ਜੇ_ਅੰਤ /* ENABLE_BMAP_STATS_OPS */
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ ext2fs_free_generic_bmap(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ਜੇ (!bmap)
		ਵਾਪਸ;

	ਜੇ (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_free_generic_bitmap(gen_bmap);
		ਵਾਪਸ;
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bmap))
		ਵਾਪਸ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS
	ਜੇ (ext2fs_ਸੁਰੱਖਿਅਤ_getenv("E2FSPROGS_BITMAP_STATS")) {
		ext2fs_print_bmap_statistics(bmap);
		bmap->bitmap_ops->print_stats(bmap);
	}
#ਜੇ_ਅੰਤ

	bmap->bitmap_ops->free_bmap(bmap);

	ਜੇ (bmap->description) {
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
	ਅੱਖਰ *descr, *new_descr;
	ext2fs_generic_bitmap_64 new_bmap;
	errcode_t retval;

	ਜੇ (!src)
		ਵਾਪਸ EINVAL;

	ਜੇ (EXT2FS_IS_32_BITMAP(src))
		ਵਾਪਸ ext2fs_copy_generic_bitmap(gen_src, dest);

	ਜੇ (!EXT2FS_IS_64_BITMAP(src))
		ਵਾਪਸ EINVAL;

	/* Allocate a new bitmap struct */
	retval = ext2fs_get_memzero(ਆਕਾਰ(ਬਣਤਰ ext2fs_struct_generic_bitmap_64),
				    &new_bmap);
	ਜੇ (retval)
		ਵਾਪਸ retval;


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
	src->stats.copy_ਗਿਣਤੀ++;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS
	ਜੇ (gettimeofday(&new_bmap->stats.created,
			 (ਬਣਤਰ timezone *) ਨਿਰਾ) == -1) {
		perror("gettimeofday");
		ext2fs_free_mem(&new_bmap);
		ਵਾਪਸ 1;
	}
	new_bmap->stats.type = src->stats.type;
#ਜੇ_ਅੰਤ

	/* Copy all the high-level parts over */
	new_bmap->magic = src->magic;
	new_bmap->fs = src->fs;
	new_bmap->ਸ਼ੁਰੂ = src->ਸ਼ੁਰੂ;
	new_bmap->ਸਿਰਾ = src->ਸਿਰਾ;
	new_bmap->real_ਸਿਰਾ = src->real_ਸਿਰਾ;
	new_bmap->bitmap_ops = src->bitmap_ops;
	new_bmap->ਅਧਾਰ_error_ਕੋਡ = src->ਅਧਾਰ_error_ਕੋਡ;
	new_bmap->cluster_ਬਿੱਟ = src->cluster_ਬਿੱਟ;

	descr = src->description;
	ਜੇ (descr) {
		retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(descr)+10, &new_descr);
		ਜੇ (retval) {
			ext2fs_free_mem(&new_bmap);
			ਵਾਪਸ retval;
		}
		strcpy(new_descr, "copy of ");
		strcat(new_descr, descr);
		new_bmap->description = new_descr;
	}

	retval = src->bitmap_ops->copy_bmap(src, new_bmap);
	ਜੇ (retval) {
		ext2fs_free_mem(&new_bmap->description);
		ext2fs_free_mem(&new_bmap);
		ਵਾਪਸ retval;
	}

	*dest = (ext2fs_generic_bitmap) new_bmap;

	ਵਾਪਸ 0;
}

errcode_t ext2fs_resize_generic_bmap(ext2fs_generic_bitmap gen_bmap,
				     __u64 new_ਸਿਰਾ,
				     __u64 new_real_ਸਿਰਾ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ਜੇ (!bmap)
		ਵਾਪਸ EINVAL;

	ਜੇ (EXT2FS_IS_32_BITMAP(bmap))
		ਵਾਪਸ ext2fs_resize_generic_bitmap(gen_bmap->magic, new_ਸਿਰਾ,
						    new_real_ਸਿਰਾ, gen_bmap);

	ਜੇ (!EXT2FS_IS_64_BITMAP(bmap))
		ਵਾਪਸ EINVAL;

	INC_STAT(bmap, resize_ਗਿਣਤੀ);

	ਵਾਪਸ bmap->bitmap_ops->resize_bmap(bmap, new_ਸਿਰਾ, new_real_ਸਿਰਾ);
}

errcode_t ext2fs_fudge_generic_bmap_ਸਿਰਾ(ext2fs_generic_bitmap gen_bitmap,
					errcode_t neq,
					__u64 ਸਿਰਾ, __u64 *oend)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ਜੇ (!bitmap)
		ਵਾਪਸ EINVAL;

	ਜੇ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ext2_ino_t tmp_oend;
		ਪੂਰਨ retval;

		retval = ext2fs_fudge_generic_bitmap_ਸਿਰਾ(gen_bitmap,
							 bitmap->magic,
							 neq, ਸਿਰਾ, &tmp_oend);
		ਜੇ (oend)
			*oend = tmp_oend;
		ਵਾਪਸ retval;
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bitmap))
		ਵਾਪਸ EINVAL;

	ਜੇ (ਸਿਰਾ > bitmap->real_ਸਿਰਾ)
		ਵਾਪਸ neq;
	ਜੇ (oend)
		*oend = bitmap->ਸਿਰਾ;
	bitmap->ਸਿਰਾ = ਸਿਰਾ;
	ਵਾਪਸ 0;
}

__u64 ext2fs_get_generic_bmap_ਸ਼ੁਰੂ(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ਜੇ (!bitmap)
		ਵਾਪਸ EINVAL;

	ਜੇ (EXT2FS_IS_32_BITMAP(bitmap))
		ਵਾਪਸ ext2fs_get_generic_bitmap_ਸ਼ੁਰੂ(gen_bitmap);

	ਜੇ (!EXT2FS_IS_64_BITMAP(bitmap))
		ਵਾਪਸ EINVAL;

	ਵਾਪਸ bitmap->ਸ਼ੁਰੂ;
}

__u64 ext2fs_get_generic_bmap_ਸਿਰਾ(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ਜੇ (!bitmap)
		ਵਾਪਸ EINVAL;

	ਜੇ (EXT2FS_IS_32_BITMAP(bitmap))
		ਵਾਪਸ ext2fs_get_generic_bitmap_ਸਿਰਾ(gen_bitmap);

	ਜੇ (!EXT2FS_IS_64_BITMAP(bitmap))
		ਵਾਪਸ EINVAL;

	ਵਾਪਸ bitmap->ਸਿਰਾ;
}

ਖਾਲੀ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ਜੇ (EXT2FS_IS_32_BITMAP(bitmap))
		ext2fs_clear_generic_bitmap(gen_bitmap);
	ਨਹੀਂਤਾਂ
		bitmap->bitmap_ops->clear_bmap(bitmap);
}

ਪੂਰਨ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ਜੇ (!bitmap)
		ਵਾਪਸ 0;

	ਜੇ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ਜੇ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap,
					    EXT2FS_MARK_ERROR, 0xffffffff);
			ਵਾਪਸ 0;
		}
		ਵਾਪਸ ext2fs_mark_generic_bitmap(gen_bitmap, arg);
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bitmap))
		ਵਾਪਸ 0;

	arg >>= bitmap->cluster_ਬਿੱਟ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
	ਜੇ (arg == bitmap->stats.ਆਖਰੀ_marked + 1)
		bitmap->stats.mark_seq++;
	ਜੇ (arg < bitmap->stats.ਆਖਰੀ_marked)
		bitmap->stats.mark_ਪਿੱਠ++;
	bitmap->stats.ਆਖਰੀ_marked = arg;
	bitmap->stats.mark_ਗਿਣਤੀ++;
#ਜੇ_ਅੰਤ

	ਜੇ ((arg < bitmap->ਸ਼ੁਰੂ) || (arg > bitmap->ਸਿਰਾ)) {
		warn_bitmap(bitmap, EXT2FS_MARK_ERROR, arg);
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ bitmap->bitmap_ops->mark_bmap(bitmap, arg);
}

ਪੂਰਨ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			       __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

	ਜੇ (!bitmap)
		ਵਾਪਸ 0;

	ਜੇ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ਜੇ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			ਵਾਪਸ 0;
		}
		ਵਾਪਸ ext2fs_unmark_generic_bitmap(gen_bitmap, arg);
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bitmap))
		ਵਾਪਸ 0;

	arg >>= bitmap->cluster_ਬਿੱਟ;

	INC_STAT(bitmap, unmark_ਗਿਣਤੀ);

	ਜੇ ((arg < bitmap->ਸ਼ੁਰੂ) || (arg > bitmap->ਸਿਰਾ)) {
		warn_bitmap(bitmap, EXT2FS_UNMARK_ERROR, arg);
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ bitmap->bitmap_ops->unmark_bmap(bitmap, arg);
}

ਪੂਰਨ ext2fs_test_generic_bmap(ext2fs_generic_bitmap gen_bitmap,
			     __u64 arg)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;
	ਜੇ (!bitmap)
		ਵਾਪਸ 0;

	ਜੇ (EXT2FS_IS_32_BITMAP(bitmap)) {
		ਜੇ (arg & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR,
					    0xffffffff);
			ਵਾਪਸ 0;
		}
		ਵਾਪਸ ext2fs_test_generic_bitmap(gen_bitmap, arg);
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bitmap))
		ਵਾਪਸ 0;

	arg >>= bitmap->cluster_ਬਿੱਟ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
	bitmap->stats.test_ਗਿਣਤੀ++;
	ਜੇ (arg == bitmap->stats.ਆਖਰੀ_tested + 1)
		bitmap->stats.test_seq++;
	ਜੇ (arg < bitmap->stats.ਆਖਰੀ_tested)
		bitmap->stats.test_ਪਿੱਠ++;
	bitmap->stats.ਆਖਰੀ_tested = arg;
#ਜੇ_ਅੰਤ

	ਜੇ ((arg < bitmap->ਸ਼ੁਰੂ) || (arg > bitmap->ਸਿਰਾ)) {
		warn_bitmap(bitmap, EXT2FS_TEST_ERROR, arg);
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ bitmap->bitmap_ops->test_bmap(bitmap, arg);
}

errcode_t ext2fs_ਸੈੱਟ_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਵਿੱਚ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ਜੇ (!bmap)
		ਵਾਪਸ EINVAL;

	ਜੇ (EXT2FS_IS_32_BITMAP(bmap)) {
		ਜੇ ((ਸ਼ੁਰੂ+ਅੰਕ-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap, EXT2FS_UNMARK_ERROR,
					    0xffffffff);
			ਵਾਪਸ EINVAL;
		}
		ਵਾਪਸ ext2fs_ਸੈੱਟ_generic_bitmap_range(gen_bmap, bmap->magic,
						       ਸ਼ੁਰੂ, ਅੰਕ, ਵਿੱਚ);
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bmap))
		ਵਾਪਸ EINVAL;

	INC_STAT(bmap, ਸੈੱਟ_range_ਗਿਣਤੀ);

	ਵਾਪਸ bmap->bitmap_ops->ਸੈੱਟ_bmap_range(bmap, ਸ਼ੁਰੂ, ਅੰਕ, ਵਿੱਚ);
}

errcode_t ext2fs_get_generic_bmap_range(ext2fs_generic_bitmap gen_bmap,
					__u64 ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਬਾਹਰਲਾ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;

	ਜੇ (!bmap)
		ਵਾਪਸ EINVAL;

	ਜੇ (EXT2FS_IS_32_BITMAP(bmap)) {
		ਜੇ ((ਸ਼ੁਰੂ+ਅੰਕ-1) & ~0xffffffffULL) {
			ext2fs_warn_bitmap2(gen_bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ਵਾਪਸ EINVAL;
		}
		ਵਾਪਸ ext2fs_get_generic_bitmap_range(gen_bmap, bmap->magic,
						       ਸ਼ੁਰੂ, ਅੰਕ, ਬਾਹਰਲਾ);
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bmap))
		ਵਾਪਸ EINVAL;

	INC_STAT(bmap, get_range_ਗਿਣਤੀ);

	ਵਾਪਸ bmap->bitmap_ops->get_bmap_range(bmap, ਸ਼ੁਰੂ, ਅੰਕ, ਬਾਹਰਲਾ);
}

errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
				      ext2fs_generic_bitmap gen_bm1,
				      ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_64 bm1 = (ext2fs_generic_bitmap_64) gen_bm1;
	ext2fs_generic_bitmap_64 bm2 = (ext2fs_generic_bitmap_64) gen_bm2;
	blk64_t	i;

	ਜੇ (!bm1 || !bm2)
		ਵਾਪਸ EINVAL;
	ਜੇ (bm1->magic != bm2->magic)
		ਵਾਪਸ EINVAL;

	/* Now we know both bitmaps have the same magic */
	ਜੇ (EXT2FS_IS_32_BITMAP(bm1))
		ਵਾਪਸ ext2fs_compare_generic_bitmap(bm1->magic, neq,
						     gen_bm1, gen_bm2);

	ਜੇ (!EXT2FS_IS_64_BITMAP(bm1))
		ਵਾਪਸ EINVAL;

	ਜੇ ((bm1->ਸ਼ੁਰੂ != bm2->ਸ਼ੁਰੂ) ||
	    (bm1->ਸਿਰਾ != bm2->ਸਿਰਾ))
		ਵਾਪਸ neq;

	ਲਈ (i = bm1->ਸ਼ੁਰੂ; i < bm1->ਸਿਰਾ; i++) {
		ਪੂਰਨ ret1, ret2;
		ret1 = !!ext2fs_test_generic_bmap(gen_bm1, i);
		ret2 = !!ext2fs_test_generic_bmap(gen_bm2, i);
		ਜੇ (ret1 != ret2) {
			ਵਾਪਸ neq;
		}
	}

	ਵਾਪਸ 0;
}

ਖਾਲੀ ext2fs_ਸੈੱਟ_generic_bmap_padding(ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	ਸ਼ੁਰੂ, ਅੰਕ;

	ਜੇ (EXT2FS_IS_32_BITMAP(bmap)) {
		ext2fs_ਸੈੱਟ_generic_bitmap_padding(gen_bmap);
		ਵਾਪਸ;
	}

	ਸ਼ੁਰੂ = bmap->ਸਿਰਾ + 1;
	ਅੰਕ = bmap->real_ਸਿਰਾ - bmap->ਸਿਰਾ;
	bmap->bitmap_ops->mark_bmap_extent(bmap, ਸ਼ੁਰੂ, ਅੰਕ);
	/* XXX ought to warn on error */
}

ਪੂਰਨ ext2fs_test_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap gen_bmap,
				    blk64_t ਖੰਡ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	ਸਿਰਾ = ਖੰਡ + ਅੰਕ;

	ਜੇ (!bmap)
		ਵਾਪਸ EINVAL;

	ਜੇ (ਅੰਕ == 1)
		ਵਾਪਸ !ext2fs_test_generic_bmap((ext2fs_generic_bitmap)
						 bmap, ਖੰਡ);

	ਜੇ (EXT2FS_IS_32_BITMAP(bmap)) {
		ਜੇ ((ਖੰਡ & ~0xffffffffULL) ||
		    ((ਖੰਡ+ਅੰਕ-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ਵਾਪਸ EINVAL;
		}
		ਵਾਪਸ ext2fs_test_ਖੰਡ_bitmap_range(
			(ext2fs_generic_bitmap) bmap, ਖੰਡ, ਅੰਕ);
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bmap))
		ਵਾਪਸ EINVAL;

	INC_STAT(bmap, test_ext_ਗਿਣਤੀ);

	/* convert to clusters if necessary */
	ਖੰਡ >>= bmap->cluster_ਬਿੱਟ;
	ਸਿਰਾ += (1ULL << bmap->cluster_ਬਿੱਟ) - 1;
	ਸਿਰਾ >>= bmap->cluster_ਬਿੱਟ;
	ਅੰਕ = ਸਿਰਾ - ਖੰਡ;

	ਜੇ ((ਖੰਡ < bmap->ਸ਼ੁਰੂ) || (ਖੰਡ > bmap->ਸਿਰਾ) ||
	    (ਖੰਡ+ਅੰਕ-1 > bmap->ਸਿਰਾ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ਖੰਡ_TEST, ਖੰਡ,
				   bmap->description);
		ਵਾਪਸ EINVAL;
	}

	ਵਾਪਸ bmap->bitmap_ops->test_clear_bmap_extent(bmap, ਖੰਡ, ਅੰਕ);
}

ਖਾਲੀ ext2fs_mark_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap gen_bmap,
				     blk64_t ਖੰਡ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	ਸਿਰਾ = ਖੰਡ + ਅੰਕ;

	ਜੇ (!bmap)
		ਵਾਪਸ;

	ਜੇ (EXT2FS_IS_32_BITMAP(bmap)) {
		ਜੇ ((ਖੰਡ & ~0xffffffffULL) ||
		    ((ਖੰਡ+ਅੰਕ-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ਵਾਪਸ;
		}
		ext2fs_mark_ਖੰਡ_bitmap_range((ext2fs_generic_bitmap) bmap,
					       ਖੰਡ, ਅੰਕ);
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bmap))
		ਵਾਪਸ;

	INC_STAT(bmap, mark_ext_ਗਿਣਤੀ);

	/* convert to clusters if necessary */
	ਖੰਡ >>= bmap->cluster_ਬਿੱਟ;
	ਸਿਰਾ += (1ULL << bmap->cluster_ਬਿੱਟ) - 1;
	ਸਿਰਾ >>= bmap->cluster_ਬਿੱਟ;
	ਅੰਕ = ਸਿਰਾ - ਖੰਡ;

	ਜੇ ((ਖੰਡ < bmap->ਸ਼ੁਰੂ) || (ਖੰਡ > bmap->ਸਿਰਾ) ||
	    (ਖੰਡ+ਅੰਕ-1 > bmap->ਸਿਰਾ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ਖੰਡ_MARK, ਖੰਡ,
				   bmap->description);
		ਵਾਪਸ;
	}

	bmap->bitmap_ops->mark_bmap_extent(bmap, ਖੰਡ, ਅੰਕ);
}

ਖਾਲੀ ext2fs_unmark_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap gen_bmap,
				       blk64_t ਖੰਡ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	__u64	ਸਿਰਾ = ਖੰਡ + ਅੰਕ;

	ਜੇ (!bmap)
		ਵਾਪਸ;

	ਜੇ (EXT2FS_IS_32_BITMAP(bmap)) {
		ਜੇ ((ਖੰਡ & ~0xffffffffULL) ||
		    ((ਖੰਡ+ਅੰਕ-1) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2((ext2fs_generic_bitmap) bmap,
					    EXT2FS_UNMARK_ERROR, 0xffffffff);
			ਵਾਪਸ;
		}
		ext2fs_unmark_ਖੰਡ_bitmap_range((ext2fs_generic_bitmap) bmap,
						 ਖੰਡ, ਅੰਕ);
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bmap))
		ਵਾਪਸ;

	INC_STAT(bmap, unmark_ext_ਗਿਣਤੀ);

	/* convert to clusters if necessary */
	ਖੰਡ >>= bmap->cluster_ਬਿੱਟ;
	ਸਿਰਾ += (1ULL << bmap->cluster_ਬਿੱਟ) - 1;
	ਸਿਰਾ >>= bmap->cluster_ਬਿੱਟ;
	ਅੰਕ = ਸਿਰਾ - ਖੰਡ;

	ਜੇ ((ਖੰਡ < bmap->ਸ਼ੁਰੂ) || (ਖੰਡ > bmap->ਸਿਰਾ) ||
	    (ਖੰਡ+ਅੰਕ-1 > bmap->ਸਿਰਾ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ਖੰਡ_UNMARK, ਖੰਡ,
				   bmap->description);
		ਵਾਪਸ;
	}

	bmap->bitmap_ops->unmark_bmap_extent(bmap, ਖੰਡ, ਅੰਕ);
}

ਖਾਲੀ ext2fs_warn_bitmap32(ext2fs_generic_bitmap gen_bitmap, ਅਚਲ ਅੱਖਰ *func)
{
	ext2fs_generic_bitmap_64 bitmap = (ext2fs_generic_bitmap_64) gen_bitmap;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
	ਜੇ (bitmap && bitmap->description)
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap for %s", func,
			bitmap->description);
	ਨਹੀਂਤਾਂ
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"called %s with 64-bit bitmap", func);
#ਜੇ_ਅੰਤ
}

errcode_t ext2fs_convert_subcluster_bitmap(ext2_filsys fs,
					   ext2fs_ਖੰਡ_bitmap *bitmap)
{
	ext2fs_generic_bitmap_64 bmap, cmap;
	ext2fs_ਖੰਡ_bitmap	gen_bmap = *bitmap, gen_cmap;
	errcode_t		retval;
	blk64_t			i, ਅਗਲਾ, b_ਸਿਰਾ, c_ਸਿਰਾ;

	bmap = (ext2fs_generic_bitmap_64) gen_bmap;
	ਜੇ (fs->cluster_ਅਨੁਪਾਤ_ਬਿੱਟ == ext2fs_get_bitmap_granularity(gen_bmap))
		ਵਾਪਸ 0;	/* Nothing to do */

	retval = ext2fs_allocate_ਖੰਡ_bitmap(fs, "converted cluster bitmap",
					      &gen_cmap);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	cmap = (ext2fs_generic_bitmap_64) gen_cmap;
	i = bmap->ਸ਼ੁਰੂ;
	b_ਸਿਰਾ = bmap->ਸਿਰਾ;
	bmap->ਸਿਰਾ = bmap->real_ਸਿਰਾ;
	c_ਸਿਰਾ = cmap->ਸਿਰਾ;
	cmap->ਸਿਰਾ = cmap->real_ਸਿਰਾ;
	ਜਦੋਂ (i < bmap->real_ਸਿਰਾ) {
		retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_ਖੰਡ_bitmap2(gen_bmap,
						i, bmap->real_ਸਿਰਾ, &ਅਗਲਾ);
		ਜੇ (retval)
			ਰੋਕੋ;
		ext2fs_mark_ਖੰਡ_bitmap2(gen_cmap, ਅਗਲਾ);
		i = EXT2FS_C2B(fs, EXT2FS_B2C(fs, ਅਗਲਾ) + 1);
	}
	bmap->ਸਿਰਾ = b_ਸਿਰਾ;
	cmap->ਸਿਰਾ = c_ਸਿਰਾ;
	ext2fs_free_ਖੰਡ_bitmap(gen_bmap);
	*bitmap = (ext2fs_ਖੰਡ_bitmap) cmap;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
					      __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ, __u64 *ਬਾਹਰਲਾ)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	ਜੇ (!bitmap)
		ਵਾਪਸ EINVAL;

	ਜੇ (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		ਜੇ (((ਸ਼ੁਰੂ) & ~0xffffffffULL) ||
		    ((ਸਿਰਾ) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, ਸ਼ੁਰੂ);
			ਵਾਪਸ EINVAL;
		}

		retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_generic_bitmap(bitmap, ਸ਼ੁਰੂ,
							       ਸਿਰਾ, &blk);
		ਜੇ (retval == 0)
			*ਬਾਹਰਲਾ = blk;
		ਵਾਪਸ retval;
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bitmap))
		ਵਾਪਸ EINVAL;

	cstart = ਸ਼ੁਰੂ >> bmap64->cluster_ਬਿੱਟ;
	cend = ਸਿਰਾ >> bmap64->cluster_ਬਿੱਟ;

	ਜੇ (cstart < bmap64->ਸ਼ੁਰੂ || cend > bmap64->ਸਿਰਾ || ਸ਼ੁਰੂ > ਸਿਰਾ) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, ਸ਼ੁਰੂ);
		ਵਾਪਸ EINVAL;
	}

	ਜੇ (bmap64->bitmap_ops->ਲੱਭੋ_ਪਹਿਲਾ_zero) {
		retval = bmap64->bitmap_ops->ਲੱਭੋ_ਪਹਿਲਾ_zero(bmap64, cstart,
							     cend, &cout);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	ਮਿਲਿਆ:
		cout <<= bmap64->cluster_ਬਿੱਟ;
		*ਬਾਹਰਲਾ = (cout >= ਸ਼ੁਰੂ) ? cout : ਸ਼ੁਰੂ;
		ਵਾਪਸ 0;
	}

	ਲਈ (cout = cstart; cout <= cend; cout++)
		ਜੇ (!bmap64->bitmap_ops->test_bmap(bmap64, cout))
			ਜਾਓ ਮਿਲਿਆ;

	ਵਾਪਸ ENOENT;
}

errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_generic_bmap(ext2fs_generic_bitmap bitmap,
					     __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ, __u64 *ਬਾਹਰਲਾ)
{
	ext2fs_generic_bitmap_64 bmap64 = (ext2fs_generic_bitmap_64) bitmap;
	__u64 cstart, cend, cout;
	errcode_t retval;

	ਜੇ (!bitmap)
		ਵਾਪਸ EINVAL;

	ਜੇ (EXT2FS_IS_32_BITMAP(bitmap)) {
		blk_t blk = 0;

		ਜੇ (((ਸ਼ੁਰੂ) & ~0xffffffffULL) ||
		    ((ਸਿਰਾ) & ~0xffffffffULL)) {
			ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, ਸ਼ੁਰੂ);
			ਵਾਪਸ EINVAL;
		}

		retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_generic_bitmap(bitmap, ਸ਼ੁਰੂ,
							      ਸਿਰਾ, &blk);
		ਜੇ (retval == 0)
			*ਬਾਹਰਲਾ = blk;
		ਵਾਪਸ retval;
	}

	ਜੇ (!EXT2FS_IS_64_BITMAP(bitmap))
		ਵਾਪਸ EINVAL;

	cstart = ਸ਼ੁਰੂ >> bmap64->cluster_ਬਿੱਟ;
	cend = ਸਿਰਾ >> bmap64->cluster_ਬਿੱਟ;

	ਜੇ (cstart < bmap64->ਸ਼ੁਰੂ || cend > bmap64->ਸਿਰਾ || ਸ਼ੁਰੂ > ਸਿਰਾ) {
		warn_bitmap(bmap64, EXT2FS_TEST_ERROR, ਸ਼ੁਰੂ);
		ਵਾਪਸ EINVAL;
	}

	ਜੇ (bmap64->bitmap_ops->ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ) {
		retval = bmap64->bitmap_ops->ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ(bmap64, cstart,
							    cend, &cout);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	ਮਿਲਿਆ:
		cout <<= bmap64->cluster_ਬਿੱਟ;
		*ਬਾਹਰਲਾ = (cout >= ਸ਼ੁਰੂ) ? cout : ਸ਼ੁਰੂ;
		ਵਾਪਸ 0;
	}

	ਲਈ (cout = cstart; cout <= cend; cout++)
		ਜੇ (bmap64->bitmap_ops->test_bmap(bmap64, cout))
			ਜਾਓ ਮਿਲਿਆ;

	ਵਾਪਸ ENOENT;
}

errcode_t ext2fs_ਗਿਣਤੀ_ਵਰਤਿਆ_ਖੰਡਾਂ(ext2_filsys fs, blk64_t ਸ਼ੁਰੂ,
				   blk64_t ਸਿਰਾ, blk64_t *ਬਾਹਰਲਾ)
{
	blk64_t		ਅਗਲਾ;
	blk64_t		tot_ਸੈੱਟ = 0;
	errcode_t	retval = 0;

	ਜਦੋਂ (ਸ਼ੁਰੂ < ਸਿਰਾ) {
		retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_ਖੰਡ_bitmap2(fs->ਖੰਡ_map,
							ਸ਼ੁਰੂ, ਸਿਰਾ, &ਅਗਲਾ);
		ਜੇ (retval) {
			ਜੇ (retval == ENOENT)
				retval = 0;
			ਰੋਕੋ;
		}
		ਸ਼ੁਰੂ = ਅਗਲਾ;

		retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_ਖੰਡ_bitmap2(fs->ਖੰਡ_map,
							ਸ਼ੁਰੂ, ਸਿਰਾ, &ਅਗਲਾ);
		ਜੇ (retval == 0) {
			tot_ਸੈੱਟ += ਅਗਲਾ - ਸ਼ੁਰੂ;
			ਸ਼ੁਰੂ  = ਅਗਲਾ + 1;
		} ਨਹੀਂਤਾਂ ਜੇ (retval == ENOENT) {
			retval = 0;
			tot_ਸੈੱਟ += ਸਿਰਾ - ਸ਼ੁਰੂ + 1;
			ਰੋਕੋ;
		} ਨਹੀਂਤਾਂ
			ਰੋਕੋ;
	}

	ਜੇ (!retval)
		*ਬਾਹਰਲਾ = tot_ਸੈੱਟ;
	ਵਾਪਸ retval;
}

errcode_t ext2fs_ਗਿਣਤੀ_ਵਰਤਿਆ_clusters(ext2_filsys fs, blk64_t ਸ਼ੁਰੂ,
				     blk64_t ਸਿਰਾ, blk64_t *ਬਾਹਰਲਾ)
{
	blk64_t		tot_ਸੈੱਟ = 0;
	errcode_t	retval = 0;

	retval = ext2fs_ਗਿਣਤੀ_ਵਰਤਿਆ_ਖੰਡਾਂ(fs, ਸ਼ੁਰੂ, ਸਿਰਾ, &tot_ਸੈੱਟ);

	ਜੇ (!retval)
		*ਬਾਹਰਲਾ = EXT2_FS_ਅੰਕ_B2_C(fs, tot_ਸੈੱਟ);
	ਵਾਪਸ retval;
}
