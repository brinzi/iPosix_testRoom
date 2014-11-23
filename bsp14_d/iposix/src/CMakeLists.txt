set(IPOSIX_ARCH "x86")
set(IPOSIX_DEBUG_SYMBOLS true)

set(IPOSIX_WERROR false)
set(IPOSIX_WSHADOW false)
set(IPOSIX_WMISSING_NORETURN false)

include(iposix_buildenv)

foreach(DIR kern user)
	add_subdirectory(${DIR})
endforeach()

