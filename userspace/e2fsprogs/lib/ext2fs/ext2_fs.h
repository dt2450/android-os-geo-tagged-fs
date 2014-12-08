/*
 *  linux/include/linux/ext2_fs.h
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/include/linux/minix_fs.h
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#ifndef _LINUX_EXT2_FS_H
#define _LINUX_EXT2_FS_H

#include <ext2fs/ext2_types.h>		/* Changed from linux/types.h */

/*
 * The second extended filesystem constants/structures
 */

/*
 * Define EXT2FS_DEBUG to produce debug messages
 */
#undef EXT2FS_DEBUG

/*
 * Define EXT2_PREALLOCATE to preallocate data blocks for expanding files
 */
#define EXT2_PREALLOCATE
#define EXT2_DEFAULT_PREALLOC_BLOCKS	8

/*
 * The second extended file system version
 */
#define EXT2FS_DATE		"95/08/09"
#define EXT2FS_VERSION		"0.5b"

/*
 * Special inode numbers
 */
#define EXT2_BAD_INO		 1	/* Bad blocks inode */
#define EXT2_ROOT_INO		 2	/* Root inode */
#define EXT4_USR_QUOTA_INO	 3	/* User quota inode */
#define EXT4_GRP_QUOTA_INO	 4	/* Group quota inode */
#define EXT2_BOOT_LOADER_INO	 5	/* Boot loader inode */
#define EXT2_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#define EXT2_RESIZE_INO		 7	/* Reserved group descriptors inode */
#define EXT2_JOURNAL_INO	 8	/* Journal inode */
#define EXT2_EXCLUDE_INO	 9	/* The "exclude" inode, for snapshots */
#define EXT4_REPLICA_INO	10	/* Used by non-upstream feature */

/* First non-reserved inode for old ext2 filesystems */
#define EXT2_GOOD_OLD_FIRST_INO	11

/*
 * The second extended file system magic number
 */
#define EXT3_SUPER_MAGIC	0xEF54
#define EXT2_SUPER_MAGIC	EXT3_SUPER_MAGIC

#ifdef __KERNEL__
#define EXT2_SB(sb)	(&((sb)->u.ext2_sb))
#else
/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#define EXT2_SB(sb)	(sb)
#endif

/*
 * Maximal count of links to a file
 */
#define EXT2_LINK_MAX		65000

/*
 * Macro-instructions used to manage several block sizes
 */
#define EXT2_MIN_BLOCK_LOG_SIZE		10	/* 1024 */
#define EXT2_MAX_BLOCK_LOG_SIZE		16	/* 65536 */
#define EXT2_MIN_BLOCK_SIZE	(1 << EXT2_MIN_BLOCK_LOG_SIZE)
#define EXT2_MAX_BLOCK_SIZE	(1 << EXT2_MAX_BLOCK_LOG_SIZE)
#ifdef __KERNEL__
#define EXT2_BLOCK_SIZE(s)	((s)->s_blocksize)
#define EXT2_BLOCK_SIZE_BITS(s)	((s)->s_blocksize_bits)
#define EXT2_ADDR_PER_BLOCK_BITS(s)	(EXT2_SB(s)->addr_per_block_bits)
#define EXT2_INODE_SIZE(s)	(EXT2_SB(s)->s_inode_size)
#define EXT2_FIRST_INO(s)	(EXT2_SB(s)->s_first_ino)
#else
#define EXT2_BLOCK_SIZE(s)	(EXT2_MIN_BLOCK_SIZE << (s)->s_log_block_size)
#define EXT2_BLOCK_SIZE_BITS(s)	((s)->s_log_block_size + 10)
#define EXT2_INODE_SIZE(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_INODE_SIZE : (s)->s_inode_size)
#define EXT2_FIRST_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 EXT2_GOOD_OLD_FIRST_INO : (s)->s_first_ino)
#endif
#define EXT2_ADDR_PER_BLOCK(s)	(EXT2_BLOCK_SIZE(s) / sizeof(__u32))

/*
 * Macro-instructions used to manage allocation clusters
 */
#define EXT2_MIN_CLUSTER_LOG_SIZE	EXT2_MIN_BLOCK_LOG_SIZE
#define EXT2_MAX_CLUSTER_LOG_SIZE	29	/* 512MB  */
#define EXT2_MIN_CLUSTER_SIZE		EXT2_MIN_BLOCK_SIZE
#define EXT2_MAX_CLUSTER_SIZE		(1 << EXT2_MAX_CLUSTER_LOG_SIZE)
#define EXT2_CLUSTER_SIZE(s)		(EXT2_MIN_BLOCK_SIZE << \
						(s)->s_log_cluster_size)
#define EXT2_CLUSTER_SIZE_BITS(s)	((s)->s_log_cluster_size + 10)

/*
 * Macro-instructions used to manage fragments
 *
 * Note: for backwards compatibility only, for the dump program.
 * Ext2/3/4 will never support fragments....
 */
#define EXT2_MIN_FRAG_SIZE              EXT2_MIN_BLOCK_SIZE
#define EXT2_MAX_FRAG_SIZE              EXT2_MAX_BLOCK_SIZE
#define EXT2_MIN_FRAG_LOG_SIZE          EXT2_MIN_BLOCK_LOG_SIZE
#define EXT2_FRAG_SIZE(s)		EXT2_BLOCK_SIZE(s)
#define EXT2_FRAGS_PER_BLOCK(s)		1

