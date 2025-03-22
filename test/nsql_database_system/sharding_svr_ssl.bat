
@echo off
cd /d %~dp0

set svr_pem=g:/mongodb_data/cert/server.pem
set clt_pem=g:/mongodb_data/cert/client.pem
set ca_pem=g:/mongodb_data/cert/ca.pem

set /a dir_route = 0

set cfg_dir=g:\mongodb_data\dev_db_sharding_svr_ssl\config

set route_dir=g:\mongodb_data\dev_db_sharding_svr_ssl\route

set shd0_dir=g:\mongodb_data\dev_db_sharding_svr_ssl\shd0
set shd1_dir=g:\mongodb_data\dev_db_sharding_svr_ssl\shd1
set shd2_dir=g:\mongodb_data\dev_db_sharding_svr_ssl\shd2

if not exist %route_dir%\ set /a dir_route=1

if %dir_route% == 1 md %route_dir%

call sharding_svr_repl_ssl_tpl.bat %cfg_dir% 127.0.0.1 12301 12302 12303 rscfg0 --configsvr %svr_pem% %ca_pem%

call sharding_svr_repl_ssl_tpl.bat %shd0_dir% 127.0.0.1 12311 12312 12313 rsshd0 --shardsvr %svr_pem% %ca_pem%
call sharding_svr_repl_ssl_tpl.bat %shd1_dir% 127.0.0.1 12314 12315 12316 rsshd1 --shardsvr %svr_pem% %ca_pem%
call sharding_svr_repl_ssl_tpl.bat %shd2_dir% 127.0.0.1 12317 12318 12319 rsshd2 --shardsvr %svr_pem% %ca_pem%

echo "wait 10 second of replicset init..."
timeout /t 10 /nobreak > nul

start mongos ^
		--configdb "rscfg0/127.0.0.1:12301,127.0.0.1:12302,127.0.0.1:12303" ^
		--logpath %route_dir%\route_log.txt ^
		--bind_ip 127.0.0.1 ^
		--port 12298 ^
		--logappend ^
		--tlsMode requireTLS ^
		--tlsCertificateKeyFile %svr_pem% ^
		--tlsCAFile %ca_pem% ^
		--tlsClusterFile %clt_pem% ^
		--tlsClusterCAFile %ca_pem%

if %dir_route% == 1 goto lab_init

goto lab_end

:lab_init
	set mongos_uri="mongodb://127.0.0.1:12298/?tls=true^&tlsCertificateKeyFile=%clt_pem%^&tlsCAFile=%ca_pem%"
	set rsshd0_addr=rsshd0/127.0.0.1:12311,127.0.0.1:12312,127.0.0.1:12313
	set rsshd1_addr=rsshd1/127.0.0.1:12314,127.0.0.1:12315,127.0.0.1:12316
	set rsshd2_addr=rsshd2/127.0.0.1:12317,127.0.0.1:12318,127.0.0.1:12319

	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh ^
		--nodb ^
		--eval "var mongos_uri = '%mongos_uri%', rsshd0_addr = '%rsshd0_addr%', rsshd1_addr = '%rsshd1_addr%', rsshd2_addr = '%rsshd2_addr%';" ^
		init_mongodb_sharding.js

:lab_end

