#!/bin/sh

if [ "${DYLD_LIBRARY_PATH_TMP}" = "" ]; then
    DYLD_LIBRARY_PATH_TMP=${DYLD_LIBRARY_PATH}
    export DYLD_LIBRARY_PATH_TMP
fi

DYLD_LIBRARY_PATH=/Users/alucard/yggdrasil_macos_project/yggdrasil/lib/Debug-x64:${DYLD_LIBRARY_PATH_TMP}
export DYLD_LIBRARY_PATH

