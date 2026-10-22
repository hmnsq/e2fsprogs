#यदि_अपरिभाषितम् HASHMAP_H
# define HASHMAP_H

# include <stdlib.h>
# include <stdint.h>

#यदि_अपरिभाषितम् __GNUC_PREREQ
#यदि defined(__GNUC__) && defined(__GNUC_MINOR__)
#परिभाषय __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#अन्यथा
#परिभाषय __GNUC_PREREQ(maj, min) 0
#यदि_अन्तः
#यदि_अन्तः

संरचना ext2fs_hashmap;

संरचना ext2fs_hashmap_प्रविष्टिः {
	शून्य *दत्त;
	ध्रुव शून्य *कुञ्चिका;
	size_t कुञ्चिका_len;
	संरचना ext2fs_hashmap_प्रविष्टिः *अग्र;
	संरचना ext2fs_hashmap_प्रविष्टिः *list_अग्र;
	संरचना ext2fs_hashmap_प्रविष्टिः *list_पूर्व;
};

संरचना ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ध्रुव शून्य*, size_t),
				शून्य(*free_fct)(शून्य*), size_t आकार);
पूर्णाङ्क ext2fs_hashmap_सङ्कलय(संरचना ext2fs_hashmap *h,
		       शून्य *दत्त, ध्रुव शून्य *कुञ्चिका,size_t कुञ्चिका_len);
शून्य *ext2fs_hashmap_lookup(संरचना ext2fs_hashmap *h, ध्रुव शून्य *कुञ्चिका,
			    size_t कुञ्चिका_len);
शून्य *ext2fs_hashmap_iter_अन्तर्_क्रमः(संरचना ext2fs_hashmap *h,
				   संरचना ext2fs_hashmap_प्रविष्टिः **it);
शून्य ext2fs_hashmap_del(संरचना ext2fs_hashmap *h,
			संरचना ext2fs_hashmap_प्रविष्टिः *e);
शून्य ext2fs_hashmap_free(संरचना ext2fs_hashmap *h);

uint32_t ext2fs_djb2_hash(ध्रुव शून्य *str, size_t आकार);

#यदि_अन्तः /* !HASHMAP_H */
