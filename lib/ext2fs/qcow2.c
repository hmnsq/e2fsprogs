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

#جے_بنا_تعریف LARGEFILE_سروت
#تعریف_کرو LARGEFILE_سروت
#جے_انت
#جے_بنا_تعریف LARGEFILE64_سروت
#تعریف_کرو LARGEFILE64_سروت
#جے_انت

#ضمن "config.h"
#ضمن <fcntl.h>
#ضمن <grp.h>
#ضمن <pwd.h>
#ضمن <stdio.h>
#جے_تعریف_شدہ HAVE_STDLIB_H
#ضمن <stdlib.h>
#جے_انت
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

بنية ext2_qcow2_hdr *qcow2_read_سرخی(صحيح fd)
{
	فراغ *بفر = عدم;
	بنية ext2_qcow2_hdr *hdr = عدم;
	size_t ناپ;
	errcode_t ret;

	ret = ext2fs_get_mem(حجم(بنية ext2_qcow2_hdr), &بفر);
	إذا (ret)
		أرجع عدم;
	memset(بفر, 0, حجم(بنية ext2_qcow2_hdr));

	إذا (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&بفر);
		أرجع عدم;
	}

	ناپ = اقرأ(fd, بفر, حجم(بنية ext2_qcow2_hdr));
	إذا (ناپ != حجم(بنية ext2_qcow2_hdr)) {
		ext2fs_free_mem(&بفر);
		أرجع عدم;
	}

	hdr = (بنية ext2_qcow2_hdr *)(بفر);

	إذا ((ext2fs_be32_منزل_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_منزل_cpu(hdr->نسخہ) != 2)) {
		ext2fs_free_mem(&hdr);
		أرجع عدم;
	}

	أرجع hdr;
}

ساكن صحيح qcow2_read_l1_جدول(بنية ext2_qcow2_image *img)
{
	صحيح fd = img->fd;
	size_t ناپ, l1_ناپ = img->l1_ناپ * حجم(blk64_t);
	blk64_t *جدول;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_ناپ, &جدول);
	إذا (ret)
		أرجع ret;

	إذا (ext2fs_llseek(fd, img->l1_وتھ, SEEK_SET) < 0) {
		ext2fs_free_mem(&جدول);
		أرجع errno;
	}

	ناپ = اقرأ(fd, جدول, l1_ناپ);
	إذا (ناپ != l1_ناپ) {
		ext2fs_free_mem(&جدول);
		أرجع errno;
	}

	img->l1_جدول = جدول;

	أرجع 0;
}

ساكن صحيح qcow2_read_l2_جدول(بنية ext2_qcow2_image *img,
			       __u64 وتھ, blk64_t **l2_جدول)
{
	صحيح fd = img->fd;
	size_t ناپ;

	أكد(*l2_جدول);

	إذا (ext2fs_llseek(fd, وتھ, SEEK_SET) < 0)
		أرجع errno;

	ناپ = اقرأ(fd, *l2_جدول, img->cluster_ناپ);
	إذا (ناپ != img->cluster_ناپ)
		أرجع errno;

	أرجع 0;
}

ساكن صحيح qcow2_copy_انکڑے(صحيح fdin, صحيح fdout, __u64 off_وچ,
			   __u64 off_باہرلا, فراغ *buf, size_t گنتی)
{
	ssize_t c1, c2, c;
	فراغ *ptr;
	صحيح retries = 10;

	أكد(buf);

	إذا (ext2fs_llseek(fdout, off_باہرلا, SEEK_SET) < 0)
		أرجع errno;

	إذا (ext2fs_llseek(fdin, off_وچ, SEEK_SET) < 0)
		أرجع errno;

	طالما (گنتی > 0) {
		errno = 0;
		c1 = اقرأ(fdin, buf, گنتی);
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
		گنتی -= c1;
	}
	أرجع 0;
}


