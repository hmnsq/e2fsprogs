#ضمن "hashmap.h"
#ضمن <string.h>

بنية ext2fs_hashmap {
	uint32_t سائيز;
	uint32_t(*hash)(ثابت فراغ *ڪنجي, size_t len);
	فراغ(*حرر)(فراغ*);
	بنية ext2fs_hashmap_داخلا *پهريون;
	بنية ext2fs_hashmap_داخلا *آخري;
#إذا __GNUC_PREREQ (4, 8)
#هدايت GCC diagnostic ڌڪو
#هدايت GCC diagnostic ignored "-Wpedantic"
#جيڪڏهن_پورو
	بنية ext2fs_hashmap_داخلا *entries[0];
#إذا __GNUC_PREREQ (4, 8)
#هدايت GCC diagnostic ڪڍو
#جيڪڏهن_پورو
};

uint32_t ext2fs_djb2_hash(ثابت فراغ *str, size_t سائيز)
{
	صحيح c;
	ثابت حرف *s = str;
	uint32_t hash = 5381;

	طالما (سائيز-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	أرجع hash;
}

بنية ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت فراغ*, size_t),
				فراغ(*free_fct)(فراغ*), size_t سائيز)
{
	بنية ext2fs_hashmap *h = صفر_يادگيري_ڏيو(1, حجم(بنية ext2fs_hashmap) +
				حجم(بنية ext2fs_hashmap_داخلا) * سائيز);
	إذا (!h)
		أرجع عدم;

	h->سائيز = سائيز;
	h->حرر = free_fct;
	h->hash = hash_fct;
	h->پهريون = h->آخري = عدم;
	أرجع h;
}

صحيح ext2fs_hashmap_جوڙيو(بنية ext2fs_hashmap *h,
		       فراغ *معلومات, ثابت فراغ *ڪنجي, size_t ڪنجي_len)
{
	uint32_t hash = h->hash(ڪنجي, ڪنجي_len) % h->سائيز;
	بنية ext2fs_hashmap_داخلا *e = يادگيري_ڏيو(حجم(*e));

	إذا (!e)
		أرجع -1;

	e->معلومات = معلومات;
	e->ڪنجي = ڪنجي;
	e->ڪنجي_len = ڪنجي_len;
	e->ايندڙ = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = عدم;
	e->list_ايندڙ = h->پهريون;
	إذا (h->پهريون)
		h->پهريون->list_prev = e;
	h->پهريون = e;
	إذا (!h->آخري)
		h->آخري = e;

	أرجع 0;
}

فراغ *ext2fs_hashmap_lookup(بنية ext2fs_hashmap *h, ثابت فراغ *ڪنجي,
			    size_t ڪنجي_len)
{
	بنية ext2fs_hashmap_داخلا *iter;
	uint32_t hash = h->hash(ڪنجي, ڪنجي_len) % h->سائيز;

	لكل (iter = h->entries[hash]; iter; iter = iter->ايندڙ)
		إذا (iter->ڪنجي_len == ڪنجي_len && !memcmp(iter->ڪنجي, ڪنجي, ڪنجي_len))
			أرجع iter->معلومات;
	أرجع عدم;
}

فراغ *ext2fs_hashmap_iter_اندريون_ترتيب(بنية ext2fs_hashmap *h,
				   بنية ext2fs_hashmap_داخلا **it)
{
	*it = *it ? (*it)->list_ايندڙ : h->پهريون;
	أرجع *it ? (*it)->معلومات : عدم;
}

فراغ ext2fs_hashmap_free(بنية ext2fs_hashmap *h)
{
	size_t	i;

	لكل (i = 0; i < h->سائيز; ++i) {
		بنية ext2fs_hashmap_داخلا *it = h->entries[i];
		طالما (it) {
			بنية ext2fs_hashmap_داخلا *tmp = it->ايندڙ;
			إذا (h->حرر)
				h->حرر(it->معلومات);
			حرر(it);
			it = tmp;
		}
	}
	حرر(h);
}
