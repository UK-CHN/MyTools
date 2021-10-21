#!/bin/bash
DELAY=3
set -x
sudo service networking stop
sleep $DELAY
sudo service network-manager stop
sleep $DELAY
sudo service networking start
sleep $DELAY
sudo service network-manager start
set +x
