/*
 * Ext4's on-disk acl format.  From linux/fs/ext4/acl.h
 */

#परिभाषय EXT4_ACL_संस्करणम्        0x0001

/* 23.2.5 acl_tag_t values */

#परिभाषय ACL_UNDEFINED_चिह्नकम्	(0x00)
#परिभाषय ACL_USER_OBJ		(0x01)
#परिभाषय ACL_USER		(0x02)
#परिभाषय ACL_गणः_OBJ		(0x04)
#परिभाषय ACL_गणः		(0x08)
#परिभाषय ACL_आवरणम्		(0x10)
#परिभाषय ACL_OTHER		(0x20)

/* 23.3.6 acl_type_t values */

#परिभाषय ACL_TYPE_ACCESS		(0x8000)
#परिभाषय ACL_TYPE_DEFAULT	(0x4000)

/* 23.2.7 ACL qualifier constants */

#परिभाषय ACL_UNDEFINED_ID	((id_t)-1)

प्रकारनिर्देश संरचना {
        __le16          e_चिह्नकम्;
        __le16          e_perm;
        __le32          e_id;
 } ext4_acl_प्रविष्टिः;
 
प्रकारनिर्देश संरचना {
        __le16          e_चिह्नकम्;
        __le16          e_perm;
} ext4_acl_प्रविष्टिः_short;

प्रकारनिर्देश संरचना {
         __le32          a_संस्करणम्;
} ext4_acl_शीर्षकम्;


/* Supported ACL a_version fields */
 #परिभाषय POSIX_ACL_XATTR_संस्करणम् 0x0002

प्रकारनिर्देश संरचना {
        __le16                  e_चिह्नकम्;
        __le16                  e_perm;
        __le32                  e_id;
} posix_acl_xattr_प्रविष्टिः;

प्रकारनिर्देश संरचना {
        __le32                  a_संस्करणम्;
#यदि __GNUC_PREREQ (4, 8)
#निर्देशः GCC diagnostic प्रक्षिप
#निर्देशः GCC diagnostic ignored "-Wpedantic"
#यदि_अन्तः
        posix_acl_xattr_प्रविष्टिः   a_entries[0];
#यदि __GNUC_PREREQ (4, 8)
#निर्देशः GCC diagnostic निष्कासय
#यदि_अन्तः
} posix_acl_xattr_शीर्षकम्;

