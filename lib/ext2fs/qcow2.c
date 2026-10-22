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

#अगर_अघोषित _LARGEFILE_SOURCE
#घोषणा _LARGEFILE_SOURCE
#पूर्ण_अगर
#अगर_अघोषित _LARGEFILE64_SOURCE
#घोषणा _LARGEFILE64_SOURCE
#पूर्ण_अगर

#समावेश "config.h"
#समावेश <fcntl.h>
#समावेश <grp.h>
#समावेश <pwd.h>
#समावेश <stdio.h>
#अगर_घोषित HAVE_STDLIB_H
#समावेश <stdlib.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <time.h>
#समावेश <unistd.h>
#समावेश <fcntl.h>
#समावेश <errno.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश <assert.h>

#समावेश "ext2fs/ext2fs.h"
#समावेश "qcow2.h"

/* Functions for converting qcow2 image into raw image */

काष्ठा ext2_qcow2_hdr *qcow2_read_header(पूर्णांक fd)
{
	व्योम *बफर = शून्य;
	काष्ठा ext2_qcow2_hdr *hdr = शून्य;
	माप_प्रकार आकार;
	errcode_t ret;

	ret = ext2fs_get_mem(माप(काष्ठा ext2_qcow2_hdr), &बफर);
	अगर (ret)
		वापस शून्य;
	स_रखो(बफर, 0, माप(काष्ठा ext2_qcow2_hdr));

	अगर (ext2fs_llseek(fd, 0, शुरू_से < 0)) {
		ext2fs_free_mem(&बफर);
		वापस शून्य;
	}

	आकार = पढ़ो(fd, बफर, माप(काष्ठा ext2_qcow2_hdr));
	अगर (आकार != माप(काष्ठा ext2_qcow2_hdr)) {
		ext2fs_free_mem(&बफर);
		वापस शून्य;
	}

	hdr = (काष्ठा ext2_qcow2_hdr *)(बफर);

	अगर ((ext2fs_be32_to_cpu(hdr->magic) != QCOW_MAGIC) ||
	    (ext2fs_be32_to_cpu(hdr->version) != 2)) {
		ext2fs_free_mem(&hdr);
		वापस शून्य;
	}

	वापस hdr;
}

अटल पूर्णांक qcow2_read_l1_table(काष्ठा ext2_qcow2_image *img)
{
	पूर्णांक fd = img->fd;
	माप_प्रकार आकार, l1_आकार = img->l1_आकार * माप(blk64_t);
	blk64_t *table;
	errcode_t ret;

	ret = ext2fs_get_memzero(l1_आकार, &table);
	अगर (ret)
		वापस ret;

	अगर (ext2fs_llseek(fd, img->l1_offset, शुरू_से) < 0) {
		ext2fs_free_mem(&table);
		वापस त्रुटि_सं;
	}

	आकार = पढ़ो(fd, table, l1_आकार);
	अगर (आकार != l1_आकार) {
		ext2fs_free_mem(&table);
		वापस त्रुटि_सं;
	}

	img->l1_table = table;

	वापस 0;
}

अटल पूर्णांक qcow2_read_l2_table(काष्ठा ext2_qcow2_image *img,
			       __u64 offset, blk64_t **l2_table)
{
	पूर्णांक fd = img->fd;
	माप_प्रकार आकार;

	निश्चित(*l2_table);

	अगर (ext2fs_llseek(fd, offset, शुरू_से) < 0)
		वापस त्रुटि_सं;

	आकार = पढ़ो(fd, *l2_table, img->cluster_आकार);
	अगर (आकार != img->cluster_आकार)
		वापस त्रुटि_सं;

	वापस 0;
}

अटल पूर्णांक qcow2_copy_तथ्य(पूर्णांक fdin, पूर्णांक fdout, __u64 off_in,
			   __u64 off_out, व्योम *buf, माप_प्रकार गिनती)
{
	ssize_t c1, c2, c;
	व्योम *ptr;
	पूर्णांक retries = 10;

	निश्चित(buf);

	अगर (ext2fs_llseek(fdout, off_out, शुरू_से) < 0)
		वापस त्रुटि_सं;

	अगर (ext2fs_llseek(fdin, off_in, शुरू_से) < 0)
		वापस त्रुटि_सं;

	जबतक (गिनती > 0) {
		त्रुटि_सं = 0;
		c1 = पढ़ो(fdin, buf, गिनती);
		अगर (c1 < 0 || ((c1 == 0) && त्रुटि_सं))
			वापस त्रुटि_सं;
		अगर (c1 == 0)
			अवरोध; 	/* EOF */

		क्रम (ptr = buf, c = c1; c > 0; ptr += c2, c -= c2) {
			त्रुटि_सं = 0;
			c2 = ग_लिखो(fdout, ptr, c1);
			अगर (c2 < 0 || ((c2 == 0) && त्रुटि_सं))
				वापस त्रुटि_सं;
			अगर (c2 == 0 && --retries <= 0)
				अवरोध; /* This should never happen...  */
		}
		गिनती -= c1;
	}
	वापस 0;
}


