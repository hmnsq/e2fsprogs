#ضمن "config.h"
#ضمن <sys/types.h>
#ضمن <sys/stat.h>
#ضمن <fcntl.h>
#ضمن <unistd.h>
#ضمن <stdint.h>
#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ܐܢ !defined(ENABLE_LIBSPARSE)
ساكن errcode_t sparse_open(ثابت حرف *name EXT2FS_ATTR((unused)),
			     صحيح flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	ܦܢܐ EXT2_ET_UNIMPLEMENTED;
}
ساكن errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	ܦܢܐ EXT2_ET_UNIMPLEMENTED;
}
ساكن بنية struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
ساكن بنية struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#ܐܚܪܢܐ
#ضمن <sparse/sparse.h>

بنية sparse_map {
	صحيح			fd;
	حرف			**blocks;
	صحيح			block_size;
	uint64_t		blocks_count;
	حرف			*file;
	بنية sparse_file	*sparse_file;
	io_channel		channel;
};

بنية sparse_io_params {
	صحيح			fd;
	حرف			*file;
	uint64_t		blocks_count;
	unsigned صحيح		block_size;
};

ساكن errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  صحيح count, ثابت فراغ *buf);

ساكن فراغ free_sparse_blocks(بنية sparse_map *sm)
{
	uint64_t i;

	لكل (i = 0; i < sm->blocks_count; ++i)
		حرر(sm->blocks[i]);
	حرر(sm->blocks);
	sm->blocks = عدم;
}

ساكن صحيح sparse_import_segment(فراغ *priv, ثابت فراغ *data, size_t len,
				 unsigned صحيح block, unsigned صحيح nr_blocks)
{
	بنية sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	ܐܢ (!nr_blocks || len % sm->block_size)
		ܦܢܐ 0;

	ܦܢܐ sparse_write_blk(sm->channel, block, nr_blocks, data);
}

ساكن errcode_t io_manager_import_sparse(بنية sparse_io_params *params,
					  بنية sparse_map *sm, io_channel io)
{
	صحيح fd;
	errcode_t retval;
	بنية sparse_file *sparse_file;

	ܐܢ (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		ܐܢ (fd < 0) {
			retval = -1;
			اذهب err_open;
		}
	} ܐܚܪܢܐ
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	ܐܢ (!sparse_file) {
		retval = -1;
		اذهب err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_count = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_count, حجم(حرف*));
	ܐܢ (!sm->blocks) {
		retval = -1;
		اذهب err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	ܐܢ (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	ܦܢܐ retval;
}

ساكن errcode_t io_manager_configure(بنية sparse_io_params *params,
				      صحيح flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	بنية sparse_map *sm = calloc(1, حجم(*sm));
	ܐܢ (!sm)
		ܦܢܐ EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	ܐܢ (retval) {
		ܐܢ (!params->block_size || !params->blocks_count) {
			retval = EINVAL;
			اذهب err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_count = params->blocks_count;
		sm->blocks = calloc(params->blocks_count, حجم(فراغ*));
		ܐܢ (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			اذهب err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_count;

	ܐܢ (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		ܐܢ (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			اذهب err_alloc;
		}
		ܐܢ (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_BINARY,
				      0644);
			ܐܢ (sm->fd < 0) {
				retval = errno;
				اذهب err_open;
			}
		} ܐܚܪܢܐ
			sm->fd = params->fd;
	} ܐܚܪܢܐ {
		sm->fd = -1;
		sm->sparse_file = عدم;
	}
	ܦܢܐ 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	حرر(sm);
	ܦܢܐ retval;
}

ساكن errcode_t sparse_open_channel(بنية sparse_io_params *sparse_params,
				     صحيح flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	ܐܢ (retval) {
		حرر(io);
		ܦܢܐ retval;
	}

	*channel = io;
	ܦܢܐ 0;
}

ساكن errcode_t read_sparse_argv(ثابت حرف *name, bool is_fd,
				  بنية sparse_io_params *sparse_params)
{
	صحيح ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_count = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	ܐܢ (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		ܦܢܐ EXT2_ET_NO_MEMORY;
	}

	ܐܢ (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_count,
			     &sparse_params->block_size);
	} ܐܚܪܢܐ {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_count,
			     &sparse_params->block_size);
	}

	ܐܢ (ret < 1) {
		حرر(sparse_params->file);
		ܦܢܐ EINVAL;
	}
	ܦܢܐ 0;
}

ساكن errcode_t sparse_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	errcode_t retval;
	بنية sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	ܐܢ (retval)
		ܦܢܐ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ܐܢ (retval)
		ܦܢܐ retval;
	(*channel)->manager = sparse_io_manager;

	ܦܢܐ retval;
}

ساكن errcode_t sparsefd_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	errcode_t retval;
	بنية sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	ܐܢ (retval)
		ܦܢܐ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ܐܢ (retval)
		ܦܢܐ retval;
	(*channel)->manager = sparsefd_io_manager;

	ܦܢܐ retval;
}

