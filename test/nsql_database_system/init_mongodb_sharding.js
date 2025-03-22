// init_mongodb_replset.js

//  var mongos_uri = 'mongod://127.0.0.1:12098', 
//      rsshd0_addr = 'rsshd0/127.0.0.1:12111,127.0.0.1:12112,127.0.0.1:12113', 
//      rsshd1_addr = 'rsshd1/127.0.0.1:12114,127.0.0.1:12115,127.0.0.1:12116', 
//      rsshd2_addr = 'rsshd2/127.0.0.1:12117,127.0.0.1:12118,127.0.0.1:12119';

//const uri = "mongodb://127.0.0.1:11098";

const uri = mongos_uri;
printjson({ 'uri': uri });

var conn = null;
var admin_db = null;
var init_status = 0;

printjson({ 'uri': uri, 'rsshd0_addr': rsshd0_addr, 'rsshd1_addr': rsshd1_addr, 'rsshd2_addr': rsshd2_addr });

try
{
    conn = new Mongo(uri);
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
            admin_db.runCommand({ addShard: rsshd0_addr, name: "rshd0" });
            admin_db.runCommand({ addShard: rsshd1_addr, name: "rshd1" });
            admin_db.runCommand({ addShard: rsshd2_addr, name: "rshd2" });
            admin_db.printShardingStatus();
        }
        catch (e)
        {
             printjson({ Error: "mongodb server not ready" });
             printjson({ Error: e });
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
        admin_db.printShardingStatus();
}

