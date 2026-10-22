#ضمن "config.h"
#ضمن <sys/types.h>
#ضمن <sys/stat.h>
#ضمن <fcntl.h>
#ضمن <unistd.h>
#ضمن <stdint.h>
#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#جيڪڏهن_وصف_نه_ٿيل O_ٻٽو
#وصف_ڪريو O_ٻٽو 0
#جيڪڏهن_پورو

#إذا !defined(ENABLE_LIBSPARSE)
ساكن errcode_t sparse_open(ثابت حرف *name EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			     صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			     io_channel *channel EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	أرجع EXT2_ET_UNIMPLEMENTED;
}
ساكن errcode_t sparse_close(io_channel channel EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	أرجع EXT2_ET_UNIMPLEMENTED;
}
ساكن بنية struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
ساكن بنية struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#وإلا
#ضمن <sparse/sparse.h>

بنية sparse_map {
	صحيح			fd;
	حرف			**بلاڪون;
	صحيح			بلاڪ_سائيز;
	uint64_t		بلاڪون_ڳڻپ;
	حرف			*file;
	بنية sparse_file	*sparse_file;
	io_channel		channel;
};

بنية sparse_io_params {
	صحيح			fd;
	حرف			*file;
	uint64_t		بلاڪون_ڳڻپ;
	بنانشان صحيح		بلاڪ_سائيز;
};

ساكن errcode_t sparse_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
				  صحيح ڳڻپ, ثابت فراغ *buf);

ساكن فراغ free_sparse_بلاڪون(بنية sparse_map *sm)
{
	uint64_t i;

	لكل (i = 0; i < sm->بلاڪون_ڳڻپ; ++i)
		حرر(sm->بلاڪون[i]);
	حرر(sm->بلاڪون);
	sm->بلاڪون = عدم;
}

ساكن صحيح sparse_import_segment(فراغ *priv, ثابت فراغ *معلومات, size_t len,
				 بنانشان صحيح بلاڪ, بنانشان صحيح nr_بلاڪون)
{
	بنية sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	إذا (!nr_بلاڪون || len % sm->بلاڪ_سائيز)
		أرجع 0;

	أرجع sparse_write_blk(sm->channel, بلاڪ, nr_بلاڪون, معلومات);
}

ساكن errcode_t io_manager_import_sparse(بنية sparse_io_params *params,
					  بنية sparse_map *sm, io_channel io)
{
	صحيح fd;
	errcode_t retval;
	بنية sparse_file *sparse_file;

	إذا (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		إذا (fd < 0) {
			retval = -1;
			اذهب err_open;
		}
	} وإلا
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	إذا (!sparse_file) {
		retval = -1;
		اذهب err_sparse;
	}

	sm->بلاڪ_سائيز = sparse_file_بلاڪ_سائيز(sparse_file);
	sm->بلاڪون_ڳڻپ = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->بلاڪ_سائيز + 1;
	sm->بلاڪون = صفر_يادگيري_ڏيو(sm->بلاڪون_ڳڻپ, حجم(حرف*));
	إذا (!sm->بلاڪون) {
		retval = -1;
		اذهب err_alloc;
	}
	io->بلاڪ_سائيز = sm->بلاڪ_سائيز;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	إذا (retval)
		free_sparse_بلاڪون(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	أرجع retval;
}

ساكن errcode_t io_manager_configure(بنية sparse_io_params *params,
				      صحيح flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_سائيز;
	بنية sparse_map *sm = صفر_يادگيري_ڏيو(1, حجم(*sm));
	إذا (!sm)
		أرجع EXT2_ET_NO_يادگيري;

	sm->file = params->file;
	sm->channel = io;
	io->ذاتي_معلومات = sm;
	retval = io_manager_import_sparse(params, sm, io);
	إذا (retval) {
		إذا (!params->بلاڪ_سائيز || !params->بلاڪون_ڳڻپ) {
			retval = EINVAL;
			اذهب err_params;
		}
		sm->بلاڪ_سائيز = params->بلاڪ_سائيز;
		sm->بلاڪون_ڳڻپ = params->بلاڪون_ڳڻپ;
		sm->بلاڪون = صفر_يادگيري_ڏيو(params->بلاڪون_ڳڻپ, حجم(فراغ*));
		إذا (!sm->بلاڪون) {
			retval = EXT2_ET_NO_يادگيري;
			اذهب err_alloc;
		}
	}
	io->بلاڪ_سائيز = sm->بلاڪ_سائيز;
	img_سائيز = (uint64_t)sm->بلاڪ_سائيز * sm->بلاڪون_ڳڻپ;

	إذا (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->بلاڪ_سائيز, img_سائيز);
		إذا (!sm->sparse_file) {
			retval = EXT2_ET_NO_يادگيري;
			اذهب err_alloc;
		}
		إذا (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_ٻٽو,
				      0644);
			إذا (sm->fd < 0) {
				retval = errno;
				اذهب err_open;
			}
		} وإلا
			sm->fd = params->fd;
	} وإلا {
		sm->fd = -1;
		sm->sparse_file = عدم;
	}
	أرجع 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_بلاڪون(sm);
