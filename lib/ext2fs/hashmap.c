#অন্তর্ভাৱ "hashmap.h"
#অন্তর্ভাৱ <string.h>

সংরচনা ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(ধ্রুৱ শূন্য *চাবি, size_t len);
	শূন্য(*মুক্ত)(শূন্য*);
	সংরচনা ext2fs_hashmap_entry *first;
	সংরচনা ext2fs_hashmap_entry *last;
#যদি __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic ঠেলো
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	সংরচনা ext2fs_hashmap_entry *entries[0];
#যদি __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic বেৰ_কৰো
#endif
};

uint32_t ext2fs_djb2_hash(ধ্রুৱ শূন্য *str, size_t size)
{
	পূর্ণাঙ্ক c;
	ধ্রুৱ ৱর্ণ *s = str;
	uint32_t hash = 5381;

	যেতিয়ালৈ (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	ঘূৰাই hash;
}

সংরচনা ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ধ্রুৱ শূন্য*, size_t),
				শূন্য(*free_fct)(শূন্য*), size_t size)
{
	সংরচনা ext2fs_hashmap *h = calloc(1, পরিমাণ(সংরচনা ext2fs_hashmap) +
				পরিমাণ(সংরচনা ext2fs_hashmap_entry) * size);
	যদি (!h)
		ঘূৰাই রিক্ত;

	h->size = size;
	h->মুক্ত = free_fct;
	h->hash = hash_fct;
	h->first = h->last = রিক্ত;
	ঘূৰাই h;
}

পূর্ণাঙ্ক ext2fs_hashmap_add(সংরচনা ext2fs_hashmap *h,
		       শূন্য *data, ধ্রুৱ শূন্য *চাবি, size_t চাবি_len)
{
	uint32_t hash = h->hash(চাবি, চাবি_len) % h->size;
	সংরচনা ext2fs_hashmap_entry *e = malloc(পরিমাণ(*e));

	যদি (!e)
		ঘূৰাই -1;

	e->data = data;
	e->চাবি = চাবি;
	e->চাবি_len = চাবি_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = রিক্ত;
	e->list_next = h->first;
	যদি (h->first)
		h->first->list_prev = e;
	h->first = e;
	যদি (!h->last)
		h->last = e;

	ঘূৰাই 0;
}

শূন্য *ext2fs_hashmap_lookup(সংরচনা ext2fs_hashmap *h, ধ্রুৱ শূন্য *চাবি,
			    size_t চাবি_len)
{
	সংরচনা ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(চাবি, চাবি_len) % h->size;

	বাবে (iter = h->entries[hash]; iter; iter = iter->next)
		যদি (iter->চাবি_len == চাবি_len && !memcmp(iter->চাবি, চাবি, চাবি_len))
			ঘূৰাই iter->data;
	ঘূৰাই রিক্ত;
}

শূন্য *ext2fs_hashmap_iter_in_order(সংরচনা ext2fs_hashmap *h,
				   সংরচনা ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	ঘূৰাই *it ? (*it)->data : রিক্ত;
}

শূন্য ext2fs_hashmap_free(সংরচনা ext2fs_hashmap *h)
{
	size_t	i;

	বাবে (i = 0; i < h->size; ++i) {
		সংরচনা ext2fs_hashmap_entry *it = h->entries[i];
		যেতিয়ালৈ (it) {
			সংরচনা ext2fs_hashmap_entry *tmp = it->next;
			যদি (h->মুক্ত)
				h->মুক্ত(it->data);
			মুক্ত(it);
			it = tmp;
		}
	}
	মুক্ত(h);
}
