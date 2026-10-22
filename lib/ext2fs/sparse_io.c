#অন্তর্ভাব "config.h"
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

#ifndef O_দ্বিমিক
#define O_দ্বিমিক 0
#endif

#যদি !defined(ENABLE_LIBSPARSE)
স্থির errcode_t sparse_open(ধ্রুবক অক্ষর *name EXT2FS_ATTR((unused)),
			     পূর্ণসংখ্যা flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	ফেরত EXT2_ET_UNIMPLEMENTED;
}
স্থির errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	ফেরত EXT2_ET_UNIMPLEMENTED;
}
স্থির কাঠামো struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
স্থির কাঠামো struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#অন্যথা
#অন্তর্ভাব <sparse/sparse.h>

কাঠামো sparse_map {
	পূর্ণসংখ্যা			fd;
	অক্ষর			**blocks;
	পূর্ণসংখ্যা			block_আকার;
	uint64_t		blocks_গুনতি;
	অক্ষর			*file;
	কাঠামো sparse_file	*sparse_file;
	io_channel		channel;
};

কাঠামো sparse_io_params {
	পূর্ণসংখ্যা			fd;
	অক্ষর			*file;
	uint64_t		blocks_গুনতি;
	unsigned পূর্ণসংখ্যা		block_আকার;
};

স্থির errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf);

স্থির শূন্য free_sparse_blocks(কাঠামো sparse_map *sm)
{
	uint64_t i;

	জন্য (i = 0; i < sm->blocks_গুনতি; ++i)
		মুক্ত(sm->blocks[i]);
	মুক্ত(sm->blocks);
	sm->blocks = ফাঁকা;
}

স্থির পূর্ণসংখ্যা sparse_import_segment(শূন্য *priv, ধ্রুবক শূন্য *তথ্য, size_t len,
				 unsigned পূর্ণসংখ্যা block, unsigned পূর্ণসংখ্যা nr_blocks)
{
	কাঠামো sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	যদি (!nr_blocks || len % sm->block_আকার)
		ফেরত 0;

	ফেরত sparse_write_blk(sm->channel, block, nr_blocks, তথ্য);
}

স্থির errcode_t io_manager_import_sparse(কাঠামো sparse_io_params *params,
					  কাঠামো sparse_map *sm, io_channel io)
{
	পূর্ণসংখ্যা fd;
	errcode_t retval;
	কাঠামো sparse_file *sparse_file;

	যদি (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		যদি (fd < 0) {
			retval = -1;
			যাও err_open;
		}
	} অন্যথা
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	যদি (!sparse_file) {
		retval = -1;
		যাও err_sparse;
	}

	sm->block_আকার = sparse_file_block_আকার(sparse_file);
	sm->blocks_গুনতি = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_আকার + 1;
	sm->blocks = calloc(sm->blocks_গুনতি, আকার(অক্ষর*));
	যদি (!sm->blocks) {
		retval = -1;
		যাও err_alloc;
	}
	io->block_আকার = sm->block_আকার;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	যদি (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	ফেরত retval;
}

স্থির errcode_t io_manager_configure(কাঠামো sparse_io_params *params,
				      পূর্ণসংখ্যা flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_আকার;
	কাঠামো sparse_map *sm = calloc(1, আকার(*sm));
	যদি (!sm)
		ফেরত EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_তথ্য = sm;
	retval = io_manager_import_sparse(params, sm, io);
	যদি (retval) {
		যদি (!params->block_আকার || !params->blocks_গুনতি) {
			retval = EINVAL;
			যাও err_params;
		}
		sm->block_আকার = params->block_আকার;
		sm->blocks_গুনতি = params->blocks_গুনতি;
		sm->blocks = calloc(params->blocks_গুনতি, আকার(শূন্য*));
		যদি (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			যাও err_alloc;
		}
	}
	io->block_আকার = sm->block_আকার;
	img_আকার = (uint64_t)sm->block_আকার * sm->blocks_গুনতি;

	যদি (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_আকার, img_আকার);
		যদি (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			যাও err_alloc;
		}
		যদি (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_দ্বিমিক,
				      0644);
			যদি (sm->fd < 0) {
				retval = errno;
				যাও err_open;
			}
		} অন্যথা
			sm->fd = params->fd;
	} অন্যথা {
		sm->fd = -1;
		sm->sparse_file = ফাঁকা;
	}
	ফেরত 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	মুক্ত(sm);
	ফেরত retval;
}

