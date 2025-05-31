
@echo off
cd /d %~dp0

set /a dir1 = 0
set /a dir2 = 0
set /a dir3 = 0

set data_dir1=g:\mongodb_data\dev_db_replica_set_lansvr\rep1\data
set data_dir2=g:\mongodb_data\dev_db_replica_set_lansvr\rep2\data
set data_dir3=g:\mongodb_data\dev_db_replica_set_lansvr\rep3\data

set log_file1=g:\mongodb_data\dev_db_replica_set_lansvr\rep1\rep_log.txt
set log_file2=g:\mongodb_data\dev_db_replica_set_lansvr\rep2\rep_log.txt
set log_file3=g:\mongodb_data\dev_db_replica_set_lansvr\rep3\rep_log.txt

if not exist %data_dir1% set /a dir1 = 1
if not exist %data_dir2% set /a dir2 = 1
if not exist %data_dir3% set /a dir3 = 1

if %dir1% == 1 md %data_dir1%
if %dir2% == 1 md %data_dir2%
if %dir3% == 1 md %data_dir3%

start mongod ^
	--dbpath %data_dir1% ^
	--logpath %log_file1% ^
	--logappend ^
	--replSet rs0 ^
	--bind_ip_all ^
	--port 11498

start mongod ^
	--dbpath %data_dir2% ^
	--logpath %log_file2% ^
	--logappend ^
	--replSet rs0 ^
	--bind_ip_all ^
	--port 11499

start mongod ^
	--dbpath %data_dir3% ^
	--logpath %log_file3% ^
	--logappend ^
	--replSet rs0 ^
	--bind_ip_all ^
	--port 11500

if %dir1% == 1 goto :lab_init
if %dir2% == 1 goto :lab_init
if %dir3% == 1 goto :lab_init

goto :lab_end

:lab_init
	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh ^
		--nodb ^
		--eval "var in_arg_addr_port1 = '192.168.1.101:11498', in_arg_addr_port2 = '192.168.1.101:11499', in_arg_addr_port3 = '192.168.1.101:11500', rs_name = 'rs0';" ^
		init_mongodb_replset.js

:lab_end

