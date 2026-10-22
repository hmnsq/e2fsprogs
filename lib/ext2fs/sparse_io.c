#شامل "config.h"
#شامل <sys/types.h>
#شامل <sys/stat.h>
#شامل <fcntl.h>
#شامل <unistd.h>
#شامل <stdint.h>
#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#اگر_غیر_تعریف_شدہ O_ثنائی
#تعریف_کرو O_ثنائی 0
#اگر_ختم

#اگر !defined(ENABLE_LIBSPARSE)
ساکن errcode_t sparse_open(ثابت حرف *name EXT2FS_ATTR((غیر_مستعمل)),
			     صحیح flags EXT2FS_ATTR((غیر_مستعمل)),
			     io_channel *channel EXT2FS_ATTR((غیر_مستعمل)))
{
	واپس EXT2_ET_UNIMPLEMENTED;
}
ساکن errcode_t sparse_close(io_channel channel EXT2FS_ATTR((غیر_مستعمل)))
{
	واپس EXT2_ET_UNIMPLEMENTED;
}
ساکن ساخت struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
ساکن ساخت struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#ورنہ
#شامل <sparse/sparse.h>

ساخت sparse_map {
	صحیح			fd;
	حرف			**بلاکیں;
	صحیح			بلاک_سائز;
	uint64_t		بلاکیں_گنتی;
	حرف			*file;
	ساخت sparse_file	*sparse_file;
	io_channel		channel;
};

ساخت sparse_io_params {
	صحیح			fd;
	حرف			*file;
	uint64_t		بلاکیں_گنتی;
	غیرعلامتی صحیح		بلاک_سائز;
};

ساکن errcode_t sparse_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
				  صحیح گنتی, ثابت خالی *buf);

ساکن خالی free_sparse_بلاکیں(ساخت sparse_map *sm)
{
	uint64_t i;

	برائے (i = 0; i < sm->بلاکیں_گنتی; ++i)
		آزاد(sm->بلاکیں[i]);
	آزاد(sm->بلاکیں);
	sm->بلاکیں = تہی;
}

ساکن صحیح sparse_import_segment(خالی *priv, ثابت خالی *ڈیٹا, size_t len,
				 غیرعلامتی صحیح بلاک, غیرعلامتی صحیح nr_بلاکیں)
{
	ساخت sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	اگر (!nr_بلاکیں || len % sm->بلاک_سائز)
		واپس 0;

	واپس sparse_write_blk(sm->channel, بلاک, nr_بلاکیں, ڈیٹا);
}

ساکن errcode_t io_manager_import_sparse(ساخت sparse_io_params *params,
					  ساخت sparse_map *sm, io_channel io)
{
	صحیح fd;
	errcode_t retval;
	ساخت sparse_file *sparse_file;

	اگر (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		اگر (fd < 0) {
			retval = -1;
			جاؤ err_open;
		}
	} ورنہ
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	اگر (!sparse_file) {
		retval = -1;
		جاؤ err_sparse;
	}

	sm->بلاک_سائز = sparse_file_بلاک_سائز(sparse_file);
	sm->بلاکیں_گنتی = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->بلاک_سائز + 1;
	sm->بلاکیں = صفر_حافظہ_دو(sm->بلاکیں_گنتی, حجم(حرف*));
	اگر (!sm->بلاکیں) {
		retval = -1;
		جاؤ err_alloc;
	}
	io->بلاک_سائز = sm->بلاک_سائز;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	اگر (retval)
		free_sparse_بلاکیں(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	واپس retval;
}

ساکن errcode_t io_manager_configure(ساخت sparse_io_params *params,
				      صحیح flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_سائز;
	ساخت sparse_map *sm = صفر_حافظہ_دو(1, حجم(*sm));
	اگر (!sm)
		واپس EXT2_ET_NO_حافظہ;

	sm->file = params->file;
	sm->channel = io;
	io->نجی_ڈیٹا = sm;
	retval = io_manager_import_sparse(params, sm, io);
	اگر (retval) {
		اگر (!params->بلاک_سائز || !params->بلاکیں_گنتی) {
			retval = EINVAL;
			جاؤ err_params;
		}
		sm->بلاک_سائز = params->بلاک_سائز;
		sm->بلاکیں_گنتی = params->بلاکیں_گنتی;
		sm->بلاکیں = صفر_حافظہ_دو(params->بلاکیں_گنتی, حجم(خالی*));
		اگر (!sm->بلاکیں) {
			retval = EXT2_ET_NO_حافظہ;
			جاؤ err_alloc;
		}
	}
	io->بلاک_سائز = sm->بلاک_سائز;
	img_سائز = (uint64_t)sm->بلاک_سائز * sm->بلاکیں_گنتی;

	اگر (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->بلاک_سائز, img_سائز);
		اگر (!sm->sparse_file) {
			retval = EXT2_ET_NO_حافظہ;
			جاؤ err_alloc;
		}
		اگر (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_ثنائی,
				      0644);
			اگر (sm->fd < 0) {
				retval = errno;
				جاؤ err_open;
			}
		} ورنہ
			sm->fd = params->fd;
	} ورنہ {
		sm->fd = -1;
		sm->sparse_file = تہی;
	}
	واپس 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_بلاکیں(sm);
