#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ifndef __GNUC_PREREQ
#যদি defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#অন্যথা
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

সংরচনা ext2fs_hashmap;

সংরচনা ext2fs_hashmap_entry {
	শূন্য *data;
	ধ্রুব শূন্য *চাবি;
	size_t চাবি_len;
	সংরচনা ext2fs_hashmap_entry *next;
	সংরচনা ext2fs_hashmap_entry *list_next;
	সংরচনা ext2fs_hashmap_entry *list_prev;
};

সংরচনা ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ধ্রুব শূন্য*, size_t),
				শূন্য(*free_fct)(শূন্য*), size_t size);
পূর্ণাঙ্ক ext2fs_hashmap_add(সংরচনা ext2fs_hashmap *h,
		       শূন্য *data, ধ্রুব শূন্য *চাবি,size_t চাবি_len);
শূন্য *ext2fs_hashmap_lookup(সংরচনা ext2fs_hashmap *h, ধ্রুব শূন্য *চাবি,
			    size_t চাবি_len);
শূন্য *ext2fs_hashmap_iter_in_order(সংরচনা ext2fs_hashmap *h,
				   সংরচনা ext2fs_hashmap_entry **it);
শূন্য ext2fs_hashmap_del(সংরচনা ext2fs_hashmap *h,
			সংরচনা ext2fs_hashmap_entry *e);
শূন্য ext2fs_hashmap_free(সংরচনা ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ধ্রুব শূন্য *str, size_t size);

#endif /* !HASHMAP_H */
