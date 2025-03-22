#!/bin/sh

clt_pem=./cert/client.pem
ca_pem=./cert/ca.pem

mongosh "mongodb://127.0.0.1:11098/?replicaSet=rs0" 
