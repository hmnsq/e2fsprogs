#ಅನ್ತರ್ಭಾವ "hashmap.h"
#ಅನ್ತರ್ಭಾವ <string.h>

ಸಂರಚನಾ ext2fs_hashmap {
	uint32_t size;
	uint32_t(*hash)(ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t len);
	ಶೂನ್ಯ(*ಮುಕ್ತ)(ಶೂನ್ಯ*);
	ಸಂರಚನಾ ext2fs_hashmap_entry *first;
	ಸಂರಚನಾ ext2fs_hashmap_entry *last;
#ಯದಿ __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic ತಳ್ಳು
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	ಸಂರಚನಾ ext2fs_hashmap_entry *entries[0];
#ಯದಿ __GNUC_PREREQ (4, 8)
#pragma GCC diagnostic ತೆಗೆ
#endif
};

uint32_t ext2fs_djb2_hash(ಧ್ರುವ ಶೂನ್ಯ *str, size_t size)
{
	ಪೂರ್ಣಾಙ್ಕ c;
	ಧ್ರುವ ವರ್ಣ *s = str;
	uint32_t hash = 5381;

	ಯಾವತ್ (size-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	ಪ್ರತ್ಯಾಗಮ hash;
}

ಸಂರಚನಾ ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ಧ್ರುವ ಶೂನ್ಯ*, size_t),
				ಶೂನ್ಯ(*free_fct)(ಶೂನ್ಯ*), size_t size)
{
	ಸಂರಚನಾ ext2fs_hashmap *h = calloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2fs_hashmap) +
				ಪರಿಮಾಣ(ಸಂರಚನಾ ext2fs_hashmap_entry) * size);
	ಯದಿ (!h)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	h->size = size;
	h->ಮುಕ್ತ = free_fct;
	h->hash = hash_fct;
	h->first = h->last = ರಿಕ್ತ;
	ಪ್ರತ್ಯಾಗಮ h;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_hashmap_add(ಸಂರಚನಾ ext2fs_hashmap *h,
		       ಶೂನ್ಯ *data, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ, size_t ಕೀಲಿ_len)
{
	uint32_t hash = h->hash(ಕೀಲಿ, ಕೀಲಿ_len) % h->size;
	ಸಂರಚನಾ ext2fs_hashmap_entry *e = malloc(ಪರಿಮಾಣ(*e));

	ಯದಿ (!e)
		ಪ್ರತ್ಯಾಗಮ -1;

	e->data = data;
	e->ಕೀಲಿ = ಕೀಲಿ;
	e->ಕೀಲಿ_len = ಕೀಲಿ_len;
	e->next = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = ರಿಕ್ತ;
	e->list_next = h->first;
	ಯದಿ (h->first)
		h->first->list_prev = e;
	h->first = e;
	ಯದಿ (!h->last)
		h->last = e;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ *ext2fs_hashmap_lookup(ಸಂರಚನಾ ext2fs_hashmap *h, ಧ್ರುವ ಶೂನ್ಯ *ಕೀಲಿ,
			    size_t ಕೀಲಿ_len)
{
	ಸಂರಚನಾ ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(ಕೀಲಿ, ಕೀಲಿ_len) % h->size;

	ಪ್ರತಿ (iter = h->entries[hash]; iter; iter = iter->next)
		ಯದಿ (iter->ಕೀಲಿ_len == ಕೀಲಿ_len && !memcmp(iter->ಕೀಲಿ, ಕೀಲಿ, ಕೀಲಿ_len))
			ಪ್ರತ್ಯಾಗಮ iter->data;
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಶೂನ್ಯ *ext2fs_hashmap_iter_in_order(ಸಂರಚನಾ ext2fs_hashmap *h,
				   ಸಂರಚನಾ ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_next : h->first;
	ಪ್ರತ್ಯಾಗಮ *it ? (*it)->data : ರಿಕ್ತ;
}

ಶೂನ್ಯ ext2fs_hashmap_free(ಸಂರಚನಾ ext2fs_hashmap *h)
{
	size_t	i;

	ಪ್ರತಿ (i = 0; i < h->size; ++i) {
		ಸಂರಚನಾ ext2fs_hashmap_entry *it = h->entries[i];
		ಯಾವತ್ (it) {
			ಸಂರಚನಾ ext2fs_hashmap_entry *tmp = it->next;
			ಯದಿ (h->ಮುಕ್ತ)
				h->ಮುಕ್ತ(it->data);
			ಮುಕ್ತ(it);
			it = tmp;
		}
	}
	ಮುಕ್ತ(h);
}
