
@echo off
cd /d %~dp0

set svr_pem=g:/mongodb_data/cert/server.pem
set clt_pem=g:/mongodb_data/cert/client.pem
set ca_pem=g:/mongodb_data/cert/ca.pem

set /a dir_route = 0

set cfg_dir=g:\mongodb_data\dev_db_sharding_svr_ssl_auth\config

set route_dir=g:\mongodb_data\dev_db_sharding_svr_ssl_auth\route

set shd0_dir=g:\mongodb_data\dev_db_sharding_svr_ssl_auth\shd0
set shd1_dir=g:\mongodb_data\dev_db_sharding_svr_ssl_auth\shd1
set shd2_dir=g:\mongodb_data\dev_db_sharding_svr_ssl_auth\shd2

if not exist %route_dir%\ set /a dir_route=1

if %dir_route% == 1 md %route_dir%

call sharding_svr_repl_ssl_auth_tpl.bat %cfg_dir% 127.0.0.1 12401 12402 12403 rscfg0 --configsvr %svr_pem% %ca_pem%

call sharding_svr_repl_ssl_auth_tpl.bat %shd0_dir% 127.0.0.1 12411 12412 12413 rsshd0 --shardsvr %svr_pem% %ca_pem%
call sharding_svr_repl_ssl_auth_tpl.bat %shd1_dir% 127.0.0.1 12414 12415 12416 rsshd1 --shardsvr %svr_pem% %ca_pem%
call sharding_svr_repl_ssl_auth_tpl.bat %shd2_dir% 127.0.0.1 12417 12418 12419 rsshd2 --shardsvr %svr_pem% %ca_pem%

echo "wait 10 second of replicset init..."
timeout /t 10 /nobreak > nul

start mongos ^
		--configdb "rscfg0/127.0.0.1:12401,127.0.0.1:12402,127.0.0.1:12403" ^
		--logpath %route_dir%\route_log.txt ^
		--bind_ip 127.0.0.1 ^
		--port 12398 ^
		--tlsMode requireTLS ^
		--tlsCertificateKeyFile %svr_pem% ^
		--tlsCAFile %ca_pem% ^
		--tlsClusterCAFile %ca_pem% ^
		--tlsClusterFile %svr_pem% ^
		--clusterAuthMode x509 

if %dir_route% == 1 goto lab_init

goto lab_end

:lab_init
	set mongos_addr="mongodb://xy:123456abc@127.0.0.1:12398/?tls=true^&tlsCertificateKeyFile=%clt_pem%^&tlsCAFile=%ca_pem%"
	set rsshd0_addr=rsshd0/127.0.0.1:12411,127.0.0.1:12412,127.0.0.1:12413
	set rsshd1_addr=rsshd1/127.0.0.1:12414,127.0.0.1:12415,127.0.0.1:12416
	set rsshd2_addr=rsshd2/127.0.0.1:12417,127.0.0.1:12418,127.0.0.1:12419

	echo "wait 5 second"
	timeout /t 5 /nobreak > nul

	mongosh ^
		--nodb ^
		--eval "var mongos_uri = '%mongos_addr%', rsshd0_addr = '%rsshd0_addr%', rsshd1_addr = '%rsshd1_addr%', rsshd2_addr = '%rsshd2_addr%';" ^
		init_mongodb_sharding.js

:lab_end

