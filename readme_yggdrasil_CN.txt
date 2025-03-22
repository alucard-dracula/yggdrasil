
1. yggdrasil是我在多年游戏开发工作中整理出来的通用功能
2. yggdrasil包含了游戏服务端所需要的基本功能，包括 网络 文件 数据库 以及常用的 线程安全的数据结构等

3. 功能模块主要包括:
    3.1 server:                  用于实现游戏服务端的架构
    3.2 client:                  用于实现游戏客户端的架构
    3.3 proxy:                   用于实现分布式游戏服务端架构
    3.4 file_system:             用于实现服务端文件的处理
    3.5 database_system:         基于ODBC开发 一般用于制作EXCEL的配置表格转换，以及不同数据库之间数据转换的开发和维护工具， 不推荐用做游戏服务器的数据库操作接口
    3.6 nsql_database_system:    基于mongodb_c_driver开发 作为游戏服务器数据库操作的接口
    3.7 charset:                 用于规范化游戏文本字符集的工具
    3.8 safe_container:          用于实现游戏逻辑的线程安全的基本容器

4. 第三方库的说明：
    第三方库的编译配置、脚本、补丁均放在yggdrasil/tools下 分为2类， 详细说明请参看tools下各个子目录
    4.1 有补丁和编译脚本的项目:
        这类项目 需要将补丁文件和编译脚本（.bat .sh等） 复制到源码的对应目录 然后使用脚本编译， 编译后安装到开发环境中
    4.2 使用IDE配置的项目
        直接使用IDE打开项目编译 编译后安装到开发环境中

5. 兼容性说明：
    5.1. 语言版本要求：
        最低要求的C++语言版本是: C++03
        最低要求的C语言版本是: C98
        最高支持的C++语言版本是: C++20
        注意：不同系统对于最低语言版本的要求包括 编译器方面 和 第三方库方面， 具体要求根据环境来决定（比如BOOST1.84后就无法支持C++03了）。

    5.2. 库版本要求：
        5.2.1. boost:
            该库的版本当前最高支持到1.82。
            由于yggdrasil.safe_container 使用了 source hacking 的技术来兼容和优化boost容器在多线程环境下的操作，
            所以需要针对不同版本的BOOST来做优化， 如果要使用优化的版本 必须保证yggr/safe_container/detail/boost_container_opt有对应版本的优化代码，
            否则只能使用非优化版本

        5.2.2. mongodb_c_dirver
            该库的版本当前最高支持到1.26.1。
            由于mongodb_c_dirver历年来改动相当频繁，切各版本之间兼容性非常差， 同时为了兼容C++03，以及保证yggdrasil的兼容性，部分代码还需要用C98的语法重新实现，
            所以该库只支持在tools中的魔改版（当前为tools/mongodb_c_driver/yggr_src_1.26.1）, 并且在未来对于该库 也只保证支持一个版本，直到mongodb_c_dirver趋于稳定

        5.2.3. x86
            对于x86平台yggdrasil的各个模块都能提供很好的支持

        5.2.4. arm
            对于ARM平台 主要的限制在系统SDK和第三方库的支持上。
            python
            luajit
            yggdrasil.serialization.bson
            yggdrasil.nsql_database_system
            yggdrasil.log.mongodb
            yggdrasil.file_system
            yggdrasil.segment
            yggdrasil.seh
            以上这些库在ARM平台不能完全支持，请根据readme_[system_plafrom].txt的说明来决定如何使用。

        5.2.5. 其他平台
            需要用户根据SDK和第三方库的支持程度自行判断该平台是否能支持。

6. 安装说明：
    详细的安装方法请参看 INSTALL_XX.txt
    6.1. WINDWOS：
        提供了.bat、.sh(mingw)、 .sln(msvc的IDE配置)、 .workspace(code blacks的IDE配置)
    6.2. LINUX：
        提供了.sh(mingw)、 .workspace(code blacks的IDE配置)
    6.3. ANDROID:
        提供了 .bat、.sh(mingw)、.mk（ndk-build）
    6.4. macos：
        提供了 .sh CMakelists.txt(cmake)  

7. 如何学习使用：
    yggdrasil 除了yggr_lib_only.sln中包含的项目外 其余都是例题和测试代码，
    由于文档工作量巨大，个人能力有限，所以请根据例题来学习如何使用。
    对于STL和BOOST的扩展功能，使用方式和STL以及BOOST类似，可参考STL和boost的文档


