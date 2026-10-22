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

#जर !defined(ENABLE_LIBSPARSE)
स्थिर errcode_t sparse_open(स्थिरांक अक्षर *name EXT2FS_ATTR((unused)),
			     पूर्णांक flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	परत EXT2_ET_UNIMPLEMENTED;
}
स्थिर errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	परत EXT2_ET_UNIMPLEMENTED;
}
स्थिर संरचना struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.खोलो			= sparse_open,
	.बंद			= sparse_close,
};
स्थिर संरचना struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.खोलो			= sparse_open,
	.बंद			= sparse_close,
};
#अन्यथा
#अन्तर्भाव <sparse/sparse.h>

संरचना sparse_map {
	पूर्णांक			fd;
	अक्षर			**blocks;
	पूर्णांक			block_size;
	uint64_t		blocks_मोजणी;
	अक्षर			*file;
	संरचना sparse_file	*sparse_file;
	io_channel		channel;
};

संरचना sparse_io_params {
	पूर्णांक			fd;
	अक्षर			*file;
	uint64_t		blocks_मोजणी;
	अचिन्हित पूर्णांक		block_size;
};

स्थिर errcode_t sparse_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				  पूर्णांक मोजणी, स्थिरांक शून्य *buf);

स्थिर शून्य free_sparse_blocks(संरचना sparse_map *sm)
{
	uint64_t i;

	साठी (i = 0; i < sm->blocks_मोजणी; ++i)
		मुक्त(sm->blocks[i]);
	मुक्त(sm->blocks);
	sm->blocks = रिक्त;
}

स्थिर पूर्णांक sparse_import_segment(शून्य *priv, स्थिरांक शून्य *data, माप_प्रकार len,
				 अचिन्हित पूर्णांक block, अचिन्हित पूर्णांक nr_blocks)
{
	संरचना sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	जर (!nr_blocks || len % sm->block_size)
		परत 0;

	परत sparse_write_blk(sm->channel, block, nr_blocks, data);
}

