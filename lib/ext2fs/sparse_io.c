#include "config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "ext2_fs.h"
#include "ext2fs.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ᬬᬾᬦ᭄ !defined(ENABLE_LIBSPARSE)
static errcode_t sparse_open(const var_na *name EXT2FS_ATTR((unused)),
			     puur_naa_nka flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	ᬯᬮᬶ EXT2_ET_UNIMPLEMENTED;
}
static errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	ᬯᬮᬶ EXT2_ET_UNIMPLEMENTED;
}
static sa_nracanaa struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
static sa_nracanaa struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#ᬮᬶᬬ
#include <sparse/sparse.h>

sa_nracanaa sparse_map {
	puur_naa_nka			fd;
	var_na			**blocks;
	puur_naa_nka			block_size;
	uint64_t		blocks_count;
	var_na			*file;
	sa_nracanaa sparse_file	*sparse_file;
	io_channel		channel;
};

sa_nracanaa sparse_io_params {
	puur_naa_nka			fd;
	var_na			*file;
	uint64_t		blocks_count;
	unsigned puur_naa_nka		block_size;
};

static errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  puur_naa_nka count, const void *buf);

static void free_sparse_blocks(sa_nracanaa sparse_map *sm)
{
	uint64_t i;

	for (i = 0; i < sm->blocks_count; ++i)
		free(sm->blocks[i]);
	free(sm->blocks);
	sm->blocks = NULL;
}

static puur_naa_nka sparse_import_segment(void *priv, const void *data, size_t len,
				 unsigned puur_naa_nka block, unsigned puur_naa_nka nr_blocks)
{
	sa_nracanaa sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	ᬬᬾᬦ᭄ (!nr_blocks || len % sm->block_size)
		ᬯᬮᬶ 0;

	ᬯᬮᬶ sparse_write_blk(sm->channel, block, nr_blocks, data);
}

static errcode_t io_manager_import_sparse(sa_nracanaa sparse_io_params *params,
					  sa_nracanaa sparse_map *sm, io_channel io)
{
	puur_naa_nka fd;
	errcode_t retval;
	sa_nracanaa sparse_file *sparse_file;

	ᬬᬾᬦ᭄ (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		ᬬᬾᬦ᭄ (fd < 0) {
			retval = -1;
			goto err_open;
		}
	} ᬮᬶᬬ
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	ᬬᬾᬦ᭄ (!sparse_file) {
		retval = -1;
		goto err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_count = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_count, parimaa_na(var_na*));
	ᬬᬾᬦ᭄ (!sm->blocks) {
		retval = -1;
		goto err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	ᬬᬾᬦ᭄ (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	ᬯᬮᬶ retval;
}

static errcode_t io_manager_configure(sa_nracanaa sparse_io_params *params,
				      puur_naa_nka flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	sa_nracanaa sparse_map *sm = calloc(1, parimaa_na(*sm));
	ᬬᬾᬦ᭄ (!sm)
		ᬯᬮᬶ EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	ᬬᬾᬦ᭄ (retval) {
		ᬬᬾᬦ᭄ (!params->block_size || !params->blocks_count) {
			retval = EINVAL;
			goto err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_count = params->blocks_count;
		sm->blocks = calloc(params->blocks_count, parimaa_na(void*));
		ᬬᬾᬦ᭄ (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			goto err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_count;

	ᬬᬾᬦ᭄ (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		ᬬᬾᬦ᭄ (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			goto err_alloc;
		}
		ᬬᬾᬦ᭄ (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_BINARY,
				      0644);
			ᬬᬾᬦ᭄ (sm->fd < 0) {
				retval = errno;
				goto err_open;
			}
		} ᬮᬶᬬ
			sm->fd = params->fd;
	} ᬮᬶᬬ {
		sm->fd = -1;
		sm->sparse_file = NULL;
	}
	ᬯᬮᬶ 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	free(sm);
	ᬯᬮᬶ retval;
}

static errcode_t sparse_open_channel(sa_nracanaa sparse_io_params *sparse_params,
				     puur_naa_nka flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, parimaa_na(sa_nracanaa struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	ᬬᬾᬦ᭄ (retval) {
		free(io);
		ᬯᬮᬶ retval;
	}

	*channel = io;
	ᬯᬮᬶ 0;
}

static errcode_t read_sparse_argv(const var_na *name, bool is_fd,
				  sa_nracanaa sparse_io_params *sparse_params)
{
	puur_naa_nka ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_count = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	ᬬᬾᬦ᭄ (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		ᬯᬮᬶ EXT2_ET_NO_MEMORY;
	}

	ᬬᬾᬦ᭄ (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_count,
			     &sparse_params->block_size);
	} ᬮᬶᬬ {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_count,
			     &sparse_params->block_size);
	}

	ᬬᬾᬦ᭄ (ret < 1) {
		free(sparse_params->file);
		ᬯᬮᬶ EINVAL;
	}
	ᬯᬮᬶ 0;
}

static errcode_t sparse_open(const var_na *name, puur_naa_nka flags, io_channel *channel)
{
	errcode_t retval;
	sa_nracanaa sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	ᬬᬾᬦ᭄ (retval)
		ᬯᬮᬶ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ᬬᬾᬦ᭄ (retval)
		ᬯᬮᬶ retval;
	(*channel)->manager = sparse_io_manager;

	ᬯᬮᬶ retval;
}

static errcode_t sparsefd_open(const var_na *name, puur_naa_nka flags, io_channel *channel)
{
	errcode_t retval;
	sa_nracanaa sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	ᬬᬾᬦ᭄ (retval)
		ᬯᬮᬶ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ᬬᬾᬦ᭄ (retval)
		ᬯᬮᬶ retval;
	(*channel)->manager = sparsefd_io_manager;

	ᬯᬮᬶ retval;
}

static errcode_t sparse_merge_blocks(sa_nracanaa sparse_map *sm, uint64_t start,
					uint64_t num)
{
	var_na *buf;
	uint64_t i;
	unsigned puur_naa_nka block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(num, block_size);
	ᬬᬾᬦ᭄ (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)num * block_size);
		ᬯᬮᬶ EXT2_ET_NO_MEMORY;
	}

	for (i = 0; i < num; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		free(sm->blocks[start + i]);
		sm->blocks[start + i] = NULL;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * num, start);

	ᬯᬮᬶ retval;
}

static errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	sa_nracanaa sparse_map *sm = channel->private_data;

	ᬬᬾᬦ᭄ (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == NULL) ? -1 : 0;
		for (i = 0; i < sm->blocks_count; ++i) {
			ᬬᬾᬦ᭄ (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} ᬮᬶᬬ ᬬᬾᬦ᭄ (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			ᬬᬾᬦ᭄ (retval)
				goto ret;
		}
		ᬬᬾᬦ᭄ (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_count - chunk_start);
			ᬬᬾᬦ᭄ (retval)
				goto ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	ᬬᬾᬦ᭄ (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	free(sm->file);
	free(sm);
	free(channel);
	ᬯᬮᬶ retval;
}

static errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	sa_nracanaa sparse_map *sm = channel->private_data;
	puur_naa_nka fd = sm->fd;

	retval = sparse_close_channel(channel);
	ᬬᬾᬦ᭄ (fd >= 0)
		close(fd);

	ᬯᬮᬶ retval;
}

static errcode_t sparse_set_blksize(io_channel channel, puur_naa_nka blksize)
{
	channel->block_size = blksize;
	ᬯᬮᬶ 0;
}

static blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, sa_nracanaa sparse_map *sm)
{
	puur_naa_nka ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	ᬯᬮᬶ ret;
}

