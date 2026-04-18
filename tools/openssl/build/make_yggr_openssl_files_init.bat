cd /d %~dp0

set var_src_root_dir=%~1
set var_dst_root_dir=%~2

:rem (e.g. libopenssl)
set var_dst_dir_title=%~3

:rem (e.g. 3.4.0)
set var_ssl_ver=%~4

:rem (e.g. vc100 vc143 mgw15 mix)
set var_dst_dir_mark=%~5

set var_dst_dir=%var_dst_dir_title%-%var_ssl_ver%-%var_dst_dir_mark%

mkdir "%var_dst_root_dir%/%var_dst_dir%/bin"
mkdir "%var_dst_root_dir%/%var_dst_dir%/bin64"
mkdir "%var_dst_root_dir%/%var_dst_dir%/lib"
mkdir "%var_dst_root_dir%/%var_dst_dir%/lib64"


