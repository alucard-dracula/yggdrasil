#!/bin/bash

var_clang_tag="clang-darwin$(clang --version | awk '/version/ {print $4}' | cut -d. -f1)"

rm -f ./tool_conv_to_darwin_cmakelists_txt
cp -f ../Release-x64/${var_clang_tag}/tool_conv_to_darwin_cmakelists_txt ./
