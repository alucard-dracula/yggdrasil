#include <fcntl.h>
#include <mongoc.h>
#include <mongoc-cluster-private.h>

#include <iostream>

#include "mongoc-cursor-private.h"
#include "mock-server.h"
#include "mongoc-client-private.h"
#include "mongoc-tests.h"
#include "TestSuite.h"

#include "test-libmongoc.h"

#define MONGOD_VERSION_HEX(a, b, c) ((a << 16) | (b << 8) | (c))


#undef MONGOC_LOG_DOMAIN
#define MONGOC_LOG_DOMAIN "client-test"

#ifdef _MSC_VER
#	if defined(_DEBUG)
#		if _MSC_VER == 1600
#			pragma comment(lib, "libbson-vc100-d.lib")
#			pragma comment(lib, "libcommon-vc100-d.lib")
#			pragma comment(lib, "libmongoc-vc100-d.lib")
#			pragma comment(lib, "libmongocrypt-vc100-d.lib")
#			pragma comment(lib, "libkms-message-vc100-d.lib")
#			pragma comment(lib, "libutf8proc-vc100-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc100-d.lib")
#			pragma comment(lib, "libbase64-vc100-d.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libbson-vc110-d.lib")
#			pragma comment(lib, "libcommon-vc110-d.lib")
#			pragma comment(lib, "libmongoc-vc110-d.lib")
#			pragma comment(lib, "libmongocrypt-vc110-d.lib")
#			pragma comment(lib, "libkms-message-vc110-d.lib")
#			pragma comment(lib, "libutf8proc-vc110-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc110-d.lib")
#			pragma comment(lib, "libbase64-vc110-d.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libbson-vc120-d.lib")
#			pragma comment(lib, "libcommon-vc120-d.lib")
#			pragma comment(lib, "libmongoc-vc120-d.lib")
#			pragma comment(lib, "libmongocrypt-vc120-d.lib")
#			pragma comment(lib, "libkms-message-vc120-d.lib")
#			pragma comment(lib, "libutf8proc-vc120-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc120-d.lib")
#			pragma comment(lib, "libbase64-vc120-d.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libbson-vc140-d.lib")
#			pragma comment(lib, "libcommon-vc140-d.lib")
#			pragma comment(lib, "libmongoc-vc140-d.lib")
#			pragma comment(lib, "libmongocrypt-vc140-d.lib")
#			pragma comment(lib, "libkms-message-vc140-d.lib")
#			pragma comment(lib, "libutf8proc-vc140-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc140-d.lib")
#			pragma comment(lib, "libbase64-vc140-d.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libbson-vc141-d.lib")
#			pragma comment(lib, "libcommon-vc141-d.lib")
#			pragma comment(lib, "libmongoc-vc141-d.lib")
#			pragma comment(lib, "libmongocrypt-vc141-d.lib")
#			pragma comment(lib, "libkms-message-vc141-d.lib")
#			pragma comment(lib, "libutf8proc-vc141-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc141-d.lib")
#			pragma comment(lib, "libbase64-vc141-d.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libbson-vc142-d.lib")
#			pragma comment(lib, "libcommon-vc142-d.lib")
#			pragma comment(lib, "libmongoc-vc142-d.lib")
#			pragma comment(lib, "libmongocrypt-vc142-d.lib")
#			pragma comment(lib, "libkms-message-vc142-d.lib")
#			pragma comment(lib, "libutf8proc-vc142-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc142-d.lib")
#			pragma comment(lib, "libbase64-vc142-d.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libbson-vc143-d.lib")
#			pragma comment(lib, "libcommon-vc143-d.lib")
#			pragma comment(lib, "libmongoc-vc143-d.lib")
#			pragma comment(lib, "libmongocrypt-vc143-d.lib")
#			pragma comment(lib, "libkms-message-vc143-d.lib")
#			pragma comment(lib, "libutf8proc-vc143-d.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc143-d.lib")
#			pragma comment(lib, "libbase64-vc143-d.lib")
#		else
#			error "set libs"
#		endif // ( _MSC_VER == 1600)
#	else // _DEBUG
#		if _MSC_VER == 1600
#			pragma comment(lib, "libbson-vc100.lib")
#			pragma comment(lib, "libcommon-vc100.lib")
#			pragma comment(lib, "libmongoc-vc100.lib")
#			pragma comment(lib, "libmongocrypt-vc100.lib")
#			pragma comment(lib, "libkms-message-vc100.lib")
#			pragma comment(lib, "libutf8proc-vc100.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc100.lib")
#			pragma comment(lib, "libbase64-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libbson-vc110.lib")
#			pragma comment(lib, "libcommon-vc110.lib")
#			pragma comment(lib, "libmongoc-vc110.lib")
#			pragma comment(lib, "libmongocrypt-vc110.lib")
#			pragma comment(lib, "libkms-message-vc110.lib")
#			pragma comment(lib, "libutf8proc-vc110.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc110.lib")
#			pragma comment(lib, "libbase64-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libbson-vc120.lib")
#			pragma comment(lib, "libcommon-vc120.lib")
#			pragma comment(lib, "libmongoc-vc120.lib")
#			pragma comment(lib, "libmongocrypt-vc120.lib")
#			pragma comment(lib, "libkms-message-vc120.lib")
#			pragma comment(lib, "libutf8proc-vc120.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc120.lib")
#			pragma comment(lib, "libbase64-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libbson-vc140.lib")
#			pragma comment(lib, "libcommon-vc140.lib")
#			pragma comment(lib, "libmongoc-vc140.lib")
#			pragma comment(lib, "libmongocrypt-vc140.lib")
#			pragma comment(lib, "libkms-message-vc140.lib")
#			pragma comment(lib, "libutf8proc-vc140.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc140.lib")
#			pragma comment(lib, "libbase64-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libbson-vc141.lib")
#			pragma comment(lib, "libcommon-vc141.lib")
#			pragma comment(lib, "libmongoc-vc141.lib")
#			pragma comment(lib, "libmongocrypt-vc141.lib")
#			pragma comment(lib, "libkms-message-vc141.lib")
#			pragma comment(lib, "libutf8proc-vc141.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc141.lib")
#			pragma comment(lib, "libbase64-vc141.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libbson-vc142.lib")
#			pragma comment(lib, "libcommon-vc142.lib")
#			pragma comment(lib, "libmongoc-vc142.lib")
#			pragma comment(lib, "libmongocrypt-vc142.lib")
#			pragma comment(lib, "libmongocrypt-vc142.lib")
#			pragma comment(lib, "libkms-message-vc142.lib")
#			pragma comment(lib, "libutf8proc-vc142.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc142.lib")
#			pragma comment(lib, "libbase64-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libbson-vc143.lib")
#			pragma comment(lib, "libcommon-vc143.lib")
#			pragma comment(lib, "libmongoc-vc143.lib")
#			pragma comment(lib, "libmongocrypt-vc143.lib")
#			pragma comment(lib, "libmongocrypt-vc143.lib")
#			pragma comment(lib, "libkms-message-vc143.lib")
#			pragma comment(lib, "libutf8proc-vc143.lib")
#			pragma comment(lib, "libintel_dfp_obj-vc143.lib")
#			pragma comment(lib, "libbase64-vc143.lib")
#		else
#			error "set libs"
#		endif // ( _MSC_VER == 1600)
#	endif // _DEBUG
#endif // _MSC_VER