স্থির errcode_t sparse_open_channel(কাঠামো sparse_io_params *sparse_params,
				     পূর্ণসংখ্যা flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, আকার(কাঠামো struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_আকার = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	যদি (retval) {
		মুক্ত(io);
		ফেরত retval;
	}

	*channel = io;
	ফেরত 0;
}

স্থির errcode_t read_sparse_argv(ধ্রুবক অক্ষর *name, bool is_fd,
				  কাঠামো sparse_io_params *sparse_params)
{
	পূর্ণসংখ্যা ret;
	sparse_params->fd = -1;
	sparse_params->block_আকার = 0;
	sparse_params->blocks_গুনতি = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	যদি (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		ফেরত EXT2_ET_NO_MEMORY;
	}

	যদি (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_গুনতি,
			     &sparse_params->block_আকার);
	} অন্যথা {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_গুনতি,
			     &sparse_params->block_আকার);
	}

	যদি (ret < 1) {
		মুক্ত(sparse_params->file);
		ফেরত EINVAL;
	}
	ফেরত 0;
}

স্থির errcode_t sparse_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, io_channel *channel)
{
	errcode_t retval;
	কাঠামো sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	যদি (retval)
		ফেরত EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	যদি (retval)
		ফেরত retval;
	(*channel)->manager = sparse_io_manager;

	ফেরত retval;
}

স্থির errcode_t sparsefd_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, io_channel *channel)
{
	errcode_t retval;
	কাঠামো sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	যদি (retval)
		ফেরত EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	যদি (retval)
		ফেরত retval;
	(*channel)->manager = sparsefd_io_manager;

	ফেরত retval;
}

স্থির errcode_t sparse_merge_blocks(কাঠামো sparse_map *sm, uint64_t start,
					uint64_t অঙ্ক)
{
	অক্ষর *buf;
	uint64_t i;
	unsigned পূর্ণসংখ্যা block_আকার = sm->block_আকার;
	errcode_t retval = 0;

	buf = calloc(অঙ্ক, block_আকার);
	যদি (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)অঙ্ক * block_আকার);
		ফেরত EXT2_ET_NO_MEMORY;
	}

	জন্য (i = 0; i < অঙ্ক; i++) {
		memcpy(buf + i * block_আকার, sm->blocks[start + i] , block_আকার);
		মুক্ত(sm->blocks[start + i]);
		sm->blocks[start + i] = ফাঁকা;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_তথ্য(sm->sparse_file, sm->blocks[start],
					block_আকার * অঙ্ক, start);

	ফেরত retval;
}

স্থির errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	কাঠামো sparse_map *sm = channel->private_তথ্য;

	যদি (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == ফাঁকা) ? -1 : 0;
		জন্য (i = 0; i < sm->blocks_গুনতি; ++i) {
			যদি (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} অন্যথা যদি (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			যদি (retval)
				যাও ret;
		}
		যদি (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_গুনতি - chunk_start);
			যদি (retval)
				যাও ret;
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
	ফেরত retval;
}

স্থির errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	কাঠামো sparse_map *sm = channel->private_তথ্য;
	পূর্ণসংখ্যা fd = sm->fd;

	retval = sparse_close_channel(channel);
	যদি (fd >= 0)
		close(fd);

	ফেরত retval;
}

স্থির errcode_t sparse_set_blksize(io_channel channel, পূর্ণসংখ্যা blksize)
{
	channel->block_আকার = blksize;
	ফেরত 0;
}

