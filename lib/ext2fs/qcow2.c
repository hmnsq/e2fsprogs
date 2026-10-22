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

#اگر_تعریف_نشده LARGEFILE_منبع
#تعریف_کن LARGEFILE_منبع
#پایان_اگر
#اگر_تعریف_نشده LARGEFILE64_منبع
#تعریف_کن LARGEFILE64_منبع
#پایان_اگر

#ضمن "config.h"
#ضمن <fcntl.h>
#ضمن <grp.h>
#ضمن <pwd.h>
#ضمن <stdio.h>
#اگر_تعریف_شده HAVE_STDLIB_H
#ضمن <stdlib.h>
#پایان_اگر
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

بنية ext2_qcow2_hdr *qcow2_read_سرآیند(صحيح fd)
{
	فراغ *میانگیر = عدم;
	بنية ext2_qcow2_hdr *hdr = عدم;
	size_t اندازه;
	errcode_t ret;

	ret = ext2fs_get_mem(حجم(بنية ext2_qcow2_hdr), &میانگیر);
	إذا (ret)
		أرجع عدم;
	memset(میانگیر, 0, حجم(بنية ext2_qcow2_hdr));

	إذا (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&میانگیر);
		أرجع عدم;
	}

	اندازه = اقرأ(fd, میانگیر, حجم(بنية ext2_qcow2_hdr));
	إذا (اندازه != حجم(بنية ext2_qcow2_hdr)) {
		ext2fs_free_mem(&میانگیر);
		أرجع عدم;
	}

	hdr = (بنية ext2_qcow2_hdr *)(میانگیر);

	إذا ((ext2fs_be32_مقصد_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_مقصد_cpu(hdr->نسخه) != 2)) {
		ext2fs_free_mem(&hdr);
		أرجع عدم;
	}

	أرجع hdr;
}

ساكن صحيح qcow2_read_l1_جدول(بنية ext2_qcow2_image *img)
{
	صحيح fd = img->fd;
	size_t اندازه, l1_اندازه = img->l1_اندازه * حجم(blk64_t);
	blk64_t *جدول;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_اندازه, &جدول);
	إذا (ret)
		أرجع ret;

	إذا (ext2fs_llseek(fd, img->l1_جابه_جایی, SEEK_SET) < 0) {
		ext2fs_free_mem(&جدول);
		أرجع errno;
	}

	اندازه = اقرأ(fd, جدول, l1_اندازه);
	إذا (اندازه != l1_اندازه) {
		ext2fs_free_mem(&جدول);
		أرجع errno;
	}

	img->l1_جدول = جدول;

	أرجع 0;
}

ساكن صحيح qcow2_read_l2_جدول(بنية ext2_qcow2_image *img,
			       __u64 جابه_جایی, blk64_t **l2_جدول)
{
	صحيح fd = img->fd;
	size_t اندازه;

	أكد(*l2_جدول);

	إذا (ext2fs_llseek(fd, جابه_جایی, SEEK_SET) < 0)
		أرجع errno;

	اندازه = اقرأ(fd, *l2_جدول, img->cluster_اندازه);
	إذا (اندازه != img->cluster_اندازه)
		أرجع errno;

	أرجع 0;
}

ساكن صحيح qcow2_copy_داده(صحيح fdin, صحيح fdout, __u64 off_درون,
			   __u64 off_بیرون, فراغ *buf, size_t شمار)
{
	ssize_t c1, c2, c;
	فراغ *ptr;
	صحيح retries = 10;

	أكد(buf);

	إذا (ext2fs_llseek(fdout, off_بیرون, SEEK_SET) < 0)
		أرجع errno;

	إذا (ext2fs_llseek(fdin, off_درون, SEEK_SET) < 0)
		أرجع errno;

	طالما (شمار > 0) {
		errno = 0;
		c1 = اقرأ(fdin, buf, شمار);
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
		شمار -= c1;
	}
	أرجع 0;
}


