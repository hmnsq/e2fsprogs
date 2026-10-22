#جے_بنا_تعریف _EXT2FS_COMPILER_H
#تعریف_کرو _EXT2FS_COMPILER_H

#ضمن <stddef.h>

#جے_تعریف_شدہ __GNUC__

#جے_بنا_تعریف __GNUC_PREREQ
#إذا defined(__GNUC__) && defined(__GNUC_MINOR__)
#تعریف_کرو __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#وإلا
#تعریف_کرو __GNUC_PREREQ(maj, min) 0
#جے_انت
#جے_انت

#تعریف_کرو container_دا(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (حرف *)__mptr - offsetof(type,member) );})
#وإلا
#تعریف_کرو container_دا(ptr, type, member)				\
	((type *)((حرف *)(ptr) - offsetof(type, member)))
#جے_انت


#جے_انت /* _EXT2FS_COMPILER_H */
