if(${IPOSIX_ARCH} STREQUAL "x86")

	set(KERNEL_ARCH_NASM_SOURCES
		arch/x86/multiboot/startup.asm
		arch/x86/core/isr.asm
		arch/x86/scheduler/scheduler.asm
	)

	set(KERNEL_ARCH_CXX_SOURCES
		arch/x86/core/cpu_core.cpp
		arch/x86/core/gate_descriptor.cpp
		arch/x86/core/global_descriptor_table.cpp
		arch/x86/core/interrupt_descriptor_table.cpp
		arch/x86/core/isr_handler.cpp
		arch/x86/core/privilege.cpp
		arch/x86/core/task_state_segment.cpp
		arch/x86/core/memory_descriptor.cpp
		arch/x86/devices/memory.cpp
		arch/x86/devices/video.cpp
		arch/x86/devices/timer.cpp
		arch/x86/devices/keyboard.cpp
		arch/x86/devices/sound.cpp
		arch/x86/devices/serial_port.cpp
		arch/x86/elf/elf_loader.cpp
		arch/x86/ata/ata.cpp
		arch/x86/ata/ata_controller.cpp
		arch/x86/ata/ata_drive.cpp
		arch/x86/multiboot/multiboot.cpp
		arch/x86/syscalls/syscaller.cpp
		arch/x86/core/interrupts.cpp
		arch/x86/vm/vmstructs.cpp
		arch/x86/scheduler/process.cpp
		arch/x86/scheduler/process_schedule_data.cpp
		arch/x86/scheduler/scheduler.cpp
	)

	set(KERNEL_ARCH_LINK_FLAGS
		"-Xlinker -melf_i386 -Xlinker --oformat=elf32-i386 -Xlinker -T${CMAKE_SOURCE_DIR}/config/linker/link_elf.ld"
	)
endif()

set(KERNEL_SHARED_CXX_SOURCES
	iposix/main.cpp
	iposix/iposix_kernel.cpp
	lib/cppruntime/cppruntime.cpp
	lib/cppruntime/runtime_startup.cpp
	lib/cppruntime/libcalls.cpp
	devices/i_block_device.cpp
	devices/i_char_device.cpp
	devices/i_console.cpp
	devices/i_core_holder.cpp
	fs/partitiontable_crawler.cpp
	elf/i_elf_loader.cpp
	fs/filehandle.cpp
	fs/i_fs.cpp
	fs/i_fs_mounting.cpp
	fs/i_fs_node_operations.cpp
	fs/i_fs_dummy_node.cpp
	fs/i_fs_node.cpp
	fs/fat_fs.cpp
	fs/fat_fs_node.cpp
	fs/dev_fs.cpp
	fs/dev_fs_node.cpp
	lib/logger/logger.cpp
	lib/logger/message.cpp
	mm/memory_area.cpp
	scheduler/i_process.cpp
	syscalls/syscall_dispatcher.cpp
	scheduler/i_scheduler.cpp
)

set(KERNEL_TEST_SOURCES
	tests/all_tests.cpp
	tests/elf_loader_test.cpp
	tests/process_test.cpp
	tests/memory_test.cpp
	tests/i_fs_test.cpp
)

set(KERNEL_CXX_SOURCES
	${KERNEL_ARCH_CXX_SOURCES}
	${KERNEL_SHARED_CXX_SOURCES}
	${KERNEL_TEST_SOURCES}
)

set(KERNEL_NASM_SOURCES
	${KERNEL_ARCH_NASM_SOURCES}
)

add_executable(kernel ${KERNEL_NASM_SOURCES} ${KERNEL_CXX_SOURCES})		## make sure startup.asm is always linked first
include_directories(AFTER ${CMAKE_SOURCE_DIR}/src/kern/include ${CMAKE_SOURCE_DIR}/src/kern/arch ${CMAKE_SOURCE_DIR}/src/kern/lib)

#set_property(SOURCE ${KERNEL_CXX_SOURCES} PROPERTY COMPILE_FLAGS "--sysroot=${Toolchain_Prefix}")
set_property(TARGET kernel PROPERTY LINK_FLAGS "-lstdc++ -nostartfiles ${KERNEL_ARCH_LINK_FLAGS}")

install(TARGETS kernel DESTINATION boot COMPONENT kernel)
add_dependencies(kernel toolchain)

