#ifndef _KERN_INCLUDE_FS_FILEHANDLE_H_
#define _KERN_INCLUDE_FS_FILEHANDLE_H_

#include <memory>
#include <stdint.h>

/* Predefine */
struct stat;

namespace iposix {
namespace fs {

/* Predefine */
class i_fs_node;

/**
 * Describes a file handle which gives access to files to userland programs.
 * \todo TODO open flags are ignored
 */
class filehandle
{
	private:
		/* Predefine */
		typedef ::std::shared_ptr< i_fs_node > i_fs_node_ptr;

	public:
		/**
		 * Constructor for a filehandle
		 * @param[in] node the depending node
		 * @param[in] flags TODO
		 * @param[in] mode TODO
		 * @param[in] filehandle_num the filehandle number
		 */
		filehandle(
				i_fs_node_ptr node,
				const char flags,
				const char mode,
				const uint32_t filehandle_num );

		/** Destructor */
		~filehandle();

		/**
		 * Returns the filehandle number
		 * @return the filehandle number
		 */
		inline uint32_t filehandle_num() const
		{
			return this->fd;
		}

		/**
		 * Returns the real file node
		 * @return the real file node
		 */
		inline i_fs_node_ptr get_node()
		{
			return this->node;
		}

		/**
		 * Reads length amount of data to the buffer
		 * @param[in] buffer the buffer to write to
		 * @param[in] length the amount of data to read
		 * @return true on success else false
		 */
		uint32_t read( void* buffer, uint32_t length );//DOTHATEVERYWHERE

		/**
		 * Write *length* amount of data from the buffer to the file
		 * @param[in] buffer the buffer to read from
		 * @param[in] length the amount of data to write
		 * @return true on success else false
		 */
		uint32_t write( void* buffer, uint32_t length );

		/**
		 * Sets the current read/write position to a specific location absoulte
		 * or relative
		 * @param[in] offset new position (absoulte or relative)
		 * @param[in] direction the direction (relative to current, relative to
		 * end, absoulte)
		 * @return true on success else false
		 */
		bool seek( int64_t offset, int32_t direction );

		/**
		 * Fills a stat structure with information about this file
		 * @param[in] st pointer to the stat struct
		 * @return true on success else false
		 */
		bool stat( struct stat* st );

	
	private:
		/** The node this filehandle points to */
		i_fs_node_ptr node;

		/** The opening mode (read, write, etc) */
		const char mode;

		/** The opening flags (append, etc) */
		const char flags;

		/** the filehandle number */
		const uint32_t fd;

		/** the current position */
		int64_t offset;
};

/**
 * A Pointer to a filehandle. Since Processes/Threads can share this.
 */
typedef ::std::shared_ptr< filehandle > filehandle_ptr;

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_FILEHANDLE_H_ */