#ifdef _MSC_VER
//#	pragma comment(lib, "ssleay32.lib")
//#	pragma comment(lib, "libeay32.lib")

#	if defined(_DEBUG)
#		pragma comment(lib, "zlibstaticd.lib")
#		pragma comment(lib, "icuucd.lib")
#	else
#		pragma comment(lib, "zlibstatic.lib")
#		pragma comment(lib, "icuuc.lib")
#	endif // _DEBUG

#	pragma comment(lib, "ws2_32.lib")
#	pragma comment(lib, "Dnsapi.lib")
#	pragma comment(lib, "Bcrypt.lib")
#	pragma comment(lib, "Secur32.lib")
#	pragma comment(lib, "Crypt32.lib")
#endif // _MSC_VER

#ifdef _MSC_VER
#	if 1
#		if _MSC_VER == 1600
#			pragma comment(lib, "libcrypto-vc100.lib")
#			pragma comment(lib, "libssl-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libcrypto-vc110.lib")
#			pragma comment(lib, "libssl-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libcrypto-vc120.lib")
#			pragma comment(lib, "libssl-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libcrypto-vc140.lib")
#			pragma comment(lib, "libssl-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libcrypto-vc141.lib")
#			pragma comment(lib, "libssl-vc141.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libcrypto-vc142.lib")
#			pragma comment(lib, "libssl-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libcrypto-vc143.lib")
#			pragma comment(lib, "libssl-vc143.lib")
#		else
#		endif // ( _MSC_VER == 1600)
#	else
#		if _MSC_VER == 1600
#			pragma comment(lib, "libcrypto_static-vc100.lib")
#			pragma comment(lib, "libssl_static-vc100.lib")
#		elif _MSC_VER == 1700
#			pragma comment(lib, "libcrypto_static-vc110.lib")
#			pragma comment(lib, "libssl_static-vc110.lib")
#		elif _MSC_VER == 1800
#			pragma comment(lib, "libcrypto_static-vc120.lib")
#			pragma comment(lib, "libssl_static-vc120.lib")
#		elif _MSC_VER == 1900
#			pragma comment(lib, "libcrypto_static-vc140.lib")
#			pragma comment(lib, "libssl_static-vc140.lib")
#		elif _MSC_VER == 1910
#			pragma comment(lib, "libcrypto_static-vc141.lib")
#			pragma comment(lib, "libssl_static-vc141.lib")
#		elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#			pragma comment(lib, "libcrypto_static-vc142.lib")
#			pragma comment(lib, "libssl_static-vc142.lib")
#		elif 1930 <= _MSC_VER && _MSC_VER <= 1940
#			pragma comment(lib, "libcrypto_static-vc143.lib")
#			pragma comment(lib, "libssl_static-vc143.lib")
#		else
#		endif // ( _MSC_VER == 1600)
#	endif // 0,1
#endif // _MSC_VER

