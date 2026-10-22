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

#यदि_अपरिभाषितम् _LARGEFILE_SOURCE
#परिभाषय _LARGEFILE_SOURCE
#यदि_अन्तः
#यदि_अपरिभाषितम् _LARGEFILE64_SOURCE
#परिभाषय _LARGEFILE64_SOURCE
#यदि_अन्तः

#अन्तर्भाव "config.h"
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <grp.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <stdio.h>
#यदि_परिभाषितम् HAVE_STDLIB_H
#अन्तर्भाव <stdlib.h>
#यदि_अन्तः
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

संरचना ext2_qcow2_hdr *qcow2_read_शीर्षकम्(पूर्णाङ्क fd)
{
	शून्य *बफर = रिक्त;
	संरचना ext2_qcow2_hdr *hdr = रिक्त;
	size_t आकार;
	errcode_t ret;

	ret = ext2fs_get_mem(परिमाण(संरचना ext2_qcow2_hdr), &बफर);
	यदि (ret)
		प्रत्यागम रिक्त;
	स्मृतिं_पूरय(बफर, 0, परिमाण(संरचना ext2_qcow2_hdr));

	यदि (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&बफर);
		प्रत्यागम रिक्त;
	}

	आकार = पठ(fd, बफर, परिमाण(संरचना ext2_qcow2_hdr));
	यदि (आकार != परिमाण(संरचना ext2_qcow2_hdr)) {
		ext2fs_free_mem(&बफर);
		प्रत्यागम रिक्त;
	}

	hdr = (संरचना ext2_qcow2_hdr *)(बफर);

	यदि ((ext2fs_be32_गन्तव्यम्_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_गन्तव्यम्_cpu(hdr->संस्करणम्) != 2)) {
		ext2fs_free_mem(&hdr);
		प्रत्यागम रिक्त;
	}

	प्रत्यागम hdr;
}

