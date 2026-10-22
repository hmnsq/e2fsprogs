#అన్తర్భావ "hashmap.h"
#అన్తర్భావ <string.h>

నిర్మాణం ext2fs_hashmap {
	uint32_t కొలత;
	uint32_t(*hash)(స్థిరరాశి శూన్య *కీలకం, size_t len);
	శూన్య(*విడుదల)(శూన్య*);
	నిర్మాణం ext2fs_hashmap_నమోదు *మొదటి;
	నిర్మాణం ext2fs_hashmap_నమోదు *చివరి;
#ఒకవేళ __GNUC_PREREQ (4, 8)
#సూచన GCC diagnostic నెట్టు
#సూచన GCC diagnostic ignored "-Wpedantic"
#అయితే_ముగింపు
	నిర్మాణం ext2fs_hashmap_నమోదు *entries[0];
#ఒకవేళ __GNUC_PREREQ (4, 8)
#సూచన GCC diagnostic తీయి
#అయితే_ముగింపు
};

uint32_t ext2fs_djb2_hash(స్థిరరాశి శూన్య *str, size_t కొలత)
{
	పూర్ణాంకం c;
	స్థిరరాశి అక్షరం *s = str;
	uint32_t hash = 5381;

	అయితే (కొలత-- > 0) {
		c = *s++;
		hash = ((hash << 5) + hash) + c;
	}
	తిరిగి hash;
}

నిర్మాణం ext2fs_hashmap *ext2fs_hashmap_create(
				uint32_t(*hash_fct)(స్థిరరాశి శూన్య*, size_t),
				శూన్య(*free_fct)(శూన్య*), size_t కొలత)
{
	నిర్మాణం ext2fs_hashmap *h = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(నిర్మాణం ext2fs_hashmap) +
				పరిమాణం(నిర్మాణం ext2fs_hashmap_నమోదు) * కొలత);
	ఒకవేళ (!h)
		తిరిగి ఖాళీ;

	h->కొలత = కొలత;
	h->విడుదల = free_fct;
	h->hash = hash_fct;
	h->మొదటి = h->చివరి = ఖాళీ;
	తిరిగి h;
}

పూర్ణాంకం ext2fs_hashmap_కూడు(నిర్మాణం ext2fs_hashmap *h,
		       శూన్య *దత్తాంశం, స్థిరరాశి శూన్య *కీలకం, size_t కీలకం_len)
{
	uint32_t hash = h->hash(కీలకం, కీలకం_len) % h->కొలత;
	నిర్మాణం ext2fs_hashmap_నమోదు *e = జ్ఞాపకం_ఇవ్వు(పరిమాణం(*e));

	ఒకవేళ (!e)
		తిరిగి -1;

	e->దత్తాంశం = దత్తాంశం;
	e->కీలకం = కీలకం;
	e->కీలకం_len = కీలకం_len;
	e->తదుపరి = h->entries[hash];
	h->entries[hash] = e;

	e->list_prev = ఖాళీ;
	e->list_తదుపరి = h->మొదటి;
	ఒకవేళ (h->మొదటి)
		h->మొదటి->list_prev = e;
	h->మొదటి = e;
	ఒకవేళ (!h->చివరి)
		h->చివరి = e;

	తిరిగి 0;
}

శూన్య *ext2fs_hashmap_lookup(నిర్మాణం ext2fs_hashmap *h, స్థిరరాశి శూన్య *కీలకం,
			    size_t కీలకం_len)
{
	నిర్మాణం ext2fs_hashmap_నమోదు *iter;
	uint32_t hash = h->hash(కీలకం, కీలకం_len) % h->కొలత;

	కొరకు (iter = h->entries[hash]; iter; iter = iter->తదుపరి)
		ఒకవేళ (iter->కీలకం_len == కీలకం_len && !memcmp(iter->కీలకం, కీలకం, కీలకం_len))
			తిరిగి iter->దత్తాంశం;
	తిరిగి ఖాళీ;
}

శూన్య *ext2fs_hashmap_iter_లోపల_క్రమం(నిర్మాణం ext2fs_hashmap *h,
				   నిర్మాణం ext2fs_hashmap_నమోదు **it)
{
	*it = *it ? (*it)->list_తదుపరి : h->మొదటి;
	తిరిగి *it ? (*it)->దత్తాంశం : ఖాళీ;
}

శూన్య ext2fs_hashmap_free(నిర్మాణం ext2fs_hashmap *h)
{
	size_t	i;

	కొరకు (i = 0; i < h->కొలత; ++i) {
		నిర్మాణం ext2fs_hashmap_నమోదు *it = h->entries[i];
		అయితే (it) {
			నిర్మాణం ext2fs_hashmap_నమోదు *tmp = it->తదుపరి;
			ఒకవేళ (h->విడుదల)
				h->విడుదల(it->దత్తాంశం);
			విడుదల(it);
			it = tmp;
		}
	}
	విడుదల(h);
}
