#समावेश "hashmap.h"
#समावेश <string.h>

काष्ठा ext2fs_hashmap {
	uint32_t आकार;
	uint32_t(*hash)(स्थिर व्योम *कुंजी, माप_प्रकार len);
	व्योम(*मुक्त)(व्योम*);
	काष्ठा ext2fs_hashmap_entry *first;
	काष्ठा ext2fs_hashmap_entry *last;
#अगर __GNUC_PREREQ (4, 8)
#आशय GCC diagnostic धकेलो
#आशय GCC diagnostic ignored "-Wpedantic"
#पूर्ण_अगर
	काष्ठा ext2fs_hashmap_entry *entries[0];
#अगर __GNUC_PREREQ (4, 8)
#आशय GCC diagnostic निकालो
#पूर्ण_अगर
};

uint32_t ext2fs_djb2_hash(स्थिर व्योम *str, माप_प्रकार आकार)
{
	पूर्णांक c;
	स्थिर अक्षर *s = str;
	uint32_t hash = 5381;

	जबतक (आकार-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	वापस hash;
}

काष्ठा ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(स्थिर व्योम*, माप_प्रकार),
				व्योम(*free_fct)(व्योम*), माप_प्रकार आकार)
{
	काष्ठा ext2fs_hashmap *h = सुस्मृति(1, माप(काष्ठा ext2fs_hashmap) +
				माप(काष्ठा ext2fs_hashmap_entry) * आकार);
	अगर (!h)
		वापस शून्य;

	h->आकार = आकार;
	h->मुक्त = free_fct;
	h->hash = hash_fct;
	h->first = h->last = शून्य;
	वापस h;
}

पूर्णांक ext2fs_hashmap_add(काष्ठा ext2fs_hashmap *h,
		       व्योम *तथ्य, स्थिर व्योम *कुंजी, माप_प्रकार कुंजी_len)
{
	uint32_t hash = h->hash(कुंजी, कुंजी_len) % h->आकार;
	काष्ठा ext2fs_hashmap_entry *e = दो_स्मृति(माप(*e));

	अगर (!e)
		वापस -1;

	e->तथ्य = तथ्य;
	e->कुंजी = कुंजी;
	e->कुंजी_len = कुंजी_len;
	e->अग्र = h->entries[hash];
	h->entries[hash] = e;

	e->list_पूर्व = शून्य;
	e->list_अग्र = h->first;
	अगर (h->first)
		h->first->list_पूर्व = e;
	h->first = e;
	अगर (!h->last)
		h->last = e;

	वापस 0;
}

व्योम *ext2fs_hashmap_lookup(काष्ठा ext2fs_hashmap *h, स्थिर व्योम *कुंजी,
			    माप_प्रकार कुंजी_len)
{
	काष्ठा ext2fs_hashmap_entry *iter;
	uint32_t hash = h->hash(कुंजी, कुंजी_len) % h->आकार;

	क्रम (iter = h->entries[hash]; iter; iter = iter->अग्र)
		अगर (iter->कुंजी_len == कुंजी_len && !स_भेद(iter->कुंजी, कुंजी, कुंजी_len))
			वापस iter->तथ्य;
	वापस शून्य;
}

व्योम *ext2fs_hashmap_iter_in_order(काष्ठा ext2fs_hashmap *h,
				   काष्ठा ext2fs_hashmap_entry **it)
{
	*it = *it ? (*it)->list_अग्र : h->first;
	वापस *it ? (*it)->तथ्य : शून्य;
}

व्योम ext2fs_hashmap_free(काष्ठा ext2fs_hashmap *h)
{
	माप_प्रकार	i;

	क्रम (i = 0; i < h->आकार; ++i) {
		काष्ठा ext2fs_hashmap_entry *it = h->entries[i];
		जबतक (it) {
			काष्ठा ext2fs_hashmap_entry *tmp = it->अग्र;
			अगर (h->मुक्त)
				h->मुक्त(it->तथ्य);
			मुक्त(it);
			it = tmp;
		}
	}
	मुक्त(h);
}