स्थिर errcode_t io_manager_import_sparse(संरचना sparse_io_params *params,
					  संरचना sparse_map *sm, io_channel io)
{
	पूर्णांक fd;
	errcode_t retval;
	संरचना sparse_file *sparse_file;

	जर (params->fd < 0) {
		fd = खोलो(params->file, O_RDONLY);
		जर (fd < 0) {
			retval = -1;
			जा err_open;
		}
	} अन्यथा
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	जर (!sparse_file) {
		retval = -1;
		जा err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_मोजणी = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = सुस्मृति(sm->blocks_मोजणी, आकार(अक्षर*));
	जर (!sm->blocks) {
		retval = -1;
		जा err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	जर (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	बंद(fd);
err_open:
	परत retval;
}

स्थिर errcode_t io_manager_configure(संरचना sparse_io_params *params,
				      पूर्णांक flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	संरचना sparse_map *sm = सुस्मृति(1, आकार(*sm));
	जर (!sm)
		परत EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	जर (retval) {
		जर (!params->block_size || !params->blocks_मोजणी) {
			retval = EINVAL;
			जा err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_मोजणी = params->blocks_मोजणी;
		sm->blocks = सुस्मृति(params->blocks_मोजणी, आकार(शून्य*));
		जर (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			जा err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_मोजणी;

	जर (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		जर (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			जा err_alloc;
		}
		जर (params->fd < 0) {
			sm->fd = खोलो(params->file, O_CREAT | O_RDWR | O_TRUNC | O_द्विमान,
				      0644);
			जर (sm->fd < 0) {
				retval = त्रुटि_सं;
				जा err_open;
			}
		} अन्यथा
			sm->fd = params->fd;
	} अन्यथा {
		sm->fd = -1;
		sm->sparse_file = रिक्त;
	}
	परत 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	मुक्त(sm);
	परत retval;
}

स्थिर errcode_t sparse_open_channel(संरचना sparse_io_params *sparse_params,
				     पूर्णांक flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = सुस्मृति(1, आकार(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	जर (retval) {
		मुक्त(io);
		परत retval;
	}

	*channel = io;
	परत 0;
}

स्थिर errcode_t read_sparse_argv(स्थिरांक अक्षर *name, bool is_fd,
				  संरचना sparse_io_params *sparse_params)
{
	पूर्णांक ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_मोजणी = 0;

	sparse_params->file = दो_स्मृति(म_माप(name) + 1);
	जर (!sparse_params->file) {
		ख_लिखो(मानक_त्रुटि, "failed to alloc %zu\n", म_माप(name) + 1);
		परत EXT2_ET_NO_MEMORY;
	}

	जर (is_fd) {
		ret = माला_पूछो(name, "(%d):%llu:%u", &sparse_params->fd,
			     (अचिन्हित दीर्घ दीर्घ *)&sparse_params->blocks_मोजणी,
			     &sparse_params->block_size);
	} अन्यथा {
		ret = माला_पूछो(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (अचिन्हित दीर्घ दीर्घ *)&sparse_params->blocks_मोजणी,
			     &sparse_params->block_size);
	}

	जर (ret < 1) {
		मुक्त(sparse_params->file);
		परत EINVAL;
	}
	परत 0;
}

स्थिर errcode_t sparse_open(स्थिरांक अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	errcode_t retval;
	संरचना sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	जर (retval)
		परत EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	जर (retval)
		परत retval;
	(*channel)->manager = sparse_io_manager;

	परत retval;
}

स्थिर errcode_t sparsefd_open(स्थिरांक अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	errcode_t retval;
	संरचना sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	जर (retval)
		परत EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	जर (retval)
		परत retval;
	(*channel)->manager = sparsefd_io_manager;

	परत retval;
}

स्थिर errcode_t sparse_merge_blocks(संरचना sparse_map *sm, uint64_t start,
					uint64_t अंक)
{
	अक्षर *buf;
	uint64_t i;
	अचिन्हित पूर्णांक block_size = sm->block_size;
	errcode_t retval = 0;

	buf = सुस्मृति(अंक, block_size);
	जर (!buf) {
		ख_लिखो(मानक_त्रुटि, "failed to alloc %llu\n",
			(अचिन्हित दीर्घ दीर्घ)अंक * block_size);
		परत EXT2_ET_NO_MEMORY;
	}

	साठी (i = 0; i < अंक; i++) {
		स_नकल(buf + i * block_size, sm->blocks[start + i] , block_size);
		मुक्त(sm->blocks[start + i]);
		sm->blocks[start + i] = रिक्त;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * अंक, start);

	परत retval;
}

स्थिर errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	संरचना sparse_map *sm = channel->private_data;

	जर (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == रिक्त) ? -1 : 0;
		साठी (i = 0; i < sm->blocks_मोजणी; ++i) {
			जर (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} अन्यथा जर (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			जर (retval)
				जा ret;
		}
		जर (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_मोजणी - chunk_start);
			जर (retval)
				जा ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	जर (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	मुक्त(sm->file);
	मुक्त(sm);
	मुक्त(channel);
	परत retval;
}

स्थिर errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	संरचना sparse_map *sm = channel->private_data;
	पूर्णांक fd = sm->fd;

	retval = sparse_close_channel(channel);
	जर (fd >= 0)
		बंद(fd);

	परत retval;
}

स्थिर errcode_t sparse_set_blksize(io_channel channel, पूर्णांक blksize)
{
	channel->block_size = blksize;
	परत 0;
}

स्थिर blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, संरचना sparse_map *sm)
{
	पूर्णांक ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	परत ret;
}

स्थिर errcode_t check_block_size(io_channel channel, संरचना sparse_map *sm)
{
	जर (sm->block_size >= channel->block_size)
		परत 0;
	परत EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

स्थिर errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   पूर्णांक मोजणी, शून्य *buf)
{
	पूर्णांक i;
	अक्षर *out = buf;
	blk64_t offset = 0, cur_block;
	संरचना sparse_map *sm = channel->private_data;

	जर (check_block_size(channel, sm))
		परत EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	जर (मोजणी < 0) { //partial read
		मोजणी = -मोजणी;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		जर (sm->blocks[cur_block])
			स_नकल(out, (sm->blocks[cur_block]) + offset, मोजणी);
		अन्यथा
			स_रखो(out, 0, मोजणी);
	} अन्यथा {
		साठी (i = 0; i < मोजणी; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			जर (sm->blocks[cur_block])
				स_नकल(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			अन्यथा जर (sm->blocks)
				स_रखो(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	परत 0;
}

स्थिर errcode_t sparse_read_blk(io_channel channel, अचिन्हित दीर्घ block,
				 पूर्णांक मोजणी, शून्य *buf)
{
	परत sparse_read_blk64(channel, block, मोजणी, buf);
}

स्थिर errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	पूर्णांक i;
	blk64_t offset = 0, cur_block;
	स्थिरांक अक्षर *in = buf;
	संरचना sparse_map *sm = channel->private_data;

	जर (check_block_size(channel, sm))
		परत EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	जर (मोजणी < 0) { //partial write
		मोजणी = -मोजणी;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		जर (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = सुस्मृति(1, sm->block_size);
			जर (!sm->blocks[cur_block])
				परत EXT2_ET_NO_MEMORY;
		}
		स_नकल(sm->blocks[cur_block] + offset, in, मोजणी);
	} अन्यथा {
		साठी (i = 0; i < मोजणी; ++i) {
			जर (block + i >= sm->blocks_मोजणी)
				परत 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			जर (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					सुस्मृति(1, sm->block_size);
				जर (!sm->blocks[cur_block])
					परत EXT2_ET_NO_MEMORY;
			}
			स_नकल(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	परत 0;
}

स्थिर errcode_t sparse_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				  पूर्णांक मोजणी, स्थिरांक शून्य *buf)
{
	परत sparse_write_blk64(channel, block, मोजणी, buf);
}

स्थिर errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, अचिन्हित दीर्घ दीर्घ मोजणी)
{
	blk64_t cur_block, offset;
	संरचना sparse_map *sm = channel->private_data;

	जर (check_block_size(channel, sm))
		परत EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	साठी (अचिन्हित दीर्घ दीर्घ i = 0; i < मोजणी; ++i) {
		जर (blk + i >= sm->blocks_मोजणी)
			परत 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		जर (!sm->blocks[cur_block])
			सुरूठेवा;
		मुक्त(sm->blocks[cur_block]);
		sm->blocks[cur_block] = रिक्त;
	}
	परत 0;
}

स्थिर errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				अचिन्हित दीर्घ दीर्घ मोजणी)
{
	परत sparse_discard(channel, blk, मोजणी);
}

स्थिर errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	परत 0;
}

स्थिर errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   स्थिरांक अक्षर *option __attribute__((unused)),
                                   स्थिरांक अक्षर *arg __attribute__((unused)))
{
	परत 0;
}

स्थिर errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			अचिन्हित दीर्घ दीर्घ मोजणी __attribute__((unused)))
{
	परत 0;
}

स्थिर संरचना struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.खोलो			= sparse_open,
	.बंद			= sparse_close,
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
	.खोलो			= sparsefd_open,
	.बंद			= sparse_close,
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
