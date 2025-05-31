#mongoc CMakeLists.txt

project(mongoc)

message(STATUS "build-sub-project [ mongoc ]")

set(var_proj_name mongoc)

set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)
set(var_proj_src_dir ${var_root_proj_dir}/mongoc)

set(var_proj_src_list 
		${var_proj_src_dir}/mcd-azure.c
		${var_proj_src_dir}/mcd-rpc.c
		${var_proj_src_dir}/mongoc-aggregate.c
		${var_proj_src_dir}/mongoc-apm.c
		${var_proj_src_dir}/mongoc-array.c
		${var_proj_src_dir}/mongoc-async-cmd.c
		${var_proj_src_dir}/mongoc-async.c
		${var_proj_src_dir}/mongoc-buffer.c
		${var_proj_src_dir}/mongoc-bulk-operation.c
		${var_proj_src_dir}/mongoc-change-stream.c
		${var_proj_src_dir}/mongoc-client-pool.c
		${var_proj_src_dir}/mongoc-client-session.c
		${var_proj_src_dir}/mongoc-client-side-encryption.c
		${var_proj_src_dir}/mongoc-client.c
		${var_proj_src_dir}/mongoc-cluster-aws.c
		${var_proj_src_dir}/mongoc-cluster-cyrus.c
		${var_proj_src_dir}/mongoc-cluster-sasl.c
		${var_proj_src_dir}/mongoc-cluster-sspi.c
		${var_proj_src_dir}/mongoc-cluster.c
		${var_proj_src_dir}/mongoc-cmd.c
		${var_proj_src_dir}/mongoc-collection.c
		${var_proj_src_dir}/mongoc-compression.c
		${var_proj_src_dir}/mongoc-counters.c
		${var_proj_src_dir}/mongoc-crypt.c
		${var_proj_src_dir}/mongoc-crypto-cng.c
		${var_proj_src_dir}/mongoc-crypto-common-crypto.c
		${var_proj_src_dir}/mongoc-crypto-openssl.c
		${var_proj_src_dir}/mongoc-crypto.c
		${var_proj_src_dir}/mongoc-cursor-array.c
		${var_proj_src_dir}/mongoc-cursor-change-stream.c
		${var_proj_src_dir}/mongoc-cursor-cmd-deprecated.c
		${var_proj_src_dir}/mongoc-cursor-cmd.c
		${var_proj_src_dir}/mongoc-cursor-find-cmd.c
		${var_proj_src_dir}/mongoc-cursor-find-opquery.c
		${var_proj_src_dir}/mongoc-cursor-find.c
		${var_proj_src_dir}/mongoc-cursor-legacy.c
		${var_proj_src_dir}/mongoc-cursor.c
		${var_proj_src_dir}/mongoc-cyrus.c
		${var_proj_src_dir}/mongoc-database.c
		${var_proj_src_dir}/mongoc-deprioritized-servers.c
		${var_proj_src_dir}/mongoc-error.c
		${var_proj_src_dir}/mongoc-find-and-modify.c
		${var_proj_src_dir}/mongoc-flags.c
		${var_proj_src_dir}/mongoc-generation-map.c
		${var_proj_src_dir}/mongoc-gridfs-bucket-file.c
		${var_proj_src_dir}/mongoc-gridfs-bucket.c
		${var_proj_src_dir}/mongoc-gridfs-file-list.c
		${var_proj_src_dir}/mongoc-gridfs-file-page.c
		${var_proj_src_dir}/mongoc-gridfs-file.c
		${var_proj_src_dir}/mongoc-gridfs.c
		${var_proj_src_dir}/mongoc-handshake.c
		${var_proj_src_dir}/mongoc-host-list.c
		${var_proj_src_dir}/mongoc-http.c
		${var_proj_src_dir}/mongoc-index.c
		${var_proj_src_dir}/mongoc-init.c
		${var_proj_src_dir}/mongoc-interrupt.c
		${var_proj_src_dir}/mongoc-libressl.c
		${var_proj_src_dir}/mongoc-linux-distro-scanner.c
		${var_proj_src_dir}/mongoc-list.c
		${var_proj_src_dir}/mongoc-log.c
		${var_proj_src_dir}/mongoc-matcher-op.c
		${var_proj_src_dir}/mongoc-matcher.c
		${var_proj_src_dir}/mongoc-memcmp.c
		${var_proj_src_dir}/mongoc-ocsp-cache.c
		${var_proj_src_dir}/mongoc-opcode.c
		${var_proj_src_dir}/mongoc-openssl.c
		${var_proj_src_dir}/mongoc-optional.c
		${var_proj_src_dir}/mongoc-opts-helpers.c
		${var_proj_src_dir}/mongoc-opts.c
		${var_proj_src_dir}/mongoc-queue.c
		${var_proj_src_dir}/mongoc-rand-cng.c
		${var_proj_src_dir}/mongoc-rand-common-crypto.c
		${var_proj_src_dir}/mongoc-rand-openssl.c
		${var_proj_src_dir}/mongoc-read-concern.c
		${var_proj_src_dir}/mongoc-read-prefs.c
		${var_proj_src_dir}/mongoc-rpc.c
		${var_proj_src_dir}/mongoc-sasl.c
		${var_proj_src_dir}/mongoc-scram.c
		${var_proj_src_dir}/mongoc-secure-channel.c
		${var_proj_src_dir}/mongoc-secure-transport.c
		${var_proj_src_dir}/mongoc-server-api.c
		${var_proj_src_dir}/mongoc-server-description.c
		${var_proj_src_dir}/mongoc-server-monitor.c
		${var_proj_src_dir}/mongoc-server-stream.c
		${var_proj_src_dir}/mongoc-set.c
		${var_proj_src_dir}/mongoc-shared.c
		${var_proj_src_dir}/mongoc-socket.c
		${var_proj_src_dir}/mongoc-ssl.c
		${var_proj_src_dir}/mongoc-sspi.c
		${var_proj_src_dir}/mongoc-stream-buffered.c
		${var_proj_src_dir}/mongoc-stream-file.c
		${var_proj_src_dir}/mongoc-stream-gridfs-download.c
		${var_proj_src_dir}/mongoc-stream-gridfs-upload.c
		${var_proj_src_dir}/mongoc-stream-gridfs.c
		${var_proj_src_dir}/mongoc-stream-socket.c
		${var_proj_src_dir}/mongoc-stream-tls-libressl.c
		${var_proj_src_dir}/mongoc-stream-tls-openssl-bio.c
		${var_proj_src_dir}/mongoc-stream-tls-openssl.c
		${var_proj_src_dir}/mongoc-stream-tls-secure-channel.c
		${var_proj_src_dir}/mongoc-stream-tls-secure-transport.c
		${var_proj_src_dir}/mongoc-stream-tls.c
		${var_proj_src_dir}/mongoc-stream.c
		${var_proj_src_dir}/mongoc-timeout.c
		${var_proj_src_dir}/mongoc-topology-background-monitoring.c
		${var_proj_src_dir}/mongoc-topology-description-apm.c
		${var_proj_src_dir}/mongoc-topology-description.c
		${var_proj_src_dir}/mongoc-topology-scanner.c
		${var_proj_src_dir}/mongoc-topology.c
		${var_proj_src_dir}/mongoc-ts-pool.c
		${var_proj_src_dir}/mongoc-uri.c
		${var_proj_src_dir}/mongoc-util.c
		${var_proj_src_dir}/mongoc-version-functions.c
		${var_proj_src_dir}/mongoc-write-command.c
		${var_proj_src_dir}/mongoc-write-concern.c
		${var_proj_src_dir}/service-gcp.c
)

