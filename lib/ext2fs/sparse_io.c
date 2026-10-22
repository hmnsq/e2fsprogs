#அந்தர்பாவ "config.h"
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

#ifndef O_இருமம்
#define O_இருமம் 0
#endif

#எனில் !defined(ENABLE_LIBSPARSE)
நிலையான errcode_t sparse_open(மாறிலி எழுத்து *name EXT2FS_ATTR((unused)),
			     முழு flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	திரும்பு EXT2_ET_UNIMPLEMENTED;
}
நிலையான errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	திரும்பு EXT2_ET_UNIMPLEMENTED;
}
நிலையான கட்டமைப்பு struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
நிலையான கட்டமைப்பு struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#இல்லையெனில்
#அந்தர்பாவ <sparse/sparse.h>

கட்டமைப்பு sparse_map {
	முழு			fd;
	எழுத்து			**blocks;
	முழு			block_size;
	uint64_t		blocks_மொத்தம்;
	எழுத்து			*file;
	கட்டமைப்பு sparse_file	*sparse_file;
	io_channel		channel;
};

கட்டமைப்பு sparse_io_params {
	முழு			fd;
	எழுத்து			*file;
	uint64_t		blocks_மொத்தம்;
	unsigned முழு		block_size;
};

நிலையான errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  முழு மொத்தம், மாறிலி வெறுமை *buf);

நிலையான வெறுமை free_sparse_blocks(கட்டமைப்பு sparse_map *sm)
{
	uint64_t i;

	க்கு (i = 0; i < sm->blocks_மொத்தம்; ++i)
		விடுவி(sm->blocks[i]);
	விடுவி(sm->blocks);
	sm->blocks = வெற்று;
}

நிலையான முழு sparse_import_segment(வெறுமை *priv, மாறிலி வெறுமை *data, size_t len,
				 unsigned முழு block, unsigned முழு nr_blocks)
{
	கட்டமைப்பு sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	எனில் (!nr_blocks || len % sm->block_size)
		திரும்பு 0;

	திரும்பு sparse_write_blk(sm->channel, block, nr_blocks, data);
}

நிலையான errcode_t io_manager_import_sparse(கட்டமைப்பு sparse_io_params *params,
					  கட்டமைப்பு sparse_map *sm, io_channel io)
{
	முழு fd;
	errcode_t retval;
	கட்டமைப்பு sparse_file *sparse_file;

	எனில் (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		எனில் (fd < 0) {
			retval = -1;
			செல் err_open;
		}
	} இல்லையெனில்
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	எனில் (!sparse_file) {
		retval = -1;
		செல் err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_மொத்தம் = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_மொத்தம், அளவு(எழுத்து*));
	எனில் (!sm->blocks) {
		retval = -1;
		செல் err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	எனில் (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	திரும்பு retval;
}

நிலையான errcode_t io_manager_configure(கட்டமைப்பு sparse_io_params *params,
				      முழு flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	கட்டமைப்பு sparse_map *sm = calloc(1, அளவு(*sm));
	எனில் (!sm)
		திரும்பு EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	எனில் (retval) {
		எனில் (!params->block_size || !params->blocks_மொத்தம்) {
			retval = EINVAL;
			செல் err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_மொத்தம் = params->blocks_மொத்தம்;
		sm->blocks = calloc(params->blocks_மொத்தம், அளவு(வெறுமை*));
		எனில் (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			செல் err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_மொத்தம்;

	எனில் (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		எனில் (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			செல் err_alloc;
		}
		எனில் (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_இருமம்,
				      0644);
			எனில் (sm->fd < 0) {
				retval = errno;
				செல் err_open;
			}
		} இல்லையெனில்
			sm->fd = params->fd;
	} இல்லையெனில் {
		sm->fd = -1;
		sm->sparse_file = வெற்று;
	}
	திரும்பு 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	விடுவி(sm);
	திரும்பு retval;
}

நிலையான errcode_t sparse_open_channel(கட்டமைப்பு sparse_io_params *sparse_params,
				     முழு flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, அளவு(கட்டமைப்பு struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	எனில் (retval) {
		விடுவி(io);
		திரும்பு retval;
	}

	*channel = io;
	திரும்பு 0;
}

நிலையான errcode_t read_sparse_argv(மாறிலி எழுத்து *name, bool is_fd,
				  கட்டமைப்பு sparse_io_params *sparse_params)
{
	முழு ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_மொத்தம் = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	எனில் (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		திரும்பு EXT2_ET_NO_MEMORY;
	}

	எனில் (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_மொத்தம்,
			     &sparse_params->block_size);
	} இல்லையெனில் {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_மொத்தம்,
			     &sparse_params->block_size);
	}

	எனில் (ret < 1) {
		விடுவி(sparse_params->file);
		திரும்பு EINVAL;
	}
	திரும்பு 0;
}

நிலையான errcode_t sparse_open(மாறிலி எழுத்து *name, முழு flags, io_channel *channel)
{
	errcode_t retval;
	கட்டமைப்பு sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	எனில் (retval)
		திரும்பு EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	எனில் (retval)
		திரும்பு retval;
	(*channel)->manager = sparse_io_manager;

	திரும்பு retval;
}

நிலையான errcode_t sparsefd_open(மாறிலி எழுத்து *name, முழு flags, io_channel *channel)
{
	errcode_t retval;
	கட்டமைப்பு sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	எனில் (retval)
		திரும்பு EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	எனில் (retval)
		திரும்பு retval;
	(*channel)->manager = sparsefd_io_manager;

	திரும்பு retval;
}

நிலையான errcode_t sparse_merge_blocks(கட்டமைப்பு sparse_map *sm, uint64_t start,
					uint64_t இலக்கம்)
{
	எழுத்து *buf;
	uint64_t i;
	unsigned முழு block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(இலக்கம், block_size);
	எனில் (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)இலக்கம் * block_size);
		திரும்பு EXT2_ET_NO_MEMORY;
	}

	க்கு (i = 0; i < இலக்கம்; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		விடுவி(sm->blocks[start + i]);
		sm->blocks[start + i] = வெற்று;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * இலக்கம், start);

	திரும்பு retval;
}

நிலையான errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	கட்டமைப்பு sparse_map *sm = channel->private_data;

	எனில் (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == வெற்று) ? -1 : 0;
		க்கு (i = 0; i < sm->blocks_மொத்தம்; ++i) {
			எனில் (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} இல்லையெனில் எனில் (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			எனில் (retval)
				செல் ret;
		}
		எனில் (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_மொத்தம் - chunk_start);
			எனில் (retval)
				செல் ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	எனில் (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	விடுவி(sm->file);
	விடுவி(sm);
	விடுவி(channel);
	திரும்பு retval;
}

நிலையான errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	கட்டமைப்பு sparse_map *sm = channel->private_data;
	முழு fd = sm->fd;

	retval = sparse_close_channel(channel);
	எனில் (fd >= 0)
		close(fd);

	திரும்பு retval;
}

நிலையான errcode_t sparse_set_blksize(io_channel channel, முழு blksize)
{
	channel->block_size = blksize;
	திரும்பு 0;
}

நிலையான blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, கட்டமைப்பு sparse_map *sm)
{
	முழு ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	திரும்பு ret;
}

