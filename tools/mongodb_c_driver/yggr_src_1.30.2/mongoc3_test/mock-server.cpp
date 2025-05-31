/*
 * Copyright 2013 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <bson.h>

#include "mock-server.h"
#include "mongoc-buffer-private.h"
#include "mongoc-stream-socket.h"
#include "mongoc-socket.h"
#include "mongoc-thread-private.h"
#include "mongoc-trace.h"


#ifdef _WIN32
# define strcasecmp _stricmp
#endif


struct _mock_server_t
{
   mock_server_handler_t  handler;
   void                  *handler_data;
   bool                   verbose;

   mongoc_thread_t        main_thread;
   mongoc_cond_t          cond;
   mongoc_cond_t          stopped_condition;
   mongoc_mutex_t         mutex;
   bool                   using_main_thread;

   const char            *address;
   mongoc_array_t         streams;  /* mongoc_stream_t pointers */

   uint16_t               port;
   mongoc_socket_t       *sock;

   int                    last_response_id;

   bool                   isMaster;
   bool                   isSecondary;
   int                    minWireVersion;
   int                    maxWireVersion;
   int                    maxBsonObjectSize;
   int                    maxMessageSizeBytes;

   const char                  *setName;
   const mongoc_host_list_t    *hosts;
   const bson_t                *tags;

   bool stopping;
   bool stopped;
};


void
mock_server_reply_simple (mock_server_t        *server,
                          mongoc_stream_t      *client,
                          const mongoc_rpc_t   *request,
                          mongoc_reply_flags_t  flags,
                          const bson_t         *doc)
{
   mongoc_iovec_t *iov;
   mongoc_array_t ar;
   mongoc_rpc_t r = {{ 0 }};
   size_t expected = 0;
   size_t n_written;
   int iovcnt;
   int i;

   BSON_ASSERT (server);
   BSON_ASSERT (request);
   BSON_ASSERT (client);
   BSON_ASSERT (doc);

   _mongoc_array_init (&ar, sizeof (mongoc_iovec_t));

   r.reply.msg_len = 0;
   r.reply.request_id = ++server->last_response_id;
   r.reply.response_to = request->header.request_id;
   r.reply.opcode = MONGOC_OPCODE_REPLY;
   r.reply.flags = 0;
   r.reply.cursor_id = 0;
   r.reply.start_from = 0;
   r.reply.n_returned = 1;
   r.reply.documents = bson_get_data (doc);
   r.reply.documents_len = doc->len;

   _mongoc_rpc_gather (&r, &ar);
   _mongoc_rpc_swab_to_le (&r);

   iov = reinterpret_cast<mongoc_iovec_t*>(ar.data);
   iovcnt = (int)ar.len;

   for (i = 0; i < iovcnt; i++) {
      expected += iov[i].iov_len;
   }

   n_written = mongoc_stream_writev (client, iov, iovcnt, -1);

   assert (n_written == expected);

   _mongoc_array_destroy (&ar);
}


static bool
handle_ping (mock_server_t   *server,
             mongoc_stream_t *client,
             mongoc_rpc_t    *rpc,
             const bson_t    *doc)
{
   bson_t reply = BSON_INITIALIZER;

   bson_append_int32 (&reply, "ok", 2, 1);
   mock_server_reply_simple (server, client, rpc, MONGOC_REPLY_NONE, &reply);
   bson_destroy (&reply);

   return true;
}


