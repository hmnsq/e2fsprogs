/*
 * io_manager.c --- the I/O manager abstraction
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

errcode_t io_channel_ಗಣ_options(io_channel channel, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opts)
{
	errcode_t retval = 0;
	ಅಕ್ಷರ *ಮುಂದಿನ, *ptr, *options, *arg;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಆದರೆ (!opts)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!channel->manager->ಗಣ_option)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	options = ಸ್ಮರಣೆ_ಕೊಡು(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(opts)+1);
	ಆದರೆ (!options)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	strcpy(options, opts);
	ptr = options;

	ಆಗ (ptr && *ptr) {
		ಮುಂದಿನ = strchr(ptr, '&');
		ಆದರೆ (ಮುಂದಿನ)
			*ಮುಂದಿನ++ = 0;

		arg = strchr(ptr, '=');
		ಆದರೆ (arg)
			*arg++ = 0;

		retval = (channel->manager->ಗಣ_option)(channel, ptr, arg);
		ಆದರೆ (retval)
			ನಿಲ್ಲಿಸು;
		ptr = ಮುಂದಿನ;
	}
	ಬಿಡುಗಡೆ(options);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t io_channel_write_byte(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸ್ಥಾನಾಂತರ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಆದರೆ (channel->manager->write_byte)
		ಹಿಂತಿರುಗಿ channel->manager->write_byte(channel, ಸ್ಥಾನಾಂತರ,
						    ಎಣಿಕೆ_2, ದತ್ತಾಂಶ);

	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_read_blk64(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				 ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಆದರೆ (channel->manager->read_blk64)
		ಹಿಂತಿರುಗಿ (channel->manager->read_blk64)(channel, ಖಂಡ,
						      ಎಣಿಕೆ_2, ದತ್ತಾಂಶ);

	ಆದರೆ ((ಖಂಡ >> 32) != 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ಹಿಂತಿರುಗಿ (channel->manager->read_blk)(channel, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) ಖಂಡ,
					     ಎಣಿಕೆ_2, ದತ್ತಾಂಶ);
}

errcode_t io_channel_write_blk64(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				 ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಆದರೆ (channel->manager->write_blk64)
		ಹಿಂತಿರುಗಿ (channel->manager->write_blk64)(channel, ಖಂಡ,
						       ಎಣಿಕೆ_2, ದತ್ತಾಂಶ);

	ಆದರೆ ((ಖಂಡ >> 32) != 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_IO_CHANNEL_NO_SUPPORT_64;

	ಹಿಂತಿರುಗಿ (channel->manager->write_blk)(channel, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) ಖಂಡ,
					     ಎಣಿಕೆ_2, ದತ್ತಾಂಶ);
}

errcode_t io_channel_discard(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಆದರೆ (channel->manager->discard)
		ಹಿಂತಿರುಗಿ (channel->manager->discard)(channel, ಖಂಡ, ಎಣಿಕೆ_2);

	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_zeroout(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
			     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);

	ಆದರೆ (channel->manager->zeroout)
		ಹಿಂತಿರುಗಿ (channel->manager->zeroout)(channel, ಖಂಡ, ಎಣಿಕೆ_2);

	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
}

errcode_t io_channel_alloc_buf(io_channel io, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *ptr)
{
	size_t	ಗಾತ್ರ_2;

	ಆದರೆ (ಎಣಿಕೆ_2 == 0)
		ಗಾತ್ರ_2 = io->ಖಂಡ_ಗಾತ್ರ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಎಣಿಕೆ_2 > 0)
		ಗಾತ್ರ_2 = io->ಖಂಡ_ಗಾತ್ರ * ಎಣಿಕೆ_2;
	ಇಲ್ಲದಿದ್ದರೆ
		ಗಾತ್ರ_2 = -ಎಣಿಕೆ_2;

	ಆದರೆ (io->align > 0) {
		ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ) io->align > ಗಾತ್ರ_2)
			ಗಾತ್ರ_2 = io->align;
		ಹಿಂತಿರುಗಿ ext2fs_get_memalign(ಗಾತ್ರ_2, io->align, ptr);
	} ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ ext2fs_get_mem(ಗಾತ್ರ_2, ptr);
}

errcode_t io_channel_cache_readahead(io_channel io, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ,
				     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ಆದರೆ (!io->manager->cache_readahead)
		ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;

	ಹಿಂತಿರುಗಿ io->manager->cache_readahead(io, ಖಂಡ, ಎಣಿಕೆ_2);
}

errcode_t io_channel_flock(io_channel io, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flock_flags)
{
	ಆದರೆ (!io->manager->flock)
		ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;

	ಹಿಂತಿರುಗಿ io->manager->flock(io, flock_flags);
}

errcode_t io_channel_funlock(io_channel io)
{
	ಆದರೆ (!io->manager->flock)
		ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;

	ಹಿಂತಿರುಗಿ io->manager->flock(io, 0);
}
