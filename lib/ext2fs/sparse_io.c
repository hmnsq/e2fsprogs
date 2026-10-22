#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

#ifndef O_દ્વિઅંકી
#define O_દ્વિઅંકી 0
#endif

#જો !defined(ENABLE_LIBSPARSE)
સ્થિર errcode_t sparse_open(અચલ અક્ષર *name EXT2FS_ATTR((unused)),
			     પૂર્ણાંક flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	પાછા EXT2_ET_UNIMPLEMENTED;
}
સ્થિર errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	પાછા EXT2_ET_UNIMPLEMENTED;
}
સ્થિર સંરચના struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
સ્થિર સંરચના struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#નહીંતર
#અન્તર્ભાવ <sparse/sparse.h>

સંરચના sparse_map {
	પૂર્ણાંક			fd;
	અક્ષર			**blocks;
	પૂર્ણાંક			block_size;
	uint64_t		blocks_ગણતરી;
	અક્ષર			*file;
	સંરચના sparse_file	*sparse_file;
	io_channel		channel;
};

સંરચના sparse_io_params {
	પૂર્ણાંક			fd;
	અક્ષર			*file;
	uint64_t		blocks_ગણતરી;
	unsigned પૂર્ણાંક		block_size;
};

સ્થિર errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf);

સ્થિર શૂન્ય free_sparse_blocks(સંરચના sparse_map *sm)
{
	uint64_t i;

	માટે (i = 0; i < sm->blocks_ગણતરી; ++i)
		મુક્ત(sm->blocks[i]);
	મુક્ત(sm->blocks);
	sm->blocks = ખાલી;
}

સ્થિર પૂર્ણાંક sparse_import_segment(શૂન્ય *priv, અચલ શૂન્ય *data, size_t len,
				 unsigned પૂર્ણાંક block, unsigned પૂર્ણાંક nr_blocks)
{
	સંરચના sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	જો (!nr_blocks || len % sm->block_size)
		પાછા 0;

	પાછા sparse_write_blk(sm->channel, block, nr_blocks, data);
}

સ્થિર errcode_t io_manager_import_sparse(સંરચના sparse_io_params *params,
					  સંરચના sparse_map *sm, io_channel io)
{
	પૂર્ણાંક fd;
	errcode_t retval;
	સંરચના sparse_file *sparse_file;

	જો (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		જો (fd < 0) {
			retval = -1;
			જાઓ err_open;
		}
	} નહીંતર
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	જો (!sparse_file) {
		retval = -1;
		જાઓ err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_ગણતરી = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_ગણતરી, કદ(અક્ષર*));
	જો (!sm->blocks) {
		retval = -1;
		જાઓ err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	જો (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	પાછા retval;
}

સ્થિર errcode_t io_manager_configure(સંરચના sparse_io_params *params,
				      પૂર્ણાંક flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	સંરચના sparse_map *sm = calloc(1, કદ(*sm));
	જો (!sm)
		પાછા EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	જો (retval) {
		જો (!params->block_size || !params->blocks_ગણતરી) {
			retval = EINVAL;
			જાઓ err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_ગણતરી = params->blocks_ગણતરી;
		sm->blocks = calloc(params->blocks_ગણતરી, કદ(શૂન્ય*));
		જો (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			જાઓ err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_ગણતરી;

	જો (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		જો (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			જાઓ err_alloc;
		}
		જો (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_દ્વિઅંકી,
				      0644);
			જો (sm->fd < 0) {
				retval = errno;
				જાઓ err_open;
			}
		} નહીંતર
			sm->fd = params->fd;
	} નહીંતર {
		sm->fd = -1;
		sm->sparse_file = ખાલી;
	}
	પાછા 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	મુક્ત(sm);
	પાછા retval;
}

સ્થિર errcode_t sparse_open_channel(સંરચના sparse_io_params *sparse_params,
				     પૂર્ણાંક flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, કદ(સંરચના struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	જો (retval) {
		મુક્ત(io);
		પાછા retval;
	}

	*channel = io;
	પાછા 0;
}

સ્થિર errcode_t read_sparse_argv(અચલ અક્ષર *name, bool is_fd,
				  સંરચના sparse_io_params *sparse_params)
{
	પૂર્ણાંક ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_ગણતરી = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	જો (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		પાછા EXT2_ET_NO_MEMORY;
	}

	જો (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_ગણતરી,
			     &sparse_params->block_size);
	} નહીંતર {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_ગણતરી,
			     &sparse_params->block_size);
	}

	જો (ret < 1) {
		મુક્ત(sparse_params->file);
		પાછા EINVAL;
	}
	પાછા 0;
}

સ્થિર errcode_t sparse_open(અચલ અક્ષર *name, પૂર્ણાંક flags, io_channel *channel)
{
	errcode_t retval;
	સંરચના sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	જો (retval)
		પાછા EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	જો (retval)
		પાછા retval;
	(*channel)->manager = sparse_io_manager;

	પાછા retval;
}

સ્થિર errcode_t sparsefd_open(અચલ અક્ષર *name, પૂર્ણાંક flags, io_channel *channel)
{
	errcode_t retval;
	સંરચના sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	જો (retval)
		પાછા EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	જો (retval)
		પાછા retval;
	(*channel)->manager = sparsefd_io_manager;

	પાછા retval;
}

સ્થિર errcode_t sparse_merge_blocks(સંરચના sparse_map *sm, uint64_t start,
					uint64_t અંક)
{
	અક્ષર *buf;
	uint64_t i;
	unsigned પૂર્ણાંક block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(અંક, block_size);
	જો (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)અંક * block_size);
		પાછા EXT2_ET_NO_MEMORY;
	}

	માટે (i = 0; i < અંક; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		મુક્ત(sm->blocks[start + i]);
		sm->blocks[start + i] = ખાલી;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * અંક, start);

	પાછા retval;
}

સ્થિર errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	સંરચના sparse_map *sm = channel->private_data;

	જો (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == ખાલી) ? -1 : 0;
		માટે (i = 0; i < sm->blocks_ગણતરી; ++i) {
			જો (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} નહીંતર જો (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			જો (retval)
				જાઓ ret;
		}
		જો (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_ગણતરી - chunk_start);
			જો (retval)
				જાઓ ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	જો (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	મુક્ત(sm->file);
	મુક્ત(sm);
	મુક્ત(channel);
	પાછા retval;
}

સ્થિર errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	સંરચના sparse_map *sm = channel->private_data;
	પૂર્ણાંક fd = sm->fd;

	retval = sparse_close_channel(channel);
	જો (fd >= 0)
		close(fd);

	પાછા retval;
}

સ્થિર errcode_t sparse_set_blksize(io_channel channel, પૂર્ણાંક blksize)
{
	channel->block_size = blksize;
	પાછા 0;
}

સ્થિર blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, સંરચના sparse_map *sm)
{
	પૂર્ણાંક ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	પાછા ret;
}