err_params:
	حرر(sm);
	أرجع retval;
}

ساكن errcode_t sparse_open_channel(بنية sparse_io_params *sparse_params,
				     صحيح flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = صفر_يادگيري_ڏيو(1, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->بلاڪ_سائيز = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	إذا (retval) {
		حرر(io);
		أرجع retval;
	}

	*channel = io;
	أرجع 0;
}

ساكن errcode_t read_sparse_argv(ثابت حرف *name, bool is_fd,
				  بنية sparse_io_params *sparse_params)
{
	صحيح ret;
	sparse_params->fd = -1;
	sparse_params->بلاڪ_سائيز = 0;
	sparse_params->بلاڪون_ڳڻپ = 0;

	sparse_params->file = يادگيري_ڏيو(لڙي_ڊيگهه(name) + 1);
	إذا (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", لڙي_ڊيگهه(name) + 1);
		أرجع EXT2_ET_NO_يادگيري;
	}

	إذا (is_fd) {
		ret = لڙي_مان_پڙهو(name, "(%d):%llu:%u", &sparse_params->fd,
			     (بنانشان ڊگهو ڊگهو *)&sparse_params->بلاڪون_ڳڻپ,
			     &sparse_params->بلاڪ_سائيز);
	} وإلا {
		ret = لڙي_مان_پڙهو(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (بنانشان ڊگهو ڊگهو *)&sparse_params->بلاڪون_ڳڻپ,
			     &sparse_params->بلاڪ_سائيز);
	}

	إذا (ret < 1) {
		حرر(sparse_params->file);
		أرجع EINVAL;
	}
	أرجع 0;
}

ساكن errcode_t sparse_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	errcode_t retval;
	بنية sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	إذا (retval)
		أرجع EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	إذا (retval)
		أرجع retval;
	(*channel)->manager = sparse_io_manager;

	أرجع retval;
}

ساكن errcode_t sparsefd_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	errcode_t retval;
	بنية sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	إذا (retval)
		أرجع EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	إذا (retval)
		أرجع retval;
	(*channel)->manager = sparsefd_io_manager;

	أرجع retval;
}

ساكن errcode_t sparse_ملايو_بلاڪون(بنية sparse_map *sm, uint64_t شروعات,
					uint64_t انگ)
{
	حرف *buf;
	uint64_t i;
	بنانشان صحيح بلاڪ_سائيز = sm->بلاڪ_سائيز;
	errcode_t retval = 0;

	buf = صفر_يادگيري_ڏيو(انگ, بلاڪ_سائيز);
	إذا (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(بنانشان ڊگهو ڊگهو)انگ * بلاڪ_سائيز);
		أرجع EXT2_ET_NO_يادگيري;
	}

	لكل (i = 0; i < انگ; i++) {
		يادگيري_نقل(buf + i * بلاڪ_سائيز, sm->بلاڪون[شروعات + i] , بلاڪ_سائيز);
		حرر(sm->بلاڪون[شروعات + i]);
		sm->بلاڪون[شروعات + i] = عدم;
	}

	/* free_sparse_blocks will release this buf. */
	sm->بلاڪون[شروعات] = buf;

	retval = sparse_file_جوڙيو_معلومات(sm->sparse_file, sm->بلاڪون[شروعات],
					بلاڪ_سائيز * انگ, شروعات);

	أرجع retval;
}