static bool
handle_ismaster (mock_server_t   *server,
                 mongoc_stream_t *client,
                 mongoc_rpc_t    *rpc,
                 const bson_t    *doc)
{
   bson_t reply_doc = BSON_INITIALIZER;
   time_t now = time (NULL);
   bson_t hosts_array;
   const char *hosts_array_key;
   char buf[32];
   uint32_t i;
   const mongoc_host_list_t *host;

   BSON_ASSERT (server);
   BSON_ASSERT (client);
   BSON_ASSERT (rpc);
   BSON_ASSERT (doc);

   bson_append_bool (&reply_doc, "ismaster", -1, server->isMaster);
   if (!server->isMaster) {
      bson_append_bool (&reply_doc, "secondary", -1, server->isSecondary);
   }

   bson_append_int32 (&reply_doc, "maxBsonObjectSize", -1,
                      server->maxBsonObjectSize);
   bson_append_int32 (&reply_doc, "maxMessageSizeBytes", -1,
                      server->maxMessageSizeBytes);
   bson_append_int32 (&reply_doc, "minWireVersion", -1,
                      server->minWireVersion);
   bson_append_int32 (&reply_doc, "maxWireVersion", -1,
                      server->maxWireVersion);
   bson_append_double (&reply_doc, "ok", -1, 1.0);
   bson_append_time_t (&reply_doc, "localtime", -1, now);
   if (server->setName) {
      BSON_APPEND_UTF8 (&reply_doc, "setName", server->setName);
      host = server->hosts;
      i = 0;
      bson_append_array_begin (&reply_doc, "hosts", -1, &hosts_array);

      while (host) {
         bson_uint32_to_string (i, &hosts_array_key, buf, sizeof buf);
         BSON_APPEND_UTF8 (&hosts_array, hosts_array_key, host->host_and_port);
         i++;
         host = host->next;
      }
      
      bson_append_array_end (&reply_doc, &hosts_array);
   }

   if (server->tags) {
      BSON_APPEND_DOCUMENT (&reply_doc, "tags", server->tags);
   }

   mock_server_reply_simple (server, client, rpc, MONGOC_REPLY_NONE, &reply_doc);

   bson_destroy (&reply_doc);

   return true;
}


static bool
handle_command (mock_server_t   *server,
                mongoc_stream_t *client,
                mongoc_rpc_t    *rpc)
{
   int32_t len;
   bool ret = false;
   bson_iter_t iter;
   const char *key;
   bson_t doc;

   BSON_ASSERT (rpc);

   if (rpc->header.opcode != MONGOC_OPCODE_QUERY) {
      return false;
   }

   memcpy (&len, rpc->query.query, 4);
   len = BSON_UINT32_FROM_LE (len);
   if (!bson_init_static (&doc, rpc->query.query, len)) {
      return false;
   }

   if (!bson_iter_init (&iter, &doc) || !bson_iter_next (&iter)) {
      return false;
   }

   key = bson_iter_key (&iter);

   if (!strcasecmp (key, "ismaster")) {
      ret = handle_ismaster (server, client, rpc, &doc);
   } else if (!strcasecmp (key, "ping")) {
      ret = handle_ping (server, client, rpc, &doc);
   }

   bson_destroy (&doc);

   return ret;
}


static void *
mock_server_worker (void *data)
{
   size_t streams_buffer_pos;
   mongoc_buffer_t buffer;
   mongoc_stream_t *stream;
   mock_server_t *server;
   mongoc_rpc_t rpc;
   bson_error_t error;
   int32_t msg_len;
   void **closure = reinterpret_cast<void**>(data);

   ENTRY;

   BSON_ASSERT(closure);

   server = reinterpret_cast<mock_server_t*>(closure[0]);
   stream = reinterpret_cast<mongoc_stream_t*>(closure[1]);

   mongoc_mutex_lock (&server->mutex);
   streams_buffer_pos = server->streams.len;
   _mongoc_array_append_val (&server->streams, stream);
   mongoc_mutex_unlock (&server->mutex);

   _mongoc_buffer_init(&buffer, NULL, 0, NULL, NULL);

again:
   if (_mongoc_buffer_fill (&buffer, stream, 4, -1, &error) == -1) {
      if (mock_server_verbose (server)) {
         MONGOC_WARNING ("%s():%d: %s", __FUNCTION__, __LINE__, error.message);
      }

      GOTO (failure);
   }

   assert (buffer.len >= 4);

   memcpy (&msg_len, buffer.data + buffer.off, 4);
   msg_len = BSON_UINT32_FROM_LE (msg_len);

   if (msg_len < 16) {
      MONGOC_WARNING ("No data");
      GOTO (failure);
   }

   if (_mongoc_buffer_fill (&buffer, stream, msg_len, -1, &error) == -1) {
      MONGOC_WARNING ("%s():%d: %s", __FUNCTION__, __LINE__, error.message);
      GOTO (failure);
   }

   assert (buffer.len >= (unsigned)msg_len);

   DUMP_BYTES (buffer, buffer.data + buffer.off, buffer.len);

   if (!_mongoc_rpc_scatter(&rpc, buffer.data + buffer.off, msg_len)) {
      MONGOC_WARNING ("%s():%d: %s", __FUNCTION__, __LINE__, "Failed to scatter");
      GOTO (failure);
   }

   _mongoc_rpc_swab_from_le(&rpc);

   if (!handle_command(server, stream, &rpc)) {
      server->handler(server, stream, &rpc, server->handler_data);
   }

   memmove (buffer.data, buffer.data + buffer.off + msg_len,
            buffer.len - msg_len);
   buffer.off = 0;
   buffer.len -= msg_len;

   GOTO (again);

failure:
   mongoc_mutex_lock (&server->mutex);
   if (!server->stopping) {
      _mongoc_array_index (&server->streams,
                           mongoc_stream_t *,
                           streams_buffer_pos) = NULL;
   }

   mongoc_mutex_unlock (&server->mutex);

   mongoc_stream_close (stream);
   mongoc_stream_destroy (stream);
   bson_free(closure);
   _mongoc_buffer_destroy (&buffer);

   RETURN (NULL);
}


