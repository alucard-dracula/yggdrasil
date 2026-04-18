@echo off
: rem cd /d %~dp0

set src_dir=%1
set find_dir=%2

: rem echo "%src_dir%"
: rem echo "%find_dir%"

for /r . %%d in (%src_dir%) do (
    if "%%~nxd"=="%find_dir%" (
        echo "%%d"
        rd /s /q "%%d"
    )
)