સ્થિર errcode_t check_block_size(io_channel channel, સંરચના sparse_map *sm)
{
	જો (sm->block_size >= channel->block_size)
		પાછા 0;
	પાછા EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

સ્થિર errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	પૂર્ણાંક i;
	અક્ષર *out = buf;
	blk64_t offset = 0, cur_block;
	સંરચના sparse_map *sm = channel->private_data;

	જો (check_block_size(channel, sm))
		પાછા EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	જો (ગણતરી < 0) { //partial read
		ગણતરી = -ગણતરી;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		જો (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, ગણતરી);
		નહીંતર
			memset(out, 0, ગણતરી);
	} નહીંતર {
		માટે (i = 0; i < ગણતરી; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			જો (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			નહીંતર જો (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	પાછા 0;
}

સ્થિર errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	પાછા sparse_read_blk64(channel, block, ગણતરી, buf);
}

સ્થિર errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	પૂર્ણાંક i;
	blk64_t offset = 0, cur_block;
	અચલ અક્ષર *in = buf;
	સંરચના sparse_map *sm = channel->private_data;

	જો (check_block_size(channel, sm))
		પાછા EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	જો (ગણતરી < 0) { //partial write
		ગણતરી = -ગણતરી;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		જો (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			જો (!sm->blocks[cur_block])
				પાછા EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, ગણતરી);
	} નહીંતર {
		માટે (i = 0; i < ગણતરી; ++i) {
			જો (block + i >= sm->blocks_ગણતરી)
				પાછા 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			જો (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				જો (!sm->blocks[cur_block])
					પાછા EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	પાછા 0;
}

સ્થિર errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	પાછા sparse_write_blk64(channel, block, ગણતરી, buf);
}

સ્થિર errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long ગણતરી)
{
	blk64_t cur_block, offset;
	સંરચના sparse_map *sm = channel->private_data;

	જો (check_block_size(channel, sm))
		પાછા EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	માટે (unsigned long long i = 0; i < ગણતરી; ++i) {
		જો (blk + i >= sm->blocks_ગણતરી)
			પાછા 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		જો (!sm->blocks[cur_block])
			ચાલુ;
		મુક્ત(sm->blocks[cur_block]);
		sm->blocks[cur_block] = ખાલી;
	}
	પાછા 0;
}

સ્થિર errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long ગણતરી)
{
	પાછા sparse_discard(channel, blk, ગણતરી);
}

સ્થિર errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	પાછા 0;
}

સ્થિર errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   અચલ અક્ષર *option __attribute__((unused)),
                                   અચલ અક્ષર *arg __attribute__((unused)))
{
	પાછા 0;
}

સ્થિર errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long ગણતરી __attribute__((unused)))
{
	પાછા 0;
}

સ્થિર સંરચના struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ખાલી,
	.set_option		= sparse_set_option,
	.get_stats		= ખાલી,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

સ્થિર સંરચના struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ખાલી,
	.set_option		= sparse_set_option,
	.get_stats		= ખાલી,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
