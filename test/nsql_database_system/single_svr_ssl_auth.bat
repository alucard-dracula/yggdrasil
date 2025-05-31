

@echo off 

cd /d %~dp0

set data_dir=g:\mongodb_data\dev_db_single_svr_ssl_auth\data
set log_file=g:\mongodb_data\dev_db_single_svr_ssl_auth\data_log.txt

set svr_pem=g:\mongodb_data\cert\server.pem
set ca_pem=g:\mongodb_data\cert\ca.pem

if exist %data_dir% goto :lab_start

:lab_create

	md %data_dir%

	start mongod --dbpath %data_dir% --logpath %log_file% --port 10398

	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:10398/?tls=false', arg_shutdown = true;" init_mongodb_auth.js

	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

:lab_start

	start mongod ^
		--dbpath %data_dir% ^
		--logpath %log_file% ^
		--bind_ip_all ^
		--port 10398 ^
		--logappend ^
		--tlsMode requireTLS ^
		--tlsCertificateKeyFile %svr_pem% ^
		--tlsCAFile %ca_pem%