set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})
set(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})
set(var_proj_ach_outdir ${var_proj_lib_outdir})

set(var_proj_include_dirs 
		${var_proj_src_dir}
		${var_proj_src_dir}/../include
		${var_proj_src_dir}/../include/bson
		${var_proj_src_dir}/../include/common
		${var_proj_src_dir}/../include/jsonsl
		${var_proj_src_dir}/../include/kms_message
		${var_proj_src_dir}/../include/mongo_crypt
		${var_proj_src_dir}/../include/mongoc
		${var_proj_src_dir}/../include/mongocrypt
		${var_proj_src_dir}/../include/mongocrypt/unicode
		${var_proj_src_dir}/../include/utf8proc
		${USRDEF_CMAKE_INCLUDE_DIRS}
)

set(var_proj_c_defineds "-DBSON_STATIC -DBUILD_MONGODB_DRIVER_YGGR_VER -DKMS_MSG_STATIC -DMLIB_USER -DMONGOCRYPT_LITTLE_ENDIAN -DMONGOCRYPT_STATIC_DEFINE -DMONGOC_COMPILATION -DMONGOC_STATIC -DUTF8PROC_STATIC ${USRDEF_CMAKE_DEFINEDS}")

set(var_proj_c_defineds_debug "-D_DEBUG ${USRDEF_CMAKE_DEFINEDS_DEBUG}")

