1. mongodb_c_dirver 1.26.1 does not fully support arm, because mongodb_c_dirver requires IntelRDFPMathLib to support DECIMAL128
2. luajit21 not support ios
3. Python does not support ios, because apple's SDK blocks third-party libraries such as libffi, and apple's clang restricts the addition of self-compiled third-party libraries through isysroot and -mxxx-version-min parameters
4. 32-bit compilation is not supported, because Apple no longer supports 32-bit applications from development tools and SDKs
5. Some functions that are not supported in the current version of yggdrasil have been blocked on macos_arm64 and IOS, and will be considered for support in later versions
6. Considering that IOS devices use ARM chips, these functions are also shielded on the IOS X86 emulator

list of unsupported features:
macos arm64: 
1. serialization.bson
2. nsql_database_system
3. log.mongodb
4. luajit

ios：
1. serialization.bson
2. nsql_database_system
3. log.mongodb
4. python
5. luajit