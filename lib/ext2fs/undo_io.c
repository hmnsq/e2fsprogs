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

#جے_بنا_تعریف LARGEFILE_سروت
#تعریف_کرو LARGEFILE_سروت
#جے_انت
#جے_بنا_تعریف LARGEFILE64_سروت
#تعریف_کرو LARGEFILE64_سروت
#جے_انت

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#جے_تعریف_شدہ __linux__
#ضمن <sys/utsname.h>
#جے_انت
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#إذا HAVE_SYS_RESOURCE_H
#ضمن <sys/resource.h>
#جے_انت
#ضمن <limits.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#جے_تعریف_شدہ __GNUC__
#تعریف_کرو ATTR(x) __attribute__(x)
#وإلا
#تعریف_کرو ATTR(x)
#جے_انت

#تعریف_رد DEBUG

#جے_تعریف_شدہ DEBUG
# define dbg_printf(f, a...)  افعل {لکھو(f, ## a); fflush(stdout); } طالما (0)
#وإلا
# define dbg_printf(f, a...)
#جے_انت

/*
 * For checking structure magic numbers...
 */

#تعریف_کرو EXT2_CHECK_MAGIC(بنية, کوڈ) \
	  إذا ((بنية)->magic != (کوڈ)) أرجع (کوڈ)
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

#تعریف_کرو E2_UNDO_STATE_مکمل	0x1	/* undo file is complete */

#تعریف_کرو E2_UNDO_MIN_بلاک_ناپ	1024	/* undo blocks are no less than 1KB */
#تعریف_کرو E2_UNDO_MAX_بلاک_ناپ	1048576	/* undo blocks are no more than 1MB */

بنية undo_سرخی {
	حرف magic[8];		/* "E2UNDO02" */
	__le64 عدد_کلیدیں;	/* how many keys? */
	__le64 super_وتھ;	/* where in the file is the superblock copy? */
	__le64 کلید_وتھ;	/* where do the key/data block chunks start? */
	__le32 بلاک_ناپ;	/* block size of the undo file */
	__le32 fs_بلاک_ناپ;	/* block size of the target device */
	__le32 sb_چکری_جانچ;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_وتھ;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 سرخی_چکری_جانچ;	/* crc32c of this header (but not this field) */
};

#تعریف_کرو E2_UNDO_MAX_EXTENT_بلاکاں	512	/* max extent size, in blocks */

بنية undo_کلید {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_چکری_جانچ;		/* crc32c of the block */
	__le32 ناپ;		/* how many bytes in this block? */
};

بنية undo_کلید_بلاک {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 چکری_جانچ;		/* block checksum */
	__le64 reserved;	/* zero */

#إذا __STDC_VERSION__ >= 199901L
	بنية undo_کلید کلیدیں[];		/* keys, which come immediately after */
#وإلا
	بنية undo_کلید کلیدیں[0];	/* keys, which come immediately after */
#جے_انت
};

بنية undo_نجی_انکڑے {
	صحيح	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_عدد;			/* next free block */
	blk64_t کلید_blk_عدد;			/* current key block location */
	blk64_t super_blk_عدد;			/* superblock location */
	blk64_t پہلا_کلید_blk;			/* first key block location */
	بنية undo_کلید_بلاک *keyb;
	size_t عدد_کلیدیں, کلیدیں_وچ_بلاک;

	/* The backing io channel */
	io_channel real;

	بنانشانی لما لما tdb_انکڑے_ناپ;
	صحيح tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t وتھ;

	ext2fs_بلاک_bitmap written_بلاک_map;
	بنية struct_ext2_filsys fake_fs;
	حرف *tdb_file;
	بنية undo_سرخی hdr;
};
#تعریف_کرو کلیدیں_فی_بلاک(d) (((d)->tdb_انکڑے_ناپ / حجم(بنية undo_کلید)) - 1)

#تعریف_کرو E2_UNDO_FEATURE_COMPAT_FS_وتھ 0x1	/* the filesystem offset */

ساكن مضمن فراغ e2undo_سیٹ_feature_fs_وتھ(بنية undo_سرخی *سرخی) {
	سرخی->f_compat |= ext2fs_le32_منزل_cpu(E2_UNDO_FEATURE_COMPAT_FS_وتھ);
}

ساكن مضمن فراغ e2undo_clear_feature_fs_وتھ(بنية undo_سرخی *سرخی) {
	سرخی->f_compat &= ~ext2fs_le32_منزل_cpu(E2_UNDO_FEATURE_COMPAT_FS_وتھ);
}

