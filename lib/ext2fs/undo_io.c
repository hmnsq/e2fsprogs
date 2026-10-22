/*
 * undo_io.c --- This is the undo io manager that copies the old data that
 * copies the old data being overwritten into a tdb database
 *
 * Copyright IBM Corporation, 2007
 * Author Aneesh Kumar K.V <aneesh.kumar@linux.vnet.ibm.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE64_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE64_ਸਰੋਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/utsname.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_RESOURCE_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/resource.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATTR(x) __attribute__(x)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATTR(x)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEBUG

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
# define dbg_printf(f, a...)  ਕਰੋ {ਲਿਖੋ(f, ## a); fflush(stdout); } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
# define dbg_printf(f, a...)
#ਜੇ_ਅੰਤ

/*
 * For checking structure magic numbers...
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CHECK_MAGIC(ਬਣਤਰ, ਕੋਡ) \
	  ਜੇ ((ਬਣਤਰ)->magic != (ਕੋਡ)) ਵਾਪਸ (ਕੋਡ)
/*
 * Undo file format: The file is cut up into undo_header.block_size blocks.
 * The first block contains the header.
 * The second block contains the superblock.
 * There is then a repeating series of blocks as follows:
 *   A key block, which contains undo_keys to map the following data blocks.
 *   Data blocks
 * (Note that there are pointers to the first key block and the sb, so this
 * order isn't strictly necessary.)
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ E2UNDO_MAGIC "E2UNDO02"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KEYBLOCK_MAGIC 0xCADECADE

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ E2_UNDO_STATE_ਮੁਕੰਮਲ	0x1	/* undo file is complete */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ E2_UNDO_MIN_ਖੰਡ_ਮਾਪ	1024	/* undo blocks are no less than 1KB */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ E2_UNDO_MAX_ਖੰਡ_ਮਾਪ	1048576	/* undo blocks are no more than 1MB */

