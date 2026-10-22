/*
 * Ext4's on-disk acl format.  From linux/fs/ext4/acl.h
 */

#నిర్వచించు EXT4_ACL_సంస్కరణ        0x0001

/* 23.2.5 acl_tag_t values */

#నిర్వచించు ACL_UNDEFINED_ట్యాగ్	(0x00)
#నిర్వచించు ACL_USER_OBJ		(0x01)
#నిర్వచించు ACL_USER		(0x02)
#నిర్వచించు ACL_సమూహం_OBJ		(0x04)
#నిర్వచించు ACL_సమూహం		(0x08)
#నిర్వచించు ACL_ముసుగు		(0x10)
#నిర్వచించు ACL_OTHER		(0x20)

/* 23.3.6 acl_type_t values */

#నిర్వచించు ACL_TYPE_ACCESS		(0x8000)
#నిర్వచించు ACL_TYPE_DEFAULT	(0x4000)

/* 23.2.7 ACL qualifier constants */

#నిర్వచించు ACL_UNDEFINED_ID	((id_t)-1)

రకనిర్వచనం నిర్మాణం {
        __le16          e_ట్యాగ్;
        __le16          e_perm;
        __le32          e_id;
 } ext4_acl_నమోదు;
 
రకనిర్వచనం నిర్మాణం {
        __le16          e_ట్యాగ్;
        __le16          e_perm;
} ext4_acl_నమోదు_short;

రకనిర్వచనం నిర్మాణం {
         __le32          a_సంస్కరణ;
} ext4_acl_శీర్షిక;


/* Supported ACL a_version fields */
 #నిర్వచించు POSIX_ACL_XATTR_సంస్కరణ 0x0002

రకనిర్వచనం నిర్మాణం {
        __le16                  e_ట్యాగ్;
        __le16                  e_perm;
        __le32                  e_id;
} posix_acl_xattr_నమోదు;

రకనిర్వచనం నిర్మాణం {
        __le32                  a_సంస్కరణ;
#ఒకవేళ __GNUC_PREREQ (4, 8)
#సూచన GCC diagnostic నెట్టు
#సూచన GCC diagnostic ignored "-Wpedantic"
#అయితే_ముగింపు
        posix_acl_xattr_నమోదు   a_entries[0];
#ఒకవేళ __GNUC_PREREQ (4, 8)
#సూచన GCC diagnostic తీయి
#అయితే_ముగింపు
} posix_acl_xattr_శీర్షిక;

