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

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include "config.h"
#include <stdio.h>
#include <string.h>
#නම් HAVE_UNISTD_H
#include <unistd.h>
#endif
#නම් HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#ifdef __linux__
#include <sys/utsname.h>
#endif
#නම් HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#නම් HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#නම් HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#include <limits.h>

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

#ifdef __GNUC__
#define ATTR(x) __attribute__(x)
#නැතහොත්
#define ATTR(x)
#endif

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  do {printf(f, ## a); fflush(stdout); } අතරතුර (0)
#නැතහොත්
# define dbg_printf(f, a...)
#endif

/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(sa_nracanaa, code) \
	  නම් ((sa_nracanaa)->magic != (code)) ආපසු (code)
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
#define E2UNDO_MAGIC "E2UNDO02"
#define KEYBLOCK_MAGIC 0xCADECADE

#define E2UNDO_STATE_FINISHED	0x1	/* undo file is complete */

#define E2UNDO_MIN_BLOCK_SIZE	1024	/* undo blocks are no less than 1KB */
#define E2UNDO_MAX_BLOCK_SIZE	1048576	/* undo blocks are no more than 1MB */

sa_nracanaa undo_header {
	var_na magic[8];		/* "E2UNDO02" */
	__le64 num_keys;	/* how many keys? */
	__le64 super_offset;	/* where in the file is the superblock copy? */
	__le64 key_offset;	/* where do the key/data block chunks start? */
	__le32 block_size;	/* block size of the undo file */
	__le32 fs_block_size;	/* block size of the target device */
	__le32 sb_crc;		/* crc32c of the superblock */
	__le32 state;		/* e2undo state flags */
	__le32 f_compat;	/* compatible features */
	__le32 f_incompat;	/* incompatible features (none so far) */
	__le32 f_rocompat;	/* ro compatible features (none so far) */
	__le32 pad32;		/* padding for fs_offset */
	__le64 fs_offset;	/* filesystem offset */
	__u8 padding[436];	/* padding */
	__le32 header_crc;	/* crc32c of this header (but not this field) */
};

#define E2UNDO_MAX_EXTENT_BLOCKS	512	/* max extent size, in blocks */

sa_nracanaa undo_key {
	__le64 fsblk;		/* where in the fs does the block go */
	__le32 blk_crc;		/* crc32c of the block */
	__le32 size;		/* how many bytes in this block? */
};

sa_nracanaa undo_key_block {
	__le32 magic;		/* KEYBLOCK_MAGIC number */
	__le32 crc;		/* block checksum */
	__le64 reserved;	/* zero */

#නම් __STDC_VERSION__ >= 199901L
	sa_nracanaa undo_key keys[];		/* keys, which come immediately after */
#නැතහොත්
	sa_nracanaa undo_key keys[0];	/* keys, which come immediately after */
#endif
};

sa_nracanaa undo_private_data {
	පූර්ණ_සංඛ්‍යා	magic;

	/* the undo file io channel */
	io_channel undo_file;
	blk64_t undo_blk_num;			/* next free block */
	blk64_t key_blk_num;			/* current key block location */
	blk64_t super_blk_num;			/* superblock location */
	blk64_t first_key_blk;			/* first key block location */
	sa_nracanaa undo_key_block *keyb;
	size_t num_keys, keys_in_block;

	/* The backing io channel */
	io_channel real;

	unsigned long long tdb_data_size;
	පූර්ණ_සංඛ්‍යා tdb_written;

	/* to support offset in unix I/O manager */
	ext2_loff_t offset;

	ext2fs_block_bitmap written_block_map;
	sa_nracanaa struct_ext2_filsys fake_fs;
	var_na *tdb_file;
	sa_nracanaa undo_header hdr;
};
#define KEYS_PER_BLOCK(d) (((d)->tdb_data_size / parimaa_na(sa_nracanaa undo_key)) - 1)

#define E2UNDO_FEATURE_COMPAT_FS_OFFSET 0x1	/* the filesystem offset */

static anta_hstha void e2undo_set_feature_fs_offset(sa_nracanaa undo_header *header) {
	header->f_compat |= ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

static anta_hstha void e2undo_clear_feature_fs_offset(sa_nracanaa undo_header *header) {
	header->f_compat &= ~ext2fs_le32_to_cpu(E2UNDO_FEATURE_COMPAT_FS_OFFSET);
}

static io_manager undo_io_backing_manager;
static var_na *tdb_file;
static පූර්ණ_සංඛ්‍යා actual_size;

errcode_t set_undo_io_backing_manager(io_manager manager)
{
	/*
	 * We may want to do some validation later
	 */
	undo_io_backing_manager = manager;
	ආපසු 0;
}

errcode_t set_undo_io_backup_file(var_na *file_name)
{
	tdb_file = strdup(file_name);

	නම් (tdb_file == NULL) {
		ආපසු EXT2_ET_NO_MEMORY;
	}

	ආපසු 0;
}

static errcode_t write_undo_indexes(sa_nracanaa undo_private_data *data, පූර්ණ_සංඛ්‍යා flush)
{
	errcode_t retval;
	sa_nracanaa ext2_super_block super;
	io_channel channel;
	පූර්ණ_සංඛ්‍යා block_size;
	__u32 sb_crc, hdr_crc;

	/* Spit out a key block, if there's any data */
	නම් (data->keys_in_block) {
		data->keyb->magic = ext2fs_cpu_to_le32(KEYBLOCK_MAGIC);
		data->keyb->crc = 0;
		data->keyb->crc = ext2fs_cpu_to_le32(
					 ext2fs_crc32c_le(~0,
					 (unsigned var_na *)data->keyb,
					 data->tdb_data_size));
		dbg_printf("Writing keyblock to blk %llu\n", data->key_blk_num);
		retval = io_channel_write_blk64(data->undo_file,
						data->key_blk_num,
						1, data->keyb);
		නම් (retval)
			ආපසු retval;
		/* Move on to the next key block if it's full. */
		නම් (data->keys_in_block == KEYS_PER_BLOCK(data)) {
			memset(data->keyb, 0, data->tdb_data_size);
			data->keys_in_block = 0;
			data->key_blk_num = data->undo_blk_num;
			data->undo_blk_num++;
		}
	}

	/* Prepare superblock for write */
	channel = data->real;
	block_size = channel->block_size;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	නම් (retval)
		goto err_out;
	sb_crc = ext2fs_crc32c_le(~0, (unsigned var_na *)&super, SUPERBLOCK_SIZE);
	super.s_magic = ~super.s_magic;

	/* Write the undo header to disk. */
	memcpy(data->hdr.magic, E2UNDO_MAGIC, parimaa_na(data->hdr.magic));
	data->hdr.num_keys = ext2fs_cpu_to_le64(data->num_keys);
	data->hdr.super_offset = ext2fs_cpu_to_le64(data->super_blk_num);
	data->hdr.key_offset = ext2fs_cpu_to_le64(data->first_key_blk);
	data->hdr.fs_block_size = ext2fs_cpu_to_le32(block_size);
	data->hdr.sb_crc = ext2fs_cpu_to_le32(sb_crc);
	data->hdr.fs_offset = ext2fs_cpu_to_le64(data->offset);
	නම් (data->offset)
		e2undo_set_feature_fs_offset(&data->hdr);
	නැතහොත්
		e2undo_clear_feature_fs_offset(&data->hdr);
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned var_na *)&data->hdr,
				   parimaa_na(data->hdr) -
				   parimaa_na(data->hdr.header_crc));
	data->hdr.header_crc = ext2fs_cpu_to_le32(hdr_crc);
	retval = io_channel_write_blk64(data->undo_file, 0,
					-(පූර්ණ_සංඛ්‍යා)parimaa_na(data->hdr),
					&data->hdr);
	නම් (retval)
		goto err_out;

	/*
	 * Record the entire superblock (in FS byte order) so that we can't
	 * apply e2undo files to the wrong FS or out of order.
	 */
	dbg_printf("Writing superblock to block %llu\n", data->super_blk_num);
	retval = io_channel_write_blk64(data->undo_file, data->super_blk_num,
					-SUPERBLOCK_SIZE, &super);
	නම් (retval)
		goto err_out;

	නම් (flush)
		retval = io_channel_flush(data->undo_file);
err_out:
	io_channel_set_blksize(channel, block_size);
	ආපසු retval;
}

static errcode_t undo_setup_tdb(sa_nracanaa undo_private_data *data)
{
	පූර්ණ_සංඛ්‍යා i;
	errcode_t retval;

	නම් (data->tdb_written == 1)
		ආපසු 0;

	data->tdb_written = 1;

	/* Make a bitmap to track what we've written */
	memset(&data->fake_fs, 0, parimaa_na(data->fake_fs));
	data->fake_fs.blocksize = data->tdb_data_size;
	retval = ext2fs_alloc_generic_bmap(&data->fake_fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				EXT2FS_BMAP64_RBTREE,
				0, ~1ULL, ~1ULL,
				"undo block map", &data->written_block_map);
	නම් (retval)
		ආපසු retval;

	/* Allocate key block */
	retval = ext2fs_get_mem(data->tdb_data_size, &data->keyb);
	නම් (retval)
		ආපසු retval;
	data->key_blk_num = data->first_key_blk;

	/* Record block size */
	dbg_printf("Undo block size %llu\n", data->tdb_data_size);
	dbg_printf("Keys per block %llu\n", KEYS_PER_BLOCK(data));
	data->hdr.block_size = ext2fs_cpu_to_le32(data->tdb_data_size);
	io_channel_set_blksize(data->undo_file, data->tdb_data_size);

	/* Ensure that we have space for header blocks */
	සඳහා (i = 0; i <= 2; i++) {
		retval = io_channel_read_blk64(data->undo_file, i, 1,
					       data->keyb);
		නම් (retval)
			memset(data->keyb, 0, data->tdb_data_size);
		retval = io_channel_write_blk64(data->undo_file, i, 1,
						data->keyb);
		නම් (retval)
			ආපසු retval;
		retval = io_channel_flush(data->undo_file);
		නම් (retval)
			ආපසු retval;
	}
	memset(data->keyb, 0, data->tdb_data_size);
	ආපසු 0;
}

static errcode_t undo_write_tdb(io_channel channel,
				unsigned long long block, පූර්ණ_සංඛ්‍යා count)

{
	පූර්ණ_සංඛ්‍යා size, sz;
	unsigned long long block_num, backing_blk_num;
	errcode_t retval = 0;
	ext2_loff_t offset;
	sa_nracanaa undo_private_data *data;
	unsigned var_na *read_ptr;
	unsigned long long end_block;
	unsigned long long data_size;
	sa_nracanaa undo_key *key;
	__u32 blk_crc;

	data = (sa_nracanaa undo_private_data *) channel->private_data;

	නම් (data->undo_file == NULL) {
		/*
		 * Transaction database not initialized
		 */
		ආපසු 0;
	}

	නම් (count == 1)
		size = channel->block_size;
	නැතහොත් {
		නම් (count < 0)
			size = -count;
		නැතහොත්
			size = count * channel->block_size;
	}

	retval = undo_setup_tdb(data);
	නම් (retval)
		ආපසු retval;
	/*
	 * Data is stored in tdb database as blocks of tdb_data_size size
	 * This helps in efficient lookup further.
	 *
	 * We divide the disk to blocks of tdb_data_size.
	 */
	offset = (block * channel->block_size) + data->offset ;
	block_num = offset / data->tdb_data_size;
	end_block = (offset + size - 1) / data->tdb_data_size;

	අතරතුර (block_num <= end_block) {
		__u32 keysz;

		/*
		 * Check if we have the record already
		 */
		නම් (ext2fs_test_block_bitmap2(data->written_block_map,
						   block_num)) {
			/* Try the next block */
			block_num++;
			continue;
		}
		ext2fs_mark_block_bitmap2(data->written_block_map, block_num);

		/*
		 * Read one block using the backing I/O manager
		 * The backing I/O manager block size may be
		 * different from the tdb_data_size.
		 * Also we need to recalculate the block number with respect
		 * to the backing I/O manager.
		 */
		offset = block_num * data->tdb_data_size +
				(data->offset % data->tdb_data_size);
		backing_blk_num = (offset - data->offset) / channel->block_size;

		retval = ext2fs_get_mem(data->tdb_data_size, &read_ptr);
		නම් (retval) {
			ආපසු retval;
		}

		memset(read_ptr, 0, data->tdb_data_size);
		actual_size = 0;
		නම් ((data->tdb_data_size % channel->block_size) == 0)
			sz = data->tdb_data_size / channel->block_size;
		නැතහොත්
			sz = -data->tdb_data_size;
		retval = io_channel_read_blk64(data->real, backing_blk_num,
					     sz, read_ptr);
		නම් (retval) {
			නම් (retval != EXT2_ET_SHORT_READ) {
				free(read_ptr);
				ආපසු retval;
			}
			/*
			 * short read so update the record size
			 * accordingly
			 */
			data_size = actual_size;
		} නැතහොත් {
			data_size = data->tdb_data_size;
		}
		නම් (data_size == 0) {
			free(read_ptr);
			block_num++;
			continue;
		}
		dbg_printf("Read %llu bytes from FS block %llu (blk=%llu cnt=%llu)\n",
		       data_size, backing_blk_num, block, data->tdb_data_size);
		නම් ((data_size % data->undo_file->block_size) == 0)
			sz = data_size / data->undo_file->block_size;
		නැතහොත්
			sz = -data_size;;
		/* extend this key? */
		නම් (data->keys_in_block) {
			key = data->keyb->keys + data->keys_in_block - 1;
			keysz = ext2fs_le32_to_cpu(key->size);
		} නැතහොත් {
			key = NULL;
			keysz = 0;
		}
		නම් (key != NULL &&
		    (ext2fs_le64_to_cpu(key->fsblk) * channel->block_size +
		     channel->block_size - 1 +
		     keysz) / channel->block_size == backing_blk_num &&
		    E2UNDO_MAX_EXTENT_BLOCKS * data->tdb_data_size >
		    keysz + data_size) {
			blk_crc = ext2fs_le32_to_cpu(key->blk_crc);
			blk_crc = ext2fs_crc32c_le(blk_crc, read_ptr, data_size);
			key->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			key->size = ext2fs_cpu_to_le32(keysz + data_size);
		} නැතහොත් {
			data->num_keys++;
			key = data->keyb->keys + data->keys_in_block;
			data->keys_in_block++;
			key->fsblk = ext2fs_cpu_to_le64(backing_blk_num);
			blk_crc = ext2fs_crc32c_le(~0, read_ptr, data_size);
			key->blk_crc = ext2fs_cpu_to_le32(blk_crc);
			key->size = ext2fs_cpu_to_le32(data_size);
		}
		dbg_printf("Writing block %llu to offset %llu size %d key %zu\n",
		       block_num,
		       data->undo_blk_num,
		       sz, data->num_keys - 1);
		retval = io_channel_write_blk64(data->undo_file,
					data->undo_blk_num, sz, read_ptr);
		නම් (retval) {
			free(read_ptr);
			ආපසු retval;
		}
		data->undo_blk_num++;
		free(read_ptr);

		/* Write out the key block */
		retval = write_undo_indexes(data, 0);
		නම් (retval)
			ආපසු retval;

		/* Next block */
		block_num++;
	}

	ආපසු retval;
}

static errcode_t undo_io_read_error(io_channel channel ATTR((unused)),
				    unsigned long block ATTR((unused)),
				    පූර්ණ_සංඛ්‍යා count ATTR((unused)),
				    void *data ATTR((unused)),
				    size_t size ATTR((unused)),
				    පූර්ණ_සංඛ්‍යා actual,
				    errcode_t error ATTR((unused)))
{
	actual_size = actual;
	ආපසු error;
}

static void undo_err_handler_init(io_channel channel)
{
	channel->read_error = undo_io_read_error;
}

static පූර්ණ_සංඛ්‍යා check_filesystem(sa_nracanaa undo_header *hdr, io_channel undo_file,
			    unsigned පූර්ණ_සංඛ්‍යා blocksize, blk64_t super_block,
			    io_channel channel)
{
	sa_nracanaa ext2_super_block super, *sb;
	var_na *buf;
	__u32 sb_crc;
	errcode_t retval;

	io_channel_set_blksize(channel, SUPERBLOCK_OFFSET);
	retval = io_channel_read_blk64(channel, 1, -SUPERBLOCK_SIZE, &super);
	නම් (retval)
		ආපසු retval;

	/*
	 * Compare the FS and the undo file superblock so that we don't
	 * append to something that doesn't match this FS.
	 */
	retval = ext2fs_get_mem(blocksize, &buf);
	නම් (retval)
		ආපසු retval;
	retval = io_channel_read_blk64(undo_file, super_block,
				       -SUPERBLOCK_SIZE, buf);
	නම් (retval)
		goto out;
	sb = (sa_nracanaa ext2_super_block *)buf;
	sb->s_magic = ~sb->s_magic;
	නම් (memcmp(&super, buf, parimaa_na(super))) {
		retval = -1;
		goto out;
	}
	sb_crc = ext2fs_crc32c_le(~0, (unsigned var_na *)buf, SUPERBLOCK_SIZE);
	නම් (ext2fs_le32_to_cpu(hdr->sb_crc) != sb_crc) {
		retval = -1;
		goto out;
	}

out:
	ext2fs_free_mem(&buf);
	ආපසු retval;
}

/*
 * Try to re-open the undo file, so that we can resume where we left off.
 * That way, the user can pass the same undo file to various programs as
 * part of an FS upgrade instead of having to create multiple files and
 * then apply them in correct order.
 */
static errcode_t try_reopen_undo_file(පූර්ණ_සංඛ්‍යා undo_fd,
				      sa_nracanaa undo_private_data *data)
{
	sa_nracanaa undo_header hdr;
	sa_nracanaa undo_key *dkey;
	ext2fs_struct_stat statbuf;
	unsigned පූර්ණ_සංඛ්‍යා blocksize, fs_blocksize;
	blk64_t super_block, lblk;
	size_t num_keys, keys_per_block, i;
	__u32 hdr_crc, key_crc;
	errcode_t retval;

	/* Zero size already? */
	retval = ext2fs_fstat(undo_fd, &statbuf);
	නම් (retval)
		goto bad_file;
	නම් (statbuf.st_size == 0)
		goto out;

	/* check the file header */
	retval = io_channel_read_blk64(data->undo_file, 0, -(පූර්ණ_සංඛ්‍යා)parimaa_na(hdr),
				       &hdr);
	නම් (retval)
		goto bad_file;

	නම් (memcmp(hdr.magic, E2UNDO_MAGIC,
		    parimaa_na(hdr.magic)))
		goto bad_file;
	hdr_crc = ext2fs_crc32c_le(~0, (unsigned var_na *)&hdr,
				   parimaa_na(sa_nracanaa undo_header) -
				   parimaa_na(__u32));
	නම් (ext2fs_le32_to_cpu(hdr.header_crc) != hdr_crc)
		goto bad_file;
	blocksize = ext2fs_le32_to_cpu(hdr.block_size);
	fs_blocksize = ext2fs_le32_to_cpu(hdr.fs_block_size);
	නම් (blocksize > E2UNDO_MAX_BLOCK_SIZE ||
	    blocksize < E2UNDO_MIN_BLOCK_SIZE ||
	    !blocksize || !fs_blocksize)
		goto bad_file;
	super_block = ext2fs_le64_to_cpu(hdr.super_offset);
	num_keys = ext2fs_le64_to_cpu(hdr.num_keys);
	io_channel_set_blksize(data->undo_file, blocksize);
	/*
	 * Do not compare hdr.f_compat with the available compatible
	 * features set, because a "missing" compatible feature should
	 * not cause any problems.
	 */
	නම් (hdr.f_incompat || hdr.f_rocompat)
		goto bad_file;

	/* Superblock matches this FS? */
	නම් (check_filesystem(&hdr, data->undo_file, blocksize, super_block,
			     data->real) != 0) {
		retval = EXT2_ET_UNDO_FILE_WRONG;
		goto out;
	}

	/* Try to set ourselves up */
	data->tdb_data_size = blocksize;
	retval = undo_setup_tdb(data);
	නම් (retval)
		goto bad_file;
	data->num_keys = num_keys;
	data->super_blk_num = super_block;
	data->first_key_blk = ext2fs_le64_to_cpu(hdr.key_offset);

	/* load the written block map */
	keys_per_block = KEYS_PER_BLOCK(data);
	lblk = data->first_key_blk;
	dbg_printf("nr_keys=%lu, kpb=%zu, blksz=%u\n",
		   num_keys, keys_per_block, blocksize);
	සඳහා (i = 0; i < num_keys; i += keys_per_block) {
		size_t j, max_j;
		__le32 crc;

		data->key_blk_num = lblk;
		retval = io_channel_read_blk64(data->undo_file,
					       lblk, 1, data->keyb);
		නම් (retval)
			goto bad_key_replay;

		/* check keys */
		නම් (ext2fs_le32_to_cpu(data->keyb->magic) != KEYBLOCK_MAGIC) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			goto bad_key_replay;
		}
		crc = data->keyb->crc;
		data->keyb->crc = 0;
		key_crc = ext2fs_crc32c_le(~0, (unsigned var_na *)data->keyb,
					   blocksize);
		නම් (ext2fs_le32_to_cpu(crc) != key_crc) {
			retval = EXT2_ET_UNDO_FILE_CORRUPT;
			goto bad_key_replay;
		}

		/* load keys from key block */
		lblk++;
		max_j = data->num_keys - i;
		නම් (max_j > keys_per_block)
			max_j = keys_per_block;
		සඳහා (j = 0, dkey = data->keyb->keys;
		     j < max_j;
		     j++, dkey++) {
			blk64_t fsblk = ext2fs_le64_to_cpu(dkey->fsblk);
			blk64_t undo_blk = fsblk * fs_blocksize / blocksize;
			size_t size = ext2fs_le32_to_cpu(dkey->size);

			ext2fs_mark_block_bitmap_range2(data->written_block_map,
					 undo_blk,
					(size + blocksize - 1) / blocksize);
			lblk += (size + blocksize - 1) / blocksize;
			data->undo_blk_num = lblk;
			data->keys_in_block = j + 1;
		}
	}
	dbg_printf("Reopen undo, keyblk=%llu undoblk=%llu nrkeys=%zu kib=%zu\n",
		   data->key_blk_num, data->undo_blk_num, data->num_keys,
		   data->keys_in_block);

	data->hdr.state = hdr.state & ~E2UNDO_STATE_FINISHED;
	data->hdr.f_compat = hdr.f_compat;
	data->hdr.f_incompat = hdr.f_incompat;
	data->hdr.f_rocompat = hdr.f_rocompat;
	ආපසු retval;

bad_key_replay:
	data->key_blk_num = data->undo_blk_num = 0;
	data->keys_in_block = 0;
	ext2fs_free_mem(&data->keyb);
	ext2fs_free_generic_bitmap(data->written_block_map);
	data->tdb_written = 0;
	goto out;
bad_file:
	retval = EXT2_ET_UNDO_FILE_CORRUPT;
out:
	ආපසු retval;
}