ساكن errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	بنية sparse_map *sm = channel->ذاتي_معلومات;

	إذا (sm->sparse_file) {
		int64_t chunk_شروعات = (sm->بلاڪون[0] == عدم) ? -1 : 0;
		لكل (i = 0; i < sm->بلاڪون_ڳڻپ; ++i) {
			إذا (!sm->بلاڪون[i] && chunk_شروعات != -1) {
				retval = sparse_ملايو_بلاڪون(sm, chunk_شروعات, i - chunk_شروعات);
				chunk_شروعات = -1;
			} وإلا إذا (sm->بلاڪون[i] && chunk_شروعات == -1) {
				chunk_شروعات = i;
			}
			إذا (retval)
				اذهب ret;
		}
		إذا (chunk_شروعات != -1) {
			retval = sparse_ملايو_بلاڪون(sm, chunk_شروعات,
							sm->بلاڪون_ڳڻپ - chunk_شروعات);
			إذا (retval)
				اذهب ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*چڪري_جاچ*/0);
	}

ret:
	إذا (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_بلاڪون(sm);
	حرر(sm->file);
	حرر(sm);
	حرر(channel);
	أرجع retval;
}

ساكن errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	بنية sparse_map *sm = channel->ذاتي_معلومات;
	صحيح fd = sm->fd;

	retval = sparse_close_channel(channel);
	إذا (fd >= 0)
		close(fd);

	أرجع retval;
}

ساكن errcode_t sparse_سيٽ_blksize(io_channel channel, صحيح blksize)
{
	channel->بلاڪ_سائيز = blksize;
	أرجع 0;
}

ساكن blk64_t بلاڪ_منزل_sparse_بلاڪ(blk64_t بلاڪ, blk64_t *وٿي,
			       io_channel channel, بنية sparse_map *sm)
{
	صحيح تناسب;
	blk64_t ret = بلاڪ;

	تناسب = sm->بلاڪ_سائيز / channel->بلاڪ_سائيز;
	ret /= تناسب;
	*وٿي = (بلاڪ % تناسب) * channel->بلاڪ_سائيز;

	أرجع ret;
}

ساكن errcode_t check_بلاڪ_سائيز(io_channel channel, بنية sparse_map *sm)
{
	إذا (sm->بلاڪ_سائيز >= channel->بلاڪ_سائيز)
		أرجع 0;
	أرجع EXT2_ET_UNEXPECTED_بلاڪ_سائيز;
}

