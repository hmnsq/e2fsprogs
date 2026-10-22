#अन्तर्भाव "config.h"
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#ifndef O_द्विमान
#define O_द्विमान 0
#endif

#सचे !defined(ENABLE_LIBSPARSE)
स्थिर errcode_t sparse_open(ध्रुव वर्ण *name EXT2FS_ATTR((unused)),
			     पूर्णाङ्क flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	पटिनिवत्त EXT2_ET_UNIMPLEMENTED;
}
स्थिर errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	पटिनिवत्त EXT2_ET_UNIMPLEMENTED;
}
स्थिर संरचना struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
स्थिर संरचना struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#अञ्ञथा
#अन्तर्भाव <sparse/sparse.h>

संरचना sparse_map {
	पूर्णाङ्क			fd;
	वर्ण			**blocks;
	पूर्णाङ्क			block_size;
	uint64_t		blocks_गणनम्;
	वर्ण			*file;
	संरचना sparse_file	*sparse_file;
	io_channel		channel;
};

संरचना sparse_io_params {
	पूर्णाङ्क			fd;
	वर्ण			*file;
	uint64_t		blocks_गणनम्;
	unsigned पूर्णाङ्क		block_size;
};

स्थिर errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  पूर्णाङ्क गणनम्, ध्रुव शून्य *buf);

स्थिर शून्य free_sparse_blocks(संरचना sparse_map *sm)
{
	uint64_t i;

	अत्थाय (i = 0; i < sm->blocks_गणनम्; ++i)
		मुक्त(sm->blocks[i]);
	मुक्त(sm->blocks);
	sm->blocks = रिक्त;
}

स्थिर पूर्णाङ्क sparse_import_segment(शून्य *priv, ध्रुव शून्य *data, size_t len,
				 unsigned पूर्णाङ्क block, unsigned पूर्णाङ्क nr_blocks)
{
	संरचना sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	सचे (!nr_blocks || len % sm->block_size)
		पटिनिवत्त 0;

	पटिनिवत्त sparse_write_blk(sm->channel, block, nr_blocks, data);
}

स्थिर errcode_t io_manager_import_sparse(संरचना sparse_io_params *params,
					  संरचना sparse_map *sm, io_channel io)
{
	पूर्णाङ्क fd;
	errcode_t retval;
	संरचना sparse_file *sparse_file;

	सचे (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		सचे (fd < 0) {
			retval = -1;
			गच्छ err_open;
		}
	} अञ्ञथा
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	सचे (!sparse_file) {
		retval = -1;
		गच्छ err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_गणनम् = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_गणनम्, परिमाण(वर्ण*));
	सचे (!sm->blocks) {
		retval = -1;
		गच्छ err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	सचे (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	पटिनिवत्त retval;
}

स्थिर errcode_t io_manager_configure(संरचना sparse_io_params *params,
				      पूर्णाङ्क flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	संरचना sparse_map *sm = calloc(1, परिमाण(*sm));
	सचे (!sm)
		पटिनिवत्त EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	सचे (retval) {
		सचे (!params->block_size || !params->blocks_गणनम्) {
			retval = EINVAL;
			गच्छ err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_गणनम् = params->blocks_गणनम्;
		sm->blocks = calloc(params->blocks_गणनम्, परिमाण(शून्य*));
		सचे (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			गच्छ err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_गणनम्;

	सचे (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		सचे (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			गच्छ err_alloc;
		}
		सचे (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_द्विमान,
				      0644);
			सचे (sm->fd < 0) {
				retval = errno;
				गच्छ err_open;
			}
		} अञ्ञथा
			sm->fd = params->fd;
	} अञ्ञथा {
		sm->fd = -1;
		sm->sparse_file = रिक्त;
	}
	पटिनिवत्त 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	मुक्त(sm);
	पटिनिवत्त retval;
}

स्थिर errcode_t sparse_open_channel(संरचना sparse_io_params *sparse_params,
				     पूर्णाङ्क flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	सचे (retval) {
		मुक्त(io);
		पटिनिवत्त retval;
	}

	*channel = io;
	पटिनिवत्त 0;
}

स्थिर errcode_t read_sparse_argv(ध्रुव वर्ण *name, bool is_fd,
				  संरचना sparse_io_params *sparse_params)
{
	पूर्णाङ्क ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_गणनम् = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	सचे (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		पटिनिवत्त EXT2_ET_NO_MEMORY;
	}

	सचे (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_गणनम्,
			     &sparse_params->block_size);
	} अञ्ञथा {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_गणनम्,
			     &sparse_params->block_size);
	}

	सचे (ret < 1) {
		मुक्त(sparse_params->file);
		पटिनिवत्त EINVAL;
	}
	पटिनिवत्त 0;
}

स्थिर errcode_t sparse_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	errcode_t retval;
	संरचना sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	सचे (retval)
		पटिनिवत्त EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	सचे (retval)
		पटिनिवत्त retval;
	(*channel)->manager = sparse_io_manager;

	पटिनिवत्त retval;
}

स्थिर errcode_t sparsefd_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	errcode_t retval;
	संरचना sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	सचे (retval)
		पटिनिवत्त EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	सचे (retval)
		पटिनिवत्त retval;
	(*channel)->manager = sparsefd_io_manager;

	पटिनिवत्त retval;
}

