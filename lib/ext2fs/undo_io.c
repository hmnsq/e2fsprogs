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

#اگر_غیر_تعریف_شدہ LARGEFILE_ماخذ
#تعریف_کرو LARGEFILE_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ LARGEFILE64_ماخذ
#تعریف_کرو LARGEFILE64_ماخذ
#اگر_ختم

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر_تعریف_شدہ __linux__
#شامل <sys/utsname.h>
#اگر_ختم
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر HAVE_SYS_RESOURCE_H
#شامل <sys/resource.h>
#اگر_ختم
#شامل <limits.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#اگر_تعریف_شدہ __GNUC__
#تعریف_کرو ATTR(x) __attribute__(x)
#ورنہ
#تعریف_کرو ATTR(x)
#اگر_ختم

#تعریف_منسوخ DEBUG

#اگر_تعریف_شدہ DEBUG
# define dbg_printf(f, a...)  کرو {لکھو(f, ## a); fflush(stdout); } جب (0)
#ورنہ
# define dbg_printf(f, a...)
#اگر_ختم

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(ساخت, رمز) \
	  اگر ((ساخت)->magic != (رمز)) واپس (رمز)
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
#تعریف_کرو E2UNDO_MAGIC "E2UNDO02"
#تعریف_کرو KEYBLOCK_MAGIC 0xCADECADE

#تعریف_کرو E2_UNDO_STATE_ختم_شدہ	0x1	/* undo file is complete */

#تعریف_کرو E2_UNDO_MIN_بلاک_سائز	1024	/* undo blocks are no less than 1KB */
#تعریف_کرو E2_UNDO_MAX_بلاک_سائز	1048576	/* undo blocks are no more than 1MB */

ساخت undo_سرخی {
	حرف magic[8];		/* "E2UNDO02" */
	__le64 عدد_کلیدیں;	/* how many keys? */
	__le64 super_انحراف;	/* where in the file is the superblock copy? */
	__le64 کلید_انحراف;	/* where do the key/data block chunks start? */
	__le32 بلاک_سائز;	/* block size of the undo file */
	__le32 fs_بلاک_سائز;	/* block size of the target device */
	__le32 sb_چکری_جانچ;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_انحراف;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 سرخی_چکری_جانچ;	/* crc32c of this header (but not this field) */
};

#تعریف_کرو E2_UNDO_MAX_EXTENT_بلاکیں	512	/* max extent size, in blocks */

ساخت undo_کلید {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_چکری_جانچ;		/* crc32c of the block */
	__le32 سائز;		/* how many bytes in this block? */
};

ساخت undo_کلید_بلاک {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 چکری_جانچ;		/* block checksum */
	__le64 reserved;	/* zero */

#اگر __STDC_VERSION__ >= 199901L
	ساخت undo_کلید کلیدیں[];		/* keys, which come immediately after */
#ورنہ
	ساخت undo_کلید کلیدیں[0];	/* keys, which come immediately after */
#اگر_ختم
};

ساخت undo_نجی_ڈیٹا {
	صحیح	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_عدد;			/* next free block */
	blk64_t کلید_blk_عدد;			/* current key block location */
	blk64_t super_blk_عدد;			/* superblock location */
	blk64_t پہلا_کلید_blk;			/* first key block location */
	ساخت undo_کلید_بلاک *keyb;
	size_t عدد_کلیدیں, کلیدیں_اندر_بلاک;

	/* The backing io channel */
	io_channel real;

	غیرعلامتی طویل طویل tdb_ڈیٹا_سائز;
	صحیح tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t انحراف;

	ext2fs_بلاک_bitmap written_بلاک_map;
	ساخت struct_ext2_filsys fake_fs;
	حرف *tdb_file;
	ساخت undo_سرخی hdr;
};
#تعریف_کرو کلیدیں_فی_بلاک(d) (((d)->tdb_ڈیٹا_سائز / حجم(ساخت undo_کلید)) - 1)

#تعریف_کرو E2_UNDO_FEATURE_COMPAT_FS_انحراف 0x1	/* the filesystem offset */

ساکن اندرونی خالی e2undo_سیٹ_feature_fs_انحراف(ساخت undo_سرخی *سرخی) {
	سرخی->f_compat |= ext2fs_le32_مقصد_cpu(E2_UNDO_FEATURE_COMPAT_FS_انحراف);
}

ساکن اندرونی خالی e2undo_clear_feature_fs_انحراف(ساخت undo_سرخی *سرخی) {
	سرخی->f_compat &= ~ext2fs_le32_مقصد_cpu(E2_UNDO_FEATURE_COMPAT_FS_انحراف);
}

ساکن io_manager undo_io_backing_manager;
ساکن حرف *tdb_file;
ساکن صحیح actual_سائز;

