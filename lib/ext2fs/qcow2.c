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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE64_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE64_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_STDLIB_H
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "ext2fs/ext2fs.h"
#ಅನ್ತರ್ಭಾವ "qcow2.h"

/* Functions for converting qcow2 image into raw image */

ರಚನೆ ext2_qcow2_hdr *qcow2_read_ಶೀರ್ಷಿಕೆ(ಪೂರ್ಣಾಂಕ fd)
{
	ಶೂನ್ಯ *ಬಫರ್ = ಖಾಲಿ;
	ರಚನೆ ext2_qcow2_hdr *hdr = ಖಾಲಿ;
	size_t ಗಾತ್ರ_2;
	errcode_t ret;

	ret = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_qcow2_hdr), &ಬಫರ್);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	memset(ಬಫರ್, 0, ಗಾತ್ರ(ರಚನೆ ext2_qcow2_hdr));

	ಆದರೆ (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&ಬಫರ್);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಗಾತ್ರ_2 = ಪಠ(fd, ಬಫರ್, ಗಾತ್ರ(ರಚನೆ ext2_qcow2_hdr));
	ಆದರೆ (ಗಾತ್ರ_2 != ಗಾತ್ರ(ರಚನೆ ext2_qcow2_hdr)) {
		ext2fs_free_mem(&ಬಫರ್);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	hdr = (ರಚನೆ ext2_qcow2_hdr *)(ಬಫರ್);

	ಆದರೆ ((ext2fs_be32_ಗಮ್ಯ_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_ಗಮ್ಯ_cpu(hdr->ಆವೃತ್ತಿ) != 2)) {
		ext2fs_free_mem(&hdr);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ hdr;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ qcow2_read_l1_ಕೋಷ್ಟಕ(ರಚನೆ ext2_qcow2_image *img)
{
	ಪೂರ್ಣಾಂಕ fd = img->fd;
	size_t ಗಾತ್ರ_2, l1_ಗಾತ್ರ = img->l1_ಗಾತ್ರ * ಗಾತ್ರ(blk64_t);
	blk64_t *ಕೋಷ್ಟಕ;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_ಗಾತ್ರ, &ಕೋಷ್ಟಕ);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	ಆದರೆ (ext2fs_llseek(fd, img->l1_ಸ್ಥಾನಾಂತರ, SEEK_SET) < 0) {
		ext2fs_free_mem(&ಕೋಷ್ಟಕ);
		ಹಿಂತಿರುಗಿ errno;
	}

	ಗಾತ್ರ_2 = ಪಠ(fd, ಕೋಷ್ಟಕ, l1_ಗಾತ್ರ);
	ಆದರೆ (ಗಾತ್ರ_2 != l1_ಗಾತ್ರ) {
		ext2fs_free_mem(&ಕೋಷ್ಟಕ);
		ಹಿಂತಿರುಗಿ errno;
	}

	img->l1_ಕೋಷ್ಟಕ = ಕೋಷ್ಟಕ;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ qcow2_read_l2_ಕೋಷ್ಟಕ(ರಚನೆ ext2_qcow2_image *img,
			       __u64 ಸ್ಥಾನಾಂತರ, blk64_t **l2_ಕೋಷ್ಟಕ)
{
	ಪೂರ್ಣಾಂಕ fd = img->fd;
	size_t ಗಾತ್ರ_2;

	ಪ್ರತಿಜ್ಞಾ(*l2_ಕೋಷ್ಟಕ);

	ಆದರೆ (ext2fs_llseek(fd, ಸ್ಥಾನಾಂತರ, SEEK_SET) < 0)
		ಹಿಂತಿರುಗಿ errno;

	ಗಾತ್ರ_2 = ಪಠ(fd, *l2_ಕೋಷ್ಟಕ, img->cluster_ಗಾತ್ರ);
	ಆದರೆ (ಗಾತ್ರ_2 != img->cluster_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ errno;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ qcow2_copy_ದತ್ತಾಂಶ(ಪೂರ್ಣಾಂಕ fdin, ಪೂರ್ಣಾಂಕ fdout, __u64 off_ಒಳ,
			   __u64 off_ಹೊರಹೋಗು, ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2)
{
	ssize_t c1, c2, c;
	ಶೂನ್ಯ *ptr;
	ಪೂರ್ಣಾಂಕ retries = 10;

	ಪ್ರತಿಜ್ಞಾ(buf);

	ಆದರೆ (ext2fs_llseek(fdout, off_ಹೊರಹೋಗು, SEEK_SET) < 0)
		ಹಿಂತಿರುಗಿ errno;

	ಆದರೆ (ext2fs_llseek(fdin, off_ಒಳ, SEEK_SET) < 0)
		ಹಿಂತಿರುಗಿ errno;

	ಆಗ (ಎಣಿಕೆ_2 > 0) {
		errno = 0;
		c1 = ಪಠ(fdin, buf, ಎಣಿಕೆ_2);
		ಆದರೆ (c1 < 0 || ((c1 == 0) && errno))
			ಹಿಂತಿರುಗಿ errno;
		ಆದರೆ (c1 == 0)
			ನಿಲ್ಲಿಸು; 	/* EOF */

		ಗಾಗಿ (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = ಲಿಖ(fdout, ptr, c1);
			ಆದರೆ (c2 < 0 || ((c2 == 0) && errno))
				ಹಿಂತಿರುಗಿ errno;
			ಆದರೆ (c2 == 0 && --retries <= 0)
				ನಿಲ್ಲಿಸು; /* This should never happen...  */
		}
		ಎಣಿಕೆ_2 -= c1;
	}
	ಹಿಂತಿರುಗಿ 0;
}


ಪೂರ್ಣಾಂಕ qcow2_write_raw_image(ಪೂರ್ಣಾಂಕ qcow2_fd, ಪೂರ್ಣಾಂಕ raw_fd,
			      ರಚನೆ ext2_qcow2_hdr *hdr)
{
	ರಚನೆ ext2_qcow2_image img;
	errcode_t ret = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ l1_ಸೂಚ್ಯಂಕ, l2_ಸೂಚ್ಯಂಕ;
	__u64 ಸ್ಥಾನಾಂತರ;
	blk64_t *l1_ಕೋಷ್ಟಕ, *l2_ಕೋಷ್ಟಕ = ಖಾಲಿ;
	ಶೂನ್ಯ *copy_buf = ಖಾಲಿ;
	size_t ಗಾತ್ರ_2;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ max_l1_ಗಾತ್ರ;

	ಆದರೆ (hdr->crypt_ವಿಧಾನ)
		ಹಿಂತಿರುಗಿ -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = ಖಾಲಿ;
	img.l1_ಕೋಷ್ಟಕ = ಖಾಲಿ;
	img.cluster_ಬಿಟ್ಟುಗಳು = ext2fs_be32_ಗಮ್ಯ_cpu(hdr->cluster_ಬಿಟ್ಟುಗಳು);
	ಆದರೆ (img.cluster_ಬಿಟ್ಟುಗಳು < 9 || img.cluster_ಬಿಟ್ಟುಗಳು > 31)
		ಹಿಂತಿರುಗಿ -QCOW_CORRUPTED;
	img.cluster_ಗಾತ್ರ = 1 << img.cluster_ಬಿಟ್ಟುಗಳು;
	img.l1_ಗಾತ್ರ = ext2fs_be32_ಗಮ್ಯ_cpu(hdr->l1_ಗಾತ್ರ);
	img.l1_ಸ್ಥಾನಾಂತರ = ext2fs_be64_ಗಮ್ಯ_cpu(hdr->l1_ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ);
	img.l2_ಗಾತ್ರ = 1 << (img.cluster_ಬಿಟ್ಟುಗಳು - 3);
	img.image_ಗಾತ್ರ = ext2fs_be64_ಗಮ್ಯ_cpu(hdr->ಗಾತ್ರ_2);

	ಆದರೆ (img.l1_ಸ್ಥಾನಾಂತರ & (img.cluster_ಗಾತ್ರ - 1))
		ಹಿಂತಿರುಗಿ -QCOW_CORRUPTED;

	max_l1_ಗಾತ್ರ = (img.image_ಗಾತ್ರ >> ((2 * img.cluster_ಬಿಟ್ಟುಗಳು) - 3)) +
		img.cluster_ಗಾತ್ರ;
	ಆದರೆ (img.l1_ಗಾತ್ರ > max_l1_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_ಗಾತ್ರ, &l2_ಕೋಷ್ಟಕ);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

	ret = ext2fs_get_memzero(1 << img.cluster_ಬಿಟ್ಟುಗಳು, &copy_buf);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ret = qcow2_read_l1_ಕೋಷ್ಟಕ(&img);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

	l1_ಕೋಷ್ಟಕ = img.l1_ಕೋಷ್ಟಕ;
	/* Walk through l1 table */
	ಗಾಗಿ (l1_ಸೂಚ್ಯಂಕ = 0; l1_ಸೂಚ್ಯಂಕ < img.l1_ಗಾತ್ರ; l1_ಸೂಚ್ಯಂಕ++) {
		__u64 off_ಹೊರಹೋಗು;

		ಸ್ಥಾನಾಂತರ = ext2fs_be64_ಗಮ್ಯ_cpu(l1_ಕೋಷ್ಟಕ[l1_ಸೂಚ್ಯಂಕ]) &
			 ~QCOW_OFLAG_COPIED;

		ಆದರೆ ((ಸ್ಥಾನಾಂತರ > img.image_ಗಾತ್ರ) ||
		    (ಸ್ಥಾನಾಂತರ <= 0))
			ಮುಂದುವರಿ;

		ಆದರೆ (ಸ್ಥಾನಾಂತರ & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			ಹೋಗು ಹೊರಹೋಗು;
		}

		ret = qcow2_read_l2_ಕೋಷ್ಟಕ(&img, ಸ್ಥಾನಾಂತರ, &l2_ಕೋಷ್ಟಕ);
		ಆದರೆ (ret)
			ನಿಲ್ಲಿಸು;

		/* Walk through l2 table and copy data blocks into raw image */
		ಗಾಗಿ (l2_ಸೂಚ್ಯಂಕ = 0; l2_ಸೂಚ್ಯಂಕ < img.l2_ಗಾತ್ರ; l2_ಸೂಚ್ಯಂಕ++) {
			ಸ್ಥಾನಾಂತರ = ext2fs_be64_ಗಮ್ಯ_cpu(l2_ಕೋಷ್ಟಕ[l2_ಸೂಚ್ಯಂಕ]) &
				 ~QCOW_OFLAG_COPIED;

			ಆದರೆ (ಸ್ಥಾನಾಂತರ == 0)
				ಮುಂದುವರಿ;

			off_ಹೊರಹೋಗು = ((__u64)l1_ಸೂಚ್ಯಂಕ * img.l2_ಗಾತ್ರ) +
				  l2_ಸೂಚ್ಯಂಕ;
			off_ಹೊರಹೋಗು <<= img.cluster_ಬಿಟ್ಟುಗಳು;
			ret = qcow2_copy_ದತ್ತಾಂಶ(qcow2_fd, raw_fd, ಸ್ಥಾನಾಂತರ,
					off_ಹೊರಹೋಗು, copy_buf, img.cluster_ಗಾತ್ರ);
			ಆದರೆ (ret)
				ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	/* Resize the output image to the filesystem size */
	ಆದರೆ (ext2fs_llseek(raw_fd, img.image_ಗಾತ್ರ - 1, SEEK_SET) < 0) {
		ret = errno;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	((ಅಕ್ಷರ *)copy_buf)[0] = 0;
	ಗಾತ್ರ_2 = ಲಿಖ(raw_fd, copy_buf, 1);
	ಆದರೆ (ಗಾತ್ರ_2 != 1) {
		ret = errno;
		ಹೋಗು ಹೊರಹೋಗು;
	}

ಹೊರಹೋಗು:
	ಆದರೆ (copy_buf)
		ext2fs_free_mem(&copy_buf);
	ಆದರೆ (img.l1_ಕೋಷ್ಟಕ)
		ext2fs_free_mem(&img.l1_ಕೋಷ್ಟಕ);
	ಆದರೆ (l2_ಕೋಷ್ಟಕ)
		ext2fs_free_mem(&l2_ಕೋಷ್ಟಕ);
	ಹಿಂತಿರುಗಿ ret;
}
