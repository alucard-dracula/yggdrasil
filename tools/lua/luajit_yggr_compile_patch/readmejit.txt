copy LuaJIT-2.1/* to LuaJIT-2.1 source dir

win:
luajit_make_vc100_yggr.bat
luajit_make_vc143_yggr.bat
luajit_make_mgw14_yggr.bat

linux:
luajit_make_linux_yggr.sh

android:
build-luajit210-android-32.sh // using msys2
build-luajit210-android-64.sh // using msys2

darwin:
build-luajit21-darwin16_yggr.sh

note:
1.  When compiling luajit with mingw,  
since mingw-gcc can't use the -m option to link the correct library files,
it needs to compile the 64-bit and 32-bit versions separately