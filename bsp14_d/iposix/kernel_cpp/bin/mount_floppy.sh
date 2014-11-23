#!/usr/bin/env bash
if [[ -z "$1" || -z "$2" ]]; then
	echo "usage: mount_floppy.sh <image_file> <mount-point>"
	exit -1
fi

IMAGE_FILE="$1"
MOUNT_PNT="$2"

if [[ ! -d "$MOUNT_PNT" ]]; then
	echo "mount point doesn't exists"
	exit -1
fi

if [[ ! -f "$IMAGE_FILE" ]]; then
	echo "image doesn't exists"
	exit -1
fi

echo "mounting image ... "
fusefat -o rw+ "$IMAGE_FILE" "$MOUNT_PNT" 1>/dev/null 2>/dev/null
if [[ ! $? -eq "0" ]]; then
	echo "error aborting..."
	exit -1
fi
