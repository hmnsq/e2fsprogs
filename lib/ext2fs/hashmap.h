#ifndef HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#ifndef __GNUC_PREREQ
#यदि defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#अन्यथा
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

संरचना ext2fs_hashmap;

संरचना ext2fs_hashmap_entry {
	शून्य *data;
	ध्रुव शून्य *कुंजी;
	size_t कुंजी_len;
	संरचना ext2fs_hashmap_entry *next;
	संरचना ext2fs_hashmap_entry *list_next;
	संरचना ext2fs_hashmap_entry *list_prev;
};

संरचना ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ध्रुव शून्य*, size_t),
				शून्य(*free_fct)(शून्य*), size_t size);
पूर्णाङ्क ext2fs_hashmap_add(संरचना ext2fs_hashmap *h,
		       शून्य *data, ध्रुव शून्य *कुंजी,size_t कुंजी_len);
शून्य *ext2fs_hashmap_lookup(संरचना ext2fs_hashmap *h, ध्रुव शून्य *कुंजी,
			    size_t कुंजी_len);
शून्य *ext2fs_hashmap_iter_in_order(संरचना ext2fs_hashmap *h,
				   संरचना ext2fs_hashmap_entry **it);
शून्य ext2fs_hashmap_del(संरचना ext2fs_hashmap *h,
			संरचना ext2fs_hashmap_entry *e);
शून्य ext2fs_hashmap_free(संरचना ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ध्रुव शून्य *str, size_t size);

#endif /* !HASHMAP_H */
