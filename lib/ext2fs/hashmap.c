#अन्तर्भाव "hashmap.h"
#अन्तर्भाव <string.h>

संरचना ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(स्थिरांक शून्य *किल्ली, माप_प्रकार len);
	शून्य(*मुक्त)(शून्य*);
	संरचना ext2fs_hashmap_entry *first;
	संरचना ext2fs_hashmap_entry *last;
#जर __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic ढकला
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	संरचना ext2fs_hashmap_entry *entries[0];
#जर __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic काढा
#endif
};

uint32_t ext2fs_djb2_hash(स्थिरांक शून्य *str, माप_प्रकार size)
{
	पूर्णांक c;
	स्थिरांक अक्षर *s = str;
	uint32_t hash = 5381;

	पर्यंत (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	परत hash;
}

संरचना ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(स्थिरांक शून्य*, माप_प्रकार),
				शून्य(*free_fct)(शून्य*), माप_प्रकार size)
{
	संरचना ext2fs_hashmap *h = सुस्मृति(1, आकार(संरचना ext2fs_hashmap) +
				आकार(संरचना ext2fs_hashmap_entry) * size);
	जर (!h)
		परत रिक्त;

	h->size = size;
	h->मुक्त = free_fct;
	h->hash = hash_fct;
	h->first = h->last = रिक्त;
	परत h;
}

पूर्णांक ext2fs_hashmap_add(संरचना ext2fs_hashmap *h,
		       शून्य *data, स्थिरांक शून्य *किल्ली, माप_प्रकार किल्ली_len)
{
	uint32_t hash = h->hash(किल्ली, किल्ली_len) % h->size;
	संरचना ext2fs_hashmap_entry *e = दो_स्मृति(आकार(*e));

	जर (!e)
		परत -1;

	e->data = data;
	e->किल्ली = किल्ली;
	e->किल्ली_len = किल्ली_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = रिक्त;
	e->list_next = h->first;
	जर (h->first)
		h->first->list_prev = e;
	h->first = e;
	जर (!h->last)
		h->last = e;

	परत 0;
}

शून्य *ext2fs_hashmap_lookup(संरचना ext2fs_hashmap *h, स्थिरांक शून्य *किल्ली,
			    माप_प्रकार किल्ली_len)
{
	संरचना ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(किल्ली, किल्ली_len) % h->size;

	साठी (iter = h->entries[hash]; iter; iter = iter->next)
		जर (iter->किल्ली_len == किल्ली_len && !स_भेद(iter->किल्ली, किल्ली, किल्ली_len))
			परत iter->data;
	परत रिक्त;
}

शून्य *ext2fs_hashmap_iter_in_order(संरचना ext2fs_hashmap *h,
				   संरचना ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	परत *it ? (*it)->data : रिक्त;
}

शून्य ext2fs_hashmap_free(संरचना ext2fs_hashmap *h)
{
	माप_प्रकार	i;

	साठी (i = 0; i < h->size; ++i) {
		संरचना ext2fs_hashmap_entry *it = h->entries[i];
		पर्यंत (it) {
			संरचना ext2fs_hashmap_entry *tmp = it->next;
			जर (h->मुक्त)
				h->मुक्त(it->data);
			मुक्त(it);
			it = tmp;
		}
	}
	मुक्त(h);
}
