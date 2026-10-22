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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE64_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE64_ਸਰੋਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_STDLIB_H
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs/ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "qcow2.h"

/* Functions for converting qcow2 image into raw image */

ਬਣਤਰ ext2_qcow2_hdr *qcow2_read_ਸਿਰਲੇਖ(ਪੂਰਨ fd)
{
	ਖਾਲੀ *ਬਫਰ = ਨਿਰਾ;
	ਬਣਤਰ ext2_qcow2_hdr *hdr = ਨਿਰਾ;
	size_t ਮਾਪ;
	errcode_t ret;

	ret = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_qcow2_hdr), &ਬਫਰ);
	ਜੇ (ret)
		ਵਾਪਸ ਨਿਰਾ;
	memset(ਬਫਰ, 0, ਆਕਾਰ(ਬਣਤਰ ext2_qcow2_hdr));

	ਜੇ (ext2fs_llseek(fd, 0, SEEK_SET < 0)) {
		ext2fs_free_mem(&ਬਫਰ);
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਮਾਪ = ਪਠ(fd, ਬਫਰ, ਆਕਾਰ(ਬਣਤਰ ext2_qcow2_hdr));
	ਜੇ (ਮਾਪ != ਆਕਾਰ(ਬਣਤਰ ext2_qcow2_hdr)) {
		ext2fs_free_mem(&ਬਫਰ);
		ਵਾਪਸ ਨਿਰਾ;
	}

	hdr = (ਬਣਤਰ ext2_qcow2_hdr *)(ਬਫਰ);

	ਜੇ ((ext2fs_be32_ਮੰਜ਼ਿਲ_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_ਮੰਜ਼ਿਲ_cpu(hdr->ਸੰਸਕਰਣ) != 2)) {
		ext2fs_free_mem(&hdr);
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਵਾਪਸ hdr;
}

ਸਥਿਰ ਪੂਰਨ qcow2_read_l1_ਸਾਰਣੀ(ਬਣਤਰ ext2_qcow2_image *img)
{
	ਪੂਰਨ fd = img->fd;
	size_t ਮਾਪ, l1_ਮਾਪ = img->l1_ਮਾਪ * ਆਕਾਰ(blk64_t);
	blk64_t *ਸਾਰਣੀ;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_ਮਾਪ, &ਸਾਰਣੀ);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	ਜੇ (ext2fs_llseek(fd, img->l1_ਵਿੱਥ, SEEK_SET) < 0) {
		ext2fs_free_mem(&ਸਾਰਣੀ);
		ਵਾਪਸ errno;
	}

	ਮਾਪ = ਪਠ(fd, ਸਾਰਣੀ, l1_ਮਾਪ);
	ਜੇ (ਮਾਪ != l1_ਮਾਪ) {
		ext2fs_free_mem(&ਸਾਰਣੀ);
		ਵਾਪਸ errno;
	}

	img->l1_ਸਾਰਣੀ = ਸਾਰਣੀ;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ qcow2_read_l2_ਸਾਰਣੀ(ਬਣਤਰ ext2_qcow2_image *img,
			       __u64 ਵਿੱਥ, blk64_t **l2_ਸਾਰਣੀ)
{
	ਪੂਰਨ fd = img->fd;
	size_t ਮਾਪ;

	ਪ੍ਰਤਿਜ੍ਞਾ(*l2_ਸਾਰਣੀ);

	ਜੇ (ext2fs_llseek(fd, ਵਿੱਥ, SEEK_SET) < 0)
		ਵਾਪਸ errno;

	ਮਾਪ = ਪਠ(fd, *l2_ਸਾਰਣੀ, img->cluster_ਮਾਪ);
	ਜੇ (ਮਾਪ != img->cluster_ਮਾਪ)
		ਵਾਪਸ errno;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ qcow2_copy_ਅੰਕੜੇ(ਪੂਰਨ fdin, ਪੂਰਨ fdout, __u64 off_ਵਿੱਚ,
			   __u64 off_ਬਾਹਰਲਾ, ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2)
{
	ssize_t c1, c2, c;
	ਖਾਲੀ *ptr;
	ਪੂਰਨ retries = 10;

	ਪ੍ਰਤਿਜ੍ਞਾ(buf);

	ਜੇ (ext2fs_llseek(fdout, off_ਬਾਹਰਲਾ, SEEK_SET) < 0)
		ਵਾਪਸ errno;

	ਜੇ (ext2fs_llseek(fdin, off_ਵਿੱਚ, SEEK_SET) < 0)
		ਵਾਪਸ errno;

	ਜਦੋਂ (ਗਿਣਤੀ_2 > 0) {
		errno = 0;
		c1 = ਪਠ(fdin, buf, ਗਿਣਤੀ_2);
		ਜੇ (c1 < 0 || ((c1 == 0) && errno))
			ਵਾਪਸ errno;
		ਜੇ (c1 == 0)
			ਰੋਕੋ; 	/* EOF */

		ਲਈ (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			errno = 0;
			c2 = ਲਿਖ(fdout, ptr, c1);
			ਜੇ (c2 < 0 || ((c2 == 0) && errno))
				ਵਾਪਸ errno;
			ਜੇ (c2 == 0 && --retries <= 0)
				ਰੋਕੋ; /* This should never happen...  */
		}
		ਗਿਣਤੀ_2 -= c1;
	}
	ਵਾਪਸ 0;
}


ਪੂਰਨ qcow2_write_raw_image(ਪੂਰਨ qcow2_fd, ਪੂਰਨ raw_fd,
			      ਬਣਤਰ ext2_qcow2_hdr *hdr)
{
	ਬਣਤਰ ext2_qcow2_image img;
	errcode_t ret = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l1_ਸੂਚਕਾਂਕ, l2_ਸੂਚਕਾਂਕ;
	__u64 ਵਿੱਥ;
	blk64_t *l1_ਸਾਰਣੀ, *l2_ਸਾਰਣੀ = ਨਿਰਾ;
	ਖਾਲੀ *copy_buf = ਨਿਰਾ;
	size_t ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_l1_ਮਾਪ;

	ਜੇ (hdr->crypt_ਵਿਧੀ)
		ਵਾਪਸ -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = ਨਿਰਾ;
	img.l1_ਸਾਰਣੀ = ਨਿਰਾ;
	img.cluster_ਬਿੱਟ = ext2fs_be32_ਮੰਜ਼ਿਲ_cpu(hdr->cluster_ਬਿੱਟ);
	ਜੇ (img.cluster_ਬਿੱਟ < 9 || img.cluster_ਬਿੱਟ > 31)
		ਵਾਪਸ -QCOW_CORRUPTED;
	img.cluster_ਮਾਪ = 1 << img.cluster_ਬਿੱਟ;
	img.l1_ਮਾਪ = ext2fs_be32_ਮੰਜ਼ਿਲ_cpu(hdr->l1_ਮਾਪ);
	img.l1_ਵਿੱਥ = ext2fs_be64_ਮੰਜ਼ਿਲ_cpu(hdr->l1_ਸਾਰਣੀ_ਵਿੱਥ);
	img.l2_ਮਾਪ = 1 << (img.cluster_ਬਿੱਟ - 3);
	img.image_ਮਾਪ = ext2fs_be64_ਮੰਜ਼ਿਲ_cpu(hdr->ਮਾਪ);

	ਜੇ (img.l1_ਵਿੱਥ & (img.cluster_ਮਾਪ - 1))
		ਵਾਪਸ -QCOW_CORRUPTED;

	max_l1_ਮਾਪ = (img.image_ਮਾਪ >> ((2 * img.cluster_ਬਿੱਟ) - 3)) +
		img.cluster_ਮਾਪ;
	ਜੇ (img.l1_ਮਾਪ > max_l1_ਮਾਪ)
		ਵਾਪਸ -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_ਮਾਪ, &l2_ਸਾਰਣੀ);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

	ret = ext2fs_get_memzero(1 << img.cluster_ਬਿੱਟ, &copy_buf);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (ext2fs_llseek(raw_fd, 0, SEEK_SET) < 0) {
		ret = errno;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ret = qcow2_read_l1_ਸਾਰਣੀ(&img);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

	l1_ਸਾਰਣੀ = img.l1_ਸਾਰਣੀ;
	/* Walk through l1 table */
	ਲਈ (l1_ਸੂਚਕਾਂਕ = 0; l1_ਸੂਚਕਾਂਕ < img.l1_ਮਾਪ; l1_ਸੂਚਕਾਂਕ++) {
		__u64 off_ਬਾਹਰਲਾ;

		ਵਿੱਥ = ext2fs_be64_ਮੰਜ਼ਿਲ_cpu(l1_ਸਾਰਣੀ[l1_ਸੂਚਕਾਂਕ]) &
			 ~QCOW_OFLAG_COPIED;

		ਜੇ ((ਵਿੱਥ > img.image_ਮਾਪ) ||
		    (ਵਿੱਥ <= 0))
			ਜਾਰੀ;

		ਜੇ (ਵਿੱਥ & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ret = qcow2_read_l2_ਸਾਰਣੀ(&img, ਵਿੱਥ, &l2_ਸਾਰਣੀ);
		ਜੇ (ret)
			ਰੋਕੋ;

		/* Walk through l2 table and copy data blocks into raw image */
		ਲਈ (l2_ਸੂਚਕਾਂਕ = 0; l2_ਸੂਚਕਾਂਕ < img.l2_ਮਾਪ; l2_ਸੂਚਕਾਂਕ++) {
			ਵਿੱਥ = ext2fs_be64_ਮੰਜ਼ਿਲ_cpu(l2_ਸਾਰਣੀ[l2_ਸੂਚਕਾਂਕ]) &
				 ~QCOW_OFLAG_COPIED;

			ਜੇ (ਵਿੱਥ == 0)
				ਜਾਰੀ;

			off_ਬਾਹਰਲਾ = ((__u64)l1_ਸੂਚਕਾਂਕ * img.l2_ਮਾਪ) +
				  l2_ਸੂਚਕਾਂਕ;
			off_ਬਾਹਰਲਾ <<= img.cluster_ਬਿੱਟ;
			ret = qcow2_copy_ਅੰਕੜੇ(qcow2_fd, raw_fd, ਵਿੱਥ,
					off_ਬਾਹਰਲਾ, copy_buf, img.cluster_ਮਾਪ);
			ਜੇ (ret)
				ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	/* Resize the output image to the filesystem size */
	ਜੇ (ext2fs_llseek(raw_fd, img.image_ਮਾਪ - 1, SEEK_SET) < 0) {
		ret = errno;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	((ਅੱਖਰ *)copy_buf)[0] = 0;
	ਮਾਪ = ਲਿਖ(raw_fd, copy_buf, 1);
	ਜੇ (ਮਾਪ != 1) {
		ret = errno;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

ਬਾਹਰਲਾ:
	ਜੇ (copy_buf)
		ext2fs_free_mem(&copy_buf);
	ਜੇ (img.l1_ਸਾਰਣੀ)
		ext2fs_free_mem(&img.l1_ਸਾਰਣੀ);
	ਜੇ (l2_ਸਾਰਣੀ)
		ext2fs_free_mem(&l2_ਸਾਰਣੀ);
	ਵਾਪਸ ret;
}
