#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

#ifndef O_দ্বিমিক
#define O_দ্বিমিক 0
#endif

#যদি !defined(ENABLE_LIBSPARSE)
স্থির errcode_t sparse_open(ধ্রুৱ ৱর্ণ *name EXT2FS_ATTR((unused)),
			     পূর্ণাঙ্ক flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	ঘূৰাই EXT2_ET_UNIMPLEMENTED;
}
স্থির errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	ঘূৰাই EXT2_ET_UNIMPLEMENTED;
}
স্থির সংরচনা struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
স্থির সংরচনা struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#অন্যথা
#অন্তর্ভাৱ <sparse/sparse.h>

সংরচনা sparse_map {
	পূর্ণাঙ্ক			fd;
	ৱর্ণ			**blocks;
	পূর্ণাঙ্ক			block_size;
	uint64_t		blocks_গুনতি;
	ৱর্ণ			*file;
	সংরচনা sparse_file	*sparse_file;
	io_channel		channel;
};

সংরচনা sparse_io_params {
	পূর্ণাঙ্ক			fd;
	ৱর্ণ			*file;
	uint64_t		blocks_গুনতি;
	unsigned পূর্ণাঙ্ক		block_size;
};

স্থির errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf);

স্থির শূন্য free_sparse_blocks(সংরচনা sparse_map *sm)
{
	uint64_t i;

	বাবে (i = 0; i < sm->blocks_গুনতি; ++i)
		মুক্ত(sm->blocks[i]);
	মুক্ত(sm->blocks);
	sm->blocks = রিক্ত;
}

স্থির পূর্ণাঙ্ক sparse_import_segment(শূন্য *priv, ধ্রুৱ শূন্য *data, size_t len,
				 unsigned পূর্ণাঙ্ক block, unsigned পূর্ণাঙ্ক nr_blocks)
{
	সংরচনা sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	যদি (!nr_blocks || len % sm->block_size)
		ঘূৰাই 0;

	ঘূৰাই sparse_write_blk(sm->channel, block, nr_blocks, data);
}

স্থির errcode_t io_manager_import_sparse(সংরচনা sparse_io_params *params,
					  সংরচনা sparse_map *sm, io_channel io)
{
	পূর্ণাঙ্ক fd;
	errcode_t retval;
	সংরচনা sparse_file *sparse_file;

	যদি (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		যদি (fd < 0) {
			retval = -1;
			গচ্ছ err_open;
		}
	} অন্যথা
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	যদি (!sparse_file) {
		retval = -1;
		গচ্ছ err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_গুনতি = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_গুনতি, পরিমাণ(ৱর্ণ*));
	যদি (!sm->blocks) {
		retval = -1;
		গচ্ছ err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	যদি (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	ঘূৰাই retval;
}

স্থির errcode_t io_manager_configure(সংরচনা sparse_io_params *params,
				      পূর্ণাঙ্ক flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	সংরচনা sparse_map *sm = calloc(1, পরিমাণ(*sm));
	যদি (!sm)
		ঘূৰাই EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	যদি (retval) {
		যদি (!params->block_size || !params->blocks_গুনতি) {
			retval = EINVAL;
			গচ্ছ err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_গুনতি = params->blocks_গুনতি;
		sm->blocks = calloc(params->blocks_গুনতি, পরিমাণ(শূন্য*));
		যদি (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			গচ্ছ err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_গুনতি;

	যদি (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		যদি (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			গচ্ছ err_alloc;
		}
		যদি (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_দ্বিমিক,
				      0644);
			যদি (sm->fd < 0) {
				retval = errno;
				গচ্ছ err_open;
			}
		} অন্যথা
			sm->fd = params->fd;
	} অন্যথা {
		sm->fd = -1;
		sm->sparse_file = রিক্ত;
	}
	ঘূৰাই 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	মুক্ত(sm);
	ঘূৰাই retval;
}

স্থির errcode_t sparse_open_channel(সংরচনা sparse_io_params *sparse_params,
				     পূর্ণাঙ্ক flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, পরিমাণ(সংরচনা struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	যদি (retval) {
		মুক্ত(io);
		ঘূৰাই retval;
	}

	*channel = io;
	ঘূৰাই 0;
}

স্থির errcode_t read_sparse_argv(ধ্রুৱ ৱর্ণ *name, bool is_fd,
				  সংরচনা sparse_io_params *sparse_params)
{
	পূর্ণাঙ্ক ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_গুনতি = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	যদি (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		ঘূৰাই EXT2_ET_NO_MEMORY;
	}

	যদি (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_গুনতি,
			     &sparse_params->block_size);
	} অন্যথা {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_গুনতি,
			     &sparse_params->block_size);
	}

	যদি (ret < 1) {
		মুক্ত(sparse_params->file);
		ঘূৰাই EINVAL;
	}
	ঘূৰাই 0;
}

স্থির errcode_t sparse_open(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক flags, io_channel *channel)
{
	errcode_t retval;
	সংরচনা sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	যদি (retval)
		ঘূৰাই EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	যদি (retval)
		ঘূৰাই retval;
	(*channel)->manager = sparse_io_manager;

	ঘূৰাই retval;
}

স্থির errcode_t sparsefd_open(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক flags, io_channel *channel)
{
	errcode_t retval;
	সংরচনা sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	যদি (retval)
		ঘূৰাই EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	যদি (retval)
		ঘূৰাই retval;
	(*channel)->manager = sparsefd_io_manager;

	ঘূৰাই retval;
}

স্থির errcode_t sparse_merge_blocks(সংরচনা sparse_map *sm, uint64_t start,
					uint64_t অঙ্ক)
{
	ৱর্ণ *buf;
	uint64_t i;
	unsigned পূর্ণাঙ্ক block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(অঙ্ক, block_size);
	যদি (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)অঙ্ক * block_size);
		ঘূৰাই EXT2_ET_NO_MEMORY;
	}

	বাবে (i = 0; i < অঙ্ক; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		মুক্ত(sm->blocks[start + i]);
		sm->blocks[start + i] = রিক্ত;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * অঙ্ক, start);

	ঘূৰাই retval;
}

স্থির errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	সংরচনা sparse_map *sm = channel->private_data;

	যদি (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == রিক্ত) ? -1 : 0;
		বাবে (i = 0; i < sm->blocks_গুনতি; ++i) {
			যদি (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} অন্যথা যদি (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			যদি (retval)
				গচ্ছ ret;
		}
		যদি (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_গুনতি - chunk_start);
			যদি (retval)
				গচ্ছ ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	যদি (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	মুক্ত(sm->file);
	মুক্ত(sm);
	মুক্ত(channel);
	ঘূৰাই retval;
}

স্থির errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	সংরচনা sparse_map *sm = channel->private_data;
	পূর্ণাঙ্ক fd = sm->fd;

	retval = sparse_close_channel(channel);
	যদি (fd >= 0)
		close(fd);

	ঘূৰাই retval;
}

স্থির errcode_t sparse_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize)
{
	channel->block_size = blksize;
	ঘূৰাই 0;
}