set(var_proj_c_defineds_release "-DNDEBUG ${USRDEF_CMAKE_DEFINEDS_RELEASE}")

set(var_proj_c_flags "-Wall -Werror=endif-labels -Werror=return-type -Wno-comment -Wno-enum-compare -Wno-multichar -Wno-sign-compare -Wno-unused-local-typedefs -fPIC -fdata-sections -fexceptions -ffunction-sections -fno-strict-aliasing ${USRDEF_CMAKE_CFLAGS}")

set(var_proj_c_flags_debug "-O0 -g ${USRDEF_CMAKE_CFLAGS_DEBUG}")

set(var_proj_c_flags_release "-O3 ${USRDEF_CMAKE_CFLAGS_RELEASE}")

set(var_proj_cxx_flags 
		-frtti
		${USRDEF_CMAKE_CXXFLAGS}
)

set(var_proj_cxx_flags_debug 
		${USRDEF_CMAKE_CXXFLAGS_DEBUG}
)

set(var_proj_cxx_flags_release 
		${USRDEF_CMAKE_CXXFLAGS_RELEASE}
)

set(var_proj_link_libs_dirs 
		${USRDEF_CMAKE_LD_LIBRARY_PATH}
)

set(var_proj_link_libs 
		${USRDEF_CMAKE_LD_LIBRARY}
)

set(var_proj_link_libs_debug 
		${USRDEF_CMAKE_LDFLAGS_DEBUG}
)

set(var_proj_link_libs_release 
		${USRDEF_CMAKE_LD_LIBRARY_RELEASE}
)

set(var_proj_ldflags 
		${USRDEF_CMAKE_LDFLAGS}
)

set(var_proj_ldflags_debug 
		${USRDEF_CMAKE_LDFLAGS_DEBUG}
)

set(var_proj_ldflags_release 
		${USRDEF_CMAKE_LDFLAGS_RELEASE}
)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set(var_proj_out_name ${var_proj_name}${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d)
	set(var_proj_c_defineds "${var_proj_c_defineds_debug} ${var_proj_c_defineds}")
	set(var_proj_c_flags "${var_proj_c_flags_debug} ${var_proj_c_flags}")
	set(var_proj_cxx_flags "${var_proj_cxx_flags_debug} ${var_proj_cxx_flags} ${var_proj_c_defineds}")
	set(var_proj_link_libs ${var_proj_link_libs_debug} ${var_proj_link_libs})
	set(var_proj_ldflags "${var_proj_ldflags_debug} ${var_proj_ldflags}")
else()
	set(var_proj_out_name ${var_proj_name}${USRDEF_CMAKE_COMPILER_VERSION_TAG})
	set(var_proj_c_defineds "${var_proj_c_defineds_release} ${var_proj_c_defineds}")
	set(var_proj_c_flags "${var_proj_c_flags_release} ${var_proj_c_flags}")
	set(var_proj_cxx_flags "${var_proj_cxx_flags_release} ${var_proj_cxx_flags} ${var_proj_c_defineds}")
	set(var_proj_link_libs ${var_proj_link_libs_release} ${var_proj_link_libs})
	set(var_proj_ldflags "${var_proj_ldflags_release} ${var_proj_ldflags}")
endif()

include_directories(${var_proj_include_dirs})
add_definitions(${var_proj_c_defineds})
set(CMAKE_C_FLAGS ${var_proj_c_flags})
set(CMAKE_CXX_FLAGS ${var_proj_cxx_flags})

add_library(${var_proj_name} ${var_proj_src_list})

set_target_properties( 
	${var_proj_name}
	PROPERTIES
	OUTPUT_NAME ${var_proj_out_name}
	RUNTIME_OUTPUT_DIRECTORY ${var_proj_bin_outdir}
	LIBRARY_OUTPUT_DIRECTORY ${var_proj_lib_outdir}
	ARCHIVE_OUTPUT_DIRECTORY ${var_proj_ach_outdir}
)

