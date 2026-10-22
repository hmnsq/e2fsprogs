#ضمن "config.h"
#ضمن <sys/types.h>
#ضمن <sys/stat.h>
#ضمن <fcntl.h>
#ضمن <unistd.h>
#ضمن <stdint.h>
#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#که_نه_تعریف_شوی O_دوګونی
#تعریف_کړه O_دوګونی 0
#د_که_پای

#إذا !defined(ENABLE_LIBSPARSE)
ساكن errcode_t sparse_open(ثابت حرف *name EXT2FS_ATTR((ناکارول_شوی)),
			     صحيح flags EXT2FS_ATTR((ناکارول_شوی)),
			     io_channel *channel EXT2FS_ATTR((ناکارول_شوی)))
{
	أرجع EXT2_ET_UNIMPLEMENTED;
}
ساكن errcode_t sparse_close(io_channel channel EXT2FS_ATTR((ناکارول_شوی)))
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
	حرف			**بلاکونه;
	صحيح			بلاک_اندازه;
	uint64_t		بلاکونه_شمېر;
	حرف			*file;
	بنية sparse_file	*sparse_file;
	io_channel		channel;
};

بنية sparse_io_params {
	صحيح			fd;
	حرف			*file;
	uint64_t		بلاکونه_شمېر;
	بینښه صحيح		بلاک_اندازه;
};

ساكن errcode_t sparse_write_blk(io_channel channel, بینښه اوږد بلاک,
				  صحيح شمېر, ثابت فراغ *buf);

ساكن فراغ free_sparse_بلاکونه(بنية sparse_map *sm)
{
	uint64_t i;

	لكل (i = 0; i < sm->بلاکونه_شمېر; ++i)
		حرر(sm->بلاکونه[i]);
	حرر(sm->بلاکونه);
	sm->بلاکونه = عدم;
}

