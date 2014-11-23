include(AddInstallTarget)

# image-build root
set(IPOSIX_IMAGE_ROOT ${CMAKE_BINARY_DIR}/image/root)

# determine paths of utility programs
set(CMAKE_PREFIX_PATH ${Toolchain_Prefix})
foreach(PROGRAM mtools grub)
	find_program(PROG_${PROGRAM} ${PROGRAM})
endforeach()

# set location of mtools configuration
set(MTOOLSRC ${CMAKE_CURRENT_SOURCE_DIR}/mtools.conf)

# define image building environment
set(IMAGEENV MTOOLSRC=${MTOOLSRC})

# set disk image geometry
set(IPOSIX_IMAGE_SECTOR_SIZE 512)
set(IPOSIX_IMAGE_SECTORS 63)
set(IPOSIX_IMAGE_CYLINDERS 40)
set(IPOSIX_IMAGE_HEADS 16)
math(EXPR IPOSIX_IMAGE_SIZE "${IPOSIX_IMAGE_SECTOR_SIZE} * ${IPOSIX_IMAGE_CYLINDERS} * ${IPOSIX_IMAGE_HEADS} * ${IPOSIX_IMAGE_SECTORS}")     ## Size * CHS

# define image installation targets and dependencies
add_custom_target(system-root)
add_custom_target(root)
add_install_target(kernel-install kernel kernel)
add_install_target(userland-install userland userland)
add_install_target(system-root-install root root)
add_dependencies(system-root system-root-install kernel-install userland-install)

# copy root-filesystem hierarchy
file(GLOB DIRS "root/*")
install(DIRECTORY ${DIRS} COMPONENT root DESTINATION ${IPOSIX_IMAGE_ROOT})

file(GLOB GRUB_COMPONENTS "${Toolchain_Prefix}/lib/grub/*/*")

# compress kernel
add_custom_command(
	OUTPUT ${IPOSIX_IMAGE_ROOT}/boot/kernel.gz
	COMMAND gzip -9 < ${IPOSIX_IMAGE_ROOT}/boot/kernel > ${IPOSIX_IMAGE_ROOT}/boot/kernel.gz
	DEPENDS ${IPOSIX_IMAGE_ROOT}/boot/kernel
)

add_custom_command(
	OUTPUT grub_install_script
	COMMAND echo "device \\(hd0\\) ../iposix.img" > grub_install_script
	COMMAND echo "root \\(hd0,0\\)" >> grub_install_script
	COMMAND echo "setup \\(hd0\\)" >> grub_install_script
)

set(COPY_TARGETS
	bin
	boot
)

# build disk image
add_custom_target(image
	COMMAND head -c ${IPOSIX_IMAGE_SIZE} /dev/zero > ../iposix.img
	COMMAND env ${IMAGEENV} ${PROG_mtools} -c mpartition -I p:
	COMMAND env ${IMAGEENV} ${PROG_mtools} -c mpartition -c -s ${IPOSIX_IMAGE_SECTORS} -t ${IPOSIX_IMAGE_CYLINDERS} -h ${IPOSIX_IMAGE_HEADS} p:
	COMMAND env ${IMAGEENV} ${PROG_mtools} -c mformat p:
	COMMAND env ${IMAGEENV} ${PROG_mtools} -c mmd p:/boot
	COMMAND env ${IMAGEENV} ${PROG_mtools} -c mmd p:/boot/grub

	COMMAND env ${IMAGEENV} ${PROG_mtools} -c mcopy -Q -m ${GRUB_COMPONENTS} p:/boot/grub
	COMMAND cat ${CMAKE_CURRENT_BINARY_DIR}/grub_install_script | ${PROG_grub} --device-map=/dev/null
	#	COMMAND ${PROG_grub-install} --force --modules="biosdisk fat part_msdos" --root-directory=. --grub-setup=/bin/true --grub-probe=/bin/true --grub-mkimage=/bin/true /dev/null || true
	#	COMMAND ${PROG_grub-mkimage} -O i386-pc --output=boot/grub/core.img --prefix=/boot/grub biosdisk fat part_msdos
	#	COMMAND ${PROG_grub-setup} --directory=boot/grub --device-map=boot/grub/device.map "--root-device=(hd9,1)" "(hd9)"
	COMMAND env ${IMAGEENV} ${PROG_mtools} -c mcopy -Q -m -s ${COPY_TARGETS} p:

	DEPENDS root/boot/kernel.gz grub_install_script
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/root
	VERBATIM
)

# set image dependencies
add_dependencies(image system-root)
