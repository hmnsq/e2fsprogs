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

#यदि_अपरिभाषितम् _EXT2FS_EXT2_IO_H
#परिभाषय _EXT2FS_EXT2_IO_H

#अन्तर्भाव <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#यदि defined(__GNUC__) || defined(HAS_LONG_LONG)
प्रकारनिर्देश दीर्घ दीर्घ	ext2_loff_t;
#अन्यथा
प्रकारनिर्देश दीर्घ		ext2_loff_t;
#यदि_अन्तः

/* llseek.c */
ext2_loff_t ext2fs_llseek (पूर्णाङ्क, ext2_loff_t, पूर्णाङ्क);

प्रकारनिर्देश संरचना struct_io_manager *io_manager;
प्रकारनिर्देश संरचना struct_io_channel *io_channel;
प्रकारनिर्देश संरचना struct_io_stats *io_stats;

#परिभाषय CHANNEL_FLAGS_WRITETHROUGH	0x01
#परिभाषय CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#परिभाषय CHANNEL_FLAGS_खण्डम्_DEVICE	0x04
#परिभाषय CHANNEL_FLAGS_तन्तवः		0x08
#परिभाषय CHANNEL_FLAGS_NODISCARD		0x10
#परिभाषय CHANNEL_FLAGS_NOZEROOUT		0x20

#परिभाषय io_channel_discard_zeroes_दत्त(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

संरचना struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	वर्ण		*name;
	पूर्णाङ्क		खण्डम्_आकार;
	errcode_t	(*read_error)(io_channel channel,
				      अचिह्नित दीर्घ खण्डम्,
				      पूर्णाङ्क गणनम्,
				      शून्य *दत्त,
				      size_t आकार,
				      पूर्णाङ्क actual_अष्टकानि_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       अचिह्नित दीर्घ खण्डम्,
				       पूर्णाङ्क गणनम्,
				       ध्रुव शून्य *दत्त,
				       size_t आकार,
				       पूर्णाङ्क actual_अष्टकानि_written,
				       errcode_t error);
	पूर्णाङ्क		refcount;
	अचिह्नित पूर्णाङ्क	flags;
	दीर्घ		reserved[14];
	शून्य		*वैयक्तिकम्_दत्त;
	शून्य		*app_दत्त;
	पूर्णाङ्क		align;
};

संरचना struct_io_stats {
	पूर्णाङ्क			अङ्कः_fields;
	पूर्णाङ्क			reserved;
	अचिह्नित दीर्घ दीर्घ	अष्टकानि_read;
	अचिह्नित दीर्घ दीर्घ	अष्टकानि_written;
	अचिह्नित दीर्घ दीर्घ	cache_लब्धानि;
	अचिह्नित दीर्घ दीर्घ	cache_अलब्धानि;
};

संरचना struct_io_manager {
	errcode_t magic;
	ध्रुव वर्ण *name;
	errcode_t (*उद्घाटय)(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel);
	errcode_t (*close)(io_channel channel);
	errcode_t (*समुच्चयः_blksize)(io_channel channel, पूर्णाङ्क blksize);
	errcode_t (*read_blk)(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			      पूर्णाङ्क गणनम्, शून्य *दत्त);
	errcode_t (*write_blk)(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, ध्रुव शून्य *दत्त);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, अचिह्नित दीर्घ विस्थापनम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *दत्त);
	errcode_t (*समुच्चयः_option)(io_channel channel, ध्रुव वर्ण *option,
				ध्रुव वर्ण *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
					पूर्णाङ्क गणनम्, शून्य *दत्त);
	errcode_t (*write_blk64)(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
					पूर्णाङ्क गणनम्, ध्रुव शून्य *दत्त);
	errcode_t (*discard)(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			     अचिह्नित दीर्घ दीर्घ गणनम्);
	errcode_t (*cache_readahead)(io_channel channel,
				     अचिह्नित दीर्घ दीर्घ खण्डम्,
				     अचिह्नित दीर्घ दीर्घ गणनम्);
	errcode_t (*zeroout)(io_channel channel, अचिह्नित दीर्घ दीर्घ खण्डम्,
			     अचिह्नित दीर्घ दीर्घ गणनम्);
	errcode_t (*flock)(io_channel channel, अचिह्नित पूर्णाङ्क flock_flags);
	दीर्घ	reserved[13];
};

