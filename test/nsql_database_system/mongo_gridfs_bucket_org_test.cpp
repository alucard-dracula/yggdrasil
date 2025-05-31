//mongo_gridfs_bucket_org_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int
main (int argc, char *argv[])
{

#if defined(YGGR_AT_MOBILE)
	const char *uri_string =
		"mongodb://192.168.1.101:10098/?appname=new-gridfs-example";
#else
	const char *uri_string =
		"mongodb://127.0.0.1:10098/?appname=new-gridfs-example";
#endif // YGGR_AT_MOBILE

	mongoc_client_t *client = 0;
	mongoc_database_t *db = 0;
	mongoc_stream_t *file_stream = 0;
	mongoc_gridfs_bucket_t *bucket = 0;
	mongoc_cursor_t *cursor = 0;
	bson_t filter = {0};
	bool res = false;
	bson_value_t file_id = {static_cast<bson_type_t>(0)};
	bson_error_t error = {0};
	const bson_t *doc = 0;
	char *str = 0;
	mongoc_init ();

	//if (argc != 3) {
	//   fprintf (stderr, "usage: %s SOURCE_FILE_PATH FILE_COPY_PATH\n", argv[0]);
	//   return EXIT_FAILURE;
	//}

	///* 1. Make a bucket. */
	client = mongoc_client_new (uri_string);
	db = mongoc_client_get_database (client, "test_gridfs_bucket");
	bucket = mongoc_gridfs_bucket_new (db, NULL, NULL, &error);
	if (!bucket) {
		printf ("Error creating gridfs bucket: %s\n", error.message);
		return EXIT_FAILURE;
	}

	///* 2. Insert a file.  */
	file_stream = mongoc_stream_file_new_for_path ("./nsql_database_system/gridfs_test_file.txt", O_RDONLY, 0);
	res = mongoc_gridfs_bucket_upload_from_stream (
		bucket, "nsql_database_system/gridfs_test_file.txt", file_stream, NULL, &file_id, &error);
	if (!res) {
		printf ("Error uploading file: %s\n", error.message);
		return EXIT_FAILURE;
	}

	mongoc_stream_close (file_stream);
	mongoc_stream_destroy (file_stream);

	///* 3. Download the file in GridFS to a local file. */
	file_stream = mongoc_stream_file_new_for_path ("./nsql_database_system/org_out_gridfs_test_file.txt", O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
	//file_stream = mongoc_stream_file_new_for_path ("./nsql_database_system/org_out_gridfs_test_file.txt", O_RDWR, 0);
	if (!file_stream) {
	   perror ("Error opening file stream");
	   return EXIT_FAILURE;
	}

	res = mongoc_gridfs_bucket_download_to_stream (
	   bucket, &file_id, file_stream, &error);
	if (!res) {
	   printf ("Error downloading file to stream: %s\n", error.message);
	   return EXIT_FAILURE;
	}
	mongoc_stream_close (file_stream);
	mongoc_stream_destroy (file_stream);

	///* 4. List what files are available in GridFS. */
	bson_init (&filter);
	cursor = mongoc_gridfs_bucket_find (bucket, &filter, NULL);

	while (mongoc_cursor_next (cursor, &doc)) {
	   str = bson_as_canonical_extended_json (doc, NULL);
	   printf ("%s\n", str);
	   bson_free (str);
	}

	///* 5. Delete the file that we added. */
	res = mongoc_gridfs_bucket_delete_by_id (bucket, &file_id, &error);
	if (!res) {
	   printf ("Error deleting the file: %s\n", error.message);
	   return EXIT_FAILURE;
	}

	///* 6. Cleanup. */
	//mongoc_stream_close (file_stream);
	//mongoc_stream_destroy (file_stream);
	mongoc_cursor_destroy (cursor);
	bson_destroy (&filter);

	mongoc_gridfs_bucket_destroy (bucket);
	mongoc_database_destroy (db);
	mongoc_client_destroy (client);
	mongoc_cleanup ();

	wait_any_key(argc, argv);

	return EXIT_SUCCESS;
}