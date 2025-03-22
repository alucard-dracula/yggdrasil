1, install vs2010 and vs2022 and msys2
2, msys2 install toolchain
	pacman -S base-devel mingw-w64-x86_64-toolchain mingw-w64-i686-toolchain
3, copy files to openssl root dir
4, run vc_build_all.bat  build msvc version
5, run mingw_build_x64.sh in msys2 mingw64
6, run mingw_build_x32.sh in msys2 mingw
7, run make_yggr_openssl_files.bat make last packet
8, in other cases, please modify the compilation script
9, andorid need using openssl 3.4.0, because 3.1.0 less riscv64
10, run build-openssl-darwin16.sh in macos