#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

#ifndef O_ദ്വിമാന
#define O_ദ്വിമാന 0
#endif

#എങ്കിൽ !defined(ENABLE_LIBSPARSE)
സ്ഥിതം errcode_t sparse_open(സ്ഥിരം അക്ഷരം *name EXT2FS_ATTR((unused)),
			     പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	തിരികെ EXT2_ET_UNIMPLEMENTED;
}
സ്ഥിതം errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	തിരികെ EXT2_ET_UNIMPLEMENTED;
}
സ്ഥിതം ഘടന struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
സ്ഥിതം ഘടന struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#അല്ലെങ്കിൽ
#അന്തര്ഭാവ <sparse/sparse.h>

ഘടന sparse_map {
	പൂർണ്ണസംഖ്യ			fd;
	അക്ഷരം			**blocks;
	പൂർണ്ണസംഖ്യ			block_size;
	uint64_t		blocks_എണ്ണം;
	അക്ഷരം			*file;
	ഘടന sparse_file	*sparse_file;
	io_channel		channel;
};

ഘടന sparse_io_params {
	പൂർണ്ണസംഖ്യ			fd;
	അക്ഷരം			*file;
	uint64_t		blocks_എണ്ണം;
	unsigned പൂർണ്ണസംഖ്യ		block_size;
};

സ്ഥിതം errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf);

