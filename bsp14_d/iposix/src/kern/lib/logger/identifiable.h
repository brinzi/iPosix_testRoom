#ifndef _KERN_LIB_LOGGER_IDENTIFIABLE_H_
#define _KERN_LIB_LOGGER_IDENTIFIABLE_H_

namespace iposix {

/**
 * Interface to make object identifiable. Classes inheriting this
 * have to implement the uname function.
 */
struct identifiable
{
	virtual ~identifiable() {};
	
	/**
	 * Function returning the name of the object.
	 *
	 * Returns a constant constant char *.
	 */
	virtual const char * uname() const = 0;
};

} //iposix

#endif /* !_KERN_LIB_LOGGER_IDENTIFIABLE_H_ */
