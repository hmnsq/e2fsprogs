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

কাঠামো ext2_qcow2_hdr *qcow2_read_header(পূর্ণসংখ্যা fd)
{
	শূন্য *বাফার = ফাঁকা;
	কাঠামো ext2_qcow2_hdr *hdr = ফাঁকা;
	size_t আকার_2;
	errcode_t ret;

	ret = ext2fs_get_mem(আকার(কাঠামো ext2_qcow2_hdr), &বাফার);
	যদি (ret)
		ফেরত ফাঁকা;
	memset(বাফার, 0, আকার(কাঠামো ext2_qcow2_hdr));

	যদি (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&বাফার);
		ফেরত ফাঁকা;
	}

	আকার_2 = পঠ(fd, বাফার, আকার(কাঠামো ext2_qcow2_hdr));
	যদি (আকার_2 != আকার(কাঠামো ext2_qcow2_hdr)) {
		ext2fs_free_mem(&বাফার);
		ফেরত ফাঁকা;
	}

	hdr = (কাঠামো ext2_qcow2_hdr *)(বাফার);

	যদি ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		ফেরত ফাঁকা;
	}

	ফেরত hdr;
}

স্থির পূর্ণসংখ্যা qcow2_read_l1_table(কাঠামো ext2_qcow2_image *img)
{
	পূর্ণসংখ্যা fd = img->fd;
	size_t আকার_2, l1_আকার = img->l1_আকার * আকার(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_আকার, &table);
	যদি (ret)
		ফেরত ret;

	যদি (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		ফেরত errno;
	}

	আকার_2 = পঠ(fd, table, l1_আকার);
	যদি (আকার_2 != l1_আকার) {
		ext2fs_free_mem(&table);
		ফেরত errno;
	}

	img->l1_table = table;

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা qcow2_read_l2_table(কাঠামো ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	পূর্ণসংখ্যা fd = img->fd;
	size_t আকার_2;

	প্রতিজ্ঞা(*l2_table);

	যদি (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		ফেরত errno;

	আকার_2 = পঠ(fd, *l2_table, img->cluster_আকার);
	যদি (আকার_2 != img->cluster_আকার)
		ফেরত errno;

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা qcow2_copy_তথ্য(পূর্ণসংখ্যা fdin, পূর্ণসংখ্যা fdout, __u64 off_in,
			   __u64 off_out, শূন্য *buf, size_t গুনতি)
{
	ssize_t c1, c2, c;
	শূন্য *ptr;
	পূর্ণসংখ্যা retries = 10;

	প্রতিজ্ঞা(buf);

	যদি (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		ফেরত errno;

	যদি (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		ফেরত errno;

	যখন (গুনতি > 0) {
		errno = 0;
		c1 = পঠ(fdin, buf, গুনতি);
		যদি (c1 < 0 || ((c1 == 0) && errno))
			ফেরত errno;
		যদি (c1 == 0)
			বিরতি; 	/* EOF */

		জন্য (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = লিখ(fdout, ptr, c1);
			যদি (c2 < 0 || ((c2 == 0) && errno))
				ফেরত errno;
			যদি (c2 == 0 && --retries <= 0)
				বিরতি; /* This should never happen...  */
		}
		গুনতি -= c1;
	}
	ফেরত 0;
}


পূর্ণসংখ্যা qcow2_write_raw_image(পূর্ণসংখ্যা qcow2_fd, পূর্ণসংখ্যা raw_fd,
			      কাঠামো ext2_qcow2_hdr *hdr)
{
	কাঠামো ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned পূর্ণসংখ্যা l1_সূচক, l2_সূচক;
	__u64 offset;
	blk64_t *l1_table, *l2_table = ফাঁকা;
	শূন্য *copy_buf = ফাঁকা;
	size_t আকার_2;
	unsigned পূর্ণসংখ্যা max_l1_আকার;

	যদি (hdr->crypt_method)
		ফেরত -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = ফাঁকা;
	img.l1_table = ফাঁকা;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	যদি (img.cluster_bits < 9 || img.cluster_bits > 31)
		ফেরত -QCOW_CORRUPTED;
	img.cluster_আকার = 1 << img.cluster_bits;
	img.l1_আকার = ext2fs_be32_to_cpu(hdr->l1_আকার);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_আকার = 1 << (img.cluster_bits - 3);
	img.image_আকার = ext2fs_be64_to_cpu(hdr->আকার_2);

	যদি (img.l1_offset & (img.cluster_আকার - 1))
		ফেরত -QCOW_CORRUPTED;

	max_l1_আকার = (img.image_আকার >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_আকার;
	যদি (img.l1_আকার > max_l1_আকার)
		ফেরত -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_আকার, &l2_table);
	যদি (ret)
		যাও out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	যদি (ret)
		যাও out;

	যদি (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		যাও out;
	}

	ret = qcow2_read_l1_table(&img);
	যদি (ret)
		যাও out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	জন্য (l1_সূচক = 0; l1_সূচক < img.l1_আকার; l1_সূচক++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_সূচক]) &
			 ~QCOW_OFLAG_COPIED;

		যদি ((offset > img.image_আকার) ||
		    (offset <= 0))
			চালু;

		যদি (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			যাও out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		যদি (ret)
			বিরতি;

		/* Walk through l2 table and copy data blocks into raw image */
		জন্য (l2_সূচক = 0; l2_সূচক < img.l2_আকার; l2_সূচক++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_সূচক]) &
				 ~QCOW_OFLAG_COPIED;

			যদি (offset == 0)
				চালু;

			off_out = ((__u64)l1_সূচক * img.l2_আকার) +
				  l2_সূচক;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_তথ্য(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_আকার);
			যদি (ret)
				যাও out;
		}
	}

	/* Resize the output image to the filesystem size */
	যদি (ext2fs_llseek(raw_fd, img.image_আকার - 1, SEEK_SET) < 0) {
		ret = errno;
		যাও out;
	}

	((অক্ষর *)copy_buf)[0] = 0;
	আকার_2 = লিখ(raw_fd, copy_buf, 1);
	যদি (আকার_2 != 1) {
		ret = errno;
		যাও out;
	}

out:
	যদি (copy_buf)
		ext2fs_free_mem(&copy_buf);
	যদি (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	যদি (l2_table)
		ext2fs_free_mem(&l2_table);
	ফেরত ret;
}
