# allow enforcement of specific compiler usage
include(CMakeForceCompiler)

# set architecture dependent configuration
if(${IPOSIX_ARCH} STREQUAL "x86")
	set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)
	set(IPOSIX_C_FLAGS "-m32")

	#CMAKE_FORCE_C_COMPILER(${CMAKE_BINARY_DIR}/toolchain/bin/i686-pc-iposix-gcc i686-pc-iposix-gcc)
	#CMAKE_FORCE_CXX_COMPILER(${CMAKE_BINARY_DIR}/toolchain/bin/i686-pc-iposix-g++ i686-pc-iposix-g++)
	CMAKE_FORCE_C_COMPILER(${Toolchain_Prefix}/bin/i686-pc-iposix-gcc i686-pc-iposix-gcc)
	CMAKE_FORCE_CXX_COMPILER(${Toolchain_Prefix}/bin/i686-pc-iposix-g++ i686-pc-iposix-g++)
endif()

# set debug symbol flags only if requested
if(IPOSIX_DEBUG_SYMBOLS)
	set(IPOSIX_ASM_NASM_FLAGS "${IPOSIX_ASM_NASM_FLAGS} -g")
	set(IPOSIX_C_FLAGS "${IPOSIX_C_FLAGS} -ggdb")
	set(IPOSIX_CXX_FLAGS "${IPOSIX_CXX_FLAGS} -ggdb")
endif()

# define set of compiler warning flags
set(IPOSIX_C_WARNS "-ansi -pedantic -Wall -Wextra -Wformat=2 -Wno-format-y2k -Wno-unused-parameter -Wpointer-arith -Wreturn-type -Wcast-qual -Wwrite-strings -Wswitch -Wcast-align -Wunused-parameter -Wchar-subscripts -Winline -Wredundant-decls -Wno-long-long -Wlogical-op -Wmissing-format-attribute -Wunsafe-loop-optimizations")

set(IPOSIX_CXX_WARNS "${IPOSIX_C_WARNS} -Wabi -Wold-style-cast -Wctor-dtor-privacy -Wnon-virtual-dtor -Wsign-promo -Woverloaded-virtual")

# set -Werror warning flags only if requested
if(IPOSIX_WSHADOW)
	set(IPOSIX_C_FLAGS "${IPOSIX_C_FLAGS} -Wshadow")
	set(IPOSIX_CXX_FLAGS "${IPOSIX_CXX_FLAGS} -Wshadow")
endif()

# set -Wmissing-noreturn warning flags only if requested
if(IPOSIX_WMISSING_NORETURN)
	set(IPOSIX_C_FLAGS "${IPOSIX_C_FLAGS} -Wmissing-noreturn")
	set(IPOSIX_CXX_FLAGS "${IPOSIX_CXX_FLAGS} -Wmissing-noreturn")
endif()

# set -Werror warning flags only if requested
if(IPOSIX_WERROR)
	set(IPOSIX_C_FLAGS "${IPOSIX_C_FLAGS} -Werror")
	set(IPOSIX_CXX_FLAGS "${IPOSIX_CXX_FLAGS} -Werror")
endif()

# set target system to an "embedded system without OS"
set(CMAKE_SYSTEM_NAME Generic)

# set architecture independent compilation options
set(IPOSIX_C_FLAGS "-O0 -ffreestanding ${IPOSIX_C_FLAGS} ${IPOSIX_C_WARNS} -std=c99")
set(IPOSIX_CXX_FLAGS "-O0 -ffreestanding ${IPOSIX_CXX_FLAGS} ${IPOSIX_CXX_WARNS} -std=c++0x")

# set combined compiler options
set(CMAKE_C_FLAGS "${IPOSIX_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${IPOSIX_CXX_FLAGS}")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

# set common include directories to search for headers
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/src/common/include)

# set path to netwide assembler
set(CMAKE_ASM_NASM_COMPILER ${Toolchain_Prefix}/bin/nasm)
enable_language(ASM_NASM OPTIONAL)
message(STATUS "Pretend there is a nasm installed until one is generated from within `toolchain' target")