ساكن io_manager undo_io_backing_manager;
ساكن حرف *tdb_file;
ساكن صحيح actual_ناپ;

errcode_t سیٹ_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	أرجع 0;
}

errcode_t سیٹ_undo_io_backup_file(حرف *file_name)
{
	tdb_file = strdup(file_name);

	إذا (tdb_file == عدم) {
		أرجع EXT2_ET_NO_حافظہ;
	}

	أرجع 0;
}

ساكن errcode_t write_undo_indexes(بنية undo_نجی_انکڑے *انکڑے, صحيح flush)
{
	errcode_t retval;
	بنية ext2_super_بلاک super;
	io_channel channel;
	صحيح بلاک_ناپ;
	__u32 sb_چکری_جانچ, hdr_چکری_جانچ;

	/* Spit out a key block, if there's any data */
	إذا (انکڑے->کلیدیں_وچ_بلاک) {
		انکڑے->keyb->magic = ext2fs_cpu_منزل_le32(KEYBLOCK_MAGIC);
		انکڑے->keyb->چکری_جانچ = 0;
		انکڑے->keyb->چکری_جانچ = ext2fs_cpu_منزل_le32(
					 ext2fs_crc32c_le(~0,
					 (بنانشانی حرف *)انکڑے->keyb,
					 انکڑے->tdb_انکڑے_ناپ));
		dbg_printf("Writing keyblock to blk %llu\n", انکڑے->کلید_blk_عدد);
		retval = io_channel_write_blk64(انکڑے->undo_file,
						انکڑے->کلید_blk_عدد,
						1, انکڑے->keyb);
		إذا (retval)
			أرجع retval;
		/* Move on to the next key block if it's full. */
		إذا (انکڑے->کلیدیں_وچ_بلاک == کلیدیں_فی_بلاک(انکڑے)) {
			memset(انکڑے->keyb, 0, انکڑے->tdb_انکڑے_ناپ);
			انکڑے->کلیدیں_وچ_بلاک = 0;
			انکڑے->کلید_blk_عدد = انکڑے->undo_blk_عدد;
			انکڑے->undo_blk_عدد++;
		}
	}

	/* Prepare superblock for write */
	channel = انکڑے->real;
	بلاک_ناپ = channel->بلاک_ناپ;

	io_channel_سیٹ_blksize(channel, SUPERBLOCK_وتھ);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_ناپ, &super);
	إذا (retval)
		اذهب err_باہرلا;
	sb_چکری_جانچ = ext2fs_crc32c_le(~0, (بنانشانی حرف *)&super, SUPERBLOCK_ناپ);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	حافظہ_نقل(انکڑے->hdr.magic, E2UNDO_MAGIC, حجم(انکڑے->hdr.magic));
	انکڑے->hdr.عدد_کلیدیں = ext2fs_cpu_منزل_le64(انکڑے->عدد_کلیدیں);
	انکڑے->hdr.super_وتھ = ext2fs_cpu_منزل_le64(انکڑے->super_blk_عدد);
	انکڑے->hdr.کلید_وتھ = ext2fs_cpu_منزل_le64(انکڑے->پہلا_کلید_blk);
	انکڑے->hdr.fs_بلاک_ناپ = ext2fs_cpu_منزل_le32(بلاک_ناپ);
	انکڑے->hdr.sb_چکری_جانچ = ext2fs_cpu_منزل_le32(sb_چکری_جانچ);
	انکڑے->hdr.fs_وتھ = ext2fs_cpu_منزل_le64(انکڑے->وتھ);
	إذا (انکڑے->وتھ)
		e2undo_سیٹ_feature_fs_وتھ(&انکڑے->hdr);
	وإلا
		e2undo_clear_feature_fs_وتھ(&انکڑے->hdr);
	hdr_چکری_جانچ = ext2fs_crc32c_le(~0, (بنانشانی حرف *)&انکڑے->hdr,
				   حجم(انکڑے->hdr) -
				   حجم(انکڑے->hdr.سرخی_چکری_جانچ));
	انکڑے->hdr.سرخی_چکری_جانچ = ext2fs_cpu_منزل_le32(hdr_چکری_جانچ);
	retval = io_channel_write_blk64(انکڑے->undo_file, 0,
					-(صحيح)حجم(انکڑے->hdr),
					&انکڑے->hdr);
	إذا (retval)
		اذهب err_باہرلا;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", انکڑے->super_blk_عدد);
	retval = io_channel_write_blk64(انکڑے->undo_file, انکڑے->super_blk_عدد,
					-SUPERBLOCK_ناپ, &super);
	إذا (retval)
		اذهب err_باہرلا;

	إذا (flush)
		retval = io_channel_flush(انکڑے->undo_file);
