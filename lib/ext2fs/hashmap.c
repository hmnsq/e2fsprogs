#ضمن "hashmap.h"
#ضمن <string.h>

بنية ext2fs_hashmap {
	uint32_t ناپ;
	uint32_t(*hash)(ثابت فراغ *کلید, size_t len);
	فراغ(*حرر)(فراغ*);
	بنية ext2fs_hashmap_اندراج *پہلا;
	بنية ext2fs_hashmap_اندراج *آخری;
#إذا __GNUC_PREREQ (4, 8)
#ہدایت GCC diagnostic ڈالو
#ہدایت GCC diagnostic ignored "-Wpedantic"
#جے_انت
	بنية ext2fs_hashmap_اندراج *entries[0];
#إذا __GNUC_PREREQ (4, 8)
#ہدایت GCC diagnostic نکالو
#جے_انت
};

uint32_t ext2fs_djb2_hash(ثابت فراغ *str, size_t ناپ)
{
	صحيح c;
	ثابت حرف *s = str;
	uint32_t hash = 5381;

	طالما (ناپ-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	أرجع hash;
}

بنية ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت فراغ*, size_t),
				فراغ(*free_fct)(فراغ*), size_t ناپ)
{
	بنية ext2fs_hashmap *h = صفر_حافظہ_دیو(1, حجم(بنية ext2fs_hashmap) +
				حجم(بنية ext2fs_hashmap_اندراج) * ناپ);
	إذا (!h)
		أرجع عدم;

	h->ناپ = ناپ;
	h->حرر = free_fct;
	h->hash = hash_fct;
	h->پہلا = h->آخری = عدم;
	أرجع h;
}

صحيح ext2fs_hashmap_جوڑو(بنية ext2fs_hashmap *h,
		       فراغ *انکڑے, ثابت فراغ *کلید, size_t کلید_len)
{
	uint32_t hash = h->hash(کلید, کلید_len) % h->ناپ;
	بنية ext2fs_hashmap_اندراج *e = حافظہ_دیو(حجم(*e));

	إذا (!e)
		أرجع -1;

	e->انکڑے = انکڑے;
	e->کلید = کلید;
	e->کلید_len = کلید_len;
	e->اگلا = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = عدم;
	e->list_اگلا = h->پہلا;
	إذا (h->پہلا)
		h->پہلا->list_prev = e;
	h->پہلا = e;
	إذا (!h->آخری)
		h->آخری = e;

	أرجع 0;
}

فراغ *ext2fs_hashmap_lookup(بنية ext2fs_hashmap *h, ثابت فراغ *کلید,
			    size_t کلید_len)
{
	بنية ext2fs_hashmap_اندراج *iter;
	uint32_t hash = h->hash(کلید, کلید_len) % h->ناپ;

	لكل (iter = h->entries[hash]; iter; iter = iter->اگلا)
		إذا (iter->کلید_len == کلید_len && !memcmp(iter->کلید, کلید, کلید_len))
			أرجع iter->انکڑے;
	أرجع عدم;
}

فراغ *ext2fs_hashmap_iter_وچ_ترتیب(بنية ext2fs_hashmap *h,
				   بنية ext2fs_hashmap_اندراج **it)
{
	*it = *it ? (*it)->list_اگلا : h->پہلا;
	أرجع *it ? (*it)->انکڑے : عدم;
}

فراغ ext2fs_hashmap_free(بنية ext2fs_hashmap *h)
{
	size_t	i;

	لكل (i = 0; i < h->ناپ; ++i) {
		بنية ext2fs_hashmap_اندراج *it = h->entries[i];
		طالما (it) {
			بنية ext2fs_hashmap_اندراج *tmp = it->اگلا;
			إذا (h->حرر)
				h->حرر(it->انکڑے);
			حرر(it);
			it = tmp;
		}
	}
	حرر(h);
}
