/*
 * qcow2.c --- Functions to generate qcow2 formatted disk images.  This
 * format is used originally by QEMU for virtual machines, and stores the
 * filesystem data on disk in a packed format to avoid creating sparse
 * image files that need lots of seeking to read and write.
 *
 * The qcow2 format supports zlib compression, but that is not yet
 * implemented.
 *
 * It is possible to directly mount a qcow2 image using qemu-nbd:
 *
 * [root]# modprobe nbd max_part=63
 * [root]# qemu-nbd -c /dev/nbd0 image.img
 * [root]# mount /dev/nbd0p1 /mnt/qemu
 *
 * Format details at http://people.gnome.org/~markmc/qcow-image-format.html
 *
 * Copyright (C) 2010 Red Hat, Inc., Lukas Czerner <lczerner@redhat.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 */

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include "config.h"
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>

#include "ext2fs/ext2fs.h"
#include "qcow2.h"

/* Functions for converting qcow2 image into raw image */

sa_nracanaa ext2_qcow2_hdr *qcow2_read_header(puur_naa_nka fd)
{
	void *buffer = NULL;
	sa_nracanaa ext2_qcow2_hdr *hdr = NULL;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_qcow2_hdr), &buffer);
	ᤀᤡ᤺ (ret)
		ᤀᤡᤴ NULL;
	memset(buffer, 0, parimaa_na(sa_nracanaa ext2_qcow2_hdr));

	ᤀᤡ᤺ (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		ᤀᤡᤴ NULL;
	}

	size = read(fd, buffer, parimaa_na(sa_nracanaa ext2_qcow2_hdr));
	ᤀᤡ᤺ (size != parimaa_na(sa_nracanaa ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		ᤀᤡᤴ NULL;
	}

	hdr = (sa_nracanaa ext2_qcow2_hdr *)(buffer);

	ᤀᤡ᤺ ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		ᤀᤡᤴ NULL;
	}

	ᤀᤡᤴ hdr;
}

static puur_naa_nka qcow2_read_l1_table(sa_nracanaa ext2_qcow2_image *img)
{
	puur_naa_nka fd = img->fd;
	size_t size, l1_size = img->l1_size * parimaa_na(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	ᤀᤡ᤺ (ret)
		ᤀᤡᤴ ret;

	ᤀᤡ᤺ (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		ᤀᤡᤴ errno;
	}

	size = read(fd, table, l1_size);
	ᤀᤡ᤺ (size != l1_size) {
		ext2fs_free_mem(&table);
		ᤀᤡᤴ errno;
	}

	img->l1_table = table;

	ᤀᤡᤴ 0;
}

static puur_naa_nka qcow2_read_l2_table(sa_nracanaa ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	puur_naa_nka fd = img->fd;
	size_t size;

	assert(*l2_table);

	ᤀᤡ᤺ (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		ᤀᤡᤴ errno;

	size = read(fd, *l2_table, img->cluster_size);
	ᤀᤡ᤺ (size != img->cluster_size)
		ᤀᤡᤴ errno;

	ᤀᤡᤴ 0;
}

static puur_naa_nka qcow2_copy_data(puur_naa_nka fdin, puur_naa_nka fdout, __u64 off_in,
			   __u64 off_out, void *buf, size_t count)
{
	ssize_t c1, c2, c;
	void *ptr;
	puur_naa_nka retries = 10;

	assert(buf);

	ᤀᤡ᤺ (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		ᤀᤡᤴ errno;

	ᤀᤡ᤺ (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		ᤀᤡᤴ errno;

	ᤀᤠ᤺ (count > 0) {
		errno = 0;
		c1 = read(fdin, buf, count);
		ᤀᤡ᤺ (c1 < 0 || ((c1 == 0) && errno))
			ᤀᤡᤴ errno;
		ᤀᤡ᤺ (c1 == 0)
			bha_nga; 	/* EOF */

		for (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = write(fdout, ptr, c1);
			ᤀᤡ᤺ (c2 < 0 || ((c2 == 0) && errno))
				ᤀᤡᤴ errno;
			ᤀᤡ᤺ (c2 == 0 && --retries <= 0)
				bha_nga; /* This should never happen...  */
		}
		count -= c1;
	}
	ᤀᤡᤴ 0;
}


puur_naa_nka qcow2_write_raw_image(puur_naa_nka qcow2_fd, puur_naa_nka raw_fd,
			      sa_nracanaa ext2_qcow2_hdr *hdr)
{
	sa_nracanaa ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned puur_naa_nka l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = NULL;
	void *copy_buf = NULL;
	size_t size;
	unsigned puur_naa_nka max_l1_size;

	ᤀᤡ᤺ (hdr->crypt_method)
		ᤀᤡᤴ -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = NULL;
	img.l1_table = NULL;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	ᤀᤡ᤺ (img.cluster_bits < 9 || img.cluster_bits > 31)
		ᤀᤡᤴ -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	ᤀᤡ᤺ (img.l1_offset & (img.cluster_size - 1))
		ᤀᤡᤴ -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	ᤀᤡ᤺ (img.l1_size > max_l1_size)
		ᤀᤡᤴ -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	ᤀᤡ᤺ (ret)
		goto out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	ᤀᤡ᤺ (ret)
		goto out;

	ᤀᤡ᤺ (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		goto out;
	}

	ret = qcow2_read_l1_table(&img);
	ᤀᤡ᤺ (ret)
		goto out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	for (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		ᤀᤡ᤺ ((offset > img.image_size) ||
		    (offset <= 0))
			continue;

		ᤀᤡ᤺ (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			goto out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		ᤀᤡ᤺ (ret)
			bha_nga;

		/* Walk through l2 table and copy data blocks into raw image */
		for (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			ᤀᤡ᤺ (offset == 0)
				continue;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			ᤀᤡ᤺ (ret)
				goto out;
		}
	}

	/* Resize the output image to the filesystem size */
	ᤀᤡ᤺ (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		goto out;
	}

	((var_na *)copy_buf)[0] = 0;
	size = write(raw_fd, copy_buf, 1);
	ᤀᤡ᤺ (size != 1) {
		ret = errno;
		goto out;
	}

out:
	ᤀᤡ᤺ (copy_buf)
		ext2fs_free_mem(&copy_buf);
	ᤀᤡ᤺ (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	ᤀᤡ᤺ (l2_table)
		ext2fs_free_mem(&l2_table);
	ᤀᤡᤴ ret;
}