static void
dummy_handler (mock_server_t   *server,
               mongoc_stream_t *stream,
               mongoc_rpc_t    *rpc,
               void            *user_data)
{
}


mock_server_t *
mock_server_new_rs (const char                  *address,
                    uint16_t                     port,
                    mock_server_handler_t        handler,
                    void                        *handler_data,
                    const char                  *setName,
                    bool                         isMaster,
                    bool                         isSecondary,
                    const mongoc_host_list_t    *hosts,
                    const bson_t                *tags)
{
   mock_server_t *server;

   if (!address) {
      address = "127.0.0.1";
   }

   if (!port) {
      port = 27017;
   }

   server = reinterpret_cast<mock_server_t*>(bson_malloc0(sizeof *server));
   server->handler = handler ? handler : dummy_handler;
   server->handler_data = handler_data;
   server->verbose = true;
   server->sock = NULL;
   _mongoc_array_init (&server->streams, sizeof (mongoc_stream_t *));
   server->address = address;
   server->port = port;

   server->minWireVersion = 0;
   server->maxWireVersion = 0;
   server->isMaster = isMaster;
   server->isSecondary = isSecondary;
   server->maxBsonObjectSize = 16777216;
   server->maxMessageSizeBytes = 48000000;

   server->setName = setName;
   server->hosts = hosts;
   server->tags = tags;

   mongoc_mutex_init (&server->mutex);
   mongoc_cond_init (&server->cond);
   mongoc_cond_init (&server->stopped_condition);

   server->stopping = false;
   server->stopped = false;

   return server;
}

mock_server_t *
mock_server_new (const char            *address,
                 uint16_t               port,
                 mock_server_handler_t  handler,
                 void                  *handler_data)
{
   return mock_server_new_rs (address, port, handler, handler_data,
                              NULL, true, false, NULL, NULL);
}


