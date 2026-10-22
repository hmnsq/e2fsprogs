#include "hashmap.h"
#include <string.h>

struct ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(const void *key, size_t len);
	void(*free)(void*);
	struct ext2fs_hashmap_entry *first;
	struct ext2fs_hashmap_entry *last;
#ако __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	struct ext2fs_hashmap_entry *entries[0];
#ако __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic pop
#endif
};

uint32_t ext2fs_djb2_hash(const void *str, size_t size)
{
	цео c;
	const char *s = str;
	uint32_t hash = 5381;

	док (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	врати hash;
}

struct ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(const void*, size_t),
				void(*free_fct)(void*), size_t size)
{
	struct ext2fs_hashmap *h = calloc(1, sizeof(struct ext2fs_hashmap) +
				sizeof(struct ext2fs_hashmap_entry) * size);
	ако (!h)
		врати NULL;

	h->size = size;
	h->free = free_fct;
	h->hash = hash_fct;
	h->first = h->last = NULL;
	врати h;
}

цео ext2fs_hashmap_add(struct ext2fs_hashmap *h,
		       void *data, const void *key, size_t key_len)
{
	uint32_t hash = h->hash(key, key_len) % h->size;
	struct ext2fs_hashmap_entry *e = malloc(sizeof(*e));

	ако (!e)
		врати -1;

	e->data = data;
	e->key = key;
	e->key_len = key_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = NULL;
	e->list_next = h->first;
	ако (h->first)
		h->first->list_prev = e;
	h->first = e;
	ако (!h->last)
		h->last = e;

	врати 0;
}

void *ext2fs_hashmap_lookup(struct ext2fs_hashmap *h, const void *key,
			    size_t key_len)
{
	struct ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(key, key_len) % h->size;

	за (iter = h->entries[hash]; iter; iter = iter->next)
		ако (iter->key_len == key_len && !memcmp(iter->key, key, key_len))
			врати iter->data;
	врати NULL;
}

void *ext2fs_hashmap_iter_in_order(struct ext2fs_hashmap *h,
				   struct ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	врати *it ? (*it)->data : NULL;
}

void ext2fs_hashmap_free(struct ext2fs_hashmap *h)
{
	size_t	i;

	за (i = 0; i < h->size; ++i) {
		struct ext2fs_hashmap_entry *it = h->entries[i];
		док (it) {
			struct ext2fs_hashmap_entry *tmp = it->next;
			ако (h->free)
				h->free(it->data);
			free(it);
			it = tmp;
		}
	}
	free(h);
}
