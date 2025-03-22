// init_mongodb_replset.js

// var in_arg_addr_port1 = '127.0.0.1:11098', in_arg_addr_port2 = '127.0.0.1:11099', in_arg_addr_port3 = '127.0.0.1:11100', rs_name = 'rs0';

//const uri_repl = "mongodb://127.0.0.1:11098/?replicaSet=rs0";
const uri_repl = "mongodb://" + in_arg_addr_port1 + "/?replicaSet=" + rs_name;
//const uri = "mongodb://127.0.0.1:11098";
const uri = "mongodb://" + in_arg_addr_port1;
//const replset_cfg = { _id: 'rs0', members: [{ _id: 0, host: '127.0.0.1:11098' }, { _id: 1, host: '127.0.0.1:11099' }, { _id: 2, host: '127.0.0.1:11100' }] };
const replset_cfg = { _id: rs_name, members: [{ _id: 0, host: in_arg_addr_port1 }, { _id: 1, host: in_arg_addr_port2 }, { _id: 2, host: in_arg_addr_port3 }] };  

printjson(replset_cfg);

var conn = null;
var admin_db = null;
var init_status = 0;

try
{
    conn = new Mongo(uri_repl);
    init_status = 1;
}
catch (e)
{
    try
    {
        conn = new Mongo(uri);
    }
    catch (e)
    {
        init_status = 2;
    }
    finally
    {
    }
}
finally
{
}

printjson({ init_status: init_status });

switch (init_status)
{
    case 0:
        admin_db = conn.getDB("admin");
        try 
        {
            admin_db.runCommand({ replSetInitiate: replset_cfg });
            printjson(admin_db.adminCommand({ replSetGetStatus: 1 }));
        }
        catch (e)
        {
             printjson({ Error: "mongodb server not ready" });
        }
        finally 
        {
        }
        break;
    case 2:
        printjson({ Error: "mongodb server not ready" });
        break;
    default:
        admin_db = conn.getDB("admin");
        printjson(admin_db.adminCommand({ replSetGetStatus: 1 }));
}

