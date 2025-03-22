#!/bin/sh

data_dir=~/mongodb_data/dev_db_single_svr/data
log_file=~/mongodb_data/dev_db_single_svr/data_log.txt

if [ ! -d ${data_dir} ];then
	mkdir -p ${data_dir}
fi

mongod --dbpath ${data_dir} --logpath ${log_file} --port 10098 --logappend &
