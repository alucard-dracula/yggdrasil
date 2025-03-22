
@echo off

cd /d %~dp0

set clt_pem=.\cert\client.pem
set ca_pem=.\cert\ca.pem

mongosh "mongodb://xy:123456abc@127.0.0.1:11198/?replicaSet=rs0"
