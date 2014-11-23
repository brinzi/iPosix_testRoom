#include <stdint.h>
#include <cxxabi.h>

/*
 * This includes functions used during the runtime of a program which makes use
 * of the c++ library. Normally there should no need for a function definition
 * here but if there is a need to overwrite the function brought in throug the
 * library you should do it here.
 */

namespace iposix
{
namespace cppruntime
{

typedef uint32_t size_t;

/* **************************************************** *
 * ********** pure virtual functions support ********** *
 * **************************************************** */

extern "C"
{
	/**
	 * This Function is called/should be called whenever the kernel
	 * detects a call to a pure virtual function. And this simply should
	 * never happen, without any hacks or undefined behaviour of the
	 * kernel.
	 * This function is provided by newlib, but we will overwrite it because we
	 * want to halt on such a call.
	 */
	void __cxa_pure_virtual() __attribute__ ((noreturn));
} //extern "C"

void __cxa_pure_virtual()
{
    // print error message
	#define ever (;;)
	for ever asm volatile("hlt");
}

} //namespace cppruntime
} //namespace iposix

namespace __gnu_cxx
{

//void __verbose_terminate_handler() __attribute__((noreturn));

/**
 * Defining this should reduce filesize.
 */
void __verbose_terminate_handler()
{
	#define ever (;;)
	for ever asm volatile("hlt");
}

}  //namespace __gnu_cxx


/* ****************************************** *
 * ********* new and delete support ********* *
 * ****************************************** */

// ****************************************************
// A very simple way is to map it to kmalloc and kfree:
// ****************************************************

/*
 * hm we don't need to implement this because it is provided by libstdc++ and it
 * calls malloc (provided by newlib) which calls malloc_r (provided by newlib)
 * which calls sbrk_r (provided by newlib) which calls sbrk which we have to
 * implement for syscalls
 */
extern "C++"
{
	/**
	 * Operator new is needed for creating objects
	 * @param[in] size memory-size needed by the object
	 * @return pointer to the memory address
	 */
	void * operator new (size_t size);
	/*
	{
		size = size;
		//return kmalloc(size);
		return static_cast<void*>(0);
	}
	*/

	/**
	 * Operator new[] is needed for creating objects
	 * @param[in] size memory-size needed by the object
	 * @return pointer to the memory address
	 */
	void * operator new[] (size_t size);
	/*
	{
		size = size;
		//return kmalloc(size);
		return static_cast<void*>(0);
	}
	*/

	/**
	 * Operator delete is needed for destroing objects
	 * @param[in] p pointer to the object with should be destroyed
	 */
	void operator delete (void * p);
	/*
	{
		p = p;
		//kfree(p);
	}
	*/

	/**
	 * Operator delete[] is needed for destroing objects
	 * @param[in] p pointer to the object with should be destroyed
	 */
	void operator delete[] (void * p);
	/*
	{
		p = p;
		//kfree(p);
	}
	*/
} //extern "C++"

