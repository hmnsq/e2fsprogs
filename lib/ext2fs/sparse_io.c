#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ifndef O_ದ್ವಿಮಾನ
#define O_ದ್ವಿಮಾನ 0
#endif

#ಯದಿ !defined(ENABLE_LIBSPARSE)
ಸ್ಥಿರ errcode_t sparse_open(ಧ್ರುವ ವರ್ಣ *name EXT2FS_ATTR((unused)),
			     ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_ATTR((unused)),
			     io_channel *channel EXT2FS_ATTR((unused)))
{
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
}
ಸ್ಥಿರ errcode_t sparse_close(io_channel channel EXT2FS_ATTR((unused)))
{
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNIMPLEMENTED;
}
ಸ್ಥಿರ ಸಂರಚನಾ struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
ಸ್ಥಿರ ಸಂರಚನಾ struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
};
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ <sparse/sparse.h>

ಸಂರಚನಾ sparse_map {
	ಪೂರ್ಣಾಙ್ಕ			fd;
	ವರ್ಣ			**blocks;
	ಪೂರ್ಣಾಙ್ಕ			block_size;
	uint64_t		blocks_ಎಣಿಕೆ;
	ವರ್ಣ			*file;
	ಸಂರಚನಾ sparse_file	*sparse_file;
	io_channel		channel;
};

ಸಂರಚನಾ sparse_io_params {
	ಪೂರ್ಣಾಙ್ಕ			fd;
	ವರ್ಣ			*file;
	uint64_t		blocks_ಎಣಿಕೆ;
	unsigned ಪೂರ್ಣಾಙ್ಕ		block_size;
};

ಸ್ಥಿರ errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf);

