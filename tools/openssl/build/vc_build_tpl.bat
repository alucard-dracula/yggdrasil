
cd /d %~dp0

set var_pl=%1
set var_outdir=%2
set var_ssldir=%3
set var_static=%4

perl Configure %var_pl% no-asm --prefix=%var_outdir% --openssldir=%var_ssldir% %var_static%

nmake
nmake install
nmake clean
