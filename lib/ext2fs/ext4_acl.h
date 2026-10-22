/*
 * Ext4's on-disk acl format.  From linux/fs/ext4/acl.h
 */

#تعریف_کرو EXT4_ACL_نسخہ        0x0001

/* 23.2.5 acl_tag_t values */

#تعریف_کرو ACL_UNDEFINED_لیبل	(0x00)
#تعریف_کرو ACL_USER_OBJ		(0x01)
#تعریف_کرو ACL_USER		(0x02)
#تعریف_کرو ACL_گروہ_OBJ		(0x04)
#تعریف_کرو ACL_گروہ		(0x08)
#تعریف_کرو ACL_نقاب		(0x10)
#تعریف_کرو ACL_OTHER		(0x20)

/* 23.3.6 acl_type_t values */

#تعریف_کرو ACL_TYPE_ACCESS		(0x8000)
#تعریف_کرو ACL_TYPE_DEFAULT	(0x4000)

/* 23.2.7 ACL qualifier constants */

#تعریف_کرو ACL_UNDEFINED_ID	((id_t)-1)

قسمتعریف ساخت {
        __le16          e_لیبل;
        __le16          e_perm;
        __le32          e_id;
 } ext4_acl_اندراج;
 
قسمتعریف ساخت {
        __le16          e_لیبل;
        __le16          e_perm;
} ext4_acl_اندراج_short;

قسمتعریف ساخت {
         __le32          a_نسخہ;
} ext4_acl_سرخی;


/* Supported ACL a_version fields */
 #تعریف_کرو POSIX_ACL_XATTR_نسخہ 0x0002

قسمتعریف ساخت {
        __le16                  e_لیبل;
        __le16                  e_perm;
        __le32                  e_id;
} posix_acl_xattr_اندراج;

قسمتعریف ساخت {
        __le32                  a_نسخہ;
#اگر __GNUC_PREREQ (4, 8)
#ہدایت GCC diagnostic ڈالو
#ہدایت GCC diagnostic ignored "-Wpedantic"
#اگر_ختم
        posix_acl_xattr_اندراج   a_entries[0];
#اگر __GNUC_PREREQ (4, 8)
#ہدایت GCC diagnostic نکالو
#اگر_ختم
} posix_acl_xattr_سرخی;

