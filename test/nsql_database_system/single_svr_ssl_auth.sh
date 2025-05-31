#!/bin/sh

data_dir=~/mongodb_data/dev_db_single_svr_ssl_auth/data
log_file=~/mongodb_data/dev_db_single_svr_ssl_auth/data_log.txt

svr_pem=~/mongodb_data/cert/server.pem
ca_pem=~/mongodb_data/cert/ca.pem

if [ ! -d ${data_dir} ];then
	mkdir -p ${data_dir}
	mongod --dbpath ${data_dir} --logpath ${log_file} --port 10398 &
	echo "wait 5 second"
	sleep 5
	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:10398/?tls=false', arg_shutdown = true;" init_mongodb_auth.js
	echo "wait 5 second"
	sleep 5
fi

mongod \
	--dbpath ${data_dir}  \
	--logpath ${log_file} \
	--bind_ip_all \
	--port 10398 \
	--auth \
	--logappend \
	--tlsMode requireTLS \
	--tlsCertificateKeyFile ${svr_pem} \
	--tlsCAFile ${ca_pem} &

