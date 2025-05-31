
#!/bin/sh

data_dir1=~/mongodb_data/dev_db_replica_set_svr_ssl_auth/rep1/data
data_dir2=~/mongodb_data/dev_db_replica_set_svr_ssl_auth/rep2/data
data_dir3=~/mongodb_data/dev_db_replica_set_svr_ssl_auth/rep3/data

log_file1=~/mongodb_data/dev_db_replica_set_svr_ssl_auth/rep1/data_log.txt
log_file2=~/mongodb_data/dev_db_replica_set_svr_ssl_auth/rep2/data_log.txt
log_file3=~/mongodb_data/dev_db_replica_set_svr_ssl_auth/rep3/data_log.txt

svr_pem=~/mongodb_data/cert/server.pem
ca_pem=~/mongodb_data/cert/ca.pem

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

if [ ${need_init} == 1 ];then
	mongod --dbpath ${data_dir1} --logpath ${log_file1} --replSet rs0 --port 11398 --logappend &
	mongod --dbpath ${data_dir2} --logpath ${log_file2} --replSet rs0 --port 11399 --logappend &
	mongod --dbpath ${data_dir3} --logpath ${log_file3} --replSet rs0 --port 11400 --logappend &

	echo "wait 5 second"
	sleep 5
	mongosh \
		--nodb \
		--eval "var in_arg_addr_port1 = '127.0.0.1:11398', in_arg_addr_port2 = '127.0.0.1:11399', in_arg_addr_port3 = '127.0.0.1:11400', rs_name = 'rs0';" \
		init_mongodb_replset.js

	echo "wait 10 second"
	sleep 10
	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11398/?replicaSet=rs0&tls=false', arg_shutdown = true;" init_mongodb_auth.js

	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11398/?tls=false', arg_force = true;" shutdown_mongodb.js
	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11399/?tls=false', arg_force = true;" shutdown_mongodb.js
	mongosh --nodb --eval "var arg_uri = 'mongodb://127.0.0.1:11400/?tls=false', arg_force = true;" shutdown_mongodb.js

	echo "wait 20 second exit config"
	sleep 20
fi

mongod \
	--dbpath ${data_dir1} \
	--logpath ${log_file1} \
	--replSet rs0 \
	--bind_ip_all \
	--port 11398 \
	--logappend \
	--tlsMode requireTLS \
	--tlsCertificateKeyFile ${svr_pem} \
	--tlsCAFile ${ca_pem} \
	--tlsClusterFile ${svr_pem} \
	--clusterAuthMode x509 \
	--auth &

mongod \
	--dbpath ${data_dir2} \
	--logpath ${log_file2} \
	--replSet rs0 \
	--bind_ip_all \
	--port 11399 \
	--logappend \
	--tlsMode requireTLS \
	--tlsCertificateKeyFile ${svr_pem} \
	--tlsCAFile ${ca_pem} \
	--tlsClusterFile ${svr_pem} \
	--clusterAuthMode x509 \
	--auth &

mongod \
	--dbpath ${data_dir3} \
	--logpath ${log_file3} \
	--replSet rs0 \
	--bind_ip_all \
	--port 11400 \
	--logappend \
	--tlsMode requireTLS \
	--tlsCertificateKeyFile ${svr_pem} \
	--tlsCAFile ${ca_pem} \
	--tlsClusterFile ${svr_pem} \
	--clusterAuthMode x509 \
	--auth &
	