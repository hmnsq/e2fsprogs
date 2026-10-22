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

কাঠামো ext2fs_hashmap;

কাঠামো ext2fs_hashmap_entry {
	শূন্য *তথ্য;
	ধ্রুবক শূন্য *চাবি;
	size_t চাবি_len;
	কাঠামো ext2fs_hashmap_entry *পরবর্তী;
	কাঠামো ext2fs_hashmap_entry *list_পরবর্তী;
	কাঠামো ext2fs_hashmap_entry *list_পূর্ব;
};

কাঠামো ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ধ্রুবক শূন্য*, size_t),
				শূন্য(*free_fct)(শূন্য*), size_t আকার_2);
পূর্ণসংখ্যা ext2fs_hashmap_add(কাঠামো ext2fs_hashmap *h,
		       শূন্য *তথ্য, ধ্রুবক শূন্য *চাবি,size_t চাবি_len);
শূন্য *ext2fs_hashmap_lookup(কাঠামো ext2fs_hashmap *h, ধ্রুবক শূন্য *চাবি,
			    size_t চাবি_len);
শূন্য *ext2fs_hashmap_iter_in_order(কাঠামো ext2fs_hashmap *h,
				   কাঠামো ext2fs_hashmap_entry **it);
শূন্য ext2fs_hashmap_del(কাঠামো ext2fs_hashmap *h,
			কাঠামো ext2fs_hashmap_entry *e);
শূন্য ext2fs_hashmap_free(কাঠামো ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ধ্রুবক শূন্য *str, size_t আকার_2);

#endif /* !HASHMAP_H */