स्थिर errcode_t sparse_merge_blocks(संरचना sparse_map *sm, uint64_t start,
					uint64_t अङ्कः)
{
	वर्ण *buf;
	uint64_t i;
	unsigned पूर्णाङ्क block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(अङ्कः, block_size);
	सचे (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)अङ्कः * block_size);
		पटिनिवत्त EXT2_ET_NO_MEMORY;
	}

	अत्थाय (i = 0; i < अङ्कः; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		मुक्त(sm->blocks[start + i]);
		sm->blocks[start + i] = रिक्त;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * अङ्कः, start);

	पटिनिवत्त retval;
}

स्थिर errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	संरचना sparse_map *sm = channel->private_data;

	सचे (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == रिक्त) ? -1 : 0;
		अत्थाय (i = 0; i < sm->blocks_गणनम्; ++i) {
			सचे (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} अञ्ञथा सचे (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			सचे (retval)
				गच्छ ret;
		}
		सचे (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_गणनम् - chunk_start);
			सचे (retval)
				गच्छ ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	सचे (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	मुक्त(sm->file);
	मुक्त(sm);
	मुक्त(channel);
	पटिनिवत्त retval;
}

स्थिर errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	संरचना sparse_map *sm = channel->private_data;
	पूर्णाङ्क fd = sm->fd;

	retval = sparse_close_channel(channel);
	सचे (fd >= 0)
		close(fd);

	पटिनिवत्त retval;
}

स्थिर errcode_t sparse_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	channel->block_size = blksize;
	पटिनिवत्त 0;
}

स्थिर blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, संरचना sparse_map *sm)
{
	पूर्णाङ्क ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	पटिनिवत्त ret;
}

स्थिर errcode_t check_block_size(io_channel channel, संरचना sparse_map *sm)
{
	सचे (sm->block_size >= channel->block_size)
		पटिनिवत्त 0;
	पटिनिवत्त EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

स्थिर errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   पूर्णाङ्क गणनम्, शून्य *buf)
{
	पूर्णाङ्क i;
	वर्ण *out = buf;
	blk64_t offset = 0, cur_block;
	संरचना sparse_map *sm = channel->private_data;

	सचे (check_block_size(channel, sm))
		पटिनिवत्त EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	सचे (गणनम् < 0) { //partial read
		गणनम् = -गणनम्;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		सचे (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, गणनम्);
		अञ्ञथा
			memset(out, 0, गणनम्);
	} अञ्ञथा {
		अत्थाय (i = 0; i < गणनम्; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			सचे (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			अञ्ञथा सचे (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	पटिनिवत्त 0;
}

स्थिर errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 पूर्णाङ्क गणनम्, शून्य *buf)
{
	पटिनिवत्त sparse_read_blk64(channel, block, गणनम्, buf);
}

स्थिर errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	पूर्णाङ्क i;
	blk64_t offset = 0, cur_block;
	ध्रुव वर्ण *in = buf;
	संरचना sparse_map *sm = channel->private_data;

	सचे (check_block_size(channel, sm))
		पटिनिवत्त EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	सचे (गणनम् < 0) { //partial write
		गणनम् = -गणनम्;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		सचे (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			सचे (!sm->blocks[cur_block])
				पटिनिवत्त EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, गणनम्);
	} अञ्ञथा {
		अत्थाय (i = 0; i < गणनम्; ++i) {
			सचे (block + i >= sm->blocks_गणनम्)
				पटिनिवत्त 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			सचे (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				सचे (!sm->blocks[cur_block])
					पटिनिवत्त EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	पटिनिवत्त 0;
}

स्थिर errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	पटिनिवत्त sparse_write_blk64(channel, block, गणनम्, buf);
}

स्थिर errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long गणनम्)
{
	blk64_t cur_block, offset;
	संरचना sparse_map *sm = channel->private_data;

	सचे (check_block_size(channel, sm))
		पटिनिवत्त EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	अत्थाय (unsigned long long i = 0; i < गणनम्; ++i) {
		सचे (blk + i >= sm->blocks_गणनम्)
			पटिनिवत्त 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		सचे (!sm->blocks[cur_block])
			अनुवर्त;
		मुक्त(sm->blocks[cur_block]);
		sm->blocks[cur_block] = रिक्त;
	}
	पटिनिवत्त 0;
}

स्थिर errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long गणनम्)
{
	पटिनिवत्त sparse_discard(channel, blk, गणनम्);
}

स्थिर errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	पटिनिवत्त 0;
}

स्थिर errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   ध्रुव वर्ण *option __attribute__((unused)),
                                   ध्रुव वर्ण *arg __attribute__((unused)))
{
	पटिनिवत्त 0;
}

स्थिर errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long गणनम् __attribute__((unused)))
{
	पटिनिवत्त 0;
}

स्थिर संरचना struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= रिक्त,
	.set_option		= sparse_set_option,
	.get_stats		= रिक्त,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

स्थिर संरचना struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= रिक्त,
	.set_option		= sparse_set_option,
	.get_stats		= रिक्त,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
