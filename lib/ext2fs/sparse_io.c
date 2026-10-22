#ضمن "config.h"
#ضمن <sys/types.h>
#ضمن <sys/stat.h>
#ضمن <fcntl.h>
#ضمن <unistd.h>
#ضمن <stdint.h>
#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#اگر_تعریف_نشده O_دودویی
#تعریف_کن O_دودویی 0
#پایان_اگر

#إذا !defined(ENABLE_LIBSPARSE)
ساكن errcode_t sparse_open(ثابت حرف *name EXT2FS_ATTR((استفادهنشده)),
			     صحيح flags EXT2FS_ATTR((استفادهنشده)),
			     io_channel *channel EXT2FS_ATTR((استفادهنشده)))
{
	أرجع EXT2_ET_UNIMPLEMENTED;
}
ساكن errcode_t sparse_close(io_channel channel EXT2FS_ATTR((استفادهنشده)))
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
	حرف			**بلوکها;
	صحيح			بلوک_اندازه;
	uint64_t		بلوکها_شمار;
	حرف			*file;
	بنية sparse_file	*sparse_file;
	io_channel		channel;
};

بنية sparse_io_params {
	صحيح			fd;
	حرف			*file;
	uint64_t		بلوکها_شمار;
	بیعلامت صحيح		بلوک_اندازه;
};

ساكن errcode_t sparse_write_blk(io_channel channel, بیعلامت دراز بلوک,
				  صحيح شمار, ثابت فراغ *buf);

ساكن فراغ free_sparse_بلوکها(بنية sparse_map *sm)
{
	uint64_t i;

	لكل (i = 0; i < sm->بلوکها_شمار; ++i)
		حرر(sm->بلوکها[i]);
	حرر(sm->بلوکها);
	sm->بلوکها = عدم;
}

