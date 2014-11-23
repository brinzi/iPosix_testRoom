#include <stdint.h>
#include <cxxabi.h>


namespace iposix
{

/**
 * This includes functions used for initialize runtime library. It provides
 * function for registering global static objects and local static object.
 * See BSD for good commented implementations:
 * http://fxr.googlebit.com/source/lib/libc/stdlib/atexit.c?v=NETBSD-CURRENT#L171
 * 
 * Other Startup Code:
 * http://fxr.googlebit.com/source/lib/csu/common_elf/crtbegin.c?v=NETBSD-CURRENT
 */
namespace cppruntime
{

typedef void (*ctor_ptr)( void );
typedef void (*dtor_ptr)( void );
typedef void (*finalize_ptr)( void* );

extern "C"
{
	//Variables

	/** 
	 * A Label which indicates the Head of the global objects constructor list. It
	 * is defined by the Linker.
	 */
	extern ctor_ptr __CTOR_LIST__[];

	/**
	 * A Label which indicates the Tail of the global objects constructor list. It
	 * is defined by the Linker.
	 */
	extern ctor_ptr __CTOR_END__[];

	/** 
	 * A Label which indicates the Head of the global objects destructor list. It
	 * is defined by the Linker.
	 */
	extern dtor_ptr __DTOR_LIST__[];

	/** 
	 * A Label which indicates the Tail of the global objects destructor list. It
	 * is defined by the Linker.
	 */
	extern dtor_ptr __DTOR_END__[];

	/**
	 * A Label which indicates the Head of the Exception Handler Frame. It is
	 * defined by the Linker.
	 */
	extern char __EH_FRAME_BEGIN__[];

	//Functions

	/**
	 * Initializes the runtime environment
	 */
	void _init();

	/**
	 * Finilizes the runtime environment
	 */
	void _fini();

	/**
	 * This is used to implement multilevel local varibales (__cxa_atexit,
	 * __cxa_finalize) This Symbol is provieded by newlib..
	 */
	void *__dso_handle __attribute__((weak));	//this is weak because it could be
												//provided by libc but in general 
												//it should be provided by the
												//startup code

	/**
	 * This currently destroys all objects with destructors registered
	 * with __cxa_atexit This Function is provieded by newlib..
	 * @param[in] handle see __dso_handle
	 */
	void __cxa_finalize(void* handle);	//this function is provided by newlib but
										//we have to call it at the end

	/**
	 * This Function saves all functionpointers 'f', all parameters 'p'
	 * and all Dynamic Shared Objects 'd' for later use on destruction
	 * in __cxa_finalize. This Function is provieded by newlib.
	 * @return 0 on success -1 in other cases
	 */
	int __cxa_atexit(finalize_ptr function, void* parameter, void* handle);

	/**
	 * This Function registers a Exception Hanlder Frame. This Function is
	 * provieded by newlib.
	 */
	void __register_frame_info( const void*, struct object* ) __attribute__((weak));

	/**
	 * This Function deregisters a Exception Hanlder Frame. This Function is
	 * provieded by newlib.
	 */
	void* __deregister_frame_info( const void* ) __attribute__((weak));
} //extern "C"


void __do_global_ctors_aux();		//predefine
void __do_global_dtors_aux();		//predefine

void _init()
{
	//just call the global constructors
	__do_global_ctors_aux();

	// registering the exception frame
	/* There are two functions __register_frame_info_bases and
	 * __register_frame_info. We use __register_frame_info because it calls the
	 * other.
	 */
	//static struct object object;
	static uint32_t object[10];	//this is nasty but there is no possibility to
								//get the correct struct
	static struct object* obj_ptr = reinterpret_cast<struct object*>( &object[0] );
	if ( __register_frame_info )
	{
		__register_frame_info( __EH_FRAME_BEGIN__, obj_ptr );
	}
}

void _fini()
{
	//call the global destructors
	__do_global_dtors_aux();

	//call the local destructors
	__cxa_finalize(__dso_handle); //TODO is this call correct?

	//deregistering the exception frame
	/* We use __deregister_frame_info because it calls
	 * __deregister_frame_info_bases see above
	 */
	if ( __deregister_frame_info )
	{
		__deregister_frame_info( __EH_FRAME_BEGIN__ );
	}
}

/**
 * Calls every Construktor from TAIL of the __CTOR_LIST__ .. __CTOR_END__ List
 */
void __do_global_ctors_aux()
{
	//The Constructors List starts (somtimes with -1 and sometimes) with the
	//count of Constructors and it is _always_ terminated by 0
	int32_t length = reinterpret_cast<uint32_t>( __CTOR_LIST__[0] );

	if ( length == -1 )
	{
		for (length = 1; // we skip first entry
			__CTOR_LIST__[length]; //not 0
			length++); //next

		--length; //else the entry [length] is 0
	}

	for ( int32_t i = length;
			i > 0; //first entry is length - so skip i = 0
			i-- )
	{
		__CTOR_LIST__[i]();
	}
}

/** 
 * Calls every Destruktor from HEAD of the __DTOR_LIST__ .. __DTOR_END__ List
 */
void __do_global_dtors_aux()
{
	//The Destructors List starts (somtimes with -1 and sometimes) with the
	//count of Destructors, so we don't care because it is _always_ terminated
	//by 0
	int32_t i = 1; //once again we skip the first element

	while ( __DTOR_LIST__[i] ) 
	{
		__DTOR_LIST__[i++]();
	}
}

} //namespace cppruntime
} //namespace iposix
