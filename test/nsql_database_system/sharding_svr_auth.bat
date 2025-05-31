@echo off
cd /d %~dp0

set keyfile=g:/mongodb_data/keyfile/replica_set_key.rsk

set /a dir_route = 0

set cfg_dir=g:\mongodb_data\dev_db_sharding_svr_auth\config

set route_dir=g:\mongodb_data\dev_db_sharding_svr_auth\route

set shd0_dir=g:\mongodb_data\dev_db_sharding_svr_auth\shd0
set shd1_dir=g:\mongodb_data\dev_db_sharding_svr_auth\shd1
set shd2_dir=g:\mongodb_data\dev_db_sharding_svr_auth\shd2

if not exist %route_dir%\ set /a dir_route=1

if %dir_route% == 1 md %route_dir%

call sharding_svr_repl_auth_tpl.bat %cfg_dir% 127.0.0.1 12201 12202 12203 rscfg0 --configsvr %keyfile%

call sharding_svr_repl_auth_tpl.bat %shd0_dir% 127.0.0.1 12211 12212 12213 rsshd0 --shardsvr %keyfile%
call sharding_svr_repl_auth_tpl.bat %shd1_dir% 127.0.0.1 12214 12215 12216 rsshd1 --shardsvr %keyfile%
call sharding_svr_repl_auth_tpl.bat %shd2_dir% 127.0.0.1 12217 12218 12219 rsshd2 --shardsvr %keyfile%

echo "wait 10 second of replicset init..."
timeout /t 10 /nobreak > nul

start mongos ^
		--configdb "rscfg0/127.0.0.1:12201,127.0.0.1:12202,127.0.0.1:12203" ^
		--logpath %route_dir%\route_log.txt ^
		--bind_ip_all ^
		--port 12198 ^
		--logappend ^
		--keyFile %keyfile% ^
		--clusterAuthMode keyFile

if %dir_route% == 1 goto lab_init

goto lab_end

:lab_init
	set mongos_addr="mongodb://xy:123456abc@127.0.0.1:12198/?tls=false"
	set rsshd0_addr=rsshd0/127.0.0.1:12211,127.0.0.1:12212,127.0.0.1:12213
	set rsshd1_addr=rsshd1/127.0.0.1:12214,127.0.0.1:12215,127.0.0.1:12216
	set rsshd2_addr=rsshd2/127.0.0.1:12217,127.0.0.1:12218,127.0.0.1:12219

	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh ^
		--nodb ^
		--eval "var mongos_uri = '%mongos_addr%', rsshd0_addr = '%rsshd0_addr%', rsshd1_addr = '%rsshd1_addr%', rsshd2_addr = '%rsshd2_addr%';" ^
		init_mongodb_sharding.js

:lab_end
