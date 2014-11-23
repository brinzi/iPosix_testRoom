#!/bin/bash

##################
## script option #
##################
## you can edit this

AUTOCLEAN="yes"					# yes cleans after each build
DRY="no"						# yes writes each command to the logfile
INCLUDE_GDB="no"				# yes downloads, builds, installs GDB
INCLUDE_GMP="yes"				# yes downloads, builds gcc with gmp support
								# GMP is a library for arbitrary precision
								# arithmetic
INCLUDE_MPFR="yes"				# yes downloads, builds gcc with mpfr support
								# MPFR is a library for arbitrary precision
								# arithmetic on floating-point numbers
INCLUDE_MPC="yes"				# yes downloads, builds gcc with mpc support
								# MPC is a library for arithmetic of complex
								# numbers with arbitrarily high precision
INCLUDE_CLOOG="yes"				# yes downloads, builds gcc with cloog support
								# PPL+CLOOG is a library to generate code for
								# scanning Z-polyhedra
INCLUDE_LIBELF="yes"			# yes downloads, build libelf

###################
## version config #
###################
## you can edit this if you know what you are doing

## ADDITIONAL TOOLS
AUTOCONF_VERSION="2.64"			# needed for reconfigure after patching
AUTOMAKE_VERSION="1.11.1"		# needed for reconfigure after patching
GRUB_VERSION="0.97"				# needed for making disk images bootable
LIBICONV_VERSION="1.13"			# needed for mtools
MTOOLS_VERSION="4.0.15"			# needed for modifying disk images
NASM_VERSION="2.09.04"			# low-level assembler
TEXINFO_VERSION="4.13"			# generator for info files

## TOOLCHAIN
BINUTILS_VERSION="2.20.1"		# needed for GCC
NEWLIB_VERSION="1.18.0"			# c-library
GMP_VERSION="4.3.2"				# math library
MPFR_VERSION="2.4.2"			# math library - 3.* does not compile fine
MPC_VERSION="0.8.1"				# math library
PPL_VERSION="0.10"				# math library - needs sync with cloog
CLOOG_VERSION="0.15.8"			# math library
LIBELF_VERSION="0.8.13"			# ELF library
GCC_VERSION="4.5.1"				# gcc itself

## GDB
TERMCAP_VERSION="1.3.1"			# needed for GDB
GDB_VERSION="6.8"				# needed for debugging

###################
## compile config #
###################

##make config
MAKE_OPTIONS=-j2

##our target
TARGET="i686-pc-iPosix"

##how to build static
STATIC_BUILD="LDFLAGS=-static"

##which flags to use in general (binutils, newlib)
CROSS_FLAGS="--disable-multilib --disable-nls --with-gnu-as --with-gnu-ld"
##which flags to use for gcc
GCC_FLAGS="--enable-languages=c,c++ --with-newlib --without-headers \
	--disable-shared"

## MPFR depends on GMP
[ "$INCLUDE_CLOOG" == "yes" ] && INCLUDE_GMP="yes"
[ "$INCLUDE_MPC" == "yes" ] && INCLUDE_GMP="yes"
[ "$INCLUDE_MPFR" == "yes" ] && INCLUDE_GMP="yes"

[ "$INCLUDE_CLOOG" == "yes" ] || GCC_FLAGS="$GCC_FLAGS --without-ppl"
[ "$INCLUDE_CLOOG" == "yes" ] || GCC_FLAGS="$GCC_FLAGS --without-cloog"
[ "$INCLUDE_GMP" == "yes" ] || GCC_FLAGS="$GCC_FLAGS --without-gmp"
[ "$INCLUDE_GMP" == "yes" ] && GCC_FLAGS="$GCC_FLAGS --enable-cxx"
[ "$INCLUDE_MPFR" == "yes" ] || GCC_FLAGS="$GCC_FLAGS --without-mpfr"
[ "$INCLUDE_MPC" == "yes" ] || GCC_FLAGS="$GCC_FLAGS --without-mpc"

echo "$GCC_FLAGS"

###################
## package config #
###################

