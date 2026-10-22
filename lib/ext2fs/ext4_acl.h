/*
 * Ext4's on-disk acl format.  From linux/fs/ext4/acl.h
 */

#وصف_ڪريو EXT4_ACL_نسخو        0x0001

/* 23.2.5 acl_tag_t values */

#وصف_ڪريو ACL_UNDEFINED_ٽيگ	(0x00)
#وصف_ڪريو ACL_USER_OBJ		(0x01)
#وصف_ڪريو ACL_USER		(0x02)
#وصف_ڪريو ACL_گروهه_OBJ		(0x04)
#وصف_ڪريو ACL_گروهه		(0x08)
#وصف_ڪريو ACL_نقاب		(0x10)
#وصف_ڪريو ACL_OTHER		(0x20)

/* 23.3.6 acl_type_t values */

#وصف_ڪريو ACL_TYPE_ACCESS		(0x8000)
#وصف_ڪريو ACL_TYPE_DEFAULT	(0x4000)

/* 23.2.7 ACL qualifier constants */

#وصف_ڪريو ACL_UNDEFINED_ID	((id_t)-1)

تعريفنوع بنية {
        __le16          e_ٽيگ;
        __le16          e_perm;
        __le32          e_id;
 } ext4_acl_داخلا;
 
تعريفنوع بنية {
        __le16          e_ٽيگ;
        __le16          e_perm;
} ext4_acl_داخلا_short;

تعريفنوع بنية {
         __le32          a_نسخو;
} ext4_acl_سرو;


/* Supported ACL a_version fields */
 #وصف_ڪريو POSIX_ACL_XATTR_نسخو 0x0002

تعريفنوع بنية {
        __le16                  e_ٽيگ;
        __le16                  e_perm;
        __le32                  e_id;
} posix_acl_xattr_داخلا;

تعريفنوع بنية {
        __le32                  a_نسخو;
#إذا __GNUC_PREREQ (4, 8)
#هدايت GCC diagnostic ڌڪو
#هدايت GCC diagnostic ignored "-Wpedantic"
#جيڪڏهن_پورو
        posix_acl_xattr_داخلا   a_entries[0];
#إذا __GNUC_PREREQ (4, 8)
#هدايت GCC diagnostic ڪڍو
#جيڪڏهن_پورو
} posix_acl_xattr_سرو;

