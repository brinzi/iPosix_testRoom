#ifndef _KERN_LIB_UTILS_H_
#define _KERN_LIB_UTILS_H_

#include "utils/noncopyable.h"

namespace iposix {
namespace utils {

/**
 * Every singleton class should inherit this in
 * the following form:
 * 
 * class myclass : public virtual ::iposix::utils::Singleton< myclass >
 * 
 * The class should also have only a private constructor
 * 
 * The singleton instance can be accessed by:
 * ::iposix::utils::Singleton< myclass >::instance()
 *
 * @param T singleton type
 */
template< typename T >
struct Singleton
	: virtual public Noncopyable
{
	/**
	 * Get the singleton instance.
	 */
	static T& instance()
	{
		static T t;
		return t;
	}
};

}} //iposix::utils

#endif /* !_KERN_LIB_UTILS_H_ */
