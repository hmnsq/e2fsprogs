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

#اگر_تعریف_نشده LARGEFILE_منبع
#تعریف_کن LARGEFILE_منبع
#پایان_اگر
#اگر_تعریف_نشده LARGEFILE64_منبع
#تعریف_کن LARGEFILE64_منبع
#پایان_اگر

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#اگر_تعریف_شده __linux__
#ضمن <sys/utsname.h>
#پایان_اگر
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر
#إذا HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#پایان_اگر
#ضمن <limits.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#اگر_تعریف_شده __GNUC__
#تعریف_کن ATTR(x) __attribute__(x)
#وإلا
#تعریف_کن ATTR(x)
#پایان_اگر

#لغو_تعریف DEBUG

#اگر_تعریف_شده DEBUG
# define dbg_printf(f, a...)  افعل {چاپ_کن(f, ## a); fflush(stdout); } طالما (0)
#وإلا
# define dbg_printf(f, a...)
#پایان_اگر

/*
 * For checking structure magic numbers...
 */

#تعریف_کن EXT2_CHECK_MAGIC(بنية, کد) \
	  إذا ((بنية)->magic != (کد)) أرجع (کد)
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
#تعریف_کن E2UNDO_MAGIC "E2UNDO02"
#تعریف_کن KEYBLOCK_MAGIC 0xCADECADE

#تعریف_کن E2_UNDO_STATE_پایانیافته	0x1	/* undo file is complete */

#تعریف_کن E2_UNDO_MIN_بلوک_اندازه	1024	/* undo blocks are no less than 1KB */
#تعریف_کن E2_UNDO_MAX_بلوک_اندازه	1048576	/* undo blocks are no more than 1MB */

بنية undo_سرآیند {
	حرف magic[8];		/* "E2UNDO02" */
	__le64 رقم_کلیدها;	/* how many keys? */
	__le64 super_جابه_جایی;	/* where in the file is the superblock copy? */
	__le64 کلید_جابه_جایی;	/* where do the key/data block chunks start? */
	__le32 بلوک_اندازه;	/* block size of the undo file */
	__le32 fs_بلوک_اندازه;	/* block size of the target device */
	__le32 sb_بررسی_چرخهای;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_جابه_جایی;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 سرآیند_بررسی_چرخهای;	/* crc32c of this header (but not this field) */
};

#تعریف_کن E2_UNDO_MAX_EXTENT_بلوکها	512	/* max extent size, in blocks */

بنية undo_کلید {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_بررسی_چرخهای;		/* crc32c of the block */
	__le32 اندازه;		/* how many bytes in this block? */
};

بنية undo_کلید_بلوک {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 بررسی_چرخهای;		/* block checksum */
	__le64 reserved;	/* zero */

#إذا __STDC_VERSION__ >= 199901L
	بنية undo_کلید کلیدها[];		/* keys, which come immediately after */
#وإلا
	بنية undo_کلید کلیدها[0];	/* keys, which come immediately after */
#پایان_اگر
};

بنية undo_خصوصی_داده {
	صحيح	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_رقم;			/* next free block */
	blk64_t کلید_blk_رقم;			/* current key block location */
	blk64_t super_blk_رقم;			/* superblock location */
	blk64_t اولی_کلید_blk;			/* first key block location */
	بنية undo_کلید_بلوک *keyb;
	size_t رقم_کلیدها, کلیدها_درون_بلوک;

	/* The backing io channel */
	io_channel real;

	بیعلامت دراز دراز tdb_داده_اندازه;
	صحيح tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t جابه_جایی;

	ext2fs_بلوک_bitmap written_بلوک_map;
	بنية struct_ext2_filsys fake_fs;
	حرف *tdb_file;
	بنية undo_سرآیند hdr;
};
#تعریف_کن کلیدها_در_هر_بلوک(d) (((d)->tdb_داده_اندازه / حجم(بنية undo_کلید)) - 1)

#تعریف_کن E2_UNDO_FEATURE_COMPAT_FS_جابه_جایی 0x1	/* the filesystem offset */

ساكن مضمن فراغ e2undo_مجموعه_feature_fs_جابه_جایی(بنية undo_سرآیند *سرآیند) {
	سرآیند->f_compat |= ext2fs_le32_مقصد_cpu(E2_UNDO_FEATURE_COMPAT_FS_جابه_جایی);
}

