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

#که_نه_تعریف_شوی LARGEFILE_سرچینه
#تعریف_کړه LARGEFILE_سرچینه
#د_که_پای
#که_نه_تعریف_شوی LARGEFILE64_سرچینه
#تعریف_کړه LARGEFILE64_سرچینه
#د_که_پای

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#که_تعریف_شوی __linux__
#ضمن <sys/utsname.h>
#د_که_پای
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای
#إذا HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#د_که_پای
#ضمن <limits.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#که_تعریف_شوی __GNUC__
#تعریف_کړه ATTR(x) __attribute__(x)
#وإلا
#تعریف_کړه ATTR(x)
#د_که_پای

#تعریف_لغوه DEBUG

#که_تعریف_شوی DEBUG
# define dbg_printf(f, a...)  افعل {چاپ_کړه(f, ## a); fflush(stdout); } طالما (0)
#وإلا
# define dbg_printf(f, a...)
#د_که_پای

/*
 * For checking structure magic numbers...
 */

#تعریف_کړه EXT2_CHECK_MAGIC(بنية, کوډ) \
	  إذا ((بنية)->magic != (کوډ)) أرجع (کوډ)
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
#تعریف_کړه E2UNDO_MAGIC "E2UNDO02"
#تعریف_کړه KEYBLOCK_MAGIC 0xCADECADE

#تعریف_کړه E2_UNDO_STATE_پای_ته_رسېدلی	0x1	/* undo file is complete */

#تعریف_کړه E2_UNDO_MIN_بلاک_اندازه	1024	/* undo blocks are no less than 1KB */
#تعریف_کړه E2_UNDO_MAX_بلاک_اندازه	1048576	/* undo blocks are no more than 1MB */

بنية undo_سرلیک {
	حرف magic[8];		/* "E2UNDO02" */
	__le64 عدد_کلیګانې;	/* how many keys? */
	__le64 super_دننه_واټن;	/* where in the file is the superblock copy? */
	__le64 کلی_دننه_واټن;	/* where do the key/data block chunks start? */
	__le32 بلاک_اندازه;	/* block size of the undo file */
	__le32 fs_بلاک_اندازه;	/* block size of the target device */
	__le32 sb_دوراني_ازموینه;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_دننه_واټن;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 سرلیک_دوراني_ازموینه;	/* crc32c of this header (but not this field) */
};

#تعریف_کړه E2_UNDO_MAX_EXTENT_بلاکونه	512	/* max extent size, in blocks */

بنية undo_کلی {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_دوراني_ازموینه;		/* crc32c of the block */
	__le32 اندازه;		/* how many bytes in this block? */
};

بنية undo_کلی_بلاک {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 دوراني_ازموینه;		/* block checksum */
	__le64 reserved;	/* zero */

#إذا __STDC_VERSION__ >= 199901L
	بنية undo_کلی کلیګانې[];		/* keys, which come immediately after */
#وإلا
	بنية undo_کلی کلیګانې[0];	/* keys, which come immediately after */
#د_که_پای
};

بنية undo_شخصي_معلومات {
	صحيح	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_عدد;			/* next free block */
	blk64_t کلی_blk_عدد;			/* current key block location */
	blk64_t super_blk_عدد;			/* superblock location */
	blk64_t لومړی_کلی_blk;			/* first key block location */
	بنية undo_کلی_بلاک *keyb;
	size_t عدد_کلیګانې, کلیګانې_دننه_بلاک;

	/* The backing io channel */
	io_channel real;

	بینښه اوږد اوږد tdb_معلومات_اندازه;
	صحيح tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t دننه_واټن;

	ext2fs_بلاک_bitmap written_بلاک_map;
	بنية struct_ext2_filsys fake_fs;
	حرف *tdb_file;
	بنية undo_سرلیک hdr;
};
#تعریف_کړه کلیګانې_فی_بلاک(d) (((d)->tdb_معلومات_اندازه / حجم(بنية undo_کلی)) - 1)

#تعریف_کړه E2_UNDO_FEATURE_COMPAT_FS_دننه_واټن 0x1	/* the filesystem offset */

