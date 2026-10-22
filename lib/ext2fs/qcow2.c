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

#నిర్వచితం_కాకపోతే LARGEFILE_ఆధారం
#నిర్వచించు LARGEFILE_ఆధారం
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే LARGEFILE64_ఆధారం
#నిర్వచించు LARGEFILE64_ఆధారం
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <grp.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <stdio.h>
#నిర్వచితమైతే HAVE_STDLIB_H
#అన్తర్భావ <stdlib.h>
#అయితే_ముగింపు
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <assert.h>

#అన్తర్భావ "ext2fs/ext2fs.h"
#అన్తర్భావ "qcow2.h"

/* Functions for converting qcow2 image into raw image */

నిర్మాణం ext2_qcow2_hdr *qcow2_read_శీర్షిక(పూర్ణాంకం fd)
{
	శూన్య *బఫరు = ఖాళీ;
	నిర్మాణం ext2_qcow2_hdr *hdr = ఖాళీ;
	size_t కొలత;
	errcode_t ret;

	ret = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_qcow2_hdr), &బఫరు);
	ఒకవేళ (ret)
		తిరిగి ఖాళీ;
	memset(బఫరు, 0, పరిమాణం(నిర్మాణం ext2_qcow2_hdr));

	ఒకవేళ (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&బఫరు);
		తిరిగి ఖాళీ;
	}

	కొలత = పఠ(fd, బఫరు, పరిమాణం(నిర్మాణం ext2_qcow2_hdr));
	ఒకవేళ (కొలత != పరిమాణం(నిర్మాణం ext2_qcow2_hdr)) {
		ext2fs_free_mem(&బఫరు);
		తిరిగి ఖాళీ;
	}

	hdr = (నిర్మాణం ext2_qcow2_hdr *)(బఫరు);

	ఒకవేళ ((ext2fs_be32_గమ్యం_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_గమ్యం_cpu(hdr->సంస్కరణ) != 2)) {
		ext2fs_free_mem(&hdr);
		తిరిగి ఖాళీ;
	}

	తిరిగి hdr;
}

