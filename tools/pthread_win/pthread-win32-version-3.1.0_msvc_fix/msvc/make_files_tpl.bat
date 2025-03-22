
:rem make_file ms_version (e.g make_file 100)
@echo off
cd /d %~dp0

echo %~dp0

set x86_bin_dir=bin/
set x86_lib_dir=lib/

set x64_bin_dir=bin64/
set x64_lib_dir=lib64/

set cc_tag=%1

:rem src file
:rem dll
set var_f_pthreadVC3d=pthreadVC3%cc_tag%-d.dll
set var_f_pthreadVC3=pthreadVC3%cc_tag%.dll

set var_f_pthreadVCE3d=pthreadVCE3%cc_tag%-d.dll
set var_f_pthreadVCE3=pthreadVCE3%cc_tag%.dll

set var_f_pthreadVSE3d=pthreadVSE3%cc_tag%-d.dll
set var_f_pthreadVSE3=pthreadVSE3%cc_tag%.dll

:rem dll.lib
set var_f_dlib_pthreadVC3d=libpthreadVC3%cc_tag%-d.lib
set var_f_dlib_pthreadVC3=libpthreadVC3%cc_tag%.lib

set var_f_dlib_pthreadVCE3d=libpthreadVCE3%cc_tag%-d.lib
set var_f_dlib_pthreadVCE3=libpthreadVCE3%cc_tag%.lib

set var_f_dlib_pthreadVSE3d=libpthreadVSE3%cc_tag%-d.lib
set var_f_dlib_pthreadVSE3=libpthreadVSE3%cc_tag%.lib

:rem lib
set var_f_lib_pthreadVC3d=libpthreadVC3%cc_tag%-s-d.lib
set var_f_lib_pthreadVC3=libpthreadVC3%cc_tag%-s.lib

set var_f_lib_pthreadVCE3d=libpthreadVCE3%cc_tag%-s-d.lib
set var_f_lib_pthreadVCE3=libpthreadVCE3%cc_tag%-s.lib

set var_f_lib_pthreadVSE3d=libpthreadVSE3%cc_tag%-s-d.lib
set var_f_lib_pthreadVSE3=libpthreadVSE3%cc_tag%-s.lib


:rem dst file
:rem dll
set var_tf_pthreadVC3d=pthreadVC3d.dll
set var_tf_pthreadVC3=pthreadVC3.dll

set var_tf_pthreadVCE3d=pthreadVCE3d.dll
set var_tf_pthreadVCE3=pthreadVCE3.dll

set var_tf_pthreadVSE3d=pthreadVSE3d.dll
set var_tf_pthreadVSE3=pthreadVSE3.dll

:rem dll.lib
set var_tf_dlib_pthreadVC3d=pthreadVC3d.lib
set var_tf_dlib_pthreadVC3=pthreadVC3.lib

set var_tf_dlib_pthreadVCE3d=pthreadVCE3d.lib
set var_tf_dlib_pthreadVCE3=pthreadVCE3.lib

set var_tf_dlib_pthreadVSE3d=pthreadVSE3d.lib
set var_tf_dlib_pthreadVSE3=pthreadVSE3.lib

:rem lib
set var_tf_lib_pthreadVC3d=libpthreadVC3d.lib
set var_tf_lib_pthreadVC3=libpthreadVC3.lib

set var_tf_lib_pthreadVCE3d=libpthreadVCE3d.lib
set var_tf_lib_pthreadVCE3=libpthreadVCE3.lib

set var_tf_lib_pthreadVSE3d=libpthreadVSE3d.lib
set var_tf_lib_pthreadVSE3=libpthreadVSE3.lib

:rem operator
:rem x86
:rem dll
if exist %x86_bin_dir%%var_f_pthreadVC3d% cp -f %x86_bin_dir%%var_f_pthreadVC3d% %x86_bin_dir%%var_tf_pthreadVC3d%
if exist %x86_bin_dir%%var_f_pthreadVC3% cp -f %x86_bin_dir%%var_f_pthreadVC3% %x86_bin_dir%%var_tf_pthreadVC3%

