#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ O_ਦੁਆਧਾਰੀ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ O_ਦੁਆਧਾਰੀ 0
#ਜੇ_ਅੰਤ

#ਜੇ !defined(ENABLE_LIBSPARSE)
ਸਥਿਰ errcode_t sparse_open(ਅਚਲ ਅੱਖਰ *name EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			     ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			     io_channel *channel EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
}
ਸਥਿਰ errcode_t sparse_close(io_channel channel EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ EXT2_ET_UNIMPLEMENTED;
}
ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <sparse/sparse.h>

ਬਣਤਰ sparse_map {
	ਪੂਰਨ			fd;
	ਅੱਖਰ			**ਖੰਡਾਂ;
	ਪੂਰਨ			ਖੰਡ_ਮਾਪ;
	uint64_t		ਖੰਡਾਂ_ਗਿਣਤੀ;
	ਅੱਖਰ			*file;
	ਬਣਤਰ sparse_file	*sparse_file;
	io_channel		channel;
};

ਬਣਤਰ sparse_io_params {
	ਪੂਰਨ			fd;
	ਅੱਖਰ			*file;
	uint64_t		ਖੰਡਾਂ_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ਖੰਡ_ਮਾਪ;
};

ਸਥਿਰ errcode_t sparse_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				  ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf);

ਸਥਿਰ ਖਾਲੀ free_sparse_ਖੰਡਾਂ(ਬਣਤਰ sparse_map *sm)
{
	uint64_t i;

	ਲਈ (i = 0; i < sm->ਖੰਡਾਂ_ਗਿਣਤੀ; ++i)
		ਮੁਕਤ(sm->ਖੰਡਾਂ[i]);
	ਮੁਕਤ(sm->ਖੰਡਾਂ);
	sm->ਖੰਡਾਂ = ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ sparse_import_segment(ਖਾਲੀ *priv, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, size_t len,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_ਖੰਡਾਂ)
{
	ਬਣਤਰ sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	ਜੇ (!nr_ਖੰਡਾਂ || len % sm->ਖੰਡ_ਮਾਪ)
		ਵਾਪਸ 0;

	ਵਾਪਸ sparse_write_blk(sm->channel, ਖੰਡ, nr_ਖੰਡਾਂ, ਅੰਕੜੇ);
}

ਸਥਿਰ errcode_t io_manager_import_sparse(ਬਣਤਰ sparse_io_params *params,
					  ਬਣਤਰ sparse_map *sm, io_channel io)
{
	ਪੂਰਨ fd;
	errcode_t retval;
	ਬਣਤਰ sparse_file *sparse_file;

	ਜੇ (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		ਜੇ (fd < 0) {
			retval = -1;
			ਜਾਓ err_open;
		}
	} ਨਹੀਂਤਾਂ
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	ਜੇ (!sparse_file) {
		retval = -1;
		ਜਾਓ err_sparse;
	}

	sm->ਖੰਡ_ਮਾਪ = sparse_file_ਖੰਡ_ਮਾਪ(sparse_file);
	sm->ਖੰਡਾਂ_ਗਿਣਤੀ = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->ਖੰਡ_ਮਾਪ + 1;
	sm->ਖੰਡਾਂ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(sm->ਖੰਡਾਂ_ਗਿਣਤੀ, ਆਕਾਰ(ਅੱਖਰ*));
	ਜੇ (!sm->ਖੰਡਾਂ) {
		retval = -1;
		ਜਾਓ err_alloc;
	}
	io->ਖੰਡ_ਮਾਪ = sm->ਖੰਡ_ਮਾਪ;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	ਜੇ (retval)
		free_sparse_ਖੰਡਾਂ(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t io_manager_configure(ਬਣਤਰ sparse_io_params *params,
				      ਪੂਰਨ flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_ਮਾਪ;
	ਬਣਤਰ sparse_map *sm = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*sm));
	ਜੇ (!sm)
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;

	sm->file = params->file;
	sm->channel = io;
	io->ਨਿੱਜੀ_ਅੰਕੜੇ = sm;
	retval = io_manager_import_sparse(params, sm, io);
	ਜੇ (retval) {
		ਜੇ (!params->ਖੰਡ_ਮਾਪ || !params->ਖੰਡਾਂ_ਗਿਣਤੀ) {
			retval = EINVAL;
			ਜਾਓ err_params;
		}
		sm->ਖੰਡ_ਮਾਪ = params->ਖੰਡ_ਮਾਪ;
		sm->ਖੰਡਾਂ_ਗਿਣਤੀ = params->ਖੰਡਾਂ_ਗਿਣਤੀ;
		sm->ਖੰਡਾਂ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(params->ਖੰਡਾਂ_ਗਿਣਤੀ, ਆਕਾਰ(ਖਾਲੀ*));
		ਜੇ (!sm->ਖੰਡਾਂ) {
			retval = EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
			ਜਾਓ err_alloc;
		}
	}
	io->ਖੰਡ_ਮਾਪ = sm->ਖੰਡ_ਮਾਪ;
	img_ਮਾਪ = (uint64_t)sm->ਖੰਡ_ਮਾਪ * sm->ਖੰਡਾਂ_ਗਿਣਤੀ;

	ਜੇ (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->ਖੰਡ_ਮਾਪ, img_ਮਾਪ);
		ਜੇ (!sm->sparse_file) {
			retval = EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
			ਜਾਓ err_alloc;
		}
		ਜੇ (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_ਦੁਆਧਾਰੀ,
				      0644);
			ਜੇ (sm->fd < 0) {
				retval = errno;
				ਜਾਓ err_open;
			}
		} ਨਹੀਂਤਾਂ
			sm->fd = params->fd;
	} ਨਹੀਂਤਾਂ {
		sm->fd = -1;
		sm->sparse_file = ਨਿਰਾ;
	}
	ਵਾਪਸ 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_ਖੰਡਾਂ(sm);
