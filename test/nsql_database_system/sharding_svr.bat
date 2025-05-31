
@echo off
cd /d %~dp0

set /a dir_route = 0

set cfg_dir=g:\mongodb_data\dev_db_sharding_svr\config
set route_dir=g:\mongodb_data\dev_db_sharding_svr\route

set shd0_dir=g:\mongodb_data\dev_db_sharding_svr\shd0
set shd1_dir=g:\mongodb_data\dev_db_sharding_svr\shd1
set shd2_dir=g:\mongodb_data\dev_db_sharding_svr\shd2

if not exist %route_dir%\ set /a dir_route=1

if %dir_route% == 1 md %route_dir%

call sharding_svr_repl_tpl.bat %cfg_dir% 127.0.0.1 12101 12102 12103 rscfg0 --configsvr

call sharding_svr_repl_tpl.bat %shd0_dir% 127.0.0.1 12111 12112 12113 rsshd0 --shardsvr
call sharding_svr_repl_tpl.bat %shd1_dir% 127.0.0.1 12114 12115 12116 rsshd1 --shardsvr
call sharding_svr_repl_tpl.bat %shd2_dir% 127.0.0.1 12117 12118 12119 rsshd2 --shardsvr

echo "wait 10 second of replicset init..."
timeout /t 10 /nobreak > nul

start mongos ^
	--configdb "rscfg0/127.0.0.1:12101,127.0.0.1:12102,127.0.0.1:12103" ^
	--logpath %route_dir%\route_log.txt ^
	--bind_ip_all ^
	--port 12098 ^
	--logappend

if %dir_route% == 1 goto :lab_init

goto lab_end

set mongos_uri=mongodb://127.0.0.1:12098
set rsshd0_addr=rsshd0/127.0.0.1:12111,127.0.0.1:12112,127.0.0.1:12113
set rsshd1_addr=rsshd1/127.0.0.1:12114,127.0.0.1:12115,127.0.0.1:12116
set rsshd2_addr=rsshd2/127.0.0.1:12117,127.0.0.1:12118,127.0.0.1:12119

:lab_init
	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh ^
		--nodb ^
		--eval "var mongos_uri = '%mongos_uri%', rsshd0_addr = '%rsshd0_addr%', rsshd1_addr = '%rsshd1_addr%', rsshd2_addr = '%rsshd2_addr%';" ^
		init_mongodb_sharding.js

:lab_end
