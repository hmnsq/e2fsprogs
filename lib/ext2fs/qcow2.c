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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <grp.h>
#অন্তর্ভাব <pwd.h>
#অন্তর্ভাব <stdio.h>
#ifdef HAVE_STDLIB_H
#অন্তর্ভাব <stdlib.h>
#endif
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <assert.h>

#অন্তর্ভাব "ext2fs/ext2fs.h"
#অন্তর্ভাব "qcow2.h"

/* Functions for converting qcow2 image into raw image */

সংরচনা ext2_qcow2_hdr *qcow2_read_header(পূর্ণাঙ্ক fd)
{
	শূন্য *buffer = রিক্ত;
	সংরচনা ext2_qcow2_hdr *hdr = রিক্ত;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(পরিমাণ(সংরচনা ext2_qcow2_hdr), &buffer);
	যদি (ret)
		প্রত্যাগম রিক্ত;
	memset(buffer, 0, পরিমাণ(সংরচনা ext2_qcow2_hdr));

	যদি (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		প্রত্যাগম রিক্ত;
	}

	size = পঠ(fd, buffer, পরিমাণ(সংরচনা ext2_qcow2_hdr));
	যদি (size != পরিমাণ(সংরচনা ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		প্রত্যাগম রিক্ত;
	}

	hdr = (সংরচনা ext2_qcow2_hdr *)(buffer);

	যদি ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		প্রত্যাগম রিক্ত;
	}

	প্রত্যাগম hdr;
}

স্থির পূর্ণাঙ্ক qcow2_read_l1_table(সংরচনা ext2_qcow2_image *img)
{
	পূর্ণাঙ্ক fd = img->fd;
	size_t size, l1_size = img->l1_size * পরিমাণ(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	যদি (ret)
		প্রত্যাগম ret;

	যদি (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		প্রত্যাগম errno;
	}

	size = পঠ(fd, table, l1_size);
	যদি (size != l1_size) {
		ext2fs_free_mem(&table);
		প্রত্যাগম errno;
	}

	img->l1_table = table;

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক qcow2_read_l2_table(সংরচনা ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	পূর্ণাঙ্ক fd = img->fd;
	size_t size;

	প্রতিজ্ঞা(*l2_table);

	যদি (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		প্রত্যাগম errno;

	size = পঠ(fd, *l2_table, img->cluster_size);
	যদি (size != img->cluster_size)
		প্রত্যাগম errno;

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক qcow2_copy_data(পূর্ণাঙ্ক fdin, পূর্ণাঙ্ক fdout, __u64 off_in,
			   __u64 off_out, শূন্য *buf, size_t গুনতি)
{
	ssize_t c1, c2, c;
	শূন্য *ptr;
	পূর্ণাঙ্ক retries = 10;

	প্রতিজ্ঞা(buf);

	যদি (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		প্রত্যাগম errno;

	যদি (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		প্রত্যাগম errno;

	যাবত্ (গুনতি > 0) {
		errno = 0;
		c1 = পঠ(fdin, buf, গুনতি);
		যদি (c1 < 0 || ((c1 == 0) && errno))
			প্রত্যাগম errno;
		যদি (c1 == 0)
			ভঙ্গ; 	/* EOF */

		প্রতি (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = লিখ(fdout, ptr, c1);
			যদি (c2 < 0 || ((c2 == 0) && errno))
				প্রত্যাগম errno;
			যদি (c2 == 0 && --retries <= 0)
				ভঙ্গ; /* This should never happen...  */
		}
		গুনতি -= c1;
	}
	প্রত্যাগম 0;
}


পূর্ণাঙ্ক qcow2_write_raw_image(পূর্ণাঙ্ক qcow2_fd, পূর্ণাঙ্ক raw_fd,
			      সংরচনা ext2_qcow2_hdr *hdr)
{
	সংরচনা ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned পূর্ণাঙ্ক l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = রিক্ত;
	শূন্য *copy_buf = রিক্ত;
	size_t size;
	unsigned পূর্ণাঙ্ক max_l1_size;

	যদি (hdr->crypt_method)
		প্রত্যাগম -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = রিক্ত;
	img.l1_table = রিক্ত;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	যদি (img.cluster_bits < 9 || img.cluster_bits > 31)
		প্রত্যাগম -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	যদি (img.l1_offset & (img.cluster_size - 1))
		প্রত্যাগম -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	যদি (img.l1_size > max_l1_size)
		প্রত্যাগম -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	যদি (ret)
		গচ্ছ out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	যদি (ret)
		গচ্ছ out;

	যদি (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		গচ্ছ out;
	}

	ret = qcow2_read_l1_table(&img);
	যদি (ret)
		গচ্ছ out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	প্রতি (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		যদি ((offset > img.image_size) ||
		    (offset <= 0))
			অনুবর্ত;

		যদি (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			গচ্ছ out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		যদি (ret)
			ভঙ্গ;

		/* Walk through l2 table and copy data blocks into raw image */
		প্রতি (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			যদি (offset == 0)
				অনুবর্ত;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			যদি (ret)
				গচ্ছ out;
		}
	}

	/* Resize the output image to the filesystem size */
	যদি (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		গচ্ছ out;
	}

	((বর্ণ *)copy_buf)[0] = 0;
	size = লিখ(raw_fd, copy_buf, 1);
	যদি (size != 1) {
		ret = errno;
		গচ্ছ out;
	}

out:
	যদি (copy_buf)
		ext2fs_free_mem(&copy_buf);
	যদি (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	যদি (l2_table)
		ext2fs_free_mem(&l2_table);
	প্রত্যাগম ret;
}
