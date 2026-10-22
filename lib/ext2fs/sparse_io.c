#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ O_ದ್ವಿಮಾನ
#ವ್ಯಾಖ್ಯಾನಿಸು O_ದ್ವಿಮಾನ 0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !defined(ENABLE_LIBSPARSE)
ಸ್ಥಿರ errcode_t sparse_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name EXT2FS_ATTR((ಬಳಸದ)),
			     ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)),
			     io_channel *channel EXT2FS_ATTR((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
}
ಸ್ಥಿರ errcode_t sparse_close(io_channel channel EXT2FS_ATTR((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ EXT2_ET_UNIMPLEMENTED;
}
ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ <sparse/sparse.h>

ರಚನೆ sparse_map {
	ಪೂರ್ಣಾಂಕ			fd;
	ಅಕ್ಷರ			**ಖಂಡಗಳು;
	ಪೂರ್ಣಾಂಕ			ಖಂಡ_ಗಾತ್ರ;
	uint64_t		ಖಂಡಗಳು_ಎಣಿಕೆ;
	ಅಕ್ಷರ			*file;
	ರಚನೆ sparse_file	*sparse_file;
	io_channel		channel;
};

ರಚನೆ sparse_io_params {
	ಪೂರ್ಣಾಂಕ			fd;
	ಅಕ್ಷರ			*file;
	uint64_t		ಖಂಡಗಳು_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ಖಂಡ_ಗಾತ್ರ;
};

ಸ್ಥಿರ errcode_t sparse_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				  ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf);

ಸ್ಥಿರ ಶೂನ್ಯ free_sparse_ಖಂಡಗಳು(ರಚನೆ sparse_map *sm)
{
	uint64_t i;

	ಗಾಗಿ (i = 0; i < sm->ಖಂಡಗಳು_ಎಣಿಕೆ; ++i)
		ಬಿಡುಗಡೆ(sm->ಖಂಡಗಳು[i]);
	ಬಿಡುಗಡೆ(sm->ಖಂಡಗಳು);
	sm->ಖಂಡಗಳು = ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sparse_import_segment(ಶೂನ್ಯ *priv, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t len,
				 ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಂಡ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_ಖಂಡಗಳು)
{
	ರಚನೆ sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	ಆದರೆ (!nr_ಖಂಡಗಳು || len % sm->ಖಂಡ_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ sparse_write_blk(sm->channel, ಖಂಡ, nr_ಖಂಡಗಳು, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ errcode_t io_manager_import_sparse(ರಚನೆ sparse_io_params *params,
					  ರಚನೆ sparse_map *sm, io_channel io)
{
	ಪೂರ್ಣಾಂಕ fd;
	errcode_t retval;
	ರಚನೆ sparse_file *sparse_file;

	ಆದರೆ (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		ಆದರೆ (fd < 0) {
			retval = -1;
			ಹೋಗು err_open;
		}
	} ಇಲ್ಲದಿದ್ದರೆ
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	ಆದರೆ (!sparse_file) {
		retval = -1;
		ಹೋಗು err_sparse;
	}

	sm->ಖಂಡ_ಗಾತ್ರ = sparse_file_ಖಂಡ_ಗಾತ್ರ(sparse_file);
	sm->ಖಂಡಗಳು_ಎಣಿಕೆ = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->ಖಂಡ_ಗಾತ್ರ + 1;
	sm->ಖಂಡಗಳು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(sm->ಖಂಡಗಳು_ಎಣಿಕೆ, ಗಾತ್ರ(ಅಕ್ಷರ*));
	ಆದರೆ (!sm->ಖಂಡಗಳು) {
		retval = -1;
		ಹೋಗು err_alloc;
	}
	io->ಖಂಡ_ಗಾತ್ರ = sm->ಖಂಡ_ಗಾತ್ರ;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	ಆದರೆ (retval)
		free_sparse_ಖಂಡಗಳು(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t io_manager_configure(ರಚನೆ sparse_io_params *params,
				      ಪೂರ್ಣಾಂಕ flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_ಗಾತ್ರ;
	ರಚನೆ sparse_map *sm = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*sm));
	ಆದರೆ (!sm)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;

	sm->file = params->file;
	sm->channel = io;
	io->ಖಾಸಗಿ_ದತ್ತಾಂಶ = sm;
	retval = io_manager_import_sparse(params, sm, io);
	ಆದರೆ (retval) {
		ಆದರೆ (!params->ಖಂಡ_ಗಾತ್ರ || !params->ಖಂಡಗಳು_ಎಣಿಕೆ) {
			retval = EINVAL;
			ಹೋಗು err_params;
		}
		sm->ಖಂಡ_ಗಾತ್ರ = params->ಖಂಡ_ಗಾತ್ರ;
		sm->ಖಂಡಗಳು_ಎಣಿಕೆ = params->ಖಂಡಗಳು_ಎಣಿಕೆ;
		sm->ಖಂಡಗಳು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(params->ಖಂಡಗಳು_ಎಣಿಕೆ, ಗಾತ್ರ(ಶೂನ್ಯ*));
		ಆದರೆ (!sm->ಖಂಡಗಳು) {
			retval = EXT2_ET_NO_ಸ್ಮರಣೆ;
			ಹೋಗು err_alloc;
		}
	}
	io->ಖಂಡ_ಗಾತ್ರ = sm->ಖಂಡ_ಗಾತ್ರ;
	img_ಗಾತ್ರ = (uint64_t)sm->ಖಂಡ_ಗಾತ್ರ * sm->ಖಂಡಗಳು_ಎಣಿಕೆ;

	ಆದರೆ (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->ಖಂಡ_ಗಾತ್ರ, img_ಗಾತ್ರ);
		ಆದರೆ (!sm->sparse_file) {
			retval = EXT2_ET_NO_ಸ್ಮರಣೆ;
			ಹೋಗು err_alloc;
		}
		ಆದರೆ (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_ದ್ವಿಮಾನ,
				      0644);
			ಆದರೆ (sm->fd < 0) {
				retval = errno;
				ಹೋಗು err_open;
			}
		} ಇಲ್ಲದಿದ್ದರೆ
			sm->fd = params->fd;
	} ಇಲ್ಲದಿದ್ದರೆ {
		sm->fd = -1;
		sm->sparse_file = ಖಾಲಿ;
	}
	ಹಿಂತಿರುಗಿ 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_ಖಂಡಗಳು(sm);
err_params:
	ಬಿಡುಗಡೆ(sm);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t sparse_open_channel(ರಚನೆ sparse_io_params *sparse_params,
				     ಪೂರ್ಣಾಂಕ flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ರಚನೆ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->ಖಂಡ_ಗಾತ್ರ = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	ಆದರೆ (retval) {
		ಬಿಡುಗಡೆ(io);
		ಹಿಂತಿರುಗಿ retval;
	}

	*channel = io;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t read_sparse_argv(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, bool is_fd,
				  ರಚನೆ sparse_io_params *sparse_params)
{
	ಪೂರ್ಣಾಂಕ ret;
	sparse_params->fd = -1;
	sparse_params->ಖಂಡ_ಗಾತ್ರ = 0;
	sparse_params->ಖಂಡಗಳು_ಎಣಿಕೆ = 0;

	sparse_params->file = ಸ್ಮರಣೆ_ಕೊಡು(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name) + 1);
	ಆದರೆ (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name) + 1);
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	}

	ಆದರೆ (is_fd) {
		ret = ಅಕ್ಷರಸರಣಿ_ಓದು(name, "(%d):%llu:%u", &sparse_params->fd,
			     (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ *)&sparse_params->ಖಂಡಗಳು_ಎಣಿಕೆ,
			     &sparse_params->ಖಂಡ_ಗಾತ್ರ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ret = ಅಕ್ಷರಸರಣಿ_ಓದು(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ *)&sparse_params->ಖಂಡಗಳು_ಎಣಿಕೆ,
			     &sparse_params->ಖಂಡ_ಗಾತ್ರ);
	}

	ಆದರೆ (ret < 1) {
		ಬಿಡುಗಡೆ(sparse_params->file);
		ಹಿಂತಿರುಗಿ EINVAL;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t sparse_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, io_channel *channel)
{
	errcode_t retval;
	ರಚನೆ sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	(*channel)->manager = sparse_io_manager;

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t sparsefd_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, io_channel *channel)
{
	errcode_t retval;
	ರಚನೆ sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	(*channel)->manager = sparsefd_io_manager;

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t sparse_ವಿಲೀನ_ಖಂಡಗಳು(ರಚನೆ sparse_map *sm, uint64_t ಆರಂಭ,
					uint64_t ಅಂಕೆ)
{
	ಅಕ್ಷರ *buf;
	uint64_t i;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ = sm->ಖಂಡ_ಗಾತ್ರ;
	errcode_t retval = 0;

	buf = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಅಂಕೆ, ಖಂಡ_ಗಾತ್ರ);
	ಆದರೆ (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)ಅಂಕೆ * ಖಂಡ_ಗಾತ್ರ);
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	}

	ಗಾಗಿ (i = 0; i < ಅಂಕೆ; i++) {
		ಸ್ಮರಣೆ_ನಕಲು(buf + i * ಖಂಡ_ಗಾತ್ರ, sm->ಖಂಡಗಳು[ಆರಂಭ + i] , ಖಂಡ_ಗಾತ್ರ);
		ಬಿಡುಗಡೆ(sm->ಖಂಡಗಳು[ಆರಂಭ + i]);
		sm->ಖಂಡಗಳು[ಆರಂಭ + i] = ಖಾಲಿ;
	}

	/* free_sparse_blocks will release this buf. */
	sm->ಖಂಡಗಳು[ಆರಂಭ] = buf;

	retval = sparse_file_ಕೂಡಿಸು_ದತ್ತಾಂಶ(sm->sparse_file, sm->ಖಂಡಗಳು[ಆರಂಭ],
					ಖಂಡ_ಗಾತ್ರ * ಅಂಕೆ, ಆರಂಭ);

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	ರಚನೆ sparse_map *sm = channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;

	ಆದರೆ (sm->sparse_file) {
		int64_t chunk_ಆರಂಭ = (sm->ಖಂಡಗಳು[0] == ಖಾಲಿ) ? -1 : 0;
		ಗಾಗಿ (i = 0; i < sm->ಖಂಡಗಳು_ಎಣಿಕೆ; ++i) {
			ಆದರೆ (!sm->ಖಂಡಗಳು[i] && chunk_ಆರಂಭ != -1) {
				retval = sparse_ವಿಲೀನ_ಖಂಡಗಳು(sm, chunk_ಆರಂಭ, i - chunk_ಆರಂಭ);
				chunk_ಆರಂಭ = -1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sm->ಖಂಡಗಳು[i] && chunk_ಆರಂಭ == -1) {
				chunk_ಆರಂಭ = i;
			}
			ಆದರೆ (retval)
				ಹೋಗು ret;
		}
		ಆದರೆ (chunk_ಆರಂಭ != -1) {
			retval = sparse_ವಿಲೀನ_ಖಂಡಗಳು(sm, chunk_ಆರಂಭ,
							sm->ಖಂಡಗಳು_ಎಣಿಕೆ - chunk_ಆರಂಭ);
			ಆದರೆ (retval)
				ಹೋಗು ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ*/0);
	}

ret:
	ಆದರೆ (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_ಖಂಡಗಳು(sm);
	ಬಿಡುಗಡೆ(sm->file);
	ಬಿಡುಗಡೆ(sm);
	ಬಿಡುಗಡೆ(channel);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	ರಚನೆ sparse_map *sm = channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ fd = sm->fd;

	retval = sparse_close_channel(channel);
	ಆದರೆ (fd >= 0)
		close(fd);

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t sparse_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize)
{
	channel->ಖಂಡ_ಗಾತ್ರ = blksize;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ blk64_t ಖಂಡ_ಗಮ್ಯ_sparse_ಖಂಡ(blk64_t ಖಂಡ, blk64_t *ಸ್ಥಾನಾಂತರ,
			       io_channel channel, ರಚನೆ sparse_map *sm)
{
	ಪೂರ್ಣಾಂಕ ಅನುಪಾತ;
	blk64_t ret = ಖಂಡ;

	ಅನುಪಾತ = sm->ಖಂಡ_ಗಾತ್ರ / channel->ಖಂಡ_ಗಾತ್ರ;
	ret /= ಅನುಪಾತ;
	*ಸ್ಥಾನಾಂತರ = (ಖಂಡ % ಅನುಪಾತ) * channel->ಖಂಡ_ಗಾತ್ರ;

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ errcode_t check_ಖಂಡ_ಗಾತ್ರ(io_channel channel, ರಚನೆ sparse_map *sm)
{
	ಆದರೆ (sm->ಖಂಡ_ಗಾತ್ರ >= channel->ಖಂಡ_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ EXT2_ET_UNEXPECTED_ಖಂಡ_ಗಾತ್ರ;
}

ಸ್ಥಿರ errcode_t sparse_read_blk64(io_channel channel, blk64_t ಖಂಡ,
				   ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ಪೂರ್ಣಾಂಕ i;
	ಅಕ್ಷರ *ಹೊರಹೋಗು = buf;
	blk64_t ಸ್ಥಾನಾಂತರ = 0, cur_ಖಂಡ;
	ರಚನೆ sparse_map *sm = channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;

	ಆದರೆ (check_ಖಂಡ_ಗಾತ್ರ(channel, sm))
		ಹಿಂತಿರುಗಿ EXT2_ET_UNEXPECTED_ಖಂಡ_ಗಾತ್ರ;

	ಆದರೆ (ಎಣಿಕೆ_2 < 0) { //partial read
		ಎಣಿಕೆ_2 = -ಎಣಿಕೆ_2;
		cur_ಖಂಡ = ಖಂಡ_ಗಮ್ಯ_sparse_ಖಂಡ(ಖಂಡ, &ಸ್ಥಾನಾಂತರ, channel, sm);
		ಆದರೆ (sm->ಖಂಡಗಳು[cur_ಖಂಡ])
			ಸ್ಮರಣೆ_ನಕಲು(ಹೊರಹೋಗು, (sm->ಖಂಡಗಳು[cur_ಖಂಡ]) + ಸ್ಥಾನಾಂತರ, ಎಣಿಕೆ_2);
		ಇಲ್ಲದಿದ್ದರೆ
			memset(ಹೊರಹೋಗು, 0, ಎಣಿಕೆ_2);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾಗಿ (i = 0; i < ಎಣಿಕೆ_2; ++i) {
			cur_ಖಂಡ = ಖಂಡ_ಗಮ್ಯ_sparse_ಖಂಡ(ಖಂಡ + i, &ಸ್ಥಾನಾಂತರ,
						    channel, sm);
			ಆದರೆ (sm->ಖಂಡಗಳು[cur_ಖಂಡ])
				ಸ್ಮರಣೆ_ನಕಲು(ಹೊರಹೋಗು + (i * channel->ಖಂಡ_ಗಾತ್ರ),
				       sm->ಖಂಡಗಳು[cur_ಖಂಡ] + ಸ್ಥಾನಾಂತರ,
				       channel->ಖಂಡ_ಗಾತ್ರ);
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sm->ಖಂಡಗಳು)
				memset(ಹೊರಹೋಗು + (i * channel->ಖಂಡ_ಗಾತ್ರ), 0,
				       channel->ಖಂಡ_ಗಾತ್ರ);
		}
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t sparse_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				 ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ sparse_read_blk64(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf);
}

ಸ್ಥಿರ errcode_t sparse_write_blk64(io_channel channel, blk64_t ಖಂಡ,
				    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ಪೂರ್ಣಾಂಕ i;
	blk64_t ಸ್ಥಾನಾಂತರ = 0, cur_ಖಂಡ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಒಳ = buf;
	ರಚನೆ sparse_map *sm = channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;

	ಆದರೆ (check_ಖಂಡ_ಗಾತ್ರ(channel, sm))
		ಹಿಂತಿರುಗಿ EXT2_ET_UNEXPECTED_ಖಂಡ_ಗಾತ್ರ;

	ಆದರೆ (ಎಣಿಕೆ_2 < 0) { //partial write
		ಎಣಿಕೆ_2 = -ಎಣಿಕೆ_2;
		cur_ಖಂಡ = ಖಂಡ_ಗಮ್ಯ_sparse_ಖಂಡ(ಖಂಡ, &ಸ್ಥಾನಾಂತರ, channel,
						  sm);
		ಆದರೆ (!sm->ಖಂಡಗಳು[cur_ಖಂಡ]) {
			sm->ಖಂಡಗಳು[cur_ಖಂಡ] = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, sm->ಖಂಡ_ಗಾತ್ರ);
			ಆದರೆ (!sm->ಖಂಡಗಳು[cur_ಖಂಡ])
				ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
		}
		ಸ್ಮರಣೆ_ನಕಲು(sm->ಖಂಡಗಳು[cur_ಖಂಡ] + ಸ್ಥಾನಾಂತರ, ಒಳ, ಎಣಿಕೆ_2);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾಗಿ (i = 0; i < ಎಣಿಕೆ_2; ++i) {
			ಆದರೆ (ಖಂಡ + i >= sm->ಖಂಡಗಳು_ಎಣಿಕೆ)
				ಹಿಂತಿರುಗಿ 0;
			cur_ಖಂಡ = ಖಂಡ_ಗಮ್ಯ_sparse_ಖಂಡ(ಖಂಡ + i, &ಸ್ಥಾನಾಂತರ,
						    channel, sm);
			ಆದರೆ (!sm->ಖಂಡಗಳು[cur_ಖಂಡ]) {
				sm->ಖಂಡಗಳು[cur_ಖಂಡ] =
					ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, sm->ಖಂಡ_ಗಾತ್ರ);
				ಆದರೆ (!sm->ಖಂಡಗಳು[cur_ಖಂಡ])
					ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
			}
			ಸ್ಮರಣೆ_ನಕಲು(sm->ಖಂಡಗಳು[cur_ಖಂಡ] + ಸ್ಥಾನಾಂತರ,
			       ಒಳ + (i * channel->ಖಂಡ_ಗಾತ್ರ),
			       channel->ಖಂಡ_ಗಾತ್ರ);
		}
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t sparse_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				  ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ sparse_write_blk64(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf);
}

ಸ್ಥಿರ errcode_t sparse_discard(io_channel channel __attribute__((ಬಳಸದ)),
				blk64_t blk, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	blk64_t cur_ಖಂಡ, ಸ್ಥಾನಾಂತರ;
	ರಚನೆ sparse_map *sm = channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;

	ಆದರೆ (check_ಖಂಡ_ಗಾತ್ರ(channel, sm))
		ಹಿಂತಿರುಗಿ EXT2_ET_UNEXPECTED_ಖಂಡ_ಗಾತ್ರ;

	ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ i = 0; i < ಎಣಿಕೆ_2; ++i) {
		ಆದರೆ (blk + i >= sm->ಖಂಡಗಳು_ಎಣಿಕೆ)
			ಹಿಂತಿರುಗಿ 0;
		cur_ಖಂಡ = ಖಂಡ_ಗಮ್ಯ_sparse_ಖಂಡ(blk + i, &ಸ್ಥಾನಾಂತರ, channel,
						  sm);
		ಆದರೆ (!sm->ಖಂಡಗಳು[cur_ಖಂಡ])
			ಮುಂದುವರಿ;
		ಬಿಡುಗಡೆ(sm->ಖಂಡಗಳು[cur_ಖಂಡ]);
		sm->ಖಂಡಗಳು[cur_ಖಂಡ] = ಖಾಲಿ;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2)
{
	ಹಿಂತಿರುಗಿ sparse_discard(channel, blk, ಎಣಿಕೆ_2);
}

ಸ್ಥಿರ errcode_t sparse_flush(io_channel channel __attribute__((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t sparse_ಗಣ_option(io_channel channel __attribute__((ಬಳಸದ)),
                                   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option __attribute__((ಬಳಸದ)),
                                   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg __attribute__((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((ಬಳಸದ)),
			blk64_t blk __attribute__((ಬಳಸದ)),
			ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಎಣಿಕೆ_2 __attribute__((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.ಗಣ_blksize		= sparse_ಗಣ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ಖಾಲಿ,
	.ಗಣ_option		= sparse_ಗಣ_option,
	.get_stats		= ಖಾಲಿ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.ಗಣ_blksize		= sparse_ಗಣ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ಖಾಲಿ,
	.ಗಣ_option		= sparse_ಗಣ_option,
	.get_stats		= ಖಾಲಿ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#ಆದರೆ_ಅಂತ್ಯ

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
