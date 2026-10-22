#अन्तर्भाव "hashmap.h"
#अन्तर्भाव <string.h>

संरचना ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(ध्रुव शून्य *कुंजी, माप_प्रकार len);
	शून्य(*मुक्त)(शून्य*);
	संरचना ext2fs_hashmap_entry *first;
	संरचना ext2fs_hashmap_entry *last;
#यदि __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic धकेलो
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	संरचना ext2fs_hashmap_entry *entries[0];
#यदि __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic निकालो
#endif
};

uint32_t ext2fs_djb2_hash(ध्रुव शून्य *str, माप_प्रकार size)
{
	पूर्णाङ्क c;
	ध्रुव वर्ण *s = str;
	uint32_t hash = 5381;

	यावत् (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	प्रत्यागम hash;
}

संरचना ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ध्रुव शून्य*, माप_प्रकार),
				शून्य(*free_fct)(शून्य*), माप_प्रकार size)
{
	संरचना ext2fs_hashmap *h = सुस्मृति(1, परिमाण(संरचना ext2fs_hashmap) +
				परिमाण(संरचना ext2fs_hashmap_entry) * size);
	यदि (!h)
		प्रत्यागम रिक्त;

	h->size = size;
	h->मुक्त = free_fct;
	h->hash = hash_fct;
	h->first = h->last = रिक्त;
	प्रत्यागम h;
}

पूर्णाङ्क ext2fs_hashmap_add(संरचना ext2fs_hashmap *h,
		       शून्य *data, ध्रुव शून्य *कुंजी, माप_प्रकार कुंजी_len)
{
	uint32_t hash = h->hash(कुंजी, कुंजी_len) % h->size;
	संरचना ext2fs_hashmap_entry *e = दो_स्मृति(परिमाण(*e));

	यदि (!e)
		प्रत्यागम -1;

	e->data = data;
	e->कुंजी = कुंजी;
	e->कुंजी_len = कुंजी_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = रिक्त;
	e->list_next = h->first;
	यदि (h->first)
		h->first->list_prev = e;
	h->first = e;
	यदि (!h->last)
		h->last = e;

	प्रत्यागम 0;
}

शून्य *ext2fs_hashmap_lookup(संरचना ext2fs_hashmap *h, ध्रुव शून्य *कुंजी,
			    माप_प्रकार कुंजी_len)
{
	संरचना ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(कुंजी, कुंजी_len) % h->size;

	प्रति (iter = h->entries[hash]; iter; iter = iter->next)
		यदि (iter->कुंजी_len == कुंजी_len && !स_भेद(iter->कुंजी, कुंजी, कुंजी_len))
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
	माप_प्रकार	i;

	प्रति (i = 0; i < h->size; ++i) {
		संरचना ext2fs_hashmap_entry *it = h->entries[i];
		यावत् (it) {
			संरचना ext2fs_hashmap_entry *tmp = it->next;
			यदि (h->मुक्त)
				h->मुक्त(it->data);
			मुक्त(it);
			it = tmp;
		}
	}
	मुक्त(h);
}
