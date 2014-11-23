#ifndef _KERN_INCLUDE_FS_FAT_TYPE_H_
#define _KERN_INCLUDE_FS_FAT_TYPE_H_

namespace iposix {
namespace fs {

/**
 * The fat filesystem type
 */
enum fat_type_t
{
	/** A fat12 Filesystem */
	fat12,
	/** A fat16 Filesystem */
	fat16,
	/** A fat32 Filesystem */
	fat32
};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_FAT_TYPE_H_ */