ساكن صحيح sparse_import_segment(فراغ *priv, ثابت فراغ *داده, size_t len,
				 بیعلامت صحيح بلوک, بیعلامت صحيح nr_بلوکها)
{
	بنية sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	إذا (!nr_بلوکها || len % sm->بلوک_اندازه)
		أرجع 0;

	أرجع sparse_write_blk(sm->channel, بلوک, nr_بلوکها, داده);
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

	sm->بلوک_اندازه = sparse_file_بلوک_اندازه(sparse_file);
	sm->بلوکها_شمار = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->بلوک_اندازه + 1;
	sm->بلوکها = حافظه_صفر_بده(sm->بلوکها_شمار, حجم(حرف*));
	إذا (!sm->بلوکها) {
		retval = -1;
		اذهب err_alloc;
	}
	io->بلوک_اندازه = sm->بلوک_اندازه;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	إذا (retval)
		free_sparse_بلوکها(sm);
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
	بنية sparse_map *sm = حافظه_صفر_بده(1, حجم(*sm));
	إذا (!sm)
		أرجع EXT2_ET_NO_حافظه;

	sm->file = params->file;
	sm->channel = io;
	io->خصوصی_داده = sm;
	retval = io_manager_import_sparse(params, sm, io);
	إذا (retval) {
		إذا (!params->بلوک_اندازه || !params->بلوکها_شمار) {
			retval = EINVAL;
			اذهب err_params;
		}
		sm->بلوک_اندازه = params->بلوک_اندازه;
		sm->بلوکها_شمار = params->بلوکها_شمار;
		sm->بلوکها = حافظه_صفر_بده(params->بلوکها_شمار, حجم(فراغ*));
		إذا (!sm->بلوکها) {
			retval = EXT2_ET_NO_حافظه;
			اذهب err_alloc;
		}
	}
	io->بلوک_اندازه = sm->بلوک_اندازه;
	img_اندازه = (uint64_t)sm->بلوک_اندازه * sm->بلوکها_شمار;

	إذا (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->بلوک_اندازه, img_اندازه);
		إذا (!sm->sparse_file) {
			retval = EXT2_ET_NO_حافظه;
			اذهب err_alloc;
		}
		إذا (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_دودویی,
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
	free_sparse_بلوکها(sm);
err_params:
	حرر(sm);
	أرجع retval;
}

ساكن errcode_t sparse_open_channel(بنية sparse_io_params *sparse_params,
				     صحيح flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = حافظه_صفر_بده(1, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->بلوک_اندازه = 0;
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
	sparse_params->بلوک_اندازه = 0;
	sparse_params->بلوکها_شمار = 0;

	sparse_params->file = حافظه_بده(طول_رشته(name) + 1);
	إذا (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", طول_رشته(name) + 1);
		أرجع EXT2_ET_NO_حافظه;
	}

	إذا (is_fd) {
		ret = از_رشته_بخوان(name, "(%d):%llu:%u", &sparse_params->fd,
			     (بیعلامت دراز دراز *)&sparse_params->بلوکها_شمار,
			     &sparse_params->بلوک_اندازه);
	} وإلا {
		ret = از_رشته_بخوان(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (بیعلامت دراز دراز *)&sparse_params->بلوکها_شمار,
			     &sparse_params->بلوک_اندازه);
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

ساكن errcode_t sparse_ادغام_بلوکها(بنية sparse_map *sm, uint64_t شروع,
					uint64_t رقم)
{
	حرف *buf;
	uint64_t i;
	بیعلامت صحيح بلوک_اندازه = sm->بلوک_اندازه;
	errcode_t retval = 0;

	buf = حافظه_صفر_بده(رقم, بلوک_اندازه);
	إذا (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(بیعلامت دراز دراز)رقم * بلوک_اندازه);
		أرجع EXT2_ET_NO_حافظه;
	}

	لكل (i = 0; i < رقم; i++) {
		کپی_حافظه(buf + i * بلوک_اندازه, sm->بلوکها[شروع + i] , بلوک_اندازه);
		حرر(sm->بلوکها[شروع + i]);
		sm->بلوکها[شروع + i] = عدم;
	}

	/* free_sparse_blocks will release this buf. */
	sm->بلوکها[شروع] = buf;

	retval = sparse_file_جمع_کن_داده(sm->sparse_file, sm->بلوکها[شروع],
					بلوک_اندازه * رقم, شروع);

	أرجع retval;
}

ساكن errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	بنية sparse_map *sm = channel->خصوصی_داده;

	إذا (sm->sparse_file) {
		int64_t chunk_شروع = (sm->بلوکها[0] == عدم) ? -1 : 0;
		لكل (i = 0; i < sm->بلوکها_شمار; ++i) {
			إذا (!sm->بلوکها[i] && chunk_شروع != -1) {
				retval = sparse_ادغام_بلوکها(sm, chunk_شروع, i - chunk_شروع);
				chunk_شروع = -1;
			} وإلا إذا (sm->بلوکها[i] && chunk_شروع == -1) {
				chunk_شروع = i;
			}
			إذا (retval)
				اذهب ret;
		}
		إذا (chunk_شروع != -1) {
			retval = sparse_ادغام_بلوکها(sm, chunk_شروع,
							sm->بلوکها_شمار - chunk_شروع);
			إذا (retval)
				اذهب ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*بررسی_چرخهای*/0);
	}

ret:
	إذا (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_بلوکها(sm);
	حرر(sm->file);
	حرر(sm);
	حرر(channel);
	أرجع retval;
}

ساكن errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	بنية sparse_map *sm = channel->خصوصی_داده;
	صحيح fd = sm->fd;

	retval = sparse_close_channel(channel);
	إذا (fd >= 0)
		close(fd);

	أرجع retval;
}

ساكن errcode_t sparse_مجموعه_blksize(io_channel channel, صحيح blksize)
{
	channel->بلوک_اندازه = blksize;
	أرجع 0;
}

ساكن blk64_t بلوک_مقصد_sparse_بلوک(blk64_t بلوک, blk64_t *جابه_جایی,
			       io_channel channel, بنية sparse_map *sm)
{
	صحيح نسبت;
	blk64_t ret = بلوک;

	نسبت = sm->بلوک_اندازه / channel->بلوک_اندازه;
	ret /= نسبت;
	*جابه_جایی = (بلوک % نسبت) * channel->بلوک_اندازه;

	أرجع ret;
}

ساكن errcode_t check_بلوک_اندازه(io_channel channel, بنية sparse_map *sm)
{
	إذا (sm->بلوک_اندازه >= channel->بلوک_اندازه)
		أرجع 0;
	أرجع EXT2_ET_UNEXPECTED_بلوک_اندازه;
}

ساكن errcode_t sparse_read_blk64(io_channel channel, blk64_t بلوک,
				   صحيح شمار, فراغ *buf)
{
	صحيح i;
	حرف *بیرون = buf;
	blk64_t جابه_جایی = 0, cur_بلوک;
	بنية sparse_map *sm = channel->خصوصی_داده;

	إذا (check_بلوک_اندازه(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلوک_اندازه;

	إذا (شمار < 0) { //partial read
		شمار = -شمار;
		cur_بلوک = بلوک_مقصد_sparse_بلوک(بلوک, &جابه_جایی, channel, sm);
		إذا (sm->بلوکها[cur_بلوک])
			کپی_حافظه(بیرون, (sm->بلوکها[cur_بلوک]) + جابه_جایی, شمار);
		وإلا
			memset(بیرون, 0, شمار);
	} وإلا {
		لكل (i = 0; i < شمار; ++i) {
			cur_بلوک = بلوک_مقصد_sparse_بلوک(بلوک + i, &جابه_جایی,
						    channel, sm);
			إذا (sm->بلوکها[cur_بلوک])
				کپی_حافظه(بیرون + (i * channel->بلوک_اندازه),
				       sm->بلوکها[cur_بلوک] + جابه_جایی,
				       channel->بلوک_اندازه);
			وإلا إذا (sm->بلوکها)
				memset(بیرون + (i * channel->بلوک_اندازه), 0,
				       channel->بلوک_اندازه);
		}
	}
	أرجع 0;
}

ساكن errcode_t sparse_read_blk(io_channel channel, بیعلامت دراز بلوک,
				 صحيح شمار, فراغ *buf)
{
	أرجع sparse_read_blk64(channel, بلوک, شمار, buf);
}

ساكن errcode_t sparse_write_blk64(io_channel channel, blk64_t بلوک,
				    صحيح شمار, ثابت فراغ *buf)
{
	صحيح i;
	blk64_t جابه_جایی = 0, cur_بلوک;
	ثابت حرف *درون = buf;
	بنية sparse_map *sm = channel->خصوصی_داده;

	إذا (check_بلوک_اندازه(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلوک_اندازه;

	إذا (شمار < 0) { //partial write
		شمار = -شمار;
		cur_بلوک = بلوک_مقصد_sparse_بلوک(بلوک, &جابه_جایی, channel,
						  sm);
		إذا (!sm->بلوکها[cur_بلوک]) {
			sm->بلوکها[cur_بلوک] = حافظه_صفر_بده(1, sm->بلوک_اندازه);
			إذا (!sm->بلوکها[cur_بلوک])
				أرجع EXT2_ET_NO_حافظه;
		}
		کپی_حافظه(sm->بلوکها[cur_بلوک] + جابه_جایی, درون, شمار);
	} وإلا {
		لكل (i = 0; i < شمار; ++i) {
			إذا (بلوک + i >= sm->بلوکها_شمار)
				أرجع 0;
			cur_بلوک = بلوک_مقصد_sparse_بلوک(بلوک + i, &جابه_جایی,
						    channel, sm);
			إذا (!sm->بلوکها[cur_بلوک]) {
				sm->بلوکها[cur_بلوک] =
					حافظه_صفر_بده(1, sm->بلوک_اندازه);
				إذا (!sm->بلوکها[cur_بلوک])
					أرجع EXT2_ET_NO_حافظه;
			}
			کپی_حافظه(sm->بلوکها[cur_بلوک] + جابه_جایی,
			       درون + (i * channel->بلوک_اندازه),
			       channel->بلوک_اندازه);
		}
	}
	أرجع 0;
}

ساكن errcode_t sparse_write_blk(io_channel channel, بیعلامت دراز بلوک,
				  صحيح شمار, ثابت فراغ *buf)
{
	أرجع sparse_write_blk64(channel, بلوک, شمار, buf);
}

ساكن errcode_t sparse_discard(io_channel channel __attribute__((استفادهنشده)),
				blk64_t blk, بیعلامت دراز دراز شمار)
{
	blk64_t cur_بلوک, جابه_جایی;
	بنية sparse_map *sm = channel->خصوصی_داده;

	إذا (check_بلوک_اندازه(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلوک_اندازه;

	لكل (بیعلامت دراز دراز i = 0; i < شمار; ++i) {
		إذا (blk + i >= sm->بلوکها_شمار)
			أرجع 0;
		cur_بلوک = بلوک_مقصد_sparse_بلوک(blk + i, &جابه_جایی, channel,
						  sm);
		إذا (!sm->بلوکها[cur_بلوک])
			استمر;
		حرر(sm->بلوکها[cur_بلوک]);
		sm->بلوکها[cur_بلوک] = عدم;
	}
	أرجع 0;
}

ساكن errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				بیعلامت دراز دراز شمار)
{
	أرجع sparse_discard(channel, blk, شمار);
}

ساكن errcode_t sparse_flush(io_channel channel __attribute__((استفادهنشده)))
{
	أرجع 0;
}

ساكن errcode_t sparse_مجموعه_option(io_channel channel __attribute__((استفادهنشده)),
                                   ثابت حرف *option __attribute__((استفادهنشده)),
                                   ثابت حرف *arg __attribute__((استفادهنشده)))
{
	أرجع 0;
}

ساكن errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((استفادهنشده)),
			blk64_t blk __attribute__((استفادهنشده)),
			بیعلامت دراز دراز شمار __attribute__((استفادهنشده)))
{
	أرجع 0;
}

ساكن بنية struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.مجموعه_blksize		= sparse_مجموعه_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= عدم,
	.مجموعه_option		= sparse_مجموعه_option,
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
	.مجموعه_blksize		= sparse_مجموعه_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= عدم,
	.مجموعه_option		= sparse_مجموعه_option,
	.get_stats		= عدم,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#پایان_اگر

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