static mongoc_collection_t *
get_test_collection (mongoc_client_t *client,
                     const char      *name)
{
   mongoc_collection_t *ret;
   char *str;

   str = gen_collection_name (name);
   ret = mongoc_client_get_collection (client, "test", str);
   bson_free (str);

   return ret;
}


static char *
gen_test_user (void)
{
   return bson_strdup_printf ("testuser_%u_%u",
                              (unsigned)time(NULL),
                              (unsigned)gettestpid());
}


static char *
gen_good_uri (const char *username,
              const char *dbname)
{
   char *host = test_framework_get_host ();
   char *uri = bson_strdup_printf ("mongodb://%s:testpass@%s/%s",
                                   username,
                                   host,
                                   dbname);

   bson_free (host);
   return uri;
}


static void
test_mongoc_client_authenticate (void)
{
   mongoc_client_t *admin_client;
   char *username;
   char *uri;
   bson_t roles;
   mongoc_database_t *database;
   mongoc_collection_t *collection;
   mongoc_client_t *auth_client;
   mongoc_cursor_t *cursor;
   const bson_t *doc;
   bson_error_t error;
   bool r;
   bson_t q;

   /*
    * Log in as admin.
    */
   admin_client = test_framework_client_new (NULL);

   /*
    * Add a user to the test database.
    */
   username = gen_test_user ();
   uri = gen_good_uri (username, "test");

   database = mongoc_client_get_database (admin_client, "test");
   mongoc_database_remove_user (database, username, &error);
   bson_init (&roles);
   BCON_APPEND (&roles,
                "0", "{", "role", "read", "db", "test", "}");
   r = mongoc_database_add_user(database, username, "testpass", &roles, NULL, &error);
   ASSERT_CMPINT(r, ==, 1);
   mongoc_database_destroy(database);

   /*
    * Try authenticating with that user.
    */
   bson_init(&q);
   auth_client = test_framework_client_new (uri);
   collection = mongoc_client_get_collection (auth_client, "test", "test");
   cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 1, 0,
                                   &q, NULL, NULL);
   r = mongoc_cursor_next(cursor, &doc);
   if (!r) {
      r = mongoc_cursor_error(cursor, &error);
      if (r) {
         MONGOC_ERROR("Authentication failure: \"%s\"", error.message);
      }
      assert(!r);
   }

   /*
    * Remove all test users.
    */
   database = mongoc_client_get_database (admin_client, "test");
   r = mongoc_database_remove_all_users (database, &error);
   assert (r);

   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (auth_client);
   bson_destroy (&roles);
   bson_free (uri);
   bson_free (username);
   mongoc_database_destroy (database);
   mongoc_client_destroy (admin_client);

   std::cout << "test_mongoc_client_authenticate end" << std::endl;
}


static void
test_mongoc_client_authenticate_failure (void)
{
   mongoc_collection_t *collection;
   mongoc_cursor_t *cursor;
   mongoc_client_t *client;
   const bson_t *doc;
   bson_error_t error;
   bool r;
   bson_t q;
   bson_t empty = BSON_INITIALIZER;
   char *host = test_framework_get_host ();
   char *bad_uri_str = bson_strdup_printf (
         "mongodb://baduser:badpass@%s/test%s",
         host,
         test_framework_get_ssl () ? "?ssl=true" : "");

   /*
    * Try authenticating with bad user.
    */
   bson_init(&q);
   client = test_framework_client_new (bad_uri_str);
   collection = mongoc_client_get_collection(client, "test", "test");
   cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 1, 0,
                                   &q, NULL, NULL);
   r = mongoc_cursor_next(cursor, &doc);
   assert(!r);
   r = mongoc_cursor_error(cursor, &error);
   assert(r);
   assert(error.domain == MONGOC_ERROR_CLIENT);
   assert(error.code == MONGOC_ERROR_CLIENT_AUTHENTICATE);
   mongoc_cursor_destroy(cursor);

   /*
    * Try various commands while in the failed state to ensure we get the
    * same sort of errors.
    */
   r = mongoc_collection_insert (collection, static_cast<mongoc_insert_flags_t>(0), &empty, NULL, &error);
   assert (!r);
   assert (error.domain == MONGOC_ERROR_CLIENT);
   assert (error.code == MONGOC_ERROR_CLIENT_AUTHENTICATE);

   /*
    * Try various commands while in the failed state to ensure we get the
    * same sort of errors.
    */
   r = mongoc_collection_update (collection, static_cast<mongoc_update_flags_t>(0), &q, &empty, NULL, &error);
   assert (!r);
   assert (error.domain == MONGOC_ERROR_CLIENT);
   assert (error.code == MONGOC_ERROR_CLIENT_AUTHENTICATE);

   bson_free (host);
   bson_free (bad_uri_str);
   mongoc_collection_destroy(collection);
   mongoc_client_destroy(client);
}


