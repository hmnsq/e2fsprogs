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

#અન્તર્ભાવ <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#જો defined(__GNUC__) || defined(HAS_LONG_LONG)
પ્રકારવ્યાખ્યા long long	ext2_loff_t;
#નહીંતર
પ્રકારવ્યાખ્યા long		ext2_loff_t;
#endif

/* llseek.c */
ext2_loff_t ext2fs_llseek (પૂર્ણાંક, ext2_loff_t, પૂર્ણાંક);

પ્રકારવ્યાખ્યા સંરચના struct_io_manager *io_manager;
પ્રકારવ્યાખ્યા સંરચના struct_io_channel *io_channel;
પ્રકારવ્યાખ્યા સંરચના struct_io_stats *io_stats;

#define CHANNEL_FLAGS_WRITETHROUGH	0x01
#define CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#define CHANNEL_FLAGS_BLOCK_DEVICE	0x04
#define CHANNEL_FLAGS_THREADS		0x08
#define CHANNEL_FLAGS_NODISCARD		0x10
#define CHANNEL_FLAGS_NOZEROOUT		0x20

#define io_channel_discard_zeroes_data(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

સંરચના struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	અક્ષર		*name;
	પૂર્ણાંક		block_size;
	errcode_t	(*read_error)(io_channel channel,
				      unsigned long block,
				      પૂર્ણાંક ગણતરી,
				      શૂન્ય *data,
				      size_t size,
				      પૂર્ણાંક actual_bytes_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       unsigned long block,
				       પૂર્ણાંક ગણતરી,
				       અચલ શૂન્ય *data,
				       size_t size,
				       પૂર્ણાંક actual_bytes_written,
				       errcode_t error);
	પૂર્ણાંક		refcount;
	unsigned પૂર્ણાંક	flags;
	long		reserved[14];
	શૂન્ય		*private_data;
	શૂન્ય		*app_data;
	પૂર્ણાંક		align;
};

સંરચના struct_io_stats {
	પૂર્ણાંક			અંક_fields;
	પૂર્ણાંક			reserved;
	unsigned long long	bytes_read;
	unsigned long long	bytes_written;
	unsigned long long	cache_hits;
	unsigned long long	cache_misses;
};

સંરચના struct_io_manager {
	errcode_t magic;
	અચલ અક્ષર *name;
	errcode_t (*open)(અચલ અક્ષર *name, પૂર્ણાંક flags, io_channel *channel);
	errcode_t (*close)(io_channel channel);
	errcode_t (*set_blksize)(io_channel channel, પૂર્ણાંક blksize);
	errcode_t (*read_blk)(io_channel channel, unsigned long block,
			      પૂર્ણાંક ગણતરી, શૂન્ય *data);
	errcode_t (*write_blk)(io_channel channel, unsigned long block,
			       પૂર્ણાંક ગણતરી, અચલ શૂન્ય *data);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, unsigned long offset,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *data);
	errcode_t (*set_option)(io_channel channel, અચલ અક્ષર *option,
				અચલ અક્ષર *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, unsigned long long block,
					પૂર્ણાંક ગણતરી, શૂન્ય *data);
	errcode_t (*write_blk64)(io_channel channel, unsigned long long block,
					પૂર્ણાંક ગણતરી, અચલ શૂન્ય *data);
	errcode_t (*discard)(io_channel channel, unsigned long long block,
			     unsigned long long ગણતરી);
	errcode_t (*cache_readahead)(io_channel channel,
				     unsigned long long block,
				     unsigned long long ગણતરી);
	errcode_t (*zeroout)(io_channel channel, unsigned long long block,
			     unsigned long long ગણતરી);
	errcode_t (*flock)(io_channel channel, unsigned પૂર્ણાંક flock_flags);
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
બાહ્ય errcode_t io_channel_set_options(io_channel channel,
					અચલ અક્ષર *options);
બાહ્ય errcode_t io_channel_write_byte(io_channel channel,
				       unsigned long offset,
				       પૂર્ણાંક ગણતરી, અચલ શૂન્ય *data);
બાહ્ય errcode_t io_channel_read_blk64(io_channel channel,
				       unsigned long long block,
				       પૂર્ણાંક ગણતરી, શૂન્ય *data);
બાહ્ય errcode_t io_channel_write_blk64(io_channel channel,
					unsigned long long block,
					પૂર્ણાંક ગણતરી, અચલ શૂન્ય *data);
બાહ્ય errcode_t io_channel_discard(io_channel channel,
				    unsigned long long block,
				    unsigned long long ગણતરી);
બાહ્ય errcode_t io_channel_zeroout(io_channel channel,
				    unsigned long long block,
				    unsigned long long ગણતરી);
બાહ્ય errcode_t io_channel_alloc_buf(io_channel channel,
				      પૂર્ણાંક ગણતરી, શૂન્ય *ptr);
બાહ્ય errcode_t io_channel_cache_readahead(io_channel io,
					    unsigned long long block,
					    unsigned long long ગણતરી);
બાહ્ય errcode_t io_channel_flock(io_channel io, unsigned પૂર્ણાંક flock_flags);
બાહ્ય errcode_t io_channel_funlock(io_channel io);

#ifdef _WIN32
/* windows_io.c */
બાહ્ય io_manager windows_io_manager;
#define default_io_manager windows_io_manager
#નહીંતર
/* unix_io.c */
બાહ્ય io_manager unix_io_manager;
બાહ્ય io_manager unixfd_io_manager;
#define default_io_manager unix_io_manager
#endif

/* sparse_io.c */
બાહ્ય io_manager sparse_io_manager;
બાહ્ય io_manager sparsefd_io_manager;

/* undo_io.c */
બાહ્ય io_manager undo_io_manager;
બાહ્ય errcode_t set_undo_io_backing_manager(io_manager manager);
બાહ્ય errcode_t set_undo_io_backup_file(અક્ષર *file_name);

/* test_io.c */
બાહ્ય io_manager test_io_manager, test_io_backing_manager;
બાહ્ય શૂન્ય (*test_io_cb_read_blk)
	(unsigned long block, પૂર્ણાંક ગણતરી, errcode_t err);
બાહ્ય શૂન્ય (*test_io_cb_write_blk)
	(unsigned long block, પૂર્ણાંક ગણતરી, errcode_t err);
બાહ્ય શૂન્ય (*test_io_cb_read_blk64)
	(unsigned long long block, પૂર્ણાંક ગણતરી, errcode_t err);
બાહ્ય શૂન્ય (*test_io_cb_write_blk64)
	(unsigned long long block, પૂર્ણાંક ગણતરી, errcode_t err);
બાહ્ય શૂન્ય (*test_io_cb_set_blksize)
	(પૂર્ણાંક blksize, errcode_t err);

#endif /* _EXT2FS_EXT2_IO_H */

