#!/bin/sh

if [ "${LD_LIBRARY_PATH_TMP}" = "" ]; then
    LD_LIBRARY_PATH_TMP=${LD_LIBRARY_PATH}
    export LD_LIBRARY_PATH_TMP
fi

LD_LIBRARY_PATH=~/yggdrasil-android-riscv64/bin-debug:~/../usr/lib:${LD_LIBRARY_PATH_TMP}
ICU_DATA=~/yggdrasil-android-riscv64/bin-debug
export LD_LIBRARY_PATH
export ICU_DATA


