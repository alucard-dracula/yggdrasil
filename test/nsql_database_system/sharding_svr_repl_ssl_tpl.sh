#!/bin/sh

data_root_dir=$1

addr=$2
port1=$3
port2=$4
port3=$5

rep_name=$6
svr_t=$7

svr_pem=$8
ca_pem=$9

addr_port1=${addr}:${port1}
addr_port2=${addr}:${port2}
addr_port3=${addr}:${port3}

data_dir1=${data_root_dir}/rep1/data
data_dir2=${data_root_dir}/rep2/data
data_dir3=${data_root_dir}/rep3/data

log_file1=${data_root_dir}/rep1/log_file.txt
log_file2=${data_root_dir}/rep2/log_file.txt
log_file3=${data_root_dir}/rep3/log_file.txt

need_init=0

if [ ! -d ${data_dir1} ]; then
	mkdir -p ${data_dir1}
	need_init=1
fi

if [ ! -d ${data_dir2} ]; then
	mkdir -p ${data_dir2}
	need_init=1
fi

if [ ! -d ${data_dir3} ]; then
	mkdir -p ${data_dir3}
	need_init=1
fi

if [ ${need_init} == 1 ]; then

	mongod ${svr_t} --dbpath ${data_dir1} --logpath ${log_file1} --replSet ${rep_name} --bind_ip=${addr} --port ${port1} --logappend &
	mongod ${svr_t} --dbpath ${data_dir2} --logpath ${log_file2} --replSet ${rep_name} --bind_ip=${addr} --port ${port2} --logappend &
	mongod ${svr_t} --dbpath ${data_dir3} --logpath ${log_file3} --replSet ${rep_name} --bind_ip=${addr} --port ${port3} --logappend &

	echo "wait 5 second"
	sleep 5
	
	mongosh \
		--nodb \
		--eval "var in_arg_addr_port1 = '${addr_port1}', in_arg_addr_port2 = '${addr_port2}', in_arg_addr_port3 = '${addr_port3}', rs_name = '${rep_name}';" \
		init_mongodb_replset.js

	echo "wait 20 second"
	sleep 20

	mongosh --nodb --eval "var arg_uri = 'mongodb://${addr_port1}/?tls=false', arg_force = true;" shutdown_mongodb.js
	mongosh --nodb --eval "var arg_uri = 'mongodb://${addr_port2}/?tls=false', arg_force = true;" shutdown_mongodb.js
	mongosh --nodb --eval "var arg_uri = 'mongodb://${addr_port3}/?tls=false', arg_force = true;" shutdown_mongodb.js

	echo "wait 10 second"
	sleep 10
fi

mongod \
	${svr_t} \
	--dbpath ${data_dir1} \
	--logpath ${log_file1} \
	--replSet ${rep_name} \
	--bind_ip ${addr} \
	--port ${port1} \
	--logappend \
	--tlsMode requireTLS \
	--tlsCertificateKeyFile ${svr_pem} \
	--tlsCAFile ${ca_pem} &

mongod \
	${svr_t} \
	--dbpath ${data_dir2} \
	--logpath ${log_file2} \
	--replSet ${rep_name} \
	--bind_ip ${addr} \
	--port ${port2} \
	--logappend \
	--tlsMode requireTLS \
	--tlsCertificateKeyFile ${svr_pem} \
	--tlsCAFile ${ca_pem} &

mongod \
	${svr_t} \
	--dbpath ${data_dir3} \
	--logpath ${log_file3} \
	--replSet ${rep_name} \
	--bind_ip ${addr} \
	--port ${port3} \
	--logappend \
	--tlsMode requireTLS \
	--tlsCertificateKeyFile ${svr_pem} \
	--tlsCAFile ${ca_pem} &