err_باہرلا:
	io_channel_سیٹ_blksize(channel, بلاک_ناپ);
	أرجع retval;
}

ساكن errcode_t undo_تیاری_tdb(بنية undo_نجی_انکڑے *انکڑے)
{
	صحيح i;
	errcode_t retval;

	إذا (انکڑے->tdb_written == 1)
		أرجع 0;

	انکڑے->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&انکڑے->fake_fs, 0, حجم(انکڑے->fake_fs));
	انکڑے->fake_fs.blocksize = انکڑے->tdb_انکڑے_ناپ;
	retval = ext2fs_alloc_generic_bmap(&انکڑے->fake_fs,
				EXT2_ET_MAGIC_بلاک_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &انکڑے->written_بلاک_map);
	إذا (retval)
		أرجع retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(انکڑے->tdb_انکڑے_ناپ, &انکڑے->keyb);
	إذا (retval)
		أرجع retval;
	انکڑے->کلید_blk_عدد = انکڑے->پہلا_کلید_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", انکڑے->tdb_انکڑے_ناپ);
	dbg_printf("Keys per block %llu\n", کلیدیں_فی_بلاک(انکڑے));
	انکڑے->hdr.بلاک_ناپ = ext2fs_cpu_منزل_le32(انکڑے->tdb_انکڑے_ناپ);
	io_channel_سیٹ_blksize(انکڑے->undo_file, انکڑے->tdb_انکڑے_ناپ);

	/* Ensure that we have space for header blocks */
	لكل (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(انکڑے->undo_file, i, 1,
					       انکڑے->keyb);
		إذا (retval)
			memset(انکڑے->keyb, 0, انکڑے->tdb_انکڑے_ناپ);
		retval = io_channel_write_blk64(انکڑے->undo_file, i, 1,
						انکڑے->keyb);
		إذا (retval)
			أرجع retval;
		retval = io_channel_flush(انکڑے->undo_file);
		إذا (retval)
			أرجع retval;
	}
	memset(انکڑے->keyb, 0, انکڑے->tdb_انکڑے_ناپ);
	أرجع 0;
}

ساكن errcode_t undo_write_tdb(io_channel channel,
				بنانشانی لما لما بلاک, صحيح گنتی)

