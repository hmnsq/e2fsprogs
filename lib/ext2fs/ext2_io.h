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

#నిర్వచితం_కాకపోతే _EXT2FS_EXT2_IO_H
#నిర్వచించు _EXT2FS_EXT2_IO_H

#అన్తర్భావ <ext2fs/ext2_types.h>

/*
 * ext2_loff_t is defined here since unix_io.c needs it.
 */
#ఒకవేళ defined(__GNUC__) || defined(HAS_LONG_LONG)
రకనిర్వచనం దీర్ఘ దీర్ఘ	ext2_loff_t;
#లేకపోతే
రకనిర్వచనం దీర్ఘ		ext2_loff_t;
#అయితే_ముగింపు

/* llseek.c */
ext2_loff_t ext2fs_llseek (పూర్ణాంకం, ext2_loff_t, పూర్ణాంకం);

రకనిర్వచనం నిర్మాణం struct_io_manager *io_manager;
రకనిర్వచనం నిర్మాణం struct_io_channel *io_channel;
రకనిర్వచనం నిర్మాణం struct_io_stats *io_stats;

#నిర్వచించు CHANNEL_FLAGS_WRITETHROUGH	0x01
#నిర్వచించు CHANNEL_FLAGS_DISCARD_ZEROES	0x02
#నిర్వచించు CHANNEL_FLAGS_ఖండం_DEVICE	0x04
#నిర్వచించు CHANNEL_FLAGS_దారాలు		0x08
#నిర్వచించు CHANNEL_FLAGS_NODISCARD		0x10
#నిర్వచించు CHANNEL_FLAGS_NOZEROOUT		0x20

#నిర్వచించు io_channel_discard_zeroes_దత్తాంశం(i) (i->flags & CHANNEL_FLAGS_DISCARD_ZEROES)

నిర్మాణం struct_io_channel {
	errcode_t	magic;
	io_manager	manager;
	అక్షరం		*name;
	పూర్ణాంకం		ఖండం_కొలత;
	errcode_t	(*read_error)(io_channel channel,
				      గుర్తులేని దీర్ఘ ఖండం,
				      పూర్ణాంకం లెక్క,
				      శూన్య *దత్తాంశం,
				      size_t కొలత,
				      పూర్ణాంకం actual_బైట్లు_read,
				      errcode_t	error);
	errcode_t	(*write_error)(io_channel channel,
				       గుర్తులేని దీర్ఘ ఖండం,
				       పూర్ణాంకం లెక్క,
				       స్థిరరాశి శూన్య *దత్తాంశం,
				       size_t కొలత,
				       పూర్ణాంకం actual_బైట్లు_written,
				       errcode_t error);
	పూర్ణాంకం		refcount;
	గుర్తులేని పూర్ణాంకం	flags;
	దీర్ఘ		reserved[14];
	శూన్య		*వ్యక్తిగత_దత్తాంశం;
	శూన్య		*app_దత్తాంశం;
	పూర్ణాంకం		align;
};

నిర్మాణం struct_io_stats {
	పూర్ణాంకం			అంకె_fields;
	పూర్ణాంకం			reserved;
	గుర్తులేని దీర్ఘ దీర్ఘ	బైట్లు_read;
	గుర్తులేని దీర్ఘ దీర్ఘ	బైట్లు_written;
	గుర్తులేని దీర్ఘ దీర్ఘ	cache_తగిలినవి;
	గుర్తులేని దీర్ఘ దీర్ఘ	cache_తప్పినవి;
};

నిర్మాణం struct_io_manager {
	errcode_t magic;
	స్థిరరాశి అక్షరం *name;
	errcode_t (*open)(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, io_channel *channel);
	errcode_t (*close)(io_channel channel);
	errcode_t (*సమితి_blksize)(io_channel channel, పూర్ణాంకం blksize);
	errcode_t (*read_blk)(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			      పూర్ణాంకం లెక్క, శూన్య *దత్తాంశం);
	errcode_t (*write_blk)(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *దత్తాంశం);
	errcode_t (*flush)(io_channel channel);
	errcode_t (*write_byte)(io_channel channel, గుర్తులేని దీర్ఘ స్థానభ్రంశం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *దత్తాంశం);
	errcode_t (*సమితి_option)(io_channel channel, స్థిరరాశి అక్షరం *option,
				స్థిరరాశి అక్షరం *arg);
	errcode_t (*get_stats)(io_channel channel, io_stats *io_stats);
	errcode_t (*read_blk64)(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
					పూర్ణాంకం లెక్క, శూన్య *దత్తాంశం);
	errcode_t (*write_blk64)(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
					పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *దత్తాంశం);
	errcode_t (*discard)(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			     గుర్తులేని దీర్ఘ దీర్ఘ లెక్క);
	errcode_t (*cache_readahead)(io_channel channel,
				     గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				     గుర్తులేని దీర్ఘ దీర్ఘ లెక్క);
	errcode_t (*zeroout)(io_channel channel, గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
			     గుర్తులేని దీర్ఘ దీర్ఘ లెక్క);
	errcode_t (*flock)(io_channel channel, గుర్తులేని పూర్ణాంకం flock_flags);
	దీర్ఘ	reserved[13];
};