ساكن مضمن فراغ e2undo_clear_feature_fs_جابه_جایی(بنية undo_سرآیند *سرآیند) {
	سرآیند->f_compat &= ~ext2fs_le32_مقصد_cpu(E2_UNDO_FEATURE_COMPAT_FS_جابه_جایی);
}

ساكن io_manager undo_io_backing_manager;
ساكن حرف *tdb_file;
ساكن صحيح actual_اندازه;

errcode_t مجموعه_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	أرجع 0;
}

errcode_t مجموعه_undo_io_backup_file(حرف *file_name)
{
	tdb_file = strdup(file_name);

	إذا (tdb_file == عدم) {
		أرجع EXT2_ET_NO_حافظه;
	}

	أرجع 0;
}

ساكن errcode_t write_undo_indexes(بنية undo_خصوصی_داده *داده, صحيح flush)
{
	errcode_t retval;
	بنية ext2_super_بلوک super;
	io_channel channel;
	صحيح بلوک_اندازه;
	__u32 sb_بررسی_چرخهای, hdr_بررسی_چرخهای;

	/* Spit out a key block, if there's any data */
	إذا (داده->کلیدها_درون_بلوک) {
		داده->keyb->magic = ext2fs_cpu_مقصد_le32(KEYBLOCK_MAGIC);
		داده->keyb->بررسی_چرخهای = 0;
		داده->keyb->بررسی_چرخهای = ext2fs_cpu_مقصد_le32(
					 ext2fs_crc32c_le(~0,
					 (بیعلامت حرف *)داده->keyb,
					 داده->tdb_داده_اندازه));
		dbg_printf("Writing keyblock to blk %llu\n", داده->کلید_blk_رقم);
		retval = io_channel_write_blk64(داده->undo_file,
						داده->کلید_blk_رقم,
						1, داده->keyb);
		إذا (retval)
			أرجع retval;
		/* Move on to the next key block if it's full. */
		إذا (داده->کلیدها_درون_بلوک == کلیدها_در_هر_بلوک(داده)) {
			memset(داده->keyb, 0, داده->tdb_داده_اندازه);
			داده->کلیدها_درون_بلوک = 0;
			داده->کلید_blk_رقم = داده->undo_blk_رقم;
			داده->undo_blk_رقم++;
		}
	}

	/* Prepare superblock for write */
	channel = داده->real;
	بلوک_اندازه = channel->بلوک_اندازه;

	io_channel_مجموعه_blksize(channel, SUPERBLOCK_جابه_جایی);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_اندازه, &super);
	إذا (retval)
		اذهب err_بیرون;
	sb_بررسی_چرخهای = ext2fs_crc32c_le(~0, (بیعلامت حرف *)&super, SUPERBLOCK_اندازه);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	کپی_حافظه(داده->hdr.magic, E2UNDO_MAGIC, حجم(داده->hdr.magic));
	داده->hdr.رقم_کلیدها = ext2fs_cpu_مقصد_le64(داده->رقم_کلیدها);
	داده->hdr.super_جابه_جایی = ext2fs_cpu_مقصد_le64(داده->super_blk_رقم);
	داده->hdr.کلید_جابه_جایی = ext2fs_cpu_مقصد_le64(داده->اولی_کلید_blk);
	داده->hdr.fs_بلوک_اندازه = ext2fs_cpu_مقصد_le32(بلوک_اندازه);
	داده->hdr.sb_بررسی_چرخهای = ext2fs_cpu_مقصد_le32(sb_بررسی_چرخهای);
	داده->hdr.fs_جابه_جایی = ext2fs_cpu_مقصد_le64(داده->جابه_جایی);
	إذا (داده->جابه_جایی)
		e2undo_مجموعه_feature_fs_جابه_جایی(&داده->hdr);
	وإلا
		e2undo_clear_feature_fs_جابه_جایی(&داده->hdr);
	hdr_بررسی_چرخهای = ext2fs_crc32c_le(~0, (بیعلامت حرف *)&داده->hdr,
				   حجم(داده->hdr) -
				   حجم(داده->hdr.سرآیند_بررسی_چرخهای));
	داده->hdr.سرآیند_بررسی_چرخهای = ext2fs_cpu_مقصد_le32(hdr_بررسی_چرخهای);
	retval = io_channel_write_blk64(داده->undo_file, 0,
					-(صحيح)حجم(داده->hdr),
					&داده->hdr);
	إذا (retval)
		اذهب err_بیرون;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", داده->super_blk_رقم);
	retval = io_channel_write_blk64(داده->undo_file, داده->super_blk_رقم,
					-SUPERBLOCK_اندازه, &super);
	إذا (retval)
		اذهب err_بیرون;

	إذا (flush)
		retval = io_channel_flush(داده->undo_file);
