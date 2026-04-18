#!/bin/bash

sed -i 's@^\(deb.*stable main\)$@#\1\ndeb https://mirrors.tuna.tsinghua.edu.cn/termux/apt/termux-main stable main@' $PREFIX/etc/apt/sources.list
sed -i 's@^\(deb.*x11 main\)$@#\1\ndeb https://mirrors.tuna.tsinghua.edu.cn/termux/apt/termux-x11 x11 main @' $PREFIX/etc/apt/sources.list.d/x11.list 
sed -i 's@^\(deb.*root main\)$@#\1\ndeb https://mirrors.tuna.tsinghua.edu.cn/termux/apt/termux-root root main @' $PREFIX/etc/apt/sources.list.d/root.list 