ساكن مضمن فراغ e2undo_سېټ_feature_fs_دننه_واټن(بنية undo_سرلیک *سرلیک) {
	سرلیک->f_compat |= ext2fs_le32_منزل_cpu(E2_UNDO_FEATURE_COMPAT_FS_دننه_واټن);
}

ساكن مضمن فراغ e2undo_clear_feature_fs_دننه_واټن(بنية undo_سرلیک *سرلیک) {
	سرلیک->f_compat &= ~ext2fs_le32_منزل_cpu(E2_UNDO_FEATURE_COMPAT_FS_دننه_واټن);
}

ساكن io_manager undo_io_backing_manager;
ساكن حرف *tdb_file;
ساكن صحيح actual_اندازه;

errcode_t سېټ_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	أرجع 0;
}

errcode_t سېټ_undo_io_backup_file(حرف *file_name)
{
	tdb_file = strdup(file_name);

	إذا (tdb_file == عدم) {
		أرجع EXT2_ET_NO_حافظه;
	}

	أرجع 0;
}

ساكن errcode_t write_undo_indexes(بنية undo_شخصي_معلومات *معلومات, صحيح flush)
{
	errcode_t retval;
	بنية ext2_super_بلاک super;
	io_channel channel;
	صحيح بلاک_اندازه;
	__u32 sb_دوراني_ازموینه, hdr_دوراني_ازموینه;

	/* Spit out a key block, if there's any data */
	إذا (معلومات->کلیګانې_دننه_بلاک) {
		معلومات->keyb->magic = ext2fs_cpu_منزل_le32(KEYBLOCK_MAGIC);
		معلومات->keyb->دوراني_ازموینه = 0;
		معلومات->keyb->دوراني_ازموینه = ext2fs_cpu_منزل_le32(
					 ext2fs_crc32c_le(~0,
					 (بینښه حرف *)معلومات->keyb,
					 معلومات->tdb_معلومات_اندازه));
		dbg_printf("Writing keyblock to blk %llu\n", معلومات->کلی_blk_عدد);
		retval = io_channel_write_blk64(معلومات->undo_file,
						معلومات->کلی_blk_عدد,
						1, معلومات->keyb);
		إذا (retval)
			أرجع retval;
		/* Move on to the next key block if it's full. */
		إذا (معلومات->کلیګانې_دننه_بلاک == کلیګانې_فی_بلاک(معلومات)) {
			memset(معلومات->keyb, 0, معلومات->tdb_معلومات_اندازه);
			معلومات->کلیګانې_دننه_بلاک = 0;
			معلومات->کلی_blk_عدد = معلومات->undo_blk_عدد;
			معلومات->undo_blk_عدد++;
		}
	}

	/* Prepare superblock for write */
	channel = معلومات->real;
	بلاک_اندازه = channel->بلاک_اندازه;

	io_channel_سېټ_blksize(channel, SUPERBLOCK_دننه_واټن);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_اندازه, &super);
	إذا (retval)
		اذهب err_بهر;
	sb_دوراني_ازموینه = ext2fs_crc32c_le(~0, (بینښه حرف *)&super, SUPERBLOCK_اندازه);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	حافظه_نقل(معلومات->hdr.magic, E2UNDO_MAGIC, حجم(معلومات->hdr.magic));
	معلومات->hdr.عدد_کلیګانې = ext2fs_cpu_منزل_le64(معلومات->عدد_کلیګانې);
	معلومات->hdr.super_دننه_واټن = ext2fs_cpu_منزل_le64(معلومات->super_blk_عدد);
	معلومات->hdr.کلی_دننه_واټن = ext2fs_cpu_منزل_le64(معلومات->لومړی_کلی_blk);
	معلومات->hdr.fs_بلاک_اندازه = ext2fs_cpu_منزل_le32(بلاک_اندازه);
	معلومات->hdr.sb_دوراني_ازموینه = ext2fs_cpu_منزل_le32(sb_دوراني_ازموینه);
	معلومات->hdr.fs_دننه_واټن = ext2fs_cpu_منزل_le64(معلومات->دننه_واټن);
	إذا (معلومات->دننه_واټن)
		e2undo_سېټ_feature_fs_دننه_واټن(&معلومات->hdr);
	وإلا
		e2undo_clear_feature_fs_دننه_واټن(&معلومات->hdr);
	hdr_دوراني_ازموینه = ext2fs_crc32c_le(~0, (بینښه حرف *)&معلومات->hdr,
				   حجم(معلومات->hdr) -
				   حجم(معلومات->hdr.سرلیک_دوراني_ازموینه));
	معلومات->hdr.سرلیک_دوراني_ازموینه = ext2fs_cpu_منزل_le32(hdr_دوراني_ازموینه);
	retval = io_channel_write_blk64(معلومات->undo_file, 0,
					-(صحيح)حجم(معلومات->hdr),
					&معلومات->hdr);
	إذا (retval)
		اذهب err_بهر;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", معلومات->super_blk_عدد);
	retval = io_channel_write_blk64(معلومات->undo_file, معلومات->super_blk_عدد,
					-SUPERBLOCK_اندازه, &super);
	إذا (retval)
		اذهب err_بهر;

	إذا (flush)
		retval = io_channel_flush(معلومات->undo_file);
