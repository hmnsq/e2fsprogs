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

#अन्तर्भाव "config.h"
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <grp.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <stdio.h>
#ifdef HAVE_STDLIB_H
#अन्तर्भाव <stdlib.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <assert.h>

#अन्तर्भाव "ext2fs/ext2fs.h"
#अन्तर्भाव "qcow2.h"

/* Functions for converting qcow2 image into raw image */

संरचना ext2_qcow2_hdr *qcow2_read_header(पूर्णाङ्क fd)
{
	शून्य *buffer = रिक्त;
	संरचना ext2_qcow2_hdr *hdr = रिक्त;
	size_t size;
	errcode_t ret;

	ret = ext2fs_get_mem(परिमाण(संरचना ext2_qcow2_hdr), &buffer);
	यदि (ret)
		प्रत्यागम रिक्त;
	memset(buffer, 0, परिमाण(संरचना ext2_qcow2_hdr));

	यदि (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&buffer);
		प्रत्यागम रिक्त;
	}

	size = पठ(fd, buffer, परिमाण(संरचना ext2_qcow2_hdr));
	यदि (size != परिमाण(संरचना ext2_qcow2_hdr)) {
		ext2fs_free_mem(&buffer);
		प्रत्यागम रिक्त;
	}

	hdr = (संरचना ext2_qcow2_hdr *)(buffer);

	यदि ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		प्रत्यागम रिक्त;
	}

	प्रत्यागम hdr;
}

स्थिर पूर्णाङ्क qcow2_read_l1_table(संरचना ext2_qcow2_image *img)
{
	पूर्णाङ्क fd = img->fd;
	size_t size, l1_size = img->l1_size * परिमाण(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_size, &table);
	यदि (ret)
		प्रत्यागम ret;

	यदि (ext2fs_llseek(fd, img->l1_offset, SEEK_SET) < 0) {
		ext2fs_free_mem(&table);
		प्रत्यागम errno;
	}

	size = पठ(fd, table, l1_size);
	यदि (size != l1_size) {
		ext2fs_free_mem(&table);
		प्रत्यागम errno;
	}

	img->l1_table = table;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क qcow2_read_l2_table(संरचना ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	पूर्णाङ्क fd = img->fd;
	size_t size;

	प्रतिज्ञा(*l2_table);

	यदि (ext2fs_llseek(fd, offset, SEEK_SET) < 0)
		प्रत्यागम errno;

	size = पठ(fd, *l2_table, img->cluster_size);
	यदि (size != img->cluster_size)
		प्रत्यागम errno;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क qcow2_copy_data(पूर्णाङ्क fdin, पूर्णाङ्क fdout, __u64 off_in,
			   __u64 off_out, शून्य *buf, size_t मोजणी)
{
	ssize_t c1, c2, c;
	शून्य *ptr;
	पूर्णाङ्क retries = 10;

	प्रतिज्ञा(buf);

	यदि (ext2fs_llseek(fdout, off_out, SEEK_SET) < 0)
		प्रत्यागम errno;

	यदि (ext2fs_llseek(fdin, off_in, SEEK_SET) < 0)
		प्रत्यागम errno;

	यावत् (मोजणी > 0) {
		errno = 0;
		c1 = पठ(fdin, buf, मोजणी);
		यदि (c1 < 0 || ((c1 == 0) && errno))
			प्रत्यागम errno;
		यदि (c1 == 0)
			भङ्ग; 	/* EOF */

		प्रति (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = लिख(fdout, ptr, c1);
			यदि (c2 < 0 || ((c2 == 0) && errno))
				प्रत्यागम errno;
			यदि (c2 == 0 && --retries <= 0)
				भङ्ग; /* This should never happen...  */
		}
		मोजणी -= c1;
	}
	प्रत्यागम 0;
}


पूर्णाङ्क qcow2_write_raw_image(पूर्णाङ्क qcow2_fd, पूर्णाङ्क raw_fd,
			      संरचना ext2_qcow2_hdr *hdr)
{
	संरचना ext2_qcow2_image img;
	errcode_t ret = 0;
	unsigned पूर्णाङ्क l1_index, l2_index;
	__u64 offset;
	blk64_t *l1_table, *l2_table = रिक्त;
	शून्य *copy_buf = रिक्त;
	size_t size;
	unsigned पूर्णाङ्क max_l1_size;

	यदि (hdr->crypt_method)
		प्रत्यागम -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = रिक्त;
	img.l1_table = रिक्त;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	यदि (img.cluster_bits < 9 || img.cluster_bits > 31)
		प्रत्यागम -QCOW_CORRUPTED;
	img.cluster_size = 1 << img.cluster_bits;
	img.l1_size = ext2fs_be32_to_cpu(hdr->l1_size);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_size = 1 << (img.cluster_bits - 3);
	img.image_size = ext2fs_be64_to_cpu(hdr->size);

	यदि (img.l1_offset & (img.cluster_size - 1))
		प्रत्यागम -QCOW_CORRUPTED;

	max_l1_size = (img.image_size >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_size;
	यदि (img.l1_size > max_l1_size)
		प्रत्यागम -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_size, &l2_table);
	यदि (ret)
		गच्छ out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	यदि (ret)
		गच्छ out;

	यदि (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		गच्छ out;
	}

	ret = qcow2_read_l1_table(&img);
	यदि (ret)
		गच्छ out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	प्रति (l1_index = 0; l1_index < img.l1_size; l1_index++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_index]) &
			 ~QCOW_OFLAG_COPIED;

		यदि ((offset > img.image_size) ||
		    (offset <= 0))
			अनुवर्त;

		यदि (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			गच्छ out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		यदि (ret)
			भङ्ग;

		/* Walk through l2 table and copy data blocks into raw image */
		प्रति (l2_index = 0; l2_index < img.l2_size; l2_index++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_index]) &
				 ~QCOW_OFLAG_COPIED;

			यदि (offset == 0)
				अनुवर्त;

			off_out = ((__u64)l1_index * img.l2_size) +
				  l2_index;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_data(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_size);
			यदि (ret)
				गच्छ out;
		}
	}

	/* Resize the output image to the filesystem size */
	यदि (ext2fs_llseek(raw_fd, img.image_size - 1, SEEK_SET) < 0) {
		ret = errno;
		गच्छ out;
	}

	((वर्ण *)copy_buf)[0] = 0;
	size = लिख(raw_fd, copy_buf, 1);
	यदि (size != 1) {
		ret = errno;
		गच्छ out;
	}

out:
	यदि (copy_buf)
		ext2fs_free_mem(&copy_buf);
	यदि (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	यदि (l2_table)
		ext2fs_free_mem(&l2_table);
	प्रत्यागम ret;
}
