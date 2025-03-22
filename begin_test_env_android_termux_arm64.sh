#!/bin/sh

if [ "${LD_LIBRARY_PATH_TMP}" = "" ]; then
    LD_LIBRARY_PATH_TMP=${LD_LIBRARY_PATH}
    export LD_LIBRARY_PATH_TMP
fi

LD_LIBRARY_PATH=~/yggdrasil-android-arm64/bin:~/../usr/lib:${LD_LIBRARY_PATH_TMP}
ICU_DATA=~/yggdrasil-android-arm64/bin
export LD_LIBRARY_PATH
export ICU_DATA

