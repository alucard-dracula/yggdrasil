
@echo off 

cd /d %~dp0

set data_dir=g:\mongodb_data\dev_db_single_svr_auth\data
set log_file=g:\mongodb_data\dev_db_single_svr_auth\data_log.txt

if exist %data_dir% goto :lab_start

:lab_create

	md g:\mongodb_data\dev_db_single_svr_auth\data

	start mongod ^
		--dbpath %data_dir% ^
		--logpath %log_file% ^
		--port 10198 ^
		--logappend
	
	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:10198/?tls=false', arg_shutdown = true;" init_mongodb_auth.js

	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

:lab_start
	start mongod ^
		--dbpath %data_dir% ^
		--logpath %log_file% ^
		--port 10198 ^
		--logappend ^
		--auth
