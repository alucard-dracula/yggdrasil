1. mongodb_c_driver 1.26.1 はARMを完全にはサポートしていません。なぜなら、mongodb_c_driverはDECIMAL128をサポートするためにIntelRDFPMathLibを必要とするからです。
2. TermuxでODBCを使用するには、データベースサーバーとODBCプラグインをコンパイルする必要があります。
3. Termuxはsegmentをサポートしていません。
4. Termuxのファイルシステムの一部の機能にはroot権限が必要です。
5. LuaJIT21はriscv64をサポートしていません。
6. 現在のバージョンのYggdrasilではサポートされていない機能の一部がAndroid上で遮断されており、今後のバージョンでのサポートを検討しています。
7. Androidスマートフォンの大部分がARMチップを使用していることを考慮し、これらの機能はX86チップでも遮蔽処理が行われています。

サポートされていない機能のリスト:
android:
1. serialization.bson
2. nsql_database_system
3. log.mongodb

android.riscv:
1. serialization.bson
2. nsql_database_system
3. log.mongodb
5. luajit