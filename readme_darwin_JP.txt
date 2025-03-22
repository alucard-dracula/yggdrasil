1. mongodb_c_driver 1.26.1 はARMを完全にはサポートしていません。なぜなら、mongodb_c_driverはDECIMAL128をサポートするためにIntelRDFPMathLibを必要とするからです。
2. LuaJIT21はiosをサポートしていません。
3. pythonはiOSをサポートしていません。なぜなら、AppleのSDKがlibffiなどのサードパーティライブラリを遮断しており、さらにAppleのclangがisysrootと-mxxx-version-minパラメータで独自にコンパイルしたサードパーティライブラリの追加を制限しているからです。 
4. 32ビットのコンパイルはサポートされていません。なぜなら、Apple社は開発ツールからSDKまで32ビットアプリケーションのサポートを終了しているからです。 
5. 現在のバージョンのyggdrasilでサポートされていない機能は、macos_arm64とiOS上で遮断処理が行われており、今後のバージョンでのサポートを検討しています。 
6. iOSデバイスはARMチップを使用するため、これらの機能はiOSのX86エミュレーター上でも遮断処理が行われています。

サポートされていない機能のリスト:
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