err_params:
	ਮੁਕਤ(sm);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t sparse_open_channel(ਬਣਤਰ sparse_io_params *sparse_params,
				     ਪੂਰਨ flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਬਣਤਰ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->ਖੰਡ_ਮਾਪ = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	ਜੇ (retval) {
		ਮੁਕਤ(io);
		ਵਾਪਸ retval;
	}

	*channel = io;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t read_sparse_argv(ਅਚਲ ਅੱਖਰ *name, bool is_fd,
				  ਬਣਤਰ sparse_io_params *sparse_params)
{
	ਪੂਰਨ ret;
	sparse_params->fd = -1;
	sparse_params->ਖੰਡ_ਮਾਪ = 0;
	sparse_params->ਖੰਡਾਂ_ਗਿਣਤੀ = 0;

	sparse_params->file = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਲੜੀ_ਲੰਬਾਈ(name) + 1);
	ਜੇ (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", ਲੜੀ_ਲੰਬਾਈ(name) + 1);
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	}

	ਜੇ (is_fd) {
		ret = ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(name, "(%d):%llu:%u", &sparse_params->fd,
			     (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ *)&sparse_params->ਖੰਡਾਂ_ਗਿਣਤੀ,
			     &sparse_params->ਖੰਡ_ਮਾਪ);
	} ਨਹੀਂਤਾਂ {
		ret = ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ *)&sparse_params->ਖੰਡਾਂ_ਗਿਣਤੀ,
			     &sparse_params->ਖੰਡ_ਮਾਪ);
	}

	ਜੇ (ret < 1) {
		ਮੁਕਤ(sparse_params->file);
		ਵਾਪਸ EINVAL;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t sparse_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, io_channel *channel)
{
	errcode_t retval;
	ਬਣਤਰ sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	ਜੇ (retval)
		ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	(*channel)->manager = sparse_io_manager;

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t sparsefd_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, io_channel *channel)
{
	errcode_t retval;
	ਬਣਤਰ sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	ਜੇ (retval)
		ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	(*channel)->manager = sparsefd_io_manager;

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t sparse_ਮਿਲਾਓ_ਖੰਡਾਂ(ਬਣਤਰ sparse_map *sm, uint64_t ਸ਼ੁਰੂ,
					uint64_t ਅੰਕ)
{
	ਅੱਖਰ *buf;
	uint64_t i;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡ_ਮਾਪ = sm->ਖੰਡ_ਮਾਪ;
	errcode_t retval = 0;

	buf = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਅੰਕ, ਖੰਡ_ਮਾਪ);
	ਜੇ (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)ਅੰਕ * ਖੰਡ_ਮਾਪ);
		ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	}

	ਲਈ (i = 0; i < ਅੰਕ; i++) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf + i * ਖੰਡ_ਮਾਪ, sm->ਖੰਡਾਂ[ਸ਼ੁਰੂ + i] , ਖੰਡ_ਮਾਪ);
		ਮੁਕਤ(sm->ਖੰਡਾਂ[ਸ਼ੁਰੂ + i]);
		sm->ਖੰਡਾਂ[ਸ਼ੁਰੂ + i] = ਨਿਰਾ;
	}

	/* free_sparse_blocks will release this buf. */
	sm->ਖੰਡਾਂ[ਸ਼ੁਰੂ] = buf;

	retval = sparse_file_ਜੋੜੋ_ਅੰਕੜੇ(sm->sparse_file, sm->ਖੰਡਾਂ[ਸ਼ੁਰੂ],
					ਖੰਡ_ਮਾਪ * ਅੰਕ, ਸ਼ੁਰੂ);

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	ਬਣਤਰ sparse_map *sm = channel->ਨਿੱਜੀ_ਅੰਕੜੇ;

	ਜੇ (sm->sparse_file) {
		int64_t chunk_ਸ਼ੁਰੂ = (sm->ਖੰਡਾਂ[0] == ਨਿਰਾ) ? -1 : 0;
		ਲਈ (i = 0; i < sm->ਖੰਡਾਂ_ਗਿਣਤੀ; ++i) {
			ਜੇ (!sm->ਖੰਡਾਂ[i] && chunk_ਸ਼ੁਰੂ != -1) {
				retval = sparse_ਮਿਲਾਓ_ਖੰਡਾਂ(sm, chunk_ਸ਼ੁਰੂ, i - chunk_ਸ਼ੁਰੂ);
				chunk_ਸ਼ੁਰੂ = -1;
			} ਨਹੀਂਤਾਂ ਜੇ (sm->ਖੰਡਾਂ[i] && chunk_ਸ਼ੁਰੂ == -1) {
				chunk_ਸ਼ੁਰੂ = i;
			}
			ਜੇ (retval)
				ਜਾਓ ret;
		}
		ਜੇ (chunk_ਸ਼ੁਰੂ != -1) {
			retval = sparse_ਮਿਲਾਓ_ਖੰਡਾਂ(sm, chunk_ਸ਼ੁਰੂ,
							sm->ਖੰਡਾਂ_ਗਿਣਤੀ - chunk_ਸ਼ੁਰੂ);
			ਜੇ (retval)
				ਜਾਓ ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*ਚੱਕਰੀ_ਜਾਂਚ*/0);
	}

