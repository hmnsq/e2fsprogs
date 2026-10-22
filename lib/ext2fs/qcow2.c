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

#اگر_غیر_تعریف_شدہ LARGEFILE_ماخذ
#تعریف_کرو LARGEFILE_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ LARGEFILE64_ماخذ
#تعریف_کرو LARGEFILE64_ماخذ
#اگر_ختم

#شامل "config.h"
#شامل <fcntl.h>
#شامل <grp.h>
#شامل <pwd.h>
#شامل <stdio.h>
#اگر_تعریف_شدہ HAVE_STDLIB_H
#شامل <stdlib.h>
#اگر_ختم
#شامل <string.h>
#شامل <time.h>
#شامل <unistd.h>
#شامل <fcntl.h>
#شامل <errno.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <assert.h>

#شامل "ext2fs/ext2fs.h"
#شامل "qcow2.h"

/* Functions for converting qcow2 image into raw image */

ساخت ext2_qcow2_hdr *qcow2_read_سرخی(صحیح fd)
{
	خالی *بفر = تہی;
	ساخت ext2_qcow2_hdr *hdr = تہی;
	size_t سائز;
	errcode_t ret;

	ret = ext2fs_get_mem(حجم(ساخت ext2_qcow2_hdr), &بفر);
	اگر (ret)
		واپس تہی;
	memset(بفر, 0, حجم(ساخت ext2_qcow2_hdr));

	اگر (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&بفر);
		واپس تہی;
	}

	سائز = اقرأ(fd, بفر, حجم(ساخت ext2_qcow2_hdr));
	اگر (سائز != حجم(ساخت ext2_qcow2_hdr)) {
		ext2fs_free_mem(&بفر);
		واپس تہی;
	}

	hdr = (ساخت ext2_qcow2_hdr *)(بفر);

	اگر ((ext2fs_be32_مقصد_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_مقصد_cpu(hdr->نسخہ) != 2)) {
		ext2fs_free_mem(&hdr);
		واپس تہی;
	}

	واپس hdr;
}

ساکن صحیح qcow2_read_l1_جدول(ساخت ext2_qcow2_image *img)
{
	صحیح fd = img->fd;
	size_t سائز, l1_سائز = img->l1_سائز * حجم(blk64_t);
	blk64_t *جدول;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_سائز, &جدول);
	اگر (ret)
		واپس ret;

	اگر (ext2fs_llseek(fd, img->l1_انحراف, SEEK_SET) < 0) {
		ext2fs_free_mem(&جدول);
		واپس errno;
	}

	سائز = اقرأ(fd, جدول, l1_سائز);
	اگر (سائز != l1_سائز) {
		ext2fs_free_mem(&جدول);
		واپس errno;
	}

	img->l1_جدول = جدول;

	واپس 0;
}

ساکن صحیح qcow2_read_l2_جدول(ساخت ext2_qcow2_image *img,
			       __u64 انحراف, blk64_t **l2_جدول)
{
	صحیح fd = img->fd;
	size_t سائز;

	أكد(*l2_جدول);

	اگر (ext2fs_llseek(fd, انحراف, SEEK_SET) < 0)
		واپس errno;

	سائز = اقرأ(fd, *l2_جدول, img->cluster_سائز);
	اگر (سائز != img->cluster_سائز)
		واپس errno;

	واپس 0;
}

ساکن صحیح qcow2_copy_ڈیٹا(صحیح fdin, صحیح fdout, __u64 off_اندر,
			   __u64 off_برآمد, خالی *buf, size_t گنتی)
{
	ssize_t c1, c2, c;
	خالی *ptr;
	صحیح retries = 10;

	أكد(buf);

	اگر (ext2fs_llseek(fdout, off_برآمد, SEEK_SET) < 0)
		واپس errno;

	اگر (ext2fs_llseek(fdin, off_اندر, SEEK_SET) < 0)
		واپس errno;

	جب (گنتی > 0) {
		errno = 0;
		c1 = اقرأ(fdin, buf, گنتی);
		اگر (c1 < 0 || ((c1 == 0) && errno))
			واپس errno;
		اگر (c1 == 0)
			توڑو; 	/* EOF */

		برائے (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = اكتب(fdout, ptr, c1);
			اگر (c2 < 0 || ((c2 == 0) && errno))
				واپس errno;
			اگر (c2 == 0 && --retries <= 0)
				توڑو; /* This should never happen...  */
		}
		گنتی -= c1;
	}
	واپس 0;
}


