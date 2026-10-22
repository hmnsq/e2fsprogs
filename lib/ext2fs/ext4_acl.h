/*
 * Ext4's on-disk acl format.  From linux/fs/ext4/acl.h
 */

#تعریف_کړه EXT4_ACL_نسخه        0x0001

/* 23.2.5 acl_tag_t values */

#تعریف_کړه ACL_UNDEFINED_نښان	(0x00)
#تعریف_کړه ACL_USER_OBJ		(0x01)
#تعریف_کړه ACL_USER		(0x02)
#تعریف_کړه ACL_ډله_OBJ		(0x04)
#تعریف_کړه ACL_ډله		(0x08)
#تعریف_کړه ACL_ماسک		(0x10)
#تعریف_کړه ACL_OTHER		(0x20)

/* 23.3.6 acl_type_t values */

#تعریف_کړه ACL_TYPE_ACCESS		(0x8000)
#تعریف_کړه ACL_TYPE_DEFAULT	(0x4000)

/* 23.2.7 ACL qualifier constants */

#تعریف_کړه ACL_UNDEFINED_ID	((id_t)-1)

تعريفنوع بنية {
        __le16          e_نښان;
        __le16          e_perm;
        __le32          e_id;
 } ext4_acl_ثبت;
 
تعريفنوع بنية {
        __le16          e_نښان;
        __le16          e_perm;
} ext4_acl_ثبت_short;

تعريفنوع بنية {
         __le32          a_نسخه;
} ext4_acl_سرلیک;


/* Supported ACL a_version fields */
 #تعریف_کړه POSIX_ACL_XATTR_نسخه 0x0002

تعريفنوع بنية {
        __le16                  e_نښان;
        __le16                  e_perm;
        __le32                  e_id;
} posix_acl_xattr_ثبت;

تعريفنوع بنية {
        __le32                  a_نسخه;
#إذا __GNUC_PREREQ (4, 8)
#لارښوونه GCC diagnostic ټېل
#لارښوونه GCC diagnostic ignored "-Wpedantic"
#د_که_پای
        posix_acl_xattr_ثبت   a_entries[0];
#إذا __GNUC_PREREQ (4, 8)
#لارښوونه GCC diagnostic راوباسه
#د_که_پای
} posix_acl_xattr_سرلیک;