static void undo_atexit(void *p)
{
	sa_nracanaa undo_private_data *data = p;
	errcode_t err;

	err = write_undo_indexes(data, 1);
	io_channel_close(data->undo_file);

	com_err(data->tdb_file, err, "while force-closing undo file");
}

static errcode_t undo_open(const var_na *name, පූර්ණ_සංඛ්‍යා flags, io_channel *channel)
{
	io_channel	io = NULL;
	sa_nracanaa undo_private_data *data = NULL;
	පූර්ණ_සංඛ්‍යා		undo_fd = -1;
	errcode_t	retval;

	/* We don't support multi-threading, at least for now */
	flags &= ~IO_FLAG_THREADS;
	නම් (name == 0)
		ආපසු EXT2_ET_BAD_DEVICE_NAME;
	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa struct_io_channel), &io);
	නම් (retval)
		goto cleanup;
	memset(io, 0, parimaa_na(sa_nracanaa struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa undo_private_data), &data);
	නම් (retval)
		goto cleanup;

	io->manager = undo_io_manager;
	retval = ext2fs_get_mem(strlen(name)+1, &io->name);
	නම් (retval)
		goto cleanup;

	strcpy(io->name, name);
	io->private_data = data;
	io->block_size = 1024;
	io->read_error = 0;
	io->write_error = 0;
	io->refcount = 1;

	memset(data, 0, parimaa_na(sa_nracanaa undo_private_data));
	data->magic = EXT2_ET_MAGIC_UNIX_IO_CHANNEL;
	data->super_blk_num = 1;
	data->first_key_blk = 2;
	data->undo_blk_num = 3;

	නම් (undo_io_backing_manager) {
		retval = undo_io_backing_manager->open(name, flags,
						       &data->real);
		නම් (retval)
			goto cleanup;

		data->tdb_file = strdup(tdb_file);
		නම් (data->tdb_file == NULL)
			goto cleanup;
		undo_fd = ext2fs_open_file(data->tdb_file, O_RDWR | O_CREAT,
					   0600);
		නම් (undo_fd < 0)
			goto cleanup;

		retval = undo_io_backing_manager->open(data->tdb_file,
						       IO_FLAG_RW,
						       &data->undo_file);
		නම් (retval)
			goto cleanup;
	} නැතහොත් {
		data->real = NULL;
		data->undo_file = NULL;
	}

	නම් (data->real)
		io->flags = (io->flags & ~CHANNEL_FLAGS_DISCARD_ZEROES) |
			    (data->real->flags & CHANNEL_FLAGS_DISCARD_ZEROES);

	/*
	 * setup err handler for read so that we know
	 * when the backing manager fails do short read
	 */
	නම් (data->real)
		undo_err_handler_init(data->real);

	නම් (data->undo_file) {
		retval = try_reopen_undo_file(undo_fd, data);
		නම් (retval)
			goto cleanup;
	}
	retval = ext2fs_add_exit_fn(undo_atexit, data);
	නම් (retval)
		goto cleanup;

	*channel = io;
	නම් (undo_fd >= 0)
		close(undo_fd);
	ආපසු retval;

