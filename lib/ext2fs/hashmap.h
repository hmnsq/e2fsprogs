#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ifndef __GNUC_PREREQ
#ಯದಿ defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ಅನ್ಯಥಾ
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

ಸಂರಚನಾ ext2fs_hashmap;

ಸಂರಚನಾ ext2fs_hashmap_entry {
	ಶೂನ್ಯ *data;
	ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ;
	size_t ಕೀಲಿ_len;
	ಸಂರಚನಾ ext2fs_hashmap_entry *next;
	ಸಂರಚನಾ ext2fs_hashmap_entry *list_next;
	ಸಂರಚನಾ ext2fs_hashmap_entry *list_prev;
};

ಸಂರಚನಾ ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ಧ್ರುವ ಶೂನ್ಯ*, size_t),
				ಶೂನ್ಯ(*free_fct)(ಶೂನ್ಯ*), size_t size);
ಪೂರ್ಣಾಙ್ಕ ext2fs_hashmap_add(ಸಂರಚನಾ ext2fs_hashmap *h,
		       ಶೂನ್ಯ *data, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ,size_t ಕೀಲಿ_len);
ಶೂನ್ಯ *ext2fs_hashmap_lookup(ಸಂರಚನಾ ext2fs_hashmap *h, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ,
			    size_t ಕೀಲಿ_len);
ಶೂನ್ಯ *ext2fs_hashmap_iter_in_order(ಸಂರಚನಾ ext2fs_hashmap *h,
				   ಸಂರಚನಾ ext2fs_hashmap_entry **it);
ಶೂನ್ಯ ext2fs_hashmap_del(ಸಂರಚನಾ ext2fs_hashmap *h,
			ಸಂರಚನಾ ext2fs_hashmap_entry *e);
ಶೂನ್ಯ ext2fs_hashmap_free(ಸಂರಚನಾ ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ಧ್ರುವ ಶೂನ್ಯ *str, size_t size);

#endif /* !HASHMAP_H */
