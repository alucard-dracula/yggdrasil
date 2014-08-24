replicset:
run repset.bat
cfg = { _id:'rs0', members:[{_id:0,host:'127.0.0.1:10099'},{_id:1,host:'127.0.0.1:10100'},{_id:2, host:'127.0.0.1:10101'}] }
rs.initiate(cfg);

shareding:

run shareding.bat

use admin
db.runCommand({addshard:'192.168.1.103:10130', allowLocal:true});
db.runCommand({addshard:'192.168.1.103:10131', allowLocal:true});
db.runCommand({addshard:'192.168.1.103:10132', allowLocal:true});