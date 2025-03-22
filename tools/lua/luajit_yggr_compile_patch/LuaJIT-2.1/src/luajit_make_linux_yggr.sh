#!/bin/sh

rm *.so
rm *.a
rm *.o
rm host/*.o

rm -fr out_lua_jit21

#mkdir -p "out_lua_jit21/include"
mkdir -p "out_lua_jit21/bin"
mkdir -p "out_lua_jit21/bin64"
mkdir -p "out_lua_jit21/lib"
mkdir -p "out_lua_jit21/lib64"

make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-d CCDEBUG=-g CFLAGS="-m64 -DLUA_USE_LINUX" LDFLAGS="-m64"


if [ ! -d "out_lua_jit21/include" ]; then
    mkdir -p "out_lua_jit21/include"
    cp -f *.h "out_lua_jit21/include/"
fi

mv -f libluajit501-gcc14-d.so out_lua_jit21/lib64/libluajit501-gcc14-d.so
mv -f libluajit501-gcc14-d.a out_lua_jit21/lib64/libluajit501-gcc14-d.a
make -f Makefile_yggr NODOTABIVER=jit501-gcc14-d CCDEBUG=-g CFLAGS="-m64 -DLUA_USE_LINUX" LDFLAGS="-m64" clean

rm *.o
rm host/*.o

make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-s-d CCDEBUG=-g BUILDMODE=static CFLAGS="-m64 -DLUA_USE_LINUX" LDFLAGS="-m64"
mv -f libluajit501-gcc14-s-d.a out_lua_jit21/lib64/libluajit501-gcc14-s-d.a
make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-s-d CCDEBUG=-g BUILDMODE=static CFLAGS="-m64 -DLUA_USE_LINUX" LDFLAGS="-m64" clean

rm *.o
rm host/*.o

make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14 CFLAGS="-O3 -m64 -DLUA_USE_LINUX" LDFLAGS="-s -m64"
mv -f libluajit501-gcc14.so out_lua_jit21/lib64/libluajit501-gcc14.so
mv -f libluajit501-gcc14.a out_lua_jit21/lib64/libluajit501-gcc14.a
cp -f luajit out_lua_jit21/bin64/lua
mv -f luajit out_lua_jit21/bin64/luajit
make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14 CFLAGS="-O3 -m64 -DLUA_USE_LINUX" LDFLAGS="-s -m64" clean

rm *.o
rm host/*.o

make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-s CFLAGS="-O3 -m64 -DLUA_USE_LINUX" LDFLAGS="-s -m64" BUILDMODE=static
mv -f libluajit501-gcc14-s.a out_lua_jit21/lib64/libluajit501-gcc14-s.a
mv -f luajit out_lua_jit21/bin64/luac
make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-s CFLAGS="-O3 -m64 -DLUA_USE_LINUX" LDFLAGS="-s -m64" BUILDMODE=static clean

rm *.o
rm host/*.o

#------32bit compile--------------

make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-d CCDEBUG=-g CFLAGS="-m32 -DLUA_USE_LINUX" LDFLAGS="-m32"
mv -f libluajit501-gcc14-d.so out_lua_jit21/lib/libluajit501-gcc14-d.so
mv -f libluajit501-gcc14-d.a out_lua_jit21/lib/libluajit501-gcc14-d.a
make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-d CCDEBUG=-g CFLAGS="-m32 -DLUA_USE_LINUX" LDFLAGS="-m32" clean

rm *.o
rm host/*.o

make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-s-d CCDEBUG=-g BUILDMODE=static CFLAGS="-m32 -DLUA_USE_LINUX" LDFLAGS="-m32"
mv -f libluajit501-gcc14-s-d.a out_lua_jit21/lib/libluajit501-gcc14-s-d.a
make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-s-d CCDEBUG=-g BUILDMODE=static CFLAGS="-m32 -DLUA_USE_LINUX" LDFLAGS="-m32" clean

rm *.o
rm host/*.o

make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14 CFLAGS="-O3 -m32 -DLUA_USE_LINUX" LDFLAGS="-s -m32"
mv -f libluajit501-gcc14.so out_lua_jit21/lib/libluajit501-gcc14.so
mv -f libluajit501-gcc14.a out_lua_jit21/lib/libluajit501-gcc14.a
cp -f luajit out_lua_jit21/bin/lua
mv -f luajit out_lua_jit21/bin/luajit
make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14 CFLAGS="-O3 -m32 -DLUA_USE_LINUX" LDFLAGS="-s -m32" clean

rm *.o
rm host/*.o

make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-s CFLAGS="-O3 -m32 -DLUA_USE_LINUX" LDFLAGS="-s -m32" BUILDMODE=static
mv -f libluajit501-gcc14-s.a out_lua_jit21/lib/libluajit501-gcc14-s.a
mv -f luajit out_lua_jit21/bin/luac
make -j12 -f Makefile_yggr NODOTABIVER=jit501-gcc14-s CFLAGS="-O3 -m32 -DLUA_USE_LINUX" LDFLAGS="-s -m32" BUILDMODE=static clean

rm *.o
rm host/*.o

cp -f *.h out_lua_jit21/include/
cp -f *.hpp out_lua_jit21/include/
