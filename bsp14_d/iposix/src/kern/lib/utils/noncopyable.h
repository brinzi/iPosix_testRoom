#ifndef _KERN_LIB_UTILS_NONCOPYABLE_H_
#define _KERN_LIB_UTILS_NONCOPYABLE_H_

namespace iposix {
namespace utils {

/**
 * Every class inheriting this, can not be copied using the = operator
 * or a copy constructor.
 */
struct Noncopyable
{
   protected:
	Noncopyable() {}
	~Noncopyable() {}
   private:
	Noncopyable( const Noncopyable& ) = delete;
	const Noncopyable& operator=( const Noncopyable& ) = delete;
};

}} //iposix::utils


#endif /* !_KERN_LIB_UTILS_NONCOPYABLE_H_ */
