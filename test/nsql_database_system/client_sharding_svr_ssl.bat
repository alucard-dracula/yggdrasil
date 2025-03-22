
@echo off

cd /d %~dp0

set clt_pem=./cert/client.pem
set ca_pem=./cert/ca.pem

mongosh ^
	--port 12298 ^
	--norc ^
	--tls ^
	--tlsCertificateKeyFile %clt_pem% ^
	--tlsCAFile %ca_pem%
