#!/bin/bash

set -x

if [ -z ${CHROME_SRC} ]; then
	echo "No chrome src"
	exit 123
fi

if [ -z ${CHROME_TARGET} ]; then
	echo "No chrome target"
	exit 124
fi

if [ -z ${CHROME_OUTDIR} ]; then
	echo "No chrome outdir"
	exit 125
fi

if [ ! -d ${CHROME_TARGET} ]; then
	mkdir -p ${CHROME_TARGET}
fi

[ ! -d ${CHROME_TARGET}/image/usr/include/chromium68 ] && mkdir -p ${CHROME_TARGET}/image/usr/include/chromium68
[ ! -d ${CHROME_TARGET}/image/usr/lib/ ] && mkdir -p ${CHROME_TARGET}/image/usr/lib
[ ! -d ${CHROME_TARGET}/image/usr/lib/cbe ] && mkdir -p ${CHROME_TARGET}/image/usr/lib/cbe

# headers
cd ${CHROME_SRC}/src
xargs --arg-file=${CHROME_SRC}/src/webos/install/weboswebruntime/staging_inc.list cp --parents --target-directory=${CHROME_TARGET}/image/usr/include/chromium68
cd - &>/dev/null

cd ${CHROME_OUTDIR}

# libcbe
if [ ! -f libcbe.so ]; then
	echo "No libce"
	exit 123
fi
cp libcbe.so ${CHROME_TARGET}/image/usr/lib/

# no idea if this is
#cp libchromium_lttng_provider.so ${CHROME_TARGET}/image/usr/lib/

cd - &>/dev/null

# binary
cd ${CHROME_OUTDIR}
xargs --arg-file=${CHROME_SRC}/src/webos/install/weboswebruntime/binary.list cp --parents --target-directory=${CHROME_TARGET}/image/usr/lib/cbe
cd - &>/dev/null
sync
