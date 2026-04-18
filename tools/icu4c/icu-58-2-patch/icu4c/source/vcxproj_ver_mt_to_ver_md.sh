#!/bin/sh

./vcxproj_ver_conv_ver.sh "-vc\$(PlatformToolsetVersion)-s" "-vc\$(PlatformToolsetVersion)"
./vcxproj_ver_conv_ver.sh "<RuntimeLibrary>MultiThreadedDebug<\/RuntimeLibrary>" "<RuntimeLibrary>MultiThreadedDebugDLL<\/RuntimeLibrary>"
./vcxproj_ver_conv_ver.sh "<RuntimeLibrary>MultiThreaded<\/RuntimeLibrary>" "<RuntimeLibrary>MultiThreadedDLL<\/RuntimeLibrary>"