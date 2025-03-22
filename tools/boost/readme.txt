Install Microsoft HPC Pack 2008 SDK // 64-bit MPI (in D:/Program Files/Microsoft HPC Pack 2008 SDK)
Install Microsoft Compute Cluster Pack // 32-bit MPI under the direct zip package decompression configuration (in the configuration D:/Program Files (x86)/Microsoft Compute Cluster Pack)
Install PYTHON  and copy the yggdrasil/tools/python file to the corresponding directory in python // Please distinguish between versions
Install and configure the ICU
Unzip zlib
Unzip boost

If you want to customize the configuration path above, you need to modify the corresponding configuration file.

Copy the compilation script(e.g build64-msvc-143.bat) of the corresponding platform to the boost root directory and modify the path such as ICU ZLIB
Launch the console and go to the boost directory
Run bootstrap.bat
Run the compilation script

Wait for the compilation to complete and check if there is a problem after there are no problems
Search for files in the stage directory in the boost directory and copy them to the corresponding bin and lib directories
Note: that 32-bit and 64-bit are compiled separately
The file is copied in the win32 win64 subdirectory of the boost_compile_patch/boost_x_xx_x_patch

Work on your own script notes:
bjam toolset=msvc -sICU_PATH="The ICU directory you configured (mine is D:/icu_vc100)" --build-type=complete //32 bits
bjam toolset=msvc address-model=64 -sICU_PATH="The ICU directory you configured (mine is D:/icu_vc100)" --build-type=complete //64bit


