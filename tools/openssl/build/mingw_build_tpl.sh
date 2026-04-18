
#!/bin/bash

var_pl=$1
var_outdir=$2
var_ssldir=$3
var_other=$4

perl Configure ${var_pl} no-asm --prefix=${var_outdir} --openssldir=${var_ssldir} ${var_other}
make depend
make -j20
make install
make clean
