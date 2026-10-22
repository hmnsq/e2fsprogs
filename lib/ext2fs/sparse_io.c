#अन्तर्भाव "config.h"
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#यदि_अपरिभाषितम् O_द्विमान
#परिभाषय O_द्विमान 0
#यदि_अन्तः

#यदि !defined(ENABLE_LIBSPARSE)
स्थिर errcode_t sparse_open(ध्रुव वर्ण *name EXT2FS_ATTR((अप्रयुक्तम्)),
			     पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)),
			     io_channel *channel EXT2FS_ATTR((अप्रयुक्तम्)))
{
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}
स्थिर errcode_t sparse_close(io_channel channel EXT2FS_ATTR((अप्रयुक्तम्)))
{
	प्रत्यागम EXT2_ET_UNIMPLEMENTED;
}
स्थिर संरचना struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.उद्घाटय			= sparse_open,
	.close			= sparse_close,
};
स्थिर संरचना struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.उद्घाटय			= sparse_open,
	.close			= sparse_close,
};
#अन्यथा
#अन्तर्भाव <sparse/sparse.h>

संरचना sparse_map {
	पूर्णाङ्क			fd;
	वर्ण			**खण्डानि;
	पूर्णाङ्क			खण्डम्_आकार;
	uint64_t		खण्डानि_गणनम्;
	वर्ण			*file;
	संरचना sparse_file	*sparse_file;
	io_channel		channel;
};

संरचना sparse_io_params {
	पूर्णाङ्क			fd;
	वर्ण			*file;
	uint64_t		खण्डानि_गणनम्;
	अचिह्नित पूर्णाङ्क		खण्डम्_आकार;
};

स्थिर errcode_t sparse_write_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
				  पूर्णाङ्क गणनम्, ध्रुव शून्य *buf);

स्थिर शून्य free_sparse_खण्डानि(संरचना sparse_map *sm)
{
	uint64_t i;

	प्रति (i = 0; i < sm->खण्डानि_गणनम्; ++i)
		मुक्त(sm->खण्डानि[i]);
	मुक्त(sm->खण्डानि);
	sm->खण्डानि = रिक्त;
}

स्थिर पूर्णाङ्क sparse_import_segment(शून्य *priv, ध्रुव शून्य *दत्त, size_t len,
				 अचिह्नित पूर्णाङ्क खण्डम्, अचिह्नित पूर्णाङ्क nr_खण्डानि)
{
	संरचना sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	यदि (!nr_खण्डानि || len % sm->खण्डम्_आकार)
		प्रत्यागम 0;

	प्रत्यागम sparse_write_blk(sm->channel, खण्डम्, nr_खण्डानि, दत्त);
}

