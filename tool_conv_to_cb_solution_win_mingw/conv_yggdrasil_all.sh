#!/bin/sh

dir1="\.\.\/tools\/base64"
dir2="\.\.\/tools\/blowfish"
dir3="\.\.\/tools\/dtl"
dir4="\.\.\/tools\/IntelRDFPMathLib\/IntelRDFPMathLib20U2"
dir5="\.\.\/tools\/lua\/lua5_14"
dir6="\.\.\/tools\/lua\/lua5_21"
dir7="\.\.\/tools\/lua\/lua5_34"
dir8="\.\.\/tools\/lua\/luabind503p"
dir9="\.\.\/tools\/lzma\/lzma2408-compile\/C\/Util\/LzmaLib"
dir10="\.\.\/tools\/md5"
dir11="\.\.\/tools\/mongodb_c_driver\/yggr_src_1.26.1"
dir12="\.\.\/tools\/sha"
dir13="\.\."

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir1}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir2}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir3}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir4}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir5}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir6}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir7}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir8}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir9}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir10}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir11}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir12}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>${dir13}<\/root_dir_>/g" tool_conv_to_cb_solution_cfg.xml
./tool_conv_to_cb_solution_win_mingw tool_conv_to_cb_solution_cfg.xml