cleanup:
	ext2fs_remove_exit_fn(undo_atexit, data);
	නම් (undo_fd >= 0)
		close(undo_fd);
	නම් (data && data->undo_file)
		io_channel_close(data->undo_file);
	නම් (data && data->tdb_file)
		free(data->tdb_file);
	නම් (data && data->real)
		io_channel_close(data->real);
	නම් (data)
		ext2fs_free_mem(&data);
	නම් (io && io->name)
		ext2fs_free_mem(&io->name);
	නම් (io)
		ext2fs_free_mem(&io);
	ආපසු retval;
}

static errcode_t undo_close(io_channel channel)
{
	sa_nracanaa undo_private_data *data;
	errcode_t	err, retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	නම් (--channel->refcount > 0)
		ආපසු 0;
	/* Before closing write the file system identity */
	නම් (!ext2fs_safe_getenv("UNDO_IO_SIMULATE_UNFINISHED"))
		data->hdr.state = ext2fs_cpu_to_le32(E2UNDO_STATE_FINISHED);
	err = write_undo_indexes(data, 1);
	ext2fs_remove_exit_fn(undo_atexit, data);
	නම් (data->real)
		retval = io_channel_close(data->real);
	නම් (data->tdb_file)
		free(data->tdb_file);
	නම් (data->undo_file)
		io_channel_close(data->undo_file);
	ext2fs_free_mem(&data->keyb);
	නම් (data->written_block_map)
		ext2fs_free_generic_bitmap(data->written_block_map);
	ext2fs_free_mem(&channel->private_data);
	නම් (channel->name)
		ext2fs_free_mem(&channel->name);
	ext2fs_free_mem(&channel);

	නම් (err)
		ආපසු err;
	ආපසු retval;
}

