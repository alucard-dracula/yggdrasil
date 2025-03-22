
1. YGGDRASIL is a generic feature that I've put together over the years I've worked on game development
2. YGGDRASIL contains the basic functions required by the game server, including network, file, database, and common thread-safe data structures

3. The functional modules mainly include:
    3.1 server:                 The architecture used to implement the game server
    3.2 client:                 The architecture used to implement the game client
    3.3 proxy:                  Used to implement the distributed game server architecture
    3.4 file_system:            Used to implement the processing of server-side files
    3.5 database_system:        ODBC-based development is generally used to make EXCEL configuration table conversion, 
                                as well as the development and maintenance tools of data conversion between different databases, 
                                and is not recommended to be used as a database operation interface for game servers
    3.6 nsql_database_system:   mongodb_c_driver-based development as an interface for game server database operations
    3.7 charset:                A tool used to normalize the character set of game text
    3.8 safe_container:         A basic thread-safe container for implementing game logic

4. Description of third-party libraries:
    The compilation configuration, scripts, and patches of third-party libraries are all placed under yggdrasil/tools and are divided into two categories,
    please refer to each subdirectory under tools for details
    4.1 Projects with patches and compiled scripts:
        For this type of project, you need to copy the patch file and compilation script (.bat .sh, etc.) to the corresponding directory of the source code, 
        then compile it with the script, compile it, and install it in the development environment
    4.2 Projects configured with the IDE
        Directly use the IDE to open the project and compile it and install it in the development environment

5. Compatibility Notes:
    5.1. Language Version Requirements:
        The minimum required C++ language version is: C++03
        The minimum required C language version is: C98
        The highest supported C++ language version is: C++20
    NOTE: THE MINIMUM LANGUAGE REQUIREMENTS FOR DIFFERENT SYSTEMS INCLUDE COMPILER AND THIRD-PARTY LIBRARIES,
          DEPENDING ON THE ENVIRONMENT (E.G. C++03 IS NOT SUPPORTED AFTER BOOST 1.84).

5.2. Library Version Requirements:
    5.2.1. boost:
        The library is currently supported up to 1.82.
        Since yggdrasil.safe_container uses source hacking techniques to be compatible and optimize the operation of boost containers in a multi-threaded environment,
        Therefore, you need to optimize for different versions of BOOST, if you want to use the optimized version, 
        you must ensure that yggr/safe_container/detail/boost_container_opt has the corresponding version of the optimization code.
        Otherwise, only the non-optimized version can be used

    5.2.2. mongodb_c_dirver
        The library is currently supported up to 1.26.1.
        Due to the frequent changes over the years in mongodb_c_dirver, the compatibility between the versions is very poor, and in order to be compatible with C++03 and ensure the compatibility of yggdrasil, some of the code needs to be re-implemented with C98 syntax.
        Therefore, the library only supports the magic version in tools (currently tools/mongodb_c_driver/yggr_src_1.26.1), and in the future it is only guaranteed to support one version of the library until the mongodb_c_dirver stabilizes

    5.2.3. x86
        For the x86 platform, YGGDRASIL provides good support for each module

    5.2.4. arm
        For the ARM platform, the main limitation is in the support of system SDKs and third-party libraries.
        
        python
        luajit
        yggdrasil.serialization.bson
        yggdrasil.nsql_database_system
        yggdrasil.log.mongodb
        yggdrasil.file_system
        yggdrasil.segment
        yggdrasil.seh
        These libraries are not fully supported on the ARM platform, please follow the instructions readme_[system_plafrom].txt to decide how to use them

    5.2.5. Other Platforms
        Users need to judge whether the platform can support the platform according to the support level of the SDK and third-party libraries

6. Installation Instructions:
    Please refer to INSTALL_XX.txt for details on how to install it
    6.1. WINDWOS：
        .bat, . sh (mingw), .sln (IDE configuration for msvc), .workspace (IDE configuration for code blacks) is provided.
    6.2. LINUX：
        .sh (mingw), .workspace (IDE configuration for code blacks) is provided.
    6.3. ANDROID:
        .bat, . sh(mingw)、.mk（ndk-build）is provided.
    6.4. macos：
        .sh CMakelists.txt (cmake) is provided

7. How to learn to use:
    yggdrasil except for the items included in the yggr_lib_only.sln are example problems and test code.
    Due to the huge amount of documentation and the limited ability of individuals, please follow the example questions to learn how to use it.
    For STL and BOOST extensions, you can use them in a similar way to STL and BOOST, see STL and BOOST documentation