صحيح qcow2_write_raw_image(صحيح qcow2_fd, صحيح raw_fd,
			      بنية ext2_qcow2_hdr *hdr)
{
	بنية ext2_qcow2_image img;
	errcode_t ret = 0;
	بنانشانی صحيح l1_اشاریہ, l2_اشاریہ;
	__u64 وتھ;
	blk64_t *l1_جدول, *l2_جدول = عدم;
	فراغ *copy_buf = عدم;
	size_t ناپ;
	بنانشانی صحيح max_l1_ناپ;

	إذا (hdr->crypt_طریقہ)
		أرجع -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = عدم;
	img.l1_جدول = عدم;
	img.cluster_بٹ = ext2fs_be32_منزل_cpu(hdr->cluster_بٹ);
	إذا (img.cluster_بٹ < 9 || img.cluster_بٹ > 31)
		أرجع -QCOW_CORRUPTED;
	img.cluster_ناپ = 1 << img.cluster_بٹ;
	img.l1_ناپ = ext2fs_be32_منزل_cpu(hdr->l1_ناپ);
	img.l1_وتھ = ext2fs_be64_منزل_cpu(hdr->l1_جدول_وتھ);
	img.l2_ناپ = 1 << (img.cluster_بٹ - 3);
	img.image_ناپ = ext2fs_be64_منزل_cpu(hdr->ناپ);

	إذا (img.l1_وتھ & (img.cluster_ناپ - 1))
		أرجع -QCOW_CORRUPTED;

	max_l1_ناپ = (img.image_ناپ >> ((2 * img.cluster_بٹ) - 3)) +
		img.cluster_ناپ;
	إذا (img.l1_ناپ > max_l1_ناپ)
		أرجع -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_ناپ, &l2_جدول);
	إذا (ret)
		اذهب باہرلا;

	ret = ext2fs_get_memzero(1 << img.cluster_بٹ, &copy_buf);
	إذا (ret)
		اذهب باہرلا;

	إذا (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		اذهب باہرلا;
	}

	ret = qcow2_read_l1_جدول(&img);
	إذا (ret)
		اذهب باہرلا;

	l1_جدول = img.l1_جدول;
	/* Walk through l1 table */
	لكل (l1_اشاریہ = 0; l1_اشاریہ < img.l1_ناپ; l1_اشاریہ++) {
		__u64 off_باہرلا;

		وتھ = ext2fs_be64_منزل_cpu(l1_جدول[l1_اشاریہ]) &
			 ~QCOW_OFLAG_COPIED;

		إذا ((وتھ > img.image_ناپ) ||
		    (وتھ <= 0))
			استمر;

		إذا (وتھ & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			اذهب باہرلا;
		}

		ret = qcow2_read_l2_جدول(&img, وتھ, &l2_جدول);
		إذا (ret)
			اكسر;

		/* Walk through l2 table and copy data blocks into raw image */
		لكل (l2_اشاریہ = 0; l2_اشاریہ < img.l2_ناپ; l2_اشاریہ++) {
			وتھ = ext2fs_be64_منزل_cpu(l2_جدول[l2_اشاریہ]) &
				 ~QCOW_OFLAG_COPIED;

			إذا (وتھ == 0)
				استمر;

			off_باہرلا = ((__u64)l1_اشاریہ * img.l2_ناپ) +
				  l2_اشاریہ;
			off_باہرلا <<= img.cluster_بٹ;
			ret = qcow2_copy_انکڑے(qcow2_fd, raw_fd, وتھ,
					off_باہرلا, copy_buf, img.cluster_ناپ);
			إذا (ret)
				اذهب باہرلا;
		}
	}

	/* Resize the output image to the filesystem size */
	إذا (ext2fs_llseek(raw_fd, img.image_ناپ - 1, SEEK_SET) < 0) {
		ret = errno;
		اذهب باہرلا;
	}

	((حرف *)copy_buf)[0] = 0;
	ناپ = اكتب(raw_fd, copy_buf, 1);
	إذا (ناپ != 1) {
		ret = errno;
		اذهب باہرلا;
	}

باہرلا:
	إذا (copy_buf)
		ext2fs_free_mem(&copy_buf);
	إذا (img.l1_جدول)
		ext2fs_free_mem(&img.l1_جدول);
	إذا (l2_جدول)
		ext2fs_free_mem(&l2_جدول);
	أرجع ret;
}
