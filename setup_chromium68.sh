#!/bin/bash
#set -x

if [ -z $WLD ]; then
	echo "WLD not set"
	exit 123
fi

export CHROME_SRC=/home/mvlad/src/agl-html5/ozone-wayland/chromium68
export CHROME_TARGET=$WLD

if [ ! -d ${CHROME_SRC}/src/out/OzoneWayland ]; then
	exit 124
fi

export CHROME_OUTDIR=${CHROME_SRC}/src/out/OzoneWayland
PATH=${CHROME_SRC}/depot_tools/:$PATH
