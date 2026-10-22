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

#якщо !defined(ENABLE_LIBSPARSE)
static errcode_t sparse_open(const char *name EXT2FS_ATTR((unused)),
			     ціле flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	повернути EXT2_ET_UNIMPLEMENTED;
}
static errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	повернути EXT2_ET_UNIMPLEMENTED;
}
static struct struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
static struct struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#інакше
#include <sparse/sparse.h>

struct sparse_map {
	ціле			fd;
	char			**blocks;
	ціле			block_size;
	uint64_t		blocks_count;
	char			*file;
	struct sparse_file	*sparse_file;
	io_channel		channel;
};

struct sparse_io_params {
	ціле			fd;
	char			*file;
	uint64_t		blocks_count;
	unsigned ціле		block_size;
};

static errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  ціле count, const void *buf);

static void free_sparse_blocks(struct sparse_map *sm)
{
	uint64_t i;

	для (i = 0; i < sm->blocks_count; ++i)
		free(sm->blocks[i]);
	free(sm->blocks);
	sm->blocks = NULL;
}

static ціле sparse_import_segment(void *priv, const void *data, size_t len,
				 unsigned ціле block, unsigned ціле nr_blocks)
{
	struct sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	якщо (!nr_blocks || len % sm->block_size)
		повернути 0;

	повернути sparse_write_blk(sm->channel, block, nr_blocks, data);
}

static errcode_t io_manager_import_sparse(struct sparse_io_params *params,
					  struct sparse_map *sm, io_channel io)
{
	ціле fd;
	errcode_t retval;
	struct sparse_file *sparse_file;

	якщо (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		якщо (fd < 0) {
			retval = -1;
			goto err_open;
		}
	} інакше
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	якщо (!sparse_file) {
		retval = -1;
		goto err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_count = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_count, sizeof(char*));
	якщо (!sm->blocks) {
		retval = -1;
		goto err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	якщо (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	повернути retval;
}

static errcode_t io_manager_configure(struct sparse_io_params *params,
				      ціле flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	struct sparse_map *sm = calloc(1, sizeof(*sm));
	якщо (!sm)
		повернути EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	якщо (retval) {
		якщо (!params->block_size || !params->blocks_count) {
			retval = EINVAL;
			goto err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_count = params->blocks_count;
		sm->blocks = calloc(params->blocks_count, sizeof(void*));
		якщо (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			goto err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_count;

	якщо (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		якщо (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			goto err_alloc;
		}
		якщо (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_BINARY,
				      0644);
			якщо (sm->fd < 0) {
				retval = errno;
				goto err_open;
			}
		} інакше
			sm->fd = params->fd;
	} інакше {
		sm->fd = -1;
		sm->sparse_file = NULL;
	}
	повернути 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	free(sm);
	повернути retval;
}

static errcode_t sparse_open_channel(struct sparse_io_params *sparse_params,
				     ціле flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, sizeof(struct struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	якщо (retval) {
		free(io);
		повернути retval;
	}

	*channel = io;
	повернути 0;
}

static errcode_t read_sparse_argv(const char *name, bool is_fd,
				  struct sparse_io_params *sparse_params)
{
	ціле ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_count = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	якщо (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		повернути EXT2_ET_NO_MEMORY;
	}

	якщо (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_count,
			     &sparse_params->block_size);
	} інакше {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_count,
			     &sparse_params->block_size);
	}

	якщо (ret < 1) {
		free(sparse_params->file);
		повернути EINVAL;
	}
	повернути 0;
}

static errcode_t sparse_open(const char *name, ціле flags, io_channel *channel)
{
	errcode_t retval;
	struct sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	якщо (retval)
		повернути EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	якщо (retval)
		повернути retval;
	(*channel)->manager = sparse_io_manager;

	повернути retval;
}

static errcode_t sparsefd_open(const char *name, ціле flags, io_channel *channel)
{
	errcode_t retval;
	struct sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	якщо (retval)
		повернути EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	якщо (retval)
		повернути retval;
	(*channel)->manager = sparsefd_io_manager;

	повернути retval;
}

static errcode_t sparse_merge_blocks(struct sparse_map *sm, uint64_t start,
					uint64_t num)
{
	char *buf;
	uint64_t i;
	unsigned ціле block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(num, block_size);
	якщо (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)num * block_size);
		повернути EXT2_ET_NO_MEMORY;
	}

	для (i = 0; i < num; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		free(sm->blocks[start + i]);
		sm->blocks[start + i] = NULL;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * num, start);

	повернути retval;
}

static errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	struct sparse_map *sm = channel->private_data;