ret:
	ਜੇ (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_ਖੰਡਾਂ(sm);
	ਮੁਕਤ(sm->file);
	ਮੁਕਤ(sm);
	ਮੁਕਤ(channel);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	ਬਣਤਰ sparse_map *sm = channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	ਪੂਰਨ fd = sm->fd;

	retval = sparse_close_channel(channel);
	ਜੇ (fd >= 0)
		close(fd);

	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t sparse_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize)
{
	channel->ਖੰਡ_ਮਾਪ = blksize;
	ਵਾਪਸ 0;
}

ਸਥਿਰ blk64_t ਖੰਡ_ਮੰਜ਼ਿਲ_sparse_ਖੰਡ(blk64_t ਖੰਡ, blk64_t *ਵਿੱਥ,
			       io_channel channel, ਬਣਤਰ sparse_map *sm)
{
	ਪੂਰਨ ਅਨੁਪਾਤ;
	blk64_t ret = ਖੰਡ;

	ਅਨੁਪਾਤ = sm->ਖੰਡ_ਮਾਪ / channel->ਖੰਡ_ਮਾਪ;
	ret /= ਅਨੁਪਾਤ;
	*ਵਿੱਥ = (ਖੰਡ % ਅਨੁਪਾਤ) * channel->ਖੰਡ_ਮਾਪ;

	ਵਾਪਸ ret;
}