நிலையான errcode_t check_block_size(io_channel channel, கட்டமைப்பு sparse_map *sm)
{
	எனில் (sm->block_size >= channel->block_size)
		திரும்பு 0;
	திரும்பு EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

நிலையான errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   முழு மொத்தம், வெறுமை *buf)
{
	முழு i;
	எழுத்து *out = buf;
	blk64_t offset = 0, cur_block;
	கட்டமைப்பு sparse_map *sm = channel->private_data;

	எனில் (check_block_size(channel, sm))
		திரும்பு EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	எனில் (மொத்தம் < 0) { //partial read
		மொத்தம் = -மொத்தம்;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		எனில் (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, மொத்தம்);
		இல்லையெனில்
			memset(out, 0, மொத்தம்);
	} இல்லையெனில் {
		க்கு (i = 0; i < மொத்தம்; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			எனில் (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			இல்லையெனில் எனில் (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	திரும்பு 0;
}

நிலையான errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 முழு மொத்தம், வெறுமை *buf)
{
	திரும்பு sparse_read_blk64(channel, block, மொத்தம், buf);
}

நிலையான errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	முழு i;
	blk64_t offset = 0, cur_block;
	மாறிலி எழுத்து *in = buf;
	கட்டமைப்பு sparse_map *sm = channel->private_data;

	எனில் (check_block_size(channel, sm))
		திரும்பு EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	எனில் (மொத்தம் < 0) { //partial write
		மொத்தம் = -மொத்தம்;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		எனில் (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			எனில் (!sm->blocks[cur_block])
				திரும்பு EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, மொத்தம்);
	} இல்லையெனில் {
		க்கு (i = 0; i < மொத்தம்; ++i) {
			எனில் (block + i >= sm->blocks_மொத்தம்)
				திரும்பு 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			எனில் (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				எனில் (!sm->blocks[cur_block])
					திரும்பு EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	திரும்பு 0;
}

நிலையான errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	திரும்பு sparse_write_blk64(channel, block, மொத்தம், buf);
}

நிலையான errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long மொத்தம்)
{
	blk64_t cur_block, offset;
	கட்டமைப்பு sparse_map *sm = channel->private_data;

	எனில் (check_block_size(channel, sm))
		திரும்பு EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	க்கு (unsigned long long i = 0; i < மொத்தம்; ++i) {
		எனில் (blk + i >= sm->blocks_மொத்தம்)
			திரும்பு 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		எனில் (!sm->blocks[cur_block])
			தொடர்;
		விடுவி(sm->blocks[cur_block]);
		sm->blocks[cur_block] = வெற்று;
	}
	திரும்பு 0;
}

நிலையான errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long மொத்தம்)
{
	திரும்பு sparse_discard(channel, blk, மொத்தம்);
}

நிலையான errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	திரும்பு 0;
}

நிலையான errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   மாறிலி எழுத்து *option __attribute__((unused)),
                                   மாறிலி எழுத்து *arg __attribute__((unused)))
{
	திரும்பு 0;
}

நிலையான errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long மொத்தம் __attribute__((unused)))
{
	திரும்பு 0;
}

நிலையான கட்டமைப்பு struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= வெற்று,
	.set_option		= sparse_set_option,
	.get_stats		= வெற்று,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

நிலையான கட்டமைப்பு struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= வெற்று,
	.set_option		= sparse_set_option,
	.get_stats		= வெற்று,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
