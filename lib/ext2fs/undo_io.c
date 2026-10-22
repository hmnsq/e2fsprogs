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

#جيڪڏهن_وصف_نه_ٿيل LARGEFILE_ذريعو
#وصف_ڪريو LARGEFILE_ذريعو
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل LARGEFILE64_ذريعو
#وصف_ڪريو LARGEFILE64_ذريعو
#جيڪڏهن_پورو

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#جيڪڏهن_وصف_ٿيل __linux__
#ضمن <sys/utsname.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#جيڪڏهن_پورو
#ضمن <limits.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#جيڪڏهن_وصف_ٿيل __GNUC__
#وصف_ڪريو ATTR(x) __attribute__(x)
#وإلا
#وصف_ڪريو ATTR(x)
#جيڪڏهن_پورو

#وصف_رد DEBUG

#جيڪڏهن_وصف_ٿيل DEBUG
# define dbg_printf(f, a...)  افعل {لکو(f, ## a); fflush(stdout); } طالما (0)
#وإلا
# define dbg_printf(f, a...)
#جيڪڏهن_پورو

/*
 * For checking structure magic numbers...
 */

#وصف_ڪريو EXT2_CHECK_MAGIC(بنية, ڪوڊ) \
	  إذا ((بنية)->magic != (ڪوڊ)) أرجع (ڪوڊ)
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
#وصف_ڪريو E2UNDO_MAGIC "E2UNDO02"
#وصف_ڪريو KEYBLOCK_MAGIC 0xCADECADE

#وصف_ڪريو E2_UNDO_STATE_ختم	0x1	/* undo file is complete */

#وصف_ڪريو E2_UNDO_MIN_بلاڪ_سائيز	1024	/* undo blocks are no less than 1KB */
#وصف_ڪريو E2_UNDO_MAX_بلاڪ_سائيز	1048576	/* undo blocks are no more than 1MB */

بنية undo_سرو {
	حرف magic[8];		/* "E2UNDO02" */
	__le64 انگ_ڪنجيون;	/* how many keys? */
	__le64 super_وٿي;	/* where in the file is the superblock copy? */
	__le64 ڪنجي_وٿي;	/* where do the key/data block chunks start? */
	__le32 بلاڪ_سائيز;	/* block size of the undo file */
	__le32 fs_بلاڪ_سائيز;	/* block size of the target device */
	__le32 sb_چڪري_جاچ;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_وٿي;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 سرو_چڪري_جاچ;	/* crc32c of this header (but not this field) */
};

#وصف_ڪريو E2_UNDO_MAX_EXTENT_بلاڪون	512	/* max extent size, in blocks */

بنية undo_ڪنجي {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_چڪري_جاچ;		/* crc32c of the block */
	__le32 سائيز;		/* how many bytes in this block? */
};

بنية undo_ڪنجي_بلاڪ {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 چڪري_جاچ;		/* block checksum */
	__le64 reserved;	/* zero */

#إذا __STDC_VERSION__ >= 199901L
	بنية undo_ڪنجي ڪنجيون[];		/* keys, which come immediately after */
#وإلا
	بنية undo_ڪنجي ڪنجيون[0];	/* keys, which come immediately after */
#جيڪڏهن_پورو
};

بنية undo_ذاتي_معلومات {
	صحيح	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_انگ;			/* next free block */
	blk64_t ڪنجي_blk_انگ;			/* current key block location */
	blk64_t super_blk_انگ;			/* superblock location */
	blk64_t پهريون_ڪنجي_blk;			/* first key block location */
	بنية undo_ڪنجي_بلاڪ *keyb;
	size_t انگ_ڪنجيون, ڪنجيون_اندريون_بلاڪ;

	/* The backing io channel */
	io_channel real;

	بنانشان ڊگهو ڊگهو tdb_معلومات_سائيز;
	صحيح tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t وٿي;

	ext2fs_بلاڪ_bitmap written_بلاڪ_map;
	بنية struct_ext2_filsys fake_fs;
	حرف *tdb_file;
	بنية undo_سرو hdr;
};
#وصف_ڪريو ڪنجيون_في_بلاڪ(d) (((d)->tdb_معلومات_سائيز / حجم(بنية undo_ڪنجي)) - 1)

#وصف_ڪريو E2_UNDO_FEATURE_COMPAT_FS_وٿي 0x1	/* the filesystem offset */

ساكن مضمن فراغ e2undo_سيٽ_feature_fs_وٿي(بنية undo_سرو *سرو) {
	سرو->f_compat |= ext2fs_le32_منزل_cpu(E2_UNDO_FEATURE_COMPAT_FS_وٿي);
}

