// shutdown_mongodb.js

//var arg_uri = 'mongodb://127.0.0.1:10198/?tls=false', arg_force = true;
//const uri = "mongodb://127.0.0.1:10198/?tls=false";
var uri = arg_uri;
var bforce = arg_force;
 
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
            admin_db.adminCommand( { shutdown: 1, force: bforce } );
        }
        catch (e)
        {
             printjson({ Error: e });
        }
        finally 
        {
        }
        break;
    default:
        printjson({ Error: "mongodb server not ready" });
}

