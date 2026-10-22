#ifndef _EXT2FS_COMPILER_H
#define _EXT2FS_COMPILER_H

#شامل <stddef.h>

#ifdef __GNUC__

#ifndef __GNUC_PREREQ
#اگر defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وگرنه
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

#define container_of(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (نویسه *)__mptr - offsetof(type,member) );})
#وگرنه
#define container_of(ptr, type, member)				\
	((type *)((نویسه *)(ptr) - offsetof(type, member)))
#endif


#endif /* _EXT2FS_COMPILER_H */
