#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ifndef __GNUC_PREREQ
#ᮚᮦᮔ᮪ defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ᮜᮤᮚ
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

sa_nracanaa ext2fs_hashmap;

sa_nracanaa ext2fs_hashmap_entry {
	void *data;
	const void *key;
	size_t key_len;
	sa_nracanaa ext2fs_hashmap_entry *next;
	sa_nracanaa ext2fs_hashmap_entry *list_next;
	sa_nracanaa ext2fs_hashmap_entry *list_prev;
};

sa_nracanaa ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(const void*, size_t),
				void(*free_fct)(void*), size_t size);
puur_naa_nka ext2fs_hashmap_add(sa_nracanaa ext2fs_hashmap *h,
		       void *data, const void *key,size_t key_len);
void *ext2fs_hashmap_lookup(sa_nracanaa ext2fs_hashmap *h, const void *key,
			    size_t key_len);
void *ext2fs_hashmap_iter_in_order(sa_nracanaa ext2fs_hashmap *h,
				   sa_nracanaa ext2fs_hashmap_entry **it);
void ext2fs_hashmap_del(sa_nracanaa ext2fs_hashmap *h,
			sa_nracanaa ext2fs_hashmap_entry *e);
void ext2fs_hashmap_free(sa_nracanaa ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(const void *str, size_t size);

#endif /* !HASHMAP_H */
