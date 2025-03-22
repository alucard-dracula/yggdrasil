1. mongodb_c_dirver 1.26.1 does not fully support arm, because mongodb_c_dirver requires IntelRDFPMathLib to support DECIMAL128
2. termux using odbc need compile database server and odbc plugin
3. termux not support segment
4. file_system in termux some feture need root
5. luajit21 not support riscv64
6. Some functions that are not supported in the current version of YGGDRASIL have been shielded on Android, and will be considered for support in later versions
7. Considering that most android phones use ARM chips, these functions are also shielded on the X86 chip

list of unsupported features:
android:
1. serialization.bson
2. nsql_database_system
3. log.mongodb

android.riscv:
1. serialization.bson
2. nsql_database_system
3. log.mongodb
5. luajit