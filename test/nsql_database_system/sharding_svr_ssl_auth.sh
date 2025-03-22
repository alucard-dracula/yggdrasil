#!/bin/sh

cfg_dir=~/mongodb_data/dev_db_sharding_svr_ssl_auth/config
route_dir=~/mongodb_data/dev_db_sharding_svr_ssl_auth/route

shd0_dir=~/mongodb_data/dev_db_sharding_svr_ssl_auth/shd0
shd1_dir=~/mongodb_data/dev_db_sharding_svr_ssl_auth/shd1
shd2_dir=~/mongodb_data/dev_db_sharding_svr_ssl_auth/shd3

svr_pem=~/mongodb_data/cert/server.pem
clt_pem=~/mongodb_data/cert/client.pem
ca_pem=~/mongodb_data/cert/ca.pem

need_init=0

# data_dir fix
if [ ! -d ${route_dir} ]; then
	mkdir -p ${route_dir}
	need_init=1
fi

sh sharding_svr_repl_ssl_auth_tpl.sh ${cfg_dir} 127.0.0.1 12401 12402 12403 rscfg0 --configsvr ${svr_pem} ${ca_pem}

sh sharding_svr_repl_ssl_auth_tpl.sh ${shd0_dir} 127.0.0.1 12411 12412 12413 rsshd0 --shardsvr ${svr_pem} ${ca_pem}
sh sharding_svr_repl_ssl_auth_tpl.sh ${shd1_dir} 127.0.0.1 12414 12415 12416 rsshd1 --shardsvr ${svr_pem} ${ca_pem}
sh sharding_svr_repl_ssl_auth_tpl.sh ${shd2_dir} 127.0.0.1 12417 12418 12419 rsshd2 --shardsvr ${svr_pem} ${ca_pem}

echo "wait 10 second of replicset init..."
sleep 10

echo "start mongos"

mongos \
	--configdb "rscfg0/127.0.0.1:12401,127.0.0.1:12402,127.0.0.1:12403" \
	--logpath ${route_dir}/route_log.txt \
	--bind_ip 127.0.0.1 \
	--port 12398 \
	--logappend \
	--tlsMode requireTLS \
	--tlsCertificateKeyFile ${svr_pem} \
	--tlsCAFile ${ca_pem} \
	--tlsClusterFile ${svr_pem} \
	--tlsClusterCAFile ${ca_pem} \
	--clusterAuthMode x509 &

if [ ${need_init} == 1 ]; then
	mongos_uri="mongodb://xy:123456abc@127.0.0.1:12398/?tls=true&tlsCertificateKeyFile=${clt_pem}&tlsCAFile=${ca_pem}"
	rsshd0_addr=rsshd0/127.0.0.1:12411,127.0.0.1:12412,127.0.0.1:12413
	rsshd1_addr=rsshd1/127.0.0.1:12414,127.0.0.1:12415,127.0.0.1:12416
	rsshd2_addr=rsshd2/127.0.0.1:12417,127.0.0.1:12418,127.0.0.1:12419

	echo "wait 5 second"
	sleep 5

	mongosh \
		--nodb \
		--eval "var mongos_uri = '${mongos_uri}', rsshd0_addr = '${rsshd0_addr}', rsshd1_addr = '${rsshd1_addr}', rsshd2_addr = '${rsshd2_addr}';" \
		init_mongodb_sharding.js
fi
