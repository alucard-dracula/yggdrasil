#!/bin/sh

if [ "${LD_LIBRARY_PATH_TMP}" = "" ]; then
    LD_LIBRARY_PATH_TMP=${LD_LIBRARY_PATH}
    export LD_LIBRARY_PATH_TMP
fi

LD_LIBRARY_PATH=/home/alucard/yggdrasil_workspace_linux/yggdrasil/Debug-x32/gcc14:${LD_LIBRARY_PATH_TMP}
export LD_LIBRARY_PATH

