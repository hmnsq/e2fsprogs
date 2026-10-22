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

#ضمن "config.h"
#ضمن <fcntl.h>
#ضمن <grp.h>
#ضمن <pwd.h>
#ضمن <stdio.h>
#ifdef HAVE_STDLIB_H
#ضمن <stdlib.h>
#endif
#ضمن <string.h>
#ضمن <time.h>
#ضمن <unistd.h>
#ضمن <fcntl.h>
#ضمن <errno.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <assert.h>

#ضمن "ext2fs/ext2fs.h"
#ضمن "qcow2.h"

/* Functions for converting qcow2 image into raw image */

بنية ext2_qcow2_hdr *qcow2_read_header(صحيح fd)
{
	فراغ *buffer = عدم;
	بنية ext2_qcow2_hdr *hdr = عدم;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(حجم(بنية ext2_qcow2_hdr), &buffer);
	𐤀𐤌 (ret)
		أرجع عدم;
	memset(buffer, 0, حجم(بنية ext2_qcow2_hdr));

	𐤀𐤌 (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		أرجع عدم;
	}

	size = اقرأ(fd, buffer, حجم(بنية ext2_qcow2_hdr));
	𐤀𐤌 (size != حجم(بنية ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		أرجع عدم;
	}

	hdr = (بنية ext2_qcow2_hdr *)(buffer);

	𐤀𐤌 ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		أرجع عدم;
	}

	أرجع hdr;
}

ساكن صحيح qcow2_read_l1_table(بنية ext2_qcow2_image *img)
{
	صحيح fd = img->fd;
	size_t size, l1_size = img->l1_size * حجم(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	𐤀𐤌 (ret)
		أرجع ret;

	𐤀𐤌 (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		أرجع errno;
	}

	size = اقرأ(fd, table, l1_size);
	𐤀𐤌 (size != l1_size) {
		ext2fs_free_mem(&table);
		أرجع errno;
	}

	img->l1_table = table;

	أرجع 0;
}

ساكن صحيح qcow2_read_l2_table(بنية ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	صحيح fd = img->fd;
	size_t size;

	أكد(*l2_table);

	𐤀𐤌 (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		أرجع errno;

	size = اقرأ(fd, *l2_table, img->cluster_size);
	𐤀𐤌 (size != img->cluster_size)
		أرجع errno;

	أرجع 0;
}

ساكن صحيح qcow2_copy_data(صحيح fdin, صحيح fdout, __u64 off_in,
			   __u64 off_out, فراغ *buf, size_t count)
{
	ssize_t c1, c2, c;
	فراغ *ptr;
	صحيح retries = 10;

	أكد(buf);

	𐤀𐤌 (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		أرجع errno;

	𐤀𐤌 (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		أرجع errno;

	طالما (count > 0) {
		errno = 0;
		c1 = اقرأ(fdin, buf, count);
		𐤀𐤌 (c1 < 0 || ((c1 == 0) && errno))
			أرجع errno;
		𐤀𐤌 (c1 == 0)
			اكسر; 	/* EOF */

		لكل (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = اكتب(fdout, ptr, c1);
			𐤀𐤌 (c2 < 0 || ((c2 == 0) && errno))
				أرجع errno;
			𐤀𐤌 (c2 == 0 && --retries <= 0)
				اكسر; /* This should never happen...  */
		}
		count -= c1;
	}
	أرجع 0;
}


صحيح qcow2_write_raw_image(صحيح qcow2_fd, صحيح raw_fd,
			      بنية ext2_qcow2_hdr *hdr)
{
	بنية ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned صحيح l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = عدم;
	فراغ *copy_buf = عدم;
	size_t size;
	unsigned صحيح max_l1_size;

	𐤀𐤌 (hdr->crypt_method)
		أرجع -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = عدم;
	img.l1_table = عدم;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	𐤀𐤌 (img.cluster_bits < 9 || img.cluster_bits > 31)
		أرجع -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	𐤀𐤌 (img.l1_offset & (img.cluster_size - 1))
		أرجع -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	𐤀𐤌 (img.l1_size > max_l1_size)
		أرجع -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	𐤀𐤌 (ret)
		اذهب out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	𐤀𐤌 (ret)
		اذهب out;

	𐤀𐤌 (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		اذهب out;
	}

	ret = qcow2_read_l1_table(&img);
	𐤀𐤌 (ret)
		اذهب out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	لكل (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		𐤀𐤌 ((offset > img.image_size) ||
		    (offset <= 0))
			استمر;

		𐤀𐤌 (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			اذهب out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		𐤀𐤌 (ret)
			اكسر;

		/* Walk through l2 table and copy data blocks into raw image */
		لكل (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			𐤀𐤌 (offset == 0)
				استمر;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			𐤀𐤌 (ret)
				اذهب out;
		}
	}

	/* Resize the output image to the filesystem size */
	𐤀𐤌 (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		اذهب out;
	}

	((حرف *)copy_buf)[0] = 0;
	size = اكتب(raw_fd, copy_buf, 1);
	𐤀𐤌 (size != 1) {
		ret = errno;
		اذهب out;
	}

out:
	𐤀𐤌 (copy_buf)
		ext2fs_free_mem(&copy_buf);
	𐤀𐤌 (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	𐤀𐤌 (l2_table)
		ext2fs_free_mem(&l2_table);
	أرجع ret;
}
