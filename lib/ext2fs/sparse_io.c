#समावेश "config.h"
#समावेश <sys/types.h>
#समावेश <sys/stat.h>
#समावेश <fcntl.h>
#समावेश <unistd.h>
#समावेश <stdint.h>
#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

#अगर_अघोषित O_द्विआधारी
#घोषणा O_द्विआधारी 0
#पूर्ण_अगर

#अगर !defined(ENABLE_LIBSPARSE)
अटल errcode_t sparse_open(स्थिर अक्षर *name EXT2FS_ATTR((unused)),
			     पूर्णांक flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	वापस EXT2_ET_UNIMPLEMENTED;
}
अटल errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	वापस EXT2_ET_UNIMPLEMENTED;
}
अटल काष्ठा struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.खोलो			= sparse_open,
	.बंद			= sparse_close,
};
अटल काष्ठा struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.खोलो			= sparse_open,
	.बंद			= sparse_close,
};
#अन्यथा
#समावेश <sparse/sparse.h>

काष्ठा sparse_map {
	पूर्णांक			fd;
	अक्षर			**blocks;
	पूर्णांक			block_आकार;
	uint64_t		blocks_गिनती;
	अक्षर			*file;
	काष्ठा sparse_file	*sparse_file;
	io_channel		channel;
};

काष्ठा sparse_io_params {
	पूर्णांक			fd;
	अक्षर			*file;
	uint64_t		blocks_गिनती;
	अचिन्हित पूर्णांक		block_आकार;
};

अटल errcode_t sparse_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				  पूर्णांक गिनती, स्थिर व्योम *buf);

अटल व्योम free_sparse_blocks(काष्ठा sparse_map *sm)
{
	uint64_t i;

	क्रम (i = 0; i < sm->blocks_गिनती; ++i)
		मुक्त(sm->blocks[i]);
	मुक्त(sm->blocks);
	sm->blocks = शून्य;
}

अटल पूर्णांक sparse_import_segment(व्योम *priv, स्थिर व्योम *तथ्य, माप_प्रकार len,
				 अचिन्हित पूर्णांक block, अचिन्हित पूर्णांक nr_blocks)
{
	काष्ठा sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	अगर (!nr_blocks || len % sm->block_आकार)
		वापस 0;

	वापस sparse_write_blk(sm->channel, block, nr_blocks, तथ्य);
}

अटल errcode_t io_manager_import_sparse(काष्ठा sparse_io_params *params,
					  काष्ठा sparse_map *sm, io_channel io)
{
	पूर्णांक fd;
	errcode_t retval;
	काष्ठा sparse_file *sparse_file;

	अगर (params->fd < 0) {
		fd = खोलो(params->file, O_RDONLY);
		अगर (fd < 0) {
			retval = -1;
			जाओ err_open;
		}
	} अन्यथा
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	अगर (!sparse_file) {
		retval = -1;
		जाओ err_sparse;
	}

	sm->block_आकार = sparse_file_block_आकार(sparse_file);
	sm->blocks_गिनती = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_आकार + 1;
	sm->blocks = सुस्मृति(sm->blocks_गिनती, माप(अक्षर*));
	अगर (!sm->blocks) {
		retval = -1;
		जाओ err_alloc;
	}
	io->block_आकार = sm->block_आकार;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	अगर (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	बंद(fd);
err_open:
	वापस retval;
}

अटल errcode_t io_manager_configure(काष्ठा sparse_io_params *params,
				      पूर्णांक flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_आकार;
	काष्ठा sparse_map *sm = सुस्मृति(1, माप(*sm));
	अगर (!sm)
		वापस EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_तथ्य = sm;
	retval = io_manager_import_sparse(params, sm, io);
	अगर (retval) {
		अगर (!params->block_आकार || !params->blocks_गिनती) {
			retval = EINVAL;
			जाओ err_params;
		}
		sm->block_आकार = params->block_आकार;
		sm->blocks_गिनती = params->blocks_गिनती;
		sm->blocks = सुस्मृति(params->blocks_गिनती, माप(व्योम*));
		अगर (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			जाओ err_alloc;
		}
	}
	io->block_आकार = sm->block_आकार;
	img_आकार = (uint64_t)sm->block_आकार * sm->blocks_गिनती;

	अगर (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_आकार, img_आकार);
		अगर (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			जाओ err_alloc;
		}
		अगर (params->fd < 0) {
			sm->fd = खोलो(params->file, O_CREAT | O_RDWR | O_TRUNC | O_द्विआधारी,
				      0644);
			अगर (sm->fd < 0) {
				retval = त्रुटि_सं;
				जाओ err_open;
			}
		} अन्यथा
			sm->fd = params->fd;
	} अन्यथा {
		sm->fd = -1;
		sm->sparse_file = शून्य;
	}
	वापस 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	मुक्त(sm);
	वापस retval;
}

