#!/bin/sh

find ./ -type f -name '*.vcxproj' -exec sed -i 's/<WindowsTargetPlatformVersion>.*<\/WindowsTargetPlatformVersion>/<WindowsTargetPlatformVersion>10.0<\/WindowsTargetPlatformVersion>/g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/<PlatformToolset>.*<\/PlatformToolset>/<PlatformToolset>v143<\/PlatformToolset>/g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/<MinimalRebuild>.*<\/MinimalRebuild>/<MinimalRebuild>false<\/MinimalRebuild>/g' {} \;

