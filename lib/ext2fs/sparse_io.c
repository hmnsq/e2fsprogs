#ضمن "config.h"
#ضمن <sys/types.h>
#ضمن <sys/stat.h>
#ضمن <fcntl.h>
#ضمن <unistd.h>
#ضمن <stdint.h>
#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#جے_بنا_تعریف O_ثنائی
#تعریف_کرو O_ثنائی 0
#جے_انت

#إذا !defined(ENABLE_LIBSPARSE)
ساكن errcode_t sparse_open(ثابت حرف *name EXT2FS_ATTR((بن_ورتیا)),
			     صحيح flags EXT2FS_ATTR((بن_ورتیا)),
			     io_channel *channel EXT2FS_ATTR((بن_ورتیا)))
{
	أرجع EXT2_ET_UNIMPLEMENTED;
}
ساكن errcode_t sparse_close(io_channel channel EXT2FS_ATTR((بن_ورتیا)))
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
	حرف			**بلاکاں;
	صحيح			بلاک_ناپ;
	uint64_t		بلاکاں_گنتی;
	حرف			*file;
	بنية sparse_file	*sparse_file;
	io_channel		channel;
};

بنية sparse_io_params {
	صحيح			fd;
	حرف			*file;
	uint64_t		بلاکاں_گنتی;
	بنانشانی صحيح		بلاک_ناپ;
};

ساكن errcode_t sparse_write_blk(io_channel channel, بنانشانی لما بلاک,
				  صحيح گنتی, ثابت فراغ *buf);

ساكن فراغ free_sparse_بلاکاں(بنية sparse_map *sm)
{
	uint64_t i;

	لكل (i = 0; i < sm->بلاکاں_گنتی; ++i)
		حرر(sm->بلاکاں[i]);
	حرر(sm->بلاکاں);
	sm->بلاکاں = عدم;
}