ساكن مضمن فراغ e2undo_clear_feature_fs_وٿي(بنية undo_سرو *سرو) {
	سرو->f_compat &= ~ext2fs_le32_منزل_cpu(E2_UNDO_FEATURE_COMPAT_FS_وٿي);
}

ساكن io_manager undo_io_backing_manager;
ساكن حرف *tdb_file;
ساكن صحيح actual_سائيز;

errcode_t سيٽ_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	أرجع 0;
}

errcode_t سيٽ_undo_io_backup_file(حرف *file_name)
{
	tdb_file = strdup(file_name);

	إذا (tdb_file == عدم) {
		أرجع EXT2_ET_NO_يادگيري;
	}

	أرجع 0;
}

ساكن errcode_t write_undo_indexes(بنية undo_ذاتي_معلومات *معلومات, صحيح flush)
{
	errcode_t retval;
	بنية ext2_super_بلاڪ super;
	io_channel channel;
	صحيح بلاڪ_سائيز;
	__u32 sb_چڪري_جاچ, hdr_چڪري_جاچ;

	/* Spit out a key block, if there's any data */
	إذا (معلومات->ڪنجيون_اندريون_بلاڪ) {
		معلومات->keyb->magic = ext2fs_cpu_منزل_le32(KEYBLOCK_MAGIC);
		معلومات->keyb->چڪري_جاچ = 0;
		معلومات->keyb->چڪري_جاچ = ext2fs_cpu_منزل_le32(
					 ext2fs_crc32c_le(~0,
					 (بنانشان حرف *)معلومات->keyb,
					 معلومات->tdb_معلومات_سائيز));
		dbg_printf("Writing keyblock to blk %llu\n", معلومات->ڪنجي_blk_انگ);
		retval = io_channel_write_blk64(معلومات->undo_file,
						معلومات->ڪنجي_blk_انگ,
						1, معلومات->keyb);
		إذا (retval)
			أرجع retval;
		/* Move on to the next key block if it's full. */
		إذا (معلومات->ڪنجيون_اندريون_بلاڪ == ڪنجيون_في_بلاڪ(معلومات)) {
			memset(معلومات->keyb, 0, معلومات->tdb_معلومات_سائيز);
			معلومات->ڪنجيون_اندريون_بلاڪ = 0;
			معلومات->ڪنجي_blk_انگ = معلومات->undo_blk_انگ;
			معلومات->undo_blk_انگ++;
		}
	}

	/* Prepare superblock for write */
	channel = معلومات->real;
	بلاڪ_سائيز = channel->بلاڪ_سائيز;

	io_channel_سيٽ_blksize(channel, SUPERBLOCK_وٿي);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_سائيز, &super);
	إذا (retval)
		اذهب err_ٻاهريون;
	sb_چڪري_جاچ = ext2fs_crc32c_le(~0, (بنانشان حرف *)&super, SUPERBLOCK_سائيز);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	يادگيري_نقل(معلومات->hdr.magic, E2UNDO_MAGIC, حجم(معلومات->hdr.magic));
	معلومات->hdr.انگ_ڪنجيون = ext2fs_cpu_منزل_le64(معلومات->انگ_ڪنجيون);
	معلومات->hdr.super_وٿي = ext2fs_cpu_منزل_le64(معلومات->super_blk_انگ);
	معلومات->hdr.ڪنجي_وٿي = ext2fs_cpu_منزل_le64(معلومات->پهريون_ڪنجي_blk);
	معلومات->hdr.fs_بلاڪ_سائيز = ext2fs_cpu_منزل_le32(بلاڪ_سائيز);
	معلومات->hdr.sb_چڪري_جاچ = ext2fs_cpu_منزل_le32(sb_چڪري_جاچ);
	معلومات->hdr.fs_وٿي = ext2fs_cpu_منزل_le64(معلومات->وٿي);
	إذا (معلومات->وٿي)
		e2undo_سيٽ_feature_fs_وٿي(&معلومات->hdr);
	وإلا
		e2undo_clear_feature_fs_وٿي(&معلومات->hdr);
	hdr_چڪري_جاچ = ext2fs_crc32c_le(~0, (بنانشان حرف *)&معلومات->hdr,
				   حجم(معلومات->hdr) -
				   حجم(معلومات->hdr.سرو_چڪري_جاچ));
	معلومات->hdr.سرو_چڪري_جاچ = ext2fs_cpu_منزل_le32(hdr_چڪري_جاچ);
	retval = io_channel_write_blk64(معلومات->undo_file, 0,
					-(صحيح)حجم(معلومات->hdr),
					&معلومات->hdr);
	إذا (retval)
		اذهب err_ٻاهريون;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", معلومات->super_blk_انگ);
	retval = io_channel_write_blk64(معلومات->undo_file, معلومات->super_blk_انگ,
					-SUPERBLOCK_سائيز, &super);
	إذا (retval)
		اذهب err_ٻاهريون;

	إذا (flush)
		retval = io_channel_flush(معلومات->undo_file);