##binutils options
BINUTILS_URL="ftp://ftp.gnu.org/gnu/binutils"
BINUTILS_FILE="binutils-$BINUTILS_VERSION.tar.bz2"
BINUTILS_DIR="binutils-$BINUTILS_VERSION"

##GCC options
GCC_URL="ftp://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION"
GCC_FILE="gcc-$GCC_VERSION.tar.bz2"
GXX_FILE="gcc-g++-$GCC_VERSION.tar.bz2"
GCC_DIR="gcc-$GCC_VERSION"

##texinfo
TEXINFO_URL="http://ftp.gnu.org/gnu/texinfo/"
TEXINFO_FILE="texinfo-$TEXINFO_VERSION.tar.lzma"
TEXINFO_DIR="texinfo-$TEXINFO_VERSION"

##GCC LIBS
GMP_URL="http://ftp.gnu.org/pub/gnu/gmp"
GMP_FILE="gmp-$GMP_VERSION.tar.bz2"
GMP_DIR="gmp-$GMP_VERSION"
MPFR_URL="http://ftp.gnu.org/pub/gnu/mpfr/"
MPFR_FILE="mpfr-$MPFR_VERSION.tar.bz2"
MPFR_DIR="mpfr-$MPFR_VERSION"
MPC_URL="http://www.multiprecision.org/mpc/download/"
MPC_FILE="mpc-$MPC_VERSION.tar.gz"
MPC_DIR="mpc-$MPC_VERSION"
PPL_URL="ftp://ftp.cs.unipr.it/pub/ppl/releases/$PPL_VERSION/"
PPL_FILE="ppl-$PPL_VERSION.tar.bz2"
PPL_DIR="ppl-$PPL_VERSION"
CLOOG_URL="ftp://gcc.gnu.org/pub/gcc/infrastructure/"
CLOOG_FILE="cloog-ppl-$CLOOG_VERSION.tar.gz"
CLOOG_DIR="cloog-ppl-$CLOOG_VERSION"
LIBELF_URL="http://www.mr511.de/software/"
LIBELF_FILE="libelf-$LIBELF_VERSION.tar.gz"
LIBELF_DIR="libelf-$LIBELF_VERSION"

##libiconv
LIBICONV_URL="http://ftp.gnu.org/pub/gnu/libiconv"
LIBICONV_FILE="libiconv-$LIBICONV_VERSION.tar.gz"
LIBICONV_DIR="libiconv-$LIBICONV_VERSION"

##newlib
NEWLIB_URL="ftp://sources.redhat.com/pub/newlib"
NEWLIB_FILE="newlib-$NEWLIB_VERSION.tar.gz"
NEWLIB_DIR="newlib-$NEWLIB_VERSION"

##autoconf
AUTOCONF_URL="http://ftp.gnu.org/gnu/autoconf"
AUTOCONF_FILE="autoconf-$AUTOCONF_VERSION.tar.bz2"
AUTOCONF_DIR="autoconf-$AUTOCONF_VERSION"

##automake
AUTOMAKE_URL="http://ftp.gnu.org/gnu/automake"
AUTOMAKE_FILE="automake-$AUTOMAKE_VERSION.tar.bz2"
AUTOMAKE_DIR="automake-$AUTOMAKE_VERSION"

##termcap
TERMCAP_URL="http://ftp.gnu.org/gnu/termcap"
TERMCAP_FILE="termcap-$TERMCAP_VERSION.tar.gz"
TERMCAP_DIR="termcap-$TERMCAP_VERSION"

##gdb
GDB_URL="http://ftp.gnu.org/gnu/gdb"
GDB_FILE="gdb-$GDB_VERSION.tar.bz2"
GDB_DIR="gdb-$GDB_VERSION"

##grub
GRUB_URL="ftp://alpha.gnu.org/gnu/grub"
GRUB_FILE="grub-${GRUB_VERSION}.tar.gz"
GRUB_DIR="grub-${GRUB_VERSION}"

##mtools
MTOOLS_URL="ftp://ftp.gnu.org/gnu/mtools"
MTOOLS_FILE="mtools-${MTOOLS_VERSION}.tar.bz2"
MTOOLS_DIR="mtools-${MTOOLS_VERSION}"

