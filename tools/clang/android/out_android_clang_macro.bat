
cd /d %~dp0

set org_path=%path%
set path=%android_ndk_clang_bin_path%;%org_path%

clang --target=aarch64-linux-android21 -E -dM - < nul > android_aarch64-linux-android21_macro.txt
clang --target=armv7a-linux-androideabi21 -E -dM - < nul > android_armv7a-linux-androideabi21_macro.txt
clang --target=i686-linux-android21 -mstackrealign -E -dM - < nul > android_i686-linux-android21_macro.txt
clang --target=riscv64-linux-android35 -E -dM - < nul > android_riscv64-linux-android35_macro.txt
clang --target=x86_64-linux-android21 -E -dM - < nul > android_x86_64-linux-android21_macro.txt