err_ٻاهريون:
	io_channel_سيٽ_blksize(channel, بلاڪ_سائيز);
	أرجع retval;
}

ساكن errcode_t undo_تياري_tdb(بنية undo_ذاتي_معلومات *معلومات)
{
	صحيح i;
	errcode_t retval;

	إذا (معلومات->tdb_written == 1)
		أرجع 0;

	معلومات->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&معلومات->fake_fs, 0, حجم(معلومات->fake_fs));
	معلومات->fake_fs.blocksize = معلومات->tdb_معلومات_سائيز;
	retval = ext2fs_alloc_generic_bmap(&معلومات->fake_fs,
				EXT2_ET_MAGIC_بلاڪ_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &معلومات->written_بلاڪ_map);
	إذا (retval)
		أرجع retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(معلومات->tdb_معلومات_سائيز, &معلومات->keyb);
	إذا (retval)
		أرجع retval;
	معلومات->ڪنجي_blk_انگ = معلومات->پهريون_ڪنجي_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", معلومات->tdb_معلومات_سائيز);
	dbg_printf("Keys per block %llu\n", ڪنجيون_في_بلاڪ(معلومات));
	معلومات->hdr.بلاڪ_سائيز = ext2fs_cpu_منزل_le32(معلومات->tdb_معلومات_سائيز);
	io_channel_سيٽ_blksize(معلومات->undo_file, معلومات->tdb_معلومات_سائيز);

	/* Ensure that we have space for header blocks */
	لكل (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(معلومات->undo_file, i, 1,
					       معلومات->keyb);
		إذا (retval)
			memset(معلومات->keyb, 0, معلومات->tdb_معلومات_سائيز);
		retval = io_channel_write_blk64(معلومات->undo_file, i, 1,
						معلومات->keyb);
		إذا (retval)
			أرجع retval;
		retval = io_channel_flush(معلومات->undo_file);
		إذا (retval)
			أرجع retval;
	}
	memset(معلومات->keyb, 0, معلومات->tdb_معلومات_سائيز);
	أرجع 0;
}

ساكن errcode_t undo_write_tdb(io_channel channel,
				بنانشان ڊگهو ڊگهو بلاڪ, صحيح ڳڻپ)