##nasm
NASM_URL="http://www.nasm.us/pub/nasm/releasebuilds/${NASM_VERSION}"
NASM_FILE="nasm-${NASM_VERSION}.tar.bz2"
NASM_DIR="nasm-${NASM_VERSION}"


##################
## script config #
##################

BUILD_SIZE="1.5 GB"

##directory config
CUR_DIR=$(pwd)
INSTALL_DIR="${CUR_DIR}/install"
DOWNLOAD_DIR="${CUR_DIR}/downloads"
DIST_DIR="${CUR_DIR}/dist"
PATCH_DIR="${CUR_DIR}/patches"
BUILD_DIR="${CUR_DIR}/build"
LOG_FILE="${BUILD_DIR}/build.log"



########
## ask #
########

echo "At least $BUILD_SIZE of space is requiered to build the toolchain."

read -p "Do you wish to proceed? [y/n]" -n 1 ANS
echo ""

if [ "$ANS" != "y" ]; then
	echo "Aborting"
	exit 0
fi

##############
## functions #
##############

###
## log_command <COMMAND...>
## 		executes <COMMAND...> an redirects the output to the log file
function log_command
{
	CMD="$*"
	[ -e "$LOG_FILE" ] || { mkdir -p "$(dirname "$LOG_FILE")"; touch "$LOG_FILE"; }
	if [[ "$DRY" == "yes" ]]; then
		echo "$CMD" 1>>"$LOG_FILE" 2>>"$LOG_FILE"
	else
		echo "************************************************" >>"$LOG_FILE"
		echo "* RUNNING COMMAND $CMD *" >>"$LOG_FILE"
		echo "************************************************" >>"$LOG_FILE"
		if ( ! $CMD 1>>"$LOG_FILE" 2>>"$LOG_FILE" ); then
			echo "Error! See build.log for details"
			echo "ERROR: ABORTING" >>"$LOG_FILE"
			exit -1
		fi
	fi
}

###
## prepare
## 		sets up the environments
function prepare
{
	#remove old log
	rm -f "$LOG_FILE"

	#creating directorys
	log_command mkdir -p "$INSTALL_DIR"
	log_command mkdir -p "$INSTALL_DIR/bin"
	log_command mkdir -p "$DOWNLOAD_DIR"
	log_command mkdir -p "$BUILD_DIR"
	log_command mkdir -p "$DIST_DIR"
	check_link "$INSTALL_DIR/$TARGET" "$INSTALL_DIR/usr"

	#cleaning path
	PATH="/usr/local/gnu/bin"				# arbi special
	PATH="$PATH:/usr/local/bin"				# normal
	PATH="$PATH:/usr/bin"					# normal
	PATH="$PATH:/bin"						# normal
	PATH="$PATH:/usr/local/lib/gcc4/bin"	# arbi special
	PATH="$INSTALL_DIR/bin:$PATH"
	export PATH
	log_command export PATH="$PATH"

	#setting host
	HOST="$(gcc -dumpmachine)"
}

###
## info
##		dumps information about the local GCC
function info
{
	log_command echo "Build using path: $PATH"
	log_command echo "Build for target: $TARGET"
	log_command echo "Build on host: $HOST"
	log_command echo "Build using GCC-Version $(gcc -dumpversion)" 
}

###
## download <URL> <FILE>
##		tries to download <FILE> from <URL>
function download
{
	local URL="$1"
	local FILE="$2"
	if [ ! -e "$DOWNLOAD_DIR/$FILE" ]; then
		echo -n "Downloading $FILE... "
		log_command mkdir -p "$DOWNLOAD_DIR"
		log_command wget --directory-prefix="$DOWNLOAD_DIR" "$URL/$FILE"
	fi
}

