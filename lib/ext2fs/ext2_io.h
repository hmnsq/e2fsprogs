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

#अगर_अघोषित _EXT2FS_EXT2_IO_H
#घोषणा _EXT2FS_EXT2_IO_H

#समावेश <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#अगर defined(__GNUC__) || defined(HAS_LONG_LONG)
प्रकार दीर्घ दीर्घ	ext2_loff_t;
#अन्यथा
प्रकार दीर्घ		ext2_loff_t;
#पूर्ण_अगर

/* llseek.c */
ext2_loff_t ext2fs_llseek (पूर्णांक, ext2_loff_t, पूर्णांक);

प्रकार काष्ठा struct_io_manager *io_manager;
प्रकार काष्ठा struct_io_channel *io_channel;
प्रकार काष्ठा struct_io_stats *io_stats;

#घोषणा CHANNEL_FLAGS_WRITETHROUGH	0x01
#घोषणा CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#घोषणा CHANNEL_FLAGS_BLOCK_DEVICE	0x04
#घोषणा CHANNEL_FLAGS_THREADS		0x08
#घोषणा CHANNEL_FLAGS_NODISCARD		0x10
#घोषणा CHANNEL_FLAGS_NOZEROOUT		0x20

#घोषणा io_channel_discard_zeroes_तथ्य(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

काष्ठा struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	अक्षर		*name;
	पूर्णांक		block_आकार;
	errcode_t	(*read_error)(io_channel channel,
				      अचिन्हित दीर्घ block,
				      पूर्णांक गिनती,
				      व्योम *तथ्य,
				      माप_प्रकार आकार,
				      पूर्णांक actual_bytes_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       अचिन्हित दीर्घ block,
				       पूर्णांक गिनती,
				       स्थिर व्योम *तथ्य,
				       माप_प्रकार आकार,
				       पूर्णांक actual_bytes_written,
				       errcode_t error);
	पूर्णांक		refcount;
	अचिन्हित पूर्णांक	flags;
	दीर्घ		reserved[14];
	व्योम		*private_तथ्य;
	व्योम		*app_तथ्य;
	पूर्णांक		align;
};

काष्ठा struct_io_stats {
	पूर्णांक			अंक_fields;
	पूर्णांक			reserved;
	अचिन्हित दीर्घ दीर्घ	bytes_read;
	अचिन्हित दीर्घ दीर्घ	bytes_written;
	अचिन्हित दीर्घ दीर्घ	cache_hits;
	अचिन्हित दीर्घ दीर्घ	cache_misses;
};

काष्ठा struct_io_manager {
	errcode_t magic;
	स्थिर अक्षर *name;
	errcode_t (*खोलो)(स्थिर अक्षर *name, पूर्णांक flags, io_channel *channel);
	errcode_t (*बंद)(io_channel channel);
	errcode_t (*set_blksize)(io_channel channel, पूर्णांक blksize);
	errcode_t (*read_blk)(io_channel channel, अचिन्हित दीर्घ block,
			      पूर्णांक गिनती, व्योम *तथ्य);
	errcode_t (*write_blk)(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक गिनती, स्थिर व्योम *तथ्य);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, अचिन्हित दीर्घ offset,
				पूर्णांक गिनती, स्थिर व्योम *तथ्य);
	errcode_t (*set_option)(io_channel channel, स्थिर अक्षर *option,
				स्थिर अक्षर *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
					पूर्णांक गिनती, व्योम *तथ्य);
	errcode_t (*write_blk64)(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
					पूर्णांक गिनती, स्थिर व्योम *तथ्य);
	errcode_t (*discard)(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ गिनती);
	errcode_t (*cache_readahead)(io_channel channel,
				     अचिन्हित दीर्घ दीर्घ block,
				     अचिन्हित दीर्घ दीर्घ गिनती);
	errcode_t (*zeroout)(io_channel channel, अचिन्हित दीर्घ दीर्घ block,
			     अचिन्हित दीर्घ दीर्घ गिनती);
	errcode_t (*flock)(io_channel channel, अचिन्हित पूर्णांक flock_flags);
	दीर्घ	reserved[13];
};

#घोषणा IO_FLAG_RW		0x0001
#घोषणा IO_FLAG_EXCLUSIVE	0x0002
#घोषणा IO_FLAG_DIRECT_IO	0x0004
#घोषणा IO_FLAG_FORCE_BOUNCE	0x0008
#घोषणा IO_FLAG_THREADS		0x0010
#घोषणा IO_FLAG_NOCACHE		0x0020

