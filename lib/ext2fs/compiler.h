#جيڪڏهن_وصف_نه_ٿيل _EXT2FS_COMPILER_H
#وصف_ڪريو _EXT2FS_COMPILER_H

#ضمن <stddef.h>

#جيڪڏهن_وصف_ٿيل __GNUC__

#جيڪڏهن_وصف_نه_ٿيل __GNUC_PREREQ
#إذا defined(__GNUC__) && defined(__GNUC_MINOR__)
#وصف_ڪريو __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وإلا
#وصف_ڪريو __GNUC_PREREQ(maj, min) 0
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#وصف_ڪريو container_جو(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (حرف *)__mptr - offsetof(type,member) );})
#وإلا
#وصف_ڪريو container_جو(ptr, type, member)				\
	((type *)((حرف *)(ptr) - offsetof(type, member)))
#جيڪڏهن_پورو


#جيڪڏهن_پورو /* _EXT2FS_COMPILER_H */