ساكن صحيح sparse_import_segment(فراغ *priv, ثابت فراغ *معلومات, size_t len,
				 بینښه صحيح بلاک, بینښه صحيح nr_بلاکونه)
{
	بنية sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	إذا (!nr_بلاکونه || len % sm->بلاک_اندازه)
		أرجع 0;

	أرجع sparse_write_blk(sm->channel, بلاک, nr_بلاکونه, معلومات);
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

	sm->بلاک_اندازه = sparse_file_بلاک_اندازه(sparse_file);
	sm->بلاکونه_شمېر = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->بلاک_اندازه + 1;
	sm->بلاکونه = حافظه_صفر_ورکړه(sm->بلاکونه_شمېر, حجم(حرف*));
	إذا (!sm->بلاکونه) {
		retval = -1;
		اذهب err_alloc;
	}
	io->بلاک_اندازه = sm->بلاک_اندازه;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	إذا (retval)
		free_sparse_بلاکونه(sm);
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
	uint64_t img_اندازه;
	بنية sparse_map *sm = حافظه_صفر_ورکړه(1, حجم(*sm));
	إذا (!sm)
		أرجع EXT2_ET_NO_حافظه;

	sm->file = params->file;
	sm->channel = io;
	io->شخصي_معلومات = sm;
	retval = io_manager_import_sparse(params, sm, io);
	إذا (retval) {
		إذا (!params->بلاک_اندازه || !params->بلاکونه_شمېر) {
			retval = EINVAL;
			اذهب err_params;
		}
		sm->بلاک_اندازه = params->بلاک_اندازه;
		sm->بلاکونه_شمېر = params->بلاکونه_شمېر;
		sm->بلاکونه = حافظه_صفر_ورکړه(params->بلاکونه_شمېر, حجم(فراغ*));
		إذا (!sm->بلاکونه) {
			retval = EXT2_ET_NO_حافظه;
			اذهب err_alloc;
		}
	}
	io->بلاک_اندازه = sm->بلاک_اندازه;
	img_اندازه = (uint64_t)sm->بلاک_اندازه * sm->بلاکونه_شمېر;

	إذا (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->بلاک_اندازه, img_اندازه);
		إذا (!sm->sparse_file) {
			retval = EXT2_ET_NO_حافظه;
			اذهب err_alloc;
		}
		إذا (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_دوګونی,
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
	free_sparse_بلاکونه(sm);
err_params:
	حرر(sm);
	أرجع retval;
}

ساكن errcode_t sparse_open_channel(بنية sparse_io_params *sparse_params,
				     صحيح flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = حافظه_صفر_ورکړه(1, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->بلاک_اندازه = 0;
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
	sparse_params->بلاک_اندازه = 0;
	sparse_params->بلاکونه_شمېر = 0;

	sparse_params->file = حافظه_ورکړه(مزي_اوږدوالی(name) + 1);
	إذا (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", مزي_اوږدوالی(name) + 1);
		أرجع EXT2_ET_NO_حافظه;
	}

	إذا (is_fd) {
		ret = له_مزي_ولوله(name, "(%d):%llu:%u", &sparse_params->fd,
			     (بینښه اوږد اوږد *)&sparse_params->بلاکونه_شمېر,
			     &sparse_params->بلاک_اندازه);
	} وإلا {
		ret = له_مزي_ولوله(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (بینښه اوږد اوږد *)&sparse_params->بلاکونه_شمېر,
			     &sparse_params->بلاک_اندازه);
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

ساكن errcode_t sparse_یوځای_بلاکونه(بنية sparse_map *sm, uint64_t پیل,
					uint64_t عدد)
{
	حرف *buf;
	uint64_t i;
	بینښه صحيح بلاک_اندازه = sm->بلاک_اندازه;
	errcode_t retval = 0;

	buf = حافظه_صفر_ورکړه(عدد, بلاک_اندازه);
	إذا (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(بینښه اوږد اوږد)عدد * بلاک_اندازه);
		أرجع EXT2_ET_NO_حافظه;
	}

	لكل (i = 0; i < عدد; i++) {
		حافظه_نقل(buf + i * بلاک_اندازه, sm->بلاکونه[پیل + i] , بلاک_اندازه);
		حرر(sm->بلاکونه[پیل + i]);
		sm->بلاکونه[پیل + i] = عدم;
	}

	/* free_sparse_blocks will release this buf. */
	sm->بلاکونه[پیل] = buf;

	retval = sparse_file_جمع_کړه_معلومات(sm->sparse_file, sm->بلاکونه[پیل],
					بلاک_اندازه * عدد, پیل);

	أرجع retval;
}

ساكن errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	بنية sparse_map *sm = channel->شخصي_معلومات;

	إذا (sm->sparse_file) {
		int64_t chunk_پیل = (sm->بلاکونه[0] == عدم) ? -1 : 0;
		لكل (i = 0; i < sm->بلاکونه_شمېر; ++i) {
			إذا (!sm->بلاکونه[i] && chunk_پیل != -1) {
				retval = sparse_یوځای_بلاکونه(sm, chunk_پیل, i - chunk_پیل);
				chunk_پیل = -1;
			} وإلا إذا (sm->بلاکونه[i] && chunk_پیل == -1) {
				chunk_پیل = i;
			}
			إذا (retval)
				اذهب ret;
		}
		إذا (chunk_پیل != -1) {
			retval = sparse_یوځای_بلاکونه(sm, chunk_پیل,
							sm->بلاکونه_شمېر - chunk_پیل);
			إذا (retval)
				اذهب ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*دوراني_ازموینه*/0);
	}

ret:
	إذا (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_بلاکونه(sm);
	حرر(sm->file);
	حرر(sm);
	حرر(channel);
	أرجع retval;
}

ساكن errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	بنية sparse_map *sm = channel->شخصي_معلومات;
	صحيح fd = sm->fd;

	retval = sparse_close_channel(channel);
	إذا (fd >= 0)
		close(fd);

	أرجع retval;
}

ساكن errcode_t sparse_سېټ_blksize(io_channel channel, صحيح blksize)
{
	channel->بلاک_اندازه = blksize;
	أرجع 0;
}

ساكن blk64_t بلاک_منزل_sparse_بلاک(blk64_t بلاک, blk64_t *دننه_واټن,
			       io_channel channel, بنية sparse_map *sm)
{
	صحيح نسبت;
	blk64_t ret = بلاک;

	نسبت = sm->بلاک_اندازه / channel->بلاک_اندازه;
	ret /= نسبت;
	*دننه_واټن = (بلاک % نسبت) * channel->بلاک_اندازه;

	أرجع ret;
}

ساكن errcode_t check_بلاک_اندازه(io_channel channel, بنية sparse_map *sm)
{
	إذا (sm->بلاک_اندازه >= channel->بلاک_اندازه)
		أرجع 0;
	أرجع EXT2_ET_UNEXPECTED_بلاک_اندازه;
}

ساكن errcode_t sparse_read_blk64(io_channel channel, blk64_t بلاک,
				   صحيح شمېر, فراغ *buf)
{
	صحيح i;
	حرف *بهر = buf;
	blk64_t دننه_واټن = 0, cur_بلاک;
	بنية sparse_map *sm = channel->شخصي_معلومات;

	إذا (check_بلاک_اندازه(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلاک_اندازه;

	إذا (شمېر < 0) { //partial read
		شمېر = -شمېر;
		cur_بلاک = بلاک_منزل_sparse_بلاک(بلاک, &دننه_واټن, channel, sm);
		إذا (sm->بلاکونه[cur_بلاک])
			حافظه_نقل(بهر, (sm->بلاکونه[cur_بلاک]) + دننه_واټن, شمېر);
		وإلا
			memset(بهر, 0, شمېر);
	} وإلا {
		لكل (i = 0; i < شمېر; ++i) {
			cur_بلاک = بلاک_منزل_sparse_بلاک(بلاک + i, &دننه_واټن,
						    channel, sm);
			إذا (sm->بلاکونه[cur_بلاک])
				حافظه_نقل(بهر + (i * channel->بلاک_اندازه),
				       sm->بلاکونه[cur_بلاک] + دننه_واټن,
				       channel->بلاک_اندازه);
			وإلا إذا (sm->بلاکونه)
				memset(بهر + (i * channel->بلاک_اندازه), 0,
				       channel->بلاک_اندازه);
		}
	}
	أرجع 0;
}

ساكن errcode_t sparse_read_blk(io_channel channel, بینښه اوږد بلاک,
				 صحيح شمېر, فراغ *buf)
{
	أرجع sparse_read_blk64(channel, بلاک, شمېر, buf);
}

ساكن errcode_t sparse_write_blk64(io_channel channel, blk64_t بلاک,
				    صحيح شمېر, ثابت فراغ *buf)
{
	صحيح i;
	blk64_t دننه_واټن = 0, cur_بلاک;
	ثابت حرف *دننه = buf;
	بنية sparse_map *sm = channel->شخصي_معلومات;

	إذا (check_بلاک_اندازه(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلاک_اندازه;

	إذا (شمېر < 0) { //partial write
		شمېر = -شمېر;
		cur_بلاک = بلاک_منزل_sparse_بلاک(بلاک, &دننه_واټن, channel,
						  sm);
		إذا (!sm->بلاکونه[cur_بلاک]) {
			sm->بلاکونه[cur_بلاک] = حافظه_صفر_ورکړه(1, sm->بلاک_اندازه);
			إذا (!sm->بلاکونه[cur_بلاک])
				أرجع EXT2_ET_NO_حافظه;
		}
		حافظه_نقل(sm->بلاکونه[cur_بلاک] + دننه_واټن, دننه, شمېر);
	} وإلا {
		لكل (i = 0; i < شمېر; ++i) {
			إذا (بلاک + i >= sm->بلاکونه_شمېر)
				أرجع 0;
			cur_بلاک = بلاک_منزل_sparse_بلاک(بلاک + i, &دننه_واټن,
						    channel, sm);
			إذا (!sm->بلاکونه[cur_بلاک]) {
				sm->بلاکونه[cur_بلاک] =
					حافظه_صفر_ورکړه(1, sm->بلاک_اندازه);
				إذا (!sm->بلاکونه[cur_بلاک])
					أرجع EXT2_ET_NO_حافظه;
			}
			حافظه_نقل(sm->بلاکونه[cur_بلاک] + دننه_واټن,
			       دننه + (i * channel->بلاک_اندازه),
			       channel->بلاک_اندازه);
		}
	}
	أرجع 0;
}

ساكن errcode_t sparse_write_blk(io_channel channel, بینښه اوږد بلاک,
				  صحيح شمېر, ثابت فراغ *buf)
{
	أرجع sparse_write_blk64(channel, بلاک, شمېر, buf);
}

ساكن errcode_t sparse_discard(io_channel channel __attribute__((ناکارول_شوی)),
				blk64_t blk, بینښه اوږد اوږد شمېر)
{
	blk64_t cur_بلاک, دننه_واټن;
	بنية sparse_map *sm = channel->شخصي_معلومات;

	إذا (check_بلاک_اندازه(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلاک_اندازه;

	لكل (بینښه اوږد اوږد i = 0; i < شمېر; ++i) {
		إذا (blk + i >= sm->بلاکونه_شمېر)
			أرجع 0;
		cur_بلاک = بلاک_منزل_sparse_بلاک(blk + i, &دننه_واټن, channel,
						  sm);
		إذا (!sm->بلاکونه[cur_بلاک])
			استمر;
		حرر(sm->بلاکونه[cur_بلاک]);
		sm->بلاکونه[cur_بلاک] = عدم;
	}
	أرجع 0;
}

ساكن errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				بینښه اوږد اوږد شمېر)
{
	أرجع sparse_discard(channel, blk, شمېر);
}

ساكن errcode_t sparse_flush(io_channel channel __attribute__((ناکارول_شوی)))
{
	أرجع 0;
}

ساكن errcode_t sparse_سېټ_option(io_channel channel __attribute__((ناکارول_شوی)),
                                   ثابت حرف *option __attribute__((ناکارول_شوی)),
                                   ثابت حرف *arg __attribute__((ناکارول_شوی)))
{
	أرجع 0;
}

ساكن errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((ناکارول_شوی)),
			blk64_t blk __attribute__((ناکارول_شوی)),
			بینښه اوږد اوږد شمېر __attribute__((ناکارول_شوی)))
{
	أرجع 0;
}

ساكن بنية struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.سېټ_blksize		= sparse_سېټ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= عدم,
	.سېټ_option		= sparse_سېټ_option,
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
	.سېټ_blksize		= sparse_سېټ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= عدم,
	.سېټ_option		= sparse_سېټ_option,
	.get_stats		= عدم,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#د_که_پای

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
