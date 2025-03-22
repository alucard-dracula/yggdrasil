#!/bin/sh

if [ ! "${LD_LIBRARY_PATH}" = "" ]; then
    LD_LIBRARY_PATH=${LD_LIBRARY_PATH_TMP}
    LD_LIBRARY_PATH_TMP=""
    export LD_LIBRARY_PATH
    export LD_LIBRARY_PATH_TMP
fi

