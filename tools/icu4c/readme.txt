unzip  icu-58-2 source
copy icu-58-2-patch/* to icu-58-2 source dir

windows:

1. open win console
2. cd you_icu_source_path/icu4c
3. icu4c_build_vc100_versioned.bat
4. icu4c_build_vc143_versioned.bat

5. open mingw64 console 
6. cd you_icu_source_path/icu4c/source/icu_mingw_build
7. icu4c_build_mingw_x64.sh

8. open mingw32 console 
9. cd you_icu_source_path/icu4c/source/icu_mingw_build
10. icu4c_build_mingw_x32.sh

11. switch to win console
12. cd you_icu_source_path/icu4c
13. icu4c_make_mix_packet.bat

linux:
1. open linux console
2. cd you_icu_source_path/icu4c/source/icu_linux_build
3. icu4c_build_linux_x64.sh
4. icu4c_build_linux_x32.sh

android:
1. open mingw console
2. cd you_icu_source_path/icu4c/source/icu_android_build
3. build-libicu-android.sh

darwin:
1. open macos console
2. cd you_icu_source_path/icu4c/source/icu_darwin_build
3. build-libicu-darwin16.sh