	якщо (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == NULL) ? -1 : 0;
		для (i = 0; i < sm->blocks_count; ++i) {
			якщо (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} інакше якщо (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			якщо (retval)
				goto ret;
		}
		якщо (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_count - chunk_start);
			якщо (retval)
				goto ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	якщо (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	free(sm->file);
	free(sm);
	free(channel);
	повернути retval;
}

static errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	struct sparse_map *sm = channel->private_data;
	ціле fd = sm->fd;

	retval = sparse_close_channel(channel);
	якщо (fd >= 0)
		close(fd);

	повернути retval;
}

static errcode_t sparse_set_blksize(io_channel channel, ціле blksize)
{
	channel->block_size = blksize;
	повернути 0;
}

static blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, struct sparse_map *sm)
{
	ціле ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	повернути ret;
}

static errcode_t check_block_size(io_channel channel, struct sparse_map *sm)
{
	якщо (sm->block_size >= channel->block_size)
		повернути 0;
	повернути EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

static errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   ціле count, void *buf)
{
	ціле i;
	char *out = buf;
	blk64_t offset = 0, cur_block;
	struct sparse_map *sm = channel->private_data;

	якщо (check_block_size(channel, sm))
		повернути EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	якщо (count < 0) { //partial read
		count = -count;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		якщо (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, count);
		інакше
			memset(out, 0, count);
	} інакше {
		для (i = 0; i < count; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			якщо (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			інакше якщо (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	повернути 0;
}

static errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 ціле count, void *buf)
{
	повернути sparse_read_blk64(channel, block, count, buf);
}

static errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    ціле count, const void *buf)
{
	ціле i;
	blk64_t offset = 0, cur_block;
	const char *in = buf;
	struct sparse_map *sm = channel->private_data;

	якщо (check_block_size(channel, sm))
		повернути EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	якщо (count < 0) { //partial write
		count = -count;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		якщо (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			якщо (!sm->blocks[cur_block])
				повернути EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, count);
	} інакше {
		для (i = 0; i < count; ++i) {
			якщо (block + i >= sm->blocks_count)
				повернути 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			якщо (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				якщо (!sm->blocks[cur_block])
					повернути EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	повернути 0;
}

static errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  ціле count, const void *buf)
{
	повернути sparse_write_blk64(channel, block, count, buf);
}

static errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long count)
{
	blk64_t cur_block, offset;
	struct sparse_map *sm = channel->private_data;

	якщо (check_block_size(channel, sm))
		повернути EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	для (unsigned long long i = 0; i < count; ++i) {
		якщо (blk + i >= sm->blocks_count)
			повернути 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		якщо (!sm->blocks[cur_block])
			continue;
		free(sm->blocks[cur_block]);
		sm->blocks[cur_block] = NULL;
	}
	повернути 0;
}

static errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long count)
{
	повернути sparse_discard(channel, blk, count);
}

static errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	повернути 0;
}

static errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   const char *option __attribute__((unused)),
                                   const char *arg __attribute__((unused)))
{
	повернути 0;
}

static errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long count __attribute__((unused)))
{
	повернути 0;
}

static struct struct_io_manager struct_sparse_manager = {
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

static struct struct_io_manager struct_sparsefd_manager = {
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
