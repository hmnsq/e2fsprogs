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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <grp.h>
#அந்தர்பாவ <pwd.h>
#அந்தர்பாவ <stdio.h>
#ifdef HAVE_STDLIB_H
#அந்தர்பாவ <stdlib.h>
#endif
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <assert.h>

#அந்தர்பாவ "ext2fs/ext2fs.h"
#அந்தர்பாவ "qcow2.h"

/* Functions for converting qcow2 image into raw image */

கட்டமைப்பு ext2_qcow2_hdr *qcow2_read_header(முழு fd)
{
	வெறுமை *buffer = வெற்று;
	கட்டமைப்பு ext2_qcow2_hdr *hdr = வெற்று;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_qcow2_hdr), &buffer);
	எனில் (ret)
		திரும்பு வெற்று;
	memset(buffer, 0, அளவு(கட்டமைப்பு ext2_qcow2_hdr));

	எனில் (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		திரும்பு வெற்று;
	}

	size = பட(fd, buffer, அளவு(கட்டமைப்பு ext2_qcow2_hdr));
	எனில் (size != அளவு(கட்டமைப்பு ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		திரும்பு வெற்று;
	}

	hdr = (கட்டமைப்பு ext2_qcow2_hdr *)(buffer);

	எனில் ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		திரும்பு வெற்று;
	}

	திரும்பு hdr;
}

நிலையான முழு qcow2_read_l1_table(கட்டமைப்பு ext2_qcow2_image *img)
{
	முழு fd = img->fd;
	size_t size, l1_size = img->l1_size * அளவு(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	எனில் (ret)
		திரும்பு ret;

	எனில் (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		திரும்பு errno;
	}

	size = பட(fd, table, l1_size);
	எனில் (size != l1_size) {
		ext2fs_free_mem(&table);
		திரும்பு errno;
	}

	img->l1_table = table;

	திரும்பு 0;
}

நிலையான முழு qcow2_read_l2_table(கட்டமைப்பு ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	முழு fd = img->fd;
	size_t size;

	ப்ரதிஜ்ஞா(*l2_table);

	எனில் (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		திரும்பு errno;

	size = பட(fd, *l2_table, img->cluster_size);
	எனில் (size != img->cluster_size)
		திரும்பு errno;

	திரும்பு 0;
}

நிலையான முழு qcow2_copy_data(முழு fdin, முழு fdout, __u64 off_in,
			   __u64 off_out, வெறுமை *buf, size_t மொத்தம்)
{
	ssize_t c1, c2, c;
	வெறுமை *ptr;
	முழு retries = 10;

	ப்ரதிஜ்ஞா(buf);

	எனில் (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		திரும்பு errno;

	எனில் (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		திரும்பு errno;

	போது (மொத்தம் > 0) {
		errno = 0;
		c1 = பட(fdin, buf, மொத்தம்);
		எனில் (c1 < 0 || ((c1 == 0) && errno))
			திரும்பு errno;
		எனில் (c1 == 0)
			நிறுத்து; 	/* EOF */

		க்கு (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = லிக(fdout, ptr, c1);
			எனில் (c2 < 0 || ((c2 == 0) && errno))
				திரும்பு errno;
			எனில் (c2 == 0 && --retries <= 0)
				நிறுத்து; /* This should never happen...  */
		}
		மொத்தம் -= c1;
	}
	திரும்பு 0;
}


முழு qcow2_write_raw_image(முழு qcow2_fd, முழு raw_fd,
			      கட்டமைப்பு ext2_qcow2_hdr *hdr)
{
	கட்டமைப்பு ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned முழு l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = வெற்று;
	வெறுமை *copy_buf = வெற்று;
	size_t size;
	unsigned முழு max_l1_size;

	எனில் (hdr->crypt_method)
		திரும்பு -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = வெற்று;
	img.l1_table = வெற்று;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	எனில் (img.cluster_bits < 9 || img.cluster_bits > 31)
		திரும்பு -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	எனில் (img.l1_offset & (img.cluster_size - 1))
		திரும்பு -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	எனில் (img.l1_size > max_l1_size)
		திரும்பு -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	எனில் (ret)
		செல் out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	எனில் (ret)
		செல் out;

	எனில் (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		செல் out;
	}

	ret = qcow2_read_l1_table(&img);
	எனில் (ret)
		செல் out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	க்கு (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		எனில் ((offset > img.image_size) ||
		    (offset <= 0))
			தொடர்;

		எனில் (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			செல் out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		எனில் (ret)
			நிறுத்து;

		/* Walk through l2 table and copy data blocks into raw image */
		க்கு (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			எனில் (offset == 0)
				தொடர்;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			எனில் (ret)
				செல் out;
		}
	}

	/* Resize the output image to the filesystem size */
	எனில் (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		செல் out;
	}

	((எழுத்து *)copy_buf)[0] = 0;
	size = லிக(raw_fd, copy_buf, 1);
	எனில் (size != 1) {
		ret = errno;
		செல் out;
	}

out:
	எனில் (copy_buf)
		ext2fs_free_mem(&copy_buf);
	எனில் (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	எனில் (l2_table)
		ext2fs_free_mem(&l2_table);
	திரும்பு ret;
}
