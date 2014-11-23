#ifndef _COMMON_INCLUDE_ERR_H_
#define _COMMON_INCLUDE_ERR_H_

#include <stdint.h>

extern "C" void err( uint32_t exit_code, const char* format, ... )
		__attribute__ ((format (gnu_printf, 2, 3)))
		__attribute__((noreturn));

extern "C" void errx( uint32_t exit_code, const char* message )
		__attribute__((noreturn));

/* hack to get rid of the warning the main should have an attribute noreturn */
extern "C" int main(int, char**)
		__attribute__((noreturn));

#endif /* !_COMMON_INCLUDE_ERR_H_ */