errcode_t سیٹ_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	واپس 0;
}

errcode_t سیٹ_undo_io_backup_file(حرف *file_name)
{
	tdb_file = strdup(file_name);

	اگر (tdb_file == تہی) {
		واپس EXT2_ET_NO_حافظہ;
	}

	واپس 0;
}

ساکن errcode_t write_undo_indexes(ساخت undo_نجی_ڈیٹا *ڈیٹا, صحیح flush)
{
	errcode_t retval;
	ساخت ext2_super_بلاک super;
	io_channel channel;
	صحیح بلاک_سائز;
	__u32 sb_چکری_جانچ, hdr_چکری_جانچ;

	/* Spit out a key block, if there's any data */
	اگر (ڈیٹا->کلیدیں_اندر_بلاک) {
		ڈیٹا->keyb->magic = ext2fs_cpu_مقصد_le32(KEYBLOCK_MAGIC);
		ڈیٹا->keyb->چکری_جانچ = 0;
		ڈیٹا->keyb->چکری_جانچ = ext2fs_cpu_مقصد_le32(
					 ext2fs_crc32c_le(~0,
					 (غیرعلامتی حرف *)ڈیٹا->keyb,
					 ڈیٹا->tdb_ڈیٹا_سائز));
		dbg_printf("Writing keyblock to blk %llu\n", ڈیٹا->کلید_blk_عدد);
		retval = io_channel_write_blk64(ڈیٹا->undo_file,
						ڈیٹا->کلید_blk_عدد,
						1, ڈیٹا->keyb);
		اگر (retval)
			واپس retval;
		/* Move on to the next key block if it's full. */
		اگر (ڈیٹا->کلیدیں_اندر_بلاک == کلیدیں_فی_بلاک(ڈیٹا)) {
			memset(ڈیٹا->keyb, 0, ڈیٹا->tdb_ڈیٹا_سائز);
			ڈیٹا->کلیدیں_اندر_بلاک = 0;
			ڈیٹا->کلید_blk_عدد = ڈیٹا->undo_blk_عدد;
			ڈیٹا->undo_blk_عدد++;
		}
	}

	/* Prepare superblock for write */
	channel = ڈیٹا->real;
	بلاک_سائز = channel->بلاک_سائز;

	io_channel_سیٹ_blksize(channel, SUPERBLOCK_انحراف);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_سائز, &super);
	اگر (retval)
		جاؤ err_برآمد;
	sb_چکری_جانچ = ext2fs_crc32c_le(~0, (غیرعلامتی حرف *)&super, SUPERBLOCK_سائز);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	حافظہ_نقل(ڈیٹا->hdr.magic, E2UNDO_MAGIC, حجم(ڈیٹا->hdr.magic));
	ڈیٹا->hdr.عدد_کلیدیں = ext2fs_cpu_مقصد_le64(ڈیٹا->عدد_کلیدیں);
	ڈیٹا->hdr.super_انحراف = ext2fs_cpu_مقصد_le64(ڈیٹا->super_blk_عدد);
	ڈیٹا->hdr.کلید_انحراف = ext2fs_cpu_مقصد_le64(ڈیٹا->پہلا_کلید_blk);
	ڈیٹا->hdr.fs_بلاک_سائز = ext2fs_cpu_مقصد_le32(بلاک_سائز);
	ڈیٹا->hdr.sb_چکری_جانچ = ext2fs_cpu_مقصد_le32(sb_چکری_جانچ);
	ڈیٹا->hdr.fs_انحراف = ext2fs_cpu_مقصد_le64(ڈیٹا->انحراف);
	اگر (ڈیٹا->انحراف)
		e2undo_سیٹ_feature_fs_انحراف(&ڈیٹا->hdr);
	ورنہ
		e2undo_clear_feature_fs_انحراف(&ڈیٹا->hdr);
	hdr_چکری_جانچ = ext2fs_crc32c_le(~0, (غیرعلامتی حرف *)&ڈیٹا->hdr,
				   حجم(ڈیٹا->hdr) -
				   حجم(ڈیٹا->hdr.سرخی_چکری_جانچ));
	ڈیٹا->hdr.سرخی_چکری_جانچ = ext2fs_cpu_مقصد_le32(hdr_چکری_جانچ);
	retval = io_channel_write_blk64(ڈیٹا->undo_file, 0,
					-(صحیح)حجم(ڈیٹا->hdr),
					&ڈیٹا->hdr);
	اگر (retval)
		جاؤ err_برآمد;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", ڈیٹا->super_blk_عدد);
	retval = io_channel_write_blk64(ڈیٹا->undo_file, ڈیٹا->super_blk_عدد,
					-SUPERBLOCK_سائز, &super);
	اگر (retval)
		جاؤ err_برآمد;

	اگر (flush)
		retval = io_channel_flush(ڈیٹا->undo_file);
