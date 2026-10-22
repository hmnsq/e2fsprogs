#अन्तर्भाव "config.h"
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#ifndef O_द्विआधारी
#define O_द्विआधारी 0
#endif

#यदि !defined(ENABLE_LIBSPARSE)
स्थिर errcode_t sparse_open(अचर वर्ण *name EXT2FS_ATTR((unused)),
			     पूर्णाङ्क flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	फर्काऊ EXT2_ET_UNIMPLEMENTED;
}
स्थिर errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	फर्काऊ EXT2_ET_UNIMPLEMENTED;
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
	पूर्णाङ्क			fd;
	वर्ण			**blocks;
	पूर्णाङ्क			block_size;
	uint64_t		blocks_गन्ती;
	वर्ण			*file;
	संरचना sparse_file	*sparse_file;
	io_channel		channel;
};

संरचना sparse_io_params {
	पूर्णाङ्क			fd;
	वर्ण			*file;
	uint64_t		blocks_गन्ती;
	अचिन्हित पूर्णाङ्क		block_size;
};

स्थिर errcode_t sparse_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				  पूर्णाङ्क गन्ती, अचर शून्य *buf);

स्थिर शून्य free_sparse_blocks(संरचना sparse_map *sm)
{
	uint64_t i;

	लागि (i = 0; i < sm->blocks_गन्ती; ++i)
		मुक्त(sm->blocks[i]);
	मुक्त(sm->blocks);
	sm->blocks = रिक्त;
}

स्थिर पूर्णाङ्क sparse_import_segment(शून्य *priv, अचर शून्य *data, माप_प्रकार len,
				 अचिन्हित पूर्णाङ्क block, अचिन्हित पूर्णाङ्क nr_blocks)
{
	संरचना sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	यदि (!nr_blocks || len % sm->block_size)
		फर्काऊ 0;

	फर्काऊ sparse_write_blk(sm->channel, block, nr_blocks, data);
}

