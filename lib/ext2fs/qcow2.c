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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <grp.h>
#അന്തര്ഭാവ <pwd.h>
#അന്തര്ഭാവ <stdio.h>
#ifdef HAVE_STDLIB_H
#അന്തര്ഭാവ <stdlib.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <assert.h>

#അന്തര്ഭാവ "ext2fs/ext2fs.h"
#അന്തര്ഭാവ "qcow2.h"

/* Functions for converting qcow2 image into raw image */

ഘടന ext2_qcow2_hdr *qcow2_read_header(പൂർണ്ണസംഖ്യ fd)
{
	ശൂന്യം *buffer = ശൂന്യ;
	ഘടന ext2_qcow2_hdr *hdr = ശൂന്യ;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_qcow2_hdr), &buffer);
	എങ്കിൽ (ret)
		തിരികെ ശൂന്യ;
	memset(buffer, 0, വലുപ്പം(ഘടന ext2_qcow2_hdr));

	എങ്കിൽ (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		തിരികെ ശൂന്യ;
	}

	size = പഠ(fd, buffer, വലുപ്പം(ഘടന ext2_qcow2_hdr));
	എങ്കിൽ (size != വലുപ്പം(ഘടന ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		തിരികെ ശൂന്യ;
	}

	hdr = (ഘടന ext2_qcow2_hdr *)(buffer);

	എങ്കിൽ ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		തിരികെ ശൂന്യ;
	}

	തിരികെ hdr;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ qcow2_read_l1_table(ഘടന ext2_qcow2_image *img)
{
	പൂർണ്ണസംഖ്യ fd = img->fd;
	size_t size, l1_size = img->l1_size * വലുപ്പം(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	എങ്കിൽ (ret)
		തിരികെ ret;

	എങ്കിൽ (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		തിരികെ errno;
	}

	size = പഠ(fd, table, l1_size);
	എങ്കിൽ (size != l1_size) {
		ext2fs_free_mem(&table);
		തിരികെ errno;
	}

	img->l1_table = table;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ qcow2_read_l2_table(ഘടന ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	പൂർണ്ണസംഖ്യ fd = img->fd;
	size_t size;

	പ്രതിജ്ഞാ(*l2_table);

	എങ്കിൽ (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		തിരികെ errno;

	size = പഠ(fd, *l2_table, img->cluster_size);
	എങ്കിൽ (size != img->cluster_size)
		തിരികെ errno;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ qcow2_copy_data(പൂർണ്ണസംഖ്യ fdin, പൂർണ്ണസംഖ്യ fdout, __u64 off_in,
			   __u64 off_out, ശൂന്യം *buf, size_t എണ്ണം)
{
	ssize_t c1, c2, c;
	ശൂന്യം *ptr;
	പൂർണ്ണസംഖ്യ retries = 10;

	പ്രതിജ്ഞാ(buf);

	എങ്കിൽ (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		തിരികെ errno;

	എങ്കിൽ (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		തിരികെ errno;

	ആകുമ്പോൾ (എണ്ണം > 0) {
		errno = 0;
		c1 = പഠ(fdin, buf, എണ്ണം);
		എങ്കിൽ (c1 < 0 || ((c1 == 0) && errno))
			തിരികെ errno;
		എങ്കിൽ (c1 == 0)
			നിർത്തുക; 	/* EOF */

		വേണ്ടി (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = ലിഖ(fdout, ptr, c1);
			എങ്കിൽ (c2 < 0 || ((c2 == 0) && errno))
				തിരികെ errno;
			എങ്കിൽ (c2 == 0 && --retries <= 0)
				നിർത്തുക; /* This should never happen...  */
		}
		എണ്ണം -= c1;
	}
	തിരികെ 0;
}


പൂർണ്ണസംഖ്യ qcow2_write_raw_image(പൂർണ്ണസംഖ്യ qcow2_fd, പൂർണ്ണസംഖ്യ raw_fd,
			      ഘടന ext2_qcow2_hdr *hdr)
{
	ഘടന ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned പൂർണ്ണസംഖ്യ l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = ശൂന്യ;
	ശൂന്യം *copy_buf = ശൂന്യ;
	size_t size;
	unsigned പൂർണ്ണസംഖ്യ max_l1_size;

	എങ്കിൽ (hdr->crypt_method)
		തിരികെ -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = ശൂന്യ;
	img.l1_table = ശൂന്യ;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	എങ്കിൽ (img.cluster_bits < 9 || img.cluster_bits > 31)
		തിരികെ -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	എങ്കിൽ (img.l1_offset & (img.cluster_size - 1))
		തിരികെ -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	എങ്കിൽ (img.l1_size > max_l1_size)
		തിരികെ -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	എങ്കിൽ (ret)
		പോകുക out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	എങ്കിൽ (ret)
		പോകുക out;

	എങ്കിൽ (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		പോകുക out;
	}

	ret = qcow2_read_l1_table(&img);
	എങ്കിൽ (ret)
		പോകുക out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	വേണ്ടി (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		എങ്കിൽ ((offset > img.image_size) ||
		    (offset <= 0))
			തുടരുക;

		എങ്കിൽ (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			പോകുക out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		എങ്കിൽ (ret)
			നിർത്തുക;

		/* Walk through l2 table and copy data blocks into raw image */
		വേണ്ടി (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			എങ്കിൽ (offset == 0)
				തുടരുക;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			എങ്കിൽ (ret)
				പോകുക out;
		}
	}

	/* Resize the output image to the filesystem size */
	എങ്കിൽ (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		പോകുക out;
	}

	((അക്ഷരം *)copy_buf)[0] = 0;
	size = ലിഖ(raw_fd, copy_buf, 1);
	എങ്കിൽ (size != 1) {
		ret = errno;
		പോകുക out;
	}

out:
	എങ്കിൽ (copy_buf)
		ext2fs_free_mem(&copy_buf);
	എങ്കിൽ (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	എങ്കിൽ (l2_table)
		ext2fs_free_mem(&l2_table);
	തിരികെ ret;
}
