
#!/bin/sh

cfg_dir=~/mongodb_data/dev_db_sharding_svr_ssl/config
route_dir=~/mongodb_data/dev_db_sharding_svr_ssl/route

shd0_dir=~/mongodb_data/dev_db_sharding_svr_ssl/shd0
shd1_dir=~/mongodb_data/dev_db_sharding_svr_ssl/shd1
shd2_dir=~/mongodb_data/dev_db_sharding_svr_ssl/shd3

svr_pem=~/mongodb_data/cert/server.pem
clt_pem=~/mongodb_data/cert/client.pem
ca_pem=~/mongodb_data/cert/ca.pem

need_init=0

# data_dir fix
if [ ! -d ${route_dir} ]; then
	mkdir -p ${route_dir}
	need_init=1
fi

sh sharding_svr_repl_ssl_tpl.sh ${cfg_dir} 127.0.0.1 12301 12302 12303 rscfg0 --configsvr ${svr_pem} ${ca_pem}

sh sharding_svr_repl_ssl_tpl.sh ${shd0_dir} 127.0.0.1 12311 12312 12313 rsshd0 --shardsvr ${svr_pem} ${ca_pem}
sh sharding_svr_repl_ssl_tpl.sh ${shd1_dir} 127.0.0.1 12314 12315 12316 rsshd1 --shardsvr ${svr_pem} ${ca_pem}
sh sharding_svr_repl_ssl_tpl.sh ${shd2_dir} 127.0.0.1 12317 12318 12319 rsshd2 --shardsvr ${svr_pem} ${ca_pem}

echo "wait 10 second of replicset init..."
sleep 10

echo "start mongos"

mongos \
	--configdb "rscfg0/127.0.0.1:12301,127.0.0.1:12302,127.0.0.1:12303" \
	--logpath ${route_dir}/route_log.txt \
	--bind_ip_all \
	--port 12298 \
	--logappend \
	-tlsMode requireTLS \
	--tlsCertificateKeyFile ${svr_pem} \
	--tlsCAFile ${ca_pem} \
	--tlsClusterFile ${clt_pem} \
	--tlsClusterCAFile ${ca_pem} &

if [ ${need_init} == 1 ]; then
	mongos_uri="mongodb://127.0.0.1:12298/?tls=true&tlsCertificateKeyFile=${clt_pem}&tlsCAFile=${ca_pem}"
	rsshd0_addr=rsshd0/127.0.0.1:12311,127.0.0.1:12312,127.0.0.1:12313
	rsshd1_addr=rsshd1/127.0.0.1:12314,127.0.0.1:12315,127.0.0.1:12316
	rsshd2_addr=rsshd2/127.0.0.1:12317,127.0.0.1:12318,127.0.0.1:12319

	echo "wait 5 second"
	sleep 5

	mongosh \
		--nodb \
		--eval "var mongos_uri = '${mongos_uri}', rsshd0_addr = '${rsshd0_addr}', rsshd1_addr = '${rsshd1_addr}', rsshd2_addr = '${rsshd2_addr}';" \
		init_mongodb_sharding.js
fi
