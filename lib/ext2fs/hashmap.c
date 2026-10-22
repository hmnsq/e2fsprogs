#अन्तर्भाव "hashmap.h"
#अन्तर्भाव <string.h>

संरचना ext2fs_hashmap {
	uint32_t आकार;
	uint32_t(*hash)(ध्रुव शून्य *कुञ्चिका, size_t len);
	शून्य(*मुक्त)(शून्य*);
	संरचना ext2fs_hashmap_प्रविष्टिः *first;
	संरचना ext2fs_hashmap_प्रविष्टिः *चरमः;
#यदि __GNUC_PREREQ (4, 8)
#निर्देशः GCC diagnostic प्रक्षिप
#निर्देशः GCC diagnostic ignored "-Wpedantic"
#यदि_अन्तः
	संरचना ext2fs_hashmap_प्रविष्टिः *entries[0];
#यदि __GNUC_PREREQ (4, 8)
#निर्देशः GCC diagnostic निष्कासय
#यदि_अन्तः
};

uint32_t ext2fs_djb2_hash(ध्रुव शून्य *str, size_t आकार)
{
	पूर्णाङ्क c;
	ध्रुव वर्ण *s = str;
	uint32_t hash = 5381;

	यावत् (आकार-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	प्रत्यागम hash;
}

संरचना ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(ध्रुव शून्य*, size_t),
				शून्य(*free_fct)(शून्य*), size_t आकार)
{
	संरचना ext2fs_hashmap *h = शून्यस्मृतिं_देहि(1, परिमाण(संरचना ext2fs_hashmap) +
				परिमाण(संरचना ext2fs_hashmap_प्रविष्टिः) * आकार);
	यदि (!h)
		प्रत्यागम रिक्त;

	h->आकार = आकार;
	h->मुक्त = free_fct;
	h->hash = hash_fct;
	h->first = h->चरमः = रिक्त;
	प्रत्यागम h;
}

पूर्णाङ्क ext2fs_hashmap_सङ्कलय(संरचना ext2fs_hashmap *h,
		       शून्य *दत्त, ध्रुव शून्य *कुञ्चिका, size_t कुञ्चिका_len)
{
	uint32_t hash = h->hash(कुञ्चिका, कुञ्चिका_len) % h->आकार;
	संरचना ext2fs_hashmap_प्रविष्टिः *e = स्मृतिं_देहि(परिमाण(*e));

	यदि (!e)
		प्रत्यागम -1;

	e->दत्त = दत्त;
	e->कुञ्चिका = कुञ्चिका;
	e->कुञ्चिका_len = कुञ्चिका_len;
	e->अग्र = h->entries[hash];
	h->entries[hash] = e;

	e->list_पूर्व = रिक्त;
	e->list_अग्र = h->first;
	यदि (h->first)
		h->first->list_पूर्व = e;
	h->first = e;
	यदि (!h->चरमः)
		h->चरमः = e;

	प्रत्यागम 0;
}

शून्य *ext2fs_hashmap_lookup(संरचना ext2fs_hashmap *h, ध्रुव शून्य *कुञ्चिका,
			    size_t कुञ्चिका_len)
{
	संरचना ext2fs_hashmap_प्रविष्टिः *iter;
	uint32_t hash = h->hash(कुञ्चिका, कुञ्चिका_len) % h->आकार;

	प्रति (iter = h->entries[hash]; iter; iter = iter->अग्र)
		यदि (iter->कुञ्चिका_len == कुञ्चिका_len && !memcmp(iter->कुञ्चिका, कुञ्चिका, कुञ्चिका_len))
			प्रत्यागम iter->दत्त;
	प्रत्यागम रिक्त;
}

शून्य *ext2fs_hashmap_iter_अन्तर्_क्रमः(संरचना ext2fs_hashmap *h,
				   संरचना ext2fs_hashmap_प्रविष्टिः **it)
{
	*it = *it ? (*it)->list_अग्र : h->first;
	प्रत्यागम *it ? (*it)->दत्त : रिक्त;
}

शून्य ext2fs_hashmap_free(संरचना ext2fs_hashmap *h)
{
	size_t	i;

	प्रति (i = 0; i < h->आकार; ++i) {
		संरचना ext2fs_hashmap_प्रविष्टिः *it = h->entries[i];
		यावत् (it) {
			संरचना ext2fs_hashmap_प्रविष्टिः *tmp = it->अग्र;
			यदि (h->मुक्त)
				h->मुक्त(it->दत्त);
			मुक्त(it);
			it = tmp;
		}
	}
	मुक्त(h);
}
