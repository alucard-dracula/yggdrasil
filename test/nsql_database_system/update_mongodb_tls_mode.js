// update_mongodb_tls_mode.js

//var arg_uri = 'mongodb://127.0.0.1:10198/?tls=false', arg_shutdown = true, arg_shutdown_force = true, arg_tls_mode = 'requireSSL';

//const uri = "mongodb://127.0.0.1:10198/?tls=false";
var uri = arg_uri;
var need_shutdown = arg_shutdown;
var shutdown_force = arg_shutdown_force;
var tls_mode = arg_tls_mode;

var conn = null;
var admin_db = null;
var init_status = 0;

try
{
    conn = new Mongo(uri);
    init_status = 1;
}
catch (e)
{
    init_status = 2;
}
finally
{
}

printjson({ init_status: init_status });

switch (init_status)
{
    case 1:
        admin_db = conn.getDB("admin");
        try 
        {
            var rst = admin_db.adminCommand({setParameter: 1, sslMode: tls_mode}); 
            printjson(rst);
        }
        catch (e)
        {
             printjson({ Error: e });
        }
        finally 
        {
             if(need_shutdown) admin_db.adminCommand( { shutdown: 1, force: arg_shutdown_force } );
        }
        break;
    default:
        printjson({ Error: "mongodb server not ready" });
}

