add_custom_target(userland)

# set architecture dependent configuration
if(${IPOSIX_ARCH} STREQUAL "x86")
	set(IPOSIX_ARCH_SYSCALL_CXX_SOURCES ${CMAKE_SOURCE_DIR}/src/kern/arch/x86/syscalls/syscaller.cpp)
endif()

set(IPOSIX_SYSCALL_CXX_SOURCES ${CMAKE_SOURCE_DIR}/src/user/syscalls/syscalls.cpp ${IPOSIX_ARCH_SYSCALL_CXX_SOURCES})
set(IPOSIX_UTIL_CXX_SOURCES  ${CMAKE_SOURCE_DIR}/src/user/utils/err.cpp)
#set(IPOSIX_USERLAND_COMPILE_FLAGS "--sysroot=${Toolchain_Prefix} -fno-builtin -fno-rtti -fno-exceptions")
set(IPOSIX_USERLAND_COMPILE_FLAGS "-fno-builtin -fno-rtti -fno-exceptions")
set(IPOSIX_USERLAND_LINK_FLAGS "-lstdc++")

