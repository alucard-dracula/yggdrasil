
#!/bin/bash

var_pl=$1
var_outdir=$2
var_ssldir=$3
var_static=$4

perl Configure ${var_pl} no-asm --prefix=${var_outdir} --openssldir=${var_ssldir} ${var_static}
make depend
make -j12
make install
make clean
