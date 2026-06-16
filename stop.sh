#!/bin/bash

systemctl stop war.service
systemctl disable war.service
rm -rf /etc/systemd/system/war.service