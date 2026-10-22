#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

#ifndef O_ଦ୍ୱିମିକ
#define O_ଦ୍ୱିମିକ 0
#endif

#ଯଦି !defined(ENABLE_LIBSPARSE)
ସ୍ଥିର errcode_t sparse_open(ଅଚଳ ଅକ୍ଷର *name EXT2FS_ATTR((unused)),
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
}
ସ୍ଥିର errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	ଫେରନ୍ତୁ EXT2_ET_UNIMPLEMENTED;
}
ସ୍ଥିର ସଂରଚନା struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
ସ୍ଥିର ସଂରଚନା struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#ଅନ୍ୟଥା
#ଅନ୍ତର୍ଭାଵ <sparse/sparse.h>

ସଂରଚନା sparse_map {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			fd;
	ଅକ୍ଷର			**blocks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			block_size;
	uint64_t		blocks_ଗଣନା;
	ଅକ୍ଷର			*file;
	ସଂରଚନା sparse_file	*sparse_file;
	io_channel		channel;
};

ସଂରଚନା sparse_io_params {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			fd;
	ଅକ୍ଷର			*file;
	uint64_t		blocks_ଗଣନା;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		block_size;
};

ସ୍ଥିର errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf);

ସ୍ଥିର ଶୂନ୍ୟ free_sparse_blocks(ସଂରଚନା sparse_map *sm)
{
	uint64_t i;

	ପାଇଁ (i = 0; i < sm->blocks_ଗଣନା; ++i)
		ମୁକ୍ତ(sm->blocks[i]);
	ମୁକ୍ତ(sm->blocks);
	sm->blocks = ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sparse_import_segment(ଶୂନ୍ୟ *priv, ଅଚଳ ଶୂନ୍ୟ *data, size_t len,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା block, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_blocks)
{
	ସଂରଚନା sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	ଯଦି (!nr_blocks || len % sm->block_size)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ sparse_write_blk(sm->channel, block, nr_blocks, data);
}

ସ୍ଥିର errcode_t io_manager_import_sparse(ସଂରଚନା sparse_io_params *params,
					  ସଂରଚନା sparse_map *sm, io_channel io)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	errcode_t retval;
	ସଂରଚନା sparse_file *sparse_file;

	ଯଦି (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		ଯଦି (fd < 0) {
			retval = -1;
			ଯାଅ err_open;
		}
	} ଅନ୍ୟଥା
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	ଯଦି (!sparse_file) {
		retval = -1;
		ଯାଅ err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_ଗଣନା = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_ଗଣନା, ଆକାର(ଅକ୍ଷର*));
	ଯଦି (!sm->blocks) {
		retval = -1;
		ଯାଅ err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	ଯଦି (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t io_manager_configure(ସଂରଚନା sparse_io_params *params,
				      ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	ସଂରଚନା sparse_map *sm = calloc(1, ଆକାର(*sm));
	ଯଦି (!sm)
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	ଯଦି (retval) {
		ଯଦି (!params->block_size || !params->blocks_ଗଣନା) {
			retval = EINVAL;
			ଯାଅ err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_ଗଣନା = params->blocks_ଗଣନା;
		sm->blocks = calloc(params->blocks_ଗଣନା, ଆକାର(ଶୂନ୍ୟ*));
		ଯଦି (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			ଯାଅ err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_ଗଣନା;

	ଯଦି (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		ଯଦି (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			ଯାଅ err_alloc;
		}
		ଯଦି (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_ଦ୍ୱିମିକ,
				      0644);
			ଯଦି (sm->fd < 0) {
				retval = errno;
				ଯାଅ err_open;
			}
		} ଅନ୍ୟଥା
			sm->fd = params->fd;
	} ଅନ୍ୟଥା {
		sm->fd = -1;
		sm->sparse_file = ଖାଲି;
	}
	ଫେରନ୍ତୁ 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	ମୁକ୍ତ(sm);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t sparse_open_channel(ସଂରଚନା sparse_io_params *sparse_params,
				     ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, ଆକାର(ସଂରଚନା struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	ଯଦି (retval) {
		ମୁକ୍ତ(io);
		ଫେରନ୍ତୁ retval;
	}

	*channel = io;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t read_sparse_argv(ଅଚଳ ଅକ୍ଷର *name, bool is_fd,
				  ସଂରଚନା sparse_io_params *sparse_params)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_ଗଣନା = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	ଯଦି (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
	}

	ଯଦି (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_ଗଣନା,
			     &sparse_params->block_size);
	} ଅନ୍ୟଥା {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_ଗଣନା,
			     &sparse_params->block_size);
	}

	ଯଦି (ret < 1) {
		ମୁକ୍ତ(sparse_params->file);
		ଫେରନ୍ତୁ EINVAL;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t sparse_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel)
{
	errcode_t retval;
	ସଂରଚନା sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	ଯଦି (retval)
		ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	(*channel)->manager = sparse_io_manager;

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t sparsefd_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel)
{
	errcode_t retval;
	ସଂରଚନା sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	ଯଦି (retval)
		ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	(*channel)->manager = sparsefd_io_manager;

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t sparse_merge_blocks(ସଂରଚନା sparse_map *sm, uint64_t start,
					uint64_t ଅଙ୍କ)
{
	ଅକ୍ଷର *buf;
	uint64_t i;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(ଅଙ୍କ, block_size);
	ଯଦି (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)ଅଙ୍କ * block_size);
		ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
	}

	ପାଇଁ (i = 0; i < ଅଙ୍କ; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		ମୁକ୍ତ(sm->blocks[start + i]);
		sm->blocks[start + i] = ଖାଲି;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * ଅଙ୍କ, start);

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	ସଂରଚନା sparse_map *sm = channel->private_data;

	ଯଦି (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == ଖାଲି) ? -1 : 0;
		ପାଇଁ (i = 0; i < sm->blocks_ଗଣନା; ++i) {
			ଯଦି (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} ଅନ୍ୟଥା ଯଦି (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			ଯଦି (retval)
				ଯାଅ ret;
		}
		ଯଦି (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_ଗଣନା - chunk_start);
			ଯଦି (retval)
				ଯାଅ ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	ଯଦି (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	ମୁକ୍ତ(sm->file);
	ମୁକ୍ତ(sm);
	ମୁକ୍ତ(channel);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	ସଂରଚନା sparse_map *sm = channel->private_data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = sm->fd;

	retval = sparse_close_channel(channel);
	ଯଦି (fd >= 0)
		close(fd);

	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t sparse_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize)
{
	channel->block_size = blksize;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, ସଂରଚନା sparse_map *sm)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର errcode_t check_block_size(io_channel channel, ସଂରଚନା sparse_map *sm)
{
	ଯଦି (sm->block_size >= channel->block_size)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

ସ୍ଥିର errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ଅକ୍ଷର *out = buf;
	blk64_t offset = 0, cur_block;
	ସଂରଚନା sparse_map *sm = channel->private_data;

	ଯଦି (check_block_size(channel, sm))
		ଫେରନ୍ତୁ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	ଯଦି (ଗଣନା_2 < 0) { //partial read
		ଗଣନା_2 = -ଗଣନା_2;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		ଯଦି (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, ଗଣନା_2);
		ଅନ୍ୟଥା
			memset(out, 0, ଗଣନା_2);
	} ଅନ୍ୟଥା {
		ପାଇଁ (i = 0; i < ଗଣନା_2; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			ଯଦି (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			ଅନ୍ୟଥା ଯଦି (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ sparse_read_blk64(channel, block, ଗଣନା_2, buf);
}

ସ୍ଥିର errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	blk64_t offset = 0, cur_block;
	ଅଚଳ ଅକ୍ଷର *in = buf;
	ସଂରଚନା sparse_map *sm = channel->private_data;

	ଯଦି (check_block_size(channel, sm))
		ଫେରନ୍ତୁ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	ଯଦି (ଗଣନା_2 < 0) { //partial write
		ଗଣନା_2 = -ଗଣନା_2;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		ଯଦି (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			ଯଦି (!sm->blocks[cur_block])
				ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, ଗଣନା_2);
	} ଅନ୍ୟଥା {
		ପାଇଁ (i = 0; i < ଗଣନା_2; ++i) {
			ଯଦି (block + i >= sm->blocks_ଗଣନା)
				ଫେରନ୍ତୁ 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			ଯଦି (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				ଯଦି (!sm->blocks[cur_block])
					ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ sparse_write_blk64(channel, block, ଗଣନା_2, buf);
}

ସ୍ଥିର errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long ଗଣନା_2)
{
	blk64_t cur_block, offset;
	ସଂରଚନା sparse_map *sm = channel->private_data;

	ଯଦି (check_block_size(channel, sm))
		ଫେରନ୍ତୁ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	ପାଇଁ (unsigned long long i = 0; i < ଗଣନା_2; ++i) {
		ଯଦି (blk + i >= sm->blocks_ଗଣନା)
			ଫେରନ୍ତୁ 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		ଯଦି (!sm->blocks[cur_block])
			ଜାରି;
		ମୁକ୍ତ(sm->blocks[cur_block]);
		sm->blocks[cur_block] = ଖାଲି;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long ଗଣନା_2)
{
	ଫେରନ୍ତୁ sparse_discard(channel, blk, ଗଣନା_2);
}

ସ୍ଥିର errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   ଅଚଳ ଅକ୍ଷର *option __attribute__((unused)),
                                   ଅଚଳ ଅକ୍ଷର *arg __attribute__((unused)))
{
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long ଗଣନା_2 __attribute__((unused)))
{
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ଖାଲି,
	.set_option		= sparse_set_option,
	.get_stats		= ଖାଲି,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

ସ୍ଥିର ସଂରଚନା struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ଖାଲି,
	.set_option		= sparse_set_option,
	.get_stats		= ଖାଲି,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
