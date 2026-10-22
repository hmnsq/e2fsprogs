#ifndef _EXT2FS_COMPILER_H
#define _EXT2FS_COMPILER_H

#अन्तर्भाव <stddef.h>

#ifdef __GNUC__

#ifndef __GNUC_PREREQ
#यदि defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#अन्यथा
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

#define container_of(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (वर्ण *)__mptr - offsetof(type,member) );})
#अन्यथा
#define container_of(ptr, type, member)				\
	((type *)((वर्ण *)(ptr) - offsetof(type, member)))
#endif


#endif /* _EXT2FS_COMPILER_H */