err_بیرون:
	io_channel_مجموعه_blksize(channel, بلوک_اندازه);
	أرجع retval;
}

ساكن errcode_t undo_راهاندازی_tdb(بنية undo_خصوصی_داده *داده)
{
	صحيح i;
	errcode_t retval;

	إذا (داده->tdb_written == 1)
		أرجع 0;

	داده->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&داده->fake_fs, 0, حجم(داده->fake_fs));
	داده->fake_fs.blocksize = داده->tdb_داده_اندازه;
	retval = ext2fs_alloc_generic_bmap(&داده->fake_fs,
				EXT2_ET_MAGIC_بلوک_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &داده->written_بلوک_map);
	إذا (retval)
		أرجع retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(داده->tdb_داده_اندازه, &داده->keyb);
	إذا (retval)
		أرجع retval;
	داده->کلید_blk_رقم = داده->اولی_کلید_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", داده->tdb_داده_اندازه);
	dbg_printf("Keys per block %llu\n", کلیدها_در_هر_بلوک(داده));
	داده->hdr.بلوک_اندازه = ext2fs_cpu_مقصد_le32(داده->tdb_داده_اندازه);
	io_channel_مجموعه_blksize(داده->undo_file, داده->tdb_داده_اندازه);

	/* Ensure that we have space for header blocks */
	لكل (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(داده->undo_file, i, 1,
					       داده->keyb);
		إذا (retval)
			memset(داده->keyb, 0, داده->tdb_داده_اندازه);
		retval = io_channel_write_blk64(داده->undo_file, i, 1,
						داده->keyb);
		إذا (retval)
			أرجع retval;
		retval = io_channel_flush(داده->undo_file);
		إذا (retval)
			أرجع retval;
	}
	memset(داده->keyb, 0, داده->tdb_داده_اندازه);
	أرجع 0;
}

ساكن errcode_t undo_write_tdb(io_channel channel,
				بیعلامت دراز دراز بلوک, صحيح شمار)

