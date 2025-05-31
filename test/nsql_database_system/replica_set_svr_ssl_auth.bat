
@echo off
cd /d %~dp0

set /a dir1 = 0
set /a dir2 = 0
set /a dir3 = 0

set data_dir1=g:\mongodb_data\dev_db_replica_set_svr_ssl_auth\rep1\data
set data_dir2=g:\mongodb_data\dev_db_replica_set_svr_ssl_auth\rep2\data
set data_dir3=g:\mongodb_data\dev_db_replica_set_svr_ssl_auth\rep3\data

set log_file1=g:\mongodb_data\dev_db_replica_set_svr_ssl_auth\rep1\rep_log.txt
set log_file2=g:\mongodb_data\dev_db_replica_set_svr_ssl_auth\rep2\rep_log.txt
set log_file3=g:\mongodb_data\dev_db_replica_set_svr_ssl_auth\rep3\rep_log.txt

set svr_pem=g:\mongodb_data\cert\server.pem
set ca_pem=g:\mongodb_data\cert\ca.pem

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
	start mongod --dbpath %data_dir1% --logpath %log_file1% --replSet rs0 --port 11398
	start mongod --dbpath %data_dir2% --logpath %log_file2% --replSet rs0 --port 11399
	start mongod --dbpath %data_dir3% --logpath %log_file3% --replSet rs0 --port 11400

	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh ^
		--nodb ^
		--eval "var in_arg_addr_port1 = '127.0.0.1:11398', in_arg_addr_port2 = '127.0.0.1:11399', in_arg_addr_port3 = '127.0.0.1:11400', rs_name = 'rs0';" ^
		init_mongodb_replset.js

	echo "wait 20 second"
	timeout /t 20 /nobreak > nul

	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11398/?replicaSet=rs0&tls=false', arg_shutdown = true;" init_mongodb_auth.js

	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11398/?tls=false', arg_force = true;" shutdown_mongodb.js
	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11399/?tls=false', arg_force = true;" shutdown_mongodb.js
	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11400/?tls=false', arg_force = true;" shutdown_mongodb.js

	echo "wait 10 second"
	timeout /t 10 /nobreak > nul

:lab_end
	start mongod ^
		--dbpath %data_dir1% ^
		--logpath %log_file1% ^
		--replSet rs0 ^
		--bind_ip_all ^
		--port 11398 ^
		--logappend ^
		--tlsMode requireTLS ^
		--tlsCertificateKeyFile %svr_pem% ^
		--tlsCAFile %ca_pem% ^
		--tlsClusterFile %svr_pem% ^
		--clusterAuthMode x509 ^
		--auth

	start mongod ^
		--dbpath %data_dir2% ^
		--logpath %log_file2% ^
		--replSet rs0 ^
		--bind_ip_all ^
		--port 11399 ^
		--logappend ^
		--tlsMode requireTLS ^
		--tlsCertificateKeyFile %svr_pem% ^
		--tlsCAFile %ca_pem% ^
		--tlsClusterFile %svr_pem% ^
		--clusterAuthMode x509 ^
		--auth

	start mongod ^
		--dbpath %data_dir3% ^
		--logpath %log_file3% ^
		--replSet rs0 ^
		--bind_ip_all ^
		--port 11400 ^
		--logappend ^
		--tlsMode requireTLS ^
		--tlsCertificateKeyFile %svr_pem% ^
		--tlsCAFile %ca_pem% ^
		--tlsClusterFile %svr_pem% ^
		--clusterAuthMode x509 ^
		--auth