स्थिर पूर्णाङ्क qcow2_read_l1_सारणी(संरचना ext2_qcow2_image *img)
{
	पूर्णाङ्क fd = img->fd;
	size_t आकार, l1_आकार = img->l1_आकार * परिमाण(blk64_t);
	blk64_t *सारणी;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_आकार, &सारणी);
	यदि (ret)
		प्रत्यागम ret;

	यदि (ext2fs_llseek(fd, img->l1_विस्थापनम्, SEEK_SET) < 0) {
		ext2fs_free_mem(&सारणी);
		प्रत्यागम errno;
	}

	आकार = पठ(fd, सारणी, l1_आकार);
	यदि (आकार != l1_आकार) {
		ext2fs_free_mem(&सारणी);
		प्रत्यागम errno;
	}

	img->l1_सारणी = सारणी;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क qcow2_read_l2_सारणी(संरचना ext2_qcow2_image *img,
			       __u64 विस्थापनम्, blk64_t **l2_सारणी)
{
	पूर्णाङ्क fd = img->fd;
	size_t आकार;

	प्रतिज्ञा(*l2_सारणी);

	यदि (ext2fs_llseek(fd, विस्थापनम्, SEEK_SET) < 0)
		प्रत्यागम errno;

	आकार = पठ(fd, *l2_सारणी, img->cluster_आकार);
	यदि (आकार != img->cluster_आकार)
		प्रत्यागम errno;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क qcow2_copy_दत्त(पूर्णाङ्क fdin, पूर्णाङ्क fdout, __u64 off_अन्तर्,
			   __u64 off_बहिर्गमनम्, शून्य *buf, size_t गणनम्)
{
	ssize_t c1, c2, c;
	शून्य *ptr;
	पूर्णाङ्क retries = 10;

	प्रतिज्ञा(buf);

	यदि (ext2fs_llseek(fdout, off_बहिर्गमनम्, SEEK_SET) < 0)
		प्रत्यागम errno;

	यदि (ext2fs_llseek(fdin, off_अन्तर्, SEEK_SET) < 0)
		प्रत्यागम errno;

	यावत् (गणनम् > 0) {
		errno = 0;
		c1 = पठ(fdin, buf, गणनम्);
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
		गणनम् -= c1;
	}
	प्रत्यागम 0;
}


पूर्णाङ्क qcow2_write_raw_image(पूर्णाङ्क qcow2_fd, पूर्णाङ्क raw_fd,
			      संरचना ext2_qcow2_hdr *hdr)
{
	संरचना ext2_qcow2_image img;
	errcode_t ret = 0;
	अचिह्नित पूर्णाङ्क l1_सूचकांक, l2_सूचकांक;
	__u64 विस्थापनम्;
	blk64_t *l1_सारणी, *l2_सारणी = रिक्त;
	शून्य *copy_buf = रिक्त;
	size_t आकार;
	अचिह्नित पूर्णाङ्क max_l1_आकार;

	यदि (hdr->crypt_विधानम्)
		प्रत्यागम -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = रिक्त;
	img.l1_सारणी = रिक्त;
	img.cluster_द्वयङ्काः = ext2fs_be32_गन्तव्यम्_cpu(hdr->cluster_द्वयङ्काः);
	यदि (img.cluster_द्वयङ्काः < 9 || img.cluster_द्वयङ्काः > 31)
		प्रत्यागम -QCOW_CORRUPTED;
	img.cluster_आकार = 1 << img.cluster_द्वयङ्काः;
	img.l1_आकार = ext2fs_be32_गन्तव्यम्_cpu(hdr->l1_आकार);
	img.l1_विस्थापनम् = ext2fs_be64_गन्तव्यम्_cpu(hdr->l1_सारणी_विस्थापनम्);
	img.l2_आकार = 1 << (img.cluster_द्वयङ्काः - 3);
	img.image_आकार = ext2fs_be64_गन्तव्यम्_cpu(hdr->आकार);

	यदि (img.l1_विस्थापनम् & (img.cluster_आकार - 1))
		प्रत्यागम -QCOW_CORRUPTED;

	max_l1_आकार = (img.image_आकार >> ((2 * img.cluster_द्वयङ्काः) - 3)) +
		img.cluster_आकार;
	यदि (img.l1_आकार > max_l1_आकार)
		प्रत्यागम -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_आकार, &l2_सारणी);
	यदि (ret)
		गच्छ बहिर्गमनम्;

	ret = ext2fs_get_memzero(1 << img.cluster_द्वयङ्काः, &copy_buf);
	यदि (ret)
		गच्छ बहिर्गमनम्;

	यदि (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		गच्छ बहिर्गमनम्;
	}

	ret = qcow2_read_l1_सारणी(&img);
	यदि (ret)
		गच्छ बहिर्गमनम्;

	l1_सारणी = img.l1_सारणी;
	/* Walk through l1 table */
	प्रति (l1_सूचकांक = 0; l1_सूचकांक < img.l1_आकार; l1_सूचकांक++) {
		__u64 off_बहिर्गमनम्;

		विस्थापनम् = ext2fs_be64_गन्तव्यम्_cpu(l1_सारणी[l1_सूचकांक]) &
			 ~QCOW_OFLAG_COPIED;

		यदि ((विस्थापनम् > img.image_आकार) ||
		    (विस्थापनम् <= 0))
			अनुवर्त;

		यदि (विस्थापनम् & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			गच्छ बहिर्गमनम्;
		}

		ret = qcow2_read_l2_सारणी(&img, विस्थापनम्, &l2_सारणी);
		यदि (ret)
			भङ्ग;

		/* Walk through l2 table and copy data blocks into raw image */
		प्रति (l2_सूचकांक = 0; l2_सूचकांक < img.l2_आकार; l2_सूचकांक++) {
			विस्थापनम् = ext2fs_be64_गन्तव्यम्_cpu(l2_सारणी[l2_सूचकांक]) &
				 ~QCOW_OFLAG_COPIED;

			यदि (विस्थापनम् == 0)
				अनुवर्त;

			off_बहिर्गमनम् = ((__u64)l1_सूचकांक * img.l2_आकार) +
				  l2_सूचकांक;
			off_बहिर्गमनम् <<= img.cluster_द्वयङ्काः;
			ret = qcow2_copy_दत्त(qcow2_fd, raw_fd, विस्थापनम्,
					off_बहिर्गमनम्, copy_buf, img.cluster_आकार);
			यदि (ret)
				गच्छ बहिर्गमनम्;
		}
	}

	/* Resize the output image to the filesystem size */
	यदि (ext2fs_llseek(raw_fd, img.image_आकार - 1, SEEK_SET) < 0) {
		ret = errno;
		गच्छ बहिर्गमनम्;
	}

	((वर्ण *)copy_buf)[0] = 0;
	आकार = लिख(raw_fd, copy_buf, 1);
	यदि (आकार != 1) {
		ret = errno;
		गच्छ बहिर्गमनम्;
	}

बहिर्गमनम्:
	यदि (copy_buf)
		ext2fs_free_mem(&copy_buf);
	यदि (img.l1_सारणी)
		ext2fs_free_mem(&img.l1_सारणी);
	यदि (l2_सारणी)
		ext2fs_free_mem(&l2_सारणी);
	प्रत्यागम ret;
}
