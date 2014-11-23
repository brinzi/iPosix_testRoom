#ifndef _KERN_INCLUDE_FS_FILEHANDLE_HOLDER_HPP_
#define _KERN_INCLUDE_FS_FILEHANDLE_HOLDER_HPP_

#include "fs/filehandle.h"

#include <memory>
#include <map>

namespace iposix
{
namespace fs
{

/* Predefine */
class i_fs_node;

/**
 * Describes a class which holds filehandles
 */
class filehandle_holder
{
	protected:
		typedef ::std::shared_ptr< i_fs_node > i_fs_node_ptr;

	private:
		/** The filehandles */
		::std::map< uint32_t, filehandle_ptr > filehandles;

		/** A counter for the filehandles */
		uint32_t fd_counter;

		/**
		 * Adds a filehandle
		 * @param[in] fh the filehandle to add
		 */
		inline void push_filehandle( filehandle_ptr fh )
		{
			this->filehandles[fh->filehandle_num()] = fh;
		}

	public:
		/** Constructor */
		filehandle_holder()
			: filehandles(),
			fd_counter( 0 )
		{ }

		/** Destructor */
		virtual ~filehandle_holder()
		{ }

		/**
		 * Returns a filehandle by filedescriptor
		 * @param[in] filedescriptor the descriptor number
		 * @return the filehandle
		 */
		inline filehandle_ptr get_filehandle( uint32_t filedescriptor )
		{
			return this->filehandles.find( filedescriptor )->second;
		}


		inline uint32_t push_filehandle(
				i_fs_node_ptr file, 
				const char flags = '\0',
				const char mode = '\0' )
		{
			filehandle* fh = new filehandle( file, flags, mode, this->fd_counter );
			filehandle_ptr fh_ptr = filehandle_ptr( fh );
			this->fd_counter++;
			this->push_filehandle( fh_ptr );

			return fh_ptr->filehandle_num();
		}

		/**
		 * Copies all filehandles from another filehandle holder
		 * @param[in] holder the holder to copy
		 */
		inline void copy_filehandles( const filehandle_holder& holder )
		{
			for (
					auto it = holder.filehandles.begin();
					it != holder.filehandles.end();
					it++
				)
			{
				this->push_filehandle( it->second );
			}
			this->fd_counter = holder.fd_counter;
		}

		/**
		 * Removes a filehandle by the descriptor number
		 * @param[in] filedescriptor the descriptor number
		 */
		inline i_fs_node_ptr remove_filehandle( uint32_t filedescriptor )
		{
			auto fh = this->filehandles.find( filedescriptor );

			if ( fh->second )
			{
				i_fs_node_ptr ret = fh->second->get_node();
				this->filehandles.erase( fh );

				return ret;
			}
			return i_fs_node_ptr();
		}

};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_FILEHANDLE_HOLDER_HPP_ */