अटल errcode_t sparse_open_channel(काष्ठा sparse_io_params *sparse_params,
				     पूर्णांक flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = सुस्मृति(1, माप(काष्ठा struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_आकार = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	अगर (retval) {
		मुक्त(io);
		वापस retval;
	}

	*channel = io;
	वापस 0;
}

अटल errcode_t read_sparse_argv(स्थिर अक्षर *name, bool is_fd,
				  काष्ठा sparse_io_params *sparse_params)
{
	पूर्णांक ret;
	sparse_params->fd = -1;
	sparse_params->block_आकार = 0;
	sparse_params->blocks_गिनती = 0;

	sparse_params->file = दो_स्मृति(म_माप(name) + 1);
	अगर (!sparse_params->file) {
		ख_लिखो(मानक_त्रुटि, "failed to alloc %zu\n", म_माप(name) + 1);
		वापस EXT2_ET_NO_MEMORY;
	}

	अगर (is_fd) {
		ret = माला_पूछो(name, "(%d):%llu:%u", &sparse_params->fd,
			     (अचिन्हित दीर्घ दीर्घ *)&sparse_params->blocks_गिनती,
			     &sparse_params->block_आकार);
	} अन्यथा {
		ret = माला_पूछो(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (अचिन्हित दीर्घ दीर्घ *)&sparse_params->blocks_गिनती,
			     &sparse_params->block_आकार);
	}

	अगर (ret < 1) {
		मुक्त(sparse_params->file);
		वापस EINVAL;
	}
	वापस 0;
}

अटल errcode_t sparse_open(स्थिर अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	errcode_t retval;
	काष्ठा sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	अगर (retval)
		वापस EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	अगर (retval)
		वापस retval;
	(*channel)->manager = sparse_io_manager;

	वापस retval;
}

अटल errcode_t sparsefd_open(स्थिर अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	errcode_t retval;
	काष्ठा sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	अगर (retval)
		वापस EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	अगर (retval)
		वापस retval;
	(*channel)->manager = sparsefd_io_manager;

	वापस retval;
}

अटल errcode_t sparse_merge_blocks(काष्ठा sparse_map *sm, uint64_t start,
					uint64_t अंक)
{
	अक्षर *buf;
	uint64_t i;
	अचिन्हित पूर्णांक block_आकार = sm->block_आकार;
	errcode_t retval = 0;

	buf = सुस्मृति(अंक, block_आकार);
	अगर (!buf) {
		ख_लिखो(मानक_त्रुटि, "failed to alloc %llu\n",
			(अचिन्हित दीर्घ दीर्घ)अंक * block_आकार);
		वापस EXT2_ET_NO_MEMORY;
	}

	क्रम (i = 0; i < अंक; i++) {
		स_नकल(buf + i * block_आकार, sm->blocks[start + i] , block_आकार);
		मुक्त(sm->blocks[start + i]);
		sm->blocks[start + i] = शून्य;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_तथ्य(sm->sparse_file, sm->blocks[start],
					block_आकार * अंक, start);

	वापस retval;
}

अटल errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	काष्ठा sparse_map *sm = channel->private_तथ्य;

	अगर (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == शून्य) ? -1 : 0;
		क्रम (i = 0; i < sm->blocks_गिनती; ++i) {
			अगर (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} अन्यथा अगर (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			अगर (retval)
				जाओ ret;
		}
		अगर (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_गिनती - chunk_start);
			अगर (retval)
				जाओ ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	अगर (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	मुक्त(sm->file);
	मुक्त(sm);
	मुक्त(channel);
	वापस retval;
}

अटल errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	काष्ठा sparse_map *sm = channel->private_तथ्य;
	पूर्णांक fd = sm->fd;

	retval = sparse_close_channel(channel);
	अगर (fd >= 0)
		बंद(fd);

	वापस retval;
}

अटल errcode_t sparse_set_blksize(io_channel channel, पूर्णांक blksize)
{
	channel->block_आकार = blksize;
	वापस 0;
}

अटल blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, काष्ठा sparse_map *sm)
{
	पूर्णांक ratio;
	blk64_t ret = block;

	ratio = sm->block_आकार / channel->block_आकार;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_आकार;

	वापस ret;
}

अटल errcode_t check_block_आकार(io_channel channel, काष्ठा sparse_map *sm)
{
	अगर (sm->block_आकार >= channel->block_आकार)
		वापस 0;
	वापस EXT2_ET_UNEXPECTED_BLOCK_आकार;
}

अटल errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   पूर्णांक गिनती, व्योम *buf)
{
	पूर्णांक i;
	अक्षर *out = buf;
	blk64_t offset = 0, cur_block;
	काष्ठा sparse_map *sm = channel->private_तथ्य;

	अगर (check_block_आकार(channel, sm))
		वापस EXT2_ET_UNEXPECTED_BLOCK_आकार;

	अगर (गिनती < 0) { //partial read
		गिनती = -गिनती;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		अगर (sm->blocks[cur_block])
			स_नकल(out, (sm->blocks[cur_block]) + offset, गिनती);
		अन्यथा
			स_रखो(out, 0, गिनती);
	} अन्यथा {
		क्रम (i = 0; i < गिनती; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			अगर (sm->blocks[cur_block])
				स_नकल(out + (i * channel->block_आकार),
				       sm->blocks[cur_block] + offset,
				       channel->block_आकार);
			अन्यथा अगर (sm->blocks)
				स_रखो(out + (i * channel->block_आकार), 0,
				       channel->block_आकार);
		}
	}
	वापस 0;
}

अटल errcode_t sparse_read_blk(io_channel channel, अचिन्हित दीर्घ block,
				 पूर्णांक गिनती, व्योम *buf)
{
	वापस sparse_read_blk64(channel, block, गिनती, buf);
}

अटल errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    पूर्णांक गिनती, स्थिर व्योम *buf)
{
	पूर्णांक i;
	blk64_t offset = 0, cur_block;
	स्थिर अक्षर *in = buf;
	काष्ठा sparse_map *sm = channel->private_तथ्य;

	अगर (check_block_आकार(channel, sm))
		वापस EXT2_ET_UNEXPECTED_BLOCK_आकार;

	अगर (गिनती < 0) { //partial write
		गिनती = -गिनती;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		अगर (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = सुस्मृति(1, sm->block_आकार);
			अगर (!sm->blocks[cur_block])
				वापस EXT2_ET_NO_MEMORY;
		}
		स_नकल(sm->blocks[cur_block] + offset, in, गिनती);
	} अन्यथा {
		क्रम (i = 0; i < गिनती; ++i) {
			अगर (block + i >= sm->blocks_गिनती)
				वापस 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			अगर (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					सुस्मृति(1, sm->block_आकार);
				अगर (!sm->blocks[cur_block])
					वापस EXT2_ET_NO_MEMORY;
			}
			स_नकल(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_आकार),
			       channel->block_आकार);
		}
	}
	वापस 0;
}

