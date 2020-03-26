# How to build locally

- set-up an install directory then source setup_chromium68.sh (see
	the source file). You need to provide a WLD env variable for it.
- gn args out/OzoneWayland
- cp args.gn to out/OzoneWayland/args.gn
- build and install with the following:
	$ ninja -C out/OzoneWayland/ webos:weboswebruntime chrome && install_chromium68.sh

PS: Most likely you need to complete the initial set-up shown on the chromium build
page found at https://chromium.googlesource.com/chromium/src/+/master/docs/linux/build_instructions.md
but you'll now when this will be followed instead.
