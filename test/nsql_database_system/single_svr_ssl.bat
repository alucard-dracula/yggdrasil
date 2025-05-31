
@echo off

cd /d %~dp0

set data_dir=g:\mongodb_data\dev_db_single_svr_ssl\data
set log_file=g:\mongodb_data\dev_db_single_svr_ssl\data_log.txt

set svr_pem=g:\mongodb_data\cert\server.pem
set ca_pem=g:\mongodb_data\cert\ca.pem

if not exist %data_dir% md %log_file%

start ^
	mongod ^
		--dbpath %data_dir% ^
		--logpath %log_file% ^
		--bind_ip_all ^
		--port 10298 ^
		--logappend ^
		--tlsMode requireTLS ^
		--tlsCertificateKeyFile %svr_pem% ^
		--tlsCAFile %ca_pem%

