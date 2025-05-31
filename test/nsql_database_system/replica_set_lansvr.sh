#!/bin/sh

data_dir1=~/mongodb_data/dev_db_replica_set_lansvr/rep1/data
data_dir2=~/mongodb_data/dev_db_replica_set_lansvr/rep2/data
data_dir3=~/mongodb_data/dev_db_replica_set_lansvr/rep3/data

log_file1=~/mongodb_data/dev_db_replica_set_lansvr/rep1/data_log.txt
log_file2=~/mongodb_data/dev_db_replica_set_lansvr/rep2/data_log.txt
log_file3=~/mongodb_data/dev_db_replica_set_lansvr/rep3/data_log.txt

need_init=0

if [ ! -d ${data_dir1} ];then
	mkdir -p ${data_dir1}
	need_init=1
fi

if [ ! -d ${data_dir2} ];then
	mkdir -p ${data_dir2}
	need_init=1
fi

if [ ! -d ${data_dir3} ];then
	mkdir -p ${data_dir3}
	need_init=1
fi

mongod --dbpath ${data_dir1} --logpath ${log_file1} --replSet rs0 --bind_ip_all --port 11498 --logappend &
mongod --dbpath ${data_dir2} --logpath ${log_file2} --replSet rs0 --bind_ip_all --port 11499 --logappend &
mongod --dbpath ${data_dir3} --logpath ${log_file3} --replSet rs0 --bind_ip_all --port 11500 --logappend &

if [ ${need_init} == 1 ];then
	echo "wait 5 second"
	sleep 5
	
	mongosh \
		--nodb \
		--eval "var in_arg_addr_port1 = '192.168.1.101:11498', in_arg_addr_port2 = '192.168.1.101:11499', in_arg_addr_port3 = '192.168.1.101:11500', rs_name = 'rs0';" \
		init_mongodb_replset.js
fi