पूर्णांक qcow2_write_raw_image(पूर्णांक qcow2_fd, पूर्णांक raw_fd,
			      काष्ठा ext2_qcow2_hdr *hdr)
{
	काष्ठा ext2_qcow2_image img;
	errcode_t ret = 0;
	अचिन्हित पूर्णांक l1_सूचकांक, l2_सूचकांक;
	__u64 offset;
	blk64_t *l1_table, *l2_table = शून्य;
	व्योम *copy_buf = शून्य;
	माप_प्रकार आकार;
	अचिन्हित पूर्णांक max_l1_आकार;

	अगर (hdr->crypt_method)
		वापस -QCOW_ENCRYPTED;

	img.fd = qcow2_fd;
	img.hdr = hdr;
	img.l2_cache = शून्य;
	img.l1_table = शून्य;
	img.cluster_bits = ext2fs_be32_to_cpu(hdr->cluster_bits);
	अगर (img.cluster_bits < 9 || img.cluster_bits > 31)
		वापस -QCOW_CORRUPTED;
	img.cluster_आकार = 1 << img.cluster_bits;
	img.l1_आकार = ext2fs_be32_to_cpu(hdr->l1_आकार);
	img.l1_offset = ext2fs_be64_to_cpu(hdr->l1_table_offset);
	img.l2_आकार = 1 << (img.cluster_bits - 3);
	img.image_आकार = ext2fs_be64_to_cpu(hdr->आकार);

	अगर (img.l1_offset & (img.cluster_आकार - 1))
		वापस -QCOW_CORRUPTED;

	max_l1_आकार = (img.image_आकार >> ((2 * img.cluster_bits) - 3)) +
		img.cluster_आकार;
	अगर (img.l1_आकार > max_l1_आकार)
		वापस -QCOW_CORRUPTED;

	ret = ext2fs_get_memzero(img.cluster_आकार, &l2_table);
	अगर (ret)
		जाओ out;

	ret = ext2fs_get_memzero(1 << img.cluster_bits, &copy_buf);
	अगर (ret)
		जाओ out;

	अगर (ext2fs_llseek(raw_fd, 0, शुरू_से) < 0) {
		ret = त्रुटि_सं;
		जाओ out;
	}

	ret = qcow2_read_l1_table(&img);
	अगर (ret)
		जाओ out;

	l1_table = img.l1_table;
	/* Walk through l1 table */
	क्रम (l1_सूचकांक = 0; l1_सूचकांक < img.l1_आकार; l1_सूचकांक++) {
		__u64 off_out;

		offset = ext2fs_be64_to_cpu(l1_table[l1_सूचकांक]) &
			 ~QCOW_OFLAG_COPIED;

		अगर ((offset > img.image_आकार) ||
		    (offset <= 0))
			जारी;

		अगर (offset & QCOW_OFLAG_COMPRESSED) {
			ret = -QCOW_COMPRESSED;
			जाओ out;
		}

		ret = qcow2_read_l2_table(&img, offset, &l2_table);
		अगर (ret)
			अवरोध;

		/* Walk through l2 table and copy data blocks into raw image */
		क्रम (l2_सूचकांक = 0; l2_सूचकांक < img.l2_आकार; l2_सूचकांक++) {
			offset = ext2fs_be64_to_cpu(l2_table[l2_सूचकांक]) &
				 ~QCOW_OFLAG_COPIED;

			अगर (offset == 0)
				जारी;

			off_out = ((__u64)l1_सूचकांक * img.l2_आकार) +
				  l2_सूचकांक;
			off_out <<= img.cluster_bits;
			ret = qcow2_copy_तथ्य(qcow2_fd, raw_fd, offset,
					off_out, copy_buf, img.cluster_आकार);
			अगर (ret)
				जाओ out;
		}
	}

	/* Resize the output image to the filesystem size */
	अगर (ext2fs_llseek(raw_fd, img.image_आकार - 1, शुरू_से) < 0) {
		ret = त्रुटि_सं;
		जाओ out;
	}

	((अक्षर *)copy_buf)[0] = 0;
	आकार = ग_लिखो(raw_fd, copy_buf, 1);
	अगर (आकार != 1) {
		ret = त्रुटि_सं;
		जाओ out;
	}

out:
	अगर (copy_buf)
		ext2fs_free_mem(&copy_buf);
	अगर (img.l1_table)
		ext2fs_free_mem(&img.l1_table);
	अगर (l2_table)
		ext2fs_free_mem(&l2_table);
	वापस ret;
}