#परिभाषय IO_FLAG_RW		0x0001
#परिभाषय IO_FLAG_EXCLUSIVE	0x0002
#परिभाषय IO_FLAG_प्रत्यक्षम्_IO	0x0004
#परिभाषय IO_FLAG_FORCE_BOUNCE	0x0008
#परिभाषय IO_FLAG_तन्तवः		0x0010
#परिभाषय IO_FLAG_NOCACHE		0x0020

/* Prevent other programs from reading or writing to underlying storage */
#परिभाषय IO_CHANNEL_FLOCK_EXCLUSIVE	0x1
/* Prevent other programs from writing to underlying storage */
#परिभाषय IO_CHANNEL_FLOCK_SHARED		0x2
/* Return EBUSY if the lock cannot be taken immediately */
#परिभाषय IO_CHANNEL_FLOCK_TRYLOCK	0x4

/*
 * Convenience functions....
 */
#परिभाषय io_channel_close(c) 		((c)->manager->close((c)))
#परिभाषय io_channel_समुच्चयः_blksize(c,s)	((c)->manager->समुच्चयः_blksize((c),s))
#परिभाषय io_channel_read_blk(c,b,n,d)	((c)->manager->read_blk((c),b,n,d))
#परिभाषय io_channel_write_blk(c,b,n,d)	((c)->manager->write_blk((c),b,n,d))
#परिभाषय io_channel_flush(c) 		((c)->manager->flush((c)))
#परिभाषय io_channel_bumpcount(c)		((c)->refcount++)

/* io_manager.c */
बाह्य errcode_t io_channel_समुच्चयः_options(io_channel channel,
					ध्रुव वर्ण *options);
बाह्य errcode_t io_channel_write_byte(io_channel channel,
				       अचिह्नित दीर्घ विस्थापनम्,
				       पूर्णाङ्क गणनम्, ध्रुव शून्य *दत्त);
बाह्य errcode_t io_channel_read_blk64(io_channel channel,
				       अचिह्नित दीर्घ दीर्घ खण्डम्,
				       पूर्णाङ्क गणनम्, शून्य *दत्त);
बाह्य errcode_t io_channel_write_blk64(io_channel channel,
					अचिह्नित दीर्घ दीर्घ खण्डम्,
					पूर्णाङ्क गणनम्, ध्रुव शून्य *दत्त);
बाह्य errcode_t io_channel_discard(io_channel channel,
				    अचिह्नित दीर्घ दीर्घ खण्डम्,
				    अचिह्नित दीर्घ दीर्घ गणनम्);
बाह्य errcode_t io_channel_zeroout(io_channel channel,
				    अचिह्नित दीर्घ दीर्घ खण्डम्,
				    अचिह्नित दीर्घ दीर्घ गणनम्);
बाह्य errcode_t io_channel_alloc_buf(io_channel channel,
				      पूर्णाङ्क गणनम्, शून्य *ptr);
बाह्य errcode_t io_channel_cache_readahead(io_channel io,
					    अचिह्नित दीर्घ दीर्घ खण्डम्,
					    अचिह्नित दीर्घ दीर्घ गणनम्);
बाह्य errcode_t io_channel_flock(io_channel io, अचिह्नित पूर्णाङ्क flock_flags);
बाह्य errcode_t io_channel_funlock(io_channel io);

#यदि_परिभाषितम् _WIN32
/* windows_io.c */
बाह्य io_manager windows_io_manager;
#परिभाषय default_io_manager windows_io_manager
#अन्यथा
/* unix_io.c */
बाह्य io_manager unix_io_manager;
बाह्य io_manager unixfd_io_manager;
#परिभाषय default_io_manager unix_io_manager
#यदि_अन्तः

/* sparse_io.c */
बाह्य io_manager sparse_io_manager;
बाह्य io_manager sparsefd_io_manager;

/* undo_io.c */
बाह्य io_manager undo_io_manager;
बाह्य errcode_t समुच्चयः_undo_io_backing_manager(io_manager manager);
बाह्य errcode_t समुच्चयः_undo_io_backup_file(वर्ण *file_name);

/* test_io.c */
बाह्य io_manager test_io_manager, test_io_backing_manager;
बाह्य शून्य (*test_io_cb_read_blk)
	(अचिह्नित दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err);
बाह्य शून्य (*test_io_cb_write_blk)
	(अचिह्नित दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err);
बाह्य शून्य (*test_io_cb_read_blk64)
	(अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err);
बाह्य शून्य (*test_io_cb_write_blk64)
	(अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err);
बाह्य शून्य (*test_io_cb_समुच्चयः_blksize)
	(पूर्णाङ्क blksize, errcode_t err);

#यदि_अन्तः /* _EXT2FS_EXT2_IO_H */

