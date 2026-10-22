#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ifndef __GNUC_PREREQ
#אם defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#אחרת
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

بنية ext2fs_hashmap;

بنية ext2fs_hashmap_entry {
	فراغ *data;
	ثابت فراغ *key;
	size_t key_len;
	بنية ext2fs_hashmap_entry *next;
	بنية ext2fs_hashmap_entry *list_next;
	بنية ext2fs_hashmap_entry *list_prev;
};

بنية ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت فراغ*, size_t),
				فراغ(*free_fct)(فراغ*), size_t size);
صحيح ext2fs_hashmap_add(بنية ext2fs_hashmap *h,
		       فراغ *data, ثابت فراغ *key,size_t key_len);
فراغ *ext2fs_hashmap_lookup(بنية ext2fs_hashmap *h, ثابت فراغ *key,
			    size_t key_len);
فراغ *ext2fs_hashmap_iter_in_order(بنية ext2fs_hashmap *h,
				   بنية ext2fs_hashmap_entry **it);
فراغ ext2fs_hashmap_del(بنية ext2fs_hashmap *h,
			بنية ext2fs_hashmap_entry *e);
فراغ ext2fs_hashmap_free(بنية ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ثابت فراغ *str, size_t size);

#endif /* !HASHMAP_H */