സ്ഥിതം ശൂന്യം free_sparse_blocks(ഘടന sparse_map *sm)
{
	uint64_t i;

	വേണ്ടി (i = 0; i < sm->blocks_എണ്ണം; ++i)
		സ്വതന്ത്രം(sm->blocks[i]);
	സ്വതന്ത്രം(sm->blocks);
	sm->blocks = ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ sparse_import_segment(ശൂന്യം *priv, സ്ഥിരം ശൂന്യം *data, size_t len,
				 unsigned പൂർണ്ണസംഖ്യ block, unsigned പൂർണ്ണസംഖ്യ nr_blocks)
{
	ഘടന sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	എങ്കിൽ (!nr_blocks || len % sm->block_size)
		തിരികെ 0;

	തിരികെ sparse_write_blk(sm->channel, block, nr_blocks, data);
}

സ്ഥിതം errcode_t io_manager_import_sparse(ഘടന sparse_io_params *params,
					  ഘടന sparse_map *sm, io_channel io)
{
	പൂർണ്ണസംഖ്യ fd;
	errcode_t retval;
	ഘടന sparse_file *sparse_file;

	എങ്കിൽ (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		എങ്കിൽ (fd < 0) {
			retval = -1;
			പോകുക err_open;
		}
	} അല്ലെങ്കിൽ
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	എങ്കിൽ (!sparse_file) {
		retval = -1;
		പോകുക err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_എണ്ണം = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_എണ്ണം, വലുപ്പം(അക്ഷരം*));
	എങ്കിൽ (!sm->blocks) {
		retval = -1;
		പോകുക err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	എങ്കിൽ (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	തിരികെ retval;
}

സ്ഥിതം errcode_t io_manager_configure(ഘടന sparse_io_params *params,
				      പൂർണ്ണസംഖ്യ flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	ഘടന sparse_map *sm = calloc(1, വലുപ്പം(*sm));
	എങ്കിൽ (!sm)
		തിരികെ EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	എങ്കിൽ (retval) {
		എങ്കിൽ (!params->block_size || !params->blocks_എണ്ണം) {
			retval = EINVAL;
			പോകുക err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_എണ്ണം = params->blocks_എണ്ണം;
		sm->blocks = calloc(params->blocks_എണ്ണം, വലുപ്പം(ശൂന്യം*));
		എങ്കിൽ (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			പോകുക err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_എണ്ണം;

	എങ്കിൽ (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		എങ്കിൽ (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			പോകുക err_alloc;
		}
		എങ്കിൽ (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_ദ്വിമാന,
				      0644);
			എങ്കിൽ (sm->fd < 0) {
				retval = errno;
				പോകുക err_open;
			}
		} അല്ലെങ്കിൽ
			sm->fd = params->fd;
	} അല്ലെങ്കിൽ {
		sm->fd = -1;
		sm->sparse_file = ശൂന്യ;
	}
	തിരികെ 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	സ്വതന്ത്രം(sm);
	തിരികെ retval;
}

സ്ഥിതം errcode_t sparse_open_channel(ഘടന sparse_io_params *sparse_params,
				     പൂർണ്ണസംഖ്യ flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, വലുപ്പം(ഘടന struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	എങ്കിൽ (retval) {
		സ്വതന്ത്രം(io);
		തിരികെ retval;
	}

	*channel = io;
	തിരികെ 0;
}

സ്ഥിതം errcode_t read_sparse_argv(സ്ഥിരം അക്ഷരം *name, bool is_fd,
				  ഘടന sparse_io_params *sparse_params)
{
	പൂർണ്ണസംഖ്യ ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_എണ്ണം = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	എങ്കിൽ (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		തിരികെ EXT2_ET_NO_MEMORY;
	}

	എങ്കിൽ (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_എണ്ണം,
			     &sparse_params->block_size);
	} അല്ലെങ്കിൽ {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_എണ്ണം,
			     &sparse_params->block_size);
	}

	എങ്കിൽ (ret < 1) {
		സ്വതന്ത്രം(sparse_params->file);
		തിരികെ EINVAL;
	}
	തിരികെ 0;
}

സ്ഥിതം errcode_t sparse_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, io_channel *channel)
{
	errcode_t retval;
	ഘടന sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	എങ്കിൽ (retval)
		തിരികെ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	എങ്കിൽ (retval)
		തിരികെ retval;
	(*channel)->manager = sparse_io_manager;

	തിരികെ retval;
}

സ്ഥിതം errcode_t sparsefd_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, io_channel *channel)
{
	errcode_t retval;
	ഘടന sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	എങ്കിൽ (retval)
		തിരികെ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	എങ്കിൽ (retval)
		തിരികെ retval;
	(*channel)->manager = sparsefd_io_manager;

	തിരികെ retval;
}

സ്ഥിതം errcode_t sparse_merge_blocks(ഘടന sparse_map *sm, uint64_t start,
					uint64_t അക്കം)
{
	അക്ഷരം *buf;
	uint64_t i;
	unsigned പൂർണ്ണസംഖ്യ block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(അക്കം, block_size);
	എങ്കിൽ (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)അക്കം * block_size);
		തിരികെ EXT2_ET_NO_MEMORY;
	}

	വേണ്ടി (i = 0; i < അക്കം; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		സ്വതന്ത്രം(sm->blocks[start + i]);
		sm->blocks[start + i] = ശൂന്യ;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * അക്കം, start);

	തിരികെ retval;
}

സ്ഥിതം errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	ഘടന sparse_map *sm = channel->private_data;

	എങ്കിൽ (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == ശൂന്യ) ? -1 : 0;
		വേണ്ടി (i = 0; i < sm->blocks_എണ്ണം; ++i) {
			എങ്കിൽ (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} അല്ലെങ്കിൽ എങ്കിൽ (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			എങ്കിൽ (retval)
				പോകുക ret;
		}
		എങ്കിൽ (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_എണ്ണം - chunk_start);
			എങ്കിൽ (retval)
				പോകുക ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	എങ്കിൽ (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	സ്വതന്ത്രം(sm->file);
	സ്വതന്ത്രം(sm);
	സ്വതന്ത്രം(channel);
	തിരികെ retval;
}

സ്ഥിതം errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	ഘടന sparse_map *sm = channel->private_data;
	പൂർണ്ണസംഖ്യ fd = sm->fd;

	retval = sparse_close_channel(channel);
	എങ്കിൽ (fd >= 0)
		close(fd);

	തിരികെ retval;
}

സ്ഥിതം errcode_t sparse_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize)
{
	channel->block_size = blksize;
	തിരികെ 0;
}

