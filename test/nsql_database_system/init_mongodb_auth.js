// init_mongodb_auth.js

//var arg_uri = 'mongodb://127.0.0.1:10198/?tls=false', arg_shutdown = true;

//const uri = "mongodb://127.0.0.1:10198/?tls=false";
var uri = arg_uri;
var need_shutdown = arg_shutdown;
 
const auth_cfg = 
    { 
        createUser: 'xy', 
        pwd: '123456abc', 
        roles: 
        [ 
            { role: 'clusterAdmin', db: 'admin' },
            { role: 'dbAdminAnyDatabase', db: 'admin' },  
            { role: 'readWriteAnyDatabase', db: 'admin' },
            { role: 'userAdminAnyDatabase', db: 'admin' },
            { role: 'clusterMonitor', db: 'admin' },
        ] 
    };   

printjson(auth_cfg);

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
            var rst = admin_db.runCommand(auth_cfg);
            printjson(rst);
        }
        catch (e)
        {
             printjson({ Error: e });
        }
        finally 
        {
             if(need_shutdown) admin_db.adminCommand( { shutdown: 1 } );
        }
        break;
    default:
        printjson({ Error: "mongodb server not ready" });
}

