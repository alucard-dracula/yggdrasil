
setlocal

call "d:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cd /d %~dp0

set var_src_root_dir=stage
set var_dst_root_dir=stage_fixed

:rem set var_dst_vc100_dir=libopenssl-3.4.0-vc100
:rem set var_dst_vc143_dir=libopenssl-3.4.0-vc143
:rem set var_dst_mgw15_dir=libopenssl-3.4.0-mgw15
:rem set var_dst_mix_dir=libopenssl-3.4.0-mix

set var_lib_title=libopenssl
set var_lib_ver=3.4.0

rm -fr %var_dst_root_dir%

:rem vc100
call make_yggr_openssl_files_init "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100"

:rem vc100-x64
call make_yggr_openssl_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x64" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x64" "release" "mt"

call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x64" "debug"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x64" "debug" "mt"

:rem vc100-x32
call make_yggr_openssl_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x32" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x32" "release" "mt"

call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x32" "debug"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "vc100" "x32" "debug" "mt"

:rem vc143
call make_yggr_openssl_files_init "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143"

:rem vc143-x64
call make_yggr_openssl_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x64" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x64" "release" "mt"

call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x64" "debug"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x64" "debug" "mt"

:rem vc143-x32
call make_yggr_openssl_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x32" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x32" "release" "mt"

call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x32" "debug"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "vc143" "x32" "debug" "mt"

:rem mgw15
call make_yggr_openssl_files_init "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mgw15"

:rem mgw15-x64
call make_yggr_openssl_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mgw15" "mgw15" "x64" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mgw15" "mgw15" "x64" "debug"

:rem mgw15-x32
call make_yggr_openssl_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mgw15" "mgw15" "x32" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mgw15" "mgw15" "x32" "debug"

:rem mix
call make_yggr_openssl_files_init "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mix"

:rem mix vc143-x64
call make_yggr_openssl_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x64" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x64" "release" "mt"

call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x64" "debug"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x64" "debug" "mt"

:rem mix vc100-x64
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x64" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x64" "release" "mt"

call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x64" "debug"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x64" "debug" "mt"

:rem mix mgw15-x64
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mgw15" "mix" "x64" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mgw15" "mix" "x64" "debug"

:rem mix vc143-x32
call make_yggr_openssl_files_first_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x32" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x32" "release" "mt"

call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x32" "debug"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc143" "mix" "x32" "debug" "mt"

:rem mix vc100-x32
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x32" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x32" "release" "mt"

call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x32" "debug"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "vc100" "mix" "x32" "debug" "mt"

:rem mix mgw15-x32
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mgw15" "mix" "x32" "release"
call make_yggr_openssl_files_copy "%var_src_root_dir%" "%var_dst_root_dir%" "%var_lib_title%" "%var_lib_ver%" "mgw15" "mix" "x32" "debug"

:debug_end

endlocal
