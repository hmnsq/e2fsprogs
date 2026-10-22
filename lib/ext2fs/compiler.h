#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _EXT2FS_COMPILER_H
#ವ್ಯಾಖ್ಯಾನಿಸು _EXT2FS_COMPILER_H

#ಅನ್ತರ್ಭಾವ <stddef.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __GNUC_PREREQ
#ಆದರೆ defined(__GNUC__) && defined(__GNUC_MINOR__)
#ವ್ಯಾಖ್ಯಾನಿಸು __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು __GNUC_PREREQ(maj, min) 0
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು container_ದ(ptr, type, member) ({				\
	__typeof__( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (ಅಕ್ಷರ *)__mptr - offsetof(type,member) );})
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು container_ದ(ptr, type, member)				\
	((type *)((ಅಕ್ಷರ *)(ptr) - offsetof(type, member)))
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ_ಅಂತ್ಯ /* _EXT2FS_COMPILER_H */
