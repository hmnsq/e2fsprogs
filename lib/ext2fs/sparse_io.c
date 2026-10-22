#అన్తర్భావ "config.h"
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

#నిర్వచితం_కాకపోతే O_ద్విమాన
#నిర్వచించు O_ద్విమాన 0
#అయితే_ముగింపు

#ఒకవేళ !defined(ENABLE_LIBSPARSE)
స్థిర errcode_t sparse_open(స్థిరరాశి అక్షరం *name EXT2FS_ATTR((వాడని)),
			     పూర్ణాంకం flags EXT2FS_ATTR((వాడని)),
			     io_channel *channel EXT2FS_ATTR((వాడని)))
{
	తిరిగి EXT2_ET_UNIMPLEMENTED;
}
స్థిర errcode_t sparse_close(io_channel channel EXT2FS_ATTR((వాడని)))
{
	తిరిగి EXT2_ET_UNIMPLEMENTED;
}
స్థిర నిర్మాణం struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
స్థిర నిర్మాణం struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#లేకపోతే
#అన్తర్భావ <sparse/sparse.h>

నిర్మాణం sparse_map {
	పూర్ణాంకం			fd;
	అక్షరం			**ఖండాలు;
	పూర్ణాంకం			ఖండం_కొలత;
	uint64_t		ఖండాలు_లెక్క;
	అక్షరం			*file;
	నిర్మాణం sparse_file	*sparse_file;
	io_channel		channel;
};

నిర్మాణం sparse_io_params {
	పూర్ణాంకం			fd;
	అక్షరం			*file;
	uint64_t		ఖండాలు_లెక్క;
	గుర్తులేని పూర్ణాంకం		ఖండం_కొలత;
};

స్థిర errcode_t sparse_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
				  పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf);

స్థిర శూన్య free_sparse_ఖండాలు(నిర్మాణం sparse_map *sm)
{
	uint64_t i;

	కొరకు (i = 0; i < sm->ఖండాలు_లెక్క; ++i)
		విడుదల(sm->ఖండాలు[i]);
	విడుదల(sm->ఖండాలు);
	sm->ఖండాలు = ఖాళీ;
}

స్థిర పూర్ణాంకం sparse_import_segment(శూన్య *priv, స్థిరరాశి శూన్య *దత్తాంశం, size_t len,
				 గుర్తులేని పూర్ణాంకం ఖండం, గుర్తులేని పూర్ణాంకం nr_ఖండాలు)
{
	నిర్మాణం sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	ఒకవేళ (!nr_ఖండాలు || len % sm->ఖండం_కొలత)
		తిరిగి 0;

	తిరిగి sparse_write_blk(sm->channel, ఖండం, nr_ఖండాలు, దత్తాంశం);
}

