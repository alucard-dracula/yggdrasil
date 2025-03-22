#!/bin/sh

clang -arch x86_64 -E -dM - < /dev/null > macos_clang16_x64_macro.txt
clang -arch arm64 -E -dM - < /dev/null > macos_clang16_a64_macro.txt
#clang -arch x86_64 -arch arm64 -E -dM - < /dev/null > macos_clang16_ax64_macro.txt

var_sdk_name=macosx
var_sdk_ver_min=12.4
var_sdk_root=$(xcrun --sdk ${var_sdk_name} --show-sdk-path)

clang -arch arm64 -E -dM -isysroot ${var_sdk_root} -mmacosx-version-min=${var_sdk_ver_min} - < /dev/null > macosx_clang16_arm64_macro.txt
clang -arch x86_64 -E -dM -isysroot ${var_sdk_root} -mmacosx-version-min=${var_sdk_ver_min} - < /dev/null > macosx_clang16_x86_64_macro.txt

var_sdk_name=iphoneos
var_sdk_ver_min=15.6
var_sdk_root=$(xcrun --sdk ${var_sdk_name} --show-sdk-path)

clang -arch arm64 -E -dM -isysroot ${var_sdk_root} -miphoneos-version-min=${var_sdk_ver_min}  - < /dev/null > iphoneos_clang16_arm64_macro.txt


var_sdk_name=iphonesimulator
var_sdk_ver_min=15.6
var_sdk_root=$(xcrun --sdk ${var_sdk_name} --show-sdk-path)

clang -arch arm64 -E -dM -isysroot ${var_sdk_root} - < /dev/null > iphonesimulator_clang16_arm64_macro.txt
clang -arch x86_64 -E -dM -isysroot ${var_sdk_root} - < /dev/null > iphonesimulator_clang16_x86_64_macro.txt