/*
 * ACL structures
 */
struct ext2_acl_header	/* Header of Access Control Lists */
{
	__u32	aclh_size;
	__u32	aclh_file_count;
	__u32	aclh_acle_count;
	__u32	aclh_first_acle;
};

struct ext2_acl_entry	/* Access Control List Entry */
{
	__u32	acle_size;
	__u16	acle_perms;	/* Access permissions */
	__u16	acle_type;	/* Type of entry */
	__u16	acle_tag;	/* User or group identity */
	__u16	acle_pad1;
	__u32	acle_next;	/* Pointer on next entry for the */
					/* same inode or on next free entry */
};

/*
 * Structure of a blocks group descriptor
 */
struct ext2_group_desc
{
	__u32	bg_block_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_table;		/* Inodes table block */
	__u16	bg_free_blocks_count;	/* Free blocks count */
	__u16	bg_free_inodes_count;	/* Free inodes count */
	__u16	bg_used_dirs_count;	/* Directories count */
	__u16	bg_flags;
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_block_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_unused;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(s_uuid+group_num+group_desc)*/
};

/*
 * Structure of a blocks group descriptor
 */
struct ext4_group_desc
{
	__u32	bg_block_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_table;		/* Inodes table block */
	__u16	bg_free_blocks_count;	/* Free blocks count */
	__u16	bg_free_inodes_count;	/* Free inodes count */
	__u16	bg_used_dirs_count;	/* Directories count */
	__u16	bg_flags;		/* EXT4_BG_flags (INODE_UNINIT, etc) */
	__u32	bg_exclude_bitmap_lo;	/* Exclude bitmap for snapshots */
	__u16	bg_block_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
	__u16	bg_itable_unused;	/* Unused inodes count */
	__u16	bg_checksum;		/* crc16(sb_uuid+group+desc) */
	__u32	bg_block_bitmap_hi;	/* Blocks bitmap block MSB */
	__u32	bg_inode_bitmap_hi;	/* Inodes bitmap block MSB */
	__u32	bg_inode_table_hi;	/* Inodes table block MSB */
	__u16	bg_free_blocks_count_hi;/* Free blocks count MSB */
	__u16	bg_free_inodes_count_hi;/* Free inodes count MSB */
	__u16	bg_used_dirs_count_hi;	/* Directories count MSB */
	__u16	bg_itable_unused_hi;	/* Unused inodes count MSB */
	__u32	bg_exclude_bitmap_hi;	/* Exclude bitmap block MSB */
	__u16	bg_block_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u16	bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
	__u32	bg_reserved;
};

#define EXT4_BG_INODE_BITMAP_CSUM_HI_END	\
	(offsetof(struct ext4_group_desc, bg_inode_bitmap_csum_hi) + \
	 sizeof(__u16))
#define EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION	\
	(offsetof(struct ext4_group_desc, bg_block_bitmap_csum_hi) + \
	 sizeof(__u16))

#define EXT2_BG_INODE_UNINIT	0x0001 /* Inode table/bitmap not initialized */
#define EXT2_BG_BLOCK_UNINIT	0x0002 /* Block bitmap not initialized */
#define EXT2_BG_INODE_ZEROED	0x0004 /* On-disk itable initialized to zero */

/*
 * Data structures used by the directory indexing feature
 *
 * Note: all of the multibyte integer fields are little endian.
 */

/*
 * Note: dx_root_info is laid out so that if it should somehow get
 * overlaid by a dirent the two low bits of the hash version will be
 * zero.  Therefore, the hash version mod 4 should never be 0.
 * Sincerely, the paranoia department.
 */
struct ext2_dx_root_info {
	__u32 reserved_zero;
	__u8 hash_version; /* 0 now, 1 at release */
	__u8 info_length; /* 8 */
	__u8 indirect_levels;
	__u8 unused_flags;
};

#define EXT2_HASH_LEGACY		0
#define EXT2_HASH_HALF_MD4		1
#define EXT2_HASH_TEA			2
#define EXT2_HASH_LEGACY_UNSIGNED	3 /* reserved for userspace lib */
#define EXT2_HASH_HALF_MD4_UNSIGNED	4 /* reserved for userspace lib */
#define EXT2_HASH_TEA_UNSIGNED		5 /* reserved for userspace lib */

#define EXT2_HASH_FLAG_INCOMPAT	0x1

struct ext2_dx_entry {
	__le32 hash;
	__le32 block;
};

struct ext2_dx_countlimit {
	__le16 limit;
	__le16 count;
};

/*
 * This goes at the end of each htree block.
 */
struct ext2_dx_tail {
	__le32 dt_reserved;
	__le32 dt_checksum;	/* crc32c(uuid+inum+dxblock) */
};

/*
 * Macro-instructions used to manage group descriptors
 */
#define EXT2_MIN_DESC_SIZE             32
#define EXT2_MIN_DESC_SIZE_64BIT       64
#define EXT2_MAX_DESC_SIZE             EXT2_MIN_BLOCK_SIZE
#define EXT2_DESC_SIZE(s)                                                \
       ((EXT2_SB(s)->s_feature_incompat & EXT4_FEATURE_INCOMPAT_64BIT) ? \
	(s)->s_desc_size : EXT2_MIN_DESC_SIZE)

