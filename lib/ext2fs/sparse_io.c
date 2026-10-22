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

#אם !defined(ENABLE_LIBSPARSE)
ساكن errcode_t sparse_open(ثابت حرف *name EXT2FS_ATTR((unused)),
			     صحيح flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	החזר EXT2_ET_UNIMPLEMENTED;
}
ساكن errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	החזר EXT2_ET_UNIMPLEMENTED;
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
#אחרת
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

	עבור (i = 0; i < sm->blocks_count; ++i)
		حرر(sm->blocks[i]);
	حرر(sm->blocks);
	sm->blocks = عدم;
}

ساكن صحيح sparse_import_segment(فراغ *priv, ثابت فراغ *data, size_t len,
				 unsigned صحيح block, unsigned صحيح nr_blocks)
{
	بنية sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	אם (!nr_blocks || len % sm->block_size)
		החזר 0;

	החזר sparse_write_blk(sm->channel, block, nr_blocks, data);
}

ساكن errcode_t io_manager_import_sparse(بنية sparse_io_params *params,
					  بنية sparse_map *sm, io_channel io)
{
	صحيح fd;
	errcode_t retval;
	بنية sparse_file *sparse_file;

	אם (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		אם (fd < 0) {
			retval = -1;
			اذهب err_open;
		}
	} אחרת
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	אם (!sparse_file) {
		retval = -1;
		اذهب err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_count = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_count, حجم(حرف*));
	אם (!sm->blocks) {
		retval = -1;
		اذهب err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	אם (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	החזר retval;
}

ساكن errcode_t io_manager_configure(بنية sparse_io_params *params,
				      صحيح flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	بنية sparse_map *sm = calloc(1, حجم(*sm));
	אם (!sm)
		החזר EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	אם (retval) {
		אם (!params->block_size || !params->blocks_count) {
			retval = EINVAL;
			اذهب err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_count = params->blocks_count;
		sm->blocks = calloc(params->blocks_count, حجم(فراغ*));
		אם (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			اذهب err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_count;

	אם (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		אם (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			اذهب err_alloc;
		}
		אם (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_BINARY,
				      0644);
			אם (sm->fd < 0) {
				retval = errno;
				اذهب err_open;
			}
		} אחרת
			sm->fd = params->fd;
	} אחרת {
		sm->fd = -1;
		sm->sparse_file = عدم;
	}
	החזר 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	حرر(sm);
	החזר retval;
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
	אם (retval) {
		حرر(io);
		החזר retval;
	}

	*channel = io;
	החזר 0;
}

ساكن errcode_t read_sparse_argv(ثابت حرف *name, bool is_fd,
				  بنية sparse_io_params *sparse_params)
{
	صحيح ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_count = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	אם (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		החזר EXT2_ET_NO_MEMORY;
	}

	אם (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_count,
			     &sparse_params->block_size);
	} אחרת {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_count,
			     &sparse_params->block_size);
	}

	אם (ret < 1) {
		حرر(sparse_params->file);
		החזר EINVAL;
	}
	החזר 0;
}

ساكن errcode_t sparse_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	errcode_t retval;
	بنية sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	אם (retval)
		החזר EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	אם (retval)
		החזר retval;
	(*channel)->manager = sparse_io_manager;

	החזר retval;
}

ساكن errcode_t sparsefd_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	errcode_t retval;
	بنية sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	אם (retval)
		החזר EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	אם (retval)
		החזר retval;
	(*channel)->manager = sparsefd_io_manager;

	החזר retval;
}

ساكن errcode_t sparse_merge_blocks(بنية sparse_map *sm, uint64_t start,
					uint64_t num)
{
	حرف *buf;
	uint64_t i;
	unsigned صحيح block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(num, block_size);
	אם (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)num * block_size);
		החזר EXT2_ET_NO_MEMORY;
	}

	עבור (i = 0; i < num; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		حرر(sm->blocks[start + i]);
		sm->blocks[start + i] = عدم;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * num, start);

	החזר retval;
}

ساكن errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	بنية sparse_map *sm = channel->private_data;

	אם (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == عدم) ? -1 : 0;
		עבור (i = 0; i < sm->blocks_count; ++i) {
			אם (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} אחרת אם (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			אם (retval)
				اذهب ret;
		}
		אם (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_count - chunk_start);
			אם (retval)
				اذهب ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	אם (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	حرر(sm->file);
	حرر(sm);
	حرر(channel);
	החזר retval;
}

ساكن errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	بنية sparse_map *sm = channel->private_data;
	صحيح fd = sm->fd;

	retval = sparse_close_channel(channel);
	אם (fd >= 0)
		close(fd);

	החזר retval;
}

ساكن errcode_t sparse_set_blksize(io_channel channel, صحيح blksize)
{
	channel->block_size = blksize;
	החזר 0;
}

ساكن blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, بنية sparse_map *sm)
{
	صحيح ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	החזר ret;
}

ساكن errcode_t check_block_size(io_channel channel, بنية sparse_map *sm)
{
	אם (sm->block_size >= channel->block_size)
		החזר 0;
	החזר EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

ساكن errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   صحيح count, فراغ *buf)
{
	صحيح i;
	حرف *out = buf;
	blk64_t offset = 0, cur_block;
	بنية sparse_map *sm = channel->private_data;

	אם (check_block_size(channel, sm))
		החזר EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	אם (count < 0) { //partial read
		count = -count;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		אם (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, count);
		אחרת
			memset(out, 0, count);
	} אחרת {
		עבור (i = 0; i < count; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			אם (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			אחרת אם (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	החזר 0;
}

ساكن errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 صحيح count, فراغ *buf)
{
	החזר sparse_read_blk64(channel, block, count, buf);
}

ساكن errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    صحيح count, ثابت فراغ *buf)
{
	صحيح i;
	blk64_t offset = 0, cur_block;
	ثابت حرف *in = buf;
	بنية sparse_map *sm = channel->private_data;

	אם (check_block_size(channel, sm))
		החזר EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	אם (count < 0) { //partial write
		count = -count;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		אם (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			אם (!sm->blocks[cur_block])
				החזר EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, count);
	} אחרת {
		עבור (i = 0; i < count; ++i) {
			אם (block + i >= sm->blocks_count)
				החזר 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			אם (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				אם (!sm->blocks[cur_block])
					החזר EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	החזר 0;
}

ساكن errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  صحيح count, ثابت فراغ *buf)
{
	החזר sparse_write_blk64(channel, block, count, buf);
}

ساكن errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long count)
{
	blk64_t cur_block, offset;
	بنية sparse_map *sm = channel->private_data;

	אם (check_block_size(channel, sm))
		החזר EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	עבור (unsigned long long i = 0; i < count; ++i) {
		אם (blk + i >= sm->blocks_count)
			החזר 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		אם (!sm->blocks[cur_block])
			استمر;
		حرر(sm->blocks[cur_block]);
		sm->blocks[cur_block] = عدم;
	}
	החזר 0;
}

ساكن errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long count)
{
	החזר sparse_discard(channel, blk, count);
}

ساكن errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	החזר 0;
}

ساكن errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   ثابت حرف *option __attribute__((unused)),
                                   ثابت حرف *arg __attribute__((unused)))
{
	החזר 0;
}

ساكن errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long count __attribute__((unused)))
{
	החזר 0;
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
