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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _EXT2FS_EXT2_IO_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _EXT2FS_EXT2_IO_H

#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#ਜੇ defined(__GNUC__) || defined(HAS_LONG_LONG)
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਲੰਮਾ ਲੰਮਾ	ext2_loff_t;
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਲੰਮਾ		ext2_loff_t;
#ਜੇ_ਅੰਤ

/* llseek.c */
ext2_loff_t ext2fs_llseek (ਪੂਰਨ, ext2_loff_t, ਪੂਰਨ);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ struct_io_manager *io_manager;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ struct_io_channel *io_channel;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ struct_io_stats *io_stats;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHANNEL_FLAGS_WRITETHROUGH	0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHANNEL_FLAGS_ਖੰਡ_DEVICE	0x04
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHANNEL_FLAGS_ਧਾਗੇ		0x08
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHANNEL_FLAGS_NODISCARD		0x10
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHANNEL_FLAGS_NOZEROOUT		0x20

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ io_channel_discard_zeroes_ਅੰਕੜੇ(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

ਬਣਤਰ struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	ਅੱਖਰ		*name;
	ਪੂਰਨ		ਖੰਡ_ਮਾਪ;
	errcode_t	(*read_error)(io_channel channel,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				      ਪੂਰਨ ਗਿਣਤੀ_2,
				      ਖਾਲੀ *ਅੰਕੜੇ,
				      size_t ਮਾਪ,
				      ਪੂਰਨ actual_ਬਾਈਟਾਂ_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				       ਪੂਰਨ ਗਿਣਤੀ_2,
				       ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ,
				       size_t ਮਾਪ,
				       ਪੂਰਨ actual_ਬਾਈਟਾਂ_written,
				       errcode_t error);
	ਪੂਰਨ		refcount;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	flags;
	ਲੰਮਾ		reserved[14];
	ਖਾਲੀ		*ਨਿੱਜੀ_ਅੰਕੜੇ;
	ਖਾਲੀ		*app_ਅੰਕੜੇ;
	ਪੂਰਨ		align;
};

ਬਣਤਰ struct_io_stats {
	ਪੂਰਨ			ਅੰਕ_fields;
	ਪੂਰਨ			reserved;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	ਬਾਈਟਾਂ_read;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	ਬਾਈਟਾਂ_written;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	cache_ਪ੍ਰਾਪਤੀਆਂ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	cache_ਖੁੰਝਾਂ;
};

ਬਣਤਰ struct_io_manager {
	errcode_t magic;
	ਅਚਲ ਅੱਖਰ *name;
	errcode_t (*open)(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, io_channel *channel);
	errcode_t (*close)(io_channel channel);
	errcode_t (*ਸੈੱਟ_blksize)(io_channel channel, ਪੂਰਨ blksize);
	errcode_t (*read_blk)(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			      ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *ਅੰਕੜੇ);
	errcode_t (*write_blk)(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਥ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);
	errcode_t (*ਸੈੱਟ_option)(io_channel channel, ਅਚਲ ਅੱਖਰ *option,
				ਅਚਲ ਅੱਖਰ *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
					ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *ਅੰਕੜੇ);
	errcode_t (*write_blk64)(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
					ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);
	errcode_t (*discard)(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2);
	errcode_t (*cache_readahead)(io_channel channel,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2);
	errcode_t (*zeroout)(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2);
	errcode_t (*flock)(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flock_flags);
	ਲੰਮਾ	reserved[13];
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IO_FLAG_RW		0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IO_FLAG_EXCLUSIVE	0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IO_FLAG_ਸਿੱਧਾ_IO	0x0004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IO_FLAG_FORCE_BOUNCE	0x0008
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IO_FLAG_ਧਾਗੇ		0x0010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IO_FLAG_NOCACHE		0x0020

/* Prevent other programs from reading or writing to underlying storage */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IO_CHANNEL_FLOCK_EXCLUSIVE	0x1
/* Prevent other programs from writing to underlying storage */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IO_CHANNEL_FLOCK_SHARED		0x2
/* Return EBUSY if the lock cannot be taken immediately */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IO_CHANNEL_FLOCK_TRYLOCK	0x4

/*
 * Convenience functions....
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ io_channel_close(c) 		((c)->manager->close((c)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ io_channel_ਸੈੱਟ_blksize(c,s)	((c)->manager->ਸੈੱਟ_blksize((c),s))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ io_channel_read_blk(c,b,n,d)	((c)->manager->read_blk((c),b,n,d))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ io_channel_write_blk(c,b,n,d)	((c)->manager->write_blk((c),b,n,d))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ io_channel_flush(c) 		((c)->manager->flush((c)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ io_channel_bumpcount(c)		((c)->refcount++)

/* io_manager.c */
ਬਾਹਰੀ errcode_t io_channel_ਸੈੱਟ_options(io_channel channel,
					ਅਚਲ ਅੱਖਰ *options);
ਬਾਹਰੀ errcode_t io_channel_write_byte(io_channel channel,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਿੱਥ,
				       ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);
ਬਾਹਰੀ errcode_t io_channel_read_blk64(io_channel channel,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *ਅੰਕੜੇ);
ਬਾਹਰੀ errcode_t io_channel_write_blk64(io_channel channel,
					ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
					ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);
ਬਾਹਰੀ errcode_t io_channel_discard(io_channel channel,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2);
ਬਾਹਰੀ errcode_t io_channel_zeroout(io_channel channel,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2);
ਬਾਹਰੀ errcode_t io_channel_alloc_buf(io_channel channel,
				      ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *ptr);
ਬਾਹਰੀ errcode_t io_channel_cache_readahead(io_channel io,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2);
ਬਾਹਰੀ errcode_t io_channel_flock(io_channel io, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flock_flags);
ਬਾਹਰੀ errcode_t io_channel_funlock(io_channel io);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
/* windows_io.c */
ਬਾਹਰੀ io_manager windows_io_manager;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ default_io_manager windows_io_manager
#ਨਹੀਂਤਾਂ
/* unix_io.c */
ਬਾਹਰੀ io_manager unix_io_manager;
ਬਾਹਰੀ io_manager unixfd_io_manager;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ default_io_manager unix_io_manager
#ਜੇ_ਅੰਤ

/* sparse_io.c */
ਬਾਹਰੀ io_manager sparse_io_manager;
ਬਾਹਰੀ io_manager sparsefd_io_manager;

/* undo_io.c */
ਬਾਹਰੀ io_manager undo_io_manager;
ਬਾਹਰੀ errcode_t ਸੈੱਟ_undo_io_backing_manager(io_manager manager);
ਬਾਹਰੀ errcode_t ਸੈੱਟ_undo_io_backup_file(ਅੱਖਰ *file_name);

/* test_io.c */
ਬਾਹਰੀ io_manager test_io_manager, test_io_backing_manager;
ਬਾਹਰੀ ਖਾਲੀ (*test_io_cb_read_blk)
	(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err);
ਬਾਹਰੀ ਖਾਲੀ (*test_io_cb_write_blk)
	(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err);
ਬਾਹਰੀ ਖਾਲੀ (*test_io_cb_read_blk64)
	(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err);
ਬਾਹਰੀ ਖਾਲੀ (*test_io_cb_write_blk64)
	(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err);
ਬਾਹਰੀ ਖਾਲੀ (*test_io_cb_ਸੈੱਟ_blksize)
	(ਪੂਰਨ blksize, errcode_t err);

#ਜੇ_ਅੰਤ /* _EXT2FS_EXT2_IO_H */

