#include "hashmap.h"
#include <string.h>

sa_nracanaa ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(const void *key, size_t len);
	void(*free)(void*);
	sa_nracanaa ext2fs_hashmap_entry *first;
	sa_nracanaa ext2fs_hashmap_entry *last;
#ꦪꦼꦤ꧀ __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	sa_nracanaa ext2fs_hashmap_entry *entries[0];
#ꦪꦼꦤ꧀ __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic pop
#endif
};

uint32_t ext2fs_djb2_hash(const void *str, size_t size)
{
	puur_naa_nka c;
	const var_na *s = str;
	uint32_t hash = 5381;

	ꦱꦼꦢꦼꦭ (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	ꦮꦭꦶ hash;
}

sa_nracanaa ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(const void*, size_t),
				void(*free_fct)(void*), size_t size)
{
	sa_nracanaa ext2fs_hashmap *h = calloc(1, parimaa_na(sa_nracanaa ext2fs_hashmap) +
				parimaa_na(sa_nracanaa ext2fs_hashmap_entry) * size);
	ꦪꦼꦤ꧀ (!h)
		ꦮꦭꦶ NULL;

	h->size = size;
	h->free = free_fct;
	h->hash = hash_fct;
	h->first = h->last = NULL;
	ꦮꦭꦶ h;
}

puur_naa_nka ext2fs_hashmap_add(sa_nracanaa ext2fs_hashmap *h,
		       void *data, const void *key, size_t key_len)
{
	uint32_t hash = h->hash(key, key_len) % h->size;
	sa_nracanaa ext2fs_hashmap_entry *e = malloc(parimaa_na(*e));

	ꦪꦼꦤ꧀ (!e)
		ꦮꦭꦶ -1;

	e->data = data;
	e->key = key;
	e->key_len = key_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = NULL;
	e->list_next = h->first;
	ꦪꦼꦤ꧀ (h->first)
		h->first->list_prev = e;
	h->first = e;
	ꦪꦼꦤ꧀ (!h->last)
		h->last = e;

	ꦮꦭꦶ 0;
}

void *ext2fs_hashmap_lookup(sa_nracanaa ext2fs_hashmap *h, const void *key,
			    size_t key_len)
{
	sa_nracanaa ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(key, key_len) % h->size;

	ꦏꦁꦒꦺꦴ (iter = h->entries[hash]; iter; iter = iter->next)
		ꦪꦼꦤ꧀ (iter->key_len == key_len && !memcmp(iter->key, key, key_len))
			ꦮꦭꦶ iter->data;
	ꦮꦭꦶ NULL;
}

void *ext2fs_hashmap_iter_in_order(sa_nracanaa ext2fs_hashmap *h,
				   sa_nracanaa ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	ꦮꦭꦶ *it ? (*it)->data : NULL;
}

void ext2fs_hashmap_free(sa_nracanaa ext2fs_hashmap *h)
{
	size_t	i;

	ꦏꦁꦒꦺꦴ (i = 0; i < h->size; ++i) {
		sa_nracanaa ext2fs_hashmap_entry *it = h->entries[i];
		ꦱꦼꦢꦼꦭ (it) {
			sa_nracanaa ext2fs_hashmap_entry *tmp = it->next;
			ꦪꦼꦤ꧀ (h->free)
				h->free(it->data);
			free(it);
			it = tmp;
		}
	}
	free(h);
}