స్థిర errcode_t io_manager_import_sparse(నిర్మాణం sparse_io_params *params,
					  నిర్మాణం sparse_map *sm, io_channel io)
{
	పూర్ణాంకం fd;
	errcode_t retval;
	నిర్మాణం sparse_file *sparse_file;

	ఒకవేళ (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		ఒకవేళ (fd < 0) {
			retval = -1;
			వెళ్ళు err_open;
		}
	} లేకపోతే
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	ఒకవేళ (!sparse_file) {
		retval = -1;
		వెళ్ళు err_sparse;
	}

	sm->ఖండం_కొలత = sparse_file_ఖండం_కొలత(sparse_file);
	sm->ఖండాలు_లెక్క = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->ఖండం_కొలత + 1;
	sm->ఖండాలు = సున్నా_జ్ఞాపకం_ఇవ్వు(sm->ఖండాలు_లెక్క, పరిమాణం(అక్షరం*));
	ఒకవేళ (!sm->ఖండాలు) {
		retval = -1;
		వెళ్ళు err_alloc;
	}
	io->ఖండం_కొలత = sm->ఖండం_కొలత;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	ఒకవేళ (retval)
		free_sparse_ఖండాలు(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	తిరిగి retval;
}

స్థిర errcode_t io_manager_configure(నిర్మాణం sparse_io_params *params,
				      పూర్ణాంకం flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_కొలత;
	నిర్మాణం sparse_map *sm = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*sm));
	ఒకవేళ (!sm)
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;

	sm->file = params->file;
	sm->channel = io;
	io->వ్యక్తిగత_దత్తాంశం = sm;
	retval = io_manager_import_sparse(params, sm, io);
	ఒకవేళ (retval) {
		ఒకవేళ (!params->ఖండం_కొలత || !params->ఖండాలు_లెక్క) {
			retval = EINVAL;
			వెళ్ళు err_params;
		}
		sm->ఖండం_కొలత = params->ఖండం_కొలత;
		sm->ఖండాలు_లెక్క = params->ఖండాలు_లెక్క;
		sm->ఖండాలు = సున్నా_జ్ఞాపకం_ఇవ్వు(params->ఖండాలు_లెక్క, పరిమాణం(శూన్య*));
		ఒకవేళ (!sm->ఖండాలు) {
			retval = EXT2_ET_NO_జ్ఞాపకం;
			వెళ్ళు err_alloc;
		}
	}
	io->ఖండం_కొలత = sm->ఖండం_కొలత;
	img_కొలత = (uint64_t)sm->ఖండం_కొలత * sm->ఖండాలు_లెక్క;

	ఒకవేళ (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->ఖండం_కొలత, img_కొలత);
		ఒకవేళ (!sm->sparse_file) {
			retval = EXT2_ET_NO_జ్ఞాపకం;
			వెళ్ళు err_alloc;
		}
		ఒకవేళ (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_ద్విమాన,
				      0644);
			ఒకవేళ (sm->fd < 0) {
				retval = errno;
				వెళ్ళు err_open;
			}
		} లేకపోతే
			sm->fd = params->fd;
	} లేకపోతే {
		sm->fd = -1;
		sm->sparse_file = ఖాళీ;
	}
	తిరిగి 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_ఖండాలు(sm);
err_params:
	విడుదల(sm);
	తిరిగి retval;
}

స్థిర errcode_t sparse_open_channel(నిర్మాణం sparse_io_params *sparse_params,
				     పూర్ణాంకం flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(నిర్మాణం struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->ఖండం_కొలత = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	ఒకవేళ (retval) {
		విడుదల(io);
		తిరిగి retval;
	}

	*channel = io;
	తిరిగి 0;
}

స్థిర errcode_t read_sparse_argv(స్థిరరాశి అక్షరం *name, bool is_fd,
				  నిర్మాణం sparse_io_params *sparse_params)
{
	పూర్ణాంకం ret;
	sparse_params->fd = -1;
	sparse_params->ఖండం_కొలత = 0;
	sparse_params->ఖండాలు_లెక్క = 0;

	sparse_params->file = జ్ఞాపకం_ఇవ్వు(అక్షరశ్రేణి_పొడవు(name) + 1);
	ఒకవేళ (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", అక్షరశ్రేణి_పొడవు(name) + 1);
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	}

	ఒకవేళ (is_fd) {
		ret = అక్షరశ్రేణి_చదువు(name, "(%d):%llu:%u", &sparse_params->fd,
			     (గుర్తులేని దీర్ఘ దీర్ఘ *)&sparse_params->ఖండాలు_లెక్క,
			     &sparse_params->ఖండం_కొలత);
	} లేకపోతే {
		ret = అక్షరశ్రేణి_చదువు(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (గుర్తులేని దీర్ఘ దీర్ఘ *)&sparse_params->ఖండాలు_లెక్క,
			     &sparse_params->ఖండం_కొలత);
	}

	ఒకవేళ (ret < 1) {
		విడుదల(sparse_params->file);
		తిరిగి EINVAL;
	}
	తిరిగి 0;
}

స్థిర errcode_t sparse_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, io_channel *channel)
{
	errcode_t retval;
	నిర్మాణం sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	ఒకవేళ (retval)
		తిరిగి EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ఒకవేళ (retval)
		తిరిగి retval;
	(*channel)->manager = sparse_io_manager;

	తిరిగి retval;
}

