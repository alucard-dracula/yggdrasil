#!/bin/sh

clt_pem=./cert/client.pem
ca_pem=./cert/ca.pem

mongosh \
	"mongodb://xy:123456abc@127.0.0.1:12398/?tls=true" \
	--norc \
	--tls \
	--tlsCertificateKeyFile ${clt_pem} \
	--tlsCAFile ${ca_pem}

