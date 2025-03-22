why using four sh build boost, because

1, if "CreateProcessA() Windows API failed: 193 - %1 Not a valid Win32 application"  occurred,
     need call the failed command of manual
2, x86_64 and x86 need switch(on-off) <compileflags>-fno-integrated-as build boost-context
3, don't add failed command to sh file must run to msys consol
4, x86 x86_64 first build context, second build others