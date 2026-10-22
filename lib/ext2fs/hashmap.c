#અન્તર્ભાવ "hashmap.h"
#અન્તર્ભાવ <string.h>

સંરચના ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(અચલ શૂન્ય *ચાવી, size_t len);
	શૂન્ય(*મુક્ત)(શૂન્ય*);
	સંરચના ext2fs_hashmap_entry *first;
	સંરચના ext2fs_hashmap_entry *last;
#જો __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic ધકેલો
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	સંરચના ext2fs_hashmap_entry *entries[0];
#જો __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic કાઢો
#endif
};

uint32_t ext2fs_djb2_hash(અચલ શૂન્ય *str, size_t size)
{
	પૂર્ણાંક c;
	અચલ અક્ષર *s = str;
	uint32_t hash = 5381;

	જ્યારે (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	પાછા hash;
}

સંરચના ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(અચલ શૂન્ય*, size_t),
				શૂન્ય(*free_fct)(શૂન્ય*), size_t size)
{
	સંરચના ext2fs_hashmap *h = calloc(1, કદ(સંરચના ext2fs_hashmap) +
				કદ(સંરચના ext2fs_hashmap_entry) * size);
	જો (!h)
		પાછા ખાલી;

	h->size = size;
	h->મુક્ત = free_fct;
	h->hash = hash_fct;
	h->first = h->last = ખાલી;
	પાછા h;
}

પૂર્ણાંક ext2fs_hashmap_add(સંરચના ext2fs_hashmap *h,
		       શૂન્ય *data, અચલ શૂન્ય *ચાવી, size_t ચાવી_len)
{
	uint32_t hash = h->hash(ચાવી, ચાવી_len) % h->size;
	સંરચના ext2fs_hashmap_entry *e = malloc(કદ(*e));

	જો (!e)
		પાછા -1;

	e->data = data;
	e->ચાવી = ચાવી;
	e->ચાવી_len = ચાવી_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = ખાલી;
	e->list_next = h->first;
	જો (h->first)
		h->first->list_prev = e;
	h->first = e;
	જો (!h->last)
		h->last = e;

	પાછા 0;
}

શૂન્ય *ext2fs_hashmap_lookup(સંરચના ext2fs_hashmap *h, અચલ શૂન્ય *ચાવી,
			    size_t ચાવી_len)
{
	સંરચના ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(ચાવી, ચાવી_len) % h->size;

	માટે (iter = h->entries[hash]; iter; iter = iter->next)
		જો (iter->ચાવી_len == ચાવી_len && !memcmp(iter->ચાવી, ચાવી, ચાવી_len))
			પાછા iter->data;
	પાછા ખાલી;
}

શૂન્ય *ext2fs_hashmap_iter_in_order(સંરચના ext2fs_hashmap *h,
				   સંરચના ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	પાછા *it ? (*it)->data : ખાલી;
}

શૂન્ય ext2fs_hashmap_free(સંરચના ext2fs_hashmap *h)
{
	size_t	i;

	માટે (i = 0; i < h->size; ++i) {
		સંરચના ext2fs_hashmap_entry *it = h->entries[i];
		જ્યારે (it) {
			સંરચના ext2fs_hashmap_entry *tmp = it->next;
			જો (h->મુક્ત)
				h->મુક્ત(it->data);
			મુક્ત(it);
			it = tmp;
		}
	}
	મુક્ત(h);
}