స్థిర errcode_t sparsefd_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, io_channel *channel)
{
	errcode_t retval;
	నిర్మాణం sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	ఒకవేళ (retval)
		తిరిగి EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ఒకవేళ (retval)
		తిరిగి retval;
	(*channel)->manager = sparsefd_io_manager;

	తిరిగి retval;
}

స్థిర errcode_t sparse_విలీనం_ఖండాలు(నిర్మాణం sparse_map *sm, uint64_t ప్రారంభం,
					uint64_t అంకె)
{
	అక్షరం *buf;
	uint64_t i;
	గుర్తులేని పూర్ణాంకం ఖండం_కొలత = sm->ఖండం_కొలత;
	errcode_t retval = 0;

	buf = సున్నా_జ్ఞాపకం_ఇవ్వు(అంకె, ఖండం_కొలత);
	ఒకవేళ (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(గుర్తులేని దీర్ఘ దీర్ఘ)అంకె * ఖండం_కొలత);
		తిరిగి EXT2_ET_NO_జ్ఞాపకం;
	}

	కొరకు (i = 0; i < అంకె; i++) {
		జ్ఞాపకం_నకలు(buf + i * ఖండం_కొలత, sm->ఖండాలు[ప్రారంభం + i] , ఖండం_కొలత);
		విడుదల(sm->ఖండాలు[ప్రారంభం + i]);
		sm->ఖండాలు[ప్రారంభం + i] = ఖాళీ;
	}

	/* free_sparse_blocks will release this buf. */
	sm->ఖండాలు[ప్రారంభం] = buf;

	retval = sparse_file_కూడు_దత్తాంశం(sm->sparse_file, sm->ఖండాలు[ప్రారంభం],
					ఖండం_కొలత * అంకె, ప్రారంభం);

	తిరిగి retval;
}

స్థిర errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	నిర్మాణం sparse_map *sm = channel->వ్యక్తిగత_దత్తాంశం;

	ఒకవేళ (sm->sparse_file) {
		int64_t chunk_ప్రారంభం = (sm->ఖండాలు[0] == ఖాళీ) ? -1 : 0;
		కొరకు (i = 0; i < sm->ఖండాలు_లెక్క; ++i) {
			ఒకవేళ (!sm->ఖండాలు[i] && chunk_ప్రారంభం != -1) {
				retval = sparse_విలీనం_ఖండాలు(sm, chunk_ప్రారంభం, i - chunk_ప్రారంభం);
				chunk_ప్రారంభం = -1;
			} లేకపోతే ఒకవేళ (sm->ఖండాలు[i] && chunk_ప్రారంభం == -1) {
				chunk_ప్రారంభం = i;
			}
			ఒకవేళ (retval)
				వెళ్ళు ret;
		}
		ఒకవేళ (chunk_ప్రారంభం != -1) {
			retval = sparse_విలీనం_ఖండాలు(sm, chunk_ప్రారంభం,
							sm->ఖండాలు_లెక్క - chunk_ప్రారంభం);
			ఒకవేళ (retval)
				వెళ్ళు ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*చక్రీయ_తనిఖీ*/0);
	}

ret:
	ఒకవేళ (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_ఖండాలు(sm);
	విడుదల(sm->file);
	విడుదల(sm);
	విడుదల(channel);
	తిరిగి retval;
}

స్థిర errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	నిర్మాణం sparse_map *sm = channel->వ్యక్తిగత_దత్తాంశం;
	పూర్ణాంకం fd = sm->fd;

	retval = sparse_close_channel(channel);
	ఒకవేళ (fd >= 0)
		close(fd);

	తిరిగి retval;
}

స్థిర errcode_t sparse_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize)
{
	channel->ఖండం_కొలత = blksize;
	తిరిగి 0;
}

