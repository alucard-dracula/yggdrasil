@echo off
cd /d %~dp0

call ndk-build-debug.bat
call ndk-build-release.bat