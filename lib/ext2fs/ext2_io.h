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

#अन्तर्भाव <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#यदि defined(__GNUC__) || defined(HAS_LONG_LONG)
प्रकारनिर्देश दीर्घ दीर्घ	ext2_loff_t;
#अन्यथा
प्रकारनिर्देश दीर्घ		ext2_loff_t;
#endif

/* llseek.c */
ext2_loff_t ext2fs_llseek (पूर्णाङ्क, ext2_loff_t, पूर्णाङ्क);

प्रकारनिर्देश संरचना struct_io_manager *io_manager;
प्रकारनिर्देश संरचना struct_io_channel *io_channel;
प्रकारनिर्देश संरचना struct_io_stats *io_stats;

#define CHANNEL_FLAGS_WRITETHROUGH	0x01
#define CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#define CHANNEL_FLAGS_BLOCK_DEVICE	0x04
#define CHANNEL_FLAGS_THREADS		0x08
#define CHANNEL_FLAGS_NODISCARD		0x10
#define CHANNEL_FLAGS_NOZEROOUT		0x20

#define io_channel_discard_zeroes_data(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

संरचना struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	वर्ण		*name;
	पूर्णाङ्क		block_size;
	errcode_t	(*read_error)(io_channel channel,
				      अचिन्हित दीर्घ block,
				      पूर्णाङ्क गन्ती,
				      शून्य *data,
				      माप_प्रकार size,
				      पूर्णाङ्क actual_bytes_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       अचिन्हित दीर्घ block,
				       पूर्णाङ्क गन्ती,
				       अचर शून्य *data,
				       माप_प्रकार size,
				       पूर्णाङ्क actual_bytes_written,
				       errcode_t error);
	पूर्णाङ्क		refcount;
	अचिन्हित पूर्णाङ्क	flags;
	दीर्घ		reserved[14];
	शून्य		*private_data;
	शून्य		*app_data;
	पूर्णाङ्क		align;
};

संरचना struct_io_stats {
	पूर्णाङ्क			अङ्क_fields;
	पूर्णाङ्क			reserved;
	अचिन्हित दीर्घ दीर्घ	bytes_read;
	अचिन्हित दीर्घ दीर्घ	bytes_written;
	अचिन्हित दीर्घ दीर्घ	cache_hits;
	अचिन्हित दीर्घ दीर्घ	cache_misses;
};

संरचना struct_io_manager {
	errcode_t magic;
	अचर वर्ण *name;
	errcode_t (*खोलो)(अचर वर्ण *name, पूर्णाङ्क flags, io_channel *channel);
	errcode_t (*बंद)(io_channel channel);
	errcode_t (*set_blksize)(io_channel channel, पूर्णाङ्क blksize);
	errcode_t (*read_blk)(io_channel channel, अचिन्हित दीर्घ block,
			      पूर्णाङ्क गन्ती, शून्य *data);
	errcode_t (*write_blk)(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णाङ्क गन्ती, अचर शून्य *data);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, अचिन्हित दीर्घ offset,
				पूर्णाङ्क गन्ती, अचर शून्य *data);
	errcode_t (*set_option)(io_channel channel, अचर वर्ण *option,
				अचर वर्ण *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
					पूर्णाङ्क गन्ती, शून्य *data);
	errcode_t (*write_blk64)(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
					पूर्णाङ्क गन्ती, अचर शून्य *data);
	errcode_t (*discard)(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ गन्ती);
	errcode_t (*cache_readahead)(io_channel channel,
				     अचिन्हित दीर्घ दीर्घ block,
				     अचिन्हित दीर्घ दीर्घ गन्ती);
	errcode_t (*zeroout)(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ गन्ती);
	errcode_t (*flock)(io_channel channel, अचिन्हित पूर्णाङ्क flock_flags);
	दीर्घ	reserved[13];
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
#define io_channel_close(c) 		((c)->manager->बंद((c)))
#define io_channel_set_blksize(c,s)	((c)->manager->set_blksize((c),s))
#define io_channel_read_blk(c,b,n,d)	((c)->manager->read_blk((c),b,n,d))
#define io_channel_write_blk(c,b,n,d)	((c)->manager->write_blk((c),b,n,d))
#define io_channel_flush(c) 		((c)->manager->flush((c)))
#define io_channel_bumpcount(c)		((c)->refcount++)

/* io_manager.c */
बाह्य errcode_t io_channel_set_options(io_channel channel,
					अचर वर्ण *options);
बाह्य errcode_t io_channel_write_byte(io_channel channel,
				       अचिन्हित दीर्घ offset,
				       पूर्णाङ्क गन्ती, अचर शून्य *data);
बाह्य errcode_t io_channel_read_blk64(io_channel channel,
				       अचिन्हित दीर्घ दीर्घ block,
				       पूर्णाङ्क गन्ती, शून्य *data);
बाह्य errcode_t io_channel_write_blk64(io_channel channel,
					अचिन्हित दीर्घ दीर्घ block,
					पूर्णाङ्क गन्ती, अचर शून्य *data);
बाह्य errcode_t io_channel_discard(io_channel channel,
				    अचिन्हित दीर्घ दीर्घ block,
				    अचिन्हित दीर्घ दीर्घ गन्ती);
बाह्य errcode_t io_channel_zeroout(io_channel channel,
				    अचिन्हित दीर्घ दीर्घ block,
				    अचिन्हित दीर्घ दीर्घ गन्ती);
बाह्य errcode_t io_channel_alloc_buf(io_channel channel,
				      पूर्णाङ्क गन्ती, शून्य *ptr);
बाह्य errcode_t io_channel_cache_readahead(io_channel io,
					    अचिन्हित दीर्घ दीर्घ block,
					    अचिन्हित दीर्घ दीर्घ गन्ती);
बाह्य errcode_t io_channel_flock(io_channel io, अचिन्हित पूर्णाङ्क flock_flags);
बाह्य errcode_t io_channel_funlock(io_channel io);

#ifdef _WIN32
/* windows_io.c */
बाह्य io_manager windows_io_manager;
#define default_io_manager windows_io_manager
#अन्यथा
/* unix_io.c */
बाह्य io_manager unix_io_manager;
बाह्य io_manager unixfd_io_manager;
#define default_io_manager unix_io_manager
#endif

/* sparse_io.c */
बाह्य io_manager sparse_io_manager;
बाह्य io_manager sparsefd_io_manager;

/* undo_io.c */
बाह्य io_manager undo_io_manager;
बाह्य errcode_t set_undo_io_backing_manager(io_manager manager);
बाह्य errcode_t set_undo_io_backup_file(वर्ण *file_name);

/* test_io.c */
बाह्य io_manager test_io_manager, test_io_backing_manager;
बाह्य शून्य (*test_io_cb_read_blk)
	(अचिन्हित दीर्घ block, पूर्णाङ्क गन्ती, errcode_t err);
बाह्य शून्य (*test_io_cb_write_blk)
	(अचिन्हित दीर्घ block, पूर्णाङ्क गन्ती, errcode_t err);
बाह्य शून्य (*test_io_cb_read_blk64)
	(अचिन्हित दीर्घ दीर्घ block, पूर्णाङ्क गन्ती, errcode_t err);
बाह्य शून्य (*test_io_cb_write_blk64)
	(अचिन्हित दीर्घ दीर्घ block, पूर्णाङ्क गन्ती, errcode_t err);
बाह्य शून्य (*test_io_cb_set_blksize)
	(पूर्णाङ्क blksize, errcode_t err);

#endif /* _EXT2FS_EXT2_IO_H */