static void
test_wire_version (void)
{
   mongoc_collection_t *collection;
   mongoc_cursor_t *cursor;
   mongoc_client_t *client;
   mock_server_t *server;
   uint16_t port;
   const bson_t *doc;
   bson_error_t error;
   bool r;
   bson_t q = BSON_INITIALIZER;
   char *uristr;

   port = 20000 + (rand () % 1000);

   server = mock_server_new ("127.0.0.1", port, NULL, NULL);
   mock_server_set_wire_version (server, 10, 11);
   mock_server_run_in_thread (server);

   usleep (5000);

   uristr = bson_strdup_printf ("mongodb://127.0.0.1:%hu/", port);
   client = mongoc_client_new (uristr);

   collection = mongoc_client_get_collection (client, "test", "test");

   cursor = mongoc_collection_find (collection,
                                    MONGOC_QUERY_NONE,
                                    0,
                                    1,
                                    0,
                                    &q,
                                    NULL,
                                    NULL);

   r = mongoc_cursor_next (cursor, &doc);
   assert (!r);

   r = mongoc_cursor_error (cursor, &error);
   assert (r);

   assert (error.domain == MONGOC_ERROR_PROTOCOL);
   assert (error.code == MONGOC_ERROR_PROTOCOL_BAD_WIRE_VERSION);

   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mock_server_quit (server);
   mongoc_client_destroy (client);
   bson_free (uristr);
}


static void
read_prefs_handler (mock_server_t   *server,
                    mongoc_stream_t *stream,
                    mongoc_rpc_t    *rpc,
                    void            *user_data)
{
   bool *success = reinterpret_cast<bool*>(user_data);
   int32_t len;
   bson_iter_t iter;
   bson_iter_t child;
   bson_iter_t child2;
   bson_iter_t child3;
   bson_t b;
   bson_t reply = BSON_INITIALIZER;
   int r;

   if (rpc->header.opcode == MONGOC_OPCODE_QUERY) {
      memcpy (&len, rpc->query.query, 4);
      len = BSON_UINT32_FROM_LE (len);

      r = bson_init_static (&b, rpc->query.query, len);
      assert (r);

      r = bson_iter_init_find (&iter, &b, "$query");
      assert (r);
      assert (BSON_ITER_HOLDS_DOCUMENT (&iter));

      r = bson_iter_init_find (&iter, &b, "$readPreference");
      assert (r);
      assert (BSON_ITER_HOLDS_DOCUMENT (&iter));

      r = bson_iter_recurse (&iter, &child);
      assert (r);

      r = bson_iter_next (&child);
      assert (r);
      assert (BSON_ITER_HOLDS_UTF8 (&child));
      assert (!strcmp ("mode", bson_iter_key (&child)));
      assert (!strcmp ("secondaryPreferred", bson_iter_utf8 (&child, NULL)));

      r = bson_iter_next (&child);
      assert (r);
      assert (BSON_ITER_HOLDS_ARRAY (&child));

      r = bson_iter_recurse (&child, &child2);
      assert (r);

      r = bson_iter_next (&child2);
      assert (r);
      assert (BSON_ITER_HOLDS_DOCUMENT (&child2));

      r = bson_iter_recurse (&child2, &child3);
      assert (r);

      r = bson_iter_next (&child3);
      assert (r);
      assert (BSON_ITER_HOLDS_UTF8 (&child3));
      assert (!strcmp ("dc", bson_iter_key (&child3)));
      assert (!strcmp ("ny", bson_iter_utf8 (&child3, NULL)));
      r = bson_iter_next (&child3);
      assert (!r);

      r = bson_iter_next (&child2);
      assert (r);

      r = bson_iter_recurse (&child2, &child3);
      assert (r);

      r = bson_iter_next (&child3);
      assert (!r);

      mock_server_reply_simple (server, stream, rpc, MONGOC_REPLY_NONE, &reply);

      *success = true;
   }
}