static errcode_t undo_set_blksize(io_channel channel, පූර්ණ_සංඛ්‍යා blksize)
{
	sa_nracanaa undo_private_data *data;
	errcode_t		retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	නම් (blksize > E2UNDO_MAX_BLOCK_SIZE || blksize < E2UNDO_MIN_BLOCK_SIZE)
		ආපසු EXT2_ET_INVALID_ARGUMENT;

	නම් (data->real)
		retval = io_channel_set_blksize(data->real, blksize);
	/*
	 * Set the block size used for tdb
	 */
	නම් (!data->tdb_data_size || !data->tdb_written)
		data->tdb_data_size = blksize;
	channel->block_size = blksize;
	ආපසු retval;
}

static errcode_t undo_read_blk64(io_channel channel, unsigned long long block,
			       පූර්ණ_සංඛ්‍යා count, void *buf)
{
	errcode_t	retval = 0;
	sa_nracanaa undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	නම් (data->real)
		retval = io_channel_read_blk64(data->real, block, count, buf);

	ආපසු retval;
}

static errcode_t undo_read_blk(io_channel channel, unsigned long block,
			       පූර්ණ_සංඛ්‍යා count, void *buf)
{
	ආපසු undo_read_blk64(channel, block, count, buf);
}

static errcode_t undo_write_blk64(io_channel channel, unsigned long long block,
				පූර්ණ_සංඛ්‍යා count, const void *buf)
{
	sa_nracanaa undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);
	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, count);
	නම් (retval)
		 ආපසු retval;
	නම් (data->real)
		retval = io_channel_write_blk64(data->real, block, count, buf);

	ආපසු retval;
}

