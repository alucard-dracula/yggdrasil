#!/bin/sh

if [[ "${OSTYPE}" =~ "darwin" ]]; then
   var_dyld_lib_path="/Users/alucard/third_part/boost_182_x64/lib"
   var_dyld_lib_path="${var_dyld_lib_path}:/Users/alucard/third_part/libicu58/lib"
   #var_dyld_lib_path="${var_dyld_lib_path}:/Users/alucard/third_part/libiconv/lib"
   var_dyld_lib_path="${var_dyld_lib_path}:${DYLD_LIBRARY_PATH}"
   export DYLD_LIBRARY_PATH="${var_dyld_lib_path}"
fi


dir13="\.\."

sed -i "" "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir13}<\/root_dir_>/g" tool_conv_to_darwin_cmakelists_txt_cfg.xml
./tool_conv_to_darwin_cmakelists_txt tool_conv_to_darwin_cmakelists_txt_cfg.xml
