include(iposix_image)

add_custom_target(run
	COMMAND	qemu-system-i386 -cpu pentium3 -no-reboot -soundhw pcspk -serial file:serial.out -monitor stdio -m 512M -hda iposix.img
)
add_dependencies(run image)

add_custom_target(gdb
	COMMAND	qemu-system-i386 -cpu pentium3 -no-reboot -soundhw pcspk -serial file:serial.out -monitor stdio -m 512M -hda iposix.img -s -S &
	COMMAND gdb -s ${CMAKE_BINARY_DIR}/image/root/boot/kernel -x ${CMAKE_SOURCE_DIR}/config/gdb/gdb.cmd
)
add_dependencies(gdb image)