err_برآمد:
	io_channel_سیٹ_blksize(channel, بلاک_سائز);
	واپس retval;
}

ساکن errcode_t undo_تیاری_tdb(ساخت undo_نجی_ڈیٹا *ڈیٹا)
{
	صحیح i;
	errcode_t retval;

	اگر (ڈیٹا->tdb_written == 1)
		واپس 0;

	ڈیٹا->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&ڈیٹا->fake_fs, 0, حجم(ڈیٹا->fake_fs));
	ڈیٹا->fake_fs.blocksize = ڈیٹا->tdb_ڈیٹا_سائز;
	retval = ext2fs_alloc_generic_bmap(&ڈیٹا->fake_fs,
				EXT2_ET_MAGIC_بلاک_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &ڈیٹا->written_بلاک_map);
	اگر (retval)
		واپس retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(ڈیٹا->tdb_ڈیٹا_سائز, &ڈیٹا->keyb);
	اگر (retval)
		واپس retval;
	ڈیٹا->کلید_blk_عدد = ڈیٹا->پہلا_کلید_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", ڈیٹا->tdb_ڈیٹا_سائز);
	dbg_printf("Keys per block %llu\n", کلیدیں_فی_بلاک(ڈیٹا));
	ڈیٹا->hdr.بلاک_سائز = ext2fs_cpu_مقصد_le32(ڈیٹا->tdb_ڈیٹا_سائز);
	io_channel_سیٹ_blksize(ڈیٹا->undo_file, ڈیٹا->tdb_ڈیٹا_سائز);

	/* Ensure that we have space for header blocks */
	برائے (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(ڈیٹا->undo_file, i, 1,
					       ڈیٹا->keyb);
		اگر (retval)
			memset(ڈیٹا->keyb, 0, ڈیٹا->tdb_ڈیٹا_سائز);
		retval = io_channel_write_blk64(ڈیٹا->undo_file, i, 1,
						ڈیٹا->keyb);
		اگر (retval)
			واپس retval;
		retval = io_channel_flush(ڈیٹا->undo_file);
		اگر (retval)
			واپس retval;
	}
	memset(ڈیٹا->keyb, 0, ڈیٹا->tdb_ڈیٹا_سائز);
	واپس 0;
}

ساکن errcode_t undo_write_tdb(io_channel channel,
				غیرعلامتی طویل طویل بلاک, صحیح گنتی)

