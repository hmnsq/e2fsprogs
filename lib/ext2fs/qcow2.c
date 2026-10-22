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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <grp.h>
#અન્તર્ભાવ <pwd.h>
#અન્તર્ભાવ <stdio.h>
#ifdef HAVE_STDLIB_H
#અન્તર્ભાવ <stdlib.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <assert.h>

#અન્તર્ભાવ "ext2fs/ext2fs.h"
#અન્તર્ભાવ "qcow2.h"

/* Functions for converting qcow2 image into raw image */

સંરચના ext2_qcow2_hdr *qcow2_read_header(પૂર્ણાંક fd)
{
	શૂન્ય *buffer = ખાલી;
	સંરચના ext2_qcow2_hdr *hdr = ખાલી;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(કદ(સંરચના ext2_qcow2_hdr), &buffer);
	જો (ret)
		પાછા ખાલી;
	memset(buffer, 0, કદ(સંરચના ext2_qcow2_hdr));

	જો (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		પાછા ખાલી;
	}

	size = પઠ(fd, buffer, કદ(સંરચના ext2_qcow2_hdr));
	જો (size != કદ(સંરચના ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		પાછા ખાલી;
	}

	hdr = (સંરચના ext2_qcow2_hdr *)(buffer);

	જો ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		પાછા ખાલી;
	}

	પાછા hdr;
}

સ્થિર પૂર્ણાંક qcow2_read_l1_table(સંરચના ext2_qcow2_image *img)
{
	પૂર્ણાંક fd = img->fd;
	size_t size, l1_size = img->l1_size * કદ(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	જો (ret)
		પાછા ret;

	જો (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		પાછા errno;
	}

	size = પઠ(fd, table, l1_size);
	જો (size != l1_size) {
		ext2fs_free_mem(&table);
		પાછા errno;
	}

	img->l1_table = table;

	પાછા 0;
}

સ્થિર પૂર્ણાંક qcow2_read_l2_table(સંરચના ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	પૂર્ણાંક fd = img->fd;
	size_t size;

	પ્રતિજ્ઞા(*l2_table);

	જો (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		પાછા errno;

	size = પઠ(fd, *l2_table, img->cluster_size);
	જો (size != img->cluster_size)
		પાછા errno;

	પાછા 0;
}

સ્થિર પૂર્ણાંક qcow2_copy_data(પૂર્ણાંક fdin, પૂર્ણાંક fdout, __u64 off_in,
			   __u64 off_out, શૂન્ય *buf, size_t ગણતરી)
{
	ssize_t c1, c2, c;
	શૂન્ય *ptr;
	પૂર્ણાંક retries = 10;

	પ્રતિજ્ઞા(buf);

	જો (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		પાછા errno;

	જો (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		પાછા errno;

	જ્યારે (ગણતરી > 0) {
		errno = 0;
		c1 = પઠ(fdin, buf, ગણતરી);
		જો (c1 < 0 || ((c1 == 0) && errno))
			પાછા errno;
		જો (c1 == 0)
			તોડો; 	/* EOF */

		માટે (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = લિખ(fdout, ptr, c1);
			જો (c2 < 0 || ((c2 == 0) && errno))
				પાછા errno;
			જો (c2 == 0 && --retries <= 0)
				તોડો; /* This should never happen...  */
		}
		ગણતરી -= c1;
	}
	પાછા 0;
}


પૂર્ણાંક qcow2_write_raw_image(પૂર્ણાંક qcow2_fd, પૂર્ણાંક raw_fd,
			      સંરચના ext2_qcow2_hdr *hdr)
{
	સંરચના ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned પૂર્ણાંક l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = ખાલી;
	શૂન્ય *copy_buf = ખાલી;
	size_t size;
	unsigned પૂર્ણાંક max_l1_size;

	જો (hdr->crypt_method)
		પાછા -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = ખાલી;
	img.l1_table = ખાલી;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	જો (img.cluster_bits < 9 || img.cluster_bits > 31)
		પાછા -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	જો (img.l1_offset & (img.cluster_size - 1))
		પાછા -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	જો (img.l1_size > max_l1_size)
		પાછા -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	જો (ret)
		જાઓ out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	જો (ret)
		જાઓ out;

	જો (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		જાઓ out;
	}

	ret = qcow2_read_l1_table(&img);
	જો (ret)
		જાઓ out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	માટે (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		જો ((offset > img.image_size) ||
		    (offset <= 0))
			ચાલુ;

		જો (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			જાઓ out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		જો (ret)
			તોડો;

		/* Walk through l2 table and copy data blocks into raw image */
		માટે (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			જો (offset == 0)
				ચાલુ;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			જો (ret)
				જાઓ out;
		}
	}

	/* Resize the output image to the filesystem size */
	જો (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		જાઓ out;
	}

	((અક્ષર *)copy_buf)[0] = 0;
	size = લિખ(raw_fd, copy_buf, 1);
	જો (size != 1) {
		ret = errno;
		જાઓ out;
	}

out:
	જો (copy_buf)
		ext2fs_free_mem(&copy_buf);
	જો (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	જો (l2_table)
		ext2fs_free_mem(&l2_table);
	પાછા ret;
}