స్థిర blk64_t ఖండం_గమ్యం_sparse_ఖండం(blk64_t ఖండం, blk64_t *స్థానభ్రంశం,
			       io_channel channel, నిర్మాణం sparse_map *sm)
{
	పూర్ణాంకం నిష్పత్తి;
	blk64_t ret = ఖండం;

	నిష్పత్తి = sm->ఖండం_కొలత / channel->ఖండం_కొలత;
	ret /= నిష్పత్తి;
	*స్థానభ్రంశం = (ఖండం % నిష్పత్తి) * channel->ఖండం_కొలత;

	తిరిగి ret;
}

స్థిర errcode_t check_ఖండం_కొలత(io_channel channel, నిర్మాణం sparse_map *sm)
{
	ఒకవేళ (sm->ఖండం_కొలత >= channel->ఖండం_కొలత)
		తిరిగి 0;
	తిరిగి EXT2_ET_UNEXPECTED_ఖండం_కొలత;
}

స్థిర errcode_t sparse_read_blk64(io_channel channel, blk64_t ఖండం,
				   పూర్ణాంకం లెక్క, శూన్య *buf)
{
	పూర్ణాంకం i;
	అక్షరం *బహిర్గతం = buf;
	blk64_t స్థానభ్రంశం = 0, cur_ఖండం;
	నిర్మాణం sparse_map *sm = channel->వ్యక్తిగత_దత్తాంశం;

	ఒకవేళ (check_ఖండం_కొలత(channel, sm))
		తిరిగి EXT2_ET_UNEXPECTED_ఖండం_కొలత;

	ఒకవేళ (లెక్క < 0) { //partial read
		లెక్క = -లెక్క;
		cur_ఖండం = ఖండం_గమ్యం_sparse_ఖండం(ఖండం, &స్థానభ్రంశం, channel, sm);
		ఒకవేళ (sm->ఖండాలు[cur_ఖండం])
			జ్ఞాపకం_నకలు(బహిర్గతం, (sm->ఖండాలు[cur_ఖండం]) + స్థానభ్రంశం, లెక్క);
		లేకపోతే
			memset(బహిర్గతం, 0, లెక్క);
	} లేకపోతే {
		కొరకు (i = 0; i < లెక్క; ++i) {
			cur_ఖండం = ఖండం_గమ్యం_sparse_ఖండం(ఖండం + i, &స్థానభ్రంశం,
						    channel, sm);
			ఒకవేళ (sm->ఖండాలు[cur_ఖండం])
				జ్ఞాపకం_నకలు(బహిర్గతం + (i * channel->ఖండం_కొలత),
				       sm->ఖండాలు[cur_ఖండం] + స్థానభ్రంశం,
				       channel->ఖండం_కొలత);
			లేకపోతే ఒకవేళ (sm->ఖండాలు)
				memset(బహిర్గతం + (i * channel->ఖండం_కొలత), 0,
				       channel->ఖండం_కొలత);
		}
	}
	తిరిగి 0;
}

స్థిర errcode_t sparse_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
				 పూర్ణాంకం లెక్క, శూన్య *buf)
{
	తిరిగి sparse_read_blk64(channel, ఖండం, లెక్క, buf);
}