ساكن errcode_t sparse_merge_blocks(بنية sparse_map *sm, uint64_t start,
					uint64_t num)
{
	حرف *buf;
	uint64_t i;
	unsigned صحيح block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(num, block_size);
	ܐܢ (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)num * block_size);
		ܦܢܐ EXT2_ET_NO_MEMORY;
	}

	لكل (i = 0; i < num; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		حرر(sm->blocks[start + i]);
		sm->blocks[start + i] = عدم;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * num, start);

	ܦܢܐ retval;
}

ساكن errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	بنية sparse_map *sm = channel->private_data;

	ܐܢ (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == عدم) ? -1 : 0;
		لكل (i = 0; i < sm->blocks_count; ++i) {
			ܐܢ (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} ܐܚܪܢܐ ܐܢ (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			ܐܢ (retval)
				اذهب ret;
		}
		ܐܢ (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_count - chunk_start);
			ܐܢ (retval)
				اذهب ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	ܐܢ (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	حرر(sm->file);
	حرر(sm);
	حرر(channel);
	ܦܢܐ retval;
}

ساكن errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	بنية sparse_map *sm = channel->private_data;
	صحيح fd = sm->fd;

	retval = sparse_close_channel(channel);
	ܐܢ (fd >= 0)
		close(fd);

	ܦܢܐ retval;
}

ساكن errcode_t sparse_set_blksize(io_channel channel, صحيح blksize)
{
	channel->block_size = blksize;
	ܦܢܐ 0;
}

ساكن blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, بنية sparse_map *sm)
{
	صحيح ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	ܦܢܐ ret;
}

ساكن errcode_t check_block_size(io_channel channel, بنية sparse_map *sm)
{
	ܐܢ (sm->block_size >= channel->block_size)
		ܦܢܐ 0;
	ܦܢܐ EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

ساكن errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   صحيح count, فراغ *buf)
{
	صحيح i;
	حرف *out = buf;
	blk64_t offset = 0, cur_block;
	بنية sparse_map *sm = channel->private_data;

	ܐܢ (check_block_size(channel, sm))
		ܦܢܐ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	ܐܢ (count < 0) { //partial read
		count = -count;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		ܐܢ (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, count);
		ܐܚܪܢܐ
			memset(out, 0, count);
	} ܐܚܪܢܐ {
		لكل (i = 0; i < count; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			ܐܢ (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			ܐܚܪܢܐ ܐܢ (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	ܦܢܐ 0;
}

ساكن errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 صحيح count, فراغ *buf)
{
	ܦܢܐ sparse_read_blk64(channel, block, count, buf);
}

ساكن errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    صحيح count, ثابت فراغ *buf)
{
	صحيح i;
	blk64_t offset = 0, cur_block;
	ثابت حرف *in = buf;
	بنية sparse_map *sm = channel->private_data;

	ܐܢ (check_block_size(channel, sm))
		ܦܢܐ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	ܐܢ (count < 0) { //partial write
		count = -count;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		ܐܢ (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			ܐܢ (!sm->blocks[cur_block])
				ܦܢܐ EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, count);
	} ܐܚܪܢܐ {
		لكل (i = 0; i < count; ++i) {
			ܐܢ (block + i >= sm->blocks_count)
				ܦܢܐ 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			ܐܢ (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				ܐܢ (!sm->blocks[cur_block])
					ܦܢܐ EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	ܦܢܐ 0;
}

ساكن errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  صحيح count, ثابت فراغ *buf)
{
	ܦܢܐ sparse_write_blk64(channel, block, count, buf);
}

ساكن errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long count)
{
	blk64_t cur_block, offset;
	بنية sparse_map *sm = channel->private_data;

	ܐܢ (check_block_size(channel, sm))
		ܦܢܐ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	لكل (unsigned long long i = 0; i < count; ++i) {
		ܐܢ (blk + i >= sm->blocks_count)
			ܦܢܐ 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		ܐܢ (!sm->blocks[cur_block])
			استمر;
		حرر(sm->blocks[cur_block]);
		sm->blocks[cur_block] = عدم;
	}
	ܦܢܐ 0;
}

ساكن errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long count)
{
	ܦܢܐ sparse_discard(channel, blk, count);
}

ساكن errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	ܦܢܐ 0;
}

ساكن errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   ثابت حرف *option __attribute__((unused)),
                                   ثابت حرف *arg __attribute__((unused)))
{
	ܦܢܐ 0;
}

ساكن errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long count __attribute__((unused)))
{
	ܦܢܐ 0;
}

ساكن بنية struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= عدم,
	.set_option		= sparse_set_option,
	.get_stats		= عدم,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

ساكن بنية struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= عدم,
	.set_option		= sparse_set_option,
	.get_stats		= عدم,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
