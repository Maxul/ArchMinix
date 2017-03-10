/**
 * @file
 *
 * @brief RTEMS File System Information Node
 *
 * @ingroup rtems_rfs
 *
 * RTEMS File System Information Node.
 *
 * The information nodes hold the data about all nodes in the file system.
 */

/*
 *  COPYRIGHT (c) 2010 Chris Johns <chrisj@rtems.org>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */


#if !defined (_RTEMS_RFS_INODE_H_)
#define _RTEMS_RFS_INODE_H_

#include <sys/stat.h>

#include <rtems/rfs/rtems-rfs-data.h>
#include <rtems/rfs/rtems-rfs-file-system.h>

/**
 * The RFS mode definitions. Currently map to the C library ones.
 */
#define RTEMS_RFS_S_ISUID  S_ISUID  /**< Set user id on execution */
#define RTEMS_RFS_S_ISGID  S_ISGID  /**< Set group id on execution */
#define RTEMS_RFS_S_ISVTX  S_ISVTX  /**< Save swapped text even after use */
#define RTEMS_RFS_S_IREAD  S_IREAD  /**< Read permission, owner */
#define RTEMS_RFS_S_IWRITE S_IWRITE /**< Write permission, owner */
#define RTEMS_RFS_S_IEXEC  S_IEXEC  /**< Execute/search permission, owner */
#define RTEMS_RFS_S_ENFMT  S_ENFMT  /**< Enforcement-mode locking */
#define RTEMS_RFS_S_IFMT   S_IFMT   /**< Type of file */
#define RTEMS_RFS_S_IFDIR  S_IFDIR  /**< Directory */
#define RTEMS_RFS_S_IFCHR  S_IFCHR  /**< Character special */
#define RTEMS_RFS_S_IFBLK  S_IFBLK  /**< Block special */
#define RTEMS_RFS_S_IFREG  S_IFREG  /**< Regular */
#define RTEMS_RFS_S_IFLNK  S_IFLNK  /**< Symbolic link */
#define RTEMS_RFS_S_IFSOCK S_IFSOCK /**< Socket */
#define RTEMS_RFS_S_IFIFO  S_IFIFO  /**< Fifo */
#define RTEMS_RFS_S_IRWXU  S_IRWXU
#define RTEMS_RFS_S_IRUSR  S_IRUSR  /**< Read permission, owner */
#define RTEMS_RFS_S_IWUSR  S_IWUSR  /**< Write permission, owner */
#define RTEMS_RFS_S_IXUSR  S_IXUSR  /**< Execute/search permission, owner */
#define RTEMS_RFS_S_IRWXG  S_IRWXG
#define RTEMS_RFS_S_IRGRP  S_IRGRP  /**< Read permission, group */
#define RTEMS_RFS_S_IWGRP  S_IWGRP  /**< Write permission, grougroup */
#define RTEMS_RFS_S_IXGRP  S_IXGRP  /**< Execute/search permission, group */
#define RTEMS_RFS_S_IRWXO  S_IRWXO
#define RTEMS_RFS_S_IROTH  S_IROTH  /**< Read permission, other */
#define RTEMS_RFS_S_IWOTH  S_IWOTH  /**< Write permission, other */
#define RTEMS_RFS_S_IXOTH  S_IXOTH  /**< Execute/search permission, other */

#define	RTEMS_RFS_S_ISBLK(m)  S_ISBLK(m)
#define	RTEMS_RFS_S_ISCHR(m)  S_ISCHR(m)
#define	RTEMS_RFS_S_ISDIR(m)  S_ISDIR(m)
#define	RTEMS_RFS_S_ISFIFO(m) S_ISFIFO(m)
#define	RTEMS_RFS_S_ISREG(m)  S_ISREG(m)
#define	RTEMS_RFS_S_ISLNK(m)  S_ISLNK(m)
#define	RTEMS_RFS_S_ISSOCK(m) S_ISSOCK(m)

/**
 * Permissions of a symlink.
 */