err_بهر:
	io_channel_سېټ_blksize(channel, بلاک_اندازه);
	أرجع retval;
}

ساكن errcode_t undo_تیاری_tdb(بنية undo_شخصي_معلومات *معلومات)
{
	صحيح i;
	errcode_t retval;

	إذا (معلومات->tdb_written == 1)
		أرجع 0;

	معلومات->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&معلومات->fake_fs, 0, حجم(معلومات->fake_fs));
	معلومات->fake_fs.blocksize = معلومات->tdb_معلومات_اندازه;
	retval = ext2fs_alloc_generic_bmap(&معلومات->fake_fs,
				EXT2_ET_MAGIC_بلاک_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &معلومات->written_بلاک_map);
	إذا (retval)
		أرجع retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(معلومات->tdb_معلومات_اندازه, &معلومات->keyb);
	إذا (retval)
		أرجع retval;
	معلومات->کلی_blk_عدد = معلومات->لومړی_کلی_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", معلومات->tdb_معلومات_اندازه);
	dbg_printf("Keys per block %llu\n", کلیګانې_فی_بلاک(معلومات));
	معلومات->hdr.بلاک_اندازه = ext2fs_cpu_منزل_le32(معلومات->tdb_معلومات_اندازه);
	io_channel_سېټ_blksize(معلومات->undo_file, معلومات->tdb_معلومات_اندازه);

	/* Ensure that we have space for header blocks */
	لكل (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(معلومات->undo_file, i, 1,
					       معلومات->keyb);
		إذا (retval)
			memset(معلومات->keyb, 0, معلومات->tdb_معلومات_اندازه);
		retval = io_channel_write_blk64(معلومات->undo_file, i, 1,
						معلومات->keyb);
		إذا (retval)
			أرجع retval;
		retval = io_channel_flush(معلومات->undo_file);
		إذا (retval)
			أرجع retval;
	}
	memset(معلومات->keyb, 0, معلومات->tdb_معلومات_اندازه);
	أرجع 0;
}

ساكن errcode_t undo_write_tdb(io_channel channel,
				بینښه اوږد اوږد بلاک, صحيح شمېر)

