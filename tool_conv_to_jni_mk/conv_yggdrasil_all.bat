cd /d %~dp0

set dir1=\.\.\/tools\/base64
set dir2=\.\.\/tools\/blowfish
set dir3=\.\.\/tools\/dtl
:rem set dir4=\.\.\/tools\/IntelRDFPMathLib\/IntelRDFPMathLib20U2
set dir5=\.\.\/tools\/lua\/lua5_14
set dir6=\.\.\/tools\/lua\/lua5_21
set dir7=\.\.\/tools\/lua\/lua5_34
:rem set dir8=\.\.\/tools\/lua\/luabind503p
:rem set dir9=\.\.\/tools\/lzma\/lzma2408-compile\/C\/Util\/LzmaLib
set dir10=\.\.\/tools\/md5
set dir11_1=\.\.\/tools\/mongodb_c_driver\/yggr_src_1.26.1
set dir11_2=\.\.\/tools\/mongodb_c_driver\/yggr_src_1.30.2
set dir12=\.\.\/tools\/sha
set dir13=\.\.

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir1%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir2%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir3%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

:rem sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir4%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
:rem tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir5%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir6%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir7%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

:rem sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir8%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
:rem tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

:rem sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir9%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
:rem tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir10%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir11_1%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir11_2%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir12%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml

sed -i "s/<root_dir_>.*<\/root_dir_>/<root_dir_>%dir13%<\/root_dir_>/g" tool_conv_to_jni_mk_cfg.xml
tool_conv_to_jni_mk.exe tool_conv_to_jni_mk_cfg.xml


