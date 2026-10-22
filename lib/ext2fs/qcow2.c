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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ifdef HAVE_STDLIB_H
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#endif
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "ext2fs/ext2fs.h"
#ಅನ್ತರ್ಭಾವ "qcow2.h"

/* Functions for converting qcow2 image into raw image */

ಸಂರಚನಾ ext2_qcow2_hdr *qcow2_read_header(ಪೂರ್ಣಾಙ್ಕ fd)
{
	ಶೂನ್ಯ *buffer = ರಿಕ್ತ;
	ಸಂರಚನಾ ext2_qcow2_hdr *hdr = ರಿಕ್ತ;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_qcow2_hdr), &buffer);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	memset(buffer, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_qcow2_hdr));

	ಯದಿ (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}

	size = ಪಠ(fd, buffer, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_qcow2_hdr));
	ಯದಿ (size != ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}

	hdr = (ಸಂರಚನಾ ext2_qcow2_hdr *)(buffer);

	ಯದಿ ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}

	ಪ್ರತ್ಯಾಗಮ hdr;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ qcow2_read_l1_table(ಸಂರಚನಾ ext2_qcow2_image *img)
{
	ಪೂರ್ಣಾಙ್ಕ fd = img->fd;
	size_t size, l1_size = img->l1_size * ಪರಿಮಾಣ(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	ಯದಿ (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		ಪ್ರತ್ಯಾಗಮ errno;
	}

	size = ಪಠ(fd, table, l1_size);
	ಯದಿ (size != l1_size) {
		ext2fs_free_mem(&table);
		ಪ್ರತ್ಯಾಗಮ errno;
	}

	img->l1_table = table;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ qcow2_read_l2_table(ಸಂರಚನಾ ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	ಪೂರ್ಣಾಙ್ಕ fd = img->fd;
	size_t size;

	ಪ್ರತಿಜ್ಞಾ(*l2_table);

	ಯದಿ (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		ಪ್ರತ್ಯಾಗಮ errno;

	size = ಪಠ(fd, *l2_table, img->cluster_size);
	ಯದಿ (size != img->cluster_size)
		ಪ್ರತ್ಯಾಗಮ errno;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ qcow2_copy_data(ಪೂರ್ಣಾಙ್ಕ fdin, ಪೂರ್ಣಾಙ್ಕ fdout, __u64 off_in,
			   __u64 off_out, ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ)
{
	ssize_t c1, c2, c;
	ಶೂನ್ಯ *ptr;
	ಪೂರ್ಣಾಙ್ಕ retries = 10;

	ಪ್ರತಿಜ್ಞಾ(buf);

	ಯದಿ (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		ಪ್ರತ್ಯಾಗಮ errno;

	ಯದಿ (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		ಪ್ರತ್ಯಾಗಮ errno;

	ಯಾವತ್ (ಎಣಿಕೆ > 0) {
		errno = 0;
		c1 = ಪಠ(fdin, buf, ಎಣಿಕೆ);
		ಯದಿ (c1 < 0 || ((c1 == 0) && errno))
			ಪ್ರತ್ಯಾಗಮ errno;
		ಯದಿ (c1 == 0)
			ಭಙ್ಗ; 	/* EOF */

		ಪ್ರತಿ (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = ಲಿಖ(fdout, ptr, c1);
			ಯದಿ (c2 < 0 || ((c2 == 0) && errno))
				ಪ್ರತ್ಯಾಗಮ errno;
			ಯದಿ (c2 == 0 && --retries <= 0)
				ಭಙ್ಗ; /* This should never happen...  */
		}
		ಎಣಿಕೆ -= c1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}


ಪೂರ್ಣಾಙ್ಕ qcow2_write_raw_image(ಪೂರ್ಣಾಙ್ಕ qcow2_fd, ಪೂರ್ಣಾಙ್ಕ raw_fd,
			      ಸಂರಚನಾ ext2_qcow2_hdr *hdr)
{
	ಸಂರಚನಾ ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = ರಿಕ್ತ;
	ಶೂನ್ಯ *copy_buf = ರಿಕ್ತ;
	size_t size;
	unsigned ಪೂರ್ಣಾಙ್ಕ max_l1_size;

	ಯದಿ (hdr->crypt_method)
		ಪ್ರತ್ಯಾಗಮ -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = ರಿಕ್ತ;
	img.l1_table = ರಿಕ್ತ;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	ಯದಿ (img.cluster_bits < 9 || img.cluster_bits > 31)
		ಪ್ರತ್ಯಾಗಮ -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	ಯದಿ (img.l1_offset & (img.cluster_size - 1))
		ಪ್ರತ್ಯಾಗಮ -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	ಯದಿ (img.l1_size > max_l1_size)
		ಪ್ರತ್ಯಾಗಮ -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	ಯದಿ (ret)
		ಗಚ್ಛ out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	ಯದಿ (ret)
		ಗಚ್ಛ out;

	ಯದಿ (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		ಗಚ್ಛ out;
	}

	ret = qcow2_read_l1_table(&img);
	ಯದಿ (ret)
		ಗಚ್ಛ out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	ಪ್ರತಿ (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		ಯದಿ ((offset > img.image_size) ||
		    (offset <= 0))
			ಅನುವರ್ತ;

		ಯದಿ (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			ಗಚ್ಛ out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		ಯದಿ (ret)
			ಭಙ್ಗ;

		/* Walk through l2 table and copy data blocks into raw image */
		ಪ್ರತಿ (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			ಯದಿ (offset == 0)
				ಅನುವರ್ತ;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			ಯದಿ (ret)
				ಗಚ್ಛ out;
		}
	}

	/* Resize the output image to the filesystem size */
	ಯದಿ (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		ಗಚ್ಛ out;
	}

	((ವರ್ಣ *)copy_buf)[0] = 0;
	size = ಲಿಖ(raw_fd, copy_buf, 1);
	ಯದಿ (size != 1) {
		ret = errno;
		ಗಚ್ಛ out;
	}

out:
	ಯದಿ (copy_buf)
		ext2fs_free_mem(&copy_buf);
	ಯದಿ (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	ಯದಿ (l2_table)
		ext2fs_free_mem(&l2_table);
	ಪ್ರತ್ಯಾಗಮ ret;
}