{
	صحيح اندازه, sz;
	بینښه اوږد اوږد بلاک_عدد, backing_blk_عدد;
	errcode_t retval = 0;
	ext2_loff_t دننه_واټن;
	بنية undo_شخصي_معلومات *معلومات;
	بینښه حرف *read_ptr;
	بینښه اوږد اوږد پای_ټکی_بلاک;
	بینښه اوږد اوږد معلومات_اندازه;
	بنية undo_کلی *کلی;
	__u32 blk_دوراني_ازموینه;

	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;

	إذا (معلومات->undo_file == عدم) {
		/*
		 * Transaction database not initialized
		 */
		أرجع 0;
	}

	إذا (شمېر == 1)
		اندازه = channel->بلاک_اندازه;
	وإلا {
		إذا (شمېر < 0)
			اندازه = -شمېر;
		وإلا
			اندازه = شمېر * channel->بلاک_اندازه;
	}

	retval = undo_تیاری_tdb(معلومات);
	إذا (retval)
		أرجع retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	دننه_واټن = (بلاک * channel->بلاک_اندازه) + معلومات->دننه_واټن ;
	بلاک_عدد = دننه_واټن / معلومات->tdb_معلومات_اندازه;
	پای_ټکی_بلاک = (دننه_واټن + اندازه - 1) / معلومات->tdb_معلومات_اندازه;

	طالما (بلاک_عدد <= پای_ټکی_بلاک) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		إذا (ext2fs_test_بلاک_bitmap2(معلومات->written_بلاک_map,
						   بلاک_عدد)) {
			/* Try the next block */
			بلاک_عدد++;
			استمر;
		}
		ext2fs_mark_بلاک_bitmap2(معلومات->written_بلاک_map, بلاک_عدد);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		دننه_واټن = بلاک_عدد * معلومات->tdb_معلومات_اندازه +
				(معلومات->دننه_واټن % معلومات->tdb_معلومات_اندازه);
		backing_blk_عدد = (دننه_واټن - معلومات->دننه_واټن) / channel->بلاک_اندازه;

		retval = ext2fs_get_mem(معلومات->tdb_معلومات_اندازه, &read_ptr);
		إذا (retval) {
			أرجع retval;
		}

		memset(read_ptr, 0, معلومات->tdb_معلومات_اندازه);
		actual_اندازه = 0;
		إذا ((معلومات->tdb_معلومات_اندازه % channel->بلاک_اندازه) == 0)
			sz = معلومات->tdb_معلومات_اندازه / channel->بلاک_اندازه;
		وإلا
			sz = -معلومات->tdb_معلومات_اندازه;
		retval = io_channel_read_blk64(معلومات->real, backing_blk_عدد,
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
			معلومات_اندازه = actual_اندازه;
		} وإلا {
			معلومات_اندازه = معلومات->tdb_معلومات_اندازه;
		}
		إذا (معلومات_اندازه == 0) {
			حرر(read_ptr);
			بلاک_عدد++;
			استمر;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       معلومات_اندازه, backing_blk_عدد, بلاک, معلومات->tdb_معلومات_اندازه);
		إذا ((معلومات_اندازه % معلومات->undo_file->بلاک_اندازه) == 0)
			sz = معلومات_اندازه / معلومات->undo_file->بلاک_اندازه;
		وإلا
			sz = -معلومات_اندازه;;
		/* extend this key? */
		إذا (معلومات->کلیګانې_دننه_بلاک) {
			کلی = معلومات->keyb->کلیګانې + معلومات->کلیګانې_دننه_بلاک - 1;
			keysz = ext2fs_le32_منزل_cpu(کلی->اندازه);
		} وإلا {
			کلی = عدم;
			keysz = 0;
		}
		إذا (کلی != عدم &&
		    (ext2fs_le64_منزل_cpu(کلی->fsblk) * channel->بلاک_اندازه +
		     channel->بلاک_اندازه - 1 +
		     keysz) / channel->بلاک_اندازه == backing_blk_عدد &&
		    E2_UNDO_MAX_EXTENT_بلاکونه * معلومات->tdb_معلومات_اندازه >
		    keysz + معلومات_اندازه) {
			blk_دوراني_ازموینه = ext2fs_le32_منزل_cpu(کلی->blk_دوراني_ازموینه);
			blk_دوراني_ازموینه = ext2fs_crc32c_le(blk_دوراني_ازموینه, read_ptr, معلومات_اندازه);
			کلی->blk_دوراني_ازموینه = ext2fs_cpu_منزل_le32(blk_دوراني_ازموینه);
			کلی->اندازه = ext2fs_cpu_منزل_le32(keysz + معلومات_اندازه);
		} وإلا {
			معلومات->عدد_کلیګانې++;
			کلی = معلومات->keyb->کلیګانې + معلومات->کلیګانې_دننه_بلاک;
			معلومات->کلیګانې_دننه_بلاک++;
			کلی->fsblk = ext2fs_cpu_منزل_le64(backing_blk_عدد);
			blk_دوراني_ازموینه = ext2fs_crc32c_le(~0, read_ptr, معلومات_اندازه);
			کلی->blk_دوراني_ازموینه = ext2fs_cpu_منزل_le32(blk_دوراني_ازموینه);
			کلی->اندازه = ext2fs_cpu_منزل_le32(معلومات_اندازه);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       بلاک_عدد,
		       معلومات->undo_blk_عدد,
		       sz, معلومات->عدد_کلیګانې - 1);
		retval = io_channel_write_blk64(معلومات->undo_file,
					معلومات->undo_blk_عدد, sz, read_ptr);
		إذا (retval) {
			حرر(read_ptr);
			أرجع retval;
		}
		معلومات->undo_blk_عدد++;
		حرر(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(معلومات, 0);
		إذا (retval)
			أرجع retval;

		/* Next block */
		بلاک_عدد++;
	}

	أرجع retval;
}

