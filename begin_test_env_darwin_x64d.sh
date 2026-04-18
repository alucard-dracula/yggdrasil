#!/bin/bash

if [ "${DYLD_LIBRARY_PATH_TMP}" = "" ]; then
    DYLD_LIBRARY_PATH_TMP=${DYLD_LIBRARY_PATH}
    export DYLD_LIBRARY_PATH_TMP
fi

PYTHONHOME=${YGGR_PYTHONHOME}
DYLD_LIBRARY_PATH=/Users/${USER}/yggdrasil_macos_project/yggdrasil/lib/Debug-x64:${DYLD_LIBRARY_PATH_TMP}

export DYLD_LIBRARY_PATH
export PYTHONHOME