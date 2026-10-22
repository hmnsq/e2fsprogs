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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <grp.h>
#ଅନ୍ତର୍ଭାଵ <pwd.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ifdef HAVE_STDLIB_H
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>

#ଅନ୍ତର୍ଭାଵ "ext2fs/ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "qcow2.h"

/* Functions for converting qcow2 image into raw image */

ସଂରଚନା ext2_qcow2_hdr *qcow2_read_header(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
	ଶୂନ୍ୟ *buffer = ଖାଲି;
	ସଂରଚନା ext2_qcow2_hdr *hdr = ଖାଲି;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_qcow2_hdr), &buffer);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ଖାଲି;
	memset(buffer, 0, ଆକାର(ସଂରଚନା ext2_qcow2_hdr));

	ଯଦି (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		ଫେରନ୍ତୁ ଖାଲି;
	}

	size = ପଠ(fd, buffer, ଆକାର(ସଂରଚନା ext2_qcow2_hdr));
	ଯଦି (size != ଆକାର(ସଂରଚନା ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		ଫେରନ୍ତୁ ଖାଲି;
	}

	hdr = (ସଂରଚନା ext2_qcow2_hdr *)(buffer);

	ଯଦି ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		ଫେରନ୍ତୁ ଖାଲି;
	}

	ଫେରନ୍ତୁ hdr;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା qcow2_read_l1_table(ସଂରଚନା ext2_qcow2_image *img)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = img->fd;
	size_t size, l1_size = img->l1_size * ଆକାର(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	ଯଦି (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		ଫେରନ୍ତୁ errno;
	}

	size = ପଠ(fd, table, l1_size);
	ଯଦି (size != l1_size) {
		ext2fs_free_mem(&table);
		ଫେରନ୍ତୁ errno;
	}

	img->l1_table = table;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା qcow2_read_l2_table(ସଂରଚନା ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = img->fd;
	size_t size;

	ପ୍ରତିଜ୍ଞା(*l2_table);

	ଯଦି (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		ଫେରନ୍ତୁ errno;

	size = ପଠ(fd, *l2_table, img->cluster_size);
	ଯଦି (size != img->cluster_size)
		ଫେରନ୍ତୁ errno;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା qcow2_copy_data(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdin, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdout, __u64 off_in,
			   __u64 off_out, ଶୂନ୍ୟ *buf, size_t ଗଣନା_2)
{
	ssize_t c1, c2, c;
	ଶୂନ୍ୟ *ptr;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retries = 10;

	ପ୍ରତିଜ୍ଞା(buf);

	ଯଦି (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		ଫେରନ୍ତୁ errno;

	ଯଦି (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		ଫେରନ୍ତୁ errno;

	ଯେତେବେଳେ (ଗଣନା_2 > 0) {
		errno = 0;
		c1 = ପଠ(fdin, buf, ଗଣନା_2);
		ଯଦି (c1 < 0 || ((c1 == 0) && errno))
			ଫେରନ୍ତୁ errno;
		ଯଦି (c1 == 0)
			ଭାଙ୍ଗ; 	/* EOF */

		ପାଇଁ (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = ଲିଖ(fdout, ptr, c1);
			ଯଦି (c2 < 0 || ((c2 == 0) && errno))
				ଫେରନ୍ତୁ errno;
			ଯଦି (c2 == 0 && --retries <= 0)
				ଭାଙ୍ଗ; /* This should never happen...  */
		}
		ଗଣନା_2 -= c1;
	}
	ଫେରନ୍ତୁ 0;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା qcow2_write_raw_image(ପୂର୍ଣ୍ଣସଂଖ୍ୟା qcow2_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା raw_fd,
			      ସଂରଚନା ext2_qcow2_hdr *hdr)
{
	ସଂରଚନା ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = ଖାଲି;
	ଶୂନ୍ୟ *copy_buf = ଖାଲି;
	size_t size;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_l1_size;

	ଯଦି (hdr->crypt_method)
		ଫେରନ୍ତୁ -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = ଖାଲି;
	img.l1_table = ଖାଲି;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	ଯଦି (img.cluster_bits < 9 || img.cluster_bits > 31)
		ଫେରନ୍ତୁ -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	ଯଦି (img.l1_offset & (img.cluster_size - 1))
		ଫେରନ୍ତୁ -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	ଯଦି (img.l1_size > max_l1_size)
		ଫେରନ୍ତୁ -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	ଯଦି (ret)
		ଯାଅ out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	ଯଦି (ret)
		ଯାଅ out;

	ଯଦି (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		ଯାଅ out;
	}

	ret = qcow2_read_l1_table(&img);
	ଯଦି (ret)
		ଯାଅ out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	ପାଇଁ (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		ଯଦି ((offset > img.image_size) ||
		    (offset <= 0))
			ଜାରି;

		ଯଦି (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			ଯାଅ out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		ଯଦି (ret)
			ଭାଙ୍ଗ;

		/* Walk through l2 table and copy data blocks into raw image */
		ପାଇଁ (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			ଯଦି (offset == 0)
				ଜାରି;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			ଯଦି (ret)
				ଯାଅ out;
		}
	}

	/* Resize the output image to the filesystem size */
	ଯଦି (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		ଯାଅ out;
	}

	((ଅକ୍ଷର *)copy_buf)[0] = 0;
	size = ଲିଖ(raw_fd, copy_buf, 1);
	ଯଦି (size != 1) {
		ret = errno;
		ଯାଅ out;
	}

out:
	ଯଦି (copy_buf)
		ext2fs_free_mem(&copy_buf);
	ଯଦି (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	ଯଦି (l2_table)
		ext2fs_free_mem(&l2_table);
	ଫେରନ୍ତୁ ret;
}