###
## extract_file <FILE> <DESTINATION>
##		extracts the content of the archive <FILE> to the folder <DESTINATION>
function extract_file
{
	FILE="$DOWNLOAD_DIR/$1"
	DEST="$2"
	echo -n "Extracting $FILE... "
	case $FILE in
		*.zip)
			log_command unzip -d "$DEST" "$FILE"
			;;
		*.tar.gz|*.tgz)
			log_command tar -C "$DEST" -xzf "$FILE"
			;;
		*.tar.bz2)
			log_command tar -C "$DEST" -xjf "$FILE"
			;;
		*.tar.lzma)
			log_command tar -C "$DEST" --lzma -xf "$FILE"
			;;
		*)
			echo "FAILED: Unsupported archive"
			exit -1
			;;
	esac
}

###
## get_sources <PACKAGE> <URL> <FILES...>
##		downloads <FILES...> from <URL> and extracts <FILES...> to <DIST_DIR>
function get_sources
{
	local PACKAGE="$1"; shift
	local URL="$1"; shift
	local FILES="$*"
	echo -n "Checking presence of $PACKAGE..."
	for FILE in $FILES; do
		download "$URL" "$FILE"
	done
	if [ ! -d "$DIST_DIR/$PACKAGE" ]; then
		for FILE in $FILES; do
			extract_file "$FILE" "$DIST_DIR"
		done
	fi
	echo "[ OK ]"
}

###
## check_link <FROM> <TO>
##		check if <TO> is a link or creates a link from <FROM> to <TO>
function check_link
{
	FROM="$1"
	TO="$2"
	echo -n "Checking link $TO... "
	if ( ! [ -h "$TO" ] || ! [ "$(readlink "$TO")" == "$FROM" ] ); then
		[ -h "$TO" ] && log_command unlink "$TO"
		echo -n "Creating Link $FROM -> $TO..."
		log_command ln -s "$FROM" "$TO"
	fi
	echo "[ OK ]"
}

###
## configure_it <PACKAGE> <OPTIONS...>
##		invokes configure for package <PACKAGE> in folder <BUILD_DIR>/<PACKAGE>
##		using parameters <OPTIONS>
function configure_it
{
	CONF_PACKAGE="$1"
	shift
	CONFIGURE_EXTRA_OPTIONS="$*"
	if [ ! -e "$BUILD_DIR/.$CONF_PACKAGE.configure" ]; then
		echo -n "Configuring $CONF_PACKAGE... "
		log_command mkdir -p "$BUILD_DIR/$CONF_PACKAGE"
		log_command cd "$BUILD_DIR/$CONF_PACKAGE"
		cd "$BUILD_DIR/$CONF_PACKAGE"
		log_command	$DIST_DIR/$CONF_PACKAGE/configure --prefix="$INSTALL_DIR" $CONFIGURE_EXTRA_OPTIONS
		touch "$BUILD_DIR/.$CONF_PACKAGE.configure"
	fi
}

###
## make_it <PACKAGE> <TARGET>
## 		invokes make for the package <PACKAGE> in folder <BUILD_DIR>/<PACKAGE>
##		with target <TARGET>:
##			"make_it qemu all" := "cd <BUILD_DIR>/qemu; make all"
function make_it
{
	MAKE_PACKAGE="$1"
	MAKE_TARGET="$2"
	if [ ! -e "$BUILD_DIR/.$MAKE_PACKAGE.make" ]; then
		echo -n "Making $MAKE_PACKAGE... "
		log_command mkdir -p "$BUILD_DIR/$MAKE_PACKAGE"
		log_command cd "$BUILD_DIR/$MAKE_PACKAGE"
		cd "$BUILD_DIR/$MAKE_PACKAGE"
		log_command make $MAKE_OPTIONS $MAKE_TARGET
		touch "$BUILD_DIR/.$MAKE_PACKAGE.make"
	fi
}

