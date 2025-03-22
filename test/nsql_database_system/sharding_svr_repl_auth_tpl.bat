
@echo off
cd /d %~dp0

set /a dir1 = 0
set /a dir2 = 0
set /a dir3 = 0

set data_root_dir=%1

set addr=%2
set port1=%3
set port2=%4
set port3=%5

set rep_name=%6
set svr_t=%7

set key_file=%8

set addr_port1=%addr%:%port1%
set addr_port2=%addr%:%port2%
set addr_port3=%addr%:%port3%

set data_dir1=%data_root_dir%\rep1\data
set data_dir2=%data_root_dir%\rep2\data
set data_dir3=%data_root_dir%\rep3\data

set log_file1=%data_root_dir%\rep1\log_file.txt
set log_file2=%data_root_dir%\rep2\log_file.txt
set log_file3=%data_root_dir%\rep3\log_file.txt

if not exist %data_dir1%\ set /a dir1=1
if not exist %data_dir2%\ set /a dir2=1
if not exist %data_dir3%\ set /a dir3=1

if %dir1% == 1 md %data_dir1%
if %dir2% == 1 md %data_dir2%
if %dir3% == 1 md %data_dir3%

if %dir1% == 1 goto lab_init
if %dir2% == 1 goto lab_init
if %dir3% == 1 goto lab_init

goto lab_end

:lab_init
	start mongod %svr_t% --dbpath %data_dir1% --logpath %log_file1% --replSet %rep_name% --bind_ip=%addr% --port %port1%
	start mongod %svr_t% --dbpath %data_dir2% --logpath %log_file2% --replSet %rep_name% --bind_ip=%addr% --port %port2%
	start mongod %svr_t% --dbpath %data_dir3% --logpath %log_file3% --replSet %rep_name% --bind_ip=%addr% --port %port3%

	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh ^
		--nodb ^
		--eval "var in_arg_addr_port1 = '%addr_port1%', in_arg_addr_port2 = '%addr_port2%', in_arg_addr_port3 = '%addr_port3%', rs_name = '%rep_name%';" ^
		init_mongodb_replset.js

	echo "wait 20 second"
	timeout /t 20 /nobreak > nul

	mongosh --nodb --eval "var arg_uri = 'mongodb://%addr_port1%/?replicaSet=%rep_name%&tls=false', arg_shutdown = true;" init_mongodb_auth.js

	mongosh --nodb --eval "var arg_uri = 'mongodb://%addr_port1%/?tls=false', arg_force = true;" shutdown_mongodb.js
	mongosh --nodb --eval "var arg_uri = 'mongodb://%addr_port2%/?tls=false', arg_force = true;" shutdown_mongodb.js
	mongosh --nodb --eval "var arg_uri = 'mongodb://%addr_port3%/?tls=false', arg_force = true;" shutdown_mongodb.js

	echo "wait 10 second"
	timeout /t 10 /nobreak > nul

:lab_end
	start mongod ^
		%svr_t% ^
		--dbpath %data_dir1% ^
		--logpath %log_file1% ^
		--replSet %rep_name% ^
		--bind_ip %addr% ^
		--port %port1% ^
		--logappend ^
		--keyFile %key_file% ^
		--auth

	start mongod ^
		%svr_t% ^
		--dbpath %data_dir2% ^
		--logpath %log_file2% ^
		--replSet %rep_name% ^
		--bind_ip %addr% ^
		--port %port2% ^
		--logappend ^
		--keyFile %key_file% ^
		--auth

	start mongod ^
		%svr_t% ^
		--dbpath %data_dir3% ^
		--logpath %log_file3% ^
		--replSet %rep_name% ^
		--bind_ip %addr% ^
		--port %port3% ^
		--logappend ^
		--keyFile %key_file% ^
		--auth
