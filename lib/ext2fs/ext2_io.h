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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _EXT2FS_EXT2_IO_H
#ವ್ಯಾಖ್ಯಾನಿಸು _EXT2FS_EXT2_IO_H

#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#ಆದರೆ defined(__GNUC__) || defined(HAS_LONG_LONG)
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದೀರ್ಘ ದೀರ್ಘ	ext2_loff_t;
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದೀರ್ಘ		ext2_loff_t;
#ಆದರೆ_ಅಂತ್ಯ

/* llseek.c */
ext2_loff_t ext2fs_llseek (ಪೂರ್ಣಾಂಕ, ext2_loff_t, ಪೂರ್ಣಾಂಕ);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ struct_io_manager *io_manager;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ struct_io_channel *io_channel;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ struct_io_stats *io_stats;

#ವ್ಯಾಖ್ಯಾನಿಸು CHANNEL_FLAGS_WRITETHROUGH	0x01
#ವ್ಯಾಖ್ಯಾನಿಸು CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#ವ್ಯಾಖ್ಯಾನಿಸು CHANNEL_FLAGS_ಖಂಡ_DEVICE	0x04
#ವ್ಯಾಖ್ಯಾನಿಸು CHANNEL_FLAGS_ಎಳೆಗಳು		0x08
#ವ್ಯಾಖ್ಯಾನಿಸು CHANNEL_FLAGS_NODISCARD		0x10
#ವ್ಯಾಖ್ಯಾನಿಸು CHANNEL_FLAGS_NOZEROOUT		0x20

#ವ್ಯಾಖ್ಯಾನಿಸು io_channel_discard_zeroes_ದತ್ತಾಂಶ(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

ರಚನೆ struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	ಅಕ್ಷರ		*name;
	ಪೂರ್ಣಾಂಕ		ಖಂಡ_ಗಾತ್ರ;
	errcode_t	(*read_error)(io_channel channel,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				      ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2,
				      ಶೂನ್ಯ *ದತ್ತಾಂಶ,
				      size_t ಗಾತ್ರ_2,
				      ಪೂರ್ಣಾಂಕ actual_ಬೈಟುಗಳು_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2,
				       ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ,
				       size_t ಗಾತ್ರ_2,
				       ಪೂರ್ಣಾಂಕ actual_ಬೈಟುಗಳು_written,
				       errcode_t error);
	ಪೂರ್ಣಾಂಕ		refcount;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	flags;
	ದೀರ್ಘ		reserved[14];
	ಶೂನ್ಯ		*ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	ಶೂನ್ಯ		*app_ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ		align;
};

ರಚನೆ struct_io_stats {
	ಪೂರ್ಣಾಂಕ			ಅಂಕೆ_fields;
	ಪೂರ್ಣಾಂಕ			reserved;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	ಬೈಟುಗಳು_read;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	ಬೈಟುಗಳು_written;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	cache_ದೊರಕಿದವು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	cache_ತಪ್ಪಿದವು;
};