static errcode_t undo_write_blk(io_channel channel, unsigned long block,
				පූර්ණ_සංඛ්‍යා count, const void *buf)
{
	ආපසු undo_write_blk64(channel, block, count, buf);
}

static errcode_t undo_write_byte(io_channel channel, unsigned long offset,
				 පූර්ණ_සංඛ්‍යා size, const void *buf)
{
	sa_nracanaa undo_private_data *data;
	errcode_t	retval = 0;
	ext2_loff_t	location;
	unsigned long blk_num, count;;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	location = offset + data->offset;
	blk_num = location/channel->block_size;
	/*
	 * the size specified may spread across multiple blocks
	 * also make sure we account for the fact that block start
	 * offset for tdb is different from the backing I/O manager
	 * due to possible different block size
	 */
	count = (size + (location % channel->block_size) +
			channel->block_size  -1)/channel->block_size;
	retval = undo_write_tdb(channel, blk_num, count);
	නම් (retval)
		ආපසු retval;
	නම් (data->real && data->real->manager->write_byte)
		retval = io_channel_write_byte(data->real, offset, size, buf);

	ආපසු retval;
}

static errcode_t undo_discard(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	sa_nracanaa undo_private_data *data;
	errcode_t	retval = 0;
	පූර්ණ_සංඛ්‍යා icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	නම් (count > INT_MAX)
		ආපසු EXT2_ET_UNIMPLEMENTED;
	icount = count;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	නම් (retval)
		ආපසු retval;
	නම් (data->real)
		retval = io_channel_discard(data->real, block, count);

	ආපසු retval;
}