#define RTEMS_RFS_S_SYMLINK \
  RTEMS_RFS_S_IFLNK | RTEMS_RFS_S_IRWXU | RTEMS_RFS_S_IRWXG | RTEMS_RFS_S_IRWXO

/**
 * The inode number or ino.
 */
typedef uint32_t rtems_rfs_ino;

/**
 * The time in the file system.
 */
typedef uint32_t rtems_rfs_time;

/**
 * The size of a block value on disk. This include the inodes and indirect
 * tables.
 */
typedef uint32_t rtems_rfs_inode_block;

/**
 * The size of the data name field in the inode.
 */
#define RTEMS_RFS_INODE_DATA_NAME_SIZE \
  (RTEMS_RFS_INODE_BLOCKS * sizeof (rtems_rfs_inode_block))

/**
 * The inode.
 */
typedef struct _rtems_rfs_inode
{
  /**
   * The number of links to the inode.
   */
  uint16_t links;

  /**
   * The mode of the node.
   */
  uint16_t mode;

  /**
   * The owner of the node.
   */
  uint32_t owner;

  /**
   * Reserved.
   */
  uint16_t flags;

  /**
   * Amount of data held in the last block data.
   */
  uint16_t block_offset;

  /**
   * Number of blocks held by this file.
   */
  uint32_t block_count;

  /**
   * The access time. The last time the file was read.
   */
  rtems_rfs_time atime;

  /**
   * The modified time. The last time the file was written too.
   */
  rtems_rfs_time mtime;

  /**
   * The change time. The last time the inode was written too.
   */
  rtems_rfs_time ctime;

  /**
   * Blocks. These are the block numbers used by the node or table of
   * nodes. The flags indicate the mode the blocks are being held in. In the
   * direct table mode the blocks are entries in this table. In the indirect
   * mode the blocks point to blocks that hold the block numbers. The data can
   * also be a name if it fits. For example a symbolic link.
   */
  union
  {
    rtems_rfs_inode_block blocks[RTEMS_RFS_INODE_BLOCKS];
    uint8_t               name[RTEMS_RFS_INODE_DATA_NAME_SIZE];
  } data;

  /**
   * The last block map block. Used as the goal when allocating a new block for
   * use in the map.
   */
  rtems_rfs_inode_block last_map_block;

  /**
   * The last data block. Used as the goal when allocating a new block.
   */
  rtems_rfs_inode_block last_data_block;

} rtems_rfs_inode;

/**
 * The size of an inode.
 */
#define RTEMS_RFS_INODE_SIZE (sizeof (rtems_rfs_inode))

/**
 * RFS Inode Handle.
 */
typedef struct _rtems_rfs_inode_handle
{
  /**
   * Handles can be linked as a list for easy processing.
   */
  rtems_chain_node link;

  /**
   * The ino for this handle.
   */
  rtems_rfs_ino ino;

  /**
   * The pointer to the inode.
   */
  rtems_rfs_inode* node;

  /**
   * The buffer that contains this inode.
   */
  rtems_rfs_buffer_handle buffer;

  /**
   * The block number that holds the inode.
   */
  rtems_rfs_buffer_block block;

  /**
   * The offset into the block for the inode.
   */
  int offset;

  /**
   * Number of load requests.
   */
  int loads;

} rtems_rfs_inode_handle;

/**
 * Is the inode loaded ?
 */
#define rtems_rfs_inode_is_loaded(_h) ((_h)->node)

/**
 * Get the inode ino for a handle.
 */
#define rtems_rfs_inode_ino(_h) ((_h)->ino)

/**
 * Get the link count.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval links The link count.
 */
static inline uint16_t
rtems_rfs_inode_get_links (rtems_rfs_inode_handle* handle)
{
  uint16_t links;
  links = rtems_rfs_read_u16 (&handle->node->links);
  if (links == 0xffff)
    links = 0;
  return links;
}

/**
 * Set the link count.
 *
 * @param[in] handle is the inode handle.
 * @param[in] links are the links.
 */
