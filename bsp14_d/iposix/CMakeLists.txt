cmake_minimum_required(VERSION 2.8.3 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/image/root)
set(Toolchain_Prefix /opt/iposix-toolchain)

set(DOXYFILE_PDFLATEX YES)

include(ForceOutOfSource)
include(UseDoxygen)

project(iposix)

## process subdires and includes
add_custom_target(toolchain)
foreach(DIR image src)
	add_subdirectory(${DIR})
endforeach()

message("")
message("all [default]          build source files")
message("help                   list all targets")
#message("toolchain              build toolchain")
message("kernel                 build kernel")
message("userland               build userland programs")
message("image                  generate image")
message("run                    run image in qemu")
message("gdb                    run image in qemu with gdb attached")
message("doc                    generate documentation")
message("")
