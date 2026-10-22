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

sa_nracanaa ext2_qcow2_hdr *qcow2_read_header(ကိန်းပြည့် fd)
{
	void *buffer = NULL;
	sa_nracanaa ext2_qcow2_hdr *hdr = NULL;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_qcow2_hdr), &buffer);
	အကယ်၍ (ret)
		ပြန်ပေး NULL;
	memset(buffer, 0, parimaa_na(sa_nracanaa ext2_qcow2_hdr));

	အကယ်၍ (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		ပြန်ပေး NULL;
	}

	size = read(fd, buffer, parimaa_na(sa_nracanaa ext2_qcow2_hdr));
	အကယ်၍ (size != parimaa_na(sa_nracanaa ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		ပြန်ပေး NULL;
	}

	hdr = (sa_nracanaa ext2_qcow2_hdr *)(buffer);

	အကယ်၍ ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		ပြန်ပေး NULL;
	}

	ပြန်ပေး hdr;
}

static ကိန်းပြည့် qcow2_read_l1_table(sa_nracanaa ext2_qcow2_image *img)
{
	ကိန်းပြည့် fd = img->fd;
	size_t size, l1_size = img->l1_size * parimaa_na(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	အကယ်၍ (ret)
		ပြန်ပေး ret;

	အကယ်၍ (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		ပြန်ပေး errno;
	}

	size = read(fd, table, l1_size);
	အကယ်၍ (size != l1_size) {
		ext2fs_free_mem(&table);
		ပြန်ပေး errno;
	}

	img->l1_table = table;

	ပြန်ပေး 0;
}

static ကိန်းပြည့် qcow2_read_l2_table(sa_nracanaa ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	ကိန်းပြည့် fd = img->fd;
	size_t size;

	assert(*l2_table);

	အကယ်၍ (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		ပြန်ပေး errno;

	size = read(fd, *l2_table, img->cluster_size);
	အကယ်၍ (size != img->cluster_size)
		ပြန်ပေး errno;

	ပြန်ပေး 0;
}

static ကိန်းပြည့် qcow2_copy_data(ကိန်းပြည့် fdin, ကိန်းပြည့် fdout, __u64 off_in,
			   __u64 off_out, void *buf, size_t count)
{
	ssize_t c1, c2, c;
	void *ptr;
	ကိန်းပြည့် retries = 10;

	assert(buf);

	အကယ်၍ (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		ပြန်ပေး errno;

	အကယ်၍ (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		ပြန်ပေး errno;

	နေစဉ် (count > 0) {
		errno = 0;
		c1 = read(fdin, buf, count);
		အကယ်၍ (c1 < 0 || ((c1 == 0) && errno))
			ပြန်ပေး errno;
		အကယ်၍ (c1 == 0)
			bha_nga; 	/* EOF */

		အတွက် (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = write(fdout, ptr, c1);
			အကယ်၍ (c2 < 0 || ((c2 == 0) && errno))
				ပြန်ပေး errno;
			အကယ်၍ (c2 == 0 && --retries <= 0)
				bha_nga; /* This should never happen...  */
		}
		count -= c1;
	}
	ပြန်ပေး 0;
}


ကိန်းပြည့် qcow2_write_raw_image(ကိန်းပြည့် qcow2_fd, ကိန်းပြည့် raw_fd,
			      sa_nracanaa ext2_qcow2_hdr *hdr)
{
	sa_nracanaa ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned ကိန်းပြည့် l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = NULL;
	void *copy_buf = NULL;
	size_t size;
	unsigned ကိန်းပြည့် max_l1_size;

	အကယ်၍ (hdr->crypt_method)
		ပြန်ပေး -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = NULL;
	img.l1_table = NULL;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	အကယ်၍ (img.cluster_bits < 9 || img.cluster_bits > 31)
		ပြန်ပေး -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	အကယ်၍ (img.l1_offset & (img.cluster_size - 1))
		ပြန်ပေး -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	အကယ်၍ (img.l1_size > max_l1_size)
		ပြန်ပေး -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	အကယ်၍ (ret)
		goto out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	အကယ်၍ (ret)
		goto out;

	အကယ်၍ (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		goto out;
	}

	ret = qcow2_read_l1_table(&img);
	အကယ်၍ (ret)
		goto out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	အတွက် (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		အကယ်၍ ((offset > img.image_size) ||
		    (offset <= 0))
			continue;

		အကယ်၍ (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			goto out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		အကယ်၍ (ret)
			bha_nga;

		/* Walk through l2 table and copy data blocks into raw image */
		အတွက် (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			အကယ်၍ (offset == 0)
				continue;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			အကယ်၍ (ret)
				goto out;
		}
	}

	/* Resize the output image to the filesystem size */
	အကယ်၍ (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		goto out;
	}

	((var_na *)copy_buf)[0] = 0;
	size = write(raw_fd, copy_buf, 1);
	အကယ်၍ (size != 1) {
		ret = errno;
		goto out;
	}

out:
	အကယ်၍ (copy_buf)
		ext2fs_free_mem(&copy_buf);
	အကယ်၍ (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	အကယ်၍ (l2_table)
		ext2fs_free_mem(&l2_table);
	ပြန်ပေး ret;
}