స్థిర పూర్ణాంకం qcow2_read_l1_పట్టిక(నిర్మాణం ext2_qcow2_image *img)
{
	పూర్ణాంకం fd = img->fd;
	size_t కొలత, l1_కొలత = img->l1_కొలత * పరిమాణం(blk64_t);
	blk64_t *పట్టిక;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_కొలత, &పట్టిక);
	ఒకవేళ (ret)
		తిరిగి ret;

	ఒకవేళ (ext2fs_llseek(fd, img->l1_స్థానభ్రంశం, SEEK_SET) < 0) {
		ext2fs_free_mem(&పట్టిక);
		తిరిగి errno;
	}

	కొలత = పఠ(fd, పట్టిక, l1_కొలత);
	ఒకవేళ (కొలత != l1_కొలత) {
		ext2fs_free_mem(&పట్టిక);
		తిరిగి errno;
	}

	img->l1_పట్టిక = పట్టిక;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం qcow2_read_l2_పట్టిక(నిర్మాణం ext2_qcow2_image *img,
			       __u64 స్థానభ్రంశం, blk64_t **l2_పట్టిక)
{
	పూర్ణాంకం fd = img->fd;
	size_t కొలత;

	ప్రతిజ్ఞా(*l2_పట్టిక);

	ఒకవేళ (ext2fs_llseek(fd, స్థానభ్రంశం, SEEK_SET) < 0)
		తిరిగి errno;

	కొలత = పఠ(fd, *l2_పట్టిక, img->cluster_కొలత);
	ఒకవేళ (కొలత != img->cluster_కొలత)
		తిరిగి errno;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం qcow2_copy_దత్తాంశం(పూర్ణాంకం fdin, పూర్ణాంకం fdout, __u64 off_లోపల,
			   __u64 off_బహిర్గతం, శూన్య *buf, size_t లెక్క)
{
	ssize_t c1, c2, c;
	శూన్య *ptr;
	పూర్ణాంకం retries = 10;

	ప్రతిజ్ఞా(buf);

	ఒకవేళ (ext2fs_llseek(fdout, off_బహిర్గతం, SEEK_SET) < 0)
		తిరిగి errno;

	ఒకవేళ (ext2fs_llseek(fdin, off_లోపల, SEEK_SET) < 0)
		తిరిగి errno;

	అయితే (లెక్క > 0) {
		errno = 0;
		c1 = పఠ(fdin, buf, లెక్క);
		ఒకవేళ (c1 < 0 || ((c1 == 0) && errno))
			తిరిగి errno;
		ఒకవేళ (c1 == 0)
			ఆపు; 	/* EOF */

		కొరకు (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = లిఖ(fdout, ptr, c1);
			ఒకవేళ (c2 < 0 || ((c2 == 0) && errno))
				తిరిగి errno;
			ఒకవేళ (c2 == 0 && --retries <= 0)
				ఆపు; /* This should never happen...  */
		}
		లెక్క -= c1;
	}
	తిరిగి 0;
}


పూర్ణాంకం qcow2_write_raw_image(పూర్ణాంకం qcow2_fd, పూర్ణాంకం raw_fd,
			      నిర్మాణం ext2_qcow2_hdr *hdr)
{
	నిర్మాణం ext2_qcow2_image img;
	errcode_t ret = 0;
	గుర్తులేని పూర్ణాంకం l1_సూచీ, l2_సూచీ;
	__u64 స్థానభ్రంశం;
	blk64_t *l1_పట్టిక, *l2_పట్టిక = ఖాళీ;
	శూన్య *copy_buf = ఖాళీ;
	size_t కొలత;
	గుర్తులేని పూర్ణాంకం max_l1_కొలత;

	ఒకవేళ (hdr->crypt_పద్ధతి)
		తిరిగి -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = ఖాళీ;
	img.l1_పట్టిక = ఖాళీ;
	img.cluster_బిట్లు = ext2fs_be32_గమ్యం_cpu(hdr->cluster_బిట్లు);
	ఒకవేళ (img.cluster_బిట్లు < 9 || img.cluster_బిట్లు > 31)
		తిరిగి -QCOW_CORRUPTED;
	img.cluster_కొలత = 1 << img.cluster_బిట్లు;
	img.l1_కొలత = ext2fs_be32_గమ్యం_cpu(hdr->l1_కొలత);
	img.l1_స్థానభ్రంశం = ext2fs_be64_గమ్యం_cpu(hdr->l1_పట్టిక_స్థానభ్రంశం);
	img.l2_కొలత = 1 << (img.cluster_బిట్లు - 3);
	img.image_కొలత = ext2fs_be64_గమ్యం_cpu(hdr->కొలత);

	ఒకవేళ (img.l1_స్థానభ్రంశం & (img.cluster_కొలత - 1))
		తిరిగి -QCOW_CORRUPTED;

	max_l1_కొలత = (img.image_కొలత >> ((2 * img.cluster_బిట్లు) - 3)) +
		img.cluster_కొలత;
	ఒకవేళ (img.l1_కొలత > max_l1_కొలత)
		తిరిగి -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_కొలత, &l2_పట్టిక);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

	ret = ext2fs_get_memzero(1 << img.cluster_బిట్లు, &copy_buf);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		వెళ్ళు బహిర్గతం;
	}

	ret = qcow2_read_l1_పట్టిక(&img);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

	l1_పట్టిక = img.l1_పట్టిక;
	/* Walk through l1 table */
	కొరకు (l1_సూచీ = 0; l1_సూచీ < img.l1_కొలత; l1_సూచీ++) {
		__u64 off_బహిర్గతం;

		స్థానభ్రంశం = ext2fs_be64_గమ్యం_cpu(l1_పట్టిక[l1_సూచీ]) &
			 ~QCOW_OFLAG_COPIED;

		ఒకవేళ ((స్థానభ్రంశం > img.image_కొలత) ||
		    (స్థానభ్రంశం <= 0))
			కొనసాగు;

		ఒకవేళ (స్థానభ్రంశం & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			వెళ్ళు బహిర్గతం;
		}

		ret = qcow2_read_l2_పట్టిక(&img, స్థానభ్రంశం, &l2_పట్టిక);
		ఒకవేళ (ret)
			ఆపు;

		/* Walk through l2 table and copy data blocks into raw image */
		కొరకు (l2_సూచీ = 0; l2_సూచీ < img.l2_కొలత; l2_సూచీ++) {
			స్థానభ్రంశం = ext2fs_be64_గమ్యం_cpu(l2_పట్టిక[l2_సూచీ]) &
				 ~QCOW_OFLAG_COPIED;

			ఒకవేళ (స్థానభ్రంశం == 0)
				కొనసాగు;

			off_బహిర్గతం = ((__u64)l1_సూచీ * img.l2_కొలత) +
				  l2_సూచీ;
			off_బహిర్గతం <<= img.cluster_బిట్లు;
			ret = qcow2_copy_దత్తాంశం(qcow2_fd, raw_fd, స్థానభ్రంశం,
					off_బహిర్గతం, copy_buf, img.cluster_కొలత);
			ఒకవేళ (ret)
				వెళ్ళు బహిర్గతం;
		}
	}

	/* Resize the output image to the filesystem size */
	ఒకవేళ (ext2fs_llseek(raw_fd, img.image_కొలత - 1, SEEK_SET) < 0) {
		ret = errno;
		వెళ్ళు బహిర్గతం;
	}

	((అక్షరం *)copy_buf)[0] = 0;
	కొలత = లిఖ(raw_fd, copy_buf, 1);
	ఒకవేళ (కొలత != 1) {
		ret = errno;
		వెళ్ళు బహిర్గతం;
	}

బహిర్గతం:
	ఒకవేళ (copy_buf)
		ext2fs_free_mem(&copy_buf);
	ఒకవేళ (img.l1_పట్టిక)
		ext2fs_free_mem(&img.l1_పట్టిక);
	ఒకవేళ (l2_పట్టిక)
		ext2fs_free_mem(&l2_పట్టిక);
	తిరిగి ret;
}
