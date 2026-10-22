#அந்தர்பாவ "hashmap.h"
#அந்தர்பாவ <string.h>

கட்டமைப்பு ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(மாறிலி வெறுமை *சாவி, size_t len);
	வெறுமை(*விடுவி)(வெறுமை*);
	கட்டமைப்பு ext2fs_hashmap_entry *first;
	கட்டமைப்பு ext2fs_hashmap_entry *last;
#எனில் __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic தள்ளு
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	கட்டமைப்பு ext2fs_hashmap_entry *entries[0];
#எனில் __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic எடு
#endif
};

uint32_t ext2fs_djb2_hash(மாறிலி வெறுமை *str, size_t size)
{
	முழு c;
	மாறிலி எழுத்து *s = str;
	uint32_t hash = 5381;

	போது (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	திரும்பு hash;
}

கட்டமைப்பு ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(மாறிலி வெறுமை*, size_t),
				வெறுமை(*free_fct)(வெறுமை*), size_t size)
{
	கட்டமைப்பு ext2fs_hashmap *h = calloc(1, அளவு(கட்டமைப்பு ext2fs_hashmap) +
				அளவு(கட்டமைப்பு ext2fs_hashmap_entry) * size);
	எனில் (!h)
		திரும்பு வெற்று;

	h->size = size;
	h->விடுவி = free_fct;
	h->hash = hash_fct;
	h->first = h->last = வெற்று;
	திரும்பு h;
}

முழு ext2fs_hashmap_add(கட்டமைப்பு ext2fs_hashmap *h,
		       வெறுமை *data, மாறிலி வெறுமை *சாவி, size_t சாவி_len)
{
	uint32_t hash = h->hash(சாவி, சாவி_len) % h->size;
	கட்டமைப்பு ext2fs_hashmap_entry *e = malloc(அளவு(*e));

	எனில் (!e)
		திரும்பு -1;

	e->data = data;
	e->சாவி = சாவி;
	e->சாவி_len = சாவி_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = வெற்று;
	e->list_next = h->first;
	எனில் (h->first)
		h->first->list_prev = e;
	h->first = e;
	எனில் (!h->last)
		h->last = e;

	திரும்பு 0;
}

வெறுமை *ext2fs_hashmap_lookup(கட்டமைப்பு ext2fs_hashmap *h, மாறிலி வெறுமை *சாவி,
			    size_t சாவி_len)
{
	கட்டமைப்பு ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(சாவி, சாவி_len) % h->size;

	க்கு (iter = h->entries[hash]; iter; iter = iter->next)
		எனில் (iter->சாவி_len == சாவி_len && !memcmp(iter->சாவி, சாவி, சாவி_len))
			திரும்பு iter->data;
	திரும்பு வெற்று;
}

வெறுமை *ext2fs_hashmap_iter_in_order(கட்டமைப்பு ext2fs_hashmap *h,
				   கட்டமைப்பு ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	திரும்பு *it ? (*it)->data : வெற்று;
}

வெறுமை ext2fs_hashmap_free(கட்டமைப்பு ext2fs_hashmap *h)
{
	size_t	i;

	க்கு (i = 0; i < h->size; ++i) {
		கட்டமைப்பு ext2fs_hashmap_entry *it = h->entries[i];
		போது (it) {
			கட்டமைப்பு ext2fs_hashmap_entry *tmp = it->next;
			எனில் (h->விடுவி)
				h->விடுவி(it->data);
			விடுவி(it);
			it = tmp;
		}
	}
	விடுவி(h);
}