ساكن errcode_t undo_io_read_error(io_channel channel ATTR((ناکارول_شوی)),
				    بینښه اوږد بلاک ATTR((ناکارول_شوی)),
				    صحيح شمېر ATTR((ناکارول_شوی)),
				    فراغ *معلومات ATTR((ناکارول_شوی)),
				    size_t اندازه ATTR((ناکارول_شوی)),
				    صحيح actual,
				    errcode_t error ATTR((ناکارول_شوی)))
{
	actual_اندازه = actual;
	أرجع error;
}

ساكن فراغ undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ساكن صحيح check_filesystem(بنية undo_سرلیک *hdr, io_channel undo_file,
			    بینښه صحيح blocksize, blk64_t super_بلاک,
			    io_channel channel)
{
	بنية ext2_super_بلاک super, *sb;
	حرف *buf;
	__u32 sb_دوراني_ازموینه;
	errcode_t retval;

	io_channel_سېټ_blksize(channel, SUPERBLOCK_دننه_واټن);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_اندازه, &super);
	إذا (retval)
		أرجع retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	إذا (retval)
		أرجع retval;
	retval = io_channel_read_blk64(undo_file, super_بلاک,
				       -SUPERBLOCK_اندازه, buf);
	إذا (retval)
		اذهب بهر;
	sb = (بنية ext2_super_بلاک *)buf;
	sb->s_magic = ~sb->s_magic;
	إذا (memcmp(&super, buf, حجم(super))) {
		retval = -1;
		اذهب بهر;
	}
	sb_دوراني_ازموینه = ext2fs_crc32c_le(~0, (بینښه حرف *)buf, SUPERBLOCK_اندازه);
	إذا (ext2fs_le32_منزل_cpu(hdr->sb_دوراني_ازموینه) != sb_دوراني_ازموینه) {
		retval = -1;
		اذهب بهر;
	}