###
## install_it <PACKAGE> <TARGET>
##		invokes make for the package <PACKAGE> in folder <BUILD_DIR>/<PACKAGE>
##		with target <TARGET>:
##			"make_it qemu all" := "cd <BUILD_DIR>/qemu; make all"
function install_it
{
	INST_PACKAGE="$1"
	MAKE_TARGET="$2"
	if [ ! -e "$BUILD_DIR/.$INST_PACKAGE.install" ]; then
		echo -n "Installing $INST_PACKAGE... "
		log_command mkdir -p "$BUILD_DIR/$INST_PACKAGE"
		log_command cd "$BUILD_DIR/$INST_PACKAGE"
		cd "$BUILD_DIR/$INST_PACKAGE"
		log_command make $MAKE_OPTIONS $MAKE_TARGET
		touch "$BUILD_DIR/.$INST_PACKAGE.install"
	fi
}

###
## clean_it <PACKAGE>
##		cleans the build folder for package <PACKAGE>
function clean_it
{
	CLEAN_PACKAGE="$1"
	if ( [ ! -e "$BUILD_DIR/.$CLEAN_PACKAGE.build_clean" ] && [ "$AUTOCLEAN" == "yes" ] ); then
		echo -n "Cleaning Build $CLEAN_PACKAGE... "
		log_command rm -rf "$BUILD_DIR/$CLEAN_PACKAGE"
		touch "$BUILD_DIR/.$CLEAN_PACKAGE.build_clean"
	fi
}

###
## patch_it <PACKAGE>
##		patches package <PACKAGE> using the patch file in
##		<PATCH_DIR>/<PACKAGE>.patch
function patch_it
{
	PATCH_PACKAGE="$1"
	if [ ! -e "$DIST_DIR/.$PATCH_PACKAGE.patch" ]; then
		echo -n "Patching $PATCH_PACKAGE... "
		log_command patch -p0 -d "$DIST_DIR" < "$PATCH_DIR/$PATCH_PACKAGE.patch"
		touch "$DIST_DIR/.$PATCH_PACKAGE.patch"
		echo "[ OK ]"
	fi
}

###
## invoke_autoconf <PACKAGE> <DIR...>
##		invokes autoconf for package <PACKAGE> in all folders <PACKAGE>/<DIR>
function invoke_autoconf
{
	AUTOCONF_PACKAGE="$1"
	shift
	RECONF_DIRS="$*"
	echo -n "Checking autoconf status of $AUTOCONF_PACKAGE... "
	if [ ! -e "$DIST_DIR/.$AUTOCONF_PACKAGE.autoconf" ]; then
		for DIR in $RECONF_DIRS; do
			echo -n "Invoking autoconf for $DIR... "
			log_command cd "$DIST_DIR/$AUTOCONF_PACKAGE/$DIR"
			cd "$DIST_DIR/$AUTOCONF_PACKAGE/$DIR"
			log_command autoconf
		done
		touch "$DIST_DIR/.$AUTOCONF_PACKAGE.autoconf"
	fi
	echo "[ OK ]"
}

###
## invoke_autoreconf <PACKAGE>
##		invokes autoreconf for package <PACKAGE> in all folders <PACKAGE>/<DIR>
function invoke_autoreconf
{
	AUTOCONF_PACKAGE="$1"
	shift
	RECONF_DIRS="$*"
	echo -n "Checking autoreconf status of $AUTOCONF_PACKAGE... "
	if [ ! -e "$DIST_DIR/.$AUTOCONF_PACKAGE.autoreconf" ]; then
		for DIR in $RECONF_DIRS; do
			echo -n "Invoking autoreconf for $DIR... "
			log_command cd "$DIST_DIR/$AUTOCONF_PACKAGE/$DIR"
			cd "$DIST_DIR/$AUTOCONF_PACKAGE/$DIR"
			log_command autoreconf
		done
		touch "$DIST_DIR/.$AUTOCONF_PACKAGE.autoreconf"
	fi
	echo "[ OK ]"
}

###
## build_it <PACKAGE> <OPTIONS>
##		invokes configure <OPTIONS>, make, make install for package <PACKAGE>
function build_it
{
	PACKAGE="$1"
	shift
	CONFIGURE_EXTRA_OPTIONS="$*"
	echo -n "Checking status of $PACKAGE... "
	configure_it $PACKAGE $CONFIGURE_EXTRA_OPTIONS
	make_it $PACKAGE
	install_it $PACKAGE install
	clean_it $PACKAGE
	echo "[ OK ]"
}