if exist %x86_bin_dir%%var_f_pthreadVCE3d% cp -f %x86_bin_dir%%var_f_pthreadVCE3d% %x86_bin_dir%%var_tf_pthreadVCE3d%
if exist %x86_bin_dir%%var_f_pthreadVCE3% cp -f %x86_bin_dir%%var_f_pthreadVCE3% %x86_bin_dir%%var_tf_pthreadVCE3%

if exist %x86_bin_dir%%var_f_pthreadVSE3d% cp -f %x86_bin_dir%%var_f_pthreadVSE3d% %x86_bin_dir%%var_tf_pthreadVSE3d%
if exist %x86_bin_dir%%var_f_pthreadVSE3% cp -f %x86_bin_dir%%var_f_pthreadVSE3% %x86_bin_dir%%var_tf_pthreadVSE3%

:rem dll.lib
if exist %x86_lib_dir%%var_f_dlib_pthreadVC3d% cp -f %x86_lib_dir%%var_f_dlib_pthreadVC3d% %x86_lib_dir%%var_tf_dlib_pthreadVC3d%
if exist %x86_lib_dir%%var_f_dlib_pthreadVC3% cp -f %x86_lib_dir%%var_f_dlib_pthreadVC3% %x86_lib_dir%%var_tf_dlib_pthreadVC3%

if exist %x86_lib_dir%%var_f_dlib_pthreadVCE3d% cp -f %x86_lib_dir%%var_f_dlib_pthreadVCE3d% %x86_lib_dir%%var_tf_dlib_pthreadVCE3d%
if exist %x86_lib_dir%%var_f_dlib_pthreadVCE3% cp -f %x86_lib_dir%%var_f_dlib_pthreadVCE3% %x86_lib_dir%%var_tf_dlib_pthreadVCE3%

if exist %x86_lib_dir%%var_f_dlib_pthreadVSE3d% cp -f %x86_lib_dir%%var_f_dlib_pthreadVSE3d% %x86_lib_dir%%var_tf_dlib_pthreadVSE3d%
if exist %x86_lib_dir%%var_f_dlib_pthreadVSE3% cp -f %x86_lib_dir%%var_f_dlib_pthreadVSE3% %x86_lib_dir%%var_tf_dlib_pthreadVSE3%

:rem lib
if exist %x86_lib_dir%%var_f_lib_pthreadVC3d% cp -f %x86_lib_dir%%var_f_lib_pthreadVC3d% %x86_lib_dir%%var_tf_lib_pthreadVC3d%
if exist %x86_lib_dir%%var_f_lib_pthreadVC3% cp -f %x86_lib_dir%%var_f_lib_pthreadVC3% %x86_lib_dir%%var_tf_lib_pthreadVC3%

if exist %x86_lib_dir%%var_f_lib_pthreadVCE3d% cp -f %x86_lib_dir%%var_f_lib_pthreadVCE3d% %x86_lib_dir%%var_tf_lib_pthreadVCE3d%
if exist %x86_lib_dir%%var_f_lib_pthreadVCE3% cp -f %x86_lib_dir%%var_f_lib_pthreadVCE3% %x86_lib_dir%%var_tf_lib_pthreadVCE3%

if exist %x86_lib_dir%%var_f_lib_pthreadVSE3d% cp -f %x86_lib_dir%%var_f_lib_pthreadVSE3d% %x86_lib_dir%%var_tf_lib_pthreadVSE3d%
if exist %x86_lib_dir%%var_f_lib_pthreadVSE3% cp -f %x86_lib_dir%%var_f_lib_pthreadVSE3% %x86_lib_dir%%var_tf_lib_pthreadVSE3%

:rem x64
:rem dll
if exist %x64_bin_dir%%var_f_pthreadVC3d% cp -f %x64_bin_dir%%var_f_pthreadVC3d% %x64_bin_dir%%var_tf_pthreadVC3d%
if exist %x64_bin_dir%%var_f_pthreadVC3% cp -f %x64_bin_dir%%var_f_pthreadVC3% %x64_bin_dir%%var_tf_pthreadVC3%

