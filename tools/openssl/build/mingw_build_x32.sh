
#!/bin/bash

var_dir=$(cd `dirname $0`; pwd)
./mingw_build_tpl.sh mingw ${var_dir}/stage/mingw32 ${var_dir}/stage/mingw32/ssl

