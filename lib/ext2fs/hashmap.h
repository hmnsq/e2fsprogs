#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __GNUC_PREREQ
#ਜੇ defined(__GNUC__) && defined(__GNUC_MINOR__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __GNUC_PREREQ(maj, min) 0
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਬਣਤਰ ext2fs_hashmap;

ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ {
	ਖਾਲੀ *ਅੰਕੜੇ;
	ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ;
	size_t ਕੁੰਜੀ_len;
	ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *ਅਗਲਾ;
	ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *list_ਅਗਲਾ;
	ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *list_prev;
};

ਬਣਤਰ ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ਅਚਲ ਖਾਲੀ*, size_t),
				ਖਾਲੀ(*free_fct)(ਖਾਲੀ*), size_t ਮਾਪ);
ਪੂਰਨ ext2fs_hashmap_ਜੋੜੋ(ਬਣਤਰ ext2fs_hashmap *h,
		       ਖਾਲੀ *ਅੰਕੜੇ, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ,size_t ਕੁੰਜੀ_len);
ਖਾਲੀ *ext2fs_hashmap_lookup(ਬਣਤਰ ext2fs_hashmap *h, ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ,
			    size_t ਕੁੰਜੀ_len);
ਖਾਲੀ *ext2fs_hashmap_iter_ਵਿੱਚ_ਕ੍ਰਮ(ਬਣਤਰ ext2fs_hashmap *h,
				   ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ **it);
ਖਾਲੀ ext2fs_hashmap_del(ਬਣਤਰ ext2fs_hashmap *h,
			ਬਣਤਰ ext2fs_hashmap_ਇੰਦਰਾਜ *e);
ਖਾਲੀ ext2fs_hashmap_free(ਬਣਤਰ ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ਅਚਲ ਖਾਲੀ *str, size_t ਮਾਪ);

#ਜੇ_ਅੰਤ /* !HASHMAP_H */
