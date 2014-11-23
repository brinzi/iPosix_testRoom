# which programs should be used

#little tools
FIND	= $(shell which find)
RM		= $(shell which rm)
GREP	= $(shell which grep)
SED		= $(shell which sed)

#compilers
CC		= $(TOOLS_DIR)/bin/i686-pc-iPosix-gcc
CXX		= $(TOOLS_DIR)/bin/i686-pc-iPosix-g++
LD		= $(TOOLS_DIR)/bin/i686-pc-iPosix-ld
#GDB		= $(TOOLS_DIR)/bin/i686-pc-iPosix-gdb
GDB		= $(shell which gdb)
NASM	= $(TOOLS_DIR)/bin/nasm

#documentation generation tool
DOXYGEN = $(shell which doxygen)

#simulators
QEMU	= $(shell which qemu)
BOCHS	= $(shell which bochs)

#general QEMU options
QEMU_OPTIONS	 = -cpu pentium3					# simulate a pentium 3 CPU
QEMU_OPTIONS	+= -no-reboot						# Exit instead of rebooting
QEMU_OPTIONS	+= -soundhw pcspk					# emulate a pc speaker
QEMU_OPTIONS	+= -serial file:serial.out			# redirects the serial port to this file
QEMU_OPTIONS	+= -monitor stdio
QEMU_OPTIONS	+= -m 512M

#qemu options
QEMU_LOG_ITEMS	= int,pcall,exec,in_asm			# log interrupts
												# log protected mode calls
												# trace (lots of logs)
												# show target assembly code

############################
# general compiler options #
############################

# where to find the system root
GCC_SYSROOT=$(TOOLS_DIR)

CFLAGS  = -m32									# compile in 32bit mode
CFLAGS  += -O0									# Optimisation Level
CFLAGS += -ffreestanding						# compile for a environment with no stdlib
CFLAGS += -ggdb									# compile debug symbols

CXXFLAGS  = $(CFLAGS)
CXXFLAGS += -ansi								# this code should be ansi compatible
CXXFLAGS += -W									# Warn
CXXFLAGS += -Wall								# Warn all
CXXFLAGS += -Wabi								# Warn an non standard abi function used
CXXFLAGS += -Wextra								# Warn more
CXXFLAGS += -Wold-style-cast					# Warn more and more
CXXFLAGS += -pedantic							# Warn a lot
CXXFLAGS += -Wno-long-long						# do not warn if long long is used
CXXFLAGS += -Wlogical-op						# warn on use of logical ops if no logical 
CXXFLAGS += -std=c++0x -DCXX0X
												# op is suggested
#CXXFLAGS += -Waggregate-return					# TODO usefull?
CXXFLAGS += -Wmissing-noreturn					# warn on non returning functions
CXXFLAGS += -Wmissing-format-attribute			# ???
CXXFLAGS += -Wunsafe-loop-optimizations			# warn on unsage loop optimizations
#CXXFLAGS += -Wpacked							# TODO usefull? warn if packed attribute does not effect
CXXFLAGS += -Wredundant-decls					# warn on decls in same namespace
#CXXFLAGS += -Wunreachable-code					# TODO usefull?
#CXXFLAGS += -Winline							# TODO usefull?
CXXFLAGS += -Wformat							# ???
CXXFLAGS += -Wformat=2							# ???
CXXFLAGS += -Wctor-dtor-privacy					# warn on un usefull classes 
												# (no-static,no-ctor,no-dtor)
CXXFLAGS += -Wnon-virtual-dtor					# warn on class with virtual funcs and 
												# no virtual dtor
CXXFLAGS += -Wsign-promo						# warn on signedness cast aways
CXXFLAGS += -Woverloaded-virtual				# warn on overloaded virtual function which
												# hiddes the virtual ones

#CXXFLAGS += -Weffc++							# do not use this see gcc bug #14482

# declare the sysroot used during compile time
CXXFLAGS += --sysroot=$(GCC_SYSROOT)

# configure the linker target
MACHINE_TYPE=elf_i386

#linker options	
LDFLAGS = -m$(MACHINE_TYPE)						# set machine type
LDFLAGS+= --oformat=elf32-i386					# links as elf

% vim:ts=4:tw=0:ft=make:
