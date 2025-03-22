
#include <yggr_detail/c_version_support.h>

#include "mongoc-deprioritized-servers-private.h"

#include "mongoc-set-private.h"

// Dedicated non-zero value to avoid confusing "key is present with a NULL item"
// from "key is not present" (also NULL).
#define MONGOC_DEPRIORITIZED_SERVERS_ITEM_VALUE ((void *) 1)

struct _mongoc_deprioritized_servers_t {
   // Use server ID (uint32_t) as keys to identify deprioritized servers.
   mongoc_set_t *ids;
};

#if defined(YGGR_EX_C99_SUPPORTED)

mongoc_deprioritized_servers_t *
mongoc_deprioritized_servers_new (void)
{
   mongoc_deprioritized_servers_t *const ret = bson_malloc (sizeof (*ret));

   *ret = (mongoc_deprioritized_servers_t){
      .ids = mongoc_set_new (1u, NULL, NULL),
   };

   return ret;
}

#else

mongoc_deprioritized_servers_t *
mongoc_deprioritized_servers_new (void)
{
   //mongoc_deprioritized_servers_t *const ret = bson_malloc (sizeof (*ret));
   mongoc_deprioritized_servers_t *ret = bson_malloc (sizeof (*ret));
   if(!ret)
   {
	   return 0;
   }

   memset(ret, 0, sizeof(mongoc_deprioritized_servers_t));

   ret->ids = mongoc_set_new (1u, NULL, NULL);

   //*ret = (mongoc_deprioritized_servers_t){
   //   .ids = mongoc_set_new (1u, NULL, NULL),
   //};

   return ret;
}

#endif // #if defined(YGGR_EX_C99_SUPPORTED)

void
mongoc_deprioritized_servers_destroy (mongoc_deprioritized_servers_t *ds)
{
   if (!ds) {
      return;
   }

   mongoc_set_destroy (ds->ids);
   bson_free (ds);
}

void
mongoc_deprioritized_servers_add (mongoc_deprioritized_servers_t *ds,
                                  const mongoc_server_description_t *sd)
{
   BSON_ASSERT_PARAM (ds);
   BSON_ASSERT_PARAM (sd);

   mongoc_set_add (ds->ids,
                   mongoc_server_description_id (sd),
                   MONGOC_DEPRIORITIZED_SERVERS_ITEM_VALUE);
}

bool
mongoc_deprioritized_servers_contains (const mongoc_deprioritized_servers_t *ds,
                                       const mongoc_server_description_t *sd)
{
   BSON_ASSERT_PARAM (ds);
   BSON_ASSERT_PARAM (sd);

   return mongoc_set_get_const (ds->ids, mongoc_server_description_id (sd)) ==
          MONGOC_DEPRIORITIZED_SERVERS_ITEM_VALUE;
}