static errcode_t check_block_size(io_channel channel, sa_nracanaa sparse_map *sm)
{
	ᬬᬾᬦ᭄ (sm->block_size >= channel->block_size)
		ᬯᬮᬶ 0;
	ᬯᬮᬶ EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

static errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   puur_naa_nka count, void *buf)
{
	puur_naa_nka i;
	var_na *out = buf;
	blk64_t offset = 0, cur_block;
	sa_nracanaa sparse_map *sm = channel->private_data;

	ᬬᬾᬦ᭄ (check_block_size(channel, sm))
		ᬯᬮᬶ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	ᬬᬾᬦ᭄ (count < 0) { //partial read
		count = -count;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		ᬬᬾᬦ᭄ (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, count);
		ᬮᬶᬬ
			memset(out, 0, count);
	} ᬮᬶᬬ {
		for (i = 0; i < count; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			ᬬᬾᬦ᭄ (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			ᬮᬶᬬ ᬬᬾᬦ᭄ (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	ᬯᬮᬶ 0;
}

static errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 puur_naa_nka count, void *buf)
{
	ᬯᬮᬶ sparse_read_blk64(channel, block, count, buf);
}

static errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    puur_naa_nka count, const void *buf)
{
	puur_naa_nka i;
	blk64_t offset = 0, cur_block;
	const var_na *in = buf;
	sa_nracanaa sparse_map *sm = channel->private_data;

	ᬬᬾᬦ᭄ (check_block_size(channel, sm))
		ᬯᬮᬶ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	ᬬᬾᬦ᭄ (count < 0) { //partial write
		count = -count;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		ᬬᬾᬦ᭄ (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			ᬬᬾᬦ᭄ (!sm->blocks[cur_block])
				ᬯᬮᬶ EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, count);
	} ᬮᬶᬬ {
		for (i = 0; i < count; ++i) {
			ᬬᬾᬦ᭄ (block + i >= sm->blocks_count)
				ᬯᬮᬶ 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			ᬬᬾᬦ᭄ (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				ᬬᬾᬦ᭄ (!sm->blocks[cur_block])
					ᬯᬮᬶ EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	ᬯᬮᬶ 0;
}

static errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  puur_naa_nka count, const void *buf)
{
	ᬯᬮᬶ sparse_write_blk64(channel, block, count, buf);
}

static errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long count)
{
	blk64_t cur_block, offset;
	sa_nracanaa sparse_map *sm = channel->private_data;

	ᬬᬾᬦ᭄ (check_block_size(channel, sm))
		ᬯᬮᬶ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	for (unsigned long long i = 0; i < count; ++i) {
		ᬬᬾᬦ᭄ (blk + i >= sm->blocks_count)
			ᬯᬮᬶ 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		ᬬᬾᬦ᭄ (!sm->blocks[cur_block])
			continue;
		free(sm->blocks[cur_block]);
		sm->blocks[cur_block] = NULL;
	}
	ᬯᬮᬶ 0;
}

static errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long count)
{
	ᬯᬮᬶ sparse_discard(channel, blk, count);
}

static errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	ᬯᬮᬶ 0;
}

static errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   const var_na *option __attribute__((unused)),
                                   const var_na *arg __attribute__((unused)))
{
	ᬯᬮᬶ 0;
}

static errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long count __attribute__((unused)))
{
	ᬯᬮᬶ 0;
}

static sa_nracanaa struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= NULL,
	.set_option		= sparse_set_option,
	.get_stats		= NULL,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

static sa_nracanaa struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= NULL,
	.set_option		= sparse_set_option,
	.get_stats		= NULL,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
