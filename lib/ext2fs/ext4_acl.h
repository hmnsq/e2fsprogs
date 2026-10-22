/*
 * Ext4's on-disk acl format.  From linux/fs/ext4/acl.h
 */

#घोषणा EXT4_ACL_VERSION        0x0001

/* 23.2.5 acl_tag_t values */

#घोषणा ACL_UNDEFINED_TAG	(0x00)
#घोषणा ACL_USER_OBJ		(0x01)
#घोषणा ACL_USER		(0x02)
#घोषणा ACL_GROUP_OBJ		(0x04)
#घोषणा ACL_GROUP		(0x08)
#घोषणा ACL_MASK		(0x10)
#घोषणा ACL_OTHER		(0x20)

/* 23.3.6 acl_type_t values */

#घोषणा ACL_TYPE_ACCESS		(0x8000)
#घोषणा ACL_TYPE_DEFAULT	(0x4000)

/* 23.2.7 ACL qualifier constants */

#घोषणा ACL_UNDEFINED_ID	((id_t)-1)

प्रकार काष्ठा {
        __le16          e_tag;
        __le16          e_perm;
        __le32          e_id;
 } ext4_acl_entry;
 
प्रकार काष्ठा {
        __le16          e_tag;
        __le16          e_perm;
} ext4_acl_entry_short;

प्रकार काष्ठा {
         __le32          a_version;
} ext4_acl_header;


/* Supported ACL a_version fields */
 #घोषणा POSIX_ACL_XATTR_VERSION 0x0002

प्रकार काष्ठा {
        __le16                  e_tag;
        __le16                  e_perm;
        __le32                  e_id;
} posix_acl_xattr_entry;

प्रकार काष्ठा {
        __le32                  a_version;
#अगर __GNUC_PREREQ (4, 8)
#आशय GCC diagnostic धकेलो
#आशय GCC diagnostic ignored "-Wpedantic"
#पूर्ण_अगर
        posix_acl_xattr_entry   a_entries[0];
#अगर __GNUC_PREREQ (4, 8)
#आशय GCC diagnostic निकालो
#पूर्ण_अगर
} posix_acl_xattr_header;

