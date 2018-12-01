#!/bin/bash

set -e

SCRIPT_DIR="$(dirname $0)"
GENIMAGE_CFG="${SCRIPT_DIR}/genimage-raspberrypi.cfg"
GENIMAGE_TMP="${BUILD_DIR}/genimage.tmp"

#correct GPU memory

for arg in --gpu_mem_256=128 --gpu_mem_512=256 --gpu_mem_1024=256
do
	case "${arg}" in
		--gpu_mem_256=*|--gpu_mem_512=*|--gpu_mem_1024=*)
		# Set GPU memory
		gpu_mem="${arg:2}"
		sed -e "/^${gpu_mem%=*}=/s,=.*,=${gpu_mem##*=}," -i "${BINARIES_DIR}/rpi-firmware/config.txt"
		;;
	esac
done

#add start_x=1
cat << __EOF__ >> "${BINARIES_DIR}/rpi-firmware/config.txt"

# enable CSI camera
start_x=1
__EOF__

#add missing start_x binaries
cp ${SCRIPT_DIR}/fixup_x.dat ${SCRIPT_DIR}/start_x.elf ${BINARIES_DIR}/rpi-firmware



rm -rf "${GENIMAGE_TMP}"

genimage                           \
	--rootpath "${TARGET_DIR}"     \
	--tmppath "${GENIMAGE_TMP}"    \
	--inputpath "${BINARIES_DIR}"  \
	--outputpath "${BINARIES_DIR}" \
	--config "${GENIMAGE_CFG}"

exit $?