స్థిర errcode_t sparse_write_blk64(io_channel channel, blk64_t ఖండం,
				    పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	పూర్ణాంకం i;
	blk64_t స్థానభ్రంశం = 0, cur_ఖండం;
	స్థిరరాశి అక్షరం *లోపల = buf;
	నిర్మాణం sparse_map *sm = channel->వ్యక్తిగత_దత్తాంశం;

	ఒకవేళ (check_ఖండం_కొలత(channel, sm))
		తిరిగి EXT2_ET_UNEXPECTED_ఖండం_కొలత;

	ఒకవేళ (లెక్క < 0) { //partial write
		లెక్క = -లెక్క;
		cur_ఖండం = ఖండం_గమ్యం_sparse_ఖండం(ఖండం, &స్థానభ్రంశం, channel,
						  sm);
		ఒకవేళ (!sm->ఖండాలు[cur_ఖండం]) {
			sm->ఖండాలు[cur_ఖండం] = సున్నా_జ్ఞాపకం_ఇవ్వు(1, sm->ఖండం_కొలత);
			ఒకవేళ (!sm->ఖండాలు[cur_ఖండం])
				తిరిగి EXT2_ET_NO_జ్ఞాపకం;
		}
		జ్ఞాపకం_నకలు(sm->ఖండాలు[cur_ఖండం] + స్థానభ్రంశం, లోపల, లెక్క);
	} లేకపోతే {
		కొరకు (i = 0; i < లెక్క; ++i) {
			ఒకవేళ (ఖండం + i >= sm->ఖండాలు_లెక్క)
				తిరిగి 0;
			cur_ఖండం = ఖండం_గమ్యం_sparse_ఖండం(ఖండం + i, &స్థానభ్రంశం,
						    channel, sm);
			ఒకవేళ (!sm->ఖండాలు[cur_ఖండం]) {
				sm->ఖండాలు[cur_ఖండం] =
					సున్నా_జ్ఞాపకం_ఇవ్వు(1, sm->ఖండం_కొలత);
				ఒకవేళ (!sm->ఖండాలు[cur_ఖండం])
					తిరిగి EXT2_ET_NO_జ్ఞాపకం;
			}
			జ్ఞాపకం_నకలు(sm->ఖండాలు[cur_ఖండం] + స్థానభ్రంశం,
			       లోపల + (i * channel->ఖండం_కొలత),
			       channel->ఖండం_కొలత);
		}
	}
	తిరిగి 0;
}

స్థిర errcode_t sparse_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
				  పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	తిరిగి sparse_write_blk64(channel, ఖండం, లెక్క, buf);
}

స్థిర errcode_t sparse_discard(io_channel channel __attribute__((వాడని)),
				blk64_t blk, గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	blk64_t cur_ఖండం, స్థానభ్రంశం;
	నిర్మాణం sparse_map *sm = channel->వ్యక్తిగత_దత్తాంశం;

	ఒకవేళ (check_ఖండం_కొలత(channel, sm))
		తిరిగి EXT2_ET_UNEXPECTED_ఖండం_కొలత;

	కొరకు (గుర్తులేని దీర్ఘ దీర్ఘ i = 0; i < లెక్క; ++i) {
		ఒకవేళ (blk + i >= sm->ఖండాలు_లెక్క)
			తిరిగి 0;
		cur_ఖండం = ఖండం_గమ్యం_sparse_ఖండం(blk + i, &స్థానభ్రంశం, channel,
						  sm);
		ఒకవేళ (!sm->ఖండాలు[cur_ఖండం])
			కొనసాగు;
		విడుదల(sm->ఖండాలు[cur_ఖండం]);
		sm->ఖండాలు[cur_ఖండం] = ఖాళీ;
	}
	తిరిగి 0;
}

స్థిర errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				గుర్తులేని దీర్ఘ దీర్ఘ లెక్క)
{
	తిరిగి sparse_discard(channel, blk, లెక్క);
}

స్థిర errcode_t sparse_flush(io_channel channel __attribute__((వాడని)))
{
	తిరిగి 0;
}

స్థిర errcode_t sparse_సమితి_option(io_channel channel __attribute__((వాడని)),
                                   స్థిరరాశి అక్షరం *option __attribute__((వాడని)),
                                   స్థిరరాశి అక్షరం *arg __attribute__((వాడని)))
{
	తిరిగి 0;
}

స్థిర errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((వాడని)),
			blk64_t blk __attribute__((వాడని)),
			గుర్తులేని దీర్ఘ దీర్ఘ లెక్క __attribute__((వాడని)))
{
	తిరిగి 0;
}

స్థిర నిర్మాణం struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.సమితి_blksize		= sparse_సమితి_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ఖాళీ,
	.సమితి_option		= sparse_సమితి_option,
	.get_stats		= ఖాళీ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

స్థిర నిర్మాణం struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.సమితి_blksize		= sparse_సమితి_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ఖాళీ,
	.సమితి_option		= sparse_సమితి_option,
	.get_stats		= ఖాళీ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#అయితే_ముగింపు

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
