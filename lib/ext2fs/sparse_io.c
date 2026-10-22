#شامل "config.h"
#شامل <sys/types.h>
#شامل <sys/stat.h>
#شامل <fcntl.h>
#شامل <unistd.h>
#شامل <stdint.h>
#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#ifndef O_دودویی
#define O_دودویی 0
#endif

#اگر !defined(ENABLE_LIBSPARSE)
ایستا errcode_t sparse_open(ثابت نویسه *name EXT2FS_ATTR((unused)),
			     صحیح flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	بازگشت EXT2_ET_UNIMPLEMENTED;
}
ایستا errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	بازگشت EXT2_ET_UNIMPLEMENTED;
}
ایستا ساختار struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
ایستا ساختار struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#وگرنه
#شامل <sparse/sparse.h>

ساختار sparse_map {
	صحیح			fd;
	نویسه			**blocks;
	صحیح			block_size;
	uint64_t		blocks_شمار;
	نویسه			*file;
	ساختار sparse_file	*sparse_file;
	io_channel		channel;
};

ساختار sparse_io_params {
	صحیح			fd;
	نویسه			*file;
	uint64_t		blocks_شمار;
	unsigned صحیح		block_size;
};

ایستا errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  صحیح شمار, ثابت تهی *buf);

ایستا تهی free_sparse_blocks(ساختار sparse_map *sm)
{
	uint64_t i;

	برای (i = 0; i < sm->blocks_شمار; ++i)
		آزاد(sm->blocks[i]);
	آزاد(sm->blocks);
	sm->blocks = پوچ;
}

ایستا صحیح sparse_import_segment(تهی *priv, ثابت تهی *data, size_t len,
				 unsigned صحیح block, unsigned صحیح nr_blocks)
{
	ساختار sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	اگر (!nr_blocks || len % sm->block_size)
		بازگشت 0;

	بازگشت sparse_write_blk(sm->channel, block, nr_blocks, data);
}

ایستا errcode_t io_manager_import_sparse(ساختار sparse_io_params *params,
					  ساختار sparse_map *sm, io_channel io)
{
	صحیح fd;
	errcode_t retval;
	ساختار sparse_file *sparse_file;

	اگر (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		اگر (fd < 0) {
			retval = -1;
			برو err_open;
		}
	} وگرنه
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	اگر (!sparse_file) {
		retval = -1;
		برو err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_شمار = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_شمار, اندازه(نویسه*));
	اگر (!sm->blocks) {
		retval = -1;
		برو err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	اگر (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	بازگشت retval;
}

ایستا errcode_t io_manager_configure(ساختار sparse_io_params *params,
				      صحیح flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	ساختار sparse_map *sm = calloc(1, اندازه(*sm));
	اگر (!sm)
		بازگشت EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	اگر (retval) {
		اگر (!params->block_size || !params->blocks_شمار) {
			retval = EINVAL;
			برو err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_شمار = params->blocks_شمار;
		sm->blocks = calloc(params->blocks_شمار, اندازه(تهی*));
		اگر (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			برو err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_شمار;

	اگر (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		اگر (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			برو err_alloc;
		}
		اگر (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_دودویی,
				      0644);
			اگر (sm->fd < 0) {
				retval = errno;
				برو err_open;
			}
		} وگرنه
			sm->fd = params->fd;
	} وگرنه {
		sm->fd = -1;
		sm->sparse_file = پوچ;
	}
	بازگشت 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	آزاد(sm);
	بازگشت retval;
}

ایستا errcode_t sparse_open_channel(ساختار sparse_io_params *sparse_params,
				     صحیح flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, اندازه(ساختار struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	اگر (retval) {
		آزاد(io);
		بازگشت retval;
	}

	*channel = io;
	بازگشت 0;
}

ایستا errcode_t read_sparse_argv(ثابت نویسه *name, bool is_fd,
				  ساختار sparse_io_params *sparse_params)
{
	صحیح ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_شمار = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	اگر (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		بازگشت EXT2_ET_NO_MEMORY;
	}

	اگر (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_شمار,
			     &sparse_params->block_size);
	} وگرنه {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_شمار,
			     &sparse_params->block_size);
	}

	اگر (ret < 1) {
		آزاد(sparse_params->file);
		بازگشت EINVAL;
	}
	بازگشت 0;
}

ایستا errcode_t sparse_open(ثابت نویسه *name, صحیح flags, io_channel *channel)
{
	errcode_t retval;
	ساختار sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	اگر (retval)
		بازگشت EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	اگر (retval)
		بازگشت retval;
	(*channel)->manager = sparse_io_manager;

	بازگشت retval;
}

ایستا errcode_t sparsefd_open(ثابت نویسه *name, صحیح flags, io_channel *channel)
{
	errcode_t retval;
	ساختار sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	اگر (retval)
		بازگشت EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	اگر (retval)
		بازگشت retval;
	(*channel)->manager = sparsefd_io_manager;

	بازگشت retval;
}

ایستا errcode_t sparse_merge_blocks(ساختار sparse_map *sm, uint64_t start,
					uint64_t رقم)
{
	نویسه *buf;
	uint64_t i;
	unsigned صحیح block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(رقم, block_size);
	اگر (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)رقم * block_size);
		بازگشت EXT2_ET_NO_MEMORY;
	}

	برای (i = 0; i < رقم; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		آزاد(sm->blocks[start + i]);
		sm->blocks[start + i] = پوچ;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * رقم, start);

	بازگشت retval;
}

ایستا errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	ساختار sparse_map *sm = channel->private_data;

	اگر (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == پوچ) ? -1 : 0;
		برای (i = 0; i < sm->blocks_شمار; ++i) {
			اگر (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} وگرنه اگر (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			اگر (retval)
				برو ret;
		}
		اگر (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_شمار - chunk_start);
			اگر (retval)
				برو ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	اگر (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	آزاد(sm->file);
	آزاد(sm);
	آزاد(channel);
	بازگشت retval;
}

ایستا errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	ساختار sparse_map *sm = channel->private_data;
	صحیح fd = sm->fd;

	retval = sparse_close_channel(channel);
	اگر (fd >= 0)
		close(fd);

	بازگشت retval;
}

ایستا errcode_t sparse_set_blksize(io_channel channel, صحیح blksize)
{
	channel->block_size = blksize;
	بازگشت 0;
}

ایستا blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, ساختار sparse_map *sm)
{
	صحیح ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	بازگشت ret;
}