স্থির blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, সংরচনা sparse_map *sm)
{
	পূর্ণাঙ্ক ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	ঘূৰাই ret;
}

স্থির errcode_t check_block_size(io_channel channel, সংরচনা sparse_map *sm)
{
	যদি (sm->block_size >= channel->block_size)
		ঘূৰাই 0;
	ঘূৰাই EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

স্থির errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	পূর্ণাঙ্ক i;
	ৱর্ণ *out = buf;
	blk64_t offset = 0, cur_block;
	সংরচনা sparse_map *sm = channel->private_data;

	যদি (check_block_size(channel, sm))
		ঘূৰাই EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	যদি (গুনতি < 0) { //partial read
		গুনতি = -গুনতি;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		যদি (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, গুনতি);
		অন্যথা
			memset(out, 0, গুনতি);
	} অন্যথা {
		বাবে (i = 0; i < গুনতি; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			যদি (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			অন্যথা যদি (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	ঘূৰাই 0;
}

স্থির errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	ঘূৰাই sparse_read_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf)
{
	পূর্ণাঙ্ক i;
	blk64_t offset = 0, cur_block;
	ধ্রুৱ ৱর্ণ *in = buf;
	সংরচনা sparse_map *sm = channel->private_data;

	যদি (check_block_size(channel, sm))
		ঘূৰাই EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	যদি (গুনতি < 0) { //partial write
		গুনতি = -গুনতি;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		যদি (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			যদি (!sm->blocks[cur_block])
				ঘূৰাই EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, গুনতি);
	} অন্যথা {
		বাবে (i = 0; i < গুনতি; ++i) {
			যদি (block + i >= sm->blocks_গুনতি)
				ঘূৰাই 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			যদি (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				যদি (!sm->blocks[cur_block])
					ঘূৰাই EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	ঘূৰাই 0;
}

স্থির errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf)
{
	ঘূৰাই sparse_write_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long গুনতি)
{
	blk64_t cur_block, offset;
	সংরচনা sparse_map *sm = channel->private_data;

	যদি (check_block_size(channel, sm))
		ঘূৰাই EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	বাবে (unsigned long long i = 0; i < গুনতি; ++i) {
		যদি (blk + i >= sm->blocks_গুনতি)
			ঘূৰাই 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		যদি (!sm->blocks[cur_block])
			অনুৱর্ত;
		মুক্ত(sm->blocks[cur_block]);
		sm->blocks[cur_block] = রিক্ত;
	}
	ঘূৰাই 0;
}

স্থির errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long গুনতি)
{
	ঘূৰাই sparse_discard(channel, blk, গুনতি);
}

স্থির errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	ঘূৰাই 0;
}

স্থির errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   ধ্রুৱ ৱর্ণ *option __attribute__((unused)),
                                   ধ্রুৱ ৱর্ণ *arg __attribute__((unused)))
{
	ঘূৰাই 0;
}

স্থির errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long গুনতি __attribute__((unused)))
{
	ঘূৰাই 0;
}

স্থির সংরচনা struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= রিক্ত,
	.set_option		= sparse_set_option,
	.get_stats		= রিক্ত,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

স্থির সংরচনা struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= রিক্ত,
	.set_option		= sparse_set_option,
	.get_stats		= রিক্ত,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