स्थिर errcode_t io_manager_import_sparse(संरचना sparse_io_params *params,
					  संरचना sparse_map *sm, io_channel io)
{
	पूर्णाङ्क fd;
	errcode_t retval;
	संरचना sparse_file *sparse_file;

	यदि (params->fd < 0) {
		fd = उद्घाटय(params->file, O_RDONLY);
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

	sm->खण्डम्_आकार = sparse_file_खण्डम्_आकार(sparse_file);
	sm->खण्डानि_गणनम् = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->खण्डम्_आकार + 1;
	sm->खण्डानि = शून्यस्मृतिं_देहि(sm->खण्डानि_गणनम्, परिमाण(वर्ण*));
	यदि (!sm->खण्डानि) {
		retval = -1;
		गच्छ err_alloc;
	}
	io->खण्डम्_आकार = sm->खण्डम्_आकार;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	यदि (retval)
		free_sparse_खण्डानि(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	प्रत्यागम retval;
}

स्थिर errcode_t io_manager_configure(संरचना sparse_io_params *params,
				      पूर्णाङ्क flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_आकार;
	संरचना sparse_map *sm = शून्यस्मृतिं_देहि(1, परिमाण(*sm));
	यदि (!sm)
		प्रत्यागम EXT2_ET_NO_स्मृतिः;

	sm->file = params->file;
	sm->channel = io;
	io->वैयक्तिकम्_दत्त = sm;
	retval = io_manager_import_sparse(params, sm, io);
	यदि (retval) {
		यदि (!params->खण्डम्_आकार || !params->खण्डानि_गणनम्) {
			retval = EINVAL;
			गच्छ err_params;
		}
		sm->खण्डम्_आकार = params->खण्डम्_आकार;
		sm->खण्डानि_गणनम् = params->खण्डानि_गणनम्;
		sm->खण्डानि = शून्यस्मृतिं_देहि(params->खण्डानि_गणनम्, परिमाण(शून्य*));
		यदि (!sm->खण्डानि) {
			retval = EXT2_ET_NO_स्मृतिः;
			गच्छ err_alloc;
		}
	}
	io->खण्डम्_आकार = sm->खण्डम्_आकार;
	img_आकार = (uint64_t)sm->खण्डम्_आकार * sm->खण्डानि_गणनम्;

	यदि (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->खण्डम्_आकार, img_आकार);
		यदि (!sm->sparse_file) {
			retval = EXT2_ET_NO_स्मृतिः;
			गच्छ err_alloc;
		}
		यदि (params->fd < 0) {
			sm->fd = उद्घाटय(params->file, O_CREAT | O_RDWR | O_TRUNC | O_द्विमान,
				      0644);
			यदि (sm->fd < 0) {
				retval = errno;
				गच्छ err_open;
			}
		} अन्यथा
			sm->fd = params->fd;
	} अन्यथा {
		sm->fd = -1;
		sm->sparse_file = रिक्त;
	}
	प्रत्यागम 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_खण्डानि(sm);
err_params:
	मुक्त(sm);
	प्रत्यागम retval;
}

स्थिर errcode_t sparse_open_channel(संरचना sparse_io_params *sparse_params,
				     पूर्णाङ्क flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = शून्यस्मृतिं_देहि(1, परिमाण(संरचना struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->खण्डम्_आकार = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	यदि (retval) {
		मुक्त(io);
		प्रत्यागम retval;
	}

	*channel = io;
	प्रत्यागम 0;
}

स्थिर errcode_t read_sparse_argv(ध्रुव वर्ण *name, bool is_fd,
				  संरचना sparse_io_params *sparse_params)
{
	पूर्णाङ्क ret;
	sparse_params->fd = -1;
	sparse_params->खण्डम्_आकार = 0;
	sparse_params->खण्डानि_गणनम् = 0;

	sparse_params->file = स्मृतिं_देहि(वाक्यदैर्घ्यम्(name) + 1);
	यदि (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", वाक्यदैर्घ्यम्(name) + 1);
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
	}

	यदि (is_fd) {
		ret = वाक्यात्_पठ(name, "(%d):%llu:%u", &sparse_params->fd,
			     (अचिह्नित दीर्घ दीर्घ *)&sparse_params->खण्डानि_गणनम्,
			     &sparse_params->खण्डम्_आकार);
	} अन्यथा {
		ret = वाक्यात्_पठ(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (अचिह्नित दीर्घ दीर्घ *)&sparse_params->खण्डानि_गणनम्,
			     &sparse_params->खण्डम्_आकार);
	}

	यदि (ret < 1) {
		मुक्त(sparse_params->file);
		प्रत्यागम EINVAL;
	}
	प्रत्यागम 0;
}

स्थिर errcode_t sparse_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	errcode_t retval;
	संरचना sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	यदि (retval)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	यदि (retval)
		प्रत्यागम retval;
	(*channel)->manager = sparse_io_manager;

	प्रत्यागम retval;
}

स्थिर errcode_t sparsefd_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	errcode_t retval;
	संरचना sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	यदि (retval)
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	यदि (retval)
		प्रत्यागम retval;
	(*channel)->manager = sparsefd_io_manager;

	प्रत्यागम retval;
}

स्थिर errcode_t sparse_merge_खण्डानि(संरचना sparse_map *sm, uint64_t start,
					uint64_t अङ्कः)
{
	वर्ण *buf;
	uint64_t i;
	अचिह्नित पूर्णाङ्क खण्डम्_आकार = sm->खण्डम्_आकार;
	errcode_t retval = 0;

	buf = शून्यस्मृतिं_देहि(अङ्कः, खण्डम्_आकार);
	यदि (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(अचिह्नित दीर्घ दीर्घ)अङ्कः * खण्डम्_आकार);
		प्रत्यागम EXT2_ET_NO_स्मृतिः;
	}

	प्रति (i = 0; i < अङ्कः; i++) {
		स्मृतिं_प्रतिलिख(buf + i * खण्डम्_आकार, sm->खण्डानि[start + i] , खण्डम्_आकार);
		मुक्त(sm->खण्डानि[start + i]);
		sm->खण्डानि[start + i] = रिक्त;
	}

	/* free_sparse_blocks will release this buf. */
	sm->खण्डानि[start] = buf;

	retval = sparse_file_सङ्कलय_दत्त(sm->sparse_file, sm->खण्डानि[start],
					खण्डम्_आकार * अङ्कः, start);

	प्रत्यागम retval;
}

