#अन्तर्भाव "hashmap.h"
#अन्तर्भाव <string.h>

संरचना ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(ध्रुव शून्य *कुञ्चिका, size_t len);
	शून्य(*मुक्त)(शून्य*);
	संरचना ext2fs_hashmap_entry *first;
	संरचना ext2fs_hashmap_entry *last;
#जइ __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic प्रक्षिप
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	संरचना ext2fs_hashmap_entry *entries[0];
#जइ __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic निष्कासय
#endif
};

uint32_t ext2fs_djb2_hash(ध्रुव शून्य *str, size_t size)
{
	पूर्णाङ्क c;
	ध्रुव वर्ण *s = str;
	uint32_t hash = 5381;

	जाव (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	प्रत्यागम hash;
}

संरचना ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ध्रुव शून्य*, size_t),
				शून्य(*free_fct)(शून्य*), size_t size)
{
	संरचना ext2fs_hashmap *h = calloc(1, परिमाण(संरचना ext2fs_hashmap) +
				परिमाण(संरचना ext2fs_hashmap_entry) * size);
	जइ (!h)
		प्रत्यागम रिक्त;

	h->size = size;
	h->मुक्त = free_fct;
	h->hash = hash_fct;
	h->first = h->last = रिक्त;
	प्रत्यागम h;
}

पूर्णाङ्क ext2fs_hashmap_add(संरचना ext2fs_hashmap *h,
		       शून्य *data, ध्रुव शून्य *कुञ्चिका, size_t कुञ्चिका_len)
{
	uint32_t hash = h->hash(कुञ्चिका, कुञ्चिका_len) % h->size;
	संरचना ext2fs_hashmap_entry *e = malloc(परिमाण(*e));

	जइ (!e)
		प्रत्यागम -1;

	e->data = data;
	e->कुञ्चिका = कुञ्चिका;
	e->कुञ्चिका_len = कुञ्चिका_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = रिक्त;
	e->list_next = h->first;
	जइ (h->first)
		h->first->list_prev = e;
	h->first = e;
	जइ (!h->last)
		h->last = e;

	प्रत्यागम 0;
}

शून्य *ext2fs_hashmap_lookup(संरचना ext2fs_hashmap *h, ध्रुव शून्य *कुञ्चिका,
			    size_t कुञ्चिका_len)
{
	संरचना ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(कुञ्चिका, कुञ्चिका_len) % h->size;

	प्रति (iter = h->entries[hash]; iter; iter = iter->next)
		जइ (iter->कुञ्चिका_len == कुञ्चिका_len && !memcmp(iter->कुञ्चिका, कुञ्चिका, कुञ्चिका_len))
			प्रत्यागम iter->data;
	प्रत्यागम रिक्त;
}

शून्य *ext2fs_hashmap_iter_in_order(संरचना ext2fs_hashmap *h,
				   संरचना ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	प्रत्यागम *it ? (*it)->data : रिक्त;
}

शून्य ext2fs_hashmap_free(संरचना ext2fs_hashmap *h)
{
	size_t	i;

	प्रति (i = 0; i < h->size; ++i) {
		संरचना ext2fs_hashmap_entry *it = h->entries[i];
		जाव (it) {
			संरचना ext2fs_hashmap_entry *tmp = it->next;
			जइ (h->मुक्त)
				h->मुक्त(it->data);
			मुक्त(it);
			it = tmp;
		}
	}
	मुक्त(h);
}