{
	صحيح ناپ, sz;
	بنانشانی لما لما بلاک_عدد, backing_blk_عدد;
	errcode_t retval = 0;
	ext2_loff_t وتھ;
	بنية undo_نجی_انکڑے *انکڑے;
	بنانشانی حرف *read_ptr;
	بنانشانی لما لما سرا_بلاک;
	بنانشانی لما لما انکڑے_ناپ;
	بنية undo_کلید *کلید;
	__u32 blk_چکری_جانچ;

	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;

	إذا (انکڑے->undo_file == عدم) {
		/*
		 * Transaction database not initialized
		 */
		أرجع 0;
	}

	إذا (گنتی == 1)
		ناپ = channel->بلاک_ناپ;
	وإلا {
		إذا (گنتی < 0)
			ناپ = -گنتی;
		وإلا
			ناپ = گنتی * channel->بلاک_ناپ;
	}

	retval = undo_تیاری_tdb(انکڑے);
	إذا (retval)
		أرجع retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	وتھ = (بلاک * channel->بلاک_ناپ) + انکڑے->وتھ ;
	بلاک_عدد = وتھ / انکڑے->tdb_انکڑے_ناپ;
	سرا_بلاک = (وتھ + ناپ - 1) / انکڑے->tdb_انکڑے_ناپ;

	طالما (بلاک_عدد <= سرا_بلاک) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		إذا (ext2fs_test_بلاک_bitmap2(انکڑے->written_بلاک_map,
						   بلاک_عدد)) {
			/* Try the next block */
			بلاک_عدد++;
			استمر;
		}
		ext2fs_mark_بلاک_bitmap2(انکڑے->written_بلاک_map, بلاک_عدد);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		وتھ = بلاک_عدد * انکڑے->tdb_انکڑے_ناپ +
				(انکڑے->وتھ % انکڑے->tdb_انکڑے_ناپ);
		backing_blk_عدد = (وتھ - انکڑے->وتھ) / channel->بلاک_ناپ;

		retval = ext2fs_get_mem(انکڑے->tdb_انکڑے_ناپ, &read_ptr);
		إذا (retval) {
			أرجع retval;
		}

		memset(read_ptr, 0, انکڑے->tdb_انکڑے_ناپ);
		actual_ناپ = 0;
		إذا ((انکڑے->tdb_انکڑے_ناپ % channel->بلاک_ناپ) == 0)
			sz = انکڑے->tdb_انکڑے_ناپ / channel->بلاک_ناپ;
		وإلا
			sz = -انکڑے->tdb_انکڑے_ناپ;
		retval = io_channel_read_blk64(انکڑے->real, backing_blk_عدد,
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
			انکڑے_ناپ = actual_ناپ;
		} وإلا {
			انکڑے_ناپ = انکڑے->tdb_انکڑے_ناپ;
		}
		إذا (انکڑے_ناپ == 0) {
			حرر(read_ptr);
			بلاک_عدد++;
			استمر;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       انکڑے_ناپ, backing_blk_عدد, بلاک, انکڑے->tdb_انکڑے_ناپ);
		إذا ((انکڑے_ناپ % انکڑے->undo_file->بلاک_ناپ) == 0)
			sz = انکڑے_ناپ / انکڑے->undo_file->بلاک_ناپ;
		وإلا
			sz = -انکڑے_ناپ;;
		/* extend this key? */
		إذا (انکڑے->کلیدیں_وچ_بلاک) {
			کلید = انکڑے->keyb->کلیدیں + انکڑے->کلیدیں_وچ_بلاک - 1;
			keysz = ext2fs_le32_منزل_cpu(کلید->ناپ);
		} وإلا {
			کلید = عدم;
			keysz = 0;
		}
		إذا (کلید != عدم &&
		    (ext2fs_le64_منزل_cpu(کلید->fsblk) * channel->بلاک_ناپ +
		     channel->بلاک_ناپ - 1 +
		     keysz) / channel->بلاک_ناپ == backing_blk_عدد &&
		    E2_UNDO_MAX_EXTENT_بلاکاں * انکڑے->tdb_انکڑے_ناپ >
		    keysz + انکڑے_ناپ) {
			blk_چکری_جانچ = ext2fs_le32_منزل_cpu(کلید->blk_چکری_جانچ);
			blk_چکری_جانچ = ext2fs_crc32c_le(blk_چکری_جانچ, read_ptr, انکڑے_ناپ);
			کلید->blk_چکری_جانچ = ext2fs_cpu_منزل_le32(blk_چکری_جانچ);
			کلید->ناپ = ext2fs_cpu_منزل_le32(keysz + انکڑے_ناپ);
		} وإلا {
			انکڑے->عدد_کلیدیں++;
			کلید = انکڑے->keyb->کلیدیں + انکڑے->کلیدیں_وچ_بلاک;
			انکڑے->کلیدیں_وچ_بلاک++;
			کلید->fsblk = ext2fs_cpu_منزل_le64(backing_blk_عدد);
			blk_چکری_جانچ = ext2fs_crc32c_le(~0, read_ptr, انکڑے_ناپ);
			کلید->blk_چکری_جانچ = ext2fs_cpu_منزل_le32(blk_چکری_جانچ);
			کلید->ناپ = ext2fs_cpu_منزل_le32(انکڑے_ناپ);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       بلاک_عدد,
		       انکڑے->undo_blk_عدد,
		       sz, انکڑے->عدد_کلیدیں - 1);
		retval = io_channel_write_blk64(انکڑے->undo_file,
					انکڑے->undo_blk_عدد, sz, read_ptr);
		إذا (retval) {
			حرر(read_ptr);
			أرجع retval;
		}
		انکڑے->undo_blk_عدد++;
		حرر(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(انکڑے, 0);
		إذا (retval)
			أرجع retval;

		/* Next block */
		بلاک_عدد++;
	}

	أرجع retval;
}

ساكن errcode_t undo_io_read_error(io_channel channel ATTR((بن_ورتیا)),
				    بنانشانی لما بلاک ATTR((بن_ورتیا)),
				    صحيح گنتی ATTR((بن_ورتیا)),
				    فراغ *انکڑے ATTR((بن_ورتیا)),
				    size_t ناپ ATTR((بن_ورتیا)),
				    صحيح actual,
				    errcode_t error ATTR((بن_ورتیا)))
{
	actual_ناپ = actual;
	أرجع error;
}

