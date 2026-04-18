#!/bin/bash


dir8="\.\.\/tools\/lua\/luabind503p"

sed -i "" "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir8}<\/root_dir_>/g" tool_conv_to_darwin_cmakelists_txt_cfg.xml
./tool_conv_to_darwin_cmakelists_txt tool_conv_to_darwin_cmakelists_txt_cfg.xml
