#!/bin/bash

if [ ! "${LD_LIBRARY_PATH}" = "" ]; then
    PATH=${PATH_TMP}
    LD_LIBRARY_PATH=${LD_LIBRARY_PATH_TMP}
    PATH_TMP=""
    LD_LIBRARY_PATH_TMP=""
    export LD_LIBRARY_PATH
    export LD_LIBRARY_PATH_TMP
    export PATH
    export PATH_TMP
fi

