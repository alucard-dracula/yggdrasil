#!/bin/sh

data_dir=~/mongodb_data/dev_db_single_svr_ssl/data
log_file=~/mongodb_data/dev_db_single_svr_ssl/data_log.txt

svr_pem=~/mongodb_data/cert/server.pem
ca_pem=~/mongodb_data/cert/ca.pem

if [ -f ${log_file} ];then
	rm -f ${log_file}
fi

if [ ! -d ${data_dir} ];then
	mkdir -p ${data_dir}
fi

mongod \
	--dbpath ${data_dir} \
	--logpath ${log_file} \
	--bind_ip 127.0.0.1 \
	--port 10298 \
	--logappend \
	--tlsMode requireTLS \
	--tlsCertificateKeyFile ${svr_pem} \
	--tlsCAFile ${ca_pem} &
