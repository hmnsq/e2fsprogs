#অন্তর্ভাব "hashmap.h"
#অন্তর্ভাব <string.h>

কাঠামো ext2fs_hashmap {
	uint32_t আকার_2;
	uint32_t(*hash)(ধ্রুবক শূন্য *চাবি, size_t len);
	শূন্য(*মুক্ত)(শূন্য*);
	কাঠামো ext2fs_hashmap_entry *first;
	কাঠামো ext2fs_hashmap_entry *last;
#যদি __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic ঠেলো
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	কাঠামো ext2fs_hashmap_entry *entries[0];
#যদি __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic বের_করো
#endif
};

uint32_t ext2fs_djb2_hash(ধ্রুবক শূন্য *str, size_t আকার_2)
{
	পূর্ণসংখ্যা c;
	ধ্রুবক অক্ষর *s = str;
	uint32_t hash = 5381;

	যখন (আকার_2-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	ফেরত hash;
}

কাঠামো ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ধ্রুবক শূন্য*, size_t),
				শূন্য(*free_fct)(শূন্য*), size_t আকার_2)
{
	কাঠামো ext2fs_hashmap *h = calloc(1, আকার(কাঠামো ext2fs_hashmap) +
				আকার(কাঠামো ext2fs_hashmap_entry) * আকার_2);
	যদি (!h)
		ফেরত ফাঁকা;

	h->আকার_2 = আকার_2;
	h->মুক্ত = free_fct;
	h->hash = hash_fct;
	h->first = h->last = ফাঁকা;
	ফেরত h;
}

পূর্ণসংখ্যা ext2fs_hashmap_add(কাঠামো ext2fs_hashmap *h,
		       শূন্য *তথ্য, ধ্রুবক শূন্য *চাবি, size_t চাবি_len)
{
	uint32_t hash = h->hash(চাবি, চাবি_len) % h->আকার_2;
	কাঠামো ext2fs_hashmap_entry *e = malloc(আকার(*e));

	যদি (!e)
		ফেরত -1;

	e->তথ্য = তথ্য;
	e->চাবি = চাবি;
	e->চাবি_len = চাবি_len;
	e->পরবর্তী = h->entries[hash];
	h->entries[hash] = e;

	e->list_পূর্ব = ফাঁকা;
	e->list_পরবর্তী = h->first;
	যদি (h->first)
		h->first->list_পূর্ব = e;
	h->first = e;
	যদি (!h->last)
		h->last = e;

	ফেরত 0;
}

শূন্য *ext2fs_hashmap_lookup(কাঠামো ext2fs_hashmap *h, ধ্রুবক শূন্য *চাবি,
			    size_t চাবি_len)
{
	কাঠামো ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(চাবি, চাবি_len) % h->আকার_2;

	জন্য (iter = h->entries[hash]; iter; iter = iter->পরবর্তী)
		যদি (iter->চাবি_len == চাবি_len && !memcmp(iter->চাবি, চাবি, চাবি_len))
			ফেরত iter->তথ্য;
	ফেরত ফাঁকা;
}

শূন্য *ext2fs_hashmap_iter_in_order(কাঠামো ext2fs_hashmap *h,
				   কাঠামো ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_পরবর্তী : h->first;
	ফেরত *it ? (*it)->তথ্য : ফাঁকা;
}

শূন্য ext2fs_hashmap_free(কাঠামো ext2fs_hashmap *h)
{
	size_t	i;

	জন্য (i = 0; i < h->আকার_2; ++i) {
		কাঠামো ext2fs_hashmap_entry *it = h->entries[i];
		যখন (it) {
			কাঠামো ext2fs_hashmap_entry *tmp = it->পরবর্তী;
			যদি (h->মুক্ত)
				h->মুক্ত(it->তথ্য);
			মুক্ত(it);
			it = tmp;
		}
	}
	মুক্ত(h);
}
