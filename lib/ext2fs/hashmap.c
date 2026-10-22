#شامل "hashmap.h"
#شامل <string.h>

ساخت ext2fs_hashmap {
	uint32_t سائز;
	uint32_t(*hash)(ثابت خالی *کلید, size_t len);
	خالی(*آزاد)(خالی*);
	ساخت ext2fs_hashmap_اندراج *پہلا;
	ساخت ext2fs_hashmap_اندراج *آخری;
#اگر __GNUC_PREREQ (4, 8)
#ہدایت GCC diagnostic ڈالو
#ہدایت GCC diagnostic ignored "-Wpedantic"
#اگر_ختم
	ساخت ext2fs_hashmap_اندراج *entries[0];
#اگر __GNUC_PREREQ (4, 8)
#ہدایت GCC diagnostic نکالو
#اگر_ختم
};

uint32_t ext2fs_djb2_hash(ثابت خالی *str, size_t سائز)
{
	صحیح c;
	ثابت حرف *s = str;
	uint32_t hash = 5381;

	جب (سائز-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	واپس hash;
}

ساخت ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت خالی*, size_t),
				خالی(*free_fct)(خالی*), size_t سائز)
{
	ساخت ext2fs_hashmap *h = صفر_حافظہ_دو(1, حجم(ساخت ext2fs_hashmap) +
				حجم(ساخت ext2fs_hashmap_اندراج) * سائز);
	اگر (!h)
		واپس تہی;

	h->سائز = سائز;
	h->آزاد = free_fct;
	h->hash = hash_fct;
	h->پہلا = h->آخری = تہی;
	واپس h;
}

صحیح ext2fs_hashmap_جوڑو(ساخت ext2fs_hashmap *h,
		       خالی *ڈیٹا, ثابت خالی *کلید, size_t کلید_len)
{
	uint32_t hash = h->hash(کلید, کلید_len) % h->سائز;
	ساخت ext2fs_hashmap_اندراج *e = حافظہ_دو(حجم(*e));

	اگر (!e)
		واپس -1;

	e->ڈیٹا = ڈیٹا;
	e->کلید = کلید;
	e->کلید_len = کلید_len;
	e->اگلا = h->entries[hash];
	h->entries[hash] = e;

	e->list_پچھلا = تہی;
	e->list_اگلا = h->پہلا;
	اگر (h->پہلا)
		h->پہلا->list_پچھلا = e;
	h->پہلا = e;
	اگر (!h->آخری)
		h->آخری = e;

	واپس 0;
}

خالی *ext2fs_hashmap_lookup(ساخت ext2fs_hashmap *h, ثابت خالی *کلید,
			    size_t کلید_len)
{
	ساخت ext2fs_hashmap_اندراج *iter;
	uint32_t hash = h->hash(کلید, کلید_len) % h->سائز;

	برائے (iter = h->entries[hash]; iter; iter = iter->اگلا)
		اگر (iter->کلید_len == کلید_len && !memcmp(iter->کلید, کلید, کلید_len))
			واپس iter->ڈیٹا;
	واپس تہی;
}

خالی *ext2fs_hashmap_iter_اندر_ترتیب(ساخت ext2fs_hashmap *h,
				   ساخت ext2fs_hashmap_اندراج **it)
{
	*it = *it ? (*it)->list_اگلا : h->پہلا;
	واپس *it ? (*it)->ڈیٹا : تہی;
}

خالی ext2fs_hashmap_free(ساخت ext2fs_hashmap *h)
{
	size_t	i;

	برائے (i = 0; i < h->سائز; ++i) {
		ساخت ext2fs_hashmap_اندراج *it = h->entries[i];
		جب (it) {
			ساخت ext2fs_hashmap_اندراج *tmp = it->اگلا;
			اگر (h->آزاد)
				h->آزاد(it->ڈیٹا);
			آزاد(it);
			it = tmp;
		}
	}
	آزاد(h);
}