স্থির blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, কাঠামো sparse_map *sm)
{
	পূর্ণসংখ্যা ratio;
	blk64_t ret = block;

	ratio = sm->block_আকার / channel->block_আকার;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_আকার;

	ফেরত ret;
}

স্থির errcode_t check_block_আকার(io_channel channel, কাঠামো sparse_map *sm)
{
	যদি (sm->block_আকার >= channel->block_আকার)
		ফেরত 0;
	ফেরত EXT2_ET_UNEXPECTED_BLOCK_আকার;
}

স্থির errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	পূর্ণসংখ্যা i;
	অক্ষর *out = buf;
	blk64_t offset = 0, cur_block;
	কাঠামো sparse_map *sm = channel->private_তথ্য;

	যদি (check_block_আকার(channel, sm))
		ফেরত EXT2_ET_UNEXPECTED_BLOCK_আকার;

	যদি (গুনতি < 0) { //partial read
		গুনতি = -গুনতি;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		যদি (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, গুনতি);
		অন্যথা
			memset(out, 0, গুনতি);
	} অন্যথা {
		জন্য (i = 0; i < গুনতি; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			যদি (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_আকার),
				       sm->blocks[cur_block] + offset,
				       channel->block_আকার);
			অন্যথা যদি (sm->blocks)
				memset(out + (i * channel->block_আকার), 0,
				       channel->block_আকার);
		}
	}
	ফেরত 0;
}

স্থির errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	ফেরত sparse_read_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	পূর্ণসংখ্যা i;
	blk64_t offset = 0, cur_block;
	ধ্রুবক অক্ষর *in = buf;
	কাঠামো sparse_map *sm = channel->private_তথ্য;

	যদি (check_block_আকার(channel, sm))
		ফেরত EXT2_ET_UNEXPECTED_BLOCK_আকার;

	যদি (গুনতি < 0) { //partial write
		গুনতি = -গুনতি;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		যদি (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_আকার);
			যদি (!sm->blocks[cur_block])
				ফেরত EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, গুনতি);
	} অন্যথা {
		জন্য (i = 0; i < গুনতি; ++i) {
			যদি (block + i >= sm->blocks_গুনতি)
				ফেরত 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			যদি (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_আকার);
				যদি (!sm->blocks[cur_block])
					ফেরত EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_আকার),
			       channel->block_আকার);
		}
	}
	ফেরত 0;
}

স্থির errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	ফেরত sparse_write_blk64(channel, block, গুনতি, buf);
}

স্থির errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long গুনতি)
{
	blk64_t cur_block, offset;
	কাঠামো sparse_map *sm = channel->private_তথ্য;

	যদি (check_block_আকার(channel, sm))
		ফেরত EXT2_ET_UNEXPECTED_BLOCK_আকার;

	জন্য (unsigned long long i = 0; i < গুনতি; ++i) {
		যদি (blk + i >= sm->blocks_গুনতি)
			ফেরত 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		যদি (!sm->blocks[cur_block])
			চালু;
		মুক্ত(sm->blocks[cur_block]);
		sm->blocks[cur_block] = ফাঁকা;
	}
	ফেরত 0;
}

স্থির errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long গুনতি)
{
	ফেরত sparse_discard(channel, blk, গুনতি);
}

স্থির errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	ফেরত 0;
}

স্থির errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   ধ্রুবক অক্ষর *option __attribute__((unused)),
                                   ধ্রুবক অক্ষর *arg __attribute__((unused)))
{
	ফেরত 0;
}

স্থির errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long গুনতি __attribute__((unused)))
{
	ফেরত 0;
}

স্থির কাঠামো struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ফাঁকা,
	.set_option		= sparse_set_option,
	.get_stats		= ফাঁকা,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

স্থির কাঠামো struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ফাঁকা,
	.set_option		= sparse_set_option,
	.get_stats		= ফাঁকা,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
