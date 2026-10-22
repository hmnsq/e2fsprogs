/*
 * Ext4's on-disk acl format.  From linux/fs/ext4/acl.h
 */

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_ACL_ಆವೃತ್ತಿ        0x0001

/* 23.2.5 acl_tag_t values */

#ವ್ಯಾಖ್ಯಾನಿಸು ACL_UNDEFINED_ಟ್ಯಾಗ್	(0x00)
#ವ್ಯಾಖ್ಯಾನಿಸು ACL_USER_OBJ		(0x01)
#ವ್ಯಾಖ್ಯಾನಿಸು ACL_USER		(0x02)
#ವ್ಯಾಖ್ಯಾನಿಸು ACL_ಗುಂಪು_OBJ		(0x04)
#ವ್ಯಾಖ್ಯಾನಿಸು ACL_ಗುಂಪು		(0x08)
#ವ್ಯಾಖ್ಯಾನಿಸು ACL_ಮುಖವಾಡ		(0x10)
#ವ್ಯಾಖ್ಯಾನಿಸು ACL_OTHER		(0x20)

/* 23.3.6 acl_type_t values */

#ವ್ಯಾಖ್ಯಾನಿಸು ACL_TYPE_ACCESS		(0x8000)
#ವ್ಯಾಖ್ಯಾನಿಸು ACL_TYPE_DEFAULT	(0x4000)

/* 23.2.7 ACL qualifier constants */

#ವ್ಯಾಖ್ಯಾನಿಸು ACL_UNDEFINED_ID	((id_t)-1)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
        __le16          e_ಟ್ಯಾಗ್;
        __le16          e_perm;
        __le32          e_id;
 } ext4_acl_ನಮೂದು;
 
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
        __le16          e_ಟ್ಯಾಗ್;
        __le16          e_perm;
} ext4_acl_ನಮೂದು_short;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
         __le32          a_ಆವೃತ್ತಿ;
} ext4_acl_ಶೀರ್ಷಿಕೆ;


/* Supported ACL a_version fields */
 #ವ್ಯಾಖ್ಯಾನಿಸು POSIX_ACL_XATTR_ಆವೃತ್ತಿ 0x0002

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
        __le16                  e_ಟ್ಯಾಗ್;
        __le16                  e_perm;
        __le32                  e_id;
} posix_acl_xattr_ನಮೂದು;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
        __le32                  a_ಆವೃತ್ತಿ;
#ಆದರೆ __GNUC_PREREQ (4, 8)
#ಸೂಚನೆ GCC diagnostic ತಳ್ಳು
#ಸೂಚನೆ GCC diagnostic ignored "-Wpedantic"
#ಆದರೆ_ಅಂತ್ಯ
        posix_acl_xattr_ನಮೂದು   a_entries[0];
#ಆದರೆ __GNUC_PREREQ (4, 8)
#ಸೂಚನೆ GCC diagnostic ತೆಗೆ
#ಆದರೆ_ಅಂತ್ಯ
} posix_acl_xattr_ಶೀರ್ಷಿಕೆ;