صحيح qcow2_write_raw_image(صحيح qcow2_fd, صحيح raw_fd,
			      بنية ext2_qcow2_hdr *hdr)
{
	بنية ext2_qcow2_image img;
	errcode_t ret = 0;
	بیعلامت صحيح l1_شاخص, l2_شاخص;
	__u64 جابه_جایی;
	blk64_t *l1_جدول, *l2_جدول = عدم;
	فراغ *copy_buf = عدم;
	size_t اندازه;
	بیعلامت صحيح max_l1_اندازه;

	إذا (hdr->crypt_روش)
		أرجع -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = عدم;
	img.l1_جدول = عدم;
	img.cluster_بیتها = ext2fs_be32_مقصد_cpu(hdr->cluster_بیتها);
	إذا (img.cluster_بیتها < 9 || img.cluster_بیتها > 31)
		أرجع -QCOW_CORRUPTED;
	img.cluster_اندازه = 1 << img.cluster_بیتها;
	img.l1_اندازه = ext2fs_be32_مقصد_cpu(hdr->l1_اندازه);
	img.l1_جابه_جایی = ext2fs_be64_مقصد_cpu(hdr->l1_جدول_جابه_جایی);
	img.l2_اندازه = 1 << (img.cluster_بیتها - 3);
	img.image_اندازه = ext2fs_be64_مقصد_cpu(hdr->اندازه);

	إذا (img.l1_جابه_جایی & (img.cluster_اندازه - 1))
		أرجع -QCOW_CORRUPTED;

	max_l1_اندازه = (img.image_اندازه >> ((2 * img.cluster_بیتها) - 3)) +
		img.cluster_اندازه;
	إذا (img.l1_اندازه > max_l1_اندازه)
		أرجع -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_اندازه, &l2_جدول);
	إذا (ret)
		اذهب بیرون;

	ret = ext2fs_get_memzero(1 << img.cluster_بیتها, &copy_buf);
	إذا (ret)
		اذهب بیرون;

	إذا (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		اذهب بیرون;
	}

	ret = qcow2_read_l1_جدول(&img);
	إذا (ret)
		اذهب بیرون;

	l1_جدول = img.l1_جدول;
	/* Walk through l1 table */
	لكل (l1_شاخص = 0; l1_شاخص < img.l1_اندازه; l1_شاخص++) {
		__u64 off_بیرون;

		جابه_جایی = ext2fs_be64_مقصد_cpu(l1_جدول[l1_شاخص]) &
			 ~QCOW_OFLAG_COPIED;

		إذا ((جابه_جایی > img.image_اندازه) ||
		    (جابه_جایی <= 0))
			استمر;

		إذا (جابه_جایی & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			اذهب بیرون;
		}

		ret = qcow2_read_l2_جدول(&img, جابه_جایی, &l2_جدول);
		إذا (ret)
			اكسر;

		/* Walk through l2 table and copy data blocks into raw image */
		لكل (l2_شاخص = 0; l2_شاخص < img.l2_اندازه; l2_شاخص++) {
			جابه_جایی = ext2fs_be64_مقصد_cpu(l2_جدول[l2_شاخص]) &
				 ~QCOW_OFLAG_COPIED;

			إذا (جابه_جایی == 0)
				استمر;

			off_بیرون = ((__u64)l1_شاخص * img.l2_اندازه) +
				  l2_شاخص;
			off_بیرون <<= img.cluster_بیتها;
			ret = qcow2_copy_داده(qcow2_fd, raw_fd, جابه_جایی,
					off_بیرون, copy_buf, img.cluster_اندازه);
			إذا (ret)
				اذهب بیرون;
		}
	}

	/* Resize the output image to the filesystem size */
	إذا (ext2fs_llseek(raw_fd, img.image_اندازه - 1, SEEK_SET) < 0) {
		ret = errno;
		اذهب بیرون;
	}

	((حرف *)copy_buf)[0] = 0;
	اندازه = اكتب(raw_fd, copy_buf, 1);
	إذا (اندازه != 1) {
		ret = errno;
		اذهب بیرون;
	}

بیرون:
	إذا (copy_buf)
		ext2fs_free_mem(&copy_buf);
	إذا (img.l1_جدول)
		ext2fs_free_mem(&img.l1_جدول);
	إذا (l2_جدول)
		ext2fs_free_mem(&l2_جدول);
	أرجع ret;
}