static inline void
rtems_rfs_inode_set_links (rtems_rfs_inode_handle* handle, uint16_t links)
{
  rtems_rfs_write_u16 (&handle->node->links, links);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the flags.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval flags The flags.
 */
static inline uint16_t
rtems_rfs_inode_get_flags (rtems_rfs_inode_handle* handle)
{
  return rtems_rfs_read_u16 (&handle->node->flags);
}

/**
 * Set the flags.
 *
 * @param[in] handle is the inode handle.
 * @param[in] flags are the flags.
 */
static inline void
rtems_rfs_inode_set_flags (rtems_rfs_inode_handle* handle, uint16_t flags)
{
  rtems_rfs_write_u16 (&handle->node->flags, flags);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the mode.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval mode The mode.
 */
static inline uint16_t
rtems_rfs_inode_get_mode (rtems_rfs_inode_handle* handle)
{
  return rtems_rfs_read_u16 (&handle->node->mode);
}

/**
 * Set the mode.
 *
 * @param[in] handle is the inode handle.
 * @param[in] mode is the mode.
 */
static inline void
rtems_rfs_inode_set_mode (rtems_rfs_inode_handle* handle, uint16_t mode)
{
  rtems_rfs_write_u16 (&handle->node->mode, mode);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the user id.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval uid The used id.
 */
static inline uint16_t
rtems_rfs_inode_get_uid (rtems_rfs_inode_handle* handle)
{
  return rtems_rfs_read_u32 (&handle->node->owner) & 0xffff;
}

/**
 * Get the group id.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval gid The grpup id.
 */
static inline uint16_t
rtems_rfs_inode_get_gid (rtems_rfs_inode_handle* handle)
{
  return (rtems_rfs_read_u32 (&handle->node->owner) >> 16) & 0xffff;
}

/**
 * Set the user id and group id.
 *
 * @param[in] handle is the inode handle.
 * @param[in] uid is the user id (uid).
 * @param[in] gid is the group id (gid).
 */
static inline void
rtems_rfs_inode_set_uid_gid (rtems_rfs_inode_handle* handle,
                             uint16_t uid, uint16_t gid)
{
  rtems_rfs_write_u32 (&handle->node->owner, (((uint32_t) gid) << 16) | uid);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the block offset.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval offset The block offset.
 */
static inline uint16_t
rtems_rfs_inode_get_block_offset (rtems_rfs_inode_handle* handle)
{
  return rtems_rfs_read_u16 (&handle->node->block_offset);
}

/**
 * Set the block offset.
 *
 * @param[in] handle is the inode handle.
 * @param[in] block_count is the block offset.
 */
static inline void
rtems_rfs_inode_set_block_offset (rtems_rfs_inode_handle* handle,
                                  uint16_t                block_offset)
{
  rtems_rfs_write_u16 (&handle->node->block_offset, block_offset);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the block count.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval count The block count.
 */
static inline uint32_t
rtems_rfs_inode_get_block_count (rtems_rfs_inode_handle* handle)
{
  return rtems_rfs_read_u32 (&handle->node->block_count);
}

/**
 * Set the block count.
 *
 * @param[in] handle is the inode handle.
 * @param[in] block_count is the block count.
 */
static inline void
rtems_rfs_inode_set_block_count (rtems_rfs_inode_handle* handle, uint32_t block_count)
{
  rtems_rfs_write_u32 (&handle->node->block_count, block_count);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the atime.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval atime The atime.
 */
static inline rtems_rfs_time
rtems_rfs_inode_get_atime (rtems_rfs_inode_handle* handle)
{
  return rtems_rfs_read_u32 (&handle->node->atime);
}

/**
 * Set the atime.
 *
 * @param[in] handle is the inode handle.
 * @param[in] atime The atime.
 */
static inline void
rtems_rfs_inode_set_atime (rtems_rfs_inode_handle* handle,
                           rtems_rfs_time          atime)
{
  rtems_rfs_write_u32 (&handle->node->atime, atime);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the mtime.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval mtime The mtime.
 */
static inline rtems_rfs_time
rtems_rfs_inode_get_mtime (rtems_rfs_inode_handle* handle)
{
  return rtems_rfs_read_u32 (&handle->node->mtime);
}

/**
 * Set the mtime.
 *
 * @param[in] handle is the inode handle.
 * @param[in] mtime The mtime.
 */
static inline void
rtems_rfs_inode_set_mtime (rtems_rfs_inode_handle* handle,
                           rtems_rfs_time          mtime)
{
  rtems_rfs_write_u32 (&handle->node->mtime, mtime);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the ctime.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval ctime The ctime.
 */
static inline rtems_rfs_time
rtems_rfs_inode_get_ctime (rtems_rfs_inode_handle* handle)
{
  return rtems_rfs_read_u32 (&handle->node->ctime);
}

/**
 * Set the ctime.
 *
 * @param[in] handle is the inode handle.
 * @param[in] ctime The ctime.
 */
static inline void
rtems_rfs_inode_set_ctime (rtems_rfs_inode_handle* handle,
                           rtems_rfs_time          ctime)
{
  rtems_rfs_write_u32 (&handle->node->ctime, ctime);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the block number.
 *
 * @param[in] handle is the inode handle.
 * @param[in] block is the block number to return.
 *
 * @retval block The block number.
 */
static inline uint32_t
rtems_rfs_inode_get_block (rtems_rfs_inode_handle* handle, int block)
{
  return rtems_rfs_read_u32 (&handle->node->data.blocks[block]);
}

/**
 * Set the block number for a given block index.
 *
 * @param[in] handle is the inode handle.
 * @param[in] block is the block index.
 * @param[in] bno is the block number.
 */
static inline void
rtems_rfs_inode_set_block (rtems_rfs_inode_handle* handle, int block, uint32_t bno)
{
  rtems_rfs_write_u32 (&handle->node->data.blocks[block], bno);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the last map block from the inode.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval block The last map block number.
 */
static inline uint32_t
rtems_rfs_inode_get_last_map_block (rtems_rfs_inode_handle* handle)
{
  return rtems_rfs_read_u32 (&handle->node->last_map_block);
}

/**
 * Set the last map block.
 *
 * @param[in] handle is the inode handle.
 * @param[in] block_count is last map block number.
 */
static inline void
rtems_rfs_inode_set_last_map_block (rtems_rfs_inode_handle* handle, uint32_t last_map_block)
{
  rtems_rfs_write_u32 (&handle->node->last_map_block, last_map_block);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Get the last data block from the inode.
 *
 * @param[in] handle is the inode handle.
 *
 * @retval block The last data block number.
 *
 */
static inline uint32_t
rtems_rfs_inode_get_last_data_block (rtems_rfs_inode_handle* handle)
{
  return rtems_rfs_read_u32 (&handle->node->last_data_block);
}

/**
 * Set the last data block.
 *
 * @param[in] handle is the inode handle.
 * @param[in] block_count is the last data block number.
 */
static inline void
rtems_rfs_inode_set_last_data_block (rtems_rfs_inode_handle* handle, uint32_t last_data_block)
{
  rtems_rfs_write_u32 (&handle->node->last_data_block, last_data_block);
  rtems_rfs_buffer_mark_dirty (&handle->buffer);
}

/**
 * Allocate an inode number and return it.
 *
 * @param[in] fs is the file system data.
 * @param[out] ino will contain the ino.
 *
 * @retval 0 Successful operation.
 * @retval error_code An error occurred.
 */
int rtems_rfs_inode_alloc (rtems_rfs_file_system* fs,
                           rtems_rfs_bitmap_bit   goal,
                           rtems_rfs_ino*         ino);

/**
 * Free an inode.
 *
 * @param[in] fs is the file system data.
 * @param[in] ino is the ino too free.
 *
 * @retval 0 Successful operation.
 * @retval error_code An error occurred.
 */
int rtems_rfs_inode_free (rtems_rfs_file_system* fs,
                          rtems_rfs_ino          ino);

/**
 * Open the inode handle. This reads the inode into the buffer and sets the
 * data pointer. All data is in media byte order and needs to be accessed via
 * the supporting calls.
 *
 * @param[in] fs is the file system.
 * @param[in] ino is the inode number.
 * @param[in] handle is the handle to the inode we are opening.
 * @param[in] load If true load the inode into memory from the media.
 *
 * @retval 0 Successful operation.
 * @retval error_code An error occurred.
 */
int rtems_rfs_inode_open (rtems_rfs_file_system*  fs,
                          rtems_rfs_ino           ino,
                          rtems_rfs_inode_handle* handle,
                          bool                    load);

/**
 * The close inode handle. All opened inodes need to be closed.
 *
 * @param[in] fs is the file system.
 * @param[in] handle is the handle to close.
 *
 * @retval 0 Successful operation.
 * @retval error_code An error occurred.
 */
int rtems_rfs_inode_close (rtems_rfs_file_system*  fs,
                           rtems_rfs_inode_handle* handle);

/**
 * Load the inode into memory.
 *
 * @param[in] fs is the file system.
 * @param[in] handle is the inode handle to load.
 *
 * @retval 0 Successful operation.
 * @retval error_code An error occurred.
 */
int rtems_rfs_inode_load (rtems_rfs_file_system*  fs,
                          rtems_rfs_inode_handle* handle);

/**
 * Unload the inode from memory.
 *
 * @param[in] fs is the file system.
 * @param[in] handle is the inode handle to unload.
 * @param[in] update_ctime Update the ctime field of the inode.
 *
 * @retval 0 Successful operation.
 * @retval error_code An error occurred.
 */
int rtems_rfs_inode_unload (rtems_rfs_file_system*  fs,
                            rtems_rfs_inode_handle* handle,
                            bool                    update_ctime);

/**
 * Create an inode allocating, initialising and adding an entry to the parent
 * directory.
 *
 * @param[in] fs is the file system data.
 * @param[in] parent is the parent inode number to add the directory entry to.
 * @param[in] name is a pointer to the name of the directory entryinode
 *             to create.
 *
 */
int rtems_rfs_inode_create (rtems_rfs_file_system*  fs,
                            rtems_rfs_ino           parent,
                            const char*             name,
                            size_t                  length,
                            uint16_t                mode,
                            uint16_t                links,
                            uid_t                   uid,
                            gid_t                   gid,
                            rtems_rfs_ino*          ino);

/**
 * Delete the inode eraseing it and release the buffer to commit the write. You
 * need to load the inode again if you wish to use it again.
 *
 * @param[in] fs is the file system.
 * @param[in] handle is the inode handle to erase.
 *
 * @retval 0 Successful operation.
 * @retval error_code An error occurred.
 */
int rtems_rfs_inode_delete (rtems_rfs_file_system*  fs,
                            rtems_rfs_inode_handle* handle);

/**
 * Initialise a new inode.
 *
 * @param[in] handle is the inode handle to initialise.
 * @param[in] links are the number of links to the inode.
 * @param[in] mode is the inode mode.
 * @param[in] uid is the user id.
 * @param[in] gid is the group id.
 *
 * @retval 0 Successful operation.
 * @retval error_code An error occurred.
 */
int rtems_rfs_inode_initialise (rtems_rfs_inode_handle* handle,
                                uint16_t                links,
                                uint16_t                mode,
                                uid_t                   uid,
                                gid_t                   gid);

/**
 * Time stamp the inode with the current time. The ctime field is hanlded
 * automatically.
 *
 * @param[in] handle is the inode handle.
 * @param[in] atime Update the atime field.
 * @param[in] mtime UPdate the mtime field.
 *
 * @retval 0 Successful operation.
 * @retval ENXIO No inode is loaded.
 * @retval error_code An error occurred.
 */
int rtems_rfs_inode_time_stamp_now (rtems_rfs_inode_handle* handle,
                                    bool                    atime,
                                    bool                    mtime);

/**
 * Calculate the size of data attached to the inode.
 *
 * @param[in] fs is the file system data.
 * @param[in] handle is the inode handle.
 *
 * @retval size The data size in bytes in the block map attched to the inode.
 */
rtems_rfs_pos rtems_rfs_inode_get_size (rtems_rfs_file_system*  fs,
                                        rtems_rfs_inode_handle* handle);

#endif