static void
test_mongoc_client_read_prefs (void)
{
   mongoc_collection_t *collection;
   mongoc_read_prefs_t *read_prefs;
   mongoc_cursor_t *cursor;
   mongoc_client_t *client;
   mock_server_t *server;
   uint16_t port;
   const bson_t *doc;
   bson_error_t error;
   bool success = false;
   bson_t b = BSON_INITIALIZER;
   bson_t q = BSON_INITIALIZER;
   char *uristr;

   port = 20000 + (rand () % 1000);

   server = mock_server_new ("127.0.0.1", port, read_prefs_handler, &success);
   mock_server_run_in_thread (server);

   usleep (5000);

   uristr = bson_strdup_printf ("mongodb://127.0.0.1:%hu/", port);
   client = mongoc_client_new (uristr);

   if (!_mongoc_client_warm_up (client, &error)) {
      assert (false);
   }

   collection = mongoc_client_get_collection (client, "test", "test");

   bson_append_utf8 (&b, "dc", 2, "ny", 2);

   read_prefs = mongoc_read_prefs_new (MONGOC_READ_SECONDARY_PREFERRED);
   mongoc_read_prefs_add_tag (read_prefs, &b);
   mongoc_read_prefs_add_tag (read_prefs, NULL);
   mongoc_collection_set_read_prefs (collection, read_prefs);

   cursor = mongoc_collection_find (collection,
                                    MONGOC_QUERY_NONE,
                                    0,
                                    1,
                                    0,
                                    &q,
                                    NULL,
                                    read_prefs);

   mongoc_cursor_next (cursor, &doc);

   usleep (50000);

   assert (success);

   mongoc_read_prefs_destroy (read_prefs);
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);
   mock_server_quit (server);
   bson_destroy (&b);
   bson_free (uristr);
}


static void
test_mongoc_client_command (void)
{
   mongoc_client_t *client;
   mongoc_cursor_t *cursor;
   const bson_t *doc;
   bool r;
   bson_t cmd = BSON_INITIALIZER;

   //client = test_framework_client_new (NULL);
    client = test_framework_client_new ("mongodb://127.0.0.1:10098/");
   assert (client);

   bson_append_int32 (&cmd, "ping", 4, 1);

   cursor = mongoc_client_command (client, "admin", MONGOC_QUERY_NONE, 0, 1, 0, &cmd, NULL, NULL);
   assert (!cursor->redir_primary);

   r = mongoc_cursor_next (cursor, &doc);
   assert (r);
   assert (doc);

   r = mongoc_cursor_next (cursor, &doc);
   assert (!r);
   assert (!doc);

   mongoc_cursor_destroy (cursor);
   mongoc_client_destroy (client);
   bson_destroy (&cmd);
}


static void
test_mongoc_client_command_secondary (void)
{
   mongoc_client_t *client;
   mongoc_cursor_t *cursor;
   mongoc_read_prefs_t *read_prefs;
   bson_t cmd = BSON_INITIALIZER;

   client = test_framework_client_new (NULL);
   assert (client);

   BSON_APPEND_INT32 (&cmd, "invalid_command_here", 1);

   read_prefs = mongoc_read_prefs_new (MONGOC_READ_PRIMARY_PREFERRED);

   suppress_one_message ();
   cursor = mongoc_client_command (client, "admin", MONGOC_QUERY_NONE, 0, 1, 0, &cmd, NULL, read_prefs);

   mongoc_read_prefs_destroy (read_prefs);

   /* ensure we detected this must go to primary */
   assert (cursor->redir_primary);

   mongoc_cursor_destroy (cursor);
   mongoc_client_destroy (client);
   bson_destroy (&cmd);
}

static void
test_mongoc_client_preselect (void)
{
   mongoc_client_t *client;
   bson_error_t error;
   uint32_t node;

   client = test_framework_client_new (NULL);
   assert (client);

   node = _mongoc_client_preselect (client, MONGOC_OPCODE_INSERT,
                                    NULL, NULL, &error);
   assert (node > 0);

   mongoc_client_destroy (client);
}


static void
test_unavailable_seeds(void)
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   mongoc_cursor_t *cursor;
   bson_t query = BSON_INITIALIZER;
   const bson_t *doc;
   
   const char* uri_strs[] = {
      "mongodb://a:1/?connectTimeoutMS=1",
      "mongodb://a:1,a:2/?connectTimeoutMS=1",
      "mongodb://a:1,a:2/?replicaSet=r&connectTimeoutMS=1",
      "mongodb://u:p@a:1/?connectTimeoutMS=1",
      "mongodb://u:p@a:1,a:2/?connectTimeoutMS=1",
      "mongodb://u:p@a:1,a:2/?replicaSet=r&connectTimeoutMS=1",
   };

   int i;

   /* hardcode the number of error messages we have to suppress */
   for (i = 0; i < 18; i++) {
      suppress_one_message ();
   }

   for (i = 0; i < (sizeof(uri_strs) / sizeof(const char *)); i++) {
      client = mongoc_client_new (uri_strs[i]);
      assert (client);

      collection = mongoc_client_get_collection (client, "test", "test");
      cursor = mongoc_collection_find (collection,
                                       MONGOC_QUERY_NONE,
                                       0,
                                       0,
                                       0,
                                       &query,
                                       NULL,
                                       NULL);

      assert (! mongoc_cursor_next (cursor, &doc));

      mongoc_cursor_destroy (cursor);
      mongoc_collection_destroy (collection);
      mongoc_client_destroy (client);
   }
}


