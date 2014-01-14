#!/bin/sh
#git clone https://github.com/paul99/v8m-rb.git -b dm-dev-mipsbe
PREFIX=/home/wei.liu/TEST/build/staging_dir/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-

export STAGING_DIR=/home/wei.liu/TEST/build/staging_dir/
export CC=${PREFIX}gcc
export CXX=${PREFIX}g++
export AR=${PREFIX}ar
export RANLIB=${PREFIX}ranlib
export LINK=$CXX
LIBPATH=/home/wei.liu/TEST/build/staging_dir/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2/lib/
export LDFLAGS='-Wl,-rpath-link '${LIBPATH}

export GYPFLAGS="-Dv8_use_mips_abi_hardfloat=false -Dv8_can_use_fpu_instructions=false"

# build a standalone v8 version 
# make mips.release library=shared  snapshot=off -j4

# build the version for compiling node
make dependencies
make mips.release snapshot=off -j4