स्थिर errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	संरचना sparse_map *sm = channel->वैयक्तिकम्_दत्त;

	यदि (sm->sparse_file) {
		int64_t chunk_start = (sm->खण्डानि[0] == रिक्त) ? -1 : 0;
		प्रति (i = 0; i < sm->खण्डानि_गणनम्; ++i) {
			यदि (!sm->खण्डानि[i] && chunk_start != -1) {
				retval = sparse_merge_खण्डानि(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} अन्यथा यदि (sm->खण्डानि[i] && chunk_start == -1) {
				chunk_start = i;
			}
			यदि (retval)
				गच्छ ret;
		}
		यदि (chunk_start != -1) {
			retval = sparse_merge_खण्डानि(sm, chunk_start,
							sm->खण्डानि_गणनम् - chunk_start);
			यदि (retval)
				गच्छ ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	यदि (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_खण्डानि(sm);
	मुक्त(sm->file);
	मुक्त(sm);
	मुक्त(channel);
	प्रत्यागम retval;
}

स्थिर errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	संरचना sparse_map *sm = channel->वैयक्तिकम्_दत्त;
	पूर्णाङ्क fd = sm->fd;

	retval = sparse_close_channel(channel);
	यदि (fd >= 0)
		close(fd);

	प्रत्यागम retval;
}

स्थिर errcode_t sparse_समुच्चयः_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	channel->खण्डम्_आकार = blksize;
	प्रत्यागम 0;
}

स्थिर blk64_t खण्डम्_गन्तव्यम्_sparse_खण्डम्(blk64_t खण्डम्, blk64_t *विस्थापनम्,
			       io_channel channel, संरचना sparse_map *sm)
{
	पूर्णाङ्क अनुपातः;
	blk64_t ret = खण्डम्;

	अनुपातः = sm->खण्डम्_आकार / channel->खण्डम्_आकार;
	ret /= अनुपातः;
	*विस्थापनम् = (खण्डम् % अनुपातः) * channel->खण्डम्_आकार;

	प्रत्यागम ret;
}

स्थिर errcode_t check_खण्डम्_आकार(io_channel channel, संरचना sparse_map *sm)
{
	यदि (sm->खण्डम्_आकार >= channel->खण्डम्_आकार)
		प्रत्यागम 0;
	प्रत्यागम EXT2_ET_UNEXPECTED_खण्डम्_आकार;
}

स्थिर errcode_t sparse_read_blk64(io_channel channel, blk64_t खण्डम्,
				   पूर्णाङ्क गणनम्, शून्य *buf)
{
	पूर्णाङ्क i;
	वर्ण *बहिर्गमनम् = buf;
	blk64_t विस्थापनम् = 0, cur_खण्डम्;
	संरचना sparse_map *sm = channel->वैयक्तिकम्_दत्त;

	यदि (check_खण्डम्_आकार(channel, sm))
		प्रत्यागम EXT2_ET_UNEXPECTED_खण्डम्_आकार;

	यदि (गणनम् < 0) { //partial read
		गणनम् = -गणनम्;
		cur_खण्डम् = खण्डम्_गन्तव्यम्_sparse_खण्डम्(खण्डम्, &विस्थापनम्, channel, sm);
		यदि (sm->खण्डानि[cur_खण्डम्])
			स्मृतिं_प्रतिलिख(बहिर्गमनम्, (sm->खण्डानि[cur_खण्डम्]) + विस्थापनम्, गणनम्);
		अन्यथा
			स्मृतिं_पूरय(बहिर्गमनम्, 0, गणनम्);
	} अन्यथा {
		प्रति (i = 0; i < गणनम्; ++i) {
			cur_खण्डम् = खण्डम्_गन्तव्यम्_sparse_खण्डम्(खण्डम् + i, &विस्थापनम्,
						    channel, sm);
			यदि (sm->खण्डानि[cur_खण्डम्])
				स्मृतिं_प्रतिलिख(बहिर्गमनम् + (i * channel->खण्डम्_आकार),
				       sm->खण्डानि[cur_खण्डम्] + विस्थापनम्,
				       channel->खण्डम्_आकार);
			अन्यथा यदि (sm->खण्डानि)
				स्मृतिं_पूरय(बहिर्गमनम् + (i * channel->खण्डम्_आकार), 0,
				       channel->खण्डम्_आकार);
		}
	}
	प्रत्यागम 0;
}

स्थिर errcode_t sparse_read_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
				 पूर्णाङ्क गणनम्, शून्य *buf)
{
	प्रत्यागम sparse_read_blk64(channel, खण्डम्, गणनम्, buf);
}