ಸ್ಥಿರ ಶೂನ್ಯ free_sparse_blocks(ಸಂರಚನಾ sparse_map *sm)
{
	uint64_t i;

	ಪ್ರತಿ (i = 0; i < sm->blocks_ಎಣಿಕೆ; ++i)
		ಮುಕ್ತ(sm->blocks[i]);
	ಮುಕ್ತ(sm->blocks);
	sm->blocks = ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sparse_import_segment(ಶೂನ್ಯ *priv, ಧ್ರುವ ಶೂನ್ಯ *data, size_t len,
				 unsigned ಪೂರ್ಣಾಙ್ಕ block, unsigned ಪೂರ್ಣಾಙ್ಕ nr_blocks)
{
	ಸಂರಚನಾ sparse_map *sm = priv;

	/* Ignore chunk headers, only write the data */
	ಯದಿ (!nr_blocks || len % sm->block_size)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ sparse_write_blk(sm->channel, block, nr_blocks, data);
}

ಸ್ಥಿರ errcode_t io_manager_import_sparse(ಸಂರಚನಾ sparse_io_params *params,
					  ಸಂರಚನಾ sparse_map *sm, io_channel io)
{
	ಪೂರ್ಣಾಙ್ಕ fd;
	errcode_t retval;
	ಸಂರಚನಾ sparse_file *sparse_file;

	ಯದಿ (params->fd < 0) {
		fd = open(params->file, O_RDONLY);
		ಯದಿ (fd < 0) {
			retval = -1;
			ಗಚ್ಛ err_open;
		}
	} ಅನ್ಯಥಾ
		fd = params->fd;
	sparse_file = sparse_file_import(fd, false, false);
	ಯದಿ (!sparse_file) {
		retval = -1;
		ಗಚ್ಛ err_sparse;
	}

	sm->block_size = sparse_file_block_size(sparse_file);
	sm->blocks_ಎಣಿಕೆ = (sparse_file_len(sparse_file, 0, 0) - 1)
				/ sm->block_size + 1;
	sm->blocks = calloc(sm->blocks_ಎಣಿಕೆ, ಪರಿಮಾಣ(ವರ್ಣ*));
	ಯದಿ (!sm->blocks) {
		retval = -1;
		ಗಚ್ಛ err_alloc;
	}
	io->block_size = sm->block_size;

	retval = sparse_file_foreach_chunk(sparse_file, true, false,
					   sparse_import_segment, sm);

	ಯದಿ (retval)
		free_sparse_blocks(sm);
err_alloc:
	sparse_file_destroy(sparse_file);
err_sparse:
	close(fd);
err_open:
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t io_manager_configure(ಸಂರಚನಾ sparse_io_params *params,
				      ಪೂರ್ಣಾಙ್ಕ flags, io_channel io)
{
	errcode_t retval;
	uint64_t img_size;
	ಸಂರಚನಾ sparse_map *sm = calloc(1, ಪರಿಮಾಣ(*sm));
	ಯದಿ (!sm)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;

	sm->file = params->file;
	sm->channel = io;
	io->private_data = sm;
	retval = io_manager_import_sparse(params, sm, io);
	ಯದಿ (retval) {
		ಯದಿ (!params->block_size || !params->blocks_ಎಣಿಕೆ) {
			retval = EINVAL;
			ಗಚ್ಛ err_params;
		}
		sm->block_size = params->block_size;
		sm->blocks_ಎಣಿಕೆ = params->blocks_ಎಣಿಕೆ;
		sm->blocks = calloc(params->blocks_ಎಣಿಕೆ, ಪರಿಮಾಣ(ಶೂನ್ಯ*));
		ಯದಿ (!sm->blocks) {
			retval = EXT2_ET_NO_MEMORY;
			ಗಚ್ಛ err_alloc;
		}
	}
	io->block_size = sm->block_size;
	img_size = (uint64_t)sm->block_size * sm->blocks_ಎಣಿಕೆ;

	ಯದಿ (flags & IO_FLAG_RW) {
		sm->sparse_file = sparse_file_new(sm->block_size, img_size);
		ಯದಿ (!sm->sparse_file) {
			retval = EXT2_ET_NO_MEMORY;
			ಗಚ್ಛ err_alloc;
		}
		ಯದಿ (params->fd < 0) {
			sm->fd = open(params->file, O_CREAT | O_RDWR | O_TRUNC | O_ದ್ವಿಮಾನ,
				      0644);
			ಯದಿ (sm->fd < 0) {
				retval = errno;
				ಗಚ್ಛ err_open;
			}
		} ಅನ್ಯಥಾ
			sm->fd = params->fd;
	} ಅನ್ಯಥಾ {
		sm->fd = -1;
		sm->sparse_file = ರಿಕ್ತ;
	}
	ಪ್ರತ್ಯಾಗಮ 0;

err_open:
	sparse_file_destroy(sm->sparse_file);
err_alloc:
	free_sparse_blocks(sm);
err_params:
	ಮುಕ್ತ(sm);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t sparse_open_channel(ಸಂರಚನಾ sparse_io_params *sparse_params,
				     ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel)
{
	errcode_t retval;
	io_channel io;

	io = calloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel));
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	io->block_size = 0;
	io->refcount = 1;

	retval = io_manager_configure(sparse_params, flags, io);
	ಯದಿ (retval) {
		ಮುಕ್ತ(io);
		ಪ್ರತ್ಯಾಗಮ retval;
	}

	*channel = io;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t read_sparse_argv(ಧ್ರುವ ವರ್ಣ *name, bool is_fd,
				  ಸಂರಚನಾ sparse_io_params *sparse_params)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	sparse_params->fd = -1;
	sparse_params->block_size = 0;
	sparse_params->blocks_ಎಣಿಕೆ = 0;

	sparse_params->file = malloc(strlen(name) + 1);
	ಯದಿ (!sparse_params->file) {
		fprintf(stderr, "failed to alloc %zu\n", strlen(name) + 1);
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	}

	ಯದಿ (is_fd) {
		ret = sscanf(name, "(%d):%llu:%u", &sparse_params->fd,
			     (unsigned long long *)&sparse_params->blocks_ಎಣಿಕೆ,
			     &sparse_params->block_size);
	} ಅನ್ಯಥಾ {
		ret = sscanf(name, "(%[^)])%*[:]%llu%*[:]%u", sparse_params->file,
			     (unsigned long long *)&sparse_params->blocks_ಎಣಿಕೆ,
			     &sparse_params->block_size);
	}

	ಯದಿ (ret < 1) {
		ಮುಕ್ತ(sparse_params->file);
		ಪ್ರತ್ಯಾಗಮ EINVAL;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t sparse_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel)
{
	errcode_t retval;
	ಸಂರಚನಾ sparse_io_params sparse_params;

	retval = read_sparse_argv(name, false, &sparse_params);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	(*channel)->manager = sparse_io_manager;

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t sparsefd_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel)
{
	errcode_t retval;
	ಸಂರಚನಾ sparse_io_params sparse_params;

	retval = read_sparse_argv(name, true, &sparse_params);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;

	retval = sparse_open_channel(&sparse_params, flags, channel);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	(*channel)->manager = sparsefd_io_manager;

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t sparse_merge_blocks(ಸಂರಚನಾ sparse_map *sm, uint64_t start,
					uint64_t ಅಂಕೆ)
{
	ವರ್ಣ *buf;
	uint64_t i;
	unsigned ಪೂರ್ಣಾಙ್ಕ block_size = sm->block_size;
	errcode_t retval = 0;

	buf = calloc(ಅಂಕೆ, block_size);
	ಯದಿ (!buf) {
		fprintf(stderr, "failed to alloc %llu\n",
			(unsigned long long)ಅಂಕೆ * block_size);
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
	}

	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ; i++) {
		memcpy(buf + i * block_size, sm->blocks[start + i] , block_size);
		ಮುಕ್ತ(sm->blocks[start + i]);
		sm->blocks[start + i] = ರಿಕ್ತ;
	}

	/* free_sparse_blocks will release this buf. */
	sm->blocks[start] = buf;

	retval = sparse_file_add_data(sm->sparse_file, sm->blocks[start],
					block_size * ಅಂಕೆ, start);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t sparse_close_channel(io_channel channel)
{
	uint64_t i;
	errcode_t retval = 0;
	ಸಂರಚನಾ sparse_map *sm = channel->private_data;

	ಯದಿ (sm->sparse_file) {
		int64_t chunk_start = (sm->blocks[0] == ರಿಕ್ತ) ? -1 : 0;
		ಪ್ರತಿ (i = 0; i < sm->blocks_ಎಣಿಕೆ; ++i) {
			ಯದಿ (!sm->blocks[i] && chunk_start != -1) {
				retval = sparse_merge_blocks(sm, chunk_start, i - chunk_start);
				chunk_start = -1;
			} ಅನ್ಯಥಾ ಯದಿ (sm->blocks[i] && chunk_start == -1) {
				chunk_start = i;
			}
			ಯದಿ (retval)
				ಗಚ್ಛ ret;
		}
		ಯದಿ (chunk_start != -1) {
			retval = sparse_merge_blocks(sm, chunk_start,
							sm->blocks_ಎಣಿಕೆ - chunk_start);
			ಯದಿ (retval)
				ಗಚ್ಛ ret;
		}
		retval = sparse_file_write(sm->sparse_file, sm->fd,
					   /*gzip*/0, /*sparse*/1, /*crc*/0);
	}

ret:
	ಯದಿ (sm->sparse_file)
		sparse_file_destroy(sm->sparse_file);
	free_sparse_blocks(sm);
	ಮುಕ್ತ(sm->file);
	ಮುಕ್ತ(sm);
	ಮುಕ್ತ(channel);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t sparse_close(io_channel channel)
{
	errcode_t retval;
	ಸಂರಚನಾ sparse_map *sm = channel->private_data;
	ಪೂರ್ಣಾಙ್ಕ fd = sm->fd;

	retval = sparse_close_channel(channel);
	ಯದಿ (fd >= 0)
		close(fd);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t sparse_set_blksize(io_channel channel, ಪೂರ್ಣಾಙ್ಕ blksize)
{
	channel->block_size = blksize;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ blk64_t block_to_sparse_block(blk64_t block, blk64_t *offset,
			       io_channel channel, ಸಂರಚನಾ sparse_map *sm)
{
	ಪೂರ್ಣಾಙ್ಕ ratio;
	blk64_t ret = block;

	ratio = sm->block_size / channel->block_size;
	ret /= ratio;
	*offset = (block % ratio) * channel->block_size;

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ errcode_t check_block_size(io_channel channel, ಸಂರಚನಾ sparse_map *sm)
{
	ಯದಿ (sm->block_size >= channel->block_size)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNEXPECTED_BLOCK_SIZE;
}

ಸ್ಥಿರ errcode_t sparse_read_blk64(io_channel channel, blk64_t block,
				   ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ವರ್ಣ *out = buf;
	blk64_t offset = 0, cur_block;
	ಸಂರಚನಾ sparse_map *sm = channel->private_data;

	ಯದಿ (check_block_size(channel, sm))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	ಯದಿ (ಎಣಿಕೆ < 0) { //partial read
		ಎಣಿಕೆ = -ಎಣಿಕೆ;
		cur_block = block_to_sparse_block(block, &offset, channel, sm);
		ಯದಿ (sm->blocks[cur_block])
			memcpy(out, (sm->blocks[cur_block]) + offset, ಎಣಿಕೆ);
		ಅನ್ಯಥಾ
			memset(out, 0, ಎಣಿಕೆ);
	} ಅನ್ಯಥಾ {
		ಪ್ರತಿ (i = 0; i < ಎಣಿಕೆ; ++i) {
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			ಯದಿ (sm->blocks[cur_block])
				memcpy(out + (i * channel->block_size),
				       sm->blocks[cur_block] + offset,
				       channel->block_size);
			ಅನ್ಯಥಾ ಯದಿ (sm->blocks)
				memset(out + (i * channel->block_size), 0,
				       channel->block_size);
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t sparse_read_blk(io_channel channel, unsigned long block,
				 ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ sparse_read_blk64(channel, block, ಎಣಿಕೆ, buf);
}

ಸ್ಥಿರ errcode_t sparse_write_blk64(io_channel channel, blk64_t block,
				    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	blk64_t offset = 0, cur_block;
	ಧ್ರುವ ವರ್ಣ *in = buf;
	ಸಂರಚನಾ sparse_map *sm = channel->private_data;

	ಯದಿ (check_block_size(channel, sm))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	ಯದಿ (ಎಣಿಕೆ < 0) { //partial write
		ಎಣಿಕೆ = -ಎಣಿಕೆ;
		cur_block = block_to_sparse_block(block, &offset, channel,
						  sm);
		ಯದಿ (!sm->blocks[cur_block]) {
			sm->blocks[cur_block] = calloc(1, sm->block_size);
			ಯದಿ (!sm->blocks[cur_block])
				ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
		}
		memcpy(sm->blocks[cur_block] + offset, in, ಎಣಿಕೆ);
	} ಅನ್ಯಥಾ {
		ಪ್ರತಿ (i = 0; i < ಎಣಿಕೆ; ++i) {
			ಯದಿ (block + i >= sm->blocks_ಎಣಿಕೆ)
				ಪ್ರತ್ಯಾಗಮ 0;
			cur_block = block_to_sparse_block(block + i, &offset,
						    channel, sm);
			ಯದಿ (!sm->blocks[cur_block]) {
				sm->blocks[cur_block] =
					calloc(1, sm->block_size);
				ಯದಿ (!sm->blocks[cur_block])
					ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
			}
			memcpy(sm->blocks[cur_block] + offset,
			       in + (i * channel->block_size),
			       channel->block_size);
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t sparse_write_blk(io_channel channel, unsigned long block,
				  ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ sparse_write_blk64(channel, block, ಎಣಿಕೆ, buf);
}

ಸ್ಥಿರ errcode_t sparse_discard(io_channel channel __attribute__((unused)),
				blk64_t blk, unsigned long long ಎಣಿಕೆ)
{
	blk64_t cur_block, offset;
	ಸಂರಚನಾ sparse_map *sm = channel->private_data;

	ಯದಿ (check_block_size(channel, sm))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	ಪ್ರತಿ (unsigned long long i = 0; i < ಎಣಿಕೆ; ++i) {
		ಯದಿ (blk + i >= sm->blocks_ಎಣಿಕೆ)
			ಪ್ರತ್ಯಾಗಮ 0;
		cur_block = block_to_sparse_block(blk + i, &offset, channel,
						  sm);
		ಯದಿ (!sm->blocks[cur_block])
			ಅನುವರ್ತ;
		ಮುಕ್ತ(sm->blocks[cur_block]);
		sm->blocks[cur_block] = ರಿಕ್ತ;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t sparse_zeroout(io_channel channel, blk64_t blk,
				unsigned long long ಎಣಿಕೆ)
{
	ಪ್ರತ್ಯಾಗಮ sparse_discard(channel, blk, ಎಣಿಕೆ);
}

ಸ್ಥಿರ errcode_t sparse_flush(io_channel channel __attribute__((unused)))
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t sparse_set_option(io_channel channel __attribute__((unused)),
                                   ಧ್ರುವ ವರ್ಣ *option __attribute__((unused)),
                                   ಧ್ರುವ ವರ್ಣ *arg __attribute__((unused)))
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t sparse_cache_readahead(
			io_channel channel __attribute__((unused)),
			blk64_t blk __attribute__((unused)),
			unsigned long long ಎಣಿಕೆ __attribute__((unused)))
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ struct_io_manager struct_sparse_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse I/O Manager",
	.open			= sparse_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ರಿಕ್ತ,
	.set_option		= sparse_set_option,
	.get_stats		= ರಿಕ್ತ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

ಸ್ಥಿರ ಸಂರಚನಾ struct_io_manager struct_sparsefd_manager = {
	.magic			= EXT2_ET_MAGIC_IO_MANAGER,
	.name			= "Android sparse fd I/O Manager",
	.open			= sparsefd_open,
	.close			= sparse_close,
	.set_blksize		= sparse_set_blksize,
	.read_blk		= sparse_read_blk,
	.write_blk		= sparse_write_blk,
	.flush			= sparse_flush,
	.write_byte		= ರಿಕ್ತ,
	.set_option		= sparse_set_option,
	.get_stats		= ರಿಕ್ತ,
	.read_blk64		= sparse_read_blk64,
	.write_blk64		= sparse_write_blk64,
	.discard		= sparse_discard,
	.cache_readahead	= sparse_cache_readahead,
	.zeroout		= sparse_zeroout,
};

#endif

io_manager sparse_io_manager = &struct_sparse_manager;
io_manager sparsefd_io_manager = &struct_sparsefd_manager;
