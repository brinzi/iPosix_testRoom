#ifndef _KERN_INCLUDE_MM_MEMORY_AREA_H_
#define _KERN_INCLUDE_MM_MEMORY_AREA_H_

#include <stdint.h>

namespace iposix {
namespace hlmemory {

/**
 * A continuous area of memory
 */
struct memory_area
{
	void * low;	/**< first useable address */
	void * high;	/**< first unuseable address */

	/**
	 * Constructor.
	 *
	 * @param[in] low first useable address
	 * @param[in] high first unuseable address
	 */
	memory_area( void * low = 0, void * high = 0 );

	/**
	 * Checks whether another memory_area is smaller than this one
	 */
	bool operator< ( const memory_area& ma );
	/**
	 * Checks whether another memory_area is greater than this one
	 */
	bool operator> ( const memory_area& ma );

	/**
	 * Size of the memory area.
	 *
	 * @return size of the memory area
	 */
	uint32_t size();

	/**
	 * Combines two memory areas
	 *
	 * @param[in] ma the memory area to combine with
	 *
	 * @return true, if successfull
	 */
	bool combine( const memory_area& ma );
};

}} //iposix::hlmemory

#endif /* !_KERN_INCLUDE_MM_MEMORY_AREA_H_ */