अटल errcode_t sparse_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				  पूर्णांक गिनती, स्थिर व्योम *buf)
{
	वापस sparse_write_blk64(channel, block, गिनती, buf);
}

अटल errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, अचिन्हित दीर्घ दीर्घ गिनती)
{
	blk64_t cur_block, offset;
	काष्ठा sparse_map *sm = channel->private_तथ्य;

	अगर (check_block_आकार(channel, sm))
		वापस EXT2_ET_UNEXPECTED_BLOCK_आकार;

	क्रम (अचिन्हित दीर्घ दीर्घ i = 0; i < गिनती; ++i) {
		अगर (blk + i >= sm->blocks_गिनती)
			वापस 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		अगर (!sm->blocks[cur_block])
			जारी;
		मुक्त(sm->blocks[cur_block]);
		sm->blocks[cur_block] = शून्य;
	}
	वापस 0;
}

अटल errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				अचिन्हित दीर्घ दीर्घ गिनती)
{
	वापस sparse_discard(channel, blk, गिनती);
}

अटल errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	वापस 0;
}

अटल errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   स्थिर अक्षर *option __attribute__((unused)),
                                   स्थिर अक्षर *arg __attribute__((unused)))
{
	वापस 0;
}

अटल errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			अचिन्हित दीर्घ दीर्घ गिनती __attribute__((unused)))
{
	वापस 0;
}

अटल काष्ठा struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.खोलो			= sparse_open,
	.बंद			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= शून्य,
	.set_option		= sparse_set_option,
	.get_stats		= शून्य,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

अटल काष्ठा struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.खोलो			= sparsefd_open,
	.बंद			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= शून्य,
	.set_option		= sparse_set_option,
	.get_stats		= शून्य,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#पूर्ण_अगर

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
