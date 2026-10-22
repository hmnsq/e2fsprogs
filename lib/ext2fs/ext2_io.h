/*
 * io.h --- the I/O manager abstraction
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ifndef _EXT2FS_EXT2_IO_H
#define _EXT2FS_EXT2_IO_H

#অন্তর্ভাব <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#যদি defined(__GNUC__) || defined(HAS_LONG_LONG)
প্রকারনির্দেশ long long	ext2_loff_t;
#অন্যথা
প্রকারনির্দেশ long		ext2_loff_t;
#endif

/* llseek.c */
ext2_loff_t ext2fs_llseek (পূর্ণাঙ্ক, ext2_loff_t, পূর্ণাঙ্ক);

প্রকারনির্দেশ সংরচনা struct_io_manager *io_manager;
প্রকারনির্দেশ সংরচনা struct_io_channel *io_channel;
প্রকারনির্দেশ সংরচনা struct_io_stats *io_stats;

#define CHANNEL_FLAGS_WRITETHROUGH	0x01
#define CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#define CHANNEL_FLAGS_BLOCK_DEVICE	0x04
#define CHANNEL_FLAGS_THREADS		0x08
#define CHANNEL_FLAGS_NODISCARD		0x10
#define CHANNEL_FLAGS_NOZEROOUT		0x20

#define io_channel_discard_zeroes_data(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

সংরচনা struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	বর্ণ		*name;
	পূর্ণাঙ্ক		block_size;
	errcode_t	(*read_error)(io_channel channel,
				      unsigned long block,
				      পূর্ণাঙ্ক গুনতি,
				      শূন্য *data,
				      size_t size,
				      পূর্ণাঙ্ক actual_bytes_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       unsigned long block,
				       পূর্ণাঙ্ক গুনতি,
				       ধ্রুব শূন্য *data,
				       size_t size,
				       পূর্ণাঙ্ক actual_bytes_written,
				       errcode_t error);
	পূর্ণাঙ্ক		refcount;
	unsigned পূর্ণাঙ্ক	flags;
	long		reserved[14];
	শূন্য		*private_data;
	শূন্য		*app_data;
	পূর্ণাঙ্ক		align;
};

সংরচনা struct_io_stats {
	পূর্ণাঙ্ক			অঙ্ক_fields;
	পূর্ণাঙ্ক			reserved;
	unsigned long long	bytes_read;
	unsigned long long	bytes_written;
	unsigned long long	cache_hits;
	unsigned long long	cache_misses;
};

সংরচনা struct_io_manager {
	errcode_t magic;
	ধ্রুব বর্ণ *name;
	errcode_t (*open)(ধ্রুব বর্ণ *name, পূর্ণাঙ্ক flags, io_channel *channel);
	errcode_t (*close)(io_channel channel);
	errcode_t (*set_blksize)(io_channel channel, পূর্ণাঙ্ক blksize);
	errcode_t (*read_blk)(io_channel channel, unsigned long block,
			      পূর্ণাঙ্ক গুনতি, শূন্য *data);
	errcode_t (*write_blk)(io_channel channel, unsigned long block,
			       পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *data);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, unsigned long offset,
				পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *data);
	errcode_t (*set_option)(io_channel channel, ধ্রুব বর্ণ *option,
				ধ্রুব বর্ণ *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, unsigned long long block,
					পূর্ণাঙ্ক গুনতি, শূন্য *data);
	errcode_t (*write_blk64)(io_channel channel, unsigned long long block,
					পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *data);
	errcode_t (*discard)(io_channel channel, unsigned long long block,
			     unsigned long long গুনতি);
	errcode_t (*cache_readahead)(io_channel channel,
				     unsigned long long block,
				     unsigned long long গুনতি);
	errcode_t (*zeroout)(io_channel channel, unsigned long long block,
			     unsigned long long গুনতি);
	errcode_t (*flock)(io_channel channel, unsigned পূর্ণাঙ্ক flock_flags);
	long	reserved[13];
};