typedef enum {
   NO_CONNECT,
   CONNECT,
   RECONNECT
} connection_option_t;


/* CDRIVER-721 catch errors in _mongoc_cluster_destroy */
static void 
test_seed_list (bool rs,
                connection_option_t connection_option)
{
   uint16_t port;
   char *uri_str;
   mongoc_uri_t *uri;
   mock_server_t *server;
   uint32_t i;
   uint32_t expected_nodes_len;
   const mongoc_host_list_t *hosts;
   mongoc_client_t *client;
   bson_error_t error;

   port = 20000 + (rand () % 1000);
   uri_str = bson_strdup_printf ("mongodb://localhost:%hu,a,b,c/%s",
                                 port,
                                 rs ? "?replicaSet=rs" : "");
   uri = mongoc_uri_new (uri_str);
   hosts = mongoc_uri_get_hosts (uri);
   
   server = mock_server_new_rs ("127.0.0.1", port, NULL, NULL,
                                rs ? "rs" : NULL, true, false, hosts, NULL);

   mock_server_run_in_thread (server);

   for (i = 0; i < 12; i++) {
      suppress_one_message ();
   }

   client = mongoc_client_new_from_uri (uri);
   assert (client);
   ASSERT_CMPINT (4, ==, client->cluster.nodes_len);
   for (i = 0; i < 4; i++) {
      assert (client->cluster.nodes[i].valid);
   }

   expected_nodes_len = rs ? 1 : 4;

   if (connection_option != NO_CONNECT) {
      /* only localhost:port responds, nodes_len is set to 1 */
      assert (_mongoc_client_warm_up (client, &error));

      /* a mongos load-balanced connection never removes down nodes */
      ASSERT_CMPINT (expected_nodes_len, ==, client->cluster.nodes_len);
      for (i = 0; i < expected_nodes_len; i++) {
         assert (client->cluster.nodes[i].valid);
      }
   }

   if (connection_option == RECONNECT) {
      for (i = 0; i < 12; i++) {
         suppress_one_message ();
      }

      assert (_mongoc_cluster_reconnect (&client->cluster, &error));
      ASSERT_CMPINT (expected_nodes_len, ==, client->cluster.nodes_len);
      for (i = 0; i < expected_nodes_len; i++) {
         assert (client->cluster.nodes[i].valid);
      }
   }

   /* testing for crashes like CDRIVER-721 */
   mongoc_client_destroy (client);

   mock_server_quit (server);
   mock_server_destroy (server);
   mongoc_uri_destroy (uri);
   bson_free (uri_str);
}


static void
test_rs_seeds_no_connect (void)
{
   test_seed_list (true, NO_CONNECT);
}


static void
test_rs_seeds_connect (void)
{
   test_seed_list (true, CONNECT);
}


static void
test_rs_seeds_reconnect (void)
{
   test_seed_list (true, RECONNECT);
}


static void
test_mongos_seeds_no_connect (void)
{
   test_seed_list (false, NO_CONNECT);
}


static void
test_mongos_seeds_connect (void)
{
   test_seed_list (false, CONNECT);
}


static void
test_mongos_seeds_reconnect (void)
{
   test_seed_list (false, RECONNECT);
}

