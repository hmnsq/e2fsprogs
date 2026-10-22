#شامل "hashmap.h"
#شامل <string.h>

ساختار ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(ثابت تهی *کلید, size_t len);
	تهی(*آزاد)(تهی*);
	ساختار ext2fs_hashmap_entry *first;
	ساختار ext2fs_hashmap_entry *last;
#اگر __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic افزودن
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	ساختار ext2fs_hashmap_entry *entries[0];
#اگر __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic برداشت
#endif
};

uint32_t ext2fs_djb2_hash(ثابت تهی *str, size_t size)
{
	صحیح c;
	ثابت نویسه *s = str;
	uint32_t hash = 5381;

	وقتی (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	بازگشت hash;
}

ساختار ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت تهی*, size_t),
				تهی(*free_fct)(تهی*), size_t size)
{
	ساختار ext2fs_hashmap *h = calloc(1, اندازه(ساختار ext2fs_hashmap) +
				اندازه(ساختار ext2fs_hashmap_entry) * size);
	اگر (!h)
		بازگشت پوچ;

	h->size = size;
	h->آزاد = free_fct;
	h->hash = hash_fct;
	h->first = h->last = پوچ;
	بازگشت h;
}

صحیح ext2fs_hashmap_add(ساختار ext2fs_hashmap *h,
		       تهی *data, ثابت تهی *کلید, size_t کلید_len)
{
	uint32_t hash = h->hash(کلید, کلید_len) % h->size;
	ساختار ext2fs_hashmap_entry *e = malloc(اندازه(*e));

	اگر (!e)
		بازگشت -1;

	e->data = data;
	e->کلید = کلید;
	e->کلید_len = کلید_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = پوچ;
	e->list_next = h->first;
	اگر (h->first)
		h->first->list_prev = e;
	h->first = e;
	اگر (!h->last)
		h->last = e;

	بازگشت 0;
}

تهی *ext2fs_hashmap_lookup(ساختار ext2fs_hashmap *h, ثابت تهی *کلید,
			    size_t کلید_len)
{
	ساختار ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(کلید, کلید_len) % h->size;

	برای (iter = h->entries[hash]; iter; iter = iter->next)
		اگر (iter->کلید_len == کلید_len && !memcmp(iter->کلید, کلید, کلید_len))
			بازگشت iter->data;
	بازگشت پوچ;
}

تهی *ext2fs_hashmap_iter_in_order(ساختار ext2fs_hashmap *h,
				   ساختار ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	بازگشت *it ? (*it)->data : پوچ;
}

تهی ext2fs_hashmap_free(ساختار ext2fs_hashmap *h)
{
	size_t	i;

	برای (i = 0; i < h->size; ++i) {
		ساختار ext2fs_hashmap_entry *it = h->entries[i];
		وقتی (it) {
			ساختار ext2fs_hashmap_entry *tmp = it->next;
			اگر (h->آزاد)
				h->آزاد(it->data);
			آزاد(it);
			it = tmp;
		}
	}
	آزاد(h);
}