err_params:
	آزاد(sm);
	واپس retval;
}

ساکن errcode_t sparse_open_channel(ساخت sparse_io_params *sparse_params,
				     صحیح flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = صفر_حافظہ_دو(1, حجم(ساخت struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->بلاک_سائز = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	اگر (retval) {
		آزاد(io);
		واپس retval;
	}

	*channel = io;
	واپس 0;
}

ساکن errcode_t read_sparse_argv(ثابت حرف *name, bool is_fd,
				  ساخت sparse_io_params *sparse_params)
{
	صحیح ret;
	sparse_params->fd = -1;
	sparse_params->بلاک_سائز = 0;
	sparse_params->بلاکیں_گنتی = 0;

	sparse_params->file = حافظہ_دو(لڑی_لمبائی(name) + 1);
	اگر (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", لڑی_لمبائی(name) + 1);
		واپس EXT2_ET_NO_حافظہ;
	}

	اگر (is_fd) {
		ret = لڑی_سے_پڑھو(name, "(%d):%llu:%u", &sparse_params->fd,
			     (غیرعلامتی طویل طویل *)&sparse_params->بلاکیں_گنتی,
			     &sparse_params->بلاک_سائز);
	} ورنہ {
		ret = لڑی_سے_پڑھو(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (غیرعلامتی طویل طویل *)&sparse_params->بلاکیں_گنتی,
			     &sparse_params->بلاک_سائز);
	}

	اگر (ret < 1) {
		آزاد(sparse_params->file);
		واپس EINVAL;
	}
	واپس 0;
}

ساکن errcode_t sparse_open(ثابت حرف *name, صحیح flags, io_channel *channel)
{
	errcode_t retval;
	ساخت sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	اگر (retval)
		واپس EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	اگر (retval)
		واپس retval;
	(*channel)->manager = sparse_io_manager;

	واپس retval;
}

ساکن errcode_t sparsefd_open(ثابت حرف *name, صحیح flags, io_channel *channel)
{
	errcode_t retval;
	ساخت sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	اگر (retval)
		واپس EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	اگر (retval)
		واپس retval;
	(*channel)->manager = sparsefd_io_manager;

	واپس retval;
}

ساکن errcode_t sparse_ملاؤ_بلاکیں(ساخت sparse_map *sm, uint64_t آغاز,
					uint64_t عدد)
{
	حرف *buf;
	uint64_t i;
	غیرعلامتی صحیح بلاک_سائز = sm->بلاک_سائز;
	errcode_t retval = 0;

	buf = صفر_حافظہ_دو(عدد, بلاک_سائز);
	اگر (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(غیرعلامتی طویل طویل)عدد * بلاک_سائز);
		واپس EXT2_ET_NO_حافظہ;
	}

	برائے (i = 0; i < عدد; i++) {
		حافظہ_نقل(buf + i * بلاک_سائز, sm->بلاکیں[آغاز + i] , بلاک_سائز);
		آزاد(sm->بلاکیں[آغاز + i]);
		sm->بلاکیں[آغاز + i] = تہی;
	}

	/* free_sparse_blocks will release this buf. */
	sm->بلاکیں[آغاز] = buf;

	retval = sparse_file_جوڑو_ڈیٹا(sm->sparse_file, sm->بلاکیں[آغاز],
					بلاک_سائز * عدد, آغاز);

	واپس retval;
}

ساکن errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	ساخت sparse_map *sm = channel->نجی_ڈیٹا;

	اگر (sm->sparse_file) {
		int64_t chunk_آغاز = (sm->بلاکیں[0] == تہی) ? -1 : 0;
		برائے (i = 0; i < sm->بلاکیں_گنتی; ++i) {
			اگر (!sm->بلاکیں[i] && chunk_آغاز != -1) {
				retval = sparse_ملاؤ_بلاکیں(sm, chunk_آغاز, i - chunk_آغاز);
				chunk_آغاز = -1;
			} ورنہ اگر (sm->بلاکیں[i] && chunk_آغاز == -1) {
				chunk_آغاز = i;
			}
			اگر (retval)
				جاؤ ret;
		}
		اگر (chunk_آغاز != -1) {
			retval = sparse_ملاؤ_بلاکیں(sm, chunk_آغاز,
							sm->بلاکیں_گنتی - chunk_آغاز);
			اگر (retval)
				جاؤ ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*چکری_جانچ*/0);
	}

ret:
	اگر (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_بلاکیں(sm);
	آزاد(sm->file);
	آزاد(sm);
	آزاد(channel);
	واپس retval;
}

ساکن errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	ساخت sparse_map *sm = channel->نجی_ڈیٹا;
	صحیح fd = sm->fd;

	retval = sparse_close_channel(channel);
	اگر (fd >= 0)
		close(fd);

	واپس retval;
}

