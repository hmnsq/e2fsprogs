#جيڪڏهن_وصف_نه_ٿيل HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#جيڪڏهن_وصف_نه_ٿيل __GNUC_PREREQ
#إذا defined(__GNUC__) && defined(__GNUC_MINOR__)
#وصف_ڪريو __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وإلا
#وصف_ڪريو __GNUC_PREREQ(maj, min) 0
#جيڪڏهن_پورو
#جيڪڏهن_پورو

بنية ext2fs_hashmap;

بنية ext2fs_hashmap_داخلا {
	فراغ *معلومات;
	ثابت فراغ *ڪنجي;
	size_t ڪنجي_len;
	بنية ext2fs_hashmap_داخلا *ايندڙ;
	بنية ext2fs_hashmap_داخلا *list_ايندڙ;
	بنية ext2fs_hashmap_داخلا *list_prev;
};

بنية ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ثابت فراغ*, size_t),
				فراغ(*free_fct)(فراغ*), size_t سائيز);
صحيح ext2fs_hashmap_جوڙيو(بنية ext2fs_hashmap *h,
		       فراغ *معلومات, ثابت فراغ *ڪنجي,size_t ڪنجي_len);
فراغ *ext2fs_hashmap_lookup(بنية ext2fs_hashmap *h, ثابت فراغ *ڪنجي,
			    size_t ڪنجي_len);
فراغ *ext2fs_hashmap_iter_اندريون_ترتيب(بنية ext2fs_hashmap *h,
				   بنية ext2fs_hashmap_داخلا **it);
فراغ ext2fs_hashmap_del(بنية ext2fs_hashmap *h,
			بنية ext2fs_hashmap_داخلا *e);
فراغ ext2fs_hashmap_free(بنية ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ثابت فراغ *str, size_t سائيز);

#جيڪڏهن_پورو /* !HASHMAP_H */
