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

#جيڪڏهن_وصف_نه_ٿيل _EXT2FS_EXT2_IO_H
#وصف_ڪريو _EXT2FS_EXT2_IO_H

#ضمن <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#إذا defined(__GNUC__) || defined(HAS_LONG_LONG)
تعريفنوع ڊگهو ڊگهو	ext2_loff_t;
#وإلا
تعريفنوع ڊگهو		ext2_loff_t;
#جيڪڏهن_پورو

/* llseek.c */
ext2_loff_t ext2fs_llseek (صحيح, ext2_loff_t, صحيح);

تعريفنوع بنية struct_io_manager *io_manager;
تعريفنوع بنية struct_io_channel *io_channel;
تعريفنوع بنية struct_io_stats *io_stats;

#وصف_ڪريو CHANNEL_FLAGS_WRITETHROUGH	0x01
#وصف_ڪريو CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#وصف_ڪريو CHANNEL_FLAGS_بلاڪ_DEVICE	0x04
#وصف_ڪريو CHANNEL_FLAGS_ڌاڳا		0x08
#وصف_ڪريو CHANNEL_FLAGS_NODISCARD		0x10
#وصف_ڪريو CHANNEL_FLAGS_NOZEROOUT		0x20

#وصف_ڪريو io_channel_discard_zeroes_معلومات(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

بنية struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	حرف		*name;
	صحيح		بلاڪ_سائيز;
	errcode_t	(*read_error)(io_channel channel,
				      بنانشان ڊگهو بلاڪ,
				      صحيح ڳڻپ,
				      فراغ *معلومات,
				      size_t سائيز,
				      صحيح actual_بائيٽون_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       بنانشان ڊگهو بلاڪ,
				       صحيح ڳڻپ,
				       ثابت فراغ *معلومات,
				       size_t سائيز,
				       صحيح actual_بائيٽون_written,
				       errcode_t error);
	صحيح		refcount;
	بنانشان صحيح	flags;
	ڊگهو		reserved[14];
	فراغ		*ذاتي_معلومات;
	فراغ		*app_معلومات;
	صحيح		align;
};

بنية struct_io_stats {
	صحيح			انگ_fields;
	صحيح			reserved;
	بنانشان ڊگهو ڊگهو	بائيٽون_read;
	بنانشان ڊگهو ڊگهو	بائيٽون_written;
	بنانشان ڊگهو ڊگهو	cache_ڪاميابيون;
	بنانشان ڊگهو ڊگهو	cache_ناڪاميون;
};

بنية struct_io_manager {
	errcode_t magic;
	ثابت حرف *name;
	errcode_t (*open)(ثابت حرف *name, صحيح flags, io_channel *channel);
	errcode_t (*close)(io_channel channel);
	errcode_t (*سيٽ_blksize)(io_channel channel, صحيح blksize);
	errcode_t (*read_blk)(io_channel channel, بنانشان ڊگهو بلاڪ,
			      صحيح ڳڻپ, فراغ *معلومات);
	errcode_t (*write_blk)(io_channel channel, بنانشان ڊگهو بلاڪ,
			       صحيح ڳڻپ, ثابت فراغ *معلومات);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, بنانشان ڊگهو وٿي,
				صحيح ڳڻپ, ثابت فراغ *معلومات);
	errcode_t (*سيٽ_option)(io_channel channel, ثابت حرف *option,
				ثابت حرف *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
					صحيح ڳڻپ, فراغ *معلومات);
	errcode_t (*write_blk64)(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
					صحيح ڳڻپ, ثابت فراغ *معلومات);
	errcode_t (*discard)(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			     بنانشان ڊگهو ڊگهو ڳڻپ);
	errcode_t (*cache_readahead)(io_channel channel,
				     بنانشان ڊگهو ڊگهو بلاڪ,
				     بنانشان ڊگهو ڊگهو ڳڻپ);
	errcode_t (*zeroout)(io_channel channel, بنانشان ڊگهو ڊگهو بلاڪ,
			     بنانشان ڊگهو ڊگهو ڳڻپ);
	errcode_t (*flock)(io_channel channel, بنانشان صحيح flock_flags);
	ڊگهو	reserved[13];
};

#وصف_ڪريو IO_FLAG_RW		0x0001
#وصف_ڪريو IO_FLAG_EXCLUSIVE	0x0002
#وصف_ڪريو IO_FLAG_سڌو_IO	0x0004
#وصف_ڪريو IO_FLAG_FORCE_BOUNCE	0x0008
#وصف_ڪريو IO_FLAG_ڌاڳا		0x0010
#وصف_ڪريو IO_FLAG_NOCACHE		0x0020