ساکن errcode_t sparse_سیٹ_blksize(io_channel channel, صحیح blksize)
{
	channel->بلاک_سائز = blksize;
	واپس 0;
}

ساکن blk64_t بلاک_مقصد_sparse_بلاک(blk64_t بلاک, blk64_t *انحراف,
			       io_channel channel, ساخت sparse_map *sm)
{
	صحیح تناسب;
	blk64_t ret = بلاک;

	تناسب = sm->بلاک_سائز / channel->بلاک_سائز;
	ret /= تناسب;
	*انحراف = (بلاک % تناسب) * channel->بلاک_سائز;

	واپس ret;
}

ساکن errcode_t check_بلاک_سائز(io_channel channel, ساخت sparse_map *sm)
{
	اگر (sm->بلاک_سائز >= channel->بلاک_سائز)
		واپس 0;
	واپس EXT2_ET_UNEXPECTED_بلاک_سائز;
}

ساکن errcode_t sparse_read_blk64(io_channel channel, blk64_t بلاک,
				   صحیح گنتی, خالی *buf)
{
	صحیح i;
	حرف *برآمد = buf;
	blk64_t انحراف = 0, cur_بلاک;
	ساخت sparse_map *sm = channel->نجی_ڈیٹا;

	اگر (check_بلاک_سائز(channel, sm))
		واپس EXT2_ET_UNEXPECTED_بلاک_سائز;

	اگر (گنتی < 0) { //partial read
		گنتی = -گنتی;
		cur_بلاک = بلاک_مقصد_sparse_بلاک(بلاک, &انحراف, channel, sm);
		اگر (sm->بلاکیں[cur_بلاک])
			حافظہ_نقل(برآمد, (sm->بلاکیں[cur_بلاک]) + انحراف, گنتی);
		ورنہ
			memset(برآمد, 0, گنتی);
	} ورنہ {
		برائے (i = 0; i < گنتی; ++i) {
			cur_بلاک = بلاک_مقصد_sparse_بلاک(بلاک + i, &انحراف,
						    channel, sm);
			اگر (sm->بلاکیں[cur_بلاک])
				حافظہ_نقل(برآمد + (i * channel->بلاک_سائز),
				       sm->بلاکیں[cur_بلاک] + انحراف,
				       channel->بلاک_سائز);
			ورنہ اگر (sm->بلاکیں)
				memset(برآمد + (i * channel->بلاک_سائز), 0,
				       channel->بلاک_سائز);
		}
	}
	واپس 0;
}

ساکن errcode_t sparse_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
				 صحیح گنتی, خالی *buf)
{
	واپس sparse_read_blk64(channel, بلاک, گنتی, buf);
}