بهر:
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
				      بنية undo_شخصي_معلومات *معلومات)
{
	بنية undo_سرلیک hdr;
	بنية undo_کلی *dkey;
	ext2fs_struct_stat statbuf;
	بینښه صحيح blocksize, fs_blocksize;
	blk64_t super_بلاک, lblk;
	size_t عدد_کلیګانې, کلیګانې_فی_بلاک_2, i;
	__u32 hdr_دوراني_ازموینه, کلی_دوراني_ازموینه;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	إذا (retval)
		اذهب bad_file;
	إذا (statbuf.st_اندازه == 0)
		اذهب بهر;

	/* check the file header */
	retval = io_channel_read_blk64(معلومات->undo_file, 0, -(صحيح)حجم(hdr),
				       &hdr);
	إذا (retval)
		اذهب bad_file;

	إذا (memcmp(hdr.magic, E2UNDO_MAGIC,
		    حجم(hdr.magic)))
		اذهب bad_file;
	hdr_دوراني_ازموینه = ext2fs_crc32c_le(~0, (بینښه حرف *)&hdr,
				   حجم(بنية undo_سرلیک) -
				   حجم(__u32));
	إذا (ext2fs_le32_منزل_cpu(hdr.سرلیک_دوراني_ازموینه) != hdr_دوراني_ازموینه)
		اذهب bad_file;
	blocksize = ext2fs_le32_منزل_cpu(hdr.بلاک_اندازه);
	fs_blocksize = ext2fs_le32_منزل_cpu(hdr.fs_بلاک_اندازه);
	إذا (blocksize > E2_UNDO_MAX_بلاک_اندازه ||
	    blocksize < E2_UNDO_MIN_بلاک_اندازه ||
	    !blocksize || !fs_blocksize)
		اذهب bad_file;
	super_بلاک = ext2fs_le64_منزل_cpu(hdr.super_دننه_واټن);
	عدد_کلیګانې = ext2fs_le64_منزل_cpu(hdr.عدد_کلیګانې);
	io_channel_سېټ_blksize(معلومات->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	إذا (hdr.f_incompat || hdr.f_rocompat)
		اذهب bad_file;

	/* Superblock matches this FS? */
	إذا (check_filesystem(&hdr, معلومات->undo_file, blocksize, super_بلاک,
			     معلومات->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		اذهب بهر;
	}

	/* Try to set ourselves up */
	معلومات->tdb_معلومات_اندازه = blocksize;
	retval = undo_تیاری_tdb(معلومات);
	إذا (retval)
		اذهب bad_file;
	معلومات->عدد_کلیګانې = عدد_کلیګانې;
	معلومات->super_blk_عدد = super_بلاک;
	معلومات->لومړی_کلی_blk = ext2fs_le64_منزل_cpu(hdr.کلی_دننه_واټن);

	/* load the written block map */
	کلیګانې_فی_بلاک_2 = کلیګانې_فی_بلاک(معلومات);
	lblk = معلومات->لومړی_کلی_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   عدد_کلیګانې, کلیګانې_فی_بلاک_2, blocksize);
	لكل (i = 0; i < عدد_کلیګانې; i += کلیګانې_فی_بلاک_2) {
		size_t j, max_j;
		__le32 دوراني_ازموینه;

		معلومات->کلی_blk_عدد = lblk;
		retval = io_channel_read_blk64(معلومات->undo_file,
					       lblk, 1, معلومات->keyb);
		إذا (retval)
			اذهب bad_کلی_replay;

		/* check keys */
		إذا (ext2fs_le32_منزل_cpu(معلومات->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			اذهب bad_کلی_replay;
		}
		دوراني_ازموینه = معلومات->keyb->دوراني_ازموینه;
		معلومات->keyb->دوراني_ازموینه = 0;
		کلی_دوراني_ازموینه = ext2fs_crc32c_le(~0, (بینښه حرف *)معلومات->keyb,
					   blocksize);
		إذا (ext2fs_le32_منزل_cpu(دوراني_ازموینه) != کلی_دوراني_ازموینه) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			اذهب bad_کلی_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = معلومات->عدد_کلیګانې - i;
		إذا (max_j > کلیګانې_فی_بلاک_2)
			max_j = کلیګانې_فی_بلاک_2;
		لكل (j = 0, dkey = معلومات->keyb->کلیګانې;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_منزل_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t اندازه = ext2fs_le32_منزل_cpu(dkey->اندازه);

			ext2fs_mark_بلاک_bitmap_range2(معلومات->written_بلاک_map,
					 undo_blk,
					(اندازه + blocksize - 1) / blocksize);
			lblk += (اندازه + blocksize - 1) / blocksize;
			معلومات->undo_blk_عدد = lblk;
			معلومات->کلیګانې_دننه_بلاک = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   معلومات->کلی_blk_عدد, معلومات->undo_blk_عدد, معلومات->عدد_کلیګانې,
		   معلومات->کلیګانې_دننه_بلاک);

	معلومات->hdr.state = hdr.state & ~E2_UNDO_STATE_پای_ته_رسېدلی;
	معلومات->hdr.f_compat = hdr.f_compat;
	معلومات->hdr.f_incompat = hdr.f_incompat;
	معلومات->hdr.f_rocompat = hdr.f_rocompat;
	أرجع retval;

bad_کلی_replay:
	معلومات->کلی_blk_عدد = معلومات->undo_blk_عدد = 0;
	معلومات->کلیګانې_دننه_بلاک = 0;
	ext2fs_free_mem(&معلومات->keyb);
	ext2fs_free_generic_bitmap(معلومات->written_بلاک_map);
	معلومات->tdb_written = 0;
	اذهب بهر;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
بهر:
	أرجع retval;
}

ساكن فراغ undo_atexit(فراغ *p)
{
	بنية undo_شخصي_معلومات *معلومات = p;
	errcode_t err;

	err = write_undo_indexes(معلومات, 1);
	io_channel_close(معلومات->undo_file);

	com_err(معلومات->tdb_file, err, "while force-closing undo file");
}

ساكن errcode_t undo_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية undo_شخصي_معلومات *معلومات = عدم;
	صحيح		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_تارونه;
	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية undo_شخصي_معلومات), &معلومات);
	إذا (retval)
		اذهب cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(مزي_اوږدوالی(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->شخصي_معلومات = معلومات;
	io->بلاک_اندازه = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(معلومات, 0, حجم(بنية undo_شخصي_معلومات));
	معلومات->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	معلومات->super_blk_عدد = 1;
	معلومات->لومړی_کلی_blk = 2;
	معلومات->undo_blk_عدد = 3;

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
	retval = ext2fs_جمع_کړه_exit_fn(undo_atexit, معلومات);
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
	بنية undo_شخصي_معلومات *معلومات;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;
	/* Before closing write the file system identity */
	إذا (!ext2fs_خوندي_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		معلومات->hdr.state = ext2fs_cpu_منزل_le32(E2_UNDO_STATE_پای_ته_رسېدلی);
	err = write_undo_indexes(معلومات, 1);
	ext2fs_remove_exit_fn(undo_atexit, معلومات);
	إذا (معلومات->real)
		retval = io_channel_close(معلومات->real);
	إذا (معلومات->tdb_file)
		حرر(معلومات->tdb_file);
	إذا (معلومات->undo_file)
		io_channel_close(معلومات->undo_file);
	ext2fs_free_mem(&معلومات->keyb);
	إذا (معلومات->written_بلاک_map)
		ext2fs_free_generic_bitmap(معلومات->written_بلاک_map);
	ext2fs_free_mem(&channel->شخصي_معلومات);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	إذا (err)
		أرجع err;
	أرجع retval;
}

ساكن errcode_t undo_سېټ_blksize(io_channel channel, صحيح blksize)
{
	بنية undo_شخصي_معلومات *معلومات;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (blksize > E2_UNDO_MAX_بلاک_اندازه || blksize < E2_UNDO_MIN_بلاک_اندازه)
		أرجع EXT2_ET_INVALID_دلیل;

	إذا (معلومات->real)
		retval = io_channel_سېټ_blksize(معلومات->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	إذا (!معلومات->tdb_معلومات_اندازه || !معلومات->tdb_written)
		معلومات->tdb_معلومات_اندازه = blksize;
	channel->بلاک_اندازه = blksize;
	أرجع retval;
}

ساكن errcode_t undo_read_blk64(io_channel channel, بینښه اوږد اوږد بلاک,
			       صحيح شمېر, فراغ *buf)
{
	errcode_t	retval = 0;
	بنية undo_شخصي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_read_blk64(معلومات->real, بلاک, شمېر, buf);

	أرجع retval;
}

ساكن errcode_t undo_read_blk(io_channel channel, بینښه اوږد بلاک,
			       صحيح شمېر, فراغ *buf)
{
	أرجع undo_read_blk64(channel, بلاک, شمېر, buf);
}

ساكن errcode_t undo_write_blk64(io_channel channel, بینښه اوږد اوږد بلاک,
				صحيح شمېر, ثابت فراغ *buf)
{
	بنية undo_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاک, شمېر);
	إذا (retval)
		 أرجع retval;
	إذا (معلومات->real)
		retval = io_channel_write_blk64(معلومات->real, بلاک, شمېر, buf);

	أرجع retval;
}

ساكن errcode_t undo_write_blk(io_channel channel, بینښه اوږد بلاک,
				صحيح شمېر, ثابت فراغ *buf)
{
	أرجع undo_write_blk64(channel, بلاک, شمېر, buf);
}

ساكن errcode_t undo_write_byte(io_channel channel, بینښه اوږد دننه_واټن,
				 صحيح اندازه, ثابت فراغ *buf)
{
	بنية undo_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	بینښه اوږد blk_عدد, شمېر;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = دننه_واټن + معلومات->دننه_واټن;
	blk_عدد = location/channel->بلاک_اندازه;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	شمېر = (اندازه + (location % channel->بلاک_اندازه) +
			channel->بلاک_اندازه  -1)/channel->بلاک_اندازه;
	retval = undo_write_tdb(channel, blk_عدد, شمېر);
	إذا (retval)
		أرجع retval;
	إذا (معلومات->real && معلومات->real->manager->write_byte)
		retval = io_channel_write_byte(معلومات->real, دننه_واټن, اندازه, buf);

	أرجع retval;
}

ساكن errcode_t undo_discard(io_channel channel, بینښه اوږد اوږد بلاک,
			      بینښه اوږد اوږد شمېر)
{
	بنية undo_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;
	صحيح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (شمېر > لوی_عدد)
		أرجع EXT2_ET_UNIMPLEMENTED;
	icount = شمېر;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاک, icount);
	إذا (retval)
		أرجع retval;
	إذا (معلومات->real)
		retval = io_channel_discard(معلومات->real, بلاک, شمېر);

	أرجع retval;
}

ساكن errcode_t undo_zeroout(io_channel channel, بینښه اوږد اوږد بلاک,
			      بینښه اوږد اوږد شمېر)
{
	بنية undo_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;
	صحيح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (شمېر > لوی_عدد)
		أرجع EXT2_ET_UNIMPLEMENTED;
	icount = شمېر;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاک, icount);
	إذا (retval)
		أرجع retval;
	إذا (معلومات->real)
		retval = io_channel_zeroout(معلومات->real, بلاک, شمېر);

	أرجع retval;
}

ساكن errcode_t undo_cache_readahead(io_channel channel,
				      بینښه اوږد اوږد بلاک,
				      بینښه اوږد اوږد شمېر)
{
	بنية undo_شخصي_معلومات *معلومات;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_cache_readahead(معلومات->real, بلاک, شمېر);

	أرجع retval;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	بنية undo_شخصي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (معلومات->real)
		retval = io_channel_flush(معلومات->real);

	أرجع retval;
}

ساكن errcode_t undo_سېټ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	errcode_t	retval = 0;
	بنية undo_شخصي_معلومات *معلومات;
	بینښه اوږد tmp;
	حرف *پای_ټکی;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(معلومات, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!strcmp(option, "tdb_data_size")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دلیل;

		tmp = strtoul(arg, &پای_ټکی, 0);
		إذا (*پای_ټکی)
			أرجع EXT2_ET_INVALID_دلیل;
		إذا (tmp > E2_UNDO_MAX_بلاک_اندازه || tmp < E2_UNDO_MIN_بلاک_اندازه)
			أرجع EXT2_ET_INVALID_دلیل;
		إذا (!معلومات->tdb_معلومات_اندازه || !معلومات->tdb_written) {
			معلومات->tdb_written = -1;
			معلومات->tdb_معلومات_اندازه = tmp;
		}
		أرجع 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	إذا (معلومات->real && معلومات->real->manager->سېټ_option) {
		retval = معلومات->real->manager->سېټ_option(معلومات->real,
							option, arg);
	}
	إذا (!retval && !strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دلیل;

		tmp = strtoul(arg, &پای_ټکی, 0);
		إذا (*پای_ټکی)
			أرجع EXT2_ET_INVALID_دلیل;
		معلومات->دننه_واټن = tmp;
	}
	أرجع retval;
}

ساكن errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	بنية undo_شخصي_معلومات *معلومات;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	معلومات = (بنية undo_شخصي_معلومات *) channel->شخصي_معلومات;
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
	.سېټ_blksize	= undo_سېټ_blksize,
	.read_blk	= undo_read_blk,
	.write_blk	= undo_write_blk,
	.flush		= undo_flush,
	.write_byte	= undo_write_byte,
	.سېټ_option	= undo_سېټ_option,
	.get_stats	= undo_get_stats,
	.read_blk64	= undo_read_blk64,
	.write_blk64	= undo_write_blk64,
	.discard	= undo_discard,
	.zeroout	= undo_zeroout,
	.cache_readahead	= undo_cache_readahead,
};

io_manager undo_io_manager = &struct_undo_manager;
