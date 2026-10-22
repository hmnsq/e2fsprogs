#അന്തര്ഭാവ "hashmap.h"
#അന്തര്ഭാവ <string.h>

ഘടന ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(സ്ഥിരം ശൂന്യം *താക്കോൽ, size_t len);
	ശൂന്യം(*സ്വതന്ത്രം)(ശൂന്യം*);
	ഘടന ext2fs_hashmap_entry *first;
	ഘടന ext2fs_hashmap_entry *last;
#എങ്കിൽ __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic തള്ളുക
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	ഘടന ext2fs_hashmap_entry *entries[0];
#എങ്കിൽ __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic എടുക്കുക
#endif
};

uint32_t ext2fs_djb2_hash(സ്ഥിരം ശൂന്യം *str, size_t size)
{
	പൂർണ്ണസംഖ്യ c;
	സ്ഥിരം അക്ഷരം *s = str;
	uint32_t hash = 5381;

	ആകുമ്പോൾ (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	തിരികെ hash;
}

ഘടന ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(സ്ഥിരം ശൂന്യം*, size_t),
				ശൂന്യം(*free_fct)(ശൂന്യം*), size_t size)
{
	ഘടന ext2fs_hashmap *h = calloc(1, വലുപ്പം(ഘടന ext2fs_hashmap) +
				വലുപ്പം(ഘടന ext2fs_hashmap_entry) * size);
	എങ്കിൽ (!h)
		തിരികെ ശൂന്യ;

	h->size = size;
	h->സ്വതന്ത്രം = free_fct;
	h->hash = hash_fct;
	h->first = h->last = ശൂന്യ;
	തിരികെ h;
}

പൂർണ്ണസംഖ്യ ext2fs_hashmap_add(ഘടന ext2fs_hashmap *h,
		       ശൂന്യം *data, സ്ഥിരം ശൂന്യം *താക്കോൽ, size_t താക്കോൽ_len)
{
	uint32_t hash = h->hash(താക്കോൽ, താക്കോൽ_len) % h->size;
	ഘടന ext2fs_hashmap_entry *e = malloc(വലുപ്പം(*e));

	എങ്കിൽ (!e)
		തിരികെ -1;

	e->data = data;
	e->താക്കോൽ = താക്കോൽ;
	e->താക്കോൽ_len = താക്കോൽ_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = ശൂന്യ;
	e->list_next = h->first;
	എങ്കിൽ (h->first)
		h->first->list_prev = e;
	h->first = e;
	എങ്കിൽ (!h->last)
		h->last = e;

	തിരികെ 0;
}

ശൂന്യം *ext2fs_hashmap_lookup(ഘടന ext2fs_hashmap *h, സ്ഥിരം ശൂന്യം *താക്കോൽ,
			    size_t താക്കോൽ_len)
{
	ഘടന ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(താക്കോൽ, താക്കോൽ_len) % h->size;

	വേണ്ടി (iter = h->entries[hash]; iter; iter = iter->next)
		എങ്കിൽ (iter->താക്കോൽ_len == താക്കോൽ_len && !memcmp(iter->താക്കോൽ, താക്കോൽ, താക്കോൽ_len))
			തിരികെ iter->data;
	തിരികെ ശൂന്യ;
}

ശൂന്യം *ext2fs_hashmap_iter_in_order(ഘടന ext2fs_hashmap *h,
				   ഘടന ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	തിരികെ *it ? (*it)->data : ശൂന്യ;
}

ശൂന്യം ext2fs_hashmap_free(ഘടന ext2fs_hashmap *h)
{
	size_t	i;

	വേണ്ടി (i = 0; i < h->size; ++i) {
		ഘടന ext2fs_hashmap_entry *it = h->entries[i];
		ആകുമ്പോൾ (it) {
			ഘടന ext2fs_hashmap_entry *tmp = it->next;
			എങ്കിൽ (h->സ്വതന്ത്രം)
				h->സ്വതന്ത്രം(it->data);
			സ്വതന്ത്രം(it);
			it = tmp;
		}
	}
	സ്വതന്ത്രം(h);
}
