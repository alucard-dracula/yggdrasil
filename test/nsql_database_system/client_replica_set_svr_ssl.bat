
@echo off

cd /d %~dp0

set clt_pem=.\cert\client.pem
set ca_pem=.\cert\ca.pem

mongosh ^
	"mongodb://127.0.0.1:11298/?replicaSet=rs0&tls=true" ^
	--norc ^
	--tls ^
	--tlsCertificateKeyFile %clt_pem% ^
	--tlsCAFile %ca_pem%
