#!/bin/sh


find ./ -type f -name '*.vcxproj' -exec sed -i 's/<WindowsTargetPlatformVersion>.*<\/WindowsTargetPlatformVersion>//g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/<PlatformToolset>.*<\/PlatformToolset>//g' {} \;
find ./ -type f -name '*.vcxproj' -exec sed -i 's/<MinimalRebuild>.*<\/MinimalRebuild>//g' {} \;