1. mongodb_c_dirver 1.26.1 不完全支持arm 因为 mongodb_c_dirver需要IntelRDFPMathLib来支持DECIMAL128
2. luajit21 不支持 ios
3. python 不支持 ios, 因为apple的SDK屏蔽了libffi等第三方库,并且apple的clang通过isysroot和-mxxx-version-min参数限制了自己编译的第三方库的导入
4. 不支持32位的编译， 因为apple公司从开发工具到sdk都已经不支持32位应用了
5. 在当前版本的yggdrasil中不支持的部分功能在macos_arm64和IOS上做了屏蔽处理，在后期的版本中考虑支持
6. 考虑到IOS设备使用ARM芯片，所以这些功能在IOS的X86的模拟器上也做了屏蔽处理

不支持的功能列表:
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