#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ifndef __GNUC_PREREQ
#ଯଦି defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ଅନ୍ୟଥା
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

ସଂରଚନା ext2fs_hashmap;

ସଂରଚନା ext2fs_hashmap_entry {
	ଶୂନ୍ୟ *data;
	ଅଚଳ ଶୂନ୍ୟ *ଚାବି;
	size_t ଚାବି_len;
	ସଂରଚନା ext2fs_hashmap_entry *next;
	ସଂରଚନା ext2fs_hashmap_entry *list_next;
	ସଂରଚନା ext2fs_hashmap_entry *list_prev;
};

ସଂରଚନା ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ଅଚଳ ଶୂନ୍ୟ*, size_t),
				ଶୂନ୍ୟ(*free_fct)(ଶୂନ୍ୟ*), size_t size);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_hashmap_add(ସଂରଚନା ext2fs_hashmap *h,
		       ଶୂନ୍ୟ *data, ଅଚଳ ଶୂନ୍ୟ *ଚାବି,size_t ଚାବି_len);
ଶୂନ୍ୟ *ext2fs_hashmap_lookup(ସଂରଚନା ext2fs_hashmap *h, ଅଚଳ ଶୂନ୍ୟ *ଚାବି,
			    size_t ଚାବି_len);
ଶୂନ୍ୟ *ext2fs_hashmap_iter_in_order(ସଂରଚନା ext2fs_hashmap *h,
				   ସଂରଚନା ext2fs_hashmap_entry **it);
ଶୂନ୍ୟ ext2fs_hashmap_del(ସଂରଚନା ext2fs_hashmap *h,
			ସଂରଚନା ext2fs_hashmap_entry *e);
ଶୂନ୍ୟ ext2fs_hashmap_free(ସଂରଚନା ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ଅଚଳ ଶୂନ୍ୟ *str, size_t size);

#endif /* !HASHMAP_H */
