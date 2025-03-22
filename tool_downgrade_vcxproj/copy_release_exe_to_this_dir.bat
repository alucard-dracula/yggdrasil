cd /d %~dp0
rm -f ./tool_downgrade_vcxproj.exe
cp -f ../Release-x64/vc100/tool_downgrade_vcxproj.exe ./