ਬਣਤਰ undo_ਸਿਰਲੇਖ {
	ਅੱਖਰ magic[8];		/* "E2UNDO02" */
	__le64 ਅੰਕ_ਕੁੰਜੀਆਂ;	/* how many keys? */
	__le64 super_ਵਿੱਥ;	/* where in the file is the superblock copy? */
	__le64 ਕੁੰਜੀ_ਵਿੱਥ;	/* where do the key/data block chunks start? */
	__le32 ਖੰਡ_ਮਾਪ;	/* block size of the undo file */
	__le32 fs_ਖੰਡ_ਮਾਪ;	/* block size of the target device */
	__le32 sb_ਚੱਕਰੀ_ਜਾਂਚ;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_ਵਿੱਥ;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 ਸਿਰਲੇਖ_ਚੱਕਰੀ_ਜਾਂਚ;	/* crc32c of this header (but not this field) */
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ E2_UNDO_MAX_EXTENT_ਖੰਡਾਂ	512	/* max extent size, in blocks */

ਬਣਤਰ undo_ਕੁੰਜੀ {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_ਚੱਕਰੀ_ਜਾਂਚ;		/* crc32c of the block */
	__le32 ਮਾਪ;		/* how many bytes in this block? */
};

ਬਣਤਰ undo_ਕੁੰਜੀ_ਖੰਡ {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 ਚੱਕਰੀ_ਜਾਂਚ;		/* block checksum */
	__le64 reserved;	/* zero */

#ਜੇ __STDC_VERSION__ >= 199901L
	ਬਣਤਰ undo_ਕੁੰਜੀ ਕੁੰਜੀਆਂ[];		/* keys, which come immediately after */
#ਨਹੀਂਤਾਂ
	ਬਣਤਰ undo_ਕੁੰਜੀ ਕੁੰਜੀਆਂ[0];	/* keys, which come immediately after */
#ਜੇ_ਅੰਤ
};

ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ {
	ਪੂਰਨ	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_ਅੰਕ;			/* next free block */
	blk64_t ਕੁੰਜੀ_blk_ਅੰਕ;			/* current key block location */
	blk64_t super_blk_ਅੰਕ;			/* superblock location */
	blk64_t ਪਹਿਲਾ_ਕੁੰਜੀ_blk;			/* first key block location */
	ਬਣਤਰ undo_ਕੁੰਜੀ_ਖੰਡ *keyb;
	size_t ਅੰਕ_ਕੁੰਜੀਆਂ, ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ;

	/* The backing io channel */
	io_channel real;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ tdb_ਅੰਕੜੇ_ਮਾਪ;
	ਪੂਰਨ tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t ਵਿੱਥ;

	ext2fs_ਖੰਡ_bitmap written_ਖੰਡ_map;
	ਬਣਤਰ struct_ext2_filsys fake_fs;
	ਅੱਖਰ *tdb_file;
	ਬਣਤਰ undo_ਸਿਰਲੇਖ hdr;
};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕੁੰਜੀਆਂ_ਪ੍ਰਤੀ_ਖੰਡ(d) (((d)->tdb_ਅੰਕੜੇ_ਮਾਪ / ਆਕਾਰ(ਬਣਤਰ undo_ਕੁੰਜੀ)) - 1)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ E2_UNDO_FEATURE_COMPAT_FS_ਵਿੱਥ 0x1	/* the filesystem offset */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ e2undo_ਸੈੱਟ_feature_fs_ਵਿੱਥ(ਬਣਤਰ undo_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ) {
	ਸਿਰਲੇਖ->f_compat |= ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(E2_UNDO_FEATURE_COMPAT_FS_ਵਿੱਥ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ e2undo_clear_feature_fs_ਵਿੱਥ(ਬਣਤਰ undo_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ) {
	ਸਿਰਲੇਖ->f_compat &= ~ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(E2_UNDO_FEATURE_COMPAT_FS_ਵਿੱਥ);
}

ਸਥਿਰ io_manager undo_io_backing_manager;
ਸਥਿਰ ਅੱਖਰ *tdb_file;
ਸਥਿਰ ਪੂਰਨ actual_ਮਾਪ;

errcode_t ਸੈੱਟ_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	ਵਾਪਸ 0;
}

errcode_t ਸੈੱਟ_undo_io_backup_file(ਅੱਖਰ *file_name)
{
	tdb_file = strdup(file_name);

	ਜੇ (tdb_file == ਨਿਰਾ) {
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t write_undo_indexes(ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ, ਪੂਰਨ flush)
{
	errcode_t retval;
	ਬਣਤਰ ext2_super_ਖੰਡ super;
	io_channel channel;
	ਪੂਰਨ ਖੰਡ_ਮਾਪ;
	__u32 sb_ਚੱਕਰੀ_ਜਾਂਚ, hdr_ਚੱਕਰੀ_ਜਾਂਚ;

	/* Spit out a key block, if there's any data */
	ਜੇ (ਅੰਕੜੇ->ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ) {
		ਅੰਕੜੇ->keyb->magic = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(KEYBLOCK_MAGIC);
		ਅੰਕੜੇ->keyb->ਚੱਕਰੀ_ਜਾਂਚ = 0;
		ਅੰਕੜੇ->keyb->ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(
					 ext2fs_crc32c_le(~0,
					 (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)ਅੰਕੜੇ->keyb,
					 ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ));
		dbg_printf("Writing keyblock to blk %llu\n", ਅੰਕੜੇ->ਕੁੰਜੀ_blk_ਅੰਕ);
		retval = io_channel_write_blk64(ਅੰਕੜੇ->undo_file,
						ਅੰਕੜੇ->ਕੁੰਜੀ_blk_ਅੰਕ,
						1, ਅੰਕੜੇ->keyb);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		/* Move on to the next key block if it's full. */
		ਜੇ (ਅੰਕੜੇ->ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ == ਕੁੰਜੀਆਂ_ਪ੍ਰਤੀ_ਖੰਡ(ਅੰਕੜੇ)) {
			memset(ਅੰਕੜੇ->keyb, 0, ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ);
			ਅੰਕੜੇ->ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ = 0;
			ਅੰਕੜੇ->ਕੁੰਜੀ_blk_ਅੰਕ = ਅੰਕੜੇ->undo_blk_ਅੰਕ;
			ਅੰਕੜੇ->undo_blk_ਅੰਕ++;
		}
	}

	/* Prepare superblock for write */
	channel = ਅੰਕੜੇ->real;
	ਖੰਡ_ਮਾਪ = channel->ਖੰਡ_ਮਾਪ;

	io_channel_ਸੈੱਟ_blksize(channel, SUPERBLOCK_ਵਿੱਥ);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_ਮਾਪ, &super);
	ਜੇ (retval)
		ਜਾਓ err_ਬਾਹਰਲਾ;
	sb_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(~0, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&super, SUPERBLOCK_ਮਾਪ);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਅੰਕੜੇ->hdr.magic, E2UNDO_MAGIC, ਆਕਾਰ(ਅੰਕੜੇ->hdr.magic));
	ਅੰਕੜੇ->hdr.ਅੰਕ_ਕੁੰਜੀਆਂ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le64(ਅੰਕੜੇ->ਅੰਕ_ਕੁੰਜੀਆਂ);
	ਅੰਕੜੇ->hdr.super_ਵਿੱਥ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le64(ਅੰਕੜੇ->super_blk_ਅੰਕ);
	ਅੰਕੜੇ->hdr.ਕੁੰਜੀ_ਵਿੱਥ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le64(ਅੰਕੜੇ->ਪਹਿਲਾ_ਕੁੰਜੀ_blk);
	ਅੰਕੜੇ->hdr.fs_ਖੰਡ_ਮਾਪ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਖੰਡ_ਮਾਪ);
	ਅੰਕੜੇ->hdr.sb_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(sb_ਚੱਕਰੀ_ਜਾਂਚ);
	ਅੰਕੜੇ->hdr.fs_ਵਿੱਥ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le64(ਅੰਕੜੇ->ਵਿੱਥ);
	ਜੇ (ਅੰਕੜੇ->ਵਿੱਥ)
		e2undo_ਸੈੱਟ_feature_fs_ਵਿੱਥ(&ਅੰਕੜੇ->hdr);
	ਨਹੀਂਤਾਂ
		e2undo_clear_feature_fs_ਵਿੱਥ(&ਅੰਕੜੇ->hdr);
	hdr_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(~0, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&ਅੰਕੜੇ->hdr,
				   ਆਕਾਰ(ਅੰਕੜੇ->hdr) -
				   ਆਕਾਰ(ਅੰਕੜੇ->hdr.ਸਿਰਲੇਖ_ਚੱਕਰੀ_ਜਾਂਚ));
	ਅੰਕੜੇ->hdr.ਸਿਰਲੇਖ_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(hdr_ਚੱਕਰੀ_ਜਾਂਚ);
	retval = io_channel_write_blk64(ਅੰਕੜੇ->undo_file, 0,
					-(ਪੂਰਨ)ਆਕਾਰ(ਅੰਕੜੇ->hdr),
					&ਅੰਕੜੇ->hdr);
	ਜੇ (retval)
		ਜਾਓ err_ਬਾਹਰਲਾ;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", ਅੰਕੜੇ->super_blk_ਅੰਕ);
	retval = io_channel_write_blk64(ਅੰਕੜੇ->undo_file, ਅੰਕੜੇ->super_blk_ਅੰਕ,
					-SUPERBLOCK_ਮਾਪ, &super);
	ਜੇ (retval)
		ਜਾਓ err_ਬਾਹਰਲਾ;

	ਜੇ (flush)
		retval = io_channel_flush(ਅੰਕੜੇ->undo_file);
err_ਬਾਹਰਲਾ:
	io_channel_ਸੈੱਟ_blksize(channel, ਖੰਡ_ਮਾਪ);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_ਤਿਆਰੀ_tdb(ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਪੂਰਨ i;
	errcode_t retval;

	ਜੇ (ਅੰਕੜੇ->tdb_written == 1)
		ਵਾਪਸ 0;

	ਅੰਕੜੇ->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&ਅੰਕੜੇ->fake_fs, 0, ਆਕਾਰ(ਅੰਕੜੇ->fake_fs));
	ਅੰਕੜੇ->fake_fs.blocksize = ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ;
	retval = ext2fs_alloc_generic_bmap(&ਅੰਕੜੇ->fake_fs,
				EXT2_ET_MAGIC_ਖੰਡ_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &ਅੰਕੜੇ->written_ਖੰਡ_map);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ, &ਅੰਕੜੇ->keyb);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਅੰਕੜੇ->ਕੁੰਜੀ_blk_ਅੰਕ = ਅੰਕੜੇ->ਪਹਿਲਾ_ਕੁੰਜੀ_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ);
	dbg_printf("Keys per block %llu\n", ਕੁੰਜੀਆਂ_ਪ੍ਰਤੀ_ਖੰਡ(ਅੰਕੜੇ));
	ਅੰਕੜੇ->hdr.ਖੰਡ_ਮਾਪ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ);
	io_channel_ਸੈੱਟ_blksize(ਅੰਕੜੇ->undo_file, ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ);

	/* Ensure that we have space for header blocks */
	ਲਈ (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(ਅੰਕੜੇ->undo_file, i, 1,
					       ਅੰਕੜੇ->keyb);
		ਜੇ (retval)
			memset(ਅੰਕੜੇ->keyb, 0, ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ);
		retval = io_channel_write_blk64(ਅੰਕੜੇ->undo_file, i, 1,
						ਅੰਕੜੇ->keyb);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		retval = io_channel_flush(ਅੰਕੜੇ->undo_file);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	memset(ਅੰਕੜੇ->keyb, 0, ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t undo_write_tdb(io_channel channel,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2)

{
	ਪੂਰਨ ਮਾਪ, sz;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ_ਅੰਕ, backing_blk_ਅੰਕ;
	errcode_t retval = 0;
	ext2_loff_t ਵਿੱਥ;
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *read_ptr;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਸਿਰਾ_ਖੰਡ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਅੰਕੜੇ_ਮਾਪ;
	ਬਣਤਰ undo_ਕੁੰਜੀ *ਕੁੰਜੀ;
	__u32 blk_ਚੱਕਰੀ_ਜਾਂਚ;

	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;

	ਜੇ (ਅੰਕੜੇ->undo_file == ਨਿਰਾ) {
		/*
		 * Transaction database not initialized
		 */
		ਵਾਪਸ 0;
	}

	ਜੇ (ਗਿਣਤੀ_2 == 1)
		ਮਾਪ = channel->ਖੰਡ_ਮਾਪ;
	ਨਹੀਂਤਾਂ {
		ਜੇ (ਗਿਣਤੀ_2 < 0)
			ਮਾਪ = -ਗਿਣਤੀ_2;
		ਨਹੀਂਤਾਂ
			ਮਾਪ = ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ;
	}

	retval = undo_ਤਿਆਰੀ_tdb(ਅੰਕੜੇ);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	ਵਿੱਥ = (ਖੰਡ * channel->ਖੰਡ_ਮਾਪ) + ਅੰਕੜੇ->ਵਿੱਥ ;
	ਖੰਡ_ਅੰਕ = ਵਿੱਥ / ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ;
	ਸਿਰਾ_ਖੰਡ = (ਵਿੱਥ + ਮਾਪ - 1) / ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ;

	ਜਦੋਂ (ਖੰਡ_ਅੰਕ <= ਸਿਰਾ_ਖੰਡ) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		ਜੇ (ext2fs_test_ਖੰਡ_bitmap2(ਅੰਕੜੇ->written_ਖੰਡ_map,
						   ਖੰਡ_ਅੰਕ)) {
			/* Try the next block */
			ਖੰਡ_ਅੰਕ++;
			ਜਾਰੀ;
		}
		ext2fs_mark_ਖੰਡ_bitmap2(ਅੰਕੜੇ->written_ਖੰਡ_map, ਖੰਡ_ਅੰਕ);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		ਵਿੱਥ = ਖੰਡ_ਅੰਕ * ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ +
				(ਅੰਕੜੇ->ਵਿੱਥ % ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ);
		backing_blk_ਅੰਕ = (ਵਿੱਥ - ਅੰਕੜੇ->ਵਿੱਥ) / channel->ਖੰਡ_ਮਾਪ;

		retval = ext2fs_get_mem(ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ, &read_ptr);
		ਜੇ (retval) {
			ਵਾਪਸ retval;
		}

		memset(read_ptr, 0, ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ);
		actual_ਮਾਪ = 0;
		ਜੇ ((ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ % channel->ਖੰਡ_ਮਾਪ) == 0)
			sz = ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ / channel->ਖੰਡ_ਮਾਪ;
		ਨਹੀਂਤਾਂ
			sz = -ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ;
		retval = io_channel_read_blk64(ਅੰਕੜੇ->real, backing_blk_ਅੰਕ,
					     sz, read_ptr);
		ਜੇ (retval) {
			ਜੇ (retval != EXT2_ET_SHORT_READ) {
				ਮੁਕਤ(read_ptr);
				ਵਾਪਸ retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			ਅੰਕੜੇ_ਮਾਪ = actual_ਮਾਪ;
		} ਨਹੀਂਤਾਂ {
			ਅੰਕੜੇ_ਮਾਪ = ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ;
		}
		ਜੇ (ਅੰਕੜੇ_ਮਾਪ == 0) {
			ਮੁਕਤ(read_ptr);
			ਖੰਡ_ਅੰਕ++;
			ਜਾਰੀ;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       ਅੰਕੜੇ_ਮਾਪ, backing_blk_ਅੰਕ, ਖੰਡ, ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ);
		ਜੇ ((ਅੰਕੜੇ_ਮਾਪ % ਅੰਕੜੇ->undo_file->ਖੰਡ_ਮਾਪ) == 0)
			sz = ਅੰਕੜੇ_ਮਾਪ / ਅੰਕੜੇ->undo_file->ਖੰਡ_ਮਾਪ;
		ਨਹੀਂਤਾਂ
			sz = -ਅੰਕੜੇ_ਮਾਪ;;
		/* extend this key? */
		ਜੇ (ਅੰਕੜੇ->ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ) {
			ਕੁੰਜੀ = ਅੰਕੜੇ->keyb->ਕੁੰਜੀਆਂ + ਅੰਕੜੇ->ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ - 1;
			keysz = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ਕੁੰਜੀ->ਮਾਪ);
		} ਨਹੀਂਤਾਂ {
			ਕੁੰਜੀ = ਨਿਰਾ;
			keysz = 0;
		}
		ਜੇ (ਕੁੰਜੀ != ਨਿਰਾ &&
		    (ext2fs_le64_ਮੰਜ਼ਿਲ_cpu(ਕੁੰਜੀ->fsblk) * channel->ਖੰਡ_ਮਾਪ +
		     channel->ਖੰਡ_ਮਾਪ - 1 +
		     keysz) / channel->ਖੰਡ_ਮਾਪ == backing_blk_ਅੰਕ &&
		    E2_UNDO_MAX_EXTENT_ਖੰਡਾਂ * ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ >
		    keysz + ਅੰਕੜੇ_ਮਾਪ) {
			blk_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ਕੁੰਜੀ->blk_ਚੱਕਰੀ_ਜਾਂਚ);
			blk_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(blk_ਚੱਕਰੀ_ਜਾਂਚ, read_ptr, ਅੰਕੜੇ_ਮਾਪ);
			ਕੁੰਜੀ->blk_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(blk_ਚੱਕਰੀ_ਜਾਂਚ);
			ਕੁੰਜੀ->ਮਾਪ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(keysz + ਅੰਕੜੇ_ਮਾਪ);
		} ਨਹੀਂਤਾਂ {
			ਅੰਕੜੇ->ਅੰਕ_ਕੁੰਜੀਆਂ++;
			ਕੁੰਜੀ = ਅੰਕੜੇ->keyb->ਕੁੰਜੀਆਂ + ਅੰਕੜੇ->ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ;
			ਅੰਕੜੇ->ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ++;
			ਕੁੰਜੀ->fsblk = ext2fs_cpu_ਮੰਜ਼ਿਲ_le64(backing_blk_ਅੰਕ);
			blk_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(~0, read_ptr, ਅੰਕੜੇ_ਮਾਪ);
			ਕੁੰਜੀ->blk_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(blk_ਚੱਕਰੀ_ਜਾਂਚ);
			ਕੁੰਜੀ->ਮਾਪ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਅੰਕੜੇ_ਮਾਪ);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       ਖੰਡ_ਅੰਕ,
		       ਅੰਕੜੇ->undo_blk_ਅੰਕ,
		       sz, ਅੰਕੜੇ->ਅੰਕ_ਕੁੰਜੀਆਂ - 1);
		retval = io_channel_write_blk64(ਅੰਕੜੇ->undo_file,
					ਅੰਕੜੇ->undo_blk_ਅੰਕ, sz, read_ptr);
		ਜੇ (retval) {
			ਮੁਕਤ(read_ptr);
			ਵਾਪਸ retval;
		}
		ਅੰਕੜੇ->undo_blk_ਅੰਕ++;
		ਮੁਕਤ(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(ਅੰਕੜੇ, 0);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		/* Next block */
		ਖੰਡ_ਅੰਕ++;
	}

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_io_read_error(io_channel channel ATTR((ਅਣਵਰਤਿਆ)),
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ ATTR((ਅਣਵਰਤਿਆ)),
				    ਪੂਰਨ ਗਿਣਤੀ_2 ATTR((ਅਣਵਰਤਿਆ)),
				    ਖਾਲੀ *ਅੰਕੜੇ ATTR((ਅਣਵਰਤਿਆ)),
				    size_t ਮਾਪ ATTR((ਅਣਵਰਤਿਆ)),
				    ਪੂਰਨ actual,
				    errcode_t error ATTR((ਅਣਵਰਤਿਆ)))
{
	actual_ਮਾਪ = actual;
	ਵਾਪਸ error;
}

ਸਥਿਰ ਖਾਲੀ undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ਸਥਿਰ ਪੂਰਨ check_filesystem(ਬਣਤਰ undo_ਸਿਰਲੇਖ *hdr, io_channel undo_file,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ blocksize, blk64_t super_ਖੰਡ,
			    io_channel channel)
{
	ਬਣਤਰ ext2_super_ਖੰਡ super, *sb;
	ਅੱਖਰ *buf;
	__u32 sb_ਚੱਕਰੀ_ਜਾਂਚ;
	errcode_t retval;

	io_channel_ਸੈੱਟ_blksize(channel, SUPERBLOCK_ਵਿੱਥ);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_ਮਾਪ, &super);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	retval = io_channel_read_blk64(undo_file, super_ਖੰਡ,
				       -SUPERBLOCK_ਮਾਪ, buf);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;
	sb = (ਬਣਤਰ ext2_super_ਖੰਡ *)buf;
	sb->s_magic = ~sb->s_magic;
	ਜੇ (memcmp(&super, buf, ਆਕਾਰ(super))) {
		retval = -1;
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	sb_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(~0, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)buf, SUPERBLOCK_ਮਾਪ);
	ਜੇ (ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(hdr->sb_ਚੱਕਰੀ_ਜਾਂਚ) != sb_ਚੱਕਰੀ_ਜਾਂਚ) {
		retval = -1;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

ਬਾਹਰਲਾ:
	ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
ਸਥਿਰ errcode_t try_reopen_undo_file(ਪੂਰਨ undo_fd,
				      ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਬਣਤਰ undo_ਸਿਰਲੇਖ hdr;
	ਬਣਤਰ undo_ਕੁੰਜੀ *dkey;
	ext2fs_struct_stat statbuf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ blocksize, fs_blocksize;
	blk64_t super_ਖੰਡ, lblk;
	size_t ਅੰਕ_ਕੁੰਜੀਆਂ, ਕੁੰਜੀਆਂ_ਪ੍ਰਤੀ_ਖੰਡ_2, i;
	__u32 hdr_ਚੱਕਰੀ_ਜਾਂਚ, ਕੁੰਜੀ_ਚੱਕਰੀ_ਜਾਂਚ;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	ਜੇ (retval)
		ਜਾਓ bad_file;
	ਜੇ (statbuf.st_ਮਾਪ == 0)
		ਜਾਓ ਬਾਹਰਲਾ;

	/* check the file header */
	retval = io_channel_read_blk64(ਅੰਕੜੇ->undo_file, 0, -(ਪੂਰਨ)ਆਕਾਰ(hdr),
				       &hdr);
	ਜੇ (retval)
		ਜਾਓ bad_file;

	ਜੇ (memcmp(hdr.magic, E2UNDO_MAGIC,
		    ਆਕਾਰ(hdr.magic)))
		ਜਾਓ bad_file;
	hdr_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(~0, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&hdr,
				   ਆਕਾਰ(ਬਣਤਰ undo_ਸਿਰਲੇਖ) -
				   ਆਕਾਰ(__u32));
	ਜੇ (ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(hdr.ਸਿਰਲੇਖ_ਚੱਕਰੀ_ਜਾਂਚ) != hdr_ਚੱਕਰੀ_ਜਾਂਚ)
		ਜਾਓ bad_file;
	blocksize = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(hdr.ਖੰਡ_ਮਾਪ);
	fs_blocksize = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(hdr.fs_ਖੰਡ_ਮਾਪ);
	ਜੇ (blocksize > E2_UNDO_MAX_ਖੰਡ_ਮਾਪ ||
	    blocksize < E2_UNDO_MIN_ਖੰਡ_ਮਾਪ ||
	    !blocksize || !fs_blocksize)
		ਜਾਓ bad_file;
	super_ਖੰਡ = ext2fs_le64_ਮੰਜ਼ਿਲ_cpu(hdr.super_ਵਿੱਥ);
	ਅੰਕ_ਕੁੰਜੀਆਂ = ext2fs_le64_ਮੰਜ਼ਿਲ_cpu(hdr.ਅੰਕ_ਕੁੰਜੀਆਂ);
	io_channel_ਸੈੱਟ_blksize(ਅੰਕੜੇ->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	ਜੇ (hdr.f_incompat || hdr.f_rocompat)
		ਜਾਓ bad_file;

	/* Superblock matches this FS? */
	ਜੇ (check_filesystem(&hdr, ਅੰਕੜੇ->undo_file, blocksize, super_ਖੰਡ,
			     ਅੰਕੜੇ->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Try to set ourselves up */
	ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ = blocksize;
	retval = undo_ਤਿਆਰੀ_tdb(ਅੰਕੜੇ);
	ਜੇ (retval)
		ਜਾਓ bad_file;
	ਅੰਕੜੇ->ਅੰਕ_ਕੁੰਜੀਆਂ = ਅੰਕ_ਕੁੰਜੀਆਂ;
	ਅੰਕੜੇ->super_blk_ਅੰਕ = super_ਖੰਡ;
	ਅੰਕੜੇ->ਪਹਿਲਾ_ਕੁੰਜੀ_blk = ext2fs_le64_ਮੰਜ਼ਿਲ_cpu(hdr.ਕੁੰਜੀ_ਵਿੱਥ);

	/* load the written block map */
	ਕੁੰਜੀਆਂ_ਪ੍ਰਤੀ_ਖੰਡ_2 = ਕੁੰਜੀਆਂ_ਪ੍ਰਤੀ_ਖੰਡ(ਅੰਕੜੇ);
	lblk = ਅੰਕੜੇ->ਪਹਿਲਾ_ਕੁੰਜੀ_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   ਅੰਕ_ਕੁੰਜੀਆਂ, ਕੁੰਜੀਆਂ_ਪ੍ਰਤੀ_ਖੰਡ_2, blocksize);
	ਲਈ (i = 0; i < ਅੰਕ_ਕੁੰਜੀਆਂ; i += ਕੁੰਜੀਆਂ_ਪ੍ਰਤੀ_ਖੰਡ_2) {
		size_t j, max_j;
		__le32 ਚੱਕਰੀ_ਜਾਂਚ;

		ਅੰਕੜੇ->ਕੁੰਜੀ_blk_ਅੰਕ = lblk;
		retval = io_channel_read_blk64(ਅੰਕੜੇ->undo_file,
					       lblk, 1, ਅੰਕੜੇ->keyb);
		ਜੇ (retval)
			ਜਾਓ bad_ਕੁੰਜੀ_replay;

		/* check keys */
		ਜੇ (ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ਅੰਕੜੇ->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			ਜਾਓ bad_ਕੁੰਜੀ_replay;
		}
		ਚੱਕਰੀ_ਜਾਂਚ = ਅੰਕੜੇ->keyb->ਚੱਕਰੀ_ਜਾਂਚ;
		ਅੰਕੜੇ->keyb->ਚੱਕਰੀ_ਜਾਂਚ = 0;
		ਕੁੰਜੀ_ਚੱਕਰੀ_ਜਾਂਚ = ext2fs_crc32c_le(~0, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)ਅੰਕੜੇ->keyb,
					   blocksize);
		ਜੇ (ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(ਚੱਕਰੀ_ਜਾਂਚ) != ਕੁੰਜੀ_ਚੱਕਰੀ_ਜਾਂਚ) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			ਜਾਓ bad_ਕੁੰਜੀ_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = ਅੰਕੜੇ->ਅੰਕ_ਕੁੰਜੀਆਂ - i;
		ਜੇ (max_j > ਕੁੰਜੀਆਂ_ਪ੍ਰਤੀ_ਖੰਡ_2)
			max_j = ਕੁੰਜੀਆਂ_ਪ੍ਰਤੀ_ਖੰਡ_2;
		ਲਈ (j = 0, dkey = ਅੰਕੜੇ->keyb->ਕੁੰਜੀਆਂ;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_ਮੰਜ਼ਿਲ_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t ਮਾਪ = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(dkey->ਮਾਪ);

			ext2fs_mark_ਖੰਡ_bitmap_range2(ਅੰਕੜੇ->written_ਖੰਡ_map,
					 undo_blk,
					(ਮਾਪ + blocksize - 1) / blocksize);
			lblk += (ਮਾਪ + blocksize - 1) / blocksize;
			ਅੰਕੜੇ->undo_blk_ਅੰਕ = lblk;
			ਅੰਕੜੇ->ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   ਅੰਕੜੇ->ਕੁੰਜੀ_blk_ਅੰਕ, ਅੰਕੜੇ->undo_blk_ਅੰਕ, ਅੰਕੜੇ->ਅੰਕ_ਕੁੰਜੀਆਂ,
		   ਅੰਕੜੇ->ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ);

	ਅੰਕੜੇ->hdr.state = hdr.state & ~E2_UNDO_STATE_ਮੁਕੰਮਲ;
	ਅੰਕੜੇ->hdr.f_compat = hdr.f_compat;
	ਅੰਕੜੇ->hdr.f_incompat = hdr.f_incompat;
	ਅੰਕੜੇ->hdr.f_rocompat = hdr.f_rocompat;
	ਵਾਪਸ retval;

bad_ਕੁੰਜੀ_replay:
	ਅੰਕੜੇ->ਕੁੰਜੀ_blk_ਅੰਕ = ਅੰਕੜੇ->undo_blk_ਅੰਕ = 0;
	ਅੰਕੜੇ->ਕੁੰਜੀਆਂ_ਵਿੱਚ_ਖੰਡ = 0;
	ext2fs_free_mem(&ਅੰਕੜੇ->keyb);
	ext2fs_free_generic_bitmap(ਅੰਕੜੇ->written_ਖੰਡ_map);
	ਅੰਕੜੇ->tdb_written = 0;
	ਜਾਓ ਬਾਹਰਲਾ;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
ਬਾਹਰਲਾ:
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਖਾਲੀ undo_atexit(ਖਾਲੀ *p)
{
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ = p;
	errcode_t err;

	err = write_undo_indexes(ਅੰਕੜੇ, 1);
	io_channel_close(ਅੰਕੜੇ->undo_file);

	com_err(ਅੰਕੜੇ->tdb_file, err, "while force-closing undo file");
}

ਸਥਿਰ errcode_t undo_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, io_channel *channel)
{
	io_channel	io = ਨਿਰਾ;
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ = ਨਿਰਾ;
	ਪੂਰਨ		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_ਧਾਗੇ;
	ਜੇ (name == 0)
		ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ struct_io_channel), &io);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	memset(io, 0, ਆਕਾਰ(ਬਣਤਰ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ), &ਅੰਕੜੇ);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(name)+1, &io->name);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	strcpy(io->name, name);
	io->ਨਿੱਜੀ_ਅੰਕੜੇ = ਅੰਕੜੇ;
	io->ਖੰਡ_ਮਾਪ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(ਅੰਕੜੇ, 0, ਆਕਾਰ(ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ));
	ਅੰਕੜੇ->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	ਅੰਕੜੇ->super_blk_ਅੰਕ = 1;
	ਅੰਕੜੇ->ਪਹਿਲਾ_ਕੁੰਜੀ_blk = 2;
	ਅੰਕੜੇ->undo_blk_ਅੰਕ = 3;

	ਜੇ (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &ਅੰਕੜੇ->real);
		ਜੇ (retval)
			ਜਾਓ cleanup;

		ਅੰਕੜੇ->tdb_file = strdup(tdb_file);
		ਜੇ (ਅੰਕੜੇ->tdb_file == ਨਿਰਾ)
			ਜਾਓ cleanup;
		undo_fd = ext2fs_open_file(ਅੰਕੜੇ->tdb_file, O_RDWR | O_CREAT,
					   0600);
		ਜੇ (undo_fd < 0)
			ਜਾਓ cleanup;

		retval = undo_io_backing_manager->open(ਅੰਕੜੇ->tdb_file,
						       IO_FLAG_RW,
						       &ਅੰਕੜੇ->undo_file);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	} ਨਹੀਂਤਾਂ {
		ਅੰਕੜੇ->real = ਨਿਰਾ;
		ਅੰਕੜੇ->undo_file = ਨਿਰਾ;
	}

	ਜੇ (ਅੰਕੜੇ->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (ਅੰਕੜੇ->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	ਜੇ (ਅੰਕੜੇ->real)
		undo_err_handler_init(ਅੰਕੜੇ->real);

	ਜੇ (ਅੰਕੜੇ->undo_file) {
		retval = try_reopen_undo_file(undo_fd, ਅੰਕੜੇ);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}
	retval = ext2fs_ਜੋੜੋ_exit_fn(undo_atexit, ਅੰਕੜੇ);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	*channel = io;
	ਜੇ (undo_fd >= 0)
		close(undo_fd);
	ਵਾਪਸ retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, ਅੰਕੜੇ);
	ਜੇ (undo_fd >= 0)
		close(undo_fd);
	ਜੇ (ਅੰਕੜੇ && ਅੰਕੜੇ->undo_file)
		io_channel_close(ਅੰਕੜੇ->undo_file);
	ਜੇ (ਅੰਕੜੇ && ਅੰਕੜੇ->tdb_file)
		ਮੁਕਤ(ਅੰਕੜੇ->tdb_file);
	ਜੇ (ਅੰਕੜੇ && ਅੰਕੜੇ->real)
		io_channel_close(ਅੰਕੜੇ->real);
	ਜੇ (ਅੰਕੜੇ)
		ext2fs_free_mem(&ਅੰਕੜੇ);
	ਜੇ (io && io->name)
		ext2fs_free_mem(&io->name);
	ਜੇ (io)
		ext2fs_free_mem(&io);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_close(io_channel channel)
{
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (--channel->refcount > 0)
		ਵਾਪਸ 0;
	/* Before closing write the file system identity */
	ਜੇ (!ext2fs_ਸੁਰੱਖਿਅਤ_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		ਅੰਕੜੇ->hdr.state = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(E2_UNDO_STATE_ਮੁਕੰਮਲ);
	err = write_undo_indexes(ਅੰਕੜੇ, 1);
	ext2fs_remove_exit_fn(undo_atexit, ਅੰਕੜੇ);
	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_close(ਅੰਕੜੇ->real);
	ਜੇ (ਅੰਕੜੇ->tdb_file)
		ਮੁਕਤ(ਅੰਕੜੇ->tdb_file);
	ਜੇ (ਅੰਕੜੇ->undo_file)
		io_channel_close(ਅੰਕੜੇ->undo_file);
	ext2fs_free_mem(&ਅੰਕੜੇ->keyb);
	ਜੇ (ਅੰਕੜੇ->written_ਖੰਡ_map)
		ext2fs_free_generic_bitmap(ਅੰਕੜੇ->written_ਖੰਡ_map);
	ext2fs_free_mem(&channel->ਨਿੱਜੀ_ਅੰਕੜੇ);
	ਜੇ (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	ਜੇ (err)
		ਵਾਪਸ err;
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize)
{
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (blksize > E2_UNDO_MAX_ਖੰਡ_ਮਾਪ || blksize < E2_UNDO_MIN_ਖੰਡ_ਮਾਪ)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_ਸੈੱਟ_blksize(ਅੰਕੜੇ->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	ਜੇ (!ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ || !ਅੰਕੜੇ->tdb_written)
		ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ = blksize;
	channel->ਖੰਡ_ਮਾਪ = blksize;
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_read_blk64(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	errcode_t	retval = 0;
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_read_blk64(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2, buf);

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਵਾਪਸ undo_read_blk64(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf);
}

ਸਥਿਰ errcode_t undo_write_blk64(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, ਖੰਡ, ਗਿਣਤੀ_2);
	ਜੇ (retval)
		 ਵਾਪਸ retval;
	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_write_blk64(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2, buf);

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਵਾਪਸ undo_write_blk64(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf);
}

ਸਥਿਰ errcode_t undo_write_byte(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਥ,
				 ਪੂਰਨ ਮਾਪ, ਅਚਲ ਖਾਲੀ *buf)
{
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ blk_ਅੰਕ, ਗਿਣਤੀ_2;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = ਵਿੱਥ + ਅੰਕੜੇ->ਵਿੱਥ;
	blk_ਅੰਕ = location/channel->ਖੰਡ_ਮਾਪ;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	ਗਿਣਤੀ_2 = (ਮਾਪ + (location % channel->ਖੰਡ_ਮਾਪ) +
			channel->ਖੰਡ_ਮਾਪ  -1)/channel->ਖੰਡ_ਮਾਪ;
	retval = undo_write_tdb(channel, blk_ਅੰਕ, ਗਿਣਤੀ_2);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਜੇ (ਅੰਕੜੇ->real && ਅੰਕੜੇ->real->manager->write_byte)
		retval = io_channel_write_byte(ਅੰਕੜੇ->real, ਵਿੱਥ, ਮਾਪ, buf);

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_discard(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;
	ਪੂਰਨ icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (ਗਿਣਤੀ_2 > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
		ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
	icount = ਗਿਣਤੀ_2;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, ਖੰਡ, icount);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_discard(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2);

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_zeroout(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;
	ਪੂਰਨ icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (ਗਿਣਤੀ_2 > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
		ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
	icount = ਗਿਣਤੀ_2;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, ਖੰਡ, icount);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_zeroout(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2);

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_cache_readahead(io_channel channel,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_cache_readahead(ਅੰਕੜੇ->real, ਖੰਡ, ਗਿਣਤੀ_2);

	ਵਾਪਸ retval;
}

/*
 * Flush data buffers to disk.
 */
ਸਥਿਰ errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = io_channel_flush(ਅੰਕੜੇ->real);

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_ਸੈੱਟ_option(io_channel channel, ਅਚਲ ਅੱਖਰ *option,
				 ਅਚਲ ਅੱਖਰ *arg)
{
	errcode_t	retval = 0;
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ tmp;
	ਅੱਖਰ *ਸਿਰਾ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (!strcmp(option, "tdb_data_size")) {
		ਜੇ (!arg)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

		tmp = strtoul(arg, &ਸਿਰਾ, 0);
		ਜੇ (*ਸਿਰਾ)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
		ਜੇ (tmp > E2_UNDO_MAX_ਖੰਡ_ਮਾਪ || tmp < E2_UNDO_MIN_ਖੰਡ_ਮਾਪ)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
		ਜੇ (!ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ || !ਅੰਕੜੇ->tdb_written) {
			ਅੰਕੜੇ->tdb_written = -1;
			ਅੰਕੜੇ->tdb_ਅੰਕੜੇ_ਮਾਪ = tmp;
		}
		ਵਾਪਸ 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	ਜੇ (ਅੰਕੜੇ->real && ਅੰਕੜੇ->real->manager->ਸੈੱਟ_option) {
		retval = ਅੰਕੜੇ->real->manager->ਸੈੱਟ_option(ਅੰਕੜੇ->real,
							option, arg);
	}
	ਜੇ (!retval && !strcmp(option, "offset")) {
		ਜੇ (!arg)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

		tmp = strtoul(arg, &ਸਿਰਾ, 0);
		ਜੇ (*ਸਿਰਾ)
			ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;
		ਅੰਕੜੇ->ਵਿੱਥ = tmp;
	}
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *ਅੰਕੜੇ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ਅੰਕੜੇ = (ਬਣਤਰ undo_ਨਿੱਜੀ_ਅੰਕੜੇ *) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(ਅੰਕੜੇ, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	ਜੇ (ਅੰਕੜੇ->real)
		retval = (ਅੰਕੜੇ->real->manager->get_stats)(ਅੰਕੜੇ->real, stats);

	ਵਾਪਸ retval;
}

ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_undo_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Undo I/O Manager",
	.open		= undo_open,
	.close		= undo_close,
	.ਸੈੱਟ_blksize	= undo_ਸੈੱਟ_blksize,
	.read_blk	= undo_read_blk,
	.write_blk	= undo_write_blk,
	.flush		= undo_flush,
	.write_byte	= undo_write_byte,
	.ਸੈੱਟ_option	= undo_ਸੈੱਟ_option,
	.get_stats	= undo_get_stats,
	.read_blk64	= undo_read_blk64,
	.write_blk64	= undo_write_blk64,
	.discard	= undo_discard,
	.zeroout	= undo_zeroout,
	.cache_readahead	= undo_cache_readahead,
};

io_manager undo_io_manager = &struct_undo_manager;
