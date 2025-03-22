
:rem call sharding_svr_repl_tpl.bat g:\mongodb_data\dev_db_sharding_svr\config 127.0.0.1 12101 12102 12103 rscfg0 --configsvr

@echo off
cd /d %~dp0

set /a dir1=0
set /a dir2=0
set /a dir3=0

set data_root_dir=%1

set addr=%2
set port1=%3
set port2=%4
set port3=%5

set rep_name=%6
set svr_t=%7

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

start mongod %svr_t% --dbpath %data_dir1% --logpath %log_file1% --replSet %rep_name% --bind_ip=%addr% --port %port1% --logappend
start mongod %svr_t% --dbpath %data_dir2% --logpath %log_file2% --replSet %rep_name% --bind_ip=%addr% --port %port2% --logappend
start mongod %svr_t% --dbpath %data_dir3% --logpath %log_file3% --replSet %rep_name% --bind_ip=%addr% --port %port3% --logappend

if %dir1% == 1 goto :lab_init
if %dir2% == 1 goto :lab_init
if %dir3% == 1 goto :lab_init

goto lab_end

:lab_init
	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh ^
		--nodb ^
		--eval "var in_arg_addr_port1 = '%addr_port1%', in_arg_addr_port2 = '%addr_port2%', in_arg_addr_port3 = '%addr_port3%', rs_name = '%rep_name%';" ^
		init_mongodb_replset.js

:lab_end