ایستا errcode_t check_block_size(io_channel channel, ساختار sparse_map *sm)
{
	اگر (sm->block_size >= channel->block_size)
		بازگشت 0;
	بازگشت EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

ایستا errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   صحیح شمار, تهی *buf)
{
	صحیح i;
	نویسه *out = buf;
	blk64_t offset = 0, cur_block;
	ساختار sparse_map *sm = channel->private_data;

	اگر (check_block_size(channel, sm))
		بازگشت EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	اگر (شمار < 0) { //partial read
		شمار = -شمار;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		اگر (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, شمار);
		وگرنه
			memset(out, 0, شمار);
	} وگرنه {
		برای (i = 0; i < شمار; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			اگر (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			وگرنه اگر (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	بازگشت 0;
}

ایستا errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 صحیح شمار, تهی *buf)
{
	بازگشت sparse_read_blk64(channel, block, شمار, buf);
}

ایستا errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    صحیح شمار, ثابت تهی *buf)
{
	صحیح i;
	blk64_t offset = 0, cur_block;
	ثابت نویسه *in = buf;
	ساختار sparse_map *sm = channel->private_data;

	اگر (check_block_size(channel, sm))
		بازگشت EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	اگر (شمار < 0) { //partial write
		شمار = -شمار;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		اگر (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			اگر (!sm->blocks[cur_block])
				بازگشت EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, شمار);
	} وگرنه {
		برای (i = 0; i < شمار; ++i) {
			اگر (block + i >= sm->blocks_شمار)
				بازگشت 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			اگر (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				اگر (!sm->blocks[cur_block])
					بازگشت EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	بازگشت 0;
}

ایستا errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  صحیح شمار, ثابت تهی *buf)
{
	بازگشت sparse_write_blk64(channel, block, شمار, buf);
}

ایستا errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long شمار)
{
	blk64_t cur_block, offset;
	ساختار sparse_map *sm = channel->private_data;

	اگر (check_block_size(channel, sm))
		بازگشت EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	برای (unsigned long long i = 0; i < شمار; ++i) {
		اگر (blk + i >= sm->blocks_شمار)
			بازگشت 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		اگر (!sm->blocks[cur_block])
			ادامه;
		آزاد(sm->blocks[cur_block]);
		sm->blocks[cur_block] = پوچ;
	}
	بازگشت 0;
}

ایستا errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long شمار)
{
	بازگشت sparse_discard(channel, blk, شمار);
}

ایستا errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	بازگشت 0;
}

ایستا errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   ثابت نویسه *option __attribute__((unused)),
                                   ثابت نویسه *arg __attribute__((unused)))
{
	بازگشت 0;
}

ایستا errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long شمار __attribute__((unused)))
{
	بازگشت 0;
}

ایستا ساختار struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= پوچ,
	.set_option		= sparse_set_option,
	.get_stats		= پوچ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

ایستا ساختار struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= پوچ,
	.set_option		= sparse_set_option,
	.get_stats		= پوچ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