#define EXT2_BLOCKS_PER_GROUP(s)	(EXT2_SB(s)->s_blocks_per_group)
#define EXT2_INODES_PER_GROUP(s)	(EXT2_SB(s)->s_inodes_per_group)
#define EXT2_CLUSTERS_PER_GROUP(s)	(EXT2_SB(s)->s_clusters_per_group)
#define EXT2_INODES_PER_BLOCK(s)	(EXT2_BLOCK_SIZE(s)/EXT2_INODE_SIZE(s))
/* limits imposed by 16-bit value gd_free_{blocks,inode}_count */
#define EXT2_MAX_BLOCKS_PER_GROUP(s)	((((unsigned) 1 << 16) - 8) *	\
					 (EXT2_CLUSTER_SIZE(s) / \
					  EXT2_BLOCK_SIZE(s)))
#define EXT2_MAX_CLUSTERS_PER_GROUP(s)	(((unsigned) 1 << 16) - 8)
#define EXT2_MAX_INODES_PER_GROUP(s)	(((unsigned) 1 << 16) - \
					 EXT2_INODES_PER_BLOCK(s))
#ifdef __KERNEL__
#define EXT2_DESC_PER_BLOCK(s)		(EXT2_SB(s)->s_desc_per_block)
#define EXT2_DESC_PER_BLOCK_BITS(s)	(EXT2_SB(s)->s_desc_per_block_bits)
#else
#define EXT2_DESC_PER_BLOCK(s)		(EXT2_BLOCK_SIZE(s) / EXT2_DESC_SIZE(s))
#endif

#define EXT2_GROUPS_TO_BLOCKS(s, g)   ((blk64_t) EXT2_BLOCKS_PER_GROUP(s) * \
				       (g))
#define EXT2_GROUPS_TO_CLUSTERS(s, g) ((blk64_t) EXT2_CLUSTERS_PER_GROUP(s) * \
				       (g))

/*
 * Constants relative to the data blocks
 */
#define EXT2_NDIR_BLOCKS		12
#define EXT2_IND_BLOCK			EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK			(EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK			(EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS			(EXT2_TIND_BLOCK + 1)

/*
 * Inode flags
 */
#define EXT2_SECRM_FL			0x00000001 /* Secure deletion */
#define EXT2_UNRM_FL			0x00000002 /* Undelete */
#define EXT2_COMPR_FL			0x00000004 /* Compress file */
#define EXT2_SYNC_FL			0x00000008 /* Synchronous updates */
#define EXT2_IMMUTABLE_FL		0x00000010 /* Immutable file */
#define EXT2_APPEND_FL			0x00000020 /* writes to file may only append */
#define EXT2_NODUMP_FL			0x00000040 /* do not dump file */
#define EXT2_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#define EXT2_DIRTY_FL			0x00000100
#define EXT2_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#define EXT2_NOCOMPR_FL			0x00000400 /* Access raw compressed data */
#define EXT2_ECOMPR_FL			0x00000800 /* Compression error */
/* End compression flags --- maybe not all used */
#define EXT2_BTREE_FL			0x00001000 /* btree format dir */
#define EXT2_INDEX_FL			0x00001000 /* hash-indexed directory */
#define EXT2_IMAGIC_FL			0x00002000
#define EXT3_JOURNAL_DATA_FL		0x00004000 /* file data should be journaled */
#define EXT2_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#define EXT2_DIRSYNC_FL 		0x00010000 /* Synchronous directory modifications */
#define EXT2_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#define EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#define EXT4_EXTENTS_FL 		0x00080000 /* Inode uses extents */
#define EXT4_EA_INODE_FL	        0x00200000 /* Inode used for large EA */
/* EXT4_EOFBLOCKS_FL 0x00400000 was here */
#define FS_NOCOW_FL			0x00800000 /* Do not cow file */
#define EXT4_SNAPFILE_FL		0x01000000  /* Inode is a snapshot */
#define EXT4_SNAPFILE_DELETED_FL	0x04000000  /* Snapshot is being deleted */
#define EXT4_SNAPFILE_SHRUNK_FL		0x08000000  /* Snapshot shrink has completed */
#define EXT4_INLINE_DATA_FL		0x10000000 /* Inode has inline data */
#define EXT2_RESERVED_FL		0x80000000 /* reserved for ext2 lib */

#define EXT2_FL_USER_VISIBLE		0x004BDFFF /* User visible flags */
#define EXT2_FL_USER_MODIFIABLE		0x004B80FF /* User modifiable flags */

/*
 * ioctl commands
 */

/* Used for online resize */
struct ext2_new_group_input {
	__u32 group;		/* Group number for this data */
	__u32 block_bitmap;	/* Absolute block number of block bitmap */
	__u32 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u32 inode_table;	/* Absolute block number of inode table start */
	__u32 blocks_count;	/* Total number of blocks in this group */
	__u16 reserved_blocks;	/* Number of reserved blocks in this group */
	__u16 unused;		/* Number of reserved GDT blocks in group */
};

