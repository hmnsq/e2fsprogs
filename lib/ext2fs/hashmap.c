#ضمن "hashmap.h"
#ضمن <string.h>

بنية ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(ثابت فراغ *مفتاح, size_t len);
	فراغ(*حرر)(فراغ*);
	بنية ext2fs_hashmap_entry *first;
	بنية ext2fs_hashmap_entry *last;
#إذا __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic ادفع
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	بنية ext2fs_hashmap_entry *entries[0];
#إذا __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic اسحب
#endif
};

uint32_t ext2fs_djb2_hash(ثابت فراغ *str, size_t size)
{
	صحيح c;
	ثابت حرف *s = str;
	uint32_t hash = 5381;

	بينما (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	إرجاع hash;
}

بنية ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت فراغ*, size_t),
				فراغ(*free_fct)(فراغ*), size_t size)
{
	بنية ext2fs_hashmap *h = calloc(1, حجم(بنية ext2fs_hashmap) +
				حجم(بنية ext2fs_hashmap_entry) * size);
	إذا (!h)
		إرجاع عدم;

	h->size = size;
	h->حرر = free_fct;
	h->hash = hash_fct;
	h->first = h->last = عدم;
	إرجاع h;
}

صحيح ext2fs_hashmap_add(بنية ext2fs_hashmap *h,
		       فراغ *data, ثابت فراغ *مفتاح, size_t مفتاح_len)
{
	uint32_t hash = h->hash(مفتاح, مفتاح_len) % h->size;
	بنية ext2fs_hashmap_entry *e = malloc(حجم(*e));

	إذا (!e)
		إرجاع -1;

	e->data = data;
	e->مفتاح = مفتاح;
	e->مفتاح_len = مفتاح_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = عدم;
	e->list_next = h->first;
	إذا (h->first)
		h->first->list_prev = e;
	h->first = e;
	إذا (!h->last)
		h->last = e;

	إرجاع 0;
}

فراغ *ext2fs_hashmap_lookup(بنية ext2fs_hashmap *h, ثابت فراغ *مفتاح,
			    size_t مفتاح_len)
{
	بنية ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(مفتاح, مفتاح_len) % h->size;

	لكل (iter = h->entries[hash]; iter; iter = iter->next)
		إذا (iter->مفتاح_len == مفتاح_len && !memcmp(iter->مفتاح, مفتاح, مفتاح_len))
			إرجاع iter->data;
	إرجاع عدم;
}

فراغ *ext2fs_hashmap_iter_in_order(بنية ext2fs_hashmap *h,
				   بنية ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	إرجاع *it ? (*it)->data : عدم;
}

فراغ ext2fs_hashmap_free(بنية ext2fs_hashmap *h)
{
	size_t	i;

	لكل (i = 0; i < h->size; ++i) {
		بنية ext2fs_hashmap_entry *it = h->entries[i];
		بينما (it) {
			بنية ext2fs_hashmap_entry *tmp = it->next;
			إذا (h->حرر)
				h->حرر(it->data);
			حرر(it);
			it = tmp;
		}
	}
	حرر(h);
}
