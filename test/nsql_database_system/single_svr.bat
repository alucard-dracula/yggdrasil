
@echo off

cd /d %~dp0

set data_dir=g:\mongodb_data\dev_db_single_svr\data
set log_file=g:\mongodb_data\dev_db_single_svr\data_log.txt

if exist %data_dir% goto :lab_start

:lab_create
	md %data_dir%

:lab_start
	start mongod ^
		--dbpath %data_dir% ^
		--logpath %log_file% ^
		--port 10098 ^
		--logappend
