cd /d %~dp0

set PATH=%PATH%;"%WindowsSdkVerBinPath%"\x86
set KRB_INSTALL_DIR=./stage-x86
set var_outdir=../stage
cd src
rm -fr "%KRB_INSTALL_DIR%"
mkdir "%KRB_INSTALL_DIR%"
mkdir "%var_outdir%"

nmake -f Makefile.in prep-windows
nmake clean NODEBUG=1
nmake NODEBUG=1
nmake install NODEBUG=1

rm -fr "%var_outdir%/bin"
rm -fr "%var_outdir%/lib"
cp -fr "%KRB_INSTALL_DIR%/bin" "%var_outdir%/bin"
cp -fr "%KRB_INSTALL_DIR%/lib" "%var_outdir%/lib"

if exist "%var_outdir%/include" goto end_fix_include

cp -fr "%KRB_INSTALL_DIR%/include" "%var_outdir%/include"

:end_fix_include

cd ..