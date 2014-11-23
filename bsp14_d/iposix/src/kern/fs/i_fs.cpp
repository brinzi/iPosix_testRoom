#include "fs/i_fs.h"
#include "fs/i_fs_node.h"

#include <vector>
#include <string>

namespace iposix {
namespace fs {

i_fs::i_fs( i_fs_node_ptr container )
	: container( container )
{ }

i_fs::~i_fs()
{ }

void i_fs::read(
		const uint64_t offset,
		void* buffer,
		const uint32_t length)
{
	this->container->read( offset, buffer, length );
}

void i_fs::write(
		const uint64_t offset, 
		void* buffer, 
		const uint32_t length)
{
	this->container->write( offset, buffer, length );
}

} //namespace fs
} //namespace iposix