int
mock_server_run (mock_server_t *server)
{
   struct sockaddr_in saddr;
   mongoc_stream_t *stream;
   mongoc_thread_t thread;
   mongoc_socket_t *ssock;
   mongoc_socket_t *csock;
   void **closure;
   int optval;

   bson_return_val_if_fail (server, -1);
   bson_return_val_if_fail (!server->sock, -1);

   MONGOC_INFO ("Starting mock server on port %d.", server->port);

   ssock = mongoc_socket_new (AF_INET, SOCK_STREAM, 0);
   if (!ssock) {
      perror("Failed to create socket.");
      return -1;
   }

   optval = 1;
   mongoc_socket_setsockopt (ssock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

   memset (&saddr, 0, sizeof saddr);

   saddr.sin_family = AF_INET;
   saddr.sin_port = htons(server->port);
   /*
    * TODO: Parse server->address.
    */
   saddr.sin_addr.s_addr = htonl (INADDR_ANY);

   if (-1 == mongoc_socket_bind (ssock, (struct sockaddr *)&saddr, sizeof saddr)) {
      perror("Failed to bind socket");
      return -1;
   }

   if (-1 == mongoc_socket_listen (ssock, 10)) {
      perror("Failed to put socket into listen mode");
      return 3;
   }

   server->sock = ssock;

   mongoc_mutex_lock (&server->mutex);
   mongoc_cond_signal (&server->cond);
   mongoc_mutex_unlock (&server->mutex);

   for (;;) {
      mongoc_mutex_lock (&server->mutex);
      if (server->stopping) {
         mongoc_mutex_unlock (&server->mutex);
         break;
      }
      mongoc_mutex_unlock (&server->mutex);

      csock = mongoc_socket_accept (server->sock,
                                    bson_get_monotonic_time () + 1000 * 1000);
      if (csock) {
         stream = mongoc_stream_socket_new (csock);
         closure = reinterpret_cast<void**>(bson_malloc0 (sizeof (void *) * 2));
         closure[0] = server;
         closure[1] = stream;

         mongoc_thread_create (&thread, mock_server_worker, closure);
      }
   }

   mongoc_mutex_lock (&server->mutex);
   mongoc_socket_destroy (server->sock);
   server->sock = NULL;
   server->stopped = true;
   mongoc_cond_signal (&server->stopped_condition);
   mongoc_mutex_unlock (&server->mutex);

   return 0;
}


static void *
main_thread (void *data)
{
   mock_server_t *server = reinterpret_cast<mock_server_t*>(data);

   mock_server_run (server);

   return NULL;
}


void
mock_server_run_in_thread (mock_server_t *server)
{
   BSON_ASSERT (server);

   server->using_main_thread = true;

   mongoc_mutex_lock (&server->mutex);
   mongoc_thread_create (&server->main_thread, main_thread, server);
   mongoc_cond_wait (&server->cond, &server->mutex);
   mongoc_mutex_unlock (&server->mutex);
}


void
mock_server_set_verbose (mock_server_t *server,
                         bool verbose)
{
   mongoc_mutex_lock (&server->mutex);
   server->verbose = verbose;
   mongoc_mutex_unlock (&server->mutex);
}


bool
mock_server_verbose (mock_server_t *server)
{
   bool verbose;

   mongoc_mutex_lock (&server->mutex);
   verbose = server->verbose;
   mongoc_mutex_unlock (&server->mutex);

   return verbose;
}


void
mock_server_quit (mock_server_t *server)
{
   size_t i;
   mongoc_stream_t *stream;

   bson_return_if_fail(server);

   mongoc_mutex_lock (&server->mutex);
   assert (!server->stopping);
   assert (!server->stopped);
   server->stopping = true;
   assert (0 == mongoc_socket_close (server->sock));  /* success */
   for (i = 0; i < server->streams.len; i++) {
      stream = _mongoc_array_index (&server->streams, mongoc_stream_t *, i);
      if (stream) {
         assert (0 == mongoc_stream_close (stream));  /* success */
      }
   }
   while (! server->stopped) {
      mongoc_cond_wait (&server->stopped_condition, &server->mutex);
   }
   mongoc_mutex_unlock (&server->mutex);
}


void
mock_server_destroy (mock_server_t *server)
{
   /* TODO: we can't actually destroy these if workers are still around.  Make
    * this operational again after fixing that up */
   return;
   if (server) {
      _mongoc_array_destroy (&server->streams);
      mongoc_cond_destroy (&server->cond);
      mongoc_cond_destroy (&server->stopped_condition);
      mongoc_mutex_destroy (&server->mutex);
      bson_free(server);
   }
}


void
mock_server_set_wire_version (mock_server_t *server,
                              int32_t   min_wire_version,
                              int32_t   max_wire_version)
{
   BSON_ASSERT (server);

   server->minWireVersion = min_wire_version;
   server->maxWireVersion = max_wire_version;
}
