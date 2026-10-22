#ifndef _EXT2FS_COMPILER_H
#define _EXT2FS_COMPILER_H

#અન્તર્ભાવ <stddef.h>

#ifdef __GNUC__

#ifndef __GNUC_PREREQ
#જો defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#નહીંતર
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

#define container_of(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (અક્ષર *)__mptr - offsetof(type,member) );})
#નહીંતર
#define container_of(ptr, type, member)				\
	((type *)((અક્ષર *)(ptr) - offsetof(type, member)))
#endif


#endif /* _EXT2FS_COMPILER_H */
