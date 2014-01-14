
#node-v0.10.20
export STAGING_DIR=/home/wei.liu/WORK/build/staging_dir

V8SOURCE=/home/wei.liu/dm-dev-mipsbe

PREFIX=${STAGING_DIR}/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-

LIBPATH=${STAGING_DIR}/toolchain-mips_r2_gcc-4.6-linaro_uClibc-0.9.33.2/lib/


export AR=mips-openwrt-linux-uclibc-ar
export CC=${PREFIX}gcc
export CXX=${PREFIX}g++
export AR=${PREFIX}ar
export RANLIB=${PREFIX}ranlib
export LINK=$CXX

export LDFLAGS='-Wl,-rpath-link '${LIBPATH}

export TARGET_PATH=~/Cross/node/opt

export GYPFLAGS="-Dv8_use_mips_abi_hardfloat=false -Dv8_can_use_fpu_instructions=false"
make clean
make distclean

./configure  --dest-cpu=mips --dest-os=linux --without-snapshot
make snapshot=off -j4
#make install
