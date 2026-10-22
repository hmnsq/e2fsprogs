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

#جے_بنا_تعریف _EXT2FS_EXT2_IO_H
#تعریف_کرو _EXT2FS_EXT2_IO_H

#ضمن <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#إذا defined(__GNUC__) || defined(HAS_LONG_LONG)
تعريفنوع لما لما	ext2_loff_t;
#وإلا
تعريفنوع لما		ext2_loff_t;
#جے_انت

/* llseek.c */
ext2_loff_t ext2fs_llseek (صحيح, ext2_loff_t, صحيح);

تعريفنوع بنية struct_io_manager *io_manager;
تعريفنوع بنية struct_io_channel *io_channel;
تعريفنوع بنية struct_io_stats *io_stats;

#تعریف_کرو CHANNEL_FLAGS_WRITETHROUGH	0x01
#تعریف_کرو CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#تعریف_کرو CHANNEL_FLAGS_بلاک_DEVICE	0x04
#تعریف_کرو CHANNEL_FLAGS_دھاگے		0x08
#تعریف_کرو CHANNEL_FLAGS_NODISCARD		0x10
#تعریف_کرو CHANNEL_FLAGS_NOZEROOUT		0x20

#تعریف_کرو io_channel_discard_zeroes_انکڑے(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

بنية struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	حرف		*name;
	صحيح		بلاک_ناپ;
	errcode_t	(*read_error)(io_channel channel,
				      بنانشانی لما بلاک,
				      صحيح گنتی,
				      فراغ *انکڑے,
				      size_t ناپ,
				      صحيح actual_بائٹاں_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       بنانشانی لما بلاک,
				       صحيح گنتی,
				       ثابت فراغ *انکڑے,
				       size_t ناپ,
				       صحيح actual_بائٹاں_written,
				       errcode_t error);
	صحيح		refcount;
	بنانشانی صحيح	flags;
	لما		reserved[14];
	فراغ		*نجی_انکڑے;
	فراغ		*app_انکڑے;
	صحيح		align;
};

بنية struct_io_stats {
	صحيح			عدد_fields;
	صحيح			reserved;
	بنانشانی لما لما	بائٹاں_read;
	بنانشانی لما لما	بائٹاں_written;
	بنانشانی لما لما	cache_کامیابیاں;
	بنانشانی لما لما	cache_کھنجاں;
};

بنية struct_io_manager {
	errcode_t magic;
	ثابت حرف *name;
	errcode_t (*open)(ثابت حرف *name, صحيح flags, io_channel *channel);
	errcode_t (*close)(io_channel channel);
	errcode_t (*سیٹ_blksize)(io_channel channel, صحيح blksize);
	errcode_t (*read_blk)(io_channel channel, بنانشانی لما بلاک,
			      صحيح گنتی, فراغ *انکڑے);
	errcode_t (*write_blk)(io_channel channel, بنانشانی لما بلاک,
			       صحيح گنتی, ثابت فراغ *انکڑے);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, بنانشانی لما وتھ,
				صحيح گنتی, ثابت فراغ *انکڑے);
	errcode_t (*سیٹ_option)(io_channel channel, ثابت حرف *option,
				ثابت حرف *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, بنانشانی لما لما بلاک,
					صحيح گنتی, فراغ *انکڑے);
	errcode_t (*write_blk64)(io_channel channel, بنانشانی لما لما بلاک,
					صحيح گنتی, ثابت فراغ *انکڑے);
	errcode_t (*discard)(io_channel channel, بنانشانی لما لما بلاک,
			     بنانشانی لما لما گنتی);
	errcode_t (*cache_readahead)(io_channel channel,
				     بنانشانی لما لما بلاک,
				     بنانشانی لما لما گنتی);
	errcode_t (*zeroout)(io_channel channel, بنانشانی لما لما بلاک,
			     بنانشانی لما لما گنتی);
	errcode_t (*flock)(io_channel channel, بنانشانی صحيح flock_flags);
	لما	reserved[13];
};

#تعریف_کرو IO_FLAG_RW		0x0001
#تعریف_کرو IO_FLAG_EXCLUSIVE	0x0002
#تعریف_کرو IO_FLAG_سدھا_IO	0x0004
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
خارجي errcode_t io_channel_سیٹ_options(io_channel channel,
					ثابت حرف *options);
خارجي errcode_t io_channel_write_byte(io_channel channel,
				       بنانشانی لما وتھ,
				       صحيح گنتی, ثابت فراغ *انکڑے);
خارجي errcode_t io_channel_read_blk64(io_channel channel,
				       بنانشانی لما لما بلاک,
				       صحيح گنتی, فراغ *انکڑے);
خارجي errcode_t io_channel_write_blk64(io_channel channel,
					بنانشانی لما لما بلاک,
					صحيح گنتی, ثابت فراغ *انکڑے);
خارجي errcode_t io_channel_discard(io_channel channel,
				    بنانشانی لما لما بلاک,
				    بنانشانی لما لما گنتی);
خارجي errcode_t io_channel_zeroout(io_channel channel,
				    بنانشانی لما لما بلاک,
				    بنانشانی لما لما گنتی);
خارجي errcode_t io_channel_alloc_buf(io_channel channel,
				      صحيح گنتی, فراغ *ptr);
خارجي errcode_t io_channel_cache_readahead(io_channel io,
					    بنانشانی لما لما بلاک,
					    بنانشانی لما لما گنتی);
خارجي errcode_t io_channel_flock(io_channel io, بنانشانی صحيح flock_flags);
خارجي errcode_t io_channel_funlock(io_channel io);

#جے_تعریف_شدہ _WIN32
/* windows_io.c */
خارجي io_manager windows_io_manager;
#تعریف_کرو default_io_manager windows_io_manager
#وإلا
/* unix_io.c */
خارجي io_manager unix_io_manager;
خارجي io_manager unixfd_io_manager;
#تعریف_کرو default_io_manager unix_io_manager
#جے_انت

/* sparse_io.c */
خارجي io_manager sparse_io_manager;
خارجي io_manager sparsefd_io_manager;

/* undo_io.c */
خارجي io_manager undo_io_manager;
خارجي errcode_t سیٹ_undo_io_backing_manager(io_manager manager);
خارجي errcode_t سیٹ_undo_io_backup_file(حرف *file_name);

/* test_io.c */
خارجي io_manager test_io_manager, test_io_backing_manager;
خارجي فراغ (*test_io_cb_read_blk)
	(بنانشانی لما بلاک, صحيح گنتی, errcode_t err);
خارجي فراغ (*test_io_cb_write_blk)
	(بنانشانی لما بلاک, صحيح گنتی, errcode_t err);
خارجي فراغ (*test_io_cb_read_blk64)
	(بنانشانی لما لما بلاک, صحيح گنتی, errcode_t err);
خارجي فراغ (*test_io_cb_write_blk64)
	(بنانشانی لما لما بلاک, صحيح گنتی, errcode_t err);
خارجي فراغ (*test_io_cb_سیٹ_blksize)
	(صحيح blksize, errcode_t err);

#جے_انت /* _EXT2FS_EXT2_IO_H */