/* Prevent other programs from reading or writing to underlying storage */
#घोषणा IO_CHANNEL_FLOCK_EXCLUSIVE	0x1
/* Prevent other programs from writing to underlying storage */
#घोषणा IO_CHANNEL_FLOCK_SHARED		0x2
/* Return EBUSY if the lock cannot be taken immediately */
#घोषणा IO_CHANNEL_FLOCK_TRYLOCK	0x4

/*
 * Convenience functions....
 */
#घोषणा io_channel_close(c) 		((c)->manager->बंद((c)))
#घोषणा io_channel_set_blksize(c,s)	((c)->manager->set_blksize((c),s))
#घोषणा io_channel_read_blk(c,b,n,d)	((c)->manager->read_blk((c),b,n,d))
#घोषणा io_channel_write_blk(c,b,n,d)	((c)->manager->write_blk((c),b,n,d))
#घोषणा io_channel_flush(c) 		((c)->manager->flush((c)))
#घोषणा io_channel_bumpcount(c)		((c)->refcount++)

/* io_manager.c */
बाह्य errcode_t io_channel_set_options(io_channel channel,
					स्थिर अक्षर *options);
बाह्य errcode_t io_channel_write_byte(io_channel channel,
				       अचिन्हित दीर्घ offset,
				       पूर्णांक गिनती, स्थिर व्योम *तथ्य);
बाह्य errcode_t io_channel_read_blk64(io_channel channel,
				       अचिन्हित दीर्घ दीर्घ block,
				       पूर्णांक गिनती, व्योम *तथ्य);
बाह्य errcode_t io_channel_write_blk64(io_channel channel,
					अचिन्हित दीर्घ दीर्घ block,
					पूर्णांक गिनती, स्थिर व्योम *तथ्य);
बाह्य errcode_t io_channel_discard(io_channel channel,
				    अचिन्हित दीर्घ दीर्घ block,
				    अचिन्हित दीर्घ दीर्घ गिनती);
बाह्य errcode_t io_channel_zeroout(io_channel channel,
				    अचिन्हित दीर्घ दीर्घ block,
				    अचिन्हित दीर्घ दीर्घ गिनती);
बाह्य errcode_t io_channel_alloc_buf(io_channel channel,
				      पूर्णांक गिनती, व्योम *ptr);
बाह्य errcode_t io_channel_cache_readahead(io_channel io,
					    अचिन्हित दीर्घ दीर्घ block,
					    अचिन्हित दीर्घ दीर्घ गिनती);
बाह्य errcode_t io_channel_flock(io_channel io, अचिन्हित पूर्णांक flock_flags);
बाह्य errcode_t io_channel_funlock(io_channel io);

#अगर_घोषित _WIN32
/* windows_io.c */
बाह्य io_manager windows_io_manager;
#घोषणा default_io_manager windows_io_manager
#अन्यथा
/* unix_io.c */
बाह्य io_manager unix_io_manager;
बाह्य io_manager unixfd_io_manager;
#घोषणा default_io_manager unix_io_manager
#पूर्ण_अगर

/* sparse_io.c */
बाह्य io_manager sparse_io_manager;
बाह्य io_manager sparsefd_io_manager;

/* undo_io.c */
बाह्य io_manager undo_io_manager;
बाह्य errcode_t set_undo_io_backing_manager(io_manager manager);
बाह्य errcode_t set_undo_io_backup_file(अक्षर *file_name);

/* test_io.c */
बाह्य io_manager test_io_manager, test_io_backing_manager;
बाह्य व्योम (*test_io_cb_read_blk)
	(अचिन्हित दीर्घ block, पूर्णांक गिनती, errcode_t err);
बाह्य व्योम (*test_io_cb_write_blk)
	(अचिन्हित दीर्घ block, पूर्णांक गिनती, errcode_t err);
बाह्य व्योम (*test_io_cb_read_blk64)
	(अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, errcode_t err);
बाह्य व्योम (*test_io_cb_write_blk64)
	(अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, errcode_t err);
बाह्य व्योम (*test_io_cb_set_blksize)
	(पूर्णांक blksize, errcode_t err);

#पूर्ण_अगर /* _EXT2FS_EXT2_IO_H */