ساكن errcode_t sparse_read_blk64(io_channel channel, blk64_t بلاڪ,
				   صحيح ڳڻپ, فراغ *buf)
{
	صحيح i;
	حرف *ٻاهريون = buf;
	blk64_t وٿي = 0, cur_بلاڪ;
	بنية sparse_map *sm = channel->ذاتي_معلومات;

	إذا (check_بلاڪ_سائيز(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلاڪ_سائيز;

	إذا (ڳڻپ < 0) { //partial read
		ڳڻپ = -ڳڻپ;
		cur_بلاڪ = بلاڪ_منزل_sparse_بلاڪ(بلاڪ, &وٿي, channel, sm);
		إذا (sm->بلاڪون[cur_بلاڪ])
			يادگيري_نقل(ٻاهريون, (sm->بلاڪون[cur_بلاڪ]) + وٿي, ڳڻپ);
		وإلا
			memset(ٻاهريون, 0, ڳڻپ);
	} وإلا {
		لكل (i = 0; i < ڳڻپ; ++i) {
			cur_بلاڪ = بلاڪ_منزل_sparse_بلاڪ(بلاڪ + i, &وٿي,
						    channel, sm);
			إذا (sm->بلاڪون[cur_بلاڪ])
				يادگيري_نقل(ٻاهريون + (i * channel->بلاڪ_سائيز),
				       sm->بلاڪون[cur_بلاڪ] + وٿي,
				       channel->بلاڪ_سائيز);
			وإلا إذا (sm->بلاڪون)
				memset(ٻاهريون + (i * channel->بلاڪ_سائيز), 0,
				       channel->بلاڪ_سائيز);
		}
	}
	أرجع 0;
}

ساكن errcode_t sparse_read_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
				 صحيح ڳڻپ, فراغ *buf)
{
	أرجع sparse_read_blk64(channel, بلاڪ, ڳڻپ, buf);
}

ساكن errcode_t sparse_write_blk64(io_channel channel, blk64_t بلاڪ,
				    صحيح ڳڻپ, ثابت فراغ *buf)
{
	صحيح i;
	blk64_t وٿي = 0, cur_بلاڪ;
	ثابت حرف *اندريون = buf;
	بنية sparse_map *sm = channel->ذاتي_معلومات;

	إذا (check_بلاڪ_سائيز(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلاڪ_سائيز;

	إذا (ڳڻپ < 0) { //partial write
		ڳڻپ = -ڳڻپ;
		cur_بلاڪ = بلاڪ_منزل_sparse_بلاڪ(بلاڪ, &وٿي, channel,
						  sm);
		إذا (!sm->بلاڪون[cur_بلاڪ]) {
			sm->بلاڪون[cur_بلاڪ] = صفر_يادگيري_ڏيو(1, sm->بلاڪ_سائيز);
			إذا (!sm->بلاڪون[cur_بلاڪ])
				أرجع EXT2_ET_NO_يادگيري;
		}
		يادگيري_نقل(sm->بلاڪون[cur_بلاڪ] + وٿي, اندريون, ڳڻپ);
	} وإلا {
		لكل (i = 0; i < ڳڻپ; ++i) {
			إذا (بلاڪ + i >= sm->بلاڪون_ڳڻپ)
				أرجع 0;
			cur_بلاڪ = بلاڪ_منزل_sparse_بلاڪ(بلاڪ + i, &وٿي,
						    channel, sm);
			إذا (!sm->بلاڪون[cur_بلاڪ]) {
				sm->بلاڪون[cur_بلاڪ] =
					صفر_يادگيري_ڏيو(1, sm->بلاڪ_سائيز);
				إذا (!sm->بلاڪون[cur_بلاڪ])
					أرجع EXT2_ET_NO_يادگيري;
			}
			يادگيري_نقل(sm->بلاڪون[cur_بلاڪ] + وٿي,
			       اندريون + (i * channel->بلاڪ_سائيز),
			       channel->بلاڪ_سائيز);
		}
	}
	أرجع 0;
}

ساكن errcode_t sparse_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
				  صحيح ڳڻپ, ثابت فراغ *buf)
{
	أرجع sparse_write_blk64(channel, بلاڪ, ڳڻپ, buf);
}

ساكن errcode_t sparse_discard(io_channel channel __attribute__((اڻ_استعمال_ٿيل)),
				blk64_t blk, بنانشان ڊگهو ڊگهو ڳڻپ)
{
	blk64_t cur_بلاڪ, وٿي;
	بنية sparse_map *sm = channel->ذاتي_معلومات;

	إذا (check_بلاڪ_سائيز(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلاڪ_سائيز;

	لكل (بنانشان ڊگهو ڊگهو i = 0; i < ڳڻپ; ++i) {
		إذا (blk + i >= sm->بلاڪون_ڳڻپ)
			أرجع 0;
		cur_بلاڪ = بلاڪ_منزل_sparse_بلاڪ(blk + i, &وٿي, channel,
						  sm);
		إذا (!sm->بلاڪون[cur_بلاڪ])
			استمر;
		حرر(sm->بلاڪون[cur_بلاڪ]);
		sm->بلاڪون[cur_بلاڪ] = عدم;
	}
	أرجع 0;
}

ساكن errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				بنانشان ڊگهو ڊگهو ڳڻپ)
{
	أرجع sparse_discard(channel, blk, ڳڻپ);
}

ساكن errcode_t sparse_flush(io_channel channel __attribute__((اڻ_استعمال_ٿيل)))
{
	أرجع 0;
}

ساكن errcode_t sparse_سيٽ_option(io_channel channel __attribute__((اڻ_استعمال_ٿيل)),
                                   ثابت حرف *option __attribute__((اڻ_استعمال_ٿيل)),
                                   ثابت حرف *arg __attribute__((اڻ_استعمال_ٿيل)))
{
	أرجع 0;
}

ساكن errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((اڻ_استعمال_ٿيل)),
			blk64_t blk __attribute__((اڻ_استعمال_ٿيل)),
			بنانشان ڊگهو ڊگهو ڳڻپ __attribute__((اڻ_استعمال_ٿيل)))
{
	أرجع 0;
}

ساكن بنية struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.سيٽ_blksize		= sparse_سيٽ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= عدم,
	.سيٽ_option		= sparse_سيٽ_option,
	.get_stats		= عدم,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

ساكن بنية struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.سيٽ_blksize		= sparse_سيٽ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= عدم,
	.سيٽ_option		= sparse_سيٽ_option,
	.get_stats		= عدم,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#جيڪڏهن_پورو

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
