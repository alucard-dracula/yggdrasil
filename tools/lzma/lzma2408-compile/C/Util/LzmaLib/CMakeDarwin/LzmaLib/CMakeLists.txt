#LzmaLib CMakeLists.txt

cmake_minimum_required(VERSION 3.12)

if(USRDEF_CMAKE_C_COMPILER)
	set(CMAKE_C_COMPILER ${USRDEF_CMAKE_C_COMPILER})
endif()

if(USRDEF_CMAKE_CXX_COMPILER)
	set(CMAKE_CXX_COMPILER ${USRDEF_CMAKE_CXX_COMPILER})
endif()

#set(USRDEF_CMAKE_COMPILER_VERSION "clang-darwin16")
#set(USRDEF_CMAKE_BUILD_LD_ARCH_TAG "a")
#set(USRDEF_CMAKE_BUILD_ARCH_BITS "64")
set(USRDEF_CMAKE_BOOST_VERSION "1_82")
set(USRDEF_CMAKE_PYTHON_VERSION "313")

set(USRDEF_CMAKE_PYTHON_INCLUDE_DIR "python3.13")

#set(USRDEF_CMAKE_OSX_SDK_NAME "macosx")

if("${USRDEF_CMAKE_OSX_SDK_NAME}" STREQUAL "macosx")
	set(USRDEF_CMAKE_BOOST_LINK_TAG_DEBUG "-d")
	set(USRDEF_CMAKE_BOOST_LINK_TAG_RELEASE "")
else()
	set(USRDEF_CMAKE_BOOST_LINK_TAG_DEBUG "-sd")
	set(USRDEF_CMAKE_BOOST_LINK_TAG_RELEASE "-s")
endif()

set(USRDEF_CMAKE_COMPILER_VERSION_TAG "-${USRDEF_CMAKE_COMPILER_VERSION}")
set(USRDEF_CMAKE_BUILD_ARCH_BITS_TAG "-${USRDEF_CMAKE_BUILD_LD_ARCH_TAG}${USRDEF_CMAKE_BUILD_ARCH_BITS}")

set(USRDEF_CMAKE_BOOST_VERSION_TAG "-${USRDEF_CMAKE_BOOST_VERSION}")

project(LzmaLib)

message(STATUS "build-root-project [ LzmaLib ]")

set(CMAKE_C_STANDARD ${USRDEF_CMAKE_C_STANDARD})
set(CMAKE_CXX_STANDARD ${USRDEF_CMAKE_CXX_STANDARD})
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE ${USRDEF_CMAKE_BUILD_TYPE})

set(CMAKE_OSX_SYSROOT ${USRDEF_CMAKE_OSX_SYSROOT})
set(CMAKE_OSX_ARCHITECTURES ${USRDEF_CMAKE_OSX_ARCHITECTURES})

string(FIND "${USRDEF_CMAKE_OSX_ARCHITECTURES}" "arm64" var_arm64_chk)
if(var_arm64_chk EQUAL -1)
	set(USRDEF_HAS_ARM64 OFF)
else()
	set(USRDEF_HAS_ARM64 ON)
endif()

string(FIND "${USRDEF_CMAKE_OSX_ARCHITECTURES}" "x86_64" var_x86_64_chk)
if(var_x86_64_chk EQUAL -1)
	set(USRDEF_HAS_X86_64 OFF)
else()
	set(USRDEF_HAS_X86_64 ON)
endif()

if("${USRDEF_CMAKE_OSX_SDK_NAME}" STREQUAL "macosx")
	 set(var_third_part_dir "third_part")
else()
	 set(var_third_part_dir "third_part_${USRDEF_CMAKE_OSX_SDK_NAME}")
endif()

set(USRDEF_CMAKE_INCLUDE_DIRS 
		~/${var_third_part_dir}/boost/include
		~/${var_third_part_dir}/libbase64/include
		~/${var_third_part_dir}/libblowfish/include
		~/${var_third_part_dir}/libmd5/include
		~/${var_third_part_dir}/libsha/include
		~/${var_third_part_dir}/libicu/include
		~/${var_third_part_dir}/libzlib/include
		~/${var_third_part_dir}/libopenssl/include
		~/${var_third_part_dir}/LzmaLib/include
		~/${var_third_part_dir}/unixODBC/include
		~/${var_third_part_dir}/libdtl/include
		~/${var_third_part_dir}/mongodb/include
		~/${var_third_part_dir}/mongodb_c_driver/include
		~/${var_third_part_dir}/mongodb_c_driver/include/bson
		~/${var_third_part_dir}/mongodb_c_driver/include/common
		~/${var_third_part_dir}/mongodb_c_driver/include/kms_message
		~/${var_third_part_dir}/mongodb_c_driver/include/mongo_crypt
		~/${var_third_part_dir}/mongodb_c_driver/include/mongoc
		~/${var_third_part_dir}/mongodb_c_driver/include/mongocrypt
		~/${var_third_part_dir}/mongodb_c_driver/include/mongocrypt/unicode
		~/${var_third_part_dir}/mongodb_c_driver/include/utf8proc
		~/${var_third_part_dir}/lua/include
		~/${var_third_part_dir}/luabind/include
		~/${var_third_part_dir}/python/include/${USRDEF_CMAKE_PYTHON_INCLUDE_DIR}
)