{
	صحيح سائيز, sz;
	بنانشان ڊگهو ڊگهو بلاڪ_انگ, backing_blk_انگ;
	errcode_t retval = 0;
	ext2_loff_t وٿي;
	بنية undo_ذاتي_معلومات *معلومات;
	بنانشان حرف *read_ptr;
	بنانشان ڊگهو ڊگهو پڇاڙي_بلاڪ;
	بنانشان ڊگهو ڊگهو معلومات_سائيز;
	بنية undo_ڪنجي *ڪنجي;
	__u32 blk_چڪري_جاچ;

	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;

	إذا (معلومات->undo_file == عدم) {
		/*
		 * Transaction database not initialized
		 */
		أرجع 0;
	}

	إذا (ڳڻپ == 1)
		سائيز = channel->بلاڪ_سائيز;
	وإلا {
		إذا (ڳڻپ < 0)
			سائيز = -ڳڻپ;
		وإلا
			سائيز = ڳڻپ * channel->بلاڪ_سائيز;
	}

	retval = undo_تياري_tdb(معلومات);
	إذا (retval)
		أرجع retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	وٿي = (بلاڪ * channel->بلاڪ_سائيز) + معلومات->وٿي ;
	بلاڪ_انگ = وٿي / معلومات->tdb_معلومات_سائيز;
	پڇاڙي_بلاڪ = (وٿي + سائيز - 1) / معلومات->tdb_معلومات_سائيز;

	طالما (بلاڪ_انگ <= پڇاڙي_بلاڪ) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		إذا (ext2fs_test_بلاڪ_bitmap2(معلومات->written_بلاڪ_map,
						   بلاڪ_انگ)) {
			/* Try the next block */
			بلاڪ_انگ++;
			استمر;
		}
		ext2fs_mark_بلاڪ_bitmap2(معلومات->written_بلاڪ_map, بلاڪ_انگ);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		وٿي = بلاڪ_انگ * معلومات->tdb_معلومات_سائيز +
				(معلومات->وٿي % معلومات->tdb_معلومات_سائيز);
		backing_blk_انگ = (وٿي - معلومات->وٿي) / channel->بلاڪ_سائيز;

		retval = ext2fs_get_mem(معلومات->tdb_معلومات_سائيز, &read_ptr);
		إذا (retval) {
			أرجع retval;
		}

		memset(read_ptr, 0, معلومات->tdb_معلومات_سائيز);
		actual_سائيز = 0;
		إذا ((معلومات->tdb_معلومات_سائيز % channel->بلاڪ_سائيز) == 0)
			sz = معلومات->tdb_معلومات_سائيز / channel->بلاڪ_سائيز;
		وإلا
			sz = -معلومات->tdb_معلومات_سائيز;
		retval = io_channel_read_blk64(معلومات->real, backing_blk_انگ,
					     sz, read_ptr);
		إذا (retval) {
			إذا (retval != EXT2_ET_SHORT_READ) {
				حرر(read_ptr);
				أرجع retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			معلومات_سائيز = actual_سائيز;
		} وإلا {
			معلومات_سائيز = معلومات->tdb_معلومات_سائيز;
		}
		إذا (معلومات_سائيز == 0) {
			حرر(read_ptr);
			بلاڪ_انگ++;
			استمر;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       معلومات_سائيز, backing_blk_انگ, بلاڪ, معلومات->tdb_معلومات_سائيز);
		إذا ((معلومات_سائيز % معلومات->undo_file->بلاڪ_سائيز) == 0)
			sz = معلومات_سائيز / معلومات->undo_file->بلاڪ_سائيز;
		وإلا
			sz = -معلومات_سائيز;;
		/* extend this key? */
		إذا (معلومات->ڪنجيون_اندريون_بلاڪ) {
			ڪنجي = معلومات->keyb->ڪنجيون + معلومات->ڪنجيون_اندريون_بلاڪ - 1;
			keysz = ext2fs_le32_منزل_cpu(ڪنجي->سائيز);
		} وإلا {
			ڪنجي = عدم;
			keysz = 0;
		}
		إذا (ڪنجي != عدم &&
		    (ext2fs_le64_منزل_cpu(ڪنجي->fsblk) * channel->بلاڪ_سائيز +
		     channel->بلاڪ_سائيز - 1 +
		     keysz) / channel->بلاڪ_سائيز == backing_blk_انگ &&
		    E2_UNDO_MAX_EXTENT_بلاڪون * معلومات->tdb_معلومات_سائيز >
		    keysz + معلومات_سائيز) {
			blk_چڪري_جاچ = ext2fs_le32_منزل_cpu(ڪنجي->blk_چڪري_جاچ);
			blk_چڪري_جاچ = ext2fs_crc32c_le(blk_چڪري_جاچ, read_ptr, معلومات_سائيز);
			ڪنجي->blk_چڪري_جاچ = ext2fs_cpu_منزل_le32(blk_چڪري_جاچ);
			ڪنجي->سائيز = ext2fs_cpu_منزل_le32(keysz + معلومات_سائيز);
		} وإلا {
			معلومات->انگ_ڪنجيون++;
			ڪنجي = معلومات->keyb->ڪنجيون + معلومات->ڪنجيون_اندريون_بلاڪ;
			معلومات->ڪنجيون_اندريون_بلاڪ++;
			ڪنجي->fsblk = ext2fs_cpu_منزل_le64(backing_blk_انگ);
			blk_چڪري_جاچ = ext2fs_crc32c_le(~0, read_ptr, معلومات_سائيز);
			ڪنجي->blk_چڪري_جاچ = ext2fs_cpu_منزل_le32(blk_چڪري_جاچ);
			ڪنجي->سائيز = ext2fs_cpu_منزل_le32(معلومات_سائيز);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       بلاڪ_انگ,
		       معلومات->undo_blk_انگ,
		       sz, معلومات->انگ_ڪنجيون - 1);
		retval = io_channel_write_blk64(معلومات->undo_file,
					معلومات->undo_blk_انگ, sz, read_ptr);
		إذا (retval) {
			حرر(read_ptr);
			أرجع retval;
		}
		معلومات->undo_blk_انگ++;
		حرر(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(معلومات, 0);
		إذا (retval)
			أرجع retval;

		/* Next block */
		بلاڪ_انگ++;
	}

	أرجع retval;
}

ساكن errcode_t undo_io_read_error(io_channel channel ATTR((اڻ_استعمال_ٿيل)),
				    بنانشان ڊگهو بلاڪ ATTR((اڻ_استعمال_ٿيل)),
				    صحيح ڳڻپ ATTR((اڻ_استعمال_ٿيل)),
				    فراغ *معلومات ATTR((اڻ_استعمال_ٿيل)),
				    size_t سائيز ATTR((اڻ_استعمال_ٿيل)),
				    صحيح actual,
				    errcode_t error ATTR((اڻ_استعمال_ٿيل)))
{
	actual_سائيز = actual;
	أرجع error;
}

ساكن فراغ undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ساكن صحيح check_filesystem(بنية undo_سرو *hdr, io_channel undo_file,
			    بنانشان صحيح blocksize, blk64_t super_بلاڪ,
			    io_channel channel)
{
	بنية ext2_super_بلاڪ super, *sb;
	حرف *buf;
	__u32 sb_چڪري_جاچ;
	errcode_t retval;

	io_channel_سيٽ_blksize(channel, SUPERBLOCK_وٿي);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_سائيز, &super);
	إذا (retval)
		أرجع retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	إذا (retval)
		أرجع retval;
	retval = io_channel_read_blk64(undo_file, super_بلاڪ,
				       -SUPERBLOCK_سائيز, buf);
	إذا (retval)
		اذهب ٻاهريون;
	sb = (بنية ext2_super_بلاڪ *)buf;
	sb->s_magic = ~sb->s_magic;
	إذا (memcmp(&super, buf, حجم(super))) {
		retval = -1;
		اذهب ٻاهريون;
	}
	sb_چڪري_جاچ = ext2fs_crc32c_le(~0, (بنانشان حرف *)buf, SUPERBLOCK_سائيز);
	إذا (ext2fs_le32_منزل_cpu(hdr->sb_چڪري_جاچ) != sb_چڪري_جاچ) {
		retval = -1;
		اذهب ٻاهريون;
	}

ٻاهريون:
	ext2fs_free_mem(&buf);
	أرجع retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
ساكن errcode_t try_reopen_undo_file(صحيح undo_fd,
				      بنية undo_ذاتي_معلومات *معلومات)
{
	بنية undo_سرو hdr;
	بنية undo_ڪنجي *dkey;
	ext2fs_struct_stat statbuf;
	بنانشان صحيح blocksize, fs_blocksize;
	blk64_t super_بلاڪ, lblk;
	size_t انگ_ڪنجيون, ڪنجيون_في_بلاڪ_2, i;
	__u32 hdr_چڪري_جاچ, ڪنجي_چڪري_جاچ;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	إذا (retval)
		اذهب bad_file;
	إذا (statbuf.st_سائيز == 0)
		اذهب ٻاهريون;

	/* check the file header */
	retval = io_channel_read_blk64(معلومات->undo_file, 0, -(صحيح)حجم(hdr),
				       &hdr);
	إذا (retval)
		اذهب bad_file;

	إذا (memcmp(hdr.magic, E2UNDO_MAGIC,
		    حجم(hdr.magic)))
		اذهب bad_file;
	hdr_چڪري_جاچ = ext2fs_crc32c_le(~0, (بنانشان حرف *)&hdr,
				   حجم(بنية undo_سرو) -
				   حجم(__u32));
	إذا (ext2fs_le32_منزل_cpu(hdr.سرو_چڪري_جاچ) != hdr_چڪري_جاچ)
		اذهب bad_file;
	blocksize = ext2fs_le32_منزل_cpu(hdr.بلاڪ_سائيز);
	fs_blocksize = ext2fs_le32_منزل_cpu(hdr.fs_بلاڪ_سائيز);
	إذا (blocksize > E2_UNDO_MAX_بلاڪ_سائيز ||
	    blocksize < E2_UNDO_MIN_بلاڪ_سائيز ||
	    !blocksize || !fs_blocksize)
		اذهب bad_file;
	super_بلاڪ = ext2fs_le64_منزل_cpu(hdr.super_وٿي);
	انگ_ڪنجيون = ext2fs_le64_منزل_cpu(hdr.انگ_ڪنجيون);
	io_channel_سيٽ_blksize(معلومات->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	إذا (hdr.f_incompat || hdr.f_rocompat)
		اذهب bad_file;

	/* Superblock matches this FS? */
	إذا (check_filesystem(&hdr, معلومات->undo_file, blocksize, super_بلاڪ,
			     معلومات->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		اذهب ٻاهريون;
	}

	/* Try to set ourselves up */
	معلومات->tdb_معلومات_سائيز = blocksize;
	retval = undo_تياري_tdb(معلومات);
	إذا (retval)
		اذهب bad_file;
	معلومات->انگ_ڪنجيون = انگ_ڪنجيون;
	معلومات->super_blk_انگ = super_بلاڪ;
	معلومات->پهريون_ڪنجي_blk = ext2fs_le64_منزل_cpu(hdr.ڪنجي_وٿي);

	/* load the written block map */
	ڪنجيون_في_بلاڪ_2 = ڪنجيون_في_بلاڪ(معلومات);
	lblk = معلومات->پهريون_ڪنجي_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   انگ_ڪنجيون, ڪنجيون_في_بلاڪ_2, blocksize);
	لكل (i = 0; i < انگ_ڪنجيون; i += ڪنجيون_في_بلاڪ_2) {
		size_t j, max_j;
		__le32 چڪري_جاچ;

		معلومات->ڪنجي_blk_انگ = lblk;
		retval = io_channel_read_blk64(معلومات->undo_file,
					       lblk, 1, معلومات->keyb);
		إذا (retval)
			اذهب bad_ڪنجي_replay;

		/* check keys */
		إذا (ext2fs_le32_منزل_cpu(معلومات->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			اذهب bad_ڪنجي_replay;
		}
		چڪري_جاچ = معلومات->keyb->چڪري_جاچ;
		معلومات->keyb->چڪري_جاچ = 0;
		ڪنجي_چڪري_جاچ = ext2fs_crc32c_le(~0, (بنانشان حرف *)معلومات->keyb,
					   blocksize);
		إذا (ext2fs_le32_منزل_cpu(چڪري_جاچ) != ڪنجي_چڪري_جاچ) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			اذهب bad_ڪنجي_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = معلومات->انگ_ڪنجيون - i;
		إذا (max_j > ڪنجيون_في_بلاڪ_2)
			max_j = ڪنجيون_في_بلاڪ_2;
		لكل (j = 0, dkey = معلومات->keyb->ڪنجيون;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_منزل_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t سائيز = ext2fs_le32_منزل_cpu(dkey->سائيز);

			ext2fs_mark_بلاڪ_bitmap_range2(معلومات->written_بلاڪ_map,
					 undo_blk,
					(سائيز + blocksize - 1) / blocksize);
			lblk += (سائيز + blocksize - 1) / blocksize;
			معلومات->undo_blk_انگ = lblk;
			معلومات->ڪنجيون_اندريون_بلاڪ = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   معلومات->ڪنجي_blk_انگ, معلومات->undo_blk_انگ, معلومات->انگ_ڪنجيون,
		   معلومات->ڪنجيون_اندريون_بلاڪ);

	معلومات->hdr.state = hdr.state & ~E2_UNDO_STATE_ختم;
	معلومات->hdr.f_compat = hdr.f_compat;
	معلومات->hdr.f_incompat = hdr.f_incompat;
	معلومات->hdr.f_rocompat = hdr.f_rocompat;
	أرجع retval;

bad_ڪنجي_replay:
	معلومات->ڪنجي_blk_انگ = معلومات->undo_blk_انگ = 0;
	معلومات->ڪنجيون_اندريون_بلاڪ = 0;
	ext2fs_free_mem(&معلومات->keyb);
	ext2fs_free_generic_bitmap(معلومات->written_بلاڪ_map);
	معلومات->tdb_written = 0;
	اذهب ٻاهريون;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
ٻاهريون:
	أرجع retval;
}

ساكن فراغ undo_atexit(فراغ *p)
{
	بنية undo_ذاتي_معلومات *معلومات = p;
	errcode_t err;

	err = write_undo_indexes(معلومات, 1);
	io_channel_close(معلومات->undo_file);

	com_err(معلومات->tdb_file, err, "while force-closing undo file");
}

ساكن errcode_t undo_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية undo_ذاتي_معلومات *معلومات = عدم;
	صحيح		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_ڌاڳا;
	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية undo_ذاتي_معلومات), &معلومات);
	إذا (retval)
		اذهب cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(لڙي_ڊيگهه(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->ذاتي_معلومات = معلومات;
	io->بلاڪ_سائيز = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(معلومات, 0, حجم(بنية undo_ذاتي_معلومات));
	معلومات->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	معلومات->super_blk_انگ = 1;
	معلومات->پهريون_ڪنجي_blk = 2;
	معلومات->undo_blk_انگ = 3;

	إذا (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &معلومات->real);
		إذا (retval)
			اذهب cleanup;

		معلومات->tdb_file = strdup(tdb_file);
		إذا (معلومات->tdb_file == عدم)
			اذهب cleanup;
		undo_fd = ext2fs_open_file(معلومات->tdb_file, O_RDWR | O_CREAT,
					   0600);
		إذا (undo_fd < 0)
			اذهب cleanup;

		retval = undo_io_backing_manager->open(معلومات->tdb_file,
						       IO_FLAG_RW,
						       &معلومات->undo_file);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		معلومات->real = عدم;
		معلومات->undo_file = عدم;
	}

	إذا (معلومات->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (معلومات->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	إذا (معلومات->real)
		undo_err_handler_init(معلومات->real);

	إذا (معلومات->undo_file) {
		retval = try_reopen_undo_file(undo_fd, معلومات);
		إذا (retval)
			اذهب cleanup;
	}
	retval = ext2fs_جوڙيو_exit_fn(undo_atexit, معلومات);
	إذا (retval)
		اذهب cleanup;

	*channel = io;
	إذا (undo_fd >= 0)
		close(undo_fd);
	أرجع retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, معلومات);
	إذا (undo_fd >= 0)
		close(undo_fd);
	إذا (معلومات && معلومات->undo_file)
		io_channel_close(معلومات->undo_file);
	إذا (معلومات && معلومات->tdb_file)
		حرر(معلومات->tdb_file);
	إذا (معلومات && معلومات->real)
		io_channel_close(معلومات->real);
	إذا (معلومات)
		ext2fs_free_mem(&معلومات);
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (io)
		ext2fs_free_mem(&io);
	أرجع retval;
}

ساكن errcode_t undo_close(io_channel channel)
{
	بنية undo_ذاتي_معلومات *معلومات;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;
	/* Before closing write the file system identity */
	إذا (!ext2fs_محفوظ_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		معلومات->hdr.state = ext2fs_cpu_منزل_le32(E2_UNDO_STATE_ختم);
	err = write_undo_indexes(معلومات, 1);
	ext2fs_remove_exit_fn(undo_atexit, معلومات);
	إذا (معلومات->real)
		retval = io_channel_close(معلومات->real);
	إذا (معلومات->tdb_file)
		حرر(معلومات->tdb_file);
	إذا (معلومات->undo_file)
		io_channel_close(معلومات->undo_file);
	ext2fs_free_mem(&معلومات->keyb);
	إذا (معلومات->written_بلاڪ_map)
		ext2fs_free_generic_bitmap(معلومات->written_بلاڪ_map);
	ext2fs_free_mem(&channel->ذاتي_معلومات);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	إذا (err)
		أرجع err;
	أرجع retval;
}

ساكن errcode_t undo_سيٽ_blksize(io_channel channel, صحيح blksize)
{
	بنية undo_ذاتي_معلومات *معلومات;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (blksize > E2_UNDO_MAX_بلاڪ_سائيز || blksize < E2_UNDO_MIN_بلاڪ_سائيز)
		أرجع EXT2_ET_INVALID_دليل;

	إذا (معلومات->real)
		retval = io_channel_سيٽ_blksize(معلومات->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	إذا (!معلومات->tdb_معلومات_سائيز || !معلومات->tdb_written)
		معلومات->tdb_معلومات_سائيز = blksize;
	channel->بلاڪ_سائيز = blksize;
	أرجع retval;
}

ساكن errcode_t undo_read_blk64(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			       صحيح ڳڻپ, فراغ *buf)
{
	errcode_t	retval = 0;
	بنية undo_ذاتي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_read_blk64(معلومات->real, بلاڪ, ڳڻپ, buf);

	أرجع retval;
}

ساكن errcode_t undo_read_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
			       صحيح ڳڻپ, فراغ *buf)
{
	أرجع undo_read_blk64(channel, بلاڪ, ڳڻپ, buf);
}

ساكن errcode_t undo_write_blk64(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
				صحيح ڳڻپ, ثابت فراغ *buf)
{
	بنية undo_ذاتي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاڪ, ڳڻپ);
	إذا (retval)
		 أرجع retval;
	إذا (معلومات->real)
		retval = io_channel_write_blk64(معلومات->real, بلاڪ, ڳڻپ, buf);

	أرجع retval;
}

ساكن errcode_t undo_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
				صحيح ڳڻپ, ثابت فراغ *buf)
{
	أرجع undo_write_blk64(channel, بلاڪ, ڳڻپ, buf);
}

ساكن errcode_t undo_write_byte(io_channel channel, بنانشان ڊگهو وٿي,
				 صحيح سائيز, ثابت فراغ *buf)
{
	بنية undo_ذاتي_معلومات *معلومات;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	بنانشان ڊگهو blk_انگ, ڳڻپ;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = وٿي + معلومات->وٿي;
	blk_انگ = location/channel->بلاڪ_سائيز;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	ڳڻپ = (سائيز + (location % channel->بلاڪ_سائيز) +
			channel->بلاڪ_سائيز  -1)/channel->بلاڪ_سائيز;
	retval = undo_write_tdb(channel, blk_انگ, ڳڻپ);
	إذا (retval)
		أرجع retval;
	إذا (معلومات->real && معلومات->real->manager->write_byte)
		retval = io_channel_write_byte(معلومات->real, وٿي, سائيز, buf);

	أرجع retval;
}

ساكن errcode_t undo_discard(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			      بنانشان ڊگهو ڊگهو ڳڻپ)
{
	بنية undo_ذاتي_معلومات *معلومات;
	errcode_t	retval = 0;
	صحيح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (ڳڻپ > وڌ_کان_وڌ_عدد)
		أرجع EXT2_ET_UNIMPLEMENTED;
	icount = ڳڻپ;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاڪ, icount);
	إذا (retval)
		أرجع retval;
	إذا (معلومات->real)
		retval = io_channel_discard(معلومات->real, بلاڪ, ڳڻپ);

	أرجع retval;
}

ساكن errcode_t undo_zeroout(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			      بنانشان ڊگهو ڊگهو ڳڻپ)
{
	بنية undo_ذاتي_معلومات *معلومات;
	errcode_t	retval = 0;
	صحيح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (ڳڻپ > وڌ_کان_وڌ_عدد)
		أرجع EXT2_ET_UNIMPLEMENTED;
	icount = ڳڻپ;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاڪ, icount);
	إذا (retval)
		أرجع retval;
	إذا (معلومات->real)
		retval = io_channel_zeroout(معلومات->real, بلاڪ, ڳڻپ);

	أرجع retval;
}

