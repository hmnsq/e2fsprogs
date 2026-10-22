#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ifndef __GNUC_PREREQ
#எனில் defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#இல்லையெனில்
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

கட்டமைப்பு ext2fs_hashmap;

கட்டமைப்பு ext2fs_hashmap_entry {
	வெறுமை *data;
	மாறிலி வெறுமை *சாவி;
	size_t சாவி_len;
	கட்டமைப்பு ext2fs_hashmap_entry *next;
	கட்டமைப்பு ext2fs_hashmap_entry *list_next;
	கட்டமைப்பு ext2fs_hashmap_entry *list_prev;
};

கட்டமைப்பு ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(மாறிலி வெறுமை*, size_t),
				வெறுமை(*free_fct)(வெறுமை*), size_t size);
முழு ext2fs_hashmap_add(கட்டமைப்பு ext2fs_hashmap *h,
		       வெறுமை *data, மாறிலி வெறுமை *சாவி,size_t சாவி_len);
வெறுமை *ext2fs_hashmap_lookup(கட்டமைப்பு ext2fs_hashmap *h, மாறிலி வெறுமை *சாவி,
			    size_t சாவி_len);
வெறுமை *ext2fs_hashmap_iter_in_order(கட்டமைப்பு ext2fs_hashmap *h,
				   கட்டமைப்பு ext2fs_hashmap_entry **it);
வெறுமை ext2fs_hashmap_del(கட்டமைப்பு ext2fs_hashmap *h,
			கட்டமைப்பு ext2fs_hashmap_entry *e);
வெறுமை ext2fs_hashmap_free(கட்டமைப்பு ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(மாறிலி வெறுமை *str, size_t size);

#endif /* !HASHMAP_H */