static void
test_recovering (void)
{
   uint16_t port;
   char *uri_str;
   mongoc_uri_t *uri;
   mock_server_t *server;
   uint32_t i;
   const mongoc_host_list_t *hosts;
   mongoc_client_t *client;
   //mongoc_read_mode_t read_mode;
   unsigned long read_mode;
   mongoc_read_prefs_t *prefs;
   bson_error_t error;

   port = 20000 + (rand () % 1000);
   uri_str = bson_strdup_printf ("mongodb://localhost:%hu/?replicaSet=rs",
                                 port);

   uri = mongoc_uri_new (uri_str);
   hosts = mongoc_uri_get_hosts (uri);

   /* server is "recovering": not master, not secondary */
   server = mock_server_new_rs ("127.0.0.1", port, NULL, NULL,
                                "rs", false, false, hosts, NULL);

   mock_server_set_verbose (server, false);
   mock_server_run_in_thread (server);

   client = mongoc_client_new_from_uri (uri);
   assert (client);
   ASSERT_CMPINT (1, ==, client->cluster.nodes_len);
   for (i = 0; i < 1; i++) {
      assert (client->cluster.nodes[i].valid);
   }

   prefs = mongoc_read_prefs_new (MONGOC_READ_PRIMARY);

   /* recovering member matches no read mode */
   for (read_mode = MONGOC_READ_PRIMARY;
        read_mode <= MONGOC_READ_NEAREST;
        read_mode++) {
      mongoc_read_prefs_set_mode (prefs, static_cast<mongoc_read_mode_t>(read_mode));
      assert (!_mongoc_cluster_preselect (&client->cluster,
                                          MONGOC_OPCODE_QUERY,
                                          NULL, prefs, &error));
   }

   mongoc_read_prefs_destroy (prefs);
   mongoc_client_destroy (client);
   mock_server_quit (server);
   mock_server_destroy (server);
   mongoc_uri_destroy (uri);
   bson_free (uri_str);
}


static void
test_exhaust_cursor (void)
{
   mongoc_write_concern_t *wr;
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   mongoc_cursor_t *cursor;
   mongoc_cursor_t *cursor2;
   mongoc_stream_t *stream;
   mongoc_cluster_node_t *node;
   const bson_t *doc;
   bson_t q;
   bson_t b[10];
   bson_t *bptr[10];
   int i;
   bool r;
   bson_error_t error;
   bson_oid_t oid;

   client = test_framework_client_new (NULL);
   assert (client);

   collection = get_test_collection (client, "test_exhaust_cursor");
   assert (collection);

   mongoc_collection_drop(collection, &error);

   wr = mongoc_write_concern_new ();
   mongoc_write_concern_set_journal (wr, true);

   /* bulk insert some records to work on */
   {
      bson_init(&q);

      for (i = 0; i < 10; i++) {
         bson_init(&b[i]);
         bson_oid_init(&oid, NULL);
         bson_append_oid(&b[i], "_id", -1, &oid);
         bson_append_int32(&b[i], "n", -1, i % 2);
         bptr[i] = &b[i];
      }

      BEGIN_IGNORE_DEPRECATIONS;
      r = mongoc_collection_insert_bulk (collection, MONGOC_INSERT_NONE,
                                         (const bson_t **)bptr, 10, wr, &error);
      END_IGNORE_DEPRECATIONS;

      if (!r) {
         MONGOC_WARNING("Insert bulk failure: %s\n", error.message);
      }
      assert(r);
   }

   /* create a couple of cursors */
   {
      cursor = mongoc_collection_find (collection, MONGOC_QUERY_EXHAUST, 0, 0, 0, &q,
                                       NULL, NULL);

      cursor2 = mongoc_collection_find (collection, MONGOC_QUERY_NONE, 0, 0, 0, &q,
                                        NULL, NULL);
   }

   /* Read from the exhaust cursor, ensure that we're in exhaust where we
    * should be and ensure that an early destroy properly causes a disconnect
    * */
   {
      r = mongoc_cursor_next (cursor, &doc);
      assert (r);
      assert (doc);
      assert (cursor->in_exhaust);
      assert (client->in_exhaust);
      node = &client->cluster.nodes[cursor->hint - 1];
      stream = node->stream;

      mongoc_cursor_destroy (cursor);
      /* make sure a disconnect happened */
      assert (stream != node->stream);
      assert (! client->in_exhaust);
   }

   /* Grab a new exhaust cursor, then verify that reading from that cursor
    * (putting the client into exhaust), breaks a mid-stream read from a
    * regular cursor */
   {
      cursor = mongoc_collection_find (collection, MONGOC_QUERY_EXHAUST, 0, 0, 0, &q,
                                       NULL, NULL);

      for (i = 0; i < 5; i++) {
         r = mongoc_cursor_next (cursor2, &doc);
         assert (r);
         assert (doc);
      }

      r = mongoc_cursor_next (cursor, &doc);
      assert (r);
      assert (doc);

      doc = NULL;
      r = mongoc_cursor_next (cursor2, &doc);
      assert (!r);
      assert (!doc);

      mongoc_cursor_error(cursor2, &error);
      assert (error.domain == MONGOC_ERROR_CLIENT);
      assert (error.code == MONGOC_ERROR_CLIENT_IN_EXHAUST);

      mongoc_cursor_destroy (cursor2);
   }

   /* make sure writes fail as well */
   {
      BEGIN_IGNORE_DEPRECATIONS;
      r = mongoc_collection_insert_bulk (collection, MONGOC_INSERT_NONE,
                                         (const bson_t **)bptr, 10, wr, &error);
      END_IGNORE_DEPRECATIONS;

      assert (!r);
      assert (error.domain == MONGOC_ERROR_CLIENT);
      assert (error.code == MONGOC_ERROR_CLIENT_IN_EXHAUST);
   }

   /* we're still in exhaust.
    *
    * 1. check that we can create a new cursor, as long as we don't read from it
    * 2. fully exhaust the exhaust cursor
    * 3. make sure that we don't disconnect at destroy
    * 4. make sure we can read the cursor we made during the exhuast
    */
   {
      cursor2 = mongoc_collection_find (collection, MONGOC_QUERY_NONE, 0, 0, 0, &q,
                                        NULL, NULL);

      node = &client->cluster.nodes[cursor->hint - 1];
      stream = node->stream;

      for (i = 1; i < 10; i++) {
         r = mongoc_cursor_next (cursor, &doc);
         assert (r);
         assert (doc);
      }

      r = mongoc_cursor_next (cursor, &doc);
      assert (!r);
      assert (!doc);

      mongoc_cursor_destroy (cursor);

      assert (stream == node->stream);

      r = mongoc_cursor_next (cursor2, &doc);
      assert (r);
      assert (doc);
   }

   bson_destroy(&q);
   for (i = 0; i < 10; i++) {
      bson_destroy(&b[i]);
   }

   r = mongoc_collection_drop (collection, &error);
   assert (r);

   mongoc_write_concern_destroy (wr);
   mongoc_cursor_destroy (cursor2);
   mongoc_collection_destroy(collection);
   mongoc_client_destroy (client);
}