ਸਥਿਰ errcode_t check_ਖੰਡ_ਮਾਪ(io_channel channel, ਬਣਤਰ sparse_map *sm)
{
	ਜੇ (sm->ਖੰਡ_ਮਾਪ >= channel->ਖੰਡ_ਮਾਪ)
		ਵਾਪਸ 0;
	ਵਾਪਸ EXT2_ET_UNEXPECTED_ਖੰਡ_ਮਾਪ;
}

ਸਥਿਰ errcode_t sparse_read_blk64(io_channel channel, blk64_t ਖੰਡ,
				   ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਪੂਰਨ i;
	ਅੱਖਰ *ਬਾਹਰਲਾ = buf;
	blk64_t ਵਿੱਥ = 0, cur_ਖੰਡ;
	ਬਣਤਰ sparse_map *sm = channel->ਨਿੱਜੀ_ਅੰਕੜੇ;

	ਜੇ (check_ਖੰਡ_ਮਾਪ(channel, sm))
		ਵਾਪਸ EXT2_ET_UNEXPECTED_ਖੰਡ_ਮਾਪ;

	ਜੇ (ਗਿਣਤੀ_2 < 0) { //partial read
		ਗਿਣਤੀ_2 = -ਗਿਣਤੀ_2;
		cur_ਖੰਡ = ਖੰਡ_ਮੰਜ਼ਿਲ_sparse_ਖੰਡ(ਖੰਡ, &ਵਿੱਥ, channel, sm);
		ਜੇ (sm->ਖੰਡਾਂ[cur_ਖੰਡ])
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਬਾਹਰਲਾ, (sm->ਖੰਡਾਂ[cur_ਖੰਡ]) + ਵਿੱਥ, ਗਿਣਤੀ_2);
		ਨਹੀਂਤਾਂ
			memset(ਬਾਹਰਲਾ, 0, ਗਿਣਤੀ_2);
	} ਨਹੀਂਤਾਂ {
		ਲਈ (i = 0; i < ਗਿਣਤੀ_2; ++i) {
			cur_ਖੰਡ = ਖੰਡ_ਮੰਜ਼ਿਲ_sparse_ਖੰਡ(ਖੰਡ + i, &ਵਿੱਥ,
						    channel, sm);
			ਜੇ (sm->ਖੰਡਾਂ[cur_ਖੰਡ])
				ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਬਾਹਰਲਾ + (i * channel->ਖੰਡ_ਮਾਪ),
				       sm->ਖੰਡਾਂ[cur_ਖੰਡ] + ਵਿੱਥ,
				       channel->ਖੰਡ_ਮਾਪ);
			ਨਹੀਂਤਾਂ ਜੇ (sm->ਖੰਡਾਂ)
				memset(ਬਾਹਰਲਾ + (i * channel->ਖੰਡ_ਮਾਪ), 0,
				       channel->ਖੰਡ_ਮਾਪ);
		}
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t sparse_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				 ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	ਵਾਪਸ sparse_read_blk64(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf);
}

