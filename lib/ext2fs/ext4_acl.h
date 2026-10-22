/*
 * Ext4's on-disk acl format.  From linux/fs/ext4/acl.h
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_ACL_ਸੰਸਕਰਣ        0x0001

/* 23.2.5 acl_tag_t values */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACL_UNDEFINED_ਟੈਗ	(0x00)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACL_USER_OBJ		(0x01)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACL_USER		(0x02)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACL_ਸਮੂਹ_OBJ		(0x04)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACL_ਸਮੂਹ		(0x08)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACL_ਨਕਾਬ		(0x10)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACL_OTHER		(0x20)

/* 23.3.6 acl_type_t values */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACL_TYPE_ACCESS		(0x8000)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACL_TYPE_DEFAULT	(0x4000)

/* 23.2.7 ACL qualifier constants */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACL_UNDEFINED_ID	((id_t)-1)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
        __le16          e_ਟੈਗ;
        __le16          e_perm;
        __le32          e_id;
 } ext4_acl_ਇੰਦਰਾਜ;
 
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
        __le16          e_ਟੈਗ;
        __le16          e_perm;
} ext4_acl_ਇੰਦਰਾਜ_short;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
         __le32          a_ਸੰਸਕਰਣ;
} ext4_acl_ਸਿਰਲੇਖ;


/* Supported ACL a_version fields */
 #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ POSIX_ACL_XATTR_ਸੰਸਕਰਣ 0x0002

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
        __le16                  e_ਟੈਗ;
        __le16                  e_perm;
        __le32                  e_id;
} posix_acl_xattr_ਇੰਦਰਾਜ;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
        __le32                  a_ਸੰਸਕਰਣ;
#ਜੇ __GNUC_PREREQ (4, 8)
#ਹਦਾਇਤ GCC diagnostic ਧੱਕੋ
#ਹਦਾਇਤ GCC diagnostic ignored "-Wpedantic"
#ਜੇ_ਅੰਤ
        posix_acl_xattr_ਇੰਦਰਾਜ   a_entries[0];
#ਜੇ __GNUC_PREREQ (4, 8)
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
#ਜੇ_ਅੰਤ
} posix_acl_xattr_ਸਿਰਲੇਖ;

