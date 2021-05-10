#!/bin/bash
file=`find . -type f -name "*.py"`
l=${#file}
file=${file:2:l}
p=`pwd`
echo "$p"
export FLASK_APP="$p/$file"
echo "--RUNNING ${FLASK_APP}--"
sudo -E flask run --host=0.0.0.0 --port=80