static void
test_server_status (void)
{
   mongoc_client_t *client;
   bson_error_t error;
   bson_iter_t iter;
   bson_t reply;
   bool r;

   client = test_framework_client_new (NULL);
   assert (client);

   r = mongoc_client_get_server_status (client, NULL, &reply, &error);
   assert (r);

   assert (bson_iter_init_find (&iter, &reply, "host"));
   assert (bson_iter_init_find (&iter, &reply, "version"));
   assert (bson_iter_init_find (&iter, &reply, "ok"));

   bson_destroy (&reply);

   mongoc_client_destroy (client);
}


static void
test_mongoc_client_ipv6 (void)
{
   mongoc_client_t *client;
   bson_error_t error;
   bson_iter_t iter;
   bson_t reply;
   bool r;

   client = mongoc_client_new ("mongodb://[::1]/");
   assert (client);

   r = mongoc_client_get_server_status (client, NULL, &reply, &error);
   assert (r);

   assert (bson_iter_init_find (&iter, &reply, "host"));
   assert (bson_iter_init_find (&iter, &reply, "version"));
   assert (bson_iter_init_find (&iter, &reply, "ok"));

   bson_destroy (&reply);

   mongoc_client_destroy (client);
}


void
test_client_install (TestSuite *suite)
{
	bool local;
	local = !getenv ("MONGOC_DISABLE_MOCK_SERVER");

	if (!local) {
		TestSuite_Add (suite, "/Client/wire_version", test_wire_version);
		TestSuite_Add (suite, "/Client/read_prefs", test_mongoc_client_read_prefs);
	}
	if (getenv ("MONGOC_CHECK_IPV6")) {
		/* try to validate ipv6 too */
		TestSuite_Add (suite, "/Client/ipv6", test_mongoc_client_ipv6);
	}
	//TestSuite_Add (suite, "/Client/authenticate", test_mongoc_client_authenticate);
	//TestSuite_Add (suite, "/Client/authenticate_failure", test_mongoc_client_authenticate_failure);
	TestSuite_Add (suite, "/Client/command", test_mongoc_client_command);
	//TestSuite_Add (suite, "/Client/command_secondary", test_mongoc_client_command_secondary);
	//TestSuite_Add (suite, "/Client/preselect", test_mongoc_client_preselect);
	//TestSuite_Add (suite, "/Client/unavailable_seeds", test_unavailable_seeds);
	//TestSuite_Add (suite, "/Client/rs_seeds_no_connect", test_rs_seeds_no_connect);
	//TestSuite_Add (suite, "/Client/rs_seeds_connect", test_rs_seeds_connect);
	//TestSuite_Add (suite, "/Client/rs_seeds_reconnect", test_rs_seeds_reconnect);
	//TestSuite_Add (suite, "/Client/mongos_seeds_no_connect", test_mongos_seeds_no_connect);
	//TestSuite_Add (suite, "/Client/mongos_seeds_connect", test_mongos_seeds_connect);
	//TestSuite_Add (suite, "/Client/mongos_seeds_reconnect", test_mongos_seeds_reconnect);
	//TestSuite_Add (suite, "/Client/recovering", test_recovering);
	//TestSuite_Add (suite, "/Client/exhaust_cursor", test_exhaust_cursor);
	//TestSuite_Add (suite, "/Client/server_status", test_server_status);
}
