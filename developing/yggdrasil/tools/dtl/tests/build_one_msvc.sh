#!/bin/sh
# usage: build_one_msvc.sh BUILDFLAG TARGETFILE.cpp
# example: sh build_one_msvc.sh debug hello.cpp
# this file should be located under the project directory

# How to add a menu item in Visual studio to rebuild the current .cpp file:
#    1.  From the Tools Menu choose "Customize ..."
#	    The Customize ... Dialog Box appears
#    2.  Select the "Tools" tab
#    3.  In the  "Menu Contents" box, cursor down to the bottom where
#	you   see an open rectangle.  Click on this line which will enable
#	you to enter in a label that will identify the tool.
#	Type "Rebuild current file (debug)" or equivalent and hit Enter.
#    4.  You will now be able to edit the fields of this Tool.
#	Modify them so that they approximate the following :
#	NOTE HERE THAT YOU WILL WANT TO USE YOUR OWN BUILD FLAG HERE INSTEAD OF debug
#
#	Command:            sh
#	Arguments           build_one_msvc.sh debug $(FileName)$(FileExt)
#	Initial Directory   $(FileDir)
#
#	X_Use Output Window __Prompt for arguments __Close window on exit
#
#    5.  Select Close to return to the main environment.


TARGET=`echo $1/$2 | sed 's/\.cpp/.obj/'`

rm -f $TARGET
echo build.sh -p cygwin-i686-msvc -b $1 $TARGET | sh