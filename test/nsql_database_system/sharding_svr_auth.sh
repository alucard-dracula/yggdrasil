#!/bin/sh

cfg_dir=~/mongodb_data/dev_db_sharding_svr_auth/config
route_dir=~/mongodb_data/dev_db_sharding_svr_auth/route

shd0_dir=~/mongodb_data/dev_db_sharding_svr_auth/shd0
shd1_dir=~/mongodb_data/dev_db_sharding_svr_auth/shd1
shd2_dir=~/mongodb_data/dev_db_sharding_svr_auth/shd3

keyfile=~/mongodb_data/keyfile/replica_set_key.rsk

need_init=0

# data_dir fix
if [ ! -d ${route_dir} ]; then
	mkdir -p ${route_dir}
	chmod 400 ${keyfile}
	need_init=1
fi

sh sharding_svr_repl_auth_tpl.sh ${cfg_dir} 127.0.0.1 12201 12202 12203 rscfg0 --configsvr ${keyfile}

sh sharding_svr_repl_auth_tpl.sh ${shd0_dir} 127.0.0.1 12211 12212 12213 rsshd0 --shardsvr ${keyfile}
sh sharding_svr_repl_auth_tpl.sh ${shd1_dir} 127.0.0.1 12214 12215 12216 rsshd1 --shardsvr ${keyfile}
sh sharding_svr_repl_auth_tpl.sh ${shd2_dir} 127.0.0.1 12217 12218 12219 rsshd2 --shardsvr ${keyfile}

echo "wait 10 second of replicset init..."
sleep 10

echo "start mongos"

mongos \
	--configdb "rscfg0/127.0.0.1:12201,127.0.0.1:12202,127.0.0.1:12203" \
	--logpath ${route_dir}/route_log.txt \
	--bind_ip_all \
	--port 12198 \
	--logappend \
	--keyFile ${keyfile} \
	--clusterAuthMode keyFile &

if [ ${need_init} == 1 ]; then
	mongos_uri="mongodb://xy:123456abc@127.0.0.1:12198/?tls=false"
	rsshd0_addr=rsshd0/127.0.0.1:12211,127.0.0.1:12212,127.0.0.1:12213
	rsshd1_addr=rsshd1/127.0.0.1:12214,127.0.0.1:12215,127.0.0.1:12216
	rsshd2_addr=rsshd2/127.0.0.1:12217,127.0.0.1:12218,127.0.0.1:12219

	echo "wait 5 second"
	sleep 5

	mongosh \
		--nodb \
		--eval "var mongos_uri = '${mongos_uri}', rsshd0_addr = '${rsshd0_addr}', rsshd1_addr = '${rsshd1_addr}', rsshd2_addr = '${rsshd2_addr}';" \
		init_mongodb_sharding.js
fi