صحیح qcow2_write_raw_image(صحیح qcow2_fd, صحیح raw_fd,
			      ساخت ext2_qcow2_hdr *hdr)
{
	ساخت ext2_qcow2_image img;
	errcode_t ret = 0;
	غیرعلامتی صحیح l1_اشاریہ, l2_اشاریہ;
	__u64 انحراف;
	blk64_t *l1_جدول, *l2_جدول = تہی;
	خالی *copy_buf = تہی;
	size_t سائز;
	غیرعلامتی صحیح max_l1_سائز;

	اگر (hdr->crypt_طریقہ)
		واپس -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = تہی;
	img.l1_جدول = تہی;
	img.cluster_بٹ = ext2fs_be32_مقصد_cpu(hdr->cluster_بٹ);
	اگر (img.cluster_بٹ < 9 || img.cluster_بٹ > 31)
		واپس -QCOW_CORRUPTED;
	img.cluster_سائز = 1 << img.cluster_بٹ;
	img.l1_سائز = ext2fs_be32_مقصد_cpu(hdr->l1_سائز);
	img.l1_انحراف = ext2fs_be64_مقصد_cpu(hdr->l1_جدول_انحراف);
	img.l2_سائز = 1 << (img.cluster_بٹ - 3);
	img.image_سائز = ext2fs_be64_مقصد_cpu(hdr->سائز);

	اگر (img.l1_انحراف & (img.cluster_سائز - 1))
		واپس -QCOW_CORRUPTED;

	max_l1_سائز = (img.image_سائز >> ((2 * img.cluster_بٹ) - 3)) +
		img.cluster_سائز;
	اگر (img.l1_سائز > max_l1_سائز)
		واپس -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_سائز, &l2_جدول);
	اگر (ret)
		جاؤ برآمد;

	ret = ext2fs_get_memzero(1 << img.cluster_بٹ, &copy_buf);
	اگر (ret)
		جاؤ برآمد;

	اگر (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		جاؤ برآمد;
	}

	ret = qcow2_read_l1_جدول(&img);
	اگر (ret)
		جاؤ برآمد;

	l1_جدول = img.l1_جدول;
	/* Walk through l1 table */
	برائے (l1_اشاریہ = 0; l1_اشاریہ < img.l1_سائز; l1_اشاریہ++) {
		__u64 off_برآمد;

		انحراف = ext2fs_be64_مقصد_cpu(l1_جدول[l1_اشاریہ]) &
			 ~QCOW_OFLAG_COPIED;

		اگر ((انحراف > img.image_سائز) ||
		    (انحراف <= 0))
			جاری;

		اگر (انحراف & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			جاؤ برآمد;
		}

		ret = qcow2_read_l2_جدول(&img, انحراف, &l2_جدول);
		اگر (ret)
			توڑو;

		/* Walk through l2 table and copy data blocks into raw image */
		برائے (l2_اشاریہ = 0; l2_اشاریہ < img.l2_سائز; l2_اشاریہ++) {
			انحراف = ext2fs_be64_مقصد_cpu(l2_جدول[l2_اشاریہ]) &
				 ~QCOW_OFLAG_COPIED;

			اگر (انحراف == 0)
				جاری;

			off_برآمد = ((__u64)l1_اشاریہ * img.l2_سائز) +
				  l2_اشاریہ;
			off_برآمد <<= img.cluster_بٹ;
			ret = qcow2_copy_ڈیٹا(qcow2_fd, raw_fd, انحراف,
					off_برآمد, copy_buf, img.cluster_سائز);
			اگر (ret)
				جاؤ برآمد;
		}
	}

	/* Resize the output image to the filesystem size */
	اگر (ext2fs_llseek(raw_fd, img.image_سائز - 1, SEEK_SET) < 0) {
		ret = errno;
		جاؤ برآمد;
	}

	((حرف *)copy_buf)[0] = 0;
	سائز = اكتب(raw_fd, copy_buf, 1);
	اگر (سائز != 1) {
		ret = errno;
		جاؤ برآمد;
	}

برآمد:
	اگر (copy_buf)
		ext2fs_free_mem(&copy_buf);
	اگر (img.l1_جدول)
		ext2fs_free_mem(&img.l1_جدول);
	اگر (l2_جدول)
		ext2fs_free_mem(&l2_جدول);
	واپس ret;
}
