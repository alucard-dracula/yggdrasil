#!/bin/sh

./vcxproj_ver_conv_ver.sh "-vc\$(PlatformToolsetVersion)" "-vc\$(PlatformToolsetVersion)-s"
./vcxproj_ver_conv_ver.sh "<RuntimeLibrary>MultiThreadedDebugDLL<\/RuntimeLibrary>" "<RuntimeLibrary>MultiThreadedDebug<\/RuntimeLibrary>"
./vcxproj_ver_conv_ver.sh "<RuntimeLibrary>MultiThreadedDLL<\/RuntimeLibrary>" "<RuntimeLibrary>MultiThreaded<\/RuntimeLibrary>"