#నిర్వచించు IO_FLAG_RW		0x0001
#నిర్వచించు IO_FLAG_EXCLUSIVE	0x0002
#నిర్వచించు IO_FLAG_ప్రత్యక్ష_IO	0x0004
#నిర్వచించు IO_FLAG_FORCE_BOUNCE	0x0008
#నిర్వచించు IO_FLAG_దారాలు		0x0010
#నిర్వచించు IO_FLAG_NOCACHE		0x0020

/* Prevent other programs from reading or writing to underlying storage */
#నిర్వచించు IO_CHANNEL_FLOCK_EXCLUSIVE	0x1
/* Prevent other programs from writing to underlying storage */
#నిర్వచించు IO_CHANNEL_FLOCK_SHARED		0x2
/* Return EBUSY if the lock cannot be taken immediately */
#నిర్వచించు IO_CHANNEL_FLOCK_TRYLOCK	0x4

/*
 * Convenience functions....
 */
#నిర్వచించు io_channel_close(c) 		((c)->manager->close((c)))
#నిర్వచించు io_channel_సమితి_blksize(c,s)	((c)->manager->సమితి_blksize((c),s))
#నిర్వచించు io_channel_read_blk(c,b,n,d)	((c)->manager->read_blk((c),b,n,d))
#నిర్వచించు io_channel_write_blk(c,b,n,d)	((c)->manager->write_blk((c),b,n,d))
#నిర్వచించు io_channel_flush(c) 		((c)->manager->flush((c)))
#నిర్వచించు io_channel_bumpcount(c)		((c)->refcount++)

/* io_manager.c */
బాహ్య errcode_t io_channel_సమితి_options(io_channel channel,
					స్థిరరాశి అక్షరం *options);
బాహ్య errcode_t io_channel_write_byte(io_channel channel,
				       గుర్తులేని దీర్ఘ స్థానభ్రంశం,
				       పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *దత్తాంశం);
బాహ్య errcode_t io_channel_read_blk64(io_channel channel,
				       గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				       పూర్ణాంకం లెక్క, శూన్య *దత్తాంశం);
బాహ్య errcode_t io_channel_write_blk64(io_channel channel,
					గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
					పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *దత్తాంశం);
బాహ్య errcode_t io_channel_discard(io_channel channel,
				    గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				    గుర్తులేని దీర్ఘ దీర్ఘ లెక్క);
బాహ్య errcode_t io_channel_zeroout(io_channel channel,
				    గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
				    గుర్తులేని దీర్ఘ దీర్ఘ లెక్క);
బాహ్య errcode_t io_channel_alloc_buf(io_channel channel,
				      పూర్ణాంకం లెక్క, శూన్య *ptr);
బాహ్య errcode_t io_channel_cache_readahead(io_channel io,
					    గుర్తులేని దీర్ఘ దీర్ఘ ఖండం,
					    గుర్తులేని దీర్ఘ దీర్ఘ లెక్క);
బాహ్య errcode_t io_channel_flock(io_channel io, గుర్తులేని పూర్ణాంకం flock_flags);
బాహ్య errcode_t io_channel_funlock(io_channel io);

#నిర్వచితమైతే _WIN32
/* windows_io.c */
బాహ్య io_manager windows_io_manager;
#నిర్వచించు default_io_manager windows_io_manager
#లేకపోతే
/* unix_io.c */
బాహ్య io_manager unix_io_manager;
బాహ్య io_manager unixfd_io_manager;
#నిర్వచించు default_io_manager unix_io_manager
#అయితే_ముగింపు

/* sparse_io.c */
బాహ్య io_manager sparse_io_manager;
బాహ్య io_manager sparsefd_io_manager;

/* undo_io.c */
బాహ్య io_manager undo_io_manager;
బాహ్య errcode_t సమితి_undo_io_backing_manager(io_manager manager);
బాహ్య errcode_t సమితి_undo_io_backup_file(అక్షరం *file_name);

/* test_io.c */
బాహ్య io_manager test_io_manager, test_io_backing_manager;
బాహ్య శూన్య (*test_io_cb_read_blk)
	(గుర్తులేని దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err);
బాహ్య శూన్య (*test_io_cb_write_blk)
	(గుర్తులేని దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err);
బాహ్య శూన్య (*test_io_cb_read_blk64)
	(గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err);
బాహ్య శూన్య (*test_io_cb_write_blk64)
	(గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err);
బాహ్య శూన్య (*test_io_cb_సమితి_blksize)
	(పూర్ణాంకం blksize, errcode_t err);

#అయితే_ముగింపు /* _EXT2FS_EXT2_IO_H */

