
if not	exist e:\mongodb_data\shareding\config\data md e:\mongodb_data\shareding\config\data
if not	exist e:\mongodb_data\shareding\route md e:\mongodb_data\shareding\route
if not	exist e:\mongodb_data\shareding\shd1\data md e:\mongodb_data\shareding\shd1\data
if not exist e:\mongodb_data\shareding\shd2\data md e:\mongodb_data\shareding\shd2\data
if not exist e:\mongodb_data\shareding\shd3\data md e:\mongodb_data\shareding\shd3\data

start mongod --dbpath e:\mongodb_data\shareding\config\data --logpath e:\mongodb_data\shareding\config\cfg_log.txt --port 10120
start mongos --configdb 192.168.1.103:10120 --logpath e:\mongodb_data\shareding\route\route_log.txt --port 10125

start mongod --dbpath e:\mongodb_data\shareding\shd1\data --logpath e:\mongodb_data\shareding\shd1\shd_log.txt --port 10130
start mongod --dbpath e:\mongodb_data\shareding\shd2\data --logpath e:\mongodb_data\shareding\shd2\shd_log.txt --port 10131
start mongod --dbpath e:\mongodb_data\shareding\shd3\data --logpath e:\mongodb_data\shareding\shd3\shd_log.txt --port 10132