{
	صحيح اندازه, sz;
	بیعلامت دراز دراز بلوک_رقم, backing_blk_رقم;
	errcode_t retval = 0;
	ext2_loff_t جابه_جایی;
	بنية undo_خصوصی_داده *داده;
	بیعلامت حرف *read_ptr;
	بیعلامت دراز دراز انتها_بلوک;
	بیعلامت دراز دراز داده_اندازه;
	بنية undo_کلید *کلید;
	__u32 blk_بررسی_چرخهای;

	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;

	إذا (داده->undo_file == عدم) {
		/*
		 * Transaction database not initialized
		 */
		أرجع 0;
	}

	إذا (شمار == 1)
		اندازه = channel->بلوک_اندازه;
	وإلا {
		إذا (شمار < 0)
			اندازه = -شمار;
		وإلا
			اندازه = شمار * channel->بلوک_اندازه;
	}

	retval = undo_راهاندازی_tdb(داده);
	إذا (retval)
		أرجع retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	جابه_جایی = (بلوک * channel->بلوک_اندازه) + داده->جابه_جایی ;
	بلوک_رقم = جابه_جایی / داده->tdb_داده_اندازه;
	انتها_بلوک = (جابه_جایی + اندازه - 1) / داده->tdb_داده_اندازه;

	طالما (بلوک_رقم <= انتها_بلوک) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		إذا (ext2fs_test_بلوک_bitmap2(داده->written_بلوک_map,
						   بلوک_رقم)) {
			/* Try the next block */
			بلوک_رقم++;
			استمر;
		}
		ext2fs_mark_بلوک_bitmap2(داده->written_بلوک_map, بلوک_رقم);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		جابه_جایی = بلوک_رقم * داده->tdb_داده_اندازه +
				(داده->جابه_جایی % داده->tdb_داده_اندازه);
		backing_blk_رقم = (جابه_جایی - داده->جابه_جایی) / channel->بلوک_اندازه;

		retval = ext2fs_get_mem(داده->tdb_داده_اندازه, &read_ptr);
		إذا (retval) {
			أرجع retval;
		}

		memset(read_ptr, 0, داده->tdb_داده_اندازه);
		actual_اندازه = 0;
		إذا ((داده->tdb_داده_اندازه % channel->بلوک_اندازه) == 0)
			sz = داده->tdb_داده_اندازه / channel->بلوک_اندازه;
		وإلا
			sz = -داده->tdb_داده_اندازه;
		retval = io_channel_read_blk64(داده->real, backing_blk_رقم,
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
			داده_اندازه = actual_اندازه;
		} وإلا {
			داده_اندازه = داده->tdb_داده_اندازه;
		}
		إذا (داده_اندازه == 0) {
			حرر(read_ptr);
			بلوک_رقم++;
			استمر;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       داده_اندازه, backing_blk_رقم, بلوک, داده->tdb_داده_اندازه);
		إذا ((داده_اندازه % داده->undo_file->بلوک_اندازه) == 0)
			sz = داده_اندازه / داده->undo_file->بلوک_اندازه;
		وإلا
			sz = -داده_اندازه;;
		/* extend this key? */
		إذا (داده->کلیدها_درون_بلوک) {
			کلید = داده->keyb->کلیدها + داده->کلیدها_درون_بلوک - 1;
			keysz = ext2fs_le32_مقصد_cpu(کلید->اندازه);
		} وإلا {
			کلید = عدم;
			keysz = 0;
		}
		إذا (کلید != عدم &&
		    (ext2fs_le64_مقصد_cpu(کلید->fsblk) * channel->بلوک_اندازه +
		     channel->بلوک_اندازه - 1 +
		     keysz) / channel->بلوک_اندازه == backing_blk_رقم &&
		    E2_UNDO_MAX_EXTENT_بلوکها * داده->tdb_داده_اندازه >
		    keysz + داده_اندازه) {
			blk_بررسی_چرخهای = ext2fs_le32_مقصد_cpu(کلید->blk_بررسی_چرخهای);
			blk_بررسی_چرخهای = ext2fs_crc32c_le(blk_بررسی_چرخهای, read_ptr, داده_اندازه);
			کلید->blk_بررسی_چرخهای = ext2fs_cpu_مقصد_le32(blk_بررسی_چرخهای);
			کلید->اندازه = ext2fs_cpu_مقصد_le32(keysz + داده_اندازه);
		} وإلا {
			داده->رقم_کلیدها++;
			کلید = داده->keyb->کلیدها + داده->کلیدها_درون_بلوک;
			داده->کلیدها_درون_بلوک++;
			کلید->fsblk = ext2fs_cpu_مقصد_le64(backing_blk_رقم);
			blk_بررسی_چرخهای = ext2fs_crc32c_le(~0, read_ptr, داده_اندازه);
			کلید->blk_بررسی_چرخهای = ext2fs_cpu_مقصد_le32(blk_بررسی_چرخهای);
			کلید->اندازه = ext2fs_cpu_مقصد_le32(داده_اندازه);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       بلوک_رقم,
		       داده->undo_blk_رقم,
		       sz, داده->رقم_کلیدها - 1);
		retval = io_channel_write_blk64(داده->undo_file,
					داده->undo_blk_رقم, sz, read_ptr);
		إذا (retval) {
			حرر(read_ptr);
			أرجع retval;
		}
		داده->undo_blk_رقم++;
		حرر(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(داده, 0);
		إذا (retval)
			أرجع retval;

		/* Next block */
		بلوک_رقم++;
	}

	أرجع retval;
}

ساكن errcode_t undo_io_read_error(io_channel channel ATTR((استفادهنشده)),
				    بیعلامت دراز بلوک ATTR((استفادهنشده)),
				    صحيح شمار ATTR((استفادهنشده)),
				    فراغ *داده ATTR((استفادهنشده)),
				    size_t اندازه ATTR((استفادهنشده)),
				    صحيح actual,
				    errcode_t error ATTR((استفادهنشده)))
{
	actual_اندازه = actual;
	أرجع error;
}