ساكن errcode_t undo_cache_readahead(io_channel channel,
				      بنانشان ڊگهو ڊگهو بلاڪ,
				      بنانشان ڊگهو ڊگهو ڳڻپ)
{
	بنية undo_ذاتي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_cache_readahead(معلومات->real, بلاڪ, ڳڻپ);

	أرجع retval;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	بنية undo_ذاتي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_flush(معلومات->real);

	أرجع retval;
}

ساكن errcode_t undo_سيٽ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	errcode_t	retval = 0;
	بنية undo_ذاتي_معلومات *معلومات;
	بنانشان ڊگهو tmp;
	حرف *پڇاڙي;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!strcmp(option, "tdb_data_size")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دليل;

		tmp = strtoul(arg, &پڇاڙي, 0);
		إذا (*پڇاڙي)
			أرجع EXT2_ET_INVALID_دليل;
		إذا (tmp > E2_UNDO_MAX_بلاڪ_سائيز || tmp < E2_UNDO_MIN_بلاڪ_سائيز)
			أرجع EXT2_ET_INVALID_دليل;
		إذا (!معلومات->tdb_معلومات_سائيز || !معلومات->tdb_written) {
			معلومات->tdb_written = -1;
			معلومات->tdb_معلومات_سائيز = tmp;
		}
		أرجع 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	إذا (معلومات->real && معلومات->real->manager->سيٽ_option) {
		retval = معلومات->real->manager->سيٽ_option(معلومات->real,
							option, arg);
	}
	إذا (!retval && !strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دليل;

		tmp = strtoul(arg, &پڇاڙي, 0);
		إذا (*پڇاڙي)
			أرجع EXT2_ET_INVALID_دليل;
		معلومات->وٿي = tmp;
	}
	أرجع retval;
}

ساكن errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	بنية undo_ذاتي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_ذاتي_معلومات *) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (معلومات->real)
		retval = (معلومات->real->manager->get_stats)(معلومات->real, stats);

	أرجع retval;
}

ساكن بنية struct_io_manager struct_undo_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Undo I/O Manager",
	.open		= undo_open,
	.close		= undo_close,
	.سيٽ_blksize	= undo_سيٽ_blksize,
	.read_blk	= undo_read_blk,
	.write_blk	= undo_write_blk,
	.flush		= undo_flush,
	.write_byte	= undo_write_byte,
	.سيٽ_option	= undo_سيٽ_option,
	.get_stats	= undo_get_stats,
	.read_blk64	= undo_read_blk64,
	.write_blk64	= undo_write_blk64,
	.discard	= undo_discard,
	.zeroout	= undo_zeroout,
	.cache_readahead	= undo_cache_readahead,
};

io_manager undo_io_manager = &struct_undo_manager;