ਸਥਿਰ errcode_t sparse_write_blk64(io_channel channel, blk64_t ਖੰਡ,
				    ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਪੂਰਨ i;
	blk64_t ਵਿੱਥ = 0, cur_ਖੰਡ;
	ਅਚਲ ਅੱਖਰ *ਵਿੱਚ = buf;
	ਬਣਤਰ sparse_map *sm = channel->ਨਿੱਜੀ_ਅੰਕੜੇ;

	ਜੇ (check_ਖੰਡ_ਮਾਪ(channel, sm))
		ਵਾਪਸ EXT2_ET_UNEXPECTED_ਖੰਡ_ਮਾਪ;

	ਜੇ (ਗਿਣਤੀ_2 < 0) { //partial write
		ਗਿਣਤੀ_2 = -ਗਿਣਤੀ_2;
		cur_ਖੰਡ = ਖੰਡ_ਮੰਜ਼ਿਲ_sparse_ਖੰਡ(ਖੰਡ, &ਵਿੱਥ, channel,
						  sm);
		ਜੇ (!sm->ਖੰਡਾਂ[cur_ਖੰਡ]) {
			sm->ਖੰਡਾਂ[cur_ਖੰਡ] = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, sm->ਖੰਡ_ਮਾਪ);
			ਜੇ (!sm->ਖੰਡਾਂ[cur_ਖੰਡ])
				ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
		}
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(sm->ਖੰਡਾਂ[cur_ਖੰਡ] + ਵਿੱਥ, ਵਿੱਚ, ਗਿਣਤੀ_2);
	} ਨਹੀਂਤਾਂ {
		ਲਈ (i = 0; i < ਗਿਣਤੀ_2; ++i) {
			ਜੇ (ਖੰਡ + i >= sm->ਖੰਡਾਂ_ਗਿਣਤੀ)
				ਵਾਪਸ 0;
			cur_ਖੰਡ = ਖੰਡ_ਮੰਜ਼ਿਲ_sparse_ਖੰਡ(ਖੰਡ + i, &ਵਿੱਥ,
						    channel, sm);
			ਜੇ (!sm->ਖੰਡਾਂ[cur_ਖੰਡ]) {
				sm->ਖੰਡਾਂ[cur_ਖੰਡ] =
					ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, sm->ਖੰਡ_ਮਾਪ);
				ਜੇ (!sm->ਖੰਡਾਂ[cur_ਖੰਡ])
					ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
			}
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(sm->ਖੰਡਾਂ[cur_ਖੰਡ] + ਵਿੱਥ,
			       ਵਿੱਚ + (i * channel->ਖੰਡ_ਮਾਪ),
			       channel->ਖੰਡ_ਮਾਪ);
		}
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t sparse_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				  ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ਵਾਪਸ sparse_write_blk64(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf);
}

ਸਥਿਰ errcode_t sparse_discard(io_channel channel __attribute__((ਅਣਵਰਤਿਆ)),
				blk64_t blk, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	blk64_t cur_ਖੰਡ, ਵਿੱਥ;
	ਬਣਤਰ sparse_map *sm = channel->ਨਿੱਜੀ_ਅੰਕੜੇ;

	ਜੇ (check_ਖੰਡ_ਮਾਪ(channel, sm))
		ਵਾਪਸ EXT2_ET_UNEXPECTED_ਖੰਡ_ਮਾਪ;

	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ i = 0; i < ਗਿਣਤੀ_2; ++i) {
		ਜੇ (blk + i >= sm->ਖੰਡਾਂ_ਗਿਣਤੀ)
			ਵਾਪਸ 0;
		cur_ਖੰਡ = ਖੰਡ_ਮੰਜ਼ਿਲ_sparse_ਖੰਡ(blk + i, &ਵਿੱਥ, channel,
						  sm);
		ਜੇ (!sm->ਖੰਡਾਂ[cur_ਖੰਡ])
			ਜਾਰੀ;
		ਮੁਕਤ(sm->ਖੰਡਾਂ[cur_ਖੰਡ]);
		sm->ਖੰਡਾਂ[cur_ਖੰਡ] = ਨਿਰਾ;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2)
{
	ਵਾਪਸ sparse_discard(channel, blk, ਗਿਣਤੀ_2);
}

ਸਥਿਰ errcode_t sparse_flush(io_channel channel __attribute__((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t sparse_ਸੈੱਟ_option(io_channel channel __attribute__((ਅਣਵਰਤਿਆ)),
                                   ਅਚਲ ਅੱਖਰ *option __attribute__((ਅਣਵਰਤਿਆ)),
                                   ਅਚਲ ਅੱਖਰ *arg __attribute__((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((ਅਣਵਰਤਿਆ)),
			blk64_t blk __attribute__((ਅਣਵਰਤਿਆ)),
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਗਿਣਤੀ_2 __attribute__((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.ਸੈੱਟ_blksize		= sparse_ਸੈੱਟ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ਨਿਰਾ,
	.ਸੈੱਟ_option		= sparse_ਸੈੱਟ_option,
	.get_stats		= ਨਿਰਾ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.ਸੈੱਟ_blksize		= sparse_ਸੈੱਟ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ਨਿਰਾ,
	.ਸੈੱਟ_option		= sparse_ਸੈੱਟ_option,
	.get_stats		= ਨਿਰਾ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#ਜੇ_ਅੰਤ

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
