#!/bin/bash
KERNEL_MODULE_FILE=virtual_homekey.ko
SERVICE_FILE=load-virtual-homekey.service

PATH_SYSTEMD=/usr/lib/systemd

if [ ! -e ${KERNEL_MODULE_FILE} ]
then
	echo "Error: No kernel module (${KERNEL_MODULE_FILE})"
	exit -1
fi

if [ ! -e ${SERVICE_FILE} ]
then
	echo "Error: No service file (${SERVICE_FILE})"
	exit -2
fi

sdb root on
sdb shell mkdir -p /data/
sdb push ${KERNEL_MODULE_FILE} /data/
sdb push ${SERVICE_FILE} ${PATH_SYSTEMD}/system/
sdb shell ln -s ${PATH_SYSTEMD}/system/${SERVICE_FILE} ${PATH_SYSTEMD}/system/graphical.target.wants/${SERVICE_FILE}
echo "Install completed. Reboot target device to use this module."
