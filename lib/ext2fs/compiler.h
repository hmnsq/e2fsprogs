#నిర్వచితం_కాకపోతే _EXT2FS_COMPILER_H
#నిర్వచించు _EXT2FS_COMPILER_H

#అన్తర్భావ <stddef.h>

#నిర్వచితమైతే __GNUC__

#నిర్వచితం_కాకపోతే __GNUC_PREREQ
#ఒకవేళ defined(__GNUC__) && defined(__GNUC_MINOR__)
#నిర్వచించు __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#లేకపోతే
#నిర్వచించు __GNUC_PREREQ(maj, min) 0
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచించు container_యొక్క(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (అక్షరం *)__mptr - offsetof(type,member) );})
#లేకపోతే
#నిర్వచించు container_యొక్క(ptr, type, member)				\
	((type *)((అక్షరం *)(ptr) - offsetof(type, member)))
#అయితే_ముగింపు


#అయితే_ముగింపు /* _EXT2FS_COMPILER_H */