സ്ഥിതം blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, ഘടന sparse_map *sm)
{
	പൂർണ്ണസംഖ്യ ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	തിരികെ ret;
}

സ്ഥിതം errcode_t check_block_size(io_channel channel, ഘടന sparse_map *sm)
{
	എങ്കിൽ (sm->block_size >= channel->block_size)
		തിരികെ 0;
	തിരികെ EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

സ്ഥിതം errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	പൂർണ്ണസംഖ്യ i;
	അക്ഷരം *out = buf;
	blk64_t offset = 0, cur_block;
	ഘടന sparse_map *sm = channel->private_data;

	എങ്കിൽ (check_block_size(channel, sm))
		തിരികെ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	എങ്കിൽ (എണ്ണം < 0) { //partial read
		എണ്ണം = -എണ്ണം;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		എങ്കിൽ (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, എണ്ണം);
		അല്ലെങ്കിൽ
			memset(out, 0, എണ്ണം);
	} അല്ലെങ്കിൽ {
		വേണ്ടി (i = 0; i < എണ്ണം; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			എങ്കിൽ (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			അല്ലെങ്കിൽ എങ്കിൽ (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	തിരികെ 0;
}

സ്ഥിതം errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	തിരികെ sparse_read_blk64(channel, block, എണ്ണം, buf);
}

സ്ഥിതം errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	പൂർണ്ണസംഖ്യ i;
	blk64_t offset = 0, cur_block;
	സ്ഥിരം അക്ഷരം *in = buf;
	ഘടന sparse_map *sm = channel->private_data;

	എങ്കിൽ (check_block_size(channel, sm))
		തിരികെ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	എങ്കിൽ (എണ്ണം < 0) { //partial write
		എണ്ണം = -എണ്ണം;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		എങ്കിൽ (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			എങ്കിൽ (!sm->blocks[cur_block])
				തിരികെ EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, എണ്ണം);
	} അല്ലെങ്കിൽ {
		വേണ്ടി (i = 0; i < എണ്ണം; ++i) {
			എങ്കിൽ (block + i >= sm->blocks_എണ്ണം)
				തിരികെ 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			എങ്കിൽ (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				എങ്കിൽ (!sm->blocks[cur_block])
					തിരികെ EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	തിരികെ 0;
}

സ്ഥിതം errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	തിരികെ sparse_write_blk64(channel, block, എണ്ണം, buf);
}

സ്ഥിതം errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long എണ്ണം)
{
	blk64_t cur_block, offset;
	ഘടന sparse_map *sm = channel->private_data;

	എങ്കിൽ (check_block_size(channel, sm))
		തിരികെ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	വേണ്ടി (unsigned long long i = 0; i < എണ്ണം; ++i) {
		എങ്കിൽ (blk + i >= sm->blocks_എണ്ണം)
			തിരികെ 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		എങ്കിൽ (!sm->blocks[cur_block])
			തുടരുക;
		സ്വതന്ത്രം(sm->blocks[cur_block]);
		sm->blocks[cur_block] = ശൂന്യ;
	}
	തിരികെ 0;
}

സ്ഥിതം errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long എണ്ണം)
{
	തിരികെ sparse_discard(channel, blk, എണ്ണം);
}

സ്ഥിതം errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	തിരികെ 0;
}

സ്ഥിതം errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   സ്ഥിരം അക്ഷരം *option __attribute__((unused)),
                                   സ്ഥിരം അക്ഷരം *arg __attribute__((unused)))
{
	തിരികെ 0;
}

സ്ഥിതം errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long എണ്ണം __attribute__((unused)))
{
	തിരികെ 0;
}

സ്ഥിതം ഘടന struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ശൂന്യ,
	.set_option		= sparse_set_option,
	.get_stats		= ശൂന്യ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

സ്ഥിതം ഘടന struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ശൂന്യ,
	.set_option		= sparse_set_option,
	.get_stats		= ശൂന്യ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