###
## build_minimal_gcc <PACKAGE> <OPTIONS>
##		special function for build the gcc (since this must been done in two
##		stages). This builds a basic gcc needed for some libs. The second stage
##		of gcc depends on these libs.
##		invokes configure <OPTIONS>, make, make install for package <PACKAGE>
function build_minimal_gcc
{
	PACKAGE="$1"
	shift
	CONFIGURE_EXTRA_OPTIONS="$*"
	echo -n "Checking status of minimal $PACKAGE... "
	if [ ! -e "$BUILD_DIR/.$PACKAGE.minimal" ]; then
		configure_it $PACKAGE $CONFIGURE_EXTRA_OPTIONS
		make_it $PACKAGE all-gcc
		install_it $PACKAGE install-gcc
		rm "$BUILD_DIR/.$PACKAGE.configure"
		rm "$BUILD_DIR/.$PACKAGE.make"
		rm "$BUILD_DIR/.$PACKAGE.install"
		touch "$BUILD_DIR/.$PACKAGE.minimal"
	fi
	echo "[ OK ]"
}

####################
## install routine #
####################

##prepare
prepare

HOST_BUILD="--build=$HOST --host=$HOST"
TARGET_BUILD="--build=$HOST --host=$HOST --target=$TARGET"

##show some informations
info

## get sources
get_sources "$AUTOCONF_DIR" "$AUTOCONF_URL" "$AUTOCONF_FILE"
get_sources "$AUTOMAKE_DIR" "$AUTOMAKE_URL" "$AUTOMAKE_FILE"
get_sources "$GRUB_DIR" "$GRUB_URL" "$GRUB_FILE"
get_sources "$LIBICONV_DIR" "$LIBICONV_URL" "$LIBICONV_FILE"
get_sources "$MTOOLS_DIR" "$MTOOLS_URL" "$MTOOLS_FILE"
get_sources "$NASM_DIR" "$NASM_URL" "$NASM_FILE"
get_sources "$TEXINFO_DIR" "$TEXINFO_URL" "$TEXINFO_FILE"

get_sources "$BINUTILS_DIR" "$BINUTILS_URL" "$BINUTILS_FILE"
get_sources "$NEWLIB_DIR" "$NEWLIB_URL" "$NEWLIB_FILE"
get_sources "$GCC_DIR" "$GCC_URL" "$GCC_FILE" "$GXX_FILE"
get_sources "$GMP_DIR" "$GMP_URL" "$GMP_FILE"
get_sources "$MPFR_DIR" "$MPFR_URL" "$MPFR_FILE"
get_sources "$MPC_DIR" "$MPC_URL" "$MPC_FILE"
get_sources "$PPL_DIR" "$PPL_URL" "$PPL_FILE"
get_sources "$CLOOG_DIR" "$CLOOG_URL" "$CLOOG_FILE"
get_sources "$LIBELF_DIR" "$LIBELF_URL" "$LIBELF_FILE"

[ "$INCLUDE_GDB" == "yes" ] && get_sources "$TERMCAP_DIR" "$TERMCAP_URL" "$TERMCAP_FILE"
[ "$INCLUDE_GDB" == "yes" ] && get_sources "$GDB_DIR" "$GDB_URL" "$GDB_FILE"



##create links
check_link "$DIST_DIR/$NEWLIB_DIR/newlib" "$DIST_DIR/$GCC_DIR/newlib"
check_link "$DIST_DIR/$NEWLIB_DIR/libgloss" "$DIST_DIR/$GCC_DIR/libgloss"
##hack: make a symlink from build_dir to dist_dir because nasm configure is buggy
check_link "$DIST_DIR/$NASM_DIR" "$BUILD_DIR/$NASM_DIR"



##build autoconf we need it for patching...
build_it "$AUTOCONF_DIR" $HOST_BUILD $STATIC_BUILD
build_it "$AUTOMAKE_DIR" $HOST_BUILD $STATIC_BUILD

