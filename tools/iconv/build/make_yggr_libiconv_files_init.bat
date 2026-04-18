cd /d %~dp0

set var_src_root_dir=%~1
set var_dst_root_dir=%~2

:rem (e.g. libiconv)
set var_dst_dir_title=%~3

:rem (e.g. v1.17)
set var_iconv_ver=%~4

:rem (e.g. vc100 vc143 mix)
set var_dst_dir_mark=%~5

set var_dst_dir=%var_dst_dir_title%-%var_iconv_ver%-%var_dst_dir_mark%

mkdir "%var_dst_root_dir%/%var_dst_dir%/bin"
mkdir "%var_dst_root_dir%/%var_dst_dir%/bin64"
mkdir "%var_dst_root_dir%/%var_dst_dir%/lib"
mkdir "%var_dst_root_dir%/%var_dst_dir%/lib64"


