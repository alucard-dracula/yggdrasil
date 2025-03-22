安装 Microsoft HPC Pack 2008 SDK  // 64位MPI (配置里在D:\Program Files\Microsoft HPC Pack 2008 SDK)
安装 Microsoft Compute Cluster Pack // 32位MPI 这个直接压缩包解压配置下（配置里在D:\Program Files (x86)\Microsoft Compute Cluster Pack）
安装 PYTHON 2.7 并且将yggdrasil/tools/python下文件 复制到python的对应目录
安装 并配置 ICU

解压boost
启动控制台 进入 boost 目录
bootstrap.bat
bjam toolset=msvc -sICU_PATH="你配置的ICU目录（我的是D:/icu_vc100)" --build-type=complete  //32位
bjam toolset=msvc address-model=64 -sICU_PATH="你配置的ICU目录（我的是D:/icu_vc100)" --build-type=complete  //64位

等编译完成 检查是否有问题 没有问题之后
在 boost目录下的的stage里 搜索文件复制到相应的bin和lib目录

注意32位和64位分开编译

文件的复制方法在win32 win64子目录下