static errcode_t undo_zeroout(io_channel channel, unsigned long long block,
			      unsigned long long count)
{
	sa_nracanaa undo_private_data *data;
	errcode_t	retval = 0;
	පූර්ණ_සංඛ්‍යා icount;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	නම් (count > INT_MAX)
		ආපසු EXT2_ET_UNIMPLEMENTED;
	icount = count;

	/*
	 * First write the existing content into database
	 */
	retval = undo_write_tdb(channel, block, icount);
	නම් (retval)
		ආපසු retval;
	නම් (data->real)
		retval = io_channel_zeroout(data->real, block, count);

	ආපසු retval;
}

static errcode_t undo_cache_readahead(io_channel channel,
				      unsigned long long block,
				      unsigned long long count)
{
	sa_nracanaa undo_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	නම් (data->real)
		retval = io_channel_cache_readahead(data->real, block, count);

	ආපසු retval;
}

/*
 * Flush data buffers to disk.
 */
static errcode_t undo_flush(io_channel channel)
{
	errcode_t	retval = 0;
	sa_nracanaa undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	නම් (data->real)
		retval = io_channel_flush(data->real);

	ආපසු retval;
}

static errcode_t undo_set_option(io_channel channel, const var_na *option,
				 const var_na *arg)
{
	errcode_t	retval = 0;
	sa_nracanaa undo_private_data *data;
	unsigned long tmp;
	var_na *end;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	නම් (!strcmp(option, "tdb_data_size")) {
		නම් (!arg)
			ආපසු EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		නම් (*end)
			ආපසු EXT2_ET_INVALID_ARGUMENT;
		නම් (tmp > E2UNDO_MAX_BLOCK_SIZE || tmp < E2UNDO_MIN_BLOCK_SIZE)
			ආපසු EXT2_ET_INVALID_ARGUMENT;
		නම් (!data->tdb_data_size || !data->tdb_written) {
			data->tdb_written = -1;
			data->tdb_data_size = tmp;
		}
		ආපසු 0;
	}
	/*
	 * Need to support offset option to work with
	 * Unix I/O manager
	 */
	නම් (data->real && data->real->manager->set_option) {
		retval = data->real->manager->set_option(data->real,
							option, arg);
	}
	නම් (!retval && !strcmp(option, "offset")) {
		නම් (!arg)
			ආපසු EXT2_ET_INVALID_ARGUMENT;

		tmp = strtoul(arg, &end, 0);
		නම් (*end)
			ආපසු EXT2_ET_INVALID_ARGUMENT;
		data->offset = tmp;
	}
	ආපසු retval;
}

static errcode_t undo_get_stats(io_channel channel, io_stats *stats)
{
	errcode_t	retval = 0;
	sa_nracanaa undo_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (sa_nracanaa undo_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_UNIX_IO_CHANNEL);

	නම් (data->real)
		retval = (data->real->manager->get_stats)(data->real, stats);

	ආපසු retval;
}

static sa_nracanaa struct_io_manager struct_undo_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "Undo I/O Manager",
	.open		= undo_open,
	.close		= undo_close,
	.set_blksize	= undo_set_blksize,
	.read_blk	= undo_read_blk,
	.write_blk	= undo_write_blk,
	.flush		= undo_flush,
	.write_byte	= undo_write_byte,
	.set_option	= undo_set_option,
	.get_stats	= undo_get_stats,
	.read_blk64	= undo_read_blk64,
	.write_blk64	= undo_write_blk64,
	.discard	= undo_discard,
	.zeroout	= undo_zeroout,
	.cache_readahead	= undo_cache_readahead,
};

io_manager undo_io_manager = &struct_undo_manager;