if(${USRDEF_HAS_X86_64})
	list(APPEND USRDEF_CMAKE_INCLUDE_DIRS 
			~/${var_third_part_dir}/IntelRDFPMathLib/include
			~/${var_third_part_dir}/IntelRDFPMathLib/include/src
	)
endif()

if((NOT(${CMAKE_SYSTEM_PROCESSOR} STREQUAL ${USRDEF_CMAKE_OSX_ARCHITECTURES}))
	OR (NOT(${USRDEF_CMAKE_OSX_SDK_NAME} STREQUAL "macosx")))
	list(APPEND USRDEF_CMAKE_INCLUDE_DIRS 
			~/${var_third_part_dir}/cyrus-sasl/include
	)
endif()

if(${USRDEF_HAS_ARM64})
	set(USRDEF_CMAKE_DEFINEDS "-DYGGR_TEST_DISABLE_NON_GENERIC_SUPPORT")
else()
	set(USRDEF_CMAKE_DEFINEDS "")
endif()
set(USRDEF_CMAKE_DEFINEDS_DEBUG "")
set(USRDEF_CMAKE_DEFINEDS_RELEASE "")

#set(USRDEF_CMAKE_CFLAGS "")
set(USRDEF_CMAKE_CFLAGS_DEBUG "")
set(USRDEF_CMAKE_CFLAGS_RELEASE "")

set(USRDEF_CMAKE_CXXFLAGS "")
set(USRDEF_CMAKE_CXXFLAGS_DEBUG "")
set(USRDEF_CMAKE_CXXFLAGS_RELEASE "")

set(USRDEF_CMAKE_LD_LIBRARY_PATH 
		~/${var_third_part_dir}/boost/lib
		~/${var_third_part_dir}/libbase64/lib
		~/${var_third_part_dir}/libblowfish/lib
		~/${var_third_part_dir}/libmd5/lib
		~/${var_third_part_dir}/libsha/lib
		~/${var_third_part_dir}/libicu/lib
		~/${var_third_part_dir}/libzlib/lib
		~/${var_third_part_dir}/libopenssl/lib
		~/${var_third_part_dir}/LzmaLib/lib
		~/${var_third_part_dir}/unixODBC/lib
		~/${var_third_part_dir}/libdtl/lib
		~/${var_third_part_dir}/mongodb/lib
		~/${var_third_part_dir}/mongodb_c_driver/lib
		~/${var_third_part_dir}/lua/lib
		~/${var_third_part_dir}/luabind/lib
		~/${var_third_part_dir}/python/lib
)

if(${USRDEF_HAS_X86_64})
	list(APPEND USRDEF_CMAKE_LD_LIBRARY_PATH 
			~/${var_third_part_dir}/IntelRDFPMathLib/lib
	)
endif()

if((NOT(${CMAKE_SYSTEM_PROCESSOR} STREQUAL ${USRDEF_CMAKE_OSX_ARCHITECTURES}))
	OR (NOT(${USRDEF_CMAKE_OSX_SDK_NAME} STREQUAL "macosx")))
	list(APPEND USRDEF_CMAKE_LD_LIBRARY_PATH 
			~/${var_third_part_dir}/cyrus-sasl/lib
	)
endif()

set(USRDEF_CMAKE_LD_LIBRARY "")
set(USRDEF_CMAKE_LD_LIBRARY_DEBUG "")
set(USRDEF_CMAKE_LD_LIBRARY_RELEASE "")

set(USRDEF_CMAKE_LDFLAGS "")
set(USRDEF_CMAKE_LDFLAGS_DEBUG "")
set(USRDEF_CMAKE_LDFLAGS_RELEASE "")

set(USRDEF_CMAKE_PROJ_ROOT_DIR ${PROJECT_SOURCE_DIR})

#add_subdirectory dynamic libraries
add_subdirectory(LzmaLib)


