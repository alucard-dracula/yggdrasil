
#!/bin/sh

svr_pem=~/mongodb_data/cert/server.pem
clt_pem=~/mongodb_data/cert/client.pem
ca_pem=~/mongodb_data/cert/ca.pem

mongos_uri="mongodb://127.0.0.1:12298/?tls=true&tlsCertificateKeyFile=${clt_pem}&tlsCAFile=${ca_pem}"
rsshd0_addr=rsshd0/127.0.0.1:12311,127.0.0.1:12312,127.0.0.1:12313
rsshd1_addr=rsshd1/127.0.0.1:12314,127.0.0.1:12315,127.0.0.1:12316
rsshd2_addr=rsshd2/127.0.0.1:12317,127.0.0.1:12318,127.0.0.1:12319

echo "wait 5 second"
sleep 5
echo ${mongos_uri}

mongosh \
	--nodb \
	--eval "var mongos_uri = '${mongos_uri}', rsshd0_addr = '${rsshd0_addr}', rsshd1_addr = '${rsshd1_addr}', rsshd2_addr = '${rsshd2_addr}';" \
	init_mongodb_sharding.js
