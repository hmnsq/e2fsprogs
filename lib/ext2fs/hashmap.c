#ਅਨ੍ਤਰ੍ਭਾਵ "hashmap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

ਬਣਤਰ ext2fs_hashmap {
	uint32_t ਮਾਪ;
	uint32_t(*hash)(ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t len);
	ਖਾਲੀ(*ਮੁਕਤ)(ਖਾਲੀ*);
	ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *ਪਹਿਲਾ;
	ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *ਆਖਰੀ;
#ਜੇ __GNUC_PREREQ (4, 8)
#ਹਦਾਇਤ GCC diagnostic ਧੱਕੋ
#ਹਦਾਇਤ GCC diagnostic ignored "-Wpedantic"
#ਜੇ_ਅੰਤ
	ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *entries[0];
#ਜੇ __GNUC_PREREQ (4, 8)
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
#ਜੇ_ਅੰਤ
};

uint32_t ext2fs_djb2_hash(ਅਚਲ ਖਾਲੀ *str, size_t ਮਾਪ)
{
	ਪੂਰਨ c;
	ਅਚਲ ਅੱਖਰ *s = str;
	uint32_t hash = 5381;

	ਜਦੋਂ (ਮਾਪ-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	ਵਾਪਸ hash;
}

ਬਣਤਰ ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ਅਚਲ ਖਾਲੀ*, size_t),
				ਖਾਲੀ(*free_fct)(ਖਾਲੀ*), size_t ਮਾਪ)
{
	ਬਣਤਰ ext2fs_hashmap *h = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਬਣਤਰ ext2fs_hashmap) +
				ਆਕਾਰ(ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ) * ਮਾਪ);
	ਜੇ (!h)
		ਵਾਪਸ ਨਿਰਾ;

	h->ਮਾਪ = ਮਾਪ;
	h->ਮੁਕਤ = free_fct;
	h->hash = hash_fct;
	h->ਪਹਿਲਾ = h->ਆਖਰੀ = ਨਿਰਾ;
	ਵਾਪਸ h;
}

ਪੂਰਨ ext2fs_hashmap_ਜੋੜੋ(ਬਣਤਰ ext2fs_hashmap *h,
		       ਖਾਲੀ *ਅੰਕੜੇ, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ, size_t ਕੁੰਜੀ_len)
{
	uint32_t hash = h->hash(ਕੁੰਜੀ, ਕੁੰਜੀ_len) % h->ਮਾਪ;
	ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *e = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*e));

	ਜੇ (!e)
		ਵਾਪਸ -1;

	e->ਅੰਕੜੇ = ਅੰਕੜੇ;
	e->ਕੁੰਜੀ = ਕੁੰਜੀ;
	e->ਕੁੰਜੀ_len = ਕੁੰਜੀ_len;
	e->ਅਗਲਾ = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = ਨਿਰਾ;
	e->list_ਅਗਲਾ = h->ਪਹਿਲਾ;
	ਜੇ (h->ਪਹਿਲਾ)
		h->ਪਹਿਲਾ->list_prev = e;
	h->ਪਹਿਲਾ = e;
	ਜੇ (!h->ਆਖਰੀ)
		h->ਆਖਰੀ = e;

	ਵਾਪਸ 0;
}

ਖਾਲੀ *ext2fs_hashmap_lookup(ਬਣਤਰ ext2fs_hashmap *h, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ,
			    size_t ਕੁੰਜੀ_len)
{
	ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *iter;
	uint32_t hash = h->hash(ਕੁੰਜੀ, ਕੁੰਜੀ_len) % h->ਮਾਪ;

	ਲਈ (iter = h->entries[hash]; iter; iter = iter->ਅਗਲਾ)
		ਜੇ (iter->ਕੁੰਜੀ_len == ਕੁੰਜੀ_len && !memcmp(iter->ਕੁੰਜੀ, ਕੁੰਜੀ, ਕੁੰਜੀ_len))
			ਵਾਪਸ iter->ਅੰਕੜੇ;
	ਵਾਪਸ ਨਿਰਾ;
}

ਖਾਲੀ *ext2fs_hashmap_iter_ਵਿੱਚ_ਕ੍ਰਮ(ਬਣਤਰ ext2fs_hashmap *h,
				   ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ **it)
{
	*it = *it ? (*it)->list_ਅਗਲਾ : h->ਪਹਿਲਾ;
	ਵਾਪਸ *it ? (*it)->ਅੰਕੜੇ : ਨਿਰਾ;
}

ਖਾਲੀ ext2fs_hashmap_free(ਬਣਤਰ ext2fs_hashmap *h)
{
	size_t	i;

	ਲਈ (i = 0; i < h->ਮਾਪ; ++i) {
		ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *it = h->entries[i];
		ਜਦੋਂ (it) {
			ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *tmp = it->ਅਗਲਾ;
			ਜੇ (h->ਮੁਕਤ)
				h->ਮੁਕਤ(it->ਅੰਕੜੇ);
			ਮੁਕਤ(it);
			it = tmp;
		}
	}
	ਮੁਕਤ(h);
}
