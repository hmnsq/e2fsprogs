#include "hashmap.h"
#include <string.h>

struct ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(const void *key, size_t len);
	void(*free)(void*);
	struct ext2fs_hashmap_entry *first;
	struct ext2fs_hashmap_entry *last;
#якщо __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	struct ext2fs_hashmap_entry *entries[0];
#якщо __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic pop
#endif
};

uint32_t ext2fs_djb2_hash(const void *str, size_t size)
{
	ціле c;
	const char *s = str;
	uint32_t hash = 5381;

	поки (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	повернути hash;
}

struct ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(const void*, size_t),
				void(*free_fct)(void*), size_t size)
{
	struct ext2fs_hashmap *h = calloc(1, sizeof(struct ext2fs_hashmap) +
				sizeof(struct ext2fs_hashmap_entry) * size);
	якщо (!h)
		повернути NULL;

	h->size = size;
	h->free = free_fct;
	h->hash = hash_fct;
	h->first = h->last = NULL;
	повернути h;
}

ціле ext2fs_hashmap_add(struct ext2fs_hashmap *h,
		       void *data, const void *key, size_t key_len)
{
	uint32_t hash = h->hash(key, key_len) % h->size;
	struct ext2fs_hashmap_entry *e = malloc(sizeof(*e));

	якщо (!e)
		повернути -1;

	e->data = data;
	e->key = key;
	e->key_len = key_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = NULL;
	e->list_next = h->first;
	якщо (h->first)
		h->first->list_prev = e;
	h->first = e;
	якщо (!h->last)
		h->last = e;

	повернути 0;
}

void *ext2fs_hashmap_lookup(struct ext2fs_hashmap *h, const void *key,
			    size_t key_len)
{
	struct ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(key, key_len) % h->size;

	для (iter = h->entries[hash]; iter; iter = iter->next)
		якщо (iter->key_len == key_len && !memcmp(iter->key, key, key_len))
			повернути iter->data;
	повернути NULL;
}

void *ext2fs_hashmap_iter_in_order(struct ext2fs_hashmap *h,
				   struct ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	повернути *it ? (*it)->data : NULL;
}

void ext2fs_hashmap_free(struct ext2fs_hashmap *h)
{
	size_t	i;

	для (i = 0; i < h->size; ++i) {
		struct ext2fs_hashmap_entry *it = h->entries[i];
		поки (it) {
			struct ext2fs_hashmap_entry *tmp = it->next;
			якщо (h->free)
				h->free(it->data);
			free(it);
			it = tmp;
		}
	}
	free(h);
}
