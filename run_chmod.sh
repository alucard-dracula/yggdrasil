#!/bin/sh

find ./ -type d -exec chmod a+rx {} \;
find ./ -type f -exec chmod a-x {} \;
find ./ -type f -exec chmod go-w {} \;
