
@echo off
cd /d %~dp0

set /a dir1 = 0
set /a dir2 = 0
set /a dir3 = 0

set data_dir1=g:\mongodb_data\dev_db_replica_set_svr_auth\rep1\data
set data_dir2=g:\mongodb_data\dev_db_replica_set_svr_auth\rep2\data
set data_dir3=g:\mongodb_data\dev_db_replica_set_svr_auth\rep3\data

set log_file1=g:\mongodb_data\dev_db_replica_set_svr_auth\rep1\rep_log.txt
set log_file2=g:\mongodb_data\dev_db_replica_set_svr_auth\rep2\rep_log.txt
set log_file3=g:\mongodb_data\dev_db_replica_set_svr_auth\rep3\rep_log.txt

set keyfile=g:\mongodb_data\keyfile\replica_set_key.rsk

if not exist %data_dir1% set /a dir1 = 1
if not exist %data_dir2% set /a dir2 = 1
if not exist %data_dir3% set /a dir3 = 1

if %dir1% == 1 md %data_dir1%
if %dir2% == 1 md %data_dir2%
if %dir3% == 1 md %data_dir3%

if %dir1% == 1 goto :lab_init
if %dir2% == 1 goto :lab_init
if %dir3% == 1 goto :lab_init

goto :lab_end

:lab_init
	start mongod --dbpath %data_dir1% --logpath %log_file1% --replSet rs0 --port 11198 --logappend
	start mongod --dbpath %data_dir2% --logpath %log_file2% --replSet rs0 --port 11199 --logappend
	start mongod --dbpath %data_dir3% --logpath %log_file3% --replSet rs0 --port 11200 --logappend

	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh ^
		--nodb ^
		--eval "var in_arg_addr_port1 = '127.0.0.1:11198', in_arg_addr_port2 = '127.0.0.1:11199', in_arg_addr_port3 = '127.0.0.1:11200', rs_name = 'rs0';" ^
		init_mongodb_replset.js

	echo "wait 10 second"
	timeout /t 10 /nobreak > nul

	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11198/?replicaSet=rs0&tls=false', arg_shutdown = true;" init_mongodb_auth.js

	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11198/?tls=false', arg_force = true;" shutdown_mongodb.js
	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11199/?tls=false', arg_force = true;" shutdown_mongodb.js
	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11200/?tls=false', arg_force = true;" shutdown_mongodb.js

	echo "wait 10 second"
	timeout /t 10 /nobreak > nul

:lab_end
	start mongod ^
		--dbpath %data_dir1% ^
		--logpath %log_file1% ^
		--replSet rs0 ^
		--port 11198 ^
		--keyFile %keyfile% ^
		--logappend ^
		--auth

	start mongod ^
		--dbpath %data_dir2% ^
		--logpath %log_file2% ^
		--replSet rs0 ^
		--port 11199 ^
		--keyFile %keyfile% ^
		--logappend ^
		--auth

	start mongod ^
		--dbpath %data_dir3% ^
		--logpath %log_file3% ^
		--replSet rs0 ^
		--port 11200 ^
		--keyFile %keyfile% ^
		--logappend ^
		--auth