स्थिर errcode_t sparse_write_blk64(io_channel channel, blk64_t खण्डम्,
				    पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	पूर्णाङ्क i;
	blk64_t विस्थापनम् = 0, cur_खण्डम्;
	ध्रुव वर्ण *अन्तर् = buf;
	संरचना sparse_map *sm = channel->वैयक्तिकम्_दत्त;

	यदि (check_खण्डम्_आकार(channel, sm))
		प्रत्यागम EXT2_ET_UNEXPECTED_खण्डम्_आकार;

	यदि (गणनम् < 0) { //partial write
		गणनम् = -गणनम्;
		cur_खण्डम् = खण्डम्_गन्तव्यम्_sparse_खण्डम्(खण्डम्, &विस्थापनम्, channel,
						  sm);
		यदि (!sm->खण्डानि[cur_खण्डम्]) {
			sm->खण्डानि[cur_खण्डम्] = शून्यस्मृतिं_देहि(1, sm->खण्डम्_आकार);
			यदि (!sm->खण्डानि[cur_खण्डम्])
				प्रत्यागम EXT2_ET_NO_स्मृतिः;
		}
		स्मृतिं_प्रतिलिख(sm->खण्डानि[cur_खण्डम्] + विस्थापनम्, अन्तर्, गणनम्);
	} अन्यथा {
		प्रति (i = 0; i < गणनम्; ++i) {
			यदि (खण्डम् + i >= sm->खण्डानि_गणनम्)
				प्रत्यागम 0;
			cur_खण्डम् = खण्डम्_गन्तव्यम्_sparse_खण्डम्(खण्डम् + i, &विस्थापनम्,
						    channel, sm);
			यदि (!sm->खण्डानि[cur_खण्डम्]) {
				sm->खण्डानि[cur_खण्डम्] =
					शून्यस्मृतिं_देहि(1, sm->खण्डम्_आकार);
				यदि (!sm->खण्डानि[cur_खण्डम्])
					प्रत्यागम EXT2_ET_NO_स्मृतिः;
			}
			स्मृतिं_प्रतिलिख(sm->खण्डानि[cur_खण्डम्] + विस्थापनम्,
			       अन्तर् + (i * channel->खण्डम्_आकार),
			       channel->खण्डम्_आकार);
		}
	}
	प्रत्यागम 0;
}

स्थिर errcode_t sparse_write_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
				  पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	प्रत्यागम sparse_write_blk64(channel, खण्डम्, गणनम्, buf);
}

स्थिर errcode_t sparse_discard(io_channel channel __attribute__((अप्रयुक्तम्)),
				blk64_t blk, अचिह्नित दीर्घ दीर्घ गणनम्)
{
	blk64_t cur_खण्डम्, विस्थापनम्;
	संरचना sparse_map *sm = channel->वैयक्तिकम्_दत्त;

	यदि (check_खण्डम्_आकार(channel, sm))
		प्रत्यागम EXT2_ET_UNEXPECTED_खण्डम्_आकार;

	प्रति (अचिह्नित दीर्घ दीर्घ i = 0; i < गणनम्; ++i) {
		यदि (blk + i >= sm->खण्डानि_गणनम्)
			प्रत्यागम 0;
		cur_खण्डम् = खण्डम्_गन्तव्यम्_sparse_खण्डम्(blk + i, &विस्थापनम्, channel,
						  sm);
		यदि (!sm->खण्डानि[cur_खण्डम्])
			अनुवर्त;
		मुक्त(sm->खण्डानि[cur_खण्डम्]);
		sm->खण्डानि[cur_खण्डम्] = रिक्त;
	}
	प्रत्यागम 0;
}

स्थिर errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				अचिह्नित दीर्घ दीर्घ गणनम्)
{
	प्रत्यागम sparse_discard(channel, blk, गणनम्);
}

स्थिर errcode_t sparse_flush(io_channel channel __attribute__((अप्रयुक्तम्)))
{
	प्रत्यागम 0;
}

स्थिर errcode_t sparse_समुच्चयः_option(io_channel channel __attribute__((अप्रयुक्तम्)),
                                   ध्रुव वर्ण *option __attribute__((अप्रयुक्तम्)),
                                   ध्रुव वर्ण *arg __attribute__((अप्रयुक्तम्)))
{
	प्रत्यागम 0;
}

स्थिर errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((अप्रयुक्तम्)),
			blk64_t blk __attribute__((अप्रयुक्तम्)),
			अचिह्नित दीर्घ दीर्घ गणनम् __attribute__((अप्रयुक्तम्)))
{
	प्रत्यागम 0;
}

स्थिर संरचना struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.उद्घाटय			= sparse_open,
	.close			= sparse_close,
	.समुच्चयः_blksize		= sparse_समुच्चयः_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= रिक्त,
	.समुच्चयः_option		= sparse_समुच्चयः_option,
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
	.उद्घाटय			= sparsefd_open,
	.close			= sparse_close,
	.समुच्चयः_blksize		= sparse_समुच्चयः_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= रिक्त,
	.समुच्चयः_option		= sparse_समुच्चयः_option,
	.get_stats		= रिक्त,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#यदि_अन्तः

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