ساكن صحيح sparse_import_segment(فراغ *priv, ثابت فراغ *انکڑے, size_t len,
				 بنانشانی صحيح بلاک, بنانشانی صحيح nr_بلاکاں)
{
	بنية sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	إذا (!nr_بلاکاں || len % sm->بلاک_ناپ)
		أرجع 0;

	أرجع sparse_write_blk(sm->channel, بلاک, nr_بلاکاں, انکڑے);
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

	sm->بلاک_ناپ = sparse_file_بلاک_ناپ(sparse_file);
	sm->بلاکاں_گنتی = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->بلاک_ناپ + 1;
	sm->بلاکاں = صفر_حافظہ_دیو(sm->بلاکاں_گنتی, حجم(حرف*));
	إذا (!sm->بلاکاں) {
		retval = -1;
		اذهب err_alloc;
	}
	io->بلاک_ناپ = sm->بلاک_ناپ;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	إذا (retval)
		free_sparse_بلاکاں(sm);
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
	uint64_t img_ناپ;
	بنية sparse_map *sm = صفر_حافظہ_دیو(1, حجم(*sm));
	إذا (!sm)
		أرجع EXT2_ET_NO_حافظہ;

	sm->file = params->file;
	sm->channel = io;
	io->نجی_انکڑے = sm;
	retval = io_manager_import_sparse(params, sm, io);
	إذا (retval) {
		إذا (!params->بلاک_ناپ || !params->بلاکاں_گنتی) {
			retval = EINVAL;
			اذهب err_params;
		}
		sm->بلاک_ناپ = params->بلاک_ناپ;
		sm->بلاکاں_گنتی = params->بلاکاں_گنتی;
		sm->بلاکاں = صفر_حافظہ_دیو(params->بلاکاں_گنتی, حجم(فراغ*));
		إذا (!sm->بلاکاں) {
			retval = EXT2_ET_NO_حافظہ;
			اذهب err_alloc;
		}
	}
	io->بلاک_ناپ = sm->بلاک_ناپ;
	img_ناپ = (uint64_t)sm->بلاک_ناپ * sm->بلاکاں_گنتی;

	إذا (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->بلاک_ناپ, img_ناپ);
		إذا (!sm->sparse_file) {
			retval = EXT2_ET_NO_حافظہ;
			اذهب err_alloc;
		}
		إذا (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_ثنائی,
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
	free_sparse_بلاکاں(sm);
err_params:
	حرر(sm);
	أرجع retval;
}

ساكن errcode_t sparse_open_channel(بنية sparse_io_params *sparse_params,
				     صحيح flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = صفر_حافظہ_دیو(1, حجم(بنية struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->بلاک_ناپ = 0;
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
	sparse_params->بلاک_ناپ = 0;
	sparse_params->بلاکاں_گنتی = 0;

	sparse_params->file = حافظہ_دیو(لڑی_لمبائی(name) + 1);
	إذا (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", لڑی_لمبائی(name) + 1);
		أرجع EXT2_ET_NO_حافظہ;
	}

	إذا (is_fd) {
		ret = لڑی_توں_پڑھو(name, "(%d):%llu:%u", &sparse_params->fd,
			     (بنانشانی لما لما *)&sparse_params->بلاکاں_گنتی,
			     &sparse_params->بلاک_ناپ);
	} وإلا {
		ret = لڑی_توں_پڑھو(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (بنانشانی لما لما *)&sparse_params->بلاکاں_گنتی,
			     &sparse_params->بلاک_ناپ);
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

ساكن errcode_t sparse_ملاؤ_بلاکاں(بنية sparse_map *sm, uint64_t شروع,
					uint64_t عدد)
{
	حرف *buf;
	uint64_t i;
	بنانشانی صحيح بلاک_ناپ = sm->بلاک_ناپ;
	errcode_t retval = 0;

	buf = صفر_حافظہ_دیو(عدد, بلاک_ناپ);
	إذا (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(بنانشانی لما لما)عدد * بلاک_ناپ);
		أرجع EXT2_ET_NO_حافظہ;
	}

	لكل (i = 0; i < عدد; i++) {
		حافظہ_نقل(buf + i * بلاک_ناپ, sm->بلاکاں[شروع + i] , بلاک_ناپ);
		حرر(sm->بلاکاں[شروع + i]);
		sm->بلاکاں[شروع + i] = عدم;
	}

	/* free_sparse_blocks will release this buf. */
	sm->بلاکاں[شروع] = buf;

	retval = sparse_file_جوڑو_انکڑے(sm->sparse_file, sm->بلاکاں[شروع],
					بلاک_ناپ * عدد, شروع);

	أرجع retval;
}

ساكن errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	بنية sparse_map *sm = channel->نجی_انکڑے;

	إذا (sm->sparse_file) {
		int64_t chunk_شروع = (sm->بلاکاں[0] == عدم) ? -1 : 0;
		لكل (i = 0; i < sm->بلاکاں_گنتی; ++i) {
			إذا (!sm->بلاکاں[i] && chunk_شروع != -1) {
				retval = sparse_ملاؤ_بلاکاں(sm, chunk_شروع, i - chunk_شروع);
				chunk_شروع = -1;
			} وإلا إذا (sm->بلاکاں[i] && chunk_شروع == -1) {
				chunk_شروع = i;
			}
			إذا (retval)
				اذهب ret;
		}
		إذا (chunk_شروع != -1) {
			retval = sparse_ملاؤ_بلاکاں(sm, chunk_شروع,
							sm->بلاکاں_گنتی - chunk_شروع);
			إذا (retval)
				اذهب ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*چکری_جانچ*/0);
	}

ret:
	إذا (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_بلاکاں(sm);
	حرر(sm->file);
	حرر(sm);
	حرر(channel);
	أرجع retval;
}

ساكن errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	بنية sparse_map *sm = channel->نجی_انکڑے;
	صحيح fd = sm->fd;

	retval = sparse_close_channel(channel);
	إذا (fd >= 0)
		close(fd);

	أرجع retval;
}

ساكن errcode_t sparse_سیٹ_blksize(io_channel channel, صحيح blksize)
{
	channel->بلاک_ناپ = blksize;
	أرجع 0;
}

ساكن blk64_t بلاک_منزل_sparse_بلاک(blk64_t بلاک, blk64_t *وتھ,
			       io_channel channel, بنية sparse_map *sm)
{
	صحيح تناسب;
	blk64_t ret = بلاک;

	تناسب = sm->بلاک_ناپ / channel->بلاک_ناپ;
	ret /= تناسب;
	*وتھ = (بلاک % تناسب) * channel->بلاک_ناپ;

	أرجع ret;
}

ساكن errcode_t check_بلاک_ناپ(io_channel channel, بنية sparse_map *sm)
{
	إذا (sm->بلاک_ناپ >= channel->بلاک_ناپ)
		أرجع 0;
	أرجع EXT2_ET_UNEXPECTED_بلاک_ناپ;
}

ساكن errcode_t sparse_read_blk64(io_channel channel, blk64_t بلاک,
				   صحيح گنتی, فراغ *buf)
{
	صحيح i;
	حرف *باہرلا = buf;
	blk64_t وتھ = 0, cur_بلاک;
	بنية sparse_map *sm = channel->نجی_انکڑے;

	إذا (check_بلاک_ناپ(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلاک_ناپ;

	إذا (گنتی < 0) { //partial read
		گنتی = -گنتی;
		cur_بلاک = بلاک_منزل_sparse_بلاک(بلاک, &وتھ, channel, sm);
		إذا (sm->بلاکاں[cur_بلاک])
			حافظہ_نقل(باہرلا, (sm->بلاکاں[cur_بلاک]) + وتھ, گنتی);
		وإلا
			memset(باہرلا, 0, گنتی);
	} وإلا {
		لكل (i = 0; i < گنتی; ++i) {
			cur_بلاک = بلاک_منزل_sparse_بلاک(بلاک + i, &وتھ,
						    channel, sm);
			إذا (sm->بلاکاں[cur_بلاک])
				حافظہ_نقل(باہرلا + (i * channel->بلاک_ناپ),
				       sm->بلاکاں[cur_بلاک] + وتھ,
				       channel->بلاک_ناپ);
			وإلا إذا (sm->بلاکاں)
				memset(باہرلا + (i * channel->بلاک_ناپ), 0,
				       channel->بلاک_ناپ);
		}
	}
	أرجع 0;
}

ساكن errcode_t sparse_read_blk(io_channel channel, بنانشانی لما بلاک,
				 صحيح گنتی, فراغ *buf)
{
	أرجع sparse_read_blk64(channel, بلاک, گنتی, buf);
}

ساكن errcode_t sparse_write_blk64(io_channel channel, blk64_t بلاک,
				    صحيح گنتی, ثابت فراغ *buf)
{
	صحيح i;
	blk64_t وتھ = 0, cur_بلاک;
	ثابت حرف *وچ = buf;
	بنية sparse_map *sm = channel->نجی_انکڑے;

	إذا (check_بلاک_ناپ(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلاک_ناپ;

	إذا (گنتی < 0) { //partial write
		گنتی = -گنتی;
		cur_بلاک = بلاک_منزل_sparse_بلاک(بلاک, &وتھ, channel,
						  sm);
		إذا (!sm->بلاکاں[cur_بلاک]) {
			sm->بلاکاں[cur_بلاک] = صفر_حافظہ_دیو(1, sm->بلاک_ناپ);
			إذا (!sm->بلاکاں[cur_بلاک])
				أرجع EXT2_ET_NO_حافظہ;
		}
		حافظہ_نقل(sm->بلاکاں[cur_بلاک] + وتھ, وچ, گنتی);
	} وإلا {
		لكل (i = 0; i < گنتی; ++i) {
			إذا (بلاک + i >= sm->بلاکاں_گنتی)
				أرجع 0;
			cur_بلاک = بلاک_منزل_sparse_بلاک(بلاک + i, &وتھ,
						    channel, sm);
			إذا (!sm->بلاکاں[cur_بلاک]) {
				sm->بلاکاں[cur_بلاک] =
					صفر_حافظہ_دیو(1, sm->بلاک_ناپ);
				إذا (!sm->بلاکاں[cur_بلاک])
					أرجع EXT2_ET_NO_حافظہ;
			}
			حافظہ_نقل(sm->بلاکاں[cur_بلاک] + وتھ,
			       وچ + (i * channel->بلاک_ناپ),
			       channel->بلاک_ناپ);
		}
	}
	أرجع 0;
}

ساكن errcode_t sparse_write_blk(io_channel channel, بنانشانی لما بلاک,
				  صحيح گنتی, ثابت فراغ *buf)
{
	أرجع sparse_write_blk64(channel, بلاک, گنتی, buf);
}

ساكن errcode_t sparse_discard(io_channel channel __attribute__((بن_ورتیا)),
				blk64_t blk, بنانشانی لما لما گنتی)
{
	blk64_t cur_بلاک, وتھ;
	بنية sparse_map *sm = channel->نجی_انکڑے;

	إذا (check_بلاک_ناپ(channel, sm))
		أرجع EXT2_ET_UNEXPECTED_بلاک_ناپ;

	لكل (بنانشانی لما لما i = 0; i < گنتی; ++i) {
		إذا (blk + i >= sm->بلاکاں_گنتی)
			أرجع 0;
		cur_بلاک = بلاک_منزل_sparse_بلاک(blk + i, &وتھ, channel,
						  sm);
		إذا (!sm->بلاکاں[cur_بلاک])
			استمر;
		حرر(sm->بلاکاں[cur_بلاک]);
		sm->بلاکاں[cur_بلاک] = عدم;
	}
	أرجع 0;
}

ساكن errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				بنانشانی لما لما گنتی)
{
	أرجع sparse_discard(channel, blk, گنتی);
}

ساكن errcode_t sparse_flush(io_channel channel __attribute__((بن_ورتیا)))
{
	أرجع 0;
}

ساكن errcode_t sparse_سیٹ_option(io_channel channel __attribute__((بن_ورتیا)),
                                   ثابت حرف *option __attribute__((بن_ورتیا)),
                                   ثابت حرف *arg __attribute__((بن_ورتیا)))
{
	أرجع 0;
}

ساكن errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((بن_ورتیا)),
			blk64_t blk __attribute__((بن_ورتیا)),
			بنانشانی لما لما گنتی __attribute__((بن_ورتیا)))
{
	أرجع 0;
}

ساكن بنية struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.سیٹ_blksize		= sparse_سیٹ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= عدم,
	.سیٹ_option		= sparse_سیٹ_option,
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
	.سیٹ_blksize		= sparse_سیٹ_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= عدم,
	.سیٹ_option		= sparse_سیٹ_option,
	.get_stats		= عدم,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#جے_انت

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