ساكن فراغ undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

ساكن صحيح check_filesystem(بنية undo_سرخی *hdr, io_channel undo_file,
			    بنانشانی صحيح blocksize, blk64_t super_بلاک,
			    io_channel channel)
{
	بنية ext2_super_بلاک super, *sb;
	حرف *buf;
	__u32 sb_چکری_جانچ;
	errcode_t retval;

	io_channel_سیٹ_blksize(channel, SUPERBLOCK_وتھ);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_ناپ, &super);
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
				       -SUPERBLOCK_ناپ, buf);
	إذا (retval)
		اذهب باہرلا;
	sb = (بنية ext2_super_بلاک *)buf;
	sb->s_magic = ~sb->s_magic;
	إذا (memcmp(&super, buf, حجم(super))) {
		retval = -1;
		اذهب باہرلا;
	}
	sb_چکری_جانچ = ext2fs_crc32c_le(~0, (بنانشانی حرف *)buf, SUPERBLOCK_ناپ);
	إذا (ext2fs_le32_منزل_cpu(hdr->sb_چکری_جانچ) != sb_چکری_جانچ) {
		retval = -1;
		اذهب باہرلا;
	}

باہرلا:
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
				      بنية undo_نجی_انکڑے *انکڑے)
{
	بنية undo_سرخی hdr;
	بنية undo_کلید *dkey;
	ext2fs_struct_stat statbuf;
	بنانشانی صحيح blocksize, fs_blocksize;
	blk64_t super_بلاک, lblk;
	size_t عدد_کلیدیں, کلیدیں_فی_بلاک_2, i;
	__u32 hdr_چکری_جانچ, کلید_چکری_جانچ;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	إذا (retval)
		اذهب bad_file;
	إذا (statbuf.st_ناپ == 0)
		اذهب باہرلا;

	/* check the file header */
	retval = io_channel_read_blk64(انکڑے->undo_file, 0, -(صحيح)حجم(hdr),
				       &hdr);
	إذا (retval)
		اذهب bad_file;

	إذا (memcmp(hdr.magic, E2UNDO_MAGIC,
		    حجم(hdr.magic)))
		اذهب bad_file;
	hdr_چکری_جانچ = ext2fs_crc32c_le(~0, (بنانشانی حرف *)&hdr,
				   حجم(بنية undo_سرخی) -
				   حجم(__u32));
	إذا (ext2fs_le32_منزل_cpu(hdr.سرخی_چکری_جانچ) != hdr_چکری_جانچ)
		اذهب bad_file;
	blocksize = ext2fs_le32_منزل_cpu(hdr.بلاک_ناپ);
	fs_blocksize = ext2fs_le32_منزل_cpu(hdr.fs_بلاک_ناپ);
	إذا (blocksize > E2_UNDO_MAX_بلاک_ناپ ||
	    blocksize < E2_UNDO_MIN_بلاک_ناپ ||
	    !blocksize || !fs_blocksize)
		اذهب bad_file;
	super_بلاک = ext2fs_le64_منزل_cpu(hdr.super_وتھ);
	عدد_کلیدیں = ext2fs_le64_منزل_cpu(hdr.عدد_کلیدیں);
	io_channel_سیٹ_blksize(انکڑے->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	إذا (hdr.f_incompat || hdr.f_rocompat)
		اذهب bad_file;

	/* Superblock matches this FS? */
	إذا (check_filesystem(&hdr, انکڑے->undo_file, blocksize, super_بلاک,
			     انکڑے->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		اذهب باہرلا;
	}

	/* Try to set ourselves up */
	انکڑے->tdb_انکڑے_ناپ = blocksize;
	retval = undo_تیاری_tdb(انکڑے);
	إذا (retval)
		اذهب bad_file;
	انکڑے->عدد_کلیدیں = عدد_کلیدیں;
	انکڑے->super_blk_عدد = super_بلاک;
	انکڑے->پہلا_کلید_blk = ext2fs_le64_منزل_cpu(hdr.کلید_وتھ);

	/* load the written block map */
	کلیدیں_فی_بلاک_2 = کلیدیں_فی_بلاک(انکڑے);
	lblk = انکڑے->پہلا_کلید_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   عدد_کلیدیں, کلیدیں_فی_بلاک_2, blocksize);
	لكل (i = 0; i < عدد_کلیدیں; i += کلیدیں_فی_بلاک_2) {
		size_t j, max_j;
		__le32 چکری_جانچ;

		انکڑے->کلید_blk_عدد = lblk;
		retval = io_channel_read_blk64(انکڑے->undo_file,
					       lblk, 1, انکڑے->keyb);
		إذا (retval)
			اذهب bad_کلید_replay;

		/* check keys */
		إذا (ext2fs_le32_منزل_cpu(انکڑے->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			اذهب bad_کلید_replay;
		}
		چکری_جانچ = انکڑے->keyb->چکری_جانچ;
		انکڑے->keyb->چکری_جانچ = 0;
		کلید_چکری_جانچ = ext2fs_crc32c_le(~0, (بنانشانی حرف *)انکڑے->keyb,
					   blocksize);
		إذا (ext2fs_le32_منزل_cpu(چکری_جانچ) != کلید_چکری_جانچ) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			اذهب bad_کلید_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = انکڑے->عدد_کلیدیں - i;
		إذا (max_j > کلیدیں_فی_بلاک_2)
			max_j = کلیدیں_فی_بلاک_2;
		لكل (j = 0, dkey = انکڑے->keyb->کلیدیں;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_منزل_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t ناپ = ext2fs_le32_منزل_cpu(dkey->ناپ);

			ext2fs_mark_بلاک_bitmap_range2(انکڑے->written_بلاک_map,
					 undo_blk,
					(ناپ + blocksize - 1) / blocksize);
			lblk += (ناپ + blocksize - 1) / blocksize;
			انکڑے->undo_blk_عدد = lblk;
			انکڑے->کلیدیں_وچ_بلاک = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   انکڑے->کلید_blk_عدد, انکڑے->undo_blk_عدد, انکڑے->عدد_کلیدیں,
		   انکڑے->کلیدیں_وچ_بلاک);

	انکڑے->hdr.state = hdr.state & ~E2_UNDO_STATE_مکمل;
	انکڑے->hdr.f_compat = hdr.f_compat;
	انکڑے->hdr.f_incompat = hdr.f_incompat;
	انکڑے->hdr.f_rocompat = hdr.f_rocompat;
	أرجع retval;

bad_کلید_replay:
	انکڑے->کلید_blk_عدد = انکڑے->undo_blk_عدد = 0;
	انکڑے->کلیدیں_وچ_بلاک = 0;
	ext2fs_free_mem(&انکڑے->keyb);
	ext2fs_free_generic_bitmap(انکڑے->written_بلاک_map);
	انکڑے->tdb_written = 0;
	اذهب باہرلا;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
باہرلا:
	أرجع retval;
}

ساكن فراغ undo_atexit(فراغ *p)
{
	بنية undo_نجی_انکڑے *انکڑے = p;
	errcode_t err;

	err = write_undo_indexes(انکڑے, 1);
	io_channel_close(انکڑے->undo_file);

	com_err(انکڑے->tdb_file, err, "while force-closing undo file");
}

ساكن errcode_t undo_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel	io = عدم;
	بنية undo_نجی_انکڑے *انکڑے = عدم;
	صحيح		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_دھاگے;
	إذا (name == 0)
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(حجم(بنية struct_io_channel), &io);
	إذا (retval)
		اذهب cleanup;
	memset(io, 0, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(حجم(بنية undo_نجی_انکڑے), &انکڑے);
	إذا (retval)
		اذهب cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &io->name);
	إذا (retval)
		اذهب cleanup;

	strcpy(io->name, name);
	io->نجی_انکڑے = انکڑے;
	io->بلاک_ناپ = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(انکڑے, 0, حجم(بنية undo_نجی_انکڑے));
	انکڑے->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	انکڑے->super_blk_عدد = 1;
	انکڑے->پہلا_کلید_blk = 2;
	انکڑے->undo_blk_عدد = 3;

	إذا (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &انکڑے->real);
		إذا (retval)
			اذهب cleanup;

		انکڑے->tdb_file = strdup(tdb_file);
		إذا (انکڑے->tdb_file == عدم)
			اذهب cleanup;
		undo_fd = ext2fs_open_file(انکڑے->tdb_file, O_RDWR | O_CREAT,
					   0600);
		إذا (undo_fd < 0)
			اذهب cleanup;

		retval = undo_io_backing_manager->open(انکڑے->tdb_file,
						       IO_FLAG_RW,
						       &انکڑے->undo_file);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		انکڑے->real = عدم;
		انکڑے->undo_file = عدم;
	}

	إذا (انکڑے->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (انکڑے->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	إذا (انکڑے->real)
		undo_err_handler_init(انکڑے->real);

	إذا (انکڑے->undo_file) {
		retval = try_reopen_undo_file(undo_fd, انکڑے);
		إذا (retval)
			اذهب cleanup;
	}
	retval = ext2fs_جوڑو_exit_fn(undo_atexit, انکڑے);
	إذا (retval)
		اذهب cleanup;

	*channel = io;
	إذا (undo_fd >= 0)
		close(undo_fd);
	أرجع retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, انکڑے);
	إذا (undo_fd >= 0)
		close(undo_fd);
	إذا (انکڑے && انکڑے->undo_file)
		io_channel_close(انکڑے->undo_file);
	إذا (انکڑے && انکڑے->tdb_file)
		حرر(انکڑے->tdb_file);
	إذا (انکڑے && انکڑے->real)
		io_channel_close(انکڑے->real);
	إذا (انکڑے)
		ext2fs_free_mem(&انکڑے);
	إذا (io && io->name)
		ext2fs_free_mem(&io->name);
	إذا (io)
		ext2fs_free_mem(&io);
	أرجع retval;
}

ساكن errcode_t undo_close(io_channel channel)
{
	بنية undo_نجی_انکڑے *انکڑے;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (--channel->refcount > 0)
		أرجع 0;
	/* Before closing write the file system identity */
	إذا (!ext2fs_محفوظ_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		انکڑے->hdr.state = ext2fs_cpu_منزل_le32(E2_UNDO_STATE_مکمل);
	err = write_undo_indexes(انکڑے, 1);
	ext2fs_remove_exit_fn(undo_atexit, انکڑے);
	إذا (انکڑے->real)
		retval = io_channel_close(انکڑے->real);
	إذا (انکڑے->tdb_file)
		حرر(انکڑے->tdb_file);
	إذا (انکڑے->undo_file)
		io_channel_close(انکڑے->undo_file);
	ext2fs_free_mem(&انکڑے->keyb);
	إذا (انکڑے->written_بلاک_map)
		ext2fs_free_generic_bitmap(انکڑے->written_بلاک_map);
	ext2fs_free_mem(&channel->نجی_انکڑے);
	إذا (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	إذا (err)
		أرجع err;
	أرجع retval;
}

ساكن errcode_t undo_سیٹ_blksize(io_channel channel, صحيح blksize)
{
	بنية undo_نجی_انکڑے *انکڑے;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (blksize > E2_UNDO_MAX_بلاک_ناپ || blksize < E2_UNDO_MIN_بلاک_ناپ)
		أرجع EXT2_ET_INVALID_دلیل;

	إذا (انکڑے->real)
		retval = io_channel_سیٹ_blksize(انکڑے->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	إذا (!انکڑے->tdb_انکڑے_ناپ || !انکڑے->tdb_written)
		انکڑے->tdb_انکڑے_ناپ = blksize;
	channel->بلاک_ناپ = blksize;
	أرجع retval;
}

ساكن errcode_t undo_read_blk64(io_channel channel, بنانشانی لما لما بلاک,
			       صحيح گنتی, فراغ *buf)
{
	errcode_t	retval = 0;
	بنية undo_نجی_انکڑے *انکڑے;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_read_blk64(انکڑے->real, بلاک, گنتی, buf);

	أرجع retval;
}

ساكن errcode_t undo_read_blk(io_channel channel, بنانشانی لما بلاک,
			       صحيح گنتی, فراغ *buf)
{
	أرجع undo_read_blk64(channel, بلاک, گنتی, buf);
}

ساكن errcode_t undo_write_blk64(io_channel channel, بنانشانی لما لما بلاک,
				صحيح گنتی, ثابت فراغ *buf)
{
	بنية undo_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاک, گنتی);
	إذا (retval)
		 أرجع retval;
	إذا (انکڑے->real)
		retval = io_channel_write_blk64(انکڑے->real, بلاک, گنتی, buf);

	أرجع retval;
}

ساكن errcode_t undo_write_blk(io_channel channel, بنانشانی لما بلاک,
				صحيح گنتی, ثابت فراغ *buf)
{
	أرجع undo_write_blk64(channel, بلاک, گنتی, buf);
}

ساكن errcode_t undo_write_byte(io_channel channel, بنانشانی لما وتھ,
				 صحيح ناپ, ثابت فراغ *buf)
{
	بنية undo_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	بنانشانی لما blk_عدد, گنتی;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = وتھ + انکڑے->وتھ;
	blk_عدد = location/channel->بلاک_ناپ;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	گنتی = (ناپ + (location % channel->بلاک_ناپ) +
			channel->بلاک_ناپ  -1)/channel->بلاک_ناپ;
	retval = undo_write_tdb(channel, blk_عدد, گنتی);
	إذا (retval)
		أرجع retval;
	إذا (انکڑے->real && انکڑے->real->manager->write_byte)
		retval = io_channel_write_byte(انکڑے->real, وتھ, ناپ, buf);

	أرجع retval;
}

ساكن errcode_t undo_discard(io_channel channel, بنانشانی لما لما بلاک,
			      بنانشانی لما لما گنتی)
{
	بنية undo_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;
	صحيح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (گنتی > ودھ_توں_ودھ_عدد)
		أرجع EXT2_ET_UNIMPLEMENTED;
	icount = گنتی;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاک, icount);
	إذا (retval)
		أرجع retval;
	إذا (انکڑے->real)
		retval = io_channel_discard(انکڑے->real, بلاک, گنتی);

	أرجع retval;
}

ساكن errcode_t undo_zeroout(io_channel channel, بنانشانی لما لما بلاک,
			      بنانشانی لما لما گنتی)
{
	بنية undo_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;
	صحيح icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (گنتی > ودھ_توں_ودھ_عدد)
		أرجع EXT2_ET_UNIMPLEMENTED;
	icount = گنتی;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, بلاک, icount);
	إذا (retval)
		أرجع retval;
	إذا (انکڑے->real)
		retval = io_channel_zeroout(انکڑے->real, بلاک, گنتی);

	أرجع retval;
}

ساكن errcode_t undo_cache_readahead(io_channel channel,
				      بنانشانی لما لما بلاک,
				      بنانشانی لما لما گنتی)
{
	بنية undo_نجی_انکڑے *انکڑے;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_cache_readahead(انکڑے->real, بلاک, گنتی);

	أرجع retval;
}

/*
 * Flush data buffers to disk.
 */
ساكن errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	بنية undo_نجی_انکڑے *انکڑے;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = io_channel_flush(انکڑے->real);

	أرجع retval;
}

ساكن errcode_t undo_سیٹ_option(io_channel channel, ثابت حرف *option,
				 ثابت حرف *arg)
{
	errcode_t	retval = 0;
	بنية undo_نجی_انکڑے *انکڑے;
	بنانشانی لما tmp;
	حرف *سرا;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (!strcmp(option, "tdb_data_size")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دلیل;

		tmp = strtoul(arg, &سرا, 0);
		إذا (*سرا)
			أرجع EXT2_ET_INVALID_دلیل;
		إذا (tmp > E2_UNDO_MAX_بلاک_ناپ || tmp < E2_UNDO_MIN_بلاک_ناپ)
			أرجع EXT2_ET_INVALID_دلیل;
		إذا (!انکڑے->tdb_انکڑے_ناپ || !انکڑے->tdb_written) {
			انکڑے->tdb_written = -1;
			انکڑے->tdb_انکڑے_ناپ = tmp;
		}
		أرجع 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	إذا (انکڑے->real && انکڑے->real->manager->سیٹ_option) {
		retval = انکڑے->real->manager->سیٹ_option(انکڑے->real,
							option, arg);
	}
	إذا (!retval && !strcmp(option, "offset")) {
		إذا (!arg)
			أرجع EXT2_ET_INVALID_دلیل;

		tmp = strtoul(arg, &سرا, 0);
		إذا (*سرا)
			أرجع EXT2_ET_INVALID_دلیل;
		انکڑے->وتھ = tmp;
	}
	أرجع retval;
}

ساكن errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	بنية undo_نجی_انکڑے *انکڑے;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	انکڑے = (بنية undo_نجی_انکڑے *) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(انکڑے, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	إذا (انکڑے->real)
		retval = (انکڑے->real->manager->get_stats)(انکڑے->real, stats);

	أرجع retval;
}

ساكن بنية struct_io_manager struct_undo_manager = {
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
