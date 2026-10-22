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

#جيڪڏهن_وصف_نه_ٿيل LARGEFILE_ذريعو
#وصف_ڪريو LARGEFILE_ذريعو
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل LARGEFILE64_ذريعو
#وصف_ڪريو LARGEFILE64_ذريعو
#جيڪڏهن_پورو

#ضمن "config.h"
#ضمن <fcntl.h>
#ضمن <grp.h>
#ضمن <pwd.h>
#ضمن <stdio.h>
#جيڪڏهن_وصف_ٿيل HAVE_STDLIB_H
#ضمن <stdlib.h>
#جيڪڏهن_پورو
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

بنية ext2_qcow2_hdr *qcow2_read_سرو(صحيح fd)
{
	فراغ *بفر = عدم;
	بنية ext2_qcow2_hdr *hdr = عدم;
	size_t سائيز;
	errcode_t ret;

	ret = ext2fs_get_mem(حجم(بنية ext2_qcow2_hdr), &بفر);
	إذا (ret)
		أرجع عدم;
	memset(بفر, 0, حجم(بنية ext2_qcow2_hdr));

	إذا (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&بفر);
		أرجع عدم;
	}

	سائيز = اقرأ(fd, بفر, حجم(بنية ext2_qcow2_hdr));
	إذا (سائيز != حجم(بنية ext2_qcow2_hdr)) {
		ext2fs_free_mem(&بفر);
		أرجع عدم;
	}

	hdr = (بنية ext2_qcow2_hdr *)(بفر);

	إذا ((ext2fs_be32_منزل_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_منزل_cpu(hdr->نسخو) != 2)) {
		ext2fs_free_mem(&hdr);
		أرجع عدم;
	}

	أرجع hdr;
}

ساكن صحيح qcow2_read_l1_جدول(بنية ext2_qcow2_image *img)
{
	صحيح fd = img->fd;
	size_t سائيز, l1_سائيز = img->l1_سائيز * حجم(blk64_t);
	blk64_t *جدول;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_سائيز, &جدول);
	إذا (ret)
		أرجع ret;

	إذا (ext2fs_llseek(fd, img->l1_وٿي, SEEK_SET) < 0) {
		ext2fs_free_mem(&جدول);
		أرجع errno;
	}

	سائيز = اقرأ(fd, جدول, l1_سائيز);
	إذا (سائيز != l1_سائيز) {
		ext2fs_free_mem(&جدول);
		أرجع errno;
	}

	img->l1_جدول = جدول;

	أرجع 0;
}

ساكن صحيح qcow2_read_l2_جدول(بنية ext2_qcow2_image *img,
			       __u64 وٿي, blk64_t **l2_جدول)
{
	صحيح fd = img->fd;
	size_t سائيز;

	أكد(*l2_جدول);

	إذا (ext2fs_llseek(fd, وٿي, SEEK_SET) < 0)
		أرجع errno;

	سائيز = اقرأ(fd, *l2_جدول, img->cluster_سائيز);
	إذا (سائيز != img->cluster_سائيز)
		أرجع errno;

	أرجع 0;
}

ساكن صحيح qcow2_copy_معلومات(صحيح fdin, صحيح fdout, __u64 off_اندريون,
			   __u64 off_ٻاهريون, فراغ *buf, size_t ڳڻپ)
{
	ssize_t c1, c2, c;
	فراغ *ptr;
	صحيح retries = 10;

	أكد(buf);

	إذا (ext2fs_llseek(fdout, off_ٻاهريون, SEEK_SET) < 0)
		أرجع errno;

	إذا (ext2fs_llseek(fdin, off_اندريون, SEEK_SET) < 0)
		أرجع errno;

	طالما (ڳڻپ > 0) {
		errno = 0;
		c1 = اقرأ(fdin, buf, ڳڻپ);
		إذا (c1 < 0 || ((c1 == 0) && errno))
			أرجع errno;
		إذا (c1 == 0)
			اكسر; 	/* EOF */

		لكل (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = اكتب(fdout, ptr, c1);
			إذا (c2 < 0 || ((c2 == 0) && errno))
				أرجع errno;
			إذا (c2 == 0 && --retries <= 0)
				اكسر; /* This should never happen...  */
		}
		ڳڻپ -= c1;
	}
	أرجع 0;
}