if exist %x64_bin_dir%%var_f_pthreadVCE3d% cp -f %x64_bin_dir%%var_f_pthreadVCE3d% %x64_bin_dir%%var_tf_pthreadVCE3d%
if exist %x64_bin_dir%%var_f_pthreadVCE3% cp -f %x64_bin_dir%%var_f_pthreadVCE3% %x64_bin_dir%%var_tf_pthreadVCE3%

if exist %x64_bin_dir%%var_f_pthreadVSE3d% cp -f %x64_bin_dir%%var_f_pthreadVSE3d% %x64_bin_dir%%var_tf_pthreadVSE3d%
if exist %x64_bin_dir%%var_f_pthreadVSE3% cp -f %x64_bin_dir%%var_f_pthreadVSE3% %x64_bin_dir%%var_tf_pthreadVSE3%

:rem dll.lib
if exist %x64_lib_dir%%var_f_dlib_pthreadVC3d% cp -f %x64_lib_dir%%var_f_dlib_pthreadVC3d% %x64_lib_dir%%var_tf_dlib_pthreadVC3d%
if exist %x64_lib_dir%%var_f_dlib_pthreadVC3% cp -f %x64_lib_dir%%var_f_dlib_pthreadVC3% %x64_lib_dir%%var_tf_dlib_pthreadVC3%

if exist %x64_lib_dir%%var_f_dlib_pthreadVCE3d% cp -f %x64_lib_dir%%var_f_dlib_pthreadVCE3d% %x64_lib_dir%%var_tf_dlib_pthreadVCE3d%
if exist %x64_lib_dir%%var_f_dlib_pthreadVCE3% cp -f %x64_lib_dir%%var_f_dlib_pthreadVCE3% %x64_lib_dir%%var_tf_dlib_pthreadVCE3%

if exist %x64_lib_dir%%var_f_dlib_pthreadVSE3d% cp -f %x64_lib_dir%%var_f_dlib_pthreadVSE3d% %x64_lib_dir%%var_tf_dlib_pthreadVSE3d%
if exist %x64_lib_dir%%var_f_dlib_pthreadVSE3% cp -f %x64_lib_dir%%var_f_dlib_pthreadVSE3% %x64_lib_dir%%var_tf_dlib_pthreadVSE3%

:rem lib
if exist %x64_lib_dir%%var_f_lib_pthreadVC3d% cp -f %x64_lib_dir%%var_f_lib_pthreadVC3d% %x64_lib_dir%%var_tf_lib_pthreadVC3d%
if exist %x64_lib_dir%%var_f_lib_pthreadVC3% cp -f %x64_lib_dir%%var_f_lib_pthreadVC3% %x64_lib_dir%%var_tf_lib_pthreadVC3%

if exist %x64_lib_dir%%var_f_lib_pthreadVCE3d% cp -f %x64_lib_dir%%var_f_lib_pthreadVCE3d% %x64_lib_dir%%var_tf_lib_pthreadVCE3d%
if exist %x64_lib_dir%%var_f_lib_pthreadVCE3% cp -f %x64_lib_dir%%var_f_lib_pthreadVCE3% %x64_lib_dir%%var_tf_lib_pthreadVCE3%

if exist %x64_lib_dir%%var_f_lib_pthreadVSE3d% cp -f %x64_lib_dir%%var_f_lib_pthreadVSE3d% %x64_lib_dir%%var_tf_lib_pthreadVSE3d%
if exist %x64_lib_dir%%var_f_lib_pthreadVSE3% cp -f %x64_lib_dir%%var_f_lib_pthreadVSE3% %x64_lib_dir%%var_tf_lib_pthreadVSE3%

:rem inclue

set var_f_head_pthread=pthread.h
set var_f_head_sched=sched.h
set var_f_head_semaphore=semaphore.h
set var_f_head__ptw32=_ptw32.h

if not exist include md include
if exist ../%var_f_head_pthread% cp -f ../%var_f_head_pthread% ./include/%var_f_head_pthread%
if exist ../%var_f_head_sched% cp -f ../%var_f_head_sched% ./include/%var_f_head_sched%
if exist ../%var_f_head_semaphore% cp -f ../%var_f_head_semaphore% ./include/%var_f_head_semaphore%
if exist ../%var_f_head__ptw32% cp -f ../%var_f_head__ptw32% ./include/%var_f_head__ptw32%
