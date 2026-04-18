
setlocal

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cd /d %~dp0

set var_src_root_dir=.
set var_dst_root_dir=stage_fixed

:rem set var_dst_vc100_dir=libiconv-v1.17-vc100
:rem set var_dst_vc143_dir=libiconv-v1.17-vc143
:rem set var_dst_mix_dir=libiconv-v1.17-mix

set var_lib_title=libiconv
set var_lib_ver=v1.17

rm -fr %var_dst_root_dir%

:rem vc100
call make_yggr_libiconv_files_init "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100"

:rem vc100-x64
call make_yggr_libiconv_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x64" "release"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x64" "release" "mt"

call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x64" "debug"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x64" "debug" "mt"

:rem vc100-x32
call make_yggr_libiconv_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x32" "release"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x32" "release" "mt"

call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x32" "debug"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x32" "debug" "mt"

:rem vc143
call make_yggr_libiconv_files_init "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143"

:rem vc143-x64
call make_yggr_libiconv_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x64" "release"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x64" "release" "mt"

call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x64" "debug"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x64" "debug" "mt"

:rem vc143-x32
call make_yggr_libiconv_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x32" "release"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x32" "release" "mt"

call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x32" "debug"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x32" "debug" "mt"

:rem mix
call make_yggr_libiconv_files_init "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mix"

:rem mix vc143-x64
call make_yggr_libiconv_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x64" "release"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x64" "release" "mt"

call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x64" "debug"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x64" "debug" "mt"

:rem mix vc100-x64
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x64" "release"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x64" "release" "mt"

call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x64" "debug"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x64" "debug" "mt"

:rem mix vc143-x32
call make_yggr_libiconv_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x32" "release"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x32" "release" "mt"

call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x32" "debug"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x32" "debug" "mt"

:rem mix vc100-x32
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x32" "release"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x32" "release" "mt"

call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x32" "debug"
call make_yggr_libiconv_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x32" "debug" "mt"

:debug_end

endlocal