{
	صحیح سائز, sz;
	غیرعلامتی طویل طویل بلاک_عدد, backing_blk_عدد;
	errcode_t retval = 0;
	ext2_loff_t انحراف;
	ساخت undo_نجی_ڈیٹا *ڈیٹا;
	غیرعلامتی حرف *read_ptr;
	غیرعلامتی طویل طویل انتہا_بلاک;
	غیرعلامتی طویل طویل ڈیٹا_سائز;
	ساخت undo_کلید *کلید;
	__u32 blk_چکری_جانچ;

	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;

	اگر (ڈیٹا->undo_file == تہی) {
		/*
		 * Transaction database not initialized
		 */
		واپس 0;
	}

	اگر (گنتی == 1)
		سائز = channel->بلاک_سائز;
	ورنہ {
		اگر (گنتی < 0)
			سائز = -گنتی;
		ورنہ
			سائز = گنتی * channel->بلاک_سائز;
	}

	retval = undo_تیاری_tdb(ڈیٹا);
	اگر (retval)
		واپس retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	انحراف = (بلاک * channel->بلاک_سائز) + ڈیٹا->انحراف ;
	بلاک_عدد = انحراف / ڈیٹا->tdb_ڈیٹا_سائز;
	انتہا_بلاک = (انحراف + سائز - 1) / ڈیٹا->tdb_ڈیٹا_سائز;

	جب (بلاک_عدد <= انتہا_بلاک) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		اگر (ext2fs_test_بلاک_bitmap2(ڈیٹا->written_بلاک_map,
						   بلاک_عدد)) {
			/* Try the next block */
			بلاک_عدد++;
			جاری;
		}
		ext2fs_mark_بلاک_bitmap2(ڈیٹا->written_بلاک_map, بلاک_عدد);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		انحراف = بلاک_عدد * ڈیٹا->tdb_ڈیٹا_سائز +
				(ڈیٹا->انحراف % ڈیٹا->tdb_ڈیٹا_سائز);
		backing_blk_عدد = (انحراف - ڈیٹا->انحراف) / channel->بلاک_سائز;

		retval = ext2fs_get_mem(ڈیٹا->tdb_ڈیٹا_سائز, &read_ptr);
		اگر (retval) {
			واپس retval;
		}

		memset(read_ptr, 0, ڈیٹا->tdb_ڈیٹا_سائز);
		actual_سائز = 0;
		اگر ((ڈیٹا->tdb_ڈیٹا_سائز % channel->بلاک_سائز) == 0)
			sz = ڈیٹا->tdb_ڈیٹا_سائز / channel->بلاک_سائز;
		ورنہ
			sz = -ڈیٹا->tdb_ڈیٹا_سائز;
		retval = io_channel_read_blk64(ڈیٹا->real, backing_blk_عدد,
					     sz, read_ptr);
		اگر (retval) {
			اگر (retval != EXT2_ET_SHORT_READ) {
				آزاد(read_ptr);
				واپس retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			ڈیٹا_سائز = actual_سائز;
		} ورنہ {
			ڈیٹا_سائز = ڈیٹا->tdb_ڈیٹا_سائز;
		}
		اگر (ڈیٹا_سائز == 0) {
			آزاد(read_ptr);
			بلاک_عدد++;
			جاری;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       ڈیٹا_سائز, backing_blk_عدد, بلاک, ڈیٹا->tdb_ڈیٹا_سائز);
		اگر ((ڈیٹا_سائز % ڈیٹا->undo_file->بلاک_سائز) == 0)
			sz = ڈیٹا_سائز / ڈیٹا->undo_file->بلاک_سائز;
		ورنہ
			sz = -ڈیٹا_سائز;;
		/* extend this key? */
		اگر (ڈیٹا->کلیدیں_اندر_بلاک) {
			کلید = ڈیٹا->keyb->کلیدیں + ڈیٹا->کلیدیں_اندر_بلاک - 1;
			keysz = ext2fs_le32_مقصد_cpu(کلید->سائز);
		} ورنہ {
			کلید = تہی;
			keysz = 0;
		}
		اگر (کلید != تہی &&
		    (ext2fs_le64_مقصد_cpu(کلید->fsblk) * channel->بلاک_سائز +
		     channel->بلاک_سائز - 1 +
		     keysz) / channel->بلاک_سائز == backing_blk_عدد &&
		    E2_UNDO_MAX_EXTENT_بلاکیں * ڈیٹا->tdb_ڈیٹا_سائز >
		    keysz + ڈیٹا_سائز) {
			blk_چکری_جانچ = ext2fs_le32_مقصد_cpu(کلید->blk_چکری_جانچ);
			blk_چکری_جانچ = ext2fs_crc32c_le(blk_چکری_جانچ, read_ptr, ڈیٹا_سائز);
			کلید->blk_چکری_جانچ = ext2fs_cpu_مقصد_le32(blk_چکری_جانچ);
			کلید->سائز = ext2fs_cpu_مقصد_le32(keysz + ڈیٹا_سائز);
		} ورنہ {
			ڈیٹا->عدد_کلیدیں++;
			کلید = ڈیٹا->keyb->کلیدیں + ڈیٹا->کلیدیں_اندر_بلاک;
			ڈیٹا->کلیدیں_اندر_بلاک++;
			کلید->fsblk = ext2fs_cpu_مقصد_le64(backing_blk_عدد);
			blk_چکری_جانچ = ext2fs_crc32c_le(~0, read_ptr, ڈیٹا_سائز);
			کلید->blk_چکری_جانچ = ext2fs_cpu_مقصد_le32(blk_چکری_جانچ);
			کلید->سائز = ext2fs_cpu_مقصد_le32(ڈیٹا_سائز);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       بلاک_عدد,
		       ڈیٹا->undo_blk_عدد,
		       sz, ڈیٹا->عدد_کلیدیں - 1);
		retval = io_channel_write_blk64(ڈیٹا->undo_file,
					ڈیٹا->undo_blk_عدد, sz, read_ptr);
		اگر (retval) {
			آزاد(read_ptr);
			واپس retval;
		}
		ڈیٹا->undo_blk_عدد++;
		آزاد(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(ڈیٹا, 0);
		اگر (retval)
			واپس retval;

		/* Next block */
		بلاک_عدد++;
	}

	واپس retval;
}

ساکن errcode_t undo_io_read_error(io_channel channel ATTR((غیر_مستعمل)),
				    غیرعلامتی طویل بلاک ATTR((غیر_مستعمل)),
				    صحیح گنتی ATTR((غیر_مستعمل)),
				    خالی *ڈیٹا ATTR((غیر_مستعمل)),
				    size_t سائز ATTR((غیر_مستعمل)),
				    صحیح actual,
				    errcode_t error ATTR((غیر_مستعمل)))
{
	actual_سائز = actual;
	واپس error;
}

ساکن خالی undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ساکن صحیح check_filesystem(ساخت undo_سرخی *hdr, io_channel undo_file,
			    غیرعلامتی صحیح blocksize, blk64_t super_بلاک,
			    io_channel channel)
{
	ساخت ext2_super_بلاک super, *sb;
	حرف *buf;
	__u32 sb_چکری_جانچ;
	errcode_t retval;

	io_channel_سیٹ_blksize(channel, SUPERBLOCK_انحراف);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_سائز, &super);
	اگر (retval)
		واپس retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	اگر (retval)
		واپس retval;
	retval = io_channel_read_blk64(undo_file, super_بلاک,
				       -SUPERBLOCK_سائز, buf);
	اگر (retval)
		جاؤ برآمد;
	sb = (ساخت ext2_super_بلاک *)buf;
	sb->s_magic = ~sb->s_magic;
	اگر (memcmp(&super, buf, حجم(super))) {
		retval = -1;
		جاؤ برآمد;
	}
	sb_چکری_جانچ = ext2fs_crc32c_le(~0, (غیرعلامتی حرف *)buf, SUPERBLOCK_سائز);
	اگر (ext2fs_le32_مقصد_cpu(hdr->sb_چکری_جانچ) != sb_چکری_جانچ) {
		retval = -1;
		جاؤ برآمد;
	}

برآمد:
	ext2fs_free_mem(&buf);
	واپس retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
ساکن errcode_t try_reopen_undo_file(صحیح undo_fd,
				      ساخت undo_نجی_ڈیٹا *ڈیٹا)
{
	ساخت undo_سرخی hdr;
	ساخت undo_کلید *dkey;
	ext2fs_struct_stat statbuf;
	غیرعلامتی صحیح blocksize, fs_blocksize;
	blk64_t super_بلاک, lblk;
	size_t عدد_کلیدیں, کلیدیں_فی_بلاک_2, i;
	__u32 hdr_چکری_جانچ, کلید_چکری_جانچ;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	اگر (retval)
		جاؤ bad_file;
	اگر (statbuf.st_سائز == 0)
		جاؤ برآمد;

	/* check the file header */
	retval = io_channel_read_blk64(ڈیٹا->undo_file, 0, -(صحیح)حجم(hdr),
				       &hdr);
	اگر (retval)
		جاؤ bad_file;

	اگر (memcmp(hdr.magic, E2UNDO_MAGIC,
		    حجم(hdr.magic)))
		جاؤ bad_file;
	hdr_چکری_جانچ = ext2fs_crc32c_le(~0, (غیرعلامتی حرف *)&hdr,
				   حجم(ساخت undo_سرخی) -
				   حجم(__u32));
	اگر (ext2fs_le32_مقصد_cpu(hdr.سرخی_چکری_جانچ) != hdr_چکری_جانچ)
		جاؤ bad_file;
	blocksize = ext2fs_le32_مقصد_cpu(hdr.بلاک_سائز);
	fs_blocksize = ext2fs_le32_مقصد_cpu(hdr.fs_بلاک_سائز);
	اگر (blocksize > E2_UNDO_MAX_بلاک_سائز ||
	    blocksize < E2_UNDO_MIN_بلاک_سائز ||
	    !blocksize || !fs_blocksize)
		جاؤ bad_file;
	super_بلاک = ext2fs_le64_مقصد_cpu(hdr.super_انحراف);
	عدد_کلیدیں = ext2fs_le64_مقصد_cpu(hdr.عدد_کلیدیں);
	io_channel_سیٹ_blksize(ڈیٹا->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	اگر (hdr.f_incompat || hdr.f_rocompat)
		جاؤ bad_file;

	/* Superblock matches this FS? */
	اگر (check_filesystem(&hdr, ڈیٹا->undo_file, blocksize, super_بلاک,
			     ڈیٹا->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		جاؤ برآمد;
	}

	/* Try to set ourselves up */
	ڈیٹا->tdb_ڈیٹا_سائز = blocksize;
	retval = undo_تیاری_tdb(ڈیٹا);
	اگر (retval)
		جاؤ bad_file;
	ڈیٹا->عدد_کلیدیں = عدد_کلیدیں;
	ڈیٹا->super_blk_عدد = super_بلاک;
	ڈیٹا->پہلا_کلید_blk = ext2fs_le64_مقصد_cpu(hdr.کلید_انحراف);

	/* load the written block map */
	کلیدیں_فی_بلاک_2 = کلیدیں_فی_بلاک(ڈیٹا);
	lblk = ڈیٹا->پہلا_کلید_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   عدد_کلیدیں, کلیدیں_فی_بلاک_2, blocksize);
	برائے (i = 0; i < عدد_کلیدیں; i += کلیدیں_فی_بلاک_2) {
		size_t j, max_j;
		__le32 چکری_جانچ;

		ڈیٹا->کلید_blk_عدد = lblk;
		retval = io_channel_read_blk64(ڈیٹا->undo_file,
					       lblk, 1, ڈیٹا->keyb);
		اگر (retval)
			جاؤ bad_کلید_replay;

		/* check keys */
		اگر (ext2fs_le32_مقصد_cpu(ڈیٹا->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			جاؤ bad_کلید_replay;
		}
		چکری_جانچ = ڈیٹا->keyb->چکری_جانچ;
		ڈیٹا->keyb->چکری_جانچ = 0;
		کلید_چکری_جانچ = ext2fs_crc32c_le(~0, (غیرعلامتی حرف *)ڈیٹا->keyb,
					   blocksize);
		اگر (ext2fs_le32_مقصد_cpu(چکری_جانچ) != کلید_چکری_جانچ) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			جاؤ bad_کلید_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = ڈیٹا->عدد_کلیدیں - i;
		اگر (max_j > کلیدیں_فی_بلاک_2)
			max_j = کلیدیں_فی_بلاک_2;
		برائے (j = 0, dkey = ڈیٹا->keyb->کلیدیں;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_مقصد_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t سائز = ext2fs_le32_مقصد_cpu(dkey->سائز);

			ext2fs_mark_بلاک_bitmap_range2(ڈیٹا->written_بلاک_map,
					 undo_blk,
					(سائز + blocksize - 1) / blocksize);
			lblk += (سائز + blocksize - 1) / blocksize;
			ڈیٹا->undo_blk_عدد = lblk;
			ڈیٹا->کلیدیں_اندر_بلاک = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   ڈیٹا->کلید_blk_عدد, ڈیٹا->undo_blk_عدد, ڈیٹا->عدد_کلیدیں,
		   ڈیٹا->کلیدیں_اندر_بلاک);

	ڈیٹا->hdr.state = hdr.state & ~E2_UNDO_STATE_ختم_شدہ;
	ڈیٹا->hdr.f_compat = hdr.f_compat;
	ڈیٹا->hdr.f_incompat = hdr.f_incompat;
	ڈیٹا->hdr.f_rocompat = hdr.f_rocompat;
	واپس retval;

bad_کلید_replay:
	ڈیٹا->کلید_blk_عدد = ڈیٹا->undo_blk_عدد = 0;
	ڈیٹا->کلیدیں_اندر_بلاک = 0;
	ext2fs_free_mem(&ڈیٹا->keyb);
	ext2fs_free_generic_bitmap(ڈیٹا->written_بلاک_map);
	ڈیٹا->tdb_written = 0;
	جاؤ برآمد;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
برآمد:
	واپس retval;
}

ساکن خالی undo_atexit(خالی *p)
{
	ساخت undo_نجی_ڈیٹا *ڈیٹا = p;
	errcode_t err;

	err = write_undo_indexes(ڈیٹا, 1);
	io_channel_close(ڈیٹا->undo_file);

	com_err(ڈیٹا->tdb_file, err, "while force-closing undo file");
}

ساکن errcode_t undo_open(ثابت حرف *name, صحیح flags, io_channel *channel)
{
	io_channel	io = تہی;
	ساخت undo_نجی_ڈیٹا *ڈیٹا = تہی;
	صحیح		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_دھاگے;
	اگر (name == 0)
		واپس EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(ساخت struct_io_channel), &io);
	اگر (retval)
		جاؤ cleanup;
	memset(io, 0, حجم(ساخت struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(ساخت undo_نجی_ڈیٹا), &ڈیٹا);
	اگر (retval)
		جاؤ cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &io->name);
	اگر (retval)
		جاؤ cleanup;

	strcpy(io->name, name);
	io->نجی_ڈیٹا = ڈیٹا;
	io->بلاک_سائز = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(ڈیٹا, 0, حجم(ساخت undo_نجی_ڈیٹا));
	ڈیٹا->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	ڈیٹا->super_blk_عدد = 1;
	ڈیٹا->پہلا_کلید_blk = 2;
	ڈیٹا->undo_blk_عدد = 3;

	اگر (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &ڈیٹا->real);
		اگر (retval)
			جاؤ cleanup;

		ڈیٹا->tdb_file = strdup(tdb_file);
		اگر (ڈیٹا->tdb_file == تہی)
			جاؤ cleanup;
		undo_fd = ext2fs_open_file(ڈیٹا->tdb_file, O_RDWR | O_CREAT,
					   0600);
		اگر (undo_fd < 0)
			جاؤ cleanup;

		retval = undo_io_backing_manager->open(ڈیٹا->tdb_file,
						       IO_FLAG_RW,
						       &ڈیٹا->undo_file);
		اگر (retval)
			جاؤ cleanup;
	} ورنہ {
		ڈیٹا->real = تہی;
		ڈیٹا->undo_file = تہی;
	}

	اگر (ڈیٹا->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (ڈیٹا->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	اگر (ڈیٹا->real)
		undo_err_handler_init(ڈیٹا->real);

	اگر (ڈیٹا->undo_file) {
		retval = try_reopen_undo_file(undo_fd, ڈیٹا);
		اگر (retval)
			جاؤ cleanup;
	}
	retval = ext2fs_جوڑو_exit_fn(undo_atexit, ڈیٹا);
	اگر (retval)
		جاؤ cleanup;

	*channel = io;
	اگر (undo_fd >= 0)
		close(undo_fd);
	واپس retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, ڈیٹا);
	اگر (undo_fd >= 0)
		close(undo_fd);
	اگر (ڈیٹا && ڈیٹا->undo_file)
		io_channel_close(ڈیٹا->undo_file);
	اگر (ڈیٹا && ڈیٹا->tdb_file)
		آزاد(ڈیٹا->tdb_file);
	اگر (ڈیٹا && ڈیٹا->real)
		io_channel_close(ڈیٹا->real);
	اگر (ڈیٹا)
		ext2fs_free_mem(&ڈیٹا);
	اگر (io && io->name)
		ext2fs_free_mem(&io->name);
	اگر (io)
		ext2fs_free_mem(&io);
	واپس retval;
}

ساکن errcode_t undo_close(io_channel channel)
{
	ساخت undo_نجی_ڈیٹا *ڈیٹا;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (--channel->refcount > 0)
		واپس 0;
	/* Before closing write the file system identity */
	اگر (!ext2fs_محفوظ_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		ڈیٹا->hdr.state = ext2fs_cpu_مقصد_le32(E2_UNDO_STATE_ختم_شدہ);
	err = write_undo_indexes(ڈیٹا, 1);
	ext2fs_remove_exit_fn(undo_atexit, ڈیٹا);
	اگر (ڈیٹا->real)
		retval = io_channel_close(ڈیٹا->real);
	اگر (ڈیٹا->tdb_file)
		آزاد(ڈیٹا->tdb_file);
	اگر (ڈیٹا->undo_file)
		io_channel_close(ڈیٹا->undo_file);
	ext2fs_free_mem(&ڈیٹا->keyb);
	اگر (ڈیٹا->written_بلاک_map)
		ext2fs_free_generic_bitmap(ڈیٹا->written_بلاک_map);
	ext2fs_free_mem(&channel->نجی_ڈیٹا);
	اگر (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	اگر (err)
		واپس err;
	واپس retval;
}

ساکن errcode_t undo_سیٹ_blksize(io_channel channel, صحیح blksize)
{
	ساخت undo_نجی_ڈیٹا *ڈیٹا;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (blksize > E2_UNDO_MAX_بلاک_سائز || blksize < E2_UNDO_MIN_بلاک_سائز)
		واپس EXT2_ET_INVALID_دلیل;

	اگر (ڈیٹا->real)
		retval = io_channel_سیٹ_blksize(ڈیٹا->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	اگر (!ڈیٹا->tdb_ڈیٹا_سائز || !ڈیٹا->tdb_written)
		ڈیٹا->tdb_ڈیٹا_سائز = blksize;
	channel->بلاک_سائز = blksize;
	واپس retval;
}

ساکن errcode_t undo_read_blk64(io_channel channel, غیرعلامتی طویل طویل بلاک,
			       صحیح گنتی, خالی *buf)
{
	errcode_t	retval = 0;
	ساخت undo_نجی_ڈیٹا *ڈیٹا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_read_blk64(ڈیٹا->real, بلاک, گنتی, buf);

	واپس retval;
}

ساکن errcode_t undo_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
			       صحیح گنتی, خالی *buf)
{
	واپس undo_read_blk64(channel, بلاک, گنتی, buf);
}

ساکن errcode_t undo_write_blk64(io_channel channel, غیرعلامتی طویل طویل بلاک,
				صحیح گنتی, ثابت خالی *buf)
{
	ساخت undo_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاک, گنتی);
	اگر (retval)
		 واپس retval;
	اگر (ڈیٹا->real)
		retval = io_channel_write_blk64(ڈیٹا->real, بلاک, گنتی, buf);

	واپس retval;
}

ساکن errcode_t undo_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
				صحیح گنتی, ثابت خالی *buf)
{
	واپس undo_write_blk64(channel, بلاک, گنتی, buf);
}

ساکن errcode_t undo_write_byte(io_channel channel, غیرعلامتی طویل انحراف,
				 صحیح سائز, ثابت خالی *buf)
{
	ساخت undo_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	غیرعلامتی طویل blk_عدد, گنتی;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = انحراف + ڈیٹا->انحراف;
	blk_عدد = location/channel->بلاک_سائز;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	گنتی = (سائز + (location % channel->بلاک_سائز) +
			channel->بلاک_سائز  -1)/channel->بلاک_سائز;
	retval = undo_write_tdb(channel, blk_عدد, گنتی);
	اگر (retval)
		واپس retval;
	اگر (ڈیٹا->real && ڈیٹا->real->manager->write_byte)
		retval = io_channel_write_byte(ڈیٹا->real, انحراف, سائز, buf);

	واپس retval;
}

ساکن errcode_t undo_discard(io_channel channel, غیرعلامتی طویل طویل بلاک,
			      غیرعلامتی طویل طویل گنتی)
{
	ساخت undo_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;
	صحیح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (گنتی > زیادہ_سے_زیادہ_عدد)
		واپس EXT2_ET_UNIMPLEMENTED;
	icount = گنتی;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاک, icount);
	اگر (retval)
		واپس retval;
	اگر (ڈیٹا->real)
		retval = io_channel_discard(ڈیٹا->real, بلاک, گنتی);

	واپس retval;
}

ساکن errcode_t undo_zeroout(io_channel channel, غیرعلامتی طویل طویل بلاک,
			      غیرعلامتی طویل طویل گنتی)
{
	ساخت undo_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;
	صحیح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (گنتی > زیادہ_سے_زیادہ_عدد)
		واپس EXT2_ET_UNIMPLEMENTED;
	icount = گنتی;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاک, icount);
	اگر (retval)
		واپس retval;
	اگر (ڈیٹا->real)
		retval = io_channel_zeroout(ڈیٹا->real, بلاک, گنتی);

	واپس retval;
}

ساکن errcode_t undo_cache_readahead(io_channel channel,
				      غیرعلامتی طویل طویل بلاک,
				      غیرعلامتی طویل طویل گنتی)
{
	ساخت undo_نجی_ڈیٹا *ڈیٹا;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_cache_readahead(ڈیٹا->real, بلاک, گنتی);

	واپس retval;
}

/*
 * Flush data buffers to disk.
 */
ساکن errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	ساخت undo_نجی_ڈیٹا *ڈیٹا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = io_channel_flush(ڈیٹا->real);

	واپس retval;
}

