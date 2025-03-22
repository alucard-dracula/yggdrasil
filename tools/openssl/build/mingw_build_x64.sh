
#!/bin/bash

var_dir=$(cd `dirname $0`; pwd)
./mingw_build_tpl.sh mingw64 ${var_dir}/stage/mingw64 ${var_dir}/stage/mingw64/ssl