#define IO_FLAG_RW		0x0001
#define IO_FLAG_EXCLUSIVE	0x0002
#define IO_FLAG_DIRECT_IO	0x0004
#define IO_FLAG_FORCE_BOUNCE	0x0008
#define IO_FLAG_THREADS		0x0010
#define IO_FLAG_NOCACHE		0x0020

/* Prevent other programs from reading or writing to underlying storage */
#define IO_CHANNEL_FLOCK_EXCLUSIVE	0x1
/* Prevent other programs from writing to underlying storage */
#define IO_CHANNEL_FLOCK_SHARED		0x2
/* Return EBUSY if the lock cannot be taken immediately */
#define IO_CHANNEL_FLOCK_TRYLOCK	0x4

/*
 * Convenience functions....
 */
#define io_channel_close(c) 		((c)->manager->close((c)))
#define io_channel_set_blksize(c,s)	((c)->manager->set_blksize((c),s))
#define io_channel_read_blk(c,b,n,d)	((c)->manager->read_blk((c),b,n,d))
#define io_channel_write_blk(c,b,n,d)	((c)->manager->write_blk((c),b,n,d))
#define io_channel_flush(c) 		((c)->manager->flush((c)))
#define io_channel_bumpcount(c)		((c)->refcount++)

/* io_manager.c */
বাহ্য errcode_t io_channel_set_options(io_channel channel,
					ধ্রুব বর্ণ *options);
বাহ্য errcode_t io_channel_write_byte(io_channel channel,
				       unsigned long offset,
				       পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *data);
বাহ্য errcode_t io_channel_read_blk64(io_channel channel,
				       unsigned long long block,
				       পূর্ণাঙ্ক গুনতি, শূন্য *data);
বাহ্য errcode_t io_channel_write_blk64(io_channel channel,
					unsigned long long block,
					পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *data);
বাহ্য errcode_t io_channel_discard(io_channel channel,
				    unsigned long long block,
				    unsigned long long গুনতি);
বাহ্য errcode_t io_channel_zeroout(io_channel channel,
				    unsigned long long block,
				    unsigned long long গুনতি);
বাহ্য errcode_t io_channel_alloc_buf(io_channel channel,
				      পূর্ণাঙ্ক গুনতি, শূন্য *ptr);
বাহ্য errcode_t io_channel_cache_readahead(io_channel io,
					    unsigned long long block,
					    unsigned long long গুনতি);
বাহ্য errcode_t io_channel_flock(io_channel io, unsigned পূর্ণাঙ্ক flock_flags);
বাহ্য errcode_t io_channel_funlock(io_channel io);

#ifdef _WIN32
/* windows_io.c */
বাহ্য io_manager windows_io_manager;
#define default_io_manager windows_io_manager
#অন্যথা
/* unix_io.c */
বাহ্য io_manager unix_io_manager;
বাহ্য io_manager unixfd_io_manager;
#define default_io_manager unix_io_manager
#endif

/* sparse_io.c */
বাহ্য io_manager sparse_io_manager;
বাহ্য io_manager sparsefd_io_manager;

/* undo_io.c */
বাহ্য io_manager undo_io_manager;
বাহ্য errcode_t set_undo_io_backing_manager(io_manager manager);
বাহ্য errcode_t set_undo_io_backup_file(বর্ণ *file_name);

/* test_io.c */
বাহ্য io_manager test_io_manager, test_io_backing_manager;
বাহ্য শূন্য (*test_io_cb_read_blk)
	(unsigned long block, পূর্ণাঙ্ক গুনতি, errcode_t err);
বাহ্য শূন্য (*test_io_cb_write_blk)
	(unsigned long block, পূর্ণাঙ্ক গুনতি, errcode_t err);
বাহ্য শূন্য (*test_io_cb_read_blk64)
	(unsigned long long block, পূর্ণাঙ্ক গুনতি, errcode_t err);
বাহ্য শূন্য (*test_io_cb_write_blk64)
	(unsigned long long block, পূর্ণাঙ্ক গুনতি, errcode_t err);
বাহ্য শূন্য (*test_io_cb_set_blksize)
	(পূর্ণাঙ্ক blksize, errcode_t err);

#endif /* _EXT2FS_EXT2_IO_H */

