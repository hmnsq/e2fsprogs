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
	अचर शून्य *साँचो;
	माप_प्रकार साँचो_len;
	संरचना ext2fs_hashmap_entry *next;
	संरचना ext2fs_hashmap_entry *list_next;
	संरचना ext2fs_hashmap_entry *list_prev;
};

संरचना ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(अचर शून्य*, माप_प्रकार),
				शून्य(*free_fct)(शून्य*), माप_प्रकार size);
पूर्णाङ्क ext2fs_hashmap_add(संरचना ext2fs_hashmap *h,
		       शून्य *data, अचर शून्य *साँचो,माप_प्रकार साँचो_len);
शून्य *ext2fs_hashmap_lookup(संरचना ext2fs_hashmap *h, अचर शून्य *साँचो,
			    माप_प्रकार साँचो_len);
शून्य *ext2fs_hashmap_iter_in_order(संरचना ext2fs_hashmap *h,
				   संरचना ext2fs_hashmap_entry **it);
शून्य ext2fs_hashmap_del(संरचना ext2fs_hashmap *h,
			संरचना ext2fs_hashmap_entry *e);
शून्य ext2fs_hashmap_free(संरचना ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(अचर शून्य *str, माप_प्रकार size);

#endif /* !HASHMAP_H */
