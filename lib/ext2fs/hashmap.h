#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ifndef __GNUC_PREREQ
#اگر defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وگرنه
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

ساختار ext2fs_hashmap;

ساختار ext2fs_hashmap_entry {
	تهی *data;
	ثابت تهی *کلید;
	size_t کلید_len;
	ساختار ext2fs_hashmap_entry *next;
	ساختار ext2fs_hashmap_entry *list_next;
	ساختار ext2fs_hashmap_entry *list_prev;
};

ساختار ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت تهی*, size_t),
				تهی(*free_fct)(تهی*), size_t size);
صحیح ext2fs_hashmap_add(ساختار ext2fs_hashmap *h,
		       تهی *data, ثابت تهی *کلید,size_t کلید_len);
تهی *ext2fs_hashmap_lookup(ساختار ext2fs_hashmap *h, ثابت تهی *کلید,
			    size_t کلید_len);
تهی *ext2fs_hashmap_iter_in_order(ساختار ext2fs_hashmap *h,
				   ساختار ext2fs_hashmap_entry **it);
تهی ext2fs_hashmap_del(ساختار ext2fs_hashmap *h,
			ساختار ext2fs_hashmap_entry *e);
تهی ext2fs_hashmap_free(ساختار ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ثابت تهی *str, size_t size);

#endif /* !HASHMAP_H */
