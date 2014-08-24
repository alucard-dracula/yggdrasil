
if not	exist e:\mongodb_data\replicset\rep1\data md e:\mongodb_data\replicset\rep1\data
if not exist e:\mongodb_data\replicset\rep2\data md e:\mongodb_data\replicset\rep2\data
if not exist e:\mongodb_data\replicset\rep3\data md e:\mongodb_data\replicset\rep3\data

start mongod --dbpath e:\mongodb_data\replicset\rep1\data --logpath e:\mongodb_data\replicset\rep1\rep_log.txt --replSet rs0 --port 10099
start mongod --dbpath e:\mongodb_data\replicset\rep2\data --logpath e:\mongodb_data\replicset\rep2\rep_log.txt --replSet rs0 --port 10100
start mongod --dbpath e:\mongodb_data\replicset\rep3\data --logpath e:\mongodb_data\replicset\rep3\rep_log.txt --replSet rs0 --port 10101


:start mongod --dbpath e:\mongodb_data\replicset\rep1\data --logpath e:\mongodb_data\replicset\rep1\rep_log.txt --replSet rs0 --port 10099 --bind_ip 192.168.1.103
:start mongod --dbpath e:\mongodb_data\replicset\rep2\data --logpath e:\mongodb_data\replicset\rep2\rep_log.txt --replSet rs0 --port 10100 --bind_ip 192.168.1.103
:start mongod --dbpath e:\mongodb_data\replicset\rep3\data --logpath e:\mongodb_data\replicset\rep3\rep_log.txt --replSet rs0 --port 10101 --bind_ip 192.168.1.103
