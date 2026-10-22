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

#اگر_غیر_تعریف_شدہ _EXT2FS_EXT2_IO_H
#تعریف_کرو _EXT2FS_EXT2_IO_H

#شامل <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#اگر defined(__GNUC__) || defined(HAS_LONG_LONG)
قسمتعریف طویل طویل	ext2_loff_t;
#ورنہ
قسمتعریف طویل		ext2_loff_t;
#اگر_ختم

/* llseek.c */
ext2_loff_t ext2fs_llseek (صحیح, ext2_loff_t, صحیح);

قسمتعریف ساخت struct_io_manager *io_manager;
قسمتعریف ساخت struct_io_channel *io_channel;
قسمتعریف ساخت struct_io_stats *io_stats;

#تعریف_کرو CHANNEL_FLAGS_WRITETHROUGH	0x01
#تعریف_کرو CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#تعریف_کرو CHANNEL_FLAGS_بلاک_DEVICE	0x04
#تعریف_کرو CHANNEL_FLAGS_دھاگے		0x08
#تعریف_کرو CHANNEL_FLAGS_NODISCARD		0x10
#تعریف_کرو CHANNEL_FLAGS_NOZEROOUT		0x20

#تعریف_کرو io_channel_discard_zeroes_ڈیٹا(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

ساخت struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	حرف		*name;
	صحیح		بلاک_سائز;
	errcode_t	(*read_error)(io_channel channel,
				      غیرعلامتی طویل بلاک,
				      صحیح گنتی,
				      خالی *ڈیٹا,
				      size_t سائز,
				      صحیح actual_بائٹیں_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       غیرعلامتی طویل بلاک,
				       صحیح گنتی,
				       ثابت خالی *ڈیٹا,
				       size_t سائز,
				       صحیح actual_بائٹیں_written,
				       errcode_t error);
	صحیح		refcount;
	غیرعلامتی صحیح	flags;
	طویل		reserved[14];
	خالی		*نجی_ڈیٹا;
	خالی		*app_ڈیٹا;
	صحیح		align;
};

ساخت struct_io_stats {
	صحیح			عدد_fields;
	صحیح			reserved;
	غیرعلامتی طویل طویل	بائٹیں_read;
	غیرعلامتی طویل طویل	بائٹیں_written;
	غیرعلامتی طویل طویل	cache_کامیابیاں;
	غیرعلامتی طویل طویل	cache_ناکامیاں;
};

ساخت struct_io_manager {
	errcode_t magic;
	ثابت حرف *name;
	errcode_t (*open)(ثابت حرف *name, صحیح flags, io_channel *channel);
	errcode_t (*close)(io_channel channel);
	errcode_t (*سیٹ_blksize)(io_channel channel, صحیح blksize);
	errcode_t (*read_blk)(io_channel channel, غیرعلامتی طویل بلاک,
			      صحیح گنتی, خالی *ڈیٹا);
	errcode_t (*write_blk)(io_channel channel, غیرعلامتی طویل بلاک,
			       صحیح گنتی, ثابت خالی *ڈیٹا);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, غیرعلامتی طویل انحراف,
				صحیح گنتی, ثابت خالی *ڈیٹا);
	errcode_t (*سیٹ_option)(io_channel channel, ثابت حرف *option,
				ثابت حرف *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, غیرعلامتی طویل طویل بلاک,
					صحیح گنتی, خالی *ڈیٹا);
	errcode_t (*write_blk64)(io_channel channel, غیرعلامتی طویل طویل بلاک,
					صحیح گنتی, ثابت خالی *ڈیٹا);
	errcode_t (*discard)(io_channel channel, غیرعلامتی طویل طویل بلاک,
			     غیرعلامتی طویل طویل گنتی);
	errcode_t (*cache_readahead)(io_channel channel,
				     غیرعلامتی طویل طویل بلاک,
				     غیرعلامتی طویل طویل گنتی);
	errcode_t (*zeroout)(io_channel channel, غیرعلامتی طویل طویل بلاک,
			     غیرعلامتی طویل طویل گنتی);
	errcode_t (*flock)(io_channel channel, غیرعلامتی صحیح flock_flags);
	طویل	reserved[13];
};

#تعریف_کرو IO_FLAG_RW		0x0001
#تعریف_کرو IO_FLAG_EXCLUSIVE	0x0002
#تعریف_کرو IO_FLAG_براہ_راست_IO	0x0004
#تعریف_کرو IO_FLAG_FORCE_BOUNCE	0x0008
#تعریف_کرو IO_FLAG_دھاگے		0x0010
#تعریف_کرو IO_FLAG_NOCACHE		0x0020

/* Prevent other programs from reading or writing to underlying storage */
#تعریف_کرو IO_CHANNEL_FLOCK_EXCLUSIVE	0x1
/* Prevent other programs from writing to underlying storage */
#تعریف_کرو IO_CHANNEL_FLOCK_SHARED		0x2
/* Return EBUSY if the lock cannot be taken immediately */
#تعریف_کرو IO_CHANNEL_FLOCK_TRYLOCK	0x4

/*
 * Convenience functions....
 */
#تعریف_کرو io_channel_close(c) 		((c)->manager->close((c)))
#تعریف_کرو io_channel_سیٹ_blksize(c,s)	((c)->manager->سیٹ_blksize((c),s))
#تعریف_کرو io_channel_read_blk(c,b,n,d)	((c)->manager->read_blk((c),b,n,d))
#تعریف_کرو io_channel_write_blk(c,b,n,d)	((c)->manager->write_blk((c),b,n,d))
#تعریف_کرو io_channel_flush(c) 		((c)->manager->flush((c)))
#تعریف_کرو io_channel_bumpcount(c)		((c)->refcount++)

/* io_manager.c */
خارجی errcode_t io_channel_سیٹ_options(io_channel channel,
					ثابت حرف *options);
خارجی errcode_t io_channel_write_byte(io_channel channel,
				       غیرعلامتی طویل انحراف,
				       صحیح گنتی, ثابت خالی *ڈیٹا);
خارجی errcode_t io_channel_read_blk64(io_channel channel,
				       غیرعلامتی طویل طویل بلاک,
				       صحیح گنتی, خالی *ڈیٹا);
خارجی errcode_t io_channel_write_blk64(io_channel channel,
					غیرعلامتی طویل طویل بلاک,
					صحیح گنتی, ثابت خالی *ڈیٹا);
خارجی errcode_t io_channel_discard(io_channel channel,
				    غیرعلامتی طویل طویل بلاک,
				    غیرعلامتی طویل طویل گنتی);
خارجی errcode_t io_channel_zeroout(io_channel channel,
				    غیرعلامتی طویل طویل بلاک,
				    غیرعلامتی طویل طویل گنتی);
خارجی errcode_t io_channel_alloc_buf(io_channel channel,
				      صحیح گنتی, خالی *ptr);
خارجی errcode_t io_channel_cache_readahead(io_channel io,
					    غیرعلامتی طویل طویل بلاک,
					    غیرعلامتی طویل طویل گنتی);
خارجی errcode_t io_channel_flock(io_channel io, غیرعلامتی صحیح flock_flags);
خارجی errcode_t io_channel_funlock(io_channel io);

#اگر_تعریف_شدہ _WIN32
/* windows_io.c */
خارجی io_manager windows_io_manager;
#تعریف_کرو default_io_manager windows_io_manager
#ورنہ
/* unix_io.c */
خارجی io_manager unix_io_manager;
خارجی io_manager unixfd_io_manager;
#تعریف_کرو default_io_manager unix_io_manager
#اگر_ختم

/* sparse_io.c */
خارجی io_manager sparse_io_manager;
خارجی io_manager sparsefd_io_manager;

/* undo_io.c */
خارجی io_manager undo_io_manager;
خارجی errcode_t سیٹ_undo_io_backing_manager(io_manager manager);
خارجی errcode_t سیٹ_undo_io_backup_file(حرف *file_name);

/* test_io.c */
خارجی io_manager test_io_manager, test_io_backing_manager;
خارجی خالی (*test_io_cb_read_blk)
	(غیرعلامتی طویل بلاک, صحیح گنتی, errcode_t err);
خارجی خالی (*test_io_cb_write_blk)
	(غیرعلامتی طویل بلاک, صحیح گنتی, errcode_t err);
خارجی خالی (*test_io_cb_read_blk64)
	(غیرعلامتی طویل طویل بلاک, صحیح گنتی, errcode_t err);
خارجی خالی (*test_io_cb_write_blk64)
	(غیرعلامتی طویل طویل بلاک, صحیح گنتی, errcode_t err);
خارجی خالی (*test_io_cb_سیٹ_blksize)
	(صحیح blksize, errcode_t err);

#اگر_ختم /* _EXT2FS_EXT2_IO_H */