ساکن errcode_t undo_سیٹ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	errcode_t	retval = 0;
	ساخت undo_نجی_ڈیٹا *ڈیٹا;
	غیرعلامتی طویل tmp;
	حرف *انتہا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (!strcmp(option, "tdb_data_size")) {
		اگر (!arg)
			واپس EXT2_ET_INVALID_دلیل;

		tmp = strtoul(arg, &انتہا, 0);
		اگر (*انتہا)
			واپس EXT2_ET_INVALID_دلیل;
		اگر (tmp > E2_UNDO_MAX_بلاک_سائز || tmp < E2_UNDO_MIN_بلاک_سائز)
			واپس EXT2_ET_INVALID_دلیل;
		اگر (!ڈیٹا->tdb_ڈیٹا_سائز || !ڈیٹا->tdb_written) {
			ڈیٹا->tdb_written = -1;
			ڈیٹا->tdb_ڈیٹا_سائز = tmp;
		}
		واپس 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	اگر (ڈیٹا->real && ڈیٹا->real->manager->سیٹ_option) {
		retval = ڈیٹا->real->manager->سیٹ_option(ڈیٹا->real,
							option, arg);
	}
	اگر (!retval && !strcmp(option, "offset")) {
		اگر (!arg)
			واپس EXT2_ET_INVALID_دلیل;

		tmp = strtoul(arg, &انتہا, 0);
		اگر (*انتہا)
			واپس EXT2_ET_INVALID_دلیل;
		ڈیٹا->انحراف = tmp;
	}
	واپس retval;
}

ساکن errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	ساخت undo_نجی_ڈیٹا *ڈیٹا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	ڈیٹا = (ساخت undo_نجی_ڈیٹا *) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(ڈیٹا, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	اگر (ڈیٹا->real)
		retval = (ڈیٹا->real->manager->get_stats)(ڈیٹا->real, stats);

	واپس retval;
}

ساکن ساخت struct_io_manager struct_undo_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Undo I/O Manager",
	.open		= undo_open,
	.close		= undo_close,
	.سیٹ_blksize	= undo_سیٹ_blksize,
	.read_blk	= undo_read_blk,
	.write_blk	= undo_write_blk,
	.flush		= undo_flush,
	.write_byte	= undo_write_byte,
	.سیٹ_option	= undo_سیٹ_option,
	.get_stats	= undo_get_stats,
	.read_blk64	= undo_read_blk64,
	.write_blk64	= undo_write_blk64,
	.discard	= undo_discard,
	.zeroout	= undo_zeroout,
	.cache_readahead	= undo_cache_readahead,
};

io_manager undo_io_manager = &struct_undo_manager;
