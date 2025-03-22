
@echo off
cd /d %~dp0

set svr_pem=g:/mongodb_data/cert/server.pem
set clt_pem=g:/mongodb_data/cert/client.pem
set ca_pem=g:/mongodb_data/cert/ca.pem

set mongos_addr="mongodb://127.0.0.1:12298/?tls=true^&tlsCertificateKeyFile=%clt_pem%^&tlsCAFile=%ca_pem%"
set rsshd0_addr=rsshd0/127.0.0.1:12311,127.0.0.1:12312,127.0.0.1:12313
set rsshd1_addr=rsshd1/127.0.0.1:12314,127.0.0.1:12315,127.0.0.1:12316
set rsshd2_addr=rsshd2/127.0.0.1:12317,127.0.0.1:12318,127.0.0.1:12319

echo "wait 5 second"
:rem timeout /t 5 /nobreak > nul
echo %mongos_addr%

mongosh ^
	--nodb ^
	--eval "var mongos_uri = '%mongos_addr%', rsshd0_addr = '%rsshd0_addr%', rsshd1_addr = '%rsshd1_addr%', rsshd2_addr = '%rsshd2_addr%';" ^
	init_mongodb_sharding.js
