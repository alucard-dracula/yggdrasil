#!/bin/sh

cfg_dir=~/mongodb_data/dev_db_sharding_svr/config
route_dir=~/mongodb_data/dev_db_sharding_svr/route

shd0_dir=~/mongodb_data/dev_db_sharding_svr/shd0
shd1_dir=~/mongodb_data/dev_db_sharding_svr/shd1
shd2_dir=~/mongodb_data/dev_db_sharding_svr/shd3

need_init=0

# data_dir fix
if [ ! -d ${route_dir} ]; then
	mkdir -p ${route_dir}
	need_init=1
fi

sh sharding_svr_repl_tpl.sh ${cfg_dir} 127.0.0.1 12101 12102 12103 rscfg0 --configsvr

sh sharding_svr_repl_tpl.sh ${shd0_dir} 127.0.0.1 12111 12112 12113 rsshd0 --shardsvr
sh sharding_svr_repl_tpl.sh ${shd1_dir} 127.0.0.1 12114 12115 12116 rsshd1 --shardsvr
sh sharding_svr_repl_tpl.sh ${shd2_dir} 127.0.0.1 12117 12118 12119 rsshd2 --shardsvr

echo "wait 10 second of replicset init..."
sleep 10

echo "start mongos"

mongos \
	--configdb "rscfg0/127.0.0.1:12101,127.0.0.1:12102,127.0.0.1:12103" \
	--logpath ${route_dir}/route_log.txt \
	--bind_ip 127.0.0.1 \
	--port 12098 \
	--logappend &

if [ ${need_init} == 1 ]; then
	mongos_uri=mongodb://127.0.0.1:12098
	rsshd0_addr=rsshd0/127.0.0.1:12111,127.0.0.1:12112,127.0.0.1:12113
	rsshd1_addr=rsshd1/127.0.0.1:12114,127.0.0.1:12115,127.0.0.1:12116
	rsshd2_addr=rsshd2/127.0.0.1:12117,127.0.0.1:12118,127.0.0.1:12119

	echo "wait 5 second"
	sleep 5

	mongosh \
		--nodb \
		--eval "var mongos_uri = '${mongos_uri}', rsshd0_addr = '${rsshd0_addr}', rsshd1_addr = '${rsshd1_addr}', rsshd2_addr = '${rsshd2_addr}';" \
		init_mongodb_sharding.js
fi