صحيح qcow2_write_raw_image(صحيح qcow2_fd, صحيح raw_fd,
			      بنية ext2_qcow2_hdr *hdr)
{
	بنية ext2_qcow2_image img;
	errcode_t ret = 0;
	بنانشان صحيح l1_اشاريو, l2_اشاريو;
	__u64 وٿي;
	blk64_t *l1_جدول, *l2_جدول = عدم;
	فراغ *copy_buf = عدم;
	size_t سائيز;
	بنانشان صحيح max_l1_سائيز;

	إذا (hdr->crypt_طريقو)
		أرجع -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = عدم;
	img.l1_جدول = عدم;
	img.cluster_بٽ = ext2fs_be32_منزل_cpu(hdr->cluster_بٽ);
	إذا (img.cluster_بٽ < 9 || img.cluster_بٽ > 31)
		أرجع -QCOW_CORRUPTED;
	img.cluster_سائيز = 1 << img.cluster_بٽ;
	img.l1_سائيز = ext2fs_be32_منزل_cpu(hdr->l1_سائيز);
	img.l1_وٿي = ext2fs_be64_منزل_cpu(hdr->l1_جدول_وٿي);
	img.l2_سائيز = 1 << (img.cluster_بٽ - 3);
	img.image_سائيز = ext2fs_be64_منزل_cpu(hdr->سائيز);

	إذا (img.l1_وٿي & (img.cluster_سائيز - 1))
		أرجع -QCOW_CORRUPTED;

	max_l1_سائيز = (img.image_سائيز >> ((2 * img.cluster_بٽ) - 3)) +
		img.cluster_سائيز;
	إذا (img.l1_سائيز > max_l1_سائيز)
		أرجع -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_سائيز, &l2_جدول);
	إذا (ret)
		اذهب ٻاهريون;

	ret = ext2fs_get_memzero(1 << img.cluster_بٽ, &copy_buf);
	إذا (ret)
		اذهب ٻاهريون;

	إذا (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		اذهب ٻاهريون;
	}

	ret = qcow2_read_l1_جدول(&img);
	إذا (ret)
		اذهب ٻاهريون;

	l1_جدول = img.l1_جدول;
	/* Walk through l1 table */
	لكل (l1_اشاريو = 0; l1_اشاريو < img.l1_سائيز; l1_اشاريو++) {
		__u64 off_ٻاهريون;

		وٿي = ext2fs_be64_منزل_cpu(l1_جدول[l1_اشاريو]) &
			 ~QCOW_OFLAG_COPIED;

		إذا ((وٿي > img.image_سائيز) ||
		    (وٿي <= 0))
			استمر;

		إذا (وٿي & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			اذهب ٻاهريون;
		}

		ret = qcow2_read_l2_جدول(&img, وٿي, &l2_جدول);
		إذا (ret)
			اكسر;

		/* Walk through l2 table and copy data blocks into raw image */
		لكل (l2_اشاريو = 0; l2_اشاريو < img.l2_سائيز; l2_اشاريو++) {
			وٿي = ext2fs_be64_منزل_cpu(l2_جدول[l2_اشاريو]) &
				 ~QCOW_OFLAG_COPIED;

			إذا (وٿي == 0)
				استمر;

			off_ٻاهريون = ((__u64)l1_اشاريو * img.l2_سائيز) +
				  l2_اشاريو;
			off_ٻاهريون <<= img.cluster_بٽ;
			ret = qcow2_copy_معلومات(qcow2_fd, raw_fd, وٿي,
					off_ٻاهريون, copy_buf, img.cluster_سائيز);
			إذا (ret)
				اذهب ٻاهريون;
		}
	}

	/* Resize the output image to the filesystem size */
	إذا (ext2fs_llseek(raw_fd, img.image_سائيز - 1, SEEK_SET) < 0) {
		ret = errno;
		اذهب ٻاهريون;
	}

	((حرف *)copy_buf)[0] = 0;
	سائيز = اكتب(raw_fd, copy_buf, 1);
	إذا (سائيز != 1) {
		ret = errno;
		اذهب ٻاهريون;
	}

ٻاهريون:
	إذا (copy_buf)
		ext2fs_free_mem(&copy_buf);
	إذا (img.l1_جدول)
		ext2fs_free_mem(&img.l1_جدول);
	إذا (l2_جدول)
		ext2fs_free_mem(&l2_جدول);
	أرجع ret;
}