##find gcc version for grub (workaround)
if [ "$(gcc -dumpversion)" \> "4.2" ]; then
	if [ -e "$(which gcc-4.1 2>/dev/null)" ]; then
		GRUB_GCC="$(which gcc-4.1)"
	else
		echo "WARNING grub might need gcc prior to 4.2 because later version might fail to compile"
		GRUB_GCC="$(which gcc)"
	fi
else
	GRUB_GCC="$(which gcc)"
fi

##build bootloader for host
build_it "$GRUB_DIR" $HOST_BUILD "CC=$GRUB_GCC"

##build libiconv for host for mtools
build_it "$LIBICONV_DIR" $HOST_BUILD

##build mtools for host
build_it "$MTOOLS_DIR" $HOST_BUILD --without-x

##build nasm for host
build_it "$NASM_DIR" $HOST_BUILD

build_it "$TEXINFO_DIR" $HOST_BUILD

##patching
patch_it "$BINUTILS_DIR"
patch_it "$GCC_DIR"
patch_it "$NEWLIB_DIR"
[ "$INCLUDE_GDB" == "yes" ] && patch_it "$GDB_DIR"

##autoconf shit missing
invoke_autoconf "$GCC_DIR" "libstdc++-v3"
invoke_autoconf "$NEWLIB_DIR" "newlib/libc/sys"
invoke_autoreconf "$NEWLIB_DIR" "newlib/libc/sys/iPosix"

##build binutils
build_it "$BINUTILS_DIR" $TARGET_BUILD $CROSS_FLAGS $STATIC_BUILD

## mathlibs
LIBS=""
[ "$INCLUDE_LIBELF" == "yes" ] && build_it "$LIBELF_DIR" $HOST_BUILD $CROSS_FLAGS
[ "$INCLUDE_LIBELF" == "yes" ] && LIBS="$LIBS --with-libelf=$INSTALL_DIR"
[ "$INCLUDE_GMP" == "yes" ] && build_it "$GMP_DIR" $HOST_BUILD $CROSS_FLAGS --enable-cxx
[ "$INCLUDE_GMP" == "yes" ] && LIBS="$LIBS --with-gmp=$INSTALL_DIR"
[ "$INCLUDE_MPFR" == "yes" ] && build_it "$MPFR_DIR" $HOST_BUILD $CROSS_FLAGS $LIBS
[ "$INCLUDE_MPFR" == "yes" ] && LIBS="$LIBS --with-mpfr=$INSTALL_DIR"
[ "$INCLUDE_MPC" == "yes" ] && build_it "$MPC_DIR" $HOST_BUILD $CROSS_FLAGS $LIBS
[ "$INCLUDE_MPC" == "yes" ] && LIBS="$LIBS --with-mpc=$INSTALL_DIR"
[ "$INCLUDE_CLOOG" == "yes" ] && build_it "$PPL_DIR" $HOST_BUILD $CROSS_FLAGS $LIBS
[ "$INCLUDE_CLOOG" == "yes" ] && LIBS="$LIBS --with-ppl=$INSTALL_DIR"
[ "$INCLUDE_CLOOG" == "yes" ] && build_it "$CLOOG_DIR" $HOST_BUILD $CROSS_FLAGS $LIBS --without-polylib
[ "$INCLUDE_CLOOG" == "yes" ] && LIBS="$LIBS --with-cloog=$INSTALL_DIR"

##build minimal gcc - needed for building newlib
build_minimal_gcc "$GCC_DIR" $TARGET_BUILD $CROSS_FLAGS $GCC_FLAGS $LIBS

##build newlib
build_it "$NEWLIB_DIR" $TARGET_BUILD $CROSS_FLAGS $LIBS

##build rest of gcc
build_it "$GCC_DIR" $TARGET_BUILD $CROSS_FLAGS $GCC_FLAGS $LIBS

##build termcap for gdb
[ "$INCLUDE_GDB" == "yes" ] && build_it "$TERMCAP_DIR" $COMMON_FLAGS

##build gdb
[ "$INCLUDE_GDB" == "yes" ] && build_it "$GDB_DIR" $COMMON_FLAGS --disable-werror

exit 0
