include(ExternalProject)

set_property(DIRECTORY PROPERTY EP_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
set_property(DIRECTORY PROPERTY EXCLUDE_FROM_ALL true)

set(PATCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/patches)
#set(BUILDENV PATH=<INSTALL_DIR>/bin:$ENV{PATH} CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER})
set(BUILDENV PATH=<INSTALL_DIR>/bin:$ENV{PATH} CC=gcc-4.6 CXX=g++-4.6)

### {{{ autoconf
ExternalProject_Add(toolchain_autoconf_2_64
	URL http://ftp.gnu.org/gnu/autoconf/autoconf-2.64.tar.bz2
	URL_MD5 ef400d672005e0be21e0d20648169074
	CONFIGURE_COMMAND env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
)

ExternalProject_Add(toolchain_autoconf_2_69
	URL http://ftp.gnu.org/gnu/autoconf/autoconf-2.69.tar.gz
	URL_MD5 82d05e03b93e45f5a39b828dc9c6c29b
	CONFIGURE_COMMAND env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
)

ExternalProject_Add(toolchain_automake_1_12
	DEPENDS toolchain_autoconf_2_64
	URL http://ftp.gnu.org/gnu/automake/automake-1.12.tar.gz
	URL_MD5 d8a8f6bd898a6b34a874728cd1e47a95
	CONFIGURE_COMMAND env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
)

ExternalProject_Add(toolchain_automake_1_14
	DEPENDS toolchain_autoconf_2_69
	URL http://ftp.gnu.org/gnu/automake/automake-1.14.tar.gz
	URL_MD5 a3c0d9298c6112f5f2c26c639ccaaed7
	CONFIGURE_COMMAND env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
)

add_custom_target(toolchain_automake)
add_dependencies(toolchain_automake toolchain_automake_1_12)
### }}}

### {{{ binutils
ExternalProject_Add(toolchain_binutils_2_22
	URL ftp://ftp.gnu.org/gnu/binutils/binutils-2.22.tar.bz2
	URL_MD5 ee0f10756c84979622b992a4a61ea3f5
	PATCH_COMMAND
		patch -N -p1 -t < ${PATCH_DIR}/binutils-2.22.patch
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--disable-shared --enable-static --target=i686-pc-iposix
			--disable-multilib --disable-nls
			--with-gnu-as --with-gnu-ld
)

add_custom_target(toolchain_binutils)
add_dependencies(toolchain_binutils toolchain_binutils_2_22)
### }}}

### {{{ GCC
ExternalProject_Add(toolchain_libelf_0_8_13
	URL http://www.mr511.de/software/libelf-0.8.13.tar.gz
	URL_MD5 4136d7b4c04df68b686570afa26988ac
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--disable-shared --enable-static --disable-nls
)

ExternalProject_Add(toolchain_gmp_5_0_5
	URL ftp://ftp.gmplib.org/pub/gmp-5.0.5/gmp-5.0.5.tar.bz2
	URL_MD5 041487d25e9c230b0c42b106361055fe
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--disable-shared --enable-static --enable-cxx
)

ExternalProject_Add(toolchain_ppl_0_12_1
	DEPENDS
		toolchain_gmp_5_0_5
	URL
		ftp://ftp.cs.unipr.it/pub/ppl/releases/0.12.1/ppl-0.12.1.tar.bz2
	URL_MD5
		8da3ab9de18e669b7af8c4707817d468
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--with-gmp=<INSTALL_DIR>
			--disable-shared --enable-static
)

ExternalProject_Add(toolchain_ppl_1_0
	DEPENDS
		toolchain_gmp_5_0_5
	URL
		http://bugseng.com/products/ppl/download/ftp/releases/1.0/ppl-1.0.tar.bz2
	URL_MD5
		8a90e0b0b3e9527609a6e5ed3616fab1
	CONFIGURE_COMMAND
		env ${BUILDENV} LIBS=-lm <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--with-gmp=<INSTALL_DIR>
			--disable-shared --enable-static
)

ExternalProject_Add(toolchain_isl_0_11_2
	DEPENDS toolchain_gmp_5_0_5
	URL ftp://ftp.linux.student.kuleuven.be/pub/people/skimo/isl/isl-0.11.2.tar.bz2
	URL_MD5 c40daa17d2995d1c98a0c1aca607541f
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--with-gmp-prefix=<INSTALL_DIR>
			--with-piplib=no
			--disable-shared --enable-static
)

ExternalProject_Add(toolchain_cloog_0_16_2
	DEPENDS
		toolchain_gmp_5_0_5
	URL
		ftp://gcc.gnu.org/pub/gcc/infrastructure/cloog-0.16.2.tar.gz
	URL_MD5
		83877caaa879c7160063138bb18348e7
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--with-gmp-prefix=<INSTALL_DIR>
			--disable-shared --enable-static
)

ExternalProject_Add(toolchain_cloog_0_17_0
	DEPENDS toolchain_gmp_5_0_5 toolchain_isl_0_11_2
	URL http://www.bastoul.net/cloog/pages/download/cloog-0.17.0.tar.gz
	URL_MD5 0aa3302c81f65ca62c114e5264f8a802
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--with-isl-prefix=<INSTALL_DIR>
			--with-gmp-prefix=<INSTALL_DIR>
			--with-osl=no
			--disable-shared --enable-static
	BUILD_IN_SOURCE 1
)

ExternalProject_Add(toolchain_cloog_0_18_0
	DEPENDS toolchain_gmp_5_0_5 toolchain_isl_0_11_2
	URL http://www.bastoul.net/cloog/pages/download/cloog-0.18.0.tar.gz
	URL_MD5 be78a47bd82523250eb3e91646db5b3d
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--with-isl-prefix=<INSTALL_DIR>
			--with-gmp-prefix=<INSTALL_DIR>
			--with-osl=no
			--disable-shared --enable-static
)

ExternalProject_Add(toolchain_cloog_parma_0_16_1
	DEPENDS
		toolchain_gmp_5_0_5
		toolchain_ppl_0_12_1
		#toolchain_ppl_1_0
	URL
		http://www.bastoul.net/cloog/pages/download/cloog-parma-0.16.1.tar.gz
	URL_MD5
		f483539b30a60a3478eea70c77b26bef
	CONFIGURE_COMMAND
		env ${BUILDENV} LIBS=-lm PPL_LIBS=-lppl_c\ -lppl\ -lgmpxx\ -lm <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--with-gmp=<INSTALL_DIR>
			--with-ppl=<INSTALL_DIR>
			--disable-shared --enable-static
			LDFLAGS=-lstdc++
)

ExternalProject_Add(toolchain_mpfr_3_1_0
	DEPENDS toolchain_gmp_5_0_5
	URL http://www.mpfr.org/mpfr-3.1.0/mpfr-3.1.0.tar.bz2
	URL_MD5 238ae4a15cc3a5049b723daef5d17938
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--with-gmp=<INSTALL_DIR>
			--disable-shared --enable-static
)

ExternalProject_Add(toolchain_mpc_0_9
	DEPENDS toolchain_gmp_5_0_5 toolchain_mpfr_3_1_0
	URL http://www.multiprecision.org/mpc/download/mpc-0.9.tar.gz
	URL_MD5 0d6acab8d214bd7d1fbbc593e83dd00d
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--with-gmp=<INSTALL_DIR>
			--with-mpfr=<INSTALL_DIR>
			--disable-shared --enable-static
)

ExternalProject_Add(toolchain_libc_stage1_newlib_1_18_0
	URL
		ftp://sourceware.org/pub/newlib/newlib-1.18.0.tar.gz
	URL_MD5
		3dae127d4aa659d72f8ea8c0ff2a7a20
	PATCH_COMMAND
		patch -N -p1 -t < ${PATCH_DIR}/newlib-1.18.0.patch
	UPDATE_COMMAND
		env ${BUILDENV} autoreconf <SOURCE_DIR>/newlib/libc/sys <SOURCE_DIR>/newlib/libc/sys/iposix
	CONFIGURE_COMMAND true		## nothing more to do
	BUILD_COMMAND true			## nothing more to do
	INSTALL_COMMAND true		## nothing more to do
)
add_dependencies(toolchain_libc_stage1_newlib_1_18_0 toolchain_automake)

ExternalProject_Add(toolchain_libc_stage1_newlib_1_19_0
	URL
		ftp://sourceware.org/pub/newlib/newlib-1.19.0.tar.gz
	URL_MD5
		0966e19f03217db9e9076894b47e6601
	PATCH_COMMAND
		patch -N -p1 -t < ${PATCH_DIR}/newlib-1.18.0.patch
	UPDATE_COMMAND
		env ${BUILDENV} autoreconf <SOURCE_DIR>/newlib/libc/sys <SOURCE_DIR>/newlib/libc/sys/iposix
	CONFIGURE_COMMAND true		## nothing more to do
	BUILD_COMMAND true			## nothing more to do
	INSTALL_COMMAND true		## nothing more to do
)
add_dependencies(toolchain_libc_stage1_newlib_1_19_0 toolchain_automake)

ExternalProject_Add(toolchain_gcc_stage1_4_6_3
	DEPENDS
		toolchain_binutils_2_22
		toolchain_libelf_0_8_13
		toolchain_gmp_5_0_5
		toolchain_mpc_0_9
		toolchain_mpfr_3_1_0
		#toolchain_cloog_0_17_0
		#toolchain_ppl_0_12_1
		toolchain_cloog_parma_0_16_1
		toolchain_libc_stage1_newlib_1_18_0
	URL
		ftp://ftp.gnu.org/gnu/gcc/gcc-4.6.3/gcc-4.6.3.tar.bz2
	URL_MD5
		773092fe5194353b02bb0110052a972e
	PATCH_COMMAND
		ln -sf ../toolchain_libc_stage1_newlib_1_18_0/newlib . &&
		ln -sf ../toolchain_libc_stage1_newlib_1_18_0/libgloss .
	UPDATE_COMMAND
		env ${BUILDENV} autoreconf <SOURCE_DIR> <SOURCE_DIR>/libstdc++-v3
	CONFIGURE_COMMAND
		env ${BUILDENV} LIBRARY_PATH=<INSTALL_DIR>/lib <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
			--with-cloog=<INSTALL_DIR> --enable-cloog-backend=isl
			--with-gmp=<INSTALL_DIR> --with-libelf=<INSTALL_DIR>
			--with-mpc=<INSTALL_DIR> --with-mpfr=<INSTALL_DIR>
			--with-ppl=<INSTALL_DIR>
			--target=i686-pc-iposix --disable-multilib
			--disable-nls
			--enable-languages=c,c++
			--with-newlib --without-headers
			--with-host-libstdcxx=-lstdc++\ -lm
			--with-system-zlib --enable-__cxa_atexit
			--disable-libunwind-exceptions --enable-clocale=gnu
			--disable-libstdcxx-pch
			--enable-gnu-unique-object
			--disable-werror
			--enable-linker-build-id
			--enable-lto
			--enable-gold
			--enable-ld=default
			--enable-plugin
			--with-plugin-ld=ld.gold
			--enable-version-specific-runtime-libs
			--enable-checking=release
			--disable-threads
			--with-sysroot=<INSTALL_DIR> --with-build-sysroot=<INSTALL_DIR>
	BUILD_COMMAND
		env ${BUILDENV} LIBRARY_PATH=<INSTALL_DIR>/lib $\(MAKE\) ## the LIBRARY_PATH should not contain the current directory
)
add_dependencies(toolchain_gcc_stage1_4_6_3 toolchain_automake)

ExternalProject_Add_Step(toolchain_gcc_stage1_4_6_3 patch-no-fixinc
	WORKING_DIRECTORY <SOURCE_DIR>
	COMMAND
		sed -i "s@\\./fixinc\\.sh@-c true@" "gcc/Makefile.in"
    DEPENDEES patch
	DEPENDERS update
)

ExternalProject_Add_Step(toolchain_gcc_stage1_4_6_3 patch-cloog
	WORKING_DIRECTORY <SOURCE_DIR>
	COMMAND
		true #patch -Np1 -t -i ${PATCH_DIR}/gcc-4.6.2-cloog-0.17.patch
    DEPENDEES patch-no-fixinc
	DEPENDERS update
)
ExternalProject_Add_Step(toolchain_gcc_stage1_4_6_3 patch-texti
	WORKING_DIRECTORY <SOURCE_DIR>
	COMMAND
		patch -Np1 -t -i ${PATCH_DIR}/gcc-4.6.3-texti.patch
    DEPENDEES patch-cloog
	DEPENDERS update
)
ExternalProject_Add_Step(toolchain_gcc_stage1_4_6_3 patch-iposix
	WORKING_DIRECTORY <SOURCE_DIR>
	COMMAND
		patch -Np1 -t -i ${PATCH_DIR}/gcc-4.6.3-iposix-target.patch
    DEPENDEES patch-texti
	DEPENDERS update
)

ExternalProject_Add(toolchain_newlib_1_18_0
	DEPENDS
		toolchain_libc_stage1_newlib_1_18_0
		toolchain_gcc_stage1_4_6_3 # inherits dependencies from gcc-stage1
	SOURCE_DIR
		${CMAKE_CURRENT_BINARY_DIR}/src
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/toolchain_libc_stage1_newlib_1_18_0/configure --prefix=<INSTALL_DIR>
			--with-cloog=<INSTALL_DIR> --with-gmp=<INSTALL_DIR>
			--with-libelf=<INSTALL_DIR> --with-mpc=<INSTALL_DIR>
			--with-mpfr=<INSTALL_DIR> --with-ppl=<INSTALL_DIR>
			--target=i686-pc-iposix --disable-multilib
			--disable-nls --with-gnu-as --with-gnu-ld
	BUILD_COMMAND
		env ${BUILDENV} $\(MAKE\) all				## newlib's configure doesn't set PATH for binutils
	INSTALL_COMMAND
		env ${BUILDENV} $\(MAKE\) install			## newlib's configure doesn't set PATH for binutils
)
add_dependencies(toolchain_newlib_1_18_0 toolchain_automake)

ExternalProject_Add(toolchain_newlib_1_19_0
	DEPENDS
		toolchain_libc_stage1_newlib_1_19_0
		toolchain_gcc_stage1_4_6_3
	SOURCE_DIR
		${CMAKE_CURRENT_BINARY_DIR}/src
	CONFIGURE_COMMAND
		env ${BUILDENV} <SOURCE_DIR>/toolchain_libc_stage1_newlib_1_19_0/configure --prefix=<INSTALL_DIR>
			--with-cloog=<INSTALL_DIR> --with-gmp=<INSTALL_DIR>
			--with-libelf=<INSTALL_DIR> --with-mpc=<INSTALL_DIR>
			--with-mpfr=<INSTALL_DIR> --with-ppl=<INSTALL_DIR>
			--target=i686-pc-iposix --disable-multilib
			--disable-nls --with-gnu-as --with-gnu-ld
	BUILD_COMMAND
		env ${BUILDENV} $\(MAKE\) all				## newlib's configure doesn't set PATH for binutils
	INSTALL_COMMAND
		env ${BUILDENV} $\(MAKE\) install			## newlib's configure doesn't set PATH for binutils
)
add_dependencies(toolchain_newlib_1_18_0 toolchain_automake)


ExternalProject_Add(toolchain_gcc_4_6_3
	DEPENDS
		toolchain_gcc_stage1_4_6_3 ## inherits dependencies from stage1
		toolchain_newlib_1_18_0
	SOURCE_DIR
		${CMAKE_CURRENT_BINARY_DIR}/src
	CONFIGURE_COMMAND
		env ${BUILDENV} LDFLAGS=-lm <SOURCE_DIR>/toolchain_gcc_stage1_4_6_3/configure --prefix=<INSTALL_DIR>
			--with-cloog=<INSTALL_DIR> --enable-cloog-backend=isl
			--with-gmp=<INSTALL_DIR> --with-libelf=<INSTALL_DIR>
			--with-mpc=<INSTALL_DIR> --with-mpfr=<INSTALL_DIR>
			--with-ppl=<INSTALL_DIR> --target=i686-pc-iposix --disable-multilib
			--disable-nls --with-gnu-as --with-gnu-ld --enable-languages=c,c++
			--with-newlib --without-headers --disable-shared
			--with-host-libstdcxx=-lstdc++
)

add_custom_target(toolchain_gcc)
add_dependencies(toolchain_gcc toolchain_gcc_4_6_3)
### }}}

### {{{ NASM
ExternalProject_Add(toolchain_nasm_2_10
	URL http://www.nasm.us/pub/nasm/releasebuilds/2.10/nasm-2.10.tar.bz2
	URL_MD5 f9d3bf31d5e372c9a17ad66a8968584a
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
)

add_custom_target(toolchain_nasm)
add_dependencies(toolchain_nasm toolchain_nasm_2_10)
### }}}

### {{{ Grub
ExternalProject_Add(toolchain_grub_0_97
	URL http://alpha.gnu.org/gnu/grub/grub-0.97.tar.gz
	URL_MD5 cd3f3eb54446be6003156158d51f4884
	PATCH_COMMAND
		patch -N -p1 -t < ${PATCH_DIR}/grub-0.97-ldflags-objcopy-remove-build-id.patch &&
		patch -N -p1 -t < ${PATCH_DIR}/grub-0.97-automake-pkglib.patch &&
		sed -e "/^AC_PROG_CC/ a\\AM_PROG_CC_C_O\\ " -i "configure.ac" &&
		sed -e "/^AC_PROG_CC/ a\\AM_PROG_AS\\ " -i "configure.ac"
	UPDATE_COMMAND
		env ${BUILDENV} aclocal &&
		env ${BUILDENV} autoconf &&
		env ${BUILDENV} autoreconf -i &&
		env ${BUILDENV} automake
	CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --disable-nls CFLAGS=-m32\ -fno-strict-aliasing
)

ExternalProject_Add(toolchain_grub_1_99
	URL ftp://ftp.gnu.org/gnu/grub/grub-1.99.tar.gz
	URL_MD5 ca9f2a2d571b57fc5c53212d1d22e2b5
	CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
	--disable-nls
)

add_custom_target(toolchain_grub)
add_dependencies(toolchain_grub toolchain_grub_0_97)
### }}}

### {{{ MTools
ExternalProject_Add(toolchain_mtools_4_0_17
	URL ftp://ftp.gnu.org/gnu/mtools/mtools-4.0.17.tar.bz2
	URL_MD5 15571c615d8f75f5f6d294272f80c7fa
	CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
)

add_custom_target(toolchain_mtools)
add_dependencies(toolchain_mtools toolchain_mtools_4_0_17)
### }}}

add_custom_target(toolchain)
add_dependencies(toolchain
	toolchain_gcc
	toolchain_nasm
	toolchain_grub
	toolchain_mtools
)
