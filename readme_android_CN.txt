1. mongodb_c_dirver 1.26.1 不完全支持arm 因为 mongodb_c_dirver需要IntelRDFPMathLib来支持DECIMAL128
2. termux 使用 odbc 需要编译 数据库服务器和对应的odbc插件
3. termux 不能支持segment
4. yggdrasil.file_system 的一些功能在termux中需要ROOT
5. luajit21 不支持 riscv64
6. 在当前版本的yggdrasil中不支持的部分功能在android上做了屏蔽处理，在后期的版本中考虑支持
7. 考虑到android手机大部分使用ARM芯片，所以这些功能在X86芯片上也做了屏蔽处理

不支持的功能列表:
android:
1. serialization.bson
2. nsql_database_system
3. log.mongodb

android.riscv:
1. serialization.bson
2. nsql_database_system
3. log.mongodb
5. luajit