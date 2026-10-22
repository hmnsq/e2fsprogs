#include "hashmap.h"
#include <string.h>

sa_nracanaa ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(const void *key, size_t len);
	void(*free)(void*);
	sa_nracanaa ext2fs_hashmap_entry *first;
	sa_nracanaa ext2fs_hashmap_entry *last;
#ຖ້າ __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	sa_nracanaa ext2fs_hashmap_entry *entries[0];
#ຖ້າ __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic pop
#endif
};

uint32_t ext2fs_djb2_hash(const void *str, size_t size)
{
	ຈຳນວນເຕັມ c;
	const var_na *s = str;
	uint32_t hash = 5381;

	ໃນຂະນະທີ່ (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	ສົ່ງຄືນ hash;
}

sa_nracanaa ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(const void*, size_t),
				void(*free_fct)(void*), size_t size)
{
	sa_nracanaa ext2fs_hashmap *h = calloc(1, parimaa_na(sa_nracanaa ext2fs_hashmap) +
				parimaa_na(sa_nracanaa ext2fs_hashmap_entry) * size);
	ຖ້າ (!h)
		ສົ່ງຄືນ NULL;

	h->size = size;
	h->free = free_fct;
	h->hash = hash_fct;
	h->first = h->last = NULL;
	ສົ່ງຄືນ h;
}

ຈຳນວນເຕັມ ext2fs_hashmap_add(sa_nracanaa ext2fs_hashmap *h,
		       void *data, const void *key, size_t key_len)
{
	uint32_t hash = h->hash(key, key_len) % h->size;
	sa_nracanaa ext2fs_hashmap_entry *e = malloc(parimaa_na(*e));

	ຖ້າ (!e)
		ສົ່ງຄືນ -1;

	e->data = data;
	e->key = key;
	e->key_len = key_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = NULL;
	e->list_next = h->first;
	ຖ້າ (h->first)
		h->first->list_prev = e;
	h->first = e;
	ຖ້າ (!h->last)
		h->last = e;

	ສົ່ງຄືນ 0;
}

void *ext2fs_hashmap_lookup(sa_nracanaa ext2fs_hashmap *h, const void *key,
			    size_t key_len)
{
	sa_nracanaa ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(key, key_len) % h->size;

	ສຳລັບ (iter = h->entries[hash]; iter; iter = iter->next)
		ຖ້າ (iter->key_len == key_len && !memcmp(iter->key, key, key_len))
			ສົ່ງຄືນ iter->data;
	ສົ່ງຄືນ NULL;
}

void *ext2fs_hashmap_iter_in_order(sa_nracanaa ext2fs_hashmap *h,
				   sa_nracanaa ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	ສົ່ງຄືນ *it ? (*it)->data : NULL;
}

void ext2fs_hashmap_free(sa_nracanaa ext2fs_hashmap *h)
{
	size_t	i;

	ສຳລັບ (i = 0; i < h->size; ++i) {
		sa_nracanaa ext2fs_hashmap_entry *it = h->entries[i];
		ໃນຂະນະທີ່ (it) {
			sa_nracanaa ext2fs_hashmap_entry *tmp = it->next;
			ຖ້າ (h->free)
				h->free(it->data);
			free(it);
			it = tmp;
		}
	}
	free(h);
}
