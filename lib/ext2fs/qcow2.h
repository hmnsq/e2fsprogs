/*
 * qcow2.h --- structures and function prototypes for qcow2.c to generate
 * qcow2 formatted disk images.  This format is used originally by QEMU
 * for virtual machines, and stores the filesystem data on disk in a
 * packed format to avoid creating sparse image files that need lots of
 * seeking to read and write.
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

#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_types.h>

/* Number of l2 tables in memory before writeback */
#ವ್ಯಾಖ್ಯಾನಿಸು L2_CACHE_PREALLOC	512


#ವ್ಯಾಖ್ಯಾನಿಸು QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#ವ್ಯಾಖ್ಯಾನಿಸು QCOW_ಆವೃತ್ತಿ		2
#ವ್ಯಾಖ್ಯಾನಿಸು QCOW_OFLAG_COPIED	(1ULL << 63)
#ವ್ಯಾಖ್ಯಾನಿಸು QCOW_OFLAG_COMPRESSED	(1ULL << 62)

#ವ್ಯಾಖ್ಯಾನಿಸು QCOW_COMPRESSED		1
#ವ್ಯಾಖ್ಯಾನಿಸು QCOW_ENCRYPTED		2
#ವ್ಯಾಖ್ಯಾನಿಸು QCOW_CORRUPTED		3

ರಚನೆ ext2_qcow2_hdr {
	__u32	magic;
	__u32	ಆವೃತ್ತಿ;

	__u64	backing_file_ಸ್ಥಾನಾಂತರ;
	__u32	backing_file_ಗಾತ್ರ;

	__u32	cluster_ಬಿಟ್ಟುಗಳು;
	__u64	ಗಾತ್ರ_2;
	__u32	crypt_ವಿಧಾನ;

	__u32	l1_ಗಾತ್ರ;
	__u64	l1_ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ;

	__u64	refcount_ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ;
	__u32	refcount_ಕೋಷ್ಟಕ_clusters;

	__u32	nb_snapshots;
	__u64	snapshots_ಸ್ಥಾನಾಂತರ;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2_qcow2_l2_ಕೋಷ್ಟಕ L2_CACHE_ತಲೆ;

ರಚನೆ ext2_qcow2_l2_ಕೋಷ್ಟಕ {
	__u32		l1_ಸೂಚ್ಯಂಕ;
	__u64		ಸ್ಥಾನಾಂತರ;
	__u64		*ದತ್ತಾಂಶ;
	L2_CACHE_ತಲೆ	*ಮುಂದಿನ;
};

ರಚನೆ ext2_qcow2_l2_cache {
	L2_CACHE_ತಲೆ	*ಬಳಸಿದ_ತಲೆ;
	L2_CACHE_ತಲೆ	*ಬಳಸಿದ_ಬಾಲ;
	L2_CACHE_ತಲೆ	*free_ತಲೆ;
	__u32		ಬಿಡುಗಡೆ;
	__u32		ಎಣಿಕೆ_2;
	__u64		ಮುಂದಿನ_ಸ್ಥಾನಾಂತರ;
};

ರಚನೆ ext2_qcow2_refcount {
	__u64	*refcount_ಕೋಷ್ಟಕ;
	__u64	refcount_ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ;
	__u64	refcount_ಖಂಡ_ಸ್ಥಾನಾಂತರ;

	__u32	refcount_ಕೋಷ್ಟಕ_clusters;
	__u32	refcount_ಕೋಷ್ಟಕ_ಸೂಚ್ಯಂಕ;
	__u32	refcount_ಖಂಡ_ಸೂಚ್ಯಂಕ;

	__u16	*refcount_ಖಂಡ;
};

ರಚನೆ ext2_qcow2_image {
	ಪೂರ್ಣಾಂಕ	fd;
	ರಚನೆ	ext2_qcow2_hdr		*hdr;
	ರಚನೆ	ext2_qcow2_l2_cache	*l2_cache;
	ರಚನೆ	ext2_qcow2_refcount	refcount;
	__u32	cluster_ಗಾತ್ರ;
	__u32	cluster_ಬಿಟ್ಟುಗಳು;
	__u32	l1_ಗಾತ್ರ;
	__u32	l2_ಗಾತ್ರ;

	__u64	*l1_ಕೋಷ್ಟಕ;
	__u64	l2_ಸ್ಥಾನಾಂತರ;
	__u64	l1_ಸ್ಥಾನಾಂತರ;
	__u64	image_ಗಾತ್ರ;
};

/* Function prototypes */

/* qcow2.c */

/* Functions for converting qcow2 image into raw image */
ರಚನೆ ext2_qcow2_hdr *qcow2_read_ಶೀರ್ಷಿಕೆ(ಪೂರ್ಣಾಂಕ);
ಪೂರ್ಣಾಂಕ qcow2_write_raw_image(ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ರಚನೆ ext2_qcow2_hdr *);