struct ext4_new_group_input {
	__u32 group;		/* Group number for this data */
	__u64 block_bitmap;	/* Absolute block number of block bitmap */
	__u64 inode_bitmap;	/* Absolute block number of inode bitmap */
	__u64 inode_table;	/* Absolute block number of inode table start */
	__u32 blocks_count;	/* Total number of blocks in this group */
	__u16 reserved_blocks;	/* Number of reserved blocks in this group */
	__u16 unused;
};

#ifdef __GNU__			/* Needed for the Hurd */
#define _IOT_ext2_new_group_input _IOT (_IOTS(__u32), 5, _IOTS(__u16), 2, 0, 0)
#endif

#define EXT2_IOC_GETFLAGS		_IOR('f', 1, long)
#define EXT2_IOC_SETFLAGS		_IOW('f', 2, long)
#define EXT2_IOC_GETVERSION		_IOR('v', 1, long)
#define EXT2_IOC_SETVERSION		_IOW('v', 2, long)
#define EXT2_IOC_GETVERSION_NEW		_IOR('f', 3, long)
#define EXT2_IOC_SETVERSION_NEW		_IOW('f', 4, long)
#define EXT2_IOC_GROUP_EXTEND		_IOW('f', 7, unsigned long)
#define EXT2_IOC_GROUP_ADD		_IOW('f', 8,struct ext2_new_group_input)
#define EXT4_IOC_GROUP_ADD		_IOW('f', 8,struct ext4_new_group_input)
#define EXT4_IOC_RESIZE_FS		_IOW('f', 16, __u64)

/*
 * Structure of an inode on the disk
 */

