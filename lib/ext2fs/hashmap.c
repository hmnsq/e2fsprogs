#ଅନ୍ତର୍ଭାଵ "hashmap.h"
#ଅନ୍ତର୍ଭାଵ <string.h>

ସଂରଚନା ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(ଅଚଳ ଶୂନ୍ୟ *ଚାବି, size_t len);
	ଶୂନ୍ୟ(*ମୁକ୍ତ)(ଶୂନ୍ୟ*);
	ସଂରଚନା ext2fs_hashmap_entry *first;
	ସଂରଚନା ext2fs_hashmap_entry *last;
#ଯଦି __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic ଠେଲ
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	ସଂରଚନା ext2fs_hashmap_entry *entries[0];
#ଯଦି __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic କାଢ଼
#endif
};

uint32_t ext2fs_djb2_hash(ଅଚଳ ଶୂନ୍ୟ *str, size_t size)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
	ଅଚଳ ଅକ୍ଷର *s = str;
	uint32_t hash = 5381;

	ଯେତେବେଳେ (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	ଫେରନ୍ତୁ hash;
}

ସଂରଚନା ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ଅଚଳ ଶୂନ୍ୟ*, size_t),
				ଶୂନ୍ୟ(*free_fct)(ଶୂନ୍ୟ*), size_t size)
{
	ସଂରଚନା ext2fs_hashmap *h = calloc(1, ଆକାର(ସଂରଚନା ext2fs_hashmap) +
				ଆକାର(ସଂରଚନା ext2fs_hashmap_entry) * size);
	ଯଦି (!h)
		ଫେରନ୍ତୁ ଖାଲି;

	h->size = size;
	h->ମୁକ୍ତ = free_fct;
	h->hash = hash_fct;
	h->first = h->last = ଖାଲି;
	ଫେରନ୍ତୁ h;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_hashmap_add(ସଂରଚନା ext2fs_hashmap *h,
		       ଶୂନ୍ୟ *data, ଅଚଳ ଶୂନ୍ୟ *ଚାବି, size_t ଚାବି_len)
{
	uint32_t hash = h->hash(ଚାବି, ଚାବି_len) % h->size;
	ସଂରଚନା ext2fs_hashmap_entry *e = malloc(ଆକାର(*e));

	ଯଦି (!e)
		ଫେରନ୍ତୁ -1;

	e->data = data;
	e->ଚାବି = ଚାବି;
	e->ଚାବି_len = ଚାବି_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = ଖାଲି;
	e->list_next = h->first;
	ଯଦି (h->first)
		h->first->list_prev = e;
	h->first = e;
	ଯଦି (!h->last)
		h->last = e;

	ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ *ext2fs_hashmap_lookup(ସଂରଚନା ext2fs_hashmap *h, ଅଚଳ ଶୂନ୍ୟ *ଚାବି,
			    size_t ଚାବି_len)
{
	ସଂରଚନା ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(ଚାବି, ଚାବି_len) % h->size;

	ପାଇଁ (iter = h->entries[hash]; iter; iter = iter->next)
		ଯଦି (iter->ଚାବି_len == ଚାବି_len && !memcmp(iter->ଚାବି, ଚାବି, ଚାବି_len))
			ଫେରନ୍ତୁ iter->data;
	ଫେରନ୍ତୁ ଖାଲି;
}

ଶୂନ୍ୟ *ext2fs_hashmap_iter_in_order(ସଂରଚନା ext2fs_hashmap *h,
				   ସଂରଚନା ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	ଫେରନ୍ତୁ *it ? (*it)->data : ଖାଲି;
}

ଶୂନ୍ୟ ext2fs_hashmap_free(ସଂରଚନା ext2fs_hashmap *h)
{
	size_t	i;

	ପାଇଁ (i = 0; i < h->size; ++i) {
		ସଂରଚନା ext2fs_hashmap_entry *it = h->entries[i];
		ଯେତେବେଳେ (it) {
			ସଂରଚନା ext2fs_hashmap_entry *tmp = it->next;
			ଯଦି (h->ମୁକ୍ତ)
				h->ମୁକ୍ତ(it->data);
			ମୁକ୍ତ(it);
			it = tmp;
		}
	}
	ମୁକ୍ତ(h);
}