ساکن errcode_t sparse_write_blk64(io_channel channel, blk64_t بلاک,
				    صحیح گنتی, ثابت خالی *buf)
{
	صحیح i;
	blk64_t انحراف = 0, cur_بلاک;
	ثابت حرف *اندر = buf;
	ساخت sparse_map *sm = channel->نجی_ڈیٹا;

	اگر (check_بلاک_سائز(channel, sm))
		واپس EXT2_ET_UNEXPECTED_بلاک_سائز;

	اگر (گنتی < 0) { //partial write
		گنتی = -گنتی;
		cur_بلاک = بلاک_مقصد_sparse_بلاک(بلاک, &انحراف, channel,
						  sm);
		اگر (!sm->بلاکیں[cur_بلاک]) {
			sm->بلاکیں[cur_بلاک] = صفر_حافظہ_دو(1, sm->بلاک_سائز);
			اگر (!sm->بلاکیں[cur_بلاک])
				واپس EXT2_ET_NO_حافظہ;
		}
		حافظہ_نقل(sm->بلاکیں[cur_بلاک] + انحراف, اندر, گنتی);
	} ورنہ {
		برائے (i = 0; i < گنتی; ++i) {
			اگر (بلاک + i >= sm->بلاکیں_گنتی)
				واپس 0;
			cur_بلاک = بلاک_مقصد_sparse_بلاک(بلاک + i, &انحراف,
						    channel, sm);
			اگر (!sm->بلاکیں[cur_بلاک]) {
				sm->بلاکیں[cur_بلاک] =
					صفر_حافظہ_دو(1, sm->بلاک_سائز);
				اگر (!sm->بلاکیں[cur_بلاک])
					واپس EXT2_ET_NO_حافظہ;
			}
			حافظہ_نقل(sm->بلاکیں[cur_بلاک] + انحراف,
			       اندر + (i * channel->بلاک_سائز),
			       channel->بلاک_سائز);
		}
	}
	واپس 0;
}

ساکن errcode_t sparse_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
				  صحیح گنتی, ثابت خالی *buf)
{
	واپس sparse_write_blk64(channel, بلاک, گنتی, buf);
}

ساکن errcode_t sparse_discard(io_channel channel __attribute__((غیر_مستعمل)),
				blk64_t blk, غیرعلامتی طویل طویل گنتی)
{
	blk64_t cur_بلاک, انحراف;
	ساخت sparse_map *sm = channel->نجی_ڈیٹا;

	اگر (check_بلاک_سائز(channel, sm))
		واپس EXT2_ET_UNEXPECTED_بلاک_سائز;

	برائے (غیرعلامتی طویل طویل i = 0; i < گنتی; ++i) {
		اگر (blk + i >= sm->بلاکیں_گنتی)
			واپس 0;
		cur_بلاک = بلاک_مقصد_sparse_بلاک(blk + i, &انحراف, channel,
						  sm);
		اگر (!sm->بلاکیں[cur_بلاک])
			جاری;
		آزاد(sm->بلاکیں[cur_بلاک]);
		sm->بلاکیں[cur_بلاک] = تہی;
	}
	واپس 0;
}

ساکن errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				غیرعلامتی طویل طویل گنتی)
{
	واپس sparse_discard(channel, blk, گنتی);
}

ساکن errcode_t sparse_flush(io_channel channel __attribute__((غیر_مستعمل)))
{
	واپس 0;
}

ساکن errcode_t sparse_سیٹ_option(io_channel channel __attribute__((غیر_مستعمل)),
                                   ثابت حرف *option __attribute__((غیر_مستعمل)),
                                   ثابت حرف *arg __attribute__((غیر_مستعمل)))
{
	واپس 0;
}

ساکن errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((غیر_مستعمل)),
			blk64_t blk __attribute__((غیر_مستعمل)),
			غیرعلامتی طویل طویل گنتی __attribute__((غیر_مستعمل)))
{
	واپس 0;
}

ساکن ساخت struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.سیٹ_blksize		= sparse_سیٹ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= تہی,
	.سیٹ_option		= sparse_سیٹ_option,
	.get_stats		= تہی,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

ساکن ساخت struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.سیٹ_blksize		= sparse_سیٹ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= تہی,
	.سیٹ_option		= sparse_سیٹ_option,
	.get_stats		= تہی,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#اگر_ختم

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
