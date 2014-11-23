#!/usr/bin/env bash
if [[ -z "$1" ]]; then
	echo "usage: umount_floppy.sh <mount-point>"
	exit -1
fi

MOUNT_PNT="$1"

echo "unmount image ... "
fusermount -u "$MOUNT_PNT"
if [[ ! $? -eq "0" ]]; then
	echo "error aborting..."
	exit -1
fi