struct ext2_inode {
	__u16	i_mode;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_size;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode change time */
	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_count;	/* Links count */
	__u32	i_blocks;	/* Blocks count */
	__u32	i_flags;	/* File flags */
	union {
		struct {
			__u32	l_i_version; /* was l_i_reserved1 */
		} linux1;
		struct {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
	__u32	i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_size_high;	/* Formerly i_dir_acl, directory ACL */
	__u32	i_faddr;	/* Fragment address */
	union {
		struct {
			__u16	l_i_blocks_hi;
			__u16	l_i_file_acl_high;
			__u16	l_i_uid_high;	/* these 2 fields    */
			__u16	l_i_gid_high;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		struct {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_mode_high;
			__u16	h_i_uid_high;
			__u16	h_i_gid_high;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
	__u64	i_latitude;	/* File latitude */
	__u64	i_longitude;	/* File longitude */
	__u32	i_accuracy;	/* File location accuracy */
	__u32	i_coord_age;	/* File location age */

};

/*
 * Permanent part of an large inode on the disk
 */
struct ext2_inode_large {
	__u16	i_mode;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_size;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Inode Change time */
	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_count;	/* Links count */
	__u32	i_blocks;	/* Blocks count */
	__u32	i_flags;	/* File flags */
	union {
		struct {
			__u32	l_i_version; /* was l_i_reserved1 */
		} linux1;
		struct {
			__u32  h_i_translator;
		} hurd1;
	} osd1;				/* OS dependent 1 */
	__u32	i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_size_high;	/* Formerly i_dir_acl, directory ACL */
	__u32	i_faddr;	/* Fragment address */
	union {
		struct {
			__u16	l_i_blocks_hi;
			__u16	l_i_file_acl_high;
			__u16	l_i_uid_high;	/* these 2 fields    */
			__u16	l_i_gid_high;	/* were reserved2[0] */
			__u16	l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
			__u16	l_i_reserved;
		} linux2;
		struct {
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_mode_high;
			__u16	h_i_uid_high;
			__u16	h_i_gid_high;
			__u32	h_i_author;
		} hurd2;
	} osd2;				/* OS dependent 2 */
	__u16	i_extra_isize;
	__u16	i_checksum_hi;	/* crc32c(uuid+inum+inode) */
	__u32	i_ctime_extra;	/* extra Change time (nsec << 2 | epoch) */
	__u32	i_mtime_extra;	/* extra Modification time (nsec << 2 | epoch) */
	__u32	i_atime_extra;	/* extra Access time (nsec << 2 | epoch) */
	__u32	i_crtime;	/* File creation time */
	__u32	i_crtime_extra;	/* extra File creation time (nsec << 2 | epoch)*/
	__u32	i_version_hi;	/* high 32 bits for 64-bit version */
};

#define EXT4_INODE_CSUM_HI_EXTRA_END	\
	(offsetof(struct ext2_inode_large, i_checksum_hi) + sizeof(__u16) - \
	 EXT2_GOOD_OLD_INODE_SIZE)

#define i_dir_acl	i_size_high

#define i_checksum_lo	osd2.linux2.l_i_checksum_lo

#if defined(__KERNEL__) || defined(__linux__)
#define i_reserved1	osd1.linux1.l_i_reserved1
#define i_frag		osd2.linux2.l_i_frag
#define i_fsize		osd2.linux2.l_i_fsize
#define i_uid_low	i_uid
#define i_gid_low	i_gid
#define i_uid_high	osd2.linux2.l_i_uid_high
#define i_gid_high	osd2.linux2.l_i_gid_high
#else
#if defined(__GNU__)

#define i_translator	osd1.hurd1.h_i_translator
#define i_frag		osd2.hurd2.h_i_frag;
#define i_fsize		osd2.hurd2.h_i_fsize;
#define i_uid_high	osd2.hurd2.h_i_uid_high
#define i_gid_high	osd2.hurd2.h_i_gid_high
#define i_author	osd2.hurd2.h_i_author

#endif  /* __GNU__ */
#endif	/* defined(__KERNEL__) || defined(__linux__) */

#define inode_uid(inode)	((inode).i_uid | (inode).osd2.linux2.l_i_uid_high << 16)
#define inode_gid(inode)	((inode).i_gid | (inode).osd2.linux2.l_i_gid_high << 16)
#define ext2fs_set_i_uid_high(inode,x) ((inode).osd2.linux2.l_i_uid_high = (x))
#define ext2fs_set_i_gid_high(inode,x) ((inode).osd2.linux2.l_i_gid_high = (x))

/*
 * File system states
 */
#define EXT2_VALID_FS			0x0001	/* Unmounted cleanly */
#define EXT2_ERROR_FS			0x0002	/* Errors detected */
#define EXT3_ORPHAN_FS			0x0004	/* Orphans being recovered */

/*
 * Misc. filesystem flags
 */
#define EXT2_FLAGS_SIGNED_HASH		0x0001  /* Signed dirhash in use */
#define EXT2_FLAGS_UNSIGNED_HASH	0x0002  /* Unsigned dirhash in use */
#define EXT2_FLAGS_TEST_FILESYS		0x0004	/* OK for use on development code */
#define EXT2_FLAGS_IS_SNAPSHOT		0x0010	/* This is a snapshot image */
#define EXT2_FLAGS_FIX_SNAPSHOT		0x0020	/* Snapshot inodes corrupted */
#define EXT2_FLAGS_FIX_EXCLUDE		0x0040	/* Exclude bitmaps corrupted */

/*
 * Mount flags
 */
#define EXT2_MOUNT_CHECK		0x0001	/* Do mount-time checks */
#define EXT2_MOUNT_GRPID		0x0004	/* Create files with directory's group */
#define EXT2_MOUNT_DEBUG		0x0008	/* Some debugging messages */
#define EXT2_MOUNT_ERRORS_CONT		0x0010	/* Continue on errors */
#define EXT2_MOUNT_ERRORS_RO		0x0020	/* Remount fs ro on errors */
#define EXT2_MOUNT_ERRORS_PANIC		0x0040	/* Panic on errors */
#define EXT2_MOUNT_MINIX_DF		0x0080	/* Mimics the Minix statfs */
#define EXT2_MOUNT_NO_UID32		0x0200  /* Disable 32-bit UIDs */

#define clear_opt(o, opt)		o &= ~EXT2_MOUNT_##opt
#define set_opt(o, opt)			o |= EXT2_MOUNT_##opt
#define test_opt(sb, opt)		(EXT2_SB(sb)->s_mount_opt & \
					 EXT2_MOUNT_##opt)
/*
 * Maximal mount counts between two filesystem checks
 */
#define EXT2_DFL_MAX_MNT_COUNT		20	/* Allow 20 mounts */
#define EXT2_DFL_CHECKINTERVAL		0	/* Don't use interval check */

/*
 * Behaviour when detecting errors
 */
#define EXT2_ERRORS_CONTINUE		1	/* Continue execution */
#define EXT2_ERRORS_RO			2	/* Remount fs read-only */
#define EXT2_ERRORS_PANIC		3	/* Panic */
#define EXT2_ERRORS_DEFAULT		EXT2_ERRORS_CONTINUE

#if (__GNUC__ >= 4)
#define ext4_offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#else
#define ext4_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

/* Metadata checksum algorithms */
#define EXT2_CRC32C_CHKSUM		1

/*
 * Structure of the super block
 */
struct ext2_super_block {
	__u32	s_inodes_count;		/* Inodes count */
	__u32	s_blocks_count;		/* Blocks count */
	__u32	s_r_blocks_count;	/* Reserved blocks count */
	__u32	s_free_blocks_count;	/* Free blocks count */
	__u32	s_free_inodes_count;	/* Free inodes count */
	__u32	s_first_data_block;	/* First Data Block */
	__u32	s_log_block_size;	/* Block size */
	__u32	s_log_cluster_size;	/* Allocation cluster size */
	__u32	s_blocks_per_group;	/* # Blocks per group */
	__u32	s_clusters_per_group;	/* # Fragments per group */
	__u32	s_inodes_per_group;	/* # Inodes per group */
	__u32	s_mtime;		/* Mount time */
	__u32	s_wtime;		/* Write time */
	__u16	s_mnt_count;		/* Mount count */
	__s16	s_max_mnt_count;	/* Maximal mount count */
	__u16	s_magic;		/* Magic signature */
	__u16	s_state;		/* File system state */
	__u16	s_errors;		/* Behaviour when detecting errors */
	__u16	s_minor_rev_level;	/* minor revision level */
	__u32	s_lastcheck;		/* time of last check */
	__u32	s_checkinterval;	/* max. time between checks */
	__u32	s_creator_os;		/* OS */
	__u32	s_rev_level;		/* Revision level */
	__u16	s_def_resuid;		/* Default uid for reserved blocks */
	__u16	s_def_resgid;		/* Default gid for reserved blocks */
	/*
	 * These fields are for EXT2_DYNAMIC_REV superblocks only.
	 *
	 * Note: the difference between the compatible feature set and
	 * the incompatible feature set is that if there is a bit set
	 * in the incompatible feature set that the kernel doesn't
	 * know about, it should refuse to mount the filesystem.
	 *
	 * e2fsck's requirements are more strict; if it doesn't know
	 * about a feature in either the compatible or incompatible
	 * feature set, it must abort and not try to meddle with
	 * things it doesn't understand...
	 */
	__u32	s_first_ino;		/* First non-reserved inode */
	__u16   s_inode_size;		/* size of inode structure */
	__u16	s_block_group_nr;	/* block group # of this superblock */
	__u32	s_feature_compat;	/* compatible feature set */
	__u32	s_feature_incompat;	/* incompatible feature set */
	__u32	s_feature_ro_compat;	/* readonly-compatible feature set */
	__u8	s_uuid[16];		/* 128-bit uuid for volume */
	char	s_volume_name[16];	/* volume name */
	char	s_last_mounted[64];	/* directory where last mounted */
	__u32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
	__u16	s_reserved_gdt_blocks;	/* Per group table for online growth */
	/*
	 * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
	__u8	s_journal_uuid[16];	/* uuid of journal superblock */
	__u32	s_journal_inum;		/* inode number of journal file */
	__u32	s_journal_dev;		/* device number of journal file */
	__u32	s_last_orphan;		/* start of list of inodes to delete */
	__u32	s_hash_seed[4];		/* HTREE hash seed */
	__u8	s_def_hash_version;	/* Default hash version to use */
	__u8	s_jnl_backup_type; 	/* Default type of journal backup */
	__u16	s_desc_size;		/* Group desc. size: INCOMPAT_64BIT */
	__u32	s_default_mount_opts;
	__u32	s_first_meta_bg;	/* First metablock group */
	__u32	s_mkfs_time;		/* When the filesystem was created */
	__u32	s_jnl_blocks[17]; 	/* Backup of the journal inode */
	__u32	s_blocks_count_hi;	/* Blocks count high 32bits */
	__u32	s_r_blocks_count_hi;	/* Reserved blocks count high 32 bits*/
	__u32	s_free_blocks_hi; 	/* Free blocks count */
	__u16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__u16	s_want_extra_isize; 	/* New inodes should reserve # bytes */
	__u32	s_flags;		/* Miscellaneous flags */
	__u16   s_raid_stride;		/* RAID stride */
	__u16   s_mmp_update_interval;  /* # seconds to wait in MMP checking */
	__u64   s_mmp_block;            /* Block for multi-mount protection */
	__u32   s_raid_stripe_width;    /* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_per_flex;	/* FLEX_BG group size */
	__u8    s_checksum_type;	/* metadata checksum algorithm */
	__u16	s_reserved_pad;		/* Padding to next 32bits */
	__u64	s_kbytes_written;	/* nr of lifetime kilobytes written */
	__u32	s_snapshot_inum;	/* Inode number of active snapshot */
	__u32	s_snapshot_id;		/* sequential ID of active snapshot */
	__u64	s_snapshot_r_blocks_count; /* reserved blocks for active
					      snapshot's future use */
	__u32	s_snapshot_list;	/* inode number of the head of the on-disk snapshot list */
#define EXT4_S_ERR_START ext4_offsetof(struct ext2_super_block, s_error_count)
	__u32	s_error_count;		/* number of fs errors */
	__u32	s_first_error_time;	/* first time an error happened */
	__u32	s_first_error_ino;	/* inode involved in first error */
	__u64	s_first_error_block;	/* block involved of first error */
	__u8	s_first_error_func[32];	/* function where the error happened */
	__u32	s_first_error_line;	/* line number where error happened */
	__u32	s_last_error_time;	/* most recent time of an error */
	__u32	s_last_error_ino;	/* inode involved in last error */
	__u32	s_last_error_line;	/* line number where error happened */
	__u64	s_last_error_block;	/* block involved of last error */
	__u8	s_last_error_func[32];	/* function where the error happened */
#define EXT4_S_ERR_END ext4_offsetof(struct ext2_super_block, s_mount_opts)
	__u8	s_mount_opts[64];
	__u32	s_usr_quota_inum;	/* inode number of user quota file */
	__u32	s_grp_quota_inum;	/* inode number of group quota file */
	__u32	s_overhead_blocks;	/* overhead blocks/clusters in fs */
	__u32	s_backup_bgs[2];	/* If sparse_super2 enabled */
	__u32   s_reserved[106];        /* Padding to the end of the block */
	__u32	s_checksum;		/* crc32c(superblock) */
};

#define EXT4_S_ERR_LEN (EXT4_S_ERR_END - EXT4_S_ERR_START)

/*
 * Codes for operating systems
 */
#define EXT2_OS_LINUX		0
#define EXT2_OS_HURD		1
#define EXT2_OBSO_OS_MASIX	2
#define EXT2_OS_FREEBSD		3
#define EXT2_OS_LITES		4

/*
 * Revision levels
 */
#define EXT2_GOOD_OLD_REV	0	/* The good old (original) format */
#define EXT2_DYNAMIC_REV	1	/* V2 format w/ dynamic inode sizes */

#define EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV
#define EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV

#define EXT2_GOOD_OLD_INODE_SIZE 128

/*
 * Journal inode backup types
 */
#define EXT3_JNL_BACKUP_BLOCKS	1

/*
 * Feature set definitions
 */

#define EXT2_HAS_COMPAT_FEATURE(sb,mask)			\
	( EXT2_SB(sb)->s_feature_compat & (mask) )
#define EXT2_HAS_RO_COMPAT_FEATURE(sb,mask)			\
	( EXT2_SB(sb)->s_feature_ro_compat & (mask) )
#define EXT2_HAS_INCOMPAT_FEATURE(sb,mask)			\
	( EXT2_SB(sb)->s_feature_incompat & (mask) )

#define EXT2_FEATURE_COMPAT_DIR_PREALLOC	0x0001
#define EXT2_FEATURE_COMPAT_IMAGIC_INODES	0x0002
#define EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
#define EXT2_FEATURE_COMPAT_EXT_ATTR		0x0008
#define EXT2_FEATURE_COMPAT_RESIZE_INODE	0x0010
#define EXT2_FEATURE_COMPAT_DIR_INDEX		0x0020
#define EXT2_FEATURE_COMPAT_LAZY_BG		0x0040
/* #define EXT2_FEATURE_COMPAT_EXCLUDE_INODE	0x0080 not used, legacy */
#define EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP	0x0100
#define EXT4_FEATURE_COMPAT_SPARSE_SUPER2	0x0200


#define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#define EXT2_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR	0x0004 not used */
#define EXT4_FEATURE_RO_COMPAT_HUGE_FILE	0x0008
#define EXT4_FEATURE_RO_COMPAT_GDT_CSUM		0x0010
#define EXT4_FEATURE_RO_COMPAT_DIR_NLINK	0x0020
#define EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE	0x0040
#define EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT	0x0080
#define EXT4_FEATURE_RO_COMPAT_QUOTA		0x0100
#define EXT4_FEATURE_RO_COMPAT_BIGALLOC		0x0200
/*
 * METADATA_CSUM implies GDT_CSUM.  When METADATA_CSUM is set, group
 * descriptor checksums use the same algorithm as all other data
 * structures' checksums.
 */
#define EXT4_FEATURE_RO_COMPAT_METADATA_CSUM	0x0400
#define EXT4_FEATURE_RO_COMPAT_REPLICA		0x0800

#define EXT2_FEATURE_INCOMPAT_COMPRESSION	0x0001
#define EXT2_FEATURE_INCOMPAT_FILETYPE		0x0002
#define EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 /* Needs recovery */
#define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 /* Journal device */
#define EXT2_FEATURE_INCOMPAT_META_BG		0x0010
#define EXT3_FEATURE_INCOMPAT_EXTENTS		0x0040
#define EXT4_FEATURE_INCOMPAT_64BIT		0x0080
#define EXT4_FEATURE_INCOMPAT_MMP		0x0100
#define EXT4_FEATURE_INCOMPAT_FLEX_BG		0x0200
#define EXT4_FEATURE_INCOMPAT_EA_INODE		0x0400
#define EXT4_FEATURE_INCOMPAT_DIRDATA		0x1000
/* 0x2000 was EXT4_FEATURE_INCOMPAT_BG_USE_META_CSUM but this was never used */
#define EXT4_FEATURE_INCOMPAT_LARGEDIR		0x4000 /* >2GB or 3-lvl htree */
#define EXT4_FEATURE_INCOMPAT_INLINE_DATA	0x8000 /* data in inode */

#define EXT2_FEATURE_COMPAT_SUPP	0
#define EXT2_FEATURE_INCOMPAT_SUPP    (EXT2_FEATURE_INCOMPAT_FILETYPE| \
				       EXT4_FEATURE_INCOMPAT_MMP)
#define EXT2_FEATURE_RO_COMPAT_SUPP	(EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
					 EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \
					 EXT4_FEATURE_RO_COMPAT_DIR_NLINK| \
					 EXT2_FEATURE_RO_COMPAT_BTREE_DIR)

/*
 * Default values for user and/or group using reserved blocks
 */
#define EXT2_DEF_RESUID		0
#define EXT2_DEF_RESGID		0

/*
 * Default mount options
 */
#define EXT2_DEFM_DEBUG		0x0001
#define EXT2_DEFM_BSDGROUPS	0x0002
#define EXT2_DEFM_XATTR_USER	0x0004
#define EXT2_DEFM_ACL		0x0008
#define EXT2_DEFM_UID16		0x0010
#define EXT3_DEFM_JMODE		0x0060
#define EXT3_DEFM_JMODE_DATA	0x0020
#define EXT3_DEFM_JMODE_ORDERED	0x0040
#define EXT3_DEFM_JMODE_WBACK	0x0060
#define EXT4_DEFM_NOBARRIER	0x0100
#define EXT4_DEFM_BLOCK_VALIDITY 0x0200
#define EXT4_DEFM_DISCARD	0x0400
#define EXT4_DEFM_NODELALLOC	0x0800

/*
 * Structure of a directory entry
 */
#define EXT2_NAME_LEN 255

struct ext2_dir_entry {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u16	name_len;		/* Name length */
	char	name[EXT2_NAME_LEN];	/* File name */
};

/*
 * The new version of the directory entry.  Since EXT2 structures are
 * stored in intel byte order, and the name_len field could never be
 * bigger than 255 chars, it's safe to reclaim the extra byte for the
 * file_type field.
 *
 * This structure is deprecated due to endianity issues. Please use struct
 * ext2_dir_entry and accessor functions
 *   ext2fs_dirent_name_len
 *   ext2fs_dirent_set_name_len
 *   ext2fs_dirent_file_type
 *   ext2fs_dirent_set_file_type
 * to get and set name_len and file_type fields.
 */
struct ext2_dir_entry_2 {
	__u32	inode;			/* Inode number */
	__u16	rec_len;		/* Directory entry length */
	__u8	name_len;		/* Name length */
	__u8	file_type;
	char	name[EXT2_NAME_LEN];	/* File name */
};

/*
 * This is a bogus directory entry at the end of each leaf block that
 * records checksums.
 */
struct ext2_dir_entry_tail {
	__u32	det_reserved_zero1;	/* Pretend to be unused */
	__u16	det_rec_len;		/* 12 */
	__u16	det_reserved_name_len;	/* 0xDE00, fake namelen/filetype */
	__u32	det_checksum;		/* crc32c(uuid+inode+dirent) */
};

/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
#define EXT2_FT_UNKNOWN		0
#define EXT2_FT_REG_FILE	1
#define EXT2_FT_DIR		2
#define EXT2_FT_CHRDEV		3
#define EXT2_FT_BLKDEV		4
#define EXT2_FT_FIFO		5
#define EXT2_FT_SOCK		6
#define EXT2_FT_SYMLINK		7

#define EXT2_FT_MAX		8

/*
 * Annoyingly, e2fsprogs always swab16s ext2_dir_entry.name_len, so we
 * have to build ext2_dir_entry_tail with that assumption too.  This
 * constant helps to build the dir_entry_tail to look like it has an
 * "invalid" file type.
 */
#define EXT2_DIR_NAME_LEN_CSUM	0xDE00

/*
 * EXT2_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
#define EXT2_DIR_PAD			4
#define EXT2_DIR_ROUND			(EXT2_DIR_PAD - 1)
#define EXT2_DIR_REC_LEN(name_len)	(((name_len) + 8 + EXT2_DIR_ROUND) & \
					 ~EXT2_DIR_ROUND)

/*
 * This structure is used for multiple mount protection. It is written
 * into the block number saved in the s_mmp_block field in the superblock.
 * Programs that check MMP should assume that if SEQ_FSCK (or any unknown
 * code above SEQ_MAX) is present then it is NOT safe to use the filesystem,
 * regardless of how old the timestamp is.
 *
 * The timestamp in the MMP structure will be updated by e2fsck at some
 * arbitary intervals (start of passes, after every few groups of inodes
 * in pass1 and pass1b).  There is no guarantee that e2fsck is updating
 * the MMP block in a timely manner, and the updates it does are purely
 * for the convenience of the sysadmin and not for automatic validation.
 *
 * Note: Only the mmp_seq value is used to determine whether the MMP block
 *	is being updated.  The mmp_time, mmp_nodename, and mmp_bdevname
 *	fields are only for informational purposes for the administrator,
 *	due to clock skew between nodes and hostname HA service takeover.
 */
#define EXT4_MMP_MAGIC     0x004D4D50U /* ASCII for MMP */
#define EXT4_MMP_SEQ_CLEAN 0xFF4D4D50U /* mmp_seq value for clean unmount */
#define EXT4_MMP_SEQ_FSCK  0xE24D4D50U /* mmp_seq value when being fscked */
#define EXT4_MMP_SEQ_MAX   0xE24D4D4FU /* maximum valid mmp_seq value */

/* Not endian-annotated; it's swapped at read/write time */
struct mmp_struct {
	__u32	mmp_magic;		/* Magic number for MMP */
	__u32	mmp_seq;		/* Sequence no. updated periodically */
	__u64	mmp_time;		/* Time last updated */
	char	mmp_nodename[64];	/* Node which last updated MMP block */
	char	mmp_bdevname[32];	/* Bdev which last updated MMP block */
	__u16	mmp_check_interval;	/* Changed mmp_check_interval */
	__u16	mmp_pad1;
	__u32	mmp_pad2[226];
	__u32	mmp_checksum;		/* crc32c(uuid+mmp_block) */
};

/*
 * Default interval for MMP update in seconds.
 */
#define EXT4_MMP_UPDATE_INTERVAL	5

/*
 * Maximum interval for MMP update in seconds.
 */
#define EXT4_MMP_MAX_UPDATE_INTERVAL	300

/*
 * Minimum interval for MMP checking in seconds.
 */
#define EXT4_MMP_MIN_CHECK_INTERVAL     5

/*
 * Minimum size of inline data.
 */
#define EXT4_MIN_INLINE_DATA_SIZE	((sizeof(__u32) * EXT2_N_BLOCKS))

/*
 * Size of a parent inode in inline data directory.
 */
#define EXT4_INLINE_DATA_DOTDOT_SIZE	(4)

#endif	/* _LINUX_EXT2_FS_H */