ساكن فراغ undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ساكن صحيح check_filesystem(بنية undo_سرآیند *hdr, io_channel undo_file,
			    بیعلامت صحيح blocksize, blk64_t super_بلوک,
			    io_channel channel)
{
	بنية ext2_super_بلوک super, *sb;
	حرف *buf;
	__u32 sb_بررسی_چرخهای;
	errcode_t retval;

	io_channel_مجموعه_blksize(channel, SUPERBLOCK_جابه_جایی);
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
	retval = io_channel_read_blk64(undo_file, super_بلوک,
				       -SUPERBLOCK_اندازه, buf);
	إذا (retval)
		اذهب بیرون;
	sb = (بنية ext2_super_بلوک *)buf;
	sb->s_magic = ~sb->s_magic;
	إذا (memcmp(&super, buf, حجم(super))) {
		retval = -1;
		اذهب بیرون;
	}
	sb_بررسی_چرخهای = ext2fs_crc32c_le(~0, (بیعلامت حرف *)buf, SUPERBLOCK_اندازه);
	إذا (ext2fs_le32_مقصد_cpu(hdr->sb_بررسی_چرخهای) != sb_بررسی_چرخهای) {
		retval = -1;
		اذهب بیرون;
	}

بیرون:
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
				      بنية undo_خصوصی_داده *داده)
{
	بنية undo_سرآیند hdr;
	بنية undo_کلید *dkey;
	ext2fs_struct_stat statbuf;
	بیعلامت صحيح blocksize, fs_blocksize;
	blk64_t super_بلوک, lblk;
	size_t رقم_کلیدها, کلیدها_در_هر_بلوک_2, i;
	__u32 hdr_بررسی_چرخهای, کلید_بررسی_چرخهای;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	إذا (retval)
		اذهب bad_file;
	إذا (statbuf.st_اندازه == 0)
		اذهب بیرون;

	/* check the file header */
	retval = io_channel_read_blk64(داده->undo_file, 0, -(صحيح)حجم(hdr),
				       &hdr);
	إذا (retval)
		اذهب bad_file;

	إذا (memcmp(hdr.magic, E2UNDO_MAGIC,
		    حجم(hdr.magic)))
		اذهب bad_file;
	hdr_بررسی_چرخهای = ext2fs_crc32c_le(~0, (بیعلامت حرف *)&hdr,
				   حجم(بنية undo_سرآیند) -
				   حجم(__u32));
	إذا (ext2fs_le32_مقصد_cpu(hdr.سرآیند_بررسی_چرخهای) != hdr_بررسی_چرخهای)
		اذهب bad_file;
	blocksize = ext2fs_le32_مقصد_cpu(hdr.بلوک_اندازه);
	fs_blocksize = ext2fs_le32_مقصد_cpu(hdr.fs_بلوک_اندازه);
	إذا (blocksize > E2_UNDO_MAX_بلوک_اندازه ||
	    blocksize < E2_UNDO_MIN_بلوک_اندازه ||
	    !blocksize || !fs_blocksize)
		اذهب bad_file;
	super_بلوک = ext2fs_le64_مقصد_cpu(hdr.super_جابه_جایی);
	رقم_کلیدها = ext2fs_le64_مقصد_cpu(hdr.رقم_کلیدها);
	io_channel_مجموعه_blksize(داده->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	إذا (hdr.f_incompat || hdr.f_rocompat)
		اذهب bad_file;

	/* Superblock matches this FS? */
	إذا (check_filesystem(&hdr, داده->undo_file, blocksize, super_بلوک,
			     داده->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		اذهب بیرون;
	}

	/* Try to set ourselves up */
	داده->tdb_داده_اندازه = blocksize;
	retval = undo_راهاندازی_tdb(داده);
	إذا (retval)
		اذهب bad_file;
	داده->رقم_کلیدها = رقم_کلیدها;
	داده->super_blk_رقم = super_بلوک;
	داده->اولی_کلید_blk = ext2fs_le64_مقصد_cpu(hdr.کلید_جابه_جایی);

	/* load the written block map */
	کلیدها_در_هر_بلوک_2 = کلیدها_در_هر_بلوک(داده);
	lblk = داده->اولی_کلید_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   رقم_کلیدها, کلیدها_در_هر_بلوک_2, blocksize);
	لكل (i = 0; i < رقم_کلیدها; i += کلیدها_در_هر_بلوک_2) {
		size_t j, max_j;
		__le32 بررسی_چرخهای;

		داده->کلید_blk_رقم = lblk;
		retval = io_channel_read_blk64(داده->undo_file,
					       lblk, 1, داده->keyb);
		إذا (retval)
			اذهب bad_کلید_replay;

		/* check keys */
		إذا (ext2fs_le32_مقصد_cpu(داده->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			اذهب bad_کلید_replay;
		}
		بررسی_چرخهای = داده->keyb->بررسی_چرخهای;
		داده->keyb->بررسی_چرخهای = 0;
		کلید_بررسی_چرخهای = ext2fs_crc32c_le(~0, (بیعلامت حرف *)داده->keyb,
					   blocksize);
		إذا (ext2fs_le32_مقصد_cpu(بررسی_چرخهای) != کلید_بررسی_چرخهای) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			اذهب bad_کلید_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = داده->رقم_کلیدها - i;
		إذا (max_j > کلیدها_در_هر_بلوک_2)
			max_j = کلیدها_در_هر_بلوک_2;
		لكل (j = 0, dkey = داده->keyb->کلیدها;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_مقصد_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t اندازه = ext2fs_le32_مقصد_cpu(dkey->اندازه);

			ext2fs_mark_بلوک_bitmap_range2(داده->written_بلوک_map,
					 undo_blk,
					(اندازه + blocksize - 1) / blocksize);
			lblk += (اندازه + blocksize - 1) / blocksize;
			داده->undo_blk_رقم = lblk;
			داده->کلیدها_درون_بلوک = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   داده->کلید_blk_رقم, داده->undo_blk_رقم, داده->رقم_کلیدها,
		   داده->کلیدها_درون_بلوک);

	داده->hdr.state = hdr.state & ~E2_UNDO_STATE_پایانیافته;
	داده->hdr.f_compat = hdr.f_compat;
	داده->hdr.f_incompat = hdr.f_incompat;
	داده->hdr.f_rocompat = hdr.f_rocompat;
	أرجع retval;

bad_کلید_replay:
	داده->کلید_blk_رقم = داده->undo_blk_رقم = 0;
	داده->کلیدها_درون_بلوک = 0;
	ext2fs_free_mem(&داده->keyb);
	ext2fs_free_generic_bitmap(داده->written_بلوک_map);
	داده->tdb_written = 0;
	اذهب بیرون;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
بیرون:
	أرجع retval;
}

ساكن فراغ undo_atexit(فراغ *p)
{
	بنية undo_خصوصی_داده *داده = p;
	errcode_t err;

	err = write_undo_indexes(داده, 1);
	io_channel_close(داده->undo_file);

	com_err(داده->tdb_file, err, "while force-closing undo file");
}

ساكن errcode_t undo_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية undo_خصوصی_داده *داده = عدم;
	صحيح		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_نخها;
	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية undo_خصوصی_داده), &داده);
	إذا (retval)
		اذهب cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(طول_رشته(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->خصوصی_داده = داده;
	io->بلوک_اندازه = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(داده, 0, حجم(بنية undo_خصوصی_داده));
	داده->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	داده->super_blk_رقم = 1;
	داده->اولی_کلید_blk = 2;
	داده->undo_blk_رقم = 3;

	إذا (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &داده->real);
		إذا (retval)
			اذهب cleanup;

		داده->tdb_file = strdup(tdb_file);
		إذا (داده->tdb_file == عدم)
			اذهب cleanup;
		undo_fd = ext2fs_open_file(داده->tdb_file, O_RDWR | O_CREAT,
					   0600);
		إذا (undo_fd < 0)
			اذهب cleanup;

		retval = undo_io_backing_manager->open(داده->tdb_file,
						       IO_FLAG_RW,
						       &داده->undo_file);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		داده->real = عدم;
		داده->undo_file = عدم;
	}

	إذا (داده->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (داده->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	إذا (داده->real)
		undo_err_handler_init(داده->real);

	إذا (داده->undo_file) {
		retval = try_reopen_undo_file(undo_fd, داده);
		إذا (retval)
			اذهب cleanup;
	}
	retval = ext2fs_جمع_کن_exit_fn(undo_atexit, داده);
	إذا (retval)
		اذهب cleanup;

	*channel = io;
	إذا (undo_fd >= 0)
		close(undo_fd);
	أرجع retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, داده);
	إذا (undo_fd >= 0)
		close(undo_fd);
	إذا (داده && داده->undo_file)
		io_channel_close(داده->undo_file);
	إذا (داده && داده->tdb_file)
		حرر(داده->tdb_file);
	إذا (داده && داده->real)
		io_channel_close(داده->real);
	إذا (داده)
		ext2fs_free_mem(&داده);
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (io)
		ext2fs_free_mem(&io);
	أرجع retval;
}

ساكن errcode_t undo_close(io_channel channel)
{
	بنية undo_خصوصی_داده *داده;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;
	/* Before closing write the file system identity */
	إذا (!ext2fs_امن_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		داده->hdr.state = ext2fs_cpu_مقصد_le32(E2_UNDO_STATE_پایانیافته);
	err = write_undo_indexes(داده, 1);
	ext2fs_remove_exit_fn(undo_atexit, داده);
	إذا (داده->real)
		retval = io_channel_close(داده->real);
	إذا (داده->tdb_file)
		حرر(داده->tdb_file);
	إذا (داده->undo_file)
		io_channel_close(داده->undo_file);
	ext2fs_free_mem(&داده->keyb);
	إذا (داده->written_بلوک_map)
		ext2fs_free_generic_bitmap(داده->written_بلوک_map);
	ext2fs_free_mem(&channel->خصوصی_داده);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	إذا (err)
		أرجع err;
	أرجع retval;
}

ساكن errcode_t undo_مجموعه_blksize(io_channel channel, صحيح blksize)
{
	بنية undo_خصوصی_داده *داده;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (blksize > E2_UNDO_MAX_بلوک_اندازه || blksize < E2_UNDO_MIN_بلوک_اندازه)
		أرجع EXT2_ET_INVALID_آرگومان;

	إذا (داده->real)
		retval = io_channel_مجموعه_blksize(داده->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	إذا (!داده->tdb_داده_اندازه || !داده->tdb_written)
		داده->tdb_داده_اندازه = blksize;
	channel->بلوک_اندازه = blksize;
	أرجع retval;
}

ساكن errcode_t undo_read_blk64(io_channel channel, بیعلامت دراز دراز بلوک,
			       صحيح شمار, فراغ *buf)
{
	errcode_t	retval = 0;
	بنية undo_خصوصی_داده *داده;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_read_blk64(داده->real, بلوک, شمار, buf);

	أرجع retval;
}

ساكن errcode_t undo_read_blk(io_channel channel, بیعلامت دراز بلوک,
			       صحيح شمار, فراغ *buf)
{
	أرجع undo_read_blk64(channel, بلوک, شمار, buf);
}

ساكن errcode_t undo_write_blk64(io_channel channel, بیعلامت دراز دراز بلوک,
				صحيح شمار, ثابت فراغ *buf)
{
	بنية undo_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلوک, شمار);
	إذا (retval)
		 أرجع retval;
	إذا (داده->real)
		retval = io_channel_write_blk64(داده->real, بلوک, شمار, buf);

	أرجع retval;
}

ساكن errcode_t undo_write_blk(io_channel channel, بیعلامت دراز بلوک,
				صحيح شمار, ثابت فراغ *buf)
{
	أرجع undo_write_blk64(channel, بلوک, شمار, buf);
}

ساكن errcode_t undo_write_byte(io_channel channel, بیعلامت دراز جابه_جایی,
				 صحيح اندازه, ثابت فراغ *buf)
{
	بنية undo_خصوصی_داده *داده;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	بیعلامت دراز blk_رقم, شمار;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = جابه_جایی + داده->جابه_جایی;
	blk_رقم = location/channel->بلوک_اندازه;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	شمار = (اندازه + (location % channel->بلوک_اندازه) +
			channel->بلوک_اندازه  -1)/channel->بلوک_اندازه;
	retval = undo_write_tdb(channel, blk_رقم, شمار);
	إذا (retval)
		أرجع retval;
	إذا (داده->real && داده->real->manager->write_byte)
		retval = io_channel_write_byte(داده->real, جابه_جایی, اندازه, buf);

	أرجع retval;
}

ساكن errcode_t undo_discard(io_channel channel, بیعلامت دراز دراز بلوک,
			      بیعلامت دراز دراز شمار)
{
	بنية undo_خصوصی_داده *داده;
	errcode_t	retval = 0;
	صحيح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (شمار > بیشترین_صحیح)
		أرجع EXT2_ET_UNIMPLEMENTED;
	icount = شمار;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلوک, icount);
	إذا (retval)
		أرجع retval;
	إذا (داده->real)
		retval = io_channel_discard(داده->real, بلوک, شمار);

	أرجع retval;
}

ساكن errcode_t undo_zeroout(io_channel channel, بیعلامت دراز دراز بلوک,
			      بیعلامت دراز دراز شمار)
{
	بنية undo_خصوصی_داده *داده;
	errcode_t	retval = 0;
	صحيح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (شمار > بیشترین_صحیح)
		أرجع EXT2_ET_UNIMPLEMENTED;
	icount = شمار;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلوک, icount);
	إذا (retval)
		أرجع retval;
	إذا (داده->real)
		retval = io_channel_zeroout(داده->real, بلوک, شمار);

	أرجع retval;
}

ساكن errcode_t undo_cache_readahead(io_channel channel,
				      بیعلامت دراز دراز بلوک,
				      بیعلامت دراز دراز شمار)
{
	بنية undo_خصوصی_داده *داده;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_cache_readahead(داده->real, بلوک, شمار);

	أرجع retval;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	بنية undo_خصوصی_داده *داده;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (داده->real)
		retval = io_channel_flush(داده->real);

	أرجع retval;
}

ساكن errcode_t undo_مجموعه_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	errcode_t	retval = 0;
	بنية undo_خصوصی_داده *داده;
	بیعلامت دراز tmp;
	حرف *انتها;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!strcmp(option, "tdb_data_size")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_آرگومان;

		tmp = strtoul(arg, &انتها, 0);
		إذا (*انتها)
			أرجع EXT2_ET_INVALID_آرگومان;
		إذا (tmp > E2_UNDO_MAX_بلوک_اندازه || tmp < E2_UNDO_MIN_بلوک_اندازه)
			أرجع EXT2_ET_INVALID_آرگومان;
		إذا (!داده->tdb_داده_اندازه || !داده->tdb_written) {
			داده->tdb_written = -1;
			داده->tdb_داده_اندازه = tmp;
		}
		أرجع 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	إذا (داده->real && داده->real->manager->مجموعه_option) {
		retval = داده->real->manager->مجموعه_option(داده->real,
							option, arg);
	}
	إذا (!retval && !strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_آرگومان;

		tmp = strtoul(arg, &انتها, 0);
		إذا (*انتها)
			أرجع EXT2_ET_INVALID_آرگومان;
		داده->جابه_جایی = tmp;
	}
	أرجع retval;
}

ساكن errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	بنية undo_خصوصی_داده *داده;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	داده = (بنية undo_خصوصی_داده *) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(داده, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (داده->real)
		retval = (داده->real->manager->get_stats)(داده->real, stats);

	أرجع retval;
}

ساكن بنية struct_io_manager struct_undo_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Undo I/O Manager",
	.open		= undo_open,
	.close		= undo_close,
	.مجموعه_blksize	= undo_مجموعه_blksize,
	.read_blk	= undo_read_blk,
	.write_blk	= undo_write_blk,
	.flush		= undo_flush,
	.write_byte	= undo_write_byte,
	.مجموعه_option	= undo_مجموعه_option,
	.get_stats	= undo_get_stats,
	.read_blk64	= undo_read_blk64,
	.write_blk64	= undo_write_blk64,
	.discard	= undo_discard,
	.zeroout	= undo_zeroout,
	.cache_readahead	= undo_cache_readahead,
};

io_manager undo_io_manager = &struct_undo_manager;
