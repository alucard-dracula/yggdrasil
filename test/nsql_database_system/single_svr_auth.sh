#!/bin/sh

data_dir=~/mongodb_data/dev_db_single_svr_auth/data
log_file=~/mongodb_data/dev_db_single_svr_auth/data_log.txt


if [ ! -d ${data_dir} ];then
	mkdir -p ${data_dir}
	mongod --dbpath ${data_dir} --logpath ${log_file} --port 10198 &
	echo "wait 5 second"
	sleep 5
	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:10198/?tls=false', arg_shutdown = true;" init_mongodb_auth.js
	echo "wait 5 second"
	sleep 5
fi

mongod --dbpath ${data_dir} --logpath ${log_file} --port 10198 --auth --logappend &