स्थिर errcode_t io_manager_import_sparse(संरचना sparse_io_params *params,
					  संरचना sparse_map *sm, io_channel io)
{
	पूर्णाङ्क fd;
	errcode_t retval;
	संरचना sparse_file *sparse_file;

	यदि (params->fd < 0) {
		fd = खोलो(params->file, O_RDONLY);
		यदि (fd < 0) {
			retval = -1;
			गच्छ err_open;
		}
	} अन्यथा
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	यदि (!sparse_file) {
		retval = -1;
		गच्छ err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_गन्ती = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = सुस्मृति(sm->blocks_गन्ती, परिमाण(वर्ण*));
	यदि (!sm->blocks) {
		retval = -1;
		गच्छ err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	यदि (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	बंद(fd);
err_open:
	फर्काऊ retval;
}

स्थिर errcode_t io_manager_configure(संरचना sparse_io_params *params,
				      पूर्णाङ्क flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	संरचना sparse_map *sm = सुस्मृति(1, परिमाण(*sm));
	यदि (!sm)
		फर्काऊ EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	यदि (retval) {
		यदि (!params->block_size || !params->blocks_गन्ती) {
			retval = EINVAL;
			गच्छ err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_गन्ती = params->blocks_गन्ती;
		sm->blocks = सुस्मृति(params->blocks_गन्ती, परिमाण(शून्य*));
		यदि (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			गच्छ err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_गन्ती;

	यदि (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		यदि (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			गच्छ err_alloc;
		}
		यदि (params->fd < 0) {
			sm->fd = खोलो(params->file, O_CREAT | O_RDWR | O_TRUNC | O_द्विआधारी,
				      0644);
			यदि (sm->fd < 0) {
				retval = त्रुटि_सं;
				गच्छ err_open;
			}
		} अन्यथा
			sm->fd = params->fd;
	} अन्यथा {
		sm->fd = -1;
		sm->sparse_file = रिक्त;
	}
	फर्काऊ 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	मुक्त(sm);
	फर्काऊ retval;
}

स्थिर errcode_t sparse_open_channel(संरचना sparse_io_params *sparse_params,
				     पूर्णाङ्क flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = सुस्मृति(1, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	यदि (retval) {
		मुक्त(io);
		फर्काऊ retval;
	}

	*channel = io;
	फर्काऊ 0;
}

स्थिर errcode_t read_sparse_argv(अचर वर्ण *name, bool is_fd,
				  संरचना sparse_io_params *sparse_params)
{
	पूर्णाङ्क ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_गन्ती = 0;

	sparse_params->file = दो_स्मृति(म_माप(name) + 1);
	यदि (!sparse_params->file) {
		ख_लिखो(मानक_त्रुटि, "failed to alloc %zu\n", म_माप(name) + 1);
		फर्काऊ EXT2_ET_NO_MEMORY;
	}

	यदि (is_fd) {
		ret = माला_पूछो(name, "(%d):%llu:%u", &sparse_params->fd,
			     (अचिन्हित दीर्घ दीर्घ *)&sparse_params->blocks_गन्ती,
			     &sparse_params->block_size);
	} अन्यथा {
		ret = माला_पूछो(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (अचिन्हित दीर्घ दीर्घ *)&sparse_params->blocks_गन्ती,
			     &sparse_params->block_size);
	}

	यदि (ret < 1) {
		मुक्त(sparse_params->file);
		फर्काऊ EINVAL;
	}
	फर्काऊ 0;
}

स्थिर errcode_t sparse_open(अचर वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	errcode_t retval;
	संरचना sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	यदि (retval)
		फर्काऊ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	यदि (retval)
		फर्काऊ retval;
	(*channel)->manager = sparse_io_manager;

	फर्काऊ retval;
}

स्थिर errcode_t sparsefd_open(अचर वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	errcode_t retval;
	संरचना sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	यदि (retval)
		फर्काऊ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	यदि (retval)
		फर्काऊ retval;
	(*channel)->manager = sparsefd_io_manager;

	फर्काऊ retval;
}

स्थिर errcode_t sparse_merge_blocks(संरचना sparse_map *sm, uint64_t start,
					uint64_t अङ्क)
{
	वर्ण *buf;
	uint64_t i;
	अचिन्हित पूर्णाङ्क block_size = sm->block_size;
	errcode_t retval = 0;

	buf = सुस्मृति(अङ्क, block_size);
	यदि (!buf) {
		ख_लिखो(मानक_त्रुटि, "failed to alloc %llu\n",
			(अचिन्हित दीर्घ दीर्घ)अङ्क * block_size);
		फर्काऊ EXT2_ET_NO_MEMORY;
	}

	लागि (i = 0; i < अङ्क; i++) {
		स_नकल(buf + i * block_size, sm->blocks[start + i] , block_size);
		मुक्त(sm->blocks[start + i]);
		sm->blocks[start + i] = रिक्त;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * अङ्क, start);

	फर्काऊ retval;
}

स्थिर errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	संरचना sparse_map *sm = channel->private_data;

	यदि (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == रिक्त) ? -1 : 0;
		लागि (i = 0; i < sm->blocks_गन्ती; ++i) {
			यदि (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} अन्यथा यदि (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			यदि (retval)
				गच्छ ret;
		}
		यदि (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_गन्ती - chunk_start);
			यदि (retval)
				गच्छ ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	यदि (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	मुक्त(sm->file);
	मुक्त(sm);
	मुक्त(channel);
	फर्काऊ retval;
}

स्थिर errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	संरचना sparse_map *sm = channel->private_data;
	पूर्णाङ्क fd = sm->fd;

	retval = sparse_close_channel(channel);
	यदि (fd >= 0)
		बंद(fd);

	फर्काऊ retval;
}

स्थिर errcode_t sparse_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	channel->block_size = blksize;
	फर्काऊ 0;
}

स्थिर blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, संरचना sparse_map *sm)
{
	पूर्णाङ्क ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	फर्काऊ ret;
}

स्थिर errcode_t check_block_size(io_channel channel, संरचना sparse_map *sm)
{
	यदि (sm->block_size >= channel->block_size)
		फर्काऊ 0;
	फर्काऊ EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

स्थिर errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   पूर्णाङ्क गन्ती, शून्य *buf)
{
	पूर्णाङ्क i;
	वर्ण *out = buf;
	blk64_t offset = 0, cur_block;
	संरचना sparse_map *sm = channel->private_data;

	यदि (check_block_size(channel, sm))
		फर्काऊ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	यदि (गन्ती < 0) { //partial read
		गन्ती = -गन्ती;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		यदि (sm->blocks[cur_block])
			स_नकल(out, (sm->blocks[cur_block]) + offset, गन्ती);
		अन्यथा
			स_रखो(out, 0, गन्ती);
	} अन्यथा {
		लागि (i = 0; i < गन्ती; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			यदि (sm->blocks[cur_block])
				स_नकल(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			अन्यथा यदि (sm->blocks)
				स_रखो(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	फर्काऊ 0;
}

स्थिर errcode_t sparse_read_blk(io_channel channel, अचिन्हित दीर्घ block,
				 पूर्णाङ्क गन्ती, शून्य *buf)
{
	फर्काऊ sparse_read_blk64(channel, block, गन्ती, buf);
}

स्थिर errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    पूर्णाङ्क गन्ती, अचर शून्य *buf)
{
	पूर्णाङ्क i;
	blk64_t offset = 0, cur_block;
	अचर वर्ण *in = buf;
	संरचना sparse_map *sm = channel->private_data;

	यदि (check_block_size(channel, sm))
		फर्काऊ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	यदि (गन्ती < 0) { //partial write
		गन्ती = -गन्ती;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		यदि (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = सुस्मृति(1, sm->block_size);
			यदि (!sm->blocks[cur_block])
				फर्काऊ EXT2_ET_NO_MEMORY;
		}
		स_नकल(sm->blocks[cur_block] + offset, in, गन्ती);
	} अन्यथा {
		लागि (i = 0; i < गन्ती; ++i) {
			यदि (block + i >= sm->blocks_गन्ती)
				फर्काऊ 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			यदि (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					सुस्मृति(1, sm->block_size);
				यदि (!sm->blocks[cur_block])
					फर्काऊ EXT2_ET_NO_MEMORY;
			}
			स_नकल(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	फर्काऊ 0;
}

स्थिर errcode_t sparse_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				  पूर्णाङ्क गन्ती, अचर शून्य *buf)
{
	फर्काऊ sparse_write_blk64(channel, block, गन्ती, buf);
}

स्थिर errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, अचिन्हित दीर्घ दीर्घ गन्ती)
{
	blk64_t cur_block, offset;
	संरचना sparse_map *sm = channel->private_data;

	यदि (check_block_size(channel, sm))
		फर्काऊ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	लागि (अचिन्हित दीर्घ दीर्घ i = 0; i < गन्ती; ++i) {
		यदि (blk + i >= sm->blocks_गन्ती)
			फर्काऊ 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		यदि (!sm->blocks[cur_block])
			जारी;
		मुक्त(sm->blocks[cur_block]);
		sm->blocks[cur_block] = रिक्त;
	}
	फर्काऊ 0;
}

स्थिर errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				अचिन्हित दीर्घ दीर्घ गन्ती)
{
	फर्काऊ sparse_discard(channel, blk, गन्ती);
}

स्थिर errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	फर्काऊ 0;
}

स्थिर errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   अचर वर्ण *option __attribute__((unused)),
                                   अचर वर्ण *arg __attribute__((unused)))
{
	फर्काऊ 0;
}

स्थिर errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			अचिन्हित दीर्घ दीर्घ गन्ती __attribute__((unused)))
{
	फर्काऊ 0;
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
