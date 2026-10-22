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

#شامل "config.h"
#شامل <fcntl.h>
#شامل <grp.h>
#شامل <pwd.h>
#شامل <stdio.h>
#ifdef HAVE_STDLIB_H
#شامل <stdlib.h>
#endif
#شامل <string.h>
#شامل <time.h>
#شامل <unistd.h>
#شامل <fcntl.h>
#شامل <errno.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <assert.h>

#شامل "ext2fs/ext2fs.h"
#شامل "qcow2.h"

/* Functions for converting qcow2 image into raw image */

ساختار ext2_qcow2_hdr *qcow2_read_header(صحیح fd)
{
	تهی *buffer = پوچ;
	ساختار ext2_qcow2_hdr *hdr = پوچ;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(اندازه(ساختار ext2_qcow2_hdr), &buffer);
	اگر (ret)
		بازگشت پوچ;
	memset(buffer, 0, اندازه(ساختار ext2_qcow2_hdr));

	اگر (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		بازگشت پوچ;
	}

	size = اقرأ(fd, buffer, اندازه(ساختار ext2_qcow2_hdr));
	اگر (size != اندازه(ساختار ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		بازگشت پوچ;
	}

	hdr = (ساختار ext2_qcow2_hdr *)(buffer);

	اگر ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		بازگشت پوچ;
	}

	بازگشت hdr;
}

ایستا صحیح qcow2_read_l1_table(ساختار ext2_qcow2_image *img)
{
	صحیح fd = img->fd;
	size_t size, l1_size = img->l1_size * اندازه(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	اگر (ret)
		بازگشت ret;

	اگر (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		بازگشت errno;
	}

	size = اقرأ(fd, table, l1_size);
	اگر (size != l1_size) {
		ext2fs_free_mem(&table);
		بازگشت errno;
	}

	img->l1_table = table;

	بازگشت 0;
}

ایستا صحیح qcow2_read_l2_table(ساختار ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	صحیح fd = img->fd;
	size_t size;

	أكد(*l2_table);

	اگر (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		بازگشت errno;

	size = اقرأ(fd, *l2_table, img->cluster_size);
	اگر (size != img->cluster_size)
		بازگشت errno;

	بازگشت 0;
}

ایستا صحیح qcow2_copy_data(صحیح fdin, صحیح fdout, __u64 off_in,
			   __u64 off_out, تهی *buf, size_t شمار)
{
	ssize_t c1, c2, c;
	تهی *ptr;
	صحیح retries = 10;

	أكد(buf);

	اگر (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		بازگشت errno;

	اگر (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		بازگشت errno;

	وقتی (شمار > 0) {
		errno = 0;
		c1 = اقرأ(fdin, buf, شمار);
		اگر (c1 < 0 || ((c1 == 0) && errno))
			بازگشت errno;
		اگر (c1 == 0)
			بشکن; 	/* EOF */

		برای (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = اكتب(fdout, ptr, c1);
			اگر (c2 < 0 || ((c2 == 0) && errno))
				بازگشت errno;
			اگر (c2 == 0 && --retries <= 0)
				بشکن; /* This should never happen...  */
		}
		شمار -= c1;
	}
	بازگشت 0;
}


صحیح qcow2_write_raw_image(صحیح qcow2_fd, صحیح raw_fd,
			      ساختار ext2_qcow2_hdr *hdr)
{
	ساختار ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned صحیح l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = پوچ;
	تهی *copy_buf = پوچ;
	size_t size;
	unsigned صحیح max_l1_size;

	اگر (hdr->crypt_method)
		بازگشت -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = پوچ;
	img.l1_table = پوچ;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	اگر (img.cluster_bits < 9 || img.cluster_bits > 31)
		بازگشت -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	اگر (img.l1_offset & (img.cluster_size - 1))
		بازگشت -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	اگر (img.l1_size > max_l1_size)
		بازگشت -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	اگر (ret)
		برو out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	اگر (ret)
		برو out;

	اگر (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		برو out;
	}

	ret = qcow2_read_l1_table(&img);
	اگر (ret)
		برو out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	برای (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		اگر ((offset > img.image_size) ||
		    (offset <= 0))
			ادامه;

		اگر (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			برو out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		اگر (ret)
			بشکن;

		/* Walk through l2 table and copy data blocks into raw image */
		برای (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			اگر (offset == 0)
				ادامه;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			اگر (ret)
				برو out;
		}
	}

	/* Resize the output image to the filesystem size */
	اگر (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		برو out;
	}

	((نویسه *)copy_buf)[0] = 0;
	size = اكتب(raw_fd, copy_buf, 1);
	اگر (size != 1) {
		ret = errno;
		برو out;
	}

out:
	اگر (copy_buf)
		ext2fs_free_mem(&copy_buf);
	اگر (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	اگر (l2_table)
		ext2fs_free_mem(&l2_table);
	بازگشت ret;
}
