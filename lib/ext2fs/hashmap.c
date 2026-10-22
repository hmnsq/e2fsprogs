#ಅನ್ತರ್ಭಾವ "hashmap.h"
#ಅನ್ತರ್ಭಾವ <string.h>

ರಚನೆ ext2fs_hashmap {
	uint32_t ಗಾತ್ರ_2;
	uint32_t(*hash)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t len);
	ಶೂನ್ಯ(*ಬಿಡುಗಡೆ)(ಶೂನ್ಯ*);
	ರಚನೆ ext2fs_hashmap_ನಮೂದು *ಮೊದಲ;
	ರಚನೆ ext2fs_hashmap_ನಮೂದು *ಕೊನೆಯ;
#ಆದರೆ __GNUC_PREREQ (4, 8)
#ಸೂಚನೆ GCC diagnostic ತಳ್ಳು
#ಸೂಚನೆ GCC diagnostic ignored "-Wpedantic"
#ಆದರೆ_ಅಂತ್ಯ
	ರಚನೆ ext2fs_hashmap_ನಮೂದು *entries[0];
#ಆದರೆ __GNUC_PREREQ (4, 8)
#ಸೂಚನೆ GCC diagnostic ತೆಗೆ
#ಆದರೆ_ಅಂತ್ಯ
};

uint32_t ext2fs_djb2_hash(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *str, size_t ಗಾತ್ರ_2)
{
	ಪೂರ್ಣಾಂಕ c;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s = str;
	uint32_t hash = 5381;

	ಆಗ (ಗಾತ್ರ_2-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	ಹಿಂತಿರುಗಿ hash;
}

ರಚನೆ ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t),
				ಶೂನ್ಯ(*free_fct)(ಶೂನ್ಯ*), size_t ಗಾತ್ರ_2)
{
	ರಚನೆ ext2fs_hashmap *h = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ರಚನೆ ext2fs_hashmap) +
				ಗಾತ್ರ(ರಚನೆ ext2fs_hashmap_ನಮೂದು) * ಗಾತ್ರ_2);
	ಆದರೆ (!h)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	h->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
	h->ಬಿಡುಗಡೆ = free_fct;
	h->hash = hash_fct;
	h->ಮೊದಲ = h->ಕೊನೆಯ = ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ h;
}

ಪೂರ್ಣಾಂಕ ext2fs_hashmap_ಕೂಡಿಸು(ರಚನೆ ext2fs_hashmap *h,
		       ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ, size_t ಕೀಲಿ_len)
{
	uint32_t hash = h->hash(ಕೀಲಿ, ಕೀಲಿ_len) % h->ಗಾತ್ರ_2;
	ರಚನೆ ext2fs_hashmap_ನಮೂದು *e = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*e));

	ಆದರೆ (!e)
		ಹಿಂತಿರುಗಿ -1;

	e->ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
	e->ಕೀಲಿ = ಕೀಲಿ;
	e->ಕೀಲಿ_len = ಕೀಲಿ_len;
	e->ಮುಂದಿನ = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = ಖಾಲಿ;
	e->list_ಮುಂದಿನ = h->ಮೊದಲ;
	ಆದರೆ (h->ಮೊದಲ)
		h->ಮೊದಲ->list_prev = e;
	h->ಮೊದಲ = e;
	ಆದರೆ (!h->ಕೊನೆಯ)
		h->ಕೊನೆಯ = e;

	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ *ext2fs_hashmap_lookup(ರಚನೆ ext2fs_hashmap *h, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಕೀಲಿ,
			    size_t ಕೀಲಿ_len)
{
	ರಚನೆ ext2fs_hashmap_ನಮೂದು *iter;
	uint32_t hash = h->hash(ಕೀಲಿ, ಕೀಲಿ_len) % h->ಗಾತ್ರ_2;

	ಗಾಗಿ (iter = h->entries[hash]; iter; iter = iter->ಮುಂದಿನ)
		ಆದರೆ (iter->ಕೀಲಿ_len == ಕೀಲಿ_len && !memcmp(iter->ಕೀಲಿ, ಕೀಲಿ, ಕೀಲಿ_len))
			ಹಿಂತಿರುಗಿ iter->ದತ್ತಾಂಶ;
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಶೂನ್ಯ *ext2fs_hashmap_iter_ಒಳ_ಕ್ರಮ(ರಚನೆ ext2fs_hashmap *h,
				   ರಚನೆ ext2fs_hashmap_ನಮೂದು **it)
{
	*it = *it ? (*it)->list_ಮುಂದಿನ : h->ಮೊದಲ;
	ಹಿಂತಿರುಗಿ *it ? (*it)->ದತ್ತಾಂಶ : ಖಾಲಿ;
}

ಶೂನ್ಯ ext2fs_hashmap_free(ರಚನೆ ext2fs_hashmap *h)
{
	size_t	i;

	ಗಾಗಿ (i = 0; i < h->ಗಾತ್ರ_2; ++i) {
		ರಚನೆ ext2fs_hashmap_ನಮೂದು *it = h->entries[i];
		ಆಗ (it) {
			ರಚನೆ ext2fs_hashmap_ನಮೂದು *tmp = it->ಮುಂದಿನ;
			ಆದರೆ (h->ಬಿಡುಗಡೆ)
				h->ಬಿಡುಗಡೆ(it->ದತ್ತಾಂಶ);
			ಬಿಡುಗಡೆ(it);
			it = tmp;
		}
	}
	ಬಿಡುಗಡೆ(h);
}
