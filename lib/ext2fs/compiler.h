#که_نه_تعریف_شوی _EXT2FS_COMPILER_H
#تعریف_کړه _EXT2FS_COMPILER_H

#ضمن <stddef.h>

#که_تعریف_شوی __GNUC__

#که_نه_تعریف_شوی __GNUC_PREREQ
#إذا defined(__GNUC__) && defined(__GNUC_MINOR__)
#تعریف_کړه __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وإلا
#تعریف_کړه __GNUC_PREREQ(maj, min) 0
#د_که_پای
#د_که_پای

#تعریف_کړه container_د(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (حرف *)__mptr - offsetof(type,member) );})
#وإلا
#تعریف_کړه container_د(ptr, type, member)				\
	((type *)((حرف *)(ptr) - offsetof(type, member)))
#د_که_پای


#د_که_پای /* _EXT2FS_COMPILER_H */