/* Prevent other programs from reading or writing to underlying storage */
#وصف_ڪريو IO_CHANNEL_FLOCK_EXCLUSIVE	0x1
/* Prevent other programs from writing to underlying storage */
#وصف_ڪريو IO_CHANNEL_FLOCK_SHARED		0x2
/* Return EBUSY if the lock cannot be taken immediately */
#وصف_ڪريو IO_CHANNEL_FLOCK_TRYLOCK	0x4

/*
 * Convenience functions....
 */
#وصف_ڪريو io_channel_close(c) 		((c)->manager->close((c)))
#وصف_ڪريو io_channel_سيٽ_blksize(c,s)	((c)->manager->سيٽ_blksize((c),s))
#وصف_ڪريو io_channel_read_blk(c,b,n,d)	((c)->manager->read_blk((c),b,n,d))
#وصف_ڪريو io_channel_write_blk(c,b,n,d)	((c)->manager->write_blk((c),b,n,d))
#وصف_ڪريو io_channel_flush(c) 		((c)->manager->flush((c)))
#وصف_ڪريو io_channel_bumpcount(c)		((c)->refcount++)

/* io_manager.c */
خارجي errcode_t io_channel_سيٽ_options(io_channel channel,
					ثابت حرف *options);
خارجي errcode_t io_channel_write_byte(io_channel channel,
				       بنانشان ڊگهو وٿي,
				       صحيح ڳڻپ, ثابت فراغ *معلومات);
خارجي errcode_t io_channel_read_blk64(io_channel channel,
				       بنانشان ڊگهو ڊگهو بلاڪ,
				       صحيح ڳڻپ, فراغ *معلومات);
خارجي errcode_t io_channel_write_blk64(io_channel channel,
					بنانشان ڊگهو ڊگهو بلاڪ,
					صحيح ڳڻپ, ثابت فراغ *معلومات);
خارجي errcode_t io_channel_discard(io_channel channel,
				    بنانشان ڊگهو ڊگهو بلاڪ,
				    بنانشان ڊگهو ڊگهو ڳڻپ);
خارجي errcode_t io_channel_zeroout(io_channel channel,
				    بنانشان ڊگهو ڊگهو بلاڪ,
				    بنانشان ڊگهو ڊگهو ڳڻپ);
خارجي errcode_t io_channel_alloc_buf(io_channel channel,
				      صحيح ڳڻپ, فراغ *ptr);
خارجي errcode_t io_channel_cache_readahead(io_channel io,
					    بنانشان ڊگهو ڊگهو بلاڪ,
					    بنانشان ڊگهو ڊگهو ڳڻپ);
خارجي errcode_t io_channel_flock(io_channel io, بنانشان صحيح flock_flags);
خارجي errcode_t io_channel_funlock(io_channel io);

#جيڪڏهن_وصف_ٿيل _WIN32
/* windows_io.c */
خارجي io_manager windows_io_manager;
#وصف_ڪريو default_io_manager windows_io_manager
#وإلا
/* unix_io.c */
خارجي io_manager unix_io_manager;
خارجي io_manager unixfd_io_manager;
#وصف_ڪريو default_io_manager unix_io_manager
#جيڪڏهن_پورو

/* sparse_io.c */
خارجي io_manager sparse_io_manager;
خارجي io_manager sparsefd_io_manager;

/* undo_io.c */
خارجي io_manager undo_io_manager;
خارجي errcode_t سيٽ_undo_io_backing_manager(io_manager manager);
خارجي errcode_t سيٽ_undo_io_backup_file(حرف *file_name);

/* test_io.c */
خارجي io_manager test_io_manager, test_io_backing_manager;
خارجي فراغ (*test_io_cb_read_blk)
	(بنانشان ڊگهو بلاڪ, صحيح ڳڻپ, errcode_t err);
خارجي فراغ (*test_io_cb_write_blk)
	(بنانشان ڊگهو بلاڪ, صحيح ڳڻپ, errcode_t err);
خارجي فراغ (*test_io_cb_read_blk64)
	(بنانشان ڊگهو ڊگهو بلاڪ, صحيح ڳڻپ, errcode_t err);
خارجي فراغ (*test_io_cb_write_blk64)
	(بنانشان ڊگهو ڊگهو بلاڪ, صحيح ڳڻپ, errcode_t err);
خارجي فراغ (*test_io_cb_سيٽ_blksize)
	(صحيح blksize, errcode_t err);

#جيڪڏهن_پورو /* _EXT2FS_EXT2_IO_H */