ರಚನೆ struct_io_manager {
	errcode_t magic;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	errcode_t (*open)(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, io_channel *channel);
	errcode_t (*close)(io_channel channel);
	errcode_t (*ಗಣ_blksize)(io_channel channel, ಪೂರ್ಣಾಂಕ blksize);
	errcode_t (*read_blk)(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			      ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
	errcode_t (*write_blk)(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸ್ಥಾನಾಂತರ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ);
	errcode_t (*ಗಣ_option)(io_channel channel, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option,
				ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
					ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
	errcode_t (*write_blk64)(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
					ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ);
	errcode_t (*discard)(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2);
	errcode_t (*cache_readahead)(io_channel channel,
				     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2);
	errcode_t (*zeroout)(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2);
	errcode_t (*flock)(io_channel channel, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flock_flags);
	ದೀರ್ಘ	reserved[13];
};

#ವ್ಯಾಖ್ಯಾನಿಸು IO_FLAG_RW		0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು IO_FLAG_EXCLUSIVE	0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು IO_FLAG_ನೇರ_IO	0x0004
#ವ್ಯಾಖ್ಯಾನಿಸು IO_FLAG_FORCE_BOUNCE	0x0008
#ವ್ಯಾಖ್ಯಾನಿಸು IO_FLAG_ಎಳೆಗಳು		0x0010
#ವ್ಯಾಖ್ಯಾನಿಸು IO_FLAG_NOCACHE		0x0020

/* Prevent other programs from reading or writing to underlying storage */
#ವ್ಯಾಖ್ಯಾನಿಸು IO_CHANNEL_FLOCK_EXCLUSIVE	0x1
/* Prevent other programs from writing to underlying storage */
#ವ್ಯಾಖ್ಯಾನಿಸು IO_CHANNEL_FLOCK_SHARED		0x2
/* Return EBUSY if the lock cannot be taken immediately */
#ವ್ಯಾಖ್ಯಾನಿಸು IO_CHANNEL_FLOCK_TRYLOCK	0x4

/*
 * Convenience functions....
 */
#ವ್ಯಾಖ್ಯಾನಿಸು io_channel_close(c) 		((c)->manager->close((c)))
#ವ್ಯಾಖ್ಯಾನಿಸು io_channel_ಗಣ_blksize(c,s)	((c)->manager->ಗಣ_blksize((c),s))
#ವ್ಯಾಖ್ಯಾನಿಸು io_channel_read_blk(c,b,n,d)	((c)->manager->read_blk((c),b,n,d))
#ವ್ಯಾಖ್ಯಾನಿಸು io_channel_write_blk(c,b,n,d)	((c)->manager->write_blk((c),b,n,d))
#ವ್ಯಾಖ್ಯಾನಿಸು io_channel_flush(c) 		((c)->manager->flush((c)))
#ವ್ಯಾಖ್ಯಾನಿಸು io_channel_bumpcount(c)		((c)->refcount++)

/* io_manager.c */
ಬಾಹ್ಯ errcode_t io_channel_ಗಣ_options(io_channel channel,
					ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *options);
ಬಾಹ್ಯ errcode_t io_channel_write_byte(io_channel channel,
				       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸ್ಥಾನಾಂತರ,
				       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಬಾಹ್ಯ errcode_t io_channel_read_blk64(io_channel channel,
				       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಬಾಹ್ಯ errcode_t io_channel_write_blk64(io_channel channel,
					ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
					ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಬಾಹ್ಯ errcode_t io_channel_discard(io_channel channel,
				    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2);
ಬಾಹ್ಯ errcode_t io_channel_zeroout(io_channel channel,
				    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2);
ಬಾಹ್ಯ errcode_t io_channel_alloc_buf(io_channel channel,
				      ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *ptr);
ಬಾಹ್ಯ errcode_t io_channel_cache_readahead(io_channel io,
					    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
					    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2);
ಬಾಹ್ಯ errcode_t io_channel_flock(io_channel io, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flock_flags);
ಬಾಹ್ಯ errcode_t io_channel_funlock(io_channel io);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
/* windows_io.c */
ಬಾಹ್ಯ io_manager windows_io_manager;
#ವ್ಯಾಖ್ಯಾನಿಸು default_io_manager windows_io_manager
#ಇಲ್ಲದಿದ್ದರೆ
/* unix_io.c */
ಬಾಹ್ಯ io_manager unix_io_manager;
ಬಾಹ್ಯ io_manager unixfd_io_manager;
#ವ್ಯಾಖ್ಯಾನಿಸು default_io_manager unix_io_manager
#ಆದರೆ_ಅಂತ್ಯ

/* sparse_io.c */
ಬಾಹ್ಯ io_manager sparse_io_manager;
ಬಾಹ್ಯ io_manager sparsefd_io_manager;

/* undo_io.c */
ಬಾಹ್ಯ io_manager undo_io_manager;
ಬಾಹ್ಯ errcode_t ಗಣ_undo_io_backing_manager(io_manager manager);
ಬಾಹ್ಯ errcode_t ಗಣ_undo_io_backup_file(ಅಕ್ಷರ *file_name);

/* test_io.c */
ಬಾಹ್ಯ io_manager test_io_manager, test_io_backing_manager;
ಬಾಹ್ಯ ಶೂನ್ಯ (*test_io_cb_read_blk)
	(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err);
ಬಾಹ್ಯ ಶೂನ್ಯ (*test_io_cb_write_blk)
	(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err);
ಬಾಹ್ಯ ಶೂನ್ಯ (*test_io_cb_read_blk64)
	(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err);
ಬಾಹ್ಯ ಶೂನ್ಯ (*test_io_cb_write_blk64)
	(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err);
ಬಾಹ್ಯ ಶೂನ್ಯ (*test_io_cb_ಗಣ_blksize)
	(ಪೂರ್ಣಾಂಕ blksize, errcode_t err);

#ಆದರೆ_ಅಂತ್ಯ /* _EXT2FS_EXT2